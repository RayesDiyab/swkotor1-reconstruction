#include "5b33f70db1-ini_write.h"
#include <cstring>
namespace base_ini_reconstruction {
WriteBindings write_bindings{};
static void append(String* destination,const char* text) {
 String right(text);alignas(String) unsigned char storage[8];
 auto* result=write_bindings.concat_005e5d10(destination,nullptr,reinterpret_cast<String*>(storage),&right);
 (destination->*bindings.assign_string_005e5c50)(result);
 result->~String();
} // result destroyed before right, matching observed temporary order
PublicIni* PublicIni::construct_005e6750() {
 auto* p=bindings.allocate_006fa7e6(12);
 if(p){auto* candidate=::new(p) WritableIni;internal=static_cast<WritableIni*>(candidate->construct_005ea540());}
 else internal=nullptr;
 return this;
}
int PublicIni::read_005e67c0(String* value,String* file,String* category,String* key) {
 return internal->read_005ea6c0(value,file,category,key);
}
int PublicIni::write_005e67d0(String* value,String* file,String* category,String* key) {
 return internal->write_005eab00(value,file,category,key);
}
int WritableIni::write_005eab00(String* value,String* requested_file,String* category,String* key) {
 char file[96],section_name[96],key_name[96],value_text[512],line[512];
 bindings.sprintf_006fadb0(file,"%s",bindings.c_str_005e5670(requested_file,nullptr));
 bindings.sprintf_006fadb0(section_name,"%s",bindings.c_str_005e5670(category,nullptr));
 bindings.sprintf_006fadb0(key_name,"%s",bindings.c_str_005e5670(key,nullptr));
 bindings.sprintf_006fadb0(value_text,"%s",bindings.c_str_005e5670(value,nullptr));
 if(!bindings.compare_nc_005e58c0(filename(),nullptr,file))bindings.destroy_005ea650(this,nullptr);
 Node* position=static_cast<Node*>(sections.internal->head);
 if(!position || !position->data) {
  (filename()->*bindings.assign_string_005e5c50)(requested_file);
  void* stream=bindings.open_006fad9d(file,"r");
  if(!stream)stream=bindings.open_006fad9d(file,"w"); // both fail: return0, no mutation phase
  if(!stream)return 0;
  if(bindings.gets_006fc469(line,512,stream)) {
   do {
    void* memory=bindings.allocate_006fa7e6(12);
    auto* section=memory?bindings.section_005ea600(static_cast<Section*>(memory),nullptr):nullptr;
    bindings.assign_cstr_005e5140(section->name(),nullptr,line);
    bindings.add_tail_005e9dc0(sections.internal,nullptr,section);
    if(line[0]=='[' && bindings.gets_006fc469(line,512,stream)) {
     while(line[0]!='[') {
      void* item=bindings.allocate_006fa7e6(8);auto* entry=item?::new(item) String:nullptr;
      bindings.assign_cstr_005e5140(entry,nullptr,line);
      bindings.add_tail_005e9dc0(section->lines.internal,nullptr,entry);
      if(!bindings.gets_006fc469(line,512,stream))goto section_complete;
     }
     const auto length=static_cast<std::uint32_t>(std::strlen(line));
     bindings.seek_006fbb19(stream,static_cast<std::int32_t>(0xffffffffu-length),1);
    }
section_complete:;
   } while(bindings.gets_006fc469(line,512,stream));
  }
  bindings.close_006facf0(stream);
 }
 position=static_cast<Node*>(sections.internal->head);Section* section=position?static_cast<Section*>(position->data):nullptr;
 while(section) {
  std::strcpy(line,bindings.c_str_005e5670(section->name(),nullptr));
  char* end=bindings.chr_006fb5e0(line,']');if(end)*end=0;
  char* begin=bindings.chr_006fb5e0(line,'[');if(!begin)begin=line;++begin;
  if(!std::strcmp(begin,section_name))break;
  section=static_cast<Section*>(bindings.next_005e9b90(sections.internal,nullptr,&position));
 }
 if(section) {
  position=static_cast<Node*>(section->lines.internal->head);auto* entry=position?static_cast<String*>(position->data):nullptr;
  char* equal=nullptr;
  while(entry) {
   std::strcpy(line,bindings.c_str_005e5670(entry,nullptr));equal=bindings.chr_006fb5e0(line,'=');if(equal)*equal=0;
   if(!std::strcmp(line,key_name))break;
   entry=static_cast<String*>(bindings.next_005e9b90(section->lines.internal,nullptr,&position));
  }
  if(entry) {
   if(equal){std::strcpy(equal+1,value_text);*equal='=';}
   else {std::strcat(line,"=");std::strcat(line,value_text);}
   bindings.assign_cstr_005e5140(entry,nullptr,line);
   append(entry,"\n"); // original always adds one, even if value already ends LF
  } else {
   auto* memory=bindings.allocate_006fa7e6(8);auto* added=memory?::new(memory) String:nullptr;
   append(added,key_name);append(added,"=");append(added,value_text);append(added,"\n");
   write_bindings.add_head_005e9d70(section->lines.internal,nullptr,added);
  }
 } else {
  auto* memory=bindings.allocate_006fa7e6(12);section=memory?bindings.section_005ea600(static_cast<Section*>(memory),nullptr):nullptr;
  bindings.assign_cstr_005e5140(section->name(),nullptr,"\n[");
  append(section->name(),section_name);append(section->name(),"]\n");
  auto* item=bindings.allocate_006fa7e6(8);auto* added=item?::new(item) String:nullptr;
  bindings.assign_cstr_005e5140(added,nullptr,key_name);
  append(added,"=");append(added,value_text);append(added,"\n");
  write_bindings.add_head_005e9d70(section->lines.internal,nullptr,added);
  bindings.add_tail_005e9dc0(sections.internal,nullptr,section);
 }
 void* output=bindings.open_006fad9d(file,"w");
 if(!output) {
  String message;
  bindings.format_string_005e5680(&message,"ERROR: %s file is marked read only! Are you sure that you are running from the install location and not from the network?",file);
  bindings.c_str_005e5670(&message,nullptr); // original obtains text but does not report/UI it
  return 0; // cache mutation is retained
 }
 position=static_cast<Node*>(sections.internal->head);section=position?static_cast<Section*>(position->data):nullptr;
 while(section) {
  write_bindings.print_006faf01(output,"%s",bindings.c_str_005e5670(section->name(),nullptr));
  Node* entry_position=static_cast<Node*>(section->lines.internal->head);auto* entry=entry_position?static_cast<String*>(entry_position->data):nullptr;
  while(entry) {
   write_bindings.print_006faf01(output,"%s",bindings.c_str_005e5670(entry,nullptr));
   entry=static_cast<String*>(bindings.next_005e9b90(section->lines.internal,nullptr,&entry_position));
  }
  section=static_cast<Section*>(bindings.next_005e9b90(sections.internal,nullptr,&position));
 }
 bindings.close_006facf0(output); // formatter and close results ignored
 return 1;
}
}
