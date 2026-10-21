// Lane C (`C-panel-ctor-abi`) source phase: two whole entry adapters for the
// original load-control thunks 005EDD30 and 005EE090.
//
// Original bodies (pinned PE, read-only static analysis):
//   005EDD30 (14 B): original entry bytes omitted from this sanitized copy; origin pin below.
//                    mov ecx,[ecx+4]; add ecx,0x304; tail-jump to Assign005e5c50
//                    ECX=internal (outer), EDX unused, one stack arg, RET 4.
//   005EE090 (8 B):  original entry bytes omitted from this sanitized copy; origin pin below.
//                    mov ecx,[ecx+4]; tail-jump to the accepted hint provider
//                    ECX=internal (outer), EDX unused, two stack args, RET 8, EAX.
//
// Both adapters call the accepted selected providers; neither provider body is
// re-authored, aliased or wrapped beyond this exact entry contract. No caller
// patch, no stack-check/GS/EH bypass, no raw bytes, no numeric-address alias.
#include "644f7234a2-load_screen_adapters.h"

// 005EDD30: the original computes
//   dest = (StartupString*)(*(void**)((char*)internal + 4) + 0x304)
// and tail-jumps to the accepted assignment body with ECX=dest and one stack
// argument. This definition has the same external contract: ECX=internal,
// EDX unused, one stack argument, RET 4. The accepted `Assign005e5c50` keeps
// its own `ret 4`; the adapter's own `ret 4` is emitted from the __fastcall
// declaration. EAX after the call is the assignment's return (`this`); the
// accepted UI declaration is void and no original caller consumes it.
extern "C" void __fastcall load_screen_set_load_music_005edd30(
    void* internal, void* unused, kotor_reconstruction::StartupString* name) {
    (void)unused;
    void* const inner =
        *reinterpret_cast<void**>(static_cast<unsigned char*>(internal) + 4);
    kotor_reconstruction::StartupString* const destination =
        reinterpret_cast<kotor_reconstruction::StartupString*>(
            static_cast<unsigned char*>(inner) + 0x304);
    destination->Assign005e5c50(name);
}

// 005EE090: the original computes
//   inner = *(void**)((char*)internal + 4)
// and tail-jumps to the accepted `@recovered_loading_hint_005f4760@16`
// provider with ECX=inner, EDX unused and two stack arguments (name, arg).
// This definition has the same external contract and returns the provider's
// EAX (unsigned hint strref) with RET 8.
extern "C" unsigned int __fastcall
load_screen_get_next_load_screen_hint_strref_005ee090(
    void* internal, void* unused, kotor_reconstruction::StartupString* name,
    unsigned int arg) {
    (void)unused;
    void* const inner =
        *reinterpret_cast<void**>(static_cast<unsigned char*>(internal) + 4);
    return recovered_loading_hint_005f4760(inner, nullptr, name, arg);
}
