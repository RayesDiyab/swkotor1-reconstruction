// claude_aoe.cpp (lane "aoe"): server area-of-effect cascade, transcribed from the original swkotor.exe (read-only).
//
//   0050C6B0 CSWSArea::ApplyEffect(CGameEffect*, Vector* pos, Vector* orient)     RET 0Ch  @kotor_fastcall_0050c6b0@20
//   00507E20 CSWSArea::PlayVisualEffect(ushort vfx, Vector* pos)                  RET 8    @kotor_fastcall_00507e20@16
//   00505AF0 CSWSArea::LoadAreaEffects(CResGFF*, CResStruct*, int state, int)    RET 10h  @kotor_fastcall_00505af0@24
//   004DADE0 CSWSEffectListHandler::OnApplyAreaOfEffect(obj, effect, load)        RET 0Ch  @kotor_fastcall_004dade0@20
//   CSWSAreaOfEffectObject (size 290h, vtable 00749EA8 -> _kotor_vtable_00749ea8):
//   00594480 ctor(id) RET 4; 005963D0 dtor RET 0; 005965B0 deleting dtor RET 4; 005945D0 GetEffectSpellId RET 0;
//   005945E0 SetEffectSpellId RET 4; 005945F0 SetShape(byte,float,float) RET 0Ch; 00594770 SetDuration(byte,float)
//   RET 8; 005947B0 LoadAreaEffect(int row) RET 4; 00594B00 LoadEffect(CResGFF*, CResStruct*) RET 8; 00594FF0
//   SetCreator(id) RET 4; 005955A0 SetTargetObjID(id) RET 4; 00595560 (target list add) RET 4; 00595640
//   UpdateSubAreas(Vector* prev) RET 4; 00595D10 AIUpdate RET 0; 00595F30 AddToArea(area,x,y,z,flag) RET 14h;
//   00596360 JumpToPoint(area, Vector*) RET 8; 005964E0 EventHandler(type, caller, event, a, b) RET 14h.
//   Bound existing bodies: 005962D0 RemoveFromArea / 00595B60 RemoveFromSubAreas [claude_w1_route_bodies],
//   005946E0 GetPosition / 00595450 InAreaOfEffect [w2_party_native], 00594D80 SaveEffect [claude_save_git].
//
// Original quirks kept (see the per-line notes):
//   * GetPosition 005946E0 calls itself without bound when the AoE has no live target or is rectangular (the bound W2
//     body reports that path at 0059474E).  Every location AoE (CSWSArea::ApplyEffect) and every rectangular AoE
//     therefore never completes AddToArea in the original either.
//   * LoadEffect does not restore the three scripts; heartbeat is skipped while LastHrtbtTime == 0; OnApplyAreaOfEffect
//     passes only the low byte of the row; null results of operator new are dereferenced (no guards added).
// Conventions: thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; game heap only.
#include "842d6b1618-w5_t3.hpp"
#include <cmath>

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); }   // CExoString() [0603]
namespace frame_control { extern unsigned message_manager_007a3a00; }                        // g_pVirtualMachine [0456]
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }       // CExoBase* [0068]
namespace kotor_reconstruction { struct StartupString { int __thiscall BorrowedEquals005e5290(const char* text); }; } // [0077]
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringNotEqual_005e5390(options_video::String* s, void*, const char* text); } // [0374]
namespace kotor_q05 { struct StructHandle; struct CResGFF { unsigned GetElementType(StructHandle* s); }; }       // [0290]

extern "C" {
using w5::U; using w5::I; using w5::U8; using w5::U16; using w5::Vector;
// ---- lane "aoe" exports (claude_aoe_base.cpp)
void* __fastcall kotor_fastcall_004cfcb0(void* self, void*, U type, U id, U flag);
U __fastcall kotor_fastcall_004b0850(void* ai, void*, void* object, I level);
I __fastcall kotor_fastcall_004ef6e0(void* creature, void*, U spell);                         // CalculateSpellSaveDC [claude_vm_vmA_endar]
U __fastcall kotor_fastcall_0056d140(void* message, void*, void* player, U vfx, float x, float y, float z);
U __fastcall kotor_fastcall_004cc390(void*, void*, void*, U);
void __fastcall kotor_fastcall_004ccf60(void*, void*, U);
void* __fastcall kotor_fastcall_004d0170(void*, void*, void*);
U8 __fastcall kotor_fastcall_004cb190(void*, void*);
void* __fastcall kotor_fastcall_004d0190(void*, void*);
U __fastcall kotor_fastcall_004cb810(void*, void*);
U16 __fastcall kotor_fastcall_004d01a0(void*, void*, U);
U __fastcall kotor_fastcall_004caec0(void*, void*, U);
int __fastcall kotor_fastcall_004caee0(void*, void*, U);
int __fastcall kotor_fastcall_004caf70(void*, void*, U);
U __fastcall kotor_fastcall_004d01b0(void*, void*);
void* __fastcall kotor_fastcall_004d01c0(void*, void*, void*);
// ---- existing providers in the c31 candidate
void* __fastcall recovered_object_array_004aed70(void* server, void*);                         // GetObjectArray [w2_party_native]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                     // GetServerAIMaster [creature_destructor_r2]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                   // GetSWSMessage [creature_destructor_r2]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                              // GetModule [0530]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                                 // GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);              // GetWorldTime [0457]
U __fastcall recovered_world_time_difference_004ae460(void* timer, void*, U day, U time, U old_day, U old_time,
    U* out_day, U* out_time);                                                                  // SubtractWorldTimes [1025b]
std::uint64_t __fastcall recovered_bridge_exo_timers_high_resolution_005e9260(void* timers, void*); // GetHighResolutionTimer [1204-21]
U __fastcall recovered_event_enqueue_004b08d0(void* ai, void*, U day, U ms, U caller, U target, U type, void* event); // AddEventDeltaTime [1017]
U __fastcall recovered_ai_remove_004af3d0(void* ai, void*, void* object);                      // CServerAIMaster::RemoveObject [1070]
void* __fastcall recovered_event_construct_004d7540(void* event, void*);                       // CScriptEvent() [1017]
void __fastcall recovered_event_object_004d7780(void* event, void*, U index, U id);            // SetObjectID [1017]
void __fastcall recovered_event_destroy_004d7590(void* event, void*);                          // ~CScriptEvent [0886]
U __fastcall recovered_message_get_object_004d64c0(void* data, void*, U index);                // CSWCCMessageData::GetObjectID [w1 b7]
void __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U id, U flag);                // CVirtualMachine::RunScript [load_screen_thin]
unsigned __fastcall recovered_vm_run_script_situation_005d0fd0(void* vm, void*, void* situation, U id, U flag); // RunScriptSituation [1194s]
unsigned __fastcall recovered_object_run_actions_0057f4a0(void* self, void*, U day, U ms, U lo, U hi); // CSWSObject::RunActions [w1 b7]
void* __fastcall recovered_object_area_004cb120(void* object, void*);                          // CSWSObject::GetArea [1060]
void __fastcall kotor_fastcall_004cb160(void* object, void*, void* area);                     // CSWSObject::SetArea [claude_combat_spell_mine]
void __fastcall recovered_object_position_004cd1a0(void* object, void*, const Vector* p, U a, U b, U c); // SetPosition [0886]
void __fastcall kotor_fastcall_00553040(void* object, void*, void* tag);                      // CSWSObject::SetTag [claude_w1_route_bodies]
U __fastcall recovered_module_remove_tag_004c5ef0(void* module, void*, void* tag, U id);       // RemoveObjectFromLookupTable [1011]
U __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U id);     // AddObjectToLookupTable [1011]
void __fastcall recovered_object_destroy_004d0220(void* self, void*);                          // CSWSObject::~CSWSObject [0963]
void __fastcall w2x_object_state_004d1cf0(void* object, void*, void* gff, void* s);            // CSWSObject::LoadObjectState [0886]
U __fastcall recovered_area_index_00506f20(void* area, void*, U* out, U x_bits);              // GetFirstObjectIndiceByX [1065]
unsigned __fastcall recovered_area_insert_0050dfd0(void* area, void*, unsigned id, unsigned flag); // AddObjectToArea [1065]
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U id);                          // GetGameObject [0530]
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);                          // GetCreatureByGameObjectID [0595]
void* __fastcall kotor_fastcall_004ae910(void* server, void*, U id);                          // GetClientObjectByObjectId [0595]
unsigned __fastcall kotor_fastcall_004d8230(void* array, void*, U id, void* out);             // CGameObjectArray::GetGameObject [0457]
U __fastcall recovered_stats_class_level_005a5090(void* stats, void*, U index, U flag);         // GetClassLevel (AL) [w2_g07]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, int id);       // CSWSpellArray::GetSpell [0820]
void __fastcall w1x_unique_append_006449d0(void* list, void*, U value);                        // CExoArrayList<ulong>::AddUnique [0886]
U __fastcall recovered_int_list_remove_0066dcc0(void* list, void*, U value);                   // CExoArrayList<int>::remove [w1 b7]
void __fastcall kotor_fastcall_00595b60(void* aoe, void*, U flag);                             // RemoveFromSubAreas [claude_w1_route_bodies]
void __fastcall kotor_fastcall_005962d0(void* aoe, void*);                                     // RemoveFromArea [claude_w1_route_bodies]
void* __fastcall recovered_aoe_position_005946e0(void* aoe, void*, void* out);                 // GetPosition [w2_party_native]
unsigned __fastcall recovered_aoe_in_area_of_effect_00595450(void* aoe, void*, float x, float y, float z); // InAreaOfEffect [w2]
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                  // CGameEffect::GetInteger [w1 b7]
void* __fastcall recovered_effect_copy_string_00503700(void* effect, void*, void* out, U index); // CopyStringFromList [w2_save_object]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id);  // SetObjectID [1194u]
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);          // SetFloat [claude_w1_chains_damage]
void __fastcall kotor_fastcall_00504240(void* effect, void*);                                  // UpdateLinked [claude_combat_attack_damage]
void __fastcall kotor_fastcall_00504810(void* effect, void*, void* a, void* b);                // SetLinked [claude_combat_attack_damage]
void __fastcall recovered_effect_destroy_00503590(void* effect, void*);                        // ~CGameEffect [w1 b7]
void __fastcall recovered_effect_apply_004d14f0(void* object, void*, void* effect, U a, U b);  // CSWSObject::ApplyEffect [0886]
void* __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);            // CExoString(char const*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                               // ~CExoString [0459]
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                 // CExoString::operator= [0199]
int __fastcall c2da_current_get_string_00413270(void* table, void*, int row, void* column, void* out); // C2DA::GetCExoStringEntry
U __fastcall c2da_current_get_float_00413430(void* table, void*, U row, void* column_name, float* out); // C2DA::GetFLOATEntry [0376]
float __fastcall gui_load_gff_float_00411d00(void* gff, void*, void* s, const char* label, unsigned* ok, float fallback); // [0292]
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* s, const char* label, U* ok, U fallback); // [0292]
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* s, const char* label, unsigned* ok, int fallback); // [0292]
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* s, const char* label, U* ok, U fallback); // [0292]
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* s, const char* label, U* ok, void* fallback); // [0292]
U __fastcall gff_list_004118c0(void* gff, void*, void* list, void* s, const char* label);      // CResGFF::GetList [0291]
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index);      // GetListElement [0291]
unsigned __fastcall gff_list_count_00411940(void* gff, void*, void* list);                    // GetListCount [0291]
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, void* construct, void* destroy); // [d1]
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, void* destroy);       // eh_vector_destructor [0186]
void __stdcall vector_constructor_iterator_00401130(void* first, U size, I count, void* construct); // `vector constructor iterator' [0526]
void* __fastcall vector_default_construct_00409e70(void* self, void*);                         // Vector::Vector() [0526]
Vector* __cdecl vector_divide_00437a90(Vector* out, const Vector* v, float divisor);           // operator/ [0529]
Vector* __cdecl kotor_cdecl_00423bc0(Vector* out, const Vector* a, const Vector* b);           // Vector operator- [0457]
Vector* __cdecl segment_project_004ab360(Vector* out, const Vector* a, const Vector* b, const Vector* p); // [0565]
extern void* kotor_cell_rules_007a3a28;                                                        // [007A3A28] CSWRules*
// ---- generic slot bodies
void __fastcall slot_void_return_4_0060e760_a1(void*, void*, U);
void __fastcall kotor_m15e_slot008_void8_005b5e90(void*, void*, U, U);
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*, void*);
U __fastcall kotor_m15e_slot0d4_true_0063e7a0(void*, void*);
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);
void __fastcall kotor_fastcall_004ccf80(void* object, void*, I damage);
int __fastcall kotor_fastcall_004cf160(void*, void*, void*, int, U, U, U);
int __fastcall kotor_fastcall_004d09e0(void*, void*, void*, int, U, U, U);
int __fastcall kotor_fastcall_004d0e40(void*, void*, void*, int, U, U, U, U);
void __fastcall recovered_object_portrait_ref_004d01f0(void*, void*, void*);
U __fastcall recovered_creature_portrait_004d0210(void*, void*);
void __fastcall recovered_object_portrait_004cad20(void*, void*, U);
}

