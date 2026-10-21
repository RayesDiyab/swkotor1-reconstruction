// lane-m15-c (L15-C, Milestone 1.5): random character names.
//   CResLTR (0x5C; ctor 007121B0, dtor 007121F0, deleting 00712980, OnResourceFreed 00712200,
//            OnResourceServiced 00712410; method table 0075FC34)
//   CResHelper<CResLTR,0x07F4> (base destructor 007110B0 with table 0075FC0C, deleting 00711270, SetResRef 00711110)
//   CSWNameGen (0x24; ctor 00711290, dtor 007112E0, deleting 00712190 with table 0075FC10, UnloadNameTable 00711090,
//            LoadNameTable 00711340, GetRandomName(count) 00711390, GetRandomName(race, flags, count) 007118C0)
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only (dumpbin /disasm); every behaviour
// cites its instruction addresses. Tables read from .rdata:
//   0075FC0C = {00711270}            0075FC10 = {00712190}
//   0075FC34 = {00712980, 00406490, 0063E7F0, 00712200, 00712410}
// Reuse: the CResHelper<CResLTR,07F4h> members are the CResHelper template whose reviewed instance bodies live in
// core47/aurora_family_impl.h (lane-m47-core, TXI shape 0070FB30 / 0070FB90); 007110B0 and 00711110 were compared
// instruction by instruction with that shape (only the tag 07F4h, CRes size 5Ch, ctor 007121B0 and table differ).
// Conventions: D1 fastcall veneers, D21/D24 typed one-slot / five-slot tables, D30-FINAL (float r = rand()*scale is
// computed wide and stored to a float exactly as FILD/FMUL dword/FSTP dword at 0071141D..2B; compares are float vs
// float like FLD dword/FCOMP dword). The original CRT rand 006FC447 is the shared port crt_rand_006fc447.
#include "9c48421c6107df8e_aurora_family_impl.h"   // core47: CResHelper template bodies, tpc_lower tables, live allocator, CResRef providers
#include "b3ed8cdd89724107_rules_common.h"         // lane-rules: new_c2da / delete_c2da / string_entry / row_count, StartupString helpers
#include "79da78e626bf1ab1_string_priority.h"      // GuiAssign005e5140 / GuiConcat005e5d10
#include "1026d9ab1ae1c129_m15c_namegen.h"
#include <cctype>
#include <initializer_list>
#include <cstring>
#include <new>

using kotor_m15c_namegen::NameGen;
using String = kotor_reconstruction::StartupString;
using Word = std::uint32_t;
using tpc_lower::load;
using tpc_lower::store;

extern "C" {
std::int32_t __cdecl crt_rand_006fc447();                                                     // lane-3d-model (original CRT rand)
std::int32_t __fastcall exostring_compare_no_case_005e6450(String*, void*, String*);         // lane-resource-lowers 005E6450
}

