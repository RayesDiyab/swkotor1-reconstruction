// Lane worker "s2p-cc" (group misc): CSWCMessage::HandleServerToPlayerCCMessage 00656D20 minors 12, 13, 14, 15, 16.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses.
// Frame/exit model: claude_s2p_cc.hpp. Senders: 13 CSWVirtualMachineCommands::ExecuteCommandDoTouchAttack,
// 15 CSWSEffectListHandler::OnApplyDispel{All,Best}Magic, 16 CSWSCreature::BroadcastStealthDetectionData; 12 and 14
// have no K1 server sender.
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
inline I sbyte(U8 v) { return static_cast<I>(static_cast<std::int8_t>(v)); }
inline I sword(U v) { return static_cast<I>(static_cast<std::int16_t>(static_cast<U16>(v))); }
inline void format3(void* out, const char* fmt, I a, I b, I c) {
    kotor_reconstruction::StartupFormat005e5680(ss(out), fmt, a, b, c);
}
// minor 16 block: SetCustomToken(0, CExoString(value)); a = (a + " + ") + Fetch(strref)
inline void plus_fetch(Frame& f, I value, U strref) {
    token_int(0, value);                                             // movsx; CExoString(int); SetCustomToken(0); ~
    Tmp plus;                                                        // [esp+2CC..31C] CExoString(" + ") (.rdata 00751C5C)
    (void)plus.set(from_text(plus.p(), " + "));
    Tmp fetched;                                                     // Fetch(strref, 0, 0, 0) -> ESI
    void* rhs = fetched.set(fetch(fetched.p(), strref));
    Tmp t1;                                                          // a + " + "
    void* lhs = t1.set(concat(t1.p(), &f.a, plus.p()));
    Tmp t2;                                                          // (a + " + ") + fetched
    str_assign(&f.a, t2.set(concat(t2.p(), lhs, rhs)));
}                                                                    // ~t2, ~t1, ~fetched, ~plus
}  // namespace

// -----------------------------------------------------------------------------------------------------------------
// minor 12 (0065F368..0065F567): "<simple 0xA416><" %d (%d +/- %d "><simple 0xA418>)" roll line. No K1 sender.
// -----------------------------------------------------------------------------------------------------------------
U case_12(Frame& f) {
    Tmp text;                                                        // [esp+A8C] ReadCExoString(0x20) (state 1BCh)
    (void)text.set(kotor_fastcall_004d67e0(f.msg, nullptr, text.p(), 0x20));   // 0065F368..0065F374
    const U ebx = rd_dword(f, 0x20);                                 // 0065F379..0065F391
    const U edi = rd_dword(f, 0x20);                                 // 0065F38D..0065F39C
    (void)rd_dword(f, 0x20);                                         // 0065F398..0065F39E (value discarded)
    const U esi = edi - ebx;                                         // 0065F3A3..0065F3A5
    if (edi == 0) return kDone;                                      // 0065F3A7..0065F3A9 -> 0065F560 ~text -> 00660377
    ExoString numbers;                                               // 0065F3AF [esp+224]
    {
        // numbers = Format(...); a = ((simple(0xA416) + numbers) + simple(0xA418)) + ")"
        if (static_cast<I>(esi) >= 0) {                              // 0065F3BB..0065F3C5 (jl)
            format3(&numbers, " %d (%d + %d ", static_cast<I>(edi), static_cast<I>(ebx), static_cast<I>(esi)); // 0065F3C7..0065F3F5 (.rdata 00751EDC)
        } else {
            const U mag = (static_cast<U>(static_cast<I>(esi) >> 31) ^ esi) - static_cast<U>(static_cast<I>(esi) >> 31); // 0065F3D9..0065F3DE cdq/xor/sub
            format3(&numbers, " %d (%d - %d ", static_cast<I>(edi), static_cast<I>(ebx), static_cast<I>(mag));  // 0065F3E0..0065F3F5 (.rdata 00751ECC)
        }
        Tmp paren;                                                   // [esp+32C] CExoString(")") (.rdata 0074F788)
        (void)paren.set(from_text(paren.p(), ")"));                  // 0065F3F8..0065F404
        Tmp s1;                                                      // [esp+A7C] GetSimpleString(0xA418) -> ESI
        void* rhs1 = s1.set(simple_string(s1.p(), 0xa418));          // 0065F409..0065F429
        Tmp s2;                                                      // [esp+A6C] GetSimpleString(0xA416)
        void* lhs = s2.set(simple_string(s2.p(), 0xa416));           // 0065F42B..0065F446
        Tmp t3;                                                      // [esp+A5C] s2 + numbers
        void* r3 = t3.set(concat(t3.p(), lhs, &numbers));            // 0065F44B..0065F465
        Tmp t4;                                                      // [esp+A4C] t3 + s1
        void* r4 = t4.set(concat(t4.p(), r3, rhs1));                 // 0065F46A..0065F47E
        Tmp t5;                                                      // [esp+A3C] t4 + ")"
        str_assign(&f.a, t5.set(concat(t5.p(), r4, paren.p())));     // 0065F483..0065F4AF
    }                                                                // 0065F4B4..0065F526 ~t5 ~t4 ~t3 ~s2 ~s1 ~paren
    append_a(f);                                                     // 0065F52B..0065F547 (a, 80h, 0)
    return kDone;                                                    // 0065F54C..0065F567 ~numbers, ~text -> 00660377
}

