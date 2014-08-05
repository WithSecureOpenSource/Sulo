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

#include <vector>

class ASMethodInfo;

class SuloPluginManager
{
public:
	static void init(void);
	static bool beforeMethodCall(ASMethodInfo* mi, string methodName, UINT32 argc, UINT32* argv);
	static void afterMethodCall(UINT32 retVal);
	static void uninit(void);

private:
	SuloPluginManager();
	SuloPluginManager(SuloPluginManager const&);
	void operator=(SuloPluginManager const&);

	static vector<ISuloPlugin*> m_plugins;
};
