#include "11dd27c2b8-baseinternal.h"
#include "0e4d19f25c-baseinternal_storage.h"
#include "9e11ed2f44-allocator_live.h"
#include "2aee45ecb6-string_priority.h"
#include <new>
#include <cstring>
namespace base_resource_table {
void __fastcall CreateResourceExtensionTable005e6d20(Internal* self,void*) {
 self->extension_count=88;
 void* ids_storage=live_operator_new(176);



 const std::uint32_t count=self->extension_count; // original reread before ID publication




 auto* ids=BeginIdStorage(ids_storage); // after original live-count reread

 self->extension_ids=ids;
 const std::uint32_t bytes=count*8u+4u; // original DWORD wrap, no checked sizing
 void* array=live_operator_new(bytes);
 String* elements=nullptr;
 if(array) {



  const std::uint32_t cookie=count;

  std::memcpy(array,&cookie,4);
  elements=StringElementsStorage(array);
  ConstructStringVectorNormal(elements,8,static_cast<std::int32_t>(count),&ConstructString,&DestroyString);
 }
 // Load the live ID pointer after allocation/constructor effects, then publish
 // String pointer. Pair stores below always use the current receiver pointers.
 std::uint16_t* first_ids=self->extension_ids;
 self->extension_strings=elements;
 first_ids[0]=resource_pairs[0].id;
 (void)GuiAssign005e5140(LiveStringElement(self->extension_strings,0),nullptr,resource_pairs[0].literal);



 for(std::uint32_t i=1;i<88;++i) {






  auto* current_ids=self->extension_ids;




  current_ids[i]=resource_pairs[i].id;


  // Original ID store is complete before the current String pointer is loaded.
  auto* current_strings=self->extension_strings;

  (void)GuiAssign005e5140(LiveStringElement(current_strings,i),nullptr,resource_pairs[i].literal);
 }
}
Internal* __fastcall ConstructBaseInternal005e8ba0(Internal* self,void*) {
 auto* live=::new(static_cast<void*>(self)) Internal;
 CreateResourceExtensionTable005e6d20(live,nullptr);



 return live;

}
}
