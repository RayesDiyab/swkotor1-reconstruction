#pragma once
// R3 exit-handler batch: 005EDB50, 006027A0 and the full 00544390 body.
//
// Original identities and ABI (oracle artifacts/inputs/swkotor.exe, read-only):
//   005EDB50..005EDB58  8 bytes  thiscall ECX=CClientExoApp outer, one stack
//                       CExoString*, mov ecx,[ecx+4]; jmp 006027A0; RET4.
//   006027A0..006027B4  20 bytes thiscall ECX=CClientExoAppInternal, one stack
//                       CExoString*, list=ECX+0x2D8; call 005FBBF0; RET4.
//   00544390..005446BB  0x32C bytes, two stack dwords (first unused by the
//                       observed body, second behaviourally signed argc), RET8.
//                       Eight CExoString locals, queue setters, six movie calls,
//                       one conditional fade, 0 or 0xFFFFF82F return, SEH
//                       cleanup of every live local.
//
// Native export ABI: the established W1/VM fastcall-with-dummy-EDX convention.
//   @12  receiver + dummy EDX + one stack dword, callee RET4.
//   @16  receiver + dummy EDX + two stack dwords, callee RET8.
// The receiver is the original ECX object. The dummy EDX is never read by the
// selected providers. The handler's argc parameter is signed on purpose: the
// original compares [ebp+0Ch] with 4 using `jle`, so a negative dword skips the
// tail pops rather than taking the unsigned >4 branch.
//
// Reused selected providers (never reimplemented or copied here):
//   005FBBF0 -> 0x0062EEE0
//               ?AddMovieToExoArrayList_005fbbf0@client_movies@@YIHPAUClient@
//               options_video@@PAXPAUString@3@1@Z
//               0374-lane-config-movie-list.obj, RET8.
//   005D1080 -> 0x005F5D00 @recovered_vm_outer_pop_string_005d1080@12,
//               vm_actions_t1.obj, RET4 (forwards the selected 005D25E0 at
//               0x004FFB40 in 1200-vm_string_live_native-live-free.obj).
//   005B3190 -> 0x005FC2E0 ?default_005b3190@String@kotor_menu_10_19@@QAEPAU12@XZ,
//               0012-startup-strings.obj (non-throwing zero-fill).
//   005E5C20 -> 0x005FC2F0 @startup_string_element_destroy_005e5c20@8,
//               0012-startup-strings.obj (non-throwing free/null).
//   005ED690 -> 0x00715B40 ?get_in_game_gui_005ed690@kotor_input_required@@
//               YIPAXPAX0@Z, 0457-successor.obj.
//   0062ABF0 -> 0x00479080 @kotor_m15_ingame_start_global_fade_0062abf0@24,
//               0814-m15-l0-gui.obj, RET0x10.
//   004AECC0/004AECD0/004AED30 -> reviewed E1 exports (separate owner).
// Globals:
//   007A39FC -> 0x008EE344 ?manager_007a39fc@kotor_menu_window_cells@@3RAXA,
//               0053-window-shared-cells.obj.
//   007A3A00 -> 0x009019CC ?message_manager_007a3a00@frame_control@@3IA,
//               0456-r4-lowers-r4-cells.obj.
//
// May-throw seams state noexcept(false) explicitly: the VM pop-string assigns a
// CExoString (game-heap allocation), the E1 string setters forward that
// assignment, the movie lower builds/opens files and appends strings, and the
// fade chain is inside the original handler's SEH region. The handler itself
// propagates those throws after cleaning its eight locals, so its
// declaration/definition and the slot509 forward declaration all state
// noexcept(false). Under the recorded /EHsc recipe the compiler otherwise
// assumes extern "C" callees do not throw; those seams must remain visible so
// the handler's eight locals are cleaned on an exception. The pending scalar
// setter is not a may-throw seam and keeps its unmarked E1 declaration.
//
// No trap, stub, fake success value or local lower body is present in this
// header or the source. The only authored original identities are 005EDB50,
// 006027A0 and 00544390; everything else is an import.

#if !defined(_M_IX86)
#error "R3 exit handler is x86-only (original ECX/stack/RET4/RET8 contract)"
#endif

