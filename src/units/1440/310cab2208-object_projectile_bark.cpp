#include "e7bb1dbf1b-object_projectile_bark.h"
#include "5f63a60116-startup_string.h"
#include "0e6410bb6d-resources.h"
#include "cca80f7c35-m15e.h"
#include <cstring>

using U=std::uint32_t;
using I=std::int32_t;
using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace kotor_menu_10_19 { struct Tlk; }
namespace gui_load { struct Text; }
namespace tlk_core {
void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk*,void*,String*);
}
namespace table29_required {
void* __fastcall gui_sound_construct_005d5a50(void*,void*);
void __fastcall gui_sound_destroy_005d6200(void*,void*);
void __fastcall gui_priority_005d5ba0(void*,void*,U);
void __fastcall gui_volume_005d5b50(void*,void*,U);
U __fastcall gui_play_005d5b10(void*,void*,menu_resources::Ref*,U,U,U,U);
}
extern "C" {
extern void* kotor_cell_tlk_client_007a3a0c;
U __fastcall kotor_fastcall_004aea40(void*,void*);
void* __fastcall kotor_fastcall_004ae910(void*,void*,U);
U __fastcall kotor_fastcall_005eda50(void*,void*,U);
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
char* __fastcall WindowCStr005e5670(String*,void*);
void __fastcall gui_load_text_params_set_text_00415e00(void*,void*,const String*) noexcept(false);
I __fastcall kotor_fastcall_00414cd0(void*,void*);
U __fastcall gui_load_text_ideal_height_00414eb0(gui_load::Text*,void*);
void __fastcall kotor_fastcall_005d5b80(void*,void*);
void __fastcall kotor_m15_stream_source_set_3d_005d6290(void*,void*,U);
void __fastcall kotor_m15_stream_source_set_position_005d62c0(void*,void*,const void*);
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
}
namespace {
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
void* at(void*p,U o){return static_cast<B*>(p)+o;}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
using AsObject=decltype(KotorM15ObjectMethods_007516c8::s00c_as_creature_or_self);
using AsCreature=decltype(KotorM15ObjectMethods_007516c8::s02c_as_creature);
void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void* server(){return ptr(kotor_menu_window_cells::manager_007a39fc,8);}
void set_duration(void*self,I character_count){
    const U per_character=0x3de147aeu,one=0x3f800000u;
    float* destination=reinterpret_cast<float*>(at(self,0x1b4));
    __asm {
        fild dword ptr character_count
        fmul dword ptr per_character
        fadd dword ptr one
        mov eax,destination
        fstp dword ptr [eax]
    }
}
}
extern "C" {
// Existing W4 GetObjectGender body, with original tlk+58 reloads restored.
// Original00674B53/B5F/B71 reload after preceding virtual calls.
std::uint8_t __fastcall kotor_fastcall_00674b40(void*tlk,void*){
    void*object=ptr(tlk,0x58);
    if(object && slot<AsObject>(object,0x0c)(object,nullptr)){
        object=ptr(tlk,0x58);
        if(slot<AsCreature>(object,0x2c)(object,nullptr)){
            object=ptr(tlk,0x58);
            void*creature=slot<AsCreature>(object,0x2c)(object,nullptr);
            if(ptr(creature,0x2f8)){
                object=ptr(tlk,0x58);
                creature=slot<AsCreature>(object,0x2c)(object,nullptr);
                return rd<B>(ptr(creature,0x2f8),0x31);
            }
        }
    }
    return 0; // Original immutable byte0073ECD0 is zero.
}
void __fastcall bark_stream_internal_set_005dbb90(void*self,void*,U value){
    if(rd<U>(self,0x68)!=value){wr<U>(self,0x68,value);wr<U>(self,0x6c,1);}
}
void __fastcall bark_stream_set_005d5bb0(void*self,void*,U value){
    if(void*internal=ptr(self,0))bark_stream_internal_set_005dbb90(internal,nullptr,value);
}
void __fastcall object_projectile_bark_006a9920(void*self,void*,U actor,String*input,
                                               menu_resources::Ref*voice,U flags){
    U player_id=invoke(kotor_fastcall_004aea40,server(),nullptr);
    void*player=invoke(kotor_fastcall_004ae910,server(),nullptr,player_id);
    U client_id=invoke(kotor_fastcall_005eda50,client(),nullptr,rd<U>(player,0x38));
    void*object=invoke(kotor_fastcall_005ed580,client(),nullptr,client_id);
    String text(*input);
    void*tlk=kotor_cell_tlk_client_007a3a0c;
    wr<void*>(tlk,0x58,object);
    B gender=invoke(kotor_fastcall_00674b40,tlk,nullptr);
    U token_flags=rd<U>(tlk,0x5c);
    wr<B>(tlk,0x3c,gender);
    wr<U>(tlk,0x5c,token_flags&0xfffffffeu);
    invoke(tlk_core::ParseStr0041dd30,static_cast<kotor_menu_10_19::Tlk*>(tlk),nullptr,&text);
    {
        String display(invoke(WindowCStr005e5670,&text,nullptr));
        invoke(gui_load_text_params_set_text_00415e00,at(self,0x14c),nullptr,
               static_cast<const String*>(&display));
    }
    I border=invoke(kotor_fastcall_00414cd0,at(self,0xd4),nullptr);
    void*text_object=at(self,0x134);
    U height=slot<decltype(&gui_load_text_ideal_height_00414eb0)>(text_object,8)(
        static_cast<gui_load::Text*>(text_object),nullptr);
    U extent[4]={rd<U>(self,4),rd<U>(self,8),rd<U>(self,0xc),
                 height+static_cast<U>(border)*2u+10u};
    slot<decltype(&kotor_fastcall_006a9fd0)>(self,4)(self,nullptr,
        static_cast<U>(reinterpret_cast<std::uintptr_t>(extent)));
    wr<U>(self,0x44,rd<U>(self,0x44)&0xfffff8ffu);
    U previous_flags=rd<U>(self,0x1c8);
    U refreshed_flags=rd<U>(self,0x1c8);
    wr<U>(self,0x1c0,actor);
    wr<U>(self,0x1c8,refreshed_flags^((previous_flags^flags)&1u));
    I length=text.BorrowedLength005e5790();
    // Original006A9A78 captures the old stream before its call-free x87 stores.
    void*stream=ptr(self,0x1b8);
    set_duration(self,length);
    if(stream){
        invoke(kotor_fastcall_005d5b80,stream,nullptr);
        stream=ptr(self,0x1b8);
        if(stream){invoke(table29_required::gui_sound_destroy_005d6200,stream,nullptr);invoke(live_free,stream);}
        wr<void*>(self,0x1b8,nullptr);
    }
    void*raw=invoke(live_operator_new,4u);
    void*created=nullptr;
    try{if(raw)created=invoke(table29_required::gui_sound_construct_005d5a50,raw,nullptr);}
    catch(...){invoke(live_free,raw);throw;}
    wr<void*>(self,0x1b8,created);
    if(actor!=0x7f000000u){
        object=invoke(kotor_fastcall_005ed580,client(),nullptr,actor);
        void*view=slot<AsObject>(object,0x0c)(object,nullptr);
        U position[3]={rd<U>(view,0x24),rd<U>(view,0x28),rd<U>(view,0x2c)};
        invoke(kotor_m15_stream_source_set_3d_005d6290,ptr(self,0x1b8),nullptr,1u);
        invoke(bark_stream_set_005d5bb0,ptr(self,0x1b8),nullptr,0u);
        invoke(kotor_m15_stream_source_set_position_005d62c0,ptr(self,0x1b8),nullptr,
               static_cast<const void*>(position));
    }
    invoke(table29_required::gui_priority_005d5ba0,ptr(self,0x1b8),nullptr,0x1au);
    invoke(table29_required::gui_volume_005d5b50,ptr(self,0x1b8),nullptr,0x7fu);
    U playing=invoke(table29_required::gui_play_005d5b10,ptr(self,0x1b8),nullptr,voice,8u,1u,0u,0u);
    wr<U>(self,0x1c4,playing);
}
}
