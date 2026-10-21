// lane-w3 unit w3-client-module: client Login / Module message handlers and the CSWCModule load they drive.
//   0064EC00 CSWCMessage::HandleServerToPlayerLogin            (major 0x02)
//   006684F0 CSWCMessage::HandleServerToPlayerModule           (major 0x03)
//   0063F660 CSWCModule::LoadModule
//   005ED440 CClientExoApp::CreateModule / 005EEA20 CClientExoAppInternal::CreateModule
//   005ED730 CClientExoApp::InitializeInGameGUI / 005F1410 CClientExoAppInternal::InitializeInGameGUI
//   00677490 CSWCMessage::SendPlayerToServerModule_ModuleLoaded
//   005ED570 CClientExoApp::GetGameObjectArray
// Original: swkotor.exe GOG K1; every statement cites the original instruction(s). Exception frames (SEH state
// bytes) only destroy CExoString / CResRef temporaries; C++ locals reproduce that cleanup in the original order.
#include "dd06b37d22-w3_client.h"
#include "73d01d13f8-w3_client_internal.h"

using namespace kotor_w3;

namespace {
const char kEmpty_0073d71c[] = "";                                    // .rdata 0073D71C
const char kCamera_0073e458[] = "camera";                             // .rdata 0073E458
const char kHak_00751834[] = "HAK:";                                  // .rdata 00751834
constexpr U kPlayerIdServer_0074c528 = 0xFFFFFFFDu;                   // .rdata 0074C528 (server player id)
}  // namespace

// .bss 008328FC: set to 1 by HandleServerToPlayerLogin minor 5 (0064EC8B), read by HandleServerToPlayerModule
// (00668528). No other instruction references it (lane-w3 tools/xref.py). Single owner: lane-w3.
extern "C" volatile U recovered_client_login_confirmed_008328fc = 0;

