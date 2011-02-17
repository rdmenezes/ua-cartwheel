#pragma once

#ifdef _WIN32

#ifndef PHYSICS_DECLSPEC
	#ifdef PHYSICS_EXPORTS
		#define PHYSICS_DECLSPEC    __declspec(dllexport) 
		#define PHYSICS_TEMPLATE(x) template class __declspec(dllexport) x;
	#else
		#define PHYSICS_DECLSPEC    __declspec(dllimport) 
		#define PHYSICS_TEMPLATE(x) template class __declspec(dllimport) x; 
	#endif
#endif

#else

#ifndef PHYSICS_DECLSPEC
	#ifdef PHYSICS_EXPORTS
		#define PHYSICS_DECLSPEC     
		#define PHYSICS_TEMPLATE(x) template class x;
	#else
		#define PHYSICS_DECLSPEC     
		#define PHYSICS_TEMPLATE(x) template class x; 
	#endif
#endif

#endif
