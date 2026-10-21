// lane-m15-b (L15-B, Milestone 1.5) unit m15b-stats:
// CSWCCreatureStats (ctor 0064AFC0, dtor 00649E40, accessors), CSWCCreatureStats_ClassInfo,
// CSWCLevelUpStats (ctor 0064B2D0, dtor 0064B400, char-gen / level-up rules) and the typed vtables
// 00751C60 / 00751C64. Original: swkotor.exe GOG K1 (image base 0x400000); every body cites its
// instruction addresses (static dumpbin disassembly, lane work\asm\<addr>.asm). D1 fastcall veneers.
//
// Fidelity notes:
// * Ability modifiers: original = fild BYTE; fmul qword 0073F3D0 (0.5); floor 006FC750; _ftol2 006FAE8C;
//   sub 5. Ported as std::floor (exact IEEE op, precedent lane-3d-bsp) + truncating conversion (_ftol2
//   semantics; the floored value is integral and within int range) -- bit-identical results (D30-FINAL).
// * Byte/word arithmetic keeps the original register widths (al/ax wrap, movsx/movzx extensions).
// * Inlined GetClass/GetClassLevel(i) (index >= 2 -> FF / 0) are reproduced inline as in the original.
// * Class element addresses are formed as classes + id*1A0h and null-tested only where the original
//   tests the sum (add/je); elsewhere they are used unchecked exactly as the original does.
// * Unwind (D30-FINAL): the two constructors mirror the original FuncInfo unwind maps (0077B290,
//   0077B2BC, 0077B100) with explicit catch blocks; string temporaries use C++ locals whose destruction
//   order equals the original's; CanChooseFeat's local CExoArrayList is freed on every exit (0064A6D6 /
//   0064A747) and has no throwing callee in between.
#include "bf662019daaaa2b6_m15b_stats.h"
#include "e1805047c9b7563b_startup_string.h"   // kotor_reconstruction::StartupString
#include "c53472623c7d6765_resources.h"        // menu_resources::Ref
#include "20e68b036c30980c_allocator_live.h"   // live_operator_new 006FA7E6 / live_free 006FA390
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

using kotor_reconstruction::StartupString;
using menu_resources::Ref;

// ---- existing owners (declarations identical in linkage/ABI to the owners' declarations) ----
extern "C" {
extern void* kotor_cell_rules_007a3a28;          // [007A3A28] g_pRules (lane-prepare, sole owner)
extern std::uint32_t kotor_rules_cell32_007a2124; // [007A2124] cached C2DA column index (lane-rules two_dim_arrays)
StartupString* __fastcall GuiAssign005e5140(StartupString*, void*, const char*);               // 005E5140 CExoString::operator=(char const*)
StartupString* __fastcall resman_string_from_int_005e5bc0(StartupString*, void*, int);         // 005E5BC0 CExoString(int) (lane-resman)
Ref* __fastcall c2da_default_ref_00405ed0(Ref*, void*);                                         // 00405ED0 CResRef()
Ref* __fastcall unresolved_ref_assign_00406290(Ref*, void*, Ref*, const char*);                // 00406290 CResRef::operator=(char const*)
Ref* __fastcall c2da_copy_00406120(Ref*, void*, Ref*, Ref*);                                    // 00406120 CResRef::operator=(CResRef)
const char* __cdecl c2da_empty_0073d71c();                                                       // .rdata "" 0073D71C
std::int32_t __fastcall c2da_current_get_int_entry_00413510(void* table, void*, std::int32_t row, std::int32_t column,
                                                             std::int32_t* out);                // 00413510 C2DA::GetINTEntry(int,int,int*) RET 0Ch (lane-tlk)
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, std::uint32_t size, std::int32_t count,
    void* (__fastcall* construct)(void*, void*), void (__fastcall* destroy)(void*, void*));      // 006FB55F (CRT port)
void __stdcall c2da_destroy_array_006fa852(void*, std::uint32_t, std::uint32_t,
    void (__fastcall*)(StartupString*, void*));                                                 // 006FA852 (CRT port; String-typed callback, D24)
void __fastcall exo_array_list_destruct_0056b6f0(void*, void*);                                // 0056B6F0 CExoArrayList::~CExoArrayList (lane-3d-render)
void __fastcall aurora_list_add16_00671c00(void* list, void*, std::uint32_t value);            // 00671C00 CExoArrayList<DWORD>::Add RET 4
// lane-m15-0 (L15-0) owners, copied verbatim from lane-m15-0\src\m15_l0.h:
void __fastcall kotor_m15_array_u16_allocate_004d2fa0(void* list, void*, std::int32_t new_size);
void __fastcall kotor_m15_array_u16_add_unique_005a7670(void* list, void*, std::uint32_t value);
void* __fastcall kotor_m15_combat_information_construct_00552280(void* self, void*) noexcept(false);
}
void* __fastcall exo_array_list_construct_005a76d0(void*, void*);                               // 005A76D0 CExoArrayList::CExoArrayList (lane-3d-model)
StartupString* __fastcall GuiConcat005e5d10(StartupString*, void*, StartupString*, const StartupString*); // 005E5D10 operator+
namespace kotor_rules {
void* __fastcall kotor_fastcall_00550c00(void* rules, void* unused_edx, std::uint16_t feat);   // 00550C00 CSWRules::GetFeat
void __fastcall kotor_fastcall_004015a0(void* self, void* unused_edx);                         // 004015A0 (empty dtor body; ~CSWPortrait)
}

namespace {
using Word = std::uint32_t;
template <class T> inline T ld(const void* base, std::uintptr_t off) noexcept {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(base) + off, sizeof(T)); return v;
}
template <class T> inline void st(void* base, std::uintptr_t off, T v) noexcept {
    std::memcpy(static_cast<unsigned char*>(base) + off, &v, sizeof(T));
}
inline unsigned char* at(void* base, std::uintptr_t off) noexcept { return static_cast<unsigned char*>(base) + off; }
inline StartupString* str_at(void* base, std::uintptr_t off) noexcept { return reinterpret_cast<StartupString*>(at(base, off)); }
inline Ref* ref_at(void* base, std::uintptr_t off) noexcept { return reinterpret_cast<Ref*>(at(base, off)); }
inline void* addr(std::uintptr_t a) noexcept { return reinterpret_cast<void*>(a); }

// ---- .rdata values used by these bodies (image values, read via tools pe.py) ----
constexpr double kHalf_0073f3d0 = 0.5;              // qword 0073F3D0 = 3FE0000000000000
const std::uint16_t kWord_0074bf14 = 6;             // word 0074BF14 (CSWCCreatureStats default race)
const std::uint8_t kByte_0074b319 = 0;              // byte 0074B319 (default class of slot 0)
const std::uint16_t kWord_0074bf10 = 5;             // word 0074BF10 (race compared by AddDefaultPackageSkills)
constexpr char kSpace_0073d5d8[] = " ";             // 0073D5D8
constexpr char kOpen_00748ea4[] = " (";             // 00748EA4
constexpr char kClose_0074f788[] = ")";             // 0074F788
constexpr float kFloat54Init = 6.0f;                // 0064B170 mov [esi+54h],40C00000h

// ---- CSWRules views ([007A3A28]) ----
inline void* rules() noexcept { return kotor_cell_rules_007a3a28; }
inline std::uint8_t skill_count(void* r) noexcept { return ld<std::uint8_t>(r, 0xAB); }
inline std::uintptr_t skill_entry(void* r, Word skill) noexcept { return static_cast<std::uintptr_t>(skill) * 0x20u + ld<std::uintptr_t>(r, 0xB4); }
inline Word skill_flags(std::uintptr_t entry) noexcept { return ld<Word>(addr(entry), 0x1C); }
inline std::uintptr_t class_addr(void* r, std::uint8_t id) noexcept { return static_cast<std::uintptr_t>(id) * 0x1A0u + ld<std::uintptr_t>(r, 0xAC); }
inline unsigned char* race_entry(void* r, std::uint16_t race) noexcept { return ld<unsigned char*>(r, 0xB0) + static_cast<std::uintptr_t>(race) * 0x34u; }

// ---- CSWCCreatureStats inline views ----
inline std::uint8_t class_count(void* s) noexcept { return ld<std::uint8_t>(s, 0x32); }
inline std::uint16_t race_of(void* s) noexcept { return ld<std::uint16_t>(s, 0x24); }
// Inlined GetClass(i) / GetClassLevel(i) (00647730 / 00647750 bodies).
inline std::uint8_t class_at(void* s, std::uint8_t i) noexcept { return i < 2 ? ld<std::uint8_t>(s, 0xF0u + static_cast<unsigned>(i) * 0x20u) : 0xFF; }
inline std::uint8_t level_at(void* s, std::uint8_t i) noexcept { return i < 2 ? ld<std::uint8_t>(s, 0xF1u + static_cast<unsigned>(i) * 0x20u) : 0; }
inline unsigned char* ranks_mod(void* s) noexcept { return ld<unsigned char*>(s, 0x84); }
inline unsigned char* ranks_base(void* s) noexcept { return ld<unsigned char*>(s, 0x88); }
inline std::uint16_t skill_points(void* s) noexcept { return ld<std::uint16_t>(s, 0xAE); }

// fild BYTE; fmul 0.5; floor 006FC750; _ftol2 006FAE8C (full 32-bit result, before the "sub 5").
inline std::int32_t half_floor(std::uint8_t v) noexcept {
    return static_cast<std::int32_t>(std::floor(static_cast<double>(v) * kHalf_0073f3d0));
}

// CExoArrayList<T>::DelIndex inlined by the original (count stored first, then the tail shifted down).
template <class T> inline void del_index(void* list, std::int32_t index) noexcept {
    const std::int32_t n = ld<std::int32_t>(list, 4) - 1;
    st<std::int32_t>(list, 4, n);
    for (std::int32_t k = index; k < ld<std::int32_t>(list, 4); ++k) {
        T* const data = ld<T*>(list, 0);
        data[k] = data[k + 1];
    }
}

// D24 typed adapters for 006FA852's String-typed element callback: run exactly the owning body on
// the same element address (object-pointer conversion only; precedent lane-rules rules_dtor.cpp).
void __fastcall array_list_destroy_element_0056b6f0(StartupString* element, void*) {
    exo_array_list_destruct_0056b6f0(element, nullptr);
}
void __fastcall class_info_destroy_element_006495f0(StartupString* element, void*) {
    kotor_m15b_class_info_destruct_006495f0(element, nullptr);
}

// Raw storage for a CExoString constructed by a callee (hidden-return targets of 005E5BC0 / 005E5D10 /
// GetNameText); destroyed (005E5C20) at scope exit once constructed, in reverse declaration order.
struct StringSlot {
    alignas(StartupString) unsigned char raw[sizeof(StartupString)];
    bool live = false;
    StartupString* get() noexcept { return reinterpret_cast<StartupString*>(raw); }
    StartupString* adopt(void* constructed) noexcept { live = true; return static_cast<StartupString*>(constructed); }
    StringSlot() = default;
    StringSlot(const StringSlot&) = delete;
    StringSlot& operator=(const StringSlot&) = delete;
    ~StringSlot() { if (live) get()->~StartupString(); }
};
}  // namespace

