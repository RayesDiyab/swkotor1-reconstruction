// lane-w3 unit w3-client-sound: the CSWCAmbientSound pieces that CSWCArea construction / LoadArea / UnloadArea reach.
//   0068DF40 CSWCAmbientSound::CSWCAmbientSound(CSWCArea*)     0068D3D0 CSWCAmbientSoundTrack::CSWCAmbientSoundTrack
//   0068EE80 CSWCAmbientSound::LoadFromMessage                 0068D860 CSWCAmbientSound::ReadInfo
//   0068D690 CSWCAmbientSound::UpdateTimeOfDay                 0068E1B0 CSWCAmbientSound::StopSounds
//   0068E1F0 CSWCAmbientSound::PlayMusic (vtbl +00)            0068EA10 CSWCAmbientSound::PlayAmbientSound (vtbl +18)
// The streaming sound sources, 2DA access and string/resref helpers are REAL runtime1115 owners.
#include "5bf2d5f03e-w3_client_area.h"

using namespace kotor_w3;

namespace table29_required {
void* __fastcall gui_sound_construct_005d5a50(void*, void*);
void __fastcall gui_loop_005d5b40(void*, void*, unsigned);
void __fastcall gui_priority_005d5ba0(void*, void*, unsigned);
unsigned __fastcall gui_playing_005d5af0(void*, void*);
unsigned __fastcall gui_play_005d5b10(void*, void*, menu_resources::Ref*, unsigned, unsigned, unsigned, unsigned);
void __fastcall gui_volume_005d5b50(void*, void*, unsigned);
menu_resources::Ref* __fastcall gui_file_playing_005d6230(void*, void*, menu_resources::Ref*);
}
namespace sound_fade29 { void __fastcall Begin005d5b90(void*, void*, unsigned); }
namespace alias_add {
kotor_reconstruction::StartupString* __fastcall Upper005e6360(kotor_reconstruction::StartupString*, void*,
                                                              kotor_reconstruction::StartupString*);
}
extern "C" {
void __fastcall kotor_fastcall_005d5b80(void* source, void*);                                    // 005D5B80 Stop
void* __fastcall async_ref_construct_string_00406d60(void* self, void*, const void* string);     // 00406D60 CResRef(CExoString&)
int __fastcall exostring_compare_no_case_005e6450(void* self, void*, const void* other);         // 005E6450 RET 4
// explicit traps for the script-driven table slots (w3_traps.cpp style, defined below)
void __fastcall recovered_client_ambient_sound_set_music_delay_0068d510(void*, void*, std::uint32_t);
void __fastcall recovered_client_ambient_sound_set_music_day_track_0068e490(void*, void*, std::uint32_t);
void __fastcall recovered_client_ambient_sound_set_music_night_track_0068e550(void*, void*, std::uint32_t);
void __fastcall recovered_client_ambient_sound_play_battle_music_0068e610(void*, void*, std::uint32_t);
void __fastcall recovered_client_ambient_sound_set_battle_music_track_0068e940(void*, void*, std::uint32_t);
void __fastcall recovered_client_ambient_sound_set_ambient_day_track_0068ed00(void*, void*, std::uint32_t);
void __fastcall recovered_client_ambient_sound_set_ambient_night_track_0068edc0(void*, void*, std::uint32_t);
void __fastcall recovered_client_ambient_sound_play_music_0068e1f0(void* self, void*, std::uint32_t on) noexcept(false);
void __fastcall recovered_client_ambient_sound_play_ambient_sound_0068ea10(void* self, void*, std::uint32_t on) noexcept(false);
void* __fastcall recovered_client_ambient_sound_track_construct_0068d3d0(void* self, void*) noexcept(false);
void __fastcall recovered_client_ambient_sound_read_info_0068d860(void* self, void*, std::uint32_t music, std::uint32_t row,
                                                                  void* track) noexcept(false);
extern const void* const recovered_client_ambient_sound_methods_007541c8[9];
}

