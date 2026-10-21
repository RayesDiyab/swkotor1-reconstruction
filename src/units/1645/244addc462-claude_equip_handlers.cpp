// Lane W4 GUI successor (worker "equip-handlers"): CSWGuiInGameEquip event handlers registered by the constructor
// (006BA980) plus the equip-panel helpers they reach that had no provider in the candidate map.
//   006B8EB0 OnSelectSlot        thiscall(CSWGuiControl*) RET 4
//   006B9160 OnOKPressed         thiscall(CSWGuiControl*) RET 4
//   006BA820 OnChangeCharacter   thiscall(CSWGuiControl*) RET 4
//   006B60F0 OnSwitchLeft        thiscall(CSWGuiControl*) RET 4
//   006B6370 OnSwitchRight       thiscall(CSWGuiControl*) RET 4
// helpers reconstructed here (no candidate provider):
//   006B7E30 CloseDescription    thiscall() RET
//   006B7590 SetCharacter        thiscall(CSWCCreature*) RET 4
//   006B5600 GetIcon             thiscall(CExoString* out, int slot) RET 8, returns out
//   005EDF80 CClientExoApp::ChangeCharacterToNextLivingPartyMember  thiscall(int,int,int) RET 0Ch -> [this+4] 005F7960
// Transcribed from the original linear sweep (claude-runs/tools/win.py); each statement carries its original PCs.
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable
namespace kotor_input_required {
// Existing provider (change_character_to_next_living_party_member_005f7960.obj). Its accepted C++ import is void,
// but every exit materializes the original 0/1 in EAX (verified in candidate kotor.exe 00743750: all eleven exits
// set EAX before the shared epilogue at 00743A41). Called below through the int-returning fastcall type.
void __fastcall change_character_to_next_living_party_member_005f7960(void* self, void*, U32, U32, U32);   // 005F7960
}

extern "C" {
// ---- existing providers (candidate map symbols)
void* __fastcall frame_GetServerArea_00605640(void* area, void*);                                 // CSWCArea::GetServerArea
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);         // CSWCCreature::GetServerCreature
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, U32 id);                      // CServerExoApp::GetItemByGameObjectID
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                       // CSWItem::GetBaseItem
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, void* object, U32, U32); // CSWGuiMessageBox::SetCallback
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, std::uint32_t, std::uint32_t); // CSWGuiManager::AddPanel
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, I32 index, I32 flag); // CSWGuiListBox::SetSelectedControl
void __fastcall show_tutorial_window_005edf40(void* app, void*, int, unsigned, unsigned, int);    // CClientExoApp::ShowTutorialWindow
void __fastcall kotor_fastcall_0040a140(void* manager, void*, std::uint32_t sound);               // CSWGuiManager::PlayGuiSound
void* __fastcall kotor_fastcall_00635460(void* party, void*);                                     // CSWParty::GetPlayerCharacter
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);             // CResRef::CResRef(const CExoString&)
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, I32) noexcept(false); // SetFillImage
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, U32 id);                   // CClientExoApp::GetGUIString
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                          // CClientExoApp::GetPlayerCreature
U32 __fastcall kotor_fastcall_005eda50(void* app, void*, U32 id);                                // CClientExoAppInternal::ServerToClientObjectId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                     // CClientExoApp::GetSWParty
void* __fastcall recovered_server_info_004aec90(void* server, void*);                            // CServerExoApp::GetServerInfo
U32 __fastcall kotor_fastcall_005636b0(void* table, void*, I32 index);                           // CSWPartyTable::GetIsNPCAvailable
U32 __fastcall recovered_npc_object_00564700(void* table, void*, I32 index, U32, U32);           // CSWPartyTable::GetNPCObject
// ---- equip-panel helpers (panels_equip.cpp)
void __fastcall kotor_fastcall_006b5b60(void* self, void*, U32 show);                            // UpdatePortraits
void __fastcall kotor_fastcall_006b7680(void* self, void*, U32 show);                            // ShowDescription
// ---- owned by worker "equip-items" (claude_equip_items.cpp)
void __fastcall kotor_fastcall_006b9470(void* self, void*, void* control);                       // OnEnterSlot, thiscall(control) RET 4
// ---- defined in this file
void __fastcall kotor_fastcall_006b7e30(void* self, void*);
void __fastcall kotor_fastcall_006b7590(void* self, void*, void* character);
void* __fastcall kotor_fastcall_006b5600(void* self, void*, void* out, I32 slot);
U32 __fastcall kotor_fastcall_005edf80(void* app, void*, U32, U32, U32);
}

