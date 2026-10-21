// claude_w1_chains_floaty.cpp (lane worker w1-chains): the client floaty-text chain reached from
// CSWSCreature::SignalMeleeDamage 005B75D0 (claude_w1_chains_damage.cpp), transcribed from the original swkotor.exe
// (read-only):
//   005EDEA0 CClientExoApp::AddFloatyText(ulong id, uchar type, int value)          RET 0Ch kotor_fastcall_005edea0
//   006027C0 CClientExoAppInternal::AddFloatyText(ulong id, uchar type, int value)  RET 0Ch kotor_fastcall_006027c0
//   0062B080 CGuiInGame::AddFloatyText(ulong, CExoString const&, Vector, float)     RET 10h kotor_fastcall_0062b080
//   0068B7C0 CSWGuiMainInterface::AddFloatyText(ulong, CExoString const&, Vector, float) RET 10h kotor_fastcall_0068b7c0
//   00688E10 CSWGuiFloatyText::CreateEntry(ulong, CExoString const&, Vector, float) RET 10h kotor_fastcall_00688e10
//   00688FF0 CSWGuiFloatyText::Update(float dt)                                     RET 4   kotor_fastcall_00688ff0
//   00688060 CSWGuiMainInterface::RemoveFloatyText(CSWGuiFloatyText*)               RET 4   kotor_fastcall_00688060
//   CSWGuiFloatyText vftable 00753EA8 (41 slots = CSWGuiLabel 0073E5B8 + slot 0/38h/A0h overrides)
// and the two slots of that table that were W4 slot traps (the coordinator removes their trap lines in
// srcw4/src/gui/gui_slot_traps_successor.cpp and gui_slot_traps.cpp):
//   0068ACF0 CSWGuiLabel::`scalar deleting destructor'(uint flags)                  RET 4   kotor_fastcall_0068acf0
//   006AEAC0 CSWGuiBlackenedLabel::Draw(int)                                        RET 4   kotor_fastcall_006aeac0
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double
// intermediates, float exactly where the original stores a dword; live_operator_new / live_free only.
// The "Vector" parameters are passed by address (the original pushes the address of a local Vector).
#include "842d6b1618-w5_t3.hpp"

namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                                  // __ftol2 low dword [w2_ftol_native]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString() [menu20_bindings]
#ifndef W1R_MENU_RESOURCES_REF
#define W1R_MENU_RESOURCES_REF
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
#endif
namespace gui_load { struct Vector; }
namespace kotor_gui_m48_cells { extern gui_load::Vector viewport_color_00833910; }            // .bss 00833910 (black) [0699]
namespace w4 {
extern void* main_interface_00833bb4;                                                         // .bss 00833BB4 CSWGuiMainInterface* [main_interface_ctor]
extern const float map_note_color_007a23f0[3];                                                // .data 007A23F0 [panels_menus]
}

