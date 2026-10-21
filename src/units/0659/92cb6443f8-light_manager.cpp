// lane-3d-construct unit light_manager: LightManager typed table 00743958 (25 slots, published in construct3d.h as
// kotor_scene3d_construct::LightManagerVTable) and every slot body plus the lowers only this family uses
// (lightcmp 004A28C0, DisableThisLight 004A4F10, EnableThisLight 004A4FF0, Aur{En,Dis}ableHeadLight 0044DA60/0044DA80,
// atexit dtor 0073BE90). Gate G2. Evidence: dumpbin /disasm swkotor.exe (GOG K1), lane-local d/<ADDR>.txt / d/<ADDR>.s.
// Float policy D30-FINAL: x87 PC_53 => double intermediates; float exactly where the original stores `fst/fstp dword`.
// EH FuncInfo 00718098 / 0071807E / 00716E80 only free local lists (no callee throws; not emitted).
#include "6c81d8874d-scene3d.h"
#include "75b7842bf0-renderer_gl_imports_gen.h"   // kotor_renderer_init::glLightfv_0073d388 / glEnable_0073d390 / glDisable_0073d22c
#include "516b075ab1-construct3d.h"
#include <cstdint>
#include <cstdlib>

extern "C" {
void __fastcall exo_array_list_remove_00445080(kotor_reconstruction::ArrayMutations* list, void*, void* value);   // lane-3d-bsp spelling
kotor_reconstruction::ArrayMutations* __fastcall texture_list_assign_0046bc80(kotor_reconstruction::ArrayMutations* self, void*,
                                                                              const kotor_reconstruction::ArrayMutations* source); // lane-startup-misc
// core: CExoArrayList<float>::operator= 00438A50 thiscall(this, source) RET 4, EAX = this (004A43C0)
kotor_reconstruction::ArrayMutations* __fastcall float_list_assign_00438a50(kotor_reconstruction::ArrayMutations* self, void*,
                                                                            const kotor_reconstruction::ArrayMutations* source);
aurora_model::MdlNode* __fastcall aurora_node_as_light_0043e3d0(aurora_model::MdlNode* self, void* unused_edx);       // lane-3d-model
}
namespace kotor_renderer_gl {   // lane-renderer-bodies render_pool.h (identical declaration): CExoArrayList::allocate 00437D60
void __fastcall allocate_00437d60(kotor_reconstruction::ArrayMutations* self, void* unused_edx, std::uint32_t capacity);
}
namespace kotor_renderer_gl_cells { extern std::uint16_t format_0078e388; }   // lane-renderer-bodies storage (head-light word)
namespace kotor_vp_cells {                                                    // lane-3d-vp vp_cells.cpp storage (identical declarations)
extern volatile std::uint32_t refresh_007a193c;
extern kotor_scene3d::Vector headlight_a_007a194c;   // .data {0.8f x3}
extern kotor_scene3d::Vector headlight_b_007a1958;   // .data {0.2f x3}
}

// ---------------------------------------------------------------- cells referenced only by this family (storage here)
namespace kotor_light_cells {
volatile std::uint32_t fade_gate_008305f4 = 0;        // BSS; read 004A4583 only, no writer
volatile std::int32_t max_lights_007a1934 = 3;        // .data 3; read 004A459D only
volatile std::uint32_t active_count_008305e8 = 0;     // BSS; written 004A3093 only
volatile std::uint32_t dynamic_count_008305ec = 0;    // BSS; written 004A4517 only
volatile std::uint32_t enabled_count_008305e4 = 0;    // BSS; written 004A49FF / 004A4A07 only
volatile std::uint32_t negative_light_008305f0 = 0;   // BSS; written 004A4A0C / 004A4A8B only
volatile float light_fade_rate_007a1940 = 3.0f;       // .data 40400000h; read 004A4099 / 004A40E8
volatile float shadow_fade_rate_007a1944 = 3.0f;      // .data 40400000h; read 004A427F..004A4326
// function-local static CExoArrayList<Light*> of 004A3F70 (data 00831E08, size 00831E0C, capacity 00831E10) + guard 00831E14
kotor_reconstruction::ArrayMutations highest_priority_lights_00831e08 = {};
volatile std::uint32_t highest_priority_guard_00831e14 = 0;
}

namespace {
using namespace kotor_scene3d;
namespace C3 = kotor_scene3d_construct;
namespace LC = kotor_light_cells;
using C3::List; using C3::PartSlots; using C3::slots_of; using C3::LightManagerVTable;

inline const LightManagerVTable* vt(void* self) { return slots_of<LightManagerVTable>(self); }
inline List* list_at(void* object, U offset) { return static_cast<List*>(at(object, offset)); }
inline void* element(U data, I index) {
    return *reinterpret_cast<void* const volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u);
}
inline void set_element(U data, I index, void* value) {
    *reinterpret_cast<void* volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u) = value;
}
inline float* float_at(U data, I index) { return reinterpret_cast<float*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u); }
inline U word_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline float f(U bits) { return bits_to_float(bits); }
inline double dt() { return static_cast<double>(f(*cells.frame_time_0078e574)); }    // [0078E574] (raw DWORD cell)
inline U16 head_light() { return static_cast<volatile std::uint16_t&>(kotor_renderer_gl_cells::format_0078e388); }
// count of `value` in a CExoArrayList (the original's inlined Contains loops: size read once, data read once)
inline I count_of(const List* list, const void* value) {
    I n = list->size, found = 0;
    if (n <= 0) return 0;
    U cursor = list->data;
    do { if (*reinterpret_cast<void* const volatile*>(static_cast<std::uintptr_t>(cursor)) == value) ++found; cursor += 4; } while (--n != 0);
    return found;
}
inline void add_allocate(List* list, void* value) {           // inlined Add with 005E03C0 growth
    const I capacity = list->capacity;
    if (list->size == capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    set_element(list->data, list->size, value);
    list->size = list->size + 1;
}
inline void add_new(List* list, void* value) {                // inlined Add with operator-new growth (0046DF0F..0046DF87)
    const I capacity = list->capacity;
    if (list->size == capacity) {
        const I grown = capacity != 0 ? capacity + capacity : 8;
        const U old = list->data;
        list->capacity = grown;                                   // stored before the allocation
        list->data = word_of(live_operator_new(static_cast<U>(grown) * 4u));
        for (I k = 0; k < list->size; ++k) set_element(list->data, k, element(old, k));
        if (old != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old)));
    }
    set_element(list->data, list->size, value);
    list->size = list->size + 1;
}
inline void remove_at(List* list, I index) {                  // inlined RemoveAt (size-1 first, then shift)
    const I last = list->size - 1;
    list->size = last;
    for (I k = index; k < list->size; ++k) set_element(list->data, k, element(list->data, k + 1));
}
inline void unique_remove(List* list, void* value) {          // `count; if (found) Remove` pattern (004A3B3A..004A3C90)
    if (list->size > 0 && count_of(list, value) != 0) exo_array_list_remove_00445080(list, nullptr, value);
}
inline void set_light(I index, void* light, double intensity_scale) {
    // colour = (s*c4C, s*c50, s*c54) rounded to float, then SetLightGL(index, +94, &colour, +58, +68)
    Vector colour;
    colour.x = static_cast<float>(intensity_scale * static_cast<double>(field<float>(light, 0x4C)));
    colour.y = static_cast<float>(intensity_scale * static_cast<double>(field<float>(light, 0x50)));
    colour.z = static_cast<float>(intensity_scale * static_cast<double>(field<float>(light, 0x54)));
    set_light_gl_004a2a00(index, at(light, 0x94), &colour, static_cast<const float*>(at(light, 0x58)), static_cast<const U*>(at(light, 0x68)));
}
void __cdecl free_highest_priority_lights_list_0073be90() {   // 0073BE90 (atexit, registered at 004A3FC7)
    const U data = LC::highest_priority_lights_00831e08.data;
    if (data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data)));
}
} // namespace

