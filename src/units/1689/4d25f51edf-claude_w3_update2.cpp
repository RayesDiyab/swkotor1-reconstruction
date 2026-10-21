// claude_w3_update2.cpp - client GameObjectUpdate leaves that 1204-16 / srcw3 w3_traps_objects.cpp still trapped, plus the
// callees no linked object provides. Original: swkotor.exe GOG K1 (image base 0x400000), read-only static disassembly;
// every statement cites the original instruction range; {E-x} = stack slot at entry ESP - x.
//   0064F430 TriggerUpdate_Update   0064F960 TriggerUpdate_Delete   0064D3A0 DoorUpdate_Delete
//   0064DA20 PlaceableUpdate_Delete 0064CC90 CreatureUpdate_Delete  0064EAC0 ItemUpdate_Delete
//   0064EB90 ItemUpdate_Update      00651AF0 CreatureUpdate_StripEquippedItems 00651B30 ItemUpdate_DestroyItem
//   00654B10 ItemScript_Add         0064C3C0 CreatureUpdate_VisEffect
//   006517B0 AreaOfEffectUpdate_Delete 006517F0 AreaOfEffectUpdate_Update
// new callees: 005ED610 / 005F1360 GetAreaOfEffectByGameObjectID, 006D9350 CSWCAreaOfEffectObject::StopEffect,
//   0060E010 CSWCCreature::StripEquippedItems, 00691D80 CSWCTrigger::RemoveFromRenderList, 0049D7D0 AurPartTriggerDelete,
//   0046D1E0 SceneRemovePartFromRooms.
// NOT here: 00651680 AreaOfEffectUpdate_Add (needs the whole CSWCAreaOfEffectObject class: vtable 00757E28, ctor 006D8310,
//   LoadEffect 006D8490 (3040 bytes) - see the delivery report); its stub stays.
#include "a1bf7fc0cb-w3_client_t3.h"

namespace w2 { namespace g07 { unsigned int is_beam_type_004ca960(unsigned int type); } }   // 004CA960 GetIsBeam (w2_g07_object.obj)

extern "C" {
// ------------------------------------------------------------------ existing providers (c13 map object noted)
void* __fastcall kotor_fastcall_005ed5d0(void* app, void*, std::uint32_t id);                       // 005ED5D0 GetTriggerByGameObjectID RET 4 (r5_client_trigger_lookup)
void* __fastcall kotor_fastcall_005ed580(void* app, void*, std::uint32_t id);                       // 005ED580 GetGameObject RET 4 (0701)
std::uint8_t __fastcall area_vfx_register_existing_004d80e0(void* array, void*, std::uint32_t* id, void* object,
                                                            std::uint32_t flag);                    // 004D80E0 CGameObjectArray::AddInternalObject RET 0Ch
void* __fastcall kotor_m15a_creature_get_server_creature_0060fb20(void* creature, void*);         // 0060FB20 (0816)
void __fastcall kotor_area_add_corpse_006056b0(void* area, void*, std::uint32_t id);               // 006056B0 CSWCArea::AddCorpse RET 4
void __fastcall kotor_fastcall_006911b0(void* trigger, void*, std::uint32_t animation, std::uint32_t a,
                                        std::uint32_t b);                                           // 006911B0 CSWCTrigger::AddFireAndForgetAnimation RET 0Ch (0833)
void __fastcall area_vfx_play_00690510(void* vfx, void*, std::uint32_t animation);                 // 00690510 CSWCVisualEffect::SetAnimation RET 4 (areaPlay)
void __cdecl scene_bsp_remove_part_0046c510(void* scene, void* part);                               // 0046C510 SceneBSPRemovePart (0506)
void __fastcall exo_array_list_remove_00445080(void* list, void*, void* value);                     // 00445080 CExoArrayList<T*>::Remove RET 4 (0314)
void __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, const void* resref, std::uint32_t x,
                                                         std::uint32_t y, std::uint32_t z, std::uint32_t a5,
                                                         std::uint32_t a6, std::uint32_t a7, std::uint32_t a8,
                                                         std::uint32_t a9, std::uint32_t a10);   // 005D5E10 RET 28h (0812)
extern void* kotor_cell_sound_007a39ec;                                                              // [007A39EC] CExoSound*
void __fastcall recovered_client_creature_set_combat_state_006182a0(void* self, void*, std::uint32_t state) noexcept(false);   // 006182A0 RET 4 (claude_w3_update.obj)

// ------------------------------------------------------------------ this unit
std::uint32_t __fastcall recovered_client_handle_trigger_update_0064f430(void* self, void*, std::uint32_t id, std::uint32_t mask);   // RET 8
std::uint32_t __fastcall recovered_client_handle_trigger_delete_0064f960(void* self, void*);       // RET
std::uint32_t __fastcall recovered_client_handle_door_delete_0064d3a0(void* self, void*);          // RET
std::uint32_t __fastcall recovered_client_handle_placeable_delete_0064da20(void* self, void*);     // RET
std::uint32_t __fastcall recovered_client_handle_creature_delete_0064cc90(void* self, void*);      // RET
std::uint32_t __fastcall recovered_client_handle_item_delete_0064eac0(void* self, void*);          // RET
std::uint32_t __fastcall recovered_client_handle_item_update_0064eb90(void* self, void*, std::uint32_t id, std::uint32_t mask);   // RET 8
std::uint32_t __fastcall recovered_client_handle_creature_strip_equipped_items_00651af0(void* self, void*);   // RET
std::uint32_t __fastcall recovered_client_handle_item_destroy_00651b30(void* self, void*);         // RET
std::uint32_t __fastcall recovered_client_handle_item_script_add_00654b10(void* self, void*);      // RET
std::uint32_t __fastcall recovered_client_handle_creature_visual_effect_0064c3c0(void* self, void*);   // RET
std::uint32_t __fastcall recovered_client_handle_aoe_delete_006517b0(void* self, void*);           // RET
std::uint32_t __fastcall recovered_client_handle_aoe_update_006517f0(void* self, void*, std::uint32_t id, std::uint32_t mask);   // RET 8
void* __fastcall recovered_client_get_aoe_by_id_005ed610(void* app, void*, std::uint32_t id);      // RET 4
void* __fastcall recovered_client_internal_get_aoe_by_id_005f1360(void* internal, void*, std::uint32_t id);   // RET 4
void __fastcall recovered_client_aoe_stop_effect_006d9350(void* aoe, void*);                        // RET
void __fastcall recovered_client_creature_strip_equipped_items_0060e010(void* creature, void*);    // RET
void __fastcall recovered_client_trigger_remove_from_render_list_00691d80(void* trigger, void*);   // RET
void __cdecl recovered_aur_part_trigger_delete_0049d7d0(void* scene, void* part, std::uint32_t destroy);   // cdecl
void __cdecl recovered_scene_remove_part_from_rooms_0046d1e0(void* scene, void* part);             // cdecl
}

using namespace kotor_w3;

