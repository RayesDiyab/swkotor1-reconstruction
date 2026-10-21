// Lane W4 GUI successor (worker "gui-char"): CSWMiniGame::Go / SetCamera, reached from
// CClientExoAppInternal::SetInputClass (006200E0) when a mini game is active (ShowPartySelectionGUI path), plus the
// mini-game helpers they reach that had no provider in the c24 candidate map. Replaces the 0782 trap stubs
// @kotor_fastcall_006714e0@8 / @kotor_fastcall_00671670@12.
//   006714E0 CSWMiniGame::Go()                                   thiscall RET
//   00671670 CSWMiniGame::SetCamera(CAurCamera*)                 thiscall RET 4, returns the camera
//   005EDAF0 CClientExoApp::GetMGOArray()                        thiscall RET
//   0066BF30 CSWMiniGameObjectArray::GetMiniGameObject(ulong, BYTE)  thiscall RET 8
//   006701C0 CSWTrackFollower::Go()                              thiscall RET
//   0066D260 CSWTrackFollower::GetModel(int)                     thiscall RET 4
//   0066C870 CSWTrackFollower::SetSpeed(float)                   thiscall RET 4, returns float (ST0)
//   0066FE50 OnAnimKeyEvent(object, const char* event, follower) cdecl (animation key callback)
//   0066DD60 OnReportKey(object, const char*, SWMGKeyInfo* key)  cdecl (animation key callback)
//   0066DD10 CExoArrayList<SWMGKeyInfo>::Add(SWMGKeyInfo)        thiscall RET 24h (36-byte element by value)
//   0066D190 CExoArrayList<SWMGKeyInfo>::Allocate(int)           thiscall RET 4
// Transcribed from the original swkotor.exe linear sweep; each statement carries its original PCs.
#include "6030a9ee65-claude_gui_char.hpp"

namespace frame_control { extern std::uint32_t message_manager_007a3a00; }                       // [007A3A00] CVirtualMachine*
namespace menu_tables_required {
unsigned __fastcall StreamPlay_005d5b10(void* self, void*, menu_resources::Ref* ref, unsigned, unsigned, unsigned, unsigned); // CExoStreamingSoundSource::Play
}

extern "C" {
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void* app, void*);                     // CClientExoApp::GetMiniGame
void* __fastcall behavior_attach_0049fb10(void* self, void*, void* gob, void* object, void* part); // CAurBehaviorAttach::CAurBehaviorAttach
w4::U32 __fastcall kotor_fastcall_005d5920(void* source, void*);                                 // CExoSoundSource::IsHardwarePlayingSoundPleaseDontUseThis
void __fastcall kotor_fastcall_005d5930(void* source, void*);                                    // CExoSoundSource::Play
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, const void* v, w4::U32 flag); // CExoSoundSource::SetPosition
void* __fastcall kotor_fastcall_0066d5d0(void* follower, void*, void* out);                      // CSWTrackFollower::GetPosition (RET 4)
void* __fastcall safe_pointer_assign_00447fa0(void* safe_pointer, void*, void* object);          // SafePointer<CAurObject>::operator=
w4::U32 __fastcall gui_load_resref_is_valid_00406030(void* ref, void*);                          // CResRef::IsValid
void* __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out);                       // CResRef::CopyToString
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag); // CVirtualMachine::RunScript
// ---- defined here
void __fastcall kotor_fastcall_006714e0(void* self, void*);
void* __fastcall kotor_fastcall_00671670(void* self, void*, void* camera);
void* __fastcall kotor_fastcall_005edaf0(void* app, void*);
void* __fastcall kotor_fastcall_0066bf30(void* array, void*, w4::U32 id, w4::U32 type);
void __fastcall kotor_fastcall_006701c0(void* follower, void*);
void* __fastcall kotor_fastcall_0066d260(void* follower, void*, w4::I32 index);
float __fastcall kotor_fastcall_0066c870(void* follower, void*, float speed);
void __cdecl kotor_cdecl_0066fe50(void* object, const char* event, void* follower);
void __cdecl kotor_cdecl_0066dd60(void* object, const char* event, void* key);
}

