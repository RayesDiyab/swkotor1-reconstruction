#pragma once
// Declarations derived from the selected committed implementation.
#include "00a0946121-resources.h"
#include <type_traits>
static_assert(sizeof(menu_resources::Ref) == 16 && alignof(menu_resources::Ref) == 1);
static_assert(std::is_trivially_copyable<menu_resources::Ref>::value);
extern "C" unsigned __fastcall recovered_player_load_bic_00561d70(void* player,void*);
extern "C" unsigned __fastcall recovered_player_deserialize_bic_00560e60(void* player,void*,menu_resources::Ref name,void* creature);
