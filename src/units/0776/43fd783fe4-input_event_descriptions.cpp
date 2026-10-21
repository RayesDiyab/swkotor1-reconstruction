// lane-m47-core: CExoInput event-description entries used by CClientExoAppInternal::SetEventDescriptions
// 005EEB10 (r4-lowers adopted/event_descriptions.cpp; first frame per D29) and their CExoInputInternal bodies:
//   CExoInput wrappers 005DF460 / 005DF470 / 005DF480 / 005DF490 / 005DF600 (mov ecx,[ecx+4]; jmp internal),
//   005DF6F0 EventExists; CExoInputInternal 005E0E20 CreateNewEvent, 005E0FA0 AddEvent, 005E0480 IsControlUsed,
//   005E11D0 ScaleEvent, 005E10B0 RemoveEvent, 005E0520 SetEventRepeatable, 005E2F70 ClearAllInputEventDescriptions;
//   CExoInputEventDesc* ctors 005DFCC0 / 005DFCF0 / 005DFD50 and their two-slot tables 0074D604 / 0074D60C
//   (slots 0063E7A0 return_true / 0063E7F0 return_zero; base table 0074D5FC is lane-input's raw_event_desc_vtable).
// Types: lane-input input_system.h (CExoInputInternal 0x3B0 view; IsEventInClass 005E0D80 is lane-input's body).
// Integration dependency: lane-input input_system.cpp (IsEventInClass, WordArraySetSize) must be in the cohort.
// Evidence: lane dis/<addr>.asm; jump tables 005E0F7C = {0EF2,0EF2,0EF2,0EBB,0F2B,0EF2},
// 005E1284 = {1223,1279,1223,1279,1279,1223}; .rdata 0074D3C4 = FFFFFFFFh (any device); tables read via tools/rd.py.
// Event-desc slot calls go through D1 views of the two-slot tables (ABI-identical to the PMF entries).
// Original allocation-failure paths store through a null object (005E0EE4..ED, 005E0F21..26, 005E0F42..56,
// 005E123D..50): reproduced as the same faulting stores to the absolute addresses (null + field), see fault_store.
#include "0e7d509bda-input_system.h"
#include "eddb76278d-allocator_live.h"
#include "0d2ad8fac8-constructors.h"  // kotor_list::Internal (as lane-input)
#include <cstdint>
#include <cstring>

using namespace kotor_input_hover;

namespace base_ini_reconstruction { struct Node; }
namespace ini_addhead { base_ini_reconstruction::Node* __fastcall AddHead005e9d70(kotor_list::Internal*, void*, void*); }
namespace add_key_lower { void __fastcall Remove005e9c10(kotor_list::Internal*, void*, void*); }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t); }
namespace base_ini_reconstruction { __declspec(noinline) void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*); }
extern "C" {
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
}

namespace {
using Word = std::uint32_t;
template <class T> T load(const void* p, unsigned off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
template <class T> void store(void* p, unsigned off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v)); }
// Faulting store of the original's null-object paths: `xor eax,eax; mov [eax+disp],reg`.
void fault_store(unsigned displacement, Word value) {
    *reinterpret_cast<volatile Word*>(static_cast<std::uintptr_t>(displacement)) = value;
}
void put(void* object, unsigned off, Word v) { if (object) store(object, off, v); else fault_store(off, v); }
// Two-slot event-description table view (D1 form of the thiscall slots).
using DescSlot = int (__fastcall*)(void*, void*);
int desc_slot(void* desc, unsigned offset) {
    DescSlot fn; std::memcpy(&fn, static_cast<const unsigned char*>(load<const void*>(desc, 0)) + offset, sizeof(fn));
    return fn(desc, nullptr);
}
void* list_at(kotor_list::Internal* l, void* pos) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_at_pos_005e9b60(l, nullptr, pos))); }
void* list_next(kotor_list::Internal* l, void** pos) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_next_005e9b90(l, nullptr, pos))); }
// Per-class device list: this + 14h + (device + class*12)*4  (005E04AC..BA, 005E1053..59, 005E110E..1A).
kotor_list::Internal* device_list(InputInternal* self, int input_class, Word device) {
    return load<kotor_list::Internal*>(self, 0x14u + (device + static_cast<Word>(input_class) * 12u) * 4u);
}
}

