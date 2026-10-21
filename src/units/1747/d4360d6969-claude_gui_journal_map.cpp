// Lane W4 G15 (gui-journal): CSWGuiInGameMap panel slots (table 00754830) whose W4 trap stubs are replaced, plus the
// callees they reach that had no rebuilt provider. Transcribed from swkotor.exe linear-sweep listings
// (claude-runs/tools/win.py <VA>); original PCs cited per statement.
//
//   00692810 CSWGuiInGameMap::Draw(float)                                     thiscall RET 4   (slot 34h)
//   00692930 CSWGuiInGameMap::HitCheckMouse(int, int)                         thiscall RET 8   (slot 40h)
//   00693650 CSWGuiInGameMap::OnPanelAdded()                                  thiscall RET 0   (slot 48h)
//   00693BA0 CSWGuiInGameMap::OnPanelRemoved()                                thiscall RET 0   (slot 4Ch)
//   00693BC0 CSWGuiInGameMap::HandleInputEvent(int, int)                      thiscall RET 8   (slot 3Ch)
// Callees reconstructed here (no rebuilt body existed in the c24 map):
//   00692BC0 CSWGuiInGameMap::OnXButton(int)                                  RET 4
//   00692B00 CSWGuiInGameMap::ReturnToEbonHawk(int)                           RET 4 (message-box callback)
//   00692C70 CSWGuiMapHider::InitializeMapNotes(int) -> CExoString            RET 8 (hidden return pointer)
//   00692E80 CSWGuiMapHider::GetNextMapNote() -> CExoString                   RET 4 (hidden return pointer)
//   00693090 CSWGuiMapHider::GetPrevMapNote() -> CExoString                   RET 4 (hidden return pointer)
//   005EDC00 CClientExoApp::DismissInGameGUI() (mov ecx,[ecx+4]; jmp 00620320) RET 0
//   00620320 CClientExoAppInternal::DismissInGameGUI()                       RET 0
// The CSWGuiMapHider (+E38) Draw / HitCheckMouse slots are owned by lane gui-hud; reached here only via its table.
// Bound to lane gui-hud bodies (claude_gui_hud.cpp): 006929B0 CSWGuiInGameMap::SetMapNote, 00579210
// CSWSAreaMap::IsWorldPointExplored (both callees here; not redefined).
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_reconstruction { struct StartupString; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                    // CExoString::operator+
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };              // CTlkTable::GetSimpleString
}
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable
namespace frame_control { extern std::uint32_t message_manager_007a3a00; }                    // [007A3A00] CVirtualMachine*
struct MapVec { float x, y, z; };
static_assert(sizeof(MapVec) == 12, "Vector");

