// Lane worker "s2p-small" (fork d): client server-to-player SafeProjectile messages (major 21h of the client
// dispatcher CSWCMessage::HandleServerToPlayerMessage 0066A640). Transcribed from the original swkotor.exe (GOG K1,
// read-only static disassembly); statement comments cite original instruction addresses. B+x = the original frame
// slot at ESP just after the prologue register pushes.
//   006631C0 CSWCMessage::HandleServerToPlayerSafeProjectile(BYTE minor)        thiscall RET 4  -> @12 export
//   00650B90 CSWCMessage::HandleServerToPlayerSafeProjectileSpell()             thiscall RET    -> @8
//   006501B0 CSWCMessage::HandleServerToPlayerSafeProjectileProjectile()        thiscall RET    -> @8
//   00696250 CSWCSpellCastVisual::LoadCastVisual(CResRef, CResRef, CResRef, CSWCObject*) RET 34h -> @60
//   004D7120 CSWMessage::ReadVector() (hidden Vector* return)                   thiscall RET 4  -> @12
//   006D42A0 CSWCProjectile::SetSpawnCombatVisualOnImpact(BYTE, int, WORD)      thiscall RET 0Ch -> @20
//   006D4440 CSWCProjectile::CreateMuzzleFlash(CExoString&, CAurObject*, CExoString&) RET 0Ch -> @20
// Original quirks kept: a failed dummy-creature registration (00650EFB) returns 0 and leaks the creature; a failed
// projectile allocation is used unchecked (SetProjectileSpellId on null, 00651271 / 0065139D / 006501B0 path);
// the Projectile handler reuses its result slot B+1C for the model index and returns it as 0 on every early exit.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

extern "C" {
// ---- message readers (w2_message_native.obj)
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);                   // MessageReadOverflow RET 4
U __fastcall kotor_fastcall_00692630(void* msg, void*);                           // CSWCMessage::ReadOBJECTIDClient RET
float __fastcall kotor_fastcall_004d67b0(void* msg, void*, U scale_bits, U bits);  // ReadFLOAT(float, int) RET 8 (ST0)
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);                   // ReadDWORD RET 4
U8 __fastcall kotor_fastcall_004d6720(void* msg, void*, U bits);                  // ReadBYTE RET 4
// ---- client app / objects
void* __fastcall kotor_fastcall_005ed580(void* app, void*, U id);                 // CClientExoApp::GetGameObject RET 4 (0701)
void* __fastcall client_get_player_creature_005ed540(void* app, void*);           // GetPlayerCreature (0580)
void* __fastcall kotor_fastcall_005ed570(void* app, void*);                       // GetGameObjectArray (w4-creature_ai)
void* __fastcall kotor_m15e_client_get_client_ai_master_005ed6b0(void* app, void*);   // GetClientAIMaster (0831)
void* __fastcall frame_R4_005ed5b0(void* app, void*, U id);                       // GetCreatureByGameObjectID RET 4 (0457)
U8 __fastcall area_vfx_register_existing_004d80e0(void* array, void*, U* id, void* object, U flag);   // AddInternalObject RET 0Ch
U __fastcall recovered_client_ai_master_add_object_00604a20(void* master, void*, void* object) noexcept(false);   // AddObject
void* __fastcall kotor_m15a_creature_construct_00616a20(void* self, void*) noexcept(false);   // CSWCCreature() (0816)
void __fastcall recovered_client_object_set_desired_fade_state_0063ce60(void* object, void*, U a, U b, U time,
                                                                      U fade);           // SetDesiredFadeState RET 10h
U __fastcall kotor_m15a_creature_get_equipped_item_id_0060ca80(void* creature, void*, U slot);   // RET 4 (0816)
U __fastcall kotor_animation_sound_damage_effect_00616890(void* creature, void*);  // CSWCCreature::GetForceShield RET
// ---- rules / 2DA / sound
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, U id);   // CSWSpellArray::GetSpell RET 4
int __fastcall c2da_current_get_string_00413270(void* table, void*, int row, void* column, void* out);   // RET 0Ch
int __fastcall c2da_current_get_int_00413660(void* table, void*, int row, void* column, int* out);       // RET 0Ch
int __fastcall c2da_current_get_float_00413430(void* table, void*, int row, void* column, float* out);   // RET 0Ch
void __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, const void* resref, U x, U y, U z,
                                                         U a5, U a6, U a7, U a8, U a9, U a10);   // RET 28h (0812)
extern void* kotor_cell_sound_007a39ec;                                            // [007A39EC] CExoSound*
extern void* kotor_cell_rules_007a3a28;                                            // [007A3A28] CSWRules*
// ---- strings / resrefs / math
void __cdecl frame_LogFormat_005e5680(void* string, const char* format, ...) noexcept(false);   // CExoString::Format
void* __fastcall frame_LogAssign_005e5140(void* s, void*, const char* text);       // CExoString::operator=(char*)
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, const void* string);   // CResRef(CExoString&)
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);              // 00641DB0 return this
void* __cdecl normalize_004ab180(void* out, const void* v);                        // normalize(Vector) (0529)
// ---- projectile (t9-object_projectile_foundation-observed.obj)
void* __fastcall object_projectile_006d57b0(void* self, void*);                    // CSWCProjectile() RET
void __fastcall object_projectile_006d41b0(void* self, void*, U spell_id);         // SetProjectileSpellId RET 4
void __fastcall object_projectile_006d4250(void* self, void*, U animation);        // SetAnimation RET 4
void __fastcall object_projectile_006d42d0(void* self, void*, U target, void* node);   // SetDesiredTargetObject RET 8
void __fastcall object_projectile_006d2c30(void* self, void*, U timer);            // SetDesiredPositionTimer RET 4
void __fastcall object_projectile_006d6ee0(void* self, void*, U type);             // SetProjectileType RET 4
void __fastcall kotor_fastcall_0064b4f0(void* self, void*, const void* position);  // 0064B4F0 SetSpellTargetPosition RET 4
// ---- spell cast visual
void __fastcall kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(void* visual, void*);   // UnloadCastVisual (0831)
void __fastcall gui_load_resref_copy_to_string_00405fb0(const void* resref, void*, char* out);   // CResRef::CopyToString(char*) RET 4
void* __cdecl aurora_new_aur_object_00449cc0(const char* name, const char* gob, void* mdl, void* mdx);   // NewCAurObject
unsigned __fastcall frame_LogIsEmpty_005e58a0(void* s, void*);                     // CExoString::IsEmpty (0457)
const char* __fastcall frame_LogCStr_005e5670(void* s, void*);                     // CExoString::CStr (0459)
}