// -----------------------------------------------------------------------------------------------------------------
// minor 13 (0065BC59..0065C358): touch attack roll line, TLK 0x57D (tokens 0..8). Sender: ExecuteCommandDoTouchAttack.
// -----------------------------------------------------------------------------------------------------------------
U case_13(Frame& f) {
    ExoString sign;                                                  // 0065BC59 [esp+194]
    ExoString attacker;                                              // 0065BC65 [esp+16C]
    ExoString kind;                                                  // 0065BC7C [esp+E8]
    ExoString result;                                                // 0065BC90 [esp+184]
    ExoString never_set;                                             // 0065BCA4 [esp+174] (only used as a token)
    const U attacker_id = rd_obj(f);                                 // 0065BCB8..0065BCC9 [esp+3C]
    const U target_id = rd_obj(f);                                   // 0065BCC7..0065BCD6 [esp+4C]
    const I modifier = sbyte(rd_byte(f, 8));                         // 0065BCD2..0065BCE6 movsx [esp+34]
    const I roll = static_cast<I>(rd_byte(f, 5));                    // 0065BCE2..0065BCF2 movzx [esp+24]
    const U type = rd_byte(f, 4);                                    // 0065BCF6..0065BD01 movzx edi
    const U hit = rd_bool(f);                                        // 0065BCFF..0065BD0D ebx
    U has_value = 0;                                                 // [esp+1C]
    if (overflow(f, 1) != 0) return kFail;                           // 0065BD09..0065BD16 -> 0065BD30..0065BD97
    has_value = rd_bool(f);                                          // 0065BD18..0065BD23
    if (overflow(f, 1) != 0) return kFail;                           // 0065BD1F..0065BD2E -> 0065BD30..0065BD97
    U value = 0;                                                     // esi
    if (has_value == 1) value = rd_byte(f, 7);                       // 0065BD9C..0065BDB1 movzx
    if (hit == 1) assign_gui(&result, 0x582);                        // 0065BDB3..0065BDEF, 0065BE28..0065BE30
    else assign_gui(&result, 0x583);                                 // 0065BDF1..0065BE30
    if (type == 1) {                                                 // 0065BE35..0065BE38
        assign_gui(&kind, 0x55b);                                    // 0065BE3A..0065BE70, 0065BFF8
    } else if (type == 2) {                                          // 0065BE75..0065BE78
        assign_gui(&kind, 0x56e);                                    // 0065BE7A..0065BEB0
    } else if (type == 4) {                                          // 0065BEB5..0065BEB8
        assign_gui(&kind, 0x55d);                                    // 0065BEBA..0065BEF0
    } else if (type == 0x11) {                                       // 0065BEF5..0065BEF8
        token_int(0, static_cast<I>(value));                         // 0065BEFA..0065BF33
        assign_fetch(&kind, 0x58a);                                  // 0065BF38..0065BF72
    } else if (type == 0x10) {                                       // 0065BF77..0065BF7A
        token_int(0, static_cast<I>(value));                         // 0065BF80..0065BFB9
        assign_fetch(&kind, 0x58b);                                  // 0065BFBE..0065BFF1
    }
    U shown = static_cast<U>(modifier);                              // 0065C005 esi
    const U total = static_cast<U>(roll) + shown;                    // 0065C00B..0065C00F edi
    if (static_cast<I>(shown) < 0) {                                 // 0065C009..0065C019
        assign_text(&sign, "-");                                     // 0065C01B..0065C020 (.rdata 0073ED40)
        shown = 0u - shown;                                          // 0065C025
    } else {
        assign_text(&sign, "+");                                     // 0065C029..0065C02E (.rdata 00748ED4)
    }
    void* first_token;
    if (object_name(target_id, &f.b) != 0) {                         // 0065C033..0065C04D
        const U r = object_name(attacker_id, &attacker);             // 0065C04F..0065C06C
        if (r != 0) {
            first_token = &attacker;                                 // 0065C105
        } else {
            assign_gui(&attacker, r);                                // 0065C072..0065C0A5, 0065C142..0065C14A
            first_token = &never_set;                                // 0065C14F (quirk: the empty [esp+174])
        }
    } else {
        assign_gui(&f.b, 0);                                         // 0065C0AA..0065C0E2
        if (object_name(attacker_id, &attacker) != 0) {              // 0065C0E7..0065C103
            first_token = &attacker;                                 // 0065C105
        } else {
            assign_gui(&attacker, 0);                                // 0065C10E..0065C14A
            first_token = &never_set;                                // 0065C14F (quirk)
        }
    }
    token(0, first_token);                                           // 0065C156..0065C15F
    token(2, &f.b);                                                  // 0065C164..0065C171
    token(1, &result);                                               // 0065C176..0065C186
    token(3, &kind);                                                 // 0065C18B..0065C19B
    token_int(4, roll);                                              // 0065C1A0..0065C1D2
    token(5, &sign);                                                 // 0065C1D7..0065C1E7
    token_int(6, static_cast<I>(shown));                             // 0065C1EC..0065C21E
    token_int(7, static_cast<I>(total));                             // 0065C223..0065C255
    { Tmp empty; token(8, empty.set(from_text(empty.p(), ""))); }    // 0065C25A..0065C297 (.rdata 0073D71C)
    assign_fetch(&f.a, 0x57d);                                       // 0065C29C..0065C2DB
    append_a(f);                                                     // 0065C2E0..0065C2FC
    return kDone;                                                    // 0065C301..0065C358 -> 00660377
}

