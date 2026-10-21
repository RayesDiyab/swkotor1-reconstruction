// Lane gui-load: CSWGuiMainMenu 0067C4C0 / 0067B500 / 0067B910 / 0067AC10, the
// non-3D DisplayMainMenu callees AddPanel 0040BC70 (+PushModalPanel 0040BD90,
// DetermineNeedToDraw 0040ACC0, deleteAt 0048DDE0) and SetInputClass 006200E0,
// and CExoResMan::CleanDirectory 00409460. Each behavior cites its instructions.
#include "ce2f5dc6ef-mainmenu.h"
#include "f89b4fe050-startup_views.h"   // M40 kotor_menu_10_19::Tlk::simple_0041e8f0 (CTlkTable::GetSimpleString)
#include <cstring>
#include <new>

using namespace gui_load;

// lane-routes client_wrappers.cpp (005FCA30 at 005FCADB) calls the ctor as kotor_fastcall_0067c4c0.
#pragma comment(linker, "/alternatename:@kotor_fastcall_0067c4c0@12=@gui_load_mainmenu_construct_0067c4c0@12")
// lane-r4-lowers consumes 0048DDE0 as kotor_fastcall_0048dde0; single body in gui_objects.cpp (review GL-1).
#pragma comment(linker, "/alternatename:@kotor_fastcall_0048dde0@12=@gui_load_list12_delete_at_0048dde0@12")

namespace {
template<class T> T& at(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(object) + offset);
}
String* as_string(unsigned char* bytes) { return reinterpret_cast<String*>(bytes); }
kih::GuiObject* event_target(MainMenu* menu) { return reinterpret_cast<kih::GuiObject*>(menu); }
template<class F> EventFunc event_func(F f) { return static_cast<EventFunc>(f); }
menu_resources::Manager* resman() { return kotor_resource_demand::resource_manager_007a39e8; }
gui_lb::ListBox* listbox(MainMenu* menu) { return reinterpret_cast<gui_lb::ListBox*>(&menu->lb_modules); }  // fork view of +64
const PanelVtbl* panel_vtbl(Panel* panel) { return reinterpret_cast<const PanelVtbl*>(panel->vtbl); }

// 0067B618..0067B67C: module arrays +354 (0x18-byte elements, dtor 0059EA30) and +344
// (CExoString) released through the array-new count word at [-4], then zeroed.
void release_module_arrays(MainMenu* self) {
    if (Word data = self->u354) {
        const Word count = *reinterpret_cast<const Word*>(data - 4);
        for (Word i = count; i-- > 0;)
            kotor_fastcall_0059ea30(reinterpret_cast<void*>(data + i * 0x18u), nullptr);
        live_free(reinterpret_cast<void*>(data - 4));
    }
    self->u354 = 0;
    if (Word data = self->u344) {
        const Word count = *reinterpret_cast<const Word*>(data - 4);
        for (Word i = count; i-- > 0;)
            reinterpret_cast<String*>(data + i * 8u)->~String();
        live_free(reinterpret_cast<void*>(data - 4));
    }
    self->u344 = 0;
}
// Constructed-member stages of 0067C4C0 (EH states 0..0x0E of its FuncInfo 0072E6A8).
enum Stage { kPanel = 0, kListBox, kView, kScene, kNewGame, kLoadGame, kMovies, kOptions, kB00,
             kNewContent, kLogo, kMenuBg, kExit, kWarp, kDone };
void unwind_mainmenu(MainMenu* self, int built) {
    if (built > kWarp) gui_load_button_destruct_00419da0(&self->btn_warp, nullptr);
    if (built > kExit) gui_load_button_destruct_00419da0(&self->btn_exit, nullptr);
    if (built > kMenuBg) gui_load_label_destruct_00419d10(&self->lbl_menubg, nullptr);
    if (built > kLogo) gui_load_label_destruct_00419d10(&self->lbl_gamelogo, nullptr);
    if (built > kNewContent) gui_load_label_destruct_00419d10(&self->lbl_newcontent, nullptr);
    if (built > kB00) gui_load_button_destruct_00419da0(&self->btn_b00, nullptr);
    if (built > kOptions) gui_load_button_destruct_00419da0(&self->btn_options, nullptr);
    if (built > kMovies) gui_load_button_destruct_00419da0(&self->btn_movies, nullptr);
    if (built > kLoadGame) gui_load_button_destruct_00419da0(&self->btn_loadgame, nullptr);
    if (built > kNewGame) gui_load_button_destruct_00419da0(&self->btn_newgame, nullptr);
    if (built > kScene) gui_load_scene_view_destruct_0067ac50(&self->lbl_3dview, nullptr);
    else if (built > kView) gui_load_control_destruct_00419a20(&self->lbl_3dview, nullptr);
    if (built > kListBox) {
        release_module_arrays(self);                                              // EH states 1-2 (+354 / +344)
        gui_load_listbox_destruct_0041b060(listbox(self), nullptr);
    }
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
}

