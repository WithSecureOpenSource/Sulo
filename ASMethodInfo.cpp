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

#include "ASMethodInfo.h"
#include "ASMultiname.h"
#include "Logger.h"

map<UINT32*,ASMethodInfo*> ASMethodInfo::m_methodInfoCache;

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


ASMethodInfo::ASMethodInfo(void)
{
}


ASMethodInfo::ASMethodInfo(UINT32* ptr) :
	m_methodSignatureBuilt(false)
{
	m_methodInfo = ptr;
	m_config = FlashPlayerConfigBuilder::instance().getConfig();
}


ASMethodInfo* ASMethodInfo::create(UINT32* ptr)
{
	if (m_methodInfoCache.find(ptr) != m_methodInfoCache.end())
	{
		return m_methodInfoCache[ptr];
	}
	else
	{
		m_methodInfoCache[ptr] = new ASMethodInfo(ptr);
		return m_methodInfoCache[ptr];
	}
}


UINT32 ASMethodInfo::getMethodId()
{
	return m_methodInfo[m_config->methodIdOffsetInMethodInfo>>2];
}


void ASMethodInfo::buildMethodSignature()
{
	UINT32* pool = (UINT32*)m_methodInfo[m_config->poolOffsetInMethodInfo>>2];	
	UINT32 method_id = m_methodInfo[m_config->methodIdOffsetInMethodInfo>>2];
	UINT32 flags = m_methodInfo[m_config->flagsOffsetInMethodInfo>>2];
	UINT32* abc_info_pos = (UINT32*)m_methodInfo[m_config->abcInfoPosOffsetInMethodInfo>>2];
	UINT32* precompNames = (UINT32*)pool[m_config->precompNamesOffsetInPoolObject/4];

	precompNames = precompNames + m_config->precompNamesHeaderSize/sizeof(UINT32);

	// Do some manual ABC parsing to find parameter and return value information
	const char* pos = (char*)abc_info_pos;

	UINT32 param_count = readU30(pos);

	UINT32 ret_type_name_index = readU30(pos);
	if (ret_type_name_index != 0)
	{
		UINT32* name = (UINT32*)&precompNames[ret_type_name_index*(m_config->multinameSize/sizeof(UINT32))];
		ASMultiname* mn = ASMultiname::create(name);

		if (mn == NULL)
		{
			m_returnValueType = "null";
		}
		else
		{
			m_returnValueType = mn->getFormatted();
		}
	}
	else
	{
		m_returnValueType = "null";
	}

	for (UINT32 i=0; i < param_count; i++)
	{
		UINT32 param_name_index = readU30(pos);
		if (param_name_index == 0)
		{
			m_parameterTypes.push_back("null");
		}
		else
		{
			ASMultiname* mn = ASMultiname::create((UINT32*)&precompNames[param_name_index*(m_config->multinameSize/sizeof(UINT32))]);
			if (mn == NULL)
			{
				m_parameterTypes.push_back("null");
			}
			else
			{
				m_parameterTypes.push_back(mn->getFormatted());
			}
		}
	}
}