extern "C" {

// ================================================================== vtables
// 00751C60: [0064B2B0] (CSWCCreatureStats); 00751C64: [0064B3E0] (CSWCLevelUpStats). One slot each:
// the next words (0063E7F0, 00654CA0) begin other classes' tables (00751C68 / 00751C6C).
const KotorM15bStatsMethods kotor_m15b_creature_stats_vtable_00751c60 = { &kotor_m15b_creature_stats_deleting_destruct_0064b2b0 };
const KotorM15bStatsMethods kotor_m15b_levelup_stats_vtable_00751c64 = { &kotor_m15b_levelup_stats_deleting_destruct_0064b3e0 };

// ================================================================== CSWCCreatureStats_ClassInfo
// ---- 0064A050 CSWCCreatureStats_ClassInfo::CSWCCreatureStats_ClassInfo(), RET, EAX = this ----
void* __fastcall kotor_m15b_class_info_construct_0064a050(void* self, void*) noexcept(false) {
    crt_eh_vector_constructor_006fb55f(self, 0x0C, 1, &exo_array_list_construct_005a76d0,       // 0064A068..0064A07D
                                       &exo_array_list_destruct_0056b6f0);
    try {                                                                                        // state 0 (0064A096)
        crt_eh_vector_constructor_006fb55f(at(self, 0x0C), 0x0C, 1, &exo_array_list_construct_005a76d0, // 0064A082..0064A09A
                                           &exo_array_list_destruct_0056b6f0);
    } catch (...) {
        c2da_destroy_array_006fa852(self, 0x0C, 1, &array_list_destroy_element_0056b6f0);        // unwind 0072A470 (FuncInfo 0077B100)
        throw;
    }
    st<std::uint8_t>(self, 0x1C, 0);                                                             // 0064A0A1
    st<std::uint8_t>(self, 0x1D, 0);                                                             // 0064A0A4
    st<std::uint8_t>(self, 0x19, 0);                                                             // 0064A0A7
    st<std::uint8_t>(self, 0x1A, 0xFF);                                                          // 0064A0AA
    st<std::uint8_t>(self, 0x1B, 0xFF);                                                          // 0064A0AD
    st<std::uint8_t>(self, 0x18, 0);                                                             // 0064A0B0..0064A0B3
    return self;                                                                                 // 0064A0B9
}

// ---- 006488B0 CSWCCreatureStats_ClassInfo::ClearKnownSpells(), RET ----
void __fastcall kotor_m15b_class_info_clear_known_spells_006488b0(void* self, void*) {
    for (std::int32_t i = ld<std::int32_t>(self, 4) - 1; i >= 0; --i)                            // 006488B1..006488B5 / 006488DA..006488DB
        del_index<Word>(self, i);                                                                // 006488B8..006488D8
}

// ---- 006495F0 CSWCCreatureStats_ClassInfo::~CSWCCreatureStats_ClassInfo(), RET ----
void __fastcall kotor_m15b_class_info_destruct_006495f0(void* self, void*) {
    kotor_m15b_class_info_clear_known_spells_006488b0(self, nullptr);                           // 00649615
    c2da_destroy_array_006fa852(at(self, 0x0C), 0x0C, 1, &array_list_destroy_element_0056b6f0); // 0064961A..00649627
    c2da_destroy_array_006fa852(self, 0x0C, 1, &array_list_destroy_element_0056b6f0);           // 0064962C..0064963E
}

// ================================================================== CSWCCreatureStats
// ---- 0064AFC0 CSWCCreatureStats::CSWCCreatureStats(), RET, EAX = this ----
void* __fastcall kotor_m15b_creature_stats_construct_0064afc0(void* self, void*) noexcept(false) {
    // Unwind map 0077B290 (11 states): A free(block) -> 9 ~+114 -> 8 destroy ClassInfo[2] -> 7 ~list C8
    // -> 6 ~list BC -> 5 ~list B0 -> 4 ~+A0 -> 3 ~portrait(004015A0) -> 2 ~+28 -> 1 ~+1C -> 0 ~+14.
    struct Unwind {
        void* self; int state = -1; void* block = nullptr;
        ~Unwind() {
            for (; state >= 0; --state) {
                switch (state) {
                case 10: live_free(block); break;                                                       // 0072A61A
                case 9: str_at(self, 0x114)->~StartupString(); break;                                   // 0072A60C
                case 8: c2da_destroy_array_006fa852(at(self, 0xD4), 0x20, 2, &class_info_destroy_element_006495f0); break; // 0072A5F4
                case 7: exo_array_list_destruct_0056b6f0(at(self, 0xC8), nullptr); break;               // 0072A5E6
                case 6: exo_array_list_destruct_0056b6f0(at(self, 0xBC), nullptr); break;               // 0072A5D8
                case 5: exo_array_list_destruct_0056b6f0(at(self, 0xB0), nullptr); break;               // 0072A5CA
                case 4: str_at(self, 0xA0)->~StartupString(); break;                                    // 0072A5BC
                case 3: kotor_rules::kotor_fastcall_004015a0(at(self, 0x70), nullptr); break;          // 0072A5B1
                case 2: str_at(self, 0x28)->~StartupString(); break;                                    // 0072A5A6
                case 1: str_at(self, 0x1C)->~StartupString(); break;                                    // 0072A59B
                case 0: str_at(self, 0x14)->~StartupString(); break;                                    // 0072A590
                default: break;
                }
            }
        }
    } unwind{self};

    st<const void*>(self, 0x00, &kotor_m15b_creature_stats_vtable_00751c60);                   // 0064AFE7 vptr 00751C60
    c2da_default_ref_00405ed0(ref_at(self, 0x04), nullptr);                                      // 0064AFED CResRef()
    ::new (at(self, 0x14)) StartupString();  unwind.state = 0;                                   // 0064AFF7 / 0064B001
    ::new (at(self, 0x1C)) StartupString();  unwind.state = 1;                                   // 0064B005 / 0064B00D
    ::new (at(self, 0x28)) StartupString();  unwind.state = 2;                                   // 0064B012 / 0064B01A
    kotor_m15b_portrait_construct_00678ec0(at(self, 0x70), nullptr); unwind.state = 3;          // 0064B01F / 0064B02A
    ::new (at(self, 0xA0)) StartupString();                                                      // 0064B02F
    st<Word>(self, 0xB4, 0); st<Word>(self, 0xB8, 0); st<Word>(self, 0xB0, 0);                  // 0064B034..0064B040 feats list
    st<Word>(self, 0xC0, 0); st<Word>(self, 0xC4, 0); st<Word>(self, 0xBC, 0);                  // 0064B046..0064B052
    st<Word>(self, 0xCC, 0); st<Word>(self, 0xD0, 0); st<Word>(self, 0xC8, 0);                  // 0064B058..0064B064
    unwind.state = 7;                                                                            // 0064B07F
    crt_eh_vector_constructor_006fb55f(at(self, 0xD4), 0x20, 2,                                  // 0064B06A..0064B084
                                       &kotor_m15b_class_info_construct_0064a050, &kotor_m15b_class_info_destruct_006495f0);
    unwind.state = 8;                                                                            // 0064B08F
    ::new (at(self, 0x114)) StartupString(); unwind.state = 9;                                  // 0064B094 / 0064B0A0
    GuiAssign005e5140(str_at(self, 0x14), nullptr, c2da_empty_0073d71c());                     // 0064B099..0064B0A5 first name = ""
    GuiAssign005e5140(str_at(self, 0x1C), nullptr, c2da_empty_0073d71c());                     // 0064B0AA..0064B0B2 last name = ""
    st<std::uint16_t>(self, 0x24, kWord_0074bf14);                                               // 0064B0B7..0064B0BE race
    st<std::uint8_t>(self, 0x33, 0);                                                             // 0064B0CA
    st<Word>(self, 0xA8, 0);                                                                     // 0064B0CD
    GuiAssign005e5140(str_at(self, 0x28), nullptr, c2da_empty_0073d71c());                     // 0064B0C2..0064B0D3
    st<std::uint8_t>(self, 0x30, 0);                                                             // 0064B0E4
    {
        Ref discarded;                                                                           // [esp+18h] hidden return of 00406290
        unresolved_ref_assign_00406290(ref_at(self, 0x04), nullptr, &discarded, c2da_empty_0073d71c()); // 0064B0D8..0064B0E7
    }
    st<std::uint8_t>(self, 0x31, 0);                                                             // 0064B0EC
    st<std::uint8_t>(self, 0x32, 3);                                                             // 0064B0EF
    st<std::uint8_t>(self, 0xF0, kByte_0074b319);                                                // 0064B0F3..0064B0F8 classes[0].class
    st<std::uint8_t>(self, 0xF1, 1);                                                             // 0064B0FE classes[0].level
    st<std::uint8_t>(self, 0x110, 0xFF);                                                         // 0064B105 classes[1].class
    st<std::uint8_t>(self, 0x111, 0);                                                            // 0064B10C classes[1].level
    const std::uint16_t race = race_of(self);                                                    // 0064B112
    st<std::uint8_t>(self, 0x32, 1);                                                             // 0064B116
    st<std::uint8_t>(self, 0x34, 8); st<std::uint8_t>(self, 0x35, 8); st<std::uint8_t>(self, 0x37, 8); // 0064B11A..0064B122
    st<std::uint8_t>(self, 0x38, 8); st<std::uint8_t>(self, 0x39, 8); st<std::uint8_t>(self, 0x36, 8); // 0064B126..0064B12E
    st<std::uint8_t>(self, 0x3A, 0); st<std::uint8_t>(self, 0x3B, 0); st<std::uint8_t>(self, 0x3D, 0); // 0064B132..0064B138
    st<std::uint8_t>(self, 0x3E, 0); st<std::uint8_t>(self, 0x3F, 0); st<std::uint8_t>(self, 0x3C, 0); // 0064B13B..0064B141
    for (unsigned o = 0x40; o <= 0x46; ++o) st<std::uint8_t>(self, o, 0);                        // 0064B144..0064B156
    st<std::uint16_t>(self, 0x50, 0); st<std::uint16_t>(self, 0x48, 0); st<std::uint16_t>(self, 0x4A, 0); // 0064B159..0064B161
    st<std::uint16_t>(self, 0x4E, 0); st<std::uint16_t>(self, 0x4C, 0);                         // 0064B165..0064B169
    st<std::uint8_t>(self, 0x52, 0);                                                             // 0064B16D
    st<float>(self, 0x54, kFloat54Init);                                                         // 0064B170
    st<Word>(self, 0x58, 0);                                                                     // 0064B177
    st<std::uint8_t>(self, 0x5C, 0); st<std::uint8_t>(self, 0x5D, 0); st<std::uint8_t>(self, 0x5E, 0); // 0064B17A..0064B180
    st<std::uint16_t>(self, 0x80, 0x32);                                                         // 0064B183 alignment 50
    st<Word>(self, 0x60, 0);                                                                     // 0064B18C
    st<std::uint8_t>(self, 0x64, 0);                                                             // 0064B18F
    st<std::uint16_t>(self, 0x8C, race);                                                         // 0064B192 appearance = race word
    st<void*>(self, 0x84, live_operator_new(skill_count(rules())));                             // 0064B199..0064B1AC
    st<void*>(self, 0x88, live_operator_new(skill_count(rules())));                             // 0064B1B2..0064B1CB
    std::memset(ranks_mod(self), 0, skill_count(rules()));                                       // 0064B1C5..0064B1EB rep stos
    std::memset(ranks_base(self), 0, skill_count(rules()));                                      // 0064B1ED..0064B20F rep stos
    void* const block = live_operator_new(0x48);                                                 // 0064B20D..0064B211
    void* combat = nullptr;
    unwind.block = block; unwind.state = 10;                                                     // 0064B219..0064B21F
    if (block) combat = kotor_m15_combat_information_construct_00552280(block, nullptr);       // 0064B21D..0064B228 / 0064B22F
    unwind.state = 9;
    st<void*>(self, 0x68, combat);                                                               // 0064B235
    st<Word>(self, 0x6C, 0);                                                                     // 0064B239
    st<std::uint16_t>(self, 0x11E, 0); st<std::uint16_t>(self, 0x120, 0);                       // 0064B23C..0064B243
    st<std::uint16_t>(self, 0x122, 0); st<std::uint16_t>(self, 0x124, 0);                       // 0064B24A..0064B251
    for (unsigned o = 0x8E; o <= 0x93; ++o) st<std::uint8_t>(self, o, 0);                        // 0064B258..0064B270
    st<std::uint8_t>(self, 0x9C, 0); st<std::uint8_t>(self, 0x9D, 0);                            // 0064B27C..0064B282
    st<Word>(self, 0x98, 0);                                                                     // 0064B288
    st<std::uint16_t>(self, 0x11C, 0xFFFF);                                                      // 0064B28E portrait id
    st<std::uint8_t>(self, 0x94, 0xFF);                                                          // 0064B297
    unwind.state = -1;                                                                           // 0064B2A3 (frame popped)
    return self;                                                                                 // 0064B29E
}

// ---- 00649E40 CSWCCreatureStats::~CSWCCreatureStats(), RET ----
// No throwing callee on this path (live_free, ClearFeats, ~CCombatInformation, 006FA852 over the
// ClassInfo destructor, ~CExoString): the original unwind states (handler 0072A51A, states 9..0) are not emitted.
void __fastcall kotor_m15b_creature_stats_destruct_00649e40(void* self, void*) noexcept(false) {
    st<const void*>(self, 0x00, &kotor_m15b_creature_stats_vtable_00751c60);                   // 00649E5F vptr 00751C60
    if (void* const p = ld<void*>(self, 0x84)) { live_free(p); st<Word>(self, 0x84, 0); }       // 00649E65..00649E82
    if (void* const p = ld<void*>(self, 0x88)) { live_free(p); st<Word>(self, 0x88, 0); }       // 00649E88..00649E9B
    kotor_m15b_creature_stats_clear_feats_00648320(self, nullptr);                              // 00649EA1..00649EA3
    if (void* const combat = ld<void*>(self, 0x68)) {                                            // 00649EA8..00649EAD
        kotor_m15b_combat_information_destruct_00550e00(combat, nullptr);                       // 00649EB1
        live_free(combat);                                                                       // 00649EB6..00649EB7
        st<Word>(self, 0x68, 0);                                                                 // 00649EBF
    }
    str_at(self, 0x114)->~StartupString();                                                       // 00649EC2..00649ECD
    c2da_destroy_array_006fa852(at(self, 0xD4), 0x20, 2, &class_info_destroy_element_006495f0); // 00649ED2..00649EE7
    live_free(ld<void*>(self, 0xC8)); st<Word>(self, 0xC8, 0);                                  // 00649EEC..00649EFD
    live_free(ld<void*>(self, 0xBC)); st<Word>(self, 0xBC, 0);                                  // 00649F03..00649F14
    live_free(ld<void*>(self, 0xB0)); st<Word>(self, 0xB0, 0);                                  // 00649F1A..00649F34
    str_at(self, 0xA0)->~StartupString();                                                        // 00649F2E..00649F3F
    kotor_rules::kotor_fastcall_004015a0(at(self, 0x70), nullptr);                              // 00649F44..00649F4C ~CSWPortrait
    str_at(self, 0x28)->~StartupString();                                                        // 00649F51..00649F59
    str_at(self, 0x1C)->~StartupString();                                                        // 00649F5E..00649F65
    str_at(self, 0x14)->~StartupString();                                                        // 00649F6A..00649F75
}

// ---- 0064B2B0 CSWCCreatureStats scalar deleting destructor (vtable 00751C60 slot 0), RET 4 ----
void* __fastcall kotor_m15b_creature_stats_deleting_destruct_0064b2b0(void* self, void*, std::uint32_t flags) noexcept(false) {
    kotor_m15b_creature_stats_destruct_00649e40(self, nullptr);                                  // 0064B2B3
    if (flags & 1u) live_free(self);                                                             // 0064B2B8..0064B2C0
    return self;                                                                                 // 0064B2C8
}

// ---- 006475E0 CSWCCreatureStats::ClearSkills(), RET ----
void __fastcall kotor_m15b_creature_stats_clear_skills_006475e0(void* self, void*) {
    std::memset(ranks_mod(self), 0, skill_count(rules()));                                       // 006475E0..00647604
    std::memset(ranks_base(self), 0, skill_count(rules()));                                      // 00647606..00647627
}

// ---- 00647630 ability modifier of a BYTE score (Ghidra "GetWISMod"), RET 4 ----
std::int8_t __fastcall kotor_m15b_creature_stats_ability_mod_00647630(void*, void*, std::uint8_t value) {
    return static_cast<std::int8_t>(half_floor(value) - 5);                                      // 00647630..00647656
}

// ---- 00647720 CSWCCreatureStats::GetArmorClass(unused), RET 4 ----
std::int16_t __fastcall kotor_m15b_creature_stats_get_armor_class_00647720(void* self, void*, std::uint32_t) {
    return ld<std::int16_t>(self, 0x50);                                                         // 00647720
}

// ---- 00647730 GetClass(i) / 00647750 GetClassLevel(i) / 00647770 SetClass(i, class) ----
std::uint8_t __fastcall kotor_m15b_creature_stats_get_class_00647730(void* self, void*, std::uint8_t index) {
    return class_at(self, index);                                                                // 00647730..0064774A
}
std::uint8_t __fastcall kotor_m15b_creature_stats_get_class_level_00647750(void* self, void*, std::uint8_t index) {
    return level_at(self, index);                                                                // 00647750..0064776A
}
void __fastcall kotor_m15b_creature_stats_set_class_00647770(void* self, void*, std::uint8_t index, std::uint8_t class_id) {
    if (index < 2) st<std::uint8_t>(self, 0xF0u + static_cast<unsigned>(index) * 0x20u, class_id); // 00647770..00647782
}

// ---- 006477E0 CSWCCreatureStats::GetCanUseSkill(WORD skill), RET 4 ----
std::int32_t __fastcall kotor_m15b_creature_stats_get_can_use_skill_006477e0(void* self, void*, std::uint16_t skill) {
    void* r = rules();                                                                           // 006477E0
    if (skill >= skill_count(r)) return 0;                                                       // 006477E6..006477FF
    if (skill_flags(skill_entry(r, skill)) & 2u) {                                               // 00647808..00647816
        if (class_count(self) == 0) return 0;                                                    // 00647818..00647826
        std::uint8_t i = 0;                                                                      // 0064781B
        for (;;) {
            const std::uint8_t c = class_at(self, i);                                            // 00647830..0064783F
            if (kotor_m15b_class_is_skill_useable_005be070(addr(class_addr(r, c)), nullptr, skill)) // 00647846..0064785F (unchecked)
                return 1;                                                                        // 00647872
            ++i;                                                                                 // 00647864
            if (i >= class_count(self)) return 0;                                                // 00647861..00647868 / 0064786A
            r = rules();                                                                         // 00647829
        }
    }
    return static_cast<std::int8_t>(ranks_base(self)[skill]) > 0 ? 1 : 0;                      // 0064787D..00647890 (setg)
}

// ---- 00647AE0 CSWCCreatureStats::CalculateNumAttacks(), RET ----
std::int32_t __fastcall kotor_m15b_creature_stats_calculate_num_attacks_00647ae0(void* self, void*) {
    std::int32_t total = 0;                                                                      // 00647AE9 edi
    for (std::int32_t i = 0; i < class_count(self); ++i) {                                       // 00647AEB..00647B51
        const std::uint8_t lvl = level_at(self, static_cast<std::uint8_t>(i));                  // 00647AF1..00647B0A
        const std::uint8_t c = class_at(self, static_cast<std::uint8_t>(i));                    // 00647B0E..00647B1D
        total += kotor_m15b_class_get_attack_bonus_005bccb0(addr(class_addr(rules(), c)), nullptr, lvl); // 00647B24..00647B4C
    }
    const std::int32_t attacks = (total + 4) / 5;                                                // 00647B53..00647B69 (imul 66666667h)
    return attacks != 0 ? attacks : 1;                                                           // 00647B6B..00647B6D
}

// ---- 00647CE0 CSWCCreatureStats::GetFullName() -> CExoString (hidden return), RET 4 ----
void* __fastcall kotor_m15b_creature_stats_get_full_name_00647ce0(void* self, void*, void* out_string) noexcept(false) {
    StartupString* const first = str_at(self, 0x14);                                             // 00647CFC
    StartupString* const last = str_at(self, 0x1C);                                              // 00647D13
    if (first->BorrowedLength005e5790() > 0 && last->BorrowedLength005e5790() > 0) {           // 00647D00..00647D1F
        StartupString space(kSpace_0073d5d8);                                                    // 00647D21..00647D2A [esp+18h]
        StringSlot joined;                                                                       // [esp+10h]
        GuiConcat005e5d10(first, nullptr, joined.get(), &space); joined.live = true;            // 00647D2F..00647D3F first + " "
        GuiConcat005e5d10(joined.get(), nullptr, static_cast<StartupString*>(out_string), last); // 00647D44..00647D51 + last
        return out_string;                                                                       // ~joined 00647D5F, ~space 00647D70
    }
    if (first->BorrowedLength005e5790() > 0) {                                                   // 00647D77..00647D80
        ::new (out_string) StartupString(*first);                                                // 00647D90..00647D97 005E5B00
        return out_string;
    }
    if (last->BorrowedLength005e5790() > 0) {                                                    // 00647D82..00647D8E
        ::new (out_string) StartupString(*last);                                                 // 00647D90..00647D97 005E5B00
        return out_string;
    }
    ::new (out_string) StartupString(c2da_empty_0073d71c());                                     // 00647D9E..00647DA9 005E5A90("")
    return out_string;
}

// ---- 00648150..00648280 ability modifiers, RET (AL defined; callers use AL) ----
std::int8_t __fastcall kotor_m15b_creature_stats_get_str_mod_00648150(void* self, void*) {
    return static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x34)) - 5);              // 00648151..00648173
}
std::int8_t __fastcall kotor_m15b_creature_stats_get_dex_mod_00648180(void* self, void*) {
    return static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x35)) - 5 +             // 00648184..006481AE
                                    ld<std::uint8_t>(self, 0x40));                               // 006481A9..006481B1 + [+40h]
}
std::int8_t __fastcall kotor_m15b_creature_stats_get_con_mod_006481c0(void* self, void*) {
    return static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x36)) - 5);              // 006481C1..006481E3
}
std::int8_t __fastcall kotor_m15b_creature_stats_get_int_mod_006481f0(void* self, void*) {
    return static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x37)) - 5);              // 006481F1..00648213
}
std::int8_t __fastcall kotor_m15b_creature_stats_get_wis_mod_00648220(void* self, void*) {
    return static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x38)) - 5);              // 00648221..00648243
}
std::int8_t __fastcall kotor_m15b_creature_stats_get_cha_mod_00648250(void* self, void*) {
    return static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x39)) - 5);              // 00648251..00648273
}
std::int8_t __fastcall kotor_m15b_creature_stats_get_base_int_mod_00648280(void* self, void*) {
    return static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x3D)) - 5);              // 00648281..006482A3
}

