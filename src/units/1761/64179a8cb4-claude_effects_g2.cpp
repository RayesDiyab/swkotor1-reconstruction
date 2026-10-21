// Lane effects-g2 (main subset): CSWSEffectListHandler callbacks 004DAA40..004DCFC0 (visual effect, beam, force
// resistance, regenerate, link, haste/slow internal, light, num attacks, taunt, limit movement speed, skill,
// special walk animation, disguise remove, timestop, see invisible, miss chance), reconstructed from the original
// swkotor.exe (GOG, read-only). Siblings: claude_effects_g2_feedback.cpp, claude_effects_g2_aoe.cpp.
//  * OnApplyX: thiscall(CSWSObject* object, CGameEffect* effect, int load_game), RET 0Ch (verified for every one)
//      -> extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect, unsigned load_game) @20
//  * OnRemoveX: thiscall(CSWSObject* object, CGameEffect* effect), RET 8 (verified for every one)
//      -> extern "C" unsigned __fastcall kotor_fastcall_<addr>(void* self, void*, void* object, void* effect) @16
//  * vtable slots: +10h AsSWSObject (CGameObject), +30h AsSWSCreature, +94h (creature) GetDead/IsPC-class query
//    used only as a boolean by OnRemoveDisguise.
//  * Missing callees reconstructed here (absent as C-linkage bodies from the c27 map), see "shared callees" section:
//      004CBC00 CSWSObject::CalculateSpellRangedMissTarget, 004CF390 AddLoopingVisualEffect,
//      004CD240 RemoveLoopingVisualEffect, 00569C40 CSWSMessage::SendServerToPlayerGameObjUpdateVisEffect,
//      005A5A60 CSWSCreatureStats::GetForceResistance, 005AFAC0 SetForceResistance, 005AFAE0 SetForceResistancePenalty,
//      004EE150 CSWSCreature::UnDisguise, 004ED850 CSWSCreature::UpdateAppearanceDependantInfo (a port-only body
//      exists in 0886 as a file-local adapter; no callable symbol), 004AE9B0/004AE9C0 CServerExoApp::Add/
//      RemoveFromExclusionList and 004B4730/004B4800 CServerExoAppInternal::Add/RemoveFromExclusionList.
//    CGameEffect::GetObjectID 005036E0 is used as an inline view (file-local, no export).
//  * Memory: live_operator_new / live_free only. No original address is dereferenced.
//  * x87: double intermediates where the original keeps values on the FPU (53-bit PC), float where it stores a dword;
//    fpatan / fsin / fcos executed by the FPU itself with the dword store done by the FPU (no double rounding).
//  * EH frames of the original only guard operator-new'd effects during their constructors and the CExoString
//    locals; none of the bound constructors throw, so the normal-path order is reproduced.
#include "842d6b1618-w5_t3.hpp"
#include <cstdint>
#include <cstring>

namespace kotor_reconstruction {
struct StartupString { void* data; w5::U length; int __thiscall BorrowedEquals005e5290(const char* text); }; // 005E5290 [0077]
}
// kotor_menu20::ConstructString005b3190 (CExoString() [menu20]) is declared by w5_action_queue.hpp
namespace w2 { namespace g07 { unsigned __cdecl is_beam_type_004ca960(unsigned type); } }                   // CLoopingVisualEffect::GetIsBeam [w2_g07_object]

extern "C" {
using w5::U; using w5::I; using w5::U16; using w5::U8; using w5::Vector;
// ---- CGameEffect ----
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                // GetInteger [w1_native_b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);    // SetInteger [0886]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id); // SetObjectID [1194u]
void* __fastcall kotor_fastcall_00503f70(void* effect, void*, void* source);                 // CGameEffect(CGameEffect*) [claude_w1_route_bodies]
void __fastcall kotor_fastcall_00503750(void* effect, void*, U day, U time);                 // SetExpiryTime [claude_0886_bodies_party]
void __fastcall kotor_fastcall_00504240(void* effect, void*);                                // UpdateLinked [claude_combat_attack_damage]
void __fastcall kotor_fastcall_00504810(void* effect, void*, void* a, void* b);              // SetLinked [claude_combat_attack_damage]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b); // CSWSObject::ApplyEffect [0886]
// ---- server / timers ----
U __fastcall kotor_fastcall_004aea40(void* server, void*);                                   // GetPlayerCreatureId [0595]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                               // CSWSPlayer::GetGameObject [0595]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                 // GetSWSMessage [creature_destructor_r2]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                               // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);            // GetWorldTime [0457]
U __fastcall kotor_fastcall_004ae1b0(void* timer, void*, float seconds);                     // GetCalendarDayFromSeconds [claude_0886_bodies_party]
U __fastcall kotor_fastcall_004ae200(void* timer, void*, float seconds);                     // GetTimeOfDayFromSeconds [claude_0886_bodies_party]
U __fastcall kotor_fastcall_004adf50(void* timer, void*, U d1, U t1, U d2, U t2, U* out_day, U* out_time); // AddWorldTimes [w4-creature_anim]
void* __fastcall recovered_active_timer_004ae830(void* server, void*, U id);                 // GetActiveTimer [w1_native_b7]
U __fastcall recovered_server_active_pause_state_004ae990(void* server, void*);              // GetActivePauseState (AL) [w1_native_b7]
void __fastcall kotor_fastcall_004ae960(void* server, void*, U flags);                       // TogglePauseState [0595]
// ---- linked list (CExoLinkedListInternal) ----
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);        // GetAtPos [0168]
// unresolved_list_next_005e9b90 (GetNext [0168]) is declared by w5_action_queue.hpp
void* __fastcall resource_append_005e9dc0(void* list, void*, void* data);                    // AddTail [0264]
void __fastcall c2da_005e9c10(void* list, void*, void* position);                            // Remove [0191]
// ---- messages ----
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);             // CreateWriteMessage [w2_message_native]
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                         // WriteOBJECTIDServer [w2_message_native]
void __fastcall kotor_fastcall_004d6300(void* message, void*, U value, U bits);              // WriteWORD [w2_message_native]
void __fastcall kotor_fastcall_004d62c0(void* message, void*, U value, U bits);              // WriteBYTE [w2_message_native]
void __fastcall kotor_fastcall_004d6920(void* message, void*, U value_bits, U scale_bits, U bits); // WriteFLOAT [w2_message_native]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);            // GetWriteMessage [w2_message_native]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
// ---- 2DA / strings ----
I __fastcall kotor_m15_c2da_get_string_entry_00413de0(void* table, void*, void* row_label, void* column, void* out); // [0811]
// c2da_current_get_float_00413430 (w5_t2.hpp) / c2da_current_get_int_00413660 (w5_t3.hpp): GetFLOAT/GetINTEntry(col name) [0376]
I __fastcall c2da_current_get_int_entry_00413510(void* table, void*, I row, I column, I* out);   // GetINTEntry(col index) [0390]
I __fastcall c2da_current_get_string_entry_00413190(void* table, void*, I row, I column, void* out); // GetCExoStringEntry(col index) [0390]
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I value);                        // CExoString(int) [0266]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);              // CExoString(const char*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* s, void*);                                // ~CExoString [0459]
void* __fastcall async_assign_string_005e5c50(void* s, void*, void* other);                  // CExoString::operator= [0199]
extern unsigned kotor_rules_cell32_007a20d4;                                                 // .data 007A20D4 appearance column [0431]
extern unsigned kotor_rules_cell32_007a2110;                                                 // .data 007A2110 appearance column [0431]
extern unsigned kotor_rules_cell32_007a2124;                                                 // .data 007A2124 appearance column [0431]
// ---- creature / stats ----
void __fastcall recovered_stats_refresh_005addc0(void* stats, void*);                        // UpdateCombatInformation [1046]
U __fastcall recovered_stats_effect_query_005a6960(void* stats, void*, U kind, void* other); // GetEffectImmunity [1094]
void __fastcall kotor_fastcall_004ed6e0(void* creature, void*);                              // UpdatePersonalSpace [claude_p2s_a_objupdate]
void __fastcall w2x_movement_rate_005a5680(void* stats, void*, U rate);                      // SetMovementRate [0886]
U __fastcall w1x_area_safe_location_004be860(void* area, void*, U position, U radius_bits, U a, U b, U out); // ComputeSafeLocation [0886]
// ---- vectors / CRT ----
Vector* __cdecl float_times_vector_00437a60(Vector* out, float scale, const Vector* v);      // operator*(float, Vector) [0555]
int __cdecl crt_rand_006fc447();                                                             // _rand [0479]
void __cdecl unresolved_ftol2_006fae8c();                                                    // __ftol2 (ST0 -> EDX:EAX) [0064]