extern "C" {
// (t2, coordinator decision after OVERLAP-W4.md) 005ED570 GetGameObjectArray dropped: single owner lane-w4 `kotor_fastcall_005ed570`.

// 005EEA20 CClientExoAppInternal::CreateModule: new CSWCModule (138h bytes) into [this+18h]. ECX internal, RET.
void* __fastcall recovered_client_internal_create_module_005eea20(void* internal, void*) noexcept(false) {
    void* const memory = live_operator_new(0x138);                    // 005EEA37..005EEA3E
    void* module = nullptr;                                           // 005EEA70..005EEA74 (null allocation)
    if (memory != nullptr)                                            // 005EEA4A..005EEA54
        module = recovered_bridge_module_construct_00643f40(memory, nullptr);   // 005EEA56..005EEA58 CSWCModule::CSWCModule
    st<void*>(internal, 0x18, module);                                // 005EEA5D / 005EEA76
    return module;
}

// 005ED440 CClientExoApp::CreateModule: tail call on [this+4]. ECX app, RET.
void* __fastcall recovered_client_create_module_005ed440(void* app, void*) noexcept(false) {
    return recovered_client_internal_create_module_005eea20(rp(app, 4), nullptr);   // 005ED440..005ED443
}

// (t2, coordinator decision after OVERLAP-W4.md) 005F1410 CClientExoAppInternal::InitializeInGameGUI and 005ED730 CClientExoApp::InitializeInGameGUI dropped: single owner lane-w4 `kotor_fastcall_005f1410` / `kotor_fastcall_005ed730`.

// 00677490 CSWCMessage::SendPlayerToServerModule_ModuleLoaded: 3-byte message 'p' 03 02 to the server player.
// ECX this (unused), RET. EAX = CNetLayer::SendMessageToPlayer result, or the null allocation.
U __fastcall recovered_client_send_module_loaded_00677490(void* /*self*/, void*) noexcept(false) {
    unsigned char* const message = static_cast<unsigned char*>(live_operator_new(3));   // 00677491..00677498
    if (message == nullptr) return 0;                                 // 0067749D..006774A2 (EAX = 0)
    message[0] = 0x70;                                                // 006774A4 'p'
    message[1] = 3;                                                   // 006774A7 major Module
    message[2] = 2;                                                   // 006774AB minor ModuleLoaded
    void* const app = client_app();                                   // 006774AF..006774B4
    // 006774BD..006774C5: CClientExoApp vtable +10h = GetNetLayer() (no arguments).
    void* const net = vslot<void* (__fastcall*)(void*, void*)>(app, 0x10)(app, nullptr);
    const U result = net_layer_send_message_to_player_005d4cb0(net, nullptr, kPlayerIdServer_0074c528,
                                                               message, 3, 0);   // 006774B7..006774CA
    live_free(message);                                               // 006774CF..006774D2
    return result;                                                    // 006774DA
}

// 0063F660 CSWCModule::LoadModule: reads the module record of the Module/LoadModule message (major 3 minor 1)
// into this CSWCModule, creates its CSWCArea (+48h) and camera (+40h), and starts the load screen.
// ECX module, RET (no stack arguments). EAX = 1 on success, 0 on failure (area/camera released).
U __fastcall recovered_client_module_load_module_0063f660(void* self, void*) noexcept(false) {
    void* const message = client_message();                           // 0063F67E..0063F68B GetSWCMessage
    void* const memory = live_operator_new(0x284);                    // 0063F690..0063F697
    void* area = nullptr;                                             // 0063F6B8
    if (memory != nullptr)                                            // 0063F6A3..0063F6AD
        area = recovered_client_area_construct_00606090(memory, nullptr);   // 0063F6AF..0063F6B1
    st<void*>(self, 0x48, area);                                      // 0063F6C3
    if (area == nullptr) return 0;                                    // 0063F6C6 -> 0063F9B3
    if (rp(self, 0x40) == nullptr)                                    // 0063F6CC..0063F6D1
        st<void*>(self, 0x40, new_aur_camera_0045d930(kCamera_0073e458));   // 0063F6D3..0063F6E0
    void* const camera = rp(self, 0x40);                              // 0063F6E3
    if (camera == nullptr) {                                          // 0063F6E6..0063F6E8
        void* const area_now = rp(self, 0x48);                        // 0063F6EA
        if (area_now != nullptr) delete_object(area_now);             // 0063F6ED..0063F6F5 [vtbl+0](1)
        st<void*>(self, 0x48, nullptr);                               // 0063F6F7
        return 0;                                                     // 0063F6FE
    }
    // 0063F712..0063F71E camera vtable +40h SetClipDist(0.1f, 10000.0f)
    vslot<void (__fastcall*)(void*, void*, float, float)>(camera, 0x40)(camera, nullptr, fbits(0x3DCCCCCDu),
                                                                         fbits(0x461C4000u));
    menu_tables_m41::SetCameraMode_0061b6f0(kotor_fastcall_005ed700(client_app(), nullptr), nullptr, 3);   // 0063F721..0063F733
    {
        ExoString name_tmp;                                           // {E-0x44}
        kotor_fastcall_004d67e0(message, nullptr, &name_tmp, 0x20);   // 0063F738..0063F741
        async_assign_string_005e5c50(at(self, 0x38), nullptr, &name_tmp);   // 0063F746..0063F756 [this+38h] module name
        frame_LogStringDtor_005e5c20(&name_tmp, nullptr);             // 0063F75B..0063F763
    }
    void* const name_field = at(self, 0x38);                          // 0063F752 kept in {E-0x50}
    {
        ResRef sound_tmp;                                             // {E-0x24}
        kotor_fastcall_00405ed0(&sound_tmp, nullptr);                 // 0063F768..0063F76E
        ExoString text_tmp;                                           // {E-0x44}
        kotor_fastcall_00692660(message, nullptr, &text_tmp, &sound_tmp, nullptr);   // 0063F773..0063F77B
        async_assign_string_005e5c50(at(self, 0x10), nullptr, &text_tmp);   // 0063F780..0063F78C [this+10h]
        frame_LogStringDtor_005e5c20(&text_tmp, nullptr);             // 0063F791..0063F799
    }
    st<U8>(self, 0xA0, kotor_fastcall_004d6720(message, nullptr, 8));   // 0063F79E..0063F7B0 minutes/hour
    {
        ResRef read_tmp, copy_tmp;                                    // {E-0x24} / {E-0x44}
        kotor_fastcall_004d6180(message, nullptr, &read_tmp, 0x10);   // 0063F7A7..0063F7B9
        c2da_copy_00406120(at(self, 0x18), nullptr, &copy_tmp, &read_tmp);   // 0063F7BE..0063F7C6 [this+18h]
        kotor_fastcall_004d6180(message, nullptr, &read_tmp, 0x10);   // 0063F7CB..0063F7D4
        c2da_copy_00406120(at(self, 0x28), nullptr, &copy_tmp, &read_tmp);   // 0063F7D9..0063F7E2 [this+28h]
    }
    recovered_bridge_world_timer_set_minutes_per_hour_004adba0(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr,
                                                               ld<U8>(self, 0xA0));   // 0063F7E7..0063F800
    if (c2da_not_equal_text_00406060(at(self, 0x18), nullptr, kEmpty_0073d71c) != 0) {   // 0063F805..0063F813
        ExoString hak;                                                // {E-0x4c}
        kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&hak), nullptr);   // 0063F819..0063F81D
        c2da_assign_text_005e5140(&hak, nullptr, kHak_00751834);     // 0063F822..0063F833
        {
            ExoString part, joined;                                   // {E-0x2c} / {E-0x34}
            resman_string_counted_005e5b70(&part, nullptr,
                static_cast<const char*>(at(self, 0x18)), 0x10);      // 0063F838..0063F846 self_return(+18h), 16
            GuiConcat005e5d10(reinterpret_cast<kotor_reconstruction::StartupString*>(&hak), nullptr,
                              reinterpret_cast<kotor_reconstruction::StartupString*>(&joined),
                              reinterpret_cast<const kotor_reconstruction::StartupString*>(&part));   // 0063F84B..0063F85B
            async_assign_string_005e5c50(&hak, nullptr, &joined);    // 0063F860..0063F86A
            frame_LogStringDtor_005e5c20(&joined, nullptr);          // 0063F86F..0063F877
            frame_LogStringDtor_005e5c20(&part, nullptr);            // 0063F87C..0063F885
        }
        const U added = resource_manager_add_encapsulated(&hak, 2);   // 0063F88A..0063F897
        if (added == 0) {                                             // 0063F89C..0063F89E
            void* const cam = rp(self, 0x40);                         // 0063F8A0
            if (cam != nullptr) delete_object(cam);                   // 0063F8A5..0063F8AD
            void* const area_now = rp(self, 0x48);                    // 0063F8AF
            st<void*>(self, 0x40, nullptr);                           // 0063F8B4
            if (area_now != nullptr) delete_object(area_now);         // 0063F8B2..0063F8BD
            st<void*>(self, 0x48, nullptr);                           // 0063F8C3
            frame_LogStringDtor_005e5c20(&hak, nullptr);              // 0063F8BF..0063F8CE
            return 0;                                                 // 0063F8D3
        }
        frame_LogStringDtor_005e5c20(&hak, nullptr);                  // 0063F8E7..0063F8F3
    }
    if (recovered_player_message::overrun_004d6230(message) != 0 ||   // 0063F8F8..0063F903
        recovered_player_message::underrun_004d6240(message) != 0) {  // 0063F909..0063F914
        void* const cam = rp(self, 0x40);                             // 0063F991
        if (cam != nullptr) delete_object(cam);                       // 0063F996..0063F99E
        void* const area_now = rp(self, 0x48);                        // 0063F9A0
        st<void*>(self, 0x40, nullptr);                               // 0063F9A5
        if (area_now != nullptr) delete_object(area_now);             // 0063F9A3..0063F9AE
        st<void*>(self, 0x48, nullptr);                               // 0063F9B0
        return 0;                                                     // 0063F9B9
    }
    kotor_m15_client_set_load_screen_by_module_005edcf0(client_app(), nullptr, name_field);   // 0063F916..0063F924
    kotor_m15_client_show_load_screen_005eda80(client_app(), nullptr, 1, 1, 0);   // 0063F929..0063F938
    void* const app = client_app();                                   // 0063F93D..0063F942
    const U step0 = client_load_step(app, 0);                         // 0063F945..0063F954 GetLoadStep(0)
    const U step1 = client_load_step(app, 1);                         // 0063F950..0063F95C GetLoadStep(1)
    kotor_m15_client_set_load_bar_progress_005edd40(app, nullptr, static_cast<I32>(step0 + step1), 1);   // 0063F95F..0063F964
    kotor_cdecl_00401c10(fbits(0x3D088889u), 0, 0);                   // 0063F969..0063F977 UpdateScreen(1/30, 0, 0)
    return 1;                                                         // 0063F97A
}