namespace {
// ---- string temporaries constructed by a callee into raw storage (operator+ 005E5D10 by-value result)
struct Fresh {
    alignas(String) unsigned char raw[sizeof(String)];
    String* object = nullptr;
    String* storage() { return reinterpret_cast<String*>(raw); }
    ~Fresh() { if (object) object->~StartupString(); }
};
// s = base + suffix: operator+ into a temporary, operator= 005E5C50, temporary destroyed (e.g. 00711E3B..93).
void assign_concat(String* target, String* base, const char* suffix) {
    String right(suffix);                                                         // 005E5A90
    Fresh sum;
    sum.object = GuiConcat005e5d10(base, nullptr, sum.storage(), &right);         // 005E5D10
    target->Assign005e5c50(sum.object);                                           // 005E5C50
}
// CRes::Release 00409B80 through the loaded flag (00711090 / 00711B45..5E / 00711C24..35 / 00711D8E..9C).
void release_loaded(NameGen* self) {
    if (self->loaded) {
        self->loaded = 0;
        c2da_release_demand_00409b80(self->resource, nullptr);
    }
}
// Inline "CResRef(name); SetResRef(&ref, 0); Demand" sequence of 00711340 / 00711BC0..E6 / 00711C3A..66.
bool demand_table(NameGen* self, String* name) {
    menu_resources::Ref ref;
    (void)async_ref_construct_string_00406d60(&ref, nullptr, name);               // 00406D60
    kotor_m15c_namegen_set_res_ref_00711110(self, nullptr, &ref, 0);              // 00711110
    return c2da_current_resource_demand_00409b10(self->resource, nullptr) != 0;   // 00409B10
}
// letter index -> character (007114E9..00711502, 007116E3..00711704): 26 -> '\'', 27 -> '-', else 'a' + index.
char letter(std::int32_t index) {
    if (index == 0x1A) return '\'';
    if (index == 0x1B) return '-';
    return static_cast<char>(index + 0x61);
}
// character -> letter index (007116D0..00711744): '\'' -> 26, '-' -> 27, else (signed char) - 'a'.
std::int32_t index_of(char c) {
    if (c == '\'') return 0x1A;
    if (c == '-') return 0x1B;
    return static_cast<std::int32_t>(static_cast<signed char>(c)) - 0x61;
}
// FILD rand / FMUL dword [0075FC20] (0x38000100 = 1/32767f) / FSTP dword (0071141D..2B, 00711560..6A).
float random_unit() {
    const std::int32_t value = crt_rand_006fc447();                               // 006FC447
    std::uint32_t bits = 0x38000100u;                                             // .rdata 0075FC20
    float scale;
    std::memcpy(&scale, &bits, sizeof(scale));
    return static_cast<float>(static_cast<double>(value) * static_cast<double>(scale));
}
// First index i in [0, n) with r < table[i] (FCOMP / FNSTSW / TEST AH,5 / JNP), or n.
std::int32_t pick(float r, const float* table, std::int32_t n) {
    std::int32_t i = 0;
    for (; i < n; ++i)
        if (r < table[i]) break;
    return i;
}
// CResLTR field views (ctor 007121B0 zeroes +28..+58; OnResourceServiced 00712410 fills them).
unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
float* singles(void* ltr, unsigned off) { return load<float*>(ltr, off); }                 // +40/+44/+48
float** doubles(void* ltr, unsigned off) { return load<float**>(ltr, off); }               // +34/+38/+3C
float*** triples(void* ltr, unsigned off) { return load<float***>(ltr, off); }             // +28/+2C/+30
std::int32_t letter_count(void* ltr) { return load<unsigned char*>(ltr, 0x4C)[8]; }        // header byte +8
// CRes method-table slot +0C (OnResourceFreed) on this (0071244D, 00712468); EAX ignored.
void call_freed_slot(void* self) {
    const void* const methods = load<const void*>(self, 0);
    tpc_lower::ResourceCallback slot;
    std::memcpy(&slot, static_cast<const unsigned char*>(methods) + 0x0C, sizeof(slot));
    (void)slot(self, nullptr);
}
void* new_array(Word n) { return live_operator_new(n << 2); }                                // operator new(n*4) 006FA7E6
}

