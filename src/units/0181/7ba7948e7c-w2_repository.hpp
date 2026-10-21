#pragma once
// 0055D290 CItemRepository::CItemRepository(owner, shared) and
// 00556150 CItemRepository::SetParentId(id) logic (W2 owner).
#include "4cb6100695-owner_projection.hpp"
namespace w2::repository {
using kotor::repository_owner_projection::Word;
Word construct_0055d290(kotor::character_load::Memory&, Word self, Word owner, Word shared);
struct ParentPorts {
    virtual ~ParentPorts() = default;
    virtual Word item_by_id_004ae760(Word server, Word id) = 0;                       // CServerExoApp::GetItemByGameObjectID
    virtual void set_possessor_00553210(Word item, Word owner, Word a, Word b, Word c) = 0;  // CSWSItem::SetPossessor
};
// 00556150 (RET 4): [self] := id; for each item id in [+0C][0..+10): item->SetPossessor([self], 0, 0, 7F000000h).
// The owner is re-read from [self] on every iteration (original).
void set_parent_id_00556150(kotor::character_load::Memory&, ParentPorts&, Word self, Word id);
}
