// lane-m15-a (L15-A) unit m15a-appearance: CSWCCreatureAppearance (body / head / BType / simple model builders).
// Original: swkotor.exe GOG K1 (image base 0x400000), static disassembly only (nothing executed); every behaviour
// cites its instruction addresses. Conventions: m15a_common.h / m15a_appearance.h (D1 fastcall veneers with the
// original stack words and callee cleanup; D21/D24 typed slot views at the call sites; object access through
// rp/rd32/ld/st, one load/store per original access, D132).
//
// Unwind (D30-FINAL): the original EH states only release the local CExoString / CResRef temporaries; the C++
// locals below (StartupString lifetimes, POD CResRefs) reproduce that order on the normal path. No new throwing
// callee is introduced.
//
// Observed quirks kept on purpose:
//  * CreateBodyAppearance passes the anim-type BYTE to CreateSimple / CreateBTypeBody as the low byte of its own
//    `data` argument slot (00698CAF mov [esp+34h],bl; 00698CC1/00698CD0 push dword [esp+34h]): the upper 24 bits
//    are those of the data pointer. Both builders forward that whole dword to CSWCCreature slot +78h LoadModel
//    (00697F0D / 0069874E push [ebp+8]); reproduced exactly.
//  * CreateSimple overwrites the low byte of its anim-type argument with the MODELTYPE anim type (00698893) and
//    tests that byte (006988FA).
#include "bbcb007afea348df_m15a_appearance.h"
#include "8d831d5c6afcac70_m15a_creature.h"
#include "468a2f31a138f45b_m15a_object.h"
#include "f155d76cd432eed7_m15_l0.h"          // lane-m15-0: GetItemByGameObjectID, IsPlayerCreature, GetMovementRateFactor
#include "bf662019daaaa2b6_m15b_stats.h"      // lane-m15-b: C2DA::GetFLOATEntry 00413350
#include "c53472623c7d6765_resources.h"       // menu_resources::Ref (CResRef 16 bytes), Manager::exists_00408bc0, unresolved_ref_assign_00406290
#include "e1805047c9b7563b_startup_string.h"  // kotor_reconstruction::StartupString (CExoString 8 bytes; 005B3190/005E5A90/005E5C20)
#include <cstdint>
#include <cstring>

using kotor_reconstruction::StartupString;
using menu_resources::Ref;
using namespace kotor_m15a;

// ---------------------------------------------------------------------------------------------------- externals
extern "C" {
// CResRef (M60 map, extern "C" D1 symbols)
Ref* __fastcall c2da_default_ref_00405ed0(Ref*, void*);                                      // 00405ED0 CResRef()
char* __fastcall c2da_ref_00405fe0(Ref*, void*);                                             // 00405FE0 GetResRefStr
int __fastcall c2da_not_equal_text_00406060(Ref*, void*, const char*);                       // 00406060 operator!=(char*)
Ref* __fastcall c2da_copy_00406120(Ref*, void*, Ref* out, Ref* source);                      // 00406120 operator=(CResRef) by value
Ref* __fastcall async_ref_assign_string_004061f0(Ref*, void*, Ref* out, StartupString* src); // 004061F0 operator=(CExoString) by value
Ref* __fastcall async_ref_construct_string_00406d60(Ref*, void*, StartupString*);            // 00406D60 CResRef(CExoString)
// C2DA (lane-tlk tlk-c2da-rowcol / startup-misc-c2da)
std::int32_t __fastcall c2da_current_get_string_entry_00413190(void* table, void*, std::int32_t row, std::int32_t column,
                                                               StartupString* out);           // 00413190 RET 0Ch
std::int32_t __fastcall c2da_current_get_string_00413270(void* table, void*, std::int32_t row, StartupString* column,
                                                         StartupString* out);                 // 00413270 RET 0Ch
std::int32_t __fastcall c2da_current_get_int_entry_00413510(void* table, void*, std::int32_t row, std::int32_t column,
                                                            std::int32_t* out);               // 00413510 RET 0Ch
// CExoString (M60 map)
std::uint32_t __fastcall async_string_length_005e5790(StartupString*, void*);               // 005E5790 GetLength
unsigned __fastcall frame_LogIsEmpty_005e58a0(void*, void*);                                  // 005E58A0 IsEmpty
StartupString* __fastcall c2da_assign_text_005e5140(StartupString*, void*, const char*);     // 005E5140 operator=(char*)
StartupString* __fastcall async_assign_string_005e5c50(StartupString*, void*, StartupString*); // 005E5C50 operator=
StartupString* __fastcall resman_string_from_int_005e5bc0(StartupString*, void*, std::int32_t); // 005E5BC0 CExoString(int)
std::int32_t __fastcall find_005e5f20(StartupString*, void*, std::uint32_t ch, std::int32_t start); // 005E5F20 Find(char,int)
char* __fastcall async_string_cstr_005e5670(StartupString*, void*);                          // 005E5670 CStr
int __cdecl format_006fadb0(char*, const char*, ...);                                         // 006FADB0 sprintf
// CClientExoApp (M60 map)
void* __fastcall client_get_player_creature_005ed540(void* app, void*);                      // 005ED540
std::uint32_t __fastcall kotor_fastcall_005edc10(void* app, void*);                          // 005EDC10 GetPausedByCombat
// Aurora gob / client object leaves (M60 map)
void* __fastcall gob_get_property_0x68_0063c070(void* object, void*);                        // 0063C070 [ecx+68h]
void __fastcall gui_load_slot_ret_004015a0(void*, void*);                                    // 004015A0 void_return (RET)
// lane-prepare / rules cells (rules_image_data.h; written by CTwoDimArrays::Load2DArrays_* column caches)
extern void* kotor_cell_rules_007a3a28;                                                       // [007A3A28] CSWRules*
extern std::uint32_t kotor_rules_cell32_007a20c4;
extern std::uint32_t kotor_rules_cell32_007a20d8;
extern std::uint32_t kotor_rules_cell32_007a20dc;
extern std::uint32_t kotor_rules_cell32_007a20e0;
extern std::uint32_t kotor_rules_cell32_007a20e4;
extern std::uint32_t kotor_rules_cell32_007a20e8;
extern std::uint32_t kotor_rules_cell32_007a20f0;
extern std::uint32_t kotor_rules_cell32_007a2104;
extern std::uint32_t kotor_rules_cell32_007a2110;
extern std::uint32_t kotor_rules_cell32_007a2114;
extern std::uint32_t kotor_rules_cell32_007a2118;
extern std::uint32_t kotor_rules_cell32_007a211c;
extern std::uint32_t kotor_rules_cell32_007a2148;
}
StartupString* __fastcall GuiConcat005e5d10(StartupString*, void*, StartupString*, const StartupString*); // 005E5D10 operator+
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }    // 005E5680 Format
namespace options_video { struct String; }
namespace client_movies { int __fastcall StringFind_005e5eb0(options_video::String*, void*, options_video::String*, int); } // 005E5EB0
namespace menu_tables_required { void __fastcall AnimateExternal_0043e150(void*, void*, float); }  // 0043E150 CAurObject
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }                      // [007A39FC] AppManager
namespace kotor_resource_demand { extern menu_resources::Manager* volatile resource_manager_007a39e8; } // [007A39E8]
namespace menu_tables_m41 { namespace cells { extern const unsigned int invalid_object_00746644; } } // .rdata 7F000000h

