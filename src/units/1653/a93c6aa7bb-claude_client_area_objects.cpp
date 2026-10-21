// Lane W4 (client-area-objects worker): CSWCArea content objects reached by CSWCArea::LoadArea (00607610) on the live
// Endar Spire client area load, plus the CSWArea / CSWCArea virtual slots the W3 CSWCArea table (0074EEF0) publishes.
// Replaces the W3 D83 traps of the same exported names (w3_traps.cpp). Transcribed from the original swkotor.exe
// disassembly (capstone linear sweep); every statement cites its original instruction range.
//
//   0058FCE0 CSWArea::SetFog                    thiscall, 5 stack args, RET 14h (W3 trap declared @20: see below)
//   0058FD10 CSWArea::SetIsNight                thiscall, 1 stack arg,  RET 4    @12
//   00604DB0 CSWCArea::SetWeather               thiscall, 2 stack args, RET 8    @16
//   00606E30 CSWCArea::AddPlaceableObjectLight  thiscall, 5 stack args, RET 14h  @28
//   006072D0 CSWCArea::AddPlaceableObjectStatic thiscall, 8 stack args, RET 20h  @40
//   00690EF0 CSWBehaviorAttachSound::CSWBehaviorAttachSound thiscall, 4 stack args, RET 10h @24
//   00690FC0 CSWBehaviorAttachSound::Control    thiscall, 1 stack arg,  RET 4    (vtable 00754418 +08)
//   00691000 CSWBehaviorAttachSound::PlaySoundA thiscall, RET 0                   @8
//   00691030 CSWBehaviorAttachSound::~CSWBehaviorAttachSound  thiscall, RET 0
//   006910B0 CSWBehaviorAttachSound scalar deleting destructor thiscall(flags), RET 4 (vtable 00754418 +00)
//   00604B50 PlayRoomSoundEvent                 cdecl(gob, event, behavior)
//   00754418 CSWBehaviorAttachSound virtual table (9 slots)
//
// Original quirks preserved (documented at each site):
//  * AddPlaceableObjectLight: an existing record with the same id gets a NEW CAurObject stored over [rec+4] (the old
//    object leaks); if that NewCAurObject fails the existing record is freed while still referenced by the area
//    array [+218h]. A failed LightColor lookup returns 1 without creating anything.
//  * AddPlaceableObjectStatic: the model-name CExoString looked up from placeables.2da (LowGore / ModelName) is never
//    used; a failed AddExternalObject returns 0 and leaks the constructed placeable; the YawPitchRoll result is
//    discarded.
//  * SetWeather: a NewCAurObject failure inside the 6-object loop returns 0 without storing the weather byte and
//    leaves the already created objects in [+18Ch..]; the lightning object is only (re)created for weather 1 / 2.
#include "5cdbe99b1f-gui_rt.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned capacity);   // CExoArrayList::Allocate
}
namespace menu_tables_required {
void* __fastcall StreamConstruct_005d5a50(void* self, void*);                                   // CExoStreamingSoundSource()
void __fastcall StreamSetLooping_005d5b40(void* stream, void*, unsigned looping);              // SetLooping RET 4
void __fastcall StreamSetPriority_005d5ba0(void* stream, void*, unsigned group);               // SetPriorityGroup RET 4
unsigned __fastcall StreamIsPlaying_005d5af0(void* stream, void*);                             // IsPlaying
unsigned __fastcall StreamPlay_005d5b10(void* stream, void*, menu_resources::Ref* ref, unsigned, unsigned, unsigned,
                                        unsigned);                                             // Play RET 14h
void __fastcall AnimateExternal_0043e150(void* object, void*, float weight);                   // CAurObject::AnimateExternal RET 4
}

extern "C" {
// ---- existing providers (c05-pre map symbols)
extern void* kotor_cell_rules_007a3a28;                                                          // [007A3A28] CSWRules*
void* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob, void* mdl, void* mdx);   // NewCAurObject
int __fastcall c2da_current_get_int_00413660(void* table, void*, unsigned row, void* column, unsigned* out);   // GetINTEntry RET 0Ch
int __fastcall c2da_current_get_float_00413430(void* table, void*, int row, void* column, float* out);       // GetFLOATEntry RET 0Ch
int __fastcall c2da_current_get_string_00413270(void* table, void*, int row, void* column, void* out);       // GetCExoStringEntry RET 0Ch
void* __fastcall kotor_fastcall_005ed570(void* app, void*) noexcept;                             // GetGameObjectArray
void* __fastcall recovered_client_get_placeable_by_id_005ed5f0(void* app, void*, std::uint32_t id) noexcept(false);   // RET 4
void* __fastcall recovered_client_placeable_construct_00682270(void* self, void*) noexcept(false);   // CSWCPlaceable()
void __fastcall recovered_client_placeable_set_appearance_00682e20(void* self, void*, std::uint32_t appearance) noexcept(false);   // RET 4
std::uint8_t __fastcall recovered_object_array_add_external_004d7ff0(void* array, void*, std::uint32_t* id, void* object,
                                                                    std::uint32_t) noexcept(false);   // RET 0Ch
