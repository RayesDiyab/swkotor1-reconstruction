// Lane endar-actions, group "world": NWScript engine-command bodies reconstructed from the original
// swkotor.exe (read-only, capstone linear sweep).  Addresses in comments are original instructions.
//   0052F2F0 ExecuteCommandChangeFaction        (173 ChangeFaction, 412 ChangeToStandardFaction)
//   0054A260 ExecuteCommandGetObjectInShape     (128 GetFirstObjectInShape, 129 GetNextObjectInShape)
//   00549360 ExecuteCommandGetInPersistentObject (262 GetFirstInPersistentObject, 263 GetNextInPersistentObject)
// CreateObject 0052F820 is NOT implemented here (missing provider cascade; see the lane report).
// Conventions: claude_vm_actions_common.hpp.  x87 at 53-bit precision: intermediates are double, every
// `fstp dword` is a float store; register-resident values stay double exactly as the original keeps them.
#include "061872cb05-claude_vm_actions_common.hpp"

extern "C" {
// ---- existing providers (c17 map; owner object in brackets) ----
void* __fastcall kotor_fastcall_004b2820(void* server, void*);                                 // CServerExoApp::GetPlayerCreature [0595]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                              // CServerExoApp::GetModule [0530]
void* __fastcall module_get_area_004c30f0(void* module, void*);                                // CSWSModule::GetArea [0530]
unsigned __fastcall recovered_area_index_00506f20(void* area, void*, unsigned* out, unsigned x_bits); // CSWSArea::GetFirstObjectIndiceByX [1065]
U __fastcall w5_trigger_in_trigger_0058ce40(void* trigger, void*, const Vector* p);             // CSWSTrigger::InTrigger [1206-03 w5_subarea]
unsigned __fastcall recovered_encounter_in_activate_area_00590090(void* encounter, void*, void* pos); // CSWSEncounter::GetInActivateArea [w2_party_native]
void* __fastcall recovered_aoe_position_005946e0(void* aoe, void*, void* out);                 // CSWSAreaOfEffectObject::GetPosition [w2_party_native]
unsigned __fastcall recovered_aoe_in_area_of_effect_00595450(void* aoe, void*, float x, float y, float z); // InAreaOfEffect(Vector) [w2_party_native]
Vector* __cdecl w5_line_project_004a98b0(Vector* out, const Vector* a, const Vector* b, const Vector* p); // lineproject [1206-11]
double __cdecl recovered_dot_product_00423c20(const Vector* a, const Vector* b);               // dotProd, ST0 [claude_w3_update]
double __cdecl vector_magnitude_004ab100(const Vector* v);                                     // vector_magnitude, ST0 [0464]
void* __fastcall kotor_fastcall_004ae770(void* server, void*, U id);                           // GetCreatureByGameObjectID [0595]
unsigned __fastcall recovered_faction_is_npc_0052b280(void* manager, void*, unsigned faction);  // CFactionManager::GetIsNPCFaction [w1_native_b7]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);               // GetPlaceableByGameObjectID [0886]
void __fastcall w2x_faction_add_member_005bfa70(void* faction, void*, U id, U flag);           // CSWSFaction::AddMember [0886]
int __fastcall recovered_server_faction_of_object_004b40c0(void* internal, void*, U id, U* out); // CServerExoAppInternal::GetFactionOfObject [vm_actions_t4]
// ---- reconstructed in this file (shared-helper section at the end) ----
int __fastcall kotor_fastcall_004ae950(void* server, void*, U id, U* out);                     // CServerExoApp::GetFactionOfObject
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction);                     // CFactionManager::GetFaction
}

namespace {
using namespace vmx;
constexpr U k_routine_change_faction = 0xad;            // 0052F317 / 0052F38B
constexpr U k_routine_change_standard_faction = 0x19c;  // 0052F341 / 0052F408
constexpr int k_routine_first_in_shape = 0x80;          // 0054A71B
constexpr int k_routine_first_in_persistent = 0x106;    // 0054944D / 005496A1 / 0054996C
constexpr I k_engine_location = 2;                      // StackPopEngineStructure type (push 2)
constexpr U8 k_type_bound_00746633 = 4;                 // .rdata 00746633 (byte), jbe => object types above it
const float k_tiny_00741870 = 0.0001f;                  // .rdata 00741870 (0x38D1B717)
const float k_zero_0073d700 = 0.0f;                     // .rdata 0073D700
const float k_radius2_00747d7c = 2.25f;                 // .rdata 00747D7C (0x40100000)
const float k_cos_00748744 = 0.866f;                    // .rdata 00748744 (0x3F5DB22D)
const float k_half_width_0073f414 = 1.5f;               // .rdata 0073F414

inline void* server() { return server_app(); }
inline void* internal() { return at<void*>(server_app(), 4); }
inline U8 type_of(void* o) { return at<U8>(o, 8); }
inline bool pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v) != 0; }
inline bool pop_object(U* v) { return recovered_vm_outer_pop_object_005d10c0(vm(), nullptr, v) != 0; }
// neg/sbb/and 7D0h/add 0FFFFF830h
inline int push_object(U id) { return recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id) ? 0 : k_push_failed; }
// CScriptLocation teardown as the handlers inline it: dtor 004015A0 is a bare RET (no-op), then free 006FA390.
inline void free_location(void* location) { if (location) live_free(location); }
inline Vector vector_at(void* o, U offset) { Vector v; std::memcpy(&v, static_cast<unsigned char*>(o) + offset, 12); return v; }
// area object list: +190 ids, +194 count (signed compares in all callers)
inline U area_list_id(void* area, I index) { return at<U*>(area, 0x190)[index]; }
inline I area_count(void* area) { return at<I>(area, 0x194); }

