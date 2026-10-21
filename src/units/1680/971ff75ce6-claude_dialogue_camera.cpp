// Lane "dialogue-start", fork B2: the dialogue camera of the client dialogue runtime, transcribed from the original
// swkotor.exe (read-only). Part 1: CGuiInGame camera arms, CSWCModule camera setters and the
// CSWBehaviorCameraDialog class shell (vtable 00756AE8). Part 2 (claude_dialogue_camera2.cpp): UpdateShotCamera and
// the CSWBehaviorCameraDialog solver (Control / GetCameraParameters / GetSideUnobstructed / SetShot 006BD260).
// x87: PC_53 is assumed; every x87 intermediate is a double (SSE double == x87 at 53-bit precision for + - * /),
// every `fstp dword` is an explicit (float); an x87-returning callee is declared `double` so ST0 is kept unrounded.
#include "6bd5403233-claude_dialogue_common.hpp"
#include "1289b1b830-claude_dialogue_camera.hpp"

using namespace dlg;

namespace kotor_menu20 { String* __fastcall AssignText005e5140(String*, void*, const char*); }  // CExoString=(char*)
namespace menu_tables_m41 { void __fastcall SetCameraMode_0061b6f0(void* options, void*, unsigned mode); }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }       // [007A39E0] CExoBase*

extern "C" {
void* __fastcall kotor_fastcall_005ed700(void* client, void*);                          // CClientExoApp::GetClientOptions
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                    // CClientExoApp::GetGameObject
void __fastcall kotor_fastcall_0063ba80(void* camera, void*, void* settings);          // CSWCameraOnAStick::SaveCameraSettings
void __fastcall kotor_fastcall_0040bc70(void* gui_manager, void*, void* panel, U a, U b); // CSWGuiManager::AddPanel
U __fastcall frame_RemovePanel_0040c830(void* gui_manager, void*, void* panel);         // CSWGuiManager::RemovePanel
void __fastcall kotor_fastcall_005edf10(void* client, void*, I effect);                  // CClientExoApp::EnableVideoEffect
unsigned long long __fastcall recovered_bridge_exo_timers_high_resolution_005e9260(void* timers, void*);
void* __fastcall kotor_fastcall_0049f110(void* behavior, void*, void* owner);           // CAurBehavior::CAurBehavior
void __fastcall behavior_destruct_0049fa70(void* behavior, void*);                       // CAurBehavior::~CAurBehavior
void __fastcall behavior_void_return_004015a0(void*, void*);                             // vtable slot 1 (ret)
void __fastcall behavior_void_return_4_0060e760(void*, void*, U);                        // vtable slot 3 (ret 4)
U __fastcall behavior_return_zero_0063e7f0(void*, void*);                                // vtable slots 5..7
U __fastcall kotor_fastcall_00445080(void* list, void*, void* item);                     // CExoArrayList::Remove
}

namespace {
const char* const k_anim_names_006288f0[0x2d8] = {
#include "edba43977d-claude_dialogue_camera_anim.inc"
};
}  // namespace

