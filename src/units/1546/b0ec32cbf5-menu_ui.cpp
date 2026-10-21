#include "ad935568e2-menu_imports.h"
#include "ca1c412957-scene3d.h"
#include <cstring>

namespace {
using U=std::uint32_t;
using I=std::int32_t;
using B=unsigned char;
using S=kotor_reconstruction::StartupString;
void* at(void* p,U n){return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p)+n);}
template<class T>T load(const void* p,U n){T v;std::memcpy(&v,static_cast<const B*>(p)+n,sizeof v);return v;}
template<class T>void store(void* p,U n,T v){std::memcpy(at(p,n),&v,sizeof v);}
const KotorM15ObjectMethods_007516c8* methods(void* p){return load<const KotorM15ObjectMethods_007516c8*>(p,0);}
const menu_server::Slots* server_methods(void* p){return load<const menu_server::Slots*>(p,0);}
void* client(){return load<void*>(menu_continuation::app_manager_007a39fc,4);}
const float* fp(void* p,U n){return static_cast<const float*>(at(p,n));}
struct HiddenString {
    alignas(S) B bytes[sizeof(S)];
    bool live=false;
    S* get(){return reinterpret_cast<S*>(bytes);}
    ~HiddenString() noexcept(false){if(live)get()->~StartupString();}
};
// Exact FCOMP/FNSTSW predicates; unordered is neither ordered > nor ordered <.
U compare_zero(const float* operand){
    static const U zero_bits=0;
    U status;
    __asm {
        mov eax, operand
        fld dword ptr [eax]
        fcomp dword ptr zero_bits
        fnstsw ax
        movzx eax, ax
        mov status, eax
    }
    return status;
}
U compare_integer(I integer,const float* operand){
    U status;
    __asm {
        fild integer
        mov eax, operand
        fcomp dword ptr [eax]
        fnstsw ax
        movzx eax, ax
        mov status, eax
    }
    return status;
}
bool greater(U sw){return (sw&0x4100u)==0;}
bool less(U sw){return (sw&0x0500u)==0x0100u;}
// 006862A7..00686354 keeps the half-width live across the two comparisons.
// Only the left candidate is rounded to m32; the right sum stays on x87.
U horizontal_case(I half,I left,I right,const float* point,float* candidate){
    U result;
    __asm {
        fild half
        mov eax, point
        fld dword ptr [eax]
        fsub st(1)
        mov eax, candidate
        fstp dword ptr [eax]
        fild left
        fcomp dword ptr [eax]
        fnstsw ax
        test ah, 41h
        jnz check_right
        fstp st(0)
        mov result, 1
        jmp finished
    check_right:
        mov eax, point
        fld dword ptr [eax]
        fadd st(1)
        fild right
        fcompp
        fnstsw ax
        fstp st(0)
        test ah, 5
        jp free_position
        mov result, 2
        jmp finished
    free_position:
        mov result, 0
    finished:
    }
    return result;
}
// The genuine conversion consumes ST0 and returns EDX:EAX. Only EAX is used
// by 00686090. Arithmetic below retains the original instruction widths/order.
U truncate_float(const float* operand){
    U result;
    __asm {
        mov eax, operand
        fld dword ptr [eax]
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
U truncate_y_minus(const float* operand,I height){
    static const U padding_bits=0x42000000u;
    U result;
    __asm {
        mov eax, operand
        fld dword ptr [eax]
        fsub dword ptr padding_bits
        fisub height
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
U truncate_y_plus(const float* operand,I height){
    static const U padding_bits=0x42000000u;
    U result;
    __asm {
        fild height
        mov eax, operand
        fadd dword ptr [eax]
        fadd dword ptr padding_bits
        call unresolved_ftol2_006fae8c
        mov result, eax
    }
    return result;
}
U clamp_original(U value,U lower,U upper){
    const U minimum=static_cast<I>(upper)<static_cast<I>(value)?upper:value;
    if(static_cast<I>(lower)>static_cast<I>(minimum))return lower;
    return static_cast<I>(upper)<static_cast<I>(value)?upper:value;
}
}

extern "C" void __fastcall kotor_fastcall_00685cb0(void* self,void*,void* target){
    S text;
    if(target!=nullptr){
        void* const server=kotor_m15a_object_get_server_object_0063d4b0(target,nullptr);
        if(server!=nullptr){
            S unused; // Original independently constructed inner string/unwind state.
            U name_id;
            kotor_fastcall_0063d300(target,nullptr,&text,&name_id);
            if(name_id==0){
                if(greater(compare_zero(fp(self,0x1adc)))){
                    HiddenString temporary;
                    auto* const manager=menu_continuation::app_manager_007a39fc;
                    const U string_id=load<U>(self,0x1ae4);
                    void* const string_client=load<void*>(manager,4);
                    auto* result=static_cast<S*>(kotor_fastcall_005edeb0(string_client,nullptr,temporary.get(),string_id));
                    temporary.live=true;
                    text.Assign005e5c50(result);
                }else{
                    const I selected=load<std::int8_t>(self,0x1aeb);
                    const U available=kotor_fastcall_00685840(at(self,0x54u+static_cast<U>(selected)*0x71cu),nullptr);
                    void* list=at(self,static_cast<U>(selected)*12u);
                    if(available!=0 && selected>=0 && load<I>(list,4)>0){
                        const I count=load<I>(list,4);
                        const I category=load<std::int8_t>(self,0x1aea);
                        const U id=load<U>(self,(static_cast<U>(selected)+3u*(static_cast<U>(category)+3u))*4u);
                        void* row=nullptr;
                        if(id!=0xffffffffu){
                            void* const data=load<void*>(list,0);
                            for(U index=0;static_cast<I>(index)<count;++index){
                                void* const entry=at(data,index*0x38u);
                                if(load<U>(entry,8)==id)row=entry; // Last duplicate wins.
                            }
                        }
                        if(row==nullptr)row=load<void*>(list,0);
                        text.Assign005e5c50(static_cast<S*>(row));
                        const I uses=load<I>(row,0x34);
                        if(uses>1)kotor_reconstruction::StartupFormat005e5680(&text,"%s (%d)",WindowCStr005e5670(&text,nullptr),uses);
                    }else{
                        HiddenString temporary;
                        auto* result=static_cast<S*>(methods(target)->s070_get_name(target,nullptr,temporary.get()));
                        temporary.live=true;
                        text.Assign005e5c50(result);
                    }
                }
            }
            const I maximum=static_cast<std::int16_t>(server_methods(server)->max_hp_098(server,nullptr,1));
            gui_m48_progress_set_max_00417f90(at(self,0x198c),nullptr,static_cast<U>(maximum));
            const I current=static_cast<std::int16_t>(server_methods(server)->current_hp_09c(server,nullptr,0));
            kotor_original::kotor_fastcall_00417fb0(at(self,0x198c),nullptr,current);
        } // Inner string is destroyed before the final text update.
    }
    kotor_fastcall_00685af0(self,nullptr,&text);
    store<U>(self,0x16dc,0x3f800000u);
}

extern "C" void __fastcall kotor_fastcall_00686090(void* self,void*,void*,void* target){
    U flags=load<U>(self,0x1aec);
    if(target==nullptr){store(self,0x1aec,flags&~1u);return;}
    flags|=1u;
    store(self,0x1aec,flags);
    const U old_height=load<U>(self,0x15b8);
    const bool populated=load<U>(self,4)!=0 || load<U>(self,0x10)!=0 || load<U>(self,0x1c)!=0;
    const U height=populated?load<U>(self,0x64)+load<U>(self,0x5c):load<U>(self,0x199c)+load<U>(self,0x1994);
    store(self,0x15b8,height);
    if((flags&2u)!=0 || greater(compare_zero(fp(self,0x1ae0)))){
        store(self,0x15b0,load<U>(self,0x15b0)+old_height-height);
        return; // Original pinned/timer path does not clamp.
    }
    kotor_scene3d::Vector point{};
    std::uint8_t category;
    if(methods(target)->s02c_as_creature(target,nullptr)!=nullptr){
        void* const creature=methods(target)->s02c_as_creature(target,nullptr);
        category=kotor_fastcall_0060ff30(creature,nullptr,&point);
    }else category=methods(target)->s13c_get_reticle_point(target,nullptr,&point);
    const I screen_width=load<std::int16_t>(kotor_fastcall_005eda40(client(),nullptr),0x6c);
    const I screen_height=load<std::int16_t>(kotor_fastcall_005eda40(client(),nullptr),0x6e);
    if(category!=0){
        const U positioned_height=load<U>(self,0x15b8); // Original reload follows virtual projection callbacks.
        const U left=load<U>(self,0x15bc);
        const U top=load<U>(self,0x15c0);
        const U width=load<U>(self,0x15b4);
        const U extent_x=load<U>(self,0x15c4);
        const U extent_y=load<U>(self,0x15c8);
        const U bottom=extent_y+top-32u;
        if(category==2){
            store(self,0x15ac,greater(compare_integer(screen_width/2,&point.x))?extent_x-width+left:left);
            if(greater(compare_integer(static_cast<I>(top+32u),&point.y)))
                store(self,0x15b0,extent_y+top-positioned_height-32u);
            else if(less(compare_integer(static_cast<I>(bottom),&point.y)))
                store(self,0x15b0,top);
            else if(greater(compare_integer(screen_height/2,&point.y)))
                store(self,0x15b0,static_cast<U>(screen_height)-truncate_y_plus(&point.y,static_cast<I>(positioned_height)));
            else store(self,0x15b0,truncate_y_minus(&point.y,static_cast<I>(positioned_height)));
        }else{
            const I half=static_cast<I>(width)/2;
            float candidate;
            const U horizontal=horizontal_case(half,static_cast<I>(left),static_cast<I>(extent_x+left),&point.x,&candidate);
            if(horizontal!=0){
                store(self,0x15ac,horizontal==1?left:extent_x-width+left);
                if(greater(compare_integer(static_cast<I>(top+32u),&point.y)))store(self,0x15b0,top);
                else if(less(compare_integer(static_cast<I>(bottom),&point.y)))store(self,0x15b0,extent_y+top-positioned_height-32u);
                else store(self,0x15b0,truncate_y_minus(&point.y,static_cast<I>(positioned_height)));
            }else{
                if(greater(compare_integer(static_cast<I>(top+32u),&point.y))){
                    store(self,0x15b0,top);
                    store(self,0x15ac,truncate_float(&candidate));
                }else if(less(compare_integer(static_cast<I>(bottom),&point.y))){
                    store(self,0x15b0,extent_y+top-positioned_height-32u);
                    store(self,0x15ac,truncate_float(&candidate));
                }else{
                    store(self,0x15ac,truncate_float(&point.x)-static_cast<U>(half));
                    store(self,0x15b0,truncate_float(&point.y)-positioned_height-32u);
                }
            }
        }
    }
    const U x_lower=load<U>(self,0x15bc);
    const U x_upper=load<U>(self,0x15c4)-load<U>(self,0x15b4)+x_lower;
    store(self,0x15ac,clamp_original(load<U>(self,0x15ac),x_lower,x_upper));
    const U y_lower=load<U>(self,0x15c0);
    const U y_upper=load<U>(self,0x15c8)+y_lower-load<U>(self,0x15b8);
    store(self,0x15b0,clamp_original(load<U>(self,0x15b0),y_lower,y_upper));
}
