// lane-m48-traps fork f-frame: CClientExoApp::QueryExitProgram 005EDA00 (WM_CLOSE path, WinMessageHandler 00402B51 —
// reachable on the main menu) with its callees EnableInput 005EDBF0/0061F7D0 and the quit-box callback
// HandleExitGameButton 005F1BD0. Evidence: dumpbin /disasm swkotor.exe 005EDA00..005EDA07, 005F6960..005F6B0F,
// 005EDBF0..005EDBF7, 0061F7D0..0061F7DA, 005F1BD0..005F1C10; user32 IAT 0073D3DC = PostQuitMessage.
// quit lane: the former D83 branch trap 005F69C4..005F69F9 (player creature present) is reconstructed: free-look
// camera restore (CSWCModule::RestoreCamera 00641DC0) and SetInputClass(0,1) 006200E0.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>
#include <cstring>
#include "fa48658349-hover_path.h"               // lane-input kotor_input_hover::GuiObject / Control (PMF event owners)

namespace {
using U32 = std::uint32_t;
namespace kih = kotor_input_hover;
inline U32 lw(const void* o, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
inline void sw(void* o, U32 off, U32 v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
inline void* lp(const void* o, U32 off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(lw(o, off))); }
using EventFunc = void (kih::GuiObject::*)(kih::Control*);   // same type as gui_load::EventFunc (gui_objects.h:112)
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; extern void* volatile input_007a39e4; }

extern "C" {
// Existing composed-link providers.
void* __fastcall frame_R4_005ed690(void* app, void*);                                   // CClientExoApp::GetInGameGui
void __fastcall kotor_fastcall_005ed310(void*, void*);                                  // void_return_this+4 005ED310
void* __fastcall kotor_fastcall_005f1420(void* internal, void*);                       // GetPlayerCreature (client)
unsigned __fastcall frame_GetPauseState_004ae980(void*, void*, unsigned);               // CServerExoApp::GetPauseState
void __fastcall kotor_fastcall_005df570(void* input, void*);                            // CExoInput::PauseRumble
void __fastcall frame_R4_005d5e80(void*, void*, unsigned);                              // CExoSound::SetSoundMode
extern void* kotor_cell_sound_007a39ec;                                                 // 007A39EC (lane-sound owner)
extern const std::uint32_t kotor_rdata_sound_mode_0074c5f4;                             // .rdata 0074C5F4
void __fastcall frame_SetAutoPaused_005f3f10(void*, void*, unsigned, unsigned);         // SetAutoPaused (RET 8)
U32 __fastcall kotor_fastcall_00641980(void* module, void*);                            // CSWCModule::GetMiniGame
void __fastcall kotor_fastcall_0061fa00(void* internal, void*, int);                    // ShowMouse (RET 4)
void __fastcall kotor_fastcall_00627130(void* box, void*, U32 allow);                   // CSWGuiMessageBox::SetAllowCancel
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                          // CClientExoApp::GetSWGuiManager
void __fastcall kotor_fastcall_0040bc70(void* gui, void*, void* panel, U32, U32) noexcept(false); // CSWGuiManager::AddPanel
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, kih::GuiObject* object,
                                                          EventFunc func, U32 argument); // CSWGuiMessageBox::SetCallback
void __fastcall kotor_fastcall_005df540(void* input, void*, int);                       // CExoInput::SetActive
void __fastcall kotor_fastcall_005eda60(void* client, void*, U32, U32);                 // CClientExoApp::SetInputClass
U32 __fastcall kotor_fastcall_00641dc0(void* module, void*);                           // CSWCModule::RestoreCamera [combat_restore]
void __fastcall kotor_fastcall_006200e0(void* internal, void*, U32, U32);              // CClientExoAppInternal::SetInputClass RET 8 [0298]
extern unsigned recovered_camera_reset_cell_007a2288;                                   // [007A2288] (w1_native_b7)
// Defined here.
void __fastcall kotor_fastcall_0061f7d0(void* internal, void*);
void __fastcall kotor_fastcall_005edbf0(void* client, void*);
std::uint32_t __fastcall kotor_fastcall_005eda00(void* outer, void*);
}

