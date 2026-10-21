// lane-m1-opt-gameplay (fork: key mappings): CExoInput keyboard-capture leaves used by
// CSWGuiInGameOptKeyMappings::Update 006EE040 and not linked by M54 under any name:
//   005DF630 CExoInput::GetLastCapturedKeyboardKey (MOV ECX,[ECX+4]; JMP 005E0230)
//   005E0230 CExoInputInternal::GetLastCapturedKeyboardKey
// Reused lane-input owners (no second copy): CExoRawInputInternal::GetKeyboardBuffer 005E3AE0
// (kih::RawInput::GetKeyboardBuffer, m48t-f-input-input-events-frame.obj), CExoDeviceBuffer ctor/dtor
// 005C5A30 / 005E2FF0 (kih::DeviceBuffer, m48t-f-input-input-frame.obj); object views from lane-input input_system.h.
// Handoff: lane-input may adopt these two bodies (same symbols) as their single owner.
// Original: swkotor.exe GOG K1, static disassembly only.
#include "62dd3107f7-opt_keymap.h"
#include "99def0a2a7-input_system.h"

namespace kih = kotor_input_hover;

namespace {
// .rdata constants of the image (read-only): first / end key ids of the keyboard key table (0074D3D8 = 6,
// 0074D3DC = 6Eh), as recorded by lane-input's generated input_key_ids.h (k_0074d3d8 / k_0074d3dc).
constexpr int kFirstKey_0074d3d8 = 6;
constexpr int kEndKey_0074d3dc = 0x6E;
// EH state 0 of 005E0230 (FuncInfo 00724728): ~CExoDeviceBuffer on unwind and on both returns.
struct BufferScope {
    kih::DeviceBuffer buffer;
    BufferScope() { buffer.Construct005c5a30(); }                                  // 005E024C..50
    ~BufferScope() { buffer.Destroy005e2ff0(); }                                   // 005E02CA..D6 / 005E02F0..FC
};
}

extern "C" {
// ---- 005E0230 (thiscall, RET): last key-table index (6..6Dh) whose DirectInput keyboard record has the
// pressed bit (byte +4 sign) in this frame's buffer; 0 when raw input is absent or capture is off.
gui_load::Word __fastcall kotor_m1_opt_input_internal_last_captured_key_005e0230(void* object, void*) noexcept(false) {
    kih::InputInternal* const self = static_cast<kih::InputInternal*>(object);
    BufferScope scope;
    gui_load::Word result = 0;                                                     // 005E025B..63
    if (self->raw == nullptr) return result;                                       // 005E0255..67
    if (self->capture_keyboard_keys == 0) return result;                           // 005E026D..73
    self->raw->GetKeyboardBuffer(&scope.buffer);                                   // 005E0275..7A (return value unused)
    const kih::U32 count = scope.buffer.count;                                     // 005E027F
    for (kih::U32 n = 0; n < count; ++n) {                                         // 005E0283..C6 (unsigned)
        const DIDEVICEOBJECTDATA& record = scope.buffer.data[n];                   // 005E028F / 005E02C2 (14h stride)
        if (static_cast<signed char>(static_cast<unsigned char>(record.dwData)) >= 0) continue;   // 005E0295..9A
        for (int i = kFirstKey_0074d3d8; i < kEndKey_0074d3dc; ++i) {              // 005E029C..BA
            if (record.dwOfs == self->keys[i]) {                                   // 005E02A5..B2 (+164 table)
                result = static_cast<gui_load::Word>(i);                           // 005E02BE
                break;
            }
        }
    }
    return result;                                                                 // 005E02DB
}
// ---- 005DF630 (thiscall, RET): forwards to the internal object.
gui_load::Word __fastcall kotor_m1_opt_input_last_captured_key_005df630(void* input, void*) noexcept(false) {
    return kotor_m1_opt_input_internal_last_captured_key_005e0230(static_cast<kih::Input*>(input)->internal, nullptr);   // 005DF630..33
}
}
