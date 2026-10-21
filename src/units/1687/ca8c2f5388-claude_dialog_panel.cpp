// Lane W4 GUI successor (dialog-panel): CSWGuiDialog / CSWGuiDialogCinematic / CSWGuiDialogTop /
// CSWGuiDialogLetterbox virtual methods and helpers formerly trapped in gui_slot_traps (bark_trap_removal successor),
// plus the small missing leaves they call (CGuiInGame::SetDialogSelection / DialogGuiDoneScroll,
// CServerExoApp(Internal)::SkipPastDialog). Re-derived from the original swkotor.exe disassembly (read-only); every
// statement cites its original PCs. x87: intermediates carried as double (PC53), dword stores as (float).
// Panel layout (ctor 006A85B0): +68 highlighted reply, +6C reply count, +70 20 reply labels (0x140 each, text params at
// +E8), +1970 reply ids, +19C0 owner id, +19C4 replies listbox, +1CA4 message label (+1D74 its text, +1D8C its params),
// +1DE4 inactive colour, +1DF0 message CExoString, +1DF8 flags, +1DFC CSWGuiDialogTop (cinematic only).
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
                                                                  kotor_reconstruction::StartupString* out,
                                                                  const kotor_reconstruction::StartupString* rhs); // CExoString::operator+
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, std::uint32_t count); // CExoArrayList::Allocate
}
namespace w2 { unsigned __cdecl ftol2_006fae8c(double value); }                                  // __ftol2 (low dword)

extern "C" {
void __fastcall frame_GuiDispatch_006339c0(void* in_game, void*) noexcept(false);              // CGuiInGame::UpdateDialog
unsigned __fastcall frame_LogIsEmpty_005e58a0(void* string, void*);                             // CExoString::IsEmpty
void __fastcall gui_m48_text_params_set_font_rename_00415df0(void* params, void*);              // CSWGuiTextParams::SetFontRename
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, U32, U32, U32) noexcept(false); // CSWGuiListBox::AddControls(list)
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, U32, U32);       // CSWGuiListBox::SetSelectedControl
void __fastcall kotor_fastcall_0040c1e0(void* manager, void*, U32 x, U32 y);                  // CSWGuiManager::HandleMouseMove
std::int32_t __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                // CSWGuiText::GetFontHeight
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, float delta);                    // CSWGuiPanel::Draw
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, U32 id, U32 down);   // CSWGuiPanel::HandleInputEvent
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                      // CSWGuiManager::PlayGuiSound
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U32, U32);          // CSWGuiManager::AddPanel
U32 __fastcall frame_RemovePanel_0040c830(void* manager, void*, void* panel);                  // CSWGuiManager::RemovePanel
U32 __fastcall kotor_fastcall_0062abd0(void* in_game, void*);                                  // CGuiInGame::IsCameraDialog (W5 claude_dialogue_client)
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, U32 id, void** out);     // CGameObjectArray::GetGameObject
extern const unsigned char object_projectile_literal_0074666c[8];                              // [0074666C] .rdata byte 0 (lookup ok)
void __cdecl frame_LogFormat_005e5680(void* string, const char* format, ...) noexcept(false);  // CExoString::Format
void* __fastcall GuiAssign005e5140(void* self, void*, const char* text) noexcept(false);       // CExoString::operator=(const char*)
void __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text) noexcept(false); // CExoString(const char*)
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* source) noexcept(false); // CExoString::operator=(const&)

void __fastcall kotor_fastcall_006a7160(void* self, void*);                                   // CSWGuiDialog::Reset (W4 panels_dialog)
// defined below (exports)
void __fastcall kotor_fastcall_0062ada0(void* in_game, void*, U32 index, U32 count, U32 send);
void __fastcall kotor_fastcall_0062ab90(void* in_game, void*);
void __fastcall kotor_fastcall_004b1f90(void* internal, void*, U32 server_id);
void __fastcall kotor_fastcall_004ae970(void* server, void*, U32 server_id);
void __fastcall kotor_fastcall_006a6f20(void* self, void*, void* control);
void __fastcall kotor_fastcall_006a7010(void* self, void*, void* text);
void __fastcall kotor_fastcall_006a70a0(void* self, void*);
U32 __fastcall kotor_fastcall_006a7110(void* self, void*, I32 index);
void __fastcall kotor_fastcall_006a7230(void* self, void*, U32 id, U32 down);
void __fastcall kotor_fastcall_006a7390(void* self, void*, void* control);
void __fastcall kotor_fastcall_006a73b0(void* self, void*, I32 x, I32 y, I32 w, I32 h);
void __fastcall kotor_fastcall_006a7440(void* self, void*, const void* extent);
void __fastcall kotor_fastcall_006a7680(void* self, void*, float delta);
void __fastcall kotor_fastcall_006a7ae0(void* self, void*, void* text);
void __fastcall kotor_fastcall_006a7c90(void* self, void*, U32 id);
void __fastcall kotor_fastcall_006a7d30(void* self, void*);
U32 __fastcall kotor_fastcall_006a7e20(void* self, void*, I32 index);
void __fastcall kotor_fastcall_006a7ef0(void* self, void*);
void __fastcall kotor_fastcall_006a86a0(void* self, void*, I32 count, void* texts, const U32* ids, U32 unused, U32 show);
void __fastcall kotor_fastcall_006a8de0(void* self, void*, I32 count, void* texts, const U32* ids, U32 unused, U32 show);
}

