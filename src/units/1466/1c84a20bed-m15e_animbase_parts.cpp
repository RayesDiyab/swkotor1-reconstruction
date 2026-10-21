// lane-m15-e (L15-E) fork A, unit m15e-animbase-parts: CSWCAnimBaseHead (this = head+50h) and
// CSWCAnimBaseWield (this = wield+5Ch) members reached from the CSWCAnimBaseHeadWield entries, plus
// CSWCItem::GetEquippedBy 00646630 (Wield::Wield callee). Original: swkotor.exe GOG K1; statements cite the
// original instructions. Conventions: m15e_animbase_internal.h. EAX leftovers: see m15e_animbase_core.cpp.
#include "628decf93e-m15e_animbase_internal.h"

using namespace kotor_m15e_ab;

namespace {
constexpr char kHeadHook_0074f328[] = "HEADHOOK";
constexpr char kHead_0074a234[] = "head";
constexpr char kDummyRight_00754d28[] = "dummyright";
constexpr char kDummyLeft_00754d1c[] = "dummyleft";
constexpr char kRHand_0074f334[] = "rhand";
constexpr char kLHand_0074f4b8[] = "lhand";
constexpr char kUnequip_00754d3c[] = "unequip";
constexpr char kEquip_00754d34[] = "equip";

inline const KotorM15ObjectMethods_007516c8* ovt(const void* object) noexcept {
    return ld<const KotorM15ObjectMethods_007516c8*>(object, 0);
}
inline void* item_by_id(U id) noexcept(false) { return kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id); }
// (item+10h)->GetModel(FFh, 1): CSWCItem's CSWCObject subobject, object slot +98h.
inline void* item_model(void* item) noexcept(false) {
    void* const object = at(item, 0x10);
    return ovt(object)->s098_get_model(object, nullptr, 0xFF, 1);
}
inline void detach(void* g) noexcept(false) {
    gvt(g)->s050_gob_attach_to_object(g, nullptr, nullptr, nullptr, 0);   // push 0 x3; call [eax+50h]
    gvt(g)->attach_to_scene(gob(g), nullptr, nullptr);                   // push 0; call [edx+4Ch]
}
inline void delete_gob(void* g) noexcept(false) { gvt(g)->s000_gob_destruct_gob(gob(g), nullptr, 1); }   // push 1; call [eax]
inline void* head_tw(void* self) noexcept { return at(self, vbe(self, -0x50, 1) - 0x50); }   // lea ecx,[vb1+this-50h]
inline void* wield_tw(void* self) noexcept { return at(self, vbe(self, -0x5C, 1) - 0x5C); }  // lea ecx,[vb1+this-5Ch]
inline void* tw_body(void* tw) noexcept { return pv(tw, 0xB8); }
// CResRef copy into a char buffer (00405FB0) and lower-casing (007135DF _strlwr).
struct Name { char text[20]; };
inline void ref_text(const void* ref, Name& out) noexcept(false) { gui_load_resref_copy_to_string_00405fb0(static_cast<const Ref*>(ref), nullptr, out.text); }
inline void assign_ref(Ref* target, Ref* source) noexcept(false) { Ref tmp; c2da_copy_00406120(target, nullptr, &tmp, source); }
inline Ref* as_ref(U word) noexcept { return reinterpret_cast<Ref*>(static_cast<std::uintptr_t>(word)); }
inline const void* as_ptr(U word) noexcept { return reinterpret_cast<const void*>(static_cast<std::uintptr_t>(word)); }
}  // namespace

