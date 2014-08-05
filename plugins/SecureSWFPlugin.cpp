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

#include "SecureSWFPlugin.h"
#include "ASMethodInfo.h"
#include "ASString.h"
#include "Logger.h"

// General commandline options
extern KNOB<bool> KnobFast;

// Commandline options for call tracer plugin
extern KNOB<string> KnobSecureSWFMethodName;

SecureSWFPlugin::SecureSWFPlugin() :
	m_methodName(KnobSecureSWFMethodName.Value()),
	m_callDepth(0),
	m_log(false),
	m_fast(KnobFast.Value())
{
}


bool SecureSWFPlugin::beforeMethodCall(ASMethodInfo* mi, string methodName, UINT32 argc, UINT32* argv)
{
	// We need to call depth information only in normal mode
	if (!m_fast)
	{
		m_callDepth++;
	}

	if (methodName.find(m_methodName) != string::npos && argc == 1)
	{
		string returnValueType = mi->getReturnValueType();
		if (returnValueType == "String")
		{
			// Store the ID of the encrypted string so we can print it when
			// we get the decrypted string
			m_stringId = (int)argv[1];
			m_logRetValCallDepth = m_callDepth-1;
			m_log = true;
			return true;
		}
	}
	return false;
}


void SecureSWFPlugin::afterMethodCall(UINT32 retVal)
{
	// We need to call depth information only in normal mode
	if (!m_fast)
	{
		m_callDepth--;
	}

	if (!m_log)
	{
		return;
	}

	if (!m_fast && m_callDepth != m_logRetValCallDepth)
	{
		return;
	}

	m_log = false;

	ASString* s = ASString::create((UINT32*)retVal);
	if (s != NULL)
	{
		LOGF("[secureswf] String ID: %d, decrypted string: \"%s\"\n", m_stringId, s->getString().c_str());
	}
	return;
}
