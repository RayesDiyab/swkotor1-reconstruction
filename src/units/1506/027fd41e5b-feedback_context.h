#pragma once
#include "3df78882af-startup_string.h"
#include <cstdint>
#include <cstring>
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*,const char*,...); }
namespace feedback {
using U=std::uint32_t;
using I=std::int32_t;
using String=kotor_reconstruction::StartupString;
template<class T> inline T read(const void* object,U offset) {
    T value; std::memcpy(&value,static_cast<const unsigned char*>(object)+offset,sizeof value); return value;
}
template<class T> inline void write(void* object,U offset,T value) {
    std::memcpy(static_cast<unsigned char*>(object)+offset,&value,sizeof value);
}
// Raw hidden-return storage: no extra CExoString constructor. Arm only after the callee returns.
struct ReturnedString {
    alignas(4) unsigned char bytes[8];
    bool live=false;
    String* storage() { return reinterpret_cast<String*>(bytes); }
    String* accept(String* result) { live=true; return result; }
    ~ReturnedString() { if(live) storage()->~String(); }
};
struct Context {
    void* self;
    void* data;
    std::uint16_t selector;
    void* player;
    String& object_name;
    String& output;
};
String* lookup(Context& c,U reference,ReturnedString& temporary);
String* fetch(U reference,void* context,ReturnedString& temporary,U unused=0,U flags=0);
void token(U index,String* value);
U integer(Context& c,I index);
U object(Context& c,I index);
U resolve_name(Context& c,U object_id,String* output);
String* message_string(Context& c,I index,ReturnedString& temporary);
String* from_integer(I value,ReturnedString& temporary);
const char* cstr(String* value);
template<class... Args> inline void format(String* output,const char* specification,Args... args) {
    kotor_reconstruction::StartupFormat005e5680(output,specification,args...);
}
// true follows006021F8 (prefix/queue); false follows00602251 (message disposal only).
bool case62(Context& c);
bool case160_165(Context& c);
}
extern "C" void __fastcall feedback_add_005fcd10(void*,void*,std::uint32_t,void*,void*);