// ---- exports: handlers (ai_resolve) ----
unsigned __fastcall kotor_fastcall_004daa40(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dadc0(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004db090(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004db180(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004db210(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004db2c0(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004db8a0(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004db8f0(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dba30(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dba50(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dbcc0(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dbce0(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dbd90(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dbe00(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dc3b0(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dc500(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dc530(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dc580(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dc5d0(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dca80(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dcac0(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dcb40(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dcc90(void* self, void*, void* object, void* effect);
unsigned __fastcall kotor_fastcall_004dcd00(void* self, void*, void* object, void* effect, unsigned load_game);
unsigned __fastcall kotor_fastcall_004dcfc0(void* self, void*, void* object, void* effect, unsigned load_game);
// ---- exports: reconstructed shared callees ----
Vector* __fastcall kotor_fastcall_004cbc00(void* object, void*, Vector* out, U creator_id, U target_id);  // RET 0Ch
void __fastcall kotor_fastcall_004cf390(void* object, void*, U visual, U source_id, U flag);               // RET 0Ch
void __fastcall kotor_fastcall_004cd240(void* object, void*, U visual);                                   // RET 4
U __fastcall kotor_fastcall_00569c40(void* message, void*, void* player, U visual, U object_id, U source_id,
    U id1, U byte6, U byte7, U target_x, U target_y, U target_z, U value11);                           // RET 2Ch
I __fastcall kotor_fastcall_005a5a60(void* stats, void*);                                               // RET 0
void __fastcall kotor_fastcall_005afac0(void* stats, void*, U value);                                    // RET 4
void __fastcall kotor_fastcall_005afae0(void* stats, void*, U value);                                    // RET 4
U __fastcall kotor_fastcall_004ee150(void* creature, void*, void* effect);                               // RET 4
void __fastcall kotor_fastcall_004ed850(void* creature, void*, U update_movement);                       // RET 4
void __fastcall kotor_fastcall_004ae9b0(void* server, void*, U id, U flags);                             // RET 8 (thunk)
void __fastcall kotor_fastcall_004ae9c0(void* server, void*, U id, U flags);                             // RET 8 (thunk)
void __fastcall kotor_fastcall_004b4730(void* internal, void*, U id, U flags);                           // RET 8
void __fastcall kotor_fastcall_004b4800(void* internal, void*, U id, U flags);                           // RET 8
}

namespace {
using namespace w5;
struct ExoString { char* text; U length; };
static_assert(sizeof(ExoString) == 8, "CExoString is 8 bytes");

constexpr U16 k_duration_instant_007473b8 = 0;      // .rdata 007473B8 (word)
constexpr U16 k_duration_temporary_007473c0 = 1;    // .rdata 007473C0 (word)
constexpr U8 k_object_type_5_00746634 = 5;          // .rdata 00746634 (byte)
constexpr U8 k_object_type_9_00746638 = 9;          // .rdata 00746638 (byte)
constexpr U8 k_object_type_a_00746639 = 0x0a;       // .rdata 00746639 (byte)
constexpr float k_one_0073d764 = 1.0f;              // .rdata 0073D764
constexpr float k_250_00747440 = 250.0f;            // .rdata 00747440
constexpr float k_two_00741be0 = 2.0f;              // .rdata 00741BE0
constexpr float k_tenth_0073e798 = 0.1f;            // .rdata 0073E798
constexpr float k_milli_0073d708 = 0.001f;          // .rdata 0073D708
constexpr float k_hundred_00741838 = 100.0f;        // .rdata 00741838
constexpr U k_two_pi_bits_00740c88 = 0x40c90fdbu;   // .rdata 00740C88 (6.2831855f)
constexpr float k_zero_0073d700 = 0.0f;             // .rdata 0073D700
constexpr float k_25_00745e5c = 25.0f;              // .rdata 00745E5C
constexpr float k_thousand_0073d6fc = 1000.0f;      // .rdata 0073D6FC
constexpr float k_ten_007455c8 = 10.0f;             // .rdata 007455C8
constexpr U k_float_one_bits = 0x3f800000u;         // push 3F800000h (WriteFLOAT scale 1.0)
constexpr U k_twenty_bits = 0x41a00000u;            // push 41A00000h (20.0f)
const char k_type_fd_007469c0[] = "Type_FD";        // .rdata 007469C0
const char k_f_0073ece0[] = "F";                    // .rdata 0073ECE0
const char k_creperspace_00745e60[] = "creperspace";// .rdata 00745E60
const char k_haslegs_00746cd4[] = "haslegs";        // .rdata 00746CD4
const char k_hasarms_00746ccc[] = "hasarms";        // .rdata 00746CCC
const char k_weaponscale_00746cc0[] = "weaponscale";// .rdata 00746CC0
const char k_prefatckdist_00746cb0[] = "PREFATCKDIST"; // .rdata 00746CB0

inline void* as_creature(void* object) { return vcall<void*>(object, 0x30); }          // AsSWSCreature
inline void* as_object(void* object) { return vcall<void*>(object, 0x10); }            // AsSWSObject
inline void* stats_of(void* creature) { return at<void*>(creature, 0xa74); }
inline void* rules() { return kotor_cell_rules_007a3a28; }
inline U8 rules_byte_aa() { return at<U8>(rules(), 0xaa); }
inline void* two_da(U offset) { return at<void*>(at<void*>(rules(), 0xb8), offset); }   // [[[7A3A28]+B8]+offset]
inline U effect_int(void* e, U i) { return recovered_effect_integer_00503690(e, nullptr, i); }
inline void effect_set_int(void* e, U i, U v) { recovered_effect_integer_005036a0(e, nullptr, i, v); }
// inline view of 005036E0 CGameEffect::GetObjectID: mov eax,[ecx+eax*4+78h]; ret 4
inline U effect_object_id(void* e, U i) { return at<U>(e, 0x78 + i * 4); }
inline void apply(void* object, void* e, U load_game) { recovered_effect_apply_004d14f0(object, nullptr, e, load_game, 0); }
inline void* new_effect_copy(void* source) {
    void* memory = live_operator_new(0x8c);
    return memory ? kotor_fastcall_00503f70(memory, nullptr, source) : nullptr;   // null -> later stores fault (original)
}
inline void exo_empty(ExoString* s) { kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr); }
inline void exo_text(ExoString* s, const char* t) { frame_LogStringCtor_005e5a90(s, nullptr, t); }
inline void exo_destroy(ExoString* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
inline U exo_equals(ExoString* s, const char* t) {
    return static_cast<U>(reinterpret_cast<kotor_reconstruction::StartupString*>(s)->BorrowedEquals005e5290(t));
}
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* ptr_of(U a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
inline Vector* vpos(void* object) { return reinterpret_cast<Vector*>(static_cast<unsigned char*>(object) + 0x90); }
inline U ftol2(double v) { U r; __asm { fld qword ptr v } __asm { call unresolved_ftol2_006fae8c } __asm { mov r, eax } return r; }
// fld y; fld1; fpatan; fstp dword  (atan2(y, 1.0) stored by the FPU as a float)
inline float x87_atan_unit_to_float(double y) {
    float r; __asm { fld qword ptr y } __asm { fld1 } __asm { fpatan } __asm { fstp dword ptr r } return r;
}
// fld st(0); fcos; fstp dword c; fsin; fstp dword s
inline void x87_cos_sin_to_float(double a, float* c, float* s) {
    float cv, sv;
    __asm { fld qword ptr a } __asm { fld st(0) } __asm { fcos } __asm { fstp dword ptr cv } __asm { fsin } __asm { fstp dword ptr sv }
    *c = cv; *s = sv;
}
}  // namespace

// ==================================================================================================================
// 004DAA40 CSWSEffectListHandler::OnApplyVisualEffect(object, effect, load_game), RET 0Ch
// ==================================================================================================================
extern "C" unsigned __fastcall kotor_fastcall_004daa40(void*, void*, void* object, void* effect, unsigned) {
    if (object == nullptr || effect == nullptr) return 1;                              // 004DAA61..71 -> 004DAD59
    U flag_1c = 0;                                                                     // 004DAA7E [esp+1C]
    Vector miss{0.0f, 0.0f, 0.0f};                                                     // 004DAA86..96 [esp+50..58]
    U object_id = at<U>(object, 4);                                                    // 004DAA77/9E [esp+20]
    U source_id;                                                                       // [esp+10]
    if (effect_object_id(effect, 0) == k_object_invalid)                              // 004DAA7A..AC
        source_id = at<U>(effect, 0x18);                                               // 004DAABD creator
    else
        source_id = effect_object_id(effect, 0);                                       // 004DAAAE..B7
    const U id1 = effect_object_id(effect, 1);                                         // 004DAAC4..D1 [esp+30]
    const U visual = effect_int(effect, 0);                                            // 004DAACD..DE [esp+18]
    const U8 byte1 = static_cast<U8>(effect_int(effect, 1));                           // 004DAADA..EB [esp+24] (byte)
    U8 mode = 1;                                                                       // 004DAAEF [esp+14] (byte)
    const U miss_flag = effect_int(effect, 2);                                         // 004DAAE7..FF [esp+28]
    const U value_c = at<U>(effect, 0xc);                                              // 004DAAFC/B03 [esp+2C]
    if (miss_flag == 1) {                                                              // 004DAAF9..B07
        Vector scratch;                                                                // [esp+5C]
        const U target_id = at<U>(object, 4);
        const U creator_id = at<U>(effect, 0x18);
        object_id = k_object_invalid;                                                  // 004DAB18
        const Vector* r = kotor_fastcall_004cbc00(object, nullptr, &scratch, creator_id, target_id); // 004DAB09..20
        const double z = static_cast<double>(at<float>(object, 0x98)) + static_cast<double>(k_one_0073d764); // 004DAB25..2D
        std::memcpy(&miss, r, sizeof(Vector));                                         // 004DAB2B..45
        mode = 2;                                                                      // 004DAB49
        if (z == static_cast<double>(miss.z)) mode = 1;                                // 004DAB3D..55 (test ah,44h; jp)
    }
    void* visual_2da = two_da(0x88);                                                   // 004DAB5A..66
    ExoString value;  exo_empty(&value);                                               // 004DAB6C..70 [esp+34]
    ExoString row;    kotor_fastcall_005e5bc0(&row, nullptr, static_cast<I>(visual));  // 004DAB75..86 [esp+3C]
    ExoString column; exo_text(&column, k_type_fd_007469c0);                           // 004DAB8B..99 [esp+44]
    const I found = kotor_m15_c2da_get_string_entry_00413de0(visual_2da, nullptr, &row, &column, &value); // 004DAB9E..B7
    const bool ok = found == 1;                                                        // 004DABBC..C7
    exo_destroy(&column);                                                              // 004DABC3..CE
    U fire_and_forget = flag_1c;                                                       // 004DABEE
    if (ok && exo_equals(&value, k_f_0073ece0) != 0) fire_and_forget = 1;              // 004DABD3..EC
    const U16 duration = static_cast<U16>(at<U8>(effect, 0xa) & 7u);                   // 004DABF2..F7
    if (duration != k_duration_instant_007473b8 && miss_flag != 1 && fire_and_forget != 1) { // 004DABFA..0D
        kotor_fastcall_004cf390(object, nullptr, visual, source_id, byte1);            // 004DAD72..83
        exo_destroy(&row);                                                             // 004DAD88..91
        exo_destroy(&value);                                                           // 004DAD96..A2
        return 0;                                                                      // 004DADAD
    }
    const float range = static_cast<float>(static_cast<double>(k_250_00747440) + static_cast<double>(k_two_00741be0)); // 004DAC13..31
    Vector position;  copy_vector(&position, vpos(object));                            // 004DAC19..3B [esp+5C]
    if (recovered_object_area_004cb120(object, nullptr) != nullptr) {                  // 004DAC35..46
        void* server = server_app();                                                   // 004DAC4C..55
        bool proceed = true;
        if (w2::g07::is_beam_type_004ca960(visual) != 0) {                             // 004DAC58..63
            if (kotor_fastcall_004ae750(server, nullptr, source_id) == nullptr) proceed = false; // 004DAC65..73
        }
        if (proceed) {
            void* player = kotor_fastcall_004ae910(server_app(), nullptr,
                                                   kotor_fastcall_004aea40(server_app(), nullptr)); // 004DAC79..96
            if (player != nullptr) {                                                   // 004DAC98
                void* game_obj = kotor_fastcall_00561080(player, nullptr);             // 004DACA0..A2
                Vector d;                                                              // [esp+44]
                kotor_cdecl_00423bc0(&d, vpos(game_obj), &position);                   // 004DACA7..B7
                double sum = static_cast<double>(d.z) * static_cast<double>(d.z);       // 004DACBC..C0
                sum = sum + static_cast<double>(d.y) * static_cast<double>(d.y);         // 004DACC7..CF
                sum = sum + static_cast<double>(d.x) * static_cast<double>(d.x);         // 004DACD1..D9
                const double rr = static_cast<double>(range) * static_cast<double>(range); // 004DACDB..DF
                if (rr >= sum) {                                                       // 004DACE3..EA (C0 -> skip, NaN skips)
                    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr); // 004DACEC..F5
                    (void)kotor_fastcall_00569c40(message, nullptr, player, visual, object_id, source_id, id1,
                                                  byte1, mode, bits(miss.x), bits(miss.y), bits(miss.z), value_c); // 004DACFA..35
                }
            }
        }
    }
    exo_destroy(&row);                                                                 // 004DAD3A..43
    exo_destroy(&value);                                                               // 004DAD48..54
    return 1;                                                                          // 004DAD59
}

// 004DADC0 OnRemoveVisualEffect(object, effect), RET 8: RemoveLoopingVisualEffect(GetInteger(0)); return 1
extern "C" unsigned __fastcall kotor_fastcall_004dadc0(void*, void*, void* object, void* effect) {
    kotor_fastcall_004cd240(object, nullptr, effect_int(effect, 0));                   // 004DADC0..D0 (object not checked)
    return 1;                                                                          // 004DADD5
}

// 004DB090 OnApplyBeam, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004db090(void*, void*, void* object, void* effect, unsigned load_game) {
    const U8 type = at<U8>(object, 8);                                                 // 004DB0AC..B0
    if (type == k_object_type_5_00746634 || type == k_object_type_9_00746638 || type == k_object_type_a_00746639) { // ..C7
        void* copy = new_effect_copy(effect);                                          // 004DB0CD..FA
        at<U16>(copy, 8) = 0x1e;                                                       // 004DB108
        recovered_effect_set_object_id_005036f0(copy, nullptr, 0, effect_object_id(effect, 0)); // 004DB0FC..18
        effect_set_int(copy, 0, effect_int(effect, 0));                                // 004DB11D..2B
        effect_set_int(copy, 1, effect_int(effect, 1));                                // 004DB130..3E
        effect_set_int(copy, 2, effect_int(effect, 2));                                // 004DB143..51
        apply(object, copy, load_game);                                                // 004DB156..60
    }
    return 0;                                                                          // 004DB16B
}

// 004DB180 OnApplyForceResistanceIncrease, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004db180(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                                      // 004DB183..90
    void* stats = stats_of(as_creature(object));                                       // 004DB192..9D
    I amount = static_cast<I>(effect_int(effect, 0));                                  // 004DB199..AA
    if (amount > 0x80) amount = 0x80;                                                  // 004DB1AC..B4
    else if (amount < 0) return 1;                                                     // 004DB1F3..FF
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x344) = 1;         // 004DB1B9..CB
    if (amount > static_cast<I>(static_cast<signed char>(kotor_fastcall_005a5a60(stats, nullptr)))) // 004DB1D5..E1
        kotor_fastcall_005afac0(stats, nullptr, static_cast<U>(amount));               // 004DB1E3..E6
    return 0;                                                                          // 004DB1ED
}

// 004DB210 OnApplyForceResistanceDecrease, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004db210(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) == nullptr) return 0;                                      // 004DB216..20
    void* stats = stats_of(as_creature(object));                                       // 004DB226..2D
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18)); // 004DB233..44
    if (recovered_stats_effect_query_005a6960(stats, nullptr, 0x1a, creator) != 0) return 1; // 004DB249..55
    I amount = static_cast<I>(effect_int(effect, 0));                                  // 004DB257..5F (index = EAX 0)
    if (amount < 0) return 1;                                                          // 004DB261..63
    if (at<U>(object, 0xf8) != 0) return 1;                                            // 004DB265..6D
    if (amount > 0x80) amount = 0x80;                                                  // 004DB26F..77
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x344) = 1;         // 004DB27C..8E
    if (static_cast<I>(at<signed char>(stats, 0x120)) <= amount)                       // 004DB298..A1 (jg skips)
        kotor_fastcall_005afae0(stats, nullptr, static_cast<U>(amount));               // 004DB2A3..A6
    return 0;                                                                          // 004DB2AD
}

// 004DB2C0 OnApplyRegenerate, RET 0Ch: stamp the world time into integers 2 (day) and 3 (time)
extern "C" unsigned __fastcall kotor_fastcall_004db2c0(void*, void*, void* object, void* effect, unsigned) {
    if (as_creature(object) != nullptr) {                                              // 004DB2C1..CC
        U day = 0, time = 0;                                                           // [esp+0Ch] (object arg slot reused) / [esp+4]
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 004DB2CE..E9
        effect_set_int(effect, 2, day);                                                // 004DB2EE..FB
        effect_set_int(effect, 3, time);                                               // 004DB300..09
    }
    return 0;                                                                          // 004DB30F
}

// 004DB8A0 OnApplyLink, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004db8a0(void*, void*, void* object, void* effect, unsigned load_game) {
    kotor_fastcall_00504240(effect, nullptr);                                          // 004DB8A2..A9 UpdateLinked
    if (void* left = at<void*>(effect, 0x28)) apply(object, left, load_game);         // 004DB8AE..C3
    if (void* right = at<void*>(effect, 0x2c)) apply(object, right, load_game);       // 004DB8C8..D5
    kotor_fastcall_00504810(effect, nullptr, nullptr, nullptr);                        // 004DB8DA..E0 SetLinked(0, 0)
    return 1;                                                                          // 004DB8E7
}

// 004DB8F0 OnApplyHasteInternal, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004db8f0(void*, void*, void* object, void* effect, unsigned load_game) {
    void* creature = as_creature(object);                                              // 004DB909..14
    if (creature != nullptr) {                                                         // 004DB916
        void* e = new_effect_copy(effect);                                             // 004DB91E..4B
        at<U16>(e, 8) = 0x1c;                                                          // 004DB95E
        effect_set_int(e, 0, 0x96);                                                    // 004DB94D..64
        apply(object, e, load_game);                                                   // 004DB969..73
        e = new_effect_copy(effect);                                                   // 004DB978..A5
        effect_set_int(e, 0, 0);                                                       // 004DB9A7..B5
        effect_set_int(e, 1, 4);                                                       // 004DB9BA..C0
        effect_set_int(e, 2, rules_byte_aa());                                         // 004DB9C5..D6
        effect_set_int(e, 5, 0x4007);                                                  // 004DB9DB..E4
        at<U16>(e, 8) = 0x30;                                                          // 004DB9EF
        apply(object, e, load_game);                                                   // 004DB9E9..F5
        at<U>(creature, 0x8d0) = 0;                                                    // 004DB9FA
        at<U>(creature, 0x8cc) = 1;                                                    // 004DBA04
    }
    return 0;                                                                          // 004DBA15
}

// 004DBA30 OnRemoveHasteInternal, RET 8
extern "C" unsigned __fastcall kotor_fastcall_004dba30(void*, void*, void* object, void*) {
    if (void* creature = as_creature(object)) at<U>(creature, 0x8cc) = 0;             // 004DBA30..3D
    return 1;                                                                          // 004DBA47
}

// 004DBA50 OnApplySlowInternal, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dba50(void*, void*, void* object, void* effect, unsigned load_game) {
    void* creature = as_creature(object);                                              // 004DBA67..74 (kept in object slot)
    if (creature != nullptr) {
        void* e = new_effect_copy(effect);                                             // 004DBA81..AE
        at<U16>(e, 8) = 0x1d;                                                          // 004DBABD
        effect_set_int(e, 0, 0x32);                                                    // 004DBAB0..C3
        apply(object, e, load_game);                                                   // 004DBAC8..D2
        e = new_effect_copy(effect);                                                   // 004DBAD7..04
        effect_set_int(e, 0, 0);                                                       // 004DBB06..10
        effect_set_int(e, 1, 2);                                                       // 004DBB15..1B
        effect_set_int(e, 2, rules_byte_aa());                                         // 004DBB20..31
        effect_set_int(e, 5, 0x4007);                                                  // 004DBB36..3F
        at<U16>(e, 8) = 0x31;                                                          // 004DBB4A
        apply(object, e, load_game);                                                   // 004DBB44..50
        e = new_effect_copy(effect);                                                   // 004DBB55..82
        at<U16>(e, 8) = 0x0b;                                                          // 004DBB8E
        effect_set_int(e, 0, 2);                                                       // 004DBB84..94
        effect_set_int(e, 1, 0);                                                       // 004DBB99..9F
        effect_set_int(e, 2, rules_byte_aa());                                         // 004DBBA4..B5
        apply(object, e, load_game);                                                   // 004DBBBA..C0
        e = new_effect_copy(effect);                                                   // 004DBBC5..F2
        effect_set_int(e, 0, 2);                                                       // 004DBBF4..FE
        effect_set_int(e, 1, 2);                                                       // 004DBC03..09
        effect_set_int(e, 3, rules_byte_aa());                                         // 004DBC0E..1F
        at<U16>(e, 8) = 0x1b;                                                          // 004DBC2A
        apply(object, e, load_game);                                                   // 004DBC24..30
        e = new_effect_copy(effect);                                                   // 004DBC35..62
        at<U16>(e, 8) = 0x3b;                                                          // 004DBC6E
        effect_set_int(e, 0, 0);                                                       // 004DBC64..74
        apply(object, e, load_game);                                                   // 004DBC79..7F
        at<U>(creature, 0x8cc) = 0;                                                    // 004DBC84..8A
        at<U>(creature, 0x8d0) = 1;                                                    // 004DBC94
    }
    return 0;                                                                          // 004DBCA3
}

// 004DBCC0 OnRemoveSlowInternal, RET 8
extern "C" unsigned __fastcall kotor_fastcall_004dbcc0(void*, void*, void* object, void*) {
    if (void* creature = as_creature(object)) at<U>(creature, 0x8d0) = 0;             // 004DBCC0..CD
    return 1;                                                                          // 004DBCD7
}

// 004DBCE0 OnApplyLight, RET 0Ch: re-applies a visual-effect (1Eh) copy carrying integer 0 to the creature
extern "C" unsigned __fastcall kotor_fastcall_004dbce0(void*, void*, void* object, void* effect, unsigned load_game) {
    if (as_creature(object) != nullptr) {                                              // 004DBCF6..05
        void* e = new_effect_copy(effect);                                             // 004DBD07..34
        at<U16>(e, 8) = 0x1e;                                                          // 004DBD42
        effect_set_int(e, 0, effect_int(effect, 0));                                   // 004DBD36..52
        apply(as_creature(object), e, load_game);                                      // 004DBD57..68 (this = AsSWSCreature)
    }
    return 0;                                                                          // 004DBD73
}

// 004DBD90 OnApplyModifyNumAttacks, RET 0Ch: combat-round extra attacks += n, clamped to [0, 2]
extern "C" unsigned __fastcall kotor_fastcall_004dbd90(void*, void*, void* object, void* effect, unsigned) {
    void* creature = as_creature(object);                                              // 004DBD90..98
    if (creature == nullptr) return 0;                                                 // 004DBD9B..9D
    void* round = at<void*>(creature, 0x9c8);                                          // 004DBD9F
    const I current = at<I>(round, 0x9a4);                                             // 004DBDA5
    if (current > 2) return 1;                                                         // 004DBDAB..B7
    I value = static_cast<I>(effect_int(effect, 0)) + current;                         // 004DBDBA..C5
    at<I>(round, 0x9a4) = value;                                                       // 004DBDCA
    if (value > 2) value = 2;                                                          // 004DBDC7..D2
    at<I>(round, 0x9a4) = value;                                                       // 004DBDDE
    value &= (value < 0) ? 0 : -1;                                                     // 004DBDD7..E5 (setl; dec; and)
    at<I>(round, 0x9a4) = value;                                                       // 004DBDE7
    return 0;                                                                          // 004DBDEE
}

// 004DBE00 OnRemoveModifyNumAttacks, RET 8
extern "C" unsigned __fastcall kotor_fastcall_004dbe00(void*, void*, void* object, void* effect) {
    void* creature = as_creature(object);                                              // 004DBE00..06
    if (creature != nullptr) {                                                         // 004DBE09..0B
        void* round = at<void*>(creature, 0x9c8);                                      // 004DBE12
        const I current = at<I>(round, 0x9a4);                                         // 004DBE19
        I value = current - static_cast<I>(effect_int(effect, 0));                     // 004DBE1F..28
        at<I>(round, 0x9a4) = value;                                                   // 004DBE2D
        if (value > 2) value = 2;                                                      // 004DBE2A..35
        at<I>(round, 0x9a4) = value;                                                   // 004DBE41
        value &= (value < 0) ? 0 : -1;                                                 // 004DBE3A..49
        at<I>(round, 0x9a4) = value;                                                   // 004DBE4B
    }
    return 1;                                                                          // 004DBE52
}

// 004DC3B0 OnApplyTaunt, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dc3b0(void*, void*, void* object, void* effect, unsigned load_game) {
    if (as_creature(object) == nullptr) return 1;                                      // 004DC3C9..D6
    void* creature = as_creature(object);                                              // 004DC3DC..E0
    if (at<U>(creature, 0xf8) != 0) return 1;                                          // 004DC3E3..EB
    at<U>(creature, 0x8e4) = 1;                                                        // 004DC3F6
    void* e = new_effect_copy(effect);                                                 // 004DC3F1..28
    at<U16>(e, 8) = 0x31;                                                              // 004DC438
    effect_set_int(e, 0, 0);                                                           // 004DC42A..3E
    effect_set_int(e, 1, effect_int(effect, 0));                                       // 004DC443..51
    effect_set_int(e, 2, rules_byte_aa());                                             // 004DC456..67
    effect_set_int(e, 5, 0x4007);                                                      // 004DC46C..75
    apply(object, e, load_game);                                                       // 004DC47A..84
    e = new_effect_copy(effect);                                                       // 004DC489..B2
    at<U16>(e, 8) = 0x19;                                                              // 004DC4C2
    effect_set_int(e, 0, 0x1e);                                                        // 004DC4B4..C8
    apply(object, e, load_game);                                                       // 004DC4CD..D3
    return 0;                                                                          // 004DC4D8
}

// 004DC500 OnRemoveTaunt, RET 8
extern "C" unsigned __fastcall kotor_fastcall_004dc500(void*, void*, void* object, void*) {
    if (as_creature(object) != nullptr) at<U>(as_creature(object), 0x8e4) = 0;         // 004DC501..17
    return 1;                                                                          // 004DC521
}

// 004DC530 OnApplyLimitMovementSpeed, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dc530(void*, void*, void* object, void*, unsigned) {
    if (as_creature(object) != nullptr) {                                              // 004DC531..3E
        if (at<U>(as_creature(object), 0xf8) != 0) return 1;                           // 004DC540..57
        at<U>(as_creature(object), 0x8e8) = 1;                                         // 004DC55A..61
    }
    return 0;                                                                          // 004DC56B
}

// 004DC580 OnApplySkillIncrease, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dc580(void*, void*, void* object, void* effect, unsigned) {
    const I amount = static_cast<I>(effect_int(effect, 0));                            // 004DC586..8A
    (void)effect_int(effect, 1);                                                       // 004DC58F..93 (unused reads)
    (void)effect_int(effect, 2);                                                       // 004DC598..9E
    (void)effect_int(effect, 3);                                                       // 004DC5A3..A7
    if (amount < 0) return 1;                                                          // 004DC5AC..B7
    return as_creature(object) != nullptr ? 0u : 1u;                                   // 004DC5BA..C8 (neg; sbb; inc)
}

// 004DC5D0 OnApplySkillDecrease, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dc5d0(void*, void*, void* object, void* effect, unsigned) {
    const I amount = static_cast<I>(effect_int(effect, 0));                            // 004DC5D6..DA
    (void)effect_int(effect, 1);                                                       // 004DC5DF..E3
    (void)effect_int(effect, 2);                                                       // 004DC5E8..EE
    (void)effect_int(effect, 3);                                                       // 004DC5F3..F7
    if (amount < 0) return 1;                                                          // 004DC5FC..FE
    if (at<U>(object, 0xf8) != 0) return 1;                                            // 004DC600..0C
    if (as_creature(object) == nullptr) return 1;                                      // 004DC60E..17
    void* creator = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18)); // 004DC619..26
    void* stats = stats_of(as_creature(object));                                       // 004DC62B..34
    return recovered_stats_effect_query_005a6960(stats, nullptr, 0x1b, creator) != 0 ? 1u : 0u; // 004DC63A..49
}

// 004DCA80 OnApplySpecialWalkAnimation, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dca80(void*, void*, void* object, void* effect, unsigned) {
    const U8 animation = static_cast<U8>(effect_int(effect, 0));                       // 004DCA80..93
    if (object == nullptr || as_creature(object) == nullptr) return 1;                 // 004DCA8D..A0
    at<U8>(as_creature(object), 0xa10) = animation;                                    // 004DCAA2..A9
    return 0;                                                                          // 004DCAB0
}

// 004DCAC0 OnRemoveDisguise, RET 8
extern "C" unsigned __fastcall kotor_fastcall_004dcac0(void*, void*, void* object, void* effect) {
    void* creature = as_creature(object);                                              // 004DCAC0..CA
    if (creature == nullptr) return 1;                                                 // 004DCACC..CE
    if (vcall<U>(creature, 0x94) != 0 && at<U>(creature, 0x9d4) == 0 && at<U>(creature, 0xec) != 0) // 004DCAD0..F0
        return 1;
    if (effect_int(effect, 1) == 0) {                                                  // 004DCAF3..02
        effect_set_int(effect, 1, 1);                                                  // 004DCB04..0A
        const U16 subtype = static_cast<U16>((at<U16>(effect, 0xa) & 0xfff8u) | k_duration_temporary_007473c0); // 004DCB0F..1A
        at<U>(effect, 0xc) = 0;                                                        // 004DCB21
        at<U16>(effect, 0xa) = subtype;                                                // 004DCB28
    }
    (void)kotor_fastcall_004ee150(creature, nullptr, effect);                          // 004DCB2C..2F
    return 1;                                                                          // 004DCB35
}

// 004DCB40 OnApplyTimestop, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dcb40(void*, void*, void* object, void* effect, unsigned) {
    if (static_cast<U8>(recovered_server_active_pause_state_004ae990(server_app(), nullptr)) == 0) { // 004DCB40..55
        kotor_fastcall_004ae9b0(server_app(), nullptr, at<U>(object, 4), 1);           // 004DCB57..69
        kotor_fastcall_004ae960(server_app(), nullptr, 1);                             // 004DCB6E..79
        return 0;                                                                      // 004DCB80
    }
    if (static_cast<U8>(recovered_server_active_pause_state_004ae990(server_app(), nullptr)) != 1) return 0; // 004DCB89..99
    const float duration = at<float>(effect, 0xc);                                     // 004DCB9F..AE (dword passed as float)
    const U add_day = kotor_fastcall_004ae1b0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, duration); // ..B9
    const U add_time = kotor_fastcall_004ae200(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, at<float>(effect, 0xc)); // ..DA
    U day = 0, time = 0;                                                               // [esp+10] / [esp+0C]
    frame_GetWorldTime_004ade40(recovered_active_timer_004ae830(server_app(), nullptr, at<U>(object, 4)), nullptr,
                                &day, &time);                                          // 004DCBDF..03
    U expiry_day = 0, expiry_time = 0;                                                 // [esp+18] / [esp+14]
    (void)kotor_fastcall_004adf50(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, day, time, add_day, add_time,
                                  &expiry_day, &expiry_time);                          // 004DCC08..2D
    const I count = at<I>(object, 0x128);                                              // 004DCC32
    if (count <= 0) return 0;                                                          // 004DCC3A..3C
    void** effects = at<void**>(object, 0x124);                                        // 004DCC42
    for (I i = 0; i < count; ++i) {                                                    // 004DCC50..5E
        if (at<U16>(effects[i], 8) == 0x40) {                                          // 004DCC52..56
            kotor_fastcall_00503750(effects[i], nullptr, expiry_day, expiry_time);     // 004DCC6B..78
            return 1;                                                                  // 004DCC7F
        }
    }
    return 0;                                                                          // 004DCC62
}

// 004DCC90 OnRemoveTimestop, RET 8
extern "C" unsigned __fastcall kotor_fastcall_004dcc90(void*, void*, void* object, void*) {
    kotor_fastcall_004ae960(server_app(), nullptr, 1);                                 // 004DCC90..9A
    kotor_fastcall_004ae9c0(server_app(), nullptr, at<U>(object, 4), 1);               // 004DCC9F..B1
    return 1;                                                                          // 004DCCB6
}

// 004DCD00 OnApplySeeInvisible, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dcd00(void*, void*, void* object, void*, unsigned) {
    if (as_creature(object) != nullptr) at<U8>(as_creature(object), 0x8ec) |= 1;      // 004DCD01..17
    return 0;                                                                          // 004DCD1E
}

// 004DCFC0 OnApplyMissChance, RET 0Ch
extern "C" unsigned __fastcall kotor_fastcall_004dcfc0(void*, void*, void*, void* effect, unsigned) {
    if (static_cast<I>(effect_int(effect, 0)) > 0x64) return 1;                        // 004DCFC5..D1
    if (static_cast<I>(effect_int(effect, 0)) < 1) return 1;                           // 004DCFD3..DF
    return 0;                                                                          // 004DCFE1
}

// ==================================================================================================================
// Shared callees (absent from the c27 map as callable bodies). Coordinator: dedupe against other effect groups.
// ==================================================================================================================

// 004CBC00 CSWSObject::CalculateSpellRangedMissTarget(Vector* out, creator_id, target_id), RET 0Ch, EAX = out.
// Quirk (original): every computation lands in a local that is never returned; the copied-out vector is the
// separate zero-initialised local [esp+34h], so the result is always (0,0,0). The rand() draws, the 2DA read and the
// vector helper calls are still performed exactly as in the original.
extern "C" Vector* __fastcall kotor_fastcall_004cbc00(void*, void*, Vector* out, U creator_id, U target_id) {
    float personal = 0.5f;                                                             // 004CBC23 [esp+8] = 3F000000h
    Vector v1{0.0f, 0.0f, 0.0f};                                                       // 004CBC2B..3B [esp+18]
    Vector result{0.0f, 0.0f, 0.0f};                                                   // 004CBC47..57 [esp+34]
    void* creator_object = kotor_fastcall_004ae750(server_app(), nullptr, creator_id); // 004CBC18..5F
    if (creator_object != nullptr && as_object(creator_object) != nullptr) {           // 004CBC64..77
        void* creator = as_object(creator_object);                                     // 004CBC7D..88
        void* target_object = kotor_fastcall_004ae750(server_app(), nullptr, target_id); // 004CBC84..98
        if (target_object != nullptr && as_object(target_object) != nullptr) {         // 004CBC9A..AB
            void* target = as_object(target_object);                                   // 004CBCB1..B9
            if (as_creature(target) != nullptr) {                                      // 004CBCBB..C4
                ExoString column; exo_text(&column, k_creperspace_00745e60);           // 004CBCC6..CF
                void* creature = as_creature(target);                                  // 004CBCD4..E0
                const U16 appearance = at<U16>(at<void*>(creature, 0xa74), 0x186);     // 004CBCE3..E9
                (void)c2da_current_get_float_00413430(two_da(0xc), nullptr, appearance, &column, &personal); // ..0D
                exo_destroy(&column);                                                  // 004CBD12..1E
                personal = static_cast<float>(static_cast<double>(personal) - static_cast<double>(k_tenth_0073e798)); // ..2D
            }
            const I k = crt_rand_006fc447() % 2000 + 1;                                // 004CBD31..4A
            const float scale = static_cast<float>(static_cast<double>(k) * static_cast<double>(k_milli_0073d708)); // ..59
            const Vector* cp = vpos(creator);                                          // 004CBD3E ebx
            const Vector* tp = vpos(target);                                           // 004CBD44 esi
            const double dx = static_cast<double>(cp->x) - static_cast<double>(tp->x);  // 004CBD5D..5F
            const double dy = static_cast<double>(cp->y) - static_cast<double>(tp->y);  // 004CBD61..64
            const double dz = static_cast<double>(cp->z) - static_cast<double>(tp->z);  // 004CBD67..6A
            double sum = dz * dz + dy * dy;                                            // 004CBD6D..75
            sum = sum + dx * dx;                                                       // 004CBD77..7B
            if (sum > static_cast<double>(k_hundred_00741838)) {                       // 004CBD7D..8E (C3|C0 -> far branch)
                Vector diff; kotor_cdecl_00423bc0(&diff, cp, tp);                       // 004CBD94..9B creator - target
                Vector n; const Vector* np = normalize_004ab180(&n, &diff);            // 004CBDA0..A6
                Vector nc; copy_vector(&nc, np);                                       // 004CBDAB..C6 [esp+3C..44]
                const float angle = x87_atan_unit_to_float(static_cast<double>(nc.y) / static_cast<double>(nc.x)); // ..CE
                double t;
                if (crt_rand_006fc447() % 2 != 0) {                                    // 004CBDD2..E3
                    const I r = crt_rand_006fc447() % 0x82e;                           // 004CBDE5..F2
                    t = static_cast<double>(angle) - static_cast<double>(r) * static_cast<double>(k_milli_0073d708); // ..00
                } else {
                    const I r = crt_rand_006fc447() % 0x82e;                           // 004CBE06..13
                    t = static_cast<double>(r) * static_cast<double>(k_milli_0073d708) + static_cast<double>(angle); // ..21
                }
                const double two_pi = static_cast<double>(flt(k_two_pi_bits_00740c88));
                if (t > two_pi) t = t - two_pi;                                        // 004CBE25..32
                if (t < static_cast<double>(k_zero_0073d700)) t = t + two_pi;          // 004CBE38..45
                x87_cos_sin_to_float(t, &v1.x, &v1.y);                                 // 004CBE4B..55
                if (static_cast<double>(nc.x) < static_cast<double>(k_zero_0073d700)) { // 004CBE59..68
                    v1.x = -v1.x;                                                      // 004CBE6A..7C
                    v1.y = -v1.y;                                                      // 004CBE78..90
                    v1.z = -v1.z;                                                      // 004CBE86..9C
                }
                vector_scale_assign_004a97a0(&v1, nullptr, scale);                     // 004CBEA0..A9
            } else {
                Vector diff; kotor_cdecl_00423bc0(&diff, tp, cp);                       // 004CBEB3..BA target - creator
                copy_vector(&v1, &diff);                                               // 004CBEBF..D2
                Vector w{0.0f, 0.0f, 0.0f};                                            // 004CBED6..E6 [esp+28]
                Vector n;
                if (crt_rand_006fc447() % 2 != 0) {                                    // 004CBEEE..FF
                    w.x = normalize_004ab180(&n, &v1)->y;                              // 004CBF01..1D
                    w.y = -normalize_004ab180(&n, &v1)->x;                             // 004CBF21..2A
                } else {
                    w.x = -normalize_004ab180(&n, &v1)->y;                             // 004CBF30..49
                    w.y = normalize_004ab180(&n, &v1)->x;                              // 004CBF4D..59
                }
                Vector scaled; vector_add_assign_004a9740(&w, nullptr, float_times_vector_00437a60(&scaled, personal, &w)); // ..7C
                Vector span; kotor_cdecl_00423bc0(&span, tp, cp);                       // 004CBF81..88
                double s = static_cast<double>(span.z) * static_cast<double>(span.z) +
                           static_cast<double>(span.y) * static_cast<double>(span.y);  // 004CBF8D..A0
                s = s + static_cast<double>(span.x) * static_cast<double>(span.x);     // 004CBFA2..AA
                if (s > static_cast<double>(k_25_00745e5c)) {                          // 004CBFAC..B7
                    const I rnd = crt_rand_006fc447();                                 // 004CBFBE
                    const I divisor = static_cast<I>(ftol2(static_cast<double>(scale) * static_cast<double>(k_thousand_0073d6fc))); // ..D2
                    const I m = rnd % divisor;                                         // 004CBFD7..DC (idiv)
                    const float jitter = static_cast<float>(static_cast<double>(m) * static_cast<double>(k_milli_0073d708)); // ..F7
                    Vector extra; vector_add_assign_004a9740(&w, nullptr, float_times_vector_00437a60(&extra, jitter, &w)); // ..08
                }
                vector_add_assign_004a9740(&v1, nullptr, &w);                          // 004CC00D..16
                const I r10 = crt_rand_006fc447() % 10;                                // 004CC01B..26
                const float length = static_cast<float>(static_cast<double>(r10) + static_cast<double>(k_ten_007455c8)); // ..3F
                Vector unit; const Vector* up = normalize_004ab180(&unit, &v1);       // 004CC042..44
                Vector stretched; const Vector* sp = vector_component_mult_00423bf0(&stretched, up, length); // ..52
                copy_vector(&v1, sp);                                                  // 004CC057..67
                v1.z = static_cast<float>(static_cast<double>(at<float>(target, 0x98)) + static_cast<double>(k_one_0073d764)); // ..7A
            }
            vector_add_assign_004a9740(&v1, nullptr, tp);                              // 004CC07E..83
        }
    }
    copy_vector(out, &result);                                                         // 004CC089..9C
    return out;                                                                        // EAX = out
}

// 004CF390 CSWSObject::AddLoopingVisualEffect(WORD visual, DWORD source, BYTE flag), RET 0Ch
extern "C" void __fastcall kotor_fastcall_004cf390(void* object, void*, U visual, U source_id, U flag) {
    void* entry = live_operator_new(0xc);                                              // 004CF392..96 (no null check)
    at<U16>(entry, 0) = static_cast<U16>(visual);                                      // 004CF3B0
    at<U>(entry, 4) = source_id;                                                       // 004CF3B3
    at<U8>(entry, 8) = static_cast<U8>(flag);                                          // 004CF3B6
    void* list = static_cast<unsigned char*>(object) + 0x130;                          // 004CF3AA CExoArrayList
    if (at<U>(list, 4) == at<U>(list, 8)) {                                            // 004CF3B9..C4
        const U capacity = at<U>(list, 8);
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            capacity != 0 ? capacity * 2 : 0x10);      // 004CF3C6..D6
    }
    at<void**>(list, 0)[at<U>(list, 4)] = entry;                                       // 004CF3DB..E0
    at<U>(list, 4) = at<U>(list, 4) + 1;                                               // 004CF3E3..E8
}

