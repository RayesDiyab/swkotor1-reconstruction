// lane-m48-traps fork f-input: CSWGuiManager input bodies reachable from ProcessInput 006227E0 on the menu.
// Verbatim extractions (D9 precedent) from lane-input r3 lane-input/src/gui_manager_input.cpp (sha256 5e7533ee00bd48653add0e0b8d4a4ccdfc8ca2ab1c5b4ffc6d870aa5c51449aa):
// 0040C570 HandleLMouseDown, 0040C8E0 HandleInputEvent. Authored here (lane-m48-traps, citations against swkotor.exe):
// 0040C610 HandleRMouseDown, 0040A1A0 HandleRMouseUp, 0040C650 HandleZMouse, 005E9250 CExoTimers::GetLowResolutionTimer.
#include "fa48658349-hover_path.h"
#include "df72b7ab87-window_shared_cells.h"
#include <cstdint>

using namespace kotor_input_hover;


// ---- 0040C570 CSWGuiManager::HandleLMouseDown(value) ----
int Manager::HandleLMouseDown(int value) {
    flags ^= (flags ^ static_cast<U32>(value)) & 1u;                       // 0040C573..87
    if (flags & 8u) RemoveToolTip();                                       // 0040C583..8E (low byte of new flags)
    if (captured && capture_flag == 1) return 0;                           // 0040C593..9E -> 0040C5FE
    AppManager* app = static_cast<AppManager*>(kotor_menu_window_cells::manager_007a39fc);  // 0040C5A0
    if (app->client->GetInputClass() == 3) {                               // 0040C5A6..B1
        for (int i = panel_count - 1; i >= 0; --i) {                       // 0040C5B3..D6
            Panel* p = panels[i];                                          // 0040C5C0..C6
            (p->*(p->vtable->handle_input_event))(0x1F9, 1);               // 0040C5C9..D2
        }
    }
    UpdateMouseOverControl();                                              // 0040C5D8..DA
    SetTemporarilyDisableToolTip(1);                                       // 0040C5DF..E3
    Control* over = mouse_over;                                            // 0040C5E8
    if (!over) return 0;                                                   // 0040C5EB..ED
    (over->*(over->vtable->handle_lmouse_down))();                         // 0040C5EF..F1
    return 1;                                                              // 0040C5F5
}

