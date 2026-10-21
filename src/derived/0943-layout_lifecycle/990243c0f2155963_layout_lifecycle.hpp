#pragma once
#include "82c5ff02a1753a36_layout_reference.hpp"
namespace kotor::server_mainloop {
struct LayoutLifecyclePorts {
 virtual ~LayoutLifecyclePorts()=default;
 virtual Word vtable(Word)=0; virtual void ref_default(Word)=0;
 virtual void assign_literal(Word,Word,Word)=0; virtual void release(Word)=0;
 virtual void free(Word)=0; virtual void string_destroy(Word)=0;
 virtual void empty(Word)=0; virtual void ref_string(Word,Word)=0;
 virtual Word parse(Word,Word)=0;
};
Word layout_construct_005de770(Memory&,LayoutLifecyclePorts&,Word,Word);
void layout_release_005de450(Memory&,LayoutLifecyclePorts&,Word);
void layout_base_destroy_005de590(Memory&,LayoutReferencePorts&,LayoutLifecyclePorts&,Word);
void layout_destroy_005de7e0(Memory&,LayoutReferencePorts&,LayoutLifecyclePorts&,Word);
Word layout_load_005df140(LayoutLifecyclePorts&,Word,Word,Word);
Word layout_count_005a4c50(Memory&,Word);
}
