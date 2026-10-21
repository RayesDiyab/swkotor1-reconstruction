// claude_quit.cpp (lane worker quit): CServerExoAppInternal::StopServices 004B7C60 and its missing callee cascade,
// transcribed from the original swkotor.exe (read-only). StopServices is reached through CServerExoApp::StopServices
// 004AE900 (0399) from CAppManager::DestroyServer 00401280 (ShutDownToMainMenu 005FCBD0 = Quit to main menu incl. the
// death screen; CSWGuiSaveLoad::LoadGame 006CB0E0 = in-game load of a save / PCAUTOSAVE; WinMain exit 00404A21) and
// from ~CAppManager 004012C0 (WinMain exit 00404A75).
//   004B7C60 CServerExoAppInternal::StopServices()                       RET 0        quit_server_stop_services_004b7c60
//   004B1200 CServerAIMaster::~CServerAIMaster()                         RET 0        kotor_fastcall_004b1200
//   005D0FB0 CVirtualMachine::~CVirtualMachine()                         RET 0        kotor_fastcall_005d0fb0
//   005D1120 CScriptCompiler::~CScriptCompiler()                         RET 0        kotor_fastcall_005d1120
//   005D14D0 CScriptCompilerInternal::~CScriptCompilerInternal()         RET 0        kotor_fastcall_005d14d0
//   005D13C0 CScriptCompilerInternal::ShutDown()                         RET 0        kotor_fastcall_005d13c0
//   005D1370 ~CScriptCompilerIncludeFileStackEntry()                     RET 0        kotor_fastcall_005d1370
//   005D11E0 CResHelper<CResNSS,2009>::~CResHelper()                     RET 0        kotor_fastcall_005d11e0
//   005D1240 CResHelper<CResNSS,2009> scalar deleting destructor         RET 4        kotor_fastcall_005d1240
//   005D1260 ~CScriptCompilerIdListEntry()                               RET 0        kotor_fastcall_005d1260
//   005D1140 ~CScriptCompilerVarStackEntry()                             RET 0        kotor_fastcall_005d1140
//   005D1190 ~CScriptCompilerStructureFieldEntry()                       RET 0        kotor_fastcall_005d1190
//   005D5020 CNetLayer::EndServerMode()                                  RET 0, EAX   kotor_fastcall_005d5020
// StopServices is exported under a lane-unique name; the rebuilt 0784 object (srcquit\0784) forwards the canonical
// @kotor_fastcall_004b7c60@8 to it.
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, stack args...); game heap only (live_free).
// EH: the original unwind states only destroy the CExoString locals / members already being destroyed; no bound callee
// throws, so they are not reproduced.
// Quirks kept:
//  - StopServices formats a debug timestamp into a local CExoString and discards its CStr() (004B7C83..004B7CA9).
//  - [007A3A04] (CScriptCompiler*) is never written by the original outside this function (only 004B7D9D/004B7DB7
//    reference it), so the compiler branch is dead in the original too; the cell is defined here zero-initialised as
//    the original .data word, and the full ~CScriptCompiler cascade is reconstructed.
//  - ~CScriptCompilerInternal::ShutDown clears [+5B8]/[+5BC]/[+610]/[+60C] but not [+228]/[+24C]/[+5A4] (as original).
//  - ~CResHelper<CResNSS,2009> resets the vptr to its own table (0074C480: {005D1240}) before releasing.
//  - The mode-2 ([007A39DC] == 2, server-owned resource manager) teardown of [007A3A28], key table, resource
//    directories, TLK and CExoResMan is reproduced although the single-player client never runs in mode 2.
#include <cstdint>
#include <cstring>

namespace {
using U = std::uint32_t;
inline void* ptr(U a) { return reinterpret_cast<void*>(a); }
inline U word(const void* p) { return reinterpret_cast<U>(p); }
inline U rd(U a) { U v; std::memcpy(&v, ptr(a), 4); return v; }
inline void wr(U a, U v) { std::memcpy(ptr(a), &v, 4); }
// thiscall scalar deleting destructor through vtable slot 0 with flag 1 (`mov e?x,[ecx]; push 1; call [e?x]`)
inline void delete_virtual(U object) {
    using F = void*(__thiscall*)(void*, U);
    (void)reinterpret_cast<F>(rd(rd(object)))(ptr(object), 1u);
}
struct ExoString { U data, length; };   // CExoString {char* +0, int +4}
}

namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }   // [007A39E0] CExoBase* [0068]
namespace frame_control {
extern unsigned message_manager_007a3a00;      // [007A3A00] CVirtualMachine* (value) [0456]
extern unsigned resource_manager_007a39e8;     // [007A39E8] CExoResMan* (value) [0151]
}
namespace menu_resources { struct Manager; void __fastcall DestroyOwnedManager00408670(Manager*, void*); } // 00408670 ~CExoResMan [0149]

extern "C" {
// ---------------- existing providers (c41 map, exact decorated arity) ----------------
void __cdecl live_free(void*);                                                              // 006FA390 [0004]
extern unsigned char kotor_rules_cell8_007a39dc;                                            // [007A39DC] app mode byte [0431]
extern void* kotor_cell_tlk_007a3a08;                                                       // [007A3A08] CTlkTable* [0384]
extern void* kotor_cell_rules_007a3a28;                                                     // [007A3A28] CSWSRules* (mode 2) [0421]
void __fastcall recovered_debug_timestamp_005e9410(void* debug, void*, void* out);          // 005E9410 CExoDebug::GetCurrentTimestamp RET 4 [0854]
const char* __fastcall frame_LogCStr_005e5670(void* string, void*);                         // 005E5670 CExoString::CStr [0459]
void* __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);      // 005E5A90 CExoString(char*) RET 4 [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                         // 005E5C20 ~CExoString [0459]
void __fastcall kotor_fastcall_004b69c0(void* internal, void*, U a1, U a2);                 // 004B69C0 CServerExoAppInternal::Shutdown RET 8 [claude_p2s_b_misc]
void __fastcall kotor_fastcall_004d7e10(void* array, void*);                                // 004D7E10 ~CGameObjectArray [0577]
void __fastcall kotor_fastcall_004b1360(void* info, void*);                                 // 004B1360 ~CServerInfo [0577]
void __fastcall kotor_fastcall_005d5600(void* net, void*);                                  // 005D5600 CNetLayer::ShutDown (EAX unused) [0577]
void __fastcall kotor_fastcall_005d5720(void* net, void*);                                  // 005D5720 ~CNetLayer [0577]
U __fastcall gui_load_resman_nuke_directory_00409480(void* resman, void*, void* path, U a, U b); // 00409480 CExoResMan::NukeDirectory RET 0Ch [0395]
void __fastcall kotor_fastcall_004088c0(void* resman, void*, void* name);                   // 004088C0 CExoResMan::RemoveFixedKeyTableFile RET 4 [0576]
void __fastcall kotor_fastcall_004088d0(void* resman, void*, void* name);                   // 004088D0 CExoResMan::RemoveResourceDirectory RET 4 [0576]
void __fastcall kotor_fastcall_0041e100(void* tlk, void*);                                  // 0041E100 ~CTlkTable [0578]
void __fastcall kotor_fastcall_004015a0(void* object, void*);                               // 004015A0 void_return (inline dtor) [0399]
void __fastcall kotor_fastcall_0052b950(void* factions, void*);                             // 0052B950 ~CFactionManager [claude_closeB_unload]
void __fastcall kotor_fastcall_004b11c0(void* ai, void*);                                   // 004B11C0 CServerAIMaster::ClearEventQueue [claude_closeB_unload]
void __fastcall exo_array_list_destruct_0056b6f0(void* list, void*);                        // 0056B6F0 CExoArrayList::~CExoArrayList [0464]
void __stdcall c2da_destroy_array_006fa852(void* first, U size, int count, void* destroy);   // 006FA852 eh vector destructor iterator [0186]
unsigned __fastcall c2da_not_equal_text_00406060(void* ref, void*, const char* text);       // 00406060 CResRef::operator!=(char const*) [0264]
void __fastcall c2da_cancel_00409b00(void* res, void*);                                     // 00409B00 CRes::CancelRequest [0264]
unsigned __fastcall c2da_release_00409cf0(void* resman, void*, void* res);                  // 00409CF0 CExoResMan::ReleaseResObject RET 4 [0264]
void __fastcall kotor_fastcall_0059ea30(void* string, void*);                               // 0059EA30 jmp ~CExoString [d1_bodies]

