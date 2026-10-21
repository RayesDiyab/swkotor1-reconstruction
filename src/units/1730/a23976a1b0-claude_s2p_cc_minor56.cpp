// Lane worker "s2p-cc" (minor56 fork): 00656D20 HandleServerToPlayerCCMessage minors 5 and 6.
// Both compose a roll/check line into Frame::a (tokens + TLK 0x57C / 0x57D, optional "<label> : " prefixes) and then read
// a trailing BOOL + optional CExoString, but neither ever calls AppendToMsgBuffer: the composed line is discarded at the
// exit (original behaviour; no K1 server sender emits minors 5 / 6). See claude_s2p_cc.hpp for the frame/exit model.
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
constexpr const char kPlus_00748ed4[] = "+";
constexpr const char kMinus_0073ed40[] = "-";
constexpr const char kEmpty_0073d71c[] = "";
constexpr const char kColon_0073e474[] = " : ";
constexpr const char kRollFormat_00751eec[] = "%s %s %d = %d";

// f.a = GetGUIString(id) + " : " + f.a  (CExoString(" : ") first, then the GUI string, two operator+ temporaries)
void prefix_gui(Frame& f, U id) {
    Tmp colon;
    (void)colon.set(from_text(colon.p(), kColon_0073e474));
    Tmp label;
    void* l = label.set(gui_string(label.p(), id));
    Tmp first;
    void* lhs = first.set(concat(first.p(), l, colon.p()));
    Tmp second;
    str_assign(&f.a, second.set(concat(second.p(), lhs, &f.a)));
}                                                    // ~second, ~first, ~label, ~colon

// f.a = s + CExoString(" : ") + f.a (rhs of the first operator+ is the ctor's EAX)
void prefix_str(Frame& f, void* s) {
    Tmp colon;
    void* c = colon.set(from_text(colon.p(), kColon_0073e474));
    Tmp first;
    void* lhs = first.set(concat(first.p(), s, c));
    Tmp second;
    str_assign(&f.a, second.set(concat(second.p(), lhs, &f.a)));
}                                                    // ~second, ~first, ~colon

// Trailing BOOL + optional CExoString read shared by both minors (00659362.. / 0065A088..).
U trailing_string(Frame& f) {
    const U has = rd_bool(f);                                        // ReadBOOL edi
    if (overflow(f, 1) != 0) return kFail;                           // -> 006593F2 / 0065A119
    if (has == 0) return kDone;                                      // -> 00659474 / 0065A19B
    ExoString text;                                                  // [esp+284] / [esp+2A4]
    {
        Tmp t;
        str_assign(&text, t.set(kotor_fastcall_004d67e0(f.msg, nullptr, t.p(), 0x20)));   // ReadCExoString(20h)
    }
    if (overflow(f, 1) != 0) return kFail;                           // ~text -> fail path
    return kDone;                                                    // ~text -> done path (string unused)
}
}  // namespace