namespace options_video {
// Pointer/view types used only by the selected movie-list facade's mangled
// signature. The layout is the observed eight-byte CExoString storage.
struct Client;
struct String;
}  // namespace options_video

namespace kotor_menu_10_19 {
// Observed eight-byte CExoString storage. The selected canonical owner is
// 0012-startup-strings.obj; this definition only carries the imported member.
struct String {
  void* text;       // +0
  unsigned length;  // +4
  String* default_005b3190();  // selected 0x005FC2E0
};
}  // namespace kotor_menu_10_19

namespace client_movies {
// Selected 0x0062EEE0 (0374-lane-config-movie-list.obj, RET8). The actual C++
// facade takes the client internal in ECX, a dummy in EDX, and name/list on the
// stack. It is the sole AddMovieToExoArrayList owner; this lane only calls it.
int __fastcall AddMovieToExoArrayList_005fbbf0(options_video::Client* self,
                                              void* unused_edx,
                                              options_video::String* name,
                                              void* list) noexcept(false);
}  // namespace client_movies

namespace kotor_input_required {
// Selected 0x00715B40 (0457-successor.obj): ECX=client outer, dummy EDX.
void* __fastcall get_in_game_gui_005ed690(void* client_outer, void* unused_edx);
}  // namespace kotor_input_required

namespace kotor_menu_window_cells {
// Selected 0x008EE344 (0053-window-shared-cells.obj).
extern void* volatile manager_007a39fc;
}  // namespace kotor_menu_window_cells

namespace frame_control {
// Selected 0x009019CC (0456-r4-lowers-r4-cells.obj); CVirtualMachine* cell.
extern unsigned message_manager_007a3a00;
}  // namespace frame_control

extern "C" {
// Selected 0x005FC2F0 canonical CExoString destroy alias, @8, RET0.
void __fastcall startup_string_element_destroy_005e5c20(void* self,
                                                        void* unused_edx);

// Selected 0x005F5D00 @12, RET4. The argument ECX is the value of the
// 007A3A00 cell (the VM outer); the selected body resolves the VM internal.
// The selected 005D25E0 assignment can allocate and throw, so this seam is
// explicitly may-throw.
unsigned __fastcall recovered_vm_outer_pop_string_005d1080(
    void* vm_outer, void* unused_edx, void* destination) noexcept(false);

// Reviewed E1 exports, fastcall(receiver, dummy, argument), @12, RET4.
// 004AECC0 is not a may-throw seam; the two string setters are.
void* __fastcall recovered_server_set_move_to_module_pending_004aecc0(
    void* server_outer, void* unused_edx, unsigned pending);
void* __fastcall recovered_server_set_move_to_module_string_004aecd0(
    void* server_outer, void* unused_edx, void* source) noexcept(false);
void* __fastcall recovered_server_set_move_to_module_start_waypoint_004aed30(
    void* server_outer, void* unused_edx, void* source) noexcept(false);

// Authored by this lane.
// 005EDB50: ECX=client outer, dummy EDX, one stack CExoString*, @12, RET4.
int __fastcall recovered_client_add_movie_005edb50(
    void* client_outer, void* unused_edx,
    kotor_menu_10_19::String* name) noexcept(false);
// 006027A0: ECX=client internal, dummy EDX, one stack CExoString*, @12, RET4.
int __fastcall recovered_client_add_movie_internal_006027a0(
    void* client_internal, void* unused_edx,
    kotor_menu_10_19::String* name) noexcept(false);
// 00544390: ECX=self, dummy EDX, first stack dword unused by the original body,
// second stack dword signed argc, @16, RET8. It may propagate a throw from the
// pop/setter/movie/fade seams after cleaning its eight locals, so the
// potentially-throwing contract is explicit on the declaration and definition.
int __fastcall recovered_vm_action_start_new_module_00544390(
    void* self, void* unused_edx, int unused_arg, int argc) noexcept(false);

// Selected 0x00479080 @24, RET0x10. ECX=gui, dummy EDX, four stack dwords
// (fade_in, a2, a3, colour12*). It is inside the original SEH region.
void __fastcall kotor_m15_ingame_start_global_fade_0062abf0(
    void* gui, void* unused_edx, int fade_in, int a2, int a3,
    void* colour) noexcept(false);
}
