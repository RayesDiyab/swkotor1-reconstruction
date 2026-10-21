// Original swkotor.exe 005EDBC0 CClientExoApp::IsSoundPlayingInDialog.
//
// Entry-to-return reconstruction. Original thiscall shape: ECX =
// CClientExoApp*, no stack arguments, RET 0, int EAX. The original body is
// `mov ecx,[ecx+4]; jmp 005F2D00`; the source projection is the tail-call
// expression below. Native review must show a tail jmp/no extra return frame;
// a call/ret pair is not accepted.
//
// This TU defines only 005EDBC0. 005F2D00 is a separate owned TU; no provider
// is stubbed or trapped here.

#include <cstdint>

static_assert(sizeof(void*) == 4, "original x86 ABI");

extern "C" int __fastcall kotor_fastcall_005f2d00(void*, void*);  // original 005F2D00

namespace {

inline void* load_ptr(const void* base, std::uintptr_t offset) {
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(base) + offset;
    return *reinterpret_cast<void* const*>(address);
}

}  // namespace

extern "C" int __fastcall kotor_fastcall_005edbc0(void* client, void* /*unused_edx*/) {
    // 005EDBC0..C3: mov ecx,[ecx+4]; jmp 005F2D00.
    return kotor_fastcall_005f2d00(load_ptr(client, 4), nullptr);
}