namespace {
void* in_game() { return pt(frame_R4_005ed690(client_app(), nullptr)); }          // [[007A39FC]+4]->GetInGameGui
void* gui_manager_of(void* panel) { return lp(panel, 0x18); }
I32 gm_w(void* gm) { return static_cast<std::int16_t>(lh(gm, 0x6C)); }              // movsx word [gm+6C]
I32 gm_h(void* gm) { return static_cast<std::int16_t>(lh(gm, 0x6E)); }              // movsx word [gm+6E]
// fild dword; fmul dword [00755788] (3EDB6DB9 = 0.428571433f); call __ftol2 (low dword).
I32 three_sevenths(I32 v) { return static_cast<I32>(w2::ftol2_006fae8c(static_cast<double>(v) * static_cast<double>(bits_f(0x3EDB6DB9u)))); }
const double k_zero_0073d700 = 0.0;                    // [0073D700] 00000000
const double k_one_0073d764 = 1.0;                     // [0073D764] 3F800000
const double k_half_0073e9ac = 0.5;                    // [0073E9AC] 3F000000
const double k_five_00743264 = 5.0;                    // [00743264] 40A00000
struct Ext { I32 x, y, w, h; };
static_assert(sizeof(Ext) == 16, "CSWGuiExtent");
void copy_ext(Ext& e, const void* src) { std::memcpy(&e, src, 16); }
void string_dtor(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
}

extern "C" {
// ============================================================================================ CGuiInGame / server leaves
// 0062ADA0 CGuiInGame::SetDialogSelection(int index, int count, int send) (thiscall, RET 0Ch).
void __fastcall kotor_fastcall_0062ada0(void* in_game_gui, void*, U32 index, U32 count, U32 send) {
    sw(in_game_gui, 0xC8, index);                                                            // 0062ADA0..0062ADA8
    sw(in_game_gui, 0xCC, count);                                                            // 0062ADA4, 0062ADB2
    sw(in_game_gui, 0xD4, send);                                                             // 0062ADAE..0062ADB8
    sw(in_game_gui, 0xD0, 1);                                                                // 0062ADBE
}
// 0062AB90 CGuiInGame::DialogGuiDoneScroll() (thiscall, RET 0).
void __fastcall kotor_fastcall_0062ab90(void* in_game_gui, void*) {
    if (lw(in_game_gui, 0x64) == 0) return;                                                  // 0062AB90..0062AB95, 0062ABAC
    sw(in_game_gui, 0xBB4, 1);                                                               // 0062AB97
    vcall<void>(lp(in_game_gui, 0x3C), 0x80);                                                // 0062ABA1..0062ABA6 jmp [vtbl+80h] ShowDialogMessage
}
// 004B1F90 CServerExoAppInternal::SkipPastDialog(ulong object_id) (thiscall, RET 4).
void __fastcall kotor_fastcall_004b1f90(void* internal, void*, U32 server_id) {
    void* object = nullptr;                                                                  // [esp] = 0 (004B1FA1)
    const unsigned char status = kotor_fastcall_004d8230(lp(internal, 0x1005C), nullptr, server_id, &object); // 004B1F91..004B1FA9
    if (status != object_projectile_literal_0074666c[0]) return;                             // 004B1FAE..004B1FB4
    if (object == nullptr) return;                                                           // 004B1FB7..004B1FBD
    if (vcall<void*>(object, 0x10) == nullptr) return;                                       // 004B1FBF..004B1FC8
    sw(vcall<void*>(object, 0x10), 0x1F4, 1);                                                // 004B1FCA..004B1FD1 (re-called)
}
// 004AE970 CServerExoApp::SkipPastDialog(ulong) (thiscall, RET 4): mov ecx,[ecx+4]; jmp 004B1F90.
void __fastcall kotor_fastcall_004ae970(void* server, void*, U32 server_id) {
    kotor_fastcall_004b1f90(lp(server, 4), nullptr, server_id);                              // 004AE970..004AE973
}

// ============================================================================================ CSWGuiDialog (007559E0)
// 006A6F20 CSWGuiDialog::OnSelectReply(CSWGuiControl*) (event PMF, RET 4 by the tail callee): jmp [vtbl+84h].
void __fastcall kotor_fastcall_006a6f20(void* self, void*, void* control) {
    vcall<void, void*>(self, 0x84, control);                                                 // 006A6F20..006A6F22
}
// 006A7010 CSWGuiDialog::SetDialogMessage(CExoString const&) (slot 7Ch, thiscall, RET 4).
void __fastcall kotor_fastcall_006a7010(void* self, void*, void* text) {
    sw(self, 0x1A08, lw(self, 0x1A08) & 0xFFFFFFFDu);                                        // 006A7013..006A7020 listbox flags
    sw(self, 0x1CE8, lw(self, 0x1CE8) & 0xFFFFFFFDu);                                        // 006A7026..006A7039 label flags
    async_assign_string_005e5c50(at(self, 0x1DF0), nullptr, text);                           // 006A702E..006A703F
    if ((lb(self, 0x1DF8) & 4) != 0 && kotor_fastcall_0062abd0(in_game(), nullptr) != 0) {  // 006A7044..006A7064
        const U32 f = (lw(self, 0x1DF8) & 0xFFFFFFFBu) | 2u;                                 // 006A7066..006A706F
        sw(self, 0x1DF8, f);                                                                 // 006A7074
        sw(self, 0x1DF8, f | 1u);                                                            // 006A707A..006A707D
        return;                                                                              // 006A7083..006A7084
    }
    vcall<void>(self, 0x80);                                                                 // 006A7087..006A708B ShowDialogMessage
    sw(self, 0x1DF8, lw(self, 0x1DF8) | 1u);                                                 // 006A7091
}
// 006A70A0 CSWGuiDialog::ShowDialogMessage() (slot 80h, thiscall, RET 0).
void __fastcall kotor_fastcall_006a70a0(void* self, void*) {
    sw(self, 0x1DF8, lw(self, 0x1DF8) & 0xFFFFFFFDu);                                        // 006A70A3
    if ((lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x14) & 4) != 0                  // 006A70AA..006A70BB GetClientOptions
        || lw(in_game(), 0xBB0) != 0) {                                                      // 006A70BD..006A70D3
        gui_load_text_params_set_text_00415e00(at(self, 0x1D8C), nullptr, at(self, 0x1DF0)); // 006A70D5..006A70E2
        sw(in_game(), 0xBB0, 0);                                                             // 006A70E7..006A70F4
    }
    sw(self, 0x1CE8, lw(self, 0x1CE8) | 2u);                                                 // 006A70FE
}
// 006A7110 CSWGuiDialog::SelectReply(int index) (slot 6Ch, thiscall, RET 4): 1 when index < count (signed), else 0.
U32 __fastcall kotor_fastcall_006a7110(void* self, void*, I32 index) {
    const I32 count = li(self, 0x6C);                                                        // 006A7110
    if (index >= count) return 0;                                                            // 006A7117..006A7119, 006A7151
    kotor_fastcall_0062ada0(in_game(), nullptr, static_cast<U32>(index), static_cast<U32>(count), 1); // 006A711B..006A712F
    frame_GuiDispatch_006339c0(in_game(), nullptr);                                          // 006A7134..006A7144 UpdateDialog
    return 1;                                                                                // 006A7149..006A714E
}
// 006A7230 CSWGuiDialog::HandleInputEvent(int id, int down) (slot 3Ch, thiscall, RET 8).
// Jump table 006A7374 (index bytes 006A7384 for id 27h..32h): 27h,2Dh -> 006A7266; 31h -> 006A72B9; 32h -> 006A72DB;
// the rest -> 006A730D.
void __fastcall kotor_fastcall_006a7230(void* self, void*, U32 id_word, U32 down) {
    if (down == 0) return;                                                                   // 006A7231..006A723A
    const I32 id = static_cast<I32>(id_word);                                                // ebx
    auto skip = [&]() {                                                                      // ClientToServerObjectId + SkipPastDialog
        const U32 server_id = kotor_fastcall_004aea30(server_app(), nullptr, lw(self, 0x19C0));
        kotor_fastcall_004ae970(server_app(), nullptr, server_id);
    };
    auto decrement = [&]() { const I32 v = li(self, 0x68); if (v > 0) sw(self, 0x68, static_cast<U32>(v - 1)); };   // 006A72B9..006A72C4
    auto increment = [&]() {                                                                 // 006A72DB..006A72E7
        const I32 v = li(self, 0x68);
        if (v < li(self, 0x6C) - 1) sw(self, 0x68, static_cast<U32>(v + 1));
    };
    if (id > 0x3D) {                                                                         // 006A7245..006A7248 (signed jg)
        if (id > 0x106) {                                                                    // 006A72C6..006A72CC
            if (id == 0x1F9) {                                                               // 006A7338..006A733E
                const U32 f = lw(self, 0x1DF8);                                              // 006A7340
                if ((f & 1u) != 0 && (f & 2u) == 0) skip();                                  // 006A7346..006A736D
                goto base_check;                                                             // 006A7372 -> 006A7317
            }
        } else if (id >= 0xFE) {                                                             // 006A72CE..006A72D4
            const I32 index = id - 0xFE;                                                     // 006A72F0
            if (index < li(self, 0x6C)) {                                                    // 006A72EC..006A72F8
                kotor_fastcall_0040a140(gui_manager_of(self), nullptr, 0);                   // 006A72FA..006A72FF
                vcall<U32, I32>(self, 0x6C, index);                                          // 006A7304..006A7309 SelectReply
            }
        } else if (id == 0x3E) {                                                             // 006A72D6..006A72D9
            increment();
        }
    } else if (id == 0x3D) {                                                                 // 006A724A
        decrement();
    } else if (static_cast<U32>(id - 0x27) <= 0xBu) {                                        // 006A724C..006A7252 (unsigned ja)
        switch (id) {                                                                        // 006A7258..006A725F
        case 0x27: case 0x2D: {                                                              // 006A7266
            const U32 f = lw(self, 0x1DF8);
            if ((f & 1u) != 0) {                                                             // 006A726C..006A726E
                if ((f & 2u) == 0) skip();                                                   // 006A7270..006A7297
            } else {
                kotor_fastcall_0040a140(gui_manager_of(self), nullptr, 0);                   // 006A729E..006A72A3
                vcall<U32, I32>(self, 0x6C, static_cast<std::int16_t>(lh(self, 0x1C8A)));   // 006A72A8..006A72B4 SelectReply(sel)
            }
            break;
        }
        case 0x31: decrement(); break;
        case 0x32: increment(); break;
        default: break;
        }
    }
    if (id == 0x27 || id == 0x2D) return;                                                    // 006A730D..006A7315
base_check:
    if ((lb(self, 0x1DF8) & 1) == 0 || id == 0 || id == 1)                                   // 006A7317..006A7327
        gui_load_panel_handle_input_00409e60(self, nullptr, static_cast<U32>(id), down);    // 006A7329..006A732D
}
// 006A7390 CSWGuiDialog::HandleSelectReply(CSWGuiControl*) (slot 84h, thiscall, RET 4).
void __fastcall kotor_fastcall_006a7390(void* self, void*, void*) {
    kotor_fastcall_0040a140(gui_manager_of(self), nullptr, 0);                               // 006A7393..006A7398 PlayGuiSound(0)
    vcall<U32, I32>(self, 0x6C, static_cast<std::int16_t>(lh(self, 0x1C8A)));                 // 006A739D..006A73A9 SelectReply
}
// 006A7C90 CSWGuiDialog::SetOwner(ulong id) (slot 78h, thiscall, RET 4).
void __fastcall kotor_fastcall_006a7c90(void* self, void*, U32 id) {
    sw(self, 0x19C0, id);                                                                    // 006A7C90..006A7C94
}
// 006A86A0 CSWGuiDialog::SetReplies(int count, CExoString* texts, ulong* ids, int unused, int show)
// (slot 74h, thiscall, RET 14h). EH states (FuncInfo 00781C6C): 0 local CExoArrayList (0056B6F0), 1 prefix, 2 joined.
void __fastcall kotor_fastcall_006a86a0(void* self, void*, I32 count, void* texts, const U32* ids, U32, U32 show) {
    vcall<void>(self, 0x70);                                                                 // 006A86C1..006A86C5 Reset
    U32 list[3] = {0, 0, 0};                                                                 // [esp+2Ch] data,size,alloc (006A86D2..006A86E2)
    Ext ext{0, 0, 0, 0};                                                                     // [esp+38h] (006A86CA..006A86D6)
    auto list_guard = on_throw([&]() { live_free(pt(list[0])); list[0] = 0; });             // state 0 -> 0056B6F0
    U32 prefix[2];                                                                           // [esp+1Ch]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(prefix), nullptr); // 006A86E6..006A86EE
    auto prefix_guard = on_throw([&]() { string_dtor(prefix); });                           // state 1
    if (count >= 0x14) count = 0x14;                                                         // 006A86F3..006A8700 (signed jl)
    sw(self, 0x6C, static_cast<U32>(count));                                                 // 006A8707
    if (count > 0) {                                                                         // 006A8705, 006A870E (signed jle)
        unsigned char* text = static_cast<unsigned char*>(texts);                            // [esp+10h]
        U32* id_slot = static_cast<U32*>(at(self, 0x1970));                                  // [esp+18h]
        unsigned char* params = static_cast<unsigned char*>(at(self, 0x158));                // esi
        I32 i = 0;                                                                           // ebx / [esp+14h]
        do {
            if (frame_LogIsEmpty_005e58a0(text, nullptr) == 0) {                             // 006A8730..006A873B
                if (i < 9) frame_LogFormat_005e5680(prefix, "%d. ", i + 1);                  // 006A8741..006A8757 (00755A6C)
                else GuiAssign005e5140(prefix, nullptr, "-. ");                              // 006A875C..006A8765 (00755A68)
                U32 joined[2];                                                               // [esp+24h]
                void* sum = GuiConcat005e5d10(reinterpret_cast<kotor_reconstruction::StartupString*>(prefix), nullptr,
                                              reinterpret_cast<kotor_reconstruction::StartupString*>(joined),
                                              reinterpret_cast<const kotor_reconstruction::StartupString*>(text)); // 006A876A..006A8778
                {
                    auto joined_guard = on_throw([&]() { string_dtor(joined); });            // state 2
                    gui_load_text_params_set_text_00415e00(params, nullptr, sum);            // 006A877D..006A8785
                    joined_guard.armed = false;
                }
                string_dtor(joined);                                                         // 006A878A..006A8793
                gui_m48_text_params_set_font_rename_00415df0(params, nullptr);               // 006A8798..006A879A
                copy_ext(ext, params - 0xE4);                                                // 006A879F..006A87C2 label+4
                ext.h = static_cast<I32>(vcall<U32>(params - 0x18, 0x08));                  // 006A87BC..006A87CF label text +8
                void* label = params - 0xE8;                                                 // ebx (006A87D3)
                vcall<void, const void*>(label, 0x04, &ext);                                 // 006A87C9..006A87E0 SetExtent
                gui_load_control_add_event_0041ab20(label, nullptr, 0x27, self, fn(&kotor_fastcall_006a6f20)); // 006A87E3..006A87ED
                if (list[1] == list[2])                                                      // 006A87F2..006A87FA
                    menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                                        list[2] != 0 ? list[2] + list[2] : 0x10u); // 006A87FC..006A880E
                static_cast<U32*>(pt(list[0]))[list[1]] = ad(label);                         // 006A8813..006A881B
                list[1] = list[1] + 1;                                                       // 006A881E..006A8827
            }
            *id_slot = ids[i];                                                               // 006A882B..006A8835
            ++id_slot; ++i; text += 8; params += 0x140;                                      // 006A883B..006A8855
        } while (i < li(self, 0x6C));                                                        // 006A8843..006A8859
    }
    if (show != 0) {                                                                         // 006A8861..006A8864
        void* listbox = at(self, 0x19C4);                                                    // esi
        kotor_fastcall_0041c1d0(listbox, nullptr, list, 0, 0, 1);                            // 006A8866..006A8879 AddControls
        sw(listbox, 0x44, lw(listbox, 0x44) | 2u);                                           // 006A887E..006A8886
        vcall<void, void*, U32>(self, 0x08, listbox, 0);                                     // 006A8884..006A888E
        sw(self, 0x68, 0);                                                                   // 006A8897
        gui_load_listbox_set_selected_0041c040(listbox, nullptr, 0, 0);                      // 006A8891..006A889E
        const I32 selected = static_cast<std::int16_t>(lh(self, 0x1C8E));                    // 006A88A3, 006A88B6
        const I32 last = li(self, 0x1C64) - 1;                                               // 006A88AA, 006A88BB
        const U32 old = lw(listbox, 0x2BC);                                                  // 006A88B0
        const U32 more = selected < last ? 1u : 0u;                                          // 006A88B9..006A88BE (setl)
        sw(listbox, 0x2BC, old ^ (((more << 5) ^ old) & 0x20u));                             // 006A88C1..006A88CD
    }
    sw(self, 0x1DF8, lw(self, 0x1DF8) & 0xFFFFFFFEu);                                        // 006A88D3..006A88DF
    {
        void* gm = gui_manager_of(self);                                                     // 006A88D9
        kotor_fastcall_0040c1e0(gm, nullptr, lw(gm, 0), lw(gm, 4));                          // 006A88E5..006A88EC HandleMouseMove
    }
    prefix_guard.armed = false;
    string_dtor(prefix);                                                                     // 006A88F1..006A88FA
    list_guard.armed = false;
    live_free(pt(list[0]));                                                                  // 006A88FF..006A890C _free
}

