// anim_hierarchy.cpp - lane-3d-anim: animation lookup and run management (FindAnimation, CurrentAnimation, AnimRun,
// Animation::~Animation, Gob::PlayAnimation), the per-frame driver Gob::Animate and its hierarchy walkers
// (AnimateHierarchy, ProceduralAnimateHierarchy) and key-event dispatch (AnimateEvents) of swkotor.exe (GOG K1).
// Every statement cites the original instruction(s) it reproduces. Float discipline per D30-FINAL (anim3d.h).
#include "3775ec35d8-anim3d.h"
#include <cstdio>
#include <cstdlib>

extern "C" int __cdecl crt_rand_006fc447();   // game CRT rand 006FC447 (shared holdrand stream; 0479-3d-model-model-lifetime)
using namespace kotor_anim3d;

namespace {
inline float f32(double value) { return static_cast<float>(value); }
inline void* word_ptr(U word) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word)); }
inline U ptr_word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* list_at(const PointerList* list, I index) { return word_ptr(reinterpret_cast<const U*>(word_ptr(list->data))[index]); }
inline void put_raw(void* dst, U value) { std::memcpy(dst, &value, 4); }
inline U raw_of(const void* src) { U u; std::memcpy(&u, src, 4); return u; }
// Gob (CAurObject) offsets used here: +04 SafePointer list, +16/+17 bytes, +2C focus distance, +58 model, +68 scene,
// +6C root part, +78 position, +94 previous position, +B0 AnimRun list, +156..+160 wind, +174 look-at animation,
// +17E animating, +184 procedural byte, +188 behavior, +18C last animated frame.
inline PointerList* runs_of(void* gob) { return static_cast<PointerList*>(at(gob, 0xB0)); }
// AnimRun (0x24 bytes, 00485DF6 push 24h; 0x200 runs carry +24..+30): +00 Animation*, +04 speed, +08 flags,
// +0C previous time, +10 time, +14 blend, +18 blend time, +1C scale, +20 finished.
inline void* run_animation(void* run) { return field<void*>(run, 0x00); }
inline const char* animation_name(void* animation) { return static_cast<const char*>(at(animation, 0x08)); }
const char kNull_0073ee04[] = "NULL";
const char kCurrent_00743288[] = "current";
const char kDone_00743280[] = "done%s";
const char kStart_00743274[] = "start%s";
const char kLoop_007432cc[] = "loop%s";
const char kDefault_0073df6c[8] = {'d', 'e', 'f', 'a', 'u', 'l', 't', '\0'};   // 8-byte .rdata copy (0048233B..00482351)
inline void do_event(void* gob, const char* event, void* data) { gob_table(gob)->do_event(as_gob(gob), nullptr, event, as_word(data)); }
}