namespace {
struct ResRefValue { unsigned char bytes[16]; };   // CResRef passed by value
static_assert(sizeof(ResRefValue) == 16, "CResRef");
struct Vec3 { float x, y, z; };
static_assert(sizeof(Vec3) == 12, "Vector");
}  // namespace

extern "C" {
// this unit
U __fastcall kotor_fastcall_006631c0(void* self, void*, unsigned minor) noexcept(false);
U __fastcall kotor_fastcall_00650b90(void* self, void*) noexcept(false);
U __fastcall kotor_fastcall_006501b0(void* self, void*) noexcept(false);
U __fastcall kotor_fastcall_00696250(void* self, void*, ResRefValue a, ResRefValue b, ResRefValue c,
                                     void* owner) noexcept(false);
void* __fastcall kotor_fastcall_004d7120(void* msg, void*, void* out) noexcept;
void __fastcall kotor_fastcall_006d42a0(void* self, void*, U spawn, U flag, U row) noexcept;
void __fastcall kotor_fastcall_006d4440(void* self, void*, void* model, void* parent, void* node) noexcept(false);
}

namespace {
constexpr U kInvalid_00746644 = 0x7f000000u;          // .rdata 00746644 (OBJECT_INVALID)
constexpr U8 kArrayOk_0074666c = 0;                   // .rdata 0074666C (CGameObjectArray status)
constexpr float kEpsilon_00747504 = 9.99999975e-06f;  // .rdata 00747504 (3727C5ACh)
constexpr float kOne_0073d764 = 1.0f;                 // .rdata 0073D764
constexpr float kZero_0073d700 = 0.0f;                // .rdata 0073D700
constexpr U kOneBits = 0x3f800000u;                   // immediate 1.0f
constexpr U kOneAndHalfBits = 0x3fc00000u;            // immediate 1.5f
const char kDummy_00751cd8[] = "c_dummy";
const char kHandConjure_00751d74[] = "handconjure";
const char kHeadConjure_0074f850[] = "headconjure";
const char kRHand_0074f334[] = "rhand";
const char kRoot_00740c00[] = "root";
const char kEmpty_0073d71c[] = "";
const char kLBullet_00751d6c[] = "lbullet";
const char kRBullet_00751d64[] = "rbullet";
const char kBulletHook_0074f590[] = "bullethook";
const char kImpact_00743318[] = "impact";
const char kModelFmt_00751d5c[] = "Model%d";
const char kDefault_0073e304[] = "DEFAULT";
const char kImpactSoundFmt_00751d4c[] = "ImpactSound%d";
const char kImpactBolt_00751d40[] = "impact_bolt";
const char kAppearanceFmt_00751d30[] = "Appearance_0%d";
const char kRadiusFmt_00751d24[] = "Radius_0%d";
const char kMuzzleFlash_00751d18[] = "MuzzleFlash";
const char kShotSoundFmt_00751d0c[] = "ShotSound%d";
const char kCastVisual_007549e4[] = "castvisual";
const char kCastGroundVisual_007549c8[] = "castgroundvisual";
const char kCast01_007549dc[] = "cast01";
const char kFade_0074fc58[] = "fade";
const char kMuzzleFlashGob_00757cb8[] = "muzzleflash";

inline void* rules() { return kotor_cell_rules_007a3a28; }
inline U fb(float f) { return fbits(f); }
inline void* game_new(U bytes) { return live_operator_new(bytes); }
// operator new + constructor under the original EH state whose unwind action frees the block (0072AA00 / 0072A9B3)
template <class Ctor> void* construct_or_free(U bytes, Ctor ctor) {
    void* mem = game_new(bytes);
    if (mem == nullptr) return nullptr;
    try {
        return ctor(mem);
    } catch (...) {
        live_free(mem);
        throw;
    }
}
inline void copy3(void* dst, const void* src) { std::memcpy(dst, src, 12); }
}  // namespace

// ================================================================================================================
// 006631C0 HandleServerToPlayerSafeProjectile(BYTE minor) RET 4
extern "C" U __fastcall kotor_fastcall_006631c0(void* self, void*, unsigned minor) noexcept(false) {
    const U8 m = static_cast<U8>(minor);                                   // 006631C0 mov dl,[esp+4]
    if (m == 1) return kotor_fastcall_00650b90(self, nullptr);             // 006631C6..006631D0
    if (m == 2) return kotor_fastcall_006501b0(self, nullptr);             // 006631D3..006631D8 (tail value in EAX)
    return 0;                                                              // 006631C4 xor eax,eax / 006631DD
}

// ================================================================================================================
// 004D7120 CSWMessage::ReadVector() -> Vector (hidden return pointer) RET 4. Reads three raw floats from the read
// buffer (+28h data, +2Ch size, +30h position); an element past the end reads 0.0 but the position still advances.
extern "C" void* __fastcall kotor_fastcall_004d7120(void* msg, void*, void* out) noexcept {
    float v[3];
    U pos = rd(msg, 0x30);                                                 // 004D7120 eax=[ecx+30]
    const U size = rd(msg, 0x2c);                                          // 004D7127 esi=[ecx+2C]
    for (int i = 0; i < 3; ++i) {                                          // 004D712A..004D7180 (unrolled x3)
        const U next = pos + 4;
        if (next > size) v[i] = kZero_0073d700;                            // jbe not taken -> fld [0073D700]
        else std::memcpy(&v[i], fa(rp(msg, 0x28), pos), 4);               // fld [eax+edi]
        wr(msg, 0x30, next);                                               // mov [ecx+30],next
        pos = next;
    }
    std::memcpy(out, v, 12);                                               // 004D7183..004D7197
    return out;                                                            // 004D7187 eax=out
}

