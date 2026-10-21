// stale-stubs lane G3 "ports": original bodies that port adapters in 1060 / 1025 / 1001 trapped on
// ("required(addr)") because no callable provider existed.
//   00507220 CSWSArea::IncreaseAILevelPriority()                         thiscall, RET 0
//   00554080 CSWSItem::GetMinEquipLevel()                                thiscall, RET 0, BYTE in AL
//   0055F0F0 CSWSItem::ReadContainerItemsFromGff(CResGFF*, CResStruct*)  thiscall, RET 8
// Transcribed from the original swkotor.exe (read-only); addresses in the comments are original instructions.
// Every callee is an existing linked provider (named beside its declaration). No traps, no invented values.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                                   // 006FAE8C __ftol2 [w2_ftol_native]

extern "C" {
// ---------------- imports: existing providers (c17 candidate map; object in brackets) ----------------
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                       // GetServerAIMaster [creature_destructor_r2]
U __fastcall recovered_ai_level_004b08a0(void* ai, void*, void* object, U level);                // CServerAIMaster::SetAILevel [1070]
int __fastcall async_string_length_005e5790(void* string, void*);                                // CExoString::GetLength [0188]
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                      // CSWItem::GetBaseItem [0820]
U __fastcall gff_list_004118c0(void* gff, void*, void* list, void* s, const char* label);         // CResGFF::GetList [0291]
U __fastcall gff_list_count_00411940(void* gff, void*, void* list);                              // CResGFF::GetListCount [0291]
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index);         // CResGFF::GetListElement [0291]
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* s, const char* label, U* ok, U d); // ReadFieldDWORD [0292]
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* s, const char* label, U* ok,
                                                    unsigned char d);                          // ReadFieldBYTE [0292]
void* __fastcall recovered_item_construct_005530a0(void* item, void*, U id);                     // CSWSItem::CSWSItem [0963]
U __fastcall recovered_load_item_00560970(void* item, void*, void* gff, void* s);                // CSWSItem::LoadItem [w2_party_native]
U __fastcall recovered_inventory_insert_0055d330(void* repository, void*, void** item, U a1, U a2, U a3); // CItemRepository::AddItem [1088]
void __fastcall recovered_item_owner_00553210(void* item, void*, U possessor, U a1, U a2, U a3); // CSWSItem::SetPossessor [1013]
// ---------------- exports ----------------
void __fastcall kotor_fastcall_00507220(void* area, void*);
unsigned char __fastcall kotor_fastcall_00554080(void* item, void*);
void __fastcall kotor_fastcall_0055f0f0(void* item, void*, void* gff, void* s);
}

namespace {
constexpr unsigned char k_lookup_ok_0074666c = 0;            // .rdata 0074666C (byte)
constexpr unsigned char k_object_type_bound_00746633 = 4;    // .rdata 00746633 (byte)
const char k_max_single_item_value_00748da4[] = "MAXSINGLEITEMVALUE";   // .rdata 00748DA4
const char k_item_list_00747210[] = "ItemList";              // .rdata 00747210
const char k_object_id_00744c24[] = "ObjectId";              // .rdata 00744C24
const char k_repos_pos_x_00749048[] = "Repos_PosX";          // .rdata 00749048
const char k_repos_pos_y_0074903c[] = "Repos_Posy";          // .rdata 0074903C
struct GffStruct { U index; };                               // CResStruct (4 bytes)
struct GffList { U words[5]; };                              // CResList (struct handle + 16-byte label)
static_assert(sizeof(GffList) == 20, "CResList");
}

