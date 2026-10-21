// claude_w3_update.cpp - successor of lane-w3 tier t3u (lane-w3/tiers/t3/src/w3_client_t3_update.cpp, unreviewed, built
// against an old base). Client GameObjectUpdate 'U' readers called by the 0066A2A0 router (1204-07 w3_client_objects.obj),
// the leaves they need that no linked object provides, and 0060E7D0 CSWCCreature::EvaluateNewWayPoints.
// Original: swkotor.exe GOG K1 (image base 0x400000), read-only static disassembly; every statement cites the original
// instruction range. Original stack slots are named by their entry-relative offset {E-x} (E = ESP at entry).
//
// Every t3u body was re-derived from the original instructions before reuse. Defects fixed against t3u:
//   D1 006975D0 GetRunLooksAwfulRate: t3u defined a second body (collides with creature_position_lowers.obj), zero-filled
//      the PUSH ECX output slot (original: appearance pointer bits) and narrowed ST0 to float. Not defined here: bound to
//      the existing wide (double) body; 006655E0 compares the wide ST0 against the run rate as the original (00666B40).
//   D2 00423C20 dotProd: ST0 is returned unrounded (x87 53-bit); t3u narrowed it to float before the epsilon compare.
//   D3 0060B750 CSWCCreaturePartyStatus(): EH state 0 destroys the +18h CExoString when operator=(char*) throws.
//   D4 006655E0 flag 2000h: [area+184h] is read before base->vtbl[104h]() (00667C40 before 00667C48); t3u left the
//      order unspecified (operands of !=).
//   D5 006655E0 / feedback / attack data / party status / path array: original EH cleanups (CExoString temporaries,
//      operator-new memory of a throwing constructor) are kept with unwind-only guards; t3u had none.
//   D6 0060E7D0 was an explicit trap in t3u; reconstructed here with its whole cascade.
#include "a1bf7fc0cb-w3_client_t3.h"
#include <initializer_list>

// ------------------------------------------------------------------ lane-w2 t4 owner (C++ linkage, cdecl, plain RET)
namespace w2 { namespace g07 { unsigned int is_beam_type_004ca960(unsigned int type); } }   // 004CA960 GetIsBeam (w2_g07_object.obj)
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }   // 005E5680 CExoString::Format

extern "C" {
// ------------------------------------------------------------------ existing providers (c05 map object noted)
void* __fastcall kotor_fastcall_005ed580(void* app, void*, std::uint32_t id);                         // 005ED580 GetGameObject RET 4 (0701)
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void* object, void*);      // 0063C060 (AX) (0817)
std::uint32_t __fastcall kotor_m15a_object_animation_stationary_0063c170(void* object, void*, std::uint32_t anim);   // RET 4 (0817)
void* __fastcall kotor_m15a_combat_attack_data_construct_0060b630(void* self, void*);                // 0060B630 (0816)
void __fastcall recovered_event_object_004d7780(void* data, void*, std::uint32_t a, std::uint32_t id);   // 004D7780 SetObjectID RET 8
void __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, const void* resref, std::uint32_t x,
                                                         std::uint32_t y, std::uint32_t z, std::uint32_t a5,
                                                         std::uint32_t a6, std::uint32_t a7, std::uint32_t a8,
                                                         std::uint32_t a9, std::uint32_t a10);   // 005D5E10 RET 28h (0812)
extern void* kotor_cell_sound_007a39ec;                                                              // [007A39EC] CExoSound* (0673)
void __fastcall show_tutorial_window_005edf40(void* app, void*, int window, unsigned a, unsigned b, int c);   // 005EDF40 RET 10h
void __fastcall kotor_fastcall_005eda60(void* app, void*, unsigned input_class, unsigned show);       // 005EDA60 SetInputClass RET 8
void __fastcall gui_load_array_add_00671c00(void* list, void*, unsigned value);                      // 00671C00 CExoArrayList::Add RET 4
void __fastcall deleteAt_006857a0(void* list, void*, int index);                                     // 006857A0 deleteAt RET 4
void __fastcall kotor_m15b_creature_stats_set_class_00647770(void* stats, void*, std::uint32_t index, std::uint32_t cls);   // RET 8
void __fastcall kotor_m15a_appearance_set_appearance_alignment_00698150(void* appearance, void*, std::uint32_t value);   // RET 4
double __fastcall recovered_client_appearance_get_run_looks_awful_rate_006975d0(void* appearance, void*);   // 006975D0 ST0 wide (creature_position_lowers.obj)
std::uint32_t __fastcall recovered_creature_incapacitated_004ef890(void* server_creature, void*);  // 004EF890 GetDeadTemp
std::uint8_t __cdecl kotor_cdecl_0045fab0();                                                         // 0045FAB0 AurCheckFrameBufferModificationsAvailable (AL)
float* __cdecl kotor_cdecl_00423bc0(float* out, const float* a, const float* b);                    // 00423BC0 out = a - b (cdecl)
void* __fastcall recovered_message_data_construct_004d69f0(void* self, void*);                       // 004D69F0 CSWCCMessageData()
std::uint32_t __fastcall recovered_party_member_00634620(void* party, void*, std::uint32_t id);     // 00634620 IsPartyMember RET 4
extern unsigned recovered_camera_reset_cell_007a2288;                                                // [007A2288]
std::uint32_t __fastcall kotor_fastcall_0063c380(void* object, void*, std::uint32_t anim);           // 0063C380 AnimationFireAndForget RET 4
std::uint32_t __fastcall kotor_fastcall_00611700(void* creature, void*, std::uint32_t a, std::uint32_t b);   // 00611700 RET 8
std::uint32_t __fastcall kotor_fastcall_00641dc0(void* module, void*);                               // 00641DC0 RestoreCamera
std::uint32_t __fastcall kotor_fastcall_006146e0(void* creature, void*, std::uint32_t target, float radius);   // 006146E0 LookAt RET 8
void __fastcall recovered_client_creature_set_lock_orientation_00610950(void* creature, void*, std::uint32_t target, std::uint32_t force);   // RET 8 (w1_native_b7)
void __fastcall recovered_client_creature_play_sound_set_sound_00611470(void* self, void*, std::uint32_t sound,
                                                                        std::uint32_t flag);   // 00611470 RET 8 (animation-sound-guard.obj)
void __fastcall w5_client_creature_compute_z_value_0060bf20(void* creature, void*, void* area, kotor_w3::Vector3* point,
                                                            float z, std::uint32_t out);   // 0060BF20 RET 10h (1206-11 w5_precalc)
std::int32_t __fastcall w5_client_area_test_direct_line_00604c40(void* area, void*, std::uint32_t id,
                                                                 const kotor_w3::Vector3* from, const kotor_w3::Vector3* to,
                                                                 float* radius, float band, std::uint32_t unused,
                                                                 void* hit);   // 00604C40 CSWCArea::TestDirectLine RET 1Ch (1206-11)
std::uint32_t __fastcall kotor_fastcall_005ed470(void* app, void*);                                  // 005ED470 GetCaptureEvents (0787)
void* __fastcall kotor_fastcall_005ed460(void* app, void*);                                          // 005ED460 GetCaptureEventFile (0787)
void* __fastcall frame_GetWorldTimer_005ed4f0(void* app, void*);                                     // 005ED4F0 GetWorldTimer (0457)
std::uint64_t __fastcall kotor_fastcall_004adc70(const void* timer, void*);                         // 004ADC70 GetSnapshotTime EDX:EAX (0453)
void __fastcall frame_LogWriteString_005e6a70(void* file, void*, void* text);                        // 005E6A70 CExoFile::Write RET 4 (0459)

// ------------------------------------------------------------------ this unit
std::uint32_t __fastcall recovered_client_handle_generic_object_update_00651880(void* self, void*, std::uint32_t id,
                                                                                std::uint32_t mask) noexcept(false);   // RET 8
std::uint32_t __fastcall recovered_client_handle_door_update_0064d420(void* self, void*, std::uint32_t id,
                                                                      std::uint32_t mask) noexcept(false);   // RET 8
std::uint32_t __fastcall recovered_client_handle_placeable_update_0064dae0(void* self, void*, std::uint32_t id,
                                                                           std::uint32_t mask) noexcept(false);   // RET 8
std::uint32_t __fastcall recovered_client_handle_update_visual_effects_0064dbc0(void* self, void*, void* object) noexcept(false);   // RET 4
std::uint32_t __fastcall recovered_client_handle_creature_update_006655e0(void* self, void*, std::uint32_t id,
                                                                          std::uint32_t mask) noexcept(false);   // RET 8
double __cdecl recovered_dot_product_00423c20(const kotor_w3::Vector3* a, const kotor_w3::Vector3* b) noexcept;   // cdecl, ST0
void __fastcall recovered_client_creature_reset_desired_position_timer_0060bf10(void* self, void*) noexcept;
void __fastcall recovered_client_creature_get_way_points_0060c4e0(void* self, void*, std::uint16_t* count, void** points,
                                                                   std::uint16_t* index) noexcept;   // RET 0Ch
void __fastcall recovered_client_creature_set_run_looks_awful_0060e1f0(void* self, void*, std::uint32_t value) noexcept(false);   // RET 4
void __fastcall recovered_client_creature_set_alignment_good_evil_0060f780(void* self, void*, std::uint32_t value) noexcept(false);   // RET 4
void __fastcall recovered_client_creature_stats_set_class_level_00647790(void* stats, void*, std::uint32_t index,
                                                                          std::uint32_t level) noexcept;   // RET 8
std::uint32_t __fastcall recovered_client_reattach_creature_to_scene_005edb80(void* app, void*, std::uint32_t id) noexcept;   // RET 4
void* __fastcall recovered_client_creature_party_status_construct_0060b750(void* self, void*) noexcept(false);
void __fastcall recovered_client_creature_set_stealth_state_00613930(void* self, void*, std::uint32_t state,
                                                                     std::uint32_t quiet) noexcept(false);   // RET 8
void __fastcall recovered_client_creature_set_combat_state_006182a0(void* self, void*, std::uint32_t state) noexcept(false);   // RET 4
void __fastcall recovered_client_creature_evaluate_new_way_points_0060e7d0(void* self, void*, std::uint32_t count,
                                                                           void* points, std::uint32_t ai_byte,
                                                                           void* position) noexcept(false);   // RET 10h
}

using namespace kotor_w3;

