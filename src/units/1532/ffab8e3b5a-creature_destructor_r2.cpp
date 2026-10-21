// Case54 round-2 destructor batch (lane R2-B).
//
// Reconstructed original bodies:
//   004FDE20  CSWSCreature scalar deleting destructor     (RET 4, returns this)
//   004F8410  CSWSCreature::~CSWSCreature                 (0x360, EH 0071B584, 10 unwind states)
//   005ACF40  CSWSCreatureStats::~CSWSCreatureStats       (0x1A0, EH 0072201E, 10 unwind states)
//   005AA8C0  CSWSCreatureStats::ClearFeats
//   005CB270  CSWLevelStats::~CSWLevelStats               (EH 00723AE4, 3 unwind states)
//   005CFD90  CPathfindInformation::Shutdown thunk (jmp 005CEDE0)
//   005CEDE0  CPathfindInformation::Shutdown body
//   004D5E60  CSWSCombatRound::~CSWSCombatRound           (0x110, EH 0071A1C5, 3 unwind states)
//   005BF920  CSWSFaction::RemoveMember                   (0x110, EH 00722E4B, 1 unwind state)
//   0052B370  CFactionManager::DeleteFaction
//   005BE8A0  CSWSFaction::~CSWSFaction                   (required by 0052B370)
//   005BE830  CSWSFaction::Initialise                     (required by 005BF750)
//   005BE960  CSWSFaction::SendFactionUpdateList
//   005BEA20  CSWSFaction::SendFactionUpdateRemove
//   005BF750  CSWSFaction::CSWSFaction(unsigned)
//   004D4480  CSWSCombatAttackData::~CSWSCombatAttackData (array element dtor of 004D5E60)
//   005A76E0  CSWSCreatureStats_ClassInfo::~...           (array element dtor of 005ACF40)
//   004AED80  CServerExoApp::GetServerAIMaster accessor
//   004AEDB0  CServerExoApp::GetSWSMessage accessor
//   004D0210  CSWSCreature::GetPortraitId (creature_table[52], offset 0xD0)
//   004F8370  CSWSCreature::GetPortrait   (creature_table[50], offset 0xC8)
//
// The 004F8410 vtable store uses the selected 0963 creature vtable (the same
// table the constructor publishes through Creature::vtable()); it is exported
// by patch 0002-0963-table-r2.  It is not the raw original address 0x7470C8.
//
// Original disassembly is in evidence/original-disasm.txt, evidence/attackdata.txt,
// evidence/cstring.txt and evidence/eh.txt.  All frees use the single game heap
// (_live_free / live_operator_new); all callees are selected providers from
// candidate1559d.map except the bodies implemented in this file.

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

extern "C" {
void __cdecl live_free(void*);                       // _live_free 006139F0
void* __cdecl live_operator_new(std::uint32_t);      // _live_operator_new 00613B50

// Single game-heap global operator new/delete are the selected 0004a objects.
// (The new-expression in CSWSFaction::RemoveMember needs the compiler cleanup
// state; the allocation still reaches the game heap.)

// Selected providers (candidate1559d.map).  Their callable contracts are the
// declarations below, verified against the provider objects and their source.
void* __fastcall recovered_object_area_004cb120(void*, void*);                  // 004D5700 @8
void* __fastcall recovered_ai_remove_004af3d0(void*, void*, void*);             // 004D79D0 @12
void* __fastcall server_get_module_004ae6b0(void*, void*);                      // 006B9C00 @8
void __fastcall recovered_module_remove_tag_004c5ef0(void*, void*, void*, unsigned); // 004CBEC0 @16
void __fastcall recovered_object_destroy_004d0220(void*, void*);                // 005933B0 @8
void* __fastcall kotor_fastcall_00555de0(void*, void*);                         // 00705320 @8
void* __fastcall kotor_fastcall_005c58f0(void*, void*);                         // 007053B0 @8
void* __fastcall w2x_inventory_item_005560b0(void*, void*, unsigned);           // 0056B5E0 @12
void* __fastcall recovered_equipment_item_005a4c20(void*, void*, unsigned);     // 004CE880 @12
void* __fastcall combat_reuse_faction_00513fc0(void*, void*);                   // 00460380 @8
unsigned __fastcall recovered_faction_is_npc_0052b280(void*, void*, unsigned);  // 0045F280 @12
unsigned __fastcall recovered_int_list_remove_0066dcc0(void*, void*, unsigned);// 0045BC60 @12
void* __fastcall kotor_fastcall_004ae770(void*, void*, unsigned);               // 00472830 @12
void* __fastcall kotor_fastcall_004ae910(void*, void*, unsigned);               // (server lookup)
void __fastcall exo_array_list_destruct_0056b6f0(void*, void*);                 // 00742A40 @8
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);                // 00535440 @8
void __fastcall w5_pathfind_delete_first_tile_alternatives_005ceb70(void*, void*); // 005291F0 @8
void __fastcall kotor_m15b_combat_information_destruct_00550e00(void*, void*);  // 00483940 @8
void __fastcall exo_locstring_destroy_005ea000(void*, void*);                   // 0062A7F0 @8
void __stdcall c2da_destroy_array_006fa852(void*, unsigned, unsigned,
                                           void (__fastcall*)(void*, void*));   // 005CE110 @16
unsigned __fastcall recovered_party_list_00567f90(void*, void*, unsigned,
                                                   unsigned, const void*,
                                                   unsigned, unsigned);         // 0040A560 @28

// 004AED80 GetServerAIMaster and 004AEDB0 GetSWSMessage are ten-byte accessors
// (not the port methods that carry the same original address in the map).
void* __fastcall recovered_server_ai_master_004aed80(void*, void*);
void* __fastcall recovered_server_sws_message_004aedb0(void*, void*);
}

extern "C" {
void __fastcall recovered_pathfind_shutdown_005cfd90(void*, void*);
void __fastcall recovered_pathfind_shutdown_005cede0(void*, void*);
}

