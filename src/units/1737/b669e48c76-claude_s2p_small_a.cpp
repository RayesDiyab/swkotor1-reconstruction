// Lane worker "s2p-small": client server-to-player handlers (CSWCMessage, client dispatcher 0066A640), part A:
//   006527E0 LevelUp, 006637C0 GuiQuickbar (+00652B00 SetButton), 006514C0 CombatRound,
//   00656BA0 Sound (+0064F050 / 0064F0A0 / 0064F0F0 sound-object minors, 0066ADC0 CSWCSoundObject::ChangePosition),
//   00665510 Ambient (+00654380..00654680 minors, 0068D530 / 0068D5E0 CSWCAmbientSound volume setters),
//   0064B8E0 Camera.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Handler ABI: thiscall(CSWCMessage* this, BYTE minor) RET 4 -> extern "C" U __fastcall kotor_fastcall_<addr>(self, edx,
// minor) @12. The dispatcher pushes a dword whose low byte is the minor; only that byte is compared (movzx/cmp byte)
// except where noted (the Camera handler reuses the whole argument slot).
#include "b0bf16f6a8-claude_s2p_small_common.hpp"

using namespace s2p;

extern "C" {
// exports of this file besides the handler table entries (declared in claude_s2p_small_common.hpp)
U __fastcall kotor_fastcall_00652b00(void* self, void*, U slot, U all);              // GuiQuickbar_SetButton RET 8
U __fastcall kotor_fastcall_0064f050(void* self, void*);                             // SoundObject_Play RET 0
U __fastcall kotor_fastcall_0064f0a0(void* self, void*);                             // SoundObject_Stop RET 0
U __fastcall kotor_fastcall_0064f0f0(void* self, void*);                             // SoundObject_ChangePosition RET 0
void __fastcall kotor_fastcall_0066adc0(void* sound, void*, const void* position);  // CSWCSoundObject::ChangePosition RET 4
U __fastcall kotor_fastcall_00654380(void* self, void*);                             // AmbientMusicPlay RET 0
U __fastcall kotor_fastcall_006543e0(void* self, void*);                             // AmbientMusicSetDelay RET 0
U __fastcall kotor_fastcall_00654450(void* self, void*);                             // AmbientMusicChangeTrack RET 0
U __fastcall kotor_fastcall_006544d0(void* self, void*);                             // AmbientBattleMusicPlay RET 0
U __fastcall kotor_fastcall_00654530(void* self, void*);                             // AmbientBattleMusicChange RET 0
U __fastcall kotor_fastcall_006545a0(void* self, void*);                             // AmbientSoundLoopPlay RET 0
U __fastcall kotor_fastcall_00654600(void* self, void*);                             // AmbientSoundLoopChange RET 0
U __fastcall kotor_fastcall_00654680(void* self, void*);                             // AmbientSoundVolume RET 0
void __fastcall kotor_fastcall_0068d530(void* ambient, void*, U volume);             // SetAmbientDayVolume RET 4
void __fastcall kotor_fastcall_0068d5e0(void* ambient, void*, U volume);             // SetAmbientNightVolume RET 4
}

namespace {
constexpr float kOne = 1.0f;                       // push 3F800000h (ReadFLOAT scale)
inline U overflow(void* m) { return kotor_fastcall_004d6230(m, nullptr, 1); }
inline U underflow(void* m) { return kotor_fastcall_004d6240(m, nullptr, 1); }
inline void* module() { return kotor_fastcall_005ed530(client(), nullptr); }      // CClientExoApp::GetModule
// CSWCModule +48h CSWCArea*, CSWCArea +1DCh CSWCAmbientSound*
inline void* ambient() { return rp(rp(module(), 0x48), 0x1dc); }
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// 006527E0 CSWCMessage::HandleServerToPlayerLevelUpMessage(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_006527e0(void* self, void*, U minor) {
    if (overflow(self) != 0) return 0;                                                  // 006527E1..006527EC
    if (static_cast<U8>(minor) == 1) {                                                  // 006527EE..006527F4
        void* gui = frame_R4_005ed690(client(), nullptr);                               // 006527F6..006527FE GetInGameGui
        if (gui != nullptr) recovered_gui_in_game_set_level_up_mode_00628650(gui, nullptr, 0); // 00652803..0065280B
    }
    if (overflow(self) != 0) return 0;                                                  // 00652810..0065281B
    if (underflow(self) != 0) return 0;                                                 // 0065281D..00652828
    return 1;                                                                           // 0065282A
}

