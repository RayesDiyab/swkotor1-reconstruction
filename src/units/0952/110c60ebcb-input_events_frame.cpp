// lane-m48-traps fork f-input: verbatim extractions (D9 precedent) from lane-input r3 lane-input/src/input_events.cpp
// (sha256 e238970fb7b1adc0dc976feb1db1cbaa70aa76c65cb9af307900524296521bc2): CExoRawInputInternal ShutDownDirectInput 005E3DC0,
// InitializeDirectInputMouse 005E3FA0, GetMouseState 005E4050, GetKeyboardBuffer 005E3AE0, IsDigital 005E39F0, GetLastState 005E3930,
// CExoInputInternal::GetEvents 005E24E0, DeadZone 0063E7C0, cell 007A2278; plus the D83 function trap for
// CExoRawInputInternal::GetJoystickBuffer 005E30E0 (joystick count [raw+18h] is only ever written 0: 005E3F6D).
#include "0e7d509bda-input_system.h"
#include "85400bb67b-input_key_ids.h"   // D13 local generated constants
#include "eddb76278d-allocator_live.h"
#include "36201186c4-window_shared_cells.h"
#include "0d2ad8fac8-constructors.h"

using namespace kotor_input_hover;
namespace ids = kotor_input_ids;

namespace base_ini_reconstruction { struct Node; }
namespace kotor_menu20 { struct Debug; }
namespace ini_addhead { base_ini_reconstruction::Node* __fastcall AddHead005e9d70(kotor_list::Internal*, void*, void*); }
namespace add_key_lower { void __fastcall Remove005e9c10(kotor_list::Internal*, void*, void*); }
namespace base_ini_reconstruction { void __fastcall warning_005e9400(kotor_menu20::Debug*, void*, int, const char*, const char*); }
namespace kotor_menu_10_19 {
struct TimerDependency { unsigned __int64 __thiscall high_resolution_005e9260(); };
namespace dependencies {
extern void* base_007a39e0;
unsigned __int64 __stdcall unsigned_divide_006fb030(unsigned __int64, unsigned __int64);
}
}
extern "C" {
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
}

namespace {
// .rdata GUID words 0074D694 / 0074D6A4 / 0074D6F4 (equal to the public dinput values).
const GUID kSysKeyboard_0074d694 = {0x6F1D2B61, 0xD5A0, 0x11CF, {0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
const GUID kSysMouse_0074d6a4 = {0x6F1D2B60, 0xD5A0, 0x11CF, {0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00}};
const GUID kIID_IDirectInput8A_0074d6f4 = {0xBF798030, 0x483A, 0x4DA2, {0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00}};
// 0074D8FC / 0074DB04 are the statically linked dinput8.lib c_dfDIKeyboard / c_dfDIMouse
// ({18h,10h,2,100h,100h,..} / {18h,10h,2,10h,7,..}); the same library objects are used here.

struct BaseView { void* word00; kotor_menu_10_19::TimerDependency* timers; kotor_menu20::Debug* debug; };
BaseView* base() { return static_cast<BaseView*>(kotor_menu_10_19::dependencies::base_007a39e0); }
U32 now_ms() {
    const unsigned __int64 t = base()->timers->high_resolution_005e9260();
    return static_cast<U32>(kotor_menu_10_19::dependencies::unsigned_divide_006fb030(t, 1000u));
}
void* list_at(kotor_list::Internal* l, void* pos) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_at_pos_005e9b60(l, nullptr, pos))); }
void* list_next(kotor_list::Internal* l, void** pos) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_next_005e9b90(l, nullptr, pos))); }
void append_event(InputInternal* self, InputEventList* list, int value, U32 w4, U32 w8, void* desc) {
    if (list->count == list->capacity)
        EventListAllocate005e0410(list, nullptr, list->capacity ? list->capacity * 2 : 0x10);
    InputEvent* e = list->data + list->count;
    e->value = value; e->word04 = w4; e->word08 = w8; e->desc = static_cast<InputEventDesc*>(desc);
    ++list->count;
    ++self->event_count;
}
DIPROPDWORD buffer_size_property() {                       // 005E3F25..3D / 005E3FE9..4006
    DIPROPDWORD p;
    p.diph.dwSize = 0x14; p.diph.dwHeaderSize = 0x10; p.diph.dwObj = 0; p.diph.dwHow = 0; p.dwData = 0x100;
    return p;
}
}

