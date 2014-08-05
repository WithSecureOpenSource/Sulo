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

#include "ASString.h"
#include "ASMethodInfo.h"
#include "CallTracerPlugin.h"
#include "Logger.h"

namespace WINDOWS
{
	#include <windows.h>
}


// Commandline options for call tracer plugin
extern KNOB<bool> KnobCallTracerEarlyTracing;
extern KNOB<string> KnobCallTracerTraceFile;


CallTracerPlugin::CallTracerPlugin() :
	m_traceLoggingEnabled(KnobCallTracerEarlyTracing.Value()),
	m_callDepth(0)
{
	m_traceFile.open(KnobCallTracerTraceFile.Value().c_str());
}


CallTracerPlugin::~CallTracerPlugin()
{
	m_traceFile.close();
}


bool CallTracerPlugin::beforeMethodCall(ASMethodInfo* mi, string methodName, UINT32 argc, UINT32* argv)
{
	// Save this so we can check the return value type
	m_mi = mi;

	m_traceFile << format("\n%s%s ()\n", string(m_callDepth, '\t').c_str(), methodName.c_str());

	// Is call trace logging (already) enabled?
	if (m_traceLoggingEnabled)
	{
		m_traceFile << format("%sthis: 0x%x\n", string(m_callDepth+1, '\t').c_str(), argv[0]);
		
		// Get parameter types
		const vector<string>* parameterTypes = mi->getParameterTypes();
		string paramType;

		// Print the actual arguments
		for (UINT32 i=0; i < argc; i++)
		{
			if (i >= parameterTypes->size())
			{
				paramType = "null";
			}
			else
			{
				paramType = parameterTypes->at(i);
			}

			if (paramType == "String")
			{
				ASString* s = ASString::create((UINT32*)argv[i+1]);
				if (s != NULL)
				{
					m_traceFile << format("%sarg %d: \"%s\"\n", string(m_callDepth+1, '\t').c_str(), i, s->getString().c_str());
					continue;
				}
			}

			m_traceFile << format("%sarg %d: 0x%x : %s\n", string(m_callDepth+1, '\t').c_str(), i, argv[i+1], paramType.c_str());
		}
	}
	else if (!m_traceLoggingEnabled && methodName == "flash.display::Sprite/constructChildren")
	{
		// We can start logging from the next method call
		m_traceLoggingEnabled = true;
	}

	m_callDepth++;
	return false;
}


void CallTracerPlugin::afterMethodCall(UINT32 retVal)
{
	m_callDepth--;

	string type = m_mi->getReturnValueType();

	if (type == "String")
	{
		ASString* s = ASString::create((UINT32*)retVal);
		if (s != NULL)
		{
			m_traceFile << format("%sReturned: \"%s\" (call depth: %d)\n", string(m_callDepth, '\t').c_str(), s->getString().c_str(), m_callDepth);
			return;
		}
	}

	m_traceFile << format("%sReturned: 0x%x : %s (call depth: %d)\n", string(m_callDepth, '\t').c_str(), retVal, type.c_str(), m_callDepth);
}


string CallTracerPlugin::format(char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	int len = vsnprintf(NULL, 0, fmt, args) + 1;
	char* buf = (char*)malloc(len);
		
	vsnprintf(buf, len-1, fmt, args);
	buf[len-1] = 0;
		
	string s(buf);

	free(buf);
	va_end(args);

	return s;
}