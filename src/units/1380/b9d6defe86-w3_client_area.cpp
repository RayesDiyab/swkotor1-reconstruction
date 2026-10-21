// lane-w3 unit w3-client-area: CSWCArea construction, its virtual table, lighting / fog / wind / day-night state,
// UnloadArea and the CSWCRoom pieces used by CSWCArea::LoadArea (w3_client_area_load.cpp).
//   00606090 CSWCArea::CSWCArea                       0074EEF0 / 0074EE80 CSWCArea / CGameObject tables
//   00606480 NVThunkAreaGameObject_desctructor       00606470 NVThunkAreaGameObject_AsSWCArea
//   00605260 SetWind  00605160 SetGlobalLightDiffuse  006051F0 SetGlobalLightAmbient  00605870 SetFog
//   00605090 SetFogColor  006058C0 SetAsDay  006059C0 SetAsNight  00606CF0 SetAsDawn  00606C50 SetAsDusk
//   00605380 StopWeather  006053A0 StartWeather  00605B10 UpdateSoundObjectsTimeOfDay
//   006075B0 ClearTilePathNodes  00606DD0 ClearAllMapPins  00609410 UnloadArea
//   00605800 CExoLinkedList<CSWBehaviorAttachSound>::CExoLinkedList
//   00646080 CSWCRoom::CSWCRoom  006460B0 CSWCRoom::~CSWCRoom  00604B60 CSWCRoom vector deleting destructor
//   006460C0 CSWCRoom::SpawnRoom  005C7DC0 CSWRoom::SetRoomEnvAudioProps
//   00640640 CSWCModule::SetTimeOfDayState  0060ED90 CSWCCreature::UpdateAreaId
//   005ED620 CClientExoApp / 005F13B0 CClientExoAppInternal::GetSoundObjectByGameObjectID
#include "c39a01a95c-w3_client_area.h"

using namespace kotor_w3;

// .bss cells written only by CSWCArea code (lane-w3 tools/xref.py: no other instruction references them).
extern "C" {
// [0078E408] owner W4 t1b: kotor_cell_0078e408 (frame/area_update.cpp, .data initial 0.5f) -- integrator binding, W3 definition removed
U recovered_client_area_texture_stats_00832c74[10] = {};              // [00832C74..00832C98] LoadArea statistics
}

namespace {
const char kEmpty_0073d71c[] = "";                                     // .rdata 0073D71C
const char kSunDiffuse_0074ee30[] = "sun gidy_sun_diff color %f %f %f";  // .rdata 0074EE30
const char kSunAmbient_0074ee54[] = "sun gidy_sun_amb color %f %f %f";  // .rdata 0074EE54
const char kFogColor_0073ee40[] = "%f %f %f";                          // .rdata 0073EE40 (CExoString::Format)
constexpr U kOneHundredth_0073dfec = 0x3C23D70Au;                     // .rdata 0073DFEC (0.01f)
constexpr U kZero_0073d700 = 0x00000000u;                             // .rdata 0073D700
constexpr U kOne_0073d764 = 0x3F800000u;                              // .rdata 0073D764
constexpr U kTwo_00741be0 = 0x40000000u;                              // .rdata 00741BE0
constexpr U kThousandth_0073d708 = 0x3A83126Fu;                       // .rdata 0073D708
constexpr U kSixty_00748734 = 0x42700000u;                            // .rdata 00748734
constexpr U kTwo32_0073d770 = 0x4F800000u;                            // .rdata 0073D770
constexpr U kInvalidObject_00746644 = 0x7F000000u;                    // .rdata 00746644
constexpr U8 kAreaObjectType_00746633 = 4;                            // .rdata 00746633
constexpr U8 kSoundObjectType_0074663e = 0x10;                        // .rdata 0074663E
constexpr U8 kGameObjectOk_0074666c = 0;                              // .rdata 0074666C
}  // namespace

// ------------------------------------------------------------------------------------------- virtual tables
// 0074EEF0 (CSWCArea, 9 slots) and 0074EE80 (CGameObject sub-object at +100h, 28 slots); original slot targets are
// cited per entry. Slot bodies absent from runtime1115 and not reconstructed here are D83 traps (w3_traps.cpp).
extern "C" {
extern const void* const recovered_client_area_methods_0074eef0[9];
extern const void* const recovered_client_area_object_methods_0074ee80[28];
const void* const recovered_client_area_methods_0074eef0[9] = {
    reinterpret_cast<const void*>(&recovered_client_area_scalar_destructor_0060a2e0),   // +00 0060A2E0
    reinterpret_cast<const void*>(&gui_load_slot_zero8_00409eb0),                        // +04 00409EB0 return_zero_0x8
    reinterpret_cast<const void*>(&recovered_area_set_fog_0058fce0),                     // +08 0058FCE0 CSWArea::SetFog
    reinterpret_cast<const void*>(&recovered_area_set_is_night_0058fd10),                // +0C 0058FD10 CSWArea::SetIsNight
    reinterpret_cast<const void*>(&recovered_client_area_set_wind_00605260),             // +10 00605260
    reinterpret_cast<const void*>(&recovered_client_area_set_global_light_diffuse_00605160),   // +14 00605160
    reinterpret_cast<const void*>(&recovered_client_area_set_global_light_ambient_006051f0),   // +18 006051F0
    reinterpret_cast<const void*>(&recovered_client_area_set_weather_00604db0),          // +1C 00604DB0
    reinterpret_cast<const void*>(&recovered_client_area_set_fog_00605870),              // +20 00605870
};
const void* const recovered_client_area_object_methods_0074ee80[28] = {
    reinterpret_cast<const void*>(&recovered_client_area_object_destructor_thunk_00606480),   // +00 00606480
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),                     // +04 0060E760 void_return (RET 4)
    reinterpret_cast<const void*>(&slot_void_return_8_005b5e90_a2),                     // +08 005B5E90 void_return (RET 8)
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +0C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +10
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +14
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +18
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +1C
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +20
    reinterpret_cast<const void*>(&recovered_client_area_object_as_area_00606470),      // +24 00606470
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +28
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +2C
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +30
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +34
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +38
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +3C
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +40
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +44
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +48
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +4C
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +50
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +54
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +58
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +5C
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +60
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +64
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +68
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),                       // +6C
};
// 00751A34 CSWCRoom table (one slot).
extern const void* const recovered_client_room_methods_00751a34[1];
const void* const recovered_client_room_methods_00751a34[1] = {
    reinterpret_cast<const void*>(&recovered_client_room_vector_destructor_00604b60),   // +00 00604B60
};
}

