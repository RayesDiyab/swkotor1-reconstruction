// lane-w3 unit w3-client-t3 (tier t3; W2 review: the client trapped on the first 5/1 message). Original: swkotor.exe
// GOG K1, read-only static disassembly (lane-w3 tools/odis.py, sdis.py); every statement cites the original
// instruction(s) it reproduces.
//   00668BB0 CSWCMessage::HandleServerToPlayerUpdate_PlayerInfo        ('I' record of GameObjectUpdate 5/1)
//   0066A1F0 CSWCMessage::HandleServerToPlayerUpdate_GuiElements       ('G' record) and its readers
//            0064FC10 GuiInventory  0064FDF0 GuiRepository  0064FEF0 GuiContainer  0064FB20 GuiQuickbarItemUseCount
//   0064E7B0 CSWCMessage::HandleServerToPlayerItemUpdate_Add           (item 'A' record; also GUI item records)
//   0064F4B0 CSWCMessage::HandleServerToPlayerTriggerUpdate_Add        (trigger 'A' record)
// PlayerInfo leaves without an existing owner (1122 map, lanes W1/W2/W4/W5 checked):
//   00617A70 CSWCCreature::DeleteSpellLikeAbilityList  00617A20 CreateNewSpellLikeAbilityList
//   00610BC0 CExoArrayList<CSWCStats_SpellLikeAbility>::Allocate  006475C0 CSWCStats_SpellLikeAbility()
//   00628650 CGuiInGame::SetLevelUpMode  00647590 CSWCLevelUpStats::setSkillRank  0064CE90 CSWCMessage::SetGuiSkillRank
//   0060E1D0 CSWCCreature::SetSpecialWalkAnimation  00649FE0 / 00649F90 CSWCCreatureStats::Remove/AddKnownSpell
//   004E3B20 CExoArrayList<WORD>::Remove  00649430 CSWCCreatureStats::SetFeatRemainingUses
//   0060C980 CSWCCreature::UpdateRelativePerception  00696CA0 CSWCCreatureAppearance::GetGenderChar
// Exception frames (SEH state bytes) only destroy CExoString temporaries; every temporary is destroyed explicitly on
// each normal path, as in the other lane-w3 units. Where the original dereferences a pointer it never checked (null
// creature on paths the server never produces), the body ends in an explicit D83 trap at that instruction instead.
#include "c75290da78-w3_client_t3.h"

using namespace kotor_w3;

namespace {
constexpr U8 kGameObjectOk_0074666c = 0;                              // .rdata 0074666C
constexpr U kInvalidObject_00746644 = 0x7F000000u;                    // .rdata 00746644
constexpr U kOne_3f800000 = 0x3F800000u;                              // ReadFLOAT scale immediate 1.0f
constexpr U kPerceptionEffect_7d1 = 0x7D1u;                           // 0060CA19 / 0060CA5E visual effect id
const char kEmpty_0073d71c[] = "";                                    // .rdata 0073D71C
const char kGender_0074b76c[] = "GENDER";                             // .rdata 0074B76C
const char kTrigger_00743754[] = "trigger";                            // .rdata 00743754 (gob class)
const char kCursorId_00754588[] = "CursorID";                         // .rdata 00754588
const char kModel_007499a0[] = "Model";                               // .rdata 007499A0
const char kMnFrag_00751cf0[] = "v_mnfrag";                           // .rdata 00751CF0
const char kEmpty_00751ce8[] = "EMPTY";                               // .rdata 00751CE8
constexpr U kTrigger_00746636 = 7;                                    // .rdata 00746636 object type byte
constexpr U kZOffset_3d4ccccd = 0x3D4CCCCDu;                          // .rdata 007415BC (0.05f)

U8 read_byte(void* m) noexcept { return kotor_fastcall_004d6720(m, nullptr, 8); }
U16 read_word(void* m) noexcept { return kotor_fastcall_004d6750(m, nullptr, 0x10); }
U read_dword(void* m) noexcept { return kotor_fastcall_004d6780(m, nullptr, 0x20); }
U read_bool(void* m) noexcept { return kotor_fastcall_004d66f0(m, nullptr); }
U read_id(void* m) noexcept { return kotor_fastcall_00692630(m, nullptr); }
float read_float(void* m) noexcept { return kotor_fastcall_004d67b0(m, nullptr, kOne_3f800000, 0x20); }
bool overflow(void* m) noexcept { return recovered_player_message::overrun_004d6230(m) != 0; }   // push 1; call 004D6230
void* rules() noexcept { return kotor_cell_rules_007a3a28; }                                     // [007A3A28]
U8 skill_count() noexcept { return ld<U8>(rules(), 0xAB); }                                      // [[007A3A28]+0ABh]
void* stats_of(void* creature) noexcept { return rp(creature, 0x2F8); }                          // CSWCCreature+2F8h
void exo_init(void* s) noexcept(false) {                              // 005B3190 CExoString()
    kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr);
}

// One "player state changed" feedback line (00668E60..00668ECB and the eleven copies after it):
// new CSWCCMessageData (34h) with the creature's id, empty text, CClientExoApp::AddFeedbackMessage(type, data, "").
void state_feedback(void* creature, U type) noexcept(false) {
    void* const memory = live_operator_new(0x34);                     // push 34h; call 006FA7E6
    void* const data = memory != nullptr ? recovered_message_data_construct_004d69f0(memory, nullptr) : nullptr;   // 004D69F0
    recovered_event_object_004d7780(data, nullptr, 0, rd32(creature, 4));   // push [edi+4]; push 0; call 004D7780
    ExoString text;                                                   // {E-0x30}
    frame_LogStringCtor_005e5a90(&text, nullptr, kEmpty_0073d71c);    // push 0073D71C; call 005E5A90
    recovered_client_add_feedback_message_005ed360(client_app(), nullptr, type, data, &text);   // push &s; push data; push type
    frame_LogStringDtor_005e5c20(&text, nullptr);                     // call 005E5C20
}
// Two-state player flag at creature+field: announce the transition 0 -> 1 with on_type and 1 -> 0 with off_type.
void state_flag(void* creature, U field, bool on, U on_type, U off_type) noexcept(false) {
    const U current = rd32(creature, field);
    if (on) {
        if (current == 0) state_feedback(creature, on_type);
        st<U>(creature, field, 1);
    } else {
        if (current == 1) state_feedback(creature, off_type);
        st<U>(creature, field, 0);
    }
}
}  // namespace

extern "C" {
volatile U recovered_client_show_all_triggers_0083285c = 0;           // [0083285C] (.data, never written by .text)

// ====================================================================================================== leaves
// 006475C0 CSWCStats_SpellLikeAbility::CSWCStats_SpellLikeAbility. ECX this, RET, EAX this.
void* __fastcall recovered_sla_construct_006475c0(void* self, void*) noexcept {
    st<U>(self, 0, 0);                                                // 006475C2
    st<U>(self, 4, 1);                                                // 006475C8
    st<U8>(self, 8, 0);                                               // 006475CF
    return self;                                                      // 006475C0 mov eax,ecx
}

// 00610BC0 CExoArrayList<CSWCStats_SpellLikeAbility>::Allocate(int capacity). 0Ch-byte elements. ECX list, RET 4.
void __fastcall recovered_sla_list_allocate_00610bc0(void* list, void*, U capacity) noexcept(false) {
    void* const old = rp(list, 0);                                    // 00610BE0 (ebx, saved {E-0x10})
    st<U>(list, 8, capacity);                                         // 00610BED
    unsigned char* const memory = static_cast<unsigned char*>(live_operator_new(capacity * 12u));   // 00610BE2..00610BF5
    unsigned char* fresh = nullptr;                                   // 00610C25 xor ebp,ebp
    if (memory != nullptr) {                                          // 00610BFE..00610C08
        for (I32 left = static_cast<I32>(capacity) - 1, i = 0; left >= 0; --left, ++i)   // 00610C0A..00610C1D
            recovered_sla_construct_006475c0(memory + i * 12, nullptr);   // 00610C12..00610C19
        fresh = memory;
    }
    st<void*>(list, 0, fresh);                                        // 00610C36
    for (I32 i = 0; i < static_cast<I32>(rd32(list, 4)); ++i)         // 00610C27..00610C60
        std::memcpy(fresh + i * 12, static_cast<unsigned char*>(old) + i * 12, 12);   // 00610C40..00610C54
    if (old != nullptr) live_free(old);                               // 00610C62..00610C6C
}

// 00617A20 CSWCCreature::CreateNewSpellLikeAbilityList: [this+3A0h] = new empty CExoArrayList (0Ch). RET.
void __fastcall recovered_client_creature_create_sla_list_00617a20(void* creature, void*) noexcept(false) {
    void* const list = live_operator_new(0xC);                        // 00617A36..00617A3A
    if (list != nullptr) {                                            // 00617A44..00617A46
        st<U>(list, 4, 0);                                            // 00617A48
        st<U>(list, 8, 0);                                            // 00617A4B
        st<U>(list, 0, 0);                                            // 00617A4E
    }
    st<void*>(creature, 0x3A0, list);                                 // 00617A58
}

// 00617A70 CSWCCreature::DeleteSpellLikeAbilityList. RET.
void __fastcall recovered_client_creature_delete_sla_list_00617a70(void* creature, void*) noexcept {
    void* list = rp(creature, 0x3A0);                                 // 00617A74
    if (list == nullptr) return;                                      // 00617A7A..00617A7C
    if (rp(list, 0) != nullptr) {                                     // 00617A7E..00617A82
        live_free(rp(list, 0));                                       // 00617A84..00617A8A
        st<U>(list, 0, 0);                                            // 00617A8D
        st<U>(list, 8, 0);                                            // 00617A93
    }
    st<U>(list, 4, 0);                                                // 00617A9A
    list = rp(creature, 0x3A0);                                       // 00617AA1
    if (list != nullptr) {                                            // 00617AA7..00617AA9
        live_free(rp(list, 0));                                       // 00617AAB..00617AAE free(NULL) (original)
        st<U>(list, 0, 0);                                            // 00617AB4
        live_free(list);                                              // 00617AB3..00617ABA
    }
    st<U>(creature, 0x3A0, 0);                                        // 00617AC2
}

// 00628650 CGuiInGame::SetLevelUpMode(BOOL). RET 4.
void __fastcall recovered_gui_in_game_set_level_up_mode_00628650(void* gui, void*, U mode) noexcept {
    if (rd32(gui, 0x10C) != mode) st<U>(gui, 0x10C, mode);            // 00628654..0062865C
}

// 00647590 CSWCLevelUpStats::setSkillRank(WORD skill, BYTE rank). RET 8.
void __fastcall recovered_level_up_stats_set_skill_rank_00647590(void* stats, void*, U skill, U rank) noexcept {
    const U16 index = static_cast<U16>(skill);                        // 0064759D mov ax,[esp+4]
    if (index < static_cast<U16>(skill_count()))                      // 00647590..006475A5
        st<U8>(rp(stats, 0x88), index, static_cast<U8>(rank));        // 006475A7..006475B4
}

// 0064CE90 CSWCMessage::SetGuiSkillRank(CSWCCreature*, WORD value, WORD mask, WORD skill). RET 10h.
// Bit set: a skill rank <= 0 becomes 1; bit clear: a rank > 0 becomes 0 (signed BYTE ranks at [[c+2F8h]+88h]).
void __fastcall recovered_client_set_gui_skill_rank_0064ce90(void* /*self*/, void*, void* creature, U value, U mask,
                                                             U skill) noexcept {
    const U16 index = static_cast<U16>(skill);                        // 0064CE9B mov ax,[esp+10h]
    const U16 count = static_cast<U16>(skill_count());                // 0064CEA2..0064CEA8 / 0064CEF0..0064CEF6
    if (static_cast<U16>(value & mask) != 0) {                        // 0064CE90..0064CEA0
        if (index >= count) return;                                   // 0064CEB0..0064CEB7
        if (static_cast<std::int8_t>(ld<U8>(rp(stats_of(creature), 0x88), index)) > 0) return;   // 0064CEBB..0064CED3
        st<U8>(rp(stats_of(creature), 0x88), index, 1);               // 0064CEDA..0064CEE9
        return;
    }
    if (index >= count) return;                                       // 0064CEFE..0064CF01
    unsigned char* const rank = static_cast<unsigned char*>(rp(stats_of(creature), 0x88)) + index;   // 0064CF03..0064CF19
    if (static_cast<std::int8_t>(*rank) > 0) *rank = 0;               // 0064CF1B..0064CF1F
}

// 0060E1D0 CSWCCreature::SetSpecialWalkAnimation(BYTE): [GetProperty0x68()+0C8h] = arg. RET 4.
void __fastcall recovered_client_creature_set_special_walk_animation_0060e1d0(void* creature, void*, U on) noexcept(false) {
    st<U8>(gob_get_property_0x68_0063c070(creature, nullptr), 0xC8, static_cast<U8>(on));   // 0060E1D0..0060E1D9
}

// 00649FE0 CSWCCreatureStats::RemoveKnownSpell(BYTE class, DWORD spell). RET 8.
void __fastcall recovered_creature_stats_remove_known_spell_00649fe0(void* stats, void*, U class_index, U spell) noexcept(false) {
    const U8 cls = static_cast<U8>(class_index);                      // 00649FE1 mov bl,[esp+8]
    if (cls >= 2) return;                                             // 00649FE5..00649FEB
    if (kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8C), nullptr, spell) == nullptr) return;   // 00649FED..0064A005
    void* const list = at(stats, 0xD4 + static_cast<U>(cls) * 0x20); // 0064A007..0064A014
    const I32 count = static_cast<I32>(rd32(list, 4));                // 0064A00D
    if (count <= 0) return;                                           // 0064A01D..0064A01F
    for (I32 i = 0; i < count; ++i) {                                 // 0064A021..0064A02D
        if (ld<U>(rp(list, 0), static_cast<U>(i) * 4) == spell) {     // 0064A023..0064A025
            recovered_int_list_remove_0066dcc0(list, nullptr, spell); // 0064A035..0064A03A (index >= 0)
            return;
        }
    }
}