std::uint32_t __fastcall kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(void* app, void*, std::uint32_t id);   // RET 4
double __cdecl kotor_m15a_yaw_004aa0f0(const W4Vector* v);                                       // Yaw (ST0 at PC53)
void* __cdecl yaw_pitch_roll_004acac0(void* out_quaternion, float yaw, float pitch, float roll); // YawPitchRoll
void* __fastcall behavior_attach_0049fb10(void* self, void*, void* gob, void* object, void* part);   // CAurBehaviorAttach() RET 0Ch
void __fastcall behavior_attach_destruct_0049fce0(void* self, void*);                            // ~CAurBehaviorAttach
std::uint32_t __fastcall behavior_attach_control_0049f3b0(void* self, void*, std::uint32_t raw_delta);   // Control RET 4
void __fastcall behavior_attach_set_scene_0049f0c0(void* self, void*);                           // slot +04
void __fastcall behavior_attach_message_0049ed00(void* self, void*, char* line);                 // slot +0C RET 4
std::uint32_t __fastcall behavior_attach_get_type_0049fcd0(void* self, void*);                   // slot +10 (3E9h)
void* __fastcall behavior_self_return_00641db0(void* self, void*);                               // slot +14
std::uint32_t __fastcall behavior_return_zero_0063e7f0(void* self, void*);                       // slots +18 / +1C
void __fastcall behavior_attach_update_gob_position_0049ecd0(void* self, void*);                 // slot +20
void* __fastcall kotor_fastcall_00405ed0(void* self, void*);                                     // CResRef()
void* __fastcall c2da_copy_00406120(void* self, void*, void* out, const void* source);           // CResRef::operator= RET 8
void __fastcall kotor_m15_stream_source_set_3d_005d6290(void* stream, void*, std::uint32_t is3d);            // Set3D RET 4
void __fastcall kotor_m15_stream_source_set_distance_005d62a0(void* stream, void*, float min_d, float max_d); // SetDistance RET 8
void __fastcall kotor_m15_stream_source_set_position_005d62c0(void* stream, void*, const void* position);    // SetPosition RET 4
void __fastcall kotor_fastcall_005d5b80(void* stream, void*);                                    // CExoStreamingSoundSource::Stop
void __fastcall kotor_fastcall_005d6200(void* stream, void*);                                    // ~CExoStreamingSoundSource
}

namespace {
// .rdata strings / constants
const char kWeather_0074ee28[] = "weather";
const char kFxSnow_0074ee20[] = "fx_snow";
const char kFxRain_0074ee18[] = "fx_rain";
const char kWeatherLight_0074ee08[] = "weatherlight";
const char kFxLightning_0074edf8[] = "fx_lightning01";
const char kSnow_0074edf0[] = "snow";
const char kRain_0074ede8[] = "rain";
const char kLightColor_007494bc[] = "LightColor";
const char kPlaceableLight_0074ef68[] = "placeablelight";
const char kFxPlaceable_0074ef58[] = "fx_placeable01";
const char kRed_0074ef54[] = "RED";
const char kGreen_0074ef4c[] = "GREEN";
const char kBlue_0074ef44[] = "BLUE";
const char kLightOffsetX_0074ef34[] = "LightOffsetX";
const char kLightOffsetY_0074ef24[] = "LightOffsetY";
const char kLightOffsetZ_0074ef14[] = "LightOffsetZ";
const char kRoot_00740c00[] = "root";
const char kLowGore_0074ef78[] = "LowGore";
const char kModelName_00749814[] = "ModelName";
constexpr U32 kZero_0073d700 = 0x00000000u;          // .rdata 0073D700 (0.0f)
constexpr U32 kOneHundredth_0073dfec = 0x3C23D70Au;  // .rdata 0073DFEC (0.01f)
constexpr U8 kGameObjectOk_0074666c = 0;             // .rdata 0074666C (byte 00h)

// CExoString() / ~CExoString RAII (005B3190 / 005E5C20): the destructor is the original EH state cleanup.
struct ExoDefault {
    U32 storage[2];
    ExoDefault() { kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(storage), nullptr); }
    ~ExoDefault() { frame_LogStringDtor_005e5c20(storage, nullptr); }
};
// [[007A3A28]+B8h]+off: the CSWRules 2DA table pointers (re-read on every use, as the original does).
inline void* rules_table(U32 off) { return lp(lp(kotor_cell_rules_007a3a28, 0xB8), off); }
// CExoArrayList<T> append at list (data +0, count +4, capacity +8): grow to 2*capacity (16 when 0) when full.
inline void array_append(void* list, U32 value) {
    const U32 capacity = lw(list, 8);
    if (lw(list, 4) == capacity)
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            capacity != 0 ? capacity + capacity : 0x10u);
    sw(lp(list, 0), lw(list, 4) * 4u, value);
    sw(list, 4, lw(list, 4) + 1u);
}
// Virtual scalar deleting destructor call (slot +00, flags 1) and the slot store clear.
inline void delete_slot(void* self, U32 off) {
    void* const object = lp(self, off);
    if (object != nullptr) {
        vcall<void*, U32>(object, 0x00, 1u);
        sw(self, off, 0);
    }
}
}  // namespace

