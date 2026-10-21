#pragma once
// lane-w3 (client world build) private header: raw-object access helpers and the declarations of the EXISTING
// runtime1115 providers that the W3 bodies call. Original: swkotor.exe GOG K1 (image base 0x400000), read-only
// static disassembly (capstone 5, lane-w3/tools/odis.py); nothing original was executed.
//
// Conventions (same as lane-m15-0/-a/-e, DECISIONS D1/D21/D24/D41 of the M60 client composition):
//  * every original __thiscall entry is an extern "C" __fastcall(Self*, void* unused_edx, <original stack words>)
//    veneer with identical callee cleanup (RET n noted per entry); callers pass nullptr for EDX;
//  * struct returns use the original hidden-return pointer as the first stack argument, returned in EAX;
//  * x87 ST0 returns are typed float;
//  * objects are opaque byte blocks touched only at the cited offsets through ld/st (memcpy, no type punning);
//  * memory: only the game allocator live_operator_new / live_free (006FA7E6 / 006FA390);
//  * every unimplemented callee is an explicit D83 trap (w3_traps.cpp), never a silent stub.
#include <cstdint>
#include <cstring>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error lane-w3 bodies require the Windows x86 (MSVC-ABI) compiler route
#endif
static_assert(sizeof(void*) == 4, "original pointer width");

namespace kotor_w3 {
using U = std::uint32_t;
using I32 = std::int32_t;
using U16 = std::uint16_t;
using U8 = std::uint8_t;

template <class T> inline T ld(const void* base, unsigned offset) noexcept {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset, sizeof value);
    return value;
}
template <class T> inline void st(void* base, unsigned offset, T value) noexcept {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value, sizeof value);
}
inline void* rp(const void* base, unsigned offset) noexcept { return ld<void*>(base, offset); }
inline U rd32(const void* base, unsigned offset) noexcept { return ld<U>(base, offset); }
inline void* at(void* base, unsigned offset) noexcept { return static_cast<unsigned char*>(base) + offset; }
inline const void* at(const void* base, unsigned offset) noexcept {
    return static_cast<const unsigned char*>(base) + offset;
}
inline float fbits(U bits) noexcept { float f; std::memcpy(&f, &bits, 4); return f; }
inline U bitsf(float f) noexcept { U u; std::memcpy(&u, &f, 4); return u; }
inline U word_of(const void* p) noexcept { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* word_ptr(U w) noexcept { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }

// Observed 8-byte CExoString and 16-byte CResRef storage (stack temporaries of the original frames).
struct ExoString { char* data; U length; };
static_assert(sizeof(ExoString) == 8, "CExoString");
struct ResRef { unsigned char bytes[16]; };
static_assert(sizeof(ResRef) == 16, "CResRef");
struct Vector3 { float x, y, z; };
static_assert(sizeof(Vector3) == 12, "Vector");

// Generic thiscall virtual-slot call helpers (object's first word = its table). Only the slot offsets cited by the
// original call sites are used; each call site names the original instruction.
template <class Fn> inline Fn vslot(const void* object, unsigned offset) noexcept {
    const void* table = rp(object, 0);
    return reinterpret_cast<Fn>(rp(table, offset));
}
}  // namespace kotor_w3

// menu_resources::Ref exactly as declared by the existing owner (resource-r1 resources.h) for its member veneer.
namespace kotor_reconstruction {struct StartupString;}
namespace c2da_current {struct Table;}
namespace menu_resources {
using String=kotor_reconstruction::StartupString;
struct Ref {unsigned char bytes[16];__declspec(noinline) Ref* __thiscall construct_00406d80(const char*);};
}
// The existing kotor_reconstruction CExoString default constructor owner (menu20 bindings).
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }
// The existing CExoString operator+ owner (ini-string-priority).
namespace kotor_reconstruction { struct StartupString; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString*, void*,
                                                                  kotor_reconstruction::StartupString*,
                                                                  const kotor_reconstruction::StartupString*);
// The existing CSWMessage read-state owner (0917-player_message.obj; SetReadMessage/Overflow/Underflow).
namespace recovered_player_message {
void initialize_read_004d66b0(void*, void*, unsigned, unsigned, unsigned);
unsigned overrun_004d6230(void*);
unsigned underrun_004d6240(void*);
}

