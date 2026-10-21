#pragma once
#include "3b4eb8c727-server_mainloop.hpp"
namespace kotor::server_mainloop {
struct ModuleMountPorts{virtual ~ModuleMountPorts()=default;virtual void literal_005e5a90(Word,Word)=0;virtual void directory_00408800(Word,Word)=0;virtual void release_004088d0(Word,Word)=0;virtual void destroy_005e5c20(Word)=0;virtual Word lookup_004071a0(Word,Word,Word,Word,Word,Word,Word)=0;};
void party_mount_005638d0(Memory&,ModuleMountPorts&,Word,Word);
void party_unmount_00563950(Memory&,ModuleMountPorts&,Word,Word);
Word resource_scope_00408c00(Memory&,ModuleMountPorts&,Word,Word,Word,Word,Word);
}
