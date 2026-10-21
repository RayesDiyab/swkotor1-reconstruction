#include "2d1a6b5349-door_contracts.h"
namespace {
using namespace door;
struct alignas(4) AnimationSlot {H low;B untouched[2];};
static_assert(sizeof(AnimationSlot)==4,"Original four-byte animation slot, only low word written");
U native_word(const AnimationSlot& value){
    const void* address=&value;U result;
    __asm {mov eax,address
        mov eax,dword ptr[eax]
        mov result,eax
    }
    return result;
}
U float_word(const float& value){U result;std::memcpy(&result,&value,4);return result;}
void animation_sound(void* self,AnimationSlot& slot){
    kotor_reconstruction::StartupString sound;
    const U door_type=get<B>(self,0x124);
    void* tables=ptr(kotor_cell_rules_007a3a28,0xb8);
    std::int32_t row;
    if(door_type==0){
        const U generic=get<B>(self,0x125);
        (void)c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(ptr(tables,0x28)),nullptr,
            static_cast<I>(generic),static_cast<I>(kotor_rules_cell32_007a2250),&row);
    }else{
        (void)c2da_current_get_int_entry_00413510(static_cast<c2da_current::Table*>(ptr(tables,0x24)),nullptr,
            static_cast<I>(door_type),static_cast<I>(kotor_rules_cell32_007a224c),&row);
    }
    const H animation=static_cast<H>(native_word(slot));
    const U column=(animation==0x14e||animation==0x14f)?kotor_rules_cell32_007a2268:
        (animation==0x150||animation==0x151)?kotor_rules_cell32_007a226c:kotor_rules_cell32_007a2270;
    (void)c2da_current_get_string_entry_00413190(static_cast<c2da_current::Table*>(ptr(ptr(kotor_cell_rules_007a3a28,0xb8),0x70)),nullptr,
        row,static_cast<I>(column),&sound);
    menu_resources::Ref reference;
    (void)async_ref_construct_string_00406d60(&reference,nullptr,&sound);
    KotorM15VectorWords position;
    std::memcpy(&position,at(self,0x24),sizeof position);
    (void)kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec,nullptr,&reference,
        position,1.5f,0x16,0,0,0.0f,0.0f);
}
}
extern "C" void __fastcall w3_door_animation_00684030(void* self,void*){
    using namespace door;
    if(kotor_fastcall_0063c380(self,nullptr,get<H>(self,0x48))==1){
        const U remaining=get<U>(self,0x64)-get<U>(self,0x44);
        put<U>(self,0x64,remaining);
        if(remaining&0x80000000u){
            const auto* methods=object_table(self);
            // Original getter is MOV AX,[ECX+58h]; it retains the negative
            // subtraction's upper EAX word before the caller pushes all32 bits.
            const U animation=(remaining&0xffff0000u)|kotor_m15a_object_get_looping_animation_0063c100(self,nullptr);
            (void)methods->s09c_set_animation(self,nullptr,animation,0,0);
            put<U>(self,0x64,0);
        }
    }
    if(kotor_m15a_object_animation_looping_0063c300(self,nullptr,get<H>(self,0x48))!=1)return;
    // Original leaves the upper half of this native slot unwritten. Never read
    // it as a C++ indeterminate value or invent a zero high word.
    AnimationSlot slot;
    float speed;U duration,data;
    if(kotor_fastcall_0063c610(self,nullptr,&slot.low,&speed,&duration,&data)==1){
        const U animation=native_word(slot);
        const U speed_word=float_word(speed);
        void* model=ptr(self,0x68);
        put<H>(self,0x48,static_cast<H>(animation));
        (void)animation_table(model)->s054(model,nullptr,animation,speed_word,1,0);
        put<U>(self,0x64,duration);
        kotor_fastcall_0063c9c0(self,nullptr);
        const H current=static_cast<H>(native_word(slot));
        if(current==0x14e||current==0x14f||current==0x150||current==0x151||current==0x149)
            animation_sound(self,slot);
    }
    const H current=get<H>(self,0x48);
    if(current==0x2726||current==0x2742||current==0x2743||current==0x2758){
        void* model=object_table(self)->s098_get_model(self,nullptr,0xff,1);
        const B state=current==0x2726?0:current==0x2742?1:current==0x2743?2:3;
        put<B>(self,0x11c,state);
        if(model)(void)gob_table(model)->s090_gob_set_ignore_hit_check(gob(model),nullptr,state?1u:0u);
    }
    if(data)live_free(pointer(data));
}
