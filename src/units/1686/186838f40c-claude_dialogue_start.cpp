// Lane "dialogue-start": the server conversation START chain, transcribed from the original swkotor.exe
// (read-only; capstone linear sweep). Addresses in comments are original instructions.
//
//   ACTION 255 BeginConversation 0052E9F0 -> CSWSObject::StartDialog 004CF490
//     -> new CResGFF("DLG ") / new CSWSDialog 005A0E80 -> CSWSDialog::LoadDialog 005A2AE0
//          (InitializePsuedoRandomCameraAngles 0059F220, Cleanup 005A27B0, LoadDialogBase 0059F5F0,
//           LoadDialogCamera 0059EAA0, LoadDialogLinkedNode 0059EC10, node/entry/reply/link/stunt ctors)
//     -> SetDialogOwner 0059F430 -> GetStartEntry 005A3640 (GetSpeaker 0059FC30, CheckScript 0059EC90,
//          BarkOneLine 005A1050, RunScript 0059ED70) -> delivered CSWSObject::SendDialogEntry 004CD460
//   CSWSDialog destructors 005A4860 / 005A4880 (vtable 0074A618 has exactly one slot: the scalar deleting dtor).
// Every callee binds to a real c05 body, a delivered dialogue object (D1-D5/D4 sources) or a function of the
// sibling files claude_dialogue_server.cpp (fork A), claude_dialogue_client.cpp (fork B1) and
// claude_dialogue_camera.cpp (fork B2) under the kotor_fastcall_<addr>@N rule.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

// ---------------------------------------------------------------------------------------------------------------
// Extra providers used only here
// ---------------------------------------------------------------------------------------------------------------
namespace menu_tables_m41 { void* __fastcall ClientListConstruct_004b25d0(void* list, void*); }  // CExoLinkedList<CSWSClient>()
namespace kotor_reconstruction {
struct StartupString { void* data; U length; int __thiscall BorrowedEquals005e5290(const char* text); };  // operator==(char*)
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs);        // CExoString::operator+
namespace kotor_menu_10_19 {
struct String {
    void* data;
    U length;
    const char* __thiscall text_005e5670();                 // existing c05 provider (non-const decoration QAE)
    const char* __thiscall text_005e5670() const;           // const decoration QBE imported by delivered 005a4010.obj
};
}

extern "C" {
extern void* kotor_cell_tlk_007a3a08;                                                       // [007A3A08] CTlkTable*
int __fastcall async_string_equal_005e5200(void* s, void*, const void* other);             // CExoString::operator==(CExoString&)
void* __fastcall exo_locstring_get_005ea130(void* loc, void*, U lang, void* out, U gender); // CExoLocString::GetString
U __fastcall tlk_fetch_0041e550(void* tlk, void*, U strref, void* result, U flag);         // CTlkTable::Fetch
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                        // CClientExoApp::GetGameObject
void* __fastcall kotor_m15b_creature_stats_get_full_name_00647ce0(void* stats, void*, void* out); // GetFullName
void __fastcall feedback_append_0062b5c0(void* gui, void*, void* text, U a, U b);          // AppendToMsgBuffer
void* __fastcall recovered_object_area_004cb120(void* object, void*);                       // CSWSObject::GetArea
// delivered D3 / D5
void __fastcall kotor_fastcall_005a0a40(void* dialog, void*, void* object, I flag);         // RunEndConversationScript
U __fastcall kotor_fastcall_004cb5c0(void* object, void*, const void* name, float range);  // GetNearestObjectByName
// fork A (claude_dialogue_server.cpp)
void __fastcall kotor_fastcall_005a0c80(void* dialog, void*);                               // ReEquipItemsForDialog
void __fastcall kotor_fastcall_005a0dd0(void* dialog, void*);                               // ReEquipHItemForDialog
// fork B1 (claude_dialogue_client.cpp)
void __fastcall kotor_fastcall_006332b0(void* gui, void*);                                  // CGuiInGame::CloseDialog
void __fastcall kotor_fastcall_00628670(void* gui, void*, U type, U computer);              // SetConversationType
void __fastcall kotor_fastcall_0062d510(void* gui, void*);                                  // HideContainerGui
// local exports not in the common header
dlg::U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out_struct);              // CResGFF::GetTopLevelStruct
void* __fastcall kotor_fastcall_0059e990(void* speaker, void*);                             // CSWSDialogSpeaker()
void __fastcall kotor_fastcall_005edbb0(void* client, void*, U id, void* text, void* sound, U flag); // owner: claude_w5_object_actions.cpp
}

// [00833A9C] logConversations: .data, zero-initialised, never written by any original instruction and never
// address-taken (only readers 005A124C and 006559C0). Shared selectany storage so the other reader can bind it.
extern "C" __declspec(selectany) int recovered_log_conversations_00833a9c = 0;

namespace {
struct GffStruct { U index; };                   // CResStruct (4 bytes)
struct GffList { U words[5]; };                  // CResList (struct handle + 16-byte label)
static_assert(sizeof(GffList) == 20, "CResList");
// Objects constructed by the callee through a hidden return pointer (no constructor here); destructor at the
// original scope end.
struct RawStr { U data, length; ~RawStr() { frame_LogStringDtor_005e5c20(this, nullptr); } };
struct RawLoc { U a, b; ~RawLoc() { exo_locstring_destroy_005ea000(this, nullptr); } };

const char kEmpty[] = "";                        // [0073D71C] ""
inline void* nul() { return nullptr; }
inline char* minus4(void* p) { return static_cast<char*>(p) - 4; }
inline U count_before(void* p) { U v; std::memcpy(&v, minus4(p), 4); return v; }
inline void* creature(void* object) { return vcall<void*>(object, 0x30); }   // AsSWSCreature
inline void* as_object(void* object) { return vcall<void*>(object, 0x10); }  // AsSWSObject
inline void* players(void* dialog) { return rp(dialog, 0x64); }               // CExoLinkedListInternal*
inline U list_head(void* internal) { return rd(internal, 0); }
inline void* remove_head(void* internal) {
    return base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(internal), nullptr);
}
inline void set_global_dialog_state(U state) { kotor_fastcall_0062ec60(in_game_gui(), nullptr, state); }
}  // namespace

// CSWSDialog vtable 0074A618 (single slot: 005A4860 scalar deleting destructor; 0074A61C is string data).
extern "C" void* __fastcall kotor_fastcall_005a4860(void* dialog, void*, U flags);
namespace { void* const g_dialog_vtable_0074a618[1] = { reinterpret_cast<void*>(&kotor_fastcall_005a4860) }; }

