// lane-m48-b fork CLIENT, unit client-status-summary: CGuiInGame::ShowStatusSummary 0062EF90 (R4 menu_continuation member
// consumer) and its leaves: the status-summary pending-flash body 00625B50 (Ghidra: no function; reached by the tail
// jump at 0062F03A), CGuiInGame::ShowFlashingStatus 0062B0B0, CSWGuiMainInterface::ShowFlashingStatus 00687FE0,
// CClientExoApp::GetTogglePauseAtEndOfFrame 005EE110, CClientExoApp::SetInputClass 005EDA60.
// Evidence: dumpbin /disasm swkotor.exe 0062EF90..0062F03A, 00625B50..00625C36 with jump table 00625C38 (9 entries),
// 0062B0B0..0062B0BF, 00687FE0..00688049 with jump table 0068804C (4 entries), 005EE110..005EE11F, 005EDA60..005EDA63.
#include "e6446a7354-r4_lowers.h"
#include "a7dad25121-hover_path.h"

using namespace r4_lowers;

extern "C" {
void __fastcall kotor_fastcall_005edc20(void* client, void*, U32 paused, U32 reason, U32 force);  // client-pause
void __fastcall kotor_fastcall_006200e0(void* internal, void*, std::uint32_t input_class, std::uint32_t show) noexcept(false); // SetInputClass (gui-load)
U32 __fastcall kotor_fastcall_005ee110(void* client, void*);
void __fastcall kotor_fastcall_005eda60(void* client, void*, U32 input_class, U32 show);
void __fastcall kotor_fastcall_00687fe0(void* main_interface, void*, U32 id);
void __fastcall kotor_fastcall_0062b0b0(void* in_game, void*, U32 id);
void __fastcall kotor_fastcall_00625b50(void* status, void*);
}

namespace { void* client() { return pw(menu_continuation::app_manager_007a39fc, 4); } }

