#include "b541422f1757d20e_trigger_creator.hpp"
#include <cstring>
namespace kotor_menu_window_cells{extern void* volatile manager_007a39fc;}
extern "C" unsigned __fastcall kotor_fastcall_004aea40(void*,void*);
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}struct Native:recovered_trigger_creator::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}U server()override{return read(word(kotor_menu_window_cells::manager_007a39fc)+8);}U player_id(U a)override{return kotor_fastcall_004aea40(ptr(a),nullptr);}U party(U a)override{return read(a+4)+0x1b770;}};}
extern "C" void __fastcall recovered_trigger_creator_0058d700(void*self,void*,unsigned id){Native p;recovered_trigger_creator::set(word(self),id,p);}
extern "C" unsigned __fastcall recovered_party_member_00563740(void*self,void*,unsigned id){Native p;return recovered_trigger_creator::member(word(self),id,p);}
