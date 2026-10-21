// lane-m15-a unit m15a-creature: CSWCCreature members used by character generation (and gameplay, M2),
// CSWCCombatAttackData / CSWCSpellCastVisual constructors, Yaw / Pitch.
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; every behaviour cites its
// instruction addresses. Conventions: m15a_common.h (D1 fastcall veneers, ld/st/rp/rd32 object views, D24 slot
// views), D21 (naked x87 ports), D30-FINAL (x87 wide intermediates as double, float only at fstp dword stores,
// __ftol2 through the original CRT port 006FAE8C), D2 (labelled value-init of never-written stack bytes).
#include "8d831d5c6afcac70_m15a_creature.h"
#include "468a2f31a138f45b_m15a_object.h"       // sibling unit m15a-object: CSWCObject members 0063C060..0063E530
#include "bbcb007afea348df_m15a_appearance.h"   // sibling unit m15a-appearance: CSWCCreatureAppearance 00696430..00698B10
#include "f155d76cd432eed7_m15_l0.h"            // lane-m15-0 shared leaves
#include "bf662019daaaa2b6_m15b_stats.h"        // lane-m15-b creature stats / rules leaves
#include "e1805047c9b7563b_startup_string.h"    // CExoString storage (kotor_reconstruction::StartupString; 005B3190/005E5A90/005E5C20)
#include "79da78e626bf1ab1_string_priority.h"   // GuiAssign005e5140 / GuiConcat005e5d10
#include <new>

using kotor_m15a::Word;
using kotor_m15a::I32;
using kotor_m15a::Vector;
using kotor_m15a::Quaternion;
using kotor_m15a::ld;
using kotor_m15a::st;
using kotor_m15a::rp;
using kotor_m15a::rd32;
using kotor_m15a::at;
using String = kotor_reconstruction::StartupString;

// ------------------------------------------------------------------------------- external providers (M60 map)
extern "C" {
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);                       // 006FA7E6 operator new
void __cdecl live_free(void*);                                                                 // 006FA390 free
std::int32_t __cdecl crt_rand_006fc447();                                                      // 006FC447 rand (D52)
void __cdecl genuine_x87_ftol_006fae8c();                                                      // 006FAE8C __ftol2 (ST0 -> EDX:EAX)
char* __fastcall WindowCStr005e5670(String*, void*);                                           // 005E5670 CExoString::CStr
std::int32_t __fastcall c2da_equal_text_004060d0(const void* ref, void*, const char*);          // 004060D0 CResRef::operator==
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, const String*);         // 00406D60 CResRef(const CExoString&)
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, std::uint32_t*, std::uint32_t*); // 004ADE40 RET 8
std::int32_t __fastcall c2da_current_get_string_entry_00413190(void* table, void*, std::int32_t row,
                                                               std::int32_t column, String* out);   // 00413190 RET 0Ch
std::int32_t __fastcall c2da_current_get_int_entry_00413510(void* table, void*, std::int32_t row,
                                                            std::int32_t column, std::int32_t* out); // 00413510 RET 0Ch
void* __fastcall gob_get_property_0x68_0063c070(void* object, void*);
void* __fastcall kotor_fastcall_00405ed0(void* ref, void*);                                    // 00405ED0 CResRef::CResRef()                          // 0063C070 -> [this+68h]
void* __fastcall kotor_fastcall_005ed700(void* app, void*);                                    // 005ED700 CClientExoApp::GetClientOptions
void* __fastcall kotor_fastcall_005ed530(void* app, void*);                                    // 005ED530 CClientExoApp::GetModule
Vector* __cdecl normalize_004ab180(Vector* out, const Vector* v);                               // 004AB180 normalize
Quaternion* __cdecl rotation_arc_004ab630(Quaternion* out, Vector from, Vector to);             // 004AB630 RotationArc
// .data cells (owners per M60 map)
extern std::uint32_t kotor_rules_cell32_007a20ec;   // appearance.2da column index (rules two_dim_arrays.cpp)
extern std::uint32_t kotor_rules_cell32_007a219c;   // footstepsounds column index
extern std::uint32_t kotor_rules_cell32_007a2124;   // appearance.2da column index
extern std::uint32_t kotor_rules_cell32_007a21a0;   // footstepsounds column index
extern void* kotor_cell_rules_007a3a28;             // g_pRules (game-prepare prepare_image_data)
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                  // 007A39FC AppManager
namespace kotor_input_required { void* __fastcall get_world_timer_005ed4f0(void* app, void*); } // 005ED4F0
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); } // 005E5680 CExoString::Format
namespace kotor_menu20 { void __fastcall OpenLogs0060e760(void*, void*, unsigned int); }        // 0060E760 void_return RET 4
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void* self, void*, const void* ref);                  // 005D60E0 CExoSoundSource(CResRef)
void __fastcall SourceSetPriorityGroup005d5900(void* source, void*, unsigned int group);        // 005D5900 RET 4
}

// ------------------------------------------------------------------------------- owned cell
// 007A22F8 (.data, image value FFFFFFFFh): alignment override read only by Setup3DScene 00610136; no writer in the
// original image (single reference in .text). Owner: this unit.
extern "C" volatile std::uint32_t kotor_m15a_cell_alignment_override_007a22f8 = 0xFFFFFFFFu;

