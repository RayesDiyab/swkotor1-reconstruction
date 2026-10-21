// Lane worker "s2p-chatinv": CSWCMessage::HandleServerToPlayerChat 00654CC0 (client side of the server chat / bark /
// feedback-string message, major 0x0B in the client dispatcher 0066A640) plus its missing callee
// CSWCObject::NewVocalization 0063D1D0.
// Transcribed from the original swkotor.exe (read-only); statement comments cite original instruction addresses and the
// original stack slot ([esp+NN] after the prologue's three register pushes).
// ABI: original BOOL __thiscall CSWCMessage::HandleServerToPlayerChat(BYTE minor) RET 4, both returns verified
// (00655274 xor eax,eax / 0065528A ret 4 and 00655DF3 mov eax,1 / 00655E03 ret 4) ->
//   extern "C" unsigned __fastcall kotor_fastcall_00654cc0(void* self, void*, unsigned minor)   (@12).
// Conventions as claude_dialogue_common.hpp (thiscall -> __fastcall(self, unused_edx, ...); game heap only; globals by
// rebuilt symbol; .rdata constants as their proven literal values). The original's SEH frame only runs the CExoString
// destructors on unwind; the RAII locals below are declared in the original construction order (18, 2C, C8, 138, 10,
// 38, 80/88), so normal and failing exits destroy 80, 38, 10, 2C, 18 in the original order (00655219.. / 00655D8F..).
// QUIRKs kept (each marked below): the discarded CExoString::operator== at 00654E88, the self-assignment at
// 0065539F, TLK strref 0 fetched as the channel prefix, the three identical message-buffer concatenations
// (00655A05..00655B11) including the unused CSWCObject::GetPortrait probe, the sprintf into an unused buffer
// (00655BAD), the colour taken from the float bits of [esp+64] for minor 7, and the direct call of the empty
// 0063CFC0 (ret 10h) with a freshly built string.
#include "6bd5403233-claude_dialogue_common.hpp"

using namespace dlg;

// ---------------------------------------------------------------------------------------------------------------
// Providers (c19 map, real bodies; none in the c19 trap census) not declared by the common header
// ---------------------------------------------------------------------------------------------------------------
namespace kotor_reconstruction {
struct StartupString { void* data; U length; int __thiscall BorrowedEquals005e5290(const char* text); };  // 0077
}
using SS = kotor_reconstruction::StartupString;
SS* __fastcall GuiConcat005e5d10(SS* self, void*, SS* out, const SS* rhs);               // 0093 CExoString::operator+
namespace kotor_menu_10_19 { struct Tlk; }
namespace tlk_core { void __fastcall ParseStr0041dd30(kotor_menu_10_19::Tlk*, void*, SS*); }   // 0389 CTlkTable::ParseStr
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void* self, void*, const void* resref);         // 0680 CExoSoundSource(CResRef)
void __fastcall SourceSetPriorityGroup005d5900(void* self, void*, U group);             // 0680 SetPriorityGroup RET 4
}