extern "C" {

volatile std::uint32_t kotor_cell_do3dgui_0078d1e4 = 1;   // .data 0078D1E4 image value 00000001

// 00409460 CExoResMan::CleanDirectory(name, a2, a3) RET 0Ch: tail of
// WipeDirectory(name, 1, 0, a3, a2) with ECX unchanged (00409460..73).
Word __fastcall gui_load_resman_clean_directory_00409460(menu_resources::Manager* self, void*, String* name,
                                                        Word a2, Word a3) noexcept(false) {
    return gui_load_resman_wipe_directory_00408e90(self, nullptr, name, 1, 0, a3, a2);
}

// 0067C4C0 CSWGuiMainMenu::CSWGuiMainMenu(manager) RET 4.
MainMenu* __fastcall gui_load_mainmenu_construct_0067c4c0(MainMenu* self, void*, Object* manager) noexcept(false) {
    gui_load_panel_construct_0040b570(self, nullptr, manager);                    // 0067C4E7
    int built = kListBox;
    try {
        self->vtbl = reinterpret_cast<const ObjectVtbl*>(&gui_load_mainmenu_methods_00752f70); // 0067C4F5
        gui_load_listbox_construct_0041be40(listbox(self), nullptr);           // 0067C4FB
        built = kView;
        self->u348 = 0; self->u34c = 0; self->u344 = 0;                            // 0067C500..0C
        self->u358 = 0; self->u35c = 0; self->u354 = 0;                            // 0067C512..1E
        gui_load_control_construct_0041aa80(&self->lbl_3dview, nullptr);          // 0067C535
        built = kScene;
        self->lbl_3dview.vtbl = reinterpret_cast<const ObjectVtbl*>(&gui_load_scene_view_methods_00752e30); // 0067C542
        gui_scene_construct_004174b0(self->scene, nullptr);                        // 0067C548 (+3BC = view+5C)
        built = kNewGame;
        gui_load_button_construct_0041c0f0(&self->btn_newgame, nullptr);  built = kLoadGame;  // 0067C55A
        gui_load_button_construct_0041c0f0(&self->btn_loadgame, nullptr); built = kMovies;    // 0067C56C
        gui_load_button_construct_0041c0f0(&self->btn_movies, nullptr);   built = kOptions;   // 0067C57C
        gui_load_button_construct_0041c0f0(&self->btn_options, nullptr);  built = kB00;       // 0067C58C
        gui_load_button_construct_0041c0f0(&self->btn_b00, nullptr);      built = kNewContent;// 0067C59C
        gui_load_label_construct_0041acd0(&self->lbl_newcontent, nullptr); built = kLogo;     // 0067C5AC
        // 0067C5CE eh_vector_constructor_iterator(+E04, 0x140, 2, 0041ACD0, 00419D10):
        // element 1 failing destroys element 0 before propagating.
        gui_load_label_construct_0041acd0(&self->lbl_gamelogo, nullptr);
        try { gui_load_label_construct_0041acd0(&self->lbl_menubg, nullptr); }
        catch (...) { gui_load_label_destruct_00419d10(&self->lbl_gamelogo, nullptr); throw; }
        built = kExit;
        gui_load_button_construct_0041c0f0(&self->btn_exit, nullptr);     built = kWarp;      // 0067C5DE
        gui_load_button_construct_0041c0f0(&self->btn_warp, nullptr);     built = kDone;      // 0067C5F0

        Ref module;
        module.construct_00406d80("MAINMENU");                                     // 0067C5FA..603
        if (resman()->exists_00408bc0(&module, 0xBBA, nullptr)) {                  // 0067C608..61A
            String rim("RIMS:MAINMENU");                                           // 0067C62C
            resman()->image_004087c0(&rim);                                        // 0067C641
        }                                                                          // 0067C64F
        unsigned char byte34;
        std::memcpy(&byte34, resman()->bytes + 0x34, 1);
        byte34 = static_cast<unsigned char>(byte34 & 0xFD);                        // 0067C659
        std::memcpy(resman()->bytes + 0x34, &byte34, 1);
        gui_load_mainmenu_load_layout_0067ace0(self, nullptr,
            static_cast<Word>(reinterpret_cast<std::uintptr_t>(manager)));         // 0067C664
        self->btn_warp.flags &= 0xFFFFFFFDu;                                       // 0067C669..73 (+128C)
        self->lbl_newcontent.flags &= 0xFFFFFFFDu;                                 // 0067C676 (+D08)
        self->lb_modules.flags &= 0xFFFFFFFDu;                                     // 0067C67C..8A (+A8)

        Control* const picks[6] = { &self->btn_newgame, &self->btn_loadgame, &self->btn_movies,
                                    &self->btn_options, &self->btn_warp, &self->btn_exit };
        const EventFunc handlers[6] = {
            event_func(&MainMenuHandlers::OnNewGamePicked),      // 0067AFB0
            event_func(&MainMenuHandlers::OnLoadSaveGame),       // 0067B1A0
            event_func(&MainMenuHandlers::OnMoviesPicked),       // 0067B250
            event_func(&MainMenuHandlers::OnOptionsPicked),      // 0067B2F0
            event_func(&MainMenuHandlers::OnWarpButtonPressed),  // 0067C4B0
            event_func(&MainMenuHandlers::OnQuitButtonPressed),  // 0067B4A0
        };
        for (int i = 0; i < 6; ++i)                                                // 0067C682..6EB (event 0x27)
            gui_load_control_add_event_0041ab20(picks[i], nullptr, 0x27, event_target(self), handlers[i]);
        for (int i = 0; i < 6; ++i)                                                // 0067C6F0..751 (event 0x2D)
            gui_load_control_add_event_0041ab20(picks[i], nullptr, 0x2D, event_target(self), handlers[i]);
        for (int i = 0; i < 6; ++i)                                                // 0067C756..7B7 (event 0)
            gui_load_control_add_event_0041ab20(picks[i], nullptr, 0, event_target(self),
                event_func(&kih::MainMenu::OnEnterButton));                        // 0067B450
        for (int i = 0; i < 6; ++i)                                                // 0067C7BC..81D (event 1)
            gui_load_control_add_event_0041ab20(picks[i], nullptr, 1, event_target(self),
                event_func(&kih::MainMenu::OnExitButton));                         // 0067B470

        // 0067C822..0067C8F9: 3D scene (calls only; bodies owned by the 3D lanes).
        const std::uint32_t do3d = kotor_cell_do3dgui_0078d1e4;
        GuiScene* scene = reinterpret_cast<GuiScene*>(self->scene);
        void* camera = scene->aur_camera;                                          // 0067C829 (+3D4)
        void* aur_scene = scene->aur_scene;                                        // 0067C82F (+3D0)
        if (do3d) {
            const float position[3] = { 0.0f, 0.0f, 0.0f };                       // 0067C86C..7C
            const float orientation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };               // 0067C84C..64
            const AurSceneVtbl* svt;
            std::memcpy(&svt, aur_scene, sizeof svt);
            svt->spawn_room(aur_scene, nullptr, "gui3D_room", position, orientation);   // 0067C884
            void* model;
            {
                String name("mainmenu");                                           // 0067C890
                model = aurora_gui_scene_add_model_name_00417620(self->scene, nullptr, &name, -1);  // 0067C8A7
            }                                                                      // 0067C8B7
            if (model) {
                const AurModelVtbl* mvt;
                std::memcpy(&mvt, model, sizeof mvt);
                mvt->play_animation(model, nullptr, "default", 1.0f, 0, 0);        // 0067C8D2
                std::memcpy(&mvt, model, sizeof mvt);
                mvt->set_anim_scale(model, nullptr, 1.0f);                         // 0067C8DE
                const AurCameraVtbl* cvt;
                std::memcpy(&cvt, camera, sizeof cvt);
                cvt->attach_to_object(camera, nullptr, model, "camerahook", 0);    // 0067C8ED
                std::memcpy(&cvt, camera, sizeof cvt);
                const Word fov_bits = 0x41B5CED9u;                                 // 0067C8F2 (22.7259...f)
                float fov; std::memcpy(&fov, &fov_bits, 4);
                cvt->set_view_angle(camera, nullptr, fov);                         // 0067C8F9
            }
        }
        gui_load_panel_set_active_control_0040a630(self, nullptr, &self->btn_newgame, 0);  // 0067C902
        self->u350 = 1;                                                            // 0067C910
        { String s("HD0:GAMEINPROGRESS"); gui_load_resman_remove_file_00406b20(resman(), nullptr, &s, 0x56); }   // 0067C907..93F
        { String s("GAMEINPROGRESS:"); gui_load_resman_clean_directory_00409460(resman(), nullptr, &s, 0, 0); } // 0067C944..972
        { String s("HD0:CURRENTGAME"); gui_load_resman_remove_file_00406b20(resman(), nullptr, &s, 0x56); }    // 0067C977..9A5
        { String s("CURRENTGAME:"); gui_load_resman_clean_directory_00409460(resman(), nullptr, &s, 0, 0); }    // 0067C9AA..9D8
        { String s("REBOOTDATA:"); gui_load_resman_clean_directory_00409460(resman(), nullptr, &s, 0, 0); }     // 0067C9DD..A0B
        table29_required::gui_sound_mode_005d5e80(kotor_cell_sound_007a39ec, nullptr,
                                                  *sound_mode29_required::word_0074c5ec);   // 0067CA10..1C
    } catch (...) {
        unwind_mainmenu(self, built);
        throw;
    }
    self->u140c = 0;                                                               // 0067CA21
    self->u1410 = 0;                                                               // 0067CA27
    return self;
}

