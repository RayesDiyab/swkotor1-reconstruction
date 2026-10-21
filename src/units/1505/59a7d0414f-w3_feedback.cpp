#include "ee8588f9b9-w3_feedback.h"
#include "3df78882af-startup_string.h" // pinned existing selected StartupString contract, never copied/redeclared here.
#include <cstring>
namespace {
void* internal(void* app){void* p;std::memcpy(&p,static_cast<unsigned char*>(app)+4,sizeof p);return p;}
}
// 005ED360..005ED406 RET0Ch. Empty strings use a new local CExoString; nonempty prefixes are forwarded.
extern "C" void __fastcall recovered_client_add_feedback_message_005ed360(
 void* app,void*,std::uint32_t selector,void* data,void* text){
 if(static_cast<kotor_reconstruction::StartupString*>(text)->BorrowedEquals005e5290("")!=0){
  alignas(kotor_reconstruction::StartupString) unsigned char empty[sizeof(kotor_reconstruction::StartupString)];
  auto* carrier=reinterpret_cast<frame_log::String*>(empty);
  frame_LogStringCtor_005e5a90(carrier,nullptr,"");
  try{feedback_add_005fcd10(internal(app),nullptr,selector,data,empty);}
  catch(...){frame_LogStringDtor_005e5c20(carrier,nullptr);throw;}
  frame_LogStringDtor_005e5c20(carrier,nullptr);
 }else{
  feedback_add_005fcd10(internal(app),nullptr,selector,data,text);
 }
}

