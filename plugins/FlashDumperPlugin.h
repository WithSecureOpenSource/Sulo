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

#include "ISuloPlugin.h"
#include "pin.H"

class FlashDumperPlugin : public ISuloPlugin
{
public:
	FlashDumperPlugin();

	bool beforeMethodCall(ASMethodInfo* mi, string methodName, UINT32 argc, UINT32* argv);
	void afterMethodCall(UINT32 retVal);

private:
	void dumpFlashToDisk(UINT8* data, UINT32 nBytes);
	
	string m_filePrefix;

	// Running count for numbering the dumped Flash files
	int m_dumpedFlashCount;
};