extern "C" {
// ---- panel / manager / in-game GUI
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, U32 delta_bits);               // CSWGuiPanel::Draw (float on stack)
std::uint32_t __fastcall gui_load_panel_hit_check_0040b690(void* panel, void*, int x, int y); // CSWGuiPanel::HitCheckMouse
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, U32 id, U32 down);   // CSWGuiPanel::HandleInputEvent
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);                         // CSWGuiPanel::OnPanelAdded
void __fastcall gui_load_panel_on_removed_0040c170(void* panel, void*);                       // CSWGuiPanel::OnPanelRemoved
void __fastcall kotor_fastcall_0040a140(void* manager, void*, U32 sound);                     // CSWGuiManager::PlayGuiSound
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, U32, U32);         // CSWGuiManager::AddPanel
int __fastcall frame_R4_0062cba0(void* in_game, void*, unsigned keep_paused);                 // CGuiInGame::HideSWInGameGui
void __fastcall kotor_fastcall_005eda60(void* app, void*, U32 input_class, U32 show);         // CClientExoApp::SetInputClass
void __fastcall kotor_fastcall_006200e0(void* client, void*, int, int);                       // CClientExoAppInternal::SetInputClass
void __fastcall kotor_fastcall_0062dd20(void* gui, void*, void* script, U32 flag, I32 forced1, I32 forced2); // CGuiInGame::ShowPartySelection
U32 __fastcall show_tutorial_window_005edf40(void* client, void*, U32 window, U32 a, U32 b, U32 c); // CClientExoApp::ShowTutorialWindow
void* __fastcall kotor_fastcall_005ed700(void* app, void*);                                   // CClientExoApp::GetClientOptions
void* __fastcall kotor_fastcall_005ed530(void* app, void*);                                   // CClientExoApp::GetModule
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                   // CClientExoApp::GetSWParty
U32 __fastcall kotor_m15_client_get_player_creature_id_005ed550(void* app, void*);            // CClientExoApp::GetPlayerCreatureId
U32 __fastcall kotor_fastcall_005ed4c0(void* client, void*);                                  // CClientExoApp::GetClientLanguage
U32 __fastcall kotor_fastcall_00635350(void* party, void*);                                   // CSWParty::IsPartyTogether
void __fastcall frame_R4_005d0fc0(void* vm, void*, unsigned* script, unsigned object, unsigned flag); // CVirtualMachine::RunScript
// ---- message box
void __fastcall kotor_fastcall_00627130(void* self, void*, std::uint32_t allow);              // CSWGuiMessageBox::SetAllowCancel
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, void* object, const void* func, U32 extra); // SetCallback
// ---- map hider (panels_menus)
void __fastcall kotor_fastcall_00692e30(void* hider, void*);                                  // CSWGuiMapHider::ClearMapNotes
// ---- defined by lane gui-hud (claude_gui_hud.cpp; identical transcription, bound here instead of a duplicate)
U32 __fastcall kotor_fastcall_00579210(void* map, void*, MapVec world);                    // CSWSAreaMap::IsWorldPointExplored RET 0Ch
void __fastcall kotor_fastcall_006929b0(void* self, void*, void* note);                     // CSWGuiInGameMap::SetMapNote RET 4
// ---- server
void* __fastcall server_get_module_004ae6b0(void* server, void*);                             // CServerExoApp::GetModule
void* __fastcall module_get_area_004c30f0(void* module, void*);                               // CSWSModule::GetArea
U32 __fastcall kotor_fastcall_004aea40(void* server, void*);                                  // CServerExoApp::GetPlayerCreatureId
U32 __fastcall kotor_fastcall_004aea30(void* server, void*, U32 id);                          // CServerExoApp::ClientToServerObjectId
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U32 id);                        // CServerExoApp::GetCreatureByGameObjectID
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U32 id);                        // CServerExoApp::GetGameObject
void* __fastcall recovered_object_array_004aed70(void* server, void*);                        // CServerExoApp::GetObjectArray
unsigned char __fastcall kotor_fastcall_004d8230(void* array, void*, U32 id, void** out);     // CGameObjectArray::GetGameObject
void* __fastcall recovered_object_area_004cb120(void* object, void*);                         // CSWSObject::GetArea
float __fastcall recovered_creature_spot_distance_004efc70(void* creature, void*);            // CSWSCreature::GetSpotCheckDistance
unsigned __fastcall recovered_area_index_00506f20(void* area, void*, I32* out, unsigned x_bits); // CSWSArea::GetFirstObjectIndiceByX
I32 __fastcall recovered_object_get_reputation_0057cb80(void* object, void*, U32 id, U32 flag); // CSWSObject::GetReputation
void* __fastcall recovered_creature_visible_element_00517a20(void* creature, void*, U32 id);  // CSWSCreature::GetVisibleListElement
U32 __fastcall kotor_fastcall_00578e00(void* map, void*, MapVec world, I32* x, I32* y);       // CSWSAreaMap::GetMapPixelFromWorldCoord
U32 __fastcall kotor_fastcall_00578f10(void* map, void*, I32 x, I32 y, I32* out_x, I32* out_y); // CSWSAreaMap::GetGridPixelFromMapPixel
// ---- strings / resources / lists
U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, U32 language, void* out, U32 gender); // CExoLocString::GetString
int __fastcall async_string_length_005e5790(void* string, void*);                             // CExoString::GetLength
void* __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out);                    // CResRef::CopyToString
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* text);           // CResRef(const CExoString&)
void __fastcall gui_load_image_params_set_image_00415900(void* params, void*, void* ref, U32) noexcept(false); // SetImage
void __fastcall gui_load_image_params_set_draw_style_004159c0(void* params, void*, U32 style); // SetDrawStyle
void __fastcall gui_load_text_params_set_alignment_00415f20(void* params, void*, U32 align);  // SetAlignment
void* __fastcall c2da_005e9d70(void* list, void*, void* data);                                // CExoLinkedListInternal::AddHead
std::uint32_t __fastcall kotor_fastcall_005e9bd0(void* list, void*, void* position);          // CExoLinkedListInternal::GetPrev
// ---- AurGUI layers
void __stdcall kotor_AurGUIStartLayer_004591B0();
unsigned __cdecl kotor_AurGUISetupViewport_004592F0(unsigned x, unsigned y, unsigned width, unsigned height,
                                                     const void* color, unsigned depth_test_zero, unsigned alpha_bits);
void __stdcall kotor_AurGUICloseViewport_00459580();
void __stdcall kotor_AurGUIStopLayer_004592B0();
extern const std::uint32_t scene3d_gui_scene_color_0078d3d8[3];                               // [0078D3D8]
// ---- colours (.data, never written)
extern const unsigned char borrowed_exit_color_007a23b4[12];                                  // [007A23B4]
extern const unsigned char kotor_m1_color_007a2408[12];                                       // [007A2408]
extern const unsigned char kotor_m1_color_007a2378[12];                                       // [007A2378]
extern const unsigned char kotor_m1_color_007a2384[12];                                       // [007A2384]

// ---- defined here
void __fastcall kotor_fastcall_00692810(void* self, void*, std::uint32_t delta_bits);
std::uint32_t __fastcall kotor_fastcall_00692930(void* self, void*, std::uint32_t x, std::uint32_t y);
void __fastcall kotor_fastcall_00693650(void* self, void*);
void __fastcall kotor_fastcall_00693ba0(void* self, void*);
void __fastcall kotor_fastcall_00693bc0(void* self, void*, std::uint32_t id, std::uint32_t down);
void __fastcall kotor_fastcall_00692bc0(void* self, void*, U32 unused);
void __fastcall kotor_fastcall_00692b00(void* self, void*, U32 unused);
void* __fastcall kotor_fastcall_00692c70(void* hider, void*, void* result, U32 flag);
void* __fastcall kotor_fastcall_00692e80(void* hider, void*, void* result);
void* __fastcall kotor_fastcall_00693090(void* hider, void*, void* result);
U32 __fastcall kotor_fastcall_005edc00(void* app, void*);
U32 __fastcall kotor_fastcall_00620320(void* internal, void*);
}

namespace {
constexpr unsigned char kGameObjectOk_0074666c = 0;                                           // .rdata 0074666C (byte 00h)
void* in_game_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }               // [[7A39FC]+4]->GetInGameGui
void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
void* vm() { return pt(frame_control::message_manager_007a3a00); }                           // [007A3A00]
I32 half_margin(I32 v, I32 base) { return (v - base) / 2; }                                  // sub; cdq; sub eax,edx; sar 1