extern "C" {

// ===============================================================================================================
// Leaves
// ===============================================================================================================

// 005E5670 CExoString::CStr (const decoration requested by the delivered 005a4010.obj): forwards to the existing
// c05 body ?text_005e5670@String@kotor_menu_10_19@@QAEPBDXZ (0078-window-native-strings.obj).
}  // extern "C"
const char* kotor_menu_10_19::String::text_005e5670() const {
    return const_cast<String*>(this)->text_005e5670();
}
extern "C" {

// 00411240 CResGFF::GetTopLevelStruct(CResStruct* out) RET 4: null -> EAX 0 (the null itself); else out->index = 0, 1.
U __fastcall kotor_fastcall_00411240(void*, void*, void* out) {
    if (out == nullptr) return 0;                                   // 00411240..00411248
    wr(out, 0, 0);                                                  // 0041124B
    return 1;                                                       // 00411251
}

// 004AF010 CServerExoApp::SetActionConversation(v) RET 4: internal+1B910 = v
void __fastcall kotor_fastcall_004af010(void* server, void*, U value) { wr(rp(server, 4), 0x1b910, value); }
// 004AF020 CServerExoApp::GetActionConversation: internal+1B910
U __fastcall kotor_fastcall_004af020(void* server, void*) { return rd(rp(server, 4), 0x1b910); }
// 004AF040 CServerExoApp::GetIgnoreConversationStartRange: internal+1B914
U __fastcall kotor_fastcall_004af040(void* server, void*) { return rd(rp(server, 4), 0x1b914); }
// 004AE810 CServerExoApp::GetPlayerLanguage(id) RET 4: tail jump to CServerExoAppInternal::GetPlayerLanguage 004B4050
U __fastcall kotor_fastcall_004ae810(void* server, void*, U player_id) {
    return kotor_fastcall_004b4050(rp(server, 4), nullptr, player_id);   // 004AE810..004AE813
}
// 005EDCA0 CClientExoApp::SetDialogSent(v) RET 4: internal+230 = v
void __fastcall kotor_fastcall_005edca0(void* client, void*, U flag) { wr(rp(client, 4), 0x230, flag); }
// 005EE010 CClientExoApp::ClearEnemySighted: internal+324 = 0
void __fastcall kotor_fastcall_005ee010(void* client, void*) { wr(rp(client, 4), 0x324, 0); }
// 005EDBB0 / 005F2C90 (client bark) are owned by claude_w5_object_actions.cpp (coordinator ruling); declared above.
// 004CB7A0 CSWSObject::SetDialogOwner(id) RET 4: +54 = id
void __fastcall kotor_fastcall_004cb7a0(void* object, void*, U id) { wr(object, 0x54, id); }
// 004CB5A0 CSWSObject::StopSoundPlayingInDialog: +220 = 1, tail jump to CClientExoApp::StopSoundPlayingInDialog (D4)
void __fastcall kotor_fastcall_004cb5a0(void* object, void*) {
    wr(object, 0x220, 1);                                           // 004CB5A0
    kotor_fastcall_005edbd0(client(), nullptr);                     // 004CB5AA..004CB5B2
}
// 004CC250 CSWSObject::ClearDirtyFlag(bit) RET 4: dword[+1FC + (bit>>5)*4] &= ~(1 << (bit & 31))
void __fastcall kotor_fastcall_004cc250(void* object, void*, U bit) {
    const U index = bit >> 5;                                       // 004CC257..004CC259
    const U shift = bit - (index << 5);                             // 004CC25C..004CC261
    const U mask = 0xffffffffu - (1u << shift);                     // 004CC263..004CC26D (or -1; sub)
    wr(object, 0x1fc + index * 4, rd(object, 0x1fc + index * 4) & mask);  // 004CC26F
}
// 004CC490 (CSWSDialog) add player record (RET 4): non-null -> AddTail on the +64 player list
void __fastcall kotor_fastcall_004cc490(void* dialog, void*, void* player) {
    if (player == nullptr) return;                                  // 004CC494..004CC496
    (void)resource_append_005e9dc0(players(dialog), nullptr, player);  // 004CC498..004CC49F (tail jump)
}
// 004064C0 CRes::GetResRef(CResRef* out, WORD* type) RET 8: *out = key(+14) resref; *type = key->+1A
void __fastcall kotor_fastcall_004064c0(void* res, void*, void* out_ref, void* out_type) {
    ResRef temp;                                                    // 004064C3 [esp+4] by-value result
    (void)ref_assign_copy_00406120(out_ref, nullptr, &temp, rp(res, 0x14));   // 004064C6..004064D3
    wr16(out_type, 0, rd16(rp(res, 0x14), 0x1a));                   // 004064D8..004064E3
}
// 005D1820 CVirtualMachineInternal::GetRunScriptReturnValue(int* type, void** value) RET 8
U __fastcall kotor_fastcall_005d1820(void* internal, void*, U* type, U* value) {
    if (rd(internal, 0x1c) != 3) return 0;                          // 005D1820..005D1828
    *type = 3;                                                      // 005D1833
    *value = rd(internal, 0x20);                                    // 005D1839..005D183C
    return 1;
}
// 005D0FE0 CVirtualMachine::GetRunScriptReturnValue: ECX = [ECX] internal, tail jump
U __fastcall kotor_fastcall_005d0fe0(void* vm_object, void*, U* type, U* value) {
    return kotor_fastcall_005d1820(rp(vm_object, 0), nullptr, type, value);
}

// ===============================================================================================================
// Node family constructors / destructors (thiscall, no stack arguments; usable as EH vector iterator callbacks)
// ===============================================================================================================

// 0059E860 CSWSDialogBase::CSWSDialogBase
void* __fastcall kotor_fastcall_0059e860(void* base, void*) {
    (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(base), nullptr); // +00 Speaker
    (void)exo_locstring_construct_005e9e10(at(base, 0x08), nullptr);  // 0059E885 +08 Text
    resref_default(at(base, 0x10));                                 // 0059E891 +10 Script
    wr(base, 0x20, 0); wr(base, 0x24, 0); wr(base, 0x2c, 0); wr(base, 0x30, 0);  // 0059E8A1..0059E8AA
    resref_default(at(base, 0x34));                                 // 0059E89E/0059E8AD +34 Sound
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(at(base, 0x44)), nullptr);
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(at(base, 0x4c)), nullptr);
    wr(base, 0x54, 0); wr(base, 0x60, 0); wr(base, 0x64, 0);        // 0059E8CB..0059E8D1
    wr(base, 0x6c, 0); wr(base, 0x70, 0); wr(base, 0x74, 0);        // 0059E8D4..0059E8DA
    wr8(base, 0x80, 0);                                             // 0059E8DD
    return base;                                                    // (+28,+58,+5C,+68,+78,+7C left as allocated)
}
// 0059E920 CSWSDialogBase::~CSWSDialogBase (does not release the +64 animation array: Cleanup does)
void __fastcall kotor_fastcall_0059e920(void* base, void*) {
    frame_LogStringDtor_005e5c20(at(base, 0x4c), nullptr);          // 0059E93D
    frame_LogStringDtor_005e5c20(at(base, 0x44), nullptr);          // 0059E94D
    exo_locstring_destroy_005ea000(at(base, 0x08), nullptr);        // 0059E95A
    frame_LogStringDtor_005e5c20(base, nullptr);                    // 0059E967
}
// 0059F4E0 CSWSDialogNode::CSWSDialogNode: base + camera (+84 Listener, +8C..+98 = 0, +9C word 7FFF, +A0 -1.0f, +A4 -1)
void* __fastcall kotor_fastcall_0059f4e0(void* node, void*) {
    (void)kotor_fastcall_0059e860(node, nullptr);                   // 0059F4FF
    void* camera = at(node, 0x84);
    (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(camera), nullptr); // 0059F512
    wr(camera, 0x08, 0); wr(camera, 0x0c, 0); wr(camera, 0x10, 0); wr(camera, 0x14, 0);  // 0059F51B..0059F524
    wr16(camera, 0x18, 0x7fff);                                     // 0059F52A
    wr(camera, 0x1c, 0xbf800000u);                                  // 0059F530 -1.0f
    wr(camera, 0x20, 0xffffffffu);                                  // 0059F537
    return node;
}
// 0059F550 CSWSDialogNode::~CSWSDialogNode. Quirk kept: a null this passes a null CExoString to the destructor
// (0059F56D..0059F57D: ecx stays 0), which faults exactly like the original.
void __fastcall kotor_fastcall_0059f550(void* node, void*) {
    void* listener = node != nullptr ? at(node, 0x84) : nul();     // 0059F56D..0059F577
    frame_LogStringDtor_005e5c20(listener, nullptr);                // 0059F57D
    kotor_fastcall_0059e920(node, nullptr);                         // 0059F58C
}
// 0059F5E0 CSWSDialogNode::~CSWSDialogNode thunk (jmp 0059F550); exact name imported by delivered 005a3820.obj
void __fastcall dialog_node_dtor_0059f5e0(void* node, void*) { kotor_fastcall_0059f550(node, nullptr); }
// 0059F5B0 CSWSDialogReply::CSWSDialogReply: node, +A8 = 0, +AC = 0, +B0 = -1 (name imported by 005a3820.obj)
void* __fastcall dialog_reply_ctor_0059f5b0(void* reply, void*) {
    (void)kotor_fastcall_0059f4e0(reply, nullptr);                  // 0059F5B3
    wr(reply, 0xa8, 0); wr(reply, 0xac, 0);                         // 0059F5BA..0059F5C0
    wr(reply, 0xb0, 0xffffffffu);                                   // 0059F5C6
    return reply;
}
// 005A0E60 CSWSDialogEntry::CSWSDialogEntry: node, +A8 = 0, +AC = 0
void* __fastcall kotor_fastcall_005a0e60(void* entry, void*) {
    (void)kotor_fastcall_0059f4e0(entry, nullptr);                  // 005A0E63
    wr(entry, 0xa8, 0); wr(entry, 0xac, 0);                         // 005A0E6A..005A0E70
    return entry;
}
// 0059EA40 CSWSDialogStuntModel::CSWSDialogStuntModel: +00 Participant string, +08 StuntModel resref
void* __fastcall kotor_fastcall_0059ea40(void* stunt, void*) {
    (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(stunt), nullptr); // 0059EA5D
    resref_default(at(stunt, 0x08));                                // 0059EA6D
    return stunt;
}
// 0059EA90 CLiveContentStruct::CLiveContentStruct (animation entry: +00 CExoString)
void* __fastcall kotor_fastcall_0059ea90(void* anim, void*) {
    (void)kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(anim), nullptr); // 0059EA93
    return anim;
}
// 0059E900 CSWSDialogLinkEntry::CSWSDialogLinkEntry: resref, +10 = 0, +14 = 0
void* __fastcall kotor_fastcall_0059e900(void* link, void*) {
    resref_default(link);                                           // 0059E903
    wr(link, 0x10, 0); wr(link, 0x14, 0);                           // 0059E90A..0059E90D
    return link;
}
// 0059E990 CSWSDialogSpeaker::CSWSDialogSpeaker: +00 = OBJECT_INVALID, +04 CExoString
void* __fastcall kotor_fastcall_0059e990(void* speaker, void*) {
    wr(speaker, 0, kInvalid);                                       // 0059E996
    (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(at(speaker, 4)), nullptr);
    return speaker;
}
// 0059EA20 speaker element destructor (Ghidra: ~CSWSPlayerJournalQuestUpdates): ~CExoString at +4
void __fastcall kotor_fastcall_0059ea20(void* speaker, void*) { frame_LogStringDtor_005e5c20(at(speaker, 4), nullptr); }
// 0059EA30 is `jmp 005E5C20` (~CExoString); the iterators below pass that real body directly.

// 0059F470 CExoLinkedList<CSWSDialogPlayer>::CExoLinkedList: [this] = new CExoLinkedListInternal (or 0)
void* __fastcall kotor_fastcall_0059f470(void* list, void*) {
    void* memory = live_operator_new(0xc);                          // 0059F487..0059F48B
    void* internal = nullptr;
    if (memory != nullptr)
        internal = recovered_server_create::construct_list_internal_005e9aa0(memory);  // 0059F4A3..0059F4A5
    wp(list, 0, internal);                                          // 0059F4AA / 0059F4C4
    return list;
}

// ===============================================================================================================
// CSWSDialog object
// ===============================================================================================================

// 005A0E80 CSWSDialog::CSWSDialog
void* __fastcall kotor_fastcall_005a0e80(void* d, void*) {
    wp(d, 0, g_dialog_vtable_0074a618);                             // 005A0EA5 [esi] = 0074A618
    resref_default(at(d, 0x1c));                                    // 005A0EAB EndConversation
    resref_default(at(d, 0x2c));                                    // 005A0EB3 EndConverAbort
    resref_default(at(d, 0x50));                                    // 005A0EBB CameraModel
    (void)kotor_fastcall_0059f470(at(d, 0x64), nullptr);            // 005A0EC3 player list
    resref_default(at(d, 0xfc));                                    // 005A0ED6 AmbientTrack
    (void)menu_tables_m41::ClientListConstruct_004b25d0(at(d, 0x114), nullptr);  // 005A0EE1
    (void)menu_tables_m41::ClientListConstruct_004b25d0(at(d, 0x118), nullptr);  // 005A0EF5
    wr(d, 0x80, kInvalid); wr(d, 0x7c, kInvalid);                   // 005A0EFF..005A0F05
    wr(d, 0x08, 0); wr(d, 0x04, 0); wr(d, 0x10, 0); wr(d, 0x0c, 0); // 005A0F08..005A0F11
    wr(d, 0x18, 0); wr(d, 0x14, 0); wr(d, 0x6c, 0); wr(d, 0x70, 0); // 005A0F14..005A0F1D
    wr(d, 0x68, 0); wr(d, 0x74, 0xffffffffu);                       // 005A0F20..005A0F23
    wr8(d, 0x78, kByte0073ecd0);                                    // 005A0F2A..005A0F30
    wr(d, 0x94, kInvalid); wr(d, 0x98, kInvalid);                   // 005A0F38..005A0F3E
    wr(d, 0x84, 0); wr(d, 0x88, 0); wr(d, 0x8c, 0); wr(d, 0x90, 0); // 005A0F4E..005A0F60
    wr(d, 0x60, 0); wr(d, 0x40, 0); wr(d, 0x44, 0); wr(d, 0x3c, 0); // 005A0F66..005A0F6F
    // 005A0F72..005A0FFC camera-angle table +9C..+F8
    static const U table[] = { 0, 1, 0, 1, 3, 1, 3, 2, 2, 1, 3, 1, 2, 1, 2, 1, 3, 2, 3, 1, 1, 1, 0, 0 };
    for (U i = 0; i < sizeof(table) / sizeof(table[0]); ++i) wr(d, 0x9c + i * 4, table[i]);
    resref_assign_text(at(d, 0xfc), kEmpty);                        // 005A1002..005A100E
    wr(d, 0x10c, 0); wr(d, 0x110, 0); wr(d, 0x120, 0); wr(d, 0x124, 0); wr(d, 0x128, 0);  // 005A1017..005A102F
    wr(d, 0x11c, 2);                                                // 005A1036
    return d;                                                       // (+48/+4C stunt list left as allocated)
}

// 005A4880 CSWSDialog::~CSWSDialog
void __fastcall kotor_fastcall_005a4880(void* d, void*) {
    wp(d, 0, g_dialog_vtable_0074a618);                             // 005A489E
    kotor_fastcall_005a0c80(d, nullptr);                            // 005A48AC ReEquipItemsForDialog
    kotor_fastcall_005a0dd0(d, nullptr);                            // 005A48B3 ReEquipHItemForDialog
    kotor_fastcall_005a27b0(d, nullptr);                            // 005A48BA Cleanup
    static const U lists[3] = { 0x118u, 0x114u, 0x64u };            // 005A48BF / 005A48DE / 005A48FD
    for (U off : lists) {
        void* internal = rp(d, off);
        if (internal == nullptr) continue;
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(internal), nullptr);
        live_free(internal);
    }
}
// 005A4860 CSWSDialog scalar deleting destructor (vtable slot 0) RET 4
void* __fastcall kotor_fastcall_005a4860(void* d, void*, U flags) {
    kotor_fastcall_005a4880(d, nullptr);                            // 005A4863
    if ((flags & 1) != 0) live_free(d);                             // 005A4868..005A4870
    return d;
}