namespace {
using namespace w5;
constexpr U k_invalid_00746644 = 0x7f000000u;          // .rdata 00746644
constexpr U8 k_lookup_ok_0074666c = 0;                   // .rdata 0074666C byte 00
constexpr U k_event_timed_007441b8 = 1;                  // .rdata 007441B8
constexpr U k_event_script_007441dc = 10;                // .rdata 007441DC
constexpr U k_event_destroy_007441e0 = 11;               // .rdata 007441E0
constexpr U16 k_effect_aoe_007466f8 = 0x1f;              // .rdata 007466F8
constexpr U16 k_effect_summon_00746694 = 6;              // .rdata 00746694
constexpr U16 k_effect_vfx_007466f4 = 0x1e;              // .rdata 007466F4
constexpr U16 k_effect_link_0074671c = 0x28;             // .rdata 0074671C
constexpr U8 k_creature_type_00746633 = 4;               // .rdata 00746633 byte 04 (object types above it carry clients)
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline void* vm() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)); }
inline void* objects() { return recovered_object_array_004aed70(server_app(), nullptr); }
inline void* ai() { return recovered_server_ai_master_004aed80(server_app(), nullptr); }
inline U8 lookup(U id, void** out) { return static_cast<U8>(kotor_fastcall_004d8230(objects(), nullptr, id, out)); }
inline float ff(double v) { return static_cast<float>(v); }
inline double fd(void* p, U o) { return static_cast<double>(at<float>(p, o)); }
// __ftol2 006FAE8C: truncate to int64, keep the low dword (NaN / out of range -> 0x8000000000000000 -> 0)
inline U ftol2_low(double v) {
    if (!(v > -9223372036854775808.0 && v < 9223372036854775808.0)) return 0;
    return static_cast<U>(static_cast<std::uint64_t>(static_cast<std::int64_t>(v)));
}
// CScriptEvent allocation shape used by UpdateSubAreas (null result dereferenced, as the original)
inline void* new_event(U16 type, U object_id) {
    void* memory = live_operator_new(0x34);
    void* event = memory != nullptr ? recovered_event_construct_004d7540(memory, nullptr) : nullptr;
    at<U16>(event, 0) = type;
    recovered_event_object_004d7780(event, nullptr, 0, object_id);
    return event;
}
// inline CExoArrayList<ulong>::Add (growth via CExoArrayList::Allocate 005E03C0)
inline void list_add(void* list, U value) {
    const I size = at<I>(list, 4), cap = at<I>(list, 8);
    if (size == cap) {
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            static_cast<unsigned>(cap != 0 ? cap * 2 : 0x10));
    }
    at<U*>(list, 0)[at<I>(list, 4)] = value;
    ++at<I>(list, 4);
}
struct ExoString { void* data; U length; };   // CExoString (8 bytes)
}

