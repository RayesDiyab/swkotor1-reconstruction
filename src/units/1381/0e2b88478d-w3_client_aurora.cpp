// lane-w3 unit w3-client-aurora: the thin Aurora-API leaves that CSWCArea::LoadArea / the area handler call and that
// runtime1115 does not provide (the render core underneath — Scene, FindScene, PartEmitter, GL imports — is REAL and
// is only called here, never re-implemented):
//   00420490 AurTextureSuspendUpdates      004204A0 AurTextureResumeUpdates
//   0044F760 AurEnableInterleavedAnimations 0044F770 AurDisableInterleavedAnimations
//   004A6870 CAurGrassInfo::CAurGrassInfo   004A6C70 CExoArrayList<CAurGrassInfo>::allocate
//   004A74F0 AurGrassAddInfo                004A67A0 AurGrassClearInfo
//   0044FCD0 CAurScene::SetAmbient          00401690 SetClearColor
//   004530D0 AurSetGlobalWind               00453140 AurSetGlobalWindVariation
//   004A5900 WindManager::SetGlobalWind     004A57A0 WindManager::SetMaximumDeviation
// Storage cells are the existing 1115 owners (texture_cells / core46-cells-storage); no cell is redefined here.
#include "dd06b37d22-w3_client.h"

using namespace kotor_w3;

namespace texture_cells { extern volatile unsigned char is_suspended_007a46d5; }   // [007A46D5]
namespace kotor_renderer_init { void __stdcall glClearColor_0073d220(float, float, float, float); }   // [0073D220] IAT

extern "C" {
extern unsigned char scene3d_interleaved_007b92f4;                     // [007B92F4]
// [00831E44] CExoArrayList<CAurGrassInfo> {data, count (+4 = 00831E48), capacity (+8 = 00831E4C)}
extern unsigned char scene3d_grass_info_00831e44[12];
void* __cdecl find_scene_0044fd60(const char* name);                 // 0044FD60 FindScene
void __fastcall part_emitter_calculate_radius_0048e1b0(void* emitter, void*);   // 0048E1B0
int __cdecl kotor_sscanf_006fae08(const char*, const char*, ...);    // 006FAE08 sscanf
}

namespace {
const char kMainScene_0074155c[] = "mainscene";                        // .rdata 0074155C
const char kScene_0073e460[] = "scene";                                // .rdata 0073E460
const char kClearFormat_0073d5dc[] = "%f %f %f %f";                    // .rdata 0073D5DC
constexpr U kQuarter = 0x3E800000u;                                    // 0.25f immediate
constexpr U kOne_0073d764 = 0x3F800000u;                               // .rdata 0073D764 (1.0f)
constexpr U kDegToRad_007415f8 = 0x3C8EFA35u;                          // .rdata 007415F8 (pi/180 as float)
constexpr unsigned kGrassInfoSize = 0x54;
U grass_count() noexcept { return ld<U>(scene3d_grass_info_00831e44, 4); }
}  // namespace