// ================================================================================================================
// 006D42A0 CSWCProjectile::SetSpawnCombatVisualOnImpact(BYTE spawn, int flag, WORD row) RET 0Ch
extern "C" void __fastcall kotor_fastcall_006d42a0(void* self, void*, U spawn, U flag, U row) noexcept {
    wr8(self, 0xfc, static_cast<U8>(spawn));                               // 006D42A8
    wr(self, 0x110, flag);                                                 // 006D42B3
    wr16(self, 0x10c, static_cast<U16>(row));                              // 006D42B9
}

// ================================================================================================================
// 006D4440 CSWCProjectile::CreateMuzzleFlash(CExoString& model, CAurObject* parent, CExoString& node) RET 0Ch
extern "C" void __fastcall kotor_fastcall_006d4440(void* self, void*, void* model, void* parent,
                                                   void* node) noexcept(false) {
    const U empty = frame_LogIsEmpty_005e58a0(model, nullptr);             // 006D4448..006D444A
    if (empty != 0) return;                                                // 006D4451
    if (parent == nullptr) return;                                         // 006D4454..006D445A
    void* flash = aurora_new_aur_object_00449cc0(frame_LogCStr_005e5670(model, nullptr), kMuzzleFlashGob_00757cb8,
                                                 nullptr, nullptr);        // 006D445C..006D446B (eax=0 pushed twice)
    wp(self, 0x1ac, flash);                                                // 006D4477
    const char* node_name = frame_LogCStr_005e5670(node, nullptr);         // 006D447D..006D4481 (ebx=vtbl first)
    vcall<void>(rp(self, 0x1ac), 0x50, parent, node_name, 3u);             // 006D4486..006D448E +50 attach (parent,node,3)
    vcall<void>(rp(self, 0x1ac), 0x18, kImpact_00743318, 1.0f, 1u, 0u);    // 006D4491..006D44A7 +18 ("impact",1.0,1,0)
}

// ================================================================================================================
// 00696250 CSWCSpellCastVisual::LoadCastVisual(CResRef hand, CResRef head, CResRef ground, CSWCObject* owner) RET 34h
// Visual layout: +00/+04/+08 previously loaded visuals (faded here), +0C hand, +10 head, +14 ground visual.
extern "C" U __fastcall kotor_fastcall_00696250(void* self, void*, ResRefValue a, ResRefValue b, ResRefValue c,
                                                void* owner) noexcept(false) {
    char name[0x14];                                                       // [esp+08] (0x14 bytes below the arguments)
    kotor_m15e_spell_cast_visual_unload_cast_visual_00695d90(self, nullptr);   // 00696254..00696256
    struct Part { const ResRefValue* ref; U slot; const char* gob; const char* node; };
    const Part parts[3] = {
        {&a, 0x0c, kCastVisual_007549e4, kHeadConjure_0074f850},           // 0069625B..006962D6
        {&b, 0x10, kCastVisual_007549e4, kHandConjure_00751d74},           // 006962D9..00696350
        {&c, 0x14, kCastGroundVisual_007549c8, kRoot_00740c00},            // 00696353..006963C8
    };
    for (const Part& p : parts) {
        gui_load_resref_copy_to_string_00405fb0(p.ref, nullptr, name);     // CResRef::CopyToString(char*)
        U length = 0;                                                      // inline strlen
        while (name[length] != 0) ++length;
        if (length == 0) continue;                                         // je
        void* visual = aurora_new_aur_object_00449cc0(name, p.gob, nullptr, nullptr);   // NewCAurObject(name, gob, 0, 0)
        wp(self, p.slot, visual);                                          // mov [esi+slot],eax
        if (visual == nullptr) continue;
        void* gob = vcall<void*>(owner, 0x98, 0xffu, 1u);                  // owner +98 (FFh,1) -> gob
        vcall<void>(rp(self, p.slot), 0x50, gob, p.node, 0u);              // visual +50 (gob, node, 0)
        vcall<void>(rp(self, p.slot), 0x18, kCast01_007549dc, 1.0f, 1u, 0u);   // visual +18 ("cast01",1.0,1,0)
    }
    static const U fade_slots[3] = {4, 0, 8};                              // 006963CB / 006963E7 / 00696400
    for (U slot : fade_slots) {
        void* old = rp(self, slot);
        if (old != nullptr) vcall<void>(old, 0x18, kFade_0074fc58, 1.0f, 1u, 0u);   // +18 ("fade",1.0,1,0)
    }
    return 1;                                                              // 0069641B
}