namespace {
// ---------------------------------------------------------------------------------------- typed slot views (D24)
// CSWCObject-family method tables (CSWCCreature 0074F5F8, CSWCItem's CSWCObject subobject at item+10h): only the
// slots called here (SLOT-AUDIT.json names).
struct ObjectSlots {
    const void* s00_74[0x1E];
    Word (__fastcall* load_model)(void*, void*, Ref* ref, Word part, Word anim_type);   // +78 0061B380 LoadModel RET 0Ch
    void (__fastcall* unload_model)(void*, void*, Word part);                            // +7C 00646650 UnloadModel RET 4
    const void* s80_90[5];
    void (__fastcall* attach_to_scene)(void*, void*, void* scene);                       // +94 0060FB40 AttachToScene RET 4
    void* (__fastcall* get_model)(void*, void*, Word part, Word flag);                   // +98 006179C0 GetModel RET 8
    const void* s9c_a8[4];
    void (__fastcall* set_env_map)(void*, void*, Word part, Word has, Word a3, Ref* ref); // +AC 0063CCC0 SetEnvMap RET 10h
    const void* sb0_114[0x1A];
    void (__fastcall* set_queued_anim_callback)(void*, void*);                           // +118 0060F150 RET 0
    Word (__fastcall* clear_queued_anim_callback)(void*, void*);                         // +11C 0060F1A0 RET 0
};
static_assert(offsetof(ObjectSlots, load_model) == 0x78 && offsetof(ObjectSlots, unload_model) == 0x7C &&
              offsetof(ObjectSlots, attach_to_scene) == 0x94 && offsetof(ObjectSlots, get_model) == 0x98 &&
              offsetof(ObjectSlots, set_env_map) == 0xAC && offsetof(ObjectSlots, set_queued_anim_callback) == 0x118 &&
              offsetof(ObjectSlots, clear_queued_anim_callback) == 0x11C, "object slots");
// Aurora gob method table (3d/render vtables_gen.h Gob table): slots called here.
struct GobSlots {
    const void* s00_14[6];
    Word (__fastcall* play_animation)(void*, void*, const char*, float, Word, Word);     // +18 00485BD0 PlayAnimation
    const void* s1c_3c[9];
    Word (__fastcall* set_can_down_sample)(void*, void*, Word);                         // +40 004437F0
    const void* s44_48[2];
    Word (__fastcall* attach_to_scene)(void*, void*, void*);                            // +4C 0044F7C0
    void* (__fastcall* attach_to_object)(void*, void*, void*, const char*, Word);      // +50 00443820
    const void* s54;
    Word (__fastcall* set_behavior)(void*, void*, void*);                               // +58 0043F1D0
    Vector* (__fastcall* set_position)(void*, void*, Vector* out, Vector position);     // +5C 0043F0C0
    const void* s60_c8[0x1B];
    Word (__fastcall* set_object_scale)(void*, void*, float, Word);                     // +CC 00444D90
    const void* sd0_d8[3];
    Word (__fastcall* enable_force_animations)(void*, void*);                           // +DC 0043E830
    Word (__fastcall* disable_force_animations)(void*, void*);                          // +E0 0043E840
};
static_assert(offsetof(GobSlots, play_animation) == 0x18 && offsetof(GobSlots, set_can_down_sample) == 0x40 &&
              offsetof(GobSlots, attach_to_scene) == 0x4C && offsetof(GobSlots, attach_to_object) == 0x50 &&
              offsetof(GobSlots, set_behavior) == 0x58 && offsetof(GobSlots, set_position) == 0x5C &&
              offsetof(GobSlots, set_object_scale) == 0xCC && offsetof(GobSlots, enable_force_animations) == 0xDC &&
              offsetof(GobSlots, disable_force_animations) == 0xE0, "gob slots");
// CSWCAnimBase-family table of [creature+68h] (SLOT-AUDIT anim_base_frontier): slots +90h / +ECh, one word each.
struct AnimBaseSlots {
    const void* s00_8c[0x24];
    void (__fastcall* slot90)(void*, void*, Word);                                       // +90 (00698D99 / 006988C8)
    const void* s94_e8[0x16];
    void (__fastcall* slotec)(void*, void*, Word);                                       // +EC (00696BB6)
};
static_assert(offsetof(AnimBaseSlots, slot90) == 0x90 && offsetof(AnimBaseSlots, slotec) == 0xEC, "anim base slots");

const ObjectSlots* oslots(void* object) { return static_cast<const ObjectSlots*>(rp(object, 0)); }
const GobSlots* gslots(void* gob) { return static_cast<const GobSlots*>(rp(gob, 0)); }
const AnimBaseSlots* aslots(void* anim) { return static_cast<const AnimBaseSlots*>(rp(anim, 0)); }

constexpr Word kPartBody = 0xFF;       // pushed 0FFh (body / whole model)
constexpr Word kPartHead = 0xFE;       // pushed 0FEh
constexpr Word kResTga = 3;            // CExoResMan::Exists type 3 (00698350)
constexpr Word kResTpc = 0xBBF;        // 0BBFh (0069837B)
const char kEmpty_0073d71c[] = "";     // .rdata 0073D71C ""
const char kDefault_0073e304[] = "DEFAULT";          // .rdata 0073E304
const char kAppearanceForceUpdate_0074f4d4[] = "AppearanceForceUpdate";  // .rdata 0074F4D4

void* creature_of(void* self) { return rp(self, 0x3C); }               // [this+3Ch] (00696494)
void* client_app() { return rp(kotor_menu_window_cells::manager_007a39fc, 4); }   // [[007A39FC]+4]
void* appearance_table() { return rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x0C); } // [[[007A3A28]+B8h]+0Ch]
std::int32_t row_of(const void* data) { return ld<U16>(data, 0x18); }  // movzx [data+18h]
Word invalid_object() { return menu_tables_m41::cells::invalid_object_00746644; }   // [00746644] 7F000000h

// Hidden-return temporaries of 004061F0 / 00406120 / 00406290 (16-byte CResRef; dead after the call).
struct RefTemp { Ref ref; };
// operator+ result storage (constructed by 005E5D10 in place, destroyed after the assignment).
struct ConcatTemp {
    alignas(4) unsigned char bytes[sizeof(StartupString)];
    StartupString* out() { return reinterpret_cast<StartupString*>(bytes); }
};
// s = lhs + rhs (005E5D10 into a temporary, 005E5C50 assignment, 005E5C20 on the temporary).
void concat_assign(StartupString* target, StartupString* lhs, const StartupString* rhs) {
    ConcatTemp temp;
    StartupString* const sum = GuiConcat005e5d10(lhs, nullptr, temp.out(), rhs);
    async_assign_string_005e5c50(target, nullptr, sum);
    sum->~StartupString();
}
void ref_from_string(Ref* target, StartupString* source) {     // CResRef::operator=(CExoString) 004061F0
    RefTemp discarded;
    async_ref_assign_string_004061f0(target, nullptr, &discarded.ref, source);
}
Word exists(Ref* ref, Word type) {                                // CExoResMan::Exists(ref, type, nullptr)
    return kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(ref, type, nullptr);
}
// CSWCObject slot +AC SetEnvMap(part, has_env, 1, ref): env ref when the ENVMAP column was non-empty, else a
// "DEFAULT" CResRef (006969C3..006969F1, 00696BF0..00696C24, 00697FEC..00698020, 006987AB..006987DC).
void set_env_map(void* creature, Word part, Word has_env, Ref* env) {
    if (has_env != 0) {
        oslots(creature)->set_env_map(creature, nullptr, part, 1, 1, env);
    } else {
        Ref fallback;
        fallback.construct_00406d80(kDefault_0073e304);
        oslots(creature)->set_env_map(creature, nullptr, part, 0, 1, &fallback);
    }
}
}

