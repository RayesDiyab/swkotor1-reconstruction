// CClientExoAppInternal::HandleInputEvent 00621210 full body, adapted from the lane-input r3 donor
// client_input_event.cpp (feaee158..., 469 lines, 0 traps). The dispatch is table 00622128 for ids 1..0x0B, direct 0x0F,
// byte table 006221FC -> dword table 00622154 for 0x19..0x108, common exit 0062211C. The donor's original-address
// comments are retained. This is the full donor body; the earlier lane-m48-traps fork's D83 case-entry traps are gone.
// The inherited PerformLButtonUp world-click traps at 006205F1/00620614 are restored by this successor to calls to
// the owned full HandleMouseClickInWorld 00620350 body below; 00621210 and its 64 cases are unchanged.
// The mouse-button actions 0061F880 / 00620530 / 0061F940 / 0061F960, IsTargetActionMenuControl 00684ED0,
// ShowSWInGameGui 0062C9B0 and SetToolTipInstantMode 005F4BE0 remain as selected providers/definitions.
#include "7eeea93bc2-client_input_event.h"
#include "3884bf8e1d-window_shared_cells.h"
#include "13bda7cf95-kotor_unreachable_trap.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

using namespace kotor_input_hover;
namespace req = kotor_input_required;
namespace cel = kotor_input_cells_required;
using kotor_input_client::DefaultAction;
using kotor_input_client::GameObject;
using kotor_input_client::GuiMessageBox;

// Authored in this fork (process_input.cpp / gui_manager_frame.cpp), lane-m48-traps extern "C" convention.
extern "C" int __fastcall kotor_fastcall_005f2ed0(void* self, void*);                 // GetControlledCharacterDead
extern "C" int __fastcall kotor_fastcall_0040c610(Manager* self, void*);              // CSWGuiManager::HandleRMouseDown
extern "C" int __fastcall kotor_fastcall_0040a1a0(Manager* self, void*);              // CSWGuiManager::HandleRMouseUp
extern "C" int __fastcall kotor_fastcall_0040c650(Manager* self, void*, int delta);   // CSWGuiManager::HandleZMouse
// Existing composed-link providers (exact defining declarations).
extern "C" void __fastcall kotor_fastcall_005df4e0(void* input, void*);               // CExoInput::ClearEvents (tail 005E0710)
extern "C" void __fastcall frame_R4_0062aa00(void* gui, void*, unsigned, unsigned);   // CGuiInGame::SetSWGuiStatus RET8
extern "C" void __fastcall kotor_fastcall_0061fa00(void* self, void*, int);           // CClientExoAppInternal::ShowMouse RET4
extern "C" void __fastcall kotor_fastcall_005df5b0(void* input, void*, std::uint32_t value); // CExoInput::RefreshMouseCursor (routes window_leaves.cpp:49; EAX unused here)
extern "C" int __cdecl format_006fadb0(char*, const char*, ...);                       // _sprintf 006FADB0 (tpc-joins provider)
namespace menu_tables_m41 { void __fastcall GuiSetSoftwareMouseCursor_0040a270(void*, void*, unsigned, unsigned, unsigned); }

// ======== typed binding-closure block (private 0769 successor) ========
// Binding-only extern-C declarations for the selected real providers in the 45-row plan.
// No original bodies, storage ownership, numeric aliases, stubs or traps are introduced.
extern "C" void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, std::uint32_t, std::uint32_t);
extern "C" void __fastcall kotor_fastcall_0063d470(void* object, void*);
extern "C" void __fastcall cool_down_event_005df4b0(void* input, void*, std::uint32_t, std::uint32_t);
extern "C" void __fastcall kotor_fastcall_0068ad60(void* main_interface, void*, std::uint32_t, std::uint32_t);
extern "C" int __fastcall kotor_fastcall_005edef0(void* app, void*);
extern "C" int __fastcall kotor_fastcall_005ede70(void* app, void*);
extern "C" void __fastcall kotor_fastcall_00620620(void* self, void*);
extern "C" void* __fastcall kotor_fastcall_005ed580(void* app, void*, std::uint32_t id);
extern "C" int __fastcall kotor_fastcall_005edc10(void* app, void*);
extern "C" int __fastcall kotor_fastcall_005f2df0(void* self, void*);
extern "C" void* __fastcall kotor_fastcall_00635460(void* party, void*);
extern "C" void* __fastcall packed_file_get_async_file_00644760(void* self, void*);
extern "C" void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);
extern "C" void* __fastcall kotor_fastcall_005eda40(void* app, void*);
extern "C" int __fastcall frame_R4_0062cba0(void* gui, void*, std::uint32_t);
extern "C" int __fastcall kotor_fastcall_00610ac0(void* creature, void*);
extern "C" void __fastcall kotor_fastcall_005edba0(void* app, void*);
extern "C" void __fastcall kotor_fastcall_00627130(void* box, void*, std::uint32_t);
extern "C" void __fastcall kotor_fastcall_005edee0(void* app, void*, std::uint32_t, std::uint32_t);
extern "C" void __fastcall frame_SetAutoPaused_005f3f10(void* self, void*, std::uint32_t, std::uint32_t);
extern "C" void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, std::uint32_t, std::uint32_t, std::uint32_t);
extern "C" void __fastcall kotor_fastcall_00610a10(void* creature, void*, std::uint32_t);
extern "C" void __fastcall kotor_fastcall_006200e0(void* self, void*, std::uint32_t, std::uint32_t);
extern "C" void __fastcall kotor_fastcall_005f2e10(void* self, void*, std::uint32_t, std::uint32_t, std::uint32_t);
extern "C" int __fastcall show_tutorial_window_005edf40(void* app, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
namespace {
using U32 = std::uint32_t;
template<class T> T at(const void* p, U32 off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(T)); return v; }
template<class T> void put(void* p, U32 off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(T)); }
AppManager* app_manager() { return static_cast<AppManager*>(kotor_menu_window_cells::manager_007a39fc); }
void* input() { return kotor_menu_window_cells::input_007a39e4; }
void* in_game_gui(const ClientInternal* self) { return self->get<void*>(0x40); }
Manager* client_manager(const ClientInternal* self) { return static_cast<Manager*>(self->get<void*>(0x274)); }
const char kScreenshotName_0074fbfc[] = "KotOR%04i.tga";          // .rdata 0074FBFC (donor D13 extraction)
const char kScreenshotNameLetter_0074fbec[] = "KotOR%04i%c.tga";  // .rdata 0074FBEC
const char kReadBinary_0073d5b0[] = "rb";                         // .rdata 0073D5B0


