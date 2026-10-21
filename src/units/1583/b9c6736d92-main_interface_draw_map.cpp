// Original 0068AB10 — CSWGuiMainInterface::DrawMap
// Full entry-to-return reconstruction.
//
// Span 0068AB10..0068ACE3 (0x1D6); thiscall ECX=this + float alpha, RET 4.
// Export @kotor_fastcall_0068ab10@12. No SEH/allocation.
//
// Uses the prior accepted replacement bodies for 00687E40, 005791B0, 00578ED0
// and this lane's 00688100/00579090. The final integrator removes/rebinds the
// selected traps/unrelated bodies; no shared file is edited here.
//
// BorderParams+0x08 is a proven 4-byte float (004150A6 dword zero;
// 00415111/14 fld/fstp dword). The reconstructed header's Word u08 is an
// external owner correction; this body stores the raw float at +0x5FB8.

#include <cstdint>
#include <cstring>

namespace {

struct Extent {
    std::int32_t left;
    std::int32_t top;
    std::int32_t width;
    std::int32_t height;
};

struct Vector3 {
    float x;
    float y;
    float z;
};

struct Quaternion {
    float w;
    float x;
    float y;
    float z;
};

std::int32_t load_i32(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<const std::int32_t*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

float load_float(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<const float*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

void* load_ptr(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<void* const*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

extern "C" int __cdecl kotor_ftol2_006fae8c();
#pragma comment(linker, "/alternatename:_kotor_ftol2_006fae8c=_genuine_x87_ftol_006fae8c")

__declspec(noinline) std::int32_t x87_ftol(double value) {
    std::int32_t result;
    __asm {
        fld qword ptr [value]
        call kotor_ftol2_006fae8c
        mov result, eax
    }
    return result;
}

using SetExtentFn = void (__fastcall*)(void*, void*, const Extent*);
using DrawLabelFn = void (__fastcall*)(void*, void*, float);
using CameraOrientFn = void* (__fastcall*)(void*, void*, void*);

}  // namespace

namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;
}

extern "C" void* __fastcall server_get_module_004ae6b0(void* app, void*);
extern "C" void* __fastcall kotor_fastcall_005ed700(void* app, void*);
extern "C" void* __fastcall kotor_fastcall_005ed560(void* app, void*);
extern "C" int __fastcall c2da_equal_text_004060d0(
    const void* ref, void*, const char* text);
extern "C" void __fastcall kotor_fastcall_00687e40(void* self, void*);
extern "C" int __fastcall kotor_fastcall_005791b0(
    void* map, void*, int index, int* out_x, int* out_y);
extern "C" float __fastcall kotor_fastcall_00578ed0(
    void* map, void*, Vector3 value);
extern "C" void __fastcall kotor_fastcall_00688100(
    void* self, void*, const Extent* extent);
extern "C" Vector3* __fastcall quaternion_ydir_0044f420(
    const Quaternion* q, void*, Vector3* out);

extern "C" void __stdcall kotor_AurGUIStartLayer_004591B0();
extern "C" int __cdecl kotor_AurGUISetupViewport_004592F0(
    int x, int y, int width, int height, const void* color, int flags,
    float alpha);
extern "C" void __stdcall kotor_AurGUICloseViewport_00459580();
extern "C" void __stdcall kotor_AurGUIStopLayer_004592B0();

extern "C" void __fastcall kotor_fastcall_0068ab10(
    void* self, void*, float alpha) {
    unsigned char* const base = static_cast<unsigned char*>(self);
    void* const app = kotor_menu_window_cells::manager_007a39fc;

    void* options = kotor_fastcall_005ed700(
        load_ptr(app, 0x04), nullptr);
    if (options != nullptr &&
        (load_i32(options, 0x14) & 8) == 0) {
        *reinterpret_cast<std::uint32_t*>(
            base + 0x60DC) &= ~2u;
        return;
    }

    void* const module = server_get_module_004ae6b0(
        load_ptr(app, 0x08), nullptr);
    void* const map = load_ptr(module, 0x218);
    if (map == nullptr) {
        *reinterpret_cast<std::uint32_t*>(base + 0x60DC) &= ~2u;
        return;
    }
    if (load_i32(map, 0x04) == 0) {
        *reinterpret_cast<std::uint32_t*>(base + 0x60DC) &= ~2u;
        return;
    }

    if (c2da_equal_text_004060d0(base + 0x5EB0, nullptr, "") != 0) {
        kotor_fastcall_00687e40(self, nullptr);
    }

    Extent extent;
    std::memcpy(&extent, base + 0x5E04, sizeof(extent));
    const float scale = load_float(map, 0x14);

    int mx;
    int my;
    (void)kotor_fastcall_005791b0(map, nullptr, 0, &mx, &my);

    const std::int32_t half_w = load_i32(self, 0x6088) / 2;
    const std::int32_t half_h = load_i32(self, 0x608C) / 2;
    // Original 0068ABB0..0068ABF8 keeps fild/fmul/fsubp in x87 PC_53.
    // Use double, not a stored float, for the subtract then ftol.
    extent.left = x87_ftol(
        static_cast<double>(half_w) -
        static_cast<double>(mx) * static_cast<double>(scale));
    extent.top = x87_ftol(
        static_cast<double>(half_h) -
        static_cast<double>(my) * static_cast<double>(scale));

    void* const label = base + 0x5E00;
    void** const label_vtable = *reinterpret_cast<void***>(label);
    const SetExtentFn set_extent =
        reinterpret_cast<SetExtentFn>(label_vtable[0x04 / sizeof(void*)]);
    set_extent(label, nullptr, &extent);

    void* const camera = kotor_fastcall_005ed560(
        load_ptr(app, 0x04), nullptr);
    Quaternion quaternion;
    Vector3 ydir;
    void** const camera_vtable = *reinterpret_cast<void***>(camera);
    const CameraOrientFn orient =
        reinterpret_cast<CameraOrientFn>(
            camera_vtable[0x24 / sizeof(void*)]);
    orient(camera, nullptr, &quaternion);
    quaternion_ydir_0044f420(&quaternion, nullptr, &ydir);
    const float rotation =
        kotor_fastcall_00578ed0(map, nullptr, ydir);
    *reinterpret_cast<float*>(base + 0x5FB8) = rotation;

    static const float black[3] = {0.0f, 0.0f, 0.0f};
    kotor_AurGUIStartLayer_004591B0();
    if (kotor_AurGUISetupViewport_004592F0(
            load_i32(self, 0x6080), load_i32(self, 0x6084),
            load_i32(self, 0x6088), load_i32(self, 0x6088),
            black, 0, load_float(self, 0x4C)) == 0) {  // orig 0068AC5F: [esi+4Ch] panel alpha, not the delta param
        kotor_AurGUIStopLayer_004592B0();
        *reinterpret_cast<std::uint32_t*>(base + 0x60DC) |= 2u;
        return;
    }

    void* const label_map = base + 0x5E00;
    void** const label_map_vtable = *reinterpret_cast<void***>(label_map);
    reinterpret_cast<DrawLabelFn>(
        label_map_vtable[0x38 / sizeof(void*)])(label_map, nullptr, alpha);

    kotor_fastcall_00688100(self, nullptr, &extent);

    void* const label_arrow = base + 0x5F40;
    void** const label_arrow_vtable = *reinterpret_cast<void***>(label_arrow);
    reinterpret_cast<DrawLabelFn>(
        label_arrow_vtable[0x38 / sizeof(void*)])(label_arrow, nullptr, alpha);

    kotor_AurGUICloseViewport_00459580();
    kotor_AurGUIStopLayer_004592B0();
    *reinterpret_cast<std::uint32_t*>(base + 0x60DC) |= 2u;
}
