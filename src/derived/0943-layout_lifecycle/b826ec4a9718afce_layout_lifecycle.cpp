#include "990243c0f2155963_layout_lifecycle.hpp"
#include <initializer_list>
namespace kotor::server_mainloop {
Word layout_construct_005de770(Memory&m,LayoutLifecyclePorts&p,Word self,Word scratch){
 m.write(self,p.vtable(0x74d350));p.ref_default(self+0xc);m.write(self+8,0);
 p.assign_literal(self+0xc,scratch+0xc,0x73d71c);
 const Word offsets[]={4,0x24,0x34,0x40,0x28,0x38,0x44,0x2c,0x3c,0x48,0x30,0x4c,0x50,0x54,0x58,0x5c};
 for(Word offset:offsets)m.write(self+offset,0);
 m.write(self,p.vtable(0x74d354));return self;
}
void layout_release_005de450(Memory&m,LayoutLifecyclePorts&p,Word self){if(m.read(self+0x20)){p.release(m.read(self+8));m.write(self+0x20,0);}}
void layout_base_destroy_005de590(Memory&m,LayoutReferencePorts&r,LayoutLifecyclePorts&p,Word self){
 Word resource=m.read(self+8);m.write(self,p.vtable(0x74d350));
 if(resource&&r.nonempty_00406060(self+0xc,0x73d71c)){
  if(m.read(self+4)){r.release_00409b00(m.read(self+8));m.write(self+4,0);}
  if(!r.remove_00409cf0(m.read(0x7a39e8),m.read(self+8))){resource=m.read(self+8);if(resource)r.destroy_resource(resource,1);m.write(self+8,0);}
 }
}
void layout_destroy_005de7e0(Memory&m,LayoutReferencePorts&r,LayoutLifecyclePorts&p,Word self){
 m.write(self,p.vtable(0x74d354));layout_release_005de450(m,p,self);
 // The next pointer is fetched before the preceding field is cleared in the original.
 Word pointer=m.read(self+0x34);p.free(pointer);
 const Word sequence[]={0x34,0x40,0x38,0x44,0x3c,0x48};
 for(unsigned i=1;i<6;++i){pointer=m.read(self+sequence[i]);m.write(self+sequence[i-1],0);p.free(pointer);}pointer=m.read(self+0x4c);m.write(self+0x48,0);
 if(pointer){Word count=m.read(pointer-4);while(count){--count;p.string_destroy(pointer+count*8);}p.free(pointer-4);}
 pointer=m.read(self+0x50);m.write(self+0x4c,0);
 if(pointer){Word count=m.read(pointer-4);while(count){--count;p.string_destroy(pointer+count*8);}p.free(pointer-4);}
 pointer=m.read(self+0x54);m.write(self+0x50,0);p.free(pointer);pointer=m.read(self+0x58);m.write(self+0x54,0);p.free(pointer);pointer=m.read(self+0x5c);m.write(self+0x58,0);p.free(pointer);m.write(self+0x5c,0);
 layout_base_destroy_005de590(m,r,p,self);
}
Word layout_load_005df140(LayoutLifecyclePorts&p,Word self,Word ref,Word scratch){p.empty(scratch+4);p.ref_string(ref,scratch+4);Word result=p.parse(self,scratch+4);p.string_destroy(scratch+4);return result;}
Word layout_count_005a4c50(Memory&m,Word self){return m.read(self+0x24);}
}