// 004CD240 CSWSObject::RemoveLoopingVisualEffect(WORD visual), RET 4: removes the last entry with that visual id
extern "C" void __fastcall kotor_fastcall_004cd240(void* object, void*, U visual) {
    I index = at<I>(object, 0x134) - 1;                                                // 004CD244..4A
    if (index < 0) return;                                                             // 004CD24B
    void** entries = at<void**>(object, 0x130);                                        // 004CD24D (cached)
    const U16 id = static_cast<U16>(visual);                                           // 004CD253
    while (at<U16>(entries[index], 0) != id) {                                         // 004CD260..65
        --index;                                                                       // 004CD267..6D
        if (index < 0) return;
    }
    live_free(entries[index]);                                                         // 004CD275..79
    at<void**>(object, 0x130)[index] = nullptr;                                        // 004CD27E..84
    const I count = at<I>(object, 0x134) - 1;                                          // 004CD28B..94
    at<I>(object, 0x134) = count;                                                      // 004CD29B
    if (index >= count) return;                                                        // 004CD297..A1
    I i = index;
    do {
        void** list = at<void**>(object, 0x130);                                       // 004CD2A3
        list[i] = list[i + 1];                                                         // 004CD2A9..AF
        ++i;                                                                           // 004CD2B7
    } while (i < at<I>(object, 0x134));                                                // 004CD2B1..BA
}

