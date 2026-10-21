// claude_w3_aoe.cpp - the client area-of-effect object CSWCAreaOfEffectObject (0x17C bytes, a CSWCObject of type 0Bh)
// with its method table 00757E28, and the GameObjectUpdate 'A' leaf that creates it. Original: swkotor.exe GOG K1
// (image base 0x400000), read-only static disassembly; nothing original executed. Every statement cites its original
// instruction range; B+x = the original frame slot at the function's post-prologue ESP + x.
//   00651680 CSWCMessage::HandleServerToPlayerAreaOfEffectUpdate_Add       (replaces the w3_traps_objects.cpp stub)
//   006D8310 CSWCAreaOfEffectObject::CSWCAreaOfEffectObject   006D9820 ~CSWCAreaOfEffectObject
//   006D9890 scalar deleting destructor (+000)                006D8490 LoadEffect(int row)
//   006D90D0 UnloadModel (+080)   006D91D0 UnloadModel(BYTE part) (+07C)   006D91F0 SetAnimation(WORD, BYTE)
//   006D9300 ResetInstanceDuration(int)                       006D9520 SetMoveTimer(DWORD, model*)
//   006D9590 SetEnvMap(a, b, CResRef&) (+0B0)                 006D96E0 SetEnvMap(BYTE part, a, b, CResRef&) (+0AC)
//   006D98B0 SetInstanceRandomPosition(BYTE)                  006DA330 UpdateInstancePosition
//   006DA520 SetPosition(Vector&) (+08C)                      006DA9B0 AIUpdate (+074)
//   0063BFA0 CSWCArea::GetEnvMap (a 16-byte CResRef copy; no callable rebuilt provider)
// StopEffect 006D9350 and the lookups 005ED610 / 005F1360 live in claude_w3_update2.cpp (not redefined here).
//
// Field map (this): +000 vptr, +004 id, +024 position, +030 orientation, +03C/+040 world time (day, time of day),
//   +044 elapsed ms this update, +068 CSWCAnimBaseVisualEffect[n] (0xF0 each, CRT vector-new with count at -4),
//   +0FC instance duration[n] +100 instance start delay[n] +104 animation time left[n] +108..+110 group durations
//   +114..+11C group instance counts +120 total instances n +124 radius +128 width +12C length +130 stop timer
//   +134 finished +138 stopping +13C..+13E group edge percentages +13F shape ('C'/'R') +140 added-to-scene[n]
//   +144 scene +148 WORD animation[n] +14C CSWCVisualEffect* +150 vfx added +154 CExoSoundSource* duration sound
//   +158 CResRef one-shot sound +168 one-shot percentage +16C orient with ground +170 row (-1) +174 duration sound
//   started +178 creator id (7F000000h).
#include "a1bf7fc0cb-w3_client_t3.h"
#include <cmath>

extern "C" {
// ------------------------------------------------------------------ existing providers (c13 map object noted)
extern const unsigned char kotor_m15e_object_methods_007516c8[];                                    // 007516C8 CSWCObject table, 82 slots (0832)
void __fastcall kotor_m15e_object_destruct_0063e800(void* self, void*);                           // 0063E800 ~CSWCObject (0831)
std::uint32_t __fastcall mp_slot30_ai_zero_0066b360(void*, void*, void*, void*, std::uint32_t);      // 0066B360 xor eax,eax; ret 0Ch (0854-slot509)
void* __fastcall kotor_fastcall_005ed580(void* app, void*, std::uint32_t id);                       // 005ED580 GetGameObject RET 4 (0701)
std::int32_t __fastcall c2da_current_get_int_entry_00413510(void* table, void*, std::int32_t row, std::int32_t column,
                                                            std::int32_t* out);                    // 00413510 GetINTEntry RET 0Ch (0390)
std::int32_t __fastcall kotor_m15b_c2da_get_float_entry_00413350(void* table, void*, std::int32_t row, std::int32_t column,
                                                                 float* out);                      // 00413350 GetFLOATEntry RET 0Ch (0820)
std::int32_t __fastcall object_vfx_table_00413ec0(void* table, void*, const void* row_label, std::int32_t column,
                                                  void* out);                                      // 00413EC0 GetCExoStringEntry(label) RET 0Ch
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, std::uint32_t size, std::int32_t count, void* construct,
                                                  void* destroy);                                  // 006FB55F eh vector ctor iterator (d1_bodies)
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void* destroy);   // 006FA852 (0186)
void* __fastcall area_vfx_construct_0068fe70(void* self, void*, std::uint32_t flags);              // 0068FE70 CSWCVisualEffect() RET 4
void __fastcall area_vfx_play_00690510(void* vfx, void*, std::uint32_t animation);                 // 00690510 CSWCVisualEffect::SetAnimation RET 4
void* __fastcall area_vfx_model_construct_006e5ce0(void* self, void*);                             // 006E5CE0 CSWCAnimBaseVisualEffect()
void __fastcall area_vfx_model_destroy_006e5d00(void* self, void*);                                // 006E5D00 ~CSWCAnimBaseVisualEffect
void __fastcall area_vfx_anim_hilite_006a03e0(void* anim, void*, const void* color, std::uint32_t a,
                                              std::uint32_t b);                                    // 006A03E0 CSWCAnimBase::Hilite RET 8
void* __fastcall async_ref_assign_string_004061f0(void* self, void*, void* result, const void* text);   // 004061F0 CResRef::operator=(CExoString&) RET 8
void* __cdecl rotation_arc_004ab630(void* out, kotor_w3::Vector3 from, kotor_w3::Vector3 to);      // 004AB630 RotationArc (0464), Quaternion by hidden return
void* __cdecl quaternion_rotate_004a9bb0(void* out, const void* quaternion, const void* v);        // 004A9BB0 operator*(Quaternion, Vector) (0464)
void* __fastcall aurora_hit_info_construct_004578f0(void* hit, void*, std::uint32_t a, std::uint32_t b,
                                                    std::uint32_t c);                              // 004578F0 CHitInfo::CHitInfo RET 0Ch
void __fastcall chit_info_destruct_00456b30(void* hit, void*);                                     // 00456B30 CHitInfo::~CHitInfo (0464)
void __fastcall kotor_fastcall_00456af0(void* hit, void*, std::uint32_t a, std::uint32_t b, std::uint32_t c);   // 00456AF0 CHitInfo::Reset RET 0Ch
void* __fastcall area_vfx_hitcheck_list_005ed8c0(void* app, void*);                                // 005ED8C0 GetHitCheckMaterialMask ([[this+4]+A4h])
std::uint32_t __fastcall recovered_world_time_difference_004ae460(void* timer, void*, std::uint32_t day1, std::uint32_t time1,
                                                                  std::uint32_t day2, std::uint32_t time2,
                                                                  std::uint32_t* out_day, std::uint32_t* out_time);   // 004AE460 RET 18h
void __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, const void* resref, std::uint32_t x,
                                                         std::uint32_t y, std::uint32_t z, std::uint32_t a5,
                                                         std::uint32_t a6, std::uint32_t a7, std::uint32_t a8,
                                                         std::uint32_t a9, std::uint32_t a10);   // 005D5E10 RET 28h (0812)
extern void* kotor_cell_sound_007a39ec;                                                              // [007A39EC] CExoSound*
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void* source, void*, std::uint32_t is3d);   // 005D5910 RET 4 (0812)
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, const void* position, float a);   // 005D59E0 RET 8 (0812)
void __fastcall kotor_fastcall_005d5930(void* source, void*);                                      // 005D5930 CExoSoundSource::Play (w4 creature_ai)
// cached 2DA column indices (0431 rules image data)
extern std::uint32_t kotor_rules_cell32_007a21b4, kotor_rules_cell32_007a21b8, kotor_rules_cell32_007a21bc,
    kotor_rules_cell32_007a21c0, kotor_rules_cell32_007a21c4, kotor_rules_cell32_007a21c8, kotor_rules_cell32_007a21e8,
    kotor_rules_cell32_007a21ec, kotor_rules_cell32_007a21f0, kotor_rules_cell32_007a21f4, kotor_rules_cell32_007a21f8,
    kotor_rules_cell32_007a21fc, kotor_rules_cell32_007a2200, kotor_rules_cell32_007a2204, kotor_rules_cell32_007a2208,
    kotor_rules_cell32_007a220c, kotor_rules_cell32_007a2210, kotor_rules_cell32_007a2214, kotor_rules_cell32_007a2218,
    kotor_rules_cell32_007a221c, kotor_rules_cell32_007a2220, kotor_rules_cell32_007a2224, kotor_rules_cell32_007a2228,
    kotor_rules_cell32_007a222c, kotor_rules_cell32_007a2230, kotor_rules_cell32_007a2234, kotor_rules_cell32_007a2238,
    kotor_rules_cell32_007a223c, kotor_rules_cell32_007a2240;

// ------------------------------------------------------------------ this unit
std::uint32_t __fastcall recovered_client_handle_aoe_add_00651680(void* self, void*) noexcept(false);   // RET
void* __fastcall recovered_client_aoe_construct_006d8310(void* self, void*) noexcept(false);           // RET
void __fastcall recovered_client_aoe_destruct_006d9820(void* self, void*) noexcept(false);             // RET
void* __fastcall recovered_client_aoe_scalar_deleting_destructor_006d9890(void* self, void*, std::uint32_t flags) noexcept(false);   // RET 4
std::uint32_t __fastcall recovered_client_aoe_load_effect_006d8490(void* self, void*, std::int32_t row) noexcept(false);   // RET 4
void __fastcall recovered_client_aoe_unload_model_006d90d0(void* self, void*) noexcept(false);         // RET
void __fastcall recovered_client_aoe_unload_model_part_006d91d0(void* self, void*, std::uint32_t part) noexcept(false);   // RET 4
std::uint32_t __fastcall recovered_client_aoe_set_animation_006d91f0(void* self, void*, std::uint32_t animation,
                                                                     std::uint32_t index) noexcept(false);   // RET 8
void __fastcall recovered_client_aoe_reset_instance_duration_006d9300(void* self, void*, std::int32_t index) noexcept;   // RET 4
void __fastcall recovered_client_aoe_set_move_timer_006d9520(void* self, void*, std::uint32_t timer,
                                                             void* model) noexcept(false);            // RET 8
std::uint32_t __fastcall recovered_client_aoe_set_env_map_006d9590(void* self, void*, std::uint32_t a, std::uint32_t b,
                                                                   void* resref) noexcept(false);     // RET 0Ch
std::uint32_t __fastcall recovered_client_aoe_set_env_map_part_006d96e0(void* self, void*, std::uint32_t part,
                                                                        std::uint32_t a, std::uint32_t b,
                                                                        void* resref) noexcept(false);   // RET 10h
std::uint32_t __fastcall recovered_client_aoe_set_instance_random_position_006d98b0(void* self, void*,
                                                                                    std::uint32_t index) noexcept(false);   // RET 4
