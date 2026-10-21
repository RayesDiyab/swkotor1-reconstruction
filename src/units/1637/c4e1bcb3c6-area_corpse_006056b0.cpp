// 006056B0 CSWCArea::AddCorpse.
//
// Original body: 006056B0..006057AF (one stack argument, RET 4).
//
// Exact pinned-original control flow (evidence/oracle-006056B0-full-disasm.txt):
//   6056B3  ring[write] = id
//   6056C5  write = write + 1; if write >= 4 write = 0   (store then zero)
//   6056EC  if read != write, jump to 6057A9 (function tail / RET4)
//   6056F2  client = [manager+4]; creature = 005ED5B0(client, ring[read])
//   605710  if creature == null, jump to tail reset 60577B
//   605712  placeableId = [creature+0x3DC]
//   605718  client = [manager+4] again (the cell is re-read, not cached)
//   605723  placeable = 005ED5F0(client, placeableId)
//   60572C  if placeable == null, jump to creature fade calls 605752
//   60572E  server = 00682E00(placeable)
//   605739  if server != null, [server+0x444] = 1
//   605743  0063CE60(placeable, 1, 0, 0, 0)  [args pushed 0,0,0,1]
//   605752  0063E220(creature, 1)
//   60575B  0063CE60(creature, 0, 1, 0xAFC8, 0x3E8)
//   605770  [creature+0xE4] = 1
//   60577B  ring[read] = 0x7F000000
//   60578D  read = byte[this+0x278] AGAIN after the sentinel store (a fresh
//           load, not the 60577B value still in edx)
//   605793  read = read + 1; if read >= 4 read = 0
//   6057A9  pop esi; RET 4
//
// Three details are material and are NOT the simplified prose in the R4 contract:
//   * the non-full branch returns without resetting the read slot or advancing
//     the read index (the jne target is 6057A9, after the reset at 60577B);
//   * the full path looks up ring[read] (the oldest slot), not the incoming id
//     and not ring[write];
//   * the tail reset re-reads +0x278 at 60578D after the sentinel store. If that
//     store aliases the index bytes (unguarded read index >= 4), the fresh value
//     is what gets incremented; it is not clamped.
// This source follows the pinned original and records the deviation in
// EVIDENCE.md and INTEGRATION.md for independent Astra review.
#include "9f6ea677d3-area_corpse_006056b0.h"

// Explicit global-scope using-declarations: a using-directive inside an unnamed
// namespace would not make these helpers or types visible in the global-scope
// body.
using r4_sound_corpse::load_ptr;
using r4_sound_corpse::load_u32;
using r4_sound_corpse::load_u8;
using r4_sound_corpse::store_u8;
using r4_sound_corpse::store_u32;
using r4_sound_corpse::U;
using r4_sound_corpse::U8;
using r4_sound_corpse::U32;

namespace {

constexpr U32 kEmptyCorpseSlot = 0x7F000000u;
constexpr U kRingOffset = 0x268;
constexpr U kReadIndexOffset = 0x278;
constexpr U kWriteIndexOffset = 0x279;
constexpr U kRingCapacity = 4;

// The original stores the incremented byte first and then overwrites it with
// zero when the comparison shows >= 4. Both stores are preserved in this order.
inline void advance_index(void* self, U offset, U8 current) noexcept {
    const U8 next = static_cast<U8>(static_cast<unsigned>(current) + 1u);
    store_u8(self, offset, next);
    if (next >= kRingCapacity) store_u8(self, offset, 0);
}

} // namespace

extern "C" void __fastcall kotor_area_add_corpse_006056b0(void* self, void*, std::uint32_t id) noexcept(false) {
    // 006056B3..006056D7: write the incoming id at the current write slot, then
    // advance/wrap the write index. The original re-reads the write byte at
    // 006056C5 after the ring store; preserve that read, not a cached value.
    const U8 write = load_u8(self, kWriteIndexOffset);
    store_u32(self, kRingOffset + 4u * write, id);
    advance_index(self, kWriteIndexOffset, load_u8(self, kWriteIndexOffset));

    // 006056E0..006056EC: not full -> return at 006057A9. No read-slot reset and
    // no read-index advance occur on this path.
    const U8 read = load_u8(self, kReadIndexOffset);
    if (read != load_u8(self, kWriteIndexOffset)) return;

    // 006056F2..006056FB: the lookup id is the oldest ring slot (ring[read]).
    void* manager = kotor_menu_window_cells::manager_007a39fc;
    void* client = load_ptr(manager, 4);
    const U32 corpse_id = load_u32(self, kRingOffset + 4u * read);
    void* const creature = frame_R4_005ed5b0(client, nullptr, corpse_id);

    if (creature != nullptr) {
        // 00605712..0060571E: the original reloads the AppManager cell and its
        // +4 client before the placeable lookup; it does not reuse the first
        // client pointer.
        const U32 placeable_id = load_u32(creature, 0x3DC);
        manager = kotor_menu_window_cells::manager_007a39fc;
        client = load_ptr(manager, 4);
        void* const placeable = recovered_client_get_placeable_by_id_005ed5f0(client, nullptr, placeable_id);

        if (placeable != nullptr) {
            // 0060572E..0060574D: server placeable, optional +0x444 flag, then
            // the placeable desired-fade call. The fade call is made even when
            // 00682E00 returns null; only the +0x444 write is conditional.
            void* const server = kotor_fastcall_00682e00(placeable, nullptr);
            if (server != nullptr) store_u32(server, 0x444, 1);
            recovered_client_object_set_desired_fade_state_0063ce60(placeable, nullptr, 1, 0, 0, 0);
        }

        // 00605752..00605770: creature fade calls and +0xE4, regardless of the
        // placeable result.
        recovered_client_object_set_fade_state_0063e220(creature, nullptr, 1);
        recovered_client_object_set_desired_fade_state_0063ce60(creature, nullptr, 0, 1, 0xAFC8, 0x3E8);
        store_u32(creature, 0xE4, 1);
    }

    // 0060577B..006057A2: tail reset, reached for every full-ring call including
    // a null creature. Store the sentinel at ring[read], then re-read +0x278 for
    // the increment: 0060578D reloads the byte after the sentinel store and does
    // not reuse the 0060577B value. An aliased index (>= 4) is deliberately not
    // clamped or guarded; the fresh value is what advances.
    const U8 tail_read = load_u8(self, kReadIndexOffset);
    store_u32(self, kRingOffset + 4u * tail_read, kEmptyCorpseSlot);
    advance_index(self, kReadIndexOffset, load_u8(self, kReadIndexOffset));
}
