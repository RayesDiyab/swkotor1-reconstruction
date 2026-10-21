// Lane W4 G15 (gui-hud, part 1): in-game HUD draw path reached from CSWGuiMainInterface::Draw (0068B4A0) and the
// in-game map hider, replacing the gui_slot_traps_successor.cpp trap lines of the same names/signatures.
// Transcribed from linear-sweep listings of swkotor.exe (claude-runs/tools/win.py <VA>); PCs cited per statement.
//
//   00686790 CSWGuiCharButton::Draw(float)                 thiscall RET 4
//   00686750 CSWGuiCharButton::DisplayToolTip() -> int      thiscall RET 0
//   0067E860 CSWGuiSpeedButton::Draw(float)                thiscall RET 4
//   0067E800 CSWGuiSpeedButton::HandleLMouseDown()         thiscall RET 0
//   0067E830 CSWGuiSpeedButton::HandleRMouseDown()         thiscall RET 0
//   006943D0 CSWGuiMapHider::Draw(float)                   thiscall RET 4
//   00693300 CSWGuiMapHider::HitCheckMouse(int, int) -> CSWGuiControl*   thiscall RET 8
//   006C00B0 CSWGuiPanel::Draw(float) (CSWGuiInGamePause slot; `jmp 0040B760`)  thiscall RET 4
// Callees reconstructed here (no rebuilt body existed in the c24 candidate):
//   00684F50 CSWGuiCharButton::UpdateToolTipText()                         RET 0
//   0040A340 CSWGuiManager::ChangeToolTipText(CExoString*)                 RET 4
//   005791F0 CSWSAreaMap::GetPartyMemberMapOrientation(int) -> float (ST0) RET 4
//   00579210 CSWSAreaMap::IsWorldPointExplored(Vector) -> int              RET 0Ch
//   00693E70 CSWGuiInGameMap::OnMapNoteClicked(CSWGuiControl*)             RET 4 (event 0x27 handler)
//   006929B0 CSWGuiInGameMap::SetMapNote(CExoString*)                      RET 4
//   006932A0 CSWGuiMapHider::SetMapNote(unsigned long)                     RET 4
// Data storage defined here (no rebuilt storage existed): [007A2678] and [007A267C] (.data, both initially 1;
// 007A267C is toggled by the `revealmap` console command at 0060ABB0).
// Original quirks kept: MapHider::Draw truncates the note-button count without destroying the surplus buttons
// (006949E0..006949E8), adds an unchecked null button if operator new fails (006947B2), and accepts a grid word
// index equal to the map size (00694510 jg / 0057928D jle, one word past the end).
#include "78d2073e39-gui_panels.hpp"
using namespace w4gui;

namespace kotor_menu_window_cells { extern void* volatile gui_manager_007a39f4; }               // [007A39F4] CSWGuiManager*
namespace kotor_menu_10_19 {
struct String;
struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned strref); };                 // CTlkTable::GetSimpleString
}
namespace kotor_input_hover { struct Control { void __thiscall HandleLMouseDown(); }; }           // 0041AB90 CSWGuiControl::HandleLMouseDown
namespace kotor_reconstruction { struct StartupString; }
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);                       // CExoString::operator+

