extern "C" unsigned __fastcall recovered_party_is_member_00563790(void*,void*,unsigned); extern "C" void* __fastcall recovered_party_repository_00563340(void*,void*);
#include "bce6ba5f75-creature_inventory_get.hpp"
#include <cstring>
namespace lane_config_required{void* __fastcall GetPartyTable_004aee70(void*,void*);}
namespace kotor_menu_window_cells{extern void* volatile manager_007a39fc;}
extern "C"{void* __fastcall frame_R4_005ed690(void*,void*);unsigned __fastcall recovered_gui_in_game_get_party_access_panel_up_0062b4d0(void*,void*,unsigned);}
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}U rd(U a){U v;std::memcpy(&v,ptr(a),4);return v;}
struct Native:recovered_creature_inventory_get::Ports{U read(U a)override{return rd(a);}U byte(U a)override{return *static_cast<unsigned char*>(ptr(a));}U server()override{return rd(word(kotor_menu_window_cells::manager_007a39fc)+8);}U client()override{return rd(word(kotor_menu_window_cells::manager_007a39fc)+4);}U party(U a)override{return word(lane_config_required::GetPartyTable_004aee70(ptr(a),nullptr));}U shared(U a)override{return word(recovered_party_repository_00563340(ptr(a),nullptr));}U module(U a)override{return rd(rd(a+4)+0x10004);}U member(U a,U b)override{return recovered_party_is_member_00563790(ptr(a),nullptr,b);}U gui(U a)override{return word(frame_R4_005ed690(ptr(a),nullptr));}U gui_state(U a,U b)override{return recovered_gui_in_game_get_party_access_panel_up_0062b4d0(ptr(a),nullptr,b);}};}
extern "C" void* __fastcall recovered_creature_inventory_004ef770(void*self,void*,unsigned shared){Native p;return ptr(recovered_creature_inventory_get::get(word(self),shared,p));}
