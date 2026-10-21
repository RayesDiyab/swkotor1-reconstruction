#pragma once
// W2 tier 6 (fork H): party experience.
//   005653A0 CSWPartyTable::DistributeExperience(int xp, BOOL feedback) RET 8
//   004EF930 CSWSCreature::ReceiveExperience(int xp) RET 4
//   005AF6A0 CSWSCreatureStats::AddExperience(int xp) RET 4 (multiclass penalty)
//   005638A0 CSWPartyTable::GetNPCID(OBJECTID) RET 4
// _ceil 006FC880 is the CRT ceil (UCRT ceil in the native link); the logic calls std::ceil (IEEE-exact, identical).
// __ftol2 006FAE8C goes through w2::ftol2_006fae8c (existing provider natively).
#include "1e16cfcf3d-server_mainloop.hpp"
namespace w2::experience {
using kotor::server_mainloop::Word;
using kotor::server_mainloop::Memory;

struct Ports {
    virtual ~Ports() = default;
    virtual Word party_npc_id_005638a0(Word party, Word id) = 0;   // W1 @recovered_party_table_npc_id_005638a0@12 (t6b)
    [[noreturn]] virtual void trap(Word address, const char* what) = 0;
    virtual Word app_007a39fc() = 0;
    virtual Word rules_007a3a28() = 0;
    virtual Word party_table_004aee70(Word server) = 0;
    virtual void str_literal_005e5a90(Word str, const char* text) = 0;
    virtual void str_destroy_005e5c20(Word str) = 0;
    virtual Word str_cstr_005e5670(Word str) = 0;                              // called for no effect (original)
    virtual Word c2da_int_00413660(Word table, Word row, Word label_str, Word out) = 0;
    virtual void stats_set_experience_005af480(Word stats, Word xp, Word flag) = 0;
    virtual Word creature_by_id_004ae770(Word server, Word id) = 0;
    virtual Word client_options_005ed700(Word client) = 0;
    virtual Word can_level_up_005a6810(Word stats) = 0;
    virtual void auto_levelup_005b27e0(Word stats, Word flag) = 0;
    virtual Word player_list_004aed90(Word server) = 0;
    virtual Word vcall0(Word object, Word slot) = 0;                           // thiscall [[o]+slot](o)
    virtual Word allocate_006fa7e6(Word bytes) = 0;
    virtual Word message_data_construct_004d69f0(Word storage) = 0;
    virtual void message_data_integer_004d76b0(Word data, Word index, Word value) = 0;
    virtual void send_feedback_004ede10(Word creature, Word id, Word data) = 0;
};

void add_experience_005af6a0(Memory&, Ports&, Word stats, Word xp);
// s: >= 0x20 bytes scratch (two CExoString temporaries + two 2DA out slots).
void receive_experience_004ef930(Memory&, Ports&, Word creature, Word xp, Word s);
void distribute_experience_005653a0(Memory&, Ports&, Word party, Word xp, Word feedback, Word s);
}
