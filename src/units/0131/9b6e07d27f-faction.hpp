#pragma once
#include "93d73ae698-w1_ports.hpp"
// Reputation (W4 review list: 0057CB80) and the faction leaves it reaches. 004AE950/004B40C0 GetFactionOfObject is W7's
// (@recovered_server_faction_of_object_004b40c0@16); the cores reach it through Ports::call(0x4ae950).
namespace w1::faction {
U is_npc_faction_0052b280(U manager, U faction, Ports& p);                      // CFactionManager::GetIsNPCFaction, ret 4
U npc_faction_reputation_0052b420(U manager, U a, U b, Ports& p);               // CFactionManager::GetNPCFactionReputation, ret 8
U creature_reputation_00513ff0(U creature, U other, U other_faction, Ports& p);  // CSWSCreature::GetCreatureReputation, ret 8
// CSWSObject::GetReputation(target id, arg2), ret 8. Frame e: 0x28 bytes per recursion level (recursion uses e + 0x28).
U get_reputation_0057cb80(U object, U target, U arg2, U e, Ports& p);
}