extern "C" {
// ======================================================================= tables (.rdata)
const tpc_lower::HelperMethods kotor_m15c_ltr_helper_methods_0075fc0c{&kotor_m15c_ltr_helper_deleting_destroy_00711270};
const tpc_lower::HelperMethods kotor_m15c_namegen_methods_0075fc10{&kotor_m15c_namegen_deleting_destroy_00712190};
const tpc_lower::ResourceMethods kotor_m15c_res_ltr_methods_0075fc34{
    &kotor_m15c_res_ltr_deleting_destroy_00712980, &c2da_resource_constant_word_00406490,
    &c2da_resource_constant_word_0063e7f0, &kotor_m15c_res_ltr_freed_00712200, &kotor_m15c_res_ltr_serviced_00712410};

// ======================================================================= CResLTR
// 007121B0: CRes::CRes 00406460; +4C/+50/+54/+58 and +28..+48 = 0; table 0075FC34; EAX = this.
void* __fastcall kotor_m15c_res_ltr_construct_007121b0(void* self, void*) noexcept(false) {
    (void)c2da_resource_base_ctor_00406460(self, nullptr);                        // 007121B3
    for (unsigned off : {0x4Cu, 0x50u, 0x54u, 0x58u, 0x28u, 0x2Cu, 0x30u, 0x34u, 0x38u, 0x3Cu, 0x40u, 0x44u, 0x48u})
        store<Word>(self, off, 0);                                                // 007121BA..DE
    store<const void*>(self, 0, &kotor_m15c_res_ltr_methods_0075fc34);           // 007121E1
    return self;                                                                  // 007121E7
}
// 007121F0: table 0075FC34; JMP CRes::~CRes 00409D40.
void __fastcall kotor_m15c_res_ltr_destroy_007121f0(void* self, void*) noexcept {
    store<const void*>(self, 0, &kotor_m15c_res_ltr_methods_0075fc34);           // 007121F0
    c2da_resource_base_destroy_00409d40(self, nullptr);                           // 007121F6
}
// 00712980 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_res_ltr_deleting_destroy_00712980(void* self, void*, Word flags) noexcept {
    kotor_m15c_res_ltr_destroy_007121f0(self, nullptr);                           // 00712983
    if ((flags & 1u) != 0u) live_free(self);                                      // 00712988..95 (_free 006FA390)
    return self;                                                                  // 00712998
}
// 00712200 OnResourceFreed (RET; EAX = 1).
Word __fastcall kotor_m15c_res_ltr_freed_00712200(void* self, void*) {
    store<Word>(self, 0x58, 0);                                                   // 0071220C
    for (unsigned off : {0x40u, 0x44u, 0x48u}) {                                  // 00712204..40
        if (void* p = load<void*>(self, off)) { live_free(p); store<Word>(self, off, 0); }
    }
    for (unsigned char i = 0; i < load<unsigned char*>(self, 0x4C)[8]; ++i) {     // 00712243..A8 (count re-read)
        for (unsigned off : {0x34u, 0x38u, 0x3Cu}) {                              // 00712250..9B
            float** const rows = doubles(self, off);
            if (rows && rows[i]) live_free(rows[i]);
        }
    }
    for (unsigned off : {0x34u, 0x38u, 0x3Cu}) {                                  // 007122AA..E0
        if (void* p = load<void*>(self, off)) { live_free(p); store<Word>(self, off, 0); }
    }
    for (unsigned char i = 0; i < load<unsigned char*>(self, 0x4C)[8]; ++i) {     // 007122E3..B9
        for (unsigned char j = 0; j < load<unsigned char*>(self, 0x4C)[8]; ++j) { // 007122F8..62: no table null checks
            for (unsigned off : {0x28u, 0x2Cu, 0x30u}) {
                if (float* p = triples(self, off)[i][j]) live_free(p);
            }
        }
        for (unsigned off : {0x28u, 0x2Cu, 0x30u}) {                              // 00712368..A4
            if (float** p = triples(self, off)[i]) live_free(p);
        }
    }
    for (unsigned off : {0x28u, 0x2Cu, 0x30u}) {                                  // 007123C2..F9
        if (void* p = load<void*>(self, off)) { live_free(p); store<Word>(self, off, 0); }
    }
    store<Word>(self, 0x4C, 0);                                                   // 007123FC
    store<Word>(self, 0x50, 0);                                                   // 007123FF
    store<Word>(self, 0x54, 0);                                                   // 00712402
    return 1;                                                                     // 00712406
}
// 00712410 OnResourceServiced (RET; EAX = 1 parsed / 0). "LTR V1.0" header, letter count byte at +8, then the
// float stream from data+9: singles start/middle/end, per i doubles start/middle/end, per i,j triples.
Word __fastcall kotor_m15c_res_ltr_serviced_00712410(void* self, void*) noexcept(false) {
    unsigned char* const data = load<unsigned char*>(self, 0x10);                 // 00712417
    if (!data) return 0;                                                          // 0071241E -> 00712450
    if (load<Word>(self, 0x58) != 0) return 1;                                    // 00712420..33
    store<Word>(self, 0x54, load<Word>(self, 0x18) - 9u);                         // 00712434..3A
    store<unsigned char*>(self, 0x4C, data);                                      // 0071243D
    if (load<Word>(data, 0) != 0x2052544Cu) { call_freed_slot(self); return 0; }  // 00712440..53 "LTR "
    if (load<Word>(data, 4) != 0x302E3156u) { call_freed_slot(self); return 0; }  // 0071245A..74 "V1.0"
    const Word n = data[8];                                                       // 00712475 (re-read per use)
    for (unsigned off : {0x40u, 0x44u, 0x48u}) store<void*>(self, off, new_array(n));   // 00712475..B0
    for (unsigned off : {0x40u, 0x44u, 0x48u}) if (!load<void*>(self, off)) return 0;     // 007124AE..BE
    for (unsigned off : {0x34u, 0x38u, 0x3Cu}) store<void*>(self, off, new_array(n));   // 007124C0..FE
    for (unsigned off : {0x34u, 0x38u, 0x3Cu}) if (!load<void*>(self, off)) return 0;     // 007124FC..14
    for (unsigned char i = 0; i < n; ++i) {                                       // 0071251A..A3
        for (unsigned off : {0x34u, 0x38u, 0x3Cu}) doubles(self, off)[i] = static_cast<float*>(new_array(n));
        for (unsigned off : {0x34u, 0x38u, 0x3Cu}) if (!doubles(self, off)[i]) return 0;
    }
    for (unsigned off : {0x28u, 0x2Cu, 0x30u}) store<void*>(self, off, new_array(n));   // 007125A5..E3
    for (unsigned off : {0x28u, 0x2Cu, 0x30u}) if (!load<void*>(self, off)) return 0;     // 007125E1..F9
    for (unsigned char i = 0; i < n; ++i) {                                       // 007125FF..47
        for (unsigned off : {0x28u, 0x2Cu, 0x30u}) triples(self, off)[i] = static_cast<float**>(new_array(n));
        for (unsigned off : {0x28u, 0x2Cu, 0x30u}) if (!triples(self, off)[i]) return 0;
        for (unsigned char j = 0; j < n; ++j) {                                   // 0071268D..2F
            for (unsigned off : {0x28u, 0x2Cu, 0x30u}) triples(self, off)[i][j] = static_cast<float*>(new_array(n));
            for (unsigned off : {0x28u, 0x2Cu, 0x30u}) if (!triples(self, off)[i][j]) return 0;
        }
    }
    unsigned char* const base = data + 9;                                         // 0071274D..56
    store<unsigned char*>(self, 0x50, base);                                      // 00712759
    const Word row = n << 2;
    Word used = 0;
    for (unsigned off : {0x40u, 0x44u, 0x48u}) {                                  // 0071275C..C1 (REP MOVS)
        std::memcpy(singles(self, off), base + used, row); used += row;
    }
    for (unsigned char i = 0; i < n; ++i)                                         // 007127C5..60
        for (unsigned off : {0x34u, 0x38u, 0x3Cu}) { std::memcpy(doubles(self, off)[i], base + used, row); used += row; }
    for (unsigned char i = 0; i < n; ++i)                                         // 00712866..56
        for (unsigned char j = 0; j < n; ++j)
            for (unsigned off : {0x28u, 0x2Cu, 0x30u}) { std::memcpy(triples(self, off)[i][j], base + used, row); used += row; }
    if (used > load<Word>(self, 0x54)) return 0;                                  // 0071295C..5F (JA, unsigned)
    store<Word>(self, 0x58, 1);                                                   // 0071296C
    return 1;
}

