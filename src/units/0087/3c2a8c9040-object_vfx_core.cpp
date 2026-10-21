// Original object-effect core; required lower closure remains separately owned.
#include "1ea72e76d6-startup_string.h"
#include "195d34d284-object_vfx_core.h"
#include <cstdint>
#include <cstring>
using U=std::uint32_t;
using H=std::uint16_t;
using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace aurora_model { struct Gob; struct Scene; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed530(void*,void*);
void* __fastcall client_get_player_creature_005ed540(void*,void*);
U __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
U __fastcall kotor_fastcall_005e9bd0(void*,void*,void*);
void __fastcall c2da_005e9c10(void*,void*,void*);
B __fastcall kotor_fastcall_006a0920(void*,void*);
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void*,void*,void*,float);
void __cdecl live_free(void*);
}
namespace {
template<class T>T rd(const void*p,U o){T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof(v));return v;}
template<class T>void wr(void*p,U o,T v){std::memcpy(static_cast<B*>(p)+o,&v,sizeof(v));}
void* ptr(const void*p,U o){return rd<void*>(p,o);}
template<class F>F slot(void*p,U o){return rd<F>(ptr(p,0),o);}
template<class F,class...A>auto invoke(F f,A...a){F volatile target=f;return target(a...);}
using Getter=void*(__fastcall*)(void*,void*);
using Delete=U(__fastcall*)(aurora_model::Gob*,void*,U);
using SoundDelete=void*(__fastcall*)(void*,void*,U);
using Disconnect=void*(__fastcall*)(void*,void*,void*,const char*,std::int32_t);
using Zero=std::int32_t(__fastcall*)(aurora_model::Gob*,void*,aurora_model::Scene*);
void* client(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void disconnect(void*self,U off){
    void*m=ptr(self,off);(void)slot<Disconnect>(m,0x50)(m,nullptr,nullptr,nullptr,0);
    m=ptr(self,off);(void)slot<Zero>(m,0x4c)(static_cast<aurora_model::Gob*>(m),nullptr,nullptr);
}
void erase_model(void*self,U off){
    void*m=ptr(self,off);if(m){slot<Delete>(m,0)(static_cast<aurora_model::Gob*>(m),nullptr,1);wr<void*>(self,off,nullptr);}
}
bool daylight(void*self){
    if(!rd<U>(self,0xdc))return false;
    void*m=invoke(kotor_fastcall_005ed530,client(),nullptr);
    return !invoke(object_vfx_00605440,ptr(m,0x48),nullptr);
}
// Destruction of already-constructed embedded strings, original unwind states1/0.
struct EmbeddedStringCleanup{
    String*p;bool active=true;
    void destroy(){if(active){active=false;p->~StartupString();}}
    ~EmbeddedStringCleanup(){destroy();}
};
}
extern "C" {
U __fastcall object_vfx_006a0480(void*self,void*){
    return rd<U>(self,0xac)&&rd<U>(self,0xb0)&&rd<U>(self,0xb4)&&rd<U>(self,0xc0)&&!rd<B>(self,0xc6)?1u:0u;
}
void __fastcall object_vfx_006a05e0(void*self,void*,void*model){
    if(model==ptr(self,0x74))wr<U>(self,0x90,3000);
    else if(model==ptr(self,0x78))wr<U>(self,0x94,3000);
    else if(model==ptr(self,0x7c))wr<U>(self,0x98,3000);
}
B __fastcall object_vfx_006a0930(void*,void*,U value){
    const H n=static_cast<H>(value);
    if(n<200)return 2;if(n<300)return 13;if(n<400)return 4;
    if(n<600)return 13;if(n<700)return 7;if(n<800)return 8;
    if(n<900)return 9;if(n<1000)return 10;if(n<1100)return 1;
    if(n<1200)return 11;if(n<1300)return 12;
    // Preserve the original late hexadecimal72/74 comparisons, even though
    // the prior guards make these two low thresholds unreachable here.
    if(n<0x72)return 14;if(n<1600)return 15;return n<0x74?16:0;
}
void __fastcall object_vfx_006a08d0(void*self,void*){
    if(ptr(self,0x8c)){disconnect(self,0x8c);erase_model(self,0x8c);}
}
void __fastcall object_vfx_006a0bf0(void*self,void*){
    if(ptr(self,0x8c)){disconnect(self,0x8c);erase_model(self,0x8c);}
    for(U i=0;i<3;++i){erase_model(self,0x74+i*4);erase_model(self,0x80+i*4);}
}
void __fastcall object_vfx_006a1810(void*self,void*){
    EmbeddedStringCleanup first{reinterpret_cast<String*>(static_cast<B*>(self)+0x24)};
    EmbeddedStringCleanup second{reinterpret_cast<String*>(static_cast<B*>(self)+0x2c)};
    void*s=ptr(self,0xec);if(s)(void)slot<SoundDelete>(s,0)(s,nullptr,1);
    invoke(object_vfx_006a0bf0,self,nullptr);
    second.destroy();first.destroy();
}
void __fastcall object_vfx_006a0c70(void*self,void*){
    wr<U>(self,0xe0,0);
    for(U i=0;i<3;++i)if(ptr(self,0x74+i*4))disconnect(self,0x74+i*4);
    if(!ptr(self,0x8c))return;
    if(rd<B>(self,0xd9)==4){
        void*o=invoke(kotor_fastcall_005ed580,client(),nullptr,rd<U>(self,0x40));
        if(o){void*v=slot<Getter>(o,0x0c)(o,nullptr);
            if(v){void*c=slot<Getter>(v,0x2c)(v,nullptr);
                if(c&&c==invoke(client_get_player_creature_005ed540,client(),nullptr)){
                    void*a=ptr(c,0x20);if(a)invoke(object_vfx_00605410,a,nullptr);
                }
            }
        }
    }
    disconnect(self,0x8c);
}
void __fastcall object_vfx_0063dd50(void*self,void*,void*removed){
    void*position=ptr(ptr(self,0x6c),4);
    void*effect=position?ptr(position,8):nullptr;
    while(position){
        if(invoke(kotor_fastcall_006a0920,removed,nullptr)!=7){
            const B c=invoke(kotor_fastcall_006a0920,effect,nullptr);
            if(invoke(kotor_fastcall_006a0920,removed,nullptr)==c&&removed!=effect&&rd<B>(effect,0xd8)&&!rd<U>(effect,0xc0)){
                if(rd<U>(effect,0xe0))invoke(object_vfx_006a0c70,effect,nullptr);
                return; // original stops after the first eligible effect, even detached
            }
        }
        effect=reinterpret_cast<void*>(invoke(kotor_fastcall_005e9bd0,ptr(self,0x6c),nullptr,&position));
    }
}
void __fastcall object_vfx_0063db90(void*self,void*,U class_word,U){
    const B wanted=static_cast<B>(class_word);B best=255;void*selected=nullptr;
    void*position=ptr(ptr(self,0x6c),0);void*effect=position?ptr(position,8):nullptr;
    while(position){
        if(!rd<U>(effect,0xc0)&&invoke(kotor_fastcall_006a0920,effect,nullptr)==wanted){
            const B priority=rd<B>(effect,0xd8);
            if(!priority){
                if(daylight(effect)){if(rd<U>(effect,0xe0))invoke(object_vfx_006a0c70,effect,nullptr);}
                else if(!rd<U>(effect,0xe0))invoke(object_vfx_006a5000,effect,nullptr);
            }else if(priority<=best){best=priority;selected=effect;}
        }
        effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(unresolved_list_next_005e9b90,ptr(self,0x6c),nullptr,&position)));
    }
    position=ptr(ptr(self,0x6c),0);effect=position?ptr(position,8):nullptr;
    while(position){
        if(!rd<U>(effect,0xc0)&&invoke(kotor_fastcall_006a0920,effect,nullptr)==wanted&&rd<B>(effect,0xd8)){
            if(effect==selected){
                if(invoke(kotor_fastcall_006a0920,effect,nullptr)==4){
                    if(daylight(effect)){
                        if(rd<U>(effect,0xe0)&&!rd<U>(effect,0xbc))invoke(object_vfx_006a6590,effect,nullptr,0u);
                        effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(unresolved_list_next_005e9b90,ptr(self,0x6c),nullptr,&position)));
                        continue;
                    }
                    if(rd<U>(effect,0xbc))invoke(object_vfx_006a5fc0,effect,nullptr);
                }
                if(!rd<U>(effect,0xe0))invoke(object_vfx_006a5000,effect,nullptr);
            }else if(rd<U>(effect,0xe0))invoke(object_vfx_006a0c70,effect,nullptr);
        }
        effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(unresolved_list_next_005e9b90,ptr(self,0x6c),nullptr,&position)));
    }
}
void __fastcall object_vfx_006a6a50(void*self,void*,U delta){
    String unused; // native original EH state0 includes every following call
    void*o=invoke(kotor_fastcall_005ed580,client(),nullptr,rd<U>(self,0x40));if(!o)return;
    void*view=slot<Getter>(o,0x0c)(o,nullptr);if(!view)return;
    const B phase=rd<B>(self,0xc6);
    if(phase==1||phase==3){
        U time=rd<U>(self,0x9c);
        if(time<=delta){
            wr<U>(self,0x9c,0);
            if(phase==1){
                invoke(object_vfx_006a56f0,self,nullptr,static_cast<U>(rd<H>(self,0xa0)));
                invoke(object_vfx_006a08d0,self,nullptr);
                if(!rd<B>(self,0xc8)){wr<B>(self,0xc6,0);wr<U>(self,0xc0,1);}
                else if(!rd<U>(self,0xc0)){
                    const U kind=rd<H>(self,0xa2);wr<B>(self,0xc6,2);
                    wr<B>(self,0xd9,invoke(object_vfx_006a0930,self,nullptr,kind));
                    (void)invoke(object_vfx_006a5890,self,nullptr,kind);
                    invoke(object_vfx_006a54d0,self,nullptr,static_cast<U>(rd<H>(self,0xa2)));
                    if(!rd<U>(self,0xe0))invoke(object_vfx_006a0c70,self,nullptr);
                }
            }else{
                if(rd<U>(self,0xe0))invoke(object_vfx_006a56f0,self,nullptr,static_cast<U>(rd<H>(self,0xa4)));
                invoke(object_vfx_006a08d0,self,nullptr);wr<B>(self,0xc6,0);
            }
        }else{
            wr<U>(self,0x9c,time-delta);
            if(rd<U>(self,0xa8)&&rd<U>(self,0xe0)){
                if(!rd<U>(self,0xc0)&&phase==1)invoke(object_vfx_006a69e0,self,nullptr,static_cast<U>(rd<H>(self,0xa0)),delta);
                else if(phase==3)invoke(object_vfx_006a69e0,self,nullptr,static_cast<U>(rd<H>(self,0xa4)),delta);
            }
        }
    }else if(phase==2&&rd<U>(self,0xa8)&&rd<U>(self,0xe0)&&!rd<U>(self,0xc0)){
        invoke(object_vfx_006a69e0,self,nullptr,static_cast<U>(rd<H>(self,0xa2)),delta);
    }
    for(U i=0;i<3;++i){
        const U timer=0x4c+i*4,state=0x58+i*2;U time=rd<U>(self,timer);
        if(!time)continue;if(time>delta){wr<U>(self,timer,time-delta);continue;}
        const H mode=rd<H>(self,state);wr<U>(self,timer,0);
        if(mode==1&&rd<B>(self,0xc8)){
            wr<H>(self,state,2);void*m=ptr(self,timer+0x28);
            using Animate=U(__fastcall*)(aurora_model::Gob*,void*,U,U,U,U);
            slot<Animate>(m,0x18)(static_cast<aurora_model::Gob*>(m),nullptr,static_cast<U>(reinterpret_cast<std::uintptr_t>("duration")),0x3f800000u,0,0); // original755778, exact raw1.0 bits
        }else if(mode==1||mode==3){
            invoke(object_vfx_006a05e0,self,nullptr,ptr(self,timer+0x28));
            // Original helper preserves EDX=1 from006A6C48 through006A6C8C.
            wr<U>(self,0xb8,1);wr<U>(self,0xc0,1);
        }
    }
    void*s=ptr(self,0xec);if(s)invoke(kotor_m15_sound_source_set_position_005d59e0,s,nullptr,static_cast<void*>(static_cast<B*>(view)+0x24),0.0f);
    if(rd<U>(self,0xb8))for(U i=0;i<3;++i){
        const U flag=0xac+i*4,timer=0x90+i*4;
        if(!rd<U>(self,flag)){U time=rd<U>(self,timer);if(time<=delta)wr<U>(self,flag,1);else wr<U>(self,timer,time-delta);}
    }
}
void __fastcall object_vfx_0063f490(void*self,void*){
    void*position=ptr(ptr(self,0x6c),0);void*effect=position?ptr(position,8):nullptr;
    using Reclassify=void(__fastcall*)(void*,void*,U,U);
    while(effect){
        invoke(object_vfx_006a6a50,effect,nullptr,rd<U>(self,0x44));
        Reclassify dc=slot<Reclassify>(self,0xdc);
        dc(self,nullptr,invoke(kotor_fastcall_006a0920,effect,nullptr),0);
        if(invoke(object_vfx_006a0480,effect,nullptr)){
            if(!rd<B>(effect,0xc8)){
                invoke(object_vfx_0063dd50,self,nullptr,effect);
                dc=slot<Reclassify>(self,0xdc);
                dc(self,nullptr,invoke(kotor_fastcall_006a0920,effect,nullptr),0);
            }
            void*next_position=position;
            void*next=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(unresolved_list_next_005e9b90,ptr(self,0x6c),nullptr,&next_position)));
            invoke(c2da_005e9c10,ptr(self,0x6c),nullptr,position);
            invoke(object_vfx_006a0bf0,effect,nullptr);
            invoke(object_vfx_006a1810,effect,nullptr);
            invoke(live_free,effect);effect=next;position=next_position;
        }else effect=reinterpret_cast<void*>(static_cast<std::uintptr_t>(invoke(unresolved_list_next_005e9b90,ptr(self,0x6c),nullptr,&position)));
    }
}
}
