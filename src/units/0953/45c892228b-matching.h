#pragma once
// Header-free declarations: no complete GUI class or owned production storage.
struct MenuColor { unsigned long words[3]; };
class ColorReceiver {
public:
    void __thiscall SetColor(const MenuColor*);
};
// Task6 must bind this borrowed identity to its sole color storage owner.
extern "C" const MenuColor borrowed_enter_color_007a23c0;
extern "C" void __stdcall menu_enter_0067b450(unsigned char*);
