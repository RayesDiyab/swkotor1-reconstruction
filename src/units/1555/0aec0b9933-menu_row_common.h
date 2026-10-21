#pragma once
#include "98d2400bef-menu_rows.h"
#include "6dab7adcb5-menu_cohort.h"
#include "3f38d05b1d-menu_callbacks.h"
#include "00a0946121-resources.h"
#include "ed6f7a65b7-startup_string.h"
#include <cstring>
#include <cstdint>
#include <new>

using MenuU = std::uint32_t;
using MenuI = std::int32_t;
using MenuString = kotor_reconstruction::StartupString;
extern "C" {
extern void* kotor_cell_rules_007a3a28;
MenuU __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void*,void*,MenuU);
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void*,void*);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*,void*) noexcept(false);
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void*,void*,MenuI) noexcept(false);
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void*,void*,MenuI) noexcept(false);
void* __fastcall kotor_m15b_item_get_base_item_005b4790(void*,void*) noexcept(false);
MenuI __fastcall kotor_m15b_creature_stats_has_feat_00649340(void*,void*,std::uint16_t) noexcept(false);
void* __fastcall recovered_object_area_004cb120(void*, void*);
void* __fastcall recovered_creature_inventory_004ef770(void*, void*, MenuU);
void __fastcall kotor_fastcall_004fddd0(void*, void*, MenuI);
MenuU __fastcall recovered_creature_can_use_item_0051b440(void*, void*, void*, MenuU);
MenuU __fastcall recovered_item_property_query_005539c0(void*, void*, void**, MenuU, MenuU);
void* __fastcall w2x_inventory_item_005560b0(void*, void*, MenuU);
MenuU __fastcall recovered_inventory_has_item_005a4950(void*, void*, void*);
MenuU __fastcall recovered_stats_has_feat_005a6630(void*, void*, MenuU);
MenuU __fastcall recovered_stats_can_use_skill_005af880(void*, void*, MenuU);
void* __fastcall recovered_base_item_get_icon_resref_005b3050(void*, void*, void*, MenuU, MenuU) noexcept(false);
char* __fastcall WindowCStr005e5670(MenuString*, void*);
MenuU __fastcall exo_locstring_get_005ea130(void*, void*, MenuU, MenuString*, MenuU);
MenuU __fastcall kotor_fastcall_005ed4c0(void*, void*);
void* __fastcall kotor_fastcall_005ed580(void*, void*, MenuU);
void* __fastcall kotor_fastcall_005ed8b0(void*, void*);
MenuString* __fastcall kotor_fastcall_005edeb0(void*, void*, MenuString*, MenuU) noexcept(false);
void* __fastcall frame_GetServerArea_00605640(void*, void*);
void* __fastcall frame_GetCharacter_006346c0(void*, void*, MenuI);
}
namespace kotor_rules { void* __fastcall kotor_fastcall_00550c00(void*, void*, std::uint16_t); }
namespace resource_ref_assignment {
menu_resources::Ref* __fastcall assign_00406120(menu_resources::Ref*, void*, menu_resources::Ref*, const menu_resources::Ref*);
}
namespace recovered_save_stats { std::uint16_t maximum_hp_00647a80(void*); }
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
MenuString* __fastcall GuiConcat005e5d10(MenuString*, void*, MenuString*, const MenuString*);
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }

namespace menu_row {
using U=MenuU; using I=MenuI; using String=MenuString;
inline void* at(const void* p,U offset=0) {
    const U address=static_cast<U>(reinterpret_cast<std::uintptr_t>(p))+offset;
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(address));
}
template<class T> T load(const void* p,U offset=0) { T v;std::memcpy(&v,at(p,offset),sizeof v);return v; }
template<class T> void store(void* p,U offset,T v) { std::memcpy(at(p,offset),&v,sizeof v); }
inline void* client() { return load<void*>(kotor_menu_window_cells::manager_007a39fc,4); }
inline void* server_creature(void* self) {
    if(kotor_m15a_object_get_server_object_0063d4b0(self,nullptr)==nullptr)return nullptr;
    void* object=kotor_m15a_object_get_server_object_0063d4b0(self,nullptr);
    return load<const menu_server::Slots*>(object)->as_creature_030(object,nullptr);
}
inline void* row(void* list,U offset) { return at(load<void*>(list),offset); }
inline void set_callback(void* list,U offset,menu_action::CallbackRecord record) {
    store(row(list,offset),0xC,record);
}
// Hidden string returns begin their lifetime in the selected provider. This
// guard is installed only after that provider returns successfully.
struct StringStorage {
    alignas(String) unsigned char bytes[sizeof(String)];
    String* get(){return static_cast<String*>(static_cast<void*>(bytes));}
};
struct StringCleanup {
    String* value;
    ~StringCleanup() noexcept(false) {value->~String();}
};
inline void assign_row_label(void* list,U offset,String* label) {
    static_cast<String*>(row(list,offset))->Assign005e5c50(label);
}
inline void item_icon(void* item,void* destination) {
    menu_resources::Ref made,discarded;
    menu_resources::Ref* icon=kotor_fastcall_005556b0(item,nullptr,&made);
    resource_ref_assignment::assign_00406120(static_cast<menu_resources::Ref*>(destination),nullptr,&discarded,icon);
}
}
