// lane-w3 unit w3-client-player-model: the creature model-request chain behind CSWCCreature::RequestAppearance
// (0060C910, reached from the creature appearance message 00655E40 while [007A2484] is set and the area is ready to
// display) and the CSWCItem constructor / tables behind CSWCMessage::CreateItem (0064B540, equipped items).
//   00698A40 CSWCCreatureAppearance::RequestBodyAppearance   00697C80 RequestBType   00696E10 RequestSimple
//   00696F60 CSWCCreatureAppearance::RequestHeadAppearance
//   0060C1E0 CSWCCreature::RequestModel (client creature table 0074F5F8 slot +158h)
//   0070D980 CAuroraInterface::IsResident   0070D8D0 CAuroraInterface::RequestModel   004640F0 BinaryFindModel
//   005E9AD0 CExoLinkedListInternal::AddBefore
//   00646230 CSWCItem::CSWCItem   005B4660 CSWItem::CSWItem   00646380 AsSWItem thunk   00646500 LoadModel thunk
//   00646930 SetArea thunk   00646830 CSWCItem::ShowVisualEffect   00646CD0 ~CSWCItem   006474B0 scalar deleting
//   destructor   00646CC0 destructor thunk   tables 00751BA0 (1 slot) / 00751A58 (82 slots) / 0074B2A0 (1 slot)
// The model request queues the model on the CAuroraInterface requested-model list (GetModelInternal mode 1); the
// model itself is loaded later by CSWCCreature::ForceAppearanceUpdate 00614A30 (lane W4) -> SetAppearance 006134C0
// -> CSWCCreature::LoadModel 0061B380 (runtime1115). Those, the render core (GetModelInternal, BinarySearchModel,
// CSWCAnimBase bodies, Gob slots) and every runtime1115 owner in w3_client_player_model.h are only called.
// Original: swkotor.exe GOG K1 (image base 0x400000), read-only static disassembly (lane-w3 tools/odis.py, sdis.py);
// nothing original was executed. Exception unwinding of the original frames is not reproduced (lane convention):
// every temporary is destroyed explicitly on each path.
#include "5475f30cc8-w3_client_player_model.h"

using namespace kotor_w3;

namespace {
constexpr U kInvalidObject_00746644 = 0x7F000000u;                     // .rdata 00746644
constexpr U8 kItemType_00746635 = 6;                                   // .rdata 00746635 (CSWCObject type: item)
const char kModelType_00747c2c[] = "MODELTYPE";                        // .rdata 00747C2C
const char kRace_0074b2bc[] = "RACE";                                  // .rdata 0074B2BC
const char kGender_0074b76c[] = "GENDER";                              // .rdata 0074B76C
const char kP_00746bb4[] = "P";                                        // .rdata 00746BB4
const char kHead_00754a0c[] = "_HEAD";                                 // .rdata 00754A0C
const char kZeroHead_00754a04[] = "0_HEAD";                            // .rdata 00754A04
const char kThreeDigits_007549fc[] = "%03u";                           // .rdata 007549FC
const char kEmpty_0073d71c[] = "";                                     // .rdata 0073D71C
const char kNull_0073ee04[] = "NULL";                                  // .rdata 0073EE04
const char kRoot_00740c00[] = "root";                                  // .rdata 00740C00
const char kListFile_0074e208[] = "exolinkedlistinternal.cpp";         // .rdata 0074E208
const char kAddBeforeNull_0074e224[] = "CExoLinkedListInternal::AddBefore() Position parameter is NULL";   // 0074E224
constexpr U kHalf_3f000000 = 0x3F000000u;                              // immediate 0.5f
// CSWAnimBase::Set(index, float) immediates of CSWCCreature::RequestModel
constexpr U kSet0_44e74000 = 0x44E74000u;                              // 1850.0f
constexpr U kSet1_45ce4000 = 0x45CE4000u;                              // 6600.0f
constexpr U kSet2_3f6ccccd = 0x3F6CCCCDu;                              // 0.925f
constexpr U kSet3_40533333 = 0x40533333u;                              // 3.3f

void exo_init(void* s) noexcept(false) {                               // 005B3190 CExoString()
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr);
}
void exo_text(void* s, const char* text) noexcept(false) { frame_LogStringCtor_005e5a90(s, nullptr, text); }   // 005E5A90
void exo_free(void* s) noexcept(false) { frame_LogStringDtor_005e5c20(s, nullptr); }                             // 005E5C20
// 005E5D10 CExoString::operator+(rhs) into `out`, then 005E5C50 operator= into `self` (the original's
// temp/assign/destroy triple around every concatenation).
void exo_append(ExoString* self, ExoString* temp, const ExoString* rhs) noexcept(false) {
    using kotor_reconstruction::StartupString;
    GuiConcat005e5d10(reinterpret_cast<StartupString*>(self), nullptr, reinterpret_cast<StartupString*>(temp),
                      reinterpret_cast<const StartupString*>(rhs));
    async_assign_string_005e5c50(self, nullptr, temp);
    exo_free(temp);
}
// [[007A3A28]+B8h] CTwoDimArrays: +0Ch appearance.2da, +10h gender.2da
void* two_da(unsigned offset) noexcept { return rp(rp(kotor_cell_rules_007a3a28, 0xB8), offset); }
// C2DA::GetCExoStringEntry(row, CExoString const& column, CExoString* out) with a literal column temporary.
void column_string(unsigned table, int row, ExoString* column, const char* name, ExoString* out) noexcept(false) {
    exo_text(column, name);
    (void)c2da_current_get_string_00413270(two_da(table), nullptr, row, column, out);
    exo_free(column);
}
// [this+3Ch] CSWCCreature* owner; slot +158h CSWCCreature::RequestModel(CResRef* model, CResRef* texture, BYTE) RET 0Ch
U owner_request_model(void* appearance, ResRef* model, ResRef* texture, U anim_type) noexcept(false) {
    void* const owner = rp(appearance, 0x3C);
    return vslot<U (__fastcall*)(void*, void*, U, U, U)>(owner, 0x158)(owner, nullptr, word_of(model), word_of(texture), anim_type);
}
using GobPtr = void (__fastcall*)(void*, void*, void*);                // Gob +74h (one dword) RET 4
using GobU = void (__fastcall*)(void*, void*, U);                      // Gob +7Ch / +C4h / +4Ch RET 4
using Gob3 = void (__fastcall*)(void*, void*, U, U, U);                // Gob +50h (three dwords) RET 0Ch
using SlotGetModel = void* (__fastcall*)(void*, void*, U, U);          // CSWCObject +98h GetModel(BYTE, BOOL) RET 8
using SlotPos = void (__fastcall*)(void*, void*, const float*);            // CSWCObject +8Ch SetPosition RET 4
using SlotVoid = U (__fastcall*)(void*, void*);                        // anim +78h UnloadModel RET 0
using AnimLoadModel = U (__fastcall*)(void*, void*, void*, U);         // anim +0Ch LoadModel(CResRef*, BYTE) RET 8
using AnimGetModel = void* (__fastcall*)(void*, void*, U);             // anim +08h GetModel(BYTE) RET 4
}  // namespace

