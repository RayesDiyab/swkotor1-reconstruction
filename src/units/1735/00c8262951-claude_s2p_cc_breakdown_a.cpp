// Lane worker "s2p-cc" (group breakdown_a): 00656D20 CSWCMessage::HandleServerToPlayerCCMessage minors 20 and 22.
//   minor 20 (006603D9..0066112F) attack breakdown  (sender CSWSCreature::BroadcastAttackBreakdown 004ECBE0 -> 0x14)
//   minor 22 (00661379..006619DC) defense breakdown (sender CSWSCreature::BroadcastDefenseBreakdown 004ECC60 -> 0x16)
// Both exit through 00662EE4 (EAX = 0) after appending the composed line. Neither checks MessageReadOverflow.
// See claude_s2p_cc.hpp for the frame/exit model. Transcribed from the original swkotor.exe (read-only).
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
// a = a + Fetch(strref, 0, 0, 0): Fetch into T1, operator+(a, T1-return) into T2, a = T2, ~T2, ~T1.
inline void append_fetch(Frame& f, U strref) {
    Tmp t1;
    void* part = t1.set(fetch(t1.p(), strref));
    Tmp t2;
    str_assign(&f.a, t2.set(concat(t2.p(), &f.a, part)));
}
// SetCustomToken(0, CExoString(value)); a = a + Fetch(strref)   (the recurring "if (value) ..." block)
inline void value_line(Frame& f, I value, U strref) {
    token_int(0, value);
    append_fetch(f, strref);
}
}  // namespace

// -----------------------------------------------------------------------------------------------------------------
// minor 20 (006603D9..0066112F): attack roll breakdown. 21 DWORD reads (3rd and 11th discarded), no overflow check.
// -----------------------------------------------------------------------------------------------------------------
U case_20(Frame& f) {
    ExoString attack;                                                // 006603D9 [esp+29C] (state 1F8h)
    const U r1 = rd_dword(f);                                        // 006603E5..006603FD edi
    const U r2 = rd_dword(f);                                        // 006603F9..00660408 [esp+70]
    (void)rd_dword(f);                                               // 00660404..0066040C (discarded)
    const U r4 = rd_dword(f);                                        // 00660411..0066041E [esp+98]
    const U r5 = rd_dword(f);                                        // 0066041A..0066042E [esp+B4]
    const U r6 = rd_dword(f);                                        // 0066042A..0066043E [esp+50]
    const U r7 = rd_dword(f);                                        // 0066043A..0066044B [esp+74]
    const U r8 = rd_dword(f);                                        // 00660447..00660458 ebx
    const U r9 = rd_dword(f);                                        // 00660454..00660463 [esp+4C]
    const U r10 = rd_dword(f);                                       // 0066045F..00660470 [esp+34]
    (void)rd_dword(f);                                               // 0066046C..00660474 (discarded)
    const U r12 = rd_dword(f);                                       // 00660479..00660486 [esp+3C]
    const U r13 = rd_dword(f);                                       // 00660482..00660493 [esp+48]
    const U r14 = rd_dword(f);                                       // 0066048F..006604A0 [esp+40]
    const U r15 = rd_dword(f);                                       // 0066049C..006604AD [esp+38]
    const U r16 = rd_dword(f);                                       // 006604A9..006604BA [esp+94]
    const U r17 = rd_dword(f);                                       // 006604B6..006604CA [esp+2C]
    const U r18 = rd_dword(f);                                       // 006604C6..006604D7 [esp+1C]
    const U r19 = rd_dword(f);                                       // 006604D3..006604E4 [esp+24]
    const U r20 = rd_dword(f);                                       // 006604E0..006604F1 [esp+78]
    const U r21 = rd_dword(f);                                       // 006604ED..00660500 esi
    assign_fetch(&attack, 0xa54au + (r8 != 0 ? 1u : 0u));             // 006604FA..00660547 setne; add 0A54Ah
    token(0, &attack);                                               // 0066054C..0066055C
    token_int(1, static_cast<I>(r1));                                // 00660561..00660599
    append_fetch(f, 0xa4a2);                                         // 0066059E..0066060A
    token_int(0, static_cast<I>(r2));                                // 0066060F..0066064B
    append_fetch(f, 0xa54c);                                         // 00660650..006606BC
    if (r20 != 0 || r21 != 0) {                                      // 006606C1..006606C7 / 006607A7..006607A9
        // a = (a + " ") + Fetch(0A596h | 0A597h); the Fetch runs before the first operator+.
        ExoString space(" ");                                        // 006606CD / 006607AF [esp+3EC|3FC] (0073D5D8)
        Tmp t1;
        void* part = t1.set(fetch(t1.p(), r20 != 0 ? 0xa596u : 0xa597u)); // 006606DE..00660707 / 006607C0..006607E9
        Tmp t2;
        void* joined = t2.set(concat(t2.p(), &f.a, &space));         // 00660709..00660725 / 006607EB..00660807
        Tmp t3;
        str_assign(&f.a, t3.set(concat(t3.p(), joined, part)));      // 0066072A..0066074F / 0066080C..00660831
    }                                                                // 00660754..006607A2 / 00660836..00660884 -> 006610F2
    else {
        token_int(0, static_cast<I>(r18));                           // 00660889..006608CD
        append_fetch(f, 0xa598);                                     // 006608D2..00660941
        if (r19 != 0) value_line(f, static_cast<I>(r19 + r10), 0xa55d);  // 00660946..00660A07
        if (r9 != 0) value_line(f, static_cast<I>(r9), 0xa55e);      // 00660A0C..00660AC7
        if (r6 != 0 && r7 != 0) {                                    // 00660ACC..00660ADE
            void* feat = kotor_rules_get_feat_00550c00(rules(), nullptr, static_cast<U16>(r6));  // 00660AE4..00660AEB
            token_gui(0, rd(feat, 8));                               // 00660AF0..00660B31
            token_int(1, static_cast<I>(r7));                        // 00660B36..00660B71
            append_fetch(f, 0xa54e);                                 // 00660B76..00660BE5
        }
        if (r12 != 0) {                                              // 00660BEA..00660BF0
            void* feat = kotor_rules_get_feat_00550c00(rules(), nullptr, static_cast<U16>(r13));  // 00660BF6..00660C01
            token_gui(0, rd(feat, 8));                               // 00660C06..00660C47
            token_int(1, static_cast<I>(r12));                       // 00660C4C..00660C87
            append_fetch(f, 0xa54e);                                 // 00660C8C..00660CFB
        }
        if (r14 != 0) value_line(f, static_cast<I>(r14), 0xa55a);    // 00660D00..00660DBB
        if (r15 != 0) value_line(f, static_cast<I>(r15), 0xa54d);    // 00660DC0..00660E7B
        ExoString unused;                                            // 00660E80..00660E87 [esp+F4C] (never used)
        U value;
        U strref;
        if (r5 != 0) { value = r5; strref = 0xa587; }                // 00660E8C..00660EA7
        else { value = r4; strref = 0xa4aa; }                        // 00660EA9..00660EB0
        if (value != 0) value_line(f, static_cast<I>(value), strref); // 00660EB5..00660F64
        if (r16 != 0) value_line(f, static_cast<I>(r16), 0xa55b);    // 00660F69..00661023
        if (r17 != 0) value_line(f, static_cast<I>(r17), 0xa55c);    // 00661028..006610DF
    }                                                                // 006610E4..006610F2 ~[F4C]
    append_a(f);                                                     // 006610F7..00661113 (a, 80h, 0)
    return kFail;                                                    // 00661118..0066112F ~[29C]; jmp 00662EE4
}

