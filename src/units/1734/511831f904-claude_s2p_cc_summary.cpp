// Lane worker "s2p-cc" (fork "summary"): HandleServerToPlayerCCMessage 00656D20 minors 18 (attack summary),
// 19 (mine detection) and 21 (threat breakdown). Transcribed from the original swkotor.exe (read-only); statement
// comments cite original instruction addresses. Frame/exit model: claude_s2p_cc.hpp.
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
// a = a + Fetch(strref)  (Fetch temp, operator+ temp, operator=, ~sum, ~fetch)
void append_fetch(Frame& f, U strref) {
    Tmp fetched;
    void* r = fetched.set(fetch(fetched.p(), strref));
    Tmp sum;
    str_assign(&f.a, sum.set(concat(sum.p(), &f.a, r)));
}
// sep = CExoString(text); a = (a + Fetch(strref)) + sep  (destroyed: sum2, sum1, fetch, sep)
void append_fetch_sep(Frame& f, U strref, const char* text) {
    Tmp sep;
    (void)sep.set(from_text(sep.p(), text));
    Tmp fetched;
    void* r = fetched.set(fetch(fetched.p(), strref));
    Tmp sum1;
    void* s1 = sum1.set(concat(sum1.p(), &f.a, r));
    Tmp sum2;
    str_assign(&f.a, sum2.set(concat(sum2.p(), s1, sep.p())));
}
}  // namespace

