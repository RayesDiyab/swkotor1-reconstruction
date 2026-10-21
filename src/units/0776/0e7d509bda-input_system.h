#pragma once
// CExoInput / CExoInputInternal / CExoRawInputInternal (lane-input, milestone 1).
// Consumed-prefix views: every named field cites an observed instruction; the
// rest is opaque. Sizes: CExoInput 8 (new at 005E3E80 caller ReInitAurora),
// CExoInputInternal 0x3B0 (new 0x3B0 at 005DF409), CExoRawInputInternal 0x34
// (new 0x34 at 005E21BF), CExoInputClass 0x30 (eh_vector stride 005E18AA).
#include "deeac869ec-hover_path.h"
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace kotor_list { struct Internal; }
namespace kotor_reconstruction { struct ArrayMutations; }

namespace kotor_input_hover {

struct ListWrapper {                                       // CExoLinkedList<T>: one word = internal*
    kotor_list::Internal* internal;
    ListWrapper* __thiscall Construct005e0350();           // 005E0350 (CExoLinkedList<CExoInputEventDesc*>)
    ListWrapper* __thiscall Construct004b25d0();           // 004B25D0 (same body, other instantiation)
};
struct ListNode { ListNode* prev; ListNode* next; void* data; };  // node layout read at 005E274D (+8 data)

struct WordArray { U32* data; int count; int capacity; };          // CExoArrayList<unsigned long>/<T*>

struct InputClass {                                        // CExoInputClass 0x30
    alignas(4) unsigned char name[8];                      // +0x00 CExoString (005E0A9E default ctor)
    ListWrapper list08;                                    // +0x08 (005E0AAC)
    ListWrapper device_lists[6];                           // +0x0C..+0x20 (005E0AC8 eh_vector 4 x 6)
    WordArray words;                                       // +0x24 (005E0AD1..D7 zero)
    InputClass* __thiscall Construct();                    // 005E0A80
};

// Raw buffered entry created by BufferEvent(dev,ctl,value) and GetEvents (0x10 node + 0x20 desc).
// [M53 lane-m53-input] __single_inheritance (hover_path.h precedent): without it MSVC gives member pointers to an
// incomplete class the 16-byte unknown-inheritance form, so slot1 sat at +0x10 and [vt+4] read 0 (M52 run: call 0
// from AddEvent 005E0FF6-equivalent). Original tables are 4-byte slots (.rdata 0074D5FC = 0063E7F0, 0063E7F0).
struct __single_inheritance RawEventDesc;
struct RawEventDescVtable {                                // .rdata 0074D5FC
    int (RawEventDesc::*slot0)();                          // 0063E7F0 return_zero (called 005E2C05 on real descs)
    int (RawEventDesc::*slot1)();                          // 0063E7F0 return_zero (called 005E2771 on real descs)
};
struct RawEventDesc {                                      // 0x20 allocation (005E08D4)
    const RawEventDescVtable* vtable;                      // +0x00 = 0074D5FC (005E08EA)
    U32 word04, word08, word0c;                            // zeroed (005E08F0..F6)
    U32 word10, word14;                                    // zeroed after (005E0914 / 005E091A)
    U32 device;                                            // +0x18 (005E0904)
    U32 control;                                           // +0x1C (005E090E)
    int __thiscall ReturnZero0063e7f0();                   // typed slot body for 0063E7F0 (D24)
};
struct RawEvent { int value; U32 word04; U32 word08; RawEventDesc* desc; };   // 0x10 (005E08C3)

// Real event descriptions (array +0x128); fields consumed by GetEvents.
struct __single_inheritance EventDesc;                     // [M53] 4-byte slots: [vt+0] 005E2C05, [vt+4] 005E2771
struct EventDescVtable {
    int (EventDesc::*has_repeat)();                        // [vt+0] 005E2C05
    int (EventDesc::*has_alt_key)();                       // [vt+4] 005E2771
};
struct EventDesc {
    const EventDescVtable* vtable;                         // +0x00
    int state;                                             // +0x04 (005E28CC, 005E2996, 005E29F3)
    U32 word08;                                            // +0x08
    int disabled;                                          // +0x0C (005E288E)
    int id;                                                // +0x10 (006229DB)
    int type;                                              // +0x14 (005E2899, table 005E2F4C)
    U32 word18;                                            // +0x18
    U32 key;                                               // +0x1C (key-table index; low byte at 005E2BCD)
    U32 word20;                                            // +0x20
    int accum;                                             // +0x24 (005E29EB, 005E2A09, 005E2C0B)
    U32 word28, word2c;
    U32 alt_key;                                           // +0x30 (005E2778)
    int alt_accum;                                         // +0x34 (005E2A11)
};
struct TimedEntry { unsigned char opaque00[0xC]; U32 remaining; };          // list +0x134 items (005E2CE2)
struct RepeatEntry { U32 desc_index; U32 initial_delay; U32 repeat_delay; U32 elapsed; int last; int current; int repeating; };  // list +0x138 (005E2D9C..2EE5)

// DIDEVICEOBJECTDATA buffers (0x14 entries) per device; local 12-byte holders.
struct DeviceBuffer {                                      // ctor 005C5A30 / dtor 005E2FF0
    DIDEVICEOBJECTDATA* data;
    U32 count;
    U32 word08;
    DeviceBuffer* __thiscall Construct005c5a30();
    void __thiscall Destroy005e2ff0();
};

struct RawInput {                                          // CExoRawInputInternal 0x34
    int initialized;                                       // +0x00 (005E3F70)
    int active;                                            // +0x04 (005DFF4A, 005E4066)
    int keyboard_mode;                                     // +0x08 (005E3B19)
    U32 word0c;                                            // +0x0C = 1 (005E3EB2)
    U32 word10, word14;
    int joystick_count;                                    // +0x18 (005E2234, 005E3E03)
    IDirectInput8A* direct_input;                          // +0x1C (DirectInput8Create out, 005E3E90)
    IDirectInputDevice8A* keyboard;                        // +0x20 (005E3EE7)
    IDirectInputDevice8A* mouse;                           // +0x24 (005E3FAE)
    IDirectInputDevice8A** joysticks;                      // +0x28 (005E3E10)
    void* word2c;                                          // +0x2C freed at 005E3E43
    void* joystick_states;                                 // +0x30 (0x74 per joystick, 005E3985)
    RawInput* __thiscall Construct(HINSTANCE instance, HWND window);  // 005E3E80 RET8
    void __thiscall ShutDownDirectInput005e3dc0();         // 005E3DC0
    void __thiscall ShutDownDirectInput005e3f90();         // 005E3F90 (JMP 005E3DC0)
    int __thiscall InitializeDirectInputMouse();           // 005E3FA0
    int __thiscall GetMouseState(DIMOUSESTATE* state);     // 005E4050 RET4
    int __thiscall GetKeyboardBuffer(DeviceBuffer* out);   // 005E3AE0 RET4
    int __thiscall GetJoystickBuffer(U32 joystick, DeviceBuffer* out);  // 005E30E0 RET8 (external)
    int __thiscall IsDigital(U32 device, U32 key);         // 005E39F0 RET8
    int __thiscall GetLastState(U32 device, U32 key);      // 005E3930 RET8
    int __thiscall DeadZone0063e7c0(U32 key);              // 0063E7C0 (xor eax,eax; ret 4) typed body (D24)
};

struct __single_inheritance InputInternal;                 // [M53] 4-byte slots: .rdata 0074D624 = 005E2340, 004015A0
struct InputInternalVtable {                               // .rdata 0074D624
    void* (InputInternal::*scalar_destroy)(unsigned flags); // 005E2340
    void (InputInternal::*configure)();                    // 004015A0 void_return
};
struct InputInternal {
    const InputInternalVtable* vtable;                     // +0x000 (005E18B4)
    U32 word004;                                           // +0x004 (005E1937)
    InputClass classes[6];                                 // +0x008..+0x128
    WordArray descs;                                       // +0x128 (EventDesc* per index; 005E2752)
    ListWrapper timed;                                     // +0x134
    ListWrapper repeats;                                   // +0x138
    U32 time_ms;                                           // +0x13C (005E22E6, 005E2CB5)
    RawInput* raw;                                         // +0x140
    InputEventList events;                                 // +0x144 (005E252D)
    int event_count;                                       // +0x150 (005E2914)
    U32 word154;                                           // +0x154
    int device_count;                                      // +0x158 (005E223A)
    alignas(4) unsigned char critical[4];                  // +0x15C CExoCriticalSection (005E9290)
    U32 word160;                                           // +0x160 (005E22EF)
    U32 keys[0x84];                                        // +0x164..+0x374 key table (005E195E..)
    ListWrapper buffered;                                  // +0x374 raw buffered events
    unsigned char mouse_mode;                              // +0x378 (005E22F5; input-snapshot)
    unsigned char pad379[3];
    int pending_x;                                         // +0x37C
    int pending_y;                                         // +0x380
    int skip_move;                                         // +0x384
    U32 word388, word38c, word390, word394;                // +0x388..0x394 (005E1940..52)
    int cursor;                                            // +0x398 (005E230D, 005E0930)
    U32 cursor_arg;                                        // +0x39C
    float delta_x;                                         // +0x3A0 (005E019C)
    float delta_y;                                         // +0x3A4
    U32 capture_keyboard_keys;                             // +0x3A8 (005E25E9)
    U32 last_mouse_down;                                   // +0x3AC (005E000E)

