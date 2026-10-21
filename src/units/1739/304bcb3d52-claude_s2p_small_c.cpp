// Lane worker "s2p-small": client server-to-player handlers (CSWCMessage, client dispatcher 0066A640), part C:
//   00663220 CheatMessage (+00651C10 Cheat_DoNasty, 005ED850 CClientExoApp::SetDebugMode, 005F1490 its internal
//   setter), 00663810 MapPin (+00652D00 MapPinEnabled, 00652D60 MapPinReferenceNumber, 00605AC0 CSWCArea::GetMapPin,
//   00606D90 CSWCArea::AddMapPin), 00654700 CharacterDownload (+005ED410 CClientExoApp::SetCharacterFile,
//   005F1A90 CClientExoAppInternal::SetCharacterFile).
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Handler ABI: thiscall(CSWCMessage* this, BYTE minor) RET 4 -> extern "C" U __fastcall kotor_fastcall_<addr>(self, edx,
// minor) @12.
#include "b0bf16f6a8-claude_s2p_small_common.hpp"

using namespace s2p;

namespace {
struct V3 { float x, y, z; };                     // Vector (passed by value: 3 dwords)
struct Q4 { float x, y, z, w; };                  // Quaternion (passed by value: 4 dwords)
static_assert(sizeof(V3) == 12 && sizeof(Q4) == 16, "by-value math types");
}  // namespace

extern "C" {
// real providers (c19 map; none in the c19 trap census)
void* __cdecl aurora_new_aur_object_00449cc0(const char* model, const char* name, void* a, void* b); // NewCAurObject
void* __fastcall quaternion_construct_004ac960(void* q, void*, V3 axis, float angle);             // Quaternion(Vector, float) RET 10h
void* __cdecl quaternion_multiply_004a9ac0(void* out, const void* a, const void* b);               // operator*(Quat, Quat)
void* __fastcall vector_add_assign_004a9740(void* v, void*, const void* other);                    // Vector::operator+= RET 4
double __cdecl kotor_m15a_pitch_004aa130(const void* v);                                          // Pitch (ST0)
double __cdecl kotor_m15a_yaw_004aa0f0(const void* v);                                            // Yaw (ST0)
void* __cdecl yaw_pitch_roll_004acac0(void* out, float yaw, float pitch, float roll);             // YawPitchRoll

// exports of this file besides the handler table entries
U __fastcall kotor_fastcall_00651c10(void* self, void*);                                         // Cheat_DoNasty RET 0
void __fastcall kotor_fastcall_005ed850(void* client, void*, U mode);                            // SetDebugMode RET 4
void __fastcall kotor_fastcall_005f1490(void* internal, void*, U mode);                          // (internal setter) RET 4
U __fastcall kotor_fastcall_00652d00(void* self, void*);                                         // MapPinEnabled RET 0
U __fastcall kotor_fastcall_00652d60(void* self, void*);                                         // MapPinReferenceNumber RET 0
void* __fastcall kotor_fastcall_00605ac0(void* area, void*, U id, U reference);                  // CSWCArea::GetMapPin RET 8
void __fastcall kotor_fastcall_00606d90(void* area, void*, void* pin);                           // CSWCArea::AddMapPin RET 4
void __fastcall kotor_fastcall_005ed410(void* client, void*, U size, const void* data, U flag);  // SetCharacterFile RET 0Ch
void __fastcall kotor_fastcall_005f1a90(void* internal, void*, U size, const void* data, U flag); // RET 0Ch
}

namespace {
inline U overflow(void* m) { return kotor_fastcall_004d6230(m, nullptr, 1); }
inline U underflow(void* m) { return kotor_fastcall_004d6240(m, nullptr, 1); }
inline void* module() { return kotor_fastcall_005ed530(client(), nullptr); }      // CClientExoApp::GetModule
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// 005F1490 CClientExoAppInternal (+194h debug mode) setter RET 4; 005ED850 CClientExoApp::SetDebugMode RET 4 (jmp)
// ---------------------------------------------------------------------------------------------------------------
extern "C" void __fastcall kotor_fastcall_005f1490(void* internal, void*, U mode) {
    wr(internal, 0x194, mode);                                                          // 005F1490..005F149A
}

extern "C" void __fastcall kotor_fastcall_005ed850(void* client_app, void*, U mode) {
    kotor_fastcall_005f1490(rp(client_app, 4), nullptr, mode);                          // 005ED850..005ED853
}

// ---------------------------------------------------------------------------------------------------------------
// 00651C10 CSWCMessage::HandleServerToPlayerCheat_DoNasty() RET 0
// Replaces the creature's +12Ch Aurora object with a "VWP_L_Blood_nast" model named "blood", plays sound-set entry
// 0Ah, places it at the creature's "root" node rotated by -pi/2 about X, offset +0.4 in z plus the creature position
// (+24h), oriented from the negated creature direction (+30h), and plays its "impact" animation.
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00651c10(void* self, void*) {
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 00651C15..00651C20 edi
    if (overflow(self) != 0) return 0;                                                  // 00651C1C..00651C29 -> 00651C9D
    void* object;                                                                       // [esp+8]
    if (kotor_fastcall_004d8230(kotor_fastcall_005ed570(client(), nullptr), nullptr, id, &object) != kStatusOk)
        return 1;                                                                       // 00651C2B..00651C4B
    if (vcall<void*>(object, 0x2c) == nullptr) return 1;                                // 00651C51..00651C5C AsSWCCreature
    void* const creature = vcall<void*>(object, 0x2c);                                  // 00651C62..00651C6B esi
    void* const old = rp(creature, 0x12c);                                              // 00651C6D
    if (old != nullptr) vcall<void>(old, 0x0, 1u);                                      // 00651C73..00651C7B deleting dtor
    void* const gob = aurora_new_aur_object_00449cc0("VWP_L_Blood_nast", "blood", nullptr, nullptr); // 00651C7D..00651C90
    wp(creature, 0x12c, gob);                                                           // 00651C95
    if (gob == nullptr) return 0;                                                       // 00651C93..00651C9B
    (void)recovered_client_creature_play_sound_set_sound_00611470(creature, nullptr, 0xa, 1); // 00651CA5..00651CAB
    Q4 orientation = {1.0f, 0.0f, 0.0f, 0.0f};                                          // 00651CB5..00651CCD [esp+38h]
    V3 position = {0.0f, 0.0f, 0.0f};                                                   // 00651CD5..00651CE5 [esp+0Ch]
    vcall<void>(rp(creature, 0x12c), 0xa0, "root", static_cast<void*>(&position),
                static_cast<void*>(&orientation));                                      // 00651CB0..00651CFF
    Q4 rotation;                                                                        // [esp+28h]
    const V3 axis = {1.0f, 0.0f, 0.0f};                                                 // 00651D0A..00651D38 (by value)
    void* const r = quaternion_construct_004ac960(&rotation, nullptr, axis, -1.5707963705062866f); // 00651D05..00651D3F (BFC90FDBh)
    Q4 product;                                                                         // [esp+18h]
    const Q4 rotated = *static_cast<const Q4*>(quaternion_multiply_004a9ac0(&product, &orientation, r)); // 00651D44..00651D72
    vcall<void>(rp(creature, 0x12c), 0x9c, "root", position, rotated);                  // 00651D75..00651D99
    vcall<void>(rp(creature, 0x12c), 0x98, "root", static_cast<void*>(&position),
                static_cast<void*>(&orientation));                                      // 00651D9F..00651DB6
    position.z = position.z + 0.4000000059604645f;                                      // 00651DBC..00651DCE (fadd [0073E320])
    (void)vector_add_assign_004a9740(&position, nullptr, at(creature, 0x24));           // 00651DC6..00651DD2
    unsigned char scratch_a[16];                                                        // [esp+28h] (hidden result)
    vcall<void>(rp(creature, 0x12c), 0x5c, static_cast<void*>(scratch_a), position);    // 00651DD7..00651DFD
    V3 back_p = {-rdf(creature, 0x30), -rdf(creature, 0x34), -rdf(creature, 0x38)};     // 00651E00..00651E1E [esp+18h]
    V3 back_y = {-rdf(creature, 0x30), -rdf(creature, 0x34), -rdf(creature, 0x38)};     // 00651E22..00651E39 [esp+28h]
    const float pitch = static_cast<float>(kotor_m15a_pitch_004aa130(&back_p));         // 00651E3D..00651E42 fstp dword
    const float yaw = static_cast<float>(kotor_m15a_yaw_004aa0f0(&back_y));             // 00651E45..00651E4F fstp dword
    Q4 facing;                                                                          // [esp+48h]
    const Q4 facing_value = *static_cast<const Q4*>(yaw_pitch_roll_004acac0(&facing, yaw, pitch, 0.0f)); // 00651E52..00651E57
    unsigned char scratch_b[16];                                                        // [esp+58h] (hidden result)
    vcall<void>(rp(creature, 0x12c), 0x60, static_cast<void*>(scratch_b), facing_value); // 00651E5C..00651E87
    vcall<void>(rp(creature, 0x12c), 0x4c, rd(rp(creature, 0x20), 0x184));             // 00651E8A..00651E9C
    vcall<void>(rp(creature, 0x12c), 0x18, "impact", 1.0f, 1u, 0u);                     // 00651E9F..00651EB5 (00743318)
    return 1;                                                                           // 00651EB8
}

// ---------------------------------------------------------------------------------------------------------------
// 00663220 CSWCMessage::HandleServerToPlayerCheatMessage(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00663220(void* self, void*, U minor) {
    switch (static_cast<U8>(minor)) {                                                   // 00663220..00663233
    case 0x0b:
        return kotor_fastcall_00651c10(self, nullptr);                                  // 00663291..00663299
    case 0x0c:
        (void)kotor_fastcall_00692630(self, nullptr);                                   // 00663271..00663273
        (void)kotor_fastcall_004d66f0(self, nullptr);                                   // 00663278..0066327A
        return overflow(self) == 0 ? 1u : 0u;                                           // 0066327F..0066328E (neg/sbb/inc)
    case 0x15: {
        const U mode = kotor_fastcall_004d66f0(self, nullptr);                          // 0066323B..00663247 edi
        if (overflow(self) != 0) return 0;                                              // 00663243..00663256
        kotor_fastcall_005ed850(client(), nullptr, mode);                               // 00663259..00663262
        return 1;                                                                       // 00663268
    }
    default:
        return 0;                                                                       // 00663235
    }
}

// ---------------------------------------------------------------------------------------------------------------
// 00605AC0 CSWCArea::GetMapPin(OBJECT_ID id, int reference) RET 8 (+1C4h pins, +1C8h count)
// ---------------------------------------------------------------------------------------------------------------
extern "C" void* __fastcall kotor_fastcall_00605ac0(void* area, void*, U id, U reference) {
    const I count = rdi(area, 0x1c8);                                                   // 00605AC2
    void* const* pins = static_cast<void* const*>(rp(area, 0x1c4));                     // 00605ACF
    for (I i = 0; i < count; ++i) {                                                     // 00605AC8..00605AFC
        void* const pin = pins[i];                                                      // 00605AE0
        if (pin != nullptr && rd(pin, 0x108) == id && rd(pin, 0x10c) == reference) return pin; // 00605AE2..00605AF4
    }
    return nullptr;                                                                     // 00605AFE
}

// ---------------------------------------------------------------------------------------------------------------
// 00606D90 CSWCArea::AddMapPin(CSWCMapPin* pin) RET 4 (CExoArrayList at +1C4h; grows to 2x or 10h)
// ---------------------------------------------------------------------------------------------------------------
extern "C" void __fastcall kotor_fastcall_00606d90(void* area, void*, void* pin) {
    void* const list = at(area, 0x1c4);                                                 // 00606D97
    const U capacity = rd(area, 0x1cc);                                                 // 00606D90
    if (rd(list, 4) == capacity) list_allocate(list, capacity != 0 ? capacity + capacity : 0x10); // 00606D9D..00606DB2
    wp(rp(list, 0), rd(list, 4) * 4, pin);                                              // 00606DB7..00606DC0
    wr(list, 4, rd(list, 4) + 1);                                                       // 00606DC3
}

// ---------------------------------------------------------------------------------------------------------------
// 00652D00 CSWCMessage::HandleServerToPlayerMapPinEnabled() RET 0
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00652d00(void* self, void*) {
    if (overflow(self) != 0) return 0;                                                  // 00652D01..00652D0C
    const U id = kotor_fastcall_00692630(self, nullptr);                                // 00652D14..00652D1D edi
    const U enabled = kotor_fastcall_004d66f0(self, nullptr);                           // 00652D1B..00652D24 ebx
    void* const pin = kotor_fastcall_00605ac0(rp(module(), 0x48), nullptr, id, 0);      // 00652D26..00652D3B
    if (pin != nullptr) wr(pin, 0xfc, enabled);                                         // 00652D40..00652D44
    return underflow(self) == 0 ? 1u : 0u;                                              // 00652D4A..00652D5B (neg/sbb/inc)
}

// ---------------------------------------------------------------------------------------------------------------
// 00652D60 CSWCMessage::HandleServerToPlayerMapPinReferenceNumber() RET 0 (adds a new CSWCMapPin, 110h bytes)
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00652d60(void* self, void*) {
    float position[3] = {0.0f, 0.0f, 0.0f};                                             // 00652D80..00652D90 [esp+1Ch]
    ExoString note;                                                                     // 00652D7C..00652D98 [esp+0Ch]
    if (overflow(self) != 0) return 0;                                                  // 00652D9F..00652DAE (~note)
    position[0] = kotor_fastcall_004d67b0(self, nullptr, 1.0f, 0x20);                   // 00652DD5..00652DE3
    position[1] = kotor_fastcall_004d67b0(self, nullptr, 1.0f, 0x20);                   // 00652DE7..00652DF5
    position[2] = kotor_fastcall_004d67b0(self, nullptr, 1.0f, 0x20);                   // 00652DF9..00652E07
    {
        StrTemp t;                                                                      // [esp+18h]
        str_assign(&note, kotor_fastcall_004d67e0(self, nullptr, &t, 0x20));            // 00652E0B..00652E23
        frame_LogStringDtor_005e5c20(&t, nullptr);                                      // 00652E28..00652E30
    }
    const U reference = kotor_fastcall_004d6780(self, nullptr, 0x20);                   // 00652E35..00652E42 ebp
    if (underflow(self) != 0) return 0;                                                 // 00652E3E..00652E4B (~note)
    void* const area = rp(module(), 0x48);                                              // 00652E72..00652E81 edi
    void* const memory = live_operator_new(0x110);                                      // 00652E84..00652E8E
    void* pin = nullptr;                                                                // esi (null when the allocation fails:
    if (memory != nullptr) {                                                            //   the vtable read below then faults)
        try {
            pin = recovered_client_map_pin_construct_00692540(memory, nullptr);         // 00652E9E..00652EA5
        } catch (...) {
            live_free(memory);                                                          // EH state 2: operator delete
            throw;
        }
    }
    vcall<void>(pin, 0x8c, static_cast<const void*>(position));                         // 00652EAB..00652EB8 SetPosition
    str_assign(at(pin, 0x100), &note);                                                  // 00652EBE..00652EC9
    wr(pin, 0x108, reference);                                                          // 00652ED6
    wr(pin, 0x10c, 1);                                                                  // 00652EDC
    wr(pin, 0xfc, 1);                                                                   // 00652EE2
    kotor_fastcall_00606d90(area, nullptr, pin);                                        // 00652ED3..00652EE8
    return 1;                                                                           // 00652EED..00652F12 (~note)
}

// ---------------------------------------------------------------------------------------------------------------
// 00663810 CSWCMessage::HandleServerToPlayerMapPin(BYTE minor) RET 4
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00663810(void* self, void*, U minor) {
    switch (static_cast<U8>(minor)) {                                                   // 00663810..0066381B
    case 1: return kotor_fastcall_00652d00(self, nullptr);                              // 0066382A
    case 4: return kotor_fastcall_00652d60(self, nullptr);                              // 00663822
    default: return 0;                                                                  // 0066381D
    }
}

// ---------------------------------------------------------------------------------------------------------------
// 005F1A90 CClientExoAppInternal::SetCharacterFile(DWORD size, void* data, int flag) RET 0Ch
// (+1B4h copy (size + 1 bytes, NUL-terminated), +1B8h size, +1BCh flag)
// ---------------------------------------------------------------------------------------------------------------
extern "C" void __fastcall kotor_fastcall_005f1a90(void* internal, void*, U size, const void* data, U flag) {
    if (rd(internal, 0x1b4) != 0) {                                                     // 005F1A93..005F1A9C
        live_free(rp(internal, 0x1b4));                                                 // 005F1A9E..005F1AA4
        wr(internal, 0x1b4, 0);                                                         // 005F1AA7
    }
    if (data != nullptr) {                                                              // 005F1AB1..005F1AB7
        char* const copy = static_cast<char*>(live_operator_new(size + 1));             // 005F1ABE..005F1AC3
        wp(internal, 0x1b4, copy);                                                      // 005F1ACF
        std::memcpy(copy, data, size);                                                  // 005F1AC8..005F1AE1 (rep movsd/movsb)
        copy[size] = 0;                                                                 // 005F1AE3..005F1AED
    }
    wr(internal, 0x1b8, size);                                                          // 005F1AF2 / 005F1B0D
    wr(internal, 0x1bc, flag);                                                          // 005F1AFA / 005F1B13
}

// 005ED410 CClientExoApp::SetCharacterFile RET 0Ch (jmp to the internal)
extern "C" void __fastcall kotor_fastcall_005ed410(void* client_app, void*, U size, const void* data, U flag) {
    kotor_fastcall_005f1a90(rp(client_app, 4), nullptr, size, data, flag);              // 005ED410..005ED413
}

// ---------------------------------------------------------------------------------------------------------------
// 00654700 CSWCMessage::HandleServerToPlayerCharacterDownload(BYTE minor) RET 4
// Minor 2: two localized strings (read into otherwise unused locals), a size and a raw blob -> SetCharacterFile.
// Minor 3: a file name and a flag -> SetCharacterFile(length, text, flag); returns 1 without the final
// overflow/underflow checks (original). Other minors return 0.
// ---------------------------------------------------------------------------------------------------------------
extern "C" U __fastcall kotor_fastcall_00654700(void* self, void*, U minor) {
    ExoString name;                                                                     // 00654723..00654727 [esp+10h]
    ExoString first;                                                                    // 0065472C..00654738 [esp+20h]
    ExoString second;                                                                   // 0065473D..00654746 [esp+18h]
    if (overflow(self) != 0) return 0;                                                  // 00654750..0065475F -> 006548BC
    const U8 m = static_cast<U8>(minor);                                                // 00654765
    if (m == 2) {                                                                       // 00654769..0065476B
        {
            ResRef sound;                                                               // [esp+40h]
            StrTemp t;                                                                  // [esp+30h]
            str_assign(&first, kotor_fastcall_00692660(self, nullptr, &t, kotor_fastcall_00405ed0(&sound, nullptr),
                                                       nullptr));                       // 006547CF..006547F1
            frame_LogStringDtor_005e5c20(&t, nullptr);                                  // 006547F6..006547FE
        }
        {
            ResRef sound;                                                               // [esp+50h]
            StrTemp t;                                                                  // [esp+38h]
            str_assign(&second, kotor_fastcall_00692660(self, nullptr, &t, kotor_fastcall_00405ed0(&sound, nullptr),
                                                        nullptr));                      // 00654803..00654825
            frame_LogStringDtor_005e5c20(&t, nullptr);                                  // 0065482A..00654832
        }
        const U size = kotor_fastcall_004d6780(self, nullptr, 0x20);                    // 00654837..00654840 edi
        void* const data = kotor_fastcall_004d6200(self, nullptr, size);                // 00654842..00654845
        kotor_fastcall_005ed410(client(), nullptr, size, data, 0);                      // 0065484A..00654857
        if (overflow(self) != 0) return 0;                                              // 0065485C..00654867
        if (underflow(self) != 0) return 0;                                             // 00654869..00654874
        return 1;                                                                       // 00654876..006548B9
    }
    if (m == 3) {                                                                       // 0065476D..0065476E
        {
            StrTemp t;                                                                  // [esp+28h]
            str_assign(&name, kotor_fastcall_004d67e0(self, nullptr, &t, 0x20));        // 00654774..0065478C
            frame_LogStringDtor_005e5c20(&t, nullptr);                                  // 00654791..00654799
        }
        const U flag = kotor_fastcall_004d66f0(self, nullptr);                          // 0065479E..006547A0
        void* const client_app = client();                                              // 006547A5..006547AB esi
        const char* const text = WindowCStr005e5670(&name, nullptr);                    // 006547AF..006547B3
        const U length = async_string_length_005e5790(&name, nullptr);                  // 006547B9..006547BD
        kotor_fastcall_005ed410(client_app, nullptr, length, text, flag);               // 006547C2..006547C5
        return 1;                                                                       // 006547CA -> 00654876
    }
    return 0;                                                                           // 0065476E -> 006548BC
}