// 006684F0 CSWCMessage::HandleServerToPlayerModule(BYTE minor). ECX this, RET 4. EAX = handled.
U __fastcall recovered_client_handle_module_006684f0(void* self, void*, U minor) noexcept(false) {
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 006684F2..006684FD
    switch (static_cast<U8>(minor)) {                                 // 00668503..00668512 (jump table 00668604)
    case 1: {                                                         // 00668519
        void* module = kotor_fastcall_005ed530(client_app(), nullptr);    // 00668519..00668526 GetModule
        if (recovered_client_login_confirmed_008328fc == 0) {         // 00668528..0066852F
            kotor_fastcall_004d6170(self, nullptr);     // 00668531..00668533
            return 1;                                                 // 00668538..0066853F
        }
        if (module == nullptr)                                        // 00668542..00668544
            module = recovered_client_create_module_005ed440(client_app(), nullptr);   // 00668546..00668554
        kotor_fastcall_004015a0(module, nullptr);                     // 00668556..00668558 void_return
        if (recovered_client_module_load_module_0063f660(module, nullptr) == 0) return 0;   // 0066855D..00668566
        kotor_fastcall_005ed730(client_app(), nullptr);   // 0066856C..00668575
        kotor_fastcall_004015a0(frame_R4_005ed690(client_app(), nullptr), nullptr);   // 0066857A..00668589 GetInGameGui; void_return
        return recovered_client_send_module_loaded_00677490(self, nullptr);   // 0066858E..00668597
    }
    case 3: return recovered_client_handle_module_update_time_006528b0(self, nullptr);         // 0066859A..006685A3
    case 6: return 1;                                                 // 006685D8..006685DF
    case 8:                                                           // 006685CA
        recovered_client_unload_module_005ed430(client_app(), nullptr);   // 006685CA..006685D3
        return 1;                                                     // 006685D9
    case 10: return recovered_client_handle_export_reply_006634a0(self, nullptr);              // 006685BE..006685C7
    case 11: return recovered_client_handle_start_new_module_start_00652840(self, nullptr);    // 006685A6..006685AF
    case 12: return recovered_client_handle_start_new_module_end_00652860(self, nullptr);      // 006685B2..006685BB
    case 13: return recovered_client_handle_save_game_status_006529f0(self, nullptr);          // 006685E2..006685EB
    case 14: return recovered_client_handle_end_game_00652ad0(self, nullptr);                  // 006685EE..006685F7
    default: return 0;                                                // 006685FA..006685FE
    }
}

