#ifndef CORE_H
#define CORE_H

#ifdef FC_PLATFORM_WINDOWS
	#ifdef FC_BUILD_DLL
		#define FALCON_API __declspec(dllexport) 
	#else 
		#define FALCON_API __declspec(dllimport)
	#endif
#else
	#error Unsuppported platform
#endif 

#endif // !CORE_H
