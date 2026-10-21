// m47_anim_lookat.cpp - lane-m47-3d group "anim": the LookAtAnimation class (Gob+174, created by
// Gob::BeginLookAtAnimate 00487190) and its non-provided callees.
//   ctor 00487020, BuildAnimation 00486140, ~LookAtAnimation 00486090, scalar deleting dtor 00487170,
//   IsActive 00481850, Deactivate 00481860, Control 00481DC0, vtable 00743290 (typed constant table, D21/D24),
//   callees: Animation::Animation 004851E0, AnimationParseField 00481880, MdlNode::MdlNode 00445210,
//   CExoArrayList<16-byte>::allocate 00437C50, Yaw 004A9F40, Pitch 004AA020, CRT fasin 006FCFF0 (x87 port).
// Conventions as m47_anim.cpp (D1 __fastcall for thiscall, D2 labels, D24 views, D30-FINAL x87 style).
// LookAtAnimation layout (0x3C, 00487241 push 3Ch): +00 vtbl, +04 U8 deactivated, +08 Animation* (MaxTree),
// +0C AnimRun*, +10 SafePointer<CAurObject> (object), +14 Gob* owner, +18 Part* looked-at part (on object),
// +1C Part* own part, +20 f yaw limit, +24 f pitch limit, +28 f anim length, +2C f max distance,
// +30 float* target key row, +34 float* rest key row, +38 U8 rest written.
#include "0099347167-anim3d.h"
#include <cmath>
#include <cstdint>
#include <cstring>

using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using kotor_scene3d::Vector; using kotor_scene3d::Quaternion; using kotor_scene3d::PointerList;
using kotor_scene3d::field; using kotor_scene3d::at; using kotor_scene3d::bits_to_float; using kotor_scene3d::float_to_bits;

// ---------------------------------------------------------------- existing providers (identical declarations)
extern "C" {
void __cdecl pointto_asin_compute_006fd028(); // ASM-only input/output ST0; sole shared compute core
void __cdecl pointto_fasin_006fcff0(); // ASM-only ST0 input/output; actual original CRT shell

void __cdecl aurora_maxtree_delete_0044aec0(aurora_model::MaxTree* tree);                            // model_load.hpp:205
void __fastcall aurora_maxtree_destruct_0043f730(void* tree, void* unused_edx);                      // M46 3d-model-model-destroy (RET 0)
aurora_model::MaxTree* __fastcall maxtree_construct_0044a900(aurora_model::MaxTree* self, void* unused_edx, const char* name);   // model_load.hpp:114
void* __cdecl operator_new_0043e1b0(U size);                                                         // construct3d.h:146 (jmp 006FA7E6)
void* __fastcall assign_00447fa0(void* safe_pointer, void* unused_edx, void* object);                // m46_camera_externs.h:19 (RET 4)
void __fastcall texture_list_remove_00445080(PointerList* list, void*, U value);                     // m46_scene_externs_c1.h:12
Quaternion* __fastcall quaternion_construct_004ac960(Quaternion* self, void* unused_edx, Vector axis, float angle);   // emitter.h:192
Quaternion* __cdecl yaw_pitch_roll_004acac0(Quaternion* out, float yaw, float pitch, float roll);  // tile_scene.h:43
extern volatile kotor_scene3d::I aurora_node_count_007a8a08;                                        // model_destroy.cpp:14
extern volatile kotor_scene3d::I aurora_node_created_007a8a0c;                                      // model_destroy.cpp:15
}
namespace kotor_renderer_gl {   // lane-renderer-bodies render_pool.h:106 (same declaration as controller_math.hpp)
void __fastcall allocate_00437d60(kotor_reconstruction::ArrayMutations* self, void* unused_edx, std::uint32_t capacity);
}

