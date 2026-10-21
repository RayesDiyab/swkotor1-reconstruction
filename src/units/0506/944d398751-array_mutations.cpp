#include "459924a746-array_mutations.h"
namespace kotor_reconstruction {
using U32=std::uint32_t;
using I32=std::int32_t;
static __forceinline I32 Signed(U32 bits) {
    return bits & 0x80000000u ? -1 - static_cast<I32>(~bits) : static_cast<I32>(bits);
}
static __forceinline U32 NextSize(ArrayMutations* list) {
    const U32 result = static_cast<U32>(list->size) - 1u;
    list->size = Signed(result);
    return result;
}
template<class Word> static __forceinline volatile Word* At(U32 base, U32 index, U32 stride, U32 offset=0) {
    return reinterpret_cast<volatile Word*>(base + index * stride + offset);
}
template<unsigned Stride> static __forceinline void Shift(ArrayMutations* list, U32 index) {
    while (Signed(index) < list->size) {
        const U32 base = list->data;
        for (U32 word=0; word != Stride/4; ++word)
            *At<U32>(base, index, Stride, word*4) = *At<U32>(base, index+1u, Stride, word*4);
        ++index;
    }
}
void ArrayMutations::RemoveReverse32(U32 value) {
    const U32 last = static_cast<U32>(size) - 1u;
    U32 index = last;
    if (Signed(index) >= 0) {
        const U32 base=data;
        while (*At<U32>(base,index,4) != value) {
            --index;
            if (Signed(index) < 0) break;
        }
    }
    size=Signed(last);
    Shift<4>(this,index);
}
void ArrayMutations::RemoveFirst32(I32 value) {
    const U32 originalSize=static_cast<U32>(size);
    U32 index=0;
    if (Signed(originalSize) > 0) {
        const U32 base=data;
        while (*At<U32>(base,index,4) != static_cast<U32>(value)) {
            ++index;
            if (Signed(index) >= size) break;
        }
    }
    size=Signed(originalSize-1u);
    Shift<4>(this,index);
}
void ArrayMutations::RemoveFirst16(std::uint16_t value) {
    const U32 originalSize=static_cast<U32>(size);
    U32 index=0;
    if (Signed(originalSize) > 0) {
        const U32 base=data;
        while (*At<std::uint16_t>(base,index,2) != value) {
            ++index;
            if (Signed(index) >= size) break;
        }
    }
    size=Signed(originalSize-1u);
    while (Signed(index) < size) {
        const U32 base=data;
        *At<std::uint16_t>(base,index,2) = *At<std::uint16_t>(base,index+1u,2);
        ++index;
    }
}
void ArrayMutations::DeleteAt32(I32 index) { NextSize(this); Shift<4>(this,static_cast<U32>(index)); }
void ArrayMutations::RemoveAtTag40(I32 index) { NextSize(this); Shift<40>(this,static_cast<U32>(index)); }
void ArrayMutations::RemoveAtCombat208(I32 index) { NextSize(this); Shift<208>(this,static_cast<U32>(index)); }
}
