// lane-3d-construct unit behavior_tables: CAurBehavior / CAurBehaviorAttach* typed vtables (00743864, 00743894,
// 007438B8, 007438DC, 00743900) and every slot body (gate G1). Evidence: dumpbin /disasm swkotor.exe (GOG K1),
// lane-local listings d/<ADDR>.txt. Float policy D30-FINAL (x87 PC_53: double intermediates, float at fstp dword).
// EH FuncInfo 00717DA8 / 00717DCB / 00717DF3 (local SafePointer unregistration / member lists on unwind) are not
// emitted: no callee throws (original operator new returns NULL).
#include "6c81d8874d-scene3d.h"
#include "516b075ab1-construct3d.h"
#include <cstdint>

extern "C" {
std::int32_t __cdecl kotor_stricmp_0070acaf(const char*, const char*);
// lane-3d-bsp bsp.h (identical declaration): CExoArrayList<T*>::Remove 00445080 thiscall(list, value) RET 4
void __fastcall exo_array_list_remove_00445080(kotor_reconstruction::ArrayMutations* list, void*, void* value);
// lane-font font_txi.hpp (D40 canonical, identical declarations)
char* __cdecl firstword_00463530(char*);
void __cdecl parse_int_004635e0(char*, volatile std::int32_t*);
void __cdecl parse_vector_00463570(char*, volatile float*);
// core (spellings of lane-3d-emitter/vp/mesh and lane-3d-bsp)
kotor_scene3d::Quaternion* __cdecl quaternion_multiply_004a9ac0(kotor_scene3d::Quaternion* out, const kotor_scene3d::Quaternion* a,
                                                               const kotor_scene3d::Quaternion* b);
kotor_scene3d::Quaternion* __cdecl yaw_pitch_roll_004acac0(kotor_scene3d::Quaternion* out, float yaw, float pitch, float roll);
}

namespace {
using namespace kotor_scene3d;
namespace C3 = kotor_scene3d_construct;
using C3::List; using C3::PartSlots; using C3::RenderTargetSlots; using C3::slots_of;

inline const aurora_model::GobVTable* gob_vt(void* gob) {
    return *static_cast<const aurora_model::GobVTable* const volatile*>(gob);
}
inline const SceneVTable* scene_vt(void* scene) { return *static_cast<const SceneVTable* const volatile*>(scene); }
inline aurora_model::Gob* as_gob(void* p) { return static_cast<aurora_model::Gob*>(p); }
inline List* list_at(void* object, U offset) { return static_cast<List*>(at(object, offset)); }
inline void* element(U data, I index) {
    return *reinterpret_cast<void* const volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u);
}
inline void set_element(U data, I index, void* value) {
    *reinterpret_cast<void* volatile*>(static_cast<std::uintptr_t>(data) + static_cast<U>(index) * 4u) = value;
}
inline U word_of(const volatile void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class T> inline T* from_word(U w) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(w)); }
inline float f(U bits) { return bits_to_float(bits); }
inline void list_add_allocate(List* list, void* value) {      // inlined CExoArrayList::Add, 005E03C0 growth
    const I capacity = list->capacity;
    if (list->size == capacity)
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<U>(capacity != 0 ? capacity + capacity : 8));
    set_element(list->data, list->size, value);
    list->size = list->size + 1;
}
// A stack SafePointer<CAurObject> registered in the target's +4 list (0049F28F..0049F2C3 / 0049F3EC..0049F41F):
// the target's destructor nulls the cell, so every later read goes through the volatile cell.
inline void* cell_address(void* volatile* cell) { return const_cast<void**>(cell); }
inline void unregister_cell(void* volatile* cell) {          // 0049F357..0049F35F / 0049F4C4..0049F4CC
    void* const target = *cell;
    if (target != nullptr) exo_array_list_remove_00445080(list_at(target, 4), nullptr, cell_address(cell));
}
// Vector + Vector with the original `fld; fadd; fstp dword` per component (0049F520..0049F550 / 0049F599..0049F5C8)
inline Vector add(const Vector& a, const Vector& b) {
    Vector r;
    r.x = static_cast<float>(static_cast<double>(a.x) + static_cast<double>(b.x));
    r.y = static_cast<float>(static_cast<double>(a.y) + static_cast<double>(b.y));
    r.z = static_cast<float>(static_cast<double>(a.z) + static_cast<double>(b.z));
    return r;
}
} // namespace

