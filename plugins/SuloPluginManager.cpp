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

#include "SuloPluginManager.h"
#include "CallTracerPlugin.h"
#include "FlashDumperPlugin.h"
#include "SecureSWFPlugin.h"

#include <iostream>

extern KNOB<bool> KnobFast;
extern KNOB<string> KnobSecureSWFMethodName;

vector<ISuloPlugin*> SuloPluginManager::m_plugins;

void SuloPluginManager::init(void)
{
	// Call tracer is disabled in fast mode
	if (!KnobFast.Value())
	{
		m_plugins.push_back(new CallTracerPlugin());
	}

	if (!KnobSecureSWFMethodName.Value().empty())
	{
		m_plugins.push_back(new SecureSWFPlugin());
	}

	m_plugins.push_back(new FlashDumperPlugin());
}


bool SuloPluginManager::beforeMethodCall(ASMethodInfo* mi, string methodName, UINT32 argc, UINT32* argv)
{
	bool logReturnAddress = false;
	for (std::vector<ISuloPlugin*>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
	{
		bool r = (*it)->beforeMethodCall(mi, methodName, argc, argv);
		if (r)
		{
			logReturnAddress = true;
		}
	}

	return logReturnAddress;
}


void SuloPluginManager::afterMethodCall(UINT32 retVal)
{
	for (std::vector<ISuloPlugin*>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
	{
		(*it)->afterMethodCall(retVal);
	}
}


void SuloPluginManager::uninit(void)
{
	for (std::vector<ISuloPlugin*>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
	{
		delete *it;
	}
}