// ---- 006482B0 CSWCCreatureStats::FeatAcquired(WORD feat), RET 4 (feats list, then bonus list) ----
std::int32_t __fastcall kotor_m15b_creature_stats_feat_acquired_006482b0(void* self, void*, std::uint16_t feat) {
    std::int32_t n = ld<std::int32_t>(self, 0xB4);                                               // 006482B0
    if (n > 0) {                                                                                 // 006482B9..006482C1
        std::int32_t hits = 0;
        const std::uint16_t* p = ld<const std::uint16_t*>(self, 0xB0);                          // 006482C3
        do { if (*p == feat) ++hits; ++p; } while (--n != 0);                                    // 006482D0..006482DA
        if (hits != 0) return 1;                                                                 // 006482DC..006482DE
    }
    n = ld<std::int32_t>(self, 0xC0);                                                            // 006482E0
    std::int32_t hits = 0;                                                                       // 006482E6
    if (n <= 0) return 0;                                                                        // 006482E8..006482EA
    const std::uint16_t* p = ld<const std::uint16_t*>(self, 0xBC);                              // 006482EC
    do { if (*p == feat) ++hits; ++p; } while (--n != 0);                                        // 006482F2..006482FC
    return hits != 0 ? 1 : 0;                                                                    // 006482FE..0064830A
}

// ---- 00648320 CSWCCreatureStats::ClearFeats(), RET ----
void __fastcall kotor_m15b_creature_stats_clear_feats_00648320(void* self, void*) {
    while (ld<std::int32_t>(self, 0xB4) != 0)                                                    // 00648323..00648374
        del_index<std::uint16_t>(at(self, 0xB0), ld<std::int32_t>(self, 0xB4) - 1);              // 00648330..0064836A (last element)
    while (ld<std::int32_t>(self, 0xC0) != 0)                                                    // 00648376..006483C4
        del_index<std::uint16_t>(at(self, 0xBC), 0);                                             // 00648380..006483BA (element 0)
    while (ld<std::int32_t>(self, 0xCC) != 0) {                                                  // 006483C6..00648421
        live_free(ld<void**>(self, 0xC8)[0]);                                                    // 006483D0..006483D9 free(data[0])
        del_index<void*>(at(self, 0xC8), 0);                                                     // 006483DE..00648417
    }
}