extern "C" {
// ============================================================================================= CSWCAnimBaseHead
// 0069B060 GetModel(part) RET 4: FE -> head gob [this-0Ch]; FF -> TW::GetModel(FFh); else 0.
void* __fastcall kotor_m15e_anim_base_head_get_model_0069b060(void* self, void*, kotor_m15e::U part) noexcept(false) {
    const U8 p = static_cast<U8>(part);                                    // 0069B060 movzx eax,byte ptr [esp+4]
    if (p == 0xFE) return pv(self, -0x0C);                                 // 0069B08B
    if (p == 0xFF) return kotor_m15e_anim_base_tw_get_model_0069bcb0(head_tw(self), nullptr, 0xFF);   // 0069B074..86
    return nullptr;                                                        // 0069B06F
}
// 0069B0A0 LoadModel(resref, part) RET 8 (table 0069B1A8: FC/FD -> TW, FE -> head model, FF -> TW + attach).
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_load_model_0069b0a0(void* self, void*, void* resref, kotor_m15e::U part) noexcept(false) {
    const U idx = static_cast<U>(static_cast<U8>(part)) + 0xFFFFFF04u;     // 0069B0AA..AD
    if (idx > 3) return 0;                                                 // 0069B0B2..B5 / 0069B19B
    if (idx == 3) {                                                        // 0069B0C3 (FF)
        if (kotor_m15e_anim_base_tw_load_model_0069bce0(head_tw(self), nullptr, resref, 0xFF) == 0) return 0;   // 0069B0C3..DE
        void* const head = pv(self, -0x0C);                                // 0069B0E4
        if (head != nullptr)                                               // 0069B0E7..E9
            gvt(head)->s050_gob_attach_to_object(head, nullptr, pv(self, vbe(self, -0x50, 1) + 0x68), kHeadHook_0074f328, 0);   // 0069B0EB..FF
        return 1;                                                          // 0069B103
    }
    if (idx != 2) {                                                        // 0069B10F (FC / FD)
        return kotor_m15e_anim_base_tw_load_model_0069bce0(head_tw(self), nullptr, resref, part) != 0 ? 1u : 0u;   // 0069B10F..29 neg/sbb/neg
    }
    Name name;                                                             // 0069B132 (FE)
    ref_text(resref, name);                                                // 0069B136..3D
    aurora_model::Gob* const head = aurora_new_aur_object_00449cc0(name.text, kHead_0074a234, nullptr, nullptr);   // 0069B142..55
    st<void*>(self, -0x0C, head);                                          // 0069B15A
    if (head == nullptr) return 0;                                         // 0069B158..5D
    assign_ref(ref_at(self, -0x34), static_cast<Ref*>(resref));            // 0069B15F..68
    st<U>(self, -0x24, 0);                                                 // 0069B173
    gvt(head)->s050_gob_attach_to_object(head, nullptr, pv(self, vbe(self, -0x50, 1) + 0x68), kHeadHook_0074f328, 0);   // 0069B16D..8B
    return 1;                                                              // 0069B18F
}
// 0069B1C0 ReplaceTexture(from, to, keep) RET 0Ch.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_replace_texture_0069b1c0(void* self, void*, kotor_m15e::U from, kotor_m15e::U to,
    kotor_m15e::U keep) noexcept(false) {
    Name a, b;
    ref_text(as_ptr(from), a);                                             // 0069B1CB..D4
    ref_text(as_ptr(to), b);                                               // 0069B1D9..E4
    c2da_current_strlwr_007135df(b.text);                                  // 0069B1E9..EE
    c2da_current_strlwr_007135df(a.text);                                  // 0069B1F3..F8
    void* const head = pv(self, -0x0C);                                    // 0069B1FD
    if (head != nullptr) gvt(head)->s034_gob_replace_texture(gob(head), nullptr, wd(a.text), wd(b.text));   // 0069B203..13
    if (keep != 0) {                                                       // 0069B216..1C
        assign_ref(ref_at(self, -0x34), as_ref(to));                       // 0069B21E..27
        st<U>(self, -0x24, 0);                                             // 0069B22C
    }
    return kotor_m15e_anim_base_tw_replace_texture_0069be20(head_tw(self), nullptr, from, to, keep);   // 0069B233..40
}
// 0069B250 ReplaceTexture(part, from, to, keep) RET 10h: FE -> head gob, else TW 0069BF30.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_replace_texture_0069b250(void* self, void*, kotor_m15e::U part, kotor_m15e::U from,
    kotor_m15e::U to, kotor_m15e::U keep) noexcept(false) {
    if (static_cast<U8>(part) != 0xFE)                                      // 0069B257..5C
        return kotor_m15e_anim_base_tw_replace_texture_0069bf30(head_tw(self), nullptr, part, from, to, keep);   // 0069B2E4..FE
    if (pv(self, -0x0C) == nullptr) return 0;                              // 0069B262..69
    Name a, b;
    ref_text(as_ptr(from), a);                                             // 0069B272..7C
    ref_text(as_ptr(to), b);                                               // 0069B281..8C
    c2da_current_strlwr_007135df(a.text);                                  // 0069B291..96
    c2da_current_strlwr_007135df(b.text);                                  // 0069B29B..A0
    void* const head = pv(self, -0x0C);                                    // 0069B2A5
    gvt(head)->s034_gob_replace_texture(gob(head), nullptr, wd(a.text), wd(b.text));   // 0069B2A8..B7
    if (keep != 0) {                                                       // 0069B2BA..C0
        assign_ref(ref_at(self, -0x34), as_ref(to));                       // 0069B2C2..CB
        st<U>(self, -0x24, 0);                                             // 0069B2D0
    }
    return 1;                                                              // 0069B2D8
}
// 0069B310 ReplaceTexture(part, from, to, count, attrs, keep, first) RET 1Ch: FE -> head gob +30h, else TW 0069C090.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_replace_texture_0069b310(void* self, void*, kotor_m15e::U part, kotor_m15e::U from,
    kotor_m15e::U to, kotor_m15e::U count, kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first) noexcept(false) {
    if (static_cast<U8>(part) != 0xFE)                                      // 0069B317..1C
        return kotor_m15e_anim_base_tw_replace_texture_0069c090(head_tw(self), nullptr, part, from, to, count, attrs, keep, first);   // 0069B3D9..402
    if (pv(self, -0x0C) == nullptr) return 0;                              // 0069B322..2F
    Name a, b;
    ref_text(as_ptr(from), a);                                             // 0069B332..3E
    ref_text(as_ptr(to), b);                                               // 0069B343..4E
    c2da_current_strlwr_007135df(a.text);                                  // 0069B353..58
    c2da_current_strlwr_007135df(b.text);                                  // 0069B35D..62
    void* const head = pv(self, -0x0C);                                    // 0069B373
    gvt(head)->s030_gob_replace_texture(gob(head), nullptr, wd(a.text), wd(b.text), count, attrs, first);   // 0069B367..88
    if (keep != 0) {                                                       // 0069B38B..91
        assign_ref(ref_at(self, -0x34), as_ref(to));                       // 0069B393..9C
        st<U>(self, -0x24, 1);                                             // 0069B3AA
        st<U8>(self, -0x38, static_cast<U8>(count));                       // 0069B3B1
        st<U8>(self, -0x37, static_cast<U8>(first));                       // 0069B3B4
        std::memcpy(at(self, -0x4C), static_cast<const unsigned char*>(as_ptr(attrs)) + first * 2u, count * 2u);   // 0069B3A5..C8 rep movs
    }
    return 1;                                                              // 0069B3CD
}
// 0069B570 RestoreTexture(part) RET 4: FE -> replay/restore the head texture, else TW 0069C4C0.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_restore_texture_0069b570(void* self, void*, kotor_m15e::U part) noexcept(false) {
    if (static_cast<U8>(part) != 0xFE)                                      // 0069B577..7C
        return kotor_m15e_anim_base_tw_restore_texture_0069c4c0(head_tw(self), nullptr, part);   // 0069B5EF..FA
    void* const head = pv(self, -0x0C);                                    // 0069B57E
    if (head == nullptr) return 0;                                         // 0069B581..83 / 0069B5E6
    if (ld<U>(self, -0x24) == 0) {                                         // 0069B585..8A
        gvt(head)->s03c_gob_restore_texture(gob(head), nullptr);           // 0069B5D5..D7
        return 1;
    }
    Ref empty;
    empty.construct_00406d80(kEmpty_0073d71c);                             // 0069B58C..95
    void* const tw = head_tw(self);                                        // 0069B59A..B4
    avt(tw)->s014(tw, nullptr, 0xFE, wd(&empty), wd(at(self, -0x34)), ld<U8>(self, -0x38), wd(at(self, -0x4C)), 0,
                  ld<U8>(self, -0x37));                                    // 0069B5A0..C6 call [edx+14h]
    return 1;                                                              // 0069B5C9
}
// 0069B610 UnloadModel() RET 0: drop the head gob, then TW::UnloadModel (jmp).
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_unload_model_0069b610(void* self, void*) noexcept(false) {
    void* const head = pv(self, -0x0C);                                    // 0069B613
    if (head != nullptr) {                                                 // 0069B616..18
        gvt(head)->s050_gob_attach_to_object(head, nullptr, nullptr, nullptr, 0);   // 0069B61A..22
        void* const again = pv(self, -0x0C);                               // 0069B625
        if (again != nullptr) delete_gob(again);                           // 0069B628..30
        st<void*>(self, -0x0C, nullptr);                                   // 0069B632
    }
    return kotor_m15e_anim_base_tw_unload_model_0069c5d0(head_tw(self), nullptr);   // 0069B639..44
}
// 0069B650 UnloadModel(part) RET 4: FE with a head gob -> drop it; else TW 0069C640.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_unload_model_0069b650(void* self, void*, kotor_m15e::U part) noexcept(false) {
    void* const head = pv(self, -0x0C);                                    // 0069B657
    if (head != nullptr && static_cast<U8>(part) == 0xFE) {                // 0069B65A..60
        gvt(head)->s050_gob_attach_to_object(head, nullptr, nullptr, nullptr, 0);   // 0069B662..6A
        void* const again = pv(self, -0x0C);                               // 0069B66D
        if (again != nullptr) delete_gob(again);                           // 0069B670..78
        st<void*>(self, -0x0C, nullptr);                                   // 0069B67A
        return 0;                                                          // 0069B682 D2 (EAX leftover; callers 00697F0A.. discard it)
    }
    return kotor_m15e_anim_base_tw_unload_model_0069c640(head_tw(self), nullptr, part);   // 0069B685..90
}
// 0069B720 SetEnvMap(part, enable, keep, resref) RET 10h: FE -> head gob +48h, else TW 0069C790.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_set_env_map_0069b720(void* self, void*, kotor_m15e::U part, kotor_m15e::U a2,
    kotor_m15e::U a3, void* resref) noexcept(false) {
    if (static_cast<U8>(part) != 0xFE)                                      // 0069B727..2C
        return kotor_m15e_anim_base_tw_set_env_map_0069c790(head_tw(self), nullptr, part, a2, a3, resref);   // 0069B793..AD
    void* const head = pv(self, -0x0C);                                    // 0069B72E
    if (head == nullptr) return 0;                                         // 0069B731..35
    if (a2 != 0) {                                                         // 0069B73F..4A
        Name name;
        ref_text(resref, name);                                            // 0069B74C..53
        gvt(head)->s048_gob_set_environment_map(gob(head), nullptr, wd(name.text));   // 0069B758..69
    } else {
        gvt(head)->s048_gob_set_environment_map(gob(head), nullptr, wd(kEmpty_0073d71c));   // 0069B762..69
    }
    if (a3 != 0) {                                                         // 0069B76C..72
        assign_ref(ref_at(self, -0x20), static_cast<Ref*>(resref));        // 0069B774..7D
        st<U>(self, -0x10, a2);                                            // 0069B782
    }
    return 1;                                                              // 0069B787
}
// 0069B810 TurnOnShadows() RET 0: head gob +C0h, then CSWCAnimBase::TurnOnShadows (jmp).
void __fastcall kotor_m15e_anim_base_head_turn_on_shadows_0069b810(void* self, void*) noexcept(false) {
    void* const head = pv(self, -0x0C);                                    // 0069B813
    if (head != nullptr) gvt(head)->s0c0_gob_turn_on_shadows(gob(head), nullptr);   // 0069B816..1C
    kotor_m15e_anim_base_turn_on_shadows_0069dc10(head_tw(self), nullptr);   // 0069B822..2D
}
// 0069B950 EnableDistortion() / 0069B9C0 DisableDistortion(): head gob, head item model (twice), then CSWCAnimBase.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_enable_distortion_0069b950(void* self, void*) noexcept(false) {
    void* const item = item_by_id(ld<U>(self, -8));                        // 0069B950..60
    void* const head = pv(self, -0x0C);                                    // 0069B965
    if (head != nullptr) gvt(head)->s0f8_gob_enable_distortion(gob(head), nullptr);   // 0069B968..70
    if (item != nullptr && item_model(item) != nullptr) {                  // 0069B976..91
        void* const model = item_model(item);                              // 0069B993..9E
        gvt(model)->s0f8_gob_enable_distortion(gob(model), nullptr);       // 0069B9A4..A8
    }
    return kotor_m15e_anim_base_enable_distortion_0069de50(head_tw(self), nullptr);   // 0069B9AE..BA
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_disable_distortion_0069b9c0(void* self, void*) noexcept(false) {
    void* const item = item_by_id(ld<U>(self, -8));                        // 0069B9C0..D0
    void* const head = pv(self, -0x0C);                                    // 0069B9D5
    if (head != nullptr) gvt(head)->s0fc_gob_disable_distortion(gob(head), nullptr);  // 0069B9D8..E0
    if (item != nullptr && item_model(item) != nullptr) {                  // 0069B9E6..A01
        void* const model = item_model(item);                              // 0069BA03..0E
        gvt(model)->s0fc_gob_disable_distortion(gob(model), nullptr);      // 0069BA14..18
    }
    return kotor_m15e_anim_base_disable_distortion_0069de70(head_tw(self), nullptr);  // 0069BA1E..2A
}

