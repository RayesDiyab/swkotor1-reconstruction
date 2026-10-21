// lane-m48-traps / f-keys: CExoInput construction / destruction chain (startup: ReInitAurora 00403BB4 -> 005DF3F0;
// teardown: ReInitAurora 00403B99 and CClientExoAppInternal::StopServices 0060433A call the deleting destructor slot).
// Donor: claude-takeover/lane-input/src/input_system.cpp + input_events.cpp (lane-input r3), re-verified against the
// original instructions cited per statement; adapted to the staged header input-hover/input_system.h and to the
// composed-link owners. Additions vs the donor: destructor chain 005E0B80 / 005E0AF0, list ctor 005E0350 kept,
// CExoArrayList<unsigned long>::operator= 0051B720 authored, vtables 0074D5EC / 0074D624 defined here.
// Generated constants: generated/input_key_ids.h (D13 local-only; regenerate with generated/gen_input_consts.py from
// the pinned swkotor.exe sha256 9c10e045...; never committed).
// EH unwind of the original constructors (eh_vector_constructor_iterator rollback on a throwing element ctor) is not
// reproduced: no element constructor on these paths throws (allocation failure returns null in the original too).
#include <new>
#include <cstring>
#include "0e7d509bda-input_system.h"
#include "0047cfa72a-startup_string.h"
#include "eddb76278d-allocator_live.h"
#include "36201186c4-window_shared_cells.h"
#include "0d2ad8fac8-constructors.h"   // kotor_list::Internal
#include "ee8b0428ab-critical_section.h"                                                      // kotor_startup::CriticalSection
#include "4f361a5bb9-input_key_ids.h"

using namespace kotor_input_hover;
using kotor_reconstruction::StartupString;
namespace ids = kotor_input_ids;

namespace kotor_reconstruction { struct ArrayMutations; }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t); }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);
}
namespace kotor_sound_internal { void __fastcall ListUnwind005d86c0(void*, void*); }   // 005D86C0 CExoLinkedList dtor (one word)
namespace kotor_menu_10_19 {
struct TimerDependency { unsigned __int64 __thiscall high_resolution_005e9260(); };
namespace dependencies {
extern void* base_007a39e0;
unsigned __int64 __stdcall unsigned_divide_006fb030(unsigned __int64, unsigned __int64);
}
}
extern "C" {
StartupString* __fastcall GuiAssign005e5140(StartupString*, void*, const char*);
void* __fastcall unresolved_critical_ctor_005e9290(void*, void*);
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
WordArray* __fastcall kotor_fastcall_0051b720(WordArray* self, void*, const WordArray* from);
void __fastcall kotor_fastcall_005e0af0(InputClass* self, void*);
void* __fastcall kotor_fastcall_005df3f0(Input* self, void*);
}

// ---- typed vtables (D21: constant arrays of member pointers; single-inheritance PMF = one code word) ----
const InputVtable input_vtable_0074d5ec = {&Input::ScalarDestroy, &Input::Configure};                       // .rdata 0074D5EC
const InputInternalVtable input_internal_vtable_0074d624 = {&InputInternal::ScalarDestroy,
                                                            &InputInternal::Configure004015a0};               // .rdata 0074D624

namespace kotor_input_cells {
std::uint32_t volatile raw_input_ready_008328a0 = 0;   // PE .bss 0; sole writer 005E3F7A (CExoRawInputInternal ctor)
}

