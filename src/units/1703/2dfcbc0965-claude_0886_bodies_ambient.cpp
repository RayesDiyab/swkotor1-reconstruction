// 0886-gates (Claude): faithful bodies for the former AREA_GATE slots of the CSWSAmbientSound vtable (0074C268)
// that the 0886 cohort area constructor installs (area_native_load_numeric.inc area_effect_vtable), plus the
// player collection and the seven CSWSMessage ambient senders they call.
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members are exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   005C9900 PlayMusic(int)  005C99A0 SetMusicDelay(int)  005C9A40 SetMusicDayTrack(int)  005C9AE0 SetMusicNightTrack(int)
//   005C9B90 PlayBattleMusic(int)  005C9C30 SetBattleMusicTrack(int)  005C9CD0 PlayAmbientSound(int)
//   005C9D70 SetAmbientDayTrack(int)  005C9E20 SetAmbientNightTrack(int)                    all RET 4, no result
//   005C9820 CSWSAmbientSound::GetPlayersInArea(CExoArrayList<ulong>* out)                    RET 4, EAX = count
//   0056AD60 / 0056AE90 / 0056AF50 SendServerToPlayerAmbient{MusicPlay,BattleMusicPlay,SoundLoopPlay}(player, bool) RET 8
//   0056ADC0 / 0056AEF0 SendServerToPlayerAmbient{MusicSetDelay,BattleMusicChange}(player, dword)                 RET 8
//   0056AE20 / 0056AFB0 SendServerToPlayerAmbient{MusicChangeTrack,SoundLoopChange}(player, bool day, dword)      RET 0Ch
// Message major 28h, minors 1..7; CreateWriteMessage(size, -1, 1); senders return 0 when GetWriteMessage fails,
// else the SendServerToPlayerMessage result.
// EH: the ambient methods register an unwind frame that only frees the local player list on a throw; the bound
// callees do not throw in normal operation, so that cleanup is not reproduced (the normal path frees it).
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
using I = std::int32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
struct IdList { U data, size, capacity; };                                       // CExoArrayList<ulong>
}

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }    // [007A39FC] CAppManager*

extern "C" {
// ---------------- existing providers (c14 map, exact decorated arity) ----------------
void __cdecl live_free(void*);                                                   // 006FA390
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);     // 004AEDB0 GetSWSMessage
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);     // 004AED90 GetPlayerList (w1_native_b7)
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);  // 005E9B60 CExoLinkedListInternal::GetAtPos (0168)
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);       // 005E9B90 CExoLinkedListInternal::GetNext (0168)
void* __fastcall kotor_fastcall_00561080(void* player, void*);                   // 00561080 CSWSPlayer::GetGameObject (0595)
void* __fastcall recovered_object_area_004cb120(void* object, void*);            // 004CB120 CSWSObject::GetArea (1060)
void __fastcall aurora_list_add16_00671c00(void* list, void*, U value);          // 00671C00 CExoArrayList::Add (0293)
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b); // 004D68A0 CreateWriteMessage (w2_message_native)
void __fastcall kotor_fastcall_004d68e0(void* message, void*, U value);          // 004D68E0 WriteBOOL
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);  // 004D6340 WriteDWORD
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size); // 004D6440 GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // 00567E60