// 0067B500 dtor: vptr 00752F70; ListBox ClearItems (0067B534); CResRef "MAINMENU"
// Exists -> resman+34 |= 2 (0067B539..67); members destroyed in reverse; module
// arrays +354 (0x18-byte elements, dtor 0059EA30) and +344 (CExoString) released
// through the array-new count word at [-4] (0067B618..7C); ListBox; Panel dtor.
void __fastcall gui_load_mainmenu_destruct_0067b500(MainMenu* self, void*) {
    self->vtbl = reinterpret_cast<const ObjectVtbl*>(&gui_load_mainmenu_methods_00752f70);
    gui_load_listbox_clear_items_00419a90(listbox(self), nullptr);
    Ref module;
    module.construct_00406d80("MAINMENU");
    if (resman()->exists_00408bc0(&module, 0xBBA, nullptr)) {
        unsigned char byte34;
        std::memcpy(&byte34, resman()->bytes + 0x34, 1);
        byte34 = static_cast<unsigned char>(byte34 | 2);
        std::memcpy(resman()->bytes + 0x34, &byte34, 1);
    }
    gui_load_button_destruct_00419da0(&self->btn_warp, nullptr);
    gui_load_button_destruct_00419da0(&self->btn_exit, nullptr);
    // 0067B5A3 eh_vector_destructor_iterator(+E04, 0x140, 2, 00419D10): reverse order.
    gui_load_label_destruct_00419d10(&self->lbl_menubg, nullptr);
    gui_load_label_destruct_00419d10(&self->lbl_gamelogo, nullptr);
    gui_load_label_destruct_00419d10(&self->lbl_newcontent, nullptr);
    gui_load_button_destruct_00419da0(&self->btn_b00, nullptr);
    gui_load_button_destruct_00419da0(&self->btn_options, nullptr);
    gui_load_button_destruct_00419da0(&self->btn_movies, nullptr);
    gui_load_button_destruct_00419da0(&self->btn_loadgame, nullptr);
    gui_load_button_destruct_00419da0(&self->btn_newgame, nullptr);
    gui_load_scene_view_destruct_0067ac50(&self->lbl_3dview, nullptr);
    release_module_arrays(self);                                               // 0067B618..0067B67C
    gui_load_listbox_destruct_0041b060(listbox(self), nullptr);
    gui_load_panel_destruct_0040cf70(self, nullptr);
}
void* __fastcall gui_load_mainmenu_deleting_destruct_0067b910(MainMenu* self, void*, Word flags) {
    gui_load_mainmenu_destruct_0067b500(self, nullptr);                            // 0067B913
    if (flags & 1u) live_free(self);                                               // 0067B918..20
    return self;
}
// 0067AC10 Draw(delta) RET 4: CSWGuiPanel::Draw (0067AC18); frame count +1410 > 4
// (signed) -> +140C = 1, else ++count.
void __fastcall gui_load_mainmenu_draw_0067ac10(Panel* panel, void*, Word delta) {
    MainMenu* self = static_cast<MainMenu*>(panel);
    kotor_panel_draw_0040B760(self, nullptr, delta);
    const std::int32_t count = static_cast<std::int32_t>(self->u1410);
    if (count > 4) self->u140c = 1;
    else self->u1410 = static_cast<Word>(count + 1);
}

