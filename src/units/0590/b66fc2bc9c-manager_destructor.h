#pragma once
#include "15076c627b-resources.h"
#include "1be714b0f9-critical_section.h"
#include "16ff5a2b95-ini_cleanup.h"
#include <new>

namespace menu_resources {
// The original entry is thiscall, ECX receiver and RET0. This source carrier
// uses ECX/ignored EDX and no stack arguments. It destroys constructed members;
// it neither frees the Manager allocation nor clears any owning global cell.
__declspec(noinline) void __fastcall DestroyManager00408670(Manager*,void*);
}

// Typed external IAT obligations. No thread or wait fallback is defined here.
// A fixture may supply these transports only for bounded call sequencing.
extern "C" {
menu_resources::Word __stdcall manager_resume_thread_0073d1dc(menu_resources::Word);
menu_resources::Word __stdcall manager_wait_single_0073d05c(menu_resources::Word,menu_resources::Word);
int __stdcall manager_close_handle_0073d200(menu_resources::Word);
}
