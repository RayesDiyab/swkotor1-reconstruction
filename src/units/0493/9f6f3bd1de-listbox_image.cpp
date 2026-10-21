// Lane gui-load / ListBox fork: CSWGuiImage (0x54, vtable 0073E390) and
// CSWGuiImageParams (0x2C), reached from CSWGuiScrollBar::CSWGuiScrollBar 0041AFE0
// (thumb +E0 / dir +134) and CSWGuiScrollBar::Load 0041BCD0 ("THUMB"/"DIR").
#include "4d74549b1b-listbox.h"

extern "C" char* __fastcall WindowCStr005e5670(gui_load::String*, void*);   // M40 window-native-strings.obj

using namespace gui_lb;

// 0073E390: 5 slots (Ghidra DEFINED_DATA run 0073E390..0073E3A0).
struct GuiLoadImageMethods {
    void* (__fastcall* deleting_destroy)(Image*, void*, Word);                  // +00 00416E70
    void (__fastcall* set_extent)(Image*, void*, const Extent*);                // +04 00415B70
    Word (__fastcall* property10)(Object*, void*);                              // +08 004064A0 (main lane, D24)
    void (__fastcall* draw)(Image*, void*, Word);                               // +0C 00415C90 (MISSING)
    void (__fastcall* load)(Image*, void*, Gff*, Struct*, String*);             // +10 00416EC0
};
static_assert(sizeof(GuiLoadImageMethods) == 0x14, "image vtable");

// CSWGuiDrawable table 0073E324: consumer name already used (UNDEF) by lane-3d-render scene3d.obj; owner = main lane (D21 table).
namespace kotor_scene3d { extern void* const gui_drawable_vtable_0073e324[]; }

namespace {
using Ref = menu_resources::Ref;
inline Word bits(float f) { Word w; std::memcpy(&w, &f, 4); return w; }
// FLD dword / FSTP dword (00416E18..1D, 00416E2F..32): x87 round trip, not a mov.
inline void x87_copy(Word* to, const Word* from) {
    __asm {
        mov eax, from
        mov edx, to
        fld dword ptr [eax]
        fstp dword ptr [edx]
    }
}
}

