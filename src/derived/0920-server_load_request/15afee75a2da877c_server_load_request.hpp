#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
// All objects/strings below are guest handles. Existing source owners remain
// authoritative; this interface does not supply filesystem, party or services.
struct LoadPorts {
 virtual ~LoadPorts()=default;
 virtual Word module_004b14f0(Word self)=0;
 virtual void unload_004b9240(Word self)=0; // existing held trap, never substituted
 virtual void empty_005b3190(Word out)=0;
 virtual void literal_005e5a90(Word out,Word text)=0;
 virtual void set_literal_005e5140(Word out,Word text)=0;
 virtual void assign_005e5c50(Word out,Word in)=0;
 virtual void destroy_005e5c20(Word string)=0;
 virtual Word concat_005e5d10(Word left,Word out,Word right)=0;
 virtual Word right_005e6170(Word in,Word out,Word count)=0;
 virtual Word find_005e5eb0(Word in,Word needle,Word start)=0;
 virtual Word length_005e5790(Word in)=0;
 virtual Word buffer_005e5670(Word in)=0;
 virtual void format_005e5680(Word out,Word format,Word number)=0;
 virtual void timestamp_005e9410(Word debug,Word out)=0;
 virtual Word directory_exists_004068c0(Word manager,Word name)=0;
 virtual void create_directory_00409460(Word manager,Word name,Word a,Word b)=0;
 virtual Word include_save_004b20e0(Word self,Word name)=0;
 virtual Word party_004aee70(Word server)=0;
 virtual void add_game_005638d0(Word party)=0;
 virtual void remove_game_00563950(Word party)=0;
 virtual void resref_00406d60(Word out,Word name)=0;
 virtual Word resource_exists_00408bc0(Word manager,Word resref,Word type,Word flag)=0;
 virtual Word resolve_005e68a0(Word aliases,Word out,Word name,Word type)=0;
 virtual void estimate_004b5f90(Word self,Word name,Word type)=0;
 virtual void add_directory_00408800(Word manager,Word name)=0;
 virtual void remove_directory_004088d0(Word manager,Word name)=0;
 virtual Word alias_005e6890(Word aliases,Word name,Word index)=0;
 virtual Word free_space_004067b0(Word manager,Word name,Word output64)=0;
 virtual Word allocate_006fa7e6(Word size)=0;
 virtual Word file_005e68d0(Word allocation,Word name,Word type,Word mode)=0;
 virtual Word file_size_005e6950(Word file)=0;
 virtual void file_destroy_005e69e0(Word file)=0;
 virtual void free_006fa390(Word allocation)=0;
 virtual void load_reply_0056cb80(Word messages,Word player,Word one,Word reason)=0;
 virtual Word messages_004aedb0(Word server)=0;
 virtual void stall_0056c8b0(Word messages,Word kind)=0;
};
// scratch is a disjoint writable mapped 0xE8-byte local region. Every original
// branch is represented; native SEH and fault cleanup remain outside this host
// projection. All provider calls must complete normally for this contract.
Word load_module_004b95b0(Memory&,LoadPorts&,Word self,Word name,Word player,Word scratch);
}

