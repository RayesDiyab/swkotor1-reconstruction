#pragma once
// Lane W4 GUI successor (worker "gui-inv"): declarations shared by claude_gui_inv_container.cpp,
// claude_gui_inv_inventory.cpp and claude_gui_inv_equip.cpp. Only existing candidate-map providers (c24 kotor.map,
// none in the c24 trap census) and the functions defined in those three files are declared here.
#include "78d2073e39-gui_panels.hpp"

namespace kotor_reconstruction { struct StartupString; struct ArrayMutations; }
namespace gui_inv { using SS = kotor_reconstruction::StartupString; }
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs); // CExoString::operator+
namespace menu_list_growth {
void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations* list, void*, unsigned capacity); // CExoArrayList::Allocate
}
namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void* server, void*); }   // CServerExoApp::GetPartyTable
namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }                       // [007A39F4] CSWGuiManager*
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };                 // CTlkTable::GetSimpleString
}

extern "C" {
// ---- GUI manager / panel / controls
void __fastcall kotor_fastcall_0040a140(void* manager, void*, w4::U32 sound);                     // CSWGuiManager::PlayGuiSound
void __fastcall gui_m48_manager_store_mouse_0040a3d0(void* manager, void*);                       // StoreCurrentMousePosition
void __fastcall gui_m48_manager_move_mouse_0040c790(void* manager, void*, w4::I32 x, w4::I32 y);  // MoveMouseToPosition
void __fastcall gui_m48_manager_restore_mouse_0040c7d0(void* manager, void*);                     // RestoreOldMousePosition
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel, w4::U32, w4::U32);     // CSWGuiManager::AddPanel
void __fastcall gui_load_panel_on_added_0040b870(void* panel, void*);                             // CSWGuiPanel::OnPanelAdded
void __fastcall gui_load_panel_on_removed_0040c170(void* panel, void*);                           // CSWGuiPanel::OnPanelRemoved
void __fastcall gui_load_panel_handle_input_00409e60(void* panel, void*, w4::U32 event, w4::U32 down); // CSWGuiPanel::HandleInputEvent
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, w4::U32 arg);                       // CSWGuiPanel::Draw
void __fastcall gui_load_listbox_clear_items_00419a90(void* listbox, void*);                      // CSWGuiListBox::ClearItems
void __fastcall gui_load_listbox_set_selected_0041c040(void* listbox, void*, w4::I32 index, w4::I32 flag); // SetSelectedControl
void __fastcall kotor_fastcall_0041c1d0(void* listbox, void*, void* list, w4::U32, w4::U32, w4::U32); // CSWGuiListBox::AddControls
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, void* object, w4::U32, w4::U32); // SetCallback
w4::U32 __fastcall frame_R4_0062cba0(void* gui, void*, w4::U32 flag);                             // CGuiInGame::HideSWInGameGui
void __fastcall kotor_fastcall_005eda60(void* app, void*, w4::U32, w4::U32);                     // CClientExoApp::SetInputClass
// ---- client / server apps
void* __fastcall kotor_fastcall_005ed6f0(void* app, void*);                                       // CClientExoApp::GetSWCMessage
void* __fastcall recovered_server_info_004aec90(void* server, void*);                             // CServerExoApp::GetServerInfo
void* __fastcall recovered_party_repository_00563340(void* table, void*);                         // CSWPartyTable::GetPartyItems
w4::U32 __fastcall recovered_npc_object_00564700(void* table, void*, w4::I32 index, w4::U32, w4::U32); // CSWPartyTable::GetNPCObject
w4::U32 __fastcall kotor_fastcall_005636b0(void* table, void*, w4::I32 index);                    // CSWPartyTable::GetIsNPCAvailable
w4::U32 __fastcall kotor_fastcall_005eda50(void* app, void*, w4::U32 id);                         // ServerToClientObjectId
w4::U32 __fastcall kotor_fastcall_004aea30(void* server, void*, w4::U32 id);                      // ClientToServerObjectId
void* __fastcall kotor_fastcall_005ed8b0(void* app, void*);                                       // CClientExoApp::GetSWParty
void* __fastcall kotor_fastcall_00635460(void* party, void*);                                     // CSWParty::GetPlayerCharacter
void* __fastcall frame_GetCharacter_006346c0(void* party, void*, w4::U32 index);                  // CSWParty::GetCharacter
w4::U32 __fastcall kotor_fastcall_004aea40(void* server, void*);                                  // CServerExoApp::GetPlayerCreatureId
void* __fastcall kotor_fastcall_004ae770(void* server, void*, w4::U32 id);                        // CServerExoApp::GetCreatureByGameObjectID
void* __fastcall recovered_item_lookup_004ae760(void* server, void*, w4::U32 id);                 // CServerExoApp::GetItemByGameObjectID
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void* app, void*, w4::U32 id);          // CClientExoApp::GetItemByGameObjectID
w4::U32 __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, w4::U32 slot); // GetEquippedItemID
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);         // CSWCCreature::GetServerCreature
void* __fastcall kotor_m15closure_00646c90(void* item, void*);                                    // CSWCItem::GetServerItem
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void* item, void*);                       // CSWItem::GetBaseItem
// ---- items / repositories
w4::U32 __fastcall kotor_fastcall_005560e0(void* repository, void*, w4::I32 index);               // ItemListGetItemObjectID
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, w4::U32 index);             // ItemListGetItem
void* __fastcall kotor_fastcall_006b7ee0(void* entry, void*);                                     // CSWGuiInGameItemEntry ctor
void __fastcall kotor_fastcall_006b6710(void* entry, void*, w4::U32 id, w4::U32 equipped, w4::U32 marked); // SetItem
// ---- strings
void* __fastcall kotor_fastcall_005edeb0(void* app, void*, void* out, w4::U32 id);               // CClientExoApp::GetGUIString
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, w4::I32 value);                       // CExoString::CExoString(int)
w4::U32 __fastcall frame_LogIsEmpty_005e58a0(void* string, void*);                                // CExoString::IsEmpty
// ---- GUI params
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const void* ref, w4::I32) noexcept(false); // SetFillImage
w4::U32 __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                         // CSWGuiText::GetFontHeight
extern const unsigned char borrowed_exit_color_007a23b4[12];                                     // [007A23B4]
extern const unsigned char kotor_m1_color_007a2408[12];                                          // [007A2408]
extern const unsigned char kotor_m1_color_007a2384[12];                                          // [007A2384]
}