extern "C" {
// ============================================================================================ CSWCCreatureAppearance
// 00697C80 RequestBType(request) RET 4 -> 1. Owner->RequestModel(GetBTypeModelAndTextureResRef model, "", 3).
U __fastcall recovered_client_appearance_request_btype_00697c80(void* appearance, void*, const void* request) noexcept(false) {
    ResRef model;                                                       // {E-0x20}
    kotor_fastcall_00405ed0(&model, nullptr);                           // 00697C86..00697C8A
    kotor_m15a_appearance_get_btype_model_and_texture_00697610(appearance, nullptr, const_cast<void*>(request), &model,
                                                                nullptr);   // 00697C8F..00697C9D (texture out 0)
    ResRef texture;                                                     // {E-0x10}
    reinterpret_cast<menu_resources::Ref*>(&texture)->construct_00406d80(kEmpty_0073d71c);   // 00697CA2..00697CAB
    owner_request_model(appearance, &model, &texture, 3);               // 00697CB0..00697CC1 [[this+3Ch]]+158h
    return 1;                                                           // 00697CC7
}

// 00696E10 RequestSimple(request) RET 4 -> 1. Model = appearance.2da RACE of the request's type (+18h), texture "",
// anim type = GetAnimType(MODELTYPE). The original pushes the request argument slot after overwriting its low byte
// with the anim type (00696EEF); RequestModel reads only that byte (0060C1FF), so the byte is passed zero-extended.
U __fastcall recovered_client_appearance_request_simple_00696e10(void* appearance, void*, const void* request) noexcept(false) {
    ExoString race, model_type, column;                                 // {E-0x34} / {E-0x3c} / {E-0x2c}
    exo_init(&race);                                                    // 00696E2C..00696E30
    exo_init(&model_type);                                              // 00696E35..00696E41
    const int row = static_cast<int>(ld<U16>(request, 0x18));           // 00696E70 movzx word [edi+18h]
    column_string(0xC, row, &column, kRace_0074b2bc, &race);           // 00696E46..00696E8F
    column_string(0xC, row, &column, kModelType_00747c2c, &model_type);   // 00696E94..00696ED5
    const U8 anim_type = kotor_m15a_appearance_get_anim_type_00696d40(appearance, nullptr, &model_type);   // 00696EDA..00696EE1
    ResRef texture, model;                                              // {E-0x2c} / {E-0x1c}
    reinterpret_cast<menu_resources::Ref*>(&texture)->construct_00406d80(kEmpty_0073d71c);   // 00696EE6..00696EF3
    async_ref_construct_string_00406d60(&model, nullptr, &race);        // 00696EF8..00696F01
    owner_request_model(appearance, &model, &texture, anim_type);       // 00696F06..00696F1A
    exo_free(&model_type);                                              // 00696F20..00696F29
    exo_free(&race);                                                    // 00696F2E..00696F3A
    return 1;                                                           // 00696F44
}

// 00698A40 RequestBodyAppearance(request) RET 4: anim type of MODELTYPE: 2 / 0Bh -> RequestSimple, 3 -> RequestBType,
// anything else -> 0.
U __fastcall recovered_client_appearance_request_body_00698a40(void* appearance, void*, const void* request) {
    ExoString model_type, column;                                       // {E-0x1c} / {E-0x14}
    exo_init(&model_type);                                              // 00698A5C..00698A60
    column_string(0xC, static_cast<int>(ld<U16>(request, 0x18)), &column, kModelType_00747c2c, &model_type);   // 00698A65..00698AB1
    U result;                                                           // esi
    switch (kotor_m15a_appearance_get_anim_type_00696d40(appearance, nullptr, &model_type)) {   // 00698AB6..00698AD0
    case 2: case 0xB:
        result = recovered_client_appearance_request_simple_00696e10(appearance, nullptr, request);   // 00698AE0..00698AE8
        break;
    case 3:
        result = recovered_client_appearance_request_btype_00697c80(appearance, nullptr, request);    // 00698AD6..00698AE8
        break;
    default:
        result = 0;                                                     // 00698AD2
        break;
    }
    exo_free(&model_type);                                              // 00698AEA..00698AF6
    return result;                                                      // 00698B00
}

// 00696F60 RequestHeadAppearance(request) RET 4 -> 1. Only for anim type 3 with a head number (+1Ah) and no helmet
// (+10h == OBJECT_INVALID): model = "P" + GENDER + RACE + <+1Ch> + "_HEAD" + %03u(+1Ah),
// texture = "P" + GENDER + RACE + "0_HEAD" + %03u(+1Ah); owner->RequestModel(model, texture, anim type of MODELTYPE).
U __fastcall recovered_client_appearance_request_head_00696f60(void* appearance, void*, const void* request) {
    ExoString model, texture, text, column, temp;                       // {E-0x4c} / {E-0x3c} / {E-0x44} / {E-0x54},{E-0x34} / {E-0x34},{E-0x54}
    exo_init(&model);                                                   // 00696F83..00696F87
    exo_init(&texture);                                                 // 00696F8C..00696F98
    exo_init(&text);                                                    // 00696F9D..00696FA6
    const int row = static_cast<int>(ld<U16>(request, 0x18));           // 00696FD5 movzx word [esi+18h]
    column_string(0xC, row, &column, kModelType_00747c2c, &text);       // 00696FAD..00696FF3
    if (kotor_m15a_appearance_get_anim_type_00696d40(appearance, nullptr, &text) == 3 &&   // 00696FF8..00697006
        ld<U8>(request, 0x1A) != 0 &&                                   // 0069700C..00697019
        rd32(request, 0x10) == kInvalidObject_00746644) {               // 0069700F..00697021
        c2da_assign_text_005e5140(&model, nullptr, kP_00746bb4);        // 00697027..00697030
        const int gender_row = ld<U8>(request, 0x1B) == 1 ? 1 : 0;      // 00697035..00697049 {E-0x54}
        column_string(0x10, gender_row, &column, kGender_0074b76c, &text);   // 00697040..00697084
        exo_append(&model, &temp, &text);                               // 00697089..006970B3
        column_string(0xC, row, &column, kRace_0074b2bc, &text);        // 006970B8..006970F8
        exo_append(&model, &temp, &text);                               // 006970FD..00697127
        async_assign_string_005e5c50(&texture, nullptr, &model);        // 0069712C..00697135
        {
            ExoString number;                                           // {E-0x54}
            resman_string_from_int_005e5bc0(&number, nullptr, static_cast<int>(ld<U8>(request, 0x1C)));   // 0069713A..00697143
            exo_append(&model, &temp, &number);                         // 00697148..00697174
            exo_free(&number);                                          // 00697179..00697181
        }
        exo_text(&column, kHead_00754a0c);                              // 00697186..0069718F {E-0x34}
        exo_append(&model, &temp, &column);                             // 00697194..006971C4 (temp {E-0x54})
        exo_free(&column);                                              // 006971C9..006971D1
        exo_text(&column, kZeroHead_00754a04);                          // 006971D6..006971DF
        exo_append(&texture, &temp, &column);                           // 006971E4..00697214
        exo_free(&column);                                              // 00697219..00697221
        char digits[4];                                                 // {E-0x58}: "%03u" of a byte is at most 3 digits
        format_006fadb0(digits, kThreeDigits_007549fc, static_cast<unsigned>(ld<U8>(request, 0x1A)));   // 00697226..0069723A
        exo_text(&column, digits);                                      // 0069723D..00697246
        exo_append(&model, &temp, &column);                             // 0069724B..0069727B
        exo_free(&column);                                              // 00697280..00697288
        exo_text(&column, digits);                                      // 0069728D..00697296
        exo_append(&texture, &temp, &column);                           // 0069729B..006972CB
        exo_free(&column);                                              // 006972D0..006972D8
        column_string(0xC, row, &column, kModelType_00747c2c, &text);   // 006972DD..0069731D
        const U8 anim_type = kotor_m15a_appearance_get_anim_type_00696d40(appearance, nullptr, &text);   // 00697322..00697337
        ResRef texture_ref, model_ref;                                  // {E-0x34} / {E-0x24}
        async_ref_construct_string_00406d60(&texture_ref, nullptr, &texture);   // 0069732E..0069733B
        async_ref_construct_string_00406d60(&model_ref, nullptr, &model);       // 00697340..00697349
        // 0069734E: the anim type sits in the low byte of {E-0x54}, whose upper bytes the gender row (0 / 1) zeroed.
        owner_request_model(appearance, &model_ref, &texture_ref, anim_type);   // 00697352..00697362
    }
    exo_free(&text);                                                    // 00697368..00697371
    exo_free(&texture);                                                 // 00697376..0069737F
    exo_free(&model);                                                   // 00697384..00697390
    return 1;                                                           // 0069739B
}

// ============================================================================================ CSWCCreature
// 0060C1E0 CSWCCreature::RequestModel(CResRef const* model, CResRef const* texture, BYTE anim_type) RET 0Ch.
// Builds the anim base for a new anim type (pending in +358h while +68h is in use), then queues the model on the
// CAuroraInterface requested list unless it is already resident. The texture string is built and only measured.
U __fastcall recovered_client_creature_request_model_0060c1e0(void* self, void*, const void* model, const void* texture,
                                                              U anim_type_word) noexcept(false) {
    const U8 anim_type = static_cast<U8>(anim_type_word);              // 0060C1FF cl
    void* const current = rp(self, 0x68);                               // 0060C1FC
    if (current == nullptr || ld<U8>(current, 0xC4) != anim_type) {     // 0060C209..0060C213
        void* made;
        switch (anim_type) {                                            // 0060C219..0060C22C (byte table 0060C494)
        case 0x0: {                                                     // 0060C233 CSWCAnimBase
            void* const memory = live_operator_new(0xF0);               // 0060C233..0060C23D
            made = memory != nullptr ? kotor_m15e_anim_base_construct_0069dfb0(memory, nullptr) : nullptr;   // 0060C244..0060C24E / 0060C295
            break;
        }
        case 0xB: {                                                     // 0060C262 CSWCAnimBaseTW
            void* const memory = live_operator_new(0x174);              // 0060C262..0060C26C
            made = memory != nullptr ? kotor_m15e_anim_base_tw_construct_0069cbd0(memory, nullptr) : nullptr;   // 0060C273..0060C281 / 0060C295
            break;
        }
        case 0x1: case 0x2: case 0x3: {                                 // 0060C2A6 / 0060C2DC / 0060C319
            void* constructed = nullptr;
            if (anim_type == 1) {
                void* const memory = live_operator_new(0x1C4);          // 0060C2A6..0060C2B0 CSWCAnimBaseHead
                if (memory != nullptr) constructed = kotor_m15e_anim_base_head_construct_0069bb80(memory, nullptr, 1);   // 0060C2B7..0060C2C7
            } else if (anim_type == 2) {
                void* const memory = live_operator_new(0x1D0);          // 0060C2DC..0060C2E6 CSWCAnimBaseWield
                if (memory != nullptr) constructed = kotor_m15e_anim_base_wield_construct_00699dd0(memory, nullptr, 1);   // 0060C2ED..0060C2FD
            } else {
                void* const memory = live_operator_new(0x220);          // 0060C319..0060C323 CSWCAnimBaseHeadWield
                if (memory != nullptr) constructed = kotor_m15e_anim_base_head_wield_construct_00698ec0(memory, nullptr, 1);   // 0060C32A..0060C33A
            }
            // 0060C2D0..0060C346: virtual-base adjustment: this + [[this]+4] (the CSWCAnimBase sub-object)
            made = constructed != nullptr
                       ? word_ptr(word_of(constructed) + rd32(rp(constructed, 0), 4))
                       : nullptr;                                       // 0060C2D8 / 0060C30E
            break;
        }
        default:
            return 0;                                                   // 0060C21F / byte table 4..0Ah -> 0060C463
        }
        st<void*>(self, 0x358, made);                                   // 0060C257 / 0060C28A / 0060C29B / 0060C348
        void* const pending = rp(self, 0x358);                          // 0060C34E
        if (pending == nullptr) return 0;                               // 0060C354..0060C356 -> 0060C463
        kotor_m15e_sw_anim_base_set_00698e30(pending, nullptr, 0, kSet0_44e74000);              // 0060C35C..0060C362
        kotor_m15e_sw_anim_base_set_00698e30(rp(self, 0x358), nullptr, 1, kSet1_45ce4000);      // 0060C367..0060C374
        kotor_m15e_sw_anim_base_set_00698e30(rp(self, 0x358), nullptr, 2, kSet2_3f6ccccd);      // 0060C379..0060C386
        kotor_m15e_sw_anim_base_set_00698e30(rp(self, 0x358), nullptr, 3, kSet3_40533333);      // 0060C38B..0060C398
        st<U>(self, 0x35C, 0);                                          // 0060C3A0
        if (rp(self, 0x68) == nullptr) {                                // 0060C39D..0060C3A6
            st<void*>(self, 0x68, rp(self, 0x358));                     // 0060C3A8..0060C3AE
            st<U>(self, 0x1F8, 0);                                      // 0060C3B1
            st<void*>(self, 0x358, nullptr);                            // 0060C3B7
        }
    }
    ExoString name;                                                     // {E-0x14}
    exo_init(&name);                                                    // 0060C3BD..0060C3C1
    c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(const_cast<void*>(model)), nullptr, reinterpret_cast<menu_resources::String*>(&name));   // 0060C3C6..0060C3D7
    if (recovered_aurora_is_resident_0070d980(async_string_cstr_005e5670(&name, nullptr)) == 0) {   // 0060C3DC..0060C3F4
        recovered_aurora_request_model_0070d8d0(async_string_cstr_005e5670(&name, nullptr));          // 0060C3F6..0060C401
        c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(const_cast<void*>(texture)), nullptr, reinterpret_cast<menu_resources::String*>(&name));                      // 0060C404..0060C40D
        (void)async_string_length_005e5790(&name, nullptr);             // 0060C412..0060C416 (result unused)
    }
    exo_free(&name);                                                    // 0060C41B..0060C423 / 0060C441..0060C445
    return 1;                                                           // 0060C42A / 0060C44C
}

