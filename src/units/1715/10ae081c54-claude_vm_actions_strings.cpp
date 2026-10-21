// Lane endar-actions, group "strings": NWScript string/number/module engine commands
// (CSWVirtualMachineCommands::ExecuteCommand*), reconstructed from the original swkotor.exe (read-only,
// capstone linear sweep).  Addresses in the comments are the original instructions.
//
// Handlers (all thiscall(int routine, int argc), RET 8 -> extern "C" __fastcall @16):
//   005417C0 PrintString            0053D610 GetStringByStrRef     00535C90 FindSubString
//   00536000 FloatToString          0053D720 GetStringLeft         00535FB0 FloatToInt
//   0053D860 GetStringLength        0053DA40 GetStringRight        00544710 StringToInt / StringToFloat
//   0053DDA0 GetSubString           0054B090 SetCustomToken        0053BEC0 GetModuleFileName / GetModuleName
//   0053BDF0 GetModule
// Missing callees reconstructed at the end: CExoDebug / CExoDebugInternal script-log forwarders,
// CSWSMessage::SendServerToPlayerSetCustomToken and the CRT atof chain (atof -> _fltin2 -> __strgtold12 /
// __ld12tod -> _ld12cvt and the 12-byte long-double helpers).
#include "061872cb05-claude_vm_actions_common.hpp"
#include <cctype>
#include <clocale>

namespace kotor_reconstruction { struct StartupString; }
namespace options_video { struct String; }
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }   // [007A39E0] CExoBase* [0068]
namespace base_ini_reconstruction {
kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*, void*,
    kotor_reconstruction::StartupString*, int);                                                 // CExoString::Right [0126]
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*, void*,
    kotor_reconstruction::StartupString*, int);                                                 // CExoString::Left [0126]
}
namespace startup_rand {
kotor_reconstruction::StartupString* __fastcall SubString005e6270(kotor_reconstruction::StartupString*, void*,
    kotor_reconstruction::StartupString*, int, int);                                            // CExoString::SubString [0147]
}
namespace client_movies {
int __fastcall StringFind_005e5eb0(options_video::String*, void*, options_video::String*, int);  // CExoString::Find(CExoString&,int) [0374]
}
namespace kotor_menu20 { int __cdecl atoi_006fafe7(const char* text); }                         // 006FAFE7 _atol [menu20_bindings]
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }                                     // 006FAE8C __ftol2 [w2_ftol_native]

extern "C" {
extern void* kotor_cell_tlk_007a3a08;                                                           // [007A3A08] CTlkTable* [0384]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                     // CExoString::Format [0125]
void* __fastcall frame_LogStringCtor_005e5a90(void* s, void*, const char* text);                // CExoString(const char*) [0459]
int __fastcall async_string_length_005e5790(void* s, void*);                                    // CExoString::GetLength [0188]
int __fastcall find_005e5f20(void* s, void*, int ch, int start);                                 // CExoString::Find(char,int) [0241]
void __fastcall frame_LogWriteString_005e6a70(void* file, void*, void* s);                       // CExoFile::Write(CExoString&) [0459]
void __fastcall cerf_c_exofile_flush_005e6a20(void* file);                                       // CExoFile::Flush [cerf_quicksave_leaves]
void* __fastcall c2da_default_ref_00405ed0(void* ref, void*);                                    // CResRef::CResRef() [0263]
U __fastcall tlk_fetch_0041e550(void* tlk, void*, U strref, void* out, U flag);                  // CTlkTable::Fetch [0385]
void __fastcall kotor_fastcall_0041db50(void* tlk, void*, I token, void* s);                     // CTlkTable::SetCustomToken [0583]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                     // CServerExoApp::GetSWSMessage [creature_destructor_r2]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                     // CServerExoApp::GetPlayerList [w1_native_b7]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);                   // CExoLinkedListInternal::GetAtPos [0168]
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);                        // CExoLinkedListInternal::GetNext [0168]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                                    // CSWSPlayer::GetGameObject [0595]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U player, U flag);          // CSWMessage::CreateWriteMessage [w2_message_native]
void __fastcall kotor_fastcall_004d6340(void* message, void*, U value, U bits);                   // CSWMessage::WriteDWORD
void __fastcall kotor_fastcall_004d6960(void* message, void*, const void* s, U bits);             // CSWMessage::WriteCExoString
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);                 // CSWMessage::GetWriteMessage
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage
void* __fastcall server_get_module_004ae6b0(void* server, void*);                                // CServerExoApp::GetModule [0530]
void* __fastcall kotor_fastcall_005e9f80(void* loc, void*, const void* src);                     // CExoLocString(const CExoLocString&) [claude_dialogue_server]
U __fastcall kotor_fastcall_004ae820(void* server, void*);                                       // CServerExoApp::GetModuleLanguage [claude_dialogue_server]
U __fastcall kotor_fastcall_005ea240(void* loc, void*, U language, void* out, U gender);         // CExoLocString::GetStringLoc [w2_message_native]
void __fastcall exo_locstring_destroy_005ea000(void* loc, void*);                                // ~CExoLocString [0265]
// reconstructed below
void __fastcall kotor_fastcall_005e9420(void* debug, void*, void* s);
void __fastcall kotor_fastcall_005e93e0(void* debug, void*);
void __fastcall kotor_fastcall_005ec5a0(void* internal, void*, void* s);
void __fastcall kotor_fastcall_005ec5b0(void* internal, void*);
U __fastcall kotor_fastcall_0056d250(void* message, void*, U player, I token, void* s);
double __cdecl kotor_cdecl_006fc5c0(const char* text);
}

namespace {
using namespace vmx;
constexpr char k_fmt_line_00740eac[] = "%s\n";                          // .rdata 00740EAC
constexpr char k_fmt_printstring_00748718[] = "PRINTSTRING: %s\n";      // .rdata 00748718
constexpr char k_fmt_width1_0074869c[] = "%%%1d.%02df\n";               // .rdata 0074869C
constexpr char k_fmt_width2_007486ac[] = "%%%2d.%02df\n";               // .rdata 007486AC
constexpr U k_tlk_fetch_flag_0073ecd0 = 0;                              // .rdata 0073ECD0 (byte 00h)
constexpr I k_custom_token_floor_0073eccc = 9;                          // .rdata 0073ECCC (dword 9)
constexpr int k_routine_string_to_int = 0xe8;                           // 232
constexpr int k_routine_module_file_name = 0xd2;                        // 210
constexpr int k_routine_module_name = 0x231;                            // 561
// [0083282C]: .bss dword read only by PrintString / PrintFloat (00541604, 00541715, 0054184B, 0054187B,
// 005418D2); the original image contains no store to it, so it keeps its zero initial value.  Kept as
// zero-initialised storage with no writer, identical to the original.
U g_script_log_suppressed_0083282c = 0;

inline void* debug_object() { return at<void*>(kotor_menu_10_19::dependencies::base_007a39e0, 8); }   // [[7A39E0]+8]
inline U pop_string(ExoString* s) { return recovered_vm_outer_pop_string_005d1080(vm(), nullptr, s); }
inline U pop_int(I* v) { return recovered_vm_outer_pop_integer_005d1000(vm(), nullptr, v); }
inline U push_string(ExoString* s) { return recovered_vm_outer_push_string_005d1090(vm(), nullptr, s); }
inline U push_int(I v) { return recovered_vm_outer_push_integer_005d1010(vm(), nullptr, v); }
inline const char* cstr(ExoString* s) { return WindowCStr005e5670(s, nullptr); }
inline kotor_reconstruction::StartupString* ss(ExoString* s) { return reinterpret_cast<kotor_reconstruction::StartupString*>(s); }
// neg/sbb/and 7D0/add F830: pushed ? 0 : -2000
inline I push_result(U pushed) { return pushed ? 0 : k_push_failed; }

// STR_RES (inline ctor at 0053D652..0053D667): +0 CExoString, +8 CResRef(16), +18 float (not initialised)
struct StrRes { ExoString text; unsigned char resref[16]; float length; };
static_assert(sizeof(StrRes) == 0x1c, "STR_RES is 0x1C bytes");

// Left/Right shape shared by 0053D720 and 0053DA40.
template<class Op> I string_cut(Op op) {
    ExoString source; exo_default(&source);                            // +10 (Left: 0053D73E, Right: 0053DA5E)
    ExoString result; exo_default(&result);                            // +08
    I count = 0;
    if (!pop_string(&source) || !pop_int(&count)) {                    // pop string, then int
        exo_destroy(&result); exo_destroy(&source);
        return k_pop_failed;
    }
    ExoString temp;                                                    // +18 (written by Left/Right)
    async_assign_string_005e5c50(&result, nullptr, op(&source, &temp, count));
    exo_destroy(&temp);
    if (!push_string(&result)) { exo_destroy(&result); exo_destroy(&source); return k_push_failed; }
    exo_destroy(&result); exo_destroy(&source);
    return 0;
}
}  // namespace