extern "C" {
// ================================================================== 00696430 CSWCCreatureAppearance(creature) RET 4
void* __fastcall kotor_m15a_appearance_construct_00696430(void* self, void*, void* creature) {
    st<Word>(self, 0x10, invalid_object());       // 00696432..38 [eax+10h] = [00746644]
    st<Word>(self, 0x02, 0);                      // 0069643D..40 (unaligned +2/+6/+0Ah dwords)
    st<Word>(self, 0x06, 0);                      // 00696442
    st<Word>(self, 0x0A, 0);                      // 00696445
    st<U8>(self, 0x1B, 0xFF);                     // 0069644C
    st<U8>(self, 0x1C, 0xFF);                     // 0069644F
    st<U8>(self, 0x1A, 0xFF);                     // 00696452
    st<U8>(self, 0x34, 0xFF);                     // 00696455
    st<U8>(self, 0x00, 0);                        // 0069645C
    st<U8>(self, 0x01, 1);                        // 0069645E
    st<U16>(self, 0x18, 0xFFFF);                  // 00696462
    st<Word>(self, 0x14, 0);                      // 00696468
    st<U8>(self, 0x1D, 0);                        // 0069646B
    st<U8>(self, 0x1E, 0);                        // 0069646E
    st<U8>(self, 0x1F, 0);                        // 00696471
    st<U8>(self, 0x20, 0);                        // 00696474
    st<U8>(self, 0x21, 0);                        // 00696477
    st<U8>(self, 0x22, 0);                        // 0069647A
    st<float>(self, 0x24, 0.6f);                  // 0069647D 3F19999Ah
    st<float>(self, 0x28, 0.5f);                  // 00696484 3F000000h
    st<Word>(self, 0x30, 0);                      // 0069648B
    st<U16>(self, 0x2C, 0x32);                    // 0069648E
    st<void*>(self, 0x3C, creature);              // 00696494
    st<Word>(self, 0x40, 0);                      // 00696497
    st<Word>(self, 0x44, 0);                      // 0069649A
    st<Word>(self, 0x48, 0);                      // 0069649D
    st<Word>(self, 0x4C, 0);                      // 006964A0
    st<Word>(self, 0x50, 0);                      // 006964A3
    st<Word>(self, 0x54, 0);                      // 006964A6
    st<Word>(self, 0x68, 0);                      // 006964A9
    st<Word>(self, 0x38, 0);                      // 006964AC
    return self;                                  // 00696430 mov eax,ecx; 006964AF ret 4
}

// ================================================================== 00696D40 GetAnimType(CExoString*) RET 4 -> AL
std::uint8_t __fastcall kotor_m15a_appearance_get_anim_type_00696d40(void* /*self*/, void*, void* model_type_arg) noexcept(false) {
    StartupString* const model_type = static_cast<StartupString*>(model_type_arg);   // 00696D59
    if (find_005e5f20(model_type, nullptr, 0x53, 0) != -1) return 0x0B;            // 00696D5D..6D 'S' -> 0Bh
    bool found_b;
    {
        StartupString b("B");                                                      // 00696D82..8B (00747C28)
        found_b = client_movies::StringFind_005e5eb0(reinterpret_cast<options_video::String*>(model_type), nullptr,
                      reinterpret_cast<options_video::String*>(&b), 0) != -1;    // 00696D90..AD setne bl
    }                                                                              // 00696DB8 ~CExoString
    if (found_b) return 3;                                                         // 00696DBD..C2
    if (find_005e5f20(model_type, nullptr, 0x46, 0) != -1) return 2;              // 00696DD6..E4 'F'
    if (find_005e5f20(model_type, nullptr, 0x4C, 0) != -1) return 2;              // 00696DE6..F8 'L'
    return 0x0B;                                                                   // 00696DF4
}

// ================================================================== 00697610 GetBTypeModelAndTextureResRef RET 0Ch
void __fastcall kotor_m15a_appearance_get_btype_model_and_texture_00697610(void* /*self*/, void*, void* data,
                                                                            void* model_out, void* texture_out) noexcept(false) {
    char model_column[7];                                    // 0069762E..66 "ModelA" (00754A38 dword, 00754A3C word, 00754A3E byte)
    char texture_column[5];                                  // 00697645..66 "TEXA" (00754A30 dword, 00754A34 byte)
    std::memcpy(model_column, "ModelA", sizeof model_column);
    std::memcpy(texture_column, "TEXA", sizeof texture_column);
    const U8 body = ld<U8>(data, 0x00);                      // 00697652
    Word letter;                                             // ecx: clamp(body, 1, 0Ah) (00697654..86)
    if (body > 0x0A) letter = 0x0A;
    else if (body < 1) letter = 1;
    else letter = body;
    const U8 texture_variation = ld<U8>(data, 0x01);         // 00697689
    std::int32_t variation = texture_variation < 1 ? 1 : texture_variation;   // 0069768C..9D
    if (rd32(data, 0x14) != 0) {                             // 006976A1..A6
        const char c = static_cast<char>(letter + 0x40);     // 006976A8 add cl,40h
        model_column[5] = c;                                 // 006976AB [esp+1Dh]
        texture_column[3] = c;                               // 006976AF [esp+13h]
    }
    StartupString value;                                     // 006976B3 005B3190
    if (model_out != nullptr) {                              // 006976BC..C9
        Word missing;
        {
            StartupString column(model_column);              // 006976CB..D4
            missing = c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &value) == 0
                          ? 1u : 0u;                         // 006976D9..0D neg/sbb/inc bl
        }                                                    // 00697714
        if (missing != 0) c2da_assign_text_005e5140(&value, nullptr, kEmpty_0073d71c);   // 00697719..26
        ref_from_string(static_cast<Ref*>(model_out), &value);                          // 0069772B..37
    }
    if (texture_out != nullptr) {                            // 0069773C..41
        std::int32_t found;
        {
            StartupString column(texture_column);            // 00697747..50
            found = c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &value); // 00697755..79
        }                                                    // 00697789
        if (found != 0) {                                    // 0069778E..90
            Ref texture;                                     // 00697796 00405ED0
            c2da_default_ref_00405ed0(&texture, nullptr);
            StartupString name;                              // 0069779F 005B3190
            kotor_reconstruction::StartupFormat005e5680(&name, "%s%02d", async_string_cstr_005e5670(&value, nullptr),
                                                        variation);              // 006977A8..CB (00754A28)
            ref_from_string(&texture, &name);                                    // 006977CE..DC
            if (exists(&texture, kResTga) == 0 && exists(&texture, kResTpc) == 0) {   // 006977E1..11
                kotor_reconstruction::StartupFormat005e5680(&name, "%s%02d", async_string_cstr_005e5670(&value, nullptr),
                                                            1);                  // 00697813..2E
                ref_from_string(&texture, &name);                                // 00697831..3F
            }
            RefTemp discarded;
            c2da_copy_00406120(static_cast<Ref*>(texture_out), nullptr, &discarded.ref, &texture);   // 00697844..50
        } else {                                                                 // 00697865..71
            RefTemp discarded;
            unresolved_ref_assign_00406290(static_cast<Ref*>(texture_out), nullptr, &discarded.ref,
                                           kEmpty_0073d71c);
        }
    }
}                                                                                // 00697876 ~CExoString

// ================================================================== 006978A0 ForceAnimate() RET
// Advances the force-animation of the head model, the item in [this+10h] and the 10h/20h equipped items.
void __fastcall kotor_m15a_appearance_force_animate_006978a0(void* self, void*) {
    if (rd32(self, 0x68) == 0) return;                                           // 006978A3..A8
    auto animate = [](void* gob) {                                               // DC / AnimateExternal(1.0) / E0
        gslots(gob)->enable_force_animations(gob, nullptr);                      // +DC
        menu_tables_required::AnimateExternal_0043e150(gob, nullptr, 1.0f);      // 0043E150 push 3F800000h
        gslots(gob)->disable_force_animations(gob, nullptr);                     // +E0
    };
    void* creature = creature_of(self);                                          // 006978AE
    void* gob = oslots(creature)->get_model(creature, nullptr, kPartBody, 1);    // 006978B4..BB
    if (gob != nullptr) animate(gob);                                            // 006978C3..E1
    creature = creature_of(self);                                                // 006978E7
    gob = oslots(creature)->get_model(creature, nullptr, kPartHead, 1);          // 006978EC..F3
    if (gob != nullptr) {                                                        // 006978FB..FD
        animate(gob);                                                            // 006978FF..19
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, rd32(self, 0x10)); // 0069791F..2B
        if (item != nullptr) {                                                   // 00697930
            void* const object = at(item, 0x10);                                 // 00697934 lea ecx,[eax+10h]
            gob = oslots(object)->get_model(object, nullptr, kPartBody, 1);      // 00697937..40
            if (gob != nullptr) animate(gob);                                    // 00697946..66
        }
    }
    {
        void* const app = client_app();                                          // 0069796C..72
        const Word id = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature_of(self), nullptr, 0x10); // 00697975..7A
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(app, nullptr, id);   // 0069797F..82
        if (item != nullptr) {                                                   // 00697987
            void* const object = at(item, 0x10);                                 // 0069798B..8E
            gob = oslots(object)->get_model(object, nullptr, kPartBody, 1);      // 00697991..98
            if (gob != nullptr) animate(gob);                                    // 0069799E..BE
        }
    }
    {
        void* const app = client_app();                                          // 006979C4..CC
        const Word id = kotor_m15a_creature_get_equipped_item_id_0060ca80(creature_of(self), nullptr, 0x20); // 006979C9..D1
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(app, nullptr, id);   // 006979D6..D9
        if (item != nullptr) {                                                   // 006979DE
            void* const object = at(item, 0x10);                                 // 006979E2..E5
            gob = oslots(object)->get_model(object, nullptr, kPartBody, 1);      // 006979E8..EF
            if (gob != nullptr) animate(gob);                                    // 006979F5..17 (tail jmp [edx+E0h])
        }
    }
}

