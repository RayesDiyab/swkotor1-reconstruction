// Original swkotor.exe 0069EDC0 (Ghidra label CSWCAnimBase::GetBoundingRect): project a Gob's bounding box through the
// module camera and write the screen rectangle {x, y, width, height} as four
// DWORDs. stdcall (Gob*, rect*), RET 8, EAX = 1 on success, 0 otherwise.
//
// Frame convention: frame[N] is original [ESP0+N], where ESP0 is ESP after the
// four register pushes at 0069EDCE..0069EDD1 (locals span ESP0+10..ESP0+FF).
// Every callee write into it is listed in the tier's frame audit.
//
// Canonical carriers only: scene3d.h Vector/Quaternion, aurora_model::GobVTable
// (vtables_gen.h) and kotor_scene3d::CameraVTable (vtables2_gen.h) slot types,
// and the selected providers' exact direct-call declarations.
#include "309ecfaddb-scene3d.h"
#include "4c326ffcda-anim_base_get_bounding_rect_0069edc0.h"
#include <cstddef>
#include <cstdint>
#include <cstring>

using U = std::uint32_t;
using I = std::int32_t;
using B = std::uint8_t;
using kotor_scene3d::Quaternion;
using kotor_scene3d::Vector;

// 0053-window-shared-cells.obj: ?manager_007a39fc@kotor_menu_window_cells@@3RAXA
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }

extern "C" {
// client_update.obj (CClientExoApp::GetModuleCamera, thiscall RET 0)
void* __fastcall kotor_fastcall_005ed560(void*, void*);
// 0417-game-prepare-client-callees.obj (CClientExoApp::GetSWGuiManager, thiscall RET 0)
void* __fastcall kotor_fastcall_005eda40(void*, void*);
// 0552-model46-f2-math.obj (Quaternion::zdir, thiscall RET 4, EAX = out)
Vector* __fastcall quaternion_zdir_0043e250(const Quaternion*, void*, Vector*);
// 0526-world-los-area-los.obj (vector constructor iterator, stdcall RET 10h)
void __stdcall vector_constructor_iterator_00401130(void*, U, U, void* (__fastcall*)(void*, void*));
// 0526-world-los-area-los.obj (Vector::Vector(), thiscall RET 0, EAX = this)
void* __fastcall vector_default_construct_00409e70(void*, void*);
// 0206-renderer-x87-ftol-alias.obj: original register ABI, ST0 in, EDX:EAX out.
void __cdecl genuine_x87_ftol_006fae8c();
}
// quaternion_rotate_004a9bb0 (cdecl, out first) is declared by scene3d.h; its
// selected provider is 0464-scene3d.obj (_quaternion_rotate_004a9bb0).