extern "C" {

// ============================================================================================ 005417C0
// PrintString(string sString, int bAddPrefix?): argc < 2 -> no second pop.  Second argument == 1 selects
// the "PRINTSTRING: " prefix.  Formats into the CExoDebug object (its leading CExoString) and writes/flushes
// the script log.  Quirk: when [0083282C] == 0 the CExoString::CStr result is fetched once and discarded
// before being fetched again (00541854 / 005418DB), kept as written.
int __fastcall recovered_vm_print_string_005417c0(void*, void*, int, unsigned argc) {
    ExoString text; exo_default(&text);                                // 005417D8..DC
    I prefix = 0;
    if (!pop_string(&text)) { exo_destroy(&text); return k_pop_failed; }   // 005417E1..FB -> 0054181C
    const char* format = k_fmt_line_00740eac;
    if (static_cast<I>(argc) >= 2) {                                   // 005417FD..02 (signed jl)
        if (!pop_int(&prefix)) { exo_destroy(&text); return k_pop_failed; } // 00541804..16
        if (prefix == 1) format = k_fmt_printstring_00748718;          // 005418C7..CC / 005418F7
    } else {
        prefix = 0;                                                    // 00541843
    }
    if (g_script_log_suppressed_0083282c == 0) (void)cstr(&text);      // 0054184B..58 / 005418D2..DF
    const char* value = cstr(&text);                                   // 0054185D..61
    frame_LogFormat_005e5680(debug_object(), format, value);           // 00541866..80
    if (g_script_log_suppressed_0083282c == 0) {                       // 0054187B..85
        kotor_fastcall_005e9420(debug_object(), nullptr, debug_object()); // 00541887..90 WriteToScriptLogFile(debug's CExoString)
        kotor_fastcall_005e93e0(debug_object(), nullptr);              // 00541895..9E FlushScriptLogFile
    }
    exo_destroy(&text);                                                // 005418A3..AF
    return 0;
}

// ============================================================================================ 00536000
// FloatToString(float f, int width = 18, int decimals = 9): width clamped to [0,18], decimals to [0,9];
// a format "%<w>.<d>f\n" is built (2-digit width field when width >= 10), then f is formatted with it.
int __fastcall recovered_vm_float_to_string_00536000(void*, void*, int, unsigned) {
    float value = 0.0f; I width = 0, decimals = 0;
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &value)) return k_pop_failed;   // 0053601E..31
    if (!pop_int(&width)) return k_pop_failed;                         // 00536033..45
    if (width >= 0x12) width = 0x12;                                   // 00536047..50 (signed)
    else if (width < 0) width = 0;                                     // 0053605A..5E
    if (!pop_int(&decimals)) return k_pop_failed;                      // 00536066..78
    if (decimals >= 0xa) decimals = 9;                                 // 00536091..9A
    else if (decimals < 0) decimals = 0;                               // 005360A4..A8
    ExoString format; exo_default(&format);                            // 005360B0..B4
    frame_LogFormat_005e5680(&format, width >= 0xa ? k_fmt_width2_007486ac : k_fmt_width1_0074869c,
                             width, decimals);                         // 005360B9..E6
    ExoString formatted; exo_default(&formatted);                      // 005360E9..ED
    const double widened = static_cast<double>(value);                 // 005360F2..FD fld dword / fstp qword
    frame_LogFormat_005e5680(&formatted, cstr(&format), widened);      // 00536100..15
    ExoString result;
    frame_LogStringCtor_005e5a90(&result, nullptr, cstr(&formatted));  // 00536118..26
    const U pushed = push_string(&result);                             // 0053612B..3B
    exo_destroy(&result);                                              // 00536146..51
    exo_destroy(&formatted);                                           // 00536156..5F
    exo_destroy(&format);                                              // 00536164..70
    return pushed ? 0 : k_push_failed;                                 // 00536175..94
}

// ============================================================================================ 0053D860
int __fastcall recovered_vm_get_string_length_0053d860(void*, void*, int, unsigned) {
    ExoString text; exo_default(&text);                                // 0053D878..7C
    if (!pop_string(&text)) { exo_destroy(&text); return k_pop_failed; }   // 0053D881..AE
    const I length = async_string_length_005e5790(&text, nullptr);     // 0053D8C4
    if (!push_int(length)) { exo_destroy(&text); return k_push_failed; }   // 0053D8C9..EA
    exo_destroy(&text);                                                // 0053D900
    return 0;
}

// ============================================================================================ 0053DA40
int __fastcall recovered_vm_get_string_right_0053da40(void*, void*, int, unsigned) {
    return string_cut([](ExoString* s, ExoString* out, I n) {          // 0053DADA..F7 Right(n) -> operator=
        return base_ini_reconstruction::right_005e6170(ss(s), nullptr, ss(out), n); });
}

// ============================================================================================ 0053D720
int __fastcall recovered_vm_get_string_left_0053d720(void*, void*, int, unsigned) {
    return string_cut([](ExoString* s, ExoString* out, I n) {          // 0053D7BA..D7 Left(n) -> operator=
        return base_ini_reconstruction::left_005e5f70(ss(s), nullptr, ss(out), n); });
}

// ============================================================================================ 0053DDA0
// GetSubString(string s, int start, int count)
int __fastcall recovered_vm_get_sub_string_0053dda0(void*, void*, int, unsigned) {
    ExoString source; exo_default(&source);                            // 0053DDBF..C3 (+18)
    ExoString result; exo_default(&result);                            // 0053DDCA..D2 (+10)
    I start = 0, count = 0;
    if (!pop_string(&source) || !pop_int(&start) || !pop_int(&count)) {   // 0053DDD7..16
        exo_destroy(&result); exo_destroy(&source);                    // 0053DE18..31
        return k_pop_failed;
    }
    ExoString temp;                                                    // +20
    async_assign_string_005e5c50(&result, nullptr,
        startup_rand::SubString005e6270(ss(&source), nullptr, ss(&temp), start, count));   // 0053DE4D..6F
    exo_destroy(&temp);                                                // 0053DE74..7D
    if (!push_string(&result)) { exo_destroy(&result); exo_destroy(&source); return k_push_failed; } // 0053DE82..B4
    exo_destroy(&result); exo_destroy(&source);                        // 0053DECB..DC
    return 0;
}