// -----------------------------------------------------------------------------------------------------------------
// minor 18 (0065F56C..0066036B, tail 00660370 -> 00660377): attack summary line in the message buffer.
// Sender: CSWSCreature::BroadcastAttackSummaryToParty.
// -----------------------------------------------------------------------------------------------------------------
U case_18(Frame& f) {
    ExoString kind;                                                  // 0065F56C [esp+C0]
    const U attacker = rd_obj(f);                                    // 0065F578..0065F58C [esp+70]
    const U target = rd_obj(f);                                      // 0065F58A..0065F599 [esp+1C]
    const U result = rd_dword(f, 0x20);                              // 0065F595..0065F5A6 [esp+78] (esi later)
    const U16 feat = rd_word(f, 0x10);                               // 0065F5A2..0065F5B3 edi
    const U roll = rd_dword(f, 0x20);                                // 0065F5AF..0065F5BE [esp+24]
    const U bonus = rd_dword(f, 0x20);                               // 0065F5BA..0065F5CB [esp+34]
    const U defense = rd_dword(f, 0x20);                             // 0065F5C7..0065F5D8 [esp+4C]
    const U flag_5e7 = rd_dword(f, 0x20);                            // 0065F5D4..0065F5E5 [esp+50]
    const U flag_5b3 = rd_dword(f, 0x20);                            // 0065F5E1..0065F5F2 [esp+74]
    const U flag_a53f = rd_dword(f, 0x20);                           // 0065F5EE..0065F5FF [esp+3C]
    const U weapon_kind = rd_dword(f, 0x20);                         // 0065F5FB..0065F60A [esp+38]
    const U offhand = rd_bool(f);                                    // 0065F608..0065F615 ebx
    const U flag_a596 = rd_bool(f);                                  // 0065F613..0065F61E [esp+48]
    const U flag_a597 = rd_bool(f);                                  // 0065F61C..0065F62B [esp+40]
    if (overflow(f, 1) != 0) return kFail;                           // 0065F627..0065F64F (~[C0])

    object_name_or_gui(attacker, &f.b);                              // 0065F654..0065F6A8
    token(0, &f.b);                                                  // 0065F6AD..0065F6BA
    const U r = result;                                              // 0065F6BF esi
    assign_gui(&kind, (r == 1 || r == 2 || r == 3) ? 0xa43bu : 0xa43cu); // 0065F6C3..0065F74A
    token(1, &kind);                                                 // 0065F74F..0065F75F
    object_name_or_gui(target, &f.b);                                // 0065F764..0065F7B8
    token(2, &f.b);                                                  // 0065F7BD..0065F7CA
    {                                                                // 0065F7CF..0065F86E a = Fetch(A43A) + ". "
        Tmp sep;
        (void)sep.set(from_text(sep.p(), ". "));                     // [esp+33C] .rdata 00751EB8
        Tmp fetched;
        void* fr = fetched.set(fetch(fetched.p(), 0xa43a));          // [esp+AEC]
        Tmp sum;
        str_assign(&f.a, sum.set(concat(sum.p(), fr, sep.p())));    // [esp+ADC]
    }
    if (feat != 0) {                                                 // 0065F873..0065F876 test di,di
        void* feat_row = kotor_rules_get_feat_00550c00(rules(), nullptr, feat); // 0065F87C..0065F883
        {
            Tmp name;                                                // 0065F888..0065F8C7 token 0 = GUI(feat+8)
            token(0, name.set(gui_string(name.p(), rd(feat_row, 8))));
        }
        append_fetch_sep(f, 0xa43e, ". ");                           // 0065F8CC..0065F999
    }
    assign_gui(&kind, (r == 1 || r == 2 || r == 3) ? 0xa495u : 0xa496u); // 0065F99E..0065FA25
    token(0, &kind);                                                 // 0065FA2A..0065FA3A
    token_int(1, static_cast<I>(roll));                              // 0065FA3F..0065FA75
    token_int(2, static_cast<I>(bonus));                             // 0065FA7A..0065FAB0
    {
        I d = static_cast<I>(defense);                               // 0065FAB5..0065FABD (signed jge: negative -> 0)
        if (d < 0) d = 0;
        token_int(3, d);                                             // 0065FABF..0065FAF1
    }
    append_fetch(f, 0xa487);                                         // 0065FAF6..0065FB63
    if (flag_5e7 != 0) append_fetch_sep(f, 0x5e7, " ");             // 0065FB68..0065FC41
    if (flag_5b3 != 0) append_fetch_sep(f, 0x5b3, " ");             // 0065FC46..0065FD1F
    if (flag_a53f != 0) append_fetch_sep(f, 0xa53f, " ");           // 0065FD24..0065FDFD
    if (flag_a596 != 0) {                                            // 0065FE02..0065FED3
        append_fetch_sep(f, 0xa596, " ");
    } else if (flag_a597 != 0) {                                     // 0065FED8..0065FFB1
        append_fetch_sep(f, 0xa597, " ");
    }
    if (r == 8) {                                                    // 0065FFB6..00660084
        append_fetch_sep(f, 0xa5ab, " ");
    } else if (r == 9) {                                             // 00660089..0066015F
        append_fetch_sep(f, 0xa5b5, " ");
    }
    if (weapon_kind != 0) {                                          // 00660164..0066016A
        object_name_or_gui(target, &f.b);                            // 00660170..006601C3
        token(0, &f.b);                                              // 006601C8..006601D5
        U id = 0;                                                    // 006601DA..006601E2 (index > 9 -> 0)
        const U add = offhand != 0 ? 0x29u : 0u;                     // neg ebx; sbb ebx,ebx; and ebx,29h
        switch (weapon_kind - 1) {                                   // 006601E8 table 0066312C
        case 0: id = add + 0xa42f; break;                            // 006601EF
        case 1: id = add + 0xa433; break;                            // 0066022E
        case 2: id = add + 0xa434; break;                            // 0066023D
        case 3: id = add + 0xa430; break;                            // 00660201
        case 4: id = add + 0xa435; break;                            // 0066024C
        case 5: id = add + 0xa436; break;                            // 0066025B
        case 6: id = add + 0xa431; break;                            // 00660210
        case 7: id = add + 0xa438; break;                            // 00660279
        case 8: id = add + 0xa432; break;                            // 0066021F
        case 9: id = add + 0xa437; break;                            // 0066026A
        default: break;
        }
        assign_gui(&kind, id);                                       // 00660288..006602C3
        token(1, &kind);                                             // 006602C8..006602D8
        append_fetch(f, 0xa42e);                                     // 006602DD..0066034A
    }
    append_a(f, 0x80, 1);                                            // 0066034F..0066036B
    return kDone;                                                    // 00660370..00660377 ~[C0]
}

