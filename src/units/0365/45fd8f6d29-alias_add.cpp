#include "6431ecf474-alias_add.h"
#include "9e11ed2f44-allocator_live.h"
#include "4f178974e9-native_strings.h"
#include "2aee45ecb6-string_priority.h"
#include <new>
namespace alias_add {
void __fastcall AddInternal005ebf30(Internal* self,void*,String* key_input,String* path_input) {
 String key(*key_input),path(*path_input);
 if(path.BorrowedEquals005e5290(dependencies.empty_0073d71c)!=0)return;
 alignas(String) unsigned char upper_storage[sizeof(String)];
 String* upper=Upper005e6360(&key,nullptr,reinterpret_cast<String*>(upper_storage));
 key.Assign005e5c50(upper);upper->~StartupString();
 const char* characters=WindowCStr005e5670(&path,nullptr);
 const std::int32_t length=path.BorrowedLength005e5790();
 if(characters[length-1]!='\\') {
  String slash(dependencies.slash_0073d8f4);
  alignas(String) unsigned char output_storage[sizeof(String)];
  String* output=GuiConcat005e5d10(&path,nullptr,reinterpret_cast<String*>(output_storage),&slash);
  path.Assign005e5c50(output);output->~StartupString();
 }
 Node* next=self->head;Node* previous=nullptr;
 while(next) {



  if(Greater005e5490(&next->key,nullptr,&key)!=0)break;

  previous=next;next=next->next;
 }
 void* storage=live_operator_new(24);
 Node* created=storage?ConstructNode005eb600(static_cast<Node*>(storage),nullptr):nullptr;
 // Original continues into assignment on null allocation; no invented refusal.
 created->key.Assign005e5c50(&key);created->path.Assign005e5c50(&path);
 if(self->head==next)self->head=created;
 created->previous=previous;created->next=next;
 if(previous)previous->next=created;

 if(next)next->previous=created;

}
void __fastcall Add005e6880(List* self,void*,String* key,String* path) {
 AddInternal005ebf30(self->internal,nullptr,key,path);
}
void __fastcall AddAlias005e7760(BaseInternal*,void*,String* filename,String* key,String* relative,String*) {
 String selected;
 int result;
 {
  String section(dependencies.section_0074ddb0);
  Base* base=static_cast<Base*>(*dependencies.base_007a39e0);
  result=dependencies.read_005e67c0(static_cast<PublicIni*>(base->ini),nullptr,&selected,filename,&section,key);
 }



 const bool use_default=result==0;

 // Original destroys section before fallback assignment.
 if(use_default)selected.Assign005e5c50(relative);
 // Reload the actual global cell after opaque read/destruction/assignment.
 Base* base=static_cast<Base*>(*dependencies.base_007a39e0);
 Add005e6880(static_cast<List*>(base->aliases),nullptr,key,&selected);
}
}
