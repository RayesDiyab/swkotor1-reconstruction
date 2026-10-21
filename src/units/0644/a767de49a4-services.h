#pragma once
#include <stdint.h>
#include <string.h>
namespace client_services {
using Word=uint32_t;
// Native thiscall callees are typed in the ABI-identical register form:
// ECX receiver, ignored EDX, remaining words on the stack, callee cleanup.
// (Integrator decision: same pattern as base_ini Bindings.) Callers pass
// nullptr for the unused EDX word.
using Constructor=void* (__fastcall*)(void*,void*);
struct String { char* text; Word length; };
struct Dependencies {
 void* (__cdecl* allocate_006fa7e6)(Word);
 Word (__stdcall* tick_0073d0f8)();
 Constructor stats_004010c0,object_table_004ae5d0,internal_005fbfe0,rules_00552c50;
 void* (__fastcall* load_screen_0067a710)(void*,void*,void*);
 String* (__fastcall* string_005e5a90)(String*,void*,const char*);
 void (__fastcall* destroy_string_005e5c20)(String*,void*);
 int (__fastcall* get_int_00413660)(void*,void*,int,String*,int*);
 void (__fastcall* gui_initialize_00632720)(void*,void*);
 void (__fastcall* event_descriptions_005eeb10)(void*,void*);
 int (__fastcall* movie_playing_0070c1f0)(void*,void*);
 void (__fastcall* unpauseable_0062b410)(void*,void*,Word);
 void (__fastcall* clear_events_005df4e0)(void*,void*);
 void (__fastcall* sound_mode_005d5e80)(void*,void*,Word);
 void (__fastcall* input_active_005df540)(void*,void*,int);
 void (__fastcall* keyboard_off_005f2980)(void*,void*);
 void (__fastcall* show_mouse_0061fa00)(void*,void*,int);
 // Borrowed singleton providers. No original game-global storage is defined.
 void* volatile* rules_007a3a28;
 void* volatile* input_007a39e4;
 void* volatile* sound_007a39ec;
 const Word* sound_mode_0074c5f8;
};
extern Dependencies dependencies;
template<unsigned Size> struct Image {
 alignas(4) unsigned char bytes[Size];
 Word word(unsigned offset) const {Word v;memcpy(&v,bytes+offset,4);return v;}
 void word(unsigned offset,Word v){memcpy(bytes+offset,&v,4);}
 void* pointer(unsigned offset) const {return reinterpret_cast<void*>(word(offset));}
};
struct Internal:Image<0x4d4> {
 Word initialize_005f51c0();
 void input_005f6b10(int active,int clear);
 // Unresolved full body; preserves five-word thiscall interface.
 Word start_services_005f8550(Word,Word,Word,Word,Word);
};
struct Client:Image<8> {
 Client* construct_005ee2f0();
 Word initialize_005ed7b0();
 void input_005eda10(int active,int clear);
 Word start_services_005ed860(Word first,Word second,Word third,Word fourth,Word fifth);
};
struct Manager:Image<0x1c> { Manager* construct_00401160(); };
static_assert(sizeof(void*)==4 && sizeof(String)==8,"original x86 words");
static_assert(sizeof(Client)==8 && sizeof(Manager)==0x1c && sizeof(Internal)==0x4d4,"observed object extents");
static_assert(sizeof(Dependencies)==92,"23 x86 slots unchanged");
}