extern "C" {
// ================================================================ small lowers
// [M45 compose] 0044DA60 body removed: existing M44 provider lane-routes kotor_cdecl_0044da60 wins (definition removed; declaration kept)
void __cdecl aur_enable_head_light_0044da60();
// [M45 compose] 0044DA80 body removed: existing M44 provider lane-routes kotor_cdecl_0044da80 wins (definition removed; declaration kept)
void __cdecl aur_disable_head_light_0044da80();
// 004A28C0 lightcmp(a, b) (qsort comparator over CExoArrayList<Light*> elements)
int __cdecl light_compare_004a28c0(const void* left, const void* right) {
    void* const pa = *static_cast<void* const*>(left);
    void* const a = slots_of<PartSlots>(pa)->as_light(static_cast<aurora_model::Part*>(pa), nullptr);   // 004A28C0..004A28CF slot +24
    void* const pb = *static_cast<void* const*>(right);
    void* const b = slots_of<PartSlots>(pb)->as_light(static_cast<aurora_model::Part*>(pb), nullptr);   // 004A28D4..004A28DC
    const I prio_a = field<I>(a, 0x74) != 0 ? field<I>(a, 0x74) : field<I>(field<void*>(a, 4), 0x90);  // 004A28E1..004A28F2
    const I prio_b = field<I>(b, 0x74) != 0 ? field<I>(b, 0x74) : field<I>(field<void*>(b, 4), 0x90);  // 004A28F8..004A2901
    Vector zero; zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;
    if (aurora_vector_equal_004aa980(at(a, 0x4C), &zero) != 0) return 1;              // 004A2907..004A2932
    if (aurora_vector_equal_004aa980(at(b, 0x4C), &zero) != 0) return -1;             // 004A2938..004A2957
    if (prio_a < prio_b) return -1;                                                     // 004A2959..004A295B (signed)
    if (prio_a > prio_b) return 1;                                                      // 004A295D
    const I kind_a = field<I>(a, 0x70), kind_b = field<I>(b, 0x70);
    if (kind_a == 0 && kind_b > 0) return 1;                                            // 004A295F..004A296B
    if (kind_a == 1) {                                                                  // 004A296D..004A297C
        if (kind_b == 0 || kind_b == 2) return -1;
    } else if (kind_a == 2) {                                                           // 004A2989..004A2993
        if (kind_b == 0) return -1;
    } else if (kind_b == 1) {                                                           // 004A29A0..004A29A3
        return 1;
    }
    const float wa = field<float>(a, 0x80), wb = field<float>(b, 0x80);
    if (wa < wb) return -1;                                                             // 004A29A5..004A29C4 (test ah,5; jp)
    if (wa > wb) return 1;                                                              // 004A29C5..004A29E4 (test ah,41h; jne)
    return word_of(b) < word_of(a) ? 1 : -1;                                            // 004A29E5..004A29EF cmp esi,edi; sbb; and 2; dec
}
// 004A4F10 / 004A4FF0 Disable/EnableThisLight(light) (thiscall, RET 4) -> 1 when the light is in the enabled set
U __fastcall light_manager_disable_this_light_004a4f10(void* self, void*, void* light) {
    List copy; copy.size = 0; copy.capacity = 0; copy.data = 0;          // 004A4F2D..004A4F35
    (void)texture_list_assign_0046bc80(&copy, nullptr, list_at(self, field<U>(self, 4) != 0 ? 0x74 : 0x44)); // 004A4F39..004A4F4F
    const I n = copy.size;
    if (count_of(&copy, light) == 0) {                                   // 004A4F5C..004A4F75
        if (copy.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(copy.data)));
        return 0;
    }
    I index = 0;
    while (index < n && element(copy.data, index) != light) ++index;     // 004A4FA2..004A4FAC
    if (index >= n) index = -1;                                          // 004A4FAE
    kotor_renderer_init::glDisable_0073d22c(static_cast<U>(index) + 0x4001u); // 004A4FB1..004A4FB7
    if (copy.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(copy.data)));
    return 1;
}
U __fastcall light_manager_enable_this_light_004a4ff0(void* self, void*, void* light) {
    List copy; copy.size = 0; copy.capacity = 0; copy.data = 0;          // 004A500D..004A5015
    (void)texture_list_assign_0046bc80(&copy, nullptr, list_at(self, field<U>(self, 4) != 0 ? 0x74 : 0x44)); // 004A5019..004A502F
    const I n = copy.size;
    if (count_of(&copy, light) == 0) {                                   // 004A503C..004A5055
        if (copy.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(copy.data)));
        return 0;
    }
    I index = 0;
    while (index < n && element(copy.data, index) != light) ++index;     // 004A5082..004A508C
    if (index >= n) index = -1;
    kotor_renderer_init::glEnable_0073d390(static_cast<U>(index) + 0x4001u); // 004A5091..004A5097
    if (copy.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(copy.data)));
    return 1;
}

