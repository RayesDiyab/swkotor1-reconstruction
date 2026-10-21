#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleResourcePorts {
 virtual ~ModuleResourcePorts()=default;
 virtual void write_byte(Word,Word)=0;
 virtual void empty_005b3190(Word)=0;
 virtual Word progress_part_005edff0(Word,Word)=0;
 virtual void notify_name_0063e7c0(Word,Word)=0;
 virtual void literal_005e5a90(Word,Word)=0;
 virtual void add_004087a0(Word,Word,Word)=0;
 virtual void destroy_005e5c20(Word)=0;
 virtual void progress_005edd40(Word,Word,Word)=0;
 virtual void pump_00401c10(Word,Word,Word)=0;
 virtual Word find_005e5f20(Word,Word,Word)=0;
 virtual Word concat_005e5d10(Word,Word,Word)=0;
 virtual void assign_005e5c50(Word,Word)=0;
 virtual Word length_005e5790(Word)=0;
 virtual Word right_005e6170(Word,Word,Word)=0;
 virtual void prepare_00408800(Word,Word)=0;
 virtual void resref_00406d60(Word,Word)=0;
 virtual Word exists_00408bc0(Word,Word,Word,Word)=0;
 virtual void finalize_004088d0(Word,Word)=0;
 virtual Word cstr_005e5670(Word)=0;
 virtual void format_005e5680(Word,Word,Word)=0;
 virtual void mount_004064f0(Word,Word,Word,Word)=0;
 virtual void sleep_0073d1e4(Word)=0;
 virtual void result_005ee0a0(Word,Word)=0;
};
void module_resources_004c4150(Memory&,ModuleResourcePorts&,Word,Word,Word);
}
