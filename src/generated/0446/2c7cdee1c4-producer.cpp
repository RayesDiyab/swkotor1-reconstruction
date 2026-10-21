// lane-m49-renderer: renderer descriptor publication (replaces aurora-reviewed/epoch/producer.cpp,
// unit stem renderer-epoch-producer).
//
// WHY: the M41 epoch producer left renderer_adoption::proposed_publication as zero BSS and expected a
// "Main" to call renderer_adoption::prepare(...) before the first CreateAurora/Initialize. No unit ever
// called it (M41..M48), so kotor_aurora_factory::views, kotor_renderer_init::shared,
// accepted_renderer_peer_views and aurora_context_adoption::live_extensions all read null members at
// the first factory call / Renderer::Initialize -> first-frame crash.
//
// ORIGINAL: there is no runtime publication step. Every consumer addresses the cells with absolute
// operands fixed by the linker/loader, e.g.
//   factory 0044ECD0:  mov eax,[007B921C] / mov word [007B9220],si / mov [eax],741518h / [007B9218]
//   Initialize 0044DAB0: [0078E38C], [0078E388], [007A6888], [007B9220], [007B9224], [0078D46C],
//                        [0078E5EC], [0078D440], [0078D438], call [007BB730], call [007BB538] (0044E381)
//   program records 0044DFAB..0044E338 (ten blocks, below)
//   TrackMatrices 004A2400: [0078E5EC], [0073F218], [0073F21C], [0073F224], call [007BB744]
//   InitRenderSystem 00425C30: call [0073D2E0], [0073D2DC], -> [007A6854], [007A47E4], rep stos
//                        007A683C x6, [007A6898]
//   TextureRefreshAll 00422360: [007B90F0], call 0044CC60, call 0044CC40, push [0078D3F4], call 00421D90
// The faithful equivalent of "who fills the descriptor" is therefore image load: this unit
// CONSTANT-initializes the publication with the link-time addresses of the single owner cells, so it
// is valid before WinMain (no .CRT$XC* initializer, no call, no admission order). prepare() and the
// Inputs path are retired (no caller existed; nothing references the symbol).
#include "34eaa2d5d4-renderer_adoption.h"
#include "5d1707ffbd-renderer_gl_cells.h"   // M49 composer: stage-relative (unit header_roots unchanged); single owner of the scalar cells, 007BB730/007BB538/007BB774, 007BB744/788/834
#include "77b827d480-render_system_gl.h"    // 0073D2E0 / 0073D2DC getters (aurora_context_adoption)
#include <cstdint>

// ---- borrowed single owners (declared with their defining units' exact types) ----
namespace kotor_aurora_window {
extern const renderer_adoption::VTable native_vtable_00741518;   // aurora_window_slots.cpp (00741518)
extern unsigned int font_loaded_007b90f0;                        // aurora_cleanup.cpp (.bss 007B90F0)
}
extern "C" void __cdecl aurora_free_font_0044cc60();             // aurora_cleanup.cpp (AurFreeFont 0044CC60)
extern "C" void __cdecl kotor_cdecl_0044cc40();                  // m48/t/f-keys aur_print.cpp (AurInitFont 0044CC40)
namespace texture_cells { extern volatile std::int32_t max_tex_memory_0078d3f4; }  // texture_cells.cpp
namespace menu_tables_required { unsigned char __cdecl TextureSetMem_00421d90(int); } // startup-misc texture_memory.cpp
namespace renderer_program_cells {                               // epoch program_cells.cpp (D75)
extern void* output_00827FEC; extern void* output_00827FF0; extern void* output_00827FF4;
extern void* output_00827FF8; extern void* output_00827FFC; extern void* output_00828014;
extern void* output_0083037C; extern void* output_00830380; extern void* output_00830388; extern void* output_0083038C;
extern std::uint32_t* parameters_00830544; extern std::uint32_t* parameters_00830548;
extern std::uint32_t* parameters_0083054C; extern std::uint32_t* parameters_00830560;
extern std::uint32_t* parameters_00830564; extern std::uint32_t* parameters_0083057C;
}
namespace kotor_renderer_gl_cells {                              // renderer_gl_loader_cells.cpp (Tier B, D76)
extern char* first_program_0083028c;
extern char* second_program_00830290;
}
namespace kotor_vp_data {                                        // 3d/vp vp_cells.cpp (D13 .data texts)
extern char arb_flat_vp_0078feb8[1095];
extern char arb_skin_vp_00792658[2926];
extern char nv_skin_vp_0079b5a8[1682];
}
namespace kotor_trimesh_data { extern char nv_program_0079ab28[574]; }  // 3d/mesh trimesh_skin_cells.cpp (D13)