extern "C" {
// Forward declarations of this unit's own exports (the vtable / tail call references).
std::uint32_t __fastcall recovered_behavior_attach_sound_play_00691000(void* self, void*);
std::uint32_t __fastcall kotor_fastcall_00690fc0(void* self, void*, std::uint32_t raw_delta);
void __fastcall kotor_fastcall_00691030(void* self, void*);
void* __fastcall kotor_fastcall_006910b0(void* self, void*, std::uint32_t flags);

// ============================================================================ 0058FCE0 CSWArea::SetFog
// mov edx,[esp+0Ch]; lea eax,[ecx+54h]; [eax]=a3; [eax+4]=a4; [eax+8]=a5; [ecx+60h]=a1; [ecx+64h]=a2; mov eax,1;
// ret 14h. FIVE stack dwords (raw copies, no FPU). The W3 table/trap declared this slot with three stack dwords
// (@recovered_area_set_fog_0058fce0@20, RET 0Ch), which does not match the original RET 14h. This body is defined
// with the original ABI (@28) and the old @20 reference (W3 vtable 0074EEF0 +08) is redirected to it below, so the
// slot pops 14h bytes exactly as the original.
std::uint32_t __fastcall recovered_area_set_fog_0058fce0(void* self, void*, std::uint32_t a1, std::uint32_t a2,
                                                         std::uint32_t c0, std::uint32_t c1, std::uint32_t c2) {
    sw(self, 0x54, c0);                                               // 0058FCE0..0058FCE7
    sw(self, 0x58, c1);                                               // 0058FCE9..0058FCED
    sw(self, 0x5C, c2);                                               // 0058FCF0..0058FCF4
    sw(self, 0x60, a1);                                               // 0058FCF7 / 0058FCFF
    sw(self, 0x64, a2);                                               // 0058FCFB / 0058FD02
    return 1;                                                         // 0058FD05..0058FD0A
}
#pragma comment(linker, "/alternatename:@recovered_area_set_fog_0058fce0@20=@recovered_area_set_fog_0058fce0@28")

// ============================================================================ 0058FD10 CSWArea::SetIsNight
std::uint32_t __fastcall recovered_area_set_is_night_0058fd10(void* self, void*, std::uint32_t value) {
    sw(self, 0xA4, value);                                            // 0058FD10..0058FD14
    return 1;                                                         // 0058FD1A..0058FD1F
}

// ============================================================================ 00604DB0 CSWCArea::SetWeather
// (BYTE weather, float amount) RET 8. Six weather CAurObjects at [+18Ch..+1A0h], lightning at [+1A4h], current
// weather byte [+1F0h], lightning flag byte [+AAh], scene [+184h]. EH state 0 = the local CExoString {esp+18h}.
std::uint32_t __fastcall recovered_client_area_set_weather_00604db0(void* self, void*, std::uint32_t weather_arg,
                                                                    float amount) {
    const U8 weather = static_cast<U8>(weather_arg);                  // byte [esp+30h]
    ExoDefault animation;                                             // 00604DCE..00604DD2 CExoString() {esp+18h}
    if (amount == bits_f(kZero_0073d700)) {                           // 00604DD7..00604DEE fcomp; test ah,44h; jnp (equal only)
        for (U32 i = 0; i < 6; ++i) delete_slot(self, 0x18C + i * 4u);   // 00604F56..00604F77
        delete_slot(self, 0x1A4);                                     // 00604F79..00604F89
        sb(self, 0x1F0, 0);                                           // 00604F93
    } else {                                                          // non-zero and unordered (NaN) amounts
        if (lb(self, 0x1F0) != weather) {                             // 00604DF4..00604DFE
            for (U32 i = 0; i < 6; ++i) delete_slot(self, 0x18C + i * 4u);   // 00604E04..00604E27
            if (weather == 2 || weather == 1) {                       // 00604E29..00604E37 (0 -> 00604E9E, other -> fail)
                const char* const effect = weather == 2 ? kFxSnow_0074ee20 : kFxRain_0074ee18;   // 00604E44 / 00604E77
                for (U8 b = 0; b < 6; ++b) {                          // 00604E3D..00604E6D / 00604E71..00604E9C
                    void* const object = aurora_new_aur_object_00449cc0(effect, kWeather_0074ee28, nullptr, nullptr);
                    sw(self, 0x18C + static_cast<U32>(b) * 4u, ad(object));   // 00604E5B / 00604E8E
                    if (object == nullptr) return 0;                  // 00604E62 / 00604E95 -> 00604EF8 (dtor, return 0)
                }
            } else if (weather != 0) {
                return 0;                                             // 00604E37 jne 00604EF8
            }
            for (U32 i = 0; i < 6; ++i) {                             // 00604E9E..00604EBB
                void* const object = lp(self, 0x18C + i * 4u);
                if (object != nullptr) vcall<U32, void*>(object, 0x4C, lp(self, 0x184));   // 00604EAB..00604EB4 AttachToScene
            }
            sb(self, 0x1F0, weather);                                 // 00604EBD..00604EC1
        }
        const U8 lightning = lb(self, 0xAA);                          // 00604EC7
        void* const light = lp(self, 0x1A4);                          // 00604ECF
        if (lightning != 0) {                                         // 00604ECD..00604ED5 (jbe on al=0)
            if (light != nullptr) {                                   // 00604ED7..00604EE1
                vcall<void*, U32>(light, 0x00, 1u);
                sw(self, 0x1A4, 0);                                   // 00604EE1
            }
            const U8 current = lb(self, 0x1F0);                       // 00604EEB
            if (current != 1 && current != 2) return 0;               // 00604EF2..00604EF6 -> 00604EF8
            void* const object = aurora_new_aur_object_00449cc0(kFxLightning_0074edf8, kWeatherLight_0074ee08, nullptr,
                                                                nullptr);   // 00604F10..00604F1E
            sw(self, 0x1A4, ad(object));                              // 00604F2A
            if (object != nullptr) vcall<U32, void*>(object, 0x4C, lp(self, 0x184));   // 00604F28..00604F3B
        } else if (light != nullptr) {                                // 00604F40..00604F4A
            vcall<void*, U32>(light, 0x00, 1u);
            sw(self, 0x1A4, 0);
        }
    }
    if (weather == 1) GuiAssign005e5140(animation.storage, nullptr, kRain_0074ede8);        // 00604F9A..00604FB5
    else if (weather == 2) GuiAssign005e5140(animation.storage, nullptr, kSnow_0074edf0);   // 00604FA2..00604FB5
    for (U32 i = 0; i < 6; ++i) {                                     // 00604FBA..00605054
        void* const object = lp(self, 0x18C + i * 4u);                // 00604FC5
        if (object == nullptr) continue;                              // 00604FC7..00604FC9
        using Play = U32 (__thiscall*)(void*, const char*, U32, U32, U32);
        using Get = U32 (__thiscall*)(void*, const char*, void*, void*);
        reinterpret_cast<Play>(vslot(object, 0x18))(object, frame_LogCStr_005e5670(animation.storage, nullptr), 0, 0x80,
                                                    0);               // 00604FCF..00604FE6 PlayAnimation(name, 0, 80h, 0)
        void* const o2 = lp(self, 0x18C + i * 4u);
        const float rate = static_cast<float>(static_cast<double>(amount) *
                                              static_cast<double>(bits_f(kOneHundredth_0073dfec)));   // 00604FE9..00605000 fmul; fstp dword
        reinterpret_cast<Play>(vslot(o2, 0x18))(o2, frame_LogCStr_005e5670(animation.storage, nullptr), f_bits(rate), 0,
                                                0);                   // 00604FF5..0060500B
        void* const o3 = lp(self, 0x18C + i * 4u);
        U32 out_14, out_10;                                           // {esp+14h} / {esp+10h} written by the slot
        reinterpret_cast<Get>(vslot(o3, 0x1C))(o3, frame_LogCStr_005e5670(animation.storage, nullptr), &out_14,
                                               &out_10);              // 0060500E..00605028
        menu_tables_required::AnimateExternal_0043e150(lp(self, 0x18C + i * 4u), nullptr, bits_f(out_10));   // 0060502B..00605032
        void* const o4 = lp(self, 0x18C + i * 4u);
        reinterpret_cast<Play>(vslot(o4, 0x18))(o4, frame_LogCStr_005e5670(animation.storage, nullptr), 0, 0,
                                                0);                   // 00605037..0060504D
    }
    return 1;                                                         // 0060505A..0060506B (dtor, return 1)
}

// ============================================================================ 00606E30 CSWCArea::AddPlaceableObjectLight
// (id, BYTE kind, Vector position) RET 14h. Area light records (8 bytes: {id, CAurObject*}) live in the
// CExoArrayList at [+218h]. EH states 0..6 each destroy the temporary column CExoString {esp+18h}.
std::uint32_t __fastcall recovered_client_area_add_placeable_light_00606e30(void* self, void*, std::uint32_t id,
                                                                           std::uint32_t kind, float x, float y,
                                                                           float z) {
    float orientation_58[4] = {1.0f, 0.0f, 0.0f, 0.0f};               // 00606E60..00606E78 {58h}=1.0, {5Ch..64h}=0
    float color_40[3] = {0.0f, 0.0f, 0.0f};                           // 00606E80..00606E90
    float offset_28[3] = {0.0f, 0.0f, 0.0f};                          // 00606E98..00606EA8
    float node_34[3] = {0.0f, 0.0f, 0.0f};                            // 00606EB0..00606EC0
    U32 return_4c[3];                                                 // {4Ch} hidden return of slot +5C
    unsigned color_row_10;                                            // {10h} LightColor value
    const U8 row = static_cast<U8>(kind);                             // 00606ED8 movzx edi,byte [ebp+0Ch]
    bool missing;
    {
        ExoString column(kLightColor_007494bc);                       // 00606E53..00606EC8
        missing = c2da_current_get_int_00413660(rules_table(0x2C), nullptr, row, column.storage, &color_row_10) == 0;   // 00606ECD..00606F06
    }                                                                 // 00606F02..00606F10
    if (missing) return 1;                                            // 00606F15..00606F17 -> 006072AB (eax=1)
    U32 found = 0;                                                    // 00606F26..00606F2A {14h}
    void* record = nullptr;
    const I32 count = li(self, 0x21C);                                // 00606F1D
    void* const records = lp(self, 0x218);                            // 00606F30..00606F36 {18h}
    for (I32 i = 0; i < count; ++i) {                                 // 00606F28..00606F50 (signed jle / jl)
        void* const entry = lp(records, static_cast<U32>(i) * 4u);
        if (entry != nullptr && lw(entry, 0) == id) {                 // 00606F42..00606F48
            record = lp(records, static_cast<U32>(i) * 4u);           // 00606F9E..00606FA2
            found = 1;                                                // 00606FA5
            break;
        }
    }
    if (found == 0) {
        record = live_operator_new(8);                                // 00606F52..00606F59
        if (record == nullptr) return 0;                              // 00606F5E..00606F60 -> 00606F88
    }
    void* const object = aurora_new_aur_object_00449cc0(kFxPlaceable_0074ef58, kPlaceableLight_0074ef68, nullptr,
                                                        nullptr);     // 00606F62..00606F75
    sw(record, 4, ad(object));                                        // 00606F7A (an existing record's old object leaks)
    if (object == nullptr) {                                          // 00606F78..00606F7D
        live_free(record);                                            // 00606F7F..00606F85 (frees an existing record too)
        return 0;                                                     // 00606F88
    }
    sw(record, 0, id);                                                // 00606FB8
    {
        ExoString column(kRed_0074ef54);                              // 00606FAF..00606FBA
        c2da_current_get_float_00413430(rules_table(0x48), nullptr, static_cast<int>(color_row_10), column.storage,
                                        &color_40[0]);                // 00606FBF..00606FEA
    }                                                                 // 00606FEF..00606FFA
    {
        ExoString column(kGreen_0074ef4c);                            // 00606FFF..00607008
        c2da_current_get_float_00413430(rules_table(0x48), nullptr, static_cast<int>(color_row_10), column.storage,
                                        &color_40[1]);                // 0060700D..00607038
    }
    {
        ExoString column(kBlue_0074ef44);                             // 0060704A..00607053
        c2da_current_get_float_00413430(rules_table(0x48), nullptr, static_cast<int>(color_row_10), column.storage,
                                        &color_40[2]);                // 00607058..00607083
    }
    {
        ExoString column(kLightOffsetX_0074ef34);                     // 00607095..0060709E
        missing = c2da_current_get_float_00413430(rules_table(0x2C), nullptr, row, column.storage, &offset_28[0]) == 0;   // 006070A3..006070D9
    }
    if (missing) offset_28[0] = 0.0f;                                 // 006070E8..006070EC
    {
        ExoString column(kLightOffsetY_0074ef24);                     // 006070F4..006070FD
        missing = c2da_current_get_float_00413430(rules_table(0x2C), nullptr, row, column.storage, &offset_28[1]) == 0;   // 00607102..00607137
    }
    if (missing) offset_28[1] = 0.0f;                                 // 00607146..0060714A
    {
        ExoString column(kLightOffsetZ_0074ef14);                     // 00607152..0060715B
        missing = c2da_current_get_float_00413430(rules_table(0x2C), nullptr, row, column.storage, &offset_28[2]) == 0;   // 00607160..00607195
    }
    if (missing) offset_28[2] = bits_f(0x40400000u);                  // 006071A4..006071A8 (3.0f)
    void* const light = lp(record, 4);                                // 006071B0
    vcall<U32, const char*, void*, void*>(light, 0xA0, kRoot_00740c00, node_34, orientation_58);   // 006071B3..006071C4
    const float lx = static_cast<float>(static_cast<double>(node_34[0]) + static_cast<double>(offset_28[0]));   // 006071CA..006071DB
    const float ly = static_cast<float>(static_cast<double>(node_34[1]) + static_cast<double>(offset_28[1]));   // 006071E2..00607207
    const float lz = static_cast<float>(static_cast<double>(node_34[2]) + static_cast<double>(offset_28[2]));   // 0060720B..00607221
    U32 q[4];
    std::memcpy(q, orientation_58, 16);                               // 006071EC..00607200 (Quaternion by value)
    void* const light2 = lp(record, 4);                               // 006071D6
    vcall<U32, const char*, U32, U32, U32, U32, U32, U32, U32>(light2, 0x9C, kRoot_00740c00, f_bits(lx), f_bits(ly),
                                                               f_bits(lz), q[0], q[1], q[2], q[3]);   // 00607229..00607231
    void* const light3 = lp(record, 4);                               // 0060723A
    vcall<void*, void*, U32, U32, U32>(light3, 0x5C, return_4c, f_bits(x), f_bits(y), f_bits(z));   // 00607237..00607257
    void* const light4 = lp(record, 4);                               // 0060725E
    vcall<U32, void*>(light4, 0x4C, lp(self, 0x184));                 // 0060725A..0060726A AttachToScene
    if (found != 0) return 1;                                         // 0060726D..00607273 -> 006072AB
    array_append(at(self, 0x218), ad(record));                        // 00607275..006072A8
    return 1;                                                         // 006072AB..006072C1
}

// ============================================================================ 006072D0 CSWCArea::AddPlaceableObjectStatic
// (id, BYTE kind, Vector position, Vector orientation) RET 20h. Static placeable ids are appended to the
// CExoArrayList at [+224h]. EH: state 0 = model CExoString {esp+20h}; 1..3 = column temporaries; 4 = free(memory).
std::uint32_t __fastcall recovered_client_area_add_placeable_static_006072d0(void* self, void*, std::uint32_t id,
                                                                            std::uint32_t kind, float px, float py,
                                                                            float pz, float ox, float oy, float oz) {
    void* const objects = kotor_fastcall_005ed570(client_app(), nullptr);   // 006072EE..006072FD {14h}
    void* placeable = recovered_client_get_placeable_by_id_005ed5f0(client_app(), nullptr, id);   // 00607302..00607318
    if (placeable == nullptr) {                                       // 0060731A..0060731E (non-null -> 006074F6)
        ExoDefault model;                                             // 00607324..00607335 {20h}, state 0
        void* const options = kotor_fastcall_005ed700(client_app(), nullptr);   // 0060732D..00607339
        if (lb(options, 0x72) == 0) {                                 // 0060733E..00607343
            bool missing;
            {
                ExoString column(kLowGore_0074ef78);                  // 00607349..00607352 {18h}
                missing = c2da_current_get_string_00413270(rules_table(0x2C), nullptr, static_cast<U8>(kind),
                                                           column.storage, model.storage) == 0;   // 00607357..0060738B
            }                                                         // 0060738D..00607392
            if (missing) {                                            // 00607397..00607399
                ExoString column(kModelName_00749814);                // 0060739B..006073A4 {18h}
                c2da_current_get_string_00413270(rules_table(0x2C), nullptr, static_cast<U8>(kind), column.storage,
                                                 model.storage);      // 006073A9..006073CA
            }                                                         // 006073CF / 00607410..00607415
        } else {
            ExoString column(kModelName_00749814);                    // 006073D5..006073DE {28h}
            c2da_current_get_string_00413270(rules_table(0x2C), nullptr, static_cast<U8>(kind), column.storage,
                                             model.storage);          // 006073E3..00607407
        }                                                             // 0060740C..00607415
        void* const memory = live_operator_new(0x150);                // 0060741A..00607427 {18h}
        if (memory != nullptr) {                                      // 0060742B..00607432
            auto free_memory = on_throw([memory]() { live_free(memory); });   // EH state 4 (0072761B -> 00727610)
            placeable = recovered_client_placeable_construct_00682270(memory, nullptr);   // 00607434..0060743B
            free_memory.armed = false;
        }
        if (placeable == nullptr) return 0;                           // 00607441..00607448 -> 00607487 (dtor, return 0)
        recovered_client_placeable_set_appearance_00682e20(placeable, nullptr, kind);   // 0060744A..00607450 (full dword)
        vcall<void, U32>(placeable, 0x04, id);                        // 00607455..0060745D SetId
        vcall<void, void*>(placeable, 0x84, self);                    // 00607460..00607469 SetArea
        // [ebp+8] itself is passed by reference: later reads of id (00607537 / 00607559) see any update.
        if (recovered_object_array_add_external_004d7ff0(objects, nullptr, &id, placeable, 1) !=
            kGameObjectOk_0074666c)                                   // 0060746F..00607485
            return 0;                                                 // 00607487..006074AB (placeable leaks)
        const W4Vector position = {px, py, pz};                       // [ebp+10h..18h]
        const W4Vector orientation = {ox, oy, oz};                    // [ebp+1Ch..24h]
        vcall<void, const W4Vector*>(placeable, 0x8C, &position);     // 006074AE..006074B6 SetPosition
        vcall<void, const W4Vector*>(placeable, 0x88, &orientation);  // 006074BC..006074C4 SetOrientation
        const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&orientation));   // 006074CA..006074D5 fstp dword
        U32 quaternion_30[4];                                         // {30h} (result discarded)
        yaw_pitch_roll_004acac0(quaternion_30, yaw, 0.0f, 0.0f);      // 006074D8..006074E2
    }                                                                 // 006074E5..006074F1 ~CExoString (model)
    sw(placeable, 0x110, 1);                                          // 006074FD
    sw(placeable, 0x128, 0);                                          // 00607507
    sw(placeable, 0x130, 0);                                          // 0060750D
    sw(placeable, 0x114, 0);                                          // 00607513
    sw(placeable, 0x118, 0);                                          // 00607519
    sw(placeable, 0x12C, 0);                                          // 0060751F
    sw(placeable, 0x120, 0);                                          // 00607525
    sw(placeable, 0x124, 0);                                          // 0060752B
    vcall<U32, U32, U32, U32>(placeable, 0x9C, 0, 0, 0);              // 006074F6..00607531 SetAnimation(0,0,0)
    kotor_m15e_client_remove_from_hitcheck_ignore_list_005ed760(client_app(), nullptr, id);   // 00607537..00607544
    array_append(at(self, 0x224), id);                                // 00607549..0060758B
    return 1;                                                         // 0060758E..006075A0
}

