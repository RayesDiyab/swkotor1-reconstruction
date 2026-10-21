#pragma once
#include <cstdint>
namespace frame_menu {
using U32=std::uint32_t;using I32=std::int32_t;
enum class CallKind:U32 { Cdecl,Thiscall,Virtual };
struct Invocation {U32 target,receiver,slot,argc,args[7];CallKind kind;};
// Synthetic/provider seam. It is not an implementation of any named dependency.
using Dispatch=U32(__cdecl*)(const Invocation*);
struct Diagnostic {U32 blocked_at;bool outside_contract;};
struct Bindings {Dispatch dispatch;U32* app_manager_007a39fc;U32* draw_tail_007a3d4c;Diagnostic* diagnostic;};
extern Bindings bindings;
struct GuiManager {U32 storage[42];void __thiscall Draw(float);}; //0040CC50, RET4
struct ClientInternal {U32 storage[309];int __thiscall CaptureMenuSlice();}; //00602EB0 bounded, RET0
static_assert(sizeof(void*)==4 && sizeof(GuiManager)==168 && sizeof(ClientInternal)==1236,"pinned x86 views");
U32 load(U32 address,U32 offset);
void store(U32 address,U32 offset,U32 value);
// Outside-contract sentinel is diagnostic only, never a historical return claim.
constexpr int blocked_result=-2147483647;
}