// ---------------------------------------------------------------------------------------------------------------
// 00652B00 CSWCMessage::HandleServerToPlayerGuiQuickbar_SetButton(BYTE slot, int all) RET 8
// The client only consumes the per-button payload (type byte -> payload shape via the byte table 00652CD4 / jump
// table 00652CC0); nothing is stored. all != 0 walks slots 0..23h, otherwise only [slot, slot+1) (byte arithmetic:
// slot FFh gives an empty range and returns 1).
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00652b00(void* self, void*, U slot, U all) {
    U8 index;                                                                           // al / [esp+0Fh]
    U8 end;                                                                             // [esp+0Eh]
    if (all != 0) {                                                                     // 00652B1E..00652B27
        index = 0; end = 0x24;                                                          // 00652B29..00652B2B
    } else {
        index = static_cast<U8>(slot);                                                  // 00652B32..00652B35
        end = static_cast<U8>(static_cast<U8>(slot) + 1);                               // 00652B37..00652B39
    }
    for (; index < end; index = static_cast<U8>(index + 1)) {                           // 00652B3D..00652B4A, 00652C6A..00652C7A
        const U type = static_cast<U>(kotor_fastcall_004d6720(self, nullptr, 8)) - 2;   // 00652B50..00652B5C
        if (type > 0x2a) continue;                                                      // 00652B5F..00652B62
        static const U8 kShape[0x2b] = {                                                // byte table 00652CD4
            0, 1, 1, 4, 4, 4, 1, 4, 1, 4, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4,
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1, 4, 4, 4, 4, 1, 4, 3};
        switch (kShape[type]) {                                                         // 00652B68..00652B6F
        case 0:                                                                         // 00652B76 (type 2)
            (void)kotor_fastcall_004d6720(self, nullptr, 8);                            // 00652B76..00652B7A
            (void)kotor_fastcall_004d6780(self, nullptr, 0x20);                         // 00652B7F..00652B83
            (void)kotor_fastcall_004d6720(self, nullptr, 8);                            // 00652B88..00652B8C
            (void)kotor_fastcall_004d6720(self, nullptr, 8);                            // 00652B91..00652B95
            break;                                                                      // 00652B9A -> 00652C5E
        case 1:                                                                         // 00652C55 (types 3,4,8,0Ah,25h,2Ah)
            (void)kotor_fastcall_004d6780(self, nullptr, 0x20);                         // 00652C55..00652C59
            break;
        case 2: {                                                                       // 00652BB6 (type 12h): two strings
            ExoString first;                                                            // 00652BB6..00652BBA [esp+18h]
            ExoString second;                                                           // 00652BBF..00652BCB [esp+10h]
            {
                StrTemp t;                                                              // [esp+20h]
                str_assign(&first, kotor_fastcall_004d67e0(self, nullptr, &t, 0x20));  // 00652BD0..00652BEC
                frame_LogStringDtor_005e5c20(&t, nullptr);                              // 00652BF1..00652BF9
            }
            {
                StrTemp t;                                                              // [esp+28h]
                str_assign(&second, kotor_fastcall_004d67e0(self, nullptr, &t, 0x20)); // 00652BFE..00652C16
                frame_LogStringDtor_005e5c20(&t, nullptr);                              // 00652C1B..00652C23
            }
            if (overflow(self) != 0) return 0;                                         // 00652C28..00652C3B -> 00652C95
                                                                                        //   (~second, ~first, return 0)
            continue;                                                                   // 00652C3D..00652C53 (~second, ~first)
        }
        case 3:                                                                         // 00652B9F (type 2Ch)
            (void)kotor_fastcall_004d6780(self, nullptr, 0x20);                         // 00652B9F..00652BA3
            (void)kotor_fastcall_004d6720(self, nullptr, 8);                            // 00652BA8..00652BAC
            break;                                                                      // 00652BB1 -> 00652C5E
        default:                                                                        // 00652C6A (no payload)
            continue;
        }
        if (overflow(self) != 0) return 0;                                             // 00652C5E..00652C68 -> 00652CAB
    }
    return 1;                                                                           // 00652C80 (ebx = 1)
}