// ============================================================================================ CSWGuiDialogLetterbox (00755A78)
// 006A73B0 CSWGuiDialogLetterbox::SetExtentSpecial(CSWGuiExtent) (extent by value, thiscall, RET 10h).
void __fastcall kotor_fastcall_006a73b0(void* self, void*, I32 x, I32 y, I32 w, I32 h) {
    const I32 kind = li(self, 0x84);                                                         // 006A73B1 esi
    I32 top = y, height = h;                                                                 // edx, eax
    if (kind == 1) top = 0;                                                                  // 006A73B9..006A73BB, 006A73CF
    else if (kind == 2) height = gm_h(lp(self, 0x18)) - y;                                   // 006A73BD..006A73CB
    if (lw(self, 0x88) != 0) {                                                               // 006A73D5..006A73DD
        sw(self, 0x64, static_cast<U32>(x));                                                 // 006A73ED
        sw(self, 0x68, static_cast<U32>(top));                                               // 006A73EF
        sw(self, 0x04, static_cast<U32>(x));                                                 // 006A73F5
        sw(self, 0x6C, static_cast<U32>(w));                                                 // 006A73F7
        sw(self, 0x08, static_cast<U32>(top));                                               // 006A73FA
        sw(self, 0x70, static_cast<U32>(height));                                            // 006A73FD
        sw(self, 0x0C, static_cast<U32>(w));                                                 // 006A7401
        sw(self, 0x10, static_cast<U32>(height));                                            // 006A7405
        return;                                                                              // 006A7409
    }
    if (kind == 1) {                                                                         // 006A740C..006A740D
        if (height > li(self, 0x70)) sw(self, 0x70, static_cast<U32>(height));               // 006A742F..006A7434 (signed jle)
    } else if (kind == 2) {                                                                  // 006A740F..006A7410
        const I32 current = li(self, 0x68);                                                  // 006A7412
        if (top < current) {                                                                 // 006A7415..006A7417 (signed jge)
            const I32 grow = current - top;                                                  // 006A7419..006A741B
            sw(self, 0x68, static_cast<U32>(current - grow));                                // 006A741D..006A741F
            sw(self, 0x70, static_cast<U32>(li(self, 0x70) + grow));                         // 006A7422..006A7428
        }
    }
}
// 006A7440 CSWGuiDialogLetterbox::SetExtent(CSWGuiExtent const&) (slot 04h, thiscall, RET 4).
void __fastcall kotor_fastcall_006a7440(void* self, void*, const void* extent) {
    Ext e; copy_ext(e, extent);                                                              // 006A7443..006A7466
    const I32 kind = li(self, 0x84);                                                         // 006A745B
    if (kind == 1) e.y = 0;                                                                  // 006A7461..006A746B, 006A748D
    else if (kind == 2) e.h = gm_h(lp(self, 0x18)) - e.y;                                    // 006A746D..006A747E
    gui_load_panel_set_extent_0040a5a0(self, nullptr, &e);                                   // 006A7482 / 006A749A CSWGuiPanel::SetExtent
}
// 006A7680 CSWGuiDialogLetterbox::Draw(float delta) (slot 34h, thiscall, RET 4).
void __fastcall kotor_fastcall_006a7680(void* self, void*, float delta) {
    if (li(self, 0x84) == 3 && !jcc_jne(lf(self, 0x9C), k_zero_0073d700, 0x41)) {          // 006A7686..006A76A4 (timer > 0)
        if (!jcc_jp(delta, k_five_00743264, 5))                                              // 006A76AA..006A76B9 (delta < 5)
            sf(self, 0x9C, static_cast<float>(static_cast<double>(delta) + lf(self, 0x9C))); // 006A76BB..006A76C5
        if (!jcc_jne(lf(self, 0x9C), lf(self, 0x94), 0x41)) {                                // 006A76CB..006A76DC (timer > delay)
            double t = (static_cast<double>(lf(self, 0x9C)) - lf(self, 0x94)) / lf(self, 0x98); // 006A76DE..006A76F2
            float alpha;                                                                     // [esp+4]
            if (lw(self, 0xA0) != 0) {                                                       // 006A76E4..006A76F8
                t = k_one_0073d764 - t;                                                      // 006A76FA fsubr
                alpha = jcc_jne(k_zero_0073d700, t, 0x41) ? static_cast<float>(t) : 0.0f;   // 006A7700..006A7719, 006A7736
            } else {
                alpha = jcc_jp(k_one_0073d764, t, 5) ? static_cast<float>(t) : 1.0f;        // 006A771B..006A7734, 006A7736
            }
            gui_load_panel_set_alpha_00404b00(self, nullptr, alpha);                         // 006A773A..006A7741
        }
    }
    if (lw(self, 0x88) == 0) {                                                               // 006A7746..006A774E
        if (!jcc_jne(delta, k_half_0073e9ac, 0x41)) delta = 0.5f;                            // 006A7754..006A7765 (delta > 0.5)
        const I32 kind = li(self, 0x84);                                                     // 006A776D
        if (kind == 1) {                                                                     // 006A7773..006A777B
            const I32 h = li(self, 0x10);                                                    // 006A7781 edi
            Ext e; copy_ext(e, at(self, 0x64));                                              // 006A7784..006A77A3
            const I32 target = li(self, 0x70);                                               // 006A779E
            if (h < target) {                                                                // 006A77A1..006A77A7 (signed jge)
                const I32 span = target - li(self, 0x80);                                    // 006A77A9..006A77AF
                const double grow = (static_cast<double>(span) + static_cast<double>(span)) * static_cast<double>(delta); // fild; fadd st,st; fmul
                e.h = static_cast<I32>(w2::ftol2_006fae8c(grow)) + h;                        // 006A77BD..006A77C8
            } else {
                sw(self, 0x88, 1);                                                           // 006A77EA
            }
            vcall<void, const void*>(self, 0x04, &e);                                        // 006A77CC..006A77D1 / 006A77F0..006A77F7
            kotor_panel_draw_0040B760(self, nullptr, delta);                                 // 006A77D4..006A77DC / 006A77FA..006A7802
            return;
        }
        if (kind == 2) {                                                                     // 006A780E..006A7811
            Ext e; copy_ext(e, at(self, 0x64));                                              // 006A7817..006A783C
            const I32 y = li(self, 0x08);                                                    // 006A7830 ebx
            const I32 target = li(self, 0x68);                                               // 006A7837
            if (y > target) {                                                                // 006A783A..006A7840 (signed jle)
                const I32 span = target - li(self, 0x78);                                    // 006A7842..006A7845
                const double grow = (static_cast<double>(span) + static_cast<double>(span)) * static_cast<double>(delta);
                const I32 d = static_cast<I32>(w2::ftol2_006fae8c(grow));                    // 006A7849..006A7853
                e.y = y + d;                                                                 // 006A785B..006A785F
                e.h = li(self, 0x10) - d;                                                    // 006A7858..006A7863
            } else {
                sw(self, 0x88, 1);                                                           // 006A7869
                kotor_fastcall_0062ab90(in_game(), nullptr);                                 // 006A786F..006A787F DialogGuiDoneScroll
                copy_ext(e, at(self, 0x64));                                                 // 006A7884..006A789B
            }
            vcall<void, const void*>(self, 0x04, &e);                                        // 006A789F..006A78A8
        }
    }
    kotor_panel_draw_0040B760(self, nullptr, delta);                                         // 006A78AD..006A78B4
}

