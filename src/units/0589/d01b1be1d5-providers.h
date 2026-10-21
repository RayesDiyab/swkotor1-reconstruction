#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 only
#endif
namespace menu_providers_40_49 {
using U32=std::uint32_t;using U8=unsigned char;
struct AI {
    U32 app_bits,word04,list08,word0c;
    __declspec(noinline) AI* __thiscall construct_00604b20();
    __declspec(noinline) void __thiscall set_app_00604790(U32);
};
struct Options {
    U32 opaque00;U8 enabled04,opaque05[3];U32 flags08;U8 opaque0c[8];
    U32 flags14;U8 mode18,opaque19[3];U32 float1c,float20,word24,word28,word2c,word30,anisotropy34,word38;
    U8 byte3c,byte3d,opaque3e[2];U32 vsync40;U8 antialias44,byte45,byte46,byte47,byte48,opaque49[7];
    U8 byte50,opaque51[3];U32 word54;U8 byte58,opaque59[3];U32 flags5c;U8 array60[11],opaque6b[21],byte80,opaque81[7];U32 word88,word8c,word90;
    // Lane CClientOptions and original160-byte allocation; bit-preserving fields.
    U32 keyboard_camera_dps94,keyboard_camera_acceleration98,keyboard_camera_deceleration9c;
    __declspec(noinline) void __thiscall defaults_0061da60();
    __declspec(noinline) void __thiscall vsync_0061d8e0(U32);
    __declspec(noinline) void __thiscall anisotropy_0061d8c0(U32);
    __declspec(noinline) void __thiscall antialias_0061d710(U8);
};
struct Internal {
    alignas(4) U8 bytes[0x30c];
    U32 word(unsigned o)const{U32 v;std::memcpy(&v,bytes+o,4);return v;}
    void word(unsigned o,U32 v){std::memcpy(bytes+o,&v,4);}
    void* pointer(unsigned o)const{return reinterpret_cast<void*>(word(o));}
    __declspec(noinline) void __thiscall mouse_0061f980(U32,U32,U32);
};
static_assert(sizeof(void*)==4 && sizeof(AI)==16 && sizeof(Options)==0xa0,"observed source prefixes");
static_assert(offsetof(Options,flags14)==0x14 && offsetof(Options,anisotropy34)==0x34 && offsetof(Options,vsync40)==0x40 && offsetof(Options,antialias44)==0x44 && offsetof(Options,flags5c)==0x5c && offsetof(Options,array60)==0x60 && offsetof(Options,byte80)==0x80 && offsetof(Options,word90)==0x90,"instruction offsets");
struct Providers {
    // D1: original __thiscall slots as ABI-identical __fastcall(receiver, unused EDX, stack words).
    void*(__fastcall* list_construct_004b25d0)(void*,void*);
    // cdecl renderer setters typed as the originals: int AurSetAntiAliasingMode(int),
    // int AurSetAnisotropy(int), void AurSetVSyncState(char) (0044EE9A reads byte [esp+8]).
    int(__cdecl* aa_0044f2f0)(int);
    int(__cdecl* aniso_0044f2c0)(int);
    void(__cdecl* vsync_0044ee90)(U8);
    void(__fastcall* gui_0040b420)(void*,void*);
    void(__fastcall* options_0061d9e0)(Options*,void*);
    // Actual caller consumes EAX after005DF5C0 despite saved void label.
    U32(__fastcall* input_cursor_005df5c0)(void*,void*,U32,U32,U32);
    void(__fastcall* gui_cursor_0040a270)(void*,void*,U32,U32,U32);
    // Canonical cells are void* volatile (window_shared_cells.h); borrowed read-only.
    void* const volatile* gui_007a39f4;
    void* const volatile* input_007a39e4;
};
extern Providers providers; // Sole production binding owner required.
}

static_assert(offsetof(menu_providers_40_49::Options,keyboard_camera_dps94)==0x94 && offsetof(menu_providers_40_49::Options,keyboard_camera_acceleration98)==0x98 && offsetof(menu_providers_40_49::Options,keyboard_camera_deceleration9c)==0x9c,"Lane-backed camera fields; no truncated allocation view");
