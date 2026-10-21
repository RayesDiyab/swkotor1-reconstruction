// lane-m15-0 unit m15-l0-leaves: shared Milestone-1.5 leaves (containers, small server leaves, C2DA string
// entry, party session time). Original: swkotor.exe GOG K1, static disassembly only (nothing executed).
// Every statement cites the original instruction(s) it reproduces.
#include "f155d76cd432eed7_m15_l0.h"
#include "e1805047c9b7563b_startup_string.h"   // M41 kotor_reconstruction::StartupString (005B3190 / 005E5C20 / 005E5C50 / 005E5290 / 005E5790)
#include "d3bcdb58b43a10ee_constructors.h"     // task-37 kotor_list::Internal::Construct = 005E9AA0 (canonical-list-constructors)
#include <cstring>
#include <new>
#include <windows.h>

extern "C" {
void* __cdecl live_operator_new(std::uint32_t);        // 006FA7E6 (live-policy.obj)
void __cdecl live_free(void*);                          // 006FA390 (live-policy.obj)
// 005E5140 CExoString::operator=(const char*) (assign-existing-carrier.obj), RET 4, EAX=this.
kotor_reconstruction::StartupString* __fastcall c2da_assign_text_005e5140(kotor_reconstruction::StartupString*, void*,
                                                                          const char*) noexcept(false);
// 005E5600 CExoString::operator[](int) -> char (alternatename to directory_list::At005e5600), RET 4.
char __fastcall c2da_current_string_index_005e5600(kotor_reconstruction::StartupString*, void*, std::int32_t);
// 00413100 C2DA::GetColumnIndex(const CExoString&) (startup-misc-c2da.obj), RET 4.
std::int32_t __fastcall c2da_current_column_index_00413100(void* table, void*, kotor_reconstruction::StartupString* column);
// 0073D71C "" (.rdata empty string; owner resource-borrowed-accessors / tlk-fetch).
}
namespace options_video { struct String; }
// 004138B0 C2DA::GetRowIndex(const CExoString&) (lane-config-movie-list.obj), RET 4.
namespace client_movies { int __fastcall C2daGetRowIndex_004138b0(void* table, void* unused_edx, options_video::String* label); }

// 007A3A20 / 007A3A24: CSWPartyTable current-session start time (FILETIME low/high; .data, image value 0).
// Writers: 00563D0F/00563D15 (this unit), 00564A3A/00564A40; reader 00563C10/00563C18. No M60 owner: defined here
// as two 32-bit scalars (consumers must declare them as std::uint32_t scalars, never arrays/pointers - D117).
extern "C" {
extern volatile std::uint32_t kotor_cell_party_session_start_lo_007a3a20;
extern volatile std::uint32_t kotor_cell_party_session_start_hi_007a3a24;
volatile std::uint32_t kotor_cell_party_session_start_lo_007a3a20 = 0;
volatile std::uint32_t kotor_cell_party_session_start_hi_007a3a24 = 0;
}

namespace {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using String = kotor_reconstruction::StartupString;
unsigned char* at(void* p, unsigned off) { return static_cast<unsigned char*>(p) + off; }
U32 rd(void* p, unsigned off) { U32 v; std::memcpy(&v, at(p, off), 4); return v; }
void wr(void* p, unsigned off, U32 v) { std::memcpy(at(p, off), &v, 4); }
void* rp(void* p, unsigned off) { void* v; std::memcpy(&v, at(p, off), 4); return v; }
void wp(void* p, unsigned off, const void* v) { std::memcpy(at(p, off), &v, 4); }
float rf(void* p, unsigned off) { float v; std::memcpy(&v, at(p, off), 4); return v; }
std::uint16_t rh(void* p, unsigned off) { std::uint16_t v; std::memcpy(&v, at(p, off), 2); return v; }
void wh(void* p, unsigned off, std::uint16_t v) { std::memcpy(at(p, off), &v, 2); }
const char kEmpty[] = "";   // 0073D71C (pushed as the empty C string)

// CExoArrayList<unsigned short> / <short>: +0 data, +4 num, +8 alloc (004D2FA7..004D2FC9).
void u16_grow(void* list) {
    const U32 alloc = rd(list, 8);                                          // 004D4333 / 005A7697 / 006169A3
    kotor_m15_array_u16_allocate_004d2fa0(list, nullptr,
        static_cast<I32>(alloc != 0 ? alloc + alloc : 0x10u));              // 004D433B..4B (ADD EAX,EAX / MOV EAX,10h)
}
}

