// Assigned W3 animation name/dispatch bodies. Fixed callees retain their owners.
#include "0b7d27aef1-w3_animation.h"
#include "b6ebbd7b5e-startup_string.h"
#include "7dff9333b0-string_priority.h"
#include "be5af23e96-native_strings.h"
#include "1572bdcf2d-table_contract.h"
#include "cc436f6baa-m15e_animbase.h"
#include "177aadaf45-scene3d.h"
#include "1a441fc83c-vtables_gen.h"
#include <cstring>
#include <new>

extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern std::uint32_t kotor_rules_cell32_007a2174;
std::int32_t __fastcall c2da_current_get_string_entry_00413190(
    c2da_current::Table*,void*,std::int32_t,std::int32_t,kotor_reconstruction::StartupString*);
int __cdecl format_006fadb0(char*,const char*,...);
}
namespace {
using U=std::uint32_t;
using W=std::uint16_t;
using B=unsigned char;
using S=kotor_reconstruction::StartupString;
template<class T> T load(const void* p,U offset){T value;std::memcpy(&value,static_cast<const B*>(p)+offset,sizeof value);return value;}

// These derived +E0 entries return AX. The generic base +E0 declaration retains
// its separate DWORD contract; a call through it would overstate this result.
using Convert=W(__fastcall*)(void*,void*,U);
W client_animation(void* self,U animation){
    return load<Convert>(load<const void*>(self,0),0xe0)(self,nullptr,animation);
}
void get_name_entry(W row,S* value){
    void* const arrays=load<void*>(kotor_cell_rules_007a3a28,0xb8);
    auto* const table=load<c2da_current::Table*>(arrays,0x3c);
    if(c2da_current_get_string_entry_00413190(table,nullptr,row,
        static_cast<std::int32_t>(kotor_rules_cell32_007a2174),value)==0)
        GuiAssign005e5140(value,nullptr,"");
}

// These outputs are constructed by their real callee, with no preceding empty
// constructor. The guard also covers an exception after a hidden result exists.
struct StringResult {
    alignas(S) B bytes[sizeof(S)];
    bool live=false;
    S* get(){return reinterpret_cast<S*>(bytes);}
    ~StringResult() noexcept(false){if(live)get()->~S();}
};
struct HiddenResult {
    S* output;
    bool live=false;
    ~HiddenResult() noexcept(false){if(live)output->~S();}
    void construct(const S& value){::new(static_cast<void*>(output))S(value);live=true;}
    void* release(){live=false;return output;}
};
const char mapping_message[]="CSWCAnimBasePlaceable::ServerToClientAnimation(): Failed to map server anim %i to client anim.";
void unmapped(W animation){
    if(animation>0x270f){char message[256];format_006fadb0(message,mapping_message,static_cast<unsigned>(animation));}
}

// 006E4F7B..4F94: preserve the actual x87 comparison (including masked IE for
// NaNs). Only ordered +/-zero replaces speed with 1.0f; input stays raw DWORD.
U normalize_speed(U bits){
    const U zero=0;
    unsigned short status;
    __asm {
        fld dword ptr bits
        fcomp dword ptr zero
        fnstsw status
    }
    return (status&0x4400u)==0x4000u?0x3f800000u:bits;
}
}
extern "C" {
W __fastcall w3_anim_placeable_convert_006e53f0(void*,void*,U animation){
    const W a=static_cast<W>(animation);
    switch(a){
    case 0x2710:return 0x130;
    case 0x271e:case 0x275d:case 0x27e5:case 0x27e6:case 0x27e7:case 0x27e8:return 0x131;
    case 0x2758:return 0x133;case 0x2759:return 0x134;case 0x275a:return 0x135;
    case 0x275b:return 0x136;case 0x275c:return 0x137;
    case 0x277a:return 0x13c;case 0x277b:return 0x13d;case 0x277c:return 0x13e;
    case 0x277e:return 0x13f;case 0x277f:return 0x140;case 0x2780:return 0x141;
    case 0x2781:return 0x142;case 0x2782:return 0x143;case 0x2783:return 0x144;
    case 0x2784:return 0x145;
    default:unmapped(a);return a;
    }
}
W __fastcall w3_anim_trigger_convert_006e6100(void*,void*,U animation){
    const W a=static_cast<W>(animation);
    if(a==0x279f)return 0x15b;
    if(a==0x27a0)return 0x15e;
    unmapped(a);return a;
}
void* __fastcall w3_anim_trigger_name_006e6040(void* self,void*,void* output,U animation){
    const W row=client_animation(self,animation);
    HiddenResult result{static_cast<S*>(output)};
    {
        S value;
        get_name_entry(row,&value);
        result.construct(value);
    }
    return result.release();
}
void* __fastcall w3_anim_placeable_name_006e51a0(void* self,void*,void* output,U animation){
    const W row=client_animation(self,animation);
    HiddenResult result{static_cast<S*>(output)};
    {
        S value;
        get_name_entry(row,&value);
        if(value.BorrowedLength005e5790()<=0){
            const W a=static_cast<W>(animation);
            if(a<0x3e8 || a>0x6bf)GuiAssign005e5140(&value,nullptr,"default");
            else{
                S suffix;
                int index=-1;
                if(a<=0x467)index=a-0x3e8;
                else if(a>=0x4b0 && a<=0x52f){GuiAssign005e5140(&suffix,nullptr,"w");index=a-0x4b0;}
                else if(a>=0x578 && a<=0x5f7){GuiAssign005e5140(&suffix,nullptr,"l");index=a-0x578;}
                else if(a>=0x640){GuiAssign005e5140(&suffix,nullptr,"wl");index=a-0x640;}
                if(++index!=0){
                    char number[4];
                    format_006fadb0(number,"%03d",index);
                    S digits(number);
                    S cut("cut");
                    StringResult first;
                    GuiConcat005e5d10(&cut,nullptr,first.get(),&digits);first.live=true;
                    StringResult second;
                    GuiConcat005e5d10(first.get(),nullptr,second.get(),&suffix);second.live=true;
                    value.Assign005e5c50(second.get());
                }
            }
        }
        result.construct(value);
    }
    return result.release();
}
U __fastcall w3_anim_placeable_set_006e4f30(void* self,void*,U animation,U speed,U,U transition){
    if(load<void*>(self,0xb8)==nullptr)return 0;
    StringResult name;
    const auto* const methods=load<const KotorM15eAnimBaseVtbl*>(self,0);
    methods->s040(self,nullptr,name.get(),animation);name.live=true;
    speed=normalize_speed(speed);
    const W a=static_cast<W>(animation);
    const bool special=a==0x2710 || (a>=0x2758 && a<=0x275c);
    const bool looping=a==0x132 || (a>=0x138 && a<=0x13b) || a==0x271e ||
        (a>=0x3e8 && a<=0x52f) || a==0x275d || (a>=0x27e5 && a<=0x27e8);
    const bool once=(a>=0x578 && a<=0x6bf) || (a>=0x277a && a<=0x277c) || (a>=0x277e && a<=0x2784);
    auto* const gob=load<aurora_model::Gob*>(self,0xb8);
    const auto* const gob_methods=load<const aurora_model::GobVTable*>(gob,0);
    if(!special && !looping && !once){
        gob_methods->s018_gob_play_animation(gob,nullptr,reinterpret_cast<U>(""),0x3f800000u,0,transition);
    }else{
        const char* const text=WindowCStr005e5670(name.get(),nullptr);
        gob_methods->s018_gob_play_animation(load<aurora_model::Gob*>(self,0xb8),nullptr,
            reinterpret_cast<U>(text),speed,looping?1u:0u,transition);
    }
    return special?0u:1u;
}
}
