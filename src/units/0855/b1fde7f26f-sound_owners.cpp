// lane-m48-b REPLACEMENT COPY of the M47 stage unit sound-owners (sound/sound_owners.cpp). Only change: `extern` on the seven
// .rdata sound-mode words. A namespace-scope `const` object declared inside an `extern "C" { ... }` block still has
// internal linkage in C++ ([basic.link]/3), so M47 emitted no external _kotor_rdata_sound_mode_0074c5f8 and its three
// consumers (game_prepare.cpp:183, intro_queue.cpp:7, d1_table_menu_normal.cpp:19) were unresolved. Values unchanged.
// Lane-sound single owners for sound globals (values from the original image,
// swkotor.exe sha256 9c10e045...1435) and the audio::providers publisher.
#include "5e08d5e1a4-sound_path.h"
#include "ed8f77ad6d-allocator_live.h"
#include <cstdint>

using Word = std::uint32_t;

extern "C" {
// 007A39EC CExoSound* cell: .data file image 00000000 (raw offset 0x3A39EC);
// written 005F23A8, read e.g. 005F2432/005F569E. Same declaration lane-prepare
// consumes (game_prepare_producers.h: extern void* kotor_cell_sound_007a39ec).
void* kotor_cell_sound_007a39ec = nullptr;
// 00832894 Disable Sound word: .bss (beyond raw data), loader zero. Written by
// WinMain 00404382 and CExoSoundInternal::Initialize 005DAAC9.
volatile Word kotor_cell_disable_sound_00832894 = 0;
// 00832890 sound pause word: .bss, loader zero; written 005D88B9.
volatile Word kotor_cell_sound_pause_00832890 = 0;
// .rdata sound-mode words 0074C5EC..0074C604, image values 0..6.
extern const Word kotor_rdata_sound_mode_0074c5ec = 0;
extern const Word kotor_rdata_sound_mode_0074c5f0 = 1;
extern const Word kotor_rdata_sound_mode_0074c5f4 = 2;
extern const Word kotor_rdata_sound_mode_0074c5f8 = 3;
extern const Word kotor_rdata_sound_mode_0074c5fc = 4;
extern const Word kotor_rdata_sound_mode_0074c600 = 5;
extern const Word kotor_rdata_sound_mode_0074c604 = 6;
}
// Donors name the 007A39EC cell table29_canonical_required::sound_007a39ec; one
// storage, joined by the linker (D9-style alternatename, no second object).
#pragma comment(linker, "/alternatename:?sound_007a39ec@table29_canonical_required@@3PAXA=_kotor_cell_sound_007a39ec")
// 0063E7F0 (XOR EAX,EAX; RET) shared leaf: pause donor's stdcall name joined to the
// existing M41 body _c2da_resource_constant_word_0063e7f0@0 (constants.obj).
#pragma comment(linker, "/alternatename:?zero_0063e7f0@sound_pause29_required@@YGIXZ=_c2da_resource_constant_word_0063e7f0@0")

// Views consumed by existing donors (lifecycle, pause/resume, sound-mode, this lane).
namespace sound_lifecycle29_required { extern volatile std::uint32_t* const word_00832894 = &kotor_cell_disable_sound_00832894; }
namespace sound_pause29_required { extern volatile std::uint32_t* const word_00832890 = &kotor_cell_sound_pause_00832890; }
namespace sound_mode29_required {
extern const volatile std::uint32_t* const word_0074c5ec = &kotor_rdata_sound_mode_0074c5ec;
extern const volatile std::uint32_t* const word_0074c5f0 = &kotor_rdata_sound_mode_0074c5f0;
extern const volatile std::uint32_t* const word_0074c5f4 = &kotor_rdata_sound_mode_0074c5f4;
extern const volatile std::uint32_t* const word_0074c5f8 = &kotor_rdata_sound_mode_0074c5f8;
extern const volatile std::uint32_t* const word_0074c5fc = &kotor_rdata_sound_mode_0074c5fc;
extern const volatile std::uint32_t* const word_0074c600 = &kotor_rdata_sound_mode_0074c600;
extern const volatile std::uint32_t* const word_0074c604 = &kotor_rdata_sound_mode_0074c604;
}

// audio::providers is statically bound in audio_retyped.cpp (all 19 slots).
// Duplicate carriers for one original body (D9-style joins, no second definition):
// 005D7AB0 / 005D7A00 bodies live in sound_lists.cpp under the EAX-returning names;
// stream_play's void carriers ignore EAX (ABI-identical: ECX, EDX, one stack DWORD, RET 4).
#pragma comment(linker, "/alternatename:?remove_attached_005d7ab0@sound_stream_play29_required@@YIXPAX00@Z=?remove_streaming_005d7ab0@kotor_sound_path_required@@YIIPAX00@Z")
#pragma comment(linker, "/alternatename:?remove_other_005d7a00@sound_stream_play29_required@@YIXPAX00@Z=?remove_nonplaying_streaming_005d7a00@kotor_sound_path_required@@YIIPAX00@Z")
extern "C" {
// 007A2374: .data image value 00000001, never written by code (read 005DBD4E, 005DF230).
extern volatile std::uint32_t kotor_cell_stream_mode_007a2374;
volatile std::uint32_t kotor_cell_stream_mode_007a2374 = 1;
}