extern "C" {

// 00406DC0 CResRef::operator!=(CResRef const&): REPE CMPSD over 4 dwords, SETNE (RET 4).
Word __fastcall gui_load_resref_not_equal_00406dc0(const Ref* self, void*, const Ref* other) {
    const Word* a = reinterpret_cast<const Word*>(other->bytes);
    const Word* b = reinterpret_cast<const Word*>(self->bytes);
    for (int i = 0; i < 4; ++i)
        if (a[i] != b[i]) return 1;
    return 0;
}

// 00406030 CResRef::IsValid: first byte != 0 (SETNE, RET 0).
Word __fastcall gui_load_resref_is_valid_00406030(const Ref* self, void*) {
    return self->bytes[0] != 0 ? 1u : 0u;
}

// 00414D30 CSWGuiImageParams::CSWGuiImageParams(CResRef, int, int, float, int, float, Vector const&) RET 1Ch.
// Own CResRef default ctor (00414D37); +18/+1C/+20 = 0 (00414D4A..50); assign arg1
// (00414D53); +10 = arg4 (00414D64); +14 = arg6 (00414D6E); flags = (old & FFFFF000) |
// ((((arg5&3F)<<4 | (arg3&F))<<1 | (arg2&1))<<1) (00414D6B..95); color = *arg7
// (00414D98..A9, +28 = 0 at 00414DA6 between the +1C and +20 copies).
ImageParams* __fastcall gui_load_image_params_construct_00414d30(ImageParams* self, void*, Ref* image,
    Word a2, Word a3, Word a4, Word a5, Word a6, const Vector* color) noexcept(false) {
    c2da_default_ref_00405ed0(&self->image, nullptr);
    std::memset(&self->color, 0, sizeof(self->color));
    Ref temporary;
    c2da_copy_00406120(&self->image, nullptr, &temporary, image);
    self->u10 = a4;
    self->u14 = a6;
    Word packed = (((a5 & 0x3Fu) << 4) | (a3 & 0xFu));
    packed = ((packed << 1) | (a2 & 1u)) << 1;
    self->flags = (self->flags & 0xFFFFF000u) | packed;
    const Word* from = reinterpret_cast<const Word*>(color);
    Word* to = reinterpret_cast<Word*>(&self->color);
    to[0] = from[0];
    to[1] = from[1];
    self->owner = nullptr;
    to[2] = from[2];
    return self;
}

// 00415900 CSWGuiImageParams::SetImage(CResRef const&, int force) RET 8:
// skip if !(ref != image) && !force (0041590E..1D); assign (00415927); owner null ->
// return (0041592F); release old CAurGUIImage via vslot +04(1) and clear (0041593A..44);
// if image valid (0041594D): CopyToString (0041595D), NewCAurGUIImage (00415967) ->
// owner+24 (0041596F); if non-null: owner vslot +04 SetExtent(&owner->extent) (00415985).
void __fastcall gui_load_image_params_set_image_00415900(ImageParams* self, void*, Ref* image, Word force) noexcept(false) {
    if (!gui_load_resref_not_equal_00406dc0(image, nullptr, &self->image) && !force) return;
    Ref temporary;
    c2da_copy_00406120(&self->image, nullptr, &temporary, image);
    if (!self->owner) return;
    if (AurObject* old = self->owner->gui_image) {
        old->vtbl->release(old, nullptr, 1);
        self->owner->gui_image = nullptr;
    }
    if (!gui_load_resref_is_valid_00406030(&self->image, nullptr)) return;
    char name[17];
    gui_load_resref_copy_to_string_00405fb0(&self->image, nullptr, name);
    self->owner->gui_image = kotor_cdecl_0045a260(name);
    Image* owner = self->owner;
    if (!owner->gui_image) return;
    vt<ImageCallVtbl>(owner)->set_extent(owner, nullptr, &owner->extent);
}

// 00415990 SetAlignment(int): flags ^= ((a<<6) ^ flags) & FC0 (00415997..A3); if owner,
// tail-jump owner vslot +04(&owner->extent) (004159A6..B6).
void __fastcall gui_load_image_params_set_alignment_00415990(ImageParams* self, void*, Word alignment) {
    self->flags = self->flags ^ (((alignment << 6) ^ self->flags) & 0xFC0u);
    if (Image* owner = self->owner) vt<ImageCallVtbl>(owner)->set_extent(owner, nullptr, &owner->extent);
}

// 004159C0 SetDrawStyle(int): flags ^= ((d*2) ^ flags) & 2 (004159C4..D1); owner tail call as above.
void __fastcall gui_load_image_params_set_draw_style_004159c0(ImageParams* self, void*, Word style) {
    self->flags = self->flags ^ (((style + style) ^ self->flags) & 2u);
    if (Image* owner = self->owner) vt<ImageCallVtbl>(owner)->set_extent(owner, nullptr, &owner->extent);
}

// 004159F0 SetImageObject(CSWGuiImage*): owner = arg (004159F9); if non-null: forced
// SetImage(own image) (00415A01), SetAlignment((flags>>6)&3F) (00415A12), SetDrawStyle((flags>>1)&1) (00415A22).
void __fastcall gui_load_image_params_set_image_object_004159f0(ImageParams* self, void*, Image* owner) noexcept(false) {
    self->owner = owner;
    if (!owner) return;
    gui_load_image_params_set_image_00415900(self, nullptr, &self->image, 1);
    gui_load_image_params_set_alignment_00415990(self, nullptr, (self->flags >> 6) & 0x3Fu);
    gui_load_image_params_set_draw_style_004159c0(self, nullptr, (self->flags >> 1) & 1u);
}

// 00416DE0 CSWGuiImageParams::operator=(CSWGuiImageParams const&) RET 4: ref assign (00416DF1);
// flags bit1, bits2..5 from src (00416DF6..15); u10 FLD/FSTP (00416E18); bits6..11 (00416E20..2C);
// u14 FLD/FSTP (00416E2F); bit0 (00416E35..40); color 3 dwords (00416E43..5C);
// SetImageObject(current owner) (00416E5F). Returns this.
ImageParams* __fastcall gui_load_image_params_assign_00416de0(ImageParams* self, void*, ImageParams* source) noexcept(false) {
    Ref temporary;
    c2da_copy_00406120(&self->image, nullptr, &temporary, &source->image);
    self->flags = self->flags ^ ((self->flags ^ source->flags) & 2u);
    self->flags = self->flags ^ ((source->flags ^ self->flags) & 0x3Cu);
    x87_copy(&self->u10, &source->u10);
    self->flags = self->flags ^ ((self->flags ^ source->flags) & 0xFC0u);
    x87_copy(&self->u14, &source->u14);
    self->flags = self->flags ^ ((self->flags ^ source->flags) & 1u);
    const Word* from = reinterpret_cast<const Word*>(&source->color);
    Word* to = reinterpret_cast<Word*>(&self->color);
    to[0] = from[0];
    to[1] = from[1];
    to[2] = from[2];
    gui_load_image_params_set_image_object_004159f0(self, nullptr, self->owner);
    return self;
}

// 00415A30 CSWGuiImage::CSWGuiImage: extent +10/+0C/+08/+04 = 0 (00415A53..5C); vptr 73E390
// (00415A6C); EH state 0 (00415A78); draw +20/+1C/+18/+14 = 0 (00415A7C..85); local color
// (1,1,1) (00415A88..98); CResRef default (00415AA3) -> ImageParams(ref,0,0,0,12h,1.0,&color)
// (00415AAB); +24 = 0 (00415AB3); SetImageObject(this) (00415AB6).
// Unwind state 0 (FuncInfo 007611B4): CSWGuiDrawable::~CSWGuiDrawable 00414F80.
Image* __fastcall gui_load_image_construct_00415a30(Image* self, void*) noexcept(false) {
    std::memset(&self->extent, 0, sizeof(self->extent));
    set_vptr(self, &gui_load_image_methods_0073e390);
    try {
        std::memset(&self->draw, 0, sizeof(self->draw));
        const Vector white = { 1.0f, 1.0f, 1.0f };
        Ref none;
        c2da_default_ref_00405ed0(&none, nullptr);
        gui_load_image_params_construct_00414d30(&self->params, nullptr, &none, 0, 0, 0, 0x12, bits(1.0f), &white);
        self->gui_image = nullptr;
        gui_load_image_params_set_image_object_004159f0(&self->params, nullptr, self);
    } catch (...) {
        gui_drawable_destruct_00414f80(self, nullptr);
        throw;
    }
    return self;
}

// 00415AD0 CSWGuiImage::~CSWGuiImage: vptr 73E390 (00415AED); gui image vslot +04(1),
// clear (00415AF3..09); vptr 73E324 (00415B14).
void __fastcall gui_load_image_destruct_00415ad0(Image* self, void*) {
    set_vptr(self, &gui_load_image_methods_0073e390);
    if (AurObject* gui = self->gui_image) {
        gui->vtbl->release(gui, nullptr, 1);
        self->gui_image = nullptr;
    }
    set_vptr(self, static_cast<const void*>(kotor_scene3d::gui_drawable_vtable_0073e324));
}

// 00416E70 scalar deleting destructor: real dtor (00416E73); if flags&1 free (00416E80); EAX=this.
void* __fastcall gui_load_image_deleting_destruct_00416e70(Image* self, void*, Word flags) {
    gui_load_image_destruct_00415ad0(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// 00415B30 GetImageExtent(out) RET 4: zeros; if gui: width=vslot +10 (00415B47), height=vslot +0C
// (00415B51); out = {0,0,width,height}; EAX=out.
Extent* __fastcall gui_load_image_get_extent_00415b30(Image* self, void*, Extent* out) {
    I32 width = 0, height = 0;
    if (AurObject* gui = self->gui_image) {
        width = gui->vtbl->width(gui, nullptr);
        AurObject* again = self->gui_image;
        height = again->vtbl->height(again, nullptr);
    }
    out->left = 0;
    out->top = 0;
    out->width = width;
    out->height = height;
    return out;
}

// 00415B70 CSWGuiImage::SetExtent(CSWGuiExtent const&) RET 4 (see per-branch cites).
void __fastcall gui_load_image_set_extent_00415b70(Image* self, void*, const Extent* extent) {
    self->extent = *extent;                                     // 00415B79..93
    AurObject* gui = self->gui_image;
    if (!gui) return;                                           // 00415B9B
    const Word style = (self->params.flags >> 1) & 1u;          // 00415BA1..A6
    if (style == 0) {                                           // 00415C6C: draw = argument extent
        self->draw = *extent;
        return;
    }
    self->draw.height = gui->vtbl->height(gui, nullptr);        // 00415BBB/C1
    AurObject* again = self->gui_image;
    const I32 w = again->vtbl->width(again, nullptr);           // 00415BC6
    self->draw.width = w;                                       // 00415BD0
    if (w >= self->extent.width) {                              // 00415BCE/D3
        self->draw.width = self->extent.width;                  // 00415BD7
        self->draw.left = self->extent.left;                    // 00415BDA
    } else {
        const Word horizontal = (self->params.flags >> 6) & 7u; // 00415BDF..E5
        if (horizontal == 1) self->draw.left = self->extent.left;                                      // 00415C0E
        else if (horizontal == 2) self->draw.left = (self->extent.width - w) / 2 + self->extent.left;  // 00415C01..10
        else if (horizontal == 4) self->draw.left = self->extent.width - w + self->extent.left;        // 00415BF3..FC
    }
    const I32 h = self->draw.height;                            // 00415C13
    if (h >= self->extent.height) {                             // 00415C19
        self->draw.height = self->extent.height;                // 00415C1D
        self->draw.top = self->extent.top;                      // 00415C20..24
        return;
    }
    const Word vertical = (self->params.flags >> 6) & 0x38u;    // 00415C2C..32
    if (vertical == 8) self->draw.top = self->extent.top;                                           // 00415C38 -> 00415C20
    else if (vertical == 0x10) self->draw.top = (self->extent.height - h) / 2 + self->extent.top;   // 00415C57..64
    else if (vertical == 0x20) self->draw.top = self->extent.top + self->extent.height - h;         // 00415C44..4F
}

// 00416E90 CSWGuiImage::Initialize(extent, params) RET 8: extent copy (00416E95..AB);
// ImageParams::operator= (00416EB6).
void __fastcall gui_load_image_initialize_00416e90(Image* self, void*, const Extent* extent, ImageParams* params) noexcept(false) {
    self->extent = *extent;
    gui_load_image_params_assign_00416de0(&self->params, nullptr, params);
}

// 00416EC0 CSWGuiImage::Load(gff, struct, CExoString const& label) RET 0Ch:
// GetStructFromStruct(label) (00416EE1) else return. IMAGE resref default "" -> SetImage(ref,0)
// (00416EEF..25); DRAWSTYLE int 0 -> SetDrawStyle (00416F3D/45); FLIPSTYLE int 0 -> flags bits2..5
// (00416F5D..7A); ALIGNMENT int 12h -> SetAlignment (00416F89/91); ROTATE float 0 (00416FA9):
// on success store it at +38 (00417029); else ROTATESTYLE int 0 (00416FD0): 0/1/2/3 ->
// 0/90/180/270 (jump table 00417034; 0073D700/0073E40C/0073E408/0073E404), other -> the
// (default) ROTATE value (00417025). Target +38 = params.u10.
void __fastcall gui_load_image_load_00416ec0(Image* self, void*, Gff* gff, Struct* s, String* label) noexcept(false) {
    const char* text = WindowCStr005e5670(label, nullptr);
    Struct sub;
    if (!gui_load_gff_struct_00411a10(gff, nullptr, &sub, s, text)) return;
    ImageParams* params = &self->params;
    Word success;
    {
        Ref none;
        none.construct_00406d80("");
        Ref value;
        Ref* read = gui_load_gff_resref_00411e10(gff, nullptr, &value, &sub, "IMAGE", &success, &none);
        gui_load_image_params_set_image_00415900(params, nullptr, read, 0);
    }
    gui_load_image_params_set_draw_style_004159c0(params, nullptr,
        static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &sub, "DRAWSTYLE", &success, 0)));
    const Word flip = static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &sub, "FLIPSTYLE", &success, 0));
    params->flags = params->flags ^ (((flip * 4u) ^ params->flags) & 0x3Cu);
    gui_load_image_params_set_alignment_00415990(params, nullptr,
        static_cast<Word>(gui_load_gff_int_00411c90(gff, nullptr, &sub, "ALIGNMENT", &success, 0x12)));
    const float rotate = gui_load_gff_float_00411d00(gff, nullptr, &sub, "ROTATE", &success, 0.0f);
    float stored = rotate;
    if (!success) {
        const I32 style = gui_load_gff_int_00411c90(gff, nullptr, &sub, "ROTATESTYLE", &success, 0);
        switch (static_cast<Word>(style)) {
            case 0: stored = 0.0f; break;
            case 1: stored = 90.0f; break;
            case 2: stored = 180.0f; break;
            case 3: stored = 270.0f; break;
            default: stored = rotate; break;
        }
    }
    std::memcpy(&params->u10, &stored, 4);
}

// 0073E390 method table.
const GuiLoadImageMethods gui_load_image_methods_0073e390 = {
    &gui_load_image_deleting_destruct_00416e70,
    &gui_load_image_set_extent_00415b70,
    &gui_load_slot_prop10_004064a0,
    &gui_load_image_draw_00415c90,
    &gui_load_image_load_00416ec0,
};

} // extern "C"
