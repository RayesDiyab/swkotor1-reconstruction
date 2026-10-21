// Lane gui-load: faithful bodies for the main-menu layout load chain.
// Each function cites the original instructions that establish its behavior.
#include "27c7999c0c-gui_control_load.h"
#include <cstring>
#include "47d8056e59-gff_resource.h"   // lane-gff: Methods0073e2d0 = CResGFF vtable 0073E2D0 typed slots (D1)

extern "C" {
// M40-provided CExoString helpers (window-native-strings.obj, task10-canonical-ref-string-carriers.obj).
char* __fastcall WindowCStr005e5670(gui_load::String*, void*);
int __fastcall async_string_equal_005e5200(gui_load::String*, void*, gui_load::String*);
}

using namespace gui_load;


extern "C" {

// 00671C00 CExoArrayList<T*>::Add: if size==capacity grow to capacity*2 or 0x10
// via 005E03C0 (00671C03..1B); store at data[size] (00671C20..29); INC size
// re-reads the live word (00671C2C).
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations* self, void*, Word value) {
    const Word capacity = static_cast<Word>(self->capacity);
    if (static_cast<Word>(self->size) == capacity)
        menu_list_growth::allocate_005e03c0(self, nullptr, capacity ? capacity + capacity : 0x10u);
    const Word index = static_cast<Word>(self->size);
    *reinterpret_cast<volatile Word*>(self->data + (index << 2)) = value;
    self->size = static_cast<std::int32_t>(static_cast<Word>(self->size) + 1u);
}

// 00417660 SetMoveToControl(int dir, ptr): store only if 0<=dir<4 (signed, 00417664..6B).
void __fastcall gui_load_nav_set_move_to_00417660(Navigable* self, void*, std::int32_t direction, Word value) {
    if (direction < 0 || direction >= 4) return;
    self->move_to[direction] = value;
}

// 00417680 GetMoveToControl(int dir): 0 outside [0,4) (00417684..96).
Word __fastcall gui_load_nav_get_move_to_00417680(Navigable* self, void*, std::int32_t direction) {
    if (direction < 0 || direction >= 4) return 0;
    return self->move_to[direction];
}

// 0040BA00 GetControl(int): only the upper signed bound is checked (0040BA04 JGE);
// negative indices are not rejected.
Control* __fastcall gui_load_panel_get_control_0040ba00(Panel* self, void*, std::int32_t index) {
    if (index >= static_cast<std::int32_t>(self->controls.size)) return nullptr;
    const Word address = self->controls.data + static_cast<Word>(index) * 4u;
    return *reinterpret_cast<Control* volatile*>(address);
}

// 0041ACC0 AddChildControl: ADD ECX,18h; JMP 00671C00 (children list at +18).
void __fastcall gui_load_control_add_child_0041acc0(Control* self, void*, Control* child) {
    gui_load_array_add_00671c00(&self->children, nullptr,
        static_cast<Word>(reinterpret_cast<std::uintptr_t>(child)));
}

// 00404B00 SetPanelAlpha(float): !(a < 1.0) incl. NaN/equal -> 1.0 (00404B04..0F, 3D);
// a <= 0.0 -> 0 (00404B15..20, 47); else store the raw argument dword (00404B33..37).
void __fastcall gui_load_panel_set_alpha_00404b00(Panel* self, void*, float alpha) {
    if (!(alpha < 1.0f)) { const Word one = 0x3F800000u; std::memcpy(&self->alpha, &one, 4); return; }
    if (alpha <= 0.0f) { const Word zero = 0; std::memcpy(&self->alpha, &zero, 4); return; }
    std::memcpy(&self->alpha, &alpha, 4);
}

