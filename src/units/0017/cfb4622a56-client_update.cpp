// Lane W4 G12: client per-frame update chain and small client leaves reached from the in-game MainLoop.
// Every body is transcribed from the original swkotor.exe (dis/<ADDR>.asm); each statement cites its PCs.
#include "27e3c46f56-w4_frame.hpp"

extern "C" {
// Existing runtime1115 providers used only here.
void* __fastcall server_internal_get_module_004b14f0(void* server_internal, void*);   // CServerExoAppInternal::GetModule
void* __fastcall module_get_area_004c30f0(void* module, void*);                       // CSWSModule::GetArea
void __fastcall kotor_m15_client_internal_stop_load_music_005f1b90(void* client, void*); // CClientExoAppInternal::StopLoadMusic
// Cross-lane pending provider (server walkmesh room query, W5 G16 family): see INTEGRATION.md.
std::int32_t __fastcall kotor_fastcall_004bb730(void* server_area, void*, W4Vector* position, std::int32_t mode); // CSWSArea::GetRoomIndex
}
namespace menu_normal_40_49 { struct MusicInternal; }
namespace kotor_sound_path {
void __fastcall StopMenuMusic005f6440(menu_normal_40_49::MusicInternal*, void*);      // CClientExoAppInternal::StopMenuMusic
}

namespace w4 {
// W4-owned write-only profiling cells of CClientAIMaster::UpdateState (no other original user).
U32 ai_profile_end_low_00832c58 = 0, ai_profile_end_high_00832c5c = 0;
U32 ai_profile_start_low_00832c60 = 0, ai_profile_start_high_00832c64 = 0;
float ai_profile_module_00832c68 = 0.0f, ai_profile_objects_00832c6c = 0.0f;
static const char not_enabled_0073d5c0[] = "NOT ENABLED";                                    // .rdata 0073D5C0
}
using namespace w4;

namespace {
void* client_internal() { return lp(client_app(), 4); }                                     // [[[7A39FC]+4]+4]
}

// 00604870 CClientAIList::GetNextObject() (thiscall, RET 0).
extern "C" void* __fastcall kotor_fastcall_00604870(void* list_owner, void*) {
    void* list = lp(list_owner, 4);                                                          // 00604873
    if (lw(list, 0) == 0) return nullptr;                                                    // 00604876..00604879, 006048BB
    void* position = static_cast<U8*>(list_owner) + 8;                                       // 0060487C
    unresolved_list_next_005e9b90(list, nullptr, position);                                  // 0060487F..00604880
    if (lw(position, 0) == 0) sw(position, 0, lw(lp(list_owner, 4), 0));                     // 00604885..0060488F
    const U32 id = unresolved_list_at_pos_005e9b60(lp(list_owner, 4), nullptr, lp(position, 0)); // 00604891..00604897
    void* object = kotor_fastcall_005f10f0(client_internal(), nullptr, id);                  // 0060489C..006048A9
    if (object == nullptr) return nullptr;                                                   // 006048AE..006048B1
    return vcall<void*>(object, 0x0C);                                                       // 006048B3..006048B8 jmp [edx+0Ch]
}