namespace {
using GobGetPosition = decltype(aurora_model::GobVTable::s064_gob_get_position);
using GobGetOrientation = decltype(aurora_model::GobVTable::s068_gob_get_orientation);
using GobGetBoundingBox = decltype(aurora_model::GobVTable::s0ec_gob_get_bounding_box);
using CameraGetPosition = decltype(kotor_scene3d::CameraVTable::s020_camera_get_position);
using CameraGetOrientation = decltype(kotor_scene3d::CameraVTable::s024_camera_get_orientation);
using CameraMapToScreen = decltype(kotor_scene3d::CameraVTable::s028_camera_map_to_screen);
using CameraGetClipDist = decltype(kotor_scene3d::CameraVTable::s03c_camera_get_clip_dist);
static_assert(offsetof(aurora_model::GobVTable, s064_gob_get_position) == 0x64, "Gob+64");
static_assert(offsetof(aurora_model::GobVTable, s068_gob_get_orientation) == 0x68, "Gob+68");
static_assert(offsetof(aurora_model::GobVTable, s0ec_gob_get_bounding_box) == 0xEC, "Gob+EC");
static_assert(offsetof(kotor_scene3d::CameraVTable, s020_camera_get_position) == 0x20, "Camera+20");
static_assert(offsetof(kotor_scene3d::CameraVTable, s024_camera_get_orientation) == 0x24, "Camera+24");
static_assert(offsetof(kotor_scene3d::CameraVTable, s028_camera_map_to_screen) == 0x28, "Camera+28");
static_assert(offsetof(kotor_scene3d::CameraVTable, s03c_camera_get_clip_dist) == 0x3C, "Camera+3C");

template<class T> T rd(const void* p, U o) { T v; std::memcpy(&v, static_cast<const B*>(p) + o, sizeof(v)); return v; }
template<class T> void wr(void* p, U o, T v) { std::memcpy(static_cast<B*>(p) + o, &v, sizeof(v)); }
void* ptr(const void* p, U o) { return rd<void*>(p, o); }
U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class F> F slot(const void* object, U offset) { return rd<F>(ptr(object, 0), offset); }
Vector* vec(B* f, U o) { return reinterpret_cast<Vector*>(f + o); }
const Quaternion* quat(B* f, U o) { return reinterpret_cast<const Quaternion*>(f + o); }
const float* flt(B* f, U o) { return reinterpret_cast<const float*>(f + o); }

// 0069EE62..0069EE87: [4C],[50],[54] = -direction.{x,y,z}, x87 FLD/FCHS/FSTP dword.
void negate_direction(B* f, const Vector* direction) {
    __asm {
        mov ecx, f
        mov eax, direction
        fld dword ptr [eax]
        fchs
        fstp dword ptr [ecx+04Ch]
        fld dword ptr [eax+4]
        fchs
        fstp dword ptr [ecx+050h]
        fld dword ptr [eax+8]
        fchs
        fstp dword ptr [ecx+054h]
    }
}

// 0069F029..0069F05F: [10..18] = gob position [58..60] + rotated, rounded to
// dword by FSTP; the corner receives the stored raw DWORDs.
void translate_corner(B* f, Vector* corner, const Vector* rotated) {
    __asm {
        mov ecx, f
        mov eax, rotated
        fld dword ptr [ecx+058h]
        fadd dword ptr [eax]
        fstp dword ptr [ecx+010h]
        fld dword ptr [ecx+05Ch]
        fadd dword ptr [eax+4]
        fstp dword ptr [ecx+014h]
        fld dword ptr [ecx+060h]
        fadd dword ptr [eax+8]
        mov eax, corner
        mov edx, dword ptr [ecx+010h]
        mov dword ptr [eax], edx
        mov edx, dword ptr [ecx+014h]
        mov dword ptr [eax+4], edx
        fstp dword ptr [ecx+018h]
        mov edx, dword ptr [ecx+018h]
        mov dword ptr [eax+8], edx
    }
}

// 0069F062..0069F097: dot(corner - camera position [64..6C], -direction [4C..54])
// in the original FPU order, FCOMP against near clip [48], mask 41h, JNP.
// Returns 0 (original exit 0069F1C3) when exactly one of C0/C3 is set
// (dot < near or dot == near); greater or unordered (NaN) continues.
B corner_in_front(B* f, const Vector* corner) {
    B result = 0;
    __asm {
        mov ecx, f
        mov eax, corner
        fld dword ptr [eax]
        fsub dword ptr [ecx+064h]
        fld dword ptr [eax+4]
        fsub dword ptr [ecx+068h]
        fld dword ptr [eax+8]
        fsub dword ptr [ecx+06Ch]
        fld dword ptr [ecx+054h]
        fmul st(0), st(1)
        fxch st(2)
        fmul dword ptr [ecx+050h]
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [ecx+04Ch]
        faddp st(1), st(0)
        fcomp dword ptr [ecx+048h]
        fnstsw ax
        fstp st(0)
        test ah, 041h
        jnp rejected
        mov result, 1
    rejected:
    }
    return result;
}

// 0069F0BC..0069F11A: min X [3C], min Y [38] (mask 5, JNP skips the copy);
// max X [40], max Y [44] (mask 41h, JE skips). Equal and unordered compares copy
// the projected raw DWORD.
void update_extent(B* f, const Vector* projected) {
    __asm {
        mov ecx, f
        mov edx, projected
        fld dword ptr [ecx+03Ch]
        fcomp dword ptr [edx]
        fnstsw ax
        test ah, 5
        jnp min_y
        mov eax, dword ptr [edx]
        mov dword ptr [ecx+03Ch], eax
    min_y:
        fld dword ptr [ecx+038h]
        fcomp dword ptr [edx+4]
        fnstsw ax
        test ah, 5
        jnp max_x
        mov eax, dword ptr [edx+4]
        mov dword ptr [ecx+038h], eax
    max_x:
        fld dword ptr [ecx+040h]
        fcomp dword ptr [edx]
        fnstsw ax
        test ah, 041h
        je max_y
        mov eax, dword ptr [edx]
        mov dword ptr [ecx+040h], eax
    max_y:
        fld dword ptr [ecx+044h]
        fcomp dword ptr [edx+4]
        fnstsw ax
        test ah, 041h
        je extent_done
        mov eax, dword ptr [edx+4]
        mov dword ptr [ecx+044h], eax
    extent_done:
    }
}

// 0069F14B..0069F153 / 0069F169..0069F171: FILD extent; FMUL dword; __ftol.
// Kept out of line so EAX/ECX/EDX clobbers by the ftol helper fall on a C++
// call boundary; only EAX (low DWORD) is consumed, as at 0069F15F/0069F17E.
__declspec(noinline) I scaled_coordinate(I extent, const float* coordinate) {
    I result;
    __asm {
        mov eax, coordinate
        fild dword ptr extent
        fmul dword ptr [eax]
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

// 0069F176..0069F18D / 0069F192..0069F1A9: FLD max; FSUB min; FIMUL extent; __ftol.
__declspec(noinline) I scaled_span(I extent, const float* maximum, const float* minimum) {
    I result;
    __asm {
        mov eax, maximum
        fld dword ptr [eax]
        mov eax, minimum
        fsub dword ptr [eax]
        fimul dword ptr extent
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

// Corner order built at 0069EEB7..0069F000 from min [20..28] / max [2C..34].
const U kCornerWords[8][3] = {
    {0x20, 0x24, 0x28}, {0x2C, 0x30, 0x34}, {0x20, 0x24, 0x34}, {0x20, 0x30, 0x28},
    {0x2C, 0x24, 0x28}, {0x20, 0x30, 0x34}, {0x2C, 0x24, 0x34}, {0x2C, 0x30, 0x28},
};
}  // namespace

extern "C" U __stdcall anim_base_get_bounding_rect_0069edc0(aurora_model::Gob* gob, void* rect) {
    // 0069EDC0..0069EDD2: camera = GetModuleCamera([[007A39FC]+4]).
    void* camera = kotor_fastcall_005ed560(ptr(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);
    if (!camera) return 0;                                                   // 0069EDDB
    if (!gob) return 0;                                                      // 0069EDEA

    alignas(4) B frame[0x100];
    for (U off = 0x20; off < 0x38; off += 4) wr<U>(frame, off, 0u);          // 0069EDF9..0069EE21

    slot<GobGetPosition>(gob, 0x64)(gob, nullptr, word(frame + 0x58));      // 0069EE29
    slot<GobGetOrientation>(gob, 0x68)(gob, nullptr, word(frame + 0xE4));   // 0069EE38
    slot<CameraGetPosition>(camera, 0x20)(camera, nullptr, vec(frame, 0x64)); // 0069EE44
    // 0069EE49..0069EE5D: the zdir output [10] is pushed before Camera+24
    // (RET 4, consumes only its own Quaternion output [D4]); the remaining
    // word is QuaternionZdir's RET 4 argument. ECX = Camera+24's EAX.
    const U orientation = slot<CameraGetOrientation>(camera, 0x24)(camera, nullptr, word(frame + 0xD4));
    const Vector* direction = quaternion_zdir_0043e250(
        reinterpret_cast<const Quaternion*>(static_cast<std::uintptr_t>(orientation)), nullptr, vec(frame, 0x10));
    negate_direction(frame, direction);                                      // 0069EE62..0069EE87
    slot<CameraGetClipDist>(camera, 0x3C)(camera, nullptr, word(frame + 0x48), word(frame + 0xD0)); // 0069EE8D
    slot<GobGetBoundingBox>(gob, 0xEC)(gob, nullptr, word(frame + 0x20), word(frame + 0x2C));       // 0069EE9E
    vector_constructor_iterator_00401130(frame + 0x70, 12u, 8u, &vector_default_construct_00409e70); // 0069EEA4..0069EEB2

    for (U i = 0; i < 8; ++i)
        for (U j = 0; j < 3; ++j)
            wr<U>(frame, 0x70 + i * 12 + j * 4, rd<U>(frame, kCornerWords[i][j]));
    wr<U>(frame, 0x38, 0x7F7FFFFFu);                                         // 0069EEEE min Y
    wr<U>(frame, 0x3C, 0x7F7FFFFFu);                                         // 0069EEF6 min X
    wr<U>(frame, 0x44, 0xFF7FFFFFu);                                         // 0069EEFE max Y
    wr<U>(frame, 0x40, 0xFF7FFFFFu);                                         // 0069EF06 max X

    for (U i = 0; i < 8; ++i) {                                              // 0069F007..0069F12D
        Vector* corner = vec(frame, 0x70 + i * 12);
        const Vector* rotated = quaternion_rotate_004a9bb0(vec(frame, 0xF4), quat(frame, 0xE4), corner); // 0069F024
        translate_corner(frame, corner, rotated);
        if (!corner_in_front(frame, corner)) return 0;                       // 0069F097 -> 0069F1C3
        // 0069F09D..0069F0B9: MapToScreen(out [D4], Vector by value, 1).
        const U screen = slot<CameraMapToScreen>(camera, 0x28)(
            camera, nullptr, word(frame + 0xD4), rd<U>(frame, 0x10), rd<U>(frame, 0x14), rd<U>(frame, 0x18), 1u);
        std::memcpy(corner, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(screen)), sizeof(Vector)); // 0069F0C0..0069F0CF
        update_extent(frame, corner);
    }

    // 0069F133..0069F13C: GetSWGuiManager([[007A39FC]+4]); extents are signed
    // WORDs at +6C (width) and +6E (height), re-read for every use.
    void* gui = kotor_fastcall_005eda40(ptr(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);
    void* out = rect;                                                        // 0069F158
    wr<I>(out, 0x0, scaled_coordinate(static_cast<I>(rd<std::int16_t>(gui, 0x6C)), flt(frame, 0x3C)));      // 0069F15F
    wr<I>(out, 0x4, scaled_coordinate(static_cast<I>(rd<std::int16_t>(gui, 0x6E)), flt(frame, 0x38)));      // 0069F17E
    wr<I>(out, 0x8, scaled_span(static_cast<I>(rd<std::int16_t>(gui, 0x6C)), flt(frame, 0x40), flt(frame, 0x3C))); // 0069F19A
    wr<I>(out, 0xC, scaled_span(static_cast<I>(rd<std::int16_t>(gui, 0x6E)), flt(frame, 0x44), flt(frame, 0x38))); // 0069F1AE
    return 1;
}
