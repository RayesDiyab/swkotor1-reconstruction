// Main-menu hover/click leaf bodies. Every behavior cites original instruction
// addresses (GOG swkotor.exe, image base 0x400000). Modern codegen; semantic
// equivalence only, no byte-equality claim.
#include "e2684e9878-hover_path.h"
#include "369787b919-window_shared_cells.h"   // M40 task6 cell: manager_007a39fc

using namespace kotor_input_hover;
namespace cells = kotor_input_cells;

// ---- 00414E10 CSWGuiTextParams::SetColor (thiscall, 1 stack word, RET4) ----
// Closes the donor's native provider symbol ColorReceiver::SetColor (task-10
// menu-enter-matching-r1 / task-24 review: "verified native thiscall SetColor").
void ColorReceiver::SetColor(const MenuColor* color) {
    TextParams* self = reinterpret_cast<TextParams*>(this);
    GuiText* text = self->text_object;            // 00414E1B load precedes the copy
    self->color.words[0] = color->words[0];       // 00414E22
    self->color.words[1] = color->words[1];       // 00414E27
    self->color.words[2] = color->words[2];       // 00414E2E
    if (!text) return;                            // 00414E1E/32
    AurGuiString* string = text->gui_string;      // 00414E34
    if (!string) return;                          // 00414E37/39
    // 00414E3D..41: tail JMP [vtbl+0C] with the stack word replaced by &self->color.
    (string->*(string->vtable->set_color))(&self->color);
}

// ---- 00414E80 CSWGuiTextParams::RestoreDefaultColor (thiscall, RET) ----
void TextParams::RestoreDefaultColor() {
    receiver()->SetColor(&default_color);          // 00414E80..84
}

// ---- 004181D0 CSWGuiControl::SetActive (thiscall, RET4) ----
void Control::SetActive(int active) {
    flags ^= (flags ^ static_cast<U32>(active)) & 1u;   // 004181D0..DF: bit0 := active bit0
}

// ---- 00418E00 CSWGuiButton::SetActive (thiscall, RET4) ----
void Button::SetActive(int active) {
    flags ^= (flags ^ static_cast<U32>(active)) & 1u;   // 00418E00..12
    if (active) {                                        // 00418E1B: full word, not bit0
        text.flags |= 0x40u;                             // 00418E20
        text.receiver()->SetColor(&borrowed_highlight_color_0078d3c0);  // 00418E24..2C tail
    } else {
        text.flags &= ~0x40u;                            // 00418E31
        text.RestoreDefaultColor();                      // 00418E35
    }
}

// ---- 00418750 CSWGuiControl::HandleInputEvent (thiscall, RET8) ----
void Control::HandleInputEvent(int event, int value) {
    if (value) {                                         // 00418755
        if (event == 0) {                                // 00418766
            (this->*(cvt()->set_active))(1);             // 0041877A..80
        } else if (event == 1 && (flags & 1u)) {         // 00418768..6F
            (this->*(cvt()->set_active))(0);             // 00418771..75
        }
    }
    const int count = event_count;                       // 00418783 (loaded once)
    ControlEvent* entry = events;                        // 0041878C
    for (int i = 0; i < count; ++i, ++entry) {
        if (entry->id == event && entry->func) {         // 00418790..9A
            last_event = event;                          // 004187AB
            last_event_value = value;                    // 004187AE
            (entry->object->*(entry->func))(this);       // 004187B1..B4
            return;
        }
    }
}

// ---- 004176C0 (control-receiver tooltip removal; Ghidra label CSWGuiManager::RemoveToolTip) ----
void Control::RemoveToolTip() {
    gui_manager()->RemoveToolTip();        // 004176C3..C9
    flags &= ~0x40u;                                     // 004176CE
}

// ---- 004189A0 CSWGuiControl::HandleMouseOver (thiscall, RET4) ----
void Control::HandleMouseOver(int over) {
    if (over) {                                          // 004189A4
        gui_manager()->ResetToolTipTimer();  // 004189A8..AE
        return;
    }
    if (flags & 0x40u) RemoveToolTip();                  // 004189B6..BC
}

// ---- 004187C0 CSWGuiControl::HitCheckMouse (thiscall, RET8) ----
Control* Control::HitCheckMouse(int px, int py) {
    if (!(flags & 2u) || (flags & 0x20u)) return nullptr;            // 004187C5..CD
    const int right = static_cast<int>(static_cast<U32>(width) + static_cast<U32>(x));    // 004187D8..DB (wrapping ADD)
    const int bottom = static_cast<int>(static_cast<U32>(height) + static_cast<U32>(y));  // 004187EB..EE
    if (px < x || px > right) return nullptr;                         // 004187D3 JL / 004187E0 JG
    if (py < y || py > bottom) return nullptr;                        // 004187E9 JL / 004187F3 JLE->hit
    return this;                                                      // EAX = ECX (004187C0)
}