// 00569C40 CSWSMessage::SendServerToPlayerGameObjUpdateVisEffect(player, visual, object_id, source_id, id1, BYTE b6,
//   BYTE b7, Vector target (by value), DWORD value11), RET 2Ch
extern "C" U __fastcall kotor_fastcall_00569c40(void* message, void*, void* player, U visual, U object_id, U source_id,
    U id1, U byte6, U byte7, U target_x, U target_y, U target_z, U value11) {
    void* source_go = kotor_fastcall_004ae750(server_app(), nullptr, source_id);       // 00569C43..5A ebp
    void* target_go = kotor_fastcall_004ae750(server_app(), nullptr, object_id);       // 00569C5F..6E
    void* source = nullptr;                                                            // ebx
    void* target = nullptr;                                                            // edi
    if (source_go != nullptr) source = as_object(source_go);                           // 00569C73..83
    if (target_go != nullptr) target = as_object(target_go);                           // 00569C89..94
    kotor_fastcall_004d68a0(message, nullptr, 6, 0xffffffffu, 1);                      // 00569C96..9E
    kotor_fastcall_00578570(message, nullptr, object_id);                              // 00569CA3..AA
    kotor_fastcall_004d6300(message, nullptr, visual, 0x10);                           // 00569CAF..B8
    if (target == nullptr) {                                                           // 00569CBD..C6
        kotor_fastcall_004d6920(message, nullptr, target_x, k_float_one_bits, 0x20);   // 00569CC8..CF
        kotor_fastcall_004d6920(message, nullptr, target_y, k_float_one_bits, 0x20);   // 00569CD4..E2
        kotor_fastcall_004d6920(message, nullptr, target_z, k_float_one_bits, 0x20);   // 00569CE7..20
    } else {
        kotor_fastcall_004d6920(message, nullptr, at<U>(target, 0x90), k_float_one_bits, 0x20); // 00569CED..F6
        kotor_fastcall_004d6920(message, nullptr, at<U>(target, 0x94), k_float_one_bits, 0x20); // 00569CFB..0B
        kotor_fastcall_004d6920(message, nullptr, at<U>(target, 0x98), k_float_one_bits, 0x20); // 00569D10..20
    }
    if (w2::g07::is_beam_type_004ca960(visual) != 0) {                                 // 00569D25..34
        kotor_fastcall_00578570(message, nullptr, source_id);                          // 00569D3A..41
        kotor_fastcall_00578570(message, nullptr, id1);                                // 00569D46..4D
        kotor_fastcall_004d62c0(message, nullptr, byte6, 8);                           // 00569D52..5B
        kotor_fastcall_004d62c0(message, nullptr, byte7, 8);                           // 00569D60..69
        kotor_fastcall_004d6920(message, nullptr, at<U>(source, 0x90), k_float_one_bits, 0x20); // 00569D6E..7E (no null check)
        kotor_fastcall_004d6920(message, nullptr, at<U>(source, 0x94), k_float_one_bits, 0x20); // 00569D83..93
        kotor_fastcall_004d6920(message, nullptr, at<U>(source, 0x98), k_float_one_bits, 0x20); // 00569D98..A8
        if (object_id == k_object_invalid)                                             // 00569DAD..B3
            kotor_fastcall_004d6920(message, nullptr, value11, k_float_one_bits, 0x20);// 00569DB5..C3
    }
    void* data = nullptr; U size = 0;                                                  // [esp+18] / [esp+14]
    const U ok = kotor_fastcall_004d6440(message, nullptr, &data, &size);              // 00569DC8..D4
    if (ok == 0) return ok;                                                            // 00569DD9..E4
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 5, 3, data, size); // 00569DE7..FF
}

