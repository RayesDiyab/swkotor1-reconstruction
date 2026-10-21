// model_indices.cpp - lane-3d-model: MdlNodeTriMesh::BuildVertexIndices 0043C400 (called from
// PartSkin::PostProcess 00489818) and its two cells.
// PATH NOTE: 007A69A8 lives in .bss and has no writer anywhere in the image (its only references are the two
// reads at 0043C40E / 0043CB62), so the strip branch (AurStripify 00481140 and the NvStripifier closure) never
// runs; it is still a direct call and therefore link-required.
#include "a7e200e217-model_instance.hpp"
#include "2645a3bdc3-nv_strip.hpp"

using namespace aurora_model;

extern "C" {
// cells (original .bss / .data values)
volatile U aurora_use_strips_007a69a8 = 0;          // .bss, no writer in the image
volatile I aurora_min_strip_length_0078e324 = 5;    // .data initial value 5; no writer in the image
// open in this lane (NvStripifier closure)
void __fastcall aurora_trimesh_build_vertex_indices_0043c400(MdlNode* self, void* unused_edx);
}

namespace {
List* list_at(void* p, U off) { return reinterpret_cast<List*>(at(p, off)); }
// Inline CExoArrayList<int>::Add as emitted in this function: capacity cap ? 2*cap : 16, new block through the
// 006FC0D2 operator-new thunk, element copy, old block through the 006FC0CD free thunk.
void add_inline(List* l, U value) {
    if (l->size == l->capacity) {
        const I cap = l->capacity ? l->capacity * 2 : 0x10;
        const U old = l->data;
        l->capacity = cap;
        l->data = addr(live_operator_new(static_cast<U>(cap) * 4));
        for (I i = 0; i < l->size; ++i) ptr<U>(l->data)[i] = ptr<U>(old)[i];
        if (old != 0) live_free(ptr<void>(old));
    }
    ptr<U>(l->data)[l->size] = value;
    l->size = l->size + 1;
}
I face_index(MdlNode* node, I f, I k) {                                             // movsx word [node58 + f*20h + 1Ah + 2k]
    return static_cast<I>(get<I16>(ptr<void>(get<U>(node, 0x58)), static_cast<U>(f) * 0x20 + 0x1A + static_cast<U>(k) * 2));
}
}