// ============================================================================ CSWBehaviorAttachSound (50h bytes)
// +00 vtable 00754418, +04..+38 CAurBehaviorAttach, +3C CResRef sound, +4C CExoStreamingSoundSource*.
extern const void* const kotor_vtable_00754418[9];
const void* const kotor_vtable_00754418[9] = {
    reinterpret_cast<const void*>(&kotor_fastcall_006910b0),                         // +00 006910B0 scalar deleting dtor
    reinterpret_cast<const void*>(&behavior_attach_set_scene_0049f0c0),              // +04 0049F0C0
    reinterpret_cast<const void*>(&kotor_fastcall_00690fc0),                         // +08 00690FC0 Control
    reinterpret_cast<const void*>(&behavior_attach_message_0049ed00),                // +0C 0049ED00
    reinterpret_cast<const void*>(&behavior_attach_get_type_0049fcd0),               // +10 0049FCD0
    reinterpret_cast<const void*>(&behavior_self_return_00641db0),                   // +14 00641DB0
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),                   // +18 0063E7F0
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),                   // +1C 0063E7F0
    reinterpret_cast<const void*>(&behavior_attach_update_gob_position_0049ecd0),    // +20 0049ECD0
};

// 00690EF0 CSWBehaviorAttachSound::CSWBehaviorAttachSound(CAurObject* gob, CAurObject* object, CAurPart* part,
// CResRef const& sound) RET 10h. EH (007804C8): state 0 = ~CAurBehaviorAttach(this); state 1 = free(memory).
void* __fastcall recovered_behavior_attach_sound_construct_00690ef0(void* self, void*, void* gob, void* object,
                                                                    void* part, void* sound) {
    behavior_attach_0049fb10(self, nullptr, gob, object, part);       // 00690F08..00690F21
    auto base = on_throw([self]() { behavior_attach_destruct_0049fce0(self, nullptr); });   // 00690F2B state 0
    sw(self, 0, ad(kotor_vtable_00754418));                           // 00690F33
    kotor_fastcall_00405ed0(at(self, 0x3C), nullptr);                 // 00690F26..00690F39 CResRef()
    unsigned char copied_0c[16];                                      // {esp+0Ch} hidden return
    c2da_copy_00406120(at(self, 0x3C), nullptr, copied_0c, sound);    // 00690F3E..00690F4A operator=
    void* const memory = live_operator_new(4);                        // 00690F4F..00690F59
    void* stream = nullptr;                                           // 00690F6F
    if (memory != nullptr) {                                          // 00690F5D..00690F64
        auto free_memory = on_throw([memory]() { live_free(memory); });   // state 1 (00730108)
        stream = menu_tables_required::StreamConstruct_005d5a50(memory, nullptr);   // 00690F66..00690F68
        free_memory.armed = false;
    }
    sw(self, 0x4C, ad(stream));                                       // 00690F7A
    menu_tables_required::StreamSetLooping_005d5b40(stream, nullptr, 1);   // 00690F71..00690F7D (no null check)
    kotor_m15_stream_source_set_3d_005d6290(lp(self, 0x4C), nullptr, 1);   // 00690F82..00690F87
    menu_tables_required::StreamSetPriority_005d5ba0(lp(self, 0x4C), nullptr, 0x19);   // 00690F8C..00690F91
    kotor_m15_stream_source_set_distance_005d62a0(lp(self, 0x4C), nullptr, bits_f(0x41A00000u),
                                                  bits_f(0x43160000u));   // 00690F96..00690FA3 (20.0f, 150.0f)
    base.armed = false;
    return self;                                                      // 00690FA8..00690FBA
}

