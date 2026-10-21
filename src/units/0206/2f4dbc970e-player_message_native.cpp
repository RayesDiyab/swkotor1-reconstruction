extern "C" unsigned __fastcall recovered_player_to_server_character_download_00525350(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_char_list_005246f0(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_level_up_00527560(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_journal_005278d0(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_map_pin_00527380(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_play_module_character_list_00527890(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_player_death_005252a0(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_shut_down_server_00525390(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_chat_00520a10(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_quick_chat_005209b0(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall recovered_player_to_server_character_sheet_00523380(void*,void*,void*,unsigned); // p2s-a
extern "C" unsigned __fastcall recovered_player_to_server_gui_container_005232a0(void*,void*,void*,unsigned); // p2s-a
extern "C" unsigned __fastcall recovered_player_to_server_inventory_00523c20(void*,void*,void*,unsigned); // p2s-a
extern "C" unsigned __fastcall recovered_player_to_server_gold_00523b20(void*,void*,void*,unsigned); // p2s-a
extern "C" unsigned __fastcall recovered_player_to_server_party_00526910(void*,void*,void*,unsigned); // p2s-a
extern "C" unsigned __fastcall recovered_player_to_server_game_object_update_005228d0(void*,void*,void*,unsigned); // p2s-a
extern "C" unsigned __fastcall recovered_player_to_server_gui_inventory_00523240(void*,void*,void*,unsigned); // p2s-a
extern "C" unsigned __fastcall recovered_player_to_server_dialog_00522810(void*,void*,void*,unsigned); // p2s-b
extern "C" unsigned __fastcall w5_message_input_dispatch_005254c0(void*,void*,void*,unsigned); // CSWSMessage::HandlePlayerToServerInputMessage (W5 changed-dispatcher)
#include "779dd90306-player_message.hpp"
#include <cstring>
#include <cstdio>
#if !defined(_M_IX86)
#error Player message native bridge requires x86
#endif
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace kotor_reconstruction {struct StartupString;void __cdecl StartupFormat005e5680(StartupString*,const char*,...);}
extern "C" {
unsigned __fastcall recovered_server_login_005244e0(void*,void*,void*,unsigned);

void __fastcall GuiAssign005e5140(void*,void*,const char*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
const char* __fastcall frame_LogCStr_005e5670(void*,void*);
void* __fastcall kotor_fastcall_004ae920(void*,void*,unsigned,unsigned);
unsigned __fastcall recovered_player_message_00524c40(void*,void*,void*,unsigned);
unsigned __fastcall recovered_module_message_00524800(void*,void*,void*,unsigned); // W2 G08
unsigned __fastcall recovered_area_message_00524b80(void*,void*,void*,unsigned);   // W2 G08
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long,const char*);
}
namespace {
using namespace recovered_player_message;
const char* literal(unsigned a){switch(a){
case 0x00745094:return "ServerStatus";
case 0x00748258:return "Login";
case 0x007442a8:return "Module";
case 0x007442b0:return "Area";
case 0x0074819c:return "GameObjUpdate";
case 0x00748250:return "Input";
case 0x00748248:return "Gold";
case 0x00748240:return "Chat";
case 0x00748234:return "Inventory";
case 0x00748224:return "GuiInventory";
case 0x0074820c:return "Party";
case 0x00748204:return "Cheat";
case 0x007481f8:return "CharList";
case 0x007481f0:return "Dialog";
case 0x007481dc:return "GuiCharacterSheet";
case 0x007481d0:return "QuickChat";
case 0x00748214:return "GuiContainer";
case 0x007481c0:return "Journal";
case 0x007481c8:return "LevelUp";
case 0x007481b4:return "GuiQuickbar";
case 0x007481ac:return "MapPin";
case 0x00748194:return "Death";
case 0x00748180:return "Character_Download";
case 0x00748170:return "ShutDownServer";
case 0x00748158:return "PlayModuleCharacterList";
default:kotor_unreachable_trap(a,"Missing player-message diagnostic literal");}}
struct Native:Ports{
void string_construct(String*s)override{std::memset(s,0,8);}void string_destroy(String*s)override{frame_LogStringDtor_005e5c20(s,nullptr);}
void initialize_read(void*s,void*d,unsigned n,unsigned a,unsigned b)override{initialize_read_004d66b0(s,d,n,a,b);}unsigned overrun(void*s,unsigned)override{return overrun_004d6230(s);}unsigned underrun(void*s,unsigned)override{return underrun_004d6240(s);}
void*player(unsigned id,unsigned mode)override{void*app=kotor_menu_window_cells::manager_007a39fc;void*server;std::memcpy(&server,static_cast<unsigned char*>(app)+8,4);void* found=kotor_fastcall_004ae920(server,nullptr,id,mode);return found;}
void format_unknown(String*s,unsigned v)override{kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(s),"unknown Major (0x%.2X)",v);}void assign_label(String*s,unsigned a)override{GuiAssign005e5140(s,nullptr,literal(a));}
unsigned handler(unsigned a,void*s,void*p,unsigned minor)override{if(a==0x005244e0)return recovered_server_login_005244e0(s,nullptr,p,minor);if(a==0x00524c40)return recovered_player_message_00524c40(s,nullptr,p,minor);if(a==0x00524800)return recovered_module_message_00524800(s,nullptr,p,minor);if(a==0x00524b80)return recovered_area_message_00524b80(s,nullptr,p,minor);if(a==0x005254c0)return w5_message_input_dispatch_005254c0(s,nullptr,p,minor);if(a==0x00522810)return recovered_player_to_server_dialog_00522810(s,nullptr,p,minor);if(a==0x00523240)return recovered_player_to_server_gui_inventory_00523240(s,nullptr,p,minor);if(a==0x005228d0)return recovered_player_to_server_game_object_update_005228d0(s,nullptr,p,minor);if(a==0x00526910)return recovered_player_to_server_party_00526910(s,nullptr,p,minor);if(a==0x00523b20)return recovered_player_to_server_gold_00523b20(s,nullptr,p,minor);if(a==0x00523c20)return recovered_player_to_server_inventory_00523c20(s,nullptr,p,minor);if(a==0x005232a0)return recovered_player_to_server_gui_container_005232a0(s,nullptr,p,minor);if(a==0x00523380)return recovered_player_to_server_character_sheet_00523380(s,nullptr,p,minor);if(a==0x005209b0)return recovered_player_to_server_quick_chat_005209b0(s,nullptr,p,minor);if(a==0x00520a10)return recovered_player_to_server_chat_00520a10(s,nullptr,p,minor);if(a==0x00525390)return recovered_player_to_server_shut_down_server_00525390(s,nullptr,p,minor);if(a==0x005252a0)return recovered_player_to_server_player_death_005252a0(s,nullptr,p,minor);if(a==0x00527890)return recovered_player_to_server_play_module_character_list_00527890(s,nullptr,p,minor);if(a==0x00527380)return recovered_player_to_server_map_pin_00527380(s,nullptr,p,minor);if(a==0x005278d0)return recovered_player_to_server_journal_005278d0(s,nullptr,p,minor);if(a==0x00527560)return recovered_player_to_server_level_up_00527560(s,nullptr,p,minor);if(a==0x005246f0)return recovered_player_to_server_char_list_005246f0(s,nullptr,p,minor);if(a==0x00525350)return recovered_player_to_server_character_download_00525350(s,nullptr,p,minor);kotor_unreachable_trap(a,"Player-message subtype handler not implemented");}
const char*text(String*s)override{return frame_LogCStr_005e5670(s,nullptr);}
void format_problem(String*s,bool over,const char*t,unsigned minor)override{kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(s),over?"Server message OVERFLOW in %s (0x%.2X)\nFIX this or dream up something nasty to do to the person responsible!":"Server message UNDERFLOW in %s (0x%.2X)\nFIX this or dream up something nasty to do to the person responsible!",t,minor);}
};}
extern "C" unsigned __fastcall recovered_player_message_00527b20(void*s,void*,unsigned id,void*p,unsigned n){Native ports;return recovered_player_message::handle_00527b20(s,id,p,n,ports);}
