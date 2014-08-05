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

#include <sstream>

#include "pin.H"

#define LOGF(...) Logger::LOGF(__VA_ARGS__)

class Logger
{
public:
	static void LOGF(char* fmt, ...)
	{ 
		va_list args;
		va_start(args, fmt);

		int len = vsnprintf(NULL, 0, fmt, args) + 1;
		char* buf = (char*)malloc(len);
		
		vsnprintf(buf, len-1, fmt, args);
		buf[len-1] = 0;
		
		LOG(buf);

		free(buf);
		va_end(args);
	} ;
};

