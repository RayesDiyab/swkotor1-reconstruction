// lane-m15-a (L15-A, Milestone 1.5) unit m15a-object: CSWCObject members (0063C060..0063E530), CSWCItem powered
// animations (00646600, 00646BA0, 00647130) and the animation event SwitchWeaponEvent 00610F40.
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only (nothing executed); every behaviour
// cites its instruction addresses. Conventions: m15a_common.h (D1 fastcall veneers, D21/D24 typed slot views of
// receiver vptrs, opaque object views through rp/rd32/ld/st). D30-FINAL: no float arithmetic here (floats are
// only passed through or stored as constants).
#include "468a2f31a138f45b_m15a_object.h"
#include "8d831d5c6afcac70_m15a_creature.h"   // sibling unit m15a-creature: GetEquippedItemID 0060CA80, GetLastInstant 0060CED0
#include "f155d76cd432eed7_m15_l0.h"          // lane-m15-0: sound source leaves, client getters, VFX list ctor, AttachmentFromClientObject
#include "bf662019daaaa2b6_m15b_stats.h"      // lane-m15-b: CSWPortrait ctor, base item getters
#include "e1805047c9b7563b_startup_string.h"  // M40 kotor_reconstruction::StartupString (CExoString 005B3190 / 005E5A90 / 005E5C20)
#include "46fde490a32f5ba2_hover_path.h"      // lane-input: kotor_input_hover::SoundSource::Play 005D5930 (m48b-gui-input.obj)
#include "dc3735945422a215_menu_continuation.h" // lane-r4-lowers: menu_continuation::InGame::IsGlobalFading_0062ac60
#include <cstring>
#include <new>

using namespace kotor_m15a;
using kotor_reconstruction::StartupString;

// ================================================================== external providers (M60 map symbols)
extern "C" {
// cells
extern void* kotor_cell_rules_007a3a28;                       // game-prepare-prepare-image-data.obj (CSWRules*)
extern void* kotor_cell_sound_007a39ec;                       // sound-owners.obj (CExoSound*, D25)
extern std::uint32_t kotor_rules_cell32_007a2154;             // rules-rules-image-data.obj (animations.2da looping column)
extern std::uint32_t kotor_rules_cell32_007a2158;             // rules-rules-image-data.obj (animations.2da stationary column)
// 00413510 C2DA::GetINTEntry(int row, int column, int* out) RET 0Ch (tlk-c2da-rowcol.obj)
int __fastcall c2da_current_get_int_entry_00413510(void* table, void*, int row, int column, int* out);
// 00413430 C2DA::GetFLOATEntry(int row, const CExoString* column, float* out) RET 0Ch (startup-misc-c2da.obj)
int __fastcall c2da_current_get_float_00413430(void* table, void*, int row, void* column, float* out);
// 00406D60 CResRef::CResRef(const CExoString&) RET 4 (ref-bodies.obj)
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* text);
// 0063C070 [this+68h] getter (3d-render-vtables.obj)
void* __fastcall gob_get_property_0x68_0063c070(void* object, void*);
// 004AEA30 CServerExoApp::ClientToServerObjectId(id) RET 4 / 004AE750 CServerExoApp::GetGameObject(id) RET 4
std::uint32_t __fastcall kotor_fastcall_004aea30(void* server, void*, std::uint32_t id);
void* __fastcall kotor_fastcall_004ae750(void* server, void*, std::uint32_t id);
// CClientExoApp (outer) getters
void* __fastcall frame_R4_005ed5b0(void* app, void*, std::uint32_t id);            // 005ED5B0 GetCreatureByGameObjectID
void* __fastcall client_get_player_creature_005ed540(void* app, void*);            // 005ED540 GetPlayerCreature
void* __fastcall kotor_fastcall_005ed530(void* app, void*);                         // 005ED530 GetModule
void* __fastcall frame_R4_005ed690(void* app, void*);                               // 005ED690 GetInGameGui
// 006FA7E6 operator new (m39-directory-allocator.obj; returns 0 on failure) / 006FA390 free (live-policy.obj)
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);
void __cdecl live_free(void*);
}
namespace kotor_binding_required {
// 004D7D60 CGameObject::CGameObject(BYTE type, DWORD id) RET 8 (game-object-ctor.obj)
void* __fastcall game_object_004D7D60(void* self, void*, unsigned int type, unsigned int id);
}
namespace kotor_sound_rev3 {
// 005D60E0 CExoSoundSource::CExoSoundSource(const CResRef&) RET 4 / 005D5900 SetPriorityGroup(group) RET 4
void* __fastcall SourceConstruct005d60e0(void* self, void*, const void* ref);
void __fastcall SourceSetPriorityGroup005d5900(void* self, void*, unsigned int group);
}
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }   // window-shared-cells.obj (AppManager*)

