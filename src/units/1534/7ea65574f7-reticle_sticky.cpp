#include "8c2a158bea-reticle_sticky.hpp"
#include "f58c269829-scene3d.h" // canonical Vector/Quaternion layouts; no new shared type owner
#include <cstddef>

#if !defined(_M_IX86)
#error "reticle_sticky.cpp requires x86"
#endif
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
extern volatile std::uint32_t height_0078d1d8;
}
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }
extern "C" {
void* __fastcall kotor_fastcall_005ed560(void*, void*);
void* __fastcall kotor_fastcall_005eda40(void*, void*);
kotor_scene3d::Vector* __fastcall quaternion_zdir_0043e250(const kotor_scene3d::Quaternion*, void*, kotor_scene3d::Vector*);
}

namespace {
using U=std::uint32_t;
using Vector=kotor_scene3d::Vector;
using Quaternion=kotor_scene3d::Quaternion;
template<class T> T& at(void* p,U offset){return *reinterpret_cast<T*>(static_cast<unsigned char*>(p)+offset);}
template<class R,class... A> R vc(void* p,U offset,A... a){
    using Fn=R(__fastcall*)(void*,void*,A...);
    return reinterpret_cast<Fn>(at<void*>(at<void*>(p,0),offset))(p,nullptr,a...);
}
void* client(){return at<void*>(kotor_menu_window_cells::manager_007a39fc,4);}
// Original local span [saved-SP+10,saved-SP+48): negative direction, reused Vector, two quaternions.
struct Frame {Vector negative;Vector vector;Quaternion node;Quaternion camera;};
static_assert(sizeof(Frame)==0x38,"original56-byte frame");
static_assert(offsetof(Frame,vector)==0x0c&&offsetof(Frame,node)==0x18&&offsetof(Frame,camera)==0x28,"callee output ranges");
const U tenth_0073e798=0x3dcccccdu;
const U one_half_0073f414=0x3fc00000u;

void negate_direction(const Vector* direction,Vector* negative){
    __asm {
        mov eax,direction
        mov ecx,negative
        fld dword ptr [eax]
        fchs
        fstp dword ptr [ecx]
        fld dword ptr [eax+4]
        fchs
        fstp dword ptr [ecx+4]
        fld dword ptr [eax+8]
        fchs
        fstp dword ptr [ecx+8]
    }
}
// Original FCOMPP compares cameraDot to pointDot, TEST AH,5/JNP: 1 iff ordered cameraDot<pointDot.
unsigned char classify(const Vector* negative,const Vector* point,const Vector* position){
    unsigned char other;
    __asm {
        mov ecx,negative
        mov edx,point
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+4]
        faddp st(1),st(0)
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        faddp st(1),st(0)
        mov edx,position
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+4]
        faddp st(1),st(0)
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        faddp st(1),st(0)
        fcompp
        fnstsw ax
        test ah,5
        setp other
    }
    return other?2:1;
}
void scale_component(float* value,int extent){
    __asm {
        mov ecx,value
        fild dword ptr extent
        fmul dword ptr [ecx]
        fstp dword ptr [ecx]
    }
}
}