namespace kotor_movie {
void __fastcall string_element_destroy_005e5c20(void*, void*);                  // 005C6E00 @8
}
namespace kotor_list { struct Internal; }
namespace kotor_reconstruction { struct ArrayMutations; }
namespace base_ini_reconstruction {
// Exact selected signatures (struct-pointer parameters, not void*).
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);            // 0061E050
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);            // 0061E030
}
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, unsigned); // 0061E670
}
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

// Exported by patch 0002-0963-table-r2 in the selected 0963 source.  It is the
// address of that TU's `creature_table`, the same object slot 0 is rebound to
// recovered_creature_scalar_delete_004fde20.
extern "C" const void* const* const kotor_recovered_creature_vtable_004f8410;

// 00406D80 (selected provider) constructs a CResRef from a literal.  The
// original 004F8370 empty branch passes the empty string at 0x73D71C; the
// literal is recovered as "" (the original bytes are 00 00 00 00).
namespace menu_resources {
struct Ref {
    Ref* construct_00406d80(const char*);
};
}

namespace case54_r2 {

using U = std::uint32_t;

// Direct little-endian 32-bit accessors; macros keep the reconstructed body
// free of helper frames under the owner's /Ob0 recipe.
#define ptr(a) (reinterpret_cast<void*>(static_cast<std::uintptr_t>(a)))
#define word(a) (static_cast<U>(reinterpret_cast<std::uintptr_t>(a)))
#define rd(a) (*reinterpret_cast<const U*>(ptr(a)))
#define wr(a, v) (*reinterpret_cast<U*>(ptr(a)) = static_cast<U>(v))

#define server_app() (ptr(rd(word(kotor_menu_window_cells::manager_007a39fc) + 8)))
#define server_internal() (ptr(rd(word(server_app()) + 4)))
#define sws_message() (recovered_server_sws_message_004aedb0(server_app(), nullptr))
#define ai_master() (recovered_server_ai_master_004aed80(server_app(), nullptr))

// Exact original accessor bodies (004AED80..89 and 004AEDB0..B9).
extern "C" void* __fastcall recovered_server_ai_master_004aed80(void* server, void*) {
    // 004AED80..89: mov eax,[ecx+4]; mov eax,[eax+0x10044]; ret
    void* internal = *reinterpret_cast<void**>(static_cast<unsigned char*>(server) + 4);
    return *reinterpret_cast<void**>(static_cast<unsigned char*>(internal) + 0x10044);
}
extern "C" void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*) {
    // 004AEDB0..B9: mov eax,[ecx+4]; mov eax,[eax+0x10010]; ret
    void* internal = *reinterpret_cast<void**>(static_cast<unsigned char*>(server) + 4);
    return *reinterpret_cast<void**>(static_cast<unsigned char*>(internal) + 0x10010);
}

// 004D0210 CSWSCreature::GetPortraitId: mov ax, word ptr [ecx+0x30]; ret.
// The caller masks the result with 0xffff.  The original writes AX only; the
// zero-extended unsigned short return is the same value at every call site.
extern "C" unsigned __fastcall recovered_creature_portrait_004d0210(void* self, void*) {
    return *reinterpret_cast<unsigned short*>(
        static_cast<unsigned char*>(self) + 0x30);
}


// 004F8370 CSWSCreature::GetPortrait (vtable slot 0xC8, creature_table[50]):
//   mov eax,[ecx+0xa74]; if stats: copy 16 bytes stats+0x16E -> out;
//   else Ref::construct(out, ""); return out in EAX; ret 4.
// __fastcall(self, unused, out): ECX=self, EDX unused, out on the stack,
// callee pops 4, returns out.  This is the original thiscall call-site ABI.
extern "C" void* __fastcall recovered_creature_portrait_ref_004f8370(
        void* self, void*, void* out) {
    void* stats = *reinterpret_cast<void**>(
        static_cast<unsigned char*>(self) + 0xa74);
    if (stats) {
        std::memcpy(out, static_cast<unsigned char*>(stats) + 0x16e, 16);
    } else {
        reinterpret_cast<::menu_resources::Ref*>(out)->construct_00406d80("");
    }
    return out;
}

// Original 004F8470: mov edx,[eax]; push 1; mov ecx,eax; call [edx].
// Two levels: [object] is the vtable pointer, [vtable] is slot 0.
using VSlot0 = void* (__thiscall*)(void*, unsigned);
#define vdelete_slot0(object) \
    (reinterpret_cast<VSlot0>(*reinterpret_cast<void**>(*reinterpret_cast<void**>(object)))(object, 1u))

// ---------------------------------------------------------------- containers
// Original layouts: CExoString { char* data; unsigned length; } (8 bytes);
// CExoArrayList { void* data; unsigned count; unsigned capacity; } (12 bytes).
struct alignas(4) CExoString {
    void* data;
    unsigned length;
    CExoString() : data(nullptr), length(0) {}
    ~CExoString() noexcept(false) {
        // 005E5C20: free(data); length = 0; data = 0.
        kotor_movie::string_element_destroy_005e5c20(this, nullptr);
    }
};

struct alignas(4) CExoArrayList {
    void* data;
    unsigned count;
    unsigned capacity;
    CExoArrayList() : data(nullptr), count(0), capacity(0) {}
    ~CExoArrayList() noexcept(false) {
        // 0056B6F0: free(data); data = 0.
        exo_array_list_destruct_0056b6f0(this, nullptr);
    }
};

struct alignas(4) CExoLocString {
    void* data;
    unsigned length;
    CExoLocString() : data(nullptr), length(0) {}
    ~CExoLocString() noexcept(false) {
        // 005EA000: free(data); data = 0; length = 0.
        exo_locstring_destroy_005ea000(this, nullptr);
    }
};

