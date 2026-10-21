// lane-m49-file: replacement for unit m40-file-engine-owner
// (stage path engine-file-epoch/file_engine_owner.cpp).
//
// Original behaviour (swkotor.exe, pinned image): there is no publication step.
// CExoFile/CExoFileInternal/path resolution use absolute operands that the
// linker and image loader fix:
//   005E68EB call 006FA7E6 (operator new, 14h)       -> allocate_006fa7e6
//   005EB890/005EB8B9 call 005E5C50 (CExoString =)   -> assign_005e5c50
//   005EB8CE/005EB8D6 call 005E5670 (CStr)           -> text_005e5670
//   005EB89E/005EB70C/005EB763/005EB77A/005EB795/005EB7CD
//            mov reg,[007A39E0] (CExoBase cell)       -> exo_base_007a39e0 / base_007a39e0
//   005EB8AC call 005E68A0 (alias resolve)            -> resolve_005e68a0
//   005EBAAD/005EBAD0/005EBAFE/005EBB0E/005EBB6D/... mov ecx,8328D8h / push 8328D8h
//            (CExoString object at 008328D8)         -> retry_log_008328d8
//   005EBAD5/005EBB13/005EBB82/005EBBCE/005EB7BC/005EB7F4 call 005E5680 (Format)
//   push 74E4F0h/74E4D8h/74E4B8h/74E494h/74E478h      -> retry/null/reopen/short/success
//   005EB6FE/005EB72A push 73D71Ch; 005EB7B6 push 74E470h; 005EB7EE push 740CCCh
// The descriptors below are therefore constant (image-load) publications over
// the single existing owners; no runtime call, no ordering dependency and no
// .CRT$XC* initializer (same treatment as lane-m49-renderer producer.cpp).
//
// 008328D8 storage: single owner is r4-lowers frame_control::log_request_008328d8
// (.bss {data,length}). The original constructs it in static initializer
// 0073B180 (__xc slot 0078D0E0: mov ecx,8328D8h / call 005B3190 / atexit 0073BEF0),
// and 005B3190 only stores 0 to +0 and +4, so loader-zero .bss is the identical
// constructed state. The atexit destructor 0073BEF0 (jmp 005E5C20) stays
// unregistered until 0073B180 is admitted by an allowlist entry (D60/D69
// interim pattern, shutdown-only effect; owned by the cell owner, not here).
// The previous Owner::publish built a SECOND private String for the retry log
// (dual storage vs. MainLoop's log flush 00603FD1..006040C6, which reads the same
// 008328D8 object); this replacement removes that duplicate.
//
// Retired (unreferenced in the M48 link): file_engine_owner::Owner::publish,
// identity, retire_after_drain, ~Owner. The header still declares the class.
#include "c472ae1e53-file_engine_owner.h"
#include "85003406ff-modern_boundaries.h"
#include "e31b02c09e-allocator_live.h"
#include "8ac99a3dae-native_strings.h"
#include "727f959748-warning_runtime.h"
#include <cstdint>
#include <type_traits>

namespace kotor_menu_10_19::dependencies { extern void* base_007a39e0; }
// Single owner: r4-lowers r4_cells.cpp (declared identically by frame-gui suffix.h).
namespace frame_control { extern std::uint32_t log_request_008328d8[2]; }

namespace file_engine_owner {
namespace {
using String = kotor_reconstruction::StartupString;
#include "63ac67d050-original-literals.h"

// Transport adapters onto the canonical owners of 005E5C50 / 005E5670 (same
// bodies as m39-descriptor-proposal's anonymous adapters; no address credit).
// Typed functions of the descriptor slot type; no function-pointer casts.
String* __fastcall assign_005e5c50(String* self, void*, const String* rhs) {
    return self->Assign005e5c50(const_cast<String*>(rhs));
}
const char* __fastcall text_005e5670(String* self, void*) {
    return WindowCStr005e5670(self, nullptr);
}

// Object-pointer view (D24: object-pointer casts acceptable) of the existing
// 8-byte CExoString cell; precedent frame-gui active_log.cpp. Address constant.
String* const retry_log_008328d8 =
    reinterpret_cast<String*>(frame_control::log_request_008328d8);

const file_internal::EngineBindings engine_bindings = {
    &live_operator_new,                                   // 006FA7E6 (005E68EB, 005EBA02)
    &assign_005e5c50,                                     // 005E5C50 (005EB890, 005EB8B9)
    &text_005e5670,                                       // 005E5670 (005EB8CE, 005EB8D6)
    &path_resolution::ResolveActual,                      // 005E68A0 (005EB8AC)
    &kotor_menu_10_19::dependencies::base_007a39e0,       // [007A39E0] (005EB89E)
    retry_log_008328d8,                                   // 008328D8 (005EBAAD..005EBBC9)
    &kotor_reconstruction::StartupFormat005e5680,         // 005E5680
    retry_format,                                         // 0074E4F0 (005EBACB)
    null_format,                                          // 0074E4D8 (005EBB09)
    reopen_format,                                        // 0074E4B8 (005EBB78)
    short_format,                                         // 0074E494 (005EBBAA)
    success_format,                                       // 0074E478 (005EBBC4)
};

const path_resolution::Bindings resolver_bindings = {
    &kotor_menu_10_19::dependencies::base_007a39e0,       // [007A39E0] (005EB70C, 005EB763, ...)
    &kotor_reconstruction::StartupFormat005e5680,         // 005E5680 (005EB7BC, 005EB7F4)
    base_ini_reconstruction::warning_empty_0073d71c,      // 0073D71C (005EB6FE, 005EB72A)
    with_extension,                                       // 0074E470 (005EB7B6)
    without_extension,                                    // 00740CCC (005EB7EE)
};
}
}

// Sole definitions of the two descriptor cells (names/types unchanged).
namespace file_internal { const EngineBindings* engine = &file_engine_owner::engine_bindings; }
namespace path_resolution { const Bindings* bindings = &file_engine_owner::resolver_bindings; }

static_assert(std::is_same<decltype(kotor_menu_10_19::dependencies::base_007a39e0), void*>::value,
              "borrow sole existing Base cell");
static_assert(sizeof(kotor_reconstruction::StartupString) == sizeof(frame_control::log_request_008328d8),
              "008328D8 CExoString {data,length}");