// ============================================================================================ 00535C90
// FindSubString(string s, string sub): s.Find(sub, 0)
int __fastcall recovered_vm_find_sub_string_00535c90(void*, void*, int, unsigned) {
    ExoString text; exo_default(&text);                                // 00535CA9..AD (+0C)
    ExoString sub; exo_default(&sub);                                  // 00535CB4..BC (+04)
    if (!pop_string(&text) || !pop_string(&sub)) {                     // 00535CC1..EC
        exo_destroy(&sub); exo_destroy(&text);                         // 00535CEE..07
        return k_pop_failed;
    }
    const int position = client_movies::StringFind_005e5eb0(reinterpret_cast<options_video::String*>(&text), nullptr,
        reinterpret_cast<options_video::String*>(&sub), 0);           // 00535D23..2D
    if (!push_int(position)) { exo_destroy(&sub); exo_destroy(&text); return k_push_failed; }   // 00535D32..60
    exo_destroy(&sub); exo_destroy(&text);                             // 00535D77..88
    return 0;
}

// ============================================================================================ 00535FB0
int __fastcall recovered_vm_float_to_int_00535fb0(void*, void*, int, unsigned) {
    float value = 0.0f;
    if (!recovered_vm_outer_pop_float_005d1020(vm(), nullptr, &value)) return k_pop_failed;   // 00535FB1..CB
    const U truncated = w2::ftol2_006fae8c(static_cast<double>(value)); // 00535FCE..D2 fld dword; __ftol2
    return push_result(push_int(static_cast<I>(truncated)));           // 00535FD7..EC
}

// ============================================================================================ 00544710
// routine 232 StringToInt: atol; any other routine (233 StringToFloat): atof narrowed to float (fstp dword).
int __fastcall recovered_vm_string_conversions_00544710(void*, void*, int routine, unsigned) {
    ExoString text; exo_default(&text);                                // 00544728..2C
    if (!pop_string(&text)) { exo_destroy(&text); return k_pop_failed; }   // 00544731..71
    if (routine == k_routine_string_to_int) {                          // 00544774..7C
        const int value = kotor_menu20::atoi_006fafe7(cstr(&text));    // 0054477E..84
        if (!push_int(value)) { exo_destroy(&text); return k_push_failed; } // 00544789..C0
    } else {
        const float value = static_cast<float>(kotor_cdecl_006fc5c0(cstr(&text)));   // 005447C3..CE fstp dword
        if (!recovered_vm_outer_push_float_005d1030(vm(), nullptr, value)) { exo_destroy(&text); return k_push_failed; } // 005447D1..04
    }
    exo_destroy(&text);                                                // 00544807..13
    return 0;
}

// ============================================================================================ 0053D610
// GetStringByStrRef(int strref): CTlkTable::Fetch into an inline STR_RES, push a copy of its text.
// Quirk: the STR_RES CResRef is constructed and never destroyed (trivial), the float is left uninitialised.
int __fastcall recovered_vm_get_string_by_strref_0053d610(void*, void*, int, unsigned) {
    I strref = 0;
    if (!pop_int(&strref)) return k_pop_failed;                        // 0053D616..4F
    StrRes entry;                                                      // +0C
    exo_default(&entry.text);                                          // 0053D652..56
    c2da_default_ref_00405ed0(entry.resref, nullptr);                  // 0053D65B..67
    (void)tlk_fetch_0041e550(kotor_cell_tlk_007a3a08, nullptr, static_cast<U>(strref), &entry,
                             k_tlk_fetch_flag_0073ecd0);               // 0053D66C..8D
    ExoString copy;
    WindowCopy005e5b00(&copy, nullptr, &entry.text);                   // 0053D692..9B
    if (!push_string(&copy)) { exo_destroy(&copy); exo_destroy(&entry.text); return k_push_failed; } // 0053D6A0..EB
    exo_destroy(&copy); exo_destroy(&entry.text);                      // 0053D6EE..FF
    return 0;
}

// ============================================================================================ 0054B090
// SetCustomToken(int token, string value).  Quirk: a pop failure returns -2000 (0FFFFF830h), not -2001.
// Tokens <= 9 are ignored; otherwise the TLK token is set and broadcast to every player with a game object.
int __fastcall recovered_vm_set_custom_token_0054b090(void*, void*, int, unsigned) {
    ExoString value; exo_default(&value);                              // 0054B0B1..B5 (+18)
    I token = 0;                                                       // +10
    if (!pop_int(&token) || !pop_string(&value)) {                     // 0054B0BA..E8
        exo_destroy(&value);                                           // 0054B0EA..F6
        return k_push_failed;                                          // 0054B0FB (original literal F830)
    }
    if (token > k_custom_token_floor_0073eccc) {                       // 0054B114..1E (signed jle)
        kotor_fastcall_0041db50(kotor_cell_tlk_007a3a08, nullptr, token, &value);   // 0054B124..30
        void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);  // 0054B135..3D
        void* players = recovered_server_player_list_004aed90(server_app(), nullptr);  // 0054B142..4D
        if (message && players) {                                      // 0054B152..5A
            void* internal = at<void*>(players, 0);                    // [list] (CExoLinkedListInternal*)
            void* position = at<void*>(internal, 0);                   // 0054B15C..66 head
            while (position) {
                void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, position);   // 0054B168..70
                if (player && kotor_fastcall_00561080(player, nullptr)) {   // 0054B172..7F
                    (void)kotor_fastcall_0056d250(message, nullptr, at<U>(player, 4), token, &value);   // 0054B181..91
                }
                (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &position);   // 0054B196..9D
            }                                                          // 0054B1A2..A8
        }
    }
    exo_destroy(&value);                                               // 0054B1AA..B6
    return 0;
}

// ============================================================================================ 0053BEC0
// routine 210 GetModuleFileName: module +5C name, text after the first ':' if any.
// routine 561 GetModuleName: module +64 CExoLocString in the module language.  Other routines: "".
// The module pointer is not null-checked (original).
int __fastcall recovered_vm_get_module_name_0053bec0(void*, void*, int routine, unsigned) {
    ExoString result; exo_default(&result);                            // 0053BEDF..E3 (+08)
    if (routine == k_routine_module_file_name) {                       // 0053BEE8..F8
        void* module = server_get_module_004ae6b0(server_app(), nullptr);   // 0053BF53..5B
        async_assign_string_005e5c50(&result, nullptr, static_cast<unsigned char*>(module) + 0x5c);   // 0053BF60..68
        const int colon = find_005e5f20(&result, nullptr, 0x3a, 0);    // 0053BF6D..75
        if (colon != -1) {                                             // 0053BF7A..7F
            const int keep = async_string_length_005e5790(&result, nullptr) - colon - 1;   // 0053BF81..8C
            ExoString temp;                                            // +18
            async_assign_string_005e5c50(&result, nullptr,
                base_ini_reconstruction::right_005e6170(ss(&result), nullptr, ss(&temp), keep));   // 0053BF8D..A6
            exo_destroy(&temp);                                        // 0053BFAB..B4
        }
    } else if (routine == k_routine_module_name) {                     // 0053BEFA..FF
        void* module = server_get_module_004ae6b0(server_app(), nullptr);   // 0053BF05..0D
        unsigned char loc[8];                                          // +10 CExoLocString
        kotor_fastcall_005e9f80(loc, nullptr, static_cast<unsigned char*>(module) + 0x64);   // 0053BF12..1A
        const U language = kotor_fastcall_004ae820(server_app(), nullptr);   // 0053BF1F..2D
        (void)kotor_fastcall_005ea240(loc, nullptr, language, &result, 0);   // 0053BF32..3E
        exo_locstring_destroy_005ea000(loc, nullptr);                  // 0053BF43..4C
    }
    if (!push_string(&result)) { exo_destroy(&result); return k_push_failed; }   // 0053BFB9..F2
    exo_destroy(&result);                                              // 0053BFF5
    return 0;
}