// ---------------------------------------------------------------------------------------------------------------
// 006637C0 CSWCMessage::HandleServerToPlayerGuiQuickbar(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_006637c0(void* self, void*, U minor) {
    const U8 m = static_cast<U8>(minor);                                                // 006637C0
    if (m == 1) return kotor_fastcall_00652b00(self, nullptr, 0, 1);                    // 006637F3..006637FF (all buttons)
    if (m == 2) {                                                                       // 006637CB..006637CC
        const U8 slot = kotor_fastcall_004d6720(self, nullptr, 8);                      // 006637D4..006637D8
        // 006637DD..006637E1: the byte is stored over the low byte of the minor argument slot and the whole dword
        // is passed (SetButton only reads its low byte).
        return kotor_fastcall_00652b00(self, nullptr, (minor & 0xffffff00u) | slot, 0); // 006637E5..006637F0
    }
    return 0;                                                                           // 006637CE..006637D1
}

// ---------------------------------------------------------------------------------------------------------------
// 006514C0 CSWCMessage::HandleServerToPlayerCombatRound(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_006514c0(void* self, void*, U minor) {
    ExoString unused;                                                                   // 006514DB..006514DF [esp+4]
    if (overflow(self) != 0) return 0;                                                  // 006514E4..006514F7 (~unused)
    if (static_cast<U8>(minor) == 1) (void)kotor_fastcall_004d6720(self, nullptr, 8);   // 0065151E..00651529
    return 1;                                                                           // 0065152E..00651543 (~unused)
}

// ---------------------------------------------------------------------------------------------------------------
// Sound objects
// ---------------------------------------------------------------------------------------------------------------
// 0064F050 CSWCMessage::HandleServerToPlayerSoundObject_Play() RET 0
extern "C" U __fastcall kotor_fastcall_0064f050(void* self, void*) {
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 0064F054..0064F05D edi
    if (overflow(self) != 0) return 0;                                                  // 0064F059..0064F066
    if (underflow(self) != 0) return 0;                                                 // 0064F068..0064F073
    void* sound = recovered_client_get_sound_object_005ed620(client(), nullptr, id);    // 0064F075..0064F07E
    if (sound != nullptr) recovered_client_sound_object_play_0066b050(sound, nullptr);  // 0064F083..0064F089
    return 1;                                                                           // 0064F08E
}

// 0064F0A0 CSWCMessage::HandleServerToPlayerSoundObject_Stop() RET 0
extern "C" U __fastcall kotor_fastcall_0064f0a0(void* self, void*) {
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 0064F0A4..0064F0AD edi
    if (overflow(self) != 0) return 0;                                                  // 0064F0A9..0064F0B6
    if (underflow(self) != 0) return 0;                                                 // 0064F0B8..0064F0C3
    void* sound = recovered_client_get_sound_object_005ed620(client(), nullptr, id);    // 0064F0C5..0064F0CE
    if (sound != nullptr) recovered_client_sound_object_stop_0066b130(sound, nullptr);  // 0064F0D3..0064F0D9
    return 1;                                                                           // 0064F0DE
}

// 0064F0F0 CSWCMessage::HandleServerToPlayerSoundObject_ChangePosition() RET 0
extern "C" U __fastcall kotor_fastcall_0064f0f0(void* self, void*) {
    float position[3] = {0.0f, 0.0f, 0.0f};                                             // 0064F0F7..0064F107 [esp+8..10h]
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 0064F10F..0064F11D edi
    position[0] = kotor_fastcall_004d67b0(self, nullptr, kOne, 0x20);                  // 0064F114..0064F124 fstp dword
    position[1] = kotor_fastcall_004d67b0(self, nullptr, kOne, 0x20);                  // 0064F128..0064F136
    position[2] = kotor_fastcall_004d67b0(self, nullptr, kOne, 0x20);                  // 0064F13A..0064F148
    if (overflow(self) != 0) return 0;                                                  // 0064F14C..0064F157
    if (underflow(self) != 0) return 0;                                                 // 0064F159..0064F164
    void* sound = recovered_client_get_sound_object_005ed620(client(), nullptr, id);    // 0064F166..0064F16F
    if (sound != nullptr) kotor_fastcall_0066adc0(sound, nullptr, position);           // 0064F174..0064F17F
    return 1;                                                                           // 0064F184
}

