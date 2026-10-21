#pragma once
#include <cstdint>
#include <cstring>
namespace gui_sounds {
using Word=std::uint32_t;struct Ref{unsigned char bytes[16];};
struct Gui{alignas(4) unsigned char bytes[0xa8];Word __thiscall load_00409f00();};
struct Bindings{
 void* (__cdecl* allocate_006fa7e6)(Word);
 Word (__thiscall* ref_text_00406d80)(void*,const char*);
 Word (__thiscall* table_00413cc0)(void*,Ref,Word);
 Word (__thiscall* table_load_004143b0)(void*);
 Word (__thiscall* string_005b3190)(void*);
 Word (__thiscall* text_005e5a90)(void*,const char*);
 Word (__thiscall* get_00413270)(void*,Word,void*,void*);
 void (__thiscall* destroy_005e5c20)(void*);
 Word (__thiscall* ref_string_00406d60)(void*,void*);
 Word (__thiscall* sound_005d60e0)(void*,void*);
 void (__thiscall* mode_005d5900)(void*,Word);
 const char* table_name_0073df60;const char* column_0073df54;
};extern Bindings bindings;
}