namespace {
// .rdata 00756560: 9-entry equip slot table, stride 14h. Column +0 = slot mask, column +8 = icon slot name.
const U32 slot_masks[9] = {0x20, 0x10, 0x1, 0x80, 0x100, 0x2, 0x8, 0x200, 0x400};                 // [00756560 + 14h*i]
const char* const slot_icons[9] = {"weap_l", "weap_r", "head", "forearm_l", "forearm_r", "armor", "hands", "implant", "belt"}; // [00756568 + 14h*i]
const char* const empty_text = "";                                                              // 0073D71C

void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
void fill(void* self, U32 params, const void* ref) { gui_load_border_params_set_fill_00414c00(at(self, params), nullptr, ref, 0); }
// creature vtable +148h GetPortrait(CResRef* out, int 1) -> CResRef*
void* portrait(void* creature, void* buf) { return vcall<void*, void*, U32>(creature, 0x148, buf, 1); }

// Shared message-box sequence of OnSelectSlot (006B8EF1..006B8F2F and its three copies).
void refuse(void* self, U32 strref) {
    void* box = lp(pt(frame_R4_005ed690(client_app(), nullptr)), 0x98);                         // GetInGameGui()->+98h
    kotor_fastcall_00627130(box, nullptr, 0);                                                    // SetAllowCancel(0)
    gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, 0, 0);                      // SetCallback(0,0,0)
    vcall<void, U32>(box, 0x6C, strref);                                                         // [vtbl+6Ch](strref)
    kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, box, 1, 1);                                 // manager->AddPanel(box,1,1)
}

// One NPC candidate of the OnSwitchLeft/OnSwitchRight scans (006B6168..006B6204 and the three copies).
// Returns 1 when the slot was taken (found), 0 otherwise.
U32 switch_candidate(void* self, I32 slot, U32 first_params, U32 second_params, U32 index_field) {
    if (slot == static_cast<std::int8_t>(lb(self, 0x4274))) return 0;                            // movsx [+4274]; cmp; je
    if (kotor_fastcall_005636b0(party_table(), nullptr, slot) == 0) return 0;                    // GetIsNPCAvailable(slot)
    void* client = client_app();                                                                 // edi = [[7A39FC]+4]
    const U32 object = recovered_npc_object_00564700(party_table(), nullptr, slot, 0, 1);        // GetNPCObject(slot,0,1)
    void* creature = pt(frame_R4_005ed5b0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, object)));
    if (creature == nullptr) return 0;                                                           // test edi,edi; je
    unsigned char a[16], b[16];
    fill(self, first_params, portrait(creature, a));
    fill(self, second_params, portrait(creature, b));
    sw(self, index_field, static_cast<U32>(slot));
    return 1;
}
// Player-creature fallback (006B6231..006B628D / 006B64AC..006B6508): GetPlayerCreature re-fetched per fill.
void switch_player(void* self, U32 first_params, U32 second_params, U32 index_field) {
    unsigned char a[16], b[16];
    fill(self, first_params, portrait(client_get_player_creature_005ed540(client_app(), nullptr), a));
    fill(self, second_params, portrait(client_get_player_creature_005ed540(client_app(), nullptr), b));
    sw(self, index_field, 0xFFFFFFFFu);
}
}  // namespace

