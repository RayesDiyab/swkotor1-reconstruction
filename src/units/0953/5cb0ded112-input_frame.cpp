// lane-m48-traps fork f-input: verbatim extractions (D9 precedent) from lane-input r3 lane-input/src/input_system.cpp
// (sha256 e45e145daa8a263d81713a2ca3cde7497710f3d7463e73cec700d1dc7d8e808e). Bodies are the lane-input text with
// its original-address citations; only the selection of functions and the surrounding declarations differ.
// Menu-reachable members (ProcessInput 006227E0 every frame): CExoInput::UpdateMouseDelta 005DF620 -> 005E0110,
// CExoInput::GetEvents 005DF6B0 (internal 005E24E0 in input_events_frame.cpp), CExoInput::WriteEventToFile 005DF4F0 -> 005E0810
// ('-ecf' event capture), CExoArrayList<CExoInputEvent> Allocate 005E0410 / copy 005E17E0, CExoLinkedListInternal::RemoveTail
// 005E9CE0, device buffer ctor/dtor 005C5A30/005E2FF0. Object layout = staged input-hover/input_system.h (lane-input).
#include "0e7d509bda-input_system.h"
#include <new>
#include "85400bb67b-input_key_ids.h"   // D13 local generated constants (gen_input_consts.py)
#include "0047cfa72a-startup_string.h"
#include "eddb76278d-allocator_live.h"
#include "36201186c4-window_shared_cells.h"
#include "0d2ad8fac8-constructors.h"  // kotor_list::Internal

using namespace kotor_input_hover;
using kotor_reconstruction::StartupString;
namespace ids = kotor_input_ids;

// ---- existing M41 providers (declarations identical to their defining objects) ----
namespace base_ini_reconstruction { struct Node; }
namespace kotor_menu20 { struct Debug; }
namespace ini_addhead { base_ini_reconstruction::Node* __fastcall AddHead005e9d70(kotor_list::Internal*, void*, void*); }
namespace add_key_lower { void __fastcall Remove005e9c10(kotor_list::Internal*, void*, void*); }
namespace base_ini_reconstruction { void __fastcall warning_005e9400(kotor_menu20::Debug*, void*, int, const char*, const char*); }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*, void*, std::uint32_t); }
namespace kotor_menu_10_19 {
struct TimerDependency { unsigned __int64 __thiscall high_resolution_005e9260(); };
namespace dependencies {
extern void* base_007a39e0;
unsigned __int64 __stdcall unsigned_divide_006fb030(unsigned __int64, unsigned __int64);
}
}
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
extern "C" {
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
void* __fastcall unresolved_critical_ctor_005e9290(void*, void*);
StartupString* __fastcall GuiAssign005e5140(StartupString*, void*, const char*);
}
// ---- genuinely missing externals (D1 free thiscall form) ----
namespace kotor_input_required {
void* __fastcall word_array_assign_0051b720(WordArray* self, void*, const WordArray* from);   // CExoArrayList<unsigned long>::operator= RET4
void __fastcall file_write_string_005e6a70(void* file, void*, StartupString* text);         // CExoFile write (005E0894) RET4
}

namespace {
struct BaseView { void* word00; kotor_menu_10_19::TimerDependency* timers; kotor_menu20::Debug* debug; };  // *(007A39E0): +4 timers, +8 debug
BaseView* base() { return static_cast<BaseView*>(kotor_menu_10_19::dependencies::base_007a39e0); }
}

DeviceBuffer* DeviceBuffer::Construct005c5a30() { count = 0; word08 = 0; data = nullptr; return this; }  // 005C5A34..3A
void DeviceBuffer::Destroy005e2ff0() {
    if (data) { live_free(data); data = nullptr; }                         // 005E2FF5..3002
}

void Input::UpdateMouseDelta(float d) { internal->UpdateMouseDelta(d); }   // 005DF620
void Input::WriteEventToFile(U32 t, int i, void* f) { internal->WriteEventToFile(t, i, f); }  // 005DF4F0
InputEventList Input::GetEvents(int* count, int input_class) {            // 005DF6B0: result in caller slot
    InputEventList result;
    internal->GetEvents(&result, count, input_class);                      // 005DF6CC
    return result;
}