// 0066ADC0 CSWCSoundObject::ChangePosition(Vector const& position) RET 4
// +118h height offset added to z; +168h selects the streaming source (+100h) over the static source (+FCh);
// then the object's own virtual SetPosition (vtable +8Ch) with the adjusted vector.
extern "C" void __fastcall kotor_fastcall_0066adc0(void* sound, void*, const void* position) {
    float adjusted[3];                                                                  // [esp+4..0Ch]
    adjusted[0] = rdf(position, 0);                                                     // 0066ADCA..0066ADE8
    adjusted[1] = rdf(position, 4);                                                     // 0066ADCC..0066ADF0
    adjusted[2] = rdf(position, 8) + rdf(sound, 0x118);                                 // 0066ADCF..0066ADEC fld/fadd/fstp dword
    if (rd(sound, 0x168) == 0) {                                                        // 0066ADE0..0066ADF4
        kotor_m15_sound_source_set_position_005d59e0(rp(sound, 0xfc), nullptr, adjusted, 0); // 0066ADF6..0066AE03
    } else {
        kotor_m15_stream_source_set_position_005d62c0(rp(sound, 0x100), nullptr, adjusted);  // 0066AE0A..0066AE15
    }
    vcall<void>(sound, 0x8c, static_cast<const void*>(adjusted));                       // 0066AE1A..0066AE23
}

// ---------------------------------------------------------------------------------------------------------------
// 00656BA0 CSWCMessage::HandleServerToPlayerSound(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00656ba0(void* self, void*, U minor) {
    switch (static_cast<U8>(minor)) {                                                   // 00656BA0..00656BB0
    case 2: return kotor_fastcall_0064f050(self, nullptr);                              // 00656BC7
    case 3: return kotor_fastcall_0064f0a0(self, nullptr);                              // 00656BBF
    case 5: return kotor_fastcall_0064f0f0(self, nullptr);                              // 00656BB7
    default: return 0;                                                                  // 00656BB2
    }
}

// ---------------------------------------------------------------------------------------------------------------
// Ambient minors: each reads its payload, re-checks overflow/underflow and calls the area's CSWCAmbientSound
// (module +48h -> area +1DCh) through its vtable (slots 0..20h) or the two volume setters below. The module/area
// pointers are not checked (original).
// ---------------------------------------------------------------------------------------------------------------
// 00654380 CSWCMessage::HandleServerToPlayerAmbientMusicPlay() RET 0
extern "C" U __fastcall kotor_fastcall_00654380(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 00654381..0065438C
    const U play = kotor_fastcall_004d66f0(self, nullptr);                              // 00654393..0065439E edi
    if (overflow(self) != 0) return 0;                                                  // 0065439A..006543A7
    if (underflow(self) != 0) return 0;                                                 // 006543A9..006543B4
    vcall<void>(ambient(), 0x0, play);                                                  // 006543B6..006543CF PlayMusic
    return 1;
}

// 006543E0 CSWCMessage::HandleServerToPlayerAmbientMusicSetDelay() RET 0
extern "C" U __fastcall kotor_fastcall_006543e0(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 006543E1..006543EC
    const U delay = kotor_fastcall_004d6780(self, nullptr, 0x20);                       // 006543F3..00654400 edi
    if (overflow(self) != 0) return 0;                                                  // 006543FC..00654409
    if (underflow(self) != 0) return 0;                                                 // 0065440B..00654416
    vcall<void>(ambient(), 0x4, delay);                                                 // 00654418..00654431 SetMusicDelay
    return 1;
}