struct Exo {
    U32 s[2]; bool live = false;
    Exo() = default;
    Exo(const Exo&) = delete;
    Exo& operator=(const Exo&) = delete;
    void def() { exo_default(s); live = true; }
    void text(const char* t) { frame_LogStringCtor_005e5a90(s, nullptr, t); live = true; }
    void born() { live = true; }
    void kill() { if (live) { live = false; frame_LogStringDtor_005e5c20(s, nullptr); } }
    ~Exo() { kill(); }
    SS* p() { return reinterpret_cast<SS*>(s); }
};
void tlk(Exo& out, U32 strref) {
    static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)->simple_0041e8f0(
        reinterpret_cast<kotor_menu_10_19::String*>(out.s), strref);
    out.born();
}
// Waypoint map-note text into `out` (GetString(+230, client language, out, 0)).
void map_note_text(void* waypoint, Exo& out) {
    void* client = client_app();                                                               // read before the calls
    exo_locstring_get_005ea130(at(waypoint, 0x230), nullptr, kotor_fastcall_005ed4c0(client, nullptr), out.s, 0);
}
bool explored(void* waypoint) {                                                                // [GetModule(server)+218] IsWorldPointExplored(+90)
    void* area_map = lp(server_get_module_004ae6b0(server_app(), nullptr), 0x218);
    MapVec world; std::memcpy(&world, at(waypoint, 0x90), 12);
    return kotor_fastcall_00579210(area_map, nullptr, world) != 0;
}
void* node_object(void* list, void* position) {                                                // GetGameObject(server, *GetAtPos(pos))
    const U32 id = lw(pt(unresolved_list_at_pos_005e9b60(list, nullptr, position)), 0);
    return kotor_fastcall_004ae750(server_app(), nullptr, id);
}
}  // namespace

