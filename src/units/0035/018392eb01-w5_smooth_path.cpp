// SmoothFollowPath00635AF0, complete original1662-byte body. W5 ownership.
// This TU adds one strong implementation to override the existing weak foreign trap.
// Native x86 only; providers and all data-derived pointers are the actual composition.
#include "d8371efa32-w5_t2.hpp"
#include <cstdint>
#include <cstring>
#if !defined(_M_IX86)
#error SmoothFollowPath requires the x86 native provider ABI
#endif
#pragma float_control(precise, on)
using namespace w5;
extern "C" {
int __fastcall kotor_fastcall_00634d40(void*, void*);
void __fastcall kotor_fastcall_006359c0(void*, void*);
void* __fastcall c2da_default_ref_00405ed0(void*, void*);
int __cdecl aurora_vector_equal_004aa980(const void*, const void*);
// 00833920 is in the zero-filled tail of original .data; no selected owner exists.
U w5_cell_follow_smooth_counter_00833920 = 0;
}
namespace {
const double k_distance2_00743308 = 1e-5; // original bits3EE4F8B588E368F1
const float k_one_0073d764 = 1.0f;
const float k_zero_0073d700 = 0.0f;
Vector* path(void* self, I index) {
    return static_cast<Vector*>(kotor_fastcall_00634cc0(self, nullptr, index));
}
I next(I index) { ++index; return index < 100 ? index : 0; }
// Every newly authored FP operation executes x87 directly. SSE DAZ/FTZ settings
// therefore do not govern these operations; the original runtime x87 CW still does.
float difference(float a, float b) {
    float value;
    __asm {
        fld a
        fsub b
        fstp value
    }
    return value;
}
float addition(float a, float b) {
    float value;
    __asm {
        fld a
        fadd b
        fstp value
    }
    return value;
}
// 00635CFE..00635D2E: exact retained second-read differences / first-read float stores.
bool line_distance_large(const Vector* from, const Vector* to, const Vector& stored) {
    const Vector* values = &stored;
    unsigned short status;
    __asm {
        mov ecx, from
        mov edx, to
        mov esi, values
        fld dword ptr [ecx]
        fsub dword ptr [edx]
        fld dword ptr [ecx+4]
        fsub dword ptr [edx+4]
        fld dword ptr [ecx+8]
        fsub dword ptr [edx+8]
        fld dword ptr [esi+8]
        fmul st(0), st(1)
        fxch st(2)
        fmul dword ptr [esi+4]
        faddp st(2), st(0)
        fxch st(2)
        fmul dword ptr [esi]
        faddp st(1), st(0)
        fcomp k_distance2_00743308
        fnstsw ax
        fstp st(0)
        mov status, ax
    }
    return (status & 0x4100u) == 0; // test AH,41: only ordered greater performs line test
}
// 00635FA0..00635FE3: retain Z in ST0 after FST dword; multiply its rounded store.
bool output_step_large(const Vector* target, const Vector& previous, Vector& delta) {
    const Vector* prior = &previous;
    Vector* differences = &delta;
    unsigned short status;
    __asm {
        mov ecx, target
        mov edx, prior
        mov esi, differences
        fld dword ptr [ecx]
        fsub dword ptr [edx]
        fstp dword ptr [esi]
        fld dword ptr [ecx+4]
        fsub dword ptr [edx+4]
        fstp dword ptr [esi+4]
        fld dword ptr [ecx+8]
        fsub dword ptr [edx+8]
        fst dword ptr [esi+8]
        fmul dword ptr [esi+8]
        fld dword ptr [esi+4]
        fmul dword ptr [esi+4]
        faddp st(1), st(0)
        fld dword ptr [esi]
        fmul dword ptr [esi]
        faddp st(1), st(0)
        fcomp k_one_0073d764
        fnstsw ax
        mov status, ax
    }
    return (status & 0x4100u) == 0;
}
bool difference_nonzero(float value) {
    unsigned short status;
    __asm {
        fld value
        fcomp k_zero_0073d700
        fnstsw ax
        mov status, ax
    }
    return (status & 0x4400u) != 0x4000u; // original test AH,44 / JP: only ordered equal preserves yaw
}
void store_yaw(float* out, const Vector* value) {
    __asm {
        mov eax, value
        push eax
        call kotor_m15a_yaw_004aa0f0
        add esp, 4
        mov ecx, out
        fstp dword ptr [ecx]
    }
}

}
extern "C" void __fastcall w5_t2_party_follow_smooth_path_00635af0(void* self, void*) {
    Vector points[201]{}; // original201 triples; append gate200 leaves one spare end slot
    I original_last = static_cast<I>(at<U>(self, 0xfa4) - 1u);
    if (original_last < 0) original_last = static_cast<I>(static_cast<U>(original_last) + 100u);
    const U leader_id = at<U>(at<void*>(self, 0xfa0), 0x24);
    const U server_id = kotor_fastcall_004aea30(server_app(), nullptr, leader_id);
    void* leader = kotor_fastcall_004ae770(server_app(), nullptr, server_id);
    if (!leader) return; // original00635B56
    const I closest = kotor_fastcall_00634d40(self, nullptr);
    I first = at<I>(at<unsigned char*>(self, 0xfa0) + closest * 0x88, 0x48);
    if (first >= 100 || first < 0) first = 0;
    copy_vector(&points[0], path(self, first));
    I scan = next(first);
    I count = 1;
    I last_good = scan; // [E+14], scan storage; reset before output rewrite
    U resref[4]; // [E+CC], full16-byte00405ED0 writes (only first byte cleared)
    c2da_default_ref_00405ed0(resref, nullptr);
    I result;
    for (;;) {
        if (scan == original_last || original_last == first) break;
        if (count >= 200) goto rewrite;
        WalkHit hit{}; // [E+84..CC); original48 bytes zeroed, blocker setinvalid
        hit.blocker_40 = 0x7f000000u;
        Vector* point = path(self, scan);
        Vector stored{difference(points[count - 1].x, point->x),
                      difference(points[count - 1].y, point->y),
                      difference(points[count - 1].z, point->z)};
        point = path(self, scan); // original second call and data re-read, not cached
        if (line_distance_large(&points[count - 1], point, stored)) {
            void* info = at<void*>(leader, 0x340);
            // 00635D37..3D: [leader+340] dereferenced unchecked as original.
            const float radius = at<float>(info, 4);
            const float band = at<float>(info, 0x14);
            point = path(self, scan); // original third call after pathfind values read
            result = w5_server_test_direct_line_004aee80(server_app(), nullptr, leader_id,
                resref, &points[count - 1], point, &radius, band, 1, &hit);
        } else result = 1;
        const U blocker = kotor_fastcall_005eda50(client_app(), nullptr, hit.blocker_40);
        if (result == -3) {
            void* party = at<void*>(self, 0xfa0);
            const I members = at<I>(party, 0);
            for (I member = 0; member < members; ++member) {
                if (at<U>(static_cast<unsigned char*>(party) + member * 0x88, 0x24) == blocker)
                    goto clear_line;
            }
        } else if (result == 1) {
clear_line:
            last_good = scan;
            scan = next(scan);
            continue;
        }
        if (aurora_vector_equal_004aa980(&points[count - 1], path(self, last_good))) {
            copy_vector(&points[count], path(self, scan));
            ++count;
            scan = next(scan);
        } else {
            copy_vector(&points[count], path(self, last_good));
            ++count;
        }
        if (count >= 200) goto rewrite;
        if (result == 0) break;
    }
    if (count < 200) {
        void* info = at<void*>(leader, 0x340);
        // 00635EA1..A7: [leader+340] dereferenced unchecked as original.
        const float radius = at<float>(info, 4);
        const float band = at<float>(info, 0x14);
        Vector* point = path(self, scan);
        result = w5_server_test_direct_line_004aee80(server_app(), nullptr, leader_id,
            resref, &points[count - 1], point, &radius, band, 1, nullptr);
        if (result != 1) {
            copy_vector(&points[count], path(self, last_good));
            ++count;
        }
        copy_vector(&points[count], path(self, scan));
        ++count;
    }
rewrite:
    last_good = 0; // 00635F5E PUSH arg;00635F61 [ESP+18] is E+14;00634CC0 RET4
    Vector previous;
    copy_vector(&previous, path(self, first));
    at<I>(self, 0xfa4) = first;
    if (count > 0) {
        Vector* cursor = &points[0];
        do {
            Vector delta;
            Vector point;
            if (output_step_large(cursor, previous, delta)) {
                Vector unit;
                normalize_004ab180(&unit, &delta);
                point = {addition(unit.x, previous.x), addition(unit.y, previous.y), addition(unit.z, previous.z)};
            } else {
                copy_vector(&point, cursor);
                ++last_good; // original E+14 progress advances only when a source point is consumed
                ++cursor;
            }
            const U index = at<U>(self, 0xfa4);
            void* out = static_cast<unsigned char*>(self) + index * 0x28u;
            copy_vector(out, &point);
            if (difference_nonzero(delta.x) || difference_nonzero(delta.y) || difference_nonzero(delta.z)) {
                // 006360B4..E5: stack operands move across two pushes; all three are point-previous.
                Vector yaw_delta{difference(point.x, previous.x),
                                 difference(point.y, previous.y), difference(point.z, previous.z)};
                Vector unit;
                store_yaw(&at<float>(out, 0xc), normalize_004ab180(&unit, &yaw_delta));
            } // all ordered-zero differences reload/preserve the slot's existing yaw
            at<I>(self, 0xfa4) = next(at<I>(self, 0xfa4));
            previous = point;
        } while (last_good < count);
    }
    U counter = w5_cell_follow_smooth_counter_00833920 + 6u;
    w5_cell_follow_smooth_counter_00833920 = static_cast<I>(counter) > 40 ? 0u : counter;
    kotor_fastcall_006359c0(at<void*>(self, 0xfa0), nullptr);
}
