// R3 exit-handler bodies: 005EDB50, 006027A0, 00544390.
//
// Owned original identities (oracle read-only; semantic summary in this
// checkpoint copy; the immutable private instruction evidence remains in the
// source-owner workspace):
//   005EDB50: 8 bytes. Reads the CClientExoAppInternal* from outer+4 and
//             tail-forwards to 006027A0 with the same name argument; RET4.
//   006027A0: 20 bytes. Reads the name stack argument, forms the list address
//             at internal+0x2D8, passes (name, list) to selected 005FBBF0 in
//             the original order, and returns with RET4.
//   00544390: prologue + SEH frame 0x71d150; eight 8-byte CExoString locals;
//             pops at 005443CB/005443F1/00544417/0054443D (first four) and
//             00544497/005444A6/005444BA/005444CC (tail, only when argc > 4);
//             tail locals constructed at 0054444A/00544453/00544461/0054446F;
//             queue at 0054457A/00544586/00544592; six movie calls at
//             0054459E/005445AA/005445B6/005445C2/005445CE/005445DA; conditional
//             fade at 005445E8..00544631; cleanup at 005444E0/00544518/00544526/
//             00544534/00544541 (failure) and 00544636..005446A3 (success);
//             failure EAX=0xFFFFF82F; success EAX=0; ret 8.
//
// Nothing below reimplements a lower: the selected VM pop/string owners, the
// selected movie-list facade, the selected fade/getter and the reviewed E1
// setters are imports. The only local bodies are the three original identities
// named above.
#include "5ae15b5c88-r3_exit_handler.hpp"

#include <cstddef>
#include <cstdint>

namespace {

// Original 00544390 failure return (EAX=0xFFFFF82F).
constexpr int kStringPopFailure = static_cast<int>(0xFFFFF82Fu);

// Original field offsets used by this batch.
constexpr std::size_t kClientInternalOffset = 4;   // [client outer+4]
constexpr std::size_t kServerOuterOffset = 8;      // [manager+8]
constexpr std::size_t kMovieListOffset = 0x2D8;    // [client internal+0x2D8]
constexpr std::size_t kGuiFieldA = 0xB4;           // first fade predicate
constexpr std::size_t kGuiFieldB = 0xC04;          // second fade predicate

// 005EDB50: mov ecx,[ecx+4]. No null check in the original.
inline void* client_internal(void* client_outer) {
  return *reinterpret_cast<void**>(
      static_cast<unsigned char*>(client_outer) + kClientInternalOffset);
}

// 00544390: [manager+4] is the CClientExoApp outer, [manager+8] the
// CServerExoApp outer. Read through the selected volatile cell each time the
// original re-reads it.
inline void* client_outer(void* manager) {
  return *reinterpret_cast<void**>(
      static_cast<unsigned char*>(manager) + kClientInternalOffset);
}
inline void* server_outer(void* manager) {
  return *reinterpret_cast<void**>(
      static_cast<unsigned char*>(manager) + kServerOuterOffset);
}

// The original handler reloads the 007A3A00 cell before every one of its eight
// pop calls (005443BA/005443E6/00544407/0054442D/00544491/005444A0/005444B4/
// 005444CD). Keep that re-read: the helper reads the selected global each call.
inline int pop_string(void* destination) {
  void* const vm_outer = reinterpret_cast<void*>(
      static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00));
  return static_cast<int>(
      recovered_vm_outer_pop_string_005d1080(vm_outer, nullptr, destination));
}

// One original CExoString local plus its construction/cleanup state. The
// original constructs the first four locals as their pops are reached, then
// constructs the four tail locals before the argc check. Cleanup is reverse
// construction order and only touches locals that were constructed:
//   full set: L4 L5 L6 L7 L3 L2 L1 L0
// The wrapper declarations below are semantic order, so reverse declaration
// order is exactly that sequence; an unconstructed wrapper is a no-op.
struct StringSlot {
  kotor_menu_10_19::String value;
  bool live;

  StringSlot() : live(false) {
    value.text = nullptr;
    value.length = 0;
  }
  StringSlot(const StringSlot&) = delete;
  StringSlot& operator=(const StringSlot&) = delete;

  ~StringSlot() {
    if (live) {
      startup_string_element_destroy_005e5c20(&value, nullptr);
    }
  }

  void construct() {
    // Selected 005B3190 zero-fill ctor; non-throwing.
    value.default_005b3190();
    live = true;
  }
};

}  // namespace

// 006027A0: ECX=client internal. list = ECX+0x2D8; call the selected
// AddMovieToExoArrayList facade with the original name/list order and RET8
// lower contract, then the wrapper's RET4.
extern "C" int __fastcall recovered_client_add_movie_internal_006027a0(
    void* client_internal_ptr, void* unused_edx,
    kotor_menu_10_19::String* name) noexcept(false) {
  void* const list =
      static_cast<unsigned char*>(client_internal_ptr) + kMovieListOffset;
  return client_movies::AddMovieToExoArrayList_005fbbf0(
      reinterpret_cast<options_video::Client*>(client_internal_ptr), unused_edx,
      reinterpret_cast<options_video::String*>(name), list);
}

// 005EDB50: ECX=client outer. mov ecx,[ecx+4]; jmp 006027A0. A normal typed call
// is ABI-equivalent to the original tail jump: 006027A0 cleans its own copied
// stack argument, then this adapter's RET4 cleans the caller's argument.
extern "C" int __fastcall recovered_client_add_movie_005edb50(
    void* client_outer_ptr, void* unused_edx,
    kotor_menu_10_19::String* name) noexcept(false) {
  return recovered_client_add_movie_internal_006027a0(
      client_internal(client_outer_ptr), unused_edx, name);
}