// ============================================================================================= CSWCAnimBaseWield
// 00699FC0 SetWieldLimited(v) RET 4: [this-18h] = v.
void __fastcall kotor_m15e_anim_base_wield_set_wield_limited_00699fc0(void* self, void*, kotor_m15e::U limited) noexcept(false) {
    st<U>(self, -0x18, limited);                                           // 00699FC0..C4
}
// 0069A010 ShowWieldedItems() RET 0.
void __fastcall kotor_m15e_anim_base_wield_show_wielded_items_0069a010(void* self, void*) noexcept(false) {
    if (ld<U>(self, -0x14) == 0) return;                                   // 0069A016..1B
    for (std::ptrdiff_t off : {static_cast<std::ptrdiff_t>(-0x28), static_cast<std::ptrdiff_t>(-0x24)}) {   // 0069A021 / 0069A051
        void* const g = pv(self, off);
        if (g == nullptr) continue;
        detach(g);                                                         // 0069A028..3A / 0069A058..6A
        void* const again = pv(self, off);                                 // 0069A03D / 0069A06D
        if (again != nullptr) delete_gob(again);                           // 0069A040..48 / 0069A070..78
        st<void*>(self, off, nullptr);                                     // 0069A04A / 0069A07A
    }
    void* const right = item_by_id(ld<U>(self, -0x20));                    // 0069A081..8F
    if (right != nullptr) {                                                // 0069A094..96
        void* const model = item_model(right);                             // 0069A098..A4
        if (model != nullptr)                                              // 0069A0AC..AE
            gvt(model)->s050_gob_attach_to_object(model, nullptr, pv(wield_tw(self), 0xB8),
                                                 WindowCStr005e5670(str_at(self, -0x38), nullptr), 0);   // 0069A0B0..CA
        void* const sound = pv(self, -8);                                  // 0069A0CD
        if (sound != nullptr) static_cast<kotor_input_hover::SoundSource*>(sound)->Play();   // 0069A0D0..D4
    }
    void* const left = item_by_id(ld<U>(self, -0x1C));                     // 0069A0D9..E6
    if (left != nullptr) {                                                 // 0069A0EB..ED
        void* const model = item_model(left);                              // 0069A0EF..FB
        if (model != nullptr)                                              // 0069A103..05
            gvt(model)->s050_gob_attach_to_object(model, nullptr, pv(wield_tw(self), 0xB8),
                                                 WindowCStr005e5670(str_at(self, -0x30), nullptr), 0);   // 0069A107..21
    }
    kotor_m15e_anim_base_refresh_hilite_0069ec00(wield_tw(self), nullptr); // 0069A124..2E
    ref_assign_text(ref_at(self, -0x58), kEmpty_0073d71c);                 // 0069A133..40
    ref_assign_text(ref_at(self, -0x48), kEmpty_0073d71c);                 // 0069A145..52
    st<U>(self, -0x14, 0);                                                 // 0069A158
}
// 0069A170 TurnOnShadows() RET 0 -> CSWCAnimBase::TurnOnShadows (jmp).
void __fastcall kotor_m15e_anim_base_wield_turn_on_shadows_0069a170(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_turn_on_shadows_0069dc10(wield_tw(self), nullptr);   // 0069A170..7A
}
// 0069A650 GetItemAurObject(item_id, part) RET 8 (this unused).
void* __fastcall kotor_m15e_anim_base_wield_get_item_aur_object_0069a650(void*, void*, kotor_m15e::U item_id, kotor_m15e::U part) noexcept(false) {
    void* const item = item_by_id(item_id);                                // 0069A650..5D
    if (item == nullptr) return nullptr;                                   // 0069A662..66
    void* const object = at(item, 0x10);                                   // 0069A66D
    return ovt(object)->s098_get_model(object, nullptr, part, 1);          // 0069A670..75
}
// 0069A680 ItemIsValid(item_id) RET 4 (this = wield base): [this+44h] == 0 -> 1, else item exists.
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_item_is_valid_0069a680(void* wield, void*, kotor_m15e::U item_id) noexcept(false) {
    if (ld<U>(wield, 0x44) == 0) return 1;                                 // 0069A680..8C
    return item_by_id(item_id) != nullptr ? 1u : 0u;                       // 0069A68F..A5
}
// 0069A6B0 HideWieldedItems(right_ref, left_ref) RET 8.
void __fastcall kotor_m15e_anim_base_wield_hide_wielded_items_0069a6b0(void* self, void*, void* right_ref, void* left_ref) noexcept(false) {
    StartupString name;                                                    // 0069A6CD..D1 (005B3190)
    void* const right = item_by_id(ld<U>(self, -0x20));                    // 0069A6D6..EA
    if (right != nullptr) {                                                // 0069A6EF..F1
        void* const model = item_model(right);                             // 0069A6F3..FF
        if (model != nullptr) {                                            // 0069A707..09
            detach(model);                                                 // 0069A70B..1E
            void* const sound = pv(self, -8);                              // 0069A721
            if (sound != nullptr) kotor_m15_sound_source_stop_005d5a20(sound, nullptr);   // 0069A724..28
        }
    }
    void* const left = item_by_id(ld<U>(self, -0x1C));                     // 0069A72D..3A
    if (left != nullptr) {                                                 // 0069A73F..41
        void* const model = item_model(left);                              // 0069A743..4F
        if (model != nullptr) detach(model);                               // 0069A757..6E
    }
    const struct { std::ptrdiff_t ref, gob; void* source; const char* dummy; const char* hook; } hands[2] = {
        {-0x58, -0x28, right_ref, kDummyRight_00754d28, kRHand_0074f334},  // 0069A771..A813
        {-0x48, -0x24, left_ref, kDummyLeft_00754d1c, kLHand_0074f4b8},    // 0069A818..A8BA
    };
    for (const auto& hand : hands) {
        Ref* const current = ref_at(self, hand.ref);                       // 0069A775 / 0069A81C
        if (gui_load_resref_not_equal_00406dc0(current, nullptr, static_cast<Ref*>(hand.source)) == 0) continue;   // 0069A778..82 / 0069A81F..29
        void* const old_gob = pv(self, hand.gob);                          // 0069A788 / 0069A82F
        if (old_gob != nullptr) {
            detach(old_gob);                                               // 0069A78F..A1 / 0069A836..48
            void* const again = pv(self, hand.gob);                        // 0069A7A4 / 0069A84B
            if (again != nullptr) delete_gob(again);                       // 0069A7A7..AF / 0069A84E..56
            st<void*>(self, hand.gob, nullptr);                            // 0069A7B1 / 0069A858
        }
        if (*static_cast<const char*>(kotor_m15e_slot00c_self_00641db0(hand.source, nullptr)) != '\0') {   // 0069A7B8..C2 / 0069A85F..69
            c2da_ref_to_string_00405f70(static_cast<Ref*>(hand.source), nullptr, &name);   // 0069A7C4..CB / 0069A86B..72
            aurora_model::Gob* const dummy = aurora_new_aur_object_00449cc0(WindowCStr005e5670(&name, nullptr), hand.dummy, nullptr, nullptr);   // 0069A7D0..E3 / 0069A877..8A
            st<void*>(self, hand.gob, dummy);                              // 0069A7ED / 0069A894
            if (dummy != nullptr)                                          // 0069A7EB..F0 / 0069A892..97
                gvt(dummy)->s050_gob_attach_to_object(dummy, nullptr, pv(wield_tw(self), 0xB8), hand.hook, 0);   // 0069A7F2..808 / 0069A899..8AF
        }
        assign_ref(current, static_cast<Ref*>(hand.source));               // 0069A80B..13 / 0069A8B2..BA
    }
    st<U>(self, -0x14, 1);                                                 // 0069A8C3
}
// 0069A8F0 EnableDistortion() / 0069A980 DisableDistortion(): left then right item models, then CSWCAnimBase.
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_enable_distortion_0069a8f0(void* self, void*) noexcept(false) {
    for (std::ptrdiff_t off : {static_cast<std::ptrdiff_t>(-0x1C), static_cast<std::ptrdiff_t>(-0x20)}) {   // 0069A8F3 / 0069A92A
        void* const item = item_by_id(ld<U>(self, off));
        if (item == nullptr) continue;
        void* const model = item_model(item);                              // 0069A909..16 / 0069A940..4D
        if (model != nullptr) gvt(model)->s0f8_gob_enable_distortion(gob(model), nullptr);   // 0069A91C..24 / 0069A953..5B
    }
    return kotor_m15e_anim_base_enable_distortion_0069de50(wield_tw(self), nullptr);   // 0069A961..6C
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_disable_distortion_0069a980(void* self, void*) noexcept(false) {
    for (std::ptrdiff_t off : {static_cast<std::ptrdiff_t>(-0x1C), static_cast<std::ptrdiff_t>(-0x20)}) {   // 0069A983 / 0069A9BA
        void* const item = item_by_id(ld<U>(self, off));
        if (item == nullptr) continue;
        void* const model = item_model(item);                              // 0069A999..A6 / 0069A9D0..DD
        if (model != nullptr) gvt(model)->s0fc_gob_disable_distortion(gob(model), nullptr);  // 0069A9AC..B4 / 0069A9E3..EB
    }
    return kotor_m15e_anim_base_disable_distortion_0069de70(wield_tw(self), nullptr);  // 0069A9F1..FC
}
// 0069AA90 GetModel(part) RET 4: F4..F7 -> left item model (FC..FF), F8..FB -> right item model, else TW 0069BCB0.
void* __fastcall kotor_m15e_anim_base_wield_get_model_0069aa90(void* self, void*, kotor_m15e::U part) noexcept(false) {
    const U idx = static_cast<U>(static_cast<U8>(part)) + 0xFFFFFF0Cu;     // 0069AA94..97
    if (idx > 7) return kotor_m15e_anim_base_tw_get_model_0069bcb0(wield_tw(self), nullptr, part);   // 0069AA9C..9F / 0069AB4C..5A
    static const U kPart[8] = {0xFC, 0xFD, 0xFE, 0xFF, 0xFC, 0xFD, 0xFE, 0xFF};     // table 0069AB60: AB38/AB24/AB10/AAFC, AAE8/AAD4/AAC0/AAAC
    const U id = idx < 4 ? ld<U>(self, -0x1C) : ld<U>(self, -0x20);        // [ecx-1Ch] (left) / [ecx-20h] (right)
    return kotor_m15e_anim_base_wield_get_item_aur_object_0069a650(at(self, -0x5C), nullptr, id, kPart[idx]);   // add ecx,-5Ch; call 0069A650
}
// 0069AB80 Wield(item_id, hook_name, hand, scale) RET 10h (dumpbin 0069AB80..0069AF01).
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_wield_0069ab80(void* self, void*, kotor_m15e::U item_id, void* hook_name,
    kotor_m15e::U hand, kotor_m15e::U scale) noexcept(false) {
    void* const new_item = item_by_id(item_id);                            // 0069AB80..93
    void* const new_model = new_item != nullptr ? item_model(new_item) : nullptr;   // 0069AB98..B5
    StartupString* const hook = static_cast<StartupString*>(hook_name);
    void* const body = pv(wield_tw(self), 0xB8);                           // [vb1+this+5Ch]
    const U8 which = static_cast<U8>(hand);                                // 0069ABB7
    // Detach a previously held model when its item is no longer equipped (GetEquippedBy 00646630 == 0).
    auto release = [&](void* old_model, U old_id) noexcept(false) {
        void* const it = item_by_id(old_id);
        if (it != nullptr && kotor_m15e_item_get_equipped_by_00646630(it, nullptr) == 0) {
            detach(old_model);                                             // push 0 x3 / push 0
            gvt(old_model)->s0cc_gob_set_object_scale(gob(old_model), nullptr, fbits(1.0f), 1);   // push 1; push 3F800000h; call [+CCh]
        }
    };
    if (which == 0) {                                                      // 0069ABBB..BD (right hand)
        void* const old_item = item_by_id(ld<U>(self, -0x20));             // 0069ABC3..D0
        void* const old_model = old_item != nullptr ? item_model(old_item) : nullptr;   // 0069ABD5..F0
        const U old_id = ld<U>(self, -0x20);                               // 0069ABF2
        if (item_id != old_id && old_model != nullptr) {                   // 0069ABF5..FF
            void* const it = item_by_id(old_id);                           // 0069AC05..0F
            if (it != nullptr && ld<U8>(kotor_m15b_item_get_base_item_005b4790(it, nullptr), 0xAC) == 0x1D) {   // 0069AC14..26
                gvt(old_model)->s018_gob_play_animation(gob(old_model), nullptr, wd(kUnequip_00754d3c), fbits(1.0f), 3, 0);   // 0069AC28..3A
                st<U>(self, -0x0C, 0);                                     // 0069AC3D
            }
            release(old_model, ld<U>(self, -0x20));                        // 0069AC44..85
        }
        if (new_model != nullptr) {                                        // 0069AC8B..8D
            if (ld<U>(self, -0x18) != 0 && item_by_id(item_id) == nullptr) {   // 0069AC93..AB (inlined ItemIsValid)
                st<U>(self, -0x20, item_id);                               // 0069AD9C
            } else {
                gvt(new_model)->s050_gob_attach_to_object(new_model, nullptr, body, WindowCStr005e5670(hook, nullptr), 0);   // 0069ACB1..CF
                gvt(new_model)->s0cc_gob_set_object_scale(gob(new_model), nullptr, scale, 1);   // 0069ACD2..DE
                str_at(self, -0x38)->Assign005e5c50(hook);                 // 0069ACE4..E8
                void* const it = item_by_id(item_id);                      // 0069ACED..FB
                if (it != nullptr && ld<U8>(kotor_m15b_item_get_base_item_005b4790(it, nullptr), 0xAC) == 0x1D) {   // 0069AD00..16
                    gvt(new_model)->s018_gob_play_animation(gob(new_model), nullptr, wd(kEquip_00754d34), fbits(1.0f), 3, 0);   // 0069AD1C..2F
                    st<U>(self, -0x0C, 1);                                 // 0069AD32
                }
                st<U>(self, -0x20, item_id);                               // 0069AD39 / 0069AD9C
            }
        } else {                                                           // 0069AD41
            const U old = ld<U>(self, -0x20);
            if (old != ld<U>(self, -0x1C) && old_model != nullptr) release(old_model, old);   // 0069AD41..89
            GuiAssign005e5140(str_at(self, -0x38), nullptr, kEmpty_0073d71c);   // 0069AD8F..97
            st<U>(self, -0x20, item_id);                                   // 0069AD9C
        }
    } else if (which == 1) {                                               // 0069ADA4..A6 (left hand)
        void* const old_item = item_by_id(ld<U>(self, -0x1C));             // 0069ADAC..B9
        void* const old_model = old_item != nullptr ? item_model(old_item) : nullptr;   // 0069ADBE..D9
        const U old_id = ld<U>(self, -0x1C);                               // 0069ADDB
        if (item_id != old_id && old_model != nullptr) release(old_model, old_id);   // 0069ADDE..22
        if (new_model != nullptr) {                                        // 0069AE28..2A
            if (kotor_m15e_anim_base_wield_item_is_valid_0069a680(at(self, -0x5C), nullptr, item_id) != 0) {   // 0069AE2C..37
                gvt(new_model)->s050_gob_attach_to_object(new_model, nullptr, body, WindowCStr005e5670(hook, nullptr), 0);   // 0069AE3D..5B
                gvt(new_model)->s0cc_gob_set_object_scale(gob(new_model), nullptr, scale, 1);   // 0069AE5E..6A
                str_at(self, -0x30)->Assign005e5c50(hook);                 // 0069AE70..74
            }
        } else {                                                           // 0069AE7F
            const U old = ld<U>(self, -0x1C);
            if (old != ld<U>(self, -0x20) && old_model != nullptr) release(old_model, old);   // 0069AE7F..CA
            GuiAssign005e5140(str_at(self, -0x30), nullptr, kEmpty_0073d71c);   // 0069AED0..D8
        }
        st<U>(self, -0x1C, item_id);                                       // 0069AEDD
    } else {
        return 0;                                                          // 0069AEFB
    }
    kotor_m15e_anim_base_refresh_hilite_0069ec00(wield_tw(self), nullptr); // 0069AEE0..EA
    return 1;                                                              // 0069AEF2
}
// 0069AF10 ReAttachWieldedObjects() RET 0 (this = wield base): right/left item models onto the body.
void __fastcall kotor_m15e_anim_base_wield_reattach_wielded_objects_0069af10(void* wield, void*) noexcept(false) {
    void* const tw = at(wield, vbe(wield, 0, 1));                          // [esi]+4 / add ecx,esi
    const struct { std::ptrdiff_t id, hook; } hands[2] = {{0x3C, 0x24}, {0x40, 0x2C}};   // 0069AF14.. / 0069AF63..
    for (const auto& hand : hands) {
        void* const item = item_by_id(ld<U>(wield, hand.id));
        if (item == nullptr) continue;                                     // 0069AF27..29 / 0069AF75..77
        void* const model = item_model(item);                              // 0069AF2B..38 / 0069AF79..86
        if (model == nullptr) continue;                                    // 0069AF40..42 / 0069AF8E..90
        gvt(model)->s050_gob_attach_to_object(model, nullptr, pv(tw, 0xB8),
                                             WindowCStr005e5670(str_at(wield, hand.hook), nullptr), 0);   // 0069AF44..60 / 0069AF92..AE
    }
    kotor_m15e_anim_base_refresh_hilite_0069ec00(tw, nullptr);             // 0069AFB1..BB jmp
}
// 0069AFE0 LoadModel(resref, part) RET 8: TW::LoadModel, then ReAttachWieldedObjects.
kotor_m15e::U __fastcall kotor_m15e_anim_base_wield_load_model_0069afe0(void* self, void*, void* resref, kotor_m15e::U part) noexcept(false) {
    void* const wield = at(self, -0x5C);                                   // 0069AFE9
    if (kotor_m15e_anim_base_tw_load_model_0069bce0(wield_tw(self), nullptr, resref, part) == 0) return 0;   // 0069AFEC..FE
    kotor_m15e_anim_base_wield_reattach_wielded_objects_0069af10(wield, nullptr);   // 0069B000..02
    return 1;                                                              // 0069B007
}

// ============================================================================================= CSWCItem
// 00646630 CSWCItem::GetEquippedBy() RET 0: [this+160h] == 7F000000h -> 0, else GetCreatureByGameObjectID.
std::uint32_t __fastcall kotor_m15e_item_get_equipped_by_00646630(void* item, void*) noexcept(false) {
    const U id = ld<U>(item, 0x160);                                       // 00646630
    if (id == 0x7F000000u) return 0;                                       // 00646636..3F
    return frame_R4_005ed5b0(client_app(), nullptr, id);                   // 00646640..4F
}
}
