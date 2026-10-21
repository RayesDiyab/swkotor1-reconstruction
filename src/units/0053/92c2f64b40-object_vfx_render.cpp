// Object VFX rendering/material providers; original GOG K1 x86 semantics.
#include "13c020f36c-object_vfx_render.h"
#include "367b860a41-startup_string.h"
#include "f904d42941-resources.h"
#include <cstring>
using U = std::uint32_t;
using H = std::uint16_t;
using B = std::uint8_t;
using String = kotor_reconstruction::StartupString;
using Ref = menu_resources::Ref;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(String*, const char*, ...); }
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall client_get_player_creature_005ed540(void*,void*);
void* __fastcall kotor_m15_client_get_item_by_id_005ed5a0(void*,void*,U);
void* __fastcall kotor_m15e_client_get_mini_game_005edb00(void*,void*);
U __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void*,void*,U);
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
Ref* __fastcall async_ref_construct_string_00406d60(Ref*,void*,String*);
U __fastcall kotor_m15a_object_replace_texture_0063c0c0(void*,void*,const void*,const void*,U) noexcept(false);
void __fastcall kotor_m15a_object_enable_distortion_0063d540(void*,void*) noexcept(false);
void __fastcall kotor_m15a_object_disable_distortion_0063d550(void*,void*) noexcept(false);
U __fastcall object_vfx_lower_0063d440(void*,void*,Ref,float);
U __fastcall object_vfx_lower_0063d460(void*,void*);
U __fastcall object_vfx_lower_0063ce10(void*,void*);
U __fastcall object_vfx_lower_0063ce30(void*,void*);
void __cdecl object_vfx_lower_0044f0a0();
void __cdecl object_vfx_lower_0044f0b0();
void __cdecl object_vfx_lower_0044f130(float);
// Unique BSS owner, absent from bounded source/map/donor reuse evidence.
volatile H object_vfx_blur_count_00833bcc = 0;
}
namespace {
template<class F, class... A> auto opaque(F fn,A... args) {
    // Preserve active original string unwind states across extern-C providers.
    F volatile target=fn; return target(args...);
}
template<class T> T read(const void* p,U o) { T v; std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v; }
template<class T> void write(void* p,U o,T v) { std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v)); }
void* ptr(const void* p,U o) { return read<void*>(p,o); }
void* at(void* p,U o) { return static_cast<B*>(p)+o; }
template<class F> F slot(void* p,U o) { return read<F>(ptr(p,0),o); }
using Getter=void* (__fastcall*)(void*,void*);
using Color=void (__fastcall*)(void*,void*,const void*,U,U,U);
using RemoveColor=void (__fastcall*)(void*,void*,U);
using SetEnv=void (__fastcall*)(void*,void*,U,U,const void*);
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc,4); }
void* object(void* self) {
    void* p=opaque(kotor_fastcall_005ed580,client(),nullptr,read<U>(self,0x40));
    return p?slot<Getter>(p,0x0c)(p,nullptr):nullptr;
}
void* equipment(void* actor) {
    void* creature=slot<Getter>(actor,0x2c)(actor,nullptr);
    if (!creature) return nullptr;
    void* app=client(); // original caches client before the equipped-ID call
    const U id=opaque(kotor_m15a_creature_get_equipped_item_id_0060ca80,creature,nullptr,1u);
    return opaque(kotor_m15_client_get_item_by_id_005ed5a0,app,nullptr,id);
}
void ref_from_string(Ref* r,String* s) { (void)opaque(async_ref_construct_string_00406d60,r,nullptr,s); }
// 0073D708 = float bits3A83126F, 0073D764 =1, 0073D770 =2^32.
const float pulse_scale=0x1.0624dep-10f;
const float one=1.0f;
const float unsigned_fixup=0x1p32f;
// Exact x87 graph and dword rounding stores006A4ADF..006A4BBE. No FMA;
// precision control remains the composition's original53-bit contract.
void interpolate(U timer,B direction,const void* first,const void* second,void* output) {
    float tx,ty,qy,qz;
    __asm {
        mov eax, first
        mov edx, second
        mov ecx, output
        fild dword ptr timer
        cmp timer,0
        jge pulse_unsigned
        fadd dword ptr unsigned_fixup
    pulse_unsigned:
        fmul dword ptr pulse_scale
        cmp direction,0
        jne pulse_phase_one
        fld st(0)
        fld st(0)
        fmul dword ptr [edx]
        fstp dword ptr tx
        fld st(0)
        fmul dword ptr [edx+4]
        fstp dword ptr ty
        fmul dword ptr [edx+8]
        fld dword ptr one
        fsub st(0), st(2)
        fld st(0)
        fmul dword ptr [eax]
        fld st(1)
        fmul dword ptr [eax+4]
        fstp dword ptr qy
        fxch st(1)
        fmul dword ptr [eax+8]
        fstp dword ptr qz
        fadd dword ptr tx
        fstp dword ptr [ecx]
        fld dword ptr qy
        fadd dword ptr ty
        fstp dword ptr [ecx+4]
        fld dword ptr qz
        fadd st(0), st(1)
        fstp dword ptr [ecx+8]
        fstp st(0)
        jmp pulse_pop
    pulse_phase_one:
        fld dword ptr one
        fsub st(0), st(1)
        fld st(0)
        fmul dword ptr [edx]
        fstp dword ptr tx
        fld st(0)
        fmul dword ptr [edx+4]
        fstp dword ptr ty
        fmul dword ptr [edx+8]
        fxch st(1)
        fld st(0)
        fmul dword ptr [eax]
        fld st(1)
        fmul dword ptr [eax+4]
        fstp dword ptr qy
        fxch st(1)
        fmul dword ptr [eax+8]
        fstp dword ptr qz
        fadd dword ptr tx
        fstp dword ptr [ecx]
        fld dword ptr qy
        fadd dword ptr ty
        fstp dword ptr [ecx+4]
        fld dword ptr qz
        fadd st(0), st(1)
        fstp dword ptr [ecx+8]
    pulse_pop:
        fstp st(0)
    }
}
static_assert(sizeof(Ref)==16&&sizeof(String)==8&&sizeof(void*)==4,"original ABI/frame");
}
extern "C" {
// 006A1220..006A13FD RET4; string lifetime begins before d8 and object lookup.
void __fastcall object_vfx_006a1220(void* self,void*,U effect) {
    String text;
    write<B>(self,0xd8,0x14);
    void* actor=object(self); if(!actor)return;
    const U index=static_cast<H>(effect-1400u)-1u;
    if(index>25)return;
    // Original table deliberately has no fx_tex_13 entry.
    static const char* const names[]={"fx_tex_01","fx_tex_02","fx_tex_03","fx_tex_04","fx_tex_05","fx_tex_06","fx_tex_07","fx_tex_08","fx_tex_09","fx_tex_10","fx_tex_11","fx_tex_12","fx_tex_14","fx_tex_15","fx_tex_16","fx_tex_17","fx_tex_18","fx_tex_19","fx_tex_20","fx_tex_21","fx_tex_22","fx_tex_23","fx_tex_24","fx_tex_25","fx_tex_26","fx_tex_stealth"};
    (void)opaque(GuiAssign005e5140,&text,nullptr,names[index]);
    Ref name;ref_from_string(&name,&text);
    (void)opaque(object_vfx_lower_0063d440,actor,nullptr,name,0.02f);
    void* item=equipment(actor);if(!item)return;
    ref_from_string(&name,&text);
    (void)opaque(object_vfx_lower_0063d440,at(item,0x10),nullptr,name,0.02f);
}
void __fastcall object_vfx_006a1470(void* self,void*) {
    void* actor=object(self);if(!actor)return;
    (void)opaque(object_vfx_lower_0063d460,actor,nullptr);
    void* item=equipment(actor);if(item)(void)opaque(object_vfx_lower_0063d460,at(item,0x10),nullptr);
}
void __fastcall object_vfx_006a14e0(void* self,void*,U effect) {
    void* actor=object(self);if(!actor)return;
    U kind=static_cast<H>(effect-1600u);if(kind!=1&&kind!=2)return;
    void* app_manager=kotor_menu_window_cells::manager_007a39fc;
    object_vfx_blur_count_00833bcc=static_cast<H>(object_vfx_blur_count_00833bcc+1u);
    void* player=opaque(client_get_player_creature_005ed540,ptr(app_manager,4),nullptr);
    if(player&&read<U>(player,4)!=read<U>(actor,4))return;
    if(object_vfx_blur_count_00833bcc!=1)return;
    if(opaque(kotor_m15e_client_get_mini_game_005edb00,client(),nullptr))return;
    opaque(object_vfx_lower_0044f130,0.75f);
    opaque(object_vfx_lower_0044f0a0);
}
void __fastcall object_vfx_006a1570(void* self,void*) {
    if(!object(self))return;
    H n=static_cast<H>(object_vfx_blur_count_00833bcc-1u);
    object_vfx_blur_count_00833bcc=n;
    if(n&0x8000u)object_vfx_blur_count_00833bcc=0;
    else if(object_vfx_blur_count_00833bcc!=0)return;
    if(opaque(kotor_m15e_client_get_mini_game_005edb00,client(),nullptr))return;
    opaque(object_vfx_lower_0044f0b0);
}
void __fastcall object_vfx_006a15d0(void* self,void*,U effect) {
    void* actor=object(self);if(actor&&static_cast<H>(effect-1800u)==0)opaque(kotor_m15a_object_enable_distortion_0063d540,actor,nullptr);
}
void __fastcall object_vfx_006a1610(void* self,void*,U effect) {
    void* actor=object(self);if(actor&&static_cast<H>(effect-1800u)==0)opaque(kotor_m15a_object_disable_distortion_0063d550,actor,nullptr);
}
void __fastcall object_vfx_006a34b0(void* self,void*,U effect) {
    String text;
    void* actor=object(self);if(!actor)return;
    const U index=static_cast<H>(effect-1000u)-10u;
    const char* name;U flag;
    if(index==0) { name="vdu_tex_stone";flag=0xc; }
    else if(index==10) { name="vdu_tex_grstone";flag=0xc; }
    else if(index==20) { name="vdu_tex_bark";flag=0x10; }
    else if(index==30) { name="vdu_tex_shade";flag=0x14; }
    else return;
    (void)opaque(GuiAssign005e5140,&text,nullptr,name);
    write<U>(actor,flag,1);
    Ref replacement,empty;ref_from_string(&replacement,&text);
    empty.construct_00406d80("");
    (void)opaque(kotor_m15a_object_replace_texture_0063c0c0,actor,nullptr,&empty,&replacement,0u);
    void* item=equipment(actor);if(!item)return;
    ref_from_string(&replacement,&text);empty.construct_00406d80("");
    (void)opaque(kotor_m15a_object_replace_texture_0063c0c0,at(item,0x10),nullptr,&empty,&replacement,0u);
}
void __fastcall object_vfx_006a3660(void* self,void*,U effect) {
    String text;
    void* actor=object(self);if(!actor)return;
    opaque(kotor_reconstruction::StartupFormat005e5680,&text,"vdu_envmap%03u",static_cast<U>(static_cast<H>(effect-100u)));
    Ref empty,newempty,env;
    empty.construct_00406d80("");newempty.construct_00406d80("");
    (void)opaque(kotor_m15a_object_replace_texture_0063c0c0,actor,nullptr,&newempty,&empty,0u);
    ref_from_string(&env,&text);
    slot<SetEnv>(actor,0xb0)(actor,nullptr,1,0,&env);
}
void __fastcall object_vfx_006a3740(void* self,void*,U effect) {
    U color[3]={0,0,0};
    void* actor=object(self);if(!actor)return;
    const U index=static_cast<H>(effect-200u);
    write<B>(self,0xd8,0x14);
    if(index>5)return;
    static const U colors[6][3]={{0x3f800000,0,0},{0,0x3f800000,0},{0,0,0x3f800000},{0x3f800000,0,0x3f800000},{0x3f800000,0x3f800000,0},{0x3f800000,0x3f800000,0x3f800000}};
    std::memcpy(color,colors[index],sizeof color);
    slot<Color>(actor,0xa4)(actor,nullptr,color,3,0,1);
}
void __fastcall object_vfx_006a4720(void* self,void*,U) {
    void* actor=object(self);if(!actor)return;
    const U offsets[]={0xc,0x10,0x14};
    for(U off:offsets)if(read<U>(actor,off)==1)write<U>(actor,off,0);
    (void)opaque(object_vfx_lower_0063ce30,actor,nullptr);
    void* item=equipment(actor);if(item)(void)opaque(object_vfx_lower_0063ce30,at(item,0x10),nullptr);
}
void __fastcall object_vfx_006a47b0(void* self,void*,U) {
    void* actor=object(self);if(!actor)return;
    (void)opaque(object_vfx_lower_0063ce10,actor,nullptr);
    (void)opaque(object_vfx_lower_0063ce30,actor,nullptr);
}
void __fastcall object_vfx_006a47f0(void* self,void*,U) {
    void* actor=object(self);if(actor)slot<RemoveColor>(actor,0xa8)(actor,nullptr,3);
}
void __fastcall object_vfx_006a04c0(void* self,void*,U effect) {
    const H kind=static_cast<H>(effect-500u);if(kind>1)return;
    write<U>(self,0,kind==0?0x3f800000:0);write<U>(self,4,0);write<U>(self,8,kind==1?0x3f800000:0);
    write<U>(self,0xc,0x3f800000);write<U>(self,0x10,0x3f800000);write<U>(self,0x14,0x3f800000);
}
void __fastcall object_vfx_006a4a80(void* self,void*,U delta) {
    void* actor=object(self);if(!actor)return;
    U timer=read<U>(self,0x9c);
    if(timer>1000) {
        timer-=1000;write<U>(self,0x9c,timer);
        write<B>(self,0xc7,read<B>(self,0xc7)==0?1:0);
    }
    // Original stores all components before the virtual call. Timer is re-read
    // after it, so callee mutation is preserved; delta wraps as an unsigned DWORD.
    U color[3];
    interpolate(read<U>(self,0x9c),read<B>(self,0xc7),self,at(self,0xc),color);
    std::memcpy(at(self,0x18),color,sizeof color);
    slot<Color>(actor,0xa4)(actor,nullptr,at(self,0x18),3,0,1);
    write<U>(self,0x9c,read<U>(self,0x9c)+delta);
}
}