// [0063D560,0063D6FE),414bytes,RET4/@12. Base-object fallback differs from creature behavior.
extern "C" unsigned char __fastcall kotor_fastcall_0063d560(void* self,void*,void* output){
    void* model=at<void*>(self,0x68);
    if(model==nullptr)return 0;
    void* part=vc<void*>(model,8,0xffu);
    if(part!=nullptr){
        Frame f;f.node=Quaternion{1.0f,0.0f,0.0f,0.0f};
        void* node=vc<void*>(part,0x10c,static_cast<const char*>("lookathook")); // original0074F77C
        if(node!=nullptr){
            auto* point=static_cast<Vector*>(output);
            vc<void>(node,0x0c,point,&f.node);
            void* camera=kotor_fastcall_005ed560(client(),nullptr);
            const auto* orientation=vc<Quaternion*>(camera,0x24,&f.camera); // original pending Vector push belongs to next call
            const Vector* direction=quaternion_zdir_0043e250(orientation,nullptr,&f.vector);
            negate_direction(direction,&f.negative);
            const auto* position=vc<Vector*>(camera,0x20,&f.vector);
            const unsigned char result=classify(&f.negative,point,position);
            const Vector value=*point; // by-value12-byte argument, captured before projection call
            const auto* projected=vc<Vector*>(camera,0x28,&f.vector,value,1u);
            point->x=projected->x;point->y=projected->y;point->z=projected->z;
            const int width=static_cast<std::int16_t>(at<std::uint16_t>(kotor_fastcall_005eda40(client(),nullptr),0x6c));
            scale_component(&point->x,width);
            const int height=static_cast<std::int16_t>(at<std::uint16_t>(kotor_fastcall_005eda40(client(),nullptr),0x6e));
            scale_component(&point->y,height);
            return result;
        }
    }
    return vc<unsigned char>(at<void*>(self,0x68),0xdc,output); // canonical0069F1E0 returns AL only
}

// [0060FD80,0060FF2C),428bytes,RET4/@12. Creature part/node selection is independent.
extern "C" unsigned char __fastcall kotor_fastcall_0060fd80(void* self,void*,void* output){
    void* model=at<void*>(self,0x68);
    void* part=vc<void*>(model,8,0xffu);
    if(part==nullptr)return 0;
    void* node=vc<void*>(part,0x10c,static_cast<const char*>("head_g")); // original0074F414
    if(node==nullptr){
        void* other=vc<void*>(at<void*>(self,0x68),8,0xfeu);
        if(other!=nullptr)node=vc<void*>(other,0x10c,static_cast<const char*>("head_g"));
        if(node==nullptr)node=vc<void*>(part,0x10c,static_cast<const char*>("rootdummy")); // original0074F408
    }
    if(node==nullptr)return 0;
    Frame f;f.node=Quaternion{1.0f,0.0f,0.0f,0.0f};
    auto* point=static_cast<Vector*>(output);
    vc<void>(node,0x0c,point,&f.node);
    void* camera=kotor_fastcall_005ed560(client(),nullptr);
    const auto* orientation=vc<Quaternion*>(camera,0x24,&f.camera);
    const Vector* direction=quaternion_zdir_0043e250(orientation,nullptr,&f.vector);
    negate_direction(direction,&f.negative);
    const auto* position=vc<Vector*>(camera,0x20,&f.vector);
    const unsigned char result=classify(&f.negative,point,position);
    const Vector value=*point;
    const auto* projected=vc<Vector*>(camera,0x28,&f.vector,value,1u);
    point->x=projected->x;point->y=projected->y;point->z=projected->z;
    const int width=static_cast<std::int16_t>(at<std::uint16_t>(kotor_fastcall_005eda40(client(),nullptr),0x6c));
    scale_component(&point->x,width);
    const int height=static_cast<std::int16_t>(at<std::uint16_t>(kotor_fastcall_005eda40(client(),nullptr),0x6e));
    scale_component(&point->y,height);
    return result;
}

// [0063D780,0063D7A7),39bytes,RET0/@8. Preserve raw ST0 and signed FILD of existing shared cells.
using kotor_menu_window_cells::height_0078d1d8;
using kotor_renderer_gl_cells::width_0078d1d4;
extern "C" __declspec(naked) double __fastcall kotor_fastcall_0063d780(void*,void*){
    __asm {
        fild dword ptr height_0078d1d8
        fmul dword ptr tenth_0073e798
        fild dword ptr width_0078d1d4
        fmul dword ptr tenth_0073e798
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fstp st(2)
        fstp st(0)
        ret
    }
}
// [00616FE0,00616FE7),7bytes,RET0/@8. Same literal independent of receiver, no forced state.
extern "C" __declspec(naked) double __fastcall kotor_fastcall_00616fe0(void*,void*){
    __asm {
        fld dword ptr one_half_0073f414
        ret
    }
}