// 0059F430 CSWSDialog::SetDialogOwner(CSWSObject*) RET 4
void __fastcall kotor_fastcall_0059f430(void* d, void*, void* object) {
    const I level = rdi(object, 0x78);                              // 0059F434
    wr(d, 0x11c, static_cast<U>(level));                            // 0059F43A
    if (level < 3) wr(object, 0x78, 3);                             // 0059F437/0059F440 (signed jge) 0059F442
    wr(d, 0x80, rd(object, 4));                                     // 0059F449..0059F458
}

// 0059E9B0 CSWSDialog::ClearDialogOwnerInObject(id) RET 4
void __fastcall kotor_fastcall_0059e9b0(void* d, void*, U id) {
    void* object = nullptr;                                         // the argument slot is reused as the out cell
    if (!get_object(id, &object)) return;                           // 0059E9B0..0059E9D7
    if (rd(object, 0x54) != rd(d, 0x80)) return;                    // 0059E9D9..0059E9E9
    kotor_fastcall_004cb7a0(object, nullptr, kInvalid);             // 0059E9EB..0059E9F0
    if (creature(object) != nullptr)                                // 0059E9F5..0059E9FE
        recovered_set_activity_004f00a0(creature(object), nullptr, 4, 0, 0);  // 0059EA00..0059EA0F
}

// 005A27B0 CSWSDialog::Cleanup
void __fastcall kotor_fastcall_005a27b0(void* d, void*) {
    void* owner = kotor_fastcall_004ae750(server(), nullptr, rd(d, 0x80));    // 005A27B1..005A27C6
    if (owner != nullptr && as_object(owner) != nullptr) {          // 005A27CF..005A27DC
        const U level = rd(d, 0x11c);                               // 005A27E0
        wr(as_object(owner), 0x78, level);                          // 005A27E6..005A27EB
        kotor_fastcall_004cb5a0(as_object(owner), nullptr);         // 005A27EE..005A27F7
    }
    kotor_fastcall_0059e9b0(d, nullptr, rd(d, 0x80));               // 005A27FC..005A2805
    if (list_head(players(d)) == 0)                                 // 005A280A..005A280F
        kotor_fastcall_0059e9b0(d, nullptr, rd(d, 0x7c));           // 005A2811..005A2817
    while (list_head(players(d)) != 0) {                            // 005A281C..005A2821 / 005A28AA..005A28B4
        void* player = remove_head(players(d));                     // 005A2827..005A282A
        kotor_fastcall_006332b0(in_game_gui(), nullptr);            // 005A282F..005A2841 CloseDialog
        void* object = nullptr;
        if (get_object(rd(player, 4), &object) && rd(object, 0x54) == rd(d, 0x80)) {  // 005A2846..005A287B
            kotor_fastcall_004cb7a0(object, nullptr, kInvalid);     // 005A287D..005A2882
            if (creature(object) != nullptr)                        // 005A2887..005A2890
                recovered_set_activity_004f00a0(creature(object), nullptr, 4, 0, 0);  // 005A2892..005A289F
        }
        live_free(player);                                          // 005A28A4..005A28A5
    }
    if (rd(d, 0x84) != 0) {                                         // 005A28BA..005A28C0 speakers
        for (U i = 0; i < rd(d, 0x88); ++i)                         // 005A28C2..005A28ED
            kotor_fastcall_0059e9b0(d, nullptr, rd(rp(d, 0x84), i * 0xc));
        void* speakers = rp(d, 0x84);                               // 005A28EF
        if (speakers != nullptr) {                                  // 005A28F5..005A28F7
            c2da_destroy_array_006fa852(speakers, 0xc, static_cast<I>(count_before(speakers)),
                                        reinterpret_cast<void*>(&kotor_fastcall_0059ea20));  // 005A28F9..005A2908
            live_free(minus4(speakers));                            // 005A290D..005A290E
        }
        wr(d, 0x84, 0); wr(d, 0x88, 0);                             // 005A2916..005A291C
    }
    // entries (+08 array of 0xB0, count +04) and replies (+10 array of 0xB4, count +0C)
    const struct { U array, count, stride; } lists[2] = { { 0x08, 0x04, 0xb0 }, { 0x10, 0x0c, 0xb4 } };
    for (const auto& l : lists) {                                   // 005A2922..005A29D7 / 005A29DA..005A2A87
        if (rd(d, l.array) == 0) continue;
        for (U i = 0; i < rd(d, l.count); ++i) {
            const U off = i * l.stride;
            if (rd(rp(d, l.array), off + 0xac) != 0) {              // links
                live_free(rp(rp(d, l.array), off + 0xac));
                wr(rp(d, l.array), off + 0xac, 0);
            }
            void* anims = rp(rp(d, l.array), off + 0x64);
            if (anims != nullptr) {                                 // animation list
                c2da_destroy_array_006fa852(anims, 0xc, static_cast<I>(count_before(anims)),
                                            reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); // 0059EA30
                live_free(minus4(anims));
                wr(rp(d, l.array), off + 0x64, 0);
                wr(rp(d, l.array), off + 0x60, 0);
            }
        }
        void* nodes = rp(d, l.array);
        if (nodes != nullptr) {
            c2da_destroy_array_006fa852(nodes, l.stride, static_cast<I>(count_before(nodes)),
                                        reinterpret_cast<void*>(&dialog_node_dtor_0059f5e0));  // 0059F5E0
            live_free(minus4(nodes));
        }
        wr(d, l.array, 0); wr(d, l.count, 0);
    }
    if (rd(d, 0x18) != 0) {                                         // 005A2A8A..005A2A9D starting list
        live_free(rp(d, 0x18));
        wr(d, 0x18, 0); wr(d, 0x14, 0);
    }
    wr(d, 0x6c, 0); wr(d, 0x70, 0); wr(d, 0x68, 0);                 // 005A2AA5..005A2AAB
    wr(d, 0x74, 0xffffffffu);                                       // 005A2AAE
    wr(d, 0x80, kInvalid); wr(d, 0x7c, kInvalid);                   // 005A2AB5..005A2ABB
    wr(d, 0x8c, 0); wr(d, 0x90, 0);                                 // 005A2AC5..005A2ACB
    wr8(d, 0x78, kByte0073ecd0);                                    // 005A2ABE/005A2AD1
    // Quirk kept: the stunt-model array (+48/+4C) is neither released here nor in the destructor.
}

// 0059F220 CSWSDialog::InitializePsuedoRandomCameraAngles(CResGFF*) RET 4
void __fastcall kotor_fastcall_0059f220(void* d, void*, void* gff) {
    ResRef ref;
    resref_default(&ref);                                           // 0059F24B [esp+20]
    ExoString name;                                                 // 0059F254 [esp+18]
    U16 type = 0;                                                   // [esp+0C] (written by GetResRef)
    kotor_fastcall_004064c0(gff, nullptr, &ref, &type);             // 0059F259..0059F26C
    c2da_ref_to_string_00405f70(&ref, nullptr, &name);              // 0059F271..0059F27A
    U sum = 0;                                                      // edi
    for (I i = 0; i < static_cast<I>(async_string_length_005e5790(&name, nullptr)); ++i) {  // 0059F27F..0059F2C9
        const char ch = c2da_current_string_index_005e5600(&name, nullptr, i);   // 0059F292..0059F297
        const double power = m47_gob2::x87_pow_006fc1f0(10.0, static_cast<double>(i)); // 0059F29C..0059F2A9 [00749358]=10.0
        const U whole = w2::ftol2_006fae8c(power);                  // 0059F2AE __ftol2
        sum += (whole + 1u) * static_cast<U>(static_cast<I>(static_cast<signed char>(ch)));  // 0059F2B3..0059F2B7
    }
    wr(d, 0xa4, sum & 1u);                                          // 0059F2E1..0059F2E4
    wr(d, 0x9c, sum % 19u);                                         // 0059F2CF..0059F2F2
    wr(d, 0xa0, sum % 5u + 1u);                                     // 0059F2F8..0059F301
}