extern "C" {
// ======================= CSWSAreaMap / CClientExoApp leaves =======================
// 00620320 CClientExoAppInternal::DismissInGameGUI: HideSWInGameGui(0) on +40; input class 0 when it was shown.
U32 __fastcall kotor_fastcall_00620320(void* internal, void*) {
    if (frame_R4_0062cba0(lp(internal, 0x40), nullptr, 0) == 0) return 0;                      // 00620323..0062032F / 00620343
    kotor_fastcall_006200e0(internal, nullptr, 0, 1);                                          // 00620331..00620337
    return 1;                                                                                  // 0062033C
}
// 005EDC00 CClientExoApp::DismissInGameGUI: mov ecx,[ecx+4]; jmp 00620320.
U32 __fastcall kotor_fastcall_005edc00(void* app, void*) { return kotor_fastcall_00620320(lp(app, 4), nullptr); }

// ======================= CSWGuiMapHider map notes (+238 CExoLinkedList<ulong>, +23C current position) ===========
// 00692C70 InitializeMapNotes(int flag) -> CExoString: [+244] = flag; every waypoint of the current server area with
// a map note (+228) is pushed (new DWORD id, AddHead); the first enabled (+22C) one at an explored point becomes
// the current note (+23C = list head) and its text is returned. Quirk kept: the 4-byte node is not null-checked.
void* __fastcall kotor_fastcall_00692c70(void* hider, void*, void* result, U32 flag) {
    sw(hider, 0x244, flag);                                                                    // 00692C88..00692CA3
    Exo text; text.text("");                                                                   // 00692C9A..00692CA9 [esp+1C] state 1 (0073D71C)
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                    // 00692CAE..00692CCD [esp+10]
    void* area = module_get_area_004c30f0(server_get_module_004ae6b0(server_app(), nullptr), nullptr); // 00692CC4..00692CDD (ebx)
    for (I32 i = 0; i < li(area, 0x194); ++i) {                                                // 00692CDF..00692CED / 00692DCD..00692DDE
        const U32 id = lw(lp(area, 0x190), static_cast<U32>(i) * 4u);                          // 00692CF3..00692CF9 (ebp)
        void* object = nullptr;                                                                // [esp+14]
        if (kotor_fastcall_004d8230(objects, nullptr, id, &object) != kGameObjectOk_0074666c) continue; // 00692CFC..00692D11
        void* waypoint = vcall<void*>(object, 0x58);                                           // 00692D17..00692D1D AsSWSWaypoint
        if (waypoint == nullptr || lw(waypoint, 0x228) == 0) continue;                         // 00692D20..00692D32
        U32* node = static_cast<U32*>(live_operator_new(4));                                   // 00692D38..00692D3F
        *node = id;                                                                            // 00692D42
        c2da_005e9d70(lp(hider, 0x238), nullptr, node);                                        // 00692D44..00692D4B AddHead
        if (lw(hider, 0x23C) != 0 || lw(waypoint, 0x22C) == 0) continue;                       // 00692D50..00692D62
        if (!explored(waypoint)) continue;                                                     // 00692D64..00692D9C
        sw(hider, 0x23C, lw(lp(hider, 0x238), 0));                                             // 00692D9E..00692DA6
        map_note_text(waypoint, text);                                                         // 00692DAC..00692DC8
    }
    WindowCopy005e5b00(result, nullptr, text.s);                                               // 00692DE4..00692DEF
    text.kill();                                                                               // 00692DF4..00692E05
    return result;                                                                             // 00692E0F
}

// 00692E80 GetNextMapNote() -> CExoString: text of the current note, then the next enabled explored note after it
// (wrapping to the head) becomes current and its text is returned. Stops when it reaches the current note again.
void* __fastcall kotor_fastcall_00692e80(void* hider, void*, void* result) {
    Exo text; text.text("");                                                                   // 00692E9D..00692EAE [esp+18] state 1
    void* list = lp(hider, 0x238);
    void* position = lp(hider, 0x23C);                                                         // 00692EB3..00692EC3 [esp+0C]
    const U32 id = position != nullptr ? lw(pt(unresolved_list_at_pos_005e9b60(list, nullptr, position)), 0) : 0x7F000000u; // 00692EC7..00692EDA
    void* current = kotor_fastcall_004ae750(server_app(), nullptr, id);                        // 00692EDC..00692EEA
    if (current != nullptr && vcall<void*>(current, 0x58) != nullptr)                          // 00692EEC..00692EF9
        map_note_text(vcall<void*>(current, 0x58), text);                                      // 00692EFB..00692F21
    bool scan = true;
    if (position != nullptr) {                                                                 // 00692F26..00692F2C
        unresolved_list_next_005e9b90(lp(hider, 0x238), nullptr, &position);                   // 00692F2E..00692F39
    }
    if (position == nullptr) {                                                                 // 00692F3E..00692F44
        position = lp(lp(hider, 0x238), 0);                                                    // 00692F46..00692F50 head
        if (position == nullptr) scan = false;                                                 // 00692F54
    }
    while (scan) {                                                                             // 00692F60
        void* stop = lp(hider, 0x23C);                                                         // 00692F60
        if (stop == nullptr || position == stop) break;                                        // 00692F66..00692F70
        void* object = node_object(lp(hider, 0x238), position);                                // 00692F76..00692F93
        void* waypoint = nullptr;
        if (object != nullptr && vcall<void*>(object, 0x58) != nullptr) {                      // 00692F95..00692FA2
            waypoint = vcall<void*>(object, 0x58);                                             // 00692FA4..00692FAB
            if (lw(waypoint, 0x22C) != 0 && explored(waypoint)) {                              // 00692FAD..00692FEF
                sw(hider, 0x23C, ad(position));                                                // 00693023..00693027
                map_note_text(waypoint, text);                                                 // 0069302D..00693048
                break;
            }
        }
        unresolved_list_next_005e9b90(lp(hider, 0x238), nullptr, &position);                   // 00692FF1..00692FFC
        if (position != nullptr) continue;                                                     // 00693001..00693007
        position = lp(lp(hider, 0x238), 0);                                                    // 0069300D..00693017
        if (position == nullptr) break;                                                        // 0069301B..00693021
    }
    WindowCopy005e5b00(result, nullptr, text.s);                                               // 0069304D..00693058
    text.kill();                                                                               // 0069305D..0069306E
    return result;
}

// 00693090 GetPrevMapNote() -> CExoString: as GetNextMapNote, walking backwards (GetPrev, wrapping to the tail +4).
void* __fastcall kotor_fastcall_00693090(void* hider, void*, void* result) {
    Exo text; text.text("");                                                                   // 006930AD..006930BE [esp+18] state 1
    void* list = lp(hider, 0x238);
    void* position = lp(hider, 0x23C);                                                         // 006930C3..006930D3 [esp+0C]
    const U32 id = position != nullptr ? lw(pt(unresolved_list_at_pos_005e9b60(list, nullptr, position)), 0) : 0x7F000000u; // 006930D7..006930EA
    void* current = kotor_fastcall_004ae750(server_app(), nullptr, id);                        // 006930EC..006930FA
    if (current != nullptr && vcall<void*>(current, 0x58) != nullptr)                          // 006930FC..00693109
        map_note_text(vcall<void*>(current, 0x58), text);                                      // 0069310B..00693131
    bool scan = true;
    if (position != nullptr) {                                                                 // 00693136..0069313C
        kotor_fastcall_005e9bd0(lp(hider, 0x238), nullptr, &position);                         // 0069313E..00693149
    }
    if (position == nullptr) {                                                                 // 0069314E..00693154
        position = lp(lp(hider, 0x238), 4);                                                    // 00693156..0069315F tail
        if (position == nullptr) scan = false;                                                 // 00693165
    }
    while (scan) {                                                                             // 00693170
        void* stop = lp(hider, 0x23C);                                                         // 00693170
        if (stop == nullptr || position == stop) break;                                        // 00693176..00693180
        void* object = node_object(lp(hider, 0x238), position);                                // 00693186..006931A3
        if (object != nullptr && vcall<void*>(object, 0x58) != nullptr) {                      // 006931A5..006931B2
            void* waypoint = vcall<void*>(object, 0x58);                                       // 006931B4..006931BB
            if (lw(waypoint, 0x22C) != 0 && explored(waypoint)) {                              // 006931BD..006931FF
                sw(hider, 0x23C, ad(position));                                                // 00693234..00693238
                map_note_text(waypoint, text);                                                 // 0069323E..00693259
                break;
            }
        }
        kotor_fastcall_005e9bd0(lp(hider, 0x238), nullptr, &position);                         // 00693201..0069320C
        if (position != nullptr) continue;                                                     // 00693211..00693217
        position = lp(lp(hider, 0x238), 4);                                                    // 0069321D..00693226
        if (position == nullptr) break;                                                        // 0069322C..00693232
    }
    WindowCopy005e5b00(result, nullptr, text.s);                                               // 0069325E..00693269
    text.kill();                                                                               // 0069326E..0069327F
    return result;
}

// ======================= CSWGuiInGameMap =======================
// 00692B00 ReturnToEbonHawk(int) (message-box callback): when the box (+98 of CGuiInGame) was accepted ([+64] bit 0)
// and the in-game GUI could be dismissed, runs "k_sup_gohawk" with [CGuiInGame+C04] = 1 around the call.
void __fastcall kotor_fastcall_00692b00(void* self, void*, U32 unused) {
    (void)self; (void)unused;
    void* box = lp(in_game_gui(), 0x98);                                                       // 00692B0E..00692B25
    if ((lb(box, 0x64) & 1u) == 0) return;                                                     // 00692B2B..00692B2F
    if (kotor_fastcall_005edc00(client_app(), nullptr) == 0) return;                           // 00692B31..00692B41
    sw(in_game_gui(), 0xC04, 1);                                                               // 00692B43..00692B5A
    Exo script; script.text("k_sup_gohawk");                                                   // 00692B51..00692B64 (007546E0)
    frame_R4_005d0fc0(vm(), nullptr, script.s, 0, 0);                                          // 00692B69..00692B80
    sw(in_game_gui(), 0xC04, 0);                                                               // 00692B85..00692B97
    script.kill();                                                                             // 00692B93..00692BA9
}

// 00692BC0 OnXButton(int): return-to-Ebon-Hawk confirmation (bit 1 of +10D8: party-table strref +108, callback
// 00692B00) or the "cannot" message (+10D4, default 9633h); the message box is then added as a modal panel.
void __fastcall kotor_fastcall_00692bc0(void* self, void*, U32 unused) {
    (void)unused;
    void* box;
    if ((lb(self, 0x10D8) & 2u) != 0) {                                                        // 00692BC4..00692BCB
        box = lp(in_game_gui(), 0x98);                                                         // 00692BCD..00692BDA
        kotor_fastcall_00627130(box, nullptr, 1);                                              // 00692BE0..00692BE4
        vcall<void, U32>(box, 0x6C, lw(party_table(), 0x108));                                 // 00692BE9..00692C02 SetText(strref)
        gui_load_messagebox_set_callback_00624a40(box, nullptr, self, fn(&kotor_fastcall_00692b00), 0); // 00692C05..00692C0C / 00692C54..00692C56
    } else {
        if (lw(self, 0x10D4) == 0) sw(self, 0x10D4, 0x9633);                                   // 00692C0F..00692C19
        box = lp(in_game_gui(), 0x98);                                                         // 00692C23..00692C31
        kotor_fastcall_00627130(box, nullptr, 0);                                              // 00692C37..00692C3B
        vcall<void, U32>(box, 0x6C, lw(self, 0x10D4));                                         // 00692C40..00692C4B
        gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);          // 00692C4E..00692C56
    }
    kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, box, 1, 1);                               // 00692C5B..00692C63
}

