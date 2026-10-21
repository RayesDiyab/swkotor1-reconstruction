#pragma once
// E1 exit-queue setters: 004AECC0, 004AECD0, 004AED30.
//
// Original ABI (all three): ECX = CServerExoApp outer, one stack dword, RET 4.
// The original bodies dereference [outer+4] to reach the CServerExoAppInternal
// and then use offsets +0x10080/+0x10084/+0x1008C. There is no null guard.
//
// Native export ABI: fastcall(receiver, unused EDX, argument), normally @12,
// RET 4. The receiver is still the outer, not the internal. This is the same
// receiver+dummy+argument adapter shape used by the current W1 binder.
//
// 004AECD0/004AED30 tail-jump the original CExoString assignment at 005E5C50.
// The selected canonical owner is 00610720 (@async_assign_string_005e5c50@12),
// a thunk to 0060BF40. It is imported, never reimplemented here.
//
// Exception contract: the canonical assignment and the two forwarding string
// setters are may-throw seams. They carry noexcept(false) explicitly because
// the recorded /EHsc recipe otherwise lets the compiler assume extern "C"
// callees do not throw; the selected assignment's allocation failure/throw must
// propagate. The pending scalar setter 004AECC0 is not a may-throw seam and
// deliberately keeps its declaration without noexcept(false). In C++17
// noexcept(false) is equivalent to an absent exception specification; it does
// not change the @12 decorated names or the RET 4 stack cleanup.

#if !defined(_M_IX86)
#error "exit-queue setters are x86-only (original ECX/stack/RET4 contract)"
#endif

extern "C" {
// Selected 00610720 -> 0060BF40 canonical CExoString assignment. The original
// 005E5C50 has thiscall(ECX=destination, one stack source pointer, RET 4) and
// returns the destination in EAX; the selected @12 fastcall form carries the
// unused EDX explicitly and preserves that stack cleanup and return.
void* __fastcall async_assign_string_005e5c50(void*, void*, void*) noexcept(false);

// 004AECC0: store the full 32-bit argument at internal+0x10080 and return the
// internal pointer in EAX, exactly as the original mov/store/ret sequence.
void* __fastcall recovered_server_set_move_to_module_pending_004aecc0(
    void* server_outer, void* unused_edx, unsigned pending);

// 004AECD0: assign the source CExoString to internal+0x10084 and forward the
// assignment's return value (destination pointer).
void* __fastcall recovered_server_set_move_to_module_string_004aecd0(
    void* server_outer, void* unused_edx, void* source) noexcept(false);

// 004AED30: assign the source CExoString to internal+0x1008C and forward the
// assignment's return value (destination pointer).
void* __fastcall recovered_server_set_move_to_module_start_waypoint_004aed30(
    void* server_outer, void* unused_edx, void* source) noexcept(false);
}
