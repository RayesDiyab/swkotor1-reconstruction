// W5 t4 menu-action handlers stored by GetDefaultActions 00620620 in CSWGuiInterfaceAction +C (w5-foreign closure of
// sol-build interaction-combined-t1-strict sealed-tier/source/w5_t4_foreign_traps.cpp, sha 6101ee19...).
// Every handler name below is defined by a real linked provider in the c11 composition, so the /alternatename trap
// fallbacks and the trap bodies are removed: the address stores bind to the provider directly. /include makes the
// linker REQUIRE each provider, so dropping one becomes a link error instead of a silent trap.
#include "2c75bb10c4-w5_t3.hpp"
#define W5_REQUIRE(sym) __pragma(comment(linker, "/include:" sym))
W5_REQUIRE("@kotor_fastcall_0060f620@16")   // interaction_npc.obj        CSWCCreature::ActionInitiateDialog RET 8
W5_REQUIRE("@kotor_fastcall_00682660@16")   // interaction.obj            CSWCPlaceable::UsePlaceable RET 8
W5_REQUIRE("@kotor_fastcall_006826a0@16")   // interaction_callbacks.obj  CSWCPlaceable::BashPlaceable RET 8
W5_REQUIRE("@kotor_fastcall_00683d90@16")   // interaction.obj            CSWCDoor::ToggleDoorState RET 8
W5_REQUIRE("@kotor_fastcall_00683e90@16")   // interaction_callbacks.obj  CSWCDoor::MenuActionBash RET 8
W5_REQUIRE("@kotor_fastcall_00691900@16")   // interaction_callbacks.obj  CSWCTrigger::ActionMenuDisableMine RET 8
W5_REQUIRE("@kotor_fastcall_00691950@16")   // interaction_callbacks.obj  CSWCTrigger::ActionMenuRecoverMine RET 8
W5_REQUIRE("@kotor_fastcall_00616800@16")   // interaction_callbacks.obj  CSWCCreature::ActionMenuAttack RET 8
W5_REQUIRE("@kotor_fastcall_00683ee0@8")    // 1205-34-w4t1b_selection.obj CSWCDoor::GetServerDoor RET 0
