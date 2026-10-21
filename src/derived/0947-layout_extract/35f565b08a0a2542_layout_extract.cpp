#include "807b25d23caa4df8_layout_extract.hpp"
namespace kotor::server_mainloop {
Word layout_reference_at_005de470(Memory&m,Word self,Word out,Word index){Word from=m.read(self+0x34)+index*16;for(Word i=0;i<16;i+=4)m.write(out+i,m.read(from+i));return out;}
Word layout_position_at_005de4a0(Memory&m,Word self,Word out,Word index){Word from=m.read(self+0x40)+index*12;for(Word i=0;i<12;i+=4)m.write(out+i,m.read(from+i));return out;}
Word layout_extract_00504870(Memory&m,LayoutExtractPorts&p,Word layout,Word scratch){
 Word count=p.count(layout);if(!count)return 0;
 Word allocation=p.allocate(count*0x4c+4),rooms=0;
 if(allocation){m.write(allocation,count);rooms=allocation+4;for(Word i=0;i<count;++i)p.construct(rooms+i*0x4c);}
 for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i){
  Word room=rooms+i*0x4c;Word position=p.position(layout,scratch+0x1c,i);Word reference=p.reference(layout,scratch+0x28,i);p.assign(room,reference,position);p.load(room,0);p.bounds(room);
 }
 for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i)for(Word j=i+1;static_cast<std::int32_t>(j)<static_cast<std::int32_t>(count);++j)p.overlap(rooms+i*0x4c,rooms+j*0x4c,0x3c23d70a);
 for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i)for(Word j=i+1;static_cast<std::int32_t>(j)<static_cast<std::int32_t>(count);++j)p.connect(rooms+i*0x4c,rooms+j*0x4c,i,j);
 for(Word i=0;static_cast<std::int32_t>(i)<static_cast<std::int32_t>(count);++i)p.finish(rooms+i*0x4c);
 return rooms;
}
}
