#pragma once
#include "b6a6f59d48-startup_views.h"
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Approved MSVC x86 source contract required
#endif
// Exact task23 declarations are reused. The raw divmod entry returns additional
// words in EBX:ECX and must only be called by an assembly-aware bridge.
static_assert(sizeof(kotor_menu_10_19::Divmod)==16,"four original DWORD results");