// 0059F5F0 CSWSDialog::LoadDialogBase(CSWSDialogBase*, CResGFF*, CResStruct*, DWORD* default_delay, int* ok) RET 14h
void __fastcall kotor_fastcall_0059f5f0(void*, void*, void* base, void* gff, void* s, U* delay, U* ok) {
    U local_ok = 0;                                                 // [esp+0C]
    U fade_ok = 0;                                                  // [esp+20]
    {
        ExoLocString fallback;                                      // 0059F613..0059F61F [esp+10]
        RawLoc text;                                                // [esp+28]
        (void)recovered_gff_localized_00411fd0(gff, nullptr, &text, s, "Text", ok, &fallback);  // 0059F624..0059F643
        (void)recovered_localized_assign_005e9e70(at(base, 0x08), nullptr, &text);              // 0059F648..0059F657
    }                                                               // 0059F65C / 0059F66D destructors
    {
        ResRef fallback; resref_from(&fallback, kEmpty);            // 0059F681..0059F68A
        ResRef value;
        (void)gui_load_gff_resref_00411e10(gff, nullptr, &value, s, "Script", &local_ok, &fallback);  // 0059F68F..0059F6A6
        resref_assign(at(base, 0x10), &value);                      // 0059F6AB..0059F6B4
    }
    {
        ExoString fallback(kEmpty);                                 // 0059F6B9..0059F6C2
        RawStr value;
        (void)gui_load_gff_string_00411ec0(gff, nullptr, &value, s, "Speaker", &local_ok, &fallback);  // ..0059F6E9
        str_assign(base, &value);                                   // 0059F6EE..0059F6F9
    }
    wr(base, 0x20, gui_load_gff_dword_00411c20(gff, nullptr, s, "WaitFlags", &local_ok, 0));  // 0059F723..0059F740
    {
        ExoString fallback(kEmpty);                                 // 0059F737..0059F743
        RawStr value;
        (void)gui_load_gff_string_00411ec0(gff, nullptr, &value, s, "Quest", &local_ok, &fallback);  // ..0059F76A
        str_assign(at(base, 0x4c), &value);                         // 0059F76F..0059F77B
    }
    wr(base, 0x54, gui_load_gff_dword_00411c20(gff, nullptr, s, "QuestEntry", &local_ok, 0));      // ..0059F7C8
    wr(base, 0x58, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, s, "PlotIndex", &local_ok, 0)));  // ..0059F7D2
    wrf(base, 0x5c, gui_load_gff_float_00411d00(gff, nullptr, s, "PlotXPPercentage", &local_ok, 0.0f));   // ..0059F7E7
    wr(base, 0x24, gui_load_gff_dword_00411c20(gff, nullptr, s, "Delay", &local_ok, 0));           // ..0059F80D
    wr8(base, 0x68, gui_load_gff_byte_00411a60(gff, nullptr, s, "FadeType", &fade_ok, 0));         // ..0059F815
    {
        U zero[3] = { 0, 0, 0 };                                    // 0059F82C..0059F83C [esp+28]
        U color[3];                                                 // [esp+10]
        void* r = gui_load_gff_vector_004122a0(gff, nullptr, color, s, "FadeColor", &local_ok, zero);  // ..0059F847
        wr(base, 0x6c, rd(r, 0)); wr(base, 0x70, rd(r, 4)); wr(base, 0x74, rd(r, 8));  // 0059F84C..0059F85C
    }
    wrf(base, 0x7c, gui_load_gff_float_00411d00(gff, nullptr, s, "FadeDelay", &local_ok, 0.0f));    // ..0059F873
    wrf(base, 0x78, gui_load_gff_float_00411d00(gff, nullptr, s, "FadeLength", &local_ok, 0.0f));   // ..0059F88A
    wr(base, 0x28, 0xffffffffu);                                    // 0059F896
    {
        ResRef fallback; resref_from(&fallback, kEmpty);            // 0059F88D..0059F89D
        ResRef value;
        (void)gui_load_gff_resref_00411e10(gff, nullptr, &value, s, "Sound", &local_ok, &fallback);  // ..0059F8B9
        resref_assign(at(base, 0x34), &value);                      // 0059F8BE..0059F8C7
    }
    ResRef vo;                                                      // [esp+4C]
    {
        ResRef fallback; resref_from(&fallback, kEmpty);            // 0059F8CC..0059F8D5
        (void)gui_load_gff_resref_00411e10(gff, nullptr, &vo, s, "VO_ResRef", &local_ok, &fallback);  // ..0059F8F1
    }
    const U8 exists = gui_load_gff_byte_00411a60(gff, nullptr, s, "SoundExists", &local_ok, 0x80);  // 0059F8F6..0059F908
    wr8(base, 0x80, exists);                                        // 0059F910
    if (gui_load_resref_is_valid_00406030(at(base, 0x34), nullptr) != 0) {   // 0059F90D..0059F91D
        wr(base, 0x2c, 1);                                          // 0059F943
    } else {
        resref_assign(at(base, 0x34), &vo);                         // 0059F91F..0059F92C
        if (gui_load_resref_is_valid_00406030(at(base, 0x34), nullptr) == 0) {  // 0059F931..0059F93B
            wr8(base, 0x80, 0);                                     // 0059F93D (al = 0)
            wr(base, 0x2c, 1);                                      // 0059F943
        }
    }
    if (rdi(base, 0x24) == -1) {                                    // 0059F94A..0059F94E
        const int valid = gui_load_resref_is_valid_00406030(at(base, 0x34), nullptr);  // 0059F950..0059F953
        const U wait = rd(base, 0x20);                              // 0059F95A
        if (valid == 0) {
            if (wait == 0) {
                resref_assign_text(at(base, 0x34), kEmpty);         // 0059F963..0059F970
            } else {
                wr(base, 0x28, 0);                                  // 0059F97A
                wr(base, 0x24, *delay);                             // 0059F977..0059F983
            }
        } else {
            wr(base, 0x24, *delay);                                 // 0059F98A..0059F98F
            if (wait == 0) wr(base, 0x20, 2);                       // 0059F992..0059F994
        }
    } else {
        wr(base, 0x20, rd(base, 0x20) | 0x10u);                     // 0059F99D
    }
    const U8 fade = rd8(base, 0x68);                                // 0059F9A1
    if (fade == 1 || fade == 2) wr(base, 0x78, 0);                  // 0059F9A4..0059F9AE
    if (fade == 0 || fade_ok == 0) {                                // 0059F9B5..0059F9BD
        wr(base, 0x7c, 0); wr(base, 0x78, 0);                       // 0059F9CB..0059F9CE
        wr(base, 0x6c, 0); wr(base, 0x70, 0);                       // 0059F9E9..0059F9EC
        wr8(base, 0x68, 0); wr(base, 0x74, 0);                      // 0059F9EF..0059F9F3
    }
    GffList list;                                                   // [esp+6C]
    (void)gff_list_004118c0(gff, nullptr, &list, s, "AnimList");    // 0059F9F6..0059FA03
    const U n = gff_list_count_00411940(gff, nullptr, &list);       // 0059FA08..0059FA0F
    wr(base, 0x60, n);                                              // 0059FA18
    if (n != 0) {                                                   // 0059FA16..0059FA1B
        void* memory = live_operator_new(n * 0xc + 4);              // 0059FA1D..0059FA28
        void* array = nullptr;
        if (memory != nullptr) {                                    // 0059FA34..0059FA41
            wr(memory, 0, n);                                       // 0059FA4E
            array = at(memory, 4);
            crt_eh_vector_constructor_006fb55f(array, 0xc, static_cast<I>(n),
                reinterpret_cast<void*>(&kotor_fastcall_0059ea90), reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
        }
        wp(base, 0x64, array);                                      // 0059FA72
    }
    for (U i = 0; i < rd(base, 0x60); ++i) {                        // 0059FA75..0059FB3C
        GffStruct element;                                          // [esp+24]
        (void)gff_list_element_00411990(gff, nullptr, &element, &list, i);   // 0059FA86..0059FA93
        {
            ExoString fallback(kEmpty);                             // 0059FA98..0059FAA1
            RawStr value;
            (void)gui_load_gff_string_00411ec0(gff, nullptr, &value, &element, "Participant", ok, &fallback);  // ..0059FACB
            str_assign(at(rp(base, 0x64), i * 0xc), &value);        // 0059FAD0..0059FAE2
        }
        const U16 animation = recovered_gff_word_00411b40(gff, nullptr, &element, "Animation", ok, 0);  // 0059FB0C..0059FB1E
        wr16(rp(base, 0x64), i * 0xc + 8, animation);               // 0059FB23..0059FB2A
    }
}

// 0059EAA0 CSWSDialog::LoadDialogCamera(CSWSDialogCamera*, CResGFF*, CResStruct*, int* /*unused*/) RET 10h
void __fastcall kotor_fastcall_0059eaa0(void*, void*, void* camera, void* gff, void* s, U*) {
    U ok;                                                           // [esp+0C], written by every reader
    {
        ExoString fallback(kEmpty);                                 // 0059EABB..0059EAC4
        RawStr value;
        (void)gui_load_gff_string_00411ec0(gff, nullptr, &value, s, "Listener", &ok, &fallback);  // ..0059EAF0
        str_assign(camera, &value);                                 // 0059EAF5..0059EB01
    }
    wr(camera, 0x08, gui_load_gff_dword_00411c20(gff, nullptr, s, "CameraAngle", &ok, 0));          // ..0059EB48
    wr(camera, 0x0c, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, s, "CameraID", &ok, 0)));  // ..0059EB5F
    wrf(camera, 0x10, gui_load_gff_float_00411d00(gff, nullptr, s, "CamHeightOffset", &ok, 0.0f));  // ..0059EB67
    wrf(camera, 0x14, gui_load_gff_float_00411d00(gff, nullptr, s, "TarHeightOffset", &ok, 0.0f));  // ..0059EB7E
    wr16(camera, 0x18, recovered_gff_word_00411b40(gff, nullptr, s, "CameraAnimation", &ok, 0));    // ..0059EBA4
    wr(camera, 0x20, static_cast<U>(gui_load_gff_int_00411c90(gff, nullptr, s, "CamVidEffect", &ok, -1)));  // ..0059EBAD
    const float fov = gui_load_gff_float_00411d00(gff, nullptr, s, "CamFieldOfView", &ok, 0.0f);  // 0059EBB0..0059EBBF
    wrf(camera, 0x1c, fov);                                         // 0059EBC4 fst
    // 0059EBC7..0059EBE0: success == 0, or fcomp 0.0 with C0|C3 set (fov <= 0, ordered) -> -1.0f; NaN keeps it
    if (ok == 0 || (!(fov > 0.0f) && fov == fov)) wr(camera, 0x1c, 0xbf800000u);
    if (rd(camera, 0x08) != 6) wr(camera, 0x0c, 0xffffffffu);       // 0059EBE7..0059EBED
}

// 0059EC10 CSWSDialog::LoadDialogLinkedNode(CSWSDialogLinkEntry*, CResGFF*, CResStruct*, int* ok, DWORD count) RET 14h
void __fastcall kotor_fastcall_0059ec10(void*, void*, void* link, void* gff, void* s, U* ok, U count) {
    {
        ResRef fallback; resref_from(&fallback, kEmpty);            // 0059EC17..0059EC20
        ResRef value;
        (void)gui_load_gff_resref_00411e10(gff, nullptr, &value, s, "Active", ok, &fallback);  // ..0059EC44
        resref_assign(link, &value);                                // 0059EC4D..0059EC55
    }
    const U index = gui_load_gff_dword_00411c20(gff, nullptr, s, "Index", ok, 0);  // 0059EC5A..0059EC65
    wr(link, 0x10, index);                                          // 0059EC6E
    if (index > count) *ok = 0;                                     // 0059EC6A..0059EC73 (unsigned jbe)
}