// -----------------------------------------------------------------------------------------------------------------
// minor 22 (00661379..006619DC): defense breakdown. 9 DWORD reads (4th discarded), no overflow check.
// -----------------------------------------------------------------------------------------------------------------
U case_22(Frame& f) {
    const U q1 = rd_dword(f);                                        // 00661379..00661386 edi
    const U q2 = rd_dword(f);                                        // 00661382..00661391 ebx
    const U q3 = rd_dword(f);                                        // 0066138D..0066139C [esp+2C]
    (void)rd_dword(f);                                               // 00661398..006613A0 (discarded)
    const U q5 = rd_dword(f);                                        // 006613A5..006613B2 [esp+94]
    const U q6 = rd_dword(f);                                        // 006613AE..006613C2 [esp+98]
    const U q7 = rd_dword(f);                                        // 006613BE..006613D2 [esp+B4]
    const U q8 = rd_dword(f);                                        // 006613CE..006613E2 [esp+70]
    const U q9 = rd_dword(f);                                        // 006613DE..006613ED [esp+78]
    if (q1 == 0) return kFail;                                       // 006613EB..006613F1 je 00662EE4 (nothing shown)
    token_int(0, static_cast<I>(q1));                                // 006613F7..00661437
    assign_fetch(&f.a, 0xa4a5);                                      // 0066143C..0066147D
    if (q2 != 0) value_line(f, static_cast<I>(q2), 0xa562);          // 00661482..00661539
    if (q3 != 0) value_line(f, static_cast<I>(q3), 0xa563);          // 0066153E..006615F9
    if (q5 != 0) value_line(f, static_cast<I>(q5), 0xa564);          // 006615FE..006616BC
    if (q6 != 0) value_line(f, static_cast<I>(q6), 0xa565);          // 006616C1..0066177F
    if (q7 != 0) value_line(f, static_cast<I>(q7), 0xa566);          // 00661784..00661842
    if (q8 != 0) value_line(f, static_cast<I>(q8), 0xa567);          // 00661847..00661902
    if (q9 != 0) value_line(f, static_cast<I>(q9), 0xa5bb);          // 00661907..006619C2
    append_a(f);                                                     // 006619C7..006619DC -> 00662ED8..00662EDF
    return kFail;                                                    // 00662EE4
}
}  // namespace s2pcc
