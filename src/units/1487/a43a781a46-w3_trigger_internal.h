#pragma once
#include "84acc80030-w3_trigger.h"
#include <cstring>
#include <cstddef>
// Canonical existing kotor_w3::Vector3 definition (w3_client.h51), without stalelegacycallee declarations.
namespace kotor_w3 {struct Vector3 {float x,y,z;};}
namespace trigger_private {
using U=std::uint32_t;using I=std::int32_t;using Vec=kotor_w3::Vector3;
static_assert(sizeof(void*)==4&&sizeof(Vec)==12,"original x86 trigger ABI");
inline void* ptr(U a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
inline U word(const void* p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
inline void* at(const void* p,U o){return ptr(word(p)+o);}
template<class T>T ld(const void* p,U o){T v;std::memcpy(&v,at(p,o),sizeof v);return v;}
template<class T>void st(void* p,U o,T v){std::memcpy(at(p,o),&v,sizeof v);}
inline U bits(float f){U v;std::memcpy(&v,&f,4);return v;}
}
