#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct PlaceableTemplatePorts{virtual ~PlaceableTemplatePorts()=default;virtual Word allocate(Word)=0;virtual Word construct(Word,Word,Word,Word)=0;virtual void root(Word,Word)=0;virtual void fields(Word,Word,Word,Word)=0;virtual void destroy(Word)=0;virtual void string_zero(Word)=0;virtual void ref_string(Word,Word)=0;virtual Word text(Word)=0;virtual void format(Word,Word,Word)=0;virtual void string_destroy(Word)=0;};
Word placeable_template_00587a70(Memory&,PlaceableTemplatePorts&,Word,Word,Word,Word);
}
