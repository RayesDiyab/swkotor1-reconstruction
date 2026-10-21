#pragma once
#include "93d73ae698-w1_ports.hpp"
// Tier 8: CSWSEffectListHandler callbacks reached from EffectDispatch 004D8320 (0854's ai_resolve table).
namespace w1::effect_handlers {
// OnApplyAssuredHit(object*, effect*, flag), thiscall ret 0xC: 0 = applied, 1 = not applicable.
U on_apply_assured_hit_004d83a0(U handler, U object, U effect, U flag, Ports& p);
// OnRemoveAssuredHit(object*, effect*), thiscall ret 8: always 1.
U on_remove_assured_hit_004d83f0(U handler, U object, U effect, Ports& p);
}
