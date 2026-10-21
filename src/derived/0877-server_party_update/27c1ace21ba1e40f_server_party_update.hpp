#pragma once
#include "41107dc43edc9ed0_server_party.hpp"
namespace kotor::server_mainloop {
struct PartyUpdatePorts {
 virtual ~PartyUpdatePorts()=default;
 virtual Word cinematic_005edc60(Word)=0;
 virtual Word busy_005edad0(Word)=0;
 virtual Wide high_time_005e9260(Word)=0;
 virtual Wide divide_006fb030(Wide,Wide)=0;
 virtual Word party_005ed8b0(Word)=0;
 virtual Word character_006346c0(Word,Word)=0;
 virtual Word creature_0060fb20(Word)=0;
 virtual Word incapacitated_004ef890(Word)=0;
 virtual void all_down_005edc40(Word)=0;
 virtual Word area_004cb120(Word)=0;
 virtual Word object_array_004aed70(Word)=0;
 virtual Word first_005071b0(Word,Word)=0;
 virtual Word next_005071e0(Word,Word)=0;
 virtual Word lookup_004d8230(Word,Word,Word)=0;
 virtual Word virtual_creature_30(Word)=0;
 virtual Word relationship_0057cb80(Word,Word,Word)=0;
 virtual Word hostile_004f7650(Word)=0;
 virtual Word safe_position_004aeb60(Word,Word,Word,Word,Word,Word)=0;
 virtual void set_position_004cd1a0(Word,Word,Word,Word,Word)=0;
 virtual Word allocate_006fa7e6(Word)=0;
 virtual Word effect_ctor_00503e40(Word,Word)=0;
 virtual void apply_effect_004d14f0(Word,Word,Word,Word)=0;
 virtual Word party_table_004aee70(Word)=0;
 // Native provider must retain original x87 operation order/control word.
 virtual bool distance_exceeds_004b7120(Word,Word)=0;
};
void party_update_004b6da0(Memory&,PartyUpdatePorts&,Word,Word);
}
