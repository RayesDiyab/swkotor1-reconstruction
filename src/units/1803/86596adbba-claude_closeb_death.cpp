// claude_closeB_death.cpp (lane worker closeB): the client death sequence behind the 0886 gate
// NativePartyUpdate::all_down_005edc40 (server party update: every party member down -> RunDeathSequence), transcribed
// from the original swkotor.exe (read-only):
//   005EDC40 CClientExoApp::RunDeathSequence()               [ecx+4]; jmp 005F7200   RET 0  kotor_fastcall_005edc40
//   005F7200 CClientExoAppInternal::RunDeathSequence()       (Ghidra: ShutDownMoviePlayer+70h)  RET 0  kotor_fastcall_005f7200
//   0040C120 CSWGuiManager::HideAllPanels()                                          RET 0  kotor_fastcall_0040c120
//   CSWCameraDeath (40h bytes, CAurBehavior-derived, vtable 0075169C -> k_camera_death_vtable_0075169c):
//   0063BBD0 CSWCameraDeath(CAurObject* owner, CAurObject* target)                    RET 8  kotor_fastcall_0063bbd0
//   0063A730 ~CSWCameraDeath()                                                        RET 0  kotor_fastcall_0063a730
//   0063BC90 scalar deleting destructor(flags)                                        RET 4  kotor_fastcall_0063bc90
//   0063A760 GetType() -> 106Fh                                                       RET 0  kotor_fastcall_0063a760
//   0063A770 SetTarget(CAurObject*)                                                   RET 4  kotor_fastcall_0063a770
//   0063A810 Control(float delta) -> 0/1                                              RET 4  kotor_fastcall_0063a810
//   00638540 Message(char const*)                                                     RET 4  kotor_fastcall_00638540
// Conventions as claude_dialogue_camera.cpp: thiscall -> extern "C" __fastcall(self, unused_edx, ...); PC_53 x87 ->
// double intermediates, every `fst/fstp dword` an explicit float rounding; game heap only. EH: the unwind states only
// guard operator-new results / the SafePointer local during calls that do not throw, so they are not reproduced.
#include "1289b1b830-claude_dialogue_camera.hpp"

using namespace dlg;

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned size); // CExoArrayList::Allocate
}

extern "C" {
// ---------------- existing providers (c35 map) ----------------
void* __fastcall kotor_fastcall_0049f110(void* behavior, void*, void* owner);           // CAurBehavior::CAurBehavior
void __fastcall behavior_destruct_0049fa70(void* behavior, void*);                       // CAurBehavior::~CAurBehavior
void __fastcall behavior_void_return_004015a0(void*, void*);                             // vtable slot 1 (ret)
U __fastcall behavior_return_zero_0063e7f0(void*, void*);                                // vtable slots 5..7
U __fastcall behavior_control_0049f270(void* behavior, void*, U delta);                  // CAurBehavior::Control
void __fastcall behavior_add_dependency_0049f130(void* behavior, void*, void* object);   // CAurBehavior::AddDependency
void __fastcall kotor_fastcall_0049f1f0(void* behavior, void*, void* object);            // CAurBehavior::RemoveDependency
void* __fastcall safe_pointer_assign_00447fa0(void* safe_pointer, void*, void* object);  // SafePointer<CAurObject>::=
U __fastcall kotor_fastcall_00445080(void* list, void*, void* item);                     // CExoArrayList::Remove
void __fastcall int_list_add_00444fe0(void* list, void*, U value);                       // CExoArrayList<int>::Add
void* __fastcall aurora_hit_info_construct_004578f0(void* hit, void*, U a, U b, U c);   // CHitInfo::CHitInfo
void __fastcall aurora_hit_info_clear_output_00456a60(void* hit, void*);                 // CHitInfo::ClearOutput
void* __fastcall quaternion_zdir_0043e250(const void* quaternion, void*, void* out);     // Quaternion::zdir -> out
const char* __cdecl firstword_00463530(const char* text);                                // firstword
int __cdecl parse_float_004635c0(const char* text, float* out);                          // Parse_float
int __cdecl kotor_stricmp_0070acaf(const char* a, const char* b);                        // __stricmp
void __fastcall kotor_m15_panel_set_visible_0040b9d0(void* panel, void*, U visible);     // CSWGuiPanel::SetVisible
void __fastcall frame_R4_00627260(void* box, void*);                                     // CSWGuiMessageBox::DisplayDeathMessage
// 004D8230 GetGameObject and 0062ABF0 StartGlobalFade are declared by claude_dialogue_common.hpp

// ---------------- bodies defined here ----------------
void __fastcall kotor_fastcall_005edc40(void* client, void*);
void __fastcall kotor_fastcall_005f7200(void* internal, void*);
void __fastcall kotor_fastcall_0040c120(void* gui_manager, void*);
void* __fastcall kotor_fastcall_0063bbd0(void* self, void*, void* owner, void* target);
void __fastcall kotor_fastcall_0063a730(void* self, void*);
void* __fastcall kotor_fastcall_0063bc90(void* self, void*, U flags);
U __fastcall kotor_fastcall_0063a760(void* self, void*);
void __fastcall kotor_fastcall_0063a770(void* self, void*, void* target);
U __fastcall kotor_fastcall_0063a810(void* self, void*, U delta);
void __fastcall kotor_fastcall_00638540(void* self, void*, const char* message);
extern const void* const k_camera_death_vtable_0075169c[8];
}