namespace {
constexpr std::uint32_t kInvalidId = 0x7F000000u;   // .rdata 00746644 (OBJECT_INVALID)

void* app_internal() { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }           // [[007A39FCh]+4]

// ---- typed slot views (D24 object views of method tables; only called slots typed)
// CSWCCreature 0074F5F8 (SLOT-AUDIT.json names).
struct CreatureSlots {
    void* (__fastcall* s000_deleting_destructor)(void*, void*, Word);   // +000
    const void* s004_084[0x21];
    void (__fastcall* s088_set_orientation)(void*, void*, const Vector*); // +088 RET 4 (006135D6)
    void (__fastcall* s08c_set_position)(void*, void*, const Vector*);    // +08C RET 4 (006135B3)
    const void* s090_098[3];
    void (__fastcall* s09c_set_animation)(void*, void*, Word, Word, Word); // +09C RET 0Ch (006136A4/006136B5)
    const void* s0a0_0d4[0xE];
    float (__fastcall* s0d8_get_head_height_offset)(void*, void*);         // +0D8 ST0 (00610411)
    const void* s0dc;
    void (__fastcall* s0e0_detach_visual_effects)(void*, void*);           // +0E0 (006134CB)
    void (__fastcall* s0e4_reattach_visual_effects)(void*, void*);         // +0E4 (006136CE)
    const void* s0e8_0f8[5];
    void (__fastcall* s0fc_apply_shadow_blob)(void*, void*);               // +0FC (0060E63D)
    void (__fastcall* s100_remove_shadow_blob)(void*, void*);              // +100 (0060E5CF/0060E5F3/0060E633)
};
static_assert(offsetof(CreatureSlots, s088_set_orientation) == 0x88 && offsetof(CreatureSlots, s09c_set_animation) == 0x9C &&
              offsetof(CreatureSlots, s0d8_get_head_height_offset) == 0xD8 && offsetof(CreatureSlots, s0e0_detach_visual_effects) == 0xE0 &&
              offsetof(CreatureSlots, s0fc_apply_shadow_blob) == 0xFC && offsetof(CreatureSlots, s100_remove_shadow_blob) == 0x100,
              "creature slots");
const CreatureSlots* cslots(void* creature) { return static_cast<const CreatureSlots*>(rp(creature, 0)); }

// Generic scalar-deleting-destructor slot 0 (CSoundSet 00752CA0, CExoSoundSource, VFX object): call [vptr](1).
struct DeletingSlot { void* (__fastcall* s00)(void*, void*, Word); };
void delete_through_slot0(void* object) { static_cast<const DeletingSlot*>(rp(object, 0))->s00(object, nullptr, 1); }

// CSWCAnimBase family [creature+68h] (vtable 00754F60 et al., SLOT-AUDIT anim_base_frontier).
struct AnimBaseSlots {
    const void* s00_04[2];
    void* (__fastcall* s08_get_model)(void*, void*, Word part);           // +08 RET 4 (0060F898, 00610268)
    const void* s0c_b0[0x2A];
    void (__fastcall* sb4_set_shadow_orientation)(void*, void*, const Quaternion*); // +B4 RET 4 (0060E6A9)
};
static_assert(offsetof(AnimBaseSlots, s08_get_model) == 8 && offsetof(AnimBaseSlots, sb4_set_shadow_orientation) == 0xB4,
              "anim base slots");
const AnimBaseSlots* aslots(void* anim) { return static_cast<const AnimBaseSlots*>(rp(anim, 0)); }

// Aurora gob/model (results of anim base +08 and CSWGuiScene::GetModel).
struct GobSlots {
    const void* s00_14[6];
    void (__fastcall* s18_play_animation)(void*, void*, const char*, float speed, Word flags, float start); // +18 RET 10h
    void (__fastcall* s1c_animation_info)(void*, void*, const char*, void* a2, float* length);            // +1C RET 0Ch
    const void* s20_108[0x3B];
    Word (__fastcall* s10c_has_node)(void*, void*, const char*);                                          // +10C RET 4 (006103AF)
    const void* s110_124[6];
    void (__fastcall* s128_slot)(void*, void*, Word);                                                     // +128 RET 4 (0060FA06)
};
static_assert(offsetof(GobSlots, s18_play_animation) == 0x18 && offsetof(GobSlots, s10c_has_node) == 0x10C &&
              offsetof(GobSlots, s128_slot) == 0x128, "gob slots");
const GobSlots* gslots(void* gob) { return static_cast<const GobSlots*>(rp(gob, 0)); }

// Aurora camera [scene view+74h].
struct CameraSlots {
    const void* s00_40[0x11];
    void (__fastcall* s44_set_view_angle)(void*, void*, float);                    // +44 RET 4 (006103EB)
    const void* s48_70[0xB];
    void (__fastcall* s74_attach)(void*, void*, void* model, const char* hook, Word); // +74 RET 0Ch (006103DB)
    const void* s78_7c[2];
    void* (__fastcall* s80_find_object)(void*, void*, Word id);                    // +80 RET 4 (006103F8)
};
static_assert(offsetof(CameraSlots, s44_set_view_angle) == 0x44 && offsetof(CameraSlots, s74_attach) == 0x74 &&
              offsetof(CameraSlots, s80_find_object) == 0x80, "camera slots");

// Server object (result of CSWCObject::GetServerObject) vslot +30h (CSWSObject::AsSWSCreature).
struct ServerSlots { const void* s00_2c[0xC]; void* (__fastcall* s30_as_creature)(void*, void*); };
static_assert(offsetof(ServerSlots, s30_as_creature) == 0x30, "server slots");

// Hidden-return CExoString storage (callee constructs it; destroyed at scope end, 005E5C20).
struct OutString {
    alignas(4) unsigned char raw[8];
    bool live = false;
    String* get() { return reinterpret_cast<String*>(raw); }
    ~OutString() { if (live) get()->~StartupString(); }
};

// `fld <double expr>; call 006FAE8C` -> EAX (low dword of __ftol2), D30-FINAL original CRT port.
std::int32_t x87_ftol(double value) {
    std::int32_t r;
    __asm {
        fld qword ptr value
        call genuine_x87_ftol_006fae8c
        mov r, eax
    }
    return r;
}
}