extern "C" {
// 005EDF80 CClientExoApp::ChangeCharacterToNextLivingPartyMember: mov ecx,[ecx+4]; jmp 005F7960 (RET 0Ch there).
U32 __fastcall kotor_fastcall_005edf80(void* app, void*, U32 a, U32 b, U32 c) {
    using Fn = U32(__fastcall*)(void*, void*, U32, U32, U32);
    const Fn target = reinterpret_cast<Fn>(reinterpret_cast<std::uintptr_t>(
        &kotor_input_required::change_character_to_next_living_party_member_005f7960));
    return target(lp(app, 4), nullptr, a, b, c);                                                 // 005EDF80..005EDF83
}

// 006B5600 CSWGuiInGameEquip::GetIcon(CExoString* out, int slot) RET 8.
void* __fastcall kotor_fastcall_006b5600(void* self, void*, void* out, I32 slot) {
    U32 local[2];
    exo_default(local);                                                                          // 006B561B..006B5627 CExoString()
    {
        auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(local, nullptr); });             // EH state 1
        // [[this+64h]+2F8h]+24h (word) vs word [0074BF10] = 5 (.rdata constant): droid icons get the "d" prefix.
        const U16 kind = lh(lp(lp(self, 0x64), 0x2F8), 0x24);                                    // 006B562C..006B5635
        const char* prefix = kind == 5 ? "d" : empty_text;                                       // 006B5639..006B564F (007566D0 / 0073D71C)
        // Quirk: slot is not range-checked (original indexes [00756568 + 14h*slot] directly); callers pass 0..8.
        frame_LogFormat_005e5680(local, "i%s%s", prefix, slot_icons[slot]);                      // 006B5654..006B566E (007566C8)
        WindowCopy005e5b00(out, nullptr, local);                                                 // 006B567A..006B5681 *out = local
        g.armed = false;
    }
    {
        auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(out, nullptr); });               // EH state 0: out constructed
        frame_LogStringDtor_005e5c20(local, nullptr);                                            // 006B5686..006B5697
        g.armed = false;
    }
    return out;                                                                                  // 006B56A0
}

// 006B7590 CSWGuiInGameEquip::SetCharacter(CSWCCreature* character) RET 4.
void __fastcall kotor_fastcall_006b7590(void* self, void*, void* character) {
    if (character != lp(self, 0x64)) {                                                           // 006B75AE..006B75B9
        sw(self, 0x64, ad(character));                                                           // 006B75C1
        if (character != nullptr) {                                                              // 006B75BF / 006B75C4
            for (I32 i = 0; i < 9; ++i) {                                                        // 006B75C6..006B7618
                U32 icon[2];
                void* name = kotor_fastcall_006b5600(self, nullptr, icon, i);                    // 006B75D0..006B75D8 GetIcon
                {
                    auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(icon, nullptr); });
                    menu_resources::Ref ref;
                    async_ref_construct_string_00406d60(&ref, nullptr, name);                    // 006B75DD..006B75EA CResRef(str)
                    fill(self, 0x10BC + static_cast<U32>(i) * 0x140u, &ref);                     // 006B75EF..006B75F8 LBL_INV_* fill
                    g.armed = false;
                }
                frame_LogStringDtor_005e5c20(icon, nullptr);                                     // 006B75FD..006B7609
            }
        } else {
            menu_resources::Ref ref;
            ref.construct_00406d80(empty_text);                                                  // 006B761C..006B7625 CResRef("")
            fill(self, 0x24C8, &ref);                                                            // 006B762A..006B7637 portrait
        }
    }
    if (void* current = lp(self, 0x64)) {                                                        // 006B763C..006B7641
        unsigned char buf[16];
        fill(self, 0x24C8, portrait(current, buf));                                              // 006B7643..006B765B
    }
}