// ================================================================== 006964C0 CreateHead(anim_type, data) RET 8
std::uint32_t __fastcall kotor_m15a_appearance_create_head_006964c0(void* self, void*, std::uint32_t anim_type,
                                                                    void* data) noexcept(false) {
    Ref head_model;      // [esp+48h] 006964E3 00405ED0
    Ref head_texture;    // [esp+68h] 006964EC
    Ref env_map;         // [esp+58h] 006964F5
    c2da_default_ref_00405ed0(&head_model, nullptr);
    c2da_default_ref_00405ed0(&head_texture, nullptr);
    c2da_default_ref_00405ed0(&env_map, nullptr);
    StartupString model_name;     // [esp+30h] 006964FE
    StartupString texture_name;   // [esp+40h] 00696507
    StartupString column_value;   // [esp+38h] 0069651B
    if (ld<U8>(data, 0x1A) == 0 && rd32(data, 0x10) == invalid_object()) {      // 0069652F..48
        kotor_m15a_creature_unload_monk_visuals_0060bad0(creature_of(self), nullptr);   // 0069654A..4D
        void* const creature = creature_of(self);                                // 00696552
        oslots(creature)->unload_model(creature, nullptr, kPartHead);            // 00696555..5C
        gui_load_slot_ret_004015a0(creature_of(self), nullptr);                  // 0069655F..62
        st<Word>(self, 0x10, rd32(data, 0x10));                                  // 00696567..6A
        st<U8>(self, 0x1A, ld<U8>(data, 0x1A));                                  // 0069656D..70
        return 1;                                                                // 00696573 -> 00696C48..84
    }
    c2da_current_get_string_entry_00413190(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a20e8), &column_value);  // 00696578..9A (ENVMAP column)
    Word has_env;                                                                // [esp+18h]
    if (static_cast<std::int32_t>(async_string_length_005e5790(&column_value, nullptr)) > 0) {   // 0069659F..AA
        ref_from_string(&env_map, &column_value);                                // 006965AC..BA
        has_env = 1;                                                             // 006965BF
    } else {
        has_env = 0;                                                             // 006965C9
    }
    if (static_cast<U8>(anim_type) == 3) {                                       // 006965D1..D5
        std::int32_t row = 0;   // [esp+10h]; always written by 00413510 (every path stores *out) or 006965E5
        const U8 forced = ld<U8>(data, 0x34);                                    // 006965DB
        if (forced != 0xFF) {                                                    // 006965DE..E0
            row = forced;                                                        // 006965E2..E5
            st<Word>(self, 0x40, 1);                                             // 006965E9
        } else {
            const std::uint32_t column = rd32(data, 0x30) != 0 ? kotor_rules_cell32_007a20c4
                                                               : kotor_rules_cell32_007a2114;  // 006965F2..17
            c2da_current_get_int_entry_00413510(appearance_table(), nullptr, row_of(data),
                                                static_cast<std::int32_t>(column), &row);      // 0069661D..21
        }
        {
            StartupString head_column("HEAD");                                   // 00696626..2F (00754A14)
            c2da_current_get_string_00413270(rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x44), nullptr, row,
                                             &head_column, &model_name);         // 00696634..5C heads table [+B8h]+44h
        }                                                                        // 00696665..6C
        if (frame_LogIsEmpty_005e58a0(&model_name, nullptr) != 0) return 0;      // 00696671..7C -> 00696B04
        ref_from_string(&head_model, &model_name);                               // 00696682..90
        ref_from_string(&head_texture, &model_name);                             // 00696695..9F -> 00696979..7D
    } else {
        c2da_assign_text_005e5140(&model_name, nullptr, "P");                    // 006966A4..AD (00746BB4)
        c2da_current_get_string_entry_00413190(rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x10), nullptr,
            ld<U8>(data, 0x1B) == 1 ? 1 : 0, static_cast<std::int32_t>(kotor_rules_cell32_007a2148),
            &column_value);                                                      // 006966B2..D9 gender table [+B8h]+10h
        concat_assign(&model_name, &model_name, &column_value);                  // 006966DE..0E
        c2da_current_get_string_entry_00413190(appearance_table(), nullptr, row_of(data),
            static_cast<std::int32_t>(kotor_rules_cell32_007a211c), &column_value);   // 00696713..35
        concat_assign(&model_name, &model_name, &column_value);                  // 0069673A..6A
        async_assign_string_005e5c50(&texture_name, nullptr, &model_name);       // 0069676F..78
        {
            ConcatTemp number;                                                   // [esp+10h] 0069677D..86 CExoString(int)
            StartupString* const index = resman_string_from_int_005e5bc0(number.out(), nullptr, ld<U8>(data, 0x1C));
            concat_assign(&model_name, &model_name, index);                      // 0069678B..BB
            index->~StartupString();                                             // 006967C5..D0
        }
        {
            StartupString suffix("_HEAD");                                       // 006967D5..DE (00754A0C)
            concat_assign(&model_name, &model_name, &suffix);                    // 006967E3..1C
        }                                                                        // 00696821..2C
        {
            StartupString suffix("0_HEAD");                                      // 00696831..3A (00754A04)
            concat_assign(&texture_name, &texture_name, &suffix);                // 0069683F..78
        }                                                                        // 0069687D..88
        char number_text[4];                                                     // [esp+1Ch] (4 bytes up to [esp+20h])
        format_006fadb0(number_text, "%03u", static_cast<unsigned>(ld<U8>(data, 0x1A)));   // 0069688D..A1 (007549FC)
        {
            StartupString text(number_text);                                     // 006968A4..AD
            concat_assign(&model_name, &model_name, &text);                      // 006968B2..EB
        }                                                                        // 006968F0..FB
        {
            StartupString text(number_text);                                     // 00696900..09
            concat_assign(&texture_name, &texture_name, &text);                  // 0069690E..47
        }                                                                        // 0069694C..57
        ref_from_string(&head_model, &model_name);                               // 0069695C..6A
        ref_from_string(&head_texture, &texture_name);                           // 0069696F..7D
    }
    // 00696982..F1: nothing changed -> only refresh the environment map.
    if (rd32(self, 0x40) == 0 && ld<U16>(self, 0x18) == ld<U16>(data, 0x18) &&
        ld<U8>(self, 0x1B) == ld<U8>(data, 0x1B) && ld<U8>(self, 0x1A) == ld<U8>(data, 0x1A) &&
        rd32(self, 0x10) == rd32(data, 0x10) && ld<U8>(self, 0x34) == ld<U8>(data, 0x34) &&
        rd32(self, 0x30) == rd32(data, 0x30)) {
        set_env_map(creature_of(self), kPartHead, has_env, &env_map);           // 006969BB..F1
        return 1;                                                                // 006969F7 -> 00696C48
    }
    kotor_m15a_creature_unload_monk_visuals_0060bad0(creature_of(self), nullptr);   // 006969FC..FF
    {
        void* const creature = creature_of(self);                                // 00696A04
        oslots(creature)->unload_model(creature, nullptr, kPartHead);            // 00696A07..0E
    }
    gui_load_slot_ret_004015a0(creature_of(self), nullptr);                      // 00696A11..14
    st<U8>(self, 0x1A, ld<U8>(data, 0x1A));                                      // 00696A19..1F
    st<U8>(self, 0x34, ld<U8>(data, 0x34));                                      // 00696A22..25
    st<Word>(self, 0x30, rd32(data, 0x30));                                      // 00696A28..2F
    {
        void* const creature = creature_of(self);                                // 00696A2B
        if (oslots(creature)->load_model(creature, nullptr, &head_model, kPartHead, anim_type) == 0) return 0;  // 00696A2E..43
    }
    const U8 type = static_cast<U8>(anim_type);
    if (type == 1 || type == 3) {                                                // 00696A49..51
        if (rd32(self, 0x40) != 0 || rd32(self, 0x10) != rd32(data, 0x10)) {     // 00696A57..64
            void* const old_item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, rd32(self, 0x10)); // 00696A6A..77
            if (old_item != nullptr) {                                           // 00696A7C..82
                void* const object = at(old_item, 0x10);                         // 00696A84
                void* const gob = oslots(object)->get_model(object, nullptr, kPartBody, 1);   // 00696A87..94
                if (gob != nullptr) {                                            // 00696A9A..9E
                    gslots(gob)->attach_to_object(gob, nullptr, nullptr, nullptr, 0);   // 00696AA0..AA detach
                    gslots(gob)->set_object_scale(gob, nullptr, 1.0f, 1);         // 00696AAD..B8
                    gslots(gob)->set_can_down_sample(gob, nullptr, 1);            // 00696ABE..C4
                }
                if (rd32(old_item, 0x30) != 0)                                   // 00696AC7..D0
                    oslots(object)->attach_to_scene(object, nullptr, nullptr);   // 00696AD2..DA
            }
            const Word new_id = rd32(data, 0x10);                                // 00696AE0
            if (new_id != invalid_object()) {                                    // 00696AE3..E9
                void* const new_item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, new_id); // 00696AEF..F9
                if (new_item == nullptr) return 0;                               // 00696AFE..02 -> 00696B04
                void* const creature = creature_of(self);                        // 00696B50
                void* const head = oslots(creature)->get_model(creature, nullptr, kPartHead, 1);   // 00696B53..5C
                void* const object = at(new_item, 0x10);                         // 00696B62
                void* const gob = oslots(object)->get_model(object, nullptr, kPartBody, 1);   // 00696B65..72
                if (gob != nullptr) {                                            // 00696B78..7C
                    gslots(gob)->attach_to_object(gob, nullptr, head, "GoggleHook", 0);   // 00696B7E..8E (007549F0)
                    gslots(gob)->play_animation(gob, nullptr, "default", 1.0f, 0, 0);    // 00696B91..A3 (0073DF6C)
                }
            }
        }
        void* const anim = gob_get_property_0x68_0063c070(creature_of(self), nullptr);   // 00696BA6..A9
        aslots(anim)->slotec(anim, nullptr, rd32(data, 0x10));                   // 00696BAE..B6
    }
    st<Word>(self, 0x10, rd32(data, 0x10));                                      // 00696BBC..C2
    {
        void* const creature = creature_of(self);                                // 00696BBF
        if (client_get_player_creature_005ed540(client_app(), nullptr) == creature) {   // 00696BC5..D4
            void* const head = oslots(creature)->get_model(creature, nullptr, kPartHead, 1);   // 00696BD6..E1
            gslots(head)->set_can_down_sample(head, nullptr, 0);                 // 00696BE7..ED
        }
    }
    set_env_map(creature_of(self), kPartHead, has_env, &env_map);               // 00696BF0..24
    st<Word>(self, 0x50, 1);                                                     // 00696C2A
    st<Word>(self, 0x40, 0);                                                     // 00696C31
    st<Word>(self, 0x68, kotor_fastcall_005edc10(client_app(), nullptr));        // 00696C38..45
    return 1;                                                                    // 00696C84
}