// ---- 00648470 CSWCCreatureStats::GetClassesString(char separator) -> CExoString, RET 8 ----
// result += sep + GetNameText() + " (" + CExoString(level) + ")" per class with a non-null element;
// the separator buffer {0,0} gets the separator after the first appended class.
void* __fastcall kotor_m15b_creature_stats_get_classes_string_00648470(void* self, void*, void* out_string,
                                                                        std::uint8_t separator) noexcept(false) {
    StartupString result;                                                                        // 00648498 [esp+14h]
    char sep_text[2] = {0, 0};                                                                   // 006484AC..006484B1 [esp+8]/[esp+9]
    for (std::int32_t i = 0; i < class_count(self); ++i) {                                       // 006484B6..006484BA / 0064868B..00648696
        const std::uint8_t c = class_at(self, static_cast<std::uint8_t>(i));                    // 006484C8..006484D7
        const std::uintptr_t cls = class_addr(rules(), c);                                       // 006484DE..006484F3
        if (cls == 0) continue;                                                                  // 006484F5
        StartupString close_text(kClose_0074f788);                                               // 006484FB..00648504 [esp+3Ch]
        StringSlot level_text;                                                                   // [esp+34h]
        level_text.adopt(resman_string_from_int_005e5bc0(level_text.get(), nullptr,              // 00648509..0064852F CExoString(int)
                                                         level_at(self, static_cast<std::uint8_t>(i))));
        StartupString open_text(kOpen_00748ea4);                                                 // 00648534..00648542 [esp+2Ch]
        StartupString sep_string(sep_text);                                                      // 00648547..00648555 [esp+24h]
        StringSlot name;                                                                         // [esp+68h]
        name.adopt(kotor_m15b_class_get_name_text_005be0e0(addr(cls), nullptr, name.get()));    // 0064855A..00648566
        StringSlot t1, t2, t3, t4, t5;                                                           // [esp+60h] .. [esp+40h]
        t1.adopt(GuiConcat005e5d10(&result, nullptr, t1.get(), &sep_string));                    // 0064856D..00648583
        t2.adopt(GuiConcat005e5d10(t1.get(), nullptr, t2.get(), name.get()));                   // 00648588..00648598
        t3.adopt(GuiConcat005e5d10(t2.get(), nullptr, t3.get(), &open_text));                   // 0064859D..006485B1
        t4.adopt(GuiConcat005e5d10(t3.get(), nullptr, t4.get(), level_text.get()));             // 006485B6..006485CA
        t5.adopt(GuiConcat005e5d10(t4.get(), nullptr, t5.get(), &close_text));                  // 006485CF..006485E3
        result.Assign005e5c50(t5.get());                                                         // 006485E8..006485F2
        sep_text[0] = static_cast<char>(separator);                                              // 00648683..00648687
    }   // destruction t5,t4,t3,t2,t1,name,sep,open,level,close = 006485F7..0064867E
    ::new (out_string) StartupString(result);                                                    // 0064869D..006486A8 005E5B00
    return out_string;                                                                           // ~result 006486BE
}

// ---- 006486E0 CSWCCreatureStats::GetFeat(WORD index), RET 4 -> feat or FFFF ----
std::uint32_t __fastcall kotor_m15b_creature_stats_get_feat_006486e0(void* self, void*, std::uint16_t index) {
    if (static_cast<std::int32_t>(index) >= ld<std::int32_t>(self, 0xB4)) return 0xFFFFu;        // 006486E0..006486F1
    return ld<std::uint16_t>(ld<unsigned char*>(self, 0xB0), static_cast<std::uintptr_t>(index) * 2u); // 006486F4..006486FA
}

// ---- 00648830 CSWCCreatureStats::GetSkillModifier(WORD skill), RET 4 (jump table 0064888C) ----
std::int8_t __fastcall kotor_m15b_creature_stats_get_skill_modifier_00648830(void* self, void*, std::uint16_t skill) {
    switch (ld<Word>(addr(skill_entry(rules(), skill)), 0x18)) {                                 // 00648830..0064884D
    case 0: return kotor_m15b_creature_stats_get_str_mod_00648150(self, nullptr);                // 00648854
    case 1: return kotor_m15b_creature_stats_get_dex_mod_00648180(self, nullptr);                // 0064885C
    case 2: return kotor_m15b_creature_stats_get_con_mod_006481c0(self, nullptr);                // 00648864
    case 3: return kotor_m15b_creature_stats_get_int_mod_006481f0(self, nullptr);                // 0064886C
    case 4: return kotor_m15b_creature_stats_get_wis_mod_00648220(self, nullptr);                // 00648874
    case 5: return kotor_m15b_creature_stats_get_cha_mod_00648250(self, nullptr);                // 0064887C
    default: return 0;                                                                           // 00648884
    }
}

// ---- 00649340 CSWCCreatureStats::HasFeat(WORD feat), RET 4 ----
std::int32_t __fastcall kotor_m15b_creature_stats_has_feat_00649340(void* self, void*, std::uint16_t feat) {
    const std::int32_t n = ld<std::int32_t>(self, 0xB4);                                         // 00649340
    const std::uint16_t* const p = ld<const std::uint16_t*>(self, 0xB0);                        // 0064934D
    for (std::int32_t i = 0; i < n; ++i)                                                         // 00649346..00649363
        if (p[i] == feat) return 1;                                                              // 00649358 / 0064936B
    return 0;                                                                                    // 00649365
}

// ---- 00649380 CSWCCreatureStats::UpdateAbilityModifiedSkillRanks(), RET ----
void __fastcall kotor_m15b_creature_stats_update_ability_modified_skill_ranks_00649380(void* self, void*) {
    void* r = rules();                                                                           // 00649383
    std::uint16_t n = skill_count(r);                                                            // 00649389
    for (std::uint16_t s = 0; s < n;) {                                                          // 00649392..00649397
        bool compute = true;
        if ((ld<Word>(addr(ld<std::uintptr_t>(r, 0xB4) + static_cast<std::uintptr_t>(s) * 0x20u), 0x1C) & 2u) == 0) { // 006493A0..006493B3
            if (s >= n) compute = false;                                                         // 006493B5..006493B8
            else if (ranks_base(self)[s] == 0) {                                                 // 006493BA..006493C4
                if (s < n) ranks_mod(self)[s] = 0;                                               // 006493C6..006493D1
                compute = false;
            }
        }
        if (compute) {                                                                           // 006493D7
            const std::uint8_t cur = s < n ? ranks_base(self)[s] : 0;                            // 006493D7..006493E6
            const std::int8_t mod = kotor_m15b_creature_stats_get_skill_modifier_00648830(self, nullptr, s); // 006493E9..006493EC
            const std::uint16_t n2 = skill_count(rules());                                       // 006493F1..006493F7
            if (s < n2) ranks_mod(self)[s] = static_cast<std::uint8_t>(mod + cur);               // 006493FF..0064940C
        }
        r = rules();                                                                             // 0064940F
        n = skill_count(r);                                                                      // 00649415
        ++s;                                                                                     // 0064941D
    }
}

// ---- 00649520 CSWCCreatureStats::ClearKnownSpells(), RET ----
void __fastcall kotor_m15b_creature_stats_clear_known_spells_00649520(void* self, void*) {
    for (std::uint8_t i = 0; i < class_count(self); ++i) {                                       // 00649523..00649573
        void* const list = at(self, 0xD4u + static_cast<unsigned>(i) * 0x20u);                 // 00649530..0064953E
        for (std::int32_t k = ld<std::int32_t>(list, 4) - 1; k >= 0; --k)                        // 00649536..00649545 / 00649569..0064956A
            del_index<Word>(list, k);                                                            // 00649547..00649567
    }
}

// ---- 00649D60 CSWCCreatureStats::InitializeCombatInformation(), RET ----
void __fastcall kotor_m15b_creature_stats_initialize_combat_information_00649d60(void* self, void*) {
    unsigned char* const combat = ld<unsigned char*>(self, 0x68);                                // 00649D65
    if (!combat) return;                                                                         // 00649D68..00649D6A
    combat[0] = static_cast<std::uint8_t>(kotor_m15b_creature_stats_calculate_num_attacks_00647ae0(self, nullptr)); // 00649D70..00649D75
    combat[1] = static_cast<std::uint8_t>(half_floor(ld<std::uint8_t>(self, 0x34)) - 5);       // 00649D77..00649D9E
    combat[2] = static_cast<std::uint8_t>(half_floor(ld<std::uint8_t>(self, 0x34)) - 5);       // 00649DA1..00649DC5
    std::int32_t value;                                                                          // [esp+8] (out-parameter; not read back)
    {
        const Word column = kotor_rules_cell32_007a2124;                                         // 00649DCC
        void* const r = rules();                                                                 // 00649DDA
        const std::int32_t row = ld<std::uint16_t>(race_entry(r, race_of(self)), 0x30);         // 00649DC8..00649DE5
        void* const table = ld<void*>(ld<void*>(r, 0xB8), 0x0C);                                 // 00649DEA..00649DF1
        c2da_current_get_int_entry_00413510(table, nullptr, row, static_cast<std::int32_t>(column), &value); // 00649DF4
    }
    combat[8] = 0;                                                                               // 00649DF9
    {
        const Word column = kotor_rules_cell32_007a2124;                                         // 00649E01
        void* const r = rules();                                                                 // 00649E0F
        const std::int32_t row = ld<std::uint16_t>(race_entry(r, race_of(self)), 0x30);         // 00649DFD..00649E1B
        void* const table = ld<void*>(ld<void*>(r, 0xB8), 0x0C);                                 // 00649E20..00649E26
        c2da_current_get_int_entry_00413510(table, nullptr, row, static_cast<std::int32_t>(column), &value); // 00649E2A
    }
    combat[9] = 0;                                                                               // 00649E2F
    combat[0x0A] = 1;                                                                            // 00649E33
}

// ================================================================== CSWCLevelUpStats
// ---- 0064B2D0 CSWCLevelUpStats::CSWCLevelUpStats(), RET, EAX = this ----
void* __fastcall kotor_m15b_levelup_stats_construct_0064b2d0(void* self, void*) noexcept(false) {
    kotor_m15b_creature_stats_construct_0064afc0(self, nullptr);                                 // 0064B2EF
    int state = 0;                                                                               // 0064B2FC (FuncInfo 0077B2BC)
    try {
        st<const void*>(self, 0x00, &kotor_m15b_levelup_stats_vtable_00751c64);                 // 0064B2F6 vptr 00751C64
        st<Word>(self, 0x12C, 0); st<Word>(self, 0x130, 0); st<Word>(self, 0x128, 0);           // 0064B300..0064B30C
        const std::uintptr_t race_off = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;     // 0064B312..0064B316
        st<std::uint8_t>(self, 0xAC, 0x1E);                                                      // 0064B319
        st<std::uint16_t>(self, 0xAE, 0);                                                        // 0064B320
        st<std::uint8_t>(self, 0x3A, 8);                                                         // 0064B32C
        st<std::uint8_t>(self, 0x34, static_cast<std::uint8_t>(                                  // 0064B32F..0064B348
            ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + race_off, 0x18) + 8));
        state = 1;                                                                               // 0064B343
        kotor_m15b_creature_stats_initialize_combat_information_00649d60(self, nullptr);        // 0064B34B
        kotor_m15b_levelup_stats_set_dex_base_0064a100(self, nullptr, 8, 0);                    // 0064B350..0064B354
        const std::uintptr_t race_off2 = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;    // 0064B359..0064B35D
        st<std::uint8_t>(self, 0x3D, 8);                                                         // 0064B360
        st<std::uint8_t>(self, 0x37, static_cast<std::uint8_t>(                                  // 0064B363..0064B375
            ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + race_off2, 0x1A) + 8));
        st<std::uint8_t>(self, 0x3E, 8);                                                         // 0064B378
        st<std::uint8_t>(self, 0x38, static_cast<std::uint8_t>(                                  // 0064B37B..0064B38F
            ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + race_off2, 0x1C) + 8));
        kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(self, nullptr);                 // 0064B392
        const std::uintptr_t race_off3 = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;    // 0064B397..0064B39B
        st<std::uint8_t>(self, 0x3F, 8);                                                         // 0064B39E
        st<std::uint8_t>(self, 0x39, static_cast<std::uint8_t>(                                  // 0064B3A1..0064B3B6
            ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + race_off3, 0x1B) + 8));
        kotor_m15b_levelup_stats_set_con_base_006496d0(self, nullptr, 8);                       // 0064B3B3..0064B3B9
    } catch (...) {
        if (state >= 1) exo_array_list_destruct_0056b6f0(at(self, 0x128), nullptr);            // unwind 0072A638 (~list +128)
        kotor_m15b_creature_stats_destruct_00649e40(self, nullptr);                              // unwind 0072A630
        throw;
    }
    return self;                                                                                 // 0064B3C3
}

