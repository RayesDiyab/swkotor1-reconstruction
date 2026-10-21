// Lane W4 GUI successor (dialog-panel), part 2: CSWGuiDialogComputer virtual methods (OnPanelAdded/Removed,
// GetFullScreenBG, HandleInputEvent, SetReplies, SetDialogMessage) with their missing helpers
// (CSWGuiDialogComputer::UpdateSkills, CSWCCreatureStats::GetSkillRank) and CSWGuiBarkBubble::Draw with its missing
// leaves (CGuiInGame::GetMiniMapVisible / GetInterfaceOffset, CExoStreamingSoundSource::GetPlayFailed).
// Re-derived from the original swkotor.exe disassembly (read-only); each statement cites its original PCs.
// Computer layout (ctor 006A8EB0): +48 computer type byte, +1DFC 4 x CSWGuiDialogComputerInfo, +2CFC LB_MESSAGE listbox,
// +361C CExoSoundSource*. Bark bubble (ctor 006A9770): +1A4 base extent, +1B4 remaining time, +1B8
// CExoStreamingSoundSource*, +1C0 speaker client id, +1C4 close-with-sound flag, +1C8 flags.
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction {
// CExoString::operator==(char const*) 005E5290 (0077 borrowed-string-operations; thiscall member provider).
struct StartupString { int __thiscall BorrowedEquals005e5290(const char* text); };
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
                                                                  kotor_reconstruction::StartupString* out,
                                                                  const kotor_reconstruction::StartupString* rhs); // CExoString::operator+
namespace kotor_renderer_gl_cells { extern unsigned width_0078d1d4; }                                    // [0078D1D4]
namespace kotor_menu_window_cells { extern volatile unsigned height_0078d1d8; }                          // [0078D1D8]

extern "C" {
extern void* volatile kotor_cell_rules_007a3a28;                                                // [007A3A28] CSWRules*
extern unsigned char kotor_cells_gui_suffix_007a3d50[40];                                       // [007A3D50] CExoString[5]; [4] = "comp"
void* __cdecl gui_load_manager_resolution_string_0040a3e0(void* out, I32 width, I32 height) noexcept(false); // CSWGuiManager::GetScreenResolutionString
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I32 value) noexcept(false);        // CExoString::CExoString(int)
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* source) noexcept(false); // CExoString::operator=(const&)
void* __fastcall WindowCopy005e5b00(void* self, void*, void* source) noexcept(false);          // CExoString::CExoString(const&)
U32 __fastcall kotor_fastcall_005d5930(void* source, void*);                                    // CExoSoundSource::Play
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*);                     // CExoSoundSource::Stop
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);                          // CSWGuiPanel::OnPanelAdded
void __fastcall gui_load_panel_on_removed_0040c170(void* panel, void*);                        // CSWGuiPanel::OnPanelRemoved
void __fastcall kotor_fastcall_006a7230(void* self, void*, U32 id, U32 down);                  // CSWGuiDialog::HandleInputEvent (claude_dialog_panel)
void __fastcall kotor_fastcall_006a86a0(void* self, void*, I32, void*, const U32*, U32, U32);  // CSWGuiDialog::SetReplies (claude_dialog_panel)
void __fastcall kotor_fastcall_006a7010(void* self, void*, void* text);                        // CSWGuiDialog::SetDialogMessage (claude_dialog_panel)
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                   // CSWGuiListBox::ClearItems
I32 __fastcall async_string_length_005e5790(void* string, void*);                              // CExoString::GetLength
std::int32_t __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                // CSWGuiText::GetFontHeight
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, I32 count,
                                                      U32 suppress, U32 restore, U32 variable) noexcept(false); // CSWGuiListBox::AddControls
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, U32); // CSWGuiListBox::SetActiveControl
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                     // CClientExoApp::GetSWParty
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, U32 index);                    // CSWParty::GetCharacter
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);       // CSWCCreature::GetServerCreature
void* __fastcall recovered_creature_inventory_004ef770(void* creature, void*, U32 flag);        // CSWSCreature::GetItemRepository
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U32 index);               // CItemRepository::ItemListGetItem
U32 __fastcall kotor_fastcall_005ede70(void* app, void*);                                       // CClientExoApp::GetCombatMode
void* __fastcall kotor_fastcall_005ed580(void* app, void*, U32 id);                             // CClientExoApp::GetGameObject
void __cdecl kotor_cdecl_00423bc0(W4Vector* out, const void* a, const void* b);                 // operator-(Vector const&, Vector const&)
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, float delta);                     // CSWGuiPanel::Draw
void __fastcall gui_m48_panel_set_to_destroy_00624a00(void* panel, void*, U32 flag);            // CSWGuiPanel::setToDestroy?
void __fastcall kotor_fastcall_005d5b80(void* stream, void*);                                   // CExoStreamingSoundSource::Stop
void __fastcall kotor_fastcall_005d6200(void* stream, void*);                                   // ~CExoStreamingSoundSource
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void* app, void*);                    // CClientExoApp::GetMiniGame
U32 __fastcall kotor_m15e_slot010_zero_0063e7f0(void* self, void*);                             // 0063E7F0 return_zero

