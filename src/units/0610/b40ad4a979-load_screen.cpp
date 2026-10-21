// lane-prepare: CSWGuiLoadScreen::CSWGuiLoadScreen 0067A710, transcribed from
// swkotor.exe (sha256 9c10e045...1435) 0067A710..0067A9BD. EH states follow the
// unwind map at FuncInfo 0077E7DC (states 0..13); each C++ scope below mirrors one
// state range and runs the same cleanup action the original funclet runs.
#include "c5794db80a-load_screen.h"
#include "d6db6bea87-prepare_image_data.h"   // D13 generated .rdata strings
#include "61f04a8cb1-start_body.h"         // client_services_start::globals (Width/Height cells)
#include <cstring>
#include <new>

namespace kotor_original {
namespace {
using String = kotor_reconstruction::StartupString;
using Word = std::uint32_t;
inline unsigned char* at(void* self, unsigned off) noexcept { return static_cast<unsigned char*>(self) + off; }
inline Word load32(const volatile void* cell) noexcept { return *static_cast<const volatile Word*>(cell); }
inline std::int32_t load16s(const void* p, unsigned off) noexcept {
    std::int16_t v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 2); return v;   // movsx
}
inline void put32(void* self, unsigned off, Word v) noexcept { std::memcpy(at(self, off), &v, 4); }
inline Word get32(void* self, unsigned off) noexcept { Word v; std::memcpy(&v, at(self, off), 4); return v; }

// InitControl with a temporary CExoString name: states 6..9 (funclets 0072E14B..
// 0072E163 destroy [ebp-28h]); state returns to 5 after 005E5C20.
void init_named(void* self, unsigned control, const char* name) {
    String tmp(name);                                               // 005E5A90
    try {
        gui_load_panel_init_control_0040b930(static_cast<gui_load::Panel*>(self), nullptr,
            reinterpret_cast<gui_load::Control*>(at(self, control)), &tmp, 1); // push 1 / &tmp / control
    } catch (...) { tmp.~String(); throw; }
    tmp.~String();                                                  // 005E5C20
}

// State 5 body (all members constructed): 0067A79A..0067A9A5.
void body(void* self, void* gui_manager) {
    gui_load::ResRef layout;
    layout.construct_00406d80(kotor_rdata_str_00752db0);                        // 0067A79A/0067A7A8 (00752DB0)
    gui_load_panel_start_layout_0040a680(static_cast<gui_load::Panel*>(self), nullptr, &layout); // 0067A7B4
    init_named(self, 0x064, "PB_PROGRESS");                         // 0067A7B9..0067A7E4 (00752DA4)
    init_named(self, 0x1B4, "LBL_HINT");                            // 0067A7E9..0067A81A (00752D98)
    init_named(self, 0x2F4, "LBL_LOGO");                            // 0067A81F..0067A850 (00752D8C)
    init_named(self, 0x434, "LBL_LOADING");                         // 0067A855..0067A886 (00752D80)
    gui_load_panel_stop_layout_0040b8f0(static_cast<gui_load::Panel*>(self), nullptr); // 0067A88D
    kotor_fastcall_00417fb0(at(self, 0x64), nullptr, 0);            // 0067A892/0067A896
    {   // state 10: SetText(LBL_HINT text params +29C, "") (0073D71C)
        String empty("");                                           // 0067A89B/0067A8A4
        try { gui_load_text_params_set_text_00415e00(at(self, 0x29C), nullptr, &empty); } // 0067A8B9
        catch (...) { empty.~String(); throw; }                     // funclet 0072E16B
        empty.~String();                                            // 0067A8C7
    }
    {   // 0067A8CC..0067A8F2: background label +578 extent {0,0,mgr[+6C],mgr[+6E]} via vtable slot +04
        gui_load::Extent extent;
        extent.width = load16s(gui_manager, 0x6C);                  // 0067A8D3 movsx [eax+6Ch] -> [esp+28h]
        extent.height = load16s(gui_manager, 0x6E);                 // 0067A8CF movsx [eax+6Eh] -> [esp+2Ch]
        extent.left = 0; extent.top = 0;                            // 0067A8EA/0067A8EE (edi=0)
        auto* background = reinterpret_cast<gui_load::Object*>(at(self, 0x578));
        background->vtbl->set_extent(background, nullptr, &extent);         // 0067A8D7 mov edx,[ebx]; 0067A8F2 call [edx+4]
    }
    // 0067A8F5/0067A8FB: [0078D1D8] height, [0078D1D4] width, through the
    // canonical published StartServices cells (Height / Width words).
    const auto* g = client_services_start::globals;
    const Word height = load32(g->words[static_cast<unsigned>(client_services_start::Global::Height)]);
    const Word width = load32(g->words[static_cast<unsigned>(client_services_start::Global::Width)]);
    alignas(String) unsigned char res_storage[sizeof(String)];
    String* res = gui_load_manager_resolution_string_0040a3e0(reinterpret_cast<String*>(res_storage), static_cast<gui_load::I32>(width), static_cast<gui_load::I32>(height)); // 0067A908 (lane-gui-load)
    try {                                                           // state 11: funclet 0072E173 [ebp-30h]
        String load(kotor_rdata_str_00752d78);                                        // 0067A910/0067A91E (00752D78)
        try {                                                       // state 12: funclet 0072E17B [ebp-28h]
            alignas(String) unsigned char cat_storage[sizeof(String)];
            String* cat = GuiConcat005e5d10(res, nullptr, reinterpret_cast<String*>(cat_storage), &load); // 0067A937
            try { res->Assign005e5c50(cat); }                       // state 13: 0067A946; funclet 0072E183
            catch (...) { cat->~String(); throw; }
            cat->~String();                                         // 0067A953
        } catch (...) { load.~String(); throw; }
        load.~String();                                             // 0067A961
        gui_load::ResRef fill;
        async_ref_construct_string_00406d60(&fill, nullptr, res);   // 0067A96F
        gui_load_border_params_set_fill_00414c00(at(self, 0x5E8), nullptr, &fill, 0); // 0067A974..0067A982
        put32(self, 0x5E8 + 0x1C, (get32(self, 0x5E8 + 0x1C) & 0xFFFFFFFEu) | 2u); // 0067A987..0067A990
        put32(self, 0x44, get32(self, 0x44) | 0x60u);               // 0067A993..0067A99D
    } catch (...) { res->~String(); throw; }
    res->~String();                                                 // 0067A9A5 (state 5)
}
}