extern "C" {

// 004CC360 CSWSObject::TimeStampCameraAnimation(): +70 = 0; +68 = low dword of HighResolutionTimer / 1000 (RET 0)
void __fastcall kotor_fastcall_004cc360(void* self, void*) {
    wr(self, 0x70, 0);                                                                     // 004CC363
    const unsigned long long t = recovered_bridge_exo_timers_high_resolution_005e9260(
        rp(kotor_menu_10_19::dependencies::base_007a39e0, 4), nullptr);                     // 004CC36A..004CC372
    wr(self, 0x68, static_cast<U>(t / 1000ull));                                           // 004CC377..004CC385 __aulldiv
}

// 0062AB70 CGuiInGame::IsLoopingDialogStuntAnimation(WORD anim): 1 when 0578h <= anim <= 06BFh (RET 4)
U __fastcall kotor_fastcall_0062ab70(void*, void*, U anim) {
    const U16 a = static_cast<U16>(anim);                                                  // 0062AB70 mov ax, word
    return (a >= 0x578 && a <= 0x6bf) ? 1u : 0u;                                           // 0062AB75..0062AB8B (jb/ja)
}

// 0062B470 CGuiInGame::SetDeferredFOVCallForAnimatedCutscenes(float fov) (RET 4)
void __fastcall kotor_fastcall_0062b470(void* self, void*, U fov_bits) {
    wr(self, 0xbfc, 1);                                                                    // 0062B474
    wr(self, 0xc00, fov_bits);                                                             // 0062B47E
}

// 006288F0 CGuiInGame::GetCameraAnimationName(CExoString* out, WORD anim) -> out (RET 8). The local starts as "none"
// ([0073ED44]); 2772h keeps it; anim-1000 <= 2D7h selects the jump-table string (216 entries keep "none").
void* __fastcall kotor_fastcall_006288f0(void*, void*, void* out, U anim) {
    ExoString name("none");                                                                // 00628909..0062891A
    const U16 a = static_cast<U16>(anim);                                                  // 0062891F
    if (a != 0x2772) {                                                                     // 00628924..00628930
        const U index = static_cast<U>(a) + 0xfffffc18u;                                   // 00628947..0062894A
        if (index <= 0x2d7 && k_anim_names_006288f0[index] != nullptr)                     // 0062894F..0062895A
            (void)kotor_menu20::AssignText005e5140(reinterpret_cast<kotor_menu20::String*>(&name), nullptr,
                                                    k_anim_names_006288f0[index]);         // 00629CFE..00629D02
    }
    (void)WindowCopy005e5b00(out, nullptr, &name);                                         // 0062893D / 00629D12
    return out;                                                                            // 00629D31
}

// 0062F250 CGuiInGame::GetCanOrientInDialog(id): 0 for OBJECT_INVALID or when id is on the +B44 list (RET 4)
U __fastcall kotor_fastcall_0062f250(void* self, void*, U id) {
    if (id == kInvalid) return 0;                                                          // 0062F256..0062F25E
    void* list = rp(self, 0xb44);                                                          // 0062F260
    void* cursor = rp(list, 0);                                                            // 0062F266..0062F268
    void* data = rp(list, 0);                                                              // 0062F26C
    if (data) data = rp(data, 8);                                                          // 0062F26E..0062F272
    if (!cursor) return 1;                                                                 // 0062F275..0062F277
    for (;;) {
        if (rd(data, 0) == id) return 0;                                                   // 0062F280..0062F282
        data = unresolved_list_next_005e9b90(rp(self, 0xb44), nullptr, &cursor);           // 0062F284..0062F28F
        if (!cursor) return 1;                                                             // 0062F294..0062F29A
    }
}

// 0062F3C0 CGuiInGame::GetCanHeadFollowInDialog(id): same walk over the +B48 list (RET 4)
U __fastcall kotor_fastcall_0062f3c0(void* self, void*, U id) {
    if (id == kInvalid) return 0;                                                          // 0062F3C6..0062F3CE
    void* list = rp(self, 0xb48);                                                          // 0062F3D0
    void* cursor = rp(list, 0);                                                            // 0062F3D6..0062F3D8
    void* data = rp(list, 0);                                                              // 0062F3DC
    if (data) data = rp(data, 8);                                                          // 0062F3DE..0062F3E2
    if (!cursor) return 1;                                                                 // 0062F3E5..0062F3E7
    for (;;) {
        if (rd(data, 0) == id) return 0;                                                   // 0062F3F0..0062F3F2
        data = unresolved_list_next_005e9b90(rp(self, 0xb48), nullptr, &cursor);           // 0062F3F4..0062F3FF
        if (!cursor) return 1;                                                             // 0062F404..0062F40A
    }
}

// 00641010 CSWCModule::SetAnimatedCamera(WORD anim, float fov, object id) -> animation length in ST0 (RET 0Ch).
// The third argument's stack slot is reused as the length output of the gob's slot-1Ch query.
float __fastcall kotor_fastcall_00641010(void* self, void*, U anim, U fov_bits, U object_id) {
    if (object_id == kInvalid) return 0.0f;                                                // 0064102D..00641038 fld [0073D700]
    if (rd(self, 0x12c) == 0) return 0.0f;                                                 // 0064103E..00641046
    void* options = kotor_fastcall_005ed700(client(), nullptr);                            // 0064104C..00641054
    if (rd8(options, 0x6d) == 3) {                                                         // 00641059..0064105E
        void* cam = vcall<void*>(rp(self, 0x40), 0x80, 0x106au);                           // 00641060..0064106A
        if (cam) kotor_fastcall_0063ba80(cam, nullptr, at(self, 0x10c));                   // 00641070..0064107D
    }
    void* scene = rp(self, 0x40);                                                          // 00641082 (edi, cached)
    vcall<void>(scene, 0x40, 0x3dcccccdu, 0x461c4000u);                                    // 00641087..00641093 (0.1f, 10000.0f)
    vcall<void>(rp(self, 0x12c), 0x4c, rd(rp(self, 0x48), 0x184));                         // 00641096..006410A8
    void* object = kotor_fastcall_005ed580(client(), nullptr, object_id);                  // 006410AB..006410B4
    if (!object) return 0.0f;                                                              // 006410BB..006410BD
    if (!vcall<U>(object, 0xc)) return 0.0f;                                               // 006410C3..006410CC
    (void)vcall<U>(object, 0xc);                                                           // 006410D2..006410D6 (result unused)
    V3 position{0, 0, 0};                                                                  // 006410E2..0064110F
    vcall<void>(rp(self, 0x12c), 0x5c, &position, position);                               // 00641112..00641137
    Q4 orientation{0x3f800000u, 0, 0, 0};                                                  // 00641117..0064112F
    vcall<void>(rp(self, 0x12c), 0x60, &orientation, orientation);                         // 0064113A..00641167
    vcall<void>(scene, 0x74, rp(self, 0x12c), "camerahook", 0u);                           // 0064116A..0064117C ([0074F42C])
    void* gui = in_game_gui();                                                             // 0064117F..00641191 (ebx)
    {
        U name[2];                                                                         // [esp+14h], built by the callee
        (void)kotor_fastcall_006288f0(gui, nullptr, name, anim);                           // 0064118D..0064119B
        str_assign(at(self, 0x130), name);                                                 // 006411A0..006411B1
        frame_LogStringDtor_005e5c20(name, nullptr);                                       // 006411B6..006411C2
    }
    vcall<void>(rp(self, 0x12c), 0x130);                                                   // 006411C7..006411CF
    vcall<void>(rp(self, 0x12c), 0x138);                                                   // 006411D5..006411DD
    U length = object_id;                                                                  // [esp+50h] = the id's slot
    U unused = 0;                                                                          // [esp+10h] second output
    {
        const char* text = WindowCStr005e5670(at(self, 0x130), nullptr);                   // 006411EB..006411F7
        vcall<void>(rp(self, 0x12c), 0x1c, text, &unused, &length);                        // 006411FC..00641203
    }
    if (fcom(bitsf(length), 0.0) & 0x41 && !jp(fcom(bitsf(length), 0.0), 0x41))           // 00641206..00641215
        length = 0;                                                                        // 00641217
    const U looping = kotor_fastcall_0062ab70(gui, nullptr, anim);                         // 0064121F..00641226
    {
        const char* text = WindowCStr005e5670(at(self, 0x130), nullptr);                   // 0064122D..0064124E
        vcall<void>(rp(self, 0x12c), 0x18, text, 0x3f800000u, looping ? 2u : 3u, 0u);      // 0064122F..0064125A
    }
    if (!(fcom(bitsf(fov_bits), 0.0) & 0x41))                                              // 0064125D..0064126C (ordered >)
        kotor_fastcall_0062b470(in_game_gui(), nullptr, fov_bits);                         // 0064126E..00641283
    return bitsf(length);                                                                  // 00641288
}

// 0062BF30 CGuiInGame::InitializeShotCamera() -> +B20 (RET 0)
U __fastcall kotor_fastcall_0062bf30(void* self, void*) {
    if (rd(self, 0xb20) == 0) {                                                            // 0062BF33..0062BF3B
        void* module = kotor_fastcall_005ed530(client(), nullptr);                         // 0062BF3D..0062BF45
        if (module) kotor_fastcall_006412f0(module, nullptr, rd(self, 0xc4));              // 0062BF4A..0062BF57
        wr(self, 0xb20, 1);                                                                // 0062BF5C
    }
    return rd(self, 0xb20);                                                                // 0062BF66
}

// 0062BF70 CGuiInGame::SetAnimatedCamera(WORD anim, float fov) -> 0/1 (RET 8)
U __fastcall kotor_fastcall_0062bf70(void* self, void*, U anim, U fov_bits) {
    if (rd(self, 0xb28) == 0) return 0;                                                    // 0062BF75..0062BF7D
    const U16 a = static_cast<U16>(anim);                                                  // 0062BF83 (ebx, compared as bx)
    if (a < 0x3e8 || a > 0x6bf) return 0;                                                  // 0062BF87..0062BF97
    void* module = kotor_fastcall_005ed530(client(), nullptr);                             // 0062BF9D..0062BFA5
    if (!module) return 0;                                                                 // 0062BFAA..0062BFAE
    const U target = rd(self, 0x188);                                                      // 0062BFB0
    wr(self, 0xb24, 1);                                                                    // 0062BFB6
    wr(self, 0xb20, 0);                                                                    // 0062BFC0
    wr16(self, 0x164, a);                                                                  // 0062BFCA
    void* object = kotor_fastcall_004ae750(server(), nullptr, target);                     // 0062BFD1..0062BFDC
    if (object && vcall<void*>(object, 0x10))                                              // 0062BFE3..0062BFF0
        kotor_fastcall_004cc360(vcall<void*>(object, 0x10), nullptr);                      // 0062BFF2..0062BFFB
    const float length = kotor_fastcall_00641010(module, nullptr, anim, fov_bits, rd(self, 0x170)); // 0062C000..0062C00F
    wrf(self, 0xb2c, length);                                                              // 0062C014
    return 1;                                                                              // 0062C01D
}

// 0062C030 CGuiInGame::SetComputerCameraShot(camera id, flag160, video effect) (RET 0Ch)
void __fastcall kotor_fastcall_0062c030(void* self, void*, U camera_id, U flag160, U effect) {
    if (rd(self, 0x1b0) == 0) return;                                                      // 0062C038..0062C041
    (void)frame_RemovePanel_0040c830(rp(self, 0x38), nullptr, rp(self, 0x3c));             // 0062C047..0062C04E
    kotor_fastcall_0040bc70(rp(self, 0x38), nullptr, rp(self, 0x48), 0, 1);                // 0062C053..0062C05E
    vcall<void>(rp(self, 0x48), 0x78, rd(rp(self, 0x3c), 0x19c0));                        // 0062C063..0062C072
    void* module = kotor_fastcall_005ed530(client(), nullptr);                             // 0062C075..0062C07E
    void* camera = vcall<void*>(rp(module, 0x40), 0x80, 0x106du);                          // 0062C083..0062C08D
    if (camera) {                                                                          // 0062C093..0062C099
        bool found = false;                                                                // ecx
        U i = 0;                                                                           // ebx
        if (static_cast<I>(rd(self, 0x1b0)) > 0) {                                         // 0062C0A9..0062C0AB
            for (;;) {
                if (rd(self, 0x1b4 + i * 0x30) == camera_id) { found = true; break; }      // 0062C0B7..0062C0B9
                ++i;                                                                       // 0062C0C1
                if (!(static_cast<I>(i) < static_cast<I>(rd(self, 0x1b0)))) break;         // 0062C0C5..0062C0C7
            }
        }
        V3 position{0, 0, 0};                                                              // [esp+18h..20h]
        Q4 orientation{0x3f800000u, 0, 0, 0};                                              // [esp+24h..30h]
        U fov = 0x425c0000u;                                                               // [esp+48h] = 55.0f
        U shot_height = 0;                                                                 // [esp+10h]
        if (found) {                                                                       // 0062C11A
            void* shot = at(self, i * 0x30);                                               // 0062C120..0062C126 (edi)
            position.x = rd(shot, 0x1b8);                                                  // 0062C12F
            position.y = rd(shot, 0x1bc);                                                  // 0062C131
            position.z = fbits(static_cast<float>(static_cast<double>(rdf(shot, 0x1c0)) +
                                                  static_cast<double>(rdf(shot, 0x1d8)))); // 0062C134..0062C153
            Q4 source;                                                                     // copied from +1C4
            std::memcpy(&source, at(shot, 0x1c4), 16);
            orientation = source;                                                          // 0062C151..0062C16C
            const float roll = static_cast<float>(kotor_cdecl_004acb90(source));           // 0062C170..0062C190
            const float pitch = static_cast<float>(quaternion_pitch_004aa020(&orientation) +
                                                   static_cast<double>(rdf(shot, 0x1d4))); // 0062C197..0062C1AB
            const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&orientation));   // 0062C1A7..0062C1B4
            Q4 rotated;                                                                    // [esp+40h] (esp-relative)
            const Q4* r = static_cast<const Q4*>(yaw_pitch_roll_004acac0(&rotated, yaw, pitch, roll)); // 0062C1B7..0062C1BC
            orientation = *r;                                                              // 0062C1C1..0062C1F7
            fov = rd(shot, 0x1dc);                                                         // 0062C1D8 / 0062C1EB
            shot_height = fbits(rdf(self, 0x1e0 + i * 0x30));                              // 0062C1DE..0062C1F3
        }
        kotor_fastcall_006bb590(camera, nullptr, 6, position, orientation, fov);           // 0062C201..0062C23F
        wr(self, 0xe8, position.x);                                                        // 0062C24E
        wr(self, 0xf4, shot_height);                                                       // 0062C250
        wr(self, 0xec, position.y);                                                        // 0062C25D
        wr(self, 0xe4, 1);                                                                 // 0062C260
        wr(self, 0xf0, position.z);                                                        // 0062C26A
        if (effect == 0xffffffffu)                                                         // 0062C25A..0062C26D
            kotor_fastcall_005edf10(client(), nullptr, 0);                                 // 0062C26F / 0062C278..0062C282
        else if (effect != 0xfffffffeu)                                                    // 0062C273..0062C276
            kotor_fastcall_005edf10(client(), nullptr, static_cast<I>(effect));            // 0062C278..0062C282
    }
    wr(self, 0x160, flag160);                                                              // 0062C287..0062C28B
}