// 00544390 full body.
extern "C" int __fastcall recovered_vm_action_start_new_module_00544390(
    void* /*self*/, void* /*unused_edx*/, int /*unused_arg*/,
    int argc) noexcept(false) {
  // Declaration order is semantic order; reverse destruction is the original
  // L4 L5 L6 L7 L3 L2 L1 L0 cleanup order.
  StringSlot module_name;
  StringSlot waypoint;
  StringSlot movie1;
  StringSlot movie2;
  StringSlot movie3;
  StringSlot movie4;
  StringSlot movie5;
  StringSlot movie6;

  // 005443B1..00544444: the first four strings are popped unconditionally, in
  // semantic order. A failed pop leaves that local constructed and cleans up.
  module_name.construct();
  if (!pop_string(&module_name.value)) return kStringPopFailure;

  waypoint.construct();
  if (!pop_string(&waypoint.value)) return kStringPopFailure;

  movie1.construct();
  if (!pop_string(&movie1.value)) return kStringPopFailure;

  movie2.construct();
  if (!pop_string(&movie2.value)) return kStringPopFailure;

  // 0054444A..00544481: construct the four tail locals before the argc check.
  // Their construction order is movie3, movie4, movie5, movie6.
  movie3.construct();
  movie4.construct();
  movie5.construct();
  movie6.construct();

  // 0054447D: cmp dword [ebp+0Ch],4 ; jle success. Signed compare: negative argc
  // skips the tail pops exactly as the original does.
  if (argc > 4) {
    // 00544497..005444DA: all four tail pops are attempted in semantic order;
    // the first missing one fails and returns 0xFFFFF82F without queueing.
    if (!pop_string(&movie3.value)) return kStringPopFailure;
    if (!pop_string(&movie4.value)) return kStringPopFailure;
    if (!pop_string(&movie5.value)) return kStringPopFailure;
    if (!pop_string(&movie6.value)) return kStringPopFailure;
  }

  // 0054456B: load the manager once; EDI=[manager+8] server outer,
  // ESI=[manager+4] client outer. Queue setters and the six movie calls use
  // those cached receivers.
  void* const manager = kotor_menu_window_cells::manager_007a39fc;
  void* const server = server_outer(manager);
  void* const client = client_outer(manager);

  // 00544576..00544592. The original ignores all three returns.
  (void)recovered_server_set_move_to_module_pending_004aecc0(server, nullptr,
                                                             1u);
  (void)recovered_server_set_move_to_module_string_004aecd0(
      server, nullptr, &module_name.value);
  (void)recovered_server_set_move_to_module_start_waypoint_004aed30(
      server, nullptr, &waypoint.value);

  // 00544597..005445DA: six independent movie calls, semantic order. Empty
  // strings are still passed; the selected lower returns 0 without appending.
  (void)recovered_client_add_movie_005edb50(client, nullptr, &movie1.value);
  (void)recovered_client_add_movie_005edb50(client, nullptr, &movie2.value);
  (void)recovered_client_add_movie_005edb50(client, nullptr, &movie3.value);
  (void)recovered_client_add_movie_005edb50(client, nullptr, &movie4.value);
  (void)recovered_client_add_movie_005edb50(client, nullptr, &movie5.value);
  (void)recovered_client_add_movie_005edb50(client, nullptr, &movie6.value);

  // 005445DF..005445EF: first getter reloads the manager and [manager+4].
  void* const manager_for_gui = kotor_menu_window_cells::manager_007a39fc;
  void* const client_for_gui = client_outer(manager_for_gui);
  void* const gui =
      kotor_input_required::get_in_game_gui_005ed690(client_for_gui, nullptr);

  // 005445F1..005445FF: gui != 0 && (gui+0xB4 != 0 || gui+0xC04 != 0).
  // The original tests +0xB4 first; a nonzero +0xB4 jumps directly to the fade
  // at 00544601 and never reads +0xC04. Keep the C++ short-circuit so an
  // unreadable +0xC04 is not touched on that path.
  if (gui != nullptr) {
    const int field_a = *reinterpret_cast<int*>(
        static_cast<unsigned char*>(gui) + kGuiFieldA);
    const bool fade_selected =
        field_a != 0 ||
        *reinterpret_cast<int*>(static_cast<unsigned char*>(gui) +
                                kGuiFieldB) != 0;
    if (fade_selected) {
      // 0054460E..0054461E: the caller's 12-byte colour local is explicitly
      // zeroed. It is the fourth stack argument, never a null pointer.
      int zero_colour[3] = {0, 0, 0};

      // 00544601..0054462F: the manager and [manager+4] are reloaded; the
      // second getter is a separate call and its result is not null-checked.
      void* const manager_for_fade =
          kotor_menu_window_cells::manager_007a39fc;
      void* const client_for_fade = client_outer(manager_for_fade);
      void* const gui_for_fade =
          kotor_input_required::get_in_game_gui_005ed690(client_for_fade,
                                                         nullptr);

      // 00544631: 0062ABF0(gui_for_fade, fade_in=0, a2=0, a3=0, &zero12).
      kotor_m15_ingame_start_global_fade_0062abf0(
          gui_for_fade, nullptr, 0, 0, 0, zero_colour);
    }
  }

  // 00544636..005446A3 cleanup runs here (eight StringSlot destructors), then
  // EAX=0 and RET8. A throw from any live seam above unwinds through the same
  // reverse-order destructors.
  return 0;
}
