// Lane "dialogue-start", fork B2, part 2: CGuiInGame::UpdateShotCamera 006306E0 and
// CSWBehaviorCameraDialog::Control 006BCF50, transcribed from the original swkotor.exe (read-only).
// x87 conventions as in claude_dialogue_camera.cpp (PC_53; fl() = fld dword, fs() = fstp dword).
#include "1289b1b830-claude_dialogue_camera.hpp"

using namespace dlg;

extern "C" {
extern void* kotor_cell_rules_007a3a28;                                                    // [007A3A28] CSWRules*
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                     // CClientExoApp::GetGameObject
U __fastcall kotor_fastcall_006146e0(void* creature, void*, U target, U radius_bits);    // CSWCCreature::LookAt
U __fastcall recovered_client_creature_set_lock_orientation_00610950(void* creature, void*, U id, U force);
double __fastcall kotor_fastcall_0060e250(void* creature, void*);                         // GetHeadTurnHorizontal (ST0)
void __fastcall kotor_fastcall_0060b3c0(void* creature, void*, const void* direction);    // setStationaryOrentation
U __fastcall kotor_fastcall_0062d700(void* gui, void*, U anim);                           // IsFireForgetDialogAnimation
U __fastcall kotor_fastcall_0062d5e0(void* gui, void*, U anim);                           // IsLoopingDialogAnimation
U __fastcall c2da_current_get_float_00413430(void* table, void*, I row, void* column, float* out); // C2DA::GetFLOATEntry
void __fastcall kotor_fastcall_005edf10(void* client, void*, I effect);                   // EnableVideoEffect
void __fastcall kotor_fastcall_005edf20(void* client, void*);                             // DisableVideoEffect
U __fastcall behavior_control_0049f270(void* behavior, void*, U delta);                   // CAurBehavior::Control
}