// 00549508..00549588 (and the two copies at 00549748/00549A1C): exact-mask -> object type byte
bool persistent_type_matches(I mask, U type) {
    switch (mask) {
    case 1: return type == 5;        // creature
    case 2: return type == 6;        // item
    case 4: return type == 7;        // trigger
    case 8: return type == 0xa;      // door
    case 0x10: return type == 0xb;   // area of effect
    case 0x20: return type == 0xc;   // waypoint
    case 0x40: return type == 9;     // placeable
    case 0x80: return type == 0xe;   // store
    case 0x100: return type == 0xd;  // encounter
    case 0x200: return type == 0x10; // sound
    case 0x7fff: return true;        // all
    default: return false;
    }
}
// 0054A814..0054A893: bit tests in this exact order; the type byte is read only for a set bit
bool shape_filter_matches(U mask, void* o) {
    if ((mask & 1) && type_of(o) == 5) return true;
    if ((mask & 2) && type_of(o) == 6) return true;
    if ((mask & 4) && type_of(o) == 7) return true;
    if ((mask & 8) && type_of(o) == 0xa) return true;
    if ((mask & 0x10) && type_of(o) == 0xb) return true;
    if ((mask & 0x20) && type_of(o) == 0xc) return true;
    if ((mask & 0x40) && type_of(o) == 9) return true;
    if ((mask & 0x80) == 0x80 && type_of(o) == 0xe) return true;
    if ((mask & 0x100) && type_of(o) == 0xd) return true;
    if ((mask & 0x200) && type_of(o) == 0x10) return true;
    return (mask & 0x7fff) == 0x7fff;
}
}  // namespace