// ---- 00418A50 CSWGuiControl::GetSelectableParent (thiscall, RET) ----
Control* Control::GetSelectableParent() {
    for (Control* p = parent_control; p; p = p->parent_control) {   // 00418A50..78
        if ((p->*(p->cvt()->is_selectable))()) return p;              // 00418A66..6E
    }
    return nullptr;                                                   // 00418A7A
}

// ---- 004189D0 CSWGuiControl::GetIsSelectable (thiscall, RET) ----
int Control::GetIsSelectable() {
    GuiObject* o = owner;                                             // 004189D4
    if (o && (o->*(o->vtable->as_control))()) {                       // 004189D9..E4
        Control* c = (o->*(o->vtable->as_control))();                 // 004189E6..EA (second call)
        if (!(c->flags & 8u)) return 0;                               // 004189ED..F1
        return (this->*(cvt()->as_navigable))() != nullptr ? 1 : 0;   // 004189F3..FC
    }
    if (!(flags & 8u)) return 0;                                      // 00418A03..07
    return (this->*(cvt()->as_navigable))() != nullptr ? 1 : 0;       // 00418A09..15
}

// ---- 0041AB90 CSWGuiControl::HandleLMouseDown (thiscall, RET) ----
void Control::HandleLMouseDown() {
    if (!(this->*(cvt()->is_selectable))()) {                         // 0041AB93..9D
        Control* parent = GetSelectableParent();                      // 0041ABA1
        if (parent) {
            (parent->*(parent->vtable->handle_lmouse_down))();        // 0041ABAA..AF tail
            return;
        }
    }
    gui_manager()->CaptureMouse(this, 1);              // 0041ABB2..BB
}

// ---- 0041ABD0 CSWGuiControl::HandleLMouseUp (thiscall, RET) ----
void Control::HandleLMouseUp() {
    Control* over = gui_manager()->mouse_over;          // 0041ABD0..D8
    if (over && (over == this || over->GetSelectableParent() == this)) {  // 0041ABDB..EA
        if (gui_manager()->flags & 1u)                  // 0041ABEC..F6
            (this->*(vtable->handle_input_event))(0x1F9, 1);          // 0041ABF8..AC03
        if (flags & 8u)                                               // 0041AC06
            (this->*(vtable->handle_input_event))(0x27, 1);           // 0041AC0C..14 (click event)
    }
    gui_manager()->ReleaseMouse(this);                 // 0041AC17..1E
    (this->*(vtable->release_capture))();                             // 0041AC23..28 tail
}

// ---- 0041A980 CSWGuiNavigable::HandleMouseOver (thiscall, RET4) ----
void Navigable::HandleMouseOver(int over) {
    if (over) {                                                       // 0041A986
        if (!(flags & 1u)) {                                          // 0041A98C
            (this->*(vtable->handle_input_event))(0, 1);              // 0041A992..98 enter event
            gui_manager()->PlayGuiSound(1);             // 0041A99B..A3 (global read after call)
        }
    } else {
        (this->*(vtable->handle_input_event))(1, 1);                  // 0041A9B5..BD exit event
    }
    Control::HandleMouseOver(over);                                   // 0041A9AB / 0041A9C3
}

// ---- 0041A9D0 CSWGuiNavigable::HandleInputEvent (thiscall, RET8) ----
void Navigable::HandleInputEvent(int event, int value) {
    if (value) {                                                      // 0041A9D4
        Control* cur = this;                                          // 0041A9E1
        for (;;) {
            if (!cur) goto done;                                      // 0041A9E3..E5
            if (!(cur->*(cur->cvt()->as_navigable))()) break;         // 0041A9E7..F0
            Navigable* nav = (cur->*(cur->cvt()->as_navigable))();    // 0041A9F2..F6 (second call)
            switch (event) {                                          // 0041A9F9..AA0B tables 0041AA6C/0041AA58
            case 0x31: case 0x3D: cur = nav->neighbors[0]; break;     // 0041AA17
            case 0x2F: case 0x3F: cur = nav->neighbors[1]; break;     // 0041AA1B
            case 0x32: case 0x3E: cur = nav->neighbors[2]; break;     // 0041AA12
            case 0x30: case 0x40: cur = nav->neighbors[3]; break;     // 0041AA20
            default: break;                                           // 0041AA23 (unchanged)
            }
            if (!cur) goto done;                                      // 0041AA23..25
            if (cur->flags & 8u) break;                               // 0041AA27..2B
            if (cur == this) break;                                   // 0041AA2D..2F
        }
        {
            GuiObject* o = owner;                                     // 0041AA31
            if (o && cur != this)                                     // 0041AA34..3A
                (o->*(o->vtable->set_active_control))(cur, 1);        // 0041AA3C..41
        }
    }
done:
    Control::HandleInputEvent(event, value);                          // 0041AA45..4D
}