// ---------------------------------------------------------------- authored here / new externals
extern "C" {
void* __fastcall look_at_animation_construct_00487020(void* self, void* unused_edx, void* object, void* part,
    void* gob, void* target_part, U raw_a, U raw_b, U raw_c, U raw_d);
void __fastcall look_at_animation_build_00486140(void* self, void* unused_edx);
void __fastcall look_at_animation_destruct_00486090(void* self, void* unused_edx);
void* __fastcall look_at_animation_scalar_delete_00487170(void* self, void* unused_edx, U flags);
U8 __fastcall look_at_animation_is_active_00481850(void* self, void* unused_edx);
void __fastcall look_at_animation_deactivate_00481860(void* self, void* unused_edx);
void __fastcall look_at_animation_control_00481dc0(void* self, void* unused_edx);
aurora_model::MaxTree* __fastcall animation_construct_004851e0(aurora_model::MaxTree* self, void* unused_edx, const char* name);
void __cdecl animation_parse_field_00481880(aurora_model::MaxTree* tree, char* line);
void* __fastcall mdl_node_construct_00445210(void* self, void* unused_edx, const char* name);
void __fastcall exo_array_list_allocate16_00437c50(PointerList* self, void* unused_edx, I capacity);
double __cdecl quaternion_yaw_004a9f40(const Quaternion* q);
double __cdecl quaternion_pitch_004aa020(const Quaternion* q);
// new undefined external: Animation::InternalParseField 0046A410 (thiscall RET 4; ASCII .mdl animation parser -
// strncmp/sscanf/Parse<char[32]> 004638F0/AurKey Insert 00468970/ParseNode 004680E0). Only reached through the
// parse-fn cell MaxTree+04 while parsing ASCII models; not authored here.
void __fastcall animation_internal_parse_field_0046a410(aurora_model::MaxTree* self, void* unused_edx, char* line);

// vtable 00743290 (5 slots: 00487170 00481850 00481860 00481DC0 00486140) - typed constant table.
struct LookAtAnimationVTable {
    void* (__fastcall* scalar_delete)(void* self, void*, U flags);   // +00 00487170 (RET 4, EAX = this)
    U8 (__fastcall* is_active)(void* self, void*);                   // +04 00481850 (AL)
    void (__fastcall* deactivate)(void* self, void*);                // +08 00481860
    void (__fastcall* control)(void* self, void*);                   // +0C 00481DC0
    void (__fastcall* build_animation)(void* self, void*);           // +10 00486140
};
extern const LookAtAnimationVTable look_at_animation_vtable_00743290;
const LookAtAnimationVTable look_at_animation_vtable_00743290 = {
    &look_at_animation_scalar_delete_00487170, &look_at_animation_is_active_00481850,
    &look_at_animation_deactivate_00481860, &look_at_animation_control_00481dc0, &look_at_animation_build_00486140,
};
}

namespace {
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* obj(U w) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
inline const LookAtAnimationVTable* table(void* self) { return static_cast<const LookAtAnimationVTable*>(field<const void*>(self, 0)); }
constexpr float kYawPitchLimit_007432d4 = 89.0f;
constexpr float kRadToDeg_00742ef8 = 0x1.ca5dc2p+5f;
constexpr U kOneBits = 0x3F800000u;
constexpr U kQuarterBits = 0x3E800000u;
constexpr float kFpZero = 0.0f;
constexpr double kFpZero64 = 0.0;

// Original 00487108..0048711F. Integer carrier avoids an inferred private FP ABI.
__declspec(naked) U __cdecl x87_abs_bits(U) {
    __asm {
        fld dword ptr [esp+4]
        fcomp dword ptr kFpZero
        fld dword ptr [esp+4]
        fnstsw ax
        test ah, 5
        jp abs_store
        fchs
    abs_store:
        fstp dword ptr [esp+4]
        mov eax, dword ptr [esp+4]
        ret
    }
}
// Original 00487067..004870BF, repeated for the second limit at 004870C2.
__declspec(naked) U __cdecl x87_limit_bits(U) {
    __asm {
        fld dword ptr [esp+4]
        fcomp dword ptr kFpZero
        fld dword ptr [esp+4]
        fnstsw ax
        test ah, 5
        jp limit_compare
        fchs
    limit_compare:
        fcomp dword ptr kYawPitchLimit_007432d4
        fnstsw ax
        test ah, 5
        jp limit_max
        fld dword ptr [esp+4]
        fcomp dword ptr kFpZero
        fld dword ptr [esp+4]
        fnstsw ax
        test ah, 5
        jp limit_store
        fchs
        jmp limit_store
    limit_max:
        fld dword ptr kYawPitchLimit_007432d4
    limit_store:
        fstp dword ptr [esp+4]
        mov eax, dword ptr [esp+4]
        ret
    }
}
// Original 00482095..004820D6. Keep the raw upper-limit copy and all stack pops.
__declspec(naked) void __cdecl x87_clamp(float*, const float*) {
    __asm {
        mov ecx, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]
        fld dword ptr [edx]
        fchs
        fld dword ptr [ecx]
        fcomp st(1)
        fnstsw ax
        test ah, 41h
        jne clamp_lower
        fld dword ptr [ecx]
        jmp clamp_upper_compare
    clamp_lower:
        fld st(0)
    clamp_upper_compare:
        fcomp dword ptr [edx]
        fnstsw ax
        test ah, 5
        jp clamp_upper
        fld dword ptr [ecx]
        fcomp st(1)
        fnstsw ax
        test ah, 41h
        je clamp_pop
        fstp dword ptr [ecx]
        ret
    clamp_upper:
        mov eax, dword ptr [edx]
        mov dword ptr [ecx], eax
    clamp_pop:
        fstp st(0)
        ret
    }
}
// Existing storage owners retained. FP correction adds no yaw/pitch cells.
volatile U yaw_vector_00831ef8[3] = {0, 0, 0};
volatile U pitch_vector_00831f08[3] = {0, 0, 0};

