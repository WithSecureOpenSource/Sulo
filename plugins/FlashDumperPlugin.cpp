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

#include "FlashDumperPlugin.h"
#include "ASByteArray.h"
#include "Logger.h"

#include <fstream>

namespace WINDOWS
{
	#include <windows.h>
}


// Commandline options for Flash dumper plugin
extern KNOB<string> KnobFlashDumperFilePrefix;


FlashDumperPlugin::FlashDumperPlugin() :
	m_filePrefix(KnobFlashDumperFilePrefix.Value()),
	m_dumpedFlashCount(0)
{
}


bool FlashDumperPlugin::beforeMethodCall(ASMethodInfo* mi, string methodName, UINT32 argc, UINT32* argv)
{
	if (methodName != "flash.display::Loader/loadBytes")
	{
		return false;
	}

	ASByteArray* ba = ASByteArray::create((UINT32*)argv[1]);
	if (ba == NULL)
	{
		return false;
	}

	UINT8* data = ba->getData();
	UINT32 nBytes = ba->getDataLength();

	LOGF("Dumping flash from 0x%x, %d bytes\n", data, nBytes);

	char filename[MAX_PATH+1];
	_snprintf(filename, MAX_PATH, "%s_flash_%u.bin", m_filePrefix.c_str(), m_dumpedFlashCount);
	m_dumpedFlashCount++;
	//LOGF("Dumping flash to %s\n", filename);
	ofstream flashFile(filename, ios::out | ios::binary);
    flashFile.write((const char*)data, nBytes);
	flashFile.close();

	LOGF("Dumped flash to %s\n", filename);

	return false;
}


void FlashDumperPlugin::afterMethodCall(UINT32 retVal)
{
	return;
}


void FlashDumperPlugin::dumpFlashToDisk(UINT8* data, UINT32 nBytes)
{
}