extern "C" {

// =====================================================================================================
// 0052F2F0 CSWVirtualMachineCommands::ExecuteCommandChangeFaction(routine, argc)  RET 8
//   173 ChangeFaction(object oObjectToChangeFaction, object oMemberOfFactionToJoin)
//   412 ChangeToStandardFaction(object oCreatureToChange, int nStandardFaction)
// Quirk: for any other routine id (never bound to this handler) the original falls through to 0052F44C
// with the faction local uninitialised; here that local starts at 0.
int __fastcall recovered_vm_change_faction_0052f2f0(void* /*self*/, void*, int routine, unsigned /*argc*/) {
    U object = 0, member = 0, faction = 0;                                          // [esp+8] [esp+10] [esp+C]
    I standard = 0;                                                                 // [esp+14]
    if (!pop_object(&object)) return k_pop_failed;                                  // 0052F2F0..0F
    const U r = static_cast<U>(routine);
    if (r == k_routine_change_faction) {                                            // 0052F317..1E
        if (!pop_object(&member)) return k_pop_failed;                              // 0052F320..3E
    } else if (r == k_routine_change_standard_faction) {                            // 0052F341..47
        if (!pop_int(&standard)) return k_pop_failed;                               // 0052F349..67
    }
    void* srv = server();                                                           // 0052F36A..6F
    void* in = at<void*>(srv, 4);
    if (!in) return 0;                                                              // 0052F372..77
    void* manager = at<void*>(in, 0x10054);                                         // 0052F37D CFactionManager*
    if (!manager) return 0;                                                         // 0052F383..85
    if (r == k_routine_change_faction) {                                            // 0052F38B
        void* c = kotor_fastcall_004ae770(srv, nullptr, object);                    // 0052F393..98
        if (!c) return 0;
        if (at<U>(at<void*>(c, 0xa74), 0x6c) != 0) return 0;                        // 0052F3A5..B0 stats +6C (PC flag)
        if (!kotor_fastcall_004ae950(srv, nullptr, member, &faction)) return 0;     // 0052F3B6..D0
        void* m = kotor_fastcall_004ae770(srv, nullptr, member);                    // 0052F3D6..E4
        if (!m) return 0;
        if (at<U>(at<void*>(m, 0xa74), 0x6c) != 0) return 0;                        // 0052F3F1..05
    } else if (r == k_routine_change_standard_faction) {                            // 0052F408..0E
        if (!recovered_faction_is_npc_0052b280(manager, nullptr, static_cast<U>(standard))) return 0; // 0052F410..1E
        void* c = kotor_fastcall_004ae770(srv, nullptr, object);                    // 0052F420..2E
        if (c && at<U>(at<void*>(c, 0xa74), 0x6c) != 0) return 0;                   // 0052F433..42
        faction = static_cast<U>(standard);                                         // 0052F444..48
    }
    void* placeable = recovered_placeable_lookup_004ae7a0(srv, nullptr, object);    // 0052F44C..59
    if (placeable) { at<U>(placeable, 0x23c) = faction; return 0; }                 // 0052F462..73
    // 0052F476..8B: GetFaction(faction) -> AddMember(object, 0); a null faction is passed through as `this`
    w2x_faction_add_member_005bfa70(kotor_fastcall_0052b3b0(manager, nullptr, static_cast<I>(faction)), nullptr, object, 0);
    return 0;                                                                       // 0052F490..97
}

// =====================================================================================================
// 0054A260 CSWVirtualMachineCommands::ExecuteCommandGetObjectInShape(routine, argc)  RET 8
//   int nShape, float fSize, location lTarget, int bLineOfSight, int nObjectFilter, vector vOrigin
//   Shapes: 0 spell cylinder, 1 cone, 2 cube, 3 spell cone, anything else (4 sphere) -> sphere test.
// Quirks kept: bLineOfSight is popped and ignored; argc is not consulted (all six are always popped);
// for shapes 0 and 3 the popped origin is replaced by the caller's position; the found-object local
// ([esp+78]) and the candidate position ([esp+2C]) persist across iterations, so a failed lookup re-tests
// the previous object/position (the original leaves [esp+78] uninitialised for shapes 1/2/4: 0 here);
// GetNext starts at the area's saved index +19C plus one.
int __fastcall recovered_vm_get_object_in_shape_0054a260(void* self, void*, int routine, unsigned /*argc*/) {
    Vector target{0.0f, 0.0f, 0.0f};                                                // [esp+10] 0054A271..81
    Vector origin{0.0f, 0.0f, 0.0f};                                                // [esp+1C] 0054A289..99
    Vector pos{0.0f, 0.0f, 0.0f};                                                   // [esp+2C] 0054A2A1..B1
    Vector proj{0.0f, 0.0f, 0.0f};                                                  // [esp+68] 0054A2B9..C9
    float size = 0.0f;                                                              // [esp+28]
    void* location = nullptr;                                                       // [esp+60]
    I shape = 0, line_of_sight = 0, filter = 0;                                     // [esp+80] [esp+104] [esp+88]
    void* found = nullptr;                                                          // [esp+78]
    // 0054A2D1..D4 GetObjectArray (inline view of 004AED70; lookup() re-reads the same cell)
    if (!pop_int(&shape)                                                            // 0054A2E2..F7
        || !recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &size)             // 0054A2FD..0F
        || !recovered_vm_outer_pop_engine_structure_005d10a0(vm(), nullptr, k_engine_location, &location) // 0054A315..29
        || !pop_int(&line_of_sight)                                                 // 0054A32F..44
        || !pop_int(&filter)                                                        // 0054A34A..5F
        || !recovered_vm_outer_pop_vector_005d1040(vm(), nullptr, &origin)) {       // 0054A365..77
        free_location(location);                                                    // 0054AE01..16
        return k_pop_failed;                                                        // 0054AE1C
    }
    (void)line_of_sight;
    void* player = kotor_fastcall_004b2820(server(), nullptr);                      // 0054A37D..86
    void* area = module_get_area_004c30f0(server_get_module_004ae6b0(server(), nullptr), nullptr); // 0054A38B..9D
    if (!player || !area) {                                                         // 0054A3A2..AE
        free_location(location);                                                    // 0054ADD7..EF
        return push_object(k_invalid);                                              // 0054ADF7 -> 0054AB8A
    }
    std::memcpy(&target, location, 12);                                             // 0054A3B4..CA
    if (shape == 3 || shape == 0) {                                                 // 0054A3CE..DC
        if (lookup(at<U>(self, 8), &found)) origin = vector_at(found, 0x90);        // 0054A3DE..0F
    }
    free_location(location); location = nullptr;                                   // 0054A417..2B
    float lo, hi;                                                                   // [esp+44] [esp+74]
    if (shape == 1 || shape == 3) {                                                 // 0054A43A..42
        const float min_x = target.x < origin.x ? target.x : origin.x;              // 0054A456..6B
        lo = static_cast<float>(static_cast<double>(min_x) - size);                 // 0054A46F..73
        const float max_x = target.x > origin.x ? target.x : origin.x;              // 0054A477..8C
        hi = static_cast<float>(static_cast<double>(max_x) + size);                 // 0054A492..96
    } else {
        lo = static_cast<float>(static_cast<double>(target.x) - size);              // 0054A444..4C
        hi = static_cast<float>(static_cast<double>(target.x) + size);              // 0054A450 / 0054A492..96
    }
    if (shape == 0) {                                                               // 0054A490..9A
        Vector corner[4];                                                           // [esp+D4] x4
        for (Vector& c : corner) c = Vector{0.0f, 0.0f, 0.0f};                      // 0054A4A0..B1 vector ctor 00409E70 (zeroes)
        Vector d{static_cast<float>(static_cast<double>(target.x) - origin.x),      // 0054A4B6..E3 [esp+48]
                 static_cast<float>(static_cast<double>(target.y) - origin.y),
                 static_cast<float>(static_cast<double>(target.z) - origin.z)};
        Vector normal_out;                                                          // [esp+8C]
        const Vector* n = normalize_004ab180(&normal_out, &d);                      // 0054A4E7
        Vector axis = *n;                                                           // [esp+38] 0054A4EC..08
        const double len2 = (static_cast<double>(axis.y) * axis.y + static_cast<double>(axis.x) * axis.x)
                            + static_cast<double>(axis.z) * axis.z;                 // 0054A4F8..1D
        if (len2 < k_tiny_00741870 || len2 == k_tiny_00741870) axis = Vector{0.0f, 1.0f, 0.0f}; // 0054A51F..58
        Vector scaled_out, sum_out;                                                 // [esp+8C] [esp+98]
        const Vector* sc = vector_component_mult_00423bf0(&scaled_out, &axis, size);// 0054A55C..6E
        const Vector* sm = vector_sum_00423b90(&sum_out, &origin, sc);              // 0054A573..81
        target = *sm;                                                               // 0054A58E..AE target := origin + axis*size
        const double wx = -static_cast<double>(axis.y) * k_half_width_0073f414;     // 0054A58A..96 kept in ST
        const float wy = static_cast<float>(static_cast<double>(axis.x) * k_half_width_0073f414); // 0054A5A5..B5 [esp+58]
        const float wz = static_cast<float>(static_cast<double>(axis.z) * k_half_width_0073f414); // 0054A5B9..C3 [esp+5C]
        corner[0] = Vector{static_cast<float>(origin.x + wx),                       // 0054A5C7..D9
                           static_cast<float>(static_cast<double>(wy) + origin.y),  // 0054A5D5..F4
                           static_cast<float>(static_cast<double>(wz) + origin.z)}; // 0054A5E8..09
        corner[1] = Vector{static_cast<float>(origin.x - wx),                       // 0054A5FF..1C
                           static_cast<float>(static_cast<double>(origin.y) - wy),  // 0054A618..37
                           static_cast<float>(static_cast<double>(origin.z) - wz)}; // 0054A62B..4C
        corner[2] = Vector{static_cast<float>(target.x + wx),                       // 0054A642..5F
                           static_cast<float>(static_cast<double>(wy) + target.y),  // 0054A65B..7A
                           static_cast<float>(static_cast<double>(wz) + target.z)}; // 0054A66E..8F
        corner[3] = Vector{static_cast<float>(target.x - wx),                       // 0054A685..9E
                           static_cast<float>(static_cast<double>(target.y) - wy),  // 0054A6AB..CE
                           static_cast<float>(static_cast<double>(target.z) - wz)}; // 0054A6C2..EA
        lo = corner[0].x;                                                           // 0054A6A5..AF
        float max_x = corner[0].x;                                                  // 0054A6E2..E6 (ST and [esp+74])
        for (int k = 1; k < 4; ++k) {                                               // 0054A6F1..15
            if (corner[k].x < lo) lo = corner[k].x;                                 // fcomp / test ah,5 / jp
            if (max_x < corner[k].x) max_x = corner[k].x;                           // fcom  / test ah,5 / jp
        }
        hi = max_x;                                                                 // 0054A717
    }
    I index = 0;                                                                    // [esp+64]
    if (routine == k_routine_first_in_shape) {                                      // 0054A71B..26
        if (!recovered_area_index_00506f20(area, nullptr, reinterpret_cast<unsigned*>(&index), bits(lo))) // 0054A728..3B
            return push_object(k_invalid);                                          // 0054A73D -> 0054AB8A
    } else {
        index = at<I>(area, 0x19c) + 1;                                             // 0054A747..4E
        if (index == area_count(area)) return push_object(k_invalid);               // 0054A752..84
    }
    const float size2 = static_cast<float>(static_cast<double>(size) * size);       // 0054A78B..9B [esp+44]
    if (index >= area_count(area)) return push_object(k_invalid);                   // 0054A795..9F -> 0054ADAA
    const U mask = static_cast<U>(filter);
    do {                                                                            // 0054A7B0
        const bool ok = lookup(area_list_id(area, index), &found);                  // 0054A7B0..D6
        if (ok && found && type_of(found) > k_type_bound_00746633)                  // 0054A7D8..E5
            pos = vector_at(found, 0x90);                                           // 0054A7E7..FD
        if (pos.x > hi) return push_object(k_invalid);                              // 0054A801..0E -> 0054ADAA
        if (!shape_filter_matches(mask, found)) goto next;                          // 0054A814..93
        switch (static_cast<U>(shape)) {                                            // 0054A899..A9 table 0054AE2C
        case 0: {                                                                   // 0054A8B0 spell cylinder
            const double dx = static_cast<double>(pos.x) - origin.x;
            const double dy = static_cast<double>(pos.y) - origin.y;
            const double dz = static_cast<double>(pos.z) - origin.z;
            const float dyf = static_cast<float>(dy), dzf = static_cast<float>(dz); // fst [esp+84] / [esp+7C]
            const double d2 = (dz * dz + dy * dy) + dx * dx;                        // 0054A8D5..E3
            if (!(d2 < size2 || d2 == size2)) goto next;                            // 0054A8E5..F4 jp
            Vector d{static_cast<float>(dx), dyf, dzf};                             // 0054A8FA..31 [esp+48]
            Vector e{static_cast<float>(static_cast<double>(target.x) - origin.x),  // 0054A905..44 [esp+B0]
                     static_cast<float>(static_cast<double>(target.y) - origin.y),
                     static_cast<float>(static_cast<double>(target.z) - origin.z)};
            Vector nd_out, ne_out;                                                  // [esp+150] [esp+174]
            const Vector* nd = normalize_004ab180(&nd_out, &d);                     // 0054A94B
            const Vector* ne = normalize_004ab180(&ne_out, &e);                     // 0054A964
            if (!(recovered_dot_product_00423c20(ne, nd) >= k_zero_0073d700)) goto next; // 0054A96D..80 (C0: less/unordered)
            Vector line_out;                                                        // [esp+120]
            proj = *w5_line_project_004a98b0(&line_out, &origin, &target, &pos);    // 0054A986..C6
            const double rx = static_cast<double>(pos.x) - proj.x;
            const double ry = static_cast<double>(pos.y) - proj.y;
            const double rz = static_cast<double>(pos.z) - proj.z;
            const double r2 = (rz * rz + ry * ry) + rx * rx;                        // 0054A9D4..E5
            if (r2 < k_radius2_00747d7c || r2 == k_radius2_00747d7c) goto match;    // 0054A9E7 -> 0054AD80..8B jnp
            goto next;
        }
        case 1: {                                                                   // 0054A9F2 cone
            Vector line_out;                                                        // [esp+108]
            proj = *w5_line_project_004a98b0(&line_out, &origin, &target, &pos);    // 0054A9F2..AA
            const Vector r{static_cast<float>(static_cast<double>(pos.x) - proj.x), // 0054AA0E..26 [esp+54]
                           static_cast<float>(static_cast<double>(pos.y) - proj.y), // 0054AA2A..48
                           static_cast<float>(static_cast<double>(pos.z) - proj.z)};// 0054AA4C..58
            Vector a{static_cast<float>(static_cast<double>(pos.x) - origin.x),     // 0054AA5C..82 [esp+BC]
                     static_cast<float>(static_cast<double>(pos.y) - origin.y),
                     static_cast<float>(static_cast<double>(pos.z) - origin.z)};
            Vector b{static_cast<float>(static_cast<double>(target.x) - origin.x),  // 0054AA89..AF [esp+A4]
                     static_cast<float>(static_cast<double>(target.y) - origin.y),
                     static_cast<float>(static_cast<double>(target.z) - origin.z)};
            Vector na_out, nb_out;                                                  // [esp+138] [esp+168]
            const Vector* na = normalize_004ab180(&na_out, &a);                     // 0054AAB6
            const Vector* nb = normalize_004ab180(&nb_out, &b);                     // 0054AACF
            if (!(recovered_dot_product_00423c20(nb, na) > k_zero_0073d700)) goto next; // 0054AAD8..EB test ah,41h
            Vector axis{static_cast<float>(static_cast<double>(target.x) - origin.x),   // 0054AAF1..1F [esp+C8]
                        static_cast<float>(static_cast<double>(target.y) - origin.y),
                        static_cast<float>(static_cast<double>(target.z) - origin.z)};
            const float ratio = static_cast<float>(static_cast<double>(size) / vector_magnitude_004ab100(&axis)); // 0054AB26..34 fdivr
            const double m = vector_magnitude_004ab100(&proj) * static_cast<double>(ratio); // 0054AB3B..40 |proj| (absolute) quirk
            const double r2 = (static_cast<double>(r.z) * r.z + static_cast<double>(r.y) * r.y)
                              + static_cast<double>(r.x) * r.x;                     // 0054AB47..64
            if (!(m * m >= r2)) goto next;                                          // 0054AB66..73 fcompp, C0 (less/unordered)
            goto match;
        }
        case 2:                                                                     // 0054ABB1 cube
            if (!(static_cast<double>(target.x) + size >= pos.x)) goto next;        // 0054ABB1..C2 C0
            if (!(static_cast<double>(target.x) - size <= pos.x)) goto next;        // 0054ABC8..D9 jp
            if (!(static_cast<double>(target.y) + size >= pos.y)) goto next;        // 0054ABDF..F0
            if (!(static_cast<double>(target.y) - size <= pos.y)) goto next;        // 0054ABF6..07
            if (!(static_cast<double>(target.z) + size >= pos.z)) goto next;        // 0054AC0D..1E
            if (static_cast<double>(target.z) - size <= pos.z) goto match;          // 0054AC24..35 jnp
            goto next;
        case 3: {                                                                   // 0054AC40 spell cone
            Vector line_out, rel_out;                                               // [esp+114] [esp+12C]
            const Vector* pr = w5_line_project_004a98b0(&line_out, &origin, &target, &pos); // 0054AC40..5C
            const Vector s = *kotor_cdecl_00423bc0(&rel_out, pr, &origin);          // 0054AC64..7F proj - origin [esp+38]
            Vector a{static_cast<float>(static_cast<double>(pos.x) - origin.x),     // 0054AC72..BC [esp+98]
                     static_cast<float>(static_cast<double>(pos.y) - origin.y),
                     static_cast<float>(static_cast<double>(pos.z) - origin.z)};
            Vector b{static_cast<float>(static_cast<double>(target.x) - origin.x),  // 0054ACC3..E9 [esp+8C]
                     static_cast<float>(static_cast<double>(target.y) - origin.y),
                     static_cast<float>(static_cast<double>(target.z) - origin.z)};
            Vector na_out, nb_out;                                                  // [esp+144] [esp+15C]
            const Vector* na = normalize_004ab180(&na_out, &a);                     // 0054ACF0
            const Vector* nb = normalize_004ab180(&nb_out, &b);                     // 0054AD09
            if (!(recovered_dot_product_00423c20(nb, na) >= k_cos_00748744)) goto next; // 0054AD12..25 C0
            const double s2 = (static_cast<double>(s.x) * s.x + static_cast<double>(s.z) * s.z)
                              + static_cast<double>(s.y) * s.y;                     // 0054AD27..41
            if (s2 < size2 || s2 == size2) goto match;                              // 0054AD43..4C jnp
            goto next;
        }
        default: {                                                                  // 0054AD54 sphere
            const double dx = static_cast<double>(pos.x) - target.x;
            const double dy = static_cast<double>(pos.y) - target.y;
            const double dz = static_cast<double>(pos.z) - target.z;
            const double d2 = (dz * dz + dy * dy) + dx * dx;                        // 0054AD6C..7A
            if (d2 < size2 || d2 == size2) goto match;                              // 0054AD7C..8B jnp
            goto next;
        }
        }
    match:                                                                          // 0054AB79
        at<I>(area, 0x19c) = index;                                                 // 0054AB79..7D
        return push_object(at<U>(found, 4));                                        // 0054AB83..AE
    next:                                                                           // 0054AD95
        ++index;
    } while (index < area_count(area));                                             // 0054AD9E..A4
    return push_object(k_invalid);                                                  // 0054ADAA..D4
}