// defined below (exports)
void __fastcall kotor_fastcall_006a6de0(void* self, void*);
void __fastcall kotor_fastcall_006a6e00(void* self, void*);
void* __fastcall kotor_fastcall_006a6e20(void* self, void*, void* out);
void __fastcall kotor_fastcall_006a81e0(void* self, void*, U32 id, U32 down);
void __fastcall kotor_fastcall_006a8240(void* self, void*);
void __fastcall kotor_fastcall_006a94e0(void* self, void*, I32 count, void* texts, const U32* ids, U32 unused, U32 show);
void __fastcall kotor_fastcall_006a9520(void* self, void*, void* text);
U8 __fastcall kotor_fastcall_006477b0(void* stats, void*, U32 skill_word);
U32 __fastcall kotor_fastcall_0062f910(void* in_game, void*);
U8 __fastcall kotor_fastcall_0062f8f0(void* in_game, void*);
U32 __fastcall kotor_fastcall_005d5b00(void* stream, void*);
void __fastcall kotor_fastcall_006a9ce0(void* self, void*, float delta);
}
namespace menu_tables_required { unsigned __fastcall StreamIsPlaying_005d5af0(void* stream, void*); } // CExoStreamingSoundSource::IsPlaying

namespace {
void* in_game() { return pt(frame_R4_005ed690(client_app(), nullptr)); }          // [[007A39FC]+4]->GetInGameGui
I32 gm_w(void* gm) { return static_cast<std::int16_t>(lh(gm, 0x6C)); }
void string_dtor(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
struct Ext { I32 x, y, w, h; };
static_assert(sizeof(Ext) == 16, "CSWGuiExtent");
void copy_ext(Ext& e, const void* src) { std::memcpy(&e, src, 16); }
const double k_zero_0073d700 = 0.0;                    // [0073D700] 00000000
// CExoString(int) -> CSWGuiTextParams::SetText -> ~CExoString, with the original single-state cleanup.
void set_number(void* params, void* storage, I32 value) {
    kotor_fastcall_005e5bc0(storage, nullptr, value);
    auto guard = on_throw([&]() { string_dtor(storage); });
    gui_load_text_params_set_text_00415e00(params, nullptr, storage);
    guard.armed = false;
    string_dtor(storage);
}
// CSWGuiPanel flag word +44: clear bit 8, then (unless bits 9..10 == 10b) clear bit 10 and set bit 9 (inline
// equivalent of 00624A00(0), two stores as in the original).
void close_panel_inline(void* self) {
    const U32 f = lw(self, 0x44) & 0xFFFFFEFFu;
    sw(self, 0x44, f);
    if ((f & 0x600u) != 0x400u) sw(self, 0x44, (f & 0xFFFFFBFFu) | 0x200u);
}
}

extern "C" {
// ============================================================================================ leaves
// 006477B0 CSWCCreatureStats::GetSkillRank(ushort skill) (thiscall, RET 4; AL result).
U8 __fastcall kotor_fastcall_006477b0(void* stats, void*, U32 skill_word) {
    const U16 count = lb(kotor_cell_rules_007a3a28, 0xAB);                                   // 006477B0..006477B5 movzx dx, byte [rules+ABh]
    const U16 skill = static_cast<U16>(skill_word);                                          // 006477BD
    if (skill >= count) return 0;                                                            // 006477C2..006477C9 (unsigned jb)
    return lb(lp(stats, 0x84), skill);                                                       // 006477CC..006477D5
}
// 0062F910 CGuiInGame::GetMiniMapVisible() (thiscall, RET 0).
U32 __fastcall kotor_fastcall_0062f910(void* in_game_gui, void*) {
    if ((lb(kotor_fastcall_005ed700(client_app(), nullptr), 0x14) & 8) == 0) return 0;      // 0062F910..0062F924, 0062F938
    if (kotor_m15e_client_get_mini_game_005edb00(client_app(), nullptr) != nullptr) return 0; // 0062F926..0062F936
    void* map = lp(in_game_gui, 0x90);                                                       // 0062F93C
    if (map == nullptr) return 1;                                                            // 0062F942..0062F944, 0062F953
    return (lw(map, 0x5D04) >> 1) & 1u;                                                      // 0062F946..0062F94E
}
// 0062F8F0 CGuiInGame::GetInterfaceOffset() (thiscall, RET 0; AL result).
U8 __fastcall kotor_fastcall_0062f8f0(void* in_game_gui, void*) {
    void* main_interface = lp(in_game_gui, 0x90);                                            // 0062F8F0
    if (main_interface == nullptr) return 0;                                                 // 0062F8F6..0062F8F8, 0062F901
    return lb(main_interface, 0xA458);                                                       // 0062F8FA
}
// 005D5B00 CExoStreamingSoundSource::GetPlayFailed() (thiscall, RET 0): internal ? jmp 0063E7F0 (return_zero) : 0.
U32 __fastcall kotor_fastcall_005d5b00(void* stream, void*) {
    void* internal = lp(stream, 0);                                                          // 005D5B00
    if (internal == nullptr) return 0;                                                       // 005D5B02..005D5B04, 005D5B0B
    return kotor_m15e_slot010_zero_0063e7f0(internal, nullptr);                              // 005D5B06
}

// ============================================================================================ CSWGuiDialogComputer (00755888)
// 006A6DE0 CSWGuiDialogComputer::OnPanelAdded() (slot 48h, thiscall, RET 0).
void __fastcall kotor_fastcall_006a6de0(void* self, void*) {
    kotor_fastcall_005d5930(lp(self, 0x361C), nullptr);                                      // 006A6DE3..006A6DE9 CExoSoundSource::Play
    gui_load_panel_on_added_0040b870(self, nullptr);                                         // 006A6DEE..006A6DF1 (tail jmp)
}
// 006A6E00 CSWGuiDialogComputer::OnPanelRemoved() (slot 4Ch, thiscall, RET 0).
void __fastcall kotor_fastcall_006a6e00(void* self, void*) {
    kotor_m15_sound_source_stop_005d5a20(lp(self, 0x361C), nullptr);                         // 006A6E03..006A6E09 CExoSoundSource::Stop
    gui_load_panel_on_removed_0040c170(self, nullptr);                                       // 006A6E0E..006A6E11 (tail jmp)
}
// 006A6E20 CSWGuiDialogComputer::GetFullScreenBG() -> CExoString (slot 64h, thiscall, hidden result pointer, RET 4).
// = GetScreenResolutionString(width, height) + "comp" ([007A3D70]) + CExoString(type byte +48).
// EH (FuncInfo 007819AC): 1 resolution string, 2 number, 3 res+"comp", 4 (res+"comp")+number.
void* __fastcall kotor_fastcall_006a6e20(void* self, void*, void* out) {
    U32 resolution[2];                                                                       // [esp+8]
    gui_load_manager_resolution_string_0040a3e0(resolution, static_cast<I32>(kotor_renderer_gl_cells::width_0078d1d4),
                                                static_cast<I32>(kotor_menu_window_cells::height_0078d1d8)); // 006A6E41..006A6E5A
    auto resolution_guard = on_throw([&]() { string_dtor(resolution); });                   // state 1
    U32 number[2];                                                                           // [esp+20h]
    void* number_ptr = kotor_fastcall_005e5bc0(number, nullptr, static_cast<I32>(lb(self, 0x48))); // 006A6E5D..006A6E73
    {
        auto number_guard = on_throw([&]() { string_dtor(number); });                       // state 2
        U32 with_comp[2];                                                                    // [esp+18h]
        void* first = GuiConcat005e5d10(reinterpret_cast<kotor_reconstruction::StartupString*>(resolution), nullptr,
                                        reinterpret_cast<kotor_reconstruction::StartupString*>(with_comp),
                                        reinterpret_cast<const kotor_reconstruction::StartupString*>(kotor_cells_gui_suffix_007a3d50 + 0x20)); // 006A6E75..006A6E88
        {
            auto first_guard = on_throw([&]() { string_dtor(with_comp); });                  // state 3
            U32 full[2];                                                                     // [esp+10h]
            void* second = GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(first), nullptr,
                                             reinterpret_cast<kotor_reconstruction::StartupString*>(full),
                                             static_cast<const kotor_reconstruction::StartupString*>(number_ptr)); // 006A6E8D..006A6E9A
            {
                auto full_guard = on_throw([&]() { string_dtor(full); });                    // state 4
                async_assign_string_005e5c50(resolution, nullptr, second);                   // 006A6E9F..006A6EA9
                full_guard.armed = false;
            }
            string_dtor(full);                                                               // 006A6EAE..006A6EB7
            first_guard.armed = false;
        }
        string_dtor(with_comp);                                                              // 006A6EBC..006A6EC5
        number_guard.armed = false;
    }
    string_dtor(number);                                                                     // 006A6ECA..006A6ED3
    WindowCopy005e5b00(out, nullptr, resolution);                                            // 006A6ED8..006A6EE3
    resolution_guard.armed = false;
    string_dtor(resolution);                                                                 // 006A6EE8..006A6EF9
    return out;                                                                              // 006A6EFE..006A6F0F
}
// 006A81E0 CSWGuiDialogComputer::HandleInputEvent(int id, int down) (slot 3Ch, thiscall, RET 8).
void __fastcall kotor_fastcall_006a81e0(void* self, void*, U32 id, U32 down) {
    if (down != 0) {                                                                         // 006A81E7..006A81EF
        void* message = at(self, 0x2CFC);
        if (id == 0x39) vcall<void, U32, U32>(message, 0x3C, 0x31, 1);                       // 006A81F1..006A81F6, 006A821D..006A822D
        else if (id == 0x3A) vcall<void, U32, U32>(message, 0x3C, 0x32, 1);                  // 006A81F8..006A820B
    }
    kotor_fastcall_006a7230(self, nullptr, id, down);                                        // 006A820E..006A8212 / 006A8230..006A8234
}
// 006A8240 CSWGuiDialogComputer::UpdateSkills() (thiscall, RET 0). EH (FuncInfo 00781B8C): one string per state.
void __fastcall kotor_fastcall_006a8240(void* self, void*) {
    void* character = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 006A8258..006A8275
    void* repository = recovered_creature_inventory_004ef770(
        kotor_m15a_creature_get_server_creature_0060fb20(character, nullptr), nullptr, 1);  // 006A8277..006A828B
    I32 spikes = 0, parts = 0;                                                               // [esp+14h], [esp+18h]
    for (U32 i = 0; i < lw(repository, 0x10); ++i) {                                         // 006A828D..006A829C, 006A82F3..006A8301 (unsigned)
        void* item = w2x_inventory_item_005560b0(repository, nullptr, i);                    // 006A82A0..006A82AC
        auto* tag = static_cast<kotor_reconstruction::StartupString*>(at(item, 0x28));       // 006A82AE ebp
        if (tag->BorrowedEquals005e5290("k_computer_spike") != 0)                            // 006A82B1..006A82BF (00755920)
            spikes += li(item, 0x28C);                                                       // 006A82C1..006A82CD
        else if (tag->BorrowedEquals005e5290("k_repair_part") != 0)                          // 006A82D3..006A82E1 (00755910)
            parts += li(item, 0x28C);                                                        // 006A82E3..006A82EF
    }
    U32 text[2];                                                                             // [esp+20h] (reuses the character slot)
    set_number(at(self, 0x2164), text,
               static_cast<std::int8_t>(kotor_fastcall_006477b0(lp(character, 0x2F8), nullptr, 0))); // 006A8309..006A8341 computer use
    set_number(at(self, 0x2524), text,
               static_cast<std::int8_t>(kotor_fastcall_006477b0(lp(character, 0x2F8), nullptr, 5))); // 006A8346..006A8380 repair
    set_number(at(self, 0x28E4), text, spikes);                                              // 006A8385..006A83B3
    U32 parts_text[2];                                                                       // [esp+18h] (reuses the parts slot)
    set_number(at(self, 0x2CA4), parts_text, parts);                                         // 006A83B8..006A83E6
}
// 006A94E0 CSWGuiDialogComputer::SetReplies(int, CExoString*, ulong*, int, int show) (slot 74h, thiscall, RET 14h).
void __fastcall kotor_fastcall_006a94e0(void* self, void*, I32 count, void* texts, const U32* ids, U32 unused, U32 show) {
    kotor_fastcall_006a86a0(self, nullptr, count, texts, ids, unused, show);                 // 006A94E0..006A94FF CSWGuiDialog::SetReplies
    if (show != 0) sw(self, 0x1CE8, lw(self, 0x1CE8) | 2u);                                  // 006A9504..006A9508
    kotor_fastcall_006a8240(self, nullptr);                                                  // 006A950F..006A9511 UpdateSkills
}
// 006A9520 CSWGuiDialogComputer::SetDialogMessage(CExoString const&) (slot 7Ch, thiscall, RET 4).
void __fastcall kotor_fastcall_006a9520(void* self, void*, void* text) {
    kotor_fastcall_006a8240(self, nullptr);                                                  // 006A9526..006A9528 UpdateSkills
    kotor_fastcall_006a7010(self, nullptr, text);                                            // 006A952D..006A9534 CSWGuiDialog::SetDialogMessage
    void* message = at(self, 0x2CFC);                                                        // edi
    const U32 flags = lw(self, 0x2D40);                                                      // 006A9539
    sw(message, 0x44, flags ^ ((lw(self, 0x1CE8) ^ flags) & 2u));                            // 006A953F..006A9552 (bit 1 from label)
    if ((lb(self, 0x2D40) & 2) == 0) return;                                                 // 006A9555..006A955C
    gui_load_listbox_clear_items_00419a90(message, nullptr);                                 // 006A955E..006A9560
    Ext e; copy_ext(e, at(self, 0x1CA8));                                                    // 006A9565..006A9584
    void* label_text = at(self, 0x1D74);                                                     // 006A958F
    if (async_string_length_005e5790(text, nullptr) > 0)                                     // 006A957E..006A9595 (signed jle)
        e.h = static_cast<I32>(vcall<U32>(label_text, 0x08));                                // 006A9597..006A9599
    else
        e.h = gui_load_text_font_height_00414ee0(label_text, nullptr);                       // 006A959E
    void* label = at(self, 0x1CA4);                                                          // 006A95A7 esi
    vcall<void, const void*>(label, 0x04, &e);                                               // 006A95A3..006A95B6 SetExtent
    void* controls[1] = {label};                                                             // [esp+20h] (the original reuses the text argument slot)
    gui_m48_listbox_add_controls_0041c2c0(message, nullptr, controls, 1, 0, 0, 0);           // 006A95B9..006A95CC AddControls
    gui_load_listbox_set_active_control_0041c160(message, nullptr, label, 0);                // 006A95D1..006A95D6 SetActiveControl
}

// ============================================================================================ CSWGuiBarkBubble (00755C60)
// 006A9CE0 CSWGuiBarkBubble::Draw(float delta) (slot 34h, thiscall, RET 4).
void __fastcall kotor_fastcall_006a9ce0(void* self, void*, float delta) {
    Ext e; copy_ext(e, at(self, 0x1A4));                                                     // 006A9CE6..006A9D0C
    e.h = li(self, 0x10);                                                                    // 006A9CFF, 006A9D10
    if (kotor_fastcall_0062f910(in_game(), nullptr) == 0) {                                  // 006A9D06..006A9D25 GetMiniMapVisible
        I32 margin_x = 0, margin_y = 0;                                                      // [esp+4], [esp+8]
        kotor_fastcall_00409ee0(lp(self, 0x18), nullptr, &margin_x, &margin_y);              // 006A9D27..006A9D34 GetSafeMargin
        e.w = gm_w(lp(self, 0x18)) - (margin_x + margin_x);                                  // 006A9D39..006A9D49
    }
    if (kotor_fastcall_005ede70(client_app(), nullptr) != 0)                                 // 006A9D4D..006A9D5D GetCombatMode
        e.y += static_cast<I32>(kotor_fastcall_0062f8f0(in_game(), nullptr));                // 006A9D5F..006A9D76 GetInterfaceOffset
    vcall<void, const void*>(self, 0x04, &e);                                                // 006A9D7A..006A9D85 SetExtent
    U32 visible = 1;                                                                         // 006A9D92 ebx
    if (lw(self, 0x1C0) != 0x7F000000u) {                                                    // 006A9D88..006A9D97
        void* character = frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr, 0); // 006A9D9D..006A9DAF
        if (character != nullptr) {                                                          // 006A9DB4..006A9DB6
            W4Vector player; std::memcpy(&player, at(character, 0x24), 12);                  // 006A9DBC..006A9DDA [esp+20h]
            void* speaker = kotor_fastcall_005ed580(client_app(), nullptr, lw(self, 0x1C0)); // 006A9DCB..006A9DE2 GetGameObject
            if (speaker == nullptr || vcall<void*>(speaker, 0x0C) == nullptr) {              // 006A9DE7..006A9DF6
                visible = 0;                                                                 // 006A9E46
            } else if ((lb(self, 0x1C8) & 1) == 0) {                                         // 006A9DF8..006A9DFE
                W4Vector d;                                                                  // [esp+14h]
                kotor_cdecl_00423bc0(&d, &player, at(vcall<void*>(speaker, 0x0C), 0x24));    // 006A9E00..006A9E15 player - speaker
                double sum = static_cast<double>(d.z) * d.z;                                 // 006A9E1A..006A9E1E
                sum = sum + static_cast<double>(d.y) * d.y;                                  // 006A9E25..006A9E2D
                sum = sum + static_cast<double>(d.x) * d.x;                                  // 006A9E2F..006A9E37
                if (!jcc_jnp(sum, static_cast<double>(bits_f(0x42100000u)), 5)) visible = 0; // 006A9E39..006A9E46 [0074F348] 36.0
            }
        }
    }
    auto draw_and_tick = [&]() {                                                             // 006A9EAF..006A9EDA
        kotor_panel_draw_0040B760(self, nullptr, delta);                                     // 006A9EAF CSWGuiPanel::Draw
        void* gm = lp(self, 0x18);                                                           // 006A9EB4
        if (!jcc_jp(lf(gm, 0x60), static_cast<double>(bits_f(0x3FC00000u)), 5))             // 006A9EB7..006A9EC5 [0073F414] 1.5 (gm+60 < 1.5)
            sf(self, 0x1B4, static_cast<float>(static_cast<double>(lf(self, 0x1B4)) - lf(gm, 0x60))); // 006A9ECB..006A9ED4
    };
    void* stream = lp(self, 0x1B8);                                                          // 006A9E48
    if (stream != nullptr && menu_tables_required::StreamIsPlaying_005d5af0(stream, nullptr) != 0) { // 006A9E4E..006A9E59
        if (visible != 0) draw_and_tick();                                                   // 006A9E5B..006A9E66
        else gui_m48_panel_set_to_destroy_00624a00(self, nullptr, 0);                        // 006A9E68..006A9E6A
    } else {
        stream = lp(self, 0x1B8);                                                            // 006A9E74
        bool timed;                                                                          // -> 006A9E91
        if (stream == nullptr) timed = false;                                                // 006A9E7A..006A9E7C
        else if (kotor_fastcall_005d5b00(stream, nullptr) != 0) timed = true;                // 006A9E7E..006A9E85 GetPlayFailed
        else timed = lw(self, 0x1C4) == 0;                                                   // 006A9E87..006A9E8F
        if (timed) {
            if (!jcc_jne(lf(self, 0x1B4), k_zero_0073d700, 0x41) && visible != 0)            // 006A9E91..006A9EA6 (time > 0)
                draw_and_tick();
            else
                close_panel_inline(self);                                                    // 006A9EDC..006A9EF8, 006A9F49..006A9F55
        } else {
            if (lp(self, 0x1B8) != nullptr) {                                                // 006A9EFA..006A9F02
                kotor_fastcall_005d5b80(lp(self, 0x1B8), nullptr);                           // 006A9F04 CExoStreamingSoundSource::Stop
                void* owned = lp(self, 0x1B8);                                               // 006A9F09
                if (owned != nullptr) {                                                      // 006A9F0F..006A9F11
                    kotor_fastcall_005d6200(owned, nullptr);                                 // 006A9F13..006A9F15 ~CExoStreamingSoundSource
                    live_free(owned);                                                        // 006A9F1A..006A9F20 operator delete
                }
                sw(self, 0x1B8, 0);                                                          // 006A9F23
            }
            close_panel_inline(self);                                                        // 006A9F2D..006A9F55
        }
    }
    if (kotor_fastcall_005ede70(client_app(), nullptr) != 0)                                 // 006A9F58..006A9F69 GetCombatMode
        sw(self, 0x08, lw(self, 0x08) - kotor_fastcall_0062f8f0(in_game(), nullptr));        // 006A9F6B..006A9F83
}
}  // extern "C"
