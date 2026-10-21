#include "fe26a93915-lower.h"
#include <new>
#include <cstring>
extern "C" void* __fastcall unresolved_keylist_ctor_00406cf0(void* self,void*) {






 const unsigned bytes=12;

 auto* allocation=static_cast<kotor_list::Internal*>(live_operator_new(bytes));
 kotor_list::Internal* result=nullptr;
 if(allocation)result=(::new(allocation) kotor_list::Internal)->Construct();



 std::memcpy(self,&result,4);return self;

}
namespace {
// Raw storage; the owner copy-constructor join starts the String lifetime.
struct Temporary {
 alignas(menu_resources::String) unsigned char bytes[sizeof(menu_resources::String)];
 menu_resources::String* value(){return std::launder(reinterpret_cast<menu_resources::String*>(bytes));}
 explicit Temporary(const menu_resources::String* input){r8_copy_construct_005e5b00(reinterpret_cast<menu_resources::String*>(bytes),nullptr,input);}
 ~Temporary(){value()->~StartupString();}
};
}
extern "C" menu_resources::String* __stdcall unresolved_string_copy_00406c70(menu_resources::String* output,menu_resources::String* input,menu_resources::Word,menu_resources::Word*) {



 Temporary temporary(input);
 r8_copy_construct_005e5b00(output,nullptr,temporary.value());

 return output;
}
