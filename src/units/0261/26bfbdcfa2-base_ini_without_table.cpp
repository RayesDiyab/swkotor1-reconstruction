#include "d2e9f84747-base_ini.h"
#include <cstring>
#include "06d9c3a3bc-alias_table.h"
namespace base_ini_reconstruction {
Base* Base::construct_005e6500() {
 void* p=bindings.allocate_006fa7e6(16);debug=p?bindings.debug_005e9370(static_cast<kotor_menu20::Debug*>(p),nullptr):nullptr;
 p=bindings.allocate_006fa7e6(4);timers=p?bindings.timers_005e91e0(static_cast<kotor_startup::Timers*>(p),nullptr):nullptr;
 p=bindings.allocate_006fa7e6(4);aliases=p?bindings.aliases_005e6810(static_cast<alias_path::AliasListView*>(p),nullptr):nullptr;
 p=bindings.allocate_006fa7e6(4);ini=p?((::new(p) PublicIni)->*bindings.ini_005e6750)():nullptr;
 p=bindings.allocate_006fa7e6(4);random=p?bindings.random_005ea420(static_cast<startup_rand::Rand*>(p),nullptr):nullptr;
 p=bindings.allocate_006fa7e6(12);internal=p?bindings.internal_005e8ba0(static_cast<BaseInternal*>(p),nullptr):nullptr;
 return this;
}
void BaseInternal::load_aliases_005e7a90(String* input) {
 String filename; //005B3190 once
 bindings.format_string_005e5680(&filename,".\\%s",bindings.c_str_005e5670(input,nullptr));
 for(const auto& e:alias_table) {
  String fallback(e.fallback),relative(e.relative),key(e.key);
  bindings.add_alias_005e7760(this,nullptr,&filename,&key,&relative,&fallback);
  // Destruction key/relative/fallback reverses the original third-to-first
  // construction order. Original repeated MUSIC/CURRENTGAME calls are retained.
 }
}
Ini* Ini::construct_005ea540() {
 (sections.*bindings.list_005ea4d0)();
 ::new (filename_storage) String; // accepted005B3190 source, exact same storage
 bindings.assign_cstr_005e5140(filename(),nullptr,""); // literal0073D71C
 return this;
}
int Ini::read_005ea6c0(String* output,String* requested_file,String* category,String* key) {
 // Original fixed buffers and original unchecked copy domain. Fixtures are
 // <=95-byte names and <=511-byte terminated stored lines. No overflow claim.
 char file[96],section_name[96],key_name[96],line[512];
 bindings.sprintf_006fadb0(file,"%s",bindings.c_str_005e5670(requested_file,nullptr));
 bindings.sprintf_006fadb0(section_name,"%s",bindings.c_str_005e5670(category,nullptr));
 bindings.sprintf_006fadb0(key_name,"%s",bindings.c_str_005e5670(key,nullptr));
 if(!bindings.compare_nc_005e58c0(filename(),nullptr,file))bindings.destroy_005ea650(this,nullptr);
 Node* position=static_cast<Node*>(sections.internal->head);
 if(!position || !position->data) {
  (filename()->*bindings.assign_string_005e5c50)(requested_file); // before open/failure
  void* stream=bindings.open_006fad9d(file,"r"); // literal00740CA4; text mode
  if(!stream)return 0;
  if(bindings.gets_006fc469(line,512,stream)) {
   do {
    void* memory=bindings.allocate_006fa7e6(12);
    Section* section=memory?bindings.section_005ea600(static_cast<Section*>(memory),nullptr):nullptr;
    // Original does not recover from allocation/constructor-null here.
    bindings.assign_cstr_005e5140(section->name(),nullptr,line);
    bindings.add_tail_005e9dc0(sections.internal,nullptr,section);
    if(line[0]=='[' && bindings.gets_006fc469(line,512,stream)) {
     while(line[0]!='[') {
      void* item=bindings.allocate_006fa7e6(8);
      String* entry=item?::new(item) String:nullptr;
      bindings.assign_cstr_005e5140(entry,nullptr,line);
      bindings.add_tail_005e9dc0(section->lines.internal,nullptr,entry);
      if(!bindings.gets_006fc469(line,512,stream))goto section_complete;
     }
     //005EA89D..8BB: signed two's-complement -1-strlen, whence1.
     const auto length=static_cast<std::uint32_t>(std::strlen(line));
     bindings.seek_006fbb19(stream,static_cast<std::int32_t>(0xffffffffu-length),1);
    }
section_complete:;
   } while(bindings.gets_006fc469(line,512,stream));
  }
  bindings.close_006facf0(stream); // return ignored as original
 }
 position=static_cast<Node*>(sections.internal->head);
 Section* section=position?static_cast<Section*>(position->data):nullptr;
 while(section) {
  std::strcpy(line,bindings.c_str_005e5670(section->name(),nullptr));
  char* end=bindings.chr_006fb5e0(line,']');if(end)*end=0;
  char* begin=bindings.chr_006fb5e0(line,'[');if(!begin)begin=line;
  ++begin; // original skips first byte even if no '[' exists
  if(!std::strcmp(begin,section_name))break; // original bytewise equality
  section=static_cast<Section*>(bindings.next_005e9b90(sections.internal,nullptr,&position));
 }
 if(!section)return 0;
 position=static_cast<Node*>(section->lines.internal->head);
 String* entry=position?static_cast<String*>(position->data):nullptr;
 while(entry) {
  std::strcpy(line,bindings.c_str_005e5670(entry,nullptr));
  char* equal=bindings.chr_006fb5e0(line,'=');if(equal)*equal=0;
  if(!std::strcmp(line,key_name))break;
  entry=static_cast<String*>(bindings.next_005e9b90(section->lines.internal,nullptr,&position));
 }
 if(!entry)return 0;
 // A matched line without '=' has the original invalid-pointer path; excluded
 // from this valid-line contract, never converted to invented successful data.
 const char* value=bindings.chr_006fb5e0(bindings.c_str_005e5670(entry,nullptr),'=')+1;
 bindings.assign_cstr_005e5140(output,nullptr,value);
 alignas(String) unsigned char temporary[8];
 auto* right=bindings.right_005e6170(output,nullptr,reinterpret_cast<String*>(temporary),1);
 const int newline=(right->*bindings.equals_005e5290)("\n");
 right->~String(); // accepted005E5C20 source
 if(newline) {
  alignas(String) unsigned char prefix[8];
  auto* left=bindings.left_005e5f70(output,nullptr,reinterpret_cast<String*>(prefix),(output->*bindings.length_005e5790)()-1);
  (output->*bindings.assign_string_005e5c50)(left);
  left->~String();
 }
 return 1;
}
}
