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

#include <map>

#include "pin.H"

namespace WINDOWS 
{
	#include <Windows.h>
}

typedef struct Config {
	IMG img;
	UINT32 loadOffset;
	UINT32 rdataRVA;
	UINT32 rdataSize;

	UINT32 setInterpRVA;
	UINT32 verifyOnCallRVA;

	string versionStr;
	UINT32 versionStrRVA;

	UINT32 byteArrayVTableRVA;
	UINT32 bufferOffsetInByteArray;
	UINT32 dataOffsetInByteArrayBuffer;
	UINT32 countOffsetInByteArrayBuffer;

	UINT32 stringVTableRVA;
	UINT32 stringLengthOffset;
	UINT32 stringBufferOffset;
	
	UINT32 methodInfoOffsetInMethodEnv;

	UINT32 invokerOffsetInMethodInfo;
	UINT32 traitsOffsetInMethodInfo;
	UINT32 poolOffsetInMethodInfo;
	UINT32 abcInfoPosOffsetInMethodInfo;
	UINT32 methodIdOffsetInMethodInfo;
	UINT32 flagsOffsetInMethodInfo;
	
	UINT32 methodsOffsetInPoolObject;
	UINT32 methodNameIndicesOffsetInPoolObject;

	UINT32 initOffsetInTraits;
	UINT32 traitsPosOffsetOffsetInTraits;
	UINT32 posTypeOffsetInTraits;
	UINT32 namespaceOffsetInTraits;
	UINT32 nameOffsetInTraits;

	UINT32 precompNamesOffsetInPoolObject;

	UINT32 uriOffsetInNamespace;

	UINT32 namespacesOffsetInNamespaceSet;

	UINT32 multinameSize;
	UINT32 precompNamesHeaderSize;

	bool debugBuild;
} Config;

class FlashPlayerConfigBuilder
{
public:

	static FlashPlayerConfigBuilder& instance()
    {
        static FlashPlayerConfigBuilder instance;
        return instance;
    }

	bool isSupportedFlashPlayer(IMG img);
	Config* getConfig(void);

private:
	FlashPlayerConfigBuilder(void);
	FlashPlayerConfigBuilder(FlashPlayerConfigBuilder const&);
	void operator=(FlashPlayerConfigBuilder const&);

	map<string,Config> m_supportedConfigs;
	Config* m_currentConfig;
};
