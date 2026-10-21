// Lane worker "s2p-small": client server-to-player handlers (CSWCMessage, client dispatcher 0066A640), part B:
//   00654230 PopUpGUIPanel, 00665590 GuiTimingEvent (+00654A30 _Info), 00654900 SaveLoad
//   (+005F29F0 CClientExoAppInternal::ShutDownLoadGameLoadBar), 00654970 ShutDownServer (client side),
//   006631E0 GuiInfoPopup, 00651BA0 VoiceChat, 00651810 GuiContainerObject (+0062D4B0 CGuiInGame::ShowContainerGui,
//   006B8130 CSWGuiContainer::SetContainer, 005560E0 CItemRepository::ItemListGetItemObjectID),
//   00654AA0 GUICharacterSheet, 0064DEA0 GuiInventory, 00651560 ActiveItemPropertyUpdate.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Handler ABI: thiscall(CSWCMessage* this, BYTE minor) RET 4 -> extern "C" U __fastcall kotor_fastcall_<addr>(self, edx,
// minor) @12.
#include "b0bf16f6a8-claude_s2p_small_common.hpp"

using namespace s2p;

namespace kotor_reconstruction { struct StartupString; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
                                                                  kotor_reconstruction::StartupString* out,
                                                                  const kotor_reconstruction::StartupString* rhs); // CExoString::operator+

extern "C" {
// real providers (c19 map; none in the c19 trap census)
void __fastcall kotor_m15e_slot090_void0_004015a0(void* self, void*);                       // void_return (004015A0 ret)
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);            // CServerExoApp::GetPlaceableByGameObjectID
void* __fastcall kotor_fastcall_006b7ee0(void* entry, void*);                                // CSWGuiInGameItemEntry::CSWGuiInGameItemEntry
void __fastcall kotor_fastcall_006b6710(void* entry, void*, U id, U a, U b);                 // CSWGuiInGameItemEntry::SetItem RET 0Ch
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, U a, U b, U c);    // CSWGuiListBox::AddControls RET 10h
void* __fastcall kotor_fastcall_005edeb0(void* client, void*, void* out, U strref);          // CClientExoApp::GetGUIString RET 8
void __fastcall gui_load_text_params_set_strref_00415e50(void* params, void*, U strref);     // CSWGuiTextParams::SetStrRef
void __fastcall gui_load_text_params_set_text_00415e00(void* params, void*, const void* s);  // CSWGuiTextParams::SetText
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U a, U b);        // CSWGuiManager::AddPanel RET 0Ch
void __fastcall kotor_fastcall_005edc20(void* client, void*, U a, U b, U c);                 // CClientExoApp::SetPausedByCombat RET 0Ch

// exports of this file besides the handler table entries
U __fastcall kotor_fastcall_00654a30(void* self, void*);                                     // GuiTimingEvent_Info RET 0
void __fastcall kotor_fastcall_005f29f0(void* internal, void*);                              // ShutDownLoadGameLoadBar RET 0
void __fastcall kotor_fastcall_0062d4b0(void* gui, void*, U id, U flag);                     // CGuiInGame::ShowContainerGui RET 8
void __fastcall kotor_fastcall_006b8130(void* container, void*, U id, U flag);               // CSWGuiContainer::SetContainer RET 8
U __fastcall kotor_fastcall_005560e0(void* repository, void*, I index);                      // ItemListGetItemObjectID RET 4
}

