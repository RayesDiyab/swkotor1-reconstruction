// nv_stl.cpp - lane-3d-model rev2: the VC7.1 standard-library instantiations used by the NvTriStrip code
// (std::vector<unsigned short>, std::vector<T*>, std::vector<24-byte element>, std::set<T*>).
// Each function is the original out-of-line instantiation; inlined size()/capacity()/iterator arithmetic of the
// original appears as local expressions. EH: the catch(...) { deallocate; throw; } funclets of the _Insert_n /
// reserve bodies (e.g. 0047DFA3) and the unwind states are cited, not emitted. The trailing "allocator" and
// empty iterator-tag arguments of the _Uninit_* helpers carry no value; they are passed as observed where cheap.
// EXCEPTIONS: _Xlen (0047D2B0) and the two _Tree throw sites build std::length_error / std::out_of_range with the
// original messages ("vector<T> too long" 0074318C, "map/set<T> too long" 007431A0, "invalid map/set<T>
// iterator" 007431B4). The original constructs them through its own std::string / logic_error members
// (0047D010, 0047CF60, 0047CAF0, 0047CEA0, 0047BF10, 0047D120, 0047D220/0047D290) and throws with ThrowInfo
// 007640D0 / 00764310; here the toolchain C++ runtime provides those classes (open gate G-NV-EXC).
#include "5b3fc79ff6-nv_stl.hpp"
#include <stdexcept>

using namespace aurora_model;
using nv::Vec;
using nv::Node;
using nv::Tree;
using nv::Elem24;

namespace {
U size16(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 1) : 0; }
U cap16(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->end - v->first) >> 1) : 0; }
U size32(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) >> 2) : 0; }
U cap32(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->end - v->first) >> 2) : 0; }
U size24(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->last - v->first) / 24) : 0; }
U cap24(const Vec* v) { return v->first ? static_cast<U>(static_cast<I>(v->end - v->first) / 24) : 0; }
U16* f16(U a) { return ptr<U16>(a); }
U* f32(U a) { return ptr<U>(a); }
Elem24* f24(U a) { return ptr<Elem24>(a); }
bool isnil(const Node* n) { return n->isnil != 0; }
}