extern "C" {
// ======================================================================= lookup
// 004822A0 FindAnimation(MaxTree* model, const char* name) cdecl. The second "default" search and its supermodel
// recursion are inlined by the original (00482300..00482628); the supermodel test after the "default" search is
// reached only with a NULL supermodel (it is kept as in the original control flow).
void* __cdecl find_animation_004822a0(void* model, const char* name) {
    if (name == nullptr) return nullptr;                                     // 004822AA..004822B1
    for (I i = 0; i < field<I>(model, 0x5C); ++i) {                          // 004822BA..004822E4
        void* animation = list_at(static_cast<PointerList*>(at(model, 0x58)), i);   // 004822C3..004822C6
        if (kotor_stricmp_0070acaf(animation_name(animation), name) == 0) return animation;   // 004822C9..004822D8 / 004825AF
    }
    if (field<void*>(model, 0x64) != nullptr) return find_animation_004822a0(field<void*>(model, 0x64), name);   // 004822E6..00482339 (inlined level)
    char fallback[8];
    std::memcpy(fallback, kDefault_0073df6c, 8);                             // 004823AA..004823BE [esp+10h]
    for (I i = 0; i < field<I>(model, 0x5C); ++i) {                          // 004823A7..004823E9
        void* animation = list_at(static_cast<PointerList*>(at(model, 0x58)), i);
        if (kotor_stricmp_0070acaf(animation_name(animation), fallback) == 0) return animation;   // 004823C4..004823DD / 0048247F
    }
    if (field<void*>(model, 0x64) != nullptr) return find_animation_004822a0(field<void*>(model, 0x64), fallback);   // 004823EB..004823F0 (never true here)
    return nullptr;                                                          // 00482628
}

// 00484CA0 CurrentAnimation(Gob*) cdecl: first AnimRun whose flags have neither 0x08 nor 0x20.
void* __cdecl current_animation_00484ca0(void* gob) {
    const I count = field<I>(gob, 0xB4);                                     // 00484CA4
    for (I i = 0; i < count; ++i) {                                          // 00484CAB..00484CCE
        void* run = list_at(runs_of(gob), i);                                // 00484CB2..00484CC0
        if ((field<U8>(run, 0x08) & 0x28u) == 0) return run;                 // 00484CC2..00484CC6 / 00484CD5
    }
    return nullptr;                                                          // 00484CD2
}

// 00484CE0 (unnamed) cdecl(MaxTree* model, Animation* animation) -> ST0: product of the supermodel animation scales
// (+84) walked until the model whose animation list holds `animation`.
double __cdecl animation_scale_00484ce0(void* model, void* animation) {
    double scale = kOne_0073d764;                                            // 00484CE0 fld 1.0
    for (;;) {
        I hits = 0;                                                          // 00484CF3
        const I count = field<I>(model, 0x5C);                               // 00484CF0
        for (I i = 0; i < count; ++i)                                        // 00484CF5..00484D09
            if (list_at(static_cast<PointerList*>(at(model, 0x58)), i) == animation) ++hits;   // 00484D00..00484D04
        if (count > 0 && hits != 0) return scale;                            // 00484D0B..00484D0D
        scale = scale * field<float>(model, 0x84);                           // 00484D0F
        model = field<void*>(model, 0x64);                                   // 00484D15
    }
}

// 00481890 AnimRun::AnimRun(Animation*), RET 4.
void* __fastcall anim_run_construct_00481890(void* run, void*, void* animation) {
    field<void*>(run, 0x00) = animation;                                     // 0048189B
    field<U>(run, 0x04) = 0x3F800000u;                                       // 0048189F (speed 1.0f)
    field<U>(run, 0x08) = 0;                                                 // 004818A2
    field<U>(run, 0x10) = 0;                                                 // 004818A5
    field<U>(run, 0x0C) = 0;                                                 // 004818A8
    field<U>(run, 0x14) = 0x3F800000u;                                       // 004818AB
    field<U>(run, 0x18) = 0;                                                 // 004818AE
    field<U>(run, 0x20) = 0;                                                 // 004818B1
    field<U>(run, 0x1C) = 0x3F800000u;                                       // 004818B4
    return run;                                                              // 00481890 mov eax,ecx
}

// 004850A0 Animation::~Animation, RET 0. Event-key list +78/+7C/+80 (CExoArrayList<AurKey<char[32]>>), then MaxTree dtor.
// SEH states 1/0/-1 (handler 007174C3) cover the member list during the base destructor; no callee here throws.
void __fastcall animation_destruct_004850a0(aurora_model::MaxTree* self, void*) {
    void* animation = self;
    if (field<U8>(animation, 0x4C) != 0) {                                   // 004850BE..004850CD
        const U keys = field<U>(animation, 0x7C);                            // 004850CF
        field<U8>(animation, 0x4C) = 0;                                      // 004850D2
        if (keys != 0) {                                                     // 004850D5
            if (field<void*>(animation, 0x78) != nullptr) {                  // 004850D7..004850DC
                live_free(field<void*>(animation, 0x78));                    // 004850DE..004850DF (006FA390)
                field<void*>(animation, 0x78) = nullptr;                     // 004850E7
            }
            field<U>(animation, 0x80) = 0;                                   // 004850EA
            field<U>(animation, 0x7C) = 0;                                   // 004850F0
        }
    }
    if (field<void*>(animation, 0x78) != nullptr) live_free(field<void*>(animation, 0x78));   // 004850F3..004850FF
    maxtree_destruct_0043f730(self, nullptr);                                // 00485107..00485111
}

// ======================================================================= events
// 004826B0 AnimateEvents(Gob* gob, Animation* animation, float t0, float t1, U flags) cdecl. Fires the animation's event
// keys (+78 rows of 0x24 bytes: float time, char name[32] at +4) crossed between t0 and t1 through Gob vtbl+16C.
// flags bit0 = reverse, bit1 = whole range. Wrapped ranges recurse with +-1e9 sentinels; the compiled body turns the
// trailing calls into a loop (004826C0) and inlines the first-level range scans - the recursion below is that source.
void __cdecl animate_events_004826b0(void* gob, void* animation, float t0, float t1, U flags) {
    for (;;) {
        if ((flags & 2u) != 0) {                                             // 004826C0
            flags &= ~2u;                                                    // 004826C5
            if ((flags & 1u) != 0) {                                         // 004826E2
                animate_events_004826b0(gob, animation, kMinusBig_00743248, kMinusBig_00743248, flags);   // 004826EB..004826F8
                t1 = kBig_00743244; t0 = kBig_00743244;                      // 00482700..00482708
                continue;                                                    // 00482710
            }
            t0 = kMinusBig_00743248; t1 = kBig_00743244;                     // inlined as the ascending scan 00482910..00482964
            continue;
        }
        const I count = field<I>(animation, 0x7C);
        if ((flags & 1u) != 0) {                                             // 00482712 (reverse)
            if (t0 < t1) {                                                   // 00482715..00482728
                animate_events_004826b0(gob, animation, t0, kMinusBig_00743248, flags);   // 0048272E..0048274C / 00482768..004827BE (inlined)
                t0 = kBig_00743244;                                          // tail: AnimateEvents(gob, animation, 1e9, t1)
                continue;                                                    // 004827C0..004827F2
            }
            for (I k = count - 1; k >= 0; --k) {                             // 00482AC3..00482B15 (descending)
                const unsigned char* key = static_cast<const unsigned char*>(field<void*>(animation, 0x78)) + 0x24 * k;
                const float time = *reinterpret_cast<const float*>(key);
                if (time >= t1 && time < t0)                                 // test ah,1 / test ah,5
                    do_event(gob, reinterpret_cast<const char*>(key + 4), nullptr);   // 00482B01..00482B0B
            }
            return;
        }
        if (t0 > t1) {                                                       // 004827F7..004827FA (forward wrap)
            animate_events_004826b0(gob, animation, t0, kBig_00743244, flags);   // 00482800..0048281E / 0048283A..00482893 (inlined)
            t0 = kMinusBig_00743248;                                         // tail: AnimateEvents(gob, animation, -1e9, t1)
            continue;                                                        // 00482895..004828C7
        }
        for (I k = 0; k < field<I>(animation, 0x7C); ++k) {                  // 00482B1C..00482B61 (ascending, count reloaded)
            const unsigned char* key = static_cast<const unsigned char*>(field<void*>(animation, 0x78)) + 0x24 * k;
            const float time = *reinterpret_cast<const float*>(key);
            if (time <= t1 && time > t0)                                     // test ah,41h jp / test ah,41h jne
                do_event(gob, reinterpret_cast<const char*>(key + 4), nullptr);   // 00482B48..00482B52
        }
        return;
    }
}

// ======================================================================= hierarchy walkers
// 00483D80 AnimateHierarchy(Part* part, MdlNode* node, float t0, float t1, float weight, bool force) cdecl. Recursive;
// the original inlines two further levels (00483EA4..004841FD) of this same body and recurses at the third.
void __cdecl animate_hierarchy_00483d80(void* part, void* node, float t0, float t1, float weight, bool force) {
    const I depth = anim_depth_008302d4 + 1;                                 // 00483D80..00483D8E
    anim_depth_008302d4 = depth;                                             // 00483D91
    if (depth > anim_max_seen_depth_008302d8) anim_max_seen_depth_008302d8 = depth;   // 00483D8F..00483D98
    if (!force && part != nullptr) {                                         // 00483D9D..00483DAE
        void* part_node = field<void*>(part, 0x04);                          // 00483DB0
        void* model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(part_node, 0x08), nullptr);   // 00483DB3..00483DB6
        if (field<void*>(part, 0x04) == field<void*>(model, 0xA8)) force = true;   // 00483DBB..00483DC6
    }
    if (field<U>(part, 0x48) == 0 && force)                                  // 00483DCB..00483DD8
        aurora_node_control_dispatch_0043f890(static_cast<aurora_model::MdlNode*>(node), nullptr,
            static_cast<aurora_model::Part*>(part), float_to_bits(t0), float_to_bits(t1), float_to_bits(weight));   // 00483DDA..00483DEE
    void* gob = field<void*>(part, 0x40);                                    // 00483DF3
    I limit = 100;                                                           // 00483DF8
    if (gob != nullptr && anim_lod_enabled_00798b2c != 0 && !gob_is_in_cutscene_0043e8a0(gob, nullptr) &&
        field<U8>(gob, 0x16) != 0) {                                         // 00483DF6..00483E18
        const float radius = field<float>(gob, 0x138);
        const double clamped = (radius < kHalf_0073e9ac) ? static_cast<double>(kHalf_0073e9ac) : static_cast<double>(radius);   // 00483E1A..00483E35
        const double size = clamped * kLodRadius_00743258;                   // 00483E3B
        const float distance = field<float>(gob, 0x2C);
        if (static_cast<double>(anim_lod_far_00798b20) * size < distance) {  // 00483E41..00483E51
            limit = anim_lod_far_depth_00798b24;                             // 00483E53
        } else if (distance > static_cast<double>(anim_lod_near_00798b1c) * size) {   // 00483E5D..00483E71
            limit = anim_lod_near_depth_00798b28;                            // 00483E73
        }
    }
    if (anim_depth_008302d4 < anim_max_depth_00798b18 && anim_depth_008302d4 < limit) {   // 00483E79..00483E8C
        for (I i = 0; i < field<I>(part, 0x34); ++i) {                       // 00483E92..00484238 (count reloaded)
            void* child = list_at(static_cast<PointerList*>(at(part, 0x30)), i);   // 00483EA4..00483EA7
            const I id = field<I>(child, 0x28);                              // 00483EAA
            if (id == -1) continue;                                          // 00483EAD..00483EB4
            const I n = field<I>(node, 0x30);                                // 00483EBE
            for (I j = 0; j < n; ++j) {                                      // 00483EC1..00483EE5
                void* candidate = list_at(static_cast<PointerList*>(at(node, 0x2C)), (j + i) % n);   // 00483ECD..00483ED6 (cdq/idiv)
                if (id == static_cast<I>(field<I16>(candidate, 0x02))) {     // 00483EDA..00483EE0 movsx
                    animate_hierarchy_00483d80(child, candidate, t0, t1, weight, force);   // inlined levels / 004841E9
                    break;
                }
            }
        }
    }
    anim_depth_008302d4 = anim_depth_008302d4 - 1;                           // 0048423E..00484247
}

