// lane-m15-e (L15-E) fork A, unit m15e-animbase-core: construction / destruction chain and the
// CSWCAnimBaseHeadWield, CSWCAnimBaseHead and CSWCAnimBaseWield members reached on the Milestone 1.5
// character-generation path (char-gen creatures use appearance modeltype 'B' -> GetAnimType 3 ->
// CSWCCreature::LoadModel 0061B52A case 3 = CSWCAnimBaseHeadWield). Original: swkotor.exe GOG K1; every
// statement cites the original instruction(s). Object model and "this" conventions: m15e_animbase_internal.h.
//
// Fidelity notes:
// * EAX leftovers: several original members end with "ret" after a call that may be skipped, so EAX is
//   whatever the last instruction left (D2/D69 class). Those entries return the value the original leaves on
//   the executed path when it is a defined call result, otherwise 0, marked "D2 (EAX leftover)". Every caller
//   of these entries in the original discards EAX (cited per entry).
// * Unwind (D30-FINAL): constructors/destructors here have no throwing callee except CExoString/CResRef
//   members whose providers never throw on these inputs; original EH states are cited, cleanup not emitted.
#include "628decf93e-m15e_animbase_internal.h"

using namespace kotor_m15e_ab;

namespace {
constexpr U kInvalidObject_00746644 = 0x7F000000u;   // .rdata 00746644 (OBJECT_INVALID)
constexpr char kHeadHook_0074f328[] = "HEADHOOK";
constexpr char kHead_0074a234[] = "head";
constexpr char kDummyRight_00754d28[] = "dummyright";
constexpr char kDummyLeft_00754d1c[] = "dummyleft";
constexpr char kRHand_0074f334[] = "rhand";
constexpr char kLHand_0074f4b8[] = "lhand";
constexpr char kUnequip_00754d3c[] = "unequip";
constexpr char kEquip_00754d34[] = "equip";

// [item+10h] is the CSWCItem's CSWCObject subobject; its table is a CSWCObject-family table (m15e.h).
inline const KotorM15ObjectMethods_007516c8* ovt(const void* object) noexcept {
    return ld<const KotorM15ObjectMethods_007516c8*>(object, 0);
}
inline void* item_by_id(U id) noexcept(false) { return kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id); }
// (item+10h)->GetModel(FFh, 1) (object slot +98h): 00699B2E.., 0069A09B.., 0069ABA0..
inline void* item_model(void* item) noexcept(false) {
    void* object = at(item, 0x10);
    return ovt(object)->s098_get_model(object, nullptr, 0xFF, 1);
}
// gob->AttachToObject(0,0,0); gob->AttachToScene(0) (vslots +50h / +4Ch): detach sequence.
inline void detach(void* g) noexcept(false) {
    gvt(g)->s050_gob_attach_to_object(g, nullptr, nullptr, nullptr, 0);
    gvt(g)->attach_to_scene(gob(g), nullptr, nullptr);
}
// Head (this = head+50h) / Wield (this = wield+5Ch) / HeadWield (this = TW = obj+8) neighbour pointers.
inline void* hw_tw(void* self) noexcept { return at(self, vbe(self, -8, 1) - 8); }          // lea ecx,[vb1+this-8]
inline void* hw_head(void* self) noexcept { return at(self, vbe(self, -8, 2) + 0x48); }      // lea ecx,[vb2+this+48h]
inline void* hw_wield(void* self) noexcept { return at(self, vbe(self, -8, 3) + 0x54); }    // lea ecx,[vb3+this+54h]
inline void* hw_wield_base(void* self) noexcept { return at(self, vbe(self, -8, 3) - 8); }   // lea ecx,[vb3+this-8]
inline void* head_tw(void* self) noexcept { return at(self, vbe(self, -0x50, 1) - 0x50); }  // lea ecx,[vb1+this-50h]
inline void* wield_tw(void* self) noexcept { return at(self, vbe(self, -0x5C, 1) - 0x5C); } // lea ecx,[vb1+this-5Ch]
// NV thunk: sub ecx,[ecx-4] (vtordisp of the virtual base).
inline void* vtordisp_adjust(void* vbase) noexcept { return at(vbase, -static_cast<std::ptrdiff_t>(ld<I>(vbase, -4))); }
}  // namespace

