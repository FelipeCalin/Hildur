#pragma once

#ifdef HR_PLATFORM_WINDOWS
	#ifdef HR_BUILD_DLL
		#define HILDUR_API _declspec(dllexport)
	#else
		#define HILDUR_API _declspec(dllimport)
	#endif
#else
	#error Hildur currently only supports windows.
#endif

#define BIT(x) (1 << x)