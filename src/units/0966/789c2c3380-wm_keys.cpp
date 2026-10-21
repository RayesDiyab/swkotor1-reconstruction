// lane-m48-fade REPLACEMENT COPY of lane-m48-traps f-keys/src/wm_keys.cpp (D86 f): the only change is that the
// console-open cell 007A3A40 is read/written through the single link cell winmain_console_open_007a3a40 instead of the
// WinMain view field kotor_winmain::Globals::debug_7a3a40 (winmain.h no longer included). If a D86 (a) revision of
// wm_keys.cpp (008338F0 link-cell join) is produced, apply this change to that revision too (wm_keys.d86f.patch).
// lane-m48-traps / f-keys: WM_CHAR / WM_KEYDOWN handlers and their small callees.
//   HandleWMCharMessage    00402240  __cdecl  (consumer routes-lane kotor_original_calls.h: U32 kotor_cdecl_00402240(U32))
//   HandleWMKeyDownMessage 004024F0  __cdecl  (consumer: U32 kotor_cdecl_004024f0(U32 hwnd, U32 vk, U32 lparam))
// Reached from WinMessageHandler 00402800 (WM_KEYDOWN 00402D15, WM_CHAR 00402D48) on every key press on the menu.
// Behaviour cites original instruction addresses. D83 branch traps: only the DoConsole 00401F00 / DoConsoleHist
// 00401870 calls, gated by the console-open cell 007A3A40 (see TRAPS in FORK.json for the static proof).
#include <cstdint>
#include <cstring>
#include <new>
#include "36201186c4-window_shared_cells.h"   // kotor_menu_window_cells::manager_007a39fc / input_007a39e4 (sole owners)
// lane-m48-fade (D86 f): 007A3A40 is the single link cell winmain_console_open_007a3a40 (lane-m48-fade
// src/winmain_console_cells.cpp); the WinMain view field Globals::debug_7a3a40 is no longer used. Declaration identical
// to lane-m48-fade include/winmain_console_cells.h.
extern "C" volatile std::uint32_t winmain_console_open_007a3a40;   // 007A3A40
#include "e1121cddb3-message.h"               // kotor_message::Globals::flag_8338f0 (sole storage of 008338F0, WndProc view)
#include "0047cfa72a-startup_string.h"
#include "f0784ab101-base_ini.h"              // base_ini_reconstruction::PublicIni (CExoIni 005E6750/005E67C0/005E67E0)
#include "0e7d509bda-input_system.h"          // kotor_input_hover::InputInternal / RawInput (CExoInputInternal 0x3B0, raw 0x34)
#include "fd249c55aa-kotor_unreachable_trap.h"

using kotor_reconstruction::StartupString;
using U32 = std::uint32_t;

namespace kotor_menu20 { struct String { char* data; U32 length; __declspec(noinline) std::int32_t __thiscall as_int_005e5630(); }; }
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
namespace kotor_menu_10_19 { namespace dependencies { unsigned __int64 __stdcall unsigned_divide_006fb030(unsigned __int64, unsigned __int64); } }
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }   // D22/D46 canonical 007A39F4

extern "C" {
void* __fastcall kotor_fastcall_005ed460(void* app, void*);                             // GetCaptureEventFile (this file)
// existing composed-link providers (exact symbols)
void* __fastcall frame_GetWorldTimer_005ed4f0(void* client, void*);                       // CClientExoApp::GetWorldTimer
unsigned __int64 __fastcall kotor_fastcall_004adc70(void* timer, void*);                  // CWorldTimer::GetSnapshotTime (EDX:EAX)
void __fastcall frame_LogWriteString_005e6a70(void* file, void*, StartupString* text) noexcept(false);  // CExoFile::Write(CExoString)
void* __fastcall kotor_fastcall_005ed700(void* object, void*);                             // [[ecx+4]+4] getter (alternatename join)
void __fastcall kotor_fastcall_005df4e0(void* input, void*);                               // CExoInput::ClearEvents
void __fastcall kotor_fastcall_005f2980(void* internal, void*);                            // CClientExoAppInternal::TurnOffKeyboardModal
U32 __fastcall frame_R4_005ed480(void* client, void*);                                    // GetCaptureEventPlayBack
U32 __fastcall kotor_fastcall_005ed4d0(void* client, void*);                               // IsClientImeUI
void* __fastcall kotor_fastcall_005ed4e0(void* client, void*);                             // GetImeUI
void __fastcall kotor_fastcall_005dfb00(void* ime, void*, U32 hwnd, U32 wparam, U32 lparam); // CExoImeUI::handleIme (ime.cpp)
}