extern "C" {
extern const void* const kotor_vtable_00749ea8[56];
void __fastcall kotor_fastcall_00595640(void* self, void*, const Vector* prev);
void __fastcall kotor_fastcall_00595f30(void* self, void*, void* area, float x, float y, float z, U flag);

// ======================================================================= small members
// 005945D0 GetEffectSpellId: +234. RET 0.
U __fastcall kotor_fastcall_005945d0(void* self, void*) { return at<U>(self, 0x234); }
// 005945E0 SetEffectSpellId(int): +234. RET 4.
void __fastcall kotor_fastcall_005945e0(void* self, void*, U id) { at<U>(self, 0x234) = id; }

// 00595560 (target creature +324 CExoArrayList<ulong>::Add): RET 4.
void __fastcall kotor_fastcall_00595560(void* creature, void*, U id) {
    list_add(bytes(creature) + 0x324, id);                                                       // 00595560..93
}

// 005945F0 SetShape(byte shape, float a, float b): 0 = circle (radius a), 1 = rectangle (width a, length b; radius =
// half diagonal; a fresh Vector[4] corner array; the previous array is not freed - original). RET 0Ch.
void __fastcall kotor_fastcall_005945f0(void* self, void*, U shape, float a, float b) {
    at<U8>(self, 0x230) = static_cast<U8>(shape);                                                // 005945FE..0C
    switch (static_cast<U8>(shape)) {                                                            // 00594612..20
    case 0:
        at<float>(self, 0x238) = a;                                                              // 005946B6..BA
        return;
    case 1: {
        const double ha = static_cast<double>(a) * 0.5, hb = static_cast<double>(b) * 0.5;       // 00594626..3E
        at<float>(self, 0x23c) = a;                                                              // 00594644
        at<float>(self, 0x240) = b;                                                              // 0059464A
        at<float>(self, 0x238) = ff(std::sqrt(hb * hb + ha * ha));                               // 00594650..5C
        void* corners = live_operator_new(0x30);                                                 // 0059463C, 66
        if (corners != nullptr) {                                                                // 00594674..7E
            vector_constructor_iterator_00401130(corners, 0xc, 4, reinterpret_cast<void*>(&vector_default_construct_00409e70)); // 00594680..8A
        }
        at<void*>(self, 0x244) = corners;                                                        // 00594693
        at<U>(self, 0x24c) = k_invalid_00746644;                                                 // 00594699
        return;
    }
    default:
        return;                                                                                  // 005946C0
    }
}

// 00594770 SetDuration(byte type, float seconds): +28C type; +288 = ftol(seconds*1000) only when seconds > 0. RET 8.
void __fastcall kotor_fastcall_00594770(void* self, void*, U type, float seconds) {
    at<U8>(self, 0x28c) = static_cast<U8>(type);                                                 // 00594774..81
    if (static_cast<double>(seconds) > 0.0)                                                      // 00594770..8C (<=, unordered -> skip)
        at<U>(self, 0x288) = ftol2_low(static_cast<double>(seconds) * 1000.0);                   // 0059478E..9D ([0073D6FC] 1000.0f)
}

// 00594FF0 SetCreator(id): RET 4.
void __fastcall kotor_fastcall_00594ff0(void* self, void*, U creator) {
    at<U>(self, 0x248) = creator;                                                                // 00594FF8
    void* c = kotor_fastcall_004ae770(server_app(), nullptr, creator);                           // 00594FFE..08
    if (c == nullptr) return;                                                                    // 0059500F..11
    if (vcall<I>(c, 0xb8) == -1) return;                                                         // 00595017..24
    at<U8>(self, 0x1d5) = at<U8>(c, 0x1d5);                                                      // 0059502A..30
    at<I>(self, 0x258) = kotor_fastcall_004ef6e0(c, nullptr, vcall<U>(c, 0xb8));                 // 00595036..48
    const U8 cls = at<U8>(c, 0x1d4);                                                             // 0059504E
    if (cls != 0xff && cls != 0xfe) {                                                            // 00595054..5A
        // 00595062..66: the class byte overwrites the low byte of the argument slot; bytes 1..3 of the creator id go along
        const U arg = (creator & 0xffffff00u) | cls;
        at<U>(self, 0x25c) = static_cast<U8>(recovered_stats_class_level_005a5090(at<void*>(c, 0xa74), nullptr, arg, 1)); // 0059506A..75
        return;
    }
    void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(at<void*>(kotor_cell_rules_007a3a28, 0x8c), nullptr,
                                                            vcall<int>(c, 0xb8));                // 00595080..96
    if (spell != nullptr) at<I>(self, 0x25c) = static_cast<I>(at<U8>(spell, 0x3f)) * 2 - 1;      // 0059509B..A7
}

// 005955A0 SetTargetObjID(id): RET 4.
void __fastcall kotor_fastcall_005955a0(void* self, void*, U target) {
    const U old = at<U>(self, 0x24c);                                                            // 005955A4
    if (old != k_invalid_00746644) {                                                             // 005955AA..B0
        void* o = nullptr;
        if (lookup(old, &o) == k_lookup_ok_0074666c && o != nullptr) {                           // 005955B2..DB
            const U id = at<U>(self, 4);                                                         // 005955E0
            (void)recovered_int_list_remove_0066dcc0(bytes(vcall<void*>(o, 0x30)) + 0x324, nullptr, id); // 005955DD..ED
        }
    }
    at<U>(self, 0x24c) = target;                                                                 // 005955F3..F7
    void* o = nullptr;
    if (lookup(target, &o) == k_lookup_ok_0074666c && o != nullptr) {                            // 005955FD..26
        const U id = at<U>(self, 4);                                                             // 00595628
        kotor_fastcall_00595560(vcall<void*>(o, 0x30), nullptr, id);                             // 0059562B..33
    }
}

// ======================================================================= construction / destruction
// 00594480 CSWSAreaOfEffectObject::CSWSAreaOfEffectObject(ulong id): RET 4.
void* __fastcall kotor_fastcall_00594480(void* self, void*, U id) {
    kotor_fastcall_004cfcb0(self, nullptr, 0xb, id, 0);                                          // 0059449E..AA
    at<const void*>(self, 0) = kotor_vtable_00749ea8;                                            // 005944C8
    crt_eh_vector_constructor_006fb55f(bytes(self) + 0x260, 8, 4,
        reinterpret_cast<void*>(&kotor_movie::string_element_construct_005b3190),
        reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));                                 // 005944AF..CE
    at<U>(self, 0x24c) = k_invalid_00746644;                                                     // 005944D3..D9
    at<U>(self, 0x250) = k_invalid_00746644;                                                     // 005944DF..E5
    at<U>(self, 0x254) = k_invalid_00746644;                                                     // 005944EB..F3
    at<U>(self, 0x248) = k_invalid_00746644;                                                     // 005944F9..0D
    const Vector zero = {0.0f, 0.0f, 0.0f};                                                      // 00594513..1B
    recovered_object_position_004cd1a0(self, nullptr, &zero, 1, 1, 0);                           // 005944E4..1F
    at<U>(self, 0x238) = 0; at<U>(self, 0x284) = 0; at<U>(self, 0x280) = 0;                      // 00594524..30
    at<U8>(self, 0x230) = 0; at<U>(self, 0x244) = 0; at<U>(self, 0x234) = 0xffffffffu;           // 00594536..42
    U day = 0, time = 0;
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 0059454C..66
    at<U>(self, 0xa8) = day; at<U>(self, 0xac) = time;                                           // 0059456B..79
    at<U>(self, 0x288) = 0; at<U8>(self, 0x28c) = 2;                                             // 0059457F..85
    (void)kotor_fastcall_004b0850(ai(), nullptr, self, 0);                                       // 0059458C..9D AIMaster->AddObject(this,0)
    at<U>(self, 0x25c) = 0; at<U>(self, 0x258) = 0xe;                                            // 005945A6..AC
    return self;
}

// 005963D0 CSWSAreaOfEffectObject::~CSWSAreaOfEffectObject: RET 0.
void __fastcall kotor_fastcall_005963d0(void* self, void*) {
    at<const void*>(self, 0) = kotor_vtable_00749ea8;                                            // 005963EF
    const U target = at<U>(self, 0x24c);                                                         // 005963F5
    if (target != k_invalid_00746644) {                                                          // 005963FB..09
        void* o = nullptr;                                                                       // [esp+4] (uninitialised in the original)
        if (lookup(target, &o) == k_lookup_ok_0074666c) {                                        // 0059640B..2C (no null check: original)
            const U id = at<U>(self, 4);                                                         // 00596435
            (void)recovered_int_list_remove_0066dcc0(bytes(vcall<void*>(o, 0x30)) + 0x324, nullptr, id); // 0059642E..42
        }
    }
    kotor_fastcall_005962d0(self, nullptr);                                                      // 00596448..4A
    if (at<void*>(self, 0x244) != nullptr) live_free(at<void*>(self, 0x244));                    // 0059644F..5F
    (void)recovered_ai_remove_004af3d0(ai(), nullptr, self);                                     // 00596462..73
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                            // 00596478..81
    if (module != nullptr) (void)recovered_module_remove_tag_004c5ef0(module, nullptr, bytes(self) + 0x18, at<U>(self, 4)); // 00596486..94
    c2da_destroy_array_006fa852(bytes(self) + 0x260, 8, 4, reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); // 00596499..AE
    recovered_object_destroy_004d0220(self, nullptr);                                            // 005964B3..BD
}

// 005965B0 scalar deleting destructor: RET 4.
void* __fastcall kotor_fastcall_005965b0(void* self, void*, U flags) {
    kotor_fastcall_005963d0(self, nullptr);                                                      // 005965B1..B3
    if (flags & 1) live_free(self);                                                              // 005965B8..C5
    return self;
}