namespace {
// ------------------------------------------------------------------ typed slot views (D24)
// CSWCAnimBase family (object +68h; vtable 00754F60 for CSWCAnimBase).
struct AnimBaseSlots {
    const void* s00_04[2];
    void* (__fastcall* get_model)(void*, void*, Word part);                                      // +08 0069CDA0 RET 4
    const void* s0c_14[3];
    Word (__fastcall* replace_texture18)(void*, void*, const void*, const void*, Word);           // +18 0069CDC0 RET 0Ch
    Word (__fastcall* replace_texture1c)(void*, void*, Word, const void*, const void*, Word);     // +1C 0069CE50 RET 10h
    Word (__fastcall* restore_texture)(void*, void*, Word);                                       // +20 0069D100 RET 4
    const void* s24_50[12];
    Word (__fastcall* set_animation)(void*, void*, Word anim, float speed, Word, Word);           // +54 0069D4F0 RET 10h
    const void* s58_78[9];
    Word (__fastcall* attach_item)(void*, void*, Word id, void* hook, Word, float scale);        // +7C (00401590 base) RET 10h
    const void* s80_dc[24];
    Word (__fastcall* server_to_client_animation)(void*, void*, Word);                            // +E0 0069F650 RET 4
    void (__fastcall* enable_distortion)(void*, void*);                                           // +E4 0069DE50
    void (__fastcall* disable_distortion)(void*, void*);                                          // +E8 0069DE70
};
static_assert(offsetof(AnimBaseSlots, get_model) == 0x08 && offsetof(AnimBaseSlots, replace_texture18) == 0x18 &&
              offsetof(AnimBaseSlots, restore_texture) == 0x20 && offsetof(AnimBaseSlots, set_animation) == 0x54 &&
              offsetof(AnimBaseSlots, attach_item) == 0x7C && offsetof(AnimBaseSlots, server_to_client_animation) == 0xE0 &&
              offsetof(AnimBaseSlots, disable_distortion) == 0xE8, "anim base slots");
// Aurora gob (result of anim base +08 GetModel / object +98 GetModel).
struct ModelSlots {
    const void* s00_14[6];
    void (__fastcall* play_animation)(void*, void*, const char*, float, Word, Word);             // +18 (006473AA)
    const void* s1c_60[18];
    void (__fastcall* get_position)(void*, void*, Vector* out);                                   // +64 (00647265)
    const void* s68_6c[2];
    void* (__fastcall* client_object)(void*, void*);                                              // +70 (00611256)
    const void* s74_c0[20];
    void (__fastcall* set_render_flags)(void*, void*, Word);                                      // +C4 (006110D8)
};
static_assert(offsetof(ModelSlots, play_animation) == 0x18 && offsetof(ModelSlots, get_position) == 0x64 &&
              offsetof(ModelSlots, client_object) == 0x70 && offsetof(ModelSlots, set_render_flags) == 0xC4, "gob slots");
// CSWCObject / CSWCCreature method table (007516C8 / 0074F5F8): slots called here.
struct ObjectSlots {
    const void* s00_94[0x26];
    void* (__fastcall* get_model)(void*, void*, Word part, Word flag);                            // +98 (0063C080 / 006179C0 RET 8)
    void (__fastcall* set_animation)(void*, void*, Word anim, Word, Word);                        // +9C 0063E930 RET 0Ch
    const void* s0a0_c8[11];
    Word (__fastcall* has_visual_effect_applied)(void*, void*, Word);                             // +CC 0063DDF0 RET 4
};
static_assert(offsetof(ObjectSlots, get_model) == 0x98 && offsetof(ObjectSlots, set_animation) == 0x9C &&
              offsetof(ObjectSlots, has_visual_effect_applied) == 0xCC, "object slots");
// CSWSObject-family server object (CGameObject vtable): +10 AsSWSObject.
struct ServerObjectSlots {
    const void* s00_0c[4];
    void* (__fastcall* as_sws_object)(void*, void*);                                              // +10 (0063D4F7)
};
static_assert(offsetof(ServerObjectSlots, as_sws_object) == 0x10, "server object slots");

const AnimBaseSlots* anim_slots(const void* anim) { return static_cast<const AnimBaseSlots*>(rp(anim, 0)); }
const ModelSlots* model_slots(const void* model) { return static_cast<const ModelSlots*>(rp(model, 0)); }
const ObjectSlots* object_slots(const void* object) { return static_cast<const ObjectSlots*>(rp(object, 0)); }
void* app_manager() { return kotor_menu_window_cells::manager_007a39fc; }               // [007A39FC]
void* client_app() { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }          // [[007A39FC]+4]
// [[g_pRules(007A3A28)+B8h]+off]: the CSWRules 2DA collection (+B8h) table at off.
void* rules_2da(unsigned off) { return rp(rp(kotor_cell_rules_007a3a28, 0xB8), off); }
void* base_item(void* item) {                                                            // rules+34h ->GetBaseItem([item+0Ch])
    return kotor_m15b_base_item_array_get_base_item_005b31a0(rp(kotor_cell_rules_007a3a28, 0x34), nullptr,
                                                             ld<std::int32_t>(item, 0x0C));
}
float float_bits(Word bits) { float f; std::memcpy(&f, &bits, sizeof f); return f; }
constexpr Word kInvalidObjectId = 0x7F000000u;                                           // .rdata 00746644 = 7F000000h
bool same_text(const char* a, const char* b) { return std::strcmp(a, b) == 0; }         // repe cmpsb over b's bytes + NUL
}

