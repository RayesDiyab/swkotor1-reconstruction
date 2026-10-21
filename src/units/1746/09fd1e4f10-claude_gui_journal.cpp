// Lane W4 G15 (gui-journal): CSWGuiInGameJournal panel slots (table 00751960) whose W4 trap stubs are replaced, plus
// the callees they reach that had no rebuilt provider. Transcribed from swkotor.exe linear-sweep listings
// (claude-runs/tools/win.py <VA>); original PCs cited per statement.
//
//   00644960 CSWGuiInGameJournal::GetQuestEntryAt(int) -> SJournalEntry*      thiscall RET 4   (slot 6Ch)
//   00645670 CSWGuiInGameJournal::Draw(float)                                 thiscall RET 4   (slot 34h)
//   006456E0 CSWGuiInGameJournal::HandleInputEvent(int, int)                  thiscall RET 8   (slot 3Ch)
//   00645E00 CSWGuiInGameJournal::OnPanelAdded()                              thiscall RET 0   (slot 48h)
//   00646070 CSWGuiInGameJournal::OnPanelRemoved()  (jmp 00645610)            thiscall RET 0   (slot 4Ch)
// Callees reconstructed here (no rebuilt body existed in the c24 map):
//   00645330 CSWGuiInGameJournal::PopulateItemListBox()                       RET 0
//   00645610 CSWGuiInGameJournal::MarkNewQuestsObserved()                     RET 0
//   00645100 CSWGuiInGameJournal::OnControlEntered(CSWGuiControl*)            RET 4 (also an event handler)
//   00644790 CSWGuiJournalItemEntry::SetEncouraged(int)                       RET 4
//   00644740 CGuiInGame +BC4 bit 0 setter (journal mode: done / active)       RET 4
//   00644770 CSWCJournal: [+3Ch] = 0 (active list changed flag clear)          RET 0
//   00644780 CSWCJournal: [+1Ch] = 0 (done list changed flag clear)            RET 0
//   00676310 CSWCJournal::GetActiveEntryAt(int)                               RET 4
//   00676340 CSWCJournal::GetDoneEntryAt(int)                                 RET 4
//   00676BC0 CSWCJournal::SetActiveSortBy(int)                                RET 4
//   00676BF0 CSWCJournal::SetDoneSortBy(int)                                  RET 4
// CSWGuiJournalItemEntry (0x1CC, CSWGuiButton + [1C4] quest index + [1C8] flags) uses table 007518C0 = the
// CSWGuiButton table (0073E658, 40 slots) with slot 0 = 0067EFA0 (folded scalar deleting dtor, panels_character).
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                    // CExoString::operator+
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };              // CTlkTable::GetSimpleString
}
namespace w4 { extern volatile U32 journal_sort_00833a90; }                                   // [00833A90] (panels_menus.cpp)

extern "C" {
// ---- client / journal (claude_s2p_small_journal*.cpp owns CSWCJournal bodies + the 00833A94 cell)
void* __fastcall kotor_fastcall_005ed320(void* client, void*);                                // CClientExoApp::GetQuestJournal
extern volatile U32 kotor_cell_journal_updated_00833a94;                                      // [00833A94]
void __fastcall kotor_fastcall_00676530(void* list, void*);                                   // CSWCSortedJournalArrayList::SortList
U32 __fastcall kotor_fastcall_00675dd0(void* object, void*);                                  // 00675DD0 GetProperty0x4 (done count)
U32 __fastcall recovered_bridge_layout_room_count_005a4c50(void* object, void*);              // 005A4C50 GetProperty0x24 (active count)
U32 __fastcall packed_file_get_async_file_00644760(void* object, void*);                      // 00644760 GetProperty0x18 (done sort)
U32 __fastcall aurora_txi_size_00712fc0(void* object, void*);                                 // 00712FC0 GetProperty0x38 (active sort)
U32 __fastcall kotor_fastcall_005ed4c0(void* client, void*);                                  // CClientExoApp::GetClientLanguage
U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, U32 language, void* out, U32 gender); // CExoLocString::GetString
extern void* kotor_cell_rules_007a3a28;                                                       // [007A3A28] CSWRules*
std::int32_t __fastcall c2da_current_get_int_00413660(void* table, void*, I32 row, void* column, I32* out); // C2DA::GetINTEntry
void __fastcall recovered_bridge_dword_array_add_unique_006449d0(void* list, void*, U32 value); // CExoArrayList<ulong>::AddUnique
int __fastcall async_string_length_005e5790(void* string, void*);                             // CExoString::GetLength
I32 __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                         // CSWGuiText::GetFontHeight
// ---- panel / manager / in-game GUI
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, U32 delta_bits);               // CSWGuiPanel::Draw (float on stack)
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, U32 id, U32 down);   // CSWGuiPanel::HandleInputEvent
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);                         // CSWGuiPanel::OnPanelAdded
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                     // CSWGuiManager::PlayGuiSound
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U32, U32);         // CSWGuiManager::AddPanel
int __fastcall frame_R4_0062cba0(void* in_game, void*, unsigned keep_paused);                 // CGuiInGame::HideSWInGameGui
void __fastcall kotor_fastcall_005eda60(void* app, void*, U32 input_class, U32 show);         // CClientExoApp::SetInputClass
// ---- list boxes / controls
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                  // CSWGuiListBox::ClearItems
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, U32, U32, U32);     // CSWGuiListBox::AddControls(CExoArrayList&)
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, I32 index, I32 flag); // CSWGuiListBox::SetSelectedControl
void __fastcall gui_m48_listbox_add_controls_0041c2c0(void* listbox, void*, void** controls, I32 count, U32, U32, U32) noexcept(false); // AddControls(ptr*)
void __fastcall gui_load_listbox_set_active_control_0041c160(void* listbox, void*, void* control, U32); // CSWGuiListBox::SetActiveControl
void* __fastcall kotor_fastcall_0067efa0(void* self, void*, U32 flags);                       // 0067EFA0 scalar deleting ~CSWGuiButton-derived
// ---- colours (.data, never written)
extern const unsigned char borrowed_exit_color_007a23b4[12];                                  // [007A23B4]
extern const unsigned char borrowed_enter_color_007a23c0[12];                                 // [007A23C0]
extern const unsigned char kotor_m1_color_007a23fc[12];                                       // [007A23FC]