// ======================================================================= vfx_persistent loading
// 005947B0 LoadAreaEffect(int row): SHAPE/RADIUS/WIDTH/LENGTH, ONENTER -> +270, HEARTBEAT -> +260, ONEXIT -> +278,
// LABEL -> tag; registers the tag in the module lookup table. RET 4.
void __fastcall kotor_fastcall_005947b0(void* self, void*, I row) {
    at<I>(self, 0x22c) = row;                                                                    // 005947CE..D5
    void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x1c);                   // 005947DB..E8
    ExoString shape; kotor_movie::string_element_construct_005b3190(&shape, nullptr);            // 005947EB..EF [esp+18]
    {
        ExoString col; frame_LogStringCtor_005e5a90(&col, nullptr, "SHAPE");                     // 005947F6..803 ([00749FB0])
        (void)c2da_current_get_string_00413270(table, nullptr, at<I>(self, 0x22c), &col, &shape); // 00594808..20
        frame_LogStringDtor_005e5c20(&col, nullptr);                                             // 00594825..2D
    }
    if (reinterpret_cast<kotor_reconstruction::StartupString*>(&shape)->BorrowedEquals005e5290("C")) { // 00594832..42 ([00749FAC])
        float radius;                                                                            // [esp+C] (unset if the entry is missing: original)
        ExoString col; frame_LogStringCtor_005e5a90(&col, nullptr, "RADIUS");                    // 00594844..4D ([007476A8])
        (void)c2da_current_get_float_00413430(table, nullptr, at<U>(self, 0x22c), &col, &radius); // 00594852..6A
        frame_LogStringDtor_005e5c20(&col, nullptr);                                             // 0059486F..77
        kotor_fastcall_005945f0(self, nullptr, 0, radius, 0.0f);                                 // 0059487C..1C SetShape(0, r, 0)
    } else if (reinterpret_cast<kotor_reconstruction::StartupString*>(&shape)->BorrowedEquals005e5290("R")) { // 00594888..98 ([007469A4])
        float width, length;                                                                     // [esp+10] / [esp+C]
        ExoString col; frame_LogStringCtor_005e5a90(&col, nullptr, "WIDTH");                     // 0059489E..A7 ([0073DF04])
        (void)c2da_current_get_float_00413430(table, nullptr, at<U>(self, 0x22c), &col, &width); // 005948AC..C4
        frame_LogStringDtor_005e5c20(&col, nullptr);                                             // 005948C9..D1
        ExoString col2; frame_LogStringCtor_005e5a90(&col2, nullptr, "LENGTH");                  // 005948D6..DF ([00749FA4])
        (void)c2da_current_get_float_00413430(table, nullptr, at<U>(self, 0x22c), &col2, &length); // 005948E4..FC
        frame_LogStringDtor_005e5c20(&col2, nullptr);                                            // 00594901..09
        kotor_fastcall_005945f0(self, nullptr, 1, width, length);                                // 0059490E..1C SetShape(1, w, l)
    }
    struct { const char* column; U field; } const scripts[3] = {
        {"ONENTER", 0x270}, {"HEARTBEAT", 0x260}, {"ONEXIT", 0x278}};                            // [00749F9C] [00749F90] [00749F88]
    ExoString values[3];                                                                         // [esp+38] [esp+30] [esp+28]
    for (U i = 0; i < 3; ++i) {                                                                  // 00594921..A21
        kotor_movie::string_element_construct_005b3190(&values[i], nullptr);
        ExoString col; frame_LogStringCtor_005e5a90(&col, nullptr, scripts[i].column);
        (void)c2da_current_get_string_00413270(table, nullptr, at<I>(self, 0x22c), &col, &values[i]);
        frame_LogStringDtor_005e5c20(&col, nullptr);
        async_assign_string_005e5c50(bytes(self) + scripts[i].field, nullptr, &values[i]);
    }
    ExoString label; kotor_movie::string_element_construct_005b3190(&label, nullptr);            // 00594A26..2A [esp+20]
    {
        ExoString col; frame_LogStringCtor_005e5a90(&col, nullptr, "LABEL");                     // 00594A2F..3D ([00748610])
        (void)c2da_current_get_string_00413270(table, nullptr, at<I>(self, 0x22c), &col, &label); // 00594A47..5A
        frame_LogStringDtor_005e5c20(&col, nullptr);                                             // 00594A5F..68
    }
    kotor_fastcall_00553040(self, nullptr, &label);                                              // 00594A6D..74 SetTag
    (void)recovered_module_register_tag_004c7de0(server_get_module_004ae6b0(server_app(), nullptr), nullptr,
                                                  bytes(self) + 0x18, at<U>(self, 4));           // 00594A79..91
    frame_LogStringDtor_005e5c20(&label, nullptr);                                               // 00594A96..9F
    frame_LogStringDtor_005e5c20(&values[2], nullptr);                                           // 00594AA4..AD
    frame_LogStringDtor_005e5c20(&values[1], nullptr);                                           // 00594AB2..BB
    frame_LogStringDtor_005e5c20(&values[0], nullptr);                                           // 00594AC0..C8
    frame_LogStringDtor_005e5c20(&shape, nullptr);                                               // 00594ACD..D9
}

// 00594B00 LoadEffect(CResGFF*, CResStruct*): GIT/save AreaEffectList entry; scripts are not restored. RET 8, EAX 1.
U __fastcall kotor_fastcall_00594b00(void* self, void*, void* gff, void* s) {
    U ok = 0;                                                                                    // [esp+10]
    ExoString fallback; frame_LogStringCtor_005e5a90(&fallback, nullptr, "");                    // 00594B1E..27 [esp+14]
    ExoString tag;                                                                               // [esp+1C]
    void* t = gui_load_gff_string_00411ec0(gff, nullptr, &tag, s, "Tag", &ok, &fallback);        // 00594B2C..53 ([00747A28])
    kotor_fastcall_00553040(self, nullptr, t);                                                   // 00594B58..60
    frame_LogStringDtor_005e5c20(&tag, nullptr);                                                 // 00594B65..6E
    frame_LogStringDtor_005e5c20(&fallback, nullptr);                                            // 00594B73..7F
    at<I>(self, 0x22c) = gui_load_gff_int_00411c90(gff, nullptr, s, "AreaEffectId", &ok, 0);     // 00594B84..A7
    at<U>(self, 0x234) = gui_load_gff_dword_00411c20(gff, nullptr, s, "SpellId", &ok, 0);        // 00594B98..C1
    const U8 shape = gui_load_gff_byte_00411a60(gff, nullptr, s, "Shape", &ok, 0);               // 00594BB2..CE
    at<U8>(self, 0x1d5) = gui_load_gff_byte_00411a60(gff, nullptr, s, "MetaMagicType", &ok, 0);  // 00594BCC..F1
    at<I>(self, 0x258) = gui_load_gff_int_00411c90(gff, nullptr, s, "SpellSaveDC", &ok, 0);      // 00594BE2..0B
    at<I>(self, 0x25c) = gui_load_gff_int_00411c90(gff, nullptr, s, "SpellLevel", &ok, 0);       // 00594BFC..16
    if (shape == 0) {                                                                            // 00594C1C..1E
        const float r = gui_load_gff_float_00411d00(gff, nullptr, s, "Radius", &ok, 0.0f);       // 00594C20..38
        kotor_fastcall_005945f0(self, nullptr, 0, r, 0.0f);                                      // 00594C3C..86
    } else if (shape == 1) {                                                                     // 00594C43..46
        const float length = gui_load_gff_float_00411d00(gff, nullptr, s, "Length", &ok, 0.0f);  // 00594C48..5C
        const float width = gui_load_gff_float_00411d00(gff, nullptr, s, "Width", &ok, 0.0f);    // 00594C60..74
        kotor_fastcall_005945f0(self, nullptr, 1, width, length);                                // 00594C78..86
    }
    at<U>(self, 0x248) = gui_load_gff_dword_00411c20(gff, nullptr, s, "CreatorId", &ok, 0);      // 00594C8B..AE
    at<U>(self, 0x24c) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LinkedToObject", &ok, 0); // 00594C9F..C8
    at<U>(self, 0x250) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastEntered", &ok, 0);    // 00594CB9..D5
    at<U>(self, 0x254) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastLeft", &ok, 0);       // 00594CD3..FC
    at<U>(self, 0x288) = gui_load_gff_dword_00411c20(gff, nullptr, s, "Duration", &ok, 0);       // 00594CED..16
    at<U8>(self, 0x28c) = gui_load_gff_byte_00411a60(gff, nullptr, s, "DurationType", &ok, 0);   // 00594D07..23
    at<U>(self, 0x280) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastHrtbtDay", &ok, 0);   // 00594D21..4A
    at<U>(self, 0x284) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastHrtbtTime", &ok, 0);  // 00594D3B..5A
    return 1;
}

