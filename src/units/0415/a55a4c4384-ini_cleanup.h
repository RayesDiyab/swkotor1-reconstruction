#pragma once
#include "8c4ff9b15c-base_ini.h"
#include "6542d604d6-allocator_live.h"
#include "fcbc7f08ae-menu_targets.h"
#include "77fb10ec4f-startup_views.h"
namespace base_ini_reconstruction {
__declspec(noinline) void __fastcall destroy_list_005e9ab0(List*,void*);
__declspec(noinline) void* __fastcall remove_head_005e9c80(List*,void*);
__declspec(noinline) void __fastcall destroy_section_005ea5a0(Section*,void*);
__declspec(noinline) void __fastcall destroy_cache_005ea650(Ini*,void*);
__declspec(noinline) void __fastcall destroy_internal_005eb500(Ini*,void*);
// Borrowed original global/literal owners. No storage is defined by this packet.
// The empty RemoveHead branch still requires the genuine debug provider.
Base* current_root_base() noexcept;
extern const char diagnostic_source_0074e208[];
extern const char diagnostic_empty_0074e360[];
void __fastcall warning_005e9400(kotor_menu20::Debug* debug,void* unused_edx,int line,
                               const char* source,const char* message);
}

namespace base_ini_reconstruction {
void __fastcall warning_internal_005ec2b0(void*,void*,int,const char*,const char*);
}