// 006412F0 CSWCModule::SetDialogCamera(flag) (RET 4): saves the stick camera (options+6Dh == 3), constructs a
// CSWBehaviorCameraDialog(scene->slot4(), flag), camera mode 4, and hands the behavior to the scene (slot 7Ch).
void __fastcall kotor_fastcall_006412f0(void* self, void*, U flag) {
    void* options = kotor_fastcall_005ed700(client(), nullptr);                            // 00641306..00641312
    if (rd8(options, 0x6d) == 3) {                                                         // 00641317..0064131C
        void* cam = vcall<void*>(rp(self, 0x40), 0x80, 0x106au);                           // 0064131E..00641328
        if (cam) kotor_fastcall_0063ba80(cam, nullptr, at(self, 0x10c));                   // 00641330..0064133B
    }
    void* behavior = live_operator_new(0x9c);                                              // 00641340..00641345
    if (behavior) {                                                                        // 00641353..0064135D
        void* owner = vcall<void*>(rp(self, 0x40), 0x4);                                   // 0064135F..00641369
        behavior = kotor_fastcall_006bb670(behavior, nullptr, owner, flag);                // 00641362..0064136F
    }
    menu_tables_m41::SetCameraMode_0061b6f0(kotor_fastcall_005ed700(client(), nullptr), nullptr, 4); // 0064137A..00641394
    vcall<void>(rp(self, 0x40), 0x7c, behavior);                                           // 00641399..0064139F
}