// 00649F90 CSWCCreatureStats::AddKnownSpell(BYTE class, DWORD spell). RET 8.
void __fastcall recovered_creature_stats_add_known_spell_00649f90(void* stats, void*, U class_index, U spell) noexcept(false) {
    const U8 cls = static_cast<U8>(class_index);                      // 00649F91
    if (cls >= 2) return;                                             // 00649F95..00649F9B
    if (kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8C), nullptr, spell) == nullptr) return;   // 00649F9D..00649FB5
    recovered_known_spell_add_00649660(at(stats, 0xD4 + static_cast<U>(cls) * 0x20), nullptr, 0, spell);   // 00649FB7..00649FC7
}

// 004E3B20 CExoArrayList<unsigned short>::Remove(WORD). The count is decremented even when the value is absent
// (original). RET 4.
void __fastcall recovered_u16_list_remove_004e3b20(void* list, void*, U value) noexcept {
    const I32 count = static_cast<I32>(rd32(list, 4));                // 004E3B21 (esi)
    I32 i = 0;                                                        // 004E3B24
    if (count > 0) {                                                  // 004E3B26..004E3B28
        const U16 v = static_cast<U16>(value);                        // 004E3B2E
        while (ld<U16>(rp(list, 0), static_cast<U>(i) * 2) != v) {    // 004E3B33..004E3B36
            ++i;                                                      // 004E3B3B
            if (i >= static_cast<I32>(rd32(list, 4))) break;          // 004E3B38..004E3B41
        }
    }
    st<U>(list, 4, static_cast<U>(count - 1));                        // 004E3B45..004E3B4A
    for (; i < static_cast<I32>(rd32(list, 4)); ++i)                  // 004E3B48..004E3B63
        st<U16>(rp(list, 0), static_cast<U>(i) * 2, ld<U16>(rp(list, 0), static_cast<U>(i) * 2 + 2));   // 004E3B50..004E3B5A
}

// 00649430 CSWCCreatureStats::SetFeatRemainingUses(WORD feat, BYTE uses). List of 4-byte {WORD feat, BYTE uses}
// records at +0C8h data / +0CCh count / +0D0h capacity. RET 8.
void __fastcall recovered_creature_stats_set_feat_remaining_uses_00649430(void* stats, void*, U feat, U uses) noexcept(false) {
    const U16 id = static_cast<U16>(feat);                            // 00649440 mov di,[esp+14h]
    const I32 count = static_cast<I32>(rd32(stats, 0xCC));            // 00649435
    for (I32 i = 0; i < count; ++i) {                                 // 0064943D..0064945E
        void* const entry = rp(rp(stats, 0xC8), static_cast<U>(i) * 4);   // 00649447..00649450
        if (ld<U16>(entry, 0) == id) {                                // 00649452..00649456
            st<U8>(entry, 2, static_cast<U8>(uses));                  // 0064948E..00649498
            return;
        }
    }
    void* const entry = live_operator_new(4);                         // 00649460..00649462
    st<U16>(entry, 0, id);                                            // 0064946D (no null check, original)
    st<U8>(entry, 2, static_cast<U8>(uses));                          // 00649470
    if (rd32(stats, 0xCC) == rd32(stats, 0xD0)) {                     // 00649473..00649484
        const U capacity = rd32(stats, 0xD0) != 0 ? rd32(stats, 0xD0) * 2 : 0x10;   // 00649486..006494A4
        void* const old = rp(stats, 0xC8);                            // 006494A4 (edi)
        st<U>(stats, 0xD0, capacity);                                 // 006494AA
        st<void*>(stats, 0xC8, live_operator_new(capacity * 4));      // 006494B0..006494BF
        for (I32 i = 0; i < static_cast<I32>(rd32(stats, 0xCC)); ++i) // 006494C8..006494E5
            st<U>(rp(stats, 0xC8), static_cast<U>(i) * 4, ld<U>(old, static_cast<U>(i) * 4));   // 006494D0..006494D9
        if (old != nullptr) live_free(old);                           // 006494E7..006494EC
    }
    st<void*>(rp(stats, 0xC8), rd32(stats, 0xCC) * 4, entry);         // 006494F4..00649500
    st<U>(stats, 0xCC, rd32(stats, 0xCC) + 1);                        // 00649503..0064950B
}

// 0060C980 CSWCCreature::UpdateRelativePerception(CSWCVisibilityNode* entry {OBJECT_ID id; BYTE flags}). RET 4.
// Flags bit0 seen, bit1 heard, bit4 (third BOOL): heard-only or bit4 shows the 7D1h perception effect on the other
// creature; seen without bit4 removes it.
void __fastcall recovered_client_creature_update_relative_perception_0060c980(void* creature, void*, void* entry) noexcept(false) {
    U show = 0;                                                       // 0060C998 (ebx)
    U hide = 0;                                                       // 0060C99B {E-0x10}
    void* const other = word_ptr(frame_R4_005ed5b0(client_app(), nullptr, rd32(entry, 0)));   // 0060C987..0060C99F
    if (other == nullptr) return;                                     // 0060C9A6..0060C9A8
    const U8 flags = ld<U8>(entry, 4);                                // 0060C9AE
    const U8 seen = flags & 1;                                        // 0060C9B1..0060C9B3 (cl)
    if (seen == 0 && (flags & 2) != 0) {                              // 0060C9B6..0060C9BA
        show = 1;                                                     // 0060C9C3
    } else if (((flags >> 4) & 1) != 0) {                             // 0060C9BC..0060C9C1
        show = 1;                                                     // 0060C9C3
    } else if (seen == 1) {                                           // 0060C9CA..0060C9D1
        hide = 1;                                                     // 0060C9D3
    }
    if (sound_volume29_required::predicate_006054e0(rp(creature, 0x20), nullptr, at(other, 0x24)) != 1) return;   // 0060C9DB..0060C9EE
    if (show == 1) {                                                  // 0060C9F4..0060C9F6
        if (rd32(other, 0x1B0) != 0) return;                          // 0060C9F8..0060CA00 (-> 0060CA4E, hide == 0)
        Vector3 zero = {0.0f, 0.0f, 0.0f};                            // {E-0xc}
        using Add = U (__fastcall*)(void*, void*, U, U, U, U, U, U, U);
        vslot<Add>(other, 0xB4)(other, nullptr, kPerceptionEffect_7d1, 0, kInvalidObject_00746644,
                                kInvalidObject_00746644, 0, 0, word_of(&zero));   // 0060CA02..0060CA38 AddVisualEffect
        st<U>(other, 0x1B0, 1);                                       // 0060CA3E
        return;
    }
    if (hide == 1 && rd32(other, 0x1B0) == 1) {                       // 0060CA4E..0060CA5A
        vslot<U (__fastcall*)(void*, void*, U)>(other, 0xB8)(other, nullptr, kPerceptionEffect_7d1);   // 0060CA5C..0060CA65
        st<U>(other, 0x1B0, 0);                                       // 0060CA6B
    }
}

// 00696CA0 CSWCCreatureAppearance::GetGenderChar: first character of gender.2da[BYTE this+1Bh].GENDER. RET, AL.
std::uint8_t __fastcall recovered_client_creature_appearance_gender_char_00696ca0(void* appearance, void*) noexcept(false) {
    ExoString value;                                                  // {E-0x1c}
    exo_init(&value);                                                 // 00696CBC..00696CC0
    ExoString column;                                                 // {E-0x14}
    frame_LogStringCtor_005e5a90(&column, nullptr, kGender_0074b76c); // 00696CC7..00696CD4
    void* const table = rp(rp(rules(), 0xB8), 0x10);                  // 00696CD9..00696CE4 [[[rules]+B8h]+10h]
    (void)c2da_current_get_string_00413270(table, nullptr, ld<U8>(appearance, 0x1B), &column, &value);   // 00696CE7..00696CFD
    frame_LogStringDtor_005e5c20(&column, nullptr);                   // 00696D02..00696D0A
    const U8 c = static_cast<U8>(c2da_current_string_index_005e5600(&value, nullptr, 0));   // 00696D0F..00696D1D
    frame_LogStringDtor_005e5c20(&value, nullptr);                    // 00696D19..00696D27
    return c;                                                         // 00696D31
}