// 006048C0 CClientAIMaster::UpdateState() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_006048c0(void* master, void*) {
    void* list_owner = static_cast<U8*>(master) + 4;                                         // 006048C6
    void* object = kotor_fastcall_00604870(list_owner, nullptr);                             // 006048C9..006048CB
    I32 count = li(list_owner, 0);                                                           // 006048D0
    {
        const U64 t = high_resolution_timer();                                               // 006048D4..006048DC
        ai_profile_start_low_00832c60 = static_cast<U32>(t);                                 // 006048E3
        ai_profile_start_high_00832c64 = static_cast<U32>(t >> 32);                          // 006048E8
    }
    if (object != nullptr && count > 0) {                                                    // 006048E1..006048F2
        do {
            if (object != nullptr) vcall<void>(object, 0x74);                                // 006048F4..006048FC AIUpdate
            void* list = lp(list_owner, 4);                                                  // 006048FF
            if (lw(list, 0) == 0) {                                                          // 00604902..00604905
                object = nullptr;                                                            // 00604907
            } else {
                void* position = static_cast<U8*>(list_owner) + 8;                           // 0060490B
                unresolved_list_next_005e9b90(list, nullptr, position);                      // 0060490E..0060490F

                if (lw(position, 0) == 0) sw(position, 0, lw(lp(list_owner, 4), 0));         // 00604914..0060491E

                const U32 id = unresolved_list_at_pos_005e9b60(lp(list_owner, 4), nullptr, lp(position, 0)); // 00604920..00604926
                void* found = kotor_fastcall_005f10f0(client_internal(), nullptr, id);       // 0060492B..00604938
                object = found ? vcall<void*>(found, 0x0C) : nullptr;                        // 0060493D..0060494C
            }
        } while (--count != 0);                                                              // 0060494E..0060494F
    }
    {
        const U64 t = high_resolution_timer();                                               // 00604951..00604959
        ai_profile_end_low_00832c58 = static_cast<U32>(t);                                   // 0060496A
        const I32 diff = static_cast<I32>(static_cast<U32>(t) - ai_profile_start_low_00832c60);
        ai_profile_end_high_00832c5c = static_cast<U32>(t >> 32);                            // 00604979
        ai_profile_objects_00832c6c =
            static_cast<float>(static_cast<double>(diff) * frame_control::profile_scale_0073d708); // 00604975..00604985
    }
    {
        const U64 t = high_resolution_timer();                                               // 0060498B..0060498E
        ai_profile_start_high_00832c64 = static_cast<U32>(t >> 32);                          // 00604993
        ai_profile_start_low_00832c60 = static_cast<U32>(t);                                 // 006049A2
    }
    // 00604999..006049C7: delta = fild qword (exact) * 1e-6f, rounded once to the PC53 product, stored as float.
    // (double)I64 is exact for |ticks| < 2^53 (~285 years of microseconds), so the product rounding matches.
    void* app = client_app();                                                                // 00604999..0060499F
    auto* timer = static_cast<menu_continuation::WorldTimer*>(lp(lp(app, 4), 0x2C));         // 006049A7..006049AA
    const I64 ticks = timer->GetSnapshotTimeDifference_004adc80();                           // 006049AD
    const float delta = static_cast<float>(static_cast<double>(ticks) * menu_continuation::scale_007439bc); // 006049BA..006049C7
    kotor_fastcall_00644650(kotor_fastcall_005ed530(app, nullptr), nullptr, delta);          // 006049CA..006049D1
    {
        const U64 t = high_resolution_timer();                                               // 006049D6..006049DF
        ai_profile_end_low_00832c58 = static_cast<U32>(t);                                   // 006049EA
        const I32 diff = static_cast<I32>(static_cast<U32>(t) - ai_profile_start_low_00832c60);
        ai_profile_end_high_00832c5c = static_cast<U32>(t >> 32);                            // 006049FB
        ai_profile_module_00832c68 =
            static_cast<float>(static_cast<double>(diff) * frame_control::profile_scale_0073d708); // 006049F5..00604A08
    }
}

// 005EE910 CClientExoAppInternal::GetActivePauseState() (thiscall, RET 0; returns AL).
extern "C" U8 __fastcall kotor_fastcall_005ee910(void* client, void*) {
    const U8 flags = lb(client, 0x1CC);                                                      // 005EE910
    if (flags & 2) return 2;                                                                 // 005EE916..005EE91C
    return static_cast<U8>(flags & 1);                                                       // 005EE91D
}

// 005EE6F0 CClientExoAppInternal::GetModuleCamera() (unlabelled; thiscall, RET 0).
extern "C" void* __fastcall kotor_fastcall_005ee6f0(void* client, void*) {
    void* module = lp(client, 0x18);                                                         // 005EE6F0
    return module ? lp(module, 0x40) : nullptr;                                              // 005EE6F3..005EE6FD
}

// 005ED560 CClientExoApp::GetModuleCamera() (thiscall, RET 0): mov ecx,[ecx+4]; jmp 005EE6F0.
extern "C" void* __fastcall kotor_fastcall_005ed560(void* app, void*) {
    return kotor_fastcall_005ee6f0(lp(app, 4), nullptr);
}

// 005EE3E0 CSWCModule::GetModuleName(CExoString* out) (thiscall, RET 4).
extern "C" void* __fastcall kotor_fastcall_005ee3e0(void* module, void*, void* out) {
    WindowCopy005e5b00(out, nullptr, static_cast<U8*>(module) + 0x10);                       // 005EE3E6..005EE3F4
    return out;                                                                              // 005EE3F9
}

// 004AED20 CServerExoApp::GetAutoSavePending() (thiscall, RET 0).
extern "C" U32 __fastcall kotor_fastcall_004aed20(void* server, void*) {
    return lw(lp(server, 4), 0x100B8);                                                       // 004AED20..004AED23
}

