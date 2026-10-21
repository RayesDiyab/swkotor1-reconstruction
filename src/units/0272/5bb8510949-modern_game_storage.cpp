#include "35e3e29e81-live_publication.h"
#include "89b65663c9-startup_views.h"
#include <cstddef>

namespace {
kotor_live_publication::Attachment owned_window_attachment{};
}
kotor_live_publication::Attachment* const kotor_menu_window_attachment =
    &owned_window_attachment;
kotor_menu_window::Publication const* const kotor_menu_window_publication =
    &owned_window_attachment.publication;

namespace kotor_menu_10_19::dependencies {
// One source-owned pointer cell. Reconstructed WinMain alone publishes its
// real Base constructor result here, then destroys and clears it in its own
// shutdown order. This cell owns no Base object or constructor invocation.
void* base_007a39e0 = nullptr;
}
static_assert(sizeof(void*) == 4 && sizeof(kotor_live_publication::Attachment) == 100,
              "Exact native x86 metadata contract");
static_assert(offsetof(kotor_live_publication::Attachment, publication) == 0,
              "Publication export borrows the sole metadata member");