// ================================================================== 00697A20 CreateHeadAppearance(data) RET 4
std::uint32_t __fastcall kotor_m15a_appearance_create_head_appearance_00697a20(void* self, void*, void* data) noexcept(false) {
    StartupString model_type;                                                    // 00697A3C..40
    c2da_current_get_string_entry_00413190(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a2110), &model_type);   // 00697A45..72 (MODELTYPE)
    if (kotor_m15a_appearance_get_anim_type_00696d40(self, nullptr, &model_type) != 3) {   // 00697A77..85
        st<U8>(self, 0x1A, ld<U8>(data, 0x1A));                                  // 00697A87..8D
        void* const creature = creature_of(self);                                // 00697A8A
        oslots(creature)->unload_model(creature, nullptr, kPartHead);            // 00697A90..97
        return 1;                                                                // 00697AAC
    }
    return kotor_m15a_appearance_create_head_006964c0(self, nullptr, 3, data);   // 00697AC3..C8
}

// ================================================================== 00697B00 CreateTailAppearance(data) RET 4
std::uint32_t __fastcall kotor_m15a_appearance_create_tail_appearance_00697b00(void* self, void*, void* data) noexcept(false) {
    StartupString model_type;                                                    // 00697B1C..20
    c2da_current_get_string_entry_00413190(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a2110), &model_type);   // 00697B25..52
    kotor_m15a_appearance_get_anim_type_00696d40(self, nullptr, &model_type);    // 00697B57..5E (result unused)
    if (find_005e5f20(&model_type, nullptr, 0x54, 0) == -1) {                    // 00697B63..73 'T'
        st<U8>(self, 0x21, ld<U8>(data, 0x21));                                  // 00697B75..7B
        void* const creature = creature_of(self);                                // 00697B78
        oslots(creature)->unload_model(creature, nullptr, 0xFD);                 // 00697B7E..85
    }
    return 1;                                                                    // 00697B9E
}

// ================================================================== 00697BC0 CreateWingAppearance(data) RET 4
std::uint32_t __fastcall kotor_m15a_appearance_create_wing_appearance_00697bc0(void* self, void*, void* data) noexcept(false) {
    StartupString model_type;                                                    // 00697BDC..E0
    c2da_current_get_string_entry_00413190(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a2110), &model_type);   // 00697BE5..12
    kotor_m15a_appearance_get_anim_type_00696d40(self, nullptr, &model_type);    // 00697C17..1E (result unused)
    if (find_005e5f20(&model_type, nullptr, 0x57, 0) == -1) {                    // 00697C23..33 'W'
        st<U8>(self, 0x22, ld<U8>(data, 0x22));                                  // 00697C35..3B
        void* const creature = creature_of(self);                                // 00697C38
        oslots(creature)->unload_model(creature, nullptr, 0xFC);                 // 00697C3E..45
    }
    return 1;                                                                    // 00697C5E
}

