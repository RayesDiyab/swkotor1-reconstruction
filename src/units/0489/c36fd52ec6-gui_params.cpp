// Lane gui-load: CSWGuiBorderParams / CSWGuiTextParams bodies (texture and font
// acquisition requests of the main-menu controls), GetInnerExtent / Intersection
// and the CSWGuiManager font-name helper. Every behavior cites its instructions.
#include "e0b892ba6e-gui_objects.h"
#include <cstring>
#include <new>

using namespace gui_load;

namespace {
// Original CResRef::operator= 00406120 returns its result by value into a caller
// temporary (00414B04 lea eax,[esp+10h]); the temporary is otherwise unused.
void assign_ref(Ref* dest, const Ref* source) {
    Ref unused;
    c2da_copy_00406120(dest, nullptr, &unused, const_cast<Ref*>(source));
}
String* as_string(unsigned char* bytes) { return reinterpret_cast<String*>(bytes); }
BorderObject* border_view(Border* border) { return reinterpret_cast<BorderObject*>(border); }
TextObject* text_view(Text* text) { return reinterpret_cast<TextObject*>(text); }
// Shared body of 00414AE0 / 00414B70 / 00414C00 (identical code, different slots):
// compare (operator!= with ECX = source, arg = slot ref; 00414AF2) unless forced;
// assign through 00406120 (00414B0B); owner +50 null -> done (00414B13);
// release previous image via its vslot +04(1) and clear (00414B17..2A);
// IsValid (00414B33) -> CopyToString into a 20-byte local (00414B43) ->
// NewCAurGUIImage (00414B4D) stored into the owner (re-read +50, 00414B52..58).
void set_image(BorderParams* params, Ref* slot, AurImage* BorderObject::*image, const Ref* source, I32 force) {
    if (!gui_load_resref_not_equal_00406dc0(source, nullptr, slot) && !force) return;
    assign_ref(slot, source);
    BorderObject* owner = reinterpret_cast<BorderObject*>(params->owner);
    if (!owner) return;
    if (AurImage* old = owner->*image) {
        old->vtbl->release(old, nullptr, 1);
        (reinterpret_cast<BorderObject*>(params->owner))->*image = nullptr;
    }
    if (!gui_load_resref_is_valid_00406030(slot, nullptr)) return;
    char name[20];
    gui_load_resref_copy_to_string_00405fb0(slot, nullptr, name);
    AurImage* created = reinterpret_cast<AurImage*>(kotor_cdecl_0045a260(name));
    (reinterpret_cast<BorderObject*>(params->owner))->*image = created;
}
// Common tail of SetText 00415E00 / SetStrRef 00415E50: owner text object and its
// CAurGUIString present -> vtable word loaded BEFORE CStr (00415E23), CStr
// (00415E27), receiver re-read (00415E2C..2F), vslot +18 (00415E33), wrapText on
// the re-read text object (00415E36..39).
void push_text(TextParams* params) {
    TextObject* object = params->text_object;
    if (!object || !object->string) return;
    const AurStringVtbl* vt = object->string->vtbl;
    const char* text = WindowCStr005e5670(as_string(params->text), nullptr);
    AurString* receiver = params->text_object->string;
    vt->set_text(receiver, nullptr, text);
    gui_load_text_wrap_00414e90(params->text_object, nullptr);
}
}