// [007A3A04] CScriptCompiler* - never written by the original outside StopServices; .data word, initially 0.
void* kotor_cell_compiler_007a3a04 = nullptr;

// ---------------- bodies defined here ----------------
void __fastcall quit_server_stop_services_004b7c60(void* internal, void*);
void __fastcall kotor_fastcall_004b1200(void* ai, void*);
void __fastcall kotor_fastcall_005d0fb0(void* vm, void*);
void __fastcall kotor_fastcall_005d1120(void* compiler, void*);
void __fastcall kotor_fastcall_005d14d0(void* internal, void*);
void __fastcall kotor_fastcall_005d13c0(void* internal, void*);
void __fastcall kotor_fastcall_005d1370(void* entry, void*);
void __fastcall kotor_fastcall_005d11e0(void* helper, void*);
void* __fastcall kotor_fastcall_005d1240(void* helper, void*, U flags);
void __fastcall kotor_fastcall_005d1260(void* entry, void*);
void __fastcall kotor_fastcall_005d1140(void* entry, void*);
void __fastcall kotor_fastcall_005d1190(void* entry, void*);
U __fastcall kotor_fastcall_005d5020(void* net, void*);
}
namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction { void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*); } // 005E9AB0 ~CExoLinkedList<uint> [0091]

namespace {
// 0074C480 CResHelper<CResNSS,2009> vftable: {005D1240 scalar deleting destructor} (next dword is string data).
using DeletingDestructor = void* (__fastcall*)(void*, void*, U);
const DeletingDestructor res_helper_nss_vtable_0074c480[] = { &kotor_fastcall_005d1240 };

// `if (a = [o+off]) { 006FA852(a, size, [a-4], dtor); free(a-4) }` (new[] cookie array)
void destroy_cookie_array(U member, U size, void* dtor) {
    const U a = rd(member);
    if (a == 0) return;
    c2da_destroy_array_006fa852(ptr(a), size, static_cast<int>(rd(a - 4)), dtor);
    live_free(ptr(a - 4));
}
// `if (p = [o+off]) free(p)` (no clear)
void free_member(U member) { if (rd(member) != 0) live_free(ptr(rd(member))); }
void* string_dtor() { return reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20); }
}

