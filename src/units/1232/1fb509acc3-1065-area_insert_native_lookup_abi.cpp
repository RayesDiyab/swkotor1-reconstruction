#include "c6ab068cb4-area_insert.hpp"
#include "fcd45b77be-area_insert_index.hpp"
#include "98b3e4d664-effect_insert.hpp"
#include "1e4deaabc8-area_attachment.hpp"
#include "66afd867ee-c2_area_adapter.hpp"
#include <cstring>
#include <new>
namespace kotor_reconstruction{struct ArrayMutations;}
namespace menu_list_growth{void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*,void*,unsigned);}
namespace kotor_menu_window_cells{extern void* volatile manager_007a39fc;}
extern "C"{unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,unsigned,void*);void* __fastcall kotor_fastcall_004ae910(void*,void*,unsigned);void* __fastcall recovered_event_construct_004d7540(void*,void*);void __fastcall recovered_event_integer_004d76b0(void*,void*,unsigned,unsigned);void __fastcall recovered_event_enqueue_004b08d0(void*,void*,unsigned,unsigned,unsigned,unsigned,unsigned,void*);__declspec(noreturn)void __cdecl kotor_unreachable_trap(unsigned long,const char*);}
// stale-stubs G3: 0050DC40 CSWSArea::RemoveObjectFromArea RET 4 (claude_combat_spell_mine).
extern "C"{unsigned __fastcall kotor_fastcall_0050dc40(void*,void*,unsigned);}
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}U rd(U a){U v;std::memcpy(&v,ptr(a),4);return v;}void wr(U a,U v){std::memcpy(ptr(a),&v,4);}U server(){return rd(word(kotor_menu_window_cells::manager_007a39fc)+8);}void grow(U a,U n){menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(a)),nullptr,n);}
struct Memory:kotor::server_mainloop::Memory{U read(U a)override{return rd(a);}void write(U a,U b)override{wr(a,b);}U read16(U a)override{unsigned short v;std::memcpy(&v,ptr(a),2);return v;}void write16(U a,U b)override{unsigned short v=static_cast<unsigned short>(b);std::memcpy(ptr(a),&v,2);}U read8(U a)override{return *static_cast<unsigned char*>(ptr(a));}};
struct Insert:kotor::server_mainloop::EffectInsertPorts{void grow(U a,U b)override{::grow(a,b);}};
struct Native:recovered_area_insert::Ports{U read(U a)override{return rd(a);}void write(U a,U b)override{wr(a,b);}void half(U a,U b)override{unsigned short v=static_cast<unsigned short>(b);std::memcpy(ptr(a),&v,2);}U objects()override{return rd(rd(server()+4)+0x1005c);}U lookup(U a,U b,U c)override{return kotor_fastcall_004d8230(ptr(a),nullptr,b,static_cast<void**>(ptr(c)));}U success()override{return 0;}void remove(U a,U id)override{(void)kotor_fastcall_0050dc40(ptr(a),nullptr,id);}U index(U,U,U)override;void insert(U a,U b,U c)override{Memory m;Insert p;kotor::server_mainloop::effect_insert_00616940(m,p,a,b,c);}void grow(U a,U b)override{::grow(a,b);}U creature(U a)override{using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(rd(rd(a)+0x30))(ptr(a)));}U player(U a)override{return word(kotor_fastcall_004ae910(ptr(server()),nullptr,a));}void increment(U area)override{w2::native::AreaMemory memory;c2::C2AreaPorts ports(memory);kotor::area_attachment::increment_players_00508c20(memory,ports,area);}U allocate(U a)override{return word(::operator new(a));}U event(U a)override{return word(recovered_event_construct_004d7540(ptr(a),nullptr));}U mode()override{return rd(rd(server()+4)+0x100c4);}void event_int(U a,U b,U c)override{recovered_event_integer_004d76b0(ptr(a),nullptr,b,c);}U kind()override{return 10;}U ai()override{return rd(rd(server()+4)+0x10044);}void enqueue(U a,U b,U c,U d,U e,U f,U g)override{(void)recovered_event_enqueue_004b08d0(ptr(a),nullptr,b,c,d,e,f,ptr(g));}};
struct Index:recovered_area_insert_index::Ports{Native&n;explicit Index(Native&v):n(v){}U read(U a)override{return rd(a);}void write(U a,U b)override{wr(a,b);}U objects()override{return n.objects();}U lookup(U a,U b,U c)override{return n.lookup(a,b,c);}U success()override{return n.success();}bool below_or_unordered(U a,U right)override{U left=rd(a);unsigned short status;__asm { fld dword ptr [left] } __asm { fcomp dword ptr [right] } __asm { fnstsw ax } __asm { mov status,ax } return (status&0x100)!=0;}};
U Native::index(U self,U output,U x){Index p(*this);unsigned scratch;return recovered_area_insert_index::find(self,output,x,word(&scratch),p);}
}
extern "C" unsigned __fastcall recovered_area_insert_0050dfd0(void*self,void*,unsigned id,unsigned fresh){Native p;unsigned scratch[3];return recovered_area_insert::add(word(self),id,fresh,word(scratch),p);}

extern "C" unsigned __fastcall recovered_area_index_00506f20(void*self,void*,unsigned*output,unsigned xBits){Native p;return p.index(word(self),word(output),xBits);}