void __fastcall recovered_client_aoe_update_instance_position_006da330(void* self, void*) noexcept(false);   // RET
void __fastcall recovered_client_aoe_set_position_006da520(void* self, void*, const void* position) noexcept(false);   // RET 4
void __fastcall recovered_client_aoe_ai_update_006da9b0(void* self, void*) noexcept(false);            // RET
void* __fastcall kotor_fastcall_0063bfa0(void* area, void*, void* out) noexcept;                       // RET 4
}

using namespace kotor_w3;

namespace {
constexpr U8 kAreaOfEffectType_0074663a = 0x0B;                        // .rdata 0074663A (byte 0Bh) CSWCObject type
constexpr U8 kArraySuccess_0074666c = 0;                               // .rdata 0074666C (CGameObjectArray status OK)
constexpr U kInvalidObject = 0x7F000000u;                              // immediate (006D8464, 006DAA28)
constexpr float kHundred_00741838 = 100.0f;                            // .rdata 00741838 (42C80000h)
constexpr float kFifty_00740cc0 = 50.0f;                               // .rdata 00740CC0 (42480000h)
constexpr float kTwo32_0073d770 = 4294967296.0f;                       // .rdata 0073D770 (4F800000h)
constexpr float kHundredth_0073dfec = 0.01f;                           // .rdata 0073DFEC (3C23D70Ah)
constexpr float kHalf_0073e9ac = 0.5f;                                 // .rdata 0073E9AC (3F000000h)
constexpr float kMinusHalf_00743884 = -0.5f;                           // .rdata 00743884 (BF000000h)
constexpr float kTen_007455c8 = 10.0f;                                 // .rdata 007455C8 (41200000h)
constexpr U kOneBits = 0x3F800000u;                                    // immediate 1.0f
constexpr U kOneAndHalfBits = 0x3FC00000u;                             // immediate 1.5f
const char kEmpty_0073d71c[] = "";                                     // .rdata 0073D71C
const char kDefault_0073e304[] = "DEFAULT";                            // .rdata 0073E304
const char kSoundImpact_0074b780[] = "SoundImpact";                    // .rdata 0074B780
const char kSoundDuration_0074b78c[] = "SoundDuration";                // .rdata 0074B78C

struct Quaternion { float w, x, y, z; };
static_assert(sizeof(Quaternion) == 16, "Quaternion");
struct HitInfo { alignas(4) unsigned char bytes[0x64]; };              // CHitInfo (fields to +60h)

void* aoe_methods();

template <class Fn> Fn vs(const void* object, unsigned offset) noexcept { return vslot<Fn>(object, offset); }
using V0 = void (__fastcall*)(void*, void*);                           // slot, no stack args
using V0p = void* (__fastcall*)(void*, void*);
using V1 = void (__fastcall*)(void*, void*, U);                        // slot, one stack word
using V1p = void* (__fastcall*)(void*, void*, U);                      // slot, one stack word, EAX
using VP = void (__fastcall*)(void*, void*, const void*);              // slot, one pointer
using DeletingDtor = void* (__fastcall*)(void*, void*, U);             // slot 0
using PartLoadModel = U (__fastcall*)(void*, void*, const void*, U);   // part +0C (resref*, FFh) RET 8
using PartAnimLength = double (__fastcall*)(void*, void*, U);          // part +38 (ST0) RET 4
using PartPlay = U (__fastcall*)(void*, void*, U, float, U, U);        // part +54 RET 10h
using PartEnvMap = void (__fastcall*)(void*, void*, U, U, const void*);   // part +9C RET 0Ch
using VfxLoadModel = U (__fastcall*)(void*, void*, const void*, U, U); // vfx +78 (resref*, FFh, 8) RET 0Ch
using GetPos = void* (__fastcall*)(void*, void*, void*);               // model +64 (out*) RET 4, EAX = out
using ModelPlace = void (__fastcall*)(void*, void*, void*, Vector3);   // model +5C (hidden result*, Vector) RET 10h
using SceneRay = U (__fastcall*)(void*, void*, Vector3, Vector3, void*, void*);   // scene +38 RET 20h
using EnvMapSlot = U (__fastcall*)(void*, void*, U, U, const void*);   // self +B0 RET 0Ch

// Unwind-only cleanup (original EH state action); every normal exit of the guarded range dismisses it first.
class Unwind {
public:
    Unwind(void (*action)(void*), void* object) noexcept : action_(action), object_(object), armed_(true) {}
    ~Unwind() { if (armed_) action_(object_); }
    void dismiss() noexcept { armed_ = false; }
    Unwind(const Unwind&) = delete;
    Unwind& operator=(const Unwind&) = delete;
private:
    void (*action_)(void*);
    void* object_;
    bool armed_;
};
void free_void(void* p) { live_free(p); }
void destroy_string_void(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
void destruct_object_void(void* s) { kotor_m15e_object_destruct_0063e800(s, nullptr); }
void destroy_strings3_void(void* s) {
    c2da_destroy_array_006fa852(s, 8, 3, reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
}
void destroy_hit_void(void* h) { chit_info_destruct_00456b30(h, nullptr); }

void construct_string(ExoString* s) noexcept(false) {
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr);   // 005B3190
}
void destroy_string(ExoString* s) noexcept(false) { frame_LogStringDtor_005e5c20(s, nullptr); }   // 005E5C20

// __ftol2 (006FAE8C) on an x87 value: truncate to 64 bits, keep the low dword; NaN / infinity / out of range -> 0.
U ftol2_low(double value) noexcept {
    if (!(value > -9223372036854775808.0 && value < 9223372036854775808.0)) return 0;
    return static_cast<U>(static_cast<std::uint64_t>(static_cast<std::int64_t>(value)));
}
// fild of an unsigned dword: fild (signed) then fadd [0073D770] 2^32 when the sign bit is set.
double fild_unsigned(U value) noexcept {
    double v = static_cast<double>(static_cast<I32>(value));
    if (static_cast<I32>(value) < 0) v += static_cast<double>(kTwo32_0073d770);
    return v;
}
// The recurring SetInstanceRandomPosition term: t = rand(); (t div-unsigned __ftol2(extent * scale)) remainder as an
// unsigned float, times 0.01f (value kept on the FPU by the original). A zero divisor faults exactly as `div` does in
// the original (extent * scale < 1, or a NaN extent).
double random_extent(float extent, float scale) noexcept(false) {
    const U t = static_cast<U>(crt_rand_006fc447());                                   // call 006FC447 (edi)
    const U divisor = ftol2_low(static_cast<double>(extent) * static_cast<double>(scale));   // fld / fmul / call 006FAE8C
    const U remainder = t % divisor;                                                    // xor edx,edx / div ecx
    return fild_unsigned(remainder) * static_cast<double>(kHundredth_0073dfec);         // fild / fadd / fmul [0073DFEC]
}
bool rand_odd() noexcept(false) { return crt_rand_006fc447() % 2 != 0; }               // and 80000001h / jns / ... / je

void* c2da_effects() noexcept { return rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x1C); }  // [[[007A3A28]+B8h]+1Ch]
void* c2da_visuals() noexcept { return rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x88); }  // [[[007A3A28]+B8h]+88h]
I32 col(U cell) noexcept { return static_cast<I32>(cell); }
void* part_at(void* self, U index) noexcept { return static_cast<unsigned char*>(rp(self, 0x68)) + index * 0xF0; }
}  // namespace

extern "C" {
// =========================================================================================== 00651680
// CSWCMessage::HandleServerToPlayerAreaOfEffectUpdate_Add. RET (EAX BOOL). OBJECTID id, DWORD row, OBJECTID creator.
// Quirk kept: when LoadEffect fails the new object is neither deleted nor registered (leaked), EAX 0.
std::uint32_t __fastcall recovered_client_handle_aoe_add_00651680(void* self, void*) noexcept(false) {
    U id = kotor_fastcall_00692630(self, nullptr);                     // 0065169B..006516A6 {esp+0Ch}
    const U row = kotor_fastcall_004d6780(self, nullptr, 0x20);        // 006516A2..006516B1 edi
    const U creator = kotor_fastcall_00692630(self, nullptr);          // 006516AF..006516BC ebx
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 006516B8..006516C5 -> 0065176B
    void* const memory = live_operator_new(0x17C);                     // 006516CB..006516D8
    void* aoe = nullptr;                                               // 006516F3
    if (memory != nullptr) {                                           // 006516DC..006516E6
        Unwind guard(&free_void, memory);                              // EH state 0 (006516DE)
        aoe = recovered_client_aoe_construct_006d8310(memory, nullptr);   // 006516E8..006516EF
        guard.dismiss();                                               // 006516F7 state -1
    }
    if (aoe == nullptr) return 0;                                      // 006516F5..006516FF
    if (recovered_client_aoe_load_effect_006d8490(aoe, nullptr, static_cast<I32>(row)) == 0) return 0;   // 00651701..0065170B
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0065170D..0065171A edi
    vs<V1>(aoe, 0x04)(aoe, nullptr, id);                               // 0065171D..00651726 SetId
    vs<VP>(aoe, 0x84)(aoe, nullptr, area);                             // 00651729..0065172E SetArea
    st<U>(aoe, 0x178, creator);                                        // 00651734
    st<U>(aoe, 0x144, rd32(area, 0x184));                              // 0065173A..00651740 scene
    void* const objects = kotor_fastcall_005ed570(client_app(), nullptr);   // 00651746..0065174F
    if (recovered_object_array_add_external_004d7ff0(objects, nullptr, &id, aoe, 1) != kArraySuccess_0074666c)
        return 0;                                                      // 00651754..00651769
    void* const master = kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr);   // 0065177F..00651787
    recovered_client_ai_master_add_object_00604a20(master, nullptr, aoe);   // 0065178C..0065178F
    return 1;                                                          // 0065179A
}