// -----------------------------------------------------------------------------------------------------------------
// minor 5 (00658693..006594E7): signed modifier roll line, TLK 0x57C (+0x589 detail). No AppendToMsgBuffer.
// -----------------------------------------------------------------------------------------------------------------
U case_05(Frame& f) {
    StrRes unused;                                                   // 00658693 [esp+1364] (never used)
    ExoString sign;                                                  // 0065869F [esp+AC]
    ExoString name1;                                                 // 006586B3 [esp+C8]
    ExoString kind;                                                  // 006586C7 [esp+A4]
    ExoString name1_default;                                         // 006586DB [esp+B8]
    ExoString detail;                                                // 006586EF [esp+D0]
    const U id1 = rd_obj(f);                                         // 00658705..00658715 [esp+3C]
    const U id2 = rd_obj(f);                                         // 00658713..00658722 [esp+1C]
    if (overflow(f, 1) != 0) return kFail;                           // 0065871E..0065872D
    I modifier = static_cast<signed char>(rd_byte(f, 8));                     // 00658733..0065873C movsx edi ([esp+38])
    const U8 base = rd_byte(f, 5);                                   // 0065873F..00658750 [esp+23]
    const U8 type = rd_byte(f, 4);                                   // 0065874C..0065875D [esp+33]
    const U8 source = rd_byte(f, 3);                                 // 00658759..00658768 [esp+2B]
    const U flag74 = rd_bool(f);                                     // 0065876C..00658773 [esp+74]
    const U flag4c = rd_bool(f);                                     // 00658777..0065877E [esp+4C]
    const U flag50 = rd_bool(f);                                     // 00658782..00658789 [esp+50]
    const U flag34 = rd_bool(f);                                     // 0065878D..00658794 [esp+34]
    const U detailed = rd_bool(f);                                   // 00658798..006587A1 [esp+48]
    if (overflow(f, 1) != 0) return kFail;                           // 0065879D..006587AC
    U roll = 0;                                                      // [esp+40]: original leaves it unset unless detailed
    if (detailed == 1) roll = rd_byte(f, 5);                         // 006587B2..006587C5 movzx
    U8 extra;                                                        // [esp+46]
    if (type == 0x11 || type == 0x10) extra = rd_byte(f, 7);         // 006587C9..006587E5
    else extra = 0;                                                  // 006587D5
    if (overflow(f, 1) != 0) return kFail;                           // 006587E9..006587F4
    const I total = static_cast<I>(base) + modifier;                 // 006587FA..00658803 [esp+24]
    if (modifier >= 0) {                                             // 00658801..0065880E jl
        assign_text(&sign, kPlus_00748ed4);                          // 00658810..00658815
    } else {
        assign_text(&sign, kMinus_0073ed40);                         // 0065881C..00658821
        modifier = -modifier;                                        // 00658826..00658828 [esp+38] = |modifier|
    }
    if (detailed == 1) {                                             // 0065882C..00658831
        I result;                                                    // edi
        if (equals_text(&sign, kMinus_0073ed40) != 0)                // 00658837..0065884A
            result = static_cast<I>(roll) - modifier;                // 0065884C..00658850
        else
            result = static_cast<I>(roll) + modifier;                // 00658856..0065885E
        token_int(0, static_cast<I>(roll));                          // 00658861..00658896
        assign_fetch(&detail, 0x589);                                // 0065889B..006588DC
        kotor_reconstruction::StartupFormat005e5680(ss(&detail), kRollFormat_00751eec, cstr(&detail), cstr(&sign),
                                                    modifier, result);  // 006588E1..00658917
    }
    switch (type) {                                                  // 0065891A..00658AE2 (al compare chain)
    case 1: assign_gui(&kind, 0x55b); break;                         // 0065891E..00658959
    case 2: assign_gui(&kind, 0x56e); break;                         // 0065895E..00658998
    case 8: assign_gui(&kind, 0); break;                             // 0065899D..006589D4
    case 4: assign_gui(&kind, 0x55d); break;                         // 006589D9..00658A13
    case 3: assign_gui(&kind, 0x588); break;                         // 00658A18..00658A52
    case 0x11:                                                       // 00658A57..00658ADB
        token_int(0, static_cast<I>(extra));
        assign_fetch(&kind, 0x58a);
        break;
    case 0x10:                                                       // 00658AE0..00658B6B
        token_int(0, static_cast<I>(extra));
        assign_fetch(&kind, 0x58b);
        break;
    default: break;
    }
    void* first_token;                                               // ECX at 00658C99
    if (object_name(id2, &f.b) != 0) {                               // 00658B70..00658B89
        if (object_name(id1, &name1) != 0) {                         // 00658B8B..00658BA8
            first_token = &name1;                                    // 00658BAA
        } else {
            assign_gui(&name1_default, 0);                           // 00658BB6..00658C8D
            first_token = &name1_default;                            // 00658C92
        }
    } else {
        assign_gui(&f.b, 0);                                         // 00658BEF..00658C26
        if (object_name(id1, &name1) != 0) {                         // 00658C2B..00658C47
            first_token = &name1;                                    // 00658C49
        } else {
            assign_gui(&name1_default, 0);                           // 00658C52..00658C8D
            first_token = &name1_default;                            // 00658C92
        }
    }
    token(0, first_token);                                           // 00658C99..00658CA2
    token(1, &f.b);                                                  // 00658CA7..00658CB4
    token(2, &kind);                                                 // 00658CB9..00658CC9
    token_int(3, static_cast<I>(base));                              // 00658CCE..00658D04
    token(4, &sign);                                                 // 00658D09..00658D19
    token_int(5, modifier);                                          // 00658D1E..00658D4F (|modifier|)
    token_int(6, total);                                             // 00658D54..00658D89
    if (detailed == 1) {                                             // 00658D8E..00658D93
        token(7, &detail);                                           // 00658D95..00658DAA
    } else {
        Tmp empty;                                                   // 00658DAC..00658DE8 CExoString("")
        token(7, empty.set(from_text(empty.p(), kEmpty_0073d71c)));
    }
    assign_fetch(&f.a, 0x57c);                                       // 00658DED..00658E2B
    if (flag34 == 1) prefix_gui(f, 0);                               // 00658E30..00658F08
    if (flag4c == 1) prefix_gui(f, 0x56f);                           // 00658F0D..00658FE1
    if (source != 0) {                                               // 00658FE6..00658FEC
        ExoString label;                                             // 00658FF2 [esp+1B4]
        if (source == 2) assign_gui(&label, 0xb2);                   // 00658FFE..00659043
        else if (source == 3) assign_gui(&label, 0xa7);              // 00659048..00659086
        else if (source == 5) assign_gui(&label, 0xa0);              // 00659088..006590C2
        else if (source == 6) assign_gui(&label, 0xb4);              // 006590C4..00659106
        prefix_str(f, &label);                                       // 0065910B..0065919E
    }                                                                // 006591A3..006591B1 ~label
    if (flag50 == 1) prefix_gui(f, 0);                               // 006591B6..00659287
    if (flag74 == 1) prefix_gui(f, 0);                               // 0065928C..0065935D
    return trailing_string(f);                                       // 00659362..006594E7
}

