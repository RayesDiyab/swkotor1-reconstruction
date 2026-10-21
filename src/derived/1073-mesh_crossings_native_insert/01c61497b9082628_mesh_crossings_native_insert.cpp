#include "b066d4a60e85dd9f_mesh_crossings.hpp"
#include "9ca911027ddfdc58_crossing_insert.hpp"
#include <new>
#include <cstring>
namespace kotor_scene3d{struct Vector;}
namespace kotor_reconstruction{struct ArrayMutations;}
extern "C" void __fastcall vector_list_allocate_00421310(kotor_reconstruction::ArrayMutations*,void*,int);
extern "C"{kotor_scene3d::Vector* __cdecl normalize_004ab180(kotor_scene3d::Vector*,const kotor_scene3d::Vector*);__declspec(noreturn)void __cdecl kotor_unreachable_trap(unsigned long,const char*);}
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(void*p){return reinterpret_cast<U>(p);}U rd(U a){U v;std::memcpy(&v,ptr(a),4);return v;}void wr(U a,U v){std::memcpy(ptr(a),&v,4);}
recovered_mesh_crossings::Projection dot(U point,U vector,U order,U cmp,bool compare){static const U orders[6][3]={{0,2,1},{0,1,2},{1,0,2},{2,0,1},{2,1,0},{1,2,0}};const U*o=orders[order];U p0=rd(point+o[0]*4),p1=rd(point+o[1]*4),p2=rd(point+o[2]*4),d0=rd(vector+o[0]*4),d1=rd(vector+o[1]*4),d2=rd(vector+o[2]*4),bits;unsigned short status=0;
 __asm{fld dword ptr[p0]
 fmul dword ptr[d0]
 fld dword ptr[p1]
 fmul dword ptr[d1]
 faddp st(1),st(0)
 fld dword ptr[p2]
 fmul dword ptr[d2]
 faddp st(1),st(0)}
 if(compare){__asm{fst dword ptr[bits]
 fcomp dword ptr[cmp]
 fnstsw ax
 mov status,ax}}
 else{__asm{fstp dword ptr[bits]}}
 return{bits,status};}
struct Native:recovered_mesh_crossings::Ports{U read(U a)override{return rd(a);}void write(U a,U b)override{wr(a,b);}void prepare(U from,U to,U s)override{U*first=static_cast<U*>(ptr(from));U*second=static_cast<U*>(ptr(to));U delta[3];delta[2]=0;
 __asm{mov ecx,first
 mov edx,second
 lea eax,delta
 fld dword ptr[edx]
 fsub dword ptr[ecx]
 fstp dword ptr[eax]
 fld dword ptr[edx+4]
 fsub dword ptr[ecx+4]
 fstp dword ptr[eax+4]}
 (void)normalize_004ab180(static_cast<kotor_scene3d::Vector*>(ptr(s)),reinterpret_cast<const kotor_scene3d::Vector*>(delta));U y=rd(s+4),neg;
 __asm{fld dword ptr[y]
 fchs
 fstp dword ptr[neg]}
 wr(s+12,neg);wr(s+16,rd(s));wr(s+20,0);wr(s+24,project(from,s,3));wr(s+28,project(from,s+12,1));}
 U project(U a,U b,U order)override{return dot(a,b,order,0,false).bits;}U compare(U a,U b)override{unsigned short flags;__asm{fld dword ptr[a]
 fcomp dword ptr[b]
 fnstsw ax
 mov flags,ax}return flags;}
 recovered_mesh_crossings::Projection previous(U a,U b,U current)override{return dot(a,b,2,current,true);}
 void interpolate(U current,U next,U line,U currentY,U nextY,U output)override{U*cur=static_cast<U*>(ptr(current));U*nxt=static_cast<U*>(ptr(next));U nx,ny,cz,x,y,z,one=0x3f800000;
 __asm{mov ecx,nxt
 mov edx,cur
 fld dword ptr[line]
 fsub dword ptr[nextY]
 fld dword ptr[currentY]
 fsub dword ptr[nextY]
 fdivp st(1),st(0)
 fld dword ptr[one]
 fsub st(0),st(1)
 fld st(0)
 fmul dword ptr[ecx]
 fstp dword ptr[nx]
 fld st(0)
 fmul dword ptr[ecx+4]
 fstp dword ptr[ny]
 fmul dword ptr[ecx+8]
 fld st(1)
 fmul dword ptr[edx]
 fld st(2)
 fmul dword ptr[edx+4]
 fxch st(3)
 fmul dword ptr[edx+8]
 fstp dword ptr[cz]
 fadd dword ptr[nx]
 fstp dword ptr[x]
 fxch st(1)
 fadd dword ptr[ny]
 fstp dword ptr[y]
 fld dword ptr[cz]
 fadd st(0),st(1)
 fstp dword ptr[z]
 fstp st(0)}
 wr(output,x);wr(output+4,y);wr(output+8,z);}
 bool forward(U a,U b,U start)override{return (dot(a,b,4,start,true).status&0x100)==0;}
 void insert(U,U,U,U,U,U)override;
};
struct CrossingInsert:recovered_crossing_insert::Ports{U read(U a)override{return rd(a);}void write(U a,U b)override{wr(a,b);}U projection(U a,U b)override{return dot(a,b,4,0,false).bits;}bool greater(U a,U b,U projection,U order)override{return (dot(a,b,order,projection,true).status&0x4100)==0;}void vector_insert(U a,U x,U y,U z,U index)override{recovered_crossing_insert::vector_insert(a,x,y,z,index,*this);}U allocate(U n)override{return word(::operator new(n));}void release(U a)override{::operator delete(ptr(a));}void vector_grow(U a,U n)override{vector_list_allocate_00421310(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(a)),nullptr,static_cast<int>(n));}};
void Native::insert(U self,U from,U direction,U point,U count,U edge){CrossingInsert p;recovered_crossing_insert::insert(self,from,direction,point,count,edge,p);}
}
extern "C" unsigned __fastcall recovered_mesh_crossings_00582630(void*self,void*,void*from,void*to,unsigned*edge){Native p;unsigned scratch[12];return recovered_mesh_crossings::count(word(self),word(from),word(to),word(edge),word(scratch),p);}
