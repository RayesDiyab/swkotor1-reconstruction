// W7 t3 named-trap fallback (selected by /alternatename in vm_actions_t3.cpp only when the action-queue owner's
// export is absent from the link).
extern "C" __declspec(noreturn) void __cdecl kotor_unreachable_trap(unsigned long address, const char* reason);
extern "C" int __fastcall w7_missing_add_action_004cea20(void*, void*, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned,
    unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned, unsigned) {
  kotor_unreachable_trap(0x004cea20, "CSWSObject::AddAction not linked (server action queue owner)");
}