// 0040A890 (unnamed in export; resolves MOVETO indices to control pointers):
// count captured once (0040A896); per control vslot +4C (0040A8B0); for d=0..3
// idx=GetMoveTo(d); target = (0<=idx<count) ? controls[idx] : 0 (0040A8C8..DB);
// SetMoveTo(d,target) (0040A8E0).
void __fastcall gui_load_panel_resolve_move_to_0040a890(Panel* self, void*) {
    const std::int32_t count = self->controls.size;
    for (std::int32_t i = 0; i < count; ++i) {
        Control* control = *reinterpret_cast<Control* volatile*>(self->controls.data + static_cast<Word>(i) * 4u);
        if (!control) continue;
        Navigable* navigable = control->vtbl->as_navigable(control, nullptr);
        if (!navigable) continue;
        for (std::int32_t direction = 0; direction < 4; ++direction) {
            const std::int32_t index = static_cast<std::int32_t>(
                gui_load_nav_get_move_to_00417680(navigable, nullptr, direction));
            Word target = 0;
            if (index >= 0 && index < count)
                target = *reinterpret_cast<volatile Word*>(self->controls.data + static_cast<Word>(index) * 4u);
            gui_load_nav_set_move_to_00417660(navigable, nullptr, direction, target);
        }
    }
}

// 00409DC0 CSWGuiExtent::Load(gff, struct): GetStructFromStruct "EXTENT" (00409DD9);
// on success ReadFieldINT LEFT/TOP/WIDTH/HEIGHT default 0 into +0/+4/+8/+C.
void __fastcall gui_load_extent_load_00409dc0(Extent* self, void*, Gff* gff, Struct* s) {
    Struct extent;
    if (!gui_load_gff_struct_00411a10(gff, nullptr, &extent, s, "EXTENT")) return;
    Word success;
    self->left = gui_load_gff_int_00411c90(gff, nullptr, &extent, "LEFT", &success, 0);
    self->top = gui_load_gff_int_00411c90(gff, nullptr, &extent, "TOP", &success, 0);
    self->width = gui_load_gff_int_00411c90(gff, nullptr, &extent, "WIDTH", &success, 0);
    self->height = gui_load_gff_int_00411c90(gff, nullptr, &extent, "HEIGHT", &success, 0);
}

// 0041B8E0 CSWGuiControl::Load(gff, struct): Extent::Load (0041B8F5); vslot +04
// SetExtent(&extent) (0041B8FF); ID default -1 -> +50 (0041B911/25);
// Obj_ParentID default -1 (0041B928); if != -1: owner vslot +28 (0041B939); if
// nonzero: parent=GetControl(id) stored +14 (0041B943/4B) and AddChildControl on
// that (possibly null) parent (0041B94E) - no null check in original.
void __fastcall gui_load_control_load_0041b8e0(Control* self, void*, Gff* gff, Struct* s) {
    gui_load_extent_load_00409dc0(&self->extent, nullptr, gff, s);
    self->vtbl->set_extent(self, nullptr, &self->extent);
    Word success;
    self->id = gui_load_gff_int_00411c90(gff, nullptr, s, "ID", &success, -1);
    const std::int32_t parent_id = gui_load_gff_int_00411c90(gff, nullptr, s, "Obj_ParentID", &success, -1);
    if (parent_id == -1) return;
    Object* owner = self->owner;
    Panel* panel = owner->vtbl->as_panel(owner, nullptr);
    if (!panel) return;
    Control* parent = gui_load_panel_get_control_0040ba00(panel, nullptr, parent_id);
    self->parent = parent;
    gui_load_control_add_child_0041acc0(parent, nullptr, self);
}

// 0041C890 CSWGuiNavigable::Load: GetStructFromStruct "MOVETO" (0041C8AA); on success
// LEFT->+60, RIGHT->+68, UP->+5C, DOWN->+64 (default 0, in that call order);
// then CSWGuiControl::Load (0041C923) unconditionally.
void __fastcall gui_load_navigable_load_0041c890(Navigable* self, void*, Gff* gff, Struct* s) {
    Struct move_to;
    if (gui_load_gff_struct_00411a10(gff, nullptr, &move_to, s, "MOVETO")) {
        Word success;
        self->move_to[1] = static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &move_to, "LEFT", &success, 0));
        self->move_to[3] = static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &move_to, "RIGHT", &success, 0));
        self->move_to[0] = static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &move_to, "UP", &success, 0));
        self->move_to[2] = static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &move_to, "DOWN", &success, 0));
    }
    gui_load_control_load_0041b8e0(self, nullptr, gff, s);
}