// ---------------------------------------------------------------------------------------------------------------
// CSWBehaviorCameraDialog (9Ch bytes; vtable 00756AE8)
// ---------------------------------------------------------------------------------------------------------------
// 006BB740 GetType() -> 106Dh
U __fastcall kotor_fastcall_006bb740(void*, void*) { return 0x106d; }                      // 006BB740

// 006BB750 ~CSWBehaviorCameraDialog: vtable reset, release the +68 / +64 dependencies, then ~CAurBehavior
void __fastcall kotor_fastcall_006bb750(void* self, void*) {
    wp(self, 0, k_camera_dialog_vtable_00756ae8);                                          // 006BB756
    if (rd(self, 0x68)) kotor_fastcall_00445080(at(rp(self, 0x68), 4), nullptr, at(self, 0x68)); // 006BB753..006BB766
    if (rd(self, 0x64)) kotor_fastcall_00445080(at(rp(self, 0x64), 4), nullptr, at(self, 0x64)); // 006BB76B..006BB779
    behavior_destruct_0049fa70(self, nullptr);                                             // 006BB77E..006BB781 (tail jmp)
}

// 006BB790 scalar deleting destructor (RET 4)
void* __fastcall kotor_fastcall_006bb790(void* self, void*, U flags) {
    kotor_fastcall_006bb750(self, nullptr);                                                // 006BB793
    if (flags & 1) live_free(self);                                                        // 006BB798..006BB7A0
    return self;                                                                           // 006BB7A8
}

