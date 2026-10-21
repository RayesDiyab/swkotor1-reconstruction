// lane-m48-a: the sound-side occlusion closure that lane-sound stopped at (lane-sound PACKET not_authored; LANE-QUEUE 5),
// now that lane-world-los provides CSWSArea::ClearLineOfSight 0050C330 and the server accessors:
//   0050CCF0 GetObstructionFactor(Vector from, Vector to)       __stdcall, 6 DWORDs (RET 18h), float in ST0
//   005D6EF0 CExoSoundInternal::GetClosestRecentSound(...)      thiscall RET 10h, EAX 0/1
//   005D88F0 CExoSoundInternal::OcclusionRender(ulong)          thiscall RET 4
// Consumers: sound_voice_required::spatial_value_0050ccf0 / spatial_query_005d6ef0 (sound-donor-voice-3d),
// kotor_sound_path_required::occlusion_render_005d88f0 (sound-render).
// Evidence: swkotor.exe GOG K1 dumpbin /disasm 0050CCF0..0050CF7B, 005D6EF0..005D71BA, 005D88F0..005D8EA7
// (lane-m48-a/d/0050CCF0.txt, d/005D6EF0.txt, d/occl.txt). .rdata (pinned exe bytes): 0073D700 0.0f, 0073D764 1.0f,
// 0073E320 0.4f (CDCCCC3E), 00747B84 0.55f (CDCC0C3F), 0073E798 0.1f (CDCCCC3D), 0073E9AC 0.5f.
// Providers (existing M47 symbols): 005E9260 TimerDependency::high_resolution_005e9260, 006FB030
// dependencies::unsigned_divide_006fb030 (operands are non-negative ms counts), 004AA980 aurora_vector_equal_004aa980,
// 004AB100 vector_magnitude_004ab100 (double, ST0), 004AB180 normalize_004ab180, 004AE6B0/004AE750/004C30F0/0050C330
// lane-world-los, 005E9B60/005E9B90 list accessors, 005DB0E0 sound_lower29::ActorPlaying005db0e0, 005DC300
// sound_streaming29::InternalIsPlaying005dc300, 005DAEB0 sound_voice_required::actor_obstruction_005daeb0, 004015A0
// renderer_void_004015a0 (the original `call 004015A0` with ECX = source: a bare ret).
// Game-object slots +18 / +48 through lane-world-los's typed GameObjectVtbl (D41). D30-FINAL: doubles for x87
// intermediates, float at every fstp dword. Not reached with Disable Sound=1 (D17).
#include "a0e855f1be-world_los.h"
#include "9e2ca73358-sound_internal.h"

namespace {
using U = std::uint32_t; using I = std::int32_t;
using kotor_world_los::Vector;
template<class T> T& field(void* base, U offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(base) + offset); }
constexpr float kZero_0073d700 = 0.0f, kOne_0073d764 = 1.0f, kSolid_0073e320 = 0.4f, kDoor_00747b84 = 0.55f,
                kStep_0073e798 = 0.1f, kNear_0073e9ac = 0.5f;
// [[007A39E0]+4]->GetHighResolutionTimer() / 1000 (call 005E9260; push 0; push 3E8h; push edx; push eax; call 006FB030):
// only the low DWORD is used by every caller here.
U now_ms() {
    auto* const timers = static_cast<kotor_menu_10_19::TimerDependency*>(field<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 4));
    return static_cast<U>(kotor_menu_10_19::dependencies::unsigned_divide_006fb030(timers->high_resolution_005e9260(), 1000u));
}
void timer_only() {                                       // 005D8B1D..005D8B26 / 005D8D8E..005D8D96: result discarded
    auto* const timers = static_cast<kotor_menu_10_19::TimerDependency*>(field<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 4));
    (void)timers->high_resolution_005e9260();
}
kotor_list::Internal* list_at(void* self, U offset) { return field<kotor_list::Internal*>(self, offset); }
void* list_head(void* self, U offset) { return field<void*>(list_at(self, offset), 0); }
void* item_at(void* self, U offset, void* pos) {
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_at_pos_005e9b60(list_at(self, offset), nullptr, pos)));
}
void list_next(void* self, U offset, void** pos) { (void)unresolved_list_next_005e9b90(list_at(self, offset), nullptr, pos); }
bool closer_than_half(const Vector& a, const Vector& b) {  // fsub/fstp dword x3; call 004AB100; fcomp 0.5; test ah,5; jp
    Vector d{static_cast<float>(static_cast<double>(a.x) - b.x), static_cast<float>(static_cast<double>(a.y) - b.y),
             static_cast<float>(static_cast<double>(a.z) - b.z)};
    return vector_magnitude_004ab100(&d) < static_cast<double>(kNear_0073e9ac);  // jp taken unless below
}
}