namespace {
const char kEmpty_0073d71c[] = "";                                     // .rdata 0073D71C
const char kResource_0074c318[] = "Resource";                          // .rdata 0074C318
const char kStinger1_007541bc[] = "Stinger1";                          // .rdata 007541BC
const char kStinger2_007541b0[] = "Stinger2";                          // .rdata 007541B0
const char kStinger3_007541a4[] = "Stinger3";                          // .rdata 007541A4
using SlotOn = void (__fastcall*)(void*, void*, U);
void* source(void* self, unsigned offset) noexcept { return rp(self, offset); }
// CExoStreamingSoundSource::Play(CResRef(track), priority_group, 0, 1, 0) on [this+E0h]; zero result -> [D0h] = 10000.
U play_track(void* self, unsigned source_offset, const void* track, U group) noexcept(false) {
    ResRef ref;                                                       // {E-0x10}
    async_ref_construct_string_00406d60(&ref, nullptr, track);        // lea ecx,[esp+8]; push track; call 00406D60
    return table29_required::gui_play_005d5b10(source(self, source_offset), nullptr,
                                               reinterpret_cast<menu_resources::Ref*>(&ref), group, 0, 1, 0);
}
}  // namespace

extern "C" {
const void* const recovered_client_ambient_sound_methods_007541c8[9] = {
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_play_music_0068e1f0),            // +00 0068E1F0 PlayMusic
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_set_music_delay_0068d510),       // +04 0068D510
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_set_music_day_track_0068e490),   // +08 0068E490
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_set_music_night_track_0068e550), // +0C 0068E550
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_play_battle_music_0068e610),     // +10 0068E610
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_set_battle_music_track_0068e940),// +14 0068E940
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_play_ambient_sound_0068ea10),    // +18 0068EA10 PlayAmbientSound
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_set_ambient_day_track_0068ed00), // +1C 0068ED00
    reinterpret_cast<const void*>(&recovered_client_ambient_sound_set_ambient_night_track_0068edc0),// +20 0068EDC0
};
// stale-stubs: the six script-driven slots, reconstructed from the original (were kotor_unreachable_trap stubs).
// 0068D510 CSWCAmbientSound::SetMusicDelay(int delay). RET 4.
void __fastcall recovered_client_ambient_sound_set_music_delay_0068d510(void* self, void*, U delay) {
    if (static_cast<I32>(delay) > 0) st<U>(self, 0xCC, delay);                          // 0068D510..0068D518 jle (signed)
}
}  // extern "C"
namespace {
// Shared shape of 0068E490/0068E550/0068E940/0068ED00/0068EDC0 (each RET 4, own EH frame 0072FEE8 = string dtor):
//   CExoString playing; playing = CResRef(source->GetFilePlaying(&tmp));
//   if (source->IsPlaying() && playing.CompareNoCase(track)) slot(0) [stopped=1];
//   ReadInfo(music, row, track); [field] = row; slot(1) (battle: only when stopped); music day/night: [D0h] = 1.
void set_track(void* self, U row, unsigned source_offset, unsigned track_offset, U music, unsigned slot,
               unsigned field, bool battle) noexcept(false) {
    ExoString playing;                                                // {esp+8} / battle {esp+0Ch}
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&playing), nullptr);
    try {
        ResRef tmp;                                                   // {esp+10h} out CResRef, no ctor/dtor
        c2da_ref_to_string_00405f70(table29_required::gui_file_playing_005d6230(source(self, source_offset), nullptr,
            reinterpret_cast<menu_resources::Ref*>(&tmp)), nullptr, &playing);
        bool stopped = false;                                         // battle: EBX (0068E9B0 / 0068E9FF)
        if (table29_required::gui_playing_005d5af0(source(self, source_offset), nullptr) != 0 &&
            exostring_compare_no_case_005e6450(&playing, nullptr, at(self, track_offset)) != 0) {
            vslot<SlotOn>(self, slot)(self, nullptr, 0);
            stopped = true;
        }
        recovered_client_ambient_sound_read_info_0068d860(self, nullptr, music, row, at(self, track_offset));
        st<U>(self, field, row);
        if (!battle || stopped) vslot<SlotOn>(self, slot)(self, nullptr, 1);
        if (music != 0 && !battle) st<U>(self, 0xD0, 1);              // 0068E522 / 0068E5E2
    } catch (...) {
        frame_LogStringDtor_005e5c20(&playing, nullptr);              // unwind state 0
        throw;
    }
    frame_LogStringDtor_005e5c20(&playing, nullptr);
}
}  // namespace
extern "C" {
// 0068E490 SetMusicDayTrack: source E0h, track 2Ch, ReadInfo(1), [0Ch]=row, vtbl+00 PlayMusic, [D0h]=1.
void __fastcall recovered_client_ambient_sound_set_music_day_track_0068e490(void* self, void*, U row) { set_track(self, row, 0xE0, 0x2C, 1, 0x00, 0x0C, false); }
// 0068E550 SetMusicNightTrack: source E0h, track 4Ch, [10h]=row.
void __fastcall recovered_client_ambient_sound_set_music_night_track_0068e550(void* self, void*, U row) { set_track(self, row, 0xE0, 0x4C, 1, 0x00, 0x10, false); }
void __fastcall object_ambient_0068e610(void*, void*, U);
void __fastcall recovered_client_ambient_sound_play_battle_music_0068e610(void* self, void*, U enable) { object_ambient_0068e610(self, nullptr, enable); }
// 0068E940 SetBattleMusicTrack: source E0h, track 6Ch, ReadInfo(1), [18h]=row, vtbl+10h PlayBattleMusic; restart only if stopped.
void __fastcall recovered_client_ambient_sound_set_battle_music_track_0068e940(void* self, void*, U row) { set_track(self, row, 0xE0, 0x6C, 1, 0x10, 0x18, true); }
// 0068ED00 SetAmbientDayTrack: source E4h, track 8Ch, ReadInfo(0), [20h]=row, vtbl+18h PlayAmbientSound.
void __fastcall recovered_client_ambient_sound_set_ambient_day_track_0068ed00(void* self, void*, U row) { set_track(self, row, 0xE4, 0x8C, 0, 0x18, 0x20, false); }
// 0068EDC0 SetAmbientNightTrack: source E4h, track ACh, ReadInfo(0), [24h]=row.
void __fastcall recovered_client_ambient_sound_set_ambient_night_track_0068edc0(void* self, void*, U row) { set_track(self, row, 0xE4, 0xAC, 0, 0x18, 0x24, false); }

// 0068D3D0 CSWCAmbientSoundTrack::CSWCAmbientSoundTrack: four CExoStrings (+0, +8, +10h, +18h). RET, EAX this.
void* __fastcall recovered_client_ambient_sound_track_construct_0068d3d0(void* self, void*) noexcept(false) {
    for (unsigned o = 0; o <= 0x18; o += 8)                           // 0068D3ED..0068D417
        kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(at(self, o)), nullptr);
    return self;                                                      // 0068D420
}

