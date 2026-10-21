#pragma once
// W6 tier t6: 00546AF0 ExecuteCommandGetSoloMode (462), claim in lane-w7/OWNERSHIP.md ("W6 t6 claim").
// Original: push [CServerExoApp::GetPartyTable() + 190h] (GetPartyTable 004AEE70 = [server+4]+1B770h, inlined as in t5).
#include "d2fbe17f4b-server_mainloop.hpp"
namespace kotor::vm_commands_t6 {
using server_mainloop::Word;using server_mainloop::Memory;using I32=std::int32_t;
constexpr I32 kPushFailed=-2000;
struct Ports{virtual ~Ports()=default;virtual Word server_app()=0;virtual Word push_integer(Word)=0;};
I32 get_solo_mode_00546af0(Memory&,Ports&);
}
