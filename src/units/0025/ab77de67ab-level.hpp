#pragma once
#include "678848ba6f-memory16.hpp"
namespace kotor::character_level_stats {
using character_load::Word;
using LevelMemory=character_load::Memory16;
struct Dependencies {
    virtual ~Dependencies()=default;
    virtual Word rules_007a3a28()=0;
    virtual Word allocate_006fa7e6(Word bytes)=0;
    virtual void vector_construct_006fb55f(Word first,Word stride,Word count,Word ctor,Word dtor)=0;
    virtual void grow_u16_004d2fa0(Word list,Word capacity_bits)=0;
};
Word construct_005cb1b0(LevelMemory&,Dependencies&,Word self);
std::uint8_t get_skill_005cb0f0(LevelMemory&,Dependencies&,Word self,Word skill_bits);
void set_skill_005cb120(LevelMemory&,Dependencies&,Word self,Word skill_bits,Word change_bits);
void add_feat_005cb150(LevelMemory&,Dependencies&,Word self,Word feat_bits);
}
