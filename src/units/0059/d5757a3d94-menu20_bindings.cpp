// Sole definition of the 60-byte const kotor_menu20::bindings descriptor plus
// the five original bodies/cells that had no M40 provider:
//   atoi 006FAFE7 (-> atol 006FAF5F), set_mouse 005E0960, open_logs 0060E760,
//   player_construct 005D5440, cell 0083288C,
// and two placement/nominal entries (005B3190 constructor, 005E5140 assign)
// that reuse the canonical StartupString lifetime and GuiAssign005e5140 body.
// Requires the projected menu_targets.h (five thiscall slots expressed as the
// ABI-identical ECX-receiver / ignored-EDX __fastcall shape, as M40 already
// does for ini_destroy_005eb500). No function-pointer casts, no stubs.
#include "77e8b6221f-menu_targets.h"
#include "3b3c807af5-startup_string.h"
#include "d735aa7dd1-string_priority.h"
#include "c46300269d-allocator_live.h"
#include "40141ae87f-ini_cleanup.h"
#include "8823e9baba-modern_warning_primitives.h"
#include "61ae7fa68b-canonical_formatter_owner.h"
#include "7fa9bc840c-warning_runtime.h"
#include "3a74ce019c-window_shared_cells.h"
#include <windows.h>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>
#include <type_traits>