extern "C" {
// =============================================================================== 004AA0F0 Yaw / 004AA130 Pitch
// Exact x87 ports (D21 naked; constants are the original .rdata values: 0073EDF8 = 0.0 (double), 0073D700 = 0.0f,
// 00742EF8 = 57.29578f).
static const double kYawZero_0073edf8 = 0.0;
static const float kYawZeroF_0073d700 = 0.0f;
static const float kRadToDeg_00742ef8 = 57.2957801818847656f;
__declspec(naked) float __cdecl kotor_m15a_yaw_004aa0f0(const void*) {
    __asm {
        mov ecx, dword ptr [esp+4]                  // 004AA0F0
        fld dword ptr [ecx+4]                       // 004AA0F4 y
        fcomp qword ptr [kYawZero_0073edf8]         // 004AA0F7
        fnstsw ax                                   // 004AA0FD
        test ah, 44h                                // 004AA0FF
        jp yaw_atan                                 // 004AA102 y != 0
        fld dword ptr [ecx]                         // 004AA104 x
        fcomp qword ptr [kYawZero_0073edf8]         // 004AA106
        fnstsw ax                                   // 004AA10C
        test ah, 44h                                // 004AA10E
        jp yaw_atan                                 // 004AA111 x != 0
        fld dword ptr [kYawZeroF_0073d700]          // 004AA113 both zero -> 0.0
        ret                                         // 004AA119
    yaw_atan:
        fld dword ptr [ecx]                         // 004AA11A
        fchs                                        // 004AA11C -x
        fld dword ptr [ecx+4]                       // 004AA11E y
        fpatan                                      // 004AA121 atan2(-x, y)
        fmul dword ptr [kRadToDeg_00742ef8]         // 004AA123
        ret                                         // 004AA129
    }
}
__declspec(naked) float __cdecl kotor_m15a_pitch_004aa130(const void*) {
    __asm {
        mov eax, dword ptr [esp+4]                  // 004AA130
        fld dword ptr [eax]                         // 004AA134 x
        fld dword ptr [eax+4]                       // 004AA136 y
        fld dword ptr [eax+8]                       // 004AA139 z
        fld st(1)                                   // 004AA13C
        fmul st, st(2)                              // 004AA13E y*y
        fld st(3)                                   // 004AA140
        fmul st, st(4)                              // 004AA142 x*x
        faddp st(1), st                             // 004AA144
        fsqrt                                       // 004AA146 r = sqrt(x*x + y*y)
        fstp st(3)                                  // 004AA148
        fxch st(1)                                  // 004AA14A
        fxch st(2)                                  // 004AA14C
        fpatan                                      // 004AA14E atan2(z, r)
        fstp st(1)                                  // 004AA150
        fmul dword ptr [kRadToDeg_00742ef8]         // 004AA152
        ret                                         // 004AA158
    }
}

// =============================================================================== 0060B630 CSWCCombatAttackData()
void* __fastcall kotor_m15a_combat_attack_data_construct_0060b630(void* self, void*) {
    st<Word>(self, 0x2C, 0); st<Word>(self, 0x30, 0); st<Word>(self, 0x34, 0);   // 0060B637..3D
    st<Word>(self, 0x04, kInvalidId);                                             // 0060B645
    st<Word>(self, 0x14, kInvalidId);                                             // 0060B648
    st<Word>(self, 0x2C, 0); st<Word>(self, 0x30, 0);                             // 0060B64D..50 (zero vector copy)
    st<std::uint8_t>(self, 0x00, 0); st<std::uint8_t>(self, 0x01, 0);             // 0060B653..55
    st<std::uint16_t>(self, 0x08, 0); st<std::uint16_t>(self, 0x0A, 0);           // 0060B658..5C
    st<std::uint8_t>(self, 0x0C, 0);                                              // 0060B660
    st<std::uint16_t>(self, 0x1E, 0);                                             // 0060B663
    st<Word>(self, 0x10, 0);                                                      // 0060B667
    st<std::uint16_t>(self, 0x18, 0); st<std::uint16_t>(self, 0x1A, 0); st<std::uint16_t>(self, 0x1C, 0); // 0060B66A..72
    st<Word>(self, 0x34, 0);                                                      // 0060B682
    st<Word>(self, 0x38, 0xFFFFFFFFu);                                            // 0060B685
    st<Word>(self, 0x3C, 0); st<Word>(self, 0x44, 0);                             // 0060B68C..8F
    st<Word>(self, 0x20, 0); st<Word>(self, 0x24, 0);                             // 0060B692..95
    st<std::uint8_t>(self, 0x28, 0);                                              // 0060B698
    st<Word>(self, 0x40, 0);                                                      // 0060B69B
    return self;                                                                  // 0060B633 eax = this
}

// =============================================================================== 00695CE0 CSWCSpellCastVisual()
void* __fastcall kotor_m15a_spell_cast_visual_construct_00695ce0(void* self, void*) {
    for (unsigned off = 0; off < 0x30; off += 4) st<Word>(self, off, 0);          // 00695CE4..00695D04
    return self;                                                                  // 00695CE0 eax = this
}

// =============================================================================== 0060B7C0 SetSoundSet(WORD)
void __fastcall kotor_m15a_creature_set_sound_set_0060b7c0(void* self, void*, std::uint32_t sound_set) noexcept(false) {
    const std::uint16_t id = static_cast<std::uint16_t>(sound_set);              // 0060B7CE ax = arg
    if (id == ld<std::uint16_t>(self, 0x2FC)) return;                            // 0060B7E0..E7
    void* const old = rp(self, 0x36C);                                           // 0060B7ED
    st<std::uint16_t>(self, 0x2FC, id);                                          // 0060B7F5
    if (old) {                                                                   // 0060B7F3..FC
        delete_through_slot0(old);                                               // 0060B7FE..802 [eax](1)
        st<void*>(self, 0x36C, nullptr);                                         // 0060B804
    }
    if (ld<std::uint16_t>(self, 0x2FC) == 0xFFFFu) return;                       // 0060B80E..17
    void* const list = kotor_m15_client_get_sound_set_list_005ed910(app_internal(), nullptr); // 0060B819..22
    if (!list) return;                                                           // 0060B827..29
    alignas(4) unsigned char ref[16];                                            // [esp+4] CResRef (hidden return)
    kotor_m15_sound_set_list_get_006788d0(list, nullptr, ref, ld<std::uint16_t>(self, 0x2FC)); // 0060B82B..3C
    if (c2da_equal_text_004060d0(ref, nullptr, "") != 0) return;                 // 0060B841..51 (0073D71C "")
    void* const memory = c2da_directory_new_006fa7e6(0x1C);                      // 0060B853..55
    void* sound = nullptr;                                                       // 0060B87B
    if (memory) {                                                                // 0060B861..6B
        try {
            sound = kotor_m15_sound_set_construct_00678b20(memory, nullptr, ref);  // 0060B86D..74
        } catch (...) { live_free(memory); throw; }                               // EH state 0 (FrameHandler 00727C8B)
    }
    st<void*>(self, 0x36C, sound);                                               // 0060B87D
}

// =============================================================================== 0060BAD0 UnloadMonkVisuals()
void __fastcall kotor_m15a_creature_unload_monk_visuals_0060bad0(void* self, void*) {
    void* const vfx = rp(self, 0x170);                                           // 0060BAD3
    if (!vfx) return;                                                            // 0060BAD9..DB
    struct Slot4c { const void* s00_48[0x13]; void (__fastcall* s4c)(void*, void*, Word); };
    static_assert(offsetof(Slot4c, s4c) == 0x4C, "+4C");
    static_cast<const Slot4c*>(rp(vfx, 0))->s4c(vfx, nullptr, 0);               // 0060BADD..E1 [eax+4Ch](0)
    void* const again = rp(self, 0x170);                                         // 0060BAE4 (re-read)
    if (again) delete_through_slot0(again);                                      // 0060BAEA..F2 [edx](1)
    st<void*>(self, 0x170, nullptr);                                             // 0060BAF4
    st<Word>(self, 0x174, 0);                                                    // 0060BAFE
    st<Word>(self, 0x178, 0);                                                    // 0060BB08
}

// =============================================================================== 0060C8C0 SetCreatureType(BYTE)
void __fastcall kotor_m15a_creature_set_creature_type_0060c8c0(void* self, void*, std::uint32_t type) {
    const std::uint8_t value = static_cast<std::uint8_t>(type);                  // 0060C8C0 al = arg
    st<std::uint8_t>(self, 0x218, value);                                        // 0060C8C4
    st<std::uint8_t>(rp(self, 0x68), 0xC6, value);                               // 0060C8CA..CD [[this+68h]+C6h]
}

// =============================================================================== 0060CA80 GetEquippedItemID
// Slot bits 1..80h via jump tables 0060CB68 / 0060CB88 -> +220h..+23Ch; 100h..20000h -> +240h..+258h;
// anything else -> OBJECT_INVALID (00746644).
std::uint32_t __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* self, void*, std::uint32_t slot) {
    switch (slot) {
    case 0x1: return rd32(self, 0x220);       // 0060CAA5
    case 0x2: return rd32(self, 0x224);       // 0060CAAE
    case 0x4: return rd32(self, 0x228);       // 0060CAB7
    case 0x8: return rd32(self, 0x22C);       // 0060CAC0
    case 0x10: return rd32(self, 0x230);      // 0060CAC9
    case 0x20: return rd32(self, 0x234);      // 0060CAD2
    case 0x40: return rd32(self, 0x238);      // 0060CADB
    case 0x80: return rd32(self, 0x23C);      // 0060CAE4
    case 0x100: return rd32(self, 0x240);     // 0060CB1D
    case 0x200: return rd32(self, 0x244);     // 0060CB14
    case 0x400: return rd32(self, 0x248);     // 0060CB0B
    case 0x4000: return rd32(self, 0x24C);    // 0060CB26
    case 0x8000: return rd32(self, 0x250);    // 0060CB5E
    case 0x10000: return rd32(self, 0x254);   // 0060CB55
    case 0x20000: return rd32(self, 0x258);   // 0060CB4C
    default: return kInvalidId;               // 0060CB44 [00746644h]
    }
}

// =============================================================================== 0060CED0 GetLastInstant
// Same dispatch (tables 0060CFB4 / 0060CFD4) over +270h..+2A8h; anything else -> 0 (0060CF94).
std::uint32_t __fastcall kotor_m15a_creature_get_last_instant_0060ced0(void* self, void*, std::uint32_t slot) {
    switch (slot) {
    case 0x1: return rd32(self, 0x270);       // 0060CEF5
    case 0x2: return rd32(self, 0x274);       // 0060CEFE
    case 0x4: return rd32(self, 0x278);       // 0060CF07
    case 0x8: return rd32(self, 0x27C);       // 0060CF10
    case 0x10: return rd32(self, 0x280);      // 0060CF19
    case 0x20: return rd32(self, 0x284);      // 0060CF22
    case 0x40: return rd32(self, 0x288);      // 0060CF2B
    case 0x80: return rd32(self, 0x28C);      // 0060CF34
    case 0x100: return rd32(self, 0x290);     // 0060CF6D
    case 0x200: return rd32(self, 0x294);     // 0060CF64
    case 0x400: return rd32(self, 0x298);     // 0060CF5B
    case 0x4000: return rd32(self, 0x29C);    // 0060CF76
    case 0x8000: return rd32(self, 0x2A0);    // 0060CFAB
    case 0x10000: return rd32(self, 0x2A4);   // 0060CFA2
    case 0x20000: return rd32(self, 0x2A8);   // 0060CF99
    default: return 0;                        // 0060CF94
    }
}

// =============================================================================== 0060D600 SetName(first, last)
void __fastcall kotor_m15a_creature_set_name_0060d600(void* self, void*, const void* first, const void* last) noexcept(false) {
    String name;                                                                  // 0060D622..26 [esp+8] (005B3190)
    void* const stats = rp(self, 0x2F8);                                          // 0060D62B
    static_cast<String*>(at(stats, 0x14))->Assign005e5c50(
        const_cast<String*>(static_cast<const String*>(first)));                  // 0060D631..40 stats+14h = first
    static_cast<String*>(at(stats, 0x1C))->Assign005e5c50(
        const_cast<String*>(static_cast<const String*>(last)));                   // 0060D645..4C stats+1Ch = last
    GuiAssign005e5140(&name, nullptr, "[");                                       // 0060D651..5A (0074394C)
    {
        String close("]");                                                        // 0060D65F..68 (0074F34C) [esp+10h]
        OutString full;                                                           // [esp+28h] hidden return
        kotor_m15b_creature_stats_get_full_name_00647ce0(rp(self, 0x2F8), nullptr, full.get()); // 0060D66D..7D
        full.live = true;
        OutString left;                                                           // [esp+20h]
        String* const l = GuiConcat005e5d10(&name, nullptr, left.get(), full.get()); // 0060D682..91 "[" + full
        left.live = true;
        OutString both;                                                           // [esp+18h]
        String* const b = GuiConcat005e5d10(l, nullptr, both.get(), &close);      // 0060D696..A7 + "]"
        both.live = true;
        name.Assign005e5c50(b);                                                   // 0060D6AC..B6
    }   // 0060D6BB..EE: ~[esp+18h], ~[esp+20h], ~[esp+28h], ~[esp+10h] (reverse declaration order = original order)
    // 0060D6F3..FA / 0060D6FF..705: two calls of the void_return leaf 0060E760 (RET 4) with &name and [this+4].
    kotor_menu20::OpenLogs0060e760(self, nullptr, static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(&name)));
    kotor_menu20::OpenLogs0060e760(self, nullptr, rd32(self, 0x4));
}                                                                                  // 0060D70A..16 ~name