namespace {
constexpr U8 kCreature_00746634 = 5;                                   // .rdata 00746634
constexpr U8 kPlaceable_00746638 = 9;                                  // .rdata 00746638
constexpr U8 kDoor_00746639 = 10;                                      // .rdata 00746639
constexpr U8 kAreaOfEffect_0074663a = 0x0B;                            // .rdata 0074663A
constexpr U8 kArraySuccess_0074666c = 0;                               // .rdata 0074666C (CGameObjectArray status OK)
constexpr U kInvalidObject_00746644 = 0x7F000000u;                     // .rdata 00746644
constexpr float kZero_0073d700 = 0.0f;                                 // .rdata 0073D700
constexpr float kThousand_0073d6fc = 1000.0f;                          // .rdata 0073D6FC
const char kEmpty_0073d71c[] = "";                                     // .rdata 0073D71C
const char kBodyBag_00746ef0[] = "Body_Bag";                           // .rdata 00746EF0
const char kCorpse_00745ef4[] = "Corpse";                              // .rdata 00745EF4
const char kExplosionSound_00751cfc[] = "ExplosionSound";              // .rdata 00751CFC
const char kDummyModel_00751cd8[] = "c_dummy";                         // .rdata 00751CD8
const char kDefault_0073df6c[] = "default";                            // .rdata 0073DF6C
const char kSoundCessation_00754408[] = "SoundCessation";              // .rdata 00754408

float rf(void* m) noexcept(false) { return kotor_fastcall_004d67b0(m, nullptr, bitsf(1.0f), 0x20); }   // ReadFLOAT(1.0, 20h)
U16 rw(void* m, U bits = 0x10) noexcept(false) { return kotor_fastcall_004d6750(m, nullptr, bits); }   // ReadWORD
U8 rb(void* m, U bits = 8) noexcept(false) { return kotor_fastcall_004d6720(m, nullptr, bits); }       // ReadBYTE
U rbool(void* m) noexcept(false) { return kotor_fastcall_004d66f0(m, nullptr); }                       // ReadBOOL
U rid(void* m) noexcept(false) { return kotor_fastcall_00692630(m, nullptr); }                         // ReadOBJECTIDClient
U overflow(void* m) noexcept(false) { return recovered_player_message::overrun_004d6230(m); }         // MessageReadOverflow(1)
U underflow(void* m) noexcept(false) { return recovered_player_message::underrun_004d6240(m); }       // MessageReadUnderflow(1)
void* object_array() noexcept { return kotor_fastcall_005ed570(client_app(), nullptr); }               // 005ED570 GetGameObjectArray

void construct_string(ExoString* s) noexcept(false) {
    kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr);   // 005B3190
}
void destroy_string(ExoString* s) noexcept(false) { frame_LogStringDtor_005e5c20(s, nullptr); }   // 005E5C20
void destroy_string_void(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
void free_void(void* p) { live_free(p); }

// Unwind-only cleanup (original EH state action); every normal exit of the guarded range dismisses it first.
class Unwind {
public:
    Unwind(void (*action)(void*), void* object) noexcept : action_(action), object_(object), armed_(true) {}
    ~Unwind() { if (armed_) action_(object_); }
    void dismiss() noexcept { armed_ = false; }
    Unwind(const Unwind&) = delete;
    Unwind& operator=(const Unwind&) = delete;
private:
    void (*action_)(void*);
    void* object_;
    bool armed_;
};
// new T(): operator new(size); NULL -> NULL without construction; a throwing constructor frees the block (EH state).
template <class Ctor> void* new_object(U size, Ctor ctor) noexcept(false) {
    void* const memory = live_operator_new(size);                      // push size; call 006FA7E6
    if (memory == nullptr) return nullptr;                             // test eax,eax / xor esi,esi
    Unwind guard(&free_void, memory);
    void* const object = ctor(memory);
    guard.dismiss();
    return object;
}
// __ftol2 (006FAE8C) on an x87 value: truncate to 64 bits, keep the low dword; NaN / infinity / out of range -> 0.
U ftol2_low(double value) noexcept {
    if (!(value > -9223372036854775808.0 && value < 9223372036854775808.0)) return 0;
    return static_cast<U>(static_cast<std::uint64_t>(static_cast<std::int64_t>(value)));
}
template <class Fn> Fn vs(const void* object, unsigned offset) noexcept { return vslot<Fn>(object, offset); }
using V0 = void* (__fastcall*)(void*, void*);
using V0u = U (__fastcall*)(void*, void*);
using V1 = void (__fastcall*)(void*, void*, U);
using VP = void (__fastcall*)(void*, void*, const void*);
using DeletingDtor = void* (__fastcall*)(void*, void*, U);             // vtable slot 0, scalar deleting destructor(1)
using AddEffect = U (__fastcall*)(void*, void*, U, U, U, U, U, U, const Vector3*);   // vtbl B4h, RET 1Ch
using LoadModel = U (__fastcall*)(void*, void*, const void*, U, U);   // vtbl 78h (resref*, FFh, 0), RET 0Ch
}  // namespace

