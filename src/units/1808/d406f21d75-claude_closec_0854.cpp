// closeC (Claude) for unit 0854: CSWSEffectListHandler summon callbacks of the 0854 ai_resolve table, and their missing
// callee CSWSCreature::UnsummonMyself.  Transcribed from the original swkotor.exe (read-only, capstone sweep); addresses
// in comments are original instructions.
//   004D8680 CSWSEffectListHandler::OnApplySummonCreature(CSWSObject*, CGameEffect*, int load_game) thiscall RET 0Ch
//            -> @kotor_fastcall_004d8680@20
//   004D8A10 CSWSEffectListHandler::OnRemoveSummonCreature(CSWSObject*, CGameEffect*)            thiscall RET 8
//            -> @kotor_fastcall_004d8a10@16
//   005149B0 CSWSCreature::UnsummonMyself()                                                       thiscall RET 0
//            -> @kotor_fastcall_005149b0@8
// The handler `self` is never read.  .rdata bound to proven values: [0073D700]=0.0f, [007473B8]=0 (word),
//   [00744214]=18h (summon AI event), [007466F4]=1Eh (word, visual-effect type), [007441E0]=0Bh (destroy AI event).
// EH: the original unwind frames only release the new'd blocks / CExoString locals when a callee throws; the bound callees
//   do not throw in normal operation, so that cleanup is not reproduced (as the sibling effects files).
// Quirks kept: OnApplySummon does not check the 0AC8h allocation (ComputeSafeLocation reads [creature+340h] and
//   LoadFromTemplate runs on the null result), the 8Ch effect / 34h event allocations are unchecked (null written
//   through), the 4th float (delay) is truncated by __ftol2 before the *1000 (whole seconds), OnRemoveSummon does not
//   check the effect, UnsummonMyself builds and destroys an unused CExoString and discards GetSWSMessage().
// x87: GetFloat results stored as dwords (float), delay compare `fcomp; test ah,41h` = ordered greater-than 0.0.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;
using U16 = std::uint16_t;

extern "C" {
// ---------------- existing providers (c35 map; object in brackets) ----------------
U __fastcall recovered_effect_integer_00503690(void* effect, void*, U index);                       // GetInteger RET 4 [W1 b7]
void __fastcall recovered_effect_integer_005036a0(void* effect, void*, U index, U value);           // SetInteger RET 8 [0886]
float __fastcall kotor_fastcall_005036c0(void* effect, void*, I index);                             // GetFloat (ST0) RET 4 [claude_effects_g4]
void __fastcall kotor_fastcall_005036d0(void* effect, void*, U index, U value_bits);                // SetFloat RET 8 [claude_w1_chains_damage]
U __fastcall kotor_fastcall_005036e0(void* effect, void*, U index);                                 // GetObjectID RET 4 [claude_effects_g3_a]
void __fastcall recovered_effect_set_object_id_005036f0(void* effect, void*, U index, U id);       // SetObjectID RET 8 [1194u]
void* __fastcall recovered_effect_copy_string_00503700(void* effect, void*, void* out, U index);   // CopyStringFromList RET 8 [w2_save_object]
void* __fastcall recovered_effect_construct_00503e40(void* effect, void*, U flag);                 // CGameEffect(int) RET 4 [0886]
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, const void* string);        // CResRef(CExoString const&) RET 4 [0200]
void* __fastcall recovered_server_creature_construct_004f7a10(void* memory, void*, U id, U flag);  // CSWSCreature() RET 8 [0963]
unsigned __fastcall w2x_creature_template_005026d0(void* creature, void*, void* ref, unsigned flag); // LoadFromTemplate RET 8 [0886]
unsigned __fastcall w1x_area_safe_location_004be860(void* area, void*, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e); // ComputeSafeLocation RET 14h [0886]
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                               // GetAreaByGameObjectID RET 4 [0530]
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                         // GetServerAIMaster [creature_destructor_r2]
U __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                           // GetSWSMessage [creature_destructor_r2]
void __fastcall kotor_fastcall_004d7720(void* data, void*, I index, float value);                  // CScriptEvent::SetFloat RET 8 [claude_combat_attack_damage]
void __fastcall kotor_fastcall_0050c6b0(void* area, void*, void* effect, const Vector* pos, void* orientation); // CSWSArea::ApplyEffect RET 0Ch [claude_aoe]
void __fastcall w5_object_clear_all_actions_004ccd80(void* object, void*, U clear_combat);         // ClearAllActions RET 4 [1206-07]
}

