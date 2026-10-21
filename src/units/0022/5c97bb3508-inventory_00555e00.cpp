// Reconstruction of original swkotor.exe 00555E00
//   CSWSInventory::FindItemByBaseItem
//
// Original range 00555E00..00555EC5.  ABI: ECX = inventory; stack
// (baseItem, nth); callee ret 8.  Returns the nth item id whose base-item
// rule byte [+0xAC] equals low8(baseItem), recursing into item containers at
// item+0x26C first; 0x7F000000 when none matches.
//
// The body preserves the original's re-reads: the list pointer, list count and
// the base-item rule are read at each site, the container recursion precedes
// the direct comparison, and the original's second base-item read is
// unconditional on every non-return path (including first-compare failure).
// No trap/stub/guard.

#include <cstdint>

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;   // [0x007A39FC] CAppManager*
}

extern "C" {
void* __fastcall recovered_item_lookup_004ae760(void*, void*, unsigned);
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void*, void*);
}

extern "C" unsigned __fastcall recovered_find_item_by_base_item_00555e00(
        void* self, void* /*edx unused*/, unsigned baseItem, unsigned nth) {
    constexpr unsigned kInvalid = 0x7f000000u;
    const unsigned char base = static_cast<unsigned char>(baseItem);
    unsigned matches = 0;
    int i = 0;
    for (;;) {
        unsigned char* inv = static_cast<unsigned char*>(self);
        // The original re-reads count and list every iteration.
        if (i >= *reinterpret_cast<const int*>(inv + 0x10)) break;
        unsigned* list = *reinterpret_cast<unsigned**>(inv + 0xC);
        const unsigned id = list[i];

        void* manager = kotor_menu_window_cells::manager_007a39fc;
        void* server = *reinterpret_cast<void**>(
            static_cast<unsigned char*>(manager) + 8);
        unsigned char* item = static_cast<unsigned char*>(
            recovered_item_lookup_004ae760(server, nullptr, id));
        if (item != nullptr) {
            void* container = *reinterpret_cast<void**>(item + 0x26C);
            if (container != nullptr) {
                const unsigned found = recovered_find_item_by_base_item_00555e00(
                    container, nullptr, baseItem, nth);
                if (found != kInvalid) return found;
            }
            unsigned char* rule = static_cast<unsigned char*>(
                kotor_m15b_item_get_base_item_005b4790(item, nullptr));
            // 00555E5D..74: a match on the requested ordinal returns the id.
            if (base == rule[0xAC] && matches == nth) return id;
            // 00555E76..83: the original re-reads the base-item rule on every
            // non-return path, including the first-compare-fail path.
            rule = static_cast<unsigned char*>(
                kotor_m15b_item_get_base_item_005b4790(item, nullptr));
            if (base == rule[0xAC] && nth != matches) {
                ++matches;                               // 00555E85..92
            }
        }
        ++i;                                             // 00555E96..A4
    }
    return kInvalid;                                     // 00555EAA
}