// ============================================================================================ CAuroraInterface
// 004640F0 BinaryFindModel(char* name) cdecl: BinarySearchModel(name, 0, [007A8A68] - 1).
void* __cdecl recovered_aurora_binary_find_model_004640f0(const char* name) noexcept(false) {
    const int count = ld<I32>(historical_storage_007a8a64, 4);          // 004640F0 [007A8A68]
    return aurora_binary_search_model_00464090(name, 0, count - 1);     // 004640F5..00464103
}
// 0070D980 CAuroraInterface::IsResident(char* name) cdecl -> BOOL: a non-null name other than "NULL" in the cache.
U __cdecl recovered_aurora_is_resident_0070d980(const char* name) noexcept(false) {
    if (name == nullptr) return 0;                                      // 0070D985..0070D987 -> 0070D9AE
    if (kotor_stricmp_0070acaf(name, kNull_0073ee04) == 0) return 0;    // 0070D989..0070D999
    return recovered_aurora_binary_find_model_004640f0(name) != nullptr ? 1u : 0u;   // 0070D99B..0070D9A9
}
// 0070D8D0 CAuroraInterface::RequestModel(char* name) cdecl: GetModelInternal(name, 1) creates the (unloaded) model
// entry; a new entry is linked into the requested list [00834094] before its sort position, or at the tail.
void __cdecl recovered_aurora_request_model_0070d8d0(const char* name) noexcept(false) {
    U created = 0;                                                      // {E-0x1c} (0070D8F2)
    void* position = nullptr;                                           // {E-0x18} (always written by the helper)
    ExoString text;                                                     // {E-0x14}
    exo_text(&text, name);                                              // 0070D8E8..0070D8FA
    void* const model = model_0070e080(&text, 1, &created, &position);  // 0070D8FF..0070D91D
    exo_free(&text);                                                    // 0070D920..0070D92E
    if (model == nullptr || created == 0) return;                       // 0070D933..0070D93D
    void* const list = rp(&kotor_requested_models::list_00834094, 0);   // 0070D945 [00834094] (CExoLinkedListInternal*)
    if (position != nullptr)                                            // 0070D93F..0070D94B
        recovered_linked_list_add_before_005e9ad0(list, nullptr, model, position);   // 0070D94D..0070D94F
    else
        resource_append_005e9dc0(list, nullptr, model);                 // 0070D964..0070D965 AddTail
}

