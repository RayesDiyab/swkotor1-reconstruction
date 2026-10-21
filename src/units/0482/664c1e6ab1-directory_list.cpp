// lane-resource-lowers: f200_ports::Enumerate005e6640 (CExoBase::GetDirectoryList)
// and its non-M40 lowers. Original evidence (swkotor.exe), lane files:
//   evidence/enum6640.txt      005E6640 mov ecx,[ecx+14h]; jmp 005E8CF0
//   evidence/dirlist8cf0.txt   005E8CF0-005E91D0 body; evidence/eh_dirlist.txt unwind funclets
//   evidence/lower6080.txt     005E6080 LowerCase      evidence/index5600.txt 005E5600 operator[]
//   evidence/cmp5410_5580.txt  005E5410 operator<, 005E5580 operator>=
//   evidence/add59a0.txt       004059A0 Add            evidence/alloc5080.txt 00405080 Allocate
//   evidence/insert8bf0.txt    005E8BF0 FILETIME Insert, 005E8C60 String Insert
//   evidence/alloc4f50.txt     004C4F50 FILETIME Allocate  evidence/dtor_b6f0.txt 0056B6F0 list dtor
// Existing M40 providers reused (no second body): StartupString ctor/dtor/copy/Assign005e5c50/
// BorrowedEquals005e5290/BorrowedLength005e5790, GuiAssign005e5140, WindowCStr005e5670,
// StartupFormat005e5680, c2da_base_005e6660, path_resolution::resolve_005e68a0,
// base_resource_table::ConstructStringVectorNormal/ConstructString/DestroyString (006FB55F
// normal loop), c2da_destroy_array_006fa852, live_operator_new (006FA7E6), live_free (006FA390).
// Win32 imports are the original's own: FindFirstFileA [0073D1B4], FindNextFileA [0073D1B8],
// FindClose [0073D1BC], CompareFileTime [0073D188], GetLastError [0073D208].
#include "6a66e7faaa-directory_list.h"
#include "634f113adb-path.h"
#include "a6dc227f17-baseinternal.h"
#include "1336cd1ecb-native_strings.h"
#include "cebae40286-string_priority.h"
#include "d11fc18ea9-warning_runtime.h"
#include <cstring>
#include <new>
#include <type_traits>
namespace {
using namespace directory_list;
using Base=base_ini_reconstruction::Base;
static_assert(offsetof(Base,aliases)==0x0c&&offsetof(Base,internal)==0x14,"005E8D94 [base+0Ch], 005E6640 [base+14h]");
static_assert(sizeof(List)==12&&offsetof(List,count)==4&&offsetof(List,capacity)==8,"CExoArrayList three words");
static_assert(offsetof(WIN32_FIND_DATAA,ftLastWriteTime)==0x14&&offsetof(WIN32_FIND_DATAA,cFileName)==0x2c,"[esp+74h]/[esp+8Ch] relative to [esp+60h]");
// Raw storage for a hidden-return/out String: the callee begins its lifetime.
struct FreshString {
 alignas(S) unsigned char bytes[sizeof(S)];bool live=false;
 S* output(){return reinterpret_cast<S*>(bytes);}
 ~FreshString(){if(live)std::launder(output())->~StartupString();}
};
// Inline 0056B6F0 (CExoArrayList<FILETIME> dtor): delete data; data=0. The normal
// exits (005E9119/005E917C) inline the same delete.
struct TimeListOwner {TimeList value{nullptr,0,0};~TimeListOwner(){live_free(value.data);value.data=nullptr;}};
S* element(List* list,std::int32_t index){return list->entries+index;}
// Byte loop shared by 005E5422-005E5458 / 005E5592-005E55C8: unsigned byte order.
std::int32_t compare(const char* left,const char* right){
 for(;;){
  const unsigned char a=static_cast<unsigned char>(*left),b=static_cast<unsigned char>(*right);
  if(a!=b)return a<b?-1:1;
  if(a==0)return 0;
  ++left;++right;
 }
}
// repe cmpsb with ECX=n (005E8EAC/005E8ECB): stops at the first differing byte.
bool same_bytes(const char* text,const char* literal,unsigned n){
 for(unsigned i=0;i<n;++i)if(text[i]!=literal[i])return false;
 return true;
}
}
namespace directory_list {
S* __fastcall LowerCase005e6080(S* self,void*,S* output){
 S buffer;                                                // [esp+10h]/[esp+14h] = 0,0 (005E60A3-005E60A7)
 if(self->data==nullptr){                                 // 005E60AB-005E60B7
  ::new(static_cast<void*>(output)) S(buffer);            // 005E60C4 copy (005E5B00)
  return output;
 }
 const W size=static_cast<W>(std::strlen(self->data))+1u; // 005E60DF-005E60EB
 buffer.capacity=size;                                    // 005E60ED [esp+14h]
 char* const text=static_cast<char*>(live_operator_new(size)); // 005E60F1 006FA7E6 (no null test, as original)
 buffer.data=text;                                        // 005E6103 [esp+10h]
 W i=0;
 if(self->data[0]!='\0'){                                 // 005E60F8-005E6107
  do{
   signed char c=static_cast<signed char>(self->data[i]); // 005E6110 (source reloaded each pass)
   if(c>='A'&&c<='Z')c=static_cast<signed char>(c+0x20); // 005E6113-005E611B
   text[i]=static_cast<char>(c);                          // 005E611D
   ++i;
  }while(self->data[i]!='\0');                            // 005E6120-005E6129
 }
 text[i]='\0';                                            // 005E612F
 ::new(static_cast<void*>(output)) S(buffer);             // 005E6139 copy (005E5B00)
 return output;                                           // buffer storage released (005E614F 006FA390)
}
char __fastcall At005e5600(S* self,void*,std::int32_t index){
 if(index<0)return 0;                                     // 005E5605 jl
 const char* const text=self->data;if(!text)return 0;     // 005E5609-005E560D
 if(static_cast<W>(std::strlen(text))<=static_cast<W>(index))return 0; // 005E561E jbe (unsigned)
 return text[index];                                      // 005E5623
}
std::int32_t __fastcall Less005e5410(S* self,void*,S* other){
 const char* const left=self->data;
 if(!left){const char* right=other->data;return right&&right[0]!='\0'?1:0;} // 005E5469-005E5481
 const char* const right=other->data;if(!right)return 0;  // 005E541F
 return compare(left,right)<0?1:0;                        // setl
}
std::int32_t __fastcall GreaterEqual005e5580(S* self,void*,S* other){
 const char* const left=self->data;
 if(!left){const char* right=other->data;return !right||right[0]=='\0'?1:0;} // 005E55D9-005E55F0
 const char* const right=other->data;if(!right)return 1;  // 005E558F -> 005E55E8 -> 1
 return compare(left,right)>=0?1:0;                       // setge
}
void __fastcall Allocate00405080(List* self,void*,std::int32_t capacity){
 S* const old=self->entries;                              // 0040509F
 self->capacity=static_cast<W>(capacity);                 // 004050A9 (before allocation)
 void* const raw=live_operator_new(static_cast<W>(capacity)*8u+4u); // 004050A1/004050AC, unchecked wrap
 S* elements=nullptr;
 if(raw){                                                 // 004050BA
  const W cookie=static_cast<W>(capacity);std::memcpy(raw,&cookie,4); // 004050D3
  elements=reinterpret_cast<S*>(static_cast<unsigned char*>(raw)+4);
  // State 0 (004050BC): funclet 007140D0 frees the raw block if construction unwinds.
  try{base_resource_table::ConstructStringVectorNormal(elements,8,capacity,&base_resource_table::ConstructString,&base_resource_table::DestroyString);}
  catch(...){live_free(raw);throw;}                       // 004050D5 006FB55F
 }
 self->entries=elements;                                  // 004050E9 (after state -1)
 for(std::int32_t i=0;i<static_cast<std::int32_t>(self->count);++i) // 004050DA-0040510A (count reread, signed)
  element(self,i)->Assign005e5c50(old+i);                 // 004050FF (data reread)
 if(old){                                                 // 0040510C
  unsigned char* const block=reinterpret_cast<unsigned char*>(old)-4;
  W cookie;std::memcpy(&cookie,block,4);                  // 00405110
  c2da_destroy_array_006fa852(old,8,cookie,&base_resource_table::DestroyString); // 0040511F
  live_free(block);                                       // 00405125
 }
}
void __fastcall Add004059a0(List* self,void*,S value){
 const W capacity=self->capacity;                         // 004059B8
 if(self->count==capacity)                                // 004059BB
  Allocate00405080(self,nullptr,capacity?static_cast<std::int32_t>(capacity*2u):16); // 004059C8-004059D8
 const W index=self->count;                               // 004059DD
 S* const slot=self->entries+index;                       // 004059E0-004059E2 (before the count store)
 self->count=index+1u;                                    // 004059EB
 slot->Assign005e5c50(&value);                            // 004059EE
}                                                          // 004059FF value destroyed by callee
void __fastcall Insert005e8c60(List* self,void*,S value,std::int32_t index){
 std::int32_t position=static_cast<std::int32_t>(self->count); // 005E8C7B (old count)
 Add004059a0(self,nullptr,value);                         // 005E8C7E-005E8C9B (fresh by-value copy)
 for(;position>index;--position)                          // 005E8CA4/005E8CB9 signed
  element(self,position)->Assign005e5c50(element(self,position-1)); // 005E8CA8-005E8CB1 (data reread)
 element(self,index)->Assign005e5c50(&value);             // 005E8CBB-005E8CC5
}                                                          // 005E8CD6 value destroyed by callee
void __fastcall AllocateTimes004c4f50(TimeList* self,void*,std::int32_t capacity){
 self->capacity=capacity;                                 // 004C4F57
 FILETIME* const old=self->data;                          // 004C4F5B
 self->data=static_cast<FILETIME*>(live_operator_new(static_cast<W>(capacity)<<3)); // 004C4F5D-004C4F69
 for(std::int32_t i=0;i<self->count;++i){                 // 004C4F66-004C4F8A (count reread, signed)
  self->data[i].dwLowDateTime=old[i].dwLowDateTime;       // 004C4F74-004C4F79
  self->data[i].dwHighDateTime=old[i].dwHighDateTime;     // 004C4F7C-004C4F80
 }
 if(old)live_free(old);                                   // 004C4F8C-004C4F91
}
void __fastcall InsertTime005e8bf0(TimeList* self,void*,FILETIME value,std::int32_t index){
 const std::int32_t capacity=self->capacity;              // 005E8BF4
 std::int32_t position=self->count;                       // 005E8BF8
 if(position==capacity)AllocateTimes004c4f50(self,nullptr,capacity?capacity*2:16); // 005E8BFB-005E8C0F
 self->data[self->count]=value;                           // 005E8C14-005E8C24
 ++self->count;                                           // 005E8C28
 for(;position>index;--position)self->data[position]=self->data[position-1]; // 005E8C2F-005E8C48
 self->data[index]=value;                                 // 005E8C4B-005E8C52
}
W __fastcall GetDirectoryList005e8cf0(void*,void*,List* list,S* directory,W type,W directories,W order){
 S pattern;                                               // [esp+18h] state 0
 S name;                                                  // [esp+10h] state 1
 S extension;                                             // [esp+20h] state 2
 extension.Assign005e5c50(c2da_base_005e6660(key_debug_root_007a39e0(),nullptr,type)); // 005E8D40-005E8D5C
 if(extension.BorrowedEquals005e5290(c2da_empty_0073d71c())!=0||directories!=0) // 005E8D61-005E8D76
  (void)GuiAssign005e5140(&extension,nullptr,"*");        // 005E8D78 push 743890 "*"
 {
  FreshString resolved;                                   // [esp+2Ch]
  auto* const aliases=static_cast<alias_path::AliasListView*>(static_cast<Base*>(key_debug_root_007a39e0())->aliases); // 005E8D89-005E8D94 (cell reread)
  S* const result=path_resolution::resolve_005e68a0(aliases,nullptr,resolved.output(),directory,0xffffu); // 005E8D8E-005E8D9C
  resolved.live=true;                                     // state 3
  pattern.Assign005e5c50(result);                         // 005E8DAE
 }                                                        // 005E8DBF, state 2
 const std::int32_t length=pattern.BorrowedLength005e5790(); // 005E8DC8
 const char last=At005e5600(&pattern,nullptr,length-1);   // 005E8DCD-005E8DD3
 {
  const char* const ext=WindowCStr005e5670(&extension,nullptr); // 005E8DE0 / 005E8DFC
  const char* const path=WindowCStr005e5670(&pattern,nullptr);  // 005E8DEA / 005E8E06
  if(last=='\\')kotor_reconstruction::StartupFormat005e5680(&pattern,"%s*.%s",path,ext);   // 74E1B4
  else kotor_reconstruction::StartupFormat005e5680(&pattern,"%s\\*.%s",path,ext);           // 74E1AC
 }
 WIN32_FIND_DATAA found;                                  // [esp+60h]
 HANDLE const handle=FindFirstFileA(WindowCStr005e5670(&pattern,nullptr),&found); // 005E8E1E-005E8E2D
 TimeListOwner times;                                     // [esp+38h..40h] = 0 (005E8E37-005E8E3F), state 4
 if(handle==INVALID_HANDLE_VALUE)return 0;                // 005E8E43 -> 005E9173..005E91D0
 DWORD error=0;                                           // [esp+34h] (005E8E57)
 do{
  (void)GuiAssign005e5140(&name,nullptr,found.cFileName); // 005E8E60-005E8E6C
  FreshString lowered;                                    // [esp+2Ch]
  (void)LowerCase005e6080(&name,nullptr,lowered.output()); // 005E8E71-005E8E7A
  lowered.live=true;                                      // state 5
  S* const key=std::launder(lowered.output());
  const unsigned char attributes=static_cast<unsigned char>(found.dwFileAttributes); // 005E8E82 al=[esp+60h]
  if(directories==0){                                     // 005E8E7F/005E8E8E
   if((attributes&0x10)==0){                              // 005E8F3D-005E8F3F
    if(order==1){                                         // 005E8F48
     std::int32_t high=static_cast<std::int32_t>(list->count)-1; // 005E8F51-005E8F54
     std::int32_t middle=high/2;                          // 005E8F55-005E8F5C (cdq/sub/sar)
     std::int32_t low=0;                                  // [esp+0Ch]
     if(high>0){                                          // 005E8F5E
      do{
       std::int32_t below;
       {FreshString item;S* const lowered_item=LowerCase005e6080(element(list,middle),nullptr,item.output());item.live=true; // state 7
        below=Less005e5410(lowered_item,nullptr,key);}    // 005E8F8E, 005E8FA1
       if(below)low=middle+1;                             // 005E8FAA-005E8FB1
       else high=middle;                                  // 005E8FB7-005E8FBD
       middle=(high+low)/2;                               // 005E8FC1-005E8FC9
      }while(low<high);                                   // 005E8FCB
     }
     if(high>=0){                                         // 005E8FD3
      std::int32_t below;
      {FreshString item;S* const lowered_item=LowerCase005e6080(element(list,middle),nullptr,item.output());item.live=true; // state 8
       below=Less005e5410(lowered_item,nullptr,key);}     // 005E8FF5, 005E9008
      if(below)++middle;                                  // 005E9011
     }
     Insert005e8c60(list,nullptr,name,middle);            // 005E9012-005E9028 / 005E9032-005E9048
    }else if(order==2){                                   // 005E9052
     std::int32_t position=0;
     if(static_cast<std::int32_t>(list->count)>0){        // 005E9057-005E905E
      const FILETIME* slot=times.value.data;              // 005E9060 (loaded once)
      do{
       if(CompareFileTime(&found.ftLastWriteTime,slot)==1)break; // 005E9064-005E9073
       ++position;++slot;                                 // 005E9078-005E9079
      }while(position<static_cast<std::int32_t>(list->count)); // 005E9075/005E907C
     }
     InsertTime005e8bf0(&times.value,nullptr,found.ftLastWriteTime,position); // 005E9080-005E908F
     Insert005e8c60(list,nullptr,name,position);          // 005E9094-005E90AA
    }else{
     Add004059a0(list,nullptr,name);                      // 005E90B1-005E90C6
    }
   }
  }else if((attributes&0x10)!=0                           // 005E8E94
         &&!same_bytes(WindowCStr005e5670(&name,nullptr),".",2)    // 005E8E9C-005E8EB5 (741480)
         &&!same_bytes(WindowCStr005e5670(&name,nullptr),"..",3)){ // 005E8EBB-005E8ED4 (74E1A8)
   if(order==0){                                          // 005E8EDA-005E8EDF
    Add004059a0(list,nullptr,name);                       // 005E90B1
   }else{
    std::int32_t position=0;                              // 005E8EE8
    if(static_cast<std::int32_t>(list->count)>0){         // 005E8EE5-005E8EEC
     do{
      std::int32_t after;
      {FreshString item;S* const lowered_item=LowerCase005e6080(element(list,position),nullptr,item.output());item.live=true; // state 6
       after=GreaterEqual005e5580(lowered_item,nullptr,key);} // 005E8F10, 005E8F23
      if(after)break;                                     // 005E8F28
      ++position;
     }while(position<static_cast<std::int32_t>(list->count)); // 005E8F30-005E8F36
    }
    Insert005e8c60(list,nullptr,name,position);           // 005E9094-005E90AA
   }
  }
  if(!FindNextFileA(handle,&found))error=GetLastError();  // 005E90CB-005E90E5
 }while(error!=0x12);                                     // 005E90ED lowered destroyed; 005E90FA ERROR_NO_MORE_FILES
 FindClose(handle);                                       // 005E9106
 return 1;                                                // 005E9119.. times, extension, name, pattern released
}
}
namespace f200_ports {
void __fastcall Enumerate005e6640(void* base,void*,void* list,String* directory,Word type,Word directories,Word order){
 void* const internal=static_cast<base_ini_reconstruction::Base*>(base)->internal; // 005E6640 mov ecx,[ecx+14h]
 (void)directory_list::GetDirectoryList005e8cf0(internal,nullptr,static_cast<directory_list::List*>(list),directory,type,directories,order); // jmp 005E8CF0
}
}