// 00692810 Draw(float): disabled return / party buttons get the disabled colours (bit 1 / bit 0 of +10D8 clear),
// CSWGuiPanel::Draw, then the map image (+1080) and the hider (+E38) inside a viewport at the panel extent (+68)
// shifted by half the screen margin over 640x480.
void __fastcall kotor_fastcall_00692810(void* self, void*, std::uint32_t delta_bits) {
    if ((lb(self, 0x10D8) & 2u) == 0) {                                                        // 00692818..00692820
        gui_load_text_params_set_color_00414e10(at(self, 0x6D0), nullptr, kotor_m1_color_007a2408); // 00692822..0069282D
        std::memcpy(at(self, 0x5F4), kotor_m1_color_007a2384, 12);                             // 00692832..0069284F
    }
    if ((lb(self, 0x10D8) & 1u) == 0) {                                                        // 00692852..00692859
        gui_load_text_params_set_color_00414e10(at(self, 0x894), nullptr, kotor_m1_color_007a2408); // 0069285B..00692866
        std::memcpy(at(self, 0x7B8), kotor_m1_color_007a2384, 12);                             // 0069286B..00692886
    }
    I32 x = li(self, 0x68), y = li(self, 0x6C);                                                // 0069288D..00692892
    const U32 width = lw(self, 0x70), height = lw(self, 0x74);                                 // 00692895..006928A2
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 00692889..006928A6
    void* manager = lp(self, 0x18);                                                            // 006928AB
    x += half_margin(static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6C))), 0x280);   // 006928AE..006928BC
    y += half_margin(static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6E))), 0x1E0);   // 006928BE..006928CC
    kotor_AurGUIStartLayer_004591B0();                                                         // 006928CE
    if (kotor_AurGUISetupViewport_004592F0(static_cast<U32>(x), static_cast<U32>(y), width, height,
                                           scene3d_gui_scene_color_0078d3d8, 0, 0x3F800000u) != 0) { // 006928D3..006928F5
        vcall<void, U32>(at(self, 0x1080), 0x0C, delta_bits);                                  // 006928F7..00692904 image Draw
        vcall<void, U32>(at(self, 0xE38), 0x38, delta_bits);                                   // 00692907..00692914 hider Draw
        kotor_AurGUICloseViewport_00459580();                                                  // 00692917
    }
    kotor_AurGUIStopLayer_004592B0();                                                          // 0069291C
}