// ================================================================================================================
// 00650B90 HandleServerToPlayerSafeProjectileSpell() RET
extern "C" U __fastcall kotor_fastcall_00650b90(void* self, void*) noexcept(false) {
    void* const msg = self;                                                // esi
    float p0[3] = {0, 0, 0};                                               // B+3C..44 (00650BBB..00650BCB)
    float p1[3] = {0, 0, 0};                                               // B+1C..24
    float dir[3] = {0, 0, 0};                                              // B+58..60
    U target_id;                                                           // B+54
    U spell_id = 0;                                                        // B+4C (uninitialised in the original)
    U projectile_type = 0;                                                 // B+28 (uninitialised for byte 6)
    if (kotor_fastcall_004d6230(msg, nullptr, 1) != 0) return 0;          // 00650C07..00650C0E
    const U caster_id = kotor_fastcall_00692630(msg, nullptr);             // 00650C16 ebp
    target_id = kotor_fastcall_00692630(msg, nullptr);                     // 00650C1F -> B+54
    p0[0] = kotor_fastcall_004d67b0(msg, nullptr, kOneBits, 0x20);         // 00650C31 -> B+3C
    p0[1] = kotor_fastcall_004d67b0(msg, nullptr, kOneBits, 0x20);         // -> B+40
    p0[2] = kotor_fastcall_004d67b0(msg, nullptr, kOneBits, 0x20);         // -> B+44
    p1[0] = kotor_fastcall_004d67b0(msg, nullptr, kOneBits, 0x20);         // -> B+1C
    p1[1] = kotor_fastcall_004d67b0(msg, nullptr, kOneBits, 0x20);         // -> B+20
    p1[2] = kotor_fastcall_004d67b0(msg, nullptr, kOneBits, 0x20);         // -> B+24
    U delay = kotor_fastcall_004d6780(msg, nullptr, 0x20);                 // 00650C98 -> B+2C
    const U8 kind = kotor_fastcall_004d6720(msg, nullptr, 8);              // 00650CA5 bl
    (void)kotor_fastcall_004d6780(msg, nullptr, 0x20);                     // 00650CB0 (discarded)
    (void)kotor_fastcall_004d6780(msg, nullptr, 0x20);                     // 00650CB9 (discarded)
    if (kotor_fastcall_004d6230(msg, nullptr, 1) != 0) return 0;          // 00650CC2..00650CC9
    if (kind == 6) {                                                       // 00650CCF
        spell_id = kotor_fastcall_004d6780(msg, nullptr, 0x20);            // 00650CD8 -> B+4C
    } else if (kind == 7) {                                                // 00650CE3
        spell_id = kotor_fastcall_004d6780(msg, nullptr, 0x20);            // 00650CEA -> B+4C
        projectile_type = kotor_fastcall_004d6720(msg, nullptr, 8);        // 00650CF7 movzx -> B+28
    } else {
        projectile_type = kotor_fastcall_004d6720(msg, nullptr, 8);        // 00650D07 movzx -> B+28
        (void)kotor_fastcall_004d6720(msg, nullptr, 8);                    // 00650D17
        (void)kotor_fastcall_004d6720(msg, nullptr, 8);                    // 00650D20
    }
    if (kotor_fastcall_004d6230(msg, nullptr, 1) != 0) return 0;          // 00650D29..00650D30
    void* caster_object = kotor_fastcall_005ed580(client(), nullptr, caster_id);   // 00650D36..00650D40
    void* caster = nullptr;                                                // edi
    if (caster_object != nullptr) caster = vcall<void*>(caster_object, 0x0c);   // 00650D49..00650D50 +0C
    void* target_object = kotor_fastcall_005ed580(client(), nullptr, target_id);   // 00650D52..00650D5F
    void* target = nullptr;                                                // ebp ([B+50] = 0 otherwise, 00650D73)
    if (target_object != nullptr) target = vcall<void*>(target_object, 0x0c);   // 00650D68..00650D6F
    // 00650D77..00650DBA: d = p1 - p0 (each difference rounded to float), |d|^2 compared with 1e-5 (53-bit x87)
    const float dz = static_cast<float>(static_cast<double>(p1[2]) - p0[2]);
    const float dy = static_cast<float>(static_cast<double>(p1[1]) - p0[1]);   // fst [B+50]
    const float dx = static_cast<float>(static_cast<double>(p1[0]) - p0[0]);   // fst [B+48]
    const double ex = static_cast<double>(p1[0]) - p0[0];                  // st values stay unrounded in the sum
    const double ey = static_cast<double>(p1[1]) - p0[1];
    const double ez = static_cast<double>(p1[2]) - p0[2];
    const double square = (ex * ex + ey * ey) + ez * ez;                  // 00650D99..00650DA7
    if (square > static_cast<double>(kEpsilon_00747504)) {                 // fcomp; test ah,41h; jne -> else
        const float d[3] = {dx, dy, dz};                                   // B+30..38 (00650DBC..00650DD9)
        float n[3];                                                        // B+90
        const void* r = normalize_004ab180(n, d);                          // 00650DDD
        std::memcpy(dir, r, 12);                                           // 00650DE2..00650E25 -> B+58..60
    } else {
        dir[0] = 1.0f; dir[1] = 0.0f; dir[2] = 0.0f;                       // 00650DF7..00650E25 (via B+10..18)
    }
    (void)dy; (void)dz;
    if (target_id != kInvalid_00746644 && target == nullptr) {             // 00650E29..00650E39
        void* dummy = construct_or_free(0x44c, [](void* m) {               // 00650E3F..00650E62 (EH state 0)
            return kotor_m15a_creature_construct_00616a20(m, nullptr);
        });
        if (dummy == nullptr) return 0;                                    // 00650E64..00650E71
        ResRef dummy_ref;                                                  // B+90
        wr(dummy, 0x310, 0);                                               // 00650E83
        resref_from(&dummy_ref, kDummy_00751cd8);                          // 00650E77..00650E8D CResRef("c_dummy")
        const U loaded = vcall<U>(dummy, 0x78, static_cast<const void*>(&dummy_ref), 0xffu, 0u);   // 00650E92..00650EA5
        void* player = nullptr;
        void* area = nullptr;
        bool registered = false;
        if (loaded != 0 && client_get_player_creature_005ed540(client(), nullptr) != nullptr) {   // 00650EA8..00650EC0
            player = client_get_player_creature_005ed540(client(), nullptr);   // 00650EC6..00650ECF
            area = rp(player, 0x20);                                       // 00650ED4 ebp=[eax+20]
            if (area != nullptr) {                                         // 00650ED7
                U new_id;                                                  // B+64
                const U8 status = area_vfx_register_existing_004d80e0(kotor_fastcall_005ed570(client(), nullptr), nullptr,
                                                                      &new_id, dummy, 0);   // 00650EDF..00650EF6
                if (status != kArrayOk_0074666c) return 0;                 // 00650EFB..00650F01 (creature leaked)
                vcall<void>(dummy, 0x04, new_id);                          // 00650F1B..00650F24 +04 (id)
                vcall<void>(dummy, 0x84, area);                            // 00650F27..00650F2C +84 (area)
                (void)recovered_client_ai_master_add_object_00604a20(
                    kotor_m15e_client_get_client_ai_master_005ed6b0(client(), nullptr), nullptr, dummy);   // 00650F32..00650F42
                vcall<void>(dummy, 0x94, rd(area, 0x184));                 // 00650F47..00650F52 +94 ([area+184])
                vcall<void>(dummy, 0x8c, static_cast<const void*>(p1));    // 00650F58..00650F61 +8C (B+1C)
                copy3(at(dummy, 0x1dc), p1);                               // 00650F67..00650F7E
                wr16(rp(dummy, 0x21c), 0x18, 0xc9);                        // 00650F81..00650F87
                target_id = new_id;                                        // 00650F8D..00650F91 B+54 = B+64
                registered = true;
            }
        }
        (void)registered;
        recovered_client_object_set_desired_fade_state_0063ce60(dummy, nullptr, 1, 1, delay + 0x1f4, 0);   // 00650F95..00650FA8
        wr(dummy, 0xe4, 1);                                                // 00650FB1
        (void)vcall<U>(dummy, 0x0c);                                       // 00650FAD..00650FBB +0C
    }
    if (kind != 6 && kind != 7) return 1;                                  // 00650FBE..00650FC6
    U8* const spell = static_cast<U8*>(
        kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, spell_id));   // 00650FCC..00650FDD ebp
    if (spell == nullptr) return 1;                                        // 00650FE4..00650FE6
    float orientation[4];                                                  // B+68..74
    U orientation_bits[4] = {kOneBits, 0, 0, 0};                           // 00650FEC..0065100B ([68]=1.0, 6C..74=0)
    std::memcpy(orientation, orientation_bits, 16);
    float start[3] = {0, 0, 0};                                            // B+10..18 (00651013..00651023)
    ResRef sound_ref;                                                      // B+80
    resref_default(&sound_ref);                                            // 0065102B
    start[0] = p0[0];                                                      // 00651051
    start[1] = p0[1];                                                      // 00651055
    start[2] = static_cast<float>(static_cast<double>(p0[2]) + kOne_0073d764);   // 00651030..0065104D fadd [0073D764]
    if (kind == 6) projectile_type = rd(spell, 0x108);                     // 00651034..00651061
    if (caster != nullptr) {                                               // 00651065
        if (vcall<void*>(caster, 0x2c) != nullptr) {                       // 0065106D..00651076 +2C
            void* visual = rp(vcall<void*>(caster, 0x2c), 0x2f0);          // 0065107C..00651085
            void* owner = vcall<void*>(caster, 0x2c);                      // 00651083..0065108D
            ResRefValue hand, head, ground;
            std::memcpy(&hand, spell + 0xac, 16);                          // 006510D3..006510F3
            std::memcpy(&head, spell + 0xbc, 16);                          // 006510B2..006510D0
            std::memcpy(&ground, spell + 0xcc, 16);                        // 00651091..006510AF
            (void)kotor_fastcall_00696250(visual, nullptr, hand, head, ground, owner);   // 006510F6
            wr(rp(vcall<void*>(caster, 0x2c), 0x2f0), 0x20, 1);            // 006510FB..00651108
        }
        if (vcall<void*>(caster, 0x98, 0xffu, 1u) != nullptr) {            // 0065110F..00651122 +98 (FFh,1)
            const U8 hand = spell[0x10c];                                  // 00651128
            const char* node = hand == 1 ? kHandConjure_00751d74
                             : hand == 2 ? kHeadConjure_0074f850
                             : hand == 3 ? kRHand_0074f334
                                         : kRoot_00740c00;                 // 0065112E..0065119C
            void* gob = vcall<void*>(caster, 0x98, 0xffu, 1u);             // 0065113D / 0065115A / 00651175 / 0065118C
            (void)vcall<U>(gob, 0x98, node, static_cast<void*>(start), static_cast<void*>(orientation));   // 006511A1..006511A5
        }
    }
    {
        ResRef scratch;                                                    // B+90
        (void)ref_assign_copy_00406120(&sound_ref, nullptr, &scratch, spell + 0xdc);   // 006511AB..006511C1
    }
    const U8* const sound_text = static_cast<const U8*>(kotor_m15e_slot00c_self_00641db0(&sound_ref, nullptr));   // 006511C6..006511CD
    if (*sound_text != 0) {                                                // 006511D2..006511D5
        const float* where = caster != nullptr ? reinterpret_cast<const float*>(at(caster, 0x24)) : p0;   // 006511E9..0065120C
        float pos[3];
        std::memcpy(pos, where, 12);
        kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &sound_ref, fb(pos[0]), fb(pos[1]),
                                                  fb(pos[2]), kOneAndHalfBits, 0xd, 0, 0, 0, 0);   // 006511D7..00651220
    }
    if (rd(spell, 0xf4) == 0) return 1;                                    // 00651225..0065122D
    void* const model = spell + 0xf8;                                      // 0065127F ebp += F8h
    void* projectile = construct_or_free(0x1c8, [](void* m) {              // 00651233..0065125C (EH state 1)
        return object_projectile_006d57b0(m, nullptr);
    });
    object_projectile_006d41b0(projectile, nullptr, spell_id);             // 00651260..00651271
    if (vcall<U>(projectile, 0x78, static_cast<const void*>(model), 0xffu, 0u) == 0) {   // 00651276..0065128D
        vcall<void>(projectile, 0x00, 1u);                                 // 0065149D..006514A3 deleting destructor
        return 1;
    }
    float position[3];                                                     // B+30..38
    std::memcpy(position, start, 12);                                      // 00651293..006512B0
    vcall<void>(projectile, 0x8c, static_cast<const void*>(position));     // 006512AB..006512B4 +8C
    if (projectile_type != 7) {                                            // 006512BA
        const float up[3] = {0.0f, 1.0f, 1.0f};                            // 006512CA..006512DA (B+30..38)
        std::memcpy(position, up, 12);
        vcall<void>(projectile, 0x88, static_cast<const void*>(position)); // 006512C1..006512E2 +88
    }
    void* module = kotor_fastcall_005ed530(client(), nullptr);             // 006512E8..006512F1
    if (module != nullptr && rp(module, 0x48) != nullptr)                  // 006512F6..006512FF
        vcall<void>(projectile, 0x94, rd(rp(module, 0x48), 0x184));        // 00651301..0065130C
    const U timer = delay;                                                 // 00651312 edi = B+2C
    object_projectile_006d2c30(projectile, nullptr, timer);                // 00651316..00651319
    object_projectile_006d4250(projectile, nullptr, 1);                    // 0065131E..00651322
    copy3(at(projectile, 0x114), p1);                                      // 00651327..00651342
    object_projectile_006d6ee0(projectile, nullptr, projectile_type);      // 0065133E..00651348
    if (projectile_type != 4) return 1;                                    // 0065134D..00651358
    U remaining = 4;                                                       // 0065135E B+2C = 4
    do {
        void* extra = construct_or_free(0x1c8, [](void* m) {               // 00651362..0065138B (EH state 2)
            return object_projectile_006d57b0(m, nullptr);
        });
        object_projectile_006d41b0(extra, nullptr, spell_id);              // 0065138F..0065139D
        if (vcall<U>(extra, 0x78, static_cast<const void*>(model), 0xffu, 0u) != 0) {   // 006513A2..006513B3
            std::memcpy(position, start, 12);                              // 006513B9..006513DA
            vcall<void>(extra, 0x8c, static_cast<const void*>(position));  // +8C
            vcall<void>(extra, 0x88, static_cast<const void*>(dir));       // 006513E0..006513E9 +88 (B+58)
            void* m2 = kotor_fastcall_005ed530(client(), nullptr);         // 006513EF..006513F8
            if (m2 != nullptr && rp(m2, 0x48) != nullptr)
                vcall<void>(extra, 0x94, rd(rp(m2, 0x48), 0x184));         // 00651401..00651413
            object_projectile_006d2c30(extra, nullptr, timer);             // 00651419..0065141C
            object_projectile_006d4250(extra, nullptr, 1);                 // 00651421..00651425
            copy3(at(extra, 0x114), p1);                                   // 0065142A..00651441
            {
                ExoString node(kEmpty_0073d71c);                           // 00651444..0065144D (EH state 3)
                object_projectile_006d42d0(extra, nullptr, target_id, &node);   // 00651452..00651469
            }                                                              // 0065146E..00651479
            object_projectile_006d6ee0(extra, nullptr, 4);                 // 0065147E..00651482
        } else {
            vcall<void>(extra, 0x00, 1u);                                  // 00651489..0065148F deleting destructor
        }
    } while (--remaining != 0);                                            // 00651491..00651495
    return 1;                                                              // 006514A5..006514BF
}