// ======================================================================= std::vector<unsigned short>
// 0047BE70 _Uninit_copy(first, last, dest, alloc, tag): construct-copy (null-checked destination); returns end.
U16* __cdecl nv_ushort_uninit_copy_0047be70(const U16* first, const U16* last, U16* dest, void*, U) {
    for (; first != last; ++first, ++dest)                                          // 0047BE78..0047BE93
        if (dest != nullptr) *dest = *first;
    return dest;
}
// 0047C1D0 _Uninit_fill_n(dest, count, value, alloc, tag).
void __cdecl nv_ushort_uninit_fill_n_0047c1d0(U16* dest, U count, const U16* value, void*, U) {
    for (; count != 0; --count, ++dest)                                             // 0047C1D4..0047C1F1
        if (dest != nullptr) *dest = *value;
}
// 0047CC30 copy(first, last, dest): returns end.
U16* __cdecl nv_ushort_copy_0047cc30(const U16* first, const U16* last, U16* dest) {
    for (; first != last; ++first, ++dest) *dest = *first;                          // 0047CC38..0047CC4F
    return dest;
}
// 0047BDE0 fill(first, last, value).
void __cdecl nv_ushort_fill_0047bde0(U16* first, U16* last, const U16* value) {
    for (; first != last; ++first) *first = *value;                                 // 0047BDE8..0047BDFC
}
// 0047C0E0 copy_backward(result*, first, last, dest): *result = start of the copied range.
U16** __cdecl nv_ushort_copy_backward_0047c0e0(U16** result, U16* first, U16* last, U16* dest) {
    while (last != first) *--dest = *--last;                                        // 0047C0E8..0047C100
    *result = dest;                                                                 // 0047C102..0047C106
    return result;
}
// 0047CC60 / 0047CC80 vector<unsigned short>::_Ucopy(first, last, dest) (two identical instantiations).
U16* __fastcall nv_ushort_vec_ucopy_0047cc60(Vec* self, void*, const U16* first, const U16* last, U16* dest) {
    return nv_ushort_uninit_copy_0047be70(first, last, dest, self, addr(dest));    // 0047CC60..0047CC75
}
U16* __fastcall nv_ushort_vec_ucopy_0047cc80(Vec* self, void*, const U16* first, const U16* last, U16* dest) {
    return nv_ushort_uninit_copy_0047be70(first, last, dest, self, addr(dest));    // 0047CC80..0047CC9D
}
// 0047CE40 vector<unsigned short>::_Ufill(dest, count, value): returns dest + count.
U16* __fastcall nv_ushort_vec_ufill_0047ce40(Vec* self, void*, U16* dest, U count, const U16* value) {
    nv_ushort_uninit_fill_n_0047c1d0(dest, count, value, self, addr(value));       // 0047CE40..0047CE57
    return dest + count;                                                            // 0047CE5F
}
// 0047D510 vector<unsigned short>::_Insert_n(where, count, value).
void __fastcall nv_ushort_vec_insert_n_0047d510(Vec* self, void*, U16* where, U count, const U16* value) {
    U16 tmp = *value;                                                               // 0047D52B..0047D540
    const U capacity = cap16(self);                                                 // 0047D538..0047D54E
    if (count == 0) return;                                                         // 0047D550..0047D555
    if (0x7FFFFFFFu - size16(self) < count) nv_vector_xlen_0047d2b0(self, nullptr); // 0047D55B..0047D577
    if (capacity < size16(self) + count) {                                          // 0047D58F..0047D5A2
        U grown = (0x7FFFFFFFu - capacity / 2 < capacity) ? 0 : capacity + capacity / 2; // 0047D5A8..0047D5BB
        if (grown < size16(self) + count) grown = size16(self) + count;             // 0047D5BD..0047D5E1
        U16* const fresh = static_cast<U16*>(live_operator_new(grown * 2));         // 0047D5E3..0047D5E9
        U16* p = nv_ushort_uninit_copy_0047be70(f16(self->first), where, fresh, self, count); // 0047D5F4..0047D613
        nv_ushort_uninit_fill_n_0047c1d0(p, count, &tmp, self, count);              // 0047D620..0047D62B
        nv_ushort_uninit_copy_0047be70(where, f16(self->last), p + count, self, count); // 0047D630..0047D64F
        count += size16(self);                                                      // 0047D654..0047D66C
        if (self->first != 0) live_free(f16(self->first));                          // 0047D66E..0047D673
        self->end = addr(fresh) + grown * 2;                                        // 0047D67B..0047D683
        self->last = addr(fresh + count);                                           // 0047D680..0047D686
        self->first = addr(fresh);                                                  // 0047D689
    } else if (static_cast<U>(static_cast<I>(self->last - addr(where)) >> 1) < count) { // 0047D6B4..0047D6C2
        nv_ushort_vec_ucopy_0047cc80(self, nullptr, where, f16(self->last), where + count); // 0047D6C4..0047D6DF
        nv_ushort_vec_ufill_0047ce40(self, nullptr, f16(self->last),
                                     count - static_cast<U>(static_cast<I>(self->last - addr(where)) >> 1), &tmp); // 0047D6E4..0047D701
        self->last = self->last + count * 2;                                        // 0047D706..0047D711
        nv_ushort_fill_0047bde0(where, f16(self->last) - count, &tmp);              // 0047D714..0047D787
    } else {
        U16* const old_end = f16(self->last);                                       // 0047D720
        self->last = addr(nv_ushort_vec_ucopy_0047cc80(self, nullptr, old_end - count, old_end, old_end)); // 0047D720..0047D745
        U16* moved = nullptr;
        nv_ushort_copy_backward_0047c0e0(&moved, where, old_end - count, old_end);  // 0047D744..0047D766
        nv_ushort_fill_0047bde0(where, where + count, &tmp);                        // 0047D76E..0047D787
    }
}
// 0047CBC0 vector<unsigned short>::erase(result*, first, last).
U16** __fastcall nv_ushort_vec_erase_0047cbc0(Vec* self, void*, U16** result, U16* first, U16* last) {
    if (first != last) {                                                            // 0047CBC7..0047CBCD
        U16* p = first;
        for (U16* q = last; q != f16(self->last); ++q, ++p) *p = *q;               // 0047CBCF..0047CBEE
        self->last = addr(p);                                                       // 0047CBF0
    }
    *result = first;                                                                // 0047CBF3..0047CBF7
    return result;
}
// 0047DB90 vector<unsigned short>::reserve(count).
void __fastcall nv_ushort_vec_reserve_0047db90(Vec* self, void*, U count) {
    if (count > 0x7FFFFFFFu) nv_vector_xlen_0047d2b0(self, nullptr);               // 0047DBAB..0047DBBE
    if (cap16(self) >= count) return;                                               // 0047DBD6..0047DBEC
    U16* const fresh = static_cast<U16*>(live_operator_new(count * 2));             // 0047DBEE..0047DBF5
    nv_ushort_uninit_copy_0047be70(f16(self->first), f16(self->last), fresh, self, count * 2); // 0047DBFA..0047DC1B
    const U size = size16(self);                                                    // 0047DC20..0047DC2F
    if (self->first != 0) live_free(f16(self->first));                              // 0047DC31..0047DC36
    self->first = addr(fresh);                                                      // 0047DC44
    self->last = addr(fresh + size);                                                // 0047DC41..0047DC49
    self->end = addr(fresh) + count * 2;                                            // 0047DC3E..0047DC50
}
// 0047DD40 vector<unsigned short>::resize(count, value).
void __fastcall nv_ushort_vec_resize_0047dd40(Vec* self, void*, U count, U16 value) {
    if (size16(self) < count) {                                                     // 0047DD41..0047DD5A
        nv_ushort_vec_insert_n_0047d510(self, nullptr, f16(self->last), count - size16(self), &value); // 0047DD5C..0047DD7B
    } else if (self->first != 0 && count < size16(self)) {                          // 0047DD85..0047DD94
        U16* r = nullptr;
        nv_ushort_vec_erase_0047cbc0(self, nullptr, &r, f16(self->first) + count, f16(self->last)); // 0047DD96..0047DDAB
    }
}
// 0047E5B0 vector<unsigned short>::push_back(value).
void __fastcall nv_ushort_vec_push_back_0047e5b0(Vec* self, void*, const U16* value) {
    if (self->first != 0 && size16(self) < cap16(self)) {                           // 0047E5B3..0047E5D3
        U16* const at_end = f16(self->last);
        nv_ushort_uninit_fill_n_0047c1d0(at_end, 1, value, self, addr(value));      // 0047E5D5..0047E5E6
        self->last = addr(at_end + 1);                                              // 0047E5EE..0047E5F1
        return;
    }
    nv_ushort_vec_insert_n_0047d510(self, nullptr, f16(self->last), 1, value);     // 0047E5F9..0047E60A
}