// 00692930 HitCheckMouse(int x, int y): CSWGuiPanel hit first; otherwise the point (unshifted by the margin unless
// [+44] bit 0) relative to the panel extent origin goes to the hider's HitCheckMouse (slot 44h).
std::uint32_t __fastcall kotor_fastcall_00692930(void* self, void*, std::uint32_t x, std::uint32_t y) {
    const std::uint32_t hit = gui_load_panel_hit_check_0040b690(self, nullptr, static_cast<int>(x), static_cast<int>(y)); // 0069293E..00692942
    if (hit != 0) return hit;                                                                  // 00692947..00692949
    I32 px = static_cast<I32>(x), py = static_cast<I32>(y);
    if ((lb(self, 0x44) & 1u) == 0) {                                                          // 0069294B..0069294F
        void* manager = lp(self, 0x18);                                                        // 00692951
        px += -half_margin(static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6C))), 0x280); // 00692954..00692964
        py += -half_margin(static_cast<I32>(static_cast<std::int16_t>(lh(manager, 0x6E))), 0x1E0); // 00692966..00692976
    }
    py -= li(self, 0x6C);                                                                      // 0069297F..00692989
    px -= li(self, 0x68);                                                                      // 0069297D..00692998
    return vcall<std::uint32_t, I32, I32>(at(self, 0xE38), 0x44, px, py);                      // 0069298F..0069299F
}

