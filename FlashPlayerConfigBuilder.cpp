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

#include "FlashPlayerConfigBuilder.h"

FlashPlayerConfigBuilder::FlashPlayerConfigBuilder(void)
{
	m_supportedConfigs["10.3.181.23_win_sa_debug"].versionStr = "WIN 10,3,181,22";
	m_supportedConfigs["10.3.181.23_win_sa_debug"].versionStrRVA = 0x453f74;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].setInterpRVA = 0x3F2A46;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].verifyOnCallRVA = 0x3F3131;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].byteArrayVTableRVA = 0x55d698;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].bufferOffsetInByteArray = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].dataOffsetInByteArrayBuffer = 0x24;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].countOffsetInByteArrayBuffer = 0x2c;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].stringVTableRVA = 0x55d4f8;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].stringLengthOffset = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].stringBufferOffset = 0x8;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].methodInfoOffsetInMethodEnv = 0x4;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].invokerOffsetInMethodInfo = 0x4;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].traitsOffsetInMethodInfo = 0xc;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].poolOffsetInMethodInfo = 0x14;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].abcInfoPosOffsetInMethodInfo = 0x18;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].methodIdOffsetInMethodInfo = 0x1c;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].flagsOffsetInMethodInfo = 0x34;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].initOffsetInTraits = 0x48;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].traitsPosOffsetOffsetInTraits = 0x50;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].posTypeOffsetInTraits = 0x79;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].namespaceOffsetInTraits = 0x3c;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].nameOffsetInTraits = 0x40;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].methodsOffsetInPoolObject = 0x8c;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].methodNameIndicesOffsetInPoolObject = 0x90;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].precompNamesOffsetInPoolObject = 0x50;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].uriOffsetInNamespace = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].namespacesOffsetInNamespaceSet = 0x4;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].multinameSize = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].precompNamesHeaderSize = 0x8;
	m_supportedConfigs["10.3.181.23_win_sa_debug"].debugBuild = true;

	m_supportedConfigs["10.3.181.23_win_sa"].versionStr = "WIN 10,3,181,22";
	m_supportedConfigs["10.3.181.23_win_sa"].versionStrRVA = 0x42a528;
	m_supportedConfigs["10.3.181.23_win_sa"].setInterpRVA = 0x3D3646;
	m_supportedConfigs["10.3.181.23_win_sa"].verifyOnCallRVA = 0x3d3c8a;
	m_supportedConfigs["10.3.181.23_win_sa"].byteArrayVTableRVA = 0x4D73B0;
	m_supportedConfigs["10.3.181.23_win_sa"].bufferOffsetInByteArray = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa"].dataOffsetInByteArrayBuffer = 0x24;
	m_supportedConfigs["10.3.181.23_win_sa"].countOffsetInByteArrayBuffer = 0x2c;
	m_supportedConfigs["10.3.181.23_win_sa"].stringVTableRVA = 0x4d72d0;
	m_supportedConfigs["10.3.181.23_win_sa"].stringLengthOffset = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa"].stringBufferOffset = 0x8;
	m_supportedConfigs["10.3.181.23_win_sa"].methodInfoOffsetInMethodEnv = 0x4;
	m_supportedConfigs["10.3.181.23_win_sa"].invokerOffsetInMethodInfo = 0x4;
	m_supportedConfigs["10.3.181.23_win_sa"].traitsOffsetInMethodInfo = 0xc;
	m_supportedConfigs["10.3.181.23_win_sa"].poolOffsetInMethodInfo = 0x14;
	m_supportedConfigs["10.3.181.23_win_sa"].abcInfoPosOffsetInMethodInfo = 0x18;
	m_supportedConfigs["10.3.181.23_win_sa"].methodIdOffsetInMethodInfo = 0x1c;
	m_supportedConfigs["10.3.181.23_win_sa"].flagsOffsetInMethodInfo = 0x30;
	m_supportedConfigs["10.3.181.23_win_sa"].initOffsetInTraits = 0x48;
	m_supportedConfigs["10.3.181.23_win_sa"].traitsPosOffsetOffsetInTraits = 0x50;
	m_supportedConfigs["10.3.181.23_win_sa"].posTypeOffsetInTraits = 0x79;
	m_supportedConfigs["10.3.181.23_win_sa"].namespaceOffsetInTraits = 0x3c;
	m_supportedConfigs["10.3.181.23_win_sa"].nameOffsetInTraits = 0x40;
	m_supportedConfigs["10.3.181.23_win_sa"].methodsOffsetInPoolObject = 0x88;
	m_supportedConfigs["10.3.181.23_win_sa"].methodNameIndicesOffsetInPoolObject = NULL;
	m_supportedConfigs["10.3.181.23_win_sa"].precompNamesOffsetInPoolObject = 0x50;
	m_supportedConfigs["10.3.181.23_win_sa"].uriOffsetInNamespace = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa"].namespacesOffsetInNamespaceSet = 0x4;
	m_supportedConfigs["10.3.181.23_win_sa"].multinameSize = 0x10;
	m_supportedConfigs["10.3.181.23_win_sa"].precompNamesHeaderSize = 0x8;
	m_supportedConfigs["10.3.181.23_win_sa"].debugBuild = false;

	m_supportedConfigs["10.3.181.23_winax"].versionStr = "WIN 10,3,181,23";
	m_supportedConfigs["10.3.181.23_winax"].versionStrRVA = 0x4CC970;
	m_supportedConfigs["10.3.181.23_winax"].setInterpRVA = 0x406766;
	m_supportedConfigs["10.3.181.23_winax"].verifyOnCallRVA = 0x406DAA;
	m_supportedConfigs["10.3.181.23_winax"].byteArrayVTableRVA = 0x57D038;
	m_supportedConfigs["10.3.181.23_winax"].bufferOffsetInByteArray = 0x10;
	m_supportedConfigs["10.3.181.23_winax"].dataOffsetInByteArrayBuffer = 0x24;
	m_supportedConfigs["10.3.181.23_winax"].countOffsetInByteArrayBuffer = 0x2c;
	m_supportedConfigs["10.3.181.23_winax"].stringVTableRVA = 0x57CF60;
	m_supportedConfigs["10.3.181.23_winax"].stringLengthOffset = 0x10;
	m_supportedConfigs["10.3.181.23_winax"].stringBufferOffset = 0x8;
	m_supportedConfigs["10.3.181.23_winax"].methodInfoOffsetInMethodEnv = 0x4;
	m_supportedConfigs["10.3.181.23_winax"].invokerOffsetInMethodInfo = 0x4;
	m_supportedConfigs["10.3.181.23_winax"].traitsOffsetInMethodInfo = 0xc;
	m_supportedConfigs["10.3.181.23_winax"].poolOffsetInMethodInfo = 0x14;
	m_supportedConfigs["10.3.181.23_winax"].abcInfoPosOffsetInMethodInfo = 0x18;
	m_supportedConfigs["10.3.181.23_winax"].methodIdOffsetInMethodInfo = 0x1c;
	m_supportedConfigs["10.3.181.23_winax"].flagsOffsetInMethodInfo = 0x30;
	m_supportedConfigs["10.3.181.23_winax"].initOffsetInTraits = 0x48;
	m_supportedConfigs["10.3.181.23_winax"].traitsPosOffsetOffsetInTraits = 0x50;
	m_supportedConfigs["10.3.181.23_winax"].posTypeOffsetInTraits = 0x79;
	m_supportedConfigs["10.3.181.23_winax"].namespaceOffsetInTraits = 0x3c;
	m_supportedConfigs["10.3.181.23_winax"].nameOffsetInTraits = 0x40;
	m_supportedConfigs["10.3.181.23_winax"].methodsOffsetInPoolObject = 0x88;
	m_supportedConfigs["10.3.181.23_winax"].methodNameIndicesOffsetInPoolObject = NULL;
	m_supportedConfigs["10.3.181.23_winax"].precompNamesOffsetInPoolObject = 0x50;
	m_supportedConfigs["10.3.181.23_winax"].uriOffsetInNamespace = 0x10;
	m_supportedConfigs["10.3.181.23_winax"].namespacesOffsetInNamespaceSet = 0x4;
	m_supportedConfigs["10.3.181.23_winax"].multinameSize = 0x10;
	m_supportedConfigs["10.3.181.23_winax"].precompNamesHeaderSize = 0x8;
	m_supportedConfigs["10.3.181.23_winax"].debugBuild = false;

	m_supportedConfigs["11.1.102.62_win_sa"].versionStr = "WIN 11,1,102,62";
	m_supportedConfigs["11.1.102.62_win_sa"].versionStrRVA = 0x5E42AC;
	m_supportedConfigs["11.1.102.62_win_sa"].setInterpRVA = 0x48B9E7;
	m_supportedConfigs["11.1.102.62_win_sa"].verifyOnCallRVA = 0x48C02D;
	m_supportedConfigs["11.1.102.62_win_sa"].byteArrayVTableRVA = 0x6AAFF8;
	m_supportedConfigs["11.1.102.62_win_sa"].bufferOffsetInByteArray = 0x10;
	m_supportedConfigs["11.1.102.62_win_sa"].dataOffsetInByteArrayBuffer = 0x20;
	m_supportedConfigs["11.1.102.62_win_sa"].countOffsetInByteArrayBuffer = 0x28;
	m_supportedConfigs["11.1.102.62_win_sa"].stringVTableRVA = 0x6AAFBC;
	m_supportedConfigs["11.1.102.62_win_sa"].stringLengthOffset = 0x10;
	m_supportedConfigs["11.1.102.62_win_sa"].stringBufferOffset = 0x8;
	m_supportedConfigs["11.1.102.62_win_sa"].methodInfoOffsetInMethodEnv = 0x8;
	m_supportedConfigs["11.1.102.62_win_sa"].invokerOffsetInMethodInfo = 0x8;
	m_supportedConfigs["11.1.102.62_win_sa"].traitsOffsetInMethodInfo = 0x10;
	m_supportedConfigs["11.1.102.62_win_sa"].poolOffsetInMethodInfo = 0x18;
	m_supportedConfigs["11.1.102.62_win_sa"].abcInfoPosOffsetInMethodInfo = 0x1c;
	m_supportedConfigs["11.1.102.62_win_sa"].methodIdOffsetInMethodInfo = 0x20;
	m_supportedConfigs["11.1.102.62_win_sa"].flagsOffsetInMethodInfo = 0x34;
	m_supportedConfigs["11.1.102.62_win_sa"].initOffsetInTraits = 0x4c;
	m_supportedConfigs["11.1.102.62_win_sa"].traitsPosOffsetOffsetInTraits = 0x54;
	m_supportedConfigs["11.1.102.62_win_sa"].posTypeOffsetInTraits = 0x7d;
	m_supportedConfigs["11.1.102.62_win_sa"].namespaceOffsetInTraits = 0x40;
	m_supportedConfigs["11.1.102.62_win_sa"].nameOffsetInTraits = 0x44;
	m_supportedConfigs["11.1.102.62_win_sa"].methodsOffsetInPoolObject = 0x88;
	m_supportedConfigs["11.1.102.62_win_sa"].methodNameIndicesOffsetInPoolObject = NULL;
	m_supportedConfigs["11.1.102.62_win_sa"].precompNamesOffsetInPoolObject = 0x50;
	m_supportedConfigs["11.1.102.62_win_sa"].uriOffsetInNamespace = 0xc;
	m_supportedConfigs["11.1.102.62_win_sa"].namespacesOffsetInNamespaceSet = 0x8;
	m_supportedConfigs["11.1.102.62_win_sa"].multinameSize = 0x10;
	m_supportedConfigs["11.1.102.62_win_sa"].precompNamesHeaderSize = 0xc;
	m_supportedConfigs["11.1.102.62_win_sa"].debugBuild = false;

	m_supportedConfigs["11.1.102.62_winax"].versionStr = "WIN 11,1,102,62";
	m_supportedConfigs["11.1.102.62_winax"].versionStrRVA = 0x68443C;
	m_supportedConfigs["11.1.102.62_winax"].setInterpRVA = 0x4BFD87;
	m_supportedConfigs["11.1.102.62_winax"].verifyOnCallRVA = 0x4C03CD;
	m_supportedConfigs["11.1.102.62_winax"].byteArrayVTableRVA = 0x7471E8;
	m_supportedConfigs["11.1.102.62_winax"].bufferOffsetInByteArray = 0x10;
	m_supportedConfigs["11.1.102.62_winax"].dataOffsetInByteArrayBuffer = 0x20;
	m_supportedConfigs["11.1.102.62_winax"].countOffsetInByteArrayBuffer = 0x28;
	m_supportedConfigs["11.1.102.62_winax"].stringVTableRVA = 0x74711C;
	m_supportedConfigs["11.1.102.62_winax"].stringLengthOffset = 0x10;
	m_supportedConfigs["11.1.102.62_winax"].stringBufferOffset = 0x8;
	m_supportedConfigs["11.1.102.62_winax"].methodInfoOffsetInMethodEnv = 0x8;
	m_supportedConfigs["11.1.102.62_winax"].invokerOffsetInMethodInfo = 0x8;
	m_supportedConfigs["11.1.102.62_winax"].traitsOffsetInMethodInfo = 0x10;
	m_supportedConfigs["11.1.102.62_winax"].poolOffsetInMethodInfo = 0x18;
	m_supportedConfigs["11.1.102.62_winax"].abcInfoPosOffsetInMethodInfo = 0x1c;
	m_supportedConfigs["11.1.102.62_winax"].methodIdOffsetInMethodInfo = 0x20;
	m_supportedConfigs["11.1.102.62_winax"].flagsOffsetInMethodInfo = 0x34;
	m_supportedConfigs["11.1.102.62_winax"].initOffsetInTraits = 0x4c;
	m_supportedConfigs["11.1.102.62_winax"].traitsPosOffsetOffsetInTraits = 0x54;
	m_supportedConfigs["11.1.102.62_winax"].posTypeOffsetInTraits = 0x7d;
	m_supportedConfigs["11.1.102.62_winax"].namespaceOffsetInTraits = 0x40;
	m_supportedConfigs["11.1.102.62_winax"].nameOffsetInTraits = 0x44;
	m_supportedConfigs["11.1.102.62_winax"].methodsOffsetInPoolObject = 0x88;
	m_supportedConfigs["11.1.102.62_winax"].methodNameIndicesOffsetInPoolObject = NULL;
	m_supportedConfigs["11.1.102.62_winax"].precompNamesOffsetInPoolObject = 0x50;
	m_supportedConfigs["11.1.102.62_winax"].uriOffsetInNamespace = 0xc;
	m_supportedConfigs["11.1.102.62_winax"].namespacesOffsetInNamespaceSet = 0x8;
	m_supportedConfigs["11.1.102.62_winax"].multinameSize = 0x10;
	m_supportedConfigs["11.1.102.62_winax"].precompNamesHeaderSize = 0xc;
	m_supportedConfigs["11.1.102.62_winax"].debugBuild = false;

	m_currentConfig = NULL;
}