// ------------------------------------------------------------------ CSWGuiManager
// 0040ACC0 DetermineNeedToDraw (transcribed 0040ACE2..0040ADDB).
void __fastcall kotor_fastcall_0040acc0(void* manager, void*) noexcept(false) {
    kih::Manager* m = static_cast<kih::Manager*>(manager);
    String* background_name = reinterpret_cast<String*>(static_cast<unsigned char*>(manager) + 0x74);
    bool found = false;
    auto mark = [&](Panel* panel) {
        Word flags = panel->flags;
        if (found) { panel->flags = flags & 0xFFFFFF7Fu; return; }
        flags |= 0x80u;
        panel->flags = flags;
        if (!(flags & 8u)) return;
        found = true;
        alignas(4) unsigned char bytes[8];
        String* name = panel_vtbl(panel)->full_screen_bg(panel, nullptr, reinterpret_cast<String*>(bytes));
        try { background_name->Assign005e5c50(name); } catch (...) { name->~String(); throw; }
        name->~String();
    };
    for (std::int32_t i = m->modal_count - 1; i >= 0; --i) {
        Panel* panel = reinterpret_cast<Panel*>(m->modal_panels[i]);
        if (!panel || !(panel->flags & 4u)) continue;
        mark(panel);
    }
    for (std::int32_t i = m->panel_count - 1; i >= 0; --i) {
        Panel* panel = reinterpret_cast<Panel*>(m->panels[i]);
        if (!panel || !(panel->flags & 4u)) continue;
        if (frame_FindPopup_0040ab70(manager, nullptr, panel) != 0xFFFFFFFFu) continue;
        mark(panel);
    }
}

