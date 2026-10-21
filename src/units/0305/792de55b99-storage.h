#pragma once
#include <cstdint>
#include <initializer_list>
namespace menu_storage {
using Word=std::uint32_t;
enum class Status {complete,missing_provider,invalid_memory};
struct Request {Word site,target,self,count,args[8];};
struct Bindings {
    void* state;
    bool (__cdecl* invoke)(void*,const Request&,Word&);
    bool (__cdecl* read)(void*,Word,void*,Word);
    bool (__cdecl* write)(void*,Word,const void*,Word);
};
struct Context {Bindings bindings;Status status=Status::complete;Word blocked_site=0,blocked_target=0;};
// One non-original producer, declared only. Production links must supply it.
Context& dependencies();
Status construct(Context&,Word entry,Word self,Word argument,Word& result);
bool owned(Word entry);
// Bounded physical views, not accepted full recovered GUI classes.
struct alignas(4) Array {std::uint8_t bytes[12];Array* __thiscall Construct(Word);Array* __thiscall ConstructEvents(Word);};
struct alignas(4) ResRef {std::uint8_t bytes[16];ResRef* __thiscall Construct();};
struct alignas(4) Panel {std::uint8_t bytes[0x64];Panel* __thiscall Construct(Word);};
struct alignas(4) Control {std::uint8_t bytes[0x5c];Control* __thiscall Construct();};
struct alignas(4) Navigable {std::uint8_t bytes[0x6c];Navigable* __thiscall Construct();};
struct alignas(4) Border {std::uint8_t bytes[0x74];Border* __thiscall Construct();};
struct alignas(4) Text {std::uint8_t bytes[0x70];Text* __thiscall Construct();};
struct alignas(4) Button {std::uint8_t bytes[0x1c4];Button* __thiscall Construct();};
struct alignas(4) Label {std::uint8_t bytes[0x140];Label* __thiscall Construct();};
static_assert(sizeof(void*)==4 && sizeof(Array)==12 && sizeof(Panel)==0x64 && sizeof(Label)==0x140 && sizeof(Button)==0x1c4,"Observed bounded x86 storage views");
}
