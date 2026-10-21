// lane-3d-construct unit construct3d: scene-construction and light bodies that use the canonical scene3d.h views.
// Evidence: dumpbin /disasm swkotor.exe (GOG K1); lane-local listings in d/<ADDR>.txt; addresses cited per behaviour.
// Float policy (D30-FINAL): the original runs x87 with PC_53 / round-to-nearest (CRT startup 007092C0), so every x87
// intermediate is a double here and every `fstp dword` is a float rounding at the same point.
// GL entries go through the single adapter owner (lane-renderer-bodies renderer_gl_imports_gen.h /
// M41 renderer_initialize.h): the original `call dword ptr [IAT]` sites 0073D388 glLightfv, 0073D390 glEnable,
// 0073D22C glDisable.
#include "36a720fede-scene3d.h"                    // canonical 3D header (proposed copy: source/proposed, diffs/scene3d.h.diff)
#include "45d124af80-font.hpp"   // assign_00447fa0 (lane-3d-model owner)
#include "96018b9c1f-renderer_gl_imports_gen.h"    // kotor_renderer_init::glLightfv_0073d388 (+ glEnable/glDisable via renderer_initialize.h)
#include "04a081a8bc-soft_shadow.h"                // kotor_soft_shadow::SoftAvailable_0045f7b0 / ATISoftAvailable_0045f7e0 / bindings (0078DAE9, 0078E420)
#include "289d120a9a-construct3d.h"
#include <cstdint>
#include <cmath>

extern "C" {
// _stricmp 0070ACAF: same spelling as lane-3d-model model_load.hpp / lane-3d-room (one provider, EXCLUDED CRT area D35)
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// lane-3d-model model_instance.hpp: Gob list 007A8A58 (D39 owner) and FindGobIdx 00440410
extern kotor_reconstruction::ArrayMutations aurora_gobs_007a8a58;
std::int32_t __cdecl aurora_find_gob_index_00440410(const char* name);
// lane-startup-misc texture_memory.cpp: CExoArrayList::operator= 0046BC80 (unused here; LightNodeTile is in light_nodes.cpp)
}
namespace texture_join {
// lane-texture-epoch texture_join.h (identical declaration; owner texture-cleanup/slots.cpp): CExoArrayList::AddUnique 0046BD10
void __fastcall add_unique_0046bd10(kotor_reconstruction::ArrayMutations*, void*, font::Texture*);
}

