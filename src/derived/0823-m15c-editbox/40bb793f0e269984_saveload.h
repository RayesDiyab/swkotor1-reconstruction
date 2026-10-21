#pragma once
// lane-m1-saveload (Milestone 1, L3): CSWGuiSaveLoad (load mode, opened by CSWGuiMainMenu::OnLoadSaveGame 0067B1A0),
// CSWGuiSaveLoadEntry and the CSWGuiRadioButton leaves its method table needs. swkotor.exe GOG K1, image base
// 0x400000; static disassembly only (nothing executed). Every body cites its original instruction addresses.
//
// Conventions (DECISIONS D1/D21/D24/D47):
//  * original __thiscall entries that are only called directly / through method-table slots are extern "C"
//    __fastcall(Self*, void* unused_edx, original stack args...) veneers with identical callee cleanup (D1);
//  * entries that are CSWGuiControl event callbacks or CSWGuiMessageBox callbacks (thiscall, RET 4, one
//    CSWGuiControl* argument) are __thiscall members of single-inheritance views of kotor_input_hover::GuiObject
//    so their PMFs are one code word; registration converts them with static_cast to the base PMF (D47);
//  * method tables are typed constant structs whose fields have the exact declared type of the function they
//    hold; the object's vptr word receives the table address by an object-pointer conversion (D21/D24);
//  * layouts are byte-offset views on the gui_load control family; every named offset cites the instruction
//    that establishes it. Unknown bytes stay opaque.
#include "c9d0373605f94236_mainmenu.h"     // gui_load views, AddPanel 0040BC70, message-box/TLK/resman externals (gui-load lane)
#include "9fda418b8be92652_listbox.h"      // gui_lb::ListBox (+2A0 items, +2C6 selected, +2CC proto)
#include "29a305efc7265995_m1_shared.h"    // L0 (lane-m1-options/shared): 00624BA0/B0/C0 thunks, 00418E80, 0041A5D0
#include <cstddef>
#include <cstdint>