// ---- 0064B400 CSWCLevelUpStats::~CSWCLevelUpStats(), RET ----
void __fastcall kotor_m15b_levelup_stats_destruct_0064b400(void* self, void*) noexcept(false) {
    live_free(ld<void*>(self, 0x128));                                                           // 0064B41D..0064B42C
    st<Word>(self, 0x128, 0);                                                                    // 0064B436
    kotor_m15b_creature_stats_destruct_00649e40(self, nullptr);                                  // 0064B448
}

// ---- 0064B3E0 CSWCLevelUpStats scalar deleting destructor (vtable 00751C64 slot 0), RET 4 ----
void* __fastcall kotor_m15b_levelup_stats_deleting_destruct_0064b3e0(void* self, void*, std::uint32_t flags) noexcept(false) {
    kotor_m15b_levelup_stats_destruct_0064b400(self, nullptr);                                   // 0064B3E3
    if (flags & 1u) live_free(self);                                                             // 0064B3E8..0064B3F0
    return self;                                                                                 // 0064B3F8
}

// ---- 0060B350 CSWCLevelUpStats::GetSkillRank(WORD skill), RET 4 ----
std::uint8_t __fastcall kotor_m15b_levelup_stats_get_skill_rank_0060b350(void* self, void*, std::uint16_t skill) {
    if (skill >= skill_count(rules())) return 0;                                                 // 0060B350..0060B369
    return ranks_base(self)[skill];                                                              // 0060B36C..0060B375
}

// ---- Set*Base: base byte, total = race adjust + base (0064A0D0 / 006478A0 / 006478D0 / 00649770 / 006496D0) ----
void __fastcall kotor_m15b_levelup_stats_set_str_base_0064a0d0(void* self, void*, std::uint8_t value) {
    const std::uintptr_t off = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;              // 0064A0D0..0064A0D8
    st<std::uint8_t>(self, 0x3A, value);                                                         // 0064A0DB
    st<std::uint8_t>(self, 0x34, static_cast<std::uint8_t>(ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + off, 0x18) + value)); // 0064A0DF..0064A0F1
    kotor_m15b_creature_stats_initialize_combat_information_00649d60(self, nullptr);            // 0064A0F4
}
void __fastcall kotor_m15b_levelup_stats_set_int_base_006478a0(void* self, void*, std::uint8_t value) {
    const std::uintptr_t off = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;              // 006478A0..006478A8
    st<std::uint8_t>(self, 0x3D, value);                                                         // 006478AC
    st<std::uint8_t>(self, 0x37, static_cast<std::uint8_t>(ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + off, 0x1A) + value)); // 006478AF..006478C1
}
void __fastcall kotor_m15b_levelup_stats_set_cha_base_006478d0(void* self, void*, std::uint8_t value) {
    const std::uintptr_t off = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;              // 006478D0..006478D8
    st<std::uint8_t>(self, 0x3F, value);                                                         // 006478DC
    st<std::uint8_t>(self, 0x39, static_cast<std::uint8_t>(ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + off, 0x1B) + value)); // 006478DF..006478F1
}
void __fastcall kotor_m15b_levelup_stats_set_wis_base_00649770(void* self, void*, std::uint8_t value) {
    const std::uintptr_t off = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;              // 00649770..00649778
    st<std::uint8_t>(self, 0x3E, value);                                                         // 0064977B
    st<std::uint8_t>(self, 0x38, static_cast<std::uint8_t>(ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + off, 0x1C) + value)); // 0064977F..00649791
    kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(self, nullptr);                     // 00649794
}
void __fastcall kotor_m15b_levelup_stats_set_con_base_006496d0(void* self, void*, std::uint8_t value) {
    const std::uintptr_t off = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;              // 006496D7..006496DB
    st<std::uint8_t>(self, 0x3C, value);                                                         // 006496DE
    st<std::uint8_t>(self, 0x36, static_cast<std::uint8_t>(ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + off, 0x1D) + value)); // 006496E1..006496F3
    if (static_cast<std::uint8_t>(kotor_m15b_levelup_stats_get_level_00648430(self, nullptr)) == 1 && // 006496F6..006496FF (AL == 1)
        ld<Word>(self, 0x58) == 0) {                                                             // 00649701..00649706
        const std::uint8_t c = class_at(self, static_cast<std::uint8_t>(class_count(self) - 1)); // 00649708..0064971B
        if (class_addr(rules(), c) != 0) {                                                       // 00649722..00649739
            const std::uint16_t base_hp = ld<std::uint16_t>(self, 0x48);                         // 0064973C
            std::int16_t hp = static_cast<std::int16_t>(                                         // 00649742..0064974B movsx ax,al; add
                static_cast<std::int16_t>(kotor_m15b_creature_stats_get_con_mod_006481c0(self, nullptr)) + base_hp);
            if (hp < 1) hp = 1;                                                                  // 0064974D..00649754 (signed)
            st<std::int16_t>(self, 0x4E, hp);                                                    // 00649759
            st<std::int16_t>(self, 0x4C, hp);                                                    // 0064975D
        }
    }
    kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(self, nullptr);                     // 00649761..00649763
}

// ---- 0064A100 CSWCLevelUpStats::SetDEXBase(BYTE value, int keep_ac_delta), RET 8 ----
void __fastcall kotor_m15b_levelup_stats_set_dex_base_0064a100(void* self, void*, std::uint8_t value, std::int32_t keep_ac_delta) {
    const std::int8_t old_mod = static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x35)) + // 0064A104..0064A13C
                                                         ld<std::uint8_t>(self, 0x40) - 5);
    const std::int32_t ac_rest = static_cast<std::int32_t>(ld<std::int16_t>(self, 0x50)) - old_mod; // 0064A131 / 0064A143 edi
    const std::uintptr_t off = static_cast<std::uintptr_t>(race_of(self)) * 0x34u;              // 0064A12D / 0064A135
    st<std::uint8_t>(self, 0x3B, value);                                                         // 0064A13F..0064A145
    st<std::uint8_t>(self, 0x35, static_cast<std::uint8_t>(ld<std::uint8_t>(ld<unsigned char*>(rules(), 0xB0) + off, 0x19) + value)); // 0064A148..0064A15C
    kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(self, nullptr);                   // 0064A15F
    const std::int8_t new_mod = static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x35)) + // 0064A16F..0064A194 / 0064A1AD..0064A1D2
                                                         ld<std::uint8_t>(self, 0x40) - 5);
    if (keep_ac_delta != 0)                                                                      // 0064A164..0064A16D
        st<std::int16_t>(self, 0x50, static_cast<std::int16_t>(new_mod + ac_rest));             // 0064A196..0064A19C
    else
        st<std::int16_t>(self, 0x50, static_cast<std::int16_t>(new_mod + 0x0A));                // 0064A1D4..0064A1DD
    kotor_m15b_creature_stats_initialize_combat_information_00649d60(self, nullptr);            // 0064A1A2 / 0064A1E1
}

// ---- 00647900 CSWCLevelUpStats::GetSkillPointCostForSkillRank(WORD skill, int increasing), RET 8 ----
std::int32_t __fastcall kotor_m15b_levelup_stats_get_skill_point_cost_00647900(void* self, void*, std::uint16_t skill,
                                                                                std::int32_t increasing) {
    std::int32_t result = 0;                                                                     // [esp+0Ch] 0064792A
    std::int32_t useable = 0;                                                                    // [esp+10h] 00647922
    std::int32_t class_skill = 0;                                                                // [esp+14h] 00647926
    const std::uintptr_t entry = skill_entry(rules(), skill);                                    // 0064790A..00647920
    if (entry == 0) return 0;                                                                    // 0064792E..00647938
    if (skill_flags(entry) & 8u) useable = 1;                                                    // 0064793B..00647941
    bool skip_check = false;
    if (class_count(self) != 0) {                                                                // 00647949..0064794F
        for (std::int32_t i = 0; i < class_count(self); ++i) {                                   // 00647951 / 0064799D..006479A4
            const std::uint8_t c = class_at(self, static_cast<std::uint8_t>(i));                // 00647951..00647960
            const std::uintptr_t cls = class_addr(rules(), c);                                   // 00647967..00647975
            if (cls == 0) continue;                                                              // 0064797B
            if (!kotor_m15b_class_is_skill_useable_005be070(addr(cls), nullptr, skill)) continue; // 0064797D..00647987
            useable = 1;                                                                         // 0064798C
            if (kotor_m15b_class_is_skill_class_skill_005be010(addr(cls), nullptr, skill)) {    // 00647989..0064799B
                class_skill = 1;                                                                 // 006479DC
                skip_check = true;                                                               // 006479E4 -> 006479AE
                break;
            }
        }
    }
    if (!skip_check && useable == 0) return result;                                              // 006479A6..006479AC -> 006479CE
    const std::int32_t cost = (class_skill == 0 ? 1 : 0) + 1;                                    // 006479AE..006479BD
    result = cost;                                                                               // 006479C0
    if (increasing == 0) result = -cost;                                                         // 006479BE..006479CA
    return result;                                                                               // 006479CE
}

// ---- 006479F0 CSWCLevelUpStats::GetBaseAttackBonus(), RET -> movzx of the byte sum ----
std::uint32_t __fastcall kotor_m15b_levelup_stats_get_base_attack_bonus_006479f0(void* self, void*) {
    std::uint8_t total = 0;                                                                      // 006479FA..00647A00 [esp+0Bh]
    for (std::uint8_t i = 0; i < class_count(self); ++i) {                                       // 006479FC..00647A04 / 00647A60..00647A6B
        const std::uint8_t lvl = level_at(self, i);                                              // 00647A06..00647A1F
        const std::uint8_t c = class_at(self, i);                                                // 00647A23..00647A32
        total = static_cast<std::uint8_t>(total +                                                // 00647A39..00647A5E (unchecked element)
            kotor_m15b_class_get_attack_bonus_005bccb0(addr(class_addr(rules(), c)), nullptr, lvl));
    }
    return total;                                                                                // 00647A6E / 00647A77 movzx
}

// ---- 00648430 CSWCLevelUpStats::GetLevel(), RET -> sum of class levels (int) ----
std::int32_t __fastcall kotor_m15b_levelup_stats_get_level_00648430(void* self, void*) {
    const std::int32_t n = class_count(self);                                                    // 00648431
    std::int32_t total = 0;                                                                      // 00648435
    for (std::int32_t i = 0; i < n; ++i) total += level_at(self, static_cast<std::uint8_t>(i)); // 00648437..0064845E
    return total;
}

// ---- 006488E0 CSWCLevelUpStats::SetSkillRankBase(WORD skill, BYTE rank), RET 8 (jump table 006489E0) ----
void __fastcall kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(void* self, void*, std::uint16_t skill, std::uint8_t rank) {
    if (skill >= skill_count(rules())) return;                                                   // 006488E0..006488F9
    ranks_base(self)[skill] = rank;                                                              // 006488FF..0064890D
    void* r = rules();                                                                           // 00648910
    const std::uintptr_t entry = skill_entry(r, skill);                                          // 00648916..00648921
    if ((skill_flags(entry) & 2u) == 0) {                                                        // 00648923..00648927
        const std::uint16_t n = skill_count(r);                                                  // 00648929
        if (skill >= n) return;                                                                  // 00648931..00648934
        if (ranks_base(self)[skill] == 0) {                                                      // 0064893A..00648944
            if (skill >= n) return;                                                              // 00648946..00648949
            ranks_mod(self)[skill] = 0;                                                          // 0064894F..00648955
            return;
        }
    }
    std::int8_t mod;
    switch (ld<Word>(addr(entry), 0x18)) {                                                       // 0064895F..00648967
    case 0: mod = kotor_m15b_creature_stats_get_str_mod_00648150(self, nullptr); break;          // 0064896E
    case 1: mod = kotor_m15b_creature_stats_get_dex_mod_00648180(self, nullptr); break;          // 00648977
    case 2: mod = kotor_m15b_creature_stats_get_con_mod_006481c0(self, nullptr); break;          // 00648980
    case 3: mod = kotor_m15b_creature_stats_get_int_mod_006481f0(self, nullptr); break;          // 00648989
    case 4: mod = kotor_m15b_creature_stats_get_wis_mod_00648220(self, nullptr); break;          // 00648992
    case 5: mod = kotor_m15b_creature_stats_get_cha_mod_00648250(self, nullptr); break;          // 0064899B
    default: mod = static_cast<std::int8_t>(rank); break;                                        // 006489A4 (AL = the rank argument)
    }
    const std::uint16_t n = skill_count(rules());                                                // 006489A8..006489AE
    const std::uint8_t cur = skill < n ? ranks_base(self)[skill] : 0;                            // 006489B6..006489C5
    if (skill < n) ranks_mod(self)[skill] = static_cast<std::uint8_t>(cur + mod);                // 006489C8..006489D5
}