// 0040BD90 PushModalPanel(panel) RET 4.
Word __fastcall gui_load_manager_push_modal_0040bd90(void* manager, void*, Panel* panel) noexcept(false) {
    if (!panel) return 0;
    kih::Manager* m = static_cast<kih::Manager*>(manager);
    if (Object* over = reinterpret_cast<Object*>(m->mouse_over))
        over->vtbl->handle_mouse_over(over, nullptr, 0);                           // 0040BDA7
    if (kotor_fastcall_0040aba0(manager, nullptr, panel) == 0xFFFFFFFFu)
        kotor_fastcall_0040bc70(manager, nullptr, panel, 0, 1);                    // 0040BDBE
    if (frame_FindPopup_0040ab70(manager, nullptr, panel) != 0xFFFFFFFFu) return 0;
    // lane-m1-verify-panels fix: the original reads CSWGuiManager+18h here (0040BDD7 MOV ECX,[ESI+18h]),
    // not +14h (last hit control, written by UpdateMouseOverControl 0040B342). +18h is zeroed by the
    // manager ctor (0040BBAA); the call is skipped while it is 0.
    if (Object* focus = at<Object*>(manager, 0x18))                                // 0040BDD7..DC
        focus->vtbl->focus_change(focus, nullptr, 0);                              // 0040BDE0..E2 vslot +7C
    gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(
        static_cast<unsigned char*>(manager) + 0x94), nullptr,
        static_cast<Word>(reinterpret_cast<std::uintptr_t>(panel)));               // 0040BDEC
    return 1;
}