// ================================================================ slots
// +00 004A2F30 PrioritizeLights(position, flag) (RET 8)
void __fastcall light_manager_prioritize_lights_004a2f30(void* self, void*, const Vector* position, U flag) {
    Vector zero; zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;                    // 004A2F39..004A2F49
    vt(self)->void_return(self, nullptr);                                         // 004A2F51 slot +58
    List* const lights = list_at(self, 0x2C);
    for (I i = 0; i < lights->size; ++i) {                                         // 004A2F54..004A2FD8 (count re-read)
        void* const light = element(lights->data, i);
        Vector delta;                                                              // 004A2F69..004A2F90
        delta.y = static_cast<float>(static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(position->y));
        delta.x = static_cast<float>(static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(position->x));
        delta.z = 0.0f;                                                            // 004A2F84
        if (aurora_vector_equal_004aa980(at(light, 0x4C), &zero) == 0 && field<float>(light, 0x58) > 0.0f) { // 004A2F94..004A2FAE
            const double weight = vector_magnitude_004ab100(&delta) / static_cast<double>(field<float>(light, 0x58)); // 004A2FB5..004A2FBA
            field<float>(light, 0x80) = static_cast<float>(weight);              // 004A2FC0
        } else {
            field<U>(light, 0x80) = 0x47C35000u;                                   // 004A2FC8 100000.0f
        }
    }
    vt(self)->prioritize_illumination(self, nullptr, flag);                       // 004A2FDC..004A2FE5 slot +4C
}
// +04 004A3080 EnableLights
void __fastcall light_manager_enable_lights_004a3080(void* self, void*) {
    if (*cells.dynamic_lights_007b92dc != 0) return;                              // 004A3080..004A308A
    if (field<I>(self, 0x48) > 0) LC::active_count_008305e8 = field<U>(self, 0x48); // 004A308C..004A3093
    vt(self)->control_lights_fade(self, nullptr, list_at(self, 0x68), list_at(self, 0x80), list_at(self, 0x44), 0u); // 004A3098..004A30AD
    if (head_light() == 1) vt(self)->enable_head_light(self, nullptr);            // 004A30B0..004A30BE
    else aur_disable_head_light_0044da80();                                       // 004A30CA
    field<U>(self, 4) = 0;                                                         // 004A30C1 / 004A30CF
}
// +08 004A4480 EnableLightsDynamicOnly
void __fastcall light_manager_enable_lights_dynamic_only_004a4480(void* self, void*) {
    if (*cells.dynamic_lights_007b92dc != 0) return;                              // 004A449B..004A44A5
    List dynamic; dynamic.data = 0; dynamic.size = 0; dynamic.capacity = 0;      // 004A44AE..004A44B6
    List* const enabled = list_at(self, 0x44);
    for (I i = 0; i < enabled->size; ++i) {                                        // 004A44BA..004A4514
        void* const light = element(enabled->data, i);
        if (field<I>(light, 0x70) == 1) add_allocate(&dynamic, light);             // 004A44D6..004A450A
    }
    LC::dynamic_count_008305ec = static_cast<U>(dynamic.size);                     // 004A4517
    vt(self)->control_lights_fade(self, nullptr, list_at(self, 0x74), list_at(self, 0x8C), &dynamic, 1u); // 004A451C..004A4532
    if (head_light() != 0) vt(self)->enable_head_light(self, nullptr);            // 004A4535..004A4544
    else aur_disable_head_light_0044da80();                                       // 004A4549
    field<U>(self, 4) = 1;                                                         // 004A4554
    if (dynamic.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(dynamic.data))); // 004A454E..004A4566
}
// +0C 004A39F0 AddLight(light) (RET 4)
void __fastcall light_manager_add_light_004a39f0(void* self, void*, void* light) {
    List* const all = list_at(self, 0x08);
    if (count_of(all, light) != 0) return;                                        // 004A39F7..004A3A1D
    Quaternion orientation; orientation.z = 0.0f; orientation.y = 0.0f; orientation.x = 0.0f; orientation.w = 1.0f; // 004A3A33..004A3A4B
    slots_of<PartSlots>(light)->get_transform(static_cast<aurora_model::Part*>(light), nullptr, at(light, 0x94), &orientation); // 004A3A23..004A3A53
    add_allocate(all, light);                                                      // 004A3A56..004A3A7B
    if (field<U>(light, 0x70) == 0) add_allocate(list_at(self, 0x20), light);      // 004A3A7E..004A3AAF
    if (field<U>(light, 0x40) != 0 && (field<U>(light, 0x70) != 0 || field<U>(light, 0x6C) != 0)) // 004A3AB2..004A3AC5
        int_list_add_00444fe0(list_at(self, 0x14), nullptr, word_of(light));       // 004A3AC7..004A3ACB
}
// +10 004A3AE0 RemoveLight(light, flag byte) (RET 8)
void __fastcall light_manager_remove_light_004a3ae0(void* self, void*, void* light, U flag) {
    List* const all = list_at(self, 0x08);
    if (all->size > 0 && count_of(all, light) != 0) {                             // 004A3AE3..004A3B04
        exo_array_list_remove_00445080(all, nullptr, light);                       // 004A3B07
        if (field<U>(light, 0x70) == 0) exo_array_list_remove_00445080(list_at(self, 0x20), nullptr, light); // 004A3B0C..004A3B17
        if (field<U>(light, 0x40) != 0 && (field<U>(light, 0x70) != 0 || field<U>(light, 0x6C) != 0))      // 004A3B1C..004A3B2F
            exo_array_list_remove_00445080(list_at(self, 0x14), nullptr, light);   // 004A3B31..004A3B35
    }
    unique_remove(list_at(self, 0xC4), light);                                     // 004A3B3A..004A3B60
    if (static_cast<U8>(flag) == 0) return;                                        // 004A3B65..004A3B6B (byte)
    static const U kLists[] = {0x44, 0x68, 0x74, 0x80, 0x8C, 0xAC, 0xB8};          // 004A3B71..004A3C90 (in order)
    for (U offset : kLists) unique_remove(list_at(self, offset), light);
}
// +14 004A3CA0 AddRenderLight(light) (RET 4)
void __fastcall light_manager_add_render_light_004a3ca0(void* self, void*, void* light) {
    List* const render = list_at(self, 0x2C);
    if (count_of(render, light) != 0) return;                                     // 004A3CA0..004A3CC1
    add_allocate(render, light);                                                   // 004A3CC3..004A3CE8
}
// +18 004A2E00 Reset: mov dword [ecx+30h],0
void __fastcall light_manager_reset_004a2e00(void* self, void*) { field<U>(self, 0x30) = 0; }
// +1C 004A2E10 UpdateLights: every light part refreshes its world position (+94)
void __fastcall light_manager_update_lights_004a2e10(void* self, void*) {
    Quaternion orientation; orientation.z = 0.0f; orientation.y = 0.0f; orientation.x = 0.0f; orientation.w = 1.0f; // 004A2E1E..004A2E36
    List* const all = list_at(self, 0x08);
    for (I i = 0; i < all->size; ++i) {                                            // 004A2E17..004A2E5D (count re-read)
        void* const light = element(all->data, i);
        slots_of<PartSlots>(light)->get_transform(static_cast<aurora_model::Part*>(light), nullptr, at(light, 0x94), &orientation); // 004A2E43..004A2E54
    }
}
// +20 004A2E70 EnableHighestPriorityLights(count) (RET 4)
void __fastcall light_manager_enable_highest_priority_lights_004a2e70(void* self, void*, U count_word) {
    const I available = field<I>(self, 0x48);                                      // 004A2E76
    if (available == 0) return;                                                    // 004A2E79..004A2E7B
    I count = static_cast<I>(count_word);
    if (!(count < available)) count = available;                                   // 004A2E87..004A2E8B (signed)
    I i = 0;
    for (; i < count; ++i) {                                                       // 004A2E8D..004A2EFE
        kotor_renderer_init::glEnable_0073d390(static_cast<U>(i) + 0x4001u);       // 004A2EA0..004A2EA7
        void* const light = element(field<U>(self, 0x44), i);                      // 004A2EA9..004A2EAC
        set_light(i, light, static_cast<double>(field<float>(light, 0x8C)));      // 004A2EAF..004A2EF3 (intensity via the arg cell)
    }
    for (; i < 3; ++i) kotor_renderer_init::glDisable_0073d22c(static_cast<U>(i) + 0x4001u); // 004A2F01..004A2F1D
}
// +24 004A3F70 EnableHighestPriorityLightsFade(count, fade) (RET 8). fade: +4 CExoArrayList<Light*> L, +10
// CExoArrayList<float> F, +1C shadow light, +20 shadow fade. Cached last state: this+DC (lights) / +E8 (fades).
void __fastcall light_manager_enable_highest_priority_lights_fade_004a3f70(void* self, void*, U count_word, void* fade) {
    const I count = static_cast<I>(count_word);
    List* const lights = list_at(fade, 0x04);                                      // 004A3F94 ebx
    List* const fades = list_at(fade, 0x10);                                       // 004A3F97 ebp
    List& wanted = LC::highest_priority_lights_00831e08;
    if ((LC::highest_priority_guard_00831e14 & 1u) == 0) {                         // 004A3F88..004A3FA7
        LC::highest_priority_guard_00831e14 |= 1u;                                 // 004A3FA9
        wanted.size = 0; wanted.capacity = 0; wanted.data = 0;                     // 004A3FB5..004A3FC1
        (void)std::atexit(free_highest_priority_lights_list_0073be90);             // 004A3FB0..004A3FC7
    }
    wanted.size = 0;                                                               // 004A3FDB
    for (I i = 0; i < count && i < field<I>(self, 0x48); ++i)                      // 004A3FD7..004A403B
        add_allocate(&wanted, element(field<U>(self, 0x44), i));
    for (I j = lights->size; j != 0;) {                                            // 004A403D..004A417B
        void* const light = element(lights->data, j - 1);                          // 004A4050..004A4052
        --j;                                                                       // 004A4059
        aurora_model::MdlNode* const node = aurora_node_as_light_0043e3d0(field<aurora_model::MdlNode*>(light, 4), nullptr); // 004A4056..004A405A
        float* const value = float_at(fades->data, j);
        if (count_of(&wanted, light) != 0) {                                       // 004A405F..004A4089
            if (field<U>(node, 0xA8) != 0) {                                       // 004A408B..004A4097 fading light
                const double v = static_cast<double>(LC::light_fade_rate_007a1940) * dt() + static_cast<double>(*value); // 004A4099..004A40AB
                *value = (1.0 < v) ? f(0x3F800000u) : static_cast<float>(v);       // 004A40AD..004A40C4 (test ah,5; jp)
            } else {
                *value = f(0x3F800000u);                                           // 004A40CB..004A40CE
            }
            continue;
        }
        if (field<U>(node, 0xA8) != 0) {                                           // 004A40DA..004A40E6
            const double v = static_cast<double>(*value) - static_cast<double>(LC::light_fade_rate_007a1940) * dt(); // 004A40E8..004A40FA
            *value = (0.0 > v) ? 0.0f : static_cast<float>(v);                     // 004A40FC..004A4113 (test ah,41h; jne keeps <, ==, NaN)
        } else {
            *value = 0.0f;                                                         // 004A4117..004A411A
        }
        if (*float_at(fades->data, j) > 0.0f) continue;                            // 004A4121..004A4132 (test ah,41h; jp: >0 or NaN keep)
        if (!(*float_at(fades->data, j) <= 0.0f)) continue;
        remove_at(lights, j);                                                      // 004A4134..004A4154
        remove_at(fades, j);                                                       // 004A4156..004A4177
    }
    if (lights->size < count && lights->size < wanted.size && wanted.size > 0) {   // 004A4181..004A41A6
        for (I k = 0; k < wanted.size; ++k) {                                      // 004A41B0..004A4247 (wanted size re-read)
            if (lights->size >= count) break;                                      // 004A41B0..004A41B7
            void* const light = element(wanted.data, k);                           // 004A41BD..004A41C6
            if (count_of(lights, light) != 0) continue;                            // 004A41C9..004A41E0
            add_allocate(lights, light);                                           // 004A41E2..004A4206
            const I capacity = fades->capacity;                                    // 004A4209 CExoArrayList<float>::Add(0)
            if (fades->size == capacity)
                kotor_renderer_gl::allocate_00437d60(fades, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8)); // 004A4211..004A4221
            *float_at(fades->data, fades->size) = 0.0f;                            // 004A4226..004A422C
            fades->size = fades->size + 1;                                         // 004A4233
        }
    }
    // 004A424D..004A4350: shadow light (first enabled light with +5C) cross-fade in fade+1C / fade+20.
    void* shadow = nullptr;
    {
        List* const enabled = list_at(self, 0x44);
        const I n = enabled->size;
        for (I i = 0; i < n; ++i) {                                                // 004A4251..004A426E (count and data read once)
            void* const light = element(enabled->data, i);
            if (field<U>(light, 0x5C) != 0) { shadow = light; break; }             // 004A4260..004A4266
        }
    }
    const double step = static_cast<double>(LC::shadow_fade_rate_007a1944) * dt();
    float& shadow_fade = field<float>(fade, 0x20);
    if (shadow == nullptr) {
        if (field<U>(fade, 0x1C) != 0) {                                           // 004A4270..004A4279
            double v = static_cast<double>(shadow_fade) - step;                    // 004A427F..004A428B
            if (0.0 > v) v = 0.0;                                                  // 004A428E..004A429F
            shadow_fade = static_cast<float>(v);                                   // 004A42A5 fst
            if (v < 0.0 || v == 0.0) field<U>(fade, 0x1C) = 0;                      // 004A42A8..004A42B9 (test ah,41h; jp: >0 or NaN keep)
        }
    } else if (field<void*>(fade, 0x1C) == nullptr) {                             // 004A42C5..004A42CE
        field<void*>(fade, 0x1C) = shadow;                                         // 004A42D0
        const double v = step + static_cast<double>(shadow_fade);                  // 004A42D3..004A42DF
        shadow_fade = (1.0 < v) ? f(0x3F800000u) : static_cast<float>(v);          // 004A4334..004A434B
    } else if (field<void*>(fade, 0x1C) == shadow) {                               // 004A42E4
        const double v = static_cast<double>(shadow_fade) + step;                  // 004A42E6..004A42F4
        shadow_fade = (1.0 < v) ? f(0x3F800000u) : static_cast<float>(v);
    } else {
        double v = static_cast<double>(shadow_fade) - step;                        // 004A42F9
        if (0.0 > v) v = 0.0;                                                      // 004A42FC..004A430D
        shadow_fade = static_cast<float>(v);                                       // 004A4319 fst
        if (v < 0.0 || v == 0.0) {                                                 // 004A4313..004A4321 (>0 or NaN keep)
            field<void*>(fade, 0x1C) = shadow;                                     // 004A4323
            const double w = v + step;                                             // 004A4326..004A4332 faddp (unrounded v)
            shadow_fade = (1.0 < w) ? f(0x3F800000u) : static_cast<float>(w);      // 004A4334..004A434B
        }
    }
    // 004A4352..004A43A1: unchanged since the last call (this+DC lights / +E8 fades) -> only copy into this+44.
    List* const last_lights = list_at(self, 0xDC);
    List* const last_fades = list_at(self, 0xE8);
    bool same = lights->size == last_lights->size;                                 // 004A4352..004A435B
    for (I c = 0; same && c < lights->size; ++c) {                                 // 004A435D..004A439B
        if (element(lights->data, c) != element(last_lights->data, c)) same = false;               // 004A4370..004A4379
        else if (!(*float_at(fades->data, c) == *float_at(last_fades->data, c))) same = false;     // 004A437B..004A4393 (test ah,44h; jp)
    }
    if (!same) {
        (void)texture_list_assign_0046bc80(last_lights, nullptr, lights);         // 004A43A9..004A43B4
        (void)float_list_assign_00438a50(last_fades, nullptr, fades);             // 004A43B9..004A43C0
        kotor_vp_cells::refresh_007a193c = 1;                                      // 004A43C5
        I i = 0;
        for (; i < lights->size; ++i) {                                            // 004A43CF..004A4432 (count re-read)
            void* const light = element(lights->data, i);
            field<U>(light, 0x78) = *reinterpret_cast<const U*>(float_at(fades->data, i)); // 004A43DD..004A43EA (raw DWORD)
            kotor_renderer_init::glEnable_0073d390(static_cast<U>(i) + 0x4001u);   // 004A43E3..004A43ED
            set_light(i, light, static_cast<double>(field<float>(light, 0x78)));  // 004A43F3..004A4424
        }
        for (i = lights->size; i < 3; ++i) kotor_renderer_init::glDisable_0073d22c(static_cast<U>(i) + 0x4001u); // 004A4434..004A444F
    }
    (void)texture_list_assign_0046bc80(list_at(self, 0x44), nullptr, lights);      // 004A4451..004A4459 / 004A43A1
}
// +28 0063CFC0 ret 10h / +48 005B5E90 ret 8 / +58 004015A0 ret (typed instances, D24)
void __fastcall light_manager_void_return_16_0063cfc0(void* self, void*, U, U, U, U) { (void)self; }
// [M45 compose] 005B5E90 (ret 8) body removed: D68 earliest-staged owner lane-gui-load (M44) (definition removed; declaration kept)
void __fastcall light_manager_void_return_8_005b5e90(void* self, void*, U, U);
void __fastcall light_manager_void_return_004015a0(void* self, void*) { (void)self; }
// +2C 004A5680 DisableBumpMapLights / +30 004A5710 EnableBumpMapLights
void __fastcall light_manager_disable_bump_map_lights_004a5680(void* self, void*) {
    List copy;
    (void)int_list_copy_construct_0046d250(&copy, nullptr, list_at(self, 0x5C));  // 004A569E..004A56A6
    const I n = copy.size; const U data = copy.data;                               // 004A56AB..004A56AF
    for (I i = 0; i < n; ++i) {                                                    // 004A56B3..004A56DF
        (void)light_manager_disable_this_light_004a4f10(self, nullptr, element(data, i)); // 004A56C0..004A56C6
        field<U8>(self, 0xF4) = 1;                                                 // 004A56CE
        kotor_vp_cells::refresh_007a193c = 1;                                      // 004A56D5
    }
    if (data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data))); // 004A56E1..004A56EE
}
void __fastcall light_manager_enable_bump_map_lights_004a5710(void* self, void*) {
    List copy;
    (void)int_list_copy_construct_0046d250(&copy, nullptr, list_at(self, 0x5C));  // 004A572E..004A5736
    const I n = copy.size; const U data = copy.data;
    for (I i = 0; i < n; ++i) {                                                    // 004A5743..004A5768
        (void)light_manager_enable_this_light_004a4ff0(self, nullptr, element(data, i)); // 004A5750..004A5756
        kotor_vp_cells::refresh_007a193c = 1;                                      // 004A575E
    }
    field<U8>(self, 0xF4) = 0;                                                     // 004A576C
    if (data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data))); // 004A576A..004A577E
}
// +34 0046DE10 CollectLightsForGivenRoom(room) (RET 4): this+38 = coloured lights of the room (or of every room of the
// active scene when room == 0) and its neighbours (room+5C); this+C4 = lens-flare lights (node+7C > 0).
void __fastcall light_manager_collect_lights_for_given_room_0046de10(void* self, void*, void* room) {
    List* const lights = list_at(self, 0x38);
    List* const flares = list_at(self, 0xC4);
    field<U>(self, 0x3C) = 0;                                                      // 0046DE2F
    field<U>(self, 0xC8) = 0;                                                      // 0046DE32
    List rooms; rooms.size = 0; rooms.capacity = 0; rooms.data = 0;               // 0046DE38..0046DE40
    if (room != nullptr) {                                                         // 0046DE48..0046DE52
        (void)texture_list_assign_0046bc80(&rooms, nullptr, list_at(room, 0x5C));  // 0046DE54..0046DE58
        add_allocate(&rooms, room);                                                // 0046DE5D..0046DE8A
    } else {
        (void)texture_list_assign_0046bc80(&rooms, nullptr, list_at(*cells.active_scene_007b92a0, 0xC8)); // 0046DE90..0046DE9D
    }
    auto colour_nonzero = [](void* light) {                                        // test ah,44h; jp / jnp (NaN counts as non-zero)
        return !(field<float>(light, 0x4C) == 0.0f) || !(field<float>(light, 0x50) == 0.0f) || !(field<float>(light, 0x54) == 0.0f);
    };
    for (I r = 0; r < rooms.size; ++r) {                                           // 0046DEA2..0046E203
        List* const room_lights = static_cast<List*>(field<void*>(element(rooms.data, r), 0x58)); // 0046DEB5..0046DEBC
        for (I j = 0; j < room_lights->size; ++j) {                                // 0046DEBF..0046E02C (count re-read)
            void* const light = element(room_lights->data, j);                     // 0046DED0..0046DED6
            if (field<I>(field<void*>(light, 4), 0x7C) > 0 && count_of(flares, light) == 0) add_new(flares, light); // 0046DED9..0046DF87
            if (colour_nonzero(light)) add_new(lights, light);                     // 0046DF8D..0046E01B (no uniqueness test)
        }
        List extra;                                                                // 0046E034..0046E03F copy of room_lights+80
        (void)int_list_copy_construct_0046d250(&extra, nullptr, static_cast<List*>(at(room_lights, 0x80)));
        for (I k = 0; k < extra.size; ++k) {                                       // 0046E044..0046E1D6
            void* const light = element(extra.data, k);
            if (field<I>(field<void*>(light, 4), 0x7C) > 0 && count_of(flares, light) == 0) add_new(flares, light); // 0046E067..0046E117
            if (colour_nonzero(light) && count_of(lights, light) == 0) add_new(lights, light); // 0046E11D..0046E1CC
        }
        if (extra.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(extra.data))); // 0046E1DE..0046E1EC
    }
    if (rooms.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(rooms.data))); // 0046E20A..0046E21E
}
// +38 004A2DB0 SetupLightsForThisGob(position, radius) (RET 8): slot +44 over this+38, then this+4 = 0
void __fastcall light_manager_setup_lights_for_this_gob_004a2db0(void* self, void*, const Vector* position, U radius_bits) {
    vt(self)->setup_lights_for_position_in(self, nullptr, position, radius_bits, list_at(self, 0x38)); // 004A2DB7..004A2DC5
    field<U>(self, 4) = 0;                                                         // 004A2DC8
}
// +3C 004A2DE0 SetupLightsForThisRoom(position, radius) (RET 8): tail of slot +44 over this+38
void __fastcall light_manager_setup_lights_for_this_room_004a2de0(void* self, void*, const Vector* position, U radius_bits) {
    vt(self)->setup_lights_for_position_in(self, nullptr, position, radius_bits, list_at(self, 0x38)); // 004A2DE0..004A2DF0
}
// +40 004A36B0 SetupLightsForThisPositionAndRadius(position, radius) over every light (this+8) (RET 8)
void __fastcall light_manager_setup_lights_for_position_004a36b0(void* self, void*, const Vector* position, U radius_bits) {
    field<U>(self, 0x30) = 0;                                                      // 004A36BB
    const double radius = static_cast<double>(f(radius_bits));
    List* const all = list_at(self, 0x08);
    for (I i = 0; i < all->size; ++i) {                                            // 004A36B3..004A3759 (count re-read)
        void* const light = element(all->data, i);
        const float reach = field<float>(light, 0x58);
        if (reach < 0.0f || reach == 0.0f) continue;                               // 004A36D6..004A36E4 (test ah,41h; jnp)
        if (field<U>(light, 0x6C) == 0 && field<U>(light, 0x70) == 0) continue;    // 004A36E6..004A36F2
        double dx = static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(position->x); // 004A36F4..004A36FA
        if (dx < 0.0) dx = -dx;                                                    // 004A36FC..004A3709
        const float gap_x = static_cast<float>(dx - static_cast<double>(reach));  // 004A370B..004A370E fstp
        double dy = static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(position->y); // 004A3712..004A3718
        if (dy < 0.0) dy = -dy;                                                    // 004A371B..004A3728
        if (!(static_cast<double>(gap_x) < radius)) continue;                      // 004A372A..004A3737 (test ah,5; jp)
        if (!(dy - static_cast<double>(reach) < radius)) continue;                 // 004A3739..004A3745
        vt(self)->add_render_light(self, nullptr, light);                          // 004A3747..004A374C slot +14
    }
}
// +44 004A35B0 SetupLightsForThisPositionAndRadius(position, radius, lights) (RET 0Ch): coloured lights within 2*radius
void __fastcall light_manager_setup_lights_for_position_in_004a35b0(void* self, void*, const Vector* position, U radius_bits, List* lights) {
    field<U>(self, 0x30) = 0;                                                      // 004A35BB
    const double twice = static_cast<double>(f(radius_bits)) + static_cast<double>(f(radius_bits)); // 004A3662..004A3666 fadd st,st
    for (I i = 0; i < lights->size; ++i) {                                         // 004A35BE..004A3699 (count re-read)
        void* const light = element(lights->data, i);
        const float reach = field<float>(light, 0x58);
        if (reach < 0.0f || reach == 0.0f) continue;                               // 004A35D5..004A35E3
        if (field<U>(light, 0x6C) == 0 && field<U>(light, 0x70) == 0) continue;    // 004A35E9..004A35F5
        if (field<float>(light, 0x4C) == 0.0f && field<float>(light, 0x50) == 0.0f && field<float>(light, 0x54) == 0.0f) continue; // 004A35FB..004A3629
        double dx = static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(position->x);
        if (dx < 0.0) dx = -dx;                                                    // 004A3634..004A3641
        const float gap_x = static_cast<float>(dx - static_cast<double>(reach));  // 004A3643..004A3646
        double dy = static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(position->y);
        if (dy < 0.0) dy = -dy;                                                    // 004A3653..004A3660
        if (!(static_cast<double>(gap_x) < twice)) continue;                       // 004A3668..004A3673
        if (!(dy - static_cast<double>(reach) < twice)) continue;                  // 004A3675..004A3683
        vt(self)->add_render_light(self, nullptr, light);                          // 004A3685..004A368A
    }
}
// +4C 004A3CF0 PrioritizeIllumination(force byte) (RET 4)
void __fastcall light_manager_prioritize_illumination_004a3cf0(void* self, void*, U force) {
    List* const render = list_at(self, 0x2C);
    List* const enabled = list_at(self, 0x44);
    enabled->size = 0;                                                             // 004A3CFD
    Vector zero; zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;                      // 004A3D00..004A3D08
    for (I i = 0; i < render->size; ++i) {                                         // 004A3CF9..004A3D98 (count re-read)
        if (static_cast<U8>(force) == 0 && field<float>(element(render->data, i), 0x80) > 1.0f) continue; // 004A3D13..004A3D32
        void* const light = element(render->data, i);
        if (field<float>(light, 0x58) == 0.0f) continue;                           // 004A3D3A..004A3D48
        if (aurora_vector_equal_004aa980(at(light, 0x4C), &zero) != 0) continue;   // 004A3D4A..004A3D5D
        add_allocate(enabled, element(render->data, i));                           // 004A3D5F..004A3D8F
    }
    qsort_006fbe00(reinterpret_cast<void*>(static_cast<std::uintptr_t>(enabled->data)), static_cast<U>(enabled->size), 4u, light_compare_004a28c0); // 004A3D9F..004A3DAE
    List* const bump = list_at(self, 0x5C);
    bump->size = 0;                                                                // 004A3DBD
    for (I i = 0; i < enabled->size && i < 3; ++i) {                               // 004A3DB3..004A3DE5
        void* const light = element(enabled->data, i);
        if (field<U>(light, 0x68) != 0 || field<U>(light, 0x70) == 0) continue;    // 004A3DD1..004A3DDD
        add_allocate(bump, light);                                                 // 004A3DF0..004A3E1A
        return;
    }
}
// +50 004A3E30 PrioritizeShadow
void __fastcall light_manager_prioritize_shadow_004a3e30(void* self, void*) {
    List* const render = list_at(self, 0x2C);
    List* const shadow = list_at(self, 0x50);
    shadow->size = 0;                                                              // 004A3E3B
    const float threshold = f(0x3DCCCCCDu);                                        // [0073E798] 0.1f
    for (I i = 0; i < render->size; ++i) {                                         // 004A3E33..004A3EB9
        void* const light = element(render->data, i);
        if (field<U>(light, 0x5C) == 0 || field<U>(light, 0x68) != 0) continue;    // 004A3E48..004A3E54
        if (field<float>(light, 0x4C) > threshold || field<float>(light, 0x50) > threshold ||
            field<float>(light, 0x54) > threshold)                                 // 004A3E56..004A3E84
            add_allocate(shadow, light);                                           // 004A3E86..004A3EB0
    }
    qsort_006fbe00(reinterpret_cast<void*>(static_cast<std::uintptr_t>(shadow->data)), static_cast<U>(shadow->size), 4u, light_compare_004a28c0); // 004A3EBD..004A3ECC
}
// +54 004A3EE0 PrioritizeBump
void __fastcall light_manager_prioritize_bump_004a3ee0(void* self, void*) {
    List* const render = list_at(self, 0x2C);
    List* const bump = list_at(self, 0x5C);
    bump->size = 0;                                                                // 004A3EEB
    for (I i = 0; i < render->size; ++i) {                                         // 004A3EE4..004A3F39
        void* const light = element(render->data, i);
        if (field<U>(light, 0x68) == 0 && field<U>(light, 0x70) != 0) add_allocate(bump, light); // 004A3EF8..004A3F30
    }
    qsort_006fbe00(reinterpret_cast<void*>(static_cast<std::uintptr_t>(bump->data)), static_cast<U>(bump->size), 4u, light_compare_004a28c0); // 004A3F3D..004A3F4C
    if (bump->size > 1) bump->size = 1;                                            // 004A3F51..004A3F60
}
// +5C 004A4580 ControlLightsFade(active, previous, desired, dynamic_only) (RET 10h)
void __fastcall light_manager_control_lights_fade_004a4580(void* self, void*, List* active, List* previous, List* desired, U dynamic_only) {
    if (LC::fade_gate_008305f4 != 0) return;                                       // 004A4583..004A458E
    const I max_lights = LC::max_lights_007a1934;                                  // 004A459D
    I limit = max_lights;                                                          // [esp+10h]
    const U blind = *cells.black_fog_008305e0;                                     // 004A4594 (Scene::SetBlindLights cell)
    if (blind != 0) {                                                              // 004A459A..004A45AE
        I lead = 0;
        const I n = desired->size; const U data = desired->data;                   // 004A45B0..004A45BB
        for (I k = 0; k < n; ++k) { if (field<U>(element(data, k), 0x40) != blind) break; ++lead; } // 004A45C0..004A45CE
        if (!(max_lights < lead)) limit = lead;                                    // 004A45D0..004A45D4
    }
    // phase 1 (004A45D8..004A4779): fade the active lights in/out, dropping the fully faded ones
    for (I j = active->size; j != 0;) {
        void* const light = element(active->data, j - 1);                          // 004A45F0..004A45F2
        --j;
        float* const value = static_cast<float*>(at(light, dynamic_only != 0 ? 0x7C : 0x78)); // 004A45F6..004A4606
        if (count_of(desired, light) != 0) {                                       // 004A4609..004A4648
            I index = 0; const I n = desired->size;
            while (index < n && element(desired->data, index) != light) ++index;   // 004A464E..004A465A
            if (index >= n) index = -1;
            if (!(index < limit)) {                                                // 004A465F..004A4667
                const double v = static_cast<double>(*value) - dt();               // 004A4669..004A466F
                *value = static_cast<float>(v);                                    // 004A4671 fst
                if (v < 0.0) { *value = 0.0f; remove_at(active, j); }              // 004A4673..004A46B3
            } else if (!(*value >= 1.0f) && !(*value != *value)) {                 // 004A46B8..004A46C5 (test ah,5; jp)
                const double v = dt() + static_cast<double>(*value);               // 004A46CB..004A46D1
                *value = (1.0 < v) ? f(0x3F800000u) : static_cast<float>(v);       // 004A46D3..004A46EA
            }
            continue;
        }
        if (count_of(list_at(self, 0x08), light) != 0) {                           // 004A46F5..004A4712
            const double v = static_cast<double>(*value) - dt();                   // 004A4714..004A471A
            *value = static_cast<float>(v);                                        // 004A471C fst
            if (v < 0.0) { *value = 0.0f; remove_at(active, j); }                  // 004A471E..004A4753
        } else {
            remove_at(active, j);                                                  // 004A4755..004A4775
        }
    }
    // phase 2 (004A477F..004A47D1): new non-fading desired lights within the limit
    I fresh = 0;
    for (I i = 0; i < limit && i < desired->size; ++i) {
        void* const light = element(desired->data, i);                             // 004A4796..004A4799
        if (field<U>(field<void*>(light, 4), 0xA8) != 0) continue;                 // 004A479C..004A47A7
        if (count_of(active, light) == 0) ++fresh;                                 // 004A47A9..004A47C6
    }
    // phase 3 (004A47D3..004A48A3): make room by dropping active lights that rank beyond the limit
    I room = limit - active->size;                                                 // 004A47D3..004A47E2
    while (fresh > room) {
        const I last = active->size - 1;                                           // 004A47F0..004A47FA
        if (last < 0) goto phase4;
        const U base = active->data;                                               // 004A4808
        const I n = desired->size;                                                 // 004A480A
        I k = last;
        bool removed = false;
        for (; k >= 0; --k) {                                                      // 004A4814..004A4873
            void* const light = *reinterpret_cast<void* const volatile*>(static_cast<std::uintptr_t>(base) + static_cast<U>(k) * 4u);
            I index;
            if (count_of(desired, light) != 0) {                                   // 004A481A..004A483D
                index = 0;
                while (index < n && element(desired->data, index) != light) ++index; // 004A483F..004A484B
                if (index >= n) index = -1;
            } else {
                index = limit + 1;                                                 // 004A4852..004A4856
            }
            if (index > limit) {                                                   // 004A4857..004A485F
                active->size = last;                                               // 004A4877..004A487D
                for (I m = k; m < active->size; ++m) set_element(active->data, m, element(active->data, m + 1)); // 004A4880..004A4892
                removed = true;
                break;
            }
        }
        if (!removed) goto phase4;                                                 // 004A4875
        ++room;                                                                    // 004A4894..004A48A3
    }
phase4:
    // phase 4 (004A48A9..004A49D8): admit desired lights while there is room; every admitted/kept light fades in
    for (I i = 0; i < limit; ++i) {
        if (i >= desired->size) break;                                             // 004A48C1..004A48C8
        const I size = active->size;                                               // 004A48CE
        if (size >= limit) break;                                                  // 004A48D1..004A48D5
        void* const light = element(desired->data, i);                             // 004A48DB..004A48E1
        float* const value = static_cast<float*>(at(light, dynamic_only != 0 ? 0x7C : 0x78)); // 004A48E4..004A48EF
        if (count_of(active, light) == 0) {                                        // 004A48F2..004A490D
            add_new(active, light);                                                // 004A4913..004A496C
            if (field<U>(field<void*>(light, 4), 0xA8) == 0) {                     // 004A496F..004A497A
                *value = f(0x3F800000u);                                           // 004A497C
            } else {
                (void)count_of(previous, light);                                   // 004A4985..004A499D (result unused)
                *value = 0.0f;                                                     // 004A499F
            }
        }
        const double v = dt() + static_cast<double>(*value);                       // 004A49A6..004A49AC
        *value = (1.0 < v) ? f(0x3F800000u) : static_cast<float>(v);               // 004A49AF..004A49CA
    }
    // phase 5 (004A49DE..004A4AF8): sort, publish counts, program GL lights, previous = desired
    qsort_006fbe00(reinterpret_cast<void*>(static_cast<std::uintptr_t>(active->data)), static_cast<U>(active->size), 4u, light_compare_004a28c0); // 004A49DE..004A49EC
    LC::enabled_count_008305e4 = static_cast<U>(limit);                            // 004A49FF
    if (!(limit < active->size)) LC::enabled_count_008305e4 = static_cast<U>(active->size); // 004A49FD..004A4A07
    LC::negative_light_008305f0 = 0;                                               // 004A4A0C
    I i = 0;
    for (; i < active->size; ++i) {                                                // 004A4A12..004A4AC5 (count re-read)
        void* const light = element(active->data, i);
        const double fade = static_cast<double>(*static_cast<const float*>(at(light, dynamic_only != 0 ? 0x7C : 0x78))); // 004A4A25..004A4A3B
        const double intensity = static_cast<double>(field<float>(light, 0x8C));   // 004A4A3D..004A4A45 (via the arg cell)
        const double red = fade * static_cast<double>(field<float>(light, 0x4C));  // 004A4A49 (kept on the x87 stack)
        const float green = static_cast<float>(fade * static_cast<double>(field<float>(light, 0x50))); // 004A4A4C..004A4A51
        const float blue = static_cast<float>(fade * static_cast<double>(field<float>(light, 0x54)));  // 004A4A55..004A4A5A
        const double lit = red * intensity;                                        // 004A4A5E
        Vector colour;
        colour.x = static_cast<float>(lit);                                        // 004A4A62 fst
        colour.y = static_cast<float>(static_cast<double>(green) * intensity);     // 004A4A66..004A4A6E
        colour.z = static_cast<float>(static_cast<double>(blue) * intensity);      // 004A4A72..004A4A7A
        if (lit < 0.0) LC::negative_light_008305f0 = 1;                            // 004A4A7E..004A4A8B (test ah,5; jp)
        kotor_renderer_init::glEnable_0073d390(static_cast<U>(i) + 0x4001u);       // 004A4A95..004A4A9C
        void* const again = element(active->data, i);                             // 004A4A9E..004A4AA0
        set_light_gl_004a2a00(i, at(again, 0x94), &colour, static_cast<const float*>(at(again, 0x58)), static_cast<const U*>(at(again, 0x68))); // 004A4AA3..004A4AB7
    }
    for (; i < 3; ++i) kotor_renderer_init::glDisable_0073d22c(static_cast<U>(i) + 0x4001u); // 004A4ACB..004A4AED
    (void)texture_list_assign_0046bc80(previous, nullptr, desired);                // 004A4AEF..004A4AF8
}
// +60 004A2FF0 EnableHeadLight: GL_LIGHT0 ambient/diffuse from 007A1958/007A194C, position = current camera +7C,
// then tail jump AurEnableHeadLight 0044DA60.
void __fastcall light_manager_enable_head_light_004a2ff0(void* self, void*) {
    (void)self;
    using namespace kotor_renderer_init;
    float w = 1.0f;                                                                // 004A2FFC
    glLightfv_0073d388(0x4000u, 0x1200u, homogenize_004aaab0(&kotor_vp_cells::headlight_b_007a1958, &w)); // 004A2FF2..004A301F ([0073F320])
    w = 1.0f;                                                                      // 004A302B
    glLightfv_0073d388(0x4000u, 0x1201u, homogenize_004aaab0(&kotor_vp_cells::headlight_a_007a194c, &w)); // 004A3021..004A3047 ([0073F324])
    void* const camera = *cells.current_camera_007bb4e8;                          // 004A3049
    w = 1.0f;                                                                      // 004A3058
    glLightfv_0073d388(0x4000u, 0x1203u, homogenize_004aaab0(static_cast<const Vector*>(at(camera, 0x7C)), &w)); // 004A304F..004A3074 ([0073F32C])
    aur_enable_head_light_0044da60();                                              // 004A307A jmp 0044DA60
}
} // extern "C"