// 0062148D..006214C9 / 0062198E..006219D8: enter GUI input class 2 (class-2 menu path: D1h..D8h).
void enter_gui_class(ClientInternal* self) {
    self->set<U32>(0x9C, 2);                                           // 006219A1
    kotor_fastcall_005df4e0(input(), nullptr);                         // 006219A7..AD
    void* gui = in_game_gui(self);                                     // 006219B2
    if (gui && at<U32>(gui, 0x34) == 1) {                              // 006219B5..BC
        frame_R4_0062aa00(gui, nullptr, 3, 1);                         // 006219BE..C1
        kotor_fastcall_0061fa00(self, nullptr, 1);                     // 006219C6..C9
    }
}

// 006219FF / 00621A3A: main interface vt+3C(id, value), no value gate (class-2 ids C8h/C9h; returns on the menu because
// CGuiInGame+34h != 1 and CGuiInGame+90h == 0).
void forward_to_main_interface(ClientInternal* self, int id, int value) {
    void* gui = in_game_gui(self);                                     // 006219FF
    if (!gui || at<U32>(gui, 0x34) != 1) return;                       // 00621A02..0E
    GuiObject* mi = at<GuiObject*>(gui, 0x90);                         // 00621A14
    if (!mi) return;                                                   // 00621A1A
    (mi->*(mi->vtable->handle_input_event))(id, value);                // 00621A22..2B
}

// 00621555..006215EB: screenshot file name probe (event 50h, class 2: reachable on the menu).
// 006FADB0 _sprintf -> tpc-joins format_006fadb0; 006FAD9D fopen / 006FACF0 fclose -> linked UCRT fopen/fclose (D69
// precedent: m46_scene_c1.cpp 00452BA4, m47_anim.cpp 004408DF; same "rb" open/close-only use, no stream I/O).
void probe_screenshot_name(ClientInternal* self) {
    char* name = reinterpret_cast<char*>(self) + 0x158;                // 00621566
    self->set<U32>(0x150, 1);                                          // 00621572
    format_006fadb0(name, kScreenshotName_0074fbfc, self->get<U32>(0x154));  // 0062155F..7C
    std::FILE* file = std::fopen(name, kReadBinary_0073d5b0);          // 00621581..87
    signed char letter = 'a';                                          // 00621591 MOV BL,61
    if (file) {
        for (;;) {
            if (letter > 'z') { self->set<U32>(0x150, 0); break; }     // 00621595..98 -> 006215CF
            std::fclose(file);                                         // 0062159A..9B
            format_006fadb0(name, kScreenshotNameLetter_0074fbec, self->get<U32>(0x154), static_cast<int>(letter));  // 006215A0..B1 (MOVSX)
            ++letter;                                                  // 006215BC
            file = std::fopen(name, kReadBinary_0073d5b0);             // 006215B6..BE
            if (!file) {                                               // 006215C6..C8
                if (letter > 'z') self->set<U32>(0x150, 0);            // 006215CA..CF
                break;
            }
        }
        // Observed: the handle that is open when 'z' is exceeded is not closed.
    }
    self->set<U32>(0x154, self->get<U32>(0x154) + 1);                  // 006215D5..DD
}
}

// ======== 0062C9B0 CGuiInGame::ShowSWInGameGui(int panel) thiscall RET4, EAX = [this+30h] ========
// Menu prefix: [this+108h] (in-game GUI initialized) is 0 on the menu - CGuiInGame ctor 0063008D; set to 1 only by
// CGuiInGame::Initialize 0063328F (<- InitializeInGameGUI 005F1410 <- 005ED730 <- HandleServerToPlayerModule 00668575);
// ShutDown 0062C617. The function returns [this+30h] (0062CB7D); the panel-creation continuation 0062C9DA.. is owned by
// the real selected ingame_gui body. Single owner src/hud/ingame_gui.cpp (kotor_fastcall_0062c9b0).
extern "C" int __fastcall kotor_fastcall_0062c9b0(void* gui, void*, U32 panel);

// ======== 00684ED0 CSWGuiTargetActionMenu::IsTargetActionMenuControl(control) thiscall RET4, AL 0/1/2 ========
// Pure address comparisons against members of `this` (no memory read): 1 for the four controls at +15CCh/+198Ch/+170Ch/
// +184Ch, 2 for the 3x4 grid at +5A0h-1C4h/+5A0h/-388h/-54Ch stepping 71Ch, else 0.
extern "C" unsigned char __fastcall kotor_fastcall_00684ed0(void* menu, void*, void* control) {
    const std::uintptr_t c = reinterpret_cast<std::uintptr_t>(control);
    const std::uintptr_t b = reinterpret_cast<std::uintptr_t>(menu);
    if (c == b + 0x15CC || c == b + 0x198C || c == b + 0x170C || c == b + 0x184C) return 1;  // 00684ED4..FB -> 00684F3E
    std::uintptr_t e = b + 0x5A0;                                      // 00684EFF
    for (int i = 0; i < 3; ++i, e += 0x71C) {                          // 00684EFD / 00684F27..30
        if (c == e - 0x1C4 || c == e || c == e - 0x388 || c == e - 0x54C) return 2;  // 00684F05..25 -> 00684F38
    }
    return 0;                                                          // 00684F32
}