// ============================================================================================ 0053BDF0
// GetModule(): module object id (+20), OBJECT_INVALID when there is no app manager/server/module.
int __fastcall recovered_vm_get_module_0053bdf0(void*, void*, int, unsigned) {
    U id = k_invalid;                                                  // 0053BDF8
    void* app = manager();                                             // 0053BDF0..FD
    if (app) {
        void* server = at<void*>(app, 8);                              // 0053BDFF..04
        if (server) {
            void* module = server_get_module_004ae6b0(server, nullptr);   // 0053BE06..0D
            if (module) id = at<U>(module, 0x20);                      // 0053BE0F
        }
    }
    return push_result(recovered_vm_outer_push_object_005d10d0(vm(), nullptr, id));   // 0053BE12..27
}

// ============================================================================================ missing callees
// 005E9420 CExoDebug::WriteToScriptLogFile(CExoString*): RET 4.  ecx = +8 internal; null -> return.
void __fastcall kotor_fastcall_005e9420(void* debug, void*, void* s) {
    void* internal = at<void*>(debug, 8);                              // 005E9420
    if (internal) kotor_fastcall_005ec5a0(internal, nullptr, s);        // 005E9423..27 (tail jmp)
}
// 005E93E0 CExoDebug::FlushScriptLogFile(): RET.
void __fastcall kotor_fastcall_005e93e0(void* debug, void*) {
    void* internal = at<void*>(debug, 8);                              // 005E93E0
    if (internal) kotor_fastcall_005ec5b0(internal, nullptr);           // 005E93E3..E7 (tail jmp)
}
// 005EC5A0 CExoDebugInternal::WriteToScriptLogFile(CExoString*): RET 4.  [this] = log enabled, +8 CExoFile*.
void __fastcall kotor_fastcall_005ec5a0(void* internal, void*, void* s) {
    if (at<U>(internal, 0) != 0) frame_LogWriteString_005e6a70(at<void*>(internal, 8), nullptr, s);   // 005EC5A0..A8
}
// 005EC5B0 CExoDebugInternal::FlushScriptLogFile(): RET.
void __fastcall kotor_fastcall_005ec5b0(void* internal, void*) {
    if (at<U>(internal, 0) != 0) cerf_c_exofile_flush_005e6a20(at<void*>(internal, 8));   // 005EC5B0..B8
}

// 0056D250 CSWSMessage::SendServerToPlayerSetCustomToken(player, token, CExoString*): RET 0Ch.
// Message 32h/01h: DWORD token, CExoString value.  The size slot (the third argument's stack cell) is
// preset to length + 8 before GetWriteMessage overwrites it (0056D26E).
U __fastcall kotor_fastcall_0056d250(void* message, void*, U player, I token, void* s) {
    U size = static_cast<U>(async_string_length_005e5790(s, nullptr)) + 8;   // 0056D25A..67, 0056D26E
    kotor_fastcall_004d68a0(message, nullptr, size, player, 1);        // 0056D265..72
    kotor_fastcall_004d6340(message, nullptr, static_cast<U>(token), 0x20);   // 0056D277..80
    kotor_fastcall_004d6960(message, nullptr, s, 0x20);                // 0056D285..8A
    void* data = nullptr;                                              // +0C
    if (!kotor_fastcall_004d6440(message, nullptr, &data, &size)) return 0;   // 0056D28F..A8
    return kotor_fastcall_00567e60(message, nullptr, player, 0x32, 1, data, size);   // 0056D2AB..C5
}

}  // extern "C"

// ============================================================================================ CRT atof chain
// The original statically links the VC CRT: atof 006FC5C0 -> _fltin2 00705343 -> __strgtold12 0070A54E and
// __ld12tod 00708F18 -> _ld12cvt 00708DC0 (DoubleFormat .data 007A34D0 = {1024,-1023,53,11,64,1023}).
// 12-byte long double (_LDBL12): +0 word XT, +2 dword MANLO, +6 dword MANHI, +A word sign|exponent; the
// helpers also treat it as three little-endian dwords.  All integer arithmetic, transcribed per function.
// Locale: isspace/isdigit (006FCA55 / 006FC9DC) and the decimal point [007A32A4] read the CRT's current
// locale; the rebuilt process's CRT locale state is read the same way (std::isspace / std::isdigit /
// localeconv), which is the "C" locale unless the game's own setlocale call (005F932D, Polish TLK) ran.
namespace {
struct Ld12 { unsigned char b[12]; };
inline U d32(const void* p, U off) { U v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 4); return v; }
inline void s32(void* p, U off, U v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 4); }
inline std::uint16_t d16(const void* p, U off) { std::uint16_t v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, 2); return v; }
inline void s16(void* p, U off, std::uint16_t v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, 2); }

