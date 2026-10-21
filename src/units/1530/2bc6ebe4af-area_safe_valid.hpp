#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaSafeValidPorts{virtual ~AreaSafeValidPorts()=default;
 virtual void bounds(Word,Word,Word)=0;virtual Word room(Word,Word,Word)=0;
 virtual void mesh(Word,Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;
 virtual Word sweep(Word,Word,Word,Word,Word,Word,Word)=0;
 virtual bool near(Word,Word,Word,Word)=0;virtual Word adjacent(Word,Word)=0;virtual bool equal(Word,Word)=0;
 virtual Word region(Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;
 virtual Word collision(Word,Word,Word,Word,Word,Word,Word,Word,Word)=0;
};
Word area_safe_valid_004be5e0(Memory&,AreaSafeValidPorts&,Word,Word,Word,Word);
}
