#pragma once
#include "8d5467951c-layout.h"
namespace kotor_signal_storage {
using kotor_signal::Word;
using kotor_signal::Action;
extern Word signal2,signal21,signal22,signal15;
// Updated declarations: these alias canonical subobjects; not extra storage.
extern Word &fpe_first,&fpe_count,&action_bytes,&lookup_count;
extern Action (&action_table)[10];
void __cdecl bind_task35_action_table();
}
extern "C" kotor_signal::Action* signal_lookup_edx_esi();