// 005F3030 CClientExoAppInternal::SetCanSendDialog(int) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005f3030(void* client, void*, U32 value) {
    sw(client, 0x228, value);                                                                // 005F3037
    void* timer = lp(client_internal(), 0x2C);                                                // 005F303D..005F3049
    const I64 now = static_cast<I64>(kotor_fastcall_004adc70(timer, nullptr));               // 005F304C



    sw(client, 0x22C, static_cast<U32>(now / 1000));                                         // 005F3051..005F305F __alldiv

}

// 006364C0 CSWParty::AddCharacter: single owner src/party/membership.cpp (full body; the former partial here was removed).

// 005F2BB0 CClientExoAppInternal::SetPlayerCreature(id) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005f2bb0(void* client, void*, U32 id) {
    sw(client, 0x20, id);                                                                    // 005F2BB0..005F2BB4
    void* party = lp(client, 0x270);                                                         // 005F2BB7
    if (party == nullptr) return;                                                            // 005F2BBD..005F2BBF
    (void)kotor_fastcall_006364c0(party, nullptr, id);                                       // 005F2BC1..005F2BC5 tail jmp
}

// 00401430 ClearLogCounters (cdecl; the caller pushes one word and cleans it).
extern "C" const char* __cdecl kotor_cdecl_00401430(U32) {
    return not_enabled_0073d5c0;                                                             // 00401430
}

// 005ED9C0 / 005ED9D0 CClientExoApp::StopMenuMusic / StopLoadMusic (mov ecx,[ecx+4]; jmp internal).
extern "C" void __fastcall kotor_fastcall_005ed9c0(void* app, void*) {
    kotor_sound_path::StopMenuMusic005f6440(static_cast<menu_normal_40_49::MusicInternal*>(lp(app, 4)), nullptr);
}
extern "C" void __fastcall kotor_fastcall_005ed9d0(void* app, void*) {
    kotor_m15_client_internal_stop_load_music_005f1b90(lp(app, 4), nullptr);
}

// 0068D4A0 CSWCAmbientSound start (unlabelled; thiscall, RET 0). Virtual slots +00h/+10h/+18h are the
// ambient-sound object's own vtable (constructed by CSWCArea::LoadArea, W3).
extern "C" void __fastcall kotor_fastcall_0068d4a0(void* ambient, void*) {
    sw(ambient, 0xF4, 1);                                                                    // 0068D4A3
    void* options = kotor_fastcall_005ed700(client_app(), nullptr);                          // 0068D4AD..0068D4B5
    const U32 mode = lb(options, 0x73) & 3u;                                                 // 0068D4BA..0068D4C5
    const U32 active = lw(ambient, 0xEC);                                                    // 0068D4BF
    sw(ambient, 0xFC, mode);                                                                 // 0068D4CA
    if (active != 0) {                                                                       // 0068D4C8..0068D4D0
        if (lw(ambient, 0x14) != 0) vcall<void, U32>(ambient, 0x10, 1);                      // 0068D4D2..0068D4DF
        else vcall<void, U32>(ambient, 0x00, lw(ambient, 4));                                // 0068D4E4..0068D4EC
    }
    if (lw(ambient, 0xF0) != 0) vcall<void, U32>(ambient, 0x18, lw(ambient, 0x1C));          // 0068D4EE..0068D500
}

// 00604D70 CSWCArea::StartSounds() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_00604d70(void* area, void*) {
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 00604D70..00604D7F
    kotor_fastcall_005ed9c0(client_app(), nullptr);                                          // 00604D84..00604D8D
    kotor_fastcall_005ed9d0(client_app(), nullptr);                                          // 00604D92..00604D9B
    kotor_fastcall_0068d4a0(lp(area, 0x1DC), nullptr);                                       // 00604DA0..00604DA7 tail jmp
}

// 005D6470 / 005D6480 CExoSound::SetDesiredEnvironment / SetDesiredAmbientScale (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005d6470(void* sound, void*, U32 environment) {
    if (void* internal = lp(sound, 0)) sw(internal, 0x20, environment);                      // 005D6470..005D647A
}
extern "C" void __fastcall kotor_fastcall_005d6480(void* sound, void*, float scale) {
    if (void* internal = lp(sound, 0)) sf(internal, 0x24, scale);                            // 005D6480..005D648A
}

