#include "77e8b6221f-menu_targets.h"
namespace kotor_menu20 {
std::int32_t String::as_int_005e5630(){char* p=data;return p?bindings.atol_006fafe7(p):0;}
void Input::set_mouse_005df5d0(int x,int y){bindings.set_mouse_005e0960(internal,nullptr,x,y);}
void Debug::open_logs_005e93f0(Word argument){void* p=internal;



 if(p)bindings.open_logs_0060e760(p,nullptr,argument);

}
void* ClientApp::get_options_005ed700(){



 return internal->options;

}
// Assignment algorithm lives once on canonical StartupString.
NetLayer* NetLayer::construct_normal_005d54a0(){
 bindings.player_construct_005d5440(reinterpret_cast<unsigned char*>(this)+0x20014,nullptr);
 for(unsigned i=0;i<4;++i)bindings.string_construct_005b3190(&strings40[i],nullptr);
 bindings.string_construct_005b3190(&string64,nullptr);peer=nullptr;initialized=0;return this;
}
Word NetLayer::initialize_005d5550(void* application){
 NetLayer* head=*bindings.network_head_0083288c;
 if(!head)*bindings.network_head_0083288c=this;
 else{peer=head;(*bindings.network_head_0083288c)->peer=this;}
 const bool already=initialized==1;field0=0;field4=0;
 if(already)return 0;
 app=application;initialized=1;field3c=0;field14=0;
 bindings.string_assign_005e5140(&string18,nullptr,bindings.initial_text_0073d71c);
 field30=0;field34=0;field38=0;field60=0;field6c=0;field70=0;field74=0;field78=0;return 1;
}
Ime* Ime::construct_normal_005ee400(){for(unsigned i=0;i<3;++i)bindings.string_construct_005b3190(&strings110[i],nullptr);field108=0;field104=0;return this;}
// Formatter algorithm lives once on canonical StartupString.
}

namespace base_ini_reconstruction {
void PublicIni::destroy_005e67e0(){auto* p=internal;if(p){kotor_menu20::bindings.ini_destroy_005eb500(p,nullptr);kotor_menu20::bindings.free_006fa390(p);}internal=nullptr;}
}
