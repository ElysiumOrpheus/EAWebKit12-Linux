/*
Copyright (C) 2013 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

////////////////////////////////////////////////////////////////////
// EAWebKitDll.cpp
// By Arpit Baldeva
// Maintained by EAWebKit Team
//
// The purpose of this file is isolate dll export related code.
///////////////////////////////////////////////////////////////////

#include "config.h"
#include <EAWebKit/EAWebKitDll.h>
#include <EAWebKit/EAWebKit.h>
#include <EAIO/Allocator.h>
#if defined(EA_PLATFORM_MICROSOFT)
#include EAWEBKIT_PLATFORM_HEADER
#endif
#if BUILDING_EAWEBKIT_DLL && !defined(EA_PLATFORM_LINUX)

extern "C" EA::WebKit::EAWebKitLib* CreateEAWebkitInstance(void)
{
	//Note by Arpit Baldeva: WTF::fastNew calls EAWebkit allocator under the hood. Since the first thing we do after loading the relocatable module is to 
	//create an instance, the application does not have a chance to setup the allocator. So if we use EAWebkit's default allocator for creating the 
	//concrete instance and later the application swaps out the allocator with a its own supplied allocator, we would end up freeing the ConcreteInstance
	//from an allocator that did not create it. The solution is to either use a global new/delete operator or create a static EAWebkitConcrete instance.
	//I chose to make a static in case we decide to overrride global new/delete for the relocatable module in the future. Then, we would avoid inadvertently
	//creating the instance from one allocator and freeing it using another.

	static EA::WebKit::EAWebKitLib concreteInstance;
	return &concreteInstance;
}

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) || defined(CS_UNDEFINED_STRING)
#include <EAWebKit/DLLInterface.h>
#endif 

extern "C" int EAWebKitDllStart(void)
{
	//Set the allocators to be used by these packages when the module is loaded.
	EA::IO::SetAllocator(EA::Allocator::ICoreAllocator::GetDefaultAllocator());

	return PLATFORM_DLL_START_SUCCESS;
}

extern "C" int EAWebKitDllStop(void)
{
	return PLATFORM_DLL_STOP_SUCCESS;

}

PLATFORM_DLL_START(EAWebKitDllStart);
PLATFORM_DLL_STOP(EAWebKitDllStop);


#if defined(_WIN32) || defined(_WIN64)
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4255)
#endif //_MSC_VER

#include <windows.h>


#if defined(_MSC_VER)
#pragma warning(pop)
#endif //_MSC_VER

BOOL APIENTRY DllMain( HANDLE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
	);

BOOL APIENTRY DllMain( HANDLE hModule,
	DWORD ul_reason_for_call,
	LPVOID lpReserved
	)
{
	BOOL returnValue = TRUE;

	switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		if(platform_dll_start_func() != PLATFORM_DLL_START_SUCCESS)
			returnValue = FALSE;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if(platform_dll_stop_func() != PLATFORM_DLL_STOP_SUCCESS)
			returnValue = FALSE;
		break;
	}

	return returnValue;
}

#endif 


#endif //BUILDING_EAWEBKIT_DLL
