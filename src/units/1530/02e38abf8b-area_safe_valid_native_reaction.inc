extern "C" unsigned __cdecl aurora_vector_equal_004aa980(const void*,const void*);
#include "8c328944af-mesh_segment_clip_native.inc"
#include "8835766bc8-room_valid_helpers_native.inc"
#include "345597adf6-area_region_native.inc"
#include "4f605bd3a5-area_creature_collision_native_reaction.inc"
struct NativeAreaSafeValid final:AreaSafeValidPorts{NativeMemory&m;explicit NativeAreaSafeValid(NativeMemory&v):m(v){}
 void bounds(Word input,Word shape,Word s)override{float* p=static_cast<float*>(ptr(input));float* h=static_cast<float*>(ptr(shape+0x14));float* q=static_cast<float*>(ptr(s));float xy=0.01f,dz=0.1f;
 __asm{mov eax,p
 mov ecx,q
 fld dword ptr[eax]
 fsub xy
 fstp dword ptr[ecx+38h]
 fld dword ptr[eax+4]
 fsub xy
 fstp dword ptr[ecx+3ch]
 fld dword ptr[eax+8]
 fsub dz
 fstp dword ptr[ecx+40h]
 fld dword ptr[eax]
 fadd xy
 fstp dword ptr[ecx+2ch]
 fld dword ptr[eax+4]
 fadd xy
 fstp dword ptr[ecx+30h]
 fld dword ptr[eax+8]
 fadd dz
 fstp dword ptr[ecx+34h]
 fld dword ptr[eax+8]
 fsub dz
 fstp dword ptr[ecx+54h]
 mov edx,h
 fld dword ptr[edx]
 fadd dword ptr[ecx+54h]
 fadd dz
 fstp dword ptr[ecx+18h]}
 }
 Word room(Word a,Word b,Word c)override{NativeAreaFindRoom p;alignas(8) unsigned char s[0x48];return area_find_room_004bb600(m,p,a,b,c,word(s));}
 void mesh(Word a,Word x,Word y,Word z,Word u,Word v,Word w,Word outA,Word outB,Word edge)override{Word from[3]={x,y,z},to[3]={u,v,w};alignas(8) unsigned char s[0x74];NativeMeshSegmentClip p;(void)mesh_segment_clip_00583e30(m,p,a,word(from),word(to),outA,outB,edge,word(s));}
 Word sweep(Word a,Word b,Word c,Word d,Word e,Word f,Word g)override{NativeRoomValidHelpers p;return room_sweep_00579680(m,p,a,b,c,d,e,f,g);}
 static bool axis(Word a,Word b){float target,hit,epsilon=0.0001f;std::memcpy(&target,&a,4);std::memcpy(&hit,&b,4);unsigned result;
 __asm{fld target
 fsub epsilon
 fcomp hit
 fnstsw ax
 test ah,41h
 jp safe_axis_fail
 fld epsilon
 fadd target
 fcomp hit
 fnstsw ax
 test ah,1
 jnz safe_axis_fail
 mov result,1
 jmp safe_axis_done
 safe_axis_fail:
 mov result,0
 safe_axis_done:}
 return result!=0;}
 bool near(Word a,Word b,Word c,Word d)override{return axis(a,c)&&axis(b,d);}
 Word adjacent(Word a,Word b)override{return room_adjacent_00579420(m,a,b);}
 bool equal(Word a,Word b)override{return aurora_vector_equal_004aa980(ptr(a),ptr(b))!=0;}
 Word region(Word a,Word b,Word c,Word d,Word e,Word f,Word g,Word h,Word i)override{NativeAreaRegion p;Word scratch;return area_region_005086d0(m,p,a,b,c,d,e,f,g,h,i,word(&scratch));}
 Word collision(Word a,Word b,Word c,Word d,Word e,Word f,Word g,Word h,Word i)override{NativeAreaCreatureCollision p;alignas(8) unsigned char scratch[0xb4];return area_creature_collision_004bc590(m,p,a,b,c,d,e,f,g,h,i,word(scratch));}
};