// 005A2AE0 CSWSDialog::LoadDialog(CResGFF*, int load_all) RET 8 -> int success
U __fastcall kotor_fastcall_005a2ae0(void* d, void*, void* gff, U load_all) {
    U ok = 1;                                                       // [esp+10]
    U ok2 = 1;                                                      // [esp+18] optional fields
    kotor_fastcall_0059f220(d, nullptr, gff);                       // 005A2B0B..005A2B16
    wr(d, 0xf4, 1); wr(d, 0xf8, 1);                                 // 005A2B1D..005A2B23
    kotor_fastcall_005a27b0(d, nullptr);                            // 005A2B29 Cleanup
    GffStruct top;                                                  // [esp+14]
    (void)kotor_fastcall_00411240(gff, nullptr, &top);              // 005A2B2E..005A2B35
    auto read_ref = [&](const char* label, U* success, void* dst) {
        ResRef fallback; resref_from(&fallback, kEmpty);
        ResRef value;
        (void)gui_load_gff_resref_00411e10(gff, nullptr, &value, &top, label, success, &fallback);
        resref_assign(dst, &value);
    };
    read_ref("CameraModel", &ok2, at(d, 0x50));                     // 005A2B3A..005A2B71
    const U delay_entry = gui_load_gff_dword_00411c20(gff, nullptr, &top, "DelayEntry", &ok, 0);  // 005A2B76..005A2B95
    (void)gui_load_gff_dword_00411c20(gff, nullptr, &top, "DelayReply", &ok, 0);  // 005A2B8E..005A2BA5 (value discarded)
    read_ref("EndConversation", &ok2, at(d, 0x1c));                 // 005A2BAA..005A2BE4
    read_ref("EndConverAbort", &ok2, at(d, 0x2c));                  // 005A2BE9..005A2C23
    wr(d, 0x3c, gui_load_gff_byte_00411a60(gff, nullptr, &top, "Skippable", &ok2, 1));  // 005A2C28..005A2C52
    const I conversation_type = gui_load_gff_int_00411c90(gff, nullptr, &top, "ConversationType", &ok2, 0);  // ..005A2C70
    // 005A2C80: ComputerType is stored as a byte into the dword cell [esp+24] that 005A2EC9 later passes on. The upper
    // 24 bits of that cell are stale stack; the consumer (CSWGuiDialogComputer::SetType 006A8472) reads only the
    // low byte, so the zero-extended value is behaviourally exact.
    const U computer_type = gui_load_gff_byte_00411a60(gff, nullptr, &top, "ComputerType", &ok2, 0);  // 005A2C5D..005A2C80
    read_ref("AmbientTrack", &ok2, at(d, 0xfc));                    // 005A2C77..005A2CB8
    wr(d, 0x40, gui_load_gff_byte_00411a60(gff, nullptr, &top, "UnequipItems", &ok2, 0));  // 005A2CBD..005A2CE8
    wr(d, 0x44, gui_load_gff_byte_00411a60(gff, nullptr, &top, "UnequipHItem", &ok2, 0));  // 005A2CD5..005A2CF6
    const U animated_cut = gui_load_gff_byte_00411a60(gff, nullptr, &top, "AnimatedCut", &ok2, 0);  // ..005A2D24 [esp+30]
    const U old_hit_check = gui_load_gff_byte_00411a60(gff, nullptr, &top, "OldHitCheck", &ok2, 0);  // ..005A2D39
    wr(in_game_gui(), 0xc0, animated_cut);                          // 005A2D30..005A2D45
    wr(in_game_gui(), 0xc4, old_hit_check);                         // 005A2D4B..005A2D5B
    if (conversation_type == 1) {                                   // 005A2D58..005A2D61 / 005A2EC0..005A2EC3
        kotor_fastcall_00628670(in_game_gui(), nullptr, 1, computer_type);  // 005A2EC9..005A2EDF (no unstealth)
    } else {
        if (conversation_type == 2) wr(d, 0x110, 1);                // 005A2D67
        kotor_fastcall_00628670(in_game_gui(), nullptr, 0, 0);      // 005A2D71..005A2D84
        recovered_unstealth_party_00563c60(lane_config_required::GetPartyTable_004aee70(server(), nullptr), nullptr, 1);  // ..005A2D9B
    }
    if (animated_cut != 0)                                          // 005A2DA0..005A2DA8
        kotor_game_prepare::SetGlobalUnpauseableState0062b410(in_game_gui(), nullptr, 1);  // 005A2DAA..005A2DBC
    GffList list;                                                   // [esp+78]
    (void)gff_list_004118c0(gff, nullptr, &list, &top, "ReplyList");    // 005A2DC1..005A2DD5
    wr(d, 0x0c, gff_list_count_00411940(gff, nullptr, &list));     // 005A2DDA..005A2DE6
    (void)gff_list_004118c0(gff, nullptr, &list, &top, "EntryList");    // 005A2DE9..005A2DFD
    const U entry_count = gff_list_count_00411940(gff, nullptr, &list);   // 005A2E02..005A2E0E
    wr(d, 0x04, entry_count);                                       // 005A2E1A
    {
        void* memory = live_operator_new(entry_count * 0xb0 + 4);   // 005A2E10..005A2E1D
        void* entries = nullptr;
        if (memory != nullptr) {                                    // 005A2E29..005A2E32
            wr(memory, 0, entry_count);                             // 005A2E48
            entries = at(memory, 4);
            crt_eh_vector_constructor_006fb55f(entries, 0xb0, static_cast<I>(entry_count),
                reinterpret_cast<void*>(&kotor_fastcall_005a0e60), reinterpret_cast<void*>(&dialog_node_dtor_0059f5e0));
        }
        wp(d, 0x08, entries);                                       // 005A2E60
    }
    GffStruct element;                                              // [esp+2C]
    GffStruct link_element;                                         // [esp+28]
    GffList links;                                                  // [esp+64]
    U i = 0;                                                        // [esp+1C]
    if (ok == 0) { kotor_fastcall_005a27b0(d, nullptr); return ok; }   // 005A2E53..005A2E6B -> 005A331B
    for (;;) {                                                      // 005A2E73
        if (i >= rd(d, 0x04)) break;                                // 005A2E77..005A2E7A
        const U off = i * 0xb0;                                     // ebx
        (void)gff_list_element_00411990(gff, nullptr, &element, &list, i);   // 005A2E80..005A2E8D
        kotor_fastcall_0059f5f0(d, nullptr, at(rp(d, 0x08), off), gff, &element, const_cast<U*>(&delay_entry), &ok);  // ..005A2EAA
        void* entry = at(rp(d, 0x08), off);                         // 005A2EAF..005A2EB2
        void* camera = entry != nullptr ? at(entry, 0x84) : nul();  // 005A2EB5..005A2EE9
        kotor_fastcall_0059eaa0(d, nullptr, camera, gff, &element, &ok);     // 005A2EEB..005A2EF9
        (void)gff_list_004118c0(gff, nullptr, &links, &element, "RepliesList");  // 005A2EFE..005A2F0F
        wr(rp(d, 0x08), off + 0xa8, gff_list_count_00411940(gff, nullptr, &links));  // 005A2F14..005A2F23
        if (load_all != 0) {                                        // 005A2F2A..005A2F33
            const U n = rd(rp(d, 0x08), off + 0xa8);                // 005A2F39..005A2F3C
            if (n != 0) {                                           // 005A2F43..005A2F45
                void* array = live_operator_new(n * 0x18);          // 005A2F47..005A2F4F (no count prefix)
                if (array != nullptr)                               // 005A2F5B..005A2F68
                    vector_constructor_iterator_00401130(array, 0x18, static_cast<I>(n),
                                                         reinterpret_cast<void*>(&kotor_fastcall_0059e900));  // ..005A2F75
                wp(rp(d, 0x08), off + 0xac, array);                 // 005A2F82..005A2F90
            }
            for (U j = 0; ok != 0 && j < rd(rp(d, 0x08), off + 0xa8); ++j) {  // 005A2F97..005A3034
                (void)gff_list_element_00411990(gff, nullptr, &link_element, &links, j);  // 005A2FC4..005A2FD3
                void* link = at(rp(rp(d, 0x08), off + 0xac), j * 0x18);   // 005A2FDC..005A2FF3
                kotor_fastcall_0059ec10(d, nullptr, link, gff, &link_element, &ok, rd(d, 0x0c));  // 005A2FD8..005A2FF6
                const U8 inactive = gui_load_gff_byte_00411a60(gff, nullptr, &link_element, "DisplayInactive", &ok2, 0);
                wr(rp(rp(d, 0x08), off + 0xac), j * 0x18 + 0x14, inactive != 0 ? 1u : 0u);  // 005A3013..005A3023
            }
        }
        ++i;                                                        // 005A3040..005A304D
        if (ok == 0) { kotor_fastcall_005a27b0(d, nullptr); return ok; }   // 005A3051..005A305E
    }
    // 005A3063: all entries read
    if (ok == 0) { kotor_fastcall_005a27b0(d, nullptr); return ok; }       // 005A3065..005A3067
    if (load_all == 0) {                                            // 005A306D..005A3074
        wr(d, 0x0c, 0);                                             // 005A3327 (replies dropped)
    } else {
        (void)gff_list_004118c0(gff, nullptr, &list, &top, "ReplyList");    // 005A307A..005A308E
        const U reply_count = rd(d, 0x0c);                          // 005A3093
        void* memory = live_operator_new(reply_count * 0xb4 + 4);   // 005A3096..005A30A2
        void* replies = nullptr;
        if (memory != nullptr) {                                    // 005A30AE..005A30BB
            wr(memory, 0, reply_count);                             // 005A30D1
            replies = at(memory, 4);
            crt_eh_vector_constructor_006fb55f(replies, 0xb4, static_cast<I>(reply_count),
                reinterpret_cast<void*>(&dialog_reply_ctor_0059f5b0), reinterpret_cast<void*>(&dialog_node_dtor_0059f5e0));
        }
        wp(d, 0x10, replies);                                       // 005A30E0
        if (ok == 0) { kotor_fastcall_005a27b0(d, nullptr); return ok; }   // 005A30E3..005A30F8
        for (U r = 0;; ++r) {                                       // 005A3102 [esp+1C], offset [esp+24]
            if (r >= rd(d, 0x0c)) break;                            // 005A3102..005A3109
            const U off = r * 0xb4;
            (void)gff_list_element_00411990(gff, nullptr, &element, &list, r);   // 005A310F..005A311C
            kotor_fastcall_0059f5f0(d, nullptr, at(rp(d, 0x10), off), gff, &element, const_cast<U*>(&delay_entry), &ok);
            void* reply = at(rp(d, 0x10), off);                     // 005A3142..005A3145
            void* camera = reply != nullptr ? at(reply, 0x84) : nul();   // 005A3147..005A3152
            kotor_fastcall_0059eaa0(d, nullptr, camera, gff, &element, &ok);     // 005A3154..005A3162
            (void)gff_list_004118c0(gff, nullptr, &links, &element, "EntriesList");  // 005A3167..005A3178
            wr(rp(d, 0x10), off + 0xa8, gff_list_count_00411940(gff, nullptr, &links));  // 005A317D..005A318C
            const U n = rd(rp(d, 0x10), off + 0xa8);                // 005A3193..005A3196
            if (n != 0) {                                           // 005A319D..005A319F
                void* array = live_operator_new(n * 0x14);          // 005A31A1..005A31A9
                if (array != nullptr) {                             // 005A31B5..005A31C2
                    for (U k = 0; k < n; ++k) {                     // 005A31C4..005A31E2
                        resref_default(at(array, k * 0x14));
                        wr(array, k * 0x14 + 0x10, 0);
                    }
                }
                wp(rp(d, 0x10), off + 0xac, array);                 // 005A31EC..005A31FE
            }
            for (U j = 0; ok != 0 && j < rd(rp(d, 0x10), off + 0xa8); ++j) {  // 005A3205..005A32E6
                (void)gff_list_element_00411990(gff, nullptr, &link_element, &links, j);  // 005A3239..005A3248
                void* link = at(rp(rp(d, 0x10), off + 0xac), j * 0x14);   // 005A324D..005A326B
                const U entries_now = rd(d, 0x04);                  // 005A325F ebp

                {
                    ResRef fallback; resref_from(&fallback, kEmpty);   // 005A3262..005A326D
                    ResRef value;
                    (void)gui_load_gff_resref_00411e10(gff, nullptr, &value, &link_element, "Active", &ok, &fallback);
                    resref_assign(link, &value);                    // 005A3295..005A32A0
                }
                const U index = gui_load_gff_dword_00411c20(gff, nullptr, &link_element, "Index", &ok, 0);  // ..005A32B8
                wr(link, 0x10, index);                              // 005A32BF
                if (index > entries_now) ok = 0;                    // 005A32BD..005A32C6 (unsigned jbe)
            }
            if (ok == 0) { kotor_fastcall_005a27b0(d, nullptr); return ok; }   // 005A32FB..005A3312
        }
        if (ok == 0) { kotor_fastcall_005a27b0(d, nullptr); return ok; }       // 005A3317..005A3322
    }
    // 005A332A StartingList
    GffList starts;                                                 // [esp+64]
    (void)gff_list_004118c0(gff, nullptr, &starts, &top, "StartingList");   // 005A332A..005A333B
    const U start_count = gff_list_count_00411940(gff, nullptr, &starts);   // 005A3340..005A3347
    wr(d, 0x14, start_count);                                       // 005A3350
    if (start_count == 0) {                                         // 005A334E..005A3353
        wr(d, 0x18, start_count);                                   // 005A3355
    } else {
        void* array = live_operator_new(start_count * 0x14);        // 005A335A..005A3362
        if (array != nullptr) {                                     // 005A336E..005A337B
            for (U k = 0; k < start_count; ++k) {                   // 005A337D..005A33A2
                resref_default(at(array, k * 0x14));
                wr(array, k * 0x14 + 0x10, 0);
            }
        }
        wp(d, 0x18, array);                                         // 005A33AC..005A33B7
    }
    bool cleaned = false;
    if (ok == 0) {                                                  // 005A33BA..005A33C8
        cleaned = true;
    } else {
        for (U k = 0; ok != 0 && k < rd(d, 0x14); ++k) {            // 005A33E0..005A348F
            (void)gff_list_element_00411990(gff, nullptr, &link_element, &starts, k);  // 005A33ED..005A33FC
            void* start = at(rp(d, 0x18), k * 0x14);                // 005A3401..005A3414
            const U entries_now = rd(d, 0x04);                      // 005A3408 ebp
            {
                ResRef fallback; resref_from(&fallback, kEmpty);    // 005A340B..005A3416
                ResRef value;
                (void)gui_load_gff_resref_00411e10(gff, nullptr, &value, &link_element, "Active", &ok, &fallback);
                resref_assign(start, &value);                       // 005A343E..005A3449
            }
            const U index = gui_load_gff_dword_00411c20(gff, nullptr, &link_element, "Index", &ok, 0);  // ..005A3461
            wr(start, 0x10, index);                                 // 005A3468
            if (index > entries_now) ok = 0;                        // 005A3466..005A346F
        }
        if (ok == 0) cleaned = true;                                // 005A3495 / 005A3497..005A3499
    }
    if (cleaned) kotor_fastcall_005a27b0(d, nullptr);               // 005A349B..005A349D (and continues)
    // 005A34A2 StuntList
    GffList stunts;                                                 // [esp+9C]
    (void)gff_list_004118c0(gff, nullptr, &stunts, &top, "StuntList");  // 005A34A2..005A34B6
    const U stunt_count = gff_list_count_00411940(gff, nullptr, &stunts);   // 005A34BB..005A34C5
    wr(d, 0x48, stunt_count);                                       // 005A34D0
    void* stunt_array = nullptr;                                    // ebp
    if (stunt_count != 0) {                                         // 005A34CE..005A34D3
        void* memory = live_operator_new(stunt_count * 0x18 + 4);   // 005A34D5..005A34E0
        if (memory != nullptr) {                                    // 005A34EC..005A34F9
            wr(memory, 0, stunt_count);                             // 005A350C
            stunt_array = at(memory, 4);
            crt_eh_vector_constructor_006fb55f(stunt_array, 0x18, static_cast<I>(stunt_count),
                reinterpret_cast<void*>(&kotor_fastcall_0059ea40), reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));
        }
    }
    wp(d, 0x4c, stunt_array);                                       // 005A3525
    for (U k = 0; k < rd(d, 0x48); ++k) {                           // 005A351E..005A3617
        if (ok == 0) break;                                         // 005A3530..005A3536
        GffStruct stunt_element;                                    // [esp+40]
        (void)gff_list_element_00411990(gff, nullptr, &stunt_element, &stunts, k);  // 005A353C..005A354C
        {
            ExoString fallback(kEmpty);                             // 005A3551..005A355A [esp+44]
            RawStr value;                                           // [esp+30]
            (void)gui_load_gff_string_00411ec0(gff, nullptr, &value, &stunt_element, "Participant", &ok, &fallback);
            str_assign(at(rp(d, 0x4c), k * 0x18), &value);          // 005A358A..005A3598
        }                                                           // 005A359D / 005A35AE destructors
        {
            ResRef fallback; resref_from(&fallback, kEmpty);        // 005A35C2..005A35CE [esp+C0]
            ResRef value;                                           // [esp+D0]
            (void)gui_load_gff_resref_00411e10(gff, nullptr, &value, &stunt_element, "StuntModel", &ok, &fallback);
            resref_assign(at(rp(d, 0x4c), k * 0x18 + 8), &value);   // 005A35F9..005A3609
        }
    }
    return ok;                                                      // 005A361D..005A3624
}