extern "C" {
// ============================================================================================= CSWAnimBase
// 00698E00 CSWAnimBase::CSWAnimBase (RET 0): vfptr 00754AE4, [+4] = [+8] = 0.
void* __fastcall kotor_m15e_sw_anim_base_construct_00698e00(void* self, void*) noexcept(false) {
    st<const void*>(self, 0, &kotor_m15e_sw_anim_base_vtable_00754ae4);    // 00698E04
    st<U>(self, 4, 0);                                                     // 00698E0A
    st<U>(self, 8, 0);                                                     // 00698E0D
    return self;                                                           // 00698E00 mov eax,ecx
}
// 00698E20 CSWAnimBase::~CSWAnimBase (RET 0): restores vfptr 00754AE4.
void __fastcall kotor_m15e_sw_anim_base_destruct_00698e20(void* self, void*) noexcept(false) {
    st<const void*>(self, 0, &kotor_m15e_sw_anim_base_vtable_00754ae4);    // 00698E20
}
// 00698E30 CSWAnimBase::Set(index, value) RET 8.
kotor_m15e::U __fastcall kotor_m15e_sw_anim_base_set_00698e30(void* self, void*, kotor_m15e::U index, kotor_m15e::U value) noexcept(false) {
    if (index > 3) return 0;                                               // 00698E34..37 / 00698E7C
    st<U>(self, 4 + 4 * static_cast<std::ptrdiff_t>(index), value);         // 00698E40..00698E71 (table 00698E84)
    return 1;
}

// ============================================================================================= constructors
// 0069BB80 CSWCAnimBaseHead::CSWCAnimBaseHead(most_derived) RET 4 (this = head base). EH states 0069BBBE/0069BBC2.
void* __fastcall kotor_m15e_anim_base_head_construct_0069bb80(void* self, void*, kotor_m15e::U most_derived) noexcept(false) {
    if (most_derived != 0) {                                               // 0069BBA1..AE
        st<const void*>(self, 0, kotor_m15e_anim_base_head_vbtable_00754e40);   // 0069BBB3
        kotor_m15e_anim_base_tw_construct_0069cbd0(at(self, 0x50), nullptr);    // 0069BBB0..B9
    }
    st<const void*>(self, vbe(self, 0, 1), &kotor_m15e_anim_base_head_vtable_00754d48);   // 0069BBCA..CF
    st<I>(self, vbe(self, 0, 1) - 4, vbe(self, 0, 1) - 0x50);              // 0069BBD6..DE (vtordisp)
    Ref tmp;
    menu_resources::Ref* const ref1c = ref_at(self, 0x1C);
    c2da_default_ref_00405ed0(ref1c, nullptr);                             // 0069BBE2..E7
    c2da_default_ref_00405ed0(ref_at(self, 0x30), nullptr);                // 0069BBEC..F1
    st<U>(self, 0x44, 0);                                                  // 0069BBF8
    st<U8>(self, vbe(self, 0, 1) + 0xC4, 1);                               // 0069BBF6..0A
    unresolved_ref_assign_00406290(ref1c, nullptr, &tmp, kEmpty_0073d71c); // 0069BBFE..12
    st<U>(self, 0x2C, 0);                                                  // 0069BC17
    st<U8>(self, 0x18, 0);                                                 // 0069BC1A
    st<U8>(self, 0x19, 0);                                                 // 0069BC1D
    for (int k = 0; k < 5; ++k) st<U>(self, 4 + 4 * k, 0);                // 0069BC20..3C
    unresolved_ref_assign_00406290(ref_at(self, 0x30), nullptr, &tmp, kEmpty_0073d71c);   // 0069BC2D..3F
    st<U>(self, 0x40, 0);                                                  // 0069BC49
    st<U>(self, 0x48, 0x7F000000u);                                       // 0069BC4C mov [esi+48h],7F000000h
    return self;
}
// 00699DD0 CSWCAnimBaseWield::CSWCAnimBaseWield(most_derived) RET 4 (this = wield base). EH 00699E0E/00699E51.
void* __fastcall kotor_m15e_anim_base_wield_construct_00699dd0(void* self, void*, kotor_m15e::U most_derived) noexcept(false) {
    if (most_derived != 0) {                                               // 00699DF2..FE
        st<const void*>(self, 0, kotor_m15e_anim_base_wield_vbtable_00754d00);  // 00699E03
        kotor_m15e_anim_base_tw_construct_0069cbd0(at(self, 0x5C), nullptr);    // 00699E00..09
    }
    st<const void*>(self, vbe(self, 0, 1), &kotor_m15e_anim_base_wield_vtable_00754c08);  // 00699E1A..1F
    st<I>(self, vbe(self, 0, 1) - 4, vbe(self, 0, 1) - 0x5C);              // 00699E26..2E (vtordisp)
    c2da_default_ref_00405ed0(ref_at(self, 4), nullptr);                   // 00699E32..37
    c2da_default_ref_00405ed0(ref_at(self, 0x14), nullptr);                // 00699E3C..41
    ::new (at(self, 0x24)) StartupString();                                // 00699E46..49 (005B3190)
    ::new (at(self, 0x2C)) StartupString();                                // 00699E4E..59
    st<U>(self, 0x3C, kInvalidObject_00746644);                            // 00699E5E..64
    st<U>(self, 0x40, kInvalidObject_00746644);                            // 00699E67..6F
    st<U8>(self, vbe(self, 0, 1) + 0xC4, 2);                               // 00699E6D..77
    Ref tmp;
    st<U>(self, 0x44, 0);                                                  // 00699E8E
    unresolved_ref_assign_00406290(ref_at(self, 4), nullptr, &tmp, kEmpty_0073d71c);      // 00699E7E..91
    unresolved_ref_assign_00406290(ref_at(self, 0x14), nullptr, &tmp, kEmpty_0073d71c);   // 00699E96..A2
    st<U>(self, 0x34, 0);                                                  // 00699EAB
    st<U>(self, 0x38, 0);                                                  // 00699EAE
    st<U>(self, 0x48, 0);                                                  // 00699EB1
    st<U>(self, 0x4C, 0);                                                  // 00699EB4
    st<U>(self, 0x50, 0);                                                  // 00699EB7
    st<U>(self, 0x54, 0);                                                  // 00699EBA
    return self;
}
// 00698EC0 CSWCAnimBaseHeadWield::CSWCAnimBaseHeadWield(most_derived) RET 4 (this = 0x220 block). EH states 00698F1C/F35.
void* __fastcall kotor_m15e_anim_base_head_wield_construct_00698ec0(void* self, void*, kotor_m15e::U most_derived) noexcept(false) {
    if (most_derived != 0) {                                               // 00698ED8..ED
        st<const void*>(self, 0, &kotor_m15e_anim_base_vbtables_00754be0[4]);       // 00698F00 (00754BF0)
        st<const void*>(self, 0x17C, &kotor_m15e_anim_base_vbtables_00754be0[2]);   // 00698F06 (00754BE8)
        st<const void*>(self, 0x1C8, &kotor_m15e_anim_base_vbtables_00754be0[0]);   // 00698F0C (00754BE0)
        kotor_m15e_anim_base_tw_construct_0069cbd0(at(self, 8), nullptr);  // 00698EFD..F13
        kotor_m15e_anim_base_head_construct_0069bb80(at(self, 0x17C), nullptr, 0);    // 00698F18..2C
        kotor_m15e_anim_base_wield_construct_00699dd0(at(self, 0x1C8), nullptr, 0);   // 00698F31..45
    }
    st<const void*>(self, vbe(self, 0, 1), &kotor_m15e_anim_base_head_wield_vtable_00754ae8);   // 00698F4C..51
    st<I>(self, vbe(self, 0, 1) - 4, vbe(self, 0, 1) - 8);                 // 00698F58..60 (vtordisp 0)
    st<U8>(self, vbe(self, 0, 1) + 0xC4, 3);                               // 00698F64..6D
    return self;
}

// ============================================================================================= destructors
// 0069B030 CSWCAnimBaseHead::~CSWCAnimBaseHead (RET 0; this = head+50h): vfptr 00754D48, vtordisp, then the
// virtual UnloadModel() through the Head table (+78h = NV thunk 0069BB00 -> 0069B610).
void __fastcall kotor_m15e_anim_base_head_destruct_0069b030(void* self, void*) noexcept(false) {
    st<const void*>(self, vbe(self, -0x50, 1) - 0x50, &kotor_m15e_anim_base_head_vtable_00754d48);   // 0069B030..36
    st<I>(self, vbe(self, -0x50, 1) - 0x54, vbe(self, -0x50, 1) - 0x50);   // 0069B03E..47
    void* const tw = at(self, vbe(self, -0x50, 1) - 0x50);                 // 0069B04B..55
    avt(tw)->s078(tw, nullptr);                                            // 0069B059 jmp [eax+78h]
}
// 0069BB00 NVThunkHeadAnimBase_UnloadModel (Head table +78h): sub ecx,[ecx-4]; jmp 0069B610.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_thunk_unload_model_0069bb00(void* vbase, void*) noexcept(false) {
    return kotor_m15e_anim_base_head_unload_model_0069b610(vtordisp_adjust(vbase), nullptr);
}
// 00699ED0 CSWCAnimBaseWield::~CSWCAnimBaseWield (RET 0; this = wield+5Ch). EH 00699F10.., 00699F87.
void __fastcall kotor_m15e_anim_base_wield_destruct_00699ed0(void* self, void*) noexcept(false) {
    st<const void*>(self, vbe(self, -0x5C, 1) - 0x5C, &kotor_m15e_anim_base_wield_vtable_00754c08);   // 00699EEA..F0
    st<I>(self, vbe(self, -0x5C, 1) - 0x60, vbe(self, -0x5C, 1) - 0x5C);   // 00699EF8..F05
    if (pv(self, -8) != nullptr) {                                         // 00699F09..18 CExoSoundSource
        delete_object(pv(self, -8));                                       // 00699F1A..1E [edx](1)
        st<void*>(self, -8, nullptr);                                      // 00699F20
    }
    for (std::ptrdiff_t off : {static_cast<std::ptrdiff_t>(-0x28), static_cast<std::ptrdiff_t>(-0x24)}) {   // 00699F23 / 00699F4B
        void* const g = pv(self, off);
        if (g == nullptr) continue;
        detach(g);                                                         // 00699F2A..38 / 00699F52..60
        void* const again = pv(self, off);                                 // 00699F3B / 00699F63
        if (again != nullptr) gvt(again)->s000_gob_destruct_gob(gob(again), nullptr, 1);   // 00699F42..46 / 00699F6A..6E
        st<void*>(self, off, nullptr);                                     // 00699F48 / 00699F70
    }
    void* const tw = at(self, vbe(self, -0x5C, 1) - 0x5C);                 // 00699F73..7D
    avt(tw)->s078(tw, nullptr);                                            // 00699F81 call [edx+78h] (Wield table: 0069C5D0)
    str_at(self, -0x30)->~StartupString();                                 // 00699F84..8B
    str_at(self, -0x38)->~StartupString();                                 // 00699F90..9B
}
// 00699D70 CSWCAnimBaseHeadWield::~CSWCAnimBaseHeadWield (RET 0; this = 0x220 block).
void __fastcall kotor_m15e_anim_base_head_wield_destruct_00699d70(void* self, void*) noexcept(false) {
    void* const tw8 = at(self, 8);                                         // 00699D79 lea esi,[edi+8]
    st<const void*>(self, vbe(self, 0, 1), &kotor_m15e_anim_base_head_wield_vtable_00754ae8);   // 00699D74..7C
    st<I>(self, vbe(self, 0, 1) - 4, vbe(self, 0, 1) - 8);                 // 00699D84..8D
    void* const tw = at(tw8, vbe(self, 0, 1) - 8);                         // 00699D91..9B
    avt(tw)->s078(tw, nullptr);                                            // 00699D9F call [edx+78h] (00754AE8: thunk 006999C0)
    kotor_m15e_anim_base_wield_destruct_00699ed0(at(self, 0x224), nullptr);   // 00699DA2..A8
    kotor_m15e_anim_base_head_destruct_0069b030(at(self, 0x1CC), nullptr);    // 00699DAD..B3
    kotor_m15e_anim_base_tw_destruct_0069cd10(tw8, nullptr);               // 00699DB9..BC jmp
}
// 00699890 NVThunkHeadWieldAnimBase_destructor (00754AE8 +00) -> 00699D40 scalar deleting dtor (RET 4).
void* __fastcall kotor_m15e_anim_base_head_wield_thunk_destructor_00699890(void* vbase, void*, kotor_m15e::U flags) noexcept(false) {
    void* const self = at(vtordisp_adjust(vbase), -8);                     // 00699890 sub ecx,[ecx-4]; 00699D41 lea esi,[ecx-8]
    kotor_m15e_anim_base_head_wield_destruct_00699d70(self, nullptr);      // 00699D46
    if ((flags & 1u) != 0) live_free(self);                                // 00699D4B..58
    return self;                                                           // 00699D5B
}