// ======== 005F4BE0 CClientExoAppInternal::SetToolTipInstantMode(int) thiscall RET4 ========
// The two main-interface calls 006856E0 / 00685710 require CGuiInGame+90h != 0 (0 before the in-game GUI is initialized:
// ctor 006300E7, set only by CGuiInGame::Initialize 0063304D / ResetInterfaceForSize 0062F65B). The real selected
// ingame_gui body is the single owner src/hud/ingame_gui.cpp (kotor_fastcall_005f4be0); no target branch traps.
extern "C" void __fastcall kotor_fastcall_005f4be0(void* self, void*, int instant);

// ======== 0061F940 / 0061F960 CClientExoAppInternal::PerformRButtonDown/UpAction (thiscall RET0, tail JMP) ========
extern "C" int __fastcall kotor_fastcall_0061f940(void* self, void*) {
    (void)self;
    cel::word_008338f0 = 1;                                            // 0061F946 (after loading ECX = [007A39F4])
    return kotor_fastcall_0040c610(static_cast<Manager*>(kotor_input_cells::gui_manager_007a39f4), nullptr);  // 0061F940 / 0061F950
}
extern "C" int __fastcall kotor_fastcall_0061f960(void* self, void*) {
    (void)self;
    cel::word_008338f0 = 0;                                            // 0061F966
    return kotor_fastcall_0040a1a0(static_cast<Manager*>(kotor_input_cells::gui_manager_007a39f4), nullptr);  // 0061F960 / 0061F970
}

// 00621210 inline helper
void* app() { return app_manager()->client; }
// 00621210 inline helper
unsigned char option_6d(const ClientInternal* self) { return at<unsigned char>(self->get<void*>(0x4), 0x6D); }
// 00621210 inline helper
void restore_camera(ClientInternal* self) {
    cel::camera_restore_007a2288 = 1;                                   // 00621866
    req::restore_camera_00641dc0(packed_file_get_async_file_00644760(self, nullptr), nullptr);  // 0062186C..73
    kotor_fastcall_006200e0(self, nullptr, 0, 1);                // 00621878..7C (push 1; push 0)
}
// 00621210 inline helper
void toggle_pause(ClientInternal* self) {
    const int paused = kotor_fastcall_005f2df0(self, nullptr);         // 00621625
    kotor_fastcall_005f2e10(self, nullptr, paused == 0 ? 1u : 0u, 4, 0);  // 0062162C..39
    if (kotor_fastcall_005edef0(app(), nullptr))                           // 0062163E..4E
        kotor_fastcall_005edee0(app(), nullptr, 0, 0);                     // 00621650..5B
    if (paused == 0 &&                                                            // 00621660
        show_tutorial_window_005edf40(app(), nullptr, 6, 0x7F000000u, 0x7F000000u, 0) != 0)  // 00621664..80
        return;
    static_cast<Manager*>(kotor_fastcall_005eda40(app(), nullptr))->PlayGuiSound(6);  // 00621686..98
}
// 00621210 inline helper
void auto_pause_on_select(ClientInternal* self) {
    if (at<unsigned char>(self->get<void*>(0x4), 0x16) & 1u)           // 006218B3..B9
        frame_SetAutoPaused_005f3f10(self, nullptr, 1, 8);            // 006218BF..C4
}
// 00621210 inline helper
void* main_interface_from_app() { return at<void*>(req::get_in_game_gui_005ed690(app(), nullptr), 0x90); }