// 005E9AD0 CExoLinkedListInternal::AddBefore(void* data, CExoLinkedListNode* position) RET 8 -> new node / 0.
// List {+0 head, +4 tail, +8 count}; node (0Ch) {+0 prev, +4 next, +8 data}.
void* __fastcall recovered_linked_list_add_before_005e9ad0(void* list, void*, void* data, void* position) noexcept(false) {
    if (position == nullptr) {                                          // 005E9AD5..005E9ADA
        base_ini_reconstruction::warning_005e9400(
            static_cast<kotor_menu20::Debug*>(rp(menu_continuation::exo_base_007a39e0, 8)), nullptr, 0xBF,
            kListFile_0074e208, kAddBeforeNull_0074e224);               // 005E9ADC..005E9AF3 [[007A39E0]+8]
        return nullptr;                                                 // 005E9AF9
    }
    void* node = live_operator_new(0xC);                                // 005E9AFF..005E9B06
    if (node != nullptr) {                                              // 005E9B09..005E9B0B
        st<U>(node, 8, 0); st<U>(node, 4, 0); st<U>(node, 0, 0);        // 005E9B0D..005E9B1B
    }  // 005E9B23: the following node+4 store retains native allocation-failure behavior.
    st<void*>(node, 4, position);                                       // 005E9B25
    st<void*>(node, 0, rp(position, 0));                                // 005E9B28..005E9B2A
    if (rp(position, 0) != nullptr) st<void*>(rp(position, 0), 4, node);   // 005E9B2C..005E9B32
    st<void*>(position, 0, node);                                       // 005E9B39
    st<void*>(node, 8, data);                                           // 005E9B35..005E9B3B
    if (rp(list, 4) == nullptr) st<void*>(list, 4, node);              // 005E9B3E..005E9B45
    if (rp(list, 0) == position) st<void*>(list, 0, node);             // 005E9B48..005E9B4C
    st<U>(list, 8, rd32(list, 8) + 1);                                  // 005E9B4E
    return node;                                                        // 005E9B53
}