namespace {
struct BaseView { void* word00; kotor_menu_10_19::TimerDependency* timers; void* debug; };    // *(007A39E0): +4 timers
U32 now_ms() {                                                                      // 005E22CC..E1
    auto* base = static_cast<BaseView*>(kotor_menu_10_19::dependencies::base_007a39e0);
    const unsigned __int64 t = base->timers->high_resolution_005e9260();
    return static_cast<U32>(kotor_menu_10_19::dependencies::unsigned_divide_006fb030(t, 1000u));
}
kotor_reconstruction::ArrayMutations* mutations(WordArray* a) { return reinterpret_cast<kotor_reconstruction::ArrayMutations*>(a); }
// Inline CExoLinkedList<T> destructor shape used by 005E0B80 / 005E0AF0: destroy the internal, free it.
void destroy_list(ListWrapper& l) {
    kotor_list::Internal* internal = l.internal;
    if (internal) { base_ini_reconstruction::destroy_list_005e9ab0(internal, nullptr); live_free(internal); }
}
const GUID kSysKeyboard_0074d694 = {0x6F1D2B61, 0xD5A0, 0x11CF, {0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};   // .rdata 0074D694
const GUID kIID_IDirectInput8A_0074d6f4 = {0xBF798030, 0x483A, 0x4DA2, {0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00}};  // .rdata 0074D6F4
}

// ================================ CExoInput ==================================
extern "C" void* __fastcall kotor_fastcall_005df3f0(Input* self, void*) {           // 005DF3F0 CExoInput::CExoInput
    void* raw = live_operator_new(0x3B0);                                          // 005DF409..14
    self->vtable = &input_vtable_0074d5ec;                                         // 005DF40E
    InputInternal* in = nullptr;
    if (raw) in = static_cast<InputInternal*>(raw)->Construct(                    // 005DF42C..3C
        static_cast<HINSTANCE>(kotor_menu_window_cells::instance_007a39c0),        // [007A39C0]
        static_cast<HWND>(kotor_menu_window_cells::render_007a39d8));              // [007A39D8]
    self->internal = in;                                                           // 005DF449
    return self;                                                                   // 005DF44C
}
void* Input::ScalarDestroy(unsigned flags) {                                       // 005DF670 RET4
    InputInternal* in = internal;                                                  // 005DF673
    vtable = &input_vtable_0074d5ec;                                               // 005DF678
    if (in) {
        (in->*(in->vtable->scalar_destroy))(1);                                    // 005DF680..84
        internal = nullptr;                                                        // 005DF686
    }
    if (flags & 1u) live_free(this);                                               // 005DF68D..95
    return this;                                                                   // 005DF69D
}
void Input::Configure() { InputInternal* in = internal; (in->*(in->vtable->configure))(); }  // 005DF500..05 (JMP [vt+4])

// ============================ CExoInputClass =================================
InputClass* InputClass::Construct() {                                              // 005E0A80
    ::new (static_cast<void*>(name)) StartupString();                              // 005E0A9E (005B3190)
    list08.Construct004b25d0();                                                    // 005E0AAC
    for (ListWrapper& l : device_lists) l.Construct004b25d0();                     // 005E0AC8 eh_vector(+0C, 4, 6, 004B25D0)
    words.capacity = 0; words.data = nullptr; words.count = 0;                     // 005E0AD1..D7 (+28, +2C, +24)
    return this;
}
extern "C" void __fastcall kotor_fastcall_005e0af0(InputClass* self, void*) {      // 005E0AF0 ~CExoInputClass
    live_free(self->words.data);                                                   // 005E0B0E..1A
    self->words.data = nullptr;                                                    // 005E0B2F
    for (int i = 5; i >= 0; --i)                                                   // 005E0B3B eh_vector_destructor(+0C, 4, 6, 005D86C0): last..first
        kotor_sound_internal::ListUnwind005d86c0(&self->device_lists[i], nullptr);
    destroy_list(self->list08);                                                    // 005E0B40..59
    reinterpret_cast<StartupString*>(self->name)->~StartupString();               // 005E0B66 (005E5C20)
}

// ============================ CExoLinkedList<T> ==============================
ListWrapper* ListWrapper::Construct005e0350() {                                    // 005E0350
    void* raw = live_operator_new(0xC);                                            // 005E0367..6B
    internal = raw ? static_cast<kotor_list::Internal*>(raw)->Construct() : nullptr;  // 005E0385 (005E9AA0) / 005E039E
    return this;
}

// ======================= CExoArrayList<unsigned long>::operator= =============
extern "C" WordArray* __fastcall kotor_fastcall_0051b720(WordArray* self, void*, const WordArray* from) {  // 0051B720 RET4
    self->count = 0;                                                               // 0051B72B
    for (int i = 0; i < from->count; ++i) {                                        // 0051B72E..31 / 0051B79F..A5
        const U32 value = from->data[i];                                           // 0051B734..3D
        if (self->count == self->capacity) {                                       // 0051B73A..44
            const int capacity = self->capacity ? self->capacity * 2 : 0x10;       // 0051B746..4E
            U32* old = self->data;                                                 // 0051B753
            self->capacity = capacity;                                             // 0051B75D
            self->data = static_cast<U32*>(live_operator_new(static_cast<std::uint32_t>(capacity) * 4u));  // 0051B755..68
            for (int j = 0; j < self->count; ++j) self->data[j] = old[j];          // 0051B76D..81
            if (old) live_free(old);                                               // 0051B783..88
        }
        self->data[self->count] = value;                                           // 0051B790..99
        ++self->count;                                                             // 0051B79C
    }
    return self;                                                                   // 0051B7A8
}

// ========================== CExoInputInternal ================================
InputInternal* InputInternal::Construct(HINSTANCE instance, HWND window) {         // 005E1880 RET8
    vtable = &input_internal_vtable_0074d624;                                      // 005E18B4
    for (InputClass& c : classes) c.Construct();                                   // 005E18BA eh_vector(+8, 30h, 6, 005E0A80)
    descs.capacity = 0; descs.count = 0; descs.data = nullptr;                     // 005E18CB..D1
    timed.Construct005e0350();                                                     // 005E18DE
    repeats.Construct005e0350();                                                   // 005E18EE
    events.count = 0; events.capacity = 0; events.data = nullptr;                  // 005E18F3..FF
    unresolved_critical_ctor_005e9290(critical, nullptr);                          // 005E1910
    buffered.Construct005e0350();                                                  // 005E1920
    word154 = 0; raw = nullptr; event_count = 0; word004 = 0;                      // 005E1925..37
    capture_keyboard_keys = 0;                                                     // 005E193A
    word388 = 0; word38c = 0; word390 = 0; word394 = 0;                            // 005E1940..52
    for (const ids::KeyStore& s : ids::key_table_stores_005e1880)                  // 005E195E..21C1 (store order kept)
        keys[s.index] = s.value;
    void* mem = live_operator_new(0x34);                                           // 005E21BF..CC
    RawInput* r = mem ? static_cast<RawInput*>(mem)->Construct(instance, window) : nullptr;  // 005E21E1..FC
    raw = r;                                                                       // 005E2205
    if (!r) return this;                                                           // 005E220B -> 005E231F
    if (r->initialized == 0) {                                                     // 005E2211..13
        r->ShutDownDirectInput005e3f90();                                          // 005E2217
        live_free(r);                                                              // 005E2221
        raw = nullptr;                                                             // 005E2229
        return this;                                                               // 005E222F -> 005E231F
    }
    device_count = r->joystick_count + 2;                                          // 005E2234..3A
    if (r->joystick_count > 4) device_count = 6;                                   // 005E2240..45 (ebp = 4 from 005E1A6A; signed JLE)
    menu_list_growth::allocate_005e03c0(mutations(&descs), nullptr, 1);            // 005E224F..53
    descs.count = 1;                                                               // 005E2258
    descs.data[0] = 0;                                                             // 005E225F..61
    for (int i = 0; i < 6; ++i) {                                                  // 005E2272 counter 6
        InputClass& c = classes[i];
        GuiAssign005e5140(reinterpret_cast<StartupString*>(c.name), nullptr, ids::unnamed_class_0074d62c);  // 005E2283..8A
        menu_list_growth::allocate_005e03c0(mutations(&c.words), nullptr, 1);      // 005E228F..93
        c.words.count = 1;                                                         // 005E2298 ([ebp+28h])
        WordArray empty{nullptr, 0, 0};                                            // [esp+18h..20h] zeroed at 005E2263..6B
        kotor_fastcall_0051b720(&c.words, nullptr, &empty);                        // 005E22A3..AB
        live_free(empty.data);                                                     // 005E22B0..B6 (free of the null word)
    }
    time_ms = now_ms();                                                            // 005E22CC..E6
    word160 = 0;                                                                   // 005E22EF
    mouse_mode = 0;                                                                // 005E22F5
    pending_x = -1; pending_y = -1;                                                // 005E22FB..301
    skip_move = 0;                                                                 // 005E2307
    cursor = -1;                                                                   // 005E230D
    cursor_arg = 0;                                                                // 005E2313
    last_mouse_down = 0;                                                           // 005E2319
    return this;                                                                   // 005E2324
}
void InputInternal::Destroy005e0b80() {                                            // 005E0B80 ~CExoInputInternal
    vtable = &input_internal_vtable_0074d624;                                      // 005E0BA1
    RawInput* r = raw;                                                             // 005E0BA7
    if (r) {                                                                       // 005E0BAF..B9
        r->ShutDownDirectInput005e3f90();                                          // 005E0BBD
        live_free(r);                                                              // 005E0BC3
        raw = nullptr;                                                             // 005E0BCB
    }
    const int count = descs.count;                                                 // 005E0BD2
    for (int i = 0; i < count; ++i)                                                // 005E0BDA..F9
        if (descs.data[i]) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(descs.data[i])));  // 005E0BE0..EE
    if (descs.data) { live_free(descs.data); descs.data = nullptr; descs.capacity = 0; }  // 005E0BFB..15
    descs.count = 0;                                                               // 005E0C1B
    void* position = *reinterpret_cast<void**>(repeats.internal);                  // 005E0C21..2B (head word)
    while (position) {                                                             // 005E0C2F / 005E0C5A
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(
            unresolved_list_at_pos_005e9b60(repeats.internal, nullptr, position))));  // 005E0C31..3E
        unresolved_list_next_005e9b90(repeats.internal, nullptr, &position);       // 005E0C43..51
    }
    while (*reinterpret_cast<void**>(buffered.internal)) {                         // 005E0C5E..66 / 005E0C9D..A5
        auto* node = static_cast<RawEvent*>(base_ini_reconstruction::remove_head_005e9c80(buffered.internal, nullptr));  // 005E0C70..76
        if (node) {                                                                // 005E0C7D
            if (node->desc) { live_free(node->desc); node->desc = nullptr; }      // 005E0C81..91
            live_free(node);                                                       // 005E0C94..95
        }
    }
    destroy_list(buffered);                                                        // 005E0CA7..C3
    reinterpret_cast<kotor_startup::CriticalSection*>(critical)->Destroy();        // 005E0CC6..D1 (005E9320)
    live_free(events.data);                                                        // 005E0CD6..E2
    events.data = nullptr;                                                         // 005E0CE7
    destroy_list(repeats);                                                         // 005E0CED..0C
    destroy_list(timed);                                                           // 005E0D0F..2B
    live_free(descs.data);                                                         // 005E0D2E..39
    descs.data = nullptr;                                                          // 005E0D48
    for (int i = 5; i >= 0; --i) kotor_fastcall_005e0af0(&classes[i], nullptr);   // 005E0D41..5C eh_vector_destructor(+8,30h,6,005E0AF0)
}
void* InputInternal::ScalarDestroy(unsigned flags) {                               // 005E2340 RET4
    Destroy005e0b80();                                                             // 005E2343
    if (flags & 1u) live_free(this);                                               // 005E2348..50
    return this;                                                                   // 005E2358
}
void InputInternal::Configure004015a0() {}                                         // 004015A0 `ret` (D24 typed slot body, no address credit)