namespace {
using namespace kotor_scene3d;
namespace C3 = kotor_scene3d_construct;
using C3::List;
using C3::BehaviorSlots;
using C3::PartSlots;
using C3::RenderTargetSlots;
using C3::slots_of;

inline const aurora_model::GobVTable* gob_vt(void* gob) {
    return *static_cast<const aurora_model::GobVTable* const volatile*>(gob);      // mov reg,[gob] (reloaded per call)
}
inline aurora_model::Gob* as_gob(void* p) { return static_cast<aurora_model::Gob*>(p); }
inline List* list_at(void* object, U offset) { return static_cast<List*>(at(object, offset)); }
// element i of a CExoArrayList<T*> (data held as a 32-bit address in the task-14 list type)
inline void* element(U data, I index) {
    return *reinterpret_cast<void* const volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u);
}
inline void set_element(U data, I index, void* value) {
    *reinterpret_cast<void* volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u) = value;
}
inline U address_of(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
inline float f(U bits) { return bits_to_float(bits); }

// CExoArrayList<T*>::Add with the 005E03C0 growth (Allocate(cap ? cap*2 : 8)) - the inlined form at 0044733F..00447364,
// 0049F171..0049F19B, 0049F1A2..0049F1DD, 0044720A..00447233, 0046CC6D..0046CCA7: capacity read first, then
// `cmp size,capacity`, then data/size reloaded for the store and `inc size`.
inline void list_add_allocate(List* list, void* value) {
    const I capacity = list->capacity;
    if (list->size == capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    const I size = list->size;
    set_element(list->data, size, value);
    list->size = list->size + 1;
}
} // namespace

extern "C" {
// ================================================================ construction
// 0043E1B0 operator_new: `jmp 006FA7E6` (tail jump, same stack argument) -> M41 live_operator_new.
void* __cdecl operator_new_0043e1b0(U size) { return live_operator_new(size); }

// 00440460 FindGob(name) -> Gob* (cdecl)
void* __cdecl find_gob_00440460(const char* name) {
    const I index = aurora_find_gob_index_00440410(name);             // 00440466..00440467
    if (index < aurora_gobs_007a8a58.size) {                           // 0044046E..00440478 cmp esi,[007A8A5C]; jge (signed)
        void* const gob = element(aurora_gobs_007a8a58.data, index);   // 0044047A..0044047F
        if (kotor_stricmp_0070acaf(static_cast<const char*>(at(gob, 0x38)), name) == 0) // 00440482..00440491 (gob+38, name)
            return element(aurora_gobs_007a8a58.data, index);          // 00440493..00440499 (data re-read)
    }
    return nullptr;                                                    // 0044049F..004404A3
}

// 00447310 Gob::AddAttachment(attachment) (thiscall, RET 4): unique add to the CExoArrayList at Gob+1BC.
void __fastcall gob_add_attachment_00447310(void* gob, void*, void* attachment) {
    List* const list = list_at(gob, 0x1BC);                           // 00447317 lea esi,[ecx+1BCh]
    I remaining = list->size;                                          // 00447310 mov eax,[ecx+1C0h]
    I found = 0;                                                       // 0044731D xor edx,edx
    if (remaining > 0) {                                               // 0044731F test; jle 0044733F
        U cursor = list->data;                                         // 00447328 mov ecx,[esi] (read once)
        do {
            if (*reinterpret_cast<void* const volatile*>(static_cast<std::uintptr_t>(cursor)) == attachment) ++found; // 00447330..00447334
            cursor += 4;                                               // 00447335
        } while (--remaining != 0);                                    // 00447338..00447339
        if (found != 0) return;                                        // 0044733B..0044733D
    }
    list_add_allocate(list, attachment);                               // 0044733F..00447364
}

// 0049F130 CAurBehavior::AddDependency(dependency) (thiscall, RET 4). EH handler 00717D8B / FuncInfo 00764F7C
// frees the 4-byte cell only during SafePointer construction; state -1 precedes the behavior-list growth.
void __fastcall behavior_add_dependency_0049f130(void* behavior, void*, void* dependency) {
    void* cell = live_operator_new(4);                                 // 0049F149..0049F14D operator new(4)
    if (cell != nullptr) {                                             // 0049F15B..0049F165
        try {
            *static_cast<void* volatile*>(cell) = dependency;          // 0049F16D mov [edi],eax (SafePointer target)
            if (dependency != nullptr)                                 // 0049F16B..0049F16F
                list_add_allocate(list_at(dependency, 4), cell);       // 0049F171..0049F19B dependency+4 registrations
        } catch (...) {
            live_free(cell);                                          // 00717D80..00717D8A unwind state 0
            throw;
        }
    } else {
        cell = nullptr;                                                // 0049F1A0 xor edi,edi
    }
    list_add_allocate(list_at(behavior, 4), cell);                     // 0049F1A2..0049F1DD this+4 (state -1, no cleanup)
}

// 0049FB10 CAurBehaviorAttach::CAurBehaviorAttach(gob, object, part) (thiscall, RET 0Ch, EAX = this). Size 3Ch
// (allocated by 00443820). EH FuncInfo 00717DF3 (states 0/1: dependency list / SafePointer; not emitted).
void* __fastcall behavior_attach_0049fb10(void* self, void*, void* gob, void* object, void* part) {
    field<U>(self, 0x08) = 0;                                          // 0049FB31 CExoArrayList ctor (dependencies +4)
    field<U>(self, 0x0C) = 0;                                          // 0049FB34
    field<U>(self, 0x04) = 0;                                          // 0049FB37
    field<void*>(self, 0x10) = gob;                                    // 0049FB3F
    field<const BehaviorSlots*>(self, 0) = &C3::behavior_attach_vtable_00743894; // 0049FB45
    field<U>(self, 0x14) = 0;                                          // 0049FB4F SafePointer ctor (+14)
    behavior_add_dependency_0049f130(self, nullptr, object);           // 0049FB51..0049FB5B
    font::Safe* const target_cell = static_cast<font::Safe*>(at(self, 0x14));
    (void)font::assign_00447fa0(target_cell, nullptr, static_cast<font::Texture*>(object)); // 0049FB60..0049FB63
    field<void*>(self, 0x18) = part;                                   // 0049FB68..0049FB6F
    void* const attached = field<void*>(self, 0x10);                   // 0049FB6C mov ebx,[esi+10h]
    field<U>(self, 0x24) = 0; field<U>(self, 0x28) = 0; field<U>(self, 0x2C) = 0; // 0049FB75..0049FB7B
    field<U>(self, 0x30) = 0; field<U>(self, 0x34) = 0; field<U>(self, 0x38) = 0; // 0049FB7E..0049FB84
    field<U>(self, 0x1C) = 0; field<U>(self, 0x20) = 0;                // 0049FB87..0049FB8A
    gob_set_scene_0043f200(attached, nullptr, nullptr);                // 0049FB72/0049FB8D Gob::SetScene(0)
    void* const parent = static_cast<void*>(target_cell->target);     // 0049FB92 mov eax,[edi] (spilled to the arg cell 0049FB9A)
    gob_set_scene_0043f200(attached, nullptr, field<void*>(parent, 0x68)); // 0049FB94..0049FB9E SetScene(parent scene)
    void* const attach_part = field<void*>(self, 0x18);                // 0049FBA3
    slots_of<PartSlots>(attach_part)->get_transform(static_cast<aurora_model::Part*>(attach_part), nullptr, at(attached, 0x78), at(attached, 0x84)); // 0049FBA6..0049FBB3
    (void)gob_vt(attached)->do_event(as_gob(attached), nullptr, "UpdateDependentPosition", 0); // 0049FBB6..0049FBC0 (0074375C)
    gob_add_attachment_00447310(parent, nullptr, attached);            // 0049FBC6..0049FBCB
    // 0049FBD0..0049FBEB: if (gob.scale_1B4 / gob.scale_1B8 != parent.scale_1B4) -> SetObjectScale(gob.1B8 * parent.1B4, 0).
    // `test ah,44h; jnp` skips only on C3 alone (equal); unordered (C3|C2) also takes the call.
    const double ratio = static_cast<double>(field<float>(attached, 0x1B4)) / static_cast<double>(field<float>(attached, 0x1B8));
    if (!(ratio == static_cast<double>(field<float>(parent, 0x1B4)))) {
        const float scale = static_cast<float>(static_cast<double>(field<float>(attached, 0x1B8)) *
                                               static_cast<double>(field<float>(parent, 0x1B4)));   // 0049FBED..0049FBFF fstp dword [esp]
        (void)gob_vt(attached)->s0cc_gob_set_object_scale(as_gob(attached), nullptr, float_to_bits(scale), 0u); // 0049FC02
    }
    // 0049FC08..0049FCB7: six RegisterCallBack(fn, event, data, 9999.0f (461C3C00h), 0) on the parent (SafePointer
    // target reloaded before each call), handles stored to +24..+38.
    const U kPriority = 0x461C3C00u;
    aurora_model::Part* const self_data = static_cast<aurora_model::Part*>(self);
    void* p = static_cast<void*>(target_cell->target);
    field<U>(self, 0x24) = gob_vt(p)->register_callback(as_gob(p), nullptr, behavior_update_scenes_attach_0049eb40, "*", self_data, kPriority, 0u);          // 0049FC1D (00743890)
    p = static_cast<void*>(target_cell->target);
    field<U>(self, 0x28) = gob_vt(p)->register_callback(as_gob(p), nullptr, behavior_update_dependent_position_0049eb90, "UpdateDependentPosition", self_data, kPriority, 0u); // 0049FC38 (0074375C)
    p = static_cast<void*>(target_cell->target);
    field<U>(self, 0x2C) = gob_vt(p)->register_callback(as_gob(p), nullptr, behavior_propagate_color_shifting_0049ebb0, "propagatecolorshift", field<aurora_model::Part*>(self, 0x10), kPriority, 0u);  // 0049FC56 (00740AAC)
    p = static_cast<void*>(target_cell->target);
    field<U>(self, 0x30) = gob_vt(p)->register_callback(as_gob(p), nullptr, behavior_propagate_illumination_0049ec00, "propagateillumination", field<aurora_model::Part*>(self, 0x10), kPriority, 0u); // 0049FC74 (00740AC0)
    p = static_cast<void*>(target_cell->target);
    field<U>(self, 0x34) = gob_vt(p)->register_callback(as_gob(p), nullptr, behavior_propagate_render_0049ec50, "propagaterender", field<aurora_model::Part*>(self, 0x10), kPriority, 0u);           // 0049FC92 (00742F04)
    p = static_cast<void*>(target_cell->target);
    field<U>(self, 0x38) = gob_vt(p)->register_callback(as_gob(p), nullptr, behavior_propagate_view_volume_0049ec90, "propagateviewvolume", field<aurora_model::Part*>(self, 0x10), kPriority, 0u); // 0049FCB0 (00740A98)
    return self;                                                       // 0049FCBB mov eax,esi
}

// 0049FDC0 / 0049FE00: base ctor, then the derived table and three zeroed DWORDs at +3C..+44 (RET 0Ch, EAX = this).
void* __fastcall behavior_attach_translate_0049fdc0(void* self, void*, void* gob, void* object, void* part) {
    behavior_attach_0049fb10(self, nullptr, gob, object, part);        // 0049FDC0..0049FDD4 (same three words)
    field<const BehaviorSlots*>(self, 0) = &C3::behavior_attach_translate_vtable_007438b8; // 0049FDDB
    field<U>(self, 0x3C) = 0; field<U>(self, 0x40) = 0; field<U>(self, 0x44) = 0;          // 0049FDE1..0049FDE7 (m_vTranslation)
    return self;                                                       // +48 (Translate::Control flag) is not written by the ctor
}
void* __fastcall behavior_attach_rotate_0049fe00(void* self, void*, void* gob, void* object, void* part) {
    behavior_attach_0049fb10(self, nullptr, gob, object, part);        // 0049FE00..0049FE14
    field<const BehaviorSlots*>(self, 0) = &C3::behavior_attach_rotate_vtable_007438dc;    // 0049FE1B
    field<U>(self, 0x3C) = 0; field<U>(self, 0x40) = 0; field<U>(self, 0x44) = 0;          // 0049FE21..0049FE27 (m_vRotation)
    return self;
}
// 0049FE40: base ctor + table 00743900 only (RET 0Ch).
void* __fastcall behavior_attach_no_orientation_0049fe40(void* self, void*, void* gob, void* object, void* part) {
    behavior_attach_0049fb10(self, nullptr, gob, object, part);        // 0049FE40..0049FE54
    field<const BehaviorSlots*>(self, 0) = &C3::behavior_attach_no_orientation_vtable_00743900; // 0049FE59
    return self;
}

// ---------------------------------------------------------------- event handlers registered by 0049FB10
// Handler ABI (Gob::DoEvent 0048662B..00486631): cdecl(gob raising the event, event name, registered data).
// 0049EB40 UpdateScenesAttach (data = behavior)
void __cdecl behavior_update_scenes_attach_0049eb40(void* gob, void* event, aurora_model::Part* context) {
    (void)gob;
    const char* const name = static_cast<const char*>(event);
    void* const behavior = static_cast<void*>(context);
    if (kotor_stricmp_0070acaf(name, "UpdateScenes") == 0) {          // 0049EB41..0049EB55 (00740AD8)
        slots_of<BehaviorSlots>(behavior)->set_scene(behavior, nullptr); // 0049EB57..0049EB5E jmp [eax+4] (tail, no stack words)
        return;
    }
    if (field<U>(behavior, 0x20) != 0 &&                               // 0049EB62..0049EB6B diewithoutparent
        kotor_stricmp_0070acaf(name, "Dying") == 0) {                  // 0049EB6D..0049EB7D (00741418)
        void* const attached = field<void*>(behavior, 0x10);           // 0049EB7F
        if (attached != nullptr)                                       // 0049EB82..0049EB84
            (void)gob_vt(attached)->s000_gob_destruct_gob(as_gob(attached), nullptr, 1u); // 0049EB86..0049EB8A scalar delete(1)
    }
}
// 0049EB90 UpdateDependentPosition (data = behavior): tail jump to behavior slot +20 (UpdateGobPosition 0049ECD0).
void __cdecl behavior_update_dependent_position_0049eb90(void* gob, void* event, aurora_model::Part* context) {
    (void)gob;
    if (kotor_stricmp_0070acaf(static_cast<const char*>(event), "UpdateDependentPosition") == 0) { // 0049EB90..0049EBA4 (0074375C)
        void* const behavior = static_cast<void*>(context);
        slots_of<BehaviorSlots>(behavior)->update_gob_position(behavior, nullptr); // 0049EBA6..0049EBAC jmp [edx+20h]
    }
}
// 0049EBB0 PropagateColorShifting (data = attached gob): target.SetColorShifting(source+CC vector, source+D8, 1).
void __cdecl behavior_propagate_color_shifting_0049ebb0(void* gob, void* event, aurora_model::Part* context) {
    if (kotor_stricmp_0070acaf(static_cast<const char*>(event), "propagatecolorshift") != 0) return; // 0049EBB0..0049EBC6 (00740AAC)
    void* const target = static_cast<void*>(context);                 // 0049EBD2 [esp+14h]
    const U raw_d8 = field<U>(gob, 0xD8);                              // 0049EBCC
    (void)gob_vt(target)->s0b4_gob_set_color_shifting(as_gob(target), nullptr,
        field<U>(gob, 0xCC), field<U>(gob, 0xD0), field<U>(gob, 0xD4), raw_d8, 1u); // 0049EBD8..0049EBF5 (Vector by value, raw DWORDs)
}
// 0049EC00 PropagateIllumination: target.SetIllumination(source+E0 vector, 1).
void __cdecl behavior_propagate_illumination_0049ec00(void* gob, void* event, aurora_model::Part* context) {
    if (kotor_stricmp_0070acaf(static_cast<const char*>(event), "propagateillumination") != 0) return; // 0049EC00..0049EC16 (00740AC0)
    void* const target = static_cast<void*>(context);
    (void)gob_vt(target)->s0b8_gob_set_illumination(as_gob(target), nullptr,
        field<U>(gob, 0xE0), field<U>(gob, 0xE4), field<U>(gob, 0xE8), 1u); // 0049EC18..0049EC3E
}
// 0049EC50 PropagateRender: if target+188 holds an object, its slot +08(0); then target slot +10(1).
void __cdecl behavior_propagate_render_0049ec50(void* gob, void* event, aurora_model::Part* context) {
    (void)gob;
    if (kotor_stricmp_0070acaf(static_cast<const char*>(event), "propagaterender") != 0) return; // 0049EC50..0049EC64 (00742F04)
    void* const target = static_cast<void*>(context);                 // 0049EC67
    void* const holder = field<void*>(target, 0x188);                  // 0049EC6B
    if (holder != nullptr)                                             // 0049EC71..0049EC73
        slots_of<RenderTargetSlots>(holder)->reset(holder, nullptr, 0u); // 0049EC75..0049EC78 push eax (= 0, the stricmp result)
    gob_vt(target)->s010_gob_render(target, nullptr, 1u);              // 0049EC7B..0049EC81 Gob::Render(1)
}
// 0049EC90 PropagateViewVolume: if source.IsInViewVolume() -> tail jump target.EnableInViewVolume().
void __cdecl behavior_propagate_view_volume_0049ec90(void* gob, void* event, aurora_model::Part* context) {
    if (kotor_stricmp_0070acaf(static_cast<const char*>(event), "propagateviewvolume") != 0) return; // 0049EC90..0049ECA4 (00740A98)
    if (gob_vt(gob)->s180_gob_is_in_view_volume(as_gob(gob), nullptr) == 0) return; // 0049ECA6..0049ECB4 (AL)
    void* const target = static_cast<void*>(context);
    (void)gob_vt(target)->s178_gob_enable_in_view_volume(as_gob(target), nullptr); // 0049ECB6..0049ECBC jmp [eax+178h]
}

// 004A6640 WindManager::WindManager() (thiscall, RET 0, EAX = this). EH FuncInfo 00718123 (no live cleanup state).
void* __fastcall wind_manager_construct_004a6640(void* self, void*) {
    field<U>(self, 0x04) = 0; field<U>(self, 0x08) = 0; field<U>(self, 0x00) = 0;          // 004A665F..004A6665
    field<U>(self, 0x10) = 0; field<U>(self, 0x14) = 0; field<U>(self, 0x0C) = 0;          // 004A6667..004A666D
    field<U>(self, 0x18) = 0; field<U>(self, 0x1C) = 0; field<U>(self, 0x20) = 0;          // 004A6670..004A6676
    field<U>(self, 0x24) = 0; field<U>(self, 0x28) = 0; field<U>(self, 0x2C) = 0;          // 004A6679..004A667F
    field<U>(self, 0x850) = 0; field<U>(self, 0x854) = 0; field<U>(self, 0x84C) = 0;       // 004A6682..004A668E
    field<U>(self, 0x24) = 0; field<U>(self, 0x18) = 0; field<U>(self, 0x28) = 0;          // 004A6696..004A66A0 (second Vector assignment)
    field<U>(self, 0x1C) = 0; field<U>(self, 0x2C) = 0; field<U>(self, 0x20) = 0;          // 004A66A3..004A66A9
    // 004A66B2..004A66BA: a zero Vector temporary on the frame (no memory outside the frame) - nothing to reproduce.
    field<U>(self, 0x838) = 0; field<U>(self, 0x83C) = 0; field<U>(self, 0x844) = 0;       // 004A66BE..004A66CA
    field<U>(self, 0x848) = 0x40000000u;                               // 004A66D0 2.0f (raw DWORD)
    wind_manager_fill_noise_field_004a57c0(self, nullptr, static_cast<float*>(at(self, 0x30)));   // 004A66AC..004A66DA fillNoiseField(this+30)
    wind_manager_fill_noise_field_004a57c0(self, nullptr, static_cast<float*>(at(self, 0x430)));  // 004A66DF..004A66E8 fillNoiseField(this+430)
    field<void*>(self, 0x830) = at(self, 0x30);                        // 004A66F1
    field<void*>(self, 0x834) = at(self, 0x430);                       // 004A66F7
    return self;                                                       // 004A66FE
}

// ================================================================ lights
// 004A38B0 LightManager::LightManager() (thiscall, RET 0, EAX = this). EH FuncInfo 0071805E (lists only).
void* __fastcall light_manager_construct_004a38b0(void* self, void*) {
    field<const C3::LightManagerVTable*>(self, 0) = &C3::light_manager_vtable_00743958; // 004A38C9
    static const U kZeroOrder[] = {                                    // 004A38CF..004A39D4, original store order
        0x0C, 0x10, 0x08, 0x18, 0x1C, 0x14, 0x24, 0x28, 0x20, 0x30, 0x34, 0x2C, 0x3C, 0x40, 0x38, 0x48, 0x4C, 0x44,
        0x54, 0x58, 0x50, 0x60, 0x64, 0x5C, 0x6C, 0x70, 0x68, 0x78, 0x7C, 0x74, 0x84, 0x88, 0x80, 0x90, 0x94, 0x8C,
        0x9C, 0xA0, 0x98, 0xB0, 0xB4, 0xAC, 0xBC, 0xC0, 0xB8, 0xC8, 0xCC, 0xC4, 0xD4, 0xD8, 0xD0, 0xE0, 0xE4, 0xDC,
        0xEC, 0xF0, 0xE8, 0x04, 0xA4, 0xA8};
    for (U offset : kZeroOrder) field<U>(self, offset) = 0;
    field<U8>(self, 0xF4) = 0;                                         // 004A39DA mov byte [eax+0F4h],cl
    return self;                                                       // 004A38BE mov eax,ecx
}
// 004A28A0 LightManager::GetBumpMapLights: lea eax,[ecx+5Ch]; ret.
List* __fastcall light_manager_get_bump_map_lights_004a28a0(void* lights, void*) { return list_at(lights, 0x5C); }
// 004A28B0 LightManager::GetLensFlareLights: lea eax,[ecx+0C4h]; ret.
List* __fastcall light_manager_get_lens_flare_lights_004a28b0(void* lights, void*) { return list_at(lights, 0xC4); }

// 004A2A00 SetLightGL(index, position, color, radius, ambient_flag) (cdecl). The original reuses its own argument cells
// 0 and 4 as the homogeneous w (1.0f) after their last read (004A2A0D / 004A2A51 / 004A2B0F / 004A2B30 / 004A2B56);
// those cells belong to this frame (cdecl caller 004A3866 discards them), so a local w is equivalent.
void __cdecl set_light_gl_004a2a00(I index, const void* position, const void* color, const float* radius, const U* ambient_flag) {
    using namespace kotor_renderer_init;
    float w = f(0x3F800000u);                                          // 004A2A0D
    const U light = static_cast<U>(index) + 0x4001u;                   // 004A2A3E / 004A2AFC add esi,4001h
    Vector zero{};
    const Vector* const rgb = static_cast<const Vector*>(color);
    if (*ambient_flag != 0) {                                          // 004A2A03..004A2A19 mov ecx,[eax]; test; je
        glLightfv_0073d388(light, 0x1200u, homogenize_004aaab0(rgb, &w));          // 004A2A1F..004A2A45 GL_AMBIENT [0073F320]
        w = f(0x3F800000u); zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;           // 004A2A51..004A2A69
        glLightfv_0073d388(light, 0x1201u, homogenize_004aaab0(&zero, &w));        // 004A2A71..004A2A81 GL_DIFFUSE [0073F324]
        w = f(0x3F800000u); zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;           // 004A2A8D..004A2AA5
        glLightfv_0073d388(light, 0x1202u, homogenize_004aaab0(&zero, &w));        // 004A2AAD..004A2ABD GL_SPECULAR [0073F328]
    } else {
        zero.x = 0.0f; zero.y = 0.0f; zero.z = 0.0f;                               // 004A2ACB..004A2ADB
        glLightfv_0073d388(light, 0x1200u, homogenize_004aaab0(&zero, &w));        // 004A2AE3..004A2B03
        w = f(0x3F800000u);                                                        // 004A2B0F
        glLightfv_0073d388(light, 0x1201u, homogenize_004aaab0(rgb, &w));          // 004A2B17..004A2B28
        w = f(0x3F800000u);                                                        // 004A2B30
        glLightfv_0073d388(light, 0x1202u, homogenize_004aaab0(rgb, &w));          // 004A2B38..004A2B49
    }
    w = f(0x3F800000u);                                                // 004A2B56
    glLightfv_0073d388(light, 0x1203u, homogenize_004aaab0(static_cast<const Vector*>(position), &w)); // 004A2B5E..004A2B6F GL_POSITION [0073F32C]
    // 004A2B71..004A2BA6: r = (radius > 0.1f) ? radius : 0.1f  (`test ah,41h; jne` takes 0.1f on <, == and unordered);
    // quadratic attenuation = 9.0f / (r*r + r*r), rounded to float by `fstp dword [esp+14h]`.
    const float minimum = f(0x3DCCCCCDu);                              // [0073E798] 0.1f
    const double r = (*radius > minimum) ? static_cast<double>(*radius) : static_cast<double>(minimum);
    const double square = r * r;                                       // 004A2B8E..004A2B95 fld st(0); fmul st,st(1)
    const float attenuation = static_cast<float>(static_cast<double>(f(0x41100000u)) / (square + square)); // 004A2B9D fadd st,st; 004A2BA0 fdivr [00742F00] 9.0f
    glLightfv_0073d388(light, 0x1209u, &attenuation);                  // 004A2B90..004A2BAC GL_QUADRATIC_ATTENUATION [0073F330]
}

// 004A3770 LightManager::GetNearestLights(scene, position, radius, flags) (thiscall, RET 18h).
// The gate 007B92DC has no writer in the image (BSS 0), so the original always returns 0 at 004A3780; the body is
// still reproduced in full.
void __fastcall light_manager_get_nearest_lights_004a3770(void* lights, void*, void* scene, Vector position, float radius, U flags) {
    (void)scene;                                                       // stack word 1 is never read (004A3770..004A38AC)
    if (*cells.dynamic_lights_007b92dc == 0) return;                   // 004A3773..004A3785 (EAX = 0, discarded by 00450DC1)
    using namespace kotor_renderer_init;
    List* const list = list_at(lights, 0x08);                          // [ecx+8] data, [ecx+0Ch] count
    I enabled = 0;                                                     // 004A378F xor ebx,ebx
    if (list->size > 0) {                                              // 004A3788..004A3798
        I i = 0;
        do {
            if (enabled >= 3) break;                                   // 004A37A0..004A37A3
            void* const light = element(list->data, i);               // 004A37A9..004A37AC (data re-read)
            bool use = true;
            if (flags != 0 && field<U>(light, 0x70) == 0) use = false; // 004A37AF..004A37BC
            else if (field<U>(light, 0x70) == 0 && field<U>(light, 0x6C) == 0) use = false; // 004A37C2..004A37CE
            if (use) {
                Vector delta;                                          // 004A37D4..004A3802 three fsub/fstp dword
                delta.x = static_cast<float>(static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(position.x));
                delta.y = static_cast<float>(static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(position.y));
                delta.z = static_cast<float>(static_cast<double>(field<float>(light, 0x9C)) - static_cast<double>(position.z));
                const double distance = vector_magnitude_004ab100(&delta);          // 004A3806 (ST0)
                const double reach = static_cast<double>(radius) + static_cast<double>(field<float>(light, 0x58)); // 004A380B..004A380F
                // 004A3815..004A381C fcompp; test ah,5; jnp: skip only when reach < distance (C0 alone).
                if (!(reach < distance)) {
                    const float light_radius = field<float>(light, 0x58);
                    // 004A381E..004A382C fcomp [0073D700] 0.0f; test ah,41h; jnp: skip on < or == (one flag), not on unordered.
                    if (!(light_radius < 0.0f) && !(light_radius == 0.0f)) {
                        glEnable_0073d390(static_cast<U>(enabled) + 0x4001u);              // 004A382E..004A3835
                        const double intensity = static_cast<double>(field<float>(light, 0x8C)); // 004A383B
                        Vector colour;                                                  // 004A3841..004A3862
                        colour.x = static_cast<float>(intensity * static_cast<double>(field<float>(light, 0x4C)));
                        colour.y = static_cast<float>(intensity * static_cast<double>(field<float>(light, 0x50)));
                        colour.z = static_cast<float>(intensity * static_cast<double>(field<float>(light, 0x54)));
                        set_light_gl_004a2a00(enabled, at(light, 0x94), &colour,
                                              static_cast<const float*>(at(light, 0x58)), static_cast<const U*>(at(light, 0x68))); // 004A3849..004A3866
                        ++enabled;                                     // 004A386E
                    }
                }
            }
            ++i;                                                       // 004A3873..004A387A
        } while (i < list->size);                                      // 004A387B..004A3881 (count re-read)
    }
    for (I k = enabled; k < 3; ++k) glDisable_0073d22c(static_cast<U>(k) + 0x4001u); // 004A3888..004A38A2 [0073D22C]
    // 004A38A6 mov eax,ebx: the enabled count is returned in EAX; the only caller (00450DBC) discards it.
}

// 00447180 Gob::CollectLights(out) (thiscall, RET 4): breadth-first walk from Gob+6C over Part children (+30/+34),
// appending each Part's slot +24 result (light) to `out`. EH FuncInfo 00715E38 frees the walk list (not emitted).
void __fastcall gob_collect_lights_00447180(void* gob, void*, List* out) {
    List walk;                                                         // [esp+20h] data, [esp+24h] size, [esp+28h] capacity
    walk.size = 0; walk.capacity = 0; walk.data = 0;                   // 0044719E..004471A6
    out->size = 0;                                                     // 004471AE mov [ebp+4],ebx
    void* const root = field<void*>(gob, 0x6C);                        // 004471B1
    menu_list_growth::allocate_005e03c0(&walk, nullptr, 8u);           // 004471B4..004471C2 Allocate(8)
    I size = walk.size;                                                // 004471C7
    U data = walk.data;                                                // 004471CB
    set_element(data, size, root);                                     // 004471CF..004471D3
    ++size; walk.size = size;                                          // 004471D6..004471D9
    I capacity = walk.capacity;
    for (I i = 0; i < size; ++i) {                                     // 004471DD..004471E1 / 004472CD..004472D8
        void* const part = element(data, i);                           // 004471F0..004471F4
        void* const light = slots_of<PartSlots>(part)->as_light(static_cast<aurora_model::Part*>(part), nullptr); // 004471F7..004471FF
        if (light != nullptr) list_add_allocate(out, light);           // 00447202..00447233 (005E03C0 growth)
        for (I j = 0; j < field<I>(part, 0x34); ++j) {                 // 00447236..00447241 / 004472BC..004472CB (count re-read)
            void* const child = element(field<U>(part, 0x30), j);     // 00447250..00447253 (data re-read)
            capacity = walk.capacity;                                  // 00447256
            if (size == capacity) {                                    // 0044725A cmp esi,eax; jne
                const I grown = capacity != 0 ? capacity + capacity : 8;     // 00447262..0044726A
                walk.capacity = grown;                                 // 00447279 mov [esp+2Ch],eax
                const U old = data;                                    // 00447277 mov ebx,edi
                void* const block = live_operator_new(static_cast<U>(grown) * 4u); // 0044726F..0044727D operator new(cap*4)
                const U fresh = address_of(block);
                for (I k = 0; k < size; ++k) set_element(fresh, k, element(old, k)); // 0044728D..0044729E (size>0)
                if (old != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old))); // 004472A4..004472A9
                data = fresh; walk.data = fresh;                       // 00447287..00447289
            }
            set_element(data, size, child);                            // 004472B5..004472B9
            ++size; walk.size = size;                                  // 004472BF / 004472C3
        }
    }
    if (data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(data))); // 004472E0..004472ED
}

