// lane-m50-vtables (W-4 scan): constant-initialized addresses of lane-tlk's typed CTlkTable / CSWTlkTable method
// tables (tlk-core/tlk_parse.cpp, D44) for the two M40 writers whose header roots do not reach tlk_core.h:
//   service-retained-11 tlk_fields.cpp  CTlkTable::CTlkTable 0041D8D0 (original 0041D8E3 mov [esi],0073ECDCh)
//   start-body start_body.cpp           StartServices 005F899B          (original 005F899B mov [eax],0074E700h)
// Address constants of objects with static storage: constant initialization (no dynamic initializer, R7 audit).
#include "4c176c7801-tlk_core.h"
#include "b593572262-m50_vtables.h"

extern "C" const void* const kotor_vtable_address_ctlktable_0073ecdc = &kotor_vtable_ctlktable_0073ecdc;
extern "C" const void* const kotor_vtable_address_cswtlktable_0074e700 = &kotor_vtable_cswtlktable_0074e700;