extern "C" {
// =========================================================================================== small leaves
// 005F1360 CClientExoAppInternal::GetAreaOfEffectByGameObjectID(id). RET 4. [this+14h] game object array; an object of
// type 0Bh answers vtbl[54h] (AsSWCAreaOfEffectObject), anything else 0.
void* __fastcall recovered_client_internal_get_aoe_by_id_005f1360(void* internal, void*, U id) {
    void* object = nullptr;                                            // 005F136E [esp] = 0
    if (kotor_fastcall_004d8230(rp(internal, 0x14), nullptr, id, &object) != kArraySuccess_0074666c) return nullptr;   // 005F1361..005F1381
    if (object == nullptr) return nullptr;                             // 005F1383..005F1389
    if (ld<U8>(object, 8) != kAreaOfEffect_0074663a) return nullptr;   // 005F138B..005F1394
    return vs<V0>(object, 0x54)(object, nullptr);                      // 005F1396..005F139C
}
// 005ED610 CClientExoApp::GetAreaOfEffectByGameObjectID: mov ecx,[ecx+4]; jmp 005F1360.
void* __fastcall recovered_client_get_aoe_by_id_005ed610(void* app, void*, U id) {
    return recovered_client_internal_get_aoe_by_id_005f1360(rp(app, 4), nullptr, id);   // 005ED610..005ED613
}
// 0046D1E0 SceneRemovePartFromRooms(scene, part) cdecl: key = part->vtbl[54h](); for every room [scene+C8h][i]
// (i < [scene+CCh]) whose list [[room+58h]+54h] contains key, CExoArrayList::Remove(key).
void __cdecl recovered_scene_remove_part_from_rooms_0046d1e0(void* scene, void* part) {
    void* const key = vs<V0>(part, 0x54)(part, nullptr);               // 0046D1E0..0046D1E9 esi
    for (I32 i = 0; i < ld<I32>(scene, 0xCC); ++i) {                   // 0046D1F2..0046D1FC / 0046D235..0046D23E
        void* const room = rp(rp(scene, 0xC8), static_cast<U>(i) * 4); // 0046D202..0046D208
        unsigned char* const list = static_cast<unsigned char*>(rp(room, 0x58)) + 0x54;   // 0046D20B / 0046D210
        if (key == nullptr) continue;                                  // 0046D200 / 0046D20E
        I32 hits = 0;                                                  // 0046D216
        const I32 size = ld<I32>(list, 4);                             // 0046D213
        if (size > 0) {                                                // 0046D218..0046D21A
            const unsigned char* p = static_cast<const unsigned char*>(rp(list, 0));   // 0046D21C
            for (I32 n = size; n != 0; --n, p += 4)                    // 0046D220..0046D229
                if (ld<void*>(p, 0) == key) ++hits;
        }
        if (hits != 0) exo_array_list_remove_00445080(list, nullptr, key);   // 0046D22B..0046D230
    }
}
// 0049D7D0 AurPartTriggerDelete(scene, part, BOOL destroy) cdecl: BSP scenes ([scene+A0h]) use SceneBSPRemovePart,
// others SceneRemovePartFromRooms; then a non-NULL part is deleted (vtbl[0](1)) when the low byte of `destroy` is set.
void __cdecl recovered_aur_part_trigger_delete_0049d7d0(void* scene, void* part, U destroy) {
    if (rp(scene, 0xA0) != nullptr) scene_bsp_remove_part_0046c510(scene, part);   // 0049D7D4..0049D7E5
    else recovered_scene_remove_part_from_rooms_0046d1e0(scene, part);             // 0049D7EC
    if (static_cast<U8>(destroy) != 0 && part != nullptr)                           // 0049D7F1..0049D7FE
        vs<DeletingDtor>(part, 0)(part, nullptr, 1);                                // 0049D800..0049D806
}
// 00691D80 CSWCTrigger::RemoveFromRenderList: area = GetModule()->[+48h] (no NULL check); when the area's trigger id
// list (+11Ch) holds [this+4]: remove it and AurPartTriggerDelete(area->[+184h] scene, [this+138h] part, 0).
void __fastcall recovered_client_trigger_remove_from_render_list_00691d80(void* trigger, void*) {
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 00691D80..00691D92
    const U id = rd32(trigger, 4);                                     // 00691D95
    unsigned char* const list = static_cast<unsigned char*>(area) + 0x11C;   // 00691D98
    I32 hits = 0;                                                      // 00691DA1
    I32 size = ld<I32>(list, 4);                                       // 00691D9E
    if (size <= 0) return;                                             // 00691DA3..00691DA5
    const unsigned char* p = static_cast<const unsigned char*>(rp(list, 0));   // 00691DA7
    for (; size != 0; --size, p += 4) if (rd32(p, 0) == id) ++hits;    // 00691DB0..00691DB9
    if (hits == 0) return;                                             // 00691DBB..00691DBD
    recovered_int_list_remove_0066dcc0(list, nullptr, id);             // 00691DBF..00691DC0
    void* const area2 = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);  // 00691DC5..00691DD9
    recovered_aur_part_trigger_delete_0049d7d0(rp(area2, 0x184), rp(trigger, 0x138), 0);   // 00691DD3..00691DEB
}
// 0060E010 CSWCCreature::StripEquippedItems: for the 18 values 0..11h passed to GetEquippedItemID (the original passes
// the loop counter, not a slot bit), every resolvable item gets SetEquippedBy(NULL) (vtbl[34h] result unchecked); then
// the model base [+68h] gets vtbl[7Ch](7F000000h, "", 0, 1.0) and vtbl[7Ch](7F000000h, "", 1, 1.0).
void __fastcall recovered_client_creature_strip_equipped_items_0060e010(void* creature, void*) {
    for (U index = 0; index < 0x12; ++index) {                         // 0060E02E..0060E07D (ebx 0.., ebp 12h..)
        const U item_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, index);   // 0060E035..0060E038
        if (item_id == kInvalidObject_00746644) continue;              // 0060E03F..0060E045
        void* object;                                                  // {E-1Ch}
        if (kotor_fastcall_004d8230(object_array(), nullptr, item_id, &object) != kArraySuccess_0074666c) continue;   // 0060E047..0060E067
        void* const item = vs<V0>(object, 0x34)(object, nullptr);      // 0060E069..0060E06F
        kotor_m15a_item_set_equipped_by_00646600(item, nullptr, nullptr);   // 0060E072..0060E076
    }
    if (rp(creature, 0x68) == nullptr) return;                         // 0060E07F..0060E084
    using SetPart = void (__fastcall*)(void*, void*, U, const void*, U, float);   // vtbl 7Ch, RET 10h
    for (U part = 0; part < 2; ++part) {                               // 0060E086..0060E0C2 / 0060E0C7..0060E100
        ExoString text;                                                // {E-1Ch} / {E-14h}
        frame_LogStringCtor_005e5a90(&text, nullptr, kEmpty_0073d71c); // 0060E086..0060E08F / 0060E0C7..0060E0D0
        {
            Unwind guard(&destroy_string_void, &text);                 // 0060E0AC / 0060E0ED EH state 0 / 1
            void* const base = rp(creature, 0x68);                     // 0060E094 / 0060E0D5
            vs<SetPart>(base, 0x7C)(base, nullptr, kInvalidObject_00746644, &text, part, 1.0f);   // 0060E097..0060E0B4 / 0060E0D8..0060E0F5
            guard.dismiss();
        }
        destroy_string(&text);                                         // 0060E0BA..0060E0C2 / 0060E0F8..0060E100
    }
}
// 006D9350 CSWCAreaOfEffectObject::StopEffect. Plays the cessation sound (vfx_persistent.2da? [[rules+B8h]+1Ch] row
// [+170h] "SoundCessation"; the original assigns "" on a successful lookup, so the played CResRef is always empty),
// stops / deletes the sound source [+154h], marks [+138h] = 1, ends the main visual (+14Ch, SetAnimation 3), sends every
// part (i < [+120h]) to animation 3, and sets the 3000 ms stop timer [+130h].
void __fastcall recovered_client_aoe_stop_effect_006d9350(void* aoe, void*) {
    ExoString sound_1c;                                                // {E-1Ch} (base E-34h + 10h, after 4 pushes)
    construct_string(&sound_1c);                                       // 006D936E..006D9372
    {
        Unwind guard(&destroy_string_void, &sound_1c);                 // 006D9380 EH state 0
        I32 found;
        {
            ExoString column;                                          // {E-10h}
            frame_LogStringCtor_005e5a90(&column, nullptr, kSoundCessation_00754408);   // 006D9377..006D9388
            Unwind guard_col(&destroy_string_void, &column);           // 006D93AE EH state 1
            void* const table = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x1C);   // 006D938D..006D9398
            found = c2da_current_get_string_00413270(table, nullptr, rd32(aoe, 0x170), &column, &sound_1c);   // 006D939B..006D93B3
            guard_col.dismiss();
            destroy_string(&column);                                   // 006D93B8..006D93C3
        }
        if (found != 0) c2da_assign_text_005e5140(&sound_1c, nullptr, kEmpty_0073d71c);   // 006D93C8..006D93D5 (quirk)
        ResRef sound_ref;                                              // {E-10h}
        async_ref_construct_string_00406d60(&sound_ref, nullptr, &sound_1c);   // 006D93DA..006D93E3
        kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &sound_ref, rd32(aoe, 0x24),
                                                  rd32(aoe, 0x28), rd32(aoe, 0x2C), 0x3FC00000u, 0xC, 0, 0, 0, 0);   // 006D93E8..006D941A
        if (rp(aoe, 0x154) != nullptr) {                               // 006D941F..006D9427
            kotor_m15_sound_source_stop_005d5a20(rp(aoe, 0x154), nullptr);   // 006D9429
            void* const source = rp(aoe, 0x154);                       // 006D942E
            if (source != nullptr) vs<DeletingDtor>(source, 0)(source, nullptr, 1);   // 006D9434..006D943C
            st<U>(aoe, 0x154, 0);                                      // 006D943E
        }
        st<U>(aoe, 0x138, 1);                                          // 006D9450
        if (rp(aoe, 0x14C) != nullptr) area_vfx_play_00690510(rp(aoe, 0x14C), nullptr, 3);   // 006D9448..006D945E
        using PartTime = double (__fastcall*)(void*, void*, U);        // part vtbl 38h (ST0), RET 4
        using PartPlay = void (__fastcall*)(void*, void*, U, float, U, U);   // part vtbl 54h, RET 10h
        for (I32 i = 0; i < ld<I32>(aoe, 0x120); ++i) {                // 006D9463..006D946D / 006D94E3..006D94EC
            if (rp(aoe, 0x68) != nullptr) {                            // 006D9470..006D9475
                const U index = static_cast<U8>(i);                    // 006D947D movzx edi,bl
                const U offset = index * 0xF0;                         // 006D9480..006D9482
                st<U16>(rp(aoe, 0x148), index * 2, 3);                 // 006D9477 / 006D9488
                void* const part = static_cast<unsigned char*>(rp(aoe, 0x68)) + offset;   // 006D948E..006D9494
                const U time = ftol2_low(vs<PartTime>(part, 0x38)(part, nullptr, 3));   // 006D9496..006D949B
                st<U>(rp(aoe, 0x104), index * 4, time);                // 006D94A0..006D94A6
                if (rd32(rp(aoe, 0x104), index * 4) == 0) st<U>(rp(aoe, 0x104), index * 4, 1);   // 006D94A9..006D94B9
                void* const part2 = static_cast<unsigned char*>(rp(aoe, 0x68)) + offset;   // 006D94BF..006D94C9
                vs<PartPlay>(part2, 0x54)(part2, nullptr, 3, 1.0f, 1, 0);   // 006D94C5..006D94D3
            }
            st<U>(rp(aoe, 0xFC), static_cast<U>(i) * 4, 0);           // 006D94D6..006D94DC
        }
        st<U>(aoe, 0x130, 0xBB8);                                      // 006D94F2
        guard.dismiss();
    }
    destroy_string(&sound_1c);                                         // 006D94EE / 006D9504
}