// 005EE110 CClientExoApp::GetTogglePauseAtEndOfFrame: ([[this+4]+37Ch] >> 2) & 1.
extern "C" U32 __fastcall kotor_fastcall_005ee110(void* client_app, void*) { return (w(pw(client_app, 4), 0x37c) >> 2) & 1u; } // 005EE110..005EE11C
// 005EDA60 CClientExoApp::SetInputClass: mov ecx,[ecx+4]; jmp 006200E0.
extern "C" void __fastcall kotor_fastcall_005eda60(void* client_app, void*, U32 input_class, U32 show) {
    kotor_fastcall_006200e0(pw(client_app, 4), nullptr, input_class, show); // 005EDA60..005EDA63
}
// 00687FE0 CSWGuiMainInterface::ShowFlashingStatus(id byte), RET 4.
extern "C" void __fastcall kotor_fastcall_00687fe0(void* main_interface, void*, U32 id_word) {
    const U32 id = static_cast<unsigned char>(id_word);                 // 00687FE0
    switch (id - 2u) {                                                  // 00687FE5..00687FED (table 0068804C)
    case 0: sw(main_interface, 0xa888, w(main_interface, 0xa888) & ~2u); break; // 0068800F
    case 1: sw(main_interface, 0xa73c, w(main_interface, 0xa73c) & ~2u); break; // 00688006
    case 2: sw(main_interface, 0xab20, w(main_interface, 0xab20) & ~2u); break; // 00687FF4
    case 3: sw(main_interface, 0xa9d4, w(main_interface, 0xa9d4) & ~2u); break; // 00687FFD
    default: break;
    }
    unsigned char* entry = bytes(main_interface) + 0xa460 + id * 0x14c; // 00688016..0068801C
    if ((w(entry, 0x44) & 2u) == 0) sw(entry, 0x44, w(entry, 0x44) | 2u); // 00688023..0068802D
    const U32 flags = w(entry, 0x144) | 1u;                             // 00688030..00688036
    sw(entry, 0x140, 0x40800000u);                                      // 00688039 (4.0f)
    sw(entry, 0x144, flags);                                            // 00688043
}
// 0062B0B0 CGuiInGame::ShowFlashingStatus(id): main interface [this+90h], tail call when non-null.
extern "C" void __fastcall kotor_fastcall_0062b0b0(void* in_game, void*, U32 id) {
    if (void* main_interface = pw(in_game, 0x90)) kotor_fastcall_00687fe0(main_interface, nullptr, id); // 0062B0B0..0062B0BA
}
// 00625B50 status summary: pending-flash sounds and status flashes, then clear the pending state.
extern "C" void __fastcall kotor_fastcall_00625b50(void* status, void*) {
    auto* gui = static_cast<kotor_input_hover::Manager*>(pw(status, 0x18));
    if (b8(status, 0x64) & 0x20) gui->PlayGuiSound(0x0c);               // 00625B54..00625B60
    if (b8(status, 0x64) & 0x40) gui->PlayGuiSound(0x0d);               // 00625B65..00625B70
    if (static_cast<signed char>(b8(status, 0x64)) < 0) gui->PlayGuiSound(0x0e); // 00625B75..00625B81
    for (U32 i = 0; i < 9; ++i) {                                       // 00625B88..00625C13 (table 00625C38)
        U32 flag = 0;
        switch (i) {
        case 0: flag = (w(status, 0x64) >> 4) & 1u; break;              // 00625B9C
        case 1: flag = w(status, 0x6c) != 0; break;                     // 00625BA4
        case 2: flag = w(status, 0x70) != 0; break;                     // 00625BB0
        case 3: flag = w(status, 0x74) != 0; break;                     // 00625BBC
        case 4: flag = b8(status, 0x68) < b8(status, 0x69); break;      // 00625BC8 (cmp/sbb/neg, unsigned)
        case 5: flag = b8(status, 0x69) < b8(status, 0x68); break;      // 00625BD8
        case 6: flag = 0; break;                                        // table[6] = 00625C0F (no flash)
        case 7: flag = (w(status, 0x64) >> 2) & 1u; break;              // 00625BE4
        case 8: flag = (w(status, 0x64) >> 3) & 1u; break;              // 00625BEC
        }
        if (flag != 0)                                                  // 00625BF5..00625BF7
            kotor_fastcall_0062b0b0(at(frame_R4_005ed690(client(), nullptr)), nullptr, i); // 00625BF9..00625C0A
    }
    const U32 cleared = w(status, 0x64) & 0xFFFFFE00u;                  // 00625C19..00625C1C
    sw(status, 0x6c, 0);                                                // 00625C22
    bytes(status)[0x68] = 0; bytes(status)[0x69] = 0;                   // 00625C25..00625C28
    sw(status, 0x70, 0); sw(status, 0x74, 0);                           // 00625C2B..00625C2E
    sw(status, 0x64, cleared);                                          // 00625C31
}

// 0062EF90 CGuiInGame::ShowStatusSummary.
void __thiscall menu_continuation::InGame::ShowStatusSummary_0062ef90() {
    void* const self = this;
    void* status = pw(self, 0xa8);                                      // 0062EF93
    if (!status) return;                                                // 0062EF99..0062EF9B
    if ((b8(status, 0x64) & 1u) == 0) { kotor_fastcall_00625b50(status, nullptr); return; } // 0062EFA1..0062EFA5 / 0062F033..0062F03A
    const unsigned char* options = static_cast<const unsigned char*>(static_cast<kotor_menu20::ClientApp*>(client())->get_options_005ed700()); // 0062EFAB..0062EFB3
    if ((options[0x14] & 0x20) == 0) { kotor_fastcall_00625b50(pw(self, 0xa8), nullptr); return; } // 0062EFB8..0062EFBC
    const U32 toggle = (w(self, 0xb38) != 0 || kotor_fastcall_005ee110(client(), nullptr) != 0) ? 1u : 0u; // 0062EFBE..0062EFDE
    void* summary = pw(self, 0xa8);                                     // 0062EFE3
    const U32 word = w(summary, 0x7c);
    sw(summary, 0x7c, word ^ ((word ^ toggle) & 1u));                   // 0062EFE9..0062EFF8
    kotor_fastcall_005edc20(client(), nullptr, 1, 6, 0);                // 0062EFF6..0062F007
    kotor_fastcall_0040bc70(static_cast<gui_ctor::Gui*>(pw(self, 0x38)), nullptr, pw(self, 0xa8), 1, 1); // 0062F00C..0062F01A
    kotor_fastcall_005eda60(client(), nullptr, 2, 0);                   // 0062F01F..0062F02C
}