extern "C" {
// 00420490 / 004204A0
void __cdecl recovered_aur_texture_suspend_updates_00420490() noexcept { texture_cells::is_suspended_007a46d5 = 1; }   // 00420490
void __cdecl recovered_aur_texture_resume_updates_004204a0() noexcept { texture_cells::is_suspended_007a46d5 = 0; }    // 004204A0
// 0044F760 / 0044F770
void __cdecl recovered_aur_enable_interleaved_animations_0044f760() noexcept { scene3d_interleaved_007b92f4 = 1; }     // 0044F760
void __cdecl recovered_aur_disable_interleaved_animations_0044f770() noexcept { scene3d_interleaved_007b92f4 = 0; }    // 0044F770
// 004A67A0 AurGrassClearInfo: count = 0.
void __cdecl recovered_aur_grass_clear_info_004a67a0() noexcept { st<U>(scene3d_grass_info_00831e44, 4, 0); }          // 004A67A0

// 004A6870 CAurGrassInfo::CAurGrassInfo (54h bytes). ECX this, RET, EAX this.
void* __fastcall recovered_aur_grass_info_construct_004a6870(void* self, void*) noexcept {
    for (unsigned o = 0x28; o <= 0x3C; o += 4) st<U>(self, o, 0);     // 004A6874..004A6883
    st<U8>(self, 0, 0);                                               // 004A6886 name[0]
    st<U>(self, 0x20, 0);                                             // 004A6888
    st<U>(self, 0x24, 0);                                             // 004A688B
    st<U8>(self, 0x50, 0);                                            // 004A688E
    for (unsigned o = 0x40; o <= 0x4C; o += 4) st<U>(self, o, kQuarter);   // 004A6891..004A689F
    return self;
}

// 004A6C70 CExoArrayList<CAurGrassInfo>::allocate(int capacity). ECX list, RET 4.
void __fastcall recovered_aur_grass_info_array_allocate_004a6c70(void* list, void*, I32 capacity) noexcept(false) {
    void* const old = rp(list, 0);                                    // 004A6C7A..004A6C83
    st<I32>(list, 8, capacity);                                       // 004A6C87
    unsigned char* data = static_cast<unsigned char*>(live_operator_new(static_cast<U>(capacity) * kGrassInfoSize));   // 004A6C7C..004A6C8A
    if (data != nullptr) {                                            // 004A6C94..004A6C96
        for (I32 i = 0; i < capacity; ++i)                            // 004A6C98..004A6CD6 (inlined constructor)
            recovered_aur_grass_info_construct_004a6870(data + static_cast<U>(i) * kGrassInfoSize, nullptr);
    }
    st<void*>(list, 0, data);                                         // 004A6CDC
    const I32 count = ld<I32>(list, 4);                               // 004A6CDE
    for (I32 i = 0; i < count; ++i)                                   // 004A6CE5..004A6D0B rep movsd 15h dwords
        std::memcpy(static_cast<unsigned char*>(rp(list, 0)) + static_cast<U>(i) * kGrassInfoSize,
                    static_cast<const unsigned char*>(old) + static_cast<U>(i) * kGrassInfoSize, kGrassInfoSize);
    if (old != nullptr) live_free(old);                               // 004A6D0D..004A6D1A
}

// 004A74F0 AurGrassAddInfo(int index, CAurGrassInfo* info) cdecl. Grows the global list with default entries up to
// index, then copies the info into slot index and normalises its four probabilities when their sum is not 1.
void __cdecl recovered_aur_grass_add_info_004a74f0(I32 index, const void* info) noexcept(false) {
    I32 count = static_cast<I32>(grass_count());                       // 004A74FB
    while (index >= count) {                                           // 004A7503..004A7505 / 004A75B1..004A75B9
        unsigned char local[kGrassInfoSize];                           // [esp+10h]
        std::memset(local, 0, sizeof local);                           // (bytes 1..1Fh are the original's stack garbage)
        for (unsigned o = 0x40; o <= 0x4C; o += 4) st<U>(local, o, kQuarter);   // 004A7510..004A7521
        const I32 capacity = ld<I32>(scene3d_grass_info_00831e44, 8);  // 004A7525
        if (count == capacity)                                         // 004A752A..004A7574
            recovered_aur_grass_info_array_allocate_004a6c70(scene3d_grass_info_00831e44, nullptr,
                                                             capacity != 0 ? capacity * 2 : 8);   // 004A7576..004A7589
        count = static_cast<I32>(grass_count());                       // 004A758E
        std::memcpy(static_cast<unsigned char*>(rp(scene3d_grass_info_00831e44, 0)) + static_cast<U>(count) * kGrassInfoSize,
                    local, kGrassInfoSize);                            // 004A7594..004A75A8
        ++count;                                                       // 004A75AA..004A75B0
        st<I32>(scene3d_grass_info_00831e44, 4, count);                // 004A75B3
    }
    unsigned char* const entry = static_cast<unsigned char*>(rp(scene3d_grass_info_00831e44, 0)) +
                                 static_cast<U>(index) * kGrassInfoSize;   // 004A75BF..004A75CA
    st<U>(entry, 0x20, ld<U>(info, 0x20));                             // 004A75D1..004A75D4
    st<U>(entry, 0x24, ld<U>(info, 0x24));                             // 004A75D7..004A75DA
    std::memcpy(entry + 0x34, static_cast<const unsigned char*>(info) + 0x34, 12);   // 004A75DD..004A75F0
    std::memcpy(entry + 0x28, static_cast<const unsigned char*>(info) + 0x28, 12);   // 004A75F3..004A7606
    const float p0 = ld<float>(info, 0x40), p1 = ld<float>(info, 0x44), p2 = ld<float>(info, 0x48), p3 = ld<float>(info, 0x4C);
    std::memcpy(entry + 0x40, static_cast<const unsigned char*>(info) + 0x40, 16);   // 004A7609..004A762A
    // 004A760C..004A7627 x87 sum (53-bit precision control: double), fcom against 1.0f
    const double sum = static_cast<double>(p0) + static_cast<double>(p1) + static_cast<double>(p2) + static_cast<double>(p3);
    if (!(sum == static_cast<double>(fbits(kOne_0073d764)))) {        // 004A762D..004A7638 (jnp: skip when equal)
        const double inverse = static_cast<double>(fbits(kOne_0073d764)) / sum;   // 004A763A fdivr
        st<float>(entry, 0x40, static_cast<float>(inverse * ld<float>(entry, 0x40)));   // 004A7640..004A7645
        st<float>(entry, 0x44, static_cast<float>(inverse * ld<float>(entry, 0x44)));   // 004A7648..004A764D
        st<float>(entry, 0x48, static_cast<float>(inverse * ld<float>(entry, 0x48)));   // 004A7650..004A7655
        st<float>(entry, 0x4C, static_cast<float>(inverse * ld<float>(entry, 0x4C)));   // 004A7658..004A765B
    }
    unsigned char* d = entry;                                          // 004A7662..004A7678 strcpy(entry, info)
    const unsigned char* s = static_cast<const unsigned char*>(info);
    unsigned char c;
    do { c = *s++; *d++ = c; } while (c != 0);
    st<U8>(entry, 0x50, 1);                                            // 004A767A..004A7680
}

// 0044FCD0 CAurScene::SetAmbient(Vector): [this+88h] = colour. ECX scene, RET 0Ch.
void __fastcall recovered_aur_scene_set_ambient_0044fcd0(void* scene, void*, float r, float g, float b) noexcept {
    st<float>(scene, 0x88, r);                                         // 0044FCD0..0044FCDE
    st<float>(scene, 0x8C, g);                                         // 0044FCE4
    st<float>(scene, 0x90, b);                                         // 0044FCE7
}

// 00401690 SetClearColor(char const* "r g b a") cdecl: sscanf four floats (missing ones stay 0) -> glClearColor.
void __cdecl recovered_set_clear_color_00401690(const char* text) noexcept(false) {
    float l0 = 0.0f, l1 = 0.0f, l2 = 0.0f, l3 = 0.0f;                  // 004016B1..004016C9
    kotor_sscanf_006fae08(text, kClearFormat_0073d5dc, &l3, &l2, &l1, &l0);   // 0040169D..004016D1
    kotor_renderer_init::glClearColor_0073d220(l3, l2, l1, l0);        // 004016D6..004016ED
}

// 004A5900 WindManager::SetGlobalWind(Vector): [this+18h] = wind; recalculate every emitter radius. RET 0Ch.
void __fastcall recovered_wind_manager_set_global_wind_004a5900(void* self, void*, float x, float y, float z) noexcept(false) {
    st<float>(self, 0x18, x);                                          // 004A590E
    st<float>(self, 0x1C, y);                                          // 004A5914
    st<float>(self, 0x20, z);                                          // 004A5917
    for (I32 i = 0; i < ld<I32>(self, 0x850); ++i)                     // 004A591A..004A5947
        part_emitter_calculate_radius_0048e1b0(rp(rp(self, 0x84C), static_cast<U>(i) * 4), nullptr);   // 004A5930..004A5939
}

// 004A57A0 WindManager::SetMaximumDeviation(float, float): [+838h], [+83Ch]. RET 8.
void __fastcall recovered_wind_manager_set_maximum_deviation_004a57a0(void* self, void*, float a, float b) noexcept {
    st<float>(self, 0x838, a);                                         // 004A57A8
    st<float>(self, 0x83C, b);                                         // 004A57AE
}

// 004530D0 AurSetGlobalWind(Vector) cdecl -> AL. Scene "mainscene", else "scene"; its WindManager is [scene+84h].
U8 __cdecl recovered_aur_set_global_wind_004530d0(float x, float y, float z) noexcept(false) {
    void* scene = find_scene_0044fd60(kMainScene_0074155c);           // 004530D3..004530D8
    if (scene == nullptr) {                                            // 004530E0..004530E2
        scene = find_scene_0044fd60(kScene_0073e460);                 // 004530E4..004530E9
        if (scene == nullptr) return 0;                               // 004530F1..004530F3 / 00453137
    }
    recovered_wind_manager_set_global_wind_004a5900(rp(scene, 0x84), nullptr, x, y, z);   // 004530F5..0045312C
    return 1;                                                          // 00453131
}

// 00453140 AurSetGlobalWindVariation(float, float) cdecl -> AL. Both values scaled by [007415F8] (degrees->radians).
U8 __cdecl recovered_aur_set_global_wind_variation_00453140(float a, float b) noexcept(false) {
    void* scene = find_scene_0044fd60(kMainScene_0074155c);           // 00453140..00453145
    if (scene == nullptr) {                                            // 0045314D..0045314F
        scene = find_scene_0044fd60(kScene_0073e460);                 // 00453151..00453156
        if (scene == nullptr) return 0;                               // 0045315E..00453160 / 0045318E
    }
    const float ra = a * fbits(kDegToRad_007415f8);                   // 00453162..00453175
    const float rb = b * fbits(kDegToRad_007415f8);                   // 00453179..00453183
    recovered_wind_manager_set_maximum_deviation_004a57a0(rp(scene, 0x84), nullptr, ra, rb);   // 00453166..00453186
    return 1;                                                          // 0045318B
}
}  // extern "C"