// ========================= CExoRawInputInternal ==============================
RawInput* RawInput::Construct(HINSTANCE instance, HWND window) {                  // 005E3E80 RET8
    initialized = 0; direct_input = nullptr; keyboard = nullptr; mouse = nullptr;  // 005E3EA2..A9
    active = 0; keyboard_mode = 0; word0c = 1; word10 = 0; word14 = 0;             // 005E3EAC..BC
    void* out = nullptr;
    HRESULT hr = DirectInput8Create(instance, 0x800, kIID_IDirectInput8A_0074d6f4, &out, nullptr);  // 005E3E8F..BF (005E50C0)
    direct_input = static_cast<IDirectInput8A*>(out);                              // out pointer is [esi+1Ch]
    if (hr != 0) {                                                                 // 005E3EC4..C6
        hr = DirectInput8Create(instance, 0x300, kIID_IDirectInput8A_0074d6f4, &out, nullptr);  // 005E3EC8..D9
        direct_input = static_cast<IDirectInput8A*>(out);
        if (hr != 0) { ShutDownDirectInput005e3dc0(); return this; }               // 005E3EDE..E0 -> 005E3F50
    }
    if (direct_input->CreateDevice(kSysKeyboard_0074d694, &keyboard, nullptr) != 0) {  // 005E3EE2..F3 [vt+0Ch]
        ShutDownDirectInput005e3dc0(); return this;
    }
    if (keyboard->SetDataFormat(&c_dfDIKeyboard) != 0) {                           // 005E3EF5..05 [vt+2Ch], 0074D8FC
        ShutDownDirectInput005e3dc0(); return this;
    }
    if (keyboard->SetCooperativeLevel(window, 6) != 0) {                           // 005E3F07..19 [vt+34h] (NONEXCLUSIVE|FOREGROUND)
        ShutDownDirectInput005e3dc0(); return this;
    }
    DIPROPDWORD property;                                                          // 005E3F1E..3D
    property.diph.dwSize = 0x14; property.diph.dwHeaderSize = 0x10; property.diph.dwObj = 0;
    property.diph.dwHow = 0; property.dwData = 0x100;
    if (keyboard->SetProperty(DIPROP_BUFFERSIZE, &property.diph) != 0) {           // 005E3F45..4E [vt+18h]
        ShutDownDirectInput005e3dc0(); return this;                                // 005E3F50..60
    }
    joysticks = nullptr; word2c = nullptr; joystick_states = nullptr;              // 005E3F64..6A
    joystick_count = 0;                                                            // 005E3F6D
    initialized = 1;                                                               // 005E3F70
    kotor_input_cells::raw_input_ready_008328a0 = 0;                               // 005E3F7A
    return this;
}
void RawInput::ShutDownDirectInput005e3f90() { ShutDownDirectInput005e3dc0(); }   // 005E3F90 JMP 005E3DC0 (body owned by f-input)
