// GLRender::EnableCombiners 0042E600..00431852 (fork F2, lane-renderer-bodies).
// Lifted from the original disassembly by fork-f2/tools/lift.py: every statement
// cites its original instruction; the 38-entry jump table at 00431854 is the
// switch; merged tails (jmp into shared push/call runs) are expanded per path
// with the original argument order. Original cdecl: (int, CAurTexture* x4);
// ESI holds the glEnable IAT (0042E609) or glBindTexture/glDisable/glTexGeni per
// path; every such call goes through the single adapter owner.
#include "0a27d352e1-nv_combiners.h"
#include "fee770f519-renderer_gl_imports.h"
#include "75b7842bf0-renderer_gl_imports_gen.h"
#include "61e3cacb4e-renderer_gl_procs.h"
#include "3462b4664f-fork_cells_f2.h"
#include <cstring>
namespace kotor_renderer_gl {
namespace {
using U32 = std::uint32_t;
using I32 = std::int32_t;
// Raw DWORD immediates pushed for float parameters (e.g. 3F800000h).
float bits_to_float(U32 bits) { float f; std::memcpy(&f, &bits, sizeof f); return f; }
}
void __cdecl EnableCombiners_0042e600(U32 mode, texture::Basic* texture1, texture::Basic* texture2,
                                      texture::Basic* texture3, texture::Basic* texture4) {
    if (mode > 0x25u) {  // 0042E610 ja 00431848 (flags from 0042E605)
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    switch (mode) {  // 0042E616 jmp [ebx*4+00431854] (38 entries)
    case 0:
    {  // -> 0042E61D
        DisableCombiners_00423cd0(texture1);  // 0042E622 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xB50u);  // 0042E62F
        ActivateTextureStage_0041fe80(0x0u);  // 0042E637 (cdecl; caller pops)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1u, 0x0u);  // 0042E64C
        ActivateTextureStage_0041fe80(0x1u);  // 0042E650 (cdecl; caller pops)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1u, 0x0u);  // 0042E65F
        ActivateTextureStage_0041fe80(0x2u);  // 0042E663 (cdecl; caller pops)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1u, 0x0u);  // 0042E672
        ActivateTextureStage_0041fe80(0x3u);  // 0042E676 (cdecl; caller pops)
        kotor_renderer_init::glBindTexture_0073d240(0xDE1u, 0x0u);  // 0042E685
        ActivateTextureStage_0041fe80(0x0u);  // 0042E689 (cdecl; caller pops)
        kotor_renderer_gl_cells::current_tid_007a683c[0] = 0x0u;  // 0042E693
        kotor_renderer_gl_cells::current_tid_007a683c[1] = 0x0u;  // 0042E69D
        kotor_renderer_gl_cells::current_tid_007a683c[2] = 0x0u;  // 0042E6A7
        kotor_renderer_gl_cells::current_tid_007a683c[3] = 0x0u;  // 0042E6B1
        return;  // 0042E6BC
    }
    case 1:
    {  // -> 0042E713
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 0042E718
        ActivateTextureStage_0041fe80(0x0u);  // 0042E720 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042E72D
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0042E736
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0xB60u);  // 0042E750
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x84C0u, 0x8536u, 0x1907u);  // 0042E76A
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 0042E784
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 0042E79B
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x84C0u, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 2:
    {  // -> 0042E93F
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0042E946
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x0u, 0x8536u, 0x1907u);  // 0042E967
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x0u, 0x8536u, 0x1907u);  // 0042E988
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x8530u, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042E9B1
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0x0u, 0x8537u, 0x1907u);  // 0042E6E2
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042E6FC
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 004304AD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 004304C4
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Cu, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 3:
    case 12:
    case 20:
    case 33:
    case 35:
    case 36:
    {  // -> 00431848
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 4:
    {  // -> 0042E7B5
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0042E7BA
        ActivateTextureStage_0041fe80(0x0u);  // 0042E7BE (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042E7CB
        texture::texture_call_bind_b4(texture4);  // 0042E7D3 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 0042E7DF (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042E7EC
        const U32 t1 = kotor_renderer_gl_cells::attenuation_texture_007a68a0;  // 0042E7EE
        kotor_renderer_init::glBindTexture_0073d240(0xDE1u, t1);  // 0042E7F9
        kotor_renderer_gl_cells::current_tid_007a683c[1] = 0x0u;  // 0042E801
        ActivateTextureStage_0041fe80(0x2u);  // 0042E80B (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0042E818
        texture::texture_call_bind_b4(texture2);  // 0042E820 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x3u);  // 0042E828 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0042E835
        texture::texture_call_bind_b4(texture3);  // 0042E83D [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x0u);  // 0042E845 (cdecl; caller pops)
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0042E853
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C2u, 0x8538u, 0x1907u);  // 0042E877
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C3u, 0x8538u, 0x1907u);  // 0042E89B
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Fu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x1u, 0x0u, 0x0u);  // 0042E8C3
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042E8DD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Fu, 0x8536u, 0x1907u);  // 0042E8F7
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 0042E911
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 0042E928
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 5:
    case 14:
    case 15:
    {  // -> 0042F809
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042F813
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 0042F819
        ActivateTextureStage_0041fe80(0x1u);  // 0042F820 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0042F82D
        texture::texture_call_bind_b4(texture2);  // 0042F835 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x0u);  // 0042F83D (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042F84A
        texture::texture_call_bind_b4(texture1);  // 0042F852 [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042F85F
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8538u, 0x1907u);  // 0042F883
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C1u, 0x8538u, 0x1907u);  // 0042F8A7
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x1u, 0x0u, 0x0u);  // 0042F8CF
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8523u, 0x0u, 0x8537u, 0x1906u);  // 0042F8F0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8524u, 0x84C1u, 0x8538u, 0x1905u);  // 0042F914
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8525u, 0x0u, 0x8537u, 0x1906u);  // 0042F935
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8526u, 0x84C1u, 0x8538u, 0x1905u);  // 0042F959
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1906u, 0x8530u, 0x8530u, 0x852Eu, 0x853Fu, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F985
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042F9A9
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x852Bu, 0x8536u, 0x1907u);  // 0042F9CD
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0042F9EE
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x0u, 0x8536u, 0x1907u);  // 0042FA0F
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042FA38
        if (!(mode == 0xFu)) {  // 0042FA4B jne 0042FA6E (flags from 0042FA3E)
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0x852Eu, 0x8536u, 0x1906u);  // 0042FA78
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0042FA8F
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 0042FAA9
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0042FAC0
            if (mode == 0x5u) {  // 0042FAC9 je 0043182E (flags from 0042FAC6)
                kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Eu, 0x8536u, 0x1906u);  // 00431842
                kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
                return;  // 00431852
            }
            if (mode == 0xFu) {  // 0042FAD2 je 0043182E (flags from 0042FACF)
                kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Eu, 0x8536u, 0x1906u);  // 00431842
                kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
                return;  // 00431852
            }
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0xB60u, 0x8536u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042FA57
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 0042FA8F
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 0042FAA9
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0042FAC0
        if (mode == 0x5u) {  // 0042FAC9 je 0043182E (flags from 0042FAC6)
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Eu, 0x8536u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        if (mode == 0xFu) {  // 0042FAD2 je 0043182E (flags from 0042FACF)
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Eu, 0x8536u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0xB60u, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 6:
    {  // -> 0042FAEC
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042FAF6
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 0042FAFC
        ActivateTextureStage_0041fe80(0x0u);  // 0042FB04 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042FB11
        texture::texture_call_bind_b4(texture1);  // 0042FB19 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 0042FB20 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0042FB2D
        texture::texture_call_bind_b4(texture2);  // 0042FB35 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x2u);  // 0042FB3D (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0042FB4A
        texture::texture_call_bind_b4(texture3);  // 0042FB52 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x3u);  // 0042FB5A (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042FB67
        texture::texture_call_bind_b4(texture4);  // 0042FB6F [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 7);  // 0042FB7C
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8538u, 0x1907u);  // 0042FBA0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C1u, 0x8538u, 0x1907u);  // 0042FBC4
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C0u, 0x8538u, 0x1907u);  // 0042FBE8
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x84C2u, 0x8538u, 0x1907u);  // 0042FC0C
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x1u, 0x1u, 0x0u);  // 0042FC33
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8523u, 0x1u, 0x8536u, 0x1906u);  // 0042FC53
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8524u, 0x84C2u, 0x8536u, 0x1905u);  // 0042FC77
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1906u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042FCA0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x0u, 0x8536u, 0x1907u);  // 0042FCC1
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8536u, 0x1907u);  // 0042FCE2
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1907u);  // 0042FD06
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x852Fu, 0x8536u, 0x1907u);  // 0042FD2A
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Fu, 0x0u, 0x0u, 0x0u, 0x0u, 0x1u);  // 0042FD52
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042FD76
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8524u, 0x852Bu, 0x8536u, 0x1907u);  // 0042FD9A
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1906u, 0x8525u, 0x852Fu, 0x8536u, 0x1905u);  // 0042FDBE
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1906u, 0x8526u, 0x852Fu, 0x8536u, 0x1905u);  // 0042FDE2
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8552u, 0x1907u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042FE0B
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8552u, 0x1906u, 0x8530u, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042FE34
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042FE58
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8524u, 0x852Du, 0x8536u, 0x1907u);  // 0042FE7C
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8525u, 0x852Du, 0x8536u, 0x1907u);  // 0042FEA0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8526u, 0x852Au, 0x8536u, 0x1907u);  // 0042FEC4
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1906u, 0x8525u, 0x852Fu, 0x8536u, 0x1906u);  // 0042FEE8
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1906u, 0x8526u, 0x852Fu, 0x8536u, 0x1906u);  // 0042FF0C
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8553u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042FF35
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8553u, 0x1906u, 0x8530u, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042FF5E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8554u, 0x1907u, 0x8523u, 0x852Cu, 0x8536u, 0x1907u);  // 0042FF82
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8554u, 0x1907u, 0x8524u, 0x1u, 0x8536u, 0x1907u);  // 0042FFA2
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8554u, 0x1907u, 0x8525u, 0x852Eu, 0x8536u, 0x1907u);  // 0042FFC6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8554u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 0042FFE7
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8554u, 0x1906u, 0x8525u, 0x852Fu, 0x8536u, 0x1906u);  // 0043000B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8554u, 0x1906u, 0x8526u, 0x852Fu, 0x8536u, 0x1906u);  // 0043002F
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8554u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00430058
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8554u, 0x1906u, 0x8530u, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00430081
        const U32 t2 = kotor_renderer_gl_cells::combiner_flag_0078d96c;  // 00430087
        if (t2 == 0) {  // 00430093 je 004300A1 (flags from 0043008C)
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8523u, 0x0u, 0x8537u, 0x1907u);  // 004300B7
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 004300DB
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1906u);  // 004300FF
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8526u, 0x852Fu, 0x8536u, 0x1907u);  // 00430123
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8555u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043014C
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 00430170
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8524u, 0x1u, 0x8536u, 0x1907u);  // 00430190
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1907u);  // 004301B4
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8526u, 0x84C3u, 0x8537u, 0x1906u);  // 004301D8
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8556u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043045F
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 00430479
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00430493
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 004304AD
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 004304C4
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Cu, 0x8536u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8523u, 0x84C3u, 0x8536u, 0x1907u);  // 004300B7
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 004300DB
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1906u);  // 004300FF
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8555u, 0x1907u, 0x8526u, 0x852Fu, 0x8536u, 0x1907u);  // 00430123
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8555u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043014C
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 00430170
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8524u, 0x1u, 0x8536u, 0x1907u);  // 00430190
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1907u);  // 004301B4
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8556u, 0x1907u, 0x8526u, 0x84C3u, 0x8537u, 0x1906u);  // 004301D8
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8556u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043045F
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 00430479
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00430493
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 004304AD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 004304C4
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Cu, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 7:
    {  // -> 004302AC
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 004302B6
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 004302BC
        ActivateTextureStage_0041fe80(0x0u);  // 004302C4 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 004302D1
        texture::texture_call_bind_b4(texture1);  // 004302D9 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 004302E0 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 004302ED
        texture::texture_call_bind_b4(texture2);  // 004302F5 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x2u);  // 004302FD (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0043030A
        texture::texture_call_bind_b4(texture3);  // 00430312 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x3u);  // 0043031A (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 00430327
        texture::texture_call_bind_b4(texture4);  // 0043032F [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0043033B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C1u, 0x8538u, 0x1907u);  // 0043035F
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C2u, 0x8538u, 0x1907u);  // 00430383
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C1u, 0x8538u, 0x1907u);  // 004303A7
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x84C3u, 0x8538u, 0x1907u);  // 004303CB
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x1u, 0x1u, 0x0u);  // 004303F2
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8523u, 0x1u, 0x8536u, 0x1906u);  // 00430412
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8524u, 0x84C3u, 0x8536u, 0x1905u);  // 00430436
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1906u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043045F
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 00430479
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00430493
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 004304AD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 004304C4
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Cu, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 8:
    {  // -> 0043050B
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 00430515
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 0043051B
        ActivateTextureStage_0041fe80(0x0u);  // 00430523 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430530
        texture::texture_call_bind_b4(texture1);  // 00430538 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 0043053F (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0043054C
        texture::texture_call_bind_b4(texture2);  // 00430554 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x2u);  // 0043055C (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430569
        texture::texture_call_bind_b4(texture3);  // 00430571 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x3u);  // 00430579 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430586
        texture::texture_call_bind_b4(texture4);  // 0043058E [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 4);  // 0043059B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8538u, 0x1907u);  // 004305BF
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C1u, 0x8538u, 0x1907u);  // 004305E3
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x852Bu, 0x8536u, 0x1907u);  // 00430607
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x852Du, 0x8536u, 0x1907u);  // 0043062B
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x1u, 0x0u, 0x0u);  // 00430653
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 00430677
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x852Fu, 0x8536u, 0x1907u);  // 0043069B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Cu, 0x8536u, 0x1907u);  // 004306BF
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 004306E0
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00430709
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0043072D
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1907u);  // 0043074E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8525u, 0x84C2u, 0x8536u, 0x1907u);  // 00430772
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 00430793
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8552u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 004307BC
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 004307E0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8524u, 0x84C3u, 0x8536u, 0x1907u);  // 00430804
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8525u, 0x0u, 0x8536u, 0x1907u);  // 00430825
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00430846
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8553u, 0x1907u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00431213
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0043122D
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431247
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431261
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431278
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8537u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 9:
    {  // -> 0043086A
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 00430874
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 0043087A
        ActivateTextureStage_0041fe80(0x0u);  // 00430882 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0043088F
        texture::texture_call_bind_b4(texture1);  // 00430897 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 0043089E (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 004308AB
        texture::texture_call_bind_b4(texture2);  // 004308B3 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x2u);  // 004308BB (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 004308C8
        texture::texture_call_bind_b4(texture3);  // 004308D0 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x3u);  // 004308D8 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 004308E5
        texture::texture_call_bind_b4(texture4);  // 004308ED [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 3);  // 004308FA
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8538u, 0x1907u);  // 0043091E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C1u, 0x8538u, 0x1907u);  // 00430942
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x852Au, 0x8536u, 0x1907u);  // 00430966
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x852Du, 0x8536u, 0x1907u);  // 0043098A
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Du, 0x8530u, 0x0u, 0x0u, 0x1u, 0x0u, 0x0u);  // 004309B2
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x84C2u, 0x8536u, 0x1907u);  // 004309D6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x84C3u, 0x8536u, 0x1907u);  // 004309FA
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Cu, 0x8536u, 0x1907u);  // 00430A1E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C3u, 0x8536u, 0x1907u);  // 00430A42
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Fu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00430A6B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8523u, 0x852Fu, 0x8536u, 0x1907u);  // 00430A8F
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1907u);  // 00430AB0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8525u, 0x852Du, 0x8536u, 0x1907u);  // 00430AD4
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8526u, 0x852Eu, 0x8536u, 0x1907u);  // 00430AF8
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8552u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00431213
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0043122D
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431247
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431261
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431278
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8537u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 10:
    {  // -> 00430B26
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 00430B30
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 00430B36
        ActivateTextureStage_0041fe80(0x0u);  // 00430B3E (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430B4B
        texture::texture_call_bind_b4(texture1);  // 00430B53 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 00430B5A (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 00430B67
        texture::texture_call_bind_b4(texture2);  // 00430B6F [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x2u);  // 00430B77 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430B84
        texture::texture_call_bind_b4(texture3);  // 00430B8C [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x3u);  // 00430B94 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430BA1
        texture::texture_call_bind_b4(texture4);  // 00430BA9 [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 4);  // 00430BB6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8538u, 0x1907u);  // 00430BDA
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C1u, 0x8538u, 0x1907u);  // 00430BFE
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x852Au, 0x8536u, 0x1907u);  // 00430C22
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x852Du, 0x8536u, 0x1907u);  // 00430C46
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Fu, 0x852Du, 0x8530u, 0x0u, 0x0u, 0x1u, 0x0u, 0x0u);  // 00430C6E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x84C2u, 0x8536u, 0x1907u);  // 00430C92
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x84C3u, 0x8536u, 0x1907u);  // 00430CB6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Cu, 0x8536u, 0x1907u);  // 00430CDA
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C3u, 0x8536u, 0x1907u);  // 00430CFE
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00430D27
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8525u, 0x852Fu, 0x8536u, 0x1905u);  // 00430D4B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8526u, 0x852Fu, 0x8536u, 0x1905u);  // 00430D6F
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1906u, 0x8530u, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00430D98
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8523u, 0x852Fu, 0x8536u, 0x1907u);  // 00430DBC
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8524u, 0x852Du, 0x8536u, 0x1907u);  // 00430DE0
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8552u, 0x1907u, 0x852Fu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043115D
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 00431181
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1907u);  // 004311A2
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1906u);  // 004311C6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8526u, 0x852Fu, 0x8536u, 0x1907u);  // 004311EA
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8553u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00431213
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0043122D
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431247
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431261
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431278
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8537u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 11:
    {  // -> 00430E09
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 00430E13
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 00430E19
        ActivateTextureStage_0041fe80(0x0u);  // 00430E21 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430E2E
        texture::texture_call_bind_b4(texture1);  // 00430E36 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 00430E3D (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 00430E4A
        texture::texture_call_bind_b4(texture2);  // 00430E52 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x2u);  // 00430E5A (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430E67
        texture::texture_call_bind_b4(texture3);  // 00430E6F [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x3u);  // 00430E77 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00430E84
        texture::texture_call_bind_b4(texture4);  // 00430E8C [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 4);  // 00430E99
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8538u, 0x1907u);  // 00430EBD
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C1u, 0x8538u, 0x1907u);  // 00430EE1
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x852Au, 0x8536u, 0x1907u);  // 00430F05
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x852Du, 0x8536u, 0x1907u);  // 00430F29
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Fu, 0x852Du, 0x8530u, 0x0u, 0x0u, 0x1u, 0x0u, 0x0u);  // 00430F51
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x84C2u, 0x8536u, 0x1907u);  // 00430F75
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x84C3u, 0x8536u, 0x1907u);  // 00430F99
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Cu, 0x8536u, 0x1907u);  // 00430FBD
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C3u, 0x8536u, 0x1907u);  // 00430FE1
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043100A
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8525u, 0x852Fu, 0x8536u, 0x1905u);  // 0043102E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8526u, 0x852Fu, 0x8536u, 0x1905u);  // 00431052
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1906u, 0x8530u, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043107B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8523u, 0x852Fu, 0x8536u, 0x1907u);  // 0043109F
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1907u, 0x8524u, 0x852Du, 0x8536u, 0x1907u);  // 004310C3
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8552u, 0x1907u, 0x852Fu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 004310EC
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1906u, 0x8525u, 0x852Fu, 0x8536u, 0x1906u);  // 00431110
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8552u, 0x1906u, 0x8526u, 0x852Fu, 0x8536u, 0x1906u);  // 00431134
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8552u, 0x1906u, 0x8530u, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043115D
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 00431181
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1907u);  // 004311A2
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1906u);  // 004311C6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8553u, 0x1907u, 0x8526u, 0x852Fu, 0x8536u, 0x1907u);  // 004311EA
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8553u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00431213
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0043122D
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431247
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431261
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431278
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8537u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 13:
    {  // -> 00430206
        ActivateTextureStage_0041fe80(0x1u);  // 00430208 (cdecl; caller pops)
        kotor_renderer_init::glDisable_0073d22c(0x8513u);  // 0043021B
        kotor_renderer_init::glDisable_0073d22c(0xDE1u);  // 00430222
        const U32 t3 = kotor_renderer_gl_cells::current_stage_007a6898;  // 00430224
        kotor_renderer_gl_cells::current_tid_007a683c[t3] = 0x0u;  // 0043022B
        ActivateTextureStage_0041fe80(0x2u);  // 00430236 (cdecl; caller pops)
        kotor_renderer_init::glDisable_0073d22c(0x8513u);  // 00430243
        kotor_renderer_init::glDisable_0073d22c(0xDE1u);  // 0043024A
        const U32 t4 = kotor_renderer_gl_cells::current_stage_007a6898;  // 0043024C
        kotor_renderer_gl_cells::current_tid_007a683c[t4] = 0x0u;  // 00430254
        ActivateTextureStage_0041fe80(0x3u);  // 0043025F (cdecl; caller pops)
        kotor_renderer_init::glDisable_0073d22c(0xDE1u);  // 0043026C
        kotor_renderer_init::glDisable_0073d22c(0x8513u);  // 00430273
        const U32 t5 = kotor_renderer_gl_cells::current_stage_007a6898;  // 00430279
        kotor_renderer_gl_cells::current_tid_007a683c[t5] = 0x0u;  // 00430280
        DisableCombiners_00423cd0(texture1);  // 0043028B (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xB50u);  // 00430298
        ActivateTextureStage_0041fe80(0x0u);  // 004302A0 (cdecl; caller pops)
        return;  // 004302AB
    }
    case 16:
    case 17:
    case 18:
    {  // -> 0042F507
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042F511
        kotor_renderer_init::glDisable_0073d22c(0xB50u);  // 0042F517
        ActivateTextureStage_0041fe80(0x1u);  // 0042F51E (cdecl; caller pops)
        texture::texture_call_bind_b4(texture4);  // 0042F52C [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x0u);  // 0042F534 (cdecl; caller pops)
        texture::texture_call_bind_b4(texture2);  // 0042F542 [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042F54F
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8538u, 0x1907u);  // 0042F573
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x84C1u, 0x8538u, 0x1907u);  // 0042F597
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x1u, 0x0u, 0x0u);  // 0042F5BF
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8523u, 0x0u, 0x8537u, 0x1906u);  // 0042F5E0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8524u, 0x84C1u, 0x8536u, 0x1905u);  // 0042F604
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1906u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F62D
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x0u, 0x8536u, 0x1907u);  // 0042F64E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8536u, 0x1907u);  // 0042F66F
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Eu, 0x8536u, 0x1907u);  // 0042F693
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x852Eu, 0x853Cu, 0x1907u);  // 0042F6B7
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x1u);  // 0042F6DF
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8523u, 0x0u, 0x8537u, 0x1906u);  // 0042F700
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8524u, 0x84C1u, 0x8538u, 0x1905u);  // 0042F724
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1906u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F74D
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8527u, 0x852Eu, 0x8536u, 0x1907u);  // 0042F767
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8528u, 0x852Eu, 0x8536u, 0x1907u);  // 0042F781
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0x8531u, 0x8536u, 0x1907u);  // 0042F79B
        if (!(mode == 0x11u)) {  // 0042F7AE jne 0042F7B7 (flags from 0042F7A1)
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Bu, 0x8536u, 0x1907u);  // 0042F7C1
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0042F7D8
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 0042F7EF
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0xB60u, 0x8536u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x8531u, 0x8536u, 0x1907u);  // 0042F7C1
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0042F7D8
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 0042F7EF
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0xB60u, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 19:
    {  // -> 0042E9BC
        kotor_renderer_gl_cells::env_map_on_00827fa0 = 0x1u;  // 0042E9C3
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042E9C9
        ActivateTextureStage_0041fe80(0x0u);  // 0042E9CF (cdecl; caller pops)
        texture::texture_call_bind_b4(texture1);  // 0042E9DD [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 0042E9E4 (cdecl; caller pops)
        texture::texture_call_bind_b4(texture2);  // 0042E9F4 [vtable+B4] Bind
        const std::uint8_t t6 = texture::texture_call_flag_0c_al(texture2);  // 0042E9FE [vtable+0C] AL
        if (t6 == 0) {  // 0042EA03 je 0042EA53 (flags from 0042EA01)
            kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042EA58
            kotor_renderer_init::glTexGeni_0073d314(0x2000u, 0x2500u, 9218);  // 0042EA6F
            kotor_renderer_init::glTexGeni_0073d314(0x2001u, 0x2500u, 9218);  // 0042EA80
            kotor_renderer_init::glEnable_0073d390(0xC60u);  // 0042EA87
            kotor_renderer_init::glEnable_0073d390(0xC61u);  // 0042EA8E
            kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042EA97
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042EABB
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042EADF
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C1u, 0x8536u, 0x1907u);  // 0042EB03
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 0042EB24
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EB4D
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042EB71
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1906u);  // 0042EB92
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1907u);  // 0042EBB6
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C0u, 0x8537u, 0x1906u);  // 0042EBDA
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EC03
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042EC1D
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431338
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431352
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431369
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Au, 0x8536u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        kotor_renderer_init::glTexGeni_0073d314(0x2000u, 0x2500u, 34066);  // 0042EA1A
        kotor_renderer_init::glTexGeni_0073d314(0x2001u, 0x2500u, 34066);  // 0042EA2B
        kotor_renderer_init::glTexGeni_0073d314(0x2002u, 0x2500u, 34066);  // 0042EA3C
        kotor_renderer_init::glEnable_0073d390(0xC60u);  // 0042EA43
        kotor_renderer_init::glEnable_0073d390(0xC61u);  // 0042EA4A
        kotor_renderer_init::glEnable_0073d390(0xC62u);  // 0042EA8E
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042EA97
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042EABB
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042EADF
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C1u, 0x8536u, 0x1907u);  // 0042EB03
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 0042EB24
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EB4D
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042EB71
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1906u);  // 0042EB92
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1907u);  // 0042EBB6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C0u, 0x8537u, 0x1906u);  // 0042EBDA
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EC03
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042EC1D
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431338
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431352
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431369
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Au, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 21:
    {  // -> 0042EE47
        kotor_renderer_gl_cells::env_map_on_00827fa0 = 0x1u;  // 0042EE4E
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042EE54
        ActivateTextureStage_0041fe80(0x0u);  // 0042EE5A (cdecl; caller pops)
        texture::texture_call_bind_b4(texture1);  // 0042EE68 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 0042EE6F (cdecl; caller pops)
        texture::texture_call_bind_b4(texture2);  // 0042EE7F [vtable+B4] Bind
        const std::uint8_t t7 = texture::texture_call_flag_0c_al(texture2);  // 0042EE89 [vtable+0C] AL
        if (t7 == 0) {  // 0042EE8E je 0042EEDE (flags from 0042EE8C)
            kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042EEE3
            kotor_renderer_init::glTexGeni_0073d314(0x2000u, 0x2500u, 9218);  // 0042EEFA
            kotor_renderer_init::glTexGeni_0073d314(0x2001u, 0x2500u, 9218);  // 0042EF0B
            kotor_renderer_init::glEnable_0073d390(0xC60u);  // 0042EF12
            kotor_renderer_init::glEnable_0073d390(0xC61u);  // 0042EF19
            kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042EF22
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042EF46
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042EF6A
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C1u, 0x8536u, 0x1907u);  // 0042EF8E
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 0042EFAF
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EFD8
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042EFFC
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8536u, 0x1906u);  // 0042F01D
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1907u);  // 0042F041
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C0u, 0x8537u, 0x1906u);  // 0042F065
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F08E
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042F0A8
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 0042F0C2
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 00431261
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431278
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8537u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        kotor_renderer_init::glTexGeni_0073d314(0x2000u, 0x2500u, 34066);  // 0042EEA5
        kotor_renderer_init::glTexGeni_0073d314(0x2001u, 0x2500u, 34066);  // 0042EEB6
        kotor_renderer_init::glTexGeni_0073d314(0x2002u, 0x2500u, 34066);  // 0042EEC7
        kotor_renderer_init::glEnable_0073d390(0xC60u);  // 0042EECE
        kotor_renderer_init::glEnable_0073d390(0xC61u);  // 0042EED5
        kotor_renderer_init::glEnable_0073d390(0xC62u);  // 0042EF19
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042EF22
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042EF46
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042EF6A
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C1u, 0x8536u, 0x1907u);  // 0042EF8E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 0042EFAF
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x852Fu, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EFD8
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042EFFC
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8536u, 0x1906u);  // 0042F01D
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x852Fu, 0x8536u, 0x1907u);  // 0042F041
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C0u, 0x8537u, 0x1906u);  // 0042F065
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F08E
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042F0A8
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 0042F0C2
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 00431261
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431278
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8537u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 22:
    {  // -> 0042EC37
        kotor_renderer_gl_cells::env_map_on_00827fa0 = 0x1u;  // 0042EC3E
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042EC44
        ActivateTextureStage_0041fe80(0x0u);  // 0042EC4A (cdecl; caller pops)
        texture::texture_call_bind_b4(texture1);  // 0042EC58 [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x2u);  // 0042EC60 (cdecl; caller pops)
        texture::texture_call_bind_b4(texture2);  // 0042EC70 [vtable+B4] Bind
        const std::uint8_t t8 = texture::texture_call_flag_0c_al(texture2);  // 0042EC7A [vtable+0C] AL
        if (t8 == 0) {  // 0042EC7F je 0042ECCF (flags from 0042EC7D)
            kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042ECD4
            kotor_renderer_init::glTexGeni_0073d314(0x2000u, 0x2500u, 9218);  // 0042ECEB
            kotor_renderer_init::glTexGeni_0073d314(0x2001u, 0x2500u, 9218);  // 0042ECFC
            kotor_renderer_init::glEnable_0073d390(0xC60u);  // 0042ED03
            kotor_renderer_init::glEnable_0073d390(0xC61u);  // 0042ED0A
            ActivateTextureStage_0041fe80(0x1u);  // 0042ED0D (cdecl; caller pops)
            kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042ED1A
            texture::texture_call_bind_b4(texture3);  // 0042ED22 [vtable+B4] Bind
            kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042ED2F
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042ED53
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042ED77
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C0u, 0x8536u, 0x1907u);  // 0042ED9B
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x84C1u, 0x8536u, 0x1907u);  // 0042EDBF
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EDE8
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042EE0C
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1906u);  // 0042EE2D
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x84C2u, 0x8536u, 0x1907u);  // 0042EBB6
            kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C0u, 0x8537u, 0x1906u);  // 0042EBDA
            kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EC03
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042EC1D
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431338
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431352
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431369
            kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Au, 0x8536u, 0x1906u);  // 00431842
            kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
            return;  // 00431852
        }
        kotor_renderer_init::glTexGeni_0073d314(0x2000u, 0x2500u, 34066);  // 0042EC96
        kotor_renderer_init::glTexGeni_0073d314(0x2001u, 0x2500u, 34066);  // 0042ECA7
        kotor_renderer_init::glTexGeni_0073d314(0x2002u, 0x2500u, 34066);  // 0042ECB8
        kotor_renderer_init::glEnable_0073d390(0xC60u);  // 0042ECBF
        kotor_renderer_init::glEnable_0073d390(0xC61u);  // 0042ECC6
        kotor_renderer_init::glEnable_0073d390(0xC62u);  // 0042ED0A
        ActivateTextureStage_0041fe80(0x1u);  // 0042ED0D (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042ED1A
        texture::texture_call_bind_b4(texture3);  // 0042ED22 [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042ED2F
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042ED53
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042ED77
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C0u, 0x8536u, 0x1907u);  // 0042ED9B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x84C1u, 0x8536u, 0x1907u);  // 0042EDBF
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EDE8
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x852Eu, 0x8536u, 0x1907u);  // 0042EE0C
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1906u);  // 0042EE2D
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x84C2u, 0x8536u, 0x1907u);  // 0042EBB6
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x84C0u, 0x8537u, 0x1906u);  // 0042EBDA
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042EC03
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042EC1D
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00431338
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431352
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431369
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Au, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 23:
    {  // -> 0042F0D9
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0042F0E0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042F104
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042F128
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0043045F
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 00430479
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 00430493
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 004304AD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 004304C4
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Cu, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 24:
    {  // -> 0043128F
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 00431296
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C3u, 0x8536u, 0x1907u);  // 004312BA
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1907u);  // 004312DB
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Fu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00431304
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0043131E
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Fu, 0x8536u, 0x1907u);  // 00431338
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431352
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431369
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Au, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 25:
    {  // -> 00431399
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 004313A0
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C2u, 0x8536u, 0x1907u);  // 004313C4
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Au, 0x8536u, 0x1907u);  // 004313E8
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x84C2u, 0x8536u, 0x1906u);  // 0043140C
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x852Bu, 0x8536u, 0x1907u);  // 00431430
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x8530u, 0x8530u, 0x852Fu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 00431459
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0x0u, 0x8536u, 0x1907u);  // 00431470
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x0u, 0x8536u, 0x1907u);  // 00431487
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0043149E
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x852Fu, 0x8536u, 0x1907u);  // 004314B8
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 004314C3
        return;  // 004314C8
    }
    case 26:
    {  // -> 0042F2D5
        ActivateTextureStage_0041fe80(0x0u);  // 0042F2D7 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042F2E4
        texture::texture_call_bind_b4(texture1);  // 0042F2EC [vtable+B4] Bind
        ActivateTextureStage_0041fe80(0x1u);  // 0042F2F8 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042F305
        texture::texture_call_bind_b4(texture2);  // 0042F30D [vtable+B4] Bind
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042F31A
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 2);  // 0042F320
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C1u, 0x8536u, 0x1907u);  // 0042F344
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1907u);  // 0042F365
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x852Cu, 0x8536u, 0x1907u);  // 0042F389
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 0042F3AA
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F3D3
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0042F3F7
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 0042F41B
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8525u, 0x0u, 0x8536u, 0x1907u);  // 0042F43C
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1907u, 0x8526u, 0x0u, 0x8536u, 0x1907u);  // 0042F45D
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F486
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8523u, 0x852Cu, 0x8536u, 0x1906u);  // 0042F4AA
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8551u, 0x1906u, 0x8524u, 0x84C0u, 0x8536u, 0x1906u);  // 0042F4CE
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8551u, 0x1906u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F4F7
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 004317DD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 004317F7
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431811
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431828
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Eu, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 27:
    {  // -> 0042F1C1
        ActivateTextureStage_0041fe80(0x1u);  // 0042F1C7 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 0042F1D4
        texture::texture_call_bind_b4(texture2);  // 0042F1DC [vtable+B4] Bind
        kotor_renderer_gl_cells::tex1_on_00827fa4 = 0x1u;  // 0042F1E8
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0042F1EE
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C1u, 0x8536u, 0x1907u);  // 0042F212
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x0u, 0x8537u, 0x1907u);  // 0042F233
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8525u, 0x852Cu, 0x8536u, 0x1907u);  // 0042F257
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8526u, 0x0u, 0x8537u, 0x1907u);  // 0042F278
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x8530u, 0x8530u, 0x852Eu, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 0042F2A1
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0x1906u);  // 0042F2BB
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 0042E8F7
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 0042E911
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 0042E928
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x0u, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 28:
    {  // -> 00431383
        kotor_renderer_init::glDisable_0073d22c(0x86DEu);  // 00431388
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 00431393
        return;  // 00431398
    }
    case 29:
    {  // -> 004314C9
        MakeLookUpTexture_00423d80(bits_to_float(0x40A00000u));  // 004314CE (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x86DEu);  // 004314DB
        ActivateTextureStage_0041fe80(0x0u);  // 004314DF (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 004314EC
        texture::texture_call_bind_b4(texture1);  // 004314F4 [vtable+B4] Bind
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 3553);  // 0043150F
        ActivateTextureStage_0041fe80(0x1u);  // 00431513 (cdecl; caller pops)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 34540);  // 0043152A
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86E4u, 33984);  // 0043153B
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86D9u, 34104);  // 0043154C
        ActivateTextureStage_0041fe80(0x2u);  // 00431550 (cdecl; caller pops)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 34540);  // 00431567
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86E4u, 33984);  // 00431578
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86D9u, 34104);  // 00431589
        ActivateTextureStage_0041fe80(0x3u);  // 0043158D (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x8513u);  // 0043159A
        texture::texture_call_bind_b4(texture4);  // 004315A2 [vtable+B4] Bind
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 34546);  // 004315B7
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86E4u, 33984);  // 004315C8
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86D9u, 34104);  // 004315D9
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 004315E0
        return;  // 004315E5
    }
    case 30:
    {  // -> 004315E6
        MakeLookUpTexture_00423d80(bits_to_float(0x40A00000u));  // 004315EB (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0x86DEu);  // 004315F8
        ActivateTextureStage_0041fe80(0x0u);  // 004315FC (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 00431609
        texture::texture_call_bind_b4(texture2);  // 00431611 [vtable+B4] Bind
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 3553);  // 0043162C
        ActivateTextureStage_0041fe80(0x1u);  // 00431630 (cdecl; caller pops)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 34540);  // 00431647
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86E4u, 33984);  // 00431658
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86D9u, 34104);  // 00431669
        ActivateTextureStage_0041fe80(0x2u);  // 0043166D (cdecl; caller pops)
        const U32 t9 = kotor_renderer_gl_cells::look_up_texture_007bb868;  // 00431672
        kotor_renderer_init::glBindTexture_0073d240(0xDE1u, t9);  // 00431680
        kotor_renderer_gl_cells::current_tid_007a683c[2] = 0x0u;  // 00431695
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 34542);  // 0043169F
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86E4u, 33984);  // 004316B0
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86D9u, 34104);  // 004316C1
        ActivateTextureStage_0041fe80(0x3u);  // 004316C5 (cdecl; caller pops)
        kotor_renderer_init::glTexEnvi_0073d2f4(0x86DEu, 0x86DFu, 0);  // 004316D9
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 004316E0
        return;  // 004316E5
    }
    case 31:
    {  // -> 0042E6BD
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0042E6C2
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0042E6CB
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0x0u, 0x8537u, 0x1907u);  // 0042E6E2
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 0042E6FC
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0x0u, 0x8536u, 0x1907u);  // 004304AD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 004304C4
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Cu, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 32:
    {  // -> 004304DE
        ActivateTextureStage_0041fe80(0x1u);  // 004304E0 (cdecl; caller pops)
        kotor_renderer_init::glDisable_0073d22c(0xDE1u);  // 004304ED
        ActivateTextureStage_0041fe80(0x0u);  // 004304F5 (cdecl; caller pops)
        DisableCombiners_00423cd0(texture1);  // 004304FF (cdecl; caller pops)
        return;  // 0043050A
    }
    case 34:
    {  // -> 004316E6
        ActivateTextureStage_0041fe80(0x0u);  // 004316E8 (cdecl; caller pops)
        kotor_renderer_init::glEnable_0073d390(0xDE1u);  // 004316F5
        texture::texture_call_bind_b4(texture1);  // 004316FD [vtable+B4] Bind
        kotor_renderer_gl_procs::glCombinerParameteriNV_007bb664(0x854Eu, 1);  // 0043170A
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8523u, 0x84C0u, 0x8536u, 0x1907u);  // 0043172E
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1907u, 0x8524u, 0x852Cu, 0x8536u, 0x1907u);  // 00431752
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8523u, 0x84C0u, 0x8536u, 0x1906u);  // 00431776
        kotor_renderer_gl_procs::glCombinerInputNV_007bb738(0x8550u, 0x1906u, 0x8524u, 0x852Cu, 0x8536u, 0x1906u);  // 0043179A
        kotor_renderer_gl_procs::glCombinerOutputNV_007bb59c(0x8550u, 0x1907u, 0x852Eu, 0x8530u, 0x8530u, 0x0u, 0x0u, 0x0u, 0x0u, 0x0u);  // 004317C3
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8523u, 0xB60u, 0x8536u, 0xB60u);  // 004317DD
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8524u, 0x852Eu, 0x8536u, 0x1907u);  // 004317F7
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8525u, 0xB60u, 0x8536u, 0x1907u);  // 00431811
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8526u, 0x0u, 0x8536u, 0x1907u);  // 00431828
        kotor_renderer_gl_procs::glFinalCombinerInputNV_007bb654(0x8529u, 0x852Eu, 0x8536u, 0x1906u);  // 00431842
        kotor_renderer_init::glEnable_0073d390(0x8522u);  // 0043184D
        return;  // 00431852
    }
    case 37:
    {  // -> 0042F151
        ActivateTextureStage_0041fe80(0x1u);  // 0042F153 (cdecl; caller pops)
        kotor_renderer_init::glDisable_0073d22c(0x8513u);  // 0042F166
        kotor_renderer_init::glDisable_0073d22c(0xDE1u);  // 0042F16D
        const U32 t10 = kotor_renderer_gl_cells::current_stage_007a6898;  // 0042F16F
        kotor_renderer_gl_cells::current_tid_007a683c[t10] = 0x0u;  // 0042F177
        ActivateTextureStage_0041fe80(0x2u);  // 0042F182 (cdecl; caller pops)
        kotor_renderer_init::glDisable_0073d22c(0x8513u);  // 0042F18F
        kotor_renderer_init::glDisable_0073d22c(0xDE1u);  // 0042F196
        const U32 t11 = kotor_renderer_gl_cells::current_stage_007a6898;  // 0042F19C
        kotor_renderer_gl_cells::current_tid_007a683c[t11] = 0x0u;  // 0042F1A3
        DisableCombiners_00423cd0(texture1);  // 0042F1AE (cdecl; caller pops)
        ActivateTextureStage_0041fe80(0x0u);  // 0042F1B5 (cdecl; caller pops)
        return;  // 0042F1C0
    }
    default: break;
    }
    // unreachable: every table entry returns
}
}