namespace w1c_floaty {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c14 candidate map; owner object in brackets) ----------------
extern const unsigned char kotor_m15d_color_007a23d8[12];                                      // .data 007A23D8 (.74, .11, 0) [0827]
extern const unsigned char kotor_m15d_color_007a23e4[12];                                      // .data 007A23E4 (.28, .92, .11) [0827]
extern const unsigned char kotor_m1_color_007a23fc[12];                                        // .data 007A23FC [0807]
extern const unsigned char gui_load_label_methods_0073e5b8[];                                  // CSWGuiLabel vftable (40 slots) [0296]
void* __fastcall kotor_fastcall_005e5bc0(void* self, void*, I value);                          // CExoString(int) [0266]
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                 // CExoString::operator= [0199]
const char* __fastcall WindowCStr005e5670(void*, void*);                                       // CExoString::CStr [0078]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                   // CExoString::Format [0125]
void* __fastcall kotor_fastcall_005fb540(void* internal, void*, void* out, U strref);          // CClientExoAppInternal::GetTLKString [tlk_token]
void* __fastcall gui_load_label_construct_0041acd0(void* self, void*);                         // CSWGuiLabel::CSWGuiLabel [0296]
void __fastcall kotor_fastcall_0068ad10(void* self, void*);                                    // CSWGuiLabel::~CSWGuiLabel [main_interface_ctor]
void __fastcall kotor_fastcall_00417750(void* self, void*, U arg);                             // CSWGuiLabel::Draw [0437]
void __fastcall gui_load_text_params_set_alignment_00415f20(void* params, void*, U alignment); // CSWGuiTextParams::SetAlignment [0295]
void __fastcall gui_load_text_params_set_text_00415e00(void* params, void*, void* text);       // CSWGuiTextParams::SetText [0295]
void __fastcall gui_load_text_params_set_color_00414e10(void* params, void*, const void* color); // CSWGuiTextParams::SetColor [0587]
void __fastcall gui_load_text_params_set_base_font_00415dd0(void* params, void*, void* resref); // CSWGuiTextParams::SetBaseFont [0295]
I __fastcall gui_load_text_font_height_00414ee0(void* text, void*);                           // CSWGuiText::GetFontHeight [0300]
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                           // CClientExoApp::GetGameObject [0701]
void* __fastcall kotor_fastcall_005ed560(void* client, void*);                                 // CClientExoApp::GetModuleCamera [client_update]
void* __fastcall kotor_fastcall_005eda40(void* client, void*);                                 // CClientExoApp::GetSWGuiManager [0417]
void __fastcall deleteAt_006857a0(void* list, void*, int index);                               // CExoArrayList::deleteAt [0609]

// ---------------- exports of this file ----------------
void __fastcall kotor_fastcall_005edea0(void* client, void*, U id, U type, I value);
void __fastcall kotor_fastcall_006027c0(void* internal, void*, U id, U type, I value);
void __fastcall kotor_fastcall_0062b080(void* gui, void*, U id, void* text, const void* color, U scale_bits);
void __fastcall kotor_fastcall_0068b7c0(void* main_interface, void*, U id, void* text, const void* color, U scale_bits);
void __fastcall kotor_fastcall_00688e10(void* self, void*, U id, void* text, const void* color, U scale_bits);
void __fastcall kotor_fastcall_00688ff0(void* self, void*, float dt);
void __fastcall kotor_fastcall_00688060(void* main_interface, void*, void* floaty);
void* __fastcall kotor_fastcall_0068acf0(void* self, void*, U flags);
void __fastcall kotor_fastcall_006aeac0(void* self, void*, U arg);
}

namespace {
// .rdata / .data constants (proven values read from the original)
constexpr U k_scale_normal_3fc00000 = 0x3fc00000u;      // 1.5f (immediate, 00602A85)
constexpr U k_scale_large_40400000 = 0x40400000u;       // 3.0f (immediate, 00602977 / 006029E2)
const unsigned char k_white_007a23a8[12] = {0, 0, 0x80, 0x3f, 0, 0, 0x80, 0x3f, 0, 0, 0x80, 0x3f}; // .data 007A23A8 (1,1,1), read only
const char k_format_0073ee2c[] = "%s %d";               // .rdata 0073EE2C
const char k_font_00753da0[] = "fnt_d16x16";            // .rdata 00753DA0
const double k_rise_00753d80 = 32.0;                    // .rdata 00753D80 (qword)
const float k_half_width_00741838 = 100.0f;             // .rdata 00741838
const float k_zero_0073d700 = 0.0f;                     // .rdata 0073D700

inline void* client_internal(void* client) { return at<void*>(client, 4); }
template <class F> inline F vslot(void* object, U offset) { return reinterpret_cast<F>(at<void*>(at<void*>(object, 0), offset)); }
using Fn0 = void*(__thiscall*)(void*);
using Fn1 = void*(__thiscall*)(void*, void*);
using Fn2 = void*(__thiscall*)(void*, U, U);
using FnInt = I(__thiscall*)(void*);
using FnProject = void*(__thiscall*)(void*, void* out, Vector in, U flag);                     // camera [vtbl+28h], RET 14h
using FnDelete = void*(__thiscall*)(void*, U);

// Scoped CExoString destruction while an exception unwinds (the original EH state cleanup).
struct StringGuard {
    void* s; bool armed = true;
    ~StringGuard() { if (armed) frame_LogStringDtor_005e5c20(s, nullptr); }
};

// CSWGuiFloatyText vftable 00753EA8: CSWGuiLabel 0073E5B8 (40 slots) with three overrides, 41 slots.
void* g_floaty_table[41];
bool g_floaty_built = false;
const void* floaty_vtable() {
    if (!g_floaty_built) {
        std::memcpy(g_floaty_table, gui_load_label_methods_0073e5b8, 40 * 4);
        g_floaty_table[0x00 / 4] = reinterpret_cast<void*>(&kotor_fastcall_0068acf0);   // 0068ACF0 ~CSWGuiLabel (deleting)
        g_floaty_table[0x38 / 4] = reinterpret_cast<void*>(&kotor_fastcall_006aeac0);   // 006AEAC0 CSWGuiBlackenedLabel::Draw
        g_floaty_table[0xa0 / 4] = reinterpret_cast<void*>(&kotor_fastcall_00688ff0);   // 00688FF0 CSWGuiFloatyText::Update
        g_floaty_built = true;
    }
    return g_floaty_table;
}
}  // namespace

