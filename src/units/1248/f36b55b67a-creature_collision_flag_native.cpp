#include "54ff08aafd-creature_collision_flag.hpp"
#include <cstring>
extern "C" {unsigned recovered_collision_disabled_007a1b34=1;unsigned __fastcall kotor_fastcall_004aea40(void*,void*);void* __fastcall kotor_fastcall_004ae770(void*,void*,unsigned);__declspec(noreturn)void __cdecl kotor_unreachable_trap(unsigned long,const char*);}
namespace kotor_menu_window_cells{extern void* volatile manager_007a39fc;}
// stale-stubs G3: 0057CB80 CSWSObject::GetReputation RET 8 (W1 b7).
extern "C"{unsigned __fastcall recovered_object_get_reputation_0057cb80(void*,void*,unsigned,unsigned);}
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}U rd(U a){U v;std::memcpy(&v,ptr(a),4);return v;}
struct Native:recovered_creature_collision_flag::Ports{U read(U a)override{return rd(a);}U disabled()override{return *static_cast<volatile unsigned*>(&recovered_collision_disabled_007a1b34);}int relation(U a,U id,U f)override{return static_cast<int>(recovered_object_get_reputation_0057cb80(ptr(a),nullptr,id,f));}U server()override{return rd(word(kotor_menu_window_cells::manager_007a39fc)+8);}U selected_id(U a)override{return kotor_fastcall_004aea40(ptr(a),nullptr);}U creature(U a,U id)override{return word(kotor_fastcall_004ae770(ptr(a),nullptr,id));}};}
extern "C" unsigned __fastcall recovered_creature_collision_flag_004f62a0(void*self,void*,void*other){Native p;return recovered_creature_collision_flag::query(word(self),word(other),p);}
