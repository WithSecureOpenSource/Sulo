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

#include "pin.H"

#include "FlashPlayerConfigBuilder.h"
#include "ASString.h"
#include "ASTraits.h"

class ASMethodInfo
{
public:
	static ASMethodInfo* create(UINT32* ptr);
	string getName();
	UINT32* getTraitsPtr();
	string getMethodNameWithTraits();
	UINT32 getFlags();
	UINT32 getMethodId();

	string getReturnValueType();
	const vector<string>* getParameterTypes();

private:
	ASMethodInfo(void);
	ASMethodInfo(UINT32*);
	ASMethodInfo(ASMethodInfo const&);
	void operator=(ASMethodInfo const&);

	void buildMethodSignature();

	Config* m_config;
	UINT32* m_methodInfo;
	
	string m_methodNameWithTraits;
	
	bool m_methodSignatureBuilt;
	string m_returnValueType;
	vector<string> m_parameterTypes;

	map<UINT32,UINT32> methodIdToMethodNameIndex;

	static map<UINT32*,ASMethodInfo*> m_methodInfoCache;

	static const UINT32 IS_GETTER = 0x100;
	static const UINT32 IS_SETTER = 0x200;
};

