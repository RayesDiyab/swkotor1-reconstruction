#include "9cde3bcc29-collision_transport.inc"
extern "C" unsigned __fastcall recovered_creature_collision_flag_004f62a0(void*,void*,void*);
extern "C" unsigned __fastcall recovered_creature_incapacitated_004ef890(void*,void*);
extern "C" double __cdecl vector_magnitude_004ab100(const kotor_scene3d::Vector*);
#include "46c63b5bd4-area_collision_route_numeric.inc"
struct NativeAreaCollisionScan final:AreaCollisionScanPorts{NativeMemory&m;explicit NativeAreaCollisionScan(NativeMemory&v):m(v){}
 Word creature(Word a,Word b)override{return word(kotor_fastcall_004ae770(ptr(a),nullptr,b));}
 Word state(Word a)override{using Fn=unsigned(__fastcall*)(void*,void*);return reinterpret_cast<Fn>(m.read(m.read(a)+0x94))(ptr(a),nullptr);}
 Word incapacitated(Word a)override{return recovered_creature_incapacitated_004ef890(ptr(a),nullptr);}
 Word reaction(Word a,Word b)override{return recovered_creature_collision_flag_004f62a0(ptr(a),nullptr,ptr(b));}
 Word geometry_route(Word object,Word scratch)override{Word shape=m.read(object+0x340);float* radius=static_cast<float*>(ptr(shape+8));float*s=static_cast<float*>(ptr(scratch));
 __asm{mov eax,radius
 mov ecx,s
 fld dword ptr[eax]
 fadd dword ptr[ecx+5ch]
 fstp dword ptr[ecx+10h]}
 Word x=m.read(object+0x90),y=m.read(object+0x94),z=m.read(object+0x98);m.write(scratch+0x60,x);m.write(scratch+0x68,z);m.write(scratch+0x64,y);m.write(scratch+0x68,0);
 __asm{mov ecx,s
 fld dword ptr[ecx+6ch]
 fsub dword ptr[ecx+78h]
 fstp dword ptr[ecx+84h]
 mov dword ptr[ecx+8ch],0
 fld dword ptr[ecx+70h]
 fsub dword ptr[ecx+7ch]
 fstp dword ptr[ecx+88h]}
 (void)vector_magnitude_004ab100(static_cast<const kotor_scene3d::Vector*>(ptr(scratch+0x84)));
 return collision_numeric_route(scratch);}
 Word contact(Word a,Word b,Word c,Word d,Word e,Word f,Word g,Word h,Word i)override{return collision_transport::contact(a,b,c,d,e,f,g,h,i);}
 void notify(Word a,Word b,Word c,Word d,Word e,Word f)override{collision_transport::notify(a,b,c,d,e,f);}
};
