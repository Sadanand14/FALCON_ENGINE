#ifndef TYPETOINT_H_
#define TYPETOINT_H_

#include <atomic>
#include "Types.h"

static std::atomic_int idCounter = 0;

namespace TypeToInt
{
	template<typename T>
	u64 ConvertType()
	{
		static u32 id = 1 << idCounter++;
		return id;
	}
}
#endif