// ================================================================== 00697CE0 CreateBTypeBody(anim_type, data) RET 8
std::uint32_t __fastcall kotor_m15a_appearance_create_btype_body_00697ce0(void* self, void*, std::uint32_t anim_type,
                                                                          void* data) noexcept(false) {
    Ref body_model;     // [esp+58h] 00697D06
    Ref body_texture;   // [esp+48h] 00697D0F
    Ref env_map;        // [esp+68h] 00697D18
    c2da_default_ref_00405ed0(&body_model, nullptr);
    c2da_default_ref_00405ed0(&body_texture, nullptr);
    c2da_default_ref_00405ed0(&env_map, nullptr);
    StartupString race;      // [esp+20h] 00697D21
    StartupString env_name;  // [esp+14h] 00697D2A
    StartupString unused30;  // [esp+30h] 00697D3E (constructed / destroyed, never written)
    {
        StartupString column("RACE");                                            // 00697D51..61 (0074B2BC)
        c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &race);   // 00697D66..90
    }                                                                            // 00697DA0
    {
        StartupString column("ENVMAP");                                          // 00697DA5..AE (0074B68C)
        c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &env_name); // 00697DB3..DB
    }                                                                            // 00697DEB
    Word has_env;                                                                // [esp+1Ch]
    if (static_cast<std::int32_t>(async_string_length_005e5790(&env_name, nullptr)) > 0) {   // 00697DF0..FB
        ref_from_string(&env_map, &env_name);                                    // 00697DFD..0B
        has_env = 1;                                                             // 00697E10
    } else {
        has_env = 0;                                                             // 00697E1A
    }
    kotor_m15a_appearance_get_btype_model_and_texture_00697610(self, nullptr, data, &body_model, &body_texture); // 00697E22..2F
    if (ld<U16>(self, 0x18) == ld<U16>(data, 0x18) && rd32(self, 0x14) == rd32(data, 0x14) &&
        ld<U8>(self, 0x00) == ld<U8>(data, 0x00) && ld<U8>(self, 0x01) == ld<U8>(data, 0x01) &&
        ld<U8>(self, 0x1B) == ld<U8>(data, 0x1B)) {                              // 00697E34..5A
        return 1;                                                                // 00697E5C..AB
    }
    st<U8>(self, 0x00, ld<U8>(data, 0x00));                                      // 00697EAE..B0
    st<U8>(self, 0x01, ld<U8>(data, 0x01));                                      // 00697EB2..B5
    st<U16>(self, 0x18, ld<U16>(data, 0x18));                                    // 00697EB8..BC
    st<Word>(self, 0x14, rd32(data, 0x14));                                      // 00697EC0..C6
    st<U8>(self, 0x1B, ld<U8>(data, 0x1B));                                      // 00697EC9..CE
    void* head;                                                                  // ebx / [esp+2Ch]
    {
        void* const creature = creature_of(self);                                // 00697EC3
        head = oslots(creature)->get_model(creature, nullptr, kPartHead, 1);     // 00697ECC..D8
    }
    if (head != nullptr) gslots(head)->set_behavior(head, nullptr, nullptr);     // 00697EE6..EE
    Word queued;                                                                 // [esp+0Ch]
    {
        void* const creature = creature_of(self);                                // 00697EF1
        queued = oslots(creature)->clear_queued_anim_callback(creature, nullptr);   // 00697EF4..F6
    }
    {
        void* const creature = creature_of(self);                                // 00697EFC
        oslots(creature)->unload_model(creature, nullptr, kPartBody);            // 00697F03..0A
    }
    {
        void* const creature = creature_of(self);                                // 00697F10
        if (oslots(creature)->load_model(creature, nullptr, &body_model, kPartBody, anim_type) == 0) {  // 00697F0D..25
            char message[0x84];                                                  // [esp+78h] .. EH record
            format_006fadb0(message, "CSWCCreatureAppearance::CreateBTypeBody(): Failed to load model '%s'.",
                            c2da_ref_00405fe0(&body_model, nullptr));            // 00697F27..40 (00754A50; buffer unused)
            return 0;                                                            // 00697F43..8F
        }
    }
    {
        void* const creature = creature_of(self);                                // 00697F92
        void* const body = oslots(creature)->get_model(creature, nullptr, kPartBody, 1);   // 00697F95..9E
        if (body != nullptr) {                                                   // 00697FA4..AA
            const Vector position = ld<Vector>(creature_of(self), 0x24);         // 00697FAC..C6 creature+24h by value
            Vector discarded;                                                    // hidden return (dead temp, 00697FCD)
            gslots(body)->set_position(body, nullptr, &discarded, position);     // 00697FC9..D2
        }
    }
    if (queued != 0) {                                                           // 00697FD9..DF
        void* const creature = creature_of(self);                                // 00697FE1
        oslots(creature)->set_queued_anim_callback(creature, nullptr);           // 00697FE4..E6
    }
    set_env_map(creature_of(self), kPartBody, has_env, &env_map);               // 00697FEC..20
    {
        void* const manager = kotor_menu_window_cells::manager_007a39fc;         // 00698026
        bool refresh = rp(manager, 8) == nullptr;                                // 0069802B..30 server app == 0
        if (!refresh) refresh = kotor_m15_client_is_player_creature_005eda30(rp(manager, 4), nullptr,
                                                                             creature_of(self)) != 0;   // 00698032..40
        if (refresh) {
            void* const creature = creature_of(self);                            // 00698042
            void* const body = oslots(creature)->get_model(creature, nullptr, kPartBody, 1);   // 00698045..4E
            gslots(body)->set_can_down_sample(body, nullptr, 0);                 // 00698054..5A
        }
    }
    if (c2da_not_equal_text_00406060(&body_texture, nullptr, kEmpty_0073d71c) != 0) {   // 0069805D..6D
        Ref all;                                                                 // [esp+38h]
        all.construct_00406d80(kEmpty_0073d71c);                                 // 0069806F..78
        kotor_m15a_object_replace_texture_0063c0a0(creature_of(self), nullptr, kPartBody, &all, &body_texture, 1); // 0069807D..91
    }
    st<U16>(self, 0x18, ld<U16>(data, 0x18));                                    // 00698096..AA
    st<Word>(self, 0x40, 1);                                                     // 006980AE
    st<Word>(self, 0x44, 1);                                                     // 006980B1
    st<Word>(self, 0x48, 1);                                                     // 006980B4
    kotor_m15a_switch_weapon_event_00610f40(nullptr, kAppearanceForceUpdate_0074f4d4, creature_of(self));   // 0069809A..BC
    if (head != nullptr) {                                                       // 006980BF..C1
        void* const creature = creature_of(self);                                // 006980C3
        void* const body = oslots(creature)->get_model(creature, nullptr, kPartBody, 1);   // 006980C6..CE
        gslots(head)->attach_to_object(head, nullptr, body, "HeadHook", 0);      // 006980D4..E0 (00754A40)
    }
    st<Word>(self, 0x50, 1);                                                     // 006980E3
    st<Word>(self, 0x68, kotor_fastcall_005edc10(client_app(), nullptr));        // 006980E6..F7
    return 1;                                                                    // 00698133
}

// ================================================================== 00698150 SetAppearanceAlignment(alignment) RET 4
void __fastcall kotor_m15a_appearance_set_appearance_alignment_00698150(void* self, void*, std::uint32_t alignment_arg) noexcept(false) {
    const U16 alignment = static_cast<U16>(alignment_arg);                       // 0069816F bx = word [ebp+8]
    const std::int16_t signed_alignment = static_cast<std::int16_t>(alignment);  // cmp bx,imm / jg (signed)
    if (rd32(self, 0x50) == 0 && ld<U16>(self, 0x2C) == alignment) return;       // 00698176..84
    void* const creature = creature_of(self);                                    // 0069818A
    st<U16>(self, 0x2C, alignment);                                              // 0069818F
    if (creature == nullptr) return;                                             // 0069818D..93
    if (oslots(creature)->get_model(creature, nullptr, kPartHead, 0) == nullptr) return;   // 00698199..A9
    StartupString head_texture;   // [esp+18h] 006981AF
    StartupString evil_texture;   // [esp+20h] 006981B8
    void* const table = appearance_table();                                      // 006981C5..D1
    Word evil_found = 0;                                                         // [esp+14h] 006981DD
    if (table == nullptr) return;                                                // 006981D6..E1 -> 00698518
    std::int32_t row = 0;   // [esp+10h]; always written by 006981F4 or 00413510 (all paths store *out)
    const U8 forced = ld<U8>(self, 0x34);                                        // 006981E7
    st<Word>(self, 0x50, 0);                                                     // 006981EC
    if (forced != 0xFF) {                                                        // 006981EA..EF
        row = forced;                                                            // 006981F1..F4
        st<Word>(self, 0x40, 1);                                                 // 006981F8
    } else {
        const std::uint32_t column = rd32(self, 0x30) != 0 ? kotor_rules_cell32_007a20c4
                                                           : kotor_rules_cell32_007a2114;   // 00698201..27
        c2da_current_get_int_entry_00413510(table, nullptr, row_of(self), static_cast<std::int32_t>(column), &row); // 00698229
    }
    std::int32_t head_found = 0;                                                 // edi (0069817A)
    const char* head_column = nullptr;
    if (signed_alignment <= 0x0A) head_column = "HEADTEXVVVE";                   // 0069822E..34 (00754AD0)
    else if (signed_alignment <= 0x14) head_column = "HEADTEXVVE";               // 00698254..5A (00754AC4)
    else if (signed_alignment <= 0x1E) head_column = "HEADTEXVE";                // 0069826F..75 (00754AB8)
    else if (signed_alignment <= 0x28) head_column = "HEADTEXE";                 // 0069828A..90 (00754AAC)
    if (head_column != nullptr) {
        StartupString column(head_column);                                       // 00698234..9E
        head_found = c2da_current_get_string_00413270(rp(rp(kotor_cell_rules_007a3a28, 0xB8), 0x44), nullptr, row,
                                                      &column, &head_texture);   // 006982A3..D1 heads table
    }                                                                            // 006982D3
    if (rd32(self, 0x14) == 0 && signed_alignment <= 0x1E) {                     // 006982D8..E3
        StartupString column("TEXAEVIL");                                        // 006982E5..EE (00754AA0)
        evil_found = static_cast<Word>(c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(self),
                                                                        &column, &evil_texture));   // 006982F3..24
    }                                                                            // 0069832D
    if (head_found != 0) {                                                       // 00698332..34
        Ref texture;                                                             // [esp+28h]
        async_ref_construct_string_00406d60(&texture, nullptr, &head_texture);   // 0069833A..43
        bool present = exists(&texture, kResTga) != 0;                           // 00698348..5E
        if (!present) {
            async_ref_construct_string_00406d60(&texture, nullptr, &head_texture);   // 00698360..69
            present = exists(&texture, kResTpc) != 0;                            // 0069836E..87
        }
        if (present) {
            async_ref_construct_string_00406d60(&texture, nullptr, &head_texture);   // 00698389..92
            Ref all;                                                             // [esp+38h]
            all.construct_00406d80(kEmpty_0073d71c);                             // 00698397..A0
            kotor_m15a_object_replace_texture_0063c0a0(creature_of(self), nullptr, kPartHead, &all, &texture, 1); // 006983A5..B9
        }
    } else {
        kotor_m15a_object_restore_texture_0063ce40(creature_of(self), nullptr, kPartHead);   // 006983C0..C8
    }
    if (evil_found != 0) {                                                       // 006983CD..D3
        char name[0x24];                                                         // [esp+48h] .. EH record [esp+6Ch]
        format_006fadb0(name, "%s01", async_string_cstr_005e5670(&evil_texture, nullptr));   // 006983D9..F2 (00754A98)
        c2da_assign_text_005e5140(&evil_texture, nullptr, name);                 // 006983F5..FE
        Ref texture;                                                             // [esp+38h]
        async_ref_construct_string_00406d60(&texture, nullptr, &evil_texture);   // 00698403..0C
        bool present = exists(&texture, kResTga) != 0;                           // 00698411..27
        if (!present) {
            async_ref_construct_string_00406d60(&texture, nullptr, &evil_texture);   // 00698429..32
            present = exists(&texture, kResTpc) != 0;                            // 00698437..50
        }
        if (present) {
            async_ref_construct_string_00406d60(&texture, nullptr, &evil_texture);   // 00698456..5F
            Ref all;                                                             // [esp+28h]
            all.construct_00406d80(kEmpty_0073d71c);                             // 00698464..6D
            kotor_m15a_object_replace_texture_0063c0a0(creature_of(self), nullptr, kPartBody, &all, &texture, 1); // 00698472..86
        }
        return;                                                                  // 0069848B -> 00698518
    }
    Ref body_texture;                                                            // [esp+28h]
    c2da_default_ref_00405ed0(&body_texture, nullptr);                           // 00698490..94
    kotor_m15a_appearance_get_btype_model_and_texture_00697610(self, nullptr, self, nullptr, &body_texture);   // 00698499..A3
    if (c2da_not_equal_text_00406060(&body_texture, nullptr, kEmpty_0073d71c) != 0) {   // 006984A8..B8
        Ref all;                                                                 // [esp+38h]
        all.construct_00406d80(kEmpty_0073d71c);                                 // 006984BA..C3
        kotor_m15a_object_replace_texture_0063c0a0(creature_of(self), nullptr, kPartBody, &all, &body_texture, 1); // 006984C8..DC
    }
    void* const manager = kotor_menu_window_cells::manager_007a39fc;             // 006984E1
    bool refresh = rp(manager, 8) == nullptr;                                    // 006984E6..EB
    if (!refresh) refresh = kotor_m15_client_is_player_creature_005eda30(rp(manager, 4), nullptr,
                                                                         creature_of(self)) != 0;   // 006984ED..FB
    if (refresh) {
        void* const owner = creature_of(self);                                   // 006984FD
        void* const body = oslots(owner)->get_model(owner, nullptr, kPartBody, 1);   // 00698500..09
        gslots(body)->set_can_down_sample(body, nullptr, 0);                     // 0069850F..15
    }
}                                                                                // 00698518..32 ~CExoString x2

