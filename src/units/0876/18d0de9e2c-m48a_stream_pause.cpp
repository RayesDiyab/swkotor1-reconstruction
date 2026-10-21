// lane-m48-a: CExoStreamingSoundSourceInternal::Pause 005DC3C0 (thiscall, no stack args, RET 0; Ghidra void) and
// CClientExoApp::GetCreditSequenceInProgress 005EE1E0 (thiscall, RET 0, EAX = [[ecx+4]+4B8h]).
// Re-port of the M35-epoch donor sound/donors/donor-stream-pause/sound_pause.cpp (excluded from M47 for its
// StartupString C2011 / absolute M35 includes) onto the current canonical headers, without its mutant macros and
// without its private 00405F70 wrapper (the existing single provider c2da_ref_to_string_00405f70 is called).
// Consumer spelling: sound_pause29_required::stream_pause_005dc3c0(void*, void*) -> U (sound-internal, stream play).
// Evidence: swkotor.exe GOG K1 dumpbin /disasm 005DC3C0..005DC47E, 005EE1E0..005EE1E9 (lane-m48-a/d/).
//   005DC3DF call 005B3190 (CExoString ctor, EH state 0 at 005DC3EC) -> StartupString() ; 005DC46A call 005E5C20
//   (dtor, state -1) -> ~StartupString (C++ RAII; emitted cleanup, D30-FINAL).
//   0074D330 "evil_ending" (pinned exe bytes). Miles IAT: 0073D568 AIL_stream_status, 0073D570 AIL_pause_stream,
//   0073D574 AIL_stream_position (lane-sound/mss_iat.txt; mss32.lib per D17/D77).
// Not reached with Disable Sound=1 (D17): the CExoSound internal object is never created.
#include <cstdint>
#include "84a5500766-startup_string.h"
#include "163d10ec4d-resources.h"                              // menu_resources::Ref / String (= kotor_reconstruction::StartupString)
#include "df72b7ab87-window_shared_cells.h"   // kotor_menu_window_cells::manager_007a39fc
#include "262ce3d515-ini_comparison.h"                         // ini_comparison::CompareNC005e58c0

extern "C" {
__declspec(dllimport) std::uint32_t __stdcall AIL_stream_status(void*);
__declspec(dllimport) void __stdcall AIL_pause_stream(void*, std::int32_t);
__declspec(dllimport) std::uint32_t __stdcall AIL_stream_position(void*);
void __fastcall c2da_ref_to_string_00405f70(menu_resources::Ref*, void*, menu_resources::String*); // existing provider
}

namespace {
using U = std::uint32_t;
template<class T> T& field(void* base, U offset) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(base) + offset); }
}

extern "C" U __fastcall client_get_credit_sequence_in_progress_005ee1e0(void* client, void* /*unused_edx*/) {
    return field<U>(field<void*>(client, 4), 0x4B8);                    // 005EE1E0..005EE1E9
}

namespace sound_pause29_required {
U __fastcall stream_pause_005dc3c0(void* stream, void* /*unused_edx*/) {
    kotor_reconstruction::StartupString name;                            // 005DC3DB..005DC3DF (005B3190)
    c2da_ref_to_string_00405f70(reinterpret_cast<menu_resources::Ref*>(static_cast<unsigned char*>(stream) + 9),
                                nullptr, &name);                         // 005DC3E4..005DC3F4 (ecx = esi+9)
    if (ini_comparison::CompareNC005e58c0(&name, nullptr, "evil_ending") != 0) { // 005DC3F9..005DC409
        void* const client = field<void*>(kotor_menu_window_cells::manager_007a39fc, 4); // 005DC40B..005DC411
        if (client_get_credit_sequence_in_progress_005ee1e0(client, nullptr) == 0) return 0; // 005DC414..005DC41B
    }
    void* const system = field<void*>(stream, 0x20);                    // 005DC41D
    if (field<U>(system, 8) == 0) return 0;                              // 005DC420..005DC425
    void* const handle = field<void*>(stream, 0);                        // 005DC427
    if (handle == nullptr) return 0;                                     // 005DC429..005DC42B
    if (field<U>(system, 8) == 0) return 0;                              // 005DC42D..005DC432 (re-read)
    if (AIL_stream_status(handle) != 4u) return 0;                       // 005DC434..005DC445 (sub 4; neg; sbb; inc)
    AIL_pause_stream(field<void*>(stream, 0), 1);                        // 005DC447..005DC44C ([esi] re-read)
    field<U>(stream, 0x40) = AIL_stream_position(field<void*>(stream, 0)); // 005DC452..005DC45B
    return 0;  // D2 label: EAX at 005DC47E is the dtor's scratch; every caller discards it (lane-sound contracts)
}
}
