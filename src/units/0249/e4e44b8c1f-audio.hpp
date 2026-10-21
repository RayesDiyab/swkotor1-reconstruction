#pragma once
#include <stdint.h>
namespace audio {
using U=uint32_t;
struct String { U data,length; };
struct Ini { U internal; };
struct Manager { U reserved; void* client; };
struct Providers {
 U (__fastcall *ini_ctor)(Ini*,void*);
 U (__fastcall *ini_read)(Ini*,void*,String*,String*,String*,String*);
 U (__fastcall *ini_write)(Ini*,void*,String*,String*,String*,String*);
 U (__fastcall *ini_dtor)(Ini*,void*);
 String* (__fastcall *string_default)(String*,void*);
 String* (__fastcall *string_text)(String*,void*,const char*);
 U (__fastcall *string_dtor)(String*,void*);
 U (__fastcall *as_int)(String*,void*);
 void* as_float_st0;
 U (__cdecl *format)(String*,const char*,...);
 void* (__cdecl *allocate)(U);
 void* (__fastcall *sound_ctor)(void*,void*,U,U,U,U);
 U (__fastcall *set_nonstream)(void*,void*,U);
 U (__fastcall *set_stream)(void*,void*,U);
 U (__fastcall *set_bias)(void*,void*,float);
 U (__fastcall *get_eax)(void*,void*);
 void* get_bias_st0;
 U (__fastcall *get_2d)(void*,void*);
 U (__fastcall *get_3d)(void*,void*);
 unsigned char* (__fastcall *settings)(void*,void*);
 Manager** manager_slot;
 void** sound_slot;
};
extern Providers providers;
static_assert(sizeof(void*)==4 && sizeof(String)==8 && sizeof(Ini)==4,"x86 layout");
// EAX is the final INI destructor's raw result. StartServices ignores it.
extern "C" U __stdcall audio_005f1f70();
}
