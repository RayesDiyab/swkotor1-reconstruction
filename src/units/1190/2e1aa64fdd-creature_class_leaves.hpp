#pragma once
#include "2f013485b7-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct CreatureClassLeafPorts{virtual ~CreatureClassLeafPorts()=default;virtual void byte_write(Word,Word)=0;};
void creature_set_class_005a5420(Memory&,CreatureClassLeafPorts&,Word,Word,Word);
Word creature_get_class_005a4e90(Memory&,Word,Word);
Word creature_force_class_005be4a0(Word);
}
