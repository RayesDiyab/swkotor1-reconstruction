#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureTemplateFinalPorts{virtual ~CreatureTemplateFinalPorts()=default;virtual void literal(Word,Word)=0;virtual void empty(Word)=0;virtual void destroy(Word)=0;virtual void assign(Word,Word)=0;virtual Word named_integer(Word,Word,Word,Word)=0;virtual Word integer(Word,Word,Word,Word)=0;virtual Word string(Word,Word,Word,Word)=0;virtual Word named_float(Word,Word,Word,Word)=0;virtual void movement(Word,Word)=0;};
void creature_template_final_004ed850(Memory&,CreatureTemplateFinalPorts&,Word,Word,Word);
}