// 00654450 CSWCMessage::HandleServerToPlayerAmbientMusicChangeTrack() RET 0
extern "C" U __fastcall kotor_fastcall_00654450(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 00654451..0065445C
    const U day = kotor_fastcall_004d66f0(self, nullptr);                               // 00654464..0065446F edi
    const U track = kotor_fastcall_004d6780(self, nullptr, 0x20);                       // 0065446B..0065447A ebx
    if (overflow(self) != 0) return 0;                                                  // 00654476..00654483
    if (underflow(self) != 0) return 0;                                                 // 00654485..00654490
    void* a = ambient();                                                                // 00654492..006544A4
    if (day != 0) vcall<void>(a, 0x8, track);                                           // 006544AF..006544B1 SetMusicDayTrack
    else vcall<void>(a, 0xc, track);                                                    // 006544BD SetMusicNightTrack
    return 1;
}

// 006544D0 CSWCMessage::HandleServerToPlayerAmbientBattleMusicPlay() RET 0
extern "C" U __fastcall kotor_fastcall_006544d0(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 006544D1..006544DC
    const U play = kotor_fastcall_004d66f0(self, nullptr);                              // 006544E3..006544EE edi
    if (overflow(self) != 0) return 0;                                                  // 006544EA..006544F7
    if (underflow(self) != 0) return 0;                                                 // 006544F9..00654504
    vcall<void>(ambient(), 0x10, play);                                                 // 00654506..0065451F PlayBattleMusic
    return 1;
}

// 00654530 CSWCMessage::HandleServerToPlayerAmbientBattleMusicChange() RET 0
extern "C" U __fastcall kotor_fastcall_00654530(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 00654531..0065453C
    const U track = kotor_fastcall_004d6780(self, nullptr, 0x20);                       // 00654543..00654550 edi
    if (overflow(self) != 0) return 0;                                                  // 0065454C..00654559
    if (underflow(self) != 0) return 0;                                                 // 0065455B..00654566
    vcall<void>(ambient(), 0x14, track);                                                // 00654568..00654581 SetBattleMusicTrack
    return 1;
}

// 006545A0 CSWCMessage::HandleServerToPlayerAmbientSoundLoopPlay() RET 0
extern "C" U __fastcall kotor_fastcall_006545a0(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 006545A1..006545AC
    const U play = kotor_fastcall_004d66f0(self, nullptr);                              // 006545B3..006545BE edi
    if (overflow(self) != 0) return 0;                                                  // 006545BA..006545C7
    if (underflow(self) != 0) return 0;                                                 // 006545C9..006545D4
    vcall<void>(ambient(), 0x18, play);                                                 // 006545D6..006545EF PlayAmbientSound
    return 1;
}

// 00654600 CSWCMessage::HandleServerToPlayerAmbientSoundLoopChange() RET 0
extern "C" U __fastcall kotor_fastcall_00654600(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 00654601..0065460C
    const U day = kotor_fastcall_004d66f0(self, nullptr);                               // 00654614..0065461F edi
    const U track = kotor_fastcall_004d6780(self, nullptr, 0x20);                       // 0065461B..0065462A ebx
    if (overflow(self) != 0) return 0;                                                  // 00654626..00654633
    if (underflow(self) != 0) return 0;                                                 // 00654635..00654640
    void* a = ambient();                                                                // 00654642..00654654
    if (day != 0) vcall<void>(a, 0x1c, track);                                          // 0065465F..00654661 SetAmbientDayTrack
    else vcall<void>(a, 0x20, track);                                                   // 0065466D SetAmbientNightTrack
    return 1;
}

// 00654680 CSWCMessage::HandleServerToPlayerAmbientSoundVolume() RET 0
extern "C" U __fastcall kotor_fastcall_00654680(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 00654681..0065468C
    const U day = kotor_fastcall_004d66f0(self, nullptr);                               // 00654694..0065469F edi
    const U volume = kotor_fastcall_004d6780(self, nullptr, 0x20);                      // 0065469B..006546AA ebx
    if (overflow(self) != 0) return 0;                                                  // 006546A6..006546B3
    if (underflow(self) != 0) return 0;                                                 // 006546B5..006546C0
    void* a = ambient();                                                                // 006546C2..006546D4
    if (day != 0) kotor_fastcall_0068d530(a, nullptr, volume);                          // 006546DB..006546DD
    else kotor_fastcall_0068d5e0(a, nullptr, volume);                                   // 006546EB
    return 1;
}

