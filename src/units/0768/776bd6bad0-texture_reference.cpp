// lane-m47-core: AurTextureGetReference 00423A60 (first-frame: lane-firstframe PATH.md 6d, GUI font
// SetFontTexture 0045BD70 -> GetReference -> AurTextureGet 00423490).
// Evidence: dumpbin /disasm swkotor.exe 00423A60..00423A86 (lane dis/00423a60.asm).
// Declaration: task-33 font.hpp (texture-epoch/include) `font::reference_00423a60` (extern "C" cdecl, two
// pointer words, caller cleanup, pointer EAX). The 3D consumers spell the same body
// `aur_texture_get_reference_00423a60(const char*, U)` (scene3d.h); that name is an ABI-identical
// composition join (/alternatename, D9 mechanism) -> see PACKET.json alternatename_or_join_needs.
#include "69638710e8-font.hpp"

namespace font {
extern "C" __declspec(noinline) Texture* __cdecl reference_00423a60(const char* name, const char* alternate) {
    // 00423A60..00423A70: AurTextureGet(name, nullptr, alternate, nullptr, 0)  (pushes 0,0,alternate,0,name)
    Texture* const found = get_00423490(name, nullptr, alternate, nullptr, 0u);
    // 00423A75..00423A7E: AurTextureGet(nullptr, found, nullptr, nullptr, 0)  (pushes 0,0,0,EAX,0)
    // 00423A83: add esp,28h (both calls' 5 words) ; RET 0, EAX = second result.
    return get_00423490(nullptr, found, nullptr, nullptr, 0u);
}
}
