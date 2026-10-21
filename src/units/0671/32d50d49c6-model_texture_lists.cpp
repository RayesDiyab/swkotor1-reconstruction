// model_texture_lists.cpp - lane-3d-model: texture back-reference containers used by model loading/destruction:
// SafePointer<CAurTexture> ctor 004204E0 / dtor 00422380, CExoArrayList<SafePointer<CAurTexture>>::allocate
// 0043DE00 / SetSize 0043DF60, Material::ClearTextures 0047B440 (member of the task-17 Material), and the normal path
// of the CRT eh vector constructor iterator 006FB55F (the unwind funclet 006FB5A9 is not modeled, D30).
#include "a7e200e217-model_instance.hpp"
#include "b22a9fca42-material.h"

using namespace aurora_model;

using ElementCtor = void* (__fastcall*)(void* self, void* unused_edx);
using ElementDtor = void (__fastcall*)(void* self, void* unused_edx);
extern "C" {
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, ElementDtor destroy);
void* __fastcall aurora_texture_safe_pointer_construct_004204e0(void* self, void*);
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void* self, void*);

// 006FB55F eh vector constructor iterator(first, size, count, ctor, dtor), __stdcall RET14h: normal path calls ctor on
// each element in order while index < count (signed) (006FB576..006FB58F); dtor is used only by the funclet.
// [M45 compose] 006FB55F body removed: existing M44 provider menu_tables_m41::VectorConstruct_006fb55f (m44-joins alias of this name) wins (definition removed; declaration kept)
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, ElementCtor construct, ElementDtor);

// 004204E0 SafePointer<CAurTexture>::SafePointer: target = 0; returns this.
void* __fastcall aurora_texture_safe_pointer_construct_004204e0(void* self, void*) {
    static_cast<font::Safe*>(self)->target = nullptr;                               // 004204E2
    return self;
}
// 00422380 SafePointer<CAurTexture>::~SafePointer: target -> RemoveReverse32(&target->registrations, this).
void __fastcall aurora_texture_safe_pointer_destroy_00422380(void* self, void*) {
    font::Texture* const target = static_cast<font::Safe*>(self)->target;          // 00422380
    if (target != nullptr) target->registrations.RemoveReverse32(addr(self));       // 00422382..0042238A
}
}

namespace {
// Inlined SafePointer<CAurTexture>::operator= of 0043DE80..0043DF04 (growth cap ? 2*cap : 16 via 005E03C0).
void assign(font::Safe* slot, font::Texture* target) {
    font::Texture* const old = slot->target;                                        // 0043DE86
    if (old != nullptr) old->registrations.RemoveReverse32(addr(slot));            // 0043DE8E..0043DED2
    slot->target = target;                                                          // 0043DED6
    if (target != nullptr) {                                                        // 0043DED8
        List& l = target->registrations;                                            // 0043DEE2
        if (l.size == l.capacity)                                                   // 0043DEDA..0043DEE5
            menu_list_growth::allocate_005e03c0(&l, nullptr, l.capacity ? static_cast<U>(l.capacity) * 2u : 0x10u); // 0043DEE7..0043DEF7
        ptr<U>(l.data)[l.size] = addr(slot);                                        // 0043DEFC..0043DF01
        l.size = l.size + 1;                                                        // 0043DF04
    }
}
}

extern "C" {
// 0043DE00 CExoArrayList<SafePointer<CAurTexture>>::allocate(count), RET4: capacity = count; new[](count*4+4) with
// the element count cookie and the vector constructor (004204E0 / 00422380); copy each old slot's target into the
// new slot (inline operator=, size re-read); destroy the old array (006FA852 with 00422380) and free(old-4).
void __fastcall aurora_texture_ptr_list_allocate_0043de00(void* self, void*, I count) {
    List* const list = static_cast<List*>(self);
    const U old = list->data;                                                       // 0043DE1F
    list->capacity = count;                                                         // 0043DE2F
    unsigned char* const block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(count) * 4u + 4u)); // 0043DE22..0043DE32 (006FC0D2)
    U first = 0;
    if (block != nullptr) {                                                         // 0043DE40..0043DE46
        put<I>(block, 0, count);                                                    // 0043DE59
        first = addr(block + 4);                                                    // 0043DE53
        crt_eh_vector_constructor_006fb55f(block + 4, 4, count, &aurora_texture_safe_pointer_construct_004204e0,
                                           &aurora_texture_safe_pointer_destroy_00422380); // 0043DE48..0043DE5B
    }
    list->data = first;                                                             // 0043DE6F
    for (I i = 0; i < list->size; ++i) {                                            // 0043DE60..0043DF19
        font::Safe* const slot = ptr<font::Safe>(list->data) + i;                   // 0043DE80..0043DE8C
        font::Texture* const target = ptr<font::Safe>(old)[i].target;               // 0043DE89
        assign(slot, target);
    }
    if (old != 0) {                                                                 // 0043DF1F..0043DF21
        c2da_destroy_array_006fa852(ptr<void>(old), 4, ptr<I>(old)[-1], &aurora_texture_safe_pointer_destroy_00422380); // 0043DF23..0043DF32
        live_free(ptr<void>(old - 4));                                              // 0043DF37..0043DF38 (006FC0CD)
    }
}
// 0043DF60 ::SetSize(n), RET4: n == 0 -> destroy+free the array (data = 0), size = 0, capacity = 0; else allocate(n),
// size = n.
void __fastcall aurora_texture_ptr_list_set_size_0043df60(void* self, void*, I count) {
    List* const list = static_cast<List*>(self);
    if (count == 0) {                                                               // 0043DF65..0043DF6A
        const U data = list->data;                                                  // 0043DF6C
        if (data != 0) {                                                            // 0043DF6E..0043DF70
            c2da_destroy_array_006fa852(ptr<void>(data), 4, ptr<I>(data)[-1], &aurora_texture_safe_pointer_destroy_00422380); // 0043DF72..0043DF82
            live_free(ptr<void>(data - 4));                                         // 0043DF87..0043DF88
            list->data = 0;                                                         // 0043DF90
        }
        list->size = 0;                                                             // 0043DF93
        list->capacity = 0;                                                         // 0043DF96
        return;
    }
    aurora_texture_ptr_list_allocate_0043de00(self, nullptr, count);               // 0043DFA2..0043DFA5
    list->size = count;                                                             // 0043DFAA
}
}

// 0047B440 Material::ClearTextures (task-17 nominal Material, member declared in lane-texture-epoch material.h):
// for each of the seven Safe slots in order, a non-null target is unlinked from its registrations (00445080),
// the slot cleared, then AurTextureRelease 00420D70(target).
void __thiscall material_owner::Material::clear_textures_0047b440() {
    for (int i = 0; i < 7; ++i) {                                                   // 0047B444..0047B552 (unrolled)
        font::Texture* const target = textures[i].target;
        if (target == nullptr) continue;
        target->registrations.RemoveReverse32(addr(&textures[i]));                  // e.g. 0047B44A..0047B44E
        textures[i].target = nullptr;                                               // e.g. 0047B454
        font::release_00420d70(target);                                             // e.g. 0047B453..0047B45A
    }
}
