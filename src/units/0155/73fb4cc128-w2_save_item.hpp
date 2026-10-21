#pragma once
// W2 tier 6 (fork C): CSWSItem save chain used by CSWSCreature::SaveCreature 00500610.
//   0055CCD0 CSWSItem::SaveItem(CResGFF*, CResStruct*) RET 8, returns 1
//   0055CFA0 CSWSItem::SaveContainerItems(CResGFF*, CResStruct*) RET 8, returns 1  ("ItemList")
//   00555790 CSWSItem::SaveItemProperties(CResGFF*, CResStruct*) RET 8, returns 1  ("PropertiesList")
// GFF writers are passed the ORIGINAL label address (.rdata); the native binding maps it to the text.
// CResStruct is a 4-byte cell, CResList is 20 bytes (out parameters of AddListElement / AddList).
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::save_item {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word rules_007a3a28() = 0;
    virtual Word base_item_005b31a0(Word array, Word type) = 0;           // CSWBaseItemArray::GetBaseItem
    virtual Word item_by_id_004ae760(Word server, Word id) = 0;           // CServerExoApp::GetItemByGameObjectID
    virtual Word item_cost_00554000(Word item) = 0;                       // CSWSItem::GetCost (W2 t4)
    virtual Word save_item_0055ccd0(Word item, Word gff, Word st) = 0;    // recursion seam (SaveContainerItems -> SaveItem)
    // CResGFF writers: (gff, struct cell, value, original label address)
    virtual void write_byte_00412620(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_word_004126c0(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_dword_00412760(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_int_004127b0(Word gff, Word st, Word value, Word label) = 0;
    virtual void write_float_00412870(Word gff, Word st, Word bits, Word label) = 0;
    virtual void write_string_00412970(Word gff, Word st, Word str, Word label) = 0;
    virtual void write_locstring_00412a10(Word gff, Word st, Word loc, Word label) = 0;
    virtual void add_list_00412450(Word gff, Word list_out, Word parent, Word label) = 0;
    virtual void add_list_element_004124e0(Word gff, Word st_out, Word list, Word type) = 0;
};

// s: >= 0x18 bytes of scratch (CResList at s, element CResStruct cell at s + 0x14).
Word save_item_0055ccd0(Memory&, Ports&, Word item, Word gff, Word st, Word s);
Word save_container_items_0055cfa0(Memory&, Ports&, Word item, Word gff, Word parent, Word s);
Word save_item_properties_00555790(Memory&, Ports&, Word item, Word gff, Word parent, Word s);
}
