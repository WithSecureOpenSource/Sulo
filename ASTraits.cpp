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

#include "ASTraits.h"
#include "ASMultiname.h"
#include "Logger.h"

map<UINT32*,ASTraits*> ASTraits::m_traitsCache;

static UINT32 readU30(const char *&p)
{
	unsigned int result = p[0];
	if (!(result & 0x00000080))
	{
		p++;
		return result;
	}
	result = (result & 0x0000007f) | p[1]<<7;
	if (!(result & 0x00004000))
	{
		p += 2;
		return result;
	}
	result = (result & 0x00003fff) | p[2]<<14;
	if (!(result & 0x00200000))
	{
		p += 3;
		return result;
	}
	result = (result & 0x001fffff) | p[3]<<21;
	if (!(result & 0x10000000))
	{
		p += 4;
		return result;
	}
	result = (result & 0x0fffffff) | p[4]<<28;
	p += 5;
	return result;
}


ASTraits::ASTraits(void)
{
}

ASTraits::ASTraits(UINT32* ptr) :
	m_methodIdToMethodNameIndexPopulated(false)
{
	m_traits = ptr;
	m_config = FlashPlayerConfigBuilder::instance().getConfig();
}


ASTraits* ASTraits::create(UINT32* ptr)
{
	if (m_traitsCache.find(ptr) != m_traitsCache.end())
	{
		return m_traitsCache[ptr];
	}
	else
	{
		m_traitsCache[ptr] = new ASTraits(ptr);
		return m_traitsCache[ptr];
	}
}


// Construct the name using info in private fields
string ASTraits::getName()
{
	if (!m_name.empty())
	{
		return m_name;
	}

	UINT32* p_ns = (UINT32*)m_traits[m_config->namespaceOffsetInTraits/4];
	UINT32* p_name = (UINT32*)m_traits[m_config->nameOffsetInTraits/4];

	if (p_ns == NULL)
	{
		LOGF("ERROR: namespace pointer is NULL in Traits object\n");
		return "ERROR";
	}

	if (p_name == NULL)
	{
		LOGF("ERROR: name pointer is NULL in Traits object\n");
		return "ERROR";
	}

	m_name = ASMultiname::format(p_ns, p_name, false); 
	return m_name;
}


UINT32* ASTraits::getInit()
{
	return (UINT32*)m_traits[m_config->initOffsetInTraits/4];
}


UINT8 ASTraits::getPosType()
{
	UINT8* tmp = (UINT8*)m_traits;
	return tmp[m_config->posTypeOffsetInTraits];
}


UINT32 ASTraits::methodIdToMethodNameIndex(UINT32 method_id)
{
	if (m_methodIdToMethodNameIndexPopulated)
	{
		if (m_methodIdToMethodNameIndex.find(method_id) == m_methodIdToMethodNameIndex.end())
		{
			return -1;
		}
		else
		{
			return m_methodIdToMethodNameIndex[method_id];
		}
	}

	UINT32* traitsPtr = m_traits;
	UINT8 posType = getPosType();
	UINT32* traitsPos = (UINT32*)traitsPtr[m_config->traitsPosOffsetOffsetInTraits/4];

	const char* pos = (char*)traitsPos;

	if (posType == TRAITSTYPE_INSTANCE || posType == TRAITSTYPE_INTERFACE)
	{
		UINT32 qname = readU30(pos);
		UINT32 baseTraits = readU30(pos);

		UINT8 flags2 = *pos++;
		if ((flags2 & 8) != 0)
		{
			UINT32 dummy = readU30(pos);
		}

		UINT32 interfaceCount = readU30(pos);
		for (UINT32 i=0; i < interfaceCount; i++)
		{
			UINT32 dummy = readU30(pos);
		}
	}

	if (posType == TRAITSTYPE_INSTANCE || posType == TRAITSTYPE_INTERFACE || posType == TRAITSTYPE_CLASS || posType == TRAITSTYPE_SCRIPT)
	{
		UINT32 init_index = readU30(pos);
	}

	//  AbcParser::parseTraits
	UINT32 count = readU30(pos);
	for (UINT32 i=0; i < count; i++)
	{
		UINT32 qindex = readU30(pos);
		UINT8 tag = *pos++;
		UINT8 kind = tag & 0xf;
		UINT32 method_index = NULL;
		UINT32 earlyDispId = 0;
		UINT32 slot_id = 0;
		UINT32 typeName = 0;

		switch (kind)
		{
			case 1:
			case 2:
			case 3:
				earlyDispId = readU30(pos);
				method_index = readU30(pos);
				break;
			case 4:
				slot_id = readU30(pos);
				method_index = readU30(pos);
				break;
			case 0:
			case 6:
				slot_id = readU30(pos);
				typeName = readU30(pos);
				// This is called value_index in original source code
				method_index = readU30(pos);
				if (method_index)
				{
					pos += 1;
				}
				break;
			default:
				break;
		}

		// Metadata?
		if (tag & 0x40)
		{
			UINT32 metadataCount = readU30(pos);
			for (UINT32 metadata = 0; metadata < metadataCount; ++metadata)
			{
				const UINT32 index = readU30(pos);
			}
		}

		m_methodIdToMethodNameIndex[method_index] = qindex;
	}

	m_methodIdToMethodNameIndexPopulated = true;
	if (m_methodIdToMethodNameIndex.find(method_id) == m_methodIdToMethodNameIndex.end())
	{
		return -1;
	}
	else
	{
		return m_methodIdToMethodNameIndex[method_id];
	}
}