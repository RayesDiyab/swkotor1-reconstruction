#pragma once
#include "3991cb1894612357_events.hpp"
namespace kotor::module {
// Original004D76B0, ECX event, two stack arguments, RET8. Reuse canonical
// event storage, memory and list-growth ownership; no native veneer installed.
void set_event_integer_004d76b0(Memory&, EventStorage&, Word self, Word index, Word value);
}