namespace {
inline U overflow(void* m) { return kotor_fastcall_004d6230(m, nullptr, 1); }
inline U underflow(void* m) { return kotor_fastcall_004d6240(m, nullptr, 1); }
inline void* ingame_gui() { return frame_R4_005ed690(client(), nullptr); }
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// 00654230 CSWCMessage::HandleServerToPlayerPopUpGUIPanel(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00654230(void* self, void*, U minor) {
    switch (static_cast<U8>(minor)) {                                                   // 0065423E..00654257
    case 1: {                                                                           // 006542B5
        ExoString text;                                                                 // 006542B5..006542B9 [esp+4]
        if (overflow(self) != 0) return 0;                                              // 006542BE..006542D1 -> 0065435B
        (void)kotor_fastcall_004d66f0(self, nullptr);                                   // 006542D7..006542D9 ReadBOOL
        (void)kotor_fastcall_004d66f0(self, nullptr);                                   // 006542DE..006542E0 ReadBOOL
        (void)kotor_fastcall_004d6780(self, nullptr, 0x20);                             // 006542E5..006542E9
        {
            StrTemp t;                                                                  // [esp+14h]
            str_assign(&text, kotor_fastcall_004d67e0(self, nullptr, &t, 0x20));        // 006542EE..00654306
            frame_LogStringDtor_005e5c20(&t, nullptr);                                  // 0065430B..00654314
        }
        if (overflow(self) != 0) return 0;                                              // 00654319..00654324
        if (underflow(self) != 0) return 0;                                             // 00654326..00654331
        return 1;                                                                       // 00654333..00654344 (~text)
    }
    case 2:                                                                             // 00654289
        kotor_m15e_slot090_void0_004015a0(ingame_gui(), nullptr);                       // 00654289..00654299 (empty member)
        return 1;                                                                       // 0065429E
    case 3: {                                                                           // 0065425D
        StrTemp t;                                                                      // [esp+0Ch]
        (void)kotor_fastcall_004d67e0(self, nullptr, &t, 0x20);                         // 0065425D..00654264 (discarded)
        frame_LogStringDtor_005e5c20(&t, nullptr);                                      // 00654269..0065426D
        return 1;                                                                       // 00654272
    }
    default:
        return 1;                                                                       // 00654257 -> 00654344
    }
}

// ---------------------------------------------------------------------------------------------------------------
// 00654A30 CSWCMessage::HandleServerToPlayerGuiTimingEvent_Info() RET 0
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00654a30(void* self, void*) {
    if (kotor_fastcall_004d66f0(self, nullptr) != 0) {                                  // 00654A31..00654A3A
        (void)kotor_fastcall_004d6780(self, nullptr, 0x20);                             // 00654A3C..00654A40
        (void)kotor_fastcall_004d6720(self, nullptr, 8);                                // 00654A45..00654A49
    }
    if (overflow(self) != 0) return 0;                                                  // 00654A4E..00654A5A
    if ((rd8(kotor_fastcall_005ed700(client(), nullptr), 0) & 0x10) == 0) return 1;    // 00654A5C..00654A6C
    void* gui = ingame_gui();                                                           // 00654A6E..00654A77
    if (gui == nullptr) return 0;                                                       // 00654A7C..00654A7E
    return rd(gui, 0x108) != 0 ? 1u : 0u;                                               // 00654A80..00654A92
}

// ---------------------------------------------------------------------------------------------------------------
// 00665590 CSWCMessage::HandleServerToPlayerGuiTimingEvent(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00665590(void* self, void*, U minor) {
    if (overflow(self) != 0) return 0;                                                  // 00665591..0066559C
    if (static_cast<U8>(minor) == 1) (void)kotor_fastcall_00654a30(self, nullptr);      // 0066559E..006655A8 (result unused)
    if (overflow(self) != 0) return 0;                                                  // 006655AD..006655B8
    if (underflow(self) != 0) return 0;                                                 // 006655BA..006655C5
    return 1;                                                                           // 006655C7
}

// ---------------------------------------------------------------------------------------------------------------
// 005F29F0 CClientExoAppInternal::ShutDownLoadGameLoadBar() RET 0
// +18Ch load-bar active; CAppManager +14h load-bar block {+0, +4, +14h} cleared (the pointer is re-read).
// ---------------------------------------------------------------------------------------------------------------
extern "C" void __fastcall kotor_fastcall_005f29f0(void* internal, void*) {
    if (rd(internal, 0x18c) == 0) return;                                               // 005F29F0..005F29FA
    wr(rp(app(), 0x14), 0x14, 0);                                                       // 005F29FC..005F2A04
    void* bar = rp(app(), 0x14);                                                        // 005F2A07..005F2A0C
    wr(bar, 0, 0);                                                                      // 005F2A0F
    wr(bar, 4, 0);                                                                      // 005F2A11
    wr(internal, 0x18c, 0);                                                             // 005F2A14
}

// ---------------------------------------------------------------------------------------------------------------
// 00654900 CSWCMessage::HandleServerToPlayerSaveLoad(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00654900(void* self, void*, U minor) {
    if (static_cast<U8>(minor) != 1) return 1;                                          // 00654900..0065490B -> 00654954
    const U8 kind = kotor_fastcall_004d6720(self, nullptr, 8);                          // 0065490D..00654918 bl
    const U done = kotor_fastcall_004d6780(self, nullptr, 4);                           // 00654914..00654923 edi (4 bits)
    if (overflow(self) != 0) return 0;                                                  // 0065491F..0065492C
    if (underflow(self) != 0) return 0;                                                 // 0065492E..00654939
    if (done != 0 && kind != 2) kotor_fastcall_005f29f0(rp(client(), 4), nullptr);      // 0065493B..0065494F
    return 1;                                                                           // 00654954..0065495C
}