// 0068DF40 CSWCAmbientSound::CSWCAmbientSound(CSWCArea* area) (10Ch bytes). RET 4, EAX this.
void* __fastcall recovered_client_ambient_sound_construct_0068df40(void* self, void*, void* area) noexcept(false) {
    st<U>(self, 4, 1);                                                // 0068DF6D
    st<U>(self, 8, 0x1388);                                           // 0068DF70
    st<U>(self, 0xC, 2);                                              // 0068DF77
    st<U>(self, 0x10, 3);                                             // 0068DF7A
    st<U>(self, 0x14, 0);                                             // 0068DF81
    st<U>(self, 0x18, 1); st<U>(self, 0x1C, 1); st<U>(self, 0x20, 1); // 0068DF84..0068DF8A
    st<U>(self, 0x24, 2);                                             // 0068DF8D
    st<U8>(self, 0x28, 0); st<U8>(self, 0x29, 0);                     // 0068DF90..0068DF93
    st<const void*>(self, 0, recovered_client_ambient_sound_methods_007541c8);   // 0068DF9D
    for (unsigned o = 0x2C; o <= 0xAC; o += 0x20)                     // 0068DF96..0068DFDD five tracks
        recovered_client_ambient_sound_track_construct_0068d3d0(at(self, o), nullptr);
    st<U>(self, 0xD0, 0);                                             // 0068DFE9
    st<U>(self, 0xCC, 0x7530);                                        // 0068DFEF
    void* music = nullptr;                                            // 0068E017
    if (void* const memory = live_operator_new(4)) music = table29_required::gui_sound_construct_005d5a50(memory, nullptr);   // 0068DFE2..0068E010
    st<void*>(self, 0xE0, music);                                     // 0068E020
    void* ambient = nullptr;                                          // 0068E044
    if (void* const memory = live_operator_new(4)) ambient = table29_required::gui_sound_construct_005d5a50(memory, nullptr);   // 0068E019..0068E03D
    st<void*>(self, 0xE4, ambient);                                   // 0068E052
    table29_required::gui_loop_005d5b40(rp(self, 0xE0), nullptr, 0); // 0068E046..0068E058
    table29_required::gui_loop_005d5b40(rp(self, 0xE4), nullptr, 1); // 0068E05D..0068E064
    table29_required::gui_priority_005d5ba0(rp(self, 0xE4), nullptr, 4);   // 0068E069..0068E071
    st<U>(self, 0xEC, 1); st<U>(self, 0xF0, 1); st<U>(self, 0xF8, 1); // 0068E07E..0068E08A
    st<void*>(self, 0xE8, area);                                      // 0068E076..0068E090
    st<U>(self, 0xF4, 0); st<U>(self, 0xFC, 0);                       // 0068E096..0068E09C
    st<U>(self, 0x100, 0); st<U>(self, 0x104, 0); st<U>(self, 0x108, 0);   // 0068E0A2..0068E0AE
    return self;                                                      // 0068E0B5
}