// 005A5A60 CSWSCreatureStats::GetForceResistance(), RET 0: max(0, (signed char)(base - penalty))
extern "C" I __fastcall kotor_fastcall_005a5a60(void* stats, void*) {
    const signed char value = static_cast<signed char>(static_cast<U8>(at<U8>(stats, 0x11f) - at<U8>(stats, 0x120))); // ..66
    if (value < 0) return 0;                                                           // 005A5A6C..70 (jns)
    return static_cast<I>(value);                                                      // 005A5A71 movsx
}

// 005AFAC0 CSWSCreatureStats::SetForceResistance(BYTE), RET 4
extern "C" void __fastcall kotor_fastcall_005afac0(void* stats, void*, U value) {
    at<U8>(stats, 0x11f) = static_cast<U8>(value);                                     // 005AFAC0..C4
    recovered_stats_refresh_005addc0(stats, nullptr);                                  // 005AFACA UpdateCombatInformation
}

// 005AFAE0 CSWSCreatureStats::SetForceResistancePenalty(BYTE), RET 4
extern "C" void __fastcall kotor_fastcall_005afae0(void* stats, void*, U value) {
    at<U8>(stats, 0x120) = static_cast<U8>(value);                                     // 005AFAE0..E4
    recovered_stats_refresh_005addc0(stats, nullptr);                                  // 005AFAEA
}