// ======================================================================= area placement / sub-area membership
// 00595640 UpdateSubAreas(Vector* previous position or NULL): enter (0Ch) / exit (0Dh) script events for creatures.
// RET 4.
void __fastcall kotor_fastcall_00595640(void* self, void*, const Vector* prev) {
    if (recovered_object_area_004cb120(self, nullptr) == nullptr) return;                        // 0059565C..65
    void* const area = recovered_object_area_004cb120(self, nullptr);                            // 0059566B..7C [esp+28]
    void* const array = objects();                                                               // 00595674..8C [esp+24]
    U first = 0;                                                                                 // [esp+1C]
    Vector tmp;                                                                                  // [esp+44] / [esp+38]
    float lo = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &tmp))->x;    // 00595685..AA [esp+10]
    float hi = lo;                                                                               // 005956AE [esp+18]
    if (prev != nullptr) {                                                                       // 005956A6..B2
        // 005956B4..DD: C0 only (position.x < prev.x) -> position.x, else (>=, unordered) prev.x
        const double px = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &tmp))->x;
        if (px < static_cast<double>(prev->x)) lo = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &tmp))->x;
        else lo = prev->x;
        // 005956E5..12: (position.x <= prev.x or unordered) -> prev.x, else position.x
        const double qx = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &tmp))->x;
        if (qx > static_cast<double>(prev->x)) hi = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &tmp))->x;
        else hi = prev->x;
    }
    const float start = ff(static_cast<double>(lo) - fd(self, 0x238));                           // 00595716..27
    (void)recovered_area_index_00506f20(recovered_object_area_004cb120(self, nullptr), nullptr, &first, bits(start)); // 0059571A..32
    I index = static_cast<I>(first);                                                             // 00595737..43 [esp+20]
    if (index >= at<I>(area, 0x194)) return;                                                     // 0059573B..47
    for (;;) {
        void* object = nullptr;                                                                  // [esp+14]
        bool check_break = false;
        if (static_cast<U8>(kotor_fastcall_004d8230(array, nullptr, at<U*>(area, 0x190)[index], &object)) == k_lookup_ok_0074666c && // 00595750..6E
            vcall<void*>(object, 0x10) != nullptr) {                                             // 00595774..7F
            if (at<U8>(object, 8) != 5) {                                                        // 00595785..8D
                check_break = true;
            } else {
                void* const creature = vcall<void*>(object, 0x30);                               // 00595793..9B (edi)
                const U self_id = at<U>(self, 4);                                                // 00595798
                void* const list = bytes(creature) + 0x330;                                      // 005957A3 (ebx)
                U matches = 0;                                                                   // 005957A9..C0 (ebp, [esp+10])
                for (I k = 0, n = at<I>(creature, 0x334); k < n; ++k) if (at<U*>(list, 0)[k] == self_id) ++matches;
                // region: 0 = continue, 1 = inside (enter if not yet a member), 2 = outside (exit if member),
                //         3 = swept through (enter + exit)
                int region;
                if (at<U8>(self, 0x230) != 0) {                                                  // 005957C4..CC
                    region = recovered_aoe_in_area_of_effect_00595450(self, nullptr, at<float>(creature, 0x90),
                        at<float>(creature, 0x94), at<float>(creature, 0x98)) == 1 ? 1 : 2;      // 005959C3..EE
                } else if (prev != nullptr) {                                                    // 005957D2..DB
                    Vector p1, p2, proj, sum, diff;
                    const Vector* const P = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &p1)); // 005957E1..E8
                    sum.x = ff(fd(creature, 0x90) + static_cast<double>(P->x));                  // 005957ED..1F
                    sum.y = ff(fd(creature, 0x94) + static_cast<double>(P->y));
                    sum.z = ff(fd(creature, 0x98) + static_cast<double>(P->z));
                    const Vector* const Q = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &p2)); // 00595818..27
                    const Vector* const pr = segment_project_004ab360(&proj, Q, prev, &sum);     // 0059582C..32
                    const Vector c = *pr;                                                        // 0059583A..55 (copied to [esp+38])
                    const double dz = fd(creature, 0x98) - static_cast<double>(c.z);             // 00595837..46
                    const double dy = fd(creature, 0x94) - static_cast<double>(c.y);             // 0059584A..51
                    const double dx = fd(creature, 0x90) - static_cast<double>(c.x);             // 00595859..5F
                    const double d2 = (dx * dx + dy * dy) + dz * dz;                             // 00595863..71
                    const double r = fd(self, 0x238);
                    // 00595879..90: jp (outside / unordered) unless d2 < r*r or d2 == r*r
                    if (!(d2 < r * r || d2 == r * r)) {
                        region = 2;                                                              // 00595A7C (ebp = [esp+10])
                    } else {
                        kotor_cdecl_00423bc0(&diff, reinterpret_cast<const Vector*>(bytes(creature) + 0x90), prev); // 00595896..A4
                        const double e2 = (static_cast<double>(diff.z) * diff.z + static_cast<double>(diff.y) * diff.y)
                                          + static_cast<double>(diff.x) * diff.x;                // 005958A9..CC
                        // 005958CE..DB: C0 (r*r < e2) -> previous position was outside: swept; else plain inside
                        region = (r * r < e2) ? 3 : 1;
                    }
                } else {
                    Vector p;
                    const Vector* const P = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &p)); // 00595968..6F
                    const double dz = fd(creature, 0x98) - static_cast<double>(P->z);           // 00595974..7A
                    const double dy = fd(creature, 0x94) - static_cast<double>(P->y);           // 0059597D..83
                    const double dx = fd(creature, 0x90) - static_cast<double>(P->x);           // 00595986..8C
                    const double d2 = (dx * dx + dy * dy) + dz * dz;                             // 0059598E..9C
                    const double r = fd(self, 0x238);
                    region = (d2 < r * r || d2 == r * r) ? 1 : 2;                                // 005959A4..BB (jp -> outside)
                }
                if (region == 3) {                                                               // 005958E1..63
                    if (matches != 0) goto next;                                                 // 005958E1..E7
                    w1x_unique_append_006449d0(list, nullptr, self_id);                          // 005958ED..F3
                    void* enter = new_event(0xc, at<U>(creature, 4));                            // 005958F8..35
                    (void)recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(creature, 4), at<U>(self, 4),
                                                           k_event_script_007441dc, enter);       // 0059593A..5E
                    region = 4;                                                                  // 00595963 jmp 00595A88
                }
                if (region == 1) {                                                               // 005959F4..77
                    if (matches != 0) goto next;
                    w1x_unique_append_006449d0(list, nullptr, self_id);                          // 005959FC..02
                    void* enter = new_event(0xc, at<U>(creature, 4));                            // 00595A07..4A
                    (void)recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(creature, 4), at<U>(self, 4),
                                                           k_event_script_007441dc, enter);       // 00595A4F..72
                    goto next;                                                                   // 00595A77
                }
                if (region == 4 || matches != 0) {                                               // 00595A80..82
                    (void)recovered_int_list_remove_0066dcc0(list, nullptr, at<U>(self, 4));     // 00595A88..8E
                    void* leave = new_event(0xd, at<U>(creature, 4));                            // 00595A93..D6
                    (void)recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(creature, 4), at<U>(self, 4),
                                                           k_event_script_007441dc, leave);       // 00595ADB..FE
                }
                check_break = true;                                                              // 00595B03
            }
        }
        if (check_break) {                                                                       // 00595B03..21
            void* o = vcall<void*>(object, 0x10);
            const double dx = fd(o, 0x90) - static_cast<double>(hi);
            if (dx > fd(self, 0x238)) return;                                                    // je (C3=C0=0) -> past the window
        }
    next:
        index = index + 1;                                                                       // 00595B23..38
        if (index >= at<I>(area, 0x194)) return;
    }
}