// ---- 00648A00 CSWCLevelUpStats::AddSkillPointsPerLevel(), RET ----
void __fastcall kotor_m15b_levelup_stats_add_skill_points_per_level_00648a00(void* self, void*) {
    const std::uint8_t c = class_at(self, static_cast<std::uint8_t>(class_count(self) - 1));     // 00648A04..00648A18
    const std::uintptr_t cls = class_addr(rules(), c);                                           // 00648A1F..00648A34
    if (cls == 0) return;                                                                        // 00648A36
    const std::int32_t level = kotor_m15b_levelup_stats_get_level_00648430(self, nullptr);      // 00648A3D..00648A3F
    const std::uint8_t base = ld<std::uint8_t>(addr(cls), 0x56);                                 // 00648A44
    const std::int8_t int_mod = static_cast<std::int8_t>(half_floor(ld<std::uint8_t>(self, 0x3D)) - 5); // 00648A4E..00648A70 / 00648AB8..00648ADA
    if (static_cast<std::uint8_t>(level) == 1) {                                                 // 00648A4A..00648A4C
        if (static_cast<std::int32_t>(int_mod) + (base >> 1) > 1) {                              // 00648A72..00648A7F
            const std::uint8_t base2 = ld<std::uint8_t>(addr(cls), 0x56);                        // 00648A81
            const std::int8_t m = kotor_m15b_creature_stats_get_base_int_mod_00648280(self, nullptr); // 00648A84..00648A86
            st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>((static_cast<std::int32_t>(m) + (base2 >> 1)) << 2)); // 00648A8B..00648A9A
        } else {
            st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>(1 << 2));                   // 00648AA4..00648AAE
        }
        return;
    }
    if ((static_cast<std::int32_t>(int_mod) + base) / 2 > 1) {                                   // 00648ADC..00648AEC (cdq; sub; sar)
        const std::uint8_t base2 = ld<std::uint8_t>(addr(cls), 0x56);                            // 00648AEE
        const std::int8_t m = kotor_m15b_creature_stats_get_base_int_mod_00648280(self, nullptr); // 00648AF1..00648AF3
        st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>(skill_points(self) +            // 00648AF8..00648B06
                                                                 (static_cast<std::int32_t>(m) + base2) / 2));
    } else {
        st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>(skill_points(self) + 1));       // 00648B11..00648B16
    }
}

// ---- 00648B30 / 00648BE0 / 00648C90 saving throws: class bonus byte-sum + ability modifier ----
void __fastcall kotor_m15b_levelup_stats_set_fort_saving_throw_00648b30(void* self, void*) {
    std::uint8_t total = 0;                                                                      // 00648B3E [esp+0Bh]
    for (std::uint8_t i = 0; i < class_count(self); ++i) {                                       // 00648B3A..00648BA6
        const std::uintptr_t cls = class_addr(rules(), class_at(self, i));                       // 00648B45..00648B70
        if (cls == 0) continue;                                                                  // 00648B72
        total = static_cast<std::uint8_t>(total + kotor_m15b_class_get_fort_save_bonus_005bccd0(addr(cls), nullptr, level_at(self, i))); // 00648B74..00648B9B
    }
    st<std::uint8_t>(self, 0x5C, static_cast<std::uint8_t>(half_floor(ld<std::uint8_t>(self, 0x36)) + total - 5)); // 00648BA8..00648BD3 (CON)
}
void __fastcall kotor_m15b_levelup_stats_set_will_saving_throw_00648be0(void* self, void*) {
    std::uint8_t total = 0;                                                                      // 00648BEE
    for (std::uint8_t i = 0; i < class_count(self); ++i) {                                       // 00648BEA..00648C56
        const std::uintptr_t cls = class_addr(rules(), class_at(self, i));                       // 00648BF5..00648C20
        if (cls == 0) continue;                                                                  // 00648C22
        total = static_cast<std::uint8_t>(total + kotor_m15b_class_get_will_save_bonus_005bcd10(addr(cls), nullptr, level_at(self, i))); // 00648C24..00648C4B
    }
    st<std::uint8_t>(self, 0x5D, static_cast<std::uint8_t>(half_floor(ld<std::uint8_t>(self, 0x38)) + total - 5)); // 00648C58..00648C83 (WIS)
}
void __fastcall kotor_m15b_levelup_stats_set_reflex_saving_throw_00648c90(void* self, void*) {
    std::uint8_t total = 0;                                                                      // 00648C9E
    for (std::uint8_t i = 0; i < class_count(self); ++i) {                                       // 00648C9A..00648D06
        const std::uintptr_t cls = class_addr(rules(), class_at(self, i));                       // 00648CA5..00648CD0
        if (cls == 0) continue;                                                                  // 00648CD2
        total = static_cast<std::uint8_t>(total + kotor_m15b_class_get_ref_save_bonus_005bccf0(addr(cls), nullptr, level_at(self, i))); // 00648CD4..00648CFB
    }
    st<std::uint8_t>(self, 0x5E, static_cast<std::uint8_t>(half_floor(ld<std::uint8_t>(self, 0x35)) + // 00648D08..00648D3A (DEX + [+40h])
                                                           ld<std::uint8_t>(self, 0x40) + total - 5));
}

// ---- 00648D50 CSWCLevelUpStats::IncreaseSkillRank(WORD skill), RET 4 ----
std::int32_t __fastcall kotor_m15b_levelup_stats_increase_skill_rank_00648d50(void* self, void*, std::uint16_t skill) {
    std::int32_t class_skill = 0;                                                                // [esp+10h] 00648D73
    std::int32_t useable = 0;                                                                    // ebp 00648D6F
    const std::uintptr_t entry = skill_entry(rules(), skill);                                    // 00648D59..00648D71
    if (entry == 0) return 0;                                                                    // 00648D77
    if (skill_flags(entry) & 8u) useable = 1;                                                    // 00648D7D..00648D83
    bool granted = false;
    if (class_count(self) != 0) {                                                                // 00648D88..00648D8D
        for (std::int32_t i = 0; i < class_count(self); ++i) {                                   // 00648D90 / 00648DE1..00648DE8
            const std::uintptr_t cls = class_addr(rules(), class_at(self, static_cast<std::uint8_t>(i))); // 00648D90..00648DB4
            if (cls == 0) continue;                                                              // 00648DBA
            if (!kotor_m15b_class_is_skill_useable_005be070(addr(cls), nullptr, skill)) continue; // 00648DBC..00648DCA
            useable = 1;                                                                         // 00648DD3
            if (kotor_m15b_class_is_skill_class_skill_005be010(addr(cls), nullptr, skill)) {    // 00648DCC..00648DDF
                class_skill = 1;                                                                 // 00648E06..00648E12
                if (skill_points(self) < 1) return 0;                                            // 00648E0B / 00648E16
                granted = true;
                break;
            }
        }
    }
    if (!granted) {
        if (useable == 0) return 0;                                                              // 00648DEE..00648DF0
        if (skill_points(self) < 2) return 0;                                                    // 00648DF2..00648DFA
    }
    std::uint8_t max_rank = static_cast<std::uint8_t>(kotor_m15b_levelup_stats_get_level_00648430(self, nullptr) + 3); // 00648E1C..00648E27
    if (class_skill == 0) max_rank = static_cast<std::uint8_t>(max_rank >> 1);                   // 00648E29..00648E2D
    const std::uint16_t n = skill_count(rules());                                                // 00648E2F..00648E35
    const std::int8_t cur = skill < n ? static_cast<std::int8_t>(ranks_base(self)[skill]) : 0;   // 00648E3D..00648E52
    if (static_cast<std::int32_t>(cur) >= static_cast<std::int32_t>(max_rank)) return 0;         // 00648E52..00648E5A
    const std::int8_t cur2 = skill < n ? static_cast<std::int8_t>(ranks_base(self)[skill]) : 0;  // 00648E5C..00648E6E
    kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(self, nullptr, skill,                  // 00648E72..00648E78
                                                          static_cast<std::uint8_t>(cur2 + 1));
    if (class_skill != 0)
        st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>(skill_points(self) - 1));       // 00648E81
    else
        st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>(skill_points(self) + 0xFFFEu)); // 00648E95
    return 1;                                                                                    // 00648E8B / 00648EA1
}

// ---- 00648EB0 CSWCLevelUpStats::DecreaseSkillRank(WORD skill, BYTE floor_rank), RET 8 ----
void __fastcall kotor_m15b_levelup_stats_decrease_skill_rank_00648eb0(void* self, void*, std::uint16_t skill, std::uint8_t floor_rank) {
    std::int32_t useable = 0;                                                                    // [esp+0Ch] 00648ED2
    std::int32_t class_skill = 0;                                                                // [esp+10h] 00648ED6
    const std::uintptr_t entry = skill_entry(rules(), skill);                                    // 00648EBA..00648ED0
    if (entry == 0) return;                                                                      // 00648EDA
    if (skill_flags(entry) & 8u) useable = 1;                                                    // 00648EE0..00648EE6
    bool found = false;
    if (class_count(self) != 0) {                                                                // 00648EEE..00648EF4
        for (std::int32_t i = 0; i < class_count(self); ++i) {                                   // 00648EF6 / 00648F42..00648F49
            const std::uintptr_t cls = class_addr(rules(), class_at(self, static_cast<std::uint8_t>(i))); // 00648EF6..00648F1A
            if (cls == 0) continue;                                                              // 00648F20
            if (!kotor_m15b_class_is_skill_useable_005be070(addr(cls), nullptr, skill)) continue; // 00648F22..00648F2C
            useable = 1;                                                                         // 00648F31
            if (kotor_m15b_class_is_skill_class_skill_005be010(addr(cls), nullptr, skill)) {    // 00648F2E..00648F40
                class_skill = 1;                                                                 // 00648F6A
                found = true;                                                                    // -> 00648F53
                break;
            }
        }
    }
    if (!found && useable == 0) return;                                                          // 00648F4B..00648F51
    const std::uint16_t n = skill_count(rules());                                                // 00648F53..00648F59
    const std::int8_t cur = skill < n ? static_cast<std::int8_t>(ranks_base(self)[skill]) : 0;   // 00648F61..00648F7D
    if (static_cast<std::int32_t>(cur) == static_cast<std::int32_t>(floor_rank)) return;         // 00648F80..00648F8A
    const std::int8_t cur2 = skill < n ? static_cast<std::int8_t>(ranks_base(self)[skill]) : 0;  // 00648F8C..00648F9E
    kotor_m15b_levelup_stats_set_skill_rank_base_006488e0(self, nullptr, skill,                  // 00648FA2..00648FA8
                                                          static_cast<std::uint8_t>(cur2 - 1));
    if (class_skill != 0)
        st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>(skill_points(self) + 1));       // 00648FB5
    else
        st<std::uint16_t>(self, 0xAE, static_cast<std::uint16_t>(skill_points(self) + 2));       // 00648FC6
}