// 0041B960 CSWGuiLabel::Load: text(+D0) vslot +10 with "TEXT" (0041B97D..B9BD), then
// border(+5C) vslot +10 with "BORDER" (0041B9C2..9F4), then CSWGuiControl::Load.
void __fastcall gui_load_label_load_0041b960(Label* self, void*, Gff* gff, Struct* s) {
    {
        String label("TEXT");
        self->text.vtbl->load(&self->text, nullptr, gff, s, &label);
    }
    {
        String label("BORDER");
        self->border.vtbl->load(&self->border, nullptr, gff, s, &label);
    }
    gui_load_control_load_0041b8e0(self, nullptr, gff, s);
}

// 0041C930 CSWGuiButton::Load: border(+6C) "BORDER", hilight(+E0) "HILIGHT",
// text(+154) "TEXT", each via vslot +10 with its temporary label destroyed
// before the next (0041C94D..CA01); then CSWGuiNavigable::Load (0041CA0A).
void __fastcall gui_load_button_load_0041c930(Button* self, void*, Gff* gff, Struct* s) {
    {
        String label("BORDER");
        self->border.vtbl->load(&self->border, nullptr, gff, s, &label);
    }
    {
        String label("HILIGHT");
        self->hilight.vtbl->load(&self->hilight, nullptr, gff, s, &label);
    }
    {
        String label("TEXT");
        self->text.vtbl->load(&self->text, nullptr, gff, s, &label);
    }
    gui_load_navigable_load_0041c890(self, nullptr, gff, s);
}

// 00418800 CSWGuiControl::Load(owner, gff, struct, CExoString const& label):
// owner -> +34 first (0041880C); CStr (0041880F); GetStructFromStruct (00418825);
// on success vslot +48 Load(gff, &sub) (00418838).
void __fastcall gui_load_control_load_label_00418800(Control* self, void*, Object* owner, Gff* gff,
                                                    Struct* s, String* label) {
    self->owner = owner;
    const char* text = WindowCStr005e5670(label, nullptr);
    Struct sub;
    if (gui_load_gff_struct_00411a10(gff, nullptr, &sub, s, text))
        self->vtbl->load(self, nullptr, gff, &sub);
}

// 00418840 CSWGuiControl::Load(owner, gff, CResList*, CExoString const& tag):
// owner -> +34 (0041886E); count=GetListCount once (00418874); local tag string
// default-constructed (00418881); for i in [0,count) signed: GetListElement
// (004188A1); default "" (004188AF); ReadFieldCExoString "TAG" (004188D4);
// assign (004188E3); destroy temp then default (004188F1/FF); compare to tag
// (0041890C); on match vslot +48 Load(gff,&element) (00418930) and stop.
// Local tag string destroyed at exit (0041893F).
void __fastcall gui_load_control_load_tag_00418840(Control* self, void*, Object* owner, Gff* gff,
                                                  List* list, String* tag) {
    self->owner = owner;
    const std::int32_t count = static_cast<std::int32_t>(gff_list_count_00411940(gff, nullptr, list));
    String value;
    // Original keeps one stack slot [esp+14h] across iterations (004188A1 out); a failed
    // GetListElement leaves it unchanged. Hoisted out of the loop.
    // D2: zero here (00418898..004188A1; the original slot has no initializing store).
    Struct element{};
    for (std::int32_t i = 0; i < count; ++i) {
        gff_list_element_00411990(gff, nullptr, &element, list, static_cast<Word>(i));
        {
            String empty("");
            alignas(4) unsigned char storage[sizeof(String)];
            String* read = reinterpret_cast<String*>(storage);
            Word success;
            String* produced = gui_load_gff_string_00411ec0(gff, nullptr, read, &element, "TAG", &success, &empty);
            value.Assign005e5c50(produced);
            read->~String();
        }
        if (async_string_equal_005e5200(&value, nullptr, tag)) {
            self->vtbl->load(self, nullptr, gff, &element);
            break;
        }
    }
}

