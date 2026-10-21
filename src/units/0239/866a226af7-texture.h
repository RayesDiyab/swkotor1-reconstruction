#pragma once
#include "b990a7ddb1-contracts.h"
namespace options_video {
struct TextureBindings {
    DefaultString default_string_005b3190;TextString text_string_005e5a90;DestroyString destroy_string_005e5c20;
    void* const volatile* app_007a3a28;
    void* (__cdecl* allocate_006fa7e6)(U32);
    ResourceRef* (__fastcall* resource_ref_00406d80)(ResourceRef*,void*,const char*);
    Table* (__fastcall* construct_table_00413cc0)(Table*,void*,ResourceRef,I32);
    I32 (__fastcall* load_table_004143b0)(Table*,void*);
    void (__fastcall* unload_table_004139e0)(Table*,void*);
    I32 (__fastcall* string_entry_00413270)(Table*,void*,I32,String*,String*);
    I32 (__fastcall* float_entry_00413430)(Table*,void*,I32,String*,float*);
    I32 (__fastcall* int_entry_00413660)(Table*,void*,I32,String*,I32*);
    void (__stdcall* reset_textures_00421d50)();
    String* (__fastcall* concatenate_005e5d10)(String*,void*,String*,String*);
    U32 (__cdecl* set_pack_0070d800)(U8,String*);
    void (__cdecl* dynamic_ratio_0041e9b0)(float);
    U8 (__cdecl* texture_memory_00421d90)(I32);
    const char* table_resource_0074e860;
    const char* texture_column_0074e858;
    const char* gui_column_0074e854;
    const char* ratio_column_0074e848;
    const char* memory_column_0074e844;
    const char* pack_prefix_0074e834;
};
extern TextureBindings& texture_bindings;
}
