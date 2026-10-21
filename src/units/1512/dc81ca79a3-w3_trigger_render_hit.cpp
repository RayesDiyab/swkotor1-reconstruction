#include "6b477b16ff-w3_trigger_render_internal.h"
using namespace trigger_private;
extern "C" {
I __cdecl box_intersect_004aa610(const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*);
I __cdecl polyhit_004ab9d0(const kotor_scene3d::Vector*,I,const kotor_scene3d::Vector*,const kotor_scene3d::Vector*,kotor_scene3d::Vector*,kotor_scene3d::Vector*);
}
extern "C" U __fastcall w3_trigger_part_hit_0049d810(void* self,void*,U from_word,U to_word,U output_word,U info_word,U){
 void* const info=ptr(info_word);if(!(ld<U>(info,4)&0x40000000u))return 0;
 const auto* const from=static_cast<const kotor_scene3d::Vector*>(ptr(from_word));auto* const to=static_cast<kotor_scene3d::Vector*>(ptr(to_word));
 if(!box_intersect_004aa610(from,to,static_cast<const kotor_scene3d::Vector*>(at(self,0x5c)),static_cast<const kotor_scene3d::Vector*>(at(self,0x68))))return 0;
 U offset=0;
 for(U index=0;trigger_render::signed_word(index)<ld<I>(self,0xd4);index+=3u,offset+=0x24u){
  const I hit=polyhit_004ab9d0(static_cast<const kotor_scene3d::Vector*>(at(ld<void*>(self,0xd0),offset)),3,from,to,to,static_cast<kotor_scene3d::Vector*>(at(info,0x40)));
  if(!hit)continue;
  if(output_word){
   void* const output=ptr(output_word);for(U axis=0;axis<3;++axis)st<U>(output,axis*4u,ld<U>(to,axis*4u));
   st<U>(info,0x50,ld<U>(self,0xa4));st<U>(info,0x34,0x40000000u);
  }
  return static_cast<U>(hit);
 }
 return 0;
}