// 0059EC90 CSWSDialog::CheckScript(CSWSObject*, CResRef const&) RET 8
int __fastcall kotor_fastcall_0059ec90(void*, void*, void* object, const void* script) {
    if (c2da_equal_text_004060d0(const_cast<void*>(script), nullptr, kEmpty) != 0) return 1;  // 0059ECAD..0059ECBD
    ExoString name;                                                 // 0059ECD4..0059ECD8
    c2da_ref_to_string_00405f70(const_cast<void*>(script), nullptr, &name);   // 0059ECDD..0059ECEC
    U result = frame_R4_005d0fc0(vm(), nullptr, &name, rd(object, 4), 1);    // 0059ECF1..0059ED06
    if (result == 1) {                                              // 0059ED0D..0059ED10
        U type = 0, value = 0;                                      // the two argument slots are reused as out cells
        if (kotor_fastcall_005d0fe0(vm(), nullptr, &type, &value) != 0 && type == 3)  // 0059ED12..0059ED30
            result = value != 0 ? 1u : 0u;                          // 0059ED32..0059ED3D
        else
            result = 0;                                             // 0059ED41
    }
    return static_cast<int>(result);                                // 0059ED43..0059ED65
}

// 0059ED70 CSWSDialog::RunScript(CSWSObject*, CResRef const&) RET 8. Returns the non-zero operator== result for an
// empty script; after running a script the original EAX is whatever ~CExoString left (all 8 original callers and
// the delivered callers ignore it); 0 is returned there.
int __fastcall kotor_fastcall_0059ed70(void*, void*, void* object, const void* script) {
    const int empty = c2da_equal_text_004060d0(const_cast<void*>(script), nullptr, kEmpty);  // 0059ED89..0059ED94
    if (empty != 0) return empty;                                   // 0059ED99..0059ED9B
    ExoString name;                                                 // 0059ED9D..0059EDA1
    c2da_ref_to_string_00405f70(const_cast<void*>(script), nullptr, &name);   // 0059EDA6..0059EDB5
    (void)frame_R4_005d0fc0(vm(), nullptr, &name, rd(object, 4), 1);         // 0059EDBA..0059EDCF
    return 0;
}

// 0059FC30 CSWSDialog::GetSpeaker(CSWSObject* object, CExoString const& tag) RET 8 -> CSWSObject* or 0
void* __fastcall kotor_fastcall_0059fc30(void* d, void*, void* object, void* tag) {
    U id = 0;                                                       // [esp+10]
    U i = 0;                                                        // edi
    for (; i < rd(d, 0x88); ++i) {                                  // 0059FC4D..0059FC84
        if (async_string_equal_005e5200(at(rp(d, 0x84), i * 0xc + 4), nullptr, tag) != 0) {  // 0059FC60..0059FC76
            id = rd(rp(d, 0x84), i * 0xc);                          // 0059FC88..0059FC94
            break;
        }
    }
    if (i == rd(d, 0x88)) {                                         // 0059FC9E..0059FCA4 not cached yet
        if (async_string_equal_005e5200(at(object, 0x18), nullptr, tag) != 0)   // 0059FCAA..0059FCB9
            id = rd(object, 4);                                     // 0059FCBB..0059FCBE
        else
            id = kotor_fastcall_004cb5c0(object, nullptr, tag, 1000.0f);   // 0059FCC4..0059FCD3 [0074744C]=1000.0f
        const U n = rd(d, 0x88) + 1;                                // 0059FCD7..0059FCE3
        void* old = rp(d, 0x84);                                    // 0059FCDD ebx
        void* memory = live_operator_new(n * 0xc + 4);              // 0059FCE4..0059FCEF
        void* fresh = nullptr;
        if (memory != nullptr) {                                    // 0059FCFD..0059FD03
            wr(memory, 0, n);                                       // 0059FD16
            fresh = at(memory, 4);
            crt_eh_vector_constructor_006fb55f(fresh, 0xc, static_cast<I>(n),
                reinterpret_cast<void*>(&kotor_fastcall_0059e990), reinterpret_cast<void*>(&kotor_fastcall_0059ea20));
        }
        wp(d, 0x84, fresh);                                         // 0059FD23
        for (U k = 0; k < rd(d, 0x88); ++k) {                       // 0059FD1D..0059FD6C
            str_assign(at(rp(d, 0x84), k * 0xc + 4), at(old, k * 0xc + 4));   // 0059FD40..0059FD4F
            wr(rp(d, 0x84), k * 0xc, rd(old, k * 0xc));             // 0059FD54..0059FD5D
        }
        str_assign(at(rp(d, 0x84), rd(d, 0x88) * 0xc + 4), tag);    // 0059FD6E..0059FD86
        wr(rp(d, 0x84), rd(d, 0x88) * 0xc, id);                     // 0059FD8B..0059FD9E
        wr(d, 0x88, rd(d, 0x88) + 1);                               // 0059FDA1..0059FDAA
        if (old != nullptr) {                                       // 0059FDA8..0059FDB0
            c2da_destroy_array_006fa852(old, 0xc, static_cast<I>(count_before(old)),
                                        reinterpret_cast<void*>(&kotor_fastcall_0059ea20));  // 0059FDB2..0059FDC1
            live_free(minus4(old));                                 // 0059FDC6..0059FDC7
        }
    }
    if (id == kInvalid) return nullptr;                             // 0059FDCF..0059FDD8
    void* found = nullptr;                                          // [esp+14]
    if (!get_object(id, &found)) return nullptr;                    // 0059FDDE..0059FDFF
    const float dx = static_cast<float>(static_cast<double>(rdf(found, 0x90)) - static_cast<double>(rdf(object, 0x90)));  // 0059FE0D..0059FE1B
    const float dy = static_cast<float>(static_cast<double>(rdf(found, 0x94)) - static_cast<double>(rdf(object, 0x94)));  // 0059FE1F..0059FE2B
    const float dz = static_cast<float>(static_cast<double>(rdf(found, 0x98)) - static_cast<double>(rdf(object, 0x98)));  // 0059FE2F..0059FE3B
    void* found_area = recovered_object_area_004cb120(found, nullptr);      // 0059FE3F
    void* object_area = recovered_object_area_004cb120(object, nullptr);    // 0059FE48
    bool in_range = false;
    if (found_area == object_area) {                                // 0059FE4D..0059FE4F
        const double dist2 = (static_cast<double>(dz) * dz + static_cast<double>(dy) * dy) + static_cast<double>(dx) * dx;  // 0059FE55..0059FE6F
        const double limit = static_cast<double>(1000.0f) * static_cast<double>(1000.0f);   // 0059FE71..0059FE77
        // 0059FE7D..0059FE84 fcompp st0=limit, st1=dist2; test ah,5; jnp -> out of range only when limit < dist2
        in_range = !(limit < dist2);
    }
    if (!in_range) {                                                // 0059FF21
        if (rd(found, 0x54) == rd(d, 0x80))                         // 0059FF21..0059FF2A
            kotor_fastcall_0059e9b0(d, nullptr, id);                // 0059FF2C..0059FF33
        return nullptr;                                             // 0059FF38
    }
    const U current = rd(found, 0x54);                              // 0059FE8A
    if (current != kInvalid && current != rd(d, 0x80)) {            // 0059FE8D..0059FE9A
        void* other = nullptr;
        if (get_object(current, &other) && rd(other, 0x54) == rd(found, 0x54)) return nullptr;  // 0059FE9C..0059FEC9
        kotor_fastcall_004cb7a0(found, nullptr, kInvalid);          // 0059FECB..0059FED2
    }
    if (rd(found, 0x54) == kInvalid) {                              // 0059FED7..0059FEDE
        kotor_fastcall_004cb7a0(found, nullptr, rd(d, 0x80));       // 0059FEE0..0059FEE9
        if (creature(found) != nullptr) {                           // 0059FEEE..0059FEF7
            w5_creature_action_manager_004f8770(creature(found), nullptr, 2);   // 0059FEF9..0059FF04
            recovered_set_activity_004f00a0(creature(found), nullptr, 4, 1, 0); // 0059FF09..0059FF18
        }
    }
    return found;                                                   // 0059FF1D
}

