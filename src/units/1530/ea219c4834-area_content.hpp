#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct AreaContentPorts{virtual ~AreaContentPorts()=default;
virtual Word allocate(Word)=0;virtual Word layout_construct_005de770(Word)=0;virtual Word layout_load_005df140(Word,Word)=0;virtual Word layout_count_005a4c50(Word)=0;virtual Word layout_extract_00504870(Word)=0;virtual void layout_release_005de450(Word)=0;virtual void destroy(Word,Word)=0;
virtual Word exists_00408bc0(Word,Word,Word,Word)=0;virtual Word gff_construct_00410630(Word,Word,Word,Word)=0;virtual void release_00409b80(Word)=0;virtual void root_00411240(Word,Word)=0;virtual void variables_0059aa80(Word,Word,Word)=0;virtual void effects_0059b0f0(Word,Word,Word)=0;virtual Word gff_byte_00411a60(Word,Word,Word,Word,Word)=0;virtual void byte(Word,Word)=0;
// Every group retains its original address and exact common argument layout.
virtual void group(Word,Word,Word,Word,Word,Word)=0;
virtual void doors_extra_00507490(Word,Word,Word)=0;virtual void extra_00505da0(Word,Word,Word)=0;virtual void extra_00505eb0(Word,Word,Word)=0;
};
Word area_layout_005073d0(Memory&,AreaContentPorts&,Word,Word);
Word area_objects_0050dd80(Memory&,AreaContentPorts&,Word,Word,Word);
}