namespace gchar_mg { struct KeyInfo { unsigned char b[36]; }; }                                   // SWMGKeyInfo (24h bytes)
static_assert(sizeof(gchar_mg::KeyInfo) == 36, "SWMGKeyInfo");
extern "C" {
void __fastcall kotor_fastcall_0066dd10(void* list, void*, gchar_mg::KeyInfo key);
void __fastcall kotor_fastcall_0066d190(void* list, void*, w4::I32 capacity);
}

using namespace gchar;
using gchar_mg::KeyInfo;

namespace {
const char k_any_00743890[] = "*";                                                               // 00743890
const char k_report_key_00743268[] = "ReportKey";                                                // 00743268
const char k_ready_00752490[] = "Ready_01";                                                      // 00752490
const char k_track_00741e88[] = "track";                                                         // 00741E88
const char k_camerahook_0074f42c[] = "camerahook";                                               // 0074F42C
const char k_custom_00752738[] = "custom_";                                                      // 00752738
const char k_loop_00742944[] = "loop";                                                           // 00742944
constexpr U32 k_hundredth_0073dfec = 0x3C23D70Au;                                                // .rdata 0073DFEC (0.01f)
constexpr U32 k_key_range = 0x461C3C00u;                                                         // 10000.0f

// 0070AD18 __strnicmp with the default ("C") locale (___ascii_strnicmp 0070B920): byte-wise, A-Z folded to a-z.
int ascii_strnicmp(const char* a, const char* b, unsigned count) {
    if (count == 0) return 0;
    int ca, cb;
    do {
        ca = static_cast<unsigned char>(*a++); cb = static_cast<unsigned char>(*b++);
        if (ca >= 'A' && ca <= 'Z') ca += 0x20;
        if (cb >= 'A' && cb <= 'Z') cb += 0x20;
    } while (--count != 0 && ca != 0 && ca == cb);
    return ca - cb;
}
// model vtable +28h: RegisterKeyCallback(fn, name, data, float range, int)
inline void register_key(void* model, const void* callback, const char* name, void* data) {
    vcall<void, const void*, const char*, void*, U32, U32>(model, 0x28, callback, name, data, k_key_range, 0);
}
// model vtable +18h: PlayAnimation(name, float speed, int flags, int)
inline void play_animation(void* model, const char* name, float speed, U32 flags) {
    vcall<void, const char*, float, U32, U32>(model, 0x18, name, speed, flags, 0);
}
inline float hundredth(float v) { return static_cast<float>(static_cast<double>(v) * static_cast<double>(bits_f(k_hundredth_0073dfec))); }
void run_script_ref(void* ref, void* object) {
    Str script; script.empty();                                                                // CExoString()
    c2da_ref_to_string_00405f70(ref, nullptr, script.p());                                     // CResRef::CopyToString
    frame_R4_005d0fc0(pt(frame_control::message_manager_007a3a00), nullptr, static_cast<unsigned*>(script.p()), lw(object, 8), 1); // RunScript(script, [obj+8], 1)
}
}  // namespace