namespace kotor_input_cells {
float volatile mouse_divisor_007a2278 = 100.0f;            // PE .data 0x42C80000
}

// ====================== CExoRawInputInternal =================================
void RawInput::ShutDownDirectInput005e3dc0() {
    if (!direct_input) return;                                             // 005E3DC4..CB
    if (keyboard) { keyboard->Unacquire(); keyboard->Release(); keyboard = nullptr; }  // 005E3DD1..E7
    if (mouse) { mouse->Unacquire(); mouse->Release(); mouse = nullptr; }  // 005E3DEA..3E00
    for (int i = 0; i < joystick_count; ++i) {                             // 005E3E03..38 (count reloaded)
        if (joysticks[i]) {
            joysticks[i]->Unacquire();                                     // 005E3E1A..1D
            joysticks[i]->Release();                                       // 005E3E20..29 (pointer reloaded)
            joysticks[i] = nullptr;                                        // 005E3E2C..2F
        }
    }
    if (word2c) { live_free(word2c); word2c = nullptr; }                   // 005E3E3A..4B
    if (joystick_states) live_free(joystick_states);                       // 005E3E4E..56 (field not cleared)
    if (joysticks) { live_free(joysticks); joysticks = nullptr; }          // 005E3E5E..6E
    direct_input->Release();                                               // 005E3E71..77
    direct_input = nullptr;                                                // 005E3E7A
}
int RawInput::InitializeDirectInputMouse() {                               // 005E3FA0
    if (direct_input->CreateDevice(kSysMouse_0074d6a4, &mouse, nullptr) != 0) { ShutDownDirectInput005e3dc0(); return 0; }  // 005E3FA7..BD
    if (mouse->SetDataFormat(&c_dfDIMouse) != 0) { ShutDownDirectInput005e3dc0(); return 0; }   // 005E3FBF..CE
    if (mouse->SetCooperativeLevel(static_cast<HWND>(kotor_menu_window_cells::render_007a39d8), 0x0A) != 0) {  // 005E3FD0..E3
        ShutDownDirectInput005e3dc0(); return 0;
    }
    DIPROPDWORD p = buffer_size_property();
    if (mouse->SetProperty(DIPROP_BUFFERSIZE, &p.diph) != 0) { ShutDownDirectInput005e3dc0(); return 0; }  // 005E3FE5..16
    if (mouse->Acquire() != 0) ShutDownDirectInput005e3dc0();             // 005E4027..35 (still returns 1)
    return 1;                                                              // 005E403B
}
int RawInput::GetMouseState(DIMOUSESTATE* state) {                        // 005E4050 RET4
    if (!mouse) InitializeDirectInputMouse();                              // 005E4053..5A
    if (!mouse || !active) return 0;                                       // 005E405F..6B
    const HRESULT hr = mouse->GetDeviceState(0x10, state);                 // 005E406D..77
    if (hr == 0) return 1;                                                 // 005E407A..7C
    if (hr == static_cast<HRESULT>(0x8007000Cu) || hr == static_cast<HRESULT>(0x8007001Eu))  // 005E407E..9A
        return mouse->Acquire() == 0 ? 1 : 0;                              // 005E409C..A7 (device pointer reloaded)
    return 0;                                                              // 005E40A9
}
int RawInput::GetKeyboardBuffer(DeviceBuffer* out) {                      // 005E3AE0 RET4
    if (!keyboard || !active) return 0;                                    // 005E3AE3..F4
    if (out->data) { live_free(out->data); out->data = nullptr; }          // 005E3AFC..0C
    out->count = 0;                                                        // 005E3B12
    if (keyboard_mode) return 1;                                           // 005E3B19..1E -> 005E3BD4
    DIDEVICEOBJECTDATA* data = static_cast<DIDEVICEOBJECTDATA*>(live_operator_new(0x1400));  // 005E3B24..29
    out->data = data;                                                      // 005E3B37
    if (!data) return 0;                                                   // 005E3B35..39 (EAX = 0)
    std::memset(data, 0, 0x100);                                           // 005E3B46..4B (REP STOS 40h dwords only)
    DWORD n = 0x100;                                                       // in-out word reuses the argument slot (005E3B41/55)
    const HRESULT hr = keyboard->GetDeviceData(0x14, data, &n, 0);         // 005E3B4D..60
    if (hr == 0) { out->count = n; return 1; }                             // 005E3BCD..D4
    if (hr == 1) return 1;                                                 // 005E3BC0..C3 (count left 0)
    if (hr == static_cast<HRESULT>(0x8007001Eu))                           // 005E3B6E..85
        return keyboard->Acquire() == 0 ? 1 : 0;
    if (hr == static_cast<HRESULT>(0x8007000Cu)) { keyboard->Acquire(); return 1; }  // 005E3BA5..B6
    return 0;                                                              // 80040207, 80070015, 80070057, others
}
int RawInput::IsDigital(U32 device, U32 key) {                            // 005E39F0 RET8
    if (device == ids::k_0074d3c8) return 1;                               // 005E39F4..FA
    if (device == ids::k_0074d3cc) {                                       // 005E39FC..02
        if (key >= 0xC && key <= 0xF) return 1;                            // 005E3A08..10
        return key <= 8 ? 1 : 0;                                           // 005E3A12..1B (CMP/SBB/INC)
    }
    if (static_cast<int>(device) < static_cast<int>(ids::k_0074d3d0)) return 0;  // 005E3A1F..25 (JL)
    if (key >= 0x30 && key <= 0x4F) return 1;                              // 005E3A2B..33
    if (key >= 0x384 && key <= 0x393) return 1;                            // 005E3A35..41
    return key == 0x20 ? 1 : 0;                                            // 005E3A43..50
}
int RawInput::GetLastState(U32 device, U32 key) {                         // 005E3930 RET8
    if (device >= 6) return 0;                                             // 005E3934..37
    if (device == ids::k_0074d3c8 || device == ids::k_0074d3cc) return 1;  // 005E3939..4E
    if (static_cast<int>(device) < static_cast<int>(ids::k_0074d3d0)) return 0;  // 005E3951..57
    if (key >= 0x30 && key <= 0x4F) return 0;                              // 005E395D..65
    if (key == 0x20) return 0;                                             // 005E3967..6A
    if (key != 0 && key != 4 && key != 0x18 && key != 0x1C) return 0;     // 005E396C..7D
    const unsigned char* s = static_cast<const unsigned char*>(joystick_states) + (device - 2) * 0x74;  // 005E397F..85
    int v;
    switch (key) {                                                         // tables 005E39CC / 005E39B8
    case 0: std::memcpy(&v, s + 0x00, 4); return v;                        // 005E399B
    case 4: std::memcpy(&v, s + 0x04, 4); return v;                        // 005E39A0
    case 0x18: std::memcpy(&v, s + 0x18, 4); return v;                     // 005E39A6
    default: std::memcpy(&v, s + 0x14, 4); return v;                       // 0x1C -> 005E39AC (+0x14)
    }
}

