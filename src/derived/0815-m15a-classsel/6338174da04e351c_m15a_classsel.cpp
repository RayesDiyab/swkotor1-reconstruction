// lane-m15-a (L15-A) unit m15a-classsel: CSWGuiMainMenu::OnNewGamePicked 0067AFB0 (guard + post-guard body;
// lifts TRAP #71 0067AFFB), CSWGuiClassSelection (ctor 006DC3C0, dtors 006DB720 / 006DC330, OnPanelAdded 006DB8F0,
// OnSelectButton 006DB9B0, OnEnterButton 006DBA70, HandleInputEvent 006DBD30, Draw 006DC030), CSWGuiClassSelChar
// (ctor 006DC350, dtor 006DB6D0) and the method table 00758020.
// Original: swkotor.exe GOG K1 (sha256 9c10e045...), static disassembly only; every behaviour cites its instruction
// addresses. Conventions: D1 fastcall veneers, D21 typed method table, D24 object views of method tables, D47 member
// callbacks, D30-FINAL (double intermediates; float only at the original fstp dword stores; original CRT ports for
// rand / __ftol2). EH: the original FuncInfo unwind states are reproduced with C++ try/catch where the callee is
// noexcept(false) (original operator new 006FA7E6 returns NULL; no throw site is reachable on the char-gen path).
#include "523fcf42472a3b45_m15a_gui.h"
#include "8d831d5c6afcac70_m15a_creature.h"     // L15-A unit m15a-creature: CSWCCreature ctor/SetSoundSet/SetAppearance/CGPauseCycle
#include "f155d76cd432eed7_m15_l0.h"            // L15-0: load screen, client getters, input cool-down, party session time
#include "bf662019daaaa2b6_m15b_stats.h"        // L15-B: CSWCCreatureStats::SetClass 00647770
#include "29a305efc7265995_m1_shared.h"         // M1 shared: PanelEventThunks::OnBButtonPressed_00624bb0, StartMenuMusic 005ED9B0
#include "232ef6efbadb2bc2_startup_views.h"     // M40: kotor_menu_10_19::Tlk::simple_0041e8f0 (CTlkTable::GetSimpleString)
#include "4e9032e94f9f5384_base_ini.h"          // M41: base_ini_reconstruction::PublicIni (CExoIni 005E6750 / 005E67E0)
#include "903d071617827bcc_table_contract.h"    // lane-startup-misc donor-c2da: C2DA 0x54 (00413CC0) + slot0 deleting dtor
#include "08e70ea18f1fcd38_load.h"              // lane-startup-misc donor-c2da: C2DA::Load2DArray 004143B0
#include "20e68b036c30980c_allocator_live.h"    // M41: live_free (006FA390 family)
#include "aae5b036321f76e1_growth.h"            // M40: CExoArrayList::Allocate 005E03C0
#include <cstdint>
#include <cstring>

using namespace kotor_m15a;
using gui_load::Object;
using gui_load::Panel;
using gui_load::Control;
using gui_load::Extent;
using gui_load::String;
using kotor_reconstruction::StartupString;
using kotor_reconstruction::ArrayMutations;

// ---------------------------------------------------------------- existing owners (declarations as the owners')
extern "C" {
__declspec(noinline) std::int32_t __fastcall c2da_current_get_int_00413660(c2da_current::Table*, void*, std::int32_t,
                                                                         c2da_current::String*, std::int32_t*);  // startup-misc-c2da
void __cdecl kotor_cdecl_00401c10(float delta, std::int32_t run_server, std::int32_t keep_frame);  // UpdateScreen (m48-fade)
std::int32_t __cdecl crt_rand_006fc447();                                     // D52 original CRT rand (3d-model)
void __cdecl genuine_x87_ftol_006fae8c();                                     // __ftol2 port (ST0 -> EDX:EAX)
void __fastcall kotor_fastcall_004088d0(void*, void*, void*);                 // 004088D0 RemoveResourceDirectory
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*, void*, menu_resources::String*); // 00406D60
std::uint32_t __fastcall gob_get_property_0x68_0063c070(void*, void*);        // 0063C070 mov eax,[ecx+68h] (3d-render vtables)
void __fastcall exo_array_list_destruct_0056b6f0(ArrayMutations*, void*);     // 0056B6F0 (3d-render scene3d; also in gui_objects.h)
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, gui_load::Word a, gui_load::Word b,
                                                           gui_load::Word c);  // 00414D00 (gui-load border/text load)
void __fastcall kotor_fastcall_005ed420(void* outer, void*, std::uint32_t argument);  // 005ED420 DisplayMainMenu (routes)
// L15-C (lane-m15-c): CSWGuiMainCharGen::CSWGuiMainCharGen(CSWGuiManager*, CSWCCreature*, CSWGuiClassSelection*) RET 0Ch.
void* __fastcall kotor_m15c_main_chargen_construct_006eb420(void* self, void* unused_edx, void* manager, void* creature,
                                                           void* class_selection) noexcept(false);
}
StartupString* __fastcall GuiConcat005e5d10(StartupString*, void*, StartupString*, const StartupString*);  // 005E5D10 operator+

namespace {
menu_resources::Manager* resman() { return kotor_resource_demand::resource_manager_007a39e8; }        // [007A39E8]
void* client() { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }                           // [[007A39FC]+4]
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); } // [007A3A08]
kih::Manager* manager_of(const Panel* panel) { return reinterpret_cast<kih::Manager*>(panel->manager); } // [this+18h]
ClassSelection* view(ClassSelectionHandlers* h) { return reinterpret_cast<ClassSelection*>(h); }
kih::GuiObject* target(ClassSelection* self) { return reinterpret_cast<kih::GuiObject*>(self); }
gui_load::EventFunc handler(void (__thiscall ClassSelectionHandlers::*f)(kih::Control*)) {
    return static_cast<gui_load::EventFunc>(f);                                // D47 derived-to-base PMF
}
float bits_float(Word bits) { float f; std::memcpy(&f, &bits, 4); return f; }
Word float_bits(float f) { Word w; std::memcpy(&w, &f, 4); return w; }

// `fld <double>; call 006FAE8C` -> EAX (low dword of __ftol2), D30-FINAL original CRT port.
I32 x87_ftol(double value) {
    I32 result;
    __asm {
        fld qword ptr value
        call genuine_x87_ftol_006fae8c
        mov result, eax
    }
    return result;
}

// Typed views of the called slots (D24). CSWCCreature table 0074F5F8 (L15-E owner; slot targets per lane-m15-f
// SLOT-AUDIT.json): +00 00618280 scalar deleting dtor RET 4, +8C 0060B9B0 SetPosition(const Vector*) RET 4,
// +94 0060FB40 AttachToScene(scene) RET 4, +F4 0060D5C0 SetPortraitId(id) RET 4.
struct CreatureCallView {
    void* (__fastcall* deleting_destroy)(void*, void*, Word);                   // +00
    const void* s04_88[0x22];
    void (__fastcall* set_position)(void*, void*, const Vector*);               // +8C
    const void* s90;
    void (__fastcall* attach_to_scene)(void*, void*, void*);                    // +94
    const void* s98_f0[0x17];
    void (__fastcall* set_portrait_id)(void*, void*, Word);                     // +F4
};
static_assert(offsetof(CreatureCallView, set_position) == 0x8C && offsetof(CreatureCallView, attach_to_scene) == 0x94 &&
              offsetof(CreatureCallView, set_portrait_id) == 0xF4, "creature slots");
