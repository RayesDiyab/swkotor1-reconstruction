#include "c92269d982-read_callback.hpp"

namespace font_callback_read {
namespace {
// This modern C++ scope guard models the completed-temporary cleanup only.
// It is not the original x86 registration, FuncInfo, runtime or SEH handler.
struct TemporaryCleanup {
 String* value;
 explicit TemporaryCleanup(String* object) noexcept:value(object) {}
 TemporaryCleanup(const TemporaryCleanup&)=delete;
 TemporaryCleanup& operator=(const TemporaryCleanup&)=delete;
 void destroy() noexcept {
  String* object=value;value=nullptr; // Disarm before the explicit destructor.
  if(object)object->~String();
 }
 ~TemporaryCleanup() noexcept {destroy();}
};
static_assert(noexcept(static_cast<String*>(nullptr)->~String()),
              "This bounded C++ cleanup contract requires a nonthrowing String destructor");
Signed signed_bits(Word value) {return value&0x80000000u?-1-static_cast<Signed>(~value):static_cast<Signed>(value);}
bool equal_four(const char* a,const char* b) {
 for(Word i=0;i<4;++i)if(a[i]!=b[i])return false;
 return true;
}
char* generic_data(void* resource,Signed* raw_size) {
 const Word size=data_size_007100a0(resource,nullptr);





 *raw_size=signed_bits(size);
 return data_pointer_007107b0(resource,nullptr);

}
}
// Review T-D1: renamed/retyped from read_0070c920 (cell type of 007B8FC4,
// 0070D9C7). Body unchanged; char* data converts implicitly to void*.
extern "C" void* __cdecl aurora_get_resource_0070c920(const char* filename,const char* extension,Signed* raw_size) noexcept(false) {
 // Actual Task11 String constructor is the first dependency call, even for
 // invalid extension/output. Its real destructor runs on each normal return.
 String name;
 if(!extension || !raw_size)return nullptr;

 if(*extension=='.')++extension; // Exactly one leading dot.

 char kind[256];
 format_006fadb0(kind,"%s",extension); // Preserve original unbounded contract.
 uppercase_007136f3(kind);
 assign_text_005e5140(&name,nullptr,static_cast<const char*>(filename));
 alignas(String) unsigned char temporary_storage[sizeof(String)];
 String* lowered=lower_construct_005e6080(&name,nullptr,temporary_storage);
#ifndef READ_OMIT_TEMP_UNWIND
 TemporaryCleanup cleanup(static_cast<String*>(static_cast<void*>(temporary_storage)));
#endif
 assign_string_005e5c50(&name,nullptr,lowered);
#ifndef READ_OMIT_TEMP_UNWIND
 cleanup.destroy();
#else
 // Valid omission mutant: reproduce the old gap only on assignment unwind.
 static_cast<String*>(static_cast<void*>(temporary_storage))->~String();
#endif

 if(equal_four(kind,"MDL")) {
  Model* resource=model_0070e080(&name,0,nullptr,nullptr);
  if(!resource)return nullptr;
  return generic_data(resource,raw_size);
 }
 if(equal_four(kind,"MDX")) {
  ModelX* resource=modelx_0070eb50(&name);
  if(!resource)return nullptr;
  return generic_data(resource,raw_size);
 }
 if(equal_four(kind,"TXI")) {
  Txi* resource=txi_0070ea90(&name);
  if(resource)return generic_data(resource,raw_size);
  ProcessedTexture* fallback=processed_0070ea30(&name);
  if(!fallback)return nullptr;
  const Word size=txi_size_raw_0070f410(fallback,nullptr);



  *raw_size=signed_bits(~size);

  return txi_pointer_raw_0070f3e0(fallback,nullptr);
 }
 if(equal_four(kind,"TGA")) {
  Texture* resource=texture_0070e8b0(&name);
  if(!resource)return nullptr;
  const Word size=txi_size_raw_0070f410(resource,nullptr);
  *raw_size=signed_bits(size);
  return txi_pointer_raw_0070f3e0(resource,nullptr);
 }
 if(equal_four(kind,"TPC")) {
  ProcessedTexture* resource=processed_0070ea30(&name);
  if(!resource)return nullptr;
  const Word size=processed_size_0070f3b0(resource,nullptr);
  *raw_size=signed_bits(size);
  return processed_pointer_0070f380(resource,nullptr);
 }
 if(equal_four(kind,"VIS")) {
  Visibility* resource=visibility_0070eaf0(&name);
  if(!resource)return nullptr;
  return generic_data(resource,raw_size);
 }
 return nullptr;
}
}
