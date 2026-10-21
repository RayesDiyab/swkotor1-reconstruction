// Lane gui-load: CSWGuiBorder::Load 004153E0 (texture names -> BorderParams image
// setters -> NewCAurGUIImage requests) and CSWGuiText::Load 00416050 (font resref,
// color, text/strref, alignment -> NewCAurGUIString request), plus the tiny
// BorderParams::SetPulsingAlpha 00414D00 they reach.
#include "14733da5f4-gui_control_load.h"
#include <cstring>

extern "C" {
// M40 window-native-strings.obj.
char* __fastcall WindowCStr005e5670(gui_load::String*, void*);

// ---- authored here ----
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, gui_load::Word a,
                                                            gui_load::Word b, gui_load::Word c);
void __fastcall gui_load_border_load_004153e0(gui_load::Border*, void*, gui_load::Gff*, gui_load::Struct*,
                                              gui_load::String*);
void __fastcall gui_load_text_load_00416050(gui_load::Text*, void*, gui_load::Gff*, gui_load::Struct*,
                                            gui_load::String*);

// ---- external, MISSING natively in M40 (texture/font acquisition boundary) ----
// 00414AE0 / 00414B70 CSWGuiBorderParams::SetCornerImage / SetEdgeImage(CResRef const&, int) RET 8.
// Donor (Context form): task-17/gui-parameter-closure-r1 border_params.cpp. Each reaches
// NewCAurGUIImage 0045A260 with the resref copied to a 17-byte name (texture request).
void __fastcall gui_load_border_params_set_corner_00414ae0(void*, void*, const gui_load::ResRef*, std::int32_t) noexcept(false);
void __fastcall gui_load_border_params_set_edge_00414b70(void*, void*, const gui_load::ResRef*, std::int32_t) noexcept(false);
// 00415DD0 SetBaseFont(CResRef const&) RET 4 -> CopyToString + SetFontRename 00415D60 (font request).
void __fastcall gui_load_text_params_set_base_font_00415dd0(void*, void*, const gui_load::ResRef*) noexcept(false);
// 00414E50 SetDefaultColor(Vector const&) RET 4 (tail-jumps SetColor 00414E10).
void __fastcall gui_load_text_params_set_default_color_00414e50(void*, void*, const gui_load::Vector*);
// 00415E00 SetText(CExoString const&) RET 4.
void __fastcall gui_load_text_params_set_text_00415e00(void*, void*, const gui_load::String*) noexcept(false);
// 00415E50 SetStrRef(DWORD) RET 4 (CTlkTable::Fetch 0041E550).
void __fastcall gui_load_text_params_set_strref_00415e50(void*, void*, gui_load::Word) noexcept(false);
// 00415F20 SetAlignment(int) RET 4.
void __fastcall gui_load_text_params_set_alignment_00415f20(void*, void*, std::int32_t);
// 00415F70 SetTextObject(CSWGuiText*) RET 4.
void __fastcall gui_load_text_params_set_text_object_00415f70(void*, void*, gui_load::Text*) noexcept(false);
// 0045BDF0 NewCAurGUIString(char const* text, char const* font, Vector const* color, float) __cdecl.
void* __cdecl gui_load_new_aur_gui_string_0045bdf0(const char*, const char*, const void*, float) noexcept(false);
}

namespace {
using namespace gui_load;
// CSWGuiText view (0x70): +14 CAurGUIString* (004161DC/0041621F), +18 TextParams
// (004160BD): params+00 CExoString text (0041620C CStr), params+0C CResRef font
// (004161F6 = text+24), params+1C color (00416203 = text+34), params+38 flags (004161C7).
struct AurObjectVtbl { const void* slot00; void (__fastcall* release)(void*, void*, Word); };   // +04 called with 1 (004161E7)
struct AurObject { const AurObjectVtbl* vtbl; };
struct TextView {
    const void* vtbl;
    unsigned char u04[0x10];
    AurObject* object;                          // +14
    String text;                                // +18 (params+00)
    unsigned char u20[4];
    ResRef font;                                // +24 (params+0C)
    unsigned char color[0x0C];                  // +34 (params+1C)
    unsigned char u40[0x10];
    Word flags;                                 // +50 (params+38) bit6 = pulsing
    unsigned char u54[0x1C];
};
static_assert(sizeof(TextView) == 0x70 && offsetof(TextView, text) == 0x18 && offsetof(TextView, font) == 0x24 &&
              offsetof(TextView, color) == 0x34 && offsetof(TextView, flags) == 0x50, "text view");
// CSWGuiBorder view (0x74): params at +14; params+00 DIMENSION (004154FA), params+04
// INNEROFFSET (00415514 = border+18), params+10 color (00415551 = border+24),
// params+1C style bits (004154D2..E1).
struct BorderView {
    const void* vtbl;
    unsigned char u04[0x10];
    std::int32_t dimension;                     // +14
    std::int32_t inner_offset;                  // +18
    unsigned char u1c[8];
    Vector color;                               // +24
};
}