extern "C" {
// 00606480 NVThunkAreaGameObject_desctructor: ECX -= 100h; jmp ~CSWCArea (scalar deleting, RET 4).
void* __fastcall recovered_client_area_object_destructor_thunk_00606480(void* object, void*, U flags) noexcept(false) {
    return recovered_client_area_scalar_destructor_0060a2e0(static_cast<unsigned char*>(object) - 0x100, nullptr, flags);
}
// 00606470 NVThunkAreaGameObject_AsSWCArea: EAX = ECX - 100h.
void* __fastcall recovered_client_area_object_as_area_00606470(void* object, void*) noexcept {
    return static_cast<unsigned char*>(object) - 0x100;               // 00606470 lea eax,[ecx-100h]
}

// 00605800 CExoLinkedList<CSWBehaviorAttachSound>::CExoLinkedList: [this] = new CExoLinkedListInternal (0Ch).
void* __fastcall recovered_client_attach_sound_list_construct_00605800(void* self, void*) noexcept(false) {
    void* const memory = live_operator_new(0xC);                      // 00605817..0060581B
    void* internal = nullptr;                                         // 0060584E..00605854
    if (memory != nullptr)                                            // 00605827..00605831
        internal = recovered_server_create::construct_list_internal_005e9aa0(memory);   // 00605833..00605835
    st<void*>(self, 0, internal);                                     // 0060583A / 00605854
    return self;                                                      // 0060583C
}

// 00606090 CSWCArea::CSWCArea (284h bytes). ECX this, RET, EAX this.
void* __fastcall recovered_client_area_construct_00606090(void* self, void*) noexcept(false) {
    recovered_bridge_area_base_construct_0058f6b0(self, nullptr);     // 006060B1 CSWArea::CSWArea
    kotor_binding_required::game_object_004D7D60(at(self, 0x100), nullptr, kAreaObjectType_00746633,
                                                 kInvalidObject_00746644);   // 006060B6..006060D1 CGameObject(4, 7F000000h)
    st<const void*>(self, 0, recovered_client_area_methods_0074eef0);          // 006060E1
    st<const void*>(self, 0x100, recovered_client_area_object_methods_0074ee80);   // 006060E7
    kotor_fastcall_00405ed0(at(self, 0x10C), nullptr);                // 006060D6..006060ED CResRef()
    st<U>(self, 0x120, 0); st<U>(self, 0x124, 0); st<U>(self, 0x11C, 0);   // 006060F2..006060FE
    for (unsigned o = 0x128; o <= 0x16C; o += 4) st<U>(self, o, 0);   // 00606104..0060616A
    st<U>(self, 0x170, 0); st<U>(self, 0x174, 0); st<U>(self, 0x178, 0);   // 0060617D..00606189
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(at(self, 0x17C)), nullptr);   // 00606170..0060618F
    st<U>(self, 0x1C8, 0); st<U>(self, 0x1CC, 0); st<U>(self, 0x1C4, 0);   // 00606194..006061A0
    st<U>(self, 0x1D4, 0); st<U>(self, 0x1D8, 0); st<U>(self, 0x1D0, 0);   // 006061A6..006061B2
    recovered_client_attach_sound_list_construct_00605800(at(self, 0x204), nullptr);   // 006061B8..006061C3
    st<U>(self, 0x210, 0); st<U>(self, 0x214, 0); st<U>(self, 0x20C, 0);   // 006061C8..006061D4
    st<U>(self, 0x21C, 0); st<U>(self, 0x220, 0); st<U>(self, 0x218, 0);   // 006061DA..006061E6
    st<U>(self, 0x228, 0); st<U>(self, 0x22C, 0); st<U>(self, 0x224, 0);   // 006061EC..006061F8
    st<U>(self, 0x23C, 0); st<U>(self, 0x240, 0); st<U>(self, 0x238, 0);   // 006061FE..0060620A
    st<U>(self, 0x248, 0); st<U>(self, 0x24C, 0); st<U>(self, 0x244, 0);   // 00606210..0060621C
    recovered_client_attach_sound_list_construct_00605800(at(self, 0x280), nullptr);   // 00606222..0060622D
    c2da_assign_text_005e5140(at(self, 0x17C), nullptr, kEmpty_0073d71c);   // 00606232..0060623E
    st<U>(self, 0x184, 0); st<U>(self, 0x188, 0);                     // 00606245..0060624B
    for (unsigned o = 0x18C; o < 0x18C + 6 * 4; o += 4) st<U>(self, o, 0);   // 00606251..0060625C rep stosd
    recovered_bridge_area_initialize_static_objects_0058f800(self, nullptr);   // 0060625E..00606260
    // 00606265..00606360: colour/flag fields (the interleaved stack-vector zeroing is dead).
    for (unsigned o = 0x128; o <= 0x178; o += 4) st<U>(self, o, 0);
    st<U>(self, 0x1A4, 0);                                            // 0060630C
    st<U>(self, 0x1B4, 0); st<U>(self, 0x1B8, 0);                     // 00606336..0060633C
    st<U>(self, 0xA4, 0);                                             // 00606342
    st<U>(self, 0x1BC, 0);                                            // 0060635A
    st<U>(self, 0x1C0, 1);                                            // 00606360
    void* const memory = live_operator_new(0x10C);                    // 00606307..0060636A
    void* ambient = nullptr;                                          // 00606389
    if (memory != nullptr)                                            // 00606376..0060637D
        ambient = recovered_client_ambient_sound_construct_0068df40(memory, nullptr, self);   // 0060637F..00606382
    st<void*>(self, 0x1DC, ambient);                                  // 0060638B
    st<U>(self, 0x1E0, 0); st<U>(self, 0x1E4, 0); st<U>(self, 0x1E8, 0); st<U>(self, 0x1EC, 0);   // 00606391..006063A3
    st<U8>(self, 0x1F0, 0); st<U8>(self, 0x1F1, 0);                   // 006063A9..006063AF
    st<U>(self, 0x1F4, 0); st<U>(self, 0x1F8, 0);                     // 006063B5..006063BB
    U time_low = 0, time_high = 0;                                    // {E-0x28} / {E-0x24}
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &time_low, &time_high);   // 006063C1..006063E0
    st<U8>(self, 0x254, 0xFF); st<U8>(self, 0x255, 0xFF); st<U8>(self, 0x256, 0xFF);   // 006063ED..006063FB
    st<U>(self, 0x1FC, time_low);                                     // 00606401
    st<U>(self, 0x208, 0); st<U>(self, 0x230, 0); st<U>(self, 0x234, 0); st<U>(self, 0x250, 0);   // 0060640B..0060641D
    st<U>(self, 0x258, 0); st<U>(self, 0x25C, 0); st<U>(self, 0x260, 0); st<U>(self, 0x264, 0);   // 00606423..00606435
    st<U8>(self, 0x278, 0);                                           // 0060643B
    st<U>(self, 0x27C, 0);                                            // 00606441
    st<U>(self, 0x200, time_high);                                    // 00606448
    st<U8>(self, 0x279, 1);                                           // 0060644E
    return self;                                                      // 00606455
}

