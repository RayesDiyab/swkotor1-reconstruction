// lane-world-los: GLRender drawable-square list used by CSWCollisionMesh::NoNonWalkPolys debug geometry.
// Evidence: lane-world-los\asm\00435B40.asm (incl. 00435B90), 0044EFB0.asm, 0044F050.asm.
#include "dc46124e28-world_los.h"

namespace {
using namespace kotor_world_los;
inline U* square_data() { return reinterpret_cast<U*>(static_cast<std::uintptr_t>(drawable_squares_007a693c.data)); }
}

extern "C" {

// 00435B40 GLRender::AddSquareToRender (cdecl).
void __cdecl gl_render_add_square_00435b40(U handle) {
    if (handle == 0xFFFFFFFFu) return;                                       // 00435B45..00435B48
    const I capacity = drawable_squares_007a693c.capacity;                   // 00435B4A
    if (drawable_squares_007a693c.size == capacity) {                        // 00435B4F..00435B55
        const U grown = capacity != 0 ? static_cast<U>(capacity) + static_cast<U>(capacity) : 8u; // 00435B57..00435B5F
        kotor_renderer_gl::allocate_00437d60(&drawable_squares_007a693c, nullptr, grown);          // 00435B64..00435B6A
    }
    square_data()[drawable_squares_007a693c.size] = handle;                   // 00435B6F..00435B7A
    drawable_squares_007a693c.size = drawable_squares_007a693c.size + 1;     // 00435B7D inc dword
}

// 00435B90 GLRender::RemoveSquareFromRender (cdecl; reached by tail jmp from 0044F05D).
void __cdecl gl_render_remove_square_00435b90(U handle) {
    if (handle == 0xFFFFFFFFu) return;                                       // 00435B95..00435B98
    const I size = drawable_squares_007a693c.size;                           // 00435B9A (read once)
    I matches = 0;                                                           // 00435BA1
    if (size > 0) {                                                          // 00435BA5..00435BA7
        const U* const data = square_data();                                 // 00435BA9 (read once)
        for (I index = 0; index < size; ++index)                             // 00435BB0..00435BB9
            if (data[index] == handle) ++matches;
        if (matches != 0)                                                    // 00435BBB..00435BBD
            texture_list_remove_00445080(&drawable_squares_007a693c, nullptr, handle); // 00435BBF..00435BC5
    }
}

// 0044EFB0 AurAddDrawableSquare(corners: 4 x 12-byte vectors, color) -> pool handle (cdecl).
U __cdecl aur_add_drawable_square_0044efb0(const float* corners12, U color) {
    U handle;                                                                // [esp+10h] slot (push ecx)
    // 0044EFB5..0044EFC2: RequestPool(0, 50h, 0, 11h, &handle); 0044EFCE stores the returned EAX over the slot.
    handle = aurora_pool_request_00423ee0(0, 0x50, 0, 0x11, &handle);
    // 0044EFC7..0044EFD2: LockPool(handle, 0, 0, 1) -> vertex memory (5 x {x,y,z,color}).
    U* const vertices = static_cast<U*>(aurora_pool_lock_00423f10(handle, 0, 0, 1));
    // 0044EFE2..0044F00A: four corners, raw DWORD copies, color at +0C of each 16-byte vertex.
    for (U corner = 0; corner < 4; ++corner) {
        std::memcpy(&vertices[corner * 4], &corners12[corner * 3], 12);
        vertices[corner * 4 + 3] = color;
    }
    // 0044F00C..0044F020: fifth vertex closes the loop with corner 0 ([ebx] = corners+8 was never advanced).
    std::memcpy(&vertices[16], &corners12[0], 4);
    std::memcpy(&vertices[17], &corners12[1], 4);
    std::memcpy(&vertices[18], &corners12[2], 4);
    vertices[19] = color;
    (void)aurora_pool_unlock_00423f30(handle);                                // 0044F023..0044F028
    gl_render_add_square_00435b40(handle);                                   // 0044F02D..0044F032
    return handle;                                                           // 0044F037
}

// 0044F050 AurRemoveDrawableSquare (cdecl): -1 -> return; else tail jmp 00435B90 with the same argument.
void __cdecl aur_remove_drawable_square_0044f050(U handle) {
    if (handle == 0xFFFFFFFFu) return;                                       // 0044F054..0044F057
    gl_render_remove_square_00435b90(handle);                                // 0044F059..0044F05D
}

} // extern "C"