// ======== 0040C8E0 CSWGuiManager::HandleInputEvent(event, value) ========
#include "84a5500766-startup_string.h"
#include "ed8f77ad6d-allocator_live.h"
#include "7a89cdda39-input_key_ids.h"
namespace menu_resources { struct Manager; }
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; }  // M41 manager-cell-owner.obj
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }
namespace kotor_input_required {
std::uint32_t __fastcall low_resolution_timer_005e9250(void* timers, void*);                  // CExoTimers::GetLowResolutionTimer RET0
void __fastcall remove_resource_image_file_00408830(menu_resources::Manager*, void*, kotor_reconstruction::StartupString*);  // RET4
}
namespace kotor_input_hover {
void __fastcall WordArraySetSize00405150(struct WordArray* self, void*, int size);           // input_system.cpp
}
namespace {
struct PanelList { Panel** data; int count; int capacity; };               // CExoArrayList<CSWGuiPanel*>
// 00416770 CExoArrayList ctor(int initial): zero, allocate when nonzero (inlined use: initial 0).
int index_of_006c2a40(const PanelList* list, Panel* p) {                   // 006C2A40 CExoArrayList<int>::indexOf
    for (int i = 0; i < list->count; ++i) if (list->data[i] == p) return i;
    return -1;
}
unsigned char& resman_flags() {                                            // [[007A39E8]+34h] byte
    return reinterpret_cast<unsigned char*>(kotor_resource_demand::resource_manager_007a39e8)[0x34];
}
}
void Manager::HandleInputEvent(int event, int value) {
    last_input_event = event;                                              // 0040C90A ([esi+68h])
    int mapped = event;                                                    // EBX
    if (value != 0) {                                                      // 0040C905..0D
        switch (mapped - 0x33) {                                           // table 0040CBDC
        case 0: if (value == 1) mapped = 0x3E; else if (value == -1) mapped = 0x3D; break;  // 0040C922
        case 1: if (value == 1) mapped = 0x40; else if (value == -1) mapped = 0x3F; break;  // 0040C93A
        case 4: if (value == 1) mapped = 0x3A; else if (value == -1) mapped = 0x39; break;  // 0040C952
        case 5: if (value == 1) mapped = 0x3C; else if (value == -1) mapped = 0x3B; break;  // 0040C96A
        default: break;                                                    // 2,3 -> 0040C980
        }
        struct BaseView { void* w0; void* timers; };
        const U32 now = kotor_input_required::low_resolution_timer_005e9250(
            static_cast<BaseView*>(kotor_menu_10_19::dependencies::base_007a39e0)->timers, nullptr);  // 0040C980..90
        value = 1;                                                         // 0040C988 (argument slot overwritten)
        const int k = mapped - 0x2F;                                       // 0040C995..9D table 0040CBF4
        if (k >= 0 && k <= 3) {
            const unsigned char last = last_direction;                     // [esi+72h]
            const unsigned char lo = (k <= 1) ? 0x30 : 0x2E;               // 2F/30 -> 0040C9C4 (31..32); 31/32 -> 0040C9A4 (2F..30)
            const unsigned char hi = (k <= 1) ? 0x32 : 0x30;
            if (last == 0) last_direction = static_cast<unsigned char>(mapped);           // 0040C9E8
            else if (last > lo && last <= hi) {                            // 0040C9AB..B3 / 0040C9CB..D3
                if (now - direction_time < 0x96u) return;                  // 0040C9B5..E2 -> 0040CBC7
                last_direction = static_cast<unsigned char>(mapped);       // 0040C9E8
            }
            direction_time = now;                                          // 0040C9EB
        }
    }
    switch (mapped - 0xB4) {                                               // tables 0040CC20 / 0040CC04
    case 0x00: case 0x2B: if (value != 0) mapped = 0x28; break;            // B4/DF -> 0040CA0B
    case 0x01: case 0x07: mapped = 0x27; break;                            // B5/BB -> 0040CA16
    case 0x02: mapped = 0x3D; break;                                       // B6
    case 0x03: mapped = 0x3E; break;                                       // B7
    case 0x04: mapped = 0x3F; break;                                       // B8
    case 0x05: mapped = 0x40; break;                                       // B9
    default: break;                                                        // 0040CA37
    }
    if (modal_count) {                                                     // 0040CA37..3F
        Panel* top = modal_panels[modal_count - 1];                        // 0040CA41..47
        (top->*(top->vtable->handle_input_event))(mapped, value);          // 0040CA4B..4F
    } else {
        PanelList copy{nullptr, 0, 0};                                     // 0040CA57..5C (00416770 with 0)
        WordArraySetSize00405150(reinterpret_cast<WordArray*>(&copy), nullptr, panel_count);  // 0040CA61..70
        for (int i = 0; i < panel_count; ++i) copy.data[i] = panels[i];    // 0040CA75..9A (count reloaded)
        const PanelList* live = reinterpret_cast<const PanelList*>(&panels);
        for (int i = 0; i < copy.count; ++i) {                             // 0040CA9C..CE
            Panel* p = copy.data[i];
            if (index_of_006c2a40(live, p) != -1)                          // 0040CAA9..B8
                (p->*(p->vtable->handle_input_event))(mapped, value);      // 0040CABA..C4 ([esp+38h])
        }
        live_free(copy.data); copy.data = nullptr;                         // 0040CADC 0056B6F0 body: free(data); data = 0
    }
    for (int i = panel_count - 1; i >= 0; --i) {                           // 0040CAE1..33
        Panel* p = panels[i];
        const U32 f = p->flags;
        const U32 kind = (f >> 9) & 3u;                                    // 0040CAFF..04
        if (kind == 2) {                                                   // 0040CB07..09
            RemovePanel(p);                                                // 0040CB0B..0E
            (p->*(p->vtable->scalar_destroy))(1);                          // 0040CB13..19
        } else if (kind != 0) {                                            // 0040CB1D..1F
            p->flags = f & 0xFFFFF8FFu;                                    // 0040CB21..27
            RemovePanel(p);                                                // 0040CB2A..2D
        }
    }
    if (resman_flags() & 2) {                                              // 0040CB35..3E
        {
            kotor_reconstruction::StartupString name(kotor_input_ids::rims_mainmenu_0073e0a4);  // 0040CB40..49
            kotor_input_required::remove_resource_image_file_00408830(kotor_resource_demand::resource_manager_007a39e8, nullptr, &name);  // 0040CB4E..61
        }                                                                  // 0040CB72 ~CExoString
        resman_flags() &= 0xFD;                                            // 0040CB77..7C (after ~CExoString at 0040CB72)
    }
    if (resman_flags() & 1) {                                              // 0040CB80..89
        {
            kotor_reconstruction::StartupString name(kotor_input_ids::rims_chargen_0073e094);  // 0040CB8B..94
            kotor_input_required::remove_resource_image_file_00408830(kotor_resource_demand::resource_manager_007a39e8, nullptr, &name);  // 0040CB99..A8
        }                                                                  // 0040CBB9 ~CExoString
        resman_flags() &= 0xFE;                                            // 0040CBBE..C3
    }
}

// ======== authored (lane-m48-traps f-input) ========
extern "C" unsigned long __stdcall unresolved_GetTickCount_005ec200();     // 005EC200: jmp [0073D0F8] GetTickCount