// ---- defined here
void* __fastcall kotor_fastcall_00644960(void* self, void*, std::uint32_t index);
void __fastcall kotor_fastcall_00645670(void* self, void*, std::uint32_t delta_bits);
void __fastcall kotor_fastcall_006456e0(void* self, void*, std::uint32_t id, std::uint32_t down);
void __fastcall kotor_fastcall_00645e00(void* self, void*);
void __fastcall kotor_fastcall_00646070(void* self, void*);
void __fastcall kotor_fastcall_00645330(void* self, void*);
void __fastcall kotor_fastcall_00645610(void* self, void*);
void __fastcall kotor_fastcall_00645100(void* self, void*, void* control);
void __fastcall kotor_fastcall_00644790(void* self, void*, U32 encouraged);
void __fastcall kotor_fastcall_00644740(void* in_game, void*, U32 value);
void __fastcall kotor_fastcall_00644770(void* journal, void*);
void __fastcall kotor_fastcall_00644780(void* journal, void*);
void* __fastcall kotor_fastcall_00676310(void* journal, void*, I32 index);
void* __fastcall kotor_fastcall_00676340(void* journal, void*, I32 index);
void __fastcall kotor_fastcall_00676bc0(void* journal, void*, U32 sort);
void __fastcall kotor_fastcall_00676bf0(void* journal, void*, U32 sort);
}

namespace {
// [007A2474] .data strref table for the four journal sort modes (image values; read-only in the original).
const U32 journal_sort_strref_007a2474[4] = {0x7DAD, 0x7DAE, 0x7DAF, 0x7DB0};

void* in_game_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }               // [[7A39FC]+4]->GetInGameGui
bool journal_done_mode() { return (lb(in_game_gui(), 0xBC4) & 1u) != 0; }                   // test byte [gui+BC4],1
void* quest_journal() { return kotor_fastcall_005ed320(client_app(), nullptr); }

// CExoString temporary with explicit lifetime (kill() = the original's in-order destructor call; the C++
// destructor only runs it while unwinding, = the EH state's cleanup).
struct Exo {
    U32 s[2]; bool live = false;
    Exo() = default;
    Exo(const Exo&) = delete;
    Exo& operator=(const Exo&) = delete;
    void def() { exo_default(s); live = true; }
    void text(const char* t) { frame_LogStringCtor_005e5a90(s, nullptr, t); live = true; }
    void born() { live = true; }
    void kill() { if (live) { live = false; frame_LogStringDtor_005e5c20(s, nullptr); } }
    ~Exo() { kill(); }
    SS* p() { return reinterpret_cast<SS*>(s); }
};
void tlk(Exo& out, U32 strref) {
    static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
        reinterpret_cast<kotor_menu_10_19::String*>(out.s), strref);
    out.born();
}
void concat(Exo& lhs, Exo& out, Exo& rhs) { GuiConcat005e5d10(lhs.p(), nullptr, out.p(), rhs.p()); out.born(); }