// ---------------------------------------------------------------------------------------------------------------
// 00654970 CSWCMessage::HandlePlayerToServerShutDownServer(BYTE minor) RET 4 (client-side table entry, major 2Eh)
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00654970(void* self, void*, U /*minor*/) {
    ExoString unused;                                                                   // 0065498B..0065498F [esp+4]
    ResRef unused_ref;                                                                  // [esp+0Ch]
    resref_default(&unused_ref);                                                        // 00654994..006549A0
    if (overflow(self) != 0) return 0;                                                  // 006549A5..006549B8 (~unused)
    (void)kotor_fastcall_004d6780(self, nullptr, 0x20);                                 // 006549BA..006549BE
    if (overflow(self) != 0) return 0;                                                  // 006549C3..006549CE
    if (underflow(self) != 0) return 0;                                                 // 006549D0..006549DB
    return 1;                                                                           // 006549DD..006549EE (~unused)
}

// ---------------------------------------------------------------------------------------------------------------
// 006631E0 CSWCMessage::HandleServerToPlayerGuiInfoPopupMessage(BYTE minor) RET 4
// (jump table 00663204: minor 2 -> 1; minors 3..6 and all others -> 0; no payload read)
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_006631e0(void* /*self*/, void*, U minor) {
    return static_cast<U8>(minor) == 2 ? 1u : 0u;                                       // 006631E0..006631FE
}

// ---------------------------------------------------------------------------------------------------------------
// 00651BA0 CSWCMessage::HandleServerToPlayerVoiceChatMessage(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00651ba0(void* self, void*, U minor) {
    if (static_cast<U8>(minor) != 1) return 1;                                          // 00651BA0..00651BAA -> 00651BF8
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 00651BAC..00651BB5 edi
    const U8 entry = kotor_fastcall_004d6720(self, nullptr, 8);                         // 00651BB1..00651BC0 bl
    if (overflow(self) != 0) return 0;                                                  // 00651BBC..00651BC9
    if (underflow(self) != 0) return 0;                                                 // 00651BCB..00651BD6
    void* creature = ptr(frame_R4_005ed5b0(client(), nullptr, id));                    // 00651BD8..00651BE1
    if (creature != nullptr)                                                            // 00651BE6..00651BE8
        (void)recovered_client_creature_play_sound_set_sound_00611470(creature, nullptr, entry, 1); // 00651BEA..00651BF3
    return 1;                                                                           // 00651BF8
}

// ---------------------------------------------------------------------------------------------------------------
// 005560E0 CItemRepository::ItemListGetItemObjectID(int index) RET 4 (+0Ch ids, +10h count)
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_005560e0(void* repository, void*, I index) {
    if (index < 0 || index >= rdi(repository, 0x10)) return kInvalid;                   // 005560E0..005560EB, 005560F6
    return rd(rp(repository, 0xc), static_cast<U>(index) * 4);                          // 005560ED..005560F3
}

// ---------------------------------------------------------------------------------------------------------------
// 006B8130 CSWGuiContainer::SetContainer(OBJECT_ID container, BYTE flag) RET 8
// Builds one CSWGuiInGameItemEntry (0x39C) per item of the server placeable's repository, adds them to the item
// listbox (+7F0h), sets the title (strref 189h when the flag byte is nonzero, else 18Ah), the two button labels
// (strref 968Eh) and the "<GUI 0BB0Ch> <GUI 968Fh>" caption, then refreshes. Original quirks kept: the server
// placeable lookup result is not checked; the entry pointer table is a stack CExoArrayList freed at exit (entries are
// owned by the listbox); the container argument slot is reused for the client app pointer.
// ---------------------------------------------------------------------------------------------------------------
namespace {
struct PtrList {                                                                        // [esp+3Ch] {data, size, capacity}
    void** data = nullptr;
    U size = 0;
    U capacity = 0;
    PtrList() = default;
    PtrList(const PtrList&) = delete;
    PtrList& operator=(const PtrList&) = delete;
    ~PtrList() { live_free(data); }                                                     // 006B83E2..006B83EF (_free)
};
static_assert(sizeof(PtrList) == 12, "CExoArrayList<CSWGuiControl*>");
}  // namespace

