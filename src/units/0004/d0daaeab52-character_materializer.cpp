// Original00561D70 /00560E60 only. Reuses all game-heap, GFF, creature, W1 and W2 providers.
#include "836d6b8cce-character_materializer.hpp"
#include "f88d6b3206-assignment.h" // canonical resource_ref_assignment::assign_00406120
#include "abb3810cb7-gff_views.h" // canonical CResGFF and 4-byte StructHandle
#include <cstdint>
#include <cstring>
#if !defined(_M_IX86)
#error Character materializer requires the original Windows x86 ABI
#endif
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __cdecl live_free(void*);
__declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long,const char*);
void* __fastcall recovered_server_creature_construct_004f7a10(void*,void*,unsigned,unsigned);
void __fastcall recovered_set_game_object_00561c40(void*,void*,void*);
void __fastcall recovered_set_in_party_004fdb20(void*,void*,unsigned,unsigned);
kotor_q05::CResGFF* __fastcall gui_load_gff_construct_00410630(kotor_q05::CResGFF*,void*,unsigned,const char*,const menu_resources::Ref*);
void __fastcall c2da_resource_serviced_00409b40(void*,void*,unsigned);
unsigned __fastcall w2x_creature_stats_005afce0(void*,void*,void*,void*,void*);
void __fastcall w1x_object_set_hp_004cc280(void*,void*,unsigned);
unsigned char __fastcall gui_load_gff_byte_00411a60(void*,void*,void*,const char*,unsigned*,unsigned char);
void __fastcall recovered_set_activity_004f00a0(void*,void*,unsigned,unsigned,unsigned);
int __fastcall gui_load_gff_int_00411c90(void*,void*,void*,const char*,unsigned*,int);
unsigned short __fastcall recovered_gff_word_00411b40(void*,void*,void*,const char*,unsigned*,unsigned short);
void __fastcall recovered_creature_scripts_004ebf20(void*,void*,void*,void*);
void __fastcall recovered_creature_inventory_004ffda0(void*,void*,void*,void*,unsigned,unsigned,unsigned);
void __fastcall w2x_known_spells_005aeb30(void*,void*,void*,void*);
void __fastcall w2x_template_finish_004f1c40(void*,void*);
}
namespace {
unsigned char* at(void* p,unsigned offset){return static_cast<unsigned char*>(p)+offset;}
template<class T> T read(void* p,unsigned offset){T value;std::memcpy(&value,at(p,offset),sizeof(value));return value;}
template<class T> void write(void* p,unsigned offset,T value){std::memcpy(at(p,offset),&value,sizeof(value));}
void destroy_scalar(void* p){
    using Destroy=void*(__fastcall*)(void*,void*,unsigned);
    const auto entry=read<Destroy>(read<void*>(p,0),0);
    (void)entry(p,nullptr,1);
}
void release_gff(void* gff){c2da_resource_serviced_00409b40(gff,nullptr,0);destroy_scalar(gff);}
}
extern "C" unsigned __fastcall recovered_player_deserialize_bic_00560e60(void* player,void*,menu_resources::Ref name,void* creature){
    void* raw=live_operator_new(0xa0);
    kotor_q05::CResGFF* gff=nullptr;
    if(raw){
        // Own original unwind0071F1E0 frees only an allocation whose ctor throws.
        try{gff=gui_load_gff_construct_00410630(static_cast<kotor_q05::CResGFF*>(raw),nullptr,0x7df,"BIC ",&name);}
        catch(...){live_free(raw);throw;}
    }
    // Original quirk: absent/unserviced BIC returns the same zero as normal success.
    const unsigned serviced94=read<unsigned>(gff,0x94);
    if(!serviced94){
        release_gff(gff);
        return 0;
    }
    menu_resources::Ref assignment_result;
    (void)resource_ref_assignment::assign_00406120(reinterpret_cast<menu_resources::Ref*>(at(player,0x65)),nullptr,&assignment_result,&name);
    kotor_q05::StructHandle root;
    (void)gff->GetTopLevelStruct(&root);
    void* stats=read<void*>(creature,0xa74);
    const unsigned status=w2x_creature_stats_005afce0(stats,nullptr,gff,&root,at(creature,0xa50));
    if(status){
        release_gff(gff);
        return status;
    }
    using MaxHP=unsigned(__fastcall*)(void*,void*,unsigned);
    const auto max_hp=read<MaxHP>(read<void*>(creature,0),0x98);
    const std::uint16_t low=static_cast<std::uint16_t>(max_hp(creature,nullptr,1));
    std::int16_t signed_hp;std::memcpy(&signed_hp,&low,sizeof(signed_hp));
    w1x_object_set_hp_004cc280(creature,nullptr,static_cast<unsigned>(static_cast<int>(signed_hp)));
    unsigned field_status;
    (void)gui_load_gff_byte_00411a60(gff,nullptr,&root,"DetectMode",&field_status,0);
    recovered_set_activity_004f00a0(creature,nullptr,2,1,0);
    const unsigned stealth=gui_load_gff_byte_00411a60(gff,nullptr,&root,"StealthMode",&field_status,0);
    recovered_set_activity_004f00a0(creature,nullptr,1,stealth,0);
    const int size=gui_load_gff_int_00411c90(gff,nullptr,&root,"CreatureSize",&field_status,3);
    write<int>(creature,0x4f8,size);
    const unsigned disguise=gui_load_gff_byte_00411a60(gff,nullptr,&root,"PM_IsDisguised",&field_status,0);
    write<unsigned>(creature,0xa48,disguise);
    if(disguise){const auto appearance=recovered_gff_word_00411b40(gff,nullptr,&root,"PM_Appearance",&field_status,0);write<unsigned short>(creature,0xa4c,appearance);}
    recovered_creature_scripts_004ebf20(creature,nullptr,gff,&root);
    recovered_creature_inventory_004ffda0(creature,nullptr,gff,&root,0,1,0);
    w2x_known_spells_005aeb30(read<void*>(creature,0xa74),nullptr,gff,&root);
    w2x_template_finish_004f1c40(creature,nullptr);
    release_gff(gff);
    return 0;
}
extern "C" unsigned __fastcall recovered_player_load_bic_00561d70(void* player,void*){
    void* raw=live_operator_new(0xac8);
    void* creature=nullptr;
    if(raw){
        // Own original unwind0071F1A0 frees only an allocation whose ctor throws.
        try{creature=recovered_server_creature_construct_004f7a10(raw,nullptr,0x7f000000u,1);}
        catch(...){live_free(raw);throw;}
    }
    recovered_set_game_object_00561c40(player,nullptr,creature);
    const unsigned id=read<unsigned>(creature,4);
    write<unsigned>(player,0x38,id);
    write<unsigned char>(player,0x64,1);
    menu_resources::Ref name;
    (void)name.construct_00406d80("TEMP_CHAR");
    const unsigned status=recovered_player_deserialize_bic_00560e60(player,nullptr,name,creature);
    // Intentionally before testing loader status, as at00561DF1..00561DFE.
    recovered_set_in_party_004fdb20(creature,nullptr,1,1);
    if(status){
        recovered_set_game_object_00561c40(player,nullptr,nullptr);
        write<unsigned>(player,0x38,0x7f000000u);
        destroy_scalar(creature);
        // Original retains player+64==1 on this error path.
    }
    return status;
}


static_assert(sizeof(void*)==4 && sizeof(kotor_q05::StructHandle)==4,"original x86 frame sizes");