// 006BB670 CSWBehaviorCameraDialog(owner, flag) (RET 8)
void* __fastcall kotor_fastcall_006bb670(void* self, void*, void* owner, U flag) {
    (void)kotor_fastcall_0049f110(self, nullptr, owner);                                   // 006BB68D..006BB690
    wp(self, 0, k_camera_dialog_vtable_00756ae8);                                          // 006BB695
    wr(self, 0x40, 0); wr(self, 0x44, 0); wr(self, 0x48, 0);                               // 006BB69D..006BB6A3
    wr(self, 0x58, 0); wr(self, 0x54, 0); wr(self, 0x50, 0);                               // 006BB6A9..006BB6AF
    wr(self, 0x4c, 0x3f800000u);                                                           // 006BB6B2
    wr(self, 0x64, 0);                                                                     // 006BB6B9
    wr(self, 0x68, 0);                                                                     // 006BB6BE
    wr(self, 0x14, 0); wr(self, 0x18, 0); wr(self, 0x1c, 0); wr(self, 0x20, 0); wr(self, 0x24, 0); // 006BB6C1..006BB6CD
    if (rd(self, 0x64)) kotor_fastcall_00445080(at(rp(self, 0x64), 4), nullptr, at(self, 0x64)); // 006BB6D0..006BB6DA
    wr(self, 0x64, 0);                                                                     // 006BB6DF
    if (rd(self, 0x68)) kotor_fastcall_00445080(at(rp(self, 0x68), 4), nullptr, at(self, 0x68)); // 006BB6E1..006BB6EC
    wr(self, 0x68, 0);                                                                     // 006BB6F5
    wr(self, 0x38, flag);                                                                  // 006BB6F8
    wr(self, 0x2c, 0); wr(self, 0x30, 0); wr(self, 0x34, 0);                               // 006BB6FB..006BB701
    for (U i = 0; i < 4; ++i) {                                                            // 006BB704..006BB71C
        wr(self, 0x74 + i * 0xc, 0);
        wr(self, 0x6c + i * 0xc, 0xffffffffu);
        wr(self, 0x70 + i * 0xc, 0xffffffffu);
    }
    wr(self, 0x3c, 1);                                                                     // 006BB723
    return self;
}