int ClientInternal::HandleInputEvent(int event, int value) {
    const int result = 0;                                              // 00621219/00621220 [esp+10h]=0
    switch (event) {
    case 0x41:                                                         // 0062125B
        return SetMouseX(value);                                       // 0062125F..6A
    case 0x42:                                                         // 00621273
        return SetMouseY(value);                                       // 00621277..82
    case 0x43:                                                         // 0062128B
        if (value != 0) kotor_input_client::perform_lbutton_down_0061f880(this, nullptr, value == -1);  // 00621293..9E
        else kotor_input_client::perform_lbutton_up_00620530(this, nullptr);                           // 006212AF..B1
        return result;
    case 0x44:                                                         // 006212C2
        if (value != 0) kotor_fastcall_0061f940(this, nullptr);  // 006212CA
        else kotor_fastcall_0061f960(this, nullptr);               // 006212DB
        return result;
    case 0x46:                                                         // 006212EC: global 007A39F4 receiver
        kotor_fastcall_0040c650(static_cast<Manager*>(kotor_input_cells::gui_manager_007a39f4), nullptr, value);  // 006212F0..F7
        return result;
    case 0x09: case 0xCE:                                              // 00621308
        if (!value) return result;
        if (option_6d(this) == 5) {                                    // 00621312..20
            cel::camera_restore_007a2288 = 1;                          // 00621322
            req::restore_camera_00641dc0(packed_file_get_async_file_00644760(this, nullptr), nullptr);  // 00621328..2F
            kotor_fastcall_006200e0(this, nullptr, 0, 1);       // 00621334..38
        } else {
            req::change_character_to_next_living_party_member_005f7960(this, nullptr, 0, 1, 1);  // 0062133F..42
            req::change_character_0062b0f0(req::get_in_game_gui_005ed690(app(), nullptr), nullptr);  // 00621347..57
        }
        static_cast<Manager*>(kotor_fastcall_005eda40(app(), nullptr))->PlayGuiSound(6);  // 0062135C..6E
        return result;
    case 0x0A: case 0xCF: {                                            // 0062137F
        if (!value) return result;
        if (option_6d(this) == 5) { restore_camera(this); return result; }  // 00621389..90 -> 0062180B
        void* gui = in_game_gui(this);                                 // 00621396
        if (!gui || at<U32>(gui, 0x30) != 0) return result;            // 00621399..A4
        req::show_solo_mode_query_0062e550(gui, nullptr, 0);           // 006213AA..AB
        return result;
    }
    case 0x0B: case 0xDF: {                                            // 006213BC
        if (!value) return result;
        int restored = 0;                                              // 006213CC EDI
        if (option_6d(this) == 5) {                                    // 006213C6..D5
            cel::camera_restore_007a2288 = 1;                          // 006213D9
            req::restore_camera_00641dc0(packed_file_get_async_file_00644760(this, nullptr), nullptr);  // 006213DF..E6
            kotor_fastcall_006200e0(this, nullptr, 0, 1);       // 006213EB..EF
            restored = 1;                                              // 006213F4
        }
        if (kotor_fastcall_005f2ed0(this, nullptr)) {  // 006213F6..FF
            if (get<U32>(0x2C0) != 0) {                                // 006214F5..FB -> 006219EC
                req::hurry_death_sequence_005f2f20(this, nullptr);     // 006219EE
                return result;
            }
            GuiMessageBox* box = at<GuiMessageBox*>(in_game_gui(this), 0x98);  // 00621501..04
            kotor_fastcall_00627130(box, nullptr, 0);           // 0062150A..0D
            gui_load_messagebox_set_callback_00624a40(box, nullptr, 0, 0, 0);         // 00621512..17
            box->vtable->set_text(box, nullptr, 0xA684);               // 0062151C..25
            kotor_fastcall_0040bc70(get<void*>(0x274), nullptr, box, 1, 1);  // 00621528..31
            return result;
        }
        void* gui = in_game_gui(this);                                 // 00621405
        if (at<U32>(gui, 0xB4) == 0 &&                                 // 00621408..0E
            req::get_sw_party_005ed8b0(app(), nullptr) &&              // 00621414..23
            kotor_fastcall_00635460(req::get_sw_party_005ed8b0(app(), nullptr), nullptr)) {  // 00621429..40
            gui = in_game_gui(this);                                   // 00621446
            if (at<U32>(gui, 0x34) == 3) {                             // 00621449..4D
                if (frame_R4_0062cba0(gui, nullptr, 0)) {  // 0062144F..57
                    kotor_fastcall_006200e0(this, nullptr, 0, 1);   // 0062145D..61
                    return result;
                }
            } else if (get<U32>(0x9C) == 0 &&                          // 00621472..78
                       kotor_fastcall_0062c9b0(gui, nullptr, 7)) {  // 0062147E..87
                if (get<U32>(0x9C) != 2) {                             // 0062148D..9A
                    enter_gui_class(this);                             // 0062149C..C4
                    put<U32>(in_game_gui(this), 0x30, 1);              // 006214C9..CC (reloaded, unguarded)
                }
                show_tutorial_window_005edf40(app(), nullptr, 7, 0x7F000000u, 0x7F000000u, 0);  // 006214CF..E4
                return result;
            }
        }
        if (restored == 0)                                             // 00621542..44
            client_manager(this)->HandleInputEvent(0xB4, 1);           // 0062154A..17 (push 1; push B4)
        return result;
    }
    case 0x50:                                                         // 00621555
        if (!value) return result;
        probe_screenshot_name(this);
        return result;                                                 // 006215E3 ([esp+0Ch] after POP EDI = result)
    case 0x02: case 0xE0: case 0xFD:                                   // 006215EE
    case 0xF1:                                                         // 00621B67 (identical body)
        if (!value) return result;
        if (option_6d(this) == 5) {                                    // 006215F8..FF
            cel::camera_restore_007a2288 = 1;                          // 00621608
            req::restore_camera_00641dc0(packed_file_get_async_file_00644760(this, nullptr), nullptr);  // 0062160E..15
            kotor_fastcall_006200e0(this, nullptr, 0, 1);       // 0062161A..1E
        }
        toggle_pause(this);                                            // 00621623..98
        return result;
    case 0x0F: case 0x48:                                              // 006216A9
        if (!value) return result;
        if (option_6d(this) != 5) return result;                       // 006216B3..BA
        restore_camera(this);                                          // 006216C2 -> 00621861
        return result;
    case 0x01: case 0xD0: {                                            // 006216C7
        if (!req::get_player_creature_005f1420(this, nullptr)) return result;          // 006216C9..D0
        if (at<U32>(req::get_player_creature_005f1420(this, nullptr), 0x138) != 0) return result;  // 006216D6..E3
        if (!value) return result;                                     // 006216E9
        if (get<U32>(0x2C0) != 0) return result;                       // 006216F3..F9
        bool free_look = option_6d(this) != 5;                         // 006216FF..09
        if (free_look) {
            void* server = kotor_m15a_creature_get_server_creature_0060fb20(req::get_player_creature_005f1420(this, nullptr), nullptr);  // 0062170F..18
            free_look = server != nullptr;                             // 0062171D
        }
        if (free_look) {
            void* server = kotor_m15a_creature_get_server_creature_0060fb20(req::get_player_creature_005f1420(this, nullptr), nullptr);  // 00621725..2E
            free_look = at<U32>(at<void*>(server, 0xFC), 0) == 0;      // 00621733..3B
        }
        if (free_look) free_look = kotor_fastcall_005edc10(app(), nullptr) == 0;  // 00621741..51
        if (free_look) {
            const int toggle = cel::camera_toggle_008338e8;            // 00621757 (read before the store)
            cel::camera_restore_007a2288 = 0;                          // 0062175E
            if (toggle == 1) {
                req::set_navigate_camera_00641610(packed_file_get_async_file_00644760(this, nullptr), nullptr);  // 006217B3..BC
                cel::camera_toggle_008338e8 = cel::camera_toggle_008338e8 + 1;  // 006217C1..CA
                return result;
            }
            void* pc = kotor_fastcall_00635460(get<void*>(0x270), nullptr);  // 00621766..6C
            const U32 id = at<U32>(pc, 0x4);                           // 00621771
            req::set_free_look_camera_006413c0(packed_file_get_async_file_00644760(this, nullptr), nullptr, id);  // 00621775..7E
            if (get<U32>(0x9C) == 4) return result;                    // 00621783..90
            set<U32>(0x9C, 4);                                         // 00621796
            kotor_fastcall_005df4e0(input(), nullptr);              // 0062179C..A2
            return result;
        }
        if (option_6d(this) != 5) return result;                       // 006217D8..DE (options reloaded)
        restore_camera(this);                                          // 006217E6 -> 00621861
        return result;
    }
    case 0x03: case 0x04: case 0x49: case 0x4A: case 0x4B: case 0x4C:  // 006217E8
        if (!value) return result;
        if (get<U32>(0x2C0) != 0) return result;                       // 006217F2..F8
        if (option_6d(this) != 5) return result;                       // 006217FE..05
        restore_camera(this);                                          // 0062180B -> 00621861
        return result;
    case 0x05: case 0xCD:                                              // 0062180F
        if (!value) return result;
        if (option_6d(this) == 5) { restore_camera(this); return result; }  // 00621819..27 -> 00621866
        req::select_nearest_object_005fb050(this, nullptr, 1, get<U32>(0x320) == 0 ? 0u : 2u);  // 00621829..39
        if (get<U32>(0x320) == 0) return result;                       // 0062183E..44
        auto_pause_on_select(this);                                    // 0062184A -> 006218B3
        return result;
    case 0x06: case 0xCC:                                              // 0062184C
        if (!value) return result;
        if (option_6d(this) == 5) { restore_camera(this); return result; }  // 00621856..5F -> 00621861
        req::select_nearest_object_005fb050(this, nullptr, 0, get<U32>(0x320) == 0 ? 0u : 2u);  // 0062188D..9D
        if (get<U32>(0x320) == 0) return result;                       // 006218A2..A8
        auto_pause_on_select(this);                                    // 006218AE..C4
        return result;
    case 0xD1: case 0xD2: case 0xD3: case 0xD4:
    case 0xD5: case 0xD6: case 0xD7: case 0xD8: {                      // 006218D5
        if (!value) return result;
        if (option_6d(this) == 5) {                                    // 006218DF..EB
            cel::camera_restore_007a2288 = 1;                          // 006218EF
            req::restore_camera_00641dc0(packed_file_get_async_file_00644760(this, nullptr), nullptr);  // 006218F5..FC
            kotor_fastcall_006200e0(this, nullptr, 0, 1);       // 00621901..05
        }
        if (kotor_fastcall_005f2ed0(this, nullptr)) {  // 0062190A..13
            if (get<U32>(0x2C0) == 0) return result;                   // 006219E0..E6
            req::hurry_death_sequence_005f2f20(this, nullptr);         // 006219EC..EE
            return result;
        }
        void* gui = in_game_gui(this);                                 // 00621919
        if (at<U32>(gui, 0xB4) != 0) return result;                    // 0062191C..22
        if (client_manager(this)->modal_count != 0) return result;     // 00621928..34
        const U32 index = static_cast<U32>(event) - 0xD1u;             // 0062193E LEA EAX,[EDI-D1]
        if (at<U32>(gui, 0x34) == 3) {                                 // 0062193A..44
            if (at<U32>(gui, 0x2C) != index) {                         // 00621946..49
                req::switch_to_sw_in_game_gui_0062cf10(gui, nullptr, index);  // 0062196E..6F
                return result;
            }
            if (!frame_R4_0062cba0(gui, nullptr, 0)) return result;  // 0062194B..53
            kotor_fastcall_006200e0(this, nullptr, 0, 1);       // 00621959..5D
            return result;
        }
        if (!kotor_fastcall_0062c9b0(gui, nullptr, index)) return result;  // 00621980..88
        if (get<U32>(0x9C) == 2) return result;                        // 0062198E..9B
        enter_gui_class(this);                                         // 006219A1..C9
        put<U32>(in_game_gui(this), 0x30, 1);                          // 006219CE..D8 (reloaded, unguarded)
        return result;
    }
    case 0xC8:                                                         // 006219FF
        forward_to_main_interface(this, 0x3D, value);
        return result;
    case 0xC9:                                                         // 00621A3A
        forward_to_main_interface(this, 0x3E, value);
        return result;
    case 0xDA:                                                         // 00621A75
        if (!value) return result;
        kotor_fastcall_005df4e0(input(), nullptr);                  // 00621A7F..85
        kotor_fastcall_005edba0(app(), nullptr);        // 00621A8A..93
        req::do_quick_save_005f4b50(this, nullptr);                    // 00621A98..9A
        cool_down_event_005df4b0(input(), nullptr, 0x107, 0x3E8); // 00621A9F..AF (push 3E8; push 107)
        return result;
    case 0x107: {                                                      // 00621AC0
        if (!value) return result;
        if (at<U32>(at<void*>(app_manager(), 0x14), 0) != 0) return result;  // 00621ACA..D5
        kotor_fastcall_005df4e0(input(), nullptr);                  // 00621ADB..E1
        cel::word_008338f4 = 0;                                        // 00621AEC
        cel::word_008338f0 = 0;                                        // 00621AF2
        kotor_fastcall_005edba0(app(), nullptr);        // 00621AE6/F8..FB
        req::do_quick_load_00633c50(req::get_in_game_gui_005ed690(app(), nullptr), nullptr);  // 00621B00..10
        return result;
    }
    case 0x108: {                                                      // 00621B21
        if (!value) return result;
        if (!kotor_fastcall_00610ac0(req::get_player_creature_005ed540(app(), nullptr), nullptr)) return result;  // 00621B2B..41
        void* pc = kotor_fastcall_00635460(get<void*>(0x270), nullptr);  // 00621B47..4F
        req::party_menu_stealth_0060f4b0(pc, nullptr, 0, 0);          // 00621B4D..56 (args pushed before 00635460)
        return result;
    }
    case 0xF2:                                                         // 00621C21
        if (!value) return result;
        req::player_flourish_weapons_005ede90(app(), nullptr);         // 00621C2B..34
        return result;
    case 0xD9: case 0x19: {                                            // 00621C45 / 00621C80 (identical bodies)
        if (!value) return result;
        void* module = get<void*>(0x18);                               // 00621C4F / 00621C8A
        if (!module || at<U32>(module, 0x48) == 0) return result;      // 00621C52..5D
        if (kotor_fastcall_005f2df0(this, nullptr)) return result;  // 00621C63..6C
        void* mini = at<void*>(at<void*>(get<void*>(0x18), 0x48), 0x264);  // 00621C72..78 (reloaded)
        if (!mini) return result;                                      // 00621CB9
        void* player = at<void*>(mini, 0x24);                          // 00621CC1
        if (!player) return result;                                    // 00621CC4
        req::fire_0066dc50(player, nullptr);                           // 00621CCC..CE
        return result;
    }
    case 0xDB: case 0xDC:                                              // 00621CDF
        cel::word_008338f0 = value;                                    // 00621CE6
        return result;
    case 0xDD: case 0xDE:                                              // 00621CF4
        cel::word_008338f8 = value;                                    // 00621CFF
        return result;
    case 0xE1:                                                         // 00621D0A
        kotor_fastcall_005f4be0(this, nullptr, value != 0 ? 1 : 0);  // 00621D12 / 00621D25
        return result;
    case 0xE8: case 0xEA: case 0xEC: case 0xEE: {                      // 00621D37 / 00621D96 / 00621DFB / 00621E5C
        if (!value) return result;
        const U32 slot = event == 0xE8 ? 0u : event == 0xEA ? 1u : event == 0xEC ? 3u : 2u;
        if (cel::word_008338f8 != 0)                                   // 00621D41 etc.
            req::select_next_action_00688820(main_interface_from_app(), nullptr, slot);  // 00621D49..5E etc.
        else
            kotor_fastcall_0068ad60(main_interface_from_app(), nullptr, slot, 1);  // 00621D6F..85 etc. (push 1; push slot)
        return result;
    }
    case 0xE2: case 0xE4: case 0xE6: {                                 // 00621EBD / 00621F0B / 00621F5A
        if (!value) return result;
        const U32 slot = event == 0xE2 ? 0u : event == 0xE4 ? 1u : 2u;
        if (cel::word_008338f8 != 0) {                                 // 00621EC7 etc.
            void* gui = req::get_in_game_gui_005ed690(app(), nullptr); // 00621ED8 etc.
            unsigned char* menu = static_cast<unsigned char*>(at<void*>(gui, 0x90)) + 0xBC;  // 00621F7C..82
            req::select_next_action_006865b0(menu, nullptr, slot);    // 00621F88
        } else {
            req::do_target_action_0068ad20(main_interface_from_app(), nullptr, slot, 1);  // 00621EE3..FA etc.
        }
        return result;
    }
    case 0xEF: {                                                       // 00621FC1
        if (!value) return result;
        if (get<U32>(0x2B4) == 0x7F000000u) return result;             // 00621FCB..D5
        kotor_fastcall_00620620(this, nullptr);              // 00621FDB..DD
        if (get<std::int32_t>(0x4CC) <= 0) return result;              // 00621FE2..E8 (signed)
        DefaultAction* action = get<DefaultAction*>(0x4C8);            // 00621FEE
        if (!action) return result;                                    // 00621FF4
        void* pc = kotor_fastcall_00635460(req::get_sw_party_005ed8b0(app(), nullptr), nullptr);  // 00621FFC..0C
        GameObject* object = static_cast<GameObject*>(kotor_fastcall_005ed580(app(), nullptr, action->object_id));  // 00622011..20
        if (!object) return result;                                    // 00622025
        void* base = object->vtable->base(object, nullptr);            // 0062202D..31
        if (!base) return result;                                      // 00622034
        void* receiver = static_cast<unsigned char*>(base) + action->adjustment;  // 00622041..44
        action->function(receiver, nullptr, action->argument, pc);     // 0062203C..46
        return result;
    }
    case 0xF0: {                                                       // 00622055
        if (!value) return result;
        if (!kotor_fastcall_005ede70(app(), nullptr)) return result;  // 0062205F..6F
        void* pc = kotor_fastcall_00635460(req::get_sw_party_005ed8b0(app(), nullptr), nullptr);  // 00622075..84
        if (!pc) return result;                                        // 0062208B
        kotor_fastcall_00610a10(pc, nullptr, 0);                 // 00622093..96
        kotor_fastcall_0063d470(pc, nullptr);                  // 0062209B..9D
        return result;
    }
    case 0xF5: case 0xFB: case 0xFA: {                                 // 006220AE / 006220D6
        if (!value) return result;
        void* gui = in_game_gui(this);                                 // 006220B4 / 006220DC
        if (!gui) return result;
        void* mi = at<void*>(gui, 0x90);                               // 006220BB / 006220E3
        if (!mi) return result;
        if (event == 0xFA) renderer_void_004015a0();                   // 006220ED (plain RET; ECX ignored)
        else req::clear_one_action_00688790(mi, nullptr);              // 006220C5
        return result;
    }
    case 0xB5:                                                         // 006220FE
        if (value != 0 && get<U32>(0x9C) == 2)                         // 00622102..0D
            client_manager(this)->HandleInputEvent(event, value);      // 0062210F..17
        return result;
    default:                                                           // 0062211C
        return result;
    }
}