namespace kotor_menu20 {
using kotor_reconstruction::StartupString;

// ---------------------------------------------------------------- 0083288C
// Original .data cell, loader-zero (PE bytes at 0083288C are zero). Head of
// the CNetLayer peer chain written/read only by NetLayer::initialize_005d5550
// (005D5550/005D555F/005D556D) in the selected cohort.
NetLayer* volatile network_head_0083288c = nullptr;

// ---------------------------------------------------------------- 006FAFE7
// 006FAFE7 is `jmp 006FAF5F` (atoi tail-jumps to atol). 006FAF5F body:
//  006FAF61..006FAF76  fetch legacy per-thread locinfo (PTD+64h / 007A2824)
//  006FAF7C..006FAFA2  skip while space: _isctype_l(c,_SPACE) if mb_cur_max>1
//                      else pctype[c]&8. The legacy PTD/locinfo is excluded by
//                      RUNTIME-BOUNDARY.md; classification is delegated to the
//                      supported /MT CRT isspace (same _SPACE class, modern
//                      CRT locale).
//  006FAFA4..006FAFB7  c=*p++; sign=c; '-' or '+' consumes one more byte
//  006FAFB8..006FAFDB  ASCII '0'..'9' only; total=total*10+digit (lea*5,*2),
//                      32-bit wraparound, no overflow clamp, no errno write
//  006FAFDD..006FAFE6  negate iff sign=='-'
// UCRT atol is NOT used: it saturates to LONG_MAX/LONG_MIN and sets ERANGE on
// overflow, which the original never does.
std::int32_t __cdecl atoi_006fafe7(const char* text) {
    const unsigned char* cursor = reinterpret_cast<const unsigned char*>(text);
    while (std::isspace(*cursor)) ++cursor;
    unsigned c = *cursor++;
    const unsigned sign = c;
    if (c == '-' || c == '+') c = *cursor++;
    std::uint32_t total = 0;
    for (;;) {
        const std::uint32_t digit = (c >= '0' && c <= '9') ? c - '0' : 0xFFFFFFFFu;
        if (digit == 0xFFFFFFFFu) break;
        total = total * 10u + digit;
        c = *cursor++;
    }
    if (sign == '-') total = 0u - total;
    return static_cast<std::int32_t>(total); // MSVC x86: modular two's complement
}

// ---------------------------------------------------------------- 005E0960
// Borrowed views of the original input-internal object and its device rows.
// Only offsets read/written by 005E0960 are named; everything else unknown.
// The producer of these objects (CInput ctor 005DF3F0 -> 005E1880) is outside
// this lane; this body only follows the stored pointers.
struct InputDevice;
struct InputDeviceVtable {
    // 005E09DC/005E09F8: `mov edx,[esi]; mov ecx,esi; call [edx]`, EAX tested.
    // Slot meaning unknown (inferred: "is active/acquired" predicate).
    int(__thiscall* slot00)(InputDevice*);
};
struct InputDevice {
    const InputDeviceVtable* vtable;  // +00
    volatile std::int32_t value04;    // +04 written with x or y
    unsigned char unknown08[0x14];
    volatile std::uint32_t kind1c;    // +1C compared with 0074D3EC/0074D3F0
};
struct InputDeviceRow {
    unsigned char unknown00[0x0c];
    InputDevice* volatile device;     // +0C
};
struct InputInternal005e0960View {
    unsigned char unknown000[0x144];
    InputDeviceRow* volatile rows;    // +144 (row stride 10h, 005E0A10)
    volatile std::int32_t row_count;  // +148 signed (jle/jl)
    unsigned char unknown14c[0x384 - 0x14c];
    volatile std::uint32_t cursor_set384; // +384 := 1 (005E09A9)
};
static_assert(sizeof(InputDeviceRow) == 0x10 && offsetof(InputDeviceRow, device) == 0x0c, "row stride");
static_assert(offsetof(InputDevice, value04) == 4 && offsetof(InputDevice, kind1c) == 0x1c, "device fields");
static_assert(offsetof(InputInternal005e0960View, rows) == 0x144 &&
              offsetof(InputInternal005e0960View, row_count) == 0x148 &&
              offsetof(InputInternal005e0960View, cursor_set384) == 0x384, "input fields");
// .rdata constants: dword 3 at 0074D3EC and dword 4 at 0074D3F0 (part of an
// ascending 0..7 dword run at 0074D3E0). Meaning inferred (X/Y axis kinds).
const std::uint32_t input_kind_0074d3ec = 3;
const std::uint32_t input_kind_0074d3f0 = 4;

void __fastcall SetMouse005e0960(void* receiver, void*, int x, int y) {
    auto* self = std::launder(static_cast<InputInternal005e0960View*>(receiver));
    // 005E096B..005E0989: pt.x=x; pt.y=height_0078D1D8-y-1; ClientToScreen(
    // render HWND 007A39D8,&pt) (IAT 0073D3C4); result ignored.
    POINT point;
    point.x = x;
    point.y = static_cast<LONG>(kotor_menu_window_cells::height_0078d1d8 -
                                static_cast<std::uint32_t>(y) - 1u);
    ::ClientToScreen(kotor_menu_window_cells::render_007a39d8, &point);
    // 005E098F..005E0999: SetCursorPos(pt.x,pt.y) (IAT 0073D3C8); result ignored.
    (void)::SetCursorPos(point.x, point.y);
    // 005E099F count read, 005E09A9 flag write, then signed loop test.
    std::int32_t count = self->row_count;
    self->cursor_set384 = 1;
    for (std::int32_t index = 0; index < count; count = self->row_count) {
        // 005E09C0..005E09C8: row address = [+144]+index*10h tested for zero.
        const std::uintptr_t row_address =
            reinterpret_cast<std::uintptr_t>(self->rows) + static_cast<std::uintptr_t>(index) * 0x10u;
        if (row_address) {
            InputDevice* device = reinterpret_cast<InputDeviceRow*>(row_address)->device;
            if (device) {
                if (device->kind1c == input_kind_0074d3ec && device->vtable->slot00(device))
                    device->value04 = x;   // 005E09E6..005E09EA
                if (device->kind1c == input_kind_0074d3f0 && device->vtable->slot00(device))
                    device->value04 = y;   // 005E0A02..005E0A06
            }
        }
        ++index;                            // 005E0A0F
    }
}

// ---------------------------------------------------------------- 0060E760
// The original body is exactly `ret 4` (0060E760: C2 04 00): a retail no-op
// shared by 12 call/jmp sites (incl. 005E93F7 jmp from Debug 005E93F0).
// This is the genuine behavior, not a placeholder.
void __fastcall OpenLogs0060e760(void*, void*, Word) {}

// ---------------------------------------------------------------- 005B3190
// 005B3190: mov eax,ecx; [eax]=0; [eax+4]=0; ret. Begins the single canonical
// StartupString lifetime in the receiver storage (same placement-entry pattern
// as debug_constructor.h ConstructDebug005e9370). kotor_menu20::String has no
// constructor, so there is no second construction.
String* __fastcall ConstructString005b3190(String* storage, void*) {
    ::new (static_cast<void*>(storage)) StartupString();
    return storage;
}

// ---------------------------------------------------------------- 005E5140
// Nominal entry onto the one canonical assign algorithm GuiAssign005e5140
// (modern-ini-support/string_priority.cpp). The receiver object was begun as
// StartupString by 005B3190/005D5440 above; launder recovers it. EAX = this.
String* __fastcall AssignText005e5140(String* receiver, void*, const char* text) {
    StartupString* live = std::launder(static_cast<StartupString*>(static_cast<void*>(receiver)));
    GuiAssign005e5140(live, nullptr, text);
    return receiver;
}

// ---------------------------------------------------------------- 005D5440
// Observed 0x28-byte prefix of the sub-object at NetLayer+20014h.
// 005D545B..005D5464 construct String at +4 (005B3190)
// 005D5469..005D5476 EH state 0; [+0]=0
// 005D546B..005D5478 String(+4) = 0073D71C via 005E5140
// 005D5481..005D5487 [+1C]=[+20]=[+24]=0; EAX=this
// +0C..+1B untouched. Unwind state 0 (00723F80) destroys String+4 via 005E5C20.
struct Player005d5440View {
    volatile std::uint32_t field00;
    unsigned char string04[8];
    unsigned char unknown0c[0x10];
    volatile std::uint32_t field1c, field20, field24;
};
static_assert(offsetof(Player005d5440View, string04) == 4 &&
              offsetof(Player005d5440View, field1c) == 0x1c &&
              sizeof(Player005d5440View) == 0x28, "observed prefix");

void* __fastcall ConstructPlayer005d5440(void* receiver, void*) {
    auto* self = std::launder(static_cast<Player005d5440View*>(receiver));
    StartupString* text = ::new (static_cast<void*>(self->string04)) StartupString();
    self->field00 = 0;
    // Original EH state 0 would destroy String+4 if 005E5140 threw. The
    // canonical GuiAssign005e5140 is extern "C" (nothrow under the fixed /EHsc
    // contract) and live_operator_new reports failure by null, so no C++
    // exception can reach this frame; no unwind action is representable.
    GuiAssign005e5140(text, nullptr, base_ini_reconstruction::warning_empty_0073d71c);
    self->field1c = 0;
    self->field20 = 0;
    self->field24 = 0;
    return receiver;
}

// ------------------------------------------------------------ descriptor
static_assert(sizeof(Bindings) == 60, "Exact fifteen x86 fields");
static_assert(std::is_same<decltype(Bindings::atol_006fafe7), decltype(&atoi_006fafe7)>::value, "cdecl int32");
static_assert(std::is_same<decltype(Bindings::allocate_006fa7e6), decltype(&live_operator_new)>::value, "006FA7E6");
static_assert(std::is_same<decltype(Bindings::free_006fa390), decltype(&live_free)>::value, "006FA390");
static_assert(std::is_same<decltype(Bindings::set_mouse_005e0960), decltype(&SetMouse005e0960)>::value, "ECX/EDX/2 words");
static_assert(std::is_same<decltype(Bindings::open_logs_0060e760), decltype(&OpenLogs0060e760)>::value, "ECX/EDX/1 word");
static_assert(std::is_same<decltype(Bindings::ini_destroy_005eb500),
                           decltype(&base_ini_reconstruction::destroy_internal_005eb500)>::value, "005EB500");
static_assert(std::is_same<decltype(Bindings::player_construct_005d5440), decltype(&ConstructPlayer005d5440)>::value, "005D5440");
static_assert(std::is_same<decltype(Bindings::string_construct_005b3190), decltype(&ConstructString005b3190)>::value, "005B3190");
static_assert(std::is_same<decltype(Bindings::string_assign_005e5140), decltype(&AssignText005e5140)>::value, "005E5140");
static_assert(std::is_same<decltype(Bindings::vsnprintf_006fd5a8),
                           decltype(&kotor_functional::modern_warning_vsnprintf)>::value, "legacy _vsnprintf boundary");
static_assert(std::is_same<std::size_t, Word>::value, "x86 size_t carrier");

extern const Bindings bindings = {
    &atoi_006fafe7,
    &live_operator_new,
    &live_free,
    &SetMouse005e0960,
    &OpenLogs0060e760,
    &base_ini_reconstruction::destroy_internal_005eb500,
    &ConstructPlayer005d5440,
    &ConstructString005b3190,
    &AssignText005e5140,
    &kotor_functional::modern_warning_vsnprintf,
    &std::strncpy,                     // 006FAB80: supported /MT CRT strncpy (same as canonical formatter)
    &network_head_0083288c,
    &kotor_canonical_string_format::format_buffer_008328a8,
    &kotor_canonical_string_format::format_capacity_008328ac,
    base_ini_reconstruction::warning_empty_0073d71c
};
}
