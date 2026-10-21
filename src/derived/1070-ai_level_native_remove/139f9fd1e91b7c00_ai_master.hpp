#pragma once
#include "e5699fe00480fd44_memory.hpp"
namespace kotor::ai_master {
using character_load::Memory;
using character_load::Word;
struct Ports {
    virtual ~Ports() = default;
    virtual void grow_005e03c0(Word list, Word capacity) = 0;
    virtual void remove_index_006857a0(Word list, Word index) = 0;
};
void add_unique_006449d0(Memory&,Ports&,Word list,Word id);
Word remove_id_004af330(Memory&,Ports&,Word list,Word id);
Word remove_object_004af3d0(Memory&,Ports&,Word master,Word object);
Word add_object_004b0850(Memory&,Ports&,Word master,Word object,Word level);
Word set_level_004b08a0(Memory&,Ports&,Word master,Word object,Word level);
}