namespace {
// 0061F8BD..0061F90C / 0062056D..006205BC (shared shape): returns true when the
// GUI path proceeds. The flag word 008338F0 is loaded before GetClientOptions.
bool gui_route(ClientInternal* self) {
    U32 flag = static_cast<U32>(cel::word_008338f0);                   // 0061F8C6 / 00620576
    const ClientOptions* options = app_manager()->client->GetClientOptions();  // 0061F8BD..CC / 0062056D..7C
    if (at<unsigned char>(options, 0x8) & 2u) {                        // 0061F8D1 / 00620581
        const U32 mode = self->get<U32>(0x9C);                         // 0061F8D7 / 00620587
        if (mode != 2 && mode != 3) flag = flag == 0 ? 1u : 0u;        // 0061F8DD..EE / 0062058D..9E
    }
    if (flag == 0 && self->get<U32>(0x9C) != 4) return true;           // 0061F8F0..FC / 006205A0..AC
    const U32 mode = self->get<U32>(0x9C);                             // 0061F8FE / 006205AE
    return mode == 2 || mode == 3;                                     // 0061F904..0C / 006205B4..BC
}

// 0061F89E..0061F8B8 / 0062054E..00620568: cursor word +38, enable bit (+1C>>2)&1.
void set_cursor(ClientInternal* self, U32 pressed) {
    void* manager = self->get<void*>(0x274);
    if (!manager) return;
    const U32 enable = (at<U32>(manager, 0x1C) >> 2) & 1u;
    menu_tables_m41::GuiSetSoftwareMouseCursor_0040a270(manager, nullptr, at<U32>(manager, 0x38), enable, pressed);
}
}

