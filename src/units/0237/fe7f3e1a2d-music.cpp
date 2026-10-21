#include "2f4bcfb219-normal.h"
using namespace menu_normal_retained_40_49;
void __thiscall MusicInternal::music_005f6480(){
 void* old=ptr(0x238);
 if(old){providers.fade_005f1b20(this,nullptr,old,500);old=ptr(0x238);if(old){providers.sound_destroy_005d6200(old,nullptr);providers.free_006fa390(old);}put(0x238,0);}
 if(*providers.sound_007a39ec==nullptr || get(0x2dc)!=0)return;
 void* sound=ptr(0x23c);
 if(sound){if(providers.playing_005d5af0(sound,nullptr)==0)providers.resume_005d5b30(ptr(0x23c),nullptr);return;}
 void* requested=bytes+0x304;
 if(providers.not_equal_005e5390(requested,nullptr,providers.empty_0073d71c)==0){providers.sound_mode_005d5e80(*providers.sound_007a39ec,nullptr,*providers.mode_0074c5f8);return;}
 RawStringStorage formattedStorage,cacheResultStorage;String* formatted=formattedStorage.pointer();String* cacheResult=cacheResultStorage.pointer();Ref ref;
 providers.string_construct_005b3190(formatted,nullptr);
 providers.format_005e5680(formatted,providers.format_0074c304,providers.cstr_005e5670(requested,nullptr));
 providers.cache_004085f0(*providers.resource_007a39e8,nullptr,cacheResult,formatted,4);
 providers.string_destroy_005e5c20(cacheResult,nullptr);
 void* allocation=providers.allocate_006fa7e6(4);
 sound=allocation?providers.sound_construct_005d5a50(allocation,nullptr):nullptr;
 put(0x23c,reinterpret_cast<U32>(sound));
 // Both calls occur even after allocation/constructor failure: no new guard.
 providers.loop_005d5b40(sound,nullptr,1);
 providers.priority_005d5ba0(ptr(0x23c),nullptr,2);
 providers.ref_string_00406d60(&ref,nullptr,requested);



 providers.play_005d5b10(ptr(0x23c),nullptr,&ref,8,0,1,0);

 providers.string_destroy_005e5c20(formatted,nullptr);
}
