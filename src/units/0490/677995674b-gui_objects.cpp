// Lane gui-load: native constructors / destructors / small virtual slots of the
// CSWGui object, control, label, button, 3D-scene-view, border, text and panel
// classes used by CSWGuiMainMenu, plus control events and the shared trivial
// slot leaves (D24). Each body cites the instructions that establish it.
// EH: constructors reproduce the original unwind order of already-constructed
// subobjects with try/catch(...) { destroy in reverse; throw; } (normal path
// unchanged); the exact unwind funclets of FuncInfo tables are not decoded (gate).
#include "b601162484-gui_objects.h"
#include "47d8056e59-gff_resource.h"   // lane-gff: CResGFF vtable 0073E2D0 typed slots (Panel dtor 0040CFC2)
#include <cstring>
#include <new>

using namespace gui_load;

namespace {
template<class V> const V* table_view(const void* table) { return static_cast<const V*>(table); }
void set_object_vptr(Object* object, const void* table) { object->vtbl = table_view<ObjectVtbl>(table); }
String* as_string(unsigned char* bytes) { return reinterpret_cast<String*>(bytes); }
BorderObject* border_view(Border* border) { return reinterpret_cast<BorderObject*>(border); }
TextObject* text_view(Text* text) { return reinterpret_cast<TextObject*>(text); }
kih::Manager* gui_manager() { return static_cast<kih::Manager*>(kotor_input_cells::gui_manager_007a39f4); }
kih::Control* input_view(Object* object) { return reinterpret_cast<kih::Control*>(object); }
Word word_at(const void* object, unsigned offset) {
    Word value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, 4); return value;
}
// CSWGuiScene (lane-3d-render table 0073E3F0) slot +04 SetExtent view.
struct SceneVtbl { const void* slot00; void (__fastcall* set_extent)(void*, void*, const Extent*); };
}