namespace {
constexpr U k_invalid_object = 0x7f000000u;
constexpr float k_zero_0073d700 = 0.0f;            // .rdata 0073D700
constexpr U16 k_duration_bits_007473b8 = 0;        // .rdata 007473B8 word
constexpr U k_event_summon_00744214 = 0x18;        // .rdata 00744214 dword
constexpr U16 k_effect_visual_007466f4 = 0x1e;     // .rdata 007466F4 word
constexpr U k_event_destroy_007441e0 = 0x0b;       // .rdata 007441E0 dword
struct ExoString { char* text; U length; };        // CExoString (8 bytes)
static_assert(sizeof(ExoString) == 8, "CExoString is 8 bytes");
inline void* as_creature(void* o) { return vcall<void*>(o, 0x30); }          // vtbl+30 AsSWSCreature
inline U addr(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline U fbits(float f) { U u; std::memcpy(&u, &f, 4); return u; }
inline float fval(U u) { float f; std::memcpy(&f, &u, 4); return f; }
// __ftol2 006FAE8C: truncating 64-bit conversion, low dword; NaN / out of range -> 0
inline U ftol2_low(double v) {
    if (!(v > -9223372036854775808.0 && v < 9223372036854775808.0)) return 0;
    return static_cast<U>(static_cast<std::uint64_t>(static_cast<std::int64_t>(v)));
}
}  // namespace

extern "C" {

// ===================================================================================================== 005149B0
// CSWSCreature::UnsummonMyself RET 0
void __fastcall kotor_fastcall_005149b0(void* self, void*) {
    ExoString unused{nullptr, 0};                                             // 005149CC..D0 CExoString() [esp+0Ch]
    (void)recovered_server_sws_message_004aedb0(server_app(), nullptr);       // 005149D5..E5 GetSWSMessage (discarded)
    w5_object_clear_all_actions_004ccd80(self, nullptr, 1);                   // 005149EA..EE ClearAllActions(1)
    void* const area = recovered_object_area_004cb120(self, nullptr);         // 005149F3..FA GetArea
    if (area != nullptr) {                                                    // 005149FC..FE
        void* const memory = live_operator_new(0x8c);                         // 00514A00..0A
        void* const effect = memory != nullptr ? recovered_effect_construct_00503e40(memory, nullptr, 1) : nullptr; // 00514A11..25
        at<U16>(effect, 8) = k_effect_visual_007466f4;                        // 00514A27..34
        at<U16>(effect, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(effect, 0xa)) & 0xfff8u) | k_duration_bits_007473b8); // 00514A2E..53
        kotor_fastcall_0050c6b0(area, nullptr, effect, reinterpret_cast<const Vector*>(static_cast<unsigned char*>(self) + 0x90),
                                static_cast<unsigned char*>(self) + 0x9c);    // 00514A38..5F ApplyEffect(e, +90, +9C)
    }
    const U id = at<U>(self, 4);                                              // 00514A6A
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server_app(), nullptr), nullptr, 0, 0, id, id,
                                     k_event_destroy_007441e0, nullptr);      // 00514A64..85 AddEventDeltaTime(0,0,id,id,0Bh,0)
    frame_LogStringDtor_005e5c20(&unused, nullptr);                           // 00514A8A..96
}