// ===================================================================================== 004B7C60
void __fastcall quit_server_stop_services_004b7c60(void* internal, void*) {
    const U s = word(internal);                                                      // 004B7C80 esi
    ExoString stamp = {0, 0};                                                        // 004B7C83..004B7C87 CExoString()
    recovered_debug_timestamp_005e9410(ptr(rd(word(kotor_menu_10_19::dependencies::base_007a39e0) + 8)), nullptr,
                                       &stamp);                                      // 004B7C8C..004B7CA0
    (void)frame_LogCStr_005e5670(&stamp, nullptr);                                   // 004B7CA5..004B7CA9 (result unused)
    kotor_fastcall_004b69c0(internal, nullptr, 0, 0);                                // 004B7CAE..004B7CB2 Shutdown(0,0)
    if (const U a = rd(s + 0x1005c)) {                                               // 004B7CB7..004B7CBF
        kotor_fastcall_004d7e10(ptr(a), nullptr);                                    // 004B7CC1..004B7CC3 ~CGameObjectArray
        live_free(ptr(a));                                                           // 004B7CC8..004B7CCE
        wr(s + 0x1005c, 0);                                                          // 004B7CD1
    }
    if (const U a = rd(s + 0x10044)) {                                               // 004B7CD7..004B7CDF
        kotor_fastcall_004b1200(ptr(a), nullptr);                                    // 004B7CE1..004B7CE3 ~CServerAIMaster
        live_free(ptr(a));                                                           // 004B7CE8..004B7CEE
        wr(s + 0x10044, 0);                                                          // 004B7CF1
    }
    if (const U a = rd(s + 0x10010)) {                                               // 004B7CF7..004B7CFF CSWSMessage*
        delete_virtual(a);                                                           // 004B7D01..004B7D05
        wr(s + 0x10010, 0);                                                          // 004B7D07
    }
    if (const U a = rd(s + 0x10004)) {                                               // 004B7D0D..004B7D15
        kotor_fastcall_004b1360(ptr(a), nullptr);                                    // 004B7D17..004B7D19 ~CServerInfo
        live_free(ptr(a));                                                           // 004B7D1E..004B7D24
        wr(s + 0x10004, 0);                                                          // 004B7D27
    }
    if (const U net = rd(s + 0x10040)) {                                             // 004B7D2D..004B7D35
        (void)kotor_fastcall_005d5020(ptr(net), nullptr);                            // 004B7D37 EndServerMode
        kotor_fastcall_005d5600(ptr(rd(s + 0x10040)), nullptr);                      // 004B7D3C..004B7D42 ShutDown (reload)
        if (const U a = rd(s + 0x10040)) {                                           // 004B7D47..004B7D4F (reload)
            kotor_fastcall_005d5720(ptr(a), nullptr);                                // 004B7D51..004B7D53 ~CNetLayer
            live_free(ptr(a));                                                       // 004B7D58..004B7D5E
        }
        wr(s + 0x10040, 0);                                                          // 004B7D61
    }
    if (const U vm = frame_control::message_manager_007a3a00) {                      // 004B7D67..004B7D6F
        kotor_fastcall_005d0fb0(ptr(vm), nullptr);                                   // 004B7D71..004B7D73 ~CVirtualMachine
        live_free(ptr(vm));                                                          // 004B7D78..004B7D7E
        frame_control::message_manager_007a3a00 = 0;                                 // 004B7D81
    }
    if (const U a = rd(s + 0x1000c)) {                                               // 004B7D87..004B7D8F CSWVirtualMachineCommands*
        delete_virtual(a);                                                           // 004B7D91..004B7D95
        wr(s + 0x1000c, 0);                                                          // 004B7D97
    }
    if (void* const compiler = kotor_cell_compiler_007a3a04) {                       // 004B7D9D..004B7DA5
        kotor_fastcall_005d1120(compiler, nullptr);                                  // 004B7DA7..004B7DA9 ~CScriptCompiler
        live_free(compiler);                                                         // 004B7DAE..004B7DB4
        kotor_cell_compiler_007a3a04 = nullptr;                                      // 004B7DB7
    }
    static const char* const nuke[3] = {"TEMP:", "CURRENTGAME:", "GAMEINPROGRESS:"}; // [00745310] [0073D8F8] [0074514C]
    for (const char* name : nuke) {                                                  // 004B7DBD..004B7E54 (unrolled x3)
        ExoString t;
        frame_LogStringCtor_005e5a90(&t, nullptr, name);
        (void)gui_load_resman_nuke_directory_00409480(ptr(frame_control::resource_manager_007a39e8), nullptr, &t,
                                                      1, 1);                         // push 1; push 1; push &t
        frame_LogStringDtor_005e5c20(&t, nullptr);
    }
    if (kotor_rules_cell8_007a39dc == 2) {                                           // 004B7E59..004B7E60
        if (void* const rules = kotor_cell_rules_007a3a28) {                         // 004B7E66..004B7E6E
            delete_virtual(word(rules));                                             // 004B7E70..004B7E74
            kotor_cell_rules_007a3a28 = nullptr;                                     // 004B7E76
        }
        {
            ExoString t;
            frame_LogStringCtor_005e5a90(&t, nullptr, "HD0:chitin");                 // 004B7E7C..004B7E85 [00745524]
            kotor_fastcall_004088c0(ptr(frame_control::resource_manager_007a39e8), nullptr, &t); // 004B7E8A..004B7E9A
            frame_LogStringDtor_005e5c20(&t, nullptr);                               // 004B7E9F..004B7EA7
        }
        static const char* const dirs[3] = {"OVERRIDE:", "ERRORTEX:", "SERVERVAULT:"}; // [0074553C] [00745530] [00745514]
        for (const char* name : dirs) {                                              // 004B7EAC..004B7F37 (unrolled x3)
            ExoString t;
            frame_LogStringCtor_005e5a90(&t, nullptr, name);
            kotor_fastcall_004088d0(ptr(frame_control::resource_manager_007a39e8), nullptr, &t);
            frame_LogStringDtor_005e5c20(&t, nullptr);
        }
        if (void* const tlk = kotor_cell_tlk_007a3a08) {                             // 004B7F3C..004B7F44
            kotor_fastcall_0041e100(tlk, nullptr);                                   // 004B7F46..004B7F48 ~CTlkTable
            live_free(tlk);                                                          // 004B7F4D..004B7F53
            kotor_cell_tlk_007a3a08 = nullptr;                                       // 004B7F56
        }
        if (const U resman = frame_control::resource_manager_007a39e8) {             // 004B7F5C..004B7F64
            menu_resources::DestroyOwnedManager00408670(static_cast<menu_resources::Manager*>(ptr(resman)),
                                                        nullptr);                    // 004B7F66..004B7F68 ~CExoResMan
            live_free(ptr(resman));                                                  // 004B7F6D..004B7F73
            frame_control::resource_manager_007a39e8 = 0;                            // 004B7F76
        }
    }
    for (U off = 0x10048; off <= 0x10050; off += 4) {                                // 004B7F7C..004B7FD6 three lists
        if (const U a = rd(s + off)) {
            kotor_fastcall_004015a0(ptr(a), nullptr);                                // void_return (trivial dtor)
            live_free(ptr(a));
            wr(s + off, 0);
        }
    }
    if (const U a = rd(s + 0x10054)) {                                               // 004B7FDC..004B7FE4
        kotor_fastcall_0052b950(ptr(a), nullptr);                                    // 004B7FE6..004B7FE8 ~CFactionManager
        live_free(ptr(a));                                                           // 004B7FED..004B7FF3
        wr(s + 0x10054, 0);                                                          // 004B7FF6
    }
    frame_LogStringDtor_005e5c20(&stamp, nullptr);                                   // 004B7FFC..004B8008
}