// =============================================================================== 0060E5B0 UpdateShadowBlob()
void __fastcall kotor_m15a_creature_update_shadow_blob_0060e5b0(void* self, void*) {
    void* const options = kotor_fastcall_005ed700(app_internal(), nullptr);      // 0060E5B0..BF
    const std::uint8_t mode = ld<std::uint8_t>(options, 0x6C);                   // 0060E5C4
    if (mode == 2) {                                                             // 0060E5C7..C9
        cslots(self)->s100_remove_shadow_blob(self, nullptr);                    // 0060E5CB..CF
    } else if (mode == 1 &&
               rd32(self, 0x4) == kotor_m15_client_get_player_creature_id_005ed550(app_internal(), nullptr)) { // 0060E5D7..EF
        cslots(self)->s100_remove_shadow_blob(self, nullptr);                    // 0060E5F1..F3
        if (kotor_fastcall_005ed530(app_internal(), nullptr)) {                  // 0060E5F9..608 GetModule
            void* const module = kotor_fastcall_005ed530(app_internal(), nullptr); // 0060E60A..613
            if (rp(module, 0x48)) (void)kotor_fastcall_005ed530(app_internal(), nullptr); // 0060E618..628 (result unused)
        }
    } else {                                                                     // 0060E62F / 0060E631
        cslots(self)->s100_remove_shadow_blob(self, nullptr);                    // 0060E631..633
        cslots(self)->s0fc_apply_shadow_blob(self, nullptr);                     // 0060E639..63D
    }
    void* const anim = rp(self, 0x68);                                           // 0060E643
    if (!anim) return;                                                           // 0060E646..48
    const Vector up = {0.0f, 0.0f, 1.0f};                                        // 0060E64A..5A
    Vector to;                                                                   // 0060E667..80 copy of [this+D8h]
    std::memcpy(&to, at(self, 0xD8), sizeof to);
    Quaternion arc;
    Quaternion* const q = rotation_arc_004ab630(&arc, up, to);                   // 0060E687..9D RotationArc(&out, up, to)
    aslots(rp(self, 0x68))->sb4_set_shadow_orientation(rp(self, 0x68), nullptr, q); // 0060E6A2..A9 (anim reloaded)
}

// =============================================================================== 0060F7C0 CGPauseCycle(float, int)
void __fastcall kotor_m15a_creature_cg_pause_cycle_0060f7c0(void* self, void*, float delta_seconds,
                                                           std::uint32_t restart) noexcept(false) {
    void* const anim0 = rp(self, 0x68);                                          // 0060F7E3
    if (!anim0 || ld<std::uint8_t>(anim0, 0xC6) != 1) return;                    // 0060F7E6..F6
    String pause;                                                                // [esp+20h] 0060F7FC
    String other;                                                                // [esp+18h] 0060F805
    std::int32_t a2 = 0;                                                         // [esp+14h] 0060F820
    float length = 0.0f;                                                         // [esp+0Ch] 0060F828 (dword 0)
    if (restart != 0) {                                                          // 0060F816..830
        const std::int32_t r = crt_rand_006fc447() % 2;                          // 0060F836..846
        if (r == 0) GuiAssign005e5140(&pause, nullptr, "pause1");                // 0060F856 (0074F3F8)
        else if (r == 1) GuiAssign005e5140(&pause, nullptr, "pause2");           // 0060F84F (0074F400)
        st<Word>(self, 0x394, static_cast<Word>((crt_rand_006fc447() % 0xC9 + 0x64) * 0x64)); // 0060F864..877
        const std::int32_t percent = crt_rand_006fc447() % 0x64;                  // 0060F87D..892 [esp+10h]
        void* const gob = aslots(rp(self, 0x68))->s08_get_model(rp(self, 0x68), nullptr, 0xFF); // 0060F88A..898
        if (gob) {                                                               // 0060F89D..9F
            gslots(gob)->s1c_animation_info(gob, nullptr, WindowCStr005e5670(&pause, nullptr), &a2, &length); // 0060F8A1..B9
            const float start = static_cast<float>(static_cast<double>(percent) * static_cast<double>(length) *
                                                   static_cast<double>(0.00999999977648258209f)); // 0060F8BC..D1 (0073DFEC)
            gslots(gob)->s18_play_animation(gob, nullptr, WindowCStr005e5670(&pause, nullptr), 1.0f, 2, start); // 0060F8D4..E3
        }
        void* const anim = rp(self, 0x68);                                       // 0060F8E6
        void* const gob2 = aslots(anim)->s08_get_model(anim, nullptr, 0xFE);     // 0060F8E9..F2
        if (gob2) {                                                              // 0060F8F7..F9
            const float start = static_cast<float>(static_cast<double>(percent) * static_cast<double>(length) *
                                                   static_cast<double>(0.00999999977648258209f)); // 0060F8FF..914
            gslots(gob2)->s18_play_animation(gob2, nullptr, WindowCStr005e5670(&pause, nullptr), 1.0f, 2, start); // 0060F917..926
        }
        return;                                                                  // 0060F929 -> 0060FAD3 (~other, ~pause)
    }
    const double elapsed = static_cast<double>(delta_seconds) * static_cast<double>(1000.0f); // 0060F92E..931 (0073D6FC)
    const Word accum = rd32(self, 0x390) + static_cast<Word>(x87_ftol(elapsed));  // 0060F937..948
    st<Word>(self, 0x390, accum);                                                // 0060F94E
    if (accum < rd32(self, 0x394)) return;                                       // 0060F942..954 (jb, unsigned)
    const std::int32_t r = crt_rand_006fc447() % 2;                              // 0060F95A..96A
    if (r == 0) GuiAssign005e5140(&pause, nullptr, "pause1");                    // 0060F97A (0074F3F8)
    else if (r == 1) GuiAssign005e5140(&pause, nullptr, "pause2");               // 0060F973 (0074F400)
    switch (crt_rand_006fc447() % 6) {                                           // 0060F988..99A (table 0060FB08)
    case 0: GuiAssign005e5140(&other, nullptr, "greeting"); break;               // 0060F9A1 (0074F3EC)
    case 1: GuiAssign005e5140(&other, nullptr, "hturnl"); break;                 // 0060F9A8 (0074F3E4)
    case 2: GuiAssign005e5140(&other, nullptr, "hturnr"); break;                 // 0060F9AF (0074F3DC)
    case 3: GuiAssign005e5140(&other, nullptr, "pausebrd"); break;               // 0060F9B6 (0074F3D0)
    case 4: GuiAssign005e5140(&other, nullptr, "pausesh"); break;                // 0060F9BD (0074F3C8)
    case 5: GuiAssign005e5140(&other, nullptr, "pause3"); break;                 // 0060F9C4 (0074F3C0)
    default: break;                                                              // 0060F998 (unsigned ja; unreachable)
    }
    void* const gob = aslots(rp(self, 0x68))->s08_get_model(rp(self, 0x68), nullptr, 0xFF); // 0060F9D2..DC
    if (gob) {                                                                   // 0060F9DF..E3
        gslots(gob)->s1c_animation_info(gob, nullptr, WindowCStr005e5670(&other, nullptr), &a2, &length); // 0060F9E5..FD
        gslots(gob)->s128_slot(gob, nullptr, 1);                                 // 0060FA00..06
        gslots(gob)->s18_play_animation(gob, nullptr, WindowCStr005e5670(&other, nullptr), 1.0f, 0x21, 0.0f); // 0060FA0C..23
        const double ms = static_cast<double>(length) * static_cast<double>(1000.0f); // 0060FA26..2A
        st<Word>(self, 0x394, rd32(self, 0x394) + static_cast<Word>(x87_ftol(ms))); // 0060FA30..41
        gslots(gob)->s18_play_animation(gob, nullptr, WindowCStr005e5670(&pause, nullptr), 1.0f, 0x60, 0.0f); // 0060FA47..5A
    }
    void* const gob2 = aslots(rp(self, 0x68))->s08_get_model(rp(self, 0x68), nullptr, 0xFE); // 0060FA5D..67
    if (gob2) {                                                                  // 0060FA6A..6E
        gslots(gob2)->s128_slot(gob2, nullptr, 1);                               // 0060FA70..76
        gslots(gob2)->s18_play_animation(gob2, nullptr, WindowCStr005e5670(&other, nullptr), 1.0f, 0x21, 0.0f); // 0060FA7C..93
        gslots(gob2)->s18_play_animation(gob2, nullptr, WindowCStr005e5670(&pause, nullptr), 1.0f, 0x60, 0.0f); // 0060FA96..AD
    }
    const std::int32_t next = crt_rand_006fc447() % 0xC9;                        // 0060FAB0..BB
    st<Word>(self, 0x390, 0);                                                    // 0060FABD
    st<Word>(self, 0x394, static_cast<Word>((next + 0x64) * 0x64));              // 0060FAC7..CD
}                                                                                // 0060FAD3..ED ~other, ~pause