extern "C" void __fastcall kotor_fastcall_006b8130(void* container, void*, U id, U flag) {
    wr(container, 0x64, id);                                                            // 006B8151..006B8158
    wr8(container, 0x68, static_cast<U8>(flag));                                        // 006B8148, 006B815B
    PtrList entries;                                                                    // 006B815E..006B8166
    void* const extra = rp(container, 0xabc);                                           // 006B816A
    void* const arg_a = at(extra, 0xd0);                                                // 006B8170..006B8176 [esp+1Ch]
    void* const arg_b = at(extra, 0x5c);                                                // 006B817A..006B8198 [esp+18h]
    void* const arg_c = at(extra, 0x140);                                               // 006B817D..006B8182 [esp+10h]
    void* const server_app = server();                                                  // 006B8186..006B818B ebx
    void* const client_app = client();                                                  // 006B818E..006B819C (-> container arg slot)
    void* const placeable = recovered_placeable_lookup_004ae7a0(
        server_app, nullptr, kotor_fastcall_004aea30(server_app, nullptr, id));         // 006B8191..006B81A8
    void* const repository = rp(placeable, 0x36c);                                      // 006B81AD ebx
    const U count = rd(repository, 0x10);                                               // 006B81B0 ebp
    if (count == 0) {                                                                   // 006B81B6..006B81B8
        if (entries.data != nullptr) {                                                  // 006B81BA..006B81C0
            live_free(entries.data);                                                    // 006B81C2..006B81C8
            entries.data = nullptr;                                                     // 006B81CB
            entries.capacity = 0;                                                       // 006B81CF
        }
    } else {
        list_allocate(&entries, count);                                                 // 006B81D5..006B81DA
    }
    entries.size = count;                                                               // 006B81EC
    void* const listbox = at(container, 0x7f0);                                         // 006B81F0 ebp
    const U width = rd(container, 0xa84) - (static_cast<U>(rd8(container, 0xab0)) << 1); // 006B81DF..006B81FA (-> flag slot)
    for (U i = 0; i < rd(repository, 0x10); ++i) {                                      // 006B81FE..006B8203, 006B8291..006B8297
        const U item = kotor_fastcall_005eda50(client_app, nullptr,
                                               kotor_fastcall_005560e0(repository, nullptr, static_cast<I>(i))); // 006B8210..006B8227
        void* memory = live_operator_new(0x39c);                                        // 006B8222..006B8233
        void* entry = nullptr;
        if (memory != nullptr) {                                                        // 006B8237..006B823E
            try {
                entry = kotor_fastcall_006b7ee0(memory, nullptr);                       // 006B8240..006B8242
            } catch (...) {
                live_free(memory);                                                      // EH state 1: operator delete
                throw;
            }
        }
        entries.data[i] = entry;                                                        // 006B824B..006B824F
        vcall<void>(entries.data[i], 0xa0, arg_a, arg_b, arg_c, width, 0u);             // 006B8252..006B8276
        kotor_fastcall_006b6710(entries.data[i], nullptr, item, 0, 0);                  // 006B827C..006B828C SetItem
    }
    kotor_fastcall_0041c1d0(listbox, nullptr, &entries, 1, 0, 0);                       // 006B829D..006B82AA AddControls
    const U title = rd8(container, 0x68) != 0 ? 0x189u : 0x18au;                        // 006B82AF..006B82C1 (neg/sbb/add)
    ExoString caption;                                                                  // [esp+34h]
    {
        ExoString space(" ");                                                           // 006B82B4..006B82C7 [esp+2Ch] (0073D5D8)
        StrTemp first, second, joined;                                                  // [esp+14h] / [esp+1Ch] / [esp+24h]
        void* const gui_a = kotor_fastcall_005edeb0(client(), nullptr, &first, 0x968f);  // 006B82CC..006B82E9 ebx
        void* const gui_b = kotor_fastcall_005edeb0(client(), nullptr, &second, 0xbb0c); // 006B82EB..006B8303
        void* const left = GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(gui_b), nullptr,
                                             reinterpret_cast<kotor_reconstruction::StartupString*>(&joined),
                                             reinterpret_cast<const kotor_reconstruction::StartupString*>(&space)); // 006B8308..006B8319
        (void)GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(left), nullptr,
                                reinterpret_cast<kotor_reconstruction::StartupString*>(&caption),
                                static_cast<const kotor_reconstruction::StartupString*>(gui_a)); // 006B831E..006B832B
        frame_LogStringDtor_005e5c20(&joined, nullptr);                                 // 006B8330..006B8339
        frame_LogStringDtor_005e5c20(&second, nullptr);                                 // 006B833E..006B8347
        frame_LogStringDtor_005e5c20(&first, nullptr);                                  // 006B834C..006B8355
    }                                                                                   // 006B835A..006B8363 ~space
    gui_load_text_params_set_strref_00415e50(at(container, 0x158), nullptr, title);     // 006B8368..006B836F
    gui_load_text_params_set_strref_00415e50(at(container, 0x3d8), nullptr, 0x968e);    // 006B8374..006B837F
    gui_load_text_params_set_strref_00415e50(at(container, 0xc3c), nullptr, 0x968e);    // 006B8384..006B838F
    gui_load_text_params_set_text_00415e00(at(container, 0xfc4), nullptr, &caption);    // 006B8394..006B839F
    wr(container, 0x6c, rd(container, 0x6c) | 1);                                       // 006B83A4
    wr(listbox, 0x2bc, rd(listbox, 0x2bc) | 0x200);                                     // 006B83A8
    vcall<void>(listbox, 0x88, 0u);                                                     // 006B83B2..006B83B9
    vcall<void>(container, 0x8, static_cast<void*>(nullptr), 0u);                       // 006B83BF..006B83C7
    vcall<void>(container, 0x8, listbox, 0u);                                           // 006B83CA..006B83D1
}                                                                                       // 006B83D4..006B83EF ~caption, _free(list)

