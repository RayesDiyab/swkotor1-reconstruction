// lane-m48-a: ImposterManager::AllocateImposter 00425AF0 (__thiscall(index, count, points) RET 0Ch; EAX unspecified,
// callers 004320E4..004320F1 / 00432139..00432149 discard it). Consumer spelling (3d/m47/m47_gob2.cpp:252):
//   extern "C" void __fastcall imposter_manager_allocate_imposter_00425af0(ImposterManager*, void*, I index, U a, U b)
// Evidence: swkotor.exe dumpbin /disasm 00425AF0..00425C29 (lane-m48-a/d/00425AF0.txt).
// Record (0x1C bytes, ImposterManager data[index]): +00 byte in-use, +04 texture name, +08 pbuffer GL context,
// +0C pbuffer handle, +10 pbuffer DC, +14 copied point array (count*12 bytes), +18 count.
// GL/WGL: original IAT calls go through the single adapter owner (windows_gl_imports.cpp, kotor_renderer_init);
// WGL_ARB_pbuffer / pixel-format entry points through the typed extension cells (renderer_gl_procs.h).
// Allocation: 006FC0D2 = jmp 006FA7E6 operator new -> kotor_menu_10_19::dependencies::allocate_006fa7e6 (joined onto
// the live SBH allocator; original allocator family). The original does not test the result (rep movs into it).
#include "536853f13e-renderer_gl_cells.h"
#include "2078381901-renderer_gl_imports.h"
#include "f13dfb9d7c-renderer_gl_procs.h"
#include <cstdint>
#include <cstring>

namespace kotor_menu_10_19 { namespace dependencies { void* allocate_006fa7e6(unsigned int); } } // 006FA7E6 (joined)

namespace {
using U = std::uint32_t; using I = std::int32_t;
struct ImposterManagerView { U data; I count; };          // 0078DB30 {data, count} (m47_gob2.cpp storage)
template<class T> T& at(U base, U offset) { return *reinterpret_cast<T*>(static_cast<std::uintptr_t>(base + offset)); }
}

extern "C" void __fastcall imposter_manager_allocate_imposter_00425af0(ImposterManagerView* manager, void* /*unused_edx*/,
                                                                       I index, U count, U points) {
    namespace cells = kotor_renderer_gl_cells;
    namespace procs = kotor_renderer_gl_procs;
    using namespace kotor_renderer_init;
    const U main_context = cells::main_context_007a6854;                 // 00425AF3 mov eax,[007A6854] -> [esp+10h]
    void* const main_dc = reinterpret_cast<void*>(static_cast<std::uintptr_t>(cells::main_dc_007a47e4)); // 00425AF9 ebx
    const U record = static_cast<U>(index) * 0x1Cu + manager->data;      // 00425B00..00425B10 ebp = index*1Ch + [ecx]
    at<U>(record, 0x18) = count;                                         // 00425B12 mov [ebp+18h],esi
    const U bytes = count * 12u;                                         // 00425B15..00425B19 lea esi,[esi+esi*2]; shl 2
    at<std::uint8_t>(record, 0) = 1;                                     // 00425B1D mov byte ptr [ebp],1
    void* const copy = kotor_menu_10_19::dependencies::allocate_006fa7e6(bytes); // 00425B1C..00425B21 call 006FC0D2
    at<U>(record, 0x14) = static_cast<U>(reinterpret_cast<std::uintptr_t>(copy)); // 00425B31 mov [ebp+14h],eax
    std::memcpy(copy, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(points)), bytes); // 00425B26..00425B44 rep movsd/movsb
    U texture = 0;                                                       // [esp+44h]: the index argument slot is reused
    glGenTextures_0073d24c(1, &texture);                                 // 00425B40..00425B4D
    at<U>(record, 4) = texture;                                          // 00425B53..00425B57
    glBindTexture_0073d240(0x0DE1u, texture);                            // 00425B5A..00425B64 GL_TEXTURE_2D
    cells::current_tid_007a683c[cells::current_stage_007a6898] = 0;      // 00425B6A..00425B7C (unchecked index)
    I format_attributes[5] = {0x202D, 1, 0x2010, 1, 0};                  // 00425B8E..00425BA6 DRAW_TO_PBUFFER, SUPPORT_OPENGL
    float float_attributes[1] = {0.0f};                                  // 00425BAA mov [esp+2Ch],edi (dword 0)
    I format = 0; U formats = 0;                                         // [esp+1Ch] / [esp+20h] out cells (written by the driver)
    (*procs::wglChoosePixelFormatARB_007bb730)(main_dc, format_attributes, float_attributes, 1u, &format, &formats); // 00425B6F..00425BAE
    I pbuffer_attributes[3] = {0x2033, 1, 0};                            // 00425BC9..00425BD5 WGL_PBUFFER_LARGEST_ARB
    void* const pbuffer = (*procs::wglCreatePbufferARB_007bb530)(main_dc, format, 0x100, 0x100, pbuffer_attributes); // 00425BB4..00425BD9
    void* const pbuffer_dc = (*procs::wglGetPbufferDCARB_007bb6b8)(pbuffer); // 00425BDF..00425BEA (pbuffer kept in [esp+48h])
    void* const context = wglCreateContext_0073d2b8(pbuffer_dc);         // 00425BEC..00425BF7
    wglShareLists_0073d2b4(reinterpret_cast<void*>(static_cast<std::uintptr_t>(main_context)), context); // 00425BF3..00425BFB
    wglMakeCurrent_0073d2b0(pbuffer_dc, context);                        // 00425C01..00425C03
    at<U>(record, 8) = static_cast<U>(reinterpret_cast<std::uintptr_t>(context));     // 00425C13 mov [ebp+8],edi
    at<U>(record, 0x10) = static_cast<U>(reinterpret_cast<std::uintptr_t>(pbuffer_dc)); // 00425C16 mov [ebp+10h],esi
    at<U>(record, 0x0C) = static_cast<U>(reinterpret_cast<std::uintptr_t>(pbuffer));   // 00425C19 mov [ebp+0Ch],edx
    wglMakeCurrent_0073d2b0(main_dc, reinterpret_cast<void*>(static_cast<std::uintptr_t>(main_context))); // 00425C09..00425C1C
}                                                                        // 00425C29 ret 0Ch