// =============================================================================== 0060FB20 GetServerCreature()
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* self, void*) {
    if (!kotor_m15a_object_get_server_object_0063d4b0(self, nullptr)) return nullptr;   // 0060FB23..2A, 0060FB3B
    void* const server = kotor_m15a_object_get_server_object_0063d4b0(self, nullptr);   // 0060FB2C..2E
    return static_cast<const ServerSlots*>(rp(server, 0))->s30_as_creature(server, nullptr); // 0060FB33..38 jmp [edx+30h]
}

// =============================================================================== 006100F0 Setup3DScene
void __fastcall kotor_m15a_creature_setup_3d_scene_006100f0(void* self, void*, void* scene_view, void* alignment_source,
                                                            void* server_creature, std::uint32_t play_pause) noexcept(false) {
    void* const stats = alignment_source ? rp(alignment_source, 0x2F8) : rp(self, 0x2F8); // 00610110..126
    std::int32_t alignment = ld<std::int16_t>(stats, 0x80);                      // 0061012C..133 movsx
    const std::uint32_t override_value = kotor_m15a_cell_alignment_override_007a22f8; // 00610136
    if (override_value != 0xFFFFFFFFu) alignment = static_cast<std::int32_t>(override_value); // 0061013B..140
    String anim_name;                                                            // [esp+1Ch] 00610142
    String align_name;                                                           // [esp+24h] 0061014B
    if (alignment == 0) GuiAssign005e5140(&align_name, nullptr, "evil");         // 0061015C..16E (0074F474)
    else if (alignment == 0x64) GuiAssign005e5140(&align_name, nullptr, "good"); // 00610175..183 (0074F46C)
    else kotor_reconstruction::StartupFormat005e5680(&align_name, "align%d", alignment / 5 + 1); // 0061018A..1AC (0074F464)
    const char* pick = nullptr;
    bool droid_or_tag_checked = false;
    if (!server_creature) {                                                      // 006101AF..B4
        if (rp(kotor_menu_window_cells::manager_007a39fc, 8)) {                  // 006101B6..C0 AppManager+8 (server)
            void* const owner = alignment_source ? alignment_source : self;      // 006101C2..C4 / 006101E1
            void* found = nullptr;                                               // 006101FC
            if (kotor_m15a_object_get_server_object_0063d4b0(owner, nullptr)) {  // 006101C6..CF / 006101E3..EA
                void* const server = kotor_m15a_object_get_server_object_0063d4b0(owner, nullptr); // 006101D1..D3
                found = static_cast<const ServerSlots*>(rp(server, 0))->s30_as_creature(server, nullptr); // 006101D8..DC
            }
            server_creature = found;                                             // 006101FE..200 [ebp+10h]
            droid_or_tag_checked = found != nullptr;                             // 00610203
        }
    } else {
        droid_or_tag_checked = true;                                             // 006101B4 jne 00610205
    }
    if (droid_or_tag_checked) {                                                  // 00610205
        String* const tag = static_cast<String*>(at(server_creature, 0x18));      // 00610208
        if (tag->BorrowedEquals005e5290("t3m4") || tag->BorrowedEquals005e5290("hk47")) // 0061020B..229 (0074F45C/0074F41C)
            pick = "pause1";                                                     // 0061022B (0074F3F8)
    }
    if (!pick) {                                                                 // 00610232
        if (alignment < 0x28) pick = "evil";                                     // 00610235..237 (0074F474)
        else if (alignment >= 0x3C) pick = "good";                               // 0061023E..243 (0074F46C)
        else pick = "neutral";                                                   // 0061024A (007485D0)
    }
    GuiAssign005e5140(&anim_name, nullptr, pick);                                // 0061024F..253
    void* const anim = gob_get_property_0x68_0063c070(self, nullptr);            // 00610258..25A
    void* const body = aslots(anim)->s08_get_model(anim, nullptr, 0xFF);         // 0061025F..268
    if (body && play_pause)                                                      // 00610270..276
        gslots(body)->s18_play_animation(body, nullptr, WindowCStr005e5670(&anim_name, nullptr), 1.0f, 2, 0.0f); // 00610278..28F
    void* const anim2 = gob_get_property_0x68_0063c070(self, nullptr);           // 00610296..298
    void* const head = aslots(anim2)->s08_get_model(anim2, nullptr, 0xFE);       // 0061029D..2A6
    if (head && play_pause)                                                      // 006102AB..2B1
        gslots(head)->s18_play_animation(head, nullptr, WindowCStr005e5670(&anim_name, nullptr), 1.0f, 2, 0.0f); // 006102B3..2CA
    void* const model = kotor_m15_gui_scene_get_model_00416750(at(scene_view, 0x5C), nullptr, 0); // 006102CD..2D5
    if (!model) return;                                                          // 006102DA..DE -> 00610450
    float scene_length = 0.0f;  // [esp+10h] written by slot +1C; D2 value-init (no store before the call 006102F9)
    gslots(model)->s1c_animation_info(model, nullptr, WindowCStr005e5670(&align_name, nullptr), nullptr, &scene_length); // 006102E4..2F9
    // 006102FC..320: fcomp [00740CBC] (-1.0f): equal (not found) -> play anim_name, else the alignment animation.
    String* const scene_anim = (static_cast<double>(scene_length) == -1.0) ? &anim_name : &align_name;
    gslots(model)->s18_play_animation(model, nullptr, WindowCStr005e5670(scene_anim, nullptr), 1.0f, 2, 0.0f); // 00610306..328
    String hook;                                                                 // [esp+14h] 0061032B
    if (server_creature) {                                                       // 00610334..33E
        String* const tag = static_cast<String*>(at(server_creature, 0x18));      // 00610340
        if (tag->BorrowedEquals005e5290("t3m4")) GuiAssign005e5140(&hook, nullptr, "camerahookt");        // 00610343..358 (0074F450)
        else if (tag->BorrowedEquals005e5290("hk47")) GuiAssign005e5140(&hook, nullptr, "camerahookh");   // 0061035A..36F (0074F444)
        else if (tag->BorrowedEquals005e5290("zaalbar")) GuiAssign005e5140(&hook, nullptr, "camerahookz"); // 00610371..38A (0074F424/0074F438)
        else GuiAssign005e5140(&hook, nullptr, "camerahook");                    // 0061038C (0074F42C)
    } else {
        GuiAssign005e5140(&hook, nullptr, "camerahook");                         // 00610393 (0074F42C)
    }
    if (gslots(model)->s10c_has_node(model, nullptr, WindowCStr005e5670(&hook, nullptr)) == 0) // 006103A1..3B7
        GuiAssign005e5140(&hook, nullptr, "camerahook");                         // 006103B9..3C2
    void* const camera = rp(scene_view, 0x74);                                   // 006103C7
    static_cast<const CameraSlots*>(rp(camera, 0))->s74_attach(camera, nullptr, model,
        WindowCStr005e5670(&hook, nullptr), 1);                                  // 006103CA..3DB
    void* const camera2 = rp(scene_view, 0x74);                                  // 006103DE..3E1
    static_cast<const CameraSlots*>(rp(camera2, 0))->s44_set_view_angle(camera2, nullptr, 22.7267246246337891f); // 006103E4..3EB (41B5CED9h)
    void* const camera3 = rp(scene_view, 0x74);                                  // 006103EE
    void* const light = static_cast<const CameraSlots*>(rp(camera3, 0))->s80_find_object(camera3, nullptr, 0x3EA); // 006103F1..3F8
    if (light) {                                                                 // 006103FE..402
        st<Word>(light, 0x48, 1);                                                // 00610408
        const float height = cslots(self)->s0d8_get_head_height_offset(self, nullptr); // 0061040F..417 (fstp dword)
        const Vector offset = {0.0f, 0.0f, height};                              // 0061041B..436
        std::memcpy(at(light, 0x3C), &offset, sizeof offset);                    // 0061042B..43F
    }
}                                                                                // 00610442..46A ~hook, ~align_name, ~anim_name

