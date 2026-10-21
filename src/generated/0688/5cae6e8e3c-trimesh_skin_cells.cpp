// trimesh_skin_cells.cpp - storage for the original globals referenced ONLY by the trimesh_skin area
// (cross-reference over the full .text listing: no other function and no .data/.rdata table holds these
// addresses). Cold values read from the pinned exe (.data initialised words; BSS = 0).
// The vertex-program source texts (.data, writable char arrays) come from the LOCAL D13-generated header
// (spec/EXTRACT-SPEC.json; extractor = lane-renderer-bodies tools/extract_original_data.py; never committed).
#include "df9dab63cf-trimesh_skin.h"
#if !__has_include(<kotor_generated/mesh/kotor_original_renderer_data.generated.h>)
#error Run extract_original_data.py spec/EXTRACT-SPEC.json generated-local against the pinned swkotor.exe first (D13).
#endif

extern "C" {
// .data render-path gates, cold value 1 (read by `mov eax,[cell]; test eax,eax`), no writer instruction.
volatile kotor_scene3d::U gate_0078e5f4 = 1, gate_0078e5f8 = 1, gate_0078e600 = 1, gate_0078e604 = 1,
    gate_0078e60c = 1, gate_0078e61c = 1, gate_0078e620 = 1, gate_0078e62c = 1, gate_0078e634 = 1,
    gate_0078e638 = 1, gate_0078e63c = 1, gate_0078e640 = 1, gate_0078e644 = 1, gate_0078e658 = 1,
    gate_0078e65c = 1, gate_0078e670 = 1, gate_0078e68c = 1, gate_00798b68 = 1;
// BSS words (cold 0).
volatile kotor_scene3d::U bss_00827fcc = 0, bss_00827fd4 = 0,
    bss_00827fe8 = 0, bss_00828000 = 0;
}

// D13 original .data program texts (writable char arrays: the original passes their addresses as char*
// to VertexProgram::VertexProgram 004A27D0, which stores the pointer at +4).
#define KOTOR_ORIGINAL_CSTRING(name, length, literal) \
    char name[length] = literal; \
    static_assert(sizeof(literal) == (length), #name " length differs from the extract manifest");
#define KOTOR_ORIGINAL_BYTES(name, length, ...) unsigned char name[length] = __VA_ARGS__;
namespace kotor_trimesh_data {
#include <kotor_generated/mesh/kotor_original_renderer_data.generated.h>
}
