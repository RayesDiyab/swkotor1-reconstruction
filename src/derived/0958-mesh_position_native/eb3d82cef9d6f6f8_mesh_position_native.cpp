#include "aca02343a18e65e3_mesh_position.hpp"
#include <cstring>
namespace{using namespace recovered_mesh_position;void* ptr(U a){return reinterpret_cast<void*>(a);}struct Native:Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U v)override{std::memcpy(ptr(a),&v,4);}void shift(U self,U vertex,U translation)override{
 const float* a=static_cast<float*>(ptr(vertex));const float* bias=static_cast<float*>(ptr(self+0x2c));const float* move=static_cast<float*>(ptr(translation));float x,y,z,tempz;
 // Original x/y additions remain extended until final store; z is rounded between additions.
 __asm{mov eax,a}__asm{mov edx,bias}__asm{fld dword ptr[eax]}__asm{fadd dword ptr[edx]}__asm{fld dword ptr[eax+4]}__asm{fadd dword ptr[edx+4]}__asm{fld dword ptr[eax+8]}__asm{fadd dword ptr[edx+8]}__asm{fstp tempz}__asm{fxch st(1)}__asm{mov edx,move}__asm{fadd dword ptr[edx]}__asm{fstp x}__asm{fadd dword ptr[edx+4]}__asm{fstp y}__asm{fld tempz}__asm{fadd dword ptr[edx+8]}
 __asm{mov eax,a}__asm{mov ecx,x}__asm{mov dword ptr[eax],ecx}__asm{mov ecx,y}__asm{mov dword ptr[eax+4],ecx}__asm{fstp z}__asm{mov ecx,z}__asm{mov dword ptr[eax+8],ecx}
}};}
extern "C" void __fastcall recovered_mesh_position_00581af0(void*s,void*,void*v){Native p;recovered_mesh_position::position(reinterpret_cast<unsigned>(s),reinterpret_cast<unsigned>(v),p);}