// ---- 00648FE0 CSWCLevelUpStats::CalcNumberFeats(BYTE* out_a, BYTE* out_b), RET 8 ----
void __fastcall kotor_m15b_levelup_stats_calc_number_feats_00648fe0(void* self, void*, std::uint8_t* out_a, std::uint8_t* out_b) {
    const std::uint8_t last = static_cast<std::uint8_t>(class_count(self) - 1);                 // 00648FE0..00648FE3
    const std::uintptr_t cls = class_addr(rules(), class_at(self, last));                        // 00648FE5..0064900C (unchecked)
    const std::int32_t lvl = level_at(self, last);                                               // 00649012..00649028
    const std::int32_t index = lvl - 1;                                                          // 0064902B
    *out_a = index < 0x14 ? ld<std::uint8_t>(addr(cls), static_cast<std::uintptr_t>(lvl) + 0x14Fu) : 0; // 0064902E..00649043
    *out_b = index < 0x14 ? ld<std::uint8_t>(addr(cls), static_cast<std::uintptr_t>(lvl) + 0x13Bu) : 0; // 00649046..00649063
}

// ---- 00649110 CSWCLevelUpStats::SetNormalBonusFlags(WORD feat, int* normal, int* bonus), RET 0Ch ----
void __fastcall kotor_m15b_levelup_stats_set_normal_bonus_flags_00649110(void* self, void*, std::uint16_t feat,
                                                                          std::int32_t* normal, std::int32_t* bonus) {
    *normal = 0;                                                                                 // 00649120
    *bonus = 0;                                                                                  // 00649128
    if (!kotor_rules::kotor_fastcall_00550c00(rules(), nullptr, feat)) return;                  // 0064912F..0064913D
    const std::uintptr_t cls = class_addr(rules(), class_at(self, static_cast<std::uint8_t>(class_count(self) - 1))); // 0064913F..0064916E
    if (cls == 0) return;                                                                        // 00649170
    if (kotor_m15b_class_is_normal_feat_005be2c0(addr(cls), nullptr, feat)) *normal = 1;        // 00649172..0064917E
    if (kotor_m15b_class_is_bonus_feat_005be270(addr(cls), nullptr, feat)) *bonus = 1;          // 00649184..00649190
}

// ---- 006491A0 CSWCLevelUpStats::AddDefaultPackageSkills(BYTE class_id, int ignore_flags), RET 8 ----
void __fastcall kotor_m15b_levelup_stats_add_default_package_skills_006491a0(void* self, void*, std::uint8_t class_id,
                                                                              std::int32_t ignore_flags) {
    void* const r = rules();                                                                     // 006491A1
    void* const cls = addr(class_addr(r, class_id));                                             // 006491A9..006491C3 (unchecked)
    const std::int32_t skills = skill_count(r);                                                  // 006491BC / 006491D2 [esp+0Ch]
    std::int32_t points = skill_points(self);                                                    // 006491C5..006491CE [esp+14h]
    std::int32_t rank = 1;                                                                       // 006491D6
    if (points <= 0) return;                                                                     // 006491CC..006491DB
    do {
        if (rank > skills) return;                                                               // 006491E0..006491E4
        const std::int32_t skill = kotor_m15b_class_get_recommended_skill_005be430(cls, nullptr, rank); // 006491E6..006491EE
        if (skill != -1) {                                                                       // 006491F0..006491F3
            bool add = true;
            if (ignore_flags == 0) {                                                             // 006491F5..006491FB
                const Word flags = skill_flags(skill_entry(rules(), static_cast<std::uint16_t>(skill))); // 006491FD..00649211
                if ((flags & 0x10u) == 0) add = false;                                           // 00649214..00649216
                else if (race_of(self) == kWord_0074bf10 && (flags & 0x20u) == 0) add = false;   // 00649218..00649227
            }
            if (add) {
                while (kotor_m15b_levelup_stats_increase_skill_rank_00648d50(self, nullptr, static_cast<std::uint16_t>(skill))) {} // 00649230..0064923A
                points = skill_points(self);                                                     // 0064923C..00649243
            }
        }
        ++rank;                                                                                  // 0064924B
    } while (points > 0);                                                                        // 00649247..0064924E
}

// ---- 00649260 CSWCLevelUpStats::FeatAcquired(WORD feat, CExoArrayList<WORD>* pending), RET 8 ----
std::int32_t __fastcall kotor_m15b_levelup_stats_feat_acquired_00649260(void* self, void*, std::uint16_t feat, void* pending) {
    if (feat == 0xFFFF) return 0;                                                                // 00649265..0064926F
    if (kotor_m15b_creature_stats_feat_acquired_006482b0(self, nullptr, feat)) return 1;        // 00649272..00649282
    if (!pending) return 0;                                                                      // 00649285..0064928C
    const std::int32_t n = ld<std::int32_t>(pending, 4);                                         // 0064928E
    const std::uint16_t* const data = ld<const std::uint16_t*>(pending, 0);                     // 00649297
    for (std::uint16_t i = 0; static_cast<std::int32_t>(i) < n; ++i)                            // 00649291..006492AC (16-bit counter)
        if (data[i] == feat) return 1;                                                           // 006492A0 / 006492B5
    return 0;                                                                                    // 006492AE
}

// ---- 006498A0 CSWCLevelUpStats::HasFeatPrereq(CSWFeat* feat, CExoArrayList<WORD>* pending), RET 8 ----
std::int32_t __fastcall kotor_m15b_levelup_stats_has_feat_prereq_006498a0(void* self, void*, void* feat, void* pending) {
    const std::uint16_t pre1 = ld<std::uint16_t>(feat, 0x38);                                    // 006498A8
    if (pre1 != 0xFFFF && !kotor_m15b_levelup_stats_feat_acquired_00649260(self, nullptr, pre1, pending)) return 0; // 006498AC..006498C3
    const std::uint16_t pre2 = ld<std::uint16_t>(feat, 0x3A);                                    // 006498C7
    if (pre2 != 0xFFFF && !kotor_m15b_levelup_stats_feat_acquired_00649260(self, nullptr, pre2, pending)) return 0; // 006498CB..006498E0
    std::int32_t any_listed = 0;                                                                 // edi 006498EB
    std::int32_t any_met = 0;                                                                    // ebp 006498ED
    for (std::int32_t i = 0; i < 5; ) {                                                          // 006498EF..006498F4
        if (static_cast<std::uint8_t>(i) < 5) {                                                  // 006498F6..006498F9
            const std::uint16_t alt = ld<std::uint16_t>(feat, 0x3Cu + static_cast<std::uintptr_t>(static_cast<std::uint8_t>(i)) * 2u); // 006498FB..00649902
            if (alt != 0xFFFF) {                                                                 // 00649907..0064990B
                any_listed = 1;                                                                  // 00649915
                if (kotor_m15b_levelup_stats_feat_acquired_00649260(self, nullptr, alt, pending)) any_met = any_listed; // 0064990D..00649923
            }
        }
        ++i;                                                                                     // 00649925
        if (any_met != 0) break;                                                                 // 00649926..00649928
    }
    if (any_listed != 0 && any_met == 0) return 0;                                               // 0064992A..00649938
    return 1;                                                                                    // 0064993E
}

// ---- 00649950 CSWCLevelUpStats::AddGrantedFeats(CExoArrayList* added), RET 4 ----
void __fastcall kotor_m15b_levelup_stats_add_granted_feats_00649950(void* self, void*, void* added) {
    void* const race = race_entry(rules(), race_of(self));                                       // 00649954..00649973 [esp+8]
    if (ld<std::uint16_t>(rules(), 0xA4) == 0) return;                                           // 0064996C..00649977
    for (std::int32_t feat = 0; feat < static_cast<std::int32_t>(ld<std::uint16_t>(rules(), 0xA4)); ++feat) { // 00649983 / 00649A50..00649A60
        if (!kotor_rules::kotor_fastcall_00550c00(rules(), nullptr, static_cast<std::uint16_t>(feat))) continue; // 00649983..0064998B
        const std::int32_t n = class_count(self);                                                // 00649991
        std::uint32_t total = 0;                                                                 // 00649995 ebx
        for (std::int32_t i = 0; i < n; ++i) total += level_at(self, static_cast<std::uint8_t>(i)); // 00649997..006499BD
        bool grant = false;
        if (static_cast<std::uint8_t>(total) == 1 && ld<Word>(self, 0x58) == 0 &&               // 006499BF..006499C9
            kotor_m15b_race_is_first_level_granted_feat_005c7cc0(race, nullptr, static_cast<std::uint16_t>(feat))) { // 006499CB..006499D7
            grant = true;                                                                        // -> 00649A38
        } else {
            const std::uint8_t last = static_cast<std::uint8_t>(class_count(self) - 1);         // 006499D9..006499DC
            const std::uintptr_t cls = class_addr(rules(), class_at(self, last));                // 006499DE..00649A01
            if (cls == 0) continue;                                                              // 00649A07
            std::uint8_t level = 0;                                                              // the argument slot [esp+18h] (low byte written)
            if (!kotor_m15b_class_is_granted_feat_005be310(addr(cls), nullptr, static_cast<std::uint16_t>(feat), &level)) continue; // 00649A09..00649A16
            if (level != level_at(self, static_cast<std::uint8_t>(class_count(self) - 1))) continue; // 00649A18..00649A36
            grant = true;
        }
        if (grant) {
            kotor_m15_array_u16_add_unique_005a7670(at(self, 0xB0), nullptr, static_cast<std::uint32_t>(feat)); // 00649A38..00649A3F
            if (added) aurora_list_add16_00671c00(added, nullptr, static_cast<std::uint32_t>(feat)); // 00649A44..00649A4B
        }
    }
}

// ---- 00649A70 CSWCLevelUpStats::ClearPackage(), RET ----
void __fastcall kotor_m15b_levelup_stats_clear_package_00649a70(void* self, void*) {
    st<std::uint16_t>(self, 0xAE, 0);                                                            // 00649A76
    std::memset(ranks_mod(self), 0, skill_count(rules()));                                       // 00649A7F..00649AA0
    std::memset(ranks_base(self), 0, skill_count(rules()));                                      // 00649AA2..00649AC2
    kotor_m15b_creature_stats_update_ability_modified_skill_ranks_00649380(self, nullptr);     // 00649AC6
    kotor_m15b_creature_stats_clear_feats_00648320(self, nullptr);                              // 00649ACD
    kotor_m15b_creature_stats_clear_known_spells_00649520(self, nullptr);                       // 00649AD4
    kotor_m15b_levelup_stats_add_granted_feats_00649950(self, nullptr, nullptr);                // 00649AD9..00649ADB
    while (ld<std::int32_t>(self, 0x12C) != 0) {                                                 // 00649AE0..00649AE8 / 00649B35..00649B3D
        const std::int32_t index = ld<std::int32_t>(self, 0x12C) - 1;                            // 00649AF0..00649AFC
        st<std::int32_t>(self, 0x12C, ld<std::int32_t>(self, 0x12C) - 1);                        // 00649AF6..00649B02
        for (std::int32_t k = index; k < ld<std::int32_t>(self, 0x12C); ++k) {                   // 00649B00..00649B33
            Ref* const data = ld<Ref*>(self, 0x128);                                             // 00649B10
            Ref discarded;                                                                       // [esp+0Ch] hidden return
            c2da_copy_00406120(&data[k], nullptr, &discarded, &data[k + 1]);                     // 00649B16..00649B22 CResRef::operator=
        }
    }
}

