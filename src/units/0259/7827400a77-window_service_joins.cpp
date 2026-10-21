#include "4c8404627d-window_service_joins.h"
#include "df72b7ab87-window_shared_cells.h"
#include "fd73ec7604-window_live_views.h"

namespace kotor_menu_service_joins {
bool attach(client_services_start::Globals& start,
            client_services::Dependencies& client,
            const kotor_menu_aliases::Cells8& cells) noexcept {
    using client_services_start::Global;
    if (!kotor_menu_live_views::canonical(cells)) return false;
    struct Join { Global slot; void volatile* address; };
    const Join joins[] = {
        {Global::Manager, cells.manager_007a39fc},
        {Global::Input, cells.input_007a39e4},
        {Global::Aurora, cells.aurora_007a39f8},
        {Global::Height, cells.height_0078d1d8}
    };
    for (const auto& join : joins) {
        void volatile* prior = start.words[static_cast<unsigned>(join.slot)];
        if (prior && prior != join.address) return false;
    }
    // Exact void* volatile* assignment: no pointer-object/Word type punning.
    auto* input = &kotor_menu_window_cells::input_007a39e4;
    if (client.input_007a39e4 && client.input_007a39e4 != input) return false;
    for (const auto& join : joins)
        start.words[static_cast<unsigned>(join.slot)] = join.address;
    client.input_007a39e4 = input;
    return true;
}
}
