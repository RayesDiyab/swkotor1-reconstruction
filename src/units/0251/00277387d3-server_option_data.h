#pragma once
#include "8af467dac9-startup_views.h"
namespace kotor_menu_10_19::dependencies {
// Process-lifetime x86 pointer words in original table order. One storage owner.
inline constexpr unsigned server_option_word_count = 57;
extern volatile Word server_option_globals_007a1e10[server_option_word_count];
extern const char server_undefined_00748794[10];
extern const char server_undefined_option_00748784[16];
static_assert(sizeof(Word)==4 && sizeof(void*)==4,"original x86 data view");
}
