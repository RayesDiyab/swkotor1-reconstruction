// Lane worker "s2p-cc" (fork "damage"): 00656D20 HandleServerToPlayerCCMessage minors 2 and 3 (damage lines).
// Sender: CSWSCreature::BroadcastDamageDataToParty 004EC4E1 (minor 2 when its flag is set, else 3).
// Transcribed from the original swkotor.exe (read-only); see claude_s2p_cc.hpp for the frame/exit model.
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
// Per-damage-type detail strrefs for slots 3..13 (identical push immediates in both minors:
// minor 2 00657238..006577CF, minor 3 00657E0D..0065839D). Slots 0..2 are summed as "physical" (0x58F);
// slot 14 is the total and is never listed.
constexpr U kDetailStrref[15] = {0, 0, 0, 0x58e, 0x5a0, 0x5a1, 0x5a2, 0x5a3, 0x5a4, 0x5a5, 0x5a6, 0x5a7, 0x5a8, 0xa3ae, 0};

// The common per-entry tail (minor 2 006577FD..006578F0, minor 3 006583CB..006584B7):
//   first entry (flag still set): flag = 0; a = a + detail
//   later entries:                a = (a + " ") + detail
inline void append_detail(Frame& f, ExoString& detail, I& first) {
    if (first == 0) {
        ExoString space(" ");                                       // .rdata 0073D5D8
        Tmp t1;
        void* lhs = t1.set(concat(t1.p(), &f.a, &space));           // a + " "
        Tmp t2;
        str_assign(&f.a, t2.set(concat(t2.p(), lhs, &detail)));     // (a + " ") + detail
    } else {
        first = 0;                                                  // written before operator+
        assign_concat(&f.a, &f.a, &detail);
    }
}

// a = Fetch(strref) + " ("  (paren constructed first, then the Fetch temporary, then the sum temporary)
inline void head_line(Frame& f, U strref) {
    ExoString paren(" (");                                          // .rdata 00748EA4
    Tmp fetched;
    void* lhs = fetched.set(fetch(fetched.p(), strref));
    Tmp sum;
    str_assign(&f.a, sum.set(concat(sum.p(), lhs, &paren)));
}

// Physical group (slots 0..2) then slots 3..13, then ")" (minor 2 006570CF..00657964, minor 3 00657CA9..0065852E).
inline void detail_list(Frame& f, ExoString& detail, const I (&dmg)[15], I& first) {
    if (dmg[0] > -1 || dmg[1] > -1 || dmg[2] > -1) {                // signed jg/jle
        I sum = 0;
        if (dmg[0] > -1) sum = dmg[0];
        if (dmg[1] > -1) sum += dmg[1];
        if (dmg[2] > -1) sum += dmg[2];
        token_int(0, sum);
        assign_fetch(&detail, 0x58f);
        assign_concat(&f.a, &f.a, &detail);                         // no separator for the physical entry
        first = 0;
    }
    for (U i = 0; i < 15; ++i) {
        if (i < 3 || i > 13) continue;                              // only cmp esi,3..0Dh dispatch arms
        if (dmg[i] <= -1) continue;
        token_int(0, dmg[i]);
        assign_fetch(&detail, kDetailStrref[i]);
        append_detail(f, detail, first);
    }
    ExoString close(")");                                           // .rdata 0074F788
    assign_concat(&f.a, &f.a, &close);
}
}  // namespace

// -----------------------------------------------------------------------------------------------------------------
// minor 2 (00656ECC..00657998): "<name> ... <total>" (TLK 0x57A) + " (" + per-type details + ")".
// -----------------------------------------------------------------------------------------------------------------
U case_02(Frame& f) {
    I first = 1;                                                     // 00656ED3 [esp+40]
    ExoString detail;                                                // 00656ECC [esp+8C]
    const U id = rd_obj(f);                                          // 00656EE0..00656EF1 [esp+1C]
    I total = 0;                                                     // 00656EF5 [esp+38]
    I dmg[15];                                                       // [esp+128]
    for (U i = 0; i < 15; ++i) {                                     // 00656F00..00656F5D
        const U present = rd_bool(f);
        if (overflow(f, 1) != 0) return kFail;                       // 00656F07..00656F14 -> 00656F6C
        dmg[i] = -1;                                                 // 00656F18
        if (present != 0) {
            const I value = static_cast<I>(rd_word(f, 0xe));         // 00656F25..00656F2E movzx
            dmg[i] = value;
            if (overflow(f, 1) != 0) return kFail;                   // 00656F31..00656F43
            total += value;                                          // 00656F45
        }
        if (i == 14) total = dmg[14];                                // 00656F49..00656F55 (overrides the running sum)
    }
    if (overflow(f, 1) != 0) return kFail;                           // 00656F5F..00656F80
    object_name_or_gui(id, &f.b);                                    // 00656F85..00656FD9
    token(0, &f.b);                                                  // 00656FDE..00656FEB
    token_int(1, total);                                             // 00656FF0..00657026
    head_line(f, 0x57a);                                             // 0065702B..006570CA
    detail_list(f, detail, dmg, first);                              // 006570CF..00657964
    append_a(f);                                                     // 00657969..00657984
    return kDone;                                                    // 00657989..00657998 ~[8C] -> 00660382
}

