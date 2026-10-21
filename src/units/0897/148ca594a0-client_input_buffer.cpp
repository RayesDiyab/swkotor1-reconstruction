// lane-m48-b fork CLIENT, unit client-input-buffer: CExoInputInternal::BufferEvent(CExoInputEvent*) 005E12A0 (routes
// window_leaves 005DF590 wrapper: WndProc mouse messages, reachable on the menu) and BufferEvent(dev, ctl, value) 005E08C0.
// Evidence: dumpbin /disasm swkotor.exe 005E12A0..005E1708 with jump table 005E170C (11 entries, message - 200h),
// 005E08C0..005E092C. .rdata words (dumpbin /rawdata): 0074D3CC = 1 (mouse device), 0074D3E0 = 0, 0074D3E4 = 1,
// 0074D3E8 = 2 (left/right/middle button controls), 0074D3EC = 3, 0074D3F0 = 4 (x/y axis), 0074D3F4 = 5 (wheel).
// The raw-event description table 0074D5FC is the published typed table (input-hover input_system.h declaration; owner
// = the unit defining ?raw_event_desc_vtable_0074d5fc@@3URawEventDescVtable@kotor_input_hover@@B, lane-m48-b gui fork).
// Return value: the original returns EAX 1 for handled messages and 0 otherwise (005E16F9 / 005E1705); the only
// consumer (routes window_leaves.cpp kotor_fastcall_005df590 and its declaration kotor_original_calls.h:96) is void,
// so the definition follows the consumer declaration (EAX not observed by any staged caller).
#include <cstdint>
#include <cstring>

namespace kotor_input_hover { struct RawEventDescVtable; }
extern const kotor_input_hover::RawEventDescVtable raw_event_desc_vtable_0074d5fc;
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { struct Node; }
namespace ini_addhead { base_ini_reconstruction::Node* __fastcall AddHead005e9d70(kotor_list::Internal*, void*, void*); }
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void __fastcall kotor_fastcall_005e08c0(void* internal, void*, std::uint32_t device, std::uint32_t control, std::uint32_t value);
void __fastcall kotor_fastcall_005e12a0(void* internal, void*, void* event);
}

namespace {
using U32 = std::uint32_t; using I32 = std::int32_t;
U32 w(const void* p, U32 o) { U32 v; std::memcpy(&v, static_cast<const unsigned char*>(p) + o, 4); return v; }
void sw(void* p, U32 o, U32 v) { std::memcpy(static_cast<unsigned char*>(p) + o, &v, 4); }
std::uint16_t h(const void* p, U32 o) { std::uint16_t v; std::memcpy(&v, static_cast<const unsigned char*>(p) + o, 2); return v; }
const U32 k_mouse_0074d3cc = 1, k_left_0074d3e0 = 0, k_right_0074d3e4 = 1, k_middle_0074d3e8 = 2;
const U32 k_axis_x_0074d3ec = 3, k_axis_y_0074d3f0 = 4, k_wheel_0074d3f4 = 5;
}

// 005E08C0 CExoInputInternal::BufferEvent(device, control, value), RET 0Ch: 10h-byte node {value, 0, 0, desc} and a
// 20h-byte description {0074D5FC, 0, 0, 0, 0, 0, device, control}, added at the head of list [this+374h].
extern "C" void __fastcall kotor_fastcall_005e08c0(void* internal, void*, U32 device, U32 control, U32 value) {
    unsigned char* node = static_cast<unsigned char*>(live_operator_new(0x10)); // 005E08C3..005E08C7 (no null test)
    sw(node, 0, value);                                                 // 005E08CE..005E08D6
    sw(node, 8, 0); sw(node, 4, 0);                                     // 005E08D8..005E08DB
    unsigned char* desc = static_cast<unsigned char*>(live_operator_new(0x20)); // 005E08D4..005E08DE
    if (desc) {                                                         // 005E08E6..005E08E8
        const void* table = &raw_event_desc_vtable_0074d5fc;            // 005E08EA (typed table object, D24 object pointer)
        std::memcpy(desc, &table, 4);
        sw(desc, 4, 0); sw(desc, 8, 0); sw(desc, 0x0c, 0);              // 005E08F0..005E08F6
    }
    std::memcpy(node + 0x0c, &desc, 4);                                 // 005E0901
    sw(desc, 0x18, device);                                             // 005E08FD..005E0904
    sw(desc, 0x1c, control);                                            // 005E090A..005E090E
    sw(desc, 0x10, 0);                                                  // 005E0914
    sw(desc, 0x14, 0);                                                  // 005E091A
    void* list; std::memcpy(&list, static_cast<unsigned char*>(internal) + 0x374, 4); // 005E091D
    ini_addhead::AddHead005e9d70(static_cast<kotor_list::Internal*>(list), nullptr, node); // 005E0923..005E0924
}

