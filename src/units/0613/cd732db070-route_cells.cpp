// lane-m1-audits (L4) REPLACEMENT COPY of the M49 stage unit game-prepare-route-cells (game-prepare/route_cells.cpp,
// sha256 c50d4b0f90499cc2fa7b40b509e0ed31bd6d0e333d39e9a2f9abcb09853c93ed). Only change: the five trailing
// winmain_routes::Cells pointer fields, which the M49 aggregate left value-initialised (nullptr; storage
// 005b111c..005b1157 in the M49 map = 15 words, 10 initialisers), are bound to real storage (D76 class; also r6allow
// open gate G-CELLS). Without this every Exit dereferences nullptr at the first teardown call:
//   WinMain 00404A2F -> 005ED810 -> Uninitialize 005F8474 -> UnloadCachedVFX 00690910 `mov ecx,[00833BC8]; cmp [ecx],0`
//   (shutdown_bodies.cpp: word_at(*cells.vfx_list_00833bc8,0)), then 005F84C3/005F84D9 [008338FC]/[008338EC] and
//   StopServices 00604280 `mov [007A39F4],ebx` (shutdown_bodies.cpp: *cells.gui_007a39f4 = nullptr).
// Bindings (all constant addresses; no dynamic initializer is added to this unit):
//   007A3CD4 product-version CExoString: no M49 storage owner. Original: initializer 0073AD20 (mov ecx,7A3CD4h; call
//            005B3190 default ctor -> {0,0}; push 73B740h; call atexit) / disposer 0073B740 (jmp 005E5C20). Here: 8 zero
//            bytes (= the 005B3190-constructed state), D69-class interim deviation: disposer 0073B740 not registered
//            (shutdown-only effect; only GetGameVersionInfo 005E6C0A/005E6C36 writes it, and only when the image has a
//            VERSIONINFO resource). Joined by /alternatename (D50 cell-join precedent) so the typed name has one storage.
//   008338FC / 008338EC: .bss words; full-image scan finds no writer other than the clears 005F84D3 / 005F84E9 (and
//            readers 005F84C3 / 005F84D9 only) -> loader-zero storage owned here.
//   00833BC8 CachedVFX CExoLinkedList wrapper: owner = lane-audit-r6allow unit xc-0073b670-vfx-list
//            (?list_00833bc8@kotor_vfx_list@@3USoundList@kotor_list@@A, initializer 0073B670 / disposer 0073C170). Joined
//            by /alternatename onto a word view (word 0 = internal pointer, as read at 00690910/00690931). REQUIRES that
//            unit (and the lane-m50-heap G-HEAP fix it depends on); without it this unit fails to link (no silent null).
//   007A39F4 GUI manager cell: existing owner kotor_input_cells::gui_manager_007a39f4 (input-cells.obj, D22/D46).
// lane-prepare: single storage owner for the original cells consumed by lane-routes'
// authored WinMain leaves (winmain_routes::cells) and AurMakeCurrent 0044ED90
// (aur_make_current::cells). BeginIntro 005ED300 is lane-config's (D36); no intro table here.
// Values are swkotor.exe load-image values (sha256 9c10e045...1435).
#include "070a1af72b-game_prepare_producers.h"
#include "643747a7ff-winmain_routes.h"
#include "397879e70b-startup_string.h"     // kotor_reconstruction::StartupString (8-byte CExoString)       // lane-routes: winmain_routes::Cells
#include "529bb7ecfc-aur_make_current.h"     // lane-routes: aur_make_current::Cells
#include "f96fb2c5c4-window_shared_cells.h"
// D50: 007B90FC CExoArrayList<PostedString*> is one 12-byte object owned by
// lane-r4-lowers (r4_cells.cpp); exact declaration repeated from r4_lowers.h.
namespace r4_lowers_cells {
struct PostedList { std::uint32_t volatile data, count, alloc; };
static_assert(sizeof(PostedList) == 12, "original CExoArrayList extent");
extern PostedList posted_list_007b90fc;
}

