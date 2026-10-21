#pragma once
#include "af11f4829d-start_body.h"
#include "6acfe4dba0-services.h"
#include "2b112e8e36-global_aliases.h"

namespace kotor_menu_service_joins {
// A narrowly scoped proposal: bind four StartServices slots and the native
// client input pointer. Preserve the seven other slots, byte cells and callbacks.
// This does not publish either table or establish startup/lifecycle readiness.
bool attach(client_services_start::Globals&, client_services::Dependencies&,
            const kotor_menu_aliases::Cells8&) noexcept;
}