// 004AEAF0 CServerExoApp::GetRoomIndex(void* unused, Vector*, int) (thiscall, RET 0Ch). The only caller
// (00605624..0060562B) pushes area+10Ch, the position and the mode; the body ignores the first word and forwards
// ([esp+10h], [esp+14h]) to CSWSArea::GetRoomIndex.
extern "C" I32 __fastcall kotor_fastcall_004aeaf0(void* server, void*, void* area_key, W4Vector* position, I32 mode) {
    (void)area_key;                                                                          // unused by the body
    if (server_internal_get_module_004b14f0(lp(server, 4), nullptr) == nullptr) return -1;   // 004AEAF3..004AEB01
    void* area = module_get_area_004c30f0(server_internal_get_module_004b14f0(lp(server, 4), nullptr), nullptr); // 004AEB03..004AEB0D
    if (area == nullptr) return -1;                                                          // 004AEB12..004AEB14
    return kotor_fastcall_004bb730(area, nullptr, position, mode);                           // 004AEB16..004AEB22
}

// 00605610 CSWCArea::GetRoomIndex(Vector*, int) (thiscall, RET 8).
extern "C" I32 __fastcall kotor_fastcall_00605610(void* area, void*, W4Vector* position, I32 mode) {
    return kotor_fastcall_004aeaf0(server_app(), nullptr, static_cast<U8*>(area) + 0x10C, position, mode); // 00605610..0060562B
}

// 006054F0 CSWCArea::GetRoom(int) (thiscall, RET 4).
extern "C" void* __fastcall kotor_fastcall_006054f0(void* area, void*, I32 index) {
    if (index == -1) return nullptr;                                                         // 006054F4..006054F9
    return static_cast<U8*>(lp(area, 0x260)) + static_cast<U32>(index) * 0x48u;              // 006054FB..00605504
}

// 005EE860 CClientExoAppInternal::UpdateRoom(Vector*) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_005ee860(void* client, void*, void* position) {
    void* module = lp(client, 0x18);                                                         // 005EE860
    if (module == nullptr) return;                                                           // 005EE863..005EE865
    void* area = lp(module, 0x48);                                                           // 005EE86D
    if (area == nullptr) return;                                                             // 005EE870..005EE872
    void* scene = lp(area, 0x184);                                                           // 005EE874
    if (scene == nullptr) return;                                                            // 005EE87A..005EE87C
    const U32 busy = vcall<U32>(scene, 0x84);                                                // 005EE87E..005EE882
    if (busy != 0) return;                                                                   // 005EE888..005EE88A
    const I32 index = kotor_fastcall_00605610(area, nullptr, static_cast<W4Vector*>(position), 0); // 005EE88C..005EE895
    void* room = kotor_fastcall_006054f0(area, nullptr, index);                              // 005EE89C..005EE89F
    if (index < 0) {                                                                         // 005EE8A4..005EE8A9
        (void)frame_R4_005ed690(client_app(), nullptr);                                      // 005EE8AB..005EE8B4 (result unused)
        return;
    }
    vcall<void, U32>(scene, 0x7C, lw(room, 0x44));                                           // 005EE8BE..005EE8C6
    kotor_fastcall_005d6470(pt(frame_control::sound_manager_007a39ec), nullptr, lw(room, 0x30)); // 005EE8C9..005EE8D3
    kotor_fastcall_005d6480(pt(frame_control::sound_manager_007a39ec), nullptr, lf(room, 0x34)); // 005EE8D8..005EE8E5
}

// 00644650 CSWCModule::Update(float delta) (thiscall, RET 4).
extern "C" void __fastcall kotor_fastcall_00644650(void* module, void*, float delta) {
    if (lw(module, 0x48) == 0) return;                                                       // 00644656..0064465B
    if (lw(module, 0x40) == 0) return;                                                       // 00644661..00644666
    if (lw(module, 0x8C) != 0) {                                                             // 0064466C..00644679
        U32 out8 = 0, outc = 0;                                                              // [esp+8], [esp+0Ch]
        if (kotor_fastcall_006441d0(module, nullptr, delta, &out8, &outc) != 0) {            // 0064467B..0064468F
            void* behavior = vcall<void*, I32>(lp(module, 0x40), 0x80, -1);                 // 00644691..00644698 GetBehavior(-1)
            if (behavior != nullptr) {                                                       // 0064469E..006446A0
                void* stick = vcall<void*>(behavior, 0x1C);                                  // 006446A2..006446A6
                if (stick != nullptr) {                                                      // 006446A9..006446AB
                    sw(stick, 0x3C, out8);                                                   // 006446AD..006446B5
                    sw(stick, 0x44, outc);                                                   // 006446B8
                }
            }
        }
        sw(module, 0x8C, 0);                                                                 // 006446BB
    }
    if (kotor_fastcall_005ee910(client_internal(), nullptr) == 0)                            // 006446C5..006446D7
        sf(module, 0xB8, static_cast<float>(static_cast<double>(delta) + lf(module, 0xB8))); // 006446D9..006446E3
    kotor_fastcall_006097f0(lp(module, 0x48), nullptr, delta);                               // 006446E9..006446ED
    void* gui = kotor_fastcall_005eda40(client_app(), nullptr);                              // 006446F2..006446FB



    if (lb(gui, 0x70) == 0) vcall<void>(lp(module, 0x40), 0x08);                             // 00644700..0064470D Camera::RenderScene

}

