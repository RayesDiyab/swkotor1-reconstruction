#pragma once
#include "4f727ab50b1551ae_server_mainloop.hpp"
namespace kotor::server_mainloop {
struct MeshSegmentClipPorts{virtual ~MeshSegmentClipPorts()=default;virtual bool equal(Word,Word)=0;virtual Word plus_one(Word)=0;virtual Word crossings(Word,Word,Word,Word)=0;virtual void length(Word)=0;virtual bool first_compare(Word,Word,bool)=0;virtual bool second_compare(Word)=0;};
Word mesh_segment_clip_00583e30(Memory&,MeshSegmentClipPorts&,Word,Word,Word,Word,Word,Word,Word);
}