// =========================================================================================== class
// 006D8310 CSWCAreaOfEffectObject::CSWCAreaOfEffectObject() RET, EAX this.
void* __fastcall recovered_client_aoe_construct_006d8310(void* self, void*) noexcept(false) {
    kotor_m15a_object_construct_0063e530(self, nullptr, kAreaOfEffectType_0074663a);   // 006D832B..006D8333
    Unwind unwind(&destruct_object_void, self);                        // EH state 0 (006D8342)
    st<U>(self, 0, word_of(aoe_methods()));                            // 006D8346 vptr 00757E28
    kotor_fastcall_00405ed0(at(self, 0x158), nullptr);                 // 006D8338..006D834C CResRef()
    U day = 0, time = 0;                                               // {esp+0Ch} {esp+10h}
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &day, &time);   // 006D8351..006D836A
    st<U>(self, 0x40, time);                                           // 006D836F..006D8377
    st<U>(self, 0x3C, day);                                            // 006D837A
    st<U>(self, 0x30, 0); st<U>(self, 0x34, kOneBits); st<U>(self, 0x38, 0);   // 006D837D..006D8393 orientation (0,1,0)
    st<U>(self, 0x24, 0); st<U>(self, 0x28, 0); st<U>(self, 0x2C, 0);  // 006D8396..006D83A0 position
    st<U>(self, 0x124, 0); st<U>(self, 0x128, 0); st<U>(self, 0x12C, 0);   // 006D83B7..006D83C3
    st<U8>(self, 0x13F, 0);                                            // 006D83C9
    for (U i = 0; i < 3; ++i) {                                        // 006D83CF..006D83EC
        st<U8>(self, 0x13C + i, 0);                                    // 006D83E0
        st<U>(self, 0x114 + i * 4, 0);                                 // 006D83E2
        st<U>(self, 0x108 + i * 4, 0);                                 // 006D83E5
    }
    st<U>(self, 0xFC, 0); st<U>(self, 0x130, 0); st<U>(self, 0x134, 0); st<U>(self, 0x120, 0);   // 006D83FA..006D840C
    st<U>(self, 0x138, 0); st<U>(self, 0x140, 0); st<U>(self, 0x144, 0); st<U>(self, 0x104, 0);  // 006D8412..006D8424
    st<U>(self, 0x148, 0); st<U>(self, 0x100, 0); st<U>(self, 0x14C, 0); st<U>(self, 0x150, 0);  // 006D842A..006D843C
    st<U>(self, 0x154, 0);                                             // 006D8442
    ResRef discarded;                                                  // {esp+18h}
    unresolved_ref_assign_00406290(at(self, 0x158), nullptr, &discarded, kEmpty_0073d71c);   // 006D83EE..006D8448
    st<U>(self, 0x168, 0); st<U>(self, 0x16C, 0); st<U>(self, 0x174, 0);   // 006D8451..006D845D
    st<U>(self, 0x178, kInvalidObject);                                // 006D8464
    st<U>(self, 0x170, 0xFFFFFFFFu);                                   // 006D846E
    unwind.dismiss();
    return self;                                                       // 006D8478
}

// 006D9820 CSWCAreaOfEffectObject::~CSWCAreaOfEffectObject() RET. Quirk kept: +154h is deleted but not cleared.
void __fastcall recovered_client_aoe_destruct_006d9820(void* self, void*) noexcept(false) {
    st<U>(self, 0, word_of(aoe_methods()));                            // 006D983D vptr 00757E28
    Unwind unwind(&destruct_object_void, self);                        // EH state 0 (006D984B)
    void* const sound = rp(self, 0x154);                               // 006D9843
    if (sound != nullptr) vs<DeletingDtor>(sound, 0)(sound, nullptr, 1);   // 006D9849..006D9859
    recovered_client_aoe_unload_model_006d90d0(self, nullptr);         // 006D985B..006D985D
    unwind.dismiss();                                                  // 006D9864 state -1
    kotor_m15e_object_destruct_0063e800(self, nullptr);                // 006D9862..006D986C
}

// 006D9890 scalar deleting destructor (slot +000) RET 4.
void* __fastcall recovered_client_aoe_scalar_deleting_destructor_006d9890(void* self, void*, U flags) noexcept(false) {
    recovered_client_aoe_destruct_006d9820(self, nullptr);             // 006D9891..006D9893
    if ((flags & 1u) != 0) live_free(self);                            // 006D9898..006D98A5
    return self;                                                       // 006D98A8
}

// 006D90D0 CSWCAreaOfEffectObject::UnloadModel() (slot +080) RET.
void __fastcall recovered_client_aoe_unload_model_006d90d0(void* self, void*) noexcept(false) {
    static const unsigned kArrays[] = {0x104, 0x148, 0x140, 0x100, 0xFC};   // 006D90D4..006D914D (this order)
    for (unsigned offset : kArrays) {
        void* const block = rp(self, offset);
        if (block != nullptr) { live_free(block); st<U>(self, offset, 0); }
    }
    void* parts = rp(self, 0x68);                                      // 006D9153
    if (parts != nullptr) {
        for (I32 i = 0; i < ld<I32>(self, 0x120); ++i) {               // 006D9158..006D918B (count re-read)
            void* const part = part_at(self, static_cast<U>(i));       // 006D9170..006D9176
            vs<V0>(part, 0x78)(part, nullptr);                         // 006D9179 part UnloadModel
        }
        parts = rp(self, 0x68);                                        // 006D918E
        if (parts != nullptr) {                                        // 006D9191..006D9194
            void* const block = static_cast<unsigned char*>(parts) - 4;   // 006D919B
            if (rd32(block, 0) != 0) vs<DeletingDtor>(parts, 0)(parts, nullptr, 3);   // 006D9196..006D91A4 vector delete
            else live_free(block);                                     // 006D91A8..006D91A9
        }
        st<U>(self, 0x68, 0);                                          // 006D91B1
    }
    void* const vfx = rp(self, 0x14C);                                 // 006D91B4
    if (vfx != nullptr) {                                              // 006D91BA..006D91BC
        vs<DeletingDtor>(vfx, 0)(vfx, nullptr, 1);                     // 006D91BE..006D91C2
        st<U>(self, 0x14C, 0);                                         // 006D91C4
    }
}

// 006D91D0 UnloadModel(BYTE part) (slot +07C) RET 4: part FFh (all) -> this->UnloadModel().
void __fastcall recovered_client_aoe_unload_model_part_006d91d0(void* self, void*, U part) noexcept(false) {
    if (static_cast<U8>(part) != 0xFF) return;                         // 006D91D0..006D91D5
    vs<V0>(self, 0x80)(self, nullptr);                                 // 006D91D7..006D91D9
}

// 006D91F0 SetAnimation(WORD animation, BYTE index) RET 8. EAX: 0 without instances, else part slot 54h's EAX.
U __fastcall recovered_client_aoe_set_animation_006d91f0(void* self, void*, U animation, U index) noexcept(false) {
    if (rp(self, 0x68) == nullptr) return 0;                           // 006D91F4..006D91FF
    const U i = static_cast<U8>(index);                                // 006D9211 movzx
    st<U16>(rp(self, 0x148), i * 2, static_cast<U16>(animation));      // 006D920A, 006D9216
    void* const part = part_at(self, i);                               // 006D921C..006D922A / 006D9279..006D9287
    if (static_cast<U16>(animation) == 1 || static_cast<U16>(animation) == 3) {   // 006D9206 / 006D9273
        const U a = static_cast<U16>(animation);
        const U time = ftol2_low(vs<PartAnimLength>(part, 0x38)(part, nullptr, a));   // 006D922D..006D9232 / 006D928A..006D928F
        st<U>(rp(self, 0x104), i * 4, time);                           // 006D9237..006D923D / 006D9294..006D929A
        if (rd32(rp(self, 0x104), i * 4) == 0) st<U>(rp(self, 0x104), i * 4, 1);   // 006D9240..006D9250 / 006D929D..006D92AD
        void* const part2 = part_at(self, i);                          // 006D9256..006D9260 / 006D92B3..006D92BD
        return vs<PartPlay>(part2, 0x54)(part2, nullptr, a, 1.0f, 1, 0);   // 006D925C..006D926A / 006D92B9..006D92C7
    }
    st<U>(rp(self, 0x104), i * 4, 0);                                  // 006D92D0..006D92D6
    return vs<PartPlay>(part, 0x54)(part, nullptr, animation, 1.0f, 0, 0);   // 006D92DD..006D92F5 (full dword pushed)
}

// 006D9300 (Ghidra FUN_006d9300) RET 4: instance `index` takes its group's duration (+108h + 4*group).
void __fastcall recovered_client_aoe_reset_instance_duration_006d9300(void* self, void*, I32 index) noexcept {
    if (rp(self, 0x68) == nullptr) return;                             // 006D9300..006D9305
    I32 sum = 0;                                                       // 006D930C
    U8 group = 0;                                                      // 006D930E
    do {
        sum += ld<I32>(self, 0x114 + group * 4u);                      // 006D9311..006D9314
        if (index < sum) break;                                        // 006D931B..006D931D (jl)
        ++group;                                                       // 006D931F
    } while (group < 3);                                               // 006D9321..006D9324 (jb)
    if (group == 3) return;                                            // 006D9326..006D932A
    st<U>(rp(self, 0xFC), static_cast<U>(index) * 4, rd32(self, 0x108 + group * 4u));   // 006D932C..006D933C
}

// 006D9520 SetMoveTimer(DWORD timer, model*) RET 8: the instance whose slot-8(FFh) model is `model` gets the delay.
void __fastcall recovered_client_aoe_set_move_timer_006d9520(void* self, void*, U timer, void* model) noexcept(false) {
    if (rp(self, 0x68) == nullptr) return;                             // 006D9523..006D9528
    I32 i = 0;                                                         // 006D9531
    for (; i < ld<I32>(self, 0x120); ++i) {                            // 006D952A..006D9535 / 006D9555..006D9564
        void* const part = part_at(self, static_cast<U>(i));           // 006D9540..006D9546
        if (vs<V1p>(part, 0x08)(part, nullptr, 0xFF) == model) break;  // 006D9549..006D9553
    }
    if (i == ld<I32>(self, 0x120)) return;                             // 006D9568..006D956E
    st<U>(rp(self, 0x100), static_cast<U>(i) * 4, timer);              // 006D9570..006D957A
}

// 0063BFA0 CSWCArea::GetEnvMap(CResRef* out) RET 4: 16-byte copy of the area's +BCh CResRef, EAX = out.
void* __fastcall kotor_fastcall_0063bfa0(void* area, void*, void* out) noexcept {
    std::memcpy(out, at(area, 0xBC), 16);                              // 0063BFA0..0063BFC0
    return out;                                                        // 0063BFA0 (eax)
}