// 00605380 CSWCArea::StopWeather(BYTE) RET 4 / 006053A0 CSWCArea::StartWeather(BYTE unused) RET 4.
void __fastcall recovered_client_area_stop_weather_00605380(void* self, void*, U value) noexcept {
    st<U8>(self, 0x1F1, static_cast<U8>(value));                      // 00605380..00605384
    st<U>(self, 0x1EC, 0x1770);                                       // 0060538A
}
void __fastcall recovered_client_area_start_weather_006053a0(void* self, void*, U /*unused*/) noexcept {
    st<U8>(self, 0x1F1, 0);                                           // 006053A0
    st<U>(self, 0x1EC, 0x1770);                                       // 006053A7
}

// 00605260 CSWCArea::SetWind(BYTE strength) RET 4 -> EAX (0 for an unknown strength).
U __fastcall recovered_client_area_set_wind_00605260(void* self, void*, U strength) noexcept(false) {
    Vector3 direction = {1.0f, 1.0f, 0.0f};                          // 00605281..00605291 {E-0x18}
    float variation_1c = 0.0f;                                        // 00605271 {E-0x1c}
    float variation_20 = 0.0f;                                        // 00605279 {E-0x20}
    Vector3 normal;                                                   // {E-0xc}
    const Vector3* n = vector_normalized_004ab180(&normal, &direction);   // 00605265..00605299
    direction = *n;                                                   // 0060529E..006052BB
    const U8 bl = static_cast<U8>(strength);                          // 006052A0
    float scale;
    switch (bl) {                                                     // 006052B2..006052C5
    case 0: scale = fbits(kZero_0073d700); break;                     // 00605301
    case 1: scale = fbits(kOne_0073d764); variation_1c = fbits(0x42C80000u); variation_20 = fbits(0x40400000u); break;   // 006052E9..006052FF (100, 3)
    case 2: scale = fbits(kTwo_00741be0); variation_1c = fbits(0x43160000u); variation_20 = fbits(0x40A00000u); break;   // 006052D1..006052E7 (150, 5)
    default: return 0;                                                // 006052C7..006052CE
    }
    // 00605307..0060533C: AurSetGlobalWind(direction * scale); each float product is rounded once on its fstp.
    recovered_aur_set_global_wind_004530d0(direction.x * scale, direction.y * scale, scale * direction.z);
    recovered_aur_set_global_wind_variation_00453140(variation_1c, variation_20);   // 00605341..0060534B
    st<U8>(self, 0xAB, bl);                                           // 00605353
    return 1;                                                         // 0060535A
}

// 00605090 CSWCArea::SetFogColor(Vector const*) RET 4: scene SetFogColor, clear colour "%f %f %f", [this+54h].
U __fastcall recovered_client_area_set_fog_color_00605090(void* self, void*, const Vector3* colour) noexcept(false) {
    void* const scene = rp(self, 0x184);                              // 006050AB
    if (scene == nullptr) return 0;                                   // 006050B1..006050B5
    vslot<void (__fastcall*)(void*, void*, const Vector3*)>(scene, 0x08)(scene, nullptr, colour);   // 006050C9..006050D1
    ExoString text;                                                   // [esp+8h]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&text), nullptr);   // 006050D4..006050D8
    kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(&text),
        kFogColor_0073ee40, static_cast<double>(colour->x), static_cast<double>(colour->y),
        static_cast<double>(colour->z));                              // 006050DD..00605105
    recovered_set_clear_color_00401690(async_string_cstr_005e5670(&text, nullptr));   // 0060510D..00605117
    st<float>(self, 0x54, colour->x); st<float>(self, 0x58, colour->y); st<float>(self, 0x5C, colour->z);   // 0060511C..0060512C
    frame_LogStringDtor_005e5c20(&text, nullptr);                     // 00605132..0060513E
    return 1;                                                         // 00605148
}

