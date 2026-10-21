#include "564c91413881b15e_texture.hpp"
#include "d1f3cb6677735635_texture_vtable.hpp"
#include <new>
#include <initializer_list>



namespace texture {
// E1 edit T4: `providers` is defined (bound) in texture_cells.cpp.
static I signed_bits(U x){return x&0x80000000U?-1-static_cast<I>(~x):static_cast<I>(x);}
static Basic* element(U data,U index){return reinterpret_cast<Basic*>(*reinterpret_cast<volatile U*>(data+index*4U));}
Ref* __cdecl find_00420ac0(const char* name,const char* derived,const U* vector,I count){
 U index=0;
 if(!derived){
  I size=providers.find_cache->size;
  if(size<=0)return nullptr;
  do {
   Basic* item=element(providers.find_cache->data,index);
   if(providers.compare(name,item->name)==0)return &element(providers.find_cache->data,index)->prefix;
   size=providers.find_cache->size;++index;
  }while(signed_bits(index)<size);
  return nullptr;
 }
 U hash=providers.hash(derived,count,vector);uint16_t low=static_cast<uint16_t>(hash);
 I size=providers.find_cache->size;if(size<=0)return nullptr;
 U data=providers.find_cache->data;
 while(low>element(data,index)->hash){++index;if(signed_bits(index)>=size)return nullptr;}
 do {
  Basic* item=element(data,index);if(item->hash!=low)return nullptr;
  if(count==item->component_count&&count==item->component_count){
   if(providers.compare(derived,item->derived)==0){
    bool same=true;
    if(count>0){const U* stored=item->components;

     for(I i=0;i<count;++i)if(vector[i]!=stored[i]){same=false;break;}



    }
    if(same)return &item->prefix;
   }
   data=providers.find_cache->data;size=providers.find_cache->size;
  }
  ++index;
 }while(signed_bits(index)<size);
 return nullptr;
}






Ref* __fastcall reference_ctor_00422e30(void* memory,void*){
 Ref* self=::new(memory) Ref;
 self->registrations.size=0;self->registrations.capacity=0;self->registrations.data=0;
 self->vtable=providers.reference_table;self->reference.target=nullptr;
 self->reference.target=nullptr;self->environment=0;return self;
}

static void copy(char* to,const char* from){char c;do{c=*from++;*to++=c;}while(c);}
Ref* __fastcall basic_ctor_00423150(void* memory,void*,const char* name,const char* derived){
 Basic* self=::new(memory) Basic;reference_ctor_00422e30(&self->prefix,nullptr);
 self->prefix.vtable=providers.basic_table;set_float_bits(self->f4c,0);set_float_bits(self->f50,0);set_float_bits(self->f54,0); /* E1: raw DWORD 0042318F..95 */self->derived[0]=0;
 copy(self->name,name?name:derived);copy(self->derived,derived);
 set_float_bits(self->f24,0x3f800000u);set_float_bits(self->f18,0x3f800000u);set_float_bits(self->f1c,0x3f800000u);set_float_bits(self->f28,0x3f800000u);set_float_bits(self->f2c,0x3f800000u);set_float_bits(self->f30,0x3f800000u); /* E1: raw DWORD 004231D3..E2 */
 self->f68=2;self->f6c=2;self->c8=2;self->ca=2;self->c2=1;self->c4=1;self->f20=-1.0f;self->b8=15;
 self->ba=0;self->bc=0;self->be=0;self->f70=0;self->f58=0;self->c0=0;self->component_count=0;self->c6=0;self->ce=0;
 self->f5c=0;self->f60=0;self->f64=0;self->f74=7;self->d3=*providers.default_flag;
 set_float_bits(self->f4c,0x3f800000u);set_float_bits(self->f50,0x3f800000u); /* E1: 0042328B/0042329A */self->d2=0;self->flags[3]=0;
 self->flags[0]=1;self->flags[1]=1;self->flags[2]=0;self->flags[4]=1;self->flags[5]=1;
 self->flags[7]=0;self->flags[6]=0;self->flags[9]=0;self->flags[8]=0;self->flags[10]=0;self->flags[11]=0;self->d4=0;
 self->flags[13]=0;self->flags[14]=0;self->flags[15]=1;set_float_bits(self->f54,0x3f800000u); /* E1: 0042330A */
 void* memory_info=live_operator_new(48);FontInfo* info=nullptr;
 if(memory_info){info=::new(memory_info)FontInfo;for(unsigned i:{7U,8U,6U,10U,11U,9U,0U,1U,2U,3U,4U,5U})font_info_zero_word(*info,i);} /* E1 edit T2: typed FontInfo */
 U cc=static_cast<uint16_t>(self->component_count);
 self->info=info;self->f44=0;self->f34=0;self->f40=0;self->components=nullptr;self->f48=0;self->hash=0;
 self->hash=static_cast<uint16_t>(providers.hash(self->derived,static_cast<I>(cc),nullptr));



 providers.add_ordered(self,nullptr);*providers.object_count=*providers.object_count+1U;

 self->e8=-1;self->flags[12]=1;return &self->prefix;
}
// E1 edit T5 (review E-D2): typed vtable slots through texture_vtable::head (live table reloaded per call).
static const texture_vtable::Head* table_of(Basic* self){return texture_vtable::head(&self->prefix);}
static void dispose(FontInfo* info){providers.info_dtor(info,nullptr);live_free(info);}
static U finish(Basic* self){U r=providers.cube_available();if(!r)self->flags[3]=0;return r;}
U __fastcall init_00422af0(Basic* self,void*,const char* name){
 char line[8192];I volatile size=0;
 U loaded=table_of(self)->common.s34(self,nullptr);



 bool embedded=(loaded&255U)!=0;

 if(!embedded){
  if(!self->info){void* memory=live_operator_new(48);self->info=memory?providers.info_ctor(memory,nullptr):nullptr;}
  void* resource=providers.read(name,".txi",&size,1);
  if(!resource){
   FontInfo* old=self->info;if(old)dispose(old);
   auto fallback=table_of(self)->common.sa4;self->info=nullptr;
   return fallback(self,nullptr,1);
  }
  I raw=size;



  I length=raw<0?signed_bits(~static_cast<U>(raw)):raw;

  if(length>0){
   const texture_vtable::Head* table=table_of(self);void* bytes=providers.bytes(0,resource);
   auto set=table->common.sa0;set(self,nullptr,bytes,length);
   const char* text=providers.next_line();
   while(text){while(*text==' '||*text=='\t')++text;providers.parse_line(self,nullptr,text);text=providers.next_line();}
  }
  providers.resource_release(resource,size);
  FontInfo* old=self->info;if(old&&old->upper_left_coords.size==0) /* E1 edit T3: words[7] = +1C */{dispose(old);self->info=nullptr;}
  table_of(self)->common.sa4(self,nullptr,1);
  return finish(self);
 }
 const unsigned char* bytes=static_cast<const unsigned char*>(table_of(self)->common.s94(self,nullptr));
 I remaining=signed_bits(table_of(self)->common.s90(self,nullptr));
 if(bytes&&remaining>0){
  do {
   I n=0;do {unsigned char c=*bytes++;line[n++]=static_cast<char>(c);if(c=='\n')break;}while(n<remaining);
   remaining-=n;line[n]=0;char* text=line;while(*text==' '||*text=='\t')++text;providers.parse_line(self,nullptr,text);
  }while(remaining>0);
 }
 return finish(self);
}
}