// ---------------------------------------------------------------------------------------------------------------
// 0062D4B0 CGuiInGame::ShowContainerGui(OBJECT_ID container, BYTE flag) RET 8
// ---------------------------------------------------------------------------------------------------------------
extern "C" void __fastcall kotor_fastcall_0062d4b0(void* gui, void*, U id, U flag) {
    if (rd(gui, 0x108) == 0) return;                                                    // 0062D4B3..0062D4BB
    kotor_fastcall_006b8130(rp(gui, 0x54), nullptr, id, flag);                          // 0062D4BD..0062D4CA
    kotor_fastcall_0040bc70(rp(gui, 0x38), nullptr, rp(gui, 0x54), 1, 1);               // 0062D4CF..0062D4DA AddPanel
    kotor_fastcall_005eda60(client(), nullptr, 2, 1);                                   // 0062D4DF..0062D4EB SetInputClass
    kotor_fastcall_005edc20(client(), nullptr, 1, 3, 0);                                // 0062D4F0..0062D4FF SetPausedByCombat
}

// ---------------------------------------------------------------------------------------------------------------
// 00651810 CSWCMessage::HandleServerToPlayerGuiContainerObjectMessage(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00651810(void* self, void*, U minor) {
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 00651815..0065181E edi
    const U flag = kotor_fastcall_004d6780(self, nullptr, 0x20);                        // 0065181A..00651829 ebx
    if (overflow(self) != 0) return 0;                                                  // 00651825..00651832
    if (underflow(self) != 0) return 0;                                                 // 00651834..0065183F
    if (static_cast<U8>(minor) == 1 && id != kInvalid)                                  // 00651841..0065184E
        kotor_fastcall_0062d4b0(ingame_gui(), nullptr, id, flag);                       // 00651850..00651861
    return 1;                                                                           // 00651866
}

// ---------------------------------------------------------------------------------------------------------------
// 00654AA0 CSWCMessage::HandleServerToPlayerGUICharacterSheet(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00654aa0(void* self, void*, U minor) {
    if (overflow(self) != 0) return 0;                                                  // 00654AA1..00654AAC
    if (static_cast<U8>(minor) == 2) {                                                  // 00654AAE..00654AB6
        (void)kotor_fastcall_00692630(self, nullptr);                                   // 00654AB8..00654ABA (discarded)
        if (overflow(self) != 0) return 0;                                              // 00654ABF..00654ACA
        (void)ingame_gui();                                                             // 00654ACC..00654AD4 (result unused)
    }
    if (overflow(self) != 0) return 0;                                                  // 00654AD9..00654AE4
    if (underflow(self) != 0) return 0;                                                 // 00654AE6..00654AF1
    return 1;                                                                           // 00654AF3
}

