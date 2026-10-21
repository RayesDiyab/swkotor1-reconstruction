// lane-m47-core: CSWGuiManager / CSWGuiPanel / CSWGuiScene leaves.
//   0040B420 CSWGuiManager::UpdateAllFonts (+ the CGuiInGame chunk 0062B490..0062B4C3 it tail-jumps into; that
//            chunk has no other entry: tools/relxref.py 0062B490 -> only 0040B480 jmp). FIRST FRAME: StartServices
//            005F93FA calls SetSize 0040BE70 (0078D1D4 x 0078D1D8, manager created at 005F93D7 with 0x0 size), whose
//            changed-size path calls 0040B420 at 0040BE95 (see adapted/service-gui_0040BE70-25/gui_size.cpp).
//   0040AAA0 CSWGuiManager::~CSWGuiManager (+ 0040A0D0 UnloadGuiSounds)                 [routes-shutdown 0060426C]
//   0040C170 CSWGuiPanel::OnPanelRemoved (+ 0040C150 RemoveToolTip, 0040B4D0 ResetToolTipTimer) [gui-load tables +4C]
//   0040A200 CSWGuiManager::ReleaseMouse                                               [listbox mouse-up 0041A793]
//   004165E0 CSWGuiScene::~CSWGuiScene                                                  [MainMenu dtor 0067AC7E]
// Evidence: lane dis/<addr>.asm; .rdata strings 0073E0A4 "RIMS:MAINMENU", 0073E094 "RIMS:CHARGEN" (tools/rd.py).
// Panel/control slots used here (+0C, +14, +44, +68, +7C) are called through D1 fastcall views read from the object's
// table (same convention as gui-load's ObjectVtbl/PanelVtbl views).
#include "f9f7ab0530-startup_string.h"
#include "a0f716d4dd-menu_targets.h"
#include "5c6f651f13-window_shared_cells.h"
#include "39c670fc66-allocator_live.h"
#include <cstdint>
#include <cstring>

namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }          // input-cells (D22/D46 owner)
namespace kotor_scene3d { struct GuiSceneVTable; }
extern "C" {
extern const kotor_scene3d::GuiSceneVTable gui_scene_vtable_0073e3f0;               // 3d-render vtables2 (owner)
extern const struct GuiLoadDrawableMethods gui_load_drawable_methods_0073e324;      // gui-load gui_vtables.cpp (owner)
unsigned __fastcall frame_R4_005ed690(void* client, void*);                          // GetInGameGui (r4-lowers)
void __fastcall kotor_fastcall_0040a230(void* gui, void*);                           // HideSoftwareMouse (startup-misc)
void __fastcall exo_array_list_destruct_0056b6f0(void*, void*);                      // CExoArrayList dtor (3d-render)
void __fastcall kotor_fastcall_00408830(void*, void*, void*);                        // RemoveResourceImageFile (startup46)
}
namespace menu_resources { struct Manager; }
extern "C" menu_resources::Manager* __cdecl c2da_manager_007a39e8();                    // [007A39E8] (resman cell reader)

namespace {
using U32 = std::uint32_t;
U32 w(const void* p, U32 off) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
void sw(void* p, U32 off, U32 v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
void* pw(const void* p, U32 off) { void* v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
unsigned char* b(void* p) { return static_cast<unsigned char*>(p); }
template <class Fn> Fn slot(const void* object, U32 offset) {
    Fn fn; std::memcpy(&fn, static_cast<const unsigned char*>(pw(object, 0)) + offset, sizeof(fn)); return fn;
}
using Void0 = void (__fastcall*)(void*, void*);
using Void1 = void (__fastcall*)(void*, void*, U32);
using Ptr0 = void* (__fastcall*)(void*, void*);
using Delete = void* (__fastcall*)(void*, void*, U32);
void reset_font(void* panel) { slot<Void0>(panel, 0x68)(panel, nullptr); }           // panel table +68 (ReSetFont)
void delete_object(void* object) { (void)slot<Delete>(object, 0)(object, nullptr, 1u); }
void* client() { return pw(kotor_menu_window_cells::manager_007a39fc, 4); }
}

namespace menu_tables_required {
// 0040B420 CSWGuiManager::UpdateAllFonts (D1 form; consumers lane-config / menu-tables d1_bodies.h).
__declspec(noinline) void __fastcall UpdateAllFonts_0040b420(void* gui, void*) {
    for (int i = 0; i < static_cast<int>(w(gui, 0x8C)); ++i) {                    // 0040B423..4E (count reloaded)
        void* const panel = pw(pw(gui, 0x88), static_cast<U32>(i) * 4u);           // 0040B430..36
        if (pw(panel, 0x14) != nullptr) reset_font(panel);                         // 0040B439..42
    }
    if (void* const popup = pw(gui, 0x3C)) reset_font(popup);                      // 0040B450..5B
    if (frame_R4_005ed690(client(), nullptr) == 0) return;                         // 0040B45E..6E
    void* const in_game = reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_R4_005ed690(client(), nullptr))); // 0040B470..7E
    // 0040B480 JMP 0062B490 (chunk 0062B490..0062B4C3, ECX = in-game GUI):
    if (void* const main_interface = pw(in_game, 0x90)) reset_font(main_interface); // 0062B493..A0
    for (U32 off = 0x0C; off < 0x2C; off += 4)                                      // 0062B4A3..BF (8 panels)
        if (void* const panel = pw(in_game, off)) reset_font(panel);
}
}