// Array wrappers whose destructors are the original c2da_destroy_array sites.
// They are real subobjects so the compiler emits the same unwind-state order,
// and their destructors reach the selected 006FA852 provider directly.
struct alignas(4) StringArray14 {                 // 14 * 8-byte CExoString
    unsigned char raw[0x70];
    ~StringArray14() noexcept(false) {
        c2da_destroy_array_006fa852(raw, 8, 14,
            &kotor_movie::string_element_destroy_005e5c20);
    }
};

struct alignas(4) NoopArray10 {                   // 10 * 0x1c-byte RET-only objects
    unsigned char raw[0x118];
    ~NoopArray10() noexcept(false) {
        c2da_destroy_array_006fa852(raw, 0x1c, 10,
            &kotor_m15e_slot090_void0_004015a0);
    }
};

// 004D4480 CSWSCombatAttackData::~CSWSCombatAttackData (array element of 004D5E60).
extern "C" void __fastcall recovered_attack_data_destruct_004d4480(void* self, void*) {
    unsigned char* b = static_cast<unsigned char*>(self);
    if (U p = rd(word(b) + 0xac)) {                                   // 004D449E..4D44D3
        U count = rd(p - 4);                                          // 004D44B2
        c2da_destroy_array_006fa852(ptr(p), 0xd0, count,
            &kotor_m15e_slot090_void0_004015a0);                      // 004D44B9..4D44C5
        live_free(ptr(p - 4));                                        // 004D44CA
        wr(word(b) + 0xac, 0);                                        // 004D44D4
    }
    if (U p = rd(word(b) + 0xa0)) { live_free(ptr(p)); wr(word(b) + 0xa0, 0); } // 004D44DA
    if (U p = rd(word(b) + 0x94)) { live_free(ptr(p)); wr(word(b) + 0x94, 0); } // 004D44F1
    if (U p = rd(word(b) + 0x88)) { live_free(ptr(p)); wr(word(b) + 0x88, 0); } // 004D4508
    kotor_movie::string_element_destroy_005e5c20(b + 0x80, nullptr);            // 004D452C
    kotor_movie::string_element_destroy_005e5c20(b + 0x78, nullptr);            // 004D4531
}

// 005A76E0 CSWSCreatureStats_ClassInfo::~CSWSCreatureStats_ClassInfo
// (array element of 005ACF40).
extern "C" void __fastcall recovered_class_info_destruct_005a76e0(void* self, void*) {
    unsigned char* b = static_cast<unsigned char*>(self);
    if (U p = rd(word(b))) {                                           // 005A76FD..5A771A
        live_free(ptr(p));
        wr(word(b), 0);
        wr(word(b) + 8, 0);
    }
    wr(word(b) + 4, 0);                                                // 005A772E
    c2da_destroy_array_006fa852(b + 0x0c, 0x0c, 1,
        &exo_array_list_destruct_0056b6f0);                            // 005A7721..5A773A
    c2da_destroy_array_006fa852(b, 0x0c, 1,
        &exo_array_list_destruct_0056b6f0);                            // 005A773F..5A7751
}

struct alignas(4) AttackDataArray7 {              // 7 * 0x14c-byte attack data
    unsigned char raw[0x914];
    ~AttackDataArray7() noexcept(false) {
        c2da_destroy_array_006fa852(raw, 0x14c, 7,
            &recovered_attack_data_destruct_004d4480);
    }
};

struct alignas(4) ClassInfoArray2 {               // 2 * 0x28-byte class info
    unsigned char raw[0x50];
    ~ClassInfoArray2() noexcept(false) {
        c2da_destroy_array_006fa852(raw, 0x28, 2,
            &recovered_class_info_destruct_005a76e0);
    }
};

// ------------------------------------------------------------------- objects
struct CSWSObjectBase {
    ~CSWSObjectBase() noexcept(false) {
        recovered_object_destroy_004d0220(this, nullptr);              // 004D0220
    }
};

struct CSWSCreatureStats;

struct CSWSCreature {
    CSWSObjectBase m_base;                        // +0x000 (unwind state 0)
    unsigned char  m_pad0[0x22f];                 // +0x001..0x230
    StringArray14  m_230;                         // +0x230 (state 1)
    StringArray14  m_2a0;                         // +0x2a0 (state 2)
    unsigned char  m_pad2[0x14];                  // 0x310..0x324
    CExoArrayList  m_324;                         // +0x324 (state 3)
    CExoArrayList  m_330;                         // +0x330 (state 4)
    unsigned char  m_pad3[0x6c];                  // 0x33c..0x3a8
    NoopArray10    m_3a8;                         // +0x3a8 (state 5)
    void*          m_4c0;                         // +0x4c0 plain
    unsigned char  m_pad4a[0x04];                 // 0x4c4..0x4c8
    CExoString     m_4c8;                         // +0x4c8 (state 6)
    unsigned char  m_pad4[0x424];                 // 0x4d0..0x8f4
    CExoArrayList  m_8f4;                         // +0x8f4 (state 7)
    unsigned char  m_pad5[0x20];                  // 0x900..0x920
    CExoArrayList  m_920;                         // +0x920 (state 8)
    unsigned char  m_pad6[0x24];                  // 0x92c..0x950
    CExoArrayList  m_950;                         // +0x950 (state 9)
    unsigned char  m_tail[0xac8 - 0x95c];         // rest of the object
    ~CSWSCreature() noexcept(false);
    void* scalar_delete(unsigned flags);          // 004FDE20, __thiscall RET 4
};