// 00690FC0 CSWBehaviorAttachSound::Control(float dt) RET 4: base Control, then the stream follows the gob position.
std::uint32_t __fastcall kotor_fastcall_00690fc0(void* self, void*, std::uint32_t raw_delta) {
    const U32 result = behavior_attach_control_0049f3b0(self, nullptr, raw_delta);   // 00690FC0..00690FCC
    void* const gob = lp(self, 0x10);                                 // 00690FD1
    U32 position_08[3];                                               // {esp+8}
    vcall<void*, void*>(gob, 0x64, position_08);                      // 00690FD4..00690FDD GetPosition
    kotor_m15_stream_source_set_position_005d62c0(lp(self, 0x4C), nullptr, position_08);   // 00690FE0..00690FE8
    return result;                                                    // 00690FED..00690FF4
}

// 00691000 CSWBehaviorAttachSound::PlaySoundA(): if the stream is not playing, Play(&sound, 4, 0, 0, 0). EAX is the
// IsPlaying result (non-zero) or the Play result.
std::uint32_t __fastcall recovered_behavior_attach_sound_play_00691000(void* self, void*) {
    const U32 playing = menu_tables_required::StreamIsPlaying_005d5af0(lp(self, 0x4C), nullptr);   // 00691003..00691006
    if (playing != 0) return playing;                                 // 0069100B..0069100D
    return menu_tables_required::StreamPlay_005d5b10(lp(self, 0x4C), nullptr,
                                                     static_cast<menu_resources::Ref*>(at(self, 0x3C)), 4, playing,
                                                     playing, playing);   // 0069100F..0069101B
}