// -----------------------------------------------------------------------------------------------------------------
// minor 14 (0065C35D..0065C5A7): "<name> total = roll +/- mod" line, TLK 0x584. No K1 server sender.
// -----------------------------------------------------------------------------------------------------------------
U case_14(Frame& f) {
    ExoString sign;                                                  // 0065C35D [esp+1EC]
    const U id = rd_obj(f);                                          // 0065C369..0065C37F [esp+1C]
    const I roll = static_cast<I>(rd_byte(f, 5));                    // 0065C37B..0065C38C movzx ebx
    U shown = static_cast<U>(sbyte(rd_byte(f, 8)));                  // 0065C388..0065C398 movsx edi
    if (overflow(f, 1) != 0) return kFail;                           // 0065C394..0065C3BB
    const U total = shown + static_cast<U>(roll);                    // 0065C3C2 esi
    if (static_cast<I>(shown) < 0) {                                 // 0065C3C0..0065C3C5
        assign_text(&sign, "-");                                     // 0065C3C7..0065C3CC
        shown = 0u - shown;                                          // 0065C3D1
    } else {
        assign_text(&sign, "+");                                     // 0065C3D5..0065C3DA
    }
    object_name_or_gui(id, &f.b);                                    // 0065C3DF..0065C433
    token(0, &f.b);                                                  // 0065C438..0065C445
    token_int(1, static_cast<I>(total));                             // 0065C44A..0065C47C
    token_int(2, roll);                                              // 0065C481..0065C4B3
    token(3, &sign);                                                 // 0065C4B8..0065C4C8
    token_int(4, static_cast<I>(shown));                             // 0065C4CD..0065C4FF
    token_int(5, static_cast<I>(total));                             // 0065C504..0065C536
    assign_fetch(&f.a, 0x584);                                       // 0065C53B..0065C57A
    append_a(f);                                                     // 0065C57F..0065C59B
    return kDone;                                                    // 0065C5A0..0065C5A7 ~sign -> 00660377
}

