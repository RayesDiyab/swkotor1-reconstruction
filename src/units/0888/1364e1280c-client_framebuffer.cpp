// lane-m48-b fork CLIENT, unit client-framebuffer: CClientOptions::SetFrameBuffer 0061D750 (lane-config options table,
// "Frame Buffer" key only) and its renderer/party leaves.
// Evidence: dumpbin /disasm swkotor.exe 0061D750..0061D8BB, 005ED8B0..005ED8B9, 0044EE70..0044EE7A, 0044EE80..0044EE8A,
// 0045FAB0..0045FB03. Renderer cells/procs borrowed from their M47 owners (gl-renderer-gl-cells / -procs /
// -framebuffer-cells, gl-capabilities). Creature virtual slots +B4/+B8/+CC are called through the receiver's own table
// word, r4_lowers.h vcall precedent (R4 draw.cpp), with the slot's exact __thiscall arity.
#include "e6446a7354-r4_lowers.h"

using namespace r4_lowers;

namespace kotor_renderer_gl_cells {
extern std::uint32_t framebuffer_gate_0078d98c;
extern std::uint32_t cache_frame_0078e528;
extern std::uint32_t capabilities_007bb85c;
}
namespace kotor_renderer_gl_procs { extern std::uint32_t mask_0078e4d4, mask_0078e4dc, mask_0078e4c8; }
namespace kotor_renderer_init { std::uint32_t __cdecl ATIFrameBufferAvailable_0045f860(); }
extern "C" {
void* __fastcall frame_GetCharacter_006346c0(void*, void*, int);   // CSWParty::GetCharacter (r4-lowers)
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);
void __cdecl kotor_cdecl_0044ee70();
void __cdecl kotor_cdecl_0044ee80();
unsigned char __cdecl kotor_cdecl_0045fab0();
}
namespace lane_config_required { void __fastcall SetFrameBuffer_0061d750(void*, void*, std::int32_t); }

// 005ED8B0 CClientExoApp::GetSWParty: [[this+4]+270h].
extern "C" void* __fastcall kotor_fastcall_005ed8b0(void* client, void*) {
    return pw(pw(client, 4), 0x270);                                    // 005ED8B0..005ED8B3
}
// 0044EE70 AurEnableFrameBufferEffects / 0044EE80 AurDisableFrameBufferEffects: [0078D98C] = 1 / 0.
extern "C" void __cdecl kotor_cdecl_0044ee70() { kotor_renderer_gl_cells::framebuffer_gate_0078d98c = 1; } // 0044EE70
extern "C" void __cdecl kotor_cdecl_0044ee80() { kotor_renderer_gl_cells::framebuffer_gate_0078d98c = 0; } // 0044EE80
// 0045FAB0 AurCheckFrameBufferModificationsAvailable: cached tri-state [0078E528] (-1 unknown), AL result.
extern "C" unsigned char __cdecl kotor_cdecl_0045fab0() {
    using namespace kotor_renderer_gl_cells;
    const std::uint32_t cached = cache_frame_0078e528;                  // 0045FAB0
    if (cached == 0xFFFFFFFFu) {                                        // 0045FAB5..0045FAB8
        using namespace kotor_renderer_gl_procs;
        const std::uint32_t need = mask_0078e4d4 | mask_0078e4dc | mask_0078e4c8; // 0045FABA..0045FACD
        if ((capabilities_007bb85c & need) == need) {                   // 0045FACF..0045FAD9
            cache_frame_0078e528 = 1;                                   // 0045FADB
            return 1;                                                   // 0045FAE5
        }
        cache_frame_0078e528 = 0;                                       // 0045FAE8
    } else if (cached != 0) {                                           // 0045FAF4..0045FAF6
        return 1;
    }
    if (kotor_renderer_init::ATIFrameBufferAvailable_0045f860() != 0) return 1; // 0045FAF8..0045FAFF
    return 0;                                                           // 0045FB01
}

namespace {
// One party pass (0061D78F..0061D80F enable / 0061D833..0061D8B3 disable): for each member creature that has effect
// `remove` (slot +CC) while frame-buffer modifications are available, remove it (slot +B8) and apply `apply` (slot +B4).
void swap_effects(void* party, U32 remove, U32 apply) {
    if (!party || static_cast<I32>(w(party, 0)) <= 0) return;           // 0061D77B..0061D789
    unsigned char i = 0;                                                // 0061D785
    do {
        void* creature = frame_GetCharacter_006346c0(party, nullptr, i); // 0061D78F..0061D794 (first index from xor eax,eax)
        if (creature && vcall1(creature, 0xcc, remove) != 0 && kotor_cdecl_0045fab0() != 0) { // 0061D79B..0061D7B9
            vcall1(creature, 0xb8, remove);                             // 0061D7BB..0061D7C4
            U32 vector[3] = {0, 0, 0};                                  // 0061D7E8..0061D7F8
            using ApplySlot = U32(__thiscall*)(void*, U32, U32, U32, U32, U32, U32, U32*);
            reinterpret_cast<ApplySlot>(w(pw(creature, 0), 0xb4))(creature, apply, 0, 0x7f000000u, 0x7f000000u, 0, 0, vector); // 0061D7CA..0061D800
        }
        ++i;                                                            // 0061D808
    } while (static_cast<I32>(i) < static_cast<I32>(w(party, 0)));      // 0061D806..0061D80F (count reread)
}
}

// 0061D750 CClientOptions::SetFrameBuffer(enable), RET 4.
void __fastcall lane_config_required::SetFrameBuffer_0061d750(void* options, void*, std::int32_t enable) {
    void* party = kotor_fastcall_005ed8b0(pw(menu_continuation::app_manager_007a39fc, 4), nullptr); // 0061D750..0061D760
    sw(options, 0x30, static_cast<U32>(enable));                        // 0061D767..0061D76D
    if (enable != 0) {                                                  // 0061D76B..0061D770
        kotor_cdecl_0044ee70();                                         // 0061D776
        swap_effects(party, 0x1f42, 0x1f40);                            // 0061D7A1 / 0061D7BD / 0061D7E1
    } else {
        kotor_cdecl_0044ee80();                                         // 0061D81A
        swap_effects(party, 0x1f40, 0x1f42);                            // 0061D845 / 0061D861 / 0061D885
    }
}
