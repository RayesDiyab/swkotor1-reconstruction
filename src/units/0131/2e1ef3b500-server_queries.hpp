#pragma once
#include "93d73ae698-w1_ports.hpp"
// Server queries reached every frame by the client/GUI side (W4 review list).
namespace w1::server_queries {
U server_player_list_004aed90(U server_app, Ports& p);                        // CServerExoApp::GetPlayerList, ret
U server_info_004aec90(U server_app, Ports& p);                               // CServerExoApp::GetServerInfo, ret
U server_is_injured_004aee40(U server_app, U client_id, U scratch, Ports& p); // CServerExoApp::IsInjured(client id), ret 4
U internal_is_injured_004b1d20(U internal, U id, U scratch, Ports& p);        // CServerExoAppInternal::IsInjured(id), ret 4
U creature_is_injured_004eff30(U creature, Ports& p);                         // CSWSCreature::IsInjured, ret
void object_delete_dialog_004cc330(U object, Ports& p);                       // CSWSObject::DeleteDialog, ret
U creature_following_leader_004f7750(U creature, Ports& p);                   // CSWSCreature::GetCurrentlyFollowingLeader, ret
}
