#pragma once

#if UE_BUILD_SHIPPING
	#define LOG_DEFAULT_VERBOSITY Warning
#else
	#define LOG_DEFAULT_VERBOSITY All
#endif

#if UE_BUILD_SHIPPING
	#define LOG_COMPILETIME_VERBOSITY Error
#else
	#define LOG_COMPILETIME_VERBOSITY Log
#endif