// 006B7E30 CSWGuiInGameEquip::CloseDescription() RET.
void __fastcall kotor_fastcall_006b7e30(void* self, void*) {
    kotor_fastcall_006b7680(self, nullptr, 0);                                                   // 006B7E49..006B7E4D ShowDescription(0)
    const U32 control = lw(self, 0x42A0);                                                        // 006B7E58
    sw(self, 0x4270, lw(self, 0x4270) & 0xFFFFFFFEu);                                            // 006B7E52..006B7E68
    vcall<void, void*, U32>(self, 0x08, pt(control), 0);                                         // 006B7E5E..006B7E6E [vtbl+8](control,0)
    kotor_fastcall_006b9470(self, nullptr, lp(self, 0x1C));                                      // 006B7E71..006B7E77 OnEnterSlot([+1Ch])
    {
        U32 text[2];
        frame_LogStringCtor_005e5a90(text, nullptr, empty_text);                                 // 006B7E7C..006B7E85
        {
            auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(text, nullptr); });
            gui_load_text_params_set_text_00415e00(at(self, 0x3080), nullptr, text);             // 006B7E8A..006B7E9D
            g.armed = false;
        }
        frame_LogStringDtor_005e5c20(text, nullptr);                                             // 006B7EA2..006B7EAE
    }
    vcall<void, U32>(at(self, 0x30D8), 0x88, 0);                                                 // 006B7EB3..006B7EC1 LB_ITEMS [vtbl+88h](0)
}

// 006B8EB0 CSWGuiInGameEquip::OnSelectSlot(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006b8eb0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                          // 006B8ECD..006B8ED5
    if (lw(frame_GetServerArea_00605640(lp(lp(self, 0x64), 0x20), nullptr), 0x2B0) != 0) {      // 006B8EDB..006B8EEF
        refuse(self, 0xA56A);                                                                    // 006B8EF1..006B8F2F
        return;
    }
    if (lw(self, 0x4278) == 2 &&                                                                 // 006B8F48..006B8F4F
        lw(kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr), 0x4E0) == 1 && // 006B8F51..006B8F60
        lb(kotor_m15a_creature_get_server_creature_0060fb20(lp(self, 0x64), nullptr), 0xAC0) == 1) { // 006B8F62..006B8F71
        refuse(self, 0x5E2);                                                                     // 006B8F73..006B8FB0
        return;
    }
    if (lw(self, 0x4278) == 0x20 && lw(self, 0x4280) != 0x7F000000u) {                           // 006B8FC9..006B8FE1
        void* server = server_app();                                                             // 006B8FE7..006B8FED edi
        void* item = recovered_item_lookup_004ae760(server, nullptr,
                                                    kotor_fastcall_004aea30(server, nullptr, lw(self, 0x4280))); // 006B8FF0..006B8FFB
        if (item != nullptr && lb(kotor_m15b_item_get_base_item_005b4790(item, nullptr), 0x1B) == 4) { // 006B9000..006B900F
            refuse(self, 0xA568);                                                                // 006B9011..006B904F
            return;
        }
    }
    if (lw(self, 0x3378) == 1) {                                                                 // 006B9068..006B906F
        refuse(self, 0xA569);                                                                    // 006B9071..006B90A0 -> 006B8F20
        return;
    }
    sw(self, 0x42A0, ad(control));                                                               // 006B90A9
    kotor_fastcall_006b7680(self, nullptr, 1);                                                   // 006B90A5..006B90AF ShowDescription(1)
    void* items = at(self, 0x30D8);                                                              // 006B90B8 LB_ITEMS
    vcall<void, void*, U32>(self, 0x08, items, 0);                                               // 006B90B4..006B90C1 [vtbl+8](items,0)
    if (lw(self, 0x427C + lw(control, 0x58) * 4u) != 0x7F000000u)                                // 006B90C4..006B90D2
        gui_load_listbox_set_selected_0041c040(items, nullptr, 1, 0);                            // 006B90D4..006B90DA
    sw(self, 0x4270, lw(self, 0x4270) | 1u);                                                     // 006B90DF
    {
        U32 text[2];
        void* s = kotor_fastcall_005edeb0(client_app(), nullptr, text, 0x950A);                  // 006B90E6..006B90F9
        {
            auto g = on_throw([&]() { frame_LogStringDtor_005e5c20(text, nullptr); });
            gui_load_text_params_set_text_00415e00(at(self, 0x3080), nullptr, s);                // 006B90FE..006B910D
            g.armed = false;
        }
        frame_LogStringDtor_005e5c20(text, nullptr);                                             // 006B9112..006B911E
    }
    show_tutorial_window_005edf40(client_app(), nullptr, 0xB, 0x7F000000u, 0x7F000000u, 0);     // 006B9123..006B9139
    vcall<void, U32>(items, 0x88, 1);                                                            // 006B913E..006B9144
}