// ============================================================================================= HeadWield members
// (this = TW subobject, vbptr at [this-8]); entries reached through the 00754AE8 NV thunks.
// 00698F90 GetModel(part) RET 4: F4..FB/FF -> Wield, FE -> Head, FC/FD -> 0 (table 00698FDC/00698FE8).
void* __fastcall kotor_m15e_anim_base_head_wield_get_model_00698f90(void* self, void*, kotor_m15e::U part) noexcept(false) {
    const U idx = static_cast<U>(static_cast<U8>(part)) + 0xFFFFFF0Cu;     // 00698F94..97
    if (idx > 0x0B) return nullptr;                                        // 00698F9D..A0 / 00698FD6
    if (idx == 10) return kotor_m15e_anim_base_head_get_model_0069b060(hw_head(self), nullptr, part);   // 00698FC3..D1
    if (idx == 8 || idx == 9) return nullptr;                              // 00698FD6
    return kotor_m15e_anim_base_wield_get_model_0069aa90(hw_wield(self), nullptr, part);                // 00698FB0..BE
}
// 00699000 LoadModel(resref, part) RET 8.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_load_model_00699000(void* self, void*, void* resref, kotor_m15e::U part) noexcept(false) {
    if (static_cast<U8>(part) == 0xFE) {                                   // 00699000..09
        if (kotor_m15e_anim_base_head_load_model_0069b0a0(hw_head(self), nullptr, resref, 0xFE) == 0) return 0;   // 0069900B..26
        kotor_m15e_anim_base_wield_reattach_wielded_objects_0069af10(hw_wield_base(self), nullptr);           // 00699028..32
        return 1;
    }
    if (kotor_m15e_anim_base_wield_load_model_0069afe0(hw_wield(self), nullptr, resref, part) == 0) return 0;  // 00699040..57
    kotor_m15e_anim_base_wield_reattach_wielded_objects_0069af10(hw_wield_base(self), nullptr);               // 00699059..63
    return 1;
}
// 00699080 ReplaceTexture(part, from, to, keep) RET 10h.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_replace_texture_00699080(void* self, void*, kotor_m15e::U part,
    kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U keep) noexcept(false) {
    const U8 p = static_cast<U8>(part);                                    // 00699084
    if (p == 0xFE) return kotor_m15e_anim_base_head_replace_texture_0069b250(hw_head(self), nullptr, part, from, to, keep);   // 006990A9..B7
    if (p == 0xFF) return kotor_m15e_anim_base_tw_replace_texture_0069bf30(hw_tw(self), nullptr, part, from, to, keep);       // 00699096..A4
    return 0;                                                              // 00699091
}
// 006990C0 ReplaceTexture(from, to, keep) RET 0Ch.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_replace_texture_006990c0(void* self, void*, kotor_m15e::U from,
    kotor_m15e::U to, kotor_m15e::U keep) noexcept(false) {
    if (kotor_m15e_anim_base_tw_replace_texture_0069be20(hw_tw(self), nullptr, from, to, keep) == 0) return 0;      // 006990D2..E6
    if (kotor_m15e_anim_base_head_replace_texture_0069b1c0(hw_head(self), nullptr, from, to, keep) == 0) return 0;  // 006990E8..FC
    return 1;                                                              // 00699101
}
// 00699120 ReplaceTexture(part, from, to, count, attrs, keep, first) RET 1Ch (FE: Head, then returns 0 at 00699172).
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_replace_texture_00699120(void* self, void*, kotor_m15e::U part,
    kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U count, kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first) noexcept(false) {
    const U8 p = static_cast<U8>(part);                                    // 00699124
    if (p == 0xFE) {                                                       // 0069912C
        kotor_m15e_anim_base_head_replace_texture_0069b310(hw_head(self), nullptr, part, from, to, count, attrs, keep, first);   // 00699144..6D
        return 0;                                                          // 00699172
    }
    if (p == 0xFF) return kotor_m15e_anim_base_tw_replace_texture_0069c090(hw_tw(self), nullptr, part, from, to, count, attrs, keep, first);   // 00699131..3F
    return 0;                                                              // 00699172
}
// 00699210 RestoreTexture(part) RET 4 -> Head 0069B570.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_restore_texture_00699210(void* self, void*, kotor_m15e::U part) noexcept(false) {
    return kotor_m15e_anim_base_head_restore_texture_0069b570(hw_head(self), nullptr, part);   // 00699210..1A
}
// 00699220 UnloadModel() RET 0: TW::UnloadModel, then Head::UnloadModel (jmp).
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_unload_model_00699220(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_tw_unload_model_0069c5d0(hw_tw(self), nullptr);   // 00699223..2D
    return kotor_m15e_anim_base_head_unload_model_0069b610(hw_head(self), nullptr);   // 00699232..3D
}
// 00699250 UnloadModel(part) RET 4: FF -> TW 0069C640(FFh), else Head 0069B650.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_unload_model_00699250(void* self, void*, kotor_m15e::U part) noexcept(false) {
    if (static_cast<U8>(part) == 0xFF)                                      // 00699254..56
        return kotor_m15e_anim_base_tw_unload_model_0069c640(hw_tw(self), nullptr, 0xFF);   // 00699258..6A ([esp+4] = 0FFh)
    return kotor_m15e_anim_base_head_unload_model_0069b650(hw_head(self), nullptr, part);    // 0069926F..7D
}
// 00699290 Wield(...) RET 10h -> Wield::Wield 0069AB80.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_wield_00699290(void* self, void*, kotor_m15e::U item_id, void* hook,
    kotor_m15e::U hand, kotor_m15e::U scale) noexcept(false) {
    return kotor_m15e_anim_base_wield_wield_0069ab80(hw_wield(self), nullptr, item_id, hook, hand, scale);   // 00699290..9A
}
// 006992A0 HideWieldedItems(right, left) RET 8 / 006992B0 ShowWieldedItems() / 006992C0 SetWieldLimited(v) RET 4.
void __fastcall kotor_m15e_anim_base_head_wield_hide_wielded_items_006992a0(void* self, void*, void* right, void* left) noexcept(false) {
    kotor_m15e_anim_base_wield_hide_wielded_items_0069a6b0(hw_wield(self), nullptr, right, left);   // 006992A0..AA
}
void __fastcall kotor_m15e_anim_base_head_wield_show_wielded_items_006992b0(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_wield_show_wielded_items_0069a010(hw_wield(self), nullptr);   // 006992B0..BA
}
void __fastcall kotor_m15e_anim_base_head_wield_set_wield_limited_006992c0(void* self, void*, kotor_m15e::U limited) noexcept(false) {
    kotor_m15e_anim_base_wield_set_wield_limited_00699fc0(hw_wield(self), nullptr, limited);   // 006992C0..CA
}
// 00699330 SetEnvMap(part, a2, a3, resref) RET 10h: FE -> Head 0069B720(FEh), else TW 0069C790.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_set_env_map_00699330(void* self, void*, kotor_m15e::U part,
    kotor_m15e::U a2, kotor_m15e::U a3, void* resref) noexcept(false) {
    if (static_cast<U8>(part) == 0xFE)                                      // 00699334..36
        return kotor_m15e_anim_base_head_set_env_map_0069b720(hw_head(self), nullptr, 0xFE, a2, a3, resref);   // 00699338..4A
    return kotor_m15e_anim_base_tw_set_env_map_0069c790(hw_tw(self), nullptr, part, a2, a3, resref);         // 0069934F..5D
}
// 00699420 SetOverlayAnimation(animation, flag) RET 8: body gob then head gob through CSWCAnimBase 0069E420.
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_set_overlay_animation_00699420(void* self, void*, kotor_m15e::U animation,
    kotor_m15e::U flag) noexcept(false) {
    const I vb1 = vbe(self, -8, 1);                                        // 00699423..26
    void* const body = pv(self, vb1 + 0xB0);                               // 00699429 [ecx+esi+B0h]
    if (body == nullptr) {                                                 // 00699432..35
        if (pv(self, vbe(self, -8, 2) + 0x3C) == nullptr) return 0;        // 00699437..45
    }
    void* const tw = at(self, vb1 - 8);                                    // 00699430 add ecx,esi; 00699455 add ecx,-8
    if (kotor_m15e_anim_base_set_overlay_animation_0069e420(tw, nullptr, body, animation, flag) == 0) return 0;   // 00699449..5F
    void* const head_gob = pv(self, vbe(self, -8, 2) + 0x3C);              // 00699461..67
    if (kotor_m15e_anim_base_set_overlay_animation_0069e420(hw_tw(self), nullptr, head_gob, animation, flag) == 0) return 0;   // 0069946B..7C
    return 1;                                                              // 00699480
}
// 00699560 SetAlwaysRender(on) RET 4 / 006995D0 SetInCutscene(on) RET 4: body and head gobs (+130/+134, +138/+13C).
// EAX: callers (CSWCObject::SetAnimation 0063EA49/0063EAD1) discard it; D2 (EAX leftover) on the skip paths.
void __fastcall kotor_m15e_anim_base_head_wield_set_always_render_00699560(void* self, void*, kotor_m15e::U on) noexcept(false) {
    void* const body = pv(self, vbe(self, -8, 1) + 0xB0);                  // 00699569..6F
    if (on != 0) {                                                         // 00699567 / 00699576
        if (body != nullptr) gvt(body)->s130_gob_enable_always_render(gob(body), nullptr);   // 00699578..7E
        void* const head = pv(self, vbe(self, -8, 2) + 0x3C);              // 00699584..8A
        if (head != nullptr) gvt(head)->s130_gob_enable_always_render(gob(head), nullptr);   // 0069958E..94
        return;
    }
    if (body != nullptr) gvt(body)->s134_gob_disable_always_render(gob(body), nullptr);      // 0069959E..A4
    void* const head = pv(self, vbe(self, -8, 2) + 0x3C);                  // 006995AA..B0
    if (head != nullptr) gvt(head)->s134_gob_disable_always_render(gob(head), nullptr);      // 006995B4..BA
    return;
}
void __fastcall kotor_m15e_anim_base_head_wield_set_in_cutscene_006995d0(void* self, void*, kotor_m15e::U on) noexcept(false) {
    void* const body = pv(self, vbe(self, -8, 1) + 0xB0);                  // 006995D9..DF
    if (on != 0) {                                                         // 006995D7 / 006995E6
        if (body != nullptr) gvt(body)->s138_gob_enable_in_cutscene(gob(body), nullptr);     // 006995E8..EE
        void* const head = pv(self, vbe(self, -8, 2) + 0x3C);              // 006995F4..FA
        if (head != nullptr) gvt(head)->s138_gob_enable_in_cutscene(gob(head), nullptr);     // 006995FE..604
        return;
    }
    if (body != nullptr) gvt(body)->s13c_gob_disable_in_cutscene(gob(body), nullptr);        // 0069960E..14
    void* const head = pv(self, vbe(self, -8, 2) + 0x3C);                  // 0069961A..20
    if (head != nullptr) gvt(head)->s13c_gob_disable_in_cutscene(gob(head), nullptr);        // 00699624..2A
    return;
}
// 00699790 TurnOnShadows() RET 0: Head 0069B810 then Wield 0069A170 (jmp).
void __fastcall kotor_m15e_anim_base_head_wield_turn_on_shadows_00699790(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_head_turn_on_shadows_0069b810(hw_head(self), nullptr);   // 00699793..9D
    kotor_m15e_anim_base_wield_turn_on_shadows_0069a170(hw_wield(self), nullptr);   // 006997A2..AD
}
// 00699830 EnableDistortion() / 00699860 DisableDistortion(): Head then Wield (jmp).
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_enable_distortion_00699830(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_head_enable_distortion_0069b950(hw_head(self), nullptr);   // 00699833..3D
    return kotor_m15e_anim_base_wield_enable_distortion_0069a8f0(hw_wield(self), nullptr);   // 00699842..4D
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_head_wield_disable_distortion_00699860(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_head_disable_distortion_0069b9c0(hw_head(self), nullptr);   // 00699863..6D
    return kotor_m15e_anim_base_wield_disable_distortion_0069a980(hw_wield(self), nullptr);   // 00699872..7D
}
// 00699CE0 SetHeadItem(id) RET 4: [vb2+this+40h] = head+48h.
void __fastcall kotor_m15e_anim_base_head_wield_set_head_item_00699ce0(void* self, void*, kotor_m15e::U id) noexcept(false) {
    st<U>(self, vbe(self, -8, 2) + 0x40, id);                              // 00699CE0..EA (EAX = id; caller 00696BB6 discards it)
}

// ============================================================================================= NV thunks (00754AE8)
// Each: sub ecx,[ecx-4]; jmp <HeadWield member>; the thunk types are the KotorM15eAnimBaseVtbl slot types.
void* __fastcall kotor_m15e_anim_base_hw_thunk_get_model_006998a0(void* vbase, void*, kotor_m15e::U part) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_get_model_00698f90(vtordisp_adjust(vbase), nullptr, part);   // 006998A0..A3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_load_model_006998b0(void* vbase, void*, void* resref, kotor_m15e::U part) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_load_model_00699000(vtordisp_adjust(vbase), nullptr, resref, part);   // 006998B0..B3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_replace_texture_006998d0(void* vbase, void*, kotor_m15e::U part, kotor_m15e::U from,
    kotor_m15e::U to, kotor_m15e::U count, kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_replace_texture_00699120(vtordisp_adjust(vbase), nullptr, part, from, to, count, attrs, keep, first);   // 006998D0..D3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_replace_texture_006998e0(void* vbase, void*, kotor_m15e::U from, kotor_m15e::U to,
    kotor_m15e::U keep) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_replace_texture_006990c0(vtordisp_adjust(vbase), nullptr, from, to, keep);   // 006998E0..E3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_replace_texture_006998f0(void* vbase, void*, kotor_m15e::U part, kotor_m15e::U from,
    kotor_m15e::U to, kotor_m15e::U keep) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_replace_texture_00699080(vtordisp_adjust(vbase), nullptr, part, from, to, keep);   // 006998F0..F3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_restore_texture_00699900(void* vbase, void*, kotor_m15e::U part) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_restore_texture_00699210(vtordisp_adjust(vbase), nullptr, part);   // 00699900..03
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_set_overlay_animation_00699950(void* vbase, void*, kotor_m15e::U animation,
    kotor_m15e::U flag) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_set_overlay_animation_00699420(vtordisp_adjust(vbase), nullptr, animation, flag);   // 00699950..53
}
void __fastcall kotor_m15e_anim_base_hw_thunk_set_always_render_00699990(void* vbase, void*, kotor_m15e::U on) noexcept(false) {
    kotor_m15e_anim_base_head_wield_set_always_render_00699560(vtordisp_adjust(vbase), nullptr, on);   // 00699990..93
}
void __fastcall kotor_m15e_anim_base_hw_thunk_set_in_cutscene_006999a0(void* vbase, void*, kotor_m15e::U on) noexcept(false) {
    kotor_m15e_anim_base_head_wield_set_in_cutscene_006995d0(vtordisp_adjust(vbase), nullptr, on);   // 006999A0..A3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_unload_model_006999b0(void* vbase, void*, kotor_m15e::U part) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_unload_model_00699250(vtordisp_adjust(vbase), nullptr, part);   // 006999B0..B3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_unload_model_006999c0(void* vbase, void*) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_unload_model_00699220(vtordisp_adjust(vbase), nullptr);   // 006999C0..C3
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_wield_006999d0(void* vbase, void*, kotor_m15e::U item_id, void* hook,
    kotor_m15e::U hand, kotor_m15e::U scale) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_wield_00699290(vtordisp_adjust(vbase), nullptr, item_id, hook, hand, scale);   // 006999D0..D3
}
void __fastcall kotor_m15e_anim_base_hw_thunk_hide_wielded_items_006999e0(void* vbase, void*, void* right, void* left) noexcept(false) {
    kotor_m15e_anim_base_head_wield_hide_wielded_items_006992a0(vtordisp_adjust(vbase), nullptr, right, left);   // 006999E0..E3
}
void __fastcall kotor_m15e_anim_base_hw_thunk_show_wielded_items_006999f0(void* vbase, void*) noexcept(false) {
    kotor_m15e_anim_base_head_wield_show_wielded_items_006992b0(vtordisp_adjust(vbase), nullptr);   // 006999F0..F3
}
void __fastcall kotor_m15e_anim_base_hw_thunk_set_wield_limited_00699a00(void* vbase, void*, kotor_m15e::U limited) noexcept(false) {
    kotor_m15e_anim_base_head_wield_set_wield_limited_006992c0(vtordisp_adjust(vbase), nullptr, limited);   // 00699A00..03
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_set_env_map_00699a10(void* vbase, void*, kotor_m15e::U part, kotor_m15e::U a2,
    kotor_m15e::U a3, void* resref) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_set_env_map_00699330(vtordisp_adjust(vbase), nullptr, part, a2, a3, resref);   // 00699A10..13
}
void __fastcall kotor_m15e_anim_base_hw_thunk_turn_on_shadows_00699a40(void* vbase, void*) noexcept(false) {
    kotor_m15e_anim_base_head_wield_turn_on_shadows_00699790(vtordisp_adjust(vbase), nullptr);   // 00699A40..43
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_enable_distortion_00699a90(void* vbase, void*) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_enable_distortion_00699830(vtordisp_adjust(vbase), nullptr);   // 00699A90..93
}
kotor_m15e::U __fastcall kotor_m15e_anim_base_hw_thunk_disable_distortion_00699aa0(void* vbase, void*) noexcept(false) {
    return kotor_m15e_anim_base_head_wield_disable_distortion_00699860(vtordisp_adjust(vbase), nullptr);   // 00699AA0..A3
}
void __fastcall kotor_m15e_anim_base_hw_thunk_set_head_item_00699d30(void* vbase, void*, kotor_m15e::U id) noexcept(false) {
    kotor_m15e_anim_base_head_wield_set_head_item_00699ce0(vtordisp_adjust(vbase), nullptr, id);   // 00699D30..33
}
}