// .data 007A36E0 _pow10pos / 007A3840 _pow10neg: 29 _LDBL12 entries each (10^1..10^7, 10^8..10^56 step 8,
// 10^64..10^448 step 64, 10^512..10^3584 step 512, 10^4096), words in memory order.
const std::uint16_t k_pow10pos_007a36e0[29][6] = {
{0x0000,0x0000,0x0000,0x0000,0xa000,0x4002},{0x0000,0x0000,0x0000,0x0000,0xc800,0x4005},
{0x0000,0x0000,0x0000,0x0000,0xfa00,0x4008},{0x0000,0x0000,0x0000,0x0000,0x9c40,0x400c},
{0x0000,0x0000,0x0000,0x0000,0xc350,0x400f},{0x0000,0x0000,0x0000,0x0000,0xf424,0x4012},
{0x0000,0x0000,0x0000,0x8000,0x9896,0x4016},{0x0000,0x0000,0x0000,0x2000,0xbebc,0x4019},
{0x0000,0x0000,0x0400,0xc9bf,0x8e1b,0x4034},{0x0000,0xa100,0xcced,0x1bce,0xd3c2,0x404e},
{0xf020,0xb59e,0x2b70,0xada8,0x9dc5,0x4069},{0x5dd0,0x25fd,0x1ae5,0x4f8e,0xeb19,0x4083},
{0x9671,0x95d7,0x0e43,0x8d05,0xaf29,0x409e},{0xbff9,0x44a0,0x81ed,0x8f12,0x8281,0x40b9},
{0x3cbf,0xa6d5,0xffcf,0x1f49,0xc278,0x40d3},{0xc66f,0x8ce0,0x80e9,0x47c9,0x93ba,0x41a8},
{0x85bc,0x556b,0x3927,0xf78d,0xe070,0x427c},{0xddbc,0xde8e,0x9df9,0xebfb,0xaa7e,0x4351},
{0xe6a1,0xe376,0xf2cc,0x2f29,0x8184,0x4426},{0x1028,0xaa17,0xaef8,0xe310,0xc4c5,0x44fa},
{0xa7eb,0xf3d4,0xebf7,0x4ae1,0x957a,0x45cf},{0xcc65,0x91c7,0xa60e,0xa0ae,0xe319,0x46a3},
{0x650d,0x0c17,0x8175,0x7586,0xc976,0x4d48},{0x4258,0xa7e4,0x3993,0x353b,0xb2b8,0x53ed},
{0xa74d,0x5de5,0xc53d,0x3b5d,0x9e8b,0x5a92},{0x5dff,0xf0a6,0x20a1,0x54c0,0x8ca5,0x6137},
{0xfdd1,0x5a8b,0xd88b,0x5d25,0xf989,0x67db},{0x95aa,0xf3f8,0xbf27,0xc8a2,0xdd5d,0x6e80},
{0xc94c,0x979b,0x8a20,0x5202,0xc460,0x7525}};
const std::uint16_t k_pow10neg_007a3840[29][6] = {
{0xcccd,0xcccd,0xcccc,0xcccc,0xcccc,0x3ffb},{0x3d71,0xd70a,0x70a3,0x0a3d,0xa3d7,0x3ff8},
{0x645a,0xdf3b,0x8d4f,0x6e97,0x8312,0x3ff5},{0xd3c3,0x652c,0xe219,0x1758,0xd1b7,0x3ff1},
{0x0fd0,0x8423,0x1b47,0xac47,0xa7c5,0x3fee},{0xa640,0x69b6,0xaf6c,0xbd05,0x8637,0x3feb},
{0x3d33,0x42bc,0xe57a,0x94d5,0xd6bf,0x3fe7},{0xfdc2,0xcefd,0x8461,0x7711,0xabcc,0x3fe4},
{0x4c2f,0xe15b,0xc44d,0x94be,0xe695,0x3fc9},{0xc492,0x3b53,0x4475,0x14cd,0x9abe,0x3faf},
{0x67de,0x94ba,0x4539,0x1ead,0xcfb1,0x3f94},{0x2324,0xe2c6,0xbabc,0x313b,0x8b61,0x3f7a},
{0x5561,0xc159,0xb17e,0x7c53,0xbb12,0x3f5f},{0xeed7,0x8d2f,0xbe06,0x8592,0xfb15,0x3f44},
{0x3f24,0xe9a5,0xa539,0xea27,0xa87f,0x3f2a},{0xac7d,0xe4a1,0x64bc,0x467c,0xddd0,0x3e55},
{0x7b63,0xcc06,0x5423,0x8377,0x91ff,0x3d81},{0xfa91,0x193a,0x637a,0x4325,0xc031,0x3cac},
{0x8921,0x38d1,0x4782,0xb897,0xfd00,0x3bd7},{0x88dc,0x0858,0xb11b,0xe3e8,0xa686,0x3b03},
{0x84c6,0x4245,0xb607,0x7599,0xdb37,0x3a2e},{0x7133,0xd21c,0xdb23,0xee32,0x9049,0x395a},
{0x87a6,0xc0be,0xda57,0x82a5,0xa2a6,0x32b5},{0x68e2,0x11b2,0x52a7,0x449f,0xb759,0x2c10},
{0x4925,0x2de4,0x3436,0x534f,0xceae,0x256b},{0x598f,0xa404,0xdec0,0x7dc2,0xe8fb,0x1ec6},
{0xe79e,0x5a88,0x9157,0xbf3c,0x8350,0x1822},{0x4b4e,0x6265,0x83fd,0xaf8f,0x9406,0x117d},
{0x2de4,0x9fde,0xd2ce,0x04c8,0xa6dd,0x0ad8}};
// .data 007A34D0 DoubleFormat
struct FpFormat { I max_exp, min_exp, precision, exp_width, format_width, bias; };
const FpFormat k_double_format_007a34d0 = {1024, -1023, 53, 11, 64, 1023};
}  // namespace

