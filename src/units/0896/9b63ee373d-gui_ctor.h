#pragma once
#include <cstdint>
#include <cstring>
namespace gui_ctor {
using Word=std::uint32_t;
struct Gui {
 alignas(4) unsigned char bytes[0xa8];
 __declspec(noinline) Gui* __thiscall construct_0040bad0();
 void __thiscall Update_0040ce70(float);
 void __thiscall Draw_0040cc50(float);
 // Same physical +60 float bits, accessed through actual object representation.
 float saved_delta() const {float value;std::memcpy(&value,bytes+0x60,sizeof(value));return value;}
 void saved_delta(float value) {std::memcpy(bytes+0x60,&value,sizeof(value));}
};
using Default=Word (__thiscall*)(void*);
struct Bindings {
 Default ref_00405ed0,string_005b3190;
 Word (__thiscall* list_00416770)(void*,Word);
 Word (__thiscall* assign_text_005e5140)(void*,const char*);
 Word (__thiscall* concat_005e5d10)(void*,void*,const void*);
 Word (__thiscall* assign_string_005e5c50)(void*,const void*);
 void (__thiscall* destroy_string_005e5c20)(void*);
 Word (__thiscall* assign_ref_00406290)(void*,void*,const char*);
 void* (__cdecl* allocate_006fa7e6)(Word);
 Word (__thiscall* auxiliary_006277c0)(void*,Gui*);
 Word (__thiscall* options_005ed700)(void*);
 Word* gui_007a39f4;const Word* manager_007a39fc;
 const char* cursor_0073df7c;const void* suffix_007a3d50;const char* empty_0073d71c;
};
extern Bindings bindings;
static_assert(sizeof(void*)==4&&sizeof(Gui)==0xa8,"original GUI receiver extent");
}
