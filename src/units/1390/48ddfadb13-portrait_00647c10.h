#pragma once

// Reconstruction of original KOTOR 1 function 00647C10
// (CSWCCreatureStats::CheckCustomPortrait), a two-argument __cdecl leaf.
//
// Original identity:
//   range            00647C10..00647CDF (0xCF bytes)
//   exits            00647C96: C3 ret, 00647CDE: C3 ret (RET 0, caller cleans 8)
//   receiver         none; incoming ECX is overwritten at 00647C29
//   called from      0066588D (creature update) and 00668AE3 (PlayerList add)
//
// The implementation delivers the exact selected-image replacement symbol
// (_recovered_client_creature_stats_check_custom_portrait_00647c10) that is
// currently the 00509070 trap in 1204-18-w3t2b_w3_traps_t2.obj.

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error "00647C10 reconstruction requires MSVC-compatible Windows x86"
#endif

extern "C" {

// int __cdecl CheckCustomPortrait_00647c10(void* portrait_out, unsigned int is_default_id)
//
// portrait_out points at the caller's 16-byte CResRef.  is_default_id is the
// caller's (portrait_id == 0xFFFE) flag: zero selects "po_hu_m_99_", nonzero
// selects "po_hu_f_99_" when the derived portrait resource is absent.
// Returns 1 when the manager contains the derived resource, 0 otherwise.
//
// noexcept(false): the function has no catch.  A failing constructor, a
// throwing GetPortrait lower-construct path, or a throwing manager call
// propagates to the caller, and the local A destructor runs on unwind after
// construction.  Callers (including the host fixture) must see a potentially
// propagating function; an extern "C" declaration without noexcept(false)
// would inherit the compiler's C-function nonthrow assumption.
int __cdecl recovered_client_creature_stats_check_custom_portrait_00647c10(
    void* portrait_out, unsigned int is_default_id) noexcept(false);

}