void* __fastcall kotor_fastcall_0067a710(void* self, void*, void* gui_manager) {
    gui_load_panel_construct_0040b570(reinterpret_cast<gui_load::Panel*>(self), nullptr, static_cast<gui_load::Object*>(gui_manager));            // 0067A736/0067A73B
    try {                                                           // state 0: ~CSWGuiPanel 0040CF70
        // 0067A74D mov [esi],00752DC0 -> modern vtable object.
        put32(self, 0, static_cast<Word>(reinterpret_cast<std::uintptr_t>(kotor_vtable_00752dc0)));
        kotor_fastcall_0041aed0(at(self, 0x064), nullptr);          // 0067A753
        try {                                                       // state 1: ~CSWGuiProgressBar 0041AF70 (+64)
            kotor_fastcall_0041acd0(at(self, 0x1B4), nullptr);      // 0067A763
            try {                                                   // state 2: ~CSWGuiLabel 00419D10 (+1B4)
                kotor_fastcall_0041acd0(at(self, 0x2F4), nullptr);  // 0067A773
                try {                                               // state 3: (+2F4)
                    kotor_fastcall_0041acd0(at(self, 0x434), nullptr); // 0067A783
                    try {                                           // state 4: (+434)
                        kotor_fastcall_0041acd0(at(self, 0x578), nullptr); // 0067A795
                        try { body(self, gui_manager); }            // state 5: (+578)
                        catch (...) { gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(at(self, 0x578)), nullptr); throw; }
                    } catch (...) { gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(at(self, 0x434)), nullptr); throw; }
                } catch (...) { gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(at(self, 0x2F4)), nullptr); throw; }
            } catch (...) { gui_load_label_destruct_00419d10(reinterpret_cast<gui_load::Label*>(at(self, 0x1B4)), nullptr); throw; }
        } catch (...) { kotor_fastcall_0041af70(at(self, 0x064), nullptr); throw; }
    } catch (...) { gui_load_panel_destruct_0040cf70(reinterpret_cast<gui_load::Panel*>(self), nullptr); throw; }
    return self;                                                    // 0067A9AF EAX=this, RET 4
}
}