// ======================================================================= CResHelper<CResLTR,0x07F4>
// 007110B0 base destructor (table 0075FC0C) = template base_destroy (0070FB30 shape).
void __fastcall kotor_m15c_ltr_helper_destroy_007110b0(NameGen* self, void*) noexcept {
    aurora_family_impl::base_destroy(self, &kotor_m15c_ltr_helper_methods_0075fc0c);   // 007110B0..0F
}
// 00711270 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_ltr_helper_deleting_destroy_00711270(void* self, void*, Word flags) noexcept {
    return aurora_family_impl::deleting<NameGen>(self, flags, &kotor_m15c_ltr_helper_destroy_007110b0);   // 00711270..8B
}
// 00711110 SetResRef(const CResRef* ref, int request) RET 8 = template set_ref (0070FB90 shape):
// tag 07F4h (007111D7/00711212), new(5Ch) (007111E8) + CResLTR::CResLTR 007121B0 (00711200).
void __fastcall kotor_m15c_namegen_set_res_ref_00711110(NameGen* self, void*, menu_resources::Ref* incoming,
                                                        std::int32_t request) noexcept(false) {
    aurora_family_impl::set_ref(self, incoming, request, 0x07F4u, 0x5Cu, &kotor_m15c_res_ltr_construct_007121b0);
}

