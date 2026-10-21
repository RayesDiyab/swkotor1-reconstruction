// W2 tier 2: native owner of 00563340 (party repository lazy getter, party+118h) and its
// constructor 0055D290 CItemRepository::CItemRepository(owner, shared), and (t2b)
// 00556150 CItemRepository::SetParentId.
// Logic for 00563340: the existing (unlinked) owner source
//   docs/kotor/recovery/server-runtime-source/adapted-donors/player-repository-attachment/owner_projection.cpp
// compiled unchanged. Native glue adapted from source/party_repository_native.inc with the
// allocator switched from ::operator new to the game CRT live_operator_new (006FA7E6).
// 0055D290 body: 48-byte original, matches retained-donors construct_repository; re-verified.
#if !defined(_M_IX86)
#error W2 native bindings require the Windows x86 route
#endif
#include "7ba7948e7c-w2_repository.hpp"
#include <cstdint>
#include <cstring>
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
unsigned __fastcall kotor_fastcall_004aea40(void*, void*);
void* __fastcall recovered_inventory_construct_0055d290(void* self, void*, unsigned owner, unsigned shared);
void* __fastcall recovered_item_lookup_004ae760(void*, void*, unsigned);                         // 1025
void __fastcall recovered_item_owner_00553210(void*, void*, unsigned, unsigned, unsigned, unsigned); // 1013
}
namespace {
using kotor::repository_owner_projection::Word;
void* ptr(Word a) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)); }
Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }
struct NativePartyRepository final : kotor::repository_owner_projection::Memory, kotor::repository_owner_projection::Ports, w2::repository::ParentPorts {
    Word read(Word a) const override { if (a == 0x7a39fcu) return word(kotor_menu_window_cells::manager_007a39fc); Word v; std::memcpy(&v, ptr(a), 4); return v; }
    void write(Word a, Word b) override { std::memcpy(ptr(a), &b, 4); }
    std::uint8_t read_byte(Word a) const override { std::uint8_t v; std::memcpy(&v, ptr(a), 1); return v; }
    void write_byte(Word a, std::uint8_t b) override { std::memcpy(ptr(a), &b, 1); }
    void write_short(Word a, std::uint16_t b) override { std::memcpy(ptr(a), &b, 2); }
    Word allocate_006fa7e6(Word a) override { return word(live_operator_new(a)); }
    Word first_pc_004aea40(Word server) override { return kotor_fastcall_004aea40(ptr(server), nullptr); }
    Word construct_0055d290(Word storage, Word owner, Word shared) override { return word(recovered_inventory_construct_0055d290(ptr(storage), nullptr, owner, shared)); }
    Word item_by_id_004ae760(Word server, Word id) override { return word(recovered_item_lookup_004ae760(ptr(server), nullptr, id)); }
    void set_possessor_00553210(Word item, Word owner, Word a, Word b, Word c) override { recovered_item_owner_00553210(ptr(item), nullptr, owner, a, b, c); }
};
}
extern "C" void* __fastcall recovered_party_repository_00563340(void* self, void*) {
    NativePartyRepository p; return ptr(kotor::repository_owner_projection::party_repository(p, p, word(self)));
}
extern "C" void* __fastcall recovered_inventory_construct_0055d290(void* self, void*, unsigned owner, unsigned shared) {
    NativePartyRepository p; return ptr(w2::repository::construct_0055d290(p, word(self), owner, shared));
}
// 00556150 CItemRepository::SetParentId(id) RET 4
extern "C" void __fastcall recovered_repository_parent_00556150(void* self, void*, unsigned id) {
    NativePartyRepository p; w2::repository::set_parent_id_00556150(p, p, word(self), id);
}
