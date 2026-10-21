#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureForcepointPorts{virtual ~CreatureForcepointPorts()=default;virtual Word party(Word)=0;virtual Word level(Word,Word)=0;virtual Word level_info(Word,Word)=0;virtual Word feat(Word,Word)=0;virtual Word class_id(Word,Word)=0;virtual Word effect(Word,Word)=0;};
Word creature_has_effect_004f7880(Memory&,Word,Word);
Word creature_force_class_present_005a6060(Memory&,Word);
// Original return is AX-only; all accumulated arithmetic below is modulo 2^16 at return.
Word creature_forcepoints_004fd490(Memory&,CreatureForcepointPorts&,Word);
}