// -----------------------------------------------------------------------------------------------------------------
// minor 15 (0065E143..0065E468): dispel line "<TLK 0> : <name> : <spell>, <spell>...". Sender: OnApplyDispel*Magic.
// -----------------------------------------------------------------------------------------------------------------
U case_15(Frame& f) {
    const U id = rd_obj(f);                                          // 0065E143..0065E14E edi
    const U8 count = rd_byte(f, 8);                                  // 0065E14A..0065E159 [esp+23]
    if (overflow(f, 1) != 0) return kFail;                           // 0065E155..0065E164
    object_name_or_gui(id, &f.b);                                    // 0065E16A..0065E1BF
    token(0, &f.b);                                                  // 0065E1C4..0065E1D1
    {
        Tmp colon1;                                                  // [esp+604] CExoString(" : ") (.rdata 0073E474)
        (void)colon1.set(from_text(colon1.p(), " : "));              // 0065E1D6..0065E1E2
        Tmp colon2;                                                  // [esp+494] CExoString(" : ")
        (void)colon2.set(from_text(colon2.p(), " : "));              // 0065E1E7..0065E1FE
        Tmp t1;                                                      // [esp+73C] Fetch(0, 0, 0, 0)
        void* r1 = t1.set(fetch(t1.p(), 0));                         // 0065E203..0065E221
        Tmp t2;                                                      // [esp+72C] t1 + colon2
        void* r2 = t2.set(concat(t2.p(), r1, colon2.p()));           // 0065E226..0065E240
        Tmp t3;                                                      // [esp+71C] t2 + b
        void* r3 = t3.set(concat(t3.p(), r2, &f.b));                 // 0065E245..0065E25D
        Tmp t4;                                                      // [esp+70C] t3 + colon1
        str_assign(&f.a, t4.set(concat(t4.p(), r3, colon1.p())));    // 0065E262..0065E28E
    }                                                                // 0065E293..0065E308
    for (U8 i = 0; i < count; ++i) {                                 // 0065E30D..0065E31B, 0065E439..0065E441 (unsigned)
        const U spell_id = rd_word(f, 9);                            // 0065E320..0065E32D movzx edi
        if (overflow(f, 1) != 0) return kFail;                       // 0065E329..0065E337
        if (i > 0) {                                                 // 0065E33D..0065E33F
            Tmp comma;                                               // [esp+2C4] CExoString(", ") (.rdata 00748DD4)
            (void)comma.set(from_text(comma.p(), ", "));             // 0065E341..0065E34D
            Tmp t;                                                   // [esp+74C] a + ", "
            str_assign(&f.a, t.set(concat(t.p(), &f.a, comma.p()))); // 0065E352..0065E383
        }                                                            // 0065E388..0065E3AE
        void* spell = kotor_m15b_spell_array_get_spell_0059b6d0(rp(rules(), 0x8c), nullptr, static_cast<I>(spell_id)); // 0065E3B3..0065E3C0
        Tmp name;                                                    // [esp+76C] GetGUIString(spell->+8)
        void* rhs = name.set(gui_string(name.p(), rd(spell, 8)));    // 0065E3C5..0065E3DA
        Tmp t;                                                       // [esp+75C] a + name
        str_assign(&f.a, t.set(concat(t.p(), &f.a, rhs)));           // 0065E3DF..0065E409
    }                                                                // 0065E40E..0065E434
    append_a(f);                                                     // 0065E447..0065E463
    return kDone;                                                    // 0065E468 -> 00660387
}

