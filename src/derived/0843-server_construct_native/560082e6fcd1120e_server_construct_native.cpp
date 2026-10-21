#include "a5884536589ccff6_server_construct.hpp"
#include <new>
#include <cstring>
#if !defined(_M_IX86)
#error Native constructor bridge requires x86
#endif
extern "C" void __fastcall GuiAssign005e5140(void*,void*,const char*);
extern "C" { void* recovered_server_internal_00831fdc=nullptr; }
namespace {
void empty_string(void* s){const std::uint32_t z=0;std::memcpy(s,&z,4);std::memcpy(static_cast<unsigned char*>(s)+4,&z,4);}
void assign_string(void* s,const char* text){GuiAssign005e5140(s,nullptr,text);}
void* allocate(std::uint32_t n){return ::operator new(n);}
void* list(void* s){return recovered_server_create::construct_list_outer_004b25d0(s,allocate);}
void embedded1(void* s){recovered_server_create::construct_embedded_0052b170(s,empty_string);}
void embedded2(void* s){recovered_server_create::construct_embedded_00563d20(s);}
void publish(void* s){recovered_server_internal_00831fdc=s;}
const recovered_server_create::Providers providers{empty_string,assign_string,embedded1,embedded2,allocate,list,publish,0x7f000000,0xfffffffe};
}
extern "C" void* __fastcall recovered_server_construct_004b55e0(void* s,void*){
 return recovered_server_create::construct_internal_004b55e0(s,providers);
}