// 0068D860 CSWCAmbientSound::ReadInfo(BOOL music, int row, CSWCAmbientSoundTrack* track) RET 0Ch.
void __fastcall recovered_client_ambient_sound_read_info_0068d860(void* self, void*, U music, U row, void* track) noexcept(false) {
    ExoString value;                                                  // {E-0x24}
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&value), nullptr);   // 0068D883..0068D887
    auto column_entry = [&](void* table, const char* column) noexcept(false) -> int {
        ExoString name;                                               // {E-0x2c} / {E-0x1c}
        frame_LogStringCtor_005e5a90(&name, nullptr, column);
        const int found = c2da_current_get_string_00413270(table, nullptr, static_cast<int>(row), &name, &value);
        frame_LogStringDtor_005e5c20(&name, nullptr);
        return found;
    };
    auto assign_upper_or_empty = [&](void* target, int found) noexcept(false) {
        if (found != 0) {
            ExoString upper;                                          // {E-0x1c}
            alias_add::Upper005e6360(reinterpret_cast<kotor_reconstruction::StartupString*>(&value), nullptr,
                                     reinterpret_cast<kotor_reconstruction::StartupString*>(&upper));
            async_assign_string_005e5c50(target, nullptr, &upper);
            frame_LogStringDtor_005e5c20(&upper, nullptr);
        } else {
            c2da_assign_text_005e5140(target, nullptr, kEmpty_0073d71c);
        }
    };
    if (music != 0) {                                                 // 0068D88C..0068D89C
        void* const table = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x50);   // 0068D8A2..0068D8AD ambientmusic
        assign_upper_or_empty(track, column_entry(table, kResource_0074c318));                     // 0068D8B0..0068D91D
        assign_upper_or_empty(at(track, 8), column_entry(table, kStinger1_007541bc));              // 0068D922..0068D994
        assign_upper_or_empty(at(track, 0x10), column_entry(table, kStinger2_007541b0));           // 0068D999..0068DA0B
        assign_upper_or_empty(at(track, 0x18), column_entry(table, kStinger3_007541a4));           // 0068DA10..0068DAFC
    } else {
        void* const table = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x54);   // 0068DA7E..0068DA8A ambientsound
        const int found = column_entry(table, kResource_0074c318);   // 0068DA8D..0068DABA
        assign_upper_or_empty(track, found);                          // 0068DABF..0068DAFC
        if (found != 0) st<U>(self, 0x108, 0);                        // 0068DAEC
    }
    frame_LogStringDtor_005e5c20(&value, nullptr);                    // 0068DB01..0068DB0D
}

