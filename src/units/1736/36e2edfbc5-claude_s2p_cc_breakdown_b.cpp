// Lane worker "s2p-cc" (fork breakdown-b): HandleServerToPlayerCCMessage minors 23, 25, 26, 27.
//   23 damage breakdown       (CSWSCreature::BroadcastDamageBreakdown 004ECCA0)            006619E1..00662762
//   25 effect-application bd  (CSWSCreature::BroadcastEffectApplicationBreakdown 004ECCE0) 00662767..00662A0B
//   26 deflect breakdown      (CSWSCreature::BroadcastDeflectBreakdown 004ECD20)           00662A10..00662D9F
//   27 poisoned               (CSWSCreature::BroadcastPoisoned)                             00662DA4..00662EE4
// All four leave through 00662EE4 (EAX = 0 -> kFail). Frame/exit model: claude_s2p_cc.hpp.
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
// accum = accum + CExoString(sep) + Fetch(strref)   (the token-0 / separator / Fetch / two operator+ blocks of minor 23)
// Order: SetCustomToken(0, CExoString(value)); CExoString sep(text); F = Fetch(strref,0,0,0); T1 = accum + sep;
// T2 = T1 + F; accum = T2; ~T2; ~T1; ~F; ~sep.
void append_value_block(ExoString& accum, I value, const char* sep, U strref) {
    token_int(0, value);
    Tmp s;
    (void)s.set(from_text(s.p(), sep));
    Tmp fe;
    void* fr = fe.set(fetch(fe.p(), strref));
    Tmp t1;
    void* r1 = t1.set(concat(t1.p(), &accum, s.p()));
    Tmp t2;
    str_assign(&accum, t2.set(concat(t2.p(), r1, fr)));
}
}  // namespace

