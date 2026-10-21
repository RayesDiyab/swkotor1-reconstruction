// lane-w3 unit w3-client-t2 (tier t2, coordinator ruling t2-7). Original: swkotor.exe GOG K1, read-only static
// disassembly (lane-w3 tools/odis.py, sdis.py); every statement cites the original instruction(s) it reproduces.
//   0064BF30 CSWCMessage::HandleServerToPlayerSetCustomToken      (major 0x32; W2 writer 0056D2D0)
//   00668640 CSWCMessage::HandleServerToPlayerPlayerList          (major 0x0A; W2 writers 0056FE40 / 00568030)
//   0064DCF0 CSWCMessage::HandleServerToPlayerUpdate_WorkRemaining ('W' record of GameObjectUpdate; W2 00567BA0)
//   006778F0 CSWCMessage::SendPlayerToServerArea_AreaLoaded      (only original caller: 0064DE68 above)
//   0060C790 CSWCCreature::SetWayPoints
//   0064B460 CSWCPlayer::CSWCPlayer   005F1090 CSWCPlayer::~CSWCPlayer   005EE5C0 CSWCPlayer::SetPrimaryPCPortraitId
//   005EE480 CSWCPlayer::SetPortrait  (005ED6C0 CClientExoApp::GetPlayerList reproduced inline: field address)
// Exception frames (SEH state bytes) only destroy CExoString temporaries; the C++ locals reproduce that cleanup in the
// original order on the normal paths (the original never throws on these paths).
//
// CSWCPlayer (38h bytes, operator new at 00668A32): +00 vtable 00751C68, +04 player id, +08 DWORD 0, +0C CExoString
// name, +14 CSWPortrait (CResRef), +24 WORD portrait id (FFFFh), +28 game object id, +2C creature id,
// +30 CExoString (creature name, CExoLocString text).
#include "835c32b896-w3_client_t2.h"

using namespace kotor_w3;

namespace {
constexpr U kPlayerIdServer_0074c528 = 0xFFFFFFFDu;                   // .rdata 0074C528 (server player id)
const char kSpace_0073d5d8[] = " ";                                   // .rdata 0073D5D8
const char kPortraitPrefix_00745e4c[] = "po_";                        // .rdata 00745E4C
const char kBaseResRef_00745e40[] = "BaseResRef";                     // .rdata 00745E40
constexpr U kUpdateScreenSeconds_3d088889 = 0x3D088889u;              // immediate 1/30 s (0064DE13)
// 00751C68 CSWCPlayer vftable: a single slot, 0063E7F0 return_zero (existing owner 0486).
const void* const kClientPlayerTable_00751c68[1] = {
    reinterpret_cast<const void*>(&slot_return_zero_0063e7f0_a0),
};
// 005ED6C0 CClientExoApp::GetPlayerList: EAX = [this+4] + 1Ch (the internal's CExoLinkedList<CSWCPlayer>).
void* client_player_list(void* app) noexcept { return at(rp(app, 4), 0x1C); }     // 005ED6C0..005ED6C6
// [list+0] = CExoLinkedListInternal*; [internal+0] = head node; node +8 = data.
void* list_internal(void* list) noexcept { return rp(list, 0); }
// rep movsd / rep movsb of `bytes` bytes (no access when bytes == 0, as the original string moves).
void copy_bytes(void* destination, const void* source, U bytes) noexcept {
    unsigned char* d = static_cast<unsigned char*>(destination);
    const unsigned char* s = static_cast<const unsigned char*>(source);
    for (U i = 0; i < bytes; ++i) d[i] = s[i];
}
kotor_reconstruction::StartupString* ss(void* exo_string) noexcept {
    return static_cast<kotor_reconstruction::StartupString*>(exo_string);
}
void construct_string(ExoString* s) noexcept(false) {
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr);   // 005B3190
}
}  // namespace