// 006D9590 SetEnvMap(a, b, CResRef& map) (slot +0B0) RET 0Ch, EAX BOOL. "DEFAULT" takes the current area's map.
U __fastcall recovered_client_aoe_set_env_map_006d9590(void* self, void*, U a, U b, void* resref) noexcept(false) {
    ExoString name;                                                    // {esp+10h}
    construct_string(&name);                                           // 006D95B3..006D95B7
    Unwind guard(&destroy_string_void, &name);                         // EH state 0 (006D95C1)
    U result = 0;
    bool apply = false;
    if (rp(self, 0x68) != nullptr) {                                   // 006D95BC..006D95C9
        void* const module = kotor_fastcall_005ed530(client_app(), nullptr);   // 006D95CB..006D95D3 esi
        if (c2da_equal_text_004060d0(resref, nullptr, kDefault_0073e304) != 0) {   // 006D95D8..006D95EB
            if (module != nullptr && rp(module, 0x48) != nullptr) {    // 006D95ED..006D95F6
                ResRef copy;                                           // {esp+18h}
                std::memcpy(&copy, at(rp(module, 0x48), 0xBC), sizeof copy);   // 006D95F8..006D961E
                c2da_ref_to_string_00405f70(&copy, nullptr, &name);    // 006D9611..006D9622
            }
            apply = async_string_length_005e5790(&name, nullptr) != 0; // 006D9627..006D9632
        } else {
            c2da_ref_to_string_00405f70(resref, nullptr, &name);       // 006D965B..006D9662
            apply = true;
        }
    }
    if (apply) {
        for (I32 i = 0; i < ld<I32>(self, 0x120); ++i) {               // 006D9667..006D9671 / 006D969F..006D96AE
            ResRef ref;                                                // {esp+18h}
            async_ref_construct_string_00406d60(&ref, nullptr, &name); // 006D9675..006D967E
            void* const part = part_at(self, static_cast<U>(i));       // 006D9683..006D9689
            vs<PartEnvMap>(part, 0x9C)(part, nullptr, a, b, &ref);     // 006D968C..006D9699
        }
        result = 1;                                                    // 006D96C7
    }
    guard.dismiss();                                                   // 006D9638 / 006D96B4 state -1
    destroy_string(&name);                                             // 006D9640 / 006D96BC
    return result;
}

// 006D96E0 SetEnvMap(BYTE part, a, b, CResRef& map) (slot +0AC) RET 10h, EAX BOOL. Only part FFh (all) applies.
U __fastcall recovered_client_aoe_set_env_map_part_006d96e0(void* self, void*, U part, U a, U b,
                                                            void* resref) noexcept(false) {
    ExoString name;                                                    // {esp+10h}
    construct_string(&name);                                           // 006D9703..006D9707
    Unwind guard(&destroy_string_void, &name);                         // EH state 0 (006D9711)
    U result = 0;
    bool apply = false;
    if (rp(self, 0x68) != nullptr && static_cast<U8>(part) == 0xFF) {  // 006D970C..006D971F
        void* const module = kotor_fastcall_005ed530(client_app(), nullptr);   // 006D9721..006D9729 esi
        if (c2da_equal_text_004060d0(resref, nullptr, kDefault_0073e304) != 0) {   // 006D972E..006D9741
            if (module != nullptr && rp(module, 0x48) != nullptr) {    // 006D9743..006D974C
                ResRef copy;                                           // {esp+18h}
                void* const map = kotor_fastcall_0063bfa0(rp(module, 0x48), nullptr, &copy);   // 006D974E..006D9758
                c2da_ref_to_string_00405f70(map, nullptr, &name);      // 006D975D..006D975F
            }
            apply = async_string_length_005e5790(&name, nullptr) != 0; // 006D9764..006D976F
        } else {
            c2da_ref_to_string_00405f70(resref, nullptr, &name);       // 006D9798..006D979F
            apply = true;
        }
    }
    if (apply) {
        for (I32 i = 0; i < ld<I32>(self, 0x120); ++i) {               // 006D97A4..006D97AE / 006D97DC..006D97EB
            ResRef ref;                                                // {esp+18h}
            async_ref_construct_string_00406d60(&ref, nullptr, &name); // 006D97B2..006D97BB
            void* const p = part_at(self, static_cast<U>(i));          // 006D97C0..006D97C6
            vs<PartEnvMap>(p, 0x9C)(p, nullptr, a, b, &ref);           // 006D97C9..006D97D6
        }
        result = 1;                                                    // 006D9804
    }
    guard.dismiss();                                                   // 006D9775 / 006D97F1 state -1
    destroy_string(&name);                                             // 006D977D / 006D97F9
    return result;
}

