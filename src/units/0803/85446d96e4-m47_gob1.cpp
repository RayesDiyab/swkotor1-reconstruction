// lane-m47-3d fork gob1: part-tree walkers called by the core46 Gob slot bodies (consumers core46-gob-tree,
// core46-gob-small, core46-gob-hit). Exact symbols/ABI = the declarations in stage 3d/core46/m46_gob_externs.h
// (included, so a mismatch fails to compile).
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1, image base 00400000); instruction addresses cited per behaviour.
//
// Structure note (all walkers): the original compiler inlined the recursive body into itself N levels deep and
// only the innermost level calls the function again. Every level performs the same visit (same callee, same order,
// same live re-reads of the child list {data +30h, count +34h} per iteration). The bodies below reproduce the exact
// per-level callee choice with an explicit `depth` (level 0 = the function's own node, deepest level calls the
// exported function), so the sequence of external calls is identical to the original's:
//   - level 0 of the "append" walkers uses the inlined CExoArrayList::Add (growth via 005E03C0 directly);
//     inner levels call CExoArrayList<int>::Add 00444FE0 (identical growth rule, 00444FE0..00445010).
//   - child access at the deepest level goes through CExoArrayList<Part*>::operator[] 00437C40 where the original
//     does so; elsewhere the original indexes [data+i*4] directly.
// Part virtual calls go through the published Part-family table type (model_vtables.hpp VTable_00740ae8; every
// Part-family table has the same Slot0 type at +24 / +2C) - D21/D41, no function-pointer casts.
#include "fb4ea0259f-m46_gob_externs.h"

using namespace m46_gob;

extern "C" {
// ---- existing M46 providers (identical declarations to their owners)
// 00437C40 CExoArrayList<Part*>::operator[] (3d/model model_postprocess.cpp:35 / model_tree.cpp:17).
U* __fastcall part_list_at_00437c40(kotor_scene3d::PointerList* self, void* unused_edx, I index);
// 0046BD10 CExoArrayList::AddUnique (RET 4): identical to 3d/emitter emitter.h:194 / core46 m46_scene_externs_c2.h:19
// (M46 join /alternatename -> texture_join::add_unique_0046bd10).
void __fastcall exo_array_list_add_unique_0046bd10(kotor_scene3d::PointerList* list, void* unused_edx, U value);
// ---- new undefined external authored in this lane (m47_gob1_tex.cpp)
// 0047AF10 Material::SetDownSampling(int) (thiscall, RET 4) -> D1 shape.
void __fastcall material_set_down_sampling_0047af10(void* material, void* unused_edx, I value);
}

