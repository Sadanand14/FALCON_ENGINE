#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>
#include <boost/container/vector.hpp>
#include <boost/container/map.hpp>
#include <boost/container/set.hpp>
#include <boost/container/list.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/unordered_map.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/make_shared.hpp>

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "PxPhysicsAPI.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


typedef int8_t  i08;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u08;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

#endif