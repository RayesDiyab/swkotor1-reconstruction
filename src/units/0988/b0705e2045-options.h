#pragma once
#include "231c08022a-contracts.h"
namespace options_video {
using IniEntry=I32 (__fastcall*)(Ini*,void*,String*,String*,String*,String*);
struct OptionsBindings {
    DefaultString default_string_005b3190;
    TextString text_string_005e5a90;
    DestroyString destroy_string_005e5c20;
    ConstructIni construct_ini_005e6750;
    DestroyIni destroy_ini_005e67e0;
    IniEntry read_005e67c0,write_005e67d0;
    I32 (__fastcall* as_int_005e5630)(String*,void*);
    void (__cdecl* format_005e5680)(String*,const char*,...);
    void (__fastcall* hardware_mouse_0061d670)(void*,void*,I32);
    const char* tooltip_key_0074e880;
    const char* game_category_0073d658;
    const char* ini_file_0073d648;
    const char* one_00741e1c;
    const char* hardware_key_0074e86c;
    const char* graphics_category_0073d6ac;
    const char* integer_format_0073d720;
};
extern OptionsBindings& options_bindings;
}
