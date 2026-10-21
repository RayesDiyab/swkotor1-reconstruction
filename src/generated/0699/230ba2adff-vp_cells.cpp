// vp_cells.cpp - storage for the original globals owned by the vp_nv_ati area and the D13 original texts.
// Cold values read from the pinned swkotor.exe (.data initialised words; BSS = 0); xrefs verified over the full
// .text listing (lane-gui-draw/exe-text.asm): each cell below is referenced only by the functions named in
// vp_nv_ati.h (plus the EH unwind funclets that clear the static guard bits).
#include "4bf744a1f4-vp_nv_ati.h"
#if !__has_include(<kotor_generated/vp-data/kotor_original_renderer_data.generated.h>) || \
    !__has_include(<kotor_generated/vp-rdata/kotor_original_renderer_data.generated.h>)
#error Run extract_original_data.py spec/EXTRACT-SPEC-{DATA,RDATA}.json generated-local/{data,rdata} against the pinned swkotor.exe first (D13).
#endif

namespace kotor_vp_cells {
volatile U cache_0078e504 = 0xFFFFFFFFu;     // .data 0078E504 cold FFFFFFFF
volatile U cache_0078e518 = 0xFFFFFFFFu;     // .data 0078E518 cold FFFFFFFF
volatile U gate_0078e628 = 1;                // .data 0078E628 cold 1
char* volatile current_source_008305dc = nullptr;   // BSS
volatile U8 lighting_008305d8 = 0;                   // BSS
volatile U generation_00830540 = 0;                  // BSS
volatile U refresh_007a193c = 1;                     // .data 007A193C cold 1
VertexProgram* volatile flat_program_008280f0 = nullptr;          volatile U flat_guard_008280f4 = 0;
VertexProgram* volatile lightmapped_program_008280f8 = nullptr;   volatile U lightmapped_guard_008280fc = 0;
VertexProgram* volatile env_program_00828100 = nullptr;           volatile U env_guard_00828104 = 0;
VertexProgram* volatile emlm_program_00828108 = nullptr;          volatile U emlm_guard_0082810c = 0;
VertexProgram* volatile skin_program_00830390 = nullptr;          volatile U skin_guard_00830394 = 0;
VertexProgram* volatile skin_sphere_program_00830398 = nullptr;   volatile U skin_sphere_guard_0083039c = 0;
VertexProgram* volatile skin_cube_program_008303a0 = nullptr;     volatile U skin_cube_guard_008303a4 = 0;
std::uint32_t* volatile parameters_00830578 = nullptr;   // BSS, no store in the image
std::uint32_t* volatile parameters_0083058c = nullptr;   // BSS, no store in the image
std::uint32_t* volatile parameters_00830590 = nullptr;   // BSS, no store in the image
}

// D13: original .data program texts (writable char arrays) ...
#define KOTOR_ORIGINAL_CSTRING(name, length, literal) \
    char name[length] = literal; \
    static_assert(sizeof(literal) == (length), #name " length differs from the extract manifest");
#define KOTOR_ORIGINAL_BYTES(name, length, ...) unsigned char name[length] = __VA_ARGS__;
namespace kotor_vp_data {
#include <kotor_generated/vp-data/kotor_original_renderer_data.generated.h>
}
#undef KOTOR_ORIGINAL_CSTRING
#undef KOTOR_ORIGINAL_BYTES
// ... and .rdata strings (const; external linkage from the header's extern declarations).
#define KOTOR_ORIGINAL_CSTRING(name, length, literal) \
    const char name[length] = literal; \
    static_assert(sizeof(literal) == (length), #name " length differs from the extract manifest");
#define KOTOR_ORIGINAL_BYTES(name, length, ...) const unsigned char name[length] = __VA_ARGS__;
namespace kotor_vp_data {
#include <kotor_generated/vp-rdata/kotor_original_renderer_data.generated.h>
}
#undef KOTOR_ORIGINAL_CSTRING
#undef KOTOR_ORIGINAL_BYTES
