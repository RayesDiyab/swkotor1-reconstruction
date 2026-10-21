#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct VectorWords { Word x,y,z; };
struct PartyPositionPorts {
 virtual ~PartyPositionPorts()=default;
 virtual Word party_005ed8b0(Word)=0;
 virtual Word module_004b14f0(Word)=0;
 virtual Word character_006346c0(Word,Word)=0;
 virtual void reveal_005792d0(Word,VectorWords,Word)=0;
 virtual void map_position_005790c0(Word,Word,VectorWords)=0;
 virtual void map_orientation_00579130(Word,Word,VectorWords)=0;
};
void party_positions_004b4e80(Memory&,PartyPositionPorts&,Word);
}
