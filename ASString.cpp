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
#include "Logger.h"

ASString::ASString(void)
{
	
}

ASString::ASString(UINT32* ptr)
{
	m_tagged = ((UINT32)ptr & 0x7) != 0;
	m_addr = (UINT32*)((UINT32)ptr & 0xfffffff8);
	m_config = FlashPlayerConfigBuilder::instance().getConfig();
}


ASString* ASString::create(UINT32* ptr)
{
	if (!ASString::isString(ptr))
	{
		return NULL;
	}

	return new ASString(ptr);
}


bool ASString::isString(UINT32* addr)
{
	// Untag the pointer and try to read it
	addr = (UINT32*)((UINT32)addr & 0xfffffff8);
	Config* config = FlashPlayerConfigBuilder::instance().getConfig();

	if (!WINDOWS::IsBadReadPtr((const void*)addr, 16))
	{
		UINT32* obj = (UINT32*)addr;
		ADDRINT vtableAddr = (UINT32)obj[0];
		if (vtableAddr == (config->loadOffset + config->stringVTableRVA))
		{
			return true;
		}
	}

	return false;
}


UINT8* ASString::getPtrToBuf()
{
	return (UINT8*)m_addr[m_config->stringBufferOffset>>2];
}

string ASString::getString(bool verbose)
{
	if (!WINDOWS::IsBadReadPtr((const void*)((ADDRINT)m_addr + m_config->stringLengthOffset), 16) && !WINDOWS::IsBadReadPtr((const void*)((ADDRINT)m_addr + m_config->stringBufferOffset), 16))
	{
		UINT32 flags = m_addr[0x14>>2];
		UINT8* buffer = NULL;

		// Dependent string?
		if (flags & 4)
		{

			ASString* master = ASString::create((UINT32*)m_addr[0xc>>2]);
			buffer = master->getPtrToBuf();
			// Offset
			buffer += m_addr[m_config->stringBufferOffset>>2];
		}
		else
		{
			buffer = (UINT8*)m_addr[m_config->stringBufferOffset>>2];
		}

		UINT32 length = m_addr[m_config->stringLengthOffset>>2];
		

		if (!WINDOWS::IsBadReadPtr((const void*)buffer, length))
		{
			string s = string((const char*)buffer, length);
			return s;
		}
	}

	return NULL;
}
