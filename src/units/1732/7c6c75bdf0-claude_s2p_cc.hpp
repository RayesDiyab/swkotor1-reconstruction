#pragma once
// Lane worker "s2p-cc": CSWCMessage::HandleServerToPlayerCCMessage 00656D20 (client side of major 0x12 "CC" messages)
// split per minor into claude_s2p_cc*.cpp. Transcribed from the original swkotor.exe (read-only).
//
// Original: thiscall(CSWCMessage* this, BYTE minor) RET 4, one 0x14C4-byte frame. Shared frame locals (constructed
// in the prologue 00656D4B..00656D74, destroyed at both exits in reverse order):
//   [esp+0C] CExoString  -> Frame::a    (the composed line: Fetch result, AppendToMsgBuffer argument)
//   [esp+54] STR_RES     -> Frame::res  (00414DC0; only its CExoString at +0 is destroyed)
//   [esp+14] CExoString  -> Frame::b    (object name / token 0 scratch)
// Exits: 00660377/00660382/00660387 -> return 1 (kDone);  00662EE4 -> return 0 (kFail; read overflow, minor 24,
// out-of-range minors, minors 22/27 tails).
// Conventions: thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only; no original address
// is dereferenced (globals bound by rebuilt symbol, .rdata strings/constants written as literal values).
#include "6bd5403233-claude_dialogue_common.hpp"

namespace kotor_reconstruction {
struct StartupString { void* data; dlg::U length; int __thiscall BorrowedEquals005e5290(const char* text); };  // ==(char*)
void __cdecl StartupFormat005e5680(StartupString* out, const char* format, ...);                    // CExoString::Format
}
kotor_reconstruction::StartupString* __fastcall GuiConcat005e5d10(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out, const kotor_reconstruction::StartupString* rhs);      // CExoString::operator+
namespace kotor_menu_10_19 { struct String; struct Tlk { String* __thiscall simple_0041e8f0(String* out, unsigned id); }; }

extern "C" {
// ---- globals ----
extern void* kotor_cell_tlk_007a3a08;            // [007A3A08] CTlkTable*            [0384-tlk-cells]
extern void* kotor_cell_tlk_client_007a3a0c;     // [007A3A0C] CSWTlkTable* (client)  [0421]
extern void* kotor_cell_rules_007a3a28;          // [007A3A28] CSWRules*              [0421]

// ---- CSWMessage / CSWCMessage readers [w2_message_native] ----
dlg::U __fastcall kotor_fastcall_004d6230(void* msg, void*, dlg::U bits);             // MessageReadOverflow RET 4
dlg::U __fastcall kotor_fastcall_004d66f0(void* msg, void*);                          // ReadBOOL RET 0
unsigned char __fastcall kotor_fastcall_004d6720(void* msg, void*, dlg::U bits);      // ReadBYTE RET 4
unsigned short __fastcall kotor_fastcall_004d6750(void* msg, void*, dlg::U bits);     // ReadWORD RET 4
dlg::U __fastcall kotor_fastcall_004d6780(void* msg, void*, dlg::U bits);             // ReadDWORD RET 4
float __fastcall kotor_fastcall_004d67b0(void* msg, void*, dlg::U scale, dlg::U bits); // ReadFLOAT RET 8 (ST0)
void* __fastcall kotor_fastcall_004d67e0(void* msg, void*, void* result, dlg::U bits); // ReadCExoString RET 8
dlg::U __fastcall kotor_fastcall_00692630(void* msg, void*);                          // ReadOBJECTIDClient RET 0
void* __fastcall kotor_fastcall_00692660(void* msg, void*, void* result, void* sound, unsigned* length_out); // ReadCExoLocStringClient RET 0C

// ---- CSWCCMessageData ----
void* __fastcall recovered_message_data_construct_004d69f0(void* data, void*);         // CSWCCMessageData() [w1_native_b7]
void __fastcall recovered_event_integer_004d76b0(void* data, void*, dlg::I index, dlg::I value);   // SetInteger RET 8 [1017]
void __fastcall recovered_event_object_004d7780(void* data, void*, dlg::I index, dlg::U id);       // SetObjectID RET 8 [1017]
void __fastcall recovered_event_string_set_004d6ae0(void* data, void*, dlg::I index, const void* s); // SetString RET 8 [0886]
void __fastcall kotor_fastcall_004d7720(void* data, void*, dlg::I index, float value);             // SetFloat RET 8 [claude_combat_attack_damage]

// ---- strings / TLK / feedback ----
void* __fastcall resman_string_from_int_005e5bc0(void* s, void*, dlg::I value);         // CExoString(int) [0266]
void* __fastcall GuiAssign005e5140(void* s, void*, const char* text);                    // CExoString::operator=(char*) [0093]
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, dlg::U index, const void* value); // CTlkTable::SetCustomToken RET 8 [0583]
void* __fastcall feedback_fetch_005ee390(void* tlk, void*, void* out, dlg::U strref, void* object, dlg::U unused,
                                         dlg::U flags);                                  // CSWTlkTable::Fetch RET 14h [feedback_lowers]
