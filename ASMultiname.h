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

class ASMultiname
{
public:

	static ASMultiname* create(UINT32* ptr);
	static string format(UINT32* ns, UINT32* name, bool hideNonPublicNamespaces=true);
	string getFormatted();

private:

	ASMultiname(void);
	ASMultiname(UINT32*);
	ASMultiname(ASMultiname const&);
	void operator=(ASMultiname const&);

	UINT32* getNamespacePtr();
	UINT32* getNamePtr();

	Config* m_config;
	UINT32* m_addr;

	static map<UINT32*,ASMultiname*> m_multinameCache;

	const static UINT32 NSSET = 0x10;
};

