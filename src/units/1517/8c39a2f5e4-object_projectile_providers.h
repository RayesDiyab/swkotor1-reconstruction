#pragma once
#include <cstdint>
namespace kotor_list { struct Internal; }
namespace kotor_reconstruction { struct StartupString; }
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
namespace base_ini_reconstruction {
void* __fastcall remove_head_005e9c80(kotor_list::Internal*,void*);
void __fastcall destroy_list_005e9ab0(kotor_list::Internal*,void*);
}
namespace object_projectile {
}
extern "C" {
void* __fastcall kotor_fastcall_005ed580(void*,void*,std::uint32_t);
void* __fastcall kotor_fastcall_005ed570(void*,void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,std::uint32_t,void**);
void __fastcall resource_append_005e9dc0(kotor_list::Internal*,void*,void*);
char* __fastcall WindowCStr005e5670(kotor_reconstruction::StartupString*,void*);
int __cdecl crt_rand_006fc447();
}
