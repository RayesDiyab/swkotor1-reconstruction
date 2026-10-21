// W6 t2 integration: add to 0886, right after `#include "vm_run_native_execute.inc"`.
// This exports the live ReadScriptFile 005D2260 (NativeVmRun::load -> kotor::vm_executor::load_script_005d2260)
// for SetUpScriptSituation 005D23E0. W6 does not add a second owner.
extern "C" int __fastcall recovered_vm_read_script_file_005d2260(void* self,void*,void* name){NativeMemory m;NativeVmRun p(m);return static_cast<int>(p.load(word(self),word(name)));}
