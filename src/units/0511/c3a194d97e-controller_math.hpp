#pragma once
// controller_math.hpp - x87 sequence equivalents used by the TextureController bodies (lane-texture-epoch).
// The original runs x87 at the CRT default precision control (53-bit) and round-to-nearest; each chain is written
// as the same IEEE double operations, rounded to binary32 exactly where the original stores `fstp dword`.
#include "1265433832-texture_controller.hpp"
#include <cmath>
#include <climits>
#include <cstring>

namespace controller_math {
using texture::U; using texture::I;
// frndint (RC nearest-even) + fistp dword: out-of-range/NaN -> integer indefinite 0x80000000.
inline I rint_i32(double v){
 if(!(v>=-2147483648.0&&v<2147483648.0))return INT_MIN;
 const double r=std::nearbyint(v);
 if(!(r>=-2147483648.0&&r<2147483648.0))return INT_MIN;
 return static_cast<I>(r);
}
// _ftol 006FAE8C (truncation): out-of-range/NaN -> 0x80000000.
inline I ftol(double v){
 if(!(v>-2147483649.0&&v<2147483648.0))return INT_MIN;
 return static_cast<I>(v);
}
inline float as_float(U bits){float f;std::memcpy(&f,&bits,4);return f;}
inline U bits_of(float f){U u;std::memcpy(&u,&f,4);return u;}
// CExoArrayList<float> element access through the list's data DWORD (object representation copy).
inline float list_float(const texture::List& list,I index){float f;std::memcpy(&f,reinterpret_cast<const void*>(list.data+static_cast<U>(index)*4u),4);return f;}
inline void set_list_bits(texture::List& list,I index,U bits){std::memcpy(reinterpret_cast<void*>(list.data+static_cast<U>(index)*4u),&bits,4);}
inline std::uint8_t* bytes(U address){return reinterpret_cast<std::uint8_t*>(address);}
inline U address(const void* p){return reinterpret_cast<U>(p);}
// fsin on the 53-bit-rounded argument, then fmul dword k, then fstp dword (fsin's extended result is not
// representable as a C++ double without an extra rounding, so the three instructions stay together).
inline float x87_sin_mul_store(double argument,float k){
 float result;
 __asm {
  fld qword ptr argument
  fsin
  fmul dword ptr k
  fstp dword ptr result
 }
 return result;
}
// REP MOVSD + REP MOVSB forward copy (original inline copy idiom).
inline void rep_copy(U destination,U source,U count){
 __asm {
  mov edi,destination
  mov esi,source
  mov ecx,count
  mov edx,ecx
  shr ecx,2
  rep movsd
  mov ecx,edx
  and ecx,3
  rep movsb
 }
}
// Owner texture virtual getters through the typed table (live reload per call).
inline I owner_width(texture_controller::TextureController* c){font::Texture* o=c->owner;return texture_vtable::head(o)->common.s48(o,nullptr);}
inline I owner_height(texture_controller::TextureController* c){font::Texture* o=c->owner;return texture_vtable::head(o)->common.s4c(o,nullptr);}
inline I owner_bpp(texture_controller::TextureController* c){font::Texture* o=c->owner;return texture_vtable::head(o)->common.s50(o,nullptr);}
inline void* owner_image(texture_controller::TextureController* c){font::Texture* o=c->owner;return texture_vtable::head(o)->common.s3c(o,nullptr);}
}

// TXI reader leaves (lane-font providers; identical declarations to lane-font/source/font_txi.hpp).
extern "C" {
 char* __cdecl firstword_00463530(char*);
 char* __cdecl skip_blanks_00463510(char*);
 void __cdecl parse_float_004635c0(char*,volatile float*);
 void __cdecl parse_int_004635e0(char*,volatile std::int32_t*);
 int __cdecl kotor_sscanf_006fae08(const char*,const char*,...);
 __declspec(noinline) std::int32_t __cdecl kotor_stricmp_0070acaf(const char*,const char*);
 char* __cdecl resource_next_0044bfa0();
 void* __cdecl resource_read_0044bfd0(std::uint32_t);
}
// CExoArrayList::Allocate 00437D60 (lane-renderer-bodies render_pool.h:106, identical declaration).
namespace kotor_renderer_gl { void __fastcall allocate_00437d60(kotor_reconstruction::ArrayMutations* self,void* unused_edx,std::uint32_t capacity); }