// =====================================================================================================
// 00549360 CSWVirtualMachineCommands::ExecuteCommandGetInPersistentObject(routine, argc)  RET 8
//   object oPersistentObject, int nResidentObjectType, int nPersistentZone (popped only while argc allows)
// Quirks kept: the argc slot doubles as the iteration index, so an encounter GetNext with a nonzero zone
// starts at argc; an object lookup that fails inside a scan repeats the same index forever (as the
// original); the encounter scan stops after the first type match; a trigger/AOE GetNext resets nothing on
// an exhausted list; the type mask must equal one OBJECT_TYPE_* value (or 7FFFh); the dead/destroyable
// checks call GetDeadTemp on AsSWSCreature() even for non-creatures (as the original).  The defaults of
// unpopped arguments and the encounter x-range for zone != 0 are uninitialised in the original (0 here;
// compiled scripts always pass all three arguments).
int __fastcall recovered_vm_get_in_persistent_object_00549360(void* /*self*/, void*, int routine, unsigned argc) {
    U oid = k_invalid;                                                              // [esp+30] 0054936C..72
    U result = k_invalid;                                                           // ebp
    U saved = k_invalid;                                                            // [esp+24] 00549376
    I type_mask = 0, zone = 0;                                                      // [esp+2C] [esp+20]
    const I n = static_cast<I>(argc);
    if (n > 0 && !pop_object(&oid)) return k_pop_failed;                            // 0054936A..9C
    if (n > 1 && !pop_int(&type_mask)) return k_pop_failed;                         // 0054939F..B6
    if (n > 2 && !pop_int(&zone)) return k_pop_failed;                              // 005493B8..CF
    void* obj = nullptr;                                                            // [esp+10]
    if (!lookup(oid, &obj)) return push_object(result);                            // 005493D1..F5
    I index = n;                                                                    // [esp+48] (argc slot)
    float lo = 0.0f, hi = 0.0f;                                                     // [esp+18] [esp+14]
    void* candidate = nullptr;                                                      // [esp+44] / [esp+1C]
    if (vcall<void*>(obj, 0x5c)) {                                                  // 005493FB..0C encounter
        void* encounter = vcall<void*>(obj, 0x5c);                                  // 00549412..1B
        void* area = recovered_object_area_004cb120(vcall<void*>(obj, 0x5c), nullptr); // 0054941D..27
        if (!area) return push_object(result);                                      // 00549429..2B
        if (zone == 0) {                                                            // 00549431..37
            lo = at<float>(encounter, 0x2a0);                                       // 00549439..45
            hi = at<float>(encounter, 0x2a4);                                       // 0054943F..49
        }
        if (routine == k_routine_first_in_persistent) {                             // 0054944D..55
            if (!recovered_area_index_00506f20(area, nullptr, reinterpret_cast<unsigned*>(&index), bits(lo))) // 00549457..6A
                return push_object(k_invalid);                                      // 0054946C..73
        } else {
            if (zone == 0) index = static_cast<I>(at<U16>(encounter, 0x228)) + 1;  // 00549478..84
            if (index == area_count(area)) return push_object(k_invalid);          // 00549488..92 -> 005496C1
        }
        bool done = false;                                                          // bl 005494A2
        Vector pos{0.0f, 0.0f, 0.0f};                                               // [esp+34] 005494A6..B6
        if (index >= area_count(area)) return push_object(result);                  // 005494A4..BE
        do {
            if (done) break;                                                        // 005494C4..C6
            if (!lookup(area_list_id(area, index), &candidate) || !candidate) {     // 005494CC..02
                result = k_invalid;                                                 // 005495EC (index unchanged)
                continue;
            }
            if (!persistent_type_matches(type_mask, type_of(candidate))) { ++index; continue; } // 00549508..88 -> 0054951A
            pos = vector_at(candidate, 0x90);                                       // 0054958A..AC
            done = true;                                                            // 005495A8
            if (pos.x > hi) return push_object(k_invalid);                          // 00549596..B3 -> 00549B3F
            if (zone != 0) continue;                                                // 005495B9..BF
            if (!recovered_encounter_in_activate_area_00590090(encounter, nullptr, &pos)) { ++index; continue; } // 005495C1..CF
            result = area_list_id(area, index);                                     // 005495D5..DF
            at<U16>(encounter, 0x228) = static_cast<U16>(index + 1);                // 005495E2..E3
        } while (index < area_count(area));                                         // 005495F1..FB
        return push_object(result);                                                 // 00549601..22
    }
    if (vcall<void*>(obj, 0x50)) {                                                  // 00549625..2A area of effect
        if (zone != 0) return push_object(k_invalid);                               // 00549630..36 -> 005496C1
        void* aoe = vcall<void*>(obj, 0x50);                                        // 0054963C..4D [esp+28]
        void* area = recovered_object_area_004cb120(vcall<void*>(obj, 0x50), nullptr); // 00549645..56
        if (!area) return push_object(result);                                      // 0054965B..5F
        Vector pos_out{0.0f, 0.0f, 0.0f};                                           // [esp+34]
        const U radius = at<U>(aoe, 0x238);                                         // 00549661..6B
        const float* p1 = static_cast<const float*>(recovered_aoe_position_005946e0(aoe, nullptr, &pos_out)); // 0054966F..72
        lo = static_cast<float>(static_cast<double>(*p1) - flt(radius));            // 00549677..8A
        hi = flt(radius);                                                           // 0054967D / 0054968E
        const float* p2 = static_cast<const float*>(recovered_aoe_position_005946e0(aoe, nullptr, &pos_out)); // 00549683..92
        hi = static_cast<float>(static_cast<double>(hi) + *p2);                     // 00549697..A6
        if (routine == k_routine_first_in_persistent) {                             // 0054969D..AA
            if (!recovered_area_index_00506f20(area, nullptr, reinterpret_cast<unsigned*>(&index), bits(lo))) // 005496AC..BF
                return push_object(k_invalid);                                      // 005496C1..C8
        } else {
            index = static_cast<I>(at<U16>(aoe, 0x228));                            // 005496CD..D4
            if (index == area_count(area)) return push_object(k_invalid);          // 005496D8..DE -> 00549B3F
        }
        bool done = false;                                                          // byte [esp+44] 005496EE
        if (index >= area_count(area)) return push_object(result);                  // 005496E4..F3
        do {
            if (done) break;                                                        // 00549700..06
            if (!lookup(area_list_id(area, index), &candidate) || !candidate) continue; // 0054970C..42 -> 005498B0
            if (!persistent_type_matches(type_mask, type_of(candidate))) { ++index; continue; } // 00549748..F9 -> 005498AC
            const Vector pos = vector_at(candidate, 0x90);                          // 005497FF..0D (EBP takes pos.z)
            if (pos.x > hi) {                                                       // 00549811..1E
                at<U16>(aoe, 0x228) = 0;                                            // 005498C5..C9
                return push_object(k_invalid);                                      // 005498D2..D8
            }
            if (vcall<void*>(candidate, 0x30))                                      // 00549824..2B
                (void)recovered_creature_incapacitated_004ef890(vcall<void*>(candidate, 0x30), nullptr); // 0054982D..38 result unused
            if (!recovered_aoe_in_area_of_effect_00595450(aoe, nullptr, pos.x, pos.y, pos.z)) { // 0054983D..55
                result = saved; ++index; continue;                                  // 005498A8..AC
            }
            if (vcall<U>(vcall<void*>(candidate, 0x10), 0x94)) { result = saved; ++index; continue; } // 00549857..6C
            if (recovered_creature_incapacitated_004ef890(vcall<void*>(candidate, 0x30), nullptr)) { // 0054986E..80
                result = saved; ++index; continue;
            }
            saved = area_list_id(area, index);                                      // 00549882..94
            at<U16>(aoe, 0x228) = static_cast<U16>(index + 1);                      // 00549893..98
            done = true;                                                            // 0054989F
            result = saved;                                                         // 005498A4 (index unchanged)
        } while (index < area_count(area));                                         // 005498B0..BA
        return push_object(result);                                                 // 005498C0 -> 00549601
    }
    if (!vcall<void*>(obj, 0x40)) return push_object(result);                       // 005498DD..E8 trigger
    if (zone != 0) return push_object(k_invalid);                                   // 005498EE..F4 -> 0054998B
    void* trigger = vcall<void*>(obj, 0x40);                                        // 005498FA..03
    void* area = recovered_object_area_004cb120(trigger, nullptr);                  // 00549905..07
    if (!area) return push_object(result);                                          // 0054990C..10
    const float* vertices = at<const float*>(trigger, 0x288);                       // 00549916 (Vector array)
    lo = vertices[0]; float max_x = vertices[0];                                    // 0054991C..2B
    const I vertex_count = at<I>(trigger, 0x284);
    if (vertex_count > 1) {                                                         // 00549924..2F
        for (I k = 1; k < vertex_count; ++k) {                                      // 00549931..6A (count-1 steps)
            const float x = vertices[k * 3];
            if (x < lo) lo = x;                                                     // 00549940..4F test ah,5 / jp
            if (x > max_x) max_x = x;                                               // 00549953..62 test ah,41h / jne
        }
    }
    hi = max_x;
    if (routine == k_routine_first_in_persistent) {                                 // 0054996C..74
        if (!recovered_area_index_00506f20(area, nullptr, reinterpret_cast<unsigned*>(&index), bits(lo))) // 00549976..89
            return push_object(k_invalid);                                          // 0054998B..91
    } else {
        index = static_cast<I>(at<U16>(trigger, 0x30a));                            // 00549996..9D
        if (index == area_count(area)) return push_object(k_invalid);               // 005499A1..A7 -> 005496C1
    }
    bool done = false;                                                              // bl 005499B7
    if (index >= area_count(area)) return push_object(result);                      // 005499AD..D3
    do {
        if (done) break;                                                            // 005499E0..E2
        if (!lookup(area_list_id(area, index), &candidate) || !candidate) continue; // 005499E8..1A -> 00549A32
        if (!persistent_type_matches(type_mask, type_of(candidate))) { ++index; continue; } // 00549A1C..A8 -> 00549A2E
        Vector pos = vector_at(candidate, 0x90);                                    // 00549AAA..C8 [esp+34]
        if (pos.x > hi) {                                                           // 00549AB6..D1
            at<U16>(trigger, 0x30a) = 0;                                            // 00549B36
            return push_object(k_invalid);                                          // 00549B3F..46
        }
        if (!w5_trigger_in_trigger_0058ce40(trigger, nullptr, &pos)) { ++index; continue; } // 00549AD3..E1
        if (vcall<U>(vcall<void*>(candidate, 0x10), 0x94)) { ++index; continue; }   // 00549AE7..FC
        if (recovered_creature_incapacitated_004ef890(vcall<void*>(candidate, 0x30), nullptr)) { ++index; continue; } // 00549B02..14
        result = area_list_id(area, index);                                         // 00549B1A..24
        at<U16>(trigger, 0x30a) = static_cast<U16>(index + 1);                      // 00549B27..28
        done = true;                                                                // 00549B2F
    } while (index < area_count(area));                                             // 00549A32..3C
    return push_object(result);                                                     // 00549A3E -> 00549601
}

// =====================================================================================================
// Shared helpers (missing callees reconstructed here; coordinator may dedupe)
// 004AE950 CServerExoApp::GetFactionOfObject(id, out): mov ecx,[ecx+4]; jmp 004B40C0. RET 8 (via target)
int __fastcall kotor_fastcall_004ae950(void* server, void*, U id, U* out) {
    return recovered_server_faction_of_object_004b40c0(at<void*>(server, 4), nullptr, id, out);
}
// 0052B3B0 CFactionManager::GetFaction(int): signed range check against the list at +24 ({+0 data, +4 count}). RET 4
void* __fastcall kotor_fastcall_0052b3b0(void* manager, void*, I faction) {
    if (faction < 0) return nullptr;                                                // 0052B3B4..B6 jl
    void* list = at<void*>(manager, 0x24);                                          // 0052B3B8
    if (faction >= at<I>(list, 4)) return nullptr;                                  // 0052B3BB..BE jge
    return at<void**>(list, 0)[faction];                                            // 0052B3C0..C5
}

}  // extern "C"
