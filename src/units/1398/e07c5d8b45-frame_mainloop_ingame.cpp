// Lane W4 G12: in-game continuations of 00602EB0 CClientExoAppInternal::MainLoop.
// Source of truth: original swkotor.exe disassembly dis/00602EB0.asm (each statement cites its original PCs).
// Register model of the original at every entry here: esi = client (CClientExoAppInternal*), ebx = 0, ebp = 1
// (region B) or ebp = party character 0 (region G). These continuations are called by the existing owner
// (frame_driver_loading.cpp / menu_r2.cpp patches, see owner-patch/) in place of the former OutsideContract
// exits 00602F51, 006035A6, 006038D6 and 00603AC5.
#include "27e3c46f56-w4_frame.hpp"

namespace w4 {
float profile_ai_update_00832908 = 0.0f;   // [00832908], written only at 00603A24
U32 anim_queue_flag_00832c50 = 0;          // [00832C50], written only at 006035CD
}

using namespace w4;

// ------------------------------------------------------------------------------------------------------------
// Region B: 00602F51..00603253.
extern "C" void __fastcall w4_mainloop_area_entry_00602f51(void* client, void*, U32* stack20) {
    void* module = lp(client, 0x18);
    if (kotor_fastcall_00641980(module, nullptr) == 0 && lw(client, 0x338) != 0x7F000000u) {  // 00602F51..00602F67
        kotor_fastcall_005fb6e0(client, nullptr);                                            // 00602F69 CrownFutureLeader
        return;                                                                              // 00602F70 jmp 00603254
    }
    if (kotor_fastcall_00641980(lp(client, 0x18), nullptr) != 0) {                          // 00602F75..00602F82
        kotor_fastcall_006200e0(client, nullptr, 1, 1);                                      // 00602F84 SetInputClass(1,1)
    } else {
        kotor_fastcall_006200e0(client, nullptr, 0, 1);                                      // 00602F8C SetInputClass(0,1)
        if (void* control = lp(client, 0x2A0)) kotor_fastcall_006792e0(control, nullptr, 1); // 00602F92..00602F9D
        if (void* in_game = lp(client, 0x40)) frame_R4_0062aa00(in_game, nullptr, 1, 1);     // 00602FA2..00602FAB
        kotor_m1_opt_in_game_initialize_map_0062b0a0(lp(client, 0x40), nullptr);            // 00602FB0..00602FB3
    }
    kotor_fastcall_0061f7d0(client, nullptr);                                                // 00602FB8 EnableInput
    {
        U32 name[2];                                                                         // [esp+30h] CExoString
        kotor_fastcall_005ee3e0(lp(client, 0x18), nullptr, name);                            // 00602FBF..00602FC7
        frame_LogStringDtor_005e5c20(name, nullptr);                                         // 00602FCC..00602FD0
    }
    {
        void* movie = lp(client, 0x12C);                                                     // 00602FD5
        bool hide = movie != nullptr && kotor_fastcall_0070c1f0(movie, nullptr) != 0;        // 00602FDB..00602FE6
        if (!hide) hide = kotor_cell_render_active_007a3a38 != 0;                            // 00602FE8..00602FEE
        if (hide) kotor_m15_client_internal_hide_load_screen_005f6e20(client, nullptr, 1, 1); // 00602FF0..00602FF4
    }
    {
        void* movie = lp(client, 0x12C);                                                     // 00602FF9
        if (movie != nullptr && kotor_fastcall_0070c1f0(movie, nullptr) == 1)                // 00602FFF..0060300A
            lane_config_required::WaitForMovie_005f6f20(static_cast<options_video::Client*>(client), nullptr, 1); // 0060300C
        else
            sw(client, 0x2DC, 0);                                                            // 00603016
    }
    if (kotor_fastcall_00641980(lp(client, 0x18), nullptr) == 0)                            // 0060301C..00603026
        kotor_fastcall_0061fa00(client, nullptr, 1);                                         // 00603028 ShowMouse(1)
    kotor_cdecl_00403de0();                                                                  // 00603030
    kotor_fastcall_0070c220(lp(client, 0x12C), nullptr);                                     // 00603035 HideFadePanel
    if (kotor_cell_render_active_007a3a38 == 0) return;                                      // 00603040..00603046
    kotor_m15_client_internal_hide_load_screen_005f6e20(client, nullptr, 1, 1);              // 0060304C..00603050
    kotor_fastcall_004adff0(lp(client, 0x28), nullptr);                                      // 00603055..00603068 Pause
    kotor_fastcall_004adff0(lp(client, 0x2C), nullptr);
    kotor_fastcall_004adff0(lp(client, 0x30), nullptr);
    kotor_fastcall_004adef0(lp(client, 0x28), nullptr, lp(client, 0x24));                   // 0060306D..0060308C Reset
    kotor_fastcall_004adef0(lp(client, 0x2C), nullptr, lp(client, 0x24));
    kotor_fastcall_004adef0(lp(client, 0x30), nullptr, lp(client, 0x24));
    frame_R4_004adbd0(lp(client, 0x24), nullptr);                                            // 00603091..006030AC Snapshot
    frame_R4_004adbd0(lp(client, 0x28), nullptr);
    frame_R4_004adbd0(lp(client, 0x2C), nullptr);
    frame_R4_004adbd0(lp(client, 0x30), nullptr);
    static const U32 order[4] = {0x2Cu, 0x30u, 0x28u, 0x24u};
    for (U32 off : order) {                                         // 006030B1..006030EE
        void* timer = lp(client, off);
        const U64 t = kotor_fastcall_004adc70(timer, nullptr);
        sw(timer, 0x14, static_cast<U32>(t));
        sw(timer, 0x18, static_cast<U32>(t >> 32));
    }
    kotor_fastcall_004ae030(lp(client, 0x24), nullptr);                                      // 006030F1..0060310C Unpause
    kotor_fastcall_004ae030(lp(client, 0x28), nullptr);
    kotor_fastcall_004ae030(lp(client, 0x2C), nullptr);
    kotor_fastcall_004ae030(lp(client, 0x30), nullptr);
    frame_R4_004adbd0(kotor_fastcall_004aede0(server_app(), nullptr), nullptr);              // 00603111..00603121
    kotor_fastcall_004ae030(kotor_fastcall_004aede0(server_app(), nullptr), nullptr);        // 00603126..00603136
    if (kotor_fastcall_004aed20(server_app(), nullptr) == 0) {                               // 0060313B..0060314A
        kotor_fastcall_00604d70(lp(lp(client, 0x18), 0x48), nullptr);                        // 00603150..00603156 StartSounds
        void* in_game = pt(frame_R4_005ed690(client_app(), nullptr));                        // 0060315B..00603164
        if (lw(in_game, 0xB4) == 0 &&
            lw(pt(frame_R4_005ed690(client_app(), nullptr)), 0xB98) == 0) {                  // 00603169..00603184
            U32 colour[3] = {0, 0, 0};                                                       // 0060319B..006031AB
            kotor_m15_ingame_start_global_fade_0062abf0(pt(frame_R4_005ed690(client_app(), nullptr)), nullptr,
                                                       1, 0.5f, 1.0f, colour);               // 00603186..006031BE
        } else {
            sw(pt(frame_R4_005ed690(client_app(), nullptr)), 0xB98, 1);                      // 006031C5..006031D2
        }
        kotor_fastcall_005f3030(client, nullptr, 1);                                         // 006031D8..006031DB
        kotor_fastcall_005ed310(kotor_menu_window_cells::input_007a39e4, nullptr);           // 006031E0..006031E6
        kotor_game_prepare::SetGlobalUnpauseableState0062b410(lp(client, 0x40), nullptr, 0); // 006031EB..006031EF
        frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr,
                          kotor_rdata_sound_mode_0074c5ec);                                  // 006031F4..00603201
    }
    if (lw(client, 0x9C) == 1 && kotor_fastcall_005f1420(client, nullptr) != nullptr) {     // 00603206..00603217
        vcall<void>(kotor_fastcall_005f1420(client, nullptr), 0x94, 0);                      // 00603219..00603225 AttachToScene(0)
        sw(client, 0x28C, lw(kotor_fastcall_005f1420(client, nullptr), 4));                  // 0060322B..0060323C
        kotor_fastcall_005f2bb0(client, nullptr, 0x7F000000u);                               // 00603235..00603242
    }

    *stack20 = 1;                                                                            // 00603248



    (void)kotor_cdecl_00401430(0);                                                           // 00603247..00603251
}