extern "C" {
// ---- event-description tables (.rdata 0074D604 / 0074D60C) and their typed slot leaves (D24)
struct EventDescTable { DescSlot slot0; DescSlot slot1; };
int __fastcall input_desc_return_true_0063e7a0(void*, void*);
int __fastcall input_desc_return_zero_0063e7f0(void*, void*);
extern const EventDescTable input_event_desc_detailed_table_0074d604;
extern const EventDescTable input_event_desc_two_button_table_0074d60c;
const EventDescTable input_event_desc_detailed_table_0074d604{&input_desc_return_true_0063e7a0, &input_desc_return_zero_0063e7f0};
const EventDescTable input_event_desc_two_button_table_0074d60c{&input_desc_return_true_0063e7a0, &input_desc_return_true_0063e7a0};
// 0063E7A0 return_true (mov eax,1; ret) / 0063E7F0 return_zero (xor eax,eax; ret), typed for this slot type.
// [M45 compose] 0063E7A0 (mov eax,1; ret) body removed: earliest-staged owner resource-successor return_one cres_base_constant_word_0063e7a0 (D68 rule) (definition removed; declaration kept)
__declspec(noinline) int __fastcall input_desc_return_true_0063e7a0(void*, void*);
__declspec(noinline) int __fastcall input_desc_return_zero_0063e7f0(void*, void*) { return 0; }

// ---- desc constructors
// 005DFCC0 CExoInputEventDescDetailed::CExoInputEventDescDetailed().
__declspec(noinline) void* __fastcall input_desc_detailed_ctor_005dfcc0(void* self, void*) {
    store(self, 4, Word{0}); store(self, 8, Word{0}); store(self, 0x0c, Word{0});           // 005DFCC9..CF
    store<const void*>(self, 0, &input_event_desc_detailed_table_0074d604);                  // 005DFCD2
    store(self, 0x28, Word{0x3f800000u}); store(self, 0x2c, Word{0x3f800000u});              // 005DFCD8..DB (1.0f)
    store(self, 0x20, Word{0}); store(self, 0x24, Word{0});                                  // 005DFCDE..E1
    return self;
}
// 005DFCF0 CExoInputEventDescDetailed::CExoInputEventDescDetailed(CExoInputEventDesc const*) RET 4.
__declspec(noinline) void* __fastcall input_desc_detailed_copy_005dfcf0(void* self, void*, const void* from) {
    store<const void*>(self, 0, &raw_event_desc_vtable_0074d5fc);                           // 005DFCF6 (base table first)
    store(self, 4, load<Word>(from, 4));                                                     // 005DFCFC..FF
    store(self, 0x10, load<Word>(from, 0x10));                                               // 005DFD02..05
    store(self, 0x14, load<Word>(from, 0x14));                                               // 005DFD08..0B
    store(self, 0x18, load<Word>(from, 0x18));                                               // 005DFD0E..11
    store(self, 0x1c, load<Word>(from, 0x1c));                                               // 005DFD14..17
    store(self, 8, load<Word>(from, 8));                                                     // 005DFD1A..1D
    store(self, 0x0c, load<Word>(from, 0x0c));                                               // 005DFD20..23
    store(self, 0x28, Word{0x3f800000u}); store(self, 0x2c, Word{0x3f800000u});              // 005DFD26..2E
    store<const void*>(self, 0, &input_event_desc_detailed_table_0074d604);                  // 005DFD33
    store(self, 0x20, Word{0}); store(self, 0x24, Word{0});                                  // 005DFD39..3C
    return self;
}
// 005DFD50 CExoInputeventDesc2ButtonAxis::CExoInputeventDesc2ButtonAxis().
__declspec(noinline) void* __fastcall input_desc_two_button_ctor_005dfd50(void* self, void*) {
    store(self, 4, Word{0}); store(self, 8, Word{0}); store(self, 0x0c, Word{0});           // 005DFD59..5F
    store(self, 0x28, Word{0x3f800000u}); store(self, 0x2c, Word{0x3f800000u});              // 005DFD62..65
    store(self, 0x20, Word{0}); store(self, 0x24, Word{0});                                  // 005DFD68..6B
    store<const void*>(self, 0, &input_event_desc_two_button_table_0074d60c);                // 005DFD6E
    store(self, 0x34, Word{0});                                                              // 005DFD74
    return self;
}

// ---- CExoInputInternal bodies
// 005E0480 IsControlUsed(key, device, class) RET 0Ch: the desc using `key` (or alt key) in that list, else 0.
__declspec(noinline) void* __fastcall input_is_control_used_005e0480(InputInternal* self, void*, Word key, int device, int input_class) {
    if (input_class < 0 || input_class >= 6) return nullptr;                                  // 005E0480..91
    if (device < 0 || device >= 6) return nullptr;                                            // 005E0493..9E
    if (key >= 0x84u) return nullptr;                                                         // 005E04A0..AA
    kotor_list::Internal* const list = device_list(self, input_class, static_cast<Word>(device)); // 005E04AC..BA
    void* position = list->head;                                                              // 005E04B6..C2
    while (position) {
        const Word id = load<Word>(list_at(device_list(self, input_class, static_cast<Word>(device)), position), 0); // 005E04C4..CC
        void* const desc = load<void*>(self->descs.data, id * 4u);                            // 005E04CE..D4
        if (load<Word>(desc, 0x1c) == key) return desc;                                       // 005E04D7..DA
        if (desc_slot(desc, 4) && load<Word>(desc, 0x30) == key) return desc;                 // 005E04DC..EA
        (void)list_next(device_list(self, input_class, static_cast<Word>(device)), &position); // 005E04EC..FE
    }
    return nullptr;
}

// 005E0FA0 AddEvent(id, class) RET 8.
__declspec(noinline) Word __fastcall input_add_event_005e0fa0(InputInternal* self, void*, Word id, Word input_class) {
    if (input_class >= 6u) return 0;                                                          // 005E0FA3..AD (JAE)
    void* const desc = load<void*>(self->descs.data, id * 4u);                                // 005E0FB3..BD
    if (!desc) return 0;                                                                      // 005E0FC0..C2
    if (self->IsEventInClass(id, static_cast<int>(input_class))) return 0;                    // 005E0FC8..D1 (lane-input 005E0D80)
    const Word key = load<Word>(desc, 0x1c);                                                  // 005E0FD7
    if (key != 0x84u && input_is_control_used_005e0480(self, nullptr, key, static_cast<int>(load<Word>(desc, 0x18)),
                                                       static_cast<int>(input_class))) return 0; // 005E0FDA..F0
    if (desc_slot(desc, 4)) {                                                                 // 005E0FF6..FF
        const Word alt = load<Word>(desc, 0x30);                                              // 005E1001
        if (alt != 0x84u && input_is_control_used_005e0480(self, nullptr, alt, static_cast<int>(load<Word>(desc, 0x18)),
                                                           static_cast<int>(input_class))) return 0; // 005E1004..1A
    }
    store(desc, 8, load<Word>(desc, 8) + 1u);                                                 // 005E1020..27
    const Word device = load<Word>(desc, 0x18);                                               // 005E1023
    void* const entry = static_cast<unsigned char*>(desc) + 0x10;                             // 005E1030 (&desc->id)
    if (device == 0xffffffffu) {                                                              // 005E102A..36 [0074D3C4]
        (void)ini_addhead::AddHead005e9d70(load<kotor_list::Internal*>(self, 0x10u + input_class * 0x30u), nullptr, entry); // 005E1038..42 (class list +08)
        return 1;
    }
    (void)ini_addhead::AddHead005e9d70(device_list(self, static_cast<int>(input_class), device), nullptr, entry); // 005E1053..5D
    const std::int32_t index = static_cast<std::int32_t>(id);
    unsigned char* const words = load<unsigned char*>(self, 0x2cu + input_class * 0x30u);    // 005E1068..70 (class words.data)
    unsigned char* const cell = words + (index / 32) * 4;                                     // 005E1062..7F (CDQ/AND/ADD/SAR)
    const int bit = index % 32;                                                               // 005E1077..88 (signed remainder)
    store(cell, 0, load<Word>(cell, 0) | (1u << (bit & 31)));                                // 005E1089..97 (shl edx,cl)
    return 1;
}

// 005E10B0 RemoveEvent(id) RET 4: returns the class it was removed from, or -1.
__declspec(noinline) int __fastcall input_remove_event_005e10b0(InputInternal* self, void*, Word id) {
    void* const desc = load<void*>(self->descs.data, id * 4u);                                // 005E10B5..C0
    if (!desc) return -1;                                                                     // 005E10C3..D1
    store<void*>(self->descs.data, id * 4u, nullptr);                                         // 005E10D5
    int input_class = 0;
    for (; input_class < 6; ++input_class)                                                    // 005E10DB..F1
        if (self->IsEventInClass(id, input_class)) break;                                     // 005E10E0..EB
    if (input_class == 6) return -1;                                                          // 005E10F3..FB
    const Word device = load<Word>(desc, 0x18);                                               // 005E110E
    void* position = device_list(self, input_class, device)->head;                            // 005E110E..26
    if (!position) return -1;
    for (;;) {
        (void)list_at(device_list(self, input_class, device), position);                     // 005E1128..32 (EAX unused)
        if (id != 0) break;                                                                   // 005E1137..3D (tests the id argument)
        (void)list_next(device_list(self, input_class, device), &position);                  // 005E113F..4D
        if (!position) return -1;                                                             // 005E1152..62
    }
    if (!position) return -1;                                                                 // 005E1165..6B
    add_key_lower::Remove005e9c10(device_list(self, input_class, device), nullptr, position); // 005E116D..77
    const std::int32_t index = static_cast<std::int32_t>(id);
    unsigned char* const words = load<unsigned char*>(self, 0x2cu + static_cast<Word>(input_class) * 0x30u); // 005E1188..8F
    unsigned char* const cell = words + (index / 32) * 4;                                     // 005E117C..9C
    store(cell, 0, load<Word>(cell, 0) & 0u);                                                 // 005E11A6..AF (xor edx,edx; shl edx,cl: mask 0)
    live_free(desc);                                                                          // 005E11AC..B1 006FA390
    return input_class;                                                                       // 005E11B9
}

// 005E0E20 CreateNewEvent(id, type, class, key, alt) RET 14h.
__declspec(noinline) Word __fastcall input_create_new_event_005e0e20(InputInternal* self, void*, Word id, Word type,
                                                                     Word input_class, Word key, Word alt) {
    if (static_cast<std::int32_t>(input_class) >= 6) return 0;                                // 005E0E21..31 (signed JL)
    const Word count = static_cast<Word>(self->descs.count);                                  // 005E0E35
    if (id >= count) {                                                                        // 005E0E40..43 (unsigned JB)
        const Word grown = id + id + 1u;                                                      // 005E0E46
        if (grown == 0) {                                                                     // 005E0E4A..52
            if (self->descs.data) {                                                           // 005E0E54..58
                live_free(self->descs.data);                                                  // 005E0E5A..5B
                self->descs.data = nullptr;                                                   // 005E0E63 (edi = 0)
                self->descs.capacity = 0;                                                     // 005E0E65
            }
        } else {
            menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&self->descs),
                                                nullptr, grown);                              // 005E0E6A..6D
        }
        self->descs.count = static_cast<int>(grown);                                          // 005E0E76
        for (std::int32_t i = static_cast<std::int32_t>(count); i < self->descs.count; ++i)   // 005E0E79..91 (signed)
            self->descs.data[i] = 0;
    }
    if (self->descs.data[id] != 0) return 0;                                                  // 005E0E94..A1
    void* desc = nullptr;                                                                     // 005E0E9D (EAX = 0)
    switch (type) {                                                                           // 005E0EA7..B4 (JA 005E0F63)
    case 0: case 1: case 2: case 5: {                                                         // 005E0EF2
        desc = live_operator_new(0x20);                                                       // 005E0EF2..F4
        if (desc) {
            store(desc, 4, Word{0}); store(desc, 8, Word{0}); store(desc, 0x0c, Word{0});    // 005E0F02..08
            store<const void*>(desc, 0, &raw_event_desc_vtable_0074d5fc);                    // 005E0F0F
        }
        put(desc, 0x14, type);                                                                // 005E0F15 / 005E0F23
        put(desc, 0x18, input_class);                                                         // 005E0F18 / 005E0F26
        break;
    }
    case 3:                                                                                   // 005E0EBB
        desc = live_operator_new(0x30);
        if (desc) desc = input_desc_detailed_ctor_005dfcc0(desc, nullptr);                    // 005E0EC9..CB
        put(desc, 0x14, 3u);                                                                  // 005E0ED4 / 005E0EE6
        put(desc, 0x18, input_class);                                                         // 005E0EDB / 005E0EED
        break;
    case 4:                                                                                   // 005E0F2B
        desc = live_operator_new(0x38);
        if (desc) desc = input_desc_two_button_ctor_005dfd50(desc, nullptr);                  // 005E0F39..3B
        put(desc, 0x18, input_class);                                                         // 005E0F44..48
        put(desc, 0x14, 4u);                                                                  // 005E0F4F
        put(desc, 0x30, alt);                                                                 // 005E0F4B / 005E0F56
        break;
    default:
        self->descs.data[id] = 0;                                                             // 005E0F63..6D (EAX = 0)
        return 1;
    }
    put(desc, 0x10, id);                                                                      // 005E0F5D
    put(desc, 0x1c, key);                                                                     // 005E0F59 / 005E0F60
    self->descs.data[id] = static_cast<U32>(reinterpret_cast<std::uintptr_t>(desc));          // 005E0F63..6D
    return 1;
}

