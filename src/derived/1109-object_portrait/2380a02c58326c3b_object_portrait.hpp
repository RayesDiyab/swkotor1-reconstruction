#pragma once
#include "d0c22ee5c6928509_creature_portrait.hpp"
namespace kotor::server_mainloop {
struct ObjectPortraitPorts:CreaturePortraitPorts{virtual Word concat(Word,Word,Word)=0;};
void object_portrait_004cad20(Memory&,ObjectPortraitPorts&,Word,Word,Word);
void object_portrait_ref_004d01f0(CreaturePortraitRefPorts&,Word,Word,Word);
}
