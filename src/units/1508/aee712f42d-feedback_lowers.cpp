// Original GOG K1 swkotor.exe SHA256 9c10e045...; bounded feedback dependency closure.
#include "5b9aa881dd-feedback_lowers.h"
#include "897d394ced-feedback_token_object.h"
#include "3f0abd0978-tlk_core.h"
#include "8c34f546dd-native_strings.h"
#include "81f59c47ba-string_priority.h"
#include "99e00ff93c-m15b_stats.h"
#include "cca80f7c35-m15e.h"
#include "03e67ad358-window_shared_cells.h"
#include <cstring>
#include <new>

extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern std::uint32_t kotor_rules_cell32_007a2258;
menu_resources::Ref* __fastcall c2da_default_ref_00405ed0(menu_resources::Ref*,void*);
unsigned char __fastcall kotor_fastcall_004d8230(void*,void*,std::uint32_t,void**);
unsigned char __fastcall kotor_fastcall_00674b40(void*,void*);
kotor_reconstruction::StartupString* __fastcall kotor_fastcall_005edeb0(void*,void*,kotor_reconstruction::StartupString*,std::uint32_t) noexcept(false);
std::uint32_t __fastcall unresolved_list_at_pos_005e9b60(void*,void*,void*);
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*,void*,void**);
int __fastcall async_string_equal_005e5200(kotor_reconstruction::StartupString*,void*,kotor_reconstruction::StartupString*);
}
namespace options_video {struct String;}
namespace client_movies {std::int32_t __fastcall StringNotEqual_005e5390(options_video::String*,void*,const char*);}
namespace {
using U=std::uint32_t; using I=std::int32_t; using S=kotor_reconstruction::StartupString;
template<class T>T get(const void* p,U n){T v;std::memcpy(&v,static_cast<const unsigned char*>(p)+n,sizeof v);return v;}
template<class T>void set(void* p,U n,T v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,sizeof v);}
void* at(void* p,U n){return static_cast<unsigned char*>(p)+n;}
void* ptr(U n){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(n));}
S* str(void* p,U n){return std::launder(reinterpret_cast<S*>(at(p,n)));}
const KotorM15ObjectMethods_007516c8* methods(void* p){return get<const KotorM15ObjectMethods_007516c8*>(p,0);}
tlk_core::Tlk* tlk(){return static_cast<tlk_core::Tlk*>(kotor_cell_tlk_007a3a08);}
struct Resource {
 S text; menu_resources::Ref sound; U duration;
 Resource(){c2da_default_ref_00405ed0(&sound,nullptr);}
};
static_assert(sizeof(Resource)==28,"STR_RES exact frame footprint");
struct Slot {
 alignas(4) unsigned char raw[8]; bool live=false;
 S* value(){return std::launder(reinterpret_cast<S*>(raw));}
 ~Slot(){if(live)value()->~StartupString();}
};
struct ExplicitResource {
 alignas(4) unsigned char raw[28];
 ExplicitResource(){feedback_str_res_construct_00414dc0(raw,nullptr);}
 tlk_core::StringResource* value(){return std::launder(reinterpret_cast<tlk_core::StringResource*>(raw));}
 ~ExplicitResource(){value()->text.~StartupString();}
};
S* fetch_resource(void* self,U offset,S* output){
 Resource resource;
 tlk_fetch_0041e550(tlk(),nullptr,get<U>(self,offset),reinterpret_cast<tlk_core::StringResource*>(&resource),0);
 WindowCopy005e5b00(output,nullptr,&resource.text);return output;
}
}
extern "C" {
// 004D64A0 RET4, x87 ST0; signed lower index deliberately remains unchecked.
float __fastcall feedback_get_float_004d64a0(void* self,void*,I index){
 if(index<get<I>(self,0x14))return get<float>(get<void*>(self,0x10),static_cast<U>(index)*4u);
 return 0.0f; // original 0073D700 exact bits 00000000.
}
// 0059B940 RET4 constructs output and destroys the local STR_RES text on normal/unwind paths.
S* __fastcall feedback_spell_name_0059b940(void* self,void*,S* output){return fetch_resource(self,8,output);}
S* __fastcall feedback_race_lower_name_005c7990(void* self,void*,S* output){return fetch_resource(self,4,output);}
S* __fastcall feedback_race_name_005c78f0(void* self,void*,S* output){return fetch_resource(self,0,output);}
S* __fastcall feedback_race_plural_name_005c7a30(void* self,void*,S* output){return fetch_resource(self,8,output);}
S* __fastcall feedback_class_lower_name_005be180(void* self,void*,S* output){return fetch_resource(self,4,output);}
U __fastcall feedback_alignment_0060b330(void* self,void*){
 const std::int16_t alignment=get<std::int16_t>(self,0x80);
 if(alignment<=40)return 3;
 return alignment>=60?2u:1u;
}
S* __fastcall feedback_alignment_name_00648710(void* self,void*,S* output,U){
 Resource resource;
 //0064874D..0064877C genuinely reads alignment, then discards its computed value.
 //Keep the original failing read even though all branches fetch strref0.
 const volatile std::int16_t alignment=get<std::int16_t>(self,0x80);(void)alignment;
 tlk_fetch_0041e550(tlk(),nullptr,0,reinterpret_cast<tlk_core::StringResource*>(&resource),0);
 WindowCopy005e5b00(output,nullptr,&resource.text);return output;
}
S* __fastcall feedback_subrace_00647660(void* self,void*,S* output){
 S* subrace=str(self,0x28);bool empty;
 {S compare("");empty=async_string_equal_005e5200(subrace,nullptr,&compare)!=0;}
 if(empty){
  void* race=at(get<void*>(kotor_cell_rules_007a3a28,0xb0),static_cast<U>(get<std::uint16_t>(self,0x24))*0x34u);
  feedback_race_name_005c78f0(race,nullptr,output);return output;
 }
 WindowCopy005e5b00(output,nullptr,subrace);return output;
}
// 005EE390 RET14h; arg4 is genuinely unused, arg5 updates only flag bit0.
S* __fastcall feedback_fetch_005ee390(void* self,void*,S* output,U id,void* object,U,U flags){
 set(self,0x58,object);
 const unsigned char gender=kotor_fastcall_00674b40(self,nullptr);
 const U changed=(get<U>(self,0x5c)^flags)&1u;
 set(self,0x5c,get<U>(self,0x5c)^changed);
 set(self,0x3c,gender);
 kotor_fastcall_005edeb0(get<void*>(kotor_menu_window_cells::manager_007a39fc,4),nullptr,output,id);
 return output;
}
// 0062B5C0 RET0Ch: bound message history to64 via original sequential assignment/reloads.
void __fastcall feedback_append_0062b5c0(void* self,void*,S* text,U color,U flag){
 if(client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(text),nullptr,"")==0)return;
 if(get<U>(self,0x100)>=0x40u){
  set(self,0x100,U{0x3f});
  std::uint16_t i=0;
  do{
   void* current=at(get<void*>(self,0xf8),static_cast<U>(i)*16u);
   void* next=at(current,16);
   str(current,0)->Assign005e5c50(str(next,0));
   set(current,8,get<U>(next,8));set(current,12,get<unsigned char>(next,12));
   ++i;
  }while(static_cast<U>(i)<get<U>(self,0x100));
 }
 str(at(get<void*>(self,0xf8),get<U>(self,0x100)*16u),0)->Assign005e5c50(text);
 set(at(get<void*>(self,0xf8),get<U>(self,0x100)*16u),8,color);
 set(at(get<void*>(self,0xf8),get<U>(self,0x100)*16u),12,static_cast<unsigned char>(flag));
 set(self,0x100,get<U>(self,0x100)+1u);
}
//0063D2D0 RET4 preserves assignment before timer store (including assignment failure).
void __fastcall feedback_set_info_0063d2d0(void* self,void*,S* text){str(self,0xe8)->Assign005e5c50(text);set(self,0xf0,U{5000});}
//00414DC0 RET0, untouched duration+18h; constructor cleanup owns only string+0.
void* __fastcall feedback_str_res_construct_00414dc0(void* self,void*){
 S* text=::new(self) S();
 try{c2da_default_ref_00405ed0(reinterpret_cast<menu_resources::Ref*>(at(self,8)),nullptr);}
 catch(...){text->~StartupString();throw;}
 return self;
}
//00646E70 RET8 (hidden output, flag). Actual item name and unidentified-name paths.
S* __fastcall feedback_item_name_00646e70(void* self,void*,S* output,U flag){
 Resource resource; S name;
 if(flag==0){WindowCopy005e5b00(output,nullptr,str(self,0x18c));return output;}
 S* custom=str(self,0x18c);
 if(custom->BorrowedLength005e5790()>0 && get<U>(self,0x114)!=0){WindowCopy005e5b00(output,nullptr,custom);return output;}
 void* base=kotor_m15b_base_item_array_get_base_item_005b31a0(get<void*>(kotor_cell_rules_007a3a28,0x34),nullptr,get<I>(self,0xc));
 tlk_fetch_0041e550(tlk(),nullptr,get<U>(base,0),reinterpret_cast<tlk_core::StringResource*>(&resource),0);
 name.Assign005e5c50(&resource.text);
 if(get<U>(self,0x114)==0){
  tlk_fetch_0041e550(tlk(),nullptr,0,reinterpret_cast<tlk_core::StringResource*>(&resource),0);
  S separator(" "); Slot first,second;
  S* joined=GuiConcat005e5d10(&name,nullptr,first.value(),&separator);first.live=true;
  S* complete=GuiConcat005e5d10(joined,nullptr,second.value(),&resource.text);second.live=true;
  name.Assign005e5c50(complete);
 }
 WindowCopy005e5b00(output,nullptr,&name);return output;
}
//005F6640 RET8; virtual slots are called through the canonical typed native table.
U __fastcall feedback_object_name_005f6640(void* self,void*,U id,S* output){
 Resource resource;void* object=nullptr;
 const unsigned char status=kotor_fastcall_004d8230(get<void*>(self,0x14),nullptr,id,&object);
 if(status!=0 || object==nullptr){
  void* list=get<void*>(self,0x1c);void* position=get<void*>(list,0);
  while(position){
   void* player=ptr(unresolved_list_at_pos_005e9b60(get<void*>(self,0x1c),nullptr,position));
   if(player && get<U>(player,0x2c)==id){output->Assign005e5c50(str(player,0x30));return 1;}
   unresolved_list_next_005e9b90(get<void*>(self,0x1c),nullptr,&position);
  }
  GuiAssign005e5140(output,nullptr,"");return 0;
 }
 const unsigned char kind=get<unsigned char>(object,8);
 if(kind==5){
  void* creature=methods(object)->s02c_as_creature(object,nullptr);Slot value;
  S* result=static_cast<S*>(kotor_m15b_creature_stats_get_full_name_00647ce0(get<void*>(creature,0x2f8),nullptr,value.value()));value.live=true;
  output->Assign005e5c50(result);return 1;
 }
 if(kind==9){
  void* door=ptr(methods(object)->s04c(object,nullptr));bool empty;
  {Slot test;S* value=static_cast<S*>(methods(door)->s070_get_name(door,nullptr,test.value()));test.live=true;empty=value->BorrowedEquals005e5290("")!=0;}
  door=ptr(methods(object)->s04c(object,nullptr));
  if(empty){
   I row;auto* table=get<c2da_current::Table*>(get<void*>(kotor_cell_rules_007a3a28,0xb8),0x2c);
   c2da_current_get_int_entry_00413510(table,nullptr,get<unsigned char>(door,0x10c),static_cast<I>(kotor_rules_cell32_007a2258),&row);
   tlk_fetch_0041e550(tlk(),nullptr,static_cast<U>(row),reinterpret_cast<tlk_core::StringResource*>(&resource),0);
   output->Assign005e5c50(&resource.text);return 1;
  }
  Slot value;S* result=static_cast<S*>(methods(door)->s070_get_name(door,nullptr,value.value()));value.live=true;output->Assign005e5c50(result);return 1;
 }
 if(kind==10 || kind==7){
  void* target=ptr(kind==10?methods(object)->s014(object,nullptr):methods(object)->s03c(object,nullptr));
  {Slot value;S* result=static_cast<S*>(methods(target)->s070_get_name(target,nullptr,value.value()));value.live=true;output->Assign005e5c50(result);}
  if(kind==7 && output->BorrowedEquals005e5290("")!=0){
   //00414DC0's additional resource has an independent lifetime.
   ExplicitResource fallback;tlk_fetch_0041e550(tlk(),nullptr,0x58d,fallback.value(),0);output->Assign005e5c50(&fallback.value()->text);
  }
  return 1;
 }
 if(kind==6){
  void* item=ptr(methods(object)->s034(object,nullptr));Slot value;
  S* result=feedback_item_name_00646e70(item,nullptr,value.value(),1);value.live=true;output->Assign005e5c50(result);return 1;
 }
 GuiAssign005e5140(output,nullptr,"");return 0;
}
}
namespace feedback_lowers {
//00674CAC..00675060 is a continuation of00674B90, not a new original entry.
void token_object(tlk_core::Tlk* self,tlk_core::Token* token,S* value){
 auto object=[self](){return get<void*>(self,0x58);};
 auto creature=[&](){void* current=object();return methods(current)->s02c_as_creature(current,nullptr);};
 auto stats=[&](){return get<void*>(creature(),0x2f8);};
 auto assign_gui=[value](U id){
  Slot slot;S* result=kotor_fastcall_005edeb0(get<void*>(kotor_menu_window_cells::manager_007a39fc,4),nullptr,slot.value(),id);slot.live=true;value->Assign005e5c50(result);
 };
 void* current=object();
 if(methods(current)->s00c_as_creature_or_self(current,nullptr)==nullptr){GuiAssign005e5140(value,nullptr,"NO OBJECT");return;}
 if(creature()==nullptr){assign_gui(::get<U>(token,0x20));return;}
 if(stats()==nullptr){GuiAssign005e5140(value,nullptr,"NO STATS");return;}
 const unsigned char saved=get<unsigned char>(self,0x3c);
 const U type=::get<U>(token,0xc);
 if(type==3 || type==4){value->Assign005e5c50(str(stats(),type==3?0x14u:0x1cu));}
 else if(type>=2 && type<=14 && type!=13){
  Slot slot;S* result=nullptr;
  switch(type){
  case 2:result=static_cast<S*>(kotor_m15b_creature_stats_get_full_name_00647ce0(stats(),nullptr,slot.value()));break;
  case 5:case 6:{
   void* s=stats();const U race=get<std::uint16_t>(s,0x24);
   void* row=at(get<void*>(kotor_cell_rules_007a3a28,0xb0),race*0x34u);
   result=type==5?feedback_race_lower_name_005c7990(row,nullptr,slot.value()):feedback_race_plural_name_005c7a30(row,nullptr,slot.value());break;
  }
  case 7:result=feedback_subrace_00647660(stats(),nullptr,slot.value());break;
  case 8:case 9:{
   const unsigned char cls=kotor_m15b_creature_stats_get_class_00647730(stats(),nullptr,0);
   void* row=at(get<void*>(kotor_cell_rules_007a3a28,0xac),static_cast<U>(cls)*0x1a0u);
   result=type==8?static_cast<S*>(kotor_m15b_class_get_name_text_005be0e0(row,nullptr,slot.value())):feedback_class_lower_name_005be180(row,nullptr,slot.value());break;
  }
  case 10:{const unsigned char level=kotor_m15b_creature_stats_get_class_level_00647750(stats(),nullptr,0);result=resman_string_from_int_005e5bc0(slot.value(),nullptr,level);break;}
  case 11:case 12:result=feedback_alignment_name_00648710(stats(),nullptr,slot.value(),type==11?0u:1u);break;
  case 14:{
   const unsigned char alignment=static_cast<unsigned char>(feedback_alignment_0060b330(stats(),nullptr));
   const U choice=alignment==2?0u:(alignment==3?2u:1u);
   const U id=::get<U>(token,0x10+choice*4u);
   result=kotor_fastcall_005edeb0(get<void*>(kotor_menu_window_cells::manager_007a39fc,4),nullptr,slot.value(),id);break;
  }
  default:break;
  }
  slot.live=true;value->Assign005e5c50(result);
 }
 //The original restores gender after the return-slot destructor; exceptions skip restoration.
 set(self,0x3c,saved);
}
}