// 006B9160 CSWGuiInGameEquip::OnOKPressed(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006b9160(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                          // 006B9168..006B916F
    if ((lw(self, 0x4270) & 1u) == 0) return;                                                    // 006B9171..006B9178
    zero(self, 0x42AC);                                                                          // 006B9182
    zero(self, 0x42B4);                                                                          // 006B9188
    if (void* a = lp(self, 0x42B0)) { vcall<void*, U32>(a, 0x00, 1); zero(self, 0x42B0); }       // 006B917A..006B9196
    if (void* b = lp(self, 0x42B8)) { vcall<void*, U32>(b, 0x00, 1); zero(self, 0x42B8); }       // 006B919C..006B91AC
    kotor_fastcall_006b7e30(self, nullptr);                                                      // 006B91B2..006B91B4 CloseDescription
}

// 006BA820 CSWGuiInGameEquip::OnChangeCharacter(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006ba820(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                          // 006BA821..006BA833
    U32 which = 1;                                                                               // 006BA82E edi
    if ((lb(recovered_server_info_004aec90(server_app(), nullptr), 0x104) & 1u) != 0) {          // 006BA839..006BA84E
        const U8 slot = lb(control, 0x58);                                                       // 006BA854 bl
        void* creature;
        if (slot == 0xFF) {                                                                      // 006BA857..006BA85A
            creature = client_get_player_creature_005ed540(client_app(), nullptr);               // 006BA85C..006BA865
        } else {
            void* client = client_app();                                                         // 006BA874 edi
            const U32 object = recovered_npc_object_00564700(party_table(), nullptr,
                                                             static_cast<std::int8_t>(slot), 0, 1); // 006BA877..006BA886
            creature = pt(frame_R4_005ed5b0(client, nullptr, kotor_fastcall_005eda50(client, nullptr, object))); // 006BA88B..006BA896
        }
        if (creature != nullptr) {                                                               // 006BA89B..006BA89D
            if ((lw(self, 0x3FB0) & 2u) != 0) {                                                  // 006BA8A3..006BA8AA
                if (control == at(self, 0x3A20)) sw(self, 0x3A78, lw(self, 0x3C3C));             // 006BA8AC..006BA8BC
            } else {
                sw(self, 0x3A78, 0xFFFFFFFEu);                                                   // 006BA8CA
            }
            sb(self, 0x4274, slot);                                                              // 006BA8C2 / 006BA8D4
            kotor_fastcall_006b7590(self, nullptr, creature);                                    // 006BA921..006BA924 SetCharacter
        }
    } else {
        if (control == at(self, 0x3BE4)) which = 2;                                              // 006BA8DC..006BA8E6
        if (kotor_fastcall_005edf80(client_app(), nullptr, 0, 0, which) == 0) return;            // 006BA8EB..006BA900 -> 006BA96F
        kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                                     // 006BA902..006BA907 PlayGuiSound(1)
        kotor_fastcall_006b7590(self, nullptr,
                                kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr)); // 006BA90C..006BA924
    }
    const U32 mask = lw(self, 0x4278);                                                           // 006BA929
    for (U32 i = 0; i < 9; ++i) {                                                                // 006BA92F..006BA945
        if (slot_masks[i] == mask) {
            vcall<void, void*, U32>(self, 0x08, at(self, 0x68 + i * 0x1C4u), 0);                 // 006BA947..006BA958
            break;
        }
    }
    kotor_fastcall_006b9470(self, nullptr, lp(self, 0x1C));                                      // 006BA95B..006BA961 OnEnterSlot([+1Ch])
    kotor_fastcall_006b5b60(self, nullptr, 1);                                                   // 006BA966..006BA96A UpdatePortraits(1)
}

