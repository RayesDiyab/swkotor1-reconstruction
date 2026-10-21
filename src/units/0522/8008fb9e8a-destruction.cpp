#include "c1ef173d00-destruction.hpp"
#include "6fdf26ef30-texture_controller.hpp"



namespace texture_destruction {
// E1 edit D3: `bindings` is defined (bound) in texture_cells.cpp.






U __fastcall destroy_00422f40(Basic* self,void*){
 self->prefix.vtable=bindings.basic_table;
 FontInfo* info=self->info;
 if(info){bindings.font_info_destroy(info,nullptr);live_free(info);}
 live_free(self->components);
 texture_controller::TextureController* member=texture_controller::from_word(self->f34);          // 00422F8F..93
 if(member)(void)texture_controller::table(member)->s00(member,nullptr,1); // E1 edit D4: typed TextureController +00 (00422F97..9B)
 if(self->d2==0){
  U sizes[5];bindings.memory_usage(self,nullptr,sizes);
  U s1=sizes[1],v1=*bindings.accounting[1];U s0=sizes[0],v0=*bindings.accounting[0];
  v1-=s1;U v2=*bindings.accounting[2];v0-=s0;U s3=sizes[3];
  *bindings.accounting[1]=v1;U s2=sizes[2];v2-=s2;U v4=*bindings.accounting[4];
  *bindings.accounting[0]=v0;U v3=*bindings.accounting[3];*bindings.accounting[2]=v2;
  U s4=sizes[4];v3-=s3;v4-=s4;*bindings.accounting[3]=v3;*bindings.accounting[4]=v4;
 }
 U image_names=self->f40;
 if(image_names){




  I count=static_cast<I>(self->c4)*static_cast<I>(self->c2);

  bindings.gl_delete_textures(count,reinterpret_cast<const U*>(image_names));



  live_free(reinterpret_cast<void*>(self->f40));

  self->f40=0;self->f58=0;
 }
 U extra=self->f48;if(extra){live_free(reinterpret_cast<void*>(extra));self->f48=0;}
 bindings.free_image(self,nullptr);
 *bindings.object_count=*bindings.object_count-1U;
 bindings.remove_ordered(self,nullptr);
 I size=bindings.cache->size;U matches=0;
 if(size>0){U data=bindings.cache->data;
  for(I index=0;index<size;++index)if(*reinterpret_cast<volatile U*>(data+static_cast<U>(index)*4U)==reinterpret_cast<U>(self))++matches;
 }
 if(matches){
  bindings.remove_cache(bindings.cache,nullptr,reinterpret_cast<U>(self));



 }



 return bindings.base_destroy(&self->prefix,nullptr);

}

}