// 005E11D0 ScaleEvent(id, magnitude, exponent) RET 0Ch (floats moved as raw dwords).
__declspec(noinline) Word __fastcall input_scale_event_005e11d0(InputInternal* self, void*, Word id, float magnitude, float exponent) {
    void* desc = load<void*>(self->descs.data, id * 4u);                                      // 005E11D9..DF
    if (!desc) return 0;                                                                      // 005E11E2..E4
    Word mag_bits, exp_bits;
    std::memcpy(&mag_bits, &magnitude, 4); std::memcpy(&exp_bits, &exponent, 4);
    if (desc_slot(desc, 0)) {                                                                 // 005E11EA..F2 [vtbl+0]
        store(desc, 0x28, mag_bits); store(desc, 0x2c, exp_bits);                             // 005E11F4..00
        store(desc, 0x20, Word{1});                                                           // 005E1203
        return 1;
    }
    const Word type = load<Word>(desc, 0x14);                                                 // 005E1214
    if (type > 5u || type == 1u || type == 3u || type == 4u) return 0;                        // 005E1217..1C (table 005E1284)
    void* const raw = live_operator_new(0x30);                                                // 005E1223..25
    void* const converted = raw ? input_desc_detailed_copy_005dfcf0(raw, nullptr, desc) : nullptr; // 005E122D..3D
    put(converted, 0x28, mag_bits);                                                           // 005E124A
    put(converted, 0x2c, exp_bits);                                                           // 005E124D
    put(converted, 0x20, 1u);                                                                 // 005E1250
    const int input_class = input_remove_event_005e10b0(self, nullptr, id);                   // 005E1247..57
    self->descs.data[id] = static_cast<U32>(reinterpret_cast<std::uintptr_t>(converted));     // 005E125C..63
    (void)input_add_event_005e0fa0(self, nullptr, id, static_cast<Word>(input_class));        // 005E1262..69
    return 1;
}

