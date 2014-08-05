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

#include "ASMultiname.h"
#include "ASString.h"
#include "FlashPlayerConfigBuilder.h"
#include "Logger.h"

map<UINT32*,ASMultiname*> ASMultiname::m_multinameCache;

ASMultiname::ASMultiname(UINT32* ptr)
{
	m_config = FlashPlayerConfigBuilder::instance().getConfig();
	m_addr = ptr;
}


ASMultiname* ASMultiname::create(UINT32* ptr)
{
	if (m_multinameCache.find(ptr) == m_multinameCache.end())
	{
		m_multinameCache[ptr] = new ASMultiname(ptr);
	}

	return m_multinameCache[ptr];
}


string ASMultiname::format(UINT32* ns, UINT32* name, bool hideNonPublicNamespaces)
{
	Config* config = FlashPlayerConfigBuilder::instance().getConfig();

	UINT32* p_uri = (UINT32*)ns[config->uriOffsetInNamespace/4];

	ASString* as_str = ASString::create(name);
	string str_name = as_str->getString();

	// Lower 3 bytes zero (public) and String length == 0, or
	// non-public and we want to hide
	if (((((UINT32)p_uri & 7) == 0) && p_uri[4] == 0) || (hideNonPublicNamespaces && (((UINT32)p_uri & 7) != 0)))
	{
		return str_name;
	}
	else
	{
		p_uri = (UINT32*)((UINT32)p_uri & 0xfffffff8);

		as_str = ASString::create(p_uri);
		string str_ns = as_str->getString();

		return str_ns + "::" + str_name;
	}
}

UINT32* ASMultiname::getNamePtr()
{
	UINT32* p_name = (UINT32*)m_addr[0];
	return p_name;
}

UINT32* ASMultiname::getNamespacePtr()
{
	UINT32 multiname_flags = m_addr[2];
	UINT32* pNamespaceOrNamespaceSet = (UINT32*)m_addr[1];
	UINT32* pNamespace = pNamespaceOrNamespaceSet;

	// Namespace set?
	if ((multiname_flags & NSSET) == NSSET)
	{
		pNamespace = (UINT32*)pNamespaceOrNamespaceSet[m_config->namespacesOffsetInNamespaceSet/sizeof(UINT32)];
	}
	
	return pNamespace;
}


string ASMultiname::getFormatted()
{
	return ASMultiname::format(getNamespacePtr(), getNamePtr());	
}