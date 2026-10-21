#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop{struct GffQuaternionPorts{virtual ~GffQuaternionPorts()=default;virtual Word label(Word,Word,Word)=0;virtual Word field(Word,Word,Word)=0;virtual Word data(Word,Word,Word)=0;};Word gff_quaternion_004121b0(Memory&,GffQuaternionPorts&,Word,Word,Word,Word,Word,Word,Word);}