// 0046CA30 UpdateStaticLighting(area, light) (cdecl). EH FuncInfo 00716E18 frees the local list (not emitted).
void __cdecl update_static_lighting_0046ca30(void* area, void* light) {
    if (field<U>(light, 0x70) != 0) return;                            // 0046CA36..0046CA54
    List found;                                                        // [esp+4] data, [esp+8] size, [esp+0Ch] capacity
    found.size = 0; found.capacity = 0; found.data = 0;                // 0046CA56..0046CA5E
    void* const bsp = field<void*>(area, 0xA0);                        // 0046CA62..0046CA66
    if (bsp != nullptr) {                                              // 0046CA6C..0046CA72
        bsp_sphere_check_004ad560(bsp, at(light, 0x94), field<float>(light, 0x58), &found); // 0046CA75..0046CA86 (radius pushed as raw DWORD)
        for (I i = 0; i < found.size; ++i)                             // 0046CA8B..0046CAB1 (count re-read)
            texture_join::add_unique_0046bd10(cells.static_light_rooms_007fbfe4, nullptr,
                static_cast<font::Texture*>(element(found.data, i)));  // 0046CA98..0046CAA5 (mov ecx,7FBFE4h)
    }
    if (found.data != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(found.data))); // 0046CAB4..0046CAC5
}