// -----------------------------------------------------------------------------------------------------------------
// minor 16 (0065E46D..0065F013): stealth detection (TLK 0x9487 + 0xA48B/0xA48A breakdowns).
// Sender: CSWSCreature::BroadcastStealthDetectionData.
// -----------------------------------------------------------------------------------------------------------------
U case_16(Frame& f) {
    ExoString name0;                                                 // 0065E46D [esp+1FC]
    ExoString name1;                                                 // 0065E479 [esp+1DC]
    const U id0 = rd_obj(f);                                         // 0065E490..0065E4A1 edi
    const U id1 = rd_obj(f);                                         // 0065E49F..0065E4AA [esp+78]
    const U shown = rd_bool(f);                                      // 0065E4A8..0065E4B7 [esp+70]
    const U w1 = rd_word(f, 0x10);                                   // 0065E4B3..0065E4C4 [esp+3C]
    const U w2 = rd_word(f, 0x10);                                   // 0065E4C0..0065E4D1 [esp+34]
    const U w3 = rd_word(f, 0x10);                                   // 0065E4CD..0065E4DE [esp+48]
    const U w4 = rd_word(f, 0x10);                                   // 0065E4DA..0065E4EB [esp+24]
    const U w5 = rd_word(f, 0x10);                                   // 0065E4E7..0065E4F8 [esp+38]
    const U w6 = rd_word(f, 0x10);                                   // 0065E4F4..0065E505 [esp+1C]
    const U w7 = rd_word(f, 0x10);                                   // 0065E501..0065E512 [esp+40]
    const U w8 = rd_word(f, 0x10);                                   // 0065E50E..0065E51F [esp+74]
    const U w9 = rd_word(f, 0x10);                                   // 0065E51B..0065E52C [esp+50]
    const U w10 = rd_word(f, 0x10);                                  // 0065E528..0065E539 [esp+4C]
    if (overflow(f, 1) != 0) return kFail;                           // 0065E535..0065E571
    ExoString unused;                                                // 0065E576 [esp+9AC]
    (void)object_name(id0, &name0);                                  // 0065E582..0065E59D (result ignored)
    (void)object_name(id1, &name1);                                  // 0065E5A2..0065E5B7 (result ignored)
    if (shown == 0) return kDone;                                    // 0065E5BC..0065E5C2 -> 0065EFE4
    const I sum_a = sword(w1 + w10 + w9 + w8 + w7 + w5 + w3);        // 0065E5C8..0065E5FB, 0065E62C movsx si
    const I sum_b = sword(w2 + w6 + w4);                             // 0065E5FD..0065E606, 0065E665 movsx di
    token(0, &name0);                                                // 0065E600..0065E612
    token(1, &name1);                                                // 0065E617..0065E627
    token_int(2, sum_a);                                             // 0065E62C..0065E660
    token_int(3, sum_b);                                             // 0065E665..0065E699
    assign_fetch(&f.a, 0x9487);                                      // 0065E69E..0065E6DC
    append_a(f, 0x80, 1);                                            // 0065E6E1..0065E6FD (a, 80h, 1)
    token(0, &name0);                                                // 0065E702..0065E712
    token_int(1, sum_a);                                             // 0065E717..0065E748
    token_int(2, sword(w1));                                         // 0065E74D..0065E783
    token_int(3, sword(w3));                                         // 0065E788..0065E7BE
    assign_fetch(&f.a, 0xa48b);                                      // 0065E7C3..0065E801
    if (static_cast<U16>(w5) != 0) plus_fetch(f, sword(w5), 0xa48d); // 0065E806..0065E91B
    if (static_cast<U16>(w7) != 0) plus_fetch(f, sword(w7), 0xa48e); // 0065E920..0065EA34
    if (static_cast<U16>(w8) != 0) plus_fetch(f, sword(w8), 0xa48f); // 0065EA39..0065EB4D
    if (static_cast<U16>(w9) != 0) plus_fetch(f, sword(w9), 0xa490); // 0065EB52..0065EC66
    if (static_cast<U16>(w10) != 0) plus_fetch(f, sword(w10), 0xa491); // 0065EC6B..0065ED7F
    append_a(f);                                                     // 0065ED84..0065EDA0
    token(0, &name1);                                                // 0065EDA5..0065EDB5
    token_int(1, sum_b);                                             // 0065EDBA..0065EDEB
    token_int(2, sword(w2));                                         // 0065EDF0..0065EE26
    token_int(3, sword(w4));                                         // 0065EE2B..0065EE61
    assign_fetch(&f.a, 0xa48a);                                      // 0065EE66..0065EEA4
    // quirk: the test is on w5 ([esp+38]) but the token value is w6 ([esp+1C])
    if (static_cast<U16>(w5) != 0) plus_fetch(f, sword(w6), 0xa48d); // 0065EEA9..0065EFBE
    append_a(f);                                                     // 0065EFC3..0065EFDF
    return kDone;                                                    // 0065EFE4..0065F013 -> 00660377
}
}  // namespace s2pcc