// =========================================================================================== 'I' PlayerInfo
// 00668BB0 CSWCMessage::HandleServerToPlayerUpdate_PlayerInfo. Record (after the 'I' letter): OBJECTID creature,
// DWORD flags, then sections in this order: 1000h (clear), 1, 2, 8, 80h, 10h, 20h, 40h, 4, 100h, 2000h, 800h, 8000h,
// 10000h, 200h, 20000h, 4000h (layout per section below; W2 writer 00572DC0). ECX this, RET; EAX 1, or 0 on a read
// overflow (or a 2000h section without a creature). Locals by the original esp+X slot at depth 64h.
U __fastcall recovered_client_handle_update_player_info_00668bb0(void* self, void*) {
    const U id = read_id(self);                                       // 00668BD2..00668BDF {esp+3Ch}
    const U flags = read_dword(self);                                 // 00668BDB..00668BEC {esp+20h}
    if (overflow(self)) return 0;                                     // 00668BE8..00668BF7
    void* const creature = word_ptr(frame_R4_005ed5b0(client_app(), nullptr, id));   // 00668BFD..00668C0B (edi)
    void* const stats = creature != nullptr ? stats_of(creature) : nullptr;   // 00668C0D..00668C1D {esp+24h}

    if ((flags & 0x1000) != 0) {                                      // 00668C21..00668C28 (test ah,10h)
        kotor_m15b_creature_stats_clear_feats_00648320(stats, nullptr);          // 00668C2A..00668C2C
        kotor_m15b_creature_stats_clear_known_spells_00649520(stats, nullptr);   // 00668C31..00668C33
        recovered_client_creature_delete_sla_list_00617a70(creature, nullptr);   // 00668C38..00668C3A
        kotor_m15e_creature_clear_visibility_list_006145d0(creature, nullptr);   // 00668C3F..00668C45
        (void)frame_R4_005ed690(client_app(), nullptr);               // 00668C4A..00668C53 GetInGameGui (unused)
    }

    if ((flags & 1) != 0) {                                           // 00668C58..00668C5D
        const U16 armour = read_word(self);                           // 00668C63..00668C70 (bp)
        const U gold = read_dword(self);                              // 00668C6C..00668C7B (ebx)
        const U field_1b4 = read_dword(self);                         // 00668C77..00668C84 {esp+30h}
        const U level_up = read_bool(self);                           // 00668C82..00668C91 {esp+28h}
        const U experience = read_dword(self);                        // 00668C8D..00668C9E {esp+34h}
        if (overflow(self)) return 0;                                 // 00668C9A..00668CA9
        if (creature != nullptr) {                                    // 00668CAF..00668CB1
            st<U>(stats_of(creature), 0x58, experience);              // 00668CB3..00668CBD
            st<U16>(stats_of(creature), 0x50, armour);                // 00668CC0..00668CC6
            st<U>(creature, 0x144, gold);                             // 00668CCE
            st<U>(creature, 0x1B4, field_1b4);                        // 00668CD4
        }
        (void)kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr);   // 00668CDA..00668CE3 (unused)
        void* const gui = frame_R4_005ed690(client_app(), nullptr);   // 00668CE8..00668CF1
        if (gui != nullptr) recovered_gui_in_game_set_level_up_mode_00628650(gui, nullptr, level_up);   // 00668CF6..00668D01
        if (level_up == 1) {                                          // 00668D06..00668D0B
            const U16 points = read_word(self);                       // 00668D11..00668D1E (ebx)
            if (overflow(self)) return 0;                             // 00668D1A..00668D27
            if (creature != nullptr) st<U16>(stats_of(creature), 0xAE, points);   // 00668D2D..00668D37
            U skill = 0;                                              // 00668D4A (ebx)
            if (skill_count() > 0) {                                  // 00668D3E..00668D4E
                do {
                    const U8 rank = read_byte(self);                  // 00668D50..00668D5D {esp+34h} low byte
                    if (overflow(self)) return 0;                     // 00668D59..00668D68
                    if (creature != nullptr)                          // 00668D6E..00668D70
                        recovered_level_up_stats_set_skill_rank_00647590(stats_of(creature), nullptr, skill, rank);   // 00668D72..00668D7E
                    ++skill;                                          // 00668D91
                } while (static_cast<U16>(skill) < static_cast<U16>(skill_count()));   // 00668D83..00668D95
            }
            const U8 feats = read_byte(self);                         // 00668D97..00668DA4 (bl)
            (void)read_byte(self);                                    // 00668DA0..00668DA6 (discarded)
            if (overflow(self)) return 0;                             // 00668DAB..00668DB6
            if (creature != nullptr) st<U8>(stats_of(creature), 0x33, feats);   // 00668DBC..00668DC6
        }
    }

    if ((flags & 2) != 0) {                                           // 00668DC9..00668DCE
        const U16 skills = read_word(self);                           // 00668DD0..00668DDD (bp)
        if (overflow(self)) return 0;                                 // 00668DD9..00668DE6
        if (creature != nullptr) {                                    // 00668DEC..00668DEE
            recovered_client_set_gui_skill_rank_0064ce90(self, nullptr, creature, skills, 2, 1);      // 00668DF0..00668DF8
            recovered_client_set_gui_skill_rank_0064ce90(self, nullptr, creature, skills, 0x20, 6);   // 00668DFD..00668E05
            recovered_client_set_gui_skill_rank_0064ce90(self, nullptr, creature, skills, 0x10, 2);   // 00668E0A..00668E12
        }
    }

    if ((flags & 8) != 0) {                                           // 00668E17..00668E1C
        const U16 state = read_word(self);                            // 00668E22..00668E2F (ebx, {esp+28h})
        if (overflow(self)) return 0;                                 // 00668E2B..00668E3C
        if (creature != nullptr) {                                    // 00668E42..00668E44
            state_flag(creature, 0x190, (state & 1) != 0, 0x53, 0x54);      // 00668E4A..00668F57
            state_flag(creature, 0x198, (state & 4) != 0, 0x57, 0x58);      // 00668F61..0066906A
            state_flag(creature, 0x19C, (state & 8) != 0, 0x59, 0x5A);      // 00669074..0066917D
            state_flag(creature, 0x1A0, (state & 0x10) != 0, 0x5B, 0x5C);   // 00669187..00669290
            state_flag(creature, 0x1A8, (state & 0x40) != 0, 0xA6, 0xA7);   // 0066929A..006693A9
            state_flag(creature, 0x1AC, (state & 0x20) != 0, 0xA8, 0xA9);   // 006693B3..006694C2
            st<U>(creature, 0x1B8, (static_cast<U>(state) >> 7) & 1);   // 006694CC..006694DE
            recovered_client_creature_set_special_walk_animation_0060e1d0(creature, nullptr,
                                                                          (state & 0x100) != 0 ? 1u : 0u);   // 006694DB..006694EE
            st<U>(creature, 0x370, (static_cast<U>(state) >> 10) & 1);   // 006694F3..00669506
            st<U>(creature, 0x374, (static_cast<U>(state) >> 11) & 1);   // 0066950C
            st<U>(creature, 0x378, (static_cast<U>(state) >> 12) & 1);   // 00669512..00669515
            (void)frame_R4_005ed690(client_app(), nullptr);           // 0066951B..00669524 (unused)
            (void)frame_R4_005ed690(client_app(), nullptr);           // 00669529..00669532 (unused)
        }
    }

    if ((flags & 0x80) != 0) {                                        // 00669537..0066953D (test al,al; jns)
        const U8 count = read_byte(self);                             // 0066953F..00669548 (bl)
        for (U8 i = 0; i < count; ++i) {                              // 0066954C..00669582 {esp+12h}
            (void)read_byte(self);                                    // 00669553..00669557
            (void)read_word(self);                                    // 0066955C..00669560
            if (overflow(self)) return 0;                             // 00669565..00669570
        }
        if (frame_R4_005ed690(client_app(), nullptr) != nullptr)      // 00669584..00669593
            (void)kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr);   // 00669595..0066959E (unused)
    }

    if ((flags & 0x10) != 0) {                                        // 006695A3..006695A8
        for (U8 cls = 0; cls < 2; ++cls) {                            // 006695AE / 0066961B..00669627 {esp+28h} low byte
            for (U16 n = read_word(self); n != 0; --n) {              // 006695B3..006695C8 / 006695E8..006695E9
                const U spell = read_dword(self);                     // 006695D0..006695D4
                if (creature != nullptr)                              // 006695D9..006695DB
                    recovered_creature_stats_remove_known_spell_00649fe0(stats, nullptr, cls, spell);   // 006695DD..006695E3
            }
            for (U16 n = read_word(self); n != 0; --n) {              // 006695EB..006695F9 / 00669618..00669619
                const U spell = read_dword(self);                     // 00669600..00669604
                if (creature != nullptr)                              // 00669609..0066960B
                    recovered_creature_stats_add_known_spell_00649f90(stats, nullptr, cls, spell);   // 0066960D..00669613
            }
        }
        if (frame_R4_005ed690(client_app(), nullptr) != nullptr)      // 00669629..00669639
            (void)kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr);   // 0066963B..00669643 (unused)
    }

    if ((flags & 0x20) != 0) {                                        // 00669648..0066964D
        for (U8 pass = 0; pass < 2; ++pass) {                         // 00669653 / 006696F1..006696F6 (bl)
            for (U16 n = read_word(self); n != 0; --n) {              // 00669655..00669663 / 00669688..00669691
                (void)read_byte(self);                                // 00669665..00669669
                (void)read_byte(self);                                // 0066966E..00669672
                if (overflow(self)) return 0;                         // 00669677..00669682
            }
            for (U16 n = read_word(self); n != 0; --n) {              // 00669693..006696A1 / 006696E6..006696EF
                (void)read_dword(self);                               // 006696A3..006696A7
                (void)read_byte(self);                                // 006696AC..006696B0
                (void)read_byte(self);                                // 006696B5..006696B9
                (void)read_bool(self);                                // 006696BE..006696C0
                (void)read_bool(self);                                // 006696C5..006696C7
                (void)read_byte(self);                                // 006696CC..006696D0
                if (overflow(self)) return 0;                         // 006696D5..006696E0
            }
        }
    }

    if ((flags & 0x40) != 0) {                                        // 006696FC..00669701
        const U8 count = read_byte(self);                             // 00669707..00669716 (bl, {esp+13h})
        if (overflow(self)) return 0;                                 // 00669712..00669721
        for (U8 i = 0; i < count; ++i) {                              // 00669727..0066979E {esp+12h} (starts at the overflow result 0)
            (void)read_byte(self);                                    // 00669730..00669734
            const U16 mask = read_word(self);                         // 00669739..00669748 (bp)
            if (overflow(self)) return 0;                             // 00669742..0066974F
            for (U8 bit = 0; bit < 1; ++bit) {                        // 00669753..0066978C
                if ((static_cast<U>(mask) & (1u << bit)) != 0) {      // 00669760..0066976B
                    (void)read_byte(self);                            // 0066976D..00669771
                    if (overflow(self)) return 0;                     // 00669776..00669781
                }
            }
        }
    }

    if ((flags & 4) != 0) {                                           // 006697A0..006697A5
        const U16 ops = read_word(self);                              // 006697AB..006697B8 (bp)
        if (overflow(self)) return 0;                                 // 006697B4..006697C1
        for (I32 i = 0; i < static_cast<I32>(ops); ++i) {             // 006697C7..00669875 {esp+2Ch} / {esp+28h}
            const U8 op = read_byte(self);                            // 006697E0..006697ED (bl)
            const U16 feat = read_word(self);                         // 006697E9..006697F8 (bp)
            if (overflow(self)) return 0;                             // 006697F4..00669801
            if (op == 0x42) {                                         // 00669807..0066980A 'B' bonus feat add
                if (creature != nullptr) kotor_m15_array_u16_add_unique_005a7670(at(stats, 0xBC), nullptr, feat);   // 0066980C..0066981B
            } else if (op == 0x58) {                                  // 00669822..00669825 'X' bonus feat remove
                if (creature != nullptr) recovered_u16_list_remove_004e3b20(at(stats, 0xBC), nullptr, feat);   // 00669827..00669861
            } else if (op == 0x41) {                                  // 00669837..0066983A 'A' feat add
                if (creature != nullptr) kotor_m15_array_u16_add_unique_005a7670(at(stats, 0xB0), nullptr, feat);   // 0066983C..0066984B
            } else {                                                  // 'D' (any other letter) feat remove
                if (creature != nullptr) recovered_u16_list_remove_004e3b20(at(stats, 0xB0), nullptr, feat);   // 00669852..00669861
            }
        }
        const U16 uses = read_word(self);                             // 0066987B..00669888 (bp)
        if (overflow(self)) return 0;                                 // 00669884..00669891
        for (I32 i = 0; i < static_cast<I32>(uses); ++i) {            // 00669897..006698E7 (ebx)
            const U16 feat = read_word(self);                         // 006698A4..006698B1 (bp)
            const U8 left = read_byte(self);                          // 006698AD..006698BC {esp+34h} low byte
            if (overflow(self)) return 0;                             // 006698B8..006698C7
            if (creature != nullptr)                                  // 006698CD..006698CF
                recovered_creature_stats_set_feat_remaining_uses_00649430(stats, nullptr, feat, left);   // 006698D1..006698DB
        }
    }

    if ((flags & 0x100) != 0) {                                       // 006698E9..006698F0 (test ah,1)
        const U8 count = read_byte(self);                             // 006698F6..00669903 (bl)
        if (overflow(self)) return 0;                                 // 006698FF..0066990C
        for (I32 i = 0; i < static_cast<I32>(count); ++i) {           // 0066990E..00669A68 {esp+2Ch}
            const U8 op = read_byte(self);                            // 00669920..00669924
            if (op == 0x44) {                                         // 00669929..0066992B 'D'
                (void)read_word(self);                                // 0066992D..00669931
                (void)read_word(self);                                // 00669936..0066993A
                if (overflow(self)) return 0;                         // 0066993F..0066994A
            } else if (op == 0x53) {                                  // 00669965..00669967 'S'
                (void)read_word(self);                                // 00669969..0066996D
                (void)read_word(self);                                // 00669972..00669976
                (void)read_id(self);                                  // 0066997B..0066997D
                if (overflow(self)) return 0;                         // 00669982 -> 0066993F..0066994A
            } else if (op == 0x41) {                                  // 00669984..00669986 'A'
                const U16 kind = read_word(self);                     // 0066998C..00669999 (bp)
                (void)read_word(self);                                // 00669995..0066999B
                bool point = kind == 0;                               // 006699A0..006699A3
                if (!point) {
                    if (kind != 2) (void)read_id(self);               // 006699A5..006699AD
                    point = kind == 4 || kind == 0x13 || kind == 2 || kind == 0xC;   // 006699B2..006699C8
                }
                if (point) {                                          // 006699CA..006699F8 three FLOATs (discarded)
                    (void)read_float(self);
                    (void)read_float(self);
                    (void)read_float(self);
                }
                if (kind == 4 || kind == 0x13) {                      // 006699FA..006699FE / 00669A1D..00669A21
                    const U spell = read_dword(self);                 // 00669A00..00669A04 / 00669A23..00669A27
                    (void)kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8C), nullptr, spell);   // 00669A09..00669A16 (unused)
                }
            } else if (op == 0x55) {                                  // 00669A40..00669A42 'U'
                (void)read_word(self);                                // 00669A44..00669A48
                (void)read_word(self);                                // 00669A4D..00669A51
                (void)read_id(self);                                  // 00669A56..00669A58
            }
        }
    }

    if ((flags & 0x2000) != 0) {                                      // 00669A6E..00669A75 (test ah,20h)
        if (creature == nullptr) return 0;                            // 00669A7B..00669A7D
        const U removed = read_dword(self);                           // 00669A83..00669A8E (ebx)
        if (overflow(self)) return 0;                                 // 00669A8E..00669A9D
        for (I32 i = 0; i < static_cast<I32>(removed); ++i) {         // 00669AA3..00669B36
            const U gone = read_id(self);                             // 00669AB0..00669AB2 (ebx)
            if (overflow(self)) return 0;                             // 00669AB7..00669AC4
            const I32 last = static_cast<I32>(rd32(creature, 0x364)) - 1;   // 00669ACA..00669AD0 (ebp)
            for (I32 j = last; j >= 0; --j) {                         // 00669AD3..00669AEE
                void* const node = rp(rp(creature, 0x360), static_cast<U>(j) * 4);   // 00669AD9..00669AE2 (edx)
                if (rd32(node, 0) != gone) continue;                  // 00669AE4..00669AE6
                st<U>(creature, 0x364, static_cast<U>(last));         // 00669AF4
                for (I32 k = j; k < static_cast<I32>(rd32(creature, 0x364)); ++k)   // 00669AF2..00669B18
                    st<U>(rp(creature, 0x360), static_cast<U>(k) * 4, ld<U>(rp(creature, 0x360), static_cast<U>(k) * 4 + 4));   // 00669B00..00669B0D
                if (node != nullptr) live_free(node);                 // 00669B1A..00669B24
                break;
            }
        }
        const U added = read_dword(self);                             // 00669B3C..00669B4B (ebx)
        if (overflow(self)) return 0;                                 // 00669B47..00669B56
        for (I32 i = 0; i < static_cast<I32>(added); ++i) {           // 00669B5C..00669D11
            const U seen_id = read_id(self);                          // 00669B68..00669B71 {esp+2Ch}
            const U b1 = read_bool(self);                             // 00669B6F..00669B7C {esp+30h}
            const U b2 = read_bool(self);                             // 00669B7A..00669B87 {esp+34h}
            const U b3 = read_bool(self);                             // 00669B85..00669B94 (ebx)
            if (overflow(self)) return 0;                             // 00669B90..00669B9D
            void* node = nullptr;                                     // ebp
            const I32 count = static_cast<I32>(rd32(creature, 0x364));
            for (I32 c = 0; c < count; ++c) {                         // 00669BA3..00669BD0
                void* const candidate = rp(rp(creature, 0x360), static_cast<U>(c) * 4);   // 00669BAF..00669BB5
                if (rd32(candidate, 0) == seen_id) { node = candidate; break; }   // 00669BB7..00669BBE
            }
            if (node != nullptr) {                                    // 00669CC3 (found)
                U8 bits;
                if (b1 == 1) {                                        // 00669CC3..00669CCA
                    bits = static_cast<U8>(ld<U8>(node, 4) | 3);      // 00669CCC..00669CCF
                } else {
                    bits = static_cast<U8>(((static_cast<U8>(b1) & 1) << 1) | (static_cast<U8>(b2) & 1) |
                                           (ld<U8>(node, 4) & 0xFC));   // 00669CD3..00669CE6
                }
                st<U8>(node, 4, bits);                                // 00669CEF
                st<U8>(node, 4, static_cast<U8>(bits ^ ((static_cast<U8>(b3 << 4) ^ bits) & 0x10)));   // 00669CE8..00669CF7
            } else {
                void* const memory = live_operator_new(8);            // 00669BD2..00669BD9
                if (memory != nullptr) {                              // 00669BDC..00669BDE
                    st<U8>(memory, 4, static_cast<U8>(ld<U8>(memory, 4) & 0xE0));   // 00669BE0..00669BEC
                    st<U>(memory, 0, kInvalidObject_00746644);        // 00669BE6
                }
                node = memory;                                        // 00669BEF / 00669BF3
                st<U>(node, 0, seen_id);                              // 00669BF5..00669BF9
                if (b1 == 1) {                                        // 00669BFC..00669C01
                    st<U8>(node, 4, static_cast<U8>(ld<U8>(node, 4) | 3));   // 00669C03
                } else {
                    const U8 cl = ld<U8>(node, 4);                    // 00669C0D
                    st<U8>(node, 4, static_cast<U8>(cl ^ ((static_cast<U8>(static_cast<U8>(b2) << 1) ^ cl) & 2)));   // 00669C09..00669C18
                }
                const U8 cl = ld<U8>(node, 4);                        // 00669C1B
                st<U8>(node, 4, static_cast<U8>(cl ^ ((static_cast<U8>(static_cast<U8>(b3) << 4) ^ cl) & 0x10)));   // 00669C1E..00669C2A
                if (rd32(creature, 0x364) == rd32(creature, 0x368)) { // 00669C2D..00669C39
                    const U capacity = rd32(creature, 0x368) != 0 ? rd32(creature, 0x368) * 2 : 0x10;   // 00669C3B..00669C43
                    void* const old = rp(creature, 0x360);            // 00669C48 ({esp+2Ch})
                    st<U>(creature, 0x368, capacity);                 // 00669C5A
                    st<void*>(creature, 0x360, live_operator_new(capacity * 4));   // 00669C4E..00669C6B
                    for (I32 c = 0; c < static_cast<I32>(rd32(creature, 0x364)); ++c)   // 00669C74..00669C99
                        st<U>(rp(creature, 0x360), static_cast<U>(c) * 4, ld<U>(old, static_cast<U>(c) * 4));   // 00669C80..00669C8D
                    if (old != nullptr) live_free(old);               // 00669C9B..00669CA4
                }
                st<void*>(rp(creature, 0x360), rd32(creature, 0x364) * 4, node);   // 00669CAC..00669CB8
                st<U>(creature, 0x364, rd32(creature, 0x364) + 1);    // 00669CBB
            }
            recovered_client_creature_update_relative_perception_0060c980(creature, nullptr, node);   // 00669CFA..00669CFD
        }
    }

    if ((flags & 0x800) != 0) {                                       // 00669D17..00669D1E (test ah,8)
        const U present = read_bool(self);                            // 00669D24..00669D2F (ebx)
        if (overflow(self)) return 0;                                 // 00669D2B..00669D38
        if (present == 1) {                                           // 00669D3E..00669D41
            U8 v[15];                                                 // bl, {esp+13h}, {esp+12h}, {esp+14h..1Fh}
            for (U8& b : v) b = read_byte(self);                      // 00669D47..00669E08 (15 x ReadBYTE, in order)
            if (overflow(self)) return 0;                             // 00669E04..00669E13
            if (creature != nullptr) {                                // 00669E19..00669E1B
                static const U8 offsets[15] = {0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                               0x5D, 0x5E, 0x5C};     // 00669E21..00669EDD
                for (int k = 0; k < 15; ++k) st<U8>(stats_of(creature), offsets[k], v[k]);
            }
        }
    }

    if ((flags & 0x8000) != 0) {                                      // 00669EE0..00669EE6 (test ah,ah; jns)
        if (skill_count() > 0) {                                      // 00669EEC..00669EFC
            for (I32 skill = 0; skill < static_cast<I32>(skill_count()); ++skill) {   // 00669EF8 / 00669F5C..00669F6B
                void* ranks = stats_of(creature);                     // 00669F00
                const U8 base = read_byte(self);                      // 00669F06..00669F0A
                if (static_cast<U16>(skill) < static_cast<U16>(skill_count()))   // 00669F0F..00669F20
                    st<U8>(rp(ranks, 0x88), static_cast<U16>(skill), base);      // 00669F22..00669F2B
                ranks = stats_of(creature);                           // 00669F2E
                const U8 modified = read_byte(self);                  // 00669F34..00669F38
                if (static_cast<U16>(skill) < static_cast<U16>(skill_count()))   // 00669F3D..00669F4E
                    st<U8>(rp(ranks, 0x84), static_cast<U16>(skill), modified);  // 00669F50..00669F59
            }
        }
    }

    if ((flags & 0x10000) != 0) {                                     // 00669F6D..00669F77
        const U16 portrait = read_word(self);                         // 00669F79..00669F7D
        if (portrait >= 0xFFFE) {                                     // 00669F82..00669F86
            const void* const portrait_methods = ld<const void*>(creature, 0);   // 00669F88: cache table before input read
            ResRef read;                                              // {esp+40h}
            void* const ref = kotor_fastcall_004d6180(self, nullptr, &read, 0x10);   // 00669F8A..00669F93
            using SetPortrait = void (__fastcall*)(void*, void*, void*);
            ld<SetPortrait>(portrait_methods, 0xEC)(creature, nullptr, ref);   // 00669F95..00669F9B: original cached table
        }
    }

    if ((flags & 0x200) != 0) {                                       // 00669FA1..00669FA7
        {
            void* const current_stats = stats_of(creature);           // 00669FA9..00669FB8
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x48, value);
        }
        {
            void* const current_stats = stats_of(creature);           // 00669FBC..00669FCB
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x4C, value);
        }
        {
            void* const current_stats = stats_of(creature);           // 00669FCF..00669FDE
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x4A, value);
        }
        {
            void* const current_stats = stats_of(creature);           // 00669FE2..00669FF1
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x4E, value);
        }
    }

    if ((flags & 0x20000) != 0) {                                     // 00669FF5..00669FFB
        {
            void* const current_stats = stats_of(creature);          // 00669FFD..0066A00C
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x120, value);
        }
        {
            void* const current_stats = stats_of(creature);         // 0066A013..0066A022
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x122, value);
        }
        {
            void* const current_stats = stats_of(creature);         // 0066A029..0066A038
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x124, value);
        }
        {
            void* const current_stats = stats_of(creature);         // 0066A03F..0066A04E
            const U16 value = read_word(self);
            st<U16>(current_stats, 0x11E, value);
        }
    }

    if ((flags & 0x4000) != 0) {                                      // 0066A055..0066A05B
        U records = read_word(self);                                  // 0066A061..0066A070 {esp+34h}
        if (overflow(self)) return 0;                                 // 0066A06A..0066A07B
        if (static_cast<U16>(records) != 0) {                         // 0066A081..0066A084
            do {
                const U8 op = read_byte(self);                        // 0066A090..0066A09D (bl)
                if (overflow(self)) return 0;                         // 0066A099..0066A0A6
                if (op == 0x53) {                                     // 0066A0AC..0066A0AF 'S' size
                    if (rp(creature, 0x3A0) == nullptr)               // 0066A0B1..0066A0B9
                        recovered_client_creature_create_sla_list_00617a20(creature, nullptr);   // 0066A0BB..0066A0BD
                    const U16 size = read_word(self);                 // 0066A0C2..0066A0CF (ebx)
                    if (overflow(self)) return 0;                     // 0066A0CB..0066A0D8
                    void* const list = rp(creature, 0x3A0);           // 0066A0DE (ebp)
                    if (size == 0) {                                  // 0066A0E4..0066A0E9
                        if (rp(list, 0) != nullptr) {                 // 0066A0EB..0066A0F0
                            live_free(rp(list, 0));                   // 0066A0F2..0066A0F3
                            st<U>(list, 0, 0);                        // 0066A0FB
                            st<U>(list, 8, 0);                        // 0066A0FE
                            st<U>(list, 4, 0);                        // 0066A101
                        } else {
                            st<U>(list, 4, 0);                        // 0066A111
                        }
                    } else {
                        recovered_sla_list_allocate_00610bc0(list, nullptr, size);   // 0066A109..0066A10C
                        st<U>(list, 4, size);                         // 0066A111
                    }
                } else if (op == 0x55) {                              // 0066A116..0066A119 'U' entry
                    const U16 index = read_word(self);                // 0066A11B..0066A124 (bp)
                    const U ready = read_bool(self);                  // 0066A126..0066A131 (ebx)
                    const U8 uses = read_byte(self);                  // 0066A12D..0066A13C {esp+1Fh}
                    const U16 spell = read_word(self);                // 0066A138..0066A149 {esp+30h}
                    if (overflow(self)) return 0;                     // 0066A145..0066A154
                    unsigned char* const entry = static_cast<unsigned char*>(rp(rp(creature, 0x3A0), 0)) +
                                                 static_cast<U>(index) * 12u;   // 0066A15A..0066A168
                    st<U>(entry, 4, ready);                           // 0066A16B
                    st<U8>(entry, 8, uses);                           // 0066A17B
                    st<U>(entry, 0, static_cast<U>(spell));           // 0066A185..0066A18C
                }
                records += 0xFFFFu;                                   // 0066A18F..0066A193
            } while (static_cast<U16>(records) != 0);                 // 0066A198..0066A19F (test ax,ax; ja)
        }
    }

    if (kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr) != id &&   // 0066A1A5..0066A1B9
        kotor_m15e_client_get_mini_game_005edb00(client_app(), nullptr) == nullptr)        // 0066A1BB..0066A1CA
        (void)recovered_client_handle_creature_control_object_0064caf0(self, nullptr, id);   // 0066A1CC..0066A1CF
    return 1;                                                         // 0066A1D9
}