// ======================================================================= CSWNameGen
// 00711290 ctor (RET; EAX = this): base table, CResRef default + "" assign, resource 0, requested 0, +1C 0,
// +20 word 0, derived table 0075FC10, +22 byte 1.
NameGen* __fastcall kotor_m15c_namegen_construct_00711290(NameGen* self, void*) noexcept(false) {
    self->methods = &kotor_m15c_ltr_helper_methods_0075fc0c;                     // 0071129D
    (void)c2da_default_ref_00405ed0(&self->reference, nullptr);                   // 007112A3
    self->resource = nullptr;                                                     // 007112B6
    {
        menu_resources::Ref discarded;
        (void)unresolved_ref_assign_00406290(&self->reference, nullptr, &discarded, c2da_empty_0073d71c());  // 007112A8..B9
    }
    self->requested = 0;                                                          // 007112BF
    self->loaded = 0;                                                             // 007112C2
    self->race = 0;                                                               // 007112C5
    self->methods = &kotor_m15c_namegen_methods_0075fc10;                        // 007112C9
    self->flags = 1;                                                              // 007112CF
    return self;                                                                  // 007112D3
}
// 007112E0 dtor: table 0075FC10; release a loaded table; base destructor 007110B0.
void __fastcall kotor_m15c_namegen_destruct_007112e0(NameGen* self, void*) noexcept {
    self->methods = &kotor_m15c_namegen_methods_0075fc10;                        // 007112FD
    if (self->loaded) {                                                           // 00711303..0E
        self->loaded = 0;                                                         // 00711313
        c2da_release_demand_00409b80(self->resource, nullptr);                    // 00711310..16
    }
    kotor_m15c_ltr_helper_destroy_007110b0(self, nullptr);                        // 00711325
}
// 00712190 scalar deleting destructor (RET 4).
void* __fastcall kotor_m15c_namegen_deleting_destroy_00712190(void* self, void*, Word flags) noexcept {
    kotor_m15c_namegen_destruct_007112e0(static_cast<NameGen*>(self), nullptr);  // 00712193
    if ((flags & 1u) != 0u) live_free(self);                                      // 00712198..A5
    return self;
}
// 00711090 UnloadNameTable (RET): if loaded { loaded = 0; JMP CRes::Release(resource) }.
void __fastcall kotor_m15c_namegen_unload_00711090(NameGen* self, void*) noexcept {
    if (self->loaded) {                                                           // 00711090..95
        self->loaded = 0;                                                         // 00711097
        c2da_release_demand_00409b80(self->resource, nullptr);                    // 0071109E..A1 (tail jump)
    }
}
// 00711340 LoadNameTable(const CExoString& name) RET 4 -> 1 loaded / 0.
std::int32_t __fastcall kotor_m15c_namegen_load_00711340(NameGen* self, void*, String* name) noexcept(false) {
    if (!demand_table(self, name)) return 0;                                      // 0071134A..6C
    self->loaded = 1;                                                             // 00711375..7A
    return 1;
}

