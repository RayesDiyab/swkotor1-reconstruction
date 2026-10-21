#include "97f9e6186903abdf_layout_parser.hpp"
#include <cstring>
#include <initializer_list>
namespace recovered_layout {
static std::int32_t signed_value(U n){std::int32_t v;std::memcpy(&v,&n,4);return v;}
U parse(U self,U name,U scratch,Ports&p){
 p.ref_from_string(scratch,name);p.set_reference(self,scratch,0);if(!p.demand(p.read(self+8)))return 0;
 U data=p.read(p.read(self+8)+0x30),size=p.read(p.read(self+8)+0x2c);
 const U buffer=p.allocate(size+4),line=p.allocate(size+4);p.zero(buffer,size+4);p.copy(buffer,data,size);
 U cursor=buffer;bool began=false;p.scan_line(cursor,line);
 do {if(p.literal_equal(line,"beginlayout",12))began=true;cursor+=p.length(line)+2;p.scan_line(cursor,line);}while(!began);
 auto finish=[&](){p.write(self+0x20,1);p.free(line);p.free(buffer);return p.read(self+0x20);};
 // The first count line has already been read while searching for beginlayout.
 for(U section=0;section<3;++section){
  if(section){p.scan_line(cursor,line);if(p.literal_equal(line,"donelayout",11))return finish();}
  const U count_at=self+0x24+section*4,refs_at=self+0x34+section*4,vec_at=self+0x40+section*4;
  p.scan_count(line,count_at);cursor+=p.length(line)+2;U count=p.read(count_at);
  if(signed_value(count)>0){
   U refs=p.allocate(count*16);if(refs)for(U i=0;i<count;++i)p.ref_empty(refs+i*16);p.write(refs_at,refs);
   count=p.read(count_at);U vectors=p.allocate(count*12);if(vectors)for(U i=0;i<count;++i)p.vector_empty(vectors+i*12);p.write(vec_at,vectors);
   for(U i=0;signed_value(i)<signed_value(p.read(count_at));++i){p.scan_line(cursor,line);p.scan_room(line,scratch+0x20,p.read(vec_at)+i*12);p.ref_assign(p.read(refs_at)+i*16,scratch,scratch+0x20);cursor+=p.length(line)+2;}
  }
 }
 p.scan_line(cursor,line);if(p.literal_equal(line,"donelayout",11))return finish();
 p.scan_count(line,self+0x30);cursor+=p.length(line)+2;U count=p.read(self+0x30);
 if(signed_value(count)>0){
  for(U field: {0x4cu,0x50u}){count=p.read(self+0x30);U raw=p.allocate(count*8+4),array=0;if(raw){p.write(raw,count);array=raw+4;for(U i=0;i<count;++i)p.string_empty(array+i*8);}p.write(self+field,array);}
  count=p.read(self+0x30);U positions=p.allocate(count*12);if(positions)for(U i=0;i<count;++i)p.vector_empty(positions+i*12);p.write(self+0x54,positions);
  count=p.read(self+0x30);U rotations=p.allocate(count*16);if(rotations)for(U i=0;i<count;++i){p.write(rotations+i*16+12,0);p.write(rotations+i*16+8,0);p.write(rotations+i*16+4,0);p.write(rotations+i*16,0x3f800000);}p.write(self+0x58,rotations);
  count=p.read(self+0x30);p.write(self+0x5c,p.allocate(count*4));
  for(U i=0;signed_value(i)<signed_value(p.read(self+0x30));++i){p.scan_line(cursor,line);p.scan_door(line,scratch+0x34,scratch+0x48,p.read(self+0x5c)+i*4,p.read(self+0x54)+i*12,p.read(self+0x58)+i*16);p.string_literal(p.read(self+0x4c)+i*8,scratch+0x34);p.string_literal(p.read(self+0x50)+i*8,scratch+0x48);cursor+=p.length(line)+2;}
 }
 return finish();
}
}