extern "C" {
// =========================================================================================================
// 005EDEA0 CClientExoApp::AddFloatyText: ECX = [ECX+4]; jmp 006027C0. RET 0Ch.
void __fastcall kotor_fastcall_005edea0(void* client, void*, U id, U type, I value) {
    kotor_fastcall_006027c0(client_internal(client), nullptr, id, type, value);               // 005EDEA0..EDEA3
}

// 006027C0 CClientExoAppInternal::AddFloatyText(ulong id, uchar type, int value): RET 0Ch.
// Disabled unless the client options byte +14h has bit 10h (or there are no options).
void __fastcall kotor_fastcall_006027c0(void* internal, void*, U id, U type, I value) {
    void* options = kotor_fastcall_005ed700(client_app(), nullptr);                            // 006027DE..27EA
    if (options != nullptr && (at<U8>(options, 0x14) & 0x10) == 0) return;                    // 006027EF..27F9
    unsigned char color[12] = {};                                                              // [esp+4Ch] 006027FF..2813
    alignas(4) unsigned char text[8];                                                          // [esp+0Ch]
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(text), nullptr); // 006027FF..281B
    StringGuard text_guard{text};                                                              // EH state 0
    U scale = k_scale_normal_3fc00000;                                                         // [esp+8]
    const U kind = type & 0xffu;                                                               // 00602820 movzx byte
    if (kind > 6) {                                                                            // 00602824..282B
        text_guard.armed = false;
        frame_LogStringDtor_005e5c20(text, nullptr);                                           // 00602AA8..2AB4
        return;
    }
    alignas(4) unsigned char tmp[8];                                                           // [esp+14h..44h] per case
    switch (kind) {                                                                            // 00602831 table 00602ACC
    case 1:                                                                                    // 00602838
    case 0: {                                                                                  // 00602887
        kotor_fastcall_005e5bc0(tmp, nullptr, value);                                          // 0060283B..2840 / 0060288A..288F
        StringGuard g{tmp};
        (void)async_assign_string_005e5c50(text, nullptr, tmp);                                // 00602845..2853 / 00602894..28A2
        g.armed = false;
        frame_LogStringDtor_005e5c20(tmp, nullptr);                                            // 00602858..2860 / 006028A7..28AF
        std::memcpy(color, kind == 1 ? kotor_m15d_color_007a23e4 : kotor_m15d_color_007a23d8, 12); // 00602865..287E / 006028B4..2A81
        break;
    }
    case 2:                                                                                    // 006028CA (0x55D)
    case 5:                                                                                    // 006029EF (0x56F)
    case 6: {                                                                                  // 00602A3B (0xA5A3)
        const U strref = kind == 2 ? 0x55du : (kind == 5 ? 0x56fu : 0xa5a3u);
        void* got = kotor_fastcall_005fb540(internal, nullptr, tmp, strref);                   // 006028CA..28D6
        StringGuard g{tmp};
        (void)async_assign_string_005e5c50(text, nullptr, got);                                // 006028DB..28E5
        g.armed = false;
        frame_LogStringDtor_005e5c20(tmp, nullptr);                                            // 006028EA..28F2
        std::memcpy(color, k_white_007a23a8, 12);                                              // 006028F7..2910
        break;
    }
    case 3:                                                                                    // 00602919 (0x9697)
    case 4: {                                                                                  // 00602984 (0x7D9A)
        void* got = kotor_fastcall_005fb540(internal, nullptr, tmp, kind == 3 ? 0x9697u : 0x7d9au); // 00602919..2925
        StringGuard g{tmp};
        frame_LogFormat_005e5680(text, k_format_0073ee2c, WindowCStr005e5670(got, nullptr), value); // 0060292A..294A
        g.armed = false;
        frame_LogStringDtor_005e5c20(tmp, nullptr);                                            // 0060294D..2955
        if (kind == 3) std::memcpy(color, kotor_m1_color_007a23fc, 12);                        // 0060295A..2973
        else std::memcpy(color, w4::map_note_color_007a23f0, 12);                              // 006029C5..29DE
        scale = k_scale_large_40400000;                                                        // 00602977 / 006029E2
        break;
    }
    }
    kotor_fastcall_0062b080(at<void*>(internal, 0x40), nullptr, id, text, color, scale);       // 00602A8D..2AA3
    text_guard.armed = false;
    frame_LogStringDtor_005e5c20(text, nullptr);                                               // 00602AA8..2AB4
}