static_assert(offsetof(CSWSCreature, m_230) == 0x230, "layout");
static_assert(offsetof(CSWSCreature, m_2a0) == 0x2a0, "layout");
static_assert(offsetof(CSWSCreature, m_324) == 0x324, "layout");
static_assert(offsetof(CSWSCreature, m_330) == 0x330, "layout");
static_assert(offsetof(CSWSCreature, m_3a8) == 0x3a8, "layout");
static_assert(offsetof(CSWSCreature, m_4c8) == 0x4c8, "layout");
static_assert(offsetof(CSWSCreature, m_8f4) == 0x8f4, "layout");
static_assert(offsetof(CSWSCreature, m_920) == 0x920, "layout");
static_assert(offsetof(CSWSCreature, m_950) == 0x950, "layout");

struct CSWLevelStats {
    // 005CB270: CExoArrayList[1] at +0, CExoArrayList[1] at +0x0c,
    // CExoArrayList at +0x18, plain pointer at +0x24.  EH 00723AE4 states 0/1
    // destroy the two one-element arrays (006FA852) and state 2 the +0x18 list.
    struct alignas(4) ArrayOne {
        unsigned char raw[0x0c];
        ~ArrayOne() noexcept(false) {
            c2da_destroy_array_006fa852(raw, 0x0c, 1,
                &exo_array_list_destruct_0056b6f0);
        }
    };
    ArrayOne      m_0;                            // +0x00 (state 0)
    ArrayOne      m_c;                            // +0x0c (state 1)
    CExoArrayList m_18;                           // +0x18 (state 2)
    void*         m_24;                           // +0x24
    ~CSWLevelStats() noexcept(false);
};

static_assert(offsetof(CSWLevelStats, m_c) == 0x0c, "layout");
static_assert(offsetof(CSWLevelStats, m_18) == 0x18, "layout");
static_assert(offsetof(CSWLevelStats, m_24) == 0x24, "layout");

struct CSWSCreatureStats {
    // 005ACF40 member order (ascending offset == reverse destruction order):
    CExoArrayList  m_0;                           // +0x000 (state 0)
    CExoArrayList  m_c;                           // +0x00c (state 1)
    CExoArrayList  m_18;                          // +0x018 (state 2)
    void*          m_24;                          // +0x024 plain
    CExoArrayList  m_28;                          // +0x028 (state 3)
    CExoLocString  m_34;                          // +0x034 (state 4)
    CExoLocString  m_3c;                          // +0x03c (state 5)
    unsigned char  m_pad0[0x14];                  // 0x044..0x058
    CExoLocString  m_58;                          // +0x058 (state 6)
    unsigned char  m_pad1[0x2c];                  // 0x060..0x08c
    ClassInfoArray2 m_8c;                         // +0x08c (state 7)
    unsigned char  m_pad2[0x04];                  // 0x0dc..0x0e0
    CExoString     m_e0;                          // +0x0e0 (state 8)
    unsigned char  m_pad3[0xbc];                  // 0x0e8..0x1a4
    CExoString     m_1a4;                         // +0x1a4 (state 9)
    void ClearFeats();
    ~CSWSCreatureStats() noexcept(false);
};

static_assert(offsetof(CSWSCreatureStats, m_c) == 0x00c, "layout");
static_assert(offsetof(CSWSCreatureStats, m_18) == 0x018, "layout");
static_assert(offsetof(CSWSCreatureStats, m_24) == 0x024, "layout");
static_assert(offsetof(CSWSCreatureStats, m_28) == 0x028, "layout");
static_assert(offsetof(CSWSCreatureStats, m_34) == 0x034, "layout");
static_assert(offsetof(CSWSCreatureStats, m_3c) == 0x03c, "layout");
static_assert(offsetof(CSWSCreatureStats, m_58) == 0x058, "layout");
static_assert(offsetof(CSWSCreatureStats, m_8c) == 0x08c, "layout");
static_assert(offsetof(CSWSCreatureStats, m_e0) == 0x0e0, "layout");
static_assert(offsetof(CSWSCreatureStats, m_1a4) == 0x1a4, "layout");

struct CSWSCombatRound {
    unsigned       m_pad0;                        // +0x000
    AttackDataArray7 m_4;                         // +0x004 (state 0)
    CExoArrayList  m_918;                         // +0x918 (state 1)
    CExoArrayList  m_924;                         // +0x924 (state 2)
    unsigned char  m_tail[0x9cc - 0x930];
    ~CSWSCombatRound() noexcept(false);
};

static_assert(offsetof(CSWSCombatRound, m_4) == 0x004, "layout");
static_assert(offsetof(CSWSCombatRound, m_918) == 0x918, "layout");
static_assert(offsetof(CSWSCombatRound, m_924) == 0x924, "layout");

struct CSWSCreature;  // fwd (stats uses the creature layout only through offsets)

struct CSWSFaction {
    CExoArrayList m_members;                      // +0x00 (state 0)
    unsigned      m_index;                        // +0x0c
    unsigned      m_10;                           // +0x10
    unsigned      m_14;                           // +0x14
    CExoString    m_name;                         // +0x18 (state 1)
    unsigned      m_20;                           // +0x20
    CSWSFaction(unsigned id);
    ~CSWSFaction() noexcept(false);
    void Initialise();
    void RemoveMember(unsigned id);
    void SendFactionUpdateList();
    void SendFactionUpdateRemove(unsigned id);
};

static_assert(offsetof(CSWSFaction, m_index) == 0x0c, "layout");
static_assert(offsetof(CSWSFaction, m_name) == 0x18, "layout");
static_assert(offsetof(CSWSFaction, m_20) == 0x20, "layout");

struct CFactionManager {
    unsigned char m_pad[0x24];
    CExoArrayList* m_factions;                    // +0x24: pointer to a list
    void DeleteFaction(unsigned index);
};

// 005BF98D: raw allocation is 006FA7E6 operator new(0x24); the original
// unwind state frees the raw block with _free if the constructor throws.
// live_operator_new is the selected wrapper for 006FA7E6 and live_free is
// the single game heap, so this guard reproduces that cleanup in the same
// order without a second heap entry.
struct FactionNewGuard {
    void* p;
    ~FactionNewGuard() noexcept(false) { if (p) live_free(p); }
};