// ======================================================================= std::vector<T*>
// 0047BE40 _Uninit_copy for 4-byte elements.
U* __cdecl nv_ptr_uninit_copy_0047be40(const U* first, const U* last, U* dest, void*, U) {
    for (; first != last; ++first, ++dest)                                          // 0047BE48..0047BE61
        if (dest != nullptr) *dest = *first;
    return dest;
}
// 0047C1A0 _Uninit_fill_n for 4-byte elements.
void __cdecl nv_ptr_uninit_fill_n_0047c1a0(U* dest, U count, const U* value, void*, U) {
    for (; count != 0; --count, ++dest)                                             // 0047C1A4..0047C1BF
        if (dest != nullptr) *dest = *value;
}
// 0047C110 fill for 4-byte elements.
void __cdecl nv_ptr_fill_0047c110(U* first, U* last, const U* value) {
    for (; first != last; ++first) *first = *value;                                 // 0047C118..0047C12A
}
// 0047C0B0 copy_backward for 4-byte elements.
U** __cdecl nv_ptr_copy_backward_0047c0b0(U** result, U* first, U* last, U* dest) {
    while (last != first) *--dest = *--last;                                        // 0047C0B8..0047C0CE
    *result = dest;                                                                 // 0047C0D0..0047C0D4
    return result;
}
// 0047CC00 vector<T*>::_Ucopy.
U* __fastcall nv_ptr_vec_ucopy_0047cc00(Vec* self, void*, const U* first, const U* last, U* dest) {
    return nv_ptr_uninit_copy_0047be40(first, last, dest, self, addr(dest));       // 0047CC00..0047CC1D
}
// 0047CE10 vector<T*>::_Ufill.
U* __fastcall nv_ptr_vec_ufill_0047ce10(Vec* self, void*, U* dest, U count, const U* value) {
    nv_ptr_uninit_fill_n_0047c1a0(dest, count, value, self, addr(value));          // 0047CE10..0047CE27
    return dest + count;                                                            // 0047CE2F
}
// 0047DE10 vector<T*>::_Insert_n(where, count, value).
void __fastcall nv_ptr_vec_insert_n_0047de10(Vec* self, void*, U* where, U count, const U* value) {
    U tmp = *value;                                                                 // 0047DE2B..0047DE3D
    const U capacity = cap32(self);                                                 // 0047DE35..0047DE4B
    if (count == 0) return;                                                         // 0047DE4E..0047DE53
    if (0x3FFFFFFFu - size32(self) < count) nv_vector_xlen_0047d2b0(self, nullptr); // 0047DE59..0047DE76
    if (capacity < size32(self) + count) {                                          // 0047DE8E..0047DEA2
        U grown = (0x3FFFFFFFu - capacity / 2 < capacity) ? 0 : capacity + capacity / 2; // 0047DEA8..0047DEBB
        if (grown < size32(self) + count) grown = size32(self) + count;             // 0047DEBD..0047DEE3
        U* const fresh = static_cast<U*>(live_operator_new(grown * 4));             // 0047DEE5..0047DEEC
        U* p = nv_ptr_uninit_copy_0047be40(f32(self->first), where, fresh, self, count); // 0047DEF1..0047DF16
        nv_ptr_uninit_fill_n_0047c1a0(p, count, &tmp, self, count);                 // 0047DF1B..0047DF2E
        nv_ptr_uninit_copy_0047be40(where, f32(self->last), p + count, self, count); // 0047DF33..0047DF52
        count += size32(self);                                                      // 0047DF57..0047DF70
        if (self->first != 0) live_free(f32(self->first));                          // 0047DF72..0047DF77
        self->end = addr(fresh) + grown * 4;                                        // 0047DF7F..0047DF87
        self->last = addr(fresh + count);                                           // 0047DF84..0047DF8A
        self->first = addr(fresh);                                                  // 0047DF8D
    } else if (static_cast<U>(static_cast<I>(self->last - addr(where)) >> 2) < count) { // 0047DFB8..0047DFC7
        nv_ptr_vec_ucopy_0047cc00(self, nullptr, where, f32(self->last), where + count); // 0047DFC9..0047DFE8
        nv_ptr_vec_ufill_0047ce10(self, nullptr, f32(self->last),
                                  count - static_cast<U>(static_cast<I>(self->last - addr(where)) >> 2), &tmp); // 0047DFED..0047E00B
        self->last = self->last + count * 4;                                        // 0047E010..0047E01B
        nv_ptr_fill_0047c110(where, f32(self->last) - count, &tmp);                 // 0047E01E..0047E092
    } else {
        U* const old_end = f32(self->last);                                         // 0047E02A
        self->last = addr(nv_ptr_vec_ucopy_0047cc00(self, nullptr, old_end - count, old_end, old_end)); // 0047E02A..0047E050
        U* moved = nullptr;
        nv_ptr_copy_backward_0047c0b0(&moved, where, old_end - count, old_end);     // 0047E04F..0047E071
        nv_ptr_fill_0047c110(where, where + count, &tmp);                           // 0047E079..0047E092
    }
}
// 0047D0B0 vector<T*>::erase(result*, first, last).
U** __fastcall nv_ptr_vec_erase_0047d0b0(Vec* self, void*, U** result, U* first, U* last) {
    if (first != last) {                                                            // 0047D0B7..0047D0BD
        U* p = first;
        for (U* q = last; q != f32(self->last); ++q, ++p) *p = *q;                 // 0047D0BF..0047D0D4
        self->last = addr(p);                                                       // 0047D0D6
    }
    *result = first;                                                                // 0047D0D9..0047D0DD
    return result;
}
// 0047DAA0 vector<T*>::reserve(count).
void __fastcall nv_ptr_vec_reserve_0047daa0(Vec* self, void*, U count) {
    if (count > 0x3FFFFFFFu) nv_vector_xlen_0047d2b0(self, nullptr);               // 0047DABB..0047DACE
    if (cap32(self) >= count) return;                                               // 0047DAE6..0047DAFD
    U* const fresh = static_cast<U*>(live_operator_new(count * 4));                 // 0047DAFF..0047DB0A
    nv_ptr_uninit_copy_0047be40(f32(self->first), f32(self->last), fresh, self, count * 4); // 0047DB0F..0047DB30
    const U size = size32(self);                                                    // 0047DB35..0047DB44
    if (self->first != 0) live_free(f32(self->first));                              // 0047DB47..0047DB4C
    self->first = addr(fresh);                                                      // 0047DB5A
    self->last = addr(fresh + size);                                                // 0047DB57..0047DB5F
    self->end = addr(fresh) + count * 4;                                            // 0047DB54..0047DB66
}
// 0047E630 vector<T*>::resize(count, value).
void __fastcall nv_ptr_vec_resize_0047e630(Vec* self, void*, U count, U value) {
    if (size32(self) < count) {                                                     // 0047E631..0047E64B
        nv_ptr_vec_insert_n_0047de10(self, nullptr, f32(self->last), count - size32(self), &value); // 0047E64D..0047E66D
    } else if (self->first != 0 && count < size32(self)) {                          // 0047E677..0047E687
        U* r = nullptr;
        nv_ptr_vec_erase_0047d0b0(self, nullptr, &r, f32(self->first) + count, f32(self->last)); // 0047E689..0047E69E
    }
}
// 0047EC60 vector<T*>::push_back(value).
void __fastcall nv_ptr_vec_push_back_0047ec60(Vec* self, void*, const U* value) {
    if (self->first != 0 && size32(self) < cap32(self)) {                           // 0047EC63..0047EC85
        U* const at_end = f32(self->last);
        nv_ptr_uninit_fill_n_0047c1a0(at_end, 1, value, self, addr(value));         // 0047EC87..0047EC98
        self->last = addr(at_end + 1);                                              // 0047ECA0..0047ECA3
        return;
    }
    nv_ptr_vec_insert_n_0047de10(self, nullptr, f32(self->last), 1, value);        // 0047ECAB..0047ECBC
}

