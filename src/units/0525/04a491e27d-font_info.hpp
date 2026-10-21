#pragma once
#include "f31f0dbce3-texture.hpp" // E1 epoch
namespace font_info_producer {
using U=texture::U;using FontInfo=texture::FontInfo;
static_assert(sizeof(FontInfo)==48,"existing partial FontInfo carrier only");
// Caller supplies aligned live storage. This starts the reused record lifetime;
// it does not establish the absent nominal Vector/List or full GUI-string join.
extern "C" FontInfo* __fastcall construct_004221c0(void*,void*);
}
