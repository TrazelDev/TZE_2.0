#pragma once

#ifdef TZE_PLATFORM_WINDOWS
	#ifdef TZE_BUILD_DLL
		#define TZE_API __declspec(dllexport)
	#else
		#define TZE_API __declspec(dllimport)
	#endif
	#else
		#error trazel_engine only supports Wondows!
#endif