// 00605870 CSWCArea::SetFog(float near, float far, Vector const* colour) RET 0Ch.
U __fastcall recovered_client_area_set_fog_00605870(void* self, void*, float fog_near, float fog_far,
                                                    const Vector3* colour) noexcept(false) {
    void* const scene = rp(self, 0x184);                              // 00605873
    if (scene == nullptr) return 0;                                   // 00605879..0060587D
    vslot<void (__fastcall*)(void*, void*, float, float)>(scene, 0x44)(scene, nullptr, fog_near, fog_far);   // 00605883..0060588F SetFogRange
    st<float>(self, 0x64, fog_far);                                   // 00605892..0060589E
    st<float>(self, 0x60, fog_near);                                  // 0060589A..006058A4
    recovered_client_area_set_fog_color_00605090(self, nullptr, colour);   // 006058A1..006058A7
    return 1;                                                         // 006058AC
}

// 00605160 CSWCArea::SetGlobalLightDiffuse(BOOL on, Vector colour) RET 10h.
U __fastcall recovered_client_area_set_global_light_diffuse_00605160(void* self, void*, U on, float r, float g,
                                                                    float b) noexcept(false) {
    void* const sun = rp(self, 0x188);                                // 00605160
    if (sun == nullptr) return 0;                                     // 0060516C..00605170
    if (on == 0) {                                                    // 0060517B..00605184
        vslot<void (__fastcall*)(void*, void*, void*)>(sun, 0x4C)(sun, nullptr, nullptr);   // 00605186..0060518A AttachToScene(0)
        return 1;
    }
    char command[0x80];                                               // [esp+0h] (sub esp,80h)
    format_006fadb0(command, kSunDiffuse_0074ee30, static_cast<double>(r), static_cast<double>(g),
                    static_cast<double>(b));                          // 0060519B..006051C8
    recovered_aur_control_part_0044b900(command);                     // 006051CD..006051D2
    return 1;                                                         // 006051DA
}

// 006051F0 CSWCArea::SetGlobalLightAmbient(BOOL unused, Vector colour) RET 10h.
U __fastcall recovered_client_area_set_global_light_ambient_006051f0(void* self, void*, U /*on*/, float r, float g,
                                                                    float b) noexcept(false) {
    if (rp(self, 0x184) == nullptr) return 0;                         // 006051F0..00605200 (scene)
    char command[0x80];
    format_006fadb0(command, kSunAmbient_0074ee54, static_cast<double>(r), static_cast<double>(g),
                    static_cast<double>(b));                          // 0060520B..00605238
    recovered_aur_control_part_0044b900(command);                     // 0060523D..00605242
    return 1;                                                         // 0060524A
}
}  // extern "C"

namespace {
// Shared body of SetAsDay / SetAsNight (006058E3.. / 006059E3..).
void apply_day_night(void* self, U beam_bits, unsigned fog_offset, unsigned fog_colour_offset,
                     unsigned ambient_offset, unsigned diffuse_offset, U night) noexcept(false) {
    beam_alpha_0078e6a0 = fbits(beam_bits);                            // 006058E3 / 006059E3
    const I32 percent = ld<U8>(self, 0xB4);                           // 006058ED..006058F4
    kotor_cell_0078e408 = static_cast<float>(percent) * fbits(kOneHundredth_0073dfec);   // 006058FF..00605909
    const float fog_far = ld<float>(self, fog_offset + 4);            // 0060590F
    const float fog_near = ld<float>(self, fog_offset);               // 00605915
    st<U>(self, 0x1C0, 1);                                            // 00605921
    st<U>(self, 0x1B4, 0); st<U>(self, 0x1B8, 0);                     // 0060592B..00605935
    st<U>(self, 0xA4, night);                                         // 0060593F
    // 00605949 [vtbl+20h] SetFog(near, far, &colour)
    vslot<U (__fastcall*)(void*, void*, float, float, const void*)>(self, 0x20)(self, nullptr, fog_near, fog_far,
                                                                              at(self, fog_colour_offset));
    // 0060594C..0060596D [vtbl+18h] SetGlobalLightAmbient(1, colour)
    vslot<U (__fastcall*)(void*, void*, U, float, float, float)>(self, 0x18)(self, nullptr, 1,
        ld<float>(self, ambient_offset), ld<float>(self, ambient_offset + 4), ld<float>(self, ambient_offset + 8));
    // 00605970..00605991 [vtbl+14h] SetGlobalLightDiffuse(1, colour)
    vslot<U (__fastcall*)(void*, void*, U, float, float, float)>(self, 0x14)(self, nullptr, 1,
        ld<float>(self, diffuse_offset), ld<float>(self, diffuse_offset + 4), ld<float>(self, diffuse_offset + 8));
    if (rp(self, 0x188) != nullptr) {                                 // 00605994..0060599C
        void* const player = client_get_player_creature_005ed540(client_app(), nullptr);   // 0060599E..006059A7
        if (player != nullptr) recovered_client_creature_get_creature_light_00614aa0(player, nullptr, 1);   // 006059AC..006059B4
    }
}
// Shared body of SetAsDawn / SetAsDusk (00606D15.. / 00606C75..).
void apply_dawn_dusk(void* self, U beam_bits, U dawn, U dusk, U night) noexcept(false) {
    beam_alpha_0078e6a0 = fbits(beam_bits);
    const I32 percent = ld<U8>(self, 0xB4);
    kotor_cell_0078e408 = static_cast<float>(percent) * fbits(kOneHundredth_0073dfec);
    const bool has_sun = rp(self, 0x188) != nullptr;
    st<U>(self, 0x1C0, 1);
    st<U>(self, 0x1B4, dawn); st<U>(self, 0x1B8, dusk);
    st<U>(self, 0xA4, night);
    if (has_sun) {
        void* const player = client_get_player_creature_005ed540(client_app(), nullptr);
        if (player != nullptr) recovered_client_creature_get_creature_light_00614aa0(player, nullptr, 1);
    }
}
}  // namespace