// Anim base [creature+68h] (CSWCAnimBase family; L15-E) slot +08 GetModel(part) RET 4 -> CAurObject*;
// Aurora model object slot +104 (scene the model is attached to) RET 0.
struct AnimBaseCallView { const void* s00_04[2]; void* (__fastcall* get_model)(void*, void*, Word); };
struct GobCallView { const void* s00_100[0x41]; void* (__fastcall* scene)(void*, void*); };
static_assert(offsetof(AnimBaseCallView, get_model) == 0x08 && offsetof(GobCallView, scene) == 0x104, "model slots");
template <class V> const V* vt(const void* object) { return static_cast<const V*>(rp(object, 0)); }   // mov reg,[obj]

// CExoString produced by a callee into hidden-return storage; destroyed at scope end (as saveload.cpp).
struct Produced {
    alignas(4) unsigned char bytes[8];
    bool live = false;
    StartupString* slot() { return reinterpret_cast<StartupString*>(bytes); }
    StartupString* take(StartupString* produced) { live = true; return produced; }
    ~Produced() { if (live) slot()->~StartupString(); }
};
StartupString* simple_string(Produced& out, Word strref) {                      // 0041E8F0 GetSimpleString(out, strref)
    return out.take(reinterpret_cast<StartupString*>(
        tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(out.bytes), strref)));
}
}

// ======================================================================== D21 method table 00758020 (27 slots)
// Read from the original .rdata 00758020..0075808B: identical to CSWGuiPanel 0073E010 except +00 (006DC330),
// +34 (006DC030 Draw), +3C (006DBD30 HandleInputEvent) and +48 (006DB8F0 OnPanelAdded).
struct KotorM15aClassSelectionMethods {
    void* (__fastcall* s00)(ClassSelection*, void*, Word);                    // +00 006DC330 scalar deleting dtor
    void (__fastcall* s04)(Object*, void*, const Extent*);                    // +04 0040A5A0 CSWGuiPanel::SetExtent
    void (__fastcall* s08)(Object*, void*, Object*, Word);                    // +08 0040A630 SetActiveControl
    void (__fastcall* s0c)(Object*, void*, Word);                             // +0C 0060E760 void_return [RET 4]
    Word (__fastcall* s10)(Object*, void*, Word, Word);                       // +10 00409EB0 return_zero_0x8
    void (__fastcall* s14)(Object*, void*);                                   // +14 004015A0
    void (__fastcall* s18)(Object*, void*);                                   // +18 004015A0
    void (__fastcall* s1c)(Object*, void*);                                   // +1C 004015A0
    void (__fastcall* s20)(Object*, void*);                                   // +20 004015A0
    void (__fastcall* s24)(Object*, void*);                                   // +24 004015A0
    Object* (__fastcall* s28)(Object*, void*);                                // +28 00641DB0 self_return
    Word (__fastcall* s2c)(Object*, void*);                                   // +2C 0063E7F0 return_zero
    Word (__fastcall* s30)(Object*, void*);                                   // +30 0063E7F0 return_zero
    void (__fastcall* s34)(Panel*, void*, Word);                              // +34 006DC030 CSWGuiClassSelection::Draw
    void (__fastcall* s38)(Object*, void*, Word);                             // +38 0060E760 void_return [RET 4]
    void (__fastcall* s3c)(Object*, void*, I32, I32);                         // +3C 006DBD30 HandleInputEvent
    Object* (__fastcall* s40)(Object*, void*, I32, I32);                      // +40 0040B690 HitCheckMouse
    Object* (__fastcall* s44)(Object*, void*);                                // +44 0040B630 GetActiveControl
    void (__fastcall* s48)(Panel*, void*);                                    // +48 006DB8F0 OnPanelAdded
    void (__fastcall* s4c)(Panel*, void*);                                    // +4C 0040C170 OnPanelRemoved
    void (__fastcall* s50)(Object*, void*, Word);                             // +50 0040B640 OnAButtonPressed
    void (__fastcall* s54)(Object*, void*, Word);                             // +54 0040B650 OnBButtonPressed
    void (__fastcall* s58)(Object*, void*, Word);                             // +58 0040B660 OnXButtonPressed
    void (__fastcall* s5c)(Object*, void*, Word);                             // +5C 0040B670 OnYButtonPressed
    void (__fastcall* s60)(Object*, void*, Word);                             // +60 0040B680 OnBlackButtonPressed
    String* (__fastcall* s64)(Panel*, void*, String*);                        // +64 0040A900 GetFullScreenBG
    void (__fastcall* s68)(Panel*, void*);                                    // +68 0040A9C0 ResetFont
};
static_assert(sizeof(KotorM15aClassSelectionMethods) == 0x6C, "00758020 slot count (27)");
extern "C" const KotorM15aClassSelectionMethods kotor_m15a_class_selection_methods_00758020 = {
    &kotor_m15a_class_selection_deleting_destruct_006dc330,   // +00
    &gui_load_panel_set_extent_0040a5a0,                      // +04
    &gui_load_panel_set_active_control_0040a630,              // +08
    &gui_load_slot_ret4_0060e760,                             // +0C
    &gui_load_slot_zero8_00409eb0,                            // +10
    &gui_load_slot_ret_004015a0,                              // +14
    &gui_load_slot_ret_004015a0,                              // +18
    &gui_load_slot_ret_004015a0,                              // +1C
    &gui_load_slot_ret_004015a0,                              // +20
    &gui_load_slot_ret_004015a0,                              // +24
    &gui_load_slot_self_00641db0,                             // +28
    &gui_load_slot_zero_0063e7f0,                             // +2C
    &gui_load_slot_zero_0063e7f0,                             // +30
    &kotor_m15a_class_selection_draw_006dc030,                // +34
    &gui_load_slot_ret4_0060e760,                             // +38
    &kotor_m15a_class_selection_handle_input_006dbd30,        // +3C
    &gui_load_panel_hit_check_0040b690,                       // +40
    &gui_load_panel_active_control_0040b630,                  // +44
    &kotor_m15a_class_selection_on_panel_added_006db8f0,      // +48
    &gui_load_panel_on_removed_0040c170,                      // +4C
    &gui_load_panel_button_a_0040b640,                        // +50
    &gui_load_panel_button_b_0040b650,                        // +54
    &gui_load_panel_button_x_0040b660,                        // +58
    &gui_load_panel_button_y_0040b670,                        // +5C
    &gui_load_panel_button_black_0040b680,                    // +60
    &gui_load_panel_full_screen_bg_0040a900,                  // +64
    &gui_load_panel_reset_font_0040a9c0,                      // +68
};