// ------------------------------------------------------------------------------------------------------------
// [007A2484]: .data initial 1, never written in the original image (readers 005F5D5C, 0061B1CC, 0064CB94,
// 0064CC4A, 00656634, 00656754, 00656A00, 00656B0C). Single owner here.
extern "C" volatile std::uint32_t kotor_cell_007a2484 = 1;

extern "C" {
// Cross-lane pending (W5 player-control family; G16).
void __fastcall kotor_fastcall_00679340(void* control, void*, std::uint32_t object_id);       // CSWPlayerControl::SetPlayer
void __fastcall kotor_fastcall_006793b0(void* control, void*, void* camera);                  // CSWPlayerControl::SetCamera
void* __fastcall kotor_fastcall_00679780(void* self, void*, std::uint32_t object_id, void* camera) noexcept(false); // CSWPlayerControlCamRelative ctor (may throw: live_operator_new failure paths)
// W4 camera fork (G14).
void __fastcall kotor_fastcall_0063f9d0(void* module, void*, std::uint32_t object_id, std::uint32_t flag); // CSWCModule::SetCameraFollow
}

// 005F5CF0 CClientExoAppInternal::AttachPlayerGameObjectToArea() (thiscall, RET 0).
extern "C" void __fastcall kotor_fastcall_005f5cf0(void* client, void*) noexcept(false) {
    
    U32 id = lw(client, 0x20);                                                               // 005F5D09
    void* creature;
    if (lw(client, 0x3BC) == id) {                                                           // 005F5D0C..005F5D13
        creature = lp(client, 0x3C0);                                                        // 005F5D15
    } else {
        creature = pt(frame_R4_005ed5b0(client_app(), nullptr, id));                        // 005F5D1D..005F5D27
        sw(client, 0x3C0, ad(creature));                                                     // 005F5D2F
        sw(client, 0x3BC, lw(client, 0x20));                                                 // 005F5D2C..005F5D35
    }
    if (creature == nullptr) {  return; }                                                         // 005F5D3D..005F5D3F
    vcall<void, void*>(creature, 0x94, lp(lp(lp(client, 0x18), 0x48), 0x184));               // 005F5D45..005F5D56 AttachToScene(scene)
    
    if (kotor_cell_007a2484 != 0) {                                                          // 005F5D5C..005F5D63
        if (lw(creature, 0x310) != 0) {                                                      // 005F5D65..005F5D6D (jbe)
            sw(creature, 0x314, 1);                                                          // 005F5D74
            sw(creature, 0x318, 1);                                                          // 005F5D7A
        }
    } else {
        kotor_fastcall_0063f9d0(lp(client, 0x18), nullptr, lw(creature, 4), 1);             // 005F5D82..005F5D8B
    }
    if (void* control = lp(client, 0x2A0)) {                                                 // 005F5D90..005F5D98
        kotor_fastcall_00679340(control, nullptr, lw(creature, 4));                          // 005F5D9A..005F5D9E
        kotor_fastcall_006793b0(lp(client, 0x2A0), nullptr, lp(lp(client, 0x18), 0x40));     // 005F5DA3..005F5DB0
        
        return;
    }
    void* memory = live_operator_new(0xCC);                                                  // 005F5DC6..005F5DCB
    void* made = nullptr;
    if (memory != nullptr) {                                                                 // 005F5DD7
        try {                                                                                // SEH state 0 (FrameHandler_0072656b)
            made = kotor_fastcall_00679780(memory, nullptr, lw(creature, 4), lp(lp(client, 0x18), 0x40)); // 005F5DE3..005F5DF0
        } catch (...) {
            live_free(memory);                                                               // unwind funclet: operator delete
            throw;
        }
    }
    sw(client, 0x2A0, ad(made));
                                                                 // 005F5DF7..005F5DF9
}

// 005ED720 CClientExoApp::AttachPlayerGameObjectToArea() (thiscall, RET 0): mov ecx,[ecx+4]; jmp 005F5CF0.
extern "C" void __fastcall kotor_fastcall_005ed720(void* app, void*) noexcept(false) {
    kotor_fastcall_005f5cf0(lp(app, 4), nullptr);
}