// 004EE150 CSWSCreature::UnDisguise(CGameEffect*), RET 4, EAX = 1 (the effect argument is unused)
extern "C" U __fastcall kotor_fastcall_004ee150(void* creature, void*, void*) {
    if (at<U>(creature, 0xa48) == 0) return 1;                                         // 004EE156..5E
    at<U16>(stats_of(creature), 0x186) = at<U16>(creature, 0xa4c);                     // 004EE164..71
    at<U16>(creature, 0xa60) = at<U16>(creature, 0xa4c);                               // 004EE178..81
    kotor_fastcall_004ed6e0(creature, nullptr);                                        // 004EE188 UpdatePersonalSpace
    kotor_fastcall_004ed850(creature, nullptr, 1);                                     // 004EE18D..91
    at<U>(creature, 0xa48) = 0;                                                        // 004EE198
    void* area = recovered_object_area_004cb120(creature, nullptr);                    // 004EE196..A2
    if (area != nullptr) {                                                             // 004EE1A7..A9
        Vector safe{0.0f, 0.0f, 0.0f};                                                 // 004EE1C7..D7 [esp+4]
        if (w1x_area_safe_location_004be860(area, nullptr, word(vpos(creature)), k_twenty_bits,
                                            at<U>(creature, 0x340), 1, word(&safe)) != 0) // 004EE1AB..E6
            recovered_object_position_004cd1a0(creature, nullptr, &safe, 1, 1, 0);     // 004EE1E8..F5
    }
    return 1;                                                                          // 004EE1FA
}