// -----------------------------------------------------------------------------------------------------------------
// minor 23 (006619E1..00662762): damage breakdown "<total> = <type> + <type> ... <modifiers>", TLK 0xA4A6.
// No overflow check; always exits through 00662EE4 (kFail).
// -----------------------------------------------------------------------------------------------------------------
U case_23(Frame& f) {
    I damage[15] = {};                                               // 006619E1..006619FA [esp+128] (15 dwords zeroed)
    U first = 1;                                                     // 00661A00 [esp+2C] = 1 (later loaded into EDI)
    ExoString accum;                                                 // 006619FC..00661A08 [esp+7C]
    for (I i = 0; i < 14; ++i) {                                     // 00661A18..00661A58
        if (i == 0 || i == 1 || i == 2) {                            // 00661A20..00661A2C
            const I v = static_cast<I>(rd_dword(f, 0x20));           // 00661A40..00661A44
            if (v > 0) damage[2] += v;                               // 00661A49..00661A4D (indices 0..2 sum into [2])
        } else {
            damage[i] = static_cast<I>(rd_dword(f, 0x20));           // 00661A2E..00661A37
        }
    }
    const I total = static_cast<I>(rd_dword(f, 0x20));               // 00661A5A..00661A67 [esp+3C]
    (void)rd_dword(f, 0x20);                                         // 00661A63..00661A6B (discarded)
    const I bonus_a4aa = static_cast<I>(rd_dword(f, 0x20));          // 00661A70..00661A7D [esp+1C]
    (void)rd_dword(f, 0x20);                                         // 00661A79..00661A81 (discarded)
    const I bonus_a4ab = static_cast<I>(rd_dword(f, 0x20));          // 00661A86..00661A93 [esp+34]
    (void)rd_dword(f, 0x20);                                         // 00661A8F..00661A97 (discarded)
    const I bonus_a4ac = static_cast<I>(rd_dword(f, 0x20));          // 00661A9C..00661AA9 [esp+4C]
    const I bonus_a57b = static_cast<I>(rd_dword(f, 0x20));          // 00661AA5..00661AB6 [esp+24]
    const I critical = static_cast<I>(rd_dword(f, 0x20));            // 00661AB2..00661AC3 [esp+94]
    const I minus_a5c1 = static_cast<I>(rd_dword(f, 0x20));          // 00661ABF..00661AD3 [esp+50]
    const I minus_a5c2 = static_cast<I>(rd_dword(f, 0x20));          // 00661ACF..00661AE0 [esp+74]
    (void)rd_dword(f, 0x20);                                         // 00661ADC..00661AE4 (discarded)
    (void)rd_dword(f, 0x20);                                         // 00661AE9..00661AED (discarded)
    (void)rd_dword(f, 0x20);                                         // 00661AF2..00661AF6 (discarded)
    U edi = first;                                                   // 00661AFB
    for (I i = 0; i < 14; ++i) {                                     // 00661AFF..00661DD7
        U strref;                                                    // 00661B01..00661B68 table 00663154 -> ESI
        switch (i) {
        case 0: case 1: case 2: strref = 0x58f; break;
        case 3: strref = 0x58e; break;
        case 4: strref = 0x5a0; break;
        case 5: strref = 0x5a1; break;
        case 6: strref = 0x5a2; break;
        case 7: strref = 0x5a3; break;
        case 8: strref = 0x5a4; break;
        case 9: strref = 0x5a5; break;
        case 10: strref = 0x5a6; break;
        case 11: strref = 0x5a7; break;
        case 12: strref = 0x5a8; break;
        case 13: strref = 0xa3ae; break;
        default: strref = 0; break;                                  // 00661B68 (unreachable: index < 14)
        }
        const I v = damage[i];                                       // 00661B04
        if (v <= 0) continue;                                        // 00661B6A..00661B6C
        token_int(0, v);                                             // 00661B72..00661BAB
        Tmp fe;                                                      // [esp+1D4]
        (void)fe.set(fetch(fe.p(), strref));                         // 00661BB0..00661BC5
        if (i == 12 || i == 0 || i == 1 || i == 2) {                 // 00661BCA..00661BEF
            if (edi != 0) {                                          // 00661CD9..00661CDB
                edi = 0;                                             // 00661D8F
                Tmp t;                                               // [esp+10FC]
                str_assign(&accum, t.set(concat(t.p(), &accum, fe.p())));   // 00661D78..00661DB2 accum = accum + F
            } else {                                                 // 00661CE1..00661D76 accum = F + " + " + accum
                Tmp s;                                               // [esp+54C]
                (void)s.set(from_text(s.p(), " + "));               // 00661CE1..00661CED (0x751C5C)
                Tmp t1;                                              // [esp+10EC]
                void* r1 = t1.set(concat(t1.p(), fe.p(), s.p()));    // 00661CF2..00661D11
                Tmp t2;                                              // [esp+10DC]
                str_assign(&accum, t2.set(concat(t2.p(), r1, &accum))); // 00661D16..00661D42
            }
        } else if (edi != 0) {                                       // 00661BF5..00661BF7
            edi = 0;                                                 // 00661CB1
            Tmp t;                                                   // [esp+10CC]
            str_assign(&accum, t.set(concat(t.p(), &accum, fe.p()))); // 00661C9A..00661CD4 accum = accum + F
        } else {                                                     // 00661BFD..00661C95 accum = accum + " + " + F
            Tmp s;                                                   // [esp+53C]
            (void)s.set(from_text(s.p(), " + "));                   // 00661BFD..00661C09 (0x751C5C)
            Tmp t1;                                                  // [esp+10BC]
            void* r1 = t1.set(concat(t1.p(), &accum, s.p()));        // 00661C0E..00661C2D
            Tmp t2;                                                  // [esp+10AC]
            str_assign(&accum, t2.set(concat(t2.p(), r1, fe.p())));  // 00661C32..00661C61
        }
    }                                                                // 00661DB2..00661DCE ~temps, ~F
    if (total > 0) {                                                 // 00661DDD..00661DE3 (jle -> 0066274E)
        if (bonus_a4aa != 0) {                                       // 00661DE9..00661DEF
            StrRes unused;                                           // 00661DF5 [esp+1468]
            append_value_block(accum, bonus_a4aa, " ", 0xa4aa);      // 00661E01..00661F20 (0x73D5D8)
        }                                                            // 00661F25..00661F34
        if (bonus_a57b != 0) {                                       // 00661F39..00661F3F
            append_value_block(accum, bonus_a57b, " ", 0xa57b);      // 00661F45..0066205A (0x73D5D8)
        }
        if (bonus_a4ab > 0) {                                        // 0066205F..00662065
            StrRes unused;                                           // 0066206B [esp+1414]
            append_value_block(accum, bonus_a4ab, " + ", 0xa4ab);    // 00662077..00662196 (0x751C5C)
        }                                                            // 0066219B..006621AA
        if (bonus_a4ac > 0) {                                        // 006621AF..006621B5
            StrRes unused;                                           // 006621BB [esp+1430]
            append_value_block(accum, bonus_a4ac, " + ", 0xa4ac);    // 006621C7..006622E5 (0x751C5C)
        }                                                            // 006622EA..006622F9
        if (minus_a5c1 != 0) {                                       // 006622FE..00662304
            StrRes unused;                                           // 0066230A [esp+144C]
            append_value_block(accum, minus_a5c1, " - ", 0xa5c1);    // 00662316..00662434 (0x748E98)
        }                                                            // 00662439..00662448
        if (minus_a5c2 != 0) {                                       // 0066244D..00662453
            StrRes unused;                                           // 00662459 [esp+1484]
            append_value_block(accum, minus_a5c2, " - ", 0xa5c2);    // 00662465..00662583 (0x748E98)
        }                                                            // 00662588..00662597
        ExoString head;                                              // 0066259C..006625A3 [esp+1E4]
        if (critical > 0) {                                          // 006625A8..006625B9
            token_int(0, critical);                                  // 006625BF..006625F8
            assign_fetch(&head, 0xa592);                             // 006625FD..0066263F
        }
        {
            Tmp n;                                                   // [esp+124C]
            void* rn = n.set(from_int(n.p(), total));                // 00662644..00662650
            Tmp t;                                                   // [esp+123C]
            str_assign(&head, t.set(concat(t.p(), &head, rn)));      // 00662655..00662683 head = head + CExoString(total)
        }                                                            // 00662688..006626AA
        token(0, &head);                                             // 006626AF..006626BF
        token(1, &accum);                                            // 006626C4..006626D1
        assign_fetch(&f.a, 0xa4a6);                                  // 006626D6..00662715
        append_a(f);                                                 // 0066271A..00662735
    }                                                                // 0066273A..00662749 ~head
    return kFail;                                                    // 0066274E..00662762 ~accum; jmp 00662EE4
}