// ===================================================================================== 004B1200
void __fastcall kotor_fastcall_004b1200(void* ai, void*) {
    const U s = word(ai);                                                            // 004B1218 esi
    kotor_fastcall_004b11c0(ai, nullptr);                                            // 004B1227 ClearEventQueue
    if (const U h = rd(s + 0x58)) { delete_virtual(h); wr(s + 0x58, 0); }            // 004B122C..004B123B effect list handler
    if (const U h = rd(s + 0x5c)) { delete_virtual(h); wr(s + 0x5c, 0); }            // 004B123E..004B124B item property handler
    for (U off = 0x80; off <= 0x8c; off += 4) {                                      // 004B124E..004B12D4 four heap arrays
        if (const U p = rd(s + off)) {
            live_free(ptr(rd(p)));                                                   // free([p])
            wr(p, 0);                                                                // [p] = 0
            live_free(ptr(p));                                                       // free(p)
            wr(s + off, 0);
        }
    }
    frame_LogStringDtor_005e5c20(ptr(s + 0x60), nullptr);                            // 004B12DA..004B12E2 ~CExoString +60h
    if (const U list = rd(s + 0x54)) {                                               // 004B12E7..004B12F0 event queue internal
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(ptr(list)), nullptr); // 004B12F2..004B12F4
        live_free(ptr(list));                                                        // 004B12F9..004B12FF (no clear)
    }
    c2da_destroy_array_006fa852(ptr(s + 4), 0x10, 5,
                                reinterpret_cast<void*>(&exo_array_list_destruct_0056b6f0)); // 004B1302..004B1317 CServerAIList[5]
}

