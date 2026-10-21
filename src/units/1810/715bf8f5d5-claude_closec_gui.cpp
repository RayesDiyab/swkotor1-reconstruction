// Lane closeC (sub-fork "gui"): faithful bodies for trap stubs on the Endar play path, transcribed from the original
// swkotor.exe (read-only). Every comment address is an original instruction. No original address is dereferenced:
// [007A39FC] is kotor_menu_window_cells::manager_007a39fc (CAppManager*: +4 client, +8 server), [007A39EC] is
// frame_control::sound_manager_007a39ec, [0074C5EC] is the .rdata cell _kotor_rdata_sound_mode_0074c5ec (0).
//   005FCBD0 CClientExoAppInternal::ShutDownToMainMenu()          thiscall RET     (replaces the 0767 trap)
//   006AA620 CSWGuiTutorialBox::PerformReturnToBase(control)      thiscall RET 4   (replaces the 0838 trap)
//   0062B060 CGuiInGame::GetGuiInGameScreenPanel(BYTE index)      thiscall RET 4   (missing callee, no provider)
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
inline unsigned char* at(void* base, U off) { return static_cast<unsigned char*>(base) + off; }
inline U ld(void* base, U off) { U v; std::memcpy(&v, at(base, off), 4); return v; }
inline void st(void* base, U off, U v) { std::memcpy(at(base, off), &v, 4); }
inline void* lp(void* base, U off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(ld(base, off))); }
inline void* pt(U v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*
namespace frame_control { extern std::uint32_t sound_manager_007a39ec; }        // [007A39EC] CExoSound*
namespace kotor_input_required {
void* __fastcall get_in_game_gui_005ed690(void* client, void*);                  // 005ED690 CClientExoApp::GetInGameGui
}

extern "C" {
extern const std::uint32_t kotor_rdata_sound_mode_0074c5ec;                       // [0074C5EC] .rdata 0
// ---------------- existing providers (c35 map, exact decorated arity) ----------------
void __fastcall frame_R4_00401280(void* manager, void*);                          // 00401280 CAppManager::DestroyServer
unsigned char __fastcall recovered_object_array_remove_004d81a0(void* array, void*, U id, void* out); // 004D81A0 CGameObjectArray::Delete RET 8
void __fastcall recovered_client_area_unload_area_00609410(void* area, void*);   // 00609410 CSWCArea::UnloadArea
void __fastcall kotor_fastcall_005f8290(void* client, void*);                     // 005F8290 CClientExoAppInternal::UnloadModule
void __fastcall kotor_fastcall_005fca30(void* client, void*, U arg);              // 005FCA30 CClientExoAppInternal::DisplayMainMenu RET 4
void __fastcall frame_R4_005d5e80(void* sound, void*, U mode);                    // 005D5E80 CExoSoundInternal::SetSoundMode RET 4
void __fastcall frame_R4_005f9af0(void* client, void*, U a, U b, U c);            // 005F9AF0 StartMenuMusic RET 0Ch
void __fastcall kotor_fastcall_005edc20(void* client, void*, U a, U b, U c);      // 005EDC20 CClientExoApp::SetPausedByCombat RET 0Ch
void __fastcall kotor_fastcall_00692bc0(void* map, void*, U unused);              // 00692BC0 CSWGuiInGameMap::OnXButton RET 4
// ---------------- defined here ----------------
void __fastcall client_internal_shut_down_to_main_menu_005fcbd0(void* self, void*);
void* __fastcall kotor_fastcall_0062b060(void* gui, void*, U index);
}

// 005FCBD0 CClientExoAppInternal::ShutDownToMainMenu(): destroy the server, unload the client area/module, and (unless
// +254h is set) return to the main menu with menu music. The [esp+8] local (Delete's out object) is zeroed first.
extern "C" void __fastcall client_internal_shut_down_to_main_menu_005fcbd0(void* self, void*) {
    U removed = 0;                                                                     // 005FCBDB..005FCBDF
    if (ld(self, 0x254) != 0) st(self, 0x250, 1);                                      // 005FCBD5..005FCBE5
    frame_R4_00401280(kotor_menu_window_cells::manager_007a39fc, nullptr);             // 005FCBEF..005FCBF5 DestroyServer
    void* const module = lp(self, 0x18);                                               // 005FCBFA
    if (module != nullptr) {                                                           // 005FCBFD..005FCBFF
        void* const area = lp(module, 0x48);                                           // 005FCC02
        if (area != nullptr) {                                                         // 005FCC05..005FCC07
            (void)recovered_object_array_remove_004d81a0(lp(self, 0x14), nullptr, ld(area, 0x104), &removed); // 005FCC09..005FCC18
            recovered_client_area_unload_area_00609410(area, nullptr);                 // 005FCC1D..005FCC1F
        }
        kotor_fastcall_005f8290(self, nullptr);                                        // 005FCC24..005FCC26 UnloadModule
    }
    if (ld(self, 0x254) != 0) return;                                                  // 005FCC2C..005FCC32 (re-read)
    kotor_fastcall_005fca30(self, nullptr, 0);                                         // 005FCC34..005FCC37 DisplayMainMenu(0)
    frame_R4_005d5e80(pt(frame_control::sound_manager_007a39ec), nullptr, kotor_rdata_sound_mode_0074c5ec); // 005FCC3C..005FCC49
    st(self, 0x1EC, 1);                                                                // 005FCC56
    frame_R4_005f9af0(self, nullptr, 1, 0x7F, 2);                                      // 005FCC4E..005FCC60 StartMenuMusic(1,7Fh,2)
    if (ld(self, 0x18C) != 0) {                                                        // 005FCC65..005FCC6B
        void* const a = lp(kotor_menu_window_cells::manager_007a39fc, 0x14);           // 005FCC6D..005FCC72
        st(a, 0x14, 0);                                                                // 005FCC75
        void* const b = lp(kotor_menu_window_cells::manager_007a39fc, 0x14);           // 005FCC78..005FCC7E (re-read)
        st(b, 0, 0);                                                                   // 005FCC81
        st(b, 4, 0);                                                                   // 005FCC83
        st(self, 0x18C, 0);                                                            // 005FCC86
    }
}

// 0062B060 CGuiInGame::GetGuiInGameScreenPanel(BYTE index): byte compare (unsigned, jb) against 8; [this+index*4+0Ch].
extern "C" void* __fastcall kotor_fastcall_0062b060(void* gui, void*, U index) {
    const unsigned char i = static_cast<unsigned char>(index);                         // 0062B060 mov al,[esp+4]
    if (i >= 8) return nullptr;                                                        // 0062B064..0062B06A
    return lp(gui, 0x0C + static_cast<U>(i) * 4);                                      // 0062B06D..0062B074
}

namespace kotor_input_hover { struct Control; }
namespace kotor_m15_l0 {
struct TutorialBoxHandlers {
    void __thiscall PerformReturnToBase_006aa620(kotor_input_hover::Control*);
};
// 006AA620 CSWGuiTutorialBox::PerformReturnToBase(control) RET 4: unpause unless +990h, clear +990h, then the
// in-game screen panel 6 (map) OnXButton(0).
void __thiscall TutorialBoxHandlers::PerformReturnToBase_006aa620(kotor_input_hover::Control*) {
    void* const box = this;
    if (ld(box, 0x990) == 0)                                                           // 006AA623..006AA62B
        kotor_fastcall_005edc20(lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr, 0, 2, 0); // 006AA62D..006AA63B
    st(box, 0x990, 0);                                                                 // 006AA640
    void* const gui = kotor_input_required::get_in_game_gui_005ed690(
        lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);                    // 006AA64A..006AA657 (re-read)
    void* const panel = kotor_fastcall_0062b060(gui, nullptr, 6);                      // 006AA653..006AA65E
    kotor_fastcall_00692bc0(panel, nullptr, 0);                                        // 006AA650..006AA665
}
}