// 0062B080 CGuiInGame::AddFloatyText: ECX = [ECX+90h] (main interface); null -> RET 10h; else jmp 0068B7C0.
void __fastcall kotor_fastcall_0062b080(void* gui, void*, U id, void* text, const void* color, U scale_bits) {
    void* main_interface = at<void*>(gui, 0x90);                                               // 0062B080
    if (main_interface == nullptr) return;                                                     // 0062B086..B08F
    kotor_fastcall_0068b7c0(main_interface, nullptr, id, text, color, scale_bits);             // 0062B08A
}

// 0068B7C0 CSWGuiMainInterface::AddFloatyText: a new CSWGuiFloatyText; older entries for the same object move
// up (and are destroyed after five stacked entries); the new one is appended to the list at +5CB4. RET 10h.
void __fastcall kotor_fastcall_0068b7c0(void* self, void*, U id, void* text, const void* color, U scale_bits) {
    void* memory = live_operator_new(0x154);                                                   // 0068B7D9..B7E0
    void* entry = nullptr;
    if (memory != nullptr) {                                                                   // 0068B7F0..B7F6
        try {
            (void)gui_load_label_construct_0041acd0(memory, nullptr);                          // 0068B7F8..B7FA (EH state 0)
        } catch (...) {
            live_free(memory);
            throw;
        }
        at<const void*>(memory, 0) = floaty_vtable();                                          // 0068B7FF
        entry = memory;
    }
    kotor_fastcall_00688e10(entry, nullptr, id, text, color, scale_bits);                      // 0068B807..B826
    for (I i = at<I>(self, 0x5cb8) - 1; i >= 0; --i) {                                         // 0068B82B..B832, B8BD..B8BE
        void* e = at<void**>(self, 0x5cb4)[i];                                                 // 0068B840..B846
        if (at<U>(e, 0x148) != id) continue;                                                   // 0068B849..B84F
        const U8 stacked = static_cast<U8>(at<U8>(e, 0x14c) + 1);                              // 0068B851..B857
        at<U8>(e, 0x14c) = stacked;                                                            // 0068B85D
        if (stacked <= 5) {                                                                    // 0068B85B..B863 (ja)
            at<I>(e, 8) = at<I>(e, 8) - at<I>(e, 0x10);                                        // 0068B865..B870
            vslot<Fn1>(e, 4)(e, static_cast<unsigned char*>(e) + 4);                           // 0068B86D..B876 SetExtent
        } else {
            void* old = at<void**>(self, 0x5cb4)[i];                                           // 0068B87B..B881
            if (old != nullptr) vslot<FnDelete>(old, 0)(old, 1);                               // 0068B884..B88C
            const I count = at<I>(self, 0x5cb8) - 1;                                           // 0068B88E..B894
            at<I>(self, 0x5cb8) = count;                                                       // 0068B89B
            for (I j = i; j < at<I>(self, 0x5cb8); ++j)                                        // 0068B897..B8BB
                at<void**>(self, 0x5cb4)[j] = at<void**>(self, 0x5cb4)[j + 1];
        }
    }
    const U capacity = at<U>(self, 0x5cbc);                                                    // 0068B8C0
    if (at<U>(self, 0x5cb8) == capacity) {                                                     // 0068B8C6..B8CD
        const U grown = capacity != 0 ? capacity + capacity : 0x10;                            // 0068B8CF..B8D7
        void** old = at<void**>(self, 0x5cb4);                                                 // 0068B8DC
        at<U>(self, 0x5cbc) = grown;                                                           // 0068B8E2
        at<void**>(self, 0x5cb4) = static_cast<void**>(live_operator_new(grown << 2));         // 0068B8E8..B8F7
        for (I k = 0; k < at<I>(self, 0x5cb8); ++k) at<void**>(self, 0x5cb4)[k] = old[k];      // 0068B900..B925
        if (old != nullptr) live_free(old);                                                    // 0068B927..B931
    }
    at<void**>(self, 0x5cb4)[at<U>(self, 0x5cb8)] = entry;                                     // 0068B934..B940
    at<U>(self, 0x5cb8) = at<U>(self, 0x5cb8) + 1;                                             // 0068B943..B94F
}