// =========================================================================================== 'G' GuiElements
// 0066A1F0 CSWCMessage::HandleServerToPlayerUpdate_GuiElements: BYTE panel letter, then the panel reader
// (byte table 0066A280 / jump table 0066A264): 'A' and 'R' -> GuiRepository(0), 'B' -> GuiRepository(1),
// 'C' -> GuiContainer, 'I' -> GuiInventory, 'Q' -> GuiQuickbarItemUseCount, 'S' -> CharacterSheet. ECX this, RET.
U __fastcall recovered_client_handle_update_gui_elements_0066a1f0(void* self, void*) {
    const U8 panel = read_byte(self);                                 // 0066A1F2..0066A1FF (bl)
    if (overflow(self)) return 0;                                     // 0066A1FB..0066A208
    switch (panel) {                                                  // 0066A20A..0066A21C
    case 0x41: return recovered_client_handle_gui_repository_0064fdf0(self, nullptr, 0);   // 0066A24A..0066A255
    case 0x42: return recovered_client_handle_gui_repository_0064fdf0(self, nullptr, 1);   // 0066A23E..0066A249
    case 0x43: return recovered_client_handle_gui_container_0064fef0(self, nullptr);       // 0066A235..0066A239
    case 0x49: return recovered_client_handle_gui_inventory_0064fc10(self, nullptr);       // 0066A223..0066A227
    case 0x51: return recovered_client_handle_gui_quickbar_use_count_0064fb20(self, nullptr);   // 0066A256..0066A25A
    case 0x52: return recovered_client_handle_gui_repository_0064fdf0(self, nullptr, 0);   // 0066A24A..0066A255
    case 0x53: return recovered_client_handle_character_sheet_00667cc0(self, nullptr);     // 0066A22C..0066A230
    default: return 0;                                                // 0066A25F..0066A263
    }
}

