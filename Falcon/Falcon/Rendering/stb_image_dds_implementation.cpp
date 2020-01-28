

/**
	Disabling warnings thrown out by vendor code
*/
#pragma warning( push )
#pragma warning( disable : 6011)
#pragma warning( disable : 6262)
#pragma warning( disable : 6308)
#pragma warning( disable : 6387)
#pragma warning( disable : 28182)
#pragma warning( disable : 4312)
#pragma warning( disable : 4005)
#pragma warning( disable : 26451)


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_DDS_IMPLEMENTATION
#include "stb_dds.h"

#pragma warning( pop )