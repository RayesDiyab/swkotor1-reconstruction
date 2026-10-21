// Reuse retained world-los traversal and vector providers.
using GroundVector=::kotor_scene3d::Vector;
using GroundHit=::kotor_world_los::SurfaceHitInfo;
extern "C" int __fastcall surface_mesh_check_aabb_node_00580920(void*,void*,int,GroundVector*,GroundVector*,GroundHit*);
extern "C" GroundVector* __cdecl normalize_004ab180(GroundVector*,const GroundVector*);
extern "C" GroundVector* __fastcall vector_add_assign_004a9740(GroundVector*,void*,const GroundVector*);
extern "C" GroundVector* __fastcall vector_scale_assign_004a97a0(GroundVector*,void*,float);
struct NativeRoomGroundRay final:RoomGroundRayPorts {
 Word node(Word a,Word b,Word c,Word d,Word e)override{return static_cast<Word>(surface_mesh_check_aabb_node_00580920(ptr(a),nullptr,static_cast<int>(b),static_cast<GroundVector*>(ptr(c)),static_cast<GroundVector*>(ptr(d)),static_cast<GroundHit*>(ptr(e))));}
 void normalize(Word a,Word b)override{(void)normalize_004ab180(static_cast<GroundVector*>(ptr(a)),static_cast<GroundVector*>(ptr(b)));}
 void scale(Word a,Word bits)override{float f;std::memcpy(&f,&bits,4);(void)vector_scale_assign_004a97a0(static_cast<GroundVector*>(ptr(a)),nullptr,f);}
 void add(Word a,Word b)override{(void)vector_add_assign_004a9740(static_cast<GroundVector*>(ptr(a)),nullptr,static_cast<GroundVector*>(ptr(b)));}
 void world_point(Word a,Word b,Word c)override{float*hit=static_cast<float*>(ptr(a));float*room=static_cast<float*>(ptr(b));float*mesh=static_cast<float*>(ptr(c));float x,y,z0,z;
 __asm{mov eax,hit
 mov ecx,room
 mov edx,mesh
 fld dword ptr[eax]
 fadd dword ptr[ecx]
 fld dword ptr[eax+4]
 fadd dword ptr[ecx+4]
 fld dword ptr[eax+8]
 fadd dword ptr[ecx+8]
 fstp z0
 fxch st(1)
 fadd dword ptr[edx]
 fstp x
 fadd dword ptr[edx+4]
 fstp y
 fld z0
 fadd dword ptr[edx+8]
 fstp z}
 std::memcpy(hit,&x,4);std::memcpy(hit+1,&y,4);std::memcpy(hit+2,&z,4);}
};
