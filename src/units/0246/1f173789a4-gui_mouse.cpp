#include "f9de668f9a-gui_mouse.h"
// lane-m48-b D81(4) REPLACEMENT COPY (only change: noexcept(false) on the 005E5A90 declaration) of
// lane-m47-core ADAPTED COPY of the M46 stage unit service-gui_0040B060-26 (base sha256 ae856af27d1afaa6f7681a69e70bc927aee0535524d2d9b13856388e07b3e99c).
// FIX: gui_mouse::bindings is never populated in the M46 stage (tools/bindings_scan.py). StartServices calls
// CSWGuiManager::CreateSoftwareMouse 0040B060 at 005F9439 whenever CExoInput::GetMouseMode 005DF6E0 returns 1
// (byte [[input+4]+378h]), and 005F49DA (ClientInternal::SetMouseMode) / 0040B297 reach it too, so every binding call
// below was a null call on that path. Each binding is replaced by a direct call of the genuine M46 provider (same arity,
// D1 fastcall form), and the three .rdata strings are reproduced as literals of identical bytes (tools/rd.py):
//   006FA7E6 -> live_operator_new (sbh-slice2 live)           004174B0 -> gui_scene_construct_004174b0 (3d-render)
//   005E5A90 -> frame_LogStringCtor_005e5a90 (r4-lowers)       005E5C20 -> frame_LogStringDtor_005e5c20 (r4-lowers)
//   00417620 -> aurora_gui_scene_add_model_name_00417620 (3d-model)  00405FE0 -> c2da_ref_00405fe0 (ref-shared-provider)
//   0043E150 -> menu_tables_required::AnimateExternal_0043e150 (startup-misc-mouse; Word 3F800000h passed as 1.0f)
//   0073DFF8 "gui_mouse", 0073D71C "", 0073DF74 "center".
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);   // 006FA7E6 owner (allocator_live.h; not included: <windows.h> defines `small`)
void* __fastcall gui_scene_construct_004174b0(void* gui_scene, void*);
void __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text) noexcept(false); // D81(4): matches r4_log_joins.cpp:36 definition
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);
void* __fastcall aurora_gui_scene_add_model_name_00417620(void* gui_scene, void*, void* name, std::int32_t index);
char* __fastcall c2da_ref_00405fe0(void* receiver, void*) noexcept;
}
namespace menu_tables_required { void __fastcall AnimateExternal_0043e150(void* object, void* unused_edx, float weight); }
namespace {
const char k_cursor_0073dff8[] = "gui_mouse";
const char k_empty_0073d71c[] = "";
const char k_center_0073df74[] = "center";
}
namespace gui_mouse {
Bindings bindings{};
static Word word(const void* p,unsigned o){Word v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,4);return v;}
static void store(void* p,unsigned o,Word v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,4);}
static std::int32_t signed_word(const void* p,unsigned o){std::int32_t v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,4);return v;}
static std::int32_t dimension(const Gui* p,unsigned o){



 std::int16_t v;

 std::memcpy(&v,p->bytes+o,2);return v;}
template<class T>static T slot(Word table,unsigned offset){T f;Word v=word(reinterpret_cast<void*>(table),offset);std::memcpy(&f,&v,4);return f;}
static Word table(Word object){return word(reinterpret_cast<void*>(object),0);}
using Rectangle=void (__thiscall*)(void*,Rect*);using Reference=void (__thiscall*)(void*,const char*,Word);
using Transform=void (__thiscall*)(void*,Vec*,Vec);using Color=void (__thiscall*)(void*,const void*,Vec);using Final=void (__thiscall*)(void*,Vec,Word);
static Vec scale(std::int32_t width,std::int32_t height){Vec v;const Word small=0x3ba3d70a,depth=0x3c147ae2;Word x,y,z;
 __asm {
 fild width
 fld dword ptr small
 fmul st,st(1)
 fstp dword ptr x
 fild height
 fmul dword ptr small
 fstp dword ptr y
 fmul dword ptr depth
 fstp dword ptr z
 }
 v={x,y,z};return v;}
static Vec position(std::int32_t x,std::int32_t y){Vec v;const Word factor=0x3c23d70a;Word a,b;
 __asm {
 fild x
 fmul dword ptr factor
 fstp dword ptr a
 fild y
 fmul dword ptr factor
 fstp dword ptr b
 }
 v={a,b,0};return v;}
Word __thiscall Gui::activate_0040b060(){void* raw=live_operator_new(0x34);Word object=raw?static_cast<Word>(reinterpret_cast<std::uintptr_t>(gui_scene_construct_004174b0(raw,nullptr))):0;store(this,0x20,object);if(!object)return 0;
 Rect rect{0,0,dimension(this,0x6c),dimension(this,0x6e)};slot<Rectangle>(table(object),4)(reinterpret_cast<void*>(object),&rect);
 alignas(4) unsigned char text[8];frame_LogStringCtor_005e5a90(text,nullptr,k_cursor_0073dff8);Word loaded=static_cast<Word>(reinterpret_cast<std::uintptr_t>(aurora_gui_scene_add_model_name_00417620(reinterpret_cast<void*>(word(this,0x20)),nullptr,text,-1)));store(this,0x24,loaded);frame_LogStringDtor_005e5c20(text,nullptr);



 if(!word(this,0x24))return 0;

 Word cached_table=table(word(this,0x24));Word reference=static_cast<Word>(reinterpret_cast<std::uintptr_t>(c2da_ref_00405fe0(bytes+0x28,nullptr)));Word receiver=word(this,0x24);



 slot<Reference>(cached_table,0x34)(reinterpret_cast<void*>(receiver),k_empty_0073d71c,reference);
 std::int32_t width=dimension(this,0x6c),height=dimension(this,0x6e);Word child=word(reinterpret_cast<void*>(word(this,0x20)),0x18);Vec vector=scale(width,height);slot<Transform>(table(child),0x18)(reinterpret_cast<void*>(child),&vector,vector);
 Word delta=static_cast<Word>(dimension(this,0x6e))-word(this,4);std::int32_t y;std::memcpy(&y,&delta,4);vector=position(signed_word(this,0),y);receiver=word(this,0x24);slot<Transform>(table(receiver),0x5c)(reinterpret_cast<void*>(receiver),&vector,vector);
 if(bytes[0x1c]&4){receiver=word(this,0x24);slot<Color>(table(receiver),0x18)(reinterpret_cast<void*>(receiver),k_center_0073df74,Vec{0x3f800000,0,0});menu_tables_required::AnimateExternal_0043e150(reinterpret_cast<void*>(word(this,0x24)),nullptr,1.0f);receiver=word(this,0x24);slot<Color>(table(receiver),0x18)(reinterpret_cast<void*>(receiver),k_empty_0073d71c,Vec{0,0x80,0});}
 receiver=word(this,0x24);slot<Final>(table(receiver),0xb8)(reinterpret_cast<void*>(receiver),Vec{0x3f800000,0x3f800000,0x3f800000},1);return 1;
}
}
