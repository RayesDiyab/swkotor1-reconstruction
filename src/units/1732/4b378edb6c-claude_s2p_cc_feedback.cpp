// Lane worker "s2p-cc" (fork "feedback"): minor 11 of 00656D20 CSWCMessage::HandleServerToPlayerCCMessage
// (0065C5AC..0065E13E). Senders: CSWSCreature::SendFeedbackMessage / BroadcastFloatyData / DisplayFloatyDataToSelf,
// CSWVirtualMachineCommands::ExecuteCommandFloatingText (CSWSMessage::SendServerToPlayerCCMessage minor 0Bh).
// Layout: WORD feedback id (16 bits), then per-id fields; each id builds one CSWCCMessageData (operator new(34h)) and
// passes it with an empty CExoString to CClientExoApp::AddFeedbackMessage(id, data, ""), which takes ownership.
// Dispatch: (id - 1) > E3h -> default 0065E103; else index byte [00663048 + id - 1] -> table 00662FCC (31 entries,
// entry 30 = default). Every non-overflow exit leaves through 00660377/00660382 (kDone); read overflow -> kFail.
// Quirks kept: after `mov esi, eax` the message pointer register is reused for the data object (no reads follow);
// id 222 (entry 28) creates its data without a MessageReadOverflow check; ids 4/47/50/51 assign the shared [esp+14]
// string (Frame::b); entry 6 (id 62) constructs an unused STR_RES; the new'd data pointer is used unchecked.
#include "7c6c75bdf0-claude_s2p_cc.hpp"