// 0040BC70 AddPanel(panel, modal_flags, sound) RET 0Ch (consumer name).
void __fastcall kotor_fastcall_0040bc70(void* manager, void*, void* panel_object, std::uint32_t mode,
                                        std::uint32_t sound) noexcept(false) {
    Panel* panel = static_cast<Panel*>(panel_object);
    if (!panel) return;
    kih::Manager* m = static_cast<kih::Manager*>(manager);
    if (sound) m->PlayGuiSound(static_cast<char>(panel->b60));                     // 0040BC88..8E
    if (kotor_fastcall_0040aba0(manager, nullptr, panel) != 0xFFFFFFFFu) {
        const Word flags = panel->flags;
        const Word state = (flags >> 9) & 3u;                                      // 0040BCA3..A8
        if (state != 2 && state == 0) return;                                      // 0040BCAB..B2
        panel->flags = flags & 0xFFFFF8FFu;                                        // 0040BCB8..BE
        if (mode & 1u) gui_load_manager_push_modal_0040bd90(manager, nullptr, panel);
    } else {
        Word flags = panel->flags;
        flags = flags ^ (((mode * 4u) ^ flags) & 0x18u);                           // 0040BCCD..D9
        panel->flags = flags;
        unsigned char* bytes = static_cast<unsigned char*>(manager);
        if (flags & 8u) ++bytes[0x70];                                             // 0040BCE4
        if (!(panel->flags & 0x10u)) ++bytes[0x71];                                // 0040BCE7..ED
        gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(bytes + 0x88),
            nullptr, static_cast<Word>(reinterpret_cast<std::uintptr_t>(panel)));  // 0040BCF7
        if (mode & 1u) gui_load_manager_push_modal_0040bd90(manager, nullptr, panel);
    }
    panel_vtbl(panel)->on_panel_added(panel, nullptr);                             // 0040BD0E
    kotor_fastcall_0040acc0(manager, nullptr);                                     // 0040BD13
}

