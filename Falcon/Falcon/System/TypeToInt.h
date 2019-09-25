#ifndef TYPETOINT_H_
#define TYPETOINT_H_

#include <atomic>
#include "Types.h"


namespace TypeToInt
{
	static std::atomic_int idCounter = 0;
	template<typename T>
	u32 ConvertType()
	{
		static u32 id = 1 << idCounter++;
		return id;
	}
}
#endif
