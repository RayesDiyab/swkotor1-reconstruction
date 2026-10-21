// aur_strip.cpp - lane-3d-model rev2: AurStripify 00481140 (MdlNodeTriMesh::BuildVertexIndices 0043C53F) and the
// CExoArrayList<CExoArrayList<int>> helpers it uses. Never executed at runtime (007A69A8 has no writer);
// link-required. EH unwind states (0071749E, 00715AAB) are cited, not emitted.
#include "3abdd0f0f1-nv_strip.hpp"

using namespace aurora_model;
using nv::Vec;
using nv::Elem24;
using nv::FaceInfo;
using nv::StripInfo;
using nv::Stripifier;

namespace {
// Inline CExoArrayList<int>::Add as emitted here: capacity cap ? 2*cap : 16, operator-new thunk 006FC0D2, element
// copy, free thunk 006FC0CD.
void add_int(List* l, U value) {
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
// inlined vector<unsigned short>::push_back.
void push16(Vec* v, U16 value, U tag) {
    const U16 tmp = value;
    const U size = v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 1) : 0;
    const U cap = v->first ? static_cast<U>(static_cast<I>(v->end - v->first) >> 1) : 0;
    if (v->first != 0 && size < cap) {
        U16* const at_end = ptr<U16>(v->last);
        nv_ushort_uninit_fill_n_0047c1d0(at_end, 1, &tmp, v, tag);
        v->last = addr(at_end + 1);
    } else {
        nv_ushort_vec_insert_n_0047d510(v, nullptr, ptr<U16>(v->last), 1, &tmp);
    }
}
U size24(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) / 24) : 0; }
U size16(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 1) : 0; }
U size32(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 2) : 0; }
}

// 005A76D0 CExoArrayList<T>::CExoArrayList(): data = size = capacity = 0; returns this (eh vector constructor
// element for 0043C260).
void* __fastcall exo_array_list_construct_005a76d0(void* self, void*) {
    List* const l = static_cast<List*>(self);
    l->size = 0;                                                                    // 005A76D2
    l->capacity = 0;                                                                // 005A76D5
    l->data = 0;                                                                    // 005A76D8
    return self;
}

// 00438BB0 CExoArrayList<int>::operator=(other), RET4: size = 0, then Add every element of other (capacity kept).
List* __fastcall exo_int_list_assign_00438bb0(List* self, void*, const List* other) {
    self->size = 0;                                                                 // 00438BBB
    for (I i = 0; i < other->size; ++i)                                             // 00438BBE..00438C35
        add_int(self, ptr<U>(other->data)[i]);                                      // 00438BC4..00438C2C
    return self;
}

// 0043C260 CExoArrayList<CExoArrayList<int>>::Allocate(count), RET4: new[] of count lists (ctor 005A76D0), the old
// lists assigned over (00438BB0), the old array delete[]d (dtor 0047ABC0).
void __fastcall exo_list_list_allocate_0043c260(List* self, void*, I count) {
    const U old = self->data;                                                       // 0043C27F
    self->capacity = count;                                                         // 0043C28C
    unsigned char* const block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(count) * 12 + 4)); // 0043C281..0043C28F (thunk 006FC0D2)
    unsigned char* first = nullptr;                                                 // 0043C29B
    if (block != nullptr) {                                                         // 0043C29D..0043C2A3
        put<I>(block, 0, count);                                                    // 0043C2B6 (array cookie)
        first = block + 4;                                                          // 0043C2B0
        crt_eh_vector_constructor_006fb55f(first, 12, count, &exo_array_list_construct_005a76d0, &exo_array_list_free_0047abc0); // 0043C2A5..0043C2B8
    }
    self->data = addr(first);                                                       // 0043C2C0
    for (I i = 0; i < self->size; ++i)                                              // 0043C2C2..0043C2E8
        exo_int_list_assign_00438bb0(ptr<List>(self->data) + i, nullptr, ptr<List>(old) + i); // 0043C2D2..0043C2DA
    if (old != 0) {                                                                 // 0043C2EA..0043C2EC (delete[] old)
        c2da_destroy_array_006fa852(ptr<void>(old), 12, get<I>(ptr<void>(old - 4), 0), &exo_array_list_free_0047abc0); // 0043C2EE..0043C2FD
        live_free(ptr<void>(old - 4));                                              // 0043C302..0043C303 (thunk 006FC0CD)
    }
}

