// W6 t7 native adapter: 0053A900 ExecuteCommandGetLastHostileTarget (721), int __fastcall(implementer, edx, routine, argc) == thiscall RET 8.
#include "19a60b5b52-vm_commands_t7.hpp"
#include <cstring>
#include <cstdint>
extern "C" {
unsigned __fastcall recovered_vm_pop_object_005d26a0(void*,void*,void*);
unsigned __fastcall recovered_vm_push_object_005d1a10(void*,void*,unsigned);
void* __fastcall kotor_fastcall_004ae770(void*,void*,unsigned);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace {
using kotor::vm_commands_t7::Word;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const volatile void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
struct NativeT7 final:kotor::vm_commands_t7::Ports{
 NativeMemory m;
 Word vm(){return m.read(frame_control::message_manager_007a3a00);}
 Word server_app()override{return m.read(word(kotor_menu_window_cells::manager_007a39fc)+8);}
 Word pop_object(Word* o)override{return recovered_vm_pop_object_005d26a0(ptr(vm()),nullptr,o);}
 Word push_object(Word v)override{return recovered_vm_push_object_005d1a10(ptr(vm()),nullptr,v);}
 Word get_creature_by_id_004ae770(Word s,Word id)override{return word(kotor_fastcall_004ae770(ptr(s),nullptr,id));}
};
}
extern "C" int __fastcall recovered_vm_get_last_hostile_target_0053a900(void*,void*,int,unsigned){NativeT7 p;return kotor::vm_commands_t7::get_last_hostile_target_0053a900(p.m,p);}