static_assert(offsetof(CFactionManager, m_factions) == 0x24, "layout");

}  // namespace case54_r2

namespace case54_r2 {

// ------------------------------------------------------------- 004F8410 body
CSWSCreature::~CSWSCreature() noexcept(false) {
    // 004F842F: primary vtable store, before GetArea and any member destruction.
    // Selected reconstructed table (0963 `creature_table`), not raw 0x7470C8.
    *reinterpret_cast<const void* const**>(static_cast<void*>(this)) =
        kotor_recovered_creature_vtable_004f8410;

    unsigned char* b = reinterpret_cast<unsigned char*>(this);

    // 004F843D: GetArea().  The original then calls RemoveFromArea(0) at
    // 004F844B when GetArea() != 0.  That branch is a cold-branch scope gate
    // for this batch: the accepted temporary AddNPC creature has
    // GetArea()==0 (no object id 0x7F000000), so omitting the call is not a
    // reached-call skip.  The case54 acceptance asserts GetArea()==0 before
    // claiming the destructor path.  Do not add a trap/stub/weak fallback for
    // 004F0FD0 here; an owner body is a separate scoped task (OPEN.md).
    (void)recovered_object_area_004cb120(this, nullptr);

    // 004F8450..004F8479: repository items, highest index first, slot-0 delete
    // with flags 1.  The original has no null check on the repository.
    {
        void* repository = ptr(rd(word(b) + 0xa30));
        int count = static_cast<int>(rd(word(repository) + 0x10));
        for (int i = count - 1; i >= 0; --i) {
            void* item = w2x_inventory_item_005560b0(repository, nullptr,
                                                     static_cast<unsigned>(i));
            if (item) vdelete_slot0(item);
        }
    }

    // 004F8480..004F84A5: equipped items, slots 0..0x11, bit 1<<i.
    {
        void* inventory = ptr(rd(word(b) + 0xa2c));
        for (unsigned i = 0; i < 0x12; ++i) {
            void* item = recovered_equipment_item_005a4c20(inventory, nullptr, 1u << i);
            if (item) vdelete_slot0(item);
        }
    }

    // 004F84A7..004F84CB: faction removal and stats faction id = -1.
    {
        CSWSFaction* faction =
            static_cast<CSWSFaction*>(combat_reuse_faction_00513fc0(this, nullptr));
        if (faction && rd(word(b) + 0x9d4) == 0) {
            faction->RemoveMember(rd(word(b) + 4));
            CSWSCreatureStats* stats =
                static_cast<CSWSCreatureStats*>(ptr(rd(word(b) + 0xa74)));
            wr(word(stats) + 0x78, 0xffffffffu);
        }
    }

    // 004F84D2..004F84EC: ~CSWSCreatureStats then free.
    if (CSWSCreatureStats* stats =
            static_cast<CSWSCreatureStats*>(ptr(rd(word(b) + 0xa74)))) {
        stats->~CSWSCreatureStats();
        live_free(stats);
        wr(word(b) + 0xa74, 0);
    }

    // 004F84F2..004F8502: GetServerAIMaster()->RemoveObject(this).
    {
        void* master = ai_master();
        recovered_ai_remove_004af3d0(master, nullptr, this);
    }

    // 004F8507..004F8523: GetModule()->RemoveObjectFromLookupTable(tag, id).
    {
        void* module = server_get_module_004ae6b0(server_app(), nullptr);
        if (module) {
            recovered_module_remove_tag_004c5ef0(module, nullptr, b + 0x18,
                                                 rd(word(b) + 4));
        }
    }

    // 004F8528..004F8542: pathfind information.
    if (U p = rd(word(b) + 0x340)) {
        recovered_pathfind_shutdown_005cfd90(ptr(p), nullptr);
        live_free(ptr(p));
        wr(word(b) + 0x340, 0);
    }

    // 004F8548..004F8562: combat round.
    if (CSWSCombatRound* round =
            static_cast<CSWSCombatRound*>(ptr(rd(word(b) + 0x9c8)))) {
        round->~CSWSCombatRound();
        live_free(round);
        wr(word(b) + 0x9c8, 0);
    }

    // 004F8568..004F8582: item repository.
    if (U p = rd(word(b) + 0xa30)) {
        kotor_fastcall_00555de0(ptr(p), nullptr);
        live_free(ptr(p));
        wr(word(b) + 0xa30, 0);
    }

    // 004F8588..004F8598: inventory object, slot-0 delete with flags 1.
    if (void* inventory = ptr(rd(word(b) + 0xa2c))) {
        vdelete_slot0(inventory);
        wr(word(b) + 0xa2c, 0);
    }

    // 004F859E..004F85BB: two-level block at +0xa20.
    if (U p = rd(word(b) + 0xa20)) {
        live_free(ptr(rd(p)));
        live_free(ptr(p));
        wr(word(b) + 0xa20, 0);
    }

    // 004F85C1..004F861A: remove and free +0x920[0..+0x924), high index first.
    {
        int edi = static_cast<int>(rd(word(b) + 0x924));
        if (edi > 0) {
            --edi;
            if (edi >= 0) {
                do {
                    U data = rd(word(b) + 0x920);
                    U element = rd(data + static_cast<U>(edi) * 4u);
                    U count = rd(word(b) + 0x924);
                    --count;
                    wr(word(b) + 0x924, count);
                    if (static_cast<U>(edi) < count) {
                        U i = static_cast<U>(edi);
                        do {
                            U d = rd(word(b) + 0x920);
                            U next = rd(d + i * 4u + 4u);
                            wr(d + i * 4u, next);
                            count = rd(word(b) + 0x924);
                            ++i;
                        } while (i < count);
                    }
                    if (element) live_free(ptr(element));
                    --edi;
                } while (edi >= 0);
            }
        }
    }

    // 004F861B..004F8635: journal.
    if (U p = rd(word(b) + 0x9cc)) {
        kotor_fastcall_005c58f0(ptr(p), nullptr);
        live_free(ptr(p));
        wr(word(b) + 0x9cc, 0);
    }

    // 004F863B..004F864E: +0x4c0.
    if (U p = rd(word(b) + 0x4c0)) {
        live_free(ptr(p));
        wr(word(b) + 0x4c0, 0);
    }

    // 004F8654..004F8671: two-level block at +0xa80.
    if (U p = rd(word(b) + 0xa80)) {
        live_free(ptr(rd(p)));
        live_free(ptr(p));
        wr(word(b) + 0xa80, 0);
    }

    // 004F8677..004F8671: member destruction is compiler-generated below
    // (m_950, m_920, m_8f4, m_4c8, m_3a8, m_330, m_324, m_2a0, m_230, base),
    // matching EH 0071B584 states 9..0.
}

// ------------------------------------------------------------- 005ACF40 body
CSWSCreatureStats::~CSWSCreatureStats() noexcept(false) {
    unsigned char* b = reinterpret_cast<unsigned char*>(this);

    // 005ACF5F..005ACF7C: +0x168.
    if (U p = rd(word(b) + 0x168)) {
        live_free(ptr(p));
        wr(word(b) + 0x168, 0);
    }

    // 005ACF82..005ACF84: ClearFeats().
    ClearFeats();

    // 005ACF89..005ACFDB: destroy the CSWLevelStats vector at +0x28/+0x2c,
    // high index first, then ~CSWLevelStats and free.
    {
        int edi = static_cast<int>(rd(word(b) + 0x2c));
        if (edi >= 0) {
            --edi;
            if (edi >= 0) {
                do {
                    U data = rd(word(b) + 0x28);
                    U element = rd(data + static_cast<U>(edi) * 4u);
                    U count = rd(word(b) + 0x2c);
                    --count;
                    wr(word(b) + 0x2c, count);
                    if (static_cast<U>(edi) < count) {
                        U i = static_cast<U>(edi);
                        do {
                            U d = rd(word(b) + 0x28);
                            U next = rd(d + i * 4u + 4u);
                            wr(d + i * 4u, next);
                            count = rd(word(b) + 0x2c);
                            ++i;
                        } while (i < count);
                    }
                    if (element) {
                        static_cast<CSWLevelStats*>(ptr(element))->~CSWLevelStats();
                        live_free(ptr(element));
                    }
                    --edi;
                } while (edi >= 0);
            }
        }
    }

    // 005ACFDC..005ACFF9: two-level block at +0x128.
    if (U p = rd(word(b) + 0x128)) {
        live_free(ptr(rd(p)));
        live_free(ptr(p));
        wr(word(b) + 0x128, 0);
    }

    // 005ACFFF..005AD019: CCombatInformation.
    if (U p = rd(word(b) + 0x118)) {
        kotor_m15b_combat_information_destruct_00550e00(ptr(p), nullptr);
        live_free(ptr(p));
        wr(word(b) + 0x118, 0);
    }

    // 005AD01F..005AD0AF: compiler-generated member destruction (m_1a4, m_e0,
    // m_8c, m_58, m_3c, m_34, m_28, m_18, m_c, m_0) matches EH 0072201E.
}

// ------------------------------------------------------------- 005AA8C0 body
void CSWSCreatureStats::ClearFeats() {
    unsigned char* b = reinterpret_cast<unsigned char*>(this);
    // 005AA8C3..005AA8DF: first array data.
    if (U p = rd(word(b))) {
        live_free(ptr(p));
        wr(word(b), 0);
        wr(word(b) + 8, 0);
    }
    // 005AA8DF..005AA8EB: count = 0.
    wr(word(b) + 4, 0);
    // 005AA8F0..005AA938: free elements from the end; no shift is needed for
    // the last element, but the original shift loop is preserved.
    while (U n = rd(word(b) + 0x10)) {
        U data = rd(word(b) + 0x0c);
        U element = rd(data + (n - 1u) * 4u);
        U count = n - 1u;
        wr(word(b) + 0x10, count);
        if (n - 1u < count) {
            for (U i = n - 1u; i < count; ++i) {
                U d = rd(word(b) + 0x0c);
                wr(d + i * 4u, rd(d + i * 4u + 4u));
            }
        }
        if (element) live_free(ptr(element));
    }
}

// ------------------------------------------------------------- 005CB270 body
CSWLevelStats::~CSWLevelStats() noexcept(false) {
    // 005CB28F..005CB2F3: explicit frees in original order.  The compiler
    // then runs the member destructors (states 2,1,0) on the now-empty lists,
    // exactly like the original's repeated 0056B6F0/006FA852 tail.
    if (m_24) { live_free(m_24); m_24 = nullptr; }
    if (m_18.data) {
        live_free(m_18.data);
        m_18.data = nullptr;
        m_18.count = 0;
        m_18.capacity = 0;
    }
    CExoArrayList* a0 = reinterpret_cast<CExoArrayList*>(m_0.raw);
    CExoArrayList* ac = reinterpret_cast<CExoArrayList*>(m_c.raw);
    if (a0->data) {
        live_free(a0->data);
        a0->data = nullptr;
        a0->count = 0;
        a0->capacity = 0;
    }
    if (ac->data) {
        live_free(ac->data);
        ac->data = nullptr;
        ac->count = 0;
        ac->capacity = 0;
    }
}

// ------------------------------------------------------------- 004D5E60 body
CSWSCombatRound::~CSWSCombatRound() noexcept(false) {
    unsigned char* b = reinterpret_cast<unsigned char*>(this);

    // 004D5E7E..004D5EF8: drain and destroy the list wrapper at +0x9b0.
    if (U wrapper = rd(word(b) + 0x9b0)) {
        U internal = rd(wrapper);
        if (rd(internal) != 0) {
            do {
                internal = rd(rd(word(b) + 0x9b0));
                U element = word(base_ini_reconstruction::remove_head_005e9c80(
                    reinterpret_cast<kotor_list::Internal*>(ptr(internal)), nullptr));
                if (element) live_free(ptr(element));
                internal = rd(rd(word(b) + 0x9b0));
            } while (rd(internal) != 0);
        }
        if (U w = rd(word(b) + 0x9b0)) {
            U in = rd(w);
            if (in) {
                base_ini_reconstruction::destroy_list_005e9ab0(
                    reinterpret_cast<kotor_list::Internal*>(ptr(in)), nullptr);
                live_free(ptr(in));
            }
            live_free(ptr(w));
        }
        wr(word(b) + 0x9b0, 0);
    }

    // 004D5EF9..004D5F0C: +0x9c8.
    if (U p = rd(word(b) + 0x9c8)) {
        live_free(ptr(p));
        wr(word(b) + 0x9c8, 0);
    }

    // 004D5F12..004D5F6F: compiler-generated member destruction
    // (m_924 state 2, m_918 state 1, m_4 state 0) matches EH 0071A1C5.
}

// ------------------------------------------------------------- 005CEDE0 body
extern "C" void __fastcall recovered_pathfind_shutdown_005cede0(void* self, void*) {
    unsigned char* b = static_cast<unsigned char*>(self);
    // 005CEDE3..005CEE91: free +0x90, 0x98, 0xac, 0xb0, 0xb8, 0xc4, 0xc8,
    // 0xd0, 0xd4 (no store-back for these).
    const unsigned offsets[] = {0x90, 0x98, 0xac, 0xb0, 0xb8, 0xc4, 0xc8, 0xd0, 0xd4};
    for (unsigned i = 0; i < sizeof(offsets) / sizeof(offsets[0]); ++i) {
        if (U p = rd(word(b) + offsets[i])) live_free(ptr(p));
    }
    // 005CEE91..005CEE9B: +0xf0 -> 005CEB70(this).
    if (rd(word(b) + 0xf0))
        w5_pathfind_delete_first_tile_alternatives_005ceb70(self, nullptr);
    // 005CEEA0..005CEF17: free +0xe8, 0xec, 0x16c, 0x238, 0x1ac with store 0.
    const unsigned zeroed[] = {0xe8, 0xec, 0x16c, 0x238, 0x1ac};
    for (unsigned i = 0; i < sizeof(zeroed) / sizeof(zeroed[0]); ++i) {
        if (U p = rd(word(b) + zeroed[i])) {
            live_free(ptr(p));
            wr(word(b) + zeroed[i], 0);
        }
    }
    // 005CEF1D..005CEF42: +0x21c, also zero +0x218.
    if (U p = rd(word(b) + 0x21c)) {
        live_free(ptr(p));
        wr(word(b) + 0x218, 0);
        wr(word(b) + 0x21c, 0);
    }
    wr(word(b) + 0x21c, 0);
    // 005CEF42..005CEF67: +0x228, also zero +0x220.
    if (U p = rd(word(b) + 0x228)) {
        live_free(ptr(p));
        wr(word(b) + 0x220, 0);
        wr(word(b) + 0x228, 0);
    }
    wr(word(b) + 0x228, 0);
    // 005CEF67..005CEF8C: +0x22c, also zero +0x224.
    if (U p = rd(word(b) + 0x22c)) {
        live_free(ptr(p));
        wr(word(b) + 0x224, 0);
        wr(word(b) + 0x22c, 0);
    }
    wr(word(b) + 0x22c, 0);
}

// 005CFD90: exact `jmp 005CEDE0` transfer (5 bytes + nop padding in original).
extern "C" __declspec(naked) void __fastcall recovered_pathfind_shutdown_005cfd90(void*, void*) {
    __asm {
        jmp recovered_pathfind_shutdown_005cede0
    }
}

// ------------------------------------------------------------- 005BF750 body
CSWSFaction::CSWSFaction(unsigned id) {
    // 005BF772..005BF781: CExoArrayList at +0 and CExoString at +0x18 are
    // constructed by the member initialisers (zeroed data/count/capacity and
    // data/length), matching the inlined stores in the original.
    // 005BF786..005BF7B4: remove the creature from its current faction.
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);
    if (combat_reuse_faction_00513fc0(creature, nullptr) != nullptr) {
        CSWSFaction* old_faction = static_cast<CSWSFaction*>(
            combat_reuse_faction_00513fc0(creature, nullptr));
        old_faction->RemoveMember(id);
    }