// ================================================================== 00698550 CreateSimple(anim_type, data) RET 8
std::uint32_t __fastcall kotor_m15a_appearance_create_simple_00698550(void* self, void*, std::uint32_t anim_type,
                                                                      void* data) noexcept(false) {
    Ref body_model;   // [esp+40h] 00698573
    Ref env_map;      // [esp+50h] 0069857C
    c2da_default_ref_00405ed0(&body_model, nullptr);
    c2da_default_ref_00405ed0(&env_map, nullptr);
    StartupString race;       // [esp+24h] 00698585
    StartupString env_name;   // [esp+1Ch] 00698590
    {
        StartupString column("RACE");                                            // 0069859D..AE (0074B2BC)
        c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &race);   // 006985B3..DD
    }                                                                            // 006985EB
    {
        StartupString column("ENVMAP");                                          // 006985F0..F9 (0074B68C)
        c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &env_name); // 006985FE..26
    }                                                                            // 00698634
    Word has_env;                                                                // [esp+0Ch]
    if (static_cast<std::int32_t>(async_string_length_005e5790(&env_name, nullptr)) > 0) {   // 00698639..44
        ref_from_string(&env_map, &env_name);                                    // 00698646..54
        has_env = 1;                                                             // 00698659
    } else {
        has_env = 0;                                                             // 00698663
    }
    ref_from_string(&body_model, &race);                                         // 00698667..75
    if (ld<U16>(self, 0x18) == ld<U16>(data, 0x18)) return 1;                    // 0069867A..B9
    // 006986BC..C4: three zero stores to an otherwise unused 12-byte local (EH state 4); no observable effect.
    {
        void* const creature = creature_of(self);                                // 006986C8
        void* const gob = oslots(creature)->get_model(creature, nullptr, 0xFB, 1);   // 006986CB..DC
        if (gob != nullptr) {                                                    // 006986E4..E6
            gslots(gob)->attach_to_scene(gob, nullptr, nullptr);                 // 006986E8..ED
            gslots(gob)->attach_to_object(gob, nullptr, nullptr, nullptr, 0);    // 006986F0..F7
        }
    }
    {
        void* const creature = creature_of(self);                                // 006986FA
        void* const gob = oslots(creature)->get_model(creature, nullptr, 0xF7, 1);   // 006986FD..06
        if (gob != nullptr) {                                                    // 0069870E..10
            gslots(gob)->attach_to_scene(gob, nullptr, nullptr);                 // 00698712..17
            gslots(gob)->attach_to_object(gob, nullptr, nullptr, nullptr, 0);    // 0069871A..21
        }
    }
    kotor_m15a_creature_unload_monk_visuals_0060bad0(creature_of(self), nullptr);   // 00698724..27
    Word queued;                                                                 // edi
    {
        void* const creature = creature_of(self);                                // 0069872C
        queued = oslots(creature)->clear_queued_anim_callback(creature, nullptr);   // 0069872F..31
    }
    {
        void* const creature = creature_of(self);                                // 00698737
        oslots(creature)->unload_model(creature, nullptr, kPartBody);            // 0069873C..43
    }
    gui_load_slot_ret_004015a0(creature_of(self), nullptr);                      // 00698746..49
    {
        void* const creature = creature_of(self);                                // 00698751
        if (oslots(creature)->load_model(creature, nullptr, &body_model, kPartBody, anim_type) == 0) return 0;   // 0069874E..66
    }
    if (queued != 0) {                                                           // 0069879C..9E
        void* const creature = creature_of(self);                                // 006987A0
        oslots(creature)->set_queued_anim_callback(creature, nullptr);           // 006987A3..A5
    }
    set_env_map(creature_of(self), kPartBody, has_env, &env_map);               // 006987AB..DC
    {
        void* const creature = creature_of(self);                                // 006987EA
        if (client_get_player_creature_005ed540(client_app(), nullptr) == creature) {   // 006987E2..F4
            void* const body = oslots(creature)->get_model(creature, nullptr, kPartBody, 1);   // 006987F6..01
            gslots(body)->set_can_down_sample(body, nullptr, 0);                 // 00698807..0C
        }
    }
    std::memcpy(self, data, 0x3C);                                               // 0069880F..19 rep movs 0Fh dwords
    st<Word>(self, 0x40, 1);                                                     // 00698824
    st<Word>(self, 0x44, 1);                                                     // 00698827
    st<Word>(self, 0x48, 1);                                                     // 0069882A
    StartupString model_type;                                                    // [esp+2Ch] 00698820..2D
    {
        StartupString column("MODELTYPE");                                       // 00698832..43 (00747C2C)
        c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &model_type); // 00698848..72
    }                                                                            // 00698880
    const U8 type = kotor_m15a_appearance_get_anim_type_00696d40(self, nullptr, &model_type);   // 00698885..8C
    anim_type = (anim_type & 0xFFFFFF00u) | type;                                // 00698893 mov [ebp+8],al
    bool legged = false;   // 'L' absent from MODELTYPE for anim types 2/3
    if (type == 2 || type == 3)                                                  // 00698891..9A
        legged = find_005e5f20(&model_type, nullptr, 0x4C, 0) == -1;            // 0069889C..AC
    if (legged) {
        if (gob_get_property_0x68_0063c070(creature_of(self), nullptr) != nullptr) {   // 006988AE..B8
            void* const anim = gob_get_property_0x68_0063c070(creature_of(self), nullptr);   // 006988BA..BD
            aslots(anim)->slot90(anim, nullptr, 0);                              // 006988C2..C8
        }
        kotor_m15a_creature_set_creature_type_0060c8c0(creature_of(self), nullptr, 1);   // 006988CE + 006988F2..F5
    } else {
        if (gob_get_property_0x68_0063c070(creature_of(self), nullptr) != nullptr) {   // 006988D1..DB
            void* const anim = gob_get_property_0x68_0063c070(creature_of(self), nullptr);   // 006988DD..E0
            aslots(anim)->slot90(anim, nullptr, 1);                              // 006988E5..EA
        }
        kotor_m15a_creature_set_creature_type_0060c8c0(creature_of(self), nullptr, 2);   // 006988F0..F5
    }
    if (static_cast<U8>(anim_type) != 3) {                                       // 006988FA..FE
        std::int32_t found;
        {
            StartupString column("RaceTex");                                     // 00698904..0D (00754ADC)
            found = c2da_current_get_string_00413270(appearance_table(), nullptr, row_of(data), &column, &env_name); // 00698912..39
        }                                                                        // 00698949
        if (found != 0) {                                                        // 0069894E..50
            Ref texture;                                                         // [esp+0Ch]
            async_ref_construct_string_00406d60(&texture, nullptr, &env_name);   // 00698952..5B
            bool present = exists(&texture, kResTga) != 0;                       // 00698960..76
            if (!present) {
                async_ref_construct_string_00406d60(&texture, nullptr, &env_name);   // 00698978..81
                present = exists(&texture, kResTpc) != 0;                        // 00698986..9F
            }
            if (present) {
                async_ref_construct_string_00406d60(&texture, nullptr, &env_name);   // 006989A1..AA
                Ref all;                                                         // [esp+60h]
                all.construct_00406d80(kEmpty_0073d71c);                         // 006989AF..B8
                kotor_m15a_object_replace_texture_0063c0c0(creature_of(self), nullptr, &all, &texture, 1);   // 006989BD..CB
            }
        }
    }
    kotor_m15a_creature_update_weapon_class_00613710(creature_of(self), nullptr);   // 006989D0..D3
    kotor_m15a_switch_weapon_event_00610f40(nullptr, kAppearanceForceUpdate_0074f4d4, creature_of(self));   // 006989D8..E3
    st<Word>(self, 0x68, kotor_fastcall_005edc10(client_app(), nullptr));        // 006989E8..FD
    return 1;                                                                    // 00698A2D (edi = 1)
}