// 004ED850 CSWSCreature::UpdateAppearanceDependantInfo(int update_movement_rate), RET 4
extern "C" void __fastcall kotor_fastcall_004ed850(void* creature, void*, U update_movement) {
    I value_i;                                                                         // [esp+8] (not re-initialised: original)
    float value_f;                                                                     // [esp+0C]
    I value_10 = 0, value_14 = 0;                                                      // [esp+10] / [esp+14]
    void* appearance = two_da(0xc);
    ExoString column;
    exo_text(&column, k_haslegs_00746cd4);                                             // 004ED872..7B
    (void)c2da_current_get_int_00413660(two_da(0xc), nullptr, at<U16>(creature, 0xa60), &column, &value_i); // ..AA
    exo_destroy(&column);                                                              // 004ED8AF..BB
    at<I>(creature, 0x4f0) = value_i;                                                  // 004ED8C0..CD
    exo_text(&column, k_hasarms_00746ccc);                                             // 004ED8C4..D3
    (void)c2da_current_get_int_00413660(two_da(0xc), nullptr, at<U16>(creature, 0xa60), &column, &value_i); // ..02
    exo_destroy(&column);                                                              // 004ED907..13
    at<I>(creature, 0x4ec) = value_i;                                                  // 004ED918..1C
    (void)c2da_current_get_int_entry_00413510(two_da(0xc), nullptr, at<U16>(creature, 0xa60),
                                              static_cast<I>(kotor_rules_cell32_007a20d4), &value_10); // 004ED922..46
    at<I>(creature, 0xac4) = value_10;                                                 // 004ED94B..4F
    (void)c2da_current_get_int_entry_00413510(two_da(0xc), nullptr, at<U16>(creature, 0xa60),
                                              static_cast<I>(kotor_rules_cell32_007a2124), &value_14); // 004ED955..79
    at<I>(creature, 0x4f8) = value_14;                                                 // 004ED97E..86
    ExoString text; exo_empty(&text);                                                  // 004ED982..8C [esp+18]
    (void)c2da_current_get_string_entry_00413190(appearance, nullptr, at<U16>(creature, 0xa60),
                                                 static_cast<I>(kotor_rules_cell32_007a2110), &text); // 004ED991..BD
    (void)async_assign_string_005e5c50(static_cast<unsigned char*>(creature) + 0x4c8, nullptr, &text); // 004ED9C2..CD
    exo_text(&column, k_weaponscale_00746cc0);                                         // 004ED9D2..DB
    const U found = c2da_current_get_float_00413430(two_da(0xc), nullptr, at<U16>(creature, 0xa60), &column, &value_f); // ..07
    const bool ok = found == 1;                                                        // 004EDA0C..17
    exo_destroy(&column);                                                              // 004EDA13..1E
    if (ok) at<float>(creature, 0x500) = value_f;                                      // 004EDA27..2B
    else at<U>(creature, 0x500) = 0x3f800000u;                                         // 004EDA33 (1.0f)
    if (update_movement != 0) w2x_movement_rate_005a5680(stats_of(creature), nullptr, 7); // 004EDA3D..4C
    ExoString column2; exo_text(&column2, k_prefatckdist_00746cb0);                    // 004EDA51..5A [esp+28]
    (void)c2da_current_get_float_00413430(two_da(0xc), nullptr, at<U16>(creature, 0xa60), &column2, &value_f); // ..86
    exo_destroy(&column2);                                                             // 004EDA8B..94
    at<float>(creature, 0x4fc) = value_f;                                              // 004EDA99..A1 (stale on failure)
    at<U>(creature, 0x344) = 1;                                                        // 004EDAA7
    exo_destroy(&text);                                                                // 004EDA9D..B9
}

