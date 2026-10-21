#pragma once
#include <cstdint>
namespace services_tail5 {
using U32=std::uint32_t;using I32=std::int32_t;
enum class Abi:U32 {Cdecl,Stdcall,Thiscall,Fastcall,Virtual};
struct Call {U32 target,ecx,edx,slot,argc,args[7];Abi abi;};
using Dispatch=U32(__cdecl*)(const Call*);
struct Bindings {Dispatch dispatch;U32* aurora_slots[13];U32* texture_memory_007a399c;};
extern Bindings bindings;
constexpr U32 aurora_slot_addresses[13]={0x7b8fc4,0x7b8fc8,0x78d404,0x78d408,0x7a46f8,0x78d41c,0x7a46fc,0x78d40c,0x78d410,0x78d414,0x78d418,0x78d420,0x78d424};
constexpr U32 aurora_targets[13]={0x70c920,0x70cbd0,0x70c790,0x70cc60,0x70c870,0x70c6e0,0x70c850,0x70d380,0x70cdd0,0x70d6b0,0x70ce80,0x70d510,0x70cd10};
// Fixture views, not recovered complete native classes. Targets/vtables remain
// inert historical identities until actual owned source bindings are reviewed.
struct Message {U32 words[20];Message* __thiscall Construct();};
struct Interface {unsigned char storage;Interface* __thiscall Construct();};
U32 __cdecl Seed(U32 value); // Returns observed retained PTD EAX, ignored by StartServices.
struct Boundary {bool outside_contract;U32 blocked_at;};
struct LocaleBindings {U32* snapshot_words[20];U32* active_007a2824;U32 default_007a27d0;Boundary* boundary;};
extern LocaleBindings locale_bindings;
constexpr U32 locale_snapshot_addresses[20]={0x83402c,0x834030,0x834014,0x834018,0x83401c,0x834020,0x834024,0x834028,0x7a3290,0x7a32a0,0x83430c,0x834310,0x834308,0x7a328c,0x833fec,0x834304,0x834300,0x7a3250,0x7a3190,0x833fe8};
U32 __cdecl Locale(I32 category,U32 name); // Nonnull-name normal path; query/EH held.
struct SoundSetList {U32 words[4];SoundSetList* __thiscall Construct();};
static_assert(sizeof(SoundSetList)==16,"observed four-word sound-list view");
U32 load(U32 address,U32 offset);
void store(U32 address,U32 offset,U32 value);
static_assert(sizeof(void*)==4&&sizeof(Message)==80&&sizeof(Interface)==1,"x86 fixture views");
}