// ======================================================================== CSWGuiMainMenu::OnNewGamePicked
// 0067AFB0 (thiscall, RET 4). Guard 0067AFCB..0067AFF5 identical to the other three main-menu handlers.
// Locals: CExoIni 0067B005 (in the dead argument slot), unused default CExoString 0067B016 (EH state 1),
// module name "END_M01AA" 0067B029 (state 2).
void __thiscall gui_load::MainMenuHandlers::OnNewGamePicked(kotor_input_hover::Control* control) {
    gui_load::MainMenu* const menu = reinterpret_cast<gui_load::MainMenu*>(this);
    if ((menu->flags & 0x600u) == 0x400u) return;                                  // 0067AFCB..D8
    if (menu->u140c == 0) return;                                                  // 0067AFDE..E6 (+140C, set by Draw 0067AC28)
    if (rd32(control, 0x4C) == 0) return;                                          // 0067AFEC..F5
    kotor_m15_party_reset_session_start_time_00563cf0();                           // 0067AFFC call 00563CF0 (static)
    base_ini_reconstruction::PublicIni ini;
    ini.construct_005e6750();                                                      // 0067B001..05 CExoIni::CExoIni
    try {
        StartupString unused;                                                      // 0067B00A..16 CExoString() (never read)
        StartupString module("END_M01AA");                                         // 0067B01B..29 (00752F58)
        {
            StartupString alias("MODULES:");                                       // 0067B033..40 (0073D90C)
            resman()->directory_00408800(&alias);                                  // 0067B045..55 AddResourceDirectory
        }                                                                          // 0067B05A..62
        menu_resources::Ref name;
        async_ref_construct_string_00406d60(&name, nullptr, &module);              // 0067B067..70 CResRef(module)
        if (!resman()->exists_00408bc0(&name, 0x7DB, nullptr)) {                   // 0067B075..8E Exists(.mod 7DBh, 0)
            async_ref_construct_string_00406d60(&name, nullptr, &module);          // 0067B090..99
            if (!resman()->exists_00408bc0(&name, 0xBBA, nullptr))                 // 0067B09E..B7 Exists(.rim BBAh, 0)
                GuiAssign005e5140(&module, nullptr, "END_M01AA");                  // 0067B0B9..C2 operator=(00752F58)
        }
        {
            StartupString alias("MODULES:");                                       // 0067B0C7..D0 (0073D90C)
            kotor_fastcall_004088d0(resman(), nullptr, &alias);                    // 0067B0D5..E5 RemoveResourceDirectory
        }                                                                          // 0067B0EA..F2
        void* const memory = c2da_directory_new_006fa7e6(0x1560);                  // 0067B0F7..FC operator new
        ClassSelection* panel = nullptr;                                           // 0067B123 xor eax,eax
        if (memory) {                                                              // 0067B108..0F
            try {
                panel = kotor_m15a_class_selection_construct_006dc3c0(static_cast<ClassSelection*>(memory), nullptr,
                                                                      menu->manager, &module);   // 0067B111..1C
            } catch (...) {
                live_free(memory);                                                 // EH state 5 (072E2F3 funclet) -> 006FA390
                throw;
            }
        }
        kotor_fastcall_0040bc70(menu->manager, nullptr, panel, 2, 1);              // 0067B125..30 AddPanel(p, 2, 1)
        table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr,
                                                  *sound_mode29_required::word_0074c5ec);   // 0067B135..42
        menu->flags = (menu->flags & 0xFFFFFCFFu) | 0x400u;                         // 0067B147..58
    } catch (...) {                                                                // 0067B154..7F unwind order
        ini.destroy_005e67e0();
        throw;
    }
    ini.destroy_005e67e0();                                                        // 0067B173..7F ~CExoIni
}

extern "C" {
// ======================================================================== CSWGuiClassSelChar
// 006DC350 ctor (RET 0, EAX = this): button, CSWGui3DSceneView (control + vptr 00752E30 + CSWGuiScene).
// EH states: 0 -> ~CSWGuiButton (007363E0), 1 -> ~CSWGuiControl (007363E8).
ClassSelChar* __fastcall kotor_m15a_class_sel_char_construct_006dc350(ClassSelChar* self, void*) noexcept(false) {
    gui_load_button_construct_0041c0f0(&self->button, nullptr);                    // 006DC370
    try {
        gui_load_control_construct_0041aa80(&self->view, nullptr);                 // 006DC375..89
        try {
            self->view.vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&gui_load_scene_view_methods_00752e30); // 006DC396
            gui_scene_construct_004174b0(&self->scene, nullptr);                   // 006DC38E..9C
        } catch (...) {
            gui_load_control_destruct_00419a20(&self->view, nullptr);              // state 1
            throw;
        }
    } catch (...) {
        gui_load_button_destruct_00419da0(&self->button, nullptr);                 // state 0
        throw;
    }
    return self;                                                                   // 006DC3A6
}
// 006DB6D0 dtor (RET 0): ~CSWGui3DSceneView (+1C4) then ~CSWGuiButton.
void __fastcall kotor_m15a_class_sel_char_destruct_006db6d0(ClassSelChar* self, void*) {
    gui_load_scene_view_destruct_0067ac50(&self->view, nullptr);                   // 006DB6ED..FB
    gui_load_button_destruct_00419da0(&self->button, nullptr);                     // 006DB700..0A
}
}

