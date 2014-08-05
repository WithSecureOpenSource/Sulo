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

#include "ASByteArray.h"
#include "FlashPlayerConfigBuilder.h"
#include "Logger.h"

ASByteArray::ASByteArray(void)
{
}


ASByteArray::ASByteArray(UINT32* ptr)
{
	m_addr = (UINT32*)((UINT32)ptr & 0xfffffff8);
	m_config = FlashPlayerConfigBuilder::instance().getConfig();
}


ASByteArray* ASByteArray::create(UINT32* ptr)
{
	if (!ASByteArray::isByteArray(ptr))
	{
		return NULL;
	}

	return new ASByteArray(ptr);
}


bool ASByteArray::isByteArray(UINT32* ptr)
{
	// Untag the pointer and try to read it
	UINT32* addr = (UINT32*)((UINT32)ptr & 0xfffffff8);
	Config* config = FlashPlayerConfigBuilder::instance().getConfig();

	if (!WINDOWS::IsBadReadPtr((const void*)addr, 16))
	{
		UINT32* ba = (UINT32*)addr;
		ADDRINT vtableAddr = (UINT32)ba[0];
		if (vtableAddr == (config->loadOffset + config->byteArrayVTableRVA))
		{
			return true;
		}
	}

	return false;
}


UINT8* ASByteArray::getData()
{
	UINT32* buffer = (UINT32*)m_addr[m_config->bufferOffsetInByteArray/sizeof(UINT32)];
	UINT8* data = (UINT8*)buffer[m_config->dataOffsetInByteArrayBuffer/sizeof(UINT32)];
	UINT32 count = buffer[m_config->countOffsetInByteArrayBuffer/sizeof(UINT32)];

	UINT8* copyOfData = (UINT8*)malloc(count);
	if (copyOfData == NULL)
	{
		return NULL;
	}

	memcpy(copyOfData, data, count);
	return copyOfData;
}


UINT32 ASByteArray::getDataLength()
{
	UINT32* buffer = (UINT32*)m_addr[m_config->bufferOffsetInByteArray/sizeof(UINT32)];
	UINT32 count = buffer[m_config->countOffsetInByteArrayBuffer/sizeof(UINT32)];
	return count;
}