// 00483630 ProceduralAnimateHierarchy(Part*) cdecl: scrolling / jittered UV animation of trimesh parts whose node has
// +138 set, written to the part's UV pool (+78) and the model's MDX pool (+AC); recursive over children (the original
// inlines two levels, 00483867..00483D71).
void __cdecl procedural_animate_hierarchy_00483630(void* part) {
    void* trimesh = as_object(part_table(part)->s02c(part, nullptr));        // 00483639..00483640
    if (trimesh != nullptr && field<I>(trimesh, 0x78) != -1) {               // 00483643..00483659
        void* node = aurora_node_as_trimesh_0043e400(field<void*>(trimesh, 0x04), nullptr);   // 0048365F..00483662
        if (field<U>(node, 0x138) != 0 && field<I>(node, 0x160) != -1 && field<I>(trimesh, 0x78) != -1) {   // 00483669..00483689
            const U uv_pool = field<U>(trimesh, 0x78);                       // 00483683
            const I modulus = ftol2(static_cast<double>(field<float>(node, 0x144)) * kThousand_0073d6fc);   // 0048368F..0048369B
            const float half = f32(static_cast<double>(field<float>(node, 0x144)) * kHalf_0073e9ac);       // 004836A0..004836B3 [esp+10h]
            float* jitter = static_cast<float*>(aurora_pool_lock_00423f10(uv_pool, 0, 0, 0x10));          // 004836AC..004836BB
            for (I i = 0; i < static_cast<I>(field<I16>(node, 0x180)); ++i) {                              // 004836C7..004837E0
                float* uv = reinterpret_cast<float*>(field<unsigned char*>(trimesh, 0x90) + 8 * i);         // 004836E0..004836FA
                uv[0] = f32(static_cast<double>(field<float>(node, 0x13C)) + uv[0]);                        // 004836E6..004836FE
                uv[1] = f32(static_cast<double>(uv[1]) + field<float>(node, 0x140));                        // 00483700..00483709
                if (modulus == 0) continue;                                                                 // 004836F6..0048370C
                for (int axis = 0; axis < 2; ++axis) {                                                      // x: 00483712..00483768, y: 0048376B..004837C6
                    const I r = crt_rand_006fc447() % modulus;                                                      // _rand 006FC447; cdq; idiv
                    const double moved = ((static_cast<double>(r) * kMilli_0073d708 - half) * field<float>(node, 0x148))
                                         + jitter[2 * i + axis];                                            // fild; fmul; fsub; fmul; fadd
                    jitter[2 * i + axis] = f32(moved);                                                      // fst [ebx+ebp(+4)]
                    const double offset = moved - uv[axis];                                                 // fsub [esi(+4)] (wide)
                    if (offset > half) jitter[2 * i + axis] = f32(static_cast<double>(half) + uv[axis]);    // fcom; test ah,41h; jne / fld; fadd; fstp
                    else if (offset < -static_cast<double>(half))
                        jitter[2 * i + axis] = f32(static_cast<double>(uv[axis]) - half);                   // fchs; fxch; fcompp; test ah,5; jp / fsub
                }
            }
            void* model = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr);   // 004837E6..004837E9
            unsigned char* mdx = static_cast<unsigned char*>(aurora_pool_lock_00423f10(field<U>(model, 0xAC), 0, 0, 1));   // 004837EE..004837FB
            for (I i = 0; i < static_cast<I>(field<I16>(node, 0x180)); ++i) {                              // 00483805..00483841
                unsigned char* dst = mdx + (field<U>(node, 0x14C) * static_cast<U>(i) + field<U>(node, 0x194) + field<U>(node, 0x160));   // 00483810..00483821
                std::memcpy(dst, jitter + 2 * i, 4);                                                        // 00483827..0048382A
                std::memcpy(dst + 4, jitter + 2 * i + 1, 4);                                                // 0048382D..00483830
            }
            void* model2 = aurora_tree_as_model_0043e1c0(field<aurora_model::MaxTree*>(node, 0x08), nullptr);   // 00483843..00483846
            aurora_pool_unlock_00423f30(field<U>(model2, 0xAC));                                           // 0048384B..00483852
            aurora_pool_unlock_00423f30(field<U>(trimesh, 0x78));                                          // 00483857..0048385B
        }
    }
    for (I i = 0; i < field<I>(part, 0x34); ++i)                              // 00483867..00483D71 (count reloaded)
        procedural_animate_hierarchy_00483630(list_at(static_cast<PointerList*>(at(part, 0x30)), i));   // 00483D34..00483D3B
}

