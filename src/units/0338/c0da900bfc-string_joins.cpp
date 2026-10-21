#include "0fd00dcdce-string_joins.h"
#include "c534281817-startup_string.h"
#include "689940810d-string_priority.h"
#include "c7e191db10-native_strings.h"
#include "7b8c81fcff-allocator_live.h"
#include <new>
#include <type_traits>
#include <cstring>
using String=kotor_reconstruction::StartupString;
static_assert(std::is_same<String,menu_normal_40_49::String>::value&&std::is_same<String,menu_normal_retained_40_49::String>::value,"one canonical live String");
static_assert(std::is_same<menu_resources::Ref,menu_normal_40_49::Ref>::value&&std::is_same<menu_resources::Ref,menu_normal_retained_40_49::Ref>::value,"existing canonical Ref owner");
static_assert(sizeof(menu_normal_40_49::Providers)==160&&sizeof(menu_normal_retained_40_49::Providers)==116,"table layout retained");
static_assert(sizeof(String)==8&&alignof(String)==1,"canonical packed raw storage");



// Cdecl implementation helpers operate on raw storage/live canonical objects.
// No callback casts a foreign String or a fastcall provider to thiscall.
extern "C" void* __cdecl K22_RawConstruct(void* storage){return ::new(storage) String();}
extern "C" void __cdecl K22_RawDestroy(void* receiver){

 static_cast<String*>(receiver)->~StartupString();



}
extern "C" void* __cdecl K22_RawAssign(void* receiver,const char* text){
 void* result=GuiAssign005e5140(static_cast<String*>(receiver),nullptr,text);



 return result;

}
extern "C" const char* __cdecl K22_RawCStr(void* receiver){
 const char* result=WindowCStr005e5670(static_cast<String*>(receiver),nullptr);



 return result;

}
extern "C" unsigned __cdecl K22_RawNotEqual(void* receiver,const char* text){
 // Boolean support composition of the already-selected genuine 005E5290
 // canonical operation. It adds no 005E5390 body or address credit.
 unsigned value=static_cast<String*>(receiver)->BorrowedEquals005e5290(text)==0?1u:0u;



 return value;

}
namespace task22_strings {
extern "C" __declspec(naked) void K22_CtorEntry(){__asm {
 push ecx
 call K22_RawConstruct
 add esp,4
 ret
}}
extern "C" __declspec(naked) void K22_DtorEntry(){__asm {
 push ecx
 call K22_RawDestroy
 add esp,4
 ret
}}
extern "C" __declspec(naked) void K22_AssignEntry(){__asm {
 push dword ptr[esp+4]
 push ecx
 call K22_RawAssign
 add esp,8
 ret 4
}}
extern "C" __declspec(naked) void K22_CStrEntry(){__asm {
 push ecx
 call K22_RawCStr
 add esp,4
 ret
}}
extern "C" __declspec(naked) void K22_NotEqualEntry(){__asm {
 push dword ptr[esp+4]
 push ecx
 call K22_RawNotEqual
 add esp,8
 ret 4
}}
template<class Callback>static Callback code_address(void(__cdecl* entry)()) noexcept {
 static_assert(sizeof(Callback)==4&&sizeof(entry)==4,"pinned MSVC x86 code-address representation");
 Callback result;std::memcpy(&result,&entry,4);return result;
}
Callbacks callbacks() noexcept {return {code_address<Constructor>(&K22_CtorEntry),code_address<Destructor>(&K22_DtorEntry),code_address<Assign>(&K22_AssignEntry),code_address<CStr>(&K22_CStrEntry),code_address<NotEqual>(&K22_NotEqualEntry)};}
template<class Table>static Callbacks read(const Table& t){return {t.string_construct_005b3190,t.string_destroy_005e5c20,t.assign_005e5140,t.cstr_005e5670,t.not_equal_005e5390};}
template<class Table>static void write(Table& t,const Callbacks& c){t.string_construct_005b3190=c.construct;t.string_destroy_005e5c20=c.destroy;t.assign_005e5140=c.assign;t.cstr_005e5670=c.cstr;t.not_equal_005e5390=c.not_equal;}
template<class F>static bool compatible(F old,F wanted){return !old||old==wanted;}
static bool compatible(const Callbacks& old,const Callbacks& wanted){return compatible(old.construct,wanted.construct)&&compatible(old.destroy,wanted.destroy)&&compatible(old.assign,wanted.assign)&&compatible(old.cstr,wanted.cstr)&&compatible(old.not_equal,wanted.not_equal);}
static bool same(const Callbacks& a,const Callbacks& b){return a.construct==b.construct&&a.destroy==b.destroy&&a.assign==b.assign&&a.cstr==b.cstr&&a.not_equal==b.not_equal;}
Result attach(Attachment& a,menu_normal_40_49::Providers& g,menu_normal_retained_40_49::Providers& r) noexcept {
 if(a.active)return Result::AlreadyAttached;
 if(&g!=&menu_normal_40_49::providers||&r!=&menu_normal_retained_40_49::providers)return Result::InvalidIdentity;
 if(g.allocate_006fa7e6!=&live_operator_new||r.allocate_006fa7e6!=&live_operator_new||g.free_006fa390!=&live_free||r.free_006fa390!=&live_free)return Result::AllocatorNotAttached;
 const auto wanted=callbacks();if(!compatible(read(g),wanted)||!compatible(read(r),wanted))return Result::Conflict;
 a.gui=&g;a.retained=&r;a.previous_gui=read(g);a.previous_retained=read(r);a.gui_resource_cell=g.resource_007a39e8;a.retained_resource_cell=r.resource_007a39e8;
 write(g,wanted);write(r,wanted);a.active=true;return Result::PreparedFiveStringSlots;
}
bool detach(Attachment& a,bool drained) noexcept {
 if(!drained)return false;if(!a.active)return true;
 if(!a.gui||!a.retained)return false;const auto wanted=callbacks();
 if(!same(read(*a.gui),wanted)||!same(read(*a.retained),wanted)||a.gui->resource_007a39e8!=a.gui_resource_cell||a.retained->resource_007a39e8!=a.retained_resource_cell||a.gui->allocate_006fa7e6!=&live_operator_new||a.retained->allocate_006fa7e6!=&live_operator_new||a.gui->free_006fa390!=&live_free||a.retained->free_006fa390!=&live_free)return false;
 write(*a.gui,a.previous_gui);write(*a.retained,a.previous_retained);a.gui=nullptr;a.retained=nullptr;a.active=false;return true;
}
}