// ================== CExoInputInternal::GetEvents 005E24E0 ====================
InputEventList* InputInternal::GetEvents(InputEventList* result, int* count, int input_class) {
    DeviceBuffer buffers[6];                                               // [esp+6Ch] eh_vector ctor 005C5A30
    for (DeviceBuffer& b : buffers) b.Construct005c5a30();
    int chosen = -1;                                                       // EBP / [esp+1Ch]
    *count = 0;                                                            // 005E2533
    events.count = 0;                                                      // 005E2535
    RawInput* r = raw;                                                     // 005E2538
    if (!r) {
        result->count = 0; result->capacity = 0; result->data = nullptr;   // 005E2559..5F
    } else if (input_class >= 6) {                                         // 005E2571..74 (signed)
        base_ini_reconstruction::warning_005e9400(base()->debug, nullptr, 0x5CB,
            ids::source_file_0074d640, ids::invalid_class_0074d658);       // 005E2576..8D
        result->count = 0; result->capacity = 0; result->data = nullptr;   // 005E25A6..AC
    } else {
        InputClass& cls = classes[input_class];
        void* positions[6];                                                // [esp+B4h]
        U32 next[6];                                                       // [esp+44h]
        for (int i = 0; i < device_count; ++i) positions[i] = cls.device_lists[i].internal->head;  // 005E25B8..E7
        if (capture_keyboard_keys == 0) r->GetKeyboardBuffer(&buffers[ids::k_0074d3c8]);  // 005E25E9..FE
        for (int d = 2; d < device_count; ++d) raw->GetJoystickBuffer(static_cast<U32>(d - 2), &buffers[d]);  // 005E2603..3C
        U32 stamp = 1;                                                     // [esp+14h] (empty flag, reused as sequence)
        if (device_count > 0) {
            for (int d = 0; d < device_count; ++d) next[d] = 0;            // 005E2650..5E REP STOS
            for (int d = 0; d < device_count; ++d) if (buffers[d].count > 0) stamp = 0;  // 005E2660..6C
        }
        event_count = 0;                                                   // 005E2678
        U32 total = 0;
        for (int d = 0; d < device_count; ++d) total += buffers[d].count;  // 005E2684..96
        if (device_count > 0 && total != 0 && stamp == 0) {               // 005E267E / 98 / A0
            bool all_done;
            do {
                int found = 0;                                             // [esp+20h]
                for (int i = 0; i < device_count; ++i) {                   // [esp+24h] (count reloaded 005E27C2)
                    if (chosen != -1 && i != chosen && next[chosen] < buffers[chosen].count) continue;  // 005E26C0..D5
                    if (found) continue;                                   // 005E26DB..E1
                    while (next[i] < buffers[i].count) {                   // 005E26F0..700
                        const DIDEVICEOBJECTDATA* entry = &buffers[i].data[next[i]];  // [esp+10h]
                        ListWrapper& lw = cls.device_lists[i];             // EBX
                        positions[i] = lw.internal->head;                  // 005E272B..3E
                        bool match = false;
                        if (positions[i]) {
                            const ListNode* head = static_cast<const ListNode*>(lw.internal->head);  // 005E2743..50 (inlined GetHead)
                            void* data = head ? head->data : nullptr;
                            for (;;) {
                                EventDesc* desc = reinterpret_cast<EventDesc*>(descs.data[*static_cast<U32*>(data)]);  // 005E2750..58
                                if (keys[desc->key] == entry->dwOfs) { match = true; break; }   // 005E275B..6B
                                if ((desc->*(desc->vtable->has_alt_key))() &&                   // 005E276D..76
                                    keys[desc->alt_key] == entry->dwOfs) { match = true; break; } // 005E2778..88
                                data = list_next(lw.internal, &positions[i]);                   // 005E278A..8D
                                if (!positions[i]) break;                                       // 005E2792..96
                            }
                        }
                        if (match) { found = 1; break; }                   // 005E27AE..B2
                        ++next[i];                                         // 005E279C..A5
                    }
                }
                if (found) {                                               // 005E27DC..E2
                    int got = 0;                                           // EDI
                    for (int c = 0; c < device_count;) {                   // 005E27F0..33
                        if (next[c] < buffers[c].count) {
                            const U32 seq = buffers[c].data[next[c]].dwSequence;  // +0Ch
                            if (chosen == -1 || stamp == seq) { stamp = seq; chosen = c; got = 1; }  // 005E2803..27
                        }
                        ++c;
                        if (got == 1) break;                               // 005E2830
                    }
                    if (!got) { ++stamp; chosen = -1; }                    // 005E283D..49
                    else {
                        const DIDEVICEOBJECTDATA* entry = &buffers[chosen].data[next[chosen]];  // 005E2852..61 (EDI)
                        void* data = list_at(cls.device_lists[chosen].internal, positions[chosen]);  // 005E2864..7E
                        EventDesc* desc = reinterpret_cast<EventDesc*>(descs.data[*static_cast<U32*>(data)]);  // 005E2883..8B
                        if (desc->disabled == 0 && static_cast<U32>(desc->type) <= 5) {  // 005E288E..9F
                            switch (desc->type) {                          // table 005E2F4C
                            case 1:                                        // 005E28AC
                                if (raw->IsDigital(static_cast<U32>(chosen), entry->dwOfs)) {
                                    desc->state = static_cast<int>(entry->dwData);              // 005E28CC
                                    append_event(this, &events, static_cast<int>(entry->dwData), entry->dwTimeStamp, entry->dwSequence, desc);
                                } else {
                                    const int dz = raw->DeadZone0063e7c0(entry->dwOfs);         // 005E291F..28
                                    const int ndz = -dz;                                        // 005E2936
                                    const int data_now = static_cast<int>(entry->dwData);
                                    const int now = data_now < ndz ? -1 : (data_now > dz ? 1 : 0);  // 005E2938..51
                                    const int last_raw = raw->GetLastState(static_cast<U32>(chosen), entry->dwOfs);  // 005E2955..63
                                    const int last = last_raw < ndz ? -1 : (last_raw > dz ? 1 : 0);  // 005E2968..7C
                                    if (now != last) {                                          // 005E297E..84
                                        desc->state = now;                                      // 005E2996
                                        append_event(this, &events, now, entry->dwTimeStamp, entry->dwSequence, desc);
                                    }
                                }
                                break;
                            case 3: desc->accum += static_cast<int>(entry->dwData); break;        // 005E29E8
                            case 4:                                                             // 005E29F8
                                if (entry->dwOfs == keys[desc->key]) desc->accum = static_cast<int>(entry->dwData);
                                else desc->alt_accum = static_cast<int>(entry->dwData);
                                break;
                            default: desc->state = static_cast<int>(entry->dwData); break;        // 0,2,5 -> 005E29F0
                            }
                        }
                        ++next[chosen];                                    // 005E2A14..1D
                    }
                }
                all_done = true;                                           // 005E2A21..4C (EBX)
                for (int d = 0; d < device_count; ++d) if (next[d] < buffers[d].count) all_done = false;
            } while (!all_done);
        }
        if (raw && raw->active) {                                          // 005E2A52..65
            if (pending_x != -1) BufferRaw(ids::k_0074d3cc, ids::k_0074d3ec, pending_x);  // 005E2A6B..ADD (inlined 005E08C0 body)
            if (pending_y != -1) BufferRaw(ids::k_0074d3cc, ids::k_0074d3f0, pending_y);  // 005E2AE6..B5D
        }
        while (static_cast<const kotor_list::Internal*>(buffered.internal)->head) {  // 005E2B62..6B / 005E2C69..76
            RawEvent* ev = static_cast<RawEvent*>(ListRemoveTail005e9ce0(buffered.internal, nullptr));  // 005E2B82..88
            ListWrapper& lw = cls.device_lists[ev->desc->device];          // 005E2B8F..9D (class*12 + device)
            void* pos = lw.internal->head;                                 // 005E2BA1..A5
            if (pos) {
                for (;;) {
                    void* data = list_at(lw.internal, pos);                // 005E2BB0..B4
                    EventDesc* desc = reinterpret_cast<EventDesc*>(descs.data[*static_cast<U32*>(data)]);  // 005E2BB9..C1
                    if ((desc->key & 0xFFu) == ev->desc->control) {        // 005E2BC4..D4
                        const int value = ev->value; const U32 w4 = ev->word04, w8 = ev->word08;  // 005E2BED..2C01
                        if ((desc->*(desc->vtable->has_repeat))()) desc->accum = value;  // 005E2BF5..0B
                        append_event(this, &events, value, w4, w8, desc);              // 005E2C0E..54
                        break;
                    }
                    list_next(lw.internal, &pos);                          // 005E2BD6..DE
                    if (!pos) break;                                       // 005E2BE3..E9
                }
            }
            live_free(ev->desc);                                           // 005E2C5A..5E
            live_free(ev);                                                 // 005E2C63..64
        }
        pending_x = -1; pending_y = -1;                                    // 005E2C7C..85
        const U32 now = now_ms();                                          // 005E2C8B..A2
        const U32 elapsed = now - time_ms;                                 // 005E2CB3..BD
        time_ms = now;                                                     // 005E2CB5
        void* pos = timed.internal->head;                                  // 005E2CAD..C5
        while (pos) {                                                      // 005E2CC9 / 005E2D2E
            TimedEntry* item = *static_cast<TimedEntry**>(list_at(timed.internal, pos));  // 005E2CD0..DC
            if (item) {
                if (item->remaining <= elapsed) {                          // 005E2CE2..E7 (unsigned)
                    void* current = pos;                                   // 005E2CE9
                    item->remaining = 0;                                   // 005E2CF1
                    list_next(timed.internal, &pos);                       // 005E2CF4..FB
                    add_key_lower::Remove005e9c10(timed.internal, nullptr, current);  // 005E2D00..07
                    continue;
                }
                item->remaining -= elapsed;                                // 005E2D0E..10
            }
            list_next(timed.internal, &pos);                               // 005E2D13..25
        }
        pos = repeats.internal->head;                                      // 005E2D32..3C
        while (pos) {
            RepeatEntry* e = static_cast<RepeatEntry*>(list_at(repeats.internal, pos));  // 005E2D50..5C
            if (e && IsEventInClass(e->desc_index, input_class)) {         // 005E2D60..7C
                EventDesc* d = reinterpret_cast<EventDesc*>(descs.data[e->desc_index]);  // 005E2D82..92
                const int state = d->state;                                // 005E2D8D
                if (state == 0) {                                          // 005E2D90..96 -> 005E2EAB
                    e->elapsed = 0; e->repeating = 0; e->last = 0; e->current = 0;
                } else if (e->elapsed == 0 && e->repeating == 0) {         // 005E2D9C..A6
                    e->elapsed = 1; e->last = d->state; e->current = d->state;  // 005E2DA8..B5
                } else if (e->last != state) {                             // 005E2DBD..C0 -> 005E2ED5
                    e->elapsed = 1; e->last = d->state; e->current = d->state; e->repeating = 0;
                    break;                                                 // falls to 005E2EE8 (loop exit, observed)
                } else {
                    e->elapsed += elapsed;                                 // 005E2DC6..CF
                    if (e->repeating) {                                    // 005E2DCC..D2
                        if (e->elapsed > e->repeat_delay) {                // 005E2DD8..DB (unsigned)
                            int v;
                            if (e->current == e->last) { v = 0; e->current = 0; }      // 005E2DE1..ED
                            else { v = e->last; e->current = e->last; }                 // 005E2DF2..F6
                            append_event(this, &events, v, 0, 0, d);       // 005E2DF9..38
                            e->elapsed = 0;                                // 005E2E3E
                        }
                    } else if (e->elapsed > e->initial_delay) {            // 005E2E4C..4F
                        e->current = 0;                                    // 005E2E51
                        append_event(this, &events, 0, 0, 0, d);           // 005E2E58..93
                        e->repeating = 1;                                  // 005E2E99
                        e->elapsed = 0;                                    // 005E2EA0
                    }
                }
            }
            list_next(repeats.internal, &pos);                             // 005E2EB7..C2
        }
        *count = event_count;                                              // 005E2EE8..F03
        EventListCopy005e17e0(result, nullptr, &events);                   // 005E2F00..05
    }
    for (DeviceBuffer& b : buffers) b.Destroy005e2ff0();                   // 005E2F12..28 eh_vector dtor 005E2FF0
    return result;
}

int RawInput::DeadZone0063e7c0(U32) { return 0; }                          // 0063E7C0 xor eax,eax; ret 4 (lane-input input_system.cpp:89)

// 005E30E0 GetJoystickBuffer: D83 function trap lives in process_input.cpp (separate TU; keeps this TU free of noreturn inference).
