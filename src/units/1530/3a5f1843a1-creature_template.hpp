#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureTemplatePorts{virtual ~CreatureTemplatePorts()=default;virtual Word allocate(Word)=0;virtual Word gff_construct(Word,Word,Word,Word)=0;virtual void destroy(Word,Word)=0;virtual void empty(Word)=0;virtual Word ref_text(Word)=0;virtual void format(Word,Word,Word)=0;virtual void ref_literal(Word,Word)=0;virtual void string_destroy(Word)=0;virtual void root(Word,Word)=0;virtual Word stats_header(Word,Word,Word,Word)=0;virtual void appearance(Word,Word,Word)=0;virtual void fields(Word,Word,Word,Word,Word,Word)=0;virtual void stats(Word,Word,Word)=0;virtual Word byte(Word,Word,Word,Word,Word)=0;virtual Word short_value(Word,Word,Word,Word,Word)=0;virtual void stealth(Word,Word,Word)=0;virtual void object_fields(Word,Word,Word)=0;virtual Word float_bits(Word,Word,Word,Word,Word)=0;virtual Word integer(Word,Word,Word,Word,Word)=0;virtual void position(Word,Word,Word,Word,Word)=0;virtual bool orientation_nonzero(Word,Word,Word)=0;virtual void orientation(Word,Word)=0;virtual void finish(Word)=0;};
Word creature_template_005026d0(Memory&,CreatureTemplatePorts&,Word,Word,Word,Word);
}