// ---- 005E0110 UpdateMouseDelta (argument unused) ----
void InputInternal::UpdateMouseDelta(float) {
    DIMOUSESTATE state{};                                                  // 005E0113..2F zero 16 bytes
    if (raw->GetMouseState(&state) != 1) {                                 // 005E0129..3B
        delta_x = 0; delta_y = 0;                                          // 005E020F..15 (integer zero)
        return;
    }
    const float sensitivity = kotor_input_cells::mouse_divisor_007a2278;   // 007A2278 (.data 100.0f; only readers 005E014F/005E01AC)
    auto scaled = [&](LONG axis) {
        const double v = static_cast<double>(-axis) / static_cast<double>(sensitivity);  // FILD/FDIV (PC53)
        if (1.0 < v) return 1.0f;                                          // 005E0155..62 / 005E0181..92
        if (!(-1.0 < v || -1.0 == v) && v == v) return -1.0f;              // 005E0166..79 (NaN keeps value)
        return static_cast<float>(v);                                      // FSTP float
    };
    delta_x = scaled(state.lX);                                            // 005E0141..9C
    delta_y = scaled(state.lY);                                            // 005E0198..200
}

// ---- 005E0810 WriteEventToFile(time, index, file) ----
void InputInternal::WriteEventToFile(U32 time, int index, void* file) {
    if (!file && index >= 0 && index < event_count) return;               // 005E0831..42 (observed gate)
    const InputEvent* e = &events.data[index];                             // 005E0845..51
    InputEventDesc* d = e->desc;
    StartupString text;                                                    // 005E0859
    kotor_reconstruction::StartupFormat005e5680(&text, ids::event_format_0074d614,
        time, 0x49, d->id, events.data[index].value);                      // 005E085E..85 (value reloaded)
    kotor_input_required::file_write_string_005e6a70(file, nullptr, &text);  // 005E088D..94
}                                                                          // 005E08A5 ~CExoString

// ======================= event list instantiations ===========================
void __fastcall kotor_input_hover::EventListAllocate005e0410(InputEventList* list, void*, int capacity) {
    list->capacity = capacity;                                             // 005E0417
    InputEvent* old = list->data;                                          // 005E041B
    list->data = static_cast<InputEvent*>(live_operator_new(static_cast<std::uint32_t>(capacity) << 4));  // 005E041D..26
    for (int i = 0; i < list->count; ++i) list->data[i] = old[i];          // 005E0428..5E
    if (old) live_free(old);                                               // 005E0462..67
}
InputEventList* __fastcall kotor_input_hover::EventListCopy005e17e0(InputEventList* self, void*, const InputEventList* from) {
    self->count = 0; self->capacity = 0; self->data = nullptr;             // 005E17EE..F4
    for (int i = 0; i < from->count; ++i) {                                // 005E17F6 / 005E1862..6F (count reloaded)
        const InputEvent e = from->data[i];                                // 005E1800..0C
        if (self->count == self->capacity)
            EventListAllocate005e0410(self, nullptr, self->capacity ? self->capacity * 2 : 0x10);  // 005E181D..35
        self->data[self->count] = e;                                       // 005E183A..58
        ++self->count;                                                     // 005E185B
    }
    return self;
}
void* __fastcall kotor_input_hover::ListRemoveTail005e9ce0(kotor_list::Internal* list, void*) {
    ListNode* tail = static_cast<ListNode*>(list->tail);                   // 005E9CE3
    if (!tail) {
        base_ini_reconstruction::warning_005e9400(base()->debug, nullptr, 0x18B,
            ids::list_source_0074e208, ids::remove_tail_empty_0074e3a0);   // 005E9CEA..9D01
        return nullptr;
    }
    ListNode* before = tail->prev;                                         // 005E9D0A
    void* data = tail->data;                                               // 005E9D0F
    list->tail = before;                                                   // 005E9D12
    if (!before) list->head = before; else before->next = nullptr;         // 005E9D15..1B
    live_free(tail);                                                       // 005E9D23
    --list->count;                                                         // 005E9D28..2F
    return data;
}