// 005A1050 CSWSDialog::BarkOneLine(entry index) RET 4
void __fastcall kotor_fastcall_005a1050(void* d, void*, U entry_index) {
    ExoString text;                                                 // 005A1073..005A1077 [esp+0C]
    ResRef sound;                                                   // 005A107C..005A1088 [esp+3C]
    resref_default(&sound);
    const U player_id = kotor_fastcall_004aea40(server(), nullptr); // 005A108D..005A1095 ebx
    const U off = entry_index * 0xb0;                               // 005A10A6 esi
    const U lang = kotor_fastcall_004ae810(server(), nullptr, player_id);   // 005A10B9..005A10BA
    (void)exo_locstring_get_005ea130(at(rp(d, 0x08), off + 0x08), nullptr, lang, &text, rd8(d, 0x78));  // ..005A10C7
    resref_assign(&sound, at(rp(d, 0x08), off + 0x34));             // 005A10CC..005A10DD
    if (rd(in_game_gui(), 0xb98) != 0) {                            // 005A10E2..005A10F8
        wr(in_game_gui(), 0xb98, 0);                                // 005A1102..005A1111
        U zero[3] = { 0, 0, 0 };                                    // 005A1123..005A1133 [esp+2C]
        kotor_m15_ingame_start_global_fade_0062abf0(in_game_gui(), nullptr, 1, 0.0f, 1.0f, zero);  // 005A1107..005A1147
    }
    if (gui_load_resref_is_valid_00406030(&sound, nullptr) == 0) {  // 005A114C..005A1157
        struct { ExoString text; ResRef sound; float length; } fetched;  // 005A1159..005A116B [esp+4C] 0x1C-byte Fetch result (length written by 0041E344/0041E350)
        resref_default(&fetched.sound);
        const U strref = rd(rp(d, 0x08), off + 0x0c);               // 005A1170..005A1173
        (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, &fetched, kByte0073ecd0);  // ..005A1191
        resref_assign(&sound, &fetched.sound);                      // 005A1196..005A11A4
    }                                                               // 005A11A9..005A11B2 ~CExoString
    if (str_compare_nc(&text, kEmpty) == 0) {   // 005A11B7..005A11C7 (text non-empty)
        void* speaker_object = kotor_fastcall_004ae750(server(), nullptr, player_id);   // 005A11CD..005A11D7
        if (speaker_object != nullptr) {                            // 005A11DC..005A11E0
            // 005A11E6/005A11FF: quirk kept - entry 0's Speaker (d+8 without the entry offset) is used
            U speaker_id;
            if (reinterpret_cast<kotor_reconstruction::StartupString*>(rp(d, 0x08))->BorrowedEquals005e5290(kEmpty) != 0)
                speaker_id = rd(d, 0x80);                           // 005A11F7
            else
                speaker_id = kotor_fastcall_004cb5c0(as_object(speaker_object), nullptr, rp(d, 0x08), 0.0f);  // 005A11FF..005A120E
            const U client_id = kotor_fastcall_005eda50(client(), nullptr, speaker_id);   // 005A1213..005A121D (ECX = [[7A39FC]+4])
            const U ignore = kotor_fastcall_004af040(server(), nullptr);    // 005A1222..005A122D
            kotor_fastcall_005edbb0(client(), nullptr, client_id, &text, &sound, ignore);   // 005A1232..005A1247
            if (recovered_log_conversations_00833a9c != 0 && player_id != kInvalid) {   // 005A124C..005A125F
                void* client_object = kotor_fastcall_005ed580(client(), nullptr, client_id);   // 005A1265..005A126F
                ExoString name;                                     // 005A1274..005A127A [esp+14]
                if (client_object != nullptr && vcall<void*>(client_object, 0x2c) != nullptr) {  // 005A127F..005A1296
                    void* c = vcall<void*>(client_object, 0x2c);    // 005A129C..005A12A3
                    if (c != nullptr) {                             // 005A12A3..005A12A5
                        {
                            RawStr full;                            // [esp+1C]
                            void* r = kotor_m15b_creature_stats_get_full_name_00647ce0(rp(c, 0x2f8), nullptr, &full);  // ..005A12B6
                            str_assign(&name, r);                   // 005A12BB..005A12C5
                        }                                           // 005A12CA..005A12D2
                        ExoString separator(": ");                  // 005A12D7..005A12E0 [00748ED0]
                        RawStr first;                               // [esp+2C]
                        auto* sum1 = GuiConcat005e5d10(reinterpret_cast<kotor_reconstruction::StartupString*>(&name), nullptr,
                            reinterpret_cast<kotor_reconstruction::StartupString*>(&first),
                            reinterpret_cast<const kotor_reconstruction::StartupString*>(&separator));   // 005A12E5..005A12F8
                        RawStr line;                                // [esp+24]
                        auto* sum2 = GuiConcat005e5d10(sum1, nullptr, reinterpret_cast<kotor_reconstruction::StartupString*>(&line),
                            reinterpret_cast<const kotor_reconstruction::StartupString*>(&text));        // 005A12FD..005A130E
                        feedback_append_0062b5c0(in_game_gui(), nullptr, sum2, 1, 0);  // 005A1313..005A1330
                    }                                               // 005A1335..005A1359 ~line, ~first, ~separator
                }
            }                                                       // 005A135E..005A1367 ~name
            void* owner = kotor_fastcall_004ae750(server(), nullptr, rd(d, 0x80));   // 005A136C..005A137B
            if (owner != nullptr && as_object(owner) != nullptr) {  // 005A1380..005A138D
                void* owner_object = as_object(owner);
                wr(d, 0x6c, 1);                                     // 005A1394
                kotor_fastcall_005a0a40(d, nullptr, owner_object, 1);   // 005A138F..005A139B RunEndConversationScript
            }
        }
    }
}                                                                   // 005A13A0..005A13AC ~text

// 005A3640 CSWSDialog::GetStartEntry(CSWSObject*) RET 4 -> entry index or -1
I __fastcall kotor_fastcall_005a3640(void* d, void*, void* object) {
    U i = 0;                                                        // ebp
    for (; i < rd(d, 0x14); ++i) {                                  // 005A3645..005A36D9
        void* start = at(rp(d, 0x18), i * 0x14);                    // edi offset
        void* speaker = at(rp(d, 0x08), rd(start, 0x10) * 0xb0);   // 005A3660..005A3675
        if (str_not_equal(speaker, kEmpty) != 0 &&   // 005A3670..005A367E
            exostring_compare_no_case_005e6450(at(object, 0x18), nullptr, speaker) == 0 &&   // 005A3680..005A369D
            kotor_fastcall_0059fc30(d, nullptr, object, at(rp(d, 0x08), rd(start, 0x10) * 0xb0)) == nullptr)  // ..005A36BC
            continue;
        if (kotor_fastcall_0059ec90(d, nullptr, object, start) != 0) break;   // 005A36BE..005A36CE
    }
    if (i >= rd(d, 0x14)) return -1;                                // 005A36D0..005A36E2 / 005A380E
    const U index = rd(at(rp(d, 0x18), i * 0x14), 0x10);           // 005A36E5..005A36F2
    void* entry = at(rp(d, 0x08), index * 0xb0);                    // 005A36F6..005A36FE
    const U links = rd(entry, 0xa8);                                // 005A3700
    auto release_players = [&]() {                                  // 005A3739..005A375A / 005A37EF..005A380C
        while (list_head(players(d)) != 0) live_free(remove_head(players(d)));
    };
    if (links == 0 && rd(d, 0x110) == 0) {                          // 005A3706..005A3712
        kotor_fastcall_005a1050(d, nullptr, index);                 // 005A3714..005A3717
        (void)kotor_fastcall_0059ed70(d, nullptr, object, at(rp(d, 0x08), rd(at(rp(d, 0x18), i * 0x14), 0x10) * 0xb0 + 0x10));
        release_players();
        return -1;
    }
    if (links != 1) return static_cast<I>(index);                   // 005A3766..005A3769
    void* reply = at(rp(d, 0x10), rd(rp(entry, 0xac), 0x10) * 0xb4);   // 005A376F..005A377B
    if (rd(reply, 0xa8) != 0 || rd(d, 0x110) != 0) return static_cast<I>(index);   // 005A3781..005A3798
    kotor_fastcall_005a1050(d, nullptr, index);                     // 005A379A..005A379D
    (void)kotor_fastcall_0059ed70(d, nullptr, object, at(rp(d, 0x08), rd(at(rp(d, 0x18), i * 0x14), 0x10) * 0xb0 + 0x10));  // ..005A37BA
    {
        void* e = at(rp(d, 0x08), rd(at(rp(d, 0x18), i * 0x14), 0x10) * 0xb0);   // 005A37BF..005A37CF
        (void)kotor_fastcall_0059ed70(d, nullptr, object, at(rp(d, 0x10), rd(rp(e, 0xac), 0x10) * 0xb4 + 0x10));  // ..005A37EA
    }
    release_players();
    return -1;
}

