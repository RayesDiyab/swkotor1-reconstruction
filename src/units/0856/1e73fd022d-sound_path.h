#pragma once
// Lane-sound: small pre-first-frame sound bodies (swkotor.exe GOG, base 0x400000).
// Every body uses the existing sound-lane carrier convention: __fastcall with
// ECX = receiver, EDX ignored, then the original stack DWORDs. This is ABI
// identical to the original __thiscall (ECX receiver, callee stack purge).
// No storage, provider table, fixture or fallback is defined in this unit.
#include <cstdint>
#include "f9f7ab0530-startup_string.h"      // kotor_reconstruction::StartupString (005E5A90/005E5C20)
#include "15d7bcda8b-resources.h"           // menu_resources::Ref (16 bytes, byte aligned)
#include "e4af213db3-ref_string_joins.h"    // async_string_cstr_005e5670 / async_string_length_005e5790
#include "29bc052957-normal.h"

namespace kotor_sound_path {
using U = std::uint32_t;
using String = kotor_reconstruction::StartupString;
using Ref = menu_resources::Ref;
using Client = menu_normal_40_49::MusicInternal;   // existing 0x30C client view

// CExoSound (4-byte object: +0 internal pointer).
void* __fastcall SoundConstruct005d5bc0(void* self, void*, U voices2d, U voices3d, U eax_mode, U hardware); // RET 10h
U __fastcall SoundSetBias005d6320(void* self, void*, float bias);   // RET 4; EAX not reproduced (see .cpp)
float __fastcall SoundGetBias005d6380(void* self, void*);           // ST0, RET 0
U __fastcall SoundVoices2d005d63a0(void* self, void*);              // AL meaningful, RET 0
U __fastcall SoundVoices3d005d63b0(void* self, void*);              // AL meaningful, RET 0
U __fastcall SoundSetStreamingEffects005d6430(void* self, void*, U value);    // RET 4
U __fastcall SoundSetNonStreamingEffects005d6440(void* self, void*, U value); // RET 4
U __fastcall SoundGetEax005d6450(void* self, void*);                // RET 0
// 005D5E80 (+005D8560): body is the reviewed donor table29_required::gui_sound_mode_005d5e80
// (D2 task-29 sound-mode-closure-r2); not duplicated here.

// CExoSound volume setters (sound_render.cpp). Null internal (Disable Sound=1): return, no stores.
void __fastcall SoundSetMusicVolume005d5c50(void* self, void*, float volume, U apply_group); // RET 8
void __fastcall SoundSetSoundEffectVolume005d5cd0(void* self, void*, float volume);       // RET 4
void __fastcall SoundSetDialogVolume005d5d50(void* self, void*, float volume);            // RET 4

// CClientExoAppInternal menu music.
void __fastcall StartMenuMusic005f9af0(Client* client, void*, U theme, U volume, U priority); // RET 0Ch
void __fastcall StopMenuMusic005f6440(Client* client, void*);       // RET 0

// CResRef::operator!=(CExoString const&) and CExoString::IsEmpty.
U __fastcall RefDiffersString00406de0(const Ref* self, void*, String* text); // RET 4
U __fastcall StringIsEmpty005e58a0(String* self, void*);            // RET 0
}

// Bodies authored here under names already declared by existing sound packets
// (or following their table29_required naming for the missing wrappers).
namespace sound_fade29_required {
void __fastcall update_005d5dd0(void*, void*);                      // CExoSound::Render thunk, RET 0
}
namespace table29_required {
std::uint32_t __fastcall gui_play_005d5b10(void*, void*, menu_resources::Ref*,
    std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);    // RET 14h
void __fastcall gui_volume_005d5b50(void*, void*, std::uint32_t);   // RET 4
menu_resources::Ref* __fastcall gui_file_playing_005d6230(void*, void*, menu_resources::Ref*); // RET 4

// Existing donors (declarations repeated verbatim, bodies NOT here):
void* __fastcall gui_sound_construct_005d5a50(void*, void*);        // task-5 sound-lifecycle-r8-private-r2
void __fastcall gui_sound_destroy_005d6200(void*, void*);           // same
std::uint32_t __fastcall gui_playing_005d5af0(void*, void*);        // task-29 sound-music-closure-r1
void __fastcall gui_loop_005d5b40(void*, void*, std::uint32_t);     // same
void __fastcall gui_priority_005d5ba0(void*, void*, std::uint32_t); // task-5 sound-volume-priority-r8r5
void __fastcall gui_fade_005f1b20(menu_normal_40_49::MusicInternal*, void*, void*, std::uint32_t); // sound-fade-r8
}
namespace sound_stream_play29 {   // task-5 sound-stream-play-private-r5
std::uint32_t __fastcall Play005dca30(void*, void*, menu_resources::Ref*,
    std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
}
namespace sound_lower29_required { // body: sound-volume-priority-r8r5 volume_priority.cpp
void __fastcall volume_005dc930(void*, void*, std::uint32_t, std::uint32_t, std::uint32_t);
}
namespace sound_lifecycle29_required { // genuine Disable Sound word owner (WinMain 00404382); no storage here
extern volatile std::uint32_t* const word_00832894;
}
namespace base_ini_reconstruction { // M40 ini-local-warning-literals.obj (0073D71C "")
extern const char warning_empty_0073d71c[];
}
extern "C" menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(
    menu_resources::Ref*, void*, menu_resources::String*);          // M40 ref-bodies.obj

// CExoSoundInternal ctor/Initialize: lane-sound sound_internal.cpp. Render: sound_render.cpp.
namespace kotor_sound_internal {
void* __fastcall Construct005d86e0(void* self, void*);
std::uint32_t __fastcall Initialize005da8a0(void* self, void*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall Render005d9590(void* self, void*);
}