// 0068EE80 CSWCAmbientSound::LoadFromMessage(CSWCMessage*) RET 4.
void __fastcall recovered_client_ambient_sound_load_from_message_0068ee80(void* self, void*, void* message) noexcept(false) {
    st<U>(self, 4, kotor_fastcall_004d66f0(message, nullptr));       // 0068EE88..0068EE91
    st<U>(self, 0x14, kotor_fastcall_004d66f0(message, nullptr));    // 0068EE94..0068EE9B
    st<U>(self, 0x1C, kotor_fastcall_004d66f0(message, nullptr));    // 0068EE9E..0068EEA7
    U delay = kotor_fastcall_004d6780(message, nullptr, 0x20);      // 0068EEA3..0068EEAA
    st<U>(self, 0xCC, delay);                                                     // 0068EEB2
    if (delay <= 1) delay = 1;                                                    // 0068EEAF..0068EEBA (unsigned ja)
    st<U>(self, 0xCC, delay);                                                     // 0068EEC3
    st<U>(self, 0xC, kotor_fastcall_004d6780(message, nullptr, 0x20));    // 0068EEBF..0068EED2 day music
    st<U>(self, 0x10, kotor_fastcall_004d6780(message, nullptr, 0x20));   // 0068EECE..0068EEDE night music
    st<U>(self, 0x18, kotor_fastcall_004d6780(message, nullptr, 0x20));   // 0068EEDA..0068EEEA battle music
    st<U>(self, 0x20, kotor_fastcall_004d6780(message, nullptr, 0x20));   // 0068EEE6..0068EEF6 day ambient
    st<U>(self, 0x24, kotor_fastcall_004d6780(message, nullptr, 0x20));   // 0068EEF2..0068EF02 night ambient
    st<U8>(self, 0x28, kotor_fastcall_004d6720(message, nullptr, 8));     // 0068EEFE..0068EF0E day volume
    st<U8>(self, 0x29, kotor_fastcall_004d6720(message, nullptr, 8));     // 0068EF0A..0068EF19 night volume
    recovered_client_ambient_sound_read_info_0068d860(self, nullptr, 1, rd32(self, 0xC), at(self, 0x2C));    // 0068EF16..0068EF25
    recovered_client_ambient_sound_read_info_0068d860(self, nullptr, 1, rd32(self, 0x10), at(self, 0x4C));   // 0068EF2A..0068EF36
    recovered_client_ambient_sound_read_info_0068d860(self, nullptr, 1, rd32(self, 0x18), at(self, 0x6C));   // 0068EF3B..0068EF47
    recovered_client_ambient_sound_read_info_0068d860(self, nullptr, 0, rd32(self, 0x20), at(self, 0x8C));   // 0068EF4C..0068EF5B
    recovered_client_ambient_sound_read_info_0068d860(self, nullptr, 0, rd32(self, 0x24), at(self, 0xAC));   // 0068EF60..0068EF6F
}