// ===================================================================================== 005D0FB0
void __fastcall kotor_fastcall_005d0fb0(void* vm, void*) {
    if (const U internal = rd(word(vm))) delete_virtual(internal);                   // 005D0FB0..005D0FBA (no clear)
}

// ===================================================================================== 005D1120
void __fastcall kotor_fastcall_005d1120(void* compiler, void*) {
    if (const U internal = rd(word(compiler))) {                                     // 005D1121..005D1125
        kotor_fastcall_005d14d0(ptr(internal), nullptr);                             // 005D1127..005D1129
        live_free(ptr(internal));                                                    // 005D112E..005D1134 (no clear)
    }
}

// ===================================================================================== 005D14D0
void __fastcall kotor_fastcall_005d14d0(void* internal, void*) {
    const U s = word(internal);
    kotor_fastcall_005d13c0(internal, nullptr);                                      // 005D14F5 ShutDown
    c2da_destroy_array_006fa852(ptr(s + 0x8a8), 8, 0x20, string_dtor());             // 005D14FA..005D150F CExoString[32]
    frame_LogStringDtor_005e5c20(ptr(s + 0x62c), nullptr);                           // 005D1514..005D151F
    frame_LogStringDtor_005e5c20(ptr(s + 0x620), nullptr);                           // 005D1524..005D152F
    frame_LogStringDtor_005e5c20(ptr(s + 0x5f0), nullptr);                           // 005D1534..005D153F
    frame_LogStringDtor_005e5c20(ptr(s + 0x5b0), nullptr);                           // 005D1544..005D154F
    c2da_destroy_array_006fa852(ptr(s + 0x260), 0x34, 0x10,
                                reinterpret_cast<void*>(&kotor_fastcall_005d1370));  // 005D1554..005D1569 include stack[16]
    frame_LogStringDtor_005e5c20(ptr(s + 0x10), nullptr);                            // 005D156E..005D1576
    frame_LogStringDtor_005e5c20(ptr(s + 8), nullptr);                               // 005D157B..005D1586
}

// ===================================================================================== 005D13C0
void __fastcall kotor_fastcall_005d13c0(void* internal, void*) {
    const U s = word(internal);
    free_member(s + 0x228);                                                          // 005D13C4..005D13D6 (no clear)
    destroy_cookie_array(s + 0x24c, 0x7c, reinterpret_cast<void*>(&kotor_fastcall_005d1260)); // 005D13D9..005D13FE (no clear)
    destroy_cookie_array(s + 0x5a4, 0x1c, reinterpret_cast<void*>(&kotor_fastcall_005d1140)); // 005D1401..005D1425 (no clear)
    if (rd(s + 0x5b8) != 0) {                                                        // 005D1428..005D1430
        destroy_cookie_array(s + 0x5b8, 0x14, reinterpret_cast<void*>(&kotor_fastcall_0059ea30)); // 005D1432..005D144C
        wr(s + 0x5b8, 0);                                                            // 005D144F
    }
    if (rd(s + 0x5bc) != 0) {                                                        // 005D1455..005D145D
        destroy_cookie_array(s + 0x5bc, 0x18, reinterpret_cast<void*>(&kotor_fastcall_005d1190)); // 005D145F..005D1479
        wr(s + 0x5bc, 0);                                                            // 005D147C
    }
    if (rd(s + 0x610) != 0) {                                                        // 005D1482..005D148A
        destroy_cookie_array(s + 0x610, 8, string_dtor());                           // 005D148C..005D14A6
        wr(s + 0x610, 0);                                                            // 005D14A9
    }
    if (rd(s + 0x60c) != 0) {                                                        // 005D14AF..005D14B8
        live_free(ptr(rd(s + 0x60c)));                                               // 005D14BA..005D14C0
        wr(s + 0x60c, 0);                                                            // 005D14C3
    }
}