extern "C" {
extern void* kotor_cell_tlk_007a3a08;                       // [007A3A08] CTlkTable*
extern void* kotor_cell_tlk_client_007a3a0c;                // [007A3A0C] CSWTlkTable* (0421)
extern void* kotor_cell_sound_007a39ec;                     // [007A39EC] CExoSound*
extern int recovered_log_conversations_00833a9c;            // [00833A9C] (claude_dialogue_start.obj)

// w2_message_native.obj
U __fastcall kotor_fastcall_004d6230(void* msg, void*, U bits);                           // MessageReadOverflow RET 4
U __fastcall kotor_fastcall_004d6240(void* msg, void*, U bits);                           // MessageReadUnderflow RET 4
U __fastcall kotor_fastcall_004d6780(void* msg, void*, U bits);                           // ReadDWORD RET 4
float __fastcall kotor_fastcall_004d67b0(void* msg, void*, U scale_bits, U bits);         // ReadFLOAT RET 8 (ST0)
void* __fastcall kotor_fastcall_004d67e0(void* msg, void*, void* result, U bits);         // ReadCExoString RET 8
void* __fastcall kotor_fastcall_004d6180(void* msg, void*, void* result, U size);         // ReadCResRef RET 8
U __fastcall kotor_fastcall_00692630(void* msg, void*);                                   // ReadOBJECTIDClient RET 0
void* __fastcall kotor_fastcall_00692660(void* msg, void*, void* result, void* resref, U* extra); // ReadCExoLocStringClient RET 0Ch
// strings / resrefs / TLK
void* __fastcall frame_LogAssign_005e5140(void* s, void*, const char* text);             // CExoString::operator=(char*)
const char* __fastcall frame_LogCStr_005e5670(void* s, void*);                           // CExoString::CStr
void* __fastcall async_ref_construct_string_00406d60(void* r, void*, const void* s);    // CResRef(CExoString const&)
void* __fastcall feedback_str_res_construct_00414dc0(void* res, void*);                  // STR_RES::STR_RES
U __fastcall tlk_fetch_0041e550(void* tlk, void*, U strref, void* out, U flag);          // CTlkTable::Fetch RET 0Ch
U __fastcall kotor_fastcall_00674b40(void* tlk, void*);                                  // CSWTlkTable::GetObjectGender
int __cdecl format_006fadb0(char* out, const char* format, ...);                          // sprintf
// client app / GUI / objects
void* __fastcall client_get_player_creature_005ed540(void* client, void*);               // GetPlayerCreature
void* __fastcall kotor_fastcall_005ed580(void* client, void*, U id);                     // GetGameObject RET 4
void* __fastcall recovered_client_get_player_by_id_005ed590(void* client, void*, U id);  // GetPlayerByGameObjectID RET 4
U __fastcall kotor_fastcall_005ed350(void* client, void*, U id, void* out);              // GetObjectName RET 8 (s2p-cc)
void __fastcall feedback_append_0062b5c0(void* gui, void*, void* text, U color, U flag); // CGuiInGame::AppendToMsgBuffer
void* __fastcall kotor_m15b_creature_stats_get_full_name_00647ce0(void* stats, void*, void* out); // GetFullName
void* __fastcall kotor_m15e_slot00c_self_00641db0(void* self, void*);                    // 00641DB0 self_return
void __fastcall light_manager_void_return_16_0063cfc0(void* self, void*, void* a, U b, U c, U d); // 0063CFC0 ret 10h
// sound
void __fastcall kotor_m15_sound_source_set_3d_005d5910(void* source, void*, U is3d);     // Set3D RET 4
void __fastcall kotor_m15_sound_source_set_position_005d59e0(void* source, void*, const void* v, U flag); // RET 8
void __fastcall kotor_fastcall_005d5930(void* source, void*);                            // CExoSoundSource::Play
void __fastcall kotor_m15_sound_source_stop_005d5a20(void* source, void*);               // CExoSoundSource::Stop
U __fastcall w5_sound_play_one_shot_005d5e00(void* sound, void*, void* ref, U group, U delay, U volume, U fixed,
                                             U pitch);                                    // PlayOneShotSound RET 18h
U __fastcall kotor_m15_sound_play_3d_one_shot_005d5e10(void* sound, void*, void* ref, U x, U y, U z, U z_offset,
                                                       U group, U delay, U volume, U fixed, U pitch); // RET 28h

// ---------------- exports ----------------
void __fastcall kotor_fastcall_0063d1d0(void* object, void*, const void* resref, U flag);
U __fastcall kotor_fastcall_00654cc0(void* self, void*, U minor);
}