// ======================================================================== CSWGuiClassSelection ctor
namespace {
// Constructed-member stages of 006DC3C0 (FuncInfo 007867E4 states 0..7; funclets 00736400..00736470).
enum Stage { kPanel = 0, kChars, kCharGen, kInstruction, kDesc, kClass, kBack, kModule };
void unwind_class_selection(ClassSelection* self, int built) {
    if (built >= kModule) reinterpret_cast<StartupString*>(self->module)->~StartupString();       // 00736467
    if (built >= kBack) gui_load_button_destruct_00419da0(&self->btn_back, nullptr);              // 00736459
    if (built >= kClass) gui_load_label_destruct_00419d10(&self->lbl_class, nullptr);             // 0073644B
    if (built >= kDesc) gui_load_label_destruct_00419d10(&self->lbl_desc, nullptr);               // 0073643D
    if (built >= kInstruction) gui_load_label_destruct_00419d10(&self->lbl_instruction, nullptr); // 0073642F
    if (built >= kCharGen) gui_load_label_destruct_00419d10(&self->lbl_char_gen, nullptr);        // 00736421
    if (built >= kChars)
        for (int i = 6; i-- > 0;) kotor_m15a_class_sel_char_destruct_006db6d0(&self->chars[i], nullptr); // 00736408 006FA852
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 00736400
}
void init_control(ClassSelection* self, Control* control, const char* tag) {      // CExoString temp + InitControl(c, &s, 1)
    StartupString name(tag);                                                       // 005E5A90
    gui_load_panel_init_control_0040b930(self, nullptr, control, &name, 1);        // 0040B930
}                                                                                  // 005E5C20
// SetLoadBarProgress(ftol(++counter * 0.0625f * 100.0f), 1) then UpdateScreen(1/30, 0, 0)
// (006DC92B..60 / 006DCC0A..46 / 006DCDDD..17).
void advance_progress(I32& counter) {
    ++counter;                                                                     // inc dword ptr [esp+10h]
    const double scaled = static_cast<double>(counter) * 0.0625 * 100.0;          // fild; fmul [0074F5F4]; fmul [00741838]
    kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, x87_ftol(scaled), 1);   // call 006FAE8C; 005EDD40
    kotor_cdecl_00401c10(bits_float(0x3D088889u), 0, 0);                           // push 0/0/3D088889h; call 00401C10
}
// CExoArrayList<int>::Add (inline at 006DC706..37 / 006DC73E..6F): grow to cap*2 (16 when 0) through 005E03C0.
void list_add(ArrayMutations* list, I32 value) {
    if (list->size == list->capacity) {
        const std::int32_t capacity = list->capacity;
        menu_list_growth::allocate_005e03c0(list, nullptr, static_cast<Word>(capacity != 0 ? capacity + capacity : 0x10));
    }
    const std::int32_t size = list->size;
    st<I32>(reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data)), static_cast<unsigned>(size) * 4u, value);
    list->size = size + 1;
}
// Random pick + removal (006DCA00..2E / 006DCA34..60): idx = rand() % size; size--; value = data[idx];
// data[idx..size-1] = data[idx+1..size].
I32 list_take_random(ArrayMutations* list) {
    const std::int32_t r = crt_rand_006fc447();                                    // call 006FC447
    std::int32_t size = list->size;
    const std::int32_t index = r % size;                                           // cdq; idiv
    --size;
    list->size = size;                                                             // dec ecx; mov [..],ecx
    void* const data = reinterpret_cast<void*>(static_cast<std::uintptr_t>(list->data));
    const I32 value = ld<I32>(data, static_cast<unsigned>(index) * 4u);
    for (std::int32_t i = index; i < size; ++i)
        st<I32>(data, static_cast<unsigned>(i) * 4u, ld<I32>(data, static_cast<unsigned>(i + 1) * 4u));
    return value;
}
}