// ===============================================================================================================
// CSWSObject::StartDialog 004CF490 (thiscall(object id with, CExoString const& dialog, int flag) RET 0Ch)
// ===============================================================================================================
U __fastcall kotor_fastcall_004cf490(void* self, void*, U with, void* dialog_name, U flag) {
    void* player = kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr));  // 004CF4A8..004CF4DA [esp+14]
    kotor_fastcall_005edca0(client(), nullptr, 0);                  // 004CF4D0..004CF4DE SetDialogSent(0)
    kotor_fastcall_005edcb0(client(), nullptr, 0);                  // 004CF4E3..004CF4EC SetDialogModelsSent(0)
    if (rd(self, 0x34) != 0) {                                      // 004CF4F1..004CF4F4 already in a dialog
        if (rd(self, 0x214) != 0) set_global_dialog_state(0);       // 004CF8AA..004CF8C3
        return 1;                                                   // 004CF8C8
    }
    wr(self, 0x38, 0); wr(self, 0x3c, 0); wr(self, 0x40, 0); wr(self, 0x44, 0);   // 004CF4FE..004CF507
    wr(self, 0x48, 0); wr(self, 0x50, 0); wr(self, 0x74, 1);        // 004CF50A..004CF510
    wr(self, 0x60, 0); wr(self, 0x64, 0); wr(self, 0x70, 0); wr(self, 0x68, 0);   // 004CF517..004CF520
    ResRef resref;                                                  // [esp+2C]
    resref_default(&resref);                                        // 004CF523
    if (reinterpret_cast<kotor_reconstruction::StartupString*>(dialog_name)->BorrowedEquals005e5290(kEmpty) != 0) {  // 004CF528..004CF53A
        ResRef own;                                                 // [esp+3C] by-value result of vtable +80
        void* r = vcall<void*>(self, 0x80, static_cast<void*>(&own));   // 004CF53C..004CF545 GetDialogResref
        resref_assign(&resref, r);                                  // 004CF54B..004CF555
    } else {
        ResRef temp;                                                // [esp+4C]
        (void)async_ref_assign_string_004061f0(&resref, nullptr, &temp, dialog_name);   // 004CF55C..004CF566
    }
    if (c2da_equal_text_004060d0(&resref, nullptr, kEmpty) != 0) return 0;   // 004CF56B..004CF57B
    void* memory = live_operator_new(0xa0);                         // 004CF581..004CF586
    void* gff = memory != nullptr ? gui_load_gff_construct_00410630(memory, nullptr, 0x7ed, "DLG ", &resref) : nul();  // ..004CF5B4
    if (rd(gff, 0x94) == 0) {                                       // 004CF5B6..004CF5C5 (null gff faults as in the original)
        vcall<void>(gff, 0, 1u);                                    // 004CF614..004CF61A delete
        return 0;
    }
    void* dmemory = live_operator_new(0x12c);                       // 004CF5C7..004CF5CC
    void* dialog = dmemory != nullptr ? kotor_fastcall_005a0e80(dmemory, nullptr) : nul();   // 004CF5D8..004CF5ED
    wp(self, 0x34, dialog);                                         // 004CF5F8
    if (kotor_fastcall_005a2ae0(dialog, nullptr, gff, 1) == 0) {    // 004CF5EF..004CF602
        void* current = rp(self, 0x34);                             // 004CF604
        if (current != nullptr) vcall<void>(current, 0, 1u);        // 004CF607..004CF60F
        wr(self, 0x34, 0);                                          // 004CF611
        vcall<void>(gff, 0, 1u);                                    // 004CF614..004CF61A
        return 0;
    }
    vcall<void>(gff, 0, 1u);                                        // 004CF623..004CF629
    wr(rp(self, 0x34), 0x90, flag);                                 // 004CF62B..004CF632
    set_global_dialog_state(1);                                     // 004CF638..004CF64A
    {
        ExoString name;                                             // 004CF64F..004CF653 [esp+18]
        c2da_ref_to_string_00405f70(&resref, nullptr, &name);       // 004CF658..004CF669 (result unused)
    }                                                               // 004CF66E..004CF676
    if (rd8(rp(rp(client(), 4), 4), 0x6d) == 5) {                   // 004CF67B..004CF68D
        recovered_camera_reset_cell_007a2288 = 1;                   // 004CF68F
        kotor_fastcall_00641dc0(kotor_fastcall_005ed530(client(), nullptr), nullptr);  // 004CF699..004CF6A3
        kotor_fastcall_005eda60(client(), nullptr, 0, 1);           // 004CF6A8..004CF6B4 SetInputClass(0, 1)
    }
    void* target = nullptr;                                         // [esp+10]
    if (!get_object(with, &target)) return 0;                       // 004CF6B9..004CF6DF
    const U player_creature = rd(player, 0x38);                     // 004CF6E5..004CF6E9
    U8 controlled = kByte0073ecd0;                                  // 004CF6F5 bl
    void* pc = kotor_fastcall_004ae750(server(), nullptr, player_creature);   // 004CF6EC..004CF6FD
    if (pc != nullptr && creature(pc) != nullptr)                   // 004CF702..004CF711
        controlled = static_cast<U8>(vcall<U>(creature(pc), 0x88));  // 004CF713..004CF724
    void* record = live_operator_new(0xc);                          // 004CF72B..004CF72D (unchecked)
    wr(record, 0, rd(player, 4));                                   // 004CF732..004CF740
    wr(record, 4, with);                                            // 004CF742
    wr8(record, 8, controlled);                                     // 004CF745
    kotor_fastcall_004cc490(rp(self, 0x34), nullptr, record);       // 004CF748..004CF74C
    wr(rp(self, 0x34), 0x7c, with);                                 // 004CF751..004CF758
    wr8(rp(self, 0x34), 0x78, controlled);                          // 004CF75B..004CF75E
    wr(target, 0x54, rd(self, 4));                                  // 004CF761..004CF768
    if (creature(target) != nullptr) {                              // 004CF76B..004CF770
        w5_creature_action_manager_004f8770(creature(target), nullptr, 2);   // 004CF772..004CF77D
        recovered_set_activity_004f00a0(creature(target), nullptr, 4, 1, 0); // 004CF782..004CF790
    }
    {
        const U id = rd(self, 4);                                   // 004CF79E
        wr(in_game_gui(), 0x188, id);                               // 004CF795..004CF7A6
    }
    wr(self, 0x54, rd(self, 4));                                    // 004CF7AC..004CF7B3
    wr(self, 0x5c, 0);                                              // 004CF7B6
    if (creature(self) != nullptr) {                                // 004CF7AF..004CF7BE
        w5_creature_action_manager_004f8770(creature(self), nullptr, 2);     // 004CF7C0..004CF7CB
        recovered_set_activity_004f00a0(creature(self), nullptr, 4, 1, 0);   // 004CF7D0..004CF7DE
    }
    kotor_fastcall_0059f430(rp(self, 0x34), nullptr, self);         // 004CF7E3..004CF7E7
    const I entry = kotor_fastcall_005a3640(rp(self, 0x34), nullptr, self);  // 004CF7EC..004CF7F0
    const U sent = static_cast<U>(kotor_fastcall_004cd460(self, nullptr, entry));   // 004CF7F5..004CF7F8 (delivered D2)
    kotor_fastcall_0062d510(in_game_gui(), nullptr);                // 004CF7FD..004CF80F HideContainerGui
    if (sent != 0) {                                                // 004CF814..004CF816
        kotor_fastcall_005ee010(client(), nullptr);                 // 004CF818..004CF821
        return sent;
    }
    if (rd(in_game_gui(), 0xb98) != 0) {                            // 004CF82D..004CF840
        wr(in_game_gui(), 0xb98, 0);                                // 004CF842..004CF850
        U zero[3] = { 0, 0, 0 };                                    // 004CF867..004CF877 [esp+20]
        kotor_m15_ingame_start_global_fade_0062abf0(in_game_gui(), nullptr, 1, 0.0f, 1.0f, zero);  // 004CF856..004CF88B
    }
    set_global_dialog_state(0);                                     // 004CF890..004CF8A1
    return sent;                                                    // 004CF8A6
}

// ===============================================================================================================
// ACTION 255 BeginConversation 0052E9F0: CSWVirtualMachineCommands::ExecuteCommandBeginConversation(cmd, argc) RET 8
// BeginConversation(string sResRef = "", object oObjectToDialog = OBJECT_INVALID). New export for the VM command
// table in 0854 (Commands::resolve 0x0052e9f0 -> &recovered_vm_begin_conversation_0052e9f0).
// ===============================================================================================================
int __fastcall recovered_vm_begin_conversation_0052e9f0(void* self, void*, int, unsigned argc) {
    U with = kInvalid;                                              // 0052EA17 [esp+14]
    U result = 0;                                                   // 0052EA1F [esp+18]
    ExoString dialog;                                               // 0052EA0F..0052EA23 [esp+20]
    void* objects = object_array();                                 // 0052EA28..0052EA39 ebp
    U caller_orientation[3] = { 0, 0, 0 };                          // 0052EA3F..0052EA47 [esp+34]
    U target_orientation[3] = { 0, 0, 0 };                          // 0052EA4B..0052EA53 [esp+28]
    if (static_cast<int>(argc) > 0) {                               // 0052EA3B..0052EA57 (signed jle)
        if (recovered_vm_outer_pop_string_005d1080(vm(), nullptr, &dialog) == 0 ||   // 0052EA59..0052EA6B
            recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, &with) == 0)       // 0052EA6D..0052EA7F
            return -2001;                                           // 0052EA81..0052EAA8 (0xFFFFF82F)
    }
    const U gui_dialog = rd(in_game_gui(), 0xb4);                   // 0052EAAB..0052EAC2 edi
    const U action = kotor_fastcall_004af020(server(), nullptr);    // 0052EAB9..0052EAC8
    kotor_fastcall_004af010(server(), nullptr, 0);                  // 0052EACD..0052EADA
    bool started = false;
    if (!(action == 0 && gui_dialog != 0) && rd(self, 4) == 1) {    // 0052EADF..0052EAEF
        void* caller = nullptr;                                     // [esp+10]
        void* target = nullptr;                                     // [esp+14]
        if (kotor_fastcall_004d8230(objects, nullptr, rd(self, 8), &caller) == kStatusOk) {  // 0052EAF5..0052EB0B
            if (with == kInvalid) with = rd(caller, 0x17c);          // 0052EB11..0052EB26
            if (kotor_fastcall_004d8230(objects, nullptr, with, &target) == kStatusOk &&     // 0052EB2A..0052EB3D
                target != nullptr && caller != nullptr) {            // 0052EB43..0052EB55
                void* player = kotor_fastcall_004ae910(server(), nullptr, kotor_fastcall_004aea40(server(), nullptr));  // ..0052EB7E ebx
                // 0052EB78..0052EC04 / 0052EC04..0052EC8E: target first, then caller
                struct { void* object; U* orientation; } sides[2] = { { target, target_orientation }, { caller, caller_orientation } };
                for (auto& side : sides) {
                    if (creature(side.object) == nullptr || player == nullptr) continue;
                    if (rd(side.object, 4) != rd(player, 0x38))
                        w5_object_clear_all_actions_004ccd80(creature(side.object), nullptr, 1);
                    void* c = creature(side.object);
                    side.orientation[0] = rd(c, 0x9c); side.orientation[1] = rd(c, 0xa0); side.orientation[2] = rd(c, 0xa4);
                    void* last = recovered_last_update_object_00561820(player, nullptr, rd(side.object, 4));
                    if (last != nullptr) {
                        vcall<void>(creature(side.object), 0x7c, 0x2710u);
                        kotor_fastcall_004cc250(creature(side.object), nullptr, 0);
                        wr(last, 0x4c, 0x2710);
                    }
                }
                if (frame_LogIsEmpty_005e58a0(&dialog, nullptr) != 0)  // 0052EC8E..0052EC99
                    str_assign(&dialog, at(caller, 0x184));          // 0052EC9B..0052ECA6
                const U target_id = rd(target, 4);                  // 0052ECAB
                const U flag = rd(caller, 0x180);                   // 0052ECB7 ebp
                (void)kotor_fastcall_004ae910(server(), nullptr, target_id);   // 0052ECAE..0052ECBE (result discarded)
                const U player_creature = rd(player, 0x38);         // 0052ECC3 (null player faults as in the original)
                if (with == player_creature) {                      // 0052ECC6..0052ECCC
                    started = kotor_fastcall_004cf490(caller, nullptr, with, &dialog, flag) != 0;   // 0052ECCE..0052ED66
                } else if (rd(self, 8) == player_creature) {        // 0052ECDA..0052ECE3
                    started = kotor_fastcall_004cf490(target, nullptr, rd(self, 8), &dialog, flag) != 0;  // 0052ECE5..0052ED66
                } else if (creature(caller) != nullptr && rd(creature(caller), 0xa88) != 0) {   // 0052ECF0..0052ED0A
                    started = kotor_fastcall_004cf490(target, nullptr, rd(self, 8), &dialog, flag) != 0;  // 0052ED0C..0052ED1C
                } else {
                    started = kotor_fastcall_004cf490(caller, nullptr, with, &dialog, flag) != 0;   // 0052ED59..0052ED66
                }
                if (started) {
                    result = 1;                                     // 0052ED25 / 0052ED6F
                } else {
                    if (creature(target) != nullptr) {              // 0052ED32..0052ED3B / 0052ED79..0052ED82
                        w5_creature_set_lock_orientation_to_object_004f34a0(creature(target), nullptr, kInvalid, 0);
                        w1x_object_set_orientation_004cd170(creature(target), nullptr, target_orientation);
                    }
                    if (creature(caller) != nullptr) {              // 0052EDAC..0052EDB5
                        w5_creature_set_lock_orientation_to_object_004f34a0(creature(caller), nullptr, kInvalid, 0);
                        w1x_object_set_orientation_004cd170(creature(caller), nullptr, caller_orientation);
                    }
                }
            }
        }
    }
    if (!started) set_global_dialog_state(0);                       // 0052EDDF..0052EDF0
    if (recovered_vm_outer_push_integer_005d1010(vm(), nullptr, result) == 0) return -2000;   // 0052EDF5..0052EE31
    return 0;                                                       // 0052EE34..0052EE4C
}

}  // extern "C"