extern "C" {
// ==================================================================================================== CSWCPlayer
// 0064B460 CSWCPlayer::CSWCPlayer(DWORD player_id). ECX this, RET 4, EAX this.
void* __fastcall recovered_client_player_construct_0064b460(void* self, void*, U player_id) noexcept(false) {
    st<const void*>(self, 0x00, kClientPlayerTable_00751c68);         // 0064B480
    construct_string(static_cast<ExoString*>(at(self, 0x0C)));        // 0064B479..0064B486
    kotor_m15b_portrait_construct_00678ec0(at(self, 0x14), nullptr);  // 0064B48B..0064B496
    construct_string(static_cast<ExoString*>(at(self, 0x30)));        // 0064B49B..0064B4A3
    st<U>(self, 0x04, player_id);                                     // 0064B4A8..0064B4B0
    st<U>(self, 0x08, 0);                                             // 0064B4B3
    st<U16>(self, 0x24, 0xFFFF);                                      // 0064B4BA
    return self;                                                      // 0064B4C0
}

// 005F1090 CSWCPlayer::~CSWCPlayer. ECX this, RET.
void __fastcall recovered_client_player_destruct_005f1090(void* self, void*) noexcept(false) {
    frame_LogStringDtor_005e5c20(at(self, 0x30), nullptr);            // 005F10AD..005F10B8
    kotor_fastcall_004015a0(at(self, 0x14), nullptr);                 // 005F10BD..005F10C5 ~CSWPortrait (void_return)
    frame_LogStringDtor_005e5c20(at(self, 0x0C), nullptr);            // 005F10CA..005F10D5
}

// 005EE480 CSWCPlayer::SetPortrait(CResRef) — CResRef by value (4 stack words). ECX this, RET 10h.
void __fastcall recovered_client_player_set_portrait_005ee480(void* self, void*, ResRef portrait) noexcept(false) {
    ResRef copy_tmp;                                                  // [esp+0] hidden-return temporary
    c2da_copy_00406120(at(self, 0x14), nullptr, &copy_tmp, &portrait);   // 005EE483..005EE490 [this+14h] = arg
}

// 005EE5C0 CSWCPlayer::SetPrimaryPCPortraitId(WORD id). ECX this, RET 4. For id != FFFFh the portrait resref becomes
// "po_" + portraits.2da[id].BaseResRef; the id is stored in every case.
void __fastcall recovered_client_player_set_primary_pc_portrait_id_005ee5c0(void* self, void*, U id_word) noexcept(false) {
    ExoString prefix_10;                                              // [esp+10h]
    construct_string(&prefix_10);                                     // 005EE5E2..005EE5E6
    ExoString base_08;                                                // [esp+08h]
    construct_string(&base_08);                                       // 005EE5EB..005EE5F7
    const U16 id = static_cast<U16>(id_word);                         // 005EE5FC mov di,[ebp+8]
    if (id != 0xFFFF) {                                               // 005EE600..005EE60A
        c2da_assign_text_005e5140(&prefix_10, nullptr, kPortraitPrefix_00745e4c);   // 005EE610..005EE619 = "po_"
        ExoString column_18;                                          // [esp+18h]
        frame_LogStringCtor_005e5a90(&column_18, nullptr, kBaseResRef_00745e40);   // 005EE61E..005EE627
        void* const portraits = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x38);   // 005EE62C..005EE637 [[rules+B8h]+38h]
        (void)c2da_current_get_string_00413270(portraits, nullptr, static_cast<int>(id), &column_18, &base_08);   // 005EE63A..005EE64F
        frame_LogStringDtor_005e5c20(&column_18, nullptr);            // 005EE654..005EE65D
        ExoString sum_18;                                             // [esp+18h] operator+ hidden return
        GuiConcat005e5d10(ss(&prefix_10), nullptr, ss(&sum_18), ss(&base_08));   // 005EE662..005EE670 prefix + base
        async_assign_string_005e5c50(&base_08, nullptr, &sum_18);     // 005EE675..005EE67F
        frame_LogStringDtor_005e5c20(&sum_18, nullptr);               // 005EE684..005EE68D
        ResRef ref_18;                                                // [esp+18h]
        reinterpret_cast<menu_resources::Ref*>(&ref_18)->construct_00406d80(
            async_string_cstr_005e5670(&base_08, nullptr));           // 005EE692..005EE6A0 CResRef(CStr())
        ResRef copy_28;                                               // [esp+28h] hidden-return temporary
        c2da_copy_00406120(at(self, 0x14), nullptr, &copy_28, &ref_18);   // 005EE6A5..005EE6B2
    }
    st<U16>(self, 0x24, id);                                          // 005EE6BB
    frame_LogStringDtor_005e5c20(&base_08, nullptr);                  // 005EE6B7..005EE6C4
    frame_LogStringDtor_005e5c20(&prefix_10, nullptr);                // 005EE6C9..005EE6D5
}