extern "C" {
// 006058C0 CSWCArea::SetAsDay. ECX this, RET. Areas that are always night (+A0h == 0 && +A4h != 0) become night.
void __fastcall recovered_client_area_set_as_day_006058c0(void* self, void*) noexcept(false) {
    if (rd32(self, 0xA0) == 0 && rd32(self, 0xA4) != 0) {             // 006058C5..006058D7
        recovered_client_area_set_as_night_006059c0(self, nullptr);   // 006058DE (tail jump)
        return;
    }
    apply_day_night(self, 0x3D4CCCCDu, 0x90, 0x164, 0x14C, 0x158, 0);  // 006058E3..006059B9
}
// 006059C0 CSWCArea::SetAsNight. ECX this, RET. Areas with +A0h == 0 && +A4h == 0 become day.
void __fastcall recovered_client_area_set_as_night_006059c0(void* self, void*) noexcept(false) {
    if (rd32(self, 0xA0) == 0 && rd32(self, 0xA4) == 0) {             // 006059C5..006059D7
        recovered_client_area_set_as_day_006058c0(self, nullptr);     // 006059DE (tail jump)
        return;
    }
    apply_day_night(self, 0x00000000u, 0x74, 0x140, 0x128, 0x134, 1);  // 006059E3..00605AB3
}
// 00606CF0 CSWCArea::SetAsDawn. ECX this, RET.
void __fastcall recovered_client_area_set_as_dawn_00606cf0(void* self, void*) noexcept(false) {
    if (rd32(self, 0xA0) == 0) {                                      // 00606CF1..00606CF9
        if (rd32(self, 0xA4) != 0) recovered_client_area_set_as_night_006059c0(self, nullptr);   // 00606CFB..00606D08
        else recovered_client_area_set_as_day_006058c0(self, nullptr);                           // 00606D10
        return;
    }
    apply_dawn_dusk(self, 0x00000000u, 1, 0, 1);                      // 00606D15..00606D87
}
// 00606C50 CSWCArea::SetAsDusk. ECX this, RET.
void __fastcall recovered_client_area_set_as_dusk_00606c50(void* self, void*) noexcept(false) {
    if (rd32(self, 0xA0) == 0) {                                      // 00606C51..00606C59
        if (rd32(self, 0xA4) != 0) recovered_client_area_set_as_night_006059c0(self, nullptr);   // 00606C5B..00606C68
        else recovered_client_area_set_as_day_006058c0(self, nullptr);                           // 00606C70
        return;
    }
    apply_dawn_dusk(self, 0x3D4CCCCDu, 0, 1, 0);                      // 00606C75..00606CE7
}

// 005F13B0 CClientExoAppInternal::GetSoundObjectByGameObjectID(id) RET 4; 005ED620 forwards on [app+4].
void* __fastcall recovered_client_internal_get_sound_object_005f13b0(void* internal, void*, U id) noexcept(false) {
    void* object = nullptr;                                           // 005F13BE [esp] = 0
    if (kotor_fastcall_004d8230(rp(internal, 0x14), nullptr, id, &object) != kGameObjectOk_0074666c) return nullptr;   // 005F13B1..005F13D1
    if (object == nullptr) return nullptr;                            // 005F13D7..005F13D9
    if (ld<U8>(object, 8) != kSoundObjectType_0074663e) return nullptr;   // 005F13DB..005F13E4
    return vslot<void* (__fastcall*)(void*, void*)>(object, 0x6C)(object, nullptr);   // 005F13E6..005F13E8 AsSWCSoundObject
}
void* __fastcall recovered_client_get_sound_object_005ed620(void* app, void*, U id) noexcept(false) {
    return recovered_client_internal_get_sound_object_005f13b0(rp(app, 4), nullptr, id);   // 005ED620..005ED623
}

// 00605B10 CSWCArea::UpdateSoundObjectsTimeOfDay. ECX this, RET.
void __fastcall recovered_client_area_update_sound_objects_time_of_day_00605b10(void* self, void*) noexcept(false) {
    const I32 count = ld<I32>(self, 0x1D4);                           // 00605B15
    for (I32 i = 0; i < count; ++i) {                                 // 00605B1D..00605B46
        void* const sound = recovered_client_get_sound_object_005ed620(client_app(), nullptr,
            rd32(rp(self, 0x1D0), static_cast<U>(i) * 4));           // 00605B21..00605B33
        if (sound != nullptr) recovered_client_sound_object_update_time_of_day_0066ae90(sound, nullptr);   // 00605B38..00605B3E
    }
}

// 00640640 CSWCModule::SetTimeOfDayState(BYTE state, DWORD world_milliseconds) RET 8.
void __fastcall recovered_client_module_set_time_of_day_state_00640640(void* self, void*, U state_word,
                                                                      U milliseconds) noexcept(false) {
    const U8 state = static_cast<U8>(state_word);                     // 00640641
    void* const area = rp(self, 0x48);                                // 00640648
    if (area != nullptr) {                                            // 0064064B..0064064D
        switch (state) {                                              // 0064064F..00640658 (table 006406F0)
        case 1: recovered_client_area_set_as_day_006058c0(area, nullptr); break;                 // 0064065F
        case 2: recovered_client_area_set_as_night_006059c0(area, nullptr); break;               // 00640666
        case 3:                                                       // 0064066D
            recovered_client_area_set_as_night_006059c0(area, nullptr);
            recovered_client_area_set_as_dawn_00606cf0(rp(self, 0x48), nullptr);                // 00640672..00640675
            break;
        case 4:                                                       // 0064067C
            recovered_client_area_set_as_day_006058c0(area, nullptr);
            recovered_client_area_set_as_dusk_00606c50(rp(self, 0x48), nullptr);                // 00640681..00640684
            break;
        default: break;
        }
    }
    if (ld<U8>(self, 0xB4) != state) {                                // 00640689..0064068F
        void* const current = rp(self, 0x48);                         // 00640691
        st<U8>(self, 0xB4, state);                                    // 00640696
        if (current != nullptr) {                                     // 00640694..0064069C
            recovered_client_area_update_sound_objects_time_of_day_00605b10(current, nullptr);   // 0064069E
            recovered_client_ambient_sound_update_time_of_day_0068d690(rp(rp(self, 0x48), 0x1DC), nullptr);   // 006406A3..006406AC
        }
    }
    // 006406B1..006406D4: [+B8h] = (float)(unsigned)milliseconds * 0.001 (x87: unsigned via +2^32 when negative)
    double ms = static_cast<double>(static_cast<I32>(milliseconds));
    if (static_cast<I32>(milliseconds) < 0) ms += static_cast<double>(fbits(kTwo32_0073d770));
    st<float>(self, 0xB8, static_cast<float>(ms * static_cast<double>(fbits(kThousandth_0073d708))));
    // 006406C9..006406E4: [+BCh] = minutes-per-hour byte * 60.0f
    st<float>(self, 0xBC, static_cast<float>(static_cast<double>(ld<U8>(self, 0xA0)) *
                                             static_cast<double>(fbits(kSixty_00748734))));
}

// 0060ED90 CSWCCreature::UpdateAreaId: [this+39Ch] = area ? area->id ([area+104h]) : invalid id.
void __fastcall recovered_client_creature_update_area_id_0060ed90(void* self, void*) noexcept {
    void* const area = rp(self, 0x20);                                // 0060ED90
    st<U>(self, 0x39C, area != nullptr ? rd32(area, 0x104) : kInvalidObject_00746644);   // 0060ED93..0060EDAA
}

// 006075B0 CSWCArea::ClearTilePathNodes. ECX this, RET.
void __fastcall recovered_client_area_clear_tile_path_nodes_006075b0(void* self, void*) noexcept {
    if (rp(self, 0x244) != nullptr) { live_free(rp(self, 0x244)); st<U>(self, 0x244, 0); }   // 006075B3..006075C9
    st<U>(self, 0x24C, 0); st<U>(self, 0x248, 0);                     // 006075CF..006075D5
    if (rp(self, 0x238) != nullptr) { live_free(rp(self, 0x238)); st<U>(self, 0x238, 0); }   // 006075DB..006075EE
    st<U>(self, 0x240, 0); st<U>(self, 0x23C, 0);                     // 006075F4..006075FA
}

// 00606DD0 CSWCArea::ClearAllMapPins. ECX this, RET.
void __fastcall recovered_client_area_clear_all_map_pins_00606dd0(void* self, void*) noexcept(false) {
    const I32 count = ld<I32>(self, 0x1C8);                           // 00606DD5
    for (I32 i = 0; i < count; ++i) {                                 // 00606DE0..00606DFA
        void* const pin = rp(rp(self, 0x1C4), static_cast<U>(i) * 4); // 00606DE4..00606DEA
        if (pin != nullptr) delete_object(pin);                       // 00606DED..00606DF5
    }
    if (rp(self, 0x1C4) != nullptr) {                                 // 00606DFC..00606E04
        live_free(rp(self, 0x1C4));                                   // 00606E06..00606E07
        st<U>(self, 0x1C4, 0); st<U>(self, 0x1CC, 0);                 // 00606E0F..00606E15
    }
    st<U>(self, 0x1C8, 0);                                            // 00606E1C
}

// 00609410 CSWCArea::UnloadArea. ECX this, RET.
void __fastcall recovered_client_area_unload_area_00609410(void* self, void*) noexcept(false) {
    if (frame_R4_005ed690(client_app(), nullptr) != nullptr)          // 00609410..00609428 GetInGameGui
        kotor_fastcall_0062fb90(frame_R4_005ed690(client_app(), nullptr), nullptr);   // 0060942A..0060943A FadeAndStopDialogAmbientTrack
    recovered_aur_grass_clear_info_004a67a0();                        // 0060943F
    recovered_client_area_clear_tile_path_nodes_006075b0(self, nullptr);   // 00609444..00609446
    if (rp(self, 0x11C) != nullptr) {                                 // 0060944B..00609455
        live_free(rp(self, 0x11C));                                   // 00609457..00609458
        st<U>(self, 0x11C, 0); st<U>(self, 0x124, 0);                 // 00609460..00609466
    }
    st<U>(self, 0x120, 0);                                            // 0060946C
    for (unsigned o = 0x18C; o < 0x18C + 6 * 4; o += 4) {             // 00609472..00609492 weather objects
        void* const object = rp(self, o);
        if (object != nullptr) { delete_object(object); st<U>(self, o, 0); }
    }
    void* const objects = kotor_fastcall_005ed570(client_app(), nullptr);   // 00609494..006094AA
    void* const ai_master = kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr);   // 006094A1..006094B1
    const I32 sound_count = ld<I32>(self, 0x1D4);                     // 006094B5..006094BF
    for (I32 i = 0; i < sound_count; ++i) {                           // 006094C3..0060951F
        void* const sound = recovered_client_get_sound_object_005ed620(client_app(), nullptr,
            rd32(rp(self, 0x1D0), static_cast<U>(i) * 4));           // 006094D0..006094E3
        if (sound == nullptr) continue;                               // 006094EA..006094EC
        recovered_client_sound_object_stop_0066b130(sound, nullptr);  // 006094EE..006094F0
        kotor_m15e_client_ai_master_remove_object_00604a30(ai_master, nullptr, sound);   // 006094F5..006094FA
        recovered_object_array_erase_004d8290(objects, nullptr, rd32(rp(self, 0x1D0), static_cast<U>(i) * 4));   // 006094FF..0060950B
        delete_object(sound);                                         // 00609510..00609516
    }
    if (rp(self, 0x1D0) != nullptr) {                                 // 00609523..0060952B
        live_free(rp(self, 0x1D0));                                   // 0060952D..0060952E
        st<U>(self, 0x1D0, 0); st<U>(self, 0x1D8, 0);                 // 00609536..0060953C
    }
    st<U>(self, 0x1D4, 0);                                            // 00609542
    if (rp(self, 0x1DC) != nullptr)                                   // 00609548..00609550
        recovered_client_ambient_sound_stop_sounds_0068e1b0(rp(self, 0x1DC), nullptr, 0);   // 00609552..00609553
    if (rp(self, 0x1A4) != nullptr) { delete_object(rp(self, 0x1A4)); st<U>(self, 0x1A4, 0); }   // 00609558..00609568
    st<U>(self, 0x1E0, 0); st<U>(self, 0x1E4, 0); st<U>(self, 0x1E8, 0); st<U>(self, 0x1EC, 0);   // 00609570..00609582
    st<U8>(self, 0x1F0, 0); st<U8>(self, 0x1F1, 0);                   // 00609588..0060958F
    st<U>(self, 0x1F4, 0); st<U>(self, 0x1F8, 0);                     // 00609596..0060959C
    recovered_client_area_clear_all_map_pins_00606dd0(self, nullptr); // 006095A2
    if (rp(self, 0x188) != nullptr) { delete_object(rp(self, 0x188)); st<U>(self, 0x188, 0); }   // 006095A7..006095B7 sun
    if (rp(self, 0x184) != nullptr) { delete_object(rp(self, 0x184)); st<U>(self, 0x184, 0); }   // 006095BD..006095CD scene
    while (rp(rp(self, 0x280), 0) != nullptr) {                       // 006095D3..006095FF attached room sounds
        void* const item = base_ini_reconstruction::remove_head_005e9c80(
            static_cast<kotor_list::Internal*>(rp(self, 0x280)), nullptr);   // 006095E0..006095E6
        if (item != nullptr) delete_object(item);                     // 006095EB..006095F5
    }
    while (rp(rp(self, 0x204), 0) != nullptr)                         // 00609601..00609623
        (void)base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(rp(self, 0x204)), nullptr);
    for (I32 i = ld<I32>(self, 0x21C) - 1; i >= 0; --i) {             // 00609625..0060968D
        void* const entry = rp(rp(self, 0x218), static_cast<U>(i) * 4);   // 00609630..00609636
        if (entry == nullptr) continue;                               // 00609639..0060963B
        if (rp(entry, 4) != nullptr) delete_object(rp(entry, 4));     // 0060963D..0060964A
        live_free(rp(rp(self, 0x218), static_cast<U>(i) * 4));        // 0060964C..00609656
        const I32 count = ld<I32>(self, 0x21C) - 1;                   // 0060965B..00609664
        st<I32>(self, 0x21C, count);                                  // 0060966B
        for (I32 k = i; k < count; ++k)                               // 00609667..0060968A (remove at i)
            st<U>(rp(self, 0x218), static_cast<U>(k) * 4, rd32(rp(self, 0x218), static_cast<U>(k + 1) * 4));
    }
    for (I32 i = ld<I32>(self, 0x228) - 1; i >= 0; --i) {             // 0060968F..0060971B
        void* found = nullptr;                                        // {E-8} out-parameter of CGameObjectArray::Delete
        if (recovered_object_array_remove_004d81a0(kotor_fastcall_005ed570(client_app(), nullptr),
                nullptr, rd32(rp(self, 0x224), static_cast<U>(i) * 4), &found) == kGameObjectOk_0074666c &&   // 006096A0..006096C9
            found != nullptr &&                                       // 006096CB..006096D1
            vslot<U (__fastcall*)(void*, void*)>(found, 0x4C)(found, nullptr) != 0 &&   // 006096D3..006096DA
            found != nullptr)                                         // 006096DC..006096E2
            delete_object(found);                                     // 006096E4..006096E8
        const I32 count = ld<I32>(self, 0x228) - 1;                   // 006096EA..006096F0
        st<I32>(self, 0x228, count);                                  // 006096F7
        for (I32 k = i; k < count; ++k)                               // 006096F3..00609718
            st<U>(rp(self, 0x224), static_cast<U>(k) * 4, rd32(rp(self, 0x224), static_cast<U>(k + 1) * 4));
    }
    if (rp(self, 0x250) != nullptr) {                                 // 0060971D..00609727
        live_free(rp(self, 0x250));                                   // 00609729..0060972A
        st<U>(self, 0x250, 0);                                        // 00609734
        st<U8>(self, 0x254, 0xFF); st<U8>(self, 0x255, 0xFF); st<U8>(self, 0x256, 0xFF);   // 0060973A..00609746
        st<U>(self, 0x258, 0);                                        // 0060974C
    }
    for (unsigned o = 0x268; o < 0x268 + 4 * 4; o += 4) {             // 00609752..006097D7
        void* const creature = frame_R4_005ed5b0(client_app(), nullptr, rd32(self, o));   // 00609760..0060976C
        if (creature != nullptr) {                                    // 00609771..00609775
            kotor_m15e_client_ai_list_remove_object_00604810(
                at(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr), 4), nullptr, creature);   // 00609777..00609788
            void* found = nullptr;
            if (recovered_object_array_remove_004d81a0(kotor_fastcall_005ed570(client_app(), nullptr),
                    nullptr, rd32(self, o), &found) == kGameObjectOk_0074666c &&   // 0060978D..006097B1
                found != nullptr &&                                   // 006097B3..006097B9
                vslot<void* (__fastcall*)(void*, void*)>(found, 0x2C)(found, nullptr) == creature)   // 006097BB..006097C2 AsSWCCreature
                delete_object(creature);                              // 006097C4..006097CA
        }
        st<U>(self, o, kInvalidObject_00746644);                      // 006097CC
    }
    st<U8>(self, 0x278, 0);                                           // 006097DA
    st<U8>(self, 0x279, 1);                                           // 006097E1
}

