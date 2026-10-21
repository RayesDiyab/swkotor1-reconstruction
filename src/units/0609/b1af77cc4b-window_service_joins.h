#pragma once
#include "d74f8dd3e3-start_body.h"
#include "9ccb42842d-services.h"
#include "e4915bc513-global_aliases.h"

namespace kotor_menu_service_joins {
// A narrowly scoped proposal: bind four StartServices slots and the native
// client input pointer. Preserve the seven other slots, byte cells and callbacks.
// This does not publish either table or establish startup/lifecycle readiness.
bool attach(client_services_start::Globals&, client_services::Dependencies&,
            const kotor_menu_aliases::Cells8&) noexcept;
}