// ------------------------------------------------------------------ CClientExoAppInternal
// 006200E0 SetInputClass(class, show) RET 8 (jump table 00620310: 0 -> 0062015C,
// 1 -> 00620127, 2 -> 00620295, 3 -> 00620280).
void __fastcall kotor_fastcall_006200e0(void* internal, void*, std::uint32_t input_class, std::uint32_t show) noexcept(false) {
    if (at<std::uint32_t>(internal, 0x9C) == input_class) return;                 // 006200E6..F3
    for (;;) {
        const std::uint32_t previous = at<std::uint32_t>(internal, 0x9C);         // 00620100
        at<std::uint32_t>(internal, 0x9C) = input_class;                          // 00620106
        kotor_fastcall_005df4e0(kotor_menu_window_cells::input_007a39e4, nullptr);  // 0062010C..12
        if (input_class > 3) return;                                               // 0062011A
        void* in_game = nullptr;
        switch (input_class) {
        case 1: {
            void* module = at<void*>(internal, 0x18);
            void* mini = module ? at<void*>(module, 0x48) : nullptr;
            void* game = mini ? at<void*>(mini, 0x264) : nullptr;
            if (game) {                                                            // 006202CC
                kotor_fastcall_005f2c60(internal, nullptr, 0, 0);
                kotor_fastcall_0061f9c0(internal, nullptr, 1);
                if ((in_game = at<void*>(internal, 0x40)) != nullptr) frame_R4_0062aa00(in_game, nullptr, 2, 0);
                kotor_fastcall_00671670(game, nullptr, at<void*>(at<void*>(internal, 0x18), 0x40));
                kotor_fastcall_006714e0(game, nullptr);
                return;
            }
            if (at<std::uint32_t>(internal, 0x9C) == previous) return;            // 00620143..50
            show = 1;                                                              // 00620149
            input_class = previous;
            continue;
        }
        case 0: {
            if (at<std::uint32_t>(internal, 0x28C) != 0x7F000000u) {               // 0062015C..69
                void* app = kotor_menu_window_cells::manager_007a39fc;
                void* creature = kotor_fastcall_004b2820(at<void*>(app, 8), nullptr);
                if (creature) {
                    const std::uint32_t id = kotor_fastcall_005eda50(
                        at<void*>(kotor_menu_window_cells::manager_007a39fc, 4), nullptr, at<std::uint32_t>(creature, 4));
                    kotor_fastcall_005f2bb0(internal, nullptr, id);
                    at<std::uint32_t>(internal, 0x28C) = 0x7F000000u;
                    kotor_fastcall_005f5cf0(internal, nullptr);
                }
            }
            void* module = at<void*>(internal, 0x18);
            if (module && at<void*>(module, 0x40)) {                               // 006201A3..AF
                std::uint32_t position[3] = { 0, 0, 0 };                           // 006201B3..C3
                if (kotor_fastcall_005f1420(internal, nullptr)) {
                    const std::uint32_t* p = reinterpret_cast<const std::uint32_t*>(
                        static_cast<unsigned char*>(kotor_fastcall_005f1420(internal, nullptr)) + 0x30);
                    position[0] = p[0]; position[1] = p[1]; position[2] = p[2];
                }
                void* area = at<void*>(at<void*>(internal, 0x18), 0x40);
                struct AreaVtbl { const void* slots[0x20]; void* (__fastcall* object_by_id)(void*, void*, std::uint32_t); };
                const AreaVtbl* avt;
                std::memcpy(&avt, area, sizeof avt);
                void* object = avt->object_by_id(area, nullptr, 0x106A);           // 006201FA..00620201 vslot +80
                if (object) {
                    at<std::uint32_t>(object, 0x124) = 0;
                    at<std::uint32_t>(object, 0x128) = position[0];
                    at<std::uint32_t>(object, 0x12C) = position[1];
                    at<std::uint32_t>(object, 0x130) = position[2];
                }
            }
            if (kotor_fastcall_005f1420(internal, nullptr)) {                      // 00620222..2B
                static const std::uint32_t rdata_0074f224 = 1;                     // .rdata 0074F224 image value 1
                kotor_fastcall_0060f090(kotor_fastcall_005f1420(internal, nullptr), nullptr, rdata_0074f224);
            }
            if (void* control = at<void*>(internal, 0x2A0)) kotor_fastcall_006792e0(control, nullptr, 1);
            kotor_game_prepare::ShowMouse0061fa00(internal, nullptr, 1);           // 00620257
            if ((in_game = at<void*>(internal, 0x40)) != nullptr) frame_R4_0062aa00(in_game, nullptr, 4, 1);
            at<std::uint32_t>(at<void*>(internal, 0x40), 0x30) = 0;                // 0062026C..72 (no null check)
            return;
        }
        case 2: {
            in_game = at<void*>(internal, 0x40);
            if (in_game && at<std::uint32_t>(in_game, 0x34) == 1) {               // 00620298..A0
                if (show) frame_R4_0062aa00(in_game, nullptr, 3, 1);               // 006202A2..AA
                kotor_game_prepare::ShowMouse0061fa00(internal, nullptr, 1);       // 006202B3
            }
            at<std::uint32_t>(at<void*>(internal, 0x40), 0x30) = 1;                // 006202B8..BE (no null check)
            return;
        }
        default:                                                                   // 3
            kotor_fastcall_005f2c60(internal, nullptr, 0, 0);                      // 00620286
            return;
        }
    }
}

// ------------------------------------------------------------------ rev2: OnPanelAdded

// 00624A40 CSWGuiMessageBox::SetCallback(object, func, argument) RET 0Ch:
// +6C = object (00624A48), +68 = func (00624A4F), +70 = argument (00624A52).
void __fastcall gui_load_messagebox_set_callback_00624a40(void* box, void*, kih::GuiObject* object,
                                                          EventFunc func, Word argument) {
    at<kih::GuiObject*>(box, 0x6C) = object;
    at<EventFunc>(box, 0x68) = func;
    at<Word>(box, 0x70) = argument;
}