// -----------------------------------------------------------------------------------------------------------------
// minor 6 (006594EC..0065A20E): feat-based roll line, TLK 0x57D (+0x589 detail). No AppendToMsgBuffer.
// -----------------------------------------------------------------------------------------------------------------
U case_06(Frame& f) {
    ExoString sign;                                                  // 006594EC [esp+110]
    ExoString name1;                                                 // 006594F8 [esp+100]
    ExoString feat_name;                                             // 0065950C [esp+164]
    ExoString kind;                                                  // 00659520 [esp+9C]
    ExoString name1_default;                                         // 00659534 [esp+F0]
    ExoString detail;                                                // 00659548 [esp+F8]
    const U id1 = rd_obj(f);                                         // 0065955C..0065956D [esp+40]
    const U id2 = rd_obj(f);                                         // 0065956B..0065957A [esp+24]
    I modifier = static_cast<signed char>(rd_byte(f, 8));                     // 00659576..00659587 movsx edi
    const U8 base = rd_byte(f, 5);                                   // 00659583..00659593 [esp+23]
    const U8 type = rd_byte(f, 4);                                   // 0065958F..006595A0 bl
    const U16 feat = rd_word(f, 0x10);                               // 0065959C..006595AB [esp+3C]
    const U8 source = rd_byte(f, 3);                                 // 006595A7..006595B6 [esp+33]
    const U flag50 = rd_bool(f);                                     // 006595BA..006595C1 [esp+50]
    const U flag4c = rd_bool(f);                                     // 006595C5..006595CC [esp+4C]
    const U detailed = rd_bool(f);                                   // 006595D0..006595D9 [esp+48]
    if (overflow(f, 1) != 0) return kFail;                           // 006595D5..006595E4
    U roll = 0;                                                      // [esp+1C]: original leaves it unset unless detailed
    if (detailed == 1) roll = rd_byte(f, 5);                         // 006595EA..006595FD movzx
    U8 extra;                                                        // [esp+2B]
    if (type == 0x11 || type == 0x10) extra = rd_byte(f, 7);         // 00659601..0065961B
    else extra = 0;                                                  // 0065960B
    if (overflow(f, 1) != 0) return kFail;                           // 0065961F..0065962A
    const I total = modifier + static_cast<I>(base);                 // 00659630..0065963A [esp+34]
    if (modifier >= 0) {                                             // 00659645 jl
        assign_text(&sign, kPlus_00748ed4);                          // 00659647..0065964C
    } else {
        assign_text(&sign, kMinus_0073ed40);                         // 00659653..00659658
        modifier = -modifier;                                        // 0065965D
    }
    if (detailed == 1) {                                             // 0065965F..00659664
        const I old_roll = static_cast<I>(roll);                     // 0065966A eax
        const I result = old_roll + modifier;                        // 0065966E..00659671 [esp+1C] (always adds: quirk)
        token_int(0, old_roll);                                      // 00659675..006596A7
        assign_fetch(&detail, 0x589);                                // 006596AC..006596EE
        kotor_reconstruction::StartupFormat005e5680(ss(&detail), kRollFormat_00751eec, cstr(&detail), cstr(&sign),
                                                    modifier, result);  // 006596F3..00659725
    }
    if (feat != 0) {                                                 // 00659728..0065972E (word compare)
        void* feat_row = kotor_rules_get_feat_00550c00(rules(), nullptr, feat);   // 00659730..0065973B
        if (feat_row != nullptr) assign_gui(&feat_name, rd(feat_row, 8));         // 00659740..00659782
    }
    switch (type) {                                                  // 00659787..00659992 (bl compare chain)
    case 1: assign_gui(&kind, 0x55b); break;                         // 00659787..006597C2
    case 2: assign_gui(&kind, 0x56e); break;                         // 006597C7..00659802
    case 8: assign_gui(&kind, 0); break;                             // 00659807..0065983F
    case 4: assign_gui(&kind, 0x55d); break;                         // 00659844..0065987F
    case 5: assign_gui(&kind, 0x585); break;                         // 00659884..006598BF
    case 6: assign_gui(&kind, 0x586); break;                         // 006598C4..006598FF
    case 0x11:                                                       // 00659904..0065998A
        token_int(0, static_cast<I>(extra));
        assign_fetch(&kind, 0x58a);
        break;
    case 0x10:                                                       // 0065998F..00659A1D
        token_int(0, static_cast<I>(extra));
        assign_fetch(&kind, 0x58b);
        break;
    default: break;
    }
    void* first_token;                                               // ECX at 00659B4D
    if (object_name(id2, &f.b) != 0) {                               // 00659A22..00659A3B
        if (object_name(id1, &name1) != 0) {                         // 00659A3D..00659A5A
            first_token = &name1;                                    // 00659A5C
        } else {
            assign_gui(&name1_default, 0);                           // 00659A68..00659B41
            first_token = &name1_default;                            // 00659B46
        }
    } else {
        assign_gui(&f.b, 0);                                         // 00659AA1..00659AD9
        if (object_name(id1, &name1) != 0) {                         // 00659ADE..00659AFA
            first_token = &name1;                                    // 00659AFC
        } else {
            assign_gui(&name1_default, 0);                           // 00659B05..00659B41
            first_token = &name1_default;                            // 00659B46
        }
    }
    token(0, first_token);                                           // 00659B4D..00659B56
    token(2, &f.b);                                                  // 00659B5B..00659B68
    token(1, &feat_name);                                            // 00659B6D..00659B7D
    token(3, &kind);                                                 // 00659B82..00659B92
    token_int(4, static_cast<I>(base));                              // 00659B97..00659BCE
    token(5, &sign);                                                 // 00659BD3..00659BE3
    token_int(6, modifier);                                          // 00659BE8..00659C1A (|modifier|)
    token_int(7, total);                                             // 00659C1F..00659C55
    if (detailed == 1) {                                             // 00659C5A..00659C5F
        token(8, &detail);                                           // 00659C61..00659C76
    } else {
        Tmp empty;                                                   // 00659C78..00659CB5 CExoString("")
        token(8, empty.set(from_text(empty.p(), kEmpty_0073d71c)));
    }
    assign_fetch(&f.a, 0x57d);                                       // 00659CBA..00659CF9
    if (flag4c != 0) prefix_gui(f, 0x56f);                           // 00659CFE..00659DD6 (test eax,eax)
    if (source != 0 && source != 1) {                                // 00659DDB..00659DE9
        ExoString label;                                             // 00659DEF [esp+1AC]
        if (source == 2) assign_gui(&label, 0xb2);                   // 00659DFB..00659E46
        else if (source == 3) assign_gui(&label, 0xa7);              // 00659E4B..00659E85
        else if (source == 5) assign_gui(&label, 0xa0);              // 00659E87..00659EC1
        else if (source == 6) assign_gui(&label, 0xb4);              // 00659EC3..00659F01 (push ebx = B4h)
        prefix_str(f, &label);                                       // 00659F06..00659F97
    }                                                                // 00659F9C..00659FAB ~label
    if (flag50 == 1) prefix_gui(f, 0);                               // 00659FB0..0065A083
    return trailing_string(f);                                       // 0065A088..0065A20E
}
}  // namespace s2pcc
