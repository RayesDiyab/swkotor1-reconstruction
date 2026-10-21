#pragma once
#include <cstddef>
#include <cstring>
namespace options_video {
using U8=unsigned char; using U16=unsigned short; using U32=unsigned int; using I32=int;
static_assert(sizeof(void*)==4 && sizeof(U32)==4,"Windows x86 only");
struct String { char* text; U32 length; };
struct Ini { void* state; };
struct ResourceRef { U8 bytes[16]; };
// Minimum addressed views, not recovered full game class declarations.
// Client = CClientExoAppInternal: full original extent 0x4D4 (operator new
// 0x4D4 at 005EE309 before ctor 005FBFE0); bytes are opaque (lane-config P2).
struct alignas(4) Client {
    U8 bytes[0x4d4];
    void __thiscall ReadAndSetVideoMode_005f5ab0(I32 forMovie,I32 fullscreen);
    void __thiscall InitializeClientOptions_005f1c20();
    I32 __thiscall SetTexturePack_005f14a0(U8 index,I32 unused);
};
struct alignas(4) Table { U8 bytes[0x54]; };
inline void* PointerAt(const void* receiver,std::size_t offset) {
    void* value;std::memcpy(&value,static_cast<const U8*>(receiver)+offset,4);return value;
}
inline I32 IntegerAt(const void* receiver,std::size_t offset) {
    I32 value;std::memcpy(&value,static_cast<const U8*>(receiver)+offset,4);return value;
}
// D1: original __thiscall slots as ABI-identical __fastcall(self, unused EDX, ...).
using DefaultString=String* (__fastcall*)(String*,void*);
using TextString=String* (__fastcall*)(String*,void*,const char*);
using DestroyString=void (__fastcall*)(String*,void*);
using ConstructIni=Ini* (__fastcall*)(Ini*,void*);
using DestroyIni=void (__fastcall*)(Ini*,void*);
static_assert(sizeof(Client)==0x4d4,"005EE309 operator new 0x4D4");
static_assert(sizeof(String)==8 && sizeof(Ini)==4 && sizeof(ResourceRef)==16,"Borrowed layout");
}