// 004AE9B0 CServerExoApp::AddToExclusionList(id, flags), RET 8: mov ecx,[ecx+4]; jmp 004B4730
extern "C" void __fastcall kotor_fastcall_004ae9b0(void* server, void*, U id, U flags) {
    kotor_fastcall_004b4730(at<void*>(server, 4), nullptr, id, flags);
}

// 004AE9C0 CServerExoApp::RemoveFromExclusionList(id, flags), RET 8: mov ecx,[ecx+4]; jmp 004B4800
extern "C" void __fastcall kotor_fastcall_004ae9c0(void* server, void*, U id, U flags) {
    kotor_fastcall_004b4800(at<void*>(server, 4), nullptr, id, flags);
}

namespace {
inline void* exclusion_list(void* internal, U offset) { return *at<void**>(internal, offset); } // [[internal+off]]
}

// 004B4730 CServerExoAppInternal::AddToExclusionList(id, BYTE flags), RET 8
//   bit 2 -> list [+10070h], else bit 1 -> list [+10074h]; appends the id unless already present.
extern "C" void __fastcall kotor_fastcall_004b4730(void* internal, void*, U id, U flags) {
    const U8 mask = static_cast<U8>(flags);                                            // 004B4730
    if (mask == 0) return;                                                             // 004B4734..39
    U offset;
    if ((mask & 2) != 0) offset = 0x10070;                                             // 004B473F..42
    else if ((mask & 1) != 0) offset = 0x10074;                                        // 004B47A3..A5
    else return;                                                                       // 004B47A5 -> 004B47FA
    void* position = *static_cast<void**>(exclusion_list(internal, offset));           // 004B4744..54 head
    while (position != nullptr) {                                                      // 004B4758..8E
        if (word(unresolved_list_at_pos_005e9b60(exclusion_list(internal, offset), nullptr, position)) == id) return; // ..70
        unresolved_list_next_005e9b90(exclusion_list(internal, offset), nullptr, &position); // 004B4776..83
    }
    resource_append_005e9dc0(exclusion_list(internal, offset), nullptr, ptr_of(id));   // 004B4790..99 AddTail
}

// 004B4800 CServerExoAppInternal::RemoveFromExclusionList(id, BYTE flags), RET 8
//   bit 1 -> list [+10074h], else bit 2 -> list [+10070h]; removes the first matching node.
extern "C" void __fastcall kotor_fastcall_004b4800(void* internal, void*, U id, U flags) {
    const U8 mask = static_cast<U8>(flags);                                            // 004B4800
    if (mask == 0) return;                                                             // 004B4804..09
    U offset;
    if ((mask & 1) != 0) offset = 0x10074;                                             // 004B480F..12
    else if ((mask & 2) != 0) offset = 0x10070;                                        // 004B4878..7A
    else return;
    void* position = *static_cast<void**>(exclusion_list(internal, offset));           // 004B4814..20
    while (position != nullptr) {                                                      // 004B4824 / 004B4858..5A
        if (word(unresolved_list_at_pos_005e9b60(exclusion_list(internal, offset), nullptr, position)) == id) { // ..40
            c2da_005e9c10(exclusion_list(internal, offset), nullptr, position);        // 004B4861..6E Remove
            return;
        }
        unresolved_list_next_005e9b90(exclusion_list(internal, offset), nullptr, &position); // 004B4842..4F
    }
}
