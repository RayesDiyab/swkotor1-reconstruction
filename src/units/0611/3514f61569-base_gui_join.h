#pragma once
#include "fd73ec7604-window_live_views.h"
#include "4c8404627d-window_service_joins.h"
#include "6edcbe2bf4-normal.h"
#include "09b393f3a0-base_ini.h"
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
