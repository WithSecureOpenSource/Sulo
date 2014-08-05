/*
Copyright 2014 F-Secure

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "ASMethodInfo.h"
#include "FlashPlayerConfigBuilder.h"
#include "Logger.h"
#include "SuloPluginManager.h"

#include "pin.h"

#include <iostream>
#include <map>


// Pin uses some base types that conflict with Windows types,
// see: https://software.intel.com/sites/landingpage/pintool/docs/49306/Pin/html/index.html#namespace
namespace WINDOWS
{
	#include <windows.h>
}


// Flash Player config: all RVAs, data structure offfsets, etc.
static Config* config = NULL;

// Map of addresses of methods that have been verified
static map<ADDRINT,bool> verified;

// Whether the return address should be instrumented
static map<ADDRINT,ADDRINT> returnAddressToInstument;

// Addresses that verified methods are known to return to
static map<ADDRINT,bool> knownVerifiedMethodReturnAddress;

// Whether to log the return value or not. The last item in the vector is the latest call, the first the oldest
static map<ADDRINT,vector<bool>*> logReturnValue;

// Fast mode uses these to store the return address that should be instrumented
ADDRINT interestingReturnAddress = NULL;
ADDRINT stackPtrInterestingReturnAddress = NULL;

// These are used to help PIN inline some of the analysis call.
// If the value is true, the method is *potential* for analyzing (i.e. can have false positives)
#define PRIME (99991)
static bool potentialVerifiedMethod[PRIME];
static bool potentialVerifiedMethodReturnAddress[PRIME];


// General commandline options
KNOB<bool> KnobFast(KNOB_MODE_WRITEONCE, "pintool", "fast", "false", "Runs plugins but no call trace generation");

// Commandline options for call tracer plugin
KNOB<bool> KnobCallTracerEarlyTracing(KNOB_MODE_WRITEONCE, "pintool", "early_tracing", "false", "Start logging before actual Flash code runs?");
KNOB<string> KnobCallTracerTraceFile(KNOB_MODE_WRITEONCE, "pintool", "tracefile", "calltrace.txt", "Call trace filename");
	
// Commandline options for Flash dumper plugin
KNOB<string> KnobFlashDumperFilePrefix(KNOB_MODE_WRITEONCE, "pintool", "flash_dump_prefix", "dumped", "Filename prefix for dumped Flash objects");

// Commandline options for SecureSWF plugin
KNOB<string> KnobSecureSWFMethodName(KNOB_MODE_WRITEONCE, "pintool", "secureswf", "", "Name of the string decryption method");


// Print help message
INT32 Usage()
{
    cerr << "This Pintool instruments Adobe Flash Player and logs calls to all ActionScript methods." << endl << endl;
    cerr << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}


// Mark the JITed the interpreted method as verified so we know to instrument it
VOID InterpretedMethodVerified(ADDRINT esi)
{
	UINT32* methodInfo = (UINT32*)esi;
	ADDRINT addr = methodInfo[config->invokerOffsetInMethodInfo/sizeof(UINT32)];
	verified[addr] = true;
	potentialVerifiedMethod[addr%PRIME] = true;
}


// Mark the JITed method as verified so we know to instrument it
VOID JITedMethodVerified(ADDRINT eax)
{
	// EAX holds the address of the method
	verified[eax] = true;
	potentialVerifiedMethod[eax%PRIME] = true;
}


// This can have false positives, i.e. call is actually not worth analyzing.
// PIN can inline this code, resulting in better performance
ADDRINT ShouldCallBeAnalyzed(UINT32* esp, ADDRINT ret_addr, ADDRINT dest)
{	
	return (potentialVerifiedMethod[dest%PRIME] + potentialVerifiedMethodReturnAddress[ret_addr%PRIME]);
}


VOID MethodCallAnalysisFast(UINT32* esp, ADDRINT ret_addr, ADDRINT dest)
{
	// Perform the necessary checks to filter out ShouldCallBeAnalyzed false positives
	if (verified.find(dest) == verified.end() && knownVerifiedMethodReturnAddress.find(ret_addr) == knownVerifiedMethodReturnAddress.end())
	{
		return;
	}

	UINT32* methodEnv = (UINT32*)esp[0];
	UINT32 argc = esp[1];
	UINT32* argv = (UINT32*)esp[2];

	UINT32* pMethodInfo = (UINT32*)methodEnv[config->methodInfoOffsetInMethodEnv/4];
	ASMethodInfo* mi = ASMethodInfo::create(pMethodInfo);
	string fullMethodName = mi->getMethodNameWithTraits();
	
	bool shouldLogReturnValue = SuloPluginManager::beforeMethodCall(mi, fullMethodName, argc, argv);
	if (shouldLogReturnValue)
	{
		interestingReturnAddress = ret_addr;
		stackPtrInterestingReturnAddress = (ADDRINT)esp;
	}
}


VOID MethodCallAnalysis(UINT32* esp, ADDRINT ret_addr, ADDRINT dest)
{
	// Perform the necessary checks to filter out ShouldCallBeAnalyzed false positives
	if (verified.find(dest) == verified.end() && knownVerifiedMethodReturnAddress.find(ret_addr) == knownVerifiedMethodReturnAddress.end())
	{
		return;
	}

	knownVerifiedMethodReturnAddress[ret_addr] = true;
	potentialVerifiedMethodReturnAddress[ret_addr%PRIME] = true;

	// This is not a call to verified method...
	if (verified.find(dest) == verified.end())
	{
		// ...but we'e seen calls to verified methods from this same address earlier,
		// so let's skip the return value
		if (logReturnValue.find(ret_addr) != logReturnValue.end())
		{
			if (logReturnValue[ret_addr] == NULL)
			{
				logReturnValue[ret_addr] = new vector<bool>;
			}
			logReturnValue[ret_addr]->push_back(false);
		}
		return;
	}

	if (logReturnValue[ret_addr] == NULL)
	{
		logReturnValue[ret_addr] = new vector<bool>;
	}
	logReturnValue[ret_addr]->push_back(true);

	UINT32* methodEnv = (UINT32*)esp[0];
	UINT32 argc = esp[1];
	UINT32* argv = (UINT32*)esp[2];

	UINT32* pMethodInfo = (UINT32*)methodEnv[config->methodInfoOffsetInMethodEnv/4];

	// Read/parse the full method name
	ASMethodInfo* mi = ASMethodInfo::create(pMethodInfo);
	string fullMethodName = mi->getMethodNameWithTraits();

	SuloPluginManager::beforeMethodCall(mi, fullMethodName, argc, argv);
}


ADDRINT ShouldReturnAddressBeAnalyzedFast(ADDRINT ret_addr)
{
	// This is NULL if we are not interested in logging the return value
	return interestingReturnAddress;
}


ADDRINT ShouldReturnAddressBeAnalyzed(ADDRINT ret_addr)
{
	if (logReturnValue.find(ret_addr) == logReturnValue.end())
	{
		return 0;
	}
	
	if (logReturnValue[ret_addr] == NULL)
	{
		return 0;
	}

	if (logReturnValue[ret_addr]->size() == 0)
	{
		return 0;
	}

	return 1;
}


VOID ReturnValueAnalysisFast(ADDRINT ret_addr, UINT32 eax, ADDRINT esp)
{
	if (ret_addr != interestingReturnAddress || esp != stackPtrInterestingReturnAddress)
	{
		return;
	}

	interestingReturnAddress = NULL;
	SuloPluginManager::afterMethodCall(eax);
}


VOID ReturnValueAnalysis(ADDRINT ret_addr, UINT32 eax, ADDRINT esp)
{
	bool log = logReturnValue[ret_addr]->back();

	if (log)
	{
		SuloPluginManager::afterMethodCall(eax);
	}
	
	logReturnValue[ret_addr]->pop_back();
}



// Pin calls this function before a code sequence is executed for the first time
VOID TraceCalls(INS ins, VOID *v)
{
	// If we don't have a proper config, we cannot instrument anything
	if (config == NULL)
	{
		return;
	}

	ADDRINT addr = INS_Address(ins);
	IMG img = IMG_FindByAddress(addr);
	
	// We are interested only calls from the JITted code. That code is not part of any valid image
	if (IMG_Valid(img) && img != config->img)
	{
		return;
	}

	// We don't know the origins of the calls (only the targets) so we need to instrument every call
	if (INS_IsCall(ins))
	{
		bool ok = false;
		
		if (INS_RegRContain(ins, REG_EAX) || INS_RegRContain(ins, REG_EDX))
		{
			ok = true;
		}
		else if (INS_IsDirectCall(ins))
		{
			ADDRINT target_addr = INS_DirectBranchOrCallTargetAddress(ins);
			IMG target_img = IMG_FindByAddress(target_addr);
			if (!IMG_Valid(img) || img == config->img)
			{
				ok = true;
			}
		}

		if (ok)
		{
			// Select which call analysis function to use depending on whether we are in fast mode
			AFUNPTR analysisFunc = (AFUNPTR)MethodCallAnalysis;
			if (KnobFast.Value())
			{
				analysisFunc = (AFUNPTR)MethodCallAnalysisFast;
			}

			ADDRINT ret_addr = INS_NextAddress(ins);
			INS_InsertIfCall(ins, IPOINT_BEFORE, (AFUNPTR)ShouldCallBeAnalyzed, IARG_FUNCARG_CALLSITE_REFERENCE, 0, IARG_ADDRINT, ret_addr, IARG_BRANCH_TARGET_ADDR, IARG_END);
			INS_InsertThenCall(ins, IPOINT_BEFORE, analysisFunc, IARG_FUNCARG_CALLSITE_REFERENCE, 0, IARG_ADDRINT, ret_addr, IARG_BRANCH_TARGET_ADDR, IARG_END);

			returnAddressToInstument[ret_addr] = true;
			return;
		}
	}

	if (returnAddressToInstument.find(addr) != returnAddressToInstument.end())
	{
		// Select which call analysis function to use depending on whether we are in fast mode
		AFUNPTR analysisFuncIf = (AFUNPTR)ShouldReturnAddressBeAnalyzed;
		AFUNPTR analysisFuncThen = (AFUNPTR)ReturnValueAnalysis;
		if (KnobFast.Value())
		{
			analysisFuncIf = (AFUNPTR)ShouldReturnAddressBeAnalyzedFast;
			analysisFuncThen = (AFUNPTR)ReturnValueAnalysisFast;
		}

		INS_InsertIfCall(ins, IPOINT_BEFORE, analysisFuncIf, IARG_ADDRINT, addr, IARG_END);
		INS_InsertThenCall(ins, IPOINT_BEFORE, analysisFuncThen, IARG_ADDRINT, addr, IARG_REG_VALUE, REG_EAX, IARG_FUNCARG_CALLSITE_REFERENCE, 0, IARG_END);
		return;
	}
}


// This routine is executed for each image
VOID ImageLoad(IMG img, VOID *v)
{
	if (!FlashPlayerConfigBuilder::instance().isSupportedFlashPlayer(img))
	{
		return;
	}
	
	LOGF("Found supported Flash Player image: %s (0x%x - 0x%x)\n", IMG_Name(img).c_str(), IMG_LowAddress(img), IMG_HighAddress(img));
	config = FlashPlayerConfigBuilder::instance().getConfig();

	// config->setInterpRVA needs to be chosen in such way that 
	// the value of _invoker can be found from [ESI+config->invinvokerOffsetInMethodInfookerOffset]
	RTN setInterp = RTN_CreateAt(config->loadOffset + config->setInterpRVA, "setInterp");
			
	// config->setInterpRVA needs to be chosen in such way that
	// the value of _implGPR can be found from EAX
	RTN verifyOnCall = RTN_CreateAt(config->loadOffset + config->verifyOnCallRVA, "verifyOnCall");
			
	if (setInterp == RTN_Invalid() || verifyOnCall == RTN_Invalid())
	{
		LOGF("Instrumenting Flash Player failed. Check setInterp and verifyOnCall RVAs in config.\n");
		return;
	}

	RTN_Open(setInterp);
	RTN_InsertCall(setInterp, IPOINT_BEFORE, (AFUNPTR)InterpretedMethodVerified, IARG_REG_VALUE, REG_ESI, IARG_END);
	RTN_Close(setInterp);

	RTN_Open(verifyOnCall);
	RTN_InsertCall(verifyOnCall, IPOINT_AFTER, (AFUNPTR)JITedMethodVerified, IARG_REG_VALUE, REG_EAX, IARG_END);
	RTN_Close(verifyOnCall);

	// Register TraceCalls to be called to instrument instructions
	INS_AddInstrumentFunction(TraceCalls, 0);
}


// This function is called when the instrumented application exits
VOID Fini(INT32 code, VOID *v)
{
	SuloPluginManager::uninit();
	LOGF("[END]\n");
}


int main(int argc, char* argv[])
{
	// This is needed for the RTN_* functions
	PIN_InitSymbols();

	// Initialize Pin
    if (PIN_Init(argc,argv))
    {
        return Usage();
    }

	// Now that we have access to commandline parameters, initialize the plugins
	SuloPluginManager::init();

    // Register ImageLoad to be called when an image is loaded
    IMG_AddInstrumentFunction(ImageLoad, 0);

	// Register Fini to be called when the application exists
	PIN_AddFiniFunction(Fini, 0);

	// Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}