// 0068E1F0 CSWCAmbientSound::PlayMusic(BOOL on) RET 4 (table slot +00).
void __fastcall recovered_client_ambient_sound_play_music_0068e1f0(void* self, void*, U on) noexcept(false) {
    if (rd32(self, 0xF4) == 0) return;                                // 0068E1F6..0068E1FF
    if (rd32(self, 0xFC) != 0) {                                      // 0068E205..0068E20D
        st<U>(self, 4, on);                                           // 0068E21D
        st<U>(self, 0xD0, 0xA);                                       // 0068E220
        if (table29_required::gui_playing_005d5af0(source(self, 0xE0), nullptr) != 0) {   // 0068E217..0068E231
            const I32 state = ld<I32>(self, 0x100);                   // 0068E233
            if (state < 1) { if (on == 0) return; }                   // 0068E239..0068E240
            else if (state != 1 || on != 0) return;                   // 0068E248..0068E250
            sound_fade29::Begin005d5b90(source(self, 0xE0), nullptr, 0x3E8);   // 0068E256..0068E261 FadeAndStop(1000)
            return;
        }
        if (on == 0) return;                                          // 0068E26E..0068E270
        if (rd32(self, 0x14) != 0) {                                  // 0068E276..0068E27B battle
            if (play_track(self, 0xE0, at(self, 0x6C), 8) == 0) st<U>(self, 0xD0, 0x2710);   // 0068E27D..0068E2A6
            st<U>(self, 0x100, 4);                                    // 0068E2B1
            return;
        }
        if (rd32(self, 0x1C) != 0) {                                  // 0068E2C2..0068E2C7 ambient
            const unsigned track = rd32(self, 0xF8) != 0 ? 0x8C : 0xAC;   // 0068E2C9..0068E2F2
            if (play_track(self, 0xE0, at(self, track), 4) == 0) st<U>(self, 0xD0, 0x2710);   // 0068E2D3..0068E31E
            st<U>(self, 0x100, 2);                                    // 0068E329
            return;
        }
        if (rd32(self, 4) != 0) {                                     // 0068E33A..0068E33F music
            const unsigned track = rd32(self, 0xF8) != 0 ? 0x2C : 0x4C;   // 0068E341..0068E367
            if (play_track(self, 0xE0, at(self, track), 8) == 0) st<U>(self, 0xD0, 0x2710);   // 0068E34F..0068E38C
            st<U>(self, 0x100, 1);                                    // 0068E397
            return;
        }
        st<U>(self, 0xD0, 0x2710);                                    // 0068E3A9
        st<U>(self, 0x100, 0);                                        // 0068E3B3
        return;
    }
    if (on == 0) {                                                    // 0068E3C4..0068E3CA
        sound_fade29::Begin005d5b90(source(self, 0xE0), nullptr, 0x320);   // 0068E3CC..0068E3D7 FadeAndStop(800)
        st<U>(self, 4, 0);                                            // 0068E3DD
        return;
    }
    if (rd32(self, 0xEC) == 0) return;                                // 0068E3EB..0068E3F3
    if (table29_required::gui_playing_005d5af0(source(self, 0xE0), nullptr) == 0) {   // 0068E3F9..0068E406
        st<U>(self, 0xD0, 0);                                         // 0068E410 (EAX = 0)
        table29_required::gui_priority_005d5ba0(source(self, 0xE0), nullptr, 2);     // 0068E408..0068E416
        const unsigned track = rd32(self, 0xF8) != 0 ? 0x2C : 0x4C;   // 0068E41B..0068E441
        if (play_track(self, 0xE0, at(self, track), 8) == 0) st<U>(self, 0xD0, 0x2710);   // 0068E425..0068E46A
    }
    st<U>(self, 4, 1);                                                // 0068E474
}