// -----------------------------------------------------------------------------------------------------------------
// minor 19 (0065F018..0065F363): mine detection, two lines (TLK A494 with flag 1, then A48B).
// Sender: CSWSCreature::BroadcastMineDetectionData.
// -----------------------------------------------------------------------------------------------------------------
U case_19(Frame& f) {
    ExoString name0;                                                 // 0065F018 [esp+1C4]
    ExoString name1;                                                 // 0065F024 [esp+214]
    const U first = rd_obj(f);                                       // 0065F03B..0065F04C [esp+70]
    const U second = rd_obj(f);                                      // 0065F04A..0065F059 [esp+78]
    const U16 w0 = rd_word(f, 0x10);                                 // 0065F055..0065F066 edi
    const U16 w1 = rd_word(f, 0x10);                                 // 0065F062..0065F071 [esp+1C]
    const U16 w2 = rd_word(f, 0x10);                                 // 0065F06D..0065F07E [esp+24]
    if (overflow(f, 1) != 0) return kFail;                           // 0065F07A..0065F0B6
    ExoString unused;                                                // 0065F0BB [esp+A2C] (never used)
    const I total = static_cast<std::int16_t>(static_cast<U16>(w1 + w0)); // 0065F0C7..0065F0D1 / 0065F139 movsx si
    (void)object_name(first, &name0);                                // 0065F0CB..0065F0F0 (result ignored)
    (void)object_name(second, &name1);                               // 0065F0F5..0065F10A (result ignored)
    token(0, &name0);                                                // 0065F10F..0065F11F
    token(1, &name1);                                                // 0065F124..0065F134
    token_int(2, total);                                             // 0065F139..0065F16D
    token_int(3, static_cast<std::int16_t>(w2));                     // 0065F172..0065F1A8
    assign_fetch(&f.a, 0xa494);                                      // 0065F1AD..0065F1EB
    append_a(f, 0x80, 1);                                            // 0065F1F0..0065F20C
    token(0, &name0);                                                // 0065F211..0065F221
    token_int(1, total);                                             // 0065F226..0065F257
    token_int(2, static_cast<std::int16_t>(w0));                     // 0065F25C..0065F290
    token_int(3, static_cast<std::int16_t>(w1));                     // 0065F295..0065F2CB
    assign_fetch(&f.a, 0xa48b);                                      // 0065F2D0..0065F30E
    append_a(f, 0x80, 0);                                            // 0065F313..0065F32F
    return kDone;                                                    // 0065F334..0065F363 -> 00660377
}

// -----------------------------------------------------------------------------------------------------------------
// minor 21 (00661134..00661374): threat breakdown line, TLK A4A4. No overflow check; exits through 00662EE4.
// Sender: CSWSCreature::BroadcastThreatBreakdown (via BroadcastBreakdownMessage).
// -----------------------------------------------------------------------------------------------------------------
U case_21(Frame& f) {
    const U v0 = rd_dword(f, 0x20);                                  // 00661134..00661141 edi
    const U v1 = rd_dword(f, 0x20);                                  // 0066113D..0066114C ebx
    const U present = rd_dword(f, 0x20);                             // 00661148..00661157 [esp+2C]
    const U v3 = rd_dword(f, 0x20);                                  // 00661153..00661164 [esp+94]
    const U v4 = rd_dword(f, 0x20);                                  // 00661160..00661174 [esp+98]
    (void)rd_dword(f, 0x20);                                         // 00661170..0066117B (discarded)
    const U sign = rd_dword(f, 0x20);                                // 00661180..00661189 esi
    if (present == 0) return kFail;                                  // 0066118B..00661191
    token_int(0, static_cast<I>(v0));                                // 00661197..006611D0
    token_int(1, static_cast<I>(v1));                                // 006611D5..00661209
    ExoString op;                                                    // 0066120E [esp+28C]
    assign_gui(&op, sign != 0 ? 0x570u : 0x571u);                    // 0066121A..0066126A neg; sbb; add 571h
    token(2, &op);                                                   // 0066126F..0066127E
    token_int(3, static_cast<I>(v3));                                // 00661283..006612BB
    token_int(4, static_cast<I>(v4));                                // 006612C0..006612F8
    assign_fetch(&f.a, 0xa4a4);                                      // 006612FD..0066133B
    append_a(f, 0x80, 0);                                            // 00661340..0066135C
    return kFail;                                                    // 00661361..00661374 ~[28C] -> 00662EE4
}
}  // namespace s2pcc