extern "C" {
// ================================================================ CAurBehavior (base)
// 0049FA70 CAurBehavior::~CAurBehavior (thiscall, RET 0)
void __fastcall behavior_destruct_0049fa70(void* self, void*) {
    field<const C3::BehaviorBaseSlots*>(self, 0) = &C3::behavior_vtable_00743864;   // 0049FA8F
    List* const deps = list_at(self, 4);
    const I count = deps->size;                                        // 0049FA95 (read once)
    for (I i = 0; i < count; ++i) {                                    // 0049FAA0..0049FAC8
        void* const cell = element(deps->data, i);                     // 0049FAA3..0049FAA6 (data re-read)
        if (cell == nullptr) continue;                                 // 0049FAA9..0049FAAB
        void* const target = *static_cast<void* volatile*>(cell);      // 0049FAAD
        if (target != nullptr) exo_array_list_remove_00445080(list_at(target, 4), nullptr, cell); // 0049FAB1..0049FAB7
        live_free(cell);                                               // 0049FABC..0049FABD
    }
    if (deps->data != 0) {                                             // 0049FACD..0049FAD2
        live_free(from_word<void>(deps->data));                        // 0049FAD4..0049FAD5
        deps->data = 0;                                                // 0049FADD
    }
    deps->capacity = 0;                                                // 0049FAE0
    deps->size = 0;                                                    // 0049FAE3
    if (deps->data != 0) live_free(from_word<void>(deps->data));       // 0049FAE6..0049FAF6 (member dtor; data is 0 here)
}
// 0049FFD0 CAurBehavior scalar deleting destructor (RET 4, EAX = this)
void* __fastcall behavior_scalar_delete_0049ffd0(void* self, void*, U flags) {
    behavior_destruct_0049fa70(self, nullptr);                         // 0049FFD3
    if ((flags & 1u) != 0) live_free(self);                            // 0049FFD8..0049FFE0
    return self;                                                       // 0049FFE8
}
// 0049F270 CAurBehavior::Control(dt) (thiscall, RET 4) -> 1 when the attached gob still owns this behavior.
U __fastcall behavior_control_0049f270(void* self, void*, U raw_delta) {
    void* volatile guard = field<void*>(self, 0x10);                   // 0049F28A..0049F28F local SafePointer
    if (guard != nullptr) list_add_allocate(list_at(guard, 4), cell_address(&guard)); // 0049F295..0049F2C3
    List* const deps = list_at(self, 4);
    for (I i = deps->size - 1; i >= 0; --i) {                          // 0049F2C7..0049F2D3 / 0049F334..0049F335
        void* const cell = element(deps->data, i);                     // 0049F2E0..0049F2E3
        void* const target = *static_cast<void* volatile*>(cell);      // 0049F2E6 mov ecx,[edi] (no null test of the cell)
        if (target != nullptr) {
            (void)gob_vt(target)->animate(as_gob(target), nullptr, raw_delta); // 0049F2EC..0049F2EF slot +14
            continue;
        }
        if (cell != nullptr) {                                         // 0049F2F4..0049F2F6
            void* const again = *static_cast<void* volatile*>(cell);  // 0049F2F8
            if (again != nullptr) exo_array_list_remove_00445080(list_at(again, 4), nullptr, cell); // 0049F2FC..0049F302
            live_free(cell);                                           // 0049F307..0049F308
        }
        const I last = deps->size - 1;                                 // 0049F310..0049F313
        deps->size = last;                                             // 0049F31A
        for (I k = i; k < deps->size; ++k)                             // 0049F31D..0049F332 (size re-read)
            set_element(deps->data, k, element(deps->data, k + 1));
    }
    if (guard == nullptr) return 0;                                    // 0049F338..0049F33F
    void* const holder = guard;
    const U owner = gob_vt(holder)->s108_gob_get_behavior(as_gob(holder), nullptr, 0xFFFFFFFFu); // 0049F341..0049F345
    const bool owned = owner == word_of(self);                         // 0049F34B
    unregister_cell(&guard);                                           // 0049F34D..0049F35F / 0049F379..0049F385
    return owned ? 1u : 0u;                                            // 0049F38F / 0049F365
}
// typed instances of the shared one-instruction bodies used by the five tables (D24)
void __fastcall behavior_void_return_004015a0(void* self, void*) { (void)self; }                        // ret
void __fastcall behavior_void_return_4_0060e760(void* self, void*, char* line) { (void)self; (void)line; } // ret 4
U __fastcall behavior_return_zero_0063e7f0(void* self, void*) { (void)self; return 0; }                // xor eax,eax; ret
void* __fastcall behavior_null_return_0063e7f0(void* self, void*) { (void)self; return nullptr; }      // xor eax,eax; ret
void* __fastcall behavior_self_return_00641db0(void* self, void*) { return self; }                     // mov eax,ecx; ret

// ================================================================ CAurBehaviorAttach
// 00447370 Gob::RemoveAttachment(attachment) (thiscall, RET 4)
void __fastcall gob_remove_attachment_00447370(void* gob, void*, void* attachment) {
    List* const list = list_at(gob, 0x1BC);                            // 00447376
    I remaining = list->size;                                          // 00447370
    I found = 0;
    if (remaining <= 0) return;                                        // 0044737F..00447381
    U cursor = list->data;                                             // 00447383
    do {
        if (*from_word<void* const volatile>(cursor) == attachment) ++found; // 00447390..00447394
        cursor += 4;
    } while (--remaining != 0);                                        // 00447398..00447399
    if (found != 0) exo_array_list_remove_00445080(list, nullptr, attachment); // 0044739B..004473A0
}
// 0049FCE0 CAurBehaviorAttach::~CAurBehaviorAttach (thiscall, RET 0)
void __fastcall behavior_attach_destruct_0049fce0(void* self, void*) {
    field<const C3::BehaviorSlots*>(self, 0) = &C3::behavior_attach_vtable_00743894; // 0049FCFF
    void* const object = field<void*>(self, 0x14);                     // 0049FD05
    if (object != nullptr) {                                           // 0049FD08..0049FD15
        void* const gob = field<void*>(self, 0x10);                    // 0049FD17
        if (gob != nullptr) {                                          // 0049FD1A..0049FD1C
            bool remove = true;
            void* const holder = field<void*>(gob, 0x188);             // 0049FD1E
            if (holder != nullptr) {                                   // 0049FD24..0049FD26
                void* const owner = slots_of<RenderTargetSlots>(holder)->query_14(holder, nullptr); // 0049FD28..0049FD2A
                if (owner != nullptr && field<void*>(owner, 0x14) == field<void*>(self, 0x14)) remove = false; // 0049FD2D..0049FD36
            }
            if (remove) gob_remove_attachment_00447370(object, nullptr, field<void*>(self, 0x10)); // 0049FD38..0049FD44
        }
        for (U offset = 0x24; offset <= 0x38; offset += 4)              // 0049FD49..0049FD88 (object captured, table reloaded)
            (void)gob_vt(object)->s02c_gob_deregister_callback(as_gob(object), nullptr, field<U>(self, offset));
    }
    void* const target = field<void*>(self, 0x14);                     // 0049FD8B (SafePointer dtor, reloaded)
    if (target != nullptr) exo_array_list_remove_00445080(list_at(target, 4), nullptr, at(self, 0x14)); // 0049FD91..0049FD95
    behavior_destruct_0049fa70(self, nullptr);                         // 0049FD9A..0049FDA4
}
// 004A0030: jmp 0049FCE0
void __fastcall behavior_attach_destruct_004a0030(void* self, void*) { behavior_attach_destruct_0049fce0(self, nullptr); }
// 0049FFF0 / 004A0010 scalar deleting destructors (RET 4, EAX = this)
void* __fastcall behavior_attach_scalar_delete_0049fff0(void* self, void*, U flags) {
    behavior_attach_destruct_0049fce0(self, nullptr);                  // 0049FFF3
    if ((flags & 1u) != 0) live_free(self);                            // 0049FFF8..004A0000
    return self;
}
void* __fastcall behavior_attach_scalar_delete_004a0010(void* self, void*, U flags) {
    behavior_attach_destruct_004a0030(self, nullptr);                  // 004A0013
    if ((flags & 1u) != 0) live_free(self);                            // 004A0018..004A0020
    return self;
}
// 0049F0C0 CAurBehaviorAttach::SetScene (slot +04, RET 0)
void __fastcall behavior_attach_set_scene_0049f0c0(void* self, void*) {
    void* const object = field<void*>(self, 0x14);                     // 0049F0C0
    if (object == nullptr) return;                                     // 0049F0C3..0049F0C5
    gob_set_scene_0043f200(field<void*>(self, 0x10), nullptr, field<void*>(object, 0x68)); // 0049F0C7..0049F0CE
}
// 0049ECD0 CAurBehaviorAttach::UpdateGobPosition (slot +20, RET 0)
void __fastcall behavior_attach_update_gob_position_0049ecd0(void* self, void*) {
    void* const gob = field<void*>(self, 0x10);                        // 0049ECD1
    void* const part = field<void*>(self, 0x18);                       // 0049ECD4
    slots_of<PartSlots>(part)->get_transform(static_cast<aurora_model::Part*>(part), nullptr, at(gob, 0x78), at(gob, 0x84)); // 0049ECD7..0049ECE4
    (void)gob_vt(gob)->do_event(as_gob(gob), nullptr, "UpdateDependentPosition", 0u); // 0049ECE7..0049ECF2 (0074375C)
}
// 0049F3B0 CAurBehaviorAttach::Control(dt) (slot +08, RET 4)
U __fastcall behavior_attach_control_0049f3b0(void* self, void*, U raw_delta) {
    bool animate = true;                                               // 0049F3D3 mov bl,1
    void* const scene = *cells.active_scene_007b92a0;                  // 0049F3CB
    if (scene != nullptr && scene_vt(scene)->s0a8_scene_is_animating(scene, nullptr) == 0) animate = false; // 0049F3D1..0049F3E3
    void* const gob = field<void*>(self, 0x10);                        // 0049F3E5 (ebp, kept)
    void* volatile guard = gob;                                        // 0049F3EC local SafePointer
    if (gob != nullptr) list_add_allocate(list_at(gob, 4), cell_address(&guard)); // 0049F3F2..0049F41F
    if (animate) {                                                     // 0049F427..0049F431
        if (behavior_control_0049f270(self, nullptr, raw_delta) == 0) { // 0049F433..0049F441
            unregister_cell(&guard);                                   // 0049F443..0049F453
            return 0;
        }
    }
    void* const holder = guard;                                        // 0049F46E / 0049F422 (no null test)
    const U owner = gob_vt(holder)->s108_gob_get_behavior(as_gob(holder), nullptr, 0xFFFFFFFFu); // 0049F472..0049F476
    if (owner == word_of(self)) {                                      // 0049F47C..0049F47E
        if (field<U>(self, 0x14) == 0) {                               // 0049F480..0049F485
            if (field<U>(self, 0x20) != 0) {                           // 0049F487..0049F48C diewithoutparent
                void* const attached = field<void*>(self, 0x10);       // 0049F48E
                if (attached != nullptr) (void)gob_vt(attached)->s000_gob_destruct_gob(as_gob(attached), nullptr, 1u); // 0049F491..0049F499
            } else {
                (void)gob_vt(gob)->s058_gob_set_behavior(as_gob(gob), nullptr, nullptr); // 0049F49D..0049F4A4
            }
        } else {
            void* const part = field<void*>(self, 0x18);               // 0049F4A9
            slots_of<PartSlots>(part)->get_transform(static_cast<aurora_model::Part*>(part), nullptr, at(gob, 0x78), at(gob, 0x84)); // 0049F4AC..0049F4B9
        }
    }
    unregister_cell(&guard);                                           // 0049F4BC..0049F4CC
    return 1;                                                          // 0049F4D7
}
// 0049F4F0 CAurBehaviorAttachTranslate::Control(dt) (RET 4)
U __fastcall behavior_attach_translate_control_0049f4f0(void* self, void*, U raw_delta) {
    const U result = behavior_attach_control_0049f3b0(self, nullptr, raw_delta); // 0049F4FA..0049F4FD
    if (result == 0) return 0;                                         // 0049F504..0049F506
    const Vector& translation = field<Vector>(self, 0x3C);
    if (field<U>(self, 0x48) != 0) {                                   // 0049F50C..0049F514 (+48: not written by the ctor)
        void* gob = field<void*>(self, 0x10);
        Vector got;
        const Vector* const position = from_word<const Vector>(gob_vt(gob)->s064_gob_get_position(as_gob(gob), nullptr, word_of(&got))); // 0049F516..0049F51D
        const Vector moved = add(*position, translation);              // 0049F520..0049F558 (position + m_vTranslation)
        gob = field<void*>(self, 0x10);                                // 0049F525 (reloaded)
        Vector ret;
        (void)gob_vt(gob)->s05c_gob_set_position(as_gob(gob), nullptr, &ret, moved); // 0049F55B..0049F560
        return result;
    }
    void* gob = field<void*>(self, 0x10);
    Quaternion got_orientation;
    const Quaternion* const orientation = from_word<const Quaternion>(
        gob_vt(gob)->s068_gob_get_orientation(as_gob(gob), nullptr, word_of(&got_orientation))); // 0049F56E..0049F579
    Vector rotated;
    const Vector* const offset = quaternion_rotate_004a9bb0(&rotated, orientation, &translation); // 0049F57C..0049F582
    gob = field<void*>(self, 0x10);                                    // 0049F587
    Vector got;
    const Vector* const position = from_word<const Vector>(gob_vt(gob)->s064_gob_get_position(as_gob(gob), nullptr, word_of(&got))); // 0049F591..0049F596
    const Vector moved = add(*position, *offset);                      // 0049F599..0049F5D0
    gob = field<void*>(self, 0x10);                                    // 0049F59D
    Vector ret;
    (void)gob_vt(gob)->s05c_gob_set_position(as_gob(gob), nullptr, &ret, moved); // 0049F5D3..0049F5D8
    return result;
}
// 0049F5F0 CAurBehaviorAttachRotate::Control(dt) (RET 4): orientation = gob orientation * YawPitchRoll(+44, +3C, +40)
U __fastcall behavior_attach_rotate_control_0049f5f0(void* self, void*, U raw_delta) {
    const U result = behavior_attach_control_0049f3b0(self, nullptr, raw_delta); // 0049F5FA..0049F5FD
    if (result == 0) return 0;                                         // 0049F604..0049F606
    void* const gob = field<void*>(self, 0x10);                        // 0049F611 (before the YawPitchRoll call)
    Quaternion ypr;
    const Quaternion* const turn = yaw_pitch_roll_004acac0(&ypr, field<float>(self, 0x44), field<float>(self, 0x3C),
                                                           field<float>(self, 0x40)); // 0049F608..0049F61C
    Quaternion got;
    const Quaternion* const current = from_word<const Quaternion>(gob_vt(gob)->s068_gob_get_orientation(as_gob(gob), nullptr, word_of(&got))); // 0049F621..0049F62E
    Quaternion product;
    const Quaternion* const q = quaternion_multiply_004a9ac0(&product, current, turn); // 0049F631..0049F637
    void* const target = field<void*>(self, 0x10);                     // 0049F63E / 0049F65F (reloaded)
    Quaternion ret;
    (void)gob_vt(target)->s060_gob_set_orientation(as_gob(target), nullptr, word_of(&ret),
        float_to_bits(q->w), float_to_bits(q->x), float_to_bits(q->y), float_to_bits(q->z)); // 0049F643..0049F662 (four DWORD copies)
    return result;
}
// 0049F670 CAurBehaviorAttachNoOrientation::Control(dt) (RET 4): keep the orientation the gob had before attaching.
U __fastcall behavior_attach_no_orientation_control_0049f670(void* self, void*, U raw_delta) {
    void* gob = field<void*>(self, 0x10);                              // 0049F676
    Quaternion saved;
    (void)gob_vt(gob)->s068_gob_get_orientation(as_gob(gob), nullptr, word_of(&saved)); // 0049F679..0049F681 (buffer reused, EAX dropped)
    const U result = behavior_attach_control_0049f3b0(self, nullptr, raw_delta); // 0049F684..0049F68B
    if (result != 0) {                                                 // 0049F690..0049F694
        gob = field<void*>(self, 0x10);                                // 0049F696
        Quaternion ret;
        (void)gob_vt(gob)->s060_gob_set_orientation(as_gob(gob), nullptr, word_of(&ret),
            float_to_bits(saved.w), float_to_bits(saved.x), float_to_bits(saved.y), float_to_bits(saved.z)); // 0049F69F..0049F6C0
    }
    return result;
}
// 0049ED00 / 0049ED40 / 0049EDA0 Message(line) (RET 4)
void __fastcall behavior_attach_message_0049ed00(void* self, void*, char* line) {
    if (kotor_stricmp_0070acaf("diewithoutparent", firstword_00463530(line)) == 0)   // 0049ED06..0049ED1E (00743774)
        parse_int_004635e0(line + 0x10, static_cast<volatile std::int32_t*>(at(self, 0x20))); // 0049ED20..0049ED28
}
void __fastcall behavior_attach_translate_message_0049ed40(void* self, void*, char* line) {
    if (kotor_stricmp_0070acaf("diewithoutparent", firstword_00463530(line)) == 0)   // 0049ED46..0049ED5E
        parse_int_004635e0(line + 0x10, static_cast<volatile std::int32_t*>(at(self, 0x20))); // 0049ED60..0049ED68
    if (kotor_stricmp_0070acaf("m_vTranslation", firstword_00463530(line)) == 0)     // 0049ED70..0049ED86 (00743788)
        parse_vector_00463570(line + 0x0E, static_cast<volatile float*>(at(self, 0x3C))); // 0049ED88..0049ED90
}
void __fastcall behavior_attach_rotate_message_0049eda0(void* self, void*, char* line) {
    if (kotor_stricmp_0070acaf("diewithoutparent", firstword_00463530(line)) == 0)   // 0049EDA6..0049EDBE
        parse_int_004635e0(line + 0x10, static_cast<volatile std::int32_t*>(at(self, 0x20))); // 0049EDC0..0049EDC8
    if (kotor_stricmp_0070acaf("m_vRotation", firstword_00463530(line)) == 0)        // 0049EDD0..0049EDE6 (00743798)
        parse_vector_00463570(line + 0x0B, static_cast<volatile float*>(at(self, 0x3C))); // 0049EDE8..0049EDF0
}
// GetType (slot +10)
U __fastcall behavior_attach_get_type_0049fcd0(void* self, void*) { (void)self; return 0x3E9; }                // mov eax,3E9h
U __fastcall behavior_attach_translate_get_type_0049fdf0(void* self, void*) { (void)self; return 0x3EA; }      // mov eax,3EAh
U __fastcall behavior_attach_rotate_get_type_0049fe30(void* self, void*) { (void)self; return 0x3EB; }         // mov eax,3EBh
U __fastcall behavior_attach_no_orientation_get_type_0049fe70(void* self, void*) { (void)self; return 0x3F0; } // mov eax,3F0h
} // extern "C"