extern "C" {

// 00414D00 SetPulsingAlpha(a,b,c): bits = (((c&1)<<1 | (b&1))<<1 | (a&1)) << 2;
// params+1C = (old & 0xFFFFFFE3) | bits (00414D00..28). RET 0Ch.
void __fastcall gui_load_border_params_set_pulsing_00414d00(void* params, void*, Word a, Word b, Word c) {
    Word bits = ((c & 1u) << 1) | (b & 1u);
    bits = ((bits << 1) | (a & 1u)) << 2;
    Word* flags = reinterpret_cast<Word*>(static_cast<unsigned char*>(params) + 0x1C);
    *flags = (*flags & 0xFFFFFFE3u) | bits;
}

// 004153E0 CSWGuiBorder::Load(gff, struct, CExoString const& label) RET 0Ch:
// CStr(label) (004153EB); GetStructFromStruct (00415401) else return.
// CORNER/EDGE/FILL resrefs, each with a fresh "" default (00415418/53/8B), feed
// SetCornerImage/SetEdgeImage/SetFillImage(ref, 0) on params(+14) (00415445/7D/B5).
// FILLSTYLE int default 0 -> params+1C low 2 bits (004154CD..E1);
// DIMENSION -> params+00 (004154FA); INNEROFFSET -> border+18 (00415514);
// COLOR default (1,1,1) -> border+24 (0041554A..72); PULSING byte default 0 ->
// SetPulsingAlpha(al!=0, al&1, (al>>1)&1) (00415575..94).
void __fastcall gui_load_border_load_004153e0(Border* self, void*, Gff* gff, Struct* s, String* label) {
    const char* text = WindowCStr005e5670(label, nullptr);
    Struct sub;
    if (!gui_load_gff_struct_00411a10(gff, nullptr, &sub, s, text)) return;
    unsigned char* params = self->params;
    Word success;
    {
        ResRef none; none.construct_00406d80("");
        ResRef value;
        const ResRef* read = gui_load_gff_resref_00411e10(gff, nullptr, &value, &sub, "CORNER", &success, &none);
        gui_load_border_params_set_corner_00414ae0(params, nullptr, read, 0);
    }
    {
        ResRef none; none.construct_00406d80("");
        ResRef value;
        const ResRef* read = gui_load_gff_resref_00411e10(gff, nullptr, &value, &sub, "EDGE", &success, &none);
        gui_load_border_params_set_edge_00414b70(params, nullptr, read, 0);
    }
    {
        ResRef none; none.construct_00406d80("");
        ResRef value;
        const ResRef* read = gui_load_gff_resref_00411e10(gff, nullptr, &value, &sub, "FILL", &success, &none);
        gui_load_border_params_set_fill_00414c00(params, nullptr, read, 0);
    }
    BorderView* view = reinterpret_cast<BorderView*>(self);
    Word* style = reinterpret_cast<Word*>(params + 0x1C);
    const std::int32_t fill_style = gui_load_gff_int_00411c90(gff, nullptr, &sub, "FILLSTYLE", &success, 0);
    *style = *style ^ ((*style ^ static_cast<Word>(fill_style)) & 3u);
    view->dimension = gui_load_gff_int_00411c90(gff, nullptr, &sub, "DIMENSION", &success, 0);
    view->inner_offset = gui_load_gff_int_00411c90(gff, nullptr, &sub, "INNEROFFSET", &success, 0);
    const Vector white = { 1.0f, 1.0f, 1.0f };
    Vector color;
    const Vector* read_color = gui_load_gff_vector_004122a0(gff, nullptr, &color, &sub, "COLOR", &success, &white);
    view->color = *read_color;
    const unsigned char pulsing = gui_load_gff_byte_00411a60(gff, nullptr, &sub, "PULSING", &success, 0);
    gui_load_border_params_set_pulsing_00414d00(params, nullptr, pulsing != 0 ? 1u : 0u, pulsing & 1u,
                                                (static_cast<Word>(pulsing) >> 1) & 1u);
}

// 00416050 CSWGuiText::Load(gff, struct, CExoString const& label) RET 0Ch:
// CStr(label) (00416070); GetStructFromStruct (00416086) else return.
// FONT resref default "dialogfont16x16" (00416094..C0) -> SetBaseFont (004160C8);
// COLOR default (1,1,1) -> SetDefaultColor (00416100/08);
// TEXT string default "" (0041610D..3E) -> SetText (0041614B), temp then default
// destroyed (00416159/6A); STRREF dword default -1 -> SetStrRef (00416182/8A);
// ALIGNMENT int default 9 -> SetAlignment (004161A2/AA); PULSING byte default 0 ->
// flags bit6 (004161C2..D9); old CAurGUIString vslot +04(1) and clear +14
// (004161DC..EA); CopyToString(font) (004161F9); NewCAurGUIString(CStr(text),
// fontname, &color, 1.0f) (004161FE..19) -> +14 (0041621F); SetTextObject(this) (00416222).
void __fastcall gui_load_text_load_00416050(Text* self, void*, Gff* gff, Struct* s, String* label) {
    const char* text = WindowCStr005e5670(label, nullptr);
    Struct sub;
    if (!gui_load_gff_struct_00411a10(gff, nullptr, &sub, s, text)) return;
    TextView* view = reinterpret_cast<TextView*>(self);
    void* params = &view->text;
    Word success;
    {
        ResRef base; base.construct_00406d80("dialogfont16x16");
        ResRef value;
        const ResRef* font = gui_load_gff_resref_00411e10(gff, nullptr, &value, &sub, "FONT", &success, &base);
        gui_load_text_params_set_base_font_00415dd0(params, nullptr, font);
    }
    {
        const Vector white = { 1.0f, 1.0f, 1.0f };
        Vector color;
        const Vector* read = gui_load_gff_vector_004122a0(gff, nullptr, &color, &sub, "COLOR", &success, &white);
        gui_load_text_params_set_default_color_00414e50(params, nullptr, read);
    }
    {
        String empty("");
        alignas(4) unsigned char storage[sizeof(String)];
        String* read = gui_load_gff_string_00411ec0(gff, nullptr, reinterpret_cast<String*>(storage), &sub, "TEXT",
                                                    &success, &empty);
        gui_load_text_params_set_text_00415e00(params, nullptr, read);
        read->~String();
    }
    gui_load_text_params_set_strref_00415e50(params, nullptr,
        gui_load_gff_dword_00411c20(gff, nullptr, &sub, "STRREF", &success, 0xFFFFFFFFu));
    gui_load_text_params_set_alignment_00415f20(params, nullptr,
        gui_load_gff_int_00411c90(gff, nullptr, &sub, "ALIGNMENT", &success, 9));
    const unsigned char pulsing = gui_load_gff_byte_00411a60(gff, nullptr, &sub, "PULSING", &success, 0);
    const Word old_flags = view->flags;
    view->flags = old_flags ^ (((static_cast<Word>(pulsing) << 6) ^ old_flags) & 0x40u);
    if (AurObject* old = view->object) {
        old->vtbl->release(old, nullptr, 1);
        view->object = nullptr;
    }
    char font_name[17];
    gui_load_resref_copy_to_string_00405fb0(&view->font, nullptr, font_name);
    view->object = static_cast<AurObject*>(gui_load_new_aur_gui_string_0045bdf0(
        WindowCStr005e5670(&view->text, nullptr), font_name, view->color, 1.0f));
    gui_load_text_params_set_text_object_00415f70(params, nullptr, self);
}

} // extern "C"