// ===================================================================================== 005D1370
void __fastcall kotor_fastcall_005d1370(void* entry, void*) {
    kotor_fastcall_005d11e0(static_cast<unsigned char*>(entry) + 8, nullptr);        // 005D138D..005D1398 CResHelper at +8
    frame_LogStringDtor_005e5c20(entry, nullptr);                                    // 005D139D..005D13A7 ~CExoString +0
}

// ===================================================================================== 005D11E0
void __fastcall kotor_fastcall_005d11e0(void* helper, void*) {
    const U s = word(helper);
    wr(s, word(res_helper_nss_vtable_0074c480));                                     // 005D11E8 vptr = 0074C480
    if (rd(s + 8) == 0) return;                                                      // 005D11E3..005D11EE
    if (c2da_not_equal_text_00406060(ptr(s + 0xc), nullptr, "") == 0) return;        // 005D11F0..005D11FF ([0073D71C] "")
    if (rd(s + 4) != 0) {                                                            // 005D1201..005D1206
        c2da_cancel_00409b00(ptr(rd(s + 8)), nullptr);                               // 005D1208..005D120B CRes::CancelRequest
        wr(s + 4, 0);                                                                // 005D1210
    }
    if (c2da_release_00409cf0(ptr(frame_control::resource_manager_007a39e8), nullptr, ptr(rd(s + 8))) != 0) // 005D1217..005D1228
        return;
    if (const U res = rd(s + 8)) delete_virtual(res);                                // 005D122A..005D1235
    wr(s + 8, 0);                                                                    // 005D1237
}

// ===================================================================================== 005D1240
void* __fastcall kotor_fastcall_005d1240(void* helper, void*, U flags) {
    kotor_fastcall_005d11e0(helper, nullptr);                                        // 005D1243
    if ((flags & 1u) != 0) live_free(helper);                                        // 005D1248..005D1255
    return helper;                                                                   // 005D1258
}

// ===================================================================================== 005D1260
void __fastcall kotor_fastcall_005d1260(void* entry, void*) {
    const U s = word(entry);
    free_member(s + 0x4c);                                                           // 005D127D..005D1292
    destroy_cookie_array(s + 0x50, 8, string_dtor());                                // 005D1295..005D12B7
    free_member(s + 0x54);                                                           // 005D12BA..005D12C7
    free_member(s + 0x58);                                                           // 005D12CA..005D12D7
    free_member(s + 0x5c);                                                           // 005D12DA..005D12E7
    destroy_cookie_array(s + 0x60, 8, string_dtor());                                // 005D12EA..005D130B
    free_member(s + 0x64);                                                           // 005D130E..005D131C
    free_member(s + 0x68);                                                           // 005D131F..005D132C
    frame_LogStringDtor_005e5c20(ptr(s + 0x20), nullptr);                            // 005D132F..005D1337
    frame_LogStringDtor_005e5c20(ptr(s + 0x18), nullptr);                            // 005D133C..005D1344
    frame_LogStringDtor_005e5c20(entry, nullptr);                                    // 005D1349..005D1353
}

// ===================================================================================== 005D1140
void __fastcall kotor_fastcall_005d1140(void* entry, void*) {
    frame_LogStringDtor_005e5c20(static_cast<unsigned char*>(entry) + 0x14, nullptr); // 005D115D..005D1168
    frame_LogStringDtor_005e5c20(entry, nullptr);                                    // 005D116D..005D1177
}

// ===================================================================================== 005D1190
void __fastcall kotor_fastcall_005d1190(void* entry, void*) {
    frame_LogStringDtor_005e5c20(static_cast<unsigned char*>(entry) + 0xc, nullptr); // 005D11AD..005D11B8
    frame_LogStringDtor_005e5c20(static_cast<unsigned char*>(entry) + 4, nullptr);   // 005D11BD..005D11C8
}

// ===================================================================================== 005D5020
U __fastcall kotor_fastcall_005d5020(void* net, void*) {
    const U s = word(net);
    if (rd(s + 0x2003c) == 0) return 0;                                              // 005D5020..005D502C
    wr(s + 0x2003c, 0);                                                              // 005D502D
    return 1;                                                                        // 005D5037
}