// 005E0520 SetEventRepeatable(id, initial, repeat) RET 0Ch (type-1 descs only; list +138h).
__declspec(noinline) Word __fastcall input_set_event_repeatable_005e0520(InputInternal* self, void*, Word id, Word initial, Word repeat) {
    void* const desc = load<void*>(self->descs.data, id * 4u);                                // 005E0528..2E
    if (!desc || load<Word>(desc, 0x14) != 1u) return 0;                                      // 005E0534..40
    void* position = self->repeats.internal->head;                                            // 005E0546..54
    while (position) {
        RepeatEntry* const entry = static_cast<RepeatEntry*>(list_at(self->repeats.internal, position)); // 005E0560..67
        if (entry && entry->desc_index == id) {                                               // 005E056C..72
            entry->initial_delay = initial;                                                   // 005E05D7
            entry->repeat_delay = repeat;                                                     // 005E05DA
            return 1;
        }
        (void)list_next(self->repeats.internal, &position);                                   // 005E0574..8A
    }
    RepeatEntry* const entry = static_cast<RepeatEntry*>(live_operator_new(0x1c));            // 005E058C..8E
    if (!entry) return 0;                                                                     // 005E0596..98
    entry->desc_index = id; entry->initial_delay = initial; entry->repeat_delay = repeat;     // 005E05A2..A7
    entry->elapsed = 0; entry->repeating = 0; entry->current = 0; entry->last = 0;            // 005E05AA..B3
    (void)ini_addhead::AddHead005e9d70(self->repeats.internal, nullptr, entry);              // 005E05B6..BD
    return 1;
}

