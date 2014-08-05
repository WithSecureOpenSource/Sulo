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

enum TraitsPosType
{
	TRAITSTYPE_INSTANCE = 0,
    TRAITSTYPE_CLASS = 1,
    TRAITSTYPE_SCRIPT = 2,
	TRAITSTYPE_CATCH = 3,
	TRAITSTYPE_ACTIVATION = 4,
	TRAITSTYPE_NVA = 5,
	TRAITSTYPE_RT = 6,
	TRAITSTYPE_INTERFACE = 7
};

class ASTraits
{
public:
	static ASTraits* create(UINT32* ptr);
	
	UINT32 methodIdToMethodNameIndex(UINT32 method_id);

	string getName();
	UINT32* getInit();
	UINT8 getPosType();

private:
	ASTraits(void);
	ASTraits(UINT32*);
	ASTraits(ASTraits const&);
	void operator=(ASTraits const&);

	Config* m_config;
	UINT32* m_traits;
	string m_name;
	bool m_methodIdToMethodNameIndexPopulated;
	map <UINT32,UINT32> m_methodIdToMethodNameIndex;

	static map<UINT32*,ASTraits*> m_traitsCache;
};

