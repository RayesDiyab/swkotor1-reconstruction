#pragma once
#include "d06e8b44c9-manager_demand.h"
#include "572d069072-base_cancel_service.h"
#include "7edb7fc583-ini_addhead.h"
#include "8df1242c60-add_key.h"
#include "40141ae87f-ini_cleanup.h"
#include <type_traits>
static_assert(std::is_same<base_ini_reconstruction::List,kotor_list::Internal>::value,"One canonical Internal");
static_assert(sizeof(base_ini_reconstruction::Node)==12 && sizeof(kotor_list::Internal)==12,"Selected x86 list and Node");