namespace {
constexpr U kOne = 0x3f800000u;          // [0073D764] 1.0f
constexpr U kMinusOne = 0xbf800000u;     // [00740CBC] -1.0f
constexpr U kRadToDeg = 0x42652ee1u;     // [00742EF8] 57.29578f
constexpr U kDegToRad = 0x3c8efa35u;     // [007415F8] 0.017453292f
constexpr U kTen = 0x41200000u;          // 10.0f (LookAt radius immediate)
constexpr U kCameraHookHeight = 0x3dcccccdu; // [0073E798] 0.1f

// CRT acos 006FC0E0 on the x87 stack (ST0 -> ST0); the operand and result are exact doubles under PC_53.
// The operand is loaded exactly as an 80-bit image; the result image is stored back as a double (exact under PC_53,
// where the CRT routine's result is already rounded to 53 bits).
__declspec(noinline) double x87_acos_006fc0e0(double x) {
    CrtExtended operand, image;
    CrtExtended* const in = &operand;
    CrtExtended* const out = &image;
    double result;
    __asm {
        fld qword ptr x
        mov eax, in
        fstp tbyte ptr [eax]
    }
    crt_acos_x87_006fc0e0(in, out);
    __asm {
        mov eax, out
        fld tbyte ptr [eax]
        fstp qword ptr result
    }
    return result;
}

// 00630902..0063094F / 00630C60..00630CAD: clamp the dot product to [-1, 1] with the original compare exits.
U clamp_unit(double dot) {
    if (jp(fcom(fl(kOne), dot), 0x05)) {                      // fld 1.0; fcomp st(1); test ah,5; jnp (1 < dot)
        if (!jne(fcom(fl(kMinusOne), dot), 0x41)) return kMinusOne; // fld -1.0; fcomp st(1); test ah,41h; jne
    }
    if (!jp(fcom(fl(kOne), dot), 0x05)) return kOne;          // fld 1.0; fcomp st(1); test ah,5; jp
    return fs(dot);                                           // fstp dword
}

// dot(a, b) = (a.z*b.z + a.y*b.y) + a.x*b.x in the original FPU order
double dot3(const V3& a, const V3& b) {
    double s = fl(a.z) * fl(b.z);
    s = s + fl(a.y) * fl(b.y);
    s = s + fl(a.x) * fl(b.x);
    return s;
}

// The head-follow rotation of one participant (0063089D..00630A8A for the speaker, 00630BFB..00630DE9 for the
// owner). `self_obj` turns toward `other_obj`; `facing` is the copy of self+30h being rotated in place.
// Returns false when GetCanHeadFollowInDialog fails (the caller then clears its orient flag).
bool head_follow(void* gui, void* self_obj, void* other_obj, V3* facing) {
    if (!kotor_fastcall_0062f3c0(gui, nullptr, rd(self_obj, 4))) return false;          // GetCanHeadFollowInDialog
    V3 delta;
    const void* d = kotor_cdecl_00423bc0(&delta, at(other_obj, 0x24), at(self_obj, 0x24)); // other - self
    V3 to_other;                                                                           // normalize(other - self)
    (void)normalize_004ab180(&to_other, d);
    V3 face;                                                                               // normalize(self+30h)
    (void)normalize_004ab180(&face, at(self_obj, 0x30));
    U angle = clamp_unit(dot3(face, to_other));
    angle = fs(x87_acos_006fc0e0(fl(angle)) * fl(kRadToDeg));                              // facos; fmul [00742EF8]
    const U limit = fs(kotor_fastcall_0060e250(vcall<void*>(self_obj, 0x2c), nullptr));   // GetHeadTurnHorizontal
    if (!kotor_cdecl_006fe517(fl(angle))) angle = 0;                                       // __finite
    if (jne(fcom(fl(angle), fl(limit)), 0x41)) return true;                                // angle <= limit: no turn
    angle = fs(fl(angle) - (fl(limit) + fl(kOne)));                                        // fadd 1.0; fsubr; fstp
    const V3 up{0, 0, kOne};
    V3 side;
    (void)vector_cross_004ab210(&side, &face, &up);
    double turn = fl(angle);
    if (!jne(fcom(dot3(side, to_other), 0.0), 0x41)) turn = turn * fl(kMinusOne);          // fcomp 0.0 ... fmul -1.0
    turn = turn * fl(kDegToRad);                                                           // fmul [007415F8]
    U q[4];
    (void)quaternion_construct_004ac960(q, nullptr, V3{0, 0, kOne}, bitsf(fs(turn)));      // Quaternion(axis Z, angle)
    V3 rotated;
    const V3* r = static_cast<const V3*>(quaternion_rotate_004a9bb0(&rotated, q, &face));
    *facing = *r;
    return true;
}

// One participant's orientation tail (00630A94..00630B9A / 00630DF3..00630F5C).
// `oriented` is the head-follow flag; `fallback` is the direction used when LookAt fails or no head follow ran.
void orient_tail(void* self_obj, void* other_obj, bool oriented, const V3& facing, const V3& fallback) {
    (void)recovered_client_creature_set_lock_orientation_00610950(vcall<void*>(self_obj, 0x2c), nullptr, kInvalid, 0);
    const U sid = kotor_fastcall_004aea30(server(), nullptr, rd(self_obj, 4));             // ClientToServerObjectId
    void* sobj = kotor_fastcall_004ae750(server(), nullptr, sid);
    if (sobj && vcall<void*>(sobj, 0x30))
        w5_creature_set_lock_orientation_to_object_004f34a0(vcall<void*>(sobj, 0x30), nullptr, kInvalid, 0);
    const V3* use = &fallback;
    V3 face = facing;
    if (oriented && kotor_fastcall_006146e0(vcall<void*>(self_obj, 0x2c), nullptr, rd(other_obj, 4), kTen))
        use = &face;
    V3 dir = *use;
    if (sobj && vcall<void*>(sobj, 0x30)) w1x_object_set_orientation_004cd170(vcall<void*>(sobj, 0x30), nullptr, &dir);
    kotor_fastcall_0060b3c0(vcall<void*>(self_obj, 0x2c), nullptr, &dir);
    vcall<void>(vcall<void*>(self_obj, 0x2c), 0x88, &dir);
}
}  // namespace