extern "C" {
// 005EDAF0 CClientExoApp::GetMGOArray: return [[this+4]].
void* __fastcall kotor_fastcall_005edaf0(void* app, void*) {
    return lp(lp(app, 4), 0);                                                                  // 005EDAF0..005EDAF3
}

// 0066BF30 CSWMiniGameObjectArray::GetMiniGameObject(ulong id, BYTE type) RET 8. Jump table 0066BF94.
void* __fastcall kotor_fastcall_0066bf30(void* array, void*, w4::U32 id, w4::U32 type) {
    if (id >= 0xFF) return nullptr;                                                            // 0066BF30..0066BF39
    void* object = lp(array, id * 4u + 4u);                                                    // 0066BF3B
    if (object == nullptr) return nullptr;                                                     // 0066BF3F..0066BF41
    switch (static_cast<U8>(type)) {                                                           // 0066BF43..0066BF4D
    case 0: return object;                                                                     // 0066BF54
    case 1: return vcall<void*>(object, 0x18);                                                 // 0066BF69
    case 2: return vcall<void*>(object, 0x1C);                                                 // 0066BF71
    case 3: return vcall<void*>(object, 0x20);                                                 // 0066BF61
    case 4: return vcall<void*>(object, 0x14);                                                 // 0066BF59
    case 5: {                                                                                  // 0066BF79
        void* p = vcall<void*>(object, 0x1C);
        if (p == nullptr || lb(p, 0x1A0) == 0) return nullptr;                                 // 0066BF7E..0066BF8A
        return p;
    }
    default: return nullptr;                                                                   // 0066BF8C
    }
}

// 0066D260 CSWTrackFollower::GetModel(int index) RET 4.
void* __fastcall kotor_fastcall_0066d260(void* follower, void*, w4::I32 index) {
    if (index >= li(follower, 0x6C)) return nullptr;                                           // 0066D260..0066D26B (jl)
    return lp(lp(follower, 0x68), static_cast<U32>(index) * 8u);                               // 0066D26E..0066D271
}

// 0066C870 CSWTrackFollower::SetSpeed(float speed) RET 4: replay "track" at speed*0.01 when running; returns [+98h].
float __fastcall kotor_fastcall_0066c870(void* follower, void*, float speed) {
    void* model = lp(follower, 0x64);                                                          // 0066C877
    sf(follower, 0x98, speed);                                                                 // 0066C87C
    if (model != nullptr && (lb(follower, 0x88) & 1) != 0) {                                   // 0066C87A..0066C88B
        const U32 flags = (li(follower, 0x80) - 1 != 0 ? 3u : 0u) + 0x11u;                     // 0066C88D..0066C8A9 (neg/sbb/and 3)
        play_animation(model, k_track_00741e88, hundredth(speed), flags);                      // 0066C893..0066C8B6
    }
    return lf(follower, 0x98);                                                                 // 0066C8B9 fld
}

// 0066D190 CExoArrayList<SWMGKeyInfo>::Allocate(int capacity) RET 4.
void __fastcall kotor_fastcall_0066d190(void* list, void*, w4::I32 capacity) {
    U8* old = static_cast<U8*>(lp(list, 0));                                                   // 0066D19E [esp+10]
    sw(list, 8, static_cast<U32>(capacity));                                                   // 0066D1A8
    U8* data = static_cast<U8*>(live_operator_new(static_cast<U32>(capacity) * 36u));           // 0066D19A..0066D1AB
    if (data != nullptr) {                                                                     // 0066D1B5..0066D1B7
        for (I32 i = 0; i < capacity; ++i) {                                                   // 0066D1B9..0066D1E8 (element ctor)
            U8* e = data + static_cast<U32>(i) * 36u;
            sw(e, 4, 0); sw(e, 8, 0); sw(e, 0xC, 0);                                           // position
            sw(e, 0x1C, 0); sw(e, 0x18, 0); sw(e, 0x14, 0); sw(e, 0x10, 0x3F800000u);          // orientation (1,0,0,0)
        }
    }
    sw(list, 0, ad(data));                                                                     // 0066D1EE
    for (I32 i = 0; i < li(list, 4); ++i)                                                      // 0066D1F0..0066D21B (size re-read)
        std::memcpy(static_cast<U8*>(lp(list, 0)) + static_cast<U32>(i) * 36u, old + static_cast<U32>(i) * 36u, 36);
    if (old != nullptr) live_free(old);                                                        // 0066D21D..0066D222
}

// 0066DD10 CExoArrayList<SWMGKeyInfo>::Add(SWMGKeyInfo key) RET 24h.
void __fastcall kotor_fastcall_0066dd10(void* list, void*, KeyInfo key) {
    if (lw(list, 4) == lw(list, 8))                                                            // 0066DD13..0066DD1B
        kotor_fastcall_0066d190(list, nullptr, lw(list, 8) != 0 ? static_cast<I32>(lw(list, 8) * 2u) : 0x10); // 0066DD1D..0066DD2D
    std::memcpy(static_cast<U8*>(lp(list, 0)) + lw(list, 4) * 36u, key.b, 36);                 // 0066DD32..0066DD46
    sw(list, 4, lw(list, 4) + 1u);                                                             // 0066DD48..0066DD4E
}

// 0066DD60 OnReportKey(object, event, SWMGKeyInfo* key) (cdecl): record the key on the player's track follower.
void __cdecl kotor_cdecl_0066dd60(void*, const char*, void* key) {
    void* game = kotor_m15e_client_get_mini_game_005edb00(client_app(), nullptr);              // 0066DD60..0066DD6E
    if (lb(game, 0x94) == 0) return;                                                           // 0066DD73..0066DD7B
    void* position = lp(key, 4);                                                               // 0066DD85 esi
    void* player = lp(game, 0x24);                                                             // 0066DD88 ebp
    void* orientation = lp(key, 8);                                                            // 0066DD96
    KeyInfo info;                                                                              // [esp+0C] (bytes 22h..23h not written)
    sw(info.b, 0, lw(key, 0));                                                                 // 0066DD8B..0066DD9B
    sw(info.b, 4, 0); sw(info.b, 8, 0); sw(info.b, 0xC, 0);                                    // 0066DDA4..0066DDB4
    sw(info.b, 0x1C, 0); sw(info.b, 0x18, 0); sw(info.b, 0x14, 0); sw(info.b, 0x10, 0x3F800000u); // 0066DDBC..0066DDD4
    info.b[0x20] = position != nullptr ? 1 : 0;                                                // 0066DD8D..0066DD92
    info.b[0x21] = orientation != nullptr ? 1 : 0;                                             // 0066DD99..0066DDDC
    if (orientation != nullptr) std::memcpy(info.b + 0x10, orientation, 16);                   // 0066DDE2..0066DDF9
    if (position != nullptr) std::memcpy(info.b + 4, position, 12);                            // 0066DE01..0066DE11
    void* list = at(player, lb(key, 0xC) != 0 ? 0x1A0u : 0x1ACu);                              // 0066DE15..0066DE32
    kotor_fastcall_0066dd10(list, nullptr, info);                                              // 0066DE18..0066DE38
}

// 0066FE50 OnAnimKeyEvent(object, event, follower) (cdecl): "custom_<name>" / "loop" animation keys.
void __cdecl kotor_cdecl_0066fe50(void* object, const char* event, void* follower) {
    if (ascii_strnicmp(event, k_custom_00752738, 7) == 0) {                                    // 0066FE6A..0066FE80
        menu_resources::Ref script;                                                            // [esp+18]
        vcall<void*, void*, U32>(follower, 0x0C, &script, 3);                                  // 0066FE86..0066FE98 GetScript(3)
        Str name(event + 7);                                                                   // 0066FE95..0066FEA0 [esp+10] state 0
        safe_pointer_assign_00447fa0(at(follower, 0x18), nullptr, object);                     // 0066FEA5..0066FEB5
        async_assign_string_005e5c50(at(follower, 0x10), nullptr, name.p());                   // 0066FEBA..0066FEC2
        if (gui_load_resref_is_valid_00406030(&script, nullptr) != 0)                          // 0066FEC7..0066FED2
            run_script_ref(&script, follower);                                                 // 0066FED4..0066FF0F
        return;                                                                                // 0066FF14..0066FFCE ~name
    }
    if (ascii_strnicmp(event, k_loop_00742944, 4) != 0) return;                                // 0066FF1D..0066FF2F
    void* track = vcall<void*>(follower, 0x14);                                                // 0066FF35..0066FF3B
    if (track == nullptr) return;                                                              // 0066FF3E..0066FF42
    if (li(track, 0x80) != -1) {                                                               // 0066FF48..0066FF51
        vcall<void, I32>(track, 0x38, li(track, 0x80) - 1);                                    // 0066FF53..0066FF59
        kotor_fastcall_0066c870(track, nullptr, lf(track, 0x98));                              // 0066FF5C..0066FF70 SetSpeed (ST0 discarded)
    }
    menu_resources::Ref script;                                                                // [esp+28]
    vcall<void*, void*, U32>(track, 0x0C, &script, 9);                                         // 0066FF72..0066FF7D GetScript(9)
    if (gui_load_resref_is_valid_00406030(&script, nullptr) != 0)                              // 0066FF80..0066FF8B
        run_script_ref(&script, track);                                                        // 0066FF8D..0066FFCE
}

// 006701C0 CSWTrackFollower::Go() RET 0: start the track animation once.
void __fastcall kotor_fastcall_006701c0(void* follower, void*) {
    if (lp(follower, 0x64) == nullptr) return;                                                 // 006701C6..006701CB
    if ((lb(follower, 0x88) & 1) != 0) return;                                                 // 006701D1..006701D8
    U32 flags = li(follower, 0x80) == 1 ? 5u : 4u;                                             // 006701DE..006701F5 [esp+10]
    const I32 count = li(follower, 0x6C);                                                      // 006701E6 ebx
    for (I32 i = 0; i < count; ++i) {                                                          // 006701FD..0067023A
        void* model = lp(lp(follower, 0x68), static_cast<U32>(i) * 8u);                        // 00670203..00670206
        register_key(model, reinterpret_cast<const void*>(&kotor_cdecl_0066fe50), k_any_00743890, follower); // 00670209..0067021F
        play_animation(model, k_ready_00752490, 1.0f, 0);                                      // 00670222..00670234
    }
    if (vcall<U32>(follower, 0x18) != 0) {                                                     // 0067023C..00670245
        register_key(lp(follower, 0x64), reinterpret_cast<const void*>(&kotor_cdecl_0066dd60), k_report_key_00743268, nullptr); // 00670247..0067025F
        flags |= 0x100u;                                                                       // 00670262
    }
    register_key(lp(follower, 0x64), reinterpret_cast<const void*>(&kotor_cdecl_0066fe50), k_any_00743890, follower); // 0067026A..00670281
    play_animation(lp(follower, 0x64), k_track_00741e88, hundredth(lf(follower, 0x98)), flags); // 00670284..006702A5
    sw(follower, 0x88, lw(follower, 0x88) | 1u);                                               // 006702A8
    void* sound = lp(follower, 0x144);                                                         // 006702AF
    if (sound != nullptr && kotor_fastcall_005d5920(sound, nullptr) == 0) {                    // 006702B5..006702C2
        unsigned char position[12];
        void* p = kotor_fastcall_0066d5d0(follower, nullptr, position);                        // 006702C4..006702CC
        kotor_m15_sound_source_set_position_005d59e0(sound, nullptr, p, 0);                    // 006702D1..006702D4 (pre-pushed 0)
        kotor_fastcall_005d5930(sound, nullptr);                                               // 006702D9..006702DB Play
    }
}

// 006714E0 CSWMiniGame::Go() RET 0.
void __fastcall kotor_fastcall_006714e0(void* self, void*) {
    void* objects = kotor_fastcall_005edaf0(client_app(), nullptr);                            // 006714E0..006714ED GetMGOArray
    if (void* player = lp(self, 0x24)) kotor_fastcall_006701c0(player, nullptr);               // 006714F2..006714FB
    for (I32 i = li(self, 0x30) - 1; i >= 0; --i) {                                            // 00671500..00671547
        void* follower = kotor_fastcall_0066bf30(objects, nullptr, lw(lp(self, 0x2C), static_cast<U32>(i) * 4u), 2); // 00671506..00671511
        if (follower != nullptr) {                                                             // 00671516..00671518
            kotor_fastcall_006701c0(follower, nullptr);                                        // 0067151A..0067151C
        } else {
            sw(self, 0x30, lw(self, 0x30) - 1u);                                               // 00671523..0067152D
            for (I32 j = i; j < li(self, 0x30); ++j)                                           // 00671529..00671544 (remove)
                sw(lp(self, 0x2C), static_cast<U32>(j) * 4u, lw(lp(self, 0x2C), static_cast<U32>(j) * 4u + 4u));
        }
    }
    if (void* music = lp(self, 0x1C))                                                          // 00671549..0067154E
        menu_tables_required::StreamPlay_005d5b10(music, nullptr, static_cast<menu_resources::Ref*>(at(self, 0x96)), 8, 0, 1, 0); // 00671550..0067155F
}

// 00671670 CSWMiniGame::SetCamera(CAurCamera* camera) RET 4: attach the camera to the player's camera hook.
void* __fastcall kotor_fastcall_00671670(void* self, void*, void* camera) {
    sw(self, 0x20, ad(camera));                                                                // 00671691
    void* player = lp(self, 0x24);
    if (camera != nullptr && player != nullptr && lp(player, 0x64) != nullptr) {               // 00671694..006716AA
        const I32 mode = li(self, 0x80);                                                       // 006716B0
        if (mode == 2) {                                                                       // 006716B9..006716D3
            vcall<void, void*, const char*, U32>(camera, 0x74, lp(player, 0x220), k_camerahook_0074f42c, 0);
        } else if (mode == 1) {                                                                // 006716D8
            const I32 count = li(player, 0x6C);                                                // 006716D8..006716E1 [esp+20]
            void* hook = nullptr;                                                              // ebp
            void* model = pt(static_cast<U32>(count));                                         // edi (stale count until the first model)
            for (I32 i = 0; i < count && hook == nullptr; ++i) {                               // 006716F0..0067171B
                model = kotor_fastcall_0066d260(lp(self, 0x24), nullptr, i);                   // 006716F4..006716FD
                if (model != nullptr) hook = vcall<void*, const char*>(model, 0x10C, k_camerahook_0074f42c); // 006716FF..00671712
            }
            void* attach = nullptr;
            bool use = false;
            if (hook != nullptr) {                                                             // 0067171D..0067171F
                use = true;
                void* memory = live_operator_new(0x3C);                                        // 00671721..00671723
                if (memory != nullptr) {                                                       // 00671731..00671739
                    auto guard = on_throw([&]() { live_free(memory); });                       // EH state 0
                    void* gob = vcall<void*>(lp(self, 0x20), 0x04);                            // 0067173B..00671742
                    attach = behavior_attach_0049fb10(memory, nullptr, gob, model, hook);      // 00671745..00671748
                    guard.armed = false;
                }
            } else {
                void* main = lp(lp(self, 0x24), 0x64);                                         // 0067174F..00671752 edi
                if (main != nullptr) {                                                         // 00671755..00671757
                    void* main_hook = vcall<void*, const char*>(main, 0x10C, k_camerahook_0074f42c); // 00671759..00671762 ebx
                    if (main_hook != nullptr) {                                                // 0067176A..0067176C
                        use = true;
                        void* memory = live_operator_new(0x3C);                                // 0067176E..00671770
                        if (memory != nullptr) {                                               // 0067177E..00671788
                            auto guard = on_throw([&]() { live_free(memory); });               // EH state 1
                            void* gob = vcall<void*>(lp(self, 0x20), 0x04);                    // 0067178A..00671791
                            attach = behavior_attach_0049fb10(memory, nullptr, gob, main, main_hook); // 00671794..00671797
                            guard.armed = false;
                        }
                    }
                }
            }
            if (use) vcall<void, void*>(lp(self, 0x20), 0x7C, attach);                         // 006717A0..006717AE
        }
        vcall<void, U32, U32>(lp(self, 0x20), 0x40, lw(self, 0x68), lw(self, 0x6C));           // 006717B1..006717BE
        vcall<void, U32>(lp(self, 0x20), 0x44, lw(self, 0x70));                                // 006717C1..006717CA
    }
    return lp(self, 0x20);                                                                     // 006717CD
}
}  // extern "C"