// =========================================================================================================
// 00507220 CSWSArea::IncreaseAILevelPriority: every object of the area above the object-type bound whose AI
// level (+78) is 0 is raised to level 1 when it is a creature, AoE or trigger; a door only with a nonempty
// +250 string and a zero +3C0; a placeable only with a zero +398 (both re-fetch the cast, as the original).
void __fastcall kotor_fastcall_00507220(void* area, void*) {
    void* array = object_array();                                                   // 00507223..2F (inline 004AED70)
    void* ai = recovered_server_ai_master_004aed80(server_app(), nullptr);          // 00507234..46 (EBX)
    for (I i = 0; i < at<I>(area, 0x194); ++i) {                                    // 00507248..56, 0050731A..2B (re-read)
        void* object = nullptr;                                                     // [esp+10]
        if (static_cast<unsigned char>(kotor_fastcall_004d8230(array, nullptr, at<U*>(area, 0x190)[i], &object))
            != k_lookup_ok_0074666c) continue;                                      // 00507260..7E
        if (at<unsigned char>(object, 8) <= k_object_type_bound_00746633) continue; // 00507284..91 (jbe)
        if (at<U>(object, 0x78) != 0) continue;                                     // 00507297..9E
        bool raise;
        if (vcall<void*>(object, 0x30) != nullptr) raise = true;                    // 005072A0..A7 AsSWSCreature
        else if (vcall<void*>(object, 0x50) != nullptr) raise = true;               // 005072A9..B4 AsSWSAreaOfEffectObject
        else if (vcall<void*>(object, 0x40) != nullptr) raise = true;               // 005072B6..C1 AsSWSTrigger
        else if (vcall<void*>(object, 0x18) != nullptr) {                           // 005072C3..D2 AsSWSDoor
            void* door = vcall<void*>(object, 0x18);                                // 005072D4..D9 (second live call)
            if (async_string_length_005e5790(static_cast<unsigned char*>(door) + 0x250, nullptr) <= 0) continue; // 005072DB..E8
            raise = at<U>(door, 0x3c0) == 0;                                        // 005072EA..F2, 0050730E
        } else {
            if (vcall<void*>(object, 0x48) == nullptr) continue;                    // 005072F4..FB AsSWSPlaceable
            raise = at<U>(vcall<void*>(object, 0x48), 0x398) == 0;                  // 005072FD..0E (second live call)
        }
        if (raise) recovered_ai_level_004b08a0(ai, nullptr, object, 1);             // 00507310..15
    }
}

// =========================================================================================================
// 00554080 CSWSItem::GetMinEquipLevel: first row of the rules +B8 -> +114 table (itemlevel 2DA) whose
// MAXSINGLEITEMVALUE is at least max(1, ftol(base cost multiplier * item cost)); plot items (+108 == 1) stop at
// the current row. Row index + 1 is returned in AL (REVIEW-STALE-COMBAT fix 1). Quirk kept: the INT out value is not initialized before
// C2DA::GetINTEntry, so a missing cell compares against the previous row's value (an indeterminate on row 0).
unsigned char __fastcall kotor_fastcall_00554080(void* item, void*) {
    void* table = at<void*>(at<void*>(kotor_cell_rules_007a3a28, 0xb8), 0x114);    // 0055409E..AC (EDI)
    I value;                                                                        // [esp+0C] (not initialized)
    I row = 0;                                                                      // 005540B5 (EBX)
    if (at<I>(table, 0x24) > 0) {                                                   // 005540B2..BB (jle)
        do {
            alignas(4) unsigned char column[8];                                     // [esp+10] CExoString
            frame_LogStringCtor_005e5a90(column, nullptr, k_max_single_item_value_00748da4); // 005540C1..CA
            c2da_current_get_int_00413660(table, nullptr, static_cast<U>(row), column, &value); // 005540CF..E4
            frame_LogStringDtor_005e5c20(column, nullptr);                          // 005540E9..F5
            if (at<U>(item, 0x108) == 1) break;                                     // 005540FA..01
            void* base = kotor_m15b_item_get_base_item_005b4790(item, nullptr);     // 00554103..05
            U cost = w2::ftol2_006fae8c(static_cast<double>(at<float>(base, 0xc4)) *
                                        static_cast<double>(at<I>(item, 0x260)));   // 0055410A..16 (fld / fimul / __ftol2)
            if (cost > 1) {                                                         // 0055411B..1E (jbe)
                void* again = kotor_m15b_item_get_base_item_005b4790(item, nullptr);    // 00554120..22 (recomputed)
                cost = w2::ftol2_006fae8c(static_cast<double>(at<float>(again, 0xc4)) *
                                          static_cast<double>(at<I>(item, 0x260))); // 00554127..33
            } else {
                cost = 1;                                                           // 0055413A
            }
            if (cost <= static_cast<U>(value)) break;                               // 0055413F..43 (jbe)
            ++row;                                                                  // 00554148
        } while (row < at<I>(table, 0x24));                                         // 00554145..4B (re-read, jl)
    }
    return static_cast<unsigned char>(row + 1);                                     // 00554156..59 (mov al, bl; inc al)
}