// 00646080 CSWCRoom::CSWCRoom (48h bytes). ECX this, RET, EAX this.
void* __fastcall recovered_client_room_construct_00646080(void* self, void*) noexcept(false) {
    recovered_room_base_construct_005c7d10(self, nullptr);            // 00646083 CSWRoom::CSWRoom
    st<U>(self, 0x3C, 0); st<U>(self, 0x40, 0);                       // 0064608A..0064608D
    st<const void*>(self, 0, recovered_client_room_methods_00751a34); // 00646090
    st<U>(self, 0x44, 0xFFFFFFFFu);                                   // 00646096
    return self;                                                      // 0064609D
}
// 006460B0 CSWCRoom::~CSWCRoom: table store then CSWRoom::~CSWRoom (tail).
void __fastcall recovered_client_room_destruct_006460b0(void* self, void*) noexcept {
    st<const void*>(self, 0, recovered_client_room_methods_00751a34); // 006460B0
    recovered_room_base_reset_005c7d00(self, nullptr);                // 006460B6
}
// 00604B60 CSWCRoom vector/scalar deleting destructor (flags: 2 = array with count prefix, 1 = free). RET 4.
void* __fastcall recovered_client_room_vector_destructor_00604b60(void* self, void*, U flags) noexcept(false) {
    if ((flags & 2) != 0) {                                           // 00604B61..00604B6B
        unsigned char* const block = static_cast<unsigned char*>(self) - 4;   // 00604B76
        const I32 count = ld<I32>(block, 0);                          // 00604B6D
        for (I32 i = count - 1; i >= 0; --i)                          // 00604B7D eh_vector_destructor_iterator (reverse)
            recovered_client_room_destruct_006460b0(static_cast<unsigned char*>(self) + static_cast<U>(i) * 0x48, nullptr);
        if ((flags & 1) != 0) live_free(block);                       // 00604B82..00604B88
        return block;                                                 // 00604B90
    }
    recovered_client_room_destruct_006460b0(self, nullptr);           // 00604B98..00604B9A
    if ((flags & 1) != 0) live_free(self);                            // 00604B9F..00604BA5
    return self;                                                      // 00604BAD
}