extern "C" ClassSelection* __fastcall kotor_m15a_class_selection_construct_006dc3c0(ClassSelection* self, void*,
    Object* manager, const String* module) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                     // 006DC3E9..F1
    int built = kPanel;
    try {
        self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15a_class_selection_methods_00758020); // 006DC414
        // 006DC41A eh_vector_constructor_iterator(+6C, 25Ch, 6, 006DC350, 006DB6D0).
        for (int i = 0; i < 6; ++i) {
            try { kotor_m15a_class_sel_char_construct_006dc350(&self->chars[i], nullptr); }
            catch (...) { while (i-- > 0) kotor_m15a_class_sel_char_destruct_006db6d0(&self->chars[i], nullptr); throw; }
        }
        built = kChars;
        gui_load_label_construct_0041acd0(&self->lbl_char_gen, nullptr);    built = kCharGen;     // 006DC41F..2D
        gui_load_label_construct_0041acd0(&self->lbl_instruction, nullptr); built = kInstruction; // 006DC432..40
        gui_load_label_construct_0041acd0(&self->lbl_desc, nullptr);        built = kDesc;        // 006DC445..53
        gui_load_label_construct_0041acd0(&self->lbl_class, nullptr);       built = kClass;       // 006DC458..66
        gui_load_button_construct_0041c0f0(&self->btn_back, nullptr);       built = kBack;        // 006DC46B..79
        new (self->module) StartupString();                                 built = kModule;      // 006DC47E..8C

        StartupString format;                                                      // 006DC491..9D [esp+64h] (state 8)
        const Vector origin = {0.0f, 0.0f, 0.0f};                                  // 006DC4B3..BB [esp+74h]
        const Quaternion orientation = {0.0f, 0.0f, 0.0f, 1.0f};                   // 006DC4C2..D7 [esp+A0h]
        {
            StartupString layout_module("classsel");                               // 006DC4A2..E2 (00758124)
            kotor_m15_client_set_load_screen_by_module_005edcf0(client(), nullptr, &layout_module);  // 006DC4E7..FD
        }                                                                          // 006DC502..0E
        kotor_m15_client_set_load_screen_hint_005edf00(client(), nullptr, 0);      // 006DC513..1C
        kotor_m15_client_show_load_screen_005eda80(client(), nullptr, 1, 0, 0);    // 006DC521..2E (push 0,0,1)
        kotor_cdecl_00401c10(bits_float(0x3D088889u), 0, 0);                       // 006DC533..3F
        {
            menu_resources::Ref rim;
            rim.construct_00406d80("CHARGEN");                                     // 006DC542..4B (00758014)
            if (resman()->exists_00408bc0(&rim, 0xBBA, nullptr)) {                 // 006DC550..68
                StartupString image("RIMS:CHARGEN");                               // 006DC56A..73 (0073E094)
                resman()->image_004087c0(&image);                                  // 006DC578..8B AddResourceImageFile
            }                                                                      // 006DC590..9C
        }
        kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, 6, 1);  // 006DC5A1..AE
        kotor_cdecl_00401c10(bits_float(0x3D088889u), 0, 0);                       // 006DC5B3..BA
        kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, 0x0C, 1);  // 006DC5BF..CF
        kotor_cdecl_00401c10(bits_float(0x3D088889u), 0, 0);                       // 006DC5D4..DB
        {
            gui_load::ResRef layout;
            reinterpret_cast<menu_resources::Ref*>(&layout)->construct_00406d80("ClassSel");   // 006DC5E3..EC (00758118)
            gui_load_panel_start_layout_0040a680(self, nullptr, &layout);          // 006DC5F1..F8
        }
        // 006DC5FD..4A new(54h) C2DA(CResRef("PORTRAITS"), 0); Load2DArray (on the NULL result as well).
        c2da_current::Table* portraits = nullptr;
        if (void* memory = c2da_directory_new_006fa7e6(0x54)) {                    // 006DC5FD..17
            try {
                menu_resources::Ref name;                                          // sub esp,10h; mov ecx,esp
                name.construct_00406d80("PORTRAITS");                              // 006DC623..28 (0074DEAC)
                portraits = c2da_current_construct_00413cc0(memory, nullptr, name, 0);  // 006DC619 push 0; 006DC62F
            } catch (...) {
                live_free(memory);                                                 // EH state 0Bh (00736496)
                throw;
            }
        }
        c2da_current_load_004143b0(portraits, nullptr);                            // 006DC640..4A
        const std::int32_t rows = static_cast<std::int32_t>(c2da_current::load_word(portraits, 0x24));   // 006DC64F [edi+24h]
        ArrayMutations male{};                                                     // [esp+40h] (006DC664..6C zeroed)
        ArrayMutations female{};                                                   // [esp+4Ch] (006DC658..60 zeroed)
        try {
            for (std::int32_t row = 0; row < rows; ++row) {                        // 006DC670..776
                // D2: GetINTEntry leaves the out value unwritten on a miss; the original locals [esp+70h]/[esp+6Ch]
                // have no initializing store (documented deviation: value-initialized to 0).
                std::int32_t for_pc = 0;
                {
                    StartupString column("ForPC");                                 // 006DC680..89 (00758110)
                    c2da_current_get_int_00413660(portraits, nullptr, row, &column, &for_pc);   // 006DC68E..A3
                }
                if (for_pc == 0) continue;                                         // 006DC6B9..BF
                std::int32_t sex = 0;
                {
                    StartupString column("Sex");                                   // 006DC6C5..CE (0075810C)
                    c2da_current_get_int_00413660(portraits, nullptr, row, &column, &sex);      // 006DC6D3..E8
                }
                if (sex == 0) list_add(&male, row);                                // 006DC6FE..37
                else if (sex == 1) list_add(&female, row);                         // 006DC739..6F
            }
            kotor_m15_client_set_load_bar_progress_005edd40(client(), nullptr, 0x12, 1);  // 006DC77C..91
            kotor_cdecl_00401c10(bits_float(0x3D088889u), 0, 0);                   // 006DC796..9F
            I32 counter = 3;                                                       // 006DC789 [esp+10h] = 3
            I32 portrait_row = 0;     // [esp+20h]: D2 (only unwritten when the stats gender is neither 0 nor 1)
            for (int k = 0; k < 6; ++k) {                                          // 006DC7C1 [esp+30h] = 6 .. 006DCC75
                ClassSelChar* const element = &self->chars[k];
                const KotorM15aClassChoice* const choice = &kotor_m15a_class_choices_007a2684[k];   // [esp+38h] 007A2684+8k
                const int number = k + 1;                                          // [esp+3Ch] from 1
                kotor_reconstruction::StartupFormat005e5680(&format, "BTN_SEL%d", number);       // 006DC7D0..E4 (00758100)
                gui_load_panel_init_control_0040b930(self, nullptr, &element->button, &format, 1);  // 006DC7E7..F7
                kotor_reconstruction::StartupFormat005e5680(&format, "3D_MODEL%d", number);      // 006DC7FC..810 (007580F4)
                gui_load_panel_init_control_0040b930(self, nullptr, &element->view, &format, 1);   // 006DC813..20
                std::memcpy(at(&element->button, 0x90), &borrowed_exit_color_007a23b4, 12);   // 006DC825..41 border color
                std::memcpy(at(&element->button, 0x104), &borrowed_enter_color_007a23c0, 12); // 006DC844..65 hilight color
                gui_load_border_params_set_pulsing_00414d00(at(&element->button, 0xF4), nullptr, 1, 1, 0);  // 006DC861..70
                gui_load_control_add_event_0041ab20(&element->button, nullptr, 0, target(self),
                    handler(&ClassSelectionHandlers::OnEnterButton_006dba70));     // 006DC875..7F
                gui_load_control_add_event_0041ab20(&element->button, nullptr, 0x27, target(self),
                    handler(&ClassSelectionHandlers::OnSelectButton_006db9b0));    // 006DC884..8E
                gui_load_control_add_event_0041ab20(&element->button, nullptr, 0x2D, target(self),
                    handler(&ClassSelectionHandlers::OnSelectButton_006db9b0));    // 006DC893..9D
                void* const aur_scene = element->scene.aur_scene;                  // 006DC8A9 [esi-4] (+234)
                void* const camera = element->scene.aur_camera;                    // 006DC8AC [esi] (+238)
                if (kotor_cell_do3dgui_0078d1e4 != 0) {                            // 006DC8A2..B2
                    vt<gui_load::AurSceneVtbl>(aur_scene)->spawn_room(aur_scene, nullptr, "gui3D_room",
                        &origin.x, &orientation.x);                                // 006DC8B4..C8 [edx+70h] (00753038)
                    void* model;
                    {
                        StartupString light("cgbody_light");                       // 006DC8CB..D7 (007580E4)
                        model = aurora_gui_scene_add_model_name_00417620(&element->scene, nullptr, &light, -1);  // 006DC8DC..F1
                    }                                                              // 006DC8F6..907
                    if (model) {                                                   // 006DC90C..0E
                        vt<gui_load::AurCameraVtbl>(camera)->attach_to_object(camera, nullptr, model, "camerahook", 0);  // 006DC910..1C (0074F42C)
                        vt<gui_load::AurCameraVtbl>(camera)->set_view_angle(camera, nullptr, bits_float(0x41B5CED9u)); // 006DC91F..28 (22.726f)
                    }
                }
                advance_progress(counter);                                         // 006DC92B..60
                void* creature = nullptr;
                if (void* memory = c2da_directory_new_006fa7e6(kCreatureSize)) {   // 006DC965..80 new(44Ch)
                    try { creature = kotor_m15a_creature_construct_00616a20(memory, nullptr); }   // 006DC982..84
                    catch (...) { live_free(memory); throw; }                      // EH state 11h
                }
                void* const stats = rp(creature, 0x2F8);                           // 006DC9A4 [esi+2F8h]
                st<U8>(stats, 0x31, choice->gender);                               // 006DC9A1..AA [edi+1] -> stats+31h
                kotor_m15b_creature_stats_set_class_00647770(rp(creature, 0x2F8), nullptr, 0, choice->class_id);  // 006DC9AD..C2
                kotor_m15a_creature_set_sound_set_0060b7c0(creature, nullptr, choice->sound_set);   // 006DC9C7..CF movzx ax,[edi+2]
                unsigned char block[0x3C];                                         // [esp+B4h] appearance block
                std::memcpy(block, rp(creature, 0x21C), sizeof block);             // 006DC9D4..E6 rep movsd 0Fh from [esi+21Ch]
                const U8 gender = ld<U8>(rp(creature, 0x2F8), 0x31);               // 006DC9EC..F2 [[esi+2F8h]+31h]
                block[0x1B] = gender;                                              // 006DC9F7 [esp+CFh]
                if (gender == 0) portrait_row = list_take_random(&male);           // 006DC9FE..2E
                else if (gender == 1) portrait_row = list_take_random(&female);    // 006DCA30..60
                std::int32_t appearance = 0;   // [esp+2Ch]: D2 (GetINTEntry miss leaves it unwritten)
                {
                    const char* column;
                    switch (choice->class_id) {                                    // 006DCA62..7A movzx [edi]; sub/dec
                    case 0: column = "Appearance_L"; break;                        // 006DCB36 (007580B0)
                    case 1: column = "AppearanceNumber"; break;                    // 006DCAF9 (007580D0)
                    case 2: column = "Appearance_S"; break;                        // 006DCABC (007580C0)
                    default: column = "AppearanceNumber"; break;                   // 006DCA7C (007580D0)
                    }
                    StartupString name(column);
                    c2da_current_get_int_00413660(portraits, nullptr, portrait_row, &name, &appearance);   // 006DCA8D..CB5F
                }                                                                  // 006DCB68..70
                vt<CreatureCallView>(creature)->set_portrait_id(creature, nullptr, static_cast<Word>(portrait_row));  // 006DCB75..7E [edx+F4h]
                st<U16>(rp(creature, 0x2F8), 0x8C, static_cast<U16>(appearance)); // 006DCB84..8F stats+8Ch (word)
                st<U16>(block, 0x18, static_cast<U16>(appearance));                // 006DCB9D..A2 [esp+CCh]
                st<Word>(block, 0x14, 1);                                          // 006DCBAA [esp+C8h] = 1
                block[0x00] = 2;                                                   // 006DCBB5 [esp+B4h] = 2
                block[0x01] = 1;                                                   // 006DCBBD [esp+B5h] = 1
                if (kotor_cell_do3dgui_0078d1e4 != 0) {                            // 006DCB96..C5
                    kotor_m15a_creature_set_appearance_006134c0(creature, nullptr, block, 3, 1);   // 006DCBC7..D5
                    vt<CreatureCallView>(creature)->attach_to_scene(creature, nullptr, aur_scene); // 006DCBDA..E3 [edx+94h]
                    vt<CreatureCallView>(creature)->set_position(creature, nullptr, &origin);      // 006DCBE9..F2 [edx+8Ch]
                    kotor_m15a_creature_cg_pause_cycle_0060f7c0(creature, nullptr, 0.0f, 1);       // 006DCBF8..FE (push 1, push 0)
                }
                element->creature = creature;                                      // 006DCC03..07 [edi+1Ch]
                st<Word>(&element->timer, 0, 0xBF800000u);                         // 006DCC12 [edi+20h] = -1.0f
                advance_progress(counter);                                         // 006DCC0A..46
            }
            init_control(self, &self->lbl_instruction, "LBL_INSTRUCTION");         // 006DCC7B..B2 (007580A0)
            init_control(self, &self->lbl_char_gen, "LBL_CHAR_GEN");               // 006DCCB7..EE (00758090)
            init_control(self, &self->lbl_desc, "LBL_DESC");                       // 006DCCF3..D2A (007548F8)
            init_control(self, &self->lbl_class, "LBL_CLASS");                     // 006DCD2F..66 (00756438)
            init_control(self, &self->btn_back, "BTN_BACK");                       // 006DCD6B..A2 (007548AC)
            gui_load_panel_stop_layout_0040b8f0(self, nullptr);                    // 006DCDA7..A9
            self->btn_back.flags &= 0xFFFFFFFBu;                                   // 006DCDAE [esi+44h]
            gui_load_control_add_event_0041ab20(&self->btn_back, nullptr, 0x27, target(self),
                kotor_m1_shared::event_func(&kotor_m1_shared::PanelEventThunks::OnBButtonPressed_00624bb0));  // 006DCDB2..BC
            gui_load_panel_set_active_control_0040a630(self, nullptr, &self->chars[0].button, 0);   // 006DCDC1..C9 (+6C)
            reinterpret_cast<StartupString*>(self->module)->Assign005e5c50(
                const_cast<StartupString*>(module));                               // 006DCDCE..D8 operator=(arg 2)
            advance_progress(counter);                                             // 006DCDDD..17
            static_cast<const c2da_current::TableSlot0*>(portraits->helper.methods)->deleting(portraits, nullptr, 1);  // 006DCE1C..27 [eax](1)
        } catch (...) {
            exo_array_list_destruct_0056b6f0(&female, nullptr);                    // EH state 0Dh (007364A4)
            exo_array_list_destruct_0056b6f0(&male, nullptr);                      // EH state 0Ch (007364AF)
            throw;
        }
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(male.data)));     // 006DCE29..36 _free([esp+40h])
        live_free(reinterpret_cast<void*>(static_cast<std::uintptr_t>(female.data)));   // 006DCE3B..48 _free([esp+4Ch])
    } catch (...) {                                                                // 006DCE50..5C ~format (scope) then members
        unwind_class_selection(self, built);
        throw;
    }
    return self;                                                                   // 006DCE6A
}