extern "C" {

// ------------------------------------------------------------------ BorderParams
void __fastcall gui_load_border_params_set_corner_00414ae0(void* params, void*, const Ref* source, I32 force) noexcept(false) {
    BorderParams* p = static_cast<BorderParams*>(params);
    set_image(p, &p->corner, &BorderObject::corner, source, force);       // slot +20, image +68
}
void __fastcall gui_load_border_params_set_edge_00414b70(void* params, void*, const Ref* source, I32 force) noexcept(false) {
    BorderParams* p = static_cast<BorderParams*>(params);
    set_image(p, &p->edge, &BorderObject::edge, source, force);           // slot +30, image +6C
}
void __fastcall gui_load_border_params_set_fill_00414c00(void* params, void*, const Ref* source, I32 force) noexcept(false) {
    BorderParams* p = static_cast<BorderParams*>(params);
    set_image(p, &p->fill, &BorderObject::fill, source, force);           // slot +40, image +70 (00414C37/78)
}

// 00414C90 SetBorder(border): +50 = border always (00414C99); if nonnull force-set
// corner/edge/fill from the params' own refs (00414C9E..BE).
void __fastcall gui_load_border_params_set_border_00414c90(BorderParams* self, void*, Border* border) noexcept(false) {
    self->owner = border;
    if (!border) return;
    gui_load_border_params_set_corner_00414ae0(self, nullptr, &self->corner, 1);
    gui_load_border_params_set_edge_00414b70(self, nullptr, &self->edge, 1);
    gui_load_border_params_set_fill_00414c00(self, nullptr, &self->fill, 1);
}

// 00415000 ctor(corner, edge, fill, fill_style, dimension, f0c, color*, inner) RET 20h.
BorderParams* __fastcall gui_load_border_params_construct_00415000(BorderParams* self, void*,
    const Ref* corner, const Ref* edge, const Ref* fill, Word fill_style, I32 dimension, float f0c,
    const Vector* color, I32 inner_offset) noexcept(false) {
    std::memset(&self->color, 0, sizeof(self->color));                   // +10,+14,+18 = 0 (00415010..16)
    c2da_default_ref_00405ed0(&self->corner, nullptr);                     // 00415019
    c2da_default_ref_00405ed0(&self->edge, nullptr);                       // 00415023
    c2da_default_ref_00405ed0(&self->fill, nullptr);                       // 0041502D
    assign_ref(&self->corner, corner);                                     // 0041503E
    assign_ref(&self->edge, edge);                                         // 0041504F
    assign_ref(&self->fill, fill);                                         // 00415060
    self->dimension = dimension;                                           // 00415071
    Word flags = (self->flags & 0xFFFFFFF8u) | (fill_style & 3u);          // 00415073..7E
    self->flags = flags;
    std::memcpy(&self->f0c, &f0c, 4);                                      // 00415081 raw dword
    self->color.x = color->x;                                              // 00415088..8A
    self->color.y = color->y;                                              // 0041508D..90
    const float z = color->z;                                              // 00415093
    self->flags = flags & 0xFFFFFFE7u;                                     // 0041509A..9D
    self->owner = nullptr;                                                 // 004150A3
    self->u08 = 0;                                                         // 004150A6
    self->color.z = z;                                                     // 004150A9
    self->inner_offset = inner_offset;                                     // 004150AC
    return self;
}

// 004150C0 operator=: refs (004150D7/E8/F9); style bits0-1 merged (004150FE..0E);
// +08 float copy (00415111/14); +00 = source GetBorderDim (00415119..21);
// +0C float copy (00415123/26); bit2 merged (00415129..34); color copy
// (00415137..4A); bit3 then bit4 merged (0041514D..6F); +04 copy (00415172..78);
// SetBorder(own +50) (0041517B). Returns this.
BorderParams* __fastcall gui_load_border_params_assign_004150c0(BorderParams* self, void*, const BorderParams* other) noexcept(false) {
    assign_ref(&self->corner, &other->corner);
    assign_ref(&self->edge, &other->edge);
    assign_ref(&self->fill, &other->fill);
    self->flags = self->flags ^ ((self->flags ^ other->flags) & 3u);
    float f08; std::memcpy(&f08, &other->u08, 4); std::memcpy(&self->u08, &f08, 4);   // FLD/FSTP dword
    const I32 dim = gui_load_border_params_dim_00414cd0(other, nullptr);
    const Word flags_now = self->flags;                                    // 0041511E read before store
    self->dimension = dim;
    self->f0c = other->f0c;
    self->flags = self->flags ^ ((flags_now ^ other->flags) & 4u);
    self->color = other->color;
    Word merged = self->flags ^ ((self->flags ^ other->flags) & 8u);
    self->flags = merged;
    merged = merged ^ ((other->flags ^ merged) & 0x10u);
    Border* owner = self->owner;                                           // 0041516C read
    self->flags = merged;
    self->inner_offset = other->inner_offset;
    gui_load_border_params_set_border_00414c90(self, nullptr, owner);
    return self;
}

// 00414CD0 GetBorderDim: single body = lane-gui-render @kotor_fastcall_00414cd0@8 (D62/D65 C2); // gui_load_border_params_dim_00414cd0 is joined to it in m45-joins/integrator_joins_m45.cpp (fastcall(ECX,EDX) @8 both).

// 0040A4A0 CSWGuiExtent::Intersection(out, a, b) __cdecl.
Extent* __cdecl gui_load_extent_intersection_0040a4a0(Extent* out, const Extent* a, const Extent* b) {
    I32 left = a->left;  if (!(left > b->left)) left = b->left;           // 0040A4B1..BA
    I32 top = a->top;    if (!(top > b->top)) top = b->top;               // 0040A4C4..CC
    I32 right = a->left + a->width; const I32 right_b = b->left + b->width;
    if (!(right < right_b)) right = right_b;                               // 0040A4D4..E0
    I32 bottom = a->top + a->height; const I32 bottom_b = b->top + b->height;
    if (!(bottom < bottom_b)) bottom = bottom_b;                           // 0040A4E2..F6
    out->left = left; out->top = top; out->width = right - left; out->height = bottom - top;
    return out;
}

// 00415240 CSWGuiBorder::GetInnerExtent(out) RET 4 (transcribed register by register).
Extent* __fastcall gui_load_border_inner_extent_00415240(Border* border, void*, Extent* out) {
    BorderObject* self = border_view(border);
    BorderParams* params = &self->params;
    I32 a, b, step;                                                       // EBP, [esp+10h], ECX
    if (gui_load_border_params_dim_00414cd0(params, nullptr)) {           // 0041524E
        step = gui_load_border_params_dim_00414cd0(params, nullptr);      // 00415259
        a = step; b = step;
    } else {
        a = 0;
        AurImage* corner = self->corner;
        if (corner && self->edge && self->extent.height && self->extent.width) {
            b = corner->vtbl->width(corner, nullptr);                     // 00415282
            a = self->corner->vtbl->height(self->corner, nullptr);        // 0041528E
            (void)self->edge->vtbl->width(self->edge, nullptr);           // 00415298 (result discarded)
            step = self->edge->vtbl->height(self->edge, nullptr);         // 004152A0
        } else {
            step = 0; b = 0;                                              // 004152A7..A9
        }
    }
    I32 x, y, w, h;
    {   // X axis 004152AD..00415303
        const I32 width = self->extent.width;
        const I32 span = width - 2 * a;
        const I32 q = step ? span / step : 0;
        if (span > 0 && (q != 0 || !(step / 2 > span))) {
            x = self->extent.left + a; w = span < 0 ? 0 : span;
        } else {
            x = width / 2 + self->extent.left; w = 0;
        }
    }
    {   // Y axis 00415303..00415360
        const I32 height = self->extent.height;
        const I32 span = height - 2 * b;
        const I32 q = step ? span / step : 0;
        if (span > 0 && (q != 0 || !(step / 2 > span))) {
            y = self->extent.top + b; h = span < 0 ? 0 : span;
        } else {
            y = height / 2 + self->extent.top; h = 0;
        }
    }
    const I32 inner = params->inner_offset;                               // 00415360 (+18)
    y -= inner;                                                            // 0041536C
    I32 m = inner < gui_load_border_params_dim_00414cd0(params, nullptr)
            ? params->inner_offset : gui_load_border_params_dim_00414cd0(params, nullptr);   // 0041536E..85
    const I32 inner2 = params->inner_offset;                               // 00415385
    x -= m;                                                                // 0041538E
    m = inner2 < gui_load_border_params_dim_00414cd0(params, nullptr)
            ? params->inner_offset : gui_load_border_params_dim_00414cd0(params, nullptr);   // 00415390..A7
    out->left = x;
    out->top = y;
    out->width = w + m * 2;                                                // 004153A7..AB
    out->height = h + params->inner_offset * 2;                            // 004153AE..B5
    return out;
}

// 00415190 CSWGuiBorder::Initialize(extent, params) RET 8: copy extent to +04
// (00415190..AB) then BorderParams::operator=(+14, params) (004151B6).
void __fastcall gui_load_border_initialize_00415190(Border* self, void*, const Extent* extent, const void* params) {
    self->extent = *extent;
    gui_load_border_params_assign_004150c0(&border_view(self)->params, nullptr,
                                           static_cast<const BorderParams*>(params));
}

// ------------------------------------------------------------------ TextParams
// 00417050 ctor: CExoString (00417070); CResRef default (0041707E); +1C..+24 and
// +2C..+34 zeroed then each pair set to 1.0 (00417083..DA); flags (x & 0xFFFFFF89)
// | 9 (004170A7..CB, stored 004170EE); +50 = 0; +08 = -1; +28 = 1.0; temporary
// CResRef "dialogfont16x16" (004170CE..F1) -> SetBaseFont (004170FD).
// EH state 0 covers the CExoString (unwind 005E5C20) if a later call throws.
TextParams* __fastcall gui_load_text_params_construct_00417050(TextParams* self, void*) noexcept(false) {
    new (self->text) String();
    try {
        c2da_default_ref_00405ed0(&self->font, nullptr);
        for (int i = 0; i < 3; ++i) { self->color[i] = 0.0f; self->default_color[i] = 0.0f; }
        const Word flags = (self->flags & 0xFFFFFF89u) | 9u;
        self->default_color[0] = 1.0f; self->color[0] = 1.0f;
        self->default_color[1] = 1.0f; self->color[1] = 1.0f;
        self->default_color[2] = 1.0f; self->color[2] = 1.0f;
        self->text_object = nullptr;
        self->strref = 0xFFFFFFFFu;
        self->f28 = 1.0f;
        self->flags = flags;
        Ref base;
        base.construct_00406d80("dialogfont16x16");                       // 0073E3DC
        gui_load_text_params_set_base_font_00415dd0(self, nullptr, &base);
    } catch (...) {
        as_string(self->text)->~String();
        throw;
    }
    return self;
}

// 00415DD0 SetBaseFont(ref) RET 4: CopyToString(ref -> +3C) (00415DDB); SetFontRename (00415DE2).
void __fastcall gui_load_text_params_set_base_font_00415dd0(void* params, void*, const Ref* font) noexcept(false) {
    TextParams* self = static_cast<TextParams*>(params);
    gui_load_resref_copy_to_string_00405fb0(font, nullptr, self->base_font);
    gui_load_text_params_set_font_rename_00415d60(self, nullptr);
}

// 00415D60 SetFontRename: byte copy +3C through NUL into a 20-byte local
// (00415D66..78); GetUpdatedFontName(gui manager, local) (00415D7A..85);
// CopyToString(font) (00415D92); __stricmp (00415DA1) nonzero -> CResRef(local)
// (00415DB6) -> SetFont (00415DC2).
void __fastcall gui_load_text_params_set_font_rename_00415d60(TextParams* self, void*) noexcept(false) {
    char name[20];
    for (int i = 0;; ++i) { name[i] = self->base_font[i]; if (!name[i]) break; }
    gui_load_manager_updated_font_name_0040b360(kotor_input_cells::gui_manager_007a39f4, nullptr, name);
    char current[17];
    gui_load_resref_copy_to_string_00405fb0(&self->font, nullptr, current);
    if (_stricmp(name, current) != 0) {
        Ref renamed;
        renamed.construct_00406d80(name);
        gui_load_text_params_set_font_00415d00(self, nullptr, &renamed);
    }
}

// 0040B360 CSWGuiManager::GetUpdatedFontName(name) RET 4 (ECX unused):
// first 12 bytes equal "fnt_console\0" -> unchanged (0040B369..77); len = strlen
// (0040B37D..92), only its low byte used as index (BL); options byte +80 from
// CClientExoApp::GetClientOptions (0040B389..9C): 1 -> 'a', 2 -> 'b', else
// width 0078D1D4 < 0x500 -> 'a', < 0x9C4 -> 'b', else 'b' (signed JGE); writes
// suffix then NUL at name[len&0xFF] (0040B3B5..C1 etc.).
void __fastcall gui_load_manager_updated_font_name_0040b360(void*, void*, char* name) {
    static const char console[12] = { 'f','n','t','_','c','o','n','s','o','l','e','\0' };   // 0073E004
    int i = 0;
    while (i < 12 && name[i] == console[i]) ++i;
    if (i == 12) return;
    std::uint32_t length = 0;
    while (name[length]) ++length;
    const unsigned index = length & 0xFFu;
    void* app_manager = kotor_menu_window_cells::manager_007a39fc;
    kih::ClientApp* client = static_cast<kih::AppManager*>(app_manager)->client;
    const unsigned char* options = reinterpret_cast<const unsigned char*>(client->GetClientOptions());
    const unsigned mode = options[0x80];
    char suffix;
    if (mode == 1) suffix = 'a';
    else if (mode == 2) suffix = 'b';
    else {
        const std::int32_t width = static_cast<std::int32_t>(kotor_renderer_gl_cells::width_0078d1d4);
        suffix = width < 0x500 ? 'a' : 'b';
    }
    name[index] = suffix;
    name[index + 1] = 0;
}

// 00415D00 SetFont(ref) RET 4: assign +0C (00415D16); text object and its string
// present and IsValid(+0C) (00415D1B..32) -> CopyToString (00415D3B) -> string
// vslot +44(name) (00415D4D) -> wrapText(re-read text object) (00415D50..53).
void __fastcall gui_load_text_params_set_font_00415d00(TextParams* self, void*, const Ref* font) {
    assign_ref(&self->font, font);
    TextObject* object = self->text_object;
    if (!object || !object->string) return;
    if (!gui_load_resref_is_valid_00406030(&self->font, nullptr)) return;
    char name[20];
    gui_load_resref_copy_to_string_00405fb0(&self->font, nullptr, name);
    AurString* string = self->text_object->string;
    string->vtbl->set_font(string, nullptr, name);
    gui_load_text_wrap_00414e90(self->text_object, nullptr);
}

// 00415E00 SetText(CExoString const&) RET 4: operator= (00415E08); +08 = -1
// after the assignment (00415E12); push_text tail.
void __fastcall gui_load_text_params_set_text_00415e00(void* params, void*, const String* text) noexcept(false) {
    TextParams* self = static_cast<TextParams*>(params);
    as_string(self->text)->Assign005e5c50(const_cast<String*>(text));
    self->strref = 0xFFFFFFFFu;
    push_text(self);
}

// 00414E90 CSWGuiText::wrapText: string, +10 (height) and +0C (width) nonzero ->
// string vslot +4C(-1) (00414E92..AB).
void __fastcall gui_load_text_wrap_00414e90(TextObject* self, void*) {
    AurString* string = self->string;
    if (!string || !self->extent.height || !self->extent.width) return;
    string->vtbl->wrap(string, nullptr, -1);
}

// 00415F20 SetAlignment(a) RET 4: flags low 6 bits replaced (00415F27..39);
// owner string present -> vslot +1C(flags byte & 0x3F) (00415F45..52) -> wrapText.
void __fastcall gui_load_text_params_set_alignment_00415f20(void* params, void*, std::int32_t alignment) {
    TextParams* self = static_cast<TextParams*>(params);
    self->flags = self->flags ^ ((self->flags ^ static_cast<Word>(alignment)) & 0x3Fu);
    TextObject* object = self->text_object;
    if (!object || !object->string) return;
    AurString* string = object->string;
    string->vtbl->set_alignment(string, nullptr, (self->flags & 0xFFu) & 0xFFFFFF3Fu);
    gui_load_text_wrap_00414e90(self->text_object, nullptr);
}

// 00415E50 SetStrRef(strref) RET 4: +08 = strref (00415E72); -1 -> done;
// STR_RES local {CExoString (00415E7F), CResRef (00415E90)}; CTlkTable::Fetch(
// strref, &str_res, byte 0073ECD0) on [007A3A08] (00415E95..B4); operator=
// (00415EC0); push_text tail; CExoString destroyed (00415EFC).
void __fastcall gui_load_text_params_set_strref_00415e50(void* params, void*, Word strref) noexcept(false) {
    TextParams* self = static_cast<TextParams*>(params);
    self->strref = strref;
    if (strref == 0xFFFFFFFFu) return;
    struct StrRes { unsigned char text[8]; Ref sound; unsigned char tail[4]; } res;  // 0x1C frame: [esp+4] / [esp+0C] / [esp+1C]
    new (res.text) String();
    try {
        c2da_default_ref_00405ed0(&res.sound, nullptr);
        static const unsigned char rdata_0073ecd0 = 0x00;                  // .rdata byte 0073ECD0 (image value 00)
        tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, self->strref, &res, rdata_0073ecd0);
        as_string(self->text)->Assign005e5c50(as_string(res.text));
        push_text(self);
    } catch (...) {
        as_string(res.text)->~String();
        throw;
    }
    as_string(res.text)->~String();
}