// 00693650 OnPanelAdded: area name, map image ("lbl_map<module resref>" when the area map has data, else
// "lbl_mapm28aa"), map notes, the party-select enable state (+10D8 bit 0, reason strref +10D4) from the area
// (+2AC), party togetherness and nearby hostile visibility, the return enable state (bit 1) from the party table
// (+100), the button colours, CSWGuiPanel::OnPanelAdded and the map tutorial (0Dh).
void __fastcall kotor_fastcall_00693650(void* self, void*) {
    void* server = server_app();                                                               // 00693668..00693679 [esp+18]
    void* module = kotor_fastcall_005ed530(client_app(), nullptr);                             // 00693676..0069367D
    Exo area_name; WindowCopy005e5b00(area_name.s, nullptr, at(lp(module, 0x48), 0x17C)); area_name.born(); // 00693682..0069368F [esp+3C] state 0
    gui_load_text_params_set_text_00415e00(at(self, 0x28C), nullptr, area_name.s);             // 00693694..006936A5
    U8 resref[16];                                                                             // [esp+44]
    std::memcpy(resref, at(server_get_module_004ae6b0(server, nullptr), 0x30), 16);            // 006936AA..006936CF
    Exo resref_text; resref_text.def();                                                        // 006936CB..006936D3 [esp+34] state 1
    Exo image_name; image_name.def();                                                          // 006936D8..006936E1 [esp+2C] state 2
    c2da_ref_to_string_00405f70(resref, nullptr, resref_text.s);                               // 006936E6..006936F4
    frame_LogFormat_005e5680(image_name.s, "lbl_map%s", frame_LogCStr_005e5670(resref_text.s, nullptr)); // 006936F9..00693712 (00747774)
    void* area_map = lp(server_get_module_004ae6b0(server, nullptr), 0x218);                   // 00693715..0069371C
    U32 has_map = 0;                                                                           // ebx
    {
        menu_resources::Ref ref;                                                               // [esp+1C]
        if (lw(area_map, 4) != 0) {                                                            // 00693722..00693729 (jbe)
            async_ref_construct_string_00406d60(&ref, nullptr, image_name.s);                  // 0069372B..00693730
            gui_load_image_params_set_image_00415900(at(self, 0x10A8), nullptr, &ref, 0);      // 00693735..00693743
            has_map = 1;                                                                       // 00693748
        } else {
            ref.construct_00406d80("lbl_mapm28aa");                                            // 0069374F..00693754 (00754700)
            gui_load_image_params_set_image_00415900(at(self, 0x10A8), nullptr, &ref, 0);      // 00693759..00693767
        }
    }
    gui_load_image_params_set_draw_style_004159c0(at(self, 0x10A8), nullptr, 0);               // 0069376C..00693770
    {
        Exo note;                                                                              // [esp+1C] (reuses the resref slot) state 3
        kotor_fastcall_00692c70(at(self, 0xE38), nullptr, note.s, has_map); note.born();       // 00693775..00693781
        kotor_fastcall_006929b0(self, nullptr, note.s);                                        // 00693786..0069378E
        note.kill();                                                                           // 00693793..0069379C
    }
    void* player = kotor_fastcall_004ae770(server, nullptr, kotor_fastcall_004aea40(server, nullptr)); // 006937A1..006937AB
    void* area = recovered_object_area_004cb120(player, nullptr);                              // 006937B0..006937B2
    U32 flags = lw(self, 0x10D8);                                                              // 006937C1
    if (lw(area, 0x2AC) != 0) {                                                                // 006937B7..006937C7
        sw(self, 0x10D4, 0x9633);                                                              // 006937C9
        flags &= 0xFFFFFFFEu;                                                                  // 006937D3
    } else {
        sw(self, 0x10D4, 0);                                                                   // 006937D8
        flags |= 1u;                                                                           // 006937DE
    }
    sw(self, 0x10D8, flags);                                                                   // 006937E3
    U32 can_return = 0;                                                                        // eax at 00693A55
    if ((flags & 1u) != 0) {                                                                   // 006937E1..006937E9
        if (kotor_fastcall_00635350(kotor_fastcall_005ed8b0(client_app(), nullptr), nullptr) == 0) { // 006937EF..00693805
            sw(self, 0x10D4, 0x9634);                                                          // 00693810
            sw(self, 0x10D8, lw(self, 0x10D8) & 0xFFFFFFFEu);                                  // 00693807..0069381A
        }
        if ((lb(self, 0x10D8) & 1u) != 0) {                                                    // 00693820..00693827
            float best = bits_f(0x7F7FFFFFu);                                                  // 00693836 [esp+10] FLT_MAX
            const U32 player_id = kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr); // 0069382D..0069383E
            const U32 server_id = kotor_fastcall_004aea30(server_app(), nullptr, player_id);   // 00693843..0069384D
            void* leader = kotor_fastcall_004ae770(server_app(), nullptr, server_id);          // 00693852..0069385C [esp+1C]
            if (li(kotor_fastcall_005ed8b0(client_app(), nullptr), 0) > 0) {                   // 00693861..00693877 (signed)
                U32 member_off = 0;                                                            // ebp
                for (I32 i = 0; i < li(kotor_fastcall_005ed8b0(client_app(), nullptr), 0); ++i, member_off += 0x88) { // 006938DE..006938F4
                    const U32 id = lw(kotor_fastcall_005ed8b0(client_app(), nullptr), member_off + 0x24); // 00693880..0069388D
                    void* member = kotor_fastcall_004ae770(server_app(), nullptr,
                                                           kotor_fastcall_004aea30(server_app(), nullptr, id)); // 00693891..006938AA
                    if (member == nullptr) continue;                                           // 006938AF..006938B3
                    const double distance = static_cast<double>(lf(member, 0x90)) -
                                            static_cast<double>(recovered_creature_spot_distance_004efc70(member, nullptr)); // 006938B5..006938BC fsubr
                    if (jcc_jp(distance, static_cast<double>(best), 0x05)) continue;           // 006938C2..006938CB (update iff <)
                    best = static_cast<float>(static_cast<double>(lf(member, 0x90)) -
                                              static_cast<double>(recovered_creature_spot_distance_004efc70(member, nullptr))); // 006938CD..006938DA
                }
            }
            void* leader_area = recovered_object_area_004cb120(leader, nullptr);               // 006938FA..006938FE (esi, [esp+1C])
            I32 index;                                                                         // [esp+14]
            recovered_area_index_00506f20(leader_area, nullptr, &index, f_bits(best));         // 00693903..00693915
            if ((lb(self, 0x10D8) & 1u) != 0) {                                                // 0069391A..00693921
                do {
                    if (index >= li(leader_area, 0x194)) break;                                // 00693930..00693936 (signed)
                    void* object = kotor_fastcall_004ae770(server_app(), nullptr,
                                                           lw(lp(leader_area, 0x190), static_cast<U32>(index) * 4u)); // 0069393C..0069394F (ebx)
                    if (object != nullptr && li(kotor_fastcall_005ed8b0(client_app(), nullptr), 0) > 0) { // 00693954..00693977
                        U32 member_off = 0;                                                    // ebp
                        for (I32 j = 0; j < li(kotor_fastcall_005ed8b0(client_app(), nullptr), 0); ++j, member_off += 0x88) { // 006939EE..00693A0C ([esp+10] reused)
                            const U32 id = lw(kotor_fastcall_005ed8b0(client_app(), nullptr), member_off + 0x24); // 00693980..0069398E
                            void* member = kotor_fastcall_004ae770(server_app(), nullptr,
                                                                   kotor_fastcall_004aea30(server_app(), nullptr, id)); // 00693992..006939AB
                            if (member == nullptr) continue;                                   // 006939B0..006939B4
                            if (recovered_object_get_reputation_0057cb80(member, nullptr, lw(object, 4), 0) > 10) continue; // 006939B6..006939C6 (signed)
                            void* seen = recovered_creature_visible_element_00517a20(object, nullptr, lw(member, 4)); // 006939C8..006939CE
                            if (seen == nullptr || (lb(seen, 4) & 1u) == 0) continue;          // 006939D3..006939DB
                            sw(self, 0x10D8, lw(self, 0x10D8) & 0xFFFFFFFEu);                  // 006939DD
                            sw(self, 0x10D4, 0x963E);                                          // 006939E4
                        }
                    }
                    ++index;                                                                   // 00693A1A..00693A28
                } while ((lb(self, 0x10D8) & 1u) != 0);                                        // 00693A1E..00693A2C
                if ((lb(self, 0x10D8) & 1u) != 0)                                              // 00693A32..00693A39
                    can_return = lw(lane_config_required::GetPartyTable_004aee70(server, nullptr), 0x100) != 0 ? 1u : 0u; // 00693A3B..00693A51
            }
        }
    }
    {
        const U32 old = lw(self, 0x10D8);                                                      // 00693A55
        sw(self, 0x10D8, old ^ (((can_return + can_return) ^ old) & 2u));                      // 00693A5B..00693A69
    }
    void* return_text = at(self, 0x6D0);                                                       // 00693A7A
    gui_load_text_params_set_strref_00415e50(return_text, nullptr,
        lw(lane_config_required::GetPartyTable_004aee70(server, nullptr), 0x104));             // 00693A67..00693A83
    gui_load_text_params_set_color_00414e10(return_text, nullptr,
        (lb(self, 0x10D8) & 2u) != 0 ? borrowed_exit_color_007a23b4 : kotor_m1_color_007a2408); // 00693A88..00693A9E
    {
        const U32 flags_now = lw(self, 0x10D8);                                                // 00693AA3
        std::memcpy(at(self, 0x5F4), (flags_now & 2u) != 0 ? kotor_m1_color_007a2378 : kotor_m1_color_007a2384, 12); // 00693AA9..00693ACE
        gui_load_text_params_set_color_00414e10(at(self, 0x894), nullptr,
            (flags_now & 1u) != 0 ? borrowed_exit_color_007a23b4 : kotor_m1_color_007a2408);   // 00693AB8..00693AE6
    }
    std::memcpy(at(self, 0x7B8), (lb(self, 0x10D8) & 1u) != 0 ? kotor_m1_color_007a2378 : kotor_m1_color_007a2384, 12); // 00693AEB..00693B11
    if ((lw(kotor_fastcall_005ed700(client_app(), nullptr), 0x14) & 0x100u) != 0) {          // 00693B14..00693B28
        gui_load_text_params_set_alignment_00415f20(at(self, 0x894), nullptr, 0x12);          // 00693B2A..00693B2E
        gui_load_text_params_set_alignment_00415f20(return_text, nullptr, 0x12);              // 00693B33..00693B37
    }
    gui_load_panel_on_added_0040b870(self, nullptr);                                           // 00693B3C..00693B3E
    show_tutorial_window_005edf40(client_app(), nullptr, 0x0D, 0x7F000000u, 0x7F000000u, 0);   // 00693B43..00693B5A
    image_name.kill();                                                                         // 00693B5F..00693B68
    resref_text.kill();                                                                        // 00693B6D..00693B76
    area_name.kill();                                                                          // 00693B7B..00693B87
}