// 00688E10 CSWGuiFloatyText::CreateEntry(ulong id, CExoString const& text, Vector color, float seconds): RET 10h.
void __fastcall kotor_fastcall_00688e10(void* self, void*, U id, void* text, const void* color, U scale_bits) {
    at<U>(self, 0x144) = scale_bits;                                                           // 00688E13..E22
    void* params = static_cast<unsigned char*>(self) + 0xe8;                                   // 00688E28
    at<U>(self, 0x140) = scale_bits;                                                           // 00688E32
    at<U8>(self, 0x14c) = 1;                                                                   // 00688E38
    at<U>(self, 0x148) = id;                                                                   // 00688E3F
    gui_load_text_params_set_alignment_00415f20(params, nullptr, 0x12);                        // 00688E2E..E45
    gui_load_text_params_set_text_00415e00(params, nullptr, text);                             // 00688E4A..E51
    gui_load_text_params_set_color_00414e10(params, nullptr, color);                           // 00688E56..E5D
    at<U>(self, 0x110) = 0x3f800000u;                                                          // 00688E6B (1.0f)
    menu_resources::Ref font;                                                                  // [esp+24h]
    font.construct_00406d80(k_font_00753da0);                                                  // 00688E62..E75
    gui_load_text_params_set_base_font_00415dd0(params, nullptr, &font);                       // 00688E7A..E81
    Vector anchor = {0.0f, 0.0f, 0.0f};                                                        // [esp+0Ch] 00688E91..EA1
    void* object = kotor_fastcall_005ed580(client_app(), nullptr, at<U>(self, 0x148));         // 00688E86..EAD
    if (object == nullptr) return;                                                             // 00688EB2..EB4
    void* visible = vslot<Fn0>(object, 0xc)(object);                                           // 00688EBA..EBE
    if (visible == nullptr) return;                                                            // 00688EC1..EC5
    vslot<Fn1>(visible, 0x13c)(visible, &anchor);                                              // 00688ECB..ED4
    at<U>(self, 0xc) = 0xc8;                                                                   // 00688EE0
    const I height = gui_load_text_font_height_00414ee0(static_cast<unsigned char*>(self) + 0xd0, nullptr); // 00688EDA..EE7
    at<I>(self, 0x10) = height;                                                                // 00688EFA
    at<U>(self, 8) = w2::ftol2_006fae8c((static_cast<double>(anchor.y) - k_rise_00753d80) - static_cast<double>(height)); // 00688EEC..EF6, EFD..F10
    at<U>(self, 4) = w2::ftol2_006fae8c(static_cast<double>(anchor.x) - k_half_width_00741838); // 00688F06..F20
    vslot<Fn1>(self, 4)(self, static_cast<unsigned char*>(self) + 4);                          // 00688F1B..F22 SetExtent
    if (vslot<FnInt>(visible, 0x2c)(visible) == 0) return;                                     // 00688F25..F2E
    void* model = vslot<Fn2>(visible, 0x98)(visible, 0xff, 1);                                 // 00688F34..F45
    void* camera = kotor_fastcall_005ed560(client_app(), nullptr);                             // 00688F47..F54
    Vector position;                                                                           // [esp+24h]
    const Vector* p = static_cast<const Vector*>(vslot<Fn1>(model, 0x64)(model, &position));   // 00688F54..F62
    Vector screen;                                                                             // [esp+18h]
    vslot<FnProject>(camera, 0x28)(camera, &screen, *p, 1);                                    // 00688F62..F82
    const I width_px = at<std::int16_t>(kotor_fastcall_005eda40(client_app(), nullptr), 0x6c); // 00688F85..F93
    at<U16>(self, 0x14e) = static_cast<U16>(w2::ftol2_006fae8c(static_cast<double>(width_px) * screen.x)); // 00688F97..FA8
    const I height_px = at<std::int16_t>(kotor_fastcall_005eda40(client_app(), nullptr), 0x6e); // 00688FAF..FBD
    at<U16>(self, 0x150) = static_cast<U16>(w2::ftol2_006fae8c(static_cast<double>(height_px) * screen.y)); // 00688FC1..FD2
}