// =============================================================================== 006105B0 LoadRollingFootstepSound()
std::uint32_t __fastcall kotor_m15a_creature_load_rolling_footstep_sound_006105b0(void* self, void*) noexcept(false) {
    String sound_name;                                                           // [esp+14h] 006105CC..D0
    if (void* const old = rp(self, 0x430)) {                                     // 006105D5..E3
        delete_through_slot0(old);                                               // 006105E5..E9 [eax](1)
        st<void*>(self, 0x430, nullptr);                                         // 006105EB
    }
    const std::int32_t row = ld<std::uint16_t>(rp(self, 0x21C), 0x18);          // 006105F1..F7 appearance+18h (row)
    void* const rules = kotor_cell_rules_007a3a28;                               // 006105FB
    std::int32_t footstep_row;                                                   // [esp+0Ch]
    if (!c2da_current_get_int_entry_00413510(rp(rp(rules, 0xB8), 0x0C), nullptr, row,
                                             static_cast<std::int32_t>(kotor_rules_cell32_007a20ec), &footstep_row)) // 00610601..621
        return 0;                                                                // 0061064E..671
    if (!c2da_current_get_string_entry_00413190(rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x58), nullptr, footstep_row,
                                                static_cast<std::int32_t>(kotor_rules_cell32_007a219c), &sound_name)) // 00610623..64C
        return 0;
    void* local10 = c2da_directory_new_006fa7e6(8);                              // 00610673..67F [esp+10h] (8-byte CExoSoundSource)
    void* source = nullptr;                                                      // 006106A8
    if (local10) {                                                               // 00610683..68A
        try {
            alignas(4) unsigned char ref[16];                                    // [esp+1Ch] CResRef
            async_ref_construct_string_00406d60(ref, nullptr, &sound_name);     // 0061068C..695
            source = kotor_sound_rev3::SourceConstruct005d60e0(local10, nullptr, ref); // 0061069A..6A1
        } catch (...) { live_free(local10); throw; }                             // EH state 1 (FrameHandler 00727C73)
    }
    st<void*>(self, 0x430, source);                                              // 006106B2
    kotor_m15_sound_source_set_3d_005d5910(source, nullptr, 1);                  // 006106AA..B8
    // D2: the original stores only the low byte of the group dword [esp+8] (006106D3/00610719/00610720) and pushes
    // the whole dword (00610725..29); the upper three bytes are never written -> value-initialised to 0 here.
    std::uint32_t group = 0;
    if (rd32(self, 0x4) == kotor_m15_client_get_player_creature_id_005ed550(app_internal(), nullptr)) { // 006106BD..D1
        group = 0x13;                                                            // 006106D3
    } else {
        // 006106DA..703: GetINTEntry(appearance row, [007A2124h], &[esp+10h]); its result is ignored and the value
        // read is whatever [esp+10h] holds (the operator-new result above when the lookup fails).
        std::int32_t value = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(local10));
        (void)c2da_current_get_int_entry_00413510(rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x0C), nullptr,
            ld<std::uint16_t>(rp(self, 0x21C), 0x18), static_cast<std::int32_t>(kotor_rules_cell32_007a2124), &value);
        group = (value == 1 || value == 2 || value == 3) ? 0x14u : 0x12u;        // 00610708..720
    }
    kotor_sound_rev3::SourceSetPriorityGroup005d5900(rp(self, 0x430), nullptr, group);  // 00610725..730
    kotor_m15_sound_source_set_looping_005d59d0(rp(self, 0x430), nullptr, 1);    // 00610735..73D
    kotor_m15_sound_source_set_position_005d59e0(rp(self, 0x430), nullptr, at(self, 0x24), 1.5f); // 00610742..751 (3FC00000h)
    (void)kotor_m15b_c2da_get_float_entry_00413350(rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x58), nullptr, footstep_row,
        static_cast<std::int32_t>(kotor_rules_cell32_007a21a0), static_cast<float*>(at(self, 0x434))); // 00610756..779
    const std::uint32_t result = rp(self, 0x430) != nullptr ? 1u : 0u;           // 0061077E..788
    st<Word>(self, 0x438, 0);                                                    // 0061078B
    st<Word>(self, 0x43C, 0);                                                    // 00610791
    return result;                                                               // 00610797..7BC
}