extern "C" {

// 006306E0 CGuiInGame::UpdateShotCamera(shot, camera id, a3, a4, flag160, video effect) (RET 18h)
void __fastcall kotor_fastcall_006306e0(void* self, void*, U shot, U camera_id, U a3, U a4, U flag160, U effect) {
    void* o = kotor_fastcall_005ed580(client(), nullptr, rd(self, 0x178));                 // 00630700..00630711
    if (o && vcall<void*>(o, 0x2c))
        (void)kotor_fastcall_006146e0(vcall<void*>(o, 0x2c), nullptr, kInvalid, 0);        // 0063071C..00630737
    o = kotor_fastcall_005ed580(client(), nullptr, rd(self, 0x17c));                       // 0063073C..0063074C
    if (o && vcall<void*>(o, 0x2c))
        (void)kotor_fastcall_006146e0(vcall<void*>(o, 0x2c), nullptr, kInvalid, 0);        // 00630757..00630772
    if (shot == 5 && rd(self, 0x178) != kInvalid) {                                        // 00630777..0063078C
        wr(self, 0x170, rd(self, 0x178));
        wr(self, 0x174, rd(self, 0x17c));                                                  // 0063078E..0063079A
    }
    if (rd(in_game_gui(), 0xc0) == 0 && rd(self, 0x3c) != rd(self, 0x44)) {                // 006307A0..006307C2
        // The client game objects are dereferenced unchecked (original quirk).
        void* owner = vcall<void*>(kotor_fastcall_005ed580(client(), nullptr, rd(self, 0x170)), 0xc); // ebp
        void* speaker = vcall<void*>(kotor_fastcall_005ed580(client(), nullptr, rd(self, 0x174)), 0xc); // edi
        if (owner && speaker) {                                                            // 00630802..0063080E
            V3 d{fs(fl(rd(owner, 0x24)) - fl(rd(speaker, 0x24))),
                 fs(fl(rd(owner, 0x28)) - fl(rd(speaker, 0x28))),
                 fs(fl(rd(owner, 0x2c)) - fl(rd(speaker, 0x2c)))};                         // 00630814..00630838
            V3 n;                                                                          // [esp+18h]
            (void)normalize_004ab180(&n, &d);                                              // 0063083C
            const V3 neg_n{fs(-fl(n.x)), fs(-fl(n.y)), fs(-fl(n.z))};                      // fchs x3 (00630ECA..)
            // speaker faces the owner
            {
                V3 facing;                                                                 // [esp+3Ch] = speaker+30h
                std::memcpy(&facing, at(speaker, 0x30), 12);
                if (kotor_fastcall_0062f250(self, nullptr, rd(speaker, 4)) && vcall<void*>(speaker, 0x2c) &&
                    !jne(fcom(fl(rd(vcall<void*>(speaker, 0x2c), 0x20c)), 0.0), 0x41)) {    // 0063085C..00630897
                    const bool oriented = head_follow(self, speaker, owner, &facing);      // 0063089D..00630A8C
                    orient_tail(speaker, owner, oriented, facing, n);                      // 00630A94..00630B9A
                }
            }
            // owner faces the speaker
            {
                V3 facing;
                std::memcpy(&facing, at(owner, 0x30), 12);                                 // 00630BA0..00630BBB
                if (kotor_fastcall_0062f250(self, nullptr, rd(owner, 4)) && vcall<void*>(owner, 0x2c) &&
                    !jne(fcom(fl(rd(vcall<void*>(owner, 0x2c), 0x20c)), 0.0), 0x41)) {      // 00630BBF..00630BF5
                    const bool oriented = head_follow(self, owner, speaker, &facing);      // 00630BFB..00630DEB
                    orient_tail(owner, speaker, oriented, facing, neg_n);                  // 00630DF3..00630F5C
                }
            }
        }
    }
    // 00630F62: the camera shot
    if (shot != 5) {
        void* module = kotor_fastcall_005ed530(client(), nullptr);                         // 00630F72..00630F7B
        void* camera = vcall<void*>(rp(module, 0x40), 0x80, 0x106du);                      // 00630F80..00630F8A
        if (shot != 6) {
            if (camera) {                                                                  // 00630F9D..00630F9F
                U range = 0;                                                               // [esp+10h]
                if (shot == 1) {                                                           // 00630FA5..00630FB0
                    U16 anim = 0x2736;                                                     // 00630FC0 (ebp)
                    if (static_cast<I>(rd(self, 0x198)) > 0) {                             // 00630FB6..00630FC5
                        const U ids = rd(self, 0x1a0), anims = rd(self, 0x1a4);
                        for (I i = 0; i < static_cast<I>(rd(self, 0x198)); ++i) {          // 00630FE0..00630FFA
                            if (rd(ptr(ids), static_cast<U>(i) * 4) == rd(self, 0x170)) {
                                anim = rd16(ptr(anims), static_cast<U>(i) * 2);            // 00630FE6 / 00630FFE
                                break;
                            }
                        }
                    }
                    // the callees and the row computation read only the low word (cmp si / movzx edx, bp)
                    if (kotor_fastcall_0062d700(in_game_gui(), nullptr, anim) ||
                        kotor_fastcall_0062d5e0(in_game_gui(), nullptr, anim)) {           // 00631000..00631031
                        float value = 0;
                        U ok;
                        {
                            ExoString column("CU_PB_Range");                               // 00631037 ([007513E0])
                            void* table = rp(rp(kotor_cell_rules_007a3a28, 0xb8), 0x40);  // 00631045..00631051
                            std::memcpy(&value, &range, 4);
                            ok = c2da_current_get_float_00413430(table, nullptr, static_cast<I>(anim) - 0x2710,
                                                                 &column, &value);         // 00631054..00631073
                            std::memcpy(&range, &value, 4);
                        }                                                                  // 0063108F ~CExoString
                        if (ok == 0) range = 0;                                            // 00631078..00631098
                        bool zero = false;
                        if (!jp(fcom(fl(range), 0.001), 0x05)) {                           // 0063109C..006310AB (< 0.001)
                            if (!jne(fcom(fl(range), -0.001), 0x41)) { range = 0; zero = true; } // 006310AD..006310C6
                        }
                        if (!zero && !jp(fcom(fl(range), 0.0), 0x44)) zero = true;         // 006310C8..006310D7 (== 0)
                        if (zero) wr(self, 0xbb8, 0);                                      // 006310D9
                    }
                    if (!jne(fcom(fl(rd(self, 0xbb8)), 0.0), 0x41)) range = rd(self, 0xbb8); // 006310DF..006310F8
                    else wr(self, 0xbb8, range);                                           // 006310FE..00631102
                } else {
                    wr(self, 0xbb8, 0);                                                    // 0063110A
                }
                U shot_copy = shot;                                                        // [esp+30h]
                kotor_fastcall_006bd260(camera, nullptr, rd(self, 0x170), rd(self, 0x174), &shot_copy, &a3, &a4,
                                        range);                                            // 00631114..0063114B
                kotor_fastcall_005edf20(client(), nullptr);                                // 00631150..00631158
            }
        } else if (camera) {                                                               // 00631162..00631164
            bool found = false;
            U i = 0;                                                                       // ebp
            if (static_cast<I>(rd(self, 0x1b0)) > 0) {                                     // 0063116A..00631176
                for (;;) {
                    if (rd(self, 0x1b4 + i * 0x30) == camera_id) { found = true; break; }  // 00631185..00631187
                    ++i;
                    if (!(static_cast<I>(i) < static_cast<I>(rd(self, 0x1b0)))) break;     // 0063118F..00631195
                }
            }
            V3 position{0, 0, 0};                                                          // [esp+18h..20h]
            Q4 orientation{kOne, 0, 0, 0};                                                 // [esp+6Ch..78h]
            U fov = 0x425c0000u;                                                           // [esp+30h] 55.0f
            U shot_height = 0;                                                             // [esp+10h]
            if (found) {
                void* s = at(self, i * 0x30);                                              // 006311EE..006311F5
                position.x = rd(s, 0x1b8);
                position.y = rd(s, 0x1bc);
                position.z = fs(fl(rd(s, 0x1c0)) + fl(rd(s, 0x1d8)));                      // 0063120A..00631222
                Q4 source;
                std::memcpy(&source, at(s, 0x1c4), 16);
                orientation = source;                                                      // 00631226..0063123B
                const float roll = static_cast<float>(kotor_cdecl_004acb90(source));       // 0063123F..0063125F
                const float pitch = static_cast<float>(quaternion_pitch_004aa020(&orientation) + fl(rd(s, 0x1d4)));
                const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&orientation)); // 0063127D..00631283
                Q4 rotated;
                orientation = *static_cast<const Q4*>(yaw_pitch_roll_004acac0(&rotated, yaw, pitch, roll)); // ..006312AD
                fov = rd(s, 0x1dc);                                                        // 006312B0 / 006312CB
                shot_height = rd(self, 0x1e0 + i * 0x30);                                  // 006312B6..006312C7
            }
            kotor_fastcall_006bb590(camera, nullptr, 6, position, orientation, fov);       // 006312D5..0063131B
            wr(self, 0xe8, position.x);                                                    // 00631334
            wr(self, 0xec, position.y);
            wr(self, 0xe4, 1);
            wr(self, 0xf0, position.z);
            wr(self, 0xf4, shot_height);                                                   // 00631346
            if (effect != 0xffffffffu && effect != 0xfffffffeu)                            // 00631327..00631351
                kotor_fastcall_005edf10(client(), nullptr, static_cast<I>(effect));        // 00631353..0063135C
        }
    }
    wr(self, 0x160, flag160);                                                              // 00631361..00631369
    wr(self, 0x16c, 0);                                                                    // 00631376
}