// 0040B930 CSWGuiPanel::InitControl(control, CExoString const& tag, int add):
// control->Load(this, layout(+2C), &layout_controls(+30), tag) (0040B93C..48);
// if add: id=control+50; id<0 -> done; id<size -> controls[id]=control;
// id>size -> append nulls while id > size (0040B976..82) then append control;
// id==size -> append control (0040B985..89).
void __fastcall gui_load_panel_init_control_0040b930(Panel* self, void*, Control* control, String* tag,
                                                    std::int32_t add) {
    gui_load_control_load_tag_00418840(control, nullptr, self, self->layout, &self->layout_controls, tag);
    if (!add) return;
    const std::int32_t id = control->id;
    if (id < 0) return;
    if (id < self->controls.size) {
        *reinterpret_cast<Control* volatile*>(self->controls.data + static_cast<Word>(id) * 4u) = control;
        return;
    }
    if (id > self->controls.size) {
        do {
            gui_load_array_add_00671c00(&self->controls, nullptr, 0);
        } while (id > self->controls.size);
    }
    gui_load_array_add_00671c00(&self->controls, nullptr,
        static_cast<Word>(reinterpret_cast<std::uintptr_t>(control)));
}

// 0040B8F0 CSWGuiPanel::StopLoadFromLayout: only if flags bit1 (0040B8F3);
// Release(layout) (0040B8FC); if layout: vslot +00(1) scalar-deleting dtor
// (0040B906..0C); flags re-read and bit1 cleared, layout=0 (0040B90E..1B);
// tail jump 0040A890 (0040B921).
void __fastcall gui_load_panel_stop_layout_0040b8f0(Panel* self, void*) {
    if (!(self->flags & 2u)) return;
    c2da_release_demand_00409b80(self->layout, nullptr);
    Gff* layout = self->layout;
    if (layout) {
        // vptr read from CRes +00 (0040B908); slot +00 = 00413010 typed by lane-gff (D1).
        const gff_resource::Methods0073e2d0* table;
        std::memcpy(&table, layout, sizeof table);
        (void)table->deleting_destroy_00413010(layout, nullptr, 1u);
    }
    const Word flags = self->flags & ~2u;
    self->layout = nullptr;
    self->flags = flags;
    gui_load_panel_resolve_move_to_0040a890(self, nullptr);
}