// =========================================================================================== CSWCCreature waypoints
// 0060C790 CSWCCreature::SetWayPoints(WORD count, Vector* points). ECX this, RET 8.
// +1BCh Vector* waypoint array, +1C0h current waypoint pointer, +1ECh WORD count.
void __fastcall recovered_client_creature_set_way_points_0060c790(void* self, void*, U count_word, U points_word) {
    const U16 count = static_cast<U16>(count_word);                   // 0060C7A7 mov si,[esp+18h]
    const void* const points = word_ptr(points_word);                 // {E+8}
    if (ld<U16>(self, 0x1EC) == count) {                              // 0060C7AE..0060C7B6
        copy_bytes(rp(self, 0x1BC), points, static_cast<U>(count) * 12u);   // 0060C7B8..0060C7D7 rep movs
        st<U>(self, 0x1C0, rd32(self, 0x1BC));                        // 0060C7D9..0060C7DF
        return;                                                       // 0060C7F6 ret 8
    }
    void* const old = rp(self, 0x1BC);                                // 0060C7F9
    st<U16>(self, 0x1EC, count);                                      // 0060C801
    if (old != nullptr) live_free(old);                               // 0060C7FF..0060C80B
    if (count == 0) {                                                 // 0060C813..0060C816
        st<U>(self, 0x1BC, 0);                                        // 0060C818
        st<U>(self, 0x1C0, 0);                                        // 0060C822
        return;                                                       // 0060C83D ret 8
    }
    const U bytes = static_cast<U>(count) * 12u;                      // 0060C840..0060C846
    void* const memory = live_operator_new(bytes);                    // 0060C849..0060C84A
    void* array = nullptr;                                            // 0060C874 (null allocation)
    if (memory != nullptr) {                                          // 0060C858..0060C862
        vector_constructor_iterator_00401130(memory, 0xC, count, &vector_default_construct_00409e70);   // 0060C864..0060C86D
        array = memory;
    }
    st<void*>(self, 0x1BC, array);                                    // 0060C878
    st<void*>(self, 0x1C0, array);                                    // 0060C87E
    if (array != nullptr) copy_bytes(array, points, bytes);           // 0060C884..0060C89C rep movs
}

// ============================================================================================ client -> server
// 006778F0 CSWCMessage::SendPlayerToServerArea_AreaLoaded: 3-byte message 'p' 04 03 to the server player.
// ECX this (unused), RET. EAX = CNetLayer::SendMessageToPlayer result, or 0 when the allocation fails.
U __fastcall recovered_client_send_area_loaded_006778f0(void* /*self*/, void*) noexcept(false) {
    unsigned char* const message = static_cast<unsigned char*>(live_operator_new(3));   // 006778F1..006778F8
    if (message == nullptr) return 0;                                 // 006778FD..00677902 (EAX = 0)
    message[0] = 0x70;                                                // 00677904 'p'
    message[1] = 4;                                                   // 00677907 major Area
    message[2] = 3;                                                   // 0067790B minor AreaLoaded
    void* const app = client_app();                                   // 0067790F..00677914
    // 0067791D..00677925: CClientExoApp vtable +10h = GetNetLayer() (no arguments).
    void* const net = vslot<void* (__fastcall*)(void*, void*)>(app, 0x10)(app, nullptr);
    const U result = net_layer_send_message_to_player_005d4cb0(net, nullptr, kPlayerIdServer_0074c528,
                                                               message, 3, 0);   // 00677917..0067792A
    live_free(message);                                               // 0067792F..00677932
    return result;                                                    // 0067793A
}