// 0068D530 CSWCAmbientSound::SetAmbientDayVolume(int volume) RET 4
// 0068D5E0 CSWCAmbientSound::SetAmbientNightVolume(int volume) RET 4
// Identical bodies on (+28h day volume byte, +8Ch day track) / (+29h night volume byte, +ACh night track): the stored
// byte (zero-extended) is compared with the whole dword argument; on change the streaming source (+E4h) volume is set
// only while it is playing the matching track, and the low byte is stored either way.
namespace {
void set_ambient_volume(void* self, U volume, U byte_off, U track_off) {
    ExoString playing;                                                                  // [esp+8]
    ResRef file;                                                                        // [esp+10h] (GetFilePlaying out)
    c2da_ref_to_string_00405f70(
        table29_required::gui_file_playing_005d6230(rp(self, 0xe4), nullptr,
                                                    reinterpret_cast<menu_resources::Ref*>(&file)),
        nullptr, &playing);                                                             // 0068D555..0068D574
    if (static_cast<U>(rd8(self, byte_off)) != volume) {                               // 0068D579..0068D583
        if (table29_required::gui_playing_005d5af0(rp(self, 0xe4), nullptr) != 0 &&      // 0068D585..0068D592
            exostring_compare_no_case_005e6450(&playing, nullptr, at(self, track_off)) != 0) { // 0068D594..0068D5A6
            table29_required::gui_volume_005d5b50(rp(self, 0xe4), nullptr, volume);     // 0068D5A8..0068D5AF
        }
        wr8(self, byte_off, static_cast<U8>(volume));                                   // 0068D5B4
    }
}                                                                                       // 0068D5B7..0068D5C3 ~playing
}  // namespace

extern "C" void __fastcall kotor_fastcall_0068d530(void* ambient, void*, U volume) {
    set_ambient_volume(ambient, volume, 0x28, 0x8c);                                    // 0068D530..0068D5D8
}

extern "C" void __fastcall kotor_fastcall_0068d5e0(void* ambient, void*, U volume) {
    set_ambient_volume(ambient, volume, 0x29, 0xac);                                    // 0068D5E0..0068D688
}

// ---------------------------------------------------------------------------------------------------------------
// 00665510 CSWCMessage::HandleServerToPlayerAmbient(BYTE minor) RET 4 (jump table 00665568, minors 1..8)
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00665510(void* self, void*, U minor) {
    switch (static_cast<U8>(minor)) {                                                   // 00665510..0066551B
    case 1: return kotor_fastcall_00654380(self, nullptr);                              // 00665522
    case 2: return kotor_fastcall_006543e0(self, nullptr);                              // 0066552A
    case 3: return kotor_fastcall_00654450(self, nullptr);                              // 00665532
    case 4: return kotor_fastcall_006544d0(self, nullptr);                              // 0066553A
    case 5: return kotor_fastcall_00654530(self, nullptr);                              // 00665542
    case 6: return kotor_fastcall_006545a0(self, nullptr);                              // 0066554A
    case 7: return kotor_fastcall_00654600(self, nullptr);                              // 00665552
    case 8: return kotor_fastcall_00654680(self, nullptr);                              // 0066555A
    default: return 0;                                                                  // 00665562
    }
}