namespace kotor_console_cells {
// 007A3CB4: keyboard-mode-was-active word saved when the console opens. .bss 0. Only references:
// DoConsole 00401F39/004021C5 (read), HandleWMCharMessage 004023FB (read) / 00402431 (write). No prior owner.
std::uint32_t volatile keyboard_mode_007a3cb4 = 0;
}

namespace {
U32 w(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void* pw(const void* p, U32 off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w(p, off))); }
void* client() { return pw(kotor_menu_window_cells::manager_007a39fc, 4); }                      // [[007A39FC]+4]
void* input() { return kotor_menu_window_cells::input_007a39e4; }                                 // [007A39E4]
U32 console_open() { return winmain_console_open_007a3a40; }                                       // [007A3A40]
void set_console_open(U32 v) { winmain_console_open_007a3a40 = v; }                                  // 0040240F / 00402453

// Event-capture record shared by both handlers: "%d %c %d\n" (0073D678) with (time/1000, tag, code).
void capture(U32 tag, U32 code) {
    StartupString text;                                                         // 00402289 / 00402558 (005B3190)
    void* timer = frame_GetWorldTimer_005ed4f0(client(), nullptr);              // 004022A9 / 00402574
    const unsigned __int64 t = kotor_fastcall_004adc70(timer, nullptr);         // 004022B0 / 0040257B
    const U32 ms = static_cast<U32>(kotor_menu_10_19::dependencies::unsigned_divide_006fb030(t, 1000u));  // 004022BE __aulldiv
    kotor_reconstruction::StartupFormat005e5680(&text, "%d %c %d\n", ms, tag, code);  // 004022CE / 00402599
    frame_LogWriteString_005e6a70(kotor_fastcall_005ed460(client(), nullptr), nullptr, &text);  // 004022E4..EB / 004025AF..B6
}                                                                               // 004022FC / 004025C7 ~CExoString
}

