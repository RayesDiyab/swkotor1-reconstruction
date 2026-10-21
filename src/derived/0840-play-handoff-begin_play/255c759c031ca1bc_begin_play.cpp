#include "34f7577ed6a2846c_begin_play.hpp"
#include <cstring>
namespace recovered_begin_play {
namespace {
std::uint32_t rd(void* p,std::uint32_t off){std::uint32_t v;std::memcpy(&v,static_cast<unsigned char*>(p)+off,4);return v;}
void* ptr(void* p,std::uint32_t off){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(rd(p,off)));}
void wr(void* p,std::uint32_t off,std::uint32_t v){std::memcpy(static_cast<unsigned char*>(p)+off,&v,4);}
void self_byte(void* stats,std::uint32_t off){auto*p=static_cast<volatile unsigned char*>(stats);unsigned char v=p[off];p[off]=v;}
}
// Ordinary original flow. Required providers retain their exact address and ABI contract.
void class_begin_006dbdf0(void* self,Ports& p){
 p.movie_cleanup_005ee120(ptr(p.manager(),4));
 p.unpauseable_0062b410(p.getter_005ed690(ptr(p.manager(),4)),1);
 String command; p.string_construct_005b3190(&command);
 void* stats=ptr(ptr(self,0x68),0x2f8);
 // Original reloads all three source pointers and performs the three byte self-assignments.
 auto* b=static_cast<volatile unsigned char*>(stats);unsigned char first=b[0x5c];
 void* client=ptr(p.manager(),4); b[0x5c]=first;
 self_byte(ptr(ptr(self,0x68),0x2f8),0x5e);self_byte(ptr(ptr(self,0x68),0x2f8),0x5d);
 stats=ptr(ptr(self,0x68),0x2f8);std::uint16_t zero=0;std::memcpy(static_cast<unsigned char*>(stats)+0x120,&zero,2);
 p.create_server_00401380(p.manager(),0);
 p.save_character_006123e0(ptr(self,0x68));
 String empty; p.string_literal_005e5a90(&empty,""); String network;
 String* net=p.getter_string_005ed490(client,&network);
 p.net_start_005d5040(p.net_layer_slot10(client),0,net,&empty,10,0x10);
 p.string_destroy_005e5c20(&network);p.string_destroy_005e5c20(&empty);
 p.ui_begin_006775e0(p.getter_005ed6f0(client));
 // Original vararg call supplies both words of the class module-string view.
 p.format_005e5680(&command,"%c%s.%s %s",0x73,"Module","Load",rd(self,0x1558),rd(self,0x155c));
 auto length=p.length_005e5790(&command);auto text=p.cstr_005e5670(&command);
 p.net_send_005d4cb0(p.net_layer_slot10(client),0xfffffffd,text,length,0);
 String temp;p.string_literal_005e5a90(&temp,"temp");p.set_temp_005edaa0(client,&temp);p.string_destroy_005e5c20(&temp);
 wr(self,0x44,(rd(self,0x44)&0xfffffcff)|0x400);
 p.set_flags_005eda60(client,0,1);
 auto* module=reinterpret_cast<String*>(static_cast<unsigned char*>(self)+0x1558);
 p.set_module_005edcf0(client,module);String module_copy;
 p.use_module_005edd30(client,p.get_module_005edd00(client,&module_copy,module));p.string_destroy_005e5c20(&module_copy);
 p.connect_flags_005eda80(client,1,1,0);
 p.connect_005edf00(client,p.get_connect_005ee090(client,module,0));
 p.string_destroy_005e5c20(&command);
}
void main_begin_006eb320(void* self,Ports& p){p.remove_subpanels_006ea830(self);class_begin_006dbdf0(ptr(self,0x68),p);}
}