// =============================================================================== 006134C0 SetAppearance
std::uint32_t __fastcall kotor_m15a_creature_set_appearance_006134c0(void* self, void*, void* appearance_block,
                                                                     std::uint32_t parts, std::uint32_t reattach) noexcept(false) {
    cslots(self)->s0e0_detach_visual_effects(self, nullptr);                    // 006134C8..CB
    const std::uint8_t flags = static_cast<std::uint8_t>(parts);                // 006134D1
    const bool body = (flags & 1) != 0;                                          // 006134D5..DA [esp+13h]
    // D2: [esp+18h] (looping) and [esp+14h] (current) are written only when bit0|bit1 is set (006134E4..F8); the
    // original never reads them otherwise (both uses are under the same condition, 00613661..66B).
    Word looping = 0;
    Word current = 0;
    if (body || (flags & 2)) {                                                   // 006134DE..E2
        looping = kotor_m15a_object_get_looping_animation_0063c100(self, nullptr); // 006134E4..ED
        current = kotor_m15a_object_get_current_animation_0063c060(self, nullptr); // 006134EB..F8
    }
    if (body) {                                                                  // 00613502..504
        Vector position, saved_1dc, orientation, saved_1c4, saved_1d0;
        std::memcpy(&position, at(self, 0x24), 12);                              // 0061350A..51D [esp+28h]
        std::memcpy(&saved_1dc, at(self, 0x1DC), 12);                            // 00613521..539 [esp+1Ch]
        std::memcpy(&orientation, at(self, 0x30), 12);                           // 0061353D..556 [esp+34h]
        std::memcpy(&saved_1c4, at(self, 0x1C4), 12);                            // 00613550..576 [esp+40h]
        std::memcpy(&saved_1d0, at(self, 0x1D0), 12);                            // 00613568..593 [esp+4Ch]
        if (!kotor_m15a_appearance_create_body_appearance_00698b10(rp(self, 0x21C), nullptr, appearance_block)) // 0061358C..59E
            return 0;                                                            // 006135A0..A7
        cslots(self)->s08c_set_position(self, nullptr, &position);               // 006135AA..B3
        std::memcpy(at(self, 0x1DC), &saved_1dc, 12);                            // 006135B9..CC
        cslots(self)->s088_set_orientation(self, nullptr, &orientation);         // 006135CF..D6
        const Vector* const n1 = normalize_004ab180(&saved_1dc, &saved_1c4);     // 006135DC..E6
        std::memcpy(at(self, 0x1C4), n1, 12);                                    // 006135EB..603
        const Vector* const n2 = normalize_004ab180(&saved_1dc, &saved_1d0);     // 006135F9..606
        std::memcpy(at(self, 0x1D0), n2, 12);                                    // 0061360B..61F
    }
    if (flags & 2) kotor_m15a_appearance_create_head_appearance_00697a20(rp(self, 0x21C), nullptr, appearance_block); // 00613622..636
    if (flags & 4) kotor_m15a_appearance_create_tail_appearance_00697b00(rp(self, 0x21C), nullptr, appearance_block); // 0061363B..649
    if (flags & 8) kotor_m15a_appearance_create_wing_appearance_00697bc0(rp(self, 0x21C), nullptr, appearance_block); // 0061364E..65C
    if (body || (flags & 2)) {                                                   // 00613661..66B
        kotor_m15a_object_set_looping_animation_0063d830(self, nullptr, looping); // 0061366D..674
        kotor_m15a_object_abort_fire_forget_animation_0063cad0(self, nullptr, 0); // 00613679..67D
        Word play = current;                                                     // 006136AC (edi)
        if (kotor_m15a_object_animation_looping_0063c300(self, nullptr, current) != 1 &&   // 00613682..68D
            kotor_m15a_object_animation_stationary_0063c170(self, nullptr, current) != 0)  // 0061368F..699
            play = looping;                                                      // 0061369B..6A1 (ebx)
        cslots(self)->s09c_set_animation(self, nullptr, play, 0, 0);             // 006136A4 / 006136B5
    }
    kotor_m15a_creature_update_shadow_blob_0060e5b0(self, nullptr);              // 006136BB..BD
    if (reattach) cslots(self)->s0e4_reattach_visual_effects(self, nullptr);     // 006136C2..CE
    (void)kotor_m15a_creature_load_rolling_footstep_sound_006105b0(self, nullptr); // 006136D4..D6
    kotor_m15a_appearance_set_appearance_alignment_00698150(rp(self, 0x21C), nullptr,
        ld<std::uint16_t>(rp(self, 0x2F8), 0x80));                               // 006136DB..6F1
    kotor_m15a_appearance_force_animate_006978a0(rp(self, 0x21C), nullptr);      // 006136F6..6FC
    return 1;                                                                    // 00613704
}

// =============================================================================== 00613710 UpdateWeaponClass()
void __fastcall kotor_m15a_creature_update_weapon_class_00613710(void* self, void*) {
    const Word right = rd32(self, 0x230);                                        // 00613716
    const Word left = rd32(self, 0x234);                                         // 0061371C
    std::uint8_t weapon_class = 0;                                               // 00613729 [esp+0Fh]
    if (right == kInvalidId) {                                                   // 00613724..72D
        if (left == right) {                                                     // 0061372F..731
            weapon_class = 8;                                                    // 00613733
            st<std::uint8_t>(gob_get_property_0x68_0063c070(self, nullptr), 0xC7, 2); // 00613738..73D
        } else if (kotor_m15_client_get_item_by_id_005ed5a0(app_internal(), nullptr, left)) { // 00613749..759
            weapon_class = 0;                                                    // 0061375F
        }
    } else {
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(app_internal(), nullptr, right); // 00613768..772
        if (item) {                                                              // 00613777..779
            void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(rp(kotor_cell_rules_007a3a28, 0x34), nullptr,
                static_cast<std::int32_t>(rd32(item, 0x0C)));                    // 0061377F..78C
            bool set_c7 = true;
            if (left == kInvalidId) {                                            // 00613791..797
                switch (ld<std::uint8_t>(base, 0x8)) {                           // 00613799..7A3 (table 00613838)
                case 1: weapon_class = 1; break;                                 // 006137AA
                case 2: weapon_class = 2; break;                                 // 006137B1
                case 3: weapon_class = 3; break;                                 // 006137B8
                case 4: weapon_class = 5; break;                                 // 006137BF
                case 5: weapon_class = 7; break;                                 // 006137C6
                case 6: weapon_class = 9; break;                                 // 006137CD
                default: break;                                                  // 006137A1 ja
                }
            } else {
                void* const item2 = kotor_m15_client_get_item_by_id_005ed5a0(app_internal(), nullptr, left); // 006137D4..7DD
                if (!item2) { set_c7 = false; }                                  // 006137E2..E4
                else {
                    base = kotor_m15b_base_item_array_get_base_item_005b31a0(rp(kotor_cell_rules_007a3a28, 0x34), nullptr,
                        static_cast<std::int32_t>(rd32(item2, 0x0C)));           // 006137E6..7F3
                    const std::uint8_t wield = ld<std::uint8_t>(base, 0x8);      // 006137F8
                    if (wield == 2) weapon_class = 4;                            // 006137FC..80D
                    else if (wield == 4) weapon_class = 6;                       // 00613801..806
                }
            }
            if (set_c7)
                st<std::uint8_t>(gob_get_property_0x68_0063c070(self, nullptr), 0xC7, ld<std::uint8_t>(base, 0x9)); // 00613812..81C
        }
    }
    st<std::uint8_t>(gob_get_property_0x68_0063c070(self, nullptr), 0xC5, weapon_class); // 00613822..82F
}