extern "C" {
// ================================================================== CSWCObject
// 0063C060: mov ax,[ecx+48h]; ret.
std::uint16_t __fastcall kotor_m15a_object_get_current_animation_0063c060(void* self, void*) {
    return ld<std::uint16_t>(self, 0x48);                                               // 0063C060
}
// 0063C0A0: anim base = [ecx+68h]; 0 -> return 0 (RET 10h); else jmp [[anim]+1Ch] with the four stack words.
std::uint32_t __fastcall kotor_m15a_object_replace_texture_0063c0a0(void* self, void*, std::uint32_t part,
    const void* old_tex, const void* new_tex, std::uint32_t keep) noexcept(false) {
    void* const anim = rp(self, 0x68);                                                   // 0063C0A0
    if (!anim) return 0;                                                                 // 0063C0A3..A9
    return anim_slots(anim)->replace_texture1c(anim, nullptr, part, old_tex, new_tex, keep);  // 0063C0AC..AE
}
// 0063C0C0: as above through slot +18h (three stack words, RET 0Ch).
std::uint32_t __fastcall kotor_m15a_object_replace_texture_0063c0c0(void* self, void*, const void* old_tex,
    const void* new_tex, std::uint32_t keep) noexcept(false) {
    void* const anim = rp(self, 0x68);                                                   // 0063C0C0
    if (!anim) return 0;                                                                 // 0063C0C3..C9
    return anim_slots(anim)->replace_texture18(anim, nullptr, old_tex, new_tex, keep);   // 0063C0CC..CE
}
// 0063C100: mov ax,[ecx+58h]; ret.
std::uint16_t __fastcall kotor_m15a_object_get_looping_animation_0063c100(void* self, void*) {
    return ld<std::uint16_t>(self, 0x58);                                               // 0063C100
}
// 0063C170 AnimationStationary(anim) RET 4.
std::uint32_t __fastcall kotor_m15a_object_animation_stationary_0063c170(void* self, void*, std::uint32_t animation) noexcept(false) {
    int stationary = 0;                                                                  // 0063C176 [esp] = 0
    void* const anim = rp(self, 0x68);                                                   // 0063C171
    Word row = animation;                                                                // 0063C18F (no anim base: eax = arg)
    if (anim) row = anim_slots(anim)->server_to_client_animation(anim, nullptr, animation);   // 0063C180..87
    // 0063C193..B2: GetINTEntry([[rules+B8h]+3Ch], movzx ax, [007A2158], &stationary)
    if (c2da_current_get_int_entry_00413510(rules_2da(0x3C), nullptr, static_cast<std::uint16_t>(row),
                                            static_cast<int>(kotor_rules_cell32_007a2158), &stationary) != 0)
        return stationary != 0 ? 1u : 0u;                                                // 0063C1BB..C7 setne
    return 1;                                                                            // 0063C1CA
}
// 0063C300 AnimationLooping(anim) RET 4.
std::uint32_t __fastcall kotor_m15a_object_animation_looping_0063c300(void* self, void*, std::uint32_t animation) noexcept(false) {
    Word row = animation;                                                                // 0063C304
    if (ld<U8>(self, 0x08) == 5) {                                                       // 0063C300 creature object type
        const std::uint16_t ax = static_cast<std::uint16_t>(animation);
        if ((ax >= 0x578 && ax <= 0x5F7) || (ax >= 0x640 && ax <= 0x6BF)) return 1;     // 0063C30A..27
    }
    int looping = 0;                                                                     // 0063C32F [esp+4] = 0
    void* const anim = rp(self, 0x68);                                                   // 0063C32A
    if (anim) row = anim_slots(anim)->server_to_client_animation(anim, nullptr, row);   // 0063C337..3C
    // 0063C342..61: GetINTEntry([[rules+B8h]+3Ch], movzx ax, [007A2154], &looping)
    if (c2da_current_get_int_entry_00413510(rules_2da(0x3C), nullptr, static_cast<std::uint16_t>(row),
                                            static_cast<int>(kotor_rules_cell32_007a2154), &looping) != 0)
        return looping != 0 ? 1u : 0u;                                                   // 0063C36A..75 setne
    return 0;                                                                            // 0063C378
}
// 0063CAD0 AbortFireForgetAnimation(keep) RET 4.
void __fastcall kotor_m15a_object_abort_fire_forget_animation_0063cad0(void* self, void*, std::uint32_t keep) noexcept(false) {
    st<Word>(self, 0x64, 0);                                                             // 0063CAD6
    if (keep == 0)                                                                       // 0063CAD4/DD
        object_slots(self)->set_animation(self, nullptr, ld<std::uint16_t>(self, 0x58), 0, 0);   // 0063CADF..EC SetAnimation(+9Ch)
}
// 0063CE40 RestoreTexture(part) RET 4: tail jump through anim base +20h.
std::uint32_t __fastcall kotor_m15a_object_restore_texture_0063ce40(void* self, void*, std::uint32_t part) noexcept(false) {
    void* const anim = rp(self, 0x68);                                                   // 0063CE40
    if (!anim) return 0;                                                                 // 0063CE43..49
    return anim_slots(anim)->restore_texture(anim, nullptr, part);                       // 0063CE4C..4E
}
// 0063D4B0 GetServerObject() RET.
void* __fastcall kotor_m15a_object_get_server_object_0063d4b0(void* self, void*) noexcept(false) {
    if (rp(self, 0xF8) == nullptr) {                                                     // 0063D4B3..BB cached [this+F8h]
        if (rd32(self, 0xE4) != 0) return nullptr;                                       // 0063D4BD..C5 -> 0063D4D3
        void* const server = rp(app_manager(), 8);                                       // 0063D4C7..CC server [[007A39FC]+8]
        if (server == nullptr) return nullptr;                                           // 0063D4CF..D1
        const std::uint32_t server_id =
            kotor_fastcall_004aea30(server, nullptr, rd32(self, 0x04));                  // 0063D4D7..DB ClientToServerObjectId([this+4])
        void* const game_object = kotor_fastcall_004ae750(rp(app_manager(), 8), nullptr, server_id);  // 0063D4E0..EA
        if (game_object) {                                                               // 0063D4EF
            void* const object = static_cast<const ServerObjectSlots*>(rp(game_object, 0))
                                     ->as_sws_object(game_object, nullptr);              // 0063D4F3..F7 slot +10h
            st<void*>(self, 0xF8, object);                                               // 0063D4FC
            if (object)                                                                  // 0063D4FA/502
                kotor_m15_sobject_attachment_from_client_004cc310(
                    object, nullptr, static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(self)));  // 0063D504..07 push esi
        }
    }
    return rp(self, 0xF8);                                                               // 0063D50C
}
// 0063D540 / 0063D550: anim base [this+68h] -> jmp [[anim]+E4h] / [[anim]+E8h]; RET without one.
void __fastcall kotor_m15a_object_enable_distortion_0063d540(void* self, void*) noexcept(false) {
    void* const anim = rp(self, 0x68);                                                   // 0063D540
    if (anim) anim_slots(anim)->enable_distortion(anim, nullptr);                        // 0063D543..49
}
void __fastcall kotor_m15a_object_disable_distortion_0063d550(void* self, void*) noexcept(false) {
    void* const anim = rp(self, 0x68);                                                   // 0063D550
    if (anim) anim_slots(anim)->disable_distortion(anim, nullptr);                       // 0063D553..59
}
// 0063D830 SetLoopingAnimation(anim) RET 4.
void __fastcall kotor_m15a_object_set_looping_animation_0063d830(void* self, void*, std::uint32_t animation) noexcept(false) {
    if (kotor_m15a_object_animation_looping_0063c300(self, nullptr, animation) == 1)   // 0063D836..41
        st<std::uint16_t>(self, 0x58, static_cast<std::uint16_t>(animation));            // 0063D843 mov [esi+58h],di
}
// 0063E530 CSWCObject::CSWCObject(BYTE type) RET 4, EAX = this.
// EH (FuncInfo via 00729B5A): states 0..4 cover CGameObject / VFX list / CSWPortrait / CExoString members; the
// unwind funclets are not emitted (D30-FINAL: the member constructors do not throw on reachable paths; original
// operator new returns 0).
void* __fastcall kotor_m15a_object_construct_0063e530(void* self, void*, std::uint32_t object_type) noexcept(false) {
    kotor_binding_required::game_object_004D7D60(self, nullptr, object_type, kInvalidObjectId);  // 0063E54F..5B (type, 7F000000h)
    st<const void*>(self, 0x00, &kotor_m15e_object_methods_007516c8);                    // 0063E562 vptr 007516C8 (L15-E table)
    st<Word>(self, 0x24, 0); st<Word>(self, 0x28, 0); st<Word>(self, 0x2C, 0);          // 0063E568..6E
    st<Word>(self, 0x30, 0); st<Word>(self, 0x34, 0); st<Word>(self, 0x38, 0);          // 0063E578..81
    kotor_m15_vfx_list_construct_0063d7b0(at(self, 0x6C), nullptr);                     // 0063E571/81 CExoLinkedList<VFX>
    st<Word>(self, 0x74, 0); st<Word>(self, 0x78, 0); st<Word>(self, 0x70, 0);          // 0063E586..8C
    kotor_m15b_portrait_construct_00678ec0(at(self, 0xA8), nullptr);                     // 0063E58F..9A CSWPortrait
    st<Word>(self, 0xD8, 0); st<Word>(self, 0xDC, 0); st<Word>(self, 0xE0, 0);          // 0063E5AA..B6
    new (at(self, 0xE8)) StartupString();                                                // 0063E59F/BC CExoString 005B3190
    st<Word>(self, 0x20, 0); st<Word>(self, 0x68, 0); st<Word>(self, 0xF4, 0);          // 0063E5CB..D1
    st<std::uint16_t>(self, 0x48, 0); st<std::uint16_t>(self, 0x4A, 0);                 // 0063E5D7..DB
    st<std::uint16_t>(self, 0x58, 0); st<Word>(self, 0x64, 0);                           // 0063E5DF..E3
    st<std::uint16_t>(self, 0x5A, 8);                                                    // 0063E5E6
    void* const table = c2da_directory_new_006fa7e6(0xE0);                               // 0063E5C1..EC operator new(E0h)
    if (table) {                                                                         // 0063E5F4..FB
        for (unsigned i = 0; i < 8; ++i) {                                               // 0063E600..24: 8 entries of 1Ch
            unsigned char* const e = static_cast<unsigned char*>(table) + i * 0x1C;
            st<Word>(e, 0x00, 0);                                                        // 0063E605 [ecx-8]
            st<std::uint16_t>(e, 0x04, 0x2710);                                          // 0063E608 word [ecx-4]
            st<Word>(e, 0x08, 0);                                                        // 0063E60E [ecx]
            st<Word>(e, 0x0C, 0);                                                        // 0063E610 [ecx+4]
            st<Word>(e, 0x10, 0x1F4);                                                    // 0063E613 [ecx+8]
            st<float>(e, 0x14, 1.0f);                                                    // 0063E61A [ecx+0Ch] = 3F800000h
            st<Word>(e, 0x18, 0);                                                        // 0063E61D [ecx+10h]
        }
    }
    st<void*>(self, 0x5C, table);                                                        // 0063E62A (0 when new failed, 0063E628)
    st<U8>(self, 0x90, 1); st<U8>(self, 0x91, 1); st<U8>(self, 0x60, 0);                // 0063E632..3E
    st<Word>(self, 0x1C, 0); st<float>(self, 0x4C, 1.0f);                                // 0063E641..44
    st<Word>(self, 0x80, 0); st<Word>(self, 0x84, 0); st<float>(self, 0x98, 1.0f);      // 0063E647..53
    st<Word>(self, 0x94, 0); st<Word>(self, 0x9C, 0); st<Word>(self, 0x88, 0);          // 0063E659..65
    st<Word>(self, 0xA0, 0);                                                             // 0063E66B
    st<Word>(self, 0x0C, 0); st<Word>(self, 0x10, 0); st<Word>(self, 0x14, 0); st<Word>(self, 0x18, 0);  // 0063E671..7A
    st<Word>(self, 0xC8, 0); st<Word>(self, 0xCC, 0); st<Word>(self, 0xD0, 0);          // 0063E67D..89
    st<Word>(self, 0xC0, 0); st<Word>(self, 0xC4, 0);                                    // 0063E68F..95
    st<Word>(self, 0xA4, 0x40066666u);                                                   // 0063E69B (2.1f bits)
    st<std::uint16_t>(self, 0xBA, 0x2710);                                               // 0063E6A5
    st<Word>(self, 0xD4, 1);                                                             // 0063E6B4 (eax = 1)
    st<Word>(self, 0xBC, kInvalidObjectId);                                              // 0063E6AE..BA [00746644]
    st<float>(self, 0xD8, 0.0f); st<float>(self, 0xDC, 0.0f); st<float>(self, 0xE0, 1.0f);   // 0063E6C0..F0 Vector(0,0,1)
    st<Word>(self, 0x7C, 0); st<Word>(self, 0xE4, 0); st<Word>(self, 0xF0, 0);          // 0063E6FA..703
    st<Word>(self, 0x50, 0); st<Word>(self, 0x54, 0); st<Word>(self, 0xF8, 0);          // 0063E709..0F
    st<std::uint16_t>(self, 0xB8, 0xFFFF);                                               // 0063E716
    return self;                                                                         // 0063E71F
}

