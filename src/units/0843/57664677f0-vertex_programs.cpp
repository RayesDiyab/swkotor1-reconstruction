// lane-window: vertex-program manager list 008305CC (CExoArrayList<CAurVertexProgram*>),
// ResetVertexPrograms 004A2470 (cdecl, RET 0) and CAurVertexProgram::~CAurVertexProgram 004A2830
// (__thiscall, RET 0). Evidence: dumpbin /disasm swkotor.exe 004A2470..004A24C1, 004A2830..004A2890,
// atexit 0073BE70 (free data if non-null). Removal is the task-14 ArrayMutations::RemoveReverse32
// (== 00445080: reverse search, store size-1, shift down; ECX list, RET 4).
#include "64c8bc673c-window_lists.h"
#include "39c670fc66-allocator_live.h"      // task-14 ArrayMutations (donor unit staged by lane-aurres)
#include <cstdint>

namespace kotor_renderer_gl_procs {   // lane-renderer-bodies renderer_gl_procs.cpp
extern void (__stdcall* glDeleteProgramsARB_007bb724)(std::int32_t, const std::uint32_t*);
extern void (__stdcall* glDeleteProgramsNV_007bb6f4)(std::int32_t, const std::uint32_t*);
extern std::uint8_t (__stdcall* glIsProgramARB_007bb5b8)(std::uint32_t);
extern std::uint8_t (__stdcall* glIsProgramNV_007bb5ac)(std::uint32_t);
}
namespace kotor_renderer_gl_cells { extern std::int32_t programs_0078e5ec; }   // D5: I32

namespace kotor_aurora_window {
// 008305CC data / 008305D0 size / 008305D4 capacity: BSS zero; the image registers atexit
// 0073BE70, which frees data when non-null. Expressed as a namespace-scope object whose
// destructor performs that same free (no other owner exists; 004A2770 Add / 004A27D0 ctor
// consumers reference this object).
kotor_reconstruction::ArrayMutations vertex_programs_008305cc{};   // 008305CC .bss zero (constant-initialised; window_lists.h)
}
extern "C" {
void __cdecl vertex_program_manager_reset_004a2470();
void __fastcall vertex_program_destroy_004a2830(void* program, void*);
}

namespace {
unsigned char* element(std::int32_t i) {
    std::uint32_t word;
    const auto& l = kotor_aurora_window::vertex_programs_008305cc;
    word = reinterpret_cast<const std::uint32_t*>(static_cast<std::uintptr_t>(l.data))[i];   // [data+i*4]
    return reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(word));
}
std::uint32_t* id_of(unsigned char* program) { return reinterpret_cast<std::uint32_t*>(program + 0x200c); }
}

extern "C" void __cdecl vertex_program_manager_reset_004a2470() {
    auto& l = kotor_aurora_window::vertex_programs_008305cc;
    for (std::int32_t i = 0; i < l.size; ++i) {                      // 004A2470..004A24BD (size reloaded each pass)
        kotor_renderer_gl_procs::glDeleteProgramsARB_007bb724(1, id_of(element(i))); // 004A2480..004A2491
        *id_of(element(i)) = 0;                                       // 004A2497..004A24A0 (+200C)
        *reinterpret_cast<std::uint32_t*>(element(i) + 0x2018) = 0;   // 004A24A6..004A24AF (+2018)
    }
}

extern "C" void __fastcall vertex_program_destroy_004a2830(void* self, void*) {
    using namespace kotor_renderer_gl_procs;
    unsigned char* p = static_cast<unsigned char*>(self);
    std::uint32_t* id = id_of(p);                                     // 004A283C..004A2842
    const std::uint8_t live = kotor_renderer_gl_cells::programs_0078e5ec != 0 ? glIsProgramARB_007bb5b8(*id) // 004A2834..004A284B
                                                                   : glIsProgramNV_007bb5ac(*id);           // 004A2853
    if (live) {                                                       // 004A2859..004A285B (AL)
        if (kotor_renderer_gl_cells::programs_0078e5ec != 0) glDeleteProgramsARB_007bb724(1, id); // 004A285D..004A2869
        else glDeleteProgramsNV_007bb6f4(1, id);                      // 004A287D
    }
    kotor_aurora_window::vertex_programs_008305cc.RemoveReverse32(
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self))); // 004A286F..004A2875 / 004A2883..004A2889
}

// 0073BE70: atexit callback registered by __xc 0078D0CC -> 0073B120 (not registered here; see window_lists.h).
extern "C" void __cdecl kotor_atexit_0073be70() {
    if (const std::uint32_t data = kotor_aurora_window::vertex_programs_008305cc.data)   // 0073BE70..0073BE77
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));            // 0073BE79..0073BE7A
}
