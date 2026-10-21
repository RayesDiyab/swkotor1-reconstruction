#pragma once
#include "92652a06f2-bridges.h"

#if !defined(_M_IX86) || !defined(_MT) || defined(_DLL)
#error Functional host requires one modern MSVC x86 static CRT family (/MT).
#endif

namespace kotor_functional {
// Borrowed supported CRT/WinMain arguments. The game owner must not free argv.
struct StartupArguments {
    HINSTANCE instance;
    HINSTANCE previous;
    LPSTR command_line;
    int show;
    int argc;
    char** argv;
};

struct GameLifetime;
// Bootstrap metadata only; this is not game storage or a ready-provider table.
struct PreparedGame {
    GameLifetime* lifetime;
    const kotor_menu_window::Publication* window;
    client_services_start::Globals* start;
};

int run(const StartupArguments&);
}

// Deliberately undefined. Task6 must adopt genuine source-owned initializers,
// durable contexts/frames, exact provider producers, and consumers of the source
// allocator's published mode1 Win32 heap. CRT-owned allocations stay separate.
// WinMain itself still constructs Base and Manager on its real route.
// prepare must retain partial ownership in result before any throwing operation.
extern "C" bool __cdecl kotor_functional_game_prepare(
    const kotor_functional::StartupArguments*, kotor_functional::PreparedGame*);
// Must accept an empty/partial result. Keep Publication/contexts/views/providers
// live through shutdown callbacks and quiescence. Task20 detach must succeed
// before destroying its borrowed contexts/views or returning; false retains
// ownership and must never permit allocator/heap teardown. Then destroy actual
// game-owned state with each owner's matching family. Caller context restoration
// runs after this release returns, including preparation failure/C++ unwind.
// Bootstrap owns INI allocate_006fa7e6=live_operator_new. Preserve that exact slot
// through the last INI/Base/string drain; bootstrap detaches it after release.
extern "C" void __cdecl kotor_functional_game_release(
    kotor_functional::PreparedGame*) noexcept;