// ================================================================================================================
// 006501B0 HandleServerToPlayerSafeProjectileProjectile() RET
extern "C" U __fastcall kotor_fastcall_006501b0(void* self, void*) noexcept(false) {
    void* const msg = self;                                                // esi
    U result = 0;                                                          // B+1C (006501D9)
    float origin[3];                                                       // B+A4..AC
    float target_pos[3];                                                   // B+3C..44
    const U shooter_id = kotor_fastcall_00692630(msg, nullptr);            // 006501DD edi, B+8C
    U target_id = kotor_fastcall_00692630(msg, nullptr);                   // 006501ED B+14
    (void)kotor_fastcall_004d7120(msg, nullptr, origin);                   // 006501F6..00650200
    (void)kotor_fastcall_004d7120(msg, nullptr, target_pos);               // 00650205..0065020C
    const U timer_word = kotor_fastcall_004d6780(msg, nullptr, 0x20);      // 00650215 -> B+28
    const U8 kind = kotor_fastcall_004d6720(msg, nullptr, 8);              // 00650222 -> B+13
    const U hook = kotor_fastcall_004d6780(msg, nullptr, 0x20);            // 0065022F ebp
    const U8 row = kotor_fastcall_004d6720(msg, nullptr, 8);               // 0065023A -> B+11
    const U8 index = kotor_fastcall_004d6720(msg, nullptr, 8);             // 00650247 -> B+12
    const U extra_word = kotor_fastcall_004d6780(msg, nullptr, 0x20);      // 00650254 -> B+48
    U slot = 0;                                                            // ebx (006501D4)
    void* const shooter = frame_R4_005ed5b0(client(), nullptr, shooter_id);   // 00650259..00650267 edi
    if (shooter == nullptr) return result;                                 // 0065026E..00650270
    float from[3];                                                         // B+54..5C
    std::memcpy(from, origin, 12);                                         // 00650276..00650297
    ExoString hook_node;                                                   // B+20 (EH state 0)
    U use_node = 0;                                                        // esi
    if (hook == 1) {                                                       // 006502A2
        void* gob = vcall<void*>(shooter, 0x98, 0xffu, hook);              // 006502AE..006502B8
        if (vcall<U>(gob, 0x10c, kLBullet_00751d6c) != 0) {                // 006502BE..006502D3
            use_node = hook;                                               // 006502D5
            (void)frame_LogAssign_005e5140(&hook_node, nullptr, kLBullet_00751d6c);   // 006502D7 -> 0065033C
        } else {
            slot = 0x20;                                                   // 006502DE
            (void)frame_LogAssign_005e5140(&hook_node, nullptr, kBulletHook_0074f590);   // 006502E3 -> 0065033C
        }
    } else if (hook == 0) {                                                // 006502EA
        void* gob = vcall<void*>(shooter, 0x98, 0xffu, 1u);                // 006502EE..006502F9
        if (vcall<U>(gob, 0x10c, kRBullet_00751d64) != 0) {                // 006502FF..00650314
            use_node = 1;                                                  // 00650316
            (void)frame_LogAssign_005e5140(&hook_node, nullptr, kRBullet_00751d64);
        } else {
            slot = 0x10;                                                   // 00650322
            (void)frame_LogAssign_005e5140(&hook_node, nullptr, kBulletHook_0074f590);
        }
    } else if (hook == 2) {                                                // 0065032E
        (void)frame_LogAssign_005e5140(&hook_node, nullptr, kImpact_00743318);   // 00650333..0065033C
    }
    float orientation[4];                                                  // B+B0..BC
    const U orientation_bits[4] = {kOneBits, 0, 0, 0};                     // 00650343..00650364
    std::memcpy(orientation, orientation_bits, 16);
    void* source_gob = nullptr;                                            // B+18 (0065036F)
    void* node_gob = nullptr;                                              // esi at 00650412
    bool call_node = false;
    if (use_node != 0) {                                                   // 00650341 / 00650377
        node_gob = vcall<void*>(shooter, 0x98, 0xffu, 1u);                 // 00650379..00650384
        source_gob = node_gob;                                             // 00650398 B+18
        call_node = true;                                                  // unchecked (-> 00650412)
    } else {
        void* gob = nullptr;
        bool have = false;
        if (hook == 2) {                                                   // 0065039F
            gob = vcall<void*>(shooter, 0x98, 0xffu, 1u);                  // 006503A4..006503AF
            have = true;
        } else {
            void* item = kotor_fastcall_005ed580(client(), nullptr,
                                                 kotor_m15a_creature_get_equipped_item_id_0060ca80(shooter, nullptr, slot));   // 006503B7..006503CB
            if (item != nullptr && vcall<void*>(item, 0x34) != nullptr) {  // 006503D2..006503DF +34
                void* client_item = vcall<void*>(item, 0x34);              // 006503E1..006503E5
                gob = vcall<void*>(at(client_item, 0x10), 0x98, 0xffu, 1u);   // 006503E8..006503F5 (object at +10)
                have = true;
            }
        }
        if (have) {
            source_gob = gob;                                              // 006503FD B+18
            if (gob != nullptr) { node_gob = gob; call_node = true; }      // 00650401..00650403
        }
    }
    if (call_node)                                                         // 00650412..00650420 gob +98 (node, &pos, &orient)
        (void)vcall<U>(node_gob, 0x98, frame_LogCStr_005e5670(&hook_node, nullptr), static_cast<void*>(from),
                       static_cast<void*>(orientation));
    if (row == 0xff) return result;                                        // 00650426..0065042B (-> 00650B49)
    ExoString model_column;                                                // B+94 (state 1)
    ExoString model_name;                                                  // B+7C (state 2)
    ExoString shot_column;                                                 // B+74 (state 3)
    ExoString shot_sound;                                                  // B+6C (state 4)
    ExoString impact_column;                                               // B+84 (state 5)
    ExoString impact_sound;                                                // B+9C (state 6)
    ExoString muzzle_model;                                                // B+64 (state 7)
    void* const items_2da = rp(rp(rules(), 0xb8), 0x64);                   // 006504A9..006504B5 ebx -> B+50
    void* projectile = construct_or_free(0x1c8, [](void* m) {              // 006504B8..006504EA (EH state 8)
        return object_projectile_006d57b0(m, nullptr);
    });
    const U model_index = index;                                           // 006504EE..006504F4 -> B+1C
    result = model_index;
    frame_LogFormat_005e5680(&model_column, kModelFmt_00751d5c, model_index);   // 006504F8..0065050D
    const int row_index = row;                                             // 00650512 ebp (-> B+60)
    (void)c2da_current_get_string_00413270(items_2da, nullptr, row_index, &model_column, &model_name);   // 0065051A..0065052E
    ResRef model_ref;                                                      // B+2C
    (void)async_ref_construct_string_00406d60(&model_ref, nullptr, &model_name);   // 00650533..0065053C
    if (vcall<U>(projectile, 0x78, static_cast<const void*>(&model_ref), 0xffu, 0u) == 0) {   // 00650541..00650556
        vcall<void>(projectile, 0x00, 1u);                                 // 0065055C..00650562 deleting destructor
        return 0;                                                          // 00650564..0065060F (strings destroyed)
    }
    resref_from(&model_ref, kDefault_0073e304);                            // 00650610..00650619 CResRef("DEFAULT")
    vcall<void>(projectile, 0xac, 0xffu, 1u, 0u, static_cast<void*>(&model_ref));   // 0065061E..00650630 +AC
    vcall<void>(projectile, 0x8c, static_cast<const void*>(from));         // 00650636..0065063F +8C (B+54)
    float delta[3];                                                        // B+2C..34 (over the resref)
    delta[2] = static_cast<float>(static_cast<double>(target_pos[2]) - origin[2]);   // 00650645..00650649
    delta[1] = static_cast<float>(static_cast<double>(target_pos[1]) - origin[1]);   // 00650652..0065065A
    delta[0] = static_cast<float>(static_cast<double>(target_pos[0]) - origin[0]);   // 00650662..0065066D
    std::memcpy(&model_ref, delta, 12);                                    // 00650675..0065067D fstp x3
    float unit[3];                                                         // B+C0
    vcall<void>(projectile, 0x88, static_cast<const void*>(normalize_004ab180(unit, &model_ref)));   // 00650681..0065068C +88
    wr8(projectile, 0x1a1, 2);                                             // 00650699
    object_projectile_006d2c30(projectile, nullptr, timer_word);           // 00650692..006506A0
    wr(projectile, 0x170, 0);                                              // 006506A9
    object_projectile_006d4250(projectile, nullptr, 1);                    // 006506A5..006506B3
    frame_LogFormat_005e5680(&impact_column, kImpactSoundFmt_00751d4c, result);   // 006506B8..006506CA (B+1C)
    (void)c2da_current_get_string_00413270(items_2da, nullptr, row_index, &impact_column, &impact_sound);   // 006506D2..006506E7
    resref_from(&model_ref, frame_LogCStr_005e5670(&impact_sound, nullptr));   // 006506EC..006506FD
    {
        ResRef scratch;                                                    // B+C0
        (void)ref_assign_copy_00406120(at(projectile, 0x140), nullptr, &scratch, &model_ref);   // 00650702..00650715
    }
    U type;
    switch (kind) {                                                        // 0065071A..00650732 (index byte table 00650B84)
    case 1: case 2: case 3: case 5: {                                      // 00650739
        {
            ExoString node(kImpact_00743318);                              // B+8C (state 9)
            object_projectile_006d42d0(projectile, nullptr, target_id, &node);   // 0065074A..00650761
        }
        wr8(projectile, 0x158, 1);                                         // 00650784
        kotor_fastcall_006d42a0(projectile, nullptr, 1, kind == 2 ? 1u : 0u, 0);   // 0065077A..00650790
        type = 1;                                                          // 00650795 -> 006509C8
        break;
    }
    case 8: case 9: case 10: {                                             // 0065079A
        wr(projectile, 0x1c4, shooter_id);                                 // 006507B1 ([B+8C])
        wr8(projectile, 0x158, 1);                                         // 006507B7
        wr(projectile, 0x1bc, extra_word);                                 // 006507BE ([B+48])
        {
            ExoString node(kImpactBolt_00751d40);                          // B+C0 (state 10)
            object_projectile_006d42d0(projectile, nullptr, target_id, &node);   // 006507C9..006507E0
        }
        if (kind == 8) {                                                   // 006507F9
            copy3(at(projectile, 0x1b0), target_pos);                      // 006507FE..00650815
            type = 0x0e;                                                   // 00650818
        } else if (kind == 10) {                                           // 0065081F
            float radius = 0.0f;                                           // 00650832 B+28 = 0
            void* target = frame_R4_005ed5b0(client(), nullptr, target_id);   // 00650828..0065083E edi
            if (target != nullptr) {                                       // 00650845
                vcall<void>(projectile, 0x84, rp(target, 0x20));           // 0065084D..00650855 +84
                const int appearance = rd16(rp(target, 0x21c), 0x18);      // 0065085B..00650861 ebp
                const U shield = kotor_animation_sound_damage_effect_00616890(target, nullptr);   // 00650865..00650867 ebx
                int found = 0;                                             // B+14 (00650874)
                U column = 0;                                              // edi
                ExoString name;                                            // B+48 (state 11)
                if (found != appearance) {                                 // 0065087D..00650889
                    for (;;) {
                        ++column;                                          // 00650890
                        if (static_cast<int>(column) > 4) break;           // 00650891..00650894
                        frame_LogFormat_005e5680(&name, kAppearanceFmt_00751d30, column);   // 00650896..006508A1
                        (void)c2da_current_get_int_00413660(rp(rp(rules(), 0xb8), 0x10c), nullptr, static_cast<int>(shield),
                                                            &name, &found);   // 006508A6..006508C8
                        if (found == appearance) break;                    // 006508CD..006508D1
                    }
                }
                frame_LogFormat_005e5680(&name, kRadiusFmt_00751d24, column);   // 006508D3..006508DE
                (void)c2da_current_get_float_00413430(rp(rp(rules(), 0xb8), 0x10c), nullptr, static_cast<int>(shield),
                                                      &name, &radius);     // 006508E3..00650904
            }                                                              // 00650909..00650915 (~name, state 7)
            wr(projectile, 0x1c0, fbits(radius));                          // 0065091E..00650922
            copy3(at(projectile, 0x1b0), target_pos);                      // 00650928..00650943
            kotor_fastcall_006d42a0(projectile, nullptr, 1, 0, 0);         // 0065093F..0065094A
            type = 0x10;                                                   // 0065094F
        } else {                                                           // kind 9: 00650953
            wr8(projectile, 0x158, 1);                                     // 00650959
            kotor_fastcall_0064b4f0(projectile, nullptr, at(shooter, 0x24));   // 00650953..00650960
            kotor_fastcall_006d42a0(projectile, nullptr, 1, 0, 0);         // 00650965..0065096D
            type = 0x0f;                                                   // 00650972
        }
        break;
    }
    default: {                                                             // 00650976 (kinds 0, 4, 6, 7, >10)
        {
            ExoString node(kEmpty_0073d71c);                               // B+2C (state 12)
            object_projectile_006d42d0(projectile, nullptr, kInvalid_00746644, &node);   // 00650984..00650998
        }
        copy3(at(projectile, 0x114), target_pos);                          // 006509AE..006509C5
        type = 1;                                                          // 006509C8
        break;
    }
    }
    object_projectile_006d6ee0(projectile, nullptr, type);                 // 006509CA..006509CC
    void* module = kotor_fastcall_005ed530(client(), nullptr);             // 006509D1..006509D9
    if (module != nullptr && rp(module, 0x48) != nullptr) {                // 006509DE..006509E7
        vcall<void>(projectile, 0x94, rd(rp(module, 0x48), 0x184));        // 006509E9..006509F4 +94
        {
            ExoString column(kMuzzleFlash_00751d18);                       // B+2C (state 13)
            (void)c2da_current_get_string_00413270(items_2da, nullptr, row_index, &column, &muzzle_model);   // 006509FA..00650A1F
        }                                                                  // 00650A24..00650A30
        kotor_fastcall_006d4440(projectile, nullptr, &muzzle_model, source_gob, &hook_node);   // 00650A35..00650A46
    }
    frame_LogFormat_005e5680(&shot_column, kShotSoundFmt_00751d0c, result);   // 00650A4B..00650A5A (B+1C)
    (void)c2da_current_get_string_00413270(items_2da, nullptr, row_index, &shot_column, &shot_sound);   // 00650A62..00650A71
    ResRef sound_ref;                                                      // B+2C
    resref_from(&sound_ref, frame_LogCStr_005e5670(&shot_sound, nullptr)); // 00650A76..00650A84
    kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &sound_ref, fb(from[0]), fb(from[1]),
                                              fb(from[2]), 0, 0xf, 0, 0, 0, 0);   // 00650A89..00650ABC
    result = 1;                                                            // 00650AC1..00650AC5
    return result;                                                         // 00650AC1..00650B76 (strings destroyed)
}
