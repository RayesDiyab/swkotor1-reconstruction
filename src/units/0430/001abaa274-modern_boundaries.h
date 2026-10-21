#pragma once
#include "897ca13cc0-internal.h"
namespace tlk_host {
// Called after normal modern CRT startup; does not initialize or publish state.
bool FileBoundaryReady() noexcept;
const file_internal::FileOps& FileBoundary();
tlk_file::Bindings MakeFileBindings(const char* binary_mode_0073d5b0);
// Main6 supplies actual resolver/Base cell/log String/literal lifetimes.
// This fills only existing genuine code/allocator/formatter entries.
file_internal::EngineBindings MakeFileEngineBindings(file_internal::EngineBindings required_state);
}