// ============================================================================================ CSWItem / CSWCItem
// 005B4660 CSWItem::CSWItem(): table 0074B2A0, [+0Ch] = 1Eh, DWORD [+4] and WORD [+8] cleared. RET 0, EAX = this.
void* __fastcall recovered_sw_item_construct_005b4660(void* self, void*) noexcept {
    st<const void*>(self, 0, recovered_sw_item_methods_0074b2a0);       // 005B4665
    st<U>(self, 0xC, 0x1E);                                             // 005B466B
    st<U>(self, 4, 0);                                                  // 005B4674
    st<U16>(self, 8, 0);                                                // 005B4676
    return self;                                                        // 005B4660 eax = ecx
}

// 00646230 CSWCItem::CSWCItem(DWORD base_item) RET 4, EAX = this. CSWItem at +0, CSWCObject (type 6) at +10h.
void* __fastcall recovered_client_item_construct_00646230(void* self, void*, U base_item) {
    recovered_sw_item_construct_005b4660(self, nullptr);               // 0064624F
    kotor_m15a_object_construct_0063e530(at(self, 0x10), nullptr, kItemType_00746635);   // 00646254..00646267
    st<const void*>(self, 0, recovered_client_item_methods_00751ba0);   // 00646277
    st<const void*>(self, 0x10, recovered_client_item_object_methods_00751a58);   // 0064627D
    kotor_fastcall_00405ed0(at(self, 0x128), nullptr);                  // 0064626C..00646283 model CResRef
    kotor_fastcall_00405ed0(at(self, 0x138), nullptr);                  // 00646288..0064628E icon CResRef
    exo_init(at(self, 0x18C));                                          // 00646293..00646299 description
    st<U>(self, 0x150, 0xFFFFFFFFu);                                    // 006462A5
    st<U>(self, 0x14C, 0xFFFFFFFFu);                                    // 006462AB
    st<U>(self, 0x154, 0xFFFFFFFFu);                                    // 006462B1
    st<U>(self, 0x158, 0xFFFFFFFFu);                                    // 006462B7
    st<U>(self, 0x184, 0xFFFFFFFFu);                                    // 006462BD
    st<U>(self, 0x148, 0);                                              // 006462C5
    st<U8>(self, 0x15D, 0xFF);                                          // 006462CB
    st<U8>(self, 0x15C, 0xFF);                                          // 006462D1
    st<U>(self, 0xC, base_item);                                        // 006462D7
    st<U8>(self, 0x15F, 0xFF);                                          // 006462DA
    st<U8>(self, 0x15E, 0xFF);                                          // 006462E0
    st<U>(self, 0x120, 0);                                              // 006462E6
    st<U>(self, 0x124, 0);                                              // 006462EC
    st<U>(self, 0x114, 0);                                              // 006462F2
    st<U>(self, 0x168, 0);                                              // 006462F8
    st<U>(self, 0x188, 0);                                              // 006462FE
    st<U>(self, 0x10C, 0);                                              // 00646304
    st<U8>(self, 0x11C, 1);                                             // 0064630A
    st<U>(self, 0x160, kInvalidObject_00746644);                        // 00646318
    st<U>(self, 0x164, kInvalidObject_00746644);                        // 0064631E
    st<U>(self, 0x16C, 0);                                              // 00646324
    st<U>(self, 0x170, 0);                                              // 0064632A
    st<U>(self, 0x174, 0);                                              // 00646330
    st<U>(self, 0x178, 0);                                              // 00646336
    st<U8>(self, 0x17C, 0xFF);                                          // 00646340
    st<U8>(self, 0x11D, 0);                                             // 00646346
    st<U>(self, 0x194, 0);                                              // 0064634C
    st<U>(self, 0x198, 0);                                              // 00646352
    st<U>(self, 0x1A4, 0);                                              // 00646358
    st<U>(self, 0x19C, 0);                                              // 0064635E
    st<U>(self, 0x1A0, 0);                                              // 00646364
    return self;                                                        // 0064636B
}

