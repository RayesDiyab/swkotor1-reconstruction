// Original object VFX light / alpha / FOV group. x86 only; runtime gate held.
#include "d5ef879cc4-object_vfx_light.h"
#include "56bd987d24-startup_string.h"
#include "e39643f917-kotor_unreachable_trap.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

using U = std::uint32_t;
using I = std::int32_t;
using String = kotor_reconstruction::StartupString;
namespace aurora_model { struct Gob; struct Scene; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*, void*, U);
void* __fastcall client_get_player_creature_005ed540(void*, void*);
void* __fastcall kotor_fastcall_005ed530(void*, void*);
void* __fastcall kotor_fastcall_005ed560(void*, void*);
U __fastcall frame_R4_005ed5b0(void*, void*, U);
U __fastcall kotor_fastcall_005edc10(void*, void*);
void __fastcall object_vfx_00605410(void*, void*);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char*, const char*, std::FILE*, std::FILE*);
String* __fastcall GuiAssign005e5140(String*, void*, const char*);
char* __fastcall WindowCStr005e5670(String*, void*);
}
String* __fastcall GuiConcat005e5d10(String*, void*, String*, const String*);

namespace {
template<class F, class... A> auto opaque(F f, A... a) {
    // The original string unwind states survive extern-C provider exceptions.
    F volatile target = f; return target(a...);
}
template<class T> T read(const void* p, U off) {
    T v; std::memcpy(&v, static_cast<const unsigned char*>(p)+off, sizeof(v)); return v;
}
template<class T> void write(void* p, U off, T v) {
    std::memcpy(static_cast<unsigned char*>(p)+off, &v, sizeof(v));
}
void* ptr(const void* p, U off) { return read<void*>(p, off); }
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc, 4); }
template<class F> F slot(void* p, U off) { return read<F>(ptr(p,0),off); }
using Get = void* (__fastcall*)(void*,void*);
using GetU = U (__fastcall*)(void*,void*);
using SetU = U (__fastcall*)(void*,void*,U);
using SetPPP = void* (__fastcall*)(void*,void*,void*,const char*,I);
using SceneAttach = I (__fastcall*)(aurora_model::Gob*,void*,aurora_model::Scene*);
using SetModelU = U (__fastcall*)(aurora_model::Gob*,void*,U);
using FloatGet = double (__fastcall*)(void*,void*);
using SetCameraFloat = void (__fastcall*)(void*,void*,float);
using Fov = U (__fastcall*)(void*,void*,U,U,U);
using Model = void* (__fastcall*)(void*,void*,U,U);
using NodeTransform = U (__fastcall*)(aurora_model::Gob*,void*,U,U,U);
using Illumination = U (__fastcall*)(void*,void*,U,U,U,U);
struct Vec3 { float x,y,z; };
struct Quaternion { float w,x,y,z; };
using Transform = U (__fastcall*)(aurora_model::Gob*,void*,U,U,U,U,U,U,U,U);
static_assert(sizeof(Vec3)==12 && sizeof(Quaternion)==16 && sizeof(String)==8,"original carriers");
void* object(void* self) { return opaque(kotor_fastcall_005ed580,client(),nullptr,read<U>(self,0x40)); }
void* target(void* self) {
    void* p=object(self); return p ? slot<Get>(p,0x0c)(p,nullptr) : nullptr;
}
float bits(U v) { float f; std::memcpy(&f,&v,4); return f; }
// Literal values are pinned by the private original audit (object_vfx_literals.inc); closeC inlines them at each use
// so no runtime address lookup (and no unknown-literal trap) remains. Never read original VA storage.
struct TemporaryString {
    alignas(String) unsigned char storage[sizeof(String)];
    bool active=false;
    String* get() { return reinterpret_cast<String*>(storage); }
    ~TemporaryString() { destroy(); }
    void destroy() { if (active) { active=false; get()->~StartupString(); } }
};
void concatenate(String& out, String* base, const char* suffix) {
    String right(suffix);
    TemporaryString result;
    String* value=opaque(GuiConcat005e5d10,base,nullptr,result.get(),&right);
    result.active=true;
    out.Assign005e5c50(value);
    result.destroy(); // original concat result before suffix destructor
}
void refresh_player_model(void* p) {
    void* creature=slot<Get>(p,0x2c)(p,nullptr);
    if (creature && creature==opaque(client_get_player_creature_005ed540,client(),nullptr)) {
        void* light=ptr(creature,0x20);
        if (light) opaque(object_vfx_00605410,light,nullptr);
    }
}
// The FST stores to alpha but leaves ST(0) for FCOMP; neither operand is
// rounded to that float store before comparison. 53-bit x87 control is required.
void alpha_ramp(float& result, U n, bool slow, bool rising) {
    float* destination=&result;
    const float unsigned_bias=bits(0x4f800000), slow_rate=bits(0x392ec33e);
    const float rate=bits(0x3a83126f), half=bits(0x3f000000);
    const float one=1.0f, low=bits(0x3e99999a), alpha_offset=bits(0x3e4ccccd);
    const float upper=bits(0x3f4ccccd), zero=0.0f;
    unsigned short status;
    if (slow) {
        if (rising) {
            __asm { fild n }
            if (static_cast<std::int32_t>(n)<0) { __asm { fadd unsigned_bias } }
            __asm { fmul slow_rate
                mov eax,destination
                fst dword ptr [eax]
                fcomp one
                fnstsw ax
                mov status, ax }
            if (!(status & 0x4100)) result=one;
        } else {
            __asm { fild n }
            if (static_cast<std::int32_t>(n)<0) { __asm { fadd unsigned_bias } }
            __asm { fmul slow_rate
                fsubr one
                mov eax,destination
                fst dword ptr [eax]
                fcomp zero
                fnstsw ax
                mov status, ax }
            if ((status & 0x0500)==0x0100 || (status & 0x0500)==0x0400) result=zero;
        }
    } else {
        if (rising) {
            __asm { fild n }
            if (static_cast<std::int32_t>(n)<0) { __asm { fadd unsigned_bias } }
            __asm { fmul rate
                fmul half
                fadd low
                mov eax,destination
                fst dword ptr [eax]
                fcomp upper
                fnstsw ax
                mov status, ax }
            if (!(status & 0x4100)) result=upper;
        } else {
            __asm { fild n }
            if (static_cast<std::int32_t>(n)<0) { __asm { fadd unsigned_bias } }
            __asm { fmul rate
                fmul half
                fadd alpha_offset
                fsubr one
                mov eax,destination
                fst dword ptr [eax]
                fcomp low
                fnstsw ax
                mov status, ax }
            if ((status & 0x0500)==0x0100 || (status & 0x0500)==0x0400) result=low;
        }
    }
}
U fov_time(U n) {
    const float bias=bits(0x4f800000),rate=bits(0x3a83126f); float result;
    __asm { fild n }
    if (static_cast<std::int32_t>(n)<0) { __asm { fadd bias } }
    __asm { fmul rate
        fstp result }
    U out; std::memcpy(&out,&result,4); return out;
}
bool fov_ready(U n) {
    const float bias=bits(0x4f800000),limit=4000.0f; unsigned short status;
    __asm { fild n }
    if (static_cast<std::int32_t>(n)<0) { __asm { fadd bias } }
    __asm { fcomp limit
        fnstsw ax
        mov status,ax }
    // TEST AH,41h; JP exits for greater/unordered, continues less/equal.
    const U v=status & 0x4100; return v==0x0100 || v==0x4000;
}
void add_stored(float& p,float delta) {
    float* destination=&p;
    __asm { mov eax,destination
        fld dword ptr [eax]
        fadd delta
        fstp dword ptr [eax] }
}
}