// 00693BA0 OnPanelRemoved: ClearMapNotes on the hider, then CSWGuiPanel::OnPanelRemoved.
void __fastcall kotor_fastcall_00693ba0(void* self, void*) {
    kotor_fastcall_00692e30(at(self, 0xE38), nullptr);                                         // 00693BA3..00693BA9
    gui_load_panel_on_removed_0040c170(self, nullptr);                                         // 00693BAE..00693BB1
}

// 00693BC0 HandleInputEvent(int id, int down). Jump table 00693D9C / index bytes 00693DB4 (id - 27h, 0..B8h):
//   27h -> party selection (bit 0 of +10D8) or the "cannot" message box;  28h/2Dh/2Eh/DFh -> HideSWInGameGui(0);
//   29h -> OnXButton unless the 27h tutorial window shows;  31h/3Dh -> previous note;  32h/3Eh -> next note.
// Every path ends in CSWGuiPanel::HandleInputEvent(id, down).
void __fastcall kotor_fastcall_00693bc0(void* self, void*, std::uint32_t id, std::uint32_t down) {
    if (down != 0) {                                                                           // 00693BD9..00693BE7
        switch (id) {                                                                          // 00693BED..00693C02
        case 0x27:                                                                             // 00693C44
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                               // 00693C44..00693C49
            if ((lb(self, 0x10D8) & 1u) != 0) {                                                // 00693C4E..00693C55
                Exo script; script.text("");                                                   // 00693C57..00693C60 [esp+0C] (0073D71C)
                kotor_fastcall_0062dd20(in_game_gui(), nullptr, script.s, 0, -1, -1);          // 00693C65..00693C88
                script.kill();                                                                 // 00693C8D..00693D77
            } else {
                void* box = lp(in_game_gui(), 0x98);                                           // 00693C96..00693CA5
                kotor_fastcall_00627130(box, nullptr, 0);                                      // 00693CAB..00693CAF
                vcall<void, U32>(box, 0x6C, lw(self, 0x10D4));                                 // 00693CB4..00693CBF
                gui_load_messagebox_set_callback_00624a40(box, nullptr, nullptr, nullptr, 0);  // 00693CC2..00693CCA
                kotor_fastcall_0040bc70(lp(self, 0x18), nullptr, box, 1, 1);                   // 00693CCF..00693CD7
            }
            break;
        case 0x28: case 0x2D: case 0x2E: case 0xDF:                                            // 00693CE2
            if (frame_R4_0062cba0(in_game_gui(), nullptr, 0) != 0)                             // 00693CE2..00693CFB
                kotor_fastcall_005eda60(client_app(), nullptr, 0, 1);                          // 00693CFD..00693D0A
            break;
        case 0x29:                                                                             // 00693C09
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 0);                               // 00693C09..00693C0E
            if (show_tutorial_window_005edf40(client_app(), nullptr, 0x27, 0x7F000000u, 0x7F000000u, 0) == 0) // 00693C13..00693C31
                kotor_fastcall_00692bc0(self, nullptr, 0);                                     // 00693C37..00693C3A
            break;
        case 0x31: case 0x3D: {                                                                // 00693D11
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                               // 00693D11..00693D16
            Exo note;                                                                          // [esp+14] state 1
            kotor_fastcall_00693090(at(self, 0xE38), nullptr, note.s); note.born();            // 00693D1B..00693D26
            kotor_fastcall_006929b0(self, nullptr, note.s);                                    // 00693D2B..00693D36
            note.kill();                                                                       // 00693D3B..00693D77
            break;
        }
        case 0x32: case 0x3E: {                                                                // 00693D41
            kotor_fastcall_0040a140(lp(self, 0x18), nullptr, 1);                               // 00693D41..00693D46
            Exo note;                                                                          // [esp+1C] state 2
            kotor_fastcall_00692e80(at(self, 0xE38), nullptr, note.s); note.born();            // 00693D4B..00693D56
            kotor_fastcall_006929b0(self, nullptr, note.s);                                    // 00693D5B..00693D66
            note.kill();                                                                       // 00693D6B..00693D77
            break;
        }
        default:
            break;
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, id, down);                             // 00693D7C..00693D80
}
}  // extern "C"