// 00688FF0 CSWGuiFloatyText::Update(float dt): counts down, removes itself when expired, otherwise tracks the
// object's screen position while it is in front of the camera. RET 4.
void __fastcall kotor_fastcall_00688ff0(void* self, void*, float dt) {
    const double left = static_cast<double>(at<float>(self, 0x140)) - dt;                     // 00688FF8..8FFF
    at<float>(self, 0x140) = static_cast<float>(left);                                         // 00689003 (fst dword)
    if (left <= k_zero_0073d700) {                                                             // 00689009..9014 (unrounded ST0)
        kotor_fastcall_00688060(w4::main_interface_00833bb4, nullptr, self);                   // 00689016..901D
        return;
    }
    void* object = kotor_fastcall_005ed580(client_app(), nullptr, at<U>(self, 0x148));         // 0068902C..903B
    if (object != nullptr) {                                                                   // 00689040..9042
        void* visible = vslot<Fn0>(object, 0xc)(object);                                       // 00689048..904C
        if (visible != nullptr && vslot<FnInt>(visible, 0x2c)(visible) != 0) {                 // 0068904F..9062
            void* model = vslot<Fn2>(visible, 0x98)(visible, 0xff, 1);                         // 00689068..9079
            void* camera = kotor_fastcall_005ed560(client_app(), nullptr);                     // 0068907B..9088
            alignas(4) unsigned char orientation[16];                                          // [esp+28h]
            Vector z;                                                                          // [esp+1Ch]
            void* q = vslot<Fn1>(camera, 0x24)(camera, orientation);                           // 0068908A..9098 GetOrientation
            const Vector* zd = static_cast<const Vector*>(quaternion_zdir_0043e250(q, nullptr, &z));                      // 0068909B..909D (arg pushed at 00689090)
            Vector view;                                                                       // [esp+10h]
            view.x = static_cast<float>(-static_cast<double>(zd->x));                          // 006890A2..90A8
            view.y = static_cast<float>(-static_cast<double>(zd->y));                          // 006890AE..90B3
            view.z = static_cast<float>(-static_cast<double>(zd->z));                          // 006890B7..90C1
            Vector tmp;                                                                        // [esp+1Ch]
            const Vector* mp = static_cast<const Vector*>(vslot<Fn1>(model, 0x64)(model, &tmp)); // 006890BA..90C5
            Vector cam_tmp;                                                                    // [esp+28h]
            const Vector* cp = static_cast<const Vector*>(vslot<Fn1>(camera, 0x20)(camera, &cam_tmp)); // 006890C8..90D3
            const double a = (static_cast<double>(view.z) * mp->z + static_cast<double>(view.y) * mp->y)
                           + static_cast<double>(view.x) * mp->x;                              // 006890D6..90ED
            const double b = (static_cast<double>(view.z) * cp->z + static_cast<double>(view.y) * cp->y)
                           + static_cast<double>(view.x) * cp->x;                              // 006890EF..9105
            if (b < a) {                                                                       // 00689107..910E (fcompp; test ah,5; jp)
                Vector pos_tmp;                                                                // [esp+28h]
                const Vector* p = static_cast<const Vector*>(vslot<Fn1>(model, 0x64)(model, &pos_tmp)); // 00689114..911D
                Vector screen;                                                                 // [esp+10h]
                vslot<FnProject>(camera, 0x28)(camera, &screen, *p, 1);                        // 00689120..9140
                const I width_px = at<std::int16_t>(kotor_fastcall_005eda40(client_app(), nullptr), 0x6c); // 00689143..9151
                const U sx = w2::ftol2_006fae8c(static_cast<double>(width_px) * screen.x);     // 00689155..9161
                const I height_px = at<std::int16_t>(kotor_fastcall_005eda40(client_app(), nullptr), 0x6e); // 00689166..9176
                const U sy = w2::ftol2_006fae8c(static_cast<double>(height_px) * screen.y);    // 0068917A..9186
                const I nx = static_cast<std::int16_t>(sx), ny = static_cast<std::int16_t>(sy);
                at<I>(self, 8) = at<I>(self, 8) + (ny - static_cast<I>(at<std::int16_t>(self, 0x150))); // 0068918B..919A
                at<I>(self, 4) = at<I>(self, 4) + (nx - static_cast<I>(at<std::int16_t>(self, 0x14e))); // 00689192..91AE
                at<U16>(self, 0x150) = static_cast<U16>(sy);                                   // 006891B0
                at<U16>(self, 0x14e) = static_cast<U16>(sx);                                   // 006891BC
                vslot<Fn1>(self, 4)(self, static_cast<unsigned char*>(self) + 4);              // 006891A4..91C3 SetExtent
                at<U>(self, 0x44) = at<U>(self, 0x44) | 2u;                                    // 006891C6..91C9
            } else {
                at<U>(self, 0x44) = at<U>(self, 0x44) & ~2u;                                   // 006891CE..91D1
            }
        }
    }
    at<float>(self, 0x110) = static_cast<float>(static_cast<double>(at<float>(self, 0x140)) / at<float>(self, 0x144)); // 006891D7..91E4
}