// 00691030 CSWBehaviorAttachSound::~CSWBehaviorAttachSound. EH (007804EC): state 0 = ~CAurBehaviorAttach(this).
void __fastcall kotor_fastcall_00691030(void* self, void*) {
    auto base = on_throw([self]() { behavior_attach_destruct_0049fce0(self, nullptr); });   // 00691058 state 0
    sw(self, 0, ad(kotor_vtable_00754418));                           // 0069104D
    if (lp(self, 0x4C) != nullptr) {                                  // 00691053..00691060
        kotor_fastcall_005d5b80(lp(self, 0x4C), nullptr);             // 00691062..00691063 Stop
        void* const stream = lp(self, 0x4C);                          // 00691068
        if (stream != nullptr) {                                      // 0069106B..0069106D
            kotor_fastcall_005d6200(stream, nullptr);                 // 0069106F..00691071
            live_free(stream);                                        // 00691076..0069107C
        }
        sw(self, 0x4C, 0);                                            // 0069107F
    }
    base.armed = false;                                               // 00691089 state -1
    behavior_attach_destruct_0049fce0(self, nullptr);                 // 00691087..00691091
}

// 006910B0 scalar deleting destructor (flags) RET 4.
void* __fastcall kotor_fastcall_006910b0(void* self, void*, std::uint32_t flags) {
    kotor_fastcall_00691030(self, nullptr);                           // 006910B1..006910B3
    if ((flags & 1u) != 0) live_free(self);                           // 006910B8..006910C5
    return self;                                                      // 006910C8..006910CB
}

// ============================================================================ 00604B50 PlayRoomSoundEvent
// cdecl(gob, event, behavior): mov ecx,[esp+0Ch]; test ecx,ecx; je ret; jmp 00691000 (tail call).
void __cdecl recovered_play_room_sound_event_00604b50(void*, void*, void* behavior) {
    if (behavior != nullptr) (void)recovered_behavior_attach_sound_play_00691000(behavior, nullptr);   // 00604B50..00604B58
}
}