// 006BCF50 CSWBehaviorCameraDialog::Control(delta) -> 1 (RET 4)
U __fastcall kotor_fastcall_006bcf50(void* self, void*, U delta) {
    if (rd(self, 0x3c) == 1 && !behavior_control_0049f270(self, nullptr, delta)) return 1; // 006BCF58..006BCF6B
    const U type = rd(self, 0x28);                                                         // 006BCF71 (edi)
    U side = delta;                                                                        // 006BCFB3 (ebp, type 6)
    if (type != 6) {
        if (!rd(self, 0x64) || !rd(self, 0x68)) return 1;                                  // 006BCF79..006BCF89
        side = kotor_fastcall_006bb610(self, nullptr, rd(self, 0x5c), rd(self, 0x60));     // 006BCF8F..006BCF99
        if (!side) return 1;                                                               // 006BCF9E..006BCFB0
    }
    V3 position;                                                                           // [esp+34h]
    std::memcpy(&position, at(self, 0x40), 12);                                            // 006BCFB7..006BCFCF
    Q4 orientation;                                                                        // [esp+40h]
    std::memcpy(&orientation, at(self, 0x4c), 16);                                         // 006BCFC6..006BCFED
    if (type != 6) {
        if (rd(self, 0x38)) {                                                              // 006BCFE8..006BCFF1
            V3 buf_b, buf_a;                                                               // [esp+28h], [esp+1Ch]
            const V3* b = vcall<const V3*>(rp(self, 0x68), 0x64, &buf_b);                  // 006BCFFC..006BD009 (edi)
            const V3* a = vcall<const V3*>(rp(self, 0x64), 0x64, &buf_a);                  // 006BD001..006BD017
            const V3 va = *a, vb = *b;
            kotor_fastcall_006bb7b0(self, nullptr, rd(self, 0x28), side, va, vb, &position, &orientation); // ..006BD17F
        } else {
            V3 hook{0, 0, 0};                                                              // [esp+10h]
            Q4 hook_orientation{kOne, 0, 0, 0};                                            // [esp+50h]
            V3 a, b;                                                                       // [esp+28h], [esp+1Ch]
            (void)vcall<void*>(rp(self, 0x64), 0x64, &a);                                  // 006BD05C..006BD09E
            (void)vcall<void*>(rp(self, 0x68), 0x64, &b);                                  // 006BD0A1..006BD0AB
            a.z = fs((fl(a.z) + fl(kCameraHookHeight)) + fl(rd(self, 0x14)));              // 006BD0AE..006BD0C8
            b.z = fs((fl(b.z) + fl(kCameraHookHeight)) + fl(rd(self, 0x18)));              // 006BD0D1..006BD0DE
            if (vcall<U>(rp(self, 0x64), 0x98, "CAMERAHOOK", &hook, &hook_orientation)) a = hook; // 006BD0E2..006BD102
            if (vcall<U>(rp(self, 0x68), 0x98, "CAMERAHOOK", &hook, &hook_orientation)) b = hook; // 006BD106..006BD138
            kotor_fastcall_006bb7b0(self, nullptr, rd(self, 0x28), side, a, b, &position, &orientation); // ..006BD17F
        }
    }
    // 006BD184: publish the position and the re-composed orientation to the owner gob
    vcall<void>(rp(self, 0x10), 0x5c, &position, position);                                // 006BD184..006BD1A7
    const float roll = static_cast<float>(kotor_cdecl_004acb90(orientation));              // 006BD1AA..006BD1CF
    const float pitch = static_cast<float>(quaternion_pitch_004aa020(&orientation) + fl(rd(self, 0x30))); // ..006BD1E7
    const float yaw = static_cast<float>(quaternion_yaw_004a9f40(&orientation) + fl(rd(self, 0x2c)));     // ..006BD1F7
    Q4 composed;                                                                           // [esp+50h]
    const Q4 result = *static_cast<const Q4*>(yaw_pitch_roll_004acac0(&composed, yaw, pitch, roll)); // 006BD1FA..006BD1FB
    orientation = result;                                                                  // 006BD200..006BD219
    vcall<void>(rp(self, 0x10), 0x60, &composed, result);                                  // 006BD21D..006BD23F
    return 1;                                                                              // 006BD245
}

}  // extern "C"