// 00688060 CSWGuiMainInterface::RemoveFloatyText(CSWGuiFloatyText*): RET 4.
void __fastcall kotor_fastcall_00688060(void* self, void*, void* floaty) {
    if (floaty == nullptr) return;                                                             // 00688061..8067
    const I count = at<I>(self, 0x5cb8);                                                       // 00688069
    void** list = at<void**>(self, 0x5cb4);
    for (I i = 0; i < count; ++i) {                                                            // 00688075..808A
        if (list[i] == floaty) {                                                               // 00688080..8082
            if (i != -1) deleteAt_006857a0(static_cast<unsigned char*>(self) + 0x5cb4, nullptr, i); // 00688099..809F
            break;
        }
    }
    vslot<FnDelete>(floaty, 0)(floaty, 1);                                                     // 0068808C..8093 / 006880A5..80AB
}

// 0068ACF0 CSWGuiLabel::`scalar deleting destructor'(uint flags): ~CSWGuiLabel; free when bit 0. RET 4, EAX = this.
void* __fastcall kotor_fastcall_0068acf0(void* self, void*, U flags) {
    kotor_fastcall_0068ad10(self, nullptr);                                                    // 0068ACF1..ACF3
    if ((flags & 1u) != 0) live_free(self);                                                    // 0068ACF8..AD05
    return self;                                                                               // 0068AD08
}

