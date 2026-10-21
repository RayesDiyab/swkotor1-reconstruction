#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {struct FactionMemberHelperPorts{virtual ~FactionMemberHelperPorts()=default;virtual void grow(Word,Word)=0;virtual Word message(Word)=0;virtual Word player(Word,Word)=0;virtual void send(Word,Word,Word,Word,Word,Word)=0;};Word faction_lookup_0052b3b0(Memory&,Word,Word);Word creature_faction_00513fc0(Memory&,Word);void unique_append_006449d0(Memory&,FactionMemberHelperPorts&,Word,Word);void faction_notify_005be9c0(Memory&,FactionMemberHelperPorts&,Word,Word,Word);}
