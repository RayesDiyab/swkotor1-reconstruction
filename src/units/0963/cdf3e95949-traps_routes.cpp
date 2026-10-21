// lane-m48-traps (main): routes-shutdown consumer (kotor_original_calls.h).
// quit lane: the D83 whole-function trap is replaced by a forwarder to the reconstructed body
// quit_server_stop_services_004b7c60 (srcw5\actions\claude_quit.cpp, which must be linked).
#include "c5c41130e2-kotor_original_calls.h"

extern "C" {
void __fastcall quit_server_stop_services_004b7c60(void*, void*);   // claude_quit.cpp
// 004B7C60 CServerExoAppInternal::StopServices (RET). Only caller CServerExoApp::StopServices 004AE900, called by
// CAppManager::DestroyServer 00401293 / ~CAppManager 004012F0 / CreateServer 004013A9 (Quit to main menu
// 005FCBF5, CSWGuiSaveLoad::LoadGame 006CB316, WinMain exit 00404A21 / 00404A75).
void __fastcall kotor_fastcall_004b7c60(void* internal, void*) {
    quit_server_stop_services_004b7c60(internal, nullptr);
}
}
