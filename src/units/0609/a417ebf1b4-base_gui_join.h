#pragma once
#include "e5be6a5a64-window_live_views.h"
#include "b1af77cc4b-window_service_joins.h"
#include "43ca02de5e-normal.h"
#include "b14bd1e217-base_ini.h"
namespace kotor_base_gui_join {
// Address joins only. Tables/Views have genuine main6-owned persistent storage;
// this does not certify provider completeness, publish a window or create objects.
bool attach(client_services_start::Globals&,client_services::Dependencies&,
            kotor_menu_live_views::Views&,menu_normal_40_49::Providers&,
            const kotor_menu_aliases::Cells8&) noexcept;
// Invoke only at the original constructor sites, on the real caller allocation
// with its C++ lifetime established. No allocation, publication or second ctor.
base_ini_reconstruction::Base* construct_base(base_ini_reconstruction::Base&);
menu_normal_40_49::Gui* construct_gui(menu_normal_40_49::Gui&);
}
static_assert(sizeof(base_ini_reconstruction::Base)==24,"actual Base receiver");
static_assert(sizeof(menu_normal_40_49::Gui)==0xc24,"actual GUI receiver");
static_assert(sizeof(menu_normal_40_49::Providers)==160,"exact GUI table");
static_assert(sizeof(base_ini_reconstruction::Bindings)==112,"existing sole INI table");