// 0067B6C0 CSWGuiMainMenu::OnPanelAdded (vtable 00752F70 +48; AddPanel 0040BD0E).
void __fastcall gui_load_mainmenu_on_added_0067b6c0(Panel* panel, void*) noexcept(false) {
    MainMenu* self = static_cast<MainMenu*>(panel);
    std::uint64_t free_bytes;                                                     // [esp+30h] qword out
    {
        String saves("SAVES:");                                                   // 0067B6E4..ED (00745174)
        gui_load_resman_free_disk_space_004067b0(resman(), nullptr, &saves, &free_bytes);   // 0067B6F2..70A
    }                                                                             // 0067B71B
    // 0067B720..30: SHRD/SHR by 14 -> free space in 16 KiB units (EBX low, EDI high).
    const Word free_low = static_cast<Word>(free_bytes >> 14);
    const Word free_high = static_cast<Word>(free_bytes >> 46);
    Word need_low = 0xC80;                                                        // 0067B733
    Word need_high = 0;                                                           // 0067B738
    String path;                                                                  // 0067B740 (005B3190)
    kotor_reconstruction::StartupFormat005e5680(&path, "SAVES:%06d - %s", 1, "AUTOSAVE");   // 0067B745..5E
    const Word created = gui_load_resman_create_directory_004068c0(resman(), nullptr, &path);   // 0067B771
    if (created) {                                                                // NEG/SBB/INC/JNE 0067B776..7F
        need_low = 0x12C0;                                                        // 0067B790
        need_high = 0;                                                            // 0067B795
        gui_load_resman_nuke_directory_00409480(resman(), nullptr, &path, 1, 1);  // 0067B781..9D
    }
    const Word sum = need_low + 2u;                                               // 0067B7A6 ADD
    need_high = need_high + (sum < need_low ? 1u : 0u);                           // 0067B7A9 ADC
    need_low = sum;
    const bool enough = static_cast<std::int32_t>(need_high) < static_cast<std::int32_t>(free_high) ||
                        (need_high == free_high && need_low <= free_low);         // 0067B7AC..B8 (JL / JG / JBE)
    if (enough) {
        kotor_fastcall_005ee180(at<void*>(kotor_menu_window_cells::manager_007a39fc, 4), nullptr, 0xFFFF);  // 0067B8D2..E0
        return;                                                                   // path dtor at scope exit (0067B8F1)
    }
    // 0067B7C7..DA: deficit = (need_low - free_low) << 14 as int; FILD; FMUL dword 007439BC; FSTP dword.
    std::int32_t deficit = static_cast<std::int32_t>((need_low - free_low) << 14);
    static const Word scale_bits = 0x358637BDu;                                   // .rdata 007439BC (1.0e-6f)
    float megabytes;
    __asm {
        fild dword ptr deficit
        fmul dword ptr scale_bits
        fstp dword ptr megabytes
    }
    void* in_game = frame_R4_005ed690(at<void*>(kotor_menu_window_cells::manager_007a39fc, 4), nullptr);  // 0067B7BE..DE
    void* box = at<void*>(in_game, 0x98);                                        // 0067B7E3
    String message;                                                               // 0067B7ED [state 2]
    String number;                                                                // 0067B7FB [state 3]
    kotor_reconstruction::StartupFormat005e5680(&number, "%2.3f", static_cast<double>(megabytes));   // 0067B800..1A
    kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, 0, &number);       // 0067B822..2F
    {
        alignas(4) unsigned char bytes[8];
        kotor_menu_10_19::String* simple = static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08)
            ->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(bytes), 0xBB78);  // 0067B834..44
        String* fetched = reinterpret_cast<String*>(simple);
        try { message.Assign005e5c50(fetched); } catch (...) { reinterpret_cast<String*>(bytes)->~String(); throw; }
        reinterpret_cast<String*>(bytes)->~String();                              // 0067B860
    }
    tlk_core::ParseStr0041dd30(static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08), nullptr, &message);  // 0067B865..70
    kotor_fastcall_00627130(box, nullptr, 0);                                     // 0067B875..79
    {
        // 0067B87E..95: CExoString copy (005E5B00) built in the outgoing argument slot and
        // passed by value to box vslot +70; the callee owns (destroys) the argument.
        struct StringBits { Word data, capacity; };
        StringBits argument;
        WindowCopy005e5b00(reinterpret_cast<String*>(&argument), nullptr, &message);
        struct BoxVtbl { const void* slots[0x1C]; void (__fastcall* set_text)(void*, void*, StringBits); };
        const BoxVtbl* vt;
        std::memcpy(&vt, box, sizeof vt);
        vt->set_text(box, nullptr, argument);
    }
    gui_load_messagebox_set_callback_00624a40(box, nullptr, event_target(self),
        event_func(&MainMenuHandlers::LaunchDashboard), 0);                       // 0067B898..A2 (0067B490)
    kotor_fastcall_0040bc70(self->manager, nullptr, box, 1, 1);                   // 0067B8A7..AF
}                                                                                 // ~number, ~message, ~path (0067B8BD/CB/F1)

} // extern "C"
