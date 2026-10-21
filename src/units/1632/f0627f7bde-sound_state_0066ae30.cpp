// S1 sound-state leaves: 005D6190, 005D6280 and 0066AE30.
//
// This is a standalone source delivery.  It has no imports, no allocator use,
// no globals, no exception frame and no dependence on another lane's files.
// The object is intended to be added to the selected composition; it does not
// replace any existing selected symbol because the three addresses have no
// selected body or donor body (see EVIDENCE.md and INTEGRATION.md).
//
// Observed original ABI:
//   005D6190  ECX=source, EDX ignored, RET0 -> read [source+4], then [+0x24].
//   005D6280  ECX=source, EDX ignored, RET0 -> read [source],   then [+4].
//   0066AE30  ECX=self, EDX ignored, one stack dword id, RET4.
//
// The original getters dereference their receiver with no null-self guard.  The
// address arithmetic below uses uintptr_t only so that a null/malformed receiver
// still reaches the original faulting access instead of becoming a C++ pointer
// arithmetic undefined-behaviour assumption.  It does not alias any absolute
// original address and no original address is dereferenced.
#include "b66892a3de-sound_state_0066ae30.h"

#include <cstdint>
#include <cstring>

namespace {
using U32 = std::uint32_t;
using U = std::uintptr_t;

U32 load_u32(const void* base, U offset) {
    U32 value;
    std::memcpy(&value, reinterpret_cast<const void*>(reinterpret_cast<U>(base) + offset), sizeof value);
    return value;
}

void* load_ptr(const void* base, U offset) {
    void* value;
    std::memcpy(&value, reinterpret_cast<const void*>(reinterpret_cast<U>(base) + offset), sizeof value);
    return value;
}
} // namespace

extern "C" {

// 005D6190: EAX = [ECX+4]; if null, EAX = 0; else EAX = [EAX+0x24]; RET.
// The raw dword is returned, not a normalized bool.
std::uint32_t __fastcall kotor_m15_sound_source_get_looping_005d6190(void* source, void*) {
    void* const internal = load_ptr(source, 4);
    if (internal == nullptr) return 0;
    return load_u32(internal, 0x24);
}

// 005D6280: EAX = [ECX]; if null, EAX = 0; else EAX = [EAX+4]; RET.
// This is the streaming-source counterpart, not a fallback to 005D6190.
std::uint32_t __fastcall kotor_m15_stream_source_get_looping_005d6280(void* source, void*) {
    void* const internal = load_ptr(source, 0);
    if (internal == nullptr) return 0;
    return load_u32(internal, 4);
}

// 0066AE30 original order in the authenticated R2 range 0066AE30..0066AE90:
//   1. if [self+0x104] == id, return with no writes;
//   2. [self+0x14C] = 0;
//   3. [self+0x104] = id;
//   4. if id == 0, return;
//   5. if [self+0x168] == 0 and [self+0xFC] != 0, read 005D6190([self+0xFC]);
//      otherwise, if [self+0x100] != 0, read 005D6280([self+0x100]);
//   6. only if that read is nonzero, [self+0x128] = 1.
//
// Store order is preserved with volatile stores: the reset and the id store are
// the two documented writes and remain ordered under optimization.  No thread
// or hardware semantics are claimed; volatile is used only as a compiler-order
// barrier here.  The original caller at 0066B4F7 ignores EAX, so this setter is
// void and does not invent a success/failure return value.
void __fastcall recovered_client_sound_object_set_sound_id_0066ae30(void* self, void*, U32 sound_id) {
    if (load_u32(self, 0x104) == sound_id) return;

    *reinterpret_cast<volatile U32*>(reinterpret_cast<U>(self) + 0x14C) = 0;
    *reinterpret_cast<volatile U32*>(reinterpret_cast<U>(self) + 0x104) = sound_id;

    if (sound_id == 0) return;

    if (load_u32(self, 0x168) == 0) {
        void* const source = load_ptr(self, 0xFC);
        if (source != nullptr) {
            if (kotor_m15_sound_source_get_looping_005d6190(source, nullptr) != 0)
                *reinterpret_cast<volatile U32*>(reinterpret_cast<U>(self) + 0x128) = 1;
            return;
        }
    }

    void* const stream = load_ptr(self, 0x100);
    if (stream == nullptr) return;
    if (kotor_m15_stream_source_get_looping_005d6280(stream, nullptr) != 0)
        *reinterpret_cast<volatile U32*>(reinterpret_cast<U>(self) + 0x128) = 1;
}

} // extern "C"
