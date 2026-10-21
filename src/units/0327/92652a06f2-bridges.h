#pragma once
#include <windows.h>
#include "c76910f86e-winmain.h"
#include "e1a5556035-window_shell.h"
#include "ba6cb3db7f-reinit.h"
#include "a41c353511-message.h"
#include "3d96915e09-start_body.h"

namespace kotor_menu_window {
// Task6 publishes this borrowed descriptor. This module defines no owner,
// singleton, heap, game object, original-address cell or substitute frame.
// Hooks provide a genuine per-invocation source frame and canonical global
// aliases; they cannot be satisfied by the old fixture/diagnostic Owner.
struct Publication {
    kotor_winmain::Context* winmain;
    kotor_window_shell::Context* shell;
    kotor_reinit::Context* (__cdecl* enter_reinit)();
    void (__cdecl* leave_reinit)(kotor_reinit::Context*) noexcept;
    kotor_message::Context* (__cdecl* enter_message)(std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
    void (__cdecl* leave_message)(kotor_message::Context*) noexcept;
};
// False means this module does not own that route. It does not supply a result.
bool __cdecl winmain_window_route(const kotor_winmain::Call&,kotor_winmain::Tick*);
bool __cdecl shell_import(const kotor_window_shell::Call&,kotor_window_shell::Result*);
bool __cdecl message_import(const kotor_message::Call&,kotor_message::Result*);
}
extern kotor_menu_window::Publication const* const kotor_menu_window_publication;
extern "C" void __cdecl kotor_menu_bind_winmain_context();
extern "C" client_services_start::Word __cdecl kotor_menu_service_00403800(const client_services_start::Call*);
extern "C" LRESULT CALLBACK kotor_menu_source_wndproc(HWND,UINT,WPARAM,LPARAM);