// ============================================================================================ server -> client
// 0064DCF0 CSWCMessage::HandleServerToPlayerUpdate_WorkRemaining: BYTE stage, BYTE total (clamped to >= 1). Updates
// the load bar to GetLoadStep(0..3) + GetLoadStep(4) * stage / total; when stage == total sends AreaLoaded.
// ECX this, RET (EAX 0 on read overflow, else 1).
U __fastcall recovered_client_handle_update_work_remaining_0064dcf0(void* self, void*) {
    const U8 stage = kotor_fastcall_004d6720(self, nullptr, 8);       // 0064DD0A..0064DD17 ([esp+0Fh])
    const U8 read_total = kotor_fastcall_004d6720(self, nullptr, 8);  // 0064DD13..0064DD1B
    const U8 total = read_total < 1 ? U8{1} : read_total;             // 0064DD20..0064DD26 (bl)
    if (recovered_player_message::overrun_004d6230(self) != 0) return 0;   // 0064DD28..0064DD47
    void* const app = client_app();                                   // 0064DD48..0064DD4D / 0064DD57..0064DD5D (esi)
    void* const module = kotor_fastcall_005ed530(app, nullptr);       // 0064DD52 GetModule (ebp)
    // x87 sequence (precision control 53 bits): fild step4; fild stage; fidiv total; fmulp; fstp float.
    const double share = static_cast<double>(static_cast<I32>(stage)) /
                         static_cast<double>(static_cast<I32>(total));   // 0064DD70..0064DD90
    float progress = static_cast<float>(static_cast<double>(static_cast<I32>(client_load_step(app, 4))) * share);   // 0064DD68..0064DD96
    progress = static_cast<float>(static_cast<double>(static_cast<I32>(client_load_step(app, 0))) + progress);   // 0064DD9A..0064DDB2
    progress = static_cast<float>(static_cast<double>(static_cast<I32>(client_load_step(app, 1))) + progress);   // 0064DDB6..0064DDCE
    progress = static_cast<float>(static_cast<double>(static_cast<I32>(client_load_step(app, 2))) + progress);   // 0064DDD2..0064DDEA
    const double total_progress = static_cast<double>(static_cast<I32>(client_load_step(app, 3))) + progress;   // 0064DDEE..0064DDFE
    kotor_m15_client_set_load_bar_progress_005edd40(app, nullptr, static_cast<I32>(total_progress), 1);   // 0064DE02..0064DE0A __ftol2; push 1 at 0064DD60
    kotor_cdecl_00401c10(fbits(kUpdateScreenSeconds_3d088889), 0, 0);   // 0064DE0F..0064DE18 UpdateScreen(1/30, 0, 0)
    if (stage == total) {                                             // 0064DE1D..0064DE26
        ExoString area_name;                                          // [esp+1Ch]
        construct_string(&area_name);                                 // 0064DE28..0064DE2C
        ResRef area_ref;                                              // [esp+24h] copy of [module+28h]
        std::memcpy(area_ref.bytes, at(module, 0x28), 16);            // 0064DE31..0064DE5D
        c2da_ref_to_string_00405f70(&area_ref, nullptr, &area_name);  // 0064DE41..0064DE61 CopyToString (unused result)
        recovered_client_send_area_loaded_006778f0(self, nullptr);    // 0064DE66..0064DE68
        frame_LogStringDtor_005e5c20(&area_name, nullptr);            // 0064DE6D..0064DE79
    }
    return 1;                                                         // 0064DE85
}