// 0064EC00 CSWCMessage::HandleServerToPlayerLogin(BYTE minor). ECX this, RET 4. EAX = handled.
// Minor 5 (login confirmed) sets the 008328FC gate read by the Module handler.
U __fastcall recovered_client_handle_login_0064ec00(void* self, void*, U minor) noexcept(false) {
    ExoString unused_24, unused_2c;                                   // {E-0x24} / {E-0x2c}
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&unused_24), nullptr);   // 0064EC23..0064EC27
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&unused_2c), nullptr);   // 0064EC2C..0064EC38
    U handled = 0;
    if (recovered_player_message::overrun_004d6230(self) == 0) {      // 0064EC3D..0064EC4D
        switch (static_cast<U8>(minor)) {                             // 0064EC53..0064EC6A (tables 0064EDA0/0064EDB8)
        case 5:                                                       // 0064EC71
            if (recovered_player_message::underrun_004d6240(self) == 0) {   // 0064EC71..0064EC85
                recovered_client_login_confirmed_008328fc = 1;        // 0064EC8B
                handled = 1;
            }
            break;
        case 6:                                                       // 0064EC9A
            handled = recovered_player_message::underrun_004d6240(self) == 0 ? 1u : 0u;   // 0064EC9A..0064ECAE
            break;
        case 10: {                                                    // 0064ECB6 character list
            // The original stores the n DWORD/BYTE pairs into a fixed 12-byte / 4-byte stack area that is never read
            // (an n > 3 list overruns its frame). The values are read in the original order and discarded here.
            const U8 n = kotor_fastcall_004d6720(self, nullptr, 8);   // 0064ECB6..0064ECC1 (jbe: n == 0 skips)
            for (U i = 0; i < n; ++i) {                               // 0064ECD2..0064ECF5
                (void)kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064ECD2..0064ECDF
                (void)kotor_fastcall_004d6720(self, nullptr, 8);       // 0064ECDB..0064ECE6
            }
            (void)kotor_fastcall_004d6780(self, nullptr, 0x20);       // 0064ECF7..0064ECFB
            handled = 1;                                              // 0064ED00 -> 0064ED10
            break;
        }
        case 16:                                                      // 0064ED02 void_return_this+4 (005ED310)
            kotor_fastcall_004015a0(rp(client_app(), 4), nullptr);    // 0064ED02..0064ED0B
            handled = 1;
            break;
        case 18:                                                      // 0064ED48
            (void)kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064ED48..0064ED4C
            handled = (recovered_player_message::overrun_004d6230(self) == 0 &&   // 0064ED51..0064ED5C
                       recovered_player_message::underrun_004d6240(self) == 0) ? 1u : 0u;   // 0064ED5E..0064ED69
            break;
        default:                                                      // 0064ED6B
            break;
        }
    }
    frame_LogStringDtor_005e5c20(&unused_2c, nullptr);                // 0064ED19 / 0064ED74
    frame_LogStringDtor_005e5c20(&unused_24, nullptr);                // 0064ED1E..0064ED2A / 0064ED79..0064ED85
    return handled;                                                   // 0064ED2F / 0064ED90
}
}  // extern "C"
