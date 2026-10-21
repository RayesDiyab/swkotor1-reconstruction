// W7 test bootstrap: performs the same gates the candidate's functional bootstrap (`run`, 0059) performs before
// any game code runs: CRT heap published, LocaleScope::claim (0114), the IniAllocatorBinding publication (0059 local
// class: base_ini_reconstruction::bindings.allocate_006fa7e6 = &live_operator_new; used by CExoLinkedList AddTail
// 005E9DC0 and other INI-era list providers), FormatterScope::claim (0178).
// The scopes are the candidate's own classes and are deliberately never destroyed (WinMain's scopes live for the
// whole session; destroying an active scope terminates by design).
#include "407096299a-modern_locale.h"
#include "65a9b8daf6-canonical_formatter_owner.h"
// server harness v2: the candidate's IniProviderScope (m60 modern-ini-support/ini_lifetime.h, same layout and members;
// declared here to avoid that header's include chain). run() claims it as gate 5 before any CExoBase / ini provider runs.
namespace kotor_functional { class IniProviderScope { bool claimed_ = false; void* borrowed_heap_ = nullptr; public: bool claim() noexcept; ~IniProviderScope(); IniProviderScope() = default; }; }
#include <cstdint>
extern "C" void* __cdecl live_operator_new(std::uint32_t);
namespace base_ini_reconstruction { struct Bindings; extern Bindings bindings; }   // 0056; first member: allocate_006fa7e6
bool w7_claim_formatter() {
  if (!kotor_functional::source_heap_identity()) return false;
  auto* locale = new kotor_functional::LocaleScope();
  if (!locale->claim()) return false;
  auto& allocate = reinterpret_cast<void*(__cdecl*&)(std::uint32_t)>(base_ini_reconstruction::bindings);
  if (allocate) return false;   // IniAllocatorBinding::publish refuses an occupied slot
  allocate = &live_operator_new;
  auto* formatter = new kotor_canonical_string_format::FormatterScope();
  if (!formatter->claim(*locale)) return false;
  auto* ini_callbacks = new kotor_functional::IniProviderScope();   // never destroyed, like run()'s scope for the session
  return ini_callbacks->claim();
}