    // 005BF7B9..005BF7DE: append id to this faction's member list.
    if (m_members.count == m_members.capacity) {
        menu_list_growth::allocate_005e03c0(
            reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&m_members), nullptr,
            m_members.capacity ? m_members.capacity * 2u : 0x10u);
    }
    static_cast<U*>(m_members.data)[m_members.count] = id;
    ++m_members.count;

    // 005BF7E1..005BF835: publish into the faction manager's list and update
    // the creature stats faction id.
    if (void* internal = server_internal()) {
        void* manager = ptr(rd(word(internal) + 0x10054));
        if (manager) {
            CExoArrayList* list = static_cast<CExoArrayList*>(
                ptr(rd(word(manager) + 0x24)));
            if (list->count == list->capacity) {
                menu_list_growth::allocate_005e03c0(
                    reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                    list->capacity ? list->capacity * 2u : 0x10u);
            }
            static_cast<CSWSFaction**>(list->data)[list->count] = this;
            ++list->count;
            m_index = list->count - 1u;
            CSWSCreatureStats* stats = static_cast<CSWSCreatureStats*>(
                ptr(rd(word(creature) + 0xa74)));
            wr(word(stats) + 0x78, m_index);
        }
    }

    // 005BF838..005BF83D: Initialise().
    Initialise();
}

