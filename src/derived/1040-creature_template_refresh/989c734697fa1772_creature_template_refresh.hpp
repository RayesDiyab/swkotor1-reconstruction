#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {struct CreatureTemplateRefreshPorts{virtual ~CreatureTemplateRefreshPorts()=default;virtual Word table_float(Word,Word,Word,Word)=0;virtual void collision_refresh(Word)=0;virtual void float_copy(Word,Word)=0;};void creature_template_refresh_004ed6e0(Memory&,CreatureTemplateRefreshPorts&,Word,Word);}
