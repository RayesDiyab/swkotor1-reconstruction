// KOTOR 1 reconstruction — original 00578ED0
// CSWSAreaMap::GetMapRotateCCWFromWorldOrientation
//
// Original body: 00578ED0..00578F08 (0x38), thiscall, ECX=this,
// one Vector3 argument by value (three dwords / 12 bytes), float result in ST0,
// callee RET 0xC. Lane export convention: @kotor_fastcall_00578ed0@20
// (ECX=this, EDX unused, 12 stack bytes).
//
// This TU is a replacement body only. The current selected candidate has no
// real body at 00578ED0; its symbol is the unrelated real
// `?format1@NativeAreaProperties@?A0x748BBD52@@UAEXIII@Z`
// (0886-collision-r2b.obj). Integration must remove/rebind that body and assign
// this body to the original 00578ED0 contract. No existing provider is edited.
//
// Faithfulness notes:
// - The nested call is the original 004AA0F0 Yaw, selected real at 00477E30 as
//   `_kotor_m15a_yaw_004aa0f0`: cdecl, one const Vector* argument, float in ST0,
//   RET 0 (caller adds 4). Yaw reads only x and y of the vector.
// - [this+0x10] is the orientation mode:
//     1 -> yaw + 180.0f  (004AA0F0 result plus original 0x73E408)
//     2 -> yaw +  90.0f  (original 0x73E40C)
//     3 -> yaw + 270.0f  (original 0x73E404)
//     other -> yaw unchanged
// - The float additions are single-precision (`fadd dword [const]`), not double.
// - No allocation, no EH, no frame; only the Yaw call and the mode switch.
//
// No original absolute address is dereferenced.

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {

using I32 = std::int32_t;

// Object-relative field access. The addition is performed on the integer
// address so an invalid/missing object faults like the original instead of
// relying on C++ pointer arithmetic. No absolute address is used.
template <class T>
T load_at(const void* base, std::ptrdiff_t offset) {
    const std::intptr_t address =
        static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(base)) + offset;
    T value;
    std::memcpy(&value, reinterpret_cast<const void*>(address), sizeof(T));
    return value;
}

}  // namespace

struct Vector3 {
    float x;
    float y;
    float z;
};
static_assert(sizeof(Vector3) == 12, "Vector3 must be three binary32 words");

// Original 004AA0F0 Yaw. Selected real body: _kotor_m15a_yaw_004aa0f0 at
// 00477E30 (0816-m15a-creature.obj). Cdecl, one pointer argument, float ST0,
// RET 0. The selected constants are at 0x83DF88 (qword 0.0), 0x83DF90 (0.0f)
// and 0x83DF94 (57.2957802f); the body is an exact clone of the original.
extern "C" float _kotor_m15a_yaw_004aa0f0(const Vector3* vector);

extern "C" __declspec(noinline) float __fastcall kotor_fastcall_00578ed0(
    void* self_,
    void* /*unused_edx*/,
    Vector3 value) {
    const unsigned char* const self = static_cast<const unsigned char*>(self_);

    // 00578ED0..00578EDD: push esi; lea eax,[esp+8]; push eax; esi=ecx;
    // call Yaw; add esp,4
    const float yaw = _kotor_m15a_yaw_004aa0f0(&value);

    // 00578EDD..00578F05: eax=[esi+0x10]; dec/dec/dec mode dispatch
    switch (load_at<I32>(self, 0x10)) {
        case 1:
            // 00578EFF..00578F05: fadd dword [0x73E408] = 180.0f
            return yaw + 180.0f;
        case 2:
            // 00578EF6..00578EFC: fadd dword [0x73E40C] = 90.0f
            return yaw + 90.0f;
        case 3:
            // 00578EED..00578EF3: fadd dword [0x73E404] = 270.0f
            return yaw + 270.0f;
        default:
            // 00578F05: ret 0xC; Yaw result unchanged
            return yaw;
    }
}

// Integration/rebind notes:
// - Target original: 00578ED0, thiscall Vector3-by-value RET 0xC
//   (lane name @...@20).
// - Selected replacement source name: kotor_fastcall_00578ed0.
// - Exact integrator export/binding: @kotor_fastcall_00578ed0@20 =
//   _kotor_fastcall_00578ed0 (the lane export generator creates the decorated
//   name from this C definition; if using alternatename directly, alias
//   @kotor_fastcall_00578ed0@20 to _kotor_fastcall_00578ed0).
// - Remove/rebind the current 00578ED0 unrelated body
//   (`?format1@NativeAreaProperties@?A0x748BBD52@@UAEXIII@Z`) and bind this
//   body to 00578ED0. Do not link both bodies at the same address.
// - Import: _kotor_m15a_yaw_004aa0f0 (selected real at 00477E30).
// - No original bytes, no absolute-address ports, no stub/trap/guard.
