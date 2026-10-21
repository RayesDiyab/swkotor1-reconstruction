#pragma once
#include "d4ed6ec73b-ini_list_next.h"
namespace list_accessor_adoption {
using Node=base_ini_reconstruction::Node;
using PublishCursor=void(__cdecl*)(void*,Node*);
// Both entry carriers load their actual cursor object before this one algorithm.
// Publish writes the same object's real C++ type before payload is reloaded.
void* __cdecl NextStep005e9b90(Node*,void*,PublishCursor);
}
namespace base_ini_reconstruction {
// Original-derived literal producer is supplied privately by Main6, once.
extern const char diagnostic_at_pos_0074e264[];
}