// Original common FP subsequence 004A9F7A..004A9FCF / 004AA05A..004AA0AF.
// Arguments: quaternion, three-float scratch, existing global vector. All x87
// intermediates and both otherwise redundant stack pops are intentional.
__declspec(naked) void __cdecl yaw_pitch_terms(const Quaternion*, float*, volatile U*) {
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]
        mov ecx, dword ptr [esp+12]
        fld dword ptr [eax+8]
        fmul dword ptr [eax+4]
        fld dword ptr [eax+12]
        fmul dword ptr [eax]
        fsubp st(1), st
        fadd st, st
        fstp dword ptr [edx]
        fld dword ptr [eax+12]
        fld dword ptr [eax+4]
        fld st(1)
        fmul st, st(2)
        fld st(1)
        fmul st, st(2)
        faddp st(1), st
        fadd st, st
        fsubr dword ptr kOneBits
        fstp dword ptr [edx+4]
        fstp st(0)
        fstp st(0)
        fld dword ptr [eax+8]
        fmul dword ptr [eax+12]
        fld dword ptr [eax]
        fmul dword ptr [eax+4]
        mov eax, dword ptr [edx]
        mov dword ptr [ecx], eax
        mov eax, dword ptr [edx+4]
        mov dword ptr [ecx+4], eax
        faddp st(1), st
        fadd st, st
        fstp dword ptr [edx+8]
        ret
    }
}
// Grow-by-doubling push of a dword into a CExoArrayList<float> through allocate 00437D60 (BuildAnimation inline).
inline void push_float_bits(PointerList* list, U bits) {
    const I count = field<I>(list, 4), capacity = field<I>(list, 8);
    if (count == capacity) kotor_renderer_gl::allocate_00437d60(list, nullptr, capacity != 0 ? static_cast<U>(capacity) * 2u : 8u);
    field<U>(obj(field<U>(list, 0)), static_cast<U>(field<I>(list, 4)) * 4u) = bits;
    field<I>(list, 4) = field<I>(list, 4) + 1;
}
}