namespace sound_lower29 { std::uint32_t __fastcall ActorPlaying005db0e0(void*, void*); }
namespace sound_streaming29 { std::uint32_t __fastcall InternalIsPlaying005dc300(void*, void*); }
extern "C" void __cdecl renderer_void_004015a0();

namespace sound_voice_required {
void __fastcall actor_obstruction_005daeb0(void*, void*);   // sound-source (005DAEB0 DoObstructionWork)

// 0050CCF0 GetObstructionFactor(from, to): steps along the from->to segment through blocking objects (0.1 past each
// hit), adding 0.4 per hit (placeable or invalid object) and 0.55 per door; >1 -> 1.0; at most ~100 ms of LOS work.
float __stdcall spatial_value_0050ccf0(U ax, U ay, U az, U bx, U by, U bz) {
    Vector from{kotor_world_los::bits_to_float(ax), kotor_world_los::bits_to_float(ay), kotor_world_los::bits_to_float(az)};
    Vector to{kotor_world_los::bits_to_float(bx), kotor_world_los::bits_to_float(by), kotor_world_los::bits_to_float(bz)};
    const U start = now_ms();                                                       // 0050CCF3..0050CD19 ebp
    float factor = 0.0f;                                                            // 0050CD1B [esp+10h]
    Vector zero{0.0f, 0.0f, 0.0f};                                                  // 0050CD23..0050CD33
    if (aurora_vector_equal_004aa980(&to, &zero) != 0) return kZero_0073d700;       // 0050CD0F..0050CD45 -> 0050CD53
    void* const server = field<void*>(kotor_menu_window_cells::manager_007a39fc, 8); // 0050CD47..0050CD4C
    if (server == nullptr) return kZero_0073d700;                                   // 0050CD4F..0050CD51
    void* const module = server_get_module_004ae6b0(server, nullptr);               // 0050CD63
    if (module == nullptr) return kZero_0073d700;                                   // 0050CD68..0050CD6A
    void* const area = module_get_area_004c30f0(module, nullptr);                   // 0050CD6C..0050CD73
    if (area == nullptr) return kZero_0073d700;                                     // 0050CD75..0050CD77
    U elapsed = 0;                                                                  // 0050CD89 xor esi,esi
    Vector hit{0.0f, 0.0f, 0.0f};                                                   // 0050CDBB..0050CDCB [esp+18h]
    U blocker = kotor_world_los::k_invalid_object_7f000000;                         // 0050CDB3 [esp+14h]
    I clear = area_clear_line_of_sight_0050c330(area, nullptr, &from, &to, &hit, &blocker,
        kotor_world_los::k_invalid_object_7f000000, kotor_world_los::k_invalid_object_7f000000, 0, 0); // 0050CD8B..0050CDD3
    while (clear == 0) {                                                            // 0050CDD8..0050CDDA / 0050CF58..0050CF5A
        if (elapsed >= 100u) break;                                                 // 0050CDE0..0050CDE3 (unsigned)
        if (blocker == kotor_world_los::k_invalid_object_7f000000) {                // 0050CDE9..0050CDF2
            factor = static_cast<float>(static_cast<double>(factor) + kSolid_0073e320); // 0050CDF4..0050CDF8, 0050CE43
        } else {
            void* const server_now = field<void*>(kotor_menu_window_cells::manager_007a39fc, 8); // 0050CE00..0050CE06
            auto* const object = static_cast<kotor_world_los::GameObject*>(kotor_fastcall_004ae750(server_now, nullptr, blocker)); // 0050CE09..0050CE0F
            if (object != nullptr) {                                                // 0050CE11..0050CE13
                if (object->vtbl->as_placeable_48(object, nullptr) != nullptr)      // 0050CE15..0050CE1E
                    factor = static_cast<float>(static_cast<double>(factor) + kSolid_0073e320); // 0050CE20..0050CE2A
                if (object->vtbl->as_door_18(object, nullptr) != nullptr)           // 0050CE2E..0050CE37
                    factor = static_cast<float>(static_cast<double>(factor) + kDoor_00747b84); // 0050CE39..0050CE43
            }
        }
        if (static_cast<double>(factor) > kOne_0073d764) return kOne_0073d764;      // 0050CE47..0050CE56 -> 0050CF6E
        Vector direction{static_cast<float>(static_cast<double>(to.x) - hit.x),     // 0050CE5C..0050CE74 [esp+3Ch]
                         static_cast<float>(static_cast<double>(to.y) - hit.y),     // 0050CE7C..0050CE8E
                         static_cast<float>(static_cast<double>(to.z) - hit.z)};    // 0050CE92..0050CEA1
        from = hit;                                                                 // 0050CE60..0050CE99 (raw DWORD copies into the arg)
        Vector unit;
        const Vector* const n = normalize_004ab180(&unit, &direction);              // 0050CE88..0050CEA5
        const double step_x = static_cast<double>(n->x) * kStep_0073e798;           // 0050CEAA..0050CEAC
        const double step_y = static_cast<double>(n->y) * kStep_0073e798;           // 0050CEB6..0050CEBC
        const float step_z = static_cast<float>(static_cast<double>(n->z) * kStep_0073e798); // 0050CEC2..0050CED3 fstp
        const U previous = blocker;                                                 // 0050CEB2 edi = [esp+1Ch]
        blocker = kotor_world_los::k_invalid_object_7f000000;                       // 0050CEC5
        from.x = static_cast<float>(step_x + from.x);                               // 0050CED7..0050CEE5
        from.y = static_cast<float>(step_y + from.y);                               // 0050CEE9..0050CF03
        from.z = static_cast<float>(static_cast<double>(step_z) + from.z);          // 0050CEF7..0050CF0F
        elapsed = now_ms() - start;                                                 // 0050CF13..0050CF51 sub esi,ebp
        clear = area_clear_line_of_sight_0050c330(area, nullptr, &from, &to, &hit, &blocker, previous,
            kotor_world_los::k_invalid_object_7f000000, 0, 0);                      // 0050CF29..0050CF53 (hit not reset)
    }
    return factor;                                                                  // 0050CF60 fld [esp+10h]
}

// 005D6EF0 GetClosestRecentSound(position, listener, time, value): searches both voice lists for a recent (< 0AFh ms)
// sample within 0.5 of both positions; on success returns the cached listener vector, time and value.
U __fastcall spatial_query_005d6ef0(void* self, void* /*unused_edx*/, void* position_word, void* listener_word, U* time, U* value) {
    const Vector position = *static_cast<const Vector*>(position_word);            // 005D6EF3..005D6F13 [esp+14h]
    const Vector listener = *static_cast<const Vector*>(listener_word);            // 005D6F09..005D6F2B [esp+20h]
    const U reference_time = *time;                                                 // 005D6F21..005D6F32 [esp+10h]
    struct Fields { U active, a, b, stamp, result; };
    const Fields lists[2] = {{0x1C, 0x78, 0x8C, 0x98, 0x9C}, {0x58, 0x5C, 0x70, 0x7C, 0x80}};
    const U list_offsets[2] = {0x40, 0x44};
    for (int which = 0; which < 2; ++which) {                                       // 005D6F2F.. / 005D7042..
        const Fields& f = lists[which];
        void* pos = list_head(self, list_offsets[which]);                           // [esp+60h]
        while (pos != nullptr) {
            void* const item = item_at(self, list_offsets[which], pos);             // 005D6F44..005D6F48
            if (item != nullptr && field<U>(item, f.active) != 0) {                 // 005D6F4F..005D6F5C
                const U stamp = field<U>(item, f.stamp);                            // ebx
                const Vector source = field<Vector>(item, f.a);                     // [esp+38h]
                const Vector cached = field<Vector>(item, f.b);                     // ebp/[esp+30h]
                if (closer_than_half(position, source) && closer_than_half(listener, cached) &&
                    stamp - reference_time < 0xAFu) {                               // 005D7015..005D7023 (unsigned)
                    *static_cast<Vector*>(listener_word) = cached;                  // 005D7153..005D7164
                    *time = stamp;                                                  // 005D7167..005D716F
                    *value = field<U>(item, f.result);                              // 005D7171..005D717A
                    return 1;
                }
            }
            list_next(self, list_offsets[which], &pos);                             // 005D7029..005D703C
        }
    }
    return 0;                                                                       // 005D7147..005D714A
}
} // namespace sound_voice_required