// 0043C400 MdlNodeTriMesh::BuildVertexIndices.
//  strips on (007A69A8): gather the face index triples into a local int list, AurStripify it into a list of
//   CExoArrayList<int>; every strip but the last shorter than 0078E324 is unrolled into triangle indices
//   (+F4 list; odd positions reversed), longer ones are copied and kept as strips (+E8 pointers, +100 lengths);
//   the last entry is appended to +F4 whole; [+130] = 4; the strip array is delete[]d (element dtor 0047ABC0).
//  strips off: one strip = a new int[3*faces] copy of the face indices (+E8 via 00437D60, +100 via 005E03C0);
//   [+130] = 3.
//  Then each strip is uploaded to its own index pool (+118 handles) as 16-bit indices and its block freed; with
//  strips on the triangle list goes to pool [+128] (count [+12C]) and +F4 is freed; with strips off the pool
//  handle of each strip is unlocked twice (0043CE77 and 0043CE81) and [+12C] = 0, [+128] = -1.
void __fastcall aurora_trimesh_build_vertex_indices_0043c400(MdlNode* self, void*) {
    if (aurora_use_strips_007a69a8 != 0) {                                          // 0043C40E..0043C427
        List strips{};                                                              // 0043C437..0043C447 ([esp+24h]; EH state 1)
        List triangles{};                                                           // 0043C42F..0043C437 ([esp+30h])
        for (I f = 0; f < get<I>(self, 0x5C); ++f)                                  // 0043C44B..0043C52F
            for (I k = 0; k < 3; ++k)                                               // 0043C469..0043C510
                add_inline(&triangles, static_cast<U>(face_index(self, f, k)));     // 0043C490..0043C508
        aurora_stripify_00481140(&triangles, &strips);                              // 0043C535..0043C53F
        for (I s = 0; s < strips.size - 1; ++s) {                                   // 0043C544..0043C976
            List* const strip = ptr<List>(strips.data) + s;
            const I length = strip->size;                                           // 0043C568
            if (length < aurora_min_strip_length_0078e324) {                        // 0043C56C..0043C572
                for (I k = 0; k < 3; ++k)                                           // 0043C578..0043C626
                    add_inline(list_at(self, 0xF4), ptr<U>((ptr<List>(strips.data) + s)->data)[k]);
                for (I i = 3; i < (ptr<List>(strips.data) + s)->size; ++i) {        // 0043C62C..0043C804 (length re-read)
                    if (i % 2 != 0) {                                               // 0043C650..0043C663 (signed remainder)
                        for (I j = i; j >= i - 2; --j)                              // 0043C669..0043C727
                            add_inline(list_at(self, 0xF4), ptr<U>((ptr<List>(strips.data) + s)->data)[j]);
                    } else {
                        for (I j = i - 2; j <= i; ++j)                              // 0043C732..0043C7E7
                            add_inline(list_at(self, 0xF4), ptr<U>((ptr<List>(strips.data) + s)->data)[j]);
                    }
                }
            } else {
                add_inline(list_at(self, 0x100), static_cast<U>(length));          // 0043C80F..0043C893
                U* const copy = static_cast<U*>(live_operator_new(static_cast<U>((ptr<List>(strips.data) + s)->size) * 4)); // 0043C899..0043C8A5
                for (I i = 0; i < (ptr<List>(strips.data) + s)->size; ++i)          // 0043C8B7..0043C8D5
                    copy[i] = ptr<U>((ptr<List>(strips.data) + s)->data)[i];
                add_inline(list_at(self, 0xE8), addr(copy));                        // 0043C8D7..0043C953
            }
        }
        {
            const List* const last = ptr<List>(strips.data) + (strips.size - 1);   // 0043C97C..0043C987
            for (I k = 0; k < (ptr<List>(strips.data) + (strips.size - 1))->size; ++k) // 0043C98A..0043CA37
                add_inline(list_at(self, 0xF4), ptr<U>(last->data)[k]);
        }
        put<U>(self, 0x130, 4);                                                     // 0043CA3F (EH state 0)
        if (strips.data != 0) {                                                     // 0043CA3D..0043CA4E: delete[] strips.data
            const I count = get<I>(ptr<void>(strips.data - 4), 0);                  // 0043CA50 (array cookie)
            c2da_destroy_array_006fa852(ptr<void>(strips.data), 12, count, &exo_array_list_free_0047abc0); // 0043CA56..0043CA5F (eh vector destructor iterator)
            live_free(ptr<void>(strips.data - 4));                                  // 0043CA64..0043CA65
        }
        if (triangles.data != 0) live_free(ptr<void>(triangles.data));              // 0043CA6D..0043CA82 (EH state -1)
    } else {
        const I count = get<I>(self, 0x5C) * 3;                                     // 0043CA8F..0043CA9D ([esp+1Ch])
        I* const indices = static_cast<I*>(live_operator_new(static_cast<U>(count) * 4)); // 0043CA95..0043CAA1
        for (I f = 0; f < get<I>(self, 0x5C); ++f)                                  // 0043CAA8..0043CAE2
            for (I k = 0; k < 3; ++k) indices[f * 3 + k] = face_index(self, f, k);  // 0043CAC0..0043CAD7
        List* const ptrs = list_at(self, 0xE8);                                     // 0043CAF6
        if (ptrs->size == ptrs->capacity)                                           // 0043CAE8..0043CAFC
            kotor_renderer_gl::allocate_00437d60(ptrs, nullptr, ptrs->capacity ? static_cast<U>(ptrs->capacity) * 2u : 0x10u); // 0043CAFE..0043CB0E
        ptr<U>(ptrs->data)[ptrs->size] = addr(indices);                             // 0043CB13..0043CB18
        ptrs->size = ptrs->size + 1;                                                // 0043CB1B
        List* const lengths = list_at(self, 0x100);                                 // 0043CB2C
        if (lengths->size == lengths->capacity)                                     // 0043CB1E..0043CB32
            menu_list_growth::allocate_005e03c0(lengths, nullptr, lengths->capacity ? static_cast<U>(lengths->capacity) * 2u : 0x10u); // 0043CB34..0043CB44
        ptr<U>(lengths->data)[lengths->size] = static_cast<U>(count);               // 0043CB49..0043CB52
        lengths->size = lengths->size + 1;                                          // 0043CB55
        put<U>(self, 0x130, 3);                                                     // 0043CB58
    }

    const bool strips_on = aurora_use_strips_007a69a8 != 0;                         // 0043CB62..0043CB71 (re-read)
    for (I i = 0; i < get<I>(self, 0xEC); ++i) {                                    // 0043CB77..0043CC97 / 0043CD6D..0043CEA1
        U handle = 0;                                                               // [esp+14h]
        aurora_pool_request_00423ee0(1, ptr<U>(get<U>(self, 0x100))[i] * 2, 2, 0, &handle); // 0043CB81..0043CB98
        const U h = handle;                                                         // 0043CBA9
        add_inline(list_at(self, 0x118), h);                                        // 0043CB9D..0043CC36
        U16* const dst = static_cast<U16*>(aurora_pool_lock_00423f10(handle, 0, 0, 1)); // 0043CC21..0043CC3C
        for (I c = 0; c < static_cast<I>(ptr<U>(get<U>(self, 0x100))[i]); ++c)      // 0043CC41..0043CC70
            dst[c] = static_cast<U16>(ptr<U>(ptr<U>(get<U>(self, 0xE8))[i])[c]);    // 0043CC53..0043CC60 (low word)
        aurora_pool_unlock_00423f30(handle);                                        // 0043CC72..0043CC77
        if (!strips_on) aurora_pool_unlock_00423f30(handle);                        // 0043CE7C..0043CE81 (second unlock, strips-off copy only)
        live_free(ptr<void>(ptr<U>(get<U>(self, 0xE8))[i]));                        // 0043CC7C..0043CC86
    }
    if (strips_on) {
        aurora_pool_request_00423ee0(1, get<U>(self, 0xF8) * 2, 2, 0, ptr<U>(addr(at(self, 0x128)))); // 0043CC9F..0043CCB4
        U16* const dst = static_cast<U16*>(aurora_pool_lock_00423f10(get<U>(self, 0x128), 0, 0, 1)); // 0043CCB9..0043CCC0
        for (I c = 0; c < get<I>(self, 0xF8); ++c)                                  // 0043CCCE..0043CCF7
            dst[c] = static_cast<U16>(ptr<U>(get<U>(self, 0xF4))[c]);
        aurora_pool_unlock_00423f30(get<U>(self, 0x128));                           // 0043CCF9..0043CCFC
        put<U>(self, 0x12C, get<U>(self, 0xF8));                                    // 0043CD01..0043CD07
        if (get<U>(self, 0xE8) != 0) {                                              // 0043CD0D..0043CD18
            live_free(ptr<void>(get<U>(self, 0xE8)));
            put<U>(self, 0xE8, 0);
        }
        put<U>(self, 0xF0, 0);                                                      // 0043CD29
        put<U>(self, 0xEC, 0);                                                      // 0043CD2F
        if (get<U>(self, 0xF4) != 0) {                                              // 0043CD35..0043CD3D
            live_free(ptr<void>(get<U>(self, 0xF4)));
            put<U>(self, 0xF4, 0);
        }
        put<U>(self, 0xFC, 0);                                                      // 0043CD4F
        put<U>(self, 0xF8, 0);                                                      // 0043CD55
        return;
    }
    if (get<U>(self, 0xE8) != 0) {                                                  // 0043CEA7..0043CEB1
        live_free(ptr<void>(get<U>(self, 0xE8)));
        put<U>(self, 0xE8, 0);
    }
    put<U>(self, 0xF0, 0);                                                          // 0043CEC6
    put<U>(self, 0xEC, 0);                                                          // 0043CECC
    put<U>(self, 0x12C, 0);                                                         // 0043CED2
    put<U>(self, 0x128, 0xFFFFFFFFu);                                               // 0043CED9
}