// ============================================================================================ CSWGuiDialogTop (00755790)
// 006A7AE0 CSWGuiDialogTop::SetReply(CExoString const&) (thiscall, RET 4).
void __fastcall kotor_fastcall_006a7ae0(void* self, void*, void* text) {
    if (lw(lp(in_game(), 0x60), 0x88) == 0) return;                                          // 006A7AE0..006A7AFF (top letterbox done)
    I32 margin_x = 0, margin_y = 0;                                                          // [esp+0Ch], [esp+8]
    kotor_fastcall_00409ee0(gui_manager_of(self), nullptr, &margin_x, &margin_y);            // 006A7B05..006A7B12 GetSafeMargin
    gui_load_text_params_set_text_00415e00(at(self, 0x14C), nullptr, text);                  // 006A7B17..006A7B22
    I32 height = static_cast<I32>(vcall<U32>(at(self, 0x134), 0x08));                       // 006A7B27..006A7B33
    Ext e; copy_ext(e, at(self, 0x68));                                                      // 006A7B36..006A7B53 label+4
    if (height <= li(self, 0x10)) {                                                          // 006A7B50..006A7B57 (signed jg)
        void* gm = gui_manager_of(self);                                                     // 006A7B59 edi
        const I32 t = three_sevenths(gm_w(gm));                                              // 006A7B5C..006A7B6E
        height = (gm_h(gm) - t) / 2 - margin_y - 5;                                          // 006A7B73..006A7B86 (cdq; sub; sar)
    }
    sw(self, 0x10, static_cast<U32>(height));                                                // 006A7B8C
    e.h = height;                                                                            // 006A7B8F
    vcall<void, const void*>(at(self, 0x64), 0x04, &e);                                      // 006A7B89..006A7B9B label SetExtent
    Ext top; copy_ext(top, at(self, 4));                                                     // 006A7B9E..006A7BBB
    top.x = 0;                                                                               // 006A7BBF
    top.y = 0;                                                                               // 006A7BC7
    top.w = gm_w(gui_manager_of(self));                                                      // 006A7BB4, 006A7BCF..006A7BD3
    top.h = li(self, 0x10) + margin_y;                                                       // 006A7BB1, 006A7BD7..006A7BDB
    kotor_fastcall_006a73b0(lp(in_game(), 0x60), nullptr, top.x, top.y, top.w, top.h);       // 006A7BDF..006A7C0F SetExtentSpecial
}