extern "C" {
U __fastcall object_vfx_0063cb50(void* self,void*,U alpha,U part) {
    void* model=ptr(self,0x68);
    if (!model) return 0;
    using SetAlpha=U (__fastcall*)(void*,void*,U,U);
    return slot<SetAlpha>(model,0x94)(model,nullptr,alpha,part);
}
void __fastcall object_vfx_006a3e80(void* self,void*,U effect) {
    void* p=target(self); if (!p) return;
    const U n=static_cast<std::uint16_t>(effect-0x190);
    write<std::uint8_t>(self,0xd8,1);
    if (n>4) return;
    if (n==1 || n==4) { write<std::uint8_t>(self,0xc7,0); return; }
    opaque(object_vfx_0063cb50,p,nullptr,read<U>(self,0xd0),U(0xff));
    write<U>(self,0xcc,read<U>(self,0xd0));
    if (n==2 || n==3) {
        const float color[3]={n==2 ? 0.25f : 0.5f,n==2 ? 0.25f : 0.0f,1.0f};
        slot<Illumination>(p,0xa4)(p,nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(color)),3,0,1);
    }
}
void __fastcall object_vfx_006a4920(void* self,void*,U effect) {
    void* p=target(self); if (!p) return;
    const U n=static_cast<std::uint16_t>(effect-0x190); if(n>4) return;
    if(n==2 || n==3) slot<SetU>(p,0xa8)(p,nullptr,3);
    opaque(object_vfx_0063cb50,p,nullptr,U(0x3f800000),U(0xff));
}
void __fastcall object_vfx_006a4c00(void* self,void*,U delta) {
    void* p=target(self); if(!p) return;
    const U n=static_cast<std::uint16_t>(read<std::uint16_t>(self,0xa2)-0x190);
    if(n!=1 && n!=4) return;
    U elapsed=read<U>(self,0x9c); U period=n==1 ? 6000u : 1000u;
    if(elapsed>=period) {
        const auto sum=static_cast<std::uint8_t>(read<std::uint8_t>(self,0xc7)+static_cast<std::uint8_t>(elapsed/period));
        std::uint8_t phase=static_cast<std::uint8_t>(sum & 3);
        if(n==4) { if(phase==1) phase=2; else if(phase==3) phase=0; }
        write<std::uint8_t>(self,0xc7,phase); write<U>(self,0x9c,elapsed%period);
    }
    const U phase=read<std::uint8_t>(self,0xc7);
    if(phase==0 || phase==2) alpha_ramp(*reinterpret_cast<float*>(static_cast<unsigned char*>(self)+0xcc),read<U>(self,0x9c),n==1,phase==2);
    else if(phase==1) write<U>(self,0xcc,n==1 ? 0u : 0x3e99999au);
    else if(phase==3) write<U>(self,0xcc,n==1 ? 0x3f800000u : 0x3f4ccccdu);
    opaque(object_vfx_0063cb50,p,nullptr,read<U>(self,0xcc),U(0xff));
    write<U>(self,0x9c,read<U>(self,0x9c)+delta);
}
void __fastcall object_vfx_006a4580(void* self,void*,U effect) {
    void* player=opaque(client_get_player_creature_005ed540,client(),nullptr);
    void* p=target(self);
    if(!player || !p || read<U>(player,4)!=read<U>(p,4)) return;
    void* camera=opaque(kotor_fastcall_005ed560,client(),nullptr);
    if(!camera || static_cast<std::uint16_t>(effect-0x5dc)!=0) return;
    auto get_fov=slot<FloatGet>(camera,0x48);
    // Original006A45F7/45FA: retain ST0 until the one FSTP32 field store.
    __asm { mov ecx,camera
        xor edx,edx
        call get_fov
        mov eax,self
        fstp dword ptr [eax+0xe4] }
    void* fresh=opaque(kotor_fastcall_005ed560,client(),nullptr);
    using GetObject=U (__fastcall*)(void*,void*,U);
    void* controller=reinterpret_cast<void*>(static_cast<std::uintptr_t>(slot<GetObject>(fresh,0x80)(fresh,nullptr,0xffffffffu)));
    if(controller) {
        if(slot<GetU>(controller,0x10)(controller,nullptr)==0x106a) {
            write<U>(self,0xe8,read<U>(controller,0x110)); write<U>(controller,0x84,1);
        }
        (void)slot<GetU>(controller,0x10)(controller,nullptr);
    }
    slot<Fov>(camera,0x54)(camera,nullptr,0x43070000,0x3f400000,0);
    write<std::uint8_t>(self,0xc7,0); write<U>(self,0x9c,0x128e);
}
void __fastcall object_vfx_006a49a0(void* self,void*,U effect,U forced) {
    void* player=opaque(client_get_player_creature_005ed540,client(),nullptr);
    void* p=target(self);
    if(!player || !p) return;
    if(!forced && read<U>(player,4)!=read<U>(p,4)) return;
    void* camera=opaque(kotor_fastcall_005ed560,client(),nullptr);
    if(!camera || static_cast<std::uint16_t>(effect-0x5dc)!=0) return;
    slot<SetU>(camera,0x58)(camera,nullptr,0);
    U saved_fov=read<U>(self,0xe4);
    auto restore_fov=slot<SetCameraFloat>(camera,0x44);
    // Original006A4A1F..4A2A pushes the saved DWORD without float evaluation.
    __asm { push saved_fov
        mov ecx,camera
        xor edx,edx
        call restore_fov }
    void* fresh=opaque(kotor_fastcall_005ed560,client(),nullptr);
    using GetObject=U (__fastcall*)(void*,void*,U);
    void* controller=reinterpret_cast<void*>(static_cast<std::uintptr_t>(slot<GetObject>(fresh,0x80)(fresh,nullptr,0xffffffffu)));
    if(controller) {
        if(slot<GetU>(controller,0x10)(controller,nullptr)==0x106a) {
            write<U>(controller,0x110,read<U>(self,0xe8)); write<U>(controller,0x84,0);
        }
        (void)slot<GetU>(controller,0x10)(controller,nullptr);
    }
}
void __fastcall object_vfx_006a4e70(void* self,void*,U effect,U) {
    void* player=opaque(client_get_player_creature_005ed540,client(),nullptr);
    void* p=target(self);
    if(!player || !p || read<U>(player,4)!=read<U>(p,4) || opaque(kotor_fastcall_005edc10,client(),nullptr)) {
        object_vfx_006a49a0(self,nullptr,effect,1); return;
    }
    if(read<std::uint8_t>(self,0xc7)) return;
    void* camera=opaque(kotor_fastcall_005ed560,client(),nullptr);
    if(!camera || read<std::uint8_t>(self,0xc7) || !fov_ready(read<U>(self,0x9c))) return;
    if(static_cast<std::uint16_t>(effect-0x5dc)==0) {
        slot<SetU>(camera,0x58)(camera,nullptr,0);
        slot<Fov>(camera,0x54)(camera,nullptr,read<U>(self,0xe4),fov_time(read<U>(self,0x9c)),0);
    }
    write<std::uint8_t>(self,0xc7,1);
}
U __fastcall object_vfx_006a4fa0(void* self,void*,U) {
    if(!target(self)) return 0;
    void* p=opaque(aurora_new_aur_object_00449cc0,static_cast<const char*>("fx_light_clr"),static_cast<const char*>("fx_light"),nullptr,nullptr);
    write<void*>(self,0x8c,p);
    if(!p) return 0;
    slot<SetModelU>(p,0xc4)(static_cast<aurora_model::Gob*>(p),nullptr,3); return 1;
}
void __fastcall object_vfx_006a0df0(void* self,void*) {
    Vec3 position={0.0f,0.0f,0.0f}; Quaternion orientation={1.0f,0.0f,0.0f,0.0f};
    String anchor;
    void* p=target(self); if(!p || !ptr(self,0x8c)) return;
    void* model=slot<Model>(p,0x98)(p,nullptr,0xff,1); if(!model) return;
    void* item=reinterpret_cast<void*>(static_cast<std::uintptr_t>(slot<GetU>(p,0x4c)(p,nullptr)));
    void* door=reinterpret_cast<void*>(static_cast<std::uintptr_t>(slot<GetU>(p,0x14)(p,nullptr)));
    void* creature=slot<Get>(p,0x2c)(p,nullptr);
    if(creature) {
        slot<NodeTransform>(model,0xa0)(static_cast<aurora_model::Gob*>(model),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(static_cast<const char*>("headconjure"))),static_cast<U>(reinterpret_cast<std::uintptr_t>(&position)),static_cast<U>(reinterpret_cast<std::uintptr_t>(&orientation)));
        add_stored(position.y,0.75f); add_stored(position.z,0.5f);
    } else if(item || door) {
        String head;
        concatenate(head,reinterpret_cast<String*>(static_cast<unsigned char*>(item ? item : door)+(item ? 0x144 : 0x130)),(item ? static_cast<const char*>("_head_hit") : static_cast<const char*>("hhit")));
        const char* name=opaque(WindowCStr005e5670,&head,nullptr);
        slot<NodeTransform>(model,0xa0)(static_cast<aurora_model::Gob*>(model),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(name)),static_cast<U>(reinterpret_cast<std::uintptr_t>(&position)),static_cast<U>(reinterpret_cast<std::uintptr_t>(&orientation)));
        add_stored(position.y,0.375f); add_stored(position.z,0.25f);
    }
    void* light=ptr(self,0x8c);
    slot<Transform>(light,0x9c)(static_cast<aurora_model::Gob*>(light),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(static_cast<const char*>("root"))),read<U>(&position,0),read<U>(&position,4),read<U>(&position,8),read<U>(&orientation,0),read<U>(&orientation,4),read<U>(&orientation,8),read<U>(&orientation,12));
    if(item || door) concatenate(anchor,reinterpret_cast<String*>(static_cast<unsigned char*>(item ? item : door)+(item ? 0x144 : 0x130)),(item ? static_cast<const char*>("_ground") : static_cast<const char*>("grnd")));
    else opaque(GuiAssign005e5140,&anchor,nullptr,static_cast<const char*>("root"));
    light=ptr(self,0x8c);
    auto attach=slot<SetPPP>(light,0x50); // original caches vtable before CStr
    const char* name=opaque(WindowCStr005e5670,&anchor,nullptr);
    light=ptr(self,0x8c);
    attach(light,nullptr,model,name,0);
    refresh_player_model(p);
}
void __fastcall object_vfx_006a4830(void* self,void*,U effect) {
    if(!ptr(self,0x8c)) return;
    void* p=target(self);
    if(ptr(self,0xe0) && p) refresh_player_model(p);
    if(static_cast<std::uint16_t>(effect+0xfed4)==0x21 && opaque(client_get_player_creature_005ed540,client(),nullptr)) {
        void* player=opaque(client_get_player_creature_005ed540,client(),nullptr);
        if(read<U>(self,0x40)==read<U>(player,4)) {
            void* module=opaque(kotor_fastcall_005ed530,client(),nullptr);
            if(module && ptr(module,0x48)) {
                void* effect_node=ptr(ptr(module,0x48),0x184);
                slot<SetU>(effect_node,0x68)(effect_node,nullptr,0);
            }
        }
    }
    void* light=ptr(self,0x8c);
    slot<SetPPP>(light,0x50)(light,nullptr,nullptr,nullptr,0);
    light=ptr(self,0x8c); slot<SceneAttach>(light,0x4c)(static_cast<aurora_model::Gob*>(light),nullptr,nullptr);
}
void __fastcall object_vfx_006a38e0(void* self,void*,U effect) {
    String name;
    void* p=target(self); if(!p || !ptr(self,0x8c)) return;
    const U adjusted=effect+0xfed4;
    const U index=static_cast<std::uint16_t>(adjusted);
    bool large=true;
    write<std::uint8_t>(self,0xd8,0x14);
    if(index>0x52) return;
    // Original byte-index table: 0..33, default34 for34..49, 0..32 for50..82.
    const U branch=index>=50 ? index-50 : index;
    if(branch>=34) return;
    if(branch<33) {
        static const char* const names[]={
            "White_5m"/*007556E4*/,
            "White_10m"/*007556D8*/,
            "White_15m"/*007556CC*/,
            "White_20m"/*007556C0*/,
            "Blue_5m"/*007556B8*/,
            "Blue_10m"/*007556AC*/,
            "Blue_15m"/*007556A0*/,
            "Blue_20m"/*00755694*/,
            "Yello_5m"/*00755688*/,
            "Yello_10m"/*0075567C*/,
            "Yello_15m"/*00755670*/,
            "Yello_20m"/*00755664*/,
            "Red_5m"/*0075565C*/,
            "Red_10m"/*00755654*/,
            "Red_15m"/*0075564C*/,
            "Red_20m"/*00755644*/,
            "Orng_5m"/*0075563C*/,
            "Orng_10m"/*00755630*/,
            "Orng_15m"/*00755624*/,
            "Orng_20m"/*00755618*/,
            "Purp_5m"/*00755610*/,
            "Purp_10m"/*00755604*/,
            "Purp_15m"/*007555F8*/,
            "Purp_20m"/*007555EC*/,
            "Gren_5m"/*007555E4*/,
            "Gren_10m"/*007555D8*/,
            "Gren_15m"/*007555CC*/,
            "Gren_20m"/*007555C0*/,
            "Dark_Vision"/*007555B4*/,
            "Blind_Vision"/*007555A4*/,
            "Low_Light_Vision"/*00755590*/,
            "Ultra_Vision"/*00755580*/,
            "darkness"/*00755574*/};
        opaque(GuiAssign005e5140,&name,nullptr,names[branch]);
        if(branch<28) large=(branch%4)!=0;
        else {
            static const std::uint8_t classes[]={0x33,0x35,0x34,0x31,0x32};
            large=false; write<std::uint8_t>(self,0xd8,classes[branch-28]);
            write<U>(self,0xdc,branch==31 ? 0u : 1u);
        }
    } else {
        // Original repeats GetPlayerCreature; do not cache across calls.
        void* app=client();
        if(app && opaque(client_get_player_creature_005ed540,app,nullptr)) {
            void* player=opaque(client_get_player_creature_005ed540,client(),nullptr);
            if(read<U>(self,0x40)==read<U>(player,4)) {
                if(opaque(frame_R4_005ed5b0,client(),nullptr,read<U>(self,0x40))) {
                    opaque(GuiAssign005e5140,&name,nullptr,static_cast<const char*>("Blind_Vision"));
                    large=false; write<std::uint8_t>(self,0xd8,0x0d); write<U>(self,0xdc,0);
                }
                void* module=opaque(kotor_fastcall_005ed530,client(),nullptr);
                if(module && ptr(module,0x48)) {
                    void* node=ptr(ptr(module,0x48),0x184);
                    slot<SetU>(node,0x68)(node,nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(ptr(self,0x8c))));
                }
            }
        }
    }
    opaque(object_vfx_006a0df0,self,nullptr);
    const U intensity=index<50 ? 0x3f800000u : 0xbe9eb852u; // original DWORD speed carrier
    void* light=ptr(self,0x8c);
    using Activate=U (__fastcall*)(aurora_model::Gob*,void*,U,U,U,U);
    auto activate=slot<Activate>(light,0x18);
    const char* selected=opaque(WindowCStr005e5670,&name,nullptr);
    light=ptr(self,0x8c); activate(static_cast<aurora_model::Gob*>(light),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>(selected)),intensity,3,0);
    if(ptr(self,0x8c)) write<U>(self,0xd4,large ? 1u : 0u);
    void* creature=slot<Get>(p,0x2c)(p,nullptr);
    void* player=opaque(client_get_player_creature_005ed540,client(),nullptr);
    light=ptr(self,0x8c); slot<SetModelU>(light,0xc4)(static_cast<aurora_model::Gob*>(light),nullptr,creature && creature==player ? 2u : 4u);
}
}

