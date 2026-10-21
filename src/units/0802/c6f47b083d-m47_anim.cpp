// m47_anim.cpp - lane-m47-3d group "anim": the ten Gob vtable (00741268) animation slots that no lane authored,
// plus the small 3D callees they need that have no M46 provider (authored_callees).
// Original evidence: swkotor.exe (GOG K1), instruction addresses cited per behavior (local disassembly lane-m47-3d/d/).
// Conventions (DECISIONS D1/D1-amendment/D2/D21/D24/D30-FINAL/D41/D52, same as lane-3d-anim anim3d.h):
//  * __thiscall bodies are __fastcall(Self*, void* unused_edx, stack...) with identical callee cleanup; EDX never read.
//  * The slot symbols and raw-word contract are the consumer declarations in 3d/render/vtables_gen.h (D42).
//  * Where the original leaves EAX undefined (or holds an incidental callee value no caller reads) the slot returns 0
//    with a "D2:EAX" label; where EAX is a deterministic value of this body, that value is returned.
//  * Virtual calls go through typed table views (published GobVTable; D24 local views for unpublished tables).
//  * Allocator family 006FA7E6 / 006FA390 -> M41 live_operator_new / live_free (scene3d.h). Original operator new
//    returns NULL on failure and the original then stores through it (faults); reproduced literally (D30-FINAL note).
//  * fopen 006FAD9D: linked UCRT fopen (same precedent as lane-aurres aurres.cpp 0044C7C0; FILE* handed to the M46
//    MDL loader 00464200, which consumes the same CRT family).
#include "3775ec35d8-anim3d.h"
#include <cstdio>
#include <cstring>

using kotor_scene3d::U; using kotor_scene3d::I; using kotor_scene3d::U8; using kotor_scene3d::U16;
using kotor_scene3d::PointerList; using kotor_scene3d::field; using kotor_scene3d::at;

// ---------------------------------------------------------------- existing M46 providers (identical declarations)
extern "C" {
// lane-3d-model model_load.hpp (declared here over the same types; anim3d.h does not include model_load.hpp)
aurora_model::MaxTree* __cdecl aurora_find_model_00464110(const char* name);
aurora_model::MaxTree* __cdecl aurora_load_model_00464200(std::FILE* mdl, std::FILE* mdx);
void __cdecl aurora_maxtree_delete_0044aec0(aurora_model::MaxTree* tree);
aurora_model::MaxTree* __fastcall aurora_tree_as_animation_0043e1d0(aurora_model::MaxTree* self, void* unused_edx);
const char* __fastcall aurora_node_translate_name_00438a00(aurora_model::MdlNode* self, void* unused_edx);
void __cdecl aurora_update_anim_footprint_00437db0(aurora_model::MdlNode* node, aurora_model::I scale);
extern volatile aurora_model::U aurora_model_no_delete_007a8a04;   // 0044B2CD cmp [007A8A04],0
extern volatile aurora_model::U aurora_anim_footprint_007a6990;    // 00439DA9..00439DD0
// lane-3d-bsp bsp.h / lane-3d-model (M46: 3d-model-model-misc)
I __fastcall exo_array_list_count_004af240(const PointerList* list, void*, void* value);
// texture-epoch-texture_x86 (m46_scene_externs_c1.h spelling)
void __fastcall texture_list_remove_00445080(PointerList* list, void*, U value);
}

// ---------------------------------------------------------------- authored here (no M46 provider) / new externals
extern "C" {
// authored callees
I __cdecl mdl_get_max_id_00438400(void* node, I max_id);
void __cdecl mdl_synchronize_nodes_00439a10(void* destination, void* source, I* next_id);
void __fastcall maxtree_synchronize_tree_00439d70(void* tree, void* unused_edx, void* other);
void* __fastcall oo_anim_run_construct_004818c0(void* self, void* unused_edx, void* animation, const U* times,
    const U* values, I count, U raw_length);
// LookAtAnimation::LookAtAnimation - authored in m47_anim_lookat.cpp (same lane group)
void* __fastcall look_at_animation_construct_00487020(void* self, void* unused_edx, void* object, void* part,
    void* gob, void* target_part, U raw_a, U raw_b, U raw_c, U raw_d);   // RET 20h (0048715E)
}