namespace {
constexpr U k_one = 0x3f800000u;            // 1.0f   (00654DF6, ReadFLOAT scale)
constexpr U k_three_quarters = 0x3f400000u; // 0.75f  (00654EBC)
constexpr float k_bark_height = 2.0f;       // .rdata [00741BE0] = 2.0f
constexpr U k_strref_none = 0xffffffffu;    // 00654E57 cmp eax,-1
constexpr U k_strref_someone = 0x550;       // 00655341 push 550h
// .rdata strings: [0073D71C] "", [0073D5D8] " ", [00748ED0] ": ", [00741558] "%u", [0074394C] "[", [00751EA4] "]: "
const char k_empty[] = "";
const char k_space[] = " ";
const char k_colon[] = ": ";
const char k_format_u[] = "%u";
const char k_open[] = "[";
const char k_close[] = "]: ";

SS* ss(void* p) { return static_cast<SS*>(p); }
bool str_equals(void* s, const char* text) { return ss(s)->BorrowedEquals005e5290(text) != 0; }
void* concat(void* lhs, void* out, const void* rhs) { return GuiConcat005e5d10(ss(lhs), nullptr, ss(out), static_cast<const SS*>(rhs)); }
void assign_text(void* s, const char* text) { (void)frame_LogAssign_005e5140(s, nullptr, text); }

// CExoString storage constructed by the callee (by-value return / operator+ result), destroyed at scope end.
struct RawStr {
    void* data;
    U length;
    RawStr() {}
    RawStr(const RawStr&) = delete;
    RawStr& operator=(const RawStr&) = delete;
    ~RawStr() { frame_LogStringDtor_005e5c20(this, nullptr); }
};
static_assert(sizeof(RawStr) == 8, "CExoString");
// STR_RES {CExoString text; CResRef sound; float length} (0x1C) built by STR_RES::STR_RES 00414DC0; its inlined
// destructor only destroys the text (00654E9A..00654EA2).
struct StrRes {
    unsigned char bytes[0x1c];
    StrRes() { (void)feedback_str_res_construct_00414dc0(bytes, nullptr); }
    StrRes(const StrRes&) = delete;
    StrRes& operator=(const StrRes&) = delete;
    ~StrRes() { frame_LogStringDtor_005e5c20(bytes, nullptr); }
};
// [esp+80] STR_RES built inline by the prologue: CExoString() at 00654D62 then CResRef() on +8 at 00654D76; the
// length dword [esp+98] is never initialised. Destroyed (text only) at 00655219 / 00655D8F.
struct InlineStrRes {
    ExoString text;
    ResRef sound;
    U length;
    InlineStrRes() { resref_default(&sound); }
};
static_assert(sizeof(InlineStrRes) == 0x1c, "STR_RES");

U read_overflow(void* m) { return kotor_fastcall_004d6230(m, nullptr, 1); }
U read_underflow(void* m) { return kotor_fastcall_004d6240(m, nullptr, 1); }

// CSWTlkTable token pass: +58 = object, +3C = gender, +5C bit0 set/cleared, ParseStr(text)
void tlk_parse_pair(void* object, void* first, void* second) {
    void* tlk = kotor_cell_tlk_client_007a3a0c;                                          // 00654FE4
    wp(tlk, 0x58, object);                                                               // 00654FEC
    U8 gender = static_cast<U8>(kotor_fastcall_00674b40(tlk, nullptr));                   // 00654FEF
    wr8(tlk, 0x3c, gender);                                                              // 00654FF4
    wr(tlk, 0x5c, rd(tlk, 0x5c) | 1u);                                                   // 00654FF7..00655004
    tlk_core::ParseStr0041dd30(static_cast<kotor_menu_10_19::Tlk*>(tlk), nullptr, ss(first)); // 00654FFA..00655007
    tlk = kotor_cell_tlk_client_007a3a0c;                                                // 0065500C
    wp(tlk, 0x58, object);                                                               // 00655014
    gender = static_cast<U8>(kotor_fastcall_00674b40(tlk, nullptr));                      // 00655017
    wr(tlk, 0x5c, rd(tlk, 0x5c) & 0xfffffffeu);                                          // 0065501C..00655026
    wr8(tlk, 0x3c, gender);                                                              // 0065502C
    tlk_core::ParseStr0041dd30(static_cast<kotor_menu_10_19::Tlk*>(tlk), nullptr, ss(second)); // 00655022..0065502F
}

// name = name + ": " + Fetch(0).text   (jump-table cases 1/2/3/5: 00655449.., 0065553B.., 00655636.., 00655731..)
void prefix_with_strref0(void* name, InlineStrRes* fetched) {
    (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, 0, fetched, kByte0073ecd0); // QUIRK: strref 0
    ExoString fetched_text(frame_LogCStr_005e5670(&fetched->text, nullptr));            // CStr -> CExoString(char*)
    ExoString colon(k_colon);                                                            // [00748ED0]
    RawStr sum1;
    (void)concat(name, &sum1, &colon);                                                   // name + ": "
    RawStr sum2;
    (void)concat(&sum1, &sum2, &fetched_text);                                           // + fetched text
    str_assign(name, &sum2);
}                                                                                         // dtors sum2, sum1, colon, text
}  // namespace