// 00711390 GetRandomName(BYTE count) -> CExoString (hidden return; RET 8). Letter chain over the loaded CResLTR.
String* __fastcall kotor_m15c_namegen_random_letters_00711390(NameGen* self, void*, String* out,
                                                              Word count_word) noexcept(false) {
    const unsigned char count = static_cast<unsigned char>(count_word);           // byte [esp+94h]
    String name;                                                                  // 007113BC [esp+30h]
    void* const ltr = self->resource;                                             // 007113DF
    const std::int32_t n = letter_count(ltr);                                     // 007113E2..E5 [esp+24h]
    char buffer[0x40];                                                            // [esp+40h]
    std::int32_t restart = 1;                                                     // 007113DB [esp+1Ch]
    std::int32_t done = 0;                                                        // 007113F0 [esp+28h]
    unsigned char fails = 0;                                                      // 007113FB [esp+13h]
    std::int32_t edi = 0, esi = 0, ebp = 0, ebx = 0;
    for (;;) {
        // ---- 00711404: first three letters from the start tables
        if (restart) {
            for (;;) {                                                            // 00711414
                float r = random_unit();
                if (n <= 0) continue;                                             // 0071142F JLE (spins as the original)
                edi = pick(r, singles(self->resource, 0x40), n);                  // 00711431..51
                if (edi >= n) continue;                                           // 00711453 / 00711457
                r = random_unit();                                                // 00711459..7B
                esi = pick(r, doubles(self->resource, 0x34)[edi], n);             // 0071146D..91
                if (esi >= n) continue;                                           // 00711493 -> 007114D9 / 00711497
                r = random_unit();                                                // 0071149D..C2
                ebp = pick(r, triples(self->resource, 0x28)[edi][esi], n);        // 007114B1..D7
                if (esi >= n || ebp >= n) continue;                               // 007114D9..E3
                break;
            }
            buffer[0] = letter(edi);                                              // 007114E9..505
            buffer[1] = letter(esi);                                              // 00711502..522
            buffer[2] = letter(ebp);                                              // 0071151F..53C
            ebx = 3;                                                              // 00711540
            restart = 0;                                                          // 00711545
        }
        // ---- 0071154D: advance the two-letter window
        if (ebp < n) { edi = esi; esi = ebp; }                                    // 0071154D..55
        const float r = random_unit();                                            // 00711557..6A
        const std::int32_t roll = crt_rand_006fc447() % 12;                       // 0071156E..79
        if (roll <= ebx || (count != 0 && ebx == static_cast<std::int32_t>(count) - 1)) {  // 0071157B..94
            for (ebp = 0; ebp < n; ++ebp) {                                       // 0071159A..C9: end tables
                if (r < triples(self->resource, 0x30)[edi][esi][ebp]) { done = 1; break; }   // 0071167C
            }
            if (!done) {                                                          // 007115CB..D1
                ++fails;                                                          // 007115D7..E5
                restart = 1;
                goto check;
            }
        }
        // ---- 0071168A
        if (restart) goto check;                                                  // 0071168A..90
        if (!done) {                                                              // 00711696..9C
            for (ebp = 0; ebp < n; ++ebp)                                         // 0071169E..C5: middle tables
                if (r < triples(self->resource, 0x2C)[edi][esi][ebp]) break;
        }
        if (ebp < n) {                                                            // 007116DF..E1
            buffer[ebx++] = letter(ebp);                                          // 007116E3..709
            goto check;
        }
        // ---- 007116C7: dead end -> back up one letter, or restart when only the first three remain
        if (ebx <= 3) {                                                           // 007116C7..CA
            ++fails;                                                              // 007115D7..E5
            restart = 1;
            goto check;
        }
        esi = index_of(buffer[ebx - 2]);                                          // 007116D0..71F
        edi = index_of(buffer[ebx - 3]);                                          // 0071171F..744
        --ebx;                                                                    // 00711747
    check:                                                                        // 007115E9
        if (fails >= 5) {                                                         // 007115E9..EE (JAE)
            new (out) String("");                                                 // 0071174D..5B (0073D71C)
            return out;
        }
        if (done) break;                                                          // 007115F4..FA
    }
    // ---- 00711600: terminate, capitalise, NameFilter.2da check
    buffer[ebx] = 0;                                                              // 00711600
    buffer[0] = static_cast<char>(std::toupper(static_cast<int>(static_cast<signed char>(buffer[0]))));
    // 0071160A 006FD268 (statically linked CRT toupper(int)); the modern CRT toupper is used (C locale: identical
    // mapping for the letter set a-z, '\'', '-'), as lane-m15-0 m15_sound.cpp.
    (void)GuiAssign005e5140(&name, nullptr, buffer);                              // 00711617..20 (005E5140)
    String entry;                                                                 // 00711629 [esp+38h]
    kotor_rules::Table* const filter = kotor_rules::new_c2da("NameFilter");      // 0071162E..75 (0075FC14)
    if (!filter) {                                                                // 0071176F..80
        new (out) String(name);                                                   // 0071179A..A3 (005E5B00)
        return out;
    }
    if (!c2da_current_load_004143b0(filter, nullptr)) {                           // 00711782..8B
        kotor_rules::delete_c2da(filter);                                         // 0071178D..93 [vtbl+0](1)
        new (out) String(name);                                                   // 00711795..A3
        return out;
    }
    std::int32_t keep = 1;                                                        // 007117BC [esp+94h]
    for (std::int32_t row = 0; row < kotor_rules::row_count(filter); ++row) {     // 007117B5..28
        const std::int32_t found = kotor_rules::string_entry(filter, row, "Name", &entry);   // 007117D0..805 (00745EEC)
        if (found && exostring_compare_no_case_005e6450(&entry, nullptr, &name) != 0) {      // 0071180A..1E
            keep = 0;                                                             // 0071182A
            break;
        }
    }
    c2da_current_unload_004139e0(filter, nullptr);                                // 00711835..37
    kotor_rules::delete_c2da(filter);                                             // 0071183C..46 [vtbl+0](1)
    if (!keep) return kotor_m15c_namegen_random_letters_00711390(self, nullptr, out, 0);   // 0071185A..61
    new (out) String(name);                                                       // 00711868..6F
    return out;
}

