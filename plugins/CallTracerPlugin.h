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

#pragma once

#include "ISuloPlugin.h"
#include "pin.H"

#include <fstream>
#include <map>

class ASMethodInfo;

class CallTracerPlugin : public ISuloPlugin
{
public:
	CallTracerPlugin();
	~CallTracerPlugin();

	bool beforeMethodCall(ASMethodInfo* mi, string methodName, UINT32 argc, UINT32* argv);
	void afterMethodCall(UINT32 retVal);

private:

	string format(char* fmt, ...);

	typedef struct CallEntry {
		string name;
		string params;
		string retval;
		vector<CallEntry*>* children;
	} CallEntry;

	// Log call trace to this stream
	ofstream m_traceFile;

	// This flag is used to prevent the logging of uninteresting method calls before the actual Flash file runs
	bool m_traceLoggingEnabled;

	ASMethodInfo* m_mi;
	int m_callDepth;
	map<int,vector<vector<CallEntry*>*>*> levelToList;
};

