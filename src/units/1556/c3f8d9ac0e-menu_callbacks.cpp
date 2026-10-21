#include "50a5e32a47-menu8_imports.h"
#include <cstring>

namespace {
using U=std::uint32_t;
template<class T> T load(const void* p,U offset) {
    T value; std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof value); return value;
}
void* client() { return load<void*>(kotor_menu_window_cells::manager_007a39fc,4); }
}

extern "C" void __fastcall kotor_fastcall_0060f330(void* self,void*,U action,void* creature) {
    const U masked_action=action&0xdfffffffu;
    void* const message=kotor_fastcall_005ed6f0(client(),nullptr);
    if(message==nullptr)return;
    void* const stats=load<void*>(creature,0x2f8);
    std::uint8_t slot=0;
    U working_action=action;
    for(;;) {
        working_action=(working_action&0xffffff00u)|slot;
        if(kotor_m15b_creature_stats_get_class_00647730(stats,nullptr,slot)==3)break;
        if(kotor_m15b_creature_stats_get_class_00647730(stats,nullptr,slot)==4)break;
        if(kotor_m15b_creature_stats_get_class_00647730(stats,nullptr,slot)==5)break;
        ++slot;
        working_action=(working_action&0xffffff00u)|slot;
        if(slot>=2) {
            if(load<std::uint16_t>(stats,0x24)==5)
                (void)kotor_fastcall_00677bd0(message,nullptr,masked_action,0,0,
                    load<U>(self,4),static_cast<unsigned char*>(self)+0x24);
            return;
        }
    }
    std::int32_t found;
    {
        kotor_reconstruction::StartupString column("FORCEHOSTILE");
        auto* const table=static_cast<c2da_current::Table*>(load<void*>(
            load<void*>(kotor_cell_rules_007a3a28,0xb8),0xf4));
        std::int32_t unused_value;
        found=c2da_current_get_int_00413660(table,nullptr,
            static_cast<std::int32_t>(masked_action),&column,&unused_value);
    }
    void* const tutorial_manager=kotor_menu_window_cells::manager_007a39fc;
    const U tutorial_id=load<U>(self,4);
    void* const tutorial_client=load<void*>(tutorial_manager,4);
    const int result=show_tutorial_window_005edf40(tutorial_client,nullptr,
        found!=0?4u:3u,tutorial_id,slot,masked_action);
    if(result!=0)return;
    const U object_id=load<U>(self,4);
    const U zero=static_cast<U>(result);
    const U position[3]={zero,zero,zero};
    void* const second_message=kotor_fastcall_005ed6f0(client(),nullptr);
    if(second_message!=nullptr)
        (void)kotor_fastcall_006776a0(second_message,nullptr,masked_action,
            working_action,0,0,0,position,object_id);
}

extern "C" void __fastcall kotor_fastcall_0060f590(void* self,void*,U action,void* creature) {
    const U creature_id=load<U>(creature,4);
    const U self_id=load<U>(self,4);
    const U masked_action=action&0xbfffffffu;
    if(creature_id!=self_id) {
        void* const message=kotor_fastcall_005ed6f0(client(),nullptr);
        if(message!=nullptr)
            (void)kotor_fastcall_00677bd0(message,nullptr,masked_action,0,0,
                0x7f000000u,static_cast<unsigned char*>(self)+0x24);
    } else {
        const U position[3]={0,0,0};
        void* const message=kotor_fastcall_005ed6f0(client(),nullptr);
        if(message!=nullptr)
            (void)kotor_fastcall_00677bd0(message,nullptr,masked_action,0,0,self_id,position);
    }
}

extern "C" void __fastcall kotor_fastcall_00616200(void* self,void*,U action,void* creature) {
    if(creature!=nullptr)kotor_fastcall_00610a10(creature,nullptr,1);
    kotor_fastcall_0060f330(self,nullptr,action,creature);
}

extern "C" void __fastcall kotor_fastcall_00617dd0(void* self,void*,U action,void* creature) {
    if(kotor_rules::kotor_fastcall_00550c00(kotor_cell_rules_007a3a28,nullptr,
            static_cast<std::uint16_t>(action))==nullptr)return;
    const U tutorial_id=load<U>(self,4);
    if(show_tutorial_window_005edf40(client(),nullptr,0,tutorial_id,
            0x7f000000u,static_cast<std::uint16_t>(action))!=0)return;
    void* const message=kotor_fastcall_005ed6f0(client(),nullptr);
    if(message!=nullptr)
        (void)kotor_fastcall_00677a90(message,nullptr,load<U>(self,4),action);
    if(creature!=nullptr)kotor_fastcall_00610a10(creature,nullptr,1);
}

extern "C" void __fastcall kotor_fastcall_00617e40(void* self,void*,U item_id,void* creature) {
    void* const server=load<void*>(kotor_menu_window_cells::manager_007a39fc,8);
    void* const item=recovered_item_lookup_004ae760(server,nullptr,item_id);
    if(item!=nullptr) {
        void* const base=kotor_m15b_item_get_base_item_005b4790(item,nullptr);
        if(load<std::uint8_t>(base,0xac)==6) {
            const U creature_id=load<U>(creature,4);
            const U self_id=load<U>(self,4);
            if(show_tutorial_window_005edf40(client(),nullptr,1,self_id,creature_id,item_id)!=0)return;
        }
    }
    if(creature!=nullptr)kotor_fastcall_00610a10(creature,nullptr,1);
    kotor_fastcall_0060f590(self,nullptr,item_id,creature);
}