extern "C" {
// 006FCA55 isspace / 006FC9DC isdigit (cdecl int(int)): current CRT locale classification (see above)
int __cdecl kotor_cdecl_006fca55(int c) { return std::isspace(c) ? 8 : 0; }       // _SPACE = 8
int __cdecl kotor_cdecl_006fc9dc(int c) { return std::isdigit(c) ? 4 : 0; }       // _DIGIT = 4

// 0070AED2 ___addl(a, b, *out): *out = a + b; returns the carry (sum < a || sum < b)
int __cdecl kotor_cdecl_0070aed2(U a, U b, U* out) {
    const U sum = a + b;                                               // 0070AEDB
    const int carry = (sum < a || sum < b) ? 1 : 0;                    // 0070AEE0..EA
    *out = sum;                                                        // 0070AEEB..EF
    return carry;
}
// 0070AEF3 ___add_12(x, y): 96-bit add, final carry dropped
void __cdecl kotor_cdecl_0070aef3(void* x, const void* y) {
    U t;
    if (kotor_cdecl_0070aed2(d32(x, 0), d32(y, 0), &t)) {             // 0070AEFD..0C
        s32(x, 0, t);
        U t1;
        if (kotor_cdecl_0070aed2(d32(x, 4), 1, &t1)) { s32(x, 4, t1); s32(x, 8, d32(x, 8) + 1); }   // 0070AF0E..22
        else s32(x, 4, t1);
    } else s32(x, 0, t);
    U t2;
    const int c2 = kotor_cdecl_0070aed2(d32(x, 4), d32(y, 4), &t2);   // 0070AF25..33
    s32(x, 4, t2);
    if (c2) s32(x, 8, d32(x, 8) + 1);                                  // 0070AF3A
    U t3;
    (void)kotor_cdecl_0070aed2(d32(x, 8), d32(y, 8), &t3);             // 0070AF3D..46
    s32(x, 8, t3);
}
// 0070AF51 ___shl_12 / 0070AF7F ___shr_12: 96-bit shift by one
void __cdecl kotor_cdecl_0070af51(void* x) {
    const U a = d32(x, 0), b = d32(x, 4), c = d32(x, 8);
    s32(x, 0, a << 1); s32(x, 4, (b << 1) | (a >> 31)); s32(x, 8, (c << 1) | (b >> 31));
}
void __cdecl kotor_cdecl_0070af7f(void* x) {
    const U a = d32(x, 0), b = d32(x, 4), c = d32(x, 8);
    s32(x, 4, (b >> 1) | (c << 31)); s32(x, 0, (a >> 1) | (b << 31)); s32(x, 8, c >> 1);
}

// 0070AFAC __mtold12(digits, count, out): decimal digit string (signed bytes) -> normalized _LDBL12
void __cdecl kotor_cdecl_0070afac(const signed char* digits, U count, void* out) {
    std::uint16_t exponent = 0x404e;                                   // 0070AFCA
    s32(out, 0, 0); s32(out, 4, 0); s32(out, 8, 0);                    // 0070AFD1..D6
    for (; count > 0; --count, ++digits) {                             // 0070AFD9..26 (unsigned)
        Ld12 saved; std::memcpy(saved.b, out, 12);                     // 0070AFDE..E6
        kotor_cdecl_0070af51(out); kotor_cdecl_0070af51(out);          // x4
        kotor_cdecl_0070aef3(out, saved.b);                            // x5
        kotor_cdecl_0070af51(out);                                     // x10
        Ld12 digit; s32(digit.b, 0, static_cast<U>(static_cast<I>(*digits))); s32(digit.b, 4, 0); s32(digit.b, 8, 0);   // 0070B002..10 movsx
        kotor_cdecl_0070aef3(out, digit.b);                            // 0070B013..18
    }
    while (d32(out, 8) == 0) {                                         // 0070B02A..59 shift by 16
        const U mid = d32(out, 4), low = d32(out, 0);
        exponent = static_cast<std::uint16_t>(exponent + 0xfff0);
        s32(out, 8, mid >> 16);
        s32(out, 4, (low >> 16) | (mid << 16));
        s32(out, 0, low << 16);
    }
    while ((d32(out, 8) & 0x8000u) == 0) {                             // 0070B05E..76 (bit 15 of the high dword)
        kotor_cdecl_0070af51(out);
        exponent = static_cast<std::uint16_t>(exponent + 0xffff);
    }
    s16(out, 0xa, exponent);                                           // 0070B07B..84
}

// 0070B3DD __ld12mul(px, py): px *= py (12-byte long doubles), round, overflow -> infinity
void __cdecl kotor_cdecl_0070b3dd(void* px, const void* py) {
    Ld12 product; s32(product.b, 0, 0); s32(product.b, 4, 0); s32(product.b, 8, 0);   // 0070B401..07 (-28..-20)
    U sticky = 0;                                                      // -18
    std::uint16_t ey = d16(py, 0xa), ex = d16(px, 0xa);
    const U sign = (static_cast<U>(ey) ^ static_cast<U>(ex)) & 0x8000u;    // 0070B40F..1C
    ey = static_cast<std::uint16_t>(ey & 0x7fff); ex = static_cast<std::uint16_t>(ex & 0x7fff);
    U sum = static_cast<U>(ey) + static_cast<U>(ex);                   // 0070B426 (dword; word compares below)
    auto put_inf = [&] {                                               // 0070B5EA..02
        s32(px, 4, 0); s32(px, 0, 0); s32(px, 8, (sign ? 0x80000000u : 0u) + 0x7fff8000u);
    };
    if (ex >= 0x7fff || ey >= 0x7fff || static_cast<std::uint16_t>(sum) > 0xbffd) { put_inf(); return; }   // 0070B422..42
    if (static_cast<std::uint16_t>(sum) <= 0x3fbf) { s32(px, 8, 0); s32(px, 4, 0); s32(px, 0, 0); return; } // 0070B448..95
    if (ex == 0) {                                                     // 0070B453..70
        ++sum;
        if ((d32(px, 8) & 0x7fffffffu) == 0 && d32(px, 4) == 0 && d32(px, 0) == 0) { s16(px, 0xa, 0); return; }
    }
    if (ey == 0) {                                                     // 0070B477..95
        ++sum;
        if ((d32(py, 8) & 0x7fffffffu) == 0 && d32(py, 4) == 0 && d32(py, 0) == 0) {
            s32(px, 8, 0); s32(px, 4, 0); s32(px, 0, 0); return;
        }
    }
    for (U i = 0; i < 5; ++i) {                                        // 0070B49A..0C
        U poffs = i * 2, qoffs = 8;
        for (U j = 5 - i; j > 0; --j) {
            const U prod = static_cast<U>(d16(px, poffs)) * static_cast<U>(d16(py, qoffs));   // 0070B4C6..D2
            U r;
            if (kotor_cdecl_0070aed2(d32(product.b, i * 2), prod, &r)) {
                s32(product.b, i * 2, r);
                s16(product.b, i * 2 + 4, static_cast<std::uint16_t>(d16(product.b, i * 2 + 4) + 1));   // 0070B4EB..EE
            } else s32(product.b, i * 2, r);
            poffs += 2; qoffs -= 2;
        }
    }
    sum += 0xc002;                                                     // 0070B50E (word view = sum - 3FFE)
    if (static_cast<std::int16_t>(sum) > 0) {                          // 0070B515..38
        while ((product.b[11] & 0x80) == 0) {
            kotor_cdecl_0070af51(product.b);
            sum += 0xffff;
            if (static_cast<std::int16_t>(sum) <= 0) break;
        }
    }
    if (static_cast<std::int16_t>(sum) <= 0) {                         // 0070B53A..76
        sum += 0xffff;
        if (static_cast<std::int16_t>(sum) < 0) {
            U count = static_cast<std::uint16_t>(-static_cast<I>(sum)); // 0070B54F..54 movzx of the negated word
            sum += count;
            do {
                if (product.b[0] & 1) ++sticky;                        // 0070B55A..60
                kotor_cdecl_0070af7f(product.b);
            } while (--count);
            if (sticky) product.b[0] |= 1;                             // 0070B570..76
        }
    }
    if (d16(product.b, 0) > 0x8000 || (d32(product.b, 0) & 0x1ffffu) == 0x18000u) {   // 0070B57A..8F
        if (d32(product.b, 2) == 0xffffffffu) {                        // 0070B591..C3
            s32(product.b, 2, 0);
            if (d32(product.b, 6) == 0xffffffffu) {
                s32(product.b, 6, 0);
                if (d16(product.b, 0xa) == 0xffff) { ++sum; s16(product.b, 0xa, 0x8000); }
                else s16(product.b, 0xa, static_cast<std::uint16_t>(d16(product.b, 0xa) + 1));
            } else s32(product.b, 6, d32(product.b, 6) + 1);
        } else s32(product.b, 2, d32(product.b, 2) + 1);
    }
    if (static_cast<std::uint16_t>(sum) >= 0x7fff) { put_inf(); return; }   // 0070B5C6..CD
    s16(px, 0, d16(product.b, 2));                                     // 0070B5CF..D3
    s32(px, 2, d32(product.b, 4));                                     // 0070B5D6..D9
    s32(px, 6, d32(product.b, 8));                                     // 0070B5DC..DF
    s16(px, 0xa, static_cast<std::uint16_t>(sum | sign));              // 0070B5E2..E4
}

// 0070B615 __multtenpow12(pld12, pow, mult12)
void __cdecl kotor_cdecl_0070b615(void* value, I power, U mult12) {
    if (power == 0) return;                                            // 0070B62E..34
    const std::uint16_t (*table)[6] = k_pow10pos_007a36e0;
    if (power < 0) { power = -power; table = k_pow10neg_007a3840; }    // 0070B636..40
    if (mult12 == 0) s16(value, 0, 0);                                 // 0070B643..4B
    I group = -1;                                                      // ebx = table - 60h, += 54h per group
    while (power != 0) {                                               // 0070B64E..8F
        const I last3 = power & 7;
        power >>= 3;                                                   // sar
        ++group;
        if (last3 == 0) continue;
        const std::uint16_t* entry = table[group * 7 + last3 - 1];
        Ld12 operand; std::memcpy(operand.b, entry, 12);
        if (d16(operand.b, 0) >= 0x8000) s32(operand.b, 2, d32(operand.b, 2) - 1);   // 0070B66C..79 unround copy
        kotor_cdecl_0070b3dd(value, operand.b);                        // 0070B67F..83
    }
}

// 0070A54E __strgtold12(pld12, &end, str, mult12, scale, decpt, implicit_E): returns SLD_* flags
// (1 underflow, 2 overflow, 4 no digits).  State machine per the jump table at 0070A958.
U __cdecl kotor_cdecl_0070a54e(void* out, const char** end, const char* text, U mult12, I scale, I decpt, I implicit_e) {
    enum { S_INIT, S_EAT0L, S_SIGNM, S_GETL, S_GETR, S_POINT, S_E, S_SIGNE, S_EAT0E, S_GETE, S_END, S_E_IMPLICIT };
    signed char buffer[0x1c];                                          // -58..-3D
    signed char* digits = buffer;                                      // -08
    U man_sign = 0;                                                    // -2C
    I exp_sign = 1;                                                    // -1C
    U count = 0;                                                       // -0C manlen
    I found_digit = 0, found_point = 0, found_exponent = 0;           // -14, -28, -24
    I power = 0;                                                       // -20
    I exp_adjust = 0;                                                  // -10
    U flags = 0;                                                       // -18
    const char decimal_point = *std::localeconv()->decimal_point;     // [007A32A4] __decimal_point
    const unsigned char* p = reinterpret_cast<const unsigned char*>(text);
    const unsigned char* saved = p;                                    // 0070A58C [ebp+10] = str (before the blank skip)
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') ++p;  // 0070A58F..A6
    auto nz = [](unsigned c) { return c >= '1' && c <= '9'; };
    auto dig = [](unsigned c) { return kotor_cdecl_006fc9dc(static_cast<int>(c)) != 0; };
    auto exp_char = [](unsigned c) { return c == 'D' || c == 'E' || c == 'd' || c == 'e'; };
    int state = S_INIT;
    unsigned c;
    while (state != S_END) {
        c = *p++;                                                      // 0070A5A8..AA
        switch (state) {
        case S_INIT:                                                   // 0070A5BB
            if (nz(c)) { state = S_GETL; --p; }
            else if (static_cast<char>(c) == decimal_point) state = S_POINT;
            else if (c == '+') { state = S_SIGNM; man_sign = 0; }
            else if (c == '-') { state = S_SIGNM; man_sign = 0x8000; }
            else if (c == '0') state = S_EAT0L;
            else { state = S_END; --p; }
            break;
        case S_EAT0L:                                                  // 0070A60E
            found_digit = 1;
            if (nz(c)) { state = S_GETL; --p; }
            else if (static_cast<char>(c) == decimal_point) state = S_GETR;
            else if (c == '+' || c == '-') { --p; state = S_E_IMPLICIT; }
            else if (c == '0') state = S_EAT0L;
            else if (exp_char(c)) state = S_E;
            else { state = S_END; --p; }
            break;
        case S_SIGNM:                                                  // 0070A66F
            if (nz(c)) { state = S_GETL; --p; }
            else if (static_cast<char>(c) == decimal_point) state = S_POINT;
            else if (c == '0') state = S_EAT0L;
            else { state = S_END; p = saved; }
            break;
        case S_GETL:                                                   // 0070A69A
            found_digit = 1;
            for (; dig(c); c = *p++) {
                if (count < 0x19) { ++count; *digits++ = static_cast<signed char>(c - '0'); }
                else ++exp_adjust;
            }
            if (static_cast<char>(c) == decimal_point) state = S_GETR;
            else if (c == '+' || c == '-') { --p; state = S_E_IMPLICIT; }
            else if (exp_char(c)) state = S_E;
            else { state = S_END; --p; }
            break;
        case S_GETR:                                                   // 0070A6D5
            found_digit = 1; found_point = 1;
            if (count == 0) for (; c == '0'; c = *p++) --exp_adjust;
            for (; dig(c); c = *p++) {
                if (count < 0x19) { ++count; *digits++ = static_cast<signed char>(c - '0'); --exp_adjust; }
            }
            if (c == '+' || c == '-') { --p; state = S_E_IMPLICIT; }
            else if (exp_char(c)) state = S_E;
            else { state = S_END; --p; }
            break;
        case S_POINT:                                                  // 0070A72D
            found_point = 1;
            if (dig(c)) { state = S_GETR; --p; }
            else { state = S_END; p = saved; }
            break;
        case S_E:                                                      // 0070A74D
            saved = p - 2;
            if (nz(c)) { state = S_GETE; --p; }
            else if (c == '+') state = S_SIGNE;
            else if (c == '-') { state = S_SIGNE; exp_sign = -1; }
            else if (c == '0') state = S_EAT0E;
            else { state = S_END; p = saved; }
            break;
        case S_EAT0E:                                                  // 0070A785
            found_exponent = 1;
            for (; c == '0'; c = *p++) {}
            if (nz(c)) { state = S_GETE; --p; }
            else { state = S_END; --p; }
            break;
        case S_SIGNE:                                                  // 0070A7DA
            if (nz(c)) { state = S_GETE; --p; }
            else if (c == '0') state = S_EAT0E;
            else { state = S_END; p = saved; }
            break;
        case S_GETE: {                                                 // 0070A83F
            found_exponent = 1;
            I value = 0;
            for (; dig(c); c = *p++) {
                value = static_cast<I>(static_cast<signed char>(c)) + value * 10 - 0x30;
                if (value > 0x1450) { value = 0x1451; break; }
            }
            power = value;
            for (; dig(c); c = *p++) {}
            state = S_END; --p;
            break;
        }
        case S_E_IMPLICIT:                                             // 0070A7F7
            if (implicit_e) {
                saved = p - 1;
                if (c == '+') state = S_SIGNE;
                else if (c == '-') { state = S_SIGNE; exp_sign = -1; }
                else { state = S_END; p = saved; }
            } else { state = S_END; --p; }
            break;
        default: break;
        }
    }
    *end = reinterpret_cast<const char*>(p);                           // 0070A7A8..AB
    U u = 0, low = 0, high = 0, exponent = 0;
    if (!found_digit) {                                                // 0070A7A4..AD -> 0070A903
        flags = 4;
    } else {
        if (count > 0x18) {                                            // 0070A7B3..C8
            if (buffer[0x17] >= 5) ++buffer[0x17];
            --digits; ++exp_adjust; count = 0x18;
        }
        if (count != 0) {                                              // 0070A7CB..CF
            for (--digits; *digits == 0; --digits) { --count; ++exp_adjust; }   // 0070A899..A3
            Ld12 value;
            kotor_cdecl_0070afac(buffer, count, value.b);              // 0070A8A5..B0
            I p10 = power;
            if (exp_sign < 0) p10 = -p10;                              // 0070A8BD..C2
            p10 += exp_adjust;
            if (!found_exponent) p10 += scale;                         // 0070A8C7..CC
            if (!found_point) p10 -= decpt;                            // 0070A8CF..D4
            if (p10 > 0x1450) {                                        // 0070A8D7 -> 0070A90C overflow
                u = 0; exponent = 0x7fff; high = 0x80000000u; low = 0; flags = 2;
            } else if (p10 < -0x1450) {                                // 0070A8DE -> 0070A923 underflow
                flags = 1;
            } else {
                kotor_cdecl_0070b615(value.b, p10, mult12);            // 0070A8E5..ED
                u = d16(value.b, 0); low = d32(value.b, 2); high = d32(value.b, 6); exponent = d32(value.b, 0xa);   // 0070A8F2..FB
            }
        }
    }
    s32(out, 2, low);                                                  // 0070A938
    s32(out, 6, high);                                                 // 0070A93B
    s16(out, 0xa, static_cast<std::uint16_t>(exponent | man_sign));   // 0070A935..3E
    s16(out, 0, static_cast<std::uint16_t>(u));                        // 0070A945
    return flags;
}

// _ld12cvt mantissa helpers (u_long man[3], man[0] most significant)
// 00708D11 _CopyMan(dst, src)
void __cdecl kotor_cdecl_00708d11(U* dst, const U* src) { for (int i = 0; i < 3; ++i) dst[i] = src[i]; }
// 00708D2C _IsZeroMan(man): 1 when all three dwords are zero
int __cdecl kotor_cdecl_00708d2c(const U* man) { for (int i = 0; i < 3; ++i) if (man[i] != 0) return 0; return 1; }
// 00708C20 _ZeroTail(man, nbit): 1 when every bit after position nbit is zero
int __cdecl kotor_cdecl_00708c20(const U* man, I nbit) {
    I i = nbit / 32;                                                   // cdq/idiv (signed)
    const U mask = ~(0xffffffffu << (31 - nbit % 32));                // 00708C2A..38
    if (man[i] & mask) return 0;                                       // 00708C3A..41
    for (++i; i < 3; ++i) if (man[i] != 0) return 0;                   // 00708C42..4C
    return 1;
}
// 00708C52 _IncMan(man, nbit): add one at position nbit, propagate the carry; returns the final carry
int __cdecl kotor_cdecl_00708c52(U* man, I nbit) {
    I i = nbit / 32;
    const U bit = 1u << (31 - nbit % 32);
    int carry = kotor_cdecl_0070aed2(man[i], bit, &man[i]);            // 00708C64..75
    for (--i; i >= 0 && carry; --i) carry = kotor_cdecl_0070aed2(man[i], 1, &man[i]);   // 00708C7D..9A
    return carry;
}
// 00708C9F _RoundMan(man, precision): round at bit `precision`; an exact half (zero tail) is not rounded up
int __cdecl kotor_cdecl_00708c9f(U* man, I precision) {
    int carry = 0;
    const I nbit = precision - 1;
    const I i = (nbit + 1) / 32;
    const I shift = 31 - (nbit + 1) % 32;
    if (man[i] & (1u << shift)) {                                      // 00708CC5..CB
        if (!kotor_cdecl_00708c20(man, nbit + 1)) carry = kotor_cdecl_00708c52(man, nbit);   // 00708CCD..E8
    }
    man[i] &= 0xffffffffu << shift;                                    // 00708CEB..F8
    for (I k = i + 1; k < 3; ++k) man[k] = 0;                          // 00708CFB..07
    return carry;
}
// 00708D45 _ShrMan(man, n): shift the 96-bit mantissa right by n bits
void __cdecl kotor_cdecl_00708d45(U* man, I n) {
    const I words = n / 32, bits = n % 32;                             // 00708D54..57
    const U mask = ~(0xffffffffu << bits);                            // 00708D5C..6E
    U carry = 0;
    for (int k = 0; k < 3; ++k) {                                      // 00708D73..96
        const U low = man[k] & mask;
        man[k] = (man[k] >> bits) | carry;
        carry = low << ((32 - bits) & 31);                             // x86 shl masks the count
    }
    for (I k = 2; k >= 0; --k) man[k] = k >= words ? man[k - words] : 0;   // 00708D98..B9
}
// 00708DC0 _ld12cvt(pld12, d, format): returns 0 ok, 1 overflow, 2 underflow
int __cdecl kotor_cdecl_00708dc0(const void* value, void* out, const FpFormat* format) {
    const U top = d16(value, 0xa);
    const U sign = top & 0x8000u;                                      // 00708DD5..DB
    U man[3] = { d32(value, 6), d32(value, 2), static_cast<U>(d16(value, 0)) << 16 };   // 00708DDE..05
    I exponent = static_cast<I>(top & 0x7fff) - 0x3fff;               // 00708DED..F3
    U biased = 0; int status;
    if (exponent == -0x3fff) {                                         // 00708DF9..09
        biased = 0;
        if (kotor_cdecl_00708d2c(man)) status = 0;
        else { man[0] = man[1] = man[2] = 0; status = 2; }
    } else {
        U saved[3]; kotor_cdecl_00708d11(saved, man);                  // 00708E29..2D
        if (kotor_cdecl_00708c9f(man, format->precision)) ++exponent;  // 00708E32..45
        if (exponent < format->min_exp - format->precision) {          // 00708E46..50
            man[0] = man[1] = man[2] = 0; biased = 0; status = 2;
        } else if (exponent <= format->min_exp) {                      // 00708E5C..98 denormal
            const I shift = format->min_exp - exponent;
            kotor_cdecl_00708d11(man, saved);
            kotor_cdecl_00708d45(man, shift);
            (void)kotor_cdecl_00708c9f(man, format->precision);
            kotor_cdecl_00708d45(man, format->exp_width + 1);
            biased = 0; status = 2;
        } else if (exponent >= format->max_exp) {                      // 00708E9C..C2 overflow -> infinity
            man[0] = man[1] = man[2] = 0;
            man[0] |= 0x80000000u;
            kotor_cdecl_00708d45(man, format->exp_width);
            biased = static_cast<U>(format->bias + format->max_exp); status = 1;
        } else {                                                       // 00708EC4..D7
            biased = static_cast<U>(format->bias + exponent);
            man[0] &= 0x7fffffffu;
            kotor_cdecl_00708d45(man, format->exp_width);
            status = 0;
        }
    }
    const U msw = (biased << (31 - format->exp_width)) | (sign ? 0x80000000u : 0u) | man[0];   // 00708EDA..F4
    if (format->format_width == 0x40) { s32(out, 4, msw); s32(out, 0, man[1]); }               // 00708EF7..05
    else if (format->format_width == 0x20) s32(out, 0, msw);                                   // 00708F09..11
    return status;
}
// 00708F18 __ld12tod(pld12, double*)
int __cdecl kotor_cdecl_00708f18(const void* value, void* out) { return kotor_cdecl_00708dc0(value, out, &k_double_format_007a34d0); }

// 00705343 _fltin2(flt, str, len, scale, radix): FLT {+0 flags, +4 nbytes, +10 dval}
void* __cdecl kotor_cdecl_00705343(void* flt, const char* text, I, I, I) {
    Ld12 value; const char* end = nullptr;
    const U flags = kotor_cdecl_0070a54e(value.b, &end, text, 0, 0, 0, 0);   // 00705354..6F
    U result = 0; unsigned char dval[8];
    if (flags & 4) {                                                   // 00705374..84
        result = 0x200; std::memset(dval, 0, 8);
    } else {
        const int status = kotor_cdecl_00708f18(value.b, dval);        // 00705386..93
        if ((flags & 2) || status == 1) result = 0x80;                 // 00705393..9F
        if ((flags & 1) || status == 2) result |= 0x100;               // 007053A4..AE
    }
    s32(flt, 0, result);                                               // 007053BD
    s32(flt, 4, static_cast<U>(end - text));                           // 007053B7..BF
    std::memcpy(static_cast<unsigned char*>(flt) + 0x10, dval, 8);     // 007053C2..CB
    return flt;
}
// 006FC5C0 atof(str): skip isspace, _fltin2(..., strlen(str), 0, 0)->dval
double __cdecl kotor_cdecl_006fc5c0(const char* text) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(text);
    while (kotor_cdecl_006fca55(*p)) ++p;                              // 006FC5CA..D9
    unsigned char flt[0x18];                                           // -18
    const char* start = reinterpret_cast<const char*>(p);
    kotor_cdecl_00705343(flt, start, static_cast<I>(std::strlen(start)), 0, 0);   // 006FC5DB..EA
    double result; std::memcpy(&result, flt + 0x10, 8);                // 006FC5EF fld qword [eax+10]
    return result;
}
}  // extern "C"