// 00646380 NVThunk CSWCItem(CSWCObject)::AsSWItem: this - 10h.
void* __fastcall recovered_client_item_as_sw_item_00646380(void* object, void*) noexcept {
    return word_ptr(word_of(object) - 0x10);                            // 00646380 lea eax, [ecx-10h]
}

// 00646500 NVThunk CSWCItem(CSWCObject)::LoadModel(CResRef const* model, BYTE part, BYTE anim_type) RET 0Ch -> 1 / 0.
// this = CSWCObject sub-object. A missing anim base is created as a plain CSWCAnimBase.
U __fastcall recovered_client_item_load_model_00646500(void* object, void*, void* resref, U part, U) noexcept(false) {
    if (rp(object, 0x68) == nullptr) {                                  // 00646519..0064651E
        void* const memory = live_operator_new(0xF0);                   // 00646520..0064652A
        void* const anim = memory != nullptr ? kotor_m15e_anim_base_construct_0069dfb0(memory, nullptr) : nullptr;   // 00646531..00646546
        st<void*>(object, 0x68, anim);                                  // 00646552
        if (anim == nullptr) return 0;                                  // 00646548..00646566
    }
    void* anim = gob_get_property_0x68_0063c070(object, nullptr);       // 00646569..0064656C
    if (vslot<AnimLoadModel>(anim, 0x0C)(anim, nullptr, resref, part) == 0) return 0;   // 00646571..00646596 anim +0Ch
    anim = gob_get_property_0x68_0063c070(object, nullptr);             // 0064659A..0064659C
    void* const model = vslot<AnimGetModel>(anim, 0x08)(anim, nullptr, part);   // 006465A1..006465A9 anim +08h GetModel
    if (model != nullptr) {                                           // 006465AB..006465AD -> 006465C9
        vslot<GobPtr>(model, 0x74)(model, nullptr, word_ptr(word_of(object) - 0x10));   // 006465AF..006465B7 Gob +74h(item)
        vslot<GobU>(model, 0x7C)(model, nullptr, kItemType_00746635);       // 006465BA..006465C6 Gob +7Ch(object type)
    }
    vslot<GobU>(model, 0xC4)(model, nullptr, 5);                        // 006465C9..006465CF Gob +C4h(5)
    if (static_cast<U8>(part) == 0xFF) st<U>(object, 0xA4, kHalf_3f000000);   // 006465D5..006465DB
    return 1;                                                           // 006465EA
}

// 00646830 CSWCItem::ShowVisualEffect(): with an anim base, attach part FCh to the item model's "root".
void __fastcall recovered_client_item_show_visual_effect_00646830(void* item, void*) noexcept(false) {
    if (rp(item, 0x78) == nullptr) return;                              // 00646830..00646835 (object +68h)
    void* const object = at(item, 0x10);                                // 0064683C
    void* const effect = vslot<SlotGetModel>(object, 0x98)(object, nullptr, 0xFC, 1);   // 0064683F..00646848
    if (effect == nullptr) return;                                      // 00646850..00646852
    void* const model = vslot<SlotGetModel>(object, 0x98)(object, nullptr, 0xFF, 1);    // 00646854..0064685F
    if (model == nullptr) return;                                       // 00646865..00646867
    vslot<Gob3>(effect, 0x50)(effect, nullptr, word_of(model), word_of(kRoot_00740c00), 0);   // 00646869..00646875
}

// 00646930 NVThunk CSWCItem(CSWCObject)::SetArea(CSWCArea*) RET 4. this = CSWCObject sub-object.
void __fastcall recovered_client_item_set_area_00646930(void* object, void*, void* area) noexcept(false) {
    U moved;                                                            // esi
    if (area != nullptr) {                                              // 00646935..0064693B
        if (rp(object, 0x68) != nullptr) {                              // 0064693D..00646942
            void* const effect = vslot<SlotGetModel>(object, 0x98)(object, nullptr, 0xFC, 1);   // 00646944..0064694D
            if (effect != nullptr) {                                    // 00646953..00646957
                vslot<Gob3>(effect, 0x50)(effect, nullptr, 0, 0, 0);    // 00646959..00646963
                vslot<GobU>(effect, 0x4C)(effect, nullptr, 0);          // 00646966..0064696C
            }
        }
        moved = rp(object, 0x20) == area ? 0u : 1u;                     // 0064696F..00646979
    } else {
        recovered_client_item_show_visual_effect_00646830(word_ptr(word_of(object) - 0x10), nullptr);   // 0064697B..0064697E
        moved = 0;                                                      // 00646983
    }
    recovered_client_object_set_area_0063f570(object, nullptr, area);   // 00646985..00646988 CSWCObject::SetArea
    if (moved != 0)                                                     // 0064698D..0064698F
        vslot<SlotPos>(object, 0x8C)(object, nullptr, reinterpret_cast<const float*>(at(object, 0x24)));   // 00646991..00646999 SetPosition(+24h)
}