// 006D8490 CSWCAreaOfEffectObject::LoadEffect(int row) RET 4, EAX BOOL. Row of the effects 2DA [[rules+B8h]+1Ch].
// Quirks kept: the three percentage floats (B+44h..4Ch) are read even when GetFLOATEntry fails (stack garbage in the
// original: here the same uninitialized locals); a total instance count < 1 returns TRUE without setting +170h.
U __fastcall recovered_client_aoe_load_effect_006d8490(void* self, void*, I32 row) noexcept(false) {
    ExoString models[3];                                               // B+70h, B+78h, B+80h
    crt_eh_vector_constructor_006fb55f(models, 8, 3, reinterpret_cast<void*>(&kotor_menu20::ConstructString005b3190),
                                       reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));   // 006D84AC..006D84C4
    Unwind guard_models(&destroy_strings3_void, models);               // EH state 0 (006D84CF)
    ExoString shape_2c;                                                // B+2Ch
    construct_string(&shape_2c);                                       // 006D84CB..006D84D6
    Unwind guard_2c(&destroy_string_void, &shape_2c);                  // EH state 1 (006D84DF)
    ExoString label_24;                                                // B+24h
    construct_string(&label_24);                                       // 006D84DB..006D84E7
    Unwind guard_24(&destroy_string_void, &label_24);                  // EH state 2 (006D8511)
    U result = 1;
    bool proceed = true;
    {
        I32 orient = 0;                                                // B+1Ch
        if (c2da_current_get_int_entry_00413510(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a21ec), &orient) == 0)
            orient = 0;                                                // 006D84EC..006D8526
        st<U>(self, 0x16C, orient != 0 ? 1u : 0u);                     // 006D852A / 006D86EB..006D86F5
    }
    if (c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a21e8),
                                               &label_24) != 0) {      // 006D8530..006D8555
        ExoString model_10;                                            // B+10h
        construct_string(&model_10);                                   // 006D855B..006D855F
        Unwind guard_10(&destroy_string_void, &model_10);              // EH state 3 (006D8588)
        const U columns[6] = {kotor_rules_cell32_007a21b4, kotor_rules_cell32_007a21b8, kotor_rules_cell32_007a21bc,
                              kotor_rules_cell32_007a21c0, kotor_rules_cell32_007a21c4, kotor_rules_cell32_007a21c8};
        bool found = false;
        for (U c : columns) {                                          // 006D8564..006D8691 (six probes)
            if (object_vfx_table_00413ec0(c2da_visuals(), nullptr, &label_24, col(c), &model_10) != 0) { found = true; break; }
        }
        if (!found) {
            proceed = false;                                           // 006D8693..006D86E6: EAX 0
            result = 0;
        } else {
            void* const memory = live_operator_new(0x130);             // 006D8704..006D8711
            void* vfx = nullptr;                                       // 006D872D
            if (memory != nullptr) {                                   // 006D8715..006D871F
                Unwind guard_vfx(&free_void, memory);                  // EH state 4 (006D8717)
                vfx = area_vfx_construct_0068fe70(memory, nullptr, 0); // 006D8721..006D8729
                guard_vfx.dismiss();                                   // 006D8731 state 3
            }
            if (vfx != nullptr) {                                      // 006D872F..006D8739
                ResRef ref;                                            // B+50h
                async_ref_construct_string_00406d60(&ref, nullptr, &model_10);   // 006D873B..006D8744
                if (vs<VfxLoadModel>(vfx, 0x78)(vfx, nullptr, &ref, 0xFF, 8) == 0) {   // 006D8749..006D875C
                    vs<DeletingDtor>(vfx, 0)(vfx, nullptr, 1);         // 006D8760..006D8766
                    vfx = nullptr;                                     // 006D876A
                } else {
                    st<U>(self, 0x150, 0);                             // 006D8770
                    st<U>(vfx, 0x11C, 0);                              // 006D8776
                    st<U>(vfx, 0x118, 0);                              // 006D877C
                    area_vfx_play_00690510(vfx, nullptr, 1);           // 006D875E..006D8782
                }
            }
            st<U>(self, 0x14C, word_of(vfx));                          // 006D878B
        }
        guard_10.dismiss();                                            // 006D8697 / 006D8791 state 2
        destroy_string(&model_10);                                     // 006D869F / 006D8799
    }
    if (proceed) {
        c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a21f0), &shape_2c);   // 006D879E..006D87BB
        const U8 shape = static_cast<U8>(*async_string_cstr_005e5670(&shape_2c, nullptr));   // 006D87C0..006D87C9
        st<U8>(self, 0x13F, shape);                                    // 006D87CD
        if (shape == 0x43) {                                           // 006D87CB..006D87D3 'C'
            float* const radius = static_cast<float*>(at(self, 0x124)); // 006D87E6
            if (kotor_m15b_c2da_get_float_entry_00413350(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a21f4), radius) == 0)
                st<U>(self, 0x124, kOneBits);                          // 006D8839..006D884A
        } else if (shape == 0x52) {                                    // 006D87EE..006D87F0 'R'
            if (kotor_m15b_c2da_get_float_entry_00413350(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a21f8),
                                                         static_cast<float*>(at(self, 0x128))) == 0)
                st<U>(self, 0x128, kOneBits);                          // 006D87F2..006D881B
            if (kotor_m15b_c2da_get_float_entry_00413350(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a21fc),
                                                         static_cast<float*>(at(self, 0x12C))) == 0)
                st<U>(self, 0x12C, kOneBits);                          // 006D8821..006D884A
        }
        // instance model names (B+70h / 78h / 80h): option byte bit 7 selects the low-detail columns only.
        const std::int8_t detail = ld<std::int8_t>(kotor_fastcall_005ed700(client_app(), nullptr), 0);   // 006D8850..006D885E
        if (detail < 0) {                                              // 006D886E..006D8877 (js)
            c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2204), &models[0]);   // 006D8923..006D892B
            c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a220c), &models[1]);   // 006D8930..006D894D
            c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2214), &models[2]);   // 006D8952..006D8972
        } else {
            if (c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2200), &models[0]) == 0)
                c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2204), &models[0]);   // 006D887D..006D88AB
            if (c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2208), &models[1]) == 0)
                c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a220c), &models[1]);   // 006D88B0..006D88F3
            if (c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2210), &models[2]) == 0)
                c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2214), &models[2]);   // 006D88F8..006D8921 / 006D8952..006D8972
        }
        c2da_current_get_int_entry_00413510(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2218), static_cast<I32*>(at(self, 0x114)));   // 006D8977..006D8996
        c2da_current_get_int_entry_00413510(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a221c), static_cast<I32*>(at(self, 0x118)));   // 006D899B..006D89BB
        c2da_current_get_int_entry_00413510(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2220), static_cast<I32*>(at(self, 0x11C)));   // 006D89C0..006D89E0
        c2da_current_get_int_entry_00413510(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2224), static_cast<I32*>(at(self, 0x108)));   // 006D89E5..006D8A05
        c2da_current_get_int_entry_00413510(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2228), static_cast<I32*>(at(self, 0x10C)));   // 006D8A0A..006D8A29
        c2da_current_get_int_entry_00413510(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a222c), static_cast<I32*>(at(self, 0x110)));   // 006D8A2E..006D8A4D
        float percent[3];                                              // B+44h..B+4Ch (no initialisation in the original)
        kotor_m15b_c2da_get_float_entry_00413350(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2230), &percent[0]);   // 006D8A52..006D8A6F
        kotor_m15b_c2da_get_float_entry_00413350(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2234), &percent[1]);   // 006D8A74..006D8A91
        kotor_m15b_c2da_get_float_entry_00413350(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2238), &percent[2]);   // 006D8A96..006D8AB3
        st<U>(self, 0x44, 0);                                          // 006D8AB8
        for (U i = 0; i < 3; ++i) {                                    // 006D8ABB..006D8AF5
            const U8 value = static_cast<U8>(ftol2_low(static_cast<double>(percent[i]) * static_cast<double>(kHundred_00741838)));   // 006D8AD0..006D8ADF
            st<U8>(self, 0x13C + i, value > 0x64 ? static_cast<U8>(0x64) : value);   // 006D8ADD..006D8AE4 (jbe)
        }
        ExoString unused_3c;                                           // B+3Ch
        construct_string(&unused_3c);                                  // 006D8AF7..006D8AFB
        Unwind guard_3c(&destroy_string_void, &unused_3c);             // EH state 5 (006D8B04)
        ExoString sound_34;                                            // B+34h
        construct_string(&sound_34);                                   // 006D8B00..006D8B0C
        Unwind guard_34(&destroy_string_void, &sound_34);              // EH state 6 (006D8B2E)
        if (c2da_current_get_string_entry_00413190(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a223c), &sound_34) != 0) {   // 006D8B11..006D8B3D
            ResRef discarded;                                          // B+60h
            async_ref_assign_string_004061f0(at(self, 0x158), nullptr, &discarded, &sound_34);   // 006D8B3F..006D8B4F
        }
        float chance = 0.0f;                                           // B+20h
        if (kotor_m15b_c2da_get_float_entry_00413350(c2da_effects(), nullptr, row, col(kotor_rules_cell32_007a2240), &chance) == 0)
            chance = fbits(0);                                         // 006D8B54..006D8B7B (EAX 0 stored)
        st<U>(self, 0x168, ftol2_low(static_cast<double>(chance) * static_cast<double>(kHundred_00741838)));   // 006D8B7F..006D8B94
        const I32 total = ld<I32>(self, 0x11C) + ld<I32>(self, 0x118) + ld<I32>(self, 0x114);   // 006D8B8E..006D8BA8
        st<I32>(self, 0x120, total);                                   // 006D8BAE
        if (total >= 1) {                                              // 006D8BAB..006D8BB4 (jge)
            result = 1;
            // instances: CRT vector new of `total` CSWCAnimBaseVisualEffect (0xF0), count stored at block[0].
            void* const block = live_operator_new(static_cast<U>(total) * 0xF0u + 4u);   // 006D8C0D..006D8C21
            void* parts = nullptr;                                     // 006D8C50
            if (block != nullptr) {                                    // 006D8C25..006D8C2F
                Unwind guard_parts(&free_void, block);                 // EH state 7 (006D8C27)
                st<I32>(block, 0, total);                              // 006D8C45
                crt_eh_vector_constructor_006fb55f(at(block, 4), 0xF0, total,
                                                   reinterpret_cast<void*>(&area_vfx_model_construct_006e5ce0),
                                                   reinterpret_cast<void*>(&area_vfx_model_destroy_006e5d00));   // 006D8C31..006D8C47
                parts = at(block, 4);                                  // 006D8C4C
                guard_parts.dismiss();                                 // 006D8C54 state 6
            }
            st<U>(self, 0x68, word_of(parts));                         // 006D8C5C
            bool ok = parts != nullptr;                                // 006D8C52..006D8C5F
            if (ok) { void* const p = live_operator_new(rd32(self, 0x120) << 2); st<U>(self, 0xFC, word_of(p)); ok = p != nullptr; }    // 006D8C65..006D8C7F
            if (ok) { void* const p = live_operator_new(rd32(self, 0x120) << 2); st<U>(self, 0x100, word_of(p)); ok = p != nullptr; }   // 006D8C81..006D8C9B
            if (ok) { void* const p = live_operator_new(rd32(self, 0x120) << 2); st<U>(self, 0x140, word_of(p)); ok = p != nullptr; }   // 006D8C9D..006D8CB7
            if (ok) { void* const p = live_operator_new(rd32(self, 0x120) << 2); st<U>(self, 0x104, word_of(p)); ok = p != nullptr; }   // 006D8CB9..006D8CD3
            if (ok) { void* const p = live_operator_new(rd32(self, 0x120) << 1); st<U>(self, 0x148, word_of(p)); ok = p != nullptr; }   // 006D8CD5..006D8CEE
            if (!ok) {
                vs<V0>(self, 0x80)(self, nullptr);                     // 006D8CF0..006D8CF4 UnloadModel
                result = 0;                                            // 006D8D5C
            } else {
                for (I32 i = 0; i < ld<I32>(self, 0x120); ++i) {       // 006D8D63..006D8DB9 (count re-read)
                    st<U>(rp(self, 0x140), static_cast<U>(i) * 4, 0);  // 006D8D70..006D8D76
                    st<U>(rp(self, 0x100), static_cast<U>(i) * 4, 0);  // 006D8D7D..006D8D83
                    st<U>(rp(self, 0xFC), static_cast<U>(i) * 4, 0);   // 006D8D8A..006D8D90
                    st<U16>(rp(self, 0x148), static_cast<U>(i) * 2, 0);   // 006D8D97..006D8D9D
                    st<U>(rp(self, 0x104), static_cast<U>(i) * 4, 0);  // 006D8DA3..006D8DA9
                }
                // three groups: model name, first start delay, delay step (ms).
                struct Group { unsigned count_offset; ExoString* model; U first; U step; };
                const Group groups[3] = {{0x114, &models[0], 0, 0x12C},    // 006D8DBB..006D8E4E
                                         {0x118, &models[1], 0x12C, 0x190},   // 006D8E50..006D8EFC
                                         {0x11C, &models[2], 0x2BC, 0x15E}};  // 006D8F02..006D8FBA
                U instance = 0;                                        // ebp (continues across the groups)
                for (const Group& group : groups) {
                    U delay = group.first;                             // {esp+10h} / {esp+18h}
                    for (I32 j = 0; j < ld<I32>(self, group.count_offset); ++j) {   // count re-read each pass
                        ResRef ref;                                    // B+60h
                        async_ref_construct_string_00406d60(&ref, nullptr, group.model);   // 006D8DD1..006D8DDA etc.
                        void* const part = part_at(self, instance);    // 006D8DDF..006D8DE5
                        if (vs<PartLoadModel>(part, 0x0C)(part, nullptr, &ref, 0xFF) != 0) {   // 006D8DE8..006D8DF7
                            st<U>(rp(self, 0x100), instance * 4, delay);   // 006D8DF9..006D8E05
                            const Vector3 color{1.0f, 1.0f, 1.0f};     // B+50h..B+58h (006D8E14..006D8E24)
                            void* const part2 = part_at(self, instance);   // 006D8E08..006D8E12
                            area_vfx_anim_hilite_006a03e0(part2, nullptr, &color, 3, 0);   // 006D8E03..006D8E2C
                        }
                        delay += group.step;                           // 006D8E3C / 006D8EEC / 006D8FAA
                        ++instance;                                    // 006D8E3B / 006D8EE4 / 006D8FA0
                    }
                }
                if (row == 7) {                                        // 006D8FC0..006D8FC3
                    ResRef map;                                        // B+60h
                    reinterpret_cast<menu_resources::Ref*>(&map)->construct_00406d80(kDefault_0073e304);   // 006D8FC5..006D8FCE
                    vs<EnvMapSlot>(self, 0xB0)(self, nullptr, 1, 1, &map); // 006D8FD3..006D8FE0
                }
                st<I32>(self, 0x170, row);                             // 006D8FEA
                result = 1;                                            // 006D904E
            }
        }
        guard_34.dismiss();                                            // 006D8FF0 state 5
        destroy_string(&sound_34);                                     // 006D8FF8
        guard_3c.dismiss();                                            // 006D9001 state 2
        destroy_string(&unused_3c);                                    // 006D9009
    }
    guard_24.dismiss();                                                // state 1
    destroy_string(&label_24);                                         // 006D86B0 / 006D901A
    guard_2c.dismiss();                                                // state 0
    destroy_string(&shape_2c);                                         // 006D86C1 / 006D902B
    guard_models.dismiss();                                            // state -1
    destroy_strings3_void(models);                                     // 006D86C6..006D86DF / 006D9030..006D9049
    return result;
}

