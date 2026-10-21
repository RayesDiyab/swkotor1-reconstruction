extern "C" unsigned __fastcall recovered_client_party_0064f1a0(void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_client_status_0064f280(void*,void*,unsigned);
// W2 routes + W3 bodies (lane-w2 MESSAGE-CONTRACT.md; lane-w3 src). 00668640/0064BF30 remain explicit traps (w2_client_handler_pending_stubs, trimmed).
// Original CSWCMessage handlers are thiscall(subtype) RET 4 -> __fastcall(self,EDX,subtype).
extern "C" unsigned __fastcall recovered_client_handle_login_0064ec00(void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_client_handle_module_006684f0(void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_client_handle_area_0064bab0(void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_client_handle_game_object_update_0066a4a0(void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_client_player_list_00668640(void*,void*,unsigned);
extern "C" unsigned __fastcall recovered_client_custom_token_0064bf30(void*,void*,unsigned);
extern "C" unsigned __fastcall kotor_fastcall_00656d20(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00654cc0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_0064dee0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_0066a370(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_006527e0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_006637c0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_006514c0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00656ba0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00665510(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_0064b8e0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00654230(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00665590(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00654900(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00654970(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_006631e0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00651ba0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00651810(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00654aa0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_0064dea0(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00651560(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00663220(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00663810(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_00654700(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_0066a410(void*,void*,unsigned); // claude-build s2p route
extern "C" unsigned __fastcall kotor_fastcall_006631c0(void*,void*,unsigned); // claude-build s2p route
struct NativeClientDispatch:ClientDispatchPorts{NativeMemory&m;NativeServerInfo text;explicit NativeClientDispatch(NativeMemory&v):m(v),text(v){}
void empty(Word a)override{text.empty(a);}void destroy(Word a)override{text.destroy(a);}
void initialize(Word a,Word b,Word c,Word d,Word e)override{recovered_player_message::initialize_read_004d66b0(ptr(a),ptr(b),c,d,e);}
Word incomplete(Word a,Word)override{return recovered_player_message::overrun_004d6230(ptr(a));}Word remaining(Word a,Word)override{return recovered_player_message::underrun_004d6240(ptr(a));}
void format_type(Word a,Word fmt,Word value)override{if(fmt!=0x748260)required(fmt);kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),"unknown Major (0x%.2X)",value);}
const char*label_text(Word a){switch(a){
#include "4ea6255ad1-client_dispatch_labels.inc"
default:required(a);}}
void label(Word a,Word b)override{c2da_assign_text_005e5140(ptr(a),nullptr,label_text(b));}
Word handler(Word target,Word self,Word sub)override{if(target==0x0064edd0){NativeLoadbar p(m);unsigned char scratch[0x20];return client_loadbar_0064edd0(m,p,self,sub,word(scratch));}if(target==0x0064f280)return recovered_client_status_0064f280(ptr(self),nullptr,sub);
switch(target){case 0x006631c0:return kotor_fastcall_006631c0(ptr(self),nullptr,sub);case 0x0066a410:return kotor_fastcall_0066a410(ptr(self),nullptr,sub);case 0x00654700:return kotor_fastcall_00654700(ptr(self),nullptr,sub);case 0x00663810:return kotor_fastcall_00663810(ptr(self),nullptr,sub);case 0x00663220:return kotor_fastcall_00663220(ptr(self),nullptr,sub);case 0x00651560:return kotor_fastcall_00651560(ptr(self),nullptr,sub);case 0x0064dea0:return kotor_fastcall_0064dea0(ptr(self),nullptr,sub);case 0x00654aa0:return kotor_fastcall_00654aa0(ptr(self),nullptr,sub);case 0x00651810:return kotor_fastcall_00651810(ptr(self),nullptr,sub);case 0x00651ba0:return kotor_fastcall_00651ba0(ptr(self),nullptr,sub);case 0x006631e0:return kotor_fastcall_006631e0(ptr(self),nullptr,sub);case 0x00654970:return kotor_fastcall_00654970(ptr(self),nullptr,sub);case 0x00654900:return kotor_fastcall_00654900(ptr(self),nullptr,sub);case 0x00665590:return kotor_fastcall_00665590(ptr(self),nullptr,sub);case 0x00654230:return kotor_fastcall_00654230(ptr(self),nullptr,sub);case 0x0064b8e0:return kotor_fastcall_0064b8e0(ptr(self),nullptr,sub);case 0x00665510:return kotor_fastcall_00665510(ptr(self),nullptr,sub);case 0x00656ba0:return kotor_fastcall_00656ba0(ptr(self),nullptr,sub);case 0x006514c0:return kotor_fastcall_006514c0(ptr(self),nullptr,sub);case 0x006637c0:return kotor_fastcall_006637c0(ptr(self),nullptr,sub);case 0x006527e0:return kotor_fastcall_006527e0(ptr(self),nullptr,sub);case 0x0066a370:return kotor_fastcall_0066a370(ptr(self),nullptr,sub);case 0x0064dee0:return kotor_fastcall_0064dee0(ptr(self),nullptr,sub);case 0x00654cc0:return kotor_fastcall_00654cc0(ptr(self),nullptr,sub);case 0x00656d20:return kotor_fastcall_00656d20(ptr(self),nullptr,sub);case 0x0064f1a0:return recovered_client_party_0064f1a0(ptr(self),nullptr,sub);case 0x0064ec00:return recovered_client_handle_login_0064ec00(ptr(self),nullptr,sub);case 0x006684f0:return recovered_client_handle_module_006684f0(ptr(self),nullptr,sub);case 0x0064bab0:return recovered_client_handle_area_0064bab0(ptr(self),nullptr,sub);case 0x0066a4a0:return recovered_client_handle_game_object_update_0066a4a0(ptr(self),nullptr,sub);case 0x00668640:return recovered_client_player_list_00668640(ptr(self),nullptr,sub);case 0x0064bf30:return recovered_client_custom_token_0064bf30(ptr(self),nullptr,sub);default:break;}
required(target);}
Word cstr(Word a)override{return text.cstr(a);}
void format_error(Word a,Word fmt,Word value,Word sub)override{const char*format=nullptr;if(fmt==0x752190)format="Client message OVERFLOW in %s (0x%.2X)\nFIX this or FLOG the person responsible!";else if(fmt==0x752138)format="Client message UNDERFLOW in %s (0x%.2X)\nFIX this or FLOG the person responsible!";else required(fmt);kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),format,static_cast<const char*>(ptr(value)),sub);}
};