// tlk(head) + sep + tlk(tail): tail fetched first, then head; joined = head + sep; result = joined + tail,
// constructed straight into `out` (assign == false) or assigned to it through a temporary (assign == true).
// Temporaries destroyed: [full,] joined, head, tail, then sep (original order at every call site below).
void tlk_join(Exo& out, U32 head_ref, const char* sep_text, U32 tail_ref, bool assign) {
    Exo sep; sep.text(sep_text);
    Exo tail; tlk(tail, tail_ref);
    Exo head; tlk(head, head_ref);
    Exo joined; concat(head, joined, sep);
    if (!assign) {
        concat(joined, out, tail);
    } else {
        Exo full; concat(joined, full, tail);
        async_assign_string_005e5c50(out.s, nullptr, full.s);
        full.kill();
    }
    joined.kill(); head.kill(); tail.kill(); sep.kill();
}

// CSWGuiJournalItemEntry table 007518C0 (40 slots, base CSWGuiButton 0073E658; slot 0 differs).
const Override ovr_journal_item_entry[] = {
    {0x00, fn(&kotor_fastcall_0067efa0)},   // 0067EFA0 ~CSWGuiJournalItemEntry (scalar deleting, folded)
};
void* table_journal_item_entry[40];
bool built_journal_item_entry = false;
const void* vt_journal_item_entry() {
    if (!built_journal_item_entry) {
        vt_build(table_journal_item_entry, 40, gui_load_button_methods_0073e658, 40, ovr_journal_item_entry, 1);
        built_journal_item_entry = true;
    }
    return table_journal_item_entry;
}

// Local CExoArrayList<CSWGuiControl*> {data, size, allocated}; its EH cleanup frees the data block.
struct ControlList {
    U32 data = 0; I32 size = 0; I32 allocated = 0;
    ~ControlList() { if (data != 0) live_free(pt(data)); }
};
}  // namespace