namespace kotor_input_client {
// 0061F880 CClientExoAppInternal::PerformLButtonDownAction (thiscall, RET 4).
void __fastcall perform_lbutton_down_0061f880(ClientInternal* self, void*, int at_end) {
    void* in = input();                                                // 0061F883
    cel::word_008338f4 = 1;                                            // 0061F88B (after the load, before the test)
    if (in) kotor_fastcall_005df5b0(in, nullptr, 1);                   // 0061F889..99 CExoInput::RefreshMouseCursor(1)
    set_cursor(self, 1);                                               // 0061F89E..B8 (push 1)
    if (!gui_route(self)) return;                                      // 0061F8BD..0C -> 0061F92E
    if (at_end == 0) self->set<U32>(0x2B8, self->get<U32>(0x2B4));     // 0061F90E..1C
    client_manager(self)->HandleLMouseDown(at_end);                    // 0061F922..29 (no null check)
}

// 00620530 CClientExoAppInternal::PerformLButtonUpAction (thiscall, RET).
// HandleMouseClickInWorld 00620350 is reached only with AppManager+8 (server) != 0: 0 on the menu (ctor 00401181; only
// writer CreateServer 00401406, callers OnModulePicked / CSWGuiSaveLoad::LoadGame / ClassSelection::BeginPlayGame).
void __fastcall perform_lbutton_up_00620530(ClientInternal* self, void*) {
    void* in = input();                                                // 00620533
    cel::word_008338f4 = 0;                                            // 0062053B
    if (in) kotor_fastcall_005df5b0(in, nullptr, 0);                   // 00620539..49
    set_cursor(self, 0);                                               // 0062054E..68 (push 0)
    if (!gui_route(self)) return;                                      // 0062056D..BC -> 0062061C
    Control* over = client_manager(self)->mouse_over;                  // 006205BE..C4
    if (over) {
        // [CGuiInGame+90h] + 0BCh computed as an address (no read; the main interface is 0 on the menu, so the value is 0BCh).
        void* menu = reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(at<void*>(in_game_gui(self), 0x90)) + 0xBC);  // 006205CB..D5
        if (kotor_fastcall_00684ed0(menu, nullptr, over) == 1) {       // 006205DB..E2
            if (at<U32>(app_manager(), 0x8) == 0) return;              // 006205E4..EF
            req::handle_mouse_click_in_world_00620350(self, nullptr);  // 006205F1..F4 (tail JMP)
            return;
        }
    }
    if (gui_manager()->HandleLMouseUp() != 0) return;                  // 006205F9..06
    if (at<U32>(app_manager(), 0x8) == 0) return;                      // 00620608..12
    req::handle_mouse_click_in_world_00620350(self, nullptr);          // 00620614..17 (tail JMP)
}
}