extern "C" {
// ---- 004CC310 CSWSObject::AttachmentFromClientObject(id), RET 4.
void __fastcall kotor_m15_sobject_attachment_from_client_004cc310(void* self, void*, U32 id) {
    wr(self, 0x224, id);                                                    // 004CC310..004CC314
}

// ---- 004D2FA0 CExoArrayList<unsigned short>::Allocate(int), RET 4.
void __fastcall kotor_m15_array_u16_allocate_004d2fa0(void* list, void*, I32 new_size) {
    wr(list, 8, static_cast<U32>(new_size));                                // 004D2FA7
    void* const old_data = rp(list, 0);                                     // 004D2FAB
    void* const data = live_operator_new(static_cast<U32>(new_size) + static_cast<U32>(new_size)); // 004D2FAD..004D2FB0
    wp(list, 0, data);                                                      // 004D2FB8
    for (I32 i = 0; i < static_cast<I32>(rd(list, 4)); ++i)                 // 004D2FBD..004D2FD3 (re-reads +4 each pass)
        wh(rp(list, 0), static_cast<unsigned>(i) * 2u, rh(old_data, static_cast<unsigned>(i) * 2u)); // 004D2FC3..004D2FC9
    if (old_data != nullptr) live_free(old_data);                           // 004D2FD5..004D2FDA
}

// ---- 004D4330 CExoArrayList<unsigned short>::Add(unsigned short), RET 4.
void __fastcall kotor_m15_array_u16_add_004d4330(void* list, void*, U32 value) {
    if (rd(list, 4) == rd(list, 8)) u16_grow(list);                         // 004D4333..004D434B
    wh(rp(list, 0), rd(list, 4) * 2u, static_cast<std::uint16_t>(value));    // 004D4350..004D435A
    wr(list, 4, rd(list, 4) + 1u);                                          // 004D435E
}

// ---- 005A7670 CExoArrayList<unsigned short>::AddUnique(unsigned short), RET 4.
void __fastcall kotor_m15_array_u16_add_unique_005a7670(void* list, void*, U32 value) {
    const std::uint16_t v = static_cast<std::uint16_t>(value);              // 005A7671 MOV BX,[ESP+8]
    const I32 num = static_cast<I32>(rd(list, 4));                          // 005A7679
    if (num > 0) {                                                          // 005A767F..005A7681
        I32 matches = 0;                                                    // 005A767D
        unsigned char* p = static_cast<unsigned char*>(rp(list, 0));        // 005A7683
        for (I32 n = num; n != 0; --n, p += 2) {                            // 005A7685..005A7691 (whole list)
            std::uint16_t e; std::memcpy(&e, p, 2);
            if (e == v) ++matches;                                          // 005A7687..005A768C
        }
        if (matches != 0) return;                                           // 005A7693..005A7695
    }
    if (static_cast<U32>(num) == rd(list, 8)) u16_grow(list);               // 005A7697..005A76AE
    wh(rp(list, 0), rd(list, 4) * 2u, v);                                   // 005A76B3..005A76B8
    wr(list, 4, rd(list, 4) + 1u);                                          // 005A76BC
}

// ---- 006169A0 CExoArrayList<short>::Insert(short value, int index), RET 8.
void __fastcall kotor_m15_array_i16_insert_006169a0(void* list, void*, U32 value, I32 index) {
    I32 i = static_cast<I32>(rd(list, 4));                                  // 006169A7 EDI = num
    if (static_cast<U32>(i) == rd(list, 8)) u16_grow(list);                 // 006169AA..006169BE (004D2FA0)
    const std::uint16_t v = static_cast<std::uint16_t>(value);              // 006169C8
    wh(rp(list, 0), rd(list, 4) * 2u, v);                                   // 006169C3..006169CD (append first)
    wr(list, 4, rd(list, 4) + 1u);                                          // 006169D1
    if (i > index) {                                                        // 006169D4..006169DA (JLE skips)
        do {                                                                // 006169E0..006169F0
            unsigned char* d = static_cast<unsigned char*>(rp(list, 0));
            std::memcpy(d + static_cast<unsigned>(i) * 2u, d + static_cast<unsigned>(i - 1) * 2u, 2);
            --i;
        } while (i > index);
    }
    wh(rp(list, 0), static_cast<unsigned>(index) * 2u, v);                  // 006169F3..006169F6
}

// ---- 006F2A20 CExoArrayList<unsigned short>::IndexOf(unsigned short) (Ghidra FUN_006f2a20), RET 4.
I32 __fastcall kotor_m15_array_u16_index_of_006f2a20(void* list, void*, U32 value) {
    const I32 num = static_cast<I32>(rd(list, 4));                          // 006F2A20
    I32 i = 0;                                                              // 006F2A23
    if (num > 0) {                                                          // 006F2A25..006F2A28
        const unsigned char* p = static_cast<const unsigned char*>(rp(list, 0)); // 006F2A2A
        const std::uint16_t v = static_cast<std::uint16_t>(value);          // 006F2A2C
        do {
            std::uint16_t e; std::memcpy(&e, p, 2);
            if (e == v) return i;                                           // 006F2A31..006F2A34 -> 006F2A41
            ++i; p += 2;                                                    // 006F2A36..006F2A37
        } while (i < num);                                                  // 006F2A3A..006F2A3C
    }
    return -1;                                                              // 006F2A3E OR EAX,-1
}

// ---- 004EC370 CSWSCreature::GetMovementRateFactor() -> float, RET 0.
// fld [ecx+A08h]; < 0.125 (0073F400) -> 0.125; > 1.5 (0073F414) -> 1.5; else (incl. NaN: JP/JNE paths) the value.
float __fastcall kotor_m15_screature_get_movement_rate_factor_004ec370(void* self, void*) {
    const float v = rf(self, 0xA08);                                        // 004EC370
    if (v < 0.125f) return 0.125f;                                          // 004EC376..004EC38B (TEST AH,5 / JP)
    if (v > 1.5f) return 1.5f;                                              // 004EC38C..004EC39B (TEST AH,41h / JNE)
    return v;                                                               // 004EC3A1
}

// ---- 00552280 CCombatInformation::CCombatInformation(), RET 0, EAX=this.
// EH frame 0071D68C: state 1 after the +34h string, state 2 after +3Ch (both destroyed on unwind).
void* __fastcall kotor_m15_combat_information_construct_00552280(void* self, void*) noexcept(false) {
    wr(self, 0x18, 0); wr(self, 0x1C, 0); wr(self, 0x14, 0);                // 005522A2..005522A8
    wr(self, 0x24, 0); wr(self, 0x28, 0); wr(self, 0x20, 0);                // 005522AF..005522B5
    String* const s34 = ::new (static_cast<void*>(at(self, 0x34))) String();   // 005522B8..005522C2 (005B3190)
    String* const s3c = ::new (static_cast<void*>(at(self, 0x3C))) String();   // 005522C7..005522D1 (005B3190)
    *at(self, 9) = 3;                                                       // 005522D6..005522DC
    *at(self, 0) = 0; *at(self, 1) = 0; *at(self, 2) = 0; *at(self, 3) = 0; *at(self, 4) = 0; // 005522EB..005522F6
    wr(self, 0x0C, 0);                                                      // 005522F9
    *at(self, 5) = 0; *at(self, 6) = 0; *at(self, 7) = 0;                   // 005522FC..00552302
    *at(self, 8) = 1;                                                       // 00552305
    *at(self, 0x0A) = 0;                                                    // 00552309
    *at(self, 0x0B) = 2;                                                    // 0055230C
    *at(self, 0x10) = 0;                                                    // 00552310
    *at(self, 0x11) = 2;                                                    // 00552313
    wr(self, 0x2C, 0x7F000000u); wr(self, 0x30, 0x7F000000u);               // 005522DF / 00552317..0055231A (OBJECT_INVALID)
    try {
        (void)c2da_assign_text_005e5140(s3c, nullptr, kEmpty);              // 005522E4..0055231D
        (void)c2da_assign_text_005e5140(s34, nullptr, kEmpty);              // 00552322..00552329
    } catch (...) {
        s3c->~String(); s34->~String();                                     // unwind states 2 -> 1 (0071D68C)
        throw;
    }
    *at(self, 0x44) = 0; *at(self, 0x45) = 0;                               // 00552333..00552336
    return self;                                                            // 00552339
}

// ---- 00563CF0 CSWPartyTable::ResetCurrentSessionStartTime() (static), RET 0.
void __cdecl kotor_m15_party_reset_session_start_time_00563cf0() {
    FILETIME now;                                                           // [esp+4] 8 bytes
    GetSystemTimeAsFileTime(&now);                                          // 00563CF4..00563CF9 call [0073D194]
    kotor_cell_party_session_start_lo_007a3a20 = now.dwLowDateTime;         // 00563CFF..00563D0F (OR with 0)
    kotor_cell_party_session_start_hi_007a3a24 = now.dwHighDateTime;        // 00563D03..00563D15
}

// ---- 0063D7B0 CExoLinkedList<CSWCVisualEffectOnObject>::CExoLinkedList(), RET 0, EAX=this.
// new(0Ch) (006FA7E6); non-null -> CExoLinkedListInternal ctor 005E9AA0; [this] = result or 0 (0063D7C7..0063D806).
void* __fastcall kotor_m15_vfx_list_construct_0063d7b0(void* self, void*) noexcept(false) {
    void* const raw = live_operator_new(0x0C);                              // 0063D7C7..0063D7CB
    kotor_list::Internal* internal = nullptr;
    if (raw != nullptr)                                                     // 0063D7D7..0063D7E1
        internal = static_cast<kotor_list::Internal*>(raw)->Construct();    // 0063D7E3..0063D7E5 (005E9AA0; cannot throw)
    wp(self, 0, internal);                                                  // 0063D7EA / 0063D804
    return self;                                                            // 0063D7EC / 0063D806
}

// ---- 00413DE0 C2DA::GetCExoStringEntry(const CExoString& row, const CExoString& column, CExoString* out), RET 0Ch.
// C2DA fields used: +1C default CExoString, +28 column count, +38 row table (CExoString* per row, 8 bytes per cell),
// +3C binary flag, +40 string pool, +44 offset table (WORD per cell).
I32 __fastcall kotor_m15_c2da_get_string_entry_00413de0(void* table, void*, void* row_label, void* column,
                                                        void* out) noexcept(false) {
    String* const result = static_cast<String*>(out);
    const I32 row = client_movies::C2daGetRowIndex_004138b0(table, nullptr,
                                                            static_cast<options_video::String*>(row_label)); // 00413DE0..00413DEA
    const I32 col = c2da_current_column_index_00413100(table, nullptr, static_cast<String*>(column)); // 00413DEF..00413DF8
    if (row == -1 || col == -1) {                                           // 00413DFD..00413E09
        result->Assign005e5c50(reinterpret_cast<String*>(at(table, 0x1C))); // 00413EA6..00413EAE
        return 0;                                                           // 00413EB5
    }
    if (rd(table, 0x3C) != 0) {                                             // 00413E0F..00413E14
        const U32 cell = rd(table, 0x28) * static_cast<U32>(row) + static_cast<U32>(col); // 00413E16..00413E1F
        const char* text = static_cast<const char*>(rp(table, 0x40)) + rh(rp(table, 0x44), cell * 2u); // 00413E19..00413E28
        String* const assigned = c2da_assign_text_005e5140(result, nullptr, text); // 00413E2D..00413E31
        const char first = c2da_current_string_index_005e5600(assigned, nullptr, 0); // 00413E2A / 00413E36..00413E38
        return first != 0 ? 1 : 0;                                          // 00413E3D..00413E44 (NEG/SBB/NEG)
    }
    String* const entry = reinterpret_cast<String*>(static_cast<unsigned char*>(rp(rp(table, 0x38), static_cast<unsigned>(row) * 4u))
                                                    + static_cast<unsigned>(col) * 8u); // 00413E49..00413E5B
    if (entry->BorrowedEquals005e5290(kEmpty) != 0) {                       // 00413E56..00413E64 (005E5290)
        (void)c2da_assign_text_005e5140(result, nullptr, kEmpty);          // 00413E66..00413E6F
        return 0;                                                           // 00413E76
    }
    result->Assign005e5c50(entry);                                          // 00413E7C..00413E8B
    return result->BorrowedLength005e5790() > 0 ? 1 : 0;                    // 00413E90..00413E9B (SETG)
}
}
