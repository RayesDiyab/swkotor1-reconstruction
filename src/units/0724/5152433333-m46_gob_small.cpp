// lane-m46-3dcore fork B: Gob vtable 00741268 slot bodies - flags, accessors and small members.
// Evidence: dumpbin /disasm of swkotor.exe (GOG K1, image base 00400000); instruction addresses cited per behaviour.
// ABI: original __thiscall members are the D1 __fastcall(Gob*, void* unused_edx, stack words...) declared in the
// canonical vtables_gen.h (raw-word contract: every stack DWORD is U; floats travel as raw bits).
// Return channel: members whose original return type is void leave EAX with an incidental value that no caller reads;
// under the U raw-word contract they return the value EAX holds at the original `ret` when that value is determined by
// the body (cited), otherwise 0 (marked "EAX incidental").
#include "fb4ea0259f-m46_gob_externs.h"

using namespace m46_gob;
using aurora_model::Gob;

namespace {
constexpr char kPropagateViewVolume_00740a98[] = "propagateviewvolume";     // .rdata 00740A98
constexpr char kPropagateColorShift_00740aac[] = "propagatecolorshift";     // .rdata 00740AAC
constexpr char kPropagateIllumination_00740ac0[] = "propagateillumination"; // .rdata 00740AC0
constexpr char kCutsceneDummy_00740c8c[] = "cutscenedummy";                 // .rdata 00740C8C
inline void copy3(void* dst, const void* src) {        // three DWORD moves (no FPU load/store canonicalisation)
    field<U>(dst, 0) = field<U>(const_cast<void*>(src), 0);
    field<U>(dst, 4) = field<U>(const_cast<void*>(src), 4);
    field<U>(dst, 8) = field<U>(const_cast<void*>(src), 8);
}
inline void copy4(void* dst, const void* src) {
    copy3(dst, src);
    field<U>(dst, 0xC) = field<U>(const_cast<void*>(src), 0xC);
}
}