extern "C" {
// ======================================================================== dtors
// 006DB720 ~CSWGuiClassSelection (RET 0).
void __fastcall kotor_m15a_class_selection_destruct_006db720(ClassSelection* self, void*) {
    self->vtbl = reinterpret_cast<const gui_load::ObjectVtbl*>(&kotor_m15a_class_selection_methods_00758020);  // 006DB741
    for (int k = 0; k < 6; ++k) {                                                  // 006DB74F..79 (+2C0, stride 25Ch)
        void* const creature = self->chars[k].creature;                            // 006DB760
        if (creature) vt<CreatureCallView>(creature)->deleting_destroy(creature, nullptr, 1);   // 006DB762..6A [eax](1)
        self->chars[k].creature = nullptr;                                         // 006DB76C
    }
    menu_resources::Ref rim;
    rim.construct_00406d80("CHARGEN");                                             // 006DB77B..84 (00758014)
    if (resman()->exists_00408bc0(&rim, 0xBBA, nullptr)) {                         // 006DB789..A2
        void* const manager = resman();                                            // 006DB7A4 mov eax,[007A39E8]
        st<U8>(manager, 0x34, static_cast<U8>(ld<U8>(manager, 0x34) | 1u));        // 006DB7A9 or byte ptr [eax+34h],1
    }
    reinterpret_cast<StartupString*>(self->module)->~StartupString();              // 006DB7AD..B8
    gui_load_button_destruct_00419da0(&self->btn_back, nullptr);                   // 006DB7BD..C8
    // 006DB7CD..006DB8AC: the four CSWGuiLabel dtors are inlined in the original (vptr 0073E5B8, ~CSWGuiText +D0,
    // ~CSWGuiBorder +5C, ~CSWGuiControl) = exactly the body of CSWGuiLabel::~CSWGuiLabel 00419D10.
    gui_load_label_destruct_00419d10(&self->lbl_class, nullptr);                   // +1254
    gui_load_label_destruct_00419d10(&self->lbl_desc, nullptr);                    // +1114
    gui_load_label_destruct_00419d10(&self->lbl_instruction, nullptr);             // +FD4
    gui_load_label_destruct_00419d10(&self->lbl_char_gen, nullptr);                // +E94
    for (int i = 6; i-- > 0;) kotor_m15a_class_sel_char_destruct_006db6d0(&self->chars[i], nullptr);  // 006DB8B1..C6 006FA852
    gui_load_panel_destruct_0040cf70(self, nullptr);                               // 006DB8CB..D5
}
// 006DC330 scalar deleting dtor (flags) RET 4.
void* __fastcall kotor_m15a_class_selection_deleting_destruct_006dc330(ClassSelection* self, void*, Word flags) {
    kotor_m15a_class_selection_destruct_006db720(self, nullptr);                   // 006DC333
    if (flags & 1u) live_free(self);                                               // 006DC338..45
    return self;                                                                   // 006DC348
}

// ======================================================================== panel slots
// 006DB8F0 OnPanelAdded (RET 0).
void __fastcall kotor_m15a_class_selection_on_panel_added_006db8f0(Panel* self, void*) noexcept(false) {
    {
        StartupString directory("GAMEINPROGRESS:");                                // 006DB90B..14 (0074514C)
        gui_load_resman_clean_directory_00409460(resman(), nullptr, &directory, 0, 0);   // 006DB919..30
    }                                                                              // 006DB935..41
    cool_down_event_005df4b0(kotor_menu_window_cells::input_007a39e4, nullptr, 0x28, 0x1F4);   // 006DB946..53
    cool_down_event_005df4b0(kotor_menu_window_cells::input_007a39e4, nullptr, 0x2E, 0x1F4);   // 006DB958..65
    kotor_m15_client_hide_load_screen_005eda90(client(), nullptr, 1, 1);           // 006DB96A..77
    gui_load_panel_on_added_0040b870(self, nullptr);                               // 006DB97C..7E
    kotor_m1_client_start_menu_music_005ed9b0(client(), nullptr, 0, 0x7F, 2);      // 006DB983..92 (push 2, 7Fh, 0)
}
// 006DBD30 HandleInputEvent(event, pressed) RET 8; jump table 006DBDC4 indexed by byte table 006DBDD4
// (event - 28h, 0..0Eh): 28h/2Eh -> 006DBD57, 35h -> 006DBD92, 36h -> 006DBDAB, others -> 006DBDB5.
void __fastcall kotor_m15a_class_selection_handle_input_006dbd30(Object* object, void*, I32 event, I32 pressed) noexcept(false) {
    Panel* const self = static_cast<Panel*>(object);
    if (pressed != 0) {                                                            // 006DBD3B..3F
        const Word index = static_cast<Word>(event - 0x28);                        // 006DBD41..47 (unsigned ja)
        if (index <= 0x0E) {
            if (event == 0x28 || event == 0x2E) {                                  // case 0 (006DBD57)
                manager_of(self)->PlayGuiSound(0);                                 // 006DBD57..5C
                self->flags = (self->flags & 0xFFFFFCFFu) | 0x400u;                // 006DBD61..70
                kotor_fastcall_005ed420(client(), nullptr, 0);                     // 006DBD73..7E DisplayMainMenu(0)
            } else if (event == 0x35) {                                            // case 1 (006DBD92)
                self->vtbl->handle_input_event(self, nullptr, 0x2F, pressed);      // 006DBD92..99 [eax+3Ch](2Fh, pressed)
            } else if (event == 0x36) {                                            // case 2 (006DBDAB)
                self->vtbl->handle_input_event(self, nullptr, 0x30, pressed);      // 006DBDAB..B2 [edx+3Ch](30h, pressed)
            }
        }
    }
    gui_load_panel_handle_input_00409e60(self, nullptr, event, pressed);           // 006DBD83..87 / 9C..A0 / B5..B9
}
// 006DC030 Draw(float delta) RET 4.
void __fastcall kotor_m15a_class_selection_draw_006dc030(Panel* panel, void*, Word delta_bits) noexcept(false) {
    ClassSelection* const self = static_cast<ClassSelection*>(panel);
    const float delta = bits_float(delta_bits);                                    // [esp+40h]
    Extent button_extent = {0, 0, 0, 0};                                           // [esp+2Ch] (006DC03D..49 zeroed)
    Extent view_extent = {0, 0, 0, 0};                                             // [esp+1Ch] (006DC04D..59 zeroed)
    for (int k = 0; k < 6; ++k) {                                                  // 006DC05D..63 / 006DC2F5..304
        ClassSelChar* const element = &self->chars[k];                             // esi = this+2C0+25Ch*k
        if (kotor_cell_do3dgui_0078d1e4 != 0 &&                                    // 006DC078..7E
            static_cast<std::int8_t>(static_cast<U8>(self->flags)) < 0) {          // 006DC080..85 byte [this+44h] sign
            void* const creature = element->creature;                              // 006DC087 [esi]
            void* const aur_scene = element->scene.aur_scene;                      // 006DC089 [esi-20h]
            void* const anim = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
                gob_get_property_0x68_0063c070(creature, nullptr)));               // 006DC08C [creature+68h]
            void* const model = vt<AnimBaseCallView>(anim)->get_model(anim, nullptr, 0xFF);   // 006DC091..9A [edx+8](FFh)
            if (vt<GobCallView>(model)->scene(model, nullptr) != aur_scene) {      // 006DC09D..A9 [edx+104h]
                void* const selected = self->selected;                             // 006DC0AB..AF [this+68h]
                vt<CreatureCallView>(selected)->attach_to_scene(selected, nullptr, aur_scene);   // 006DC0B2..B5 [edx+94h]
            }
            kotor_m15a_creature_cg_pause_cycle_0060f7c0(element->creature, nullptr, delta, 0);  // 006DC0BB..C4
        }
        std::memcpy(&button_extent, &element->button.extent, sizeof button_extent);   // 006DC0C9..0E [esi-250h]
        std::memcpy(&view_extent, &element->view.extent, sizeof view_extent);         // 006DC0E2..0E [esi-8Ch]
        const I32 top = button_extent.top;                                         // ebp
        float* const timer = &element->timer;                                      // [esi+4]
        const bool timer_unset = rd32(timer, 0) == 0xBF800000u;                    // cmp [esi+4],ebx (-1.0f bits)
        I32 delta_px;
        if (element->button.flags & 1u) {                                          // 006DC0FC test byte [esi-210h],1
            const I32 span = top - 0x76;                                           // 006DC118 lea ecx,[ebp-76h]
            if (span == 0) { st<Word>(timer, 0, 0xBF800000u); continue; }           // 006DC11B -> 006DC2F2
            if (timer_unset) {                                                     // 006DC123..26
                *timer = static_cast<float>(static_cast<double>(span / 10) * 0.25);   // 006DC128..47 (/10; fmul [0073EDF0])
                continue;
            }
            const float left = static_cast<float>(static_cast<double>(*timer) - static_cast<double>(delta));  // 006DC14F..56 fst
            *timer = left;                                                         // 006DC15A fstp [esi+4]
            I32 target;
            if (left <= 0.0f) {                                                    // 006DC15D..6C fcomp [0073D700]; jnp
                st<Word>(timer, 0, 0xBF800000u); target = 0x76;                    // 006DC195..98
            } else {
                target = 0x80 - x87_ftol((0.25 - static_cast<double>(left)) * 4.0 * 10.0);   // 006DC16E..8E
                if (!(target > 0x76)) { st<Word>(timer, 0, 0xBF800000u); target = 0x76; }   // 006DC190..98
            }
            delta_px = top - target;                                               // 006DC19D..9F
            if (delta_px == 0) continue;                                           // 006DC1A1
            const I32 twice = delta_px + delta_px;                                 // 006DC1B9
            button_extent.top = top - delta_px;                                    // 006DC1AF..B1
            button_extent.left -= delta_px;                                        // 006DC1B5
            button_extent.height += twice;                                         // 006DC1BC / C0
            button_extent.width += twice;                                          // 006DC1BE / CE
            element->button.vtbl->set_extent(&element->button, nullptr, &button_extent);   // 006DC1C4..D5 [edx+4]
            view_extent.top -= delta_px;                                           // 006DC1D8..E6
            view_extent.left -= delta_px;                                          // 006DC1DC / EE
            view_extent.width += twice;                                            // 006DC1F0
            view_extent.height += twice;                                           // 006DC1F2
        } else {
            const I32 span = 0x80 - top;                                           // 006DC1F9..FE
            if (span == 0) { st<Word>(timer, 0, 0xBF800000u); continue; }           // 006DC200 -> 006DC2F2
            if (timer_unset) {                                                     // 006DC206..09
                *timer = static_cast<float>(static_cast<double>(span / 10) * 0.25);   // 006DC20B..2A
                continue;
            }
            const float left = static_cast<float>(static_cast<double>(*timer) - static_cast<double>(delta));  // 006DC232..39
            *timer = left;                                                         // 006DC23D
            I32 target;
            if (left <= 0.0f) {                                                    // 006DC240..4F
                st<Word>(timer, 0, 0xBF800000u); target = 0x80;                    // 006DC27B..7E
            } else {
                target = 0x76 - x87_ftol((0.25 - static_cast<double>(left)) * 4.0 * -10.0);  // 006DC251..71 ([0075808C])
                if (!(target < 0x80)) { st<Word>(timer, 0, 0xBF800000u); target = 0x80; }   // 006DC273..7E
            }
            delta_px = target - top;                                               // 006DC283
            if (delta_px == 0) continue;                                           // 006DC285
            const I32 twice = delta_px + delta_px;                                 // 006DC299
            button_extent.top = top + delta_px;                                    // 006DC28F..91
            button_extent.left += delta_px;                                        // 006DC295
            button_extent.height -= twice;                                         // 006DC29C / A0
            button_extent.width -= twice;                                          // 006DC29E / AE
            element->button.vtbl->set_extent(&element->button, nullptr, &button_extent);   // 006DC2A4..B5 [edx+4]
            view_extent.top += delta_px;                                           // 006DC2B8..C6
            view_extent.left += delta_px;                                          // 006DC2BC / CE
            view_extent.width -= twice;                                            // 006DC2D0
            view_extent.height -= twice;                                           // 006DC2D2
        }
        element->view.vtbl->set_extent(&element->view, nullptr, &view_extent);     // 006DC2D4..ED [edx+4] (+1C4)
    }
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                          // 006DC30A..13 CSWGuiPanel::Draw
}
}

