#pragma once
#include "9136b2d0c5-area_vfx.hpp"
extern "C" {
void* __fastcall resman_string_from_int_005e5bc0(void*,void*,std::uint32_t);
void* __fastcall async_ref_construct_string_00406d60(void*,void*,const void*);
void __cdecl genuine_x87_ftol_006fae8c();
void __fastcall object_vfx_camera_00641730(void*,void*,std::uint32_t);
void __fastcall object_vfx_camera_006416f0(void*,void*,std::uint32_t,std::uint32_t);
}
namespace area_vfx_native {
using namespace kotor_w3;
// Exact local CExoString lifetime shell: actual canonical constructors/destructor.
struct LocalString {
    ExoString value;
    LocalString() { kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&value),nullptr); }
    explicit LocalString(U number) { resman_string_from_int_005e5bc0(&value,nullptr,number); }
    explicit LocalString(const char* text) { frame_LogStringCtor_005e5a90(&value,nullptr,text); }
    ~LocalString() noexcept(false) { frame_LogStringDtor_005e5c20(&value,nullptr); }
    LocalString(const LocalString&)=delete;
    LocalString& operator=(const LocalString&)=delete;
};
inline void* visual_table() { return rp(rp(kotor_cell_rules_007a3a28,0xb8),0x88); }
inline I32 milliseconds(float seconds) {
    const float multiplier=1000.0f;
    I32 result;
    __asm {
        fld dword ptr seconds
        fmul dword ptr multiplier
        call genuine_x87_ftol_006fae8c
        mov result,eax
    }
    return result;
}
}