// ---- 0041AD40 CSWGuiButton::HandleInputEvent (thiscall, RET8) ----
void Button::HandleInputEvent(int event, int value) {
    if (value && event == 0x27) {                                     // 0041AD4B..54
        GuiObject* o = owner;                                         // 0041AD56
        if (o) {
            Panel* panel = nullptr;
            bool have = false;
            if ((o->*(o->vtable->as_panel))()) {                      // 0041AD5D..62
                GuiObject* o2 = owner;                                // 0041AD64 reload
                panel = (o2->*(o2->vtable->as_panel))();              // 0041AD69..6B (no null check)
                have = true;
            } else {
                GuiObject* o2 = owner;                                // 0041AD64 reload
                if ((o2->*(o2->vtable->as_control))()) {              // 0041AD70..77
                    GuiObject* o3 = owner;                            // 0041AD79 reload
                    Control* c = (o3->*(o3->vtable->as_control))();   // 0041AD7C..7E
                    GuiObject* o4 = c->owner;                         // 0041AD81
                    panel = (o4->*(o4->vtable->as_panel))();          // 0041AD84..86 (no null check)
                    have = true;
                }
            }
            if (have) {
                Manager* m = panel->manager;                          // 0041AD89
                if (m) m->PlayGuiSound(static_cast<char>(click_sound));  // 0041AD8C..96 (MOV DL, zero-extended push)
            }
        }
    }
    Navigable::HandleInputEvent(event, value);                        // 0041AD9B..9F
}

// ---- 0067B450 / 0067B470 CSWGuiMainMenu::OnEnterButton / OnExitButton ----
// thiscall member form (ECX ignored, one stack word, RET4) so the address can be
// stored in a ControlEvent PMF slot without a function-pointer cast.
void MainMenu::OnEnterButton(Control* control) {
    Button* b = static_cast<Button*>(control);   // control+0x16C receiver (0067B450..59)
    b->text.receiver()->SetColor(&borrowed_enter_color_007a23c0);     // 0067B454 push 7A23C0, call 00414E10
}
void MainMenu::OnExitButton(Control* control) {
    Button* b = static_cast<Button*>(control);   // 0067B470..79
    b->text.receiver()->SetColor(&borrowed_exit_color_007a23b4);      // 0067B474 push 7A23B4, call 00414E10
}

// ---- 0040A140 CSWGuiManager::PlayGuiSound (thiscall, RET4) ----
void Manager::PlayGuiSound(char sound) {
    const signed char s = static_cast<signed char>(sound);           // 0040A140 MOV DL
    if (s == -1) return;                                              // 0040A144 CMP DL,FF
    if (s >= sound_count) return;                                     // 0040A149 signed JGE
    if (!sounds) return;                                              // 0040A151..59
    SoundSource* src = sounds[s];                                     // 0040A15B..5E MOVSX index
    if (!src) return;                                                 // 0040A161
    src->Play();                                                      // 0040A165..67
}

// ---- 0040B4D0 CSWGuiManager::ResetToolTipTimer (thiscall, RET) ----
void Manager::ResetToolTipTimer() {
    if (tooltip_mode) return;                                         // 0040B4D3..D8
    AppManager* app = static_cast<AppManager*>(kotor_menu_window_cells::manager_007a39fc);  // 0040B4DA
    ClientOptions* options = app->client->GetClientOptions();         // 0040B4DF..E2
    if (options->flags14 & 0x400u) tooltip_timer = 0;                 // 0040B4E7..EF (TEST CH,4)
}

// ---- 0040A1C0 CSWGuiManager::CaptureMouse (thiscall, RET8) ----
void Manager::CaptureMouse(Control* control, int flag) {
    Control* previous = captured;                                     // 0040A1C3
    if (previous) (previous->*(previous->vtable->release_capture))(); // 0040A1C6..CC
    captured = control;                                               // 0040A1D7 / 0040A1E9
    capture_flag = flag;                                              // 0040A1DA / 0040A1EC
}

// ---- 0040A200 CSWGuiManager::ReleaseMouse (thiscall, RET4) ----
void Manager::ReleaseMouse(Control* control) {
    if (control && captured == control) {                             // 0040A20A..11
        captured = nullptr;                                           // 0040A213
        (control->*(control->vtable->release_capture))();             // 0040A216..18
    }
    capture_flag = 0;                                                 // 0040A21B
}