// 0068EA10 CSWCAmbientSound::PlayAmbientSound(BOOL on) RET 4 (table slot +18).
void __fastcall recovered_client_ambient_sound_play_ambient_sound_0068ea10(void* self, void*, U on) noexcept(false) {
    if (rd32(self, 0xF4) == 0) return;                                // 0068EA16..0068EA1E
    if (rd32(self, 0xFC) != 0) {                                      // 0068EA24..0068EA2D
        st<U>(self, 0x1C, on);                                        // 0068EA3D
        st<U>(self, 0xD0, 0xA);                                       // 0068EA40
        if (table29_required::gui_playing_005d5af0(source(self, 0xE0), nullptr) != 0) {   // 0068EA37..0068EA51
            const I32 state = ld<I32>(self, 0x100);                   // 0068EA53
            if (state < 2) { if (on == 0) return; }                   // 0068EA59..0068EA60
            else if (state != 2 || on != 0) return;                   // 0068EA7E..0068EA86
            sound_fade29::Begin005d5b90(source(self, 0xE0), nullptr, 0x3E8);   // 0068EA66..0068EA71
            return;
        }
        if (on == 0) return;                                          // 0068EA8E..0068EA90
        if (rd32(self, 0x14) != 0) {                                  // 0068EA96..0068EA9B
            if (play_track(self, 0xE0, at(self, 0x6C), 8) == 0) st<U>(self, 0xD0, 0x2710);   // 0068EA9D..0068EAC6
            st<U>(self, 0x100, 4);                                    // 0068EAD1
            return;
        }
        if (rd32(self, 0x1C) != 0) {                                  // 0068EAE2..0068EAE7
            const unsigned track = rd32(self, 0xF8) != 0 ? 0x8C : 0xAC;   // 0068EAE9..0068EB3B
            if (play_track(self, 0xE0, at(self, track), 4) == 0) st<U>(self, 0xD0, 0x2710);   // 0068EAF3..0068EB1F
            st<U>(self, 0x100, 2);                                    // 0068EB2A
            return;
        }
        if (rd32(self, 4) != 0) {                                     // 0068EB5A..0068EB5F
            const unsigned track = rd32(self, 0xF8) != 0 ? 0x2C : 0x4C;   // 0068EB61..0068EBB0
            if (play_track(self, 0xE0, at(self, track), 8) == 0) st<U>(self, 0xD0, 0x2710);   // 0068EB6F..0068EB94
            st<U>(self, 0x100, 1);                                    // 0068EB9F
            return;
        }
        st<U>(self, 0xD0, 0x2710);                                    // 0068EBC9
        st<U>(self, 0x100, 0);                                        // 0068EBD3
        return;
    }
    if (on == 0) {                                                    // 0068EBE4..0068EBEA
        sound_fade29::Begin005d5b90(source(self, 0xE4), nullptr, 0x1770);   // 0068EBEC..0068EBF7 FadeAndStop(6000)
        st<U>(self, 0x1C, 0);                                         // 0068EBFD
        st<U>(self, 0x108, 0);                                        // 0068EC04
        return;
    }
    if (rd32(self, 0xF0) == 0) return;                                // 0068EC15..0068EC1D
    if (table29_required::gui_playing_005d5af0(source(self, 0xE4), nullptr) == 0) {   // 0068EC23..0068EC30
        unsigned track;
        if (rd32(self, 0xF8) != 0) {                                  // 0068EC36..0068EC40
            table29_required::gui_volume_005d5b50(source(self, 0xE4), nullptr, ld<U8>(self, 0x28));   // 0068EC42..0068EC4C
            table29_required::gui_priority_005d5ba0(source(self, 0xE4), nullptr, 4);                 // 0068EC51..0068EC59
            track = 0x8C;                                             // 0068EC5E
        } else {
            table29_required::gui_volume_005d5b50(source(self, 0xE4), nullptr, ld<U8>(self, 0x29));   // 0068EC66..0068EC70
            table29_required::gui_priority_005d5ba0(source(self, 0xE4), nullptr, 4);                 // 0068EC75..0068EC7D
            track = 0xAC;                                             // 0068EC82
        }
        U failed = play_track(self, 0xE4, at(self, track), 8) == 0 ? 1u : 0u;   // 0068EC88..0068ECAE (neg/sbb/inc)
        st<U>(self, 0x108, failed);                                   // 0068ECAF
        if (failed != 0) {                                            // 0068ECB5
            failed = play_track(self, 0xE4, at(self, track), 4) == 0 ? 1u : 0u;   // 0068ECB7..0068ECDD
            st<U>(self, 0x108, failed);                               // 0068ECDE
        }
    }
    st<U>(self, 0x1C, 1);                                             // 0068ECE4
}

// 0068E1B0 CSWCAmbientSound::StopSounds(BOOL keep_music) RET 4.
void __fastcall recovered_client_ambient_sound_stop_sounds_0068e1b0(void* self, void*, U keep_music) noexcept(false) {
    vslot<SlotOn>(self, 0)(self, nullptr, 0);                         // 0068E1B4..0068E1B9 [vtbl+0] PlayMusic(0)
    st<U>(self, 0x14, 0);                                             // 0068E1BF
    st<U>(self, 0xD0, 0);                                             // 0068E1C2
    if (keep_music != 0) {                                            // 0068E1BB..0068E1C8
        kotor_fastcall_005d5b80(source(self, 0xE4), nullptr);         // 0068E1CA..0068E1D0 Stop
        st<U>(self, 0x1C, 0);                                         // 0068E1D5
        st<U>(self, 0x108, 0);                                        // 0068E1D8
        return;
    }
    vslot<SlotOn>(self, 0x18)(self, nullptr, 0);                      // 0068E1E3..0068E1E8 [vtbl+18h] PlayAmbientSound(0)
}