// 00646CD0 CSWCItem::~CSWCItem().
void __fastcall recovered_client_item_destruct_00646cd0(void* item, void*) noexcept(false) {
    void* const object = at(item, 0x10);                                // 00646CED
    st<const void*>(item, 0, recovered_client_item_methods_00751ba0);   // 00646CF4
    st<const void*>(object, 0, recovered_client_item_object_methods_00751a58);   // 00646CFA
    void* anim = rp(item, 0x78);                                        // 00646D00 (object +68h)
    if (anim != nullptr) vslot<U (__fastcall*)(void*, void*, U)>(anim, 0x74)(anim, nullptr, 0xFC);  // 00646D05..00646D18 anim +74h(FCh)
    anim = rp(object, 0x68);                                            // 00646D1B
    if (anim != nullptr) {                                              // 00646D1E..00646D20
        vslot<SlotVoid>(anim, 0x78)(anim, nullptr);                     // 00646D22..00646D24 anim +78h
        if (rp(object, 0x68) != nullptr) delete_object(rp(object, 0x68));   // 00646D27..00646D32 [vtbl+0](1)
        st<void*>(object, 0x68, nullptr);                               // 00646D34
    }
    if (rp(item, 0x124) != nullptr) {                                   // 00646D37..00646D3F
        live_free(rp(item, 0x124));                                     // 00646D41..00646D42
        st<void*>(item, 0x124, nullptr);                                // 00646D4A
    }
    void* const effect = rp(item, 0x194);                               // 00646D50
    if (effect != nullptr) {                                            // 00646D56..00646D58
        vslot<Gob3>(effect, 0x50)(effect, nullptr, 0, word_of(kEmpty_0073d71c), 0);   // 00646D5A..00646D63
        if (rp(item, 0x194) != nullptr) delete_object(rp(item, 0x194)); // 00646D66..00646D74
        st<void*>(item, 0x194, nullptr);                                // 00646D76
    }
    if (rp(item, 0x1A4) != nullptr) delete_object(rp(item, 0x1A4));     // 00646D7C..00646D8A
    st<void*>(item, 0x1A4, nullptr);                                    // 00646D92
    exo_free(at(item, 0x18C));                                          // 00646D8C..00646D9D
    kotor_m15e_object_destruct_0063e800(object, nullptr);               // 00646DA2..00646DA8
    recovered_item_base_destroy_005b4680(item, nullptr);                // 00646DAD..00646DB7
}
// 006474B0 CSWCItem scalar deleting destructor(flags) RET 4 -> this.
void* __fastcall recovered_client_item_scalar_destructor_006474b0(void* item, void*, U flags) noexcept(false) {
    recovered_client_item_destruct_00646cd0(item, nullptr);            // 006474B1..006474B3
    if ((flags & 1) != 0) live_free(item);                              // 006474B8..006474C0
    return item;                                                        // 006474C8
}
// 00646CC0 NVThunk CSWCItem(CSWCObject) destructor: this - 10h, tail jump 006474B0.
void* __fastcall recovered_client_item_object_destructor_thunk_00646cc0(void* object, void*, U flags) noexcept(false) {
    return recovered_client_item_scalar_destructor_006474b0(word_ptr(word_of(object) - 0x10), nullptr, flags);   // 00646CC0..00646CC3
}