string ASMethodInfo::getName()
{
	UINT32* declaringScopeOrTraits = (UINT32*)m_methodInfo[m_config->traitsOffsetInMethodInfo>>2];
	UINT32* pool = (UINT32*)m_methodInfo[m_config->poolOffsetInMethodInfo>>2];	
	UINT32 method_id = m_methodInfo[m_config->methodIdOffsetInMethodInfo>>2];
	UINT32 flags = m_methodInfo[m_config->flagsOffsetInMethodInfo>>2];
	UINT32* precompNames = (UINT32*)pool[m_config->precompNamesOffsetInPoolObject/4];

	/**
	 * Getting the name is easier with debug build because it has method_name_indices array that
	 * maps method_id to an index in precompNames (precomputer multinames). With non-debug builds
	 * we need to manually calculate that index. Since the calculation is slow, we cache the results.
	 */

	INT32 methodNameIndex;
	
	// Debug build is easier to handle
	if (m_config->debugBuild)
	{
		UINT32* method_name_indices = (UINT32*)pool[m_config->methodNameIndicesOffsetInPoolObject/4];

		UINT32 method_name_indices_size = method_name_indices[0];
		if (method_id >= method_name_indices_size)
		{
			LOGF("ERROR: method_id %d is larger than method_name_indices_size %d\n",  method_id, method_name_indices_size);
			return "ERROR";
		}

		// The first two element are for other stuff
		methodNameIndex = method_name_indices[method_id+2];

		if (methodNameIndex == 0)
		{
			LOGF("ERROR: methodNameIndex is zero for method_id 0x%x\n", method_id);
			return "ERROR";
		}
		else if (methodNameIndex > 0)
		{
			// No name needed
			return "";
		}

		// Change the sign because we prefer positive indeces
		methodNameIndex = -methodNameIndex;
	}
	else
	{
		UINT32* traitsPtr = getTraitsPtr();		
		ASTraits* as_traits = ASTraits::create(traitsPtr);
		methodNameIndex = as_traits->methodIdToMethodNameIndex(method_id);
		if (methodNameIndex == -1)
		{
			// No name needed
			return "";
		}
	}

	UINT32* pMultiname = precompNames + m_config->precompNamesHeaderSize/sizeof(UINT32);
	pMultiname += methodNameIndex*(m_config->multinameSize/sizeof(UINT32));
	ASMultiname* mn = ASMultiname::create(pMultiname);
	if (mn == NULL)
	{
		LOGF("ERROR: Creating ASMultiname from precompNames failed\n");
		return "ERROR";
	}
	return mn->getFormatted();
}


UINT32 ASMethodInfo::getFlags()
{
	return m_methodInfo[m_config->flagsOffsetInMethodInfo>>2];
}


UINT32* ASMethodInfo::getTraitsPtr()
{

	UINT32* declaringScopeOrTraits = (UINT32*)m_methodInfo[m_config->traitsOffsetInMethodInfo/4];

	if ((UINT32)declaringScopeOrTraits & 1)
	{
		declaringScopeOrTraits = (UINT32*)((UINT32)declaringScopeOrTraits & ~1);
		return (UINT32*)declaringScopeOrTraits[2];
	}
	else
	{
		return declaringScopeOrTraits;
	}
}


string ASMethodInfo::getMethodNameWithTraits()
{
	if (!m_methodNameWithTraits.empty())
	{
		return m_methodNameWithTraits;
	}

	UINT32* pTraits = getTraitsPtr();
	if (((UINT32)pTraits & ~3) == NULL)
	{
		return "Function/<anonymous>";
	}

	ASTraits* traits = ASTraits::create(pTraits);

	if (traits == NULL)
	{
		LOGF("ERROR: ASTraits::create returned NULL\n");
		m_methodNameWithTraits = "ERROR";
		return m_methodNameWithTraits;
	}

	UINT32* init = traits->getInit();
	if (init == NULL)
	{
		LOGF("ERROR: traits->getInit returned NULL\n");
		m_methodNameWithTraits = "ERROR";
		return m_methodNameWithTraits;
	}

	string traitsName = traits->getName();
	string name = getName();

	if (init == m_methodInfo)
	{
		UINT8 posType = traits->getPosType();

		if (posType == TRAITSTYPE_SCRIPT)
		{
			m_methodNameWithTraits = traitsName + "$init";
			return m_methodNameWithTraits;
		}
		else if (posType == TRAITSTYPE_CLASS)
		{
			m_methodNameWithTraits = traitsName + "cinit";
			return m_methodNameWithTraits;
		}
		else
		{
			m_methodNameWithTraits = traitsName;
			return m_methodNameWithTraits;
		}
	}

	UINT32 flags = getFlags();
	string sep = "/";

	if (flags & IS_GETTER)
	{
		sep = "/get ";
	}
	else if (flags & IS_SETTER)
	{
		sep = "/set ";
	}

	m_methodNameWithTraits = traitsName + sep + name;
	return m_methodNameWithTraits;
}


string ASMethodInfo::getReturnValueType()
{
	if (!m_methodSignatureBuilt)
	{
		buildMethodSignature();
	}

	return m_returnValueType; 
}


const vector<string>* ASMethodInfo::getParameterTypes()
{
	if (!m_methodSignatureBuilt)
	{
		buildMethodSignature();
	}

	return &m_parameterTypes; 
}