extern "C" {
// ---------------------------------------------------------------- byte / word flags
U __fastcall gob_enable_force_animations_0043e830(Gob* self, void*) {   // +DC
    field<U8>(self, 0x10) = 1;                                          // 0043E830 mov byte [ecx+10h],1
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_disable_force_animations_0043e840(Gob* self, void*) {  // +E0
    field<U8>(self, 0x10) = 0;                                          // 0043E840
    return 0;                                                           // EAX incidental (void)
}
U8 __fastcall gob_is_force_animated_0043e850(Gob* self, void*) {       // +170
    return field<U8>(self, 0x10);                                       // 0043E850 mov al,[ecx+10h]
}
U __fastcall gob_enable_always_render_0043e860(Gob* self, void*) {      // +130
    field<U8>(self, 0x12) = 1;                                          // 0043E860
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_disable_always_render_0043e870(Gob* self, void*) {     // +134
    field<U8>(self, 0x12) = 0;                                          // 0043E870
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_enable_in_cutscene_0043e880(Gob* self, void*) {        // +138
    field<U8>(self, 0x14) = 1;                                          // 0043E880
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_disable_in_cutscene_0043e890(Gob* self, void*) {       // +13C
    field<U8>(self, 0x14) = 0;                                          // 0043E890
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_enable_in_view_volume_0043e900(Gob* self, void*) {     // +178
    field<U8>(self, 0x11) = 1;                                          // 0043E909
    // 0043E900..0043E90D: this->DoEvent("propagateviewvolume", 0) through vtbl+16C; its EAX is returned (plain ret).
    return gob_vt(self)->do_event(self, nullptr, kPropagateViewVolume_00740a98, 0);
}
U __fastcall gob_disable_in_view_volume_0043e920(Gob* self, void*) {    // +17C
    field<U8>(self, 0x11) = 0;                                          // 0043E920
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_disable_render_gob_bbox_0043ee30(Gob* self, void*) {   // +D4
    field<U16>(self, 0xC8) = 0;                                         // 0043EE30 mov word [ecx+C8h],0
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_turn_off_shadows_00449a50(Gob* self, void*) {          // +BC
    field<U16>(self, 0xC2) = 0;                                         // 00449A50
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_turn_on_shadows_00449a60(Gob* self, void*) {           // +C0
    field<U16>(self, 0xC2) = 1;                                         // 00449A60
    return 0;                                                           // EAX incidental (void)
}
U8 __fastcall gob_is_animating_00449a70(Gob* self, void*) {            // +174
    return field<U8>(self, 0x17E);                                      // 00449A70
}
U8 __fastcall gob_is_in_view_volume_00449a80(Gob* self, void*) {       // +180
    return field<U8>(self, 0x11);                                       // 00449A80
}
U __fastcall gob_set_as_character_00449a90(Gob* self, void*) {         // +100
    const U model = field<U>(self, 0x58);                               // 00449A90 eax=[ecx+58h]
    if (model != 0) field<U8>(ptr<void>(model), 0x50) |= 4;             // 00449A97 or byte [eax+50h],4
    return model;                                                       // EAX = [ecx+58h]
}
U __fastcall gob_set_render_personal_space_00449aa0(Gob* self, void*, U value) {   // +114
    field<U8>(self, 0x154) = static_cast<U8>(value);                    // 00449AA0..00449AA4 (low byte of the word)
    return 0;                                                           // EAX incidental (AL = arg, upper bits stale)
}
U __fastcall gob_set_motion_blurred_00449ab0(Gob* self, void*, U value) {   // +144
    field<U8>(self, 0x155) = static_cast<U8>(value);                    // 00449AB0..00449AB4
    return 0;                                                           // EAX incidental
}
U8 __fastcall gob_get_motion_blurred_00449ac0(Gob* self, void*) {      // +148
    return field<U8>(self, 0x155);                                      // 00449AC0
}
U __fastcall gob_set_max_blur_length_00449ad0(Gob* self, void*, U length_bits) {   // +158
    field<U>(self, 0x180) = length_bits;                                // 00449AD0..00449AD4 (raw DWORD)
    return length_bits;                                                 // EAX = arg
}
double __fastcall gob_get_max_blur_length_00449ae0(Gob* self, void*) { // +15C
    return W(field<float>(self, 0x180));                                // 00449AE0 fld dword [ecx+180h] (ST0)
}
U __fastcall gob_enable_distortion_00449af0(Gob* self, void*) {        // +F8
    field<U8>(self, 0x17C) = 1;                                         // 00449AF0
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_disable_distortion_00449b00(Gob* self, void*) {       // +FC
    field<U8>(self, 0x17C) = 0;                                         // 00449B00
    return 0;                                                           // EAX incidental (void)
}
U __fastcall gob_set_ignore_hit_check_00449a40(Gob* self, void*, U value) {   // +90
    field<U8>(self, 0x17D) = static_cast<U8>(value);                    // 00449A40..00449A44
    return 0;                                                           // EAX incidental
}

// ---------------------------------------------------------------- external data / names
U __fastcall gob_get_external_data_00449a00(Gob* self, void*) { return field<U>(self, 0x1AC); }          // 00449A00
U __fastcall gob_set_external_data_00449a10(Gob* self, void*, U value) {                                  // 00449A10
    field<U>(self, 0x1AC) = value;
    return value;                                                       // EAX = arg
}
U __fastcall gob_get_external_data_type_00449a20(Gob* self, void*) { return field<U>(self, 0x1B0); }     // 00449A20
U __fastcall gob_set_external_data_type_00449a30(Gob* self, void*, U value) {                             // 00449A30
    field<U>(self, 0x1B0) = value;
    return value;                                                       // EAX = arg
}
U __fastcall gob_inherit_external_0043f250(Gob* self, void*, U other) {    // +80
    field<U>(self, 0x1AC) = field<U>(ptr<void>(other), 0x1AC);           // 0043F254..0043F25A
    const U type = field<U>(ptr<void>(other), 0x1B0);                   // 0043F260
    field<U>(self, 0x1B0) = type;                                       // 0043F266
    return type;                                                        // EAX = other+1B0
}
U __fastcall gob_get_name_00449970(Gob* self, void*) {                 // +08
    return word(kotor_scene3d::at(self, 0x38));                                        // 00449970 lea eax,[ecx+38h] (CResRef)
}
U __fastcall gob_get_model_name_0043f1c0(Gob* self, void*) {           // +0C
    const U model = field<U>(self, 0x58);                               // 0043F1C0
    if (model == 0) return 0;                                           // 0043F1C3..0043F1C7
    return model + 8;                                                   // 0043F1C8 add eax,8 (MaxTree name)
}

// ---------------------------------------------------------------- transforms
U __fastcall gob_get_position_00449980(Gob* self, void*, U out) {      // +64
    copy3(ptr<void>(out), kotor_scene3d::at(self, 0x78));                              // 00449980..00449997
    return out;                                                         // EAX = out
}
U __fastcall gob_get_orientation_004499a0(Gob* self, void*, U out) {   // +68
    copy4(ptr<void>(out), kotor_scene3d::at(self, 0x84));                              // 004499A0..004499C0
    return out;
}
U __fastcall gob_get_previous_position_004499d0(Gob* self, void*, U out) {   // +6C
    copy3(ptr<void>(out), kotor_scene3d::at(self, 0x94));                              // 004499D0..004499EA
    return out;
}
// Gob::SetOrientation(Quaternion* out, Quaternion q): stores q at +84, copies it to *out, returns out (RET 14h).
U __fastcall gob_set_orientation_0043f0f0(Gob* self, void*, U out, U w, U x, U y, U z) {   // +60
    field<U>(self, 0x84) = w;                                           // 0043F101
    field<U>(self, 0x88) = x;                                           // 0043F103
    field<U>(self, 0x8C) = y;                                           // 0043F10B
    field<U>(self, 0x90) = z;                                           // 0043F113
    void* o = ptr<void>(out);                                           // 0043F116 eax=[esp+10h]
    field<U>(o, 0) = w; field<U>(o, 4) = x; field<U>(o, 8) = y; field<U>(o, 0xC) = z;   // 0043F11C..0043F124
    return out;
}
// Gob::GetBoundingBox(Vector* minimum, Vector* maximum) (RET 8): copies +120 and +12C.
U __fastcall gob_get_bounding_box_0043e8c0(Gob* self, void*, U minimum, U maximum) {   // +EC
    copy3(ptr<void>(minimum), kotor_scene3d::at(self, 0x120));                         // 0043E8C0..0043E8D8
    copy3(ptr<void>(maximum), kotor_scene3d::at(self, 0x12C));                         // 0043E8DB..0043E8F2
    return field<U>(self, 0x130);                                       // EAX = [ecx+12Ch+4] (0043E8E9: last eax load)
}
// Gob::GetMinimumBoundingBox(Vector*, Vector*) (RET 8): refresh through vtbl+184, then copy +120 / +12C.
U __fastcall gob_get_minimum_bounding_box_0043ef80(Gob* self, void*, U minimum, U maximum) {   // +F0
    (void)gob_vt(self)->slot184(self, nullptr);                          // 0043EF83..0043EF85 call [eax+184h]
    copy3(ptr<void>(minimum), kotor_scene3d::at(self, 0x120));                         // 0043EF8B..0043EFA2
    copy3(ptr<void>(maximum), kotor_scene3d::at(self, 0x12C));                         // 0043EFA5..0043EFBC
    return field<U>(self, 0x134);                                       // EAX = [esi+134h] (0043EFB9)
}
// Gob::GetMinimumBoundingSphere(Vector* center, float* radius) (RET 8): refresh through vtbl+188.
U __fastcall gob_get_minimum_bounding_sphere_0043efd0(Gob* self, void*, U center, U radius) {   // +F4
    (void)gob_vt(self)->s188_gob_get_minimum_sphere(self, nullptr);     // 0043EFD3..0043EFD5 call [eax+188h]
    copy3(ptr<void>(center), kotor_scene3d::at(self, 0x13C));                          // 0043EFDB..0043EFF6
    field<U>(ptr<void>(radius), 0) = field<U>(self, 0x138);             // 0043EFF9..0043EFFF
    return radius;                                                      // EAX = [esp+0Ch] (0043EFF2)
}
// Gob::GetMinimumSphere() (plain ret): root part's sphere, offset by the root local position, scaled.
U __fastcall gob_get_minimum_sphere_0043f010(Gob* self, void*) {       // +188
    void* root = field<void*>(self, 0x6C);                              // 0043F016
    if (root == nullptr) return 0;                                      // 0043F01B (EAX incidental)
    float* radius = &field<float>(self, 0x138);                         // 0043F025 lea ebx,[edi+138h]
    float* center = &field<float>(self, 0x13C);                         // 0043F02C lea esi,[edi+13Ch]
    // 0043F021..0043F033: root->GetMinimumSphere(center, radius) (Part vtbl+78, RET 8).
    (void)table<PartTable>(root)->s078(root, nullptr, word(center), word(radius));
    const float* local = &field<float>(field<void*>(self, 0x6C), 8);    // 0043F038..0043F03E [edi+6Ch]+8 (re-read)
    const float cx = S(W(center[0]) + W(local[0]));                     // 0043F036..0043F043 fstp
    const float cy = S(W(center[1]) + W(local[1]));                     // 0043F04B..0043F051
    const float cz = S(W(center[2]) + W(local[2]));                     // 0043F055..0043F064
    center[0] = cx; center[1] = cy; center[2] = cz;                     // 0043F05F / 0043F061 / 0043F074
    const double scale = W(field<float>(self, 0x1B4));                  // 0043F06C fld [edi+1B4h]
    const float sx = S(scale * W(center[0]));                           // 0043F077..0043F079
    const float sy = S(scale * W(center[1]));                           // 0043F083..0043F086
    const float sz = S(scale * W(center[2]));                           // 0043F08E..0043F096
    center[0] = sx; center[1] = sy; center[2] = sz;                     // 0043F091 / 0043F093 / 0043F0A6
    // 0043F09A..0043F0B0: radius = scale * radius * [0078E338] (one wide product chain, one fstp).
    *radius = S(W(field<float>(self, 0x1B4)) * W(*radius) * W(gob_sphere_scale_0078e338));
    return 0;                                                           // EAX incidental (void)
}
// Gob::GetCutsceneDummyPosition(Vector* out) (RET 4).
U __fastcall gob_get_cutscene_dummy_position_004404d0(Gob* self, void*, U out) {   // +140
    aurora_model::Part* dummy = find_part_0043fe00(field<aurora_model::Part*>(self, 0x6C), kCutsceneDummy_00740c8c);   // 004404D0..004404DC
    void* o = ptr<void>(out);
    if (dummy == nullptr) {                                             // 004404E8
        field<U>(o, 0) = 0; field<U>(o, 4) = 0; field<U>(o, 8) = 0;     // 0044053B..00440544
        return out;
    }
    U position[3] = {0, 0, 0};                                          // 004404EC..004404F4
    U orientation[4] = {0x3F800000u, 0, 0, 0};                          // 004404F8..00440510 (w = 1.0f)
    // 00440504..00440518: dummy->GetWorld(&position, &orientation) (Part vtbl+0C, RET 8).
    (void)table<PartTable>(dummy)->s00c(dummy, nullptr, word(position), word(orientation));
    field<U>(o, 0) = position[0]; field<U>(o, 4) = position[1]; field<U>(o, 8) = position[2];   // 0044051B..00440532
    return out;
}
U __fastcall gob_get_part_position_0043f430(Gob* self, void*, U name, U position_out, U orientation_out) {   // +98
    void* part = gob_vt(self)->s10c_gob_get_part(self, nullptr, ptr<const char>(name));   // 0043F430..0043F43A
    if (part == nullptr) return 0;                                      // 0043F442..0043F447 (EAX = 0)
    U position[3]; U orientation[4];
    copy3(position, kotor_scene3d::at(part, 8));                                       // 0043F44A..0043F45D (local position)
    copy4(orientation, kotor_scene3d::at(part, 0x14));                                 // 0043F461..0043F484 (local orientation)
    (void)table<PartTable>(part)->s00c(part, nullptr, word(position), word(orientation));   // 0043F47B..0043F48D GetWorld
    if (position_out != 0) copy3(ptr<void>(position_out), position);    // 0043F490..0043F4A9
    if (orientation_out != 0) copy4(ptr<void>(orientation_out), orientation);   // 0043F4AC..0043F4CC
    return 1;                                                           // 0043F4CF
}
U __fastcall gob_set_part_local_position_0043f4e0(Gob* self, void*, U name, U px, U py, U pz, U qw, U qx, U qy, U qz) {   // +9C
    void* part = gob_vt(self)->s10c_gob_get_part(self, nullptr, ptr<const char>(name));   // 0043F4E0..0043F4E7
    if (part == nullptr) return 0;                                      // 0043F4EF..0043F4F1
    field<U>(part, 0x08) = px; field<U>(part, 0x0C) = py; field<U>(part, 0x10) = pz;   // 0043F4F4..0043F508
    field<U>(part, 0x14) = qw; field<U>(part, 0x18) = qx; field<U>(part, 0x1C) = qy;   // 0043F50B..0043F51F
    field<U>(part, 0x48) = 1;                                           // 0043F526 (transform dirty)
    field<U>(part, 0x20) = qz;                                          // 0043F52D
    return 1;                                                           // 0043F530
}
U __fastcall gob_get_part_local_position_0043f540(Gob* self, void*, U name, U position_out, U orientation_out) {   // +A0
    void* part = gob_vt(self)->s10c_gob_get_part(self, nullptr, ptr<const char>(name));   // 0043F540..0043F547
    if (part == nullptr) return 0;                                      // 0043F54F..0043F551
    if (position_out != 0) copy3(ptr<void>(position_out), kotor_scene3d::at(part, 8));          // 0043F554..0043F56D
    if (orientation_out != 0) copy4(ptr<void>(orientation_out), kotor_scene3d::at(part, 0x14)); // 0043F571..0043F58F
    return 1;                                                           // 0043F592
}

// ---------------------------------------------------------------- render state setters
// Gob::SetWind(U8 enable, U a, U b, U c) (RET 10h).
U __fastcall gob_set_wind_0043edd0(Gob* self, void*, U enable, U a, U b, U c) {   // +D8
    field<U8>(self, 0x156) = static_cast<U8>(enable);                   // 0043EDD0 / 0043EDD8
    field<U>(self, 0x15C) = a;                                          // 0043EDE2
    field<U>(self, 0x158) = b;                                          // 0043EDEC
    field<U>(self, 0x160) = c;                                          // 0043EDF2
    return b;                                                           // EAX = [esp+0Ch] (0043EDDE)
}
// Gob::EnableRenderGobBBox(const Vector* color) (RET 4).
U __fastcall gob_enable_render_gob_bbox_0043ee00(Gob* self, void*, U color) {   // +D0
    field<U16>(self, 0xC8) = 1;                                         // 0043EE04
    copy3(kotor_scene3d::at(self, 0x148), ptr<void>(color));                           // 0043EE0D..0043EE20
    return field<U>(ptr<void>(color), 8);                               // EAX = color[2] (0043EE1D)
}
// Gob::SetColorShifting(r, g, b, a, propagate) (RET 14h).
U __fastcall gob_set_color_shifting_0043ee50(Gob* self, void*, U r, U g, U b, U a, U propagate) {   // +B4
    field<U>(self, 0xCC) = r; field<U>(self, 0xD0) = g; field<U>(self, 0xD4) = b;   // 0043EE50..0043EE67
    field<U>(self, 0xD8) = a;                                           // 0043EE6E
    field<U16>(self, 0xC4) = 1;                                         // 0043EE7A
    if (propagate == 0) return 0;                                       // 0043EE78..0043EE83 (EAX = arg = 0)
    return gob_vt(self)->do_event(self, nullptr, kPropagateColorShift_00740aac, 0);   // 0043EE85..0043EE8E
}
// Gob::SetIllumination(r, g, b, propagate) (RET 10h).
U __fastcall gob_set_illumination_0043eea0(Gob* self, void*, U r, U g, U b, U propagate) {   // +B8
    field<U>(self, 0xE0) = r; field<U>(self, 0xE4) = g; field<U>(self, 0xE8) = b;   // 0043EEA0..0043EEB7
    field<U16>(self, 0xC6) = 1;                                         // 0043EEC0
    if (propagate == 0) return 0;                                       // 0043EEBE..0043EEC9 (EAX = arg = 0)
    return gob_vt(self)->do_event(self, nullptr, kPropagateIllumination_00740ac0, 0);   // 0043EECB..0043EED4
}
U __fastcall gob_disable_fog_00440870(Gob* self, void*) {              // +E4
    part_set_fog_00440550(field<void*>(self, 0x6C), 0);                 // 00440870..00440876 PartSetFog(root, 0)
    return 0;                                                           // EAX incidental (callee's)
}
U __fastcall gob_enable_fog_00440880(Gob* self, void*) {               // +E8
    part_set_fog_00440550(field<void*>(self, 0x6C), 1);                 // 00440880..00440886 PartSetFog(root, 1)
    return 0;                                                           // EAX incidental (callee's)
}
// Gob::SetCanDownSample(U8 flag) (RET 4).
U __fastcall gob_set_can_down_sample_004437f0(Gob* self, void*, U flag) {   // +40
    if (field<U8>(self, 0xC0) == static_cast<U8>(flag)) return 0;       // 004437F4..004437FE (xor eax,eax)
    field<U8>(self, 0xC0) = static_cast<U8>(flag);                      // 00443802
    return part_set_can_down_sample_004434a0(field<void*>(self, 0x6C), flag);   // 00443801..0044380C (raw DWORD pushed, D12)
}
// Gob::SetUpImposters(a, b) (RET 8).
U __fastcall gob_set_up_imposters_004404b0(Gob* self, void*, U a, U b) {   // +18C
    field<U>(self, 0x34) = a;                                           // 004404BD
    const U imposter = gl_render_set_up_imposters_00432160(a, b);       // 004404BB..004404C5
    field<U>(self, 0x30) = imposter;                                    // 004404C8
    return imposter;                                                    // EAX
}
// Gob::EnumParts(callback, out) (RET 8): stdcall walker 004474D0 over the root part.
U __fastcall gob_enum_parts_004477b0(Gob* self, void*, U (__cdecl* callback)(void*), U out) {   // D75 typed callback   // +110
    return enum_parts_004474d0(field<void*>(self, 0x6C), callback, out);   // 004477B0..004477BE
}
// Gob::GetBehavior(int type) (RET 4): -1 = any.
U __fastcall gob_get_behavior_0043f130(Gob* self, void*, U type) {     // +108
    if (type != 0xFFFFFFFFu) {                                          // 0043F136..0043F13B
        void* behavior = field<void*>(self, 0x188);                     // 0043F13D
        if (behavior == nullptr) return 0;                              // 0043F145..0043F151
        if (table<BehaviorTable>(behavior)->get_type(behavior, nullptr) != type) return 0;   // 0043F147..0043F14E
    }
    return field<U>(self, 0x188);                                       // 0043F157
}
// Gob::BehaviorMessage(char* line) (RET 4): tail jump into the behaviour's +0C slot.
U __fastcall gob_behavior_message_0043f5a0(Gob* self, void*, U line) { // +A8
    void* behavior = field<void*>(self, 0x188);                         // 0043F5A0
    if (behavior != nullptr)                                            // 0043F5A6
        table<BehaviorTable>(behavior)->message(behavior, nullptr, ptr<char>(line));   // 0043F5AA..0043F5AC jmp [eax+0Ch]
    return 0;                                                           // EAX incidental (void slot / 0043F5AF)
}
// Gob::HitCheckHack(start xyz, end xyz, Vector* out) (RET 1Ch): forwards to vtbl+8C HitCheck with a null hit info.
U __fastcall gob_hit_check_hack_0043f170(Gob* self, void*, U sx, U sy, U sz, U ex, U ey, U ez, U out) {   // +84
    const Vector start{bits_to_float(sx), bits_to_float(sy), bits_to_float(sz)};   // 0043F197..0043F1A9 (DWORD copies)
    const Vector end{bits_to_float(ex), bits_to_float(ey), bits_to_float(ez)};     // 0043F17E..0043F190
    // 0043F17B..0043F1AC: push 0 (hit info), push out, end, start; call [eax+8Ch]; EAX passed through.
    return static_cast<U>(gob_vt(self)->s08c_gob_hit_check(self, nullptr, start, end, ptr<Vector>(out), nullptr));
}
// Gob::SetObjectScale(float scale, U8 relative) (RET 8): propagates to the attached child gobs (+1BC list).
U __fastcall gob_set_object_scale_00444d90(Gob* self, void*, U scale_bits, U relative) {   // +CC
    if (field<U>(self, 0x58) == 0) return 0;                            // 00444D93..00444D9D (EAX = [esi+58h] = 0; AL = 0)
    const double scale = W(bits_to_float(scale_bits));
    double product;
    if (static_cast<U8>(relative) == 1) {                               // 00444DA0 cmp byte [esp+0Ch],1
        const double ratio = W(field<float>(self, 0x1B4)) / W(field<float>(self, 0x1B8));   // 00444DA7..00444DB1 fdiv
        field<U>(self, 0x1B8) = scale_bits;                             // 00444DB7 (raw DWORD)
        field<float>(self, 0x1B4) = S(ratio);                           // 00444DBD fstp
        product = scale * W(field<float>(self, 0x1B4));                 // 00444DC3..00444DC7
    } else {
        product = scale * W(field<float>(self, 0x1B8));                 // 00444DCF..00444DD3
    }
    field<float>(self, 0x1B4) = S(product);                             // 00444DDF fstp [esi+1B4h]
    U eax = field<U>(self, 0x1C0);                                      // 00444DD9
    for (I i = 0; i < static_cast<I>(eax); ++i) {                       // 00444DE6..00444E13 (count re-read per pass)
        void* child = ptr<void>(field<U*>(self, 0x1BC)[i]);             // 00444DF0..00444DF6
        (void)gob_vt(child)->s0cc_gob_set_object_scale(as_gob(child), nullptr, field<U>(self, 0x1B4), 0);   // 00444DF9..00444E04
        eax = field<U>(self, 0x1C0);                                    // 00444E0A
    }
    return (eax & 0xFFFFFF00u) | 1u;                                    // 00444E16 mov al,1 (upper bits = last count read)
}
} // extern "C"