// 0046CC00 AddLightToRooms(area, light) (cdecl): add the light to every room whose XY box contains the light position;
// when none does, add it to every room (second loop, inlined CExoArrayList::Add with operator-new growth).
void __cdecl add_light_to_rooms_0046cc00(void* area, void* light) {
    I added = 0;                                                       // 0046CC0E xor ebx,ebx
    if (field<I>(area, 0xCC) > 0) {                                    // 0046CC06..0046CC14
        for (I i = 0; i < field<I>(area, 0xCC); ++i) {                 // 0046CCAB..0046CCB4 (count re-read)
            void* const room = element(field<U>(area, 0xC8), i);       // 0046CC20..0046CC2A
            const float lx = field<float>(light, 0x94), ly = field<float>(light, 0x98);
            // 0046CC2D..0046CC6B: skip on `test ah,1` (C0: x < min, or unordered) / `test ah,41h; jp` (x > max, or unordered)
            if (!(lx >= field<float>(room, 0x1C))) continue;
            if (!(ly >= field<float>(room, 0x20))) continue;
            if (!(lx <= field<float>(room, 0x28))) continue;
            if (!(ly <= field<float>(room, 0x2C))) continue;
            list_add_allocate(list_at(field<void*>(room, 0x58), 0x80), light); // 0046CC6D..0046CCA7 (room+58 -> +80 list)
            ++added;                                                   // 0046CCAA
        }
        if (added != 0) return;                                        // 0046CCBA..0046CCBC
    }
    for (I i = 0; i < field<I>(area, 0xCC); ++i) {                     // 0046CCC8..0046CCD2 / 0046CD5C..0046CD65
        void* const room = element(field<U>(area, 0xC8), i);           // 0046CCE0..0046CCE6
        List* const list = list_at(field<void*>(room, 0x58), 0x80);    // 0046CCE9..0046CCF8
        const I capacity = list->capacity;                             // 0046CCEC mov eax,[esi+88h]
        if (list->size == capacity) {                                  // 0046CCF2..0046CD00
            const I grown = capacity != 0 ? capacity + capacity : 8;   // 0046CD02..0046CD0A
            const U old = list->data;                                  // 0046CD0F mov edi,[esi]
            list->capacity = grown;                                    // 0046CD19 (before the allocation)
            list->data = address_of(live_operator_new(static_cast<U>(grown) * 4u)); // 0046CD11..0046CD24
            for (I k = 0; k < list->size; ++k)                         // 0046CD21..0046CD3E (size and data re-read)
                set_element(list->data, k, element(old, k));
            if (old != 0) live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(old))); // 0046CD40..0046CD45
        }
        set_element(list->data, list->size, light);                    // 0046CD51..0046CD56
        list->size = list->size + 1;                                   // 0046CD59 inc
    }
}

