#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct GffStoragePorts {virtual ~GffStoragePorts()=default;virtual Word allocate(Word)=0;virtual void free(Word)=0;virtual void release_004108c0(Word)=0;virtual void byte(Word,Word)=0;};
void gff_owned_storage_00410aa0(Memory&,GffStoragePorts&,Word);
}
