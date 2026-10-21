#include "73fb4cc128-w2_save_item.hpp"
#include <cstdint>
namespace w2::save_item {
namespace {
using I = std::int32_t;
Word bit(Memory& m, Word item, Word shift) { return (m.read(item + 0x288) >> shift) & 0xffffff01u; }   // shr; and 0FFFFFF01h (dword pushed)
}

// 0055CCD0. Field order and labels exactly as the original call sequence.
Word save_item_0055ccd0(Memory& m, Ports& p, Word item, Word gff, Word st, Word s) {
    p.write_int_004127b0(gff, st, m.read(item + 0x0c), 0x00749018);           // BaseItem
    p.write_string_00412970(gff, st, item + 0x28, 0x00747a28);                // Tag
    p.write_byte_00412620(gff, st, 1, 0x0074900c);                            // Identified (constant 1)
    p.write_locstring_00412a10(gff, st, item + 0x278, 0x007488c0);            // Description
    p.write_locstring_00412a10(gff, st, item + 0x270, 0x00748ffc);            // DescIdentified
    p.write_locstring_00412a10(gff, st, item + 0x280, 0x00748fec);            // LocalizedName
    p.write_word_004126c0(gff, st, m.read16(item + 0x28c), 0x00748fe0);       // StackSize
    p.write_byte_00412620(gff, st, bit(m, item, 5), 0x00748fd8);              // Stolen
    p.write_dword_00412760(gff, st, m.read(item + 0x294), 0x00748fcc);        // Upgrades
    p.write_byte_00412620(gff, st, bit(m, item, 3), 0x0074721c);              // Dropable
    p.write_byte_00412620(gff, st, bit(m, item, 4), 0x00748fbc);              // Pickpocketable
    p.write_byte_00412620(gff, st, m.read8(item + 0x264), 0x00748fac);        // ModelVariation
    const Word array = m.read(p.rules_007a3a28() + 0x34);
    if (m.read8(p.base_item_005b31a0(array, m.read(item + 0x0c)) + 0x18) == 1) {   // 0055CDF2 model type 1
        p.write_byte_00412620(gff, st, m.read8(item + 0x265), 0x00748f9c);    // BodyVariation
        p.write_byte_00412620(gff, st, m.read8(item + 0x266), 0x00748f90);    // TextureVar
    }
    p.write_byte_00412620(gff, st, m.read8(item + 0x258), 0x00748f88);        // Charges
    p.write_byte_00412620(gff, st, m.read8(item + 0x25c), 0x00748f7c);        // MaxCharges
    p.write_dword_00412760(gff, st, p.item_cost_00554000(item), 0x00748f74);  // Cost
    p.write_dword_00412760(gff, st, m.read(item + 0x260), 0x00748f6c);        // AddCost
    p.write_byte_00412620(gff, st, m.read8(item + 0x108), 0x00748f64);        // Plot
    if (m.read(p.base_item_005b31a0(m.read(p.rules_007a3a28() + 0x34), m.read(item + 0x0c)) + 0x70) != 0)
        (void)save_container_items_0055cfa0(m, p, item, gff, st, s);          // 0055CEAC
    (void)save_item_properties_00555790(m, p, item, gff, st, s);
    p.write_float_00412870(gff, st, m.read(item + 0xa0), 0x00745d80);         // XPosition
    p.write_float_00412870(gff, st, m.read(item + 0xa4), 0x00745d74);         // YPosition
    p.write_float_00412870(gff, st, m.read(item + 0xa8), 0x00745d68);         // ZPosition
    p.write_float_00412870(gff, st, m.read(item + 0xac), 0x00745d38);         // XOrientation
    p.write_float_00412870(gff, st, m.read(item + 0xb0), 0x00745d48);         // YOrientation
    p.write_float_00412870(gff, st, m.read(item + 0xb4), 0x00745d58);         // ZOrientation
    p.write_byte_00412620(gff, st, bit(m, item, 6), 0x00748f54);              // NonEquippable
    p.write_byte_00412620(gff, st, bit(m, item, 7), 0x00748f4c);              // NewItem
    p.write_byte_00412620(gff, st, bit(m, item, 8), 0x00748f40);              // DELETING
    return 1;
}

// 0055CFA0: "ItemList" of the contained repository [item+26Ch]; each element struct is written by SaveItem.
Word save_container_items_0055cfa0(Memory& m, Ports& p, Word item, Word gff, Word parent, Word s) {
    const Word list = s, element = s + 0x14;   // original: list local; element struct reuses the parent argument slot
    p.add_list_00412450(gff, list, parent, 0x00747210);                       // ItemList
    for (Word i = 0; i < m.read(m.read(item + 0x26c) + 0x10); ++i) {          // unsigned JB, count re-read
        p.add_list_element_004124e0(gff, element, list, i);
        const Word repo = m.read(item + 0x26c);
        Word child = 0;
        if (static_cast<I>(i) >= 0 && static_cast<I>(i) < static_cast<I>(m.read(repo + 0x10)))
            child = p.item_by_id_004ae760(m.read(p.app_007a39fc() + 8), m.read(m.read(repo + 0x0c) + 4 * i));
        if (child == 0) p.trap(0x0055d013, "W2: SaveContainerItems on a missing item id (original calls SaveItem on null)");
        (void)p.save_item_0055ccd0(child, gff, element);
    }
    return 1;
}

// 00555790: "PropertiesList": passive properties [+250h] (count +248h) then active [+254h] (count +24Ch), 1Ch stride.
// Element struct type: passive index; active: passive count (re-read) + index.
Word save_item_properties_00555790(Memory& m, Ports& p, Word item, Word gff, Word parent, Word s) {
    const Word list = s, element = s + 0x14;
    p.add_list_00412450(gff, list, parent, 0x00748e84);                       // PropertiesList
    auto fields = [&](Word e) {
        p.write_word_004126c0(gff, element, m.read16(e), 0x00748e74);         // PropertyName
        p.write_word_004126c0(gff, element, m.read16(e + 2), 0x00748e6c);     // Subtype
        p.write_byte_00412620(gff, element, m.read8(e + 4), 0x00748e60);      // CostTable
        p.write_word_004126c0(gff, element, m.read16(e + 6), 0x00748e54);     // CostValue
        p.write_byte_00412620(gff, element, m.read8(e + 8), 0x00748e4c);      // Param1
        p.write_byte_00412620(gff, element, m.read8(e + 9), 0x00748e40);      // Param1Value
        p.write_byte_00412620(gff, element, m.read8(e + 0x0a), 0x00748e30);   // ChanceAppear
        p.write_byte_00412620(gff, element, m.read8(e + 0x10), 0x00748bfc);   // UsesPerDay
        p.write_byte_00412620(gff, element, m.read8(e + 0x0c), 0x00748e28);   // Useable
        p.write_byte_00412620(gff, element, m.read8(e + 0x11), 0x00748db8);   // UpgradeType
    };
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(item + 0x248)); ++i) {
        p.add_list_element_004124e0(gff, element, list, i);
        fields(m.read(item + 0x250) + i * 0x1c);
    }
    for (Word i = 0; static_cast<I>(i) < static_cast<I>(m.read(item + 0x24c)); ++i) {
        p.add_list_element_004124e0(gff, element, list, m.read(item + 0x248) + i);
        fields(m.read(item + 0x254) + i * 0x1c);
    }
    return 1;
}
}