// 006460C0 CSWCRoom::SpawnRoom(CAurScene*) RET 4 -> EAX = room Gob (0 when the room was already spawned).
// Scene vtable +70h SpawnRoom(name, &position(+4h), &orientation(+10h)); +74h FindRoom(name) -> [+3Ch].
void* __fastcall recovered_client_room_spawn_room_006460c0(void* self, void*, void* scene) noexcept(false) {
    if (rp(self, 0x38) != nullptr) return nullptr;                    // 006460DB..006460F3
    ExoString name;                                                   // [esp+0Ch]
    frame_LogStringCtor_005e5a90(&name, nullptr, c2da_ref_00405fe0(at(self, 0x20), nullptr));   // 006460F8..00646105
    void* const gob = vslot<void* (__fastcall*)(void*, void*, const char*, const void*, const void*)>(scene, 0x70)(
        scene, nullptr, async_string_cstr_005e5670(&name, nullptr), at(self, 4), at(self, 0x10));   // 0064610A..0064612C
    st<void*>(self, 0x40, gob);                                       // 00646131
    if (gob != nullptr)                                               // 0064612F..00646134
        st<void*>(self, 0x3C, vslot<void* (__fastcall*)(void*, void*, const char*)>(scene, 0x74)(
            scene, nullptr, async_string_cstr_005e5670(&name, nullptr)));   // 00646136..00646147
    void* const result = rp(self, 0x40);                              // 0064614A
    frame_LogStringDtor_005e5c20(&name, nullptr);                     // 0064614D..00646159
    return result;                                                    // 00646164
}

