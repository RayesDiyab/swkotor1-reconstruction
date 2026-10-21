#include "0aec0b9933-menu_row_common.h"
using namespace menu_row;
extern "C" U __fastcall kotor_fastcall_00616140(void* self,void*,U feat1,U feat2,U feat3) {
    void* creature=server_creature(self);
    void* stats=creature!=nullptr?load<void*>(creature,0xA74):nullptr;
    if(stats!=nullptr){
        if(recovered_stats_has_feat_005a6630(stats,nullptr,feat1)!=0)return feat1;
        if(recovered_stats_has_feat_005a6630(load<void*>(creature,0xA74),nullptr,feat2)!=0)return feat2;
        if(recovered_stats_has_feat_005a6630(load<void*>(creature,0xA74),nullptr,feat3)!=0)return feat3;
    }else{
        if(kotor_m15b_creature_stats_has_feat_00649340(load<void*>(self,0x2F8),nullptr,static_cast<std::uint16_t>(feat1))!=0)return feat1;
        if(kotor_m15b_creature_stats_has_feat_00649340(load<void*>(self,0x2F8),nullptr,static_cast<std::uint16_t>(feat2))!=0)return feat2;
        if(kotor_m15b_creature_stats_has_feat_00649340(load<void*>(self,0x2F8),nullptr,static_cast<std::uint16_t>(feat3))!=0)return feat3;
    }
    return 0xFFFFFFFFu;
}
extern "C" U __fastcall kotor_fastcall_00616520(void* self,void*,void* item,U,U flags) {
    if(item==nullptr)return 0;
    void* creature=server_creature(self);
    void* area=frame_GetServerArea_00605640(load<void*>(self,0x20),nullptr);
    volatile U in_minigame=load<U>(area,0x2B0); // Original dereferences before testing creature.
    if(creature==nullptr)return 0;
    const auto can_use=[&](){return recovered_creature_can_use_item_0051b440(creature,nullptr,item,1);};
    void* property=item; // Original output scratch aliases the incoming item word.
    const auto query=[&](U kind){return recovered_item_property_query_005539c0(item,nullptr,&property,kind,0);};
    const auto skill_item=[&](){
        if(load<std::uint8_t>(property,0xC)==0)return U{0};
        if(recovered_stats_can_use_skill_005af880(load<void*>(creature,0xA74),nullptr,1)==0)return U{0};
        return can_use();
    };
    const auto spell_item=[&](std::uint8_t require_flag){
        void* base=kotor_m15b_item_get_base_item_005b4790(item,nullptr);
        if(base!=nullptr){
            const auto kind=load<std::uint8_t>(base,0xAC);
            if(kind==0xC||kind==0xD||kind==0x14){
                if(recovered_inventory_has_item_005a4950(load<void*>(creature,0xA2C),nullptr,item)==0)return U{0};
            }
            if(require_flag==0)return U{0};
        }
        const auto spell_id=load<std::uint16_t>(property,2);
        void* spell=kotor_m15b_spell_array_get_spell_0059b6d0(load<void*>(kotor_cell_rules_007a3a28,0x8C),nullptr,static_cast<I>(spell_id));
        if(spell==nullptr||load<std::uint8_t>(spell,0x194)!=1)return U{0};
        return can_use();
    };
    const auto flag_byte=static_cast<std::uint8_t>(flags);
    if(load<U>(item,0x108)!=0){
        if((flag_byte&2)!=0&&load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC)==0x1C){
            if(query(0x2E)==0)return can_use();
            return skill_item();
        }
        if(query(0xA)==0)return 0;
        return spell_item(static_cast<std::uint8_t>(flag_byte&4));
    }
    if(in_minigame!=0)return 0;
    if((flag_byte&1)!=0){
        // Each comparison intentionally calls GetBaseItem again.
        if(load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC)==0x2D||
           load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC)==0x2F||
           load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC)==0x1A){
            if(query(0xA)==0)return can_use();
            return load<std::uint8_t>(property,0xC)!=0?can_use():0;
        }
    }
    const auto flag4=static_cast<std::uint8_t>(flag_byte&4);
    if(flag4!=0&&load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC)==0x19){
        if(query(0xA)==0)return can_use();
        return load<std::uint8_t>(property,0xC)!=0?can_use():0;
    }
    if((flag_byte&2)!=0&&load<std::uint8_t>(kotor_m15b_item_get_base_item_005b4790(item,nullptr),0xAC)==0x1C){
        if(query(0x2E)==0)return can_use();
        return skill_item();
    }
    if(query(0xA)==0||load<std::uint8_t>(property,0xC)==0)return 0;
    return spell_item(flag4);
}