// 007118C0 GetRandomName(WORD race, BYTE flags, BYTE count) -> CExoString (hidden return; RET 10h).
// race: 0xFFFF animal, 0..6 via jump table 00712170; flags: 1 male first, 2 female first, 4 last name.
String* __fastcall kotor_m15c_namegen_random_name_007118c0(NameGen* self, void*, String* out, Word race_word,
                                                           Word flags_word, Word count_word) noexcept(false) {
    const std::uint16_t race = static_cast<std::uint16_t>(race_word);             // di (00711918)
    const unsigned char flags = static_cast<unsigned char>(flags_word);           // bl (007119AA)
    const unsigned char count = static_cast<unsigned char>(count_word);           // [esp+B8h]
    String base;                                                                  // 007118ED [esp+14h]
    String first;                                                                 // 00711902 [esp+1Ch]
    String last;                                                                  // 00711913 [esp+24h]
    if (race != 0xFFFFu && race > 6) {                                            // 00711930..3B
        new (out) String("");                                                     // 00712109..17
        return out;
    }
    const char* race_name = nullptr;
    switch (race) {                                                               // 00711941 jump table 00712170
    case 0xFFFF: race_name = "animal"; break;                                     // 00711997 (0075FC2C)
    case 0: race_name = "dwarf"; break;                                           // 0071194F (00752C68)
    case 1: race_name = "elf"; break;                                             // 00711956 (00752C7C)
    case 2: race_name = "gnome"; break;                                           // 0071195D (00752C60)
    case 3: race_name = "halfling"; break;                                        // 00711964 (00752C70)
    case 4:                                                                       // 0071196B: rand() % 2 (signed)
        race_name = (crt_rand_006fc447() % 2) != 0 ? "elf" : "human";             // 00711982 / 00711989
        break;
    case 5: race_name = "halforc"; break;                                         // 00711990 (00752C80)
    default: race_name = "human"; break;                                          // 6: 00711948 (00752C88)
    }
    (void)GuiAssign005e5140(&base, nullptr, race_name);                           // 007119A0 (005E5140)
    if (race != 0xFFFFu) {                                                        // 007119A5..B1
        switch (flags) {                                                          // 007119B7..CB
        case 1: assign_concat(&first, &base, "m"); goto single;                   // 00711E3B (0075FC28)
        case 2: assign_concat(&first, &base, "f"); goto single;                   // 00711DDE (0075FC24)
        case 4: assign_concat(&last, &base, "l"); goto single;                    // 00711D18 (007551B8)
        default: break;
        }
        if ((flags & 1u) && (flags & 2u)) {                                       // 007119D1..DA
            new (out) String("");                                                 // 00711DC6..D4
            return out;
        }
        if (flags & 1u) assign_concat(&first, &base, "m");                        // 007119E0..A3A
        else if (flags & 2u) assign_concat(&first, &base, "f");                   // 00711A3C..9F
        if (flags & 4u) assign_concat(&last, &base, "l");                         // 00711AA4..B04
        if (first.BorrowedLength005e5790() == 0 || last.BorrowedLength005e5790() == 0) {   // 00711B09..25
            new (out) String("");                                                 // 00711DC6
            return out;
        }
        // ---- 00711B2B: first name + " " + last name
        String a;                                                                 // 00711B2F [esp+2Ch]
        String b;                                                                 // 00711B40 [esp+34h]
        release_loaded(self);                                                     // 00711B45..5E
        if (count == 0) {                                                         // 00711B63..6C
            if (!kotor_m15c_namegen_load_00711340(self, nullptr, &first)) {       // 00711F49..57
                new (out) String("");                                             // 0071204A..58
                return out;
            }
            {
                Fresh t;
                t.object = kotor_m15c_namegen_random_letters_00711390(self, nullptr, t.storage(), 0);   // 00711F5D..66
                a.Assign005e5c50(t.object);                                       // 00711F6B..78
            }
            kotor_m15c_namegen_unload_00711090(self, nullptr);                    // 00711F8E..90
            if (!kotor_m15c_namegen_load_00711340(self, nullptr, &last)) {        // 00711F95..A3
                new (out) String("");
                return out;
            }
            {
                Fresh t;
                t.object = kotor_m15c_namegen_random_letters_00711390(self, nullptr, t.storage(), 0);   // 00711FA9..B2
                b.Assign005e5c50(t.object);                                       // 00711FB7..C4
            }
            kotor_m15c_namegen_unload_00711090(self, nullptr);                    // 00711FDA..DC
            if (a.BorrowedLength005e5790() <= 0 || b.BorrowedLength005e5790() <= 0) {   // 00711FE1..F9
                new (out) String("");                                             // 0071204A
                return out;
            }
        } else {
            const std::int32_t third = static_cast<std::int32_t>(count) / 3;      // 00711B72..83 (imul 55555556h)
            std::int32_t tries = crt_rand_006fc447() % third + third;             // 00711B85..8F (IDIV: count 1..2 faults as the original)
            if (tries <= 0) {                                                     // 00711B91..93 -> 00711F28
                new (out) String("");
                return out;
            }
            const std::int32_t two_thirds = (static_cast<std::int32_t>(count) * 2) / 3;   // 00711B99..AA [esp+4Ch]
            for (;;) {                                                            // 00711BB0
                for (std::int32_t k = tries; k < two_thirds; ++k) {               // 00711BB0..B6 / 00711CDA..E1
                    if (!demand_table(self, &first)) {                            // 00711BC0..E6
                        new (out) String("");                                     // 00711E98..A6
                        return out;
                    }
                    self->loaded = 1;                                             // 00711BF9
                    {
                        Fresh t;
                        t.object = kotor_m15c_namegen_random_letters_00711390(self, nullptr, t.storage(),
                                                                              static_cast<Word>(k));   // 00711BEC..FC
                        a.Assign005e5c50(t.object);                               // 00711C01..0E
                    }
                    release_loaded(self);                                         // 00711C24..35
                    if (!demand_table(self, &last)) {                             // 00711C3A..66
                        new (out) String("");                                     // 00711F2D..3B
                        return out;
                    }
                    self->loaded = 1;                                             // 00711C7B
                    {
                        const unsigned char rest = static_cast<unsigned char>(count - static_cast<unsigned char>(k));  // 00711C6C..75
                        Fresh t;
                        t.object = kotor_m15c_namegen_random_letters_00711390(self, nullptr, t.storage(), rest);  // 00711C7E..82
                        b.Assign005e5c50(t.object);                               // 00711C87..94
                    }
                    release_loaded(self);                                         // 00711CAA..BB
                    if (a.BorrowedLength005e5790() > 0 && b.BorrowedLength005e5790() > 0) break;   // 00711CC0..D8
                }
                if (a.BorrowedLength005e5790() > 0 && b.BorrowedLength005e5790() > 0) break;       // 00711CEC..04
                if (--tries <= 0) {                                               // 00711D0A..0D
                    new (out) String("");                                         // 00711F2D
                    return out;
                }
            }
        }
        // 00711EB8 / 00711FFB: out = (a + " ") + b, the second operator+ constructing straight into out.
        String space(" ");                                                        // 00711EB8..C1 (0073D5D8)
        Fresh left;
        left.object = GuiConcat005e5d10(&a, nullptr, left.storage(), &space);     // 00711EC6..DD
        (void)GuiConcat005e5d10(left.object, nullptr, out, &b);                   // 00711EE2..F9
        return out;
    }
single:                                                                           // 00711D75
    if (self->loaded) {
        if (self->race == race && self->flags == static_cast<unsigned char>(flags & 0xFBu)) {   // 00711D7C..8C
            new (out) String("");                                                 // 00711DC6
            return out;
        }
        release_loaded(self);                                                     // 00711D8E..9C
    }
    if (first.BorrowedLength005e5790() > 0) {                                     // 00711DA1..AC
        if (!kotor_m15c_namegen_load_00711340(self, nullptr, &first)) {           // 00711DB2..C0
            new (out) String("");
            return out;
        }
    } else if (last.BorrowedLength005e5790() > 0) {                               // 00712088..93
        if (!kotor_m15c_namegen_load_00711340(self, nullptr, &last)) {            // 00712099..A7
            new (out) String("");
            return out;
        }
    } else {
        new (out) String("");                                                     // 00711DC6
        return out;
    }
    self->race = race;                                                            // 007120B4
    self->flags = static_cast<unsigned char>(flags & 0xFBu);                      // 007120C0..C6
    return kotor_m15c_namegen_random_letters_00711390(self, nullptr, out, count_word);   // 007120AD..C9
}
}
