#pragma once
#include "71ca184fe3-model_family_a.h"
#include "45d12a6f86-model_methods_a.h"
#include "5c51976762-model_family_b.h"
#include "cc436f6baa-m15e_animbase.h"
#include "94d6ff964a-scene3d.h"
#include <cstring>
#include <cstddef>
static_assert(sizeof(void*)==4,"original x86 object ABI");
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace model_family_a {
using U=std::uint32_t;using I=std::int32_t;
// Original address arithmetic is modulo32, including negative/unusual vtordisp words.
inline void* off(void* p,U o){const U a=static_cast<U>(reinterpret_cast<std::uintptr_t>(p));return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a+o));}
inline void* off(void* p,I o){return off(p,static_cast<U>(o));}
template<class T>T ld(const void* p,U o){T v;std::memcpy(&v,off(const_cast<void*>(p),o),sizeof v);return v;}
template<class T>T ld(const void* p,I o){return ld<T>(p,static_cast<U>(o));}
template<class T>void st(void* p,U o,T v){std::memcpy(off(p,o),&v,sizeof v);}
template<class T>void st(void* p,I o,T v){st(p,static_cast<U>(o),v);}
inline U vb(void* p,I o,unsigned index){return ld<const U*>(p,o)[index];}
inline void* tw(void* p){return off(p,vb(p,-8,1)-8);}
inline void* head(void* p){return off(p,vb(p,-8,2)+0x48);}
inline void* wield(void* p){return off(p,vb(p,-8,3)+0x54);}
inline const aurora_model::GobVTable* gvt(void* p){return ld<const aurora_model::GobVTable*>(p,0);}
inline aurora_model::Gob* gob(void* p){return static_cast<aurora_model::Gob*>(p);}
inline const KotorM15eAnimBaseVtbl* avt(void* p){return ld<const KotorM15eAnimBaseVtbl*>(p,0);}
inline U word(const void* p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
inline void* client(){return ld<void*>(const_cast<void*>(kotor_menu_window_cells::manager_007a39fc),4);}
}
extern "C" {
std::uint32_t __fastcall recovered_anim_base_internal_0069e8d0(void*,void*,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t) noexcept(false);
void __fastcall recovered_anim_base_turn_off_shadows_0069dc30(void*,void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_wield_destruct_00699ed0(void*,void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_tw_destruct_0069cd10(void*,void*) noexcept(false);
void __fastcall kotor_m15e_anim_base_refresh_hilite_0069ec00(void*,void*) noexcept(false);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*,void*,std::uint32_t) noexcept(false);
void __cdecl live_free(void*);
}
