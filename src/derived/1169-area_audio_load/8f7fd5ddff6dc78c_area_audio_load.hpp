#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop{struct AreaAudioLoadPorts{virtual ~AreaAudioLoadPorts()=default;virtual Word integer(Word,Word,Word,Word,Word)=0;virtual Word byte(Word)=0;virtual void byte(Word,Word)=0;};Word area_audio_load_005c95f0(Memory&,AreaAudioLoadPorts&,Word,Word,Word,Word);}
