#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreaturePortraitPorts{virtual ~CreaturePortraitPorts()=default;virtual void literal(Word,Word)=0;virtual void empty(Word)=0;virtual void ref_literal(Word,Word)=0;virtual void set_ref(Word,Word,Word)=0;virtual Word table_string(Word,Word,Word,Word)=0;virtual void assign(Word,Word)=0;virtual Word text(Word)=0;virtual void destroy(Word)=0;};
struct CreaturePortraitRefPorts{virtual ~CreaturePortraitRefPorts()=default;virtual void ref_assign(Word,Word,Word)=0;};
void creature_portrait_ref_004f83c0(Memory&,CreaturePortraitRefPorts&,Word,Word,Word);
void creature_portrait_004ed4c0(Memory&,CreaturePortraitPorts&,Word,Word,Word);
}
