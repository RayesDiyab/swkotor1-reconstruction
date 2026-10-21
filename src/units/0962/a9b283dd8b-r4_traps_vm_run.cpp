// lane-m48-traps (main): D83 whole-function traps for the r4-lowers consumers (r4_lowers.h declarations,
// extern "C" __fastcall kotor_fastcall_<addr>(self, unused EDX, stack words...)). Each body calls the shared
// fatal trap with the original function address; none returns, none produces a value. Reachability evidence
// for every entry: lane-m48-traps/TRAPS.json. These are milestone-2+ debt, not reconstructed behaviour.
#include "c480e742dd-r4_lowers.h"
#include "82db854ff2-kotor_unreachable_trap.h"

extern "C" unsigned __fastcall recovered_vm_run_script_005d45d0(void*,void*,void*,unsigned,unsigned);
extern "C" std::uint32_t __fastcall closeB_server_unload_module_004b9240(void*,void*);   // lane closeB (claude_closeB_unload.cpp)

extern "C" {
// 004B9240 CServerExoAppInternal::UnloadModule (RET). Reached only through the server object
// (CServerExoApp::UnloadModule 004AE8A0 / LoadModule 004B9698 / DealWithLoadGameError 004BA5BC);
// AppManager+8 (server) is 0 on the menu (ctor 00401181; only CreateServer 00401406 writes it).
// Lane closeC: trap removed. The faithful body (with its callee cascade) is lane closeB's
// srcw5/actions/claude_closeB_unload.cpp closeB_server_unload_module_004b9240 (RET 0, EAX 1); this canonical name
// forwards to it (both original callers, LoadModule 004B9698 and DealWithLoadGameError 004BA5BC, ignore EAX).
void __fastcall kotor_fastcall_004b9240(void* self, void* edx) {
    (void)closeB_server_unload_module_004b9240(self, edx);
}
// 005D45D0 CVirtualMachineInternal::RunScript (RET 0Ch). Script VM; menu caller 0060351D gated by client+32Ch.
void __fastcall kotor_fastcall_005d45d0(void* self, void* edx, std::uint32_t* script, std::uint32_t context, std::uint32_t flag) {
    (void)recovered_vm_run_script_005d45d0(self, edx, static_cast<void*>(script), context, flag);
}
}