bool FlashPlayerConfigBuilder::isSupportedFlashPlayer(IMG img)
{
	UINT32 loadOffset = IMG_LoadOffset(img);
	UINT32 rdataRVA = 0;
	UINT32 rdataSize = 0;

	// Find where the .rdata section is
	bool rdataFound = false;
	for (SEC sec = IMG_SecHead(img); SEC_Valid(sec); sec = SEC_Next(sec))
	{
		if (SEC_Name(sec) == ".rdata")
		{
			rdataRVA = SEC_Address(sec) - loadOffset;
			rdataSize = SEC_Size(sec);
			rdataFound = true;
			break;
		}
	}

	// If no .rdata, this is not Flash Player
	if (!rdataFound)
	{
		return false;
	}

	for (map<string,Config>::iterator it = m_supportedConfigs.begin(); it != m_supportedConfigs.end(); ++it)
	{
		Config* fc = &it->second;

		// Make sure the address is within .rdata
		if ((fc->versionStrRVA + fc->versionStr.length() + 1) > (rdataRVA + rdataSize) || (fc->versionStrRVA < rdataRVA))
		{
			continue;
		}

		if (!WINDOWS::IsBadReadPtr((const void*)(fc->versionStrRVA+loadOffset), fc->versionStr.length()+1))
		{
			if (strcmp(fc->versionStr.c_str(), (const char*)(fc->versionStrRVA+loadOffset)) == 0)
			{
				// Set load offset and IMG values now that we know those
				fc->img = img;
				fc->loadOffset = loadOffset;
				fc->rdataRVA = rdataRVA;
				fc->rdataSize = rdataSize;
				m_currentConfig = fc;
				
				return true;
			}
		}
	}

	return false;
}

Config* FlashPlayerConfigBuilder::getConfig(void)
{
	return m_currentConfig;
}