extern "C" {

// ------------------------------------------------------------------ shared leaves (D24)
void __fastcall gui_load_slot_ret8_005b5e90(Object*, void*, Word, Word) {}            // 005B5E90 RET 8
void __fastcall gui_load_slot_ret_004015a0(Object*, void*) {}                         // 004015A0 RET
void __fastcall gui_load_slot_ret4_0060e760(Object*, void*, Word) {}                  // 0060E760 RET 4
Word __fastcall gui_load_slot_zero_0063e7f0(Object*, void*) { return 0; }             // 0063E7F0 XOR EAX,EAX
Object* __fastcall gui_load_slot_self_00641db0(Object* self, void*) { return self; }  // 00641DB0 MOV EAX,ECX
Word __fastcall gui_load_slot_zero8_00409eb0(Object*, void*, Word, Word) { return 0; } // 00409EB0 RET 8
Word __fastcall gui_load_slot_prop10_004064a0(Object* self, void*) { return word_at(self, 0x10); } // 004064A0

// ------------------------------------------------------------------ CSWGuiObject / CSWGuiDrawable
// 00409E90 SetExtent: four dwords to +04.
void __fastcall gui_load_object_set_extent_00409e90(Object* self, void*, const Extent* extent) {
    self->extent = *extent;
}
// 00409EC0 scalar deleting dtor: vptr = 0073DF20 (00409EC8); flags&1 -> free (00409ED1).
void* __fastcall gui_load_object_deleting_destruct_00409ec0(Object* self, void*, Word flags) {
    set_object_vptr(self, &gui_load_object_methods_0073df20);
    if (flags & 1u) live_free(self);
    return self;
}
// 00414F90 drawable scalar deleting dtor: vptr = 0073E324 (00414F98).
void* __fastcall gui_load_drawable_deleting_destruct_00414f90(void* self, void*, Word flags) {
    const void* table = &gui_load_drawable_methods_0073e324;
    std::memcpy(self, &table, sizeof table);
    if (flags & 1u) live_free(self);
    return self;
}

// ------------------------------------------------------------------ CSWGuiControl
// 0041AA80 ctor: extent zero (+10,+0C,+08,+04; 0041AAA0..A9); vptr 0073E488
// (0041AAB4); children list(0) (0041AABA) [state 1]; CExoString +28 (0041AAC7)
// [state 2]; event list(0) +38 (0041AAD5); flags (x & 0xFFFFFF8E) | 0x0A
// (0041AADA..E7); +34,+58,+54b,+14,+24,+30 = 0; id = -1; +55b = 1 (0041AAEA..B03).
Control* __fastcall gui_load_control_construct_0041aa80(Control* self, void*) noexcept(false) {
    self->extent.height = 0; self->extent.width = 0; self->extent.top = 0; self->extent.left = 0;
    set_object_vptr(self, &gui_load_control_methods_0073e488);
    try {
        exo_array_list_construct_00416770(&self->children, nullptr, 0);
        try {
            new (self->text28) String();
            try {
                gui_load_event_list_construct_0041a870(&self->events, nullptr, 0);
            } catch (...) { as_string(self->text28)->~String(); throw; }
        } catch (...) { exo_array_list_destruct_0056b6f0(&self->children, nullptr); throw; }
    } catch (...) { set_object_vptr(self, &gui_load_object_methods_0073df20); throw; }
    self->flags = (self->flags & 0xFFFFFF8Eu) | 0x0Au;
    self->owner = nullptr;
    self->u58 = 0;
    self->b54 = 0;
    self->parent = nullptr;
    self->u24 = 0;
    self->u30 = 0;
    self->id = -1;
    self->b55 = 1;
    return self;
}
// 00419A20 dtor: vptr 0073E488; events list dtor (00419A4E); CExoString (00419A5B);
// children list dtor (00419A68); vptr 0073DF20 (00419A71).
void __fastcall gui_load_control_destruct_00419a20(Control* self, void*) {
    set_object_vptr(self, &gui_load_control_methods_0073e488);
    exo_array_list_destruct_0056b6f0(&self->events, nullptr);
    as_string(self->text28)->~String();
    exo_array_list_destruct_0056b6f0(&self->children, nullptr);
    set_object_vptr(self, &gui_load_object_methods_0073df20);
}
void* __fastcall gui_load_control_deleting_destruct_00419cc0(Control* self, void*, Word flags) {
    gui_load_control_destruct_00419a20(self, nullptr);                  // 00419CC3
    if (flags & 1u) live_free(self);                                    // 00419CC8..D0
    return self;
}

// 0041B8B0 CSWGuiNavigable ctor: control ctor; vptr 0073E520; +5C..+68 = 0; flags |= 4.
Navigable* __fastcall gui_load_navigable_construct_0041b8b0(Navigable* self, void*) noexcept(false) {
    gui_load_control_construct_0041aa80(self, nullptr);
    set_object_vptr(self, &gui_load_navigable_methods_0073e520);
    self->move_to[0] = 0; self->move_to[1] = 0; self->move_to[2] = 0; self->move_to[3] = 0;
    self->flags |= 4u;
    return self;
}
// 00419CE0 dtor: vptr 0073E520; tail jump 00419A20.
void __fastcall gui_load_navigable_destruct_00419ce0(Navigable* self, void*) {
    set_object_vptr(self, &gui_load_navigable_methods_0073e520);
    gui_load_control_destruct_00419a20(self, nullptr);
}
void* __fastcall gui_load_navigable_deleting_destruct_00419cf0(Navigable* self, void*, Word flags) {
    gui_load_navigable_destruct_00419ce0(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// 0041ACD0 CSWGuiLabel ctor: control ctor; vptr 0073E5B8; border +5C [state 0];
// text +D0 [state 1]; flags &= ~4 (0041AD18..22).
Label* __fastcall gui_load_label_construct_0041acd0(Label* self, void*) noexcept(false) {
    gui_load_control_construct_0041aa80(self, nullptr);
    set_object_vptr(self, &gui_load_label_methods_0073e5b8);
    try {
        gui_load_border_construct_004167f0(&self->border, nullptr);
        try {
            gui_load_text_construct_00417280(&self->text, nullptr);
        } catch (...) { gui_load_border_destruct_004151c0(&self->border, nullptr); throw; }
    } catch (...) { gui_load_control_destruct_00419a20(self, nullptr); throw; }
    self->flags &= 0xFFFFFFFBu;
    return self;
}
// 00419D10 dtor: vptr 0073E5B8; text dtor; border dtor; control dtor.
void __fastcall gui_load_label_destruct_00419d10(Label* self, void*) {
    set_object_vptr(self, &gui_load_label_methods_0073e5b8);
    gui_load_text_destruct_00415ff0(&self->text, nullptr);
    gui_load_border_destruct_004151c0(&self->border, nullptr);
    gui_load_control_destruct_00419a20(self, nullptr);
}
void* __fastcall gui_load_label_deleting_destruct_00419d80(Label* self, void*, Word flags) {
    gui_load_label_destruct_00419d10(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// 0041C0F0 CSWGuiButton ctor: navigable ctor; vptr 0073E658; border +6C [0];
// hilight +E0 [1]; text +154 [2].
Button* __fastcall gui_load_button_construct_0041c0f0(Button* self, void*) noexcept(false) {
    gui_load_navigable_construct_0041b8b0(self, nullptr);
    set_object_vptr(self, &gui_load_button_methods_0073e658);
    try {
        gui_load_border_construct_004167f0(&self->border, nullptr);
        try {
            gui_load_border_construct_004167f0(&self->hilight, nullptr);
            try {
                gui_load_text_construct_00417280(&self->text, nullptr);
            } catch (...) { gui_load_border_destruct_004151c0(&self->hilight, nullptr); throw; }
        } catch (...) { gui_load_border_destruct_004151c0(&self->border, nullptr); throw; }
    } catch (...) { gui_load_navigable_destruct_00419ce0(self, nullptr); throw; }
    return self;
}
// 00419DA0 dtor: vptr 0073E658; text, hilight, border dtors; vptr 0073E520 (inlined
// navigable dtor, 00419DFD); control dtor.
void __fastcall gui_load_button_destruct_00419da0(Button* self, void*) {
    set_object_vptr(self, &gui_load_button_methods_0073e658);
    gui_load_text_destruct_00415ff0(&self->text, nullptr);
    gui_load_border_destruct_004151c0(&self->hilight, nullptr);
    gui_load_border_destruct_004151c0(&self->border, nullptr);
    set_object_vptr(self, &gui_load_navigable_methods_0073e520);
    gui_load_control_destruct_00419a20(self, nullptr);
}
void* __fastcall gui_load_button_deleting_destruct_00419e20(Button* self, void*, Word flags) {
    gui_load_button_destruct_00419da0(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// 0067AC50 CSWGui3DSceneView dtor: vptr 00752E30; CSWGuiScene dtor +5C (0067AC7E);
// control dtor (0067AC8D).
void __fastcall gui_load_scene_view_destruct_0067ac50(Control* self, void*) {
    set_object_vptr(self, &gui_load_scene_view_methods_00752e30);
    gui_scene_destruct_004165e0(reinterpret_cast<unsigned char*>(self) + 0x5C, nullptr);
    gui_load_control_destruct_00419a20(self, nullptr);
}
void* __fastcall gui_load_scene_view_deleting_destruct_0067acc0(Control* self, void*, Word flags) {
    gui_load_scene_view_destruct_0067ac50(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// 0041AC70 HandleRMouseUp: manager mouse-over control equal to this or whose
// GetSelectableParent is this, and flags bit3 -> vslot +3C(0x44, 1)
// (0041AC70..9A); ReleaseMouse(this) (0041ACA4); tail vslot +14 (0041ACAE).
void __fastcall gui_load_control_handle_rmouse_up_0041ac70(Object* object, void*) {
    Control* self = static_cast<Control*>(object);
    kih::Control* over = gui_manager()->mouse_over;
    if (over) {
        bool mine = reinterpret_cast<Object*>(over) == object;
        if (!mine) mine = reinterpret_cast<Object*>(over->GetSelectableParent()) == object;
        if (mine && (self->flags & 8u)) self->vtbl->handle_input_event(self, nullptr, 0x44, 1);
    }
    gui_manager()->ReleaseMouse(input_view(self));
    self->vtbl->release_capture(self, nullptr);
}
// 00418960 HandleFocusChange(flag) RET 4: flag and owner -> owner vslot +28; panel ->
// (flags bit0 clear -> PlayGuiSound(1)) then panel vslot +08(this, 0).
void __fastcall gui_load_control_focus_change_00418960(Object* object, void*, Word flag) {
    Control* self = static_cast<Control*>(object);
    if (!flag) return;
    Object* owner = self->owner;
    if (!owner) return;
    Panel* panel = owner->vtbl->as_panel(owner, nullptr);
    if (!panel) return;
    if (!(self->flags & 1u)) gui_manager()->PlayGuiSound(1);
    panel->vtbl->set_active_control(panel, nullptr, self, 0);
}
// 0041AC30 HandleRMouseDown: vslot +80 nonzero -> CaptureMouse(this, 4); else
// parent = GetSelectableParent: nonzero -> tail parent vslot +84; else capture.
void __fastcall gui_load_control_rmouse_down_0041ac30(Object* object, void*) {
    if (!object->vtbl->is_selectable(object, nullptr)) {
        kih::Control* parent = input_view(object)->GetSelectableParent();
        if (parent) {
            Object* target = reinterpret_cast<Object*>(parent);
            target->vtbl->rmouse_down(target, nullptr);
            return;
        }
    }
    gui_manager()->CaptureMouse(input_view(object), 4);
}
// 004176A0 SetEnabled(e) RET 4: flags bit3 = bit0 of e (004176A4..B5).
void __fastcall gui_load_control_set_enabled_004176a0(Object* object, void*, Word enabled) {
    Control* self = static_cast<Control*>(object);
    self->flags = self->flags ^ (((enabled << 3) ^ self->flags) & 8u);
}

// 00417780 CSWGuiLabel::SetExtent: copy to +04; border vslot +04(&+04);
// GetInnerExtent(border) (004177B9); text vtable read then vslot +04(&inner).
void __fastcall gui_load_label_set_extent_00417780(Object* object, void*, const Extent* extent) {
    Label* self = static_cast<Label*>(object);
    self->extent = *extent;
    self->border.vtbl->set_extent(&self->border, nullptr, &self->extent);
    Extent inner;
    gui_load_border_inner_extent_00415240(&self->border, nullptr, &inner);
    self->text.vtbl->set_extent(&self->text, nullptr, &inner);
}
// 00417A20 CSWGuiButton::SetExtent: copy to +04; border(+6C) and hilight(+E0)
// vslot +04 with the ARGUMENT extent (00417A4E/60); text vtable read early
// (00417A66); inner(hilight), inner(border), Intersection(out, border, hilight)
// (00417A79..91); text vslot +04(&out) (00417A9C).
void __fastcall gui_load_button_set_extent_00417a20(Object* object, void*, const Extent* extent) {
    Button* self = static_cast<Button*>(object);
    self->extent = *extent;
    self->border.vtbl->set_extent(&self->border, nullptr, extent);
    self->hilight.vtbl->set_extent(&self->hilight, nullptr, extent);
    const TextVtbl* text_vtbl = self->text.vtbl;
    Extent hilight_inner, border_inner, out;
    gui_load_border_inner_extent_00415240(&self->hilight, nullptr, &hilight_inner);
    gui_load_border_inner_extent_00415240(&self->border, nullptr, &border_inner);
    gui_load_extent_intersection_0040a4a0(&out, &border_inner, &hilight_inner);
    text_vtbl->set_extent(&self->text, nullptr, &out);
}
// 00418380 CSWGui3DSceneView::SetExtent: scene(+5C) vslot +04(arg) then copy arg to +04.
void __fastcall gui_load_scene_view_set_extent_00418380(Object* object, void*, const Extent* extent) {
    void* scene = reinterpret_cast<unsigned char*>(object) + 0x5C;
    const SceneVtbl* vt;
    std::memcpy(&vt, scene, sizeof vt);
    vt->set_extent(scene, nullptr, extent);
    object->extent = *extent;
}

// ------------------------------------------------------------------ control events
// 0041A870 ctor(n) RET 4: size, capacity, data = 0; n -> Allocate(n).
kotor_reconstruction::ArrayMutations* __fastcall gui_load_event_list_construct_0041a870(
    kotor_reconstruction::ArrayMutations* self, void*, Word count) noexcept(false) {
    self->size = 0; self->capacity = 0; self->data = 0;
    if (count) gui_load_event_list_allocate_004186e0(self, nullptr, count);
    return self;
}
// 004186E0 Allocate(n) RET 4: capacity = n; new(n*12) stored before copying the
// live size entries (3 dwords each); old data freed (006FA390).
void __fastcall gui_load_event_list_allocate_004186e0(kotor_reconstruction::ArrayMutations* self, void*, Word count) noexcept(false) {
    self->capacity = static_cast<std::int32_t>(count);
    const Word old = self->data;
    self->data = reinterpret_cast<Word>(c2da_directory_new_006fa7e6(count * 12u));
    for (std::int32_t i = 0; i < self->size; ++i) {
        volatile Word* to = reinterpret_cast<volatile Word*>(self->data + static_cast<Word>(i) * 12u);
        const volatile Word* from = reinterpret_cast<const volatile Word*>(old + static_cast<Word>(i) * 12u);
        to[0] = from[0]; to[1] = from[1]; to[2] = from[2];
    }
    if (old) live_free(reinterpret_cast<void*>(old));
}
// 0041A8A0 Add(event by value) RET 0Ch: grow to capacity*2 or 0x10; store; ++size.
void __fastcall gui_load_event_list_add_0041a8a0(kotor_reconstruction::ArrayMutations* self, void*, ControlEvent event) noexcept(false) {
    const Word capacity = static_cast<Word>(self->capacity);
    if (static_cast<Word>(self->size) == capacity)
        gui_load_event_list_allocate_004186e0(self, nullptr, capacity ? capacity + capacity : 0x10u);
    Word words[3];
    std::memcpy(words, &event, 12);
    volatile Word* to = reinterpret_cast<volatile Word*>(self->data + static_cast<Word>(self->size) * 12u);
    to[0] = words[0]; to[1] = words[1]; to[2] = words[2];
    self->size = static_cast<std::int32_t>(static_cast<Word>(self->size) + 1u);
}
// 0048DDE0 deleteAt(i) RET 4 (12-byte elements): size decremented first; elements
// i+1.. shifted down while i < live size.
void __fastcall gui_load_list12_delete_at_0048dde0(kotor_reconstruction::ArrayMutations* self, void*, I32 index) {
    const I32 count = self->size - 1;
    self->size = count;
    for (I32 i = index; i < self->size; ++i) {
        volatile Word* at = reinterpret_cast<volatile Word*>(self->data + static_cast<Word>(i) * 12u);
        at[0] = at[3]; at[1] = at[4]; at[2] = at[5];
    }
}
// 0041AB20 AddEvent(id, object, func) RET 0Ch: search id at +8 of entries
// (0041AB22..41); found: func nonzero -> object/func replaced (0041AB72..78),
// zero -> deleteAt (0041AB84); not found: func nonzero -> Add({object,func,id}).
void __fastcall gui_load_control_add_event_0041ab20(Control* self, void*, I32 id,
    kih::GuiObject* object, EventFunc func) noexcept(false) {
    const I32 count = self->events.size;
    I32 index = 0;
    for (; index < count; ++index) {
        ControlEvent* entry = reinterpret_cast<ControlEvent*>(self->events.data + static_cast<Word>(index) * 12u);
        if (entry->id == id) {
            if (func) { entry->object = object; entry->func = func; }
            else gui_load_list12_delete_at_0048dde0(&self->events, nullptr, index);
            return;
        }
    }
    if (!func) return;
    ControlEvent event;
    event.object = object; event.func = func; event.id = id;
    gui_load_event_list_add_0041a8a0(&self->events, nullptr, event);
}

// ------------------------------------------------------------------ CSWGuiBorder
// 004167F0 ctor: extent zero; vptr 0073E338 [state 0]; color local {1,1,1}; three
// default CResRef temporaries (00416855..69); BorderParams ctor(+14, r, r, r, 0, 0,
// 1.0f, &color, 0) (00416871); +68/+6C/+70 = 0 (00416879..7F); SetBorder(+14, this)
// (00416882).
Border* __fastcall gui_load_border_construct_004167f0(Border* border, void*) noexcept(false) {
    BorderObject* self = border_view(border);
    self->extent.height = 0; self->extent.width = 0; self->extent.top = 0; self->extent.left = 0;
    self->vtbl = table_view<BorderVtbl>(&gui_load_border_methods_0073e338);
    try {
        const Vector color = { 1.0f, 1.0f, 1.0f };
        Ref first, second, third;
        c2da_default_ref_00405ed0(&first, nullptr);
        c2da_default_ref_00405ed0(&second, nullptr);
        c2da_default_ref_00405ed0(&third, nullptr);
        gui_load_border_params_construct_00415000(&self->params, nullptr, &third, &second, &first,
                                                  0, 0, 1.0f, &color, 0);
        self->corner = nullptr; self->edge = nullptr; self->fill = nullptr;
        gui_load_border_params_set_border_00414c90(&self->params, nullptr, border);
    } catch (...) {
        const void* base = &gui_load_drawable_methods_0073e324;
        std::memcpy(border, &base, sizeof base);
        throw;
    }
    return border;
}
// 004151C0 dtor: vptr 0073E338; images +68/+6C/+70: nonzero -> vslot +04(1) then 0;
// vptr 0073E324.
void __fastcall gui_load_border_destruct_004151c0(Border* border, void*) {
    BorderObject* self = border_view(border);
    self->vtbl = table_view<BorderVtbl>(&gui_load_border_methods_0073e338);
    if (AurImage* image = self->corner) { image->vtbl->release(image, nullptr, 1); self->corner = nullptr; }
    if (AurImage* image = self->edge) { image->vtbl->release(image, nullptr, 1); self->edge = nullptr; }
    if (AurImage* image = self->fill) { image->vtbl->release(image, nullptr, 1); self->fill = nullptr; }
    const void* base = &gui_load_drawable_methods_0073e324;
    std::memcpy(border, &base, sizeof base);
}
void* __fastcall gui_load_border_deleting_destruct_004168a0(Border* self, void*, Word flags) {
    gui_load_border_destruct_004151c0(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// ------------------------------------------------------------------ CSWGuiText
// 00417280 ctor: extent zero; vptr 0073E3A4; TextParams ctor +18 [state 0];
// +14 = 0 (004172C9) [state 1]; SetTextObject(+18, this) (004172CC); +6C = 0x190.
Text* __fastcall gui_load_text_construct_00417280(Text* text, void*) noexcept(false) {
    TextObject* self = text_view(text);
    self->extent.height = 0; self->extent.width = 0; self->extent.top = 0; self->extent.left = 0;
    self->vtbl = table_view<TextVtbl>(&gui_load_text_methods_0073e3a4);
    try {
        gui_load_text_params_construct_00417050(&self->params, nullptr);
        try {
            self->string = nullptr;
            gui_load_text_params_set_text_object_00415f70(&self->params, nullptr, text);
        } catch (...) { as_string(self->params.text)->~String(); throw; }
    } catch (...) {
        const void* base = &gui_load_drawable_methods_0073e324;
        std::memcpy(text, &base, sizeof base);
        throw;
    }
    self->params.u54 = 0x190;
    return text;
}
// 00415FF0 dtor: vptr 0073E3A4; string nonzero -> vslot +04(1) (not cleared);
// CExoString +18 dtor; vptr 0073E324.
void __fastcall gui_load_text_destruct_00415ff0(Text* text, void*) {
    TextObject* self = text_view(text);
    self->vtbl = table_view<TextVtbl>(&gui_load_text_methods_0073e3a4);
    if (AurString* string = self->string) string->vtbl->release(string, nullptr, 1);
    as_string(self->params.text)->~String();
    const void* base = &gui_load_drawable_methods_0073e324;
    std::memcpy(text, &base, sizeof base);
}
void* __fastcall gui_load_text_deleting_destruct_004172f0(Text* self, void*, Word flags) {
    gui_load_text_destruct_00415ff0(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// 00414EB0 CSWGuiText::GetIdealHeight: no string -> 0; font height (vslot +48) < 0x10
// (signed) -> vslot +50(-1) + 1, else vslot +50(-1) (string re-read, 00414EC2).
Word __fastcall gui_load_text_ideal_height_00414eb0(Text* text, void*) {
    TextObject* self = text_view(text);
    AurString* string = self->string;
    if (!string) return 0;
    const I32 font = string->vtbl->font_height(string, nullptr);
    AurString* again = self->string;
    if (font < 0x10) return static_cast<Word>(again->vtbl->text_height(again, nullptr, -1) + 1);
    return static_cast<Word>(again->vtbl->text_height(again, nullptr, -1));
}

// ------------------------------------------------------------------ CSWGuiPanel
// 0040B570 ctor(manager) RET 4: extent zero; vptr 0073E010; controls list(0)
// [state 1]; color zero (0040B5B8..BE); new(0x74) -> CSWGuiBorder ctor (freed on
// throw) -> +5C; flags (x & 0xFFFFF89C) | 0x84; +18 = manager; +1C,+2C = 0;
// +48b = 0; alpha 1.0; +60b = 0xFF; +14 = 1.
Panel* __fastcall gui_load_panel_construct_0040b570(Panel* self, void*, Object* manager) noexcept(false) {
    self->extent.height = 0; self->extent.width = 0; self->extent.top = 0; self->extent.left = 0;
    set_object_vptr(self, &gui_load_panel_methods_0073e010);
    try {
        exo_array_list_construct_00416770(&self->controls, nullptr, 0);
        try {
            self->color.x = 0.0f; self->color.y = 0.0f; self->color.z = 0.0f;
            void* memory = c2da_directory_new_006fa7e6(0x74);
            Border* border = nullptr;
            if (memory) {
                try {
                    border = gui_load_border_construct_004167f0(static_cast<Border*>(memory), nullptr);
                } catch (...) { live_free(memory); throw; }
            }
            const Word flags = self->flags;
            self->background = border;
            self->manager = manager;
            self->active = nullptr;
            self->layout = nullptr;
            self->b48 = 0;
            self->flags = (flags & 0xFFFFF89Cu) | 0x84u;
            self->alpha = 1.0f;
            self->b60 = 0xFF;
            self->u14 = 1;
        } catch (...) { exo_array_list_destruct_0056b6f0(&self->controls, nullptr); throw; }
    } catch (...) { set_object_vptr(self, &gui_load_object_methods_0073df20); throw; }
    return self;
}
// 0040CF70 dtor: vptr 0073E010; background vslot +00(1) then 0; StopLoadFromLayout;
// layout vslot +00(1) through lane-gff's 0073E2D0 type, then 0; manager nonzero and
// PanelExists -> RemovePanel; controls list dtor; vptr 0073DF20.
void __fastcall gui_load_panel_destruct_0040cf70(Panel* self, void*) {
    set_object_vptr(self, &gui_load_panel_methods_0073e010);
    if (Border* border = self->background) border->vtbl->deleting_destroy(border, nullptr, 1);
    self->background = nullptr;
    gui_load_panel_stop_layout_0040b8f0(self, nullptr);
    if (Gff* layout = self->layout) {
        const gff_resource::Methods0073e2d0* table;
        std::memcpy(&table, layout, sizeof table);
        (void)table->deleting_destroy_00413010(layout, nullptr, 1u);
    }
    Object* manager = self->manager;
    self->layout = nullptr;
    if (manager && frame_ContainsPanel_0040bd70(manager, nullptr, self))         // 0040CFD5
        frame_RemovePanel_0040c830(manager, nullptr, self);                      // 0040CFE1
    exo_array_list_destruct_0056b6f0(&self->controls, nullptr);
    set_object_vptr(self, &gui_load_object_methods_0073df20);
}
void* __fastcall gui_load_panel_deleting_destruct_0040d010(Panel* self, void*, Word flags) {
    gui_load_panel_destruct_0040cf70(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}
// 0040A5A0 SetExtent: copy to +04; background vslot +04({0,0,w,h}).
void __fastcall gui_load_panel_set_extent_0040a5a0(Object* object, void*, const Extent* extent) {
    Panel* self = static_cast<Panel*>(object);
    Border* background = self->background;
    self->extent = *extent;
    const Extent local = { 0, 0, extent->width, extent->height };
    background->vtbl->set_extent(background, nullptr, &local);
}
// 0040A630 SetActiveControl(control, sound) RET 8.
void __fastcall gui_load_panel_set_active_control_0040a630(Object* object, void*, Object* control, Word sound) {
    Panel* self = static_cast<Panel*>(object);
    Control* current = self->active;
    if (current == control) return;
    if (current) current->vtbl->handle_input_event(current, nullptr, 1, 1);    // 0040A649
    self->active = static_cast<Control*>(control);
    if (!control) return;
    control->vtbl->handle_input_event(control, nullptr, 0, 1);                   // 0040A65B
    if (!sound || !self->manager) return;
    reinterpret_cast<kih::Manager*>(self->manager)->PlayGuiSound(
        static_cast<char>(static_cast<Control*>(control)->b55));                // 0040A66F..73
}
// 00409E60 HandleInputEvent: active control -> tail vslot +3C (args unchanged).
void __fastcall gui_load_panel_handle_input_00409e60(Object* object, void*, I32 event, I32 value) {
    Control* active = static_cast<Panel*>(object)->active;
    if (active) active->vtbl->handle_input_event(active, nullptr, event, value);
}
Object* __fastcall gui_load_panel_active_control_0040b630(Object* object, void*) {   // 0040B630
    return static_cast<Panel*>(object)->active;
}
// 0040B870 OnPanelAdded (transcribed 0040B873..EA).
void __fastcall gui_load_panel_on_added_0040b870(Panel* self, void*) {
    table_view<PanelVtbl>(self->vtbl)->reset_font(self, nullptr);
    gui_manager()->UpdateMouseOverControl();
    kih::Manager* manager = reinterpret_cast<kih::Manager*>(self->manager);
    Object* over = reinterpret_cast<Object*>(manager->mouse_over);
    if (!over) return;
    const I32 count = self->controls.size;
    I32 i = 0;
    for (; i < count; ++i)
        if (*reinterpret_cast<Object* volatile*>(self->controls.data + static_cast<Word>(i) * 4u) == over) break;
    if (i >= count) return;
    Control* control = static_cast<Control*>(over);
    if (!(control->flags & 8u)) return;
    manager->mouse_over = reinterpret_cast<kih::Control*>(control);
    control->vtbl->handle_mouse_over(control, nullptr, 1);
    Control* active = self->active;
    if (!active || control == active) return;
    if (!control->vtbl->slot58(control, nullptr)) return;
    if (!(control->flags & 4u)) return;
    self->active->vtbl->handle_input_event(self->active, nullptr, 1, 1);
}
// 0040B640..0040B680 OnA/B/X/Y/BlackButtonPressed RET 4: vslot +3C(0x27..0x2B, 1).
void __fastcall gui_load_panel_button_a_0040b640(Object* o, void*, Word) { o->vtbl->handle_input_event(o, nullptr, 0x27, 1); }
void __fastcall gui_load_panel_button_b_0040b650(Object* o, void*, Word) { o->vtbl->handle_input_event(o, nullptr, 0x28, 1); }
void __fastcall gui_load_panel_button_x_0040b660(Object* o, void*, Word) { o->vtbl->handle_input_event(o, nullptr, 0x29, 1); }
void __fastcall gui_load_panel_button_y_0040b670(Object* o, void*, Word) { o->vtbl->handle_input_event(o, nullptr, 0x2A, 1); }
void __fastcall gui_load_panel_button_black_0040b680(Object* o, void*, Word) { o->vtbl->handle_input_event(o, nullptr, 0x2B, 1); }

// 0040A3E0 CSWGuiManager::GetScreenResolutionString(out, w, h) __cdecl: constructs
// out from 0073DFA8/9C/90/84 or 0073DF7C per the width/height tests.
String* __cdecl gui_load_manager_resolution_string_0040a3e0(String* out, I32 width, I32 height) noexcept(false) {
    const char* text = "800x600";                                               // 0073DF7C
    if (width == 0x400) { if (height == 0x300) text = "1024x768"; }             // 0073DFA8
    else if (width == 0x500) {
        if (height == 0x3C0) text = "1280x960";                                 // 0073DF9C
        else if (height == 0x400) text = "1280x1024";                           // 0073DF90
    } else if (width == 0x640) { if (height == 0x4B0) text = "1600x1200"; }     // 0073DF84
    new (out) String(text);
    return out;
}
// 0040A900 GetFullScreenBG(out) RET 4: res = resolution(width 0078D1D4, height
// 0078D1D8) (0040A935); tmp = res + suffix[+48] (0040A95A); res = tmp (0040A969);
// ~tmp; out = copy(res) (0040A987); ~res.
String* __fastcall gui_load_panel_full_screen_bg_0040a900(Panel* self, void*, String* out) noexcept(false) {
    alignas(4) unsigned char res_bytes[8];
    String* res = gui_load_manager_resolution_string_0040a3e0(
        reinterpret_cast<String*>(res_bytes),
        static_cast<I32>(kotor_renderer_gl_cells::width_0078d1d4),
        static_cast<I32>(kotor_menu_window_cells::height_0078d1d8));
    try {
        alignas(4) unsigned char tmp_bytes[8];
        String* tmp = GuiConcat005e5d10(res, nullptr, reinterpret_cast<String*>(tmp_bytes),
                                        reinterpret_cast<String*>(kotor_cells_gui_suffix_007a3d50 + static_cast<Word>(self->b48) * 8u));
        try { res->Assign005e5c50(tmp); } catch (...) { tmp->~String(); throw; }
        tmp->~String();
        WindowCopy005e5b00(out, nullptr, res);
    } catch (...) { res->~String(); throw; }
    res->~String();
    return out;
}
// 0040A9C0 ResetFont: +14 nonzero -> each nonnull control vslot +8C (count re-read).
void __fastcall gui_load_panel_reset_font_0040a9c0(Panel* self, void*) {
    if (!self->u14) return;
    for (I32 i = 0; i < self->controls.size; ++i) {
        Object* control = *reinterpret_cast<Object* volatile*>(self->controls.data + static_cast<Word>(i) * 4u);
        if (control) control->vtbl->reset_font(control, nullptr);
    }
}
// 0040AA00 GetExtentAccountingForPanelOffset(out) RET 4 (EAX not meaningful).
void __fastcall gui_load_panel_offset_extent_0040aa00(Panel* self, void*, Extent* out) {
    *out = self->extent;
    const unsigned char* manager = reinterpret_cast<const unsigned char*>(self->manager);
    auto half = [](I32 v) { return v / 2; };
    if (self->flags & 8u) {
        std::int16_t mw; std::memcpy(&mw, manager + 0x6C, 2);
        out->left += half(static_cast<I32>(mw) - out->width);
        std::int16_t mh; std::memcpy(&mh, reinterpret_cast<const unsigned char*>(self->manager) + 0x6E, 2);
        out->top += half(static_cast<I32>(mh) - out->height);
        return;
    }
    if (self->flags & 0x20u) {
        std::int16_t mw; std::memcpy(&mw, manager + 0x6C, 2);
        out->left += half(static_cast<I32>(mw) - 0x280);
    }
    if (self->flags & 0x40u) {
        std::int16_t mh; std::memcpy(&mh, reinterpret_cast<const unsigned char*>(self->manager) + 0x6E, 2);
        out->top += half(static_cast<I32>(mh) - 0x1E0);
    }
}
// 0040BA20 GetLocalMouseCoords(x*, y*) RET 8.
void __fastcall gui_load_panel_local_mouse_0040ba20(Object* object, void*, I32* x, I32* y) {
    Panel* self = static_cast<Panel*>(object);
    *x = gui_manager()->mouse_x;
    *y = gui_manager()->mouse_y;
    Extent local = self->extent;
    if (!(self->flags & 1u)) {
        *x += -((static_cast<I32>(kotor_renderer_gl_cells::width_0078d1d4) - 0x280) / 2);
        *y += -((static_cast<I32>(kotor_menu_window_cells::height_0078d1d8) - 0x1E0) / 2);
        *x -= local.left;
        *y -= local.top;
        return;
    }
    gui_load_panel_offset_extent_0040aa00(self, nullptr, &local);
    *x -= local.left;
    *y -= local.top;
}

} // extern "C"