extern "C" {
// ---- CClientExoApp field getters ([ecx+4] = CClientExoAppInternal) ----
U32 __fastcall kotor_fastcall_005ed470(void* app, void*) { return w(pw(app, 4), 0x8C); }    // GetCaptureEvents 005ED470..79
void* __fastcall kotor_fastcall_005ed460(void* app, void*) { return pw(pw(app, 4), 0x88); } // GetCaptureEventFile 005ED460..69
U32 __fastcall kotor_fastcall_005ed4c0(void* app, void*) { return w(pw(app, 4), 0x144); }  // GetClientLanguage 005ED4C0..C9
// 005EDA20 CClientExoApp::TurnOffKeyboardModal: mov ecx,[ecx+4]; jmp 005F2980
void __fastcall kotor_fastcall_005eda20(void* app, void*) { kotor_fastcall_005f2980(pw(app, 4), nullptr); }

// ---- CExoRawInputInternal keyboard mode (keyboard device +20h: IDirectInputDevice8A) ----
void __fastcall kotor_fastcall_005e3be0(kotor_input_hover::RawInput* raw, void*) {        // KeyboardModeOn
    raw->keyboard_mode = 1;                                                     // 005E3BE0
    IDirectInputDevice8A* keyboard = raw->keyboard;                             // 005E3BE7
    if (keyboard) keyboard->Unacquire();                                        // 005E3BEA..F1 ([vt+20h])
}
void __fastcall kotor_fastcall_005e3c00(kotor_input_hover::RawInput* raw, void*) {        // KeyboardModeOff
    raw->keyboard_mode = 0;                                                     // 005E3C00
    IDirectInputDevice8A* keyboard = raw->keyboard;                             // 005E3C07
    if (keyboard) keyboard->Acquire();                                          // 005E3C0A..11 ([vt+1Ch])
}
// ---- CExoInputInternal keyboard mode ----
void __fastcall kotor_fastcall_005dff60(kotor_input_hover::InputInternal* in, void*) {    // KeyboardModeOn
    if (in->raw) kotor_fastcall_005e3be0(in->raw, nullptr);                     // 005DFF63..6D
    in->word004 = 1;                                                            // 005DFF72
}
void __fastcall kotor_fastcall_005dff80(kotor_input_hover::InputInternal* in, void*) {    // KeyboardModeOff
    if (in->raw) { kotor_fastcall_005e3c00(in->raw, nullptr); in->word004 = 0; return; }  // 005DFF83..99
    in->word004 = 1;                                                            // 005DFF9B (no raw input: stays active)
}
// ---- CExoInput forwarders: mov ecx,[ecx+4]; jmp ----
void __fastcall kotor_fastcall_005df520(kotor_input_hover::Input* in, void*) { kotor_fastcall_005dff60(in->internal, nullptr); }  // 005DF520
void __fastcall kotor_fastcall_005df530(kotor_input_hover::Input* in, void*) { kotor_fastcall_005dff80(in->internal, nullptr); }  // 005DF530

// ---- 0040B2A0 CSWGuiManager::HandleKeyPress(key), __thiscall RET 4 ----
U32 __fastcall kotor_fastcall_0040b2a0(void* gui, void*, U32 key) {
    const U32 modal_count = w(gui, 0x98);                                       // 0040B2A2
    if (modal_count) {                                                          // 0040B2A8..AA
        void* top = pw(pw(gui, 0x94), (modal_count - 1) * 4);                   // 0040B2AC..B2
        void* focus = pw(gui, 0x18);                                            // 0040B2B7
        using ActiveControl = void* (__thiscall*)(void*);
        void* active = reinterpret_cast<ActiveControl>(w(pw(top, 0), 0x44))(top);  // 0040B2BA..BC panel [vt+44h]
        if (active == focus) {                                                  // 0040B2BF..C1
            using KeyPress = void (__thiscall*)(void*, U32);
            reinterpret_cast<KeyPress>(w(pw(focus, 0), 0x9C))(focus, key);      // 0040B2C3..CC control [vt+9Ch]
        }
        return 0;                                                               // 0040B2D3 (modal path always 0)
    }
    void* focus = pw(gui, 0x18);                                                // 0040B2D8
    if (!focus) return 0;                                                       // 0040B2DB..DD
    using KeyPress = void (__thiscall*)(void*, U32);
    reinterpret_cast<KeyPress>(w(pw(focus, 0), 0x9C))(focus, key);              // 0040B2DF..E8
    return 1;                                                                   // 0040B2EE
}

// ---- 00402240 HandleWMCharMessage(WORD ch) ----
U32 __cdecl kotor_cdecl_00402240(U32 argument) {
    const std::uint16_t ch = static_cast<std::uint16_t>(argument);              // 0040226E (si = low word)
    if (kotor_fastcall_005ed470(client(), nullptr) &&                           // 00402263..71
        kotor_fastcall_005ed460(client(), nullptr))                             // 0040227D..87
        capture(0x43u, ch);                                                     // 00402289..FC ('C', movzx si)
    const U32 language = kotor_fastcall_005ed4c0(client(), nullptr);           // 00402307..0A
    std::uint16_t console_key;                                                  // 0040230F..27
    if (language == 2) console_key = 0xB0;
    else if (language == 5) console_key = 3;
    else console_key = 0x60;
    if (ch == console_key) {                                                    // 0040232C..2F
        if (console_key == 3) kotor_message::bound_context->globals->flag_8338f0 = 0;  // 00402335..3B
        base_ini_reconstruction::PublicIni ini;                                 // [esp+4]
        ini.construct_005e6750();                                               // 00402349
        const U32 result = 1;                                                   // 0040234E esi = 1
        {
            StartupString value;                                                // 0040235B [esp+10h]
            {
                StartupString key("EnableCheats");                              // 00402369 (0073D668)
                StartupString section("Game Options");                          // 0040237C (0073D658)
                StartupString file(".\\swkotor.ini");                          // 0040238F (0073D648)
                ini.read_005e67c0(&value, &file, &section, &key);               // 004023B6 (value, file, section, key)
            }                                                                   // 004023C4..E0 ~file, ~section, ~key
            if (reinterpret_cast<kotor_menu20::String*>(&value)->as_int_005e5630() == 1) {  // 004023E9..F0
                if (console_open()) {                                           // 004023F2..F9
                    if (kotor_console_cells::keyboard_mode_007a3cb4)            // 004023FB..02
                        kotor_fastcall_005df530(static_cast<kotor_input_hover::Input*>(input()), nullptr);  // 0040240A
                    set_console_open(0);                                        // 0040240F
                } else {
                    const U32 active = static_cast<U32>(reinterpret_cast<std::uintptr_t>(kotor_fastcall_005ed700(input(), nullptr)));  // 00402421
                    kotor_console_cells::keyboard_mode_007a3cb4 = active == 0 ? 1u : 0u;  // 0040242C..31 (neg/sbb/inc)
                    kotor_fastcall_005df520(static_cast<kotor_input_hover::Input*>(input()), nullptr);  // 00402436
                    kotor_fastcall_005df4e0(input(), nullptr);                  // 00402441
                    kotor_fastcall_005eda20(client(), nullptr);                 // 0040244E
                    set_console_open(result);                                   // 00402453 (= esi = 1)
                }
            }
        }                                                                       // 00402462 ~value
        ini.destroy_005e67e0();                                                 // 00402473
        return result;                                                          // 00402478 (EAX = esi = 1)
    }
    if (!kotor_fastcall_005ed700(input(), nullptr)) return 0;                   // 0040248A..97 -> 004024D7
    if (console_open())                                                         // 00402499..A0
        kotor_unreachable_trap(0x004024A2ul, "HandleWMCharMessage@004024A2 DoConsole 00401F00: console cell 007A3A40 set only at 00402453 behind [Game Options] EnableCheats=1 in .\\swkotor.ini (absent in the run configuration)");
    void* gui = kotor_input_cells::gui_manager_007a39f4;                        // 004024C0
    if (gui && kotor_fastcall_0040b2a0(gui, nullptr, ch)) return 1;             // 004024C6..D5 -> 004024AB
    return 0;                                                                   // 004024D7..DB
}

// ---- 004024F0 HandleWMKeyDownMessage(hwnd, vk, lparam) ----
U32 __cdecl kotor_cdecl_004024f0(U32 hwnd, U32 vk, U32 lparam) {
    if (frame_R4_005ed480(client(), nullptr)) return 0;                         // 0040250A..19 (event playback active)
    if (kotor_fastcall_005ed470(client(), nullptr) &&                           // 00402535..40
        kotor_fastcall_005ed460(client(), nullptr))                             // 0040254C..56
        capture(0x4Bu, vk);                                                     // 00402558..C7 ('K', vk)
    const U32 index = vk - 0x25u;                                               // 004025CC
    if (index > 0xC0u) return 0;                                                // 004025CF..D5 -> 00402704
    // Byte table 00402728 / dword table 00402718: VK_LEFT..VK_DOWN -> 004025E9, VK_F10 -> 004026A5,
    // VK_PROCESSKEY (E5h) -> 004026BB, every other key -> 00402704.
    if (vk >= 0x25u && vk <= 0x28u) {                                           // 004025E9
        if (!kotor_fastcall_005ed700(input(), nullptr)) return 0;              // 004025E9..F6 (input active)
        if (console_open())                                                     // 004025FC..03
            kotor_unreachable_trap(0x00402605ul, "HandleWMKeyDownMessage@00402605 DoConsoleHist 00401870: console cell 007A3A40 set only at 00402453 behind [Game Options] EnableCheats=1 in .\\swkotor.ini (absent in the run configuration)");
        // Dword table 004027EC: 0 (VK_LEFT) -> 1Ch, 1 (UP) -> 1Dh, 2 (RIGHT) -> 1Eh, 3 (DOWN) -> 1Fh.
        static const U32 arrow_chars[4] = {0x1Cu, 0x1Du, 0x1Eu, 0x1Fu};         // 0040266B / 00402631 / 00402688 / 0040264E
        kotor_cdecl_00402240(arrow_chars[index]);                               // push imm; call 00402240
        return 0;
    }
    if (vk == 0x79u) return 1;                                                  // VK_F10 004026A5..BA
    if (vk == 0xE5u) {                                                          // VK_PROCESSKEY 004026BB
        if (!kotor_fastcall_005ed4d0(client(), nullptr)) return 0;              // 004026C1..CB
        void* ime = kotor_fastcall_005ed4e0(client(), nullptr);                 // 004026D2..D5
        if (!w(ime, 0x108)) return 0;                                           // 004026DA..E2
        kotor_fastcall_005dfb00(kotor_fastcall_005ed4e0(client(), nullptr), nullptr, hwnd, vk, lparam);  // 004026E4..FF
    }
    return 0;                                                                   // 00402704..09
}
}