// 0061F7D0 CClientExoAppInternal::EnableInput: [this+2BCh] = 0; ret.
extern "C" void __fastcall kotor_fastcall_0061f7d0(void* internal, void*) { sw(internal, 0x2BC, 0); }   // 0061F7D0
// 005EDBF0 CClientExoApp::EnableInput: mov ecx,[ecx+4]; jmp 0061F7D0.
extern "C" void __fastcall kotor_fastcall_005edbf0(void* client, void*) { kotor_fastcall_0061f7d0(lp(client, 4), nullptr); }

namespace {
// 005F1BD0 CClientExoAppInternal::HandleExitGameButton(CSWGuiControl*) (thiscall, RET 4), registered by
// QueryExitProgram as the quit message box's event (SetCallback object = the box itself, 005F6ADA). Expressed as a
// member of a GuiObject-derived handler so the event is a genuine single-inheritance PMF (D47 pattern); the body
// uses only offsets of the receiving object, as the original does with ECX.
struct ExitGameButtonEvent : kih::GuiObject {
    void HandleExitGameButton_005f1bd0(kih::Control*);
};
void ExitGameButtonEvent::HandleExitGameButton_005f1bd0(kih::Control*) {
    void* const client = lp(kotor_menu_window_cells::manager_007a39fc, 4);           // 005F1BD0..005F1BD8
    void* const box = lp(frame_R4_005ed690(client, nullptr), 0x98);                   // 005F1BDB..005F1BE0
    const unsigned char flags = static_cast<const unsigned char*>(box)[0x64];         // 005F1BE6
    if ((flags & 1u) == 0) return;                                                    // 005F1BEE..005F1BF0 (RET 4)
    sw(this, 0x250, 1);                                                               // 005F1BF2 [receiver+250h] = 1
    sw(this, 0x254, 1);                                                               // 005F1BF8 [receiver+254h] = 1
    PostQuitMessage(0);                                                               // 005F1BFF..005F1C07 tail JMP [0073D3DC], arg 0
}
}