void* __fastcall kotor_fastcall_005edeb0(void* client, void*, void* out, dlg::U id);     // CClientExoApp::GetGUIString RET 8 [tlk_token]
dlg::U __fastcall feedback_object_name_005f6640(void* internal, void*, dlg::U id, void* out); // ClientInternal::GetObjectName RET 8
void __fastcall feedback_append_0062b5c0(void* gui, void*, void* text, dlg::U color, dlg::U flag); // CGuiInGame::AppendToMsgBuffer RET 0C
void __fastcall feedback_set_info_0063d2d0(void* object, void*, void* text);             // CSWCObject::SetFeedbackInfo RET 4
void* __fastcall feedback_str_res_construct_00414dc0(void* res, void*);                  // STR_RES::STR_RES [feedback_lowers]
void __fastcall recovered_client_add_feedback_message_005ed360(void* client, void*, dlg::U kind, void* data,
                                                               void* text);              // CClientExoApp::AddFeedbackMessage RET 0C [w3_feedback]
// ---- rules / objects ----
void* __fastcall kotor_rules_get_feat_00550c00(void* rules, void*, unsigned short feat);  // CSWRules::GetFeat RET 4 [0428]
void* __fastcall kotor_m15b_base_item_array_get_base_item_005b31a0(void* items, void*, dlg::I id); // GetBaseItem RET 4 [0820]
void* __fastcall kotor_fastcall_005b4690(void* base_item, void*, void* out);              // CSWBaseItem::GetNameText RET 4 [w4 panels_debug]
void* __fastcall kotor_m15b_spell_array_get_spell_0059b6d0(void* spells, void*, dlg::I id); // CSWSpellArray::GetSpell RET 4 [0820]
void* __fastcall kotor_m15b_feat_get_name_text_005cd760(void* feat, void*, void* out);   // CSWFeat::GetNameText RET 4 [0820]
// (CClientExoApp::GetGameObject 005ED580 is declared by claude_dialogue_common.hpp? no -> declared here)
void* __fastcall kotor_fastcall_005ed580(void* client, void*, dlg::U id);                 // CClientExoApp::GetGameObject RET 4 [0701]

// ---- reconstructed in claude_s2p_cc.cpp (missing callees) ----
dlg::U __fastcall kotor_fastcall_005ed350(void* client, void*, dlg::U id, void* out);    // CClientExoApp::GetObjectName RET 8
dlg::U __fastcall kotor_fastcall_006500b0(void* message, void*, dlg::I value, dlg::U strref, void* out); // GetSingleCustomTokenStringPart RET 0C

// ---- export ----
dlg::U __fastcall kotor_fastcall_00656d20(void* self, void*, dlg::U minor);             // HandleServerToPlayerCCMessage RET 4
}