// 00595F30 AddToArea(CSWSArea*, x, y, z, int flag): RET 14h.
void __fastcall kotor_fastcall_00595f30(void* self, void*, void* area, float x, float y, float z, U flag) {
    if (area == nullptr) return;                                                                 // 00595F37..3C
    kotor_fastcall_004cb160(self, nullptr, area);                                                // 00595F44..45
    const Vector pos = {x, y, z};                                                                // 00595F4A..6B
    recovered_object_position_004cd1a0(self, nullptr, &pos, 0, 1, 0);                            // 00595F56..6F
    if (at<U8>(self, 0x230) == 1 && at<void*>(self, 0x244) != nullptr) {                         // 00595F74..89
        Vector n;                                                                                // [esp+10]
        normalize_004ab180(&n, reinterpret_cast<const Vector*>(bytes(self) + 0x9c));                                              // 00595F90..9C
        const double W = fd(self, 0x23c), L = fd(self, 0x240);
        Vector* const c = at<Vector*>(self, 0x244);
        const double nx = n.x, ny = n.y, nz = n.z;
        // corner 0 = N*L + W*(-Ny, Nx, Nz): x keeps the unrounded W*-Ny, y/z use the stored float products
        {
            const double pwx = -ny * W;                                                          // 00595FA1..C4
            const float pwy = ff(nx * W), pwz = ff(nz * W);                                      // 00595FC6..D4
            const float ax = ff(nx * L), ay = ff(ny * L), az = ff(L * nz);                       // 00595FD8..F6
            c[0].x = ff(static_cast<double>(ax) + pwx);                                          // 00595FFA..08
            c[0].y = ff(static_cast<double>(ay) + pwy);                                          // 0059600E..26
            c[0].z = ff(static_cast<double>(az) + pwz);                                          // 0059601A..31
        }
        // corner 1 = N*L - W*(-Ny, Nx, Nz)
        {
            const double pwx = W * -ny;                                                          // 00596034..3C
            const float pwy = ff(nx * W), pwz = ff(nz * W);                                      // 0059603E..4C
            const float ax = ff(nx * L), ay = ff(ny * L), az = ff(L * nz);                       // 00596050..6E
            c[1].x = ff(static_cast<double>(ax) - pwx);                                          // 00596072..95
            c[1].y = ff(static_cast<double>(ay) - pwy);                                          // 00596080..A7
            c[1].z = ff(static_cast<double>(az) - pwz);                                          // 0059609B..B2
        }
        // corner 2 = -N*L - W*(-Ny, Nx, Nz)
        {
            const double pwx = W * -ny;                                                          // 005960B5..C3
            const float pwy = ff(nx * W), pwz = ff(nz * W);                                      // 005960C9..E0
            const float mnz = ff(-nz);                                                           // 005960F0..F6
            const float bx = ff(-nx * L), by = ff(-ny * L), bz = ff(static_cast<double>(mnz) * L); // 005960FA..14
            c[2].x = ff(static_cast<double>(bx) - pwx);                                          // 00596118..26
            c[2].y = ff(static_cast<double>(by) - pwy);                                          // 0059612C..44
            c[2].z = ff(static_cast<double>(bz) - pwz);                                          // 00596138..4F
        }
        // corner 3 = -N*L + W*(-Ny, Nx, Nz): x from two stored floats, y from two unrounded products, z mixed
        {
            const float pwx = ff(W * -ny);                                                       // 00596152..62
            const double pwy = nx * W;                                                           // 00596166..6A
            const double pwz = nz * W;                                                           // 0059616C..70
            const float mnz = ff(-nz);                                                           // 00596190..99
            const float bx = ff(-nx * L);                                                        // 005961A1..A5
            const double by = -ny * L;                                                           // 005961A9
            const float bz = ff(static_cast<double>(mnz) * L);                                   // 005961AD..B5
            c[3].x = ff(static_cast<double>(bx) + pwx);                                          // 005961B9..C9
            c[3].y = ff(by + pwy);                                                               // 005961CB..DB
            c[3].z = ff(static_cast<double>(bz) + pwz);                                          // 005961D1..E8
        }
        for (U i = 0; i < 4; ++i) {                                                              // 005961F0..50
            Vector p, half;
            const Vector* const P = static_cast<Vector*>(recovered_aoe_position_005946e0(self, nullptr, &p)); // 005961F0..F7
            Vector* const corner = at<Vector*>(self, 0x244) + i;                                 // 005961FC..02
            const Vector* const h = vector_divide_00437a90(&half, corner, 2.0f);                 // 00596205..12
            const float rx = ff(static_cast<double>(h->x) + P->x);                               // 00596217..24
            const float ry = ff(static_cast<double>(P->y) + h->y);                               // 00596228..2E
            const float rz = ff(static_cast<double>(P->z) + h->z);                               // 00596232..45
            corner->x = rx; corner->y = ry; corner->z = rz;                                      // 0059623C..4D
        }
    }
    (void)recovered_area_insert_0050dfd0(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4), flag); // 00596253..65
    void* const area2 = recovered_object_area_004cb120(self, nullptr);                           // 0059626A..6C
    list_add(bytes(area2) + 0x1a0, at<U>(self, 4));                                              // 00596271..A4
    vcall<void>(self, 0x7c, 1u);                                                                 // 005962A7..AD SetAnimation(1)
    kotor_fastcall_00595640(self, nullptr, nullptr);                                             // 005962B0..B4
}

// 00596360 JumpToPoint(CSWSArea*, Vector*): RET 8.
void __fastcall kotor_fastcall_00596360(void* self, void*, void* area, const Vector* pos) {
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(self, 0x24c));        // 00596360..73
    if (creature == nullptr) return;                                                             // 0059637A..7C
    if (recovered_object_area_004cb120(self, nullptr) != area) return;                           // 0059637E..89
    void* const list = bytes(creature) + 0x330;                                                  // 0059638E
    (void)recovered_int_list_remove_0066dcc0(list, nullptr, at<U>(self, 4));                     // 0059638B..97
    kotor_fastcall_00595b60(self, nullptr, 0);                                                   // 0059639C..A0
    recovered_object_position_004cd1a0(self, nullptr, pos, 1, 1, 0);                             // 005963A5..B2
    w1x_unique_append_006449d0(list, nullptr, at<U>(self, 4));                                   // 005963B7..BD
    kotor_fastcall_00595640(self, nullptr, nullptr);                                             // 005963C2..C6
}

// ======================================================================= AI
// 00595D10 AIUpdate: heartbeat script every 6000 ms (skipped while LastHrtbtTime == 0), follow the linked target,
// expire temporary AoEs, run queued actions. RET 0.
void __fastcall kotor_fastcall_00595d10(void* self, void*) {
    const std::uint64_t timer = recovered_bridge_exo_timers_high_resolution_005e9260(
        at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 4), nullptr);                   // 00595D10..20
    const U lo = static_cast<U>(timer), hi = static_cast<U>(timer >> 32);                        // edi / ebx
    U day = 0, time = 0;                                                                         // [esp+10] / [esp+C]
    frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 00595D25..43
    U dday = 0, dtime = 0;                                                                       // [esp+20] / [esp+14]
    (void)recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, day, time,
        at<U>(self, 0xa8), at<U>(self, 0xac), &dday, &dtime);                                    // 00595D48..7A
    U hday = 0, htime = 0;                                                                       // [esp+24] / [esp+18]
    (void)recovered_world_time_difference_004ae460(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, day, time,
        at<U>(self, 0x280), at<U>(self, 0x284), &hday, &htime);                                  // 00595D7F..B1
    at<U>(self, 0xcc) = dtime;                                                                   // 00595DB6..C2
    at<U>(self, 0xa8) = day; at<U>(self, 0xac) = time;                                           // 00595DD0..D6
    if (htime >= 6000) {                                                                         // 00595DC8..DC (unsigned)
        if (at<U>(self, 0x284) != 0)                                                             // 00595DDE..E6
            frame_R4_005d0fc0(vm(), nullptr, bytes(self) + 0x260, at<U>(self, 4), 1);            // 00595DE8..FB HEARTBEAT
        at<U>(self, 0x284) = time; at<U>(self, 0x280) = day;                                     // 00595E00..0E
    }
    const U target = at<U>(self, 0x24c);                                                         // 00595E14
    if (target != k_invalid_00746644 && at<U8>(self, 0x230) == 0) {                              // 00595E1A..2E
        void* o = nullptr;                                                                       // [esp+1C]
        if (lookup(target, &o) == k_lookup_ok_0074666c && o != nullptr) {                        // 00595E34..5D
            const Vector p = *reinterpret_cast<const Vector*>(bytes(o) + 0x90);                  // 00595E5F..76 (no vcall: original)
            if (recovered_object_area_004cb120(self, nullptr) != nullptr)                        // 00595E7A..81
                kotor_fastcall_00595640(self, nullptr, &p);                                      // 00595E83..8A
            recovered_object_position_004cd1a0(self, nullptr, &p, 1, 1, 0);                      // 00595E8F..9C
        } else {
            (void)recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(self, 4), at<U>(self, 4),
                                                   k_event_destroy_007441e0, nullptr);            // 00595EA3..C5
        }
    }
    if (at<U8>(self, 0x28c) == 1) {                                                              // 00595ECA..D1
        if (at<U>(self, 0x288) <= dtime) {                                                       // 00595ED3..DF (ja = keep)
            (void)recovered_event_enqueue_004b08d0(ai(), nullptr, 0, 0, at<U>(self, 4), at<U>(self, 4),
                                                   k_event_destroy_007441e0, nullptr);            // 00595EE1..02
            return;                                                                              // 00595F07..0D
        }
        at<U>(self, 0x288) -= dtime;                                                             // 00595F0E..10
    }
    (void)recovered_object_run_actions_0057f4a0(self, nullptr, day, time, lo, hi);               // 00595F16..24
}

// 005964E0 EventHandler(type, caller, void* event, a, b): RET 14h.
void __fastcall kotor_fastcall_005964e0(void* self, void*, U type, U, void* event, U, U) {
    if (type == k_event_timed_007441b8) {                                                        // 005964E7..ED
        if (event != nullptr)                                                                    // 005964EF..F5
            (void)recovered_vm_run_script_situation_005d0fd0(vm(), nullptr, event, at<U>(self, 4), 1); // 005964FB..08
        return;
    }
    if (type == k_event_script_007441dc) {                                                       // 00596511..17
        const U16 kind = at<U16>(event, 0);                                                      // 0059651A..1E
        if (kind == 0xc) {                                                                       // 00596521..25
            at<U>(self, 0x250) = recovered_message_get_object_004d64c0(event, nullptr, 0);       // 00596527..35
            frame_R4_005d0fc0(vm(), nullptr, bytes(self) + 0x270, at<U>(self, 4), 1);            // 00596530..6C ONENTER
        } else if (kind == 0xd) {                                                                // 00596544..48
            at<U>(self, 0x254) = recovered_message_get_object_004d64c0(event, nullptr, 0);       // 0059654A..53
            frame_R4_005d0fc0(vm(), nullptr, bytes(self) + 0x278, at<U>(self, 4), 1);            // 00596559..6C ONEXIT
        }
        recovered_event_destroy_004d7590(event, nullptr);                                        // 00596571..73
        live_free(event);                                                                        // 00596578..7E
        return;
    }
    if (type == k_event_destroy_007441e0 && at<U>(self, 0xec) != 0) {                            // 00596586..96
        kotor_fastcall_005962d0(self, nullptr);                                                  // 00596598..9A
        vcall<void*>(self, 0x00, 1u);                                                            // 0059659F..A5 delete this
    }
}