    InputInternal* __thiscall Construct(HINSTANCE instance, HWND window);  // 005E1880 RET8
    void* __thiscall ScalarDestroy(unsigned flags);        // 005E2340 RET4
    void __thiscall Destroy005e0b80();                     // 005E0B80 (external)
    void __thiscall Configure004015a0();                   // 004015A0 typed void_return (D24)
    int __thiscall GetActive();                            // 005DFF40
    void __thiscall BufferRaw(U32 device, U32 control, int value);  // 005E08C0 RET0C
    int __thiscall BufferEvent(const InputMessage* message);       // 005E12A0 RET4
    void __thiscall UpdateMouseDelta(float unused);        // 005E0110 RET4
    int __thiscall SetMouseCursor(int cursor, U32 unused, U32 down);  // 005E0000 RET0C
    int __thiscall RefreshMouseCursor(int down);           // 005E0930 RET4
    void __thiscall WriteEventToFile(U32 time, int index, void* file);  // 005E0810 RET0C
    int __thiscall IsEventInClass(U32 desc_index, int input_class);    // 005E0D80 RET8
    InputEventList* __thiscall GetEvents(InputEventList* result, int* count, int input_class);  // 005E24E0 RET0C
};

struct InputVtable {                                       // .rdata 0074D5EC
    void* (Input::*scalar_destroy)(unsigned flags);        // 005DF670
    void (Input::*configure)();                            // 005DF500
};

// Event list helpers (original template instantiations).
void __fastcall EventListAllocate005e0410(InputEventList* list, void*, int capacity);       // 005E0410 RET4
InputEventList* __fastcall EventListCopy005e17e0(InputEventList* self, void*, const InputEventList* from);  // 005E17E0 RET4
void* __fastcall ListRemoveTail005e9ce0(kotor_list::Internal* list, void*);                // 005E9CE0 RET0
void __fastcall WordArraySetSize00405150(WordArray* self, void*, int size);                 // 00405150 RET4

static_assert(sizeof(RawEventDescVtable) == 8 && sizeof(EventDescVtable) == 8 && sizeof(InputInternalVtable) == 8 &&
              sizeof(InputVtable) == 8, "[M53] original 4-byte vtable slots (0074D5FC, 0074D624, 0074D5EC; [vt+4] 005E2771)");
static_assert(sizeof(InputClass) == 0x30 && offsetof(InputClass, words) == 0x24, "input class");
static_assert(sizeof(RawEventDesc) == 0x20 && sizeof(RawEvent) == 0x10, "raw desc/node");
static_assert(offsetof(EventDesc, alt_accum) == 0x34 && offsetof(EventDesc, key) == 0x1C, "event desc");
static_assert(sizeof(RepeatEntry) == 0x1C && offsetof(TimedEntry, remaining) == 0xC, "list items");
static_assert(sizeof(DeviceBuffer) == 12 && sizeof(DIDEVICEOBJECTDATA) == 0x14 && sizeof(DIMOUSESTATE) == 0x10, "buffers");
static_assert(sizeof(RawInput) == 0x34 && offsetof(RawInput, joystick_states) == 0x30, "raw");
static_assert(offsetof(InputInternal, descs) == 0x128 && offsetof(InputInternal, raw) == 0x140 &&
              offsetof(InputInternal, events) == 0x144 && offsetof(InputInternal, device_count) == 0x158 &&
              offsetof(InputInternal, keys) == 0x164 && offsetof(InputInternal, buffered) == 0x374 &&
              offsetof(InputInternal, mouse_mode) == 0x378 && offsetof(InputInternal, cursor) == 0x398 &&
              offsetof(InputInternal, delta_x) == 0x3A0 && sizeof(InputInternal) == 0x3B0, "internal 0x3B0");
}

// Typed vtable data (D21: typed C++ constant arrays of member pointers).
extern const kotor_input_hover::InputVtable input_vtable_0074d5ec;
extern const kotor_input_hover::InputInternalVtable input_internal_vtable_0074d624;
extern const kotor_input_hover::RawEventDescVtable raw_event_desc_vtable_0074d5fc;

namespace kotor_input_cells {
extern std::uint32_t volatile raw_input_ready_008328a0;   // 008328A0 (sole writer 005E3F7A)
extern float volatile mouse_divisor_007a2278;             // 007A2278 PE .data 100.0f
}