// 005BE830 CSWSFaction::Initialise.  The original constructs an empty
// CExoString from the literal at 0x73D71C (an empty string), assigns it to
// +0x18, destroys the temporary, then stores +0x14 = 0 and +0x20 = 1.
void CSWSFaction::Initialise() {
    if (m_name.data) {
        live_free(m_name.data);
        m_name.data = nullptr;
    }
    m_name.length = 0;
    m_14 = 0;
    m_20 = 1;
}

// 005BE8A0 CSWSFaction::~CSWSFaction.  Compiler-generated tail destroys
// m_name (state 1) then m_members (state 0), matching handler 00722E08 /
// FuncInfo 00772514, maxState 1 (00722E14 is int3 padding; 00722E33/maxState 2
// belongs to the 005BF750 constructor).
CSWSFaction::~CSWSFaction() noexcept(false) {
}

// 005BF920 CSWSFaction::RemoveMember (RET 4).  EH 00722E4B: one cleanup state
// for the `new CSWSFaction(id)` allocation.
void CSWSFaction::RemoveMember(unsigned id) {
    // 005BF93D..005BF950: creature lookup.
    void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);
    if (!creature) return;

    // 005BF956..005BF972: count and membership scan (signed count).
    int count = static_cast<int>(m_members.count);
    if (count <= 0) return;
    int matches = 0;
    void** data = static_cast<void**>(m_members.data);
    for (int i = 0; i < count; ++i)
        if (data[i] == ptr(id)) ++matches;
    if (matches == 0) return;

    // 005BF978..005BF97B: remove the id from the member list.
    recovered_int_list_remove_0066dcc0(this, nullptr, id);

    // 005BF980..005BF9BB: when the creature is a party member (creature+0x9d4
    // == 1), build a fresh faction and broadcast its member list.
    if (rd(word(creature) + 0x9d4) == 1) {
        FactionNewGuard guard{live_operator_new(0x24)};
        CSWSFaction* fresh = ::new (guard.p) CSWSFaction(id);
        guard.p = nullptr;
        fresh->SendFactionUpdateList();
    }

    // 005BF9C0..005BFA07 / 005BFA09..005BFA0C.
    if (m_members.count != 0) {
        SendFactionUpdateRemove(id);
        return;
    }

    // The list is now empty: delete the faction only for a non-NPC faction
    // whose creature stats mark it as a PC (+0x6c == 1).
    void* internal = server_internal();
    if (!internal) return;
    void* manager = ptr(rd(word(internal) + 0x10054));
    if (!manager) return;
    if (recovered_faction_is_npc_0052b280(manager, nullptr, m_index) != 0) return;
    CSWSCreatureStats* stats = static_cast<CSWSCreatureStats*>(
        ptr(rd(word(creature) + 0xa74)));
    if (rd(word(stats) + 0x6c) != 1) return;
    static_cast<CFactionManager*>(manager)->DeleteFaction(m_index);
}