namespace {
constexpr U8 kDoor_00746639 = 10;                                      // .rdata 00746639 (0Ah)
constexpr U kInvalidObject_00746644 = 0x7F000000u;                     // .rdata 00746644 (7F000000h)
constexpr float kEpsilon_00747504 = 9.99999974737875e-06f;             // .rdata 00747504 (3727C5ACh)
constexpr float kNearDistanceSq_0074f348 = 36.0f;                      // .rdata 0074F348 (42100000h)
constexpr float kRunRateScale_0073d708 = 0.001f;                       // .rdata 0073D708 (3A83126Fh)
constexpr U kLineBand_3c800000 = 0x3C800000u;                          // immediate 0.015625f (0060E87B / 0060E897)
const char kPortraitPrefix_00745e4c[] = "po_";                         // .rdata 00745E4C
const char kBaseResRef_00745e40[] = "BaseResRef";                      // .rdata 00745E40
const char kEmpty_0073d71c[] = "";                                     // .rdata 0073D71C
const char kInvisibleSound_00752124[] = "sdr_invisible";               // .rdata 00752124
const char kCaptureFormat_0074f3ac[] = "%d %c %x %f %f %f\n";          // .rdata 0074F3AC

float rf(void* m) noexcept(false) { return kotor_fastcall_004d67b0(m, nullptr, bitsf(1.0f), 0x20); }   // ReadFLOAT(1.0, 20h)
U16 rw(void* m, U bits = 0x10) noexcept(false) { return kotor_fastcall_004d6750(m, nullptr, bits); }   // ReadWORD
U8 rb(void* m, U bits = 8) noexcept(false) { return kotor_fastcall_004d6720(m, nullptr, bits); }       // ReadBYTE
U rbool(void* m) noexcept(false) { return kotor_fastcall_004d66f0(m, nullptr); }                       // ReadBOOL
U rid(void* m) noexcept(false) { return kotor_fastcall_00692630(m, nullptr); }                         // ReadOBJECTIDClient
// MessageReadOverflow(1): 004D6230 ignores its stack word ([+2Ch] < [+30h], RET 4).
U overflow(void* m) noexcept(false) { return recovered_player_message::overrun_004d6230(m); }
U fb(float f) noexcept { return bitsf(f); }

void construct_string(ExoString* s) noexcept(false) {
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr);   // 005B3190
}
void destroy_string(ExoString* s) noexcept(false) { frame_LogStringDtor_005e5c20(s, nullptr); }   // 005E5C20
void destroy_string_void(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
void free_void(void* p) { live_free(p); }
kotor_reconstruction::StartupString* ss(void* exo_string) noexcept {
    return static_cast<kotor_reconstruction::StartupString*>(exo_string);
}

// Unwind-only cleanup: runs the original EH-state action only when its range is left by an exception (each normal exit
// dismisses it first). Normal
// paths perform the original explicit destructor calls themselves, in the original order.
class Unwind {
public:
    Unwind(void (*action)(void*), void* object) noexcept : action_(action), object_(object), armed_(true) {}
    ~Unwind() { if (armed_) action_(object_); }   // reached armed only while unwinding
    void dismiss() noexcept { armed_ = false; }   // every normal exit of the guarded range
    Unwind(const Unwind&) = delete;
    Unwind& operator=(const Unwind&) = delete;
private:
    void (*action_)(void*);
    void* object_;
    bool armed_;
};
// new T(...): operator new(size); NULL -> NULL without construction; a throwing constructor frees the block (EH state).
template <class Ctor> void* new_object(U size, Ctor ctor) noexcept(false) {
    void* const memory = live_operator_new(size);                      // push size; call 006FA7E6
    if (memory == nullptr) return nullptr;                             // test eax,eax / xor eax,eax
    Unwind guard(&free_void, memory);
    void* const object = ctor(memory);
    guard.dismiss();
    return object;
}
// rep movsd + rep movsb through the raw pointers (a NULL destination faults as the original).
void copy_movs(void* destination, const void* source, U bytes) noexcept {
    volatile unsigned char* d = static_cast<volatile unsigned char*>(destination);
    const volatile unsigned char* s = static_cast<const volatile unsigned char*>(source);
    for (U i = 0; i < bytes; ++i) d[i] = s[i];
}

// x87 squared lengths in the original operand order (fld/fmul/faddp) evaluated in double: the products of two floats are
// exact in double and each sum rounds once, as x87 with 53-bit precision control.
double sq2(double x, double y) noexcept { return x * x + y * y; }
double sq3_zyx(double x, double y, double z) noexcept { return (z * z + y * y) + x * x; }
double sq3_xyz(double x, double y, double z) noexcept { return (x * x + y * y) + z * z; }
// fcomp + fnstsw + "test ah,41h": jnp/jp pairs below reduce to an ordered <= (C0|C3 without C2).
bool le(double a, float b) noexcept { return a <= static_cast<double>(b); }

// Inline sequence repeated 14 times in 006655E0 (one call site per feedback code, cited by the caller):
//   new CSWCCMessageData (34h; EH state frees on a throwing ctor); SetObjectID(0, creature id); CExoString t("");
//   AddFeedbackMessage(code, data, &t) (EH state destroys t); ~CExoString. A failed allocation passes a NULL this to
//   SetObjectID, as the original.
void feedback(void* creature, U code) noexcept(false) {
    void* const data = new_object(0x34, [](void* m) { return recovered_message_data_construct_004d69f0(m, nullptr); });
    recovered_event_object_004d7780(data, nullptr, 0, rd32(creature, 4));   // push [esi+4]; push 0; call 004D7780
    ExoString text;
    frame_LogStringCtor_005e5a90(&text, nullptr, kEmpty_0073d71c);     // push 0073D71C; call 005E5A90
    {
        Unwind guard(&destroy_string_void, &text);
        recovered_client_add_feedback_message_005ed360(client_app(), nullptr, code, data, &text);   // call 005ED360
        guard.dismiss();
    }
    destroy_string(&text);                                             // call 005E5C20
}
// Play3DOneShotSound("sdr_invisible") at the creature position (006667C7..0066680A / 006668C4..00666907).
void invisible_sound(void* creature) noexcept(false) {
    ResRef sound_ref;                                                  // {E-60h}
    reinterpret_cast<menu_resources::Ref*>(&sound_ref)->construct_00406d80(kInvisibleSound_00752124);   // 00406D80
    kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &sound_ref, rd32(creature, 0x24),
                                              rd32(creature, 0x28), rd32(creature, 0x2C), 0, 0xB, 0, 0, 0, 0);   // 005D5E10
}
// PowerItem(0, 0, 1, 0) on the item equipped in `slot` (0066696C..00666992 / 00666997..006669BD).
void power_equipped(void* creature, U slot) noexcept(false) {
    const U item_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, slot);   // 0060CA80
    void* item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, item_id);       // 005ED5A0
    if (item != nullptr) kotor_m15a_item_power_item_00647130(item, nullptr, 0, 0, 1, 0);          // 00647130
}
void* attack_data_ctor(void* m) noexcept(false) { return kotor_m15a_combat_attack_data_construct_0060b630(m, nullptr); }
}  // namespace

