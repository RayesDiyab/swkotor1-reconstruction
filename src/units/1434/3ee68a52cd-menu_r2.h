#pragma once
#include "8d68d771ce-driver.h"
#include "7fdae3f99f-menu_continuation.h"
namespace frame_menu {
using U64=std::uint64_t;
using WideDispatch=U64(__cdecl*)(const Invocation*);
struct MenuBindings {
 WideDispatch wide_dispatch;
 U32* frame_delta_0078e574;
 U32* resource_manager_007a39e8;
 U32* sound_manager_007a39ec;
 U32* message_manager_007a3a00;
 U32* quiet_mode_0083291c;
};
extern MenuBindings menu_bindings;
// Single nonvirtual base supplies the unchanged first increment's object view.
struct MenuClient:ClientInternal {int __thiscall MainLoopMenuIncrement(menu_continuation::Carry* carry);};
static_assert(sizeof(MenuClient)==sizeof(ClientInternal),"no new historical object fields");
// Native typed virtual-call adapters, tested solely against owned fixtures.
void panel_draw(U32 receiver,float delta); //slot34, ECX + one word, RET4
U32 selected_query(U32 receiver); //slot90, ECX, RET0, caller observes EAX
void drag_draw(U32 receiver,I32 x,I32 y); //slot94, ECX + two words, RET8
U32 deleting_destructor(U32 receiver,U32 flags); //slot0, ECX + one word, RET4
void scene_draw(U32 receiver,float delta); //slot0C, ECX + one word, RET4
U32 player_query(U32 receiver,U32 a,U32 b); //slot98, ECX + two words; target pending
}
