// KOTOR 1 reconstruction — original 005791B0
// CSWSAreaMap::GetPartyMemberMapLocation
//
// Original body: 005791B0..005791E8 (0x38), thiscall, ECX=this,
// stack args (int index, int* outX, int* outY), callee RET 0xC.
// Lane export convention: @kotor_fastcall_005791b0@20 (ECX=this, EDX unused,
// three stack words; the original bytes use ret 0xC).
//
// This TU is a replacement body only. The current selected candidate has no
// real body at 005791B0; its 005791B0 symbol is the unrelated production trap
// `?party_restrict@NativeAreaProperties@?A0x748BBD52@@UAEXII@Z`
// (0886-collision-r2b.obj). Integration must remove/rebind that trap and assign
// this body to the original 005791B0 contract. No existing provider is edited.
//
// Faithfulness notes:
// - [this+4] is tested with `test eax,eax; jbe`, i.e. a zero-only test. A
//   negative/nonzero value proceeds. Do not change this to an unsigned <= 0.
// - index is tested with a signed `cmp index,3; jge`. Negative indices are not
//   rejected by the original; the indexed reads preserve that arithmetic.
// - the x cell at [this+index*4+0x28] is compared to the exact sentinel -1.
// - outX/outY are written without null checks; null outputs fault as original.
// - return is int 1 on success, 0 on failure; no allocation, no EH, no calls.
//
// No original absolute address is dereferenced. The only storage accessed is
// the object passed in ECX and the caller-provided output pointers.

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {

using U32 = std::uint32_t;
using I32 = std::int32_t;

// Object-relative field access. The addition is performed on the integer
// address so the original's unchecked/negative indexed reads remain raw faults
// rather than C++ pointer-arithmetic UB. No absolute address is used.
template <class T>
T load_at(const void* base, std::ptrdiff_t offset) {
    const std::intptr_t address =
        static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(base)) + offset;
    T value;
    std::memcpy(&value, reinterpret_cast<const void*>(address), sizeof(T));
    return value;
}

}  // namespace

extern "C" __declspec(noinline) int __fastcall kotor_fastcall_005791b0(
    void* self_,
    void* /*unused_edx*/,
    int index,
    int* outX,
    int* outY) {
    const unsigned char* const self = static_cast<const unsigned char*>(self_);

    // 005791B0..005791B7: mov eax,[ecx+4]; test eax,eax; jbe fail
    if (load_at<U32>(self, 0x04) == 0) return 0;

    // 005791B7..005791C0: cmp edx,3; jge fail (signed)
    if (index >= 3) return 0;

    // 005791C0..005791C9: mov eax,[ecx+edx*4+0x28]; cmp eax,-1; jne write
    const I32 x = load_at<I32>(self, 0x28 + static_cast<std::ptrdiff_t>(index) * 4);
    if (x == -1) return 0;

    // 005791CE..005791E8: *outX=x; *outY=[ecx+edx*4+0x34]; return 1
    *outX = x;
    *outY = load_at<I32>(self, 0x34 + static_cast<std::ptrdiff_t>(index) * 4);
    return 1;
}

// Integration/rebind notes:
// - Target original: 005791B0, thiscall RET 0xC (lane name @...@20).
// - Selected replacement source name: kotor_fastcall_005791b0.
// - Exact integrator export/binding: @kotor_fastcall_005791b0@20 =
//   _kotor_fastcall_005791b0 (the lane export generator creates the decorated
//   name from this C definition; if using alternatename directly, alias
//   @kotor_fastcall_005791b0@20 to _kotor_fastcall_005791b0).
// - Remove/rebind the current 005791B0 production trap
//   (`?party_restrict@NativeAreaProperties@?A0x748BBD52@@UAEXII@Z`) and bind
//   this body to 005791B0. Do not link both bodies at the same address.
// - Imports: none.
// - No original bytes, no absolute-address ports, no stub/trap/guard.