// -----------------------------------------------------------------------------------------------------------------
// minor 25 (00662767..00662A0B): effect-application breakdown "<name>: <result> <effect type>", TLK 0xA4AD/0xA4AE..0xA4B0.
// -----------------------------------------------------------------------------------------------------------------
U case_25(Frame& f) {
    const U id = rd_obj(f);                                          // 00662767..00662772 edi
    (void)rd_dword(f, 0x20);                                         // 0066276E..00662774 (discarded)
    const U kind = rd_dword(f, 0x20);                                // 00662779..00662786 ebx
    (void)rd_dword(f, 0x20);                                         // 00662782..00662788 (discarded)
    (void)rd_dword(f, 0x20);                                         // 0066278D..00662791 (discarded)
    (void)rd_dword(f, 0x20);                                         // 00662796..0066279A (discarded)
    (void)rd_dword(f, 0x20);                                         // 0066279F..006627A3 (discarded)
    (void)rd_dword(f, 0x20);                                         // 006627A8..006627AC (discarded)
    (void)rd_dword(f, 0x20);                                         // 006627B1..006627B5 (discarded)
    const U result = rd_dword(f, 0x20);                              // 006627BA..006627C3 esi
    if (result == 0xffffffffu) return kFail;                         // 006627C5..006627C8
    if (result == 1) return kFail;                                   // 006627CE..006627D1
    ExoString prefix;                                                // 006627D7..006627DE [esp+294]
    assign_fetch(&prefix, 0xa4ad);                                   // 006627E3..00662830
    object_name_or_gui(id, &f.b);                                    // 00662835..00662885
    token(0, &f.b);                                                  // 0066288A..00662897
    U strref;                                                        // 0066289C..006628B5 esi
    if (result == 0) strref = 0xa4ae;
    else if (result == 1) strref = 0xa4af;                           // (unreachable: rejected above)
    else if (result == 2) strref = 0xa4b0;
    else strref = 0xa4ae;
    if (kind != 0) {                                                 // 006628BA..006628BE
        U type_ref = 0;                                              // 006628BA eax
        switch (kind - 1) {                                          // 006628C4..006628CC table 0066318C (unsigned > 9 -> 0)
        case 0: type_ref = 0xa42f; break;
        case 1: type_ref = 0xa433; break;
        case 2: type_ref = 0xa434; break;
        case 3: type_ref = 0xa430; break;
        case 4: type_ref = 0xa435; break;
        case 5: type_ref = 0xa436; break;
        case 6: type_ref = 0xa431; break;
        case 7: type_ref = 0xa438; break;
        case 8: type_ref = 0xa432; break;
        case 9: type_ref = 0xa437; break;
        default: break;
        }
        Tmp t;                                                       // [esp+62C]
        (void)t.set(fetch(t.p(), type_ref));                         // 00662917..0066292C
        token(1, t.p());                                             // 00662931..00662949
    }                                                                // 0066294E..0066295D
    {
        Tmp fe;                                                      // [esp+129C]
        void* fr = fe.set(fetch(fe.p(), strref));                    // 00662962..00662977
        Tmp t;                                                       // [esp+128C]
        str_assign(&f.a, t.set(concat(t.p(), &prefix, fr)));         // 0066297C..006629A7 a = prefix + F
    }                                                                // 006629AC..006629CE
    append_a(f);                                                     // 006629D3..006629EF
    return kFail;                                                    // 006629F4..00662A0B ~prefix; jmp 00662EE4
}

