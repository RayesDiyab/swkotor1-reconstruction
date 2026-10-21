// lane-startup-misc: the two INI-conditional renderer setters of the menu_providers_40_49
// table (slots +08 / +0C, lane-menu-tables d1_table_menu_providers.cpp) and the swap-control
// predicate 0045F580. swkotor.exe GOG K1, base 0x400000. GL/WGL cells and the anisotropic
// predicate come from their single owner lane-renderer-bodies (renderer_gl_cells.h /
// renderer_gl_procs.h / renderer_gl_loader_lowers.cpp); the anisotropy level word 007A685C
// from lane-upload (upload_cells.cpp). No GL import is called by these bodies.
#include "f6e86afef9-renderer_gl_cells.h"   // kotor_renderer_gl_cells (lane-renderer-bodies storage owner)
#include "7cd3e01780-renderer_gl_procs.h"   // kotor_renderer_gl_procs::{mask_0078e4f0, wglSwapIntervalEXT_007bb538}
#include "e83b290bda-extension_loader.h"    // kotor_renderer_init::AnisotropicAvailable_0045f8e0
#include <cstdint>
#include <type_traits>

namespace {
template<class T> T load(T& cell) { return *static_cast<volatile T*>(&cell); }
template<class T> void store(T& cell, T value) { *static_cast<volatile T*>(&cell) = value; }
}

// 007B9278: anisotropy level shadow (BSS 0). Writer 0044F2E3 (this file); reader 0044F2E9.
// No other owner in M41 or the lane packets read for this lane.
namespace kotor_startup_misc_cells { std::int32_t anisotropy_007b9278 = 0; }

namespace kotor_renderer_gl {
// 0045F580 (Ghidra "glSetVSync?"; capability row "WGL_EXT_swap_control", 004368B2):
// eax=[0078E4F0]; ecx=[007BB85C]&eax; sete al; ret. Only AL is defined (upper EAX = mask bytes);
// the sole caller 0044EE95 tests AL.
std::uint8_t __stdcall SwapControlAvailable_0045f580() {
    const std::uint32_t mask = load(kotor_renderer_gl_procs::mask_0078e4f0);                    // 0045F580
    return (load(kotor_renderer_gl_cells::capabilities_007bb85c) & mask) == mask ? 1u : 0u;     // 0045F585..0045F58F
}
}

namespace menu_tables_required {
// 0044F2C0 AurSetAnisotropy(int level), cdecl, EAX = level or the stored shadow.
int __cdecl SetAnisotropy_0044f2c0(int level) {
    if (kotor_renderer_init::AnisotropicAvailable_0045f8e0() != 0) {                            // 0044F2C0..0044F2C7 full EAX
        // fild [esp+4]; fcomp dword [007A6858]; test ah,41h; jp -> skip when greater or unordered.
        // int32 -> double and float -> double are exact, as the x87 compare is.
        const double requested = static_cast<double>(level);
        const double maximum = static_cast<double>(load(kotor_renderer_gl_cells::max_anisotropy_007a6858));
        if (requested < maximum || requested == maximum) {
            store(texture_upload_cells::anisotropy_007a685c, static_cast<std::int32_t>(level));  // 0044F2DE
            store(kotor_startup_misc_cells::anisotropy_007b9278, static_cast<std::int32_t>(level)); // 0044F2E3
            return level;                                                                         // 0044F2E8 EAX = arg
        }
    }
    return load(kotor_startup_misc_cells::anisotropy_007b9278);                                    // 0044F2E9
}
// 0044EE90 AurSetVSyncState(unsigned char), cdecl. EAX is not defined on return (void).
void __cdecl SetVSyncState_0044ee90(unsigned char enabled) {
    if (kotor_renderer_gl::SwapControlAvailable_0045f580() == 0) return;                          // 0044EE90..0044EE97 AL
    const kotor_renderer_gl_procs::PFN_wglSwapIntervalEXT swap =
        load(kotor_renderer_gl_procs::wglSwapIntervalEXT_007bb538);                               // 0044EEA6 call [007BB538]
    (void)swap(enabled != 0 ? 1 : 0);                                                             // 0044EE9E..0044EEA5 setne al
    store(kotor_renderer_gl_cells::vsync_0078d438, static_cast<kotor_renderer_gl_cells::U8>(enabled)); // 0044EEAC after the call
}
}
static_assert(std::is_same<decltype(&menu_tables_required::SetAnisotropy_0044f2c0), int(__cdecl*)(int)>::value, "+08 slot");
static_assert(std::is_same<decltype(&menu_tables_required::SetVSyncState_0044ee90), void(__cdecl*)(unsigned char)>::value, "+0C slot");