extern "C" {
// ---- controls / panel / manager
void __fastcall kotor_fastcall_00417ab0(void* button, void*, U32 delta_bits);                    // CSWGuiButton::Draw
void __fastcall gui_load_control_rmouse_down_0041ac30(void* control, void*);                    // CSWGuiControl::HandleRMouseDown
void __fastcall kotor_panel_draw_0040B760(void* panel, void*, U32 delta_bits);                   // CSWGuiPanel::Draw
void __fastcall gui_m48_manager_display_tooltip_0040b490(void* manager, void*, void* text) noexcept(false); // CSWGuiManager::DisplayToolTip
void __fastcall gui_m48_tooltip_panel_set_text_00624ae0(void* panel, void*, void* text) noexcept(false);   // CSWGuiToolTip::SetText
void __fastcall kotor_fastcall_0040c1e0(void* manager, void*, U32 x, U32 y);                     // CSWGuiManager::HandleMouseMove
void* __fastcall gui_load_text_params_construct_00417050(void* self, void*);                    // CSWGuiTextParams::CSWGuiTextParams
void* __fastcall gui_load_border_params_construct_00415000(void* self, void*, const void* corner, const void* edge,
    const void* fill, U32 fill_style, I32 dimension, float alpha, const void* color, I32 pulsing) noexcept(false); // CSWGuiBorderParams
void* __fastcall c2da_default_ref_00405ed0(void* self, void*);                                   // CResRef::CResRef()
void __fastcall gui_load_array_add_00671c00(void* array, void*, U32 value);                      // CExoArrayList<>::Add
void __fastcall kotor_fastcall_00692810(void* map, void*, U32 delta_bits);                       // CSWGuiInGameMap::Draw (lane gui-journal)
// ---- Aurora GUI image / viewport
U32 __cdecl kotor_AurGUISetupViewport_004592F0(U32 x, U32 y, U32 w, U32 h, const void* color, U32 depth_test_zero, U32 alpha_bits);
void __stdcall kotor_AurGUICloseViewport_00459580();                                            // AurGUICloseViewport
void __cdecl kotor_cdecl_0045a170(U32 alpha_bits);                                              // CAurGUIImage::FlushBuffer
extern const U32 kotor_gui_image_alpha_0078e43c;                                                // [0078E43C] (1.0f)
extern const unsigned char scene3d_gui_scene_color_0078d3d8[12];                                // [0078D3D8]
extern const unsigned char borrowed_exit_color_007a23b4[12];                                    // [007A23B4]
extern const unsigned char borrowed_enter_color_007a23c0[12];                                   // [007A23C0]
// ---- server / area map / client
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                // CServerExoApp::GetModule
U32 __fastcall kotor_fastcall_00579090(void* map, void*, U32* size, I32* res_x, I32* res_y);     // CSWSAreaMap::GetMapData
U32 __fastcall kotor_fastcall_005791b0(void* map, void*, I32 index, I32* x, I32* y);            // CSWSAreaMap::GetPartyMemberMapLocation
U32 __fastcall kotor_fastcall_00578e00(void* map, void*, W4Vector world, I32* x, I32* y);        // CSWSAreaMap::GetMapPixelFromWorldCoord
U32 __fastcall kotor_fastcall_00578f10(void* map, void*, I32 x, I32 y, I32* out_x, I32* out_y);  // CSWSAreaMap::GetGridPixelFromMapPixel
void* __fastcall kotor_fastcall_005ed8b0(void* client, void*);                                   // CClientExoApp::GetSWParty
U32 __fastcall kotor_fastcall_005ed4c0(void* client, void*);                                     // CClientExoApp::GetClientLanguage
U32 __fastcall exo_locstring_get_005ea130(void* loc, void*, U32 language, void* out, U32 gender); // CExoLocString::GetString
std::int32_t __fastcall async_string_length_005e5790(void* self, void*);                         // CExoString::GetLength
// ---- this unit
U32 __fastcall kotor_fastcall_00686750(void* self, void*);
void __fastcall kotor_fastcall_00686790(void* self, void*, U32 delta_bits);
void __fastcall kotor_fastcall_0067e800(void* self, void*);
void __fastcall kotor_fastcall_0067e830(void* self, void*);
void __fastcall kotor_fastcall_0067e860(void* self, void*, U32 delta_bits);
void __fastcall kotor_fastcall_006943d0(void* self, void*, U32 delta_bits);
void* __fastcall kotor_fastcall_00693300(void* self, void*, I32 x, I32 y);
void __fastcall kotor_fastcall_006c00b0(void* self, void*, U32 delta_bits);
void __fastcall kotor_fastcall_00684f50(void* self, void*);
void __fastcall kotor_fastcall_0040a340(void* manager, void*, void* text);
float __fastcall kotor_fastcall_005791f0(void* map, void*, I32 index);
U32 __fastcall kotor_fastcall_00579210(void* map, void*, W4Vector world);
void __fastcall kotor_fastcall_00693e70(void* self, void*, void* control);
void __fastcall kotor_fastcall_006929b0(void* self, void*, void* text);
void __fastcall kotor_fastcall_006932a0(void* self, void*, U32 id);
// .data storage (see header)
extern volatile U32 kotor_cell_map_fog_tiles_007a2678;
extern volatile U32 kotor_cell_map_reveal_007a267c;
volatile U32 kotor_cell_map_fog_tiles_007a2678 = 1u;                                             // [007A2678] .data 1
volatile U32 kotor_cell_map_reveal_007a267c = 1u;                                                // [007A267C] .data 1
}

namespace {
constexpr U32 kOneF = 0x3F800000u;                                                               // 1.0f
constexpr U32 kMinusOneF = 0xBF800000u;                                                          // -1.0f
void* gui_manager() { return kotor_menu_window_cells::gui_manager_007a39f4; }                    // [007A39F4]
kotor_menu_10_19::Tlk* tlk() { return static_cast<kotor_menu_10_19::Tlk*>(kotor_cell_tlk_007a3a08); } // [007A3A08]
// A stack CExoString destroyed at scope end (normally or while unwinding) once constructed.
struct Str {
    U32 s[2];
    bool live = false;
    Str() = default;
    Str(const Str&) = delete;
    Str& operator=(const Str&) = delete;
    ~Str() { if (live) frame_LogStringDtor_005e5c20(s, nullptr); }
    void born() { live = true; }
    void* p() { return s; }
};
// CTlkTable::GetSimpleString(&out, strref) into a fresh stack string; returns the result pointer (eax).
void* simple(Str& out, U32 strref) {
    void* r = tlk()->simple_0041e8f0(reinterpret_cast<kotor_menu_10_19::String*>(out.p()), strref);
    out.born();
    return r;
}
const char* cstr(void* string) { return frame_LogCStr_005e5670(string, nullptr); }               // CExoString::CStr
// `x & (1 << (x srem 32))` word/bit split used by the explored-grid bitmaps (cdq/and 1Fh/sar 5 + and 8000001Fh).
I32 word_index(I32 i) { return i / 32; }
U32 bit_of(I32 i) { return 1u << (static_cast<U32>(i % 32) & 31u); }
}