// ------------------------------------------------------------------------------------------------------------
// Region E: 006035A6..00603892.
extern "C" void __fastcall w4_mainloop_anim_queue_006035a6(void* client, void*) {
    const double countdown = lf(client, 0x16C);
    const double delta = frame_delta();



    if (jcc_jp(countdown, delta, 0x41)) {                                                    // 006035A6..006035B7

        sf(client, 0x16C, static_cast<float>(countdown - delta));                            // 00603881..0060388D
        return;
    }
    void* list = lp(client, 0x168);                                                          // 006035BD
    sw(client, 0x16C, 0);                                                                    // 006035C3
    if (lw(list, 0) == 0) {                                                                  // 006035C9..006035CB
        anim_queue_flag_00832c50 = 0;                                                        // 006035CD
        return;                                                                              // 006035D3 jmp 00603893
    }
    // 006035D8..0060387F: non-empty debug animation queue. No AddHead/AddTail/AddBefore of client+168h exists in
    // the original image (MAINLOOP-PATHS.md), so this arm is unreachable in retail; kept as an explicit trap.
    kotor_unreachable_trap(0x006035D8u, "CClientExoAppInternal::MainLoop@006035D8 animation test queue non-empty");
}

// ------------------------------------------------------------------------------------------------------------
// Region G: 006038D6..00603AA3.
extern "C" void __fastcall w4_mainloop_world_tick_006038d6(void* client, void*, void* character, void* area) {
    const U32 sounds = lw(client, 0x26C);                                                    // 006038D6
    if (sounds == 0) {
        if (lw(client, 0x188) != 0) sw(client, 0x26C, 1);                                    // 006038E0..006038F2
    } else if (lw(client, 0x188) == 0) {                                                     // 006038F8..006038FE
        kotor_fastcall_00604d70(area, nullptr);                                              // 00603900 StartSounds
        sw(client, 0x26C, 0);                                                                // 00603907
    }
    void* scene = lp(area, 0x184);                                                           // 0060390D
    if (scene != nullptr && character != nullptr) {                                          // 00603913..00603919
        W4Vector listener;
        if (lw(lp(client, 0x40), 0xB4) == 0) {                                               // 0060391B..00603924
            std::memcpy(&listener, static_cast<U8*>(character) + 0x24, 12);                  // 00603926..0060393D
        } else {
            W4Vector tmp;                                                                    // [esp+30h]
            listener = *kotor_fastcall_0062ed30(lp(client, 0x40), nullptr, &tmp, 1);         // 00603947..00603967
        }
        vcall<void, W4Vector>(scene, 0x48, listener);                                        // 00603940/0060396C
    }
    const U32 panel = frame_ContainsPanel_0040bd70(lp(client, 0x274), nullptr,
                                                   lp(lp(client, 0x40), 0xA0));              // 00603971..00603981
    bool turn;
    if (frame_R4_005ed480(client_app(), nullptr) == 0 && lw(client, 0x9C) != 1) {           // 00603986..006039A1
        turn = panel != 0;                                                                   // 006039A3..006039A5
        if (!turn) kotor_fastcall_005f5e10(client, nullptr, frame_delta());                  // 006039A7..006039B0
    } else {



        turn = panel != 0;                                                                   // 006039B7..006039B9

    }
    if (turn) {
        kotor_fastcall_0063fe10(lp(client, 0x18), nullptr, 0, 0);                            // 006039BB..006039C0
        kotor_fastcall_00640090(lp(client, 0x18), nullptr, 0, 0);                            // 006039C5..006039CA
    }
    {
        const U64 t = high_resolution_timer();                                               // 006039CF..006039D7
        menu_continuation::high_resolution_low_008328f0 = static_cast<U32>(t);               // 006039DF
        menu_continuation::high_resolution_high_008328f4 = static_cast<U32>(t >> 32);        // 006039E4
    }
    kotor_fastcall_006048c0(lp(client, 0x0C), nullptr);                                      // 006039DC..006039EA UpdateState
    {
        const U64 t = high_resolution_timer();                                               // 006039EF..006039F8
        frame_control::end_timer_high_008328ec = static_cast<U32>(t >> 32);                  // 006039FD
        frame_control::end_timer_low_008328e8 = static_cast<U32>(t);                         // 00603A09
        const I32 diff = static_cast<I32>(static_cast<U32>(t) - menu_continuation::high_resolution_low_008328f0);
        profile_ai_update_00832908 =
            static_cast<float>(static_cast<double>(diff) * frame_control::profile_scale_0073d708); // 00603A1A..00603A24
    }
    void* options = kotor_fastcall_005ed700(client_app(), nullptr);                         // 00603A2A..00603A2D
    if (lb(options, 0x6D) == 7 && kotor_fastcall_005ed530(client_app(), nullptr) != nullptr &&
        lw(kotor_fastcall_005ed530(client_app(), nullptr), 0x40) != 0) {                     // 00603A32..00603A5A
        void* camera = lp(kotor_fastcall_005ed530(client_app(), nullptr), 0x40);             // 00603A5C..00603A6A
        W4Vector tmp;                                                                        // [esp+30h]
        W4Vector* position = vcall<W4Vector*, W4Vector*>(camera, 0x20, &tmp);                // 00603A6D..00603A74 GetPosition
        kotor_fastcall_005ee860(client, nullptr, position);                                  // 00603A9C..00603A9F
        return;
    }
    if (character == nullptr) return;                                                        // 00603A79..00603A7B
    if (lw(lp(client, 0x40), 0xB4) == 0) {                                                   // 00603A7D..00603A86
        kotor_fastcall_005ee860(client, nullptr, static_cast<U8*>(character) + 0x1DC);       // 00603A88..00603A9F
    } else {
        W4Vector tmp;                                                                        // [esp+30h]
        kotor_fastcall_005ee860(client, nullptr, kotor_fastcall_0062ed30(lp(client, 0x40), nullptr, &tmp, 0)); // 00603A91..00603A9F
    }
}

// ------------------------------------------------------------------------------------------------------------
// Region H: 00603AC5..00603B15.
extern "C" void __fastcall w4_mainloop_server_dialog_00603ac5(void* client, void*, U32 dialog_id) {
    (void)client;
    const U32 server_id = kotor_fastcall_004aea30(server_app(), nullptr, dialog_id);        // 00603AC5..00603AC6
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, server_id);               // 00603ACB..00603AD5
    if (object == nullptr) return;                                                           // 00603ADC
    if (vcall<void*>(object, 0x10) == nullptr) return;                                       // 00603AE0..00603AE9
    kotor_fastcall_004cd580(vcall<void*>(object, 0x10), nullptr);                            // 00603AEB..00603AF4
    if (lw(vcall<void*>(object, 0x10), 0x214) == 0) return;                                  // 00603AF9..00603B06
    recovered_object_delete_dialog_004cc330(vcall<void*>(object, 0x10), nullptr);                            // 00603B08..00603B11
}