extern "C" {
// =========================================================================================== leaves
// 00423C20 dotProd(Vector const&, Vector const&) cdecl: ST0 = (a.z*b.z + a.y*b.y) + a.x*b.x, left wide (no fstp).
double __cdecl recovered_dot_product_00423c20(const Vector3* a, const Vector3* b) noexcept {
    const double z = static_cast<double>(a->z) * b->z;                 // 00423C28..00423C2B
    const double y = static_cast<double>(a->y) * b->y;                 // 00423C2E..00423C31
    return (z + y) + static_cast<double>(a->x) * b->x;                 // 00423C34..00423C3C
}
// 0060BF10 CSWCCreature::ResetDesiredPositionTimer: [this+1E8h] = 200h. RET.
void __fastcall recovered_client_creature_reset_desired_position_timer_0060bf10(void* self, void*) noexcept {
    st<U>(self, 0x1E8, 0x200);                                         // 0060BF10..0060BF1A
}
// 0060C4E0 CSWCCreature::GetWayPoints(WORD* count, Vector** points, WORD* index). RET 0Ch. index = position of the
// current waypoint pointer [+1C0h] in the array [+1BCh] (0 when absent).
void __fastcall recovered_client_creature_get_way_points_0060c4e0(void* self, void*, U16* count, void** points,
                                                                   U16* index) noexcept {
    *count = ld<U16>(self, 0x1EC);                                     // 0060C4E0..0060C4EB
    *points = rp(self, 0x1BC);                                         // 0060C4EE..0060C4F8
    const I32 n = ld<U16>(self, 0x1EC);                                // 0060C4FA movzx
    I32 i = 0;                                                         // 0060C501
    if (n > 0) {                                                       // 0060C503..0060C506 (jle)
        const U current = rd32(self, 0x1C0);                           // 0060C508
        U p = rd32(self, 0x1BC);                                       // 0060C50E
        do {
            if (p == current) { *index = static_cast<U16>(i); return; }   // 0060C514..0060C516 / 0060C52D..0060C535
            ++i; p += 0xC;                                             // 0060C518..0060C519
        } while (i < n);                                               // 0060C51C..0060C51E
    }
    *index = 0;                                                        // 0060C520..0060C52A
}
// 0060E1F0 CSWCCreature::SetRunLooksAwful(BOOL): [GetProperty0x68()+CCh] = value. RET 4.
void __fastcall recovered_client_creature_set_run_looks_awful_0060e1f0(void* self, void*, U value) noexcept(false) {
    void* const base = gob_get_property_0x68_0063c070(self, nullptr);  // 0060E1F0
    st<U>(base, 0xCC, value);                                          // 0060E1F5..0060E1F9
}
// 0060F780 CSWCCreature::SetAlignmentGoodEvil(short): stats +80h = clamp(value, 0, 100); appearance alignment(value). RET 4.
void __fastcall recovered_client_creature_set_alignment_good_evil_0060f780(void* self, void*, U value) noexcept(false) {
    void* const stats = rp(self, 0x2F8);                               // 0060F780
    if (stats != nullptr) {                                            // 0060F786..0060F78D
        const std::int16_t v = static_cast<std::int16_t>(value);       // 0060F789 si
        I32 clamped = 0;                                               // 0060F7A3
        if (v >= 0) clamped = v > 0x64 ? 0x64 : v;                     // 0060F78F..0060F7A1
        st<U16>(stats, 0x80, static_cast<U16>(clamped));               // 0060F7A5
    }
    void* const appearance = rp(self, 0x21C);                          // 0060F7AC
    if (appearance != nullptr) kotor_m15a_appearance_set_appearance_alignment_00698150(appearance, nullptr, value);   // 0060F7B2..0060F7B7 (full dword)
}
// 00647790 CSWCCreatureStats::SetClassLevel(BYTE index, BYTE level): index < 2 -> BYTE [this + index*20h + F1h]. RET 8.
void __fastcall recovered_client_creature_stats_set_class_level_00647790(void* stats, void*, U index, U level) noexcept {
    const U8 i = static_cast<U8>(index);                               // 00647790
    if (i >= 2) return;                                                // 00647794..00647796 (jae)
    st<U8>(stats, static_cast<U>(i) * 0x20 + 0xF1, static_cast<U8>(level));   // 00647798..006477A2
}
// 005EDB80 CClientExoApp::ReattachCreatureToScene(id): [[this+4]+28Ch] != id. RET 4.
U __fastcall recovered_client_reattach_creature_to_scene_005edb80(void* app, void*, U id) noexcept {
    return rd32(rp(app, 4), 0x28C) != id ? 1u : 0u;                    // 005EDB80..005EDB98
}
// 0060B750 CSWCCreaturePartyStatus::CSWCCreaturePartyStatus: +18h CExoString(), +0Ch = 0, +4/+8 = 1, +14h = invalid,
// +18h = "" (EH state 0 destroys +18h when operator= throws). EAX = this.
void* __fastcall recovered_client_creature_party_status_construct_0060b750(void* self, void*) noexcept(false) {
    construct_string(static_cast<ExoString*>(at(self, 0x18)));         // 0060B76A..0060B773
    Unwind guard(&destroy_string_void, at(self, 0x18));                // [esp+14h] = 0
    st<U>(self, 0x0C, 0);                                              // 0060B783
    st<U>(self, 0x04, 1);                                              // 0060B78D
    st<U>(self, 0x08, 1);                                              // 0060B790
    st<U>(self, 0x14, kInvalidObject_00746644);                        // 0060B793
    c2da_assign_text_005e5140(at(self, 0x18), nullptr, kEmpty_0073d71c);   // 0060B786..0060B79A
    guard.dismiss();
    return self;                                                       // 0060B7A4
}
// 00613930 CSWCCreature::SetStealthState(BOOL state, BOOL quiet). RET 8. Entering stealth (from +194h == 0) plays the
// sound-set entry 15h and adds the stealth effect (1F40h when frame-buffer effects are available, else 1F42h); leaving
// stealth (from 1) removes it and adds 1F41h. Without `quiet`, either transition also fires animation 2798h.
void __fastcall recovered_client_creature_set_stealth_state_00613930(void* self, void*, U state, U quiet) noexcept(false) {
    using AddEffect = U (__fastcall*)(void*, void*, U, U, U, U, U, U, Vector3*);
    U effect, a2;
    if (state == 1) {                                                  // 00613939..0061393E
        if (rd32(self, 0x194) != 0) { st<U>(self, 0x194, state); return; }   // 00613940..00613948 -> 00613A55
        recovered_client_creature_play_sound_set_sound_00611470(self, nullptr, 0x15, state);   // 0061394E..00613951
        void* const options = kotor_fastcall_005ed700(client_app(), nullptr);   // 00613956..0061395E
        effect = (rp(options, 0x30) != nullptr && kotor_cdecl_0045fab0() != 0) ? 0x1F40u : 0x1F42u;   // 00613963..006139A7
        a2 = 0;                                                        // 00613986 / 006139A5
    } else {
        if (state != 0 || rd32(self, 0x194) != 1) { st<U>(self, 0x194, state); return; }   // 006139AE..006139BD
        void* const options = kotor_fastcall_005ed700(client_app(), nullptr);   // 006139C3..006139CB
        const U removed = (rp(options, 0x30) != nullptr && kotor_cdecl_0045fab0() != 0) ? 0x1F40u : 0x1F42u;   // 006139D0..006139F3
        vslot<U (__fastcall*)(void*, void*, U)>(self, 0xB8)(self, nullptr, removed);   // 006139E0..006139FA
        effect = 0x1F41;                                               // 00613A15
        a2 = 1;                                                        // 00613A13
    }
    Vector3 zero = {0.0f, 0.0f, 0.0f};                                 // 00613A1E..00613A2E {E-0Ch}
    vslot<AddEffect>(self, 0xB4)(self, nullptr, effect, a2, kInvalidObject_00746644, kInvalidObject_00746644, 0, 0,
                                 &zero);                               // 00613A00..00613A36
    if (quiet == 0)                                                    // 00613A3C..00613A42
        vslot<U (__fastcall*)(void*, void*, U, U)>(self, 0xA0)(self, nullptr, 0x2798, 0);   // 00613A44..00613A4F
    st<U>(self, 0x194, state);                                         // 00613A55
}
// 006182A0 CSWCCreature::SetCombatState(BOOL). RET 4. On a change, powers the two equipped weapons (slots 10h / 20h)
// unless the server creature is dead; entering combat also looks at nothing and sets +140h.
void __fastcall recovered_client_creature_set_combat_state_006182a0(void* self, void*, U state) noexcept(false) {
    const U old = rd32(self, 0x138);                                   // 006182AB
    st<U>(self, 0x138, state);                                         // 006182B1
    if (state != 0) (void)gob_get_property_0x68_0063c070(self, nullptr);   // 006182A5..006182B9 (result unused)
    if (old != state && kotor_m15a_object_get_server_object_0063d4b0(self, nullptr) != nullptr) {   // 006182BE..006182CF
        void* const server_object = kotor_m15a_object_get_server_object_0063d4b0(self, nullptr);   // 006182D5..006182D7
        void* const server = vslot<void* (__fastcall*)(void*, void*)>(server_object, 0x30)(server_object, nullptr);   // 006182DC..006182E0
        if (server != nullptr &&                                       // 006182E3..006182E7
            vslot<U (__fastcall*)(void*, void*)>(server, 0x94)(server, nullptr) == 0 &&   // 006182ED..006182F9
            recovered_creature_incapacitated_004ef890(server, nullptr) == 0) {          // 006182FF..00618308
            for (U slot : {0x10u, 0x20u}) {                            // 0061830E / 00618353
                const U item_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(self, nullptr, slot);   // 00618310..00618312
                if (item_id == kInvalidObject_00746644) continue;      // 00618317..0061831C
                void* const object = kotor_fastcall_005ed580(client_app(), nullptr, item_id);   // 0061831E..00618328
                if (object == nullptr) continue;                       // 0061832D..00618331
                using AsItem = void* (__fastcall*)(void*, void*);
                if (vslot<AsItem>(object, 0x34)(object, nullptr) == nullptr) continue;   // 00618333..0061833C
                void* const item = vslot<AsItem>(object, 0x34)(object, nullptr);   // 0061833E..00618349
                kotor_m15a_item_power_item_00647130(item, nullptr, state, 0, 1, 0);   // 00618340..0061834E
            }
        }
    }
    if (state != 0) {                                                  // 00618398..0061839A
        kotor_fastcall_006146e0(self, nullptr, kInvalidObject_00746644, 0.0f);   // 0061839C..006183A5
        st<U>(self, 0x140, 1);                                         // 006183AB
    } else {
        st<U>(self, 0x140, 0);                                         // 006183BB
    }
}

// 0060E7D0 CSWCCreature::EvaluateNewWayPoints(int count, Vector* points, BYTE ai, Vector* position). RET 10h.
// Chooses the waypoint list the creature walks: the server path as is when the direct line from the current position
// to its first point is clear; else [position] + path when the second (identical) test passes; else the tail from the
// first index whose test passes; else, when the destination (vtbl 150h) reaches the path's last point, the remaining own
// waypoints + that last point; else it snaps to `position` (ComputeZValue, SetPosition, desired position, vtbl 10Ch).
// The capture-events log line is written when enabled. SetWayPoints(count', points'); [+1F4h] = 1; own copies are freed.
// Original-game quirks kept (issue #23):
//  Q1 the tail search loop (0060E9C2..0060EA2B) always tests points[1] (EBP is never advanced), only the index advances;
//  Q2 the tail copy (0060EA7E..0060EA9C) copies count-i+1 vectors from points[i]: one Vector past the caller's array;
//  Q3 count <= 0 or no area (0060EC93): SetWayPoints receives the points pointer as the count and the `ai` stack word
//     as the points pointer (the original's never-assigned result variables live in those registers / slots);
//  Q4 TestDirectLine's in/out radius {E-58h} = 0.015625 is set only before the first call; later calls reuse whatever
//     the previous call left there.
void __fastcall recovered_client_creature_evaluate_new_way_points_0060e7d0(void* self, void*, U count_arg, void* points_arg,
                                                                           U ai_arg, void* position_arg) noexcept(false) {
    Vector3 out_18 = {0.0f, 0.0f, 0.0f};                               // 0060E7F5..0060E805 {E-18h}
    U found_54 = 0;                                                    // 0060E80D {E-54h}
    U owns_5c = 0;                                                     // 0060E811 {E-5Ch}
    if (rd32(self, 0x214) != 0 && ai_arg != 0 &&                       // 0060E7EB..0060E81D
        static_cast<U>(ld<U8>(self, 0x1EF)) != ai_arg)                 // 0060E81F..0060E828 (movzx vs full stack word)
        return;                                                        // -> 0060ED6D
    const I32 count = static_cast<I32>(count_arg);                     // 0060E831 edi
    const unsigned char* const points = static_cast<const unsigned char*>(points_arg);
    const Vector3* const position = static_cast<const Vector3*>(position_arg);   // {E+10h}
    U new_count;                                                       // ebp at 0060EC97
    U new_points;                                                      // {E+0Ch} at 0060ED44
    void* const area = rp(self, 0x20);                                 // 0060E83D
    if (count <= 0 || area == nullptr) {                               // 0060E835..0060E842 (jle / je)
        new_count = word_of(points_arg);                               // 0060EC93 ebp = {E+8} (Q3)
        new_points = ai_arg;                                           // {E+0Ch} untouched (Q3)
    } else {
        Vector3 start_3c, end_48;                                      // {E-3Ch}, {E-48h}
        float radius_58;                                               // {E-58h}
        const U id = rd32(self, 4);                                    // 0060E892
        const float band = fbits(kLineBand_3c800000);
        start_3c = ld<Vector3>(self, 0x24);                            // 0060E84C..0060E861
        end_48 = ld<Vector3>(points, 0);                               // 0060E867..0060E88E
        radius_58 = fbits(kLineBand_3c800000);                         // 0060E897
        if (w5_client_area_test_direct_line_00604c40(area, nullptr, id, &start_3c, &end_48, &radius_58, band, 0,
                                                     nullptr) == 1) {  // 0060E865..0060E8A7
            new_points = word_of(points_arg);                          // 0060E8A9 {E+0Ch} = points
            found_54 = 1;                                              // 0060E8B1
            owns_5c = 0;                                               // 0060E8B5
            new_count = static_cast<U>(count);                         // 0060E8BD ebp = edi
        } else {
            start_3c = ld<Vector3>(self, 0x24);                        // 0060E8C4..0060E8DB
            end_48 = ld<Vector3>(points, 0);                           // 0060E8DF..0060E90A
            if (w5_client_area_test_direct_line_00604c40(rp(self, 0x20), nullptr, rd32(self, 4), &start_3c, &end_48,
                                                         &radius_58, band, 0, nullptr) == 1) {   // 0060E8D5..0060E91E (Q4)
                const U n = static_cast<U>(count) + 1;                 // 0060E924 ebp
                found_54 = 1;                                          // 0060E92F
                owns_5c = 1;                                           // 0060E933
                void* const array = new_object(n * 12u, [n](void* m) {   // 0060E92E..0060E95D (EH state 0)
                    vector_constructor_iterator_00401130(m, 0xC, n, &vector_default_construct_00409e70);
                    return m;
                });
                copy_movs(array, position, 12);                        // 0060E95F..0060E976 [array] = *position
                copy_movs(static_cast<unsigned char*>(array) + 12, points, static_cast<U>(count) * 12u);   // 0060E979..0060E98E
                new_points = word_of(array);                           // 0060E9A0
                new_count = n;
            } else {
                // 0060E9A6..0060EA2B: tail search
                I32 i = 1;                                             // 0060E9B0 {E+4} = 1
                bool tail_found = false;
                while (i < count) {                                    // 0060E9C2..0060E9CB (jge)
                    start_3c = ld<Vector3>(self, 0x24);                // 0060E9D1..0060E9E6
                    end_48 = ld<Vector3>(points, static_cast<U>(i) * 12u);   // 0060E9EC..0060EA15 ebp advances by 0Ch per pass: points[i] (review U1)
                    if (w5_client_area_test_direct_line_00604c40(rp(self, 0x20), nullptr, rd32(self, 4), &start_3c,
                                                                 &end_48, &radius_58, band, 0, nullptr) == 1) {   // 0060EA1D..0060EA25
                        tail_found = true;
                        break;
                    }
                    ++i;                                               // 0060EA27
                }
                if (tail_found) {
                    const U n = static_cast<U>(count - i + 1);         // 0060EA2D..0060EA34 ebp
                    const U bytes = n * 12u;                           // 0060EA36..0060EA3A edi
                    owns_5c = 1;                                       // 0060EA3E
                    void* const array = live_operator_new(bytes);      // 0060EA3D..0060EA4B
                    if (array != nullptr) {                            // 0060EA4E..0060EA50
                        for (I32 k = 0; k <= static_cast<I32>(n) - 1; ++k) {   // 0060EA52..0060EA78 inline Vector()
                            float* const v = reinterpret_cast<float*>(static_cast<unsigned char*>(array) + static_cast<U>(k) * 12u);
                            v[0] = 0.0f; v[1] = 0.0f; v[2] = 0.0f;
                        }
                    }
                    copy_movs(array, points + static_cast<U>(i) * 12u, bytes);   // 0060EA7E..0060EA9C (Q2)
                    new_points = word_of(array);                       // 0060EA89 {E+0Ch}
                    new_count = n;
                } else {
                    // 0060EAA3: the destination reaches the path's last point -> remaining own waypoints + last point
                    using GetDestination = U (__fastcall*)(void*, void*);
                    bool snap = true;
                    if (vslot<GetDestination>(self, 0x150)(self, nullptr) != 0) {   // 0060EAA3..0060EAAF
                        Vector3 dest_24 = {0.0f, 0.0f, 0.0f};          // 0060EAB9..0060EAC9 {E-24h}
                        Vector3 last_30 = {0.0f, 0.0f, 0.0f};          // 0060EAD1..0060EAE1 {E-30h}
                        const U d = vslot<GetDestination>(self, 0x150)(self, nullptr);   // 0060EAE9
                        if (d != 0) {                                  // 0060EAEF..0060EAF1
                            dest_24 = ld<Vector3>(word_ptr(d), 0);     // 0060EAF7..0060EAFC
                            last_30 = ld<Vector3>(points, static_cast<U>(count - 1) * 12u);   // 0060EAFF..0060EB0F
                        }
                        start_3c = dest_24;                            // 0060EB2C..0060EB4B
                        end_48 = last_30;                              // 0060EB43..0060EB5F
                        if (w5_client_area_test_direct_line_00604c40(rp(self, 0x20), nullptr, rd32(self, 4), &start_3c,
                                                                     &end_48, &radius_58, band, 0, nullptr) == 1) {   // 0060EB63..0060EB6B
                            snap = false;
                            owns_5c = 1;                               // 0060EB71
                            U16 way_count = 0, way_index = 0;          // {E+10h} / {E+8} low words
                            void* way_points = nullptr;                // {E+4}
                            recovered_client_creature_get_way_points_0060c4e0(self, nullptr, &way_count, &way_points,
                                                                              &way_index);   // 0060EB75..0060EB89
                            const U n = static_cast<U>(static_cast<I32>(way_count) - static_cast<I32>(way_index) + 1);   // 0060EB8E..0060EB9A
                            const U bytes = n * 12u;                   // 0060EB9B..0060EB9F
                            void* const array = new_object(bytes, [n](void* m) {   // 0060EBA2..0060EBD4 (EH state 1)
                                vector_constructor_iterator_00401130(m, 0xC, n, &vector_default_construct_00409e70);
                                return m;
                            });
                            copy_movs(array, static_cast<const unsigned char*>(way_points) + static_cast<U>(way_index) * 12u,
                                      bytes - 12u);                    // 0060EBD8..0060EBF3
                            copy_movs(static_cast<unsigned char*>(array) + bytes - 12u, &end_48, 12);   // 0060EBF5..0060EC1A
                            new_points = word_of(array);               // 0060EBFD
                            new_count = n;
                        }
                    }
                    if (snap) {
                        // 0060EC23: snap to `position`
                        Vector3 first_24 = ld<Vector3>(points, 0);     // 0060EC23..0060EC42 {E-24h}
                        owns_5c = 0;                                   // 0060EC4E
                        w5_client_creature_compute_z_value_0060bf20(self, nullptr, rp(self, 0x20), &first_24, first_24.z,
                                                                    word_of(&out_18));   // 0060EC39..0060EC56
                        new_points = word_of(points_arg);              // 0060EC5D {E+0Ch} = points
                        vslot<void (__fastcall*)(void*, void*, const Vector3*)>(self, 0x8C)(self, nullptr, position);   // 0060EC5B..0060EC66
                        st<Vector3>(self, 0x1DC, *position);           // 0060EC6C..0060EC81
                        vslot<void (__fastcall*)(void*, void*, Vector3*)>(self, 0x10C)(self, nullptr, &out_18);   // 0060EC84..0060EC8B
                        new_count = static_cast<U>(count);             // 0060EC64 ebp = edi
                    }
                }
            }
        }
    }
    (void)found_54;
    // 0060EC97: capture-events log line
    if (kotor_fastcall_005ed470(client_app(), nullptr) == 1) {        // 0060EC97..0060ECAC
        ExoString line_50;                                             // {E-50h}
        construct_string(&line_50);                                    // 0060ECB2..0060ECB6
        {
            Unwind guard(&destroy_string_void, &line_50);              // 0060ECD1 EH state 2
            const double z = ld<float>(self, 0x2C);                    // 0060ECBB / 0060ECCD fstp qword
            const double y = ld<float>(self, 0x28);                    // 0060ECDC..0060ECDF
            const double x = ld<float>(self, 0x24);                    // 0060ECE3..0060ECE6
            const U id = rd32(self, 4);                                // 0060ECBE / 0060ECE9
            const std::uint64_t snapshot =
                kotor_fastcall_004adc70(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr);   // 0060ECEC..0060ECF3
            const U seconds = static_cast<U>(snapshot / 1000u);        // 0060ECF8..0060ED01 __aulldiv, low dword pushed
            kotor_reconstruction::StartupFormat005e5680(ss(&line_50), kCaptureFormat_0074f3ac, seconds, 0x59, id, x, y,
                                                        z);            // 0060ED06..0060ED11
            void* const file = kotor_fastcall_005ed460(client_app(), nullptr);   // 0060ED16..0060ED27
            frame_LogWriteString_005e6a70(file, nullptr, &line_50);    // 0060ED22 / 0060ED2C..0060ED2E
            guard.dismiss();
        }
        destroy_string(&line_50);                                      // 0060ED33..0060ED3F
    }
    recovered_client_creature_set_way_points_0060c790(self, nullptr, new_count, new_points);   // 0060ED44..0060ED4C
    st<U>(self, 0x1F4, 1);                                             // 0060ED55
    if (owns_5c == 1 && new_points != 0) live_free(word_ptr(new_points));   // 0060ED51..0060ED62
}

// =========================================================================================== 'U' readers
// 0064DBC0 CSWCMessage::HandleServerToPlayerUpdateVisualEffects(CSWCObject* object). RET 4. WORD n, then n records
// {BYTE 'A'/'D', WORD effect, [beam: OBJECTID target, BYTE]}; 'A' adds (vtbl B4h), 'D' removes (vtbl B8h) on a non-null
// object. EAX 0 on a read overflow.
U __fastcall recovered_client_handle_update_visual_effects_0064dbc0(void* self, void*, void* object) noexcept(false) {
    U16 remaining = rw(self);                                          // 0064DBC5..0064DBD4 {E-10h}
    if (overflow(self) != 0) return 0;                                 // 0064DBCE..0064DBE8
    while (remaining != 0) {                                           // 0064DBEB..0064DBF0 / 0064DCBF..0064DCCF
        const U8 kind = rb(self);                                      // 0064DC00..0064DC0D (stored over the arg slot)
        const U16 effect = rw(self);                                   // 0064DC09..0064DC1A ebx
        if (overflow(self) != 0) return 0;                             // 0064DC16..0064DC23
        U target;
        U8 extra;                                                      // {E-14h} (low byte written; see D-note below)
        if (w2::g07::is_beam_type_004ca960(effect) != 0) {             // 0064DC29..0064DC34 (cdecl)
            target = rid(self);                                        // 0064DC36..0064DC41
            extra = rb(self);                                          // 0064DC3D..0064DC48
        } else {
            target = kInvalidObject_00746644;                          // 0064DC4E
            extra = 0;                                                 // 0064DC53
        }
        if (overflow(self) != 0) return 0;                             // 0064DC58..0064DC63
        if (kind == 0x41) {                                            // 0064DC65..0064DC6B
            if (object != nullptr) {                                   // 0064DC6D..0064DC6F
                Vector3 zero = {0.0f, 0.0f, 0.0f};                     // 0064DC8B..0064DC9B {E-0Ch}
                // D-note: the original pushes the whole dword {E-14h} whose upper 3 bytes are stale; the BYTE is passed
                // zero-extended here.
                using AddEffect = U (__fastcall*)(void*, void*, U, U, U, U, U, U, Vector3*);
                vslot<AddEffect>(object, 0xB4)(object, nullptr, effect, 0, target, kInvalidObject_00746644, extra, 0,
                                               &zero);                 // 0064DC71..0064DCA3
            }
        } else if (kind == 0x44) {                                     // 0064DCAB..0064DCAD
            if (object != nullptr)                                     // 0064DCAF..0064DCB1
                vslot<U (__fastcall*)(void*, void*, U)>(object, 0xB8)(object, nullptr, effect);   // 0064DCB3..0064DCB9
        }
        remaining = static_cast<U16>(remaining - 1);                   // 0064DCBF..0064DCCB
    }
    return 1;                                                          // 0064DCD5..0064DCE1
}

// 00651880 CSWCMessage::HandleServerToPlayerGenericObjectUpdate_Update(id, mask). RET 8. Common 'U' fields of every
// non-creature object: 1 position, 2 orientation, 20h portrait (WORD [+ CResRef when >= FFFEh]), 4 animation speed +
// WORD animation (doors map 2726h/2742h/2743h to CSWCDoor::SetState 0/1/2), 8 visual effects. EAX 0 on overflow.
U __fastcall recovered_client_handle_generic_object_update_00651880(void* self, void*, U id, U mask) {
    ResRef portrait_ref;                                               // {E-30h}
    kotor_fastcall_00405ed0(&portrait_ref, nullptr);                   // 00651889..0065188D
    Vector3 position = {0.0f, 0.0f, 0.0f};                             // 0065189B..006518AB {E-48h}
    Vector3 orientation = {0.0f, 0.0f, 0.0f};                          // 006518B3..006518C3 {E-3Ch}
    void* const object = kotor_fastcall_005ed580(client_app(), nullptr, id);   // 00651892..006518D1 (ebx)
    void* target = nullptr;                                            // 006518CF xor esi,esi
    if ((mask & 1) != 0) {                                             // 006518D6..006518DF (ebp)
        position.x = rf(self); position.y = rf(self); position.z = rf(self);   // 006518E1..00651913
    }
    if ((mask & 2) != 0) {                                             // 00651917..00651922 {E-58h}
        orientation.x = rf(self); orientation.y = rf(self); orientation.z = rf(self);   // 00651924..00651956
    }
    U16 portrait = 0;                                                  // {E-50h}
    if ((mask & 0x20) != 0) {                                          // 0065195A..00651965 {E-54h}
        portrait = rw(self);                                           // 00651967..00651974
        if (portrait >= 0xFFFE) {                                      // 00651970..00651978
            ResRef read_20;                                            // {E-20h}
            ResRef out_10;                                             // {E-10h} hidden-return temporary
            c2da_copy_00406120(&portrait_ref, nullptr, &out_10,
                               kotor_fastcall_004d6180(self, nullptr, &read_20, 0x10));   // 0065197A..00651992
        }
    }
    U speed_bits = 0;                                                  // {E-4Ch}
    U16 animation = 0;                                                 // {E-5Ch}
    if ((mask & 4) != 0) {                                             // 00651997..006519A2 {E+4} reused
        speed_bits = fb(rf(self));                                     // 006519A4..006519B2
        animation = rw(self);                                          // 006519BE..006519C7
    }
    if (overflow(self) != 0) return 0;                                 // 006519CB..006519D6 -> 00651AC7
    using AsObject = void* (__fastcall*)(void*, void*);
    if (object != nullptr && vslot<AsObject>(object, 0x0C)(object, nullptr) != nullptr) {   // 006519DC..006519ED
        target = vslot<AsObject>(object, 0x0C)(object, nullptr);       // 006519F3..006519FC
        if ((mask & 1) != 0)                                           // 006519FA..006519FE
            vslot<void (__fastcall*)(void*, void*, Vector3*)>(target, 0x8C)(target, nullptr, &position);   // 00651A00..00651A09
        if ((mask & 2) != 0)                                           // 00651A0F..00651A15
            vslot<void (__fastcall*)(void*, void*, Vector3*)>(target, 0x88)(target, nullptr, &orientation);   // 00651A17..00651A20
        if ((mask & 0x20) != 0) {                                      // 00651A26..00651A2C
            vslot<void (__fastcall*)(void*, void*, U)>(target, 0xF4)(target, nullptr, portrait);   // 00651A2E..00651A37
            if (portrait >= 0xFFFE)                                    // 00651A3D..00651A42
                vslot<void (__fastcall*)(void*, void*, ResRef*)>(target, 0xEC)(target, nullptr, &portrait_ref);   // 00651A44..00651A4D
        }
        if ((mask & 4) != 0) {                                         // 00651A53..00651A59
            st<U>(target, 0x4C, speed_bits);                           // 00651A5B..00651A66
            if (ld<U8>(target, 8) == kDoor_00746639) {                 // 00651A5F..00651A6F
                using AsDoor = void* (__fastcall*)(void*, void*);
                if (animation == 0x2726)                               // 00651A71..00651A7A
                    recovered_client_door_set_state_00683d40(vslot<AsDoor>(target, 0x14)(target, nullptr), nullptr, 0);   // 00651A78 / 00651A97..00651AA0
                else if (animation == 0x2742)                          // 00651A7C..00651A81
                    recovered_client_door_set_state_00683d40(vslot<AsDoor>(target, 0x14)(target, nullptr), nullptr, 1);   // 00651A83..00651AA0
                else if (animation == 0x2743)                          // 00651A8E..00651A93
                    recovered_client_door_set_state_00683d40(vslot<AsDoor>(target, 0x14)(target, nullptr), nullptr, 2);   // 00651A95..00651AA0
            }
            vslot<U (__fastcall*)(void*, void*, U, U, U)>(target, 0x9C)(target, nullptr, animation, 0, 0);   // 00651AA5..00651AAE
        }
    }
    if ((mask & 8) != 0 &&                                             // 00651AB4..00651AB9
        recovered_client_handle_update_visual_effects_0064dbc0(self, nullptr, target) == 0)   // 00651ABB..00651AC5
        return 0;                                                      // 00651AC7..00651AD0
    return 1;                                                          // 00651AD3..00651ADF
}

// 0064D420 CSWCMessage::HandleServerToPlayerDoorUpdate_Update(id, mask). RET 8. EAX 0 when the door is unknown or the
// flag-10h BOOLs overflow. 10h: BOOL +114h, BOOL "usable" (sets +110h; vtbl-less 0060E760 notify on a change of +120h),
// BOOL +108h, BOOL +104h.
U __fastcall recovered_client_handle_door_update_0064d420(void* self, void*, U id, U mask) {
    void* const door = recovered_client_get_door_by_id_005ed600(client_app(), nullptr, id);   // 0064D420..0064D431
    if (door == nullptr) return 0;                                     // 0064D436..0064D43E (EAX = esi = 0)
    if ((mask & 0x10) == 0) return 1;                                  // 0064D441..0064D448 -> 0064D4EA
    const U b1 = rbool(self);                                          // 0064D44E..0064D457 ebx
    const U b2 = rbool(self);                                          // 0064D455..0064D460 ebp
    const U b3 = rbool(self);                                          // 0064D45E..0064D469 {E+4}
    const U b4 = rbool(self);                                          // 0064D467..0064D476 {E+8}
    if (overflow(self) != 0) return 0;                                 // 0064D472..0064D489
    st<U>(door, 0x108, b3);                                            // 0064D48E..0064D496
    st<U>(door, 0x114, b1);                                            // 0064D4A2
    st<U>(door, 0x104, b4);                                            // 0064D492..0064D4A8
    if (b2 != 0) {                                                     // 0064D48C / 0064D4AE
        if (rd32(door, 0x120) == 0) slot_void_return_4_0060e760_a1(door, nullptr, 1);   // 0064D49C..0064D4B8
        st<U>(door, 0x110, 1);                                         // 0064D4C0
    } else {
        if (rd32(door, 0x120) != 0) slot_void_return_4_0060e760_a1(door, nullptr, 0);   // 0064D4D3..0064D4DB
        st<U>(door, 0x110, 0);                                         // 0064D4E0
    }
    return 1;                                                          // 0064D4BD..0064D4D0 / 0064D4EA..0064D4F3
}

// 0064DAE0 CSWCMessage::HandleServerToPlayerPlaceableUpdate_Update(id, mask). RET 8. The BOOLs are read (and checked
// for overflow) even when the placeable is unknown. 10h: BOOL +124h, BOOL "usable" (+11Ch; 0060E760 on a change of
// +13Ch), BOOL +118h, BOOL +110h.
U __fastcall recovered_client_handle_placeable_update_0064dae0(void* self, void*, U id, U mask) {
    void* const placeable = recovered_client_get_placeable_by_id_005ed5f0(client_app(), nullptr, id);   // 0064DAE0..0064DAF3 (edi)
    if ((mask & 0x10) == 0) return 1;                                  // 0064DAFA..0064DAFF -> 0064DBA5
    const U b1 = rbool(self);                                          // 0064DB05..0064DB0E ebx
    const U b2 = rbool(self);                                          // 0064DB0C..0064DB17 ebp
    const U b3 = rbool(self);                                          // 0064DB15..0064DB20 {E+4}
    const U b4 = rbool(self);                                          // 0064DB1E..0064DB2D {E+8}
    if (overflow(self) != 0) return 0;                                 // 0064DB29..0064DB40
    if (placeable == nullptr) return 1;                                // 0064DB43..0064DB45 -> 0064DBA5
    st<U>(placeable, 0x118, b3);                                       // 0064DB49..0064DB51
    st<U>(placeable, 0x124, b1);                                       // 0064DB5D
    st<U>(placeable, 0x110, b4);                                       // 0064DB4D..0064DB63
    if (b2 != 0) {                                                     // 0064DB47 / 0064DB69
        if (rd32(placeable, 0x13C) == 0) slot_void_return_4_0060e760_a1(placeable, nullptr, 1);   // 0064DB57..0064DB73
        st<U>(placeable, 0x11C, 1);                                    // 0064DB78
    } else {
        if (rd32(placeable, 0x13C) != 0) slot_void_return_4_0060e760_a1(placeable, nullptr, 0);   // 0064DB8E..0064DB96
        st<U>(placeable, 0x11C, 0);                                    // 0064DB9B
    }
    return 1;                                                          // 0064DB82..0064DB8B / 0064DBA5..0064DBAE
}

// 006655E0 CSWCMessage::HandleServerToPlayerCreatureUpdate_Update(id, mask). RET 8. EAX 0 on a read overflow, else 1.
// Field order (lane-w2 t4 writer 00574A10 mirrors it): 1 position; 2 orientation + BYTE [OBJECTID lock target];
// 20h portrait; 4 FLOAT speed, WORD animation [attack records | DWORD + OBJECTID | BYTE kind ...]; 8 visual effects;
// 4 BYTE ai, WORD path count [BOOL [FLOAT]] + path points; 40h; 100h; 40000h; 200h; 400h; 80000h; 10000h; 4000h; 8000h;
// 20000h; 800h; 1000h; 2000h (no data). Original stack slots are named by their entry-relative offset {E-x}.
// Domain note: the attack-record arrays of the original frame hold 3 records (the OBJECTID array {E-18h} reaches the
// exception registration at {E-0Ch} with a 4th); larger counts corrupt the original frame and are out of domain. The
// arrays here are sized for the full BYTE range.
U __fastcall recovered_client_handle_creature_update_006655e0(void* self, void*, U id, U mask) {
    using V0 = U (__fastcall*)(void*, void*);
    using V1 = U (__fastcall*)(void*, void*, U);
    using V3 = U (__fastcall*)(void*, void*, U, U, U);
    using VP = U (__fastcall*)(void*, void*, const void*);
    ResRef portrait_ref;                                               // {E-38h}
    kotor_fastcall_00405ed0(&portrait_ref, nullptr);                   // 00665601..00665608
    Vector3 position = {0.0f, 0.0f, 0.0f};                             // 0066561B..0066562B {E-C0h}
    Vector3 orientation = {0.0f, 0.0f, 0.0f};                          // 00665633..00665643 {E-98h}
    U animation = 0;                                                   // 00665619 ebp / 0066564F {E-D0h}
    U has_speed = 0;                                                   // 00665653 {E-B4h}
    U path_speed_bits = fb(1.0f);                                      // 00665657 {E-74h}
    U8 attack_count = 0;                                               // 00665662 {E-E9h}
    Vector3 target_position = {0.0f, 0.0f, 0.0f};                      // 00665667..0066566F {E-A8h}
    void* const c = frame_R4_005ed5b0(client_app(), nullptr, id);      // 0066560D..00665673 (esi)
    void* const stats = c != nullptr ? rp(c, 0x2F8) : nullptr;         // 00665678..0066568A {E-9Ch}
    const U flag1 = mask & 1;                                          // 0066568E..0066569A {E-70h}
    if (flag1 != 0) {                                                  // 006656A1
        position.x = rf(self); position.y = rf(self); position.z = rf(self);   // 006656A3..006656D5
    } else if (c != nullptr) {                                         // 006656DB..006656DD
        position = ld<Vector3>(c, 0x24);                               // 006656DF..006656F2
    }
    const U flag2 = mask & 2;                                          // 006656F6..006656FB {E-D4h}
    U lock_target = 0;                                                 // {E-68h} (written only under flag 2)
    if (flag2 != 0) {                                                  // 006656FF
        orientation.x = rf(self); orientation.y = rf(self); orientation.z = rf(self);   // 00665705..00665737
        lock_target = rb(self) != 0 ? rid(self) : kInvalidObject_00746644;   // 0066573B..0066575E
        // fcomp + "test ah,5" + jp: falls through only for an ordered |orientation|^2 < epsilon
        if (sq3_zyx(orientation.x, orientation.y, orientation.z) < static_cast<double>(kEpsilon_00747504))   // 00665765..0066578C
            orientation.x = 1.0f;                                      // 0066578E
    } else if (c != nullptr) {                                         // 00665798..0066579A
        orientation = ld<Vector3>(c, 0x30);                            // 0066579C..006657AF
    }
    if ((mask & 0x20) != 0) {                                          // 006657B3..006657B6
        ExoString s_b0, s_e8;                                          // {E-B0h}, {E-E8h}
        bool failed;
        construct_string(&s_b0);                                       // 006657BC..006657C0
        {
            Unwind guard_b0(&destroy_string_void, &s_b0);              // 006657C9 EH state 0
            construct_string(&s_e8);                                   // 006657C5..006657D4
            {
                Unwind guard_e8(&destroy_string_void, &s_e8);          // 006657DD EH state 1
                const U16 portrait = rw(self);                         // 006657D9..006657EE ebx
                failed = overflow(self) != 0;                          // 006657EA..006657F7
                if (!failed) {
                    if (c != nullptr) vslot<V1>(c, 0xF4)(c, nullptr, portrait);   // 006657F9..00665802
                    if (portrait >= 0xFFFE) {                          // 00665808..0066580D
                        ResRef read_60, out_28;                        // {E-60h}, {E-28h}
                        c2da_copy_00406120(&portrait_ref, nullptr, &out_28,
                                           kotor_fastcall_004d6180(self, nullptr, &read_60, 0x10));   // 00665813..00665834
                        failed = overflow(self) != 0;                  // 00665839..00665844
                        if (!failed && c != nullptr) {                 // 00665872..00665874
                            (void)recovered_client_creature_stats_check_custom_portrait_00647c10(&portrait_ref,
                                                                                                 portrait == 0xFFFE ? 1 : 0);   // 0066587A..00665894 (cdecl)
                            vslot<VP>(c, 0xEC)(c, nullptr, &portrait_ref);   // 00665897..006658A1
                        }
                    } else {
                        c2da_assign_text_005e5140(&s_b0, nullptr, kPortraitPrefix_00745e4c);   // 006658AC..006658B5 = "po_"
                        ExoString column_7c;                           // {E-7Ch}
                        frame_LogStringCtor_005e5a90(&column_7c, nullptr, kBaseResRef_00745e40);   // 006658BA..006658C6
                        {
                            Unwind guard_7c(&destroy_string_void, &column_7c);   // 006658ED EH state 2
                            void* const portraits = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x38);   // 006658CB..006658D7
                            (void)c2da_current_get_string_00413270(portraits, nullptr, portrait, &column_7c, &s_e8);   // 006658DA..006658F5
                            guard_7c.dismiss();
                        }
                        destroy_string(&column_7c);                    // 006658FA..00665909 (state 1)
                        ExoString sum_7c;                              // {E-7Ch} operator+ hidden return
                        GuiConcat005e5d10(ss(&s_b0), nullptr, ss(&sum_7c), ss(&s_e8));   // 0066590E..0066591F
                        {
                            Unwind guard_sum(&destroy_string_void, &sum_7c);   // 00665929 EH state 3
                            async_assign_string_005e5c50(&s_e8, nullptr, &sum_7c);   // 00665924..00665931
                            guard_sum.dismiss();
                        }
                        destroy_string(&sum_7c);                       // 00665936..00665945 (state 1)
                        ResRef out_60;                                 // {E-60h}
                        unresolved_ref_assign_00406290(&portrait_ref, nullptr, &out_60,
                                                       async_string_cstr_005e5670(&s_e8, nullptr));   // 0066594A..00665963
                    }
                }
                guard_e8.dismiss();
            }
            destroy_string(&s_e8);                                     // 00665968..00665974 / 00665846..00665852 (state 0)
            guard_b0.dismiss();
        }
        destroy_string(&s_b0);                                         // 00665979..00665988 / 00665857..00665866 (state -1)
        if (failed) return 0;                                          // 0066586B..0066586D
    }
    if (c != nullptr) animation = kotor_m15a_object_get_current_animation_0063c060(c, nullptr);   // 00665994..006659A1
    const U flag4 = mask & 4;                                          // 006659A5..006659A8 {E-C4h}
    if (flag4 != 0) {                                                  // 006659AC
        const U speed = fb(rf(self));                                  // 006659AE..006659BC {E-DCh}
        if (overflow(self) != 0) return 0;                             // 006659C0..006659CB
        if (c != nullptr) st<U>(c, 0x4C, speed);                       // 006659D1..006659D9
    }
    U animation_read = 1;                                              // 006659E2 {E-8Ch}
    // attack records (animation 2719h / 277Dh)
    U ids_18[256]; U16 w12_7c[256], w14_b0[256], w08_40[256], w64_48[256], dmg_e8[256];
    U8 b5c_50[256], b3a_4c[256]; U b40_28[256], b48_60[256];
    U8 attack_kind = 0;                                                // {E-C8h} (BYTE; written only for the 2721h.. set)
    U attack_target = 0;                                               // {E-CCh}
    U attack_dword_dc = 0, attack_dword_f0 = 0;                        // {E-DCh}, {E-F0h}
    Vector3 scratch_88 = {0.0f, 0.0f, 0.0f};                           // {E-88h}
    if (flag4 != 0) {                                                  // 006659DC..006659EA
        animation = rw(self);                                          // 006659F0..00665A00 (ebp, {E-D0h})
        const U16 a = static_cast<U16>(animation);
        if (a == 0x2719 || a == 0x277D) {                              // 006659FB..00665A0B
            attack_count = rb(self, 2);                                // 00665A11..00665A21 ReadBYTE(2)
            for (I32 i = 0; i < static_cast<I32>(attack_count); ++i) { // 00665A1D..00665ACC
                ids_18[i] = rid(self);                                 // 00665A30..00665A3B
                w12_7c[i] = rw(self);                                  // 00665A37..00665A4B
                w14_b0[i] = rw(self);                                  // 00665A47..00665A5C
                w08_40[i] = rw(self);                                  // 00665A58..00665A6A
                b5c_50[i] = rb(self, 4);                               // 00665A66..00665A7B
                w64_48[i] = rw(self);                                  // 00665A77..00665A8B
                dmg_e8[i] = rw(self, 9);                               // 00665A87..00665A9A ReadWORD(9)
                b40_28[i] = rbool(self);                               // 00665A98..00665AA6
                b48_60[i] = rbool(self);                               // 00665AAD..00665AB6
                b3a_4c[i] = rb(self, 4);                               // 00665AB2..00665AC2
            }
            if (overflow(self) != 0) return 0;                         // 00665AD2..00665ADD
        }
        if (a == 0x271F || a == 0x2720 || a == 0x2AF8 || a == 0x2721 || a == 0x2722 || a == 0x2723 || a == 0x2724 ||
            a == 0x274D || a == 0x28A1) {                              // 00665AE7..00665B24
            attack_dword_dc = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 00665B26..00665B31
            const U object = rid(self);                                // 00665B35..00665B3E ebx
            if (overflow(self) != 0) return 0;                         // 00665B3A..00665B47
            if (c != nullptr) { st<U>(c, 0x2C4, attack_dword_dc); st<U>(c, 0x2C8, object); }   // 00665B4D..00665B5B
        }
        if (a == 0x2721 || a == 0x2722 || a == 0x2723 || a == 0x2724 || a == 0x274D || a == 0x28A1) {   // 00665B61..00665B89
            attack_kind = rb(self);                                    // 00665B8F..00665B9E
            if (attack_kind == 1) {                                    // 00665B98..00665B9E
                attack_target = rid(self);                             // 00665BA0..00665BA7
            } else if (attack_kind == 2) {                             // 00665BAD..00665BAF
                attack_target = kInvalidObject_00746644;               // 00665BBA
                const float x = rf(self);                              // 00665BB1..00665BC7 {E-D8h}
                const float y = rf(self);                              // 00665BCB..00665BD9 {E-F0h}
                const float z = rf(self);                              // 00665BDD..00665BEB {E-80h}
                scratch_88 = {x, y, z};                                // 00665BF2..00665C02
                target_position = {x, y, z};                           // 00665C10..00665C18
            }
            attack_dword_f0 = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 00665C1C..00665C29
            if (overflow(self) != 0) return 0;                         // 00665C25..00665C38
        }
    } else if (c != nullptr) {                                         // 00665C3D..00665C3F
        animation_read = 0;                                            // 00665C43
        animation = kotor_m15a_object_get_current_animation_0063c060(c, nullptr);   // 00665C41..00665C52
    }
    if ((mask & 8) != 0 &&                                             // 00665C56..00665C5E
        recovered_client_handle_update_visual_effects_0064dbc0(self, nullptr, c) == 0)   // 00665C60..00665C6A
        return 0;
    U8 ai_byte = 0;                                                    // {E-F1h}
    if (flag4 != 0) {                                                  // 00665C70..00665C76
        ai_byte = rb(self);                                            // 00665C78..00665C83
        if (c == nullptr) goto skip_path_6665ac;                       // 00665C81..00665C87
        st<U8>(c, 0x1F0, ai_byte);                                     // 00665C8D
    } else {
        ai_byte = 0;                                                   // 00665C97
        if (c == nullptr) goto skip_path_6665ac;                       // 00665C95..00665C9C
    }
    {
        // ------------------------------------------------------------ creature present (00665CA2)
        if (flag2 != 0) {                                              // 00665CA2..00665CA8
            kotor_fastcall_0060b3c0(c, nullptr, &orientation);         // 00665CAA..00665CB1 (lane-w4)
            if (rd32(c, 0x214) != 0 &&                                 // 00665CB6..00665CBE
                rd32(c, 4) != kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr))   // 00665CC0..00665CD3
                recovered_client_creature_set_moving_orientation_0060b380(c, nullptr, &orientation);   // 00665CD5..00665CDC
            recovered_client_creature_set_lock_orientation_00610950(c, nullptr, lock_target, 0);   // 00665CE1..00665CED
        }
        const U current = kotor_m15a_object_get_current_animation_0063c060(c, nullptr);   // 00665CF2..00665CFB (ebx)
        const U16 a = static_cast<U16>(animation);
        if (kotor_m15a_object_animation_stationary_0063c170(c, nullptr, current) == 0 &&   // 00665CFD..00665D05
            kotor_m15a_object_animation_stationary_0063c170(c, nullptr, animation) == 1 &&   // 00665D0B..00665D16
            kotor_m15a_object_animation_looping_0063c300(c, nullptr, animation) == 1 &&   // 00665D1C..00665D27
            a != 0x2716 && a != 0x2718 && a != 0x27AC && a != 0x272E && a != 0x27AB && a != 0x2736 && a != 0x27AA &&
            a != 0x2737 && a != 0x2738 && a != 0x2739 && a != 0x273A) {   // 00665D2D..00665D94
            kotor_m15a_object_set_looping_animation_0063d830(c, nullptr, animation);   // 00665D96..00665D99
            void* const destination = word_ptr(vslot<V0>(c, 0x150)(c, nullptr));   // 00665D9E..00665DA2
            if (destination != nullptr) {                              // 00665DA8..00665DAA
                if (flag1 == 0) goto skip_path_6665ac;                 // 00665DAC..00665DB5
                kotor_cdecl_00423bc0(&scratch_88.x, static_cast<const float*>(destination), &position.x);   // 00665DBB..00665DC9
                if (le(sq2(scratch_88.x, scratch_88.y), kEpsilon_00747504)) goto skip_path_6665ac;   // 00665DCE..00665DFA
            }
        }
        // 00665E00
        Vector3* const here = static_cast<Vector3*>(at(c, 0x24));
        if ((kotor_m15a_object_animation_stationary_0063c170(c, nullptr, current) == 1 &&   // 00665E00..00665E0B
             kotor_m15a_object_animation_stationary_0063c170(c, nullptr, animation) == 0 &&   // 00665E0D..00665E17
             le(recovered_dot_product_00423c20(here, here), kEpsilon_00747504)) ||   // 00665E19..00665E31
            rd32(c, 0x9C) == 1) {                                      // 00665E33..00665E3A
            if (flag1 != 0) {                                          // 00665E3C..00665E45
                vslot<VP>(c, 0x8C)(c, nullptr, &position);             // 00665E47..00665E50
                st<Vector3>(c, 0x1DC, position);                       // 00665E56..00665E6D
            }
            st<U>(c, 0x9C, 0);                                         // 00665E70
        }
        if (kotor_fastcall_0063c380(c, nullptr, animation) == 0) {     // 00665E7A..00665E84 (lane-w4)
            if (animation_read != 0) vslot<V3>(c, 0x9C)(c, nullptr, animation, 0, 0);   // 00665E86..00665E9B
        } else if (flag4 != 0) {                                       // 00665EA6..00665EAC
            if (a == 0x2719 || a == 0x277D) {                          // 00665EB2..00665EC2 -> 00665FF4
                void* const first = new_object(0x48, &attack_data_ctor);   // 00665FF4..0066601E {E-C8h} (EH state 4)
                void* record = first;                                  // 0066602B ebx
                const I32 count = attack_count;                        // 00666026 movzx
                for (I32 i = 0; i < count; ++i) {                      // 0066602F..0066610B
                    st<U>(record, 0x04, id);                           // 00666050..00666066
                    st<U16>(record, 0x1E, w64_48[i]);                  // 00666069..00666071
                    st<U16>(record, 0x08, w08_40[i]);                  // 00666057 / 0066607A
                    st<U16>(record, 0x1C, w14_b0[i]);                  // 00666075 / 00666085
                    st<U8>(record, 0x0C, b5c_50[i]);                   // 0066605F / 00666090
                    st<U>(record, 0x14, ids_18[i]);                    // 0066607E / 0066609B
                    st<U>(record, 0x24, b48_60[i]);                    // 00666089 / 006660A3
                    st<U16>(record, 0x1A, w12_7c[i]);                  // 00666093 / 006660AA
                    st<U16>(record, 0x0A, dmg_e8[i]);                  // 0066609E / 006660B5
                    st<U8>(record, 0x00, 1);                           // 006660C0
                    st<U>(record, 0x20, b40_28[i]);                    // 006660AE / 006660C3
                    st<U8>(record, 0x28, b3a_4c[i]);                   // 006660B9 / 006660C6
                    if (i < count - 1) {                               // 00666044..00666045 / 006660A6..006660C9
                        void* const next = new_object(0x48, &attack_data_ctor);   // 006660CB..006660F1 (EH state 5)
                        st<void*>(record, 0x40, next);                 // 006660F3
                        record = next;                                 // 00666101
                    }
                }
                vslot<U (__fastcall*)(void*, void*, U, void*)>(c, 0xA0)(c, nullptr, animation, first);   // 00666111..0066611F
            } else if (a == 0x2721 || a == 0x2722 || a == 0x2723 || a == 0x2724 || a == 0x274D || a == 0x28A1) {
                U play = 0;                                            // 00665EC8..00665F65
                if (a == 0x2721) play = 0x280A;                        // 00665F12..00665F1F
                else if (a == 0x2722) play = 0x280B;                   // 00665F27..00665F2B
                else if (a == 0x2723) play = 0x280C;                   // 00665EFD..00665F0A
                else if (a == 0x2724) play = 0x280D;                   // 00665F39..00665F3D
                else if (a == 0x274D) play = 0x2815;                   // 00665F4B..00665F58
                else play = 0x28A1;                                    // 00665F67..00665F6B
                vslot<V3>(c, 0x9C)(c, nullptr, play, 0, 0);            // 00665F70..00665F74 (and the inline copies)
                void* const data = new_object(0x48, &attack_data_ctor);   // 00665F7A..00665FA0 (EH state 6; NULL faults below, as the original)
                st<U8>(data, 0x01, attack_kind);                       // 00665FA2..00665FAA
                st<U>(data, 0x14, attack_target);                      // 00665FA6..00665FAD
                st<Vector3>(data, 0x2C, target_position);              // 00665FB0..00665FC4
                st<U>(data, 0x38, attack_dword_dc);                    // 00665FC7..00665FCF
                st<U8>(data, 0x00, 4);                                 // 00665FD3
                st<U>(data, 0x3C, attack_dword_f0);                    // 00665FCB..00665FD6
                vslot<U (__fastcall*)(void*, void*, U, void*)>(c, 0xA0)(c, nullptr, animation, data);   // 00665FD2..00665FE9
            }
        }
        // ------------------------------------------------------------ path (00666129)
        U path_count = 0;                                              // ebx (low WORD meaningful)
        bool evaluate = true;                                          // false -> SetWayPoints(0, 0) / 006661F9
        if (kotor_m15a_object_animation_stationary_0063c170(c, nullptr, animation) == 0) {   // 00666129..00666137
            if (flag4 != 0) {                                          // 00666139..0066613B
                path_count = rw(self);                                 // 0066613D..00666146
                if (static_cast<U16>(path_count) != 0) {               // 00666148..0066614B
                    has_speed = rbool(self);                           // 0066614D..00666156
                    if (has_speed != 0) path_speed_bits = fb(rf(self));   // 0066615A..0066616A
                }
            } else {
                U16 way_count = 0, way_index = 0;                      // {E-F0h}, {E-C8h}
                void* way_points = nullptr;                            // {E-D8h}
                recovered_client_creature_get_way_points_0060c4e0(c, nullptr, &way_count, &way_points, &way_index);   // 00666173..00666184
                path_count = way_count;                                // 00666189 (upper WORD of the slot is stale)
            }
            if (overflow(self) != 0) return 0;                         // 0066618D..0066619C
        } else {
            bool skip_214 = false;                                     // 006661E9 entry (speed present)
            if (flag4 != 0) {                                          // 006661A1..006661A3
                path_count = rw(self);                                 // 006661A5..006661AE
                if (static_cast<U16>(path_count) != 0) {               // 006661B0..006661B3
                    has_speed = rbool(self);                           // 006661B5..006661BE
                    if (has_speed != 0) { path_speed_bits = fb(rf(self)); skip_214 = true; }   // 006661C2..006661D9
                }
            } else {
                path_count = attack_dword_f0;                          // 006661DB ebx = {E-F0h} (stale; cleared below)
            }
            if (!skip_214 && rd32(c, 0x214) == 0) evaluate = false;    // 006661DF..006661E7 -> 006661F9
            else if (ai_byte == 0) evaluate = false;                   // 006661E9..006661EF -> 006661F9
            else if (ld<U8>(c, 0x1EF) == ai_byte) evaluate = false;    // 006661F1..006661F7 (equal -> 006661F9)
        }
        if (!evaluate) {
            recovered_client_creature_set_way_points_0060c790(c, nullptr, 0, 0);   // 006661F9..006661FF
            path_count = 0;                                            // 00666204
        } else if (static_cast<U16>(path_count) != 0) {                // 00666208..0066620B
            st<U>(c, 0x214, (has_speed != 0 || rd32(c, 0x214) != 0) ? 1u : 0u);   // 0066620D..0066622F
            st<U>(c, 0x1FC, path_speed_bits);                          // 00666228..00666235
        }
        // 0066623B
        scratch_88 = {0.0f, 0.0f, 0.0f};                               // 0066623E..00666254
        if (kotor_m15a_object_animation_stationary_0063c170(c, nullptr, animation) == 0 &&   // 0066623B..00666266
            static_cast<U16>(path_count) != 0) {                       // 0066626C..0066626F
            if (flag4 != 0) {                                          // 00666275..0066627B
                const U n = static_cast<U16>(path_count);              // 00666281 movzx ebp,bx
                Vector3* const points = static_cast<Vector3*>(new_object(n * 12u, [n](void* m) {   // 00666284..006662B9 (EH state 7)
                    vector_constructor_iterator_00401130(m, 0xC, n, &vector_default_construct_00409e70);   // 006662A9..006662B2
                    return m;
                }));
                if (points == nullptr) return 0;                       // 006662BB..006662CC
                for (U k = 0; k < n; ++k) {                            // 006662D2..00666332
                    points[k].x = rf(self);                            // 006662E1..006662EF
                    points[k].y = rf(self);                            // 006662F2..00666300
                    points[k].z = rf(self);                            // 00666303..00666311
                    if (overflow(self) != 0) return 0;                 // 00666313..0066631E (the array leaks, as the original)
                }
                bool evaluate_points = true;
                void* const destination = word_ptr(vslot<V0>(c, 0x150)(c, nullptr));   // 00666338..0066633C
                if (destination != nullptr) {                          // 00666342..00666344
                    Vector3 last = points[n - 1];                      // 0066634A..00666367 {E-E8h}
                    Vector3 delta_a8;                                  // {E-A8h}
                    kotor_cdecl_00423bc0(&delta_a8.x, static_cast<const float*>(destination), &last.x);   // 00666366..00666371
                    if (le(sq2(delta_a8.x, delta_a8.y), kEpsilon_00747504)) {   // 00666376..00666396
                        evaluate_points = false;
                        Vector3 delta_60;                              // {E-60h}
                        const float* d = kotor_cdecl_00423bc0(&delta_60.x, &here->x, &last.x);   // 00666398..006663A9
                        const Vector3 copy = {d[0], d[1], d[2]};       // 006663AE..006663CA {E-A8h}
                        if (le(sq2(copy.x, copy.y), kEpsilon_00747504)) {   // 006663BA..006663E2
                            const U loop = kotor_m15a_object_get_looping_animation_0063c100(c, nullptr);   // 006663E4..006663EC
                            vslot<V3>(c, 0x9C)(c, nullptr, loop, 0, 0);   // 006663F1..006663F4
                        }
                    }
                }
                if (evaluate_points)                                   // 006663FC..0066640B
                    recovered_client_creature_evaluate_new_way_points_0060e7d0(c, nullptr, n, points, ai_byte, &position);
                live_free(points);                                     // 00666410..00666416
            }
            // 00666419
            if (flag2 != 0 && lock_target == kInvalidObject_00746644)  // 00666419..00666432
                recovered_client_creature_set_lock_orientation_00610950(c, nullptr, lock_target, 0);   // 00666438..0066643D
            goto section_40_666673;
        }
        // 00666447 (stationary, or no path)
        if (rd32(c, 0x214) != 0 && ai_byte != 0 && ld<U8>(c, 0x1EF) != ai_byte) goto section_40_666673;   // 00666447..0066645F
        if (rp(c, 0x20) != nullptr)                                    // 00666465..0066646A
            w5_client_creature_compute_z_value_0060bf20(c, nullptr, rp(c, 0x20), &position, position.z,
                                                        word_of(&scratch_88));   // 0066646C..0066647E (lane-w5)
        if (flag1 == 0) goto section_40_666673;                        // 00666483..0066648C
        {
            Vector3* const desired = static_cast<Vector3*>(at(c, 0x1DC));
            const double dz = static_cast<double>(desired->z) - position.z;   // 00666492..00666498
            const double dy = static_cast<double>(desired->y) - position.y;   // 0066649C..006664A2
            const double dx = static_cast<double>(desired->x) - position.x;   // 006664A6..006664AC
            if (sq3_xyz(dx, dy, dz) > static_cast<double>(kEpsilon_00747504)) {   // 006664B0..006664D1 (jne skips <=/unordered)
                *desired = position;                                   // 006664D3..006664EB
                recovered_client_creature_reset_desired_position_timer_0060bf10(c, nullptr);   // 006664F1..006664F3
                vslot<VP>(c, 0x10C)(c, nullptr, &scratch_88);          // 006664F8..00666501
            }
        }
        {
            const double dz = static_cast<double>(here->z) - position.z;   // 00666507..0066650A
            const double dy = static_cast<double>(here->y) - position.y;   // 0066650E..00666511
            const double dx = static_cast<double>(here->x) - position.x;   // 00666515..00666518
            const double here_sq = sq3_zyx(here->x, here->y, here->z);     // 0066651C..00666530
            bool place = le(here_sq, kEpsilon_00747504);               // 00666532..0066653D (jnp -> 00666568)
            if (!place) place = sq3_xyz(dx, dy, dz) >= static_cast<double>(kNearDistanceSq_0074f348);   // 0066653F..00666560 ("test ah,1; jne" skips </unordered)
            if (place) {                                               // 0066656E
                vslot<VP>(c, 0x8C)(c, nullptr, &position);             // 0066656E..00666577
                vslot<VP>(c, 0x10C)(c, nullptr, &scratch_88);          // 0066657D..00666586
                if (flag2 != 0) vslot<VP>(c, 0x88)(c, nullptr, &orientation);   // 0066658C..006665A1
            }
        }
        goto section_40_666673;
    }