namespace {
constexpr U k360 = 0x43b40000u;       // [00741868] 360.0f
constexpr U kZero = 0;                // [0073D700] 0.0f
constexpr U kNinety = 0x42b40000u;    // [0073E40C] 90.0f
constexpr U kQuarter = 0x3e800000u;   // [0073EDF0] 0.25f
constexpr U8 kTypeCreature = 5;       // [00746634] (byte 05)
}

extern "C" {

// ===================================================================================== 005EDC40
void __fastcall kotor_fastcall_005edc40(void* client, void*) {
    kotor_fastcall_005f7200(rp(client, 4), nullptr);                                       // 005EDC40..005EDC43
}

// ===================================================================================== 0040C120
void __fastcall kotor_fastcall_0040c120(void* gui_manager, void*) {
    if (static_cast<I>(rd(gui_manager, 0x8c)) <= 0) return;                                // 0040C124..0040C12E
    I i = 0;
    do {
        kotor_m15_panel_set_visible_0040b9d0(rp(rp(gui_manager, 0x88), static_cast<U>(i) * 4), nullptr, 0); // 0040C130..0040C13B
        ++i;                                                                               // 0040C146
    } while (i < static_cast<I>(rd(gui_manager, 0x8c)));                                   // 0040C140..0040C149
}

// ===================================================================================== 005F7200
void __fastcall kotor_fastcall_005f7200(void* internal, void*) {
    void* const s = internal;
    void* const gui_manager = rp(s, 0x274);                                                // 005F721C
    wr(s, 0x2c0, 1);                                                                       // 005F7227
    wr(s, 0x2c4, 1);                                                                       // 005F722D
    kotor_fastcall_0040c120(gui_manager, nullptr);                                         // 005F7233
    frame_R4_00627260(rp(rp(s, 0x40), 0x98), nullptr);                                     // 005F7238..005F7241
    void* object = nullptr;                                                                // [esp+8]
    void* creature = nullptr;                                                              // ebx
    if (kotor_fastcall_004d8230(rp(s, 0x14), nullptr, rd(s, 0x2d4), &object) == kStatusOk &&   // 005F7246..005F7268
        object != nullptr && rd8(object, 8) == kTypeCreature)                              // 005F726A..005F727B
        creature = vcall<void*>(object, 0x2c);                                             // 005F727D..005F7282
    void* const module = rp(s, 0x18);                                                      // 005F7288
    if (module != nullptr && creature != nullptr) {                                        // 005F728B..005F7291
        void* const cameras = rp(module, 0x40);                                            // 005F7294
        if (cameras != nullptr) {                                                          // 005F7297..005F7299
            void* const memory = live_operator_new(0x40);                                  // 005F729C..005F72A5
            void* camera = nullptr;                                                        // 005F72AC xor eax, eax
            if (memory != nullptr) {                                                       // 005F72AE..005F72B4
                void* const target = vcall<void*>(creature, 0xd0);                         // 005F72B6..005F72BA
                void* const owner = vcall<void*>(cameras, 0x4);                            // 005F72C0..005F72C5
                camera = kotor_fastcall_0063bbd0(memory, nullptr, owner, target);          // 005F72C2..005F72CB
            }
            vcall<void>(cameras, 0x7c, camera);                                            // 005F72D0..005F72DD
        }
    }
    const U color[3] = {0, 0, 0};                                                          // [esp+10h..18h] 005F72F6..005F7306
    kotor_m15_ingame_start_global_fade_0062abf0(rp(s, 0x40), nullptr, 0, 12.0f, 1.0f, color); // 005F72E2..005F730E
                                                                                           // (push 0 / 41400000h / 3F800000h)
}

// ===================================================================================== CSWCameraDeath
U __fastcall kotor_fastcall_0063a760(void*, void*) { return 0x106f; }                     // 0063A760

void __fastcall kotor_fastcall_0063a730(void* self, void*) {
    wp(self, 0, k_camera_death_vtable_0075169c);                                           // 0063A736
    if (rd(self, 0x14) != 0)                                                               // 0063A73C..0063A740
        (void)kotor_fastcall_00445080(at(rp(self, 0x14), 4), nullptr, at(self, 0x14));    // 0063A742..0063A746
    behavior_destruct_0049fa70(self, nullptr);                                             // 0063A74B..0063A74E (tail jmp)
}

void* __fastcall kotor_fastcall_0063bc90(void* self, void*, U flags) {
    kotor_fastcall_0063a730(self, nullptr);                                                // 0063BC93
    if (flags & 1) live_free(self);                                                        // 0063BC98..0063BCA0
    return self;                                                                           // 0063BCA8
}

void __fastcall kotor_fastcall_0063a770(void* self, void*, void* target) {
    if (rd(self, 0x14) != 0) kotor_fastcall_0049f1f0(self, nullptr, rp(self, 0x14));       // 0063A778..0063A783
    if (target != nullptr) {                                                               // 0063A788..0063A78E
        behavior_add_dependency_0049f130(self, nullptr, target);                           // 0063A791..0063A794
        U o_buf[3], t_buf[3];                                                              // [esp+28h], [esp+34h]
        const U* const o = vcall<const U*>(rp(self, 0x10), 0x64, o_buf);                   // 0063A799..0063A7A3 (edi)
        const U* const t = vcall<const U*>(target, 0x64, t_buf);                           // 0063A7A6..0063A7B1
        U d[3];                                                                            // [esp+10h]
        const double dz = fl(t[2]) - fl(o[2]);                                             // 0063A7B4..0063A7B7
        const double dy = fl(t[1]) - fl(o[1]);                                             // 0063A7BE..0063A7C2
        const double dx = fl(t[0]) - fl(o[0]);                                             // 0063A7C5..0063A7CB
        d[0] = fs(dx); d[1] = fs(dy); d[2] = fs(dz);                                       // 0063A7CE..0063A7D6
        U n[3];                                                                            // [esp+1Ch]
        (void)normalize_004ab180(n, d);                                                    // 0063A7BA..0063A7DA
        wr(self, 0x18, fs(kotor_m15a_yaw_004aa0f0(n)));                                    // 0063A7DF..0063A7E9
        wr(self, 0x34, 0x3f400000u);                                                       // 0063A7EF (0.75f)
    }
    (void)safe_pointer_assign_00447fa0(at(self, 0x14), nullptr, target);                   // 0063A7F7..0063A7FA
}

void* __fastcall kotor_fastcall_0063bbd0(void* self, void*, void* owner, void* target) {
    (void)kotor_fastcall_0049f110(self, nullptr, owner);                                   // 0063BBEE..0063BBF3
    wp(self, 0, k_camera_death_vtable_0075169c);                                           // 0063BBFB
    wr(self, 0x14, 0);                                                                     // 0063BC09
    wr(self, 0x18, 0);                                                                     // 0063BC19
    wr(self, 0x28, 0x3f000000u);                                                           // 0063BC20 0.5
    wr(self, 0x1c, 0x41f00000u);                                                           // 0063BC23 30.0
    wr(self, 0x2c, 0x3c23d70au);                                                           // 0063BC2A 0.01
    wr(self, 0x20, 0x42b40000u);                                                           // 0063BC2D 90.0
    wr(self, 0x30, 0x3f000000u);                                                           // 0063BC34 0.5
    wr(self, 0x24, 0x40400000u);                                                           // 0063BC37 3.0
    wr(self, 0x34, 0x3f400000u);                                                           // 0063BC3E 0.75
    wr(self, 0x38, 0);                                                                     // 0063BC45
    wr(self, 0x3c, 0x3c23d70au);                                                           // 0063BC4C 0.01
    if (rd(self, 0x14) != 0)                                                               // 0063BC4F..0063BC58 (dead: just cleared)
        (void)kotor_fastcall_00445080(at(rp(self, 0x14), 4), nullptr, at(self, 0x14));    // 0063BC5A..0063BC5E
    wr(self, 0x14, 0);                                                                     // 0063BC6A
    kotor_fastcall_0063a770(self, nullptr, target);                                        // 0063BC63..0063BC70
    return self;                                                                           // 0063BC7A
}

void __fastcall kotor_fastcall_00638540(void* self, void*, const char* message) {
    struct Field { const char* name; U skip; U offset; };
    static const Field fields[7] = {
        {"m_yaw", 5, 0x18}, {"m_speedYaw", 0xa, 0x1c}, {"m_desiredPitch", 0xe, 0x20},       // 00638546..006385BD
        {"m_desiredDistance", 0x11, 0x24}, {"m_logYaw", 8, 0x28}, {"m_logPitch", 0xa, 0x2c}, // 006385C0..00638635
        {"m_logDistance", 0xd, 0x30}};                                                     // 00638638..0063865D
    for (const Field& f : fields)
        if (kotor_stricmp_0070acaf(f.name, firstword_00463530(message)) == 0)
            (void)parse_float_004635c0(message + f.skip, reinterpret_cast<float*>(at(self, f.offset)));
}

// 0063A810 Control(float delta). The frame is modelled as the original [esp+X] slots (post-prologue esp).
U __fastcall kotor_fastcall_0063a810(void* self, void*, U delta) {
    alignas(4) U q[0xd8 / 4] = {};
    auto F = [&q](U off) -> U& { return q[off / 4]; };
    auto L = [&q](U off) -> double { return fl(q[off / 4]); };
    auto S = [&q](U off, double v) { q[off / 4] = fs(v); };
    unsigned char* const hit = reinterpret_cast<unsigned char*>(&q[0x74 / 4]);            // [esp+74h] CHitInfo
    // [esp+28h] SafePointer<CAurObject> local on the owner (inline constructor: register &local in owner+4)
    volatile U safe = rd(self, 0x10);                                                      // 0063A831..0063A836
    if (safe != 0) {                                                                       // 0063A834..0063A83A
        void* const list = at(ptr(safe), 4);                                               // 0063A842
        if (rd(list, 4) == rd(list, 8)) {                                                  // 0063A83C..0063A847
            const U allocated = rd(list, 8);
            menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                                allocated ? allocated + allocated : 0x10u);  // 0063A849..0063A859
        }
        wr(rp(list, 0), rd(list, 4) * 4, word(const_cast<U*>(&safe)));                      // 0063A85E..0063A867
        wr(list, 4, rd(list, 4) + 1);                                                      // 0063A86A
    }
    auto release = [&]() {                                                                 // inline ~SafePointer
        if (safe != 0) (void)kotor_fastcall_00445080(at(ptr(safe), 4), nullptr, const_cast<U*>(&safe));
    };
    if (behavior_control_0049f270(self, nullptr, delta) == 0) {                            // 0063A86D..0063A889
        release();                                                                         // 0063A88B..0063A89B
        return 0;                                                                          // 0063A8A0
    }
    if (vcall<void*>(ptr(safe), 0x108, 0xffffffffu) != self) {                             // 0063A8A7..0063A8B7
        release();                                                                         // 0063A8B9..0063AD13
        return 1;
    }
    if (rd(self, 0x14) == 0) { release(); return 1; }                                      // 0063A8CF..0063A8D6 -> 0063AD03
    // blend the distance offset and advance the yaw by speed * delta, wrapped into [0, 360)
    wr(self, 0x34, fs((fl(rd(self, 0x38)) - fl(rd(self, 0x34))) * fl(rd(self, 0x3c)) + fl(rd(self, 0x34)))); // 0063A8DC..0063A8E8
    {
        const double yaw = fl(delta) * fl(rd(self, 0x1c)) + fl(rd(self, 0x18));            // 0063A8EB..0063A8F5
        const U8 ah = fcom(yaw, fl(kZero));                                                // 0063A8F8..0063A901
        wr(self, 0x18, fs(yaw));                                                           // 0063A8FE fst
        if (!jp(ah, 0x05)) {
            wr(self, 0x18, fs(yaw + fl(k360)));                                            // 0063A908..0063A90E
        } else if (!jne(fcom(yaw, fl(k360)), 0x01)) {
            wr(self, 0x18, fs(yaw - fl(k360)));                                            // 0063A913..0063A926
        }                                                                                  // 0063A92B fstp st(0)
    }
    // current owner yaw, unwound to the turn just below the desired yaw
    {
        const void* const orientation = vcall<const void*>(rp(self, 0x10), 0x68, &F(0x64)); // 0063A92D..0063A937
        double st = quaternion_yaw_004a9f40(orientation);                                  // 0063A93A..0063A93B
        F(0x10) = fs(st);                                                                  // 0063A940 fst [esp+14h] (pre-pop)
        st = st + fl(k360);                                                                // 0063A944
        if (!jp(fcom(st, fl(rd(self, 0x18))), 0x05)) {                                     // 0063A94D..0063A955
            F(0x14) = rd(self, 0x18);                                                      // 0063A957..0063A95A
            do {
                F(0x10) = fs(st);                                                          // 0063A960
                st = st + fl(k360);                                                        // 0063A964
            } while (!jp(fcom(st, L(0x14)), 0x05));                                        // 0063A96A..0063A973
        }                                                                                  // 0063A975 fstp st(0)
    }
    // desired pitch (90 - +20, made positive) and the owner pitch unwound to it
    S(0x18, fl(kNinety) - fl(rd(self, 0x20)));                                             // 0063A977..0063A980
    if (!jp(fcom(fl(kNinety) - fl(rd(self, 0x20)), fl(kZero)), 0x05))                      // 0063A984..0063A98F
        S(0x18, L(0x18) + fl(k360));                                                       // 0063A991..0063A99B
    double pitch;
    {
        const void* const orientation = vcall<const void*>(rp(self, 0x10), 0x68, &F(0x64)); // 0063A99F..0063A9A9
        pitch = quaternion_pitch_004aa020(orientation);                                    // 0063A9AC..0063A9AD
        if (!jp(fcom(pitch, L(0x18)), 0x05)) {                                             // 0063A9B2..0063A9BE
            do pitch = pitch + fl(k360);                                                   // 0063A9C0
            while (!jp(fcom(pitch, L(0x18)), 0x05));                                       // 0063A9C6..0063A9CF
        }
    }
    S(0x2c, (fl(rd(self, 0x18)) - L(0x10)) * fl(rd(self, 0x28)) + L(0x10));               // 0063A9D1..0063A9E5 yaw
    S(0x14, (L(0x18) - pitch) * fl(rd(self, 0x2c)) + pitch);                               // 0063A9E9..0063A9F8 pitch
    (void)yaw_pitch_roll_004acac0(&F(0x54), bitsf(F(0x2c)), bitsf(F(0x14)), 0.0f);        // 0063A9D4..0063AA05 [esp+54h]
    // distance: blend from the current owner->target distance towards +24 by +30
    void* const target = rp(self, 0x14);                                                   // 0063AA0F (ebp)
    {
        U o_buf[3], t_buf[3];                                                              // [esp+3Ch], [esp+30h]
        const U* const o = vcall<const U*>(rp(self, 0x10), 0x64, o_buf);                   // 0063AA0A..0063AA1A (edi)
        const U* const t = vcall<const U*>(target, 0x64, t_buf);                           // 0063AA1D..0063AA29
        const double dz = fl(t[2]) - fl(o[2]);                                             // 0063AA2C..0063AA2F
        const double dy = fl(t[1]) - fl(o[1]);                                             // 0063AA36..0063AA3A
        const double dx = fl(t[0]) - fl(o[0]);                                             // 0063AA3D..0063AA3F
        S(0x1c, dx); S(0x20, dy); S(0x24, dz);                                             // 0063AA41..0063AA49
    }
    {
        const double mag = vector_magnitude_004ab100(&F(0x1c));                            // 0063AA4D
        F(0x14) = rd(self, 0x34);                                                          // 0063AA55..0063AA5F
        S(0x10, (fl(rd(self, 0x24)) - mag) * fl(rd(self, 0x30)) + mag);                    // 0063AA52..0063AA70
    }
    // focus = target position raised by the blended height (+34)
    {
        const U* const t = vcall<const U*>(target, 0x64, &F(0x1c));                        // 0063AA58..0063AA76
        const double h = L(0x14) + fl(t[2]);                                               // 0063AA79..0063AA81
        F(0x18) = fs(h);                                                                   // 0063AA89 fst
        const U ty = t[1];                                                                 // 0063AA8D
        F(0x30) = t[0];                                                                    // 0063AA90..0063AA96
        F(0x2c) = ty;                                                                      // 0063AA92 fst
        F(0x34) = ty;                                                                      // 0063AA9A
        F(0x38) = fs(h);                                                                   // 0063AA9E
    }
    // camera = focus + distance * zdir(orientation)
    {
        const U* const z = static_cast<const U*>(quaternion_zdir_0043e250(&F(0x54), nullptr, &F(0x1c))); // 0063AA85..0063AAA2
        const double kz = L(0x10) * fl(z[2]);                                              // 0063AAA7..0063AAAB
        const double ky = L(0x10) * fl(z[1]);                                              // 0063AAAE..0063AAB2
        S(0x1c, L(0x10) * fl(z[0]));                                                       // 0063AAB5..0063AABB
        S(0x10, kz + L(0x18));                                                             // 0063AAC3..0063AAD5
        F(0x48) = 0;                                                                       // 0063AAD9
        F(0x44) = F(0x10);                                                                 // 0063AAE1..0063AAE9
        F(0x4c) = 0;                                                                       // 0063AAED
        F(0x50) = 0;                                                                       // 0063AAF5
        const double cy = ky + L(0x2c);                                                    // 0063AAE5
        F(0x14) = fs(cy);                                                                  // 0063AAFD fst
        S(0x3c, L(0x1c) + L(0x30));                                                        // 0063AB01..0063AB09
        F(0x40) = fs(cy);                                                                  // 0063AB0D
    }
    (void)aurora_hit_info_construct_004578f0(hit, nullptr, 0, 0xbfffffffu, 0);             // 0063AAC1..0063AB11
    int_list_add_00444fe0(hit + 8, nullptr, rd(self, 0x14));                               // 0063AB16..0063AB2F
    aurora_hit_info_clear_output_00456a60(hit, nullptr);                                   // 0063AB34..0063AB38
    {
        U d[3];                                                                            // [esp+1Ch]
        const double dz = L(0x10) - L(0x18);                                               // 0063AB3D..0063AB41
        const double dy = L(0x14) - L(0x2c);                                               // 0063AB49..0063AB4E
        const double dx = L(0x3c) - L(0x30);                                               // 0063AB56..0063AB5B
        d[0] = fs(dx); d[1] = fs(dy); d[2] = fs(dz);                                       // 0063AB5F..0063AB67
        F(0x1c) = d[0]; F(0x20) = d[1]; F(0x24) = d[2];
        const U* const n = static_cast<const U*>(normalize_004ab180(&F(0x64), &F(0x1c)));  // 0063AB45..0063AB6B
        S(0x2c, fl(n[2]) * fl(kQuarter));                                                  // 0063AB70..0063AB7F
        S(0x18, fl(n[1]) * fl(kQuarter));                                                  // 0063AB83..0063AB8C
        S(0x1c, fl(n[0]) * fl(kQuarter));                                                  // 0063AB90..0063ABA1
    }
    // line of sight from the focus to the camera position pushed a quarter step further out
    void* const scene = vcall<void*>(rp(self, 0x10), 0x104);                               // 0063AB73..0063ABA5
    const double az = L(0x2c) + L(0x10);                                                   // 0063ABAB..0063ABB1
    const double ay = L(0x18) + L(0x14);                                                   // 0063ABB9..0063ABBE
    const double ax = L(0x1c) + L(0x3c);                                                   // 0063ABC6..0063ABCB
    S(0x1c, ax); S(0x20, ay); S(0x24, az);                                                 // 0063ABD7..0063ABF4
    const V3 far_point{F(0x1c), F(0x20), F(0x24)};
    const V3 focus{F(0x30), F(0x34), F(0x38)};                                             // 0063ABF7..0063AC0A
    const U blocked = vcall<U>(scene, 0x38, focus, far_point, &F(0x48), hit);              // 0063ABAF..0063AC0F
    F(0x14) = blocked != 0 ? 1u : 0u;                                                      // 0063AC12..0063AC18
    U px, py, pz;
    if (blocked != 0) {                                                                    // 0063AC1C
        const double nz = fl(rd(hit, 0x48)) * fl(kQuarter);                                // 0063AC1E..0063AC25
        S(0x14, fl(rd(hit, 0x44)) * fl(kQuarter));                                         // 0063AC2B..0063AC38
        S(0x1c, fl(rd(hit, 0x40)) * fl(kQuarter));                                         // 0063AC3C..0063AC49
        const double z = nz + L(0x50);                                                     // 0063AC4D
        const double y = L(0x4c) + L(0x14);                                                // 0063AC51..0063AC55
        S(0x1c, L(0x1c) + L(0x48));                                                        // 0063AC59..0063AC61
        px = F(0x1c);                                                                      // 0063AC65
        S(0x20, y); py = F(0x20);                                                          // 0063AC69..0063AC6D
        S(0x24, z); pz = F(0x24);                                                          // 0063AC71..0063AC75
    } else {
        py = F(0x40); px = F(0x3c); pz = F(0x44);                                          // 0063AC7B..0063AC83
    }
    (void)vcall<void*>(rp(self, 0x10), 0x5c, &F(0x64), V3{px, py, pz});                    // 0063AC87..0063AC9F
    const Q4 orientation{F(0x54), F(0x58), F(0x5c), F(0x60)};                              // 0063ACA5..0063ACC4
    (void)vcall<void*>(rp(self, 0x10), 0x60, &F(0x64), orientation);                       // 0063ACA2..0063ACCC
    // inline ~CHitInfo: release the +38 dependency, free the ignore-list data
    if (rd(hit, 0x38) != 0) (void)kotor_fastcall_00445080(at(rp(hit, 0x38), 4), nullptr, hit + 0x38); // 0063ACCF..0063ACED
    if (rd(hit, 8) != 0) live_free(rp(hit, 8));                                            // 0063ACF2..0063ACFB
    release();                                                                             // 0063AD03..0063AD13
    return 1;                                                                              // 0063AD18
}

}  // extern "C"

// vtable 0075169C: [0063BC90 dtor, 004015A0 void, 0063A810 Control, 00638540 Message, 0063A760 GetType,
//                   0063E7F0 x3 return_zero]
extern "C" const void* const k_camera_death_vtable_0075169c[8] = {
    reinterpret_cast<const void*>(&kotor_fastcall_0063bc90),
    reinterpret_cast<const void*>(&behavior_void_return_004015a0),
    reinterpret_cast<const void*>(&kotor_fastcall_0063a810),
    reinterpret_cast<const void*>(&kotor_fastcall_00638540),
    reinterpret_cast<const void*>(&kotor_fastcall_0063a760),
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),
};