extern "C" {

// 00487020 LookAtAnimation::LookAtAnimation(CAurObject* object, Part* part, Gob* gob, Part* own_part,
// float yaw_limit, float pitch_limit, float length, float distance)  thiscall RET 20h.
void* __fastcall look_at_animation_construct_00487020(void* self, void*, void* object, void* part,
    void* gob, void* target_part, U raw_a, U raw_b, U raw_c, U raw_d) {
    field<U>(self, 0x08) = 0;                                           // 00487040
    field<U>(self, 0x0C) = 0;                                           // 00487043
    field<U8>(self, 0x04) = 0;                                          // 00487046
    field<const void*>(self, 0x00) = &look_at_animation_vtable_00743290;   // 0048704C mov [esi],743290h
    field<U>(self, 0x10) = 0;                                           // 00487056 SafePointer ctor
    assign_00447fa0(at(self, 0x10), nullptr, object);                   // 00487058..00487062 SafePointer::operator=
    field<void*>(self, 0x1C) = target_part;                             // 00487071 / 00487081
    field<void*>(self, 0x18) = part;                                    // 00487075 / 00487086
    field<void*>(self, 0x14) = gob;                                     // 0048707D / 00487089
    // 00487067..004870BF: +20 = |a| < 89 ? |a| : 89 (NaN -> 89).
    field<U>(self, 0x20) = x87_limit_bits(raw_a);
    field<U>(self, 0x24) = x87_limit_bits(raw_b);
    field<U>(self, 0x28) = x87_abs_bits(raw_c);                         // 00487108..0048711F
    field<U>(self, 0x2C) = x87_abs_bits(raw_d);                         // 00487122..0048713B
    field<U>(self, 0x30) = 0;                                           // 0048713E
    field<U>(self, 0x34) = 0;                                           // 00487141
    field<U8>(self, 0x38) = 0;                                          // 00487144
    look_at_animation_build_00486140(self, nullptr);                    // 00487139..00487147 (direct call)
    return self;                                                        // 00487150
}

// 00486140 LookAtAnimation::BuildAnimation()  thiscall RET 0: builds a one-controller animation tree
// ("LookAtAnimation" / root "looking" / one "lookingParent" node per ancestor of the own part).
void __fastcall look_at_animation_build_00486140(void* self, void*) {
    void* const anim_memory = operator_new_0043e1b0(0x88);              // 0048615E..00486167
    aurora_model::MaxTree* const animation = anim_memory != nullptr     // 00486175..0048618D
        ? animation_construct_004851e0(static_cast<aurora_model::MaxTree*>(anim_memory), nullptr, "LookAtAnimation")   // 0048617D (007432BC)
        : nullptr;
    field<void*>(self, 0x08) = animation;                               // 00486196
    void* const root_memory = live_operator_new(0x50);                  // 00486190..00486199
    void* const root = root_memory != nullptr                           // 004861A5..004861C7
        ? mdl_node_construct_00445210(root_memory, nullptr, "looking")  // 004861B1 (007432B4)
        : nullptr;
    field<U16>(root, 0x02) = field<U16>(field<void*>(self, 0x1C), 0x28);   // 004861C7..004861D2
    void* previous = root;                                              // ebx
    for (void* ancestor = field<void*>(field<void*>(self, 0x1C), 0x3C); ancestor != nullptr;
         ancestor = field<void*>(ancestor, 0x3C)) {                     // 004861D6..004861E2 / 0048629E..004862A3
        void* const memory = live_operator_new(0x50);                   // 004861F0..004861F6
        void* const node = memory != nullptr                            // 00486202..00486220
            ? mdl_node_construct_00445210(memory, nullptr, "lookingParent")   // 0048620E (007432A4)
            : nullptr;
        field<void*>(previous, 0x0C) = node;                            // 00486220 child->parent
        // 00486223..00486293: node->children (+2C data, +30 count, +34 capacity) append, inline grow-by-doubling
        // with operator new 006FA7E6 + element copy + free 006FA390 of the old block.
        if (field<I>(node, 0x30) == field<I>(node, 0x34)) {             // 00486223..00486233
            const I capacity = field<I>(node, 0x34);
            const I grown = capacity != 0 ? capacity * 2 : 8;           // 00486235..0048623D
            void* const old = field<void*>(node, 0x2C);                 // 00486242
            field<I>(node, 0x34) = grown;                               // 0048624D
            field<void*>(node, 0x2C) = live_operator_new(static_cast<U>(grown) * 4u);   // 00486245..00486258
            for (I i = 0; i < field<I>(node, 0x30); ++i)                // 0048625E..00486273
                field<U>(field<void*>(node, 0x2C), static_cast<U>(i) * 4u) = field<U>(old, static_cast<U>(i) * 4u);
            if (old != nullptr) live_free(old);                         // 00486275..0048627F
        }
        field<void*>(field<void*>(node, 0x2C), static_cast<U>(field<I>(node, 0x30)) * 4u) = previous;   // 00486286..00486290
        field<I>(node, 0x30) = field<I>(node, 0x30) + 1;                // 00486293
        field<U16>(node, 0x02) = field<U16>(ancestor, 0x28);            // 00486296..0048629A
        previous = node;                                                // 00486231 mov ebx,esi
    }
    field<void*>(previous, 0x0C) = nullptr;                             // 004862AD (top node has no parent)
    field<void*>(previous, 0x08) = field<void*>(self, 0x08);            // 004862B4..004862B7 node->tree
    field<void*>(field<void*>(self, 0x08), 0x28) = previous;            // 004862BA..004862BD tree->root
    // Controller data on the root node: CExoArrayList<float> at +44 (data, +48 count, +4C capacity).
    auto* const keys = static_cast<PointerList*>(at(root, 0x44));
    const U16 key_start = field<U16>(root, 0x48);                       // 004862C0..004862CA
    push_float_bits(keys, 0);                                           // 004862CF..004862FB time 0.0
    push_float_bits(keys, kQuarterBits);                                // 004862FE..00486328 time 0.25
    const U16 data_start = field<U16>(root, 0x48);                      // 0048632B..00486339
    push_float_bits(keys, 0); push_float_bits(keys, 0); push_float_bits(keys, 0); push_float_bits(keys, kOneBits);   // 0048633E..004863F8 row 0 (x,y,z,w)
    push_float_bits(keys, 0); push_float_bits(keys, 0); push_float_bits(keys, 0); push_float_bits(keys, kOneBits);   // 004863FB..004864C3 row 1
    // 004864C0..00486515: controller record {type 14h, word ?, word rows 2, word key_start, word data_start,
    // byte columns 4, 3 bytes ?} appended to the root's 16-byte controller list (+38 data, +3C count, +40 capacity).
    // D2 (0048650B / 00486502): the low word of [esp+24h] and the high 3 bytes of [esp+2Ch] are never written by
    // the original (uninitialised stack bytes copied into the record); value-initialised to 0 here.
    auto* const controllers = static_cast<PointerList*>(at(root, 0x38));
    if (field<I>(controllers, 4) == field<I>(controllers, 8)) {          // 004864C6..004864DD
        const I capacity = field<I>(controllers, 8);
        exo_array_list_allocate16_00437c50(controllers, nullptr, capacity != 0 ? capacity * 2 : 8);   // 004864DF..004864EF
    }
    void* const record = at(field<void*>(controllers, 0), static_cast<U>(field<I>(controllers, 4)) * 16u);   // 004864F4..00486500
    field<U>(record, 0x00) = 0x14u;                                     // 00486506 (ebx = 14h)
    field<U>(record, 0x04) = 0x00020000u;                               // 00486508 (word 2 at [esp+26h]; D2 low word)
    field<U>(record, 0x08) = static_cast<U>(key_start) | (static_cast<U>(data_start) << 16);   // 0048650F
    field<U>(record, 0x0C) = 4u;                                        // 00486512 (byte 4 at [esp+2Ch]; D2 high bytes)
    field<I>(controllers, 4) = field<I>(controllers, 4) + 1;            // 00486515
    void* const data = field<void*>(keys, 0);                           // 00486518 (re-read after growth)
    field<void*>(self, 0x34) = at(data, static_cast<U>(data_start) * 4u);          // 0048651A..00486524
    field<void*>(self, 0x30) = at(data, static_cast<U>(data_start) * 4u + 0x10u);  // 00486527..00486530
    field<U>(field<void*>(self, 0x08), 0x50) = field<U>(self, 0x28);    // 0048652D..00486537 length
    field<U>(field<void*>(self, 0x08), 0x54) = field<U>(self, 0x28);    // 0048653A..00486546 transition
}

// 00486090 LookAtAnimation::~LookAtAnimation()  thiscall RET 0.
void __fastcall look_at_animation_destruct_00486090(void* self, void*) {
    field<const void*>(self, 0x00) = &look_at_animation_vtable_00743290;   // 00486097
    I index = field<I>(field<void*>(self, 0x14), 0xB4);                 // 0048609D (gob runs count, read once)
    while (index != 0) {                                                // 004860A3 / 004860FA..004860FC
        void* const gob = field<void*>(self, 0x14);                     // 004860B0 (re-read)
        const U run = field<U>(obj(field<U>(gob, 0xB0)), static_cast<U>(index - 1) * 4u);   // 004860B3..004860B9
        --index;                                                        // 004860C0
        if (run != field<U>(self, 0x0C)) continue;                      // 004860BD..004860C3
        const I new_count = field<I>(gob, 0xB4) - 1;                    // 004860C5..004860CB
        field<I>(gob, 0xB4) = new_count;                                // 004860D2
        if (index < new_count) {                                        // 004860CC..004860D8
            I i = index;
            do {                                                        // 004860E0..004860F8 (data, count re-read)
                const U d = field<U>(gob, 0xB0);
                field<U>(obj(d), static_cast<U>(i) * 4u) = field<U>(obj(d), static_cast<U>(i + 1) * 4u);
                ++i;
            } while (i < field<I>(gob, 0xB4));
        }
    }
    void* const object = field<void*>(self, 0x10);                      // 004860FF (~SafePointer<CAurObject>)
    if (object != nullptr)                                              // 00486102..00486107
        texture_list_remove_00445080(static_cast<PointerList*>(at(object, 4)), nullptr, word_of(at(self, 0x10)));   // 00486109..0048610D
    // 00486112: base-class vtable 00743234 stored before the inlined base destructor. Not reproduced: no virtual
    // call follows on this object before it is freed (deviation without observable effect; base table unpublished).
    void* const animation = field<void*>(self, 0x08);                   // 00486118
    if (animation != nullptr) {                                         // 0048611B..0048611D
        aurora_maxtree_destruct_0043f730(animation, nullptr);           // 0048611F..00486121
        aurora_maxtree_delete_0044aec0(static_cast<aurora_model::MaxTree*>(animation));   // 00486126..0048612C
    }
}

// 00487170 LookAtAnimation scalar deleting destructor  thiscall RET 4, EAX = this.
void* __fastcall look_at_animation_scalar_delete_00487170(void* self, void*, U flags) {
    look_at_animation_destruct_00486090(self, nullptr);                 // 00487173
    if ((flags & 1u) != 0) live_free(self);                             // 00487178..00487185 _free 006FA390
    return self;                                                        // 00487188
}

// 00481850 LookAtAnimation::IsActive()  AL = (run->[+20] == 0). (Upper EAX bytes = run pointer bits; AL contract.)
U8 __fastcall look_at_animation_is_active_00481850(void* self, void*) {
    return field<U>(field<void*>(self, 0x0C), 0x20) == 0 ? 1 : 0;       // 00481850..0048185B
}

// 00481860 LookAtAnimation::Deactivate()
void __fastcall look_at_animation_deactivate_00481860(void* self, void*) {
    field<U8>(self, 0x04) = 1;                                          // 00481860
}

// 00481DC0 LookAtAnimation::Control()  thiscall RET 0 (vtable +0C).
void __fastcall look_at_animation_control_00481dc0(void* self, void*) {
    if (field<void*>(self, 0x10) == nullptr) table(self)->deactivate(self, nullptr);   // 00481DCB..00481DD7 call [vtbl+8]
    void* const object = field<void*>(self, 0x10);                      // 00481DDA
    if (object != nullptr) {                                            // 00481DDD..00481DDF
        void* const gob = field<void*>(self, 0x14);                     // 00481DE1
        Vector separation;                                                  // 00481DE4..00481E0B (three fsub, fstp dword)
        U beyond;
        __asm {
            mov eax, gob
            mov ecx, object
            fld dword ptr [eax+80h]
            fsub dword ptr [ecx+80h]
            fld dword ptr [eax+7ch]
            fsub dword ptr [ecx+7ch]
            fld dword ptr [eax+78h]
            fsub dword ptr [ecx+78h]
            lea edx, separation
            fstp dword ptr [edx]
            fstp dword ptr [edx+4]
            fstp dword ptr [edx+8]
            push edx
            call vector_magnitude_004ab100
            add esp, 4
            mov ecx, self
            fcomp dword ptr [ecx+2ch]
            fnstsw ax
            test ah, 41h
            setz al
            movzx eax, al
            mov beyond, eax
        }
        if (beyond != 0)
            table(self)->deactivate(self, nullptr);                     // 00481E21..00481E25
    }
    if (field<U8>(self, 0x04) != 0) {                                   // 00481E28..00481E2B -> 004821BD
        if (field<U8>(self, 0x38) != 0) return;                         // 004821BD..004821C0
        field<U8>(self, 0x38) = 1;                                      // 004821C8
        void* const part = field<void*>(self, 0x1C);                    // 004821C2
        float* const rest = static_cast<float*>(field<void*>(self, 0x34));
        float* const target = static_cast<float*>(field<void*>(self, 0x30));
        field<U>(rest, 0x0) = field<U>(part, 0x18); field<U>(rest, 0x4) = field<U>(part, 0x1C);    // 004821CC..004821D7
        field<U>(rest, 0x8) = field<U>(part, 0x20); field<U>(rest, 0xC) = field<U>(part, 0x14);    // 004821DA..004821EC
        field<U>(target, 0x0) = field<U>(part, 0x18); field<U>(target, 0x4) = field<U>(part, 0x1C);  // 004821EF..004821FD
        field<U>(target, 0x8) = field<U>(part, 0x20); field<U>(target, 0xC) = field<U>(part, 0x14);  // 00482200..0048220E
        void* const run = field<void*>(self, 0x0C);
        field<U>(run, 0x0C) = 0;                                        // 00482211..00482214
        field<U>(run, 0x10) = 0;                                        // 00482217..0048221A
        field<U>(run, 0x14) = kOneBits;                                 // 0048221D..00482220
        field<U>(run, 0x08) = 9;                                        // 00482227..0048222A
        field<U>(field<void*>(self, 0x08), 0x50) = 0;                   // 00482231..00482234 animation length 0
        return;
    }
    void* const own_part = field<void*>(self, 0x1C);                    // 00481E31
    float* const rest = static_cast<float*>(field<void*>(self, 0x34));
    field<U>(rest, 0x0) = field<U>(own_part, 0x18);                     // 00481E34..00481E3A
    field<U>(rest, 0x4) = field<U>(own_part, 0x1C);                     // 00481E3C..00481E42
    field<U>(static_cast<float*>(field<void*>(self, 0x34)), 0x8) = field<U>(own_part, 0x20);   // 00481E45..00481E4E
    field<U>(static_cast<float*>(field<void*>(self, 0x34)), 0xC) = field<U>(own_part, 0x14);   // 00481E51..00481E56
    // Own part world transform (Part vtbl+0C GetWorld 0043E4A0 family; RET 8) into zero/identity-initialised locals.
    Vector own_position = {0.0f, 0.0f, 0.0f};                           // 00481E85..00481E95
    Quaternion own_orientation = {1.0f, 0.0f, 0.0f, 0.0f};              // 00481E65..00481E7D
    void* const own = field<void*>(self, 0x1C);                         // 00481E59
    kotor_anim3d::part_table(own)->s00c(own, nullptr, word_of(&own_position), word_of(&own_orientation));   // 00481E9D..00481EA0
    const Vector up = {0.0f, 1.0f, 0.0f};                               // 00481EB5..00481EC5
    Vector forward;
    quaternion_rotate_004a9bb0(&forward, &own_orientation, &up);        // 00481EA3..00481ED2
    Vector target_position = {0.0f, 0.0f, 0.0f};                        // 00481EE4..00481EF4
    Quaternion target_orientation = {1.0f, 0.0f, 0.0f, 0.0f};           // 00481EFC..00481F1D
    void* const looked = field<void*>(self, 0x18);                      // 00481ED2
    kotor_anim3d::part_table(looked)->s00c(looked, nullptr, word_of(&target_position), word_of(&target_orientation));   // 00481F28..00481F2B
    Vector delta;                                                       // 00481F2E..00481F7B (fsub, fstp dword)
    __asm {
        lea eax, target_position
        lea ecx, own_position
        lea edx, delta
        fld dword ptr [eax]
        fsub dword ptr [ecx]
        fstp dword ptr [edx]
        fld dword ptr [eax+4]
        fsub dword ptr [ecx+4]
        fstp dword ptr [edx+4]
        fld dword ptr [eax+8]
        fsub dword ptr [ecx+8]
        fstp dword ptr [edx+8]
    }
    const Vector flat_delta = {delta.x, delta.y, 0.0f};                 // 00481F83..00481F96 (z word zeroed 00481F87)
    const Vector flat_forward = {forward.x, forward.y, 0.0f};           // 00481F9C..00481FAF (z from zeroed copy 00481F5F)
    Quaternion arc;
    rotation_arc_004ab630(&arc, flat_forward, flat_delta);              // 00481FA7..00481FB2
    Quaternion turned;                                                  // 00481FB7..0048200D: arc * own part orientation
    const Quaternion* const product = quaternion_multiply_004a9ac0(&turned, &arc, static_cast<const Quaternion*>(at(own_part, 0x14)));
    const Quaternion combined = *product;                               // 00481FED..00482009 (copy into [esp+50h])
    float yaw;
    __asm {
        lea eax, combined
        push eax
        call quaternion_yaw_004a9f40
        add esp, 4
        fstp dword ptr yaw
    }
    Vector direction;
    const Vector* const normalized = normalize_004ab180(&direction, &delta);   // 00482016..00482023
    delta = *normalized;                                                // 00482028..00482038 (copied back over delta)
    float pitch;
    __asm {
        lea eax, delta
        fld dword ptr [eax+8]
        call pointto_fasin_006fcff0
        fstp dword ptr pitch
    }
    Quaternion axis_angle;
    const Vector x_axis = {1.0f, 0.0f, 0.0f};                           // 00482054..00482083
    const Quaternion* const pitch_quat = quaternion_construct_004ac960(&axis_angle, nullptr, x_axis, pitch);   // 00482086
    __asm {
        mov eax, pitch_quat
        push eax
        call quaternion_pitch_004aa020
        add esp, 4
        fstp dword ptr pitch
    }
    x87_clamp(&yaw, static_cast<const float*>(at(self, 0x20)));
    x87_clamp(&pitch, static_cast<const float*>(at(self, 0x24)));
    Quaternion pitch_rotation, yaw_rotation, result;
    Quaternion* const p = yaw_pitch_roll_004acac0(&pitch_rotation, 0.0f, pitch, 0.0f);   // 0048211C..00482126
    Quaternion* const y = yaw_pitch_roll_004acac0(&yaw_rotation, yaw, 0.0f, 0.0f);       // 00482132..0048213B
    const Quaternion* const r = quaternion_multiply_004a9ac0(&result, y, p);             // 00482143..0048214C
    const Quaternion q = *r;                                            // 00482151..0048216F
    float* const target = static_cast<float*>(field<void*>(self, 0x30));
    field<U>(target, 0x0) = float_to_bits(q.x);                                    // 00482173 (raw dword copies)
    field<U>(static_cast<float*>(field<void*>(self, 0x30)), 0x4) = float_to_bits(q.y);   // 00482175..0048217C
    field<U>(static_cast<float*>(field<void*>(self, 0x30)), 0x8) = float_to_bits(q.z);   // 0048217F..00482186
    field<U>(static_cast<float*>(field<void*>(self, 0x30)), 0xC) = float_to_bits(q.w);   // 00482189..00482190
    void* const run = field<void*>(self, 0x0C);
    field<U>(run, 0x0C) = 0;                                            // 00482193..00482196
    field<U>(field<void*>(self, 0x0C), 0x10) = 0;                       // 00482199..0048219C
    field<U>(field<void*>(self, 0x0C), 0x14) = kOneBits;                // 0048219F..004821A2
    field<U>(field<void*>(self, 0x0C), 0x20) = 0;                       // 004821A9..004821AF
}

// 004851E0 Animation::Animation(char* name)  thiscall RET 4.
aurora_model::MaxTree* __fastcall animation_construct_004851e0(aurora_model::MaxTree* self, void*, const char* name) {
    maxtree_construct_0044a900(self, nullptr, name);                    // 004851FA..004851FD
    field<U>(self, 0x7C) = 0;                                           // 00485204
    field<U>(self, 0x80) = 0;                                           // 00485207
    field<U>(self, 0x78) = 0;                                           // 0048520D
    field<U>(self, 0x84) = 0;                                           // 00485210
    field<U8>(self, 0x4C) = 5;                                          // 00485216..0048521C byte [00740A12] = 05 (.rdata)
    field<U8>(self, 0x58) = 0;                                          // 00485223
    field<U>(self, 0x50) = kOneBits;                                    // 00485226 length 1.0f
    field<U>(self, 0x54) = kQuarterBits;                                // 0048522D transition 0.25f
    using TreeParseFn = void (__cdecl*)(aurora_model::MaxTree*, char*);   // model_load.hpp:81
    field<TreeParseFn>(self, 0x04) = &animation_parse_field_00481880;  // 00485234 mov [esi+4],481880h
    return self;                                                        // 0048523B
}

// 00481880 AnimationParseField(void* tree, char* line)  cdecl: tail to Animation::InternalParseField(tree, line).
void __cdecl animation_parse_field_00481880(aurora_model::MaxTree* tree, char* line) {
    animation_internal_parse_field_0046a410(tree, nullptr, line);       // 00481880..00481889
}

// 00445210 MdlNode::MdlNode(char* name)  thiscall RET 4; the name argument is not read by the body.
void* __fastcall mdl_node_construct_00445210(void* self, void*, const char*) {
    // 0044522C..00445263: zero +10 +14 +18 +28 +24 +20 +30 +34 +2C +3C +40 +38 +48 +4C +44 +0C; +1C = 1.0f;
    // word +2 = 0; word +0 = 1 (node type); then 00445272..004452A0 re-stores +10 +14 +1C(1.0f) +20 +18 +24 +28.
    static const U zero_offsets[] = {0x10, 0x14, 0x18, 0x28, 0x24, 0x20, 0x30, 0x34, 0x2C, 0x3C, 0x40, 0x38, 0x48, 0x4C, 0x44, 0x0C};
    for (U off : zero_offsets) field<U>(self, off) = 0;
    field<U>(self, 0x1C) = kOneBits;                                    // 0044523E
    field<U16>(self, 0x02) = 0;                                         // 00445263
    field<U16>(self, 0x00) = 1;                                         // 00445267
    aurora_node_count_007a8a08 = aurora_node_count_007a8a08 + 1;        // 0044526C
    field<U>(self, 0x10) = 0; field<U>(self, 0x14) = 0;                 // 00445272..00445277
    field<U>(self, 0x1C) = kOneBits;                                    // 0044528A
    field<U>(self, 0x20) = 0; field<U>(self, 0x18) = 0;                 // 0044528D / 0044529A
    field<U>(self, 0x24) = 0; field<U>(self, 0x28) = 0;                 // 0044529D / 004452A0
    aurora_node_created_007a8a0c = aurora_node_created_007a8a0c + 1;    // 004452AF
    return self;                                                        // EAX = this (00445228)
}

// 00437C50 CExoArrayList<16-byte element>::allocate(int capacity)  thiscall RET 4. operator new 006FC0D2 / free
// 006FC0CD are the jmp thunks of 006FA7E6 / 006FA390 (M41 live family).
void __fastcall exo_array_list_allocate16_00437c50(PointerList* self, void*, I capacity) {
    field<I>(self, 8) = capacity;                                       // 00437C57
    void* const old = field<void*>(self, 0);                            // 00437C5B
    field<void*>(self, 0) = live_operator_new(static_cast<U>(capacity) << 4);   // 00437C5D..00437C66
    for (I i = 0; i < field<I>(self, 4); ++i) {                         // 00437C68..00437C9E (count re-read)
        void* const to = at(field<void*>(self, 0), static_cast<U>(i) * 16u);
        const void* const from = static_cast<const unsigned char*>(old) + static_cast<U>(i) * 16u;
        for (U k = 0; k < 16; k += 4) field<U>(to, k) = *reinterpret_cast<const U*>(static_cast<const unsigned char*>(from) + k);
    }
    if (old != nullptr) live_free(old);                                 // 00437CA2..00437CAC
}

// 004A9F40 Yaw(Quaternion* q)  cdecl, ST0 result (degrees).
__declspec(naked) double __cdecl quaternion_yaw_004a9f40(const Quaternion*) {
    __asm {
        sub esp, 12
        mov eax, dword ptr [esp+16]
        mov edx, esp
        lea ecx, yaw_vector_00831ef8
        push ecx
        push edx
        push eax
        call yaw_pitch_terms
        add esp, 12
        fld dword ptr [esp+4]
        mov edx, dword ptr [esp+8]
        fcomp qword ptr kFpZero64
        mov dword ptr yaw_vector_00831ef8[8], edx
        fnstsw ax
        test ah, 44h
        jp yaw_angle
        fld dword ptr [esp]
        fcomp qword ptr kFpZero64
        fnstsw ax
        test ah, 44h
        jp yaw_angle
        fld dword ptr kFpZero
        add esp, 12
        ret
    yaw_angle:
        fld dword ptr [esp]
        fchs
        fld dword ptr [esp+4]
        fpatan
        fmul dword ptr kRadToDeg_00742ef8
        add esp, 12
        ret
    }
}

// 004AA020 Pitch(Quaternion* q), exact original x87 sequence and ST0 return.
__declspec(naked) double __cdecl quaternion_pitch_004aa020(const Quaternion*) {
    __asm {
        sub esp, 12
        mov eax, dword ptr [esp+16]
        mov edx, esp
        lea ecx, pitch_vector_00831f08
        push ecx
        push edx
        push eax
        call yaw_pitch_terms
        add esp, 12
        fld dword ptr [esp+8]
        mov edx, dword ptr [esp+8]
        fld dword ptr [esp+4]
        mov dword ptr pitch_vector_00831f08[8], edx
        fmul dword ptr [esp+4]
        fld dword ptr [esp]
        fmul dword ptr [esp]
        faddp st(1), st
        fsqrt
        fxch st(1)
        fxch st(1)
        fpatan
        fmul dword ptr kRadToDeg_00742ef8
        add esp, 12
        ret
    }
}

} // extern "C"

// Sole sharedasin compute core: original006FD028..006FD036, input/output ST0.
// Control and PointTo both use the sole original raw asin shell; no double wrapper.
extern "C" __declspec(naked) void __cdecl pointto_asin_compute_006fd028(){
    __asm {
        fld1
        fadd st, st(1)
        fld1
        fsub st, st(2)
        fmulp st(1), st
        fsqrt
        fpatan
        ret
    }
}