namespace {
using kotor_scene3d::PointerList;
inline PointerList* children(void* part) { return static_cast<PointerList*>(at(part, 0x30)); }   // +30 data, +34 count
inline I child_count(void* part) { return children(part)->size; }                           // [part+34h] (re-read)
inline void* child(void* part, I i) {                                                       // mov eax,[p+30h]; [eax+i*4]
    return ptr<void>(static_cast<const U*>(ptr<void>(children(part)->data))[i]);
}
inline void* child_via_operator(void* part, I i) {                                          // 00437C40 then [eax]
    return ptr<void>(*part_list_at_00437c40(children(part), nullptr, i));
}
inline U part_slot24(void* part) { return table<PartTable>(part)->s024(part, nullptr); }   // call [eax+24h]
inline U part_slot2c(void* part) { return table<PartTable>(part)->s02c(part, nullptr); }   // call [eax+2Ch]
// Inlined CExoArrayList::Add (e.g. 00445AFB..00445B20): growth to capacity*2 (8 when 0) via 005E03C0, then store.
inline void add_inline(PointerList* list, U value) {
    const I capacity = list->capacity;
    if (list->size == capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    static_cast<U*>(ptr<void>(list->data))[list->size] = value;
    list->size = list->size + 1;
}
// ---- findalllightparts 00445AE0 (levels 0..10; level-10 children -> 00437C40 + recursive call 00445D17..00445D27)
void lights_level(void* part, PointerList* out, int depth) {
    const U light = part_slot24(part);                         // 00445AEE / 00445B4F / ... / 00445CFA call [edx+24h]
    if (light != 0) {
        if (depth == 0) add_inline(out, light);                // 00445AFB..00445B20
        else int_list_add_00444fe0(out, nullptr, light);       // 00445B56..00445B59 (and each inner level)
    }
    for (I i = 0; i < child_count(part); ++i) {                // count re-read each iteration (e.g. 00445D2C..00445D35)
        if (depth == 10) find_all_light_parts_00445ae0(child_via_operator(part, i), out);   // 00445D17..00445D27
        else lights_level(child(part, i), out, depth + 1);     // 00445B40..00445B43 etc.
    }
}
// ---- findalltrimeshparts 004461D0: same shape as 00445AE0 with slot +2C (004461DE ... 004463BE), recursion 0044640A..00446417
void trimesh_level(void* part, PointerList* out, int depth) {
    const U mesh = part_slot2c(part);
    if (mesh != 0) {
        if (depth == 0) add_inline(out, mesh);                 // 004461EB..00446210
        else int_list_add_00444fe0(out, nullptr, mesh);        // 00446244.. (00444FE0 at levels 1..10)
    }
    for (I i = 0; i < child_count(part); ++i) {
        if (depth == 10) find_all_tri_mesh_parts_004461d0(child_via_operator(part, i), out);   // 0044640A..00446417
        else trimesh_level(child(part, i), out, depth + 1);
    }
}
// ---- findallmaterials 00445EC0: [part+44h] (Material*) -> CExoArrayList::AddUnique 0046BD10 at every level
//      (00445EC9..00445ED5, ..., 004460A6); levels 0..10; recursion via 00437C40 (004460BA..004460C7).
void materials_level(void* part, PointerList* out, int depth) {
    const U material = field<U>(part, 0x44);                   // 00445EC9 mov eax,[edi+44h]
    if (material != 0) exo_array_list_add_unique_0046bd10(out, nullptr, material);   // 00445ECC..00445ED5
    for (I i = 0; i < child_count(part); ++i) {
        if (depth == 10) find_all_materials_00445ec0(child_via_operator(part, i), out);   // 004460BA..004460C7
        else materials_level(child(part, i), out, depth + 1);  // 00445EF0..00445EF3
    }
}
// ---- PartSetFog 00440550: mesh = part->+2C; if mesh: byte [mesh+7Dh] = low byte of arg (00440564..00440568);
//      levels 0..10; recursion via 00437C40 (00440757..00440764).
void fog_level(void* part, U enable, int depth) {
    const U mesh = part_slot2c(part);                          // 0044055D call [eax+2Ch]
    if (mesh != 0) field<U8>(ptr<void>(mesh), 0x7D) = static_cast<U8>(enable);   // 00440564 mov cl,[esp+58h]; 00440568
    for (I i = 0; i < child_count(part); ++i) {
        if (depth == 10) part_set_fog_00440550(child_via_operator(part, i), enable);   // 00440757..00440764
        else fog_level(child(part, i), enable, depth + 1);     // 00440580..00440583
    }
}
// ---- PartSetCanDownSample 004434A0: node = [part+4]; if ((node[0] & 21h) == 21h && node[A8h] != 0 &&
//      repe cmpsb(node+A8h, "NULL" 0073EE04, 5) differs) { Material::SetDownSampling([part+44h], movsx byte arg);
//      n = 1 } ; n += child results; return n. Levels 0..5; level-5 children: direct call 00443700..0044370C.
constexpr char kNull_0073ee04[5] = {'N', 'U', 'L', 'L', '\0'};   // .rdata 0073EE04 "NULL"
// repe cmps byte, ecx=5 (004434D4..004434DD): stops at the first differing byte (never reads past it).
bool equals_null5(const U8* text) {
    for (int k = 0; k < 5; ++k)
        if (text[k] != static_cast<U8>(kNull_0073ee04[k])) return false;
    return true;
}
I down_sample_level(void* part, U flag, int depth) {
    I count = 0;                                               // 004434AE/004434B8 ([esp+10h] = 0)
    const U8* node = field<const U8*>(part, 0x04);             // 004434A8 mov eax,[ebx+4]
    if ((node[0] & 0x21) == 0x21 && node[0xA8] != 0 && !equals_null5(node + 0xA8)) {   // 004434AB..004434DD
        material_set_down_sampling_0047af10(field<void*>(part, 0x44), nullptr,
                                            static_cast<I>(static_cast<signed char>(static_cast<U8>(flag))));   // 004434DF..004434E8 movsx
        count = 1;                                             // 004434ED
    }
    for (I i = 0; i < child_count(part); ++i) {                // 004434F5.. / 004437C4..004437D2 (count re-read)
        if (depth == 5) count += static_cast<I>(part_set_can_down_sample_004434a0(child(part, i), flag));   // 00443700..00443711
        else count += down_sample_level(child(part, i), flag, depth + 1);   // 00443510..00443513, sums 0044372D/0044374F/...
    }
    return count;                                              // 004437D8 mov eax,[esp+10h]
}
// ---- Gob::EnumParts 004474D0 (original stdcall-like: stack part/callback/out, RET 0Ch; ECX saved at 004474DA and
//      re-passed to the recursive call 004476E9 but otherwise unused -> not observable, not reproduced).
//      if (!part) return; if (!callback || callback(part)) add part; recurse non-null-checked children.
//      Levels 0..8; level-8 children: direct call (004476E0..004476ED).
using PartEnumerator = U (__cdecl*)(void* part);              // `push esi; call ebp; add esp,4` (0044751D..00447520)
void enum_level(void* part, PartEnumerator callback, PartEnumerator callback_word, PointerList* out, int depth) {
    if (callback == nullptr) {                                 // 004474EE / 00447558 test ebp,ebp
        if (depth == 0) add_inline(out, word(part));           // 004474F3..00447518
        else int_list_add_00444fe0(out, nullptr, word(part));  // 00447567..00447569
    } else if (callback(part) != 0) {                          // 0044751D..00447525
        if (depth == 0) int_list_add_00444fe0(out, nullptr, word(part));   // 00447527..0044752A
        else int_list_add_00444fe0(out, nullptr, word(part));  // 00447566..00447569
    }
    for (I i = 0; i < child_count(part); ++i) {                // 0044752F.. (count re-read 00447791..00447798)
        void* const c = child(part, i);                        // 00447546..00447549
        if (depth == 8) {
            (void)enum_parts_004474d0(c, callback_word, word(out));   // 004476E0..004476ED (null child returns at 004474D8)
        } else {
            if (c == nullptr) continue;                        // 0044754C..00447552 je
            enum_level(c, callback, callback_word, out, depth + 1);
        }
    }
}
}

extern "C" {
// 00445AE0 findalllightparts(Part*, CExoArrayList<PartLight*>*) cdecl, plain RET (00445E26).
void __cdecl find_all_light_parts_00445ae0(void* part, PointerList* out) { lights_level(part, out, 0); }
// 004461D0 findalltrimeshparts(Part*, CExoArrayList<PartTriMesh*>*) cdecl, plain RET (00446516).
void __cdecl find_all_tri_mesh_parts_004461d0(void* part, PointerList* out) { trimesh_level(part, out, 0); }
// 00445EC0 findallmaterials(Part*, CExoArrayList<Material*>*) cdecl, plain RET (004461C6).
void __cdecl find_all_materials_00445ec0(void* part, PointerList* out) { materials_level(part, out, 0); }
// 00440550 PartSetFog(Part*, bool) cdecl, plain RET (00440863); the callee reads only the low byte (00440564).
void __cdecl part_set_fog_00440550(void* part, U enable) { fog_level(part, enable, 0); }
// 004434A0 PartSetCanDownSample(Part*, char) cdecl -> int count (EAX 004437D8), plain RET (004437E3).
U __cdecl part_set_can_down_sample_004434a0(void* part, U flag) { return static_cast<U>(down_sample_level(part, flag, 0)); }
// 004474D0 Gob::EnumParts(Part*, PartEnumerator*, CExoArrayList<Part*>*) RET 0Ch (004477A5).
// The callback arrives as the raw DWORD word the consumer passes (address of a cdecl predicate, e.g. 0043EE40);
// it is converted once from that integer word to its cdecl type (integer -> function pointer; no function-pointer
// to function-pointer cast). EAX at the original `ret` is incidental (entry EAX / last count or list word, never
// read by callers: 004477BE `ret 8` passes it through to Gob::EnumParts whose callers discard it) -> D2: returns 0.
U __stdcall enum_parts_004474d0(void* part, PartEnumerator callback, U out) {   // D75 typed callback
    if (part == nullptr) return 0;                             // 004474D8..004474DE (D2: EAX incidental)
    PartEnumerator const fn = callback;   // ebp (004474EA); D75: typed parameter, no integer->function-pointer conversion
    enum_level(part, fn, callback, ptr<PointerList>(out), 0);
    return 0;                                                  // D2 label 004477A5: EAX incidental
}
// 0043EE40 PartHasMaterial(CAurPart*) cdecl: xor eax,eax; test [p+44h]; setne al (0043EE44..0043EE4B); RET.
U __cdecl part_has_material_0043ee40(void* part) { return field<U>(part, 0x44) != 0 ? 1u : 0u; }
}