// ================================================================ published typed table 00743958
namespace kotor_scene3d_construct {
const LightManagerVTable light_manager_vtable_00743958 = {
    light_manager_prioritize_lights_004a2f30,                // +00 004A2F30
    light_manager_enable_lights_004a3080,                    // +04 004A3080
    light_manager_enable_lights_dynamic_only_004a4480,       // +08 004A4480
    light_manager_add_light_004a39f0,                        // +0C 004A39F0
    light_manager_remove_light_004a3ae0,                     // +10 004A3AE0
    light_manager_add_render_light_004a3ca0,                 // +14 004A3CA0
    light_manager_reset_004a2e00,                            // +18 004A2E00
    light_manager_update_lights_004a2e10,                    // +1C 004A2E10
    light_manager_enable_highest_priority_lights_004a2e70,   // +20 004A2E70
    light_manager_enable_highest_priority_lights_fade_004a3f70, // +24 004A3F70
    light_manager_void_return_16_0063cfc0,                   // +28 0063CFC0
    light_manager_disable_bump_map_lights_004a5680,          // +2C 004A5680
    light_manager_enable_bump_map_lights_004a5710,           // +30 004A5710
    light_manager_collect_lights_for_given_room_0046de10,    // +34 0046DE10
    light_manager_setup_lights_for_this_gob_004a2db0,        // +38 004A2DB0
    light_manager_setup_lights_for_this_room_004a2de0,       // +3C 004A2DE0
    light_manager_setup_lights_for_position_004a36b0,        // +40 004A36B0
    light_manager_setup_lights_for_position_in_004a35b0,     // +44 004A35B0
    light_manager_void_return_8_005b5e90,                    // +48 005B5E90
    light_manager_prioritize_illumination_004a3cf0,          // +4C 004A3CF0
    light_manager_prioritize_shadow_004a3e30,                // +50 004A3E30
    light_manager_prioritize_bump_004a3ee0,                  // +54 004A3EE0
    light_manager_void_return_004015a0,                      // +58 004015A0
    light_manager_control_lights_fade_004a4580,              // +5C 004A4580
    light_manager_enable_head_light_004a2ff0,                // +60 004A2FF0
};
}