// 006D98B0 SetInstanceRandomPosition(BYTE index) RET 4, EAX BOOL. Picks a random point inside (edge percentage
// passed) or on the rim of the AoE shape, rotates it by the orientation, drops a ray 10 units above/below and places
// the instance on the hit (optionally oriented to the ground normal). Original division faults are kept
// (random_extent).
U __fastcall recovered_client_aoe_set_instance_random_position_006d98b0(void* self, void*, U index) noexcept(false) {
    Vector3 v{0.0f, 0.0f, 0.0f};                                       // B+14h..B+1Ch (006D98D6..006D98E6)
    if (rp(self, 0x68) == nullptr) return 0;                           // 006D98D0..006D98EE
    const U i = static_cast<U8>(index);                                // 006D98F4 movzx
    I32 sum = 0;                                                       // 006D98FC
    U8 group = 0;                                                      // 006D98FE
    do {
        sum += ld<I32>(self, 0x114 + group * 4u);                      // 006D9900..006D9903
        if (static_cast<I32>(i) < sum) break;                          // 006D990A..006D990C (jl)
        ++group;                                                       // 006D990E
    } while (group < 3);                                               // 006D9910..006D9913
    if (group == 3) return 0;                                          // 006D9915..006D9918
    const I32 roll = crt_rand_006fc447() % 100;                        // 006D991E..006D9929 (cdq / idiv)
    const I32 edge = ld<U8>(self, 0x13C + group);                      // 006D992B..006D992E
    const U8 shape = ld<U8>(self, 0x13F);                              // 006D9936
    const float radius = ld<float>(self, 0x124);
    const float width = ld<float>(self, 0x128);
    const float length = ld<float>(self, 0x12C);
    const double half = static_cast<double>(kHalf_0073e9ac);
    if (roll < edge) {                                                 // 006D993C..006D993E (jge -> 006D9C96)
        if (shape == 0x43) {                                           // 006D9944..006D9946 'C': anywhere on the disc
            if (rand_odd()) {                                          // 006D994C..006D995D
                const bool positive = rand_odd();                      // 006D9963..006D9974
                const double x = random_extent(radius, kHundred_00741838);   // 006D9976..006D99A8 / 006D99B0..006D99E2
                v.x = static_cast<float>(positive ? x : -x);           // 006D99E8 fchs / 006D99EA fstp
                const bool positive_y = rand_odd();                    // 006D99EE..006D99FF
                const double r = radius;                               // 006D99FF..006D9A07
                const double root = std::sqrt(r * r - static_cast<double>(v.x) * static_cast<double>(v.x));   // ..006D9A13
                v.y = static_cast<float>(positive_y ? root : -root);   // 006D9A15..006D9A24
            } else {
                const bool positive = rand_odd();                      // 006D9A2F..006D9A40
                const double y = random_extent(radius, kHundred_00741838);   // 006D9A42..006D9A74 / 006D9A7C..006D9AAE
                v.y = static_cast<float>(positive ? y : -y);           // 006D9AB4 / 006D9AB6
                const bool positive_x = rand_odd();                    // 006D9ABA..006D9ACB
                const double r = radius;
                const double root = std::sqrt(r * r - static_cast<double>(v.y) * static_cast<double>(v.y));   // 006D9ACB..006D9ADF
                v.x = static_cast<float>(positive_x ? root : -root);   // 006D9AE1..006D9AF0
            }
        } else if (shape == 0x52) {                                    // 006D9AFB..006D9AFD 'R'
            if (rand_odd()) {                                          // 006D9B03..006D9B14
                const bool positive = rand_odd();                      // 006D9B1A..006D9B2B
                const double x = random_extent(width, kFifty_00740cc0);   // 006D9B2D..006D9B5F / 006D9B67..006D9B99
                v.x = static_cast<float>(positive ? x : -x);           // 006D9B9F / 006D9BA1
                const bool positive_y = rand_odd();                    // 006D9BA5..006D9BB6
                v.y = static_cast<float>(static_cast<double>(length) *
                                         static_cast<double>(positive_y ? kHalf_0073e9ac : kMinusHalf_00743884));   // 006D9BB6..006D9BCF / 006DA065
            } else {
                const bool positive = rand_odd();                      // 006D9BD4..006D9BE5
                const double y = random_extent(length, kFifty_00740cc0);   // 006D9BE7..006D9C19 / 006D9C21..006D9C53
                v.y = static_cast<float>(positive ? y : -y);           // 006D9C59 / 006D9C5B
                const bool positive_x = rand_odd();                    // 006D9C5F..006D9C70
                v.x = static_cast<float>(static_cast<double>(width) *
                                         static_cast<double>(positive_x ? kHalf_0073e9ac : kMinusHalf_00743884));   // 006D9C70..006D9C8D
            }
        }
    } else {
        if (shape == 0x43) {                                           // 006D9C96..006D9C98 'C' rim
            if (rand_odd()) {                                          // 006D9C9E..006D9CAF
                const bool plus = rand_odd();                          // 006D9CB5..006D9CC6
                const double x = random_extent(radius, kHundred_00741838);   // 006D9CC8..006D9CFA / 006D9D08..006D9D3A
                v.x = static_cast<float>(plus ? x - half : half - x);  // 006D9D00 fsub / 006D9D40 fsubr; 006D9D46
                const double r = radius;
                const float rest = static_cast<float>(std::sqrt(r * r - static_cast<double>(v.x) * static_cast<double>(v.x)));   // 006D9D4A..006D9D64 B+10h
                const bool plus_y = rand_odd();                        // 006D9D66..006D9D77
                const double y = random_extent(rest, kHundred_00741838);   // 006D9D79..006D9DAD / 006D9DBE..006DA059
                v.y = static_cast<float>(plus_y ? y - half : half - y);   // 006D9DB3 / 006DA01F, 006DA05F; 006DA065
            } else {
                const bool plus = rand_odd();                          // 006D9DE9..006D9DFA
                const double y = random_extent(radius, kHundred_00741838);   // 006D9DFC..006D9E2E / 006D9E3C..006D9E6E
                v.y = static_cast<float>(plus ? y - half : half - y);  // 006D9E34 / 006D9E74; 006D9E7A
                const double r = radius;
                const float rest = static_cast<float>(std::sqrt(r * r - static_cast<double>(v.y) * static_cast<double>(v.y)));   // 006D9E7E..006D9E98 B+10h
                const bool plus_x = rand_odd();                        // 006D9E9A..006D9EAB
                const double x = random_extent(rest, kHundred_00741838);   // 006D9EAD..006D9EDD / 006D9EF2..006D9F22
                v.x = static_cast<float>(plus_x ? x - half : half - x);   // 006D9EE3 / 006D9F28; 006D9EE9 / 006D9F2E
            }
        } else if (shape == 0x52) {                                    // 006D9F37..006D9F39 'R' rim
            const bool plus = rand_odd();                              // 006D9F3F..006D9F50
            const double x = random_extent(width, kFifty_00740cc0);    // 006D9F52..006D9F84 / 006D9F92..006D9FC4
            v.x = static_cast<float>(plus ? x - half : half - x);      // 006D9F8A / 006D9FCA; 006D9FD0
            const bool plus_y = rand_odd();                            // 006D9FD4..006D9FE5
            const double y = random_extent(length, kFifty_00740cc0);   // 006D9FE7..006DA019 / 006DA027..006DA059
            v.y = static_cast<float>(plus_y ? y - half : half - y);    // 006DA01F / 006DA05F; 006DA065
        }
    }
    // 006DA069..006DA128: v = RotationArc((0,1,0), orientation) * v + position (each sum rounded to float).
    {
        Quaternion arc;                                                // B+20h
        const Vector3 up{0.0f, 1.0f, 0.0f};                            // B+20h..B+28h temporaries (006DA089..006DA0B9)
        const Vector3 orientation = ld<Vector3>(self, 0x30);           // 006DA071..006DA083
        void* const q = rotation_arc_004ab630(&arc, up, orientation);  // 006DA0B4..006DA0C1
        Vector3 rotated;                                               // B+3Ch
        const Vector3* const r = static_cast<const Vector3*>(quaternion_rotate_004a9bb0(&rotated, q, &v));   // 006DA0C4..006DA0CA
        v = *r;                                                        // 006DA0CF..006DA0DF
        const float z = static_cast<float>(static_cast<double>(v.z) + static_cast<double>(ld<float>(self, 0x2C)));   // 006DA0E3..006DA0E7
        const float y = static_cast<float>(static_cast<double>(v.y) + static_cast<double>(ld<float>(self, 0x28)));   // 006DA0ED..006DA0F3
        const float x = static_cast<float>(static_cast<double>(v.x) + static_cast<double>(ld<float>(self, 0x24)));   // 006DA0FB..006DA101
        v.x = x; v.y = y; v.z = z;                                     // 006DA104..006DA128
    }
    HitInfo hit;                                                       // B+48h
    aurora_hit_info_construct_004578f0(&hit, nullptr, 0, 0xBFFFFFFFu, 0);   // 006DA0F1..006DA12C
    Vector3 out{0.0f, 0.0f, 0.0f};                                     // B+30h (006DA136..006DA146)
    Unwind guard_hit(&destroy_hit_void, &hit);                         // EH state 0 (006DA151)
    void* const scene = rp(rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48), 0x184);   // 006DA131..006DA164 edi
    st<U>(hit.bytes, 0, 1);                                            // 006DA170
    st<U>(hit.bytes, 4, word_of(area_vfx_hitcheck_list_005ed8c0(client_app(), nullptr)));   // 006DA16A..006DA192
    const Vector3 top{v.x, v.y, static_cast<float>(static_cast<double>(v.z) + static_cast<double>(kTen_007455c8))};      // 006DA180..006DA19E
    const Vector3 bottom{v.x, v.y, static_cast<float>(static_cast<double>(v.z) - static_cast<double>(kTen_007455c8))};   // 006DA1A6..006DA1BE
    const U hit_found = vs<SceneRay>(scene, 0x38)(scene, nullptr, top, bottom, &out, &hit);   // 006DA1B4..006DA1EF
    U result = 0;
    if (hit_found != 0) {                                              // 006DA1F2..006DA1F4
        void* const part = part_at(self, i);                           // 006DA234..006DA248
        vs<VP>(part, 0x68)(part, nullptr, &out);                       // 006DA24B..006DA250 SetPosition
        if (rd32(self, 0x16C) != 0) {                                  // 006DA253..006DA25B
            const Vector3 normal = ld<Vector3>(hit.bytes, 0x40);       // 006DA263..006DA282
            const Vector3 z_axis{0.0f, 0.0f, 1.0f};                    // 006DA285..006DA2BB
            Quaternion ground;                                         // B+ACh
            void* const q = rotation_arc_004ab630(&ground, z_axis, normal);   // 006DA2A7..006DA2BE
            void* const part2 = part_at(self, i);                      // 006DA25D..006DA2CA
            vs<VP>(part2, 0x60)(part2, nullptr, q);                    // 006DA2C9..006DA2CC SetOrientation
        }
        result = 1;                                                    // 006DA306
    }
    guard_hit.dismiss();                                               // 006DA1FF / 006DA2D8 state -1
    chit_info_destruct_00456b30(&hit, nullptr);                        // 006DA1F6..006DA22A / 006DA2CF..006DA303 (inlined)
    return result;
}