// 0046B790 LightBins(node, context) (cdecl BSPTraverse callback, pushed at 0046DDDC): for each element of
// [node+58]+60 (count +64, re-read) it calls 004015A0 void_return with ECX = element. 004015A0 is a bare `ret`
// (no stack words, no memory access), so the loop's only effects are its reads; they are kept.
void __cdecl light_bins_0046b790(void* node, void* context) {
    (void)context;
    void* const room = field<void*>(node, 0x58);                       // 0046B790..0046B796
    for (I i = 0; i < field<I>(room, 0x64); ++i)                       // 0046B799..0046B7B3
        (void)element(field<U>(room, 0x60), i);                        // 0046B7A2..0046B7A5 (then call 004015A0: ret)
}

// 0046DDA0 DoStaticLightBuckets(scene) (cdecl)
void __cdecl do_static_light_buckets_0046dda0(void* scene) {
    PointerList* const rooms = cells.static_light_rooms_007fbfe4;
    I size = rooms->size;                                              // 0046DDA0 mov eax,[007FBFE8]
    if (size > 0) {                                                    // 0046DDA8..0046DDAF
        I i = 0;
        do {
            light_node_tile_0046db60(element(rooms->data, i), scene);  // 0046DDB1..0046DDBB (data re-read)
            size = rooms->size;                                        // 0046DDC0 (re-read)
            ++i;
        } while (i < size);                                            // 0046DDC8..0046DDCB
    }
    if (size == 0) return;                                             // 0046DDCD/0046DDCF (test eax,eax; je)
    void* const bsp = field<void*>(scene, 0xA0);                       // 0046DDD1
    if (bsp != nullptr) bsp_traverse_004acea0(bsp, light_bins_0046b790, scene); // 0046DDD7..0046DDE2
}

