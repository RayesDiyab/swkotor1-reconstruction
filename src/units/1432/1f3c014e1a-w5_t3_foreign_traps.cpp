// W5 t3 foreign imports (w5-foreign closure of 1127 w5c/tiers/t3/source/w5_t3_foreign_traps.cpp, sha 53b3f33b...).
// Every import name below is defined by a real linked provider in the c11 composition, so the /alternatename trap
// fallbacks and the trap bodies are removed: callers bind to the provider directly. /include makes the linker
// REQUIRE each provider, so dropping one becomes a link error instead of a silent trap.
#include "842d6b1618-w5_t3.hpp"
#define W5_REQUIRE(sym) __pragma(comment(linker, "/include:" sym))
W5_REQUIRE("@kotor_fastcall_00617ad0@12")                                       // 1205-13-w4t1b_can_see.obj CSWCCreature::CanSee RET 4
W5_REQUIRE("@kotor_fastcall_00604bc0@12")                                       // w4-creature_ai.obj CSWCArea::GetIsObjectCorpse RET 4
W5_REQUIRE("@kotor_fastcall_00682e00@8")                                        // 1205-34-w4t1b_selection.obj GetServerPlaceable RET 0
W5_REQUIRE("@recovered_client_internal_add_to_hitcheck_ignore_list_005f6340@12") // header-1204-04-w3t2b_w3_client_door_placeable.obj RET 4
W5_REQUIRE("@recovered_set_stealth_mode_0050ee50@16")                           // w2_ifo_native.obj CSWSCreature::SetStealthMode RET 8
W5_REQUIRE("@recovered_find_item_with_tag_00555ed0@12")                         // w2_party_native.obj CItemRepository::FindItemWithTag RET 4