// ---------------- bodies defined here ----------------
U __fastcall kotor_fastcall_005c9820(void* sound, void*, void* out);
U __fastcall kotor_fastcall_0056ad60(void* message, void*, U player, U value);
U __fastcall kotor_fastcall_0056adc0(void* message, void*, U player, U value);
U __fastcall kotor_fastcall_0056ae20(void* message, void*, U player, U day, U track);
U __fastcall kotor_fastcall_0056ae90(void* message, void*, U player, U value);
U __fastcall kotor_fastcall_0056aef0(void* message, void*, U player, U value);
U __fastcall kotor_fastcall_0056af50(void* message, void*, U player, U value);
U __fastcall kotor_fastcall_0056afb0(void* message, void*, U player, U day, U track);
void __fastcall kotor_fastcall_005c9900(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c99a0(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c9a40(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c9ae0(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c9b90(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c9c30(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c9cd0(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c9d70(void* sound, void*, U value);
void __fastcall kotor_fastcall_005c9e20(void* sound, void*, U value);
}

namespace {
void* server() {
    return *reinterpret_cast<void* const*>(static_cast<unsigned char*>(kotor_menu_window_cells::manager_007a39fc) + 8);
}
void* sws_message() { return recovered_server_sws_message_004aedb0(server(), nullptr); }
// Shared tail of the seven senders: GetWriteMessage, then SendServerToPlayerMessage(player, 28h, minor, ...).
U finish(void* message, U player, U minor) {
    void* data = nullptr;                                                        // [esp+8]
    U size = 0;                                                                  // [esp+4]
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;  // 0056AD7D..0056AD96
    return kotor_fastcall_00567e60(message, nullptr, player, 0x28, minor, data, size); // 0056AD99..0056ADAE
}
}

// ===================================================================================== 005C9820
// GetPlayersInArea: clears `out` (frees data, capacity 0), then adds the id (+4) of every player whose game
// object's creature (vtbl +30h) is in the area whose id (+120h) equals this +2Ch.
// GetGameObject and GetArea are called twice, as in the original.
U __fastcall kotor_fastcall_005c9820(void* sound, void*, void* out) {
    const U list = word(out);
    if (rd(list) != 0) {                                                         // 005C9827..005C9830
        live_free(ptr(rd(list)));                                                // 005C9832..005C9833
        wr(list, 0);                                                             // 005C983B
        wr(list + 8, 0);                                                         // 005C9842
    }
    wr(list + 4, 0);                                                             // 005C9849
    void* const players = recovered_server_player_list_004aed90(server(), nullptr); // 005C9850..005C9858
    if (players != nullptr) {                                                    // 005C985F..005C9861
        void* pos = ptr(rd(rd(word(players))));                                  // 005C9867..005C986D (head node)
        if (pos != nullptr) {                                                    // 005C986B..005C9871
            do {
                void* const player = unresolved_list_at_pos_005e9b60(ptr(rd(word(players))), nullptr, pos); // 005C9875..005C9878
                if (player != nullptr && kotor_fastcall_00561080(player, nullptr) != nullptr) {          // 005C987F..005C988C
                    void* const object = kotor_fastcall_00561080(player, nullptr);                        // 005C988E..005C9890
                    using AsCreature = void*(__thiscall*)(void*);
                    void* const creature = reinterpret_cast<AsCreature>(rd(rd(word(object)) + 0x30))(object); // 005C9895..005C9899
                    if (creature != nullptr && recovered_object_area_004cb120(creature, nullptr) != nullptr) {   // 005C989C..005C98AB
                        const U area = word(recovered_object_area_004cb120(creature, nullptr));            // 005C98AD..005C98AF
                        if (rd(area + 0x11c + 4) == rd(word(sound) + 0x2c))                                // 005C98B4..005C98C5
                            aurora_list_add16_00671c00(out, nullptr, rd(word(player) + 4));               // 005C98C7..005C98CD
                    }
                }
                (void)unresolved_list_next_005e9b90(ptr(rd(word(players))), nullptr, &pos);                // 005C98D2..005C98D9
            } while (pos != nullptr);                                                                      // 005C98DE..005C98E4
        }
    }
    return rd(list + 4);                                                         // 005C98E8..005C98EE
}

// ===================================================================================== CSWSMessage senders
U __fastcall kotor_fastcall_0056ad60(void* message, void*, U player, U value) {  // AmbientMusicPlay, minor 1
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                // 0056AD64..0056AD6C
    kotor_fastcall_004d68e0(message, nullptr, value);                            // 0056AD71..0056AD78
    return finish(message, player, 1);                                           // 0056AD7D..0056ADB7
}
U __fastcall kotor_fastcall_0056adc0(void* message, void*, U player, U value) {  // AmbientMusicSetDelay, minor 2
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                // 0056ADC4..0056ADCC
    kotor_fastcall_004d6340(message, nullptr, value, 0x20);                      // 0056ADD1..0056ADDA
    return finish(message, player, 2);                                           // 0056ADDF..0056AE19
}
U __fastcall kotor_fastcall_0056ae20(void* message, void*, U player, U day, U track) { // AmbientMusicChangeTrack, minor 3
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                // 0056AE24..0056AE2C
    kotor_fastcall_004d68e0(message, nullptr, day);                              // 0056AE31..0056AE38
    kotor_fastcall_004d6340(message, nullptr, track, 0x20);                      // 0056AE3D..0056AE46
    return finish(message, player, 3);                                           // 0056AE4B..0056AE85
}
U __fastcall kotor_fastcall_0056ae90(void* message, void*, U player, U value) {  // AmbientBattleMusicPlay, minor 4
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                // 0056AE94..0056AE9C
    kotor_fastcall_004d68e0(message, nullptr, value);                            // 0056AEA1..0056AEA8
    return finish(message, player, 4);                                           // 0056AEAD..0056AEE7
}
U __fastcall kotor_fastcall_0056aef0(void* message, void*, U player, U value) {  // AmbientBattleMusicChange, minor 5
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                // 0056AEF4..0056AEFC
    kotor_fastcall_004d6340(message, nullptr, value, 0x20);                      // 0056AF01..0056AF0A
    return finish(message, player, 5);                                           // 0056AF0F..0056AF49
}
U __fastcall kotor_fastcall_0056af50(void* message, void*, U player, U value) {  // AmbientSoundLoopPlay, minor 6
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);                // 0056AF54..0056AF5C
    kotor_fastcall_004d68e0(message, nullptr, value);                            // 0056AF61..0056AF68
    return finish(message, player, 6);                                           // 0056AF6D..0056AFA7
}
U __fastcall kotor_fastcall_0056afb0(void* message, void*, U player, U day, U track) { // AmbientSoundLoopChange, minor 7
    kotor_fastcall_004d68a0(message, nullptr, 8, 0xffffffffu, 1);                // 0056AFB4..0056AFBC
    kotor_fastcall_004d68e0(message, nullptr, day);                              // 0056AFC1..0056AFC8
    kotor_fastcall_004d6340(message, nullptr, track, 0x20);                      // 0056AFCD..0056AFD6
    return finish(message, player, 7);                                           // 0056AFDB..0056B015
}

// ===================================================================================== ambient methods
namespace {
// "Always" shape (PlayMusic / SetMusicDayTrack / PlayAmbientSound): store, collect, send to each player.
// "Changed" shape (the others): only when the stored value differs; the (possibly empty) list is freed either way.
template<class Send> void broadcast(void* sound, U offset, U value, bool only_changed, Send send) {
    void* const message = sws_message();                                         // GetSWSMessage (first call)
    IdList players{0, 0, 0};                                                     // local CExoArrayList<ulong>
    if (!only_changed || rd(word(sound) + offset) != value) {                    // cmp [this+off], value; je
        wr(word(sound) + offset, value);                                         // mov [this+off], value
        const I count = static_cast<I>(kotor_fastcall_005c9820(sound, nullptr, &players)); // GetPlayersInArea
        for (I i = 0; i < count; ++i) send(message, rd(players.data + 4u * static_cast<U>(i))); // signed loop
    }
    live_free(ptr(players.data));                                                // push data; call 006FA390
}
}
void __fastcall kotor_fastcall_005c9900(void* sound, void*, U value) {           // 005C9900..005C9994 [+4]
    broadcast(sound, 0x04, value, false, [&](void* m, U p) { (void)kotor_fastcall_0056ad60(m, nullptr, p, value); });
}
void __fastcall kotor_fastcall_005c99a0(void* sound, void*, U value) {           // 005C99A0..005C9A3C [+8]
    broadcast(sound, 0x08, value, true, [&](void* m, U p) { (void)kotor_fastcall_0056adc0(m, nullptr, p, value); });
}
void __fastcall kotor_fastcall_005c9a40(void* sound, void*, U value) {           // 005C9A40..005C9AD6 [+C]
    broadcast(sound, 0x0c, value, false, [&](void* m, U p) { (void)kotor_fastcall_0056ae20(m, nullptr, p, 1, value); });
}
void __fastcall kotor_fastcall_005c9ae0(void* sound, void*, U value) {           // 005C9AE0..005C9B7E [+10]
    broadcast(sound, 0x10, value, true, [&](void* m, U p) { (void)kotor_fastcall_0056ae20(m, nullptr, p, 0, value); });
}
void __fastcall kotor_fastcall_005c9b90(void* sound, void*, U value) {           // 005C9B90..005C9C2C [+14]
    broadcast(sound, 0x14, value, true, [&](void* m, U p) { (void)kotor_fastcall_0056ae90(m, nullptr, p, value); });
}
void __fastcall kotor_fastcall_005c9c30(void* sound, void*, U value) {           // 005C9C30..005C9CCC [+18]
    broadcast(sound, 0x18, value, true, [&](void* m, U p) { (void)kotor_fastcall_0056aef0(m, nullptr, p, value); });
}
void __fastcall kotor_fastcall_005c9cd0(void* sound, void*, U value) {           // 005C9CD0..005C9D64 [+1C]
    broadcast(sound, 0x1c, value, false, [&](void* m, U p) { (void)kotor_fastcall_0056af50(m, nullptr, p, value); });
}
void __fastcall kotor_fastcall_005c9d70(void* sound, void*, U value) {           // 005C9D70..005C9E0E [+20]
    broadcast(sound, 0x20, value, true, [&](void* m, U p) { (void)kotor_fastcall_0056afb0(m, nullptr, p, 1, value); });
}
void __fastcall kotor_fastcall_005c9e20(void* sound, void*, U value) {           // 005C9E20..005C9EBE [+24]
    broadcast(sound, 0x24, value, true, [&](void* m, U p) { (void)kotor_fastcall_0056afb0(m, nullptr, p, 0, value); });
}
