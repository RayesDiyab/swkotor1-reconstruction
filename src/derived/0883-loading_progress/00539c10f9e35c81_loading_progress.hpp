#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct LoadingPorts {
 virtual ~LoadingPorts()=default;
 virtual void input_005ed310(Word)=0;
 virtual Word timer_005e9250(Word)=0;
 virtual void literal_005e5a90(Word,Word)=0;
 virtual Word choose_005f4760(Word,Word,Word)=0;
 virtual void panel_0067a9c0(Word,Word)=0;
 virtual void destroy_005e5c20(Word)=0;
 virtual void progress_00417fb0(Word,Word)=0;
 virtual void empty_005b3190(Word)=0;
 virtual void format_005e5680(Word,Word,Word)=0;
};
void loading_hint_005f7b20(Memory&,LoadingPorts&,Word,Word);
void loading_progress_005fb3f0(Memory&,LoadingPorts&,Word,Word,Word,Word);
}