// ======================================================================= Gob::PlayAnimation
// 00485BD0 Gob::PlayAnimation(const char* name, float speed, U flags, float start), Gob vtbl+18, RET 10h, EAX 1/0.
U __fastcall gob_play_animation_00485bd0(aurora_model::Gob* self, void* unused_edx, U raw_name, U raw_speed, U flags, U raw_start) {
    (void)unused_edx;
    void* gob = self;
    const char* name = static_cast<const char*>(as_object(raw_name));      // [esp+15Ch] (raw-word slot contract)
    PointerList* runs = runs_of(gob);
    char done[0x40];                                                         // [esp+18h]
    char event[0x100];                                                       // [esp+58h]
    if (name == nullptr || name[0] == 0 || kotor_stricmp_0070acaf(kNull_0073ee04, name) == 0) {   // 00485BE2..00485C06
        for (I n = runs->size; n != 0;) {                                    // 00486023..00486076
            --n;                                                             // 00486036
            void* look_at = field<void*>(gob, 0x174);                        // 00486030
            if (look_at != nullptr && field<void*>(look_at, 0x0C) == list_at(runs, n)) {   // 00486037..00486047
                field<void*>(look_at, 0x0C) = nullptr;                       // 00486049
                void* again = field<void*>(gob, 0x174);                      // 0048604C
                if (again != nullptr) slots<LookAtSlots>(again)->scalar_delete(again, nullptr, 1);   // 00486052..0048605A
                field<void*>(gob, 0x174) = nullptr;                          // 0048605C
            }
            live_free(list_at(runs, n));                                     // 00486062..0048606C
        }
        runs->size = 0;                                                      // 00486078
        return 1;                                                            // 00486081
    }
    void* run = nullptr;                                                     // edi / [esp+10h]
    void* found = nullptr;                                                   // [esp+10h]
    bool search_current = true;
    if ((flags & 0x40u) == 0) {                                              // 00485C0C..00485C1B
        for (I i = runs->size - 1; i >= 0; --i) {                            // 00485C1D..00485C4D
            void* candidate = list_at(runs, i);
            if (kotor_stricmp_0070acaf(animation_name(run_animation(candidate)), name) == 0) {   // 00485C30..00485C4A
                found = list_at(runs, i);                                    // 00485C51..00485C64
                if ((flags & 0x400u) != 0) {                                 // 00485C5A..00485C68
                    field<U>(found, 0x08) = field<U>(found, 0x08) | 0x401u;  // 00485C6A..00485C74
                    return 1;
                }
                if (found != nullptr) {                                      // 00485C87..00485C89
                    kotor_fastcall_00445080(runs, nullptr, found);           // 00485CB6..00485CC3 CExoArrayList::Remove
                    run = found;                                             // 00485CC8
                    search_current = false;
                }
                break;
            }
        }
    }
    if (search_current && kotor_stricmp_0070acaf(name, kCurrent_00743288) == 0) {   // 00485C91..00485CA1
        found = current_animation_00484ca0(gob);                             // 00485CA3..00485CAE
        if (found != nullptr) kotor_fastcall_00445080(runs, nullptr, found); // 00485CB8..00485CC3
        run = found;                                                         // 00485CC8
    }
    if ((flags & 0x80u) != 0) {                                              // 00485CCC..00485CD5 (stop)
        if (run == nullptr) return 0;                                        // 00485CD7..00485CD9 -> 00485DE7
        std::sprintf(done, kDone_00743280, animation_name(run_animation(run)));   // 00485CDF..00485CEF (sprintf 006FADB0)
        do_event(gob, done, nullptr);                                        // 00485CF4..00485D01
        void* look_at = field<void*>(gob, 0x174);                            // 00485D07
        if (look_at != nullptr && field<void*>(look_at, 0x0C) == run) {      // 00485D0D..00485D14
            field<void*>(look_at, 0x0C) = nullptr;                           // 00485D16
            void* again = field<void*>(gob, 0x174);                          // 00485D19
            if (again != nullptr) slots<LookAtSlots>(again)->scalar_delete(again, nullptr, 1);   // 00485D1F..00485D27
            field<void*>(gob, 0x174) = nullptr;                              // 00485D29
        }
        live_free(run);                                                      // 00485D2F..00485D30
        return 1;
    }
    void* animation = nullptr;                                               // ebp
    void* source = nullptr;                                                  // [esp+14h]
    if (field<void*>(gob, 0x64) != nullptr) {                                // 00485D4A..00485D4F
        animation = find_animation_004822a0(field<void*>(gob, 0x64), name);  // 00485D51..00485D58
        source = field<void*>(gob, 0x64);                                    // 00485D5A..00485D62
    }
    if (animation == nullptr) {                                              // 00485D60..00485D66
        animation = find_animation_004822a0(field<void*>(gob, 0x58), name);  // 00485D68..00485D78
        source = field<void*>(gob, 0x58);                                    // 00485D72..00485D7A
    }
    float start = bits_to_float(raw_start);                                  // [esp+168h]
    if (start < kZero_0073d700) put_raw(&start, 0u);                         // 00485D7E..00485D92
    if (animation != nullptr) {                                              // 00485D9D
        const float length = field<float>(animation, 0x50);                  // 00485DA1
        if (start > length) {                                                // 00485DA4..00485DB2 (test ah,41h; jne)
            double wrapped;
            do {
                wrapped = static_cast<double>(start) - length;               // 00485DC0..00485DC7
                start = f32(wrapped);                                        // 00485DC9 fst
            } while (wrapped > length);                                      // 00485DD0..00485DD7 (compares the unrounded value)
        }
    }
    if (run == nullptr) {                                                    // 00485DDB..00485DDD
        if (animation == nullptr) return 0;                                  // 00485DE3..00485DF3
        void* storage = live_operator_new(0x24);                             // 00485DF6..00485DF8 (006FA7E6)
        void* created = (storage != nullptr) ? anim_run_construct_00481890(storage, nullptr, animation) : nullptr;   // 00485E02..00485E12
        found = created;                                                     // 00485E1B [esp+10h]
        const float speed = bits_to_float(raw_speed);
        if (speed == kZero_0073d700) field<U>(created, 0x10) = 0;            // 00485E14..00485E2C (test ah,44h; jp)
        if (speed < kZero_0073d700)                                          // 00485E2F..00485E41
            field<float>(created, 0x10) = f32(static_cast<double>(field<float>(animation, 0x50)) + kEpsilon_0074327c);   // 00485E43..00485E4C
        if (speed > kZero_0073d700) field<U>(created, 0x10) = kMinusEpsilonBits;   // 00485E4F..00485E63
        if (!(start == kZero_0073d700)) put_raw(at(created, 0x10), raw_of(&start));   // 00485E6A..00485E85 (test ah,44h; jnp)
        if ((flags & 2u) == 0) {                                             // 00485E88..00485E92
            field<U>(created, 0x14) = 0;                                     // 00485E96
            field<U>(created, 0x18) = field<U>(animation, 0x54);             // 00485E94..00485E9C
        }
        run = found;                                                         // 00485E9F
    }
    field<float>(run, 0x1C) = f32(animation_scale_00484ce0(source, animation));   // 00485EAC..00485EB7 (fstp [edi+1Ch])
    field<U>(run, 0x20) = 0;                                                 // 00485ECB
    field<U>(run, 0x04) = raw_speed;                                         // 00485EBA / 00485ECE
    field<U>(run, 0x08) = flags;                                             // 00485ED1
    const U keep_order = flags & 8u;                                         // 00485EC3..00485ED4 [esp+14h]
    I position = 0;                                                          // ebp
    if (keep_order != 0) {                                                   // 00485EDE
        while (position < runs->size && (field<U>(list_at(runs, position), 0x08) & 0x200u) != 0) ++position;   // 00485EE0..00485F03
    } else {
        while (position < runs->size) {                                      // 00485F07..00485F2F
            const U other = field<U>(list_at(runs, position), 0x08);         // 00485F11..00485F13
            if ((other & 8u) == 0) {                                         // 00485F16
                if ((flags & 0x20u) != 0) break;                             // 00485F1A..00485F1D
                if ((other & 0x20u) == 0) break;                             // 00485F1F..00485F21
            }
            ++position;                                                      // 00485F29
        }
    }
    aurora_list_insert_00445020(runs, nullptr, ptr_word(run), position);     // 00485F31..00485F3B
    if ((flags & 0x10u) != 0 && runs->size > position + 1) {                 // 00485F40..00485F56
        field<U>(run, 0x10) = field<U>(list_at(runs, position + 1), 0x10);   // 00485F58..00485F61
        return 1;
    }
    if (keep_order == 0 && (flags & 0x20u) == 0) {                           // 00485F76..00485F84
        while (runs->size > position + 1) {                                  // 00485F8A..00485FE2
            void* later = list_at(runs, position + 1);                       // 00485F97..00485F99
            std::sprintf(event, kDone_00743280, animation_name(run_animation(later)));   // 00485F9D..00485FAD
            kotor_fastcall_00445080(runs, nullptr, later);                   // 00485FB5..00485FB8
            do_event(gob, event, nullptr);                                   // 00485FBD..00485FC8
            live_free(later);                                                // 00485FCE..00485FCF
        }
        run = found;                                                         // 00485FE4 mov edi,[esp+10h]
        std::sprintf(event, kStart_00743274, animation_name(run_animation(run)));   // 00485FE8..00485FF8
        do_event(gob, event, nullptr);                                       // 00485FFD..0048600B
    }
    return 1;
}

