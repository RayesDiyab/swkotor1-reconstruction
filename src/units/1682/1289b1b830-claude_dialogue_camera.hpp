#pragma once
// Lane "dialogue-start", fork B2: shared declarations for claude_dialogue_camera.cpp / claude_dialogue_camera2.cpp.
#include "6bd5403233-claude_dialogue_common.hpp"

namespace dlg {
struct V3 { U x, y, z; };          // Vector, passed by value as three dwords
struct Q4 { U w, x, y, z; };       // Quaternion, passed by value as four dwords (memory order of the original)
static_assert(sizeof(V3) == 12 && sizeof(Q4) == 16, "x86 value sizes");

// x87 `fcom st, src` + `fnstsw ax`: AH bits C0=01h (st < src or unordered), C2=04h (unordered), C3=40h (st == src
// or unordered). `test ah, m` then sets ZF = !(ah & m) and PF = even parity of (ah & m).
inline U8 fcom(double st, double src) {
    if (st != st || src != src) return 0x45;
    if (st < src) return 0x01;
    if (st == src) return 0x40;
    return 0x00;
}
inline bool jne(U8 ah, U8 mask) { return (ah & mask) != 0; }
inline bool jp(U8 ah, U8 mask) {
    U8 v = static_cast<U8>(ah & mask);
    v ^= static_cast<U8>(v >> 4); v ^= static_cast<U8>(v >> 2); v ^= static_cast<U8>(v >> 1);
    return (v & 1) == 0;
}
inline double fl(U bits) { return static_cast<double>(bitsf(bits)); }   // fld dword
inline U fs(double v) { return fbits(static_cast<float>(v)); }           // fstp dword
}  // namespace dlg

extern "C" {
// x87-returning cdecl providers (declared double so ST0 is stored unrounded under PC_53)
double __cdecl kotor_cdecl_004acb90(dlg::Q4 q);                          // Roll (1205-01 camera_stick)
double __cdecl quaternion_pitch_004aa020(const void* q);                 // Pitch(Quaternion const&) (0625)
double __cdecl quaternion_yaw_004a9f40(const void* q);                   // Yaw(Quaternion const&) (0625)
double __cdecl kotor_m15a_pitch_004aa130(const void* v);                 // Pitch(Vector const&) (0816)
double __cdecl kotor_m15a_yaw_004aa0f0(const void* v);                   // Yaw(Vector const&) (0816)
void* __cdecl yaw_pitch_roll_004acac0(void* out, float yaw, float pitch, float roll); // YawPitchRoll -> out (0542)
void* __cdecl quaternion_rotate_004a9bb0(void* out, const void* q, const void* v);   // operator*(Quaternion, Vector)
void* __cdecl normalize_004ab180(void* out, const void* v);              // normalize(Vector const&) by value
void* __cdecl vector_cross_004ab210(void* out, const void* a, const void* b);        // vectorCrossProduct
double __cdecl vector_magnitude_004ab100(const void* v);                 // vector_magnitude
void* __cdecl kotor_cdecl_00423bc0(void* out, const void* a, const void* b);         // operator-(Vector, Vector)
int __cdecl aurora_vector_equal_004aa980(const void* a, const void* b);  // operator==(Vector, Vector)
void* __fastcall quaternion_construct_004ac960(void* q, void*, dlg::V3 axis, float angle); // Quaternion(axis, angle)
struct CrtExtended { unsigned char bytes[10]; };                        // 80-bit x87 image (crt_x87::Extended layout)
// linked provider t11b-crt_acos_006fc0e0-observed.obj: the original ST0 -> ST0 routine 006FC0E0 transported as
// void __cdecl (const Extended* st0_in, Extended* st0_out) (0056461C mov eax,[ebp+8])
void __cdecl crt_acos_x87_006fc0e0(const CrtExtended* st0_in, CrtExtended* st0_out);

// this lane's exports
void* __fastcall kotor_fastcall_006288f0(void* gui, void*, void* out, dlg::U anim);
dlg::U __fastcall kotor_fastcall_0062ab70(void* gui, void*, dlg::U anim);
void __fastcall kotor_fastcall_0062b470(void* gui, void*, dlg::U fov_bits);
dlg::U __fastcall kotor_fastcall_0062bf30(void* gui, void*);
dlg::U __fastcall kotor_fastcall_0062bf70(void* gui, void*, dlg::U anim, dlg::U fov_bits);
void __fastcall kotor_fastcall_0062c030(void* gui, void*, dlg::U camera_id, dlg::U flag160, dlg::U effect);
dlg::U __fastcall kotor_fastcall_0062f250(void* gui, void*, dlg::U id);
dlg::U __fastcall kotor_fastcall_0062f3c0(void* gui, void*, dlg::U id);
void __fastcall kotor_fastcall_006306e0(void* gui, void*, dlg::U shot, dlg::U camera_id, dlg::U a3, dlg::U a4,
                                        dlg::U flag160, dlg::U effect);
float __fastcall kotor_fastcall_00641010(void* module, void*, dlg::U anim, dlg::U fov_bits, dlg::U object_id);
void __fastcall kotor_fastcall_006412f0(void* module, void*, dlg::U flag);
void __fastcall kotor_fastcall_004cc360(void* object, void*);
void* __fastcall kotor_fastcall_006bb670(void* self, void*, void* owner, dlg::U flag);
void __fastcall kotor_fastcall_006bb750(void* self, void*);
void* __fastcall kotor_fastcall_006bb790(void* self, void*, dlg::U flags);
dlg::U __fastcall kotor_fastcall_006bb740(void* self, void*);
void __fastcall kotor_fastcall_006bb590(void* self, void*, dlg::U type, dlg::V3 position, dlg::Q4 orientation,
                                        dlg::U fov_bits);
dlg::U __fastcall kotor_fastcall_006bb610(void* self, void*, dlg::U a, dlg::U b);
dlg::U __fastcall kotor_fastcall_006bcec0(void* self, void*, dlg::U a, dlg::U b);
dlg::U __fastcall kotor_fastcall_006bc600(void* self, void*);
dlg::U __fastcall kotor_fastcall_006bcf50(void* self, void*, dlg::U delta);
void __fastcall kotor_fastcall_006bb7b0(void* self, void*, dlg::U type, dlg::U side, dlg::V3 a, dlg::V3 b,
                                        void* out_position, void* out_orientation);
void __fastcall kotor_fastcall_006bd260(void* self, void*, dlg::U id_a, dlg::U id_b, void* shot, void* a4,
                                        void* a5, dlg::U a6);
int __cdecl kotor_cdecl_006fe517(double value);                          // __finite
extern const void* const k_camera_dialog_vtable_00756ae8[8];
}