// 005E12A0 CExoInputInternal::BufferEvent(CExoInputEvent*), RET 4. Event view: +4 message, +0A wheel delta (word),
// +0C x (word), +0E y (word). [this+384h] = suppress-next flag, [this+37Ch]/[this+380h] = last cursor x/y.
extern "C" void __fastcall kotor_fastcall_005e12a0(void* internal, void*, void* event) {
    const U32 index = w(event, 4) - 0x200u;                             // 005E12A8..005E12AB
    if (index > 0x0a || index == 9) return;                             // 005E12B0..005E12B5 / table[9] = 005E1702 (EAX 0)
    if (index == 0) {                                                   // 005E12C2 (WM_MOUSEMOVE)
        if (w(internal, 0x384) != 0) { sw(internal, 0x384, 0); return; } // 005E12C2..005E12DF
        sw(internal, 0x37c, static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(h(event, 0x0c))))); // 005E12E2..005E12E6
        sw(internal, 0x380, static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(h(event, 0x0e))))); // 005E12EC..005E12F1
        return;
    }
    U32 control = 0, value = 0;
    bool unsigned_xy = false;
    switch (index) {
    case 1: control = k_left_0074d3e0; value = 1; break;                // 005E1302 WM_LBUTTONDOWN
    case 2: control = k_left_0074d3e0; value = 0; break;                // 005E13E7 WM_LBUTTONUP
    case 3: control = k_left_0074d3e0; value = 0xFFFFFFFFu; break;      // 005E1377 WM_LBUTTONDBLCLK
    case 4: control = k_right_0074d3e4; value = 1; break;               // 005E1453 WM_RBUTTONDOWN
    case 5: control = k_right_0074d3e4; value = 0; break;               // 005E1538 WM_RBUTTONUP
    case 6: control = k_right_0074d3e4; value = 0xFFFFFFFFu; break;     // 005E14C8 WM_RBUTTONDBLCLK
    case 7: control = k_middle_0074d3e8; value = 1; unsigned_xy = true; break; // 005E1617 WM_MBUTTONDOWN (movzx)
    case 8: control = k_middle_0074d3e8; value = 0; unsigned_xy = true; break; // 005E1687 WM_MBUTTONUP (movzx)
    case 10: control = k_wheel_0074d3f4;                                // 005E15A4 WM_MOUSEWHEEL
             value = static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(h(event, 0x0a)))); break; // 005E15EF / 005E1608
    }
    if (w(internal, 0x384) == 0) {                                      // e.g. 005E1302..005E130A
        const U32 x = unsigned_xy ? static_cast<U32>(h(event, 0x0c)) : static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(h(event, 0x0c))));
        const U32 y = unsigned_xy ? static_cast<U32>(h(event, 0x0e)) : static_cast<U32>(static_cast<I32>(static_cast<std::int16_t>(h(event, 0x0e))));
        kotor_fastcall_005e08c0(internal, nullptr, k_mouse_0074d3cc, k_axis_x_0074d3ec, x); // e.g. 005E130C..005E1324
        kotor_fastcall_005e08c0(internal, nullptr, k_mouse_0074d3cc, k_axis_y_0074d3f0, y); // e.g. 005E1329..005E1339
        sw(internal, 0x37c, 0xFFFFFFFFu);                               // e.g. 005E133E..005E1341
        sw(internal, 0x380, 0xFFFFFFFFu);                               // e.g. 005E1347
    } else {
        sw(internal, 0x384, 0);                                         // e.g. 005E135D
    }
    kotor_fastcall_005e08c0(internal, nullptr, k_mouse_0074d3cc, control, value); // 005E16E7..005E16F1
}