// ---------------------------------------------------------------------------------------------------------------
// 0064DEA0 CSWCMessage::HandleServerToPlayerGuiInventory(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_0064dea0(void* self, void*, U minor) {
    if (static_cast<U8>(minor) != 1) return 1;                                          // 0064DEA0..0064DEA8 -> 0064DED1
    (void)kotor_fastcall_004d6720(self, nullptr, 8);                                    // 0064DEAA..0064DEAC
    if (overflow(self) != 0) return 0;                                                  // 0064DEB1..0064DEBC
    if (underflow(self) != 0) return 0;                                                 // 0064DEBE..0064DEC9
    return 1;                                                                           // 0064DED1
}

// ---------------------------------------------------------------------------------------------------------------
// 00651560 CSWCMessage::HandleServerToPlayerActiveItemPropertyUpdate(BYTE minor) RET 4
// Payload: item id, active-property bit mask (byte), uses mask (byte), then one uses byte per set uses bit (0..7).
// Original frame quirk reproduced through an explicit model of the 0Ch-byte local frame (sub esp,0Ch at L): the active
// mask byte is L+3 ([esp+13h] while the second ReadBYTE argument is pushed, and again after `push edi`), the uses bytes
// are L+4+i (i = 0..7); the active loop tests (1 << (i & 31)) in a byte (dl); the uses loop runs i = 0..3Fh testing
// (1 << (i & 31)) against the zero-extended uses mask, so for i = 32..39 it reads L+4+i = L+24h..L+2Bh, beyond the
// frame: the caller's (dispatcher 0066A640) stack. There L+28h..L+2Bh is the dispatcher's minor dword (the value
// passed in `minor`), L+27h its major byte (17h for this handler) and L+24h..L+26h dispatcher bytes the original
// never initialises (indeterminate in the original; modelled as 0). Original-game bug (out-of-frame read), reported
// for issue #23.
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00651560(void* self, void*, U minor) {
    U8 frame[0x2c] = {};                                                                // L+0 .. L+2Bh (see above)
    std::memcpy(frame + 0x10, &minor, 4);                                               // L+10h argument slot
    frame[0x27] = 0x17;                                                                 // caller [esp+13h] major byte
    std::memcpy(frame + 0x28, &minor, 4);                                               // caller [esp+14h] minor dword
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 00651568..00651571 ebp
    frame[3] = kotor_fastcall_004d6720(self, nullptr, 8);                               // 0065156D..0065157C [esp+13h] = L+3
    const U8 uses_mask = kotor_fastcall_004d6720(self, nullptr, 8);                     // 00651578..00651589 bl
    if (overflow(self) != 0) return 0;                                                  // 00651585..00651599
    if (static_cast<U8>(minor) != 1) return 1;                                          // 0065159F..006515A5 -> 00651662
    const U mask = uses_mask;                                                           // 006515AB..006515B0 (-> L+10h)
    std::memcpy(frame + 0x10, &mask, 4);
    for (I i = 0; i < 8; ++i) {                                                         // 006515AE..006515E9
        if ((mask & (1u << (static_cast<U>(i) & 31))) == 0) continue;                   // 006515B4..006515BF
        const U8 uses = kotor_fastcall_004d6720(self, nullptr, 8);                      // 006515C1..006515CE
        if (overflow(self) != 0) return 0;                                              // 006515CA..006515D7 -> 00651671
        frame[4 + i] = uses;                                                            // 006515DD [esp+edi+14h]
    }
    void* item = kotor_m15_client_get_item_by_id_005ed5a0(client(), nullptr, id);      // 006515EB..006515FA edi
    if (item == nullptr) return 1;                                                      // 006515FC..006515FE
    for (I i = 0; i < rdi(item, 0x120); ++i) {                                          // 00651600..0065160A, 00651632..0065163B
        void* property = recovered_client_item_get_active_property_006466a0(item, nullptr, i); // 00651610..00651613
        if (property != nullptr) {                                                      // 00651618..0065161A
            const U8 bit = static_cast<U8>(1u << (static_cast<U>(i) & 31));             // 0065161C..00651623 (dl)
            wr(property, 0xc, (bit & frame[3]) != 0 ? 1u : 0u);                         // 00651625..0065162F [esp+13h] = L+3
        }
    }
    for (I i = 0; i < 0x40; ++i) {                                                      // 0065163D..00651660
        if ((mask & (1u << (static_cast<U>(i) & 31))) == 0) continue;                   // 00651640..0065164B
        recovered_client_item_set_property_uses_left_006469b0(item, nullptr, i, frame[4 + i]); // 0065164D..00651657
    }
    return 1;                                                                           // 00651662
}