// ======================================================================= Gob::Animate
// 00486670 Gob::Animate(float dt), Gob vtbl+14, RET 4. The local `safe` is the SafePointer<Gob> the original registers
// in Gob+4 (0048679E..004867E4) so that a Gob destroyed by an event callback nulls it; every callback is followed by
// the original's re-test of [esp+14h]. SEH state 0 (handler 0071756B) would unregister it on unwind; no callee on the
// reachable paths throws (D30-FINAL unwind note), so the cleanup is emitted only on the normal paths.
// Raw-word slot contract (vtables_gen.h Gob vtbl+14): the original leaves EAX unspecified on return; D2-label: returns 0,
// no consumer of the slot reads it.
U __fastcall gob_animate_00486670(aurora_model::Gob* self, void* unused_edx, U raw_dt) {
    (void)unused_edx;
    void* gob = self;
    gob_process_00444840(gob, nullptr);                                      // 0048668F..00486691
    if (anim_enabled_00798b40 == 0) return 0;                                  // 00486696..0048669F
    const float dt = bits_to_float(raw_dt);
    void* scene = field<void*>(gob, 0x68);                                   // 004866A5
    if (scene != nullptr) {                                                  // 004866A8..004866AA
        if (field<U>(gob, 0x18C) == field<U>(scene, 0x50)) return 0;           // 004866AC..004866B7
        field<U>(gob, 0x18C) = field<U>(scene, 0x50);                        // 004866C1
    }
    if (field<U8>(gob, 0x156) != 0 && scene != nullptr && field<void*>(scene, 0x84) != nullptr) {   // 004866C7..004866E3
        const U strength = (field<float>(gob, 0x158) == kZero_0073d700) ? kWindDefaultBits : field<U>(gob, 0x158);   // 004866E9..0048670C [esp+20h]
        U radius;                                                            // [esp+1Ch]
        if (field<float>(gob, 0x160) == kZero_0073d700) {                    // 00486710..00486721
            Vector delta;
            kotor_cdecl_00423bc0(&delta.x, &field<float>(gob, 0x78), &field<float>(gob, 0x94));   // 00486723..00486733
            if (vector_magnitude_004ab100(&delta) < static_cast<double>(kOne_0073d764)) {         // 00486738..0048674C
                Vector again;
                kotor_cdecl_00423bc0(&again.x, &field<float>(gob, 0x78), &field<float>(gob, 0x94));   // 0048674E..00486755
                radius = float_to_bits(f32(vector_magnitude_004ab100(&again)));                  // 0048675A..00486760
            } else {
                radius = 0x3F800000u;                                        // 00486769
            }
        } else {
            radius = field<U>(gob, 0x160);                                   // 00486773..00486779
        }
        const Vector position = field<Vector>(gob, 0x78);                    // 0048678E..004867A3 (by-value copy)
        wind_manager_add_point_source_wind_004a6510(field<void*>(scene, 0x84), nullptr, position, strength, radius,
                                                   field<U>(gob, 0x15C));     // 0048677D..004867AF
    }
    void* volatile safe = gob;                                               // 004867B7 [esp+14h]
    PointerList* safe_list = static_cast<PointerList*>(at(gob, 0x04));      // 004867B4
    if (safe_list->size == safe_list->capacity) {                            // 004867BB..004867C1
        const U grow = (safe_list->capacity != 0) ? static_cast<U>(safe_list->capacity) * 2u : 8u;   // 004867C3..004867CB
        menu_list_growth::allocate_005e03c0(safe_list, nullptr, grow);       // 004867D0..004867D3
    }
    reinterpret_cast<U*>(word_ptr(safe_list->data))[safe_list->size] = ptr_word(const_cast<void**>(&safe));   // 004867D8..004867E1
    safe_list->size = safe_list->size + 1;                                   // 004867E4
    auto unregister = [&]() {                                                // 00486FED..00486FFD
        void* owner = safe;
        if (owner != nullptr) kotor_fastcall_00445080(static_cast<PointerList*>(at(owner, 0x04)), nullptr,
                                                       const_cast<void**>(&safe));
    };
    void* behavior = field<void*>(gob, 0x188);                               // 004867E7
    if (behavior != nullptr) {                                               // 004867ED..004867F6
        slots<BehaviorSlots>(behavior)->slot08(behavior, nullptr, raw_dt);   // 004867F8..00486802
        if (safe == nullptr) return 0;                                         // 00486805..00486809
    }
    if (dt <= kZero_0073d700) {                                              // 0048680F..00486821 (test ah,41h; jp: > or NaN continue)
        void* model = field<void*>(gob, 0x58);                               // 00486823
        if (model == nullptr || ((field<U8>(model, 0x50) >> 4) & 1u) == 0) { unregister(); return 0; }   // 00486826..00486836
    }
    scene = field<void*>(gob, 0x68);                                         // 00486841 (esi)
    if (scene != nullptr && anim_focus_override_008302e4 == 0) {             // 00486844..0048684E
        const float dx = f32(aurora_fabs_00423b70(f32(static_cast<double>(field<float>(gob, 0x78)) - field<float>(scene, 0xA8))));   // 00486850..00486862
        const double dy = aurora_fabs_00423b70(f32(static_cast<double>(field<float>(gob, 0x7C)) - field<float>(scene, 0xAC)));       // 00486866..00486872
        field<float>(gob, 0x2C) = f32((dx > dy) ? static_cast<double>(dx) : dy);   // 00486877..0048688D (test ah,41h; jne keeps dy)
    } else {
        field<U>(gob, 0x2C) = 0;                                             // 00486892
    }
    bool hierarchy = true;                                                   // 00486897 [esp+13h]
    if (scene != nullptr && scene_table(scene)->s0a8_scene_is_animating(scene, nullptr) == 0) {   // 00486895..004868AA
        if (gob_table(gob)->s170_gob_is_force_animated(as_gob(gob), nullptr) == 0 && gob_model_flag4_00481d80(gob, nullptr) == 0) {   // 004868AC..004868C3
            unregister(); return 0;                                            // 004868C5..004868CB
        }
        void* model = field<void*>(gob, 0x58);                               // 004868D0
        if (model != nullptr && ((field<U8>(model, 0x50) >> 4) & 1u) != 0) { // 004868D3..004868E1
            if (gob_table(gob)->s170_gob_is_force_animated(as_gob(gob), nullptr) == 0) hierarchy = false;   // 004868E3..004868F1
        }
    }
    if (field<void*>(gob, 0x6C) != nullptr && field<U8>(gob, 0x184) == 1)    // 004868F5..00486903
        procedural_animate_hierarchy_00483630(field<void*>(gob, 0x6C));      // 00486905..00486906
    void* look_at = field<void*>(gob, 0x174);                                // 0048690E
    if (look_at != nullptr) {                                                // 00486914..00486916
        if (slots<LookAtSlots>(look_at)->query04(look_at, nullptr) != 0) {   // 0048691C..00486923
            void* again = field<void*>(gob, 0x174);                          // 00486925
            slots<LookAtSlots>(again)->slot0c(again, nullptr);               // 0048692B..0048692D
        } else {
            void* target = field<void*>(field<void*>(gob, 0x174), 0x08);     // 0048693A..00486940
            for (I n = field<I>(gob, 0xB4); n != 0;) {                       // 00486932..00486968
                void* candidate = list_at(runs_of(gob), n - 1);              // 00486950..00486953
                --n;                                                         // 00486959
                if (run_animation(candidate) == target) deleteAt_006857a0(runs_of(gob), nullptr, n);   // 00486957..00486961
            }
            void* holder = field<void*>(gob, 0x174);                         // 0048696A
            if (field<void*>(holder, 0x0C) != nullptr) {                     // 00486970..00486975
                live_free(field<void*>(holder, 0x0C));                       // 00486977..00486978
                field<void*>(field<void*>(gob, 0x174), 0x0C) = nullptr;      // 0048697D..00486986
            }
            void* deleting = field<void*>(gob, 0x174);                       // 0048698D
            if (deleting != nullptr) slots<LookAtSlots>(deleting)->scalar_delete(deleting, nullptr, 1);   // 00486993..0048699B
            field<void*>(gob, 0x174) = nullptr;                              // 0048699D
        }
    }
    void* model = field<void*>(gob, 0x58);                                   // 004869A7
    if (model != nullptr && !gob_is_in_cutscene_0043e8a0(gob, nullptr) && field<U8>(gob, 0x17) == 0 &&
        field<U>(gob, 0xBC) == 0 && (field<U8>(model, 0x50) & 1u) == 0 && ((field<U8>(model, 0x50) >> 6) & 1u) == 0 &&
        gob_table(gob)->s190_gob_is_always_rendering(as_gob(gob), nullptr) == 0 && ((field<U8>(field<void*>(gob, 0x58), 0x50) >> 3) & 1u) == 0 &&
        gob_table(gob)->s180_gob_is_in_view_volume(as_gob(gob), nullptr) == 0 &&
        ((field<U8>(field<void*>(gob, 0x58), 0x50) >> 4) & 1u) == 0) {      // 004869AA..00486A1C
        if (gob_table(gob)->s180_gob_is_in_view_volume(as_gob(gob), nullptr) != 0) { unregister(); return 0; }   // 00486A1E..00486A2A
        if (gob_model_flag2_00481d60(gob, nullptr) == 0) { unregister(); return 0; }        // 00486A30..00486A39
        hierarchy = false;                                                   // 00486A3F
    }
    PointerList* runs = runs_of(gob);
    I index = runs->size;                                                    // 00486A44 (ebp)
    field<U8>(gob, 0x17) = 0;                                                // 00486A4C
    field<U8>(gob, 0x17E) = (index > 0) ? 1 : 0;                             // 00486A50..00486A5B
    char start_a[0x40], start_c[0x40], loop_b[0x40], loop_d[0x40];          // [esp+40h] / [esp+0C0h] / [esp+80h] / [esp+100h]
    if (index != 0) {                                                        // 00486A62..00486A64
        void* last = list_at(runs, index - 1);                               // 00486A66..00486A6C
        if ((field<U8>(last, 0x08) & 0x20u) != 0) {                          // 00486A74..00486A78
            field<U>(last, 0x08) = field<U>(last, 0x08) + 0xFFFFFFE0u;       // 00486A7C
            std::sprintf(start_a, kStart_00743274, animation_name(run_animation(list_at(runs, index - 1))));   // 00486A80..00486A9A
            do_event(gob, start_a, nullptr);                                 // 00486A9F..00486AAD
        }
    }
    if (safe == nullptr) return 0;                                             // 00486AB3..00486AB9
    I following = index + 1;                                                 // 00486ABF..00486AC2 [esp+1Ch]
    for (;;) {
        const I previous_index = index;                                      // 00486ACA
        --index; --following;                                                // 00486ACC..00486ACD ([esp+24h] / [esp+1Ch])
        if (previous_index == 0) break;                                      // 00486ACE..00486AD8 -> 00486F84
        void* run = list_at(runs, index);                                    // 00486ADE..00486AE4
        const U flags = field<U>(run, 0x08);                                 // 00486AE7
        bool clamped = false;                                                // 00486AF3 [esp+1Bh]
        if ((flags & 0x20u) == 0) {                                          // 00486AEA..00486AF8
            void* animation = run_animation(run);
            const double advanced = static_cast<double>(dt) * field<float>(run, 0x04);   // 00486B01..00486B08
            field<U>(run, 0x0C) = field<U>(run, 0x10);                       // 00486B0B..00486B0E
            const double t = advanced + field<float>(run, 0x10);             // 00486B11
            field<float>(run, 0x10) = f32(t);                                // 00486B14 fst
            bool removed = false;
            if ((flags & 1u) != 0) {                                         // 00486AFE / 00486B17
                if (field<U>(run, 0x20) != 0) {                              // 00486B1D..00486B22
                    std::sprintf(start_a, kDone_00743280, animation_name(animation));   // 00486B24..00486B36
                    kotor_fastcall_00445080(runs, nullptr, run);             // 00486B3E..00486B41
                    do_event(gob, start_a, nullptr);                         // 00486B46..00486B51
                    if ((field<U>(run, 0x08) & 0x200u) != 0) {               // 00486B57..00486B5D
                        live_free(field<void*>(run, 0x28));                  // 00486B5F..00486B63
                        live_free(field<void*>(run, 0x2C));                  // 00486B68..00486B6C
                        field<U>(run, 0x28) = 0; field<U>(run, 0x2C) = 0;    // 00486B74 / 00486B77
                        live_free(run);                                      // 00486B73..00486B7A
                    } else {
                        live_free(run);                                      // 00486B87..00486B88
                    }
                    removed = true;
                } else {
                    U wrapped = 0;                                           // 00486B9B
                    if (t < kZero_0073d700) {                                // 00486B95..00486BA2
                        field<U>(run, 0x10) = 0; wrapped = 1;                // 00486BA6 / 00486BE2
                    } else if ((flags & 0x200u) != 0) {                      // 00486BAB..00486BAE
                        if (t > field<float>(run, 0x24)) {                   // 00486BB0..00486BB8
                            field<U>(run, 0x10) = field<U>(run, 0x24);       // 00486BBA / 00486BC4
                            field<U>(run, 0x18) = 0x3F000000u;               // 00486BBD (0.5f)
                            clamped = true; wrapped = 1;                     // 00486BC7 / 00486BE2
                        }
                    } else if (t > field<float>(animation, 0x50)) {          // 00486BCE..00486BD8
                        field<U>(run, 0x10) = field<U>(animation, 0x50);     // 00486BDA..00486BDF
                        wrapped = 1;                                         // 00486BE2
                    }
                    if ((flags & 0x400u) != 0 || wrapped != 0) {             // 00486BE7..00486BEE
                        if (index > 0) {                                     // 00486BF4..00486BF6
                            void* earlier = list_at(runs, index - 1);        // 00486BF8..00486BFA
                            if ((field<U8>(earlier, 0x08) & 0x20u) != 0) {   // 00486BFE..00486C06
                                field<U>(earlier, 0x08) = field<U>(earlier, 0x08) + 0xFFFFFFE0u;   // 00486C0A
                                std::sprintf(start_c, kStart_00743274, animation_name(run_animation(list_at(runs, index - 1))));   // 00486C0E..00486C27
                                do_event(gob, start_c, nullptr);             // 00486C2C..00486C3D
                            }
                        }
                        const float blend_time = field<float>(run, 0x18);
                        const double step = (blend_time <= kZero_0073d700) ? static_cast<double>(kTwo_00741be0)
                                          : (static_cast<double>(dt) / blend_time) + (static_cast<double>(dt) / blend_time);   // 00486C43..00486C65 (fadd st0,st)
                        const double blend = static_cast<double>(field<float>(run, 0x14)) - step;   // 00486C67 fsubr
                        field<float>(run, 0x14) = f32(blend);                // 00486C6A fst
                        if (blend <= static_cast<double>(kZero_0073d700)) {  // 00486C6D..00486C78
                            field<U>(run, 0x14) = 0;                         // 00486C81
                            field<U>(run, 0x20) = 1;                         // 00486C8E
                            if (field<float>(run, 0x18) == kZero_0073d700) field<U>(run, 0x14) = 0x3F800000u;   // 00486C7E..00486CA0
                        }
                    }
                }
            } else {
                if (t < kZero_0073d700) {                                    // 00486CAC..00486CB7
                    if (field<float>(run, 0x04) <= kZero_0073d700) {         // 00486CB9..00486CC7
                        if ((flags & 4u) != 0) {                             // 00486CC9..00486CCC
                            std::sprintf(loop_b, kLoop_007432cc, animation_name(animation));   // 00486CCE..00486CE1
                            do_event(gob, loop_b, nullptr);                  // 00486CE6..00486CF7
                        }
                        const double back = static_cast<double>(field<float>(run_animation(run), 0x50)) + field<float>(run, 0x10);   // 00486CFD..00486D02
                        field<float>(run, 0x10) = f32(back);                 // 00486D05 fst
                        if (back < kZero_0073d700) field<U>(run, 0x10) = 0;  // 00486D08..00486D15
                    }
                }
                void* anim = run_animation(run);                             // 00486D1C
                if (field<float>(run, 0x10) > field<float>(anim, 0x50)) {    // 00486D1E..00486D29
                    if (field<float>(run, 0x04) >= kZero_0073d700) {         // 00486D2B..00486D39 (test ah,1; jne)
                        if ((field<U8>(run, 0x08) & 4u) != 0) {              // 00486D3B..00486D3F
                            std::sprintf(loop_d, kLoop_007432cc, animation_name(anim));   // 00486D41..00486D52
                            do_event(gob, loop_d, nullptr);                  // 00486D57..00486D68
                        }
                        void* anim2 = run_animation(run);                    // 00486D6E
                        const double forward = static_cast<double>(field<float>(run, 0x10)) - field<float>(anim2, 0x50);   // 00486D70..00486D73
                        field<float>(run, 0x10) = f32(forward);              // 00486D76 fst
                        if (forward > field<float>(anim2, 0x50)) field<U>(run, 0x10) = field<U>(anim2, 0x50);   // 00486D79..00486D86
                    }
                }
            }
            if (!removed) {
                if (field<U>(run, 0x20) == 0 && field<float>(run, 0x14) < kOne_0073d764) {   // 00486D89..00486DA2
                    const float blend_time = field<float>(run, 0x18);
                    const double increment = (blend_time <= kZero_0073d700) ? static_cast<double>(kOne_0073d764)
                                                                            : static_cast<double>(dt) / blend_time;   // 00486DA8..00486DC7
                    const double blend = increment + field<float>(run, 0x14);    // 00486DCA
                    field<float>(run, 0x14) = f32(blend);                    // 00486DCD fst
                    if (blend > kOneDouble_0073ede8) {                       // 00486DD0..00486DDB
                        field<U>(run, 0x14) = 0x3F800000u;                   // 00486DE1
                        if ((field<U8>(run, 0x08) & 8u) == 0) {              // 00486DDD..00486DE8
                            while (runs->size > following) {                 // 00486DEA..00486DF4 / 00486E0E..00486E1D
                                void* later = list_at(runs, index + 1);      // 00486DF6..00486DFC
                                kotor_fastcall_00445080(runs, nullptr, later);   // 00486E00..00486E03
                                live_free(later);                            // 00486E08..00486E09
                            }
                        }
                    }
                }
                const U reverse = (field<float>(run, 0x04) < kZero_0073d700) ? 1u : 0u;   // 00486E1F..00486E36
                aurora_control_weight_00798b38 = field<U>(run, 0x1C);        // 00486E38..00486E3B
                if ((field<U>(run, 0x08) & 0x100u) != 0) {                   // 00486E41..00486E47
                    anim_report_run_008302cc = run;                          // 00486E49
                    anim_report_reverse_008302c8 = reverse;                  // 00486E4F
                    put_raw(const_cast<float*>(&anim_report_length_008302d0), field<U>(run_animation(run), 0x50));   // 00486E55..00486E5A
                }
                void* root = field<void*>(gob, 0x6C);
                void* animation_now = run_animation(run);
                if ((field<U>(run, 0x08) & 0x200u) != 0) {                   // 00486E5F..00486E65
                    const double x = static_cast<double>(field<float>(run, 0x10)) / field<float>(run, 0x24);   // 00486E6B..00486E71
                    const float* keys = field<float*>(run, 0x2C);            // 00486E6E
                    I k = 0;                                                 // 00486E74
                    if (x > keys[0]) {                                       // 00486E76..00486E7D
                        const I limit = field<I>(run, 0x30);                 // 00486E7F (loaded once)
                        while (k < limit) {                                  // 00486E82..00486E84
                            ++k;                                             // 00486E8D
                            if (!(x > keys[k])) break;                       // 00486E89..00486E93
                        }
                    }
                    U fraction;                                              // [esp+20h]
                    if (k == field<I>(run, 0x30)) {                          // 00486E95..00486E98
                        fraction = 0;                                        // 00486E9C
                    } else {
                        // keys[k-1] with k == 0 reads the word before the array, exactly as 00486EA6 does.
                        const float* before = reinterpret_cast<const float*>(word_ptr(ptr_word(keys) + 4u * static_cast<U>(k) - 4u));
                        fraction = float_to_bits(f32((x - *before) / (static_cast<double>(keys[k]) - *before)));   // 00486EA6..00486EB5
                    }
                    if (hierarchy) {                                         // 00486EB9..00486EBF
                        const float* times = field<float*>(run, 0x28);       // 00486ECB
                        if (clamped) {                                       // 00486EC5..00486ED0
                            animate_hierarchy_00483d80(root, field<void*>(animation_now, 0x28), kMinus100_0074186c,
                                times[k], field<float>(run, 0x14), false);   // 00486ECE..00486EE9
                            put_raw(const_cast<float*>(&anim_last_blend_008302e8), field<U>(run, 0x14));   // 00486EEE..00486EF1
                        } else {
                            const float* before = reinterpret_cast<const float*>(word_ptr(ptr_word(times) + 4u * static_cast<U>(k) - 4u));
                            animate_hierarchy_00483d80(root, field<void*>(animation_now, 0x28), *before, times[k],
                                bits_to_float(fraction), false);             // 00486EF9..00486F3D
                        }
                        field<U8>(gob, 0x16) = 1;                            // 00486F45
                    }
                } else if (hierarchy) {                                      // 00486F14..00486F1A
                    const float t0 = f32(static_cast<double>(kMinus100_0074186c) - field<float>(run, 0x0C));   // 00486F1F..00486F35
                    animate_hierarchy_00483d80(root, field<void*>(animation_now, 0x28), t0, field<float>(run, 0x10),
                                               field<float>(run, 0x14), false);   // 00486F1C..00486F3D
                    field<U8>(gob, 0x16) = 1;                                // 00486F45
                }
                animate_events_004826b0(gob, run_animation(run), field<float>(run, 0x0C), field<float>(run, 0x10), reverse);   // 00486F49..00486F56
                anim_report_run_008302cc = nullptr;                          // 00486F64
                anim_report_reverse_008302c8 = 0;                            // 00486F69
                put_raw(const_cast<float*>(&anim_report_length_008302d0), 0u);   // 00486F6E
            }
        }
        if (safe == nullptr) return 0;                                         // 00486F73..00486F7F
    }
    if (safe == nullptr) return 0;                                             // 00486F84..00486F8A
    void* root = field<void*>(gob, 0x6C);                                    // 00486F8C
    if (root != nullptr) {                                                   // 00486F8F..00486F91
        bool animate_parts;
        if (field<void*>(gob, 0x68) == nullptr) animate_parts = gob_model_flag4_00481d80(gob, nullptr) != 0;   // 00486F93..00486FB6
        else if (field<U8>(gob, 0x17E) != 0 || anim_lod_parts_00798b44 == 0) animate_parts = true;           // 00486F9A..00486FAB
        else animate_parts = gob_model_flag4_00481d80(gob, nullptr) != 0;   // 00486FAD..00486FB6
        if (animate_parts) {
            part_table(root)->s068(root, nullptr, raw_dt);                   // 00486FB8..00486FC4
            if (anim_debug_008302e0 != 0) gob_table(gob)->slot184(as_gob(gob), nullptr);   // 00486FC7..00486FD4
            if (anim_slot188_enabled_00798b3c != 0) gob_table(gob)->s188_gob_get_minimum_sphere(as_gob(gob), nullptr);   // 00486FDA..00486FE7
        }
    }
    unregister();                                                            // 00486FED..00486FFD
    return 0;
}
} // extern "C"
