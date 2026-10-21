#pragma once
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Approved MSVC x86 compiler required
#endif
// ONE naming epoch for genuine original callees shared by lane-routes (WinMain route
// table) and lane-prepare (client_services::Dependencies slots).
// Convention (same as base_ini Bindings / lane-prepare services-fastcall.diff):
//   original __thiscall  -> extern "C" __fastcall kotor_fastcall_<addr>(self, unused EDX, stack words...)
//   original __cdecl     -> extern "C" __cdecl   kotor_cdecl_<addr>(...)
//   custom register ABI  -> extern "C" __cdecl   kotor_eax_<addr>() called only from inline asm
// Each declaration states whether lane-routes DEFINES it (authored here) or it is a
// still-missing genuine provider (UNDEF; no stub, fixture or default anywhere).
#include <cstdint>
namespace gui_ctor { struct Gui; }
// lane-prepare-declared slot producers (game_prepare_producers.h), reused verbatim.
namespace client_services { using Word = std::uint32_t; }
namespace kotor_game_prepare {
void __fastcall SetEventDescriptions005eeb10(void* self, void* edx);
void __fastcall SetGlobalUnpauseableState0062b410(void* self, void* edx, client_services::Word);
void __fastcall ShowMouse0061fa00(void* self, void* edx, int);
}
extern "C" {
// ---------------- DEFINED by lane-routes (authored from 9c10e045 evidence) -------------
// 005ED420 DisplayMainMenu outer: mov ecx,[ecx+4]; jmp 005FCA30 (callee RET4).
void __fastcall kotor_fastcall_005ed420(void* outer, void*, std::uint32_t argument);
// 005FCA30 CClientExoAppInternal::DisplayMainMenu, RET4 (also R4 @frame_R4_005fca30@12).
void __fastcall kotor_fastcall_005fca30(void* internal, void*, std::uint32_t unused);
// 005EDFD0 SetLoadStep: byte [[ecx+4]+step+3E4] = (byte)value, RET8.
void __fastcall kotor_fastcall_005edfd0(void* outer, void*, std::uint32_t step, std::uint32_t value);
// 005EDB40 IsMoviePlaying outer -> 005F2BD0 internal -> 0070C1F0 player -> 00404DD0.
std::uint32_t __fastcall kotor_fastcall_005edb40(void* outer, void*);
std::uint32_t __fastcall kotor_fastcall_005f2bd0(void* internal, void*);
int __fastcall kotor_fastcall_0070c1f0(void* player, void*);   // == Dependencies::movie_playing_0070c1f0 (fastcall epoch)
std::uint32_t __fastcall kotor_fastcall_00404dd0(void* object, void*);
// 005EE1F0 EnableMouseAtEndOfLoadModuleMovie outer -> 005F4DB0 internal.
void __fastcall kotor_fastcall_005ee1f0(void* outer, void*);
void __fastcall kotor_fastcall_005f4db0(void* internal, void*);
// WinMain-direct cdecl leaves.
void __cdecl kotor_cdecl_004015b0();                         // StoreMouseTrailsState
void __cdecl kotor_cdecl_00401610();                         // StoreScreenSaverState
void __cdecl kotor_cdecl_00401700(int argc, char** argv);    // checkCommandLineArgs
char* __cdecl kotor_cdecl_0044c980(const char* name);        // dofile
void __cdecl kotor_cdecl_0044ef00(float, float, float, float); // AurSetPauseOverlayColor (plain RET)
void __cdecl kotor_cdecl_0044da60();                         // AurEnableHeadLight
void __cdecl kotor_cdecl_0044da80();                         // AurDisableHeadLight
void __cdecl kotor_cdecl_0044d6a0();                         // AurRenderStrings (tail JMP 0046EE00)
long __cdecl crt_clock_006fc546();                           // typed UCRT clock() adapter
// ---------------- still-MISSING genuine providers (UNDEF) ----------------
void* __fastcall kotor_fastcall_0067c4c0(void* menu, void*, gui_ctor::Gui* gui);          // CSWGuiMainMenu ctor
void __fastcall kotor_fastcall_006200e0(void* internal, void*, std::uint32_t, std::uint32_t) noexcept(false); // SetInputClass-like (2,1)
void __fastcall kotor_fastcall_0040bc70(gui_ctor::Gui* gui, void*, void* panel, std::uint32_t, std::uint32_t) noexcept(false); // AddPanel
void __fastcall kotor_fastcall_005ed820(void* outer, void*, void* string);                  // CreateCaptureEventFile
void __fastcall kotor_fastcall_005ed830(void* outer, void*, void* string);                  // setUpEventPlayBack
void __fastcall kotor_fastcall_005ed840(void* outer, void*, std::int32_t rate);             // setCaptureEventRate
void* __cdecl kotor_cdecl_0044c740(const char* name, const char* extension, int, int);    // AurResGet
void __cdecl kotor_cdecl_0044c8f0(void* handle, int);                                     // AurResFree
void* __cdecl kotor_cdecl_0044bd70(void* entry);                                          // GetResource
void __cdecl kotor_eax_0044be70();      // getnextline_res: EAX=entry in, EAX=line out; preserves EBX/ESI/EDI
char* __cdecl kotor_cdecl_0044c1f0(char* line);                                           // FuncInterp
void __cdecl kotor_cdecl_0044d620();                                                      // PostTraceStrings
void __fastcall kotor_fastcall_0044d010(void* posted, void*);                             // PostedString::Post
void __cdecl kotor_cdecl_0046ee00();                                                      // countreset
// R4-consumed names reused verbatim (still MISSING; one name per original function):
void __fastcall frame_R4_005f9af0(void*, void*, unsigned, unsigned, unsigned); // R4 name (MISSING; shared)
unsigned __fastcall frame_ContainsPanel_0040bd70(gui_ctor::Gui*, void*, void*); // R4 name (MISSING; shared)
// ---- authored (window route leaves, window_leaves.cpp) ----
std::uint32_t __fastcall kotor_fastcall_0044d6d0(void* aurora, void*);        // CAurInternal::FullScreen (vtable 00741518 +4)
void __cdecl kotor_cdecl_0044ef60(); void __cdecl kotor_cdecl_0044ef70(); void __cdecl kotor_cdecl_0044ef80();
char* __cdecl kotor_cdecl_0044eec0();                                          // AurGetVersionString
std::uint32_t __fastcall kotor_fastcall_005df6e0(void* input, void*);          // GetMouseMode
std::uint32_t __fastcall kotor_fastcall_005ed7c0(void* outer, void*);          // IsClientInitialized
std::uint32_t __fastcall kotor_fastcall_005ed4d0(void* outer, void*);          // IsClientImeUI
std::uint32_t __fastcall kotor_fastcall_005ed4e0(void* outer, void*);          // GetImeUI
void __fastcall kotor_fastcall_005ee170(void* outer, void*, std::uint32_t);    // HideMouse outer
void __fastcall kotor_fastcall_005ee180(void* outer, void*, std::uint32_t);    // ShowMouse outer
void __fastcall kotor_fastcall_005ed870(void* outer, void*);                   // SetEventDescriptions outer
void __fastcall kotor_fastcall_005ed980(void* outer, void*);                   // RestartMoviesIfNecessary outer
void __fastcall kotor_fastcall_005ee160(void* outer, void*);                   // CancelMovie outer
std::uint32_t __fastcall kotor_fastcall_005df510(void* input, void*);          // GetActive outer
void __fastcall kotor_fastcall_005df590(void* input, void*, void* event);      // BufferEvent outer
void __fastcall kotor_fastcall_005df5b0(void* input, void*, std::uint32_t);    // RefreshMouseCursor outer
void __fastcall kotor_fastcall_005df5a0(void* input, void*, std::uint32_t);    // SetMouseMode outer
void __fastcall kotor_fastcall_0061f9c0(void* internal, void*, std::uint32_t); // HideMouse
void __cdecl kotor_cdecl_004015f0(); void __cdecl kotor_cdecl_00401650();      // handleMouseTrails / handleScreenSaver
// ---- still MISSING genuine providers reached by window rows ----
std::uint32_t __fastcall kotor_fastcall_0044ea30(void* aurora, void*, std::uint32_t, std::uint32_t); // CAurInternalGL::ShutDown (+20)
std::uint32_t __fastcall kotor_fastcall_0044d6e0(void* aurora, void*, void*, void*, void*, void*, std::uint32_t); // GetValidMode (+C)
std::uint32_t __fastcall kotor_fastcall_0044d8d0(void* aurora, void*);          // RestoreDesktopDisplayMode (+18)
std::uint32_t __fastcall kotor_fastcall_0044d900(void* aurora, void*);          // RestoreAuroraDisplayMode (+1C)
void* __fastcall kotor_fastcall_005df3f0(void* input, void*);                   // CExoInput ctor
void __cdecl kotor_cdecl_00401d90(); void __cdecl kotor_cdecl_00401e00();       // Deactivate/ActivateRenderWindow
void __cdecl kotor_cdecl_00401e90(std::uint32_t, std::uint32_t);                // toggleFullScreen
void __fastcall kotor_fastcall_005f9780(void* internal, void*);                 // RestartMoviesIfNecessary
void __fastcall kotor_fastcall_005f4a00(void* internal, void*);                 // CancelMovie
std::uint32_t __fastcall kotor_fastcall_005dff40(void* internal, void*);        // CExoInputInternal::GetActive
void __fastcall kotor_fastcall_005e12a0(void* internal, void*, void*);          // CExoInputInternal::BufferEvent
void __fastcall kotor_fastcall_005e0930(void* internal, void*, std::uint32_t);  // RefreshMouseCursor internal
void __fastcall kotor_fastcall_005e1740(void* internal, void*, std::uint32_t);  // SetMouseMode internal
void __fastcall kotor_fastcall_005df5e0(void* input, void*);                    // HideMouse callee (input)
void __fastcall kotor_fastcall_0040a260(void* gui, void*);                      // HideMouse callee (gui)
std::uint32_t __fastcall kotor_fastcall_005eda00(void* outer, void*);           // QueryExitProgram
std::uint32_t __cdecl kotor_cdecl_004024f0(std::uint32_t, std::uint32_t, std::uint32_t); // key handler
std::uint32_t __cdecl kotor_cdecl_00402240(std::uint32_t);                      // HandleWMCharMessage
void __fastcall kotor_fastcall_005dfb00(void* ime, void*, std::uint32_t, std::uint32_t, std::uint32_t);
void __fastcall kotor_fastcall_005dfc40(void* ime, void*, std::uint32_t, std::uint32_t, std::uint32_t);
void __cdecl kotor_cdecl_00403de0();                                            // RestoreResolutionFromMovie
unsigned __fastcall frame_R4_005ed480(void*, void*);                            // R4 name: GetCaptureEventPlayBack
// ---- authored (lifecycle_leaves.cpp) ----
void __cdecl kotor_cdecl_005e6b10();                                            // GetGameVersionInfo (now DEFINED)
void __fastcall kotor_fastcall_00401280(void* manager, void*);                  // CAppManager::DestroyServer
void __fastcall kotor_fastcall_004012c0(void* manager, void*);                  // CAppManager::~CAppManager
void __fastcall kotor_fastcall_005e66a0(void* base, void*);                     // CExoBase::~CExoBase
void __fastcall kotor_fastcall_005ed810(void* outer, void*);                    // Uninitialize outer
void __fastcall kotor_fastcall_005ed880(void* outer, void*);                    // StopServices outer
void __fastcall kotor_fastcall_005f8470(void* internal, void*);                 // CClientExoAppInternal::Uninitialize
// ---- still MISSING genuine providers below the shutdown bodies ----
void __fastcall kotor_fastcall_005ed310(void* server, void*);                   // void_return_this+4 (server pre-stop)
void __fastcall kotor_fastcall_004ae900(void* server, void*);                   // CServerExoApp::StopServices
void __fastcall kotor_fastcall_004aec70(void* server, void*);                   // CServerExoApp::~CServerExoApp
void __cdecl kotor_cdecl_00401440();                                            // SetWindowTitle
void __fastcall kotor_fastcall_005ee360(void* client, void*);                   // CClientExoApp::~CClientExoApp
void __fastcall kotor_fastcall_004ae640(void* table, void*);                    // CObjectTableManager::~
void __fastcall kotor_fastcall_00401000(void* stats, void*);                    // CSWReentrantServerStats::~
void __fastcall kotor_fastcall_005e8bb0(void*, void*);                          // CExoBaseInternal::~
void __fastcall kotor_fastcall_005e69b0(void*, void*);                          // CExoAliasList::~
void __fastcall kotor_fastcall_005e9270(void*, void*);                          // CExoTimers::~
void __fastcall kotor_fastcall_005e9430(void*, void*);                          // CExoDebug::~
void __fastcall kotor_fastcall_005ea4a0(void*, void*);                          // CExoRand::~
void __fastcall kotor_fastcall_00690910(void*, void*);                          // CSWCVisualEffect::UnloadCachedVFX
void __fastcall kotor_fastcall_005f6410(void*, void*);                          // EmptyHitcheckIgnoreList
void __fastcall kotor_fastcall_00604130(void*, void*);                          // CClientExoAppInternal::StopServices (large)
// ---- authored (shutdown_bodies.cpp): 004015A0 005ED310 00401440 004AE640 00401000 005EE360 004AEC70 004AE900
//      005E8BB0 005EC0E0 005E69B0 005E9270 005ECC80 005EA4A0 005E9430 005F6410 00690910 00604130 (declared above/below)
void __fastcall kotor_fastcall_004015a0(void*, void*);                          // void_return (`ret`)
void __fastcall kotor_fastcall_005ec0e0(void*, void*);                          // CExoAliasListInternal::~
void __fastcall kotor_fastcall_005ecc80(void*, void*);                          // CExoRandInternal::~
// ---- still MISSING genuine providers below StopServices / the destructors ----
void __fastcall kotor_fastcall_005fc6f0(void*, void*);   // CClientExoAppInternal::~ (21 member subobjects; counterpart of lane-prepare 005FBFE0)
void __fastcall kotor_fastcall_004b7230(void*, void*);   // CServerExoAppInternal::~ (server only)
void __fastcall kotor_fastcall_004b7c60(void*, void*);   // CServerExoAppInternal::StopServices (server only)
void __fastcall kotor_fastcall_005ec5c0(void*, void*);   // CExoDebugInternal::CloseLogFiles
void __fastcall kotor_fastcall_005f7190(void*, void*);   // ShutDownMoviePlayer
void __fastcall kotor_fastcall_00678880(void*, void*);   // CSoundSetList::~
void __fastcall kotor_fastcall_004d7e10(void*, void*);   // CGameObjectArray::~
void __fastcall kotor_fastcall_00676de0(void*, void*);   // CSWCWorldJournal::~
void __fastcall kotor_fastcall_005f1020(void*, void*);   // CExoImeUI::~
void __fastcall kotor_fastcall_006303b0(void*, void*);   // CGuiInGame::~
void __fastcall kotor_fastcall_0040aaa0(void*, void*);   // CSWGuiManager::~
void __fastcall kotor_fastcall_00604b40(void*, void*);   // CClientAIMaster::~
void __fastcall kotor_fastcall_004b1360(void*, void*);   // CServerInfo::~
void __fastcall kotor_fastcall_005d5600(void*, void*);   // CNetLayer::ShutDown
void __fastcall kotor_fastcall_005d5720(void*, void*);   // CNetLayer::~
void __fastcall kotor_fastcall_005d6200(void*, void*);   // CExoStreamingSoundSource::~
void __fastcall kotor_fastcall_005d62f0(void*, void*);   // CExoSound::~
void __fastcall kotor_fastcall_0041e100(void*, void*);   // CTlkTable::~
void __fastcall kotor_fastcall_0070d340(void*, void*);   // CAuroraInterface::~
void __cdecl kotor_cdecl_0044ed80();                     // CAurInternal::DestroyCAuroraInstance (jmp 0044D940)
void __fastcall kotor_fastcall_004088d0(void*, void*, void*);   // CExoResMan::RemoveResourceDirectory(String*)
void __fastcall kotor_fastcall_004088c0(void*, void*, void*);   // CExoResMan::RemoveFixedKeyTableFile(String*)
void __fastcall kotor_fastcall_00408830(void*, void*, void*);   // CExoResMan::RemoveResourceImageFile(String*)
void __fastcall kotor_fastcall_00602c10(void*, void*);   // CSWCJournal::~
void __fastcall frame_R4_0062c310(void*, void*);         // R4 name: CGuiInGame::ShutDown
// ---------------- existing M40 provider referenced (defined) ----------------
unsigned __int64 __cdecl unresolved_ftol2_006fae8c();   // world-timer-x87-conversion.obj (custom x87 ABI)
}