// 006AEAC0 CSWGuiBlackenedLabel::Draw(int): the text four times in black at +-1 pixel offsets (outline), then the
// label in its own colour. RET 4.
void __fastcall kotor_fastcall_006aeac0(void* self, void*, U arg) {
    unsigned char* const s = static_cast<unsigned char*>(self);
    I rect[4];                                                                                 // [esp+1Ch]
    std::memcpy(rect, s + 0xd4, 16);                                                           // 006AEAC9..AEEE
    unsigned char saved_color[12];                                                             // [esp+10h]
    std::memcpy(saved_color, s + 0x104, 12);                                                   // 006AEAE6..AF0D
    void* params = s + 0xe8;                                                                   // 006AEAFC
    gui_load_text_params_set_color_00414e10(params, nullptr, &kotor_gui_m48_cells::viewport_color_00833910); // 006AEB02..EB11
    void* text = s + 0xd0;                                                                     // 006AEB2A
    using SetExtentFn = void(__thiscall*)(void*, I*);
    using DrawFn = void(__thiscall*)(void*, U);
    rect[0] -= 1; rect[1] += 1;                                                                // 006AEB16..EB34
    reinterpret_cast<SetExtentFn>(at<void*>(at<void*>(text, 0), 4))(text, rect);              // 006AEB38..EB3B
    reinterpret_cast<DrawFn>(at<void*>(at<void*>(text, 0), 0xc))(text, arg);                   // 006AEB3E..EB47
    rect[0] += 2;                                                                              // 006AEB4A..EB55
    reinterpret_cast<SetExtentFn>(at<void*>(at<void*>(text, 0), 4))(text, rect);              // 006AEB59..EB5E
    reinterpret_cast<DrawFn>(at<void*>(at<void*>(text, 0), 0xc))(text, arg);                   // 006AEB61..EB66
    rect[1] -= 2;                                                                              // 006AEB69..EB74
    reinterpret_cast<SetExtentFn>(at<void*>(at<void*>(text, 0), 4))(text, rect);              // 006AEB78..EB7D
    reinterpret_cast<DrawFn>(at<void*>(at<void*>(text, 0), 0xc))(text, arg);                   // 006AEB80..EB85
    rect[0] -= 2;                                                                              // 006AEB88..EB93
    reinterpret_cast<SetExtentFn>(at<void*>(at<void*>(text, 0), 4))(text, rect);              // 006AEB97..EB9C
    reinterpret_cast<DrawFn>(at<void*>(at<void*>(text, 0), 0xc))(text, arg);                   // 006AEB9F..EBA4
    gui_load_text_params_set_color_00414e10(params, nullptr, saved_color);                     // 006AEBA7..EBAE
    rect[0] += 1; rect[1] += 1;                                                                // 006AEBB3..EBCA
    reinterpret_cast<SetExtentFn>(at<void*>(at<void*>(text, 0), 4))(text, rect);              // 006AEBC5..EBCE
    kotor_fastcall_00417750(self, nullptr, arg);                                               // 006AEBD1..EBD4 CSWGuiLabel::Draw
}
}  // extern "C"
}  // namespace w1c_floaty
