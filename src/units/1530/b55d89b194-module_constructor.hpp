#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleConstructorPorts {
 virtual ~ModuleConstructorPorts()=default;
 virtual Word vtable(Word)=0;
 virtual void write_byte(Word,Word)=0;
 virtual void list_00405ed0(Word)=0;
 virtual Word list_insert_00406290(Word,Word,Word)=0;
 virtual void object_004d7d60(Word,Word,Word)=0;
 virtual void table_004c4e20(Word)=0;
 virtual void linked_005e9e10(Word)=0;
 virtual void empty_005b3190(Word)=0;
 virtual void location_0059afe0(Word)=0;
 virtual void strings_006fb55f(Word,Word,Word,Word,Word)=0;
 virtual Word allocate_006fa7e6(Word)=0;
 virtual Word map_00578be0(Word)=0;
 virtual Word literal_005e5a90(Word,Word)=0;
 virtual Word concat_005e5d10(Word,Word,Word)=0;
 virtual Word assign_005e5c50(Word,Word)=0;
 virtual void destroy_005e5c20(Word)=0;
 virtual void set_literal_005e5140(Word,Word)=0;
 virtual void set_name_004c4150(Word,Word)=0;
 virtual void resref_00406d80(Word,Word)=0;
 virtual void event_004c4cc0(Word,Word,Word)=0;
 virtual Word array_004aed70(Word)=0;
 virtual void register_004d80e0(Word,Word,Word,Word)=0;
 virtual Word timer_004aede0(Word)=0;
 virtual void time_004ade40(Word,Word,Word)=0;
};
Word module_constructor_004c84a0(Memory&,ModuleConstructorPorts&,Word,Word,Word,Word,Word);
}
