// Lane worker "s2p-cc": 00656D20 CSWCMessage::HandleServerToPlayerCCMessage(BYTE minor) RET 4 -> dispatcher, minors
// 1 / 4 / 24, and the two missing callees (005ED350 GetObjectName, 006500B0 GetSingleCustomTokenStringPart).
// The other minors live in claude_s2p_cc_<group>.cpp (see claude_s2p_cc.hpp for the frame/exit model).
#include "7c6c75bdf0-claude_s2p_cc.hpp"

using namespace s2pcc;

// =================================================================================================================
// 00656D20 CSWCMessage::HandleServerToPlayerCCMessage(BYTE minor) RET 4
// =================================================================================================================
extern "C" U __fastcall kotor_fastcall_00656d20(void* self, void*, U minor) {
    Frame f(self);                                   // 00656D4B..00656D74: [esp+0C], STR_RES [esp+54], [esp+14]
    switch (static_cast<U8>(minor)) {                // 00656D79..00656D8F movzx; dec; cmp 1Ah; ja 00662EE4; table 00662F34
    case 1: return case_01(f);                       // 00656D96
    case 2: return case_02(f);                       // 00656ECC
    case 3: return case_03(f);                       // 0065799D
    case 4: return case_04(f);                       // 0065AC7E
    case 5: return case_05(f);                       // 00658693
    case 6: return case_06(f);                       // 006594EC
    case 7: return case_07(f);                       // 0065A659
    case 8: return case_08(f);                       // 0065AF21
    case 9: return case_09(f);                       // 0065B4A0
    case 10: return case_10(f);                      // 0065A213
    case 11: return case_11(f);                      // 0065C5AC
    case 12: return case_12(f);                      // 0065F368
    case 13: return case_13(f);                      // 0065BC59
    case 14: return case_14(f);                      // 0065C35D
    case 15: return case_15(f);                      // 0065E143
    case 16: return case_16(f);                      // 0065E46D
    case 17: return case_17(f);                      // 0065A422
    case 18: return case_18(f);                      // 0065F56C
    case 19: return case_19(f);                      // 0065F018
    case 20: return case_20(f);                      // 006603D9
    case 21: return case_21(f);                      // 00661134
    case 22: return case_22(f);                      // 00661379
    case 23: return case_23(f);                      // 006619E1
    case 25: return case_25(f);                      // 00662767
    case 26: return case_26(f);                      // 00662A10
    case 27: return case_27(f);                      // 00662DA4
    default: return kFail;                           // minor 0, 24 (table entry 00662EE4), > 27
    }
}                                                    // 00662EE4..00662F30 / 00660387..006603D6: ~[14], ~[54], ~[0C]

namespace s2pcc {
// -----------------------------------------------------------------------------------------------------------------
// minor 1 (00656D96..00656EC7): "<object> <GUI string>" line, TLK 0x579. No K1 server sender.
// -----------------------------------------------------------------------------------------------------------------
U case_01(Frame& f) {
    const U id = rd_obj(f);                                          // 00656D96..00656D98 edi
    const U gui_id = rd_dword(f, 0x20);                              // 00656D9D..00656DA3 ebx
    if (overflow(f, 1) != 0) return kFail;                           // 00656DA8..00656DB5
    Tmp text;                                                        // [esp+344] (EH state 3 after the call)
    (void)text.set(gui_string(text.p(), gui_id));                    // 00656DBB..00656DDF
    object_name_or_gui(id, &f.b);                                    // 00656DD1..00656E28
    token(0, &f.b);                                                  // 00656E2D..00656E3A
    token(1, text.p());                                              // 00656E3F..00656E4F
    assign_fetch(&f.a, 0x579);                                       // 00656E54..00656E93
    append_a(f);                                                     // 00656E98..00656EB3 (a, 80h, 0)
    return kDone;                                                    // 00656EB8..00656EC7 ~[344] -> 00660382
}

// -----------------------------------------------------------------------------------------------------------------
// minor 4 (0065AC7E..0065AF1C): death line "<killer/victim>", TLK 0x57F. Sender: CSWSCreature::BroadcastDeathDataToParty.
// -----------------------------------------------------------------------------------------------------------------
U case_04(Frame& f) {
    ExoString name0;                                                 // 0065AC7E [esp+19C]
    ExoString name1;                                                 // 0065AC8A [esp+17C]
    StrRes unused0;                                                  // 0065AC9E [esp+13DC] (never used)
    StrRes unused1;                                                  // 0065ACB2 [esp+13C0] (never used)
    const U first = rd_obj(f);                                       // 0065ACC8..0065ACD8 [esp+24]
    const U second = rd_obj(f);                                      // 0065ACD6..0065ACE5 [esp+1C]
    const U value = rd_dword(f, 0x20);                               // 0065ACE1..0065ACF2 edi
    if (overflow(f, 1) != 0) return kFail;                           // 0065ACEE..0065AD4D (locals destroyed)
    object_name_or_gui(second, &name0);                              // 0065AD52..0065ADAA
    token(0, &name0);                                                // 0065ADAF..0065ADBF
    object_name_or_gui(first, &name1);                               // 0065ADC4..0065AE1D
    token(1, &name1);                                                // 0065AE22..0065AE32
    token_int(2, static_cast<I>(value));                             // 0065AE37..0065AE68
    assign_fetch(&f.a, 0x57f);                                       // 0065AE6D..0065AEAB
    append_a(f);                                                     // 0065AEB0..0065AECC
    return kDone;                                                    // 0065AED1..0065AF1C -> 00660382
}
}  // namespace s2pcc

// =================================================================================================================
// 005ED350 CClientExoApp::GetObjectName(ulong id, CExoString& out) RET 8: mov ecx,[ecx+4]; jmp 005F6640
// =================================================================================================================
extern "C" U __fastcall kotor_fastcall_005ed350(void* client_app, void*, U id, void* out) {
    return feedback_object_name_005f6640(rp(client_app, 4), nullptr, id, out);
}

// =================================================================================================================
// 006500B0 CSWCMessage::GetSingleCustomTokenStringPart(int value, ulong strref, CExoString* out) RET 0Ch (ECX unused)
// =================================================================================================================
extern "C" U __fastcall kotor_fastcall_006500b0(void*, void*, I value, U strref, void* out) {
    ExoString part;                                                  // 006500CA..006500D2 [esp+8]
    U result = 0;                                                    // 006500D0 esi
    if (value != 0) {                                                // 006500D7..006500E1
        token_int(0, value);                                         // 006500E7..0065010F SetCustomToken(0, CExoString(value))
        assign_fetch(&part, strref);                                 // 00650114..00650143 part = Fetch(strref, 0, 0, 0)
        assign_concat(out, out, &part);                              // 00650148..00650172 *out = *out + part
        result = 1;                                                  // 00650177
    }
    return result;                                                   // 0065017C..0065019F ~part
}