// 006BB590 SetShot(type, Vector position, Quaternion orientation, float fov) (RET 24h): only type 6 is applied
void __fastcall kotor_fastcall_006bb590(void* self, void*, U type, V3 position, Q4 orientation, U fov_bits) {
    if (type != 6) return;                                                                 // 006BB590..006BB598
    wr(self, 0x34, 0);                                                                     // 006BB59D
    if (vcall<void*>(rp(self, 0x10), 0x4))                                                 // 006BB59A..006BB5AB
        vcall<void>(vcall<void*>(rp(self, 0x10), 0x4), 0x44, fov_bits);                    // 006BB5AD..006BB5BE
    std::memcpy(at(self, 0x40), &position, 12);                                            // 006BB5C1..006BB5E9
    wr(self, 0x28, 6);                                                                     // 006BB5D2
    std::memcpy(at(self, 0x4c), &orientation, 16);                                         // 006BB5E0..006BB5FA
}

// 006BB610 GetLineOfActionSide(a, b) (RET 8): the four (+6C, +70, +74) line-of-action records
U __fastcall kotor_fastcall_006bb610(void* self, void*, U a, U b) {
    for (U i = 0; i < 4; ++i) {                                                            // 006BB61B..006BB642
        const U base = 0x6c + i * 0xc;
        if (rd(self, base + 8) == 0) return 0;                                             // 006BB620..006BB625
        if (rd(self, base + 4) == a && rd(self, base) == b) return rd(self, base + 8);     // 006BB627..006BB630 / 006BB64C
        if (rd(self, base + 4) == b && rd(self, base) == a)                                // 006BB632..006BB639
            return (rd(self, base + 8) != 2 ? 1u : 0u) + 1u;                               // 006BB659..006BB66B
    }
    return 0;                                                                              // 006BB644..006BB649
}