// ================================================================ typed tables (.rdata images, slot order cited)
namespace kotor_scene3d_construct {
const BehaviorBaseSlots behavior_vtable_00743864 = {          // 00743864: 0049FFD0 004015A0 0049F270 0060E760 0063E7F0 x4
    behavior_scalar_delete_0049ffd0, behavior_void_return_004015a0, behavior_control_0049f270, behavior_void_return_4_0060e760,
    behavior_return_zero_0063e7f0, behavior_null_return_0063e7f0, behavior_return_zero_0063e7f0, behavior_return_zero_0063e7f0};
const BehaviorSlots behavior_attach_vtable_00743894 = {       // 00743894: 0049FFF0 0049F0C0 0049F3B0 0049ED00 0049FCD0 00641DB0 0063E7F0 0063E7F0 0049ECD0
    {behavior_attach_scalar_delete_0049fff0, behavior_attach_set_scene_0049f0c0, behavior_attach_control_0049f3b0,
     behavior_attach_message_0049ed00, behavior_attach_get_type_0049fcd0, behavior_self_return_00641db0,
     behavior_return_zero_0063e7f0, behavior_return_zero_0063e7f0},
    behavior_attach_update_gob_position_0049ecd0};
const BehaviorSlots behavior_attach_translate_vtable_007438b8 = { // 007438B8: 004A0010 0049F0C0 0049F4F0 0049ED40 0049FDF0 00641DB0 0063E7F0 0063E7F0 0049ECD0
    {behavior_attach_scalar_delete_004a0010, behavior_attach_set_scene_0049f0c0, behavior_attach_translate_control_0049f4f0,
     behavior_attach_translate_message_0049ed40, behavior_attach_translate_get_type_0049fdf0, behavior_self_return_00641db0,
     behavior_return_zero_0063e7f0, behavior_return_zero_0063e7f0},
    behavior_attach_update_gob_position_0049ecd0};
const BehaviorSlots behavior_attach_rotate_vtable_007438dc = {   // 007438DC: 004A0010 0049F0C0 0049F5F0 0049EDA0 0049FE30 00641DB0 0063E7F0 0063E7F0 0049ECD0
    {behavior_attach_scalar_delete_004a0010, behavior_attach_set_scene_0049f0c0, behavior_attach_rotate_control_0049f5f0,
     behavior_attach_rotate_message_0049eda0, behavior_attach_rotate_get_type_0049fe30, behavior_self_return_00641db0,
     behavior_return_zero_0063e7f0, behavior_return_zero_0063e7f0},
    behavior_attach_update_gob_position_0049ecd0};
const BehaviorSlots behavior_attach_no_orientation_vtable_00743900 = { // 00743900: 004A0010 0049F0C0 0049F670 0049ED00 0049FE70 00641DB0 0063E7F0 0063E7F0 0049ECD0
    {behavior_attach_scalar_delete_004a0010, behavior_attach_set_scene_0049f0c0, behavior_attach_no_orientation_control_0049f670,
     behavior_attach_message_0049ed00, behavior_attach_no_orientation_get_type_0049fe70, behavior_self_return_00641db0,
     behavior_return_zero_0063e7f0, behavior_return_zero_0063e7f0},
    behavior_attach_update_gob_position_0049ecd0};
}
