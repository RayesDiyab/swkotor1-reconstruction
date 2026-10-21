#include "9e86832032-w3_player.h"
#include <cstring>
namespace {
template<class T> T load(const void* p,std::uint32_t offset) {
    T value; std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,sizeof value); return value;
}
template<class T> void store(void* p,std::uint32_t offset,T value) {
    std::memcpy(static_cast<unsigned char*>(p)+offset,&value,sizeof value);
}
}
// 005F4EC0 RET4. Original0074666C byte=0;00746634 byte=5 (verified read-only).
extern "C" std::uint32_t __fastcall recovered_client_internal_dont_use_this_set_player_creature_005f4ec0(
    void* internal,void*,std::uint32_t id) {
    const std::uint32_t lookup_id=id==0x7f000000u ? id : (id|0x80000000u);
    void* object=nullptr;
    if(kotor_fastcall_004d8230(load<void*>(internal,0x14),nullptr,lookup_id,&object)!=0) return 0;
    if(object==nullptr) return 0;
    if(load<unsigned char>(object,8)!=5) return 0;
    using AsCreature=void* (__fastcall*)(void*,void*);
    auto* creature=load<AsCreature>(load<void*>(object,0),0x2c)(object,nullptr);
    if(creature==nullptr) return 0;
    store(internal,0x3bc,id); store(internal,0x20,id); store(internal,0x3c0,creature);
    return 1;
}
extern "C" std::uint32_t __fastcall recovered_client_dont_use_this_set_player_creature_005ee2e0(
    void* app,void*,std::uint32_t id) {
    return recovered_client_internal_dont_use_this_set_player_creature_005f4ec0(load<void*>(app,4),nullptr,id);
}