// ---- 0064A2E0 CSWCLevelUpStats::FeatRequirementsMet(WORD feat, CExoArrayList<WORD>* pending), RET 8 ----
std::int32_t __fastcall kotor_m15b_levelup_stats_feat_requirements_met_0064a2e0(void* self, void*, std::uint16_t feat, void* pending) {
    unsigned char* const f = static_cast<unsigned char*>(kotor_rules::kotor_fastcall_00550c00(rules(), nullptr, feat)); // 0064A2EC..0064A2F8
    if (!f) return 0;                                                                            // 0064A2FA..0064A304
    const std::uintptr_t last_cls = class_addr(rules(), class_at(self, static_cast<std::uint8_t>(class_count(self) - 1))); // 0064A307..0064A331 (unchecked)
    const std::uint8_t granted_at = kotor_m15b_class_get_level_feat_granted_005be220(addr(last_cls), nullptr, feat); // 0064A337..0064A33D
    if (granted_at != 0xFF &&                                                                    // 0064A33F..0064A342
        granted_at > static_cast<std::uint8_t>(kotor_m15b_levelup_stats_get_level_00648430(self, nullptr))) return 0; // 0064A344..0064A34D
    if (f[0x32] != 0) {                                                                          // 0064A35B..0064A360 min spell level
        std::int32_t found = 0;                                                                  // 0064A366..0064A368 [esp+14h]
        std::uint8_t i = 0;                                                                      // 0064A36C
        for (;;) {
            if (i >= class_count(self)) {                                                        // 0064A370..0064A373
                if (found == 0) return 0;                                                        // 0064A419..0064A41B
                break;
            }
            const std::uint8_t c = class_at(self, i);                                            // 0064A379..0064A388
            void* const cls = addr(class_addr(rules(), c));                                      // 0064A38F..0064A3A4 (unchecked)
            if (ld<Word>(cls, 0x19C) != 0) {                                                     // 0064A3AA..0064A3B2
                if (kotor_m15b_class_get_spells_known_per_level_005bcd60(cls, nullptr, level_at(self, i), f[0x32], c, // 0064A3B4..0064A3F8
                                                                        race_of(self), ld<std::uint8_t>(self, 0x3F)))
                    found = 1;                                                                   // 0064A3FD..0064A401
            }
            ++i;                                                                                 // 0064A40D
            if (found != 0) break;                                                               // 0064A40F..0064A417
        }
    }
    if (f[0x2D] > static_cast<std::uint8_t>(kotor_m15b_levelup_stats_get_level_00648430(self, nullptr))) return 0; // 0064A421..0064A42B
    const std::uint8_t min_bab = f[0x2C];                                                        // 0064A431
    if (static_cast<std::int32_t>(min_bab) >                                                     // 0064A434..0064A440 (signed int compare)
        static_cast<std::int32_t>(kotor_m15b_levelup_stats_get_base_attack_bonus_006479f0(self, nullptr))) return 0;
    if (f[0x2E] > ld<std::uint8_t>(self, 0x3A)) return 0;                                        // 0064A446..0064A44C STR base
    if (f[0x2F] > ld<std::uint8_t>(self, 0x3B)) return 0;                                        // 0064A452..0064A458 DEX base
    if (f[0x30] > ld<std::uint8_t>(self, 0x3D)) return 0;                                        // 0064A45E..0064A464 INT base
    if (f[0x31] > ld<std::uint8_t>(self, 0x3E)) return 0;                                        // 0064A46A..0064A470 WIS base
    if (!kotor_m15b_levelup_stats_has_feat_prereq_006498a0(self, nullptr, f, pending)) return 0; // 0064A476..0064A485
    const std::uint16_t skill = ld<std::uint16_t>(f, 0x46);                                      // 0064A48B..0064A48D
    if (skill != 0xFFFF && !kotor_m15b_creature_stats_get_can_use_skill_006477e0(self, nullptr, skill)) return 0; // 0064A491..0064A4A1
    return 1;                                                                                    // 0064A4AA
}

// ---- 0064A4C0 CSWCLevelUpStats::CanChooseFeat(WORD feat, BYTE normal_left, BYTE bonus_left, CExoArrayList<WORD>* pending), RET 10h ----
std::int32_t __fastcall kotor_m15b_levelup_stats_can_choose_feat_0064a4c0(void* self, void*, std::uint16_t feat,
    std::uint8_t normal_left, std::uint8_t bonus_left, void* pending) noexcept(false) {
    std::int32_t normal = 0;                                                                     // [esp+14h] 0064A4EE
    std::int32_t bonus = 0;                                                                      // [esp+10h] 0064A4F2
    if (!kotor_rules::kotor_fastcall_00550c00(rules(), nullptr, feat)) return 0;                // 0064A4E1..0064A4FD
    if (normal_left == 0 && bonus_left == 0) return 0;                                           // 0064A4FF..0064A50D
    {   // already owned (inlined GetFeat over the feats list, 16-bit index)
        const std::int32_t count = ld<std::int32_t>(self, 0xB4);                                 // 0064A525
        for (std::uint16_t i = 0; i < static_cast<std::uint16_t>(count); ++i) {                  // 0064A52B..0064A531 / 0064A554..0064A558
            const std::uint16_t owned = static_cast<std::int32_t>(i) >= count ? 0xFFFF :         // 0064A533..0064A53F
                ld<std::uint16_t>(ld<unsigned char*>(self, 0xB0), static_cast<std::uintptr_t>(i) * 2u); // 0064A541..0064A547
            if (owned == feat) return 0;                                                         // 0064A54B..0064A54E
        }
    }
    {   // already pending
        const std::int32_t count = ld<std::int32_t>(pending, 4);                                 // 0064A55A..0064A55E
        if (count > 0) {                                                                         // 0064A563..0064A565
            const std::uint16_t* const data = ld<const std::uint16_t*>(pending, 0);             // 0064A567 mov esi,[edi]
            for (std::uint16_t i = 0; static_cast<std::int32_t>(i) < count; ++i)                 // 0064A561 / 0064A57F..0064A585
                if (data[i] == feat) return 0;                                                   // 0064A575..0064A579 cmp bx,[esi+eax*2]
        }
    }
    if (!kotor_m15b_levelup_stats_feat_requirements_met_0064a2e0(self, nullptr, feat, pending)) return 0; // 0064A58D..0064A598
    kotor_m15b_levelup_stats_set_normal_bonus_flags_00649110(self, nullptr, feat, &normal, &bonus); // 0064A59E..0064A5AB
    if (normal == 0 && bonus == 0) return 0;                                                     // 0064A5B0..0064A5BA
    std::int32_t used = 0;                                                                       // [esp+44h] 0064A5CA
    // Local CExoArrayList<int> {data [esp+1Ch], count [esp+20h], capacity [esp+24h]} (EH state 0).
    std::int32_t* marks = nullptr;                                                               // 0064A5DE
    const std::int32_t list_count = ld<std::int32_t>(pending, 4);                                // 0064A5E2
    if (list_count != 0)                                                                         // 0064A5E5..0064A5EB
        marks = static_cast<std::int32_t*>(live_operator_new(static_cast<std::uint32_t>(list_count) * 4u)); // 0064A5ED..0064A603
    for (std::int32_t k = 0; k < ld<std::int32_t>(pending, 4); ++k) marks[k] = 0;                // 0064A607..0064A61E
    bool ok = false;
    std::uint8_t dl = 0, bl = 0;
    for (std::uint16_t i = 0; static_cast<std::int32_t>(i) < ld<std::int32_t>(pending, 4); ++i) { // 0064A620..0064A68F
        const std::uint16_t other = ld<std::uint16_t>(ld<unsigned char*>(pending, 0), static_cast<std::uintptr_t>(i) * 2u); // 0064A630..0064A639
        kotor_m15b_levelup_stats_set_normal_bonus_flags_00649110(self, nullptr, other, &normal, &bonus); // 0064A632..0064A647
        if (normal != 0) {                                                                       // 0064A64C..0064A656
            if (bonus != 0) continue;                                                            // 0064A658..0064A65A
            if (normal_left == 0) goto done;                                                     // 0064A65C..0064A662 -> 0064A6CD
            --normal_left;                                                                       // 0064A664..0064A666
            marks[i] = 1;                                                                        // 0064A67E
        } else {
            if (bonus == 0) continue;                                                            // 0064A66C..0064A66E
            if (bonus_left == 0) continue;                                                       // 0064A670..0064A676
            --bonus_left;                                                                        // 0064A678..0064A67A
            marks[i] = 1;                                                                        // 0064A67E
        }
    }
    kotor_m15b_levelup_stats_set_normal_bonus_flags_00649110(self, nullptr, feat, &normal, &bonus); // 0064A691..0064A6A4
    if (normal != 0) {                                                                           // 0064A6A9..0064A6B3
        if (bonus != 0) { dl = normal_left; bl = bonus_left; }                                   // 0064A6B5..0064A6B7 / 0064A6FE..0064A702
        else {
            dl = normal_left;                                                                    // 0064A6B9
            if (dl == 0) goto done;                                                              // 0064A6BD..0064A6BF
            --dl; used = 1; bl = bonus_left;                                                     // 0064A6C1..0064A6C3 / 0064A702
        }
    } else {
        if (bonus != 0) {                                                                        // 0064A6E2..0064A6E4
            bl = bonus_left;                                                                     // 0064A6E6
            if (bl == 0) goto done;                                                              // 0064A6EA..0064A6EC
            dl = normal_left; --bl; used = 1;                                                    // 0064A6EE..0064A6F4
        } else { dl = normal_left; bl = bonus_left; }                                            // 0064A6FE..0064A702
    }
    {
        const std::int32_t count = ld<std::int32_t>(pending, 4);                                 // 0064A706
        for (std::uint16_t i = 0; static_cast<std::int32_t>(i) < count; ++i) {                   // 0064A709..0064A72C
            if (marks[i] != 0) continue;                                                         // 0064A711..0064A716
            if (dl != 0) { --dl; continue; }                                                     // 0064A718..0064A71C
            if (bl == 0) goto done;                                                              // 0064A720..0064A722
            --bl;                                                                                // 0064A724
        }
    }
    if (used == 0 && dl == 0 && bl == 0) goto done;                                              // 0064A72E..0064A73C
    ok = true;
done:
    live_free(marks);                                                                            // 0064A6CD..0064A6D6 / 0064A73E..0064A747
    return ok ? 1 : 0;                                                                           // 0064A6DE / 0064A74F
}

// ---- 0064A770 CSWCLevelUpStats::AddDefaultPackageFeats(BYTE class_id, CExoArrayList<WORD>* out, int allow_a, int allow_b), RET 10h ----
std::uint8_t __fastcall kotor_m15b_levelup_stats_add_default_package_feats_0064a770(void* self, void*, std::uint8_t class_id,
    void* out_list, std::int32_t allow_a, std::int32_t allow_b) noexcept(false) {
    std::uint8_t count_a = 0;                                                                    // [esp+4] 0064A781
    std::uint8_t count_b = 0;                                                                    // [esp]   0064A786
    kotor_m15b_levelup_stats_calc_number_feats_00648fe0(self, nullptr, &count_a, &count_b);     // 0064A773..0064A78B
    if (allow_a == 0) count_a = 0;                                                               // 0064A790..0064A798
    if (allow_b == 0) count_b = 0;                                                               // 0064A79D..0064A7A7
    std::uint8_t remaining = static_cast<std::uint8_t>(count_b + count_a);                       // 0064A7AD..0064A7C4
    void* const r = rules();                                                                     // 0064A7C6
    void* const cls = addr(class_addr(r, class_id));                                             // 0064A7B7..0064A7D8 (unchecked)
    const std::int32_t feat_total = ld<std::uint16_t>(r, 0xA4);                                  // 0064A7D1 / 0064A7E5
    std::int32_t rank = 1;                                                                       // 0064A7DC..0064A7E1
    if (remaining == 0) return remaining;                                                        // 0064A7DA..0064A7E9
    do {
        if (rank > feat_total) break;                                                            // 0064A7F7..0064A7FB
        const std::int32_t pick = kotor_m15b_class_get_recommended_feat_005be3c0(cls, nullptr, rank); // 0064A7FD..0064A805
        const std::uint16_t feat = static_cast<std::uint16_t>(pick);                             // 0064A807 movzx ecx,bp (the -1 test below is
        // compiled against the zero-extended word: 0064A80A cmp ecx,-1 -> never equal; mirrored as a no-op).
        if (kotor_m15b_levelup_stats_can_choose_feat_0064a4c0(self, nullptr, feat, count_a, count_b, out_list)) { // 0064A80F..0064A826
            if (ld<std::int32_t>(out_list, 4) == ld<std::int32_t>(out_list, 8)) {                // 0064A828..0064A82E
                const std::int32_t capacity = ld<std::int32_t>(out_list, 8);
                kotor_m15_array_u16_allocate_004d2fa0(out_list, nullptr, capacity != 0 ? capacity * 2 : 0x10); // 0064A830..0064A840
            }
            const std::int32_t n = ld<std::int32_t>(out_list, 4);                                // 0064A845
            st<std::uint16_t>(ld<unsigned char*>(out_list, 0), static_cast<std::uintptr_t>(n) * 2u, feat); // 0064A848..0064A84A
            st<std::int32_t>(out_list, 4, n + 1);                                                // 0064A84E
            --remaining;                                                                         // 0064A851
        }
        ++rank;                                                                                  // 0064A853..0064A85A
    } while (remaining != 0);                                                                    // 0064A858..0064A85E
    return remaining;                                                                            // 0064A863
}

}  // extern "C"
