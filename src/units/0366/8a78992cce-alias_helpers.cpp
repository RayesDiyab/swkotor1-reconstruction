#include "6431ecf474-alias_add.h"
#include "9e11ed2f44-allocator_live.h"
#include <new>
namespace alias_add {
int __fastcall Greater005e5490(String* self,void*,String* right) {
 const unsigned char* left=reinterpret_cast<const unsigned char*>(self->data);
 if(!left)return 0;
 const unsigned char* other=reinterpret_cast<const unsigned char*>(right->data);
 if(!other)return *left!=0?1:0;
 for(;;) {
  const unsigned char a=*left,b=*other;
  if(a!=b)return a>b?1:0;
  if(!a)return 0;
  ++left;++other;
 }
}
String* __fastcall Upper005e6360(String* self,void*,String* output) {
 // Actual String lifetime on fresh private return-temporary storage. Original
 // normal completion releases cached allocation, not its mutable data field.
 alignas(String) unsigned char storage[sizeof(String)];
 String* temporary=::new(static_cast<void*>(storage)) String;
 char* allocation=nullptr;
 if(self->data) {
  const volatile unsigned char* initial=reinterpret_cast<const volatile unsigned char*>(self->data);
  const volatile unsigned char* end=initial;while(*end)++end;
  const std::uint32_t count=static_cast<std::uint32_t>(end-initial)+1u;
  temporary->capacity=count;allocation=static_cast<char*>(live_operator_new(count));temporary->data=allocation;
  std::uint32_t i=0;
  while(self->data[i]) {
   const unsigned char c=static_cast<unsigned char>(self->data[i]);



   allocation[i]=static_cast<char>(c>=97&&c<=122?c-32:c);

   ++i;
  }
  allocation[i]=0;
 }
 String* result=::new(static_cast<void*>(output)) String(*temporary);
 if(allocation)live_free(allocation);
 // No implicit temporary destructor: storage release ends this placement
 // lifetime after its original cached allocation was released exactly once.
 return result;
}
Node* __fastcall ConstructNode005eb600(Node* storage,void*) {
 // Genuine68-byte body constructs two String members; link words are untouched.
 return ::new(static_cast<void*>(storage)) Node;
}
}