// -----------------------------------------------------------------------------------------------------------------
// minor 26 (00662A10..00662D9F): deflect breakdown, TLK 0xA5B1 with token 2 = composed modifier list.
// -----------------------------------------------------------------------------------------------------------------
U case_26(Frame& f) {
    const U id = rd_obj(f);                                          // 00662A10..00662A1B [esp+70]
    (void)rd_dword(f, 0x20);                                         // 00662A17..00662A1F (discarded)
    const U roll = rd_dword(f, 0x20);                                // 00662A24..00662A31 [esp+78]
    const U total = rd_dword(f, 0x20);                               // 00662A2D..00662A3E [esp+1C]
    const U base = rd_dword(f, 0x20);                                // 00662A3A..00662A4B ebx
    const U feat_bonus = rd_dword(f, 0x20);                          // 00662A47..00662A56 edi
    const U feat = rd_dword(f, 0x20);                                // 00662A52..00662A61 [esp+2C]
    const U part_a5b3 = rd_dword(f, 0x20);                           // 00662A5D..00662A6E [esp+94]
    const U part_a5b4 = rd_dword(f, 0x20);                           // 00662A6A..00662A7E [esp+98]
    const U part_a67c = rd_dword(f, 0x20);                           // 00662A7A..00662A8A [esp+B4]
    if (total == 0) return kFail;                                    // 00662A91..00662A97
    ExoString parts;                                                 // 00662A9D..00662AA4 [esp+120]
    (void)kotor_fastcall_006500b0(f.msg, nullptr, static_cast<I>(base), 0xa54c, &parts); // 00662AA9..00662AC4
    if (feat_bonus != 0) {                                           // 00662AC9..00662ACD
        {
            void* feat_ptr = kotor_rules_get_feat_00550c00(rules(), nullptr, static_cast<unsigned short>(feat)); // 00662AD3..00662AE6
            Tmp n;                                                   // [esp+12AC]
            token(0, n.set(kotor_m15b_feat_get_name_text_005cd760(feat_ptr, nullptr, n.p())));  // 00662AEB..00662B03
        }                                                            // 00662B08..00662B16
        {
            Tmp t;                                                   // [esp+63C]
            (void)t.set(from_int(t.p(), static_cast<I>(feat_bonus))); // 00662B1B..00662B23
            token(1, t.p());                                         // 00662B28..00662B40
        }                                                            // 00662B45..00662B53
        Tmp fe;                                                      // [esp+12CC]
        void* fr = fe.set(fetch(fe.p(), 0xa54e));                    // 00662B58..00662B71
        Tmp t;                                                       // [esp+12BC]
        str_assign(&parts, t.set(concat(t.p(), &parts, fr)));        // 00662B76..00662BA3 parts = parts + F
    }                                                                // 00662BA8..00662BCA
    (void)kotor_fastcall_006500b0(f.msg, nullptr, static_cast<I>(part_a5b3), 0xa5b3, &parts); // 00662BCF..00662BE6
    (void)kotor_fastcall_006500b0(f.msg, nullptr, static_cast<I>(part_a5b4), 0xa5b4, &parts); // 00662BEB..00662C02
    (void)kotor_fastcall_006500b0(f.msg, nullptr, static_cast<I>(part_a67c), 0xa67c, &parts); // 00662C07..00662C1E
    object_name_or_gui(id, &f.b);                                    // 00662C23..00662C76
    token(0, &f.b);                                                  // 00662C7B..00662C88
    {
        Tmp t;                                                       // [esp+64C]
        (void)t.set(from_int(t.p(), static_cast<I>(total)));         // 00662C8D..00662C99
        token(1, t.p());                                             // 00662C9E..00662CB6
    }                                                                // 00662CBB..00662CC9
    token(2, &parts);                                                // 00662CCE..00662CDE
    {
        Tmp t;                                                       // [esp+65C]
        (void)t.set(from_int(t.p(), static_cast<I>(roll)));          // 00662CE3..00662CEF
        token(3, t.p());                                             // 00662CF4..00662D0C
    }                                                                // 00662D11..00662D1F
    assign_fetch(&f.a, 0xa5b1);                                      // 00662D24..00662D62
    append_a(f);                                                     // 00662D67..00662D83
    return kFail;                                                    // 00662D88..00662D9F ~parts; jmp 00662EE4
}