// 00749EA8 CSWSAreaOfEffectObject vtable (56 slots, original order).
const void* const kotor_vtable_00749ea8[56] = {
    reinterpret_cast<const void*>(&kotor_fastcall_005965b0),                   // +00 deleting dtor
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +04
    reinterpret_cast<const void*>(&kotor_m15e_slot008_void8_005b5e90),         // +08
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +0C
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),          // +10 AsSWSObject
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +14
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +18
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +1C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +20
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +24
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +28
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +2C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +30 AsSWSCreature
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +34
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +38
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +3C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +40
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +44
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +48
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +4C
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),          // +50 AsSWSAreaOfEffectObject
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +54
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +58
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +5C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +60
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +64
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +68
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +6C
    reinterpret_cast<const void*>(&kotor_fastcall_00595d10),                   // +70 AIUpdate
    reinterpret_cast<const void*>(&kotor_fastcall_004cc390),                   // +74 ClearAction
    reinterpret_cast<const void*>(&kotor_fastcall_005964e0),                   // +78 EventHandler
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf60),                   // +7C SetAnimation
    reinterpret_cast<const void*>(&kotor_fastcall_004d0170),                   // +80 GetDialogResref
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),          // +84 return_true
    reinterpret_cast<const void*>(&kotor_fastcall_004cb190),                   // +88 GetGender
    reinterpret_cast<const void*>(&kotor_fastcall_004d0190),                   // +8C GetLastName
    reinterpret_cast<const void*>(&kotor_fastcall_004d0190),                   // +90 GetLastName
    reinterpret_cast<const void*>(&kotor_fastcall_004cb810),                   // +94 GetDead
    reinterpret_cast<const void*>(&kotor_fastcall_004d01a0),                   // +98 GetMaxHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004caec0),                   // +9C GetCurrentHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf80),                   // +A0 DoDamage
    reinterpret_cast<const void*>(&kotor_fastcall_004d09e0),                   // +A4 DoDamageReduction
    reinterpret_cast<const void*>(&kotor_fastcall_004d0e40),                   // +A8 DoDamageResistance
    reinterpret_cast<const void*>(&kotor_fastcall_004cf160),                   // +AC DoDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caee0),                   // +B0 GetDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caf70),                   // +B4 GetDamageImmunityByFlags
    reinterpret_cast<const void*>(&kotor_fastcall_005945d0),                   // +B8 GetEffectSpellId
    reinterpret_cast<const void*>(&kotor_fastcall_005945e0),                   // +BC SetEffectSpellId
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +C0
    reinterpret_cast<const void*>(&kotor_fastcall_004d01b0),                   // +C4 GetLockOrientationToObject
    reinterpret_cast<const void*>(&kotor_fastcall_004d01c0),                   // +C8 GetPortrait
    reinterpret_cast<const void*>(&recovered_object_portrait_ref_004d01f0),    // +CC SetPortrait
    reinterpret_cast<const void*>(&recovered_creature_portrait_004d0210),      // +D0 GetPortraitId
    reinterpret_cast<const void*>(&recovered_object_portrait_004cad20),        // +D4 SetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),         // +D8
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +DC
};

// ======================================================================= CSWSArea / effect-list entry points
// 00507E20 CSWSArea::PlayVisualEffect(ushort vfx, Vector* pos): send message 4/2 to every client creature within
// (visualeffects RADIUS + 250) of pos. RET 8.
void __fastcall kotor_fastcall_00507e20(void* self, void*, U vfx, const Vector* pos) {
    void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x88);                   // 00507E38..47
    float radius;                                                                                // [esp+10] (unset if the entry is missing: original)
    {
        ExoString col; frame_LogStringCtor_005e5a90(&col, nullptr, "RADIUS");                    // 00507E4F..58 ([007476A8])
        (void)c2da_current_get_float_00413430(table, nullptr, static_cast<U16>(vfx), &col, &radius); // 00507E5D..77
        frame_LogStringDtor_005e5c20(&col, nullptr);                                             // 00507E7C..88
    }
    radius = ff(250.0 + static_cast<double>(radius));                                            // 00507E8D..A1 ([00747440] 250.0f)
    const float minx = ff(static_cast<double>(pos->x) - radius);                                 // 00507EA8..AE
    const float maxx = ff(static_cast<double>(radius) + pos->x);                                 // 00507EB2..B8
    void* const array = objects();                                                               // 00507EBC..C5 [esp+14]
    U index = 0;                                                                                 // [esp+44] (argument slot reuse)
    const U found = recovered_area_index_00506f20(self, nullptr, &index, bits(minx));             // 00507EC9..D1
    if (found != 1) return;                                                                      // 00507ED6..D9
    I i = static_cast<I>(index);                                                                 // 00507EDF (ebx)
    while (i < at<I>(self, 0x194)) {                                                             // 00507EE3..E9, 00507FE4..F1
        void* o = nullptr;                                                                       // [esp+18]
        if (static_cast<U8>(kotor_fastcall_004d8230(array, nullptr, at<U*>(self, 0x190)[i], &o)) == k_lookup_ok_0074666c &&
            o != nullptr && at<U8>(o, 8) > k_creature_type_00746633) {                           // 00507EF0..29
            const Vector p = *reinterpret_cast<const Vector*>(bytes(o) + 0x90);                  // 00507F2F..4D
            if (static_cast<double>(p.x) > static_cast<double>(maxx)) return;                    // 00507F3B..56 (je: C3=C0=0)
            void* client = kotor_fastcall_004ae910(server_app(), nullptr, at<U>(o, 4));          // 00507F5C..69
            if (client != nullptr) {                                                             // 00507F6E..72
                const double dx = static_cast<double>(p.x) - pos->x;                             // 00507F74..78
                const double dy = static_cast<double>(p.y) - pos->y;                             // 00507F7A..7E
                const double dz = static_cast<double>(p.z) - pos->z;                             // 00507F81..85
                const double d2 = (dz * dz + dy * dy) + dx * dx;                                 // 00507F88..96
                const double r2 = static_cast<double>(radius) * radius;                          // 00507F98..9C
                if (!(r2 < d2) && r2 == r2 && d2 == d2) {                                        // 00507FA0..AD (C0: less or unordered -> skip)
                    (void)kotor_fastcall_0056d140(recovered_server_sws_message_004aedb0(server_app(), nullptr), nullptr,
                                                  client, vfx, pos->x, pos->y, pos->z);          // 00507FAF..DB
                }
            }
        }
        ++i;                                                                                     // 00507FE0..ED
    }
}

// 0050C6B0 CSWSArea::ApplyEffect(CGameEffect*, Vector* pos, Vector* orientation): RET 0Ch.
void __fastcall kotor_fastcall_0050c6b0(void* self, void*, void* effect, const Vector* pos, void* orientation) {
    const U16 type = at<U16>(effect, 8);                                                         // 0050C6D1..D5
    if (type == k_effect_aoe_007466f8) {                                                         // 0050C6DC..E5
        void* memory = live_operator_new(0x290);                                                 // 0050C6EB..F5
        void* aoe = memory != nullptr ? kotor_fastcall_00594480(memory, nullptr, k_invalid_00746644) : nullptr; // 0050C6FC..12
        kotor_fastcall_00594ff0(aoe, nullptr, at<U>(effect, 0x18));                              // 0050C714..21
        if ((at<U8>(effect, 0xa) & 0x18) == 8 && at<U>(effect, 0x18) != k_invalid_00746644) {    // 0050C726..39
            void* creator = kotor_fastcall_004ae750(server_app(), nullptr, at<U>(effect, 0x18)); // 0050C73B..45
            if (creator != nullptr && vcall<void*>(creator, 0x10) != nullptr) {                  // 0050C74C..59
                const U id = at<U>(aoe, 4);                                                      // 0050C75B..62
                w1x_unique_append_006449d0(bytes(vcall<void*>(creator, 0x10)) + 0x13c, nullptr, id); // 0050C75E..74
            }
        }
        kotor_fastcall_005947b0(aoe, nullptr, static_cast<I>(recovered_effect_integer_00503690(effect, nullptr, 0))); // 0050C779..85
        kotor_fastcall_00594770(aoe, nullptr, at<U8>(effect, 0xa) & 7u, at<float>(effect, 0xc)); // 0050C78A..A2
        w1x_object_set_orientation_004cd170(aoe, nullptr, orientation);                          // 0050C7A7..AE
        vcall<void>(aoe, 0xbc, at<U>(effect, 0x1c));                                             // 0050C7B3..BB SetEffectSpellId
        const U fields[3] = {0x270, 0x260, 0x278};                                               // list strings 0 / 1 / 2
        for (U k = 0; k < 3; ++k) {                                                              // 0050C7C1..E8
            ExoString tmp;
            void* s = recovered_effect_copy_string_00503700(effect, nullptr, &tmp, k);
            const int differs = client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(s), nullptr, "");
            frame_LogStringDtor_005e5c20(&tmp, nullptr);
            if (differs) {
                ExoString tmp2;
                void* s2 = recovered_effect_copy_string_00503700(effect, nullptr, &tmp2, k);
                async_assign_string_005e5c50(bytes(aoe) + fields[k], nullptr, s2);
                frame_LogStringDtor_005e5c20(&tmp2, nullptr);
            }
        }
        kotor_fastcall_00595f30(aoe, nullptr, self, pos->x, pos->y, pos->z, 1);                  // 0050C8ED..05
    } else if (type == k_effect_summon_00746694) {                                               // 0050C90F..1A
        void* creature = kotor_fastcall_004ae770(server_app(), nullptr, at<U>(effect, 0x18));    // 0050C91C..29
        if (creature != nullptr) {                                                               // 0050C92E..32
            recovered_effect_set_object_id_005036f0(effect, nullptr, 0, at<U>(self, 0x120));     // 0050C938..43
            kotor_fastcall_005036d0(effect, nullptr, 0, bits(pos->x));                           // 0050C948..53
            kotor_fastcall_005036d0(effect, nullptr, 1, bits(pos->y));                           // 0050C958..60
            kotor_fastcall_005036d0(effect, nullptr, 2, bits(pos->z));                           // 0050C965..6D
            recovered_effect_apply_004d14f0(creature, nullptr, effect, 0, 0);                    // 0050C972..79
            return;                                                                              // 0050C97E (effect kept)
        }
    } else if (type == k_effect_vfx_007466f4) {                                                  // 0050C980..86
        kotor_fastcall_00507e20(self, nullptr, recovered_effect_integer_00503690(effect, nullptr, 0), pos); // 0050C988..99
    } else if (type == k_effect_link_0074671c) {                                                 // 0050C9A0..A6
        kotor_fastcall_00504240(effect, nullptr);                                                // 0050C9A8..AA
        if (at<void*>(effect, 0x28) != nullptr)
            kotor_fastcall_0050c6b0(self, nullptr, at<void*>(effect, 0x28), pos, orientation);   // 0050C9AF..C3
        if (at<void*>(effect, 0x2c) != nullptr)
            kotor_fastcall_0050c6b0(self, nullptr, at<void*>(effect, 0x2c), pos, orientation);   // 0050C9C8..D4
        kotor_fastcall_00504810(effect, nullptr, nullptr, nullptr);                              // 0050C9D9..DF
    }
    recovered_effect_destroy_00503590(effect, nullptr);                                          // 0050C9E4..E6
    live_free(effect);                                                                           // 0050C9EB..F1
}