extern "C" {
// ======================= CSWCJournal accessors =======================
// 00676310 GetActiveEntryAt(int i): 0 <= i < [+24h] ? [+20h] + [[+2Ch] + i*4] * 3Ch : 0.
void* __fastcall kotor_fastcall_00676310(void* journal, void*, I32 index) {
    if (index < 0 || index >= li(journal, 0x24)) return nullptr;                               // 00676310..0067631B (signed)
    const U32 slot = lw(lp(journal, 0x2C), static_cast<U32>(index) * 4u);                      // 0067631D..00676320
    return pt(slot * 0x3Cu + lw(journal, 0x20));                                               // 00676323..0067632B
}
// 00676340 GetDoneEntryAt(int i): 0 <= i < [+4] ? [+0] + [[+0Ch] + i*4] * 3Ch : 0.
void* __fastcall kotor_fastcall_00676340(void* journal, void*, I32 index) {
    if (index < 0 || index >= li(journal, 0x04)) return nullptr;                               // 00676340..0067634B (signed)
    const U32 slot = lw(lp(journal, 0x0C), static_cast<U32>(index) * 4u);                      // 0067634D..00676350
    return pt(slot * 0x3Cu + lw(journal, 0x00));                                               // 00676353..0067635A
}
// 00676BC0 SetActiveSortBy(int): modes 0..3 only; re-sorts the active list (+20h) when sorting is enabled.
void __fastcall kotor_fastcall_00676bc0(void* journal, void*, U32 sort) {
    if (sort != 0 && sort != 1 && sort != 2 && sort != 3) return;                              // 00676BC0..00676BD5
    sw(journal, 0x38, sort);                                                                   // 00676BD7
    if ((lb(journal, 0x40) & 1u) != 0) kotor_fastcall_00676530(at(journal, 0x20), nullptr);    // 00676BDA..00676BE3
}
// 00676BF0 SetDoneSortBy(int): modes 0..3 only; re-sorts the done list (+0) when sorting is enabled.
void __fastcall kotor_fastcall_00676bf0(void* journal, void*, U32 sort) {
    if (sort != 0 && sort != 1 && sort != 2 && sort != 3) return;                              // 00676BF0..00676C05
    sw(journal, 0x18, sort);                                                                   // 00676C07
    if ((lb(journal, 0x40) & 1u) != 0) kotor_fastcall_00676530(journal, nullptr);              // 00676C0A..00676C10
}
// 00644770 / 00644780: clear the active (+3Ch) / done (+1Ch) "list changed" flags.
void __fastcall kotor_fastcall_00644770(void* journal, void*) { sw(journal, 0x3C, 0); }        // 00644770
void __fastcall kotor_fastcall_00644780(void* journal, void*) { sw(journal, 0x1C, 0); }        // 00644780
// 00644740 CGuiInGame: [+BC4] bit 0 = value bit 0.
void __fastcall kotor_fastcall_00644740(void* in_game, void*, U32 value) {
    const U32 old = lw(in_game, 0xBC4);                                                        // 00644740
    sw(in_game, 0xBC4, old ^ ((old ^ value) & 1u));                                            // 00644746..00644755
}

// ======================= CSWGuiJournalItemEntry =======================
// 00644790 SetEncouraged(int): bit 0 of [+1C8]; encouraged -> colour 007A23FC on border (+90) and hilight (+104)
// with border alpha [+8C] = 0.5; otherwise the standard exit/enter colours. No change when the bit already matches.
void __fastcall kotor_fastcall_00644790(void* self, void*, U32 encouraged) {
    const U32 flags = lw(self, 0x1C8);                                                         // 00644790
    if ((flags & 1u) == encouraged) return;                                                    // 00644796..006447A2
    const U32 next = flags ^ ((flags ^ encouraged) & 1u);                                      // 006447A8..006447AF
    sw(self, 0x1C8, next);                                                                     // 006447B3
    if ((next & 1u) != 0) {                                                                    // 006447B1..006447B9
        std::memcpy(at(self, 0x90), kotor_m1_color_007a23fc, 8);                               // 006447BB..006447CD
        sw(self, 0x8C, 0x3F000000u);                                                           // 006447D5 (0.5f)
        std::memcpy(at(self, 0x98), kotor_m1_color_007a23fc + 8, 4);                           // 006447D0 / 006447DF
        std::memcpy(at(self, 0x104), kotor_m1_color_007a23fc, 12);                             // 006447E2..006447FE
        return;
    }
    std::memcpy(at(self, 0x90), borrowed_exit_color_007a23b4, 12);                             // 00644805..00644822
    std::memcpy(at(self, 0x104), borrowed_enter_color_007a23c0, 12);                           // 00644825..00644840
}

// ======================= CSWGuiInGameJournal =======================
// 00644960 GetQuestEntryAt(int): done list when CGuiInGame +BC4 bit 0, else active list; 0 when out of range.
void* __fastcall kotor_fastcall_00644960(void* self, void*, std::uint32_t index) {
    (void)self;
    void* journal = quest_journal();                                                           // 00644960..00644969
    const I32 i = static_cast<I32>(index);
    if (journal_done_mode()) {                                                                 // 0064496E..00644987
        const I32 count = static_cast<I32>(kotor_fastcall_00675dd0(journal, nullptr));         // 00644989
        if (i >= count) return nullptr;                                                        // 0064498E..00644994 -> 006449AF
        return kotor_fastcall_00676340(journal, nullptr, i);                                   // 00644996..0064499F
    }
    const I32 count = static_cast<I32>(recovered_bridge_layout_room_count_005a4c50(journal, nullptr)); // 006449A2
    if (i >= count) return nullptr;                                                            // 006449A7..006449AD
    return kotor_fastcall_00676310(journal, nullptr, i);                                       // 006449B5..006449BE
}

// 00645610 MarkNewQuestsObserved: clears flag 4 (new) of every viewed entry index in +FBC (CExoArrayList<ulong>
// {FBC data, FC0 size, FC4 allocated}), then frees the list.
void __fastcall kotor_fastcall_00645610(void* self, void*) {
    for (I32 i = 0; i < li(self, 0xFC0); ++i) {                                                // 00645614..00645645 (size re-read)
        const U32 index = lw(lp(self, 0xFBC), static_cast<U32>(i) * 4u);                       // 00645623..00645629
        void* entry = vcall<void*, U32>(self, 0x6C, index);                                    // 0064562C..00645631 GetQuestEntryAt
        if (entry != nullptr) sw(entry, 0x2C, lw(entry, 0x2C) & 0xFFFFFFFBu);                  // 00645634..00645638
    }
    if (void* data = lp(self, 0xFBC)) {                                                        // 00645647..0064564F
        live_free(data);                                                                       // 00645651..00645657
        sw(self, 0xFBC, 0);                                                                    // 0064565A
        sw(self, 0xFC4, 0);                                                                    // 00645660
    }
    sw(self, 0xFC0, 0);                                                                        // 00645667
}
// 00646070 OnPanelRemoved = jmp 00645610 (the base CSWGuiPanel::OnPanelRemoved is not called; original quirk).
void __fastcall kotor_fastcall_00646070(void* self, void*) { kotor_fastcall_00645610(self, nullptr); }

// 00645100 OnControlEntered(CSWGuiControl* item): description listbox (+1A4) shows the entry text (+8 locstring),
// prefixed by "tlk(planets.2da Name[planet]):\n" when the entry has a planet ([+34] != -1); the item index is
// recorded as viewed (+FBC AddUnique). The description label (+484) is then re-sized and re-added.
void __fastcall kotor_fastcall_00645100(void* self, void*, void* control) {
    if (control != nullptr) {                                                                  // 0064511E..00645128
        const U32 index = lw(control, 0x1C4);                                                  // 0064512E
        gui_load_listbox_clear_items_00419a90(at(self, 0x1A4), nullptr);                       // 00645134..0064513A
        void* entry = vcall<void*, U32>(self, 0x6C, index);                                    // 0064513F..00645144
        if (entry != nullptr) {                                                                // 00645147..0064514B
            recovered_bridge_dword_array_add_unique_006449d0(at(self, 0xFBC), nullptr, index); // 00645151..00645158
            Exo text; text.def();                                                              // 0064515D..00645161 [esp+18] state 0
            exo_locstring_get_005ea130(at(entry, 8), nullptr, kotor_fastcall_005ed4c0(client_app(), nullptr), text.s, 0); // 00645166..00645187
            if (li(entry, 0x34) != -1) {                                                       // 0064518C..00645190
                I32 name_strref;                                                               // [esp+10] (written by GetINTEntry)
                {
                    Exo column; column.text("Name");                                           // 00645196..0064519F [esp+20] state 1
                    void* planets = lp(lp(kotor_cell_rules_007a3a28, 0xB8), 0x104);            // 006451A4..006451AF
                    c2da_current_get_int_00413660(planets, nullptr, li(entry, 0x34), column.s, &name_strref); // 006451B5..006451CA
                    column.kill();                                                             // 006451CF..006451D8
                }
                Exo sep; sep.text(":\n");                                                      // 006451DD..006451E6 [esp+20] state 2
                Exo name; tlk(name, static_cast<U32>(name_strref));                            // 006451EB..00645200 [esp+38] state 3
                Exo joined; concat(name, joined, sep);                                         // 00645205..00645216 [esp+30] state 4
                Exo full; concat(joined, full, text);                                          // 0064521B..0064522D [esp+28] state 5
                async_assign_string_005e5c50(text.s, nullptr, full.s);                         // 00645232..0064523C
                full.kill();                                                                   // 00645241..00645249
                joined.kill();                                                                 // 0064524E..00645257
                name.kill();                                                                   // 0064525C..00645265
                sep.kill();                                                                    // 0064526A..00645273
            }
            gui_load_text_params_set_text_00415e00(at(self, 0x56C), nullptr, text.s);          // 00645278..00645283
            text.kill();                                                                       // 00645288..00645294
        }
    }
    I32 extent[4];                                                                             // [esp+40] = label +484 extent
    std::memcpy(extent, at(self, 0x488), 16);                                                  // 00645299..006452BC
    if (async_string_length_005e5790(at(self, 0x56C), nullptr) > 0)                            // 006452B2..006452CD
        extent[3] = vcall<I32>(at(self, 0x554), 0x08);                                         // 006452CF..006452D1
    else
        extent[3] = gui_load_text_font_height_00414ee0(at(self, 0x554), nullptr);              // 006452D6
    void* label = at(self, 0x484);                                                             // 006452DF
    vcall<void, const void*>(label, 0x04, extent);                                             // 006452DB..006452EE SetExtent
    void* controls[1] = {label};                                                               // [esp+14]
    gui_m48_listbox_add_controls_0041c2c0(at(self, 0x1A4), nullptr, controls, 1, 0, 0, 0);     // 006452F1..0064530A
    gui_load_listbox_set_active_control_0041c160(at(self, 0x1A4), nullptr, label, 0);          // 0064530F..00645314
}

// 00645330 PopulateItemListBox: one CSWGuiJournalItemEntry per quest entry (GetQuestEntryAt 0, 1, ...) built from the
// item listbox prototype (+890) with width = listbox width (+858) - 2 * border (+884 byte), collected in a local
// CExoArrayList and added to the listbox (+5C4); then selects the first item (or clears the description).
void __fastcall kotor_fastcall_00645330(void* self, void*) {
    void* journal = quest_journal();                                                           // 00645354..00645365 [esp+20]
    Exo text; text.def();                                                                      // 00645361..00645369 [esp+2C] state 0
    const U32 language = kotor_fastcall_005ed4c0(client_app(), nullptr);                       // 0064536E..00645382 [esp+1C]
    ControlList list;                                                                          // 00645386..0064538E [esp+34] state 1
    void* proto = lp(self, 0x890);                                                             // 00645392..00645398 [esp+18]
    I32 extent[4];                                                                             // [esp+40]
    std::memcpy(extent, at(proto, 4), 16);                                                     // 0064539C..006453C7
    extent[2] = li(self, 0x858) - (static_cast<I32>(lb(self, 0x884)) << 1);                    // 006453AF..006453D6
    I32 index = 0;                                                                             // [esp+14]
    void* entry = vcall<void*, U32>(self, 0x6C, 0);                                            // 006453CB..006453DE
    while (entry != nullptr) {                                                                 // 006453E3..006453E5 / 00645523..00645525
        void* item = nullptr;
        if (void* raw = live_operator_new(0x1CC)) {                                            // 006453F0..0064540A state 2
            auto guard = on_throw([&]() { live_free(raw); });
            gui_load_button_construct_0041c0f0(raw, nullptr);                                  // 0064540C..0064540E
            const U32 flags = lw(raw, 0x1C8) & 0xFFFFFFFEu;                                    // 00645413..00645419
            set_vptr(raw, vt_journal_item_entry());                                            // 0064541C (007518C0)
            sw(raw, 0x1C4, 0);                                                                 // 00645422
            sw(raw, 0x1C8, flags);                                                             // 00645428
            guard.armed = false;
            item = raw;
        }                                                                                      // 00645434 state 1
        if (item != nullptr) {                                                                 // 00645432..00645439
            sw(item, 0x1C4, static_cast<U32>(index));                                          // 0064543F..00645449
            vcall<void, const void*, void*>(item, 0x98, extent, proto);                        // 0064544F..00645457 Initialize
            gui_load_control_add_event_0041ab20(item, nullptr, 0, self, fn(&kotor_fastcall_00645100)); // 0064545D..00645466
            exo_locstring_get_005ea130(entry, nullptr, language, text.s, 0);                   // 0064546B..00645478
            gui_load_text_params_set_text_00415e00(at(item, 0x16C), nullptr, text.s);          // 0064547D..00645488
            if ((lb(entry, 0x2C) & 4u) != 0) kotor_fastcall_00644790(item, nullptr, 1);        // 0064548D..00645497
            if (list.size == list.allocated) {                                                 // 0064549C..006454A4
                const I32 capacity = list.allocated != 0 ? list.allocated * 2 : 0x10;          // 006454A6..006454B3
                const U32 old = list.data;                                                     // 006454B3
                list.allocated = capacity;                                                     // 006454BF
                list.data = ad(live_operator_new(static_cast<U32>(capacity) * 4u));            // 006454B7..006454CC
                for (I32 i = 0; i < list.size; ++i)                                            // 006454D3..006454F1
                    sw(pt(list.data), static_cast<U32>(i) * 4u, lw(pt(old), static_cast<U32>(i) * 4u));
                if (old != 0) live_free(pt(old));                                              // 006454F3..006454F8
            }
            sw(pt(list.data), static_cast<U32>(list.size) * 4u, ad(item));                     // 00645500..00645508
            ++list.size;                                                                       // 0064550B
        }
        ++index;                                                                               // 0064550F..0064551A
        entry = vcall<void*, U32>(self, 0x6C, static_cast<U32>(index));                        // 00645513..0064551E
    }
    void* items = at(self, 0x5C4);                                                             // 00645534
    kotor_fastcall_0041c1d0(items, nullptr, &list, 1, 1, 0);                                   // 0064552B..0064553D AddControls
    if (void* quest = lp(self, 0xFB8)) vcall<void, U32>(quest, 0x40, 0);                       // 00645542..0064554F
    if (list.size > 0) {                                                                       // 00645552..00645559 (signed)
        gui_load_listbox_set_selected_0041c040(items, nullptr, 0, 0);                          // 0064555B..0064555C
    } else {
        gui_load_listbox_set_selected_0041c040(items, nullptr, -1, 0);                         // 00645563..00645565
        { Exo empty; empty.text("");                                                           // 0064556A..00645573 state 3 (0073D71C)
          gui_load_text_params_set_text_00415e00(at(self, 0x56C), nullptr, empty.s);           // 00645578..00645588
          empty.kill(); }                                                                      // 0064558D..00645596
        kotor_fastcall_00645100(self, nullptr, nullptr);                                       // 0064559B..0064559E
    }
    if (journal_done_mode()) kotor_fastcall_00644780(journal, nullptr);                        // 006455A3..006455C8
    else kotor_fastcall_00644770(journal, nullptr);
    live_free(pt(list.data));                                                                  // 006455CA..006455D3 state 0
    list.data = 0;                                                                             // 006455DF
    text.kill();                                                                               // 006455DB..006455EB
}

// 00645670 Draw(float): repopulate when the shown list (done: +1C, active: +3C) changed, then CSWGuiPanel::Draw.
void __fastcall kotor_fastcall_00645670(void* self, void*, std::uint32_t delta_bits) {
    void* journal = quest_journal();                                                           // 00645670..00645681
    bool populate = false;
    if (!journal_done_mode() && lw(journal, 0x3C) != 0) populate = true;                       // 00645681..0064569F
    else if (journal_done_mode() && lw(journal, 0x1C) != 0) populate = true;                   // 006456A1..006456BD (GetInGameGui again)
    if (populate) kotor_fastcall_00645330(self, nullptr);                                      // 006456BF..006456C1
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 006456C6..006456CD
}

// 00645E00 OnPanelAdded: re-applies the stored sort mode ([00833A90], forced to 0 for this call - without writing
// the cell - when [00833A94] journal-updated is set, which is cleared), rebuilds the sort-button and title texts,
// repopulates, resets the exit button and runs CSWGuiPanel::OnPanelAdded.
void __fastcall kotor_fastcall_00645e00(void* self, void*) {
    void* journal = quest_journal();                                                           // 00645E1E..00645E2B (edi)
    U32 sort = w4::journal_sort_00833a90;                                                      // 00645E30 (ebp)
    if (kotor_cell_journal_updated_00833a94 != 0) {                                            // 00645E38..00645E3F
        sort = 0;                                                                              // 00645E41
        kotor_cell_journal_updated_00833a94 = 0;                                               // 00645E43
    }
    if (journal_done_mode()) kotor_fastcall_00676bf0(journal, nullptr, sort);                  // 00645E49..00645E6D SetDoneSortBy
    else kotor_fastcall_00676bc0(journal, nullptr, sort);                                      //                    SetActiveSortBy
    I32 next = static_cast<I32>(sort) + 1;                                                     // 00645E6F
    if (next >= 4) next = 0;                                                                   // 00645E72..00645E77
    Exo text;                                                                                  // [esp+10] state 5
    tlk_join(text, 0xA646, " ", journal_sort_strref_007a2474[next], false);                    // 00645E79..00645F1E (0073D5D8)
    gui_load_text_params_set_text_00415e00(at(self, 0xD98), nullptr, text.s);                  // 00645F23..00645F2E
    {
        Exo sep; sep.text(" - ");                                                              // 00645F33..00645F3C (00748E98)
        Exo tail; tlk(tail, journal_sort_strref_007a2474[sort]);                               // 00645F41..00645F59
        Exo head_text; tlk(head_text, journal_done_mode() ? 0x7DB1u : 0x7DB2u);                // 00645F5E..00645F91 (bit read after the tail)
        Exo joined; concat(head_text, joined, sep);                                            // 00645F96..00645FA7
        Exo full; concat(joined, full, tail);                                                  // 00645FAC..00645FB9
        async_assign_string_005e5c50(text.s, nullptr, full.s);                                 // 00645FBE..00645FC8
        full.kill(); joined.kill(); head_text.kill(); tail.kill(); sep.kill();                 // 00645FCD..0064600E
    }
    gui_load_text_params_set_text_00415e00(at(self, 0x14C), nullptr, text.s);                  // 00646013..0064601E
    kotor_fastcall_00645330(self, nullptr);                                                    // 00646023..00646025
    vcall<void, U32>(at(self, 0xDF0), 0x0C, 0);                                                // 0064602A..00646038
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 0064603B..0064603D
    text.kill();                                                                               // 00646042..0064604E
}

// 006456E0 HandleInputEvent(int id, int down). Jump table 00645D28 / index bytes 00645D44 (id - 28h, 0..B7h):
//   28h/2Dh/2Eh/DFh -> HideSWInGameGui(0) and input class 0;  29h -> open the quest-items panel (+FB4);
//   2Ah -> swap active/done;  2Bh -> next sort mode;  39h/3Ah -> scroll the description (+1A4) as 31h/32h.
// Every path ends in CSWGuiPanel::HandleInputEvent(id, down).
void __fastcall kotor_fastcall_006456e0(void* self, void*, std::uint32_t id, std::uint32_t down) {
    if (down != 0) {                                                                           // 00645701..0064570B
        void* journal = quest_journal();                                                       // 00645711..0064571E (edi)
        switch (id) {                                                                          // 00645720..00645738
        case 0x2B: {                                                                           // 0064573F sort
            U32 current = journal_done_mode() ? packed_file_get_async_file_00644760(journal, nullptr)   // 0064573F..00645762
                                              : aurora_txi_size_00712fc0(journal, nullptr);
            ++current;                                                                         // 00645764
            w4::journal_sort_00833a90 = current;                                               // 00645768
            if (static_cast<I32>(current) >= 4) w4::journal_sort_00833a90 = 0;                 // 00645765..0064576F
            if (journal_done_mode()) kotor_fastcall_00676bf0(journal, nullptr, w4::journal_sort_00833a90); // 00645779..006457A2
            else kotor_fastcall_00676bc0(journal, nullptr, w4::journal_sort_00833a90);
            I32 next = static_cast<I32>(w4::journal_sort_00833a90) + 1;                        // 006457A4..006457AA
            if (next >= 4) next = 0;                                                           // 006457AB..006457B0
            Exo text;                                                                          // [esp+18] state 5
            tlk_join(text, 0xA646, " ", journal_sort_strref_007a2474[next], false);            // 006457B2..00645875
            gui_load_text_params_set_text_00415e00(at(self, 0xD98), nullptr, text.s);          // 0064587A..00645885
            {
                Exo sep; sep.text(" - ");                                                      // 0064588A..00645893
                Exo tail; tlk(tail, journal_sort_strref_007a2474[w4::journal_sort_00833a90]);  // 00645898..006458B9
                Exo head; tlk(head, journal_done_mode() ? 0x7DB1u : 0x7DB2u);                  // 006458C0..006458F8
                Exo joined; concat(head, joined, sep);                                         // 006458FD..00645911
                Exo full; concat(joined, full, tail);                                          // 00645916..0064592A
                async_assign_string_005e5c50(text.s, nullptr, full.s);                         // 0064592F..0064593C
                full.kill(); joined.kill(); head.kill(); tail.kill(); sep.kill();              // 00645941..00645996
            }
            gui_load_text_params_set_text_00415e00(at(self, 0x14C), nullptr, text.s);          // 0064599B..006459A6
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                               // 006459AB..006459B0
            text.kill();                                                                       // 006459B5..006459C4
            break;
        }
        case 0x2A: {                                                                           // 006459CE swap
            kotor_fastcall_00645610(self, nullptr);                                            // 006459CE..006459D0
            w4::journal_sort_00833a90 = journal_done_mode() ? packed_file_get_async_file_00644760(journal, nullptr) // 006459D5..006459FF
                                                            : aurora_txi_size_00712fc0(journal, nullptr);
            const U32 value = ~lw(in_game_gui(), 0xBC4) & 1u;                                  // 006459F9..00645A1D
            kotor_fastcall_00644740(in_game_gui(), nullptr, value);                            // 00645A20..00645A28
            kotor_fastcall_00645330(self, nullptr);                                            // 00645A2D..00645A2F
            Exo text; text.def();                                                              // 00645A34..00645A38 [esp+10] state 0Eh
            if (journal_done_mode()) {                                                         // 00645A3D..00645A64
                gui_load_text_params_set_strref_00415e50(at(self, 0xBD4), nullptr, 0x7DB2);    // 00645A5E..00645A6F
                tlk_join(text, 0x7DB1, " - ", journal_sort_strref_007a2474[w4::journal_sort_00833a90], true); // 00645A74..00645B57
            } else {
                gui_load_text_params_set_strref_00415e50(at(self, 0xBD4), nullptr, 0x7DB1);    // 00645B54..00645B59
                tlk_join(text, 0x7DB2, " - ", journal_sort_strref_007a2474[w4::journal_sort_00833a90], true); // 00645B5E..00645C57
            }
            gui_load_text_params_set_text_00415e00(at(self, 0x14C), nullptr, text.s);          // 00645C5C..00645C67
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                               // 00645C6C..00645C71
            text.kill();                                                                       // 00645C76..00645C85
            break;
        }
        case 0x29:                                                                             // 00645C8C quest items
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                               // 00645C8C..00645C91
            kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, lp(self, 0xFB4), 3, 1);           // 00645C96..00645CA4
            break;
        case 0x28: case 0x2D: case 0x2E: case 0xDF:                                            // 00645CAB
            if (frame_R4_0062cba0(in_game_gui(), nullptr, 0) != 0)                             // 00645CAB..00645CC4
                kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                          // 00645CC6..00645CD3
            break;
        case 0x39:                                                                             // 00645CDA
            vcall<void, U32, U32>(at(self, 0x1A4), 0x3C, 0x31, 1);                             // 00645CDA..00645CEA
            break;
        case 0x3A:                                                                             // 00645CEF
            vcall<void, U32, U32>(at(self, 0x1A4), 0x3C, 0x32, 1);                             // 00645CEF..00645CFF
            break;
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, id, down);                             // 00645D02..00645D0C
}
}  // extern "C"