namespace gui_inv {
using namespace w4;
using namespace w4gui;
inline void* gui_manager() { return kotor_input_cells::gui_manager_007a39f4; }                   // [007A39F4]
inline void* party_table() { return lane_config_required::GetPartyTable_004aee70(server_app(), nullptr); }
inline void* ingame_gui() { return pt(frame_R4_005ed690(client_app(), nullptr)); }
inline void play_sound(void* panel, U32 sound) { kotor_fastcall_0040a140(lp(panel, 0x18), nullptr, sound); } // [this+18h]

// CExoString living on the caller's stack: constructed by a callee (by-value return) or by a ctor call, destroyed by
// ~CExoString when it goes out of scope (also while unwinding, = the original EH state's cleanup).
struct Str {
    U32 s[2]; bool live = false;
    Str() = default;
    explicit Str(const char* text) { frame_LogStringCtor_005e5a90(s, nullptr, text); live = true; }
    Str(const Str&) = delete; Str& operator=(const Str&) = delete;
    ~Str() { if (live) frame_LogStringDtor_005e5c20(s, nullptr); }
    void empty() { w4gui::exo_default(s); live = true; }                                       // CExoString() 005B3190
    void* p() { return s; }
    SS* ss() { return static_cast<SS*>(static_cast<void*>(s)); }
    void destroy() { live = false; frame_LogStringDtor_005e5c20(s, nullptr); }
};
// GetGUIString(out, id) into a stack temporary.
inline void* gui_string(Str& out, U32 id) { void* r = kotor_fastcall_005edeb0(client_app(), nullptr, out.s, id); out.live = true; return r; }
// a + b into out (operator+ constructs out).
inline void* concat(void* a, Str& out, void* b) {
    void* r = GuiConcat005e5d10(static_cast<SS*>(a), nullptr, out.ss(), static_cast<SS*>(b)); out.live = true; return r;
}

// Stack CExoArrayList<CSWGuiControl*> {data, size, capacity}; the destructor is the original _free(data).
struct PtrList {
    void** data = nullptr; I32 size = 0; I32 capacity = 0;
    PtrList() = default; PtrList(const PtrList&) = delete; PtrList& operator=(const PtrList&) = delete;
    ~PtrList() { live_free(data); }
};
static_assert(sizeof(PtrList) == 12, "CExoArrayList");
// Inlined CExoArrayList::Add: grow to capacity*2 (0x10 when empty) through Allocate when full, then append.
inline void list_add(void* list, void* value) {
    const I32 capacity = li(list, 8);
    if (li(list, 4) == capacity)
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            capacity != 0 ? static_cast<U32>(capacity) * 2u : 0x10u);
    sw(lp(list, 0), static_cast<U32>(li(list, 4)) * 4u, ad(value));
    sw(list, 4, lw(list, 4) + 1u);
}
// operator new(0x39C) + CSWGuiInGameItemEntry::CSWGuiInGameItemEntry (EH state: operator delete on a throwing ctor).
inline void* new_item_entry() {
    void* memory = live_operator_new(0x39c);
    if (memory == nullptr) return nullptr;
    auto guard = w4gui::on_throw([&]() { live_free(memory); });
    void* entry = kotor_fastcall_006b7ee0(memory, nullptr);
    guard.armed = false;
    return entry;
}
}  // namespace gui_inv