// ---- 0040A170 CSWGuiManager::HandleLMouseUp (thiscall, RET) ----
int Manager::HandleLMouseUp() {
    Control* c = captured;                                            // 0040A173
    if (c && capture_flag == 1) {                                     // 0040A178..80
        (c->*(c->vtable->handle_lmouse_up))();                        // 0040A182..84
        flags &= ~1u;                                                 // 0040A187
        return 1;                                                     // 0040A18B
    }
    flags &= ~1u;                                                     // 0040A194
    return 0;                                                         // 0040A176 / 0040A192
}

// ---- 0040ABE0 CSWGuiManager::HitCheckMouse (thiscall, RET14) ----
// Observed quirk: the modal-panel path returns the hit control word in EAX
// (0040AC30/0040AC87), the normal path returns 1/0.
int Manager::HitCheckMouse(int px, int py, Panel** out_panel, Control** out_control, int) {
    const int modal = modal_count;                                    // 0040ABE4
    if (modal) {
        Panel* top = modal_panels[modal - 1];                         // 0040ABEE..F4
        Control* hit = (top->*(top->pvt()->hit_check_mouse))(px, py); // 0040ABF8..AC04
        if (out_panel) *out_panel = hit ? modal_panels[modal - 1] : nullptr;  // 0040AC07..21 (reloaded)
        if (out_control) *out_control = hit;                          // 0040AC27..30
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(hit));
    }
    for (int i = panel_count - 1; i >= 0; --i) {                      // 0040AC36..3F / 0040AC64
        Panel* p = panels[i];                                         // 0040AC50..56
        Control* hit = (p->*(p->pvt()->hit_check_mouse))(px, py);     // 0040AC59..5D
        if (hit) {
            if (out_panel) *out_panel = panels[i];                    // 0040AC8C..9D (reloaded)
            if (out_control) *out_control = hit;                      // 0040AC9F..A7
            return 1;                                                 // 0040ACAC
        }
    }
    if (out_panel) *out_panel = nullptr;                              // 0040AC67..6F
    if (out_control) *out_control = nullptr;                          // 0040AC75..7D
    return 0;                                                         // 0040AC84
}

// ---- 0040B300 CSWGuiManager::UpdateMouseOverControl (thiscall, RET) ----
void Manager::UpdateMouseOverControl() {
    Panel* panel;                                                     // [esp+0Ch] out (written by callee on every path)
    Control* hit;                                                     // [esp+08h] out
    HitCheckMouse(mouse_x, mouse_y, &panel, &hit, 0);                 // 0040B305..1C
    (void)panel;
    Control* old = mouse_over;                                        // 0040B321
    if (hit != old) {                                                 // 0040B328
        if (old) (old->*(old->vtable->handle_mouse_over))(0);         // 0040B32C..34
        mouse_over = hit;                                             // 0040B337
    }
    Control* previous = last_hit;                                     // 0040B33A
    Control* current = mouse_over;                                    // 0040B33D reload
    last_hit = hit;                                                   // 0040B342
    if (current != previous) flags &= ~1u;                            // 0040B340..47
}

// ---- 0061F7E0 CClientExoAppInternal::SetMouseX (thiscall, RET4) ----
int ClientInternal::SetMouseX(int px) {
    if (mouse_x == px) return 0;                                      // 0061F7E4..EE
    int v = px < 0 ? 0 : px;                                          // 0061F7F3..FF (SETL/DEC/AND)
    const int limit = static_cast<int>(kotor_renderer_gl_cells::width_0078d1d4 - 1u);   // 0061F807..0D
    if (v >= limit) v = limit;                                        // 0061F80E..12 (JL keeps v)
    mouse_x = v;                                                      // 0061F7F8/0061F801/0061F814 (final value)
    return 1;
}

// ---- 0061F830 CClientExoAppInternal::SetMouseY (thiscall, RET4) ----
int ClientInternal::SetMouseY(int py) {
    const int flipped = static_cast<int>(kotor_menu_window_cells::height_0078d1d8 - static_cast<U32>(py) - 1u);  // 0061F834..41
    if (mouse_y == flipped) return 0;                                 // 0061F83B..48
    int v = flipped < 0 ? 0 : flipped;                                // 0061F84B..59
    const int limit = static_cast<int>(kotor_menu_window_cells::height_0078d1d8 - 1u);  // 0061F861..67 (reloaded)
    if (v >= limit) v = limit;                                        // 0061F868..6C
    mouse_y = v;                                                      // final store 0061F86E
    return 1;
}