// 0064FDF0 CSWCMessage::HandleServerToPlayerUpdate_GuiRepository(BOOL). BYTE op: 'A' item add (ItemUpdate_Add with
// flag 1), 'U' OBJECTID + DWORD stack size + DWORD +180h, 'D' OBJECTID (looked up, nothing else). The argument is
// unused. ECX this, RET 4.
U __fastcall recovered_client_handle_gui_repository_0064fdf0(void* self, void*, U /*which*/) noexcept(false) {
    const U8 op = read_byte(self);                                    // 0064FDF4..0064FE01 (bl)
    if (overflow(self)) return 0;                                     // 0064FDFD..0064FE0A
    if (op == 0x41) {                                                 // 0064FE0C..0064FE0F
        if (overflow(self)) return 0;                                 // 0064FE11..0064FE1C
        U item = 0;                                                   // {E-0x4} (the pushed ecx slot)
        (void)recovered_client_handle_item_add_0064e7b0(self, nullptr, 1, word_of(&item));   // 0064FE27..0064FE30 (result unused)
        return 1;                                                     // 0064FE37
    }
    if (op == 0x55) {                                                 // 0064FE41..0064FE45
        const U id = read_id(self);                                   // 0064FE47..0064FE52 (edi)
        const U stack = read_dword(self);                             // 0064FE4E..0064FE5D (ebx)
        const U value = read_dword(self);                             // 0064FE59..0064FE68 (ebp)
        if (overflow(self)) return 0;                                 // 0064FE64..0064FE71
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 0064FE73..0064FE82
        if (item != nullptr) {                                        // 0064FE84..0064FE86
            recovered_client_item_set_stack_size_00646810(item, nullptr, stack);   // 0064FE88..0064FE8B
            st<U>(item, 0x180, value);                                // 0064FE90
        }
        return 1;                                                     // 0064FE99
    }
    if (op == 0x44) {                                                 // 0064FEA3..0064FEA6
        const U id = read_id(self);                                   // 0064FEA8..0064FEB3
        if (overflow(self)) return 0;                                 // 0064FEAF..0064FEBC
        (void)kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 0064FEBE..0064FEC8 (unused)
        return 1;                                                     // 0064FED0
    }
    return 0;                                                         // 0064FEDA..0064FEE1
}

// 0064FEF0 CSWCMessage::HandleServerToPlayerUpdate_GuiContainer. BYTE op: 'P' BYTE (returns 0), 'A' BYTE x, BYTE y,
// item add (flag 1) -> item +15Ch/+15Dh = x/y, +168h = 1; 'U' as GuiRepository; 'D' OBJECTID; 'M' BYTE x, BYTE y,
// OBJECTID -> item +15Ch/+15Dh (0 when the item is unknown). ECX this, RET.
U __fastcall recovered_client_handle_gui_container_0064fef0(void* self, void*) noexcept(false) {
    const U8 op = read_byte(self);                                    // 0064FEF6..0064FF03 (bl)
    if (overflow(self)) return 0;                                     // 0064FEFF..0064FF0C
    if (op == 0x50) {                                                 // 0064FF17..0064FF1B
        (void)read_byte(self);                                        // 0064FF1D..0064FF21
        return 0;                                                     // 0064FF26..0064FF2F
    }
    if (op == 0x41) {                                                 // 0064FF30..0064FF33
        const U8 x = read_byte(self);                                 // 0064FF35..0064FF42 (bl)
        const U8 y = read_byte(self);                                 // 0064FF3E..0064FF4D {esp+13h}
        if (overflow(self)) return 0;                                 // 0064FF49..0064FF58
        U item = 0;                                                   // {esp+14h} (uninitialised in the original)
        (void)recovered_client_handle_item_add_0064e7b0(self, nullptr, 1, word_of(&item));   // 0064FF5A..0064FF63 (result unused)
        void* const object = word_ptr(item);                          // 0064FF68
        st<U8>(object, 0x15C, x);                                     // 0064FF72
        st<U8>(object, 0x15D, y);                                     // 0064FF78
        st<U>(object, 0x168, 1);                                      // 0064FF7F
        return 1;                                                     // 0064FF89
    }
    if (op == 0x55) {                                                 // 0064FF93..0064FF96
        const U id = read_id(self);                                   // 0064FF98..0064FFA3 (edi)
        const U stack = read_dword(self);                             // 0064FF9F..0064FFAE (ebx)
        const U value = read_dword(self);                             // 0064FFAA..0064FFB9 (ebp)
        if (overflow(self)) return 0;                                 // 0064FFB5..0064FFC2
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 0064FFC8..0064FFD7
        if (item != nullptr) {                                        // 0064FFD9..0064FFDB
            recovered_client_item_set_stack_size_00646810(item, nullptr, stack);   // 0064FFDD..0064FFE0
            st<U>(item, 0x180, value);                                // 0064FFE5
        }
        return 1;                                                     // 0064FFEE
    }
    if (op == 0x44) {                                                 // 0064FFF8..0064FFFB
        const U id = read_id(self);                                   // 0064FFFD..00650008
        if (overflow(self)) return 0;                                 // 00650004..00650011
        (void)kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 00650017..00650020 (unused)
        return 1;                                                     // 00650028
    }
    if (op == 0x4D) {                                                 // 00650032..00650035
        const U8 x = read_byte(self);                                 // 0065003B..00650048 (bl)
        const U8 y = read_byte(self);                                 // 00650044..00650051 {esp+13h}
        const U id = read_id(self);                                   // 0065004F..0065005E (edi)
        if (overflow(self)) return 0;                                 // 0065005A..00650067
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 0065006D..00650077
        if (item == nullptr) return 0;                                // 0065007C..0065007E
        st<U8>(item, 0x15C, x);                                       // 0065008A
        st<U8>(item, 0x15D, y);                                       // 00650091
        return 1;                                                     // 00650097
    }
    return 0;                                                         // 0064FF26..0064FF2F
}

// 0064FB20 CSWCMessage::HandleServerToPlayerUpdate_GuiQuickbarItemUseCount. BYTE count, then per entry BYTE, BYTE
// (discarded), OBJECTID item, BYTE property, WORD uses: an active property not of kind 1 gets the uses left, otherwise
// the item's stack size is set. ECX this, RET.
U __fastcall recovered_client_handle_gui_quickbar_use_count_0064fb20(void* self, void*) noexcept(false) {
    const U8 count = read_byte(self);                                 // 0064FB25..0064FB32 (bl)
    if (overflow(self)) return 0;                                     // 0064FB2E..0064FB3B
    for (I32 i = 0; i < static_cast<I32>(count); ++i) {               // 0064FB45..0064FBED {esp+10h} / {esp+14h}
        (void)read_byte(self);                                        // 0064FB60..0064FB64
        (void)read_byte(self);                                        // 0064FB69..0064FB6D
        const U id = read_id(self);                                   // 0064FB72..0064FB7D (edi)
        const U8 property = read_byte(self);                          // 0064FB79..0064FB88 (bl)
        const U16 uses = read_word(self);                             // 0064FB84..0064FB93 (bp)
        if (overflow(self)) return 0;                                 // 0064FB8F..0064FB9C
        void* const item = kotor_m15_client_get_item_by_id_005ed5a0(client_app(), nullptr, id);   // 0064FB9E..0064FBAC
        if (item == nullptr) continue;                                // 0064FBAE..0064FBB0
        void* const active = recovered_client_item_get_active_property_006466a0(item, nullptr, property);   // 0064FBB2..0064FBB8
        if (active != nullptr && ld<U16>(active, 6) != 1)             // 0064FBBD..0064FBC6
            recovered_client_item_set_property_uses_left_006469b0(item, nullptr, property, uses);   // 0064FBC8..0064FBCC
        else
            recovered_client_item_set_stack_size_00646810(item, nullptr, uses);   // 0064FBD3..0064FBD9
    }
    return 1;                                                         // 0064FBF6
}

// 0064FC10 CSWCMessage::HandleServerToPlayerUpdate_GuiInventory. BYTE op: 'A' DWORD slot + item (ItemUpdate_Add flag
// 1) -> item +154h = slot, +158h = -1, equipped on the player creature unless already there; 'D' DWORD slot (read
// only). While the party-selection GUI is up (server info +104h bit 0) the equip-screen NPC is made the player
// creature for the duration. ECX this, RET.
U __fastcall recovered_client_handle_gui_inventory_0064fc10(void* self, void*) noexcept(false) {
    const U8 op = read_byte(self);                                    // 0064FC15..0064FC22 (bl)
    if (overflow(self)) return 0;                                     // 0064FC1E..0064FC2B
    U swapped = 0;                                                    // 0064FC3E..0064FC41 (ebp, {esp+10h})
    const U previous = kotor_m15_client_get_player_creature_id_005ed550(client_app(), nullptr);   // 0064FC35..0064FC55 (edi, {esp+18h})
    void* const server = rp(kotor_menu_window_cells::manager_007a39fc, 8);   // 0064FC4A..0064FC50 [[007A39FC]+8]
    const U info = rd32(rp(server, 4), 0x10004);                      // 0064FC59 004AEC90 GetServerInfo (inline field read)
    if ((ld<U8>(word_ptr(info), 0x104) & 1) != 0 &&                   // 0064FC5E..0064FC65
        recovered_gui_in_game_get_party_access_panel_up_0062b4d0(frame_R4_005ed690(client_app(), nullptr), nullptr, 1) != 0) {   // 0064FC67..0064FC80
        const I32 npc = recovered_gui_in_game_get_current_equip_screen_npc_0062fea0(
            frame_R4_005ed690(client_app(), nullptr), nullptr);       // 0064FC82..0064FC91
        void* const party = lane_config_required::GetPartyTable_004aee70(server, nullptr);   // 0064FC96..0064FCA3
        const U npc_id = recovered_npc_object_00564700(party, nullptr, static_cast<U>(npc), 0, 1);   // 0064FC9F..0064FCAA
        if (recovered_client_dont_use_this_set_player_creature_005ee2e0(client_app(), nullptr, npc_id) != 0)   // 0064FCAF..0064FCC0
            swapped = 1;                                              // 0064FCC2..0064FCCA
    }
    U result = 0;
    if (op == 0x41) {                                                 // 0064FCCE..0064FCD1
        const U slot = read_dword(self);                              // 0064FCD7..0064FCE4 (ebx)
        if (overflow(self)) {                                         // 0064FCE0..0064FCED -> 0064FD8B
            result = 0;
        } else {
            U item_word = 0;                                          // {esp+14h}
            if (recovered_client_handle_item_add_0064e7b0(self, nullptr, 1, word_of(&item_word)) == 0) {   // 0064FCF3..0064FD03
                result = 0;                                           // -> 0064FDC7
            } else {
                void* const item = word_ptr(item_word);               // 0064FD09 (esi)
                st<U>(item, 0x154, slot);                             // 0064FD0D
                st<U>(item, 0x158, 0xFFFFFFFFu);                      // 0064FD13
                void* const player = client_get_player_creature_005ed540(client_app(), nullptr);   // 0064FD1D..0064FD25
                if (player != nullptr &&                              // 0064FD2A..0064FD2E
                    kotor_m15a_creature_get_equipped_item_id_0060ca80(player, nullptr, slot) != rd32(item, 0x14))   // 0064FD30..0064FD3D
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(player, nullptr, slot, item);   // 0064FD3F..0064FD43
                result = 1;                                           // 0064FD66
            }
        }
    } else if (op == 0x44) {                                          // 0064FD70..0064FD73
        (void)read_dword(self);                                       // 0064FD75..0064FD79
        result = overflow(self) ? 0u : 1u;                            // 0064FD7E..0064FD89
    }
    if (swapped != 0)                                                 // 0064FD48..0064FD4E / 0064FD8B / 0064FDA7 / 0064FDC7
        (void)recovered_client_dont_use_this_set_player_creature_005ee2e0(client_app(), nullptr, previous);   // 0064FD50..0064FD5E
    return result;
}

