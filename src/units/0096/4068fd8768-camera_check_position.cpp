// Original006D18E0: four camera collision rays, exact x87 stores and HitInfo lifetime.
#include "288009485d-camera_check_position.h"
#include <cstring>
#include <cstdint>
#if !defined(_M_IX86)
#error CheckCameraPosition requires Windows x86
#endif
using kotor_scene3d::Vector;
using kotor_scene3d::Quaternion;
using U=std::uint32_t;
using I=std::int32_t;
namespace w4cam { extern float probe_radius_007a241c; }
namespace kotor_world_los { extern Vector up_007a19b0; }
extern "C" {
void* __fastcall aurora_hit_info_construct_004578f0(void*,void*,U,U,U);
void __fastcall aurora_hit_info_clear_output_00456a60(void*,void*);
void __fastcall chit_info_destruct_00456b30(void*,void*);
Vector* __cdecl normalize_004ab180(Vector*,const Vector*);
Vector* __fastcall vector_subtract_assign_004a9770(Vector*,void*,const Vector*);
Vector* __fastcall vector_add_assign_004a9740(Vector*,void*,const Vector*);
}
namespace {
const float k_zero_0073d700=0.0f;
const float k_one_0073d764=1.0f;
const float k_extra_0074a604=0.15000000596046448f;
static_assert(sizeof(Vector)==12 && sizeof(Quaternion)==16,"canonical native layouts");
template<class T> T load(const void* p,U off) { T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+off,sizeof v);return v; }
template<class F> F slot(void* p,U off) { return load<F>(load<void*>(p,0),off); }
U pointer_word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
void* pointer(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
struct HitLocal {
    alignas(4) unsigned char bytes[0x64];
    HitLocal() { aurora_hit_info_construct_004578f0(bytes,nullptr,0,0xbfffffffu,0); }
    ~HitLocal() noexcept(false) { chit_info_destruct_00456b30(bytes,nullptr); }
};
void initial_delta(Vector* out,U* radius_bits,const Vector* initial,const Vector* desired) {
    const float* cell=&w4cam::probe_radius_007a241c;
    __asm {
        mov eax,initial
        mov ecx,desired
        mov edx,out
        fld dword ptr [eax+8]
        mov esi,cell
        mov esi,dword ptr [esi]
        mov edi,radius_bits
        mov dword ptr [edi],esi
        fsub dword ptr [ecx+8]
        fld dword ptr [eax+4]
        fsub dword ptr [ecx+4]
        fld dword ptr [eax]
        fsub dword ptr [ecx]
        fstp dword ptr [edx]
        fstp dword ptr [edx+4]
        fstp dword ptr [edx+8]
    }
}
void difference(Vector* out,const Vector* a,const Vector* b) {
    __asm {
        mov eax,a
        mov ecx,b
        mov edx,out
        fld dword ptr [eax+8]
        fsub dword ptr [ecx+8]
        fld dword ptr [eax+4]
        fsub dword ptr [ecx+4]
        fld dword ptr [eax]
        fsub dword ptr [ecx]
        fstp dword ptr [edx]
        fstp dword ptr [edx+4]
        fstp dword ptr [edx+8]
    }
}
// 006D19AF..19DA: three separate DWORD stores, in z/y/x order.
void side_offset(Vector* out,U radius_bits,const Vector* normal) {
    __asm {
        mov eax,normal
        mov edx,out
        fld dword ptr radius_bits
        fmul dword ptr [eax+8]
        fstp dword ptr [edx+8]
        fld dword ptr radius_bits
        fmul dword ptr [eax+4]
        fstp dword ptr [edx+4]
        fld dword ptr radius_bits
        fmul dword ptr [eax]
        fstp dword ptr [edx]
    }
}
void endpoint_plus(Vector* out,const Vector* desired,const Vector* displacement) {
    __asm {
        mov eax,desired
        mov ecx,displacement
        mov edx,out
        fld dword ptr [eax+8]
        fadd dword ptr [ecx+8]
        fst dword ptr [edx+8]
        fld dword ptr [eax+4]
        fadd dword ptr [ecx+4]
        fst dword ptr [edx+4]
        fld dword ptr [ecx]
        fadd dword ptr [eax]
        fstp dword ptr [edx]
        fstp dword ptr [edx+4]
        fstp dword ptr [edx+8]
    }
}
void endpoint_minus(Vector* out,const Vector* desired,const Vector* displacement) {
    __asm {
        mov eax,desired
        mov ecx,displacement
        mov edx,out
        fld dword ptr [eax+8]
        fsub dword ptr [ecx+8]
        fst dword ptr [edx+8]
        fld dword ptr [eax+4]
        fsub dword ptr [ecx+4]
        fst dword ptr [edx+4]
        fld dword ptr [eax]
        fsub dword ptr [ecx]
        fstp dword ptr [edx]
        fstp dword ptr [edx+4]
        fstp dword ptr [edx+8]
    }
}
// Original rereads radius twice, once by FLD and once by integer MOV.
void vertical_offset(Vector* out) {
    const float* cell=&w4cam::probe_radius_007a241c;
    __asm {
        mov eax,cell
        mov edx,out
        fld dword ptr [eax]
        mov ecx,dword ptr [eax]
        fmul dword ptr k_zero_0073d700
        mov dword ptr [edx+8],ecx
        fst dword ptr [edx+4]
        fstp dword ptr [edx]
    }
}
// Consume actual provider ST0 directly. TEST AH,5/JNP skips only ordered less;
// equality and unordered both cause the original second magnitude call.
bool not_less(const Vector* v,const float* bound) {
    unsigned char result;
    __asm {
        push bound
        push v
        call vector_magnitude_004ab100
        mov ecx,dword ptr [esp+4]
        fcomp dword ptr [ecx]
        add esp,8
        fnstsw ax
        test ah,5
        setp result
    }
    return result!=0;
}
void magnitude_word(float* out,const Vector* v) {
    __asm {
        push out
        push v
        call vector_magnitude_004ab100
        mov eax,dword ptr [esp+4]
        fstp dword ptr [eax]
        add esp,8
    }
}
void retained_scale(Vector* out,const float* scalar,const Vector* normal) {
    __asm {
        mov eax,normal
        mov ecx,scalar
        mov edx,out
        fld dword ptr [ecx]
        fmul dword ptr [eax+8]
        fld dword ptr [ecx]
        fmul dword ptr [eax+4]
        fld dword ptr [ecx]
        fmul dword ptr [eax]
        fstp dword ptr [edx]
        fstp dword ptr [edx+4]
        fstp dword ptr [edx+8]
    }
}
void orientation_z(Vector* out,const Quaternion* q) {
    __asm {
        mov eax,q
        mov edx,out
        fld dword ptr [eax+8]
        fld dword ptr [eax+4]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fadd st(0),st(0)
        fsubr dword ptr k_one_0073d764
        fstp st(2)
        fstp st(0)
        fld dword ptr [eax+12]
        fmul dword ptr [eax+8]
        fld dword ptr [eax+4]
        fmul dword ptr [eax]
        fsubp st(1),st(0)
        fadd st(0),st(0)
        fld dword ptr [eax+12]
        fmul dword ptr [eax+4]
        fld dword ptr [eax+8]
        fmul dword ptr [eax]
        faddp st(1),st(0)
        fadd st(0),st(0)
        fstp dword ptr [edx]
        fstp dword ptr [edx+4]
        fstp dword ptr [edx+8]
    }
}
void lift_scale(float* out,const float* best,const float* path_length) {
    const float* cell=&w4cam::probe_radius_007a241c;
    __asm {
        mov eax,best
        mov ecx,path_length
        fld dword ptr [eax]
        fdiv dword ptr [ecx]
        mov ecx,cell
        fld dword ptr [ecx]
        fadd dword ptr k_extra_0074a604
        fmulp st(1),st(0)
        mov edx,out
        fstp dword ptr [edx]
    }
}
bool probe(void* self,HitLocal& hit,const Vector* initial,const Vector& end,Vector* point) {
    aurora_hit_info_clear_output_00456a60(hit.bytes,nullptr);
    hit.bytes[0x5e]=1;
    void* owner=load<void*>(self,0x10);
    // The owner is the Camera's Gob facet; its inherited104 getter returns a raw word.
    using GetScene=decltype(aurora_model::GobVTable::s104_gob_get_property_0x68);
    void* scene=pointer(slot<GetScene>(owner,0x104)(static_cast<aurora_model::Gob*>(owner),nullptr));
    using Ray=decltype(kotor_scene3d::SceneVTable::s038_scene_hit_check);
    return slot<Ray>(scene,0x38)(scene,nullptr,*initial,end,point,hit.bytes)!=0;
}
void update_best(float* best,const Vector& point,const Vector& end,bool first) {
    Vector delta;
    difference(&delta,&point,&end);
    if(not_less(&delta,first?&k_zero_0073d700:best)) {
        // A fresh second subtraction and magnitude call are intentional.
        difference(&delta,&point,&end);
        magnitude_word(best,&delta);
    }
}
}
extern "C" Vector* __fastcall camera_check_position_006d18e0(
    void* self,void*,Vector* out,void* character,const Vector* desired,const Vector* initial) {
    Vector point{0.0f,0.0f,0.0f};
    HitLocal hit;
    Vector result;
    std::memcpy(&result,desired,sizeof result);
    hit.bytes[0x5c]=1;
    aurora_hit_info_clear_output_00456a60(hit.bytes,nullptr);
    Vector delta,cross,normal,offset,end;
    U radius_bits;
    initial_delta(&delta,&radius_bits,initial,&result);
    const Vector* c=vector_cross_004ab210(&cross,&kotor_world_los::up_007a19b0,&delta);
    const Vector* n=normalize_004ab180(&normal,c);
    side_offset(&offset,radius_bits,n);
    float best=0.0f;
    bool any=false;
    endpoint_plus(&end,&result,&offset);
    if(probe(self,hit,initial,end,&point)) { any=true;update_best(&best,point,end,true); }
    endpoint_minus(&end,&result,&offset);
    if(probe(self,hit,initial,end,&point)) { any=true;update_best(&best,point,end,false); }
    vertical_offset(&offset);
    endpoint_plus(&end,&result,&offset);
    if(probe(self,hit,initial,end,&point)) { any=true;update_best(&best,point,end,false); }
    endpoint_minus(&end,&result,&offset);
    if(probe(self,hit,initial,end,&point)) { any=true;update_best(&best,point,end,false); }
    if(any) {
        float length;
        difference(&delta,desired,initial);
        magnitude_word(&length,&delta);
        n=normalize_004ab180(&normal,&delta);
        Vector back;
        retained_scale(&back,&best,n);
        vector_subtract_assign_004a9770(&result,nullptr,&back);
        using GetModel=void*(__fastcall*)(void*,void*,U,U);
        void* gob=slot<GetModel>(character,0x98)(character,nullptr,0xff,1);
        Quaternion rotation;
        using Orientation=decltype(aurora_model::GobVTable::s068_gob_get_orientation);
        const auto* q=static_cast<const Quaternion*>(pointer(slot<Orientation>(gob,0x68)(static_cast<aurora_model::Gob*>(gob),nullptr,pointer_word(&rotation))));
        Vector axis,lift;
        orientation_z(&axis,q);
        float factor;
        lift_scale(&factor,&best,&length);
        n=normalize_004ab180(&normal,&axis);
        retained_scale(&lift,&factor,n);
        vector_add_assign_004a9740(&result,nullptr,&lift);
    }
    std::memcpy(out,&result,sizeof result);
    return out; // Original HitInfo cleanup runs after output stores and before pointer return.
}
