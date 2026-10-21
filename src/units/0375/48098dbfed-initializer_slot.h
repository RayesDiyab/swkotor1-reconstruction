#pragma once
#include "00dc47def0-renderer_initialize.h"
#include "de6b2458eb-reinit.h"
namespace aurora_context_adoption {
// Transport adapter only. Main6 owns the complete vtable and receiver lifetime.
__declspec(noinline) kotor_renderer_init::I32 __fastcall initialize_slot(
    kotor_renderer_init::Renderer*,void*,void*,void*,
    std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t,
    std::int32_t,std::int32_t,std::int32_t);
std::uint32_t initializer_slot_word() noexcept;
// Handles only the exact current ReInit call; false requires caller routing.
bool dispatch_initializer(const kotor_reinit::Call&,kotor_reinit::Result&);
}