namespace kotor_m1_saveload {
using gui_load::Word;
using gui_load::I32;
using gui_load::String;
using gui_load::ResRef;
namespace kih = kotor_input_hover;

// ------------------------------------------------------------------ CSWGuiSaveLoad (0x1160; operator new at 0067B1DD)
struct SaveLoad : gui_load::Panel {
    Word sl_flags;                       // +0064 bit0 save mode (ctor arg2), bit1 opened from the main menu (ctor arg3)
                                         //       (006CCACA..006CCAE1); tested 006C870A / 006CC100 / 006CC2EA / 006C8D67
    Word next_number;                    // +0068 = 2 (006CCAE4); next free save number (006CC560)
    unsigned char directory[8];          // +006C CExoString (005B3190 at 006CC6C1; = "" at 006CC9DA): resource
                                         //       directory currently added for the shown game (006C8B83..006C8BD1)
    gui_load::Label lbl_panelname;       // +0074 "LBL_PANELNAME"  (006CC6CE / 006CC7A8)
    gui_load::Label lbl_screenshot;      // +01B4 "LBL_SCREENSHOT" (006CC6DE / 006CC7DE)
    gui_load::Label lbl_planetname;      // +02F4 "LBL_PLANETNAME" (006CC6EE / 006CC814)
    gui_load::Label lbl_areaname;        // +0434 "LBL_AREANAME"   (006CC6FE / 006CC84A)
    gui_load::Label lbl_pm[3];           // +0574 "LBL_PM1..3" (eh_vector_constructor_iterator 006CC720, 3 x 0x140)
    gui_load::ListBox lb_games;          // +0934 "LB_GAMES"       (006CC730 / 006CC922)
    gui_load::Button btn_saveload;       // +0C14 "BTN_SAVELOAD"   (006CC742 / 006CC952)
    gui_load::Button btn_back;           // +0DD8 "BTN_BACK"       (006CC752 / 006CC988)
    gui_load::Button btn_delete;         // +0F9C "BTN_DELETE"     (006CC764 / 006CC9B8)
};
static_assert(offsetof(SaveLoad, sl_flags) == 0x64 && offsetof(SaveLoad, directory) == 0x6C &&
              offsetof(SaveLoad, lbl_panelname) == 0x74 && offsetof(SaveLoad, lbl_screenshot) == 0x1B4 &&
              offsetof(SaveLoad, lbl_planetname) == 0x2F4 && offsetof(SaveLoad, lbl_areaname) == 0x434 &&
              offsetof(SaveLoad, lbl_pm) == 0x574 && offsetof(SaveLoad, lb_games) == 0x934 &&
              offsetof(SaveLoad, btn_saveload) == 0xC14 && offsetof(SaveLoad, btn_back) == 0xDD8 &&
              offsetof(SaveLoad, btn_delete) == 0xF9C && sizeof(SaveLoad) == 0x1160, "CSWGuiSaveLoad layout");

// ------------------------------------------------------------------ CSWGuiSaveLoadEntry (0x248; operator new 006CC290)
// Derived from CSWGuiRadioButton (dtor restores vtable 007566D8 at 006CBB3A) which adds +1C4 to CSWGuiButton.
struct SaveLoadEntry : gui_load::Button {
    Word entry_flags;                    // +1C4 bit0 info read, bit1 corrupt, bit3 REBOOTAUTOSAVE, bit4 PCAUTOSAVE,
                                         //      bit5 live content missing, bit7 CHEATUSED (006C9177, 006C93AB..006C9422,
                                         //      006C9687..006C96A6, 006C971C); bits2|6 "new save" item (006CC487)
    I32 number;                          // +1C8 save number (006C8F06; 006CC495)
    Word time_played;                    // +1CC TIMEPLAYED (006C92F1)
    Word u1d0;                           // +1D0 = 0 (006CBA42)
    unsigned char gameplay_hint;         // +1D4 GAMEPLAYHINT (006CBA28; 006C94B8)
    unsigned char story_hint;            // +1D5 STORYHINT (006CBA2F; 006C94F6)
    unsigned char u1d6[2];
    unsigned char save_name[8];          // +1D8 SAVEGAMENAME (= "Bad" at 006CBA48; 006C932A)
    unsigned char dir_suffix[8];         // +1E0 directory name after "NNNNNN - " (006C8F37)
    unsigned char area_name[8];          // +1E8 AREANAME (006C9231)
    unsigned char last_module[8];        // +1F0 LASTMODULE (006C929C)
    unsigned char s1f8[8];               // +1F8 (constructed 006CB9B0; not read on this path)
    unsigned char title[8];              // +200 title text (006C97B3, 006C996A, 006C9A2E)
    unsigned char screenshot[8];         // +208 SCREENSHOT (006C945F)
    unsigned char live_missing[8];       // +210 missing LIVEn content name (006C972E) / "" (006C96A1)
    ResRef portraits[3];                 // +218 PORTRAIT0..2 (006CB9F5..006CBA0B; 006C9655)
};
static_assert(offsetof(SaveLoadEntry, entry_flags) == 0x1C4 && offsetof(SaveLoadEntry, number) == 0x1C8 &&
              offsetof(SaveLoadEntry, time_played) == 0x1CC && offsetof(SaveLoadEntry, gameplay_hint) == 0x1D4 &&
              offsetof(SaveLoadEntry, save_name) == 0x1D8 && offsetof(SaveLoadEntry, dir_suffix) == 0x1E0 &&
              offsetof(SaveLoadEntry, area_name) == 0x1E8 && offsetof(SaveLoadEntry, title) == 0x200 &&
              offsetof(SaveLoadEntry, screenshot) == 0x208 && offsetof(SaveLoadEntry, live_missing) == 0x210 &&
              offsetof(SaveLoadEntry, portraits) == 0x218 && sizeof(SaveLoadEntry) == 0x248, "entry layout");

// ------------------------------------------------------------------ event / message-box callbacks (thiscall RET 4)
struct SaveLoadHandlers : kih::GuiObject {
    void __thiscall ShowGame_006c89d0(kih::Control* entry);        // 006C89D0 (event 0 of each entry; direct 006CC3BE)
    void __thiscall RemoveGame_006c8900(kih::Control* unused);     // 006C8900 (message-box callback, 006CAAC9)
    void __thiscall DeleteGame_006caa90(kih::Control* control);    // 006CAA90 (BTN_DELETE 27h; entries 29h)
    void __thiscall VerifyLoadGame_006cc0e0(kih::Control* control);// 006CC0E0 (load-mode BTN_SAVELOAD / entries 27h)
    void __thiscall LoadGame_006cb0e0(kih::Control* entry);        // 006CB0E0 D83 function TRAP (server creation)
    void __thiscall HandleSaveButton_006cbb60(kih::Control* ctrl); // 006CBB60 D83 function TRAP (save mode only)
};
// CSWGuiRadioButton mouse leaves (method-table slots +18/+1C of 007566D8 and 00757740).
struct RadioButtonLeaves : kih::Control {
    void __thiscall HandleLMouseDown_00418680();                   // 00418680 RET
    void __thiscall HandleLMouseUp_00418e90();                     // 00418E90 RET (tail jmp vslot +14)
};
}