extern "C" {
// ------------------------------------------------------------------ allocator / trap (existing owners)
void* __cdecl live_operator_new(std::uint32_t);                                   // 006FA7E6 operator new
void __cdecl live_free(void*);                                                     // 006FA390 free
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long original_address, const char* symbol);
// ------------------------------------------------------------------ CExoString (existing owners)
void __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);        // 005E5A90 CExoString(char*) RET 4
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                          // 005E5C20 ~CExoString
void* __fastcall async_assign_string_005e5c50(void* self, void*, const void* rhs);        // 005E5C50 operator=(CExoString&) RET 4
void* __fastcall c2da_assign_text_005e5140(void* self, void*, const char* text);          // 005E5140 operator=(char*) RET 4
char* __fastcall async_string_cstr_005e5670(void* self, void*);                           // 005E5670 CStr
void* __fastcall resman_string_counted_005e5b70(void* self, void*, const char* text, std::int32_t length);  // 005E5B70 RET 8
void* __fastcall r8_copy_construct_005e5b00(void* self, void*, const void* source);       // 005E5B00 CExoString(CExoString&) RET 4
int __fastcall async_string_equal_005e5200(void* self, void*, const void* rhs);           // 005E5200 operator==(CExoString&) RET 4
std::uint32_t __fastcall frame_LogIsEmpty_005e58a0(void* self, void*);                    // 005E58A0 IsEmpty
// ------------------------------------------------------------------ CResRef (existing owners)
void* __fastcall kotor_fastcall_00405ed0(void* self, void*);                               // 00405ED0 CResRef()
menu_resources::Ref* __fastcall c2da_copy_00406120(menu_resources::Ref* self, void*, menu_resources::Ref* out, menu_resources::Ref* source);    // 00406120 operator=(CResRef&) RET 8
char* __fastcall c2da_ref_00405fe0(void* self, void*);                                     // 00405FE0 GetResRefStr
void __fastcall gui_load_resref_copy_to_string_00405fb0(const menu_resources::Ref* self, void*, char* out_string);   // 00405FB0 CopyToString RET 4
int __fastcall c2da_not_equal_text_00406060(void* self, void*, const char* text);         // 00406060 operator!=(char*) RET 4
void* __fastcall SoundAssign00406170(void* self, void*, void* out, const void* bytes16);  // 00406170 operator=(char(&)[16]) RET 8
menu_resources::Ref* __fastcall unresolved_ref_assign_00406290(menu_resources::Ref* self, void*, menu_resources::Ref* out, const char* text);   // 00406290 operator=(char*) RET 8
// ------------------------------------------------------------------ TLK (existing owners)
extern void* kotor_cell_tlk_007a3a08;                                                     // [007A3A08] CTlkTable*
std::uint32_t __fastcall tlk_fetch_0041e550(void* tlk, void*, std::uint32_t strref, void* str_res,
                                            std::uint32_t gender);                         // 0041E550 RET 0Ch (AL only)
// ------------------------------------------------------------------ CSWMessage / CSWCMessage readers
// Single owner lane-w2 tier t2 (src/w2_message_native.cpp; RECEIPT.json "native_export"; coordinator ruling t2-1).
// Original thiscall, callee cleanup as noted; exported __fastcall(self, EDX unused, <stack words>). 004D6780 is W2's
// veneer onto the existing 0935 owner (server_mainloop::message_read_word_004d6780).
void __fastcall kotor_fastcall_004d6170(void* self, void*);                                   // ClearReadMessage   @8  RET
void* __fastcall kotor_fastcall_004d6180(void* self, void*, void* result, unsigned size);    // ReadCResRef        @16 RET 8 (hidden return)
unsigned __fastcall kotor_fastcall_004d6250(void* self, void*);                               // MessageMoreDataToRead @8 RET
unsigned __fastcall kotor_fastcall_004d66f0(void* self, void*);                               // ReadBOOL           @8  RET
unsigned char __fastcall kotor_fastcall_004d6720(void* self, void*, unsigned bits);           // ReadBYTE           @12 RET 4 (AL)
unsigned short __fastcall kotor_fastcall_004d6750(void* self, void*, unsigned bits);          // ReadWORD           @12 RET 4 (AX)
unsigned __fastcall kotor_fastcall_004d6780(void* self, void*, unsigned bits);                // ReadDWORD          @12 RET 4
float __fastcall kotor_fastcall_004d67b0(void* self, void*, unsigned scale, unsigned bits);   // ReadFLOAT          @16 RET 8 (ST0; scale = float bits)
void* __fastcall kotor_fastcall_004d67e0(void* self, void*, void* result, unsigned bits);     // ReadCExoString     @16 RET 8 (hidden return)
unsigned __fastcall kotor_fastcall_00692630(void* self, void*);                               // ReadOBJECTIDClient @8  RET
void* __fastcall kotor_fastcall_00692660(void* self, void*, void* result, void* sound,
                                         unsigned* length_out);                               // ReadCExoLocStringClient @20 RET 0Ch
// ------------------------------------------------------------------ lane-w3 (w3_message_read.cpp; W2 does not provide it)
void* __fastcall recovered_resref_from_bytes_00406da0(void* self, void*, const void* bytes16) noexcept;   // CResRef(char(&)[16]) RET 4
}