// 006DA330 CSWCAreaOfEffectObject::UpdateInstancePosition() RET. Starts delayed instances (random position, scene
// add, animation 1, duration, optional one-shot sound), ends expired ones (animation 3), and flags +134h when the
// stopping effect has no instance left running.
void __fastcall recovered_client_aoe_update_instance_position_006da330(void* self, void*) noexcept(false) {
    I32 idle = 0;                                                      // {esp+10h}
    if (rp(self, 0x68) != nullptr) {                                   // 006DA338..006DA344
        for (I32 i = 0; i < ld<I32>(self, 0x120); ++i) {               // 006DA34A..006DA354 / 006DA4C3..006DA4D2
            const U index = static_cast<U>(i);
            const U duration = rd32(rp(self, 0xFC), index * 4);        // 006DA360..006DA369
            if (duration == 0) {                                       // 006DA36B..006DA36D
                if (rd32(self, 0x138) != 0) { ++idle; continue; }      // 006DA373..006DA381
                const U delay = rd32(rp(self, 0x100), index * 4);      // 006DA386..006DA391
                if (delay == 0) continue;                              // 006DA394 (jbe)
                const U elapsed = rd32(self, 0x44);                    // 006DA39A
                if (delay > elapsed) { st<U>(rp(self, 0x100), index * 4, delay - elapsed); continue; }   // 006DA39D..006DA39F, 006DA4BF..006DA4C1
                if (recovered_client_aoe_set_instance_random_position_006d98b0(self, nullptr, index) == 0) continue;   // 006DA3A5..006DA3AF
                if (rd32(rp(self, 0x140), index * 4) == 0) {           // 006DA3B5..006DA3BF
                    void* const part = part_at(self, index);           // 006DA3C1..006DA3C7
                    vs<V1>(part, 0x04)(part, nullptr, rd32(self, 0x144));   // 006DA3CA..006DA3D1 AddToScene(scene)
                    st<U>(rp(self, 0x140), index * 4, 1);              // 006DA3D4..006DA3DA
                }
                recovered_client_aoe_set_animation_006d91f0(self, nullptr, 1, index);   // 006DA3E1..006DA3E6
                recovered_client_aoe_reset_instance_duration_006d9300(self, nullptr, i);   // 006DA3EB..006DA3EE
                st<U>(rp(self, 0x100), index * 4, 0);                  // 006DA3F3..006DA401
                void* const sound_ref = at(self, 0x158);               // 006DA3F9 ebx
                if (ld<U8>(word_ptr(kotor_m15e_slot00c_self_00641db0(sound_ref, nullptr)), 0) == 0) continue;   // 006DA3FF..006DA410
                if (crt_rand_006fc447() % 100 >= ld<I32>(self, 0x168)) continue;   // 006DA416..006DA429 (jge)
                void* const part = part_at(self, index);               // 006DA42F..006DA435
                if (vs<V1p>(part, 0x08)(part, nullptr, 0xFF) == nullptr) continue;   // 006DA438..006DA442
                void* const part2 = part_at(self, index);              // 006DA444..006DA44A
                void* const model = vs<V1p>(part2, 0x08)(part2, nullptr, 0xFF);   // 006DA44C..006DA451
                Vector3 buffer;                                        // B+14h
                const void* const where = vs<GetPos>(model, 0x64)(model, nullptr, &buffer);   // 006DA454..006DA45D
                kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, sound_ref, rd32(where, 0),
                                                          rd32(where, 4), rd32(where, 8), kOneAndHalfBits, 0xC, 0, 0, 0, 0);   // 006DA460..006DA48B
            } else {
                if (ld<U16>(rp(self, 0x148), index * 2) != 2) continue;   // 006DA492..006DA49D
                const U elapsed = rd32(self, 0x44);                    // 006DA49F
                if (duration > elapsed) { st<U>(rp(self, 0xFC), index * 4, duration - elapsed); continue; }   // 006DA4A2..006DA4A4, 006DA4BF..006DA4C1
                recovered_client_aoe_set_animation_006d91f0(self, nullptr, 3, index);   // 006DA4A6..006DA4AB
                st<U>(rp(self, 0xFC), index * 4, 0);                   // 006DA4B0..006DA4B6
            }
        }
    }
    void* const vfx = rp(self, 0x14C);                                 // 006DA4D8
    const U vfx_done = vfx != nullptr ? rd32(vfx, 0x114) : 1u;         // 006DA4DE..006DA4E7
    if (rd32(self, 0x138) != 0 && vfx_done != 0 && idle == ld<I32>(self, 0x120))   // 006DA4ED..006DA505
        st<U>(self, 0x134, 1);                                         // 006DA507
}

// 006DA520 CSWCAreaOfEffectObject::SetPosition(Vector& position) (slot +08C) RET 4. Moves the main visual (optionally
// ground-oriented) and every instance model by the same delta (re-grounded through the scene ray when +16Ch).
void __fastcall recovered_client_aoe_set_position_006da520(void* self, void*, const void* position) noexcept(false) {
    HitInfo hit;                                                       // B+74h
    aurora_hit_info_construct_004578f0(&hit, nullptr, 0, 0xBFFFFFFFu, 0);   // 006DA53F..006DA555
    Vector3 out{0.0f, 0.0f, 0.0f};                                     // B+68h (006DA55F..006DA56F)
    Unwind guard_hit(&destroy_hit_void, &hit);                         // EH state 0 (006DA57A)
    void* const scene = rp(rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48), 0x184);   // 006DA55A..006DA595 edi
    void* const vfx = rp(self, 0x14C);                                 // 006DA58A
    if (vfx != nullptr) {                                              // 006DA590..006DA5A2
        vs<VP>(vfx, 0x8C)(vfx, nullptr, position);                     // 006DA5A8..006DA5AB
        if (scene != nullptr && rd32(self, 0x16C) != 0) {              // 006DA5B1..006DA5C1
            st<U>(hit.bytes, 0, 1);                                    // 006DA5CC
            st<U>(hit.bytes, 4, word_of(area_vfx_hitcheck_list_005ed8c0(client_app(), nullptr)));   // 006DA5C7..006DA5DC
            const Vector3 p = ld<Vector3>(position, 0);                // 006DA5E0..006DA607 (ebp)
            const Vector3 top{p.x, p.y, static_cast<float>(static_cast<double>(p.z) + static_cast<double>(kTen_007455c8))};      // 006DA5E0..006DA603
            const Vector3 bottom{p.x, p.y, static_cast<float>(static_cast<double>(p.z) - static_cast<double>(kTen_007455c8))};   // 006DA618..006DA62F
            if (vs<SceneRay>(scene, 0x38)(scene, nullptr, top, bottom, &out, &hit) != 0) {   // 006DA60E..006DA65D
                void* const model = gob_get_property_0x68_0063c070(rp(self, 0x14C), nullptr);   // 006DA65F..006DA665
                const Vector3 normal = ld<Vector3>(hit.bytes, 0x40);   // 006DA66C..006DA68F
                const Vector3 z_axis{0.0f, 0.0f, 1.0f};                // 006DA692..006DA6C8
                Quaternion ground;                                     // B+E4h
                void* const q = rotation_arc_004ab630(&ground, z_axis, normal);   // 006DA6B4..006DA6CB
                vs<VP>(model, 0x60)(model, nullptr, q);                // 006DA6D0..006DA6D8 SetOrientation
            }
        }
    }
    const Vector3 p = ld<Vector3>(position, 0);                        // 006DA6DB..006DA6FD (ebp re-read)
    const float dz = static_cast<float>(static_cast<double>(p.z) - static_cast<double>(ld<float>(self, 0x2C)));   // 006DA6DB..006DA6EB B+50h
    const float dy = static_cast<float>(static_cast<double>(p.y) - static_cast<double>(ld<float>(self, 0x28)));   // 006DA6F3..006DA6F9 B+64h
    const float dx = static_cast<float>(static_cast<double>(p.x) - static_cast<double>(ld<float>(self, 0x24)));   // 006DA6FD..006DA702 B+D8h
    if (rp(self, 0x68) != nullptr) {                                   // 006DA6E9..006DA709
        for (I32 i = 0; i < ld<I32>(self, 0x120); ++i) {               // 006DA70F..006DA719 / 006DA8EA..006DA909
            void* const part = part_at(self, static_cast<U>(i));       // 006DA723..006DA72D
            void* const model = vs<V1p>(part, 0x08)(part, nullptr, 0xFF);   // 006DA72F..006DA737 esi
            if (model == nullptr) continue;                            // 006DA739..006DA73B
            Vector3 buffer;                                            // B+F4h
            const Vector3 at_model = *static_cast<const Vector3*>(vs<GetPos>(model, 0x64)(model, nullptr, &buffer));   // 006DA741..006DA75C
            const float nz = static_cast<float>(static_cast<double>(at_model.z) + static_cast<double>(dz));   // 006DA760..006DA771 fst B+34h
            const float ny = static_cast<float>(static_cast<double>(at_model.y) + static_cast<double>(dy));   // 006DA777..006DA77B
            const float nx = static_cast<float>(static_cast<double>(at_model.x) + static_cast<double>(dx));   // 006DA77F..006DA786
            alignas(4) unsigned char placed[0x10];                     // B+E4h (hidden result of model slot 5Ch)
            vs<ModelPlace>(model, 0x5C)(model, nullptr, placed, Vector3{nx, ny, nz});   // 006DA78A..006DA7B4
            if (scene == nullptr || rd32(self, 0x16C) == 0) continue;  // 006DA7B7..006DA7CB
            kotor_fastcall_00456af0(&hit, nullptr, 0, 0xBFFFFFFFu, 0); // 006DA7D1..006DA7E1 CHitInfo::Reset
            st<U>(hit.bytes, 0, 1);                                    // 006DA7EB
            st<U>(hit.bytes, 4, word_of(area_vfx_hitcheck_list_005ed8c0(client_app(), nullptr)));   // 006DA7E6..006DA82C
            const Vector3 top{nx, ny, static_cast<float>(static_cast<double>(nz) + static_cast<double>(kTen_007455c8))};      // 006DA7FB..006DA812
            const Vector3 bottom{nx, ny, static_cast<float>(static_cast<double>(nz) - static_cast<double>(kTen_007455c8))};   // 006DA816..006DA837
            if (vs<SceneRay>(scene, 0x38)(scene, nullptr, top, bottom, &out, &hit) == 0) continue;   // 006DA809..006DA863
            const Vector3 normal = ld<Vector3>(hit.bytes, 0x40);       // 006DA86C..006DA892
            const Vector3 z_axis{0.0f, 0.0f, 1.0f};                    // 006DA895..006DA8CB
            Quaternion ground;                                         // B+100h
            void* const q = rotation_arc_004ab630(&ground, z_axis, normal);   // 006DA8B7..006DA8CE
            void* const part2 = part_at(self, static_cast<U>(i));      // 006DA869..006DA8DA
            vs<VP>(part2, 0x60)(part2, nullptr, q);                    // 006DA8D9..006DA8DC SetOrientation (on the instance)
        }
    }
    st<Vector3>(self, 0x24, ld<Vector3>(position, 0));                 // 006DA90F..006DA921
    guard_hit.dismiss();                                               // 006DA92D state -1
    chit_info_destruct_00456b30(&hit, nullptr);                        // 006DA924..006DA991 (inlined)
}