// ============================================================================================ CSWGuiDialogCinematic (00755800)
// 006A7D30 CSWGuiDialogCinematic::ShowDialogMessage() (slot 80h, thiscall, RET 0).
void __fastcall kotor_fastcall_006a7d30(void* self, void*) {
    kotor_fastcall_006a70a0(self, nullptr);                                                  // 006A7D36 CSWGuiDialog::ShowDialogMessage
    Ext e; copy_ext(e, at(self, 0x1CA8));                                                    // 006A7D3B..006A7D64
    const I32 grow = static_cast<I32>(vcall<U32>(at(self, 0x1D74), 0x08)) - e.h;             // 006A7D58..006A7D6B
    if (grow > 0) {                                                                          // 006A7D6F..006A7D71
        sw(self, 0x08, static_cast<U32>(li(self, 0x08) - grow));                             // 006A7D73..006A7D7B
        sw(self, 0x10, static_cast<U32>(li(self, 0x10) + grow));                             // 006A7D76, 006A7D82..006A7D86
        e.h += grow;                                                                         // 006A7D7E..006A7D89
    }
    vcall<void, const void*>(at(self, 0x1CA4), 0x04, &e);                                    // 006A7D8D..006A7D9E message label SetExtent
    Ext panel; copy_ext(panel, at(self, 4));                                                 // 006A7DA1..006A7DBE
    void* gm = gui_manager_of(self);                                                         // 006A7DA9
    panel.x = 0;                                                                             // 006A7DC2
    panel.w = gm_w(gm);                                                                      // 006A7DCA..006A7DCE
    panel.h = gm_h(gm) - panel.y;                                                            // 006A7DD2..006A7DDE
    kotor_fastcall_006a73b0(lp(in_game(), 0x64), nullptr, panel.x, panel.y, panel.w, panel.h); // 006A7DE2..006A7E0D bottom SetExtentSpecial
}
// 006A7E20 CSWGuiDialogCinematic::SelectReply(int index) (slot 6Ch, thiscall, RET 4; returns 1).
// EH state 0 (FuncInfo 00781A90): the empty CExoString.
U32 __fastcall kotor_fastcall_006a7e20(void* self, void*, I32 index) {
    const I32 count = li(self, 0x6C);                                                        // 006A7E3F
    if (index < count) {                                                                     // 006A7E42..006A7E44 (signed jge)
        kotor_fastcall_0062ada0(in_game(), nullptr, static_cast<U32>(index), static_cast<U32>(count), 1); // 006A7E4A..006A7E5E
        frame_GuiDispatch_006339c0(in_game(), nullptr);                                      // 006A7E63..006A7E73 UpdateDialog
        U32 empty[2];                                                                        // [esp+4]
        frame_LogStringCtor_005e5a90(empty, nullptr, "");                                    // 006A7E78..006A7E81 (0073D71C)
        {
            auto guard = on_throw([&]() { string_dtor(empty); });
            kotor_fastcall_006a7ae0(lp(self, 0x1DFC), nullptr, empty);                       // 006A7E86..006A7E99 DialogTop::SetReply
            guard.armed = false;
        }
        string_dtor(empty);                                                                  // 006A7E9E..006A7EAA
        if (frame_ContainsPanel_0040bd70(gui_manager_of(self), nullptr, lp(self, 0x1DFC)) != 0) // 006A7EAF..006A7EC0
            frame_RemovePanel_0040c830(gui_manager_of(self), nullptr, lp(self, 0x1DFC));     // 006A7EC2..006A7ECC
    }
    return 1;                                                                                // 006A7ED1..006A7EE5
}
// 006A7EF0 CSWGuiDialogCinematic::Reset() (slot 70h, thiscall, RET 0). EH state 0 (FuncInfo 00781AB4): empty string.
void __fastcall kotor_fastcall_006a7ef0(void* self, void*) {
    Ext e{0, 0, 0, 0};                                                                       // [esp+24h] (006A7F1D..006A7F29)
    I32 margin_x = 0, margin_y = 0;                                                          // [esp+10h], [esp+18h]
    kotor_fastcall_00409ee0(gui_manager_of(self), nullptr, &margin_x, &margin_y);            // 006A7F0E..006A7F2D GetSafeMargin
    e.x = margin_x;                                                                          // 006A7F32..006A7F39
    void* gm = gui_manager_of(self);                                                         // 006A7F36 ebx
    const I32 h = gm_h(gm);                                                                  // 006A7F41 edi
    const I32 t = three_sevenths(gm_w(gm));                                                  // 006A7F3D..006A7F53
    e.y = h - (h - t) / 2;                                                                   // 006A7F58..006A7F69 (cdq; sub; sar)
    e.w = gm_w(gm) - (margin_x + margin_x);                                                  // 006A7F6D..006A7F7F
    const I32 old_minus = li(self, 0x10) - 0x64;                                             // 006A7F71, 006A7F8E edi
    e.h = 0x64;                                                                              // 006A7F86
    vcall<void, const void*>(self, 0x04, &e);                                                // 006A7F77..006A7F91 SetExtent
    void* gm2 = gui_manager_of(self);                                                        // 006A7F94
    e.x = 0;                                                                                 // 006A7F97
    e.w = gm_w(gm2);                                                                         // 006A7F9B..006A7F9F
    e.h = gm_h(gm2) - e.y;                                                                   // 006A7FA3..006A7FB0
    vcall<void, const void*>(lp(in_game(), 0x64), 0x04, &e);                                 // 006A7FAB..006A7FC6 bottom letterbox SetExtent
    Ext m; copy_ext(m, at(self, 0x1CA8));                                                    // 006A7FC9..006A7FE4
    m.h -= old_minus;                                                                        // 006A7FDF..006A7FF2
    vcall<void, const void*>(at(self, 0x1CA4), 0x04, &m);                                    // 006A7FE8..006A7FF9 message label SetExtent
    U32 empty[2];                                                                            // [esp+1Ch]
    frame_LogStringCtor_005e5a90(empty, nullptr, "");                                        // 006A7FFC..006A8005 (0073D71C)
    {
        auto guard = on_throw([&]() { string_dtor(empty); });
        gui_load_text_params_set_text_00415e00(at(self, 0x1D8C), nullptr, empty);            // 006A800A..006A8019
        guard.armed = false;
    }
    string_dtor(empty);                                                                      // 006A801E..006A802A
    kotor_fastcall_006a7160(self, nullptr);                                                  // 006A802F..006A8031 CSWGuiDialog::Reset (direct)
}
// 006A8DE0 CSWGuiDialogCinematic::SetReplies(int, CExoString*, ulong*, int, int show) (slot 74h, thiscall, RET 14h).
void __fastcall kotor_fastcall_006a8de0(void* self, void*, I32 count, void* texts, const U32* ids, U32 unused, U32 show) {
    if (show != 0) kotor_fastcall_006a7ae0(lp(self, 0x1DFC), nullptr, at(self, 0x1DF0));     // 006A8DE4..006A8DFC DialogTop::SetReply
    kotor_fastcall_006a86a0(self, nullptr, count, texts, ids, unused, show);                 // 006A8E01..006A8E18 CSWGuiDialog::SetReplies
    if ((lb(self, 0x1A08) & 2) != 0) {                                                       // 006A8E1D..006A8E24
        Ext e; copy_ext(e, at(self, 0x19C8));                                                // 006A8E26..006A8E4A
        const I32 font = gui_load_text_font_height_00414ee0(at(self, 0x140), nullptr);       // 006A8E44..006A8E4E (reply 0 text)
        e.h = e.h - e.h % font;                                                              // 006A8E53..006A8E6D (cdq; idiv; sub)
        vcall<void, const void*>(at(self, 0x19C4), 0x04, &e);                                // 006A8E5E..006A8E71 listbox SetExtent
    }
    if (show != 0) {                                                                         // 006A8E75..006A8E77
        if (frame_ContainsPanel_0040bd70(gui_manager_of(self), nullptr, lp(self, 0x1DFC)) == 0) // 006A8E79..006A8E8A
            kotor_fastcall_0040bc70(gui_manager_of(self), nullptr, lp(self, 0x1DFC), 0, 1);  // 006A8E8C..006A8E99 AddPanel
    }
}
}  // extern "C"