extern "C" {
// .data 0078E388 headLightEnabled: image FFFF (WORD).
extern std::uint16_t volatile kotor_cell_headlight_0078e388;   // storage: lane-renderer-bodies format_0078e388 (D65 C3 join)
// .data 0078E394: image 0000007F -> 0x7F000000.
std::uint32_t volatile kotor_cell_pause_overlay_0078e394 = 0x7F000000u;
// .data 0078E398 vertexBufferObjectsWriteOnly: image 01.
extern unsigned char volatile kotor_cell_vbo_write_only_0078e398;   // storage: lane-renderer-bodies renderer_flag_0078e398 (D65 C3 join)
// .bss (loader zero) 007B8FD8 resources table / 007B8FDC count.
extern std::uint32_t volatile kotor_cell_resource_table_007b8fd8;   // storage: lane-aurres aurres_stack_007b8fd8 word 0 (D39/D65 C3 join)
std::uint32_t volatile kotor_cell_resource_count_007b8fdc = 0;
// .bss 007B8FE8 dofileErrorBuffer: 100h bytes (next symbol 007B90E8 currentPrintLine).
extern char kotor_cell_dofile_message_007b8fe8[0x100];   // storage: lane-aurres aurres_dofile_message_007b8fe8 (D65 C3 join)
// .bss 007B9228 versionString: 20h bytes (next symbol 007B9248 viewportAlpha).
char kotor_cell_version_007b9228[0x20] = {};
// .bss 007B927D disableVertexBufferObjects.
extern unsigned char volatile kotor_cell_vbo_disabled_007b927d;   // storage: lane-renderer-bodies vertex_buffer_disabled_007b927d (D65 C3 join)
// .bss 007B9270 sleepSeconds (read-only in the image: 0044ED90/0044EDA6).
float volatile kotor_cell_make_current_delay_007b9270 = 0.0f;
}

namespace kotor_input_cells { extern void* volatile gui_manager_007a39f4; }   // 007A39F4 owner (input-hover/input_cells.cpp:8)
namespace kotor_route_cells_m1 {
// 007A3CD4 typed view; storage = kotor_cell_product_version_storage_007a3cd4 below (alternatename join).
extern kotor_reconstruction::StartupString product_version_007a3cd4;
}
extern "C" {
// .bss 008338FC / 008338EC (loader zero; cleared at 005F84D3 / 005F84E9; no other writer in swkotor.exe).
void* volatile kotor_cell_object_008338fc = nullptr;
void* volatile kotor_cell_object_008338ec = nullptr;
// .bss 007A3CD4: 8-byte CExoString {data, length} in the 005B3190-constructed state {0,0}.
__declspec(align(4)) unsigned char kotor_cell_product_version_storage_007a3cd4[8] = {};
// 00833BC8 word 0 (internal list pointer) of the r6allow-owned CExoLinkedList wrapper (alternatename join).
extern void* volatile kotor_cell_vfx_list_00833bc8;
}
#pragma comment(linker, "/alternatename:?product_version_007a3cd4@kotor_route_cells_m1@@3UStartupString@kotor_reconstruction@@A=_kotor_cell_product_version_storage_007a3cd4")
#pragma comment(linker, "/alternatename:_kotor_cell_vfx_list_00833bc8=?list_00833bc8@kotor_vfx_list@@3USoundList@kotor_list@@A")

namespace {
const winmain_routes::Cells winmain_cells_storage{
    &kotor_cell_headlight_0078e388, &kotor_cell_pause_overlay_0078e394,
    kotor_cell_dofile_message_007b8fe8, &kotor_cell_resource_table_007b8fd8,
    &kotor_cell_resource_count_007b8fdc, &r4_lowers_cells::posted_list_007b90fc.data,
    &r4_lowers_cells::posted_list_007b90fc.count, kotor_cell_version_007b9228,
    &kotor_cell_vbo_disabled_007b927d, &kotor_cell_vbo_write_only_0078e398,
    &kotor_route_cells_m1::product_version_007a3cd4,   // 007A3CD4 (005E6C0A)
    &kotor_cell_object_008338fc,                       // 008338FC (005F84C3)
    &kotor_cell_object_008338ec,                       // 008338EC (005F84D9)
    &kotor_cell_vfx_list_00833bc8,                     // 00833BC8 (00690910)
    &kotor_input_cells::gui_manager_007a39f4};         // 007A39F4 (00604280)
static_assert(sizeof(winmain_routes::Cells) == 15 * sizeof(void*), "all 15 Cells fields initialised");
// 007B9274 has one storage: the WinMain Views member bias_7b9274 (read at 00404576).
// prepare publishes its address after Views construction and clears it at release.
aur_make_current::Cells aur_cells_storage{&kotor_cell_make_current_delay_007b9270, nullptr};
}
const winmain_routes::Cells& winmain_routes::cells = winmain_cells_storage;
const aur_make_current::Cells& aur_make_current::cells = aur_cells_storage;

namespace kotor_game_prepare {
void publish_make_current_bias(float volatile* bias_7b9274) noexcept { aur_cells_storage.bias_007b9274 = bias_7b9274; }

}