// 0064BF30 CSWCMessage::HandleServerToPlayerSetCustomToken(BYTE minor). ECX this, RET 4.
// minor 1: one {DWORD number, CExoString value}; minor 2: DWORD count then count pairs. Each pair goes to
// CTlkTable::SetCustomToken of the client TLK. EAX 1 when the message was consumed exactly, else 0.
unsigned __fastcall recovered_client_custom_token_0064bf30(void* self, void*, unsigned minor) noexcept(false) {
    ExoString value_10;                                               // [esp+10h]
    construct_string(&value_10);                                      // 0064BF53..0064BF57
    bool ok = false;
    if (recovered_player_message::overrun_004d6230(self) == 0) {      // 0064BF5C..0064BF6F
        const U8 kind = static_cast<U8>(minor);                       // 0064BF75 mov al,[ebp+8]
        bool known = true;
        if (kind == 1) {                                              // 0064BF78..0064BF7A
            const U number = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064BF7C..0064BF85
            ExoString read_18;                                        // [esp+18h]
            kotor_fastcall_004d67e0(self, nullptr, &read_18, 0x20);   // 0064BF87..0064BF90
            async_assign_string_005e5c50(&value_10, nullptr, &read_18);   // 0064BF95..0064BF9F
            frame_LogStringDtor_005e5c20(&read_18, nullptr);          // 0064BFA4..0064BFAD
            kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, number, &value_10);   // 0064BFB2..0064BFBE
        } else if (kind == 2) {                                       // 0064BFC5..0064BFC7
            const U count = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064BFCD..0064BFD1
            for (U left = count; left != 0; --left) {                 // 0064BFD6..0064BFDA / 0064C027..0064C028
                const U number = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 0064BFE0..0064BFE4
                ExoString read_18;                                    // [esp+18h]
                kotor_fastcall_004d67e0(self, nullptr, &read_18, 0x20);   // 0064BFE9..0064BFF4
                async_assign_string_005e5c50(&value_10, nullptr, &read_18);   // 0064BFF9..0064C003
                frame_LogStringDtor_005e5c20(&read_18, nullptr);      // 0064C008..0064C011
                kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, number, &value_10);   // 0064C016..0064C022
            }
        } else {
            known = false;                                            // 0064BFC7 jne 0064C06E
        }
        if (known && recovered_player_message::overrun_004d6230(self) == 0 &&    // 0064C02A..0064C035
            recovered_player_message::underrun_004d6240(self) == 0)   // 0064C037..0064C042
            ok = true;
    }
    frame_LogStringDtor_005e5c20(&value_10, nullptr);                 // 0064C044..0064C050 / 0064C06E..0064C07A
    return ok ? 1u : 0u;                                              // 0064C055 / 0064C085
}