// ======================================================================= std::vector<24-byte element>
// 0047BEA0 _Uninit_copy for 24-byte elements (rep movsd, 6 dwords).
Elem24* __cdecl nv_e24_uninit_copy_0047bea0(const Elem24* first, const Elem24* last, Elem24* dest, void*, U) {
    for (; first != last; ++first, ++dest)                                          // 0047BEA9..0047BECA
        if (dest != nullptr) *dest = *first;
    return dest;
}
// 0047C200 _Uninit_fill_n for 24-byte elements.
void __cdecl nv_e24_uninit_fill_n_0047c200(Elem24* dest, U count, const Elem24* value, void*, U) {
    for (; count != 0; --count, ++dest)                                             // 0047C204..0047C228
        if (dest != nullptr) *dest = *value;
}
// 0047C130 fill for 24-byte elements.
void __cdecl nv_e24_fill_0047c130(Elem24* first, Elem24* last, const Elem24* value) {
    for (; first != last; ++first) *first = *value;                                 // 0047C138..0047C153
}
// 0047BE00 copy_backward for 24-byte elements (result*, first, last, dest, tag).
Elem24** __cdecl nv_e24_copy_backward_0047be00(Elem24** result, Elem24* first, Elem24* last, Elem24* dest, U) {
    while (last != first) *--dest = *--last;                                        // 0047BE09..0047BE26
    *result = dest;                                                                 // 0047BE28..0047BE3A
    return result;
}
// 0047C160 copy_backward dispatch for 24-byte elements -> 0047BE00.
Elem24** __cdecl nv_e24_copy_backward_0047c160(Elem24** result, Elem24* first, Elem24* last, Elem24* dest) {
    nv_e24_copy_backward_0047be00(result, first, last, dest, addr(dest));          // 0047C160..0047C186
    return result;                                                                  // 0047C18E
}
// 0047CCB0 vector<24>::_Ucopy.
Elem24* __fastcall nv_e24_vec_ucopy_0047ccb0(Vec* self, void*, const Elem24* first, const Elem24* last, Elem24* dest) {
    return nv_e24_uninit_copy_0047bea0(first, last, dest, self, addr(dest));       // 0047CCB0..0047CCCD
}
// 0047CE70 vector<24>::_Ufill.
Elem24* __fastcall nv_e24_vec_ufill_0047ce70(Vec* self, void*, Elem24* dest, U count, const Elem24* value) {
    nv_e24_uninit_fill_n_0047c200(dest, count, value, self, addr(value));          // 0047CE70..0047CE87
    return dest + count;                                                            // 0047CE8C..0047CE92
}
// 0047BAB0 vector<24>::size().
U __fastcall nv_e24_vec_size_0047bab0(Vec* self, void*) {
    return size24(self);                                                            // 0047BAB0..0047BAD0
}
// 0047E170 vector<24>::_Insert_n(where, count, value).
void __fastcall nv_e24_vec_insert_n_0047e170(Vec* self, void*, Elem24* where, U count, const Elem24* value) {
    Elem24 tmp = *value;                                                            // 0047E18D..0047E19B
    const U capacity = cap24(self);                                                 // 0047E19D..0047E1C1
    if (count == 0) return;                                                         // 0047E1C3..0047E1C8
    if (0x0AAAAAAAu - size24(self) < count) nv_vector_xlen_0047d2b0(self, nullptr); // 0047E1CE..0047E1F9
    if (capacity < size24(self) + count) {                                          // 0047E211..0047E233
        U grown = (0x0AAAAAAAu - capacity / 2 < capacity) ? 0 : capacity + capacity / 2; // 0047E239..0047E24C
        if (grown < size24(self) + count) grown = nv_e24_vec_size_0047bab0(self, nullptr) + count; // 0047E24E..0047E27B
        Elem24* const fresh = static_cast<Elem24*>(live_operator_new(grown * 24));  // 0047E27D..0047E284
        Elem24* p = nv_e24_uninit_copy_0047bea0(f24(self->first), where, fresh, self, addr(where)); // 0047E289..0047E2AC
        nv_e24_uninit_fill_n_0047c200(p, count, &tmp, self, addr(where));           // 0047E2B1..0047E2C2
        nv_e24_uninit_copy_0047bea0(where, f24(self->last), p + count, self, addr(where)); // 0047E2C7..0047E2E9
        count += size24(self);                                                      // 0047E2EE..0047E312
        if (self->first != 0) live_free(f24(self->first));                          // 0047E314..0047E319
        self->end = addr(fresh) + grown * 24;                                       // 0047E321..0047E32C
        self->last = addr(fresh + count);                                           // 0047E324..0047E32F
        self->first = addr(fresh);                                                  // 0047E332
    } else if (static_cast<U>(static_cast<I>(self->last - addr(where)) / 24) < count) { // 0047E35D..0047E37D
        nv_e24_vec_ucopy_0047ccb0(self, nullptr, where, f24(self->last), where + count); // 0047E37F..0047E39D
        nv_e24_vec_ufill_0047ce70(self, nullptr, f24(self->last),
                                  count - static_cast<U>(static_cast<I>(self->last - addr(where)) / 24), &tmp); // 0047E3A2..0047E3CB
        self->last = self->last + count * 24;                                       // 0047E3D0..0047E3D8
        nv_e24_fill_0047c130(where, f24(self->last) - count, &tmp);                 // 0047E3DB..0047E450
    } else {
        Elem24* const old_end = f24(self->last);                                    // 0047E35D..0047E37A
        self->last = addr(nv_e24_vec_ucopy_0047ccb0(self, nullptr, old_end - count, old_end, old_end)); // 0047E3EA..0047E410
        Elem24* moved = nullptr;
        nv_e24_copy_backward_0047c160(&moved, where, old_end - count, old_end);     // 0047E40D..0047E432
        nv_e24_fill_0047c130(where, where + count, &tmp);                           // 0047E43A..0047E450
    }
}
// 0047E6B0 vector<24>::insert(result*, where, value): offset kept across the reallocation.
Elem24** __fastcall nv_e24_vec_insert_0047e6b0(Vec* self, void*, Elem24** result, Elem24* where, const Elem24* value) {
    const U off = size24(self) == 0 ? 0 : static_cast<U>(static_cast<I>(addr(where) - self->first) / 24); // 0047E6B9..0047E6F5
    nv_e24_vec_insert_n_0047e170(self, nullptr, where, 1, value);                   // 0047E6F7..0047E705
    *result = f24(self->first) + off;                                               // 0047E70A..0047E719
    return result;
}
// 0047ECD0 vector<24>::push_back(value).
void __fastcall nv_e24_vec_push_back_0047ecd0(Vec* self, void*, const Elem24* value) {
    if (self->first != 0 && size24(self) < cap24(self)) {                           // 0047ECD5..0047ED14
        Elem24* const at_end = f24(self->last);
        nv_e24_uninit_fill_n_0047c200(at_end, 1, value, self, addr(value));         // 0047ED16..0047ED27
        self->last = addr(at_end + 1);                                              // 0047ED2F..0047ED32
        return;
    }
    Elem24* r = nullptr;
    nv_e24_vec_insert_0047e6b0(self, nullptr, &r, f24(self->last), value);         // 0047ED3B..0047ED4F
}

