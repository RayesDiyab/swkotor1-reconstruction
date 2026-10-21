#pragma once
// W6 tier t7: 0053A900 ExecuteCommandGetLastHostileTarget (721), claim in lane-w7/OWNERSHIP.md ("W6 t7 claim").
// Original (dis/t7_721.txt): pop object (fail -2001); creature = GetCreatureByGameObjectID; push creature ? +53C : INVALID.
#include "d2fbe17f4b-server_mainloop.hpp"
namespace kotor::vm_commands_t7 {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
constexpr I32 kPushFailed=-2000,kPopFailed=-2001;
constexpr Word kInvalid=0x7f000000;
struct Ports{virtual ~Ports()=default;virtual Word server_app()=0;virtual Word pop_object(Word*)=0;virtual Word push_object(Word)=0;virtual Word get_creature_by_id_004ae770(Word server,Word id)=0;};
I32 get_last_hostile_target_0053a900(Memory&,Ports&);
}