// ================================================================== 00698B10 CreateBodyAppearance(data) RET 4
std::uint32_t __fastcall kotor_m15a_appearance_create_body_appearance_00698b10(void* self, void*, void* data) noexcept(false) {
    StartupString model_type;                                                    // [esp+14h] 00698B2E
    kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a20d8), static_cast<float*>(at(self, 0x58)));   // 00698B37..63
    kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a20dc), static_cast<float*>(at(self, 0x5C)));   // 00698B68..88
    kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a20e0), static_cast<float*>(at(self, 0x60)));   // 00698B8D..AD
    kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a20e4), static_cast<float*>(at(self, 0x64)));   // 00698BB2..D2
    // fcomp against 0.0f (0073D700); jp skips the default store when not equal.
    if (ld<float>(self, 0x58) == 0.0f) st<float>(self, 0x58, 25.0f);            // 00698BD7..E7 41C80000h
    if (ld<float>(self, 0x5C) == 0.0f) st<float>(self, 0x5C, 6.0f);             // 00698BEE..FD 40C00000h
    if (ld<float>(self, 0x60) == 0.0f) st<float>(self, 0x60, 2.0f);             // 00698C03..13 40000000h
    if (ld<float>(self, 0x64) == 0.0f) st<float>(self, 0x64, 4.0f);             // 00698C1A..2A 40800000h
    void* const server = kotor_m15a_creature_get_server_creature_0060fb20(creature_of(self), nullptr);   // 00698C31..34
    if (server != nullptr) {                                                     // 00698C39..3B
        const double factor = kotor_m15_screature_get_movement_rate_factor_004ec370(server, nullptr);   // 00698C3D..3F st0
        st<float>(self, 0x5C, static_cast<float>(factor * static_cast<double>(ld<float>(self, 0x5C))));  // 00698C44 fmul; 00698C46 fstp dword
    }
    float value = 0.0f;   // [esp+10h]; GetFLOATEntry 00413350 stores *out on every path
    kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a2118), &value);         // 00698C48..69
    st<float>(self, 0x24, value);                                                // 00698C6E..72
    c2da_current_get_string_entry_00413190(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a2110), &model_type);   // 00698C75..96 MODELTYPE
    const U8 type = kotor_m15a_appearance_get_anim_type_00696d40(self, nullptr, &model_type);   // 00698C9B..A7
    // 00698CAF: the type byte replaces the low byte of the `data` argument slot, which is then pushed whole.
    const std::uint32_t type_word = (static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(data)) & 0xFFFFFF00u) |
                                    type;
    std::uint32_t built;                                                         // ebp
    if (type == 2 || type == 0x0B)                                               // 00698CAC..BB
        built = kotor_m15a_appearance_create_simple_00698550(self, nullptr, type_word, data);       // 00698CD0..DD
    else if (type == 3)                                                          // 00698CB5..B6
        built = kotor_m15a_appearance_create_btype_body_00697ce0(self, nullptr, type_word, data);   // 00698CC1..CE
    else
        built = 0;                                                               // 00698CBD
    kotor_m15b_c2da_get_float_entry_00413350(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a2104), &value);         // 00698CDF..01
    st<float>(self, 0x28, value);                                                // 00698D06..0A
    c2da_current_get_int_entry_00413510(appearance_table(), nullptr, row_of(data),
        static_cast<std::int32_t>(kotor_rules_cell32_007a20f0), static_cast<std::int32_t*>(at(self, 0x38)));   // 00698D0D..2D
    if (built != 0) {                                                            // 00698D32..34
        bool legged = false;   // 'L' absent from MODELTYPE for anim types 2/3
        if (type == 2 || type == 3) {                                            // 00698D3A..42
            StartupString letter("L");                                           // 00698D44..4D (00746BA4)
            legged = client_movies::StringFind_005e5eb0(reinterpret_cast<options_video::String*>(&model_type), nullptr,
                         reinterpret_cast<options_video::String*>(&letter), 0) == -1;   // 00698D52..6E sete bl
        }                                                                        // 00698D76
        if (legged) {
            if (gob_get_property_0x68_0063c070(creature_of(self), nullptr) != nullptr) {   // 00698D7F..89
                void* const anim = gob_get_property_0x68_0063c070(creature_of(self), nullptr);   // 00698D8B..8E
                aslots(anim)->slot90(anim, nullptr, 0);                          // 00698D93..99
            }
            kotor_m15a_creature_set_creature_type_0060c8c0(creature_of(self), nullptr, 1);   // 00698D9F + 00698DC5..C8
        } else {
            if (gob_get_property_0x68_0063c070(creature_of(self), nullptr) != nullptr) {   // 00698DA3..AD
                void* const anim = gob_get_property_0x68_0063c070(creature_of(self), nullptr);   // 00698DAF..B2
                aslots(anim)->slot90(anim, nullptr, 1);                          // 00698DB7..BD
            }
            kotor_m15a_creature_set_creature_type_0060c8c0(creature_of(self), nullptr, 2);   // 00698DC3..C8
        }
        kotor_m15a_creature_update_weapon_class_00613710(creature_of(self), nullptr);   // 00698DCD..D0
    }
    return built;                                                                // 00698DEC mov eax,ebp
}
}
