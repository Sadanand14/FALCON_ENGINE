#ifndef PHYSICS_H
#define PHYSICS_H
#include <ctype.h>
#include "PxPhysicsAPI.h"

#include "../Log.h"
#define PX_SUPPORT_PVD	
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}


namespace physics
{
	bool InitPhysX();
	bool ShutdownPhysX();

}

#endif