// ======================================================================== event callbacks (RET 4)
// 006DB9B0 OnSelectButton(control): events 27h/2Dh of the six class buttons.
void __thiscall ClassSelectionHandlers::OnSelectButton_006db9b0(kih::Control* control) {
    ClassSelection* const self = view(this);
    if (control->last_event_value == 0) return;                                    // 006DB9C7..D2 [edi+4Ch]
    if (control->last_event == 0x2D)                                               // 006DB9D8..DC [edi+48h]
        manager_of(self)->PlayGuiSound(static_cast<char>(control->click_sound));   // 006DB9DE..ED [edi+54h]
    Word index = 0;                                                                // 006DB9F2..05: index of the
    for (; index < 6; ++index)                                                     // element whose base == control
        if (static_cast<void*>(control) == static_cast<void*>(&self->chars[index])) break;
    self->selected = rp(self, 0x2C0u + index * 0x25Cu);                             // 006DBA07..19 [idx*25Ch+esi+2C0h]
    void* panel = nullptr;
    if (void* memory = c2da_directory_new_006fa7e6(0x22DC)) {                      // 006DBA14..32 new(22DCh)
        try {
            panel = kotor_m15c_main_chargen_construct_006eb420(memory, nullptr, self->manager, self->selected, self);  // 006DBA34..3F
        } catch (...) {
            live_free(memory);                                                     // EH state 0 (0072E... funclet) -> 006FA390
            throw;
        }
    }
    kotor_fastcall_0040bc70(self->manager, nullptr, panel, 3, 1);                  // 006DBA48..58 AddPanel(p, 3, 1)
}
// 006DBA70 OnEnterButton(control): event 0 (mouse enter) of the six class buttons -> gender + class name in
// LBL_CLASS and the class description strref in LBL_DESC. The callback is registered only on chars[k].button
// (006DC875..7F), so the searched index is < 6 on every call.
void __thiscall ClassSelectionHandlers::OnEnterButton_006dba70(kih::Control* control) {
    ClassSelection* const self = view(this);
    if (control->last_event_value == 0) return;                                    // 006DBA8E..9D [edx+4Ch]
    int index = 0;                                                                 // 006DBAA3..B5
    for (; index < 6; ++index)
        if (static_cast<void*>(control) == static_cast<void*>(&self->chars[index])) break;
    const KotorM15aClassChoice& choice = kotor_m15a_class_choices_007a2684[index];
    StartupString description;                                                     // 006DBAB7..BB [esp+10h]
    {
        Produced gender;
        simple_string(gender, choice.gender == 0 ? 0x286u : 0x287u);               // 006DBAC0..BB04 [edi*8+007A2685h]
        description.Assign005e5c50(gender.slot());                                 // 006DBB09..0E
    }                                                                              // 006DBB13..1C
    {
        StartupString space(" ");                                                  // 006DBB2E..3D / C7..CD / 4A (0073D5D8)
        Produced name;
        // class byte vs .rdata 0074B31B (02h) then 0074B31A (01h) (006DBB21..33 / 006DBBC1..CB)
        simple_string(name, choice.class_id == 0x02 ? 0x87u : (choice.class_id == 0x01 ? 0x85u : 0x86u));   // 006DBB42..57 / BBD2..E7 / BC4F..64
        Produced first;
        first.take(GuiConcat005e5d10(&description, nullptr, first.slot(), &space));      // 006DBB5E..71 description + " "
        Produced joined;
        joined.take(GuiConcat005e5d10(first.slot(), nullptr, joined.slot(), name.slot()));  // 006DBB76..84 + class name
        description.Assign005e5c50(joined.slot());                                 // 006DBB89..93
    }                                                                              // 006DBB98..CD7 (R2, R1, name, space)
    gui_load_text_params_set_strref_00415e50(at(self, 0x11FC), nullptr, choice.description);  // 006DBCDC..EE LBL_DESC
    gui_load_text_params_set_text_00415e00(at(self, 0x133C), nullptr, &description);           // 006DBCF3..FE LBL_CLASS
}                                                                                  // 006DBD03..0F
