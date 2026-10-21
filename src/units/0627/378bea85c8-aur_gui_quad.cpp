// lane-gui-render: GLRender::RenderObjectsVTQuadUP 00431EB0 / RenderObjectsVTCQuadUP 00431CC0
// (D49). Both are cdecl(float* buffer, int vertex_count), RET; the immediate-mode path ends with a
// tail jump to glEnd. GL entry points: the single adapter owner kotor_renderer_init (lane-renderer-
// bodies); extension cell glBindBufferARB 007BB7B4 read at every call (renderer_gl_procs.h).
#include "7cd3e01780-renderer_gl_procs.h"
#include <cstdint>

namespace kotor_renderer_init {
extern void __stdcall glEnableClientState_0073d260(std::uint32_t);
extern void __stdcall glDisableClientState_0073d264(std::uint32_t);
extern void __stdcall glTexCoordPointer_0073d29c(std::int32_t, std::uint32_t, std::int32_t, const void*);
extern void __stdcall glColorPointer_0073d2a0(std::int32_t, std::uint32_t, std::int32_t, const void*);
extern void __stdcall glVertexPointer_0073d2a8(std::int32_t, std::uint32_t, std::int32_t, const void*);
extern void __stdcall glDrawArrays_0073d2ac(std::uint32_t, std::int32_t, std::int32_t);
extern void __stdcall glEnd_0073d2c4();
extern void __stdcall glTexCoord2f_0073d2cc(float, float);
extern void __stdcall glBegin_0073d2d0(std::uint32_t);
extern void __stdcall glVertex3fv_0073d2e4(const float*);
extern void __stdcall glColor4ubv_0073d2e8(const std::uint8_t*);
// lane-renderer-bodies extension_loader.h (exact copy): 0045FC90.
extern std::uint32_t __stdcall VertexBufferAvailable_0045fc90();
extern std::int32_t VertexProgramsAvailable_0045f770();
}

namespace kotor_renderer_gl_cells { extern std::int32_t programs_0078e5ec; }

namespace {
namespace gl = kotor_renderer_init;
constexpr std::uint32_t kArrayBuffer = 0x8892u;     // push 8892h (GL_ARRAY_BUFFER_ARB)
constexpr std::uint32_t kVertexArray = 0x8074u;     // push 8074h
constexpr std::uint32_t kNormalArray = 0x8075u;     // push 8075h
constexpr std::uint32_t kColorArray = 0x8076u;      // push 8076h
constexpr std::uint32_t kTexCoordArray = 0x8078u;   // push 8078h
constexpr std::uint32_t kFloat = 0x1406u;           // push 1406h
constexpr std::uint32_t kUnsignedByte = 0x1401u;    // push 1401h
constexpr std::uint32_t kQuads = 7u;                // push 7 (immediate, not the 0073F34C cell)
// Fixed-function particle quads do not consume skin weights or indices. Some
// drivers still fetch enabled skin arrays after the vertex program is disabled.
// Establish this at each draw: intervening chunk/mesh draws may re-enable them.
void disable_skin_arrays() {
    if (!gl::VertexProgramsAvailable_0045f770()) return;
    if (kotor_renderer_gl_cells::programs_0078e5ec != 0) {
        kotor_renderer_gl_procs::glDisableVertexAttribArrayARB_007bb770(1);
        kotor_renderer_gl_procs::glDisableVertexAttribArrayARB_007bb770(4);
    } else {
        gl::glDisableClientState_0073d264(0x8651u); // GL_VERTEX_ATTRIB_ARRAY1_NV
        gl::glDisableClientState_0073d264(0x8654u); // GL_VERTEX_ATTRIB_ARRAY4_NV
    }
}
void bind_buffer_zero() {   // call [007BB7B4] with (8892h, 0)
    kotor_renderer_gl_procs::glBindBufferARB_007bb7b4(kArrayBuffer, 0u);
}
}