// 00414E50 SetDefaultColor(color) RET 4: copy to +2C (00414E56..69) then tail
// jump SetColor 00414E10 (lane-input ColorReceiver::SetColor) with the same arg.
void __fastcall gui_load_text_params_set_default_color_00414e50(void* params, void*, const Vector* color) {
    TextParams* self = static_cast<TextParams*>(params);
    self->default_color[0] = color->x; self->default_color[1] = color->y; self->default_color[2] = color->z;
    reinterpret_cast<ColorReceiver*>(self)->SetColor(reinterpret_cast<const MenuColor*>(color));
}

// 00415F70 SetTextObject(text) RET 4: +50 = text (00415F7C); null -> done; no
// string yet -> CopyToString(font) + NewCAurGUIString(CStr(+0), name, &+1C, 1.0)
// stored at (re-read +50)+14 (00415F88..B6); strref saved (00415FBA);
// SetFontRename (00415FBF); SetText(own string) (00415FC7); SetAlignment(flags &
// 0x3F) (00415FD5); SetColor(&+1C) (00415FE0); strref restored (00415FE5).
void __fastcall gui_load_text_params_set_text_object_00415f70(void* params, void*, Text* text) noexcept(false) {
    TextParams* self = static_cast<TextParams*>(params);
    self->text_object = text_view(text);
    if (!text) return;
    if (!text_view(text)->string) {
        char name[20];
        gui_load_resref_copy_to_string_00405fb0(&self->font, nullptr, name);
        AurString* created = gui_load_new_aur_gui_string_0045bdf0(
            WindowCStr005e5670(as_string(self->text), nullptr), name, self->color, 1.0f);
        self->text_object->string = created;
    }
    const Word saved = self->strref;
    gui_load_text_params_set_font_rename_00415d60(self, nullptr);
    gui_load_text_params_set_text_00415e00(self, nullptr, as_string(self->text));
    gui_load_text_params_set_alignment_00415f20(self, nullptr, static_cast<std::int32_t>(self->flags & 0x3Fu));
    reinterpret_cast<ColorReceiver*>(self)->SetColor(reinterpret_cast<const MenuColor*>(self->color));
    self->strref = saved;
}

// 00416280 CSWGuiText::SetExtent(extent) RET 4: copy to +04 (0041628A..A0);
// string present -> vslot +64(left, top) (004162AA..B3), vslot +74(width,
// height) on the re-read string (004162B6..C3), wrapText (004162C8).
void __fastcall gui_load_text_set_extent_00416280(Text* text, void*, const Extent* extent) {
    TextObject* self = text_view(text);
    self->extent = *extent;
    if (!self->string) return;
    self->string->vtbl->set_position(self->string, nullptr, extent->left, extent->top);
    self->string->vtbl->set_size(self->string, nullptr, extent->width, extent->height);
    gui_load_text_wrap_00414e90(self, nullptr);
}

} // extern "C"