// -----------------------------------------------------------------------------------------------------------------
// minor 3 (0065799D..0065868E): "<attacker> ... <target> <total>" (TLK 0x57B) + details, then an optional extra line.
// -----------------------------------------------------------------------------------------------------------------
U case_03(Frame& f) {
    I first = 1;                                                     // 006579A4 [esp+34] (later held in EDI)
    ExoString name_a;                                                // 0065799D [esp+108]
    ExoString name_fallback;                                         // 006579B1 [esp+D8]
    ExoString detail;                                                // 006579C5 [esp+84]
    const U id_a = rd_obj(f);                                        // 006579DB..006579EB [esp+48]
    const U id_b = rd_obj(f);                                        // 006579E9..006579F6 [esp+24]
    I total = 0;                                                     // 006579FA [esp+40]
    I dmg[15];                                                       // [esp+128]
    for (U i = 0; i < 15; ++i) {                                     // 00657A00..00657A56
        const U present = rd_bool(f);                                // [esp+1C]
        if (overflow(f, 1) != 0) return kFail;                       // 00657A07..00657A16 -> 006585DF
        dmg[i] = -1;                                                 // 00657A22
        if (present != 0) dmg[i] = static_cast<I>(rd_word(f, 0xe));  // 00657A2F..00657A3B (no overflow check, no sum)
        if (i == 14) total = dmg[14];                                // 00657A42..00657A4E
    }
    if (overflow(f, 1) != 0) return kFail;                           // 00657A58..00657A63
    if (object_name(id_b, &f.b) != 0) {                              // 00657A69..00657A8B
        if (object_name(id_a, &name_a) != 0) {                       // 00657A8D..00657AA1
            token(0, &name_a);                                       // 00657AA3, 00657AF3..00657AFC
        } else {
            assign_gui(&name_fallback, 0);                           // 00657AAC..00657AE7
            token(0, &name_fallback);                                // 00657AEC..00657AFC
        }
        token(1, &f.b);                                              // 00657B01, 00657BBE..00657BC7
    } else {
        assign_gui(&f.b, 0);                                         // 00657B0A..00657B39 (push 0)
        if (object_name(id_a, &name_a) != 0) {                       // 00657B3E..00657B5A
            token(0, &name_a);                                       // 00657B5C, 00657BAC..00657BB5
        } else {
            assign_gui(&name_fallback, 0);                           // 00657B65..00657BA0
            token(0, &name_fallback);                                // 00657BA5..00657BB5
        }
        token(1, f.res.raw);                                         // 00657BBA..00657BC7 quirk: STR_RES text [esp+54], not [esp+14]
    }
    token_int(2, total);                                             // 00657BCC..00657C01
    head_line(f, 0x57b);                                             // 00657C06..00657CA4
    detail_list(f, detail, dmg, first);                              // 00657CA9..0065852E
    append_a(f);                                                     // 00658533..0065854E
    const U extra = rd_bool(f);                                      // 00658553..0065855E edi
    if (overflow(f, 1) != 0) return kFail;                           // 0065855A..00658567 -> 006585DF
    if (extra != 0) {                                                // 00658569..0065856B
        ExoString line;                                              // 00658571 [esp+274]
        {
            Tmp t;                                                   // [esp+E54]
            str_assign(&line, t.set(kotor_fastcall_004d67e0(f.msg, nullptr, t.p(), 0x20)));  // 0065857D..006585BA
        }
        if (overflow(f, 1) != 0) return kFail;                       // 006585BF..0065861B
        append(&line, 0x80, 0);                                      // 00658620..0065863F
    }                                                                // 00658644..00658652 ~line
    return kDone;                                                    // 00658657..0065868E -> 00660382
}
}  // namespace s2pcc
