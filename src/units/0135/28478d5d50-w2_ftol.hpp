#pragma once
// __ftol2 006FAE8C as W2 logic sees it: double in, low dword of the int64 truncation out (callers read EAX).
// Native: src/w2_ftol_native.cpp loads the value into ST(0) and calls the existing provider
// _genuine_x87_ftol_006fae8c (0206-renderer-x87-ftol-alias). Host tests link tests/ftol_host.cpp, a model of
// the same contract (truncate; NaN / out of int64 range -> 8000000000000000h -> low dword 0).
#include <cstdint>
namespace w2 { std::uint32_t ftol2_006fae8c(double value); }