// ---------------------------------------------------------------------------------------------------------------
// 0064B8E0 CSWCMessage::HandleServerToPlayerCamera(BYTE minor) RET 4
// Every path returns 0 (original). Minor 1: flag byte; bit0 yaw, bit1 distance, bit2 pitch floats, bit3 an extra byte;
// each present value is formatted ("yaw %f" with (v - pi/2) * 180/pi, "dist %f", "pitch %f": note bit1 is the second
// float and is printed as "dist", bit2 the third as "pitch") and sent to the module camera (module +40h) through its
// vtable +60h. Quirk: with no module, the camera pointer is the reused minor argument slot (low byte = bit0 flag).
// Minor 2: camera mode byte -> CClientOptions::SetCameraMode with the reused argument dword.
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_0064b8e0(void* self, void*, U minor) {
    if (overflow(self) != 0) return 0;                                                  // 0064B8E4..0064B8EF
    U slot = minor;                                                                     // [esp+5Ch] argument slot
    const U8 m = static_cast<U8>(minor);                                                // 0064B8F5
    if (m == 1) {                                                                       // 0064B8F9..0064B8FC
        const U8 flags = kotor_fastcall_004d6720(self, nullptr, 8);                     // 0064B902..0064B90F bl
        if (overflow(self) != 0) return 0;                                              // 0064B90B..0064B918
        float yaw, distance, pitch;                                                     // [esp+0Ch] / [esp+10h] / [esp+14h]
        const U8 has_yaw = flags & 1;                                                   // 0064B91E..0064B922 -> [esp+5Ch]
        slot = (slot & 0xffffff00u) | has_yaw;
        if (has_yaw != 0) yaw = kotor_fastcall_004d67b0(self, nullptr, kOne, 0x20);     // 0064B928..0064B936
        const U8 has_distance = flags & 2;                                              // 0064B93A..0064B93E [esp+0Ah]
        if (has_distance != 0) distance = kotor_fastcall_004d67b0(self, nullptr, kOne, 0x20); // 0064B944..0064B952
        const U8 has_pitch = flags & 4;                                                 // 0064B956..0064B95A [esp+0Bh]
        if (has_pitch != 0) pitch = kotor_fastcall_004d67b0(self, nullptr, kOne, 0x20); // 0064B960..0064B96E
        if ((flags & 8) != 0) (void)kotor_fastcall_004d6720(self, nullptr, 8);          // 0064B972..0064B97B
        if (overflow(self) != 0) return 0;                                              // 0064B980..0064B98B
        void* camera;                                                                   // esi
        if (module() != nullptr) camera = rp(module(), 0x40);                           // 0064B991..0064B9B0 (GetModule twice)
        else camera = ptr(slot);                                                        // 0064B9B5 (quirk)
        if (camera == nullptr) return 0;                                                // 0064B9B9..0064B9BB
        char text[0x40];                                                                // [esp+18h]
        if (has_yaw != 0) {                                                             // 0064B9C1..0064B9C7
            // fld yaw; fsub [00741BB8] (1.5707964f); fmul [00742EF8] (57.29578f); fstp qword (53-bit precision)
            const double degrees = (static_cast<double>(yaw) - static_cast<double>(1.5707963705062866f)) *
                                   static_cast<double>(57.295780181884766f);
            format_006fadb0(text, "yaw %f", degrees);                                   // 0064B9C9..0064B9E9 (0074FC28)
            vcall<void>(camera, 0x60, static_cast<const char*>(text));                  // 0064B9EE..0064B9FA
        }
        if (has_distance != 0) {                                                        // 0064B9FD..0064BA03
            format_006fadb0(text, "dist %f", static_cast<double>(distance));            // 0064BA05..0064BA19 (0074FC3C)
            vcall<void>(camera, 0x60, static_cast<const char*>(text));                  // 0064BA1E..0064BA2A
        }
        if (has_pitch != 0) {                                                           // 0064BA2D..0064BA33
            format_006fadb0(text, "pitch %f", static_cast<double>(pitch));              // 0064BA35..0064BA49 (0074FC30)
            vcall<void>(camera, 0x60, static_cast<const char*>(text));                  // 0064BA4E..0064BA5A
        }
        return 0;                                                                       // 0064BA5D..0064BA64 / 0064BAA3
    }
    if (m == 2) {                                                                       // 0064BA67..0064BA69
        const U8 mode = kotor_fastcall_004d6720(self, nullptr, 8);                      // 0064BA6B..0064BA78 -> [esp+5Ch]
        slot = (slot & 0xffffff00u) | mode;
        if (overflow(self) != 0) return 0;                                              // 0064BA74..0064BA83
        void* options = kotor_fastcall_005ed700(client(), nullptr);                     // 0064BA85..0064BA8E
        if (options != nullptr) menu_tables_m41::SetCameraMode_0061b6f0(options, nullptr, slot); // 0064BA93..0064BA9E
    }
    return 0;                                                                           // 0064BAA3..0064BAAA
}
