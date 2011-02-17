#pragma once

#ifdef _WIN32
#ifndef GLUTILS_DECLSPEC
	#ifdef GLUTILS_EXPORTS
		#define GLUTILS_DECLSPEC    __declspec(dllexport) 
		#define GLUTILS_TEMPLATE(x) template class __declspec(dllexport) x;
	#else
		#define GLUTILS_DECLSPEC    __declspec(dllimport) 
		#define GLUTILS_TEMPLATE(x) template class __declspec(dllimport) x;
	#endif
#endif

#else
#ifndef GLUTILS_DECLSPEC
	#ifdef GLUTILS_EXPORTS
		#define GLUTILS_DECLSPEC     
		#define GLUTILS_TEMPLATE(x) template class x;
	#else
		#define GLUTILS_DECLSPEC     
		#define GLUTILS_TEMPLATE(x) template class x;
	#endif
#endif
#endif
