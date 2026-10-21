#include "4e896be59fd013e5_resource_load.hpp"
#include <cstring>
namespace kotor::vm_executor {
namespace {
std::int32_t signed_word(Word x) { std::int32_t y;std::memcpy(&y,&x,4);return y; }
constexpr Word empty=0x0073d71c;
Word descriptor(Memory& m,Word s) { return s+0x2c+40*m.read(s+0x28); }
}
Word construct_ncs_005d4c30(Memory& m,ResourcePorts& p,Word s) {
 p.resource_construct_00406460(s);
 m.write(s+0x28,0);m.write(s+0x2c,0);m.write(s+0x30,0);
 m.write(s,0x0074c508);return s;
}
void select_resource_005d1ac0(Memory& m,ResourcePorts& p,Word s,Word ref,Word flag,ResourceScratch scratch) {
 const Word stored=s+0xc;
 if(p.resref_equal_004060b0(ref,stored))return;
 if(m.read(s+8) && p.resref_not_chars_00406060(stored,empty)) {
  if(m.read(s+4))p.resource_cancel_00409b00(m.read(s+8));
  const Word resource=m.read(s+8);
  if(!p.resource_remove_00409cf0(m.read(0x007a39e8),resource)) {
   const Word live=m.read(s+8);
   if(live)p.resource_virtual_destroy(live,1);
   m.write(s+8,0);
  }
 }
 if(p.resref_equal_chars_004060d0(ref,empty)) {
  m.write(s+8,0);p.resref_assign_chars_00406290(stored,scratch.assignment_result,empty);return;
 }
 Word resource=p.resource_find_004074d0(m.read(0x007a39e8),ref,0x7da);
 if(!resource) {
  const Word allocation=p.allocate_006fa7e6(0x34);
  resource=allocation?construct_ncs_005d4c30(m,p,allocation):0;
  p.resource_register_00407680(m.read(0x007a39e8),ref,0x7da,resource);
  if(!resource)return;
 }
 m.write(s+8,resource);
 if(flag) {m.write(s+4,1);p.resource_service_00408620(resource);}
 p.resref_assign_00406120(stored,scratch.assignment_result,ref);
}
void copy_script_005d16f0(Memory& m,ResourcePorts& p,Word d,Word data,Word size) {
 m.write(d+0x20,0);m.write(d,0);m.write(d+4,0);m.write(d+8,13);m.write(d+0xc,0);m.write(d+0x14,size);
 const Word target=p.allocate_006fa7e6(size);m.write(d+0x10,target);
 // Original REP MOVSD then MOVSB: preserve chunk order, not memmove semantics.
 Word offset=0;
 for(Word i=0;i<(size>>2);++i,offset+=4)m.write(target+offset,m.read(data+offset));
 for(Word i=0;i<(size&3);++i,++offset)m.write_byte(target+offset,m.read_byte(data+offset));
 m.write(d+0x24,0);
}
Word load_script_005d2260(Memory& m,ResourcePorts& p,Word s,Word script,ResourceScratch scratch) {
 Word index=m.read(s+0x28)+1;m.write(s+0x28,index);
 if(signed_word(index)>=8){m.write(s+0x28,index-1);return 0xffffffa2;}
 if(script)p.resref_from_string_00406d60(scratch.resref,script);
 else p.resref_from_chars_00406d80(scratch.resref,empty);
 select_resource_005d1ac0(m,p,s,scratch.resref,0,scratch);
 if(!p.resource_demand_00409b10(m.read(s+8))) {
  m.write(s+0x28,m.read(s+0x28)-1);return 0xffffffa1;
 }
 const Word data=p.resource_data_00404dd0(m.read(s+8));
 const Word size=p.resource_size_00710e50(m.read(s+8));
 p.string_assign_005e5c50(descriptor(m,s)+0x18,script);
 bool header=m.read_byte(data)=='N' && m.read_byte(data+1)=='C' && m.read_byte(data+2)=='S'
  && m.read_byte(data+3)==' ' && m.read_byte(data+4)=='V' && m.read_byte(data+6)=='.';
 if(header) {
  const auto major=m.read_byte(data+5);Word version=major>='1'&&major<='9'?Word(major-'0')*10:0;
  const auto minor=m.read_byte(data+7);if(minor>='1'&&minor<='9')version+=minor-'0';
  header=version==10 && m.read_byte(data+8)=='B';
 }
 if(!header) {
  m.write(s+0x28,m.read(s+0x28)-1);p.resource_release_00409b80(m.read(s+8));return 0xffffffa0;
 }
 copy_script_005d16f0(m,p,descriptor(m,s),data+13,size-13);
 p.resource_release_00409b80(m.read(s+8));
 p.resref_from_chars_00406d80(scratch.resref,empty);
 select_resource_005d1ac0(m,p,s,scratch.resref,0,scratch);return 0;
}
}
