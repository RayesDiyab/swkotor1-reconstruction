#pragma once
#include "f166f203af-base_ini.h"
namespace ini_string_join {
// Read support only. Caller/table/INI lifetimes remain with their existing owner.
// Populate a detached candidate or publish only while every consumer is quiescent.
void bind(base_ini_reconstruction::Bindings&) noexcept;
}
