// lane-3d-construct (sub-area "construction"): AurTextureGet 00423490.
// AurTextureGetReference 00423A60 (two calls of this function) is owned by lane-texture-epoch (D39) and is NOT
// defined here; it reaches this body through the task-33 font.hpp declaration `get_00423490`.
// Evidence: dumpbin /disasm swkotor.exe 00423490..004235EC (lane-local d/00423490.txt).
// Types: lane-texture-epoch texture.hpp (texture::Basic 236 bytes, Ref = font::Texture 24 bytes) and texture_cells.hpp
// (aur_refresh_textures_007a4770 storage owner). Allocator: 006FA7E6 -> live_operator_new (M41).
#include "5fcd820543-texture.hpp"
#include "80578872a3-texture_cells.hpp"
#include "ac41715f26-growth.h"
#include <cstdint>

namespace {
using texture::U; using texture::I; using texture::Ref; using texture::Basic;
// Address arithmetic exactly as the original's `lea reg,[base+disp]` on a possibly-null base (implementation-
// defined pointer<->integer mapping on x86; reproduces the original's null-allocation behaviour without UB in
// the pointer arithmetic itself).
template<class T> inline T* displaced(const void* base, U displacement) {
    return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(base) + displacement);
}
}

extern "C" {
// 00423490 AurTextureGet(name, existing, alternate, vector, count) -> CAurTexture*   (__cdecl, RET 0)
//   stack at entry+4..+14h: [esp+38h] name, [esp+3Ch] existing, [esp+40h] alternate, [esp+44h] vector, [esp+48h] count
//   (frame: SEH 3 words + sub 20h + esi/edi); EH FuncInfo 007157F6 (unwind of the two `new` allocations: not emitted,
//   neither constructor can throw - D30-FINAL unwind rule; original operator new returns NULL).
font::Texture* __cdecl get_00423490(const char* name, font::Texture* existing, const char* alternate, void* vector, U count) {
    char derived[32];
    derived[0] = 0;                                                    // 004234B0 mov byte [esp+8],0
    if (existing == nullptr) {                                         // 004234AE test edi,edi; jne 0042353E
        // 004234BB..004234DA: the alternate name when non-null and non-empty, else the name (unbounded byte copy,
        // terminator included; the original has no length check: a name of 32+ bytes overruns the frame).
        const char* from = (alternate != nullptr && alternate[0] != 0) ? alternate : name;
        char* to = derived;
        char c;
        do { c = *from++; *to++ = c; } while (c != 0);                 // 004234D2..004234DA / 004234E6..004234EE
        Ref* const found = texture::find_00420ac0(name, derived, static_cast<const U*>(vector),
                                                  static_cast<I>(count)); // 004234F0..00423505 (name, derived, vector, count)
        if (found != nullptr) return found;                            // 00423508 jne 004235DC (EAX = found)
        void* const memory = live_operator_new(0xEC);                  // 00423510..0042351A operator new(0ECh)
        Ref* texture_ref = nullptr;                                    // 00423589 xor esi,esi (allocation failed)
        if (memory != nullptr)                                         // 00423521 test eax,eax
            texture_ref = texture::basic_ctor_00423150(memory, nullptr, name, derived); // 0042352D..0042353A ctor(name, derived), ESI = EAX
        // 0042358B..0042359C CAurTextureBasic::Init(this = ESI, ESI+98h): the derived-name member of the new texture.
        // With a NULL allocation the original calls Init(this = 0, 98h) (no test) - reproduced by address arithmetic.
        // Ref is the first member of the standard-layout Basic, so the Ref* returned by the ctor and the Basic* are
        // pointer-interconvertible (same address, as ESI is reused by the original).
        Basic* const basic = reinterpret_cast<Basic*>(texture_ref);
        (void)texture::init_00422af0(basic, nullptr, displaced<const char>(texture_ref, 0x98)); // EAX result dropped
        // 004235A1..004235DA CExoArrayList<CAurTextureBasic*>::Add on aurRefreshTextures 007A4770 (inlined):
        font::List& refresh = texture_cells::aur_refresh_textures_007a4770;
        const I capacity = refresh.capacity;                           // 004235A1 mov eax,[007A4778]
        if (refresh.size == capacity) {                                // 004235A6 cmp [007A4774],eax; jne
            menu_list_growth::allocate_005e03c0(&refresh, nullptr,
                static_cast<U>(capacity != 0 ? capacity + capacity : 8)); // 004235AE..004235C1 (add eax,eax | 8)
        }
        const U data = refresh.data;                                   // 004235C6 mov eax,[007A4770]
        const I size = refresh.size;                                   // 004235CB mov ecx,[007A4774]
        *reinterpret_cast<Ref* volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(size) * 4u) = texture_ref; // 004235D1
        refresh.size = refresh.size + 1;                               // 004235D4 inc dword [007A4774]
        return texture_ref;                                            // 004235DA mov eax,esi
    }
    // 0042353E..00423588: a reference wrapper around an existing texture.
    void* const memory = live_operator_new(0x18);                      // 0042353E..00423545 operator new(18h)
    Ref* wrapper = nullptr;                                            // 00423561 xor eax,eax
    if (memory != nullptr)                                             // 0042354C test eax,eax
        wrapper = texture::reference_ctor_00422e30(memory, nullptr);   // 00423558..0042355A CAurTexture::CAurTexture
    // 00423563..00423571 SafePointer<CAurObject>::operator=(wrapper+10h, existing): lea ecx,[eax+10h] with no null
    // test (a NULL allocation assigns through address 10h, as the original). Ref+10h is Ref::reference.
    (void)font::assign_00447fa0(displaced<font::Safe>(wrapper, 0x10), nullptr, existing);
    return wrapper;                                                    // 00423577 mov eax,esi
}
}
static_assert(offsetof(font::Texture, reference) == 0x10 && offsetof(texture::Basic, derived) == 0x98,
              "00423564 lea ecx,[eax+10h] / 0042358B lea edx,[esi+98h]");