// =========================================================================================================
// 0055F0F0 CSWSItem::ReadContainerItemsFromGff(gff, struct): for a container item (+26C repository), every
// ItemList element is constructed with its ObjectId, loaded with CSWSItem::LoadItem (a failed load deletes it
// through the scalar deleting destructor), the Repos_PosX / Repos_Posy bytes are read and discarded as in the
// original, the item is added to the repository and, when AddItem succeeds and leaves the item pointer set,
// its possessor becomes this container (+14 id).
void __fastcall kotor_fastcall_0055f0f0(void* self, void*, void* gff, void* s) {
    if (at<void*>(self, 0x26c) == nullptr) return;                                   // 0055F10B..13
    GffList list;                                                                   // [esp+1C]
    if (gff_list_004118c0(gff, nullptr, &list, s, k_item_list_00747210) != 1) return; // 0055F119..38
    if (gff_list_count_00411940(gff, nullptr, &list) == 0) return;                  // 0055F13E..4E (jbe)
    U index = 0;                                                                    // 0055F145 (EBP)
    do {
        GffStruct element;                                                          // [esp+44]
        (void)gff_list_element_00411990(gff, nullptr, &element, &list, index);      // 0055F155..62
        U ok;                                                                       // [esp+14]
        const U id = gui_load_gff_dword_00411c20(gff, nullptr, &element, k_object_id_00744c24, &ok,
                                                 0x7f000000u);                      // 0055F167..7D (EDI)
        void* memory = live_operator_new(0x29c);                                    // 0055F182..8E
        void* item = memory != nullptr ? recovered_item_construct_005530a0(memory, nullptr, id) : nullptr; // 0055F195..AD
        if (item != nullptr) {                                                      // 0055F1AF..BD; [esp+3C] = item
            if (recovered_load_item_00560970(item, nullptr, gff, &element) == 0) {  // 0055F1C3..D2
                vcall<void>(item, 0x0, 1u);                                         // 0055F1D4..DA scalar deleting dtor(1)
            } else {
                U pos_ok;                                                           // [esp+10]
                (void)gui_load_gff_byte_00411a60(gff, nullptr, &element, k_repos_pos_x_00749048, &pos_ok, 0xff); // 0055F1DE..F4
                (void)gui_load_gff_byte_00411a60(gff, nullptr, &element, k_repos_pos_y_0074903c, &pos_ok, 0xff); // 0055F1F9..0F
                if (recovered_inventory_insert_0055d330(at<void*>(self, 0x26c), nullptr, &item, 1, 0, 1) != 0 // 0055F214..2C
                    && item != nullptr)                                             // 0055F22E..34
                    recovered_item_owner_00553210(item, nullptr, at<U>(self, 0x14), 1, 0, 0x7f000000u); // 0055F236..43
            }
        }
        ++index;                                                                    // 0055F24F
    } while (index < gff_list_count_00411940(gff, nullptr, &list));                 // 0055F248..57 (jb, re-read)
}