namespace s2pcc {
namespace {
// AddFeedbackMessage(id, data, CExoString("")): the "" temporary lives across the call and is destroyed after it.
inline void post(U id, void* data) {
    ExoString empty("");                                             // push 0073D71C (""); call 005E5A90
    add_feedback(id, data, &empty);                                  // push &empty, data, edi; call 005ED360
}                                                                    // ~empty (00660377/00660382 tail or inline)
// dst = ReadCExoString(0x20); ~temp
inline void read_str(Frame& f, void* dst) {
    Tmp t;
    str_assign(dst, t.set(kotor_fastcall_004d67e0(f.msg, nullptr, t.p(), 0x20)));
}
inline void set_int(void* d, I index, U value) { recovered_event_integer_004d76b0(d, nullptr, index, static_cast<I>(value)); }
inline void set_obj(void* d, I index, U id) { recovered_event_object_004d7780(d, nullptr, index, id); }
inline void set_str(void* d, I index, const void* s) { recovered_event_string_set_004d6ae0(d, nullptr, index, s); }

// ids 50/51 (entry 5) and 4/47 (entry 1): BOOL selects a localized string vs a base-item BYTE; result in Frame::b.
U item_or_locstring(Frame& f, U id, bool with_object) {
    U object = 0;
    if (with_object) object = rd_obj(f);                             // 0065DC5C..0065DC65 [esp+24]
    const U flag = rd_bool(f);                                       // 0065DAF2 / 0065DC69 ebx, [esp+1C]
    if (overflow(f, 1) != 0) return kFail;                           // 0065DAFB..0065DB0A / 0065DC70..0065DC7F
    U8 item = 0;                                                     // [esp+47] (only written on the BYTE path)
    if (flag == 1) {                                                 // 0065DB10 / 0065DC85 cmp ebx, 1
        Tmp text;                                                    // [esp+6BC] / [esp+6DC]
        ResRef sound;                                                // [esp+14C0] / [esp+14B0]
        void* ref = kotor_fastcall_00405ed0(&sound, nullptr);        // 0065DB15..0065DB1D CResRef()
        str_assign(&f.b, text.set(read_locstring(f, text.p(), ref, nullptr)));  // 0065DB22..0065DB5A (length_out = 0)
    } else {
        item = rd_byte(f, 8);                                        // 0065DB61..0065DB6A / 0065DCD6..0065DCDF
    }
    if (overflow(f, 1) != 0) return kFail;                           // 0065DB73..0065DB7E / 0065DCE8..0065DCF3
    if (flag == 0) {                                                 // 0065DB84..0065DB8A / 0065DCF9..0065DCFF
        Tmp name;                                                    // [esp+6CC] / [esp+6EC]
        void* base = kotor_m15b_base_item_array_get_base_item_005b31a0(rp(rules(), 0x34), nullptr,
                                                                       static_cast<I>(item));   // 0065DB8C..0065DBA2
        str_assign(&f.b, name.set(kotor_fastcall_005b4690(base, nullptr, name.p())));          // 0065DBA7..0065DBD1
    }
    void* d = new_message_data(0x34);                                // 0065DBD6..0065DBFE / 0065DD4C..0065DD74
    if (with_object) set_obj(d, 0, object);                          // 0065DD76..0065DD86
    set_str(d, 0, &f.b);                                             // 0065DC00..0065DC10 / 0065DD8B..0065DD94
    post(id, d);                                                     // 0065DC15..0065DC44 / 0065DD99..0065DDC8
    return kDone;                                                    // 0065DC49..0065DC57 / 0065DDCD..0065DDDB -> 00660382
}
}  // namespace

U case_11(Frame& f) {
    const U id = rd_word(f, 0x10);                                   // 0065C5AC..0065C5B9 movzx edi, ax
    if (overflow(f, 1) != 0) return kFail;                           // 0065C5B5..0065C5C3
    switch (id) {                                                    // 0065C5C9..0065C5DE byte table 00663048 -> 00662FCC
    case 1: {                                                        // entry 0 (0065C5E5)
        const U v = rd_dword(f);                                     // 0065C5E5..0065C5F2 ebx
        if (overflow(f, 1) != 0) return kFail;                       // 0065C5EE..0065C5FB
        void* d = new_message_data(0x34);                            // 0065C601..0065C629
        set_int(d, 0, v);                                            // 0065C62B..0065C63B
        post(id, d);                                                 // 0065C640..0065C66F
        return kDone;                                                // 0065C674..0065C67B -> 00660377
    }
    case 31: case 32: case 34: case 35: case 36: case 37: case 38: case 39: case 40: case 41: case 42: case 43:
    case 44: case 46: case 48: case 125: case 126: case 127: case 128: case 129: case 130: case 131: case 132:
    case 133: case 134: case 135: case 136: case 137: case 138: case 139: case 140: case 141: case 144: case 145:
    case 146: case 202: case 227: {                                  // entry 4 (0065C680)
        const U o = rd_obj(f);                                       // 0065C680..0065C68B ebx
        if (overflow(f, 1) != 0) return kFail;                       // 0065C687..0065C694
        void* d = new_message_data(0x34);                            // 0065C69A..0065C6C2
        set_obj(d, 0, o);                                            // 0065C6C4..0065C6D4
        post(id, d);                                                 // 0065C6D9..0065C707
        return kDone;                                                // 0065C70C..0065C713
    }
    case 5:                                                          // entry 2 (0065C718)
    case 63:                                                         // entry 7 (0065C98B)
    case 64:                                                         // entry 8 (0065CB0D)
    case 68:                                                         // entry 11 (0065CC90)
    case 69:                                                         // entry 12 (0065CD44)
    case 93:                                                         // entry 15 (0065CFB4)
    case 151:                                                        // entry 21 (0065DDE0)
    case 228: {                                                      // entry 29 (0065DE94)
        // Eight separately compiled but instruction-identical bodies: obj (ebx), dword ([esp+1C]); overflow; new;
        // SetObjectID(0, obj); SetInteger(0, dword); post. (e.g. 0065C718..0065C7C7 for id 5)
        const U o = rd_obj(f);                                       // ReadOBJECTIDClient -> ebx
        const U v = rd_dword(f);                                     // ReadDWORD(20h) -> [esp+1C]
        if (overflow(f, 1) != 0) return kFail;                       // MessageReadOverflow(1); jne 00662EE4
        void* d = new_message_data(0x34);                            // push 34h; operator_new; ctor
        set_obj(d, 0, o);                                            // SetObjectID(0, ebx)
        set_int(d, 0, v);                                            // SetInteger(0, [esp+1C])
        post(id, d);                                                 // CExoString(""); AddFeedbackMessage
        return kDone;                                                // -> 00660377
    }
    case 71:                                                         // entry 14 (0065C7CC)
    case 108: {                                                      // entry 17 (0065D9A2)
        const U v0 = rd_dword(f);                                    // 0065C7CC..0065C7D9 ebx
        const U v1 = rd_dword(f);                                    // 0065C7D5..0065C7E4 [esp+1C]
        if (overflow(f, 1) != 0) return kFail;                       // 0065C7E0..0065C7EF
        void* d = new_message_data(0x34);                            // 0065C7F5..0065C81D
        set_int(d, 0, v0);                                           // 0065C81F..0065C82F
        set_int(d, 1, v1);                                           // 0065C834..0065C83D
        post(id, d);                                                 // 0065C842..0065C871
        return kDone;                                                // 0065C876..0065C87D
    }
    case 62: {                                                       // entry 6 (0065C882)
        StrRes unused;                                               // 0065C882..0065C889 [esp+13F8] (never used)
        const U o = rd_obj(f);                                       // 0065C88E..0065C8A4 ebx
        const U v0 = rd_dword(f);                                    // 0065C8A0..0065C8AF [esp+1C]
        const U v1 = rd_dword(f);                                    // 0065C8AB..0065C8BC [esp+24]
        if (overflow(f, 1) != 0) return kFail;                       // 0065C8B8..0065C8E0 (~STR_RES)
        void* d = new_message_data(0x34);                            // 0065C8E5..0065C90A
        set_obj(d, 0, o);                                            // 0065C90C..0065C919
        set_int(d, 0, v0);                                           // 0065C91E..0065C927
        set_int(d, 1, v1);                                           // 0065C92C..0065C935
        post(id, d);                                                 // 0065C93A..0065C97A
        return kDone;                                                // 0065C97F..0065C986 ~STR_RES -> 00660377
    }
    case 66:                                                         // entry 9 (0065CA3F)
    case 67:                                                         // entry 10 (0065CBC1)
    case 70: {                                                       // entry 13 (0065CDF8)
        const U o = rd_obj(f);                                       // 0065CA3F..0065CA4A ebx
        const U v0 = rd_dword(f);                                    // 0065CA46..0065CA55 [esp+1C]
        const U v1 = rd_dword(f);                                    // 0065CA51..0065CA62 [esp+24]
        if (overflow(f, 1) != 0) return kFail;                       // 0065CA5E..0065CA6D
        void* d = new_message_data(0x34);                            // 0065CA73..0065CA9B
        set_obj(d, 0, o);                                            // 0065CA9D..0065CAAD
        set_int(d, 0, v0);                                           // 0065CAB2..0065CABB
        set_int(d, 1, v1);                                           // 0065CAC0..0065CAC9
        post(id, d);                                                 // 0065CACE..0065CAFC
        return kDone;                                                // 0065CB01..0065CB08
    }
    case 142: {                                                      // entry 18 (0065CEC6)
        const U o = rd_obj(f);                                       // 0065CEC6..0065CED1 ebx
        const U v0 = rd_dword(f);                                    // 0065CECD..0065CEDC [esp+1C]
        const U v1 = rd_dword(f);                                    // 0065CED8..0065CEEE [esp+24]
        const float value = rd_float(f, 0x3f800000u, 0x20);          // 0065CEE5..0065CEF7 ReadFLOAT(1.0f, 20h); fstp dword [esp+34]
        if (overflow(f, 1) != 0) return kFail;                       // 0065CEFB..0065CF06
        void* d = new_message_data(0x34);                            // 0065CF0C..0065CF34
        set_obj(d, 0, o);                                            // 0065CF36..0065CF46
        set_int(d, 0, v0);                                           // 0065CF4B..0065CF54
        set_int(d, 1, v1);                                           // 0065CF59..0065CF62
        kotor_fastcall_004d7720(d, nullptr, 0, value);               // 0065CF67..0065CF70 SetFloat(0, dword [esp+34])
        post(id, d);                                                 // 0065CF75..0065CFA3
        return kDone;                                                // 0065CFA8..0065CFAF
    }
    case 94: {                                                       // entry 16 (0065D068)
        ExoString text;                                              // 0065D068..0065D06F [esp+22C]
        const U o = rd_obj(f);                                       // 0065D074..0065D086 ebx
        read_str(f, &text);                                          // 0065D088..0065D0BD
        if (overflow(f, 1) != 0) return kFail;                       // 0065D0C2..0065D0E6 (~text)
        void* d = new_message_data(0x34);                            // 0065D0EB..0065D110
        set_obj(d, 0, o);                                            // 0065D112..0065D11F
        set_str(d, 0, &text);                                        // 0065D124..0065D130
        post(id, d);                                                 // 0065D135..0065D175
        return kDone;                                                // 0065D17A..0065D181 ~text -> 00660377
    }
    case 204: {                                                      // entry 27 (0065D186)
        ExoString text;                                              // 0065D186..0065D18D [esp+25C]
        read_str(f, &text);                                          // 0065D192..0065D1D2
        if (overflow(f, 1) != 0) return kFail;                       // 0065D1D7..0065D1FB
        void* d = new_message_data(0x34);                            // 0065D200..0065D225
        set_str(d, 0, &text);                                        // 0065D227..0065D23B
        post(id, d);                                                 // 0065D240..0065D27F
        return kDone;                                                // 0065D284..0065D28B
    }
    case 143:                                                        // entry 19 (0065D290)
    case 148: case 149:                                              // entry 20 (0065DA58)
    case 182: case 183: {                                            // entry 26 (0065DF48)
        const U v = rd_dword(f);                                     // 0065D290..0065D29D ebx
        if (overflow(f, 1) != 0) return kFail;                       // 0065D299..0065D2A6
        void* d = new_message_data(0x34);                            // 0065D2AC..0065D2D4
        set_int(d, 0, v);                                            // 0065D2D6..0065D2E6
        post(id, d);                                                 // 0065D2EB..0065D319
        return kDone;                                                // 0065D31E..0065D325
    }
    case 160: {                                                      // entry 22 (0065D32A)
        ExoString first;                                             // 0065D32A..0065D331 [esp+244]
        ExoString second;                                            // 0065D336..0065D348 [esp+26C]
        const U o = rd_obj(f);                                       // 0065D34D..0065D35C ebx
        read_str(f, &first);                                         // 0065D35E..0065D393
        read_str(f, &second);                                        // 0065D398..0065D3CD
        const U v = rd_dword(f);                                     // 0065D3D2..0065D3DF [esp+1C]
        if (overflow(f, 1) != 0) return kFail;                       // 0065D3DB..0065D417
        void* d = new_message_data(0x34);                            // 0065D41C..0065D441
        set_obj(d, 0, o);                                            // 0065D443..0065D450
        set_str(d, 0, &first);                                       // 0065D455..0065D461
        set_str(d, 1, &second);                                      // 0065D466..0065D472
        set_int(d, 0, v);                                            // 0065D477..0065D480
        post(id, d);                                                 // 0065D485..0065D4C5
        return kDone;                                                // 0065D4CA..0065D4E5
    }
    case 161: case 164:                                              // entry 23 (0065D4EA)
    case 162: case 165: {                                            // entry 24 (0065D63E) (identical body)
        ExoString text;                                              // 0065D4EA..0065D4F1 [esp+264]
        const U o = rd_obj(f);                                       // 0065D4F6..0065D514 ebx
        read_str(f, &text);                                          // 0065D508..0065D53F
        const U v0 = rd_dword(f);                                    // 0065D544..0065D551 [esp+1C]
        const U v1 = rd_dword(f);                                    // 0065D54D..0065D55E [esp+24]
        if (overflow(f, 1) != 0) return kFail;                       // 0065D55A..0065D582
        void* d = new_message_data(0x34);                            // 0065D587..0065D5AC
        set_obj(d, 0, o);                                            // 0065D5AE..0065D5BB
        set_str(d, 1, &text);                                        // 0065D5C0..0065D5CC (string index 1)
        set_int(d, 0, v0);                                           // 0065D5D1..0065D5DA
        set_int(d, 1, v1);                                           // 0065D5DF..0065D5E8
        post(id, d);                                                 // 0065D5ED..0065D62D
        return kDone;                                                // 0065D632..0065D639
    }
    case 163: {                                                      // entry 25 (0065D792)
        ExoString first;                                             // 0065D792..0065D799 [esp+23C]
        ExoString second;                                            // 0065D79E..0065D7B0 [esp+234]
        const U o = rd_obj(f);                                       // 0065D7B5..0065D7D0 ebx
        read_str(f, &first);                                         // 0065D7C4..0065D7FB
        const U v0 = rd_dword(f);                                    // 0065D800..0065D80D [esp+1C]
        const U v1 = rd_dword(f);                                    // 0065D809..0065D822 [esp+24]
        read_str(f, &second);                                        // 0065D816..0065D84F
        const U v2 = rd_dword(f);                                    // 0065D854..0065D861 [esp+34]
        const U v3 = rd_dword(f);                                    // 0065D85D..0065D86E [esp+4C]
        if (overflow(f, 1) != 0) return kFail;                       // 0065D86A..0065D8A6
        void* d = new_message_data(0x34);                            // 0065D8AB..0065D8D0
        set_obj(d, 0, o);                                            // 0065D8D2..0065D8DF
        set_str(d, 1, &first);                                       // 0065D8E4..0065D8F0
        set_str(d, 2, &second);                                      // 0065D8F5..0065D901
        set_int(d, 0, v0);                                           // 0065D906..0065D90F
        set_int(d, 1, v1);                                           // 0065D914..0065D91D
        set_int(d, 2, v2);                                           // 0065D922..0065D92B
        set_int(d, 3, v3);                                           // 0065D930..0065D939
        post(id, d);                                                 // 0065D93E..0065D97D
        return kDone;                                                // 0065D982..0065D99D
    }
    case 50: case 51:                                                // entry 5 (0065DAF2)
        return item_or_locstring(f, id, false);
    case 4: case 47:                                                 // entry 1 (0065DC5C)
        return item_or_locstring(f, id, true);
    case 13: {                                                       // entry 3 (0065DFE2)
        const U o = rd_obj(f);                                       // 0065DFE2..0065DFED ebx
        if (overflow(f, 1) != 0) return kFail;                       // 0065DFE9..0065DFF6
        void* d = new_message_data(0x34);                            // 0065DFFC..0065E024
        set_obj(d, 0, o);                                            // 0065E026..0065E036
        post(id, d);                                                 // 0065E03B..0065E069
        return kDone;                                                // 0065E06E..0065E075
    }
    case 222: {                                                      // entry 28 (0065E07A): no overflow check
        const U v = rd_dword(f);                                     // 0065E07A..0065E085 ebx
        void* d = new_message_data(0x34);                            // 0065E083..0065E0AD
        set_int(d, 0, v);                                            // 0065E0AF..0065E0BF
        post(id, d);                                                 // 0065E0C4..0065E0F2
        return kDone;                                                // 0065E0F7..0065E0FE
    }
    default:                                                         // entry 30 / id 0 / id > 228 (0065E103)
        post(id, nullptr);                                           // 0065E103..0065E132 AddFeedbackMessage(id, 0, "")
        return kDone;                                                // 0065E137..0065E13E -> 00660377
    }
}
}  // namespace s2pcc
