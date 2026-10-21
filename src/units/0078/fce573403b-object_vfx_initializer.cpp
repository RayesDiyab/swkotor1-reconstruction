// Complete original006A09E0 constructor and006A1880 initializer; no render fakes.
#include "e80cbc13bb-object_vfx_initializer.h"
#include "bcf7412b34-startup_string.h"
#include <cstring>
#include <new>
#include <cstdio>
using U=std::uint32_t;using I=std::int32_t;using H=std::uint16_t;using B=std::uint8_t;
using String=kotor_reconstruction::StartupString;
namespace kotor_menu_window_cells {extern void* volatile manager_007a39fc;}
namespace kotor_reconstruction {String* __fastcall StartupAssign005e5c50(String*,void*,String*);}
namespace options_video {struct String;}
namespace aurora_model {struct Gob;}
namespace c2da_current {struct Table;}
namespace client_movies {int __fastcall StringNotEqual_005e5390(options_video::String*,void*,const char*);}
String* __fastcall GuiConcat005e5d10(String*,void*,String*,const String*);
extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern U kotor_rules_cell32_007a2124;
void* __fastcall kotor_fastcall_005ed580(void*,void*,U);
void* __fastcall kotor_fastcall_005ed700(void*,void*);
String* __fastcall GuiAssign005e5140(String*,void*,const char*);
String* __fastcall resman_string_from_int_005e5bc0(String*,void*,I);
char* __fastcall WindowCStr005e5670(String*,void*);
I __fastcall kotor_m15_c2da_get_string_entry_00413de0(void*,void*,void*,void*,void*) noexcept(false);
I __fastcall c2da_current_get_int_entry_00413510(c2da_current::Table*,void*,I,I,I*);
I __fastcall kotor_fastcall_00414110(void*,void*,String*,String*,I*);
aurora_model::Gob* __cdecl aurora_new_aur_object_00449cc0(const char*,const char*,std::FILE*,std::FILE*);
U __fastcall object_vfx_006a0620(void*,void*,U);
}
namespace {
template<class F,class... A> auto opaque(F fn,A... args) {F volatile target=fn;return target(args...);}
template<class T> T read(const void* p,U o) {T v;std::memcpy(&v,static_cast<const B*>(p)+o,sizeof v);return v;}
template<class T> void write(void* p,U o,T v) {std::memcpy(static_cast<B*>(p)+o,&v,sizeof v);}
void* at(void* p,U o) {return static_cast<B*>(p)+o;}
void* ptr(const void* p,U o) {return read<void*>(p,o);}
String* str(void* p,U o) {return reinterpret_cast<String*>(at(p,o));}
template<class F> F slot(void* p,U o) {return read<F>(ptr(p,0),o);}
using Getter=void* (__fastcall*)(void*,void*);
using WordGetter=U (__fastcall*)(void*,void*);
using GetModel=void* (__fastcall*)(void*,void*,U,U);
using Node=void* (__fastcall*)(aurora_model::Gob*,void*,const char*);
using Set=U (__fastcall*)(aurora_model::Gob*,void*,U);
using None=U (__fastcall*)(aurora_model::Gob*,void*);
void* word_view(void* object,U offset) {
 const U bits=slot<WordGetter>(object,offset)(object,nullptr);
 return reinterpret_cast<void*>(static_cast<std::uintptr_t>(bits));
}
void* client() {return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void* table(U o) {return ptr(ptr(kotor_cell_rules_007a3a28,0xb8),o);}
void* view(U id) {
 void* p=opaque(kotor_fastcall_005ed580,client(),nullptr,id);
 return p?slot<Getter>(p,0x0c)(p,nullptr):nullptr;
}
// Storage for original returned-string constructors. Arm only after successful
// construction/concat; normal destruction remains at the original boundary.
struct ReturnedString {
 alignas(String) B bytes[sizeof(String)];bool active=false;
 String* get() {return reinterpret_cast<String*>(bytes);}
 ~ReturnedString() {if(active)get()->~StartupString();}
};
// Constructor cleanup owns only successfully constructed embedded String fields.
struct ConstructorCleanup {
 void* self;U count=0;
 ~ConstructorCleanup() {
  if(count==2)str(self,0x2c)->~StartupString();
  if(count>=1)str(self,0x24)->~StartupString();
 }
};
I string_entry(String* row,const char* label,String* out) {
 String column(label);
 return opaque(kotor_m15_c2da_get_string_entry_00413de0,table(0x88),nullptr,row,&column,out);
}
I integer_entry(String* row,const char* label,I* out) {
 String column(label);
 return opaque(kotor_fastcall_00414110,table(0x88),nullptr,row,&column,out);
}
bool nonempty(String* s) {return opaque(client_movies::StringNotEqual_005e5390,reinterpret_cast<options_video::String*>(s),nullptr,"")!=0;}
void assign(String* dst,String* source) {(void)opaque(kotor_reconstruction::StartupAssign005e5c50,dst,nullptr,source);}
void choose_root(String* row,String* ground,I size) {
 if(size>=5)(void)string_entry(row,"Imp_Root_H_Node",ground);
 if(size>=4&&ground->BorrowedLength005e5790()==0)(void)string_entry(row,"Imp_Root_L_Node",ground);
 if(size>=1&&ground->BorrowedLength005e5790()==0)(void)string_entry(row,"Imp_Root_M_Node",ground);
}
void concat_to(String* dst,void* base,U offset,const char* suffix) {
 String tail(suffix);
 ReturnedString result;
 String* value=opaque(GuiConcat005e5d10,str(base,offset),nullptr,result.get(),&tail);
 result.active=true;
 assign(dst,value);
}
void other_node_name(String* dst,void* first,void* second,B mode) {
 if(first) {
  const char* suffix=mode==0?"_hand":mode==1?"_impact":mode==2?"_head":"_ground";
  concat_to(dst,first,0x144,suffix);
 } else if(second) {
  concat_to(dst,second,0x130,mode<=2?"impc":"grnd");
 } else {
  const char* name=mode==0?"handconjure":mode==1?"impact":mode==2?"headconjure":mode==3?"lhand":mode==4?"rhand":"root";
  (void)opaque(GuiAssign005e5140,dst,nullptr,name);
 }
}
void own_node_name(String* dst,void* first,void* second,B mode) {
 const bool impact=mode==0||mode==1;
 if(first)concat_to(dst,first,0x144,impact?"_impact":"_ground");
 else if(second)concat_to(dst,second,0x130,impact?"impc":"grnd");
 else (void)opaque(GuiAssign005e5140,dst,nullptr,impact?"impact":"root");
}
void check_node(void* object,String* name) {
 auto* model=static_cast<aurora_model::Gob*>(slot<GetModel>(object,0x98)(object,nullptr,0xff,1));
 if(model) {
  void* methods=ptr(model,0); // original caches table pointer before CStr
  const char* text=opaque(WindowCStr005e5670,name,nullptr);
  if(!read<Node>(methods,0x10c)(model,nullptr,text))(void)opaque(GuiAssign005e5140,name,nullptr,"root");
 }
}
void create_model(void* self,String* name,const char* tag,U model_offset,U ready_offset) {
 if(name->BorrowedLength005e5790()<=0)return;
 const char* text=opaque(WindowCStr005e5670,name,nullptr);
 aurora_model::Gob* model=opaque(aurora_new_aur_object_00449cc0,text,tag,nullptr,nullptr);
 write<void*>(self,model_offset,model);
 if(!model)return;
 write<U>(self,ready_offset,0);
 (void)slot<Set>(model,0xc4)(model,nullptr,3);
 model=static_cast<aurora_model::Gob*>(ptr(self,model_offset)); // original reloads object pointer after virtualC4
 (void)slot<None>(model,0xe4)(model,nullptr);
}
static_assert(sizeof(String)==8&&sizeof(void*)==4,"original x86 frame");
}
extern "C" {
void* __fastcall object_vfx_006a09e0(void* self,void*) {
 for(U o=0;o<=0x20;o+=4)write<U>(self,o,0);
 ConstructorCleanup cleanup{self};
 new(str(self,0x24)) String;cleanup.count=1;
 new(str(self,0x2c)) String;cleanup.count=2;
 write<U>(self,0x34,0);write<U>(self,0x38,0);write<U>(self,0x3c,0);
 (void)opaque(GuiAssign005e5140,str(self,0x24),nullptr,"");
 write<U>(self,0x40,0x7f000000);write<U>(self,0x44,0x7f000000);write<U>(self,0x48,0x7f000000);
 for(U i=0;i<3;++i) {
  write<U>(self,0x74+i*4,0);write<U>(self,0x80+i*4,0);write<U>(self,0x4c+i*4,0);
  write<H>(self,0x58+i*2,0);write<U>(self,0x60+i*4,0);write<H>(self,0x6c+i*2,0);
 }
 for(U o=0;o<0x20;o+=4)write<U>(self,o,0);
 write<H>(self,0xa0,0);write<H>(self,0xa2,0);write<H>(self,0xa4,0);
 write<U>(self,0xa8,0);write<H>(self,0xc4,0);
 write<U>(self,0xac,0);write<U>(self,0xb0,0);write<U>(self,0xb4,0);write<U>(self,0xb8,0);
 write<U>(self,0x90,0);write<U>(self,0x94,0);write<U>(self,0x98,0);write<U>(self,0x9c,0);
 write<B>(self,0xc6,0);write<B>(self,0xc7,0);
 write<U>(self,0x8c,0);write<U>(self,0xbc,0);write<U>(self,0xc0,0);write<U>(self,0x20,0);
 write<U>(self,0xd0,0);write<U>(self,0xd4,0);write<B>(self,0xd8,0);write<B>(self,0xd9,0);
 write<U>(self,0xdc,0);write<U>(self,0xe0,0);write<U>(self,0xe4,0);write<U>(self,0xe8,0);
 write<U>(self,0xec,0);write<U>(self,0xf0,0);write<U>(self,0xf4,0);write<U>(self,0xcc,0x3f800000);
 cleanup.count=0;return self;
}
U __fastcall object_vfx_006a1880(void* self,void*,U effect,U arg1,U target_id,U arg2,U arg3,U arg4,U arg5,ObjectVfxPosition position) {
 String type_fd,head,impact,ground;
 write<H>(self,0xc4,static_cast<H>(effect));write<U>(self,0x40,target_id);write<U>(self,0x44,arg2);write<U>(self,0x48,arg3);
 void* actor=view(target_id);if(!actor)return 0;
 void* other=view(read<U>(self,0x44));
 ReturnedString row;
 (void)opaque(resman_string_from_int_005e5bc0,row.get(),nullptr,static_cast<I>(static_cast<H>(effect)));row.active=true;
 (void)string_entry(row.get(),"Type_FD",&type_fd);
 (void)string_entry(row.get(),"Imp_HeadCon_Node",&head);
 if(nonempty(&head)) {
  String quality;
  if(read<std::int8_t>(opaque(kotor_fastcall_005ed700,client(),nullptr),0)>=0) {
   if(string_entry(row.get(),"LowQuality",&quality))assign(&head,&quality);
  }
  if(read<B>(opaque(kotor_fastcall_005ed700,client(),nullptr),0x72)<2) {
   if(string_entry(row.get(),"LowViolence",&quality))assign(&head,&quality);
  }
 }
 (void)string_entry(row.get(),"Imp_Impact_Node",&impact);
 if(nonempty(&impact)) {
  String quality;
  if(read<std::int8_t>(opaque(kotor_fastcall_005ed700,client(),nullptr),0)>=0) {
   if(string_entry(row.get(),"LowQuality",&quality))assign(&impact,&quality);
  }
  if(read<B>(opaque(kotor_fastcall_005ed700,client(),nullptr),0x72)<2) {
   String violence;
   if(string_entry(row.get(),"LowViolence",&violence))(void)string_entry(&violence,"Imp_Impact_Node",&impact);
  }
 }
 I size;
 void* creature=slot<Getter>(actor,0x2c)(actor,nullptr);
 if(!creature)size=3;
 else {
  const H appearance=read<H>(ptr(creature,0x21c),0x18);
  if(!opaque(c2da_current_get_int_entry_00413510,static_cast<c2da_current::Table*>(table(0x0c)),nullptr,static_cast<I>(appearance),static_cast<I>(kotor_rules_cell32_007a2124),&size))size=3;
 }
 choose_root(row.get(),&ground,size);
 if(nonempty(&ground)) {
  String quality;
  if(read<std::int8_t>(opaque(kotor_fastcall_005ed700,client(),nullptr),0)>=0) {
   if(string_entry(row.get(),"LowQuality",&quality))assign(&ground,&quality);
  }
  if(read<B>(opaque(kotor_fastcall_005ed700,client(),nullptr),0x72)<2) {
   String violence;
   if(string_entry(row.get(),"LowViolence",&violence))choose_root(&violence,&ground,size);
  }
 }
 I prog_impact,prog_duration,prog_cessation;
 I has_impact=0,has_duration=0,has_cessation=0;
 if(ground.BorrowedLength005e5790()==0) {
  has_impact=integer_entry(row.get(),"ProgFX_Impact",&prog_impact);
  has_duration=integer_entry(row.get(),"ProgFX_Duration",&prog_duration);
  has_cessation=integer_entry(row.get(),"ProgFX_Cessation",&prog_cessation);
 }
 void* first=nullptr;void* second=nullptr;
 if(other) {first=word_view(other,0x4c);second=word_view(other,0x14);}
 other_node_name(str(self,0x24),first,second,static_cast<B>(arg4));
 first=word_view(actor,0x4c);second=word_view(actor,0x14);
 own_node_name(str(self,0x2c),first,second,static_cast<B>(arg5));
 if(other)check_node(other,str(self,0x24));
 check_node(actor,str(self,0x2c));
 write<ObjectVfxPosition>(self,0x34,position);
 write<U>(self,0xac,1);write<U>(self,0xb0,1);write<U>(self,0xb4,1);
 write<H>(self,0xa0,0xffff);write<H>(self,0xa2,0xffff);write<H>(self,0xa4,0xffff);
 write<B>(self,0xc8,arg1==0?1:0);
 if(has_impact)write<H>(self,0xa0,static_cast<H>(prog_impact));
 if(arg1==0) {
  if(has_duration)write<H>(self,0xa2,static_cast<H>(prog_duration));
  if(has_cessation)write<H>(self,0xa4,static_cast<H>(prog_cessation));
 }
 create_model(self,&ground,"fxground",0x74,0xac);
 create_model(self,&impact,"fximpact",0x78,0xb0);
 create_model(self,&head,"fxhead",0x7c,0xb4);
 write<U>(self,0x9c,0);
 if(read<H>(self,0xa0)!=0xffff)write<U>(self,0x9c,opaque(object_vfx_006a0620,self,nullptr,static_cast<U>(read<H>(self,0xa0))));
 else if(read<H>(self,0xa2)==0xffff&&read<H>(self,0xa4)!=0xffff)write<U>(self,0x9c,1000);
 write<U>(self,0xbc,1);write<U>(self,0xf0,0);
 return 1;
}
}
