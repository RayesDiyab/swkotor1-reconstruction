#pragma once
#include <cstdint>
namespace menu_continuation { struct AppManager; extern AppManager* app_manager_007a39fc; }
namespace kotor_reconstruction { struct ArrayMutations; }
namespace recovered_player_message { unsigned overrun_004d6230(void*); }
extern "C" {
extern void* kotor_cell_rules_007a3a28;
void* __cdecl live_operator_new(std::uint32_t);void __cdecl live_free(void*);
void* __fastcall kotor_fastcall_005ed570(void*,void*) noexcept;
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,std::uint32_t,void**);
unsigned __fastcall kotor_fastcall_00692630(void*,void*);
unsigned char __fastcall kotor_fastcall_004d6720(void*,void*,unsigned);
unsigned short __fastcall kotor_fastcall_004d6750(void*,void*,unsigned);
unsigned __fastcall kotor_fastcall_004d6780(void*,void*,unsigned);
unsigned __fastcall kotor_fastcall_004d66f0(void*,void*);
void* __fastcall kotor_m15_combat_information_construct_00552280(void*,void*) noexcept(false);
void* __fastcall recovered_combat_modifier_construct_0054f260(void*,void*);
void __fastcall kotor_m15b_combat_information_destruct_00550e00(void*,void*) noexcept(false);
void __fastcall gui_load_array_add_00671c00(kotor_reconstruction::ArrayMutations*,void*,std::uint32_t);
void __fastcall kotor_m15b_creature_stats_clear_feats_00648320(void*,void*);
void __fastcall kotor_m15_array_u16_add_unique_005a7670(void*,void*,std::uint32_t);
void __fastcall recovered_client_set_gui_skill_rank_0064ce90(void*,void*,void*,std::uint32_t,std::uint32_t,std::uint32_t) noexcept;
void __fastcall recovered_client_stats_set_skill_rank_00647560(void*,void*,std::uint32_t,std::uint32_t);
void __fastcall recovered_client_stats_set_combat_information_00647a90(void*,void*,void*);
std::uint32_t __fastcall recovered_client_handle_character_sheet_00667cc0(void*,void*);
}