// 00481140 AurStripify(triangles, strips), cdecl: run NvStripifier::Stripify (cache 16, min strip length 2) on the
// triangle list, then CreateStrips; strips gets one CExoArrayList<int> per strip plus a final one holding the
// leftover triangle list; the NvFaceInfo objects kept in 008302A8 are deleted afterwards.
void __cdecl aurora_stripify_00481140(List* triangles, List* strips) {
    Vec faces{};                                                                    // [esp+48h] vector<24-byte face>
    Vec lengths{};                                                                  // [esp+38h] vector<unsigned short>
    Vec out_strips{};                                                               // [esp+68h] vector<NvStripInfo*>
    Vec strip_indices{};                                                            // [esp+78h] vector<unsigned short>
    Vec out_face_list{};                                                            // [esp+58h] vector<NvFaceInfo*>
    // (0048115E..004811A3: the five vectors zeroed; EH state 4)
    Stripifier* stripifier = static_cast<Stripifier*>(live_operator_new(0x20));     // 004811A9..004811B2
    if (stripifier != nullptr) {                                                    // 004811BA..004811C7 (NvStripifier(): indices empty)
        stripifier->indices.first = 0; stripifier->indices.last = 0; stripifier->indices.end = 0;
    }
    for (I i = 0; i < triangles->size; i += 3) {                                    // 004811D1..0048121F
        Elem24 face{};                                                              // [esp+20h]: v0, v1, v2 (the last three dwords are
        face.w[0] = ptr<U>(triangles->data)[i];                                     //  uninitialised stack in the original and never read)
        face.w[1] = ptr<U>(triangles->data)[i + 1];
        face.w[2] = ptr<U>(triangles->data)[i + 2];
        nv_e24_vec_push_back_0047ecd0(&faces, nullptr, &face);                      // 00481201..00481212
    }
    Vec indices{};                                                                  // 00481221..00481229 ([esp+20h]; EH state 6)
    nv_ushort_vec_reserve_0047db90(&indices, nullptr, size24(&faces) * 3);          // 0048122D..00481260
    for (U i = 0; faces.first != 0 && i < size24(&faces); ++i) {                    // 0048126D..004813DD
        const Elem24* const f = ptr<Elem24>(faces.first) + i;
        push16(&indices, static_cast<U16>(f->w[0]), addr(&indices));        // 004812A3..00481303
        push16(&indices, static_cast<U16>(f->w[1]), addr(&indices));        // 00481303..00481363
        push16(&indices, static_cast<U16>(f->w[2]), addr(&indices));        // 00481363..004813C4
    }
    nv_stripify_00480ef0(stripifier, nullptr, &indices, 0x10, 2, addr(&faces), &out_strips, &out_face_list); // 004813E2..00481400
    nv_create_strips_0047e720(stripifier, nullptr, &out_strips, &strip_indices, &lengths); // 00481405..00481416
    if (stripifier != nullptr) {                                                    // 0048141B..0048143E (delete stripifier)
        if (stripifier->indices.first != 0) live_free(ptr<void>(stripifier->indices.first));
        stripifier->indices.first = 0; stripifier->indices.last = 0; stripifier->indices.end = 0;
        live_free(stripifier);
    }
    const I count = static_cast<I>(size16(&lengths));                               // 00481441..00481455
    const I total = count + 1;                                                      // 0048145E
    if (total == 0) {                                                               // 00481461..00481463
        if (strips->data != 0) {                                                    // 00481465..00481469 (delete[] data)
            c2da_destroy_array_006fa852(ptr<void>(strips->data), 12, get<I>(ptr<void>(strips->data - 4), 0), &exo_array_list_free_0047abc0); // 0048146B..0048147A
            live_free(ptr<void>(strips->data - 4));                                 // 0048147F..00481480
            strips->data = 0;                                                       // 0048148C
        }
        strips->capacity = 0;                                                       // 0048148E
    } else {
        exo_list_list_allocate_0043c260(strips, nullptr, total);                    // 00481497..0048149A
    }
    strips->size = total;                                                           // 0048149F..004814A1
    U running = 0;                                                                  // ebx
    for (U k = 0; lengths.first != 0 && k < size16(&lengths); ++k) {               // 004814A4..0048159E
        const U16 n = ptr<U16>(lengths.first)[k];                                   // 004814CC..004814D7
        for (U m = 0; m < n; ++m)                                                   // 004814E0..0048156F (bound re-read)
            add_int(ptr<List>(strips->data) + k, ptr<U16>(strip_indices.first)[running++]); // 004814E0..00481561
    }
    for (U i = 0; i < size32(&out_face_list); ++i) {                                // 004815B1..0048175B
        const FaceInfo* const f = ptr<FaceInfo>(ptr<U>(out_face_list.first)[i]);   // 004815DB..004815DE
        add_int(ptr<List>(strips->data) + count, static_cast<U>(f->v0));            // 004815E0..0048164A
        add_int(ptr<List>(strips->data) + count, static_cast<U>(ptr<FaceInfo>(ptr<U>(out_face_list.first)[i])->v1)); // 0048164D..004816D1
        add_int(ptr<List>(strips->data) + count, static_cast<U>(ptr<FaceInfo>(ptr<U>(out_face_list.first)[i])->v2)); // 004816D4..00481752
    }
    for (I i = nv_face_list_008302a8.size; i != 0;) {                               // 00481760..00481786 (delete each kept NvFaceInfo)
        --i;
        live_free(ptr<void>(ptr<U>(nv_face_list_008302a8.data)[i]));
    }
    for (U i = 0; i < size32(&out_strips); ++i) {                                   // 00481788..004817CE (delete outStrips[i])
        StripInfo* const s = ptr<StripInfo>(ptr<U>(out_strips.first)[i]);
        if (s == nullptr) continue;
        if (s->faces.first != 0) live_free(ptr<void>(s->faces.first));
        s->faces.first = 0; s->faces.last = 0; s->faces.end = 0;
        live_free(s);
    }
    if (indices.first != 0) live_free(ptr<void>(indices.first));                    // 004817D0..004817DB
    if (out_face_list.first != 0) live_free(ptr<void>(out_face_list.first));        // 004817E3..004817E8
    if (lengths.first != 0) live_free(ptr<void>(lengths.first));                    // 004817F0..004817FB
    if (strip_indices.first != 0) live_free(ptr<void>(strip_indices.first));        // 00481803..0048180C
    if (out_strips.first != 0) live_free(ptr<void>(out_strips.first));              // 00481814..00481819
    if (faces.first != 0) live_free(ptr<void>(faces.first));                        // 00481821..0048182C
}