// ======== 00620350 CClientExoAppInternal::HandleMouseClickInWorld ========
// Original range [00620350,00620480): common return 00620420..00620424, alternate path tail-jumps
// 0062046C -> 00689D80, NOP alignment to 0062047F. thiscall ECX=self, no stack arguments, RET 0,
// no EH/x87. Callers: only 00620530 at 006205F4/00620617, both tail jumps.
//
// Entry-to-all-returns map (instruction addresses are original VAs):
//   00620350..5E  get_in_game_gui(app())            -> gui
//   00620363..6B  [gui+0xB4] != 0                   -> return   (no gui-null guard)
//   00620371..79  [[self+0x40]+0x30] != 0           -> return   (no self+0x40-null guard)
//   0062037F..86  byte [[self+4]+0x6D] == 5         -> return   (no self+4-null guard)
//   0062038C      downID = [self+0x2B8]             (single load)
//   00620392..97  downID == 0x7F000000              -> selection 00620425
//   0062039D      target = [self+0x2B4]             (single load)
//   006203A3..A9  target != [self+0x4A4]            -> selection
//   006203AB..AD  downID != target                  -> selection  (downID not reloaded)
//   006203AF..B5  count = [self+0x4CC]              (signed load before reset)
//   006203B7      [self+0x2B8] = 0x7F000000         (reset before signed count test)
//   006203C1      count <= 0                        -> return
//   006203C3..CB  action = [self+0x4C8]; if null    -> return
//   006203CD..DC  party = GetSWParty(app()); pc = GetPlayerCharacter(party)  (no pc-null guard)
//   006203E1..F0  appManager = [007A39FC]; objectID = [action+0x1C]; app = [appManager+4];
//                  object = GetGameObject(app, objectID); if null -> return
//   006203F9..FD  base = object->vtable[0x0C](object); if null -> return
//   00620406..0E  Manager::PlayGuiSound(6)          ([self+0x274], no manager-null guard)
//   00620413      argument = [action+0x08]          (read after PlayGuiSound)
//   00620416      adjustment = [action+0x10]        (read after argument)
//   00620419..1D  push pc; push argument; ECX = base + adjustment; call [action+0x0C]
//   00620420..24  common return
// Selection/re-populate 00620425..0062046C:
//   00620425      hover = [self+0x4A4]             (fresh read; not the earlier comparison load)
//   0062042B..37  if hover != 0x7F000000 SetLastTarget(self, hover, 0)
//   0062043C..52  first GetInGameGui(app())/[+0x90] null gate -> return
//   00620454..5D  SECOND GetInGameGui(app())        (fresh; app cell reloaded)
//   00620462..6C  [secondGui+0x90] -> PopulateMenus; no second null test.
//                 The original tail jump is emitted as a normal RET-0 call + this function's return;
//                 no stack arguments, no caller-frame/EH/lifetime dependency.
//
// Direct providers (selected real, no new bodies here): GetInGameGui 005ED690 and GetSWParty 005ED8B0
// use the existing C++-name declarations; GetPlayerCharacter 00635460 (@8), GetGameObject 005ED580
// (@12), SetLastTarget 005F4A20 (@16) and PopulateMenus 00689D80 (@8) use the selected extern-C
// aliases below; PlayGuiSound 0040A140 is the existing Manager member. The callback is real action
// data from the selected default-action array; 0060F620/00417B10 is not hardcoded here.
extern "C" void* __fastcall kotor_fastcall_00635460(void* party, void*);
extern "C" void* __fastcall kotor_fastcall_005ed580(void* app, void*, std::uint32_t object_id);
extern "C" void __fastcall kotor_fastcall_005f4a20(void* self, void*, std::uint32_t target, std::uint32_t value);
extern "C" void __fastcall kotor_fastcall_00689d80(void* main_interface, void*);