// =========================================================================================== 'U' / 'D' handlers
// 0064F430 CSWCMessage::HandleServerToPlayerTriggerUpdate_Update(id, mask). RET 8. EAX 0 for an unknown trigger or a
// read overflow. Flag 10h on a trigger with [+108h]: BOOL highlighted -> UpdateTriggerColor(!BOOL).
U __fastcall recovered_client_handle_trigger_update_0064f430(void* self, void*, U id, U mask) {
    void* const trigger = kotor_fastcall_005ed5d0(client_app(), nullptr, id);   // 0064F430..0064F446
    if (trigger == nullptr) return 0;                                  // 0064F448..0064F44E (EAX = esi = 0)
    if ((mask & 0x10) == 0 || rd32(trigger, 0x108) == 0) return 1;     // 0064F451..0064F461 -> 0064F4A0
    const U value = rbool(self);                                       // 0064F463..0064F46E ebx
    if (overflow(self) != 0) return 0;                                 // 0064F46A..0064F47E
    recovered_client_trigger_update_color_006913b0(trigger, nullptr, value != 0 ? 0u : 1u);   // 0064F481..0064F49B
    return 1;                                                          // 0064F48E..0064F4A8
}
// 0064F960 CSWCMessage::HandleServerToPlayerTriggerUpdate_Delete. RET. OBJECTID, BYTE kind. Unknown trigger or overflow
// -> 0. The trigger leaves the render list first. Kind 1: explosion (traps.2da-like [[rules+B8h]+8Ch] row [+148h]
// "ExplosionSound" played at the trigger) + fire-and-forget animation 15Ch; kind 2: animation 15Dh; otherwise the
// object is removed from the game object array and deleted (AsTrigger vtbl[3Ch], then vtbl[0](1)).
U __fastcall recovered_client_handle_trigger_delete_0064f960(void* self, void*) {
    const U id = rid(self);                                            // 0064F97B..0064F97D edi
    const U8 kind = rb(self);                                          // 0064F982..0064F991 bl
    if (overflow(self) != 0) return 0;                                 // 0064F98D..0064F99A -> 0064FADC
    void* const trigger = kotor_fastcall_005ed5d0(client_app(), nullptr, id);   // 0064F9A0..0064F9A9
    if (trigger == nullptr) return 0;                                  // 0064F9AE..0064F9B2
    recovered_client_trigger_remove_from_render_list_00691d80(trigger, nullptr);   // 0064F9B8..0064F9BA
    if (kind == 1) {                                                   // 0064F9BF..0064F9C2
        ExoString sound_14;                                            // {E-14h}
        construct_string(&sound_14);                                   // 0064F9C8..0064F9CC
        {
            Unwind guard(&destroy_string_void, &sound_14);             // 0064F9DA EH state 0
            const U8 row = ld<U8>(trigger, 0x148);                     // 0064F9D1
            if (row != 0xFF) {                                         // 0064F9D7..0064F9E2
                I32 found;
                {
                    ExoString column;                                  // {E-24h}
                    frame_LogStringCtor_005e5a90(&column, nullptr, kExplosionSound_00751cfc);   // 0064F9E8..0064F9F1
                    Unwind guard_col(&destroy_string_void, &column);   // 0064FA18 EH state 1
                    void* const table = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x8C);   // 0064F9F6..0064FA02
                    found = c2da_current_get_string_00413270(table, nullptr, row, &column, &sound_14);   // 0064FA08..0064FA1D
                    guard_col.dismiss();
                    destroy_string(&column);                           // 0064FA22..0064FA2D
                }
                if (found != 0) {                                      // 0064FA32..0064FA34
                    ResRef sound_ref;                                  // {E-24h}
                    async_ref_construct_string_00406d60(&sound_ref, nullptr, &sound_14);   // 0064FA36..0064FA3F
                    kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &sound_ref,
                                                              rd32(trigger, 0x24), rd32(trigger, 0x28), rd32(trigger, 0x2C),
                                                              0, 0x16, 0, 0, 0, 0);   // 0064FA44..0064FA73
                }
            }
            kotor_fastcall_006911b0(trigger, nullptr, 0x15C, 0, 0);    // 0064FA78..0064FA83
            guard.dismiss();
        }
        destroy_string(&sound_14);                                     // 0064FA88..0064FA94
        return 1;                                                      // 0064FA99 -> 0064FB01
    }
    if (kind == 2) {                                                   // 0064FA9B..0064FA9E
        kotor_fastcall_006911b0(trigger, nullptr, 0x15D, 0, 0);        // 0064FAA0..0064FAAB
        return 1;
    }
    void* object;                                                      // {E-24h}
    if (recovered_object_array_remove_004d81a0(object_array(), nullptr, id, &object) != kArraySuccess_0074666c)
        return 0;                                                      // 0064FAB2..0064FAD2
    if (object == nullptr) return 0;                                   // 0064FAD4..0064FADA
    void* const as_trigger = vs<V0>(object, 0x3C)(object, nullptr);    // 0064FAF0..0064FAF2
    if (as_trigger != nullptr) vs<DeletingDtor>(as_trigger, 0)(as_trigger, nullptr, 1);   // 0064FAF5..0064FAFF
    return 1;                                                          // 0064FB01..0064FB17
}
// 0064D3A0 CSWCMessage::HandleServerToPlayerDoorUpdate_Delete. RET. OBJECTID; overflow, a failed array removal or a NULL
// object -> 0. A removed object whose AsDoor (vtbl[14h]) is non-NULL is deleted (vtbl[0](1)); GetClientAIMaster is
// called and its result discarded, as the original.
U __fastcall recovered_client_handle_door_delete_0064d3a0(void* self, void*) {
    const U id = rid(self);                                            // 0064D3A3..0064D3A5 edi
    if (overflow(self) != 0) return 0;                                 // 0064D3AA..0064D3B7
    void* object;                                                      // {E-4}
    if (recovered_object_array_remove_004d81a0(object_array(), nullptr, id, &object) != kArraySuccess_0074666c)
        return 0;                                                      // 0064D3B9..0064D3D9
    if (object == nullptr) return 0;                                   // 0064D3DB..0064D3E1
    if (vs<V0>(object, 0x14)(object, nullptr) != nullptr) {            // 0064D3E9..0064D3F0
        (void)kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr);   // 0064D3F2..0064D3FA (unused)
        void* const door = vs<V0>(object, 0x14)(object, nullptr);      // 0064D3FF..0064D405
        if (door != nullptr) vs<DeletingDtor>(door, 0)(door, nullptr, 1);   // 0064D408..0064D412
    }
    return 1;                                                          // 0064D414..0064D41C
}
// 0064DA20 CSWCMessage::HandleServerToPlayerPlaceableUpdate_Delete. RET. OBJECTID, BOOL instant, BOOL snap, DWORD fade.
// The placeable fades out: SetDesiredFadeState(0, 1, instant ? 0 : 45000, snap ? 0 : fade) (snap first SetFadeState(0)),
// then [+E4h] = 1. Overflow or an unknown object -> 0.
U __fastcall recovered_client_handle_placeable_delete_0064da20(void* self, void*) {
    const U id = rid(self);                                            // 0064DA25..0064DA2E edi
    const U instant = rbool(self);                                     // 0064DA30..0064DA37 ebx
    const U snap = rbool(self);                                        // 0064DA39..0064DA42 ebp
    const U fade = kotor_fastcall_004d6780(self, nullptr, 0x20);       // 0064DA3E..0064DA4D {E-4}
    if (overflow(self) != 0) return 0;                                 // 0064DA49..0064DA58
    void* const object = kotor_fastcall_005ed580(client_app(), nullptr, id);   // 0064DA5A..0064DA68
    if (object == nullptr) return 0;                                   // 0064DA6A..0064DA6C
    const U time = instant != 0 ? 0u : 0xAFC8u;                        // 0064DA78..0064DA86 (setne/dec/and)
    if (vs<V0>(object, 0x4C)(object, nullptr) == nullptr) return 1;    // 0064DA88..0064DA8F
    void* const placeable = vs<V0>(object, 0x4C)(object, nullptr);     // 0064DA91..0064DA9A
    U last;
    if (snap != 0) {                                                   // 0064DA98..0064DA9C
        recovered_client_object_set_fade_state_0063e220(placeable, nullptr, 0);   // 0064DA9E..0064DAA2
        last = 0;                                                      // 0064DAA7
    } else {
        last = fade;                                                   // 0064DAAB..0064DAAF
    }
    recovered_client_object_set_desired_fade_state_0063ce60(placeable, nullptr, 0, 1, time, last);   // 0064DAB0..0064DAB7
    st<U>(placeable, 0xE4, 1);                                         // 0064DABC
    return 1;                                                          // 0064DAC6..0064DAD0
}
// 0064CC90 CSWCMessage::HandleServerToPlayerCreatureUpdate_Delete. RET. OBJECTID, BOOL instant, BOOL snap, DWORD fade,
// BOOL dead. Overflow -> 0; unknown creature -> 1. Alive: fade out (as the placeable). Dead in an area: AddCorpse unless
// appearance.2da "Body_Bag" row -> bodybag table ([[rules+B8h]+D4h]) "Corpse" is non-zero. Then vtbl[BCh](),
// SetCombatState(0), [+1B0h] = 0, and [+E4h] = 1 unless (no server creature and the creature is in an area).
U __fastcall recovered_client_handle_creature_delete_0064cc90(void* self, void*) {
    const U id = rid(self);                                            // 0064CCAC..0064CCB5 edi
    const U instant = rbool(self);                                     // 0064CCB7..0064CCBE ebx
    const U snap = rbool(self);                                        // 0064CCC0..0064CCC9 {E-2Ch}
    const U fade = kotor_fastcall_004d6780(self, nullptr, 0x20);       // 0064CCC5..0064CCD4 {E-28h}
    const U dead = rbool(self);                                        // 0064CCD2..0064CCE1 ebp
    if (overflow(self) != 0) return 0;                                 // 0064CCDD..0064CD00
    const U time = instant != 0 ? 0u : 0xAFC8u;                        // 0064CD0A..0064CD18 ebx
    void* const c = frame_R4_005ed5b0(client_app(), nullptr, id);      // 0064CD01..0064CD1F esi
    if (c == nullptr) return 1;                                        // 0064CD21..0064CD23 -> 0064CE74
    void* const server = kotor_m15a_creature_get_server_creature_0060fb20(c, nullptr);   // 0064CD29..0064CD32 {E-18h}
    if (dead == 0) {                                                   // 0064CD30..0064CD36
        if (snap != 0) {                                               // 0064CD38..0064CD40
            recovered_client_object_set_fade_state_0063e220(c, nullptr, 0);   // 0064CD42..0064CD43
            recovered_client_object_set_desired_fade_state_0063ce60(c, nullptr, 0, 1, time, 0);   // 0064CD48..0064CD4F
        } else {
            recovered_client_object_set_fade_state_0063e220(c, nullptr, 1);   // 0064CD59..0064CD5B
            recovered_client_object_set_desired_fade_state_0063ce60(c, nullptr, 0, 1, time, fade);   // 0064CD60..0064CD6C
        }
    } else if (rp(c, 0x20) != nullptr) {                               // 0064CD76..0064CD7B
        bool corpse = true;
        U body_bag = 0;                                                // {E-20h}
        I32 found;
        {
            ExoString column;                                          // {E-28h}
            frame_LogStringCtor_005e5a90(&column, nullptr, kBodyBag_00746ef0);   // 0064CD81..0064CD8A
            Unwind guard(&destroy_string_void, &column);               // 0064CDB6 EH state 0
            const U row = ld<U16>(rp(c, 0x21C), 0x18);                 // 0064CD8F..0064CDB1
            void* const table = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x0C);   // 0064CD99..0064CDA5
            found = c2da_current_get_int_00413660(table, nullptr, row, &column, &body_bag);   // 0064CDA8..0064CDBE
            guard.dismiss();
            destroy_string(&column);                                   // 0064CDC6..0064CDD0
        }
        if (found != 0) {                                              // 0064CDD5..0064CDD7
            U corpse_flag = 0;                                         // {E-1Ch}
            {
                ExoString column;                                      // {E-14h}
                frame_LogStringCtor_005e5a90(&column, nullptr, kCorpse_00745ef4);   // 0064CDD9..0064CDE2
                Unwind guard(&destroy_string_void, &column);           // 0064CE0A EH state 1
                void* const table = rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0xD4);   // 0064CDE7..0064CDF3
                found = c2da_current_get_int_00413660(table, nullptr, body_bag, &column, &corpse_flag);   // 0064CDF9..0064CE12
                guard.dismiss();
                destroy_string(&column);                               // 0064CE17..0064CE21
            }
            if (found != 0 && corpse_flag != 0) corpse = false;        // 0064CE26..0064CE30
        }
        if (corpse) kotor_area_add_corpse_006056b0(rp(c, 0x20), nullptr, rd32(c, 4));   // 0064CE32..0064CE39
    }
    vs<V0>(c, 0xBC)(c, nullptr);                                       // 0064CE3E..0064CE42
    recovered_client_creature_set_combat_state_006182a0(c, nullptr, 0);   // 0064CE48..0064CE4C (claude_w3_update.obj)
    st<U>(c, 0x1B0, 0);                                                // 0064CE57
    if (server != nullptr || rp(c, 0x20) == nullptr) st<U>(c, 0xE4, 1);   // 0064CE51..0064CE6A
    return 1;                                                          // 0064CE74..0064CE8B
}
// 0064EAC0 CSWCMessage::HandleServerToPlayerItemUpdate_Delete. RET. OBJECTID, BOOL destroy. destroy == 1: remove from the
// array and delete the AsItem (vtbl[34h]) object, EAX = 1. Otherwise: look the object up (unchecked AsItem), detach it
// from its area (item+10h vtbl[84h](0)) and, when nobody equips it, vtbl[94h](0) on the same sub-object.
U __fastcall recovered_client_handle_item_delete_0064eac0(void* self, void*) {
    const U id = rid(self);                                            // 0064EAC4..0064EACD ebx
    const U destroy = rbool(self);                                     // 0064EACF..0064EAD8 edi
    if (overflow(self) != 0) return 0;                                 // 0064EAD4..0064EAE1
    void* const array = object_array();                                // 0064EAE3..0064EAEB
    void* object;                                                      // {E-4}
    if (destroy == 1) {                                                // 0064EAF0..0064EAF5
        if (recovered_object_array_remove_004d81a0(array, nullptr, id, &object) != kArraySuccess_0074666c) return 0;   // 0064EAF7..0064EB08
        if (object == nullptr) return 0;                               // 0064EB0A..0064EB10
        if (vs<V0>(object, 0x34)(object, nullptr) == nullptr) return 1;   // 0064EB12..0064EB19
        void* const item = vs<V0>(object, 0x34)(object, nullptr);      // 0064EB1B..0064EB21
        if (item == nullptr) return 1;                                 // 0064EB24..0064EB26
        vs<DeletingDtor>(item, 0)(item, nullptr, destroy);             // 0064EB28..0064EB2D (push edi = 1)
        return destroy;                                                // 0064EB2F mov eax,edi
    }
    if (kotor_fastcall_004d8230(array, nullptr, id, &object) != kArraySuccess_0074666c) return 0;   // 0064EB36..0064EB47
    void* const item = vs<V0>(object, 0x34)(object, nullptr);          // 0064EB49..0064EB52
    if (item == nullptr) return 0;                                     // 0064EB54..0064EB56
    void* const sub = static_cast<unsigned char*>(item) + 0x10;        // 0064EB62 esi
    vs<V1>(sub, 0x84)(sub, nullptr, 0);                                // 0064EB5F..0064EB69
    if (kotor_m15e_item_get_equipped_by_00646630(item, nullptr) == nullptr)   // 0064EB6F..0064EB78
        vs<V1>(sub, 0x94)(sub, nullptr, 0);                            // 0064EB7A..0064EB7F (push eax = 0)
    return 1;                                                          // 0064EB85..0064EB8E
}
// 0064EB90 CSWCMessage::HandleServerToPlayerItemUpdate_Update(id, mask). RET 8. No reads: a known item is re-attached to
// the current area (item+10h vtbl[94h]([area+184h]), vtbl[84h](area); module/area unchecked as the original). Unknown
// item -> 0. The CResRef() temporary of the original is constructed and unused.
U __fastcall recovered_client_handle_item_update_0064eb90(void*, void*, U id, U) {
    ResRef unused;                                                     // {E-10h}
    kotor_fastcall_00405ed0(&unused, nullptr);                         // 0064EB94..0064EB98
    void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 0064EB9D..0064EBAF
    if (item == nullptr) return 0;                                     // 0064EBB1..0064EBB9
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0064EBBC..0064EBCA edi
    void* const sub = static_cast<unsigned char*>(item) + 0x10;        // 0064EBD6
    vs<V1>(sub, 0x94)(sub, nullptr, rd32(area, 0x184));                // 0064EBCD..0064EBDC
    vs<VP>(sub, 0x84)(sub, nullptr, area);                             // 0064EBE2..0064EBE7
    return 1;                                                          // 0064EBED..0064EBF7
}
// 00651AF0 CSWCMessage::HandleServerToPlayerCreatureUpdate_StripEquippedItems. RET. OBJECTID; overflow or unknown -> 0.
U __fastcall recovered_client_handle_creature_strip_equipped_items_00651af0(void* self, void*) {
    const U id = rid(self);                                            // 00651AF2..00651AFD
    if (overflow(self) != 0) return 0;                                 // 00651AF9..00651B06
    void* const c = frame_R4_005ed5b0(client_app(), nullptr, id);      // 00651B08..00651B11
    if (c == nullptr) return 0;                                        // 00651B16..00651B18
    recovered_client_creature_strip_equipped_items_0060e010(c, nullptr);   // 00651B1F..00651B21
    return 1;                                                          // 00651B26..00651B2D
}
// 00651B30 CSWCMessage::HandleServerToPlayerItemUpdate_DestroyItem. RET. OBJECTID; the item is removed from the game
// object array by its own id [+14h] and deleted (vtbl[0](1)). Overflow, unknown item or a failed removal -> 0.
U __fastcall recovered_client_handle_item_destroy_00651b30(void* self, void*) {
    const U id = rid(self);                                            // 00651B33..00651B3E
    if (overflow(self) != 0) return 0;                                 // 00651B3A..00651B47
    void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 00651B49..00651B57
    if (item == nullptr) return 0;                                     // 00651B59..00651B5B
    void* removed;                                                     // {E-4}
    if (recovered_object_array_remove_004d81a0(object_array(), nullptr, rd32(item, 0x14), &removed) != kArraySuccess_0074666c)
        return 0;                                                      // 00651B5D..00651B81
    vs<DeletingDtor>(item, 0)(item, nullptr, 1);                       // 00651B89..00651B8F
    return 1;                                                          // 00651B91..00651B99
}
// 00654B10 CSWCMessage::HandleServerToPlayerItemScript_Add. RET. OBJECTID, then ItemAppearance(base, variation, a, b).
// An unknown id creates the item (CreateItem(base, variation, 4Dh)), names it (vtbl[4](id)), sets its area and adds it to
// the object array; a known one is its AsItem (vtbl[34h], unchecked). Then: [+168h] = 0, visibility for the area,
// vtbl[94h]([area+184h]), AI-master registration, ActiveItemProperties(id), vtbl[78h](&[+128h] model, FFh, 0) (0 ->
// EAX 0); a base item with [+48h] gets vtbl[B0h](FFh, 1, "default").
U __fastcall recovered_client_handle_item_script_add_00654b10(void* self, void*) {
    U id = rid(self);                                                  // 00654B14..00654B1F {E-24h}
    if (overflow(self) != 0) return 0;                                 // 00654B1B..00654B32
    U base_item;                                                       // {E-1Ch}
    U variation_slot = 0;                                              // {E-20h} (BYTE written; low byte consumed)
    U8 extra_a, extra_b;                                               // {E-25h}, {E-26h}
    U8 variation = 0;
    (void)recovered_client_handle_item_appearance_0064e730(self, nullptr, &base_item, &variation, &extra_a, &extra_b);   // 00654B33..00654B4C
    variation_slot = variation;
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 00654B51..00654B68 ebx
    void* const array = object_array();                                // 00654B5F..00654B7C {E-18h}
    void* object;                                                      // {E-14h}
    void* item;                                                        // edi
    if (kotor_fastcall_004d8230(array, nullptr, id, &object) == kArraySuccess_0074666c) {   // 00654B70..00654B8B
        item = vs<V0>(object, 0x34)(object, nullptr);                  // 00654BE4..00654BED
    } else {
        item = recovered_client_create_item_0064b540(self, nullptr, base_item, variation_slot, 0x4D);   // 00654B8D..00654BA0
        if (item == nullptr) return 0;                                 // 00654BA2..00654BA4
        void* const sub = static_cast<unsigned char*>(item) + 0x10;    // 00654BAD
        vs<V1>(sub, 0x04)(sub, nullptr, id);                           // 00654BA6..00654BB3
        vs<VP>(sub, 0x84)(sub, nullptr, area);                         // 00654BB6..00654BBB
        if (recovered_object_array_add_external_004d7ff0(array, nullptr, &id, sub, 1) != kArraySuccess_0074666c)
            return 0;                                                  // 00654BC1..00654BD8
    }
    void* const sub = static_cast<unsigned char*>(item) + 0x10;        // 00654BEF esi
    st<U>(item, 0x168, 0);                                             // 00654BF5
    recovered_client_object_set_visibility_on_set_area_0063e350(sub, nullptr, area);   // 00654BF2..00654BFF
    vs<V1>(sub, 0x94)(sub, nullptr, rd32(area, 0x184));                // 00654C04..00654C0F
    (void)recovered_client_ai_master_add_object_00604a20(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr),
                                                         nullptr, sub);   // 00654C15..00654C26
    (void)recovered_client_handle_active_item_properties_0064e3d0(self, nullptr, id);   // 00654C2B..00654C32
    if (vs<LoadModel>(sub, 0x78)(sub, nullptr, at(item, 0x128), 0xFF, 0) == 0) return 0;   // 00654C37..00654C4E
    void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(rp(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                                         rd32(item, 0x0C));   // 00654C50..00654C5D
    if (rp(base, 0x48) != nullptr) {                                   // 00654C62..00654C67
        ResRef hook;                                                   // {E-10h}
        reinterpret_cast<menu_resources::Ref*>(&hook)->construct_00406d80(kDefault_0073df6c);   // 00654C69..00654C72
        using AttachHook = U (__fastcall*)(void*, void*, U, U, const void*);   // vtbl B0h, RET 0Ch
        (void)vs<AttachHook>(sub, 0xB0)(sub, nullptr, 0xFF, 1, &hook); // 00654C77..00654C87
    }
    return 1;                                                          // 00654C8D..00654C99
}