// =============================================================================== 00616A20 CSWCCreature()
void* __fastcall kotor_m15a_creature_construct_00616a20(void* self, void*) noexcept(false) {
    kotor_m15a_object_construct_0063e530(self, nullptr, 5);                      // 00616A39..4A (byte [00746634h] = 05)
    st<const void*>(self, 0x0, &kotor_m15e_creature_methods_0074f5f8);          // 00616A51 vtable 0074F5F8 (L15-E)
    for (unsigned off = 0x1C4; off <= 0x1E4; off += 4) st<Word>(self, off, 0);   // 00616A57..7B
    st<Word>(self, 0x200, 0); st<Word>(self, 0x204, 0); st<Word>(self, 0x208, 0); // 00616A97..A3
    kotor_fastcall_00405ed0(at(self, 0x2DD), nullptr);                           // 00616A8D..AE CResRef() at +2DDh
    st<Word>(self, 0x364, 0); st<Word>(self, 0x368, 0); st<Word>(self, 0x360, 0); // 00616AAE..BA
    st<Word>(self, 0x440, (rd32(self, 0x440) & 0xFFFFFFFEu) | 2u);              // 00616AC0..D6 (bitfield RMW of the new block)
    st<Word>(self, 0x3A4, 0);                                                    // 00616ADC
    st<Word>(self, 0x36C, 0);                                                    // 00616AE2
    st<std::uint16_t>(self, 0x2FC, 0xFFFFu);                                     // 00616AE8
    void* const stats_memory = c2da_directory_new_006fa7e6(0x134);               // 00616ACC..F1
    void* stats = nullptr;                                                       // 00616B0F
    if (stats_memory) {                                                          // 00616AFD..B04
        try { stats = kotor_m15b_levelup_stats_construct_0064b2d0(stats_memory, nullptr); } // 00616B06..08
        catch (...) { live_free(stats_memory); throw; }                          // EH state 2 (FrameHandler 00727E47)
    }
    st<void*>(self, 0x2F8, stats);                                               // 00616B22
    st<Word>(self, 0x1BC, 0); st<Word>(self, 0x1C0, 0);                          // 00616B28..2E
    st<std::uint16_t>(self, 0x1EC, 0);                                           // 00616B34
    st<std::uint16_t>(self, 0x48, 0x2710);                                       // 00616B3B
    st<Word>(self, 0x2C0, 0); st<Word>(self, 0x14C, 0); st<Word>(self, 0x150, 0); // 00616B3F..4B
    st<Word>(self, 0x1B8, 0); st<Word>(self, 0x3D8, 0);                          // 00616B51..57
    st<Word>(self, 0x268, kInvalidId); st<Word>(self, 0x26C, kInvalidId);        // 00616B5D..63
    st<Word>(self, 0x1E8, 0);                                                    // 00616B69
    void* const attack1 = c2da_directory_new_006fa7e6(0x48);                     // 00616B1B..6F
    st<void*>(self, 0x2B8, attack1 ? kotor_m15a_combat_attack_data_construct_0060b630(attack1, nullptr) : nullptr); // 00616B77..88
    void* const attack2 = c2da_directory_new_006fa7e6(0x48);                     // 00616B86..8E
    st<void*>(self, 0x2BC, attack2 ? kotor_m15a_combat_attack_data_construct_0060b630(attack2, nullptr) : nullptr); // 00616B96..A5
    for (unsigned i = 0; i < 0x12; ++i) {                                        // 00616BAB..CF
        st<Word>(self, 0x220 + 4 * i, kInvalidId);                               // 00616BC0..C6 [eax-50h] = [00746644h]
        st<Word>(self, 0x270 + 4 * i, 0);                                        // 00616BC9
    }
    std::uint32_t time_a = 0, time_b = 0;                                        // [esp+10h] / [esp+14h] (out params)
    void* const timer = kotor_input_required::get_world_timer_005ed4f0(app_internal(), nullptr); // 00616BD1..E3
    frame_GetWorldTime_004ade40(timer, nullptr, &time_a, &time_b);               // 00616BD9..EA (&[esp+10h], &[esp+14h])
    st<Word>(self, 0x3C, time_a);                                                // 00616BF9
    st<Word>(self, 0x40, time_b);                                                // 00616BFC
    void* const appearance_memory = c2da_directory_new_006fa7e6(0x6C);           // 00616BF7..FF
    void* appearance = nullptr;                                                  // 00616C1E
    if (appearance_memory) {                                                     // 00616C0B..12
        try { appearance = kotor_m15a_appearance_construct_00696430(appearance_memory, nullptr, self); } // 00616C14..17
        catch (...) { live_free(appearance_memory); throw; }                     // EH state 3
    }
    st<void*>(self, 0x21C, appearance);                                          // 00616C27
    if (!appearance) return self;                                                // 00616C20..2D -> 00616F83
    st<float>(self, 0x30, 1.0f); st<float>(self, 0x34, 0.0f); st<float>(self, 0x38, 0.0f);    // 00616C33..5F
    st<float>(self, 0x1C4, 1.0f); st<float>(self, 0x1C8, 0.0f);                  // 00616C62..80
    st<float>(self, 0x1D0, 1.0f); st<float>(self, 0x1CC, 0.0f);                  // 00616C8E..A4
    st<float>(self, 0x1D4, 0.0f);                                                // 00616CC7
    st<std::uint8_t>(self, 0x1EE, 0); st<std::uint8_t>(self, 0x1EF, 0); st<std::uint8_t>(self, 0x1F0, 0); // 00616CCF..DB
    st<Word>(self, 0x1F4, 1);                                                    // 00616CE1
    st<Word>(self, 0x214, 0);                                                    // 00616CE7
    st<float>(self, 0x1D8, 0.0f);                                                // 00616CED
    st<std::uint16_t>(self, 0x58, 0x2710);                                       // 00616CF3
    st<Word>(self, 0x2CC, 0); st<Word>(self, 0x17C, 0); st<Word>(self, 0x180, 0); // 00616CF7..D03
    st<std::uint16_t>(self, 0x184, 0);                                           // 00616D09
    st<Word>(self, 0x188, 0); st<Word>(self, 0x18C, 1);                          // 00616D10..16
    void* const visual = c2da_directory_new_006fa7e6(0x30);                      // 00616CCD..D1C
    st<void*>(self, 0x2F0, visual ? kotor_m15a_spell_cast_visual_construct_00695ce0(visual, nullptr) : nullptr); // 00616D24..3C
    st<Word>(self, 0x2F4, kInvalidId);                                           // 00616D42..47
    st<std::uint16_t>(self, 0x2D0, 0xFFFFu);                                     // 00616D4D
    st<Word>(self, 0x138, 0); st<Word>(self, 0x15C, 0);                          // 00616D56..5C
    for (unsigned off = 0x190; off <= 0x1B0; off += 4) st<Word>(self, off, 0);   // 00616D62..92
    for (unsigned off = 0xFC; off < 0x12C; off += 4) st<Word>(self, off, 0);     // 00616D98..A5 rep stos 0Ch dwords
    st<Word>(self, 0x12C, 0); st<Word>(self, 0x130, 0); st<Word>(self, 0x134, 0); // 00616DA7..B3
    st<Word>(self, 0x148, 0); st<Word>(self, 0x300, 0); st<Word>(self, 0x304, 0); // 00616DB9..C5
    for (unsigned off = 0x160; off <= 0x178; off += 4) st<Word>(self, off, 0);   // 00616DCB..F1
    st<std::uint8_t>(self, 0x2DC, 2);                                            // 00616DF7
    st<Word>(self, 0x1B4, 0);                                                    // 00616DFE
    st<Word>(self, 0x308, 0x30D40);                                              // 00616E04
    st<Word>(self, 0x30C, 0); st<Word>(self, 0x144, 0);                          // 00616E0E..14
    st<Word>(self, 0x2C8, kInvalidId);                                           // 00616E1A..20
    st<Word>(self, 0x158, 0x7D0); st<Word>(self, 0x154, 0x1770);                 // 00616E26..30
    st<Word>(self, 0x358, 0); st<Word>(self, 0x35C, 0);                          // 00616E3A..40
    st<Word>(self, 0x310, 0); st<Word>(self, 0x314, 0); st<Word>(self, 0x318, 0); // 00616E46..52
    st<Word>(self, 0x370, 0); st<Word>(self, 0x374, 0); st<Word>(self, 0x378, 0); // 00616E58..64
    for (unsigned off = 0x37C; off <= 0x388; off += 4) st<Word>(self, off, kInvalidId); // 00616E6A..92
    st<Word>(self, 0x390, 0); st<Word>(self, 0x398, 0);                          // 00616E98..9E
    st<Word>(self, 0x394, static_cast<Word>((crt_rand_006fc447() % 4 + 2) * 0x7530)); // 00616EA4..BE (signed % 4)
    st<float>(self, 0x1FC, 1.0f);                                                // 00616EC4
    st<Word>(self, 0x39C, kInvalidId);                                           // 00616ECE..D4
    st<Word>(self, 0x3A0, 0);                                                    // 00616EDA
    for (unsigned off = 0x3A8; off <= 0x3B8; off += 4) st<Word>(self, off, 0);   // 00616EE0..F2
    st<Word>(self, 0x3BC, 0x2772);                                               // 00616EF8
    st<Word>(self, 0x3E8, 0); st<Word>(self, 0x13C, 0); st<Word>(self, 0x140, 0); // 00616F08..14
    st<Word>(self, 0x3C8, kInvalidId); st<Word>(self, 0x3CC, 0);                 // 00616F1A..20
    st<Word>(self, 0x3C0, 0xFFFFFFFFu); st<Word>(self, 0x3C4, 0xFFFFFFFFu);      // 00616F26..2F
    st<Word>(self, 0x3D0, 0); st<Word>(self, 0x3DC, kInvalidId);                 // 00616F35..3B
    st<std::uint8_t>(self, 0x3E0, 0); st<Word>(self, 0x3E4, kInvalidId);         // 00616F41..47
    for (unsigned off = 0x3EC; off < 0x430; off += 4) st<Word>(self, off, 0);    // 00616F4D..5A rep stos 11h dwords
    for (unsigned off = 0x430; off <= 0x43C; off += 4) st<Word>(self, off, 0);   // 00616F5C..6E
    st<std::uint16_t>(self, 0x444, 0xFFFFu);                                     // 00616F74
    st<Word>(self, 0x448, 0);                                                    // 00616F7D
    return self;
}
}