// ============================================================================================ virtual tables
// Generated from the original .rdata slot words (lane-w3 tools: odis.rd). CSWCObject slots shared with the door /
// placeable tables point at the same runtime1115 bodies / D83 traps (lane-m15-e table 007516C8) and lane-w3 bodies;
// CSWCItem thunks absent from runtime1115 are reconstructed above or are explicit traps (w3_traps_player_model.cpp).
const void* const recovered_client_item_methods_00751ba0[1] = {
    reinterpret_cast<const void*>(&recovered_client_item_scalar_destructor_006474b0),                 // +000 006474B0 CSWCItem::~CSWCItem
};
const void* const recovered_sw_item_methods_0074b2a0[1] = {
    reinterpret_cast<const void*>(&recovered_sw_item_scalar_destructor_005b4770),                     // +000 005B4770 CSWItem::~CSWItem
};
const void* const recovered_client_item_object_methods_00751a58[82] = {
    reinterpret_cast<const void*>(&recovered_client_item_object_destructor_thunk_00646cc0),           // +000 00646CC0 NVThunkSWCItmSWCObject_destructor
    reinterpret_cast<const void*>(&cswc_object_set_id_00616fa0),                                       // +004 00616FA0 CSWCObject::SetId
    reinterpret_cast<const void*>(&kotor_m15e_slot008_void8_005b5e90),                                 // +008 005B5E90 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),                                  // +00C 00641DB0 self_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +010 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +014 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +018 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +01C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +020 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +024 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +028 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot02c_zero_0063e7f0),                                  // +02C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +030 0063E7F0 return_zero
    reinterpret_cast<const void*>(&recovered_client_item_as_sw_item_00646380),                         // +034 00646380 NVThunkItemGameObject_AsSWItem
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +038 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +03C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +040 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +044 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +048 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +04C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +050 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +054 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +058 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +05C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +060 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +064 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +068 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +06C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e730),                                          // +070 0063E730 CSWCObject::GetName
    reinterpret_cast<const void*>(&recovered_client_item_ai_update_00646390),                          // +074 00646390 NVThunkSWCItmSWCObject_AIUpdate
    reinterpret_cast<const void*>(&recovered_client_item_load_model_00646500),                         // +078 00646500 NVThunkSWCItmSWCObject_LoadModel
    reinterpret_cast<const void*>(&kotor_m15e_object_unload_model_00646650),                           // +07C 00646650 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_object_unload_model_006825f0),                     // +080 006825F0 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_item_set_area_00646930),                           // +084 00646930 NVThunkSWCItmSWCObject_SetArea
    reinterpret_cast<const void*>(&recovered_client_item_set_orientation_00646dd0),                    // +088 00646DD0 NVThunkSWCItmSWCObject_SetOrientation
    reinterpret_cast<const void*>(&recovered_client_item_set_position_00647030),                       // +08C 00647030 NVThunkSWCItmSWCObject_SetPosition
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +090 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_object_attach_to_scene_0063d820),                        // +094 0063D820 CSWCObject::AttachToScene
    reinterpret_cast<const void*>(&recovered_client_object_get_model_0063c080),                        // +098 0063C080 CSWCObject::GetModel
    reinterpret_cast<const void*>(&kotor_m15e_object_set_animation_0063e930),                          // +09C 0063E930 CSWCObject::SetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_object_add_fire_forget_animation_0063d8c0),              // +0A0 0063D8C0 CSWCObject::AddFireForgetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cb00),                                          // +0A4 0063CB00 CSWCObject::Hilite
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cb30),                                          // +0A8 0063CB30 CSWCObject::UnHilite
    reinterpret_cast<const void*>(&kotor_m15e_object_set_env_map_0063ccc0),                            // +0AC 0063CCC0 CSWCObject::SetEnvMap
    reinterpret_cast<const void*>(&recovered_client_object_set_env_map_0063cb70),                      // +0B0 0063CB70 CSWCObject::SetEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d970),                                          // +0B4 0063D970 CSWCObject::AddVisualEffect
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f400),                                          // +0B8 0063F400 CSWCObject::DeleteVisualEffect
    reinterpret_cast<const void*>(&kotor_m15e_object_kill_all_visual_effects_0063dae0),                // +0BC 0063DAE0 CSWCObject::KillAllVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063db20),                                          // +0C0 0063DB20 CSWCObject::RefreshVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f490),                                          // +0C4 0063F490 CSWCObject::UpdateVisualEffectsList
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063daa0),                                          // +0C8 0063DAA0 CSWCObject::StopAllVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063ddf0),                                          // +0CC 0063DDF0 CSWCObject::HasVisualEffectApplied
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e760),                                          // +0D0 0063E760 CSWCObject::GetCameraFollowPart
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),                                  // +0D4 0063E7A0 return_true
    reinterpret_cast<const void*>(&kotor_m15e_trap_00616fe0),                                          // +0D8 00616FE0 return_1.5
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063db90),                                          // +0DC 0063DB90 CSWCObject::ActivateHardCodedVisualWithHighestPriority
    reinterpret_cast<const void*>(&kotor_m15e_object_detach_visual_effects_0063e450),                  // +0E0 0063E450 CSWCObject::DetachVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_object_reattach_visual_effects_0063e4b0),                // +0E4 0063E4B0 CSWCObject::ReAttachVisualEffects
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +0E8 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063cfd0),                                          // +0EC 0063CFD0 CSWCObject::SetPortrait
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e770),                                          // +0F0 0063E770 CSWCObject::GetPortrait
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d140),                                          // +0F4 0063D140 CSWCObject::SetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e790),                                          // +0F8 0063E790 CSWCObject::GetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),                                  // +0FC 0063E7A0 return_true
    reinterpret_cast<const void*>(&kotor_m15e_object_remove_shadow_blob_0063d070),                     // +100 0063D070 CSWCObject::RemoveShadowBlob
    reinterpret_cast<const void*>(&recovered_client_object_show_shadow_blob_00616fb0),                 // +104 00616FB0 CSWCObject::ShowShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_object_hide_shadow_blob_00616fd0),                       // +108 00616FD0 CSWCObject::HideShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d0a0),                                          // +10C 0063D0A0 CSWCObject::SetGroundNormal
    reinterpret_cast<const void*>(&recovered_client_object_turn_on_shadows_0060b310),                  // +110 0060B310 CSWCObject::TurnOnShadows
    reinterpret_cast<const void*>(&recovered_client_object_turn_off_shadows_0060b320),                 // +114 0060B320 CSWCObject::TurnOffShadows
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),                                 // +118 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +11C 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7b0),                                          // +120 0063E7B0 CSWCObject::BarkString
    reinterpret_cast<const void*>(&kotor_m15e_trap_0060f560),                                          // +124 0060F560 CSWCObject::BarkBubble
    reinterpret_cast<const void*>(&kotor_m15e_slot128_false4_0063e7c0),                                // +128 0063E7C0 return_false_4
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7d0),                                          // +12C 0063E7D0 CSWCObject::ClientServerSyncHandler_Position
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063e7e0),                                          // +130 0063E7E0 CSWCObject::ClientServerSyncHandler_Orientation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d3f0),                                          // +134 0063D3F0 CSWCObject::ClientServerSyncHandler_Animation
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +138 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d560),                                          // +13C 0063D560 CSWCObject::GetReticlePoint
    reinterpret_cast<const void*>(&kotor_m15e_trap_00616fe0),                                          // +140 00616FE0 return_1.5
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d780),                                          // +144 0063D780 CSWCObject::GetStickyMouseThreshhold
};
}  // extern "C"
