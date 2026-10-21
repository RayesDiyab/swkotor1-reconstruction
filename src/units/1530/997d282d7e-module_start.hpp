#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
// Partial restoration: prefix only. Ready means continue at004C919B, not module loaded.
struct ModuleStartPorts {
 virtual ~ModuleStartPorts()=default;
 virtual void empty_005b3190(Word)=0;
 virtual void manager_begin_004070a0(Word)=0;
 virtual Word objects_004aed70(Word)=0;
 virtual Word literal_005e5a90(Word,Word)=0;
 virtual Word concat_005e5d10(Word,Word,Word)=0;
 virtual void assign_005e5c50(Word,Word)=0;
 virtual void destroy_005e5c20(Word)=0;
 virtual Word compare_005e5310(Word,Word)=0;
 virtual Word length_005e5790(Word)=0;
 virtual void release_path_004c44d0(Word,Word)=0;
 virtual void setup_004c4150(Word,Word)=0;
 virtual Word resource_ready_00409b10(Word)=0;
};
enum class ModulePrefixResult { ContinueAt004c919b, OriginalReturn1 };
ModulePrefixResult module_start_prefix_004c9050(Memory&,ModuleStartPorts&,Word,Word,Word);
}