namespace s2pcc {
using namespace dlg;
constexpr U kDone = 1;   // exit through 00660387 (EAX = 1)
constexpr U kFail = 0;   // exit through 00662EE4 (EAX = 0)

using SS = kotor_reconstruction::StartupString;
inline SS* ss(void* s) { return static_cast<SS*>(s); }

// STR_RES {CExoString text; CResRef sound; DWORD duration} (28 bytes). Its destruction is only ~CExoString(+0).
struct StrRes {
    alignas(4) unsigned char raw[28];
    StrRes() { (void)feedback_str_res_construct_00414dc0(raw, nullptr); }
    StrRes(const StrRes&) = delete;
    StrRes& operator=(const StrRes&) = delete;
    ~StrRes() { frame_LogStringDtor_005e5c20(raw, nullptr); }
};

// A CExoString constructed by a callee into caller storage (returned by value / constructed temporary). It is destroyed
// at scope end only once the constructing call has returned (mirrors the EH state the original sets after the call).
struct Tmp {
    alignas(4) unsigned char raw[8];
    bool live = false;
    Tmp() = default;
    Tmp(const Tmp&) = delete;
    Tmp& operator=(const Tmp&) = delete;
    void* p() { return raw; }
    void* set(void* r) { live = true; return r; }
    ~Tmp() { if (live) frame_LogStringDtor_005e5c20(raw, nullptr); }
};

struct Frame {
    void* msg;          // ESI: CSWCMessage* this
    ExoString a;        // [esp+0C]
    StrRes res;         // [esp+54]
    ExoString b;        // [esp+14]
    explicit Frame(void* m) : msg(m) {}
};

// ---- message reads ----
inline U rd_obj(Frame& f) { return kotor_fastcall_00692630(f.msg, nullptr); }
inline U rd_dword(Frame& f, U bits = 0x20) { return kotor_fastcall_004d6780(f.msg, nullptr, bits); }
inline U16 rd_word(Frame& f, U bits = 0x10) { return kotor_fastcall_004d6750(f.msg, nullptr, bits); }
inline U8 rd_byte(Frame& f, U bits = 8) { return kotor_fastcall_004d6720(f.msg, nullptr, bits); }
inline U rd_bool(Frame& f) { return kotor_fastcall_004d66f0(f.msg, nullptr); }
inline float rd_float(Frame& f, U scale_bits, U bits) { return kotor_fastcall_004d67b0(f.msg, nullptr, scale_bits, bits); }
inline U overflow(Frame& f, U bits = 1) { return kotor_fastcall_004d6230(f.msg, nullptr, bits); }

// ---- app / gui ----
inline void* tlk() { return kotor_cell_tlk_007a3a08; }
inline void* tlk_client() { return kotor_cell_tlk_client_007a3a0c; }
inline void* rules() { return kotor_cell_rules_007a3a28; }

// ---- raw primitives (each is exactly one original call) ----
inline void* fetch(void* out, U strref, void* object = nullptr, U unused = 0, U flags = 0) {
    return feedback_fetch_005ee390(tlk_client(), nullptr, out, strref, object, unused, flags);   // CSWTlkTable::Fetch
}
inline void* gui_string(void* out, U id) { return kotor_fastcall_005edeb0(client(), nullptr, out, id); }
inline U object_name(U id, void* out) { return kotor_fastcall_005ed350(client(), nullptr, id, out); }
inline void token(U index, const void* value) { kotor_fastcall_0041db50(tlk(), nullptr, index, value); }
inline void append(void* text, U color, U flag) { feedback_append_0062b5c0(in_game_gui(), nullptr, text, color, flag); }
inline void* concat(void* out, void* lhs, const void* rhs) { return GuiConcat005e5d10(ss(lhs), nullptr, ss(out), static_cast<const SS*>(rhs)); }
inline void* from_int(void* out, I value) { return resman_string_from_int_005e5bc0(out, nullptr, value); }
inline void* from_text(void* out, const char* text) { return frame_LogStringCtor_005e5a90(out, nullptr, text); }
inline void assign_text(void* s, const char* text) { (void)GuiAssign005e5140(s, nullptr, text); }
inline const char* cstr(void* s) { return WindowCStr005e5670(s, nullptr); }
inline I length(void* s) { return static_cast<I>(async_string_length_005e5790(s, nullptr)); }
inline int equals_text(void* s, const char* text) { return ss(s)->BorrowedEquals005e5290(text); }
inline void* simple_string(void* out, U strref) {
    return reinterpret_cast<kotor_menu_10_19::Tlk*>(tlk())->simple_0041e8f0(static_cast<kotor_menu_10_19::String*>(out), strref);
}
inline void* read_locstring(Frame& f, void* out, void* sound, unsigned* length_out) {
    return kotor_fastcall_00692660(f.msg, nullptr, out, sound, length_out);
}
inline void* game_object(U id) { return kotor_fastcall_005ed580(client(), nullptr, id); }

// ---- recurring original sequences (each helper documents the exact call order it stands for) ----
// dst = Fetch(strref, object, unused, flags); ~temp
inline void assign_fetch(void* dst, U strref, void* object = nullptr, U unused = 0, U flags = 0) {
    Tmp t; str_assign(dst, t.set(fetch(t.p(), strref, object, unused, flags)));
}
// dst = GetGUIString(id); ~temp
inline void assign_gui(void* dst, U id) { Tmp t; str_assign(dst, t.set(gui_string(t.p(), id))); }
// dst = simple(strref); ~temp
inline void assign_simple(void* dst, U strref) { Tmp t; str_assign(dst, t.set(simple_string(t.p(), strref))); }
// if (!GetObjectName(id, dst)) dst = GetGUIString(0 /*the EAX=0 return*/); ~temp
inline void object_name_or_gui(U id, void* dst) {
    const U r = object_name(id, dst);
    if (r == 0) assign_gui(dst, r);
}
// SetCustomToken(index, CExoString(value)); ~temp
inline void token_int(U index, I value) { Tmp t; token(index, t.set(from_int(t.p(), value))); }
// SetCustomToken(index, GetGUIString(id)); ~temp
inline void token_gui(U index, U id) { Tmp t; token(index, t.set(gui_string(t.p(), id))); }
// SetCustomToken(index, Fetch(strref, ...)); ~temp
inline void token_fetch(U index, U strref, void* object = nullptr, U unused = 0, U flags = 0) {
    Tmp t; token(index, t.set(fetch(t.p(), strref, object, unused, flags)));
}
// dst = dst_lhs + rhs (operator+ into a temporary, then operator=, then ~temp)
inline void assign_concat(void* dst, void* lhs, const void* rhs) { Tmp t; str_assign(dst, t.set(concat(t.p(), lhs, rhs))); }
// AppendToMsgBuffer(&f.a, 0x80, 0): the common tail
inline void append_a(Frame& f, U color = 0x80, U flag = 0) { append(&f.a, color, flag); }
// new CSWCCMessageData: push 34h; call operator_new; test eax; je -> esi = 0; else esi = ctor(eax). The EH state set
// around the ctor frees the block if the ctor throws (new-expression cleanup). The result is used unchecked.
inline void* new_message_data(U size = 0x34) {
    void* mem = live_operator_new(size);
    if (mem == nullptr) return nullptr;
    try {
        return recovered_message_data_construct_004d69f0(mem, nullptr);
    } catch (...) {
        live_free(mem);
        throw;
    }
}
inline void add_feedback(U kind, void* data, void* text) {
    recovered_client_add_feedback_message_005ed360(client(), nullptr, kind, data, text);
}

// ---- per-minor bodies (claude_s2p_cc_<group>.cpp); each returns kDone or kFail per the original exit taken ----
U case_01(Frame& f);  U case_02(Frame& f);  U case_03(Frame& f);  U case_04(Frame& f);  U case_05(Frame& f);
U case_06(Frame& f);  U case_07(Frame& f);  U case_08(Frame& f);  U case_09(Frame& f);  U case_10(Frame& f);
U case_11(Frame& f);  U case_12(Frame& f);  U case_13(Frame& f);  U case_14(Frame& f);  U case_15(Frame& f);
U case_16(Frame& f);  U case_17(Frame& f);  U case_18(Frame& f);  U case_19(Frame& f);  U case_20(Frame& f);
U case_21(Frame& f);  U case_22(Frame& f);  U case_23(Frame& f);  U case_25(Frame& f);  U case_26(Frame& f);
U case_27(Frame& f);
}  // namespace s2pcc
