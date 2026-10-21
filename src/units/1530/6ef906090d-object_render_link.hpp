#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop{
struct ObjectRenderLinkPorts{virtual ~ObjectRenderLinkPorts()=default;virtual Word client_id(Word,Word)=0;virtual Word client_object(Word,Word)=0;virtual Word virtual_render(Word)=0;virtual void back_reference(Word,Word)=0;};
Word object_render_link_004cc2b0(Memory&,ObjectRenderLinkPorts&,Word);
Word object_dirty_004cc220(Memory&,Word,Word);
void object_clear_dirty_004cc250(Memory&,Word,Word);
}