// ================================================================ lights: shadows (Scene vtable slots +FC / +100)
// Both slots are declared by the canonical vtables_gen.h with the raw-word contract (EAX = U). Neither original body
// defines EAX on exit and the only caller (Scene::RenderShadows 00451B85 / 00451BD1) overwrites EAX immediately;
// 0 is returned on that undefined channel (proposed slot type: void, PACKET.json review_needed).
} // extern "C"
namespace {
// x87 FCOS/FSIN exactly as 00450918..0045092E: `fld st(0); fcos; fmul r; fadd base; fstp dword` (and fsin likewise).
// The transcendental instruction is the original's; fmul/fadd run under the process x87 control word.
float x87_cos_scale_add(double angle, float radius, float base) {
    float result;
    __asm {
        fld   angle
        fcos
        fmul  radius
        fadd  base
        fstp  result
    }
    return result;
}
float x87_sin_scale_add(double angle, float radius, float base) {
    float result;
    __asm {
        fld   angle
        fsin
        fmul  radius
        fadd  base
        fstp  result
    }
    return result;
}
U stencil_shadow_guard_007b9420 = 0;          // function-local static guard (bit 0), 00450781 / 0045078A
void* stencil_shadow_fan_007b941c = nullptr;  // function-local static: 0B0h-byte fan (11 x {x,y,z,colour}), 004507A8
}
extern "C" {
// 00450550 Scene::DoGobShadowsForThisLight(light) (thiscall, RET 4)
U __fastcall scene_do_gob_shadows_for_this_light_00450550(void* scene, void*, U light_word) {
    (void)scene;
    void* const light = reinterpret_cast<void*>(static_cast<std::uintptr_t>(light_word));       // 00450554 mov ebx,[esp+3Ch]
    // 00450558..00450573: radius = (light+60 > 0) ? light+60 : light+58 (`test ah,41h; jne` keeps +58 on <=, unordered).
    U radius_bits = field<U>(light, 0x58);
    if (field<float>(light, 0x60) > 0.0f) radius_bits = field<U>(light, 0x60);
    const float radius = f(radius_bits);
    I index = *cells.gob_bucket_count_007fbfd0;                        // 00450577
    if (index == 0) return 0;                                          // 0045057D..0045057F
    do {
        --index;                                                       // 0045058B
        if (*cells.animations_0078e3b8 != 0 && *cells.interleaved_007b92f4 != 0) { // 00450586..00450597
            if (index >= *cells.gob_bucket_count_007fbfd0) continue;   // 00450599..0045059F (-> 00450716 loop test)
            void* const animated = (*cells.gob_bucket_007fbfcc)[index]; // 004505A5..004505AB
            if (field<U>(animated, 0x188) != 0)                        // 004505AE..004505B6
                (void)gob_vt(animated)->animate(as_gob(animated), nullptr, *cells.frame_time_0078e574); // 004505B8..004505C1 slot +14 (dt raw)
            if (index >= *cells.gob_bucket_count_007fbfd0) continue;   // 004505C4..004505CA
        }
        void* const gob = (*cells.gob_bucket_007fbfcc)[index];         // 004505D0..004505D5
        if (field<U16>(gob, 0xC2) == 0) continue;                      // 004505D8..004505E0
        if (field<U8>(gob, 0x166) == 0) continue;                      // 004505E6..004505EE
        const U8 flags = static_cast<U8>(field<U16>(field<void*>(gob, 0x58), 0x50)); // 004505F4..004505FB (low byte)
        if (((flags >> 2) & 1u) == 0 && ((flags >> 5) & 1u) == 0) continue;       // 004505FD..0045060A
        Vector position = field<Vector>(gob, 0x78);                    // 00450610..00450625 (three DWORD copies)
        if (gob_is_in_cutscene_0043e8a0(gob, nullptr)) {               // 00450623..00450630
            aurora_model::Part* const dummy = find_part_0043fe00(field<aurora_model::Part*>(gob, 0x6C), "cutscenedummy"); // 00450632..0045063B (00740C8C)
            Quaternion orientation;                                    // 00450645..0045065D
            orientation.z = 0.0f; orientation.y = 0.0f; orientation.x = 0.0f; orientation.w = f(0x3F800000u);
            if (dummy != nullptr) {                                    // 00450665
                void* const part = static_cast<void*>(dummy);
                slots_of<PartSlots>(part)->get_transform(dummy, nullptr, &position, &orientation); // 00450667..00450675
            }
        }
        if (*cells.shadow_distance_check_0078e3cc != 0) {             // 00450678..0045067F
            void* const active = *cells.active_scene_007b92a0;         // 00450681
            Vector delta;                                              // 00450686..004506B1
            delta.x = static_cast<float>(static_cast<double>(field<float>(active, 0xA8)) - static_cast<double>(position.x));
            delta.y = static_cast<float>(static_cast<double>(field<float>(active, 0xAC)) - static_cast<double>(position.y));
            delta.z = static_cast<float>(static_cast<double>(field<float>(active, 0xB0)) - static_cast<double>(position.z));
            const double distance = vector_magnitude_004ab100(&delta); // 004506B5
            // 004506BA..004506C8 fcomp [0078E3C8]; test ah,41h; je -> skip when distance > limit (neither C0 nor C3).
            if (distance > static_cast<double>(*cells.shadow_distance_0078e3c8)) continue;
        }
        Vector to_light;                                               // 004506CA..004506F5
        to_light.x = static_cast<float>(static_cast<double>(field<float>(light, 0x94)) - static_cast<double>(position.x));
        to_light.y = static_cast<float>(static_cast<double>(field<float>(light, 0x98)) - static_cast<double>(position.y));
        to_light.z = static_cast<float>(static_cast<double>(field<float>(light, 0x9C)) - static_cast<double>(position.z));
        const double reach = vector_magnitude_004ab100(&to_light);    // 004506F9
        // 004506FE..0045070A fcomp [radius]; test ah,5; jp -> project only when reach < radius (C0 alone).
        if (!(reach < static_cast<double>(radius))) continue;
        project_dynamic_shadow_0044ff50(gob, light);                   // 0045070C..00450713
    } while (index != 0);                                              // 00450716..00450718
    return 0;
}

// 00450730 Scene::RenderStencilShadowsForThisLight(light, alpha, stencil_bit) (thiscall, RET 0Ch). EH FuncInfo
// 0071659E only resets the static guard if `new` throws (it returns NULL instead: not emitted).
U __fastcall scene_render_stencil_shadows_for_this_light_00450730(void* scene, void*, U light_word, U alpha_bits, U stencil_bit) {
    using namespace kotor_renderer_init;
    void* const light = reinterpret_cast<void*>(static_cast<std::uintptr_t>(light_word));       // 00450748
    U radius_bits = field<U>(light, 0x58);                             // 0045074F..00450758
    if (field<float>(light, 0x60) > 0.0f) radius_bits = field<U>(light, 0x60); // 0045074C..00450768
    float radius = f(radius_bits);
    // 0045076C..0045077B fcomp 0.0; test ah,44h; jnp -> return when radius == 0 (unordered continues).
    if (radius == 0.0f) return 0;
    if ((stencil_shadow_guard_007b9420 & 1u) == 0) {                   // 00450781..00450788
        stencil_shadow_guard_007b9420 |= 1u;                           // 0045078A
        stencil_shadow_fan_007b941c = live_operator_new(0xB0);         // 00450791..004507A8
    }
    unsigned char* const fan = static_cast<unsigned char*>(stencil_shadow_fan_007b941c); // 004507A8 / 004507B8
    // 004507BE..004507CD fcomp [00740CC0] 50.0f; test ah,41h; jne -> the light branch on <=, ==, unordered.
    if (radius > f(0x42480000u)) {
        *reinterpret_cast<U*>(fan + 0) = field<U>(scene, 0xA8);        // 004507CF..004507D5
        *reinterpret_cast<U*>(fan + 4) = field<U>(scene, 0xAC);        // 004507D7..004507DD
        *reinterpret_cast<U*>(fan + 8) = field<U>(scene, 0xB0);        // 004507E0..004507EE
        radius = f(0x42480000u);                                       // 004507E6 50.0f
    } else {
        // 004507F3..00450842: r' = sqrt(max(r*r - dz*dz, 0)), dz = |light.z - scene.z| (x87 PC_53: doubles)
        const double dz = std::fabs(static_cast<double>(field<float>(light, 0x9C)) - static_cast<double>(field<float>(scene, 0xB0)));
        const double r = static_cast<double>(radius);
        double squared = r * r - dz * dz;                              // 00450801..0045080D
        // 00450811..00450820 fcom 0; test ah,5; jp -> keep on >=, unordered; replace with 0.0 when < 0.
        if (squared < 0.0) squared = 0.0;
        const double root = std::sqrt(squared);                        // 00450826 fsqrt (IEEE correctly rounded, as SQRTSD)
        *reinterpret_cast<U*>(fan + 0) = field<U>(light, 0x94);        // 00450828..0045082E
        *reinterpret_cast<U*>(fan + 4) = field<U>(light, 0x98);        // 00450830..00450836
        *reinterpret_cast<U*>(fan + 8) = field<U>(scene, 0xB0);        // 00450839..0045083F
        radius = static_cast<float>(root);                             // 00450842 fstp dword [esp+1Ch]
    }
    glStencilFunc_0073d298(0x203u, static_cast<I32>(1u << ((stencil_bit - 1u) & 31u)), 0xFFFFFFFFu); // 00450846..0045085B ([0073F2AC]; shl edx,cl)
    glStencilOp_0073d270(0x1E00u, 0x1E00u, 0x1E00u);                  // 00450861..00450869 ([0073F2C8] x3)
    glDisable_0073d22c(*cells.gl_depth_test_0078d474);                 // 0045086F..00450876
    glEnable_0073d390(*cells.gl_blend_0078d448);                       // 0045087C..00450889
    bool soft = false;
    if (*kotor_soft_shadow::bindings.requested_0078e420 != 0 &&        // 0045088B..00450892
        aur_nv2x_level_shaders_available_0045fbd0()) {                 // 00450894..0045089B
        if (kotor_soft_shadow::SoftAvailable_0045f7b0() != 0 ||        // 0045089D..004508A4
            kotor_soft_shadow::ATISoftAvailable_0045f7e0() != 0) {     // 004508A6..004508AD
            soft = *kotor_soft_shadow::bindings.enabled_0078dae9 != 0; // 004508AF..004508B6
        }
    }
    if (soft) gl_blend_func_indexed_00401670(2u, 3u);                          // 004508B8..004508C1
    else glBlendFunc_0073d38c(0x302u, 0x303u);                         // 004508C6..004508D3 ([0073F270], [0073F274])
    // 004508D9..004508F9: centre colour = _ftol2(alpha * 0.5f * 255.0f) << 24 (in range: truncation == _ftol2).
    const double alpha = static_cast<double>(f(alpha_bits)) * static_cast<double>(f(0x3F000000u)) * static_cast<double>(f(0x437F0000u));
    *reinterpret_cast<U*>(fan + 0x0C) = static_cast<U>(static_cast<I>(alpha)) << 24;
    for (I i = 0; i < 10; ++i) {                                       // 004508F7..00450941 (counter kept in the arg cell)
        const double angle = static_cast<double>(i) * static_cast<double>(f(0x3F32B8C2u)); // 00450903..0045090E fild; fmul [007415C0]
        unsigned char* const vertex = fan + 0x10 * (i + 1);
        *reinterpret_cast<float*>(vertex + 0) = x87_cos_scale_add(angle, radius, *reinterpret_cast<const float*>(fan + 0)); // 00450918..00450922
        *reinterpret_cast<float*>(vertex + 4) = x87_sin_scale_add(angle, radius, *reinterpret_cast<const float*>(fan + 4)); // 00450925..0045092E
        *reinterpret_cast<U*>(vertex + 8) = field<U>(scene, 0xB0);     // 00450931..00450937 (re-read)
        *reinterpret_cast<U*>(vertex + 0x0C) = 0;                      // 0045093A
    }
    gl_render_objects_vcup_00431f70(5u, fan, 0x0B);                    // 00450943..00450948
    glEnable_0073d390(*cells.gl_depth_test_0078d474);                  // 0045094D..00450957
    return 0;
}
} // extern "C"
