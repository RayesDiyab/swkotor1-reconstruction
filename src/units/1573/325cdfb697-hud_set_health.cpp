// Private reconstruction of original 006857D0 CSWGuiMainInterfaceChar::SetHealth.
//
// Original ABI: thiscall(self, int current, int max, int kind), ECX=self,
// three stack arguments, callee RET 0xC. Reconstruction export:
// @kotor_fastcall_006857d0@20 (dummy EDX, three stack args, generated RET 0xC).
// No SEH, no heap, no destructor; the local is a 16-byte CResRef POD.
//
// Selected providers:
//   00417F90  @gui_m48_progress_set_max_00417f90@12
//   00417FB0  ?kotor_fastcall_00417fb0@kotor_original@@YIXPAX0H@Z
//   00406D80  ?construct_00406d80@Ref@menu_resources@@QAEPAU12@PBD@Z
//   00414C00  @gui_load_border_params_set_fill_00414c00@16
//
// Source-only private proposal; not built/run. No native or runtime acceptance.
#include "da79f00dee-w4_providers.hpp"

#include <cstdint>

using w4::U32;

extern "C" void __fastcall gui_m48_progress_set_max_00417f90(void*, void*, int);
extern "C" void __fastcall gui_load_border_params_set_fill_00414c00(void*, void*, const void*, int);

namespace kotor_original {
void __fastcall kotor_fastcall_00417fb0(void*, void*, int);
}

namespace menu_resources {
struct Ref {
    U32 a, b, c, d;  // 16-byte CResRef; scalar members avoid /GS array shape
    Ref* __thiscall construct_00406d80(const char* text);
};
static_assert(sizeof(Ref) == 16, "16-byte CResRef local");
}

namespace {
void* atv(void* p, U32 off) {
    return reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(p) + off);
}
}

extern "C" void __fastcall kotor_fastcall_006857d0(
    void* self, void*, int current, int max, int kind) {
    // 006857E3/E6: SetMax(self+0x7A8, max), then 006857EF/F2 SetValue(..., current).
    gui_m48_progress_set_max_00417f90(atv(self, 0x7A8), nullptr, max);
    kotor_original::kotor_fastcall_00417fb0(atv(self, 0x7A8), nullptr, current);

    // 00685809/E: kind != 0 -> greenfill (0x753B14), else redfill (0x753B0C).
    menu_resources::Ref ref;
    ref.construct_00406d80(kind != 0 ? "greenfill" : "redfill");

    // 0068581C/20: SetFillImage(self+0x898, &ref, 0).
    gui_load_border_params_set_fill_00414c00(atv(self, 0x898), nullptr, &ref, 0);

    // 0068582A/2F/35: current at +0, max at +4, updated marker at +0xEA4.
    w4::sw(self, 0x00, static_cast<U32>(current));
    w4::sw(self, 0x04, static_cast<U32>(max));
    w4::sw(self, 0xEA4, 1);
}