// ---- storage first owned here (no other M48 unit defines these addresses) ----
namespace kotor_aurora_factory {
// 007B921C Aurora singleton / 007B9218 reference count: .data loader-zero (raw size 0x17000 < 0x3B21C).
// Readers/writers: factory 0044ECD0..0044ED31, release 0044D940..0044D958.
void* volatile singleton_007b921c = nullptr;
volatile Word counter_007b9218 = 0;
}
namespace renderer_track_cells {
// .rdata words read by TrackMatrices 004A2400 (004A2409/0E/32). Values from the pinned image:
// 0073F218 = 8629h (GL_MODELVIEW_PROJECTION_NV), 0073F21C = 1700h (GL_MODELVIEW), 0073F224 = 862Ah (GL_IDENTITY_NV).
extern const std::uint32_t matrix_0073f218 = 0x8629u;
extern const std::uint32_t matrix_0073f21c = 0x1700u;
extern const std::uint32_t transform_0073f224 = 0x862Au;
}
namespace renderer_program_cells {
// D13: the seven original .data program texts not yet extracted by any lane. Bytes come only from the
// LOCAL generated header (spec lane-m49-renderer/spec/EXTRACT-SPEC.json, extractor
// lane-renderer-bodies/tools/extract_original_data.py against the pinned exe). Never committed.
#if !__has_include(<kotor_generated/epoch/kotor_renderer_programs_original_data.generated.h>)
#error Run extract_original_data.py with lane-m49-renderer/spec/EXTRACT-SPEC.json first (D13).
#endif
#define KOTOR_ORIGINAL_CSTRING(name, length, literal) \
    char name[length] = literal; \
    static_assert(sizeof(literal) == (length), #name " length differs from the extract manifest");
#include <kotor_generated/epoch/kotor_renderer_programs_original_data.generated.h>
#undef KOTOR_ORIGINAL_CSTRING
}

namespace {
using namespace kotor_renderer_init;
// D24-style typed adapter: renderer_texture::Views::set_memory is U32(__cdecl*)(U32); the single owner of
// 00421D90 is typed unsigned char(__cdecl*)(int). Original EAX at 00421EC8..00421ECC is xor eax,eax;
// setle al, i.e. exactly the zero-extended byte, and 00422379 pushes the raw dword argument.
std::uint32_t __cdecl texture_set_memory_00421d90(std::uint32_t memory) {
    return menu_tables_required::TextureSetMem_00421d90(static_cast<int>(memory));
}
// renderer_track::Views over 0078E5EC / 0073F218 / 0073F21C / 0073F224 / 007BB744 / 007BB788 / 007BB834.
renderer_track::Views track_views = {
    &kotor_renderer_gl_cells::programs_0078e5ec,
    &renderer_track_cells::matrix_0073f218, &renderer_track_cells::matrix_0073f21c,
    &renderer_track_cells::transform_0073f224,
    &kotor_renderer_gl_procs::glTrackMatrixNV_007bb744,
    &kotor_renderer_gl_procs::glBindProgramARB_007bb788,
    &kotor_renderer_gl_procs::glBindProgramNV_007bb834};
// renderer_system::Views over 0073D2E0 / 0073D2DC / 007A6854 / 007A47E4 / 007A683C[6] / 007A6898.
renderer_system::Views system_views = {
    &aurora_context_adoption::current_context_0073d2e0, &aurora_context_adoption::current_dc_0073d2dc,
    &kotor_renderer_gl_cells::main_context_007a6854, &kotor_renderer_gl_cells::main_dc_007a47e4,
    kotor_renderer_gl_cells::current_tid_007a683c, &kotor_renderer_gl_cells::current_stage_007a6898};
// renderer_texture::Views over 007B90F0 / 0078D3F4 / 0044CC60 / 0044CC40 / 00421D90.
// 0078D3F4's owner is a signed volatile dword; the view reads the same object as unsigned (object-pointer
// cast between corresponding signed/unsigned types, D24; 00422373 reads the raw dword).
renderer_texture::Views texture_views = {
    &kotor_aurora_window::font_loaded_007b90f0,
    reinterpret_cast<const volatile std::uint32_t*>(&texture_cells::max_tex_memory_0078d3f4),
    &aurora_free_font_0044cc60, &kotor_cdecl_0044cc40, &texture_set_memory_00421d90};
}

namespace renderer_adoption {
namespace c = kotor_renderer_gl_cells;
namespace p = renderer_program_cells;
// Image-load publication (see header comment). Field order follows renderer_adoption.h Publication.
Publication proposed_publication = {
    // factory 0044ECD0: singleton 007B921C, counter 007B9218, state word 007B9220 (0044ED03), vtable 00741518 (0044ED10)
    {&kotor_aurora_factory::singleton_007b921c, &kotor_aurora_factory::counter_007b9218,
     &c::color_bits_007b9220, &kotor_aurora_window::native_vtable_00741518},
    // SharedBindings: 0078E38C, 0078E388, 007A6888, 007B9220, 007B9224, 0078D46C, 0078D440, 0078D438, 0078E5EC
    {&c::initialized_0078e38c, &c::format_0078e388, &c::antialias_007a6888,
     &c::color_bits_007b9220, &c::depth_bits_007b9224, &c::vertex_enum_0078d46c,
     &c::previous_antialias_0078d440, &c::vsync_0078d438, &c::programs_0078e5ec,
     // Ten program records, Initialize 0044DAB0 (identical records read by InitEditor 0044E3B0).
     // {nonzero ([0078E5EC]!=0) text, zero text, dynamic nonzero cell, parameters cell, output cell, Enable arg}
     {{nullptr, p::source_007990F0, &c::second_program_00830290, &p::parameters_0083054C, &p::output_0083037C, 0},     // 0044DFD0..0044E009
      {kotor_vp_data::arb_skin_vp_00792658, kotor_vp_data::nv_skin_vp_0079b5a8, nullptr, &p::parameters_0083054C, &p::output_00830380, 1}, // 0044E02E..0044E064
      {kotor_vp_data::arb_flat_vp_0078feb8, kotor_trimesh_data::nv_program_0079ab28, nullptr, &p::parameters_00830564, &p::output_00827FEC, 1}, // 0044E089..0044E0BF
      {p::source_00790E70, kotor_trimesh_data::nv_program_0079ab28, nullptr, &p::parameters_00830560, &p::output_00827FF0, 0}, // 0044E0E4..0044E119
      {kotor_vp_data::arb_flat_vp_0078feb8, kotor_trimesh_data::nv_program_0079ab28, nullptr, &p::parameters_00830544, &p::output_00827FF4, 1}, // 0044E13E..0044E174
      {nullptr, p::source_00798CC8, &c::first_program_0083028c, &p::parameters_00830544, &p::output_00827FF8, 0},      // 0044E199..0044E1CF
      {nullptr, p::source_00798CC8, &c::first_program_0083028c, &p::parameters_00830548, &p::output_00827FFC, 0},      // 0044E1F4..0044E22A
      {p::source_00794600, p::source_0079BC40, nullptr, &p::parameters_0083057C, &p::output_00830388, 0},              // 0044E24F..0044E284
      {p::source_00794600, p::source_0079BC40, nullptr, &p::parameters_0083054C, &p::output_0083038C, 0},              // 0044E2A9..0044E2DE
      {p::source_00797488, p::source_007A0B10, nullptr, &p::parameters_00830560, &p::output_00828014, 0}}},            // 0044E303..0044E338
    // Peer views: TrackMatrices 004A2400, InitRenderSystem 00425C30, TextureRefreshAll 00422360.
    {&track_views, &system_views, &texture_views},
    // Extension cells: 007BB730 (choose), 007BB538 (swap, D51 writable owner), 007BB774 (D11 compressed upload).
    {&kotor_renderer_gl_procs::wglChoosePixelFormatARB_007bb730, &kotor_renderer_gl_procs::wglSwapIntervalEXT_007bb538,
     &kotor_renderer_gl_procs::glCompressedTexImage2DARB_007bb774}};
}
// Descriptor reference exports (unchanged names/types from the M41 producer).
namespace kotor_aurora_factory {Views* const views=&renderer_adoption::proposed_publication.factory;}
namespace kotor_renderer_init {
SharedBindings& shared=renderer_adoption::proposed_publication.shared;
AcceptedRendererPeerViews& accepted_renderer_peer_views=renderer_adoption::proposed_publication.peers;
}
namespace aurora_context_adoption {const LiveExtensions& live_extensions=renderer_adoption::proposed_publication.extensions;}
