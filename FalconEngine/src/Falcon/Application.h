#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"
namespace Falcon {
	class FALCON_API Application
	{
	public:
		Application();
		//Game classes are supposed to extend the application and define destructor
		virtual ~Application();
		void Run();
	};



}
#endif // !APPLICATION_H