// 006BCEC0 AddCalculatedLineOfAction(a, b) (RET 8)
U __fastcall kotor_fastcall_006bcec0(void* self, void*, U a, U b) {
    U i = 0;                                                                               // 006BCECE
    for (; i < 4; ++i) {                                                                   // 006BCED0..006BCEF5
        const U base = 0x6c + i * 0xc;
        if (rd(self, base + 8) == 0) break;                                                // 006BCED3..006BCED8
        if ((rd(self, base + 4) == a && rd(self, base) == b) ||
            (rd(self, base + 4) == b && rd(self, base) == a)) {                            // 006BCEDA..006BCEEC
            if (rd(self, 0x38) == 0) return kotor_fastcall_006bb610(self, nullptr, a, b);  // 006BCF1F..006BCF2A
            return rd(self, base + 8);                                                     // 006BCF36..006BCF39
        }
    }
    if (i == 4) i = 0;                                                                     // 006BCEF7..006BCEFC
    const U side = kotor_fastcall_006bc600(self, nullptr);                                 // 006BCEFE..006BCF00
    wr(self, 0x6c + i * 0xc, b);                                                           // 006BCF05..006BCF0F
    wr(self, 0x70 + i * 0xc, a);                                                           // 006BCF15
    wr(self, 0x74 + i * 0xc, side);                                                        // 006BCF18
    return side;
}

// 006FE517 __finite(double): exponent != 7FFh (cdecl)
int __cdecl kotor_cdecl_006fe517(double value) {
    U16 high;
    std::memcpy(&high, reinterpret_cast<const char*>(&value) + 6, 2);                      // 006FE517 mov ax, [esp+0Ah]
    return (high & 0x7ff0) != 0x7ff0 ? 1 : 0;                                              // 006FE51C..006FE529
}

}  // extern "C"

// vtable 00756AE8: [006BB790 dtor, 004015A0 void, 006BCF50 Control, 0060E760 void(4), 006BB740 GetType,
//                   0063E7F0 x3 return_zero]
extern "C" const void* const k_camera_dialog_vtable_00756ae8[8] = {
    reinterpret_cast<const void*>(&kotor_fastcall_006bb790),
    reinterpret_cast<const void*>(&behavior_void_return_004015a0),
    reinterpret_cast<const void*>(&kotor_fastcall_006bcf50),
    reinterpret_cast<const void*>(&behavior_void_return_4_0060e760),
    reinterpret_cast<const void*>(&kotor_fastcall_006bb740),
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),
    reinterpret_cast<const void*>(&behavior_return_zero_0063e7f0),
};