// 0068D690 CSWCAmbientSound::UpdateTimeOfDay. ECX this, RET.
void __fastcall recovered_client_ambient_sound_update_time_of_day_0068d690(void* self, void*) noexcept(false) {
    ExoString music_playing, ambient_playing;                         // {E-0x2c} / {E-0x24}
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&music_playing), nullptr);     // 0068D6AB..0068D6AF
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(&ambient_playing), nullptr);   // 0068D6B4..0068D6C0
    {
        ResRef tmp;                                                   // {E-0x1c}
        c2da_ref_to_string_00405f70(table29_required::gui_file_playing_005d6230(source(self, 0xE0), nullptr,
            reinterpret_cast<menu_resources::Ref*>(&tmp)), nullptr, &music_playing);   // 0068D6C5..0068D6E1
        c2da_ref_to_string_00405f70(table29_required::gui_file_playing_005d6230(source(self, 0xE4), nullptr,
            reinterpret_cast<menu_resources::Ref*>(&tmp)), nullptr, &ambient_playing);   // 0068D6E6..0068D6FD
    }
    const U8 state = ld<U8>(kotor_fastcall_005ed530(client_app(), nullptr), 0xB4);   // 0068D702..0068D710
    if (state <= 4) {                                                 // 0068D717..0068D720 (table 0068D84C)
        const bool night = state == 2 || state == 4;
        if (!night) {                                                 // 0068D727 day / dawn / state 0
            if (rd32(self, 0xF8) != 1) {                              // 0068D727..0068D72E
                bool restart = exostring_compare_no_case_005e6450(&music_playing, nullptr, at(self, 0x4C)) != 0;   // 0068D730..0068D73F
                if (!restart && rd32(self, 0xFC) != 0)                // 0068D741..0068D749
                    restart = exostring_compare_no_case_005e6450(&music_playing, nullptr, at(self, 0xAC)) != 0;   // 0068D74B..0068D75D
                if (restart) {
                    vslot<SlotOn>(self, 0)(self, nullptr, 0);         // 0068D75F..0068D765
                    vslot<SlotOn>(self, 0)(self, nullptr, 1);         // 0068D767..0068D76D
                }
                if (exostring_compare_no_case_005e6450(&ambient_playing, nullptr, at(self, 0xAC)) != 0) {   // 0068D76F..0068D781
                    vslot<SlotOn>(self, 0x18)(self, nullptr, 0);      // 0068D783..0068D789
                    vslot<SlotOn>(self, 0x18)(self, nullptr, 1);      // 0068D78C..0068D792
                }
            }
            st<U>(self, 0xF8, 1);                                     // 0068D795
        } else {                                                      // 0068D7A1 night / dusk
            if (rd32(self, 0xF8) != 0) {                              // 0068D7A1..0068D7A9
                bool restart = exostring_compare_no_case_005e6450(&music_playing, nullptr, at(self, 0x2C)) != 0;   // 0068D7AB..0068D7BA
                if (!restart && rd32(self, 0xFC) != 0)                // 0068D7BC..0068D7C4
                    restart = exostring_compare_no_case_005e6450(&music_playing, nullptr, at(self, 0x8C)) != 0;   // 0068D7C6..0068D7D8
                if (restart) {
                    vslot<SlotOn>(self, 0)(self, nullptr, 0);         // 0068D7DA..0068D7E0
                    vslot<SlotOn>(self, 0)(self, nullptr, 1);         // 0068D7E2..0068D7E8
                }
                if (exostring_compare_no_case_005e6450(&ambient_playing, nullptr, at(self, 0x8C)) != 0) {   // 0068D7EA..0068D7FC
                    vslot<SlotOn>(self, 0x18)(self, nullptr, 0);      // 0068D7FE..0068D804
                    vslot<SlotOn>(self, 0x18)(self, nullptr, 1);      // 0068D807..0068D80D
                }
            }
            st<U>(self, 0xF8, 0);                                     // 0068D810
        }
    }
    frame_LogStringDtor_005e5c20(&ambient_playing, nullptr);          // 0068D81A..0068D823
    frame_LogStringDtor_005e5c20(&music_playing, nullptr);            // 0068D828..0068D834
}
}  // extern "C"
