#include "06b965e51a-fb80.h"
#include <cstring>
#include <new>
namespace {
using W=menu_resources::Word;using S=menu_resources::String;using Ref=menu_resources::Ref;
W load(const void*p,W n){W v;std::memcpy(&v,static_cast<const unsigned char*>(p)+n,4);return v;}
void store(void*p,W n,W v){std::memcpy(static_cast<unsigned char*>(p)+n,&v,4);}
W word(const void*p){return reinterpret_cast<W>(p);}void* ptr(W v){return reinterpret_cast<void*>(v);}
void* field(void*p,W n){return ptr(load(p,n));}
void* object(void*self,W i){return field(field(self,0x30),i*4u);}
S* name(void*p,W n){return std::launder(reinterpret_cast<S*>(static_cast<unsigned char*>(p)+n));}
void close(void*p){f3c0_ports::Destroy005e69e0(p,nullptr);live_free(p);}
S* assign_text(S*self,const char*text){
 // Preserve typed C++ exception state across the existing extern-C veneer.
 S* (__fastcall* volatile assign)(S*,void*,const char*)=&GuiAssign005e5140;
 return assign(self,nullptr,text);
}
struct FreshString {
 alignas(S) unsigned char bytes[sizeof(S)];bool live=false;
 S* output(){return reinterpret_cast<S*>(bytes);}
 void completed(){live=true;}
 ~FreshString(){if(live)std::launder(output())->~StartupString();}
};
void* file_attempt(void*self){
 void* raw=live_operator_new(4);if(!raw)return nullptr;
 alignas(S) unsigned char storage[sizeof(S)];S* mode;
 try{mode=::new(storage)S("rb");}catch(...){live_free(raw);throw;}
 void* file;try{file=f3c0_ports::Construct005e68d0(raw,nullptr,name(self,0x20),0x270f,mode);}
 catch(...){mode->~StartupString();live_free(raw);throw;}
 mode->~StartupString();return file;
}
W capacity(W count){
 // Exact authenticated constants0073E2A8 and0073D770, not a ratio substitute.
 static const double scale=-1.1764705882352942;
 static const float bias=4294967296.0f;W converted;
 __asm {
  fild dword ptr count
  test count,80000000h
  jz nonnegative
  fadd dword ptr bias
 nonnegative:
  fmul qword ptr scale
  call genuine_x87_ftol_006fae8c
  mov converted,eax
 }
 return 1023u-converted;
}
void late_failure(void*self,void*blob,void*file,bool release_array){
 if(release_array){live_free(field(self,0x30));store(self,0x30,0);}
 live_free(field(self,0x10));store(self,0x10,0);
 fb80_ports::ReleaseBlob005e9360(blob);close(file);
}
void first_name(void*self,W index,S*root,S*filename,std::int32_t dot){
 S separator(":");FreshString stem;
 S* stem_result=startup_rand::SubString005e6270(filename,nullptr,stem.output(),0,dot);stem.completed();
 FreshString root_colon;
 S* prefix_result=GuiConcat005e5d10(root,nullptr,root_colon.output(),&separator);root_colon.completed();
 FreshString full;
 S* full_result=GuiConcat005e5d10(prefix_result,nullptr,full.output(),stem_result);full.completed();
 name(object(self,index),4)->Assign005e5c50(full_result);
}
void fallback_name(void*self,W index,S*fallback,S*filename,std::int32_t dot){
 assign_text(fallback,"CD0:");
 FreshString stem;S* stem_result=startup_rand::SubString005e6270(filename,nullptr,stem.output(),0,dot);stem.completed();
 FreshString full;S* full_result=GuiConcat005e5d10(fallback,nullptr,full.output(),stem_result);full.completed();
 name(object(self,index),4)->Assign005e5c50(full_result);
}
}
namespace fb80_private {
W __cdecl LoadRawType(const TypeFrame* frame){
 W transported;
 // Machine-byte transport only: never evaluate an indeterminate C++ DWORD.
 __asm { mov eax,frame }
 __asm { mov eax,dword ptr [eax] }
 __asm { mov transported,eax }
 return transported;
}
W __fastcall Core(void*self,void*,W last,TypeFrame*type_frame){
 S root,filename,fallback;Ref reference{};
 void* file=file_attempt(self);if(!file){store(self,0,0);return 0;}
 if(!f3c0_ports::Opened005e6a10(file,nullptr)){store(self,0,0);close(file);return 0;}
 unsigned char header[24];(void)f3c0_ports::Read005e6960(file,nullptr,header,24,1);
 if(load(header,0)!=0x2059454bu||load(header,4)!=0x20203156u||(load(header,8)==0&&load(header,12)==0)){store(self,0,0);close(file);return 0;}
 W slots=capacity(load(header,12));store(self,12,slots);
 void* entries=f200_ports::Allocate0040eb80(self,nullptr,slots);store(self,16,word(entries));
 if(!entries){store(self,0,0);close(file);return 0;}
 W bytes=load(header,20)-load(header,16);void* blob=fb80_ports::AllocateBlob005e9350(bytes);
 if(!blob){store(self,0,0);live_free(field(self,16));store(self,16,0);close(file);return 0;}
 (void)f3c0_ports::Seek005e6aa0(file,nullptr,load(header,16),0);
 (void)f3c0_ports::Read005e6960(file,nullptr,blob,bytes,1);
 std::int32_t colon=fb80_ports::Find005e5f20(name(self,0x20),nullptr,':',0);
 if(colon<=0)return 0; // Original intentionally retains file/blob/entries.
 {FreshString prefix;S* result=startup_rand::SubString005e6270(name(self,0x20),nullptr,prefix.output(),0,colon);prefix.completed();root.Assign005e5c50(result);}
 W files=load(header,8);store(self,0x2c,files);void* array=live_operator_new(files*4u);store(self,0x30,word(array));
 if(!array){late_failure(self,blob,file,false);return 0;}
 W cursor=word(blob)+4u;
 for(W i=0;i<load(header,8);++i,cursor+=12u){
  void* archive=live_operator_new(56);
  if(archive){void* raw=archive;try{archive=fb80_ports::Archive0040ee80(raw,nullptr);}catch(...){live_free(raw);throw;}}
  store(field(self,0x30),i*4u,word(archive));archive=object(self,i);
  if(!archive){late_failure(self,blob,file,true);return 0;}
  std::memcpy(static_cast<unsigned char*>(archive)+12,ptr(cursor+6u),2);
  store(object(self,i),16,load(ptr(cursor-4u),0));
  W relative=load(ptr(cursor),0)-load(header,16);store(ptr(cursor),0,relative);
  assign_text(&filename,static_cast<const char*>(ptr(word(blob)+relative)));
  std::int32_t dot=fb80_ports::Find005e5f20(&filename,nullptr,'.',0);
  if(dot<0){late_failure(self,blob,file,true);return 0;}
  first_name(self,i,&root,&filename,dot);
  unsigned short flags;std::memcpy(&flags,static_cast<unsigned char*>(object(self,i))+12,2);
  if(flags!=1&&!fb80_ports::Probe0040d200(self,nullptr,name(object(self,i),4)))fallback_name(self,i,&fallback,&filename,dot);
  archive=object(self,i);using Call=void(__thiscall*)(void*,W);Call call;std::memcpy(&call,static_cast<unsigned char*>(field(archive,0))+0x3c,4);call(archive,0);
 }
 (void)f3c0_ports::Seek005e6aa0(file,nullptr,load(header,20),0);
 Ref source;W id;
 for(W i=0;i<load(header,12);++i){
  (void)f3c0_ports::Read005e6960(file,nullptr,&source,16,1);
  (void)f3c0_ports::Read005e6960(file,nullptr,type_frame->bytes,2,1);
  (void)f3c0_ports::Read005e6960(file,nullptr,&id,4,1);
  Ref discarded;(void)f990_ports::RefCopy00406170(&reference,nullptr,&discarded,&source);
  id&=0x3fffffffu;
  W type=LoadRawType(type_frame);
  f200_ports::Insert0040e990(self,nullptr,&reference,type,id,last);
 }
 close(file);fb80_ports::ReleaseBlob005e9360(blob);return 1;
}
}
namespace add_key_body_ports {
__declspec(naked) Word __fastcall Build0040fb80(void*,void*,Word){
 // A normal C++ local lowered to PUSH ECX, writing the slot. Reserve raw
 // storage explicitly; this resource-free frame needs no C++ cleanup state.
 using fb80_private::Core;
 __asm {
  push ebp
  mov ebp,esp
  sub esp,4
  lea eax,[ebp-4]
  push eax
  push dword ptr [ebp+8]
  xor edx,edx
  call Core
  mov esp,ebp
  pop ebp
  ret 4
 }
}
}