namespace kotor_sound_path_required {
// 005D88F0 OcclusionRender(budget_word): time-sliced obstruction refresh over both voice lists (this+40 / this+44),
// resuming at the saved position (+A4) / list (+A8, 1 or 2); +AC/+B0 visit count/max, +B4 max elapsed, +B8 max
// recomputes, +BC max of the argument when a cached value is reused, +C0 budget, +C4 elapsed, +C8 recomputes.
void __fastcall occlusion_render_005d88f0(void* self, void* /*unused_edx*/, U argument) {
    U& position_cell = field<U>(self, 0xA4);
    U& list_cell = field<U>(self, 0xA8);
    const U start_position = position_cell;                                         // 005D88FD ebx -> [esp+24h]
    bool first = true;                                                              // [esp+28h] = 1
    U last = now_ms();                                                              // 005D8914..005D892E [esp+10h]
    void* pos = nullptr;                                                            // [esp+14h]
    auto advance_clock = [&]() {                                                    // timer; C4 += now - last; last = now
        const U now = now_ms();
        field<U>(self, 0xC4) = field<U>(self, 0xC4) + (now - last);
        last = now;
    };
    field<U>(self, 0xAC) = field<U>(self, 0xAC) + 1;                                // 005D8928..005D893B
    if (position_cell == 0) {                                                       // 005D8932..005D8941
        position_cell = static_cast<U>(reinterpret_cast<std::uintptr_t>(list_head(self, 0x40))); // 005D8943..005D8948
        list_cell = 1;                                                              // 005D894E
    }
    if (position_cell == 0) field<U>(self, 0xAC) = 0;                               // 005D8954..005D895E
    {
        const U elapsed = field<U>(self, 0xC4);                                     // 005D8964
        if (elapsed > field<U>(self, 0xB4)) field<U>(self, 0xB4) = elapsed;         // 005D896A..005D8972
        if (elapsed >= field<U>(self, 0xC0)) goto done;                             // 005D8978..005D897E
    }
check_list:                                                                         // 005D8984
    if (list_cell != 1) goto second_list_entry;                                     // 005D8984..005D898A
    pos = reinterpret_cast<void*>(static_cast<std::uintptr_t>(position_cell));      // 005D8990..005D8992
    if (pos == nullptr) goto switch_to_second;                                      // 005D8996
first_loop:                                                                         // 005D89A0
    if (field<U>(self, 0xC4) >= field<U>(self, 0xC0)) {                             // 005D89A0..005D89AC -> 005D8D09
        if (pos == nullptr) goto switch_to_second;                                  // 005D8D09..005D8D0B
        position_cell = static_cast<U>(reinterpret_cast<std::uintptr_t>(pos));      // 005D8D11
        goto after_switch_clock;                                                    // 005D8D17 -> 005D8BA3
    }
    {
        field<U>(self, 0xAC) = 0;                                                   // 005D89B6
        void* const item = item_at(self, 0x40, pos);                                // 005D89B2..005D89C1
        if (item != nullptr && sound_lower29::ActorPlaying005db0e0(item, nullptr) != 0 && // 005D89C3..005D89D4
            field<U>(item, 0x1C) != 0 && field<U>(item, 0x28) == 0) {               // 005D89DA..005D89E6
            Vector source = field<Vector>(item, 0x78);                              // 005D89EC..005D8A07 [esp+44h]
            Vector listener = field<Vector>(self, 0x98);                            // 005D89FF..005D8A22 [esp+2Ch]
            U stamp = now_ms();                                                     // 005D8A15..005D8A3D [esp+18h]
            U value = 0;                                                            // 005D8A52 [esp+1Ch]
            if (sound_voice_required::spatial_query_005d6ef0(self, nullptr, &source, &listener, &stamp, &value) != 0) { // 005D8A38..005D8A61
                if (argument > field<U>(self, 0xBC)) field<U>(self, 0xBC) = argument; // 005D8A63..005D8A6F
                field<Vector>(item, 0x8C) = listener;                               // 005D8A75..005D8A90
                field<U>(item, 0x98) = stamp;                                       // 005D8A97
                field<U>(item, 0x9C) = value;                                       // 005D8A9D
            } else {
                field<float>(item, 0x9C) = sound_voice_required::spatial_value_0050ccf0(
                    kotor_world_los::float_to_bits(source.x), kotor_world_los::float_to_bits(source.y),
                    kotor_world_los::float_to_bits(source.z), kotor_world_los::float_to_bits(listener.x),
                    kotor_world_los::float_to_bits(listener.y), kotor_world_los::float_to_bits(listener.z)); // 005D8AA5..005D8ADC
                field<Vector>(item, 0x8C) = listener;                               // 005D8AE2..005D8AF5
                field<U>(item, 0x98) = stamp;                                       // 005D8AF8
                const U recomputed = field<U>(self, 0xC8) + 1;                      // 005D8AFE..005D8B0F
                field<U>(self, 0xC8) = recomputed;
                if (recomputed > field<U>(self, 0xB8)) field<U>(self, 0xB8) = recomputed; // 005D8B0D..005D8B17
            }
            timer_only();                                                           // 005D8B1D..005D8B26
            sound_voice_required::actor_obstruction_005daeb0(item, nullptr);        // 005D8B2B..005D8B2D
        }
        list_next(self, 0x40, &pos);                                                // 005D8B38..005D8B40
        advance_clock();                                                            // 005D8B45..005D8B7A
        if (start_position == static_cast<U>(reinterpret_cast<std::uintptr_t>(pos))) goto save_and_finish; // 005D8B78..005D8B80
        if (pos != nullptr) goto first_loop;                                        // 005D8B86..005D8B88
    }
switch_to_second:                                                                   // 005D8B8E
    list_cell = 2;                                                                  // 005D8B91
    position_cell = static_cast<U>(reinterpret_cast<std::uintptr_t>(list_head(self, 0x44))); // 005D8B8E..005D8B9D
after_switch_clock:                                                                 // 005D8BA3
    advance_clock();                                                                // 005D8BA3..005D8BD4
second_list_entry:                                                                  // 005D8BD8
    if (!first && start_position == position_cell) goto clear_and_finish;           // 005D8BD8..005D8BE4 -> 005D8E80
    if (list_cell != 2) goto rotate;                                                // 005D8BEA..005D8BF1 -> 005D8E4E
    pos = reinterpret_cast<void*>(static_cast<std::uintptr_t>(position_cell));      // 005D8BF7..005D8BFF
    if (pos == nullptr) goto back_to_first;                                         // 005D8C03 -> 005D8E04
second_loop:                                                                        // 005D8C10
    if (field<U>(self, 0xC4) >= field<U>(self, 0xC0)) {                             // 005D8C10..005D8C1C -> 005D8E00
        if (pos != nullptr) goto store_position;                                    // 005D8E00..005D8E02 -> 005D8E13 (eax = pos)
        goto back_to_first;
    }
    {
        field<U>(self, 0xAC) = 0;                                                   // 005D8C26
        void* const item = item_at(self, 0x44, pos);                                // 005D8C22..005D8C31
        if (item != nullptr && sound_streaming29::InternalIsPlaying005dc300(item, nullptr) != 0 && // 005D8C33..005D8C44
            field<U>(item, 0x58) != 0) {                                            // 005D8C4A..005D8C4D
            Vector source = field<Vector>(item, 0x5C);                              // 005D8C53..005D8C6E [esp+50h]
            Vector listener = field<Vector>(self, 0x98);                            // 005D8C66..005D8C89 [esp+38h]
            U stamp = now_ms();                                                     // 005D8C7C..005D8C9F [esp+18h]
            U value = 0;                                                            // 005D8CB9 [esp+20h]
            if (sound_voice_required::spatial_query_005d6ef0(self, nullptr, &source, &listener, &stamp, &value) != 0) { // 005D8CA3..005D8CC8
                if (argument > field<U>(self, 0xBC)) field<U>(self, 0xBC) = argument; // 005D8CCA..005D8CD6
                field<Vector>(item, 0x70) = listener;                               // 005D8CDC..005D8CF4
                field<U>(item, 0x7C) = stamp;                                       // 005D8CFB
                field<U>(item, 0x80) = value;                                       // 005D8CFE
            } else {
                field<float>(item, 0x80) = sound_voice_required::spatial_value_0050ccf0(
                    kotor_world_los::float_to_bits(source.x), kotor_world_los::float_to_bits(source.y),
                    kotor_world_los::float_to_bits(source.z), kotor_world_los::float_to_bits(listener.x),
                    kotor_world_los::float_to_bits(listener.y), kotor_world_los::float_to_bits(listener.z)); // 005D8D1C..005D8D53
                field<Vector>(item, 0x70) = listener;                               // 005D8D59..005D8D69
                field<U>(item, 0x7C) = stamp;                                       // 005D8D6C
                const U recomputed = field<U>(self, 0xC8) + 1;                      // 005D8D6F..005D8D80
                field<U>(self, 0xC8) = recomputed;
                if (recomputed > field<U>(self, 0xB8)) field<U>(self, 0xB8) = recomputed; // 005D8D7E..005D8D88
            }
            timer_only();                                                           // 005D8D8E..005D8D96
            renderer_void_004015a0();                                               // 005D8D9B..005D8D9D call 004015A0 (ret)
        }
        list_next(self, 0x44, &pos);                                                // 005D8DA8..005D8DB0
        advance_clock();                                                            // 005D8DB5..005D8DEA
        if (start_position == static_cast<U>(reinterpret_cast<std::uintptr_t>(pos))) goto save_and_finish; // 005D8DE8..005D8DF0
        if (pos != nullptr) goto second_loop;                                       // 005D8DF6..005D8DF8
    }
back_to_first:                                                                      // 005D8E04
    list_cell = 1;                                                                  // 005D8E07
    pos = list_head(self, 0x40);                                                    // 005D8E04..005D8E11 (eax)
store_position:                                                                     // 005D8E13
    position_cell = static_cast<U>(reinterpret_cast<std::uintptr_t>(pos));
    advance_clock();                                                                // 005D8E19..005D8E4A
rotate:                                                                             // 005D8E4E
    field<U>(self, 0xC8) = 0;                                                       // 005D8E56
    if (start_position == position_cell) goto done;                                 // 005D8E4E..005D8E5C
    first = false;                                                                  // 005D8E6A
    if (field<U>(self, 0xC4) >= field<U>(self, 0xC0)) goto done;                    // 005D8E5E..005D8E6E
    goto check_list;                                                                // 005D8E70..005D8E75 (edi = 1)
save_and_finish:                                                                    // 005D8E7A
    position_cell = static_cast<U>(reinterpret_cast<std::uintptr_t>(pos));
clear_and_finish:                                                                   // 005D8E80
    field<U>(self, 0xC8) = 0;
done:                                                                               // 005D8E86
    {
        const U visits = field<U>(self, 0xAC);
        field<U>(self, 0xC4) = 0;                                                   // 005D8E92
        if (visits > field<U>(self, 0xB0)) field<U>(self, 0xB0) = visits;           // 005D8E8C..005D8E9A
    }
}                                                                                   // 005D8EA7 ret 4
} // namespace kotor_sound_path_required
