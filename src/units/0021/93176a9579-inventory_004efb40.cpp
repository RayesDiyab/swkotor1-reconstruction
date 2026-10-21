// Reconstruction of original swkotor.exe 004EFB40
//   CSWSCreature::AddPazaakCard (base source "special" insert path)
//
// Original range 004EFB40..004EFC28.  ABI: ECX = creature; stack (item);
// callee ret 4.  The party-table slot (item.byte[0x264] + 0xB) % 18 gains
// item.dword[0x28C]; the creature's inventory type 1 is searched for base item
// 0x2B.  A hit returns that item id.  A miss allocates 0x29C, constructs a
// CSWSItem at 0x7F000000, builds the template Ref/name "g_I_PazSidebd001"
// (original literal VA 0x746F14), loads the template, and inserts with
// combine=1/notify=0/mark=1, returning the insert result.
//
// The original reads item.byte[0x264] and item.dword[0x28C] before any null
// check; a null item faults and is preserved.  The Ref temporary is a POD; the
// original has no normal-path destructor call for it.  No trap/stub/guard.

#include <cstdint>

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;   // [0x007A39FC] CAppManager*
}
namespace lane_config_required {
void* __fastcall GetPartyTable_004aee70(void*, void*);
}
namespace menu_resources {
struct Ref {
    unsigned char bytes[16];
    Ref* __thiscall construct_00406d80(const char*);
};
}
namespace kotor_menu_10_19 {
namespace dependencies {
void* __cdecl allocate_006fa7e6(unsigned);
}
}

extern "C" {
void* __fastcall recovered_creature_inventory_004ef770(void*, void*, unsigned);
unsigned __fastcall recovered_find_item_by_base_item_00555e00(
        void*, void*, unsigned, unsigned);
void* __fastcall recovered_item_construct_005530a0(void*, void*, unsigned);
unsigned __fastcall recovered_item_template_005608b0(void*, void*, void*);
unsigned __fastcall recovered_inventory_insert_0055d330(
        void*, void*, void**, unsigned, unsigned, unsigned);
}

extern "C" unsigned __fastcall recovered_add_pazaak_card_004efb40(
        void* creature, void* /*edx unused*/, void* item) {
    constexpr unsigned kInvalid = 0x7f000000u;
    unsigned char* it = static_cast<unsigned char*>(item);

    // 004EFB5D..8A: read the item fields before any null check.
    const unsigned slot = (static_cast<unsigned>(it[0x264]) + 0xBu) % 18u;
    void* manager = kotor_menu_window_cells::manager_007a39fc;
    void* server = *reinterpret_cast<void**>(
        static_cast<unsigned char*>(manager) + 8);
    void* party = lane_config_required::GetPartyTable_004aee70(server, nullptr);
    *reinterpret_cast<unsigned*>(
        static_cast<unsigned char*>(party) + 0x120 + slot * 4) +=
        *reinterpret_cast<unsigned*>(it + 0x28C);

    // 004EFB90..A6
    void* inventory = recovered_creature_inventory_004ef770(creature, nullptr, 1);
    const unsigned existing = recovered_find_item_by_base_item_00555e00(
        inventory, nullptr, 0x2Bu, 0);
    if (existing != kInvalid) return existing;           // 004EFBA6..AB

    // 004EFBAD..D8: allocate 0x29C; on failure item2 is null but the original
    // still builds the Ref and calls the template loader.
    void* p = kotor_menu_10_19::dependencies::allocate_006fa7e6(0x29Cu);
    void* item2 = p != nullptr
        ? recovered_item_construct_005530a0(p, nullptr, kInvalid)
        : nullptr;

    // 004EFBDA..F4: Ref/name from literal "g_I_PazSidebd001" (VA 0x746F14).
    menu_resources::Ref ref;
    (void)ref.construct_00406d80("g_I_PazSidebd001");
    (void)recovered_item_template_005608b0(item2, nullptr, &ref);

    // 004EFC02..0F: combine=1, notify=0, mark=1; branch result in EAX.
    return recovered_inventory_insert_0055d330(
        inventory, nullptr, &item2, 1, 0, 1);
}