// ======================================================================= _Xlen
// 0047D2B0 vector::_Xlen: throw length_error("vector<T> too long") (string 0074318C; ThrowInfo 007640D0).
[[noreturn]] void __fastcall nv_vector_xlen_0047d2b0(void*, void*) {
    throw std::length_error("vector<T> too long");                                  // 0047D2C8..0047D325
}

// ======================================================================= std::set<T*> (_Tree)
// 0047C030 _Buynode() for the head: left/parent/right = 0 (each placement-checked), black, not nil.
Node* __cdecl nv_tree_buy_head_0047c030() {
    Node* const n = static_cast<Node*>(live_operator_new(0x14));                    // 0047C030..0047C032
    if (n != nullptr) put<U>(n, 0, 0);                                              // 0047C03A..0047C03E
    if (addr(n) + 4 != 0) put<U>(ptr<void>(addr(n) + 4), 0, 0);                     // 0047C044..0047C04B
    if (addr(n) + 8 != 0) put<U>(ptr<void>(addr(n) + 8), 0, 0);                     // 0047C051..0047C058
    put<char>(ptr<void>(addr(n)), 0x10, nv::kBlack);                                // 0047C05E
    put<char>(ptr<void>(addr(n)), 0x11, 0);                                         // 0047C062
    return n;
}
// 0047BED0 _Buynode(left, parent, right, value, color).
Node* __fastcall nv_tree_buy_node_0047bed0(Tree*, void*, Node* left, Node* parent, Node* right, const U* value, char color) {
    Node* const n = static_cast<Node*>(live_operator_new(0x14));                    // 0047BED0..0047BED2
    if (n != nullptr) {                                                             // 0047BEDA..0047BEDC
        n->left = left; n->parent = parent; n->right = right;                       // 0047BEE6..0047BEF3
        n->value = *value;                                                          // 0047BEF6..0047BEFC
        n->color = color;                                                           // 0047BEFF
        n->isnil = 0;                                                               // 0047BF02
    }
    return n;
}
// 0047C070 _Erase(root): free the subtree without rebalancing (right recursion, left iteration).
void __fastcall nv_tree_erase_subtree_0047c070(Tree* self, void*, Node* root) {
    for (Node* n = root; !isnil(n);) {                                              // 0047C077..0047C09F
        nv_tree_erase_subtree_0047c070(self, nullptr, n->right);                    // 0047C082..0047C088
        Node* const next = n->left;                                                 // 0047C08D
        live_free(n);                                                               // 0047C08F..0047C090
        n = next;
    }
}
// 0047BB20 const_iterator::_Inc.
void __fastcall nv_tree_iter_inc_0047bb20(Node** self, void*) {
    Node* p = *self;
    if (isnil(p)) return;                                                           // 0047BB20..0047BB27
    if (!isnil(p->right)) {                                                         // 0047BB29..0047BB32
        Node* m = p->right;
        while (!isnil(m->left)) m = m->left;                                        // 0047BB34..0047BB49
        *self = m;                                                                  // 0047BB4B
        return;
    }
    Node* q = p->parent;                                                            // 0047BB4F
    while (!isnil(q) && *self == q->right) {                                        // 0047BB52..0047BB71
        *self = q;
        q = q->parent;
    }
    *self = q;                                                                      // 0047BB73
}
// 0047BD80 const_iterator::_Dec.
void __fastcall nv_tree_iter_dec_0047bd80(Node** self, void*) {
    Node* p = *self;
    if (isnil(p)) { *self = p->right; return; }                                     // 0047BD80..0047BD8C
    if (!isnil(p->left)) {                                                          // 0047BD8F..0047BD97
        Node* m = p->left;
        while (!isnil(m->right)) m = m->right;                                      // 0047BD99..0047BDAD
        *self = m;                                                                  // 0047BDAF
        return;
    }
    Node* q = p->parent;                                                            // 0047BDB3
    while (!isnil(q) && *self == q->left) {                                         // 0047BDB6..0047BDD0
        *self = q;
        q = q->parent;
    }
    if (!isnil(q)) *self = q;                                                       // 0047BDD2..0047BDD9
}
// 0047BB00 _Min(node) / 0047BAE0 _Max(node).
Node* __cdecl nv_tree_min_0047bb00(Node* node) {
    while (!isnil(node->left)) node = node->left;                                   // 0047BB04..0047BB19
    return node;
}
Node* __cdecl nv_tree_max_0047bae0(Node* node) {
    while (!isnil(node->right)) node = node->right;                                 // 0047BAE4..0047BAFA
    return node;
}
// 0047BCC0 _Lrotate(where) / 0047BD20 _Rrotate(where).
void __fastcall nv_tree_lrotate_0047bcc0(Tree* self, void*, Node* where) {
    Node* const p = where->right;                                                   // 0047BCC4
    where->right = p->left;                                                         // 0047BCC9..0047BCCB
    if (!isnil(p->left)) p->left->parent = where;                                   // 0047BCCE..0047BCD7
    p->parent = where->parent;                                                      // 0047BCDA..0047BCDD
    if (where == self->head->parent) self->head->parent = p;                        // 0047BCE0..0047BCE8
    else if (where == where->parent->left) where->parent->left = p;                 // 0047BCF5..0047BCFC
    else where->parent->right = p;                                                  // 0047BD08
    p->left = where;                                                                // 0047BCEC / 0047BCFF / 0047BD0C
    where->parent = p;                                                              // 0047BCEE / 0047BD01 / 0047BD0E
}
void __fastcall nv_tree_rrotate_0047bd20(Tree* self, void*, Node* where) {
    Node* const p = where->left;                                                    // 0047BD24
    where->left = p->right;                                                         // 0047BD28..0047BD2B
    if (!isnil(p->right)) p->right->parent = where;                                 // 0047BD2D..0047BD37
    p->parent = where->parent;                                                      // 0047BD3A..0047BD3D
    if (where == self->head->parent) self->head->parent = p;                        // 0047BD40..0047BD48
    else if (where == where->parent->right) where->parent->right = p;               // 0047BD56..0047BD5E
    else where->parent->left = p;                                                   // 0047BD6C
    p->right = where;                                                               // 0047BD4C / 0047BD62 / 0047BD6F
    where->parent = p;                                                              // 0047BD4F / 0047BD65 / 0047BD72
}
// 0047CA80 find(result*, key): lower_bound, then end() unless the bound equals key.
Node** __fastcall nv_tree_find_0047ca80(Tree* self, void*, Node** result, const U* key) {
    Node* const head = self->head;                                                  // 0047CA81
    Node* where = head;
    for (Node* p = head->parent; !isnil(p);) {                                      // 0047CA84..0047CAB3
        if (p->value < *key) p = p->right;                                          // 0047CAA0..0047CAA5
        else { where = p; p = p->left; }                                            // 0047CAAA..0047CAAC
    }
    *result = (where == head || *key < where->value) ? head : where;                // 0047CAB5..0047CAE7
    return result;
}
// 0047D330 _Insert(result*, add_left, where, value): new red node under where, then red-black fix-up.
Node** __fastcall nv_tree_insert_at_0047d330(Tree* self, void*, Node** result, U8 add_left, Node* where, const U* value) {
    if (!(self->size < 0x3FFFFFFEu))                                                // 0047D34C..0047D353
        throw std::length_error("map/set<T> too long");                             // 0047D355..0047D3AC (007431A0, ThrowInfo 007640D0)
    Node* const created = nv_tree_buy_node_0047bed0(self, nullptr, self->head, where, self->head, value, nv::kRed); // 0047D3B1..0047D3C5
    self->size = self->size + 1;                                                    // 0047D3CA..0047D3D9
    if (where == self->head) {                                                      // 0047D3D3..0047D3DC
        self->head->parent = created;                                               // 0047D3DE
        self->head->left = created;                                                 // 0047D3E1..0047D3E4
        self->head->right = created;                                                // 0047D3E6..0047D3E9
    } else if (add_left != 0) {                                                     // 0047D3EE..0047D3F4
        where->left = created;                                                      // 0047D3F6
        if (where == self->head->left) self->head->left = created;                  // 0047D3F8..0047D3FF
    } else {
        where->right = created;                                                     // 0047D403
        if (where == self->head->right) self->head->right = created;                // 0047D406..0047D40E
    }
    for (Node* p = created; p->parent->color == nv::kRed;) {                        // 0047D411..0047D4D3
        Node* const parent = p->parent;
        Node* const grand = parent->parent;
        if (parent == grand->left) {                                                // 0047D426..0047D430
            Node* const uncle = grand->right;                                       // 0047D432
            if (uncle->color == nv::kRed) {                                         // 0047D435..0047D439
                parent->color = nv::kBlack;                                         // 0047D43B
                uncle->color = nv::kBlack;                                          // 0047D43E
                p->parent->parent->color = nv::kRed;                                // 0047D441..0047D446
                p = p->parent->parent;                                              // 0047D44A..0047D44C
            } else {
                if (p == parent->right) {                                           // 0047D451..0047D454
                    p = parent;                                                     // 0047D456
                    nv_tree_lrotate_0047bcc0(self, nullptr, p);                     // 0047D458..0047D45B
                }
                p->parent->color = nv::kBlack;                                      // 0047D460..0047D463
                p->parent->parent->color = nv::kRed;                                // 0047D466..0047D46C
                nv_tree_rrotate_0047bd20(self, nullptr, p->parent->parent);         // 0047D470..0047D479
            }
        } else {
            Node* const uncle = grand->left;                                        // 0047D480 (edx = grand->left)
            if (uncle->color == nv::kRed) {                                         // 0047D480..0047D484
                parent->color = nv::kBlack;                                         // 0047D486
                uncle->color = nv::kBlack;                                          // 0047D489
                p->parent->parent->color = nv::kRed;                                // 0047D48C..0047D491
                p = p->parent->parent;                                              // 0047D495..0047D497
            } else {
                if (p == parent->left) {                                            // 0047D49C..0047D49E
                    p = parent;                                                     // 0047D4A0
                    nv_tree_rrotate_0047bd20(self, nullptr, p);                     // 0047D4A2..0047D4A5
                }
                p->parent->color = nv::kBlack;                                      // 0047D4AA..0047D4AD
                p->parent->parent->color = nv::kRed;                                // 0047D4B0..0047D4B6
                nv_tree_lrotate_0047bcc0(self, nullptr, p->parent->parent);         // 0047D4BA..0047D4C3
            }
        }
    }
    self->head->parent->color = nv::kBlack;                                         // 0047D4DE..0047D4E4
    *result = created;                                                              // 0047D4E7..0047D4EC
    return result;
}
// 0047DC80 insert(result*, value) -> pair<iterator, bool>.
nv::InsertResult* __fastcall nv_tree_insert_0047dc80(Tree* self, void*, nv::InsertResult* result, const U* value) {
    Node* const head = self->head;                                                  // 0047DC87
    Node* where_node = head;                                                        // 0047DC98
    U8 add_left = 1;                                                                // 0047DC90
    for (Node* p = head->parent; !isnil(p);) {                                      // 0047DC8A..0047DCBD
        where_node = p;                                                             // 0047DCAA
        add_left = *value < p->value ? 1 : 0;                                       // 0047DCA2..0047DCA5 (setb)
        p = add_left ? p->left : p->right;                                          // 0047DCB0..0047DCB6
    }
    Node* where = where_node;                                                       // 0047DCC1..0047DCC3
    if (add_left != 0) {                                                            // 0047DCBF..0047DCC7
        if (where_node == head->left) {                                             // 0047DCC9..0047DCCB (begin())
            Node* created = nullptr;
            nv_tree_insert_at_0047d330(self, nullptr, &created, 1, where_node, value); // 0047DCCD..0047DCD8
            result->where = created; result->inserted = 1;                          // 0047DCDD..0047DCE8
            return result;
        }
        nv_tree_iter_dec_0047bd80(&where, nullptr);                                 // 0047DCF1..0047DCF5
    }
    if (where->value < *value) {                                                    // 0047DCFE..0047DD03
        Node* created = nullptr;
        nv_tree_insert_at_0047d330(self, nullptr, &created, add_left, where_node, value); // 0047DD05..0047DD13
        result->where = created; result->inserted = 1;                              // 0047DD18..0047DD23
        return result;
    }
    result->where = where; result->inserted = 0;                                    // 0047DD2C..0047DD35
    return result;
}
// 0047D7B0 erase(result*, where): unlink (successor lift when two subtrees), red-black fix-up, free the node.
Node** __fastcall nv_tree_erase_0047d7b0(Tree* self, void*, Node** result, Node* where) {
    if (isnil(where)) throw std::out_of_range("invalid map/set<T> iterator");      // 0047D7CA..0047D830 (007431B4, ThrowInfo 00764310)
    Node* const erased = where;                                                     // 0047D836..0047D83D
    nv_tree_iter_inc_0047bb20(&where, nullptr);                                     // 0047D839..0047D841
    Node* pnode = erased;
    Node* fix;
    if (isnil(erased->left)) fix = erased->right;                                   // 0047D846..0047D850 / 0047D864
    else if (isnil(erased->right)) fix = erased->left;                              // 0047D852..0047D859
    else { pnode = where; fix = pnode->right; }                                     // 0047D85B..0047D864
    Node* fix_parent;
    Node* const head = self->head;
    if (pnode == erased) {                                                          // 0047D866..0047D868
        fix_parent = erased->parent;                                                // 0047D86F
        if (!isnil(fix)) fix->parent = fix_parent;                                  // 0047D86A..0047D874
        if (head->parent == erased) head->parent = fix;                             // 0047D877..0047D883
        else if (fix_parent->left == erased) fix_parent->left = fix;                // 0047D888..0047D88C
        else fix_parent->right = fix;                                               // 0047D890
        if (head->left == erased)                                                   // 0047D893..0047D898
            head->left = isnil(fix) ? fix_parent : nv_tree_min_0047bb00(fix);       // 0047D89A..0047D8B2
        if (head->right == erased)                                                  // 0047D8B4..0047D8BA
            head->right = isnil(fix) ? fix_parent : nv_tree_max_0047bae0(fix);      // 0047D8BC..0047D8D3
    } else {
        erased->left->parent = pnode;                                               // 0047D8D8..0047D8DA
        pnode->left = erased->left;                                                 // 0047D8DD..0047D8DF
        if (pnode == erased->right) {                                               // 0047D8E1..0047D8E4
            fix_parent = pnode;                                                     // 0047D8E6
        } else {
            fix_parent = pnode->parent;                                             // 0047D8EF
            if (!isnil(fix)) fix->parent = fix_parent;                              // 0047D8EA..0047D8F4
            fix_parent->left = fix;                                                 // 0047D8F7
            pnode->right = erased->right;                                           // 0047D8F9..0047D8FC
            erased->right->parent = pnode;                                          // 0047D8FE..0047D901
        }
        if (head->parent == erased) head->parent = pnode;                           // 0047D904..0047D910
        else if (erased->parent->left == erased) erased->parent->left = pnode;      // 0047D915..0047D91C
        else erased->parent->right = pnode;                                         // 0047D920
        pnode->parent = erased->parent;                                             // 0047D923..0047D926
        const char c = pnode->color; pnode->color = erased->color; erased->color = c; // 0047D929..0047D932
    }
    if (erased->color == nv::kBlack) {                                              // 0047D935..0047D944
        for (; fix != self->head->parent && fix->color == nv::kBlack; fix = fix_parent, fix_parent = fix->parent) { // 0047D94A..0047DA0B
            if (fix == fix_parent->left) {                                          // 0047D95F..0047D963
                Node* p = fix_parent->right;                                        // 0047D965
                if (p->color == nv::kRed) {                                         // 0047D968..0047D96D
                    p->color = nv::kBlack;                                          // 0047D96F
                    fix_parent->color = nv::kRed;                                   // 0047D975
                    nv_tree_lrotate_0047bcc0(self, nullptr, fix_parent);            // 0047D972..0047D979
                    p = fix_parent->right;                                          // 0047D97E
                }
                if (isnil(p)) continue;                                             // 0047D981..0047D986 -> 0047D9FE
                if (p->left->color == nv::kBlack && p->right->color == nv::kBlack) { // 0047D988..0047D995
                    p->color = nv::kRed;                                            // 0047D9FA
                    continue;
                }
                if (p->right->color == nv::kBlack) {                                // 0047D997..0047D99D
                    p->left->color = nv::kBlack;                                    // 0047D99F..0047D9A1
                    p->color = nv::kRed;                                            // 0047D9A7
                    nv_tree_rrotate_0047bd20(self, nullptr, p);                     // 0047D9A4..0047D9AB
                    p = fix_parent->right;                                          // 0047D9B0
                }
                p->color = fix_parent->color;                                       // 0047D9B3..0047D9B6
                fix_parent->color = nv::kBlack;                                     // 0047D9B9
                p->right->color = nv::kBlack;                                       // 0047D9BC..0047D9C2
                nv_tree_lrotate_0047bcc0(self, nullptr, fix_parent);                // 0047D9BF..0047D9C5
                break;                                                              // 0047D9CA
            }
            Node* p = fix_parent->left;                                             // 0047D9CC (eax = fix_parent->left)
            if (p->color == nv::kRed) {                                             // 0047D9CC..0047D9D1
                p->color = nv::kBlack;                                              // 0047D9D3
                fix_parent->color = nv::kRed;                                       // 0047D9D9
                nv_tree_rrotate_0047bd20(self, nullptr, fix_parent);                // 0047D9D6..0047D9DD
                p = fix_parent->left;                                               // 0047D9E2
            }
            if (isnil(p)) continue;                                                 // 0047D9E4..0047D9E9
            if (p->right->color == nv::kBlack && p->left->color == nv::kBlack) {    // 0047D9EB..0047D9F8
                p->color = nv::kRed;                                                // 0047D9FA
                continue;
            }
            if (p->left->color == nv::kBlack) {                                     // 0047DA13..0047DA18
                p->right->color = nv::kBlack;                                       // 0047DA1A..0047DA1D
                p->color = nv::kRed;                                                // 0047DA23
                nv_tree_lrotate_0047bcc0(self, nullptr, p);                         // 0047DA20..0047DA27
                p = fix_parent->left;                                               // 0047DA2C
            }
            p->color = fix_parent->color;                                           // 0047DA2E..0047DA31
            fix_parent->color = nv::kBlack;                                         // 0047DA34
            p->left->color = nv::kBlack;                                            // 0047DA37..0047DA3C
            nv_tree_rrotate_0047bd20(self, nullptr, fix_parent);                    // 0047DA39..0047DA3F
            break;
        }
        fix->color = nv::kBlack;                                                    // 0047DA44
    }
    live_free(erased);                                                              // 0047DA47..0047DA4C
    if (self->size > 0) self->size = self->size - 1;                                // 0047DA51..0047DA5E
    *result = where;                                                                // 0047DA61..0047DA69
    return result;
}
// 0047E0B0 erase(result*, first, last): whole-tree fast path, else erase one by one (iterator ++ inlined).
Node** __fastcall nv_tree_erase_range_0047e0b0(Tree* self, void*, Node** result, Node* first, Node* last) {
    Node* const head = self->head;
    if (first == head->left && last == head) {                                      // 0047E0BD..0047E0C6
        nv_tree_erase_subtree_0047c070(self, nullptr, head->parent);                // 0047E0C8..0047E0CE
        self->head->parent = self->head;                                            // 0047E0D3..0047E0D6
        self->size = 0;                                                             // 0047E0DC
        self->head->left = self->head;                                              // 0047E0D9..0047E0E3
        self->head->right = self->head;                                             // 0047E0E5..0047E0E8
        *result = self->head->left;                                                 // 0047E0EB..0047E0F6
        return result;
    }
    while (first != last) {                                                         // 0047E0FC..0047E159
        Node* const current = first;                                                // 0047E105
        if (!isnil(first)) {                                                        // 0047E100..0047E107 (inlined ++first)
            if (!isnil(first->right)) {
                first = first->right;                                               // 0047E109..0047E113
                while (!isnil(first->left)) first = first->left;                    // 0047E115..0047E129
            } else {
                Node* q = first->parent;                                            // 0047E12D
                while (!isnil(q) && first == q->right) { first = q; q = q->parent; } // 0047E130..0047E146
                first = q;                                                          // 0047E148
            }
        }
        Node* r = nullptr;
        nv_tree_erase_0047d7b0(self, nullptr, &r, current);                         // 0047E14A..0047E152
    }
    *result = first;                                                                // 0047E15B..0047E160
    return result;
}