extern "C" {
// ======================= CSWGuiCharButton =======================
// 00684F50 CSWGuiCharButton::UpdateToolTipText() (thiscall, RET 0). Formats the party member summary held at
// [this+1C4] into the tooltip string at this+1C8.
void __fastcall kotor_fastcall_00684f50(void* self, void*) {
    void* info = lp(self, 0x1C4);                                                              // 00684F6B
    if (info == nullptr) return;                                                               // 00684F71..00684F73
    if (lw(info, 0x0C) != 0) {                                                                 // 00684F79..00684F86
        Str s1, s2, s3, s4;                                                                    // [esp+38] [esp+30] [esp+28] [esp+20] (EH 0..3)
        void* t1 = simple(s1, 0xBD4Bu);                                                        // 00684F81..00684F9C (ebp)
        void* t2 = simple(s2, 0x7D9Au);                                                        // 00684F9E..00684FBB (ebx)
        void* t3 = simple(s3, 0x424u);                                                         // 00684FBD..00684FD7 ([esp+10])
        void* t4 = simple(s4, 0x425u);                                                         // 00684FDB..00684FF5 ([esp+14])
        void* edi = lp(self, 0x1C4);                                                           // 00684FF9..00685008
        const U32 v14 = lw(edi, 0x14);                                                         // 00685001
        const char* c1 = cstr(t1);                                                             // 0068500A..00685012
        const U32 v10 = lw(lp(self, 0x1C4), 0x10);                                             // 00685017..0068501D
        const char* c2 = cstr(t2);                                                             // 00685022..00685024
        const U32 v08 = lw(edi, 0x08), v0c = lw(edi, 0x0C);                                    // 00685029..0068502D
        const char* c3 = cstr(t3);                                                             // 00685032..00685036
        const U32 v04 = lw(edi, 0x04), v00 = lw(edi, 0x00);                                    // 0068503B..00685043
        const char* c4 = cstr(t4);                                                             // 0068503E..00685047
        const char* n20 = cstr(at(lp(self, 0x1C4), 0x20));                                     // 0068504C..00685056
        const char* n18 = cstr(at(lp(self, 0x1C4), 0x18));                                     // 0068505B..00685065
        frame_LogFormat_005e5680(at(self, 0x1C8), "%s %s\n%s: %d/%d\n%s: %d/%d\n%s: %d\n%s: %d",  // 0068506A..00685077 (00753AE4)
                                 n18, n20, c4, v00, v04, c3, v08, v0c, c2, v10, c1, v14);
    } else {                                                                                   // 006850B2
        Str s1, s2, s3;                                                                        // [esp+50] [esp+48] [esp+40] (EH 4..6)
        void* t1 = simple(s1, 0xBD4Bu);                                                        // 006850B2..006850C2 (ebp)
        void* t2 = simple(s2, 0x7D9Au);                                                        // 006850C4..006850E1 (ebx)
        void* t3 = simple(s3, 0x425u);                                                         // 006850E3..006850FD ([esp+14])
        void* edi = lp(self, 0x1C4);                                                           // 00685101
        const U32 v14 = lw(edi, 0x14);                                                         // 00685109
        const char* c1 = cstr(t1);                                                             // 0068510D..00685114
        const U32 v10 = lw(lp(self, 0x1C4), 0x10);                                             // 0068511A..00685120
        const char* c2 = cstr(t2);                                                             // 00685124..00685126
        const U32 v04 = lw(edi, 0x04), v00 = lw(edi, 0x00);                                    // 0068512B..00685133
        const char* c3 = cstr(t3);                                                             // 0068512E..00685137
        const char* n20 = cstr(at(lp(self, 0x1C4), 0x20));                                     // 0068513C..00685146
        const char* n18 = cstr(at(lp(self, 0x1C4), 0x18));                                     // 0068514B..00685155
        frame_LogFormat_005e5680(at(self, 0x1C8), "%s %s\n%s: %d/%d\n%s: %d\n%s: %d",              // 0068515A..00685167 (00753AC4)
                                 n18, n20, c3, v00, v04, c2, v10, c1, v14);
    }                                                                                          // 0068516F..00685197 ~CExoString x3/x4
}

// 0040A340 CSWGuiManager::ChangeToolTipText(CExoString*) (thiscall, RET 4).
void __fastcall kotor_fastcall_0040a340(void* manager, void*, void* text) {
    void* tooltip = lp(manager, 0x3C);                                                         // 0040A340
    if (tooltip == nullptr) return;                                                            // 0040A343..0040A345 (ret 4)
    gui_m48_tooltip_panel_set_text_00624ae0(tooltip, nullptr, text);                           // 0040A347 jmp 00624AE0
}

// 00686750 CSWGuiCharButton::DisplayToolTip() (thiscall, RET 0) -> int.
U32 __fastcall kotor_fastcall_00686750(void* self, void*) {
    if ((lb(self, 0x44) & 2u) == 0) return 0;                                                  // 00686753..0068675C
    kotor_fastcall_00684f50(self, nullptr);                                                    // 0068675D..0068675F UpdateToolTipText
    gui_m48_manager_display_tooltip_0040b490(gui_manager(), nullptr, at(self, 0x1C8));         // 00686764..00686771
    sw(self, 0x44, lw(self, 0x44) | 0x40u);                                                    // 00686776
    return 1;                                                                                  // 0068677A
}

// 00686790 CSWGuiCharButton::Draw(float) (thiscall, RET 4).
void __fastcall kotor_fastcall_00686790(void* self, void*, U32 delta_bits) {
    if ((lb(self, 0x44) & 0x40u) != 0 && lp(self, 0x1C4) != nullptr                            // 00686793..006867A1
        && lw(lp(self, 0x1C4), 0xEA4) != 0) {                                                  // 006867A3..006867AB
        kotor_fastcall_00684f50(self, nullptr);                                                // 006867AD..006867AF UpdateToolTipText
        kotor_fastcall_0040a340(gui_manager(), nullptr, at(self, 0x1C8));                      // 006867B4..006867C1 ChangeToolTipText
        sw(lp(self, 0x1C4), 0xEA4, 0);                                                         // 006867C6..006867CC
    }
    kotor_fastcall_00417ab0(self, nullptr, delta_bits);                                        // 006867D6..006867DD CSWGuiButton::Draw
}

// ======================= CSWGuiSpeedButton =======================
// 0067E800 CSWGuiSpeedButton::HandleLMouseDown() (thiscall, RET 0).
void __fastcall kotor_fastcall_0067e800(void* self, void*) {
    sw(self, 0x1D0, lw(self, 0x1D0) | 1u);                                                     // 0067E803
    static_cast<kotor_input_hover::Control*>(self)->HandleLMouseDown();                        // 0067E80A CSWGuiControl::HandleLMouseDown
    vcall<void, U32, U32>(self, 0x3C, 0x16256u, 1u);                                           // 0067E80F..0067E81A
    sw(self, 0x1CC, lw(self, 0x1C8));                                                          // 0067E81D..0067E823
}
// 0067E830 CSWGuiSpeedButton::HandleRMouseDown() (thiscall, RET 0).
void __fastcall kotor_fastcall_0067e830(void* self, void*) {
    sw(self, 0x1D0, lw(self, 0x1D0) & ~1u);                                                    // 0067E833
    gui_load_control_rmouse_down_0041ac30(self, nullptr);                                      // 0067E83A CSWGuiControl::HandleRMouseDown
    vcall<void, U32, U32>(self, 0x3C, 0x16257u, 1u);                                           // 0067E83F..0067E84A
    sw(self, 0x1CC, lw(self, 0x1C8));                                                          // 0067E84D..0067E853
}
// 0067E860 CSWGuiSpeedButton::Draw(float) (thiscall, RET 4). Auto-repeat while the button holds the mouse capture.
void __fastcall kotor_fastcall_0067e860(void* self, void*, U32 delta_bits) {
    kotor_fastcall_00417ab0(self, nullptr, delta_bits);                                        // 0067E860..0067E868 CSWGuiButton::Draw
    if (lp(gui_manager(), 0x10) != self) return;                                               // 0067E86D..0067E876
    const double left = static_cast<double>(lf(self, 0x1CC)) - static_cast<double>(bits_f(delta_bits)); // 0067E878..0067E87E
    sf(self, 0x1CC, static_cast<float>(left));                                                 // 0067E882 fst dword
    if (jcc_jp(left, 0.0, 0x05)) return;                                                       // 0067E888..0067E893 (fcomp [0073D700]=0.0)
    do {                                                                                       // 0067E8A0
        const U8 flags = lb(self, 0x1D0);                                                      // 0067E8A6
        sf(self, 0x1CC, static_cast<float>(static_cast<double>(lf(self, 0x1C4)) + static_cast<double>(lf(self, 0x1CC)))); // 0067E8A0..0067E8B8
        if ((flags & 1u) != 0) vcall<void, U32, U32>(self, 0x3C, 0x16256u, 1u);                 // 0067E8BE..0067E8C7
        else vcall<void, U32, U32>(self, 0x3C, 0x16257u, 1u);                                  // 0067E8CC..0067E8D3
    } while (jcc_jnp(static_cast<double>(lf(self, 0x1CC)), 0.0, 0x05));                        // 0067E8D6..0067E8E7
}

// ======================= CSWGuiPanel::Draw slot =======================
// 006C00B0 (CSWGuiInGamePause table slot 34h): `jmp 0040B760` CSWGuiPanel::Draw.
void __fastcall kotor_fastcall_006c00b0(void* self, void*, U32 delta_bits) {
    kotor_panel_draw_0040B760(self, nullptr, delta_bits);                                      // 006C00B0
}

// ======================= CSWSAreaMap =======================
// 005791F0 CSWSAreaMap::GetPartyMemberMapOrientation(int) (thiscall, RET 4) -> float in ST0.
float __fastcall kotor_fastcall_005791f0(void* map, void*, I32 index) {
    if (lw(map, 0x04) == 0) return bits_f(0);                                                  // 005791F0..005791F5 (jbe) fld [0073D700]
    if (index < 3) return lf(map, 0x40 + static_cast<U32>(index) * 4u);                        // 005791F7..005791FE (jl), 00579209
    return bits_f(0);                                                                          // 00579200 fld [0073D700] (0.0)
}
// 00579210 CSWSAreaMap::IsWorldPointExplored(Vector) (thiscall, RET 0Ch) -> int.
U32 __fastcall kotor_fastcall_00579210(void* map, void*, W4Vector world) {
    const U32 size = lw(map, 0x04);                                                            // 00579217 edi
    if (size == 0) return 0;                                                                   // 0057921A..0057921C (jbe)
    I32 px, py;                                                                                // [esp+0C] / arg slot [esp+14]
    I32 gy = 0;                                                                                // [esp+08] (00579240)
    if (kotor_fastcall_00578e00(map, nullptr, world, &px, &py) == 0) return 0;                 // 0057921E..00579252
    I32 gx;                                                                                    // arg slot [esp+14] reused
    if (kotor_fastcall_00578f10(map, nullptr, px, py, &gx, &gy) == 0) return 0;                // 00579254..00579271
    const I32 index = (li(map, 0x08) + 1) * gy + gx;                                           // 00579273..00579280
    const I32 word = word_index(index);                                                        // 00579282..0057928A
    if (word > static_cast<I32>(size)) return 0;                                               // 0057928D..0057928F (jle ok)
    return (lw(lp(map, 0x00), static_cast<U32>(word) * 4u) & bit_of(index)) != 0 ? 1u : 0u;     // 0057929B..005792BB
}

// ======================= CSWGuiMapHider / CSWGuiInGameMap =======================
// 006932A0 CSWGuiMapHider::SetMapNote(unsigned long id) (thiscall, RET 4): select the note list position holding id.
void __fastcall kotor_fastcall_006932a0(void* self, void*, U32 id) {
    void* pos = lp(lp(self, 0x238), 0);                                                        // 006932A4..006932AE ([esp+4])
    if (pos == nullptr) return;                                                                // 006932B2
    do {
        if (lw(pt(unresolved_list_at_pos_005e9b60(lp(self, 0x238), nullptr, pos)), 0) == id) { // 006932C0..006932CE
            sw(self, 0x23C, ad(pos));                                                          // 006932EE..006932F2
            return;
        }
        unresolved_list_next_005e9b90(lp(self, 0x238), nullptr, &pos);                        // 006932D0..006932DB
    } while (pos != nullptr);                                                                  // 006932E0..006932E6
}

// 006929B0 CSWGuiInGameMap::SetMapNote(CExoString* text) (thiscall, RET 4).
void __fastcall kotor_fastcall_006929b0(void* self, void*, void* text) {
    Str note;                                                                                  // [esp+08] (EH 0)
    exo_default(note.p()); note.born();                                                        // 006929D2..006929D6
    if (async_string_length_005e5790(text, nullptr) == 0) {                                    // 006929DE..006929EF
        ExoString empty("");                                                                   // 006929F1..006929FA (0073D71C) (EH 1)
        gui_load_text_params_set_text_00415e00(at(self, 0x3CC), nullptr, empty.storage);       // 006929FF..00692A0F
        return;                                                                                // 00692A14..00692A22, 00692ACE.. ~note
    }
    {
        Str sep, label, joined, full;                                                          // [esp+18] [esp+30] [esp+28] [esp+20] (EH 2..5)
        frame_LogStringCtor_005e5a90(sep.p(), nullptr, ": "); sep.born();                     // 00692A27..00692A30 (00748ED0)
        void* name = simple(label, 0x15Du);                                                    // 00692A35..00692A4A
        void* a = GuiConcat005e5d10(static_cast<SS*>(name), nullptr, static_cast<SS*>(joined.p()), static_cast<SS*>(sep.p())); // 00692A4F..00692A60
        joined.born();
        void* b = GuiConcat005e5d10(static_cast<SS*>(a), nullptr, static_cast<SS*>(full.p()), static_cast<SS*>(text)); // 00692A65..00692A72
        full.born();
        async_assign_string_005e5c50(note.p(), nullptr, b);                                    // 00692A77..00692A81
    }                                                                                          // 00692A86..00692AB9 ~full ~joined ~label ~sep
    gui_load_text_params_set_text_00415e00(at(self, 0x3CC), nullptr, note.p());                // 00692ABE..00692AC9
}                                                                                              // 00692ACE..00692ADA ~note

// 00693E70 CSWGuiInGameMap::OnMapNoteClicked(CSWGuiControl*) (thiscall, RET 4): event 0x27 of the note buttons
// created by CSWGuiMapHider::Draw (target = the hider's parent panel [hider+34]).
void __fastcall kotor_fastcall_00693e70(void* self, void*, void* control) {
    if (lw(control, 0x4C) == 0) return;                                                        // 00693E89..00693E95
    Str text;                                                                                  // [esp+0C] (EH 0)
    exo_default(text.p()); text.born();                                                        // 00693E9C..00693EA0
    const U32 id = lw(control, 0x58);                                                          // 00693EA5 ebx
    void* object = kotor_fastcall_004ae750(server_app(), nullptr, id);                         // 00693EA8..00693EB9 GetGameObject
    if (object != nullptr && vcall<void*>(object, 0x58) != nullptr) {                          // 00693EBE..00693ECD
        void* client = client_app();                                                           // 00693ECF..00693ED7 ebp
        void* waypoint = vcall<void*>(object, 0x58);                                           // 00693ED4..00693EDF
        const U32 language = kotor_fastcall_005ed4c0(client, nullptr);                         // 00693EE1..00693EEA GetClientLanguage
        exo_locstring_get_005ea130(at(waypoint, 0x230), nullptr, language, text.p(), 0);       // 00693EEF..00693EF6
    }
    kotor_fastcall_006929b0(self, nullptr, text.p());                                          // 00693EFC..00693F03 SetMapNote
    kotor_fastcall_006932a0(at(self, 0xE38), nullptr, id);                                     // 00693F08..00693F0F CSWGuiMapHider::SetMapNote
    kotor_fastcall_00692810(self, nullptr, 0x3A83126Fu);                                       // 00693F14..00693F1B Draw(0.001f)
    void* manager = lp(self, 0x18);                                                            // 00693F20
    kotor_fastcall_0040c1e0(manager, nullptr, lw(manager, 0), lw(manager, 4));                 // 00693F23..00693F2A HandleMouseMove
}                                                                                              // 00693F2F..00693F3B ~text

// 00693300 CSWGuiMapHider::HitCheckMouse(int x, int y) (thiscall, RET 8) -> CSWGuiControl*.
void* __fastcall kotor_fastcall_00693300(void* self, void*, I32 x, I32 y) {
    const I32 left = li(self, 0x04);                                                           // 0069330A
    if (x < left) return nullptr;                                                              // 0069330D..0069330F
    if (x > li(self, 0x0C) + left) return nullptr;                                             // 00693311..0069331A
    const I32 top = li(self, 0x08);                                                            // 0069331C
    if (y < top) return nullptr;                                                               // 00693323..00693325
    if (y > li(self, 0x10) + top) return nullptr;                                              // 00693327..00693330
    const I32 rel_y = y - top, rel_x = x - left;                                               // 00693332..0069333B
    void* marker = at(self, 0x68);                                                             // 0069333D
    if (vcall<U32, I32, I32>(marker, 0x44, rel_x, rel_y) != 0) return marker;                  // 00693340..0069334B
    for (I32 i = 0; i < li(self, 0x230); ++i)                                                  // 00693354..0069337D
        if (vcall<U32, I32, I32>(lp(lp(self, 0x22C), static_cast<U32>(i) * 4u), 0x44, rel_x, rel_y) != 0) // 00693360..00693372
            return lp(lp(self, 0x22C), static_cast<U32>(i) * 4u);                              // 00693388..0069338E
    return nullptr;                                                                            // 0069337F..00693385
}

// 006943D0 CSWGuiMapHider::Draw(float) (thiscall, RET 4). Fog of war, map-note buttons, party markers, player arrow.
void __fastcall kotor_fastcall_006943d0(void* self, void*, U32 delta_bits) {
    if (lw(self, 0x244) == 0) return;                                                          // 006943EF..006943FB
    U32 neg_one[3] = {kMinusOneF, kMinusOneF, kMinusOneF};                                     // [esp+54] (0069441C..0069442C)
    if (kotor_AurGUISetupViewport_004592F0(lw(self, 0x04), lw(self, 0x08), lw(self, 0x0C), lw(self, 0x10), // 00694401..00694439
                                           neg_one, 0, kOneF) == 0) return;                    // 0069443C..0069443E
    void* map = lp(server_get_module_004ae6b0(server_app(), nullptr), 0x218);                  // 00694444..0069445A ([esp+4C])
    if (map != nullptr && lw(map, 0x04) != 0) {                                                // 0069445E..00694467 (jbe)
        U32 size = 0;                                                                          // [esp+64]
        I32 res_x = 0, res_y = 0;                                                              // [esp+44] / [esp+50]
        const U32 data = kotor_fastcall_00579090(map, nullptr, &size, &res_x, &res_y);         // 0069446D..0069447E ([esp+1C])
        if (data != 0 && kotor_cell_map_reveal_007a267c != 0) {                                // 00694483..00694495
            const I32 cells = (res_x + 1) * (res_y + 1);                                       // 0069449F..006944BD edi
            float x = bits_f(0), y = bits_f(0);                                                // [esp+38] / [esp+10]
            const float step_x = static_cast<float>(440.0 / static_cast<double>(res_x));      // 0069449B..006944B9 [esp+3C] (00747748)
            const float step_y = static_cast<float>(256.0 / static_cast<double>(res_y));      // 006944C0..006944CC [esp+20] (007455D4)
            const float tile_w = static_cast<float>(static_cast<double>(step_x) * 4.0);        // 006944D0..006944DA [esp+40] (0073F404)
            const float tile_h = static_cast<float>(static_cast<double>(step_y) * 4.0);        // 006944DE..006944E8 [esp+34]
            for (I32 i = 0; i < cells;) {                                                      // 006944EC / 006945DB..006945DD
                const I32 word = word_index(i);                                                // 006944F2..0069450D
                if (word > static_cast<I32>(size)) break;                                      // 00694510..00694512 (jg)
                const U32 explored = lw(pt(data), static_cast<U32>(word) * 4u) & bit_of(i);    // 00694518..00694528 ([esp+60])
                if (explored == 0 && kotor_cell_map_fog_tiles_007a2678 != 0) {                 // 0069452C..00694534
                    const double h = static_cast<double>(li(self, 0x10));                      // 00694536 fild
                    const double w = static_cast<double>(li(self, 0x0C));                      // 0069453E fild
                    neg_one[0] = kMinusOneF; neg_one[1] = kMinusOneF; neg_one[2] = kMinusOneF;  // 00694554..00694564
                    const float a3 = static_cast<float>(static_cast<double>(tile_h) / h);      // 00694545..0069456E
                    const float a2 = static_cast<float>(static_cast<double>(tile_w) / w);      // 00694572..00694578
                    const float a1 = static_cast<float>((static_cast<double>(y) - static_cast<double>(tile_h) * 0.5) / h); // 0069457C..0069458C (0073E9AC)
                    const float a0 = static_cast<float>((static_cast<double>(x) - static_cast<double>(tile_w) * 0.5) / w); // 00694590..006945A0
                    vcall<void, float, float, float, float, U32, U32, const void*, U32>(       // 006945A7 [[ebx+5C]]+20h
                        lp(self, 0x5C), 0x20, a0, a1, a2, a3, 0, 0, neg_one, kOneF);
                }
                ++i;                                                                           // 006945AE
                if (i % (res_x + 1) != 0) {                                                    // 006945AA..006945B7
                    x = static_cast<float>(static_cast<double>(x) + static_cast<double>(step_x)); // 006945B9..006945C1
                } else {
                    x = bits_f(0);                                                             // 006945CB
                    y = static_cast<float>(static_cast<double>(y) + static_cast<double>(step_y)); // 006945C7..006945D7
                }
            }
        }
        kotor_cdecl_0045a170(kotor_gui_image_alpha_0078e43c);                                  // 006945E3..006945EA FlushBuffer
        I32 rect[4] = {0, 0, 0, 0};                                                            // [esp+24..30] (006945F5..00694601)
        I32 party_x = 0, party_y = 0;                                                          // [esp+14] / [esp+18] (00694605..00694609)
        U32 drawn = 0;                                                                         // edi (00694612)
        void* pos = lp(lp(self, 0x238), 0);                                                    // 006945EF..00694616 ([esp+48])
        while (pos != nullptr) {                                                               // 0069461A / 006949D2..006949D8
            U32 id = lw(pt(unresolved_list_at_pos_005e9b60(lp(self, 0x238), nullptr, pos)), 0); // 00694620..0069462C ebp, [esp+34]
            void* object = kotor_fastcall_004ae750(server_app(), nullptr, id);                 // 0069462E..0069463C
            void* note = nullptr;
            if (object != nullptr && vcall<void*>(object, 0x58) != nullptr                      // 00694641..00694654
                && lw(note = vcall<void*>(object, 0x58), 0x22C) != 0) {                         // 0069465A..0069466B
                void* explored_map = lp(server_get_module_004ae6b0(server_app(), nullptr), 0x218); // 00694671..00694685 ([esp+1C])
                W4Vector where; std::memcpy(&where, at(note, 0x90), 12);                       // 00694689..006946A3
                if (kotor_fastcall_00579210(explored_map, nullptr, where) != 0) {              // 006946A6..006946B1
                    I32 note_y = 0, note_x;                                                    // [esp+10] (006946BD) / [esp+20]
                    void* pixel_map = lp(server_get_module_004ae6b0(server_app(), nullptr), 0x218); // 006946B7..006946CD
                    std::memcpy(&where, at(note, 0x90), 12);                                   // 006946DD..006946EF
                    if (kotor_fastcall_00578e00(pixel_map, nullptr, where, &note_x, &note_y) != 0) { // 006946D3..006946FB
                        if (ad(pos) == lw(self, 0x23C)) {                                      // 00694701..0069470B
                            rect[0] = note_x - 10; rect[1] = note_y - 10;                      // 0069470D..00694736
                            rect[2] = 0x14; rect[3] = 0x14;                                    // 0069471F..0069472B
                            void* marker = at(self, 0x68);                                     // 0069472F
                            sw(self, 0xC0, id);                                                // 0069473D (marker+58h)
                            vcall<void, const void*>(marker, 0x04, rect);                      // 00694732..00694743 SetExtent
                            vcall<void, U32>(marker, 0x38, delta_bits);                        // 00694746..00694752 Draw
                        } else {
                            rect[2] = 0x0E; rect[3] = 0x0E;                                    // 00694762..0069476B
                            rect[0] = note_x - 7; rect[1] = note_y - 7;                        // 0069475A..00694781
                            if (lw(self, 0x230) == drawn) {                                    // 0069476F..00694785
                                void* block = live_operator_new(0x1C4);                        // 0069478B..00694795 ([esp+60])
                                void* created = nullptr;
                                if (block != nullptr) {                                        // 0069479E..006947A7
                                    auto guard = on_throw([&]() { live_free(block); });        // EH state 0
                                    created = gui_load_button_construct_0041c0f0(block, nullptr); // 006947A9..006947AB
                                    guard.armed = false;
                                }                                                              // 006947BD state -1
                                gui_load_array_add_00671c00(at(self, 0x22C), nullptr, ad(created)); // 006947B4..006947C8 Add
                                menu_resources::Ref target_a; target_a.construct_00406d80("whitetarget"); // 006947CD..006947D9 [esp+90] (00754710)
                                menu_resources::Ref target_b; target_b.construct_00406d80("whitetarget"); // 006947DE..006947EA [esp+A0]
                                unsigned char text[0x58];                                      // [esp+F0] CSWGuiTextParams
                                void* text_params = gui_load_text_params_construct_00417050(text, nullptr); // 006947EF..006947FB ([esp+1C])
                                {
                                    auto text_guard = on_throw([&]() { frame_LogStringDtor_005e5c20(text, nullptr); }); // EH state 1
                                    void* button = lp(lp(self, 0x22C), drawn * 4u);            // 006947FF..00694801 ([esp+3C])
                                    const U32 color_a[3] = {kOneF, kOneF, kOneF};              // [esp+74] (00694827..0069483D)
                                    const U32 color_b[3] = {kOneF, kOneF, kOneF};              // [esp+68] (00694848..00694858)
                                    const I32 extent[4] = {0, 0, 0x0E, 0x0E};                  // [esp+80] (00694804..0069486A)
                                    menu_resources::Ref none_a, none_b, none_c, none_d;        // [esp+B0] [esp+E0] [esp+D0] [esp+C0]
                                    unsigned char border_a[0x74], border_b[0x74];              // [esp+144] / [esp+198] CSWGuiBorderParams
                                    c2da_default_ref_00405ed0(&none_a, nullptr);               // 00694875..00694893
                                    c2da_default_ref_00405ed0(&none_b, nullptr);               // 00694899..006948A0
                                    void* first = gui_load_border_params_construct_00415000(border_a, nullptr, &none_b, &none_a, // 006948A6..006948AD
                                                                                            &target_a, 2, 0, 1.0f, color_a, 0);
                                    c2da_default_ref_00405ed0(&none_c, nullptr);               // 006948CB..006948D2
                                    c2da_default_ref_00405ed0(&none_d, nullptr);               // 006948D8..006948DF
                                    void* second = gui_load_border_params_construct_00415000(border_b, nullptr, &none_d, &none_c, // 006948E5..006948EC
                                                                                             &target_b, 2, 0, 1.0f, color_b, 0);
                                    vcall<void, const void*, void*, void*, void*>(button, 0x9C, extent, text_params, second, first); // 006948F1..00694907 Initialize
                                    text_guard.armed = false;
                                }
                                frame_LogStringDtor_005e5c20(text, nullptr);                   // 0069490D..0069491F
                                std::memcpy(at(lp(lp(self, 0x22C), drawn * 4u), 0x90), borrowed_exit_color_007a23b4, 12);   // 00694924..00694944
                                std::memcpy(at(lp(lp(self, 0x22C), drawn * 4u), 0x104), borrowed_enter_color_007a23c0, 12); // 00694947..00694967
                                void* fresh = lp(lp(self, 0x22C), drawn * 4u);                 // 0069496A..0069496C
                                sw(fresh, 0x44, lw(fresh, 0x44) & ~4u);                        // 0069496F
                                gui_load_control_add_event_0041ab20(lp(lp(self, 0x22C), drawn * 4u), nullptr, 0x27, lp(self, 0x34), // 00694973..00694983
                                                                    reinterpret_cast<const void*>(&kotor_fastcall_00693e70));
                                // 00694988 ebp = [esp+34]: reload of the same note id
                            }
                            void* button = lp(lp(self, 0x22C), drawn * 4u);                    // 0069498C..00694992
                            sw(button, 0x58, id);                                              // 00694995
                            vcall<void, const void*>(lp(lp(self, 0x22C), drawn * 4u), 0x04, rect); // 00694998..006949A8 SetExtent
                            vcall<void, U32>(lp(lp(self, 0x22C), drawn * 4u), 0x38, delta_bits);  // 006949AB..006949BE Draw
                            ++drawn;                                                           // 006949C1
                        }
                    }
                }
            }
            unresolved_list_next_005e9b90(lp(self, 0x238), nullptr, &pos);                    // 006949C2..006949CD
        }
        if (static_cast<I32>(drawn) < li(self, 0x230)) sw(self, 0x230, drawn);                // 006949E0..006949E8 (jge)
        const U8* party = static_cast<const U8*>(kotor_fastcall_005ed8b0(client_app(), nullptr)); // 006949EE..006949F7 GetSWParty
        const I32 members = party != nullptr ? static_cast<I32>(party[0]) : 3;                 // 006949FC..00694A06
        for (I32 member = 1; member < members; ++member) {                                     // 00694A09..00694A10 / 00694A81..00694A84
            party_x = 0; party_y = 0;                                                          // 00694A20..00694A26
            if (kotor_fastcall_005791b0(map, nullptr, member, &party_x, &party_y) != 0         // 00694A2A..00694A40
                && lp(self, 0x64) != nullptr) {                                                // 00694A42..00694A47
                rect[1] = party_y - 8; rect[0] = party_x - 8;                                  // 00694A49..00694A5F
                rect[2] = 0x10; rect[3] = 0x10;                                                // 00694A63..00694A67
                vcall<void, const void*>(lp(self, 0x64), 0x04, rect);                          // 00694A6B..00694A6E SetExtent
                vcall<void, U32>(lp(self, 0x64), 0x0C, delta_bits);                            // 00694A71..00694A7E Draw
            }
        }
        party_x = 0; party_y = 0;                                                              // 00694A8D..00694A91
        if (lp(self, 0x60) != nullptr                                                          // 00694A86..00694A95
            && kotor_fastcall_005791b0(map, nullptr, 0, &party_x, &party_y) != 0) {            // 00694A9B..00694AB3
            rect[0] = party_x - 0x10; rect[1] = party_y - 0x10; rect[2] = 0x20; rect[3] = 0x20; // 00694AB5..00694AE3 (dead stores)
            if (kotor_AurGUISetupViewport_004592F0(static_cast<U32>(rect[0]), static_cast<U32>(rect[1]), 0x20, 0x20, // 00694ABD..00694AE7
                                                   scene3d_gui_scene_color_0078d3d8, 0, kOneF) != 0) { // 00694AEC..00694AF1
                void* arrow = lp(self, 0x60);                                                  // 00694AF3
                sf(arrow, 0x38, kotor_fastcall_005791f0(map, nullptr, 0));                     // 00694AF6..00694B08 (+28h+10h)
                vcall<void, U32>(lp(self, 0x60), 0x0C, delta_bits);                            // 00694B0B..00694B13 Draw
                kotor_AurGUICloseViewport_00459580();                                          // 00694B16
            }
        }
    }
    kotor_AurGUICloseViewport_00459580();                                                      // 00694B1B
}
}