namespace {
// The three inline "dummy source/target creature" sequences of 0064C3C0 (0064C552.., 0064C6E7.., 0064C93C..): new
// CSWCCreature (44Ch; NULL -> handler EAX 0), [+310h] = 0, vtbl[78h]("c_dummy", FFh, 0); when that loads and the module
// has an area: AddInternalObject(&id {E-5Ch}, c, 0) (failure -> handler EAX 0), vtbl[4](id), vtbl[84h](area), AI master,
// vtbl[94h]([area+184h]), SetPosition(position), desired position, appearance row [+18h] = C9h. `zero_is_failure`: the
// first copy tests the AddInternalObject status with "test al,al / je fail" (0064C5EB), the others with
// "cmp al,[0074666C] / jne fail".
struct Dummy { void* creature; bool fail; };
Dummy make_dummy(const Vector3* position, bool zero_is_failure) noexcept(false) {
    void* const c = new_object(0x44C, [](void* m) { return kotor_m15a_creature_construct_00616a20(m, nullptr); });   // 0064C552..0064C576
    if (c == nullptr) return {nullptr, true};                          // 0064C578..0064C582
    ResRef model;                                                      // {E-2Ch} / {E-1Ch}
    reinterpret_cast<menu_resources::Ref*>(&model)->construct_00406d80(kDummyModel_00751cd8);   // 0064C588..0064C597
    st<U>(c, 0x310, 0);                                                // 0064C591
    if (vs<LoadModel>(c, 0x78)(c, nullptr, &model, 0xFF, 0) == 0) return {c, false};   // 0064C59C..0064C5B0
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0064C5B6..0064C5C4
    if (area == nullptr) return {c, false};                            // 0064C5C7..0064C5C9
    U id;                                                              // {E-5Ch}
    const U8 status = area_vfx_register_existing_004d80e0(object_array(), nullptr, &id, c, 0);   // 0064C5CF..0064C5E6
    if (zero_is_failure ? status == 0 : status != kArraySuccess_0074666c) return {c, true};   // 0064C5EB / 0064C783
    vs<V1>(c, 0x04)(c, nullptr, id);                                   // 0064C5F3..0064C5FC
    vs<VP>(c, 0x84)(c, nullptr, area);                                 // 0064C5FF..0064C604
    (void)recovered_client_ai_master_add_object_00604a20(kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr),
                                                         nullptr, c);  // 0064C60A..0064C61A
    vs<V1>(c, 0x94)(c, nullptr, rd32(area, 0x184));                    // 0064C61F..0064C62A
    vs<VP>(c, 0x8C)(c, nullptr, position);                             // 0064C630..0064C639
    st<Vector3>(c, 0x1DC, *position);                                  // 0064C63F..0064C656
    st<U16>(rp(c, 0x21C), 0x18, 0xC9);                                 // 0064C659..0064C65F
    return {c, false};
}
// fade-in time of the beam dummies: 6000 ms when the read duration {E-64h} equals 0.0, else ftol(duration * 1000).
U beam_fade_time(float duration) noexcept {
    // fld; fcomp [0073D700]; test ah,44h; jp -> computed (not equal or unordered)
    if (static_cast<double>(duration) == static_cast<double>(kZero_0073d700)) return 0x1770;   // 0064C669..0064C680
    return ftol2_low(static_cast<double>(duration) * kThousand_0073d6fc);                       // 0064C682..0064C691
}
}  // namespace

