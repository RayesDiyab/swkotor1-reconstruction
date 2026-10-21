#pragma once
#include "9f8b86cc9d-startup_string.h"
#include <cstdint>

// Existing image-loader transport. The receiver must designate the actual
// selected base_ini_reconstruction::Base and its live BaseInternal member.
extern "C" kotor_reconstruction::StartupString* __fastcall c2da_base_005e6660(
    void* receiver, void* unused_edx, std::uint32_t type_word);