extern "C" {
// 0040A0D0 CSWGuiManager::UnloadGuiSounds.
__declspec(noinline) void __fastcall kotor_fastcall_0040a0d0(void* gui, void*) {
    if (pw(gui, 0xA0) != nullptr) {                                                 // 0040A0D3..DB
        for (int i = 0; i < static_cast<signed char>(b(gui)[0xA4]); ++i) {         // 0040A0DD..11A (movsx, reloaded)
            if (void* const sound = pw(pw(gui, 0xA0), static_cast<U32>(i) * 4u)) {  // 0040A0F0..FB
                delete_object(sound);                                               // 0040A0FD..01
                sw(pw(gui, 0xA0), static_cast<U32>(i) * 4u, 0);                     // 0040A103..09
            }
        }
        live_free(pw(gui, 0xA0));                                                   // 0040A11C..23
        sw(gui, 0xA0, 0);                                                           // 0040A12B
    }
    b(gui)[0xA4] = 0;                                                               // 0040A136
}
// 0040AAA0 CSWGuiManager::~CSWGuiManager.
__declspec(noinline) void __fastcall kotor_fastcall_0040aaa0(void* gui, void*) {
    static const U32 owned[3] = {0x84u, 0x80u, 0x7Cu};
    for (U32 off : owned)                                                           // 0040AABE..B01
        if (void* const object = pw(gui, off)) { delete_object(object); sw(gui, off, 0); }
    kotor_fastcall_0040a230(gui, nullptr);                                          // 0040AB04..06 HideSoftwareMouse
    if (void* const popup = pw(gui, 0x3C)) { delete_object(popup); sw(gui, 0x3C, 0); } // 0040AB0B..18
    kotor_fastcall_0040a0d0(gui, nullptr);                                          // 0040AB1B..1D
    kotor_input_cells::gui_manager_007a39f4 = nullptr;                              // 0040AB28
    exo_array_list_destruct_0056b6f0(b(gui) + 0x94, nullptr);                       // 0040AB22..33
    exo_array_list_destruct_0056b6f0(b(gui) + 0x88, nullptr);                       // 0040AB38..42
    reinterpret_cast<kotor_reconstruction::StartupString*>(b(gui) + 0x74)->~StartupString(); // 0040AB47..52
}
// 0040B4D0 CSWGuiManager::ResetToolTipTimer.
__declspec(noinline) void __fastcall kotor_fastcall_0040b4d0(void* gui, void*) {
    if (w(gui, 0x4C) != 0) return;                                                  // 0040B4D3..D8
    kotor_menu20::ClientApp* const app = static_cast<kotor_menu20::ClientApp*>(client());
    if ((w(app->get_options_005ed700(), 0x14) & 0x400u) == 0) return;               // 0040B4DA..ED (test ch,4)
    sw(gui, 0x44, 0);                                                               // 0040B4EF
}
// 0040C150 CSWGuiManager::RemoveToolTip.
__declspec(noinline) void __fastcall kotor_fastcall_0040c150(void* gui, void*) {
    sw(gui, 0x1C, w(gui, 0x1C) & 0xFFFFFFF7u);                                      // 0040C153
    kotor_fastcall_0040b4d0(gui, nullptr);                                          // 0040C157
    if (void* const control = pw(gui, 0x40)) {                                      // 0040C15C..61
        sw(control, 0x44, w(control, 0x44) & 0xFFFFFFBFu);                          // 0040C163
        sw(gui, 0x40, 0);                                                           // 0040C167
    }
}
// 0040A200 CSWGuiManager::ReleaseMouse(control) RET 4.
__declspec(noinline) void __fastcall kotor_fastcall_0040a200(void* gui, void*, void* control) {
    if (control != nullptr && pw(gui, 0x10) == control) {                           // 0040A204..11
        sw(gui, 0x10, 0);                                                           // 0040A213
        slot<Void0>(control, 0x14)(control, nullptr);                               // 0040A216..18 [vtbl+14h]
    }
    sw(gui, 0x0C, 0);                                                               // 0040A21B
}
// 0040C170 CSWGuiPanel::OnPanelRemoved (gui-load spelling).
__declspec(noinline) void __fastcall gui_load_panel_on_removed_0040c170(void* panel, void*) {
    void* const gui = kotor_input_cells::gui_manager_007a39f4;                      // 0040C173
    kotor_fastcall_0040c150(gui, nullptr);                                          // 0040C17A
    if (void* const focus = pw(kotor_input_cells::gui_manager_007a39f4, 0x18)) {    // 0040C17F..89
        if (slot<Ptr0>(panel, 0x44)(panel, nullptr) == focus) {                     // 0040C18B..94 [vtbl+44h]
            void* const current = pw(kotor_input_cells::gui_manager_007a39f4, 0x18); // 0040C196..9B
            slot<Void1>(current, 0x7C)(current, nullptr, 0u);                       // 0040C19E..A2 [vtbl+7Ch](0)
        }
    }
    void* const manager = pw(panel, 0x18);                                          // 0040C1A5
    void* const hovered = pw(manager, 8);                                           // 0040C1A8
    if (!hovered) return;                                                           // 0040C1AB..AD
    const int count = static_cast<int>(w(panel, 0x24));                             // 0040C1AF
    for (int i = 0; i < count; ++i) {                                               // 0040C1B4..CA
        if (pw(pw(panel, 0x20), static_cast<U32>(i) * 4u) == hovered) {             // 0040C1C0..C2
            sw(manager, 8, 0);                                                      // 0040C1CF
            slot<Void1>(hovered, 0x0C)(hovered, nullptr, 0u);                       // 0040C1D6..DA [vtbl+0Ch](0)
            return;
        }
    }
}
// 004165E0 CSWGuiScene::~CSWGuiScene.
__declspec(noinline) void __fastcall gui_scene_destruct_004165e0(void* scene, void*) {
    const void* const table = &gui_scene_vtable_0073e3f0;
    std::memcpy(scene, &table, 4);                                                  // 00416601
    for (int i = 0; i < static_cast<int>(w(scene, 0x20)); ++i)                      // 00416607..36 (count reloaded)
        if (void* const object = pw(pw(scene, 0x1C), static_cast<U32>(i) * 4u)) delete_object(object); // 00416620..2E
    sw(scene, 0x20, 0);                                                             // 0041663D
    if (void* const camera = pw(scene, 0x18)) { delete_object(camera); sw(scene, 0x18, 0); } // 00416638..48
    if (void* const aur = pw(scene, 0x14)) { delete_object(aur); sw(scene, 0x14, 0); }       // 0041664B..58
    void* const resman = static_cast<void*>(c2da_manager_007a39e8());           // 0041665B
    if ((b(resman)[0x34] & 2u) != 0) {                                              // 00416660..67
        {
            kotor_reconstruction::StartupString name("RIMS:MAINMENU");              // 00416669..72 (0073E0A4)
            kotor_fastcall_00408830(static_cast<void*>(c2da_manager_007a39e8()), nullptr, &name); // 00416677..86
        }                                                                           // 0041668B..94 ~name
        b(static_cast<void*>(c2da_manager_007a39e8()))[0x34] &= 0xFD;               // 00416699..9E
    }
    if ((b(static_cast<void*>(c2da_manager_007a39e8()))[0x34] & 1u) != 0) {         // 004166A2..AC
        {
            kotor_reconstruction::StartupString name("RIMS:CHARGEN");               // 004166AE..B7 (0073E094)
            kotor_fastcall_00408830(static_cast<void*>(c2da_manager_007a39e8()), nullptr, &name); // 004166BC..CC
        }                                                                           // 004166D1..DA ~name
        b(static_cast<void*>(c2da_manager_007a39e8()))[0x34] &= 0xFE;               // 004166DF..E4
    }
    exo_array_list_destruct_0056b6f0(b(scene) + 0x1C, nullptr);                     // 004166E8..F0
    const void* const base = &gui_load_drawable_methods_0073e324;
    std::memcpy(scene, &base, 4);                                                   // 004166FA (CSWGuiDrawable table)
}
}