// 005EDA00 CClientExoApp::QueryExitProgram (thiscall, RET 0): mov ecx,[ecx+4]; jmp 005F6960.
// The consumer (routes-lane kotor_original_calls.h:101) declares a U32 result; the original leaves EAX unspecified and
// its caller discards it (00402B58 xor eax,eax): returns 0 (D2 label, D69 precedent).
extern "C" std::uint32_t __fastcall kotor_fastcall_005eda00(void* outer, void*) {
    void* const self = lp(outer, 4);                                                  // 005EDA00
    void* const client = lp(kotor_menu_window_cells::manager_007a39fc, 4);           // 005F6960..005F6968
    if (client == nullptr) return 0;                                                  // 005F696B..005F696D
    if (frame_R4_005ed690(client, nullptr) == nullptr) return 0;                       // 005F6973..005F697A
    if (lw(frame_R4_005ed690(lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr), 0xC0) != 0) return 0; // 005F6980..005F6996
    kotor_fastcall_005ed310(lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);  // 005F699C..005F69A6
    const U32 yes = 1;                                                                // 005F69B2 ebx = 1
    if (static_cast<const unsigned char*>(lp(self, 4))[0x6D] == 5 &&                  // 005F69AB..005F69B7
        kotor_fastcall_005f1420(self, nullptr) != nullptr &&                          // 005F69B9..005F69C2
        lw(kotor_fastcall_005f1420(self, nullptr), 0x138) == 0 &&                     // 005F69C4..005F69D3 creature [+138h]
        lw(self, 0x2C0) == 0 &&                                                       // 005F69D5..005F69DD
        lp(self, 0x18) != nullptr) {                                                  // 005F69DF..005F69E4 module
        recovered_camera_reset_cell_007a2288 = yes;                                   // 005F69E6 [007A2288] = 1
        (void)kotor_fastcall_00641dc0(lp(self, 0x18), nullptr);                       // 005F69EC..005F69EF RestoreCamera
        kotor_fastcall_006200e0(self, nullptr, 0, yes);                               // 005F69F4..005F69F9 SetInputClass(0,1)
    }
    if ((static_cast<const unsigned char*>(self)[0x384] & yes) == 0) {                // 005F69FE..005F6A04
        if (lw(lp(self, 0x40), 0xB4) == 0) {                                          // 005F6A06..005F6A11
            void* const server = lp(kotor_menu_window_cells::manager_007a39fc, 8);    // 005F6A13..005F6A1E
            const U32 paused = server ? frame_GetPauseState_004ae980(server, nullptr, 2) : 0;   // 005F6A20..005F6A2B
            U32 bits = lw(self, 0x37C);                                               // 005F6A2B
            if ((bits & 4u) == 0 && paused != yes) {                                  // 005F6A31..005F6A38
                sw(self, 0x37C, bits | 4u);                                           // 005F6A3A..005F6A3D
                static_cast<unsigned char*>(self)[0x388] = 4;                         // 005F6A43
                sw(self, 0x380, yes);                                                 // 005F6A4A
                kotor_fastcall_005df570(kotor_menu_window_cells::input_007a39e4, nullptr);   // 005F6A50..005F6A56
                frame_R4_005d5e80(kotor_cell_sound_007a39ec, nullptr, kotor_rdata_sound_mode_0074c5f4); // 005F6A5B..005F6A67
                sw(self, 0x4C4, yes);                                                 // 005F6A6C
            }
        }
        frame_SetAutoPaused_005f3f10(self, nullptr, yes, 0);                          // 005F6A72..005F6A77
    }
    void* const module = lp(self, 0x18);                                              // 005F6A7C
    if (module != nullptr && kotor_fastcall_00641980(module, nullptr) != 0)           // 005F6A81..005F6A8A
        kotor_fastcall_0061fa00(self, nullptr, static_cast<int>(yes));                // 005F6A8C..005F6A8F
    void* const box = lp(frame_R4_005ed690(lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr), 0x98); // 005F6A94..005F6AA2
    kotor_fastcall_00627130(box, nullptr, yes);                                       // 005F6AA8..005F6AAB
    {   // 005F6AB0..005F6AB9 box vtable slot +6Ch (CSWGuiMessageBox::SetMessage 006249D0, RET 4) with strref 0A56Ch.
        using SetMessage = void (__fastcall*)(void*, void*, U32);
        const unsigned char* table;
        std::memcpy(&table, box, sizeof table);
        SetMessage slot;
        std::memcpy(&slot, table + 0x6C, sizeof slot);
        slot(box, nullptr, 0xA56Cu);
    }
    kotor_fastcall_0040bc70(kotor_fastcall_005eda40(lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr),
                            nullptr, box, yes, yes);                                  // 005F6ABC..005F6ACE AddPanel(box,1,1)
    gui_load_messagebox_set_callback_00624a40(box, nullptr, static_cast<kih::GuiObject*>(box),
        static_cast<EventFunc>(&ExitGameButtonEvent::HandleExitGameButton_005f1bd0), 0); // 005F6AD3..005F6ADD (object = box, 005F1BD0, 0)
    kotor_fastcall_005df540(kotor_menu_window_cells::input_007a39e4, nullptr, static_cast<int>(yes)); // 005F6AE2..005F6AE9
    kotor_fastcall_005edbf0(lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);  // 005F6AEE..005F6AF7
    kotor_fastcall_005eda60(lp(kotor_menu_window_cells::manager_007a39fc, 4), nullptr, 2, yes); // 005F6AFC..005F6B08 SetInputClass(2,1)
    return 0;                                                                         // 005F6B0F (EAX unspecified; D2 label)
}
