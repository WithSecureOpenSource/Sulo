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

class ASString
{
public:

	enum Type
    {
		DYNAMIC_STRING = 0,
		STATIC_STRING = 1,
		DEPENDENT_STRING = 2
    };

	static ASString* create(UINT32* ptr);
	string getString(bool verbose=false);

private:
	ASString(void);
	ASString(UINT32*);
	ASString(ASString const&);
	void operator=(ASString const&);

	static BOOL isString(UINT32* addr);
	UINT8* getPtrToBuf();

	bool m_tagged;
	UINT32* m_addr;
	Config* m_config;
};