// 006DA9B0 CSWCAreaOfEffectObject::AIUpdate() (slot +074) RET. Elapsed time, follow the creator, start the duration
// sound once ("SoundImpact" one-shot + looping "SoundDuration"; lookups that fail leave ""), age the instance
// animations, then either finish (stop timer elapsed: vtbl[94h](0), remove from the object array, delete self) or
// update the instances, and keep the main visual and the sound positioned.
void __fastcall recovered_client_aoe_ai_update_006da9b0(void* self, void*) noexcept(false) {
    U day = 0, time = 0;                                               // B+10h, B+14h
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &day, &time);   // 006DA9CE..006DA9EC
    U diff_day = 0, diff_time = 0;                                     // B+1Ch, B+18h
    recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, day, time,
                                             rd32(self, 0x3C), rd32(self, 0x40), &diff_day, &diff_time);   // 006DA9F1..006DAA1D
    st<U>(self, 0x44, diff_time);                                      // 006DAA2D..006DAA31
    const U creator = rd32(self, 0x178);                               // 006DAA22
    if (creator != kInvalidObject) {                                   // 006DAA28..006DAA34
        void* const object = kotor_fastcall_005ed580(client_app(), nullptr, creator);   // 006DAA36..006DAA40
        if (object != nullptr) {                                       // 006DAA45..006DAA47
            void* const client_object = vs<V0p>(object, 0x0C)(object, nullptr);   // 006DAA49..006DAA4D
            vs<VP>(self, 0x8C)(self, nullptr, at(client_object, 0x24));   // 006DAA50..006DAA58 SetPosition
        }
    }
    if (rd32(self, 0x174) == 0) {                                      // 006DAA5E..006DAA66
        ExoString impact;                                              // B+38h
        construct_string(&impact);                                     // 006DAA6C..006DAA70
        Unwind guard_impact(&destroy_string_void, &impact);            // EH state 0 (006DAA79)
        ExoString duration;                                            // B+30h
        construct_string(&duration);                                   // 006DAA75..006DAA81
        Unwind guard_duration(&destroy_string_void, &duration);        // EH state 1 (006DAA8F)
        {
            ExoString column;                                          // B+20h
            frame_LogStringCtor_005e5a90(&column, nullptr, kSoundImpact_0074b780);   // 006DAA86..006DAA94
            Unwind guard_column(&destroy_string_void, &column);        // EH state 2 (006DAABA)
            const I32 found = c2da_current_get_string_00413270(c2da_effects(), nullptr, ld<I32>(self, 0x170), &column, &impact);   // 006DAA99..006DAABF
            guard_column.dismiss();                                    // 006DAAD0 state 1
            destroy_string(&column);                                   // 006DAACA..006DAAD5
            if (found == 0) c2da_assign_text_005e5140(&impact, nullptr, kEmpty_0073d71c);   // 006DAAC4..006DAAE7
        }
        {
            ExoString column;                                          // B+20h
            frame_LogStringCtor_005e5a90(&column, nullptr, kSoundDuration_0074b78c);   // 006DAAEC..006DAAF5
            Unwind guard_column(&destroy_string_void, &column);        // EH state 3 (006DAB1C)
            const I32 found = c2da_current_get_string_00413270(c2da_effects(), nullptr, ld<I32>(self, 0x170), &column, &duration);   // 006DAAFA..006DAB21
            guard_column.dismiss();                                    // 006DAB32 state 1
            destroy_string(&column);                                   // 006DAB2C..006DAB37
            if (found == 0) c2da_assign_text_005e5140(&duration, nullptr, kEmpty_0073d71c);   // 006DAB26..006DAB49
        }
        {
            ResRef impact_ref;                                         // B+20h
            async_ref_construct_string_00406d60(&impact_ref, nullptr, &impact);   // 006DAB4E..006DAB57
            kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &impact_ref, rd32(self, 0x24),
                                                      rd32(self, 0x28), rd32(self, 0x2C), kOneAndHalfBits, 0xC, 0, 0, 0, 0);   // 006DAB5C..006DAB90
        }
        void* const memory = live_operator_new(8);                     // 006DAB95..006DABA1
        void* source = nullptr;                                        // 006DABCA
        if (memory != nullptr) {                                       // 006DABA5..006DABAC
            Unwind guard_source(&free_void, memory);                   // EH state 4 (006DABA7)
            ResRef duration_ref;                                       // B+40h
            async_ref_construct_string_00406d60(&duration_ref, nullptr, &duration);   // 006DABAE..006DABB7
            source = kotor_sound_rev3::SourceConstruct005d60e0(memory, nullptr, &duration_ref);   // 006DABBC..006DABC3
            guard_source.dismiss();                                    // 006DABCE state 1
        }
        st<U>(self, 0x154, word_of(source));                           // 006DABD3
        if (source != nullptr) {                                       // 006DABCC..006DABD9
            kotor_m15_sound_source_set_3d_005d5910(source, nullptr, 1);   // 006DABDB..006DABDF
            kotor_sound_rev3::SourceSetPriorityGroup005d5900(rp(self, 0x154), nullptr, 0xC);   // 006DABE4..006DABEC
            kotor_m15_sound_source_set_looping_005d59d0(rp(self, 0x154), nullptr, 1);   // 006DABF1..006DABF9
            kotor_m15_sound_source_set_position_005d59e0(rp(self, 0x154), nullptr, at(self, 0x24), 1.5f);   // 006DABFE..006DAC0A
            kotor_fastcall_005d5930(rp(self, 0x154), nullptr);         // 006DAC0F..006DAC15 Play
        }
        st<U>(self, 0x174, 1);                                         // 006DAC1E
        guard_duration.dismiss();                                      // 006DAC28 state 0
        destroy_string(&duration);                                     // 006DAC1A..006DAC2D
        guard_impact.dismiss();                                        // 006DAC36 state -1
        destroy_string(&impact);                                       // 006DAC32..006DAC3E
    }
    // 006DAC43..006DACFA: instance animation timers (a BYTE counter, as in the original).
    if (ld<I32>(self, 0x120) > 0) {                                    // 006DAC43..006DAC4D
        U8 counter = 0;                                                // 006DAC49
        do {
            const U i = counter;                                       // 006DAC53 / 006DACF5 movzx
            const U left = rd32(rp(self, 0x104), i * 4);               // 006DAC55..006DAC5E
            if (left != 0) {                                           // 006DAC60..006DAC62 (jbe)
                const U elapsed = rd32(self, 0x44);                    // 006DAC68
                if (left > elapsed) {                                  // 006DAC6B..006DAC6D (jbe)
                    st<U>(rp(self, 0x104), i * 4, left - elapsed);     // 006DAC6F..006DAC71
                } else {
                    st<U>(rp(self, 0x104), i * 4, 0);                  // 006DAC75
                    const U16 animation = ld<U16>(rp(self, 0x148), i * 2);   // 006DAC7B..006DAC84
                    if (animation == 1) {                              // 006DAC87..006DAC8B
                        if (rp(self, 0x68) != nullptr) {               // 006DAC8D..006DAC92
                            st<U16>(rp(self, 0x148), i * 2, 2);        // 006DAC94
                            st<U>(rp(self, 0x104), i * 4, 0);          // 006DAC99..006DAC9F
                            void* const part = part_at(self, i);       // 006DACA6..006DACBE
                            vs<PartPlay>(part, 0x54)(part, nullptr, 2, 1.0f, 0, 0);   // 006DACAF..006DACC0
                        }
                    } else if (animation == 3) {                       // 006DACC5..006DACC9
                        void* const part = part_at(self, i);           // 006DACCB..006DACDE
                        void* const model = vs<V1p>(part, 0x08)(part, nullptr, 0xFF);   // 006DACD9..006DACE0
                        recovered_client_aoe_set_move_timer_006d9520(self, nullptr, 1, model);   // 006DACE3..006DACE8
                    }
                }
            }
            ++counter;                                                 // 006DACF3
        } while (static_cast<I32>(static_cast<U>(counter)) < ld<I32>(self, 0x120));   // 006DACED..006DACFA
    }
    if (rd32(self, 0x134) != 0) {                                      // 006DAD00..006DAD08
        const U stop = rd32(self, 0x130);                              // 006DAD0A
        const U elapsed = rd32(self, 0x44);                            // 006DAD10
        if (stop <= elapsed) {                                         // 006DAD13..006DAD15 (ja)
            st<U>(self, 0x130, 0);                                     // 006DAD1D
            vs<V1>(self, 0x94)(self, nullptr, 0);                      // 006DAD17..006DAD27
            recovered_object_array_erase_004d8290(kotor_fastcall_005ed570(client_app(), nullptr), nullptr, rd32(self, 4));   // 006DAD2D..006DAD41
            vs<DeletingDtor>(self, 0)(self, nullptr, 1);               // 006DAD46..006DAD4C
            return;                                                    // 006DAD4E..006DAD5F (time not stored)
        }
        st<U>(self, 0x130, stop - elapsed);                            // 006DAD60..006DAD62
    } else {
        recovered_client_aoe_update_instance_position_006da330(self, nullptr);   // 006DAD6A..006DAD6C
    }
    void* const vfx = rp(self, 0x14C);                                 // 006DAD71
    if (vfx != nullptr) {                                              // 006DAD77..006DAD79
        if (rd32(self, 0x150) == 0) {                                  // 006DAD7B..006DAD83
            vs<V1>(vfx, 0x94)(vfx, nullptr, rd32(self, 0x144));        // 006DAD85..006DAD8E
            st<U>(self, 0x150, 1);                                     // 006DAD94
        }
        void* const vfx2 = rp(self, 0x14C);                            // 006DAD9E
        vs<V0>(vfx2, 0x74)(vfx2, nullptr);                             // 006DADA4..006DADA6 AIUpdate
    }
    if (rp(self, 0x154) != nullptr)                                    // 006DADA9..006DADB1
        kotor_m15_sound_source_set_position_005d59e0(rp(self, 0x154), nullptr, at(self, 0x24), 1.5f);   // 006DADB3..006DADBC
    st<U>(self, 0x40, time);                                           // 006DADC1..006DADC9
    st<U>(self, 0x3C, day);                                            // 006DADC5..006DADD1
}
}  // extern "C"

namespace {
// 00757E28 CSWCAreaOfEffectObject method table (82 slots): the CSWCObject table 007516C8 with nine class slots.
// The shared slots are the L15-E table's own entries (copied once, never the raw original addresses).
void* aoe_methods() {
    static void* table[82];
    static bool built = false;
    if (!built) {
        std::memcpy(table, kotor_m15e_object_methods_007516c8, sizeof table);
        table[0x000 / 4] = reinterpret_cast<void*>(&recovered_client_aoe_scalar_deleting_destructor_006d9890);   // 006D9890
        table[0x054 / 4] = reinterpret_cast<void*>(&kotor_m15e_slot00c_self_00641db0);                          // 00641DB0 AsSWCAreaOfEffectObject
        table[0x074 / 4] = reinterpret_cast<void*>(&recovered_client_aoe_ai_update_006da9b0);                   // 006DA9B0 AIUpdate
        table[0x078 / 4] = reinterpret_cast<void*>(&mp_slot30_ai_zero_0066b360);                                // 0066B360 LoadModel -> 0
        table[0x07C / 4] = reinterpret_cast<void*>(&recovered_client_aoe_unload_model_part_006d91d0);           // 006D91D0
        table[0x080 / 4] = reinterpret_cast<void*>(&recovered_client_aoe_unload_model_006d90d0);                // 006D90D0 UnloadModel
        table[0x08C / 4] = reinterpret_cast<void*>(&recovered_client_aoe_set_position_006da520);                // 006DA520 SetPosition
        table[0x0AC / 4] = reinterpret_cast<void*>(&recovered_client_aoe_set_env_map_part_006d96e0);            // 006D96E0
        table[0x0B0 / 4] = reinterpret_cast<void*>(&recovered_client_aoe_set_env_map_006d9590);                 // 006D9590
        built = true;
    }
    return table;
}
}  // namespace
