// lane-m1-movies (Milestone 1, L2): client leaves reached from the Movies button.
//   005EE120 CClientExoApp::BusyWaitMenuMusic          005F4A60 CClientExoAppInternal body (unnamed in the exports)
//   0061D3D0 CClientOptions::GetMovieShown(CResRef const&)
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only; nothing executed.
// Bound existing M49 owners: M40 timer/divide/base views (startup_views.h), lane-sound cell 007A39EC and CExoSound::
// Render 005D5DD0, sound-donor-fade FadeAndStop 005D5B90, m48b-client-dtors Stop 005D5B80, ref-to-string 00405F70,
// lowers CompareNoCase 005E6450, lane-config C2DA::GetRowIndex 004138B0, lane-prepare CSWRules cell 007A3A28.
#include "ca04f22e44-movies_m1.h"
#include "e347588a95-startup_views.h"     // M40: kotor_menu_10_19::TimerDependency / dependencies::{base_007a39e0, unsigned_divide_006fb030}
#include <cstdint>
#include <cstring>

using kotor_reconstruction::StartupString;

// ---------------------------------------------------------------- existing owners (declarations repeated exactly)
namespace sound_fade29 { void __fastcall Begin005d5b90(void* outer, void*, std::uint32_t duration); }  // 005D5B90 FadeAndStop
namespace sound_fade29_required { void __fastcall update_005d5dd0(void*, void*); }                     // 005D5DD0 CExoSound::Render
namespace options_video { struct String; }
namespace client_movies { std::int32_t __fastcall C2daGetRowIndex_004138b0(void* table, void* unused_edx,
                                                                           options_video::String* label); }  // lane-config
extern "C" {
void __fastcall kotor_fastcall_005d5b80(void* source, void*);                         // 005D5B80 CExoStreamingSoundSource::Stop
extern void* kotor_cell_sound_007a39ec;                                               // .data 007A39EC CExoSound* (lane-sound, D25)
extern void* kotor_cell_rules_007a3a28;                                               // .data 007A3A28 CSWRules* (lane-prepare)
void __fastcall c2da_ref_to_string_00405f70(void* ref, void*, StartupString* out);    // 00405F70 CResRef::CopyToString (RET 4)
std::int32_t __fastcall exostring_compare_no_case_005e6450(StartupString*, void*, StartupString*); // 005E6450 CompareNoCase
}

namespace {
void* pointer_at(const void* object, unsigned offset) {
    void* value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, 4); return value;
}
// GetHighResolutionTimer 005E9260 on [[007A39E0]+4], then __aulldiv(ticks, 3E8h) 006FB030; only EAX (the low
// dword of the quotient) is used by 005F4A60 (005F4A60..7F, 005F4AA0..BC).
std::uint32_t milliseconds_low() {
    using namespace kotor_menu_10_19;
    TimerDependency* const timer = static_cast<TimerDependency*>(pointer_at(dependencies::base_007a39e0, 4));
    return static_cast<std::uint32_t>(dependencies::unsigned_divide_006fb030(timer->high_resolution_005e9260(), 1000));
}
}

extern "C" {
// ================================================================ 005F4A60 (CClientExoAppInternal, thiscall) RET
// Busy-waits 750 ms rendering the sound system; the menu music source (+238h) is faded out (2500 ms) first and
// stopped afterwards. The +238h word is read at 005F4A7F and re-read at 005F4AC5; the timer base and the sound
// cell are re-read on every pass.
void __fastcall kotor_fastcall_005f4a60(void* client_internal, void*) {
    const std::uint32_t start = milliseconds_low();                                  // 005F4A60..87 (esi)
    if (void* const music = pointer_at(client_internal, 0x238))                      // 005F4A7F..89
        sound_fade29::Begin005d5b90(music, nullptr, 0x9C4);                          // 005F4A8B..90 FadeAndStop(2500)
    do {
        sound_fade29_required::update_005d5dd0(kotor_cell_sound_007a39ec, nullptr);  // 005F4A95..9B CExoSound::Render
    } while (milliseconds_low() - start < 0x2EEu);                                   // 005F4AA0..C3 (sub; cmp 2EEh; jb)
    if (void* const music = pointer_at(client_internal, 0x238))                      // 005F4AC5..CF
        kotor_fastcall_005d5b80(music, nullptr);                                     // 005F4AD1 jmp Stop
}                                                                                    // 005F4AD6 ret

// ================================================================ 005EE120 CClientExoApp::BusyWaitMenuMusic() RET
void __fastcall kotor_fastcall_005ee120(void* client_app, void*) {
    kotor_fastcall_005f4a60(pointer_at(client_app, 4), nullptr);                     // 005EE120 mov ecx,[ecx+4]; jmp 005F4A60
}

// ================================================================ 0061D3D0 CClientOptions::GetMovieShown(CResRef const&) RET 4
// Returns 1 when the name equals the CExoString cell 008338D8 (CompareNoCase non-zero), 0 when the row is absent
// from the movies 2DA ([[[007A3A28]+B8h]+11Ch]) or the byte index (AL) is >= 0Bh, else the RAW masked bit
// byte[options+60h+index] & (1 << (row % 8)) (D12: not normalised).
gui_load::Word __fastcall kotor_fastcall_0061d3d0(void* client_options, void*, menu_resources::Ref* movie) {
    StartupString name;                                                              // [esp+4] 0061D3EB..3EF (EH state 0)
    c2da_ref_to_string_00405f70(movie, nullptr, &name);                              // 0061D3F4..405
    if (exostring_compare_no_case_005e6450(&name, nullptr,
            reinterpret_cast<StartupString*>(kotor_cell_movie_name_008338d8)) != 0)  // 0061D40A..41A
        return 1;                                                                    // 0061D41C..441 (~name)
    void* const movies = pointer_at(pointer_at(kotor_cell_rules_007a3a28, 0xB8), 0x11C);  // 0061D444..450
    const std::int32_t row = client_movies::C2daGetRowIndex_004138b0(movies, nullptr,
                                 reinterpret_cast<options_video::String*>(&name));   // 0061D456..45D
    if (row < 0) return 0;                                                           // 0061D464..46E jge / 0061D470..48A (~name)
    // 0061D48D..495: CDQ; AND EDX,7; ADD; SAR 3 -> signed row/8 (row >= 0 here); only AL is used below.
    const std::uint8_t index = static_cast<std::uint8_t>(row / 8);
    if (index >= 0x0B) return 0;                                                     // 0061D498..49A cmp al,0Bh; jae 0061D470
    const std::uint8_t shift = static_cast<std::uint8_t>(static_cast<std::uint8_t>(row) -
                                                         static_cast<std::uint8_t>(index << 3));  // 0061D49C..4A1 (CL - DL)
    std::uint8_t bits;
    std::memcpy(&bits, static_cast<const unsigned char*>(client_options) + 0x60 + index, 1);  // 0061D4A3..4A6 movzx
    const gui_load::Word mask = 1u << (shift & 31u);                                 // 0061D4AB..4B0 (SHL masks CL to 5 bits)
    return static_cast<gui_load::Word>(bits) & mask;                                 // 0061D4B6..4C1 (~name at 0061D4B8)
}
}
