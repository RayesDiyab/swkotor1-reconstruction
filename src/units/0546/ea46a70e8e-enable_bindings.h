#pragma once
#include "af422c2b90-vertex_program.h"
namespace kotor_vertex_program {
using U32 = std::uint32_t;
using I32 = std::int32_t;
using U8 = std::uint8_t;
// Partial views used only at the observed offsets; no ownership/storage here.
struct LightingInputView { U8 untouched[0x40]; U32 condition_word; };
struct EngineView { U8 untouched[0x80]; void* light_manager; };
struct EnableBindings {
    char** current_source_008305dc;
    I32* refresh_007a193c;
    U8* lighting_008305d8;
    I32* generation_00830540;
    I32* arb_0078e5ec;
    std::uint16_t* format_0078e388;
    const U32* suffix_00743950;
    EngineView* const* engine_007b92a0;
};
extern const EnableBindings& enable_shared;
extern U8 (__stdcall* const is_arb_007bb5b8)(U32);
extern U8 (__stdcall* const is_nv_007bb5ac)(U32);
extern void (__stdcall* const gen_arb_007bb528)(I32, U32*);
extern void (__stdcall* const gen_nv_007bb7b0)(I32, U32*);
extern void (__stdcall* const bind_arb_007bb788)(U32, U32);
extern void (__stdcall* const bind_nv_007bb834)(U32, U32);
extern U8 __cdecl ValidateNonRadeon_0045f5e0();
extern void __cdecl BindVertexProgram_0046f550(U32);
extern char* __cdecl SetupVertexProgramLights_004a30e0(char*, I32, I32);
extern void __cdecl LoadVertexProgram_004a23c0(U32, U32, const char*, U32*);
extern void __cdecl VertexProgramARBError_0047a2c0(const char*);
extern void (__thiscall* const set_light_constants_004a50d0)(void*, I32, I32);
static_assert(offsetof(LightingInputView, condition_word) == 0x40, "observed first-argument load");
static_assert(offsetof(EngineView, light_manager) == 0x80, "observed engine manager load");
}