// 005C7DC0 CSWRoom::SetRoomEnvAudioProps(int count, CExoString* names, int* env, float* level) RET 10h.
void __fastcall recovered_room_set_env_audio_props_005c7dc0(void* self, void*, I32 count, const void* names,
                                                            const U* env, const U* level) noexcept(false) {
    if (rp(self, 0x38) != nullptr) return;                            // 005C7DDB..005C7DE3
    ExoString room_name;                                              // [esp+0Ch]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&room_name), nullptr);   // 005C7DE6..005C7DEA
    c2da_ref_to_string_00405f70(at(self, 0x20), nullptr, &room_name);  // 005C7DEF..005C7DFB
    I32 i = 0;
    for (; i < count; ++i) {                                          // 005C7E04..005C7E2B
        const void* const entry = static_cast<const unsigned char*>(names) + static_cast<U>(i) * 8;
        if (ini_comparison::CompareNC005e58c0(reinterpret_cast<kotor_reconstruction::StartupString*>(&room_name), nullptr,
                async_string_cstr_005e5670(const_cast<void*>(entry), nullptr)) != 0)   // 005C7E10..005C7E23
            break;
    }
    if (i != count) {                                                 // 005C7E2E..005C7E39
        st<U>(self, 0x30, env[i]);                                    // 005C7E3B..005C7E46
        st<U>(self, 0x34, level[i]);                                  // 005C7E42..005C7E4C
    }
    frame_LogStringDtor_005e5c20(&room_name, nullptr);                // 005C7E4F..005C7E53
}
}  // extern "C"