// 00668640 CSWCMessage::HandleServerToPlayerPlayerList(BYTE minor). ECX this, RET 4.
// minor 1 (All): destroy every client player, then BYTE count records; minor 2 (Add): one record;
// minor 3 (Remove): DWORD player id. Record: DWORD player id, OBJECTID game object, CExoString name, BOOL has
// creature [OBJECTID creature, LocString first, LocString last, WORD portrait id, CResRef portrait if id >= FFFEh].
// EAX 1 when the message was consumed exactly (and, for Remove, the player was found), else 0.
unsigned __fastcall recovered_client_player_list_00668640(void* self, void*, unsigned minor) noexcept(false) {
    ExoString name_1c;                                                // [esp+1Ch]
    construct_string(&name_1c);                                       // 00668666..0066866A
    ExoString unused_24;                                              // [esp+24h]
    construct_string(&unused_24);                                     // 0066866F..0066867E
    ExoString first_0c;                                               // [esp+0Ch]
    construct_string(&first_0c);                                      // 00668683..0066868F
    ExoString last_14;                                                // [esp+14h]
    construct_string(&last_14);                                       // 00668694..006686A0
    ResRef portrait_4c;                                               // [esp+4Ch]
    kotor_fastcall_00405ed0(&portrait_4c, nullptr);                   // 006686A5..006686B1
    bool ok = false;
    if (recovered_player_message::overrun_004d6230(self) == 0) {      // 006686B6..006686C1
        void* const list = client_player_list(client_app());          // 006686C7..006686CF ([esp+3Ch], edi)
        (void)frame_R4_005ed690(client_app(), nullptr);               // 006686D4..006686E3 GetInGameGui (unused)
        const U8 kind = static_cast<U8>(minor);                       // 006686E8 movzx eax,[ebp+8]
        U8 count = 0;                                                 // bl
        bool records = true;
        if (kind == 3) {                                              // 006686EC..006686FB (Remove)
            records = false;
            const U player_id = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 00668701..0066870E (ebx)
            if (recovered_player_message::overrun_004d6230(self) == 0 &&    // 0066870A..00668717
                recovered_player_message::underrun_004d6240(self) == 0) {   // 0066871D..00668728
                void* position = rp(list_internal(list), 0);          // 0066872E..00668732 [esp+30h] = head node
                void* const head = rp(list_internal(list), 0);        // 00668736..00668738
                void* player = head != nullptr ? rp(head, 8) : nullptr;   // 0066873A..00668743 (esi)
                while (player != nullptr && rd32(player, 4) != player_id)   // 00668750..00668753
                    player = unresolved_list_next_005e9b90(list_internal(list), nullptr, &position);   // 00668755..00668765
                if (player != nullptr) {                              // 00668745 / 00668767 -> fail
                    c2da_005e9c10(list_internal(list), nullptr, position);   // 0066876C..00668773 Remove(position)
                    recovered_client_player_destruct_005f1090(player, nullptr);   // 00668778..0066877A
                    live_free(player);                                // 0066877F..00668785
                    ok = true;                                        // 00668788 -> EAX 1
                }
            }
        } else if (kind == 2) {                                       // 006686F3..006686F4 (Add)
            count = 1;                                                // 006687EB mov bl,1
        } else if (kind == 1) {                                       // 006686EC..006686ED (All)
            while (rp(list_internal(list), 0) != nullptr) {           // 006687EF..006687F4 / 00668811..0066881A
                void* const player = base_ini_reconstruction::remove_head_005e9c80(
                    static_cast<kotor_list::Internal*>(list_internal(list)), nullptr);   // 006687F6..006687FB
                if (player != nullptr) {                              // 006687FD..006687FF
                    recovered_client_player_destruct_005f1090(player, nullptr);   // 00668801..00668803
                    live_free(player);                                // 00668808..0066880E
                }
            }
            count = kotor_fastcall_004d6720(self, nullptr, 8);        // 0066881C..00668825
        } else {
            records = false;                                          // 006686FB jne 00668B45
        }
        if (records && recovered_player_message::overrun_004d6230(self) == 0) {   // 00668827..00668832
            bool failed = false;
            const U16 records_count = count;                          // 0066883C..00668843 [esp+2Eh]
            for (U16 i = 0; i < records_count && !failed; ++i) {      // 00668838 / 00668B20..00668B2E
                const U player_id = kotor_fastcall_004d6780(self, nullptr, 0x20);   // 00668850..0066885B (edi)
                const U object_id = kotor_fastcall_00692630(self, nullptr);   // 0066885D..0066886B [esp+44h]
                ExoString read_78;                                    // [esp+78h]
                kotor_fastcall_004d67e0(self, nullptr, &read_78, 0x20);   // 00668862..0066886F
                async_assign_string_005e5c50(&name_1c, nullptr, &read_78);   // 00668874..00668881
                frame_LogStringDtor_005e5c20(&read_78, nullptr);      // 00668886..00668892
                const U has_creature = kotor_fastcall_004d66f0(self, nullptr);   // 00668897..006688A1 [esp+48h]
                U creature_id = 0;                                    // [esp+40h] (written only when has_creature == 1)
                U portrait_id = 0;                                    // [esp+38h]
                if (has_creature == 1) {                              // 0066889E..006688A5
                    creature_id = kotor_fastcall_00692630(self, nullptr);   // 006688AB..006688BB
                    ResRef sound_90;                                  // [esp+90h]
                    kotor_fastcall_00405ed0(&sound_90, nullptr);      // 006688B2..006688BF
                    ExoString read_70;                                // [esp+70h]
                    kotor_fastcall_00692660(self, nullptr, &read_70, &sound_90, nullptr);   // 006688C4..006688CC
                    async_assign_string_005e5c50(&first_0c, nullptr, &read_70);   // 006688D1..006688DE
                    frame_LogStringDtor_005e5c20(&read_70, nullptr);  // 006688E3..006688EF
                    ResRef sound_b0;                                  // [esp+B0h]
                    kotor_fastcall_00405ed0(&sound_b0, nullptr);      // 006688F4..006688FD
                    ExoString read_88;                                // [esp+88h]
                    kotor_fastcall_00692660(self, nullptr, &read_88, &sound_b0, nullptr);   // 00668902..0066890D
                    async_assign_string_005e5c50(&last_14, nullptr, &read_88);   // 00668912..0066891F
                    frame_LogStringDtor_005e5c20(&read_88, nullptr);  // 00668924..00668933
                    if (frame_LogIsEmpty_005e58a0(&last_14, nullptr) == 0) {   // 00668938..00668943
                        ExoString space_5c;                           // [esp+5Ch]
                        frame_LogStringCtor_005e5a90(&space_5c, nullptr, kSpace_0073d5d8);   // 00668949..00668952
                        ExoString sum_64;                             // [esp+64h]
                        GuiConcat005e5d10(ss(&first_0c), nullptr, ss(&sum_64), ss(&space_5c));   // 00668957..0066896D
                        ExoString sum_80;                             // [esp+80h]
                        GuiConcat005e5d10(ss(&sum_64), nullptr, ss(&sum_80), ss(&last_14));   // 00668972..00668989
                        async_assign_string_005e5c50(&first_0c, nullptr, &sum_80);   // 0066898E..0066899B
                        frame_LogStringDtor_005e5c20(&sum_80, nullptr);   // 006689A0..006689AF
                        frame_LogStringDtor_005e5c20(&sum_64, nullptr);   // 006689B4..006689C0
                        frame_LogStringDtor_005e5c20(&space_5c, nullptr);   // 006689C5..006689D1
                    }
                    portrait_id = kotor_fastcall_004d6750(self, nullptr, 0x10);   // 006689D6..006689E3 (AX)
                    if (static_cast<U16>(portrait_id) >= 0xFFFE) {    // 006689DF..006689E7
                        ResRef read_a0;                               // [esp+A0h]
                        void* const read = kotor_fastcall_004d6180(self, nullptr, &read_a0, 0x10);   // 006689E9..006689F5
                        ResRef copy_c0;                               // [esp+C0h]
                        c2da_copy_00406120(&portrait_4c, nullptr, &copy_c0, read);   // 006689FA..00668A07
                    }
                }
                U created = 0;                                        // 00668A10 xor ebx,ebx
                if (recovered_player_message::overrun_004d6230(self) != 0) { failed = true; break; }   // 00668A0C..00668A19
                void* player = kotor_fastcall_005ed6d0(client_app(), nullptr, player_id);   // 00668A1F..00668A29
                if (player == nullptr) {                              // 00668A2E..00668A30
                    created = 1;                                      // 00668A34
                    void* const memory = live_operator_new(0x38);     // 00668A32..00668A39
                    player = memory != nullptr                        // 00668A45..00668A4F
                                 ? recovered_client_player_construct_0064b460(memory, nullptr, player_id)   // 00668A51..00668A54
                                 : nullptr;                           // 00668A67
                } else {
                    player = kotor_fastcall_005ed6d0(client_app(), nullptr, player_id);   // 00668A77..00668A80 (second call)
                }
                if (player == nullptr) { failed = true; break; }      // 00668A89
                st<U>(player, 0x28, object_id);                       // 00668A8F..00668A97
                async_assign_string_005e5c50(at(player, 0x0C), nullptr, &name_1c);   // 00668A93..00668A9E
                if (has_creature == 1) {                              // 00668AA3..00668AA8
                    async_assign_string_005e5c50(at(player, 0x30), nullptr, &first_0c);   // 00668AAA..00668AB2
                    st<U>(player, 0x2C, creature_id);                 // 00668AB7..00668ABF
                    recovered_client_player_set_primary_pc_portrait_id_005ee5c0(player, nullptr, portrait_id);   // 00668ABB..00668AC5
                    if (static_cast<U16>(portrait_id) >= 0xFFFE) {    // 00668ACA..00668AD2
                        // 00668AD4..00668AE3: cdecl CheckCustomPortrait(&portrait, id == FFFEh); its two argument
                        // words are reused (sub esp,8) as the low half of SetPortrait's by-value CResRef.
                        (void)recovered_client_creature_stats_check_custom_portrait_00647c10(
                            &portrait_4c, static_cast<U16>(portrait_id) == 0xFFFE ? 1 : 0);
                        recovered_client_player_set_portrait_005ee480(player, nullptr, portrait_4c);   // 00668AE8..00668B0A
                    }
                }
                if (created == 1)                                     // 00668B0F..00668B12
                    c2da_005e9d70(list_internal(list), nullptr, player);   // 00668B14..00668B1B AddHead
            }
            if (!failed && recovered_player_message::underrun_004d6240(self) == 0)   // 00668B34..00668B3F
                ok = true;                                            // -> 00668788
        }
    }
    frame_LogStringDtor_005e5c20(&last_14, nullptr);                  // 00668788 / 00668B45
    frame_LogStringDtor_005e5c20(&first_0c, nullptr);                 // 00668799 / 00668B56
    frame_LogStringDtor_005e5c20(&unused_24, nullptr);                // 006687AA / 00668B67
    frame_LogStringDtor_005e5c20(&name_1c, nullptr);                  // 006687BB / 00668B78
    return ok ? 1u : 0u;                                              // 006687CF / 00668B95
}
}  // extern "C"