// ================================================================== CSWCItem
// 00646600 SetEquippedBy(creature) RET 4.
void __fastcall kotor_m15a_item_set_equipped_by_00646600(void* self, void*, void* creature) {
    if (creature) st<Word>(self, 0x160, rd32(creature, 0x04));                           // 00646604..0B
    else st<Word>(self, 0x160, kInvalidObjectId);                                        // 00646614
}
// 00646BA0 ResolveCreaturePoweredAnimations(powered) RET 4.
void __fastcall kotor_m15a_item_resolve_creature_powered_animations_00646ba0(void* self, void*, std::uint32_t powered) noexcept(false) {
    void* const creature = frame_R4_005ed5b0(client_app(), nullptr, rd32(self, 0x160));  // 00646BA0..B3
    if (!creature) return;                                                               // 00646BBA..BC
    U8 hands = 0;                                                                        // 00646BCB xor bl,bl
    if (kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x10) == rd32(self, 0x14)) {  // 00646BC4..D4
        hands = 1;                                                                       // 00646BD6
    } else if (kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x20) == rd32(self, 0x14)) {  // 00646BDA..E8
        hands = 2;                                                                       // 00646BEA
    }
    if (gob_get_property_0x68_0063c070(creature, nullptr) == nullptr) return;           // 00646BEC..F5
    if (powered != 0) {                                                                  // 00646BFB..01
        if (rd32(self, 0x19C) == 0)                                                      // 00646C03..0B
            hands = static_cast<U8>(hands | ld<U8>(gob_get_property_0x68_0063c070(creature, nullptr), 0xDC));   // 00646C0D..14
    } else if (rd32(self, 0x19C) != 0) {                                                 // 00646C1C..24
        const U8 current = ld<U8>(gob_get_property_0x68_0063c070(creature, nullptr), 0xDC);   // 00646C26..2D
        hands = static_cast<U8>(static_cast<U8>(~hands) & current);                      // 00646C33..35
    } else {
        hands = static_cast<U8>(hands & 0xFC);                                           // 00646C39
    }
    // 00646C3C..6A: base item +ACh (weapon wield) 27h / 28h / 29h -> clear bit 2, else set it (GetBaseItem per test)
    if (ld<U8>(kotor_m15b_item_get_base_item_005b4790(self, nullptr), 0xAC) == 0x27 ||
        ld<U8>(kotor_m15b_item_get_base_item_005b4790(self, nullptr), 0xAC) == 0x28 ||
        ld<U8>(kotor_m15b_item_get_base_item_005b4790(self, nullptr), 0xAC) == 0x29)
        hands = static_cast<U8>(hands & 0xFB);                                           // 00646C71
    else
        hands = static_cast<U8>(hands | 4);                                              // 00646C6C
    st<Word>(gob_get_property_0x68_0063c070(creature, nullptr), 0xDC, hands);           // 00646C74..7E movzx esi,bl
}
// 00647130 PowerItem(powered, override_state, animate, quiet) RET 10h.
void __fastcall kotor_m15a_item_power_item_00647130(void* self, void*, std::uint32_t powered,
    std::uint32_t override_state, std::uint32_t animate, std::uint32_t quiet) noexcept(false) {
    void* const module = kotor_fastcall_005ed530(client_app(), nullptr);                 // 0064713E..56 GetModule
    if (rp(rp(module, 0x48), 0x264) != nullptr) return;                                  // 0064715B..66 area mini-game
    if (rd32(self, 0x160) != kInvalidObjectId &&                                         // 0064716C..77
        frame_R4_005ed5b0(client_app(), nullptr, rd32(self, 0x160)) != nullptr) {        // 00647179..8A
        void* holder = nullptr;                                                          // 00647199 (id == 7F000000h)
        if (rd32(self, 0x160) != kInvalidObjectId)                                       // 0064718C..97
            holder = frame_R4_005ed5b0(client_app(), nullptr, rd32(self, 0x160));       // 0064719D..A7
        // 006471AC: the holder was found by the same lookup just above (no write in between).
        if (rd32(holder, 0x3D8) == 1 && override_state == 0) return;                    // 006471AC..BB
    }
    if (powered == 1 &&                                                                  // 006471C1..C8
        kotor_m15_client_get_area_not_ready_005edad0(client_app(), nullptr) == powered && // 006471CA..D9
        reinterpret_cast<menu_continuation::InGame*>(frame_R4_005ed690(client_app(), nullptr))
                ->IsGlobalFading_0062ac60() == 0) {                                      // 006471DB..F2
        kotor_m15a_item_resolve_creature_powered_animations_00646ba0(self, nullptr, 0);  // 006471F4..F7 (push eax = 0)
        return;                                                                          // 006471FC
    }
    kotor_m15a_item_resolve_creature_powered_animations_00646ba0(self, nullptr, powered); // 00647201..04
    if (rd32(base_item(self), 0x90) != 1) return;                                        // 00647209..22
    frame_R4_005ed5b0(client_app(), nullptr, rd32(self, 0x160));                         // 00647228..37 (result unused)
    if (powered != rd32(self, 0x19C)) {                                                  // 0064723C..42
        void* const anim = gob_get_property_0x68_0063c070(at(self, 0x10), nullptr);      // 00647248..4B ([item+78h])
        void* const model = anim_slots(anim)->get_model(anim, nullptr, 0xFF);            // 00647250..59
        Vector position;                                                                 // [esp+10h]
        model_slots(model)->get_position(model, nullptr, &position);                     // 0064725C..65 slot +64h
        if (rp(self, 0x1A4) == nullptr) {                                                // 00647268..70
            void* const memory = c2da_directory_new_006fa7e6(8);                         // 00647272..74
            void* source = nullptr;                                                      // 006472BD
            if (memory) {                                                                // 00647282..8C
                try {                                                                    // EH state 0 (00647284): free on throw
                    unsigned char ref[16];                                               // [esp+1Ch] CResRef
                    async_ref_construct_string_00406d60(ref, nullptr, at(base_item(self), 0x94));   // 0064728E..AA
                    source = kotor_sound_rev3::SourceConstruct005d60e0(memory, nullptr, ref);     // 006472AF..B6
                } catch (...) {
                    live_free(memory);
                    throw;
                }
            }
            st<void*>(self, 0x1A4, source);                                              // 006472CB
            kotor_m15_sound_source_set_3d_005d5910(source, nullptr, 1);                  // 006472BF..D1
            kotor_m15_sound_source_set_position_005d59e0(rp(self, 0x1A4), nullptr, &position, 0.0f);   // 006472D6..E3
        }
        Word first_animation;                                                            // ebx
        Word second_animation;                                                           // edi
        if (powered != 0) {                                                              // 006472E8..EE
            first_animation = 0x128; second_animation = 0x129;                           // 006472F2..F7
            if (quiet == 0 && rp(self, 0x1A4) != nullptr) {                              // 006472F0..06
                kotor_sound_rev3::SourceSetPriorityGroup005d5900(rp(self, 0x1A4), nullptr, 0x15);  // 00647308..0A
                kotor_m15_sound_source_set_looping_005d59d0(rp(self, 0x1A4), nullptr, 1);  // 0064730F..17
                kotor_m15_sound_source_set_3d_005d5910(rp(self, 0x1A4), nullptr, 1);       // 0064731C..24
                kotor_m15_sound_source_set_position_005d59e0(rp(self, 0x1A4), nullptr, &position,
                                                             float_bits(quiet));         // 00647329..35 push ebp (0)
                static_cast<kotor_input_hover::SoundSource*>(rp(self, 0x1A4))->Play();   // 0064733A..40 005D5930
            }
        } else {
            first_animation = 0x12A; second_animation = 0x164;                           // 0064734F..54
            if (rp(self, 0x1A4) != nullptr)                                              // 00647347..59
                kotor_m15_sound_source_stop_005d5a20(rp(self, 0x1A4), nullptr);          // 0064735B
        }
        if (animate != 0) {                                                              // 00647360..66
            if (rd32(self, 0x19C) == 0) {                                                // 0064736C..74
                void* const item_anim = rp(self, 0x78);                                  // 0064737A
                if (anim_slots(item_anim)->get_model(item_anim, nullptr, 0xFF) != nullptr) {   // 0064737D..89
                    void* const item_anim2 = rp(self, 0x78);                             // 0064738B
                    void* const gob = anim_slots(item_anim2)->get_model(item_anim2, nullptr, 0xFF);   // 0064738E..95
                    model_slots(gob)->play_animation(gob, nullptr, "NULL", 1.0f, 0, 0);  // 00647398..AA (0073EE04)
                }
                if (quiet == 0) {                                                        // 006473AD..AF
                    unsigned char ref[16];                                               // [esp+1Ch]
                    async_ref_construct_string_00406d60(ref, nullptr, at(base_item(self), 0x9C));  // 006473B5..D0
                    KotorM15VectorWords where = {position.x, position.y, position.z};    // 006473D5..F6 (by value)
                    kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, ref, where,
                        float_bits(quiet), 0x15, quiet, quiet, float_bits(quiet), float_bits(quiet));  // 006473DD..4C
                }
            } else if (quiet == 0) {                                                     // 006473FC..FE
                unsigned char ref[16];                                                   // [esp+2Ch]
                async_ref_construct_string_00406d60(ref, nullptr, at(base_item(self), 0xA4));  // 00647400..1C
                KotorM15VectorWords where = {position.x, position.y, position.z};        // 00647421..42
                kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, ref, where,
                    float_bits(quiet), 0x15, quiet, quiet, float_bits(quiet), float_bits(quiet));  // 00647429..4C
            }
            void* const item_anim = rp(self, 0x78);                                      // 00647451
            anim_slots(item_anim)->set_animation(item_anim, nullptr, first_animation, 1.0f, 1, 0);    // 00647454..60
            void* const item_anim2 = rp(self, 0x78);                                     // 00647463
            anim_slots(item_anim2)->set_animation(item_anim2, nullptr, second_animation, 1.0f, 0x20, 0);  // 00647466..72
        } else {
            void* const item_anim = rp(self, 0x78);                                      // 00647477
            anim_slots(item_anim)->set_animation(item_anim, nullptr, second_animation, 1.0f, 0, 0);  // 0064747A..86
        }
    }
    st<Word>(self, 0x19C, powered);                                                      // 00647489..8D
}