// ===================================================================================================== 004D8680
// OnApplySummonCreature RET 0Ch: spawn the creature template (string 0) near the location in object/floats 0..2, apply the
// optional visual effect (integer 0) there and queue the summon AI event after floor(float 3) seconds.
unsigned __fastcall kotor_fastcall_004d8680(void*, void*, void* object, void* effect, unsigned load_game) {
    if (load_game == 1) return 0;                                             // 004D869C..9F
    void* const owner = as_creature(object);                                  // 004D86A6..B0 vtbl+30 -> [esp+18h]
    if (effect == nullptr || owner == nullptr) return 0;                      // 004D86B7..C7
    void* const memory = live_operator_new(0xac8);                            // 004D86CD..D7
    void* creature = nullptr;                                                 // 004D86B7 xor ebp,ebp
    if (memory != nullptr) creature = recovered_server_creature_construct_004f7a10(memory, nullptr, k_invalid_object, 0); // 004D86E4..F3
    if (kotor_fastcall_005036e0(effect, nullptr, 0) == k_invalid_object) {    // 004D86F9..0B
        recovered_effect_set_object_id_005036f0(effect, nullptr, 0, at<U>(object, 0x8c)); // 004D870D..18
        kotor_fastcall_005036d0(effect, nullptr, 0, at<U>(object, 0x90));    // 004D871D..28
        kotor_fastcall_005036d0(effect, nullptr, 1, at<U>(object, 0x94));    // 004D872D..38
        kotor_fastcall_005036d0(effect, nullptr, 2, at<U>(object, 0x98));    // 004D873D..48
    }
    void* const server = server_app();                                        // 004D874D..53
    void* const area = kotor_fastcall_004ae780(server, nullptr, kotor_fastcall_005036e0(effect, nullptr, 0)); // 004D8756..62
    if (area == nullptr) {                                                    // 004D8767..6B
        if (creature != nullptr) vcall<void>(creature, 0, 1u);                // 004D89E2..ED scalar deleting dtor(1)
        return 0;
    }
    float x = kotor_fastcall_005036c0(effect, nullptr, 0);                    // 004D8771..7A -> [E+0Ch]
    float y = kotor_fastcall_005036c0(effect, nullptr, 1);                    // 004D877E..87 -> [E+8]
    float z = kotor_fastcall_005036c0(effect, nullptr, 2);                    // 004D878B..94 -> [E-58h]
    float delay = 0.0f;                                                       // 004D879C [E-54h] = 0
    if (kotor_fastcall_005036c0(effect, nullptr, 3) > k_zero_0073d700)       // 004D87A4..B4 fcomp; test ah,41h
        delay = kotor_fastcall_005036c0(effect, nullptr, 3);                  // 004D87B6..BF
    unsigned char resref[0x10];                                               // [E-1Ch] CResRef
    {
        ExoString name{nullptr, 0};                                           // [E-48h]
        void* const s = recovered_effect_copy_string_00503700(effect, nullptr, &name, 0); // 004D87C3..CC
        async_ref_construct_string_00406d60(resref, nullptr, s);              // 004D87D1..DE
        frame_LogStringDtor_005e5c20(&name, nullptr);                         // 004D87E3..EB
    }
    w2x_creature_template_005026d0(creature, nullptr, resref, 0);             // 004D87F0..F9
    Vector position; position.x = x; position.y = y; position.z = z;          // 004D87FE..20 [E-34h]
    Vector safe; safe.x = 0.0f; safe.y = 0.0f; safe.z = 0.0f;                 // 004D8830..40 [E-40h]
    // 004D880E..48: ComputeSafeLocation(&position, 20.0, creature+340, 0, &safe) (creature unchecked)
    if (w1x_area_safe_location_004be860(area, nullptr, addr(&position), 0x41a00000u, at<U>(creature, 0x340), 0,
                                        addr(&safe)) == 1) {                  // 004D884D..50
        y = safe.y;                                                           // 004D8852..62
        x = safe.x;                                                           // 004D8856..69
        z = fval(w1x_area_height_bits_004bc380(area, nullptr, &safe, 0, nullptr)); // 004D885A..75 ComputeHeight(&safe,0,0)
    } else {
        x = position.x; y = position.y; z = position.z;                       // 004D887B..8F
    }
    const U visual = recovered_effect_integer_00503690(effect, nullptr, 0);   // 004D8893..9E -> [E-50h]
    if (visual != 0xffffffffu) {                                              // 004D889C..A2
        void* const em = live_operator_new(0x8c);                             // 004D88A8..B2
        void* const vfx = em != nullptr ? recovered_effect_construct_00503e40(em, nullptr, 1) : nullptr; // 004D88B9..D2
        at<U16>(vfx, 8) = 0x1e;                                               // 004D88DE
        at<U16>(vfx, 0xa) = static_cast<U16>((static_cast<U>(at<U16>(vfx, 0xa)) & 0xfff8u) | k_duration_bits_007473b8); // 004D88DA..FD
        recovered_effect_integer_005036a0(vfx, nullptr, 0, visual);           // 004D88E4..01 SetInteger(0, visual)
        Vector at_point; at_point.x = x; at_point.y = y; at_point.z = z;      // 004D8906..2A [E-28h]
        kotor_fastcall_0050c6b0(area, nullptr, vfx, &at_point, static_cast<unsigned char*>(owner) + 0x9c); // 004D8916..31
    }
    const U new_id = at<U>(creature, 4);                                      // 004D8936
    void* const evm = live_operator_new(0x34);                                // 004D8939..40
    void* const event = evm != nullptr ? recovered_event_construct_004d7540(evm, nullptr) : nullptr; // 004D8947..5E
    kotor_fastcall_004d7720(event, nullptr, 0, x);                            // 004D8960..71
    kotor_fastcall_004d7720(event, nullptr, 1, y);                            // 004D8976..7F
    kotor_fastcall_004d7720(event, nullptr, 2, z);                            // 004D8984..8D
    recovered_event_object_004d7780(event, nullptr, 0, new_id);               // 004D8992..97
    const U caller = at<U>(object, 4);                                        // 004D89A0..A4
    const U ms = ftol2_low(static_cast<double>(delay)) * 1000u;               // 004D899C, 004D89BA..BF __ftol2; imul 3E8h
    recovered_event_enqueue_004b08d0(recovered_server_ai_master_004aed80(server, nullptr), nullptr, 0, ms, caller, caller,
                                     k_event_summon_00744214, event);         // 004D89A7..D1 AddEventDeltaTime
    recovered_effect_set_object_id_005036f0(effect, nullptr, 1, new_id);      // 004D89D6..DB SetObjectID(1, id)
    return 0;                                                                 // 004D89F7
}

// ===================================================================================================== 004D8A10
// OnRemoveSummonCreature RET 8: unsummon the creature recorded in object id 1 (effect unchecked).
unsigned __fastcall kotor_fastcall_004d8a10(void*, void*, void*, void* effect) {
    void* const creature = kotor_fastcall_004ae770(server_app(), nullptr, kotor_fastcall_005036e0(effect, nullptr, 1)); // 004D8A10..27
    if (creature != nullptr) kotor_fastcall_005149b0(creature, nullptr);      // 004D8A2C..33
    return 1;                                                                 // 004D8A38
}

}  // extern "C"