// ---- 005E9250 CExoTimers::GetLowResolutionTimer (thiscall RET0): mov ecx,[ecx]; jmp 005EC200 ----
std::uint32_t __fastcall kotor_input_required::low_resolution_timer_005e9250(void* timers, void*) {
    (void)timers;                                                          // 005E9250 ECX = [this] (internal; unused by the tail)
    return static_cast<std::uint32_t>(unresolved_GetTickCount_005ec200()); // 005E9252 tail JMP
}

// ---- 0040C610 CSWGuiManager::HandleRMouseDown (thiscall RET0, EAX 1/0) ----
extern "C" int __fastcall kotor_fastcall_0040c610(Manager* self, void*) {
    if (self->flags & 8u) self->RemoveToolTip();                           // 0040C613..19
    if (self->captured != nullptr) return 0;                               // 0040C61E..23 -> 0040C64B
    self->UpdateMouseOverControl();                                        // 0040C625..27
    self->SetTemporarilyDisableToolTip(1);                                 // 0040C62C..30
    Control* over = self->mouse_over;                                      // 0040C635
    if (over == nullptr) return 0;                                         // 0040C638..3A
    (over->*(over->cvt()->handle_rmouse_down))();                          // 0040C63C..3E (vt+84h)
    return 1;                                                              // 0040C644
}

// ---- 0040A1A0 CSWGuiManager::HandleRMouseUp (thiscall RET0, EAX 1/0) ----
extern "C" int __fastcall kotor_fastcall_0040a1a0(Manager* self, void*) {
    Control* captured = self->captured;                                    // 0040A1A2
    if (captured == nullptr) return 0;                                     // 0040A1A5..A9
    if (self->capture_flag != 4) return 0;                                 // 0040A1AB..AF -> 0040A1BC
    (captured->*(captured->vtable->handle_rmouse_up))();                   // 0040A1B1..B3 (vt+24h)
    return 1;                                                              // 0040A1B6
}

// ---- 0040C650 CSWGuiManager::HandleZMouse(int delta) (thiscall RET4, EAX 1/0) ----
// 0x88888889 / SHR 6 = unsigned division by 120 (WHEEL_DELTA): one list-box scroll event per notch.
extern "C" int __fastcall kotor_fastcall_0040c650(Manager* self, void*, int delta) {
    if (self->flags & 8u) self->RemoveToolTip();                           // 0040C656..5D
    Panel* panel = nullptr;                                                // [esp+0Ch] (written by the callee)
    Control* hit = nullptr;                                                // [esp+8h]
    self->HitCheckMouse(self->mouse_x, self->mouse_y, &panel, &hit, 0);    // 0040C662..77 (push 0, &hit, &panel, y, x)
    Control* control = hit;                                                // 0040C67C
    if (control != self->mouse_over) self->mouse_over = control;          // 0040C680..85
    if (control == nullptr) return 0;                                      // 0040C688..93
    if ((control->*(control->cvt()->as_listbox))()) {                      // 0040C696..A0 (vt+70h)
        if (delta > 0) {                                                   // 0040C6A6..AC
            U32 notches = static_cast<U32>(delta - 1) / 120u + 1u;         // 0040C6AE..BD
            do {
                Control* list = reinterpret_cast<Control*>((control->*(control->cvt()->as_listbox))());  // 0040C6C0..C4
                (list->*(list->vtable->handle_input_event))(0x1F4, 1);     // 0040C6C7..D2
            } while (--notches != 0);                                      // 0040C6D5..D6
        } else if (delta < 0) {                                            // 0040C703 (JGE -> 0040C776 when 0)
            U32 notches = static_cast<U32>(-1 - delta) / 120u + 1u;        // 0040C705..16
            do {
                Control* list = reinterpret_cast<Control*>((control->*(control->cvt()->as_listbox))());  // 0040C717..1B
                (list->*(list->vtable->handle_input_event))(0x1F5, 1);     // 0040C71E..29
            } while (--notches != 0);                                      // 0040C72C..2D
        } else {
            return 1;                                                      // 0040C703 JGE 0040C776 (delta == 0)
        }
        if (self->tooltip_mode == 1) self->SetTemporarilyDisableToolTip(0);  // 0040C6D8..E2
        (control->*(control->cvt()->as_listbox))()->HandleMouseMove();     // 0040C6E7..F0 (0041C400)
        return 1;                                                          // 0040C6F7
    }
    if (!(control->*(control->cvt()->is_selectable))())                    // 0040C731..3D (vt+80h)
        control = control->GetSelectableParent();                          // 0040C73F..46
    if (control == nullptr) return 1;                                      // 0040C748..4A
    if (!(control->*(control->cvt()->slot58))()) return 1;                 // 0040C74C..55 (vt+58h)
    if (delta == 0) return 1;                                              // 0040C757..5D
    (control->*(control->vtable->handle_input_event))(delta <= 0 ? 0x1F5 : 0x1F4, 1);  // 0040C75F..73 (SETLE + 1F4h)
    return 1;                                                              // 0040C778
}