// 005E2F70 ClearAllInputEventDescriptions: empty device lists 0..device_count-1 and zero the class words of each
// of the six classes; then descs.count = 0 (descriptions are not freed).
__declspec(noinline) void __fastcall input_clear_event_descriptions_005e2f70(InputInternal* self, void*) {
    for (int input_class = 0; input_class < 6; ++input_class) {                               // 005E2F7A / 005E2FD1..DD
        for (int device = 0; device < self->device_count; ++device) {                         // 005E2F82..B0 (count reloaded)
            kotor_list::Internal* const list = self->classes[input_class].device_lists[device].internal; // 005E2F8E..91
            while (list->head != nullptr)                                                     // 005E2F93..A2
                (void)base_ini_reconstruction::remove_head_005e9c80(self->classes[input_class].device_lists[device].internal, nullptr);
        }
        for (int i = 0; i < self->classes[input_class].words.count; ++i)                      // 005E2FB2..CF
            self->classes[input_class].words.data[i] = 0;
    }
    self->descs.count = 0;                                                                    // 005E2FE1
}

// ---- CExoInput wrappers (consumer declarations: r4-lowers adopted/event_descriptions.h)
// 005DF600: mov ecx,[ecx+4]; jmp 005E2F70.
__declspec(noinline) void __fastcall kotor_fastcall_005df600(void* input, void*) {
    input_clear_event_descriptions_005e2f70(load<InputInternal*>(input, 4), nullptr);
}
// 005DF460: mov ecx,[ecx+4]; jmp 005E0E20 (RET 14h).
__declspec(noinline) void __fastcall kotor_fastcall_005df460(void* input, void*, std::uint32_t id, std::uint32_t type,
                                                             std::uint32_t a, std::uint32_t b, std::uint32_t c) {
    (void)input_create_new_event_005e0e20(load<InputInternal*>(input, 4), nullptr, id, type, a, b, c);
}
// 005DF470: mov ecx,[ecx+4]; jmp 005E0FA0 (RET 8).
__declspec(noinline) void __fastcall kotor_fastcall_005df470(void* input, void*, std::uint32_t id, std::uint32_t context) {
    (void)input_add_event_005e0fa0(load<InputInternal*>(input, 4), nullptr, id, context);
}
// 005DF480: mov ecx,[ecx+4]; jmp 005E11D0 (RET 0Ch).
__declspec(noinline) void __fastcall kotor_fastcall_005df480(void* input, void*, std::uint32_t id, float magnitude, float exponent) {
    (void)input_scale_event_005e11d0(load<InputInternal*>(input, 4), nullptr, id, magnitude, exponent);
}
// 005DF490: mov ecx,[ecx+4]; jmp 005E0520 (RET 0Ch).
__declspec(noinline) void __fastcall kotor_fastcall_005df490(void* input, void*, std::uint32_t id, std::int32_t wait, std::int32_t rate) {
    (void)input_set_event_repeatable_005e0520(load<InputInternal*>(input, 4), nullptr, id, static_cast<Word>(wait),
                                              static_cast<Word>(rate));
}
// 005DF6F0 EventExists(id) RET 4: id < count (signed) && descs[id] != 0.
__declspec(noinline) int __fastcall kotor_fastcall_005df6f0(void* input, void*, std::uint32_t id) {
    InputInternal* const internal = load<InputInternal*>(input, 4);                           // 005DF6F0
    if (internal->descs.count <= static_cast<int>(id)) return 0;                              // 005DF6F7..FD (signed JLE)
    return internal->descs.data[id] != 0 ? 1 : 0;                                             // 005DF6FF..10
}
}
