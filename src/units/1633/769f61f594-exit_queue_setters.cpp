// E1: reconstructed CServerExoApp move-to-module queue setters.
//
// Original identities and bodies (read-only oracle; semantic summary, no dumps):
//   004AECC0..004AECD0  loads the internal from outer+4, stores the full 32-bit
//                       stack argument at internal+0x10080, returns the internal
//                       pointer, RET4.
//   004AECD0..004AECDE  loads the internal from outer+4, then tail-forwards the
//                       source to selected 005E5C50 with destination internal+0x10084.
//   004AED30..004AED3E  loads the internal from outer+4, then tail-forwards the
//                       source to selected 005E5C50 with destination internal+0x1008C.
//
// The original has no outer/internal null check. This source deliberately keeps
// that dereference boundary: a null outer faults at [outer+4], a null internal
// faults at +0x10080 or inside the assignment. Do not add a guard, fallback
// queue, or fabricated success value.
//
// The two string setters forward to the selected canonical assignment owner
// 00610720 (@async_assign_string_005e5c50@12, thunk to 0060BF40). Allocation,
// empty/self behavior and any failure inside that body remain the selected
// owner's; this adapter does not catch, translate, or repair them.
//
// Exception contract: the imported assignment and both string setters are
// may-throw seams and state noexcept(false) explicitly. Under the recorded
// /EHsc recipe extern "C" callees are otherwise assumed nonthrowing; the
// selected assignment's throw must still propagate. The pending setter
// 004AECC0 is not a may-throw seam and keeps its original non-noexcept
// declaration and body.
#include "e63cb64482-exit_queue_setters.hpp"

static_assert(sizeof(unsigned) == 4,
              "004AECC0 consumes and stores the original 32-bit argument width");

namespace {

// Original field offsets. Only these three fields and the +4 internal pointer
// are part of this reconstruction; no other outer/internal layout is assumed.
constexpr unsigned kServerInternalOffset = 4;
constexpr unsigned kPendingOffset = 0x10080;
constexpr unsigned kModuleNameOffset = 0x10084;
constexpr unsigned kStartWaypointOffset = 0x1008C;

// Original: loads the internal pointer from outer+4. The outer's only field used by these setters is
// the internal pointer at +4; no other outer layout is assumed.
inline unsigned char* server_internal(void* server_outer) {
  void* const* const field = reinterpret_cast<void* const*>(
      static_cast<unsigned char*>(server_outer) + kServerInternalOffset);
  return static_cast<unsigned char*>(*field);
}

}  // namespace

extern "C" void* __fastcall recovered_server_set_move_to_module_pending_004aecc0(
    void* server_outer, void* /*unused_edx*/, unsigned pending) {
  unsigned char* const internal = server_internal(server_outer);
  // Original stores the complete dword, not a normalized bool/byte. The server
  // pump reads this field as a dword, so wider nonzero inputs remain nonzero.
  *reinterpret_cast<unsigned*>(internal + kPendingOffset) = pending;
  // Original EAX after the store is still the internal pointer loaded first.
  return internal;
}

extern "C" void* __fastcall recovered_server_set_move_to_module_string_004aecd0(
    void* server_outer, void* unused_edx, void* source) noexcept(false) {
  unsigned char* const internal = server_internal(server_outer);
  // Original tail-jumps 005E5C50 with ECX = internal+0x10084 and the source on
  // the stack. The selected @12 provider ignores the dummy EDX; pass the
  // adapter's dummy through (the provider does not read it). No stack cleanup
  // or return contract is replaced.
  return async_assign_string_005e5c50(internal + kModuleNameOffset, unused_edx,
                                      source);
}

extern "C" void* __fastcall recovered_server_set_move_to_module_start_waypoint_004aed30(
    void* server_outer, void* unused_edx, void* source) noexcept(false) {
  unsigned char* const internal = server_internal(server_outer);
  // Original tail-jumps 005E5C50 with ECX = internal+0x1008C.
  return async_assign_string_005e5c50(internal + kStartWaypointOffset,
                                      unused_edx, source);
}