// 005BE960 CSWSFaction::SendFactionUpdateList.
void CSWSFaction::SendFactionUpdateList() {
    void* message = sws_message();
    for (int i = 0; i < static_cast<int>(m_members.count); ++i) {
        unsigned id = static_cast<unsigned>(static_cast<U*>(m_members.data)[i]);
        void* player = kotor_fastcall_004ae910(server_app(), nullptr, id);
        if (player) {
            (void)recovered_party_list_00567f90(
                message, nullptr, rd(word(player) + 4), m_members.count,
                m_members.data, 1u, id);
        }
    }
}

// 005BEA20 CSWSFaction::SendFactionUpdateRemove (RET 4).
void CSWSFaction::SendFactionUpdateRemove(unsigned id) {
    void* message = sws_message();
    for (int i = 0; i < static_cast<int>(m_members.count); ++i) {
        unsigned member = static_cast<unsigned>(static_cast<U*>(m_members.data)[i]);
        void* object = kotor_fastcall_004ae910(server_app(), nullptr, member);
        if (object) {
            (void)recovered_party_list_00567f90(
                message, nullptr, rd(word(object) + 4), 1u, &id, 4u,
                0x7f000000u);
        }
    }
}

// 0052B370 CFactionManager::DeleteFaction (RET 4).
void CFactionManager::DeleteFaction(unsigned index) {
    if (!m_factions) return;
    CSWSFaction* faction = static_cast<CSWSFaction*>(
        static_cast<void**>(m_factions->data)[index]);
    if (!faction) return;
    faction->~CSWSFaction();                                        // 0052B38B 005BE8A0
    live_free(faction);                                             // 0052B391
    static_cast<void**>(m_factions->data)[index] = nullptr;         // 0052B39E
}

// ------------------------------------------------------------- 004FDE20 body
// Original: push esi; mov esi,ecx; call 004F8410; test byte [esp+8],1; je;
// push esi; call _free; add esp,4; mov eax,esi; pop esi; ret 4.
// A non-static member with one stack argument is __thiscall: ECX = this,
// callee pops the flags argument, returns this in EAX.  The external C name
// used by the 0963 creature_table slot is bound to this member by the linker
// /alternatename below, so the table call site stays a thiscall RET 4.
void* CSWSCreature::scalar_delete(unsigned flags) {
    this->~CSWSCreature();                                          // 004FDE23
    if (flags & 1u) live_free(this);                                // 004FDE28..004FDE35
    return this;                                                    // 004FDE38..004FDE3B
}

}  // namespace case54_r2

// 0963 `creature_table[0]` declares `extern "C" void* recovered_creature_-
// scalar_delete_004fde20()` (C symbol _recovered_...).  Bind that symbol to
// the real __thiscall body without inventing a second function.  The option is
// repeated in INTEGRATION.md so the production link uses the same binding.
#pragma comment(linker, "/alternatename:_recovered_creature_scalar_delete_004fde20=?scalar_delete@CSWSCreature@case54_r2@@QAEPAXI@Z")

