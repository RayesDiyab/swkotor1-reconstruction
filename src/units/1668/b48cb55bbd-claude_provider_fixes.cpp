// W5 shared-provider trap fixes (lane worker "provider-traps"): the bodies that the review's provider blockers B1-B7
// (research/REVIEW-w5-actions.md) need and that exist in no lane. Everything else in B1-B7 is a rebind inside an
// existing source; those edits are listed in research/PROVIDER-FIX-EDITS.md (applied by the coordinator).
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are the original instructions.
//
// Functions written here:
//   00587560 CSWSPlaceable::CloseInventory(closer id, take)  thiscall RET 8  -> kotor_fastcall_00587560 (@16)
//     (B3: called by CSWSCreature::AutoCloseGUIPanels 004EDED0, w5_drive.cpp, for an open placeable inventory)
// Memory: live_operator_new / live_free only. The original SEH state around operator new + CScriptEvent ctor is kept as
// a catch(...) that frees the block and rethrows.
#include "842d6b1618-w5_t3.hpp"
#include "5889fde5f1-w5_action_bodies.hpp"

using namespace w5;

extern "C" {
// ---------------- existing providers (c08 map; exact decorated names; bodies checked for traps) ----------------
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                         // CSWSPlayer::GetGameObject [0595]
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);                         // CItemRepository::ItemListGetItem [0886]
unsigned __fastcall recovered_remove_item_00555fd0(void* repository, void*, void* item);                // CItemRepository::RemoveItem [w2_party_native]
unsigned __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a, U b, U c); // CItemRepository::AddItem [1088]
void __fastcall recovered_item_owner_00553210(void* item, void*, U owner, U a, U b, U c);               // CSWSItem::SetPossessor [1013]

// ---------------- W5 export written here ----------------
void __fastcall kotor_fastcall_00587560(void* placeable, void*, U closer, U take);
}

namespace {
constexpr U k_ai_event_script_007441dc = 10;     // .rdata 007441DC
constexpr U k_ai_event_destroy_007441e0 = 11;    // .rdata 007441E0
constexpr U k_anim_close_0000275c = 0x275c;      // push 275Ch (00587688)

// operator new(34h) + CScriptEvent ctor (0058762F..00587654); the original EH state ([esp+14]=0) frees the block when
// the constructor throws. A failed allocation yields a null event that is then written through (original quirk).
void* new_script_event() {
    void* memory = live_operator_new(0x34);
    if (memory == nullptr) return nullptr;
    try { return recovered_event_construct_004d7540(memory, nullptr); } catch (...) { live_free(memory); throw; }
}
}  // namespace

extern "C" {

// 00587560 CSWSPlaceable::CloseInventory(closer, take): with `take`, every item of the placeable repository (+36C) moves,
// last first, into the closer's creature repository and its possessor becomes the closer. Then OnClosed (17h) is queued
// from the closer to the placeable, the close animation 275Ch plays (vtbl+7C), the open flag +338 is cleared and an
// emptied placeable flagged +324 (and +334) clears +328 and queues its own destroy event (11).
void __fastcall kotor_fastcall_00587560(void* placeable, void*, U closer, U take) {
    if (at<U>(placeable, 0x338) == 0) return;                                       // 00587579..82 -> 005876EE
    if (take != 0) {                                                                // 00587588..92
        void* player = kotor_fastcall_004ae910(server_app(), nullptr, closer);      // 00587598..A1 (EBX = closer)
        if (player != nullptr) {                                                    // 005875A6..A8
            void* creature = vcall<void*>(kotor_fastcall_00561080(player, nullptr), 0x30); // 005875AE..BC (EDI)
            if (at<U>(at<void*>(placeable, 0x36c), 0x10) != 0) {                    // 005875BE..C9 (jbe: unsigned)
                do {
                    void* repository = at<void*>(placeable, 0x36c);                 // 005875D0
                    void* moved = w2x_inventory_item_005560b0(repository, nullptr,
                                                              at<U>(repository, 0x10) - 1); // 005875D6..DB
                    // the item pointer is kept in the `take` argument slot ([esp+20]), its address is AddItem's item**
                    recovered_remove_item_00555fd0(at<void*>(placeable, 0x36c), nullptr, moved); // 005875E0..EB
                    recovered_inventory_insert_0055d330(recovered_creature_inventory_004ef770(creature, nullptr, 1), nullptr,
                                                        &moved, 1, 1, 1);           // 005875F0..06
                    if (moved != nullptr)                                           // 0058760B..11 (re-read slot)
                        recovered_item_owner_00553210(moved, nullptr, closer, 1, 1, k_object_invalid); // 00587613..1D
                } while (at<U>(at<void*>(placeable, 0x36c), 0x10) != 0);            // 00587622..2D (ja: unsigned)
            }
        }
    }
    void* event = new_script_event();                                               // 0058762F..54
    at<U16>(event, 0) = 0x17;                                                       // 00587655 (OnClosed; unchecked)
    recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, closer, at<U>(placeable, 4),
                                     k_ai_event_script_007441dc, event);            // 0058765A..81
    vcall<U>(placeable, 0x7c, k_anim_close_0000275c);                               // 00587686..8F
    const U destroy_when_empty = at<U>(placeable, 0x324);                           // 00587692
    at<U>(placeable, 0x338) = 0;                                                    // 0058769A
    if (destroy_when_empty != 0 && at<U>(placeable, 0x334) != 0 &&                  // 00587698..AE
        at<U>(at<void*>(placeable, 0x36c), 0x10) == 0) {                            // 005876B0..BB
        at<U>(placeable, 0x328) = 0;                                                // 005876BD
        const U self_id = at<U>(placeable, 4);                                      // 005876CD
        recovered_event_enqueue_004b08d0(ai_master(), nullptr, 0, 0, self_id, self_id,
                                         k_ai_event_destroy_007441e0, nullptr);     // 005876C7..E9
    }
}

}  // extern "C"
