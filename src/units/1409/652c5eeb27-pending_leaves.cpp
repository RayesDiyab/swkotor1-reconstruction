// Lane W4 G15: LINK ONLY IF ABSENT. Server-side leaves that CSWGuiInGameEquip::UpdatePortraits (006B5B60) reaches
// during CGuiInGame::Initialize and that have no owner in the 1121 map. Bodies are faithful transcriptions of the tiny
// getters (so the construction path does not trap); the larger GetNPCObject stays an explicit trap. Remove each one as
// soon as the server owner (W1) exports the symbol. See integration/hud.json "pending_leaves".
#include "3f50283d60-w4_native.hpp"
#include "0cdbed471d-w4_providers.hpp"
using namespace w4;
extern "C" {
// 004AEC90 CServerExoApp::GetServerInfo: mov eax,[ecx+4]; mov eax,[eax+10004h]; ret.
// 004AEC90 CServerExoApp::GetServerInfo: bound to W1 tier6 @recovered_server_info_004aec90@8 (t1b); link-only stub removed.
// 005636B0 CSWPartyTable::GetIsNPCAvailable(int) RET 4: 0 <= i < 9 ? [this+30h+i*4] : 0.
// 1127: body superseded by W2 t5 @recovered_party_npc_available_005636b0@12 (identical: 0..8 -> [+30h+4i], else 0); forwarder only.
unsigned __fastcall recovered_party_npc_available_005636b0(void*, void*, unsigned);
U32 __fastcall kotor_fastcall_005636b0(void* table, void*, I32 index) {
    return recovered_party_npc_available_005636b0(table, nullptr, static_cast<unsigned>(index));
}
// 005636D0 CSWPartyTable::GetNumNPCAvailable(): count of non-zero [this+30h..+50h] (byte result in AL).
U8 __fastcall kotor_fastcall_005636d0(void* table, void*) {
    U8 n = 0;
    for (U32 i = 0; i < 9; ++i) if (lw(table, 0x30 + i * 4u) != 0) ++n;
    return n;
}
// 00564700 CSWPartyTable::GetNPCObject(int, int, int) RET 0Ch (448 bytes, server object creation): not reconstructed.
// 00564700 now bound to W2 @recovered_npc_object_00564700@20 (t1 review rebind table); stub removed.
}
