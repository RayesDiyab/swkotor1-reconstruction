// W6 t6 native adapter: 00546AF0 ExecuteCommandGetSoloMode (462), int __fastcall(implementer, edx, routine, argc) == thiscall RET 8.
#include "5f1ba2e9f5-vm_commands_t6.hpp"
#include <cstring>
#include <cstdint>
extern "C" unsigned __fastcall recovered_vm_push_integer_005d1850(void*,void*,unsigned);
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace frame_control { extern unsigned message_manager_007a3a00; }
namespace {
using kotor::vm_commands_t6::Word;
void* ptr(Word a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
Word word(const volatile void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
struct NativeMemory final:kotor::server_mainloop::Memory{
 Word read(Word a)override{Word v;std::memcpy(&v,ptr(a),4);return v;}
 void write(Word a,Word v)override{std::memcpy(ptr(a),&v,4);}
 Word read16(Word a)override{std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
 void write16(Word a,Word v)override{const std::uint16_t x=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&x,2);}
 Word read8(Word a)override{return *static_cast<const unsigned char*>(ptr(a));}
};
struct NativeT6 final:kotor::vm_commands_t6::Ports{
 NativeMemory m;
 Word server_app()override{return m.read(word(kotor_menu_window_cells::manager_007a39fc)+8);}
 Word push_integer(Word v)override{return recovered_vm_push_integer_005d1850(ptr(m.read(frame_control::message_manager_007a3a00)),nullptr,v);}
};
}
extern "C" int __fastcall recovered_vm_get_solo_mode_00546af0(void*,void*,int,unsigned){NativeT6 p;return kotor::vm_commands_t6::get_solo_mode_00546af0(p.m,p);}