extern "C" {

// 00431EB0 GLRender::RenderObjectsVTQuadUP(buffer, count): vertex = {x, y, z, u, v} (20 bytes).
void __cdecl kotor_cdecl_00431eb0(float* buffer, std::int32_t count) {
    if (gl::VertexBufferAvailable_0045fc90() != 0) {                          // 00431EB1..00431EB8
        bind_buffer_zero();                                                   // 00431EBA..00431EC1
        gl::glEnableClientState_0073d260(kVertexArray);                       // 00431ECD..00431ED2
        gl::glEnableClientState_0073d260(kTexCoordArray);                     // 00431ED4..00431ED9
        gl::glDisableClientState_0073d264(kNormalArray);                      // 00431EE1..00431EE6
        gl::glDisableClientState_0073d264(kColorArray);                       // 00431EE8..00431EED
        gl::glTexCoordPointer_0073d29c(2, kFloat, 0x14, buffer + 3);          // 00431EEF..00431F00
        gl::glVertexPointer_0073d2a8(3, kFloat, 0x14, buffer);                // 00431F06..00431F10
        gl::glDrawArrays_0073d2ac(kQuads, 0, count);                          // 00431F16..00431F1F
        return;                                                               // 00431F25..00431F26
    }
    gl::glBegin_0073d2d0(kQuads);                                             // 00431F27..00431F2A
    if (count > 0) {                                                          // 00431F30..00431F36
        const float* vertex = buffer;
        std::int32_t remaining = count;
        do {
            gl::glTexCoord2f_0073d2cc(vertex[3], vertex[4]);                  // 00431F50..00431F57
            gl::glVertex3fv_0073d2e4(vertex);                                 // 00431F59..00431F5D
            vertex += 5;                                                      // 00431F5F
        } while (--remaining != 0);                                           // 00431F62..00431F63
    }
    gl::glEnd_0073d2c4();                                                     // 00431F69 jmp [0073D2C4]
}

// 00431CC0 GLRender::RenderObjectsVTCQuadUP(buffer, count): vertex = {x, y, z, rgba, u, v} (24 bytes).
void __cdecl kotor_cdecl_00431cc0(float* buffer, std::int32_t count) {
    disable_skin_arrays();
    if (gl::VertexBufferAvailable_0045fc90() != 0) {                          // 00431CC1..00431CC8
        bind_buffer_zero();                                                   // 00431CCE..00431CD5
        gl::glEnableClientState_0073d260(kVertexArray);                       // 00431CE1..00431CE6
        gl::glEnableClientState_0073d260(kColorArray);                        // 00431CE8..00431CED
        gl::glEnableClientState_0073d260(kTexCoordArray);                     // 00431CEF..00431CF4
        gl::glDisableClientState_0073d264(kNormalArray);                      // 00431CF6..00431CFB
        gl::glColorPointer_0073d2a0(4, kUnsignedByte, 0x18, buffer + 3);      // 00431D01..00431D12
        gl::glTexCoordPointer_0073d29c(2, kFloat, 0x18, buffer + 4);          // 00431D18..00431D25
        gl::glVertexPointer_0073d2a8(3, kFloat, 0x18, buffer);                // 00431D2B..00431D35
        gl::glDrawArrays_0073d2ac(kQuads, 0, count);                          // 00431D3B..00431D44
        bind_buffer_zero();                                                   // 00431D4A..00431D51
        return;                                                               // 00431D57..00431D58
    }
    gl::glBegin_0073d2d0(kQuads);                                             // 00431D59..00431D5C
    if (count > 0) {                                                          // 00431D62..00431D68
        const float* vertex = buffer;
        std::int32_t remaining = count;
        do {
            gl::glColor4ubv_0073d2e8(reinterpret_cast<const std::uint8_t*>(vertex + 3));   // 00431D80..00431D84
            gl::glTexCoord2f_0073d2cc(vertex[4], vertex[5]);                  // 00431D86..00431D8D
            gl::glVertex3fv_0073d2e4(vertex);                                 // 00431D8F..00431D93
            vertex += 6;                                                      // 00431D99
        } while (--remaining != 0);                                           // 00431D9C..00431D9D
    }
    gl::glEnd_0073d2c4();                                                     // 00431DA3 jmp [0073D2C4]
}

}   // extern "C"