skip_path_6665ac:
    {
        // ------------------------------------------------------------ path not applied (006665AC)
        U16 count = 0;                                                 // bp
        if (flag4 != 0) {                                              // 006665AC..006665B2
            count = rw(self);                                          // 006665B4..006665BD
            if (count != 0) {                                          // 006665BF..006665C2
                const U speed_flag = rbool(self);                      // 006665C4..006665CB ebx
                const U speed = speed_flag != 0 ? fb(rf(self)) : path_speed_bits;   // 006665CD..006665E1
                if (c != nullptr) {                                    // 006665E8..006665EA
                    const U moving = (speed_flag != 0 || rd32(c, 0x214) != 0) ? 1u : 0u;   // 006665EC..0066660A
                    st<U>(c, 0x1FC, speed);                            // 006665FC / 0066660F
                    st<U>(c, 0x214, moving);                           // 00666602 / 00666615
                }
            }
        } else {
            count = static_cast<U16>(attack_dword_f0);                 // 0066661D bp = {E-F0h} (not read below)
        }
        if (flag4 != 0 && count != 0) {                                // 00666625..00666637
            for (U k = count; k != 0; --k) { (void)rf(self); (void)rf(self); (void)rf(self); }   // 00666640..00666671
        }
    }
section_40_666673:
    // ---------------------------------------------------------------- 40h (00666673)
    if ((mask & 0x40) != 0) {
        const U16 w_f0 = rw(self);                                     // 00666681..0066668E {E-F0h}
        const U8 party_kind = rb(self);                                // 00666687..0066669B {E-E9h}
        const U16 w_d8 = rw(self);                                     // 00666697..006666A8 {E-D8h}
        const U8 b_c8 = rb(self);                                      // 006666A4..006666B5 {E-C8h}
        const U8 stealth = rb(self);                                   // 006666B1..006666C0 bl
        const U b_dc = rbool(self);                                    // 006666BE..006666C9 {E-DCh}
        const U b_8c = rbool(self);                                    // 006666C7..006666D6 {E-8Ch}
        if (overflow(self) != 0) return 0;                             // 006666D2..006666E1
        const U party_object = party_kind == 2 ? rid(self) : kInvalidObject_00746644;   // 006666E7..006666FB {E-CCh}
        if (overflow(self) != 0) return 0;                             // 00666703..0066670E
        if (c != nullptr) {                                            // 00666714..00666716
            if (stealth == 1) {                                        // 0066671C..0066671F
                if (rd32(c, 0x194) == 0) {                             // 00666725..0066672D
                    feedback(c, 0x55);                                 // 00666733..006667AE
                    if (rd32(c, 4) == kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr))   // 006667B3..006667C5
                        invisible_sound(c);                            // 006667C7..0066680A
                }
                recovered_client_creature_set_stealth_state_00613930(c, nullptr, 1, b_dc);   // 0066680F..00666915
            } else if (stealth == 0) {                                 // 0066681B..0066681D
                if (rd32(c, 0x194) == 1) {                             // 00666823..0066682A
                    feedback(c, 0x56);                                 // 00666830..006668AB
                    if (rd32(c, 4) == kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr))   // 006668B0..006668C2
                        invisible_sound(c);                            // 006668C4..00666907
                }
                recovered_client_creature_set_stealth_state_00613930(c, nullptr, 0, b_dc);   // 0066690C..00666915
            }
        }
        // 0066691A
        if (c != nullptr) {                                            // 00666927..00666929 / 00666A0F..00666A11
            if (w_f0 != 0) {                                           // 0066691E..00666921
                kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(client_app(), nullptr, rd32(c, 4));   // 00666A17..00666A24
            } else {
                if (recovered_client_get_player_by_id_005ed590(client_app(), nullptr, rd32(c, 4)) == nullptr &&   // 0066692F..00666943
                    rd32(c, 0x188) == 0)                               // 00666945..0066694D
                    recovered_client_add_to_hitcheck_ignore_list_005ed750(client_app(), nullptr, rd32(c, 4));   // 0066694F..0066695C
                kotor_fastcall_00611700(c, nullptr, 0, 0);             // 00666961..00666967 (lane-w4)
                power_equipped(c, 0x10);                               // 0066696C..00666992
                power_equipped(c, 0x20);                               // 00666997..006669BD
                if (recovered_party_member_00634620(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, rd32(c, 4)) != 0 &&   // 006669C2..006669DD
                    ld<I32>(kotor_fastcall_005ed8b0(client_app(), nullptr), 0) > 1)   // 006669DF..006669F0
                    show_tutorial_window_005edf40(client_app(), nullptr, 0xF, kInvalidObject_00746644,
                                                  kInvalidObject_00746644, 0);   // 006669F2..00666A08
            }
            // 00666A29
            st<U16>(c, 0x2D0, w_f0);                                   // 00666A36
            st<U8>(c, 0x2D2, party_kind);                              // 00666A41
            st<U8>(c, 0x2DC, b_c8);                                    // 00666A4B
            st<U16>(c, 0x2D4, w_d8);                                   // 00666A54
            st<U>(c, 0x2D8, party_object);                             // 00666A5B
            recovered_client_creature_set_combat_state_006182a0(c, nullptr, b_8c);   // 00666A51..00666A61
            if (b_8c != 0 && rd32(c, 0x3A4) != 0 &&                    // 00666A66..00666A72
                ld<U8>(kotor_fastcall_005ed700(client_app(), nullptr), 0x6D) == 5) {   // 00666A74..00666A85
                recovered_camera_reset_cell_007a2288 = 1;              // 00666A8D (lane-w1 cell)
                kotor_fastcall_00641dc0(kotor_fastcall_005ed530(client_app(), nullptr), nullptr);   // 00666A87..00666AA1 (lane-w4)
                kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);  // 00666AA6..00666AB3
            }
            (void)frame_R4_005ed690(client_app(), nullptr);            // 00666AB8..00666AC0 (result unused)
        }
    }
    // ---------------------------------------------------------------- 100h (00666AC5)
    if ((mask & 0x100) != 0) {
        const float walk = rf(self);                                   // 00666AD5..00666AE3 {E-F0h}
        const float run = rf(self);                                    // 00666AE7..00666AF5 {E-D4h}
        if (overflow(self) != 0) return 0;                             // 00666AF9..00666B04
        if (c != nullptr) {                                            // 00666B0A..00666B0C
            st<float>(c, 0x20C, walk);                                 // 00666B26
            st<float>(c, 0x210, run);                                  // 00666B2C
            st<float>(rp(c, 0x21C), 0x5C, run * kRunRateScale_0073d708);   // 00666B0E..00666B32
            const double awful = recovered_client_appearance_get_run_looks_awful_rate_006975d0(rp(c, 0x21C), nullptr);   // 00666B35..00666B3B (ST0 wide)
            // fcomp dword [run]; test ah,41h; jne -> 0: 1 only for an ordered ST0 > run
            recovered_client_creature_set_run_looks_awful_0060e1f0(c, nullptr, awful > static_cast<double>(run) ? 1u : 0u);   // 00666B40..00666B53
        }
    }
    if ((mask & 0x40000) != 0) {                                       // 00666B58..00666B5E
        (void)rid(self);                                               // 00666B60..00666B62 (area id, unused)
        if (overflow(self) != 0) return 0;                             // 00666B67..00666B72
    }
    if ((mask & 0x200) != 0) {                                         // 00666B78..00666B7B
        (void)kotor_fastcall_004d6780(self, nullptr, 0xA);             // 00666B7D..00666B81 ReadDWORD(0Ah)
        (void)kotor_fastcall_004d6780(self, nullptr, 0xA);             // 00666B86..00666B8A
        if (overflow(self) != 0) return 0;                             // 00666B8F..00666B9A
    }
    if ((mask & 0x400) != 0) {                                         // 00666BA0..00666BA3
        const U16 hp = rw(self);                                       // 00666BA5..00666BB2 ebx
        const U16 e0 = rw(self);                                       // 00666BAE..00666BBD ebp
        const U16 e4 = rw(self);                                       // 00666BB9..00666BC8 {E-F0h}
        const U16 max_hp = rw(self);                                   // 00666BC4..00666BD5 {E-D8h}
        if (overflow(self) != 0) return 0;                             // 00666BD1..00666BE0
        if (c != nullptr) {                                            // 00666BE6..00666BE8
            st<U16>(rp(c, 0x2F8), 0x48, e0);                           // 00666BEA..00666BFA
            st<U16>(rp(c, 0x2F8), 0x4A, e4);                           // 00666BFE..00666C04
            st<U16>(rp(c, 0x2F8), 0x4C, hp);                           // 00666C08..00666C0E
            st<U16>(rp(c, 0x2F8), 0x4E, max_hp);                       // 00666C12..00666C18
        }
    }
    if ((mask & 0x80000) != 0) {                                       // 00666C1C..00666C27
        const U16 w1 = rw(self);                                       // 00666C2D..00666C3A ebp
        const U16 w2 = rw(self);                                       // 00666C36..00666C45 ebx
        const U16 w3 = rw(self);                                       // 00666C41..00666C50 {E-F0h}
        const U16 w4 = rw(self);                                       // 00666C4C..00666C5D {E-D8h}
        if (overflow(self) != 0) return 0;                             // 00666C59..00666C68
        void* const creature = frame_R4_005ed5b0(client_app(), nullptr, id);   // 00666C6E..00666C7E (looked up again)
        if (creature != nullptr) {                                     // 00666C83..00666C85
            st<U16>(rp(creature, 0x2F8), 0x120, w2);                   // 00666C87..00666C92
            st<U16>(rp(creature, 0x2F8), 0x124, w3);                   // 00666C99..00666C9F
            st<U16>(rp(creature, 0x2F8), 0x122, w1);                   // 00666CA6..00666CAC
            st<U16>(rp(creature, 0x2F8), 0x11E, w4);                   // 00666CB3..00666CBE
        }
    }
    if ((mask & 0x10000) != 0) {                                       // 00666CC5..00666CD2
        ResRef sound_60;                                               // {E-60h}
        ExoString first_b0;                                            // {E-B0h}
        kotor_fastcall_00692660(self, nullptr, &first_b0, kotor_fastcall_00405ed0(&sound_60, nullptr), nullptr);   // 00666CD8..00666CEE
        bool failed;
        {
            Unwind guard_b0(&destroy_string_void, &first_b0);          // 00666CFC EH state 0Ch
            ResRef sound_28;                                           // {E-28h}
            ExoString last_e8;                                         // {E-E8h}
            kotor_fastcall_00692660(self, nullptr, &last_e8, kotor_fastcall_00405ed0(&sound_28, nullptr), nullptr);   // 00666CF3..00666D14
            {
                Unwind guard_e8(&destroy_string_void, &last_e8);       // 00666D1D EH state 0Dh
                failed = overflow(self) != 0;                          // 00666D19..00666D2A
                guard_e8.dismiss();
            }
            destroy_string(&last_e8);                                  // 00666D3E / 00665852 (state 0Ch)
            guard_b0.dismiss();
        }
        destroy_string(&first_b0);                                     // 00666D43..00666D52 / 00665857..00665866
        if (failed) return 0;                                          // 0066586B
    }
    if ((mask & 0x4000) != 0) {                                        // 00666D57..00666D5A
        const U p0 = rbool(self);                                      // 00666D60..00666D69 ebp
        const U p1 = rbool(self);                                      // 00666D67..00666D72 {E-F0h}
        const U p2 = rbool(self);                                      // 00666D70..00666D7D {E-D8h}
        const U p3 = rbool(self);                                      // 00666D7B..00666D88 {E-CCh}
        const U has_leader = rbool(self);                              // 00666D86..00666D95 ebx
        U leader = kInvalidObject_00746644;                            // 00666D97 {E-DCh}
        ExoString first_e8, last_b0;                                   // {E-E8h}, {E-B0h}
        construct_string(&first_e8);                                   // 00666D91..00666D9F
        {
            Unwind guard_e8(&destroy_string_void, &first_e8);          // 00666DA8 EH state 0Eh
            construct_string(&last_b0);                                // 00666DA4..00666DB3
            {
                Unwind guard_b0(&destroy_string_void, &last_b0);       // 00666DBA EH state 0Fh
                if (has_leader != 0) {                                 // 00666DB8..00666DC2
                    leader = rid(self);                                // 00666DC8..00666DD8
                    ResRef sound_60;                                   // {E-60h}
                    ExoString read_7c;                                 // {E-7Ch}
                    kotor_fastcall_00692660(self, nullptr, &read_7c, kotor_fastcall_00405ed0(&sound_60, nullptr), nullptr);   // 00666DCF..00666DEC
                    {
                        Unwind guard_7c(&destroy_string_void, &read_7c);   // 00666DF6 EH state 10h
                        async_assign_string_005e5c50(&first_e8, nullptr, &read_7c);   // 00666DF1..00666DFE
                        guard_7c.dismiss();
                    }
                    destroy_string(&read_7c);                          // 00666E03..00666E12
                    kotor_fastcall_00692660(self, nullptr, &read_7c, kotor_fastcall_00405ed0(&sound_60, nullptr), nullptr);   // 00666E17..00666E30
                    {
                        Unwind guard_7c(&destroy_string_void, &read_7c);   // 00666E3A EH state 11h
                        async_assign_string_005e5c50(&last_b0, nullptr, &read_7c);   // 00666E35..00666E42
                        guard_7c.dismiss();
                    }
                    destroy_string(&read_7c);                          // 00666E47..00666E56
                }
                (void)rbool(self);                                     // 00666E5B..00666E5D
                (void)rbool(self);                                     // 00666E62..00666E64
                if (overflow(self) != 0) {                             // 00666E69..00666E74
                    guard_b0.dismiss();
                    guard_e8.dismiss();
                    destroy_string(&last_b0);                          // 00666E76..00666E82
                    destroy_string(&first_e8);                         // 00666E87..00666E96
                    return 0;                                          // 00666E9B..00666E9D
                }
                if (c != nullptr) {                                    // 00666EA2..00666EA4
                    if (rp(c, 0x30C) == nullptr) {                     // 00666EAA..00666EB2
                        void* const status = new_object(0x20, [](void* m) {   // 00666EB4..00666ED7 (EH state 12h)
                            return recovered_client_creature_party_status_construct_0060b750(m, nullptr);
                        });
                        st<void*>(c, 0x30C, status);                   // 00666EE1
                    }
                    st<U>(rp(c, 0x30C), 0x00, p0);                     // 00666EE9..00666EF7
                    st<U>(rp(c, 0x30C), 0x04, p1);                     // 00666EF9..00666EFF
                    st<U>(rp(c, 0x30C), 0x08, p2);                     // 00666F02..00666F0C
                    st<U>(rp(c, 0x30C), 0x0C, p3);                     // 00666F0F..00666F15
                    st<U>(rp(c, 0x30C), 0x10, has_leader);             // 00666F18..00666F1E
                    if (has_leader != 0) {                             // 00666F21
                        st<U>(rp(c, 0x30C), 0x14, leader);             // 00666F23..00666F2D
                        async_assign_string_005e5c50(at(rp(c, 0x30C), 0x18), nullptr, &first_e8);   // 00666F30..00666F3E
                    }
                }
                guard_b0.dismiss();
            }
            destroy_string(&last_b0);                                  // 00666F43..00666F4F (state 0Eh)
            guard_e8.dismiss();
        }
        destroy_string(&first_e8);                                     // 00666F54..00666F63
    }
    if ((mask & 0x8000) != 0) {                                        // 00666F68..00666F71
        void* const me = client_get_player_creature_005ed540(client_app(), nullptr);   // 00666F77..00666F80
        const U me_id = me != nullptr ? rd32(me, 4) : kInvalidObject_00746644;   // 00666F85..00666F96 {E-D0h}
        const U e1 = rbool(self);                                      // 00666F9A..00666FA3 {E-CCh}
        const U e2 = rbool(self);                                      // 00666FA7..00666FAE {E-DCh}
        const U e3 = rbool(self);                                      // 00666FB2..00666FB9 ebx
        if (overflow(self) != 0) return 0;                             // 00666FB7..00666FC4
        if (c != nullptr) {                                            // 00666FCA..00666FCC
            const I32 count = ld<I32>(c, 0x364);                       // 00666FD2
            U found = 0;                                               // 00666FD8 ecx
            void* element = nullptr;                                   // eax
            if (count > 0) {                                           // 00666FDA..00666FE4
                for (I32 i = 0; i < count; ++i) {                      // 00666FE6..00667008
                    element = rp(rp(c, 0x360), static_cast<U>(i) * 4);   // 00666FF0
                    if (rd32(element, 0) == me_id) { found = 1; break; }   // 00666FF2..00666FF8 / 00667010
                }
            }
            const U8 bits = static_cast<U8>(((e3 & 1u) << 4) | ((e2 & 1u) << 1) | (e1 & 1u));   // 00667108..00667120 / 0066713D..00667158
            if (me_id == id) {                                         // 0066701B..00667026
                if (found == 0) {                                      // 00667028..0066702A
                    void* fresh = live_operator_new(8);                // 0066702C..00667033
                    if (fresh != nullptr) {                            // 00667036..00667038
                        st<U>(fresh, 0, kInvalidObject_00746644);      // 00667040
                        st<U8>(fresh, 4, static_cast<U8>(ld<U8>(fresh, 4) & 0xE0));   // 0066703A..00667046
                    }
                    const U8 old = ld<U8>(fresh, 4);                   // 0066704F (NULL faults, as the original)
                    st<U>(fresh, 0, me_id);                            // 00667062
                    st<U8>(fresh, 4, static_cast<U8>((old & ~0x10u) | ((e3 << 4) & 0x10u) | 3u));   // 00667056..0066706F
                    gui_load_array_add_00671c00(at(c, 0x360), nullptr, word_of(fresh));   // 00667068..00667072
                    recovered_client_creature_update_relative_perception_0060c980(c, nullptr, fresh);   // 00667077..0066707A
                } else {
                    const U8 old = ld<U8>(element, 4);                 // 00667084
                    st<U8>(element, 4, static_cast<U8>((old & ~0x10u) | ((e3 << 4) & 0x10u) | 3u));   // 00667087..00667097
                    recovered_client_creature_update_relative_perception_0060c980(c, nullptr, element);   // 00667094..0066709A
                }
            } else if (found == 0) {                                   // 006670A4..006670A6
                if (count > 0) {                                       // 006670AC..006670AE
                    do {
                        void* const head = rp(rp(c, 0x360), 0);        // 006670B0..006670B6
                        if (head != nullptr) live_free(head);          // 006670B8..006670C8
                        deleteAt_006857a0(at(c, 0x360), nullptr, 0);   // 006670CB..006670CF
                    } while (ld<I32>(c, 0x364) > 0);                   // 006670D4..006670D9
                }
                if (me_id != kInvalidObject_00746644) {                // 006670DB..006670E5
                    void* fresh = live_operator_new(8);                // 006670E7..006670EE
                    if (fresh != nullptr) {                            // 006670F1..006670F3
                        st<U>(fresh, 0, kInvalidObject_00746644);      // 006670FB
                        st<U8>(fresh, 4, static_cast<U8>(ld<U8>(fresh, 4) & 0xE0));   // 006670F5..00667101
                    }
                    const U8 old = ld<U8>(fresh, 4);                   // 00667122 (NULL faults, as the original)
                    st<U>(fresh, 0, me_id);                            // 00667131
                    st<U8>(fresh, 4, static_cast<U8>((old & 0xECu) | bits));   // 00667125..00667133
                    gui_load_array_add_00671c00(at(c, 0x360), nullptr, word_of(fresh));   // 0066712A..00667136
                }
            } else {
                st<U8>(element, 4, static_cast<U8>((ld<U8>(element, 4) & 0xECu) | bits));   // 0066713D..0066715F
            }
        }
    }
    if ((mask & 0x20000) != 0) {                                       // 00667162..0066716D
        const U8 state = rb(self);                                     // 00667173..00667182 {E-F1h}
        if (overflow(self) != 0) return 0;                             // 0066717E..0066718D
        if (c != nullptr) {                                            // 00667193..00667195
            if ((state & 1) != 0) {                                    // 0066719B..006671A4
                if (rd32(c, 0x190) == 0) feedback(c, 0x53);            // 006671AA..0066722C
                st<U>(c, 0x190, 1);                                    // 00667231 / 00667243
            } else {
                if (rd32(c, 0x190) == 1) feedback(c, 0x54);            // 00667252..006672D5
                st<U>(c, 0x190, 0);                                    // 006672DF
            }
            if ((state & 2) != 0) {                                    // 006672E9..006672F4
                if (rd32(c, 0x194) == 0) feedback(c, 0x55);            // 006672FA..00667375
                recovered_client_creature_set_stealth_state_00613930(c, nullptr, 1, 0);   // 0066737A..0066740A
            } else {
                if (rd32(c, 0x194) == 1) feedback(c, 0x56);            // 00667383..006673FF
                recovered_client_creature_set_stealth_state_00613930(c, nullptr, 0, 0);   // 00667404..0066740A
            }
            struct Bit { U8 bit; unsigned offset; U on, off; };
            const Bit bits[] = {
                {0x04, 0x198, 0x57, 0x58},                             // 0066740F..00667530
                {0x08, 0x19C, 0x59, 0x5A},                             // 0066753A..0066765B
                {0x10, 0x1A0, 0x5B, 0x5C},                             // 00667665..00667786
                {0x40, 0x1A8, 0xA6, 0xA7},                             // 00667790..006678B7
                {0x20, 0x1AC, 0xA8, 0xA9},                             // 006678C1..006679E8
            };
            for (const Bit& b : bits) {
                if ((state & b.bit) != 0) {
                    if (rd32(c, b.offset) == 0) feedback(c, b.on);
                    st<U>(c, b.offset, 1);
                } else {
                    if (rd32(c, b.offset) == 1) feedback(c, b.off);
                    st<U>(c, b.offset, 0);
                }
            }
        }
    }
    if ((mask & 0x800) != 0) {                                         // 006679F2..006679FF
        const U8 damage = rb(self);                                    // 00667A01..00667A0E
        if (overflow(self) != 0) return 0;                             // 00667A0A..00667A17
        if (c != nullptr) st<U8>(rp(c, 0x2F8), 0x52, damage);          // 00667A1D..00667A27
    }
    if ((mask & 0x1000) != 0) {                                        // 00667A2A..00667A30
        ExoString name_70;                                             // {E-70h}
        construct_string(&name_70);                                    // 00667A36..00667A3D
        {
            Unwind guard_70(&destroy_string_void, &name_70);           // 00667A4B EH state 2Fh
            const U16 w24 = rw(self);                                      // 00667A47..00667A5B ebp
            ExoString read_68;                                             // {E-68h}
            kotor_fastcall_004d67e0(self, nullptr, &read_68, 0x20);        // 00667A57..00667A65
            {
                Unwind guard_68(&destroy_string_void, &read_68);       // 00667A72 EH state 30h
                async_assign_string_005e5c50(&name_70, nullptr, &read_68);   // 00667A6A..00667A7A
                guard_68.dismiss();
            }
            destroy_string(&read_68);                                      // 00667A7F..00667A8D
            const U8 b30 = rb(self);                                       // 00667A92..00667A9F {E-C8h}
            const U8 b31 = rb(self);                                       // 00667A9B..00667AAC {E-F1h}
            const U16 alignment = rw(self);                                // 00667AA8..00667AB7 {E-F0h}
            const U hostile_list = rbool(self);                            // 00667AB5..00667AC4 ebx
            const U8 classes = rb(self);                                   // 00667AC2..00667ACF {E-E9h}
            if (overflow(self) != 0) { guard_70.dismiss(); destroy_string(&name_70); return 0; }   // 00667ACB..00667ADA -> 00667C99
            if (c != nullptr) {                                            // 00667AE0..00667AE2
                st<U16>(stats, 0x24, w24);                                 // 00667AE8..00667AF7
                async_assign_string_005e5c50(at(stats, 0x28), nullptr, &name_70);   // 00667AEC..00667AFB
                st<U8>(stats, 0x30, b30);                                  // 00667B00..00667B0C
                st<U8>(stats, 0x31, b31);                                  // 00667B13
                recovered_client_creature_set_alignment_good_evil_0060f780(c, nullptr, alignment);   // 00667B0F..00667B19
                kotor_fastcall_004015a0(c, nullptr);                       // 00667B1E..00667B20 void_return
                st<U>(c, 0x188, hostile_list);                             // 00667B2B
                if (hostile_list == 1) {                                   // 00667B25..00667B32
                    kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(client_app(), nullptr, rd32(c, 4));   // 00667B34..00667B3D
                } else if (recovered_client_get_player_by_id_005ed590(client_app(), nullptr, rd32(c, 4)) == nullptr &&   // 00667B44..00667B54
                           ld<U16>(c, 0x2D0) == 0) {                       // 00667B56..00667B5D
                    recovered_client_add_to_hitcheck_ignore_list_005ed750(client_app(), nullptr, rd32(c, 4));   // 00667B5F..00667B6C
                }
                st<U8>(stats, 0x32, classes);                              // 00667B71..00667B79
            }
            for (I32 i = 0; i < static_cast<I32>(classes); ++i) {          // 00667B7C..00667BD7
                const U8 cls = rb(self);                                   // 00667B87..00667B94 {E-F0h} low byte
                const U8 level = rb(self);                                 // 00667B90..00667BA1 {E-D8h} low byte
                if (overflow(self) != 0) { guard_70.dismiss(); destroy_string(&name_70); return 0; }   // 00667B9D..00667BAC
                if (c != nullptr) {                                        // 00667BB2..00667BB4
                    // the original pushes the whole {E-F0h}/{E-D8h} dwords; both callees read the low byte only
                    kotor_m15b_creature_stats_set_class_00647770(stats, nullptr, static_cast<U>(i), cls);   // 00667BB6..00667BC0
                    recovered_client_creature_stats_set_class_level_00647790(stats, nullptr, static_cast<U>(i), level);   // 00667BC5..00667BCF
                }
            }
            guard_70.dismiss();
        }
        destroy_string(&name_70);                                      // 00667BD9..00667BEB
    }
    if ((mask & 0x2000) != 0 && c != nullptr) {                        // 00667BF7..00667C01
        void* const module = kotor_fastcall_005ed530(client_app(), nullptr);   // 00667C03..00667C0B
        void* const area = module != nullptr ? rp(module, 0x48) : nullptr;     // 00667C10..00667C14
        if (area != nullptr && rd32(c, 0x39C) == rd32(area, 0x104)) {  // 00667C17..00667C27
            using GetBase = void* (__fastcall*)(void*, void*, U, U);
            void* const base = vslot<GetBase>(c, 0x98)(c, nullptr, 0xFF, 0);   // 00667C29..00667C34
            if (base != nullptr &&                                     // 00667C3A..00667C3C
                [&] { const U before = rd32(area, 0x184);                  // 00667C40 (read before the call)
                      return vslot<V0>(base, 0x104)(base, nullptr) != before; }() &&   // 00667C3E..00667C50
                recovered_client_reattach_creature_to_scene_005edb80(client_app(), nullptr, rd32(c, 4)) != 0)   // 00667C52..00667C66
                vslot<V1>(c, 0x94)(c, nullptr, rd32(area, 0x184));     // 00667C68..00667C73
        }
    }
    return 1;                                                          // 00667C79..00667C96
}
}  // extern "C"
