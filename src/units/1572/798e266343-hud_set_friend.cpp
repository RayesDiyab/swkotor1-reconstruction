// Private reconstruction of original 006859E0
// CSWGuiTargetActionMenu::SetFriend.
//
// Original ABI: thiscall(self, int friend, int poison), ECX=self,
// two stack arguments, callee RET 8. Reconstruction export:
// @kotor_fastcall_006859e0@16 (dummy EDX, two stack args, generated RET 8).
//
// The original has a local 8-byte CExoString at S+8 and a 16-byte CResRef at
// S+0x10. It installs an SEH frame whose state-0 unwind action calls 005E5C20 on
// the CExoString; there is no catch. The constructor (005B3190) and destructor
// (005E5C20) are outside the protected state; every fill/assign between them is
// inside it. The ScopedString below expresses that lifetime; normal /EHs emits
// the unwind path.
//
// Selected providers:
//   005B3190  ?ConstructString005b3190@kotor_menu20@@YIPAUString@1@PAU21@PAX@Z
//   00406D80  ?construct_00406d80@Ref@menu_resources@@QAEPAU12@PBD@Z
//   00414C00  @gui_load_border_params_set_fill_00414c00@16
//   005E5140  ?AssignText005e5140@kotor_menu20@@YIPAUString@1@PAU21@PAXPBD@Z
//   00406D60  @async_ref_construct_string_00406d60@12
//   005E5C20  @frame_LogStringDtor_005e5c20@8
//
// Source-only private proposal; not built/run. No native or runtime acceptance.
#include "da79f00dee-w4_providers.hpp"

#include <cstdint>

using w4::U32;

extern "C" {
void __fastcall gui_load_border_params_set_fill_00414c00(void*, void*, const void*, int);
void* __fastcall async_ref_construct_string_00406d60(void*, void*, const void*);
void __fastcall frame_LogStringDtor_005e5c20(void*, void*);
}

namespace kotor_menu20 {
struct String {
    void* data;
    U32 size;
};
String* __fastcall ConstructString005b3190(String*, void*);
String* __fastcall AssignText005e5140(String*, void*, const char*);
}

namespace menu_resources {
struct Ref {
    U32 a, b, c, d;  // 16-byte CResRef; scalar members avoid /GS array shape
    Ref* __thiscall construct_00406d80(const char* text);
};
static_assert(sizeof(Ref) == 16, "16-byte CResRef local");
}

namespace {
void* atv(void* p, U32 off) {
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) + off);
}

struct ScopedString {
    kotor_menu20::String value;
    ScopedString() { kotor_menu20::ConstructString005b3190(&value, nullptr); }
    ~ScopedString() { frame_LogStringDtor_005e5c20(&value, nullptr); }
};
}

extern "C" void __fastcall kotor_fastcall_006859e0(
    void* self, void*, unsigned friend_flag, unsigned poison) {
    ScopedString label;

    // 00685A05..00685A50 / 00685A57..00685A8A: four-way bar + scroll selection.
    const char* bar;
    const char* scroll;
    if (friend_flag != 0) {
        if (poison != 0) {
            bar = "POISON_BAR";       // 0x753B58
            scroll = "lbl_miscroll_f"; // 0x753B3C
        } else {
            bar = "FRIEND_BAR";       // 0x753B4C
            scroll = "lbl_miscroll_f";
        }
    } else {
        if (poison != 0) {
            bar = "POISON_BAR";
            scroll = "lbl_miscroll_h"; // 0x753B20
        } else {
            bar = "ENEMY_BAR";        // 0x753B30
            scroll = "lbl_miscroll_h";
        }
    }

    menu_resources::Ref ref;
    ref.construct_00406d80(bar);  // 00685A26/39/60/73
    gui_load_border_params_set_fill_00414c00(atv(self, 0x1A7C), nullptr, &ref, 0);

    // 00685A8F/93: assign the selected scroll label once.
    kotor_menu20::AssignText005e5140(&label.value, nullptr, scroll);

    // 00685A9E..C6: three action menus, stride 0x71C, each fill at +0xD4.
    for (int i = 0; i < 3; ++i) {
        menu_resources::Ref action_ref;
        async_ref_construct_string_00406d60(&action_ref, nullptr, &label.value);
        gui_load_border_params_set_fill_00414c00(
            atv(self, 0xD4u + static_cast<U32>(i) * 0x71Cu), nullptr, &action_ref, 0);
    }

    // ScopedString destructor: 00685AD4 (005E5C20 on the local CExoString).
}