// 0040A680 CSWGuiPanel::StartLoadFromLayout(CResRef const& name):
// skip if flags bit1 (0040A69B); new(0xA0) (0040A6AC) then CResGFF(0x7FF,"GUI ",name)
// (0040A6D5) or null; store +2C (0040A6E9) BEFORE Demand (0040A6EC) - on Demand
// failure return leaving +2C set and bit1 clear. TopLevelStruct (0040A700);
// Extent::Load(+04) (0040A70E); COLOR vector default (-1,-1,-1) -> +50 (0040A746..64);
// if struct "BORDER" (0040A76F): background vslot +10 Load(gff,&top,"BORDER")
// (0040A79E) then vslot +04 SetExtent({0,0,width,height}) (0040A7AE..D6);
// else BACKGROUND resref default "" (0040A7DB..80A) -> SetFillImage(params+14,ref,0)
// (0040A814). ALPHA default 1.0 -> SetPanelAlpha (0040A82F/3A); GetList
// "CONTROLS" -> +30 (0040A84F); flags |= 2 (0040A854).
void __fastcall gui_load_panel_start_layout_0040a680(Panel* self, void*, ResRef* name) {
    if (self->flags & 2u) return;
    void* allocation = c2da_directory_new_006fa7e6(0xA0);
    Gff* gff = allocation
        ? gui_load_gff_construct_00410630(static_cast<Gff*>(allocation), nullptr, 0x7FF, "GUI ", name)
        : nullptr;
    self->layout = gff;
    if (!c2da_current_resource_demand_00409b10(gff, nullptr)) return;
    Struct top;
    gff->GetTopLevelStruct(&top);
    gui_load_extent_load_00409dc0(&self->extent, nullptr, gff, &top);
    Word success;
    const Vector color_default = { -1.0f, -1.0f, -1.0f };
    Vector color;
    const Vector* produced = gui_load_gff_vector_004122a0(gff, nullptr, &color, &top, "COLOR", &success,
                                                          &color_default);
    self->color = *produced;
    Struct border;
    if (gui_load_gff_struct_00411a10(gff, nullptr, &border, &top, "BORDER")) {
        {
            String label("BORDER");
            self->background->vtbl->load(self->background, nullptr, gff, &top, &label);
        }
        const Extent fill = { 0, 0, self->extent.width, self->extent.height };
        self->background->vtbl->set_extent(self->background, nullptr, &fill);
    } else {
        ResRef none;
        none.construct_00406d80("");
        unsigned char* params = self->background->params;
        ResRef image;
        const ResRef* read = gui_load_gff_resref_00411e10(gff, nullptr, &image, &top, "BACKGROUND", &success,
                                                          &none);
        gui_load_border_params_set_fill_00414c00(params, nullptr, read, 0);
    }
    const float alpha = gui_load_gff_float_00411d00(gff, nullptr, &top, "ALPHA", &success, 1.0f);
    gui_load_panel_set_alpha_00404b00(self, nullptr, alpha);
    gff_list_004118c0(gff, nullptr, &self->layout_controls, &top, "CONTROLS");
    self->flags |= 2u;
}

// 0067ACE0 CSWGuiMainMenu::LoadFromLayout(unused): CResRef "mainmenu" (0067AD0B);
// StartLoadFromLayout (0067AD17); flags |= 1 (0067AD1C); eleven InitControl(member,
// tag, 1) calls in this exact order with each temporary tag destroyed before the
// next (0067AD20..AF83); StopLoadFromLayout (0067AF8A).
void __fastcall gui_load_mainmenu_load_layout_0067ace0(MainMenu* self, void*, Word) {
    ResRef name;
    name.construct_00406d80("mainmenu");
    gui_load_panel_start_layout_0040a680(self, nullptr, &name);
    self->flags |= 1u;
    struct Entry { Control* control; const char* tag; };
    const Entry entries[] = {
        { &self->lb_modules, "LB_MODULES" },        // +0064
        { &self->btn_exit, "BTN_EXIT" },            // +1084
        { &self->btn_warp, "BTN_WARP" },            // +1248
        { &self->lbl_3dview, "LBL_3DVIEW" },        // +0360
        { &self->btn_newgame, "BTN_NEWGAME" },      // +03F0
        { &self->btn_loadgame, "BTN_LOADGAME" },    // +05B4
        { &self->btn_movies, "BTN_MOVIES" },        // +0778
        { &self->btn_options, "BTN_OPTIONS" },      // +093C
        { &self->lbl_newcontent, "LBL_NEWCONTENT" },// +0CC4
        { &self->lbl_gamelogo, "LBL_GAMELOGO" },    // +0E04
        { &self->lbl_menubg, "LBL_MENUBG" },        // +0F44
    };
    for (const Entry& entry : entries) {
        String tag(entry.tag);
        gui_load_panel_init_control_0040b930(self, nullptr, entry.control, &tag, 1);
    }
    gui_load_panel_stop_layout_0040b8f0(self, nullptr);
}

} // extern "C"
