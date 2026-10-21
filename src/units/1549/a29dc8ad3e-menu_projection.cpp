// 0060FF30: creature position to screen coordinates, exact x87 operation order.
#include "56f214f303-menu_projection.h"
#include "fe0daa935e-scene3d.h"
#include "63176f6f15-startup_string.h"
#include "6dab7adcb5-menu_cohort.h"
#include <cstring>
using namespace kotor_scene3d;
#include "02afe8a61c-vtables2_gen.h"

namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
void* __fastcall kotor_fastcall_005ed560(void*, void*);
void* __fastcall kotor_fastcall_005eda40(void*, void*);
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void*, void*) noexcept(false);
}

namespace {
template<class T> T load(const void* base, U offset) {
    T value;
    std::memcpy(&value, reinterpret_cast<const unsigned char*>(base) + offset, sizeof value);
    return value;
}
template<class T> void store(void* base, U offset, T value) {
    std::memcpy(reinterpret_cast<unsigned char*>(base) + offset, &value, sizeof value);
}
const CameraVTable* camera_vt(void* camera) { return load<const CameraVTable*>(camera, 0); }
void* client() { return load<void*>(kotor_menu_window_cells::manager_007a39fc, 4); }
const U one = 0x3F800000u;
const U ordinary_height = 0x3FE00000u;
const U tall_height = 0x40000000u;

// The coordinate copy occurs while the original x87 intermediates are live.
// In particular, do not combine the three forward DWORD copies into memmove.
void direction_and_position(const Quaternion* rotation, void* self, Vector* output,
                            Vector* negative, Quaternion* temporary) {
    __asm {
        mov eax, rotation
        mov edx, output
        mov ecx, self
        add ecx, 24h
        fld dword ptr [eax+8]
        fmul dword ptr [eax]
        fld dword ptr [eax+4]
        fmul dword ptr [eax+0Ch]
        faddp st(1), st(0)
        fadd st(0), st(0)
        fld dword ptr [eax+8]
        fmul dword ptr [eax+0Ch]
        fld dword ptr [eax]
        fmul dword ptr [eax+4]
        fsubp st(1), st(0)
        fadd st(0), st(0)
        fld dword ptr [eax+8]
        fld dword ptr [eax+4]
        fld st(1)
        mov eax, dword ptr [ecx]
        mov dword ptr [edx], eax
        fmul st(0), st(2)
        mov eax, dword ptr [ecx+4]
        fld st(1)
        mov dword ptr [edx+4], eax
        fmul st(0), st(2)
        mov eax, dword ptr [ecx+8]
        mov dword ptr [edx+8], eax
        faddp st(1), st(0)
        fadd st(0), st(0)
        fsubr dword ptr one
        mov eax, temporary
        fstp dword ptr [eax+8]
        fstp st(0)
        fstp st(0)
        fxch st(1)
        fchs
        mov ecx, negative
        fstp dword ptr [ecx]
        fchs
        fstp dword ptr [ecx+4]
        fld dword ptr [eax+8]
        fchs
        fstp dword ptr [ecx+8]
    }
}
void add_height(Vector* output, const U* height) {
    __asm {
        mov eax, output
        mov ecx, height
        fld dword ptr [eax+8]
        fadd dword ptr [ecx]
        fstp dword ptr [eax+8]
    }
}
std::uint8_t classify(const Vector* negative, const Vector* output, const Vector* position) {
    std::uint8_t other;
    __asm {
        mov ecx, negative
        mov edx, output
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+4]
        faddp st(1), st(0)
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+8]
        faddp st(1), st(0)
        mov edx, position
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+4]
        faddp st(1), st(0)
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        faddp st(1), st(0)
        fcompp
        fnstsw ax
        test ah, 5
        setp other
    }
    return other != 0 ? 2 : 1;
}
void scale(float* value, I extent) {
    __asm {
        mov eax, value
        fild dword ptr extent
        fmul dword ptr [eax]
        fstp dword ptr [eax]
    }
}
}

extern "C" std::uint8_t __fastcall kotor_fastcall_0060ff30(void* self, void*, Vector* output) {
    Vector negative;
    Quaternion temporary;
    void* const camera = kotor_fastcall_005ed560(client(), nullptr);
    const Quaternion* const orientation = reinterpret_cast<const Quaternion*>(
        camera_vt(camera)->s024_camera_get_orientation(camera, nullptr, reinterpret_cast<U>(&temporary)));
    direction_and_position(orientation, self, output, &negative, &temporary);
    bool tall = false;
    if (kotor_m15a_object_get_server_object_0063d4b0(self, nullptr) != nullptr) {
        void* const server = kotor_m15a_object_get_server_object_0063d4b0(self, nullptr);
        void* const creature = load<const menu_server::Slots*>(server, 0)->as_creature_030(server, nullptr);
        if (creature != nullptr) {
            auto* const name = reinterpret_cast<kotor_reconstruction::StartupString*>(
                static_cast<unsigned char*>(creature) + 0x18);
            tall = name->BorrowedEquals005e5290("zaalbar") != 0 || name->BorrowedEquals005e5290("hk47") != 0;
        }
    }
    add_height(output, tall ? &tall_height : &ordinary_height);
    const Vector* const position = camera_vt(camera)->s020_camera_get_position(
        camera, nullptr, reinterpret_cast<Vector*>(&temporary));
    const std::uint8_t result = classify(&negative, output, position);
    const U x = load<U>(output, 0);
    const U y = load<U>(output, 4);
    const U z = load<U>(output, 8);
    const void* const screen = reinterpret_cast<const void*>(camera_vt(camera)->s028_camera_map_to_screen(
        camera, nullptr, reinterpret_cast<U>(&temporary), x, y, z, 1));
    store<U>(output, 0, load<U>(screen, 0));
    store<U>(output, 4, load<U>(screen, 4));
    store<U>(output, 8, load<U>(screen, 8));
    const I width = load<std::int16_t>(kotor_fastcall_005eda40(client(), nullptr), 0x6C);
    scale(&output->x, width);
    const I height = load<std::int16_t>(kotor_fastcall_005eda40(client(), nullptr), 0x6E);
    scale(&output->y, height);
    return result;
}