namespace kotor_input_required {
void __fastcall handle_mouse_click_in_world_00620350(void* self_void, void*) {
    ClientInternal* const self = static_cast<ClientInternal*>(self_void);
    void* const gui = req::get_in_game_gui_005ed690(app(), nullptr);       // 00620350..5E
    if (at<U32>(gui, 0xB4) != 0) return;                                  // 00620363..6B
    void* const self_gui = self->get<void*>(0x40);                        // 00620371
    if (at<U32>(self_gui, 0x30) != 0) return;                             // 00620374..79
    if (option_6d(self) == 5) return;                                      // 0062037F..86
    const U32 down_id = self->get<U32>(0x2B8);                            // 0062038C
    if (down_id != 0x7F000000u) {                                         // 00620392..97
        const U32 current_target = self->get<U32>(0x2B4);                 // 0062039D
        if (current_target == self->get<U32>(0x4A4) &&                    // 006203A3..A9
            down_id == current_target) {                                  // 006203AB..AD
            const std::int32_t count = self->get<std::int32_t>(0x4CC);    // 006203AF..B5 (load)
            self->set<U32>(0x2B8, 0x7F000000u);                           // 006203B7 (reset before test)
            if (count > 0) {                                              // 006203C1 (signed)
                DefaultAction* const action = self->get<DefaultAction*>(0x4C8); // 006203C3..CB
                if (action) {                                             // 006203C9..CB
                    void* const party = req::get_sw_party_005ed8b0(app(), nullptr); // 006203CD..D5
                    void* const pc = kotor_fastcall_00635460(party, nullptr);       // 006203DC
                    AppManager* const manager = app_manager();            // 006203E1 ([007A39FC])
                    const U32 object_id = action->object_id;              // 006203E7
                    void* const app_for_object = manager->client;         // 006203EA ([eax+4])
                    GameObject* const object = static_cast<GameObject*>(
                        kotor_fastcall_005ed580(app_for_object, nullptr, object_id)); // 006203EE..F0
                    if (object) {                                         // 006203F5..F7
                        void* const base = object->vtable->base(object, nullptr); // 006203F9..FD
                        if (base) {                                       // 00620402..04
                            client_manager(self)->PlayGuiSound(static_cast<char>(6)); // 00620406..0E
                            const U32 argument = action->argument;        // 00620413
                            const std::int32_t adjustment = action->adjustment; // 00620416
                            void* const receiver = static_cast<unsigned char*>(base) + adjustment; // 0062041B
                            action->function(receiver, nullptr, argument, pc); // 00620419..1D
                        }
                    }
                }
            }
            return;                                                       // 00620420
        }
    }
    const U32 hover = self->get<U32>(0x4A4);                              // 00620425 (fresh)
    if (hover != 0x7F000000u)                                             // 0062042B..30
        kotor_fastcall_005f4a20(self, nullptr, hover, 0);                 // 00620432..37
    void* const first_gui = req::get_in_game_gui_005ed690(app(), nullptr); // 0062043C..45
    if (at<void*>(first_gui, 0x90) != nullptr) {                          // 0062044A..52 (gate only)
        void* const second_gui = req::get_in_game_gui_005ed690(app(), nullptr); // 00620454..5D (fresh)
        kotor_fastcall_00689d80(at<void*>(second_gui, 0x90), nullptr);    // 00620462..6C (no 2nd null test)
    }
}
}