extern "C" {
// ---- authored here (new names)
kotor_m1_saveload::SaveLoad* __fastcall kotor_m1_saveload_construct_006cc680(kotor_m1_saveload::SaveLoad*, void*,
    gui_load::Object* manager, gui_load::Word mode, gui_load::Word from_main_menu) noexcept(false);
void __fastcall kotor_m1_saveload_destruct_006ca110(kotor_m1_saveload::SaveLoad*, void*) noexcept(false);
void* __fastcall kotor_m1_saveload_deleting_destruct_006cb0c0(kotor_m1_saveload::SaveLoad*, void*, gui_load::Word) noexcept(false);
void __fastcall kotor_m1_saveload_handle_input_006c86d0(gui_load::Object*, void*, gui_load::I32, gui_load::I32) noexcept(false);
void __fastcall kotor_m1_saveload_populate_game_list_006cc160(kotor_m1_saveload::SaveLoad*, void*) noexcept(false);
kotor_m1_saveload::SaveLoadEntry* __fastcall kotor_m1_saveload_entry_construct_006cb940(kotor_m1_saveload::SaveLoadEntry*, void*) noexcept(false);
void __fastcall kotor_m1_saveload_entry_destruct_006cba90(kotor_m1_saveload::SaveLoadEntry*, void*) noexcept(false);
void* __fastcall kotor_m1_saveload_entry_deleting_destruct_006cba70(kotor_m1_saveload::SaveLoadEntry*, void*, gui_load::Word) noexcept(false);
void __fastcall kotor_m1_saveload_entry_load_data_006c8e50(kotor_m1_saveload::SaveLoadEntry*, void*, gui_load::String*) noexcept(false);
void __fastcall kotor_m1_saveload_entry_set_xbox_title_006c9780(kotor_m1_saveload::SaveLoadEntry*, void*, gui_load::String*) noexcept(false);
gui_load::ResRef* __fastcall kotor_m1_saveload_entry_get_portrait_006c8200(kotor_m1_saveload::SaveLoadEntry*, void*,
    gui_load::ResRef* out, gui_load::I32 index) noexcept(false);
gui_load::String* __fastcall kotor_m1_saveload_entry_get_game_directory_006c8250(kotor_m1_saveload::SaveLoadEntry*, void*,
    gui_load::String* out) noexcept(false);
void __fastcall kotor_m1_radio_button_destruct_006be360(gui_load::Button*, void*) noexcept(false);
void* __fastcall kotor_m1_radio_button_deleting_destruct_006b60d0(gui_load::Button*, void*, gui_load::Word) noexcept(false);
extern const struct KotorM1SaveLoadMethods kotor_m1_saveload_methods_00757650;
extern const struct KotorM1SaveLoadEntryMethods kotor_m1_saveload_entry_methods_00757740;
extern const struct KotorM1RadioButtonMethods kotor_m1_radio_button_methods_007566d8;
// .data colour cells (no .text writer: every reference is a read / address push), proposed owner this lane.
extern const MenuColor kotor_m1_color_007a2378;   // {1.0, 1.0, 1.0}   (006C8D9C, 006C8DC9, 006C8E0E)
extern const MenuColor kotor_m1_color_007a2384;   // {0.35,0.35,0.35}  (006C8D95, 006C8E15)
extern const MenuColor kotor_m1_color_007a2408;   // {0.0, 0.33,0.49}  (006C8D76, 006C8DF6)
}
