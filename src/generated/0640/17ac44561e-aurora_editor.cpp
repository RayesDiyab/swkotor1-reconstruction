// lane-window: CAurInternalGL::InitEditor 0044E3B0 (vtable 00741518 +10), __thiscall, 7 stack
// words, RET 1Ch. Evidence: dumpbin /disasm swkotor.exe 0044E3B0..0044E9D3; EH FuncInfo 00763078
// (handler 00716560): states 0..7 each unwind with free([allocation]) (funclets 007164F0..).
// Sibling of Renderer::Initialize 0044DAB0; program records are read from the same epoch
// SharedBindings::programs[] the M41 Initialize uses (records 0,1,2,3*,4,5,6), plus the
// editor-only program (source 0079C310, parameters 00830580, output 00830384).
#include "f9c9586b20-aurora_window_slots.h"
#include "7b8c81fcff-allocator_live.h"
#include <cstring>

using namespace kotor_renderer_init;

namespace kotor_renderer_init {
void __stdcall LoadExtensions_00436490();          // lane-renderer-bodies extension_loader.cpp
}
namespace renderer_program_cells {                   // editor-only program inputs (sole owner: this unit)
extern char source_0079C310[];                       // .data program text "!!VP1.0 # ... skinned animations" (1682 bytes incl. NUL)
extern std::uint32_t* parameters_00830580;           // read 0044E7A8 (also 0048C4EF)
// 00830580: .bss (loader zero); no store to it exists in the image, so it stays null.
std::uint32_t* parameters_00830580 = nullptr;
// D13: bytes come only from the LOCAL generated header (lane-window/generated-local, spec/EXTRACT-SPEC.json,
// extractor lane-renderer-bodies/tools/extract_original_data.py against the pinned exe). Never committed.
#if !__has_include(<kotor_generated/window/kotor_window_original_data.generated.h>)
#error Run extract_original_data.py with lane-window/spec/EXTRACT-SPEC.json first (D13).
#endif
#define KOTOR_ORIGINAL_CSTRING(name, length, literal) \
    char name[length] = literal; \
    static_assert(sizeof(literal) == (length), #name " length differs from the extract manifest");
#include <kotor_generated/window/kotor_window_original_data.generated.h>
#undef KOTOR_ORIGINAL_CSTRING
}
namespace kotor_aurora_window {
// 007B9226: editor "programs created" byte; only InitEditor reads/writes it (0044E696/0044E6A3).
volatile std::uint8_t editor_programs_007b9226 = 0;
}
extern "C" std::int32_t __fastcall genuine_editor_0044e3b0(Renderer*, void*, U32, U32, U32, U32, U32, U32, U32);

namespace {
template<class T> void put(U8* p, T v) { std::memcpy(p, &v, sizeof v); }
template<class T> T get(const U8* p) { T v; std::memcpy(&v, p, sizeof v); return v; }
const char* record_source(const ProgramBinding& b) {
    return *shared.programs_0078e5ec != 0 ? (b.dynamic_nonzero_source ? *b.dynamic_nonzero_source : b.nonzero_source)
                                          : b.zero_source;      // e.g. 0044E6E8..0044E6F8
}
// One allocate/construct/store/enable block (0044E6C8..0044E725 pattern).
void create(char* source, U32* parameters, void*& output, U8 second) {
    void* program = live_operator_new(0x201c);                       // push 201Ch; call 006FA7E6
    if (program) {
        try {                                                         // EH state n: free(allocation)
            program = construct_program_004a27d0(program, nullptr, source, parameters); // call 004A27D0
        } catch (...) { live_free(program); throw; }
    }
    output = program;                                                 // mov [cell],eax (also when null)
    enable_program_004a24d0(program, nullptr, nullptr, second);       // push second; push 0; call 004A24D0
}
}

extern "C" std::int32_t __fastcall genuine_editor_0044e3b0(Renderer* self, void*, U32 top_window, U32 window_word,
    U32 width, U32 height, U32 bits, U32 frequency, U32 fullscreen) {
    (void)top_window;                                                 // [ebp+8] never read
    HWND window = reinterpret_cast<HWND>(static_cast<std::uintptr_t>(window_word)); // ebx=[ebp+0Ch]
    (void)ShowWindow_0073d400(window, 0);                             // 0044E3D7..0044E3E0
    U32 style; U32 insert_after;
    if (fullscreen != 0) {                                            // 0044E3E6..0044E3EB
        DisplayMode mode{}; put<U16>(mode.bytes + 36, 156);           // 0044E3F1..0044E407
        U32 index = 0;
        if (EnumDisplaySettingsA_0073d3e4(nullptr, index, &mode)) {   // 0044E411
            for (;;) {
                if (get<U32>(mode.bytes + 108) == width && get<U32>(mode.bytes + 112) == height &&
                    get<U32>(mode.bytes + 104) == static_cast<U8>(bits) && get<U32>(mode.bytes + 120) == frequency) { // 0044E420..0044E44F
                    if (ChangeDisplaySettingsA_0073d3e8(&mode, 2) != 0) return 0; // 0044E47C..0044E48B -> 0044E5D8
                    std::memcpy(self->saved_mode.bytes, mode.bytes, sizeof mode.bytes); // 0044E491..0044E4A1
                    (void)ChangeDisplaySettingsA_0073d3e8(&mode, 4);  // 0044E4A3..0044E4AA
                    self->mode_changed = 1;                           // 0044E4B4
                    break;
                }
                std::memset(mode.bytes, 0, sizeof mode.bytes); put<U16>(mode.bytes + 36, 156); // 0044E451..0044E466
                ++index;                                              // 0044E458 inc esi
                if (!EnumDisplaySettingsA_0073d3e4(nullptr, index, &mode)) break; // 0044E470..0044E47A
            }
        }
        if (self->mode_changed == 0) {                                // 0044E4BE..0044E4CA
            (void)ShowWindow_0073d400(window, 1);                     // 0044E4CC..0044E4CF
            return 0;
        }
        style = 0x80000000u; insert_after = 0xffffffffu;              // 0044E4EE..0044E4F6 (HWND_TOPMOST)
        self->fullscreen_style = 1;                                   // 0044E4FE
    } else {
        style = 0x40000000u; insert_after = 0xfffffffeu;              // 0044E50A..0044E512 (HWND_NOTOPMOST)
        self->fullscreen_style = 0;                                   // 0044E51A
    }
    void* dc = GetDC_0073d408(window);                                // 0044E524..0044E525
    if (!dc) return 0;                                                // 0044E52D..0044E531 -> 0044E9BF (EAX 0)
    PixelFormat pfd{};                                                // 0044E537 rep stos 0Ah dwords
    put<U16>(pfd.bytes, 40); put<U16>(pfd.bytes + 2, 1); put<U32>(pfd.bytes + 4, 0x25); // 0044E556..0044E564
    pfd.bytes[8] = 0; pfd.bytes[9] = 0x20; pfd.bytes[23] = 0x20;      // 0044E542..0044E54C
    pfd.bytes[24] = 8; pfd.bytes[26] = 0;                             // 0044E571 / 0044E56C
    const I32 format = ChoosePixelFormat_0073d018(dc, &pfd);          // 0044E576
    if (format == 0 || SetPixelFormat_0073d014(dc, format, &pfd) == 0) { // 0044E57E..0044E591
        (void)ReleaseDC_0073d404(window, dc);                         // 0044E5D0..0044E5D2
        return 0;
    }
    (void)DescribePixelFormat_0073d008(dc, format, 40, &pfd);         // 0044E593..0044E59C (result unused)
    self->context = wglCreateContext_0073d2b8(dc);                    // 0044E5A2..0044E5AF
    if (!self->context) { (void)ReleaseDC_0073d404(window, dc); return 0; } // 0044E5B5 -> 0044E5D0
    if (wglMakeCurrent_0073d2b0(dc, self->context) == 0) {            // 0044E5B7..0044E5C1
        (void)wglDeleteContext_0073d30c(self->context);               // 0044E5C3..0044E5CA (+B4 reread, kept)
        (void)ReleaseDC_0073d404(window, dc);
        return 0;
    }
    (void)ReleaseDC_0073d404(window, dc);                             // 0044E5F1..0044E5F3
    Rect rect{0, 0, width, height};                                   // 0044E603..0044E61F
    (void)AdjustWindowRect_0073d3e0(&rect, style, 0);                 // 0044E623
    (void)SetWindowLongA_0073d3ec(window, -16, style);                 // 0044E629..0044E631
    (void)SetWindowPos_0073d40c(window, reinterpret_cast<void*>(static_cast<std::uintptr_t>(insert_after)), 0, 0,
        rect.right - rect.left, rect.bottom - rect.top, 0x120);       // 0044E637..0044E65C
    (void)SetWindowPos_0073d40c(window, nullptr, 0, 0, width, height, 0x120); // 0044E662..0044E670
    (void)SendMessageA_0073d428(window, 5, 0, ((height & 0xffffu) << 16) | (width & 0xffffu)); // 0044E676..0044E687
    (void)ShowWindow_0073d400(window, 1);                             // 0044E68D..0044E690
    if (kotor_aurora_window::editor_programs_007b9226 != 0) return 1; // 0044E696..0044E69D
    kotor_aurora_window::editor_programs_007b9226 = 1;                // 0044E6A3
    LoadExtensions_00436490();                                        // 0044E6AA
    if (VertexProgramsAvailable_0045f770() == 0) return 1;            // 0044E6AF..0044E6B6
    glEnable_0073d390(*shared.vertex_enum_0078d46c);                  // 0044E6BC..0044E6C2
    const ProgramBinding* r = shared.programs;
    create(const_cast<char*>(record_source(r[0])), *r[0].constructor_parameters, *r[0].output, 0); // 0044E6C8..0044E725 (0083037C)
    create(const_cast<char*>(record_source(r[1])), *r[1].constructor_parameters, *r[1].output, 1); // 0044E72A..0044E783 (00830380)
    create(renderer_program_cells::source_0079C310, renderer_program_cells::parameters_00830580,
           kotor_aurora_window::output_00830384, 1);                  // 0044E788..0044E7D1 (unconditional source)
    create(const_cast<char*>(record_source(r[2])), *r[2].constructor_parameters, *r[2].output, 1); // 0044E7D6..0044E82F (00827FEC)
    // 0044E834..0044E88D: record 3's parameters (00830560) and output (00827FF0), but record 2's
    // sources (0078FEB8 / 0079AB28) - Initialize uses 00790E70 for this slot, InitEditor does not.
    create(const_cast<char*>(record_source(r[2])), *r[3].constructor_parameters, *r[3].output, 0);
    create(const_cast<char*>(record_source(r[4])), *r[4].constructor_parameters, *r[4].output, 1); // 0044E892..0044E8EB (00827FF4)
    create(const_cast<char*>(record_source(r[5])), *r[5].constructor_parameters, *r[5].output, 0); // 0044E8F0..0044E94A (00827FF8)
    create(const_cast<char*>(record_source(r[6])), *r[6].constructor_parameters, *r[6].output, 0); // 0044E94F..0044E9A9 (00827FFC)
    glDisable_0073d22c(*shared.vertex_enum_0078d46c);                 // 0044E9AE..0044E9B4
    return 1;                                                         // 0044E9BA
}