// 0064C3C0 CSWCMessage::HandleServerToPlayerCreatureUpdate_VisEffect. RET. OBJECTID target, WORD effect, Vector position,
// beam: OBJECTID source, OBJECTID beam target, BYTE a, BYTE b, Vector source position [, FLOAT duration when the target
// is invalid]. Missing endpoints are stood in for by "c_dummy" creatures; then AddEffect (vtbl B4h) on the target.
// Quirks kept: the first dummy (beam source) treats a successful AddInternalObject (status 0) as failure (EAX 0, the
// dummy stays registered nowhere and leaks); the existing-source branch does nothing with the source object.
U __fastcall recovered_client_handle_creature_visual_effect_0064c3c0(void* self, void*) {
    Vector3 source_position = {0.0f, 0.0f, 0.0f};                      // 0064C3DE..0064C3EE {E-38h}
    Vector3 position = {0.0f, 0.0f, 0.0f};                             // 0064C3F6..0064C406 {E-44h}
    float duration = 0.0f;                                             // 0064C40E {E-64h}
    const U target_id = rid(self);                                     // 0064C416..0064C421 ebp / {E-4Ch}
    const U effect = rw(self);                                         // 0064C41D..0064C430 edi / {E-60h}
    if (overflow(self) != 0) return 0;                                 // 0064C42A..0064C43B
    position.x = rf(self); position.y = rf(self); position.z = rf(self);   // 0064C441..0064C473
    if (w2::g07::is_beam_type_004ca960(effect) == 0) {                 // 0064C477..0064C484
        // ---------------------------------------------------------------- non-beam (0064C910)
        if (overflow(self) != 0) return 0;                             // 0064C910..0064C919
        if (underflow(self) != 0) return 0;                            // 0064C91F..0064C92A
        void* target;
        if (target_id == kInvalidObject_00746644) {                    // 0064C930..0064C936
            const Dummy d = make_dummy(&position, false);              // 0064C93C..0064CA59 (EH state 2)
            if (d.fail) return 0;
            recovered_client_object_set_desired_fade_state_0063ce60(d.creature, nullptr, 1, 1, 0x2710, 0);   // 0064CA5D..0064CA69
            st<U>(d.creature, 0xE4, 1);                                // 0064CA74
            target = d.creature;                                       // 0064CA7F
        } else {
            target = kotor_fastcall_005ed580(client_app(), nullptr, target_id);   // 0064CA83..0064CA8D
            if (target == nullptr) return 0;                           // 0064CA94..0064CA96
        }
        (void)vs<AddEffect>(target, 0xB4)(target, nullptr, effect, 1, kInvalidObject_00746644, kInvalidObject_00746644,
                                          0, 0, &position);            // 0064CA98..0064CAB0
        return 1;                                                      // 0064CAB6..0064CACD
    }
    // -------------------------------------------------------------------- beam (0064C48A)
    const U source_id = rid(self);                                     // 0064C48A..0064C491 ebx
    const U beam_target = rid(self);                                   // 0064C493..0064C49C {E-50h}
    const U8 a = rb(self);                                             // 0064C498..0064C4A9 {E-54h} low byte
    const U8 b = rb(self);                                             // 0064C4A5..0064C4BB {E-58h} low byte
    source_position.x = rf(self); source_position.y = rf(self); source_position.z = rf(self);   // 0064C4B2..0064C4E8
    if (target_id == kInvalidObject_00746644) duration = rf(self);     // 0064C4EC..0064C502
    if (overflow(self) != 0) return 0;                                 // 0064C506..0064C511
    void* const source = kotor_fastcall_005ed580(client_app(), nullptr, source_id);   // 0064C517..0064C520
    const auto is_world = [](void* o) {                                // creature / placeable / door
        const U8 t = ld<U8>(o, 8);
        return t == kCreature_00746634 || t == kPlaceable_00746638 || t == kDoor_00746639;
    };
    if (source == nullptr || !is_world(source)) {                      // 0064C525..0064C54C
        const Dummy d = make_dummy(&source_position, true);            // 0064C552..0064C665 (EH state 0)
        if (d.fail) return 0;
        recovered_client_object_set_desired_fade_state_0063ce60(d.creature, nullptr, 1, 1, beam_fade_time(duration), 0);   // 0064C669..0064C698
        st<U>(d.creature, 0xE4, 1);                                    // 0064C69D
    }
    void* target;                                                      // ecx at 0064C849
    void* const existing = kotor_fastcall_005ed580(client_app(), nullptr, target_id);   // 0064C6A7..0064C6B5
    if (existing != nullptr && is_world(existing)) {                   // 0064C6BA..0064C6D7
        target = vs<V0>(existing, 0x0C)(existing, nullptr);            // 0064C6D9..0064C6E0
    } else {
        const Dummy d = make_dummy(&position, false);                  // 0064C6E7..0064C801 (EH state 1)
        if (d.fail) return 0;
        recovered_client_object_set_desired_fade_state_0063ce60(d.creature, nullptr, 1, 1, beam_fade_time(duration), 0);   // 0064C805..0064C834
        st<U>(d.creature, 0xE4, 1);                                    // 0064C839
        target = d.creature;                                           // 0064C843
    }
    // 0064C849: a2 = 1 only when duration == 0.0 (ordered); the vector is the position for an invalid target id, else a
    // fresh zero vector {E-2Ch}.
    const U a2 = static_cast<double>(duration) == static_cast<double>(kZero_0073d700) ? 1u : 0u;   // 0064C849..0064C85F
    Vector3 zero_2c = {0.0f, 0.0f, 0.0f};                              // 0064C86B..0064C87B / 0064C8C3..0064C8D3
    const Vector3* const vector = target_id == kInvalidObject_00746644 ? &position : &zero_2c;   // 0064C861..0064C887
    (void)vs<AddEffect>(target, 0xB4)(target, nullptr, effect, a2, source_id, beam_target, a, b, vector);   // 0064C887..0064C8F2
    return 1;                                                          // 0064C8A1..0064C90F
}

// 006517B0 CSWCMessage::HandleServerToPlayerAreaOfEffectUpdate_Delete. RET. OBJECTID; overflow -> 0; a known area of
// effect stops its effect. EAX 1.
U __fastcall recovered_client_handle_aoe_delete_006517b0(void* self, void*) {
    const U id = rid(self);                                            // 006517B2..006517BD
    if (overflow(self) != 0) return 0;                                 // 006517B9..006517CC
    void* const aoe = recovered_client_get_aoe_by_id_005ed610(client_app(), nullptr, id);   // 006517CD..006517D6
    if (aoe != nullptr) recovered_client_aoe_stop_effect_006d9350(aoe, nullptr);   // 006517DB..006517E1
    return 1;                                                          // 006517E6..006517ED
}
// 006517F0 CSWCMessage::HandleServerToPlayerAreaOfEffectUpdate_Update(id, mask). RET 8. No reads: EAX = (area of
// effect id is known).
U __fastcall recovered_client_handle_aoe_update_006517f0(void*, void*, U id, U) {
    return recovered_client_get_aoe_by_id_005ed610(client_app(), nullptr, id) != nullptr ? 1u : 0u;   // 006517F0..00651808
}
}  // extern "C"