// -----------------------------------------------------------------------------------------------------------------
// minor 27 (00662DA4..00662EE4): poisoned "<name> <poison>", TLK 0xBB01; falls into 00662EE4 (kFail).
// -----------------------------------------------------------------------------------------------------------------
U case_27(Frame& f) {
    void* app = client();                                            // 00662DA4..00662DAA edi (loaded before the read)
    const U id = rd_obj(f);                                          // 00662DAD..00662DB4
    const U named = kotor_fastcall_005ed350(app, nullptr, id, &f.b); // 00662DB9..00662DBC
    if (named == 0) assign_gui(&f.b, 0);                             // 00662DC1..00662E1E (push 0)
    token(0, &f.b);                                                  // 00662DC5..00662DD2 / 00662E23..00662E30
    const U strref = rd_dword(f, 0x20);                              // 00662E35..00662E39
    {
        Tmp s;                                                       // [esp+130C]
        token(1, s.set(simple_string(s.p(), strref)));               // 00662E3E..00662E66
    }                                                                // 00662E6B..00662E79
    assign_fetch(&f.a, 0xbb01);                                      // 00662E7E..00662EBF
    append_a(f);                                                     // 00662EC4..00662EDF
    return kFail;                                                    // 00662EE4
}
}  // namespace s2pcc
