#pragma once

#include <memory>


#ifdef HR_PLATFORM_WINDOWS
	#ifdef HR_DYNAMIC_LINK
		#ifdef HR_BUILD_DLL
			#define HILDUR_API _declspec(dllexport)
		#else
			#define HILDUR_API _declspec(dllimport)
		#endif
	#else
		#define HILDUR_API
	#endif
#else
	#error Hildur currently only supports windows.
#endif

#ifdef HR_DEBUG
	#define HR_ENABLE_ASSERTS
#endif

#ifdef HR_ENABLE_ASSERTS
	#define HR_ASSERT(x, ...) { if(!(x)) { HR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HR_CORE_ASSERT(x, ...) { if(!(x)) { HR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HR_ASSERT(x, ...)
	#define HR_CORE_ASSERT(x, ...)
#endif 


#define BIT(x) (1 << x)

#define HR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


namespace Hildur {


	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;


}