// ================================================================================================ item 'A'
// 0064E7B0 CSWCMessage::HandleServerToPlayerItemUpdate_Add(DWORD flag, CSWCItem** out). Record: OBJECTID item, item
// appearance (0064E730: DWORD base item, BYTE variation [, BYTE, BYTE for base items with +18h == 1]), then the
// active properties (0064E3D0). flag 0: area item (GameObjectUpdate 'A'); flag 1: inventory / GUI item, returned
// through *out and kept out of the area. An item the client already has is reused: an area copy is detached from
// its equipping creature; any other flag value falls through to creating a new item. ECX this, RET 8; EAX 1, or 0
// on overflow / CreateItem failure / AddExternalObject failure.
U __fastcall recovered_client_handle_item_add_0064e7b0(void* self, void*, U flag, U item_out) {
    U8 gender = 0x4D;                                                 // 0064E7B6 {E-0x14} = 'M'
    U id = read_id(self);                                             // 0064E7B4..0064E7C4 {E-0x18}
    if (overflow(self)) return 0;                                     // 0064E7C0..0064E7D7
    U base_item = 0;                                                  // {E-0x4}
    U8 variation = 0, extra_a = 0, extra_b = 0;                       // {E-0x8} (low byte) / {E-0x19} / {E-0x1a}
    (void)recovered_client_handle_item_appearance_0064e730(self, nullptr, &base_item, &variation, &extra_a, &extra_b);   // 0064E7DD..0064E7F3 (result unused)
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0064E7F8..0064E80F (ebx)
    void* const objects = kotor_fastcall_005ed570(client_app(), nullptr);          // 0064E806..0064E823 {E-0x10}
    void* found = nullptr;                                            // {E-0xc}
    if (kotor_fastcall_004d8230(objects, nullptr, id, &found) == kGameObjectOk_0074666c) {   // 0064E817..0064E832
        void* const item = vslot<void* (__fastcall*)(void*, void*)>(found, 0x34)(found, nullptr);   // 0064E838..0064E841 (esi)
        st<U>(item, 0x168, 0);                                        // 0064E846
        void* const base = kotor_m15b_base_item_array_get_base_item_005b31a0(rp(rules(), 0x34), nullptr, rd32(item, 0xC));   // 0064E843..0064E85A
        if (base != nullptr && ld<U8>(base, 0x18) == 1)               // 0064E85F..0064E867
            (void)client_get_player_creature_005ed540(client_app(), nullptr);   // 0064E869..0064E872 (unused)
        void* const object = at(item, 0x10);                          // CSWCObject sub-object
        if (flag == 0) {                                              // 0064E877..0064E87D
            if (rd32(item, 0x164) == kInvalidObject_00746644) {       // 0064E883..0064E88D not on a creature
                recovered_client_object_set_visibility_on_set_area_0063e350(object, nullptr, area);   // 0064E88F..0064E895
                vslot<U (__fastcall*)(void*, void*, void*)>(object, 0x94)(object, nullptr, word_ptr(rd32(area, 0x184)));   // 0064E89A..0064E8A5 AttachToScene
                recovered_client_ai_master_add_object_00604a20(
                    kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr), nullptr, object);   // 0064E8AB..0064E8BC
                (void)recovered_client_handle_active_item_properties_0064e3d0(self, nullptr, id);   // 0064E8C1..0064E8C6 -> 0064EAA2
                return 1;                                             // 0064EAAC
            }
            void* const holder = word_ptr(kotor_m15e_item_get_equipped_by_00646630(item, nullptr));   // 0064E8CB..0064E8CD (edi)
            if (holder != nullptr) {                                  // 0064E8D2..0064E8D6
                if (kotor_m15a_creature_get_equipped_item_id_0060ca80(holder, nullptr, 0x10) == id)   // 0064E8D8..0064E8E5
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(holder, nullptr, 0x10, nullptr);   // 0064E8E7..0064E8ED
                if (kotor_m15a_creature_get_equipped_item_id_0060ca80(holder, nullptr, 0x20) == id)   // 0064E8F2..0064E8FF
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(holder, nullptr, 0x20, nullptr);   // 0064E901..0064E907
                if (kotor_m15a_creature_get_equipped_item_id_0060ca80(holder, nullptr, 1) == id)      // 0064E90C..0064E919
                    recovered_client_creature_equip_to_inventory_slot_00613ba0(holder, nullptr, 1, nullptr);      // 0064E91B..0064E921
            } else {
                recovered_client_ai_master_add_object_00604a20(
                    kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr), nullptr, object);   // 0064E928..0064E93C
            }
            kotor_m15a_item_set_equipped_by_00646600(item, nullptr, nullptr);   // 0064E941..0064E945
            void* previous_holder = nullptr;                          // {E+0x4} (the flag argument slot, reused)
            if (kotor_fastcall_004d8230(objects, nullptr, rd32(item, 0x164), &previous_holder) == kGameObjectOk_0074666c &&   // 0064E94A..0064E965
                vslot<void* (__fastcall*)(void*, void*)>(previous_holder, 0x2C)(previous_holder, nullptr) != nullptr) {   // 0064E967..0064E972
                kotor_m15a_switch_weapon_event_00610f40(
                    0, nullptr, vslot<void* (__fastcall*)(void*, void*)>(previous_holder, 0x2C)(previous_holder, nullptr));   // 0064E974..0064E987
            }
            vslot<void (__fastcall*)(void*, void*, void*)>(object, 0x84)(object, nullptr, nullptr);   // 0064E98A..0064E994 SetArea(0)
            vslot<U (__fastcall*)(void*, void*, void*)>(object, 0x94)(object, nullptr, word_ptr(rd32(area, 0x184)));   // 0064E99A..0064E9A5 AttachToScene
            (void)recovered_client_handle_active_item_properties_0064e3d0(self, nullptr, id);   // 0064EA9D..0064EAA4
            return 1;                                                 // 0064EAAC
        }
        if (flag == 1) {                                              // 0064E9B0..0064E9B3
            vslot<void (__fastcall*)(void*, void*, void*)>(object, 0x84)(object, nullptr, nullptr);   // 0064E9B5..0064E9BD SetArea(0)
            st<void*>(word_ptr(item_out), 0, item);                   // 0064E9C3..0064E9C7
            (void)recovered_client_handle_active_item_properties_0064e3d0(self, nullptr, id);   // 0064EA9D..0064EAA4
            return 1;                                                 // 0064EAAC
        }
    }
    void* const player = client_get_player_creature_005ed540(client_app(), nullptr);   // 0064E9CE..0064E9D6
    if (player != nullptr && rp(player, 0x21C) != nullptr)            // 0064E9DB..0064E9E7
        gender = recovered_client_creature_appearance_gender_char_00696ca0(rp(player, 0x21C), nullptr);   // 0064E9E9..0064E9F0
    void* const item = recovered_client_create_item_0064b540(self, nullptr, base_item, variation, gender);   // 0064E9F4..0064EA05
    if (item == nullptr) return 0;                                    // 0064EA0A..0064EA19
    void* const object = at(item, 0x10);                              // 0064EA23 (esi)
    vslot<void (__fastcall*)(void*, void*, U)>(object, 0x04)(object, nullptr, id);   // 0064EA1C..0064EA29 SetId
    if (flag == 1) {                                                  // 0064EA2C..0064EA35
        vslot<U (__fastcall*)(void*, void*, void*)>(object, 0x94)(object, nullptr, nullptr);   // 0064EA37..0064EA39 AttachToScene(0)
        vslot<void (__fastcall*)(void*, void*, void*)>(object, 0x84)(object, nullptr, nullptr);   // 0064EA3F..0064EA45 SetArea(0)
        st<void*>(word_ptr(item_out), 0, item);                       // 0064EA4B..0064EA4F
    } else {
        vslot<U (__fastcall*)(void*, void*, void*)>(object, 0x94)(object, nullptr, word_ptr(rd32(area, 0x184)));   // 0064EA53..0064EA5A
        vslot<void (__fastcall*)(void*, void*, void*)>(object, 0x84)(object, nullptr, area);   // 0064EA60..0064EA65 SetArea(area)
        recovered_client_ai_master_add_object_00604a20(
            kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr), nullptr, object);   // 0064EA6B..0064EA7B
    }
    if (recovered_object_array_add_external_004d7ff0(objects, nullptr, &id, object, 1) != kGameObjectOk_0074666c)   // 0064EA80..0064EA97
        return 0;                                                     // -> 0064EA10
    (void)recovered_client_handle_active_item_properties_0064e3d0(self, nullptr, id);   // 0064EA9D..0064EAA4
    return 1;                                                         // 0064EAAC
}
// ============================================================================================ trigger 'A'
// Tables generated from the original .rdata slot words (checked slot by slot against swkotor.exe); shared slots point
// at the same owners as the CSWCDoor / CSWCAnimBaseDoor tables of w3_client_door_placeable.cpp.
const void* const recovered_client_trigger_methods_00754440[82] = {
    reinterpret_cast<const void*>(&recovered_client_trigger_scalar_destructor_00691ca0),               // +000 00691CA0 CSWCTrigger::~CSWCTrigger (D83 trap)
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
    reinterpret_cast<const void*>(&kotor_m15e_slot02c_zero_0063e7f0),                                  // +034 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),                                  // +038 0063E7F0 return_zero
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),                                  // +03C 00641DB0 self_return
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
    reinterpret_cast<const void*>(&recovered_client_trigger_get_name_006912b0),                        // +070 006912B0 CSWCTrigger::GetName
    reinterpret_cast<const void*>(&recovered_client_trigger_ai_update_00692410),                       // +074 00692410 CSWCTrigger::AIUpdate (D83 trap)
    reinterpret_cast<const void*>(&recovered_client_trigger_load_model_006910d0),                      // +078 006910D0 CSWCTrigger::LoadModel
    reinterpret_cast<const void*>(&kotor_m15e_object_unload_model_00646650),                          // +07C 00646650 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_object_unload_model_006825f0),                     // +080 006825F0 CSWCObject::UnloadModel
    reinterpret_cast<const void*>(&recovered_client_object_set_area_0063f570),                         // +084 0063F570 CSWCObject::SetArea
    reinterpret_cast<const void*>(&kotor_m15e_object_set_orientation_0063c110),                        // +088 0063C110 CSWCObject::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063f340),                                          // +08C 0063F340 CSWCObject::SetPosition
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
    reinterpret_cast<const void*>(&recovered_client_trigger_is_hostile_006912e0),                      // +138 006912E0 CSWCTrigger::IsHostile
    reinterpret_cast<const void*>(&recovered_client_trigger_get_reticle_point_00691b00),               // +13C 00691B00 CSWCTrigger::GetReticlePoint (D83 trap)
    reinterpret_cast<const void*>(&kotor_m15e_trap_00616fe0),                                          // +140 00616FE0 return_1.5
    reinterpret_cast<const void*>(&kotor_m15e_trap_0063d780),                                          // +144 0063D780 CSWCObject::GetStickyMouseThreshhold
};
const void* const recovered_anim_base_trigger_methods_00758c70[62] = {
    reinterpret_cast<const void*>(&w3_anim_trigger_scalar_006e6020),                   // +000 006E6020 CSWCAnimBaseTrigger::~CSWCAnimBaseTrigger (D83 trap)
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_attach_to_scene_0069cd80),                     // +004 0069CD80 CSWCAnimBase::AttachToScene
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_model_0069cda0),                           // +008 0069CDA0 CSWCAnimBase::GetModel
    reinterpret_cast<const void*>(&recovered_anim_base_trigger_load_model_006e5fb0),                   // +00C 006E5FB0 CSWCAnimBaseTrigger::LoadModel
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069cfd0_s010),                                  // +010 0069CFD0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069cef0),                     // +014 0069CEF0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069cdc0),                     // +018 0069CDC0 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_replace_texture_0069ce50),                     // +01C 0069CE50 CSWCAnimBase::ReplaceTexture
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_restore_texture_0069d100),                     // +020 0069D100 CSWCAnimBase::RestoreTexture
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d0a0_s024),                                  // +024 0069D0A0 CSWCAnimBase::RestoreTexture
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dd90_s028),                                  // +028 0069DD90 CSWCAnimBase::EnableBumpedOutShell
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069ddf0_s02c),                                  // +02C 0069DDF0 CSWCAnimBase::DisableBumpedOutShell
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069de10_s030),                                  // +030 0069DE10 CSWCAnimBase::SetMotionBlur
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069de40_s034),                                  // +034 0069DE40 CSWCAnimBase::SetMaxMotionBlurLength
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_animation_length_0069d250),                // +038 0069D250 CSWCAnimBase::GetAnimationLength
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_animation_time_0069d180),                  // +03C 0069D180 CSWCAnimBase::GetAnimationTime
    reinterpret_cast<const void*>(&w3_anim_trigger_name_006e6040),           // +040 006E6040 CSWCAnimBaseTrigger::GetAnimationName (D83 trap)
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_006a03c0_s044),                                  // +044 006A03C0 CSWCAnimBase::SetOverlayAnimation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d3e0_s048),                                  // +048 0069D3E0 CSWCAnimBase::GetOverlayAnimation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_get_overlay_animation_playing_0069d320),       // +04C 0069D320 CSWCAnimBase::GetOverlayAnimationPlaying
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_animation_exists_0069d910),                    // +050 0069D910 CSWCAnimBase::AnimationExists
    reinterpret_cast<const void*>(&recovered_anim_base_set_animation_0069d4f0),                        // +054 0069D4F0 CSWCAnimBase::SetAnimation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d520_s058),                                  // +058 0069D520 CSWCAnimBase::SetAnimationOnSubObject
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d540_s05c),                                  // +05C 0069D540 CSWCAnimBase::SetAnimationVisemes
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_orientation_0069d640),                     // +060 0069D640 CSWCAnimBase::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_orientation_0069d5d0),                     // +064 0069D5D0 CSWCAnimBase::SetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_position_0069d690),                        // +068 0069D690 CSWCAnimBase::SetPosition
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d6e0_s06c),                                  // +06C 0069D6E0 CSWCAnimBase::SetAlwaysRender
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d710_s070),                                  // +070 0069D710 CSWCAnimBase::SetInCutscene
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_unload_model_0069d770),                        // +074 0069D770 CSWCAnimBase::UnloadModel
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_unload_model_0069d740),                        // +078 0069D740 CSWCAnimBase::UnloadModel
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_00401590_s07c),                                  // +07C 00401590 return_zero_0x10
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_005b5e90_s080),                                  // +080 005B5E90 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s084),                                  // +084 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s088),                                  // +088 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_004015a0_s08c),                                  // +08C 004015A0 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s090),                                  // +090 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_alpha_0069eb20),                           // +094 0069EB20 CSWCAnimBase::SetAlpha
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_env_map_0069d830),                         // +098 0069D830 CSWCAnimBase::SetEnvMap
    reinterpret_cast<const void*>(&recovered_anim_base_set_env_map_0069d7b0),                          // +09C 0069D7B0 CSWCAnimBase::SetEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d8b0_s0a0),                                  // +0A0 0069D8B0 CSWCAnimBase::RestoreEnvMap
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_apply_shadow_blob_0069da00),                   // +0A4 0069DA00 CSWCAnimBase::ApplyShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_remove_shadow_blob_0069da70),                  // +0A8 0069DA70 CSWCAnimBase::RemoveShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_show_shadow_blob_0069dac0),                    // +0AC 0069DAC0 CSWCAnimBase::ShowShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_hide_shadow_blob_0069db10),                    // +0B0 0069DB10 CSWCAnimBase::HideShadowBlob
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_set_shadow_blob_orientation_0069db50),         // +0B4 0069DB50 CSWCAnimBase::SetShadowBlobOrientation
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_turn_on_shadows_0069dc10),                     // +0B8 0069DC10 CSWCAnimBase::TurnOnShadows
    reinterpret_cast<const void*>(&recovered_anim_base_turn_off_shadows_0069dc30),                     // +0BC 0069DC30 CSWCAnimBase::TurnOffShadows
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dc50_s0c0),                                  // +0C0 0069DC50 CSWCAnimBase::GetOrientation
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s0c4),                                  // +0C4 0060E760 void_return
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069a3a0_s0c8),                                  // +0C8 0069A3A0 CSWCAnimBase::AddAnimationFlag
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069a3c0_s0cc),                                  // +0CC 0069A3C0 CSWCAnimBase::RemoveAnimationFlag
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dd30_s0d0),                                  // +0D0 0069DD30 CSWCAnimBase::SetLean
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069dc90_s0d4),                                  // +0D4 0069DC90 CSWCAnimBase::Lean
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069f630_s0d8),                                  // +0D8 0069F630 CSWCAnimBase::GetBoundingRect
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069f1e0_s0dc),                                  // +0DC 0069F1E0 CSWCAnimBase::GetCenterPoint
    reinterpret_cast<const void*>(&w3_anim_trigger_convert_006e6100),   // +0E0 006E6100 CSWCAnimBaseTrigger::ServerToClientAnimation (D83 trap)
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_enable_distortion_0069de50),                   // +0E4 0069DE50 CSWCAnimBase::EnableDistortion
    reinterpret_cast<const void*>(&kotor_m15e_anim_base_disable_distortion_0069de70),                  // +0E8 0069DE70 CSWCAnimBase::DisableDistortion
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0060e760_s0ec),                                  // +0EC 0060E760 void_return
    reinterpret_cast<const void*>(&recovered_anim_base_internal_0069e8d0),                             // +0F0 0069E8D0 CSWCAnimBase::SetAnimationInternal
    reinterpret_cast<const void*>(&kotor_m15e_ab_trap_0069d5a0_s0f4),                                  // +0F4 0069D5A0 CSWCAnimBase::SetAnimationOnSubObjectInternal
};

// 006E5F40 CSWCAnimBaseTrigger::CSWCAnimBaseTrigger: CSWCAnimBase base (REAL 0069DFB0), table 00758C70, [+C4h] = 9.
void* __fastcall recovered_anim_base_trigger_construct_006e5f40(void* self, void*) noexcept(false) {
    kotor_m15e_anim_base_construct_0069dfb0(self, nullptr);           // 006E5F41..006E5F43
    st<const void*>(self, 0, recovered_anim_base_trigger_methods_00758c70);   // 006E5F48
    st<U8>(self, 0xC4, 9);                                            // 006E5F4E
    return self;                                                      // 006E5F55
}

// 006E5FB0 CSWCAnimBaseTrigger::LoadModel(const CResRef*, BYTE part) (slot +0Ch) RET 8: any part; NewCAurObject
// (REAL render core) with gob class "trigger"; model at [+B8h], resref at [+2Ch].
U __fastcall recovered_anim_base_trigger_load_model_006e5fb0(void* self, void*, void* resref, U /*part*/) noexcept(false) {
    char name[0x14];                                                  // {E-0x14}
    gui_load_resref_copy_to_string_00405fb0(reinterpret_cast<const menu_resources::Ref*>(resref), nullptr, name);   // 006E5FB5..006E5FC2
    void* const gob = aurora_new_aur_object_00449cc0(name, kTrigger_00743754, nullptr, nullptr);   // 006E5FC7..006E5FDA
    st<void*>(self, 0xB8, gob);                                       // 006E5FDF
    if (gob == nullptr) return 0;                                     // 006E5FDD..006E5FEC
    ResRef tmp;                                                       // {E-0x24} hidden return
    c2da_copy_00406120(reinterpret_cast<menu_resources::Ref*>(at(self, 0x2C)), nullptr, reinterpret_cast<menu_resources::Ref*>(&tmp), reinterpret_cast<menu_resources::Ref*>(resref));        // 006E5FEF..006E5FF8
    st<U>(self, 0x3C, 0);                                             // 006E5FFE
    return 1;                                                         // 006E6005
}

// 006911E0 CSWCTrigger::CSWCTrigger (14Ch bytes): CSWCObject(7) base (REAL 0063E530), table 00754440, two CExoStrings,
// creation world time. RET, EAX this.
void* __fastcall recovered_client_trigger_construct_006911e0(void* self, void*) noexcept(false) {
    kotor_m15a_object_construct_0063e530(self, nullptr, kTrigger_00746636);   // 006911F8..00691208
    st<const void*>(self, 0, recovered_client_trigger_methods_00754440);   // 00691219
    exo_init(at(self, 0x100));                                        // 0069120F..0069121F
    exo_init(at(self, 0x140));                                        // 00691224..0069122F
    U low = 0, high = 0;                                              // {E-0x18} / {E-0x14}
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(client_app(), nullptr), nullptr, &low, &high);   // 00691234..00691253
    st<U>(self, 0x3C, low);                                           // 00691260
    st<U>(self, 0x130, 0);                                            // 00691267
    st<U>(self, 0x134, 0);                                            // 0069126D
    st<U>(self, 0x108, 0);                                            // 00691273
    st<U>(self, 0x10C, 0);                                            // 00691279
    st<U>(self, 0x110, 0);                                            // 0069127F
    st<U>(self, 0x118, 0);                                            // 00691285
    st<U>(self, 0xFC, 0);                                             // 0069128B
    st<U>(self, 0x138, 0);                                            // 00691291
    st<U>(self, 0x40, high);                                          // 00691297
    st<U8>(self, 0x148, 0xFF);                                        // 0069129A
    return self;                                                      // 006912A2
}

// 006912B0 CSWCTrigger::GetName (slot +70h): CExoString copy of [+100h] through the hidden return. RET 4.
void* __fastcall recovered_client_trigger_get_name_006912b0(void* self, void*, void* out) noexcept(false) {
    r8_copy_construct_005e5b00(out, nullptr, at(self, 0x100));        // 006912B2..006912C7
    return out;                                                       // 006912CC
}

// 006912E0 CSWCTrigger::IsHostile (slot +138h): [+114h]. RET.
U __fastcall recovered_client_trigger_is_hostile_006912e0(void* self, void*) noexcept { return rd32(self, 0x114); }   // 006912E0

// 006910D0 CSWCTrigger::LoadModel(const CResRef*, BYTE part, BYTE) (slot +78h) RET 0Ch: creates the
// CSWCAnimBaseTrigger (F0h) on first use, loads the model through its +0Ch, then hands the gob (+08h(part)) this
// object (+74h) and the object type 7 (+7Ch). EAX 1, or 0 when the anim base or the model is missing.
U __fastcall recovered_client_trigger_load_model_006910d0(void* self, void*, void* resref, U part, U /*flag*/) noexcept(false) {
    if (rp(self, 0x68) == nullptr) {                                  // 006910E9..006910EE
        void* const memory = live_operator_new(0xF0);                 // 006910F0..006910F5
        void* const anim = memory != nullptr ? recovered_anim_base_trigger_construct_006e5f40(memory, nullptr) : nullptr;   // 006910FD..00691116
        st<void*>(self, 0x68, anim);                                  // 00691122
        if (anim == nullptr) return 0;                                // 00691118..00691136
    }
    void* const anim = rp(self, 0x68);                                // 00691139
    if (vslot<U (__fastcall*)(void*, void*, void*, U)>(anim, 0x0C)(anim, nullptr, resref, part) == 0) return 0;   // 0069113C..00691160
    void* const gob = vslot<void* (__fastcall*)(void*, void*, U)>(anim, 0x08)(anim, nullptr, part);   // 00691163..0069116C
    if (gob != nullptr) {                                             // 0069116E..00691170
        vslot<void (__fastcall*)(void*, void*, void*)>(gob, 0x74)(gob, nullptr, self);   // 00691172..00691177
        vslot<void (__fastcall*)(void*, void*, U)>(gob, 0x7C)(gob, nullptr, kTrigger_00746636);   // 0069117A..00691186
    }
    return 1;                                                         // 0069118E
}

// 006914D0 CSWCTrigger::SetCursor(BYTE row): [+12Ch] = WORD [[rules+B8h]+4Ch][row].CursorID; kept when the entry is
// missing. RET 4.
void __fastcall recovered_client_trigger_set_cursor_006914d0(void* self, void*, U cursor) noexcept(false) {
    ExoString column;                                                 // {E-0x14}
    frame_LogStringCtor_005e5a90(&column, nullptr, kCursorId_00754588);   // 006914EC..006914F5
    U value = 0;                                                      // {E-0x18} (not initialised in the original)
    const int found = c2da_current_get_int_00413660(rp(rp(rules(), 0xB8), 0x4C), nullptr, static_cast<U8>(cursor),
                                                    &column, &value); // 006914FA..00691522
    frame_LogStringDtor_005e5c20(&column, nullptr);                   // 00691527..00691535
    if (found != 0) st<U16>(self, 0x12C, static_cast<U16>(value));    // 0069153A..00691543
}

// 006913B0 CSWCTrigger::UpdateTriggerColor(DWORD unused): highlight colour of the AurPart trigger [+138h] (render
// core): area transitions (+108h) keep theirs; hostile (+10Ch) blue (0,0,1), else (0.3,1,1); alpha 0.25 while
// displayed (+110h). RET 4.
void __fastcall recovered_client_trigger_update_color_006913b0(void* self, void*, U /*unused*/) noexcept(false) {
    if (rd32(self, 0x108) != 0) return;                               // 006913B6..006913BE
    float color[3];                                                   // {E-0xc}..{E-0x4}
    color[2] = 1.0f;                                                  // 006913D6
    if (rd32(self, 0x10C) != 0) { color[0] = 0.0f; color[1] = 0.0f; } // 006913E2..006913EC
    else { color[1] = 1.0f; color[0] = fbits(0x3E99999Au); }          // 0069140F..00691417
    const U alpha = rd32(self, 0x110) != 0 ? 0x3E800000u : 0u;        // 006913F6 / 00691427 / 006913FD / 00691434
    recovered_aur_part_trigger_set_highlight_params_0049d9b0(rp(self, 0x138), color, alpha, 0, 0, 0);   // 00691436..00691442
    recovered_aur_part_trigger_set_highlight_0049e610(rp(self, 0x138), 1, ld<float>(self, 0x118), 1);   // 00691447..0069145F
}

// 00691470 CSWCTrigger::DisplayTrigger(BOOL). RET 4.
void __fastcall recovered_client_trigger_display_00691470(void* self, void*, U on) noexcept(false) {
    if (on == rd32(self, 0x110)) return;                              // 00691470..0069147A
    st<U>(self, 0x110, on);                                           // 0069147C
    U arg;
    if (rd32(self, 0x108) != 0) arg = rd32(self, 0x114) != 0 ? 0u : 1u;   // 00691482..00691496 / 006914B5
    else arg = rd32(self, 0x10C) != 0 ? 2u : 1u;                     // 006914A3..006914B5
    recovered_client_trigger_update_color_006913b0(self, nullptr, arg);   // 0069149E / 006914BD (tail jumps)
}

// 00691CC0 CSWCTrigger::AddToRenderList(DWORD colour): appends the id to the area's trigger id list (+11Ch), creates
// the AurPart trigger from the geometry (+134h points, +130h count) in the area scene, then colours it. RET 4.
void __fastcall recovered_client_trigger_add_to_render_list_00691cc0(void* self, void*, U color) noexcept(false) {
    void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 00691CC0..00691CD5
    void* const list = at(area, 0x11C);                               // 00691CE7 (esi)
    const U id = rd32(self, 4);                                       // 00691CE4
    if (rd32(list, 4) == rd32(list, 8)) {                             // 00691CD8..00691CEF
        const U capacity = rd32(list, 8) != 0 ? rd32(list, 8) * 2 : 0x10;   // 00691CF1..00691CF9
        menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr, capacity);   // 00691CFE..00691D01
    }
    st<U>(rp(list, 0), rd32(list, 4) * 4, id);                        // 00691D06..00691D0B
    st<U>(list, 4, rd32(list, 4) + 1);                                // 00691D0E
    void* const scene = rp(rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48), 0x184);   // 00691D11..00691D21
    U zero[4] = {0, 0, 0, 0};                                         // {E-0x10}..{E-0x4}
    void* const part = recovered_aur_part_trigger_create_0049e630(scene, rp(self, 0x134), rd32(self, 0x130), 1, zero,
                                                                  self);   // 00691D27..00691D50
    st<void*>(self, 0x138, part);                                     // 00691D5F
    recovered_client_trigger_update_color_006913b0(self, nullptr, color);   // 00691D55..00691D65
}

// 00691E00 CSWCTrigger::SpecialDisplayTrigger(BOOL show_all). Area transitions are left alone; others get a render
// part on demand and a highlight (hostile blue 0.25, else (1,1,0) 0.35; alpha 0 unless show_all or displayed). RET 4.
void __fastcall recovered_client_trigger_special_display_00691e00(void* self, void*, U all) noexcept(false) {
    float color[3] = {0.0f, 0.0f, 0.0f};                              // 00691E0E..00691E1E {E-0xc}
    if (rd32(self, 0x108) != 0) return;                               // 00691E06..00691E26
    U alpha;                                                          // {E-0x10}
    if (rd32(self, 0x10C) != 0) {                                     // 00691E2C..00691E3A
        if (rp(self, 0x138) == nullptr) recovered_client_trigger_add_to_render_list_00691cc0(self, nullptr, 2);   // 00691E3C..00691E42
        color[0] = 0.0f; color[1] = 0.0f; color[2] = 1.0f;            // 00691E47..00691E57
        alpha = 0x3E800000u;                                          // 00691E5F
    } else {
        if (rp(self, 0x138) == nullptr) recovered_client_trigger_add_to_render_list_00691cc0(self, nullptr, 1);   // 00691E69..00691E71
        color[0] = 1.0f; color[1] = 1.0f; color[2] = 0.0f;            // 00691E76..00691E86
        alpha = 0x3EB33333u;                                          // 00691E8E
    }
    if (all == 0 && rd32(self, 0x110) == 0) alpha = 0;                // 00691E96..00691EA8
    recovered_aur_part_trigger_set_highlight_params_0049d9b0(rp(self, 0x138), color, alpha, 0, 0, 0);   // 00691EB0..00691EC7
    recovered_aur_part_trigger_set_highlight_0049e610(rp(self, 0x138), 1, ld<float>(self, 0x118), 1);   // 00691ECC..00691EE4
}

// 0064F4B0 CSWCMessage::HandleServerToPlayerTriggerUpdate_Add. Record (after 'A' + type 7): OBJECTID, LocString name,
// BOOL area transition, BOOL hostile, BYTE cursor, [BOOL +114h, BYTE transition row when area transition], FLOAT
// highlight width, LocString +140h, BYTE point count, count x 3 FLOAT (z + 0.05). ECX this, RET; EAX 1, or 0 on overflow
// or AddExternalObject failure (the trigger is not freed then, as in the original).
U __fastcall recovered_client_handle_trigger_add_0064f4b0(void* self, void*) {
    ExoString name;                                                   // {esp+4Ch}
    exo_init(&name);                                                  // 0064F4D4..0064F4F0
    ExoString text;                                                   // {esp+44h}
    exo_init(&text);                                                  // 0064F4F7..0064F4FF
    U field_114 = 0;                                                  // 0064F50B {esp+18h}
    U id = read_id(self);                                             // 0064F504..0064F519 {esp+14h}
    {
        ResRef sound;                                                 // {esp+34h}
        kotor_fastcall_00405ed0(&sound, nullptr);                     // 0064F514..0064F51D
        ExoString read;                                               // {esp+54h}
        kotor_fastcall_00692660(self, nullptr, &read, &sound, nullptr);   // 0064F522..0064F52A
        async_assign_string_005e5c50(&name, nullptr, &read);          // 0064F52F..0064F53C
        frame_LogStringDtor_005e5c20(&read, nullptr);                 // 0064F541..0064F54A
    }
    const U transition = read_bool(self);                             // 0064F54F..0064F55A (ebp, {esp+30h})
    const U hostile = read_bool(self);                                // 0064F558..0064F567 {esp+20h}
    const U8 cursor = read_byte(self);                                // 0064F563..0064F572 {esp+28h}
    U8 transition_row = 0;                                            // {esp+12h} (uninitialised in the original)
    if (transition != 0) {                                            // 0064F570..0064F576
        field_114 = read_bool(self);                                  // 0064F578..0064F583
        transition_row = read_byte(self);                             // 0064F57F..0064F58C
    }
    const float width = read_float(self);                             // 0064F590..0064F59E {esp+2Ch}
    {
        ResRef sound;                                                 // {esp+34h}
        kotor_fastcall_00405ed0(&sound, nullptr);                     // 0064F5A2..0064F5A7
        ExoString read;                                               // {esp+54h}
        kotor_fastcall_00692660(self, nullptr, &read, &sound, nullptr);   // 0064F5AC..0064F5B4
        async_assign_string_005e5c50(&text, nullptr, &read);          // 0064F5B9..0064F5C6
        frame_LogStringDtor_005e5c20(&read, nullptr);                 // 0064F5CB..0064F5D4
    }
    const U8 count = read_byte(self);                                 // 0064F5D9..0064F5E6 {esp+13h}
    U result = 0;
    if (!overflow(self)) {                                            // 0064F5E2..0064F5F1
        void* const objects = kotor_fastcall_005ed570(client_app(), nullptr);   // 0064F5F7..0064F605 {esp+24h}
        void* const area = rp(kotor_fastcall_005ed530(client_app(), nullptr), 0x48);   // 0064F609..0064F61E {esp+1Ch}
        void* const memory = live_operator_new(0x14C);                // 0064F619..0064F622
        void* const trigger = memory != nullptr ? recovered_client_trigger_construct_006911e0(memory, nullptr) : nullptr;   // 0064F62A..0064F642 (esi)
        using LoadModel = U (__fastcall*)(void*, void*, void*, U, U);
        if (transition != 0) {                                        // 0064F644..0064F64B
            ExoString model;                                          // {esp+34h}
            exo_init(&model);                                         // 0064F651..0064F655
            ExoString column;                                         // {esp+54h}
            frame_LogStringCtor_005e5a90(&column, nullptr, kModel_007499a0);   // 0064F65A..0064F66B
            const int found = c2da_current_get_string_00413270(rp(rp(rules(), 0xB8), 0x8C), nullptr, transition_row,
                                                               &column, &model);   // 0064F670..0064F69C
            frame_LogStringDtor_005e5c20(&column, nullptr);           // 0064F6A7..0064F6B2
            if (found == 0) c2da_assign_text_005e5140(&model, nullptr, kMnFrag_00751cf0);   // 0064F6A1..0064F6C4
            ResRef ref;                                               // {esp+54h}
            async_ref_construct_string_00406d60(&ref, nullptr, &model);   // 0064F6C9..0064F6D2
            vslot<LoadModel>(trigger, 0x78)(trigger, nullptr, &ref, 0xFF, 9);   // 0064F6D7..0064F6E7
            frame_LogStringDtor_005e5c20(&model, nullptr);            // 0064F6EA..0064F6F3
        } else {
            ResRef ref;                                               // {esp+54h}
            reinterpret_cast<menu_resources::Ref*>(&ref)->construct_00406d80(kEmpty_00751ce8);   // 0064F6FA..0064F703
            vslot<LoadModel>(trigger, 0x78)(trigger, nullptr, &ref, 0xFF, 9);   // 0064F708..0064F718
        }
        vslot<void (__fastcall*)(void*, void*, U)>(trigger, 0x04)(trigger, nullptr, id);   // 0064F71B..0064F724 SetId
        vslot<void (__fastcall*)(void*, void*, void*)>(trigger, 0x84)(trigger, nullptr, area);   // 0064F727..0064F730 SetArea
        if (recovered_object_array_add_external_004d7ff0(objects, nullptr, &id, trigger, 1) == kGameObjectOk_0074666c) {   // 0064F736..0064F751
            recovered_client_ai_master_add_object_00604a20(
                kotor_m15e_client_get_client_ai_master_005ed6b0(client_app(), nullptr), nullptr, trigger);   // 0064F757..0064F768
            async_assign_string_005e5c50(at(trigger, 0x100), nullptr, &name);   // 0064F76D..0064F778
            if (transition != 0) {                                    // 0064F77D..0064F77F
                st<U>(trigger, 0x108, 1);                             // 0064F785
                st<U8>(trigger, 0x148, transition_row);               // 0064F78B
            } else if (hostile != 0) {                                // 0064F793..0064F799
                st<U>(trigger, 0x10C, 1);                             // 0064F79B
            }
            st<U>(trigger, 0x114, field_114);                         // 0064F7A5..0064F7AC
            recovered_client_trigger_set_cursor_006914d0(trigger, nullptr, cursor);   // 0064F7A1..0064F7B2
            const U n = count;                                        // 0064F7B7 (ebx)
            st<U>(trigger, 0x118, bitsf(width));                      // 0064F7C4..0064F7C8
            if (static_cast<I32>(rd32(trigger, 0x130)) < static_cast<I32>(n)) {   // 0064F7BC..0064F7CE
                void* const points = live_operator_new(n * 12u);      // 0064F7D0..0064F7D7
                if (points != nullptr)                                // 0064F7E5..0064F7EC
                    vector_constructor_iterator_00401130(points, 0xC, n, &vector_default_construct_00409e70);   // 0064F7EE..0064F7F7
                st<void*>(trigger, 0x134, points);                    // 0064F805
                st<U>(trigger, 0x130, n);                             // 0064F80B
            }
            bool ok = true;
            for (U i = 0; i < n; ++i) {                               // 0064F811..0064F893
                const float x = read_float(self);                     // 0064F820..0064F82E {esp+64h}
                const float y = read_float(self);                     // 0064F832..0064F840 {esp+68h}
                const float z = static_cast<float>(static_cast<double>(read_float(self)) +
                                                   static_cast<double>(fbits(kZOffset_3d4ccccd)));   // 0064F844..0064F85C fadd [007415BC]
                if (overflow(self)) { ok = false; break; }            // 0064F858..0064F867
                unsigned char* const p = static_cast<unsigned char*>(rp(trigger, 0x134)) + i * 12u;   // 0064F869..0064F877
                st<float>(p, 0, x); st<float>(p, 4, y); st<float>(p, 8, z);   // 0064F879..0064F88C
            }
            if (ok) {
                if (transition != 0) {                                // 0064F895..0064F89D
                    vslot<U (__fastcall*)(void*, void*, void*)>(trigger, 0x94)(trigger, nullptr, word_ptr(rd32(area, 0x184)));   // 0064F89F..0064F8AC AttachToScene
                } else if (hostile != 0) {                            // 0064F8E8..0064F8EE
                    recovered_client_trigger_add_to_render_list_00691cc0(trigger, nullptr, 2);   // 0064F8F0..0064F8F2
                    recovered_client_trigger_display_00691470(trigger, nullptr, 1);   // 0064F8F7..0064F8FB
                } else {
                    recovered_client_trigger_add_to_render_list_00691cc0(trigger, nullptr, 1);   // 0064F902..0064F904
                }
                recovered_client_trigger_special_display_00691e00(trigger, nullptr, recovered_client_show_all_triggers_0083285c);   // 0064F909..0064F911
                async_assign_string_005e5c50(at(trigger, 0x140), nullptr, &text);   // 0064F916..0064F921
                result = 1;                                           // 0064F94A
            }
        }
    }
    frame_LogStringDtor_005e5c20(&text, nullptr);                     // 0064F8B4..0064F8BD / 0064F926..0064F92F
    frame_LogStringDtor_005e5c20(&name, nullptr);                     // 0064F8C2..0064F8CE / 0064F934..0064F940
    return result;
}

}  // extern "C"
