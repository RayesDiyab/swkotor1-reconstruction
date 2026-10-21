// 0066B310 CSWCSoundObject::SetPosition.
//
// Original body: 0066B310..0066B352 (one stack argument, RET 4).
//   0066B319  call 0063F340(this, position) unconditionally
//   0066B31E  [this+0x168] == 0 ?
//   0066B328  [this+0xFC] != 0 ? call 005D59E0([this+0xFC], position, 0), return
//   0066B33F  otherwise [this+0x100] != 0 ? call 005D62C0([this+0x100], position)
//   0066B351  ret 4
//
// The function does not copy the three floats itself, does not add a null guard
// on position, and does not read +0xFC when +0x168 is nonzero.
#include "b602527e78-sound_position_0066b310.h"

// Explicit global-scope using-declarations: a using-directive inside an unnamed
// namespace would not make these helpers visible in the global-scope body.
using r4_sound_corpse::load_ptr;
using r4_sound_corpse::load_u32;

extern "C" void __fastcall kotor_sound_object_set_position_0066b310(void* self, void*,
                                                                    const void* position) noexcept(false) {
    // 0066B316..0066B319: always first, with the original position pointer.
    area_vfx_base_set_position_0063f340(self, nullptr, position);

    // 0066B31E..0066B330: +0x168 nonzero skips the sound-source branch; a zero
    // +0xFC also falls through. +0xFC is not read when +0x168 is nonzero.
    if (load_u32(self, 0x168) == 0) {
        void* const source = load_ptr(self, 0xFC);
        if (source != nullptr) {
            // 0066B332..0066B33C: push 0 then position; RET 4, no stream call.
            kotor_m15_sound_source_set_position_005d59e0(source, nullptr, position, 0.0f);
            return;
        }
    }

    // 0066B33F..0066B351: the stream branch is still reached when +0x168 is
    // nonzero or when +0xFC was null. A null +0x100 does nothing.
    void* const stream = load_ptr(self, 0x100);
    if (stream != nullptr) {
        kotor_m15_stream_source_set_position_005d62c0(stream, nullptr, position);
    }
}
