#pragma once
// lane-m15-e (L15-E): CSWCAnimBase family (CSWAnimBase 00698E00, CSWCAnimBase 0069DFB0, CSWCAnimBaseTW 0069CBD0,
// CSWCAnimBaseHead 0069BB80, CSWCAnimBaseWield 00699DD0, CSWCAnimBaseHeadWield 00698EC0) as reached from
// CSWCCreature::LoadModel 0061B380 and the creature/object slot bodies. Original: swkotor.exe GOG K1.
//
// The creature's anim base pointer [creature+68h] points at the CSWCAnimBase(TW) subobject whose first word is
// the vfptr (for CSWCAnimBaseHeadWield: subobject at +8 of the 0x220 allocation, table 00754AE8). All calls
// from creature/object code go through KotorM15eAnimBaseVtbl (layout of 00754AE8 / 00754F60 / 00754E58, 62
// slots); member names are sNNN (hex byte offset). Conventions as m15e.h (D1 fastcall veneers, typed tables).
#include <cstdint>
#include "8fe1a9cf4a-m15e.h"

extern "C" {
struct KotorM15eAnimBaseVtbl {
    void* (__fastcall* s000)(void* self, void* edx, kotor_m15e::U flags);                         // scalar deleting dtor, RET 4
    kotor_m15e::U (__fastcall* s004)(void* self, void* edx, void* scene);                         // AttachToScene RET 4
    void* (__fastcall* s008)(void* self, void* edx, kotor_m15e::U part);                          // GetModel(part) RET 4 -> Gob*
    kotor_m15e::U (__fastcall* s00c)(void* self, void* edx, void* resref, kotor_m15e::U part);    // LoadModel RET 8
    kotor_m15e::U (__fastcall* s010)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // ReplaceTexture RET 18h
    kotor_m15e::U (__fastcall* s014)(void* self, void* edx, kotor_m15e::U part, kotor_m15e::U from, kotor_m15e::U to, kotor_m15e::U count,
        kotor_m15e::U attrs, kotor_m15e::U keep, kotor_m15e::U first);                                   // ReplaceTexture RET 1Ch
    kotor_m15e::U (__fastcall* s018)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);  // ReplaceTexture RET 0Ch
    kotor_m15e::U (__fastcall* s01c)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // ReplaceTexture RET 10h
    kotor_m15e::U (__fastcall* s020)(void*, void*, kotor_m15e::U);                                // RestoreTexture RET 4
    kotor_m15e::U (__fastcall* s024)(void*, void*);                                               // RestoreTexture
    kotor_m15e::U (__fastcall* s028)(void*, void*, kotor_m15e::U, kotor_m15e::U);                 // EnableBumpedOutShell RET 8
    kotor_m15e::U (__fastcall* s02c)(void*, void*);                                               // DisableBumpedOutShell
    kotor_m15e::U (__fastcall* s030)(void*, void*, kotor_m15e::U);                                // SetMotionBlur RET 4
    kotor_m15e::U (__fastcall* s034)(void*, void*, kotor_m15e::U);                                // SetMaxMotionBlurLength RET 4
    float (__fastcall* s038)(void* self, void* edx, kotor_m15e::U animation);                     // GetAnimationLength RET 4, ST0
    float (__fastcall* s03c)(void* self, void* edx, kotor_m15e::U animation);                     // GetAnimationTime RET 4, ST0
    void* (__fastcall* s040)(void* self, void* edx, void* out_string, kotor_m15e::U animation);   // GetAnimationName RET 8
    kotor_m15e::U (__fastcall* s044)(void* self, void* edx, kotor_m15e::U animation, kotor_m15e::U a2);   // SetOverlayAnimation RET 8
    kotor_m15e::U (__fastcall* s048)(void*, void*, kotor_m15e::U);                                // GetOverlayAnimation RET 4
    kotor_m15e::U (__fastcall* s04c)(void*, void*, kotor_m15e::U);                                // GetOverlayAnimationPlaying RET 4
    kotor_m15e::U (__fastcall* s050)(void* self, void* edx, kotor_m15e::U animation);             // AnimationExists RET 4
    kotor_m15e::U (__fastcall* s054)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // SetAnimation RET 10h
    kotor_m15e::U (__fastcall* s058)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // SetAnimationOnSubObject RET 14h
    kotor_m15e::U (__fastcall* s05c)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // SetAnimationVisemes RET 10h
    kotor_m15e::U (__fastcall* s060)(void* self, void* edx, const float* quaternion);             // SetOrientation(Quaternion) RET 4
    kotor_m15e::U (__fastcall* s064)(void* self, void* edx, const float* vector3);                // SetOrientation(Vector) RET 4
    kotor_m15e::U (__fastcall* s068)(void* self, void* edx, const float* vector3);                // SetPosition RET 4
    void (__fastcall* s06c)(void* self, void* edx, kotor_m15e::U on);                    // SetAlwaysRender RET 4
    void (__fastcall* s070)(void* self, void* edx, kotor_m15e::U on);                    // SetInCutscene RET 4
    kotor_m15e::U (__fastcall* s074)(void* self, void* edx, kotor_m15e::U part);                  // UnloadModel(part) RET 4
    kotor_m15e::U (__fastcall* s078)(void* self, void* edx);                                      // UnloadModel()
    kotor_m15e::U (__fastcall* s07c)(void* self, void* edx, kotor_m15e::U item_id, void* hook_name, kotor_m15e::U hand, kotor_m15e::U scale);   // Wield RET 10h
    void (__fastcall* s080)(void* self, void* edx, void* right_ref, void* left_ref);              // HideWieldedItems RET 8 (EAX discarded)
    void (__fastcall* s084)(void* self, void* edx);                                               // ShowWieldedItems (EAX discarded)
    void (__fastcall* s088)(void*, void*);                                               // HideHeadItem
    void (__fastcall* s08c)(void*, void*);                                               // ShowHeadItem
    void (__fastcall* s090)(void* self, void* edx, kotor_m15e::U limited);                        // SetWieldLimited RET 4 (= L15-A view)
    kotor_m15e::U (__fastcall* s094)(void*, void*, kotor_m15e::U, kotor_m15e::U);                 // SetAlpha RET 8
    kotor_m15e::U (__fastcall* s098)(void* self, void* edx, kotor_m15e::U part, kotor_m15e::U a2, kotor_m15e::U a3, void* resref);   // SetEnvMap RET 10h
    kotor_m15e::U (__fastcall* s09c)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);  // SetEnvMap RET 0Ch
    kotor_m15e::U (__fastcall* s0a0)(void*, void*);                                               // RestoreEnvMap
    kotor_m15e::U (__fastcall* s0a4)(void* self, void* edx, void* name_string);                   // ApplyShadowBlob RET 4 -> 0/1
    kotor_m15e::U (__fastcall* s0a8)(void* self, void* edx);                                      // RemoveShadowBlob
    kotor_m15e::U (__fastcall* s0ac)(void*, void*, kotor_m15e::U);                                // ShowShadowBlob RET 4
    kotor_m15e::U (__fastcall* s0b0)(void* self, void* edx);                                      // HideShadowBlob
    void (__fastcall* s0b4)(void* self, void* edx, const float* quaternion);                      // SetShadowBlobOrientation RET 4 (= L15-A view)
    kotor_m15e::U (__fastcall* s0b8)(void* self, void* edx);                                      // TurnOnShadows
    kotor_m15e::U (__fastcall* s0bc)(void* self, void* edx);                                      // TurnOffShadows
    void* (__fastcall* s0c0)(void*, void*, void*);                                                // GetOrientation RET 4
    void (__fastcall* s0c4)(void*, void*, kotor_m15e::U);                                // UpdateSound RET 4
    kotor_m15e::U (__fastcall* s0c8)(void*, void*, kotor_m15e::U);                                // AddAnimationFlag RET 4
    kotor_m15e::U (__fastcall* s0cc)(void*, void*, kotor_m15e::U);                                // RemoveAnimationFlag RET 4
    void (__fastcall* s0d0)(void*, void*, kotor_m15e::U);                                // SetLean RET 4
    void (__fastcall* s0d4)(void*, void*);                                               // Lean
    kotor_m15e::U (__fastcall* s0d8)(void*, void*, void*);                                                // GetBoundingRect RET 4
    std::uint8_t (__fastcall* s0dc)(void*, void*, void*);                                                // GetCenterPoint RET 4
    kotor_m15e::U (__fastcall* s0e0)(void* self, void* edx, kotor_m15e::U animation);             // ServerToClientAnimation RET 4
    kotor_m15e::U (__fastcall* s0e4)(void* self, void* edx);                                      // EnableDistortion
    kotor_m15e::U (__fastcall* s0e8)(void* self, void* edx);                                      // DisableDistortion
    void (__fastcall* s0ec)(void* self, void* edx, kotor_m15e::U item_id);                        // SetHeadItem RET 4 (= L15-A view)
    kotor_m15e::U (__fastcall* s0f0)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // SetAnimationInternal RET 14h
    kotor_m15e::U (__fastcall* s0f4)(void*, void*, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U, kotor_m15e::U);   // SetAnimationOnSubObjectInternal RET 18h
};
static_assert(sizeof(KotorM15eAnimBaseVtbl) == 62 * 4, "anim-base vtable 00754AE8..00754BDF");

// Constructors / helpers called directly by CSWCCreature::LoadModel 0061B380 (ECX = new block).
void* __fastcall kotor_m15e_anim_base_construct_0069dfb0(void* self, void*) noexcept(false);                    // RET 0, EAX=this
void* __fastcall kotor_m15e_anim_base_tw_construct_0069cbd0(void* self, void*) noexcept(false);                 // RET 0
void* __fastcall kotor_m15e_anim_base_head_construct_0069bb80(void* self, void*, kotor_m15e::U most_derived) noexcept(false);   // RET 4
void* __fastcall kotor_m15e_anim_base_wield_construct_00699dd0(void* self, void*, kotor_m15e::U most_derived) noexcept(false);  // RET 4
void* __fastcall kotor_m15e_anim_base_head_wield_construct_00698ec0(void* self, void*, kotor_m15e::U most_derived) noexcept(false);   // RET 4
// 00698E30 CSWAnimBase::Set(index, value) RET 8: index 0..3 -> [this+4/8/C/10h] = value, returns 1; else 0.
kotor_m15e::U __fastcall kotor_m15e_sw_anim_base_set_00698e30(void* self, void*, kotor_m15e::U index, kotor_m15e::U value) noexcept(false);
}
