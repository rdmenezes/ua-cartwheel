#pragma once

#ifdef _WIN32

#ifndef UTILS_DECLSPEC
	#ifdef UTILS_EXPORTS
		#define UTILS_DECLSPEC    __declspec(dllexport) 
		#define UTILS_TEMPLATE(x) template class __declspec(dllexport) x;
	#else
		#define UTILS_DECLSPEC    __declspec(dllimport) 
		#define UTILS_TEMPLATE(x) extern template class __declspec(dllimport) x;
	#endif
#endif

#else

#ifndef UTILS_DECLSPEC
        #ifdef UTILS_EXPORTS
                #define UTILS_DECLSPEC     
                #define UTILS_TEMPLATE(x) template class x;
        #else
                #define UTILS_DECLSPEC     
                #define UTILS_TEMPLATE(x) extern template class x;
        #endif

#endif 

#endif
#include <Utils/UtilsTemplates.h>
