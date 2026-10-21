#pragma once
#include <cstdint>
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 source/physical ABI only
#endif
// Borrowed outer+0 is nullable. A live internal has ten raw mode words at
// +0x118, depth at+0x140 and latch at+0x144. Depth must be 0..9 at each read,
// including after helper callbacks. Nil outer and malformed objects are held.
namespace table29_required {
void __fastcall gui_sound_mode_005d5e80(void* outer,void* unused_edx,std::uint32_t mode);
}
// Task29 owns these four original helper bodies. These are declarations only;
// the production object does not supply substitutes or imply their completion.
namespace sound_mode29 {
std::uint32_t __fastcall PauseAll005d82c0(void* internal,void* unused_edx,std::uint32_t all);
std::uint32_t __fastcall ResumeAll005d83f0(void* internal,void* unused_edx);
std::uint32_t __fastcall Mute005d6a10(void* internal,void* unused_edx);
std::uint32_t __fastcall Unmute005d6a50(void* internal,void* unused_edx);
}
// Proposed read views for seven genuine original word cells. Main6 owns their
// canonical data/storage identities and must review these names and qualifiers.
// No data, pointer slot, canonical cell or initializer is defined in production.
namespace sound_mode29_required {
extern const volatile std::uint32_t* const word_0074c5ec;
extern const volatile std::uint32_t* const word_0074c5f0;
extern const volatile std::uint32_t* const word_0074c5f4;
extern const volatile std::uint32_t* const word_0074c5f8;
extern const volatile std::uint32_t* const word_0074c5fc;
extern const volatile std::uint32_t* const word_0074c600;
extern const volatile std::uint32_t* const word_0074c604;
}
static_assert(sizeof(void*)==4 && sizeof(std::uint32_t)==4,"observed x86 words");
