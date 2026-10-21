#pragma once
#include "4512f576a5-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct FactionPorts {virtual ~FactionPorts()=default;virtual void empty(Word)=0;virtual void literal(Word,Word)=0;virtual void assign(Word,Word)=0;virtual void destroy(Word)=0;virtual Word count(Word,Word)=0;virtual void element(Word,Word,Word,Word)=0;virtual Word string(Word,Word,Word,Word,Word,Word)=0;virtual Word dword(Word,Word,Word,Word,Word)=0;virtual Word short_value(Word,Word,Word,Word,Word)=0;virtual Word allocate(Word)=0;virtual void grow_005e03c0(Word,Word)=0;virtual Word compare_005e58c0(Word,Word)=0;virtual void table_string_00413270(Word,Word,Word,Word)=0;};
void faction_reset_005be830(Memory&,FactionPorts&,Word,Word);
Word faction_construct_005bf860(Memory&,FactionPorts&,Word,Word);
void faction_named_0052b2a0(Memory&,FactionPorts&,Word,Word,Word);
Word faction_defaults_0052b490(Memory&,FactionPorts&,Word,Word,Word);
Word faction_load_0052b5c0(Memory&,FactionPorts&,Word,Word,Word,Word);
}
