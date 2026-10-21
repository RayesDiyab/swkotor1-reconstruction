#include "9665a9df47-gui_sounds.h"
// lane-sound rev3 ADAPTED COPY of the M46 stage unit service-gui_00409F00-27
// (isolated-inputs\r12-mode1-router-ini-r3\service-gui_00409F00-27\gui_sounds.cpp).
// FIX: gui_sounds::bindings is never populated in the M46 stage, but StartServices calls
// CSWGuiManager::LoadGuiSounds 00409F00 unconditionally at 005F9451 (first frame), so every
// binding call was a null call. Control flow is the replica's (default branches; BAD_* mutants
// dropped); each binding is replaced by a direct call of its genuine provider:
//   006FA7E6 -> live_operator_new (live-policy.obj)
//   00406D80 -> menu_resources::Ref::construct_00406d80 (service-resource-13.obj)
//   00413CC0 / 004143B0 / 00413270 -> @c2da_current_construct_00413cc0@28 / @c2da_current_load_004143b0@8 /
//              @c2da_current_get_string_00413270@20 (lane-startup-misc)
//   005B3190 / 005E5A90 / 005E5C20 -> kotor_reconstruction::StartupString ctor/ctor(text)/dtor (startup-strings.obj)
//   00406D60 -> @async_ref_construct_string_00406d60@12 (ref-bodies.obj)
//   005D60E0 / 005D5900 -> kotor_sound_rev3::SourceConstruct005d60e0 / SourceSetPriorityGroup005d5900 (lane-sound rev3)
//   0073DF60 "guisounds", 0073DF54 "SoundResRef": literals of identical bytes.
#include "f9f7ab0530-startup_string.h"
#include "15d7bcda8b-resources.h"
#include <new>
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);
void* __fastcall c2da_current_construct_00413cc0(void* table, void*, menu_resources::Ref name, std::int32_t mode);
int __fastcall c2da_current_load_004143b0(void* table, void*);
std::int32_t __fastcall c2da_current_get_string_00413270(void*, void*, std::int32_t, kotor_reconstruction::StartupString*, kotor_reconstruction::StartupString*);
menu_resources::Ref* __fastcall async_ref_construct_string_00406d60(menu_resources::Ref*, void*, menu_resources::String*);
}
namespace kotor_sound_rev3 {
void* __fastcall SourceConstruct005d60e0(void* self, void*, const void* ref);
void __fastcall SourceSetPriorityGroup005d5900(void* self, void*, std::uint32_t group_word);
}
namespace {
const char k_table_0073df60[] = "guisounds";
const char k_column_0073df54[] = "SoundResRef";
}
namespace gui_sounds {
Bindings bindings{};   // symbol kept for link compatibility; no longer read
using String = kotor_reconstruction::StartupString;
static Word get(const void* p,unsigned o){Word v;std::memcpy(&v,static_cast<const unsigned char*>(p)+o,4);return v;}
static void put(void* p,unsigned o,Word v){std::memcpy(static_cast<unsigned char*>(p)+o,&v,4);}
static std::int32_t rows(const void* table){std::int32_t v;std::memcpy(&v,static_cast<const unsigned char*>(table)+0x24,4);return v;}
static std::int32_t count(const Gui* p){std::int8_t v;std::memcpy(&v,p->bytes+0xa4,1);return v;}
using Delete=void* (__fastcall*)(void*,void*,Word);
static void dispose(void* table){Delete d;std::memcpy(&d,static_cast<unsigned char*>(*static_cast<void**>(table)),4);(void)d(table,nullptr,1);}
static void* ptr(Word w){void* p;std::memcpy(&p,&w,4);return p;}
static Word bits(const void* p){Word w;std::memcpy(&w,&p,4);return w;}
Word __thiscall Gui::load_00409f00(){
 void* raw=live_operator_new(0x54);void* table=nullptr;
 if(raw){menu_resources::Ref reference;reference.construct_00406d80(k_table_0073df60);table=c2da_current_construct_00413cc0(raw,nullptr,reference,0);}
 if(!table)return 0;
 if(!c2da_current_load_004143b0(table,nullptr)){dispose(table);return 0;}
 if(rows(table)>0){
  alignas(4) unsigned char value_bytes[8];String* value=::new(static_cast<void*>(value_bytes)) String();
  Word low=get(table,0x24)&255;bytes[0xa4]=static_cast<unsigned char>(low);
  Word size=static_cast<Word>(count(this))*4;void* array=live_operator_new(size);put(this,0xa0,bits(array));
  if(array){std::int32_t i=0;
   while(i<count(this)){
    std::int32_t found;{String column(k_column_0073df54);found=c2da_current_get_string_00413270(table,nullptr,i,&column,value);}
    if(found){raw=live_operator_new(8);Word sound=0;
     if(raw){menu_resources::Ref reference;async_ref_construct_string_00406d60(&reference,nullptr,value);sound=bits(kotor_sound_rev3::SourceConstruct005d60e0(raw,nullptr,&reference));}
     put(ptr(get(this,0xa0)),static_cast<unsigned>(i)*4,sound);
     Word current=get(ptr(get(this,0xa0)),static_cast<unsigned>(i)*4);if(current)kotor_sound_rev3::SourceSetPriorityGroup005d5900(ptr(current),nullptr,0xb);
    }
    ++i;
   }
  }
  value->~StartupString();
 }
 dispose(table);return 1;
}
}