// =================================================================================================================
// 0063D1D0 CSWCObject::NewVocalization(CResRef const& resref, int flag) RET 8
// =================================================================================================================
extern "C" void __fastcall kotor_fastcall_0063d1d0(void* object, void*, const void* resref, U flag) {
    if (void* old = rp(object, 0xc8)) (void)vcall<void*>(old, 0, 1u);                     // 0063D1E9..0063D1F7 delete
    void* const memory = live_operator_new(8);                                            // 0063D1F9..0063D200 new(8)
    if (memory != nullptr) {                                                              // 0063D207..0063D211
        void* source;
        try {
            source = kotor_sound_rev3::SourceConstruct005d60e0(memory, nullptr, resref);  // 0063D213..0063D21A
        } catch (...) {
            live_free(memory);                                                            // EH state 0: delete memory
            throw;
        }
        wp(object, 0xc8, source);                                                         // 0063D223
        wr(object, 0xd0, flag);                                                           // 0063D21F..0063D229
        return;
    }
    wr(object, 0xc8, 0);                                                                  // 0063D245..0063D247
    wr(object, 0xd0, flag);                                                               // 0063D24D..0063D251
}

// =================================================================================================================
// 00654CC0 CSWCMessage::HandleServerToPlayerChat(BYTE minor) RET 4
// =================================================================================================================
extern "C" U __fastcall kotor_fastcall_00654cc0(void* self, void*, U minor_arg) {
    const U8 minor = static_cast<U8>(minor_arg);                    // byte [ebp+8] everywhere
    ExoString text;                                                 // [esp+18] 00654CE6..00654D02 (state 0)
    U position[3] = {0, 0, 0};                                      // [esp+68..70] 00654CEA..00654CFA
    ExoString echo;                                                 // [esp+2C] 00654D09..00654D14 (state 1)
    ResRef sound; resref_default(&sound);                           // [esp+C8] 00654D19..00654D28
    ResRef sound_default; resref_default(&sound_default);           // [esp+138] 00654D2D..00654D34
    ExoString name;                                                 // [esp+10] 00654D39..00654D3D (state 2)
    ExoString display;                                              // [esp+38] 00654D42..00654D4E (state 3)
    InlineStrRes fetched;                                           // [esp+80]/[esp+88] 00654D53..00654D76 (4/5)
    U locstring_extra = 0;                                          // [esp+A4] 00654D82
    void* const client_app = client();                              // [esp+34] 00654D7B..00654D8C
    void* creature = nullptr;                                       // [esp+40] 00654D9C
    void* object = nullptr;                                         // [esp+44] 00654DA0
    U speaker_id = kInvalid;                                        // [esp+28] 00654DA4
    U announce = 1;                                                 // [esp+A8] 00654DAC
    U bark = 0;                                                     // [esp+78] 00654DB7
    U tint;                                                         // [esp+64]
    void* speaker = nullptr;                                        // ebx 00654D80
    if (read_overflow(self) != 0) return 0;                         // 00654D90..00654DC2 -> 00655219

    if (minor != 5) speaker_id = kotor_fastcall_00692630(self, nullptr); // 00654DCB..00654DDA / 00654E32..00654E37
    if (minor == 0xc || minor == 0xb) {                             // 00654DDE..00654DEB
        // ---- 00654EBA: bark with a localized string, sound and a TLK-token object ----
        tint = k_three_quarters;                                    // 00654EBC
        bark = 1;                                                   // 00654EC4
        const U token_object_id = kotor_fastcall_00692630(self, nullptr); // 00654EBA..00654ED8 ebx
        {
            RawStr got;                                             // [esp+20]
            str_assign(&text, kotor_fastcall_00692660(self, nullptr, &got, &sound_default, &locstring_extra)); // ..00654EFC
        }                                                           // 00654F01..00654F0D
        {
            ResRef read;                                            // [esp+48]
            resref_assign(&sound, kotor_fastcall_004d6180(self, nullptr, &read, 0x10)); // 00654F12..00654F30 (tmp F4)
        }
        if (gui_load_resref_is_valid_00406030(&sound, nullptr) == 0) // 00654F35..00654F43
            resref_assign(&sound, &sound_default);                  // 00654F45..00654F59
        void* const silenced = kotor_fastcall_005ed580(client_app, nullptr, kotor_fastcall_00692630(self, nullptr)); // 00654F5E..00654F6F edi
        if (silenced != nullptr && vcall<void*>(silenced, 0xc) != nullptr) { // 00654F71..00654F7E
            void* const o = vcall<void*>(silenced, 0xc);            // 00654F80..00654F84
            void* const source = rp(o, 0xc8);                       // 00654F87
            object = o;                                             // 00654F8F [esp+44]
            if (source != nullptr && rd(o, 0xd0) != 0)              // 00654F8D..00654F9D
                kotor_m15_sound_source_stop_005d5a20(source, nullptr); // 00654F9F
        }
        const U skip_if_empty = kotor_fastcall_004d6780(self, nullptr, 0x20); // 00654FA4..00654FB6 edi
        if (str_equals(&text, k_empty) && skip_if_empty == 1) return 1; // 00654FAD..00654FC4 -> 00655D8F
        void* const token_object = kotor_fastcall_005ed580(client_app, nullptr, token_object_id);  // 00654FCA..00654FD4 ebx
        str_assign(&echo, &text);                                   // 00654FD6..00654FDF
        tlk_parse_pair(token_object, &echo, &text);                 // 00654FE4..0065502F
    } else {
        tint = k_one;                                               // 00654DF6
        if (minor == 8 || minor == 9 || minor == 0xa || minor == 0xf) {
            StrRes res;                                             // [esp+48] 00654E3D..00654E41 (state 7)
            const U strref = kotor_fastcall_004d6780(self, nullptr, 0x20); // 00654E46..00654E52
            if (strref != k_strref_none &&                          // 00654E57
                tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, strref, res.bytes, kByte0073ecd0) != 0) // ..00654E78
                str_assign(&text, res.bytes);                       // 00654E7A..00654E83
            (void)str_equals(&text, k_empty);                       // QUIRK 00654E88..00654E91: result discarded
        } else {                                                    // 00654E96..00654EA2 ~STR_RES (text)
            RawStr got;                                             // [esp+20]
            str_assign(&text, kotor_fastcall_004d67e0(self, nullptr, &got, 0x20)); // 00654E0C..00654E27
        }                                                           // 00654E2C..00654EA2
        str_assign(&echo, &text);                                   // 00654EA7..00654EB0
    }

    assign_text(&name, k_empty);                                    // 00655038..00655041
    if (minor == 2 || minor == 4 || minor == 6) {                   // 00655046..00655053
        bark = 1;                                                   // 00655062 [esp+78]
        (void)kotor_fastcall_004d67b0(self, nullptr, k_one, 0x20);   // 00655059..00655072 fstp st(0)
        (void)kotor_fastcall_004d67b0(self, nullptr, k_one, 0x20);   // 00655074..00655082
        (void)kotor_fastcall_004d67b0(self, nullptr, k_one, 0x20);   // 00655084..00655092
        {
            ResRef none;                                            // [esp+48]
            RawStr got;                                             // [esp+20]
            str_assign(&name, kotor_fastcall_00692660(self, nullptr, &got, kotor_fastcall_00405ed0(&none, nullptr), nullptr));
        }                                                           // 00655094..006550CA
        ExoString space(k_space);                                   // [esp+9C] 006550CF..006550DB (state A)
        ResRef none;                                                // [esp+48]
        RawStr second;                                              // [esp+B8]
        void* const got = kotor_fastcall_00692660(self, nullptr, &second, kotor_fastcall_00405ed0(&none, nullptr), nullptr); // ..00655103
        RawStr sum1;                                                // [esp+B0]
        (void)concat(&name, &sum1, &space);                         // 00655105..00655121
        RawStr sum2;                                                // [esp+20]
        (void)concat(&sum1, &sum2, got);                            // 00655126..00655136
        str_assign(&name, &sum2);                                   // 0065513B..00655148
    }                                                               // 0065514D..00655195 dtors 20, B0, B8, 9C

    if (read_overflow(self) != 0) return 0;                         // 0065519E..006551A9
    if (read_underflow(self) != 0) return 0;                        // 006551AB..006551B6
    if (minor != 5 && minor != 0xf) {                               // 006551B8..006551C1
        speaker = kotor_fastcall_005ed580(client_app, nullptr, speaker_id);                        // 006551C3..006551D1
        if (speaker == nullptr && bark == 0) return 0;              // 006551D3..006551DD
    }
    if (speaker != nullptr) {                                       // 006551DF..006551E1
        creature = vcall<void*>(speaker, 0x2c);                     // 006551E7..006551F4 [esp+40]
        object = vcall<void*>(speaker, 0xc);                        // 006551EE..006551FF [esp+44]
        if (creature == nullptr && object == nullptr && bark == 0) return 0; // 006551FB..00655217
    }
    // 0065528D / 00655291: [esp+28] now holds the player record
    void* const player = recovered_client_get_player_by_id_005ed590(client(), nullptr, speaker_id); // ..006552AC
    if (str_equals(&name, k_empty) && minor != 5 && minor != 0xf) { // 006552A3..006552CC
        if (creature != nullptr) {                                  // 006552D2
            RawStr full;                                            // [esp+20]
            str_assign(&name, kotor_m15b_creature_stats_get_full_name_00647ce0(rp(creature, 0x2f8), nullptr, &full)); // ..006552F3
        } else if (object == nullptr || kotor_fastcall_005ed350(client(), nullptr, rd(object, 4), &name) == 0) { // 006552FE..0065531B
            StrRes res;                                             // [esp+48] 0065531D..00655321 (state F)
            (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, k_strref_someone, res.bytes, kByte0073ecd0); // ..00655346
            str_assign(&name, res.bytes);                           // 0065534B..00655354
        }                                                           // 00655359..00655365
    }
    void* const gui = in_game_gui();                                // 0065536A..00655379 [esp+34]
    if (gui == nullptr) return 0;                                   // 0065537D
    if (rd(gui, 0x108) == 0) return 0;                              // 00655383..0065538B
    str_assign(&display, &name);                                    // 00655391..0065539A
    str_assign(&name, &name);                                       // QUIRK 0065539F..006553A8 self-assignment

    U color;                                                        // esi
    switch (minor) {                                                // 006553AD..006553C1 (index byte table 00655E2C)
    case 12:
        announce = 0;                                               // 006553C8 [esp+A8]
        [[fallthrough]];
    case 1: case 8: case 11: {                                      // 006553D3
        ExoString colon(k_colon);                                   // [esp+9C]
        RawStr sum;                                                 // [esp+110]
        (void)concat(&name, &sum, &colon);                          // 006553E4..00655400
        str_assign(&name, &sum);                                    // 00655405..00655412
        color = 1;                                                  // 0065543F
        break;
    }
    case 2: case 9: prefix_with_strref0(&name, &fetched); color = 4; break;     // 00655449..00655531
    case 3: case 10: prefix_with_strref0(&name, &fetched); color = 8; break;    // 0065553B..0065562C
    case 4: prefix_with_strref0(&name, &fetched); color = 2; break;             // 00655636..00655727
    case 6: prefix_with_strref0(&name, &fetched); color = 0x40; break;          // 00655731..00655813
    case 7: color = tint; break;                                    // QUIRK 0065581A: float bits as colour
    case 5: case 15: return 1;                                      // -> 00655D8F
    default: return 0;                                              // 13, 14 (table 8) and out of range -> 00655219
    }

    U px, py, pz;
    if (object != nullptr) {                                        // 0065581E..00655824
        px = rd(object, 0x24);                                      // 00655829 edi
        py = rd(object, 0x28);                                      // 0065582B ebx
        position[2] = fbits(rdf(object, 0x2c) + k_bark_height);     // 0065582E..0065583F fld/fadd/fstp dword
        pz = position[2];
    } else {
        py = position[1];                                           // 00655845
        px = position[0];                                           // 00655849
        pz = position[2];
    }
    void* const module = kotor_fastcall_005ed530(client(), nullptr); // 0065584D..00655856
    if (module == nullptr) return 0;                                // 0065585B..0065585D
    void* const camera = rp(module, 0x40);                          // 00655863
    if (camera == nullptr) return 0;                                // 00655866..00655868
    (void)vcall<U>(camera, 0x28, static_cast<void*>(position), px, py, pz, 0u); // 0065586E..0065588B MapToScreen

    const int sound_valid = gui_load_resref_is_valid_00406030(&sound, nullptr); // 0065588E..00655895
    if (sound_valid != 0 && object != nullptr) {                    // 0065589A..006558A8
        kotor_fastcall_0063d1d0(object, nullptr, &sound, 1);        // 006558AE..006558BA
        if (rp(object, 0xc8) != nullptr) {                          // 006558BF..006558C7
            kotor_m15_sound_source_set_3d_005d5910(rp(object, 0xc8), nullptr, 0); // 006558C9..006558CB
            kotor_m15_sound_source_set_position_005d59e0(rp(object, 0xc8), nullptr, fa(object, 0x24), 0); // ..006558DC
            U group = 9;                                            // 006558FE
            if (creature != nullptr && client_get_player_creature_005ed540(client(), nullptr) == creature) // 006558E1..006558F8
                group = 8;                                          // 006558FA
            kotor_sound_rev3::SourceSetPriorityGroup005d5900(rp(object, 0xc8), nullptr, group); // 00655900..00655906
            kotor_fastcall_005d5930(rp(object, 0xc8), nullptr);     // 0065590B..00655911 Play
            (void)vcall<U>(object, 0x128, static_cast<void*>(&sound)); // 00655916..00655922
        }
    }
    if (object != nullptr) wr(gui, 0x180, rd(object, 4));          // 00655928..00655933

    if (minor == 7) {                                               // 0065593F..00655943
        ResRef one;                                                 // [esp+68]
        (void)async_ref_construct_string_00406d60(&one, nullptr, &text); // 00655949..00655952 / 00655993..0065599C
        if (object != nullptr)                                      // 00655945..00655947
            (void)kotor_m15_sound_play_3d_one_shot_005d5e10(kotor_cell_sound_007a39ec, nullptr, &one, rd(object, 0x24),
                                                            rd(object, 0x28), rd(object, 0x2c), 0, 0xa, 0, 0, 0, 0); // ..00655989
        else
            (void)w5_sound_play_one_shot_005d5e00(kotor_cell_sound_007a39ec, nullptr, &one, 0xa, 0, 0, 0, 0); // ..006559B6
        return 1;                                                   // -> 00655D8F
    }

    if (recovered_log_conversations_00833a9c != 0) {                // 006559C0..006559C7
        // QUIRK: all six branches build the same line (name + text); only the GetPortrait probe differs.
        RawStr line;                                                // [esp+48]
        if (creature == nullptr && object != nullptr) {             // 006559DD / 00655A01 / 00655A84 / 00655AA3
            ResRef portrait;                                        // [esp+68]
            void* const r = kotor_m15e_slot00c_self_00641db0(vcall<void*>(object, 0xf0, static_cast<void*>(&portrait), 1u),
                                                             nullptr); // 00655A05..00655A18 / 00655AA7..00655ABA
            (void)rd8(r, 0);                                        // 00655A1D / 00655ABF cmp byte ptr [eax],0
        }
        (void)concat(&name, &line, &text);                          // 006559E1..00655B11 (states 22h..29h)
        feedback_append_0062b5c0(gui, nullptr, &line, color, 0);    // 00655B19..00655B1F
    }                                                               // 00655B24..00655B30

    if (announce == 0) return 1;                                    // 00655B35..00655B3E
    void* const pc = client_get_player_creature_005ed540(client(), nullptr); // 00655B44..00655B4D
    if (pc != nullptr && player != nullptr) {                       // 00655B52..00655B60
        void* const mine = recovered_client_get_player_by_id_005ed590(client(), nullptr, rd(pc, 4)); // 00655B66..00655B73
        assign_text(&name, k_empty);                                // 00655B78..00655B83
        if (mine == nullptr || rd(player, 4) != rd(mine, 4)) {      // 00655B88..00655B92
            char unused[0x14];                                      // [esp+148]
            (void)format_006fadb0(unused, k_format_u, rd(player, 4)); // QUIRK 00655B98..00655BB2 result unused
            assign_text(&name, k_open);                             // 00655BB5..00655BBE
            {
                RawStr t;                                           // [esp+48]
                (void)concat(&name, &t, &display);                  // 00655BC3..00655BD1
                str_assign(&name, &t);                              // 00655BD6..00655BE3
            }                                                       // 00655BE8..00655BF4
            ExoString close(k_close);                               // [esp+20] 00655BF9..00655C02
            RawStr t;                                               // [esp+48]
            (void)concat(&name, &t, &close);                        // 00655C07..00655C1E
            str_assign(&name, &t);                                  // 00655C23..00655C30
        }                                                           // 00655C35..00655C51
    }
    if (object == nullptr) return 1;                                // 00655C56..00655C58
    const U type = rd8(object, 8);                                  // 00655C5E
    if (type == 7 || type == 9 || type == 10) {                     // 00655C62..00655C6F
        ExoString close(k_close);                                   // [esp+20] 00655C84..00655C8D
        ExoString open(k_open);                                     // [esp+C0] 00655C92..00655CA6
        RawStr t1;                                                  // [esp+F8]
        (void)concat(&open, &t1, &display);                         // 00655CAB..00655CC2
        RawStr t2;                                                  // [esp+48]
        (void)concat(&t1, &t2, &close);                             // 00655CC7..00655CDC
        str_assign(&name, &t2);                                     // 00655CE1..00655CEE
    } else {                                                        // 00655CF3..00655D37 dtors 48, F8, C0, 20
        assign_text(&name, k_empty);                                // 00655C71..00655C7A
    }
    {
        RawStr line;                                                // [esp+A8]
        (void)concat(&name, &line, &echo);                          // 00655D3C..00655D4D
        light_manager_void_return_16_0063cfc0(object, nullptr, &line, 1, tint, locstring_extra); // QUIRK 00655D52..00655D6C
    }                                                               // 00655D71..00655D80
    wr(object, 0xc0, 1);                                            // 00655D85
    return 1;                                                       // 00655D8F..00655E03
}