// 004DADE0 CSWSEffectListHandler::OnApplyAreaOfEffect(CSWSObject*, CGameEffect*, int): creature-linked AoE. RET 0Ch,
// EAX 1 when the object is not a creature, else 0.
U __fastcall kotor_fastcall_004dade0(void*, void*, void* object, void* effect, U) {
    void* const creature = vcall<void*>(object, 0x30);                                           // 004DADF8..FE
    if (creature == nullptr) return 1;                                                           // 004DAE01..1C
    void* memory = live_operator_new(0x290);                                                     // 004DAE23..28
    void* aoe = memory != nullptr ? kotor_fastcall_00594480(memory, nullptr, k_invalid_00746644) : nullptr; // 004DAE30..50
    kotor_fastcall_00594ff0(aoe, nullptr, at<U>(effect, 0x18));                                  // 004DAE52..63
    kotor_fastcall_005947b0(aoe, nullptr, static_cast<I>(static_cast<U8>(recovered_effect_integer_00503690(effect, nullptr, 0)))); // 004DAE68..77 (movzx al)
    const U fields[3] = {0x270, 0x260, 0x278};
    for (U k = 0; k < 3; ++k) {                                                                  // 004DAE7C..FA3
        ExoString tmp;
        void* s = recovered_effect_copy_string_00503700(effect, nullptr, &tmp, k);
        const int differs = client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(s), nullptr, "");
        frame_LogStringDtor_005e5c20(&tmp, nullptr);
        if (differs) {
            ExoString tmp2;
            void* s2 = recovered_effect_copy_string_00503700(effect, nullptr, &tmp2, k);
            async_assign_string_005e5c50(bytes(aoe) + fields[k], nullptr, s2);
            frame_LogStringDtor_005e5c20(&tmp2, nullptr);
        }
    }
    vcall<void>(aoe, 0xbc, at<U>(effect, 0x1c));                                                 // 004DAFA8..B0 SetEffectSpellId
    void* const area = recovered_object_area_004cb120(creature, nullptr);                        // 004DAFB6..BC
    kotor_fastcall_005955a0(aoe, nullptr, at<U>(creature, 4));                                   // 004DAFC1..C9
    const Vector* p = reinterpret_cast<const Vector*>(bytes(creature) + 0x90);                   // 004DAFCE..D9
    kotor_fastcall_00595f30(aoe, nullptr, area, p->x, p->y, p->z, 1);                            // 004DAFDC..E4
    recovered_effect_set_object_id_005036f0(effect, nullptr, 0, at<U>(aoe, 4));                  // 004DAFE9..F1
    return 0;                                                                                    // 004DAFFD
}

// 00505AF0 CSWSArea::LoadAreaEffects(CResGFF*, CResStruct*, int load_state, int): RET 10h, EAX 1.
U __fastcall kotor_fastcall_00505af0(void* self, void*, void* gff, void* parent, U load_state, U) {
    unsigned char list[0x14] = {};                                                               // CResList: dword + char[16] label (GetList 004118C0 strncpy 16 at +4); [esp+3C] CResList
    if (gff_list_004118c0(gff, nullptr, list, parent, "AreaEffectList") == 0) return 1;          // 00505B08..2A ([0074751C])
    U i = 0;                                                                                     // [esp+10]
    if (gff_list_count_00411940(gff, nullptr, list) == 0) return 1;                              // 00505B30..44
    do {
        unsigned char element[0xc] = {};                                                         // argument slot reuse in the original
        U ok = 0;                                                                                // argument slot reuse in the original
        if (gff_list_element_00411990(gff, nullptr, element, list, i) != 0 &&                    // 00505B50..64
            static_cast<kotor_q05::CResGFF*>(gff)->GetElementType(
                reinterpret_cast<kotor_q05::StructHandle*>(element)) == 0xd) {                   // 00505B6A..79
            const U id = gui_load_gff_dword_00411c20(gff, nullptr, element, "ObjectId", &ok, k_invalid_00746644); // 00505B7F..95
            void* memory = live_operator_new(0x290);                                             // 00505B9A..A6
            void* aoe = memory != nullptr ? kotor_fastcall_00594480(memory, nullptr, id) : nullptr; // 00505BAD..C5
            if (aoe != nullptr) {                                                                // 00505BC7..D1
                if (kotor_fastcall_00594b00(aoe, nullptr, gff, element) == 0) {                  // 00505BD7..E6
                    vcall<void*>(aoe, 0x00, 1u);                                                 // 00505BE8..EE
                } else {
                    const float oz = gui_load_gff_float_00411d00(gff, nullptr, element, "OrientationZ", &ok, 0.0f); // 00505BF5..0D
                    const float oy = gui_load_gff_float_00411d00(gff, nullptr, element, "OrientationY", &ok, 0.0f); // 00505C11..29
                    const float ox = gui_load_gff_float_00411d00(gff, nullptr, element, "OrientationX", &ok, 0.0f); // 00505C2D..45
                    Vector v = {ox, oy, oz};                                                     // 00505C49..65 [esp+18]
                    const double len2 = (static_cast<double>(ox) * ox + static_cast<double>(oy) * oy)
                                        + static_cast<double>(oz) * oz;                          // 00505C49..73
                    Vector orientation;
                    if (len2 > static_cast<double>(0.0001f)) {                                   // 00505C75..80 ([00741870])
                        Vector n;
                        orientation = *normalize_004ab180(&n, &v);         // 00505C82..A4
                    } else {
                        orientation.x = 0.0f; orientation.y = 1.0f; orientation.z = 0.0f;        // 00505CA6..CE
                    }
                    w1x_object_set_orientation_004cd170(aoe, nullptr, &orientation);             // 00505CD2..DD
                    if (load_state != 0) w2x_object_state_004d1cf0(aoe, nullptr, gff, element);  // 00505CE2..EE
                    const U flag = load_state == 0 ? 1u : 0u;                                    // 00505CF3..F7
                    const float pz = gui_load_gff_float_00411d00(gff, nullptr, element, "PositionZ", &ok, 0.0f); // 00505CFA..0E
                    const float py = gui_load_gff_float_00411d00(gff, nullptr, element, "PositionY", &ok, 0.0f); // 00505D17..2A
                    const float px = gui_load_gff_float_00411d00(gff, nullptr, element, "PositionX", &ok, 0.0f); // 00505D33..46
                    kotor_fastcall_00595f30(aoe, nullptr, self, px, py, pz, flag);               // 00505D4B..56
                }
            }
        }
        ++i;                                                                                     // 00505D5B..67
    } while (i < gff_list_count_00411940(gff, nullptr, list));                                   // 00505D6B..72
    return 1;
}
}