// ================================================================== SwitchWeaponEvent
// 00610F40 (cdecl; arg 1 unused): equips the creature's right (slot 10h) / left (slot 20h) hand items on the
// body model when they changed since the last call ([creature+268h] / [creature+26Ch]) or when forced.
void __cdecl kotor_m15a_switch_weapon_event_00610f40(void*, const char* event, void* creature) noexcept(false) {
    Word force = 0;                                                                      // 00610F68 [esp+10h]
    Word loading = 0;                                                                    // 00610F6C [esp+18h]
    Word animate = 1;                                                                    // 00610F70 [esp+14h]
    if (event) {                                                                         // 00610F64..78
        if (same_text(event, "AppearanceForceUpdate")) {                                 // 00610F7A..8A (0074F4D4, 16h bytes)
            force = 1;                                                                   // 00610F8E
            if (creature && rd32(creature, 0x138) == 0) animate = 0;                     // 00610F8C..A0
        }
        if (same_text(event, "Loading")) {                                               // 00610FA4..B4 (0074F4CC, 8 bytes)
            loading = 1;                                                                 // 00610FB6
            animate = 0;                                                                 // 00610FBE
        }
    }
    void* dropped = nullptr;                                                             // 00610FC4 [esp+48h] = 0
    if (!creature) return;                                                               // 00610FC2..C8
    const Word right_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x10);   // 00610FCE..D2
    void* const right = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, right_id);     // 00610FD7..E5
    const Word left_id = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature, nullptr, 0x20);    // 00610FEA..F0
    void* const left = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, left_id);       // 00610FF5..11003
    float scale;                                                                         // [esp+4Ch]
    {
        StartupString column("WEAPONSCALE");                                             // 0061100A..17 (0074F4C0)
        // 0061101C..47: GetFLOATEntry([[rules+B8h]+0Ch], appearance row [[creature+21Ch]+18h], column, &scale)
        const int found = c2da_current_get_float_00413430(rules_2da(0x0C), nullptr,
            ld<std::uint16_t>(rp(creature, 0x21C), 0x18), &column, &scale);
        if (found == 0) scale = 1.0f;                                                    // 0061104C..69 (bl = !found)
    }                                                                                    // 00611060 ~CExoString
    const Word refresh = loading | force;                                                // 00611194..98 / 00611261..6B
    // ---- right hand
    if (rd32(creature, 0x268) != right_id || force != 0) {                               // 00611071..83
        if (right) {                                                                     // 00611089..8D
            void* const item_object = at(right, 0x10);                                   // 00611096
            void* const model = object_slots(item_object)->get_model(item_object, nullptr, 0xFF, 1);   // 0061108B..A0
            kotor_m15a_item_set_equipped_by_00646600(right, nullptr, creature);          // 006110A6..AB
            st<Word>(right, 0x164, rd32(creature, 0x04));                                // 006110B2..B5
            if (model) {                                                                 // 006110B0..BB
                if (creature == client_get_player_creature_005ed540(client_app(), nullptr))   // 006110C1..CE
                    model_slots(model)->set_render_flags(model, nullptr, 2);             // 006110D4..D8
                else
                    model_slots(model)->set_render_flags(model, nullptr, 4);             // 006110E0..E4
                if (object_slots(creature)->has_visual_effect_applied(creature, nullptr, 0x1F40) != 0) {   // 006110EA..FC
                    void* const previous = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr,
                                                                                    rd32(creature, 0x268));  // 006110FE..10D
                    if (previous) kotor_m15a_object_disable_distortion_0063d550(at(previous, 0x10), nullptr);  // 00611112..19
                    kotor_m15a_object_enable_distortion_0063d540(item_object, nullptr);  // 0061111E..20
                }
                {
                    StartupString hook("rhand");                                         // 00611125..2E (0074F334)
                    void* const anim = gob_get_property_0x68_0063c070(creature, nullptr);   // 00611133..3D
                    anim_slots(anim)->attach_item(anim, nullptr, rd32(right, 0x14), &hook, 1, scale);   // 00611142..56 +7Ch
                }                                                                        // 00611159..65
                if (kotor_m15a_object_animation_looping_0063c300(creature, nullptr,
                        kotor_m15a_object_get_current_animation_0063c060(creature, nullptr)) != 0) {   // 0061116A..7B
                    const ObjectSlots* const slots = object_slots(creature);             // 0061117D
                    slots->set_animation(creature, nullptr,
                        kotor_m15a_object_get_current_animation_0063c060(creature, nullptr), 0, 0);  // 00611180..8E
                }
                const Word instant =
                    kotor_m15a_creature_get_last_instant_0060ced0(creature, nullptr, 0x10) == 0 ? 1u : 0u;   // 0061119D..AA
                kotor_m15a_item_power_item_00647130(right, nullptr, animate, 0, instant, refresh);   // 00611194..B5
            }
        } else {
            void* const model = object_slots(creature)->get_model(creature, nullptr, 0xF7, 1);   // 0061108B / 0061123E..48
            if (model) {                                                                 // 0061124E
                dropped = model_slots(model)->client_object(model, nullptr);             // 00611252..5B slot +70h
                if (dropped) {                                                           // 0061125F
                    st<Word>(dropped, 0x164, kInvalidObjectId);                          // 00611272
                    const Word instant =
                        kotor_m15a_creature_get_last_instant_0060ced0(creature, nullptr, 0x10) == 0 ? 1u : 0u;   // 0061126E..85
                    kotor_m15a_item_power_item_00647130(dropped, nullptr, 0, 0, instant, refresh);   // 00611286..8D
                }
            }
            StartupString hook("");                                                      // 00611292..9B (0073D71C)
            void* const anim = gob_get_property_0x68_0063c070(creature, nullptr);       // 006112A0..AA
            anim_slots(anim)->attach_item(anim, nullptr, kInvalidObjectId, &hook, 1, 1.0f);   // 006112AF..C4
        }                                                                                // 006112C7..D3
        st<Word>(creature, 0x268, right_id);                                             // 006111BE..C2
    }
    // ---- left hand
    if (rd32(creature, 0x26C) == left_id && force == 0) return;                          // 006111C8..DA
    if (left) {                                                                          // 006111E0..E4
        void* const item_object = at(left, 0x10);                                        // 006111ED
        void* const model = object_slots(item_object)->get_model(item_object, nullptr, 0xFF, 1);   // 006111E2..F7
        kotor_m15a_item_set_equipped_by_00646600(left, nullptr, creature);               // 006111FD..202
        st<Word>(left, 0x164, rd32(creature, 0x04));                                     // 00611209..20C
        if (model) {                                                                     // 00611207..212
            if (creature == client_get_player_creature_005ed540(client_app(), nullptr))   // 00611218..29
                model_slots(model)->set_render_flags(model, nullptr, 2);                 // 0061122F..33
            else
                model_slots(model)->set_render_flags(model, nullptr, 4);                 // 006112DD..E1
            if (object_slots(creature)->has_visual_effect_applied(creature, nullptr, 0x1F40) != 0) {   // 006112E7..F9
                void* const previous = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr,
                                                                                rd32(creature, 0x26C));  // 006112FB..30A
                if (previous) kotor_m15a_object_disable_distortion_0063d550(at(previous, 0x10), nullptr);  // 0061130F..16
                kotor_m15a_object_enable_distortion_0063d540(item_object, nullptr);      // 0061131B..1D
            }
            {
                StartupString hook("lhand");                                             // 00611322..2B (0074F4B8)
                void* const anim = gob_get_property_0x68_0063c070(creature, nullptr);   // 00611330..3A
                anim_slots(anim)->attach_item(anim, nullptr, rd32(left, 0x14), &hook, 0, scale);   // 0061133F..53
            }                                                                            // 00611356..62
            if (kotor_m15a_object_animation_looping_0063c300(creature, nullptr,
                    kotor_m15a_object_get_current_animation_0063c060(creature, nullptr)) != 0) {   // 00611367..78
                const ObjectSlots* const slots = object_slots(creature);                 // 0061137A
                slots->set_animation(creature, nullptr,
                    kotor_m15a_object_get_current_animation_0063c060(creature, nullptr), 0, 0);  // 0061137D..8B
            }
            const Word instant =
                kotor_m15a_creature_get_last_instant_0060ced0(creature, nullptr, 0x20) == 0 ? 1u : 0u;   // 00611391..A0
            kotor_m15a_item_power_item_00647130(left, nullptr, animate, 0, instant, 0);  // 006113A1..AB (push 0)
        }
    } else {
        void* const model = object_slots(creature)->get_model(creature, nullptr, 0xFB, 1);   // 006111E2 / 006113B5..BF
        if (model) {                                                                     // 006113C5..C7
            dropped = model_slots(model)->client_object(model, nullptr);                 // 006113C9..D2 slot +70h
            if (dropped) st<Word>(dropped, 0x164, kInvalidObjectId);                     // 006113D6..D8
        }
        // 006113E2: [esp+48h] -- still the right-hand result when this model lookup failed (0 initially, 00610FC4).
        if (dropped) {                                                                   // 006113E2..E8
            const Word instant =
                kotor_m15a_creature_get_last_instant_0060ced0(creature, nullptr, 0x10) == 0 ? 1u : 0u;   // 006113EA..F9 (slot 10h)
            kotor_m15a_item_power_item_00647130(dropped, nullptr, 0, 0, instant, 0);     // 006113FA..401
        }
        StartupString hook("");                                                          // 00611406..0F (0073D71C)
        void* const anim = gob_get_property_0x68_0063c070(creature, nullptr);           // 00611414..1E
        anim_slots(anim)->attach_item(anim, nullptr, kInvalidObjectId, &hook, 0, 1.0f);  // 00611423..38
    }                                                                                    // 0061143B..47
    st<Word>(creature, 0x26C, left_id);                                                  // 0061144C..50
}
}