// 006B60F0 CSWGuiInGameEquip::OnSwitchLeft(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006b60f0(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                          // 006B60FA..006B60FF
    const U8 first = lb(self, 0x3A78);                                                           // 006B6106 bl
    U32 found = 0;                                                                               // 006B610D ebp
    if (first == 0xFE) return;                                                                   // 006B610F..006B6113
    fill(self, 0x3C64, at(self, 0x3AE0));                                                        // 006B6119..006B6127 slot 1 image -> slot 2
    fill(self, 0x3CD8, at(self, 0x3B54));                                                        // 006B612C..006B613A
    sw(self, 0x3C3C, lw(self, 0x3A78));                                                          // 006B613F..006B6145
    const I32 cur = static_cast<std::int8_t>(first);                                             // 006B614B [esp+34h]
    I32 slot = cur - 1;                                                                          // 006B614E
    if (slot >= 0) {                                                                             // 006B6151..006B6157
        do {
            if (found != 0) return;                                                              // 006B6160..006B6162
            found = switch_candidate(self, slot, 0x3AA0, 0x3B14, 0x3A78);                        // 006B6168..006B6204
            --slot;
        } while (slot >= 0);                                                                     // 006B6209..006B620A dec; jns
        if (found != 0) return;                                                                  // 006B6210..006B6212
    }
    if (static_cast<std::int8_t>(lb(self, 0x4274)) != -1 && lb(self, 0x3C3C) != 0xFF) {         // 006B621C..006B622F
        switch_player(self, 0x3AA0, 0x3B14, 0x3A78);                                             // 006B6231..006B628D
        return;
    }
    slot = 8;                                                                                    // 006B629B
    if (cur >= slot) return;                                                                     // 006B62A0..006B62A2
    do {
        if (found != 0) return;                                                                  // 006B62A8..006B62AA
        found = switch_candidate(self, slot, 0x3AA0, 0x3B14, 0x3A78);                            // 006B62B0..006B634B
        --slot;
    } while (slot > cur);                                                                        // 006B6350..006B6357
}

// 006B6370 CSWGuiInGameEquip::OnSwitchRight(CSWGuiControl* control) RET 4.
void __fastcall kotor_fastcall_006b6370(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                          // 006B637A..006B637F
    const U8 second = lb(self, 0x3C3C);                                                          // 006B6386 bl
    U32 found = 0;                                                                               // 006B638D ebp
    if (second == 0xFE) return;                                                                  // 006B638F..006B6393
    fill(self, 0x3AA0, at(self, 0x3CA4));                                                        // 006B6399..006B63A7 slot 2 image -> slot 1
    fill(self, 0x3B14, at(self, 0x3D18));                                                        // 006B63AC..006B63BA
    sw(self, 0x3A78, lw(self, 0x3C3C));                                                          // 006B63BF..006B63C5
    const I32 cur = static_cast<std::int8_t>(second);                                            // 006B63CB [esp+34h]
    I32 slot = cur + 1;                                                                          // 006B63CE
    if (slot < 9) {                                                                              // 006B63D1..006B63D8
        do {
            if (found != 0) return;                                                              // 006B63E0..006B63E2
            found = switch_candidate(self, slot, 0x3C64, 0x3CD8, 0x3C3C);                        // 006B63E8..006B6484
            ++slot;
        } while (slot < 9);                                                                      // 006B6489..006B648D
        if (found != 0) return;                                                                  // 006B6493..006B6495
    }
    // Unlike OnSwitchLeft, no check of the other slot's index byte here (original asymmetry, 006B649F..006B64AA).
    if (static_cast<std::int8_t>(lb(self, 0x4274)) != -1) {
        switch_player(self, 0x3C64, 0x3CD8, 0x3C3C);                                             // 006B64AC..006B6508
        return;
    }
    slot = 0;                                                                                    // 006B6516
    if (cur <= 0) return;                                                                        // 006B6518..006B651A
    do {
        if (found != 0) return;                                                                  // 006B6520..006B6522
        found = switch_candidate(self, slot, 0x3C64, 0x3CD8, 0x3C3C);                            // 006B6528..006B65C3
        ++slot;
    } while (slot < cur);                                                                        // 006B65C8..006B65CF
}
}  // extern "C"