namespace {
using kotor_anim3d::gob_table;
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline void* obj(U w) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(w)); }
inline PointerList* list_at(void* o, U off) { return static_cast<PointerList*>(at(o, off)); }
// Element i of a CExoArrayList<T*> (data word at +0 re-read at each use, as the original does).
inline U element(void* list_owner, U data_off, I i) { return field<U>(obj(field<U>(list_owner, data_off)), static_cast<U>(i) * 4u); }
// D24 local view of the LookAtAnimation table (vtable 00743290; no published table): +00 scalar deleting destructor
// (00487170, RET 4), +08 Deactivate (00481860, RET 0). anim3d.h LookAtSlots leaves +08 untyped.
struct LookAtView {
    void (__fastcall* scalar_delete)(void* object, void*, U flags);
    const void* s04;
    void (__fastcall* deactivate)(void* object, void*);
};
inline const LookAtView* look_at_table(void* o) { return static_cast<const LookAtView*>(field<void*>(o, 0)); }
// D2: original leaves EAX as an incidental value no caller reads.
constexpr U kD2Eax = 0;
}

extern "C" {

// 00484C40 Gob::AddAnimationFlag(int flag)  thiscall RET 4.
U __fastcall gob_add_animation_flag_00484c40(aurora_model::Gob* self, void*, U flag) {
    const U last = field<U>(self, 0xB4) - 1u;                         // 00484C40..00484C46 count-1
    if (static_cast<I>(last) < 0) return last;                        // 00484C47 js (EAX = count-1)
    const U run = element(self, 0xB0, static_cast<I>(last));          // 00484C49..00484C4F data[count-1]
    field<U>(obj(run), 0x08) |= flag;                                 // 00484C52..00484C5B run+8 |= flag
    return run;                                                       // EAX = run (00484C4F)
}

// 00484C70 Gob::RemoveAnimationFlag(int flag)  thiscall RET 4. Note: subtracts (00484C91 sub), not and-not.
U __fastcall gob_remove_animation_flag_00484c70(aurora_model::Gob* self, void*, U flag) {
    const U last = field<U>(self, 0xB4) - 1u;                         // 00484C70..00484C76
    if (static_cast<I>(last) < 0) return last;                        // 00484C77 js (EAX = count-1)
    void* const run = obj(element(self, 0xB0, static_cast<I>(last))); // 00484C79..00484C7F
    const U flags = field<U>(run, 0x08);                              // 00484C82
    if (flags == 0) return flags;                                     // 00484C85..00484C87
    if (flag == 0) return flags;                                      // 00484C89..00484C8F
    field<U>(run, 0x08) = flags - flag;                               // 00484C91..00484C93
    return flags - flag;
}

// 00481870 Gob::EndLookAtAnimate()  thiscall RET 0: tail jump to LookAtAnimation slot +08 (Deactivate).
U __fastcall gob_end_look_at_animate_00481870(aurora_model::Gob* self, void*) {
    void* const look_at = field<void*>(self, 0x174);                  // 00481870
    if (look_at == nullptr) return kD2Eax;                            // 00481876..00481878 (D2:EAX caller's value)
    look_at_table(look_at)->deactivate(look_at, nullptr);             // 0048187A..0048187C jmp [eax+8]
    return kD2Eax;                                                    // D2:EAX (callee leaves EAX = vtable word)
}

// 0044B2C0 Gob::RemoveAddInAnimations()  thiscall RET 0.
U __fastcall gob_remove_add_in_animations_0044b2c0(aurora_model::Gob* self, void*) {
    auto* const tree = static_cast<aurora_model::MaxTree*>(field<void*>(self, 0x64));   // 0044B2C3
    if (tree != nullptr && aurora_model_no_delete_007a8a04 == 0) {    // 0044B2C9..0044B2D3
        if (--field<I>(tree, 0x54) == 0)                              // 0044B2D5 dec [eax+54h]; jne
            aurora_maxtree_delete_0044aec0(tree);                     // 0044B2DA..0044B2E0
    }
    field<void*>(self, 0x64) = nullptr;                               // 0044B2E3
    return kD2Eax;                                                    // D2:EAX (tree / flag / delete result)
}

// 00482250 Gob::StopOutOfOrderAnimation(char* name)  thiscall RET 4.
U __fastcall gob_stop_out_of_order_animation_00482250(aurora_model::Gob* self, void*, U name) {
    const I count = field<I>(self, 0xB4);                             // 00482251 (read once)
    I i = 0;                                                          // 00482257
    if (count <= 0) return 0;                                         // 00482259..0048225B (EAX = 0)
    const U data = field<U>(self, 0xB0);                              // 0048225D (data read once; edx walks it)
    for (; i < count; ++i) {                                          // 0048226F..00482275
        void* const run = obj(field<U>(obj(data), static_cast<U>(i) * 4u));   // 00482264
        if ((field<U>(run, 0x08) & 0x200u) != 0) {                    // 00482266 test [ebx+8],200h
            // 0048227C..0048228F: PlayAnimation(name, 1.0f, 400h, 0) via Gob vtbl+18; EAX = its result.
            return gob_table(self)->s018_gob_play_animation(self, nullptr, name, 0x3F800000u, 0x400u, 0u);
        }
    }
    return static_cast<U>(i);                                         // 00482277 (EAX = count)
}

// 00482640 Gob::DeregisterCallback(CallBack* cb)  thiscall RET 4: remove from Gob+190 list, then free(cb).
U __fastcall gob_deregister_callback_00482640(aurora_model::Gob* self, void*, U callback) {
    const I count = field<I>(self, 0x194);                            // 00482641 (read once for the search)
    I i = 0;                                                          // 00482647
    if (count <= 0) return 0;                                         // 00482649..0048264B (EAX = 0)
    const U data = field<U>(self, 0x190);                             // 0048264D
    for (;; ++i) {                                                    // 00482658..00482662
        if (callback == field<U>(obj(data), static_cast<U>(i) * 4u)) break;
        if (i + 1 >= count) return static_cast<U>(i + 1);            // 00482664 (EAX = count)
    }
    const I new_count = field<I>(self, 0x194) - 1;                    // 00482669..0048266F
    field<I>(self, 0x194) = new_count;                                // 00482674
    if (i < new_count) {                                              // 00482672..0048267A
        do {
            const U d = field<U>(self, 0x190);                        // 00482680 (re-read)
            field<U>(obj(d), static_cast<U>(i) * 4u) = field<U>(obj(d), static_cast<U>(i + 1) * 4u);   // 00482686..0048268D
            ++i;                                                      // 00482695
        } while (i < field<I>(self, 0x194));                          // 0048268F..00482698 (re-read count)
    }
    live_free(obj(callback));                                         // 0048269A..004826A0 _free 006FA390
    return kD2Eax;                                                    // D2:EAX (free() result)
}

// 00484D20 Gob::GetAnimation(char* name, float* time_out, float* length_out)  thiscall RET 0Ch.
// Returns the matching out-of-order/current run (Gob+B0 list) or 0.
U __fastcall gob_get_animation_00484d20(aurora_model::Gob* self, void*, U name_word, U time_out, U length_out) {
    const char* const name = static_cast<const char*>(obj(name_word));
    U found = 0;                                                      // 00484D30 ebx
    for (I i = 0; i < field<I>(self, 0xB4); ++i) {                    // 00484D32..00484D65 (count re-read)
        void* const run = obj(element(self, 0xB0, i));                // 00484D40..00484D46
        const char* const anim_name = static_cast<const char*>(at(field<void*>(run, 0), 0x08));   // 00484D49..00484D4B
        if (kotor_stricmp_0070acaf(anim_name, name) == 0) {           // 00484D4E..00484D5A
            found = element(self, 0xB0, i);                           // 00484DB1..00484DB7 (re-read)
            if (found == 0) break;                                    // 00484DBA..00484DBC -> 00484D67
            if (time_out != 0) field<U>(obj(time_out), 0) = field<U>(obj(found), 0x10);   // 00484DBE..00484DC9
            if (length_out != 0)                                      // 00484DCB..00484DD1
                field<U>(obj(length_out), 0) = field<U>(field<void*>(obj(found), 0), 0x50);   // 00484DD3..00484DDA
            return found;                                             // 00484DDD
        }
    }
    void* animation = nullptr;
    void* const add_in = field<void*>(self, 0x64);                    // 00484D67
    if (add_in != nullptr) animation = find_animation_004822a0(add_in, name);   // 00484D6E..00484D75
    if (animation == nullptr)                                         // 00484D78..00484D7A
        animation = find_animation_004822a0(field<void*>(self, 0x58), name);    // 00484D7C..00484D86
    if (time_out != 0) field<U>(obj(time_out), 0) = 0xBF800000u;      // 00484D89..00484D91 (-1.0f)
    if (length_out != 0) {                                            // 00484D97..00484D9D
        if (animation != nullptr) field<U>(obj(length_out), 0) = field<U>(animation, 0x50);   // 00484D9F..00484DA8
        else field<U>(obj(length_out), 0) = 0xBF800000u;              // 00484DE3 (-1.0f)
    }
    return found;                                                     // 00484DAB / 00484DEC (EAX = ebx)
}

// 00485130 Gob::PlayOutOfOrderAnimation(char* name, float* times, float* values, int count, float length)
// thiscall RET 14h.
U __fastcall gob_play_out_of_order_animation_00485130(aurora_model::Gob* self, void*, U name, U times, U values,
    U count, U raw_length) {
    void* const animation = find_animation_004822a0(field<void*>(self, 0x58), static_cast<const char*>(obj(name)));   // 0048514D..00485152
    if (animation == nullptr) return kD2Eax;                          // 0048515C..0048515E (EAX = 0 here)
    void* const memory = live_operator_new(0x34);                     // 00485160..00485162
    void* const run = memory != nullptr                               // 0048516E..00485198 (EH state 0 around ctor)
        ? oo_anim_run_construct_004818c0(memory, nullptr, animation, static_cast<const U*>(obj(times)),
              static_cast<const U*>(obj(values)), static_cast<I>(count), raw_length)
        : nullptr;
    field<U>(run, 0x10) = 0;                                          // 0048519A
    field<U>(run, 0x14) = 0;                                          // 004851A1
    field<U>(run, 0x18) = field<U>(animation, 0x54);                  // 004851A8 / 004851BC
    field<U>(run, 0x08) = 0x209u;                                     // 004851BF
    aurora_list_insert_00445020(list_at(self, 0xB0), nullptr, word_of(run), 0);   // 004851AB..004851C6
    return kD2Eax;                                                    // D2:EAX (Insert result)
}

// 00440890 Gob::LoadAddInAnimations(char* model_name)  thiscall RET 4. Returns 1, or 0 when the .mdl cannot be opened.
U __fastcall gob_load_add_in_animations_00440890(aurora_model::Gob* self, void*, U name_word) {
    const char* const name = static_cast<const char*>(obj(name_word));
    aurora_model::MaxTree* tree = aurora_find_model_00464110(name);   // 00440899..0044089C
    field<void*>(self, 0x64) = tree;                                  // 004408A6
    if (tree == nullptr) {                                            // 004408A4..004408A9
        char path[0x40];                                              // sub esp,40h (00440890)
        char* d = path; const char* s = name;                         // 004408AF..004408BF unbounded byte copy incl. NUL
        do { *d = *s; ++d; } while (*s++ != '\0');
        char* end = path; while (*end != '\0') ++end;                 // 004408C1..004408CC
        static const char kMdl_00740ca8[5] = { '.', 'm', 'd', 'l', '\0' };   // 004408CE..004408E5 dword [00740CA8] + byte [00740CAC]
        std::memcpy(end, kMdl_00740ca8, 5);
        std::FILE* const file = std::fopen(path, "r");                // 004408DF..004408E8 (mode 00740CA4 "r")
        if (file == nullptr) return 0;                                // 004408F0..004408F2 -> 0044094D
        tree = aurora_load_model_00464200(file, nullptr);             // 004408F4..004408F7
        field<void*>(self, 0x64) = tree;                              // 00440905
        maxtree_synchronize_tree_00439d70(tree, nullptr, field<void*>(self, 0x58));   // 004408FC..00440908
        for (I i = 0; i < field<I>(field<void*>(self, 0x58), 0x5C); ++i) {   // 0044090D..00440938 (count re-read)
            void* const model = field<void*>(self, 0x58);             // 00440920
            maxtree_synchronize_tree_00439d70(obj(element(model, 0x58, i)), nullptr, model);   // 00440923..0044092A
        }
    }
    ++field<I>(field<void*>(self, 0x64), 0x54);                       // 0044093A..0044093D
    return 1;                                                         // 00440941
}

// 00487190 Gob::BeginLookAtAnimate(CAurObject* object, char* part_name, char* own_part_name, float a, float b,
// float c, float d)  thiscall RET 1Ch.
U __fastcall gob_begin_look_at_animate_00487190(aurora_model::Gob* self, void*, U object, U part_name,
    U own_part_name, U raw_a, U raw_b, U raw_c, U raw_d) {
    auto* const part = find_part_0043fe00(field<aurora_model::Part*>(obj(object), 0x6C),
        static_cast<const char*>(obj(part_name)));                    // 004871AE..004871B8
    auto* const own_part = find_part_0043fe00(field<aurora_model::Part*>(self, 0x6C),
        static_cast<const char*>(obj(own_part_name)));                // 004871BD..004871CC
    if (part == nullptr || own_part == nullptr) {                     // 004871D6..004871E4
        gob_table(self)->s11c_gob_end_look_at_animate(self, nullptr); // 004872F1..004872F5 call [vtbl+11Ch]
        return kD2Eax;                                                // D2:EAX
    }
    void* look_at = field<void*>(self, 0x174);                        // 004871EA
    if (look_at != nullptr) {                                         // 004871F0..004871F2
        void* const old_run = field<void*>(look_at, 0x0C);            // 004871F4
        if (old_run != nullptr) {                                     // 004871F7..004871F9
            if (exo_array_list_count_004af240(list_at(self, 0xB0), nullptr, old_run) != 0)   // 004871FB..0048720B
                texture_list_remove_00445080(list_at(self, 0xB0), nullptr, word_of(old_run));  // 0048720D..00487210
            live_free(old_run);                                       // 00487215..00487216
            field<void*>(field<void*>(self, 0x174), 0x0C) = nullptr;  // 0048721B / 00487228
        }
        void* const current = field<void*>(self, 0x174);              // 0048722B
        if (current != nullptr) look_at_table(current)->scalar_delete(current, nullptr, 1);   // 00487231..00487239
        field<void*>(self, 0x174) = nullptr;                          // 0048723B
    }
    void* const memory = live_operator_new(0x3C);                     // 00487241..00487243
    look_at = memory != nullptr                                       // 0048724F..00487280 (EH state 0 around ctor)
        ? look_at_animation_construct_00487020(memory, nullptr, obj(object), part, self, own_part,
              raw_a, raw_b, raw_c, raw_d)                             // 00487257..00487279
        : nullptr;
    field<void*>(self, 0x174) = look_at;                              // 00487282
    void* const animation = field<void*>(look_at, 0x08);              // 00487288
    void* const run = live_operator_new(0x24);                        // 0048728B..00487295
    if (run != nullptr) {                                             // 0048729D..0048729F; inline AnimRun init
        field<void*>(run, 0x00) = animation;                          // 004872A6
        field<U>(run, 0x04) = 0x3F800000u;                            // 004872A8 (1.0f)
        field<U>(run, 0x08) = 0;                                      // 004872AB
        field<U>(run, 0x10) = 0;                                      // 004872AE
        field<U>(run, 0x0C) = 0;                                      // 004872B1
        field<U>(run, 0x14) = 0x3F800000u;                            // 004872B4
        field<U>(run, 0x18) = 0;                                      // 004872B7
        field<U>(run, 0x20) = 0;                                      // 004872BA
        field<U>(run, 0x1C) = 0x3F800000u;                            // 004872BD
    }
    field<U>(run, 0x10) = 0;                                          // 004872C6
    field<U>(run, 0x14) = 0;                                          // 004872C9
    field<U>(run, 0x18) = field<U>(animation, 0x54);                  // 004872CC / 004872D7
    field<U>(run, 0x08) = 8;                                          // 004872DA
    aurora_list_insert_00445020(list_at(self, 0xB0), nullptr, word_of(run), 0);   // 004872CF..004872E1
    field<void*>(field<void*>(self, 0x174), 0x0C) = run;              // 004872E6..004872EC
    return kD2Eax;                                                    // D2:EAX
}

// ---------------------------------------------------------------- authored callees

// 00438400 GetMaxID(MdlNode* node, int max)  cdecl: max of the node ids (word +2, sign-extended 00438404) over the
// subtree (children data +2C, count +30). The original is the compiler's 10-level inline expansion of this recursion
// (recursive call at 0043858C, deepest level via CExoArrayList::operator[] 00437C40 = &data[i]); counts and data
// pointers are read once per level, which this recursion also does. Pure function (no stores).
I __cdecl mdl_get_max_id_00438400(void* node, I max_id) {
    const I id = static_cast<I>(field<std::int16_t>(node, 0x02));     // 00438404 movsx
    I result = id > max_id ? id : max_id;                             // 0043840F..00438413
    const I count = field<I>(node, 0x30);                             // 00438415
    if (count <= 0) return result;                                    // 00438418..0043841A
    const U data = field<U>(node, 0x2C);                              // 00438420
    for (I i = 0; i < count; ++i)                                     // 00438430..00438693
        result = mdl_get_max_id_00438400(obj(field<U>(obj(data), static_cast<U>(i) * 4u)), result);
    return result;                                                    // 00438689 / 004386A0
}

// 00439A10 SynchronizeNodes(MdlNode* dst, MdlNode* src, int* next_id)  cdecl, recursive (00439D5F; three further
// levels are inlined, each identical). dst->id = src->id; each dst child whose translated name matches (stricmp)
// a src child is synchronized with it; an unmatched dst child (and not its subtree) gets id = *next_id, ++*next_id.
void __cdecl mdl_synchronize_nodes_00439a10(void* destination, void* source, I* next_id) {
    field<U16>(destination, 0x02) = field<U16>(source, 0x02);         // 00439A13..00439A1F
    for (I i = 0; i < field<I>(destination, 0x30); ++i) {             // 00439A23..00439B30 (count re-read)
        void* const child = obj(element(destination, 0x2C, i));       // 00439A40..00439A43
        bool matched = false;
        for (I j = 0; j < field<I>(source, 0x30); ++j) {              // 00439A46..00439B0B (count re-read)
            void* const other = obj(element(source, 0x2C, j));       // 00439A61..00439A6C
            // 00439A6F..00439AED: TranslateNodeName inlined at the top level (identical to 00438A00, called at the
            // inner levels 00439B72/00439B7A); stricmp(name(dst child), name(src child)).
            const char* const other_name = aurora_node_translate_name_00438a00(static_cast<aurora_model::MdlNode*>(other), nullptr);
            const char* const child_name = aurora_node_translate_name_00438a00(static_cast<aurora_model::MdlNode*>(child), nullptr);
            if (kotor_stricmp_0070acaf(child_name, other_name) == 0) {   // 00439AF5..00439AF7
                mdl_synchronize_nodes_00439a10(child, other, next_id);   // 00439B3E.. (inlined) / 00439D5F
                matched = true;
                break;
            }
        }
        if (!matched) {                                               // 00439B11..00439B1C
            field<U16>(child, 0x02) = static_cast<U16>(*next_id);     // mov cx,word [eax]; mov [ebx+2],cx
            ++*next_id;                                               // inc dword [eax]
        }
    }
}

// 00439D70 MaxTree::SynchronizeTree(MaxTree* other)  thiscall RET 4.
void __fastcall maxtree_synchronize_tree_00439d70(void* tree, void*, void* other) {
    if (other == nullptr) return;                                     // 00439D74..00439D79
    void* const other_root = field<void*>(other, 0x28);               // 00439D7C
    I next_id = mdl_get_max_id_00438400(other_root, -1) + 1;          // 00439D7F..00439D8B (stored over the arg slot)
    mdl_synchronize_nodes_00439a10(field<void*>(tree, 0x28), other_root, &next_id);   // 00439D87..00439D96
    auto* const self_tree = static_cast<aurora_model::MaxTree*>(tree);
    if (aurora_tree_as_animation_0043e1d0(self_tree, nullptr) == nullptr) return;   // 00439D9E..00439DA7
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + 0x8Cu;       // 00439DA9..00439DB7
    auto* const animation = aurora_tree_as_animation_0043e1d0(self_tree, nullptr);   // 00439DBD
    const U frames = field<U>(animation, 0x7C);                       // 00439DC2
    aurora_anim_footprint_007a6990 = aurora_anim_footprint_007a6990 + frames * 36u; // 00439DC5..00439DD0 (lea x9, x4)
    aurora_update_anim_footprint_00437db0(field<aurora_model::MdlNode*>(tree, 0x28), 1);   // 00439DD6..00439DE1
}

// 004818C0 OOAnimRun::OOAnimRun(Animation* animation, float* times, float* values, int count, float length)
// thiscall RET 14h. The original unrolls the copy loop by 4 (0048195C..004819E9) with a scalar tail (00481A10..);
// element results are independent, so one loop in index order is equivalent.
void* __fastcall oo_anim_run_construct_004818c0(void* self, void*, void* animation, const U* times,
    const U* values, I count, U raw_length) {
    field<void*>(self, 0x00) = animation;                             // 004818E0
    field<U>(self, 0x04) = 0x3F800000u;                               // 004818E2 (1.0f)
    field<U>(self, 0x08) = 0;                                         // 004818E5
    field<U>(self, 0x10) = 0;                                         // 004818E8
    field<U>(self, 0x0C) = 0;                                         // 004818EB
    field<U>(self, 0x14) = 0x3F800000u;                               // 004818EE
    field<U>(self, 0x18) = 0;                                         // 004818F1
    field<U>(self, 0x20) = 0;                                         // 004818F4
    field<U>(self, 0x1C) = 0x3F800000u;                               // 004818F7
    const U bytes = static_cast<U>(count) * 4u;                       // 004818D8 lea ebp,[ebx*4]
    field<void*>(self, 0x28) = live_operator_new(bytes);              // 004818DF..00481900
    field<void*>(self, 0x2C) = live_operator_new(bytes);              // 004818FF..00481914
    for (I i = 0; i < count; ++i) {
        // fld dword values[i]; fmul dword animation+50; fstp dword -> [+28][i] (00481960..0048196E / 00481A14..00481A20)
        const double scaled = static_cast<double>(kotor_scene3d::bits_to_float(values[i])) *
                              static_cast<double>(field<float>(animation, 0x50));
        field<float>(field<void*>(self, 0x28), static_cast<U>(i) * 4u) = static_cast<float>(scaled);
        field<U>(field<void*>(self, 0x2C), static_cast<U>(i) * 4u) = times[i];   // 00481971..00481977 / 00481A27..00481A29 (raw dword)
    }
    field<I>(self, 0x30) = count;                                     // 00481A34
    field<U>(self, 0x24) = raw_length;                                // 00481A2F / 00481A37
    return self;                                                      // 00481A3A
}

} // extern "C"
