// CSWCAmbientSound stinger and battle music. Original sound providers own playback.
#include "902c083eea-object_ambient.h"
#include "2600d775a9-resources.h"
#include "2fb025a280-base_extension_type.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using I=std::int32_t;
using String=kotor_reconstruction::StartupString;
using Ref=menu_resources::Ref;
namespace options_video { struct String; }
namespace client_movies {
I __fastcall StringNotEqual_005e5390(options_video::String*,void*,const char*);
}
namespace table29_required {
void* __fastcall gui_sound_construct_005d5a50(void*,void*);
void __fastcall gui_sound_destroy_005d6200(void*,void*);
U __fastcall gui_playing_005d5af0(void*,void*);
void __fastcall gui_priority_005d5ba0(void*,void*,U);
U __fastcall gui_play_005d5b10(void*,void*,Ref*,U,U,U,U);
Ref* __fastcall gui_file_playing_005d6230(void*,void*,Ref*);
}
namespace sound_fade29 { void __fastcall Begin005d5b90(void*,void*,U); }
extern "C" {
void* __cdecl live_operator_new(U);
void __cdecl live_free(void*);
I __cdecl crt_rand_006fc447();
Ref* __fastcall c2da_default_ref_00405ed0(Ref*,void*);
Ref* __fastcall async_ref_assign_string_004061f0(Ref*,void*,Ref*,String*);
Ref* __fastcall async_ref_construct_string_00406d60(Ref*,void*,String*);
void __fastcall c2da_ref_to_string_00405f70(Ref*,void*,String*);
void __fastcall kotor_fastcall_0068dc90(void*,void*);
}
namespace {
template<class T>T read(const void*p,U offset){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+offset,sizeof(v));return v;}
template<class T>void write(void*p,U offset,T v){std::memcpy(static_cast<unsigned char*>(p)+offset,&v,sizeof(v));}
void* ptr(const void*p,U offset){return read<void*>(p,offset);}
String* text(void*p,U offset){return reinterpret_cast<String*>(static_cast<unsigned char*>(p)+offset);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
bool nonempty(String*p){return invoke(client_movies::StringNotEqual_005e5390,reinterpret_cast<options_video::String*>(p),nullptr,"")!=0;}
using On=void(__fastcall*)(void*,void*,U);
struct ConstructorAllocation {
    void* allocation;
    ~ConstructorAllocation(){if(allocation)live_free(allocation);}
    void release(){allocation=nullptr;}
};
static_assert(sizeof(Ref)==16 && sizeof(String)==8,"original local carriers");
}
extern "C" void __fastcall object_ambient_0068db30(void*self,void*,String*track){
    Ref ref;
    (void)invoke(c2da_default_ref_00405ed0,&ref,nullptr); // before readiness guard
    if(!read<U>(self,0xf4))return;
    String* first=text(track,8);
    if(!nonempty(first))return;
    I count=1;
    String* second=text(track,0x10);
    if(nonempty(second)){
        count=2;
        if(nonempty(text(track,0x18)))count=3;
    }
    I choice=invoke(crt_rand_006fc447)%count; // original signed IDIV
    Ref output;
    if(choice==0)(void)invoke(async_ref_assign_string_004061f0,&ref,nullptr,&output,first);
    else if(choice==1)(void)invoke(async_ref_assign_string_004061f0,&ref,nullptr,&output,second);
    else if(choice==2)(void)invoke(async_ref_assign_string_004061f0,&ref,nullptr,&output,text(track,0x18));
    void* previous=ptr(self,0x104);
    if(previous){
        invoke(table29_required::gui_sound_destroy_005d6200,previous,nullptr);
        invoke(live_free,previous);
    }
    void* allocation=invoke(live_operator_new,U(4));
    void* stream=nullptr;
    {
        ConstructorAllocation constructor{allocation}; // original state0 only
        if(allocation)stream=invoke(table29_required::gui_sound_construct_005d5a50,allocation,nullptr);
        constructor.release(); // original state-1 before priority/play
    }
    write(self,0x104,stream);
    invoke(table29_required::gui_priority_005d5ba0,stream,nullptr,U(1));
    if(invoke(table29_required::gui_play_005d5b10,ptr(self,0x104),nullptr,&ref,U(4),U(0),U(0),U(0)))return;
    stream=ptr(self,0x104);
    if(stream){
        invoke(table29_required::gui_sound_destroy_005d6200,stream,nullptr);
        invoke(live_free,stream);
    }
    write(self,0x104,U(0));
}
extern "C" void __fastcall object_ambient_0068e610(void*self,void*,U enable){
    Ref unused_ref;
    (void)invoke(c2da_default_ref_00405ed0,&unused_ref,nullptr);
    String playing; // original state0 owns this string through all branches
    Ref ref; // separate original +14h CResRef from the default +24h local
    if(read<U>(self,0xfc)){
        write(self,0x14,enable);
        write(self,0xd0,U(10));
        if(invoke(table29_required::gui_playing_005d5af0,ptr(self,0xe0),nullptr)){
            I state=read<I>(self,0x100);
            if(state<4){
                if(enable)invoke(sound_fade29::Begin005d5b90,ptr(self,0xe0),nullptr,U(1000));
            }else if(state==4 && !enable){
                invoke(sound_fade29::Begin005d5b90,ptr(self,0xe0),nullptr,U(1000));
                invoke(object_ambient_0068db30,self,nullptr,text(self,0x6c));
            }
            return;
        }
        if(!enable)return;
        U track_offset,group,state;
        if(read<U>(self,0x14)){track_offset=0x6c;group=8;state=4;}
        else if(read<U>(self,0x1c)){track_offset=read<U>(self,0xf8)?0x8c:0xac;group=4;state=2;}
        else if(read<U>(self,4)){track_offset=read<U>(self,0xf8)?0x2c:0x4c;group=8;state=1;}
        else{write(self,0xd0,U(10000));write(self,0x100,U(0));return;}
        (void)invoke(async_ref_construct_string_00406d60,&ref,nullptr,text(self,track_offset));
        if(!invoke(table29_required::gui_play_005d5b10,ptr(self,0xe0),nullptr,&ref,group,U(0),U(1),U(0)))write(self,0xd0,U(10000));
        write(self,0x100,state);
        return;
    }
    if(!enable){
        if(!read<U>(self,0x14))return;
        invoke(sound_fade29::Begin005d5b90,ptr(self,0xe0),nullptr,U(800));
        if(read<U>(self,0xec)){
            invoke(object_ambient_0068db30,self,nullptr,text(self,0x6c));
            On on=read<On>(ptr(self,0),0);
            on(self,nullptr,U(1));
        }
        write(self,0x14,U(0));
        return;
    }
    if(!read<U>(self,0xec)||!read<U>(self,0xf4))return;
    String* battle=text(self,0x6c);
    if(!nonempty(battle))return;
    write(self,0x14,U(1));
    if(invoke(table29_required::gui_playing_005d5af0,ptr(self,0xe0),nullptr)){
        Ref* current=invoke(table29_required::gui_file_playing_005d6230,ptr(self,0xe0),nullptr,&ref);
        invoke(c2da_ref_to_string_00405f70,current,nullptr,&playing);
        if(invoke(exostring_compare_no_case_005e6450,&playing,nullptr,battle))return;
        invoke(sound_fade29::Begin005d5b90,ptr(self,0xe0),nullptr,U(800));
        write(self,4,U(0));
        write(self,0xd0,U(10));
        invoke(kotor_fastcall_0068dc90,self,nullptr);
        return;
    }
    invoke(table29_required::gui_priority_005d5ba0,ptr(self,0xe0),nullptr,U(2));
    (void)invoke(async_ref_construct_string_00406d60,&ref,nullptr,battle);
    U result=invoke(table29_required::gui_play_005d5b10,ptr(self,0xe0),nullptr,&ref,U(8),U(0),U(1),U(0));
    write(self,0xd0,result?U(0):U(10000)); // original NEG/SBB/AND/ADD
}
