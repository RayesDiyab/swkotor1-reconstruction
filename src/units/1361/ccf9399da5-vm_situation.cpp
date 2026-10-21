#include "7acd3f8ce2-vm_situation.hpp"
#include <cstring>
namespace kotor::vm_situation {
namespace {
I32 si(Word x){I32 y;std::memcpy(&y,&x,4);return y;}
Word bits(I32 x){Word y;std::memcpy(&y,&x,4);return y;}
I32 asr2(Word x){return si(x)<0?si(~(~x>>2)):si(x>>2);}
I32 tag_signed(Memory& m,Word a){const Word t=m.read8(a);return (t&0x80)?si(t|0xffffff00):si(t);}
void copy_bytes(Memory& m,Ports& p,Word dst,Word src,Word n){for(Word i=0;i<n;++i)p.write8(dst+i,m.read8(src+i));}
float f32(Word v){float f;std::memcpy(&f,&v,4);return f;}
Word frame(Word vm,Memory& m){return vm+40*m.read(vm+0x28);}
}
// ---------------- implementer slots ----------------
Word location_construct_004ca7a0(Memory& m,Word self){for(Word o=0;o<0x18;o+=4)m.write(self+o,0);return self;}
void location_copy_004ca7c0(Memory& m,Word self,Word source){
 if(!source)return;
 m.write(self+0xc,m.read(source+0xc));m.write(self+0x10,m.read(source+0x10));m.write(self+0x14,m.read(source+0x14));
 m.write(self,m.read(source));m.write(self+4,m.read(source+4));m.write(self+8,m.read(source+8));
}
Word location_equals_0052bf80(Memory&,Ports& p,Word self,Word other){
 if(!p.vector_equal_004aa980(self,other))return 0;
 if(!p.vector_equal_004aa980(self+0xc,other+0xc))return 0;
 return 1;
}
Word talent_construct_004ca9e0(Memory& m,Ports& p,Word self){
 m.write(self+4,0xffffffff);m.write(self,0xffffffff);m.write(self+0x10,0xffffffff);
 p.write8(self+8,0);m.write(self+0xc,0x7f000000);p.write8(self+0x14,0xff);p.write8(self+0x15,0xff);return self;
}
void talent_copy_004caa10(Memory& m,Ports& p,Word self,Word source){
 if(!source)return;
 m.write(self+4,m.read(source+4));m.write(self,m.read(source));p.write8(self+8,m.read8(source+8));
 m.write(self+0xc,m.read(source+0xc));m.write(self+0x10,m.read(source+0x10));
 p.write8(self+0x14,m.read8(source+0x14));p.write8(self+0x15,m.read8(source+0x15));
}
Word talent_equals_004cabb0(Memory& m,Word self,Word other){
 return m.read(self)==m.read(other)&&m.read(self+4)==m.read(other+4)&&m.read8(self+8)==m.read8(other+8)
  &&m.read(self+0xc)==m.read(other+0xc)&&m.read(self+0x10)==m.read(other+0x10)
  &&m.read8(self+0x14)==m.read8(other+0x14)&&m.read8(self+0x15)==m.read8(other+0x15);
}
Word event_equal_004d7270(Memory& m,Ports& p,Word a,Word b){
 const Word ints=m.read(a+8);if(ints!=m.read(b+8))return 0;
 for(I32 i=0;i<si(ints);++i)if(m.read(m.read(a+4)+4*bits(i))!=m.read(m.read(b+4)+4*bits(i)))return 0;
 const Word floats=m.read(a+0x14);if(floats!=m.read(b+0x14))return 0;
 // FLD a[i]; FCOMP b[i]; test ah,44h; jp -> not equal (also unordered). Exact equality incl. +0 == -0.
 for(I32 i=0;i<si(floats);++i)if(!(f32(m.read(m.read(a+0x10)+4*bits(i)))==f32(m.read(m.read(b+0x10)+4*bits(i)))))return 0;
 const Word objects=m.read(a+0x2c);if(objects!=m.read(b+0x2c))return 0;
 for(I32 i=0;i<si(objects);++i)if(m.read(m.read(a+0x28)+4*bits(i))!=m.read(m.read(b+0x28)+4*bits(i)))return 0;
 const Word strings=m.read(a+0x20);if(strings!=m.read(b+0x20))return 0;
 for(I32 i=0;i<si(m.read(a+0x20));++i)if(p.string_not_equal_005e5310(m.read(a+0x1c)+8*bits(i),m.read(b+0x1c)+8*bits(i)))return 0;
 return 1;
}
namespace {
// int/float list (+4 / +0x10): Allocate when nonzero, else release data and zero data/capacity; size = count.
void copy_scalar_list(Memory& m,Ports& p,Word dst,Word src,Word off){
 const Word count=m.read(src+off+4);
 if(count==0){const Word old=m.read(dst+off);if(old){p.free_006fa390(old);m.write(dst+off,0);m.write(dst+off+8,0);}}
 else p.list_allocate_005e03c0(dst+off,count);
 m.write(dst+off+4,count);
 for(I32 i=0;i<si(m.read(src+off+4));++i)m.write(m.read(dst+off)+4*bits(i),m.read(m.read(src+off)+4*bits(i)));
}
}
// 004D77F0 CScriptEvent::CopyScriptEvent (null source: immediate return)
void event_copy_004d77f0(Memory& m,Ports& p,Word dst,Word src){
 if(!src)return;
 m.write16(dst,m.read16(src));
 copy_scalar_list(m,p,dst,src,4);
 copy_scalar_list(m,p,dst,src,0x10);
 const Word objects=m.read(src+0x2c);
 if(objects==0){const Word old=m.read(dst+0x28);if(old){p.free_006fa390(old);m.write(dst+0x28,0);m.write(dst+0x30,0);}}
 else{
  const Word old=m.read(dst+0x28);m.write(dst+0x30,objects);
  const Word data=p.allocate_006fa7e6(objects*4);const Word count=m.read(dst+0x2c);m.write(dst+0x28,data);
  for(I32 i=0;i<si(count);++i)m.write(m.read(dst+0x28)+4*bits(i),m.read(old+4*bits(i)));
  if(old)p.free_006fa390(old);
 }
 m.write(dst+0x2c,objects);
 for(I32 i=0;i<si(m.read(src+0x2c));++i)m.write(m.read(dst+0x28)+4*bits(i),m.read(m.read(src+0x28)+4*bits(i)));
 const Word strings=m.read(src+0x20);
 if(strings==0){
  const Word old=m.read(dst+0x1c);
  if(old){p.string_array_destroy_006fa852(old,m.read(old-4));p.free_006fa390(old-4);m.write(dst+0x1c,0);m.write(dst+0x24,0);}
 }else p.string_list_allocate_00405080(dst+0x1c,strings);
 m.write(dst+0x20,strings);
 for(I32 i=0;i<si(m.read(src+0x20));++i)p.string_assign_005e5c50(m.read(dst+0x1c)+8*bits(i),m.read(src+0x1c)+8*bits(i));
}
Word copy_game_defined_structure_0052c140(Memory& m,Ports& p,Word type,Word source){
 Word allocation=0,result=0;
 switch(type){
 case 0:allocation=p.allocate_006fa7e6(0x8c);result=allocation?p.effect_construct_00503e40(allocation,0):0;p.effect_copy_00504090(result,source,0);return result;
 case 1:allocation=p.allocate_006fa7e6(0x34);result=allocation?p.event_construct_004d7540(allocation):0;event_copy_004d77f0(m,p,result,source);return result;
 case 2:allocation=p.allocate_006fa7e6(0x18);result=allocation?location_construct_004ca7a0(m,allocation):0;location_copy_004ca7c0(m,result,source);return result;
 case 3:allocation=p.allocate_006fa7e6(0x18);result=allocation?talent_construct_004ca9e0(m,p,allocation):0;talent_copy_004caa10(m,p,result,source);return result;
 default:return 0;
 }
}
Word get_equal_game_defined_structure_0052c510(Memory& m,Ports& p,Word type,Word a,Word b){
 switch(type){
 case 0:return m.read(a)==m.read(b)&&m.read(a+4)==m.read(b+4);
 case 1:return event_equal_004d7270(m,p,a,b);
 case 2:return location_equals_0052bf80(m,p,a,b);
 case 3:return talent_equals_004cabb0(m,a,b);
 default:return 0;
 }
}
void destroy_game_defined_structure_00548360(Memory&,Ports& p,Word type,Word value){
 if(type>3||!value)return;
 if(type==0)p.effect_destroy_00503590(value);
 else if(type==1)p.event_destroy_004d7590(value);
 // types 2,3: the original calls the empty 004015A0 then frees
 p.free_006fa390(value);
}
// ---------------- script situations ----------------
void copy_from_stack_005d28f0(Memory& m,Ports& p,Word dst,Word src,Word top_bytes,Word base_bytes){
 p.stack_clear_005d2140(dst);
 p.free_006fa390(m.read(dst+0xc));p.free_006fa390(m.read(dst+0x10));
 I32 top=asr2(top_bytes),base=asr2(base_bytes);
 m.write(dst+0xc,0);m.write(dst+0x10,0);m.write(dst+8,0);m.write(dst+4,0);m.write(dst,0);
 const I32 src_depth=si(m.read(src)),src_base=si(m.read(src+4));
 if(top==0&&base==0){top=src_depth-src_base;base=src_base;}
 if(base>0x10000)base=0x10000;else if(base<0)base=0;
 if(src_base<base)base=src_base;
 if(top>0x10000)top=0x10000;else if(top<0)top=0;
 if(src_base<base)base=src_base;
 const I32 total=top+base;
 if(total==0)return;
 const Word tags=p.allocate_006fa7e6(bits(total+0x10));m.write(dst+0xc,tags);
 const Word words=p.allocate_006fa7e6(bits(total+0x10)*4);m.write(dst+0x10,words);
 m.write(dst,bits(top+base));m.write(dst+8,bits(total+0x10));m.write(dst+4,bits(base));
 copy_bytes(m,p,m.read(dst+0xc)+bits(base),m.read(src+0xc)+bits(src_depth-top),bits(top));
 copy_bytes(m,p,m.read(dst+0xc),m.read(src+0xc)+bits(src_base-base),bits(base));
 auto element=[&](Word dst_index,Word src_index){
  const I32 tag=tag_signed(m,m.read(dst+0xc)+dst_index);
  const Word value=m.read(m.read(src+0x10)+4*src_index);
  Word out;
  if(tag==5){
   const Word allocation=p.allocate_006fa7e6(8);Word string=0;
   if(allocation){p.string_default_005b3190(allocation);string=allocation;}
   p.string_assign_005e5c50(string,value);out=string;
  }else if(tag>=0x10&&tag<=0x19){
   const Word implementer=m.read(m.read(dst+0x14)+0x3d4);
   out=p.implementer_copy_slot8(implementer,bits(tag_signed(m,m.read(src+0xc)+src_index)-0x10),value);
  }else out=value;
  m.write(m.read(dst+0x10)+4*dst_index,out);
 };
 for(I32 i=0;i<base;++i)element(bits(i),bits(src_base-base+i));
 for(I32 i=0;i<top;++i)element(m.read(dst+4)+bits(i),bits(src_depth-top+i));
}
void script_destroy_005d4550(Memory& m,Ports& p,Word s){
 const Word stack=m.read(s);
 if(stack){p.stack_clear_005d2140(stack);p.free_006fa390(stack);}
 m.write(s,0);
 const Word code=m.read(s+0x10);
 if(code){p.free_006fa390(code);m.write(s+0x10,0);}
 p.string_destroy_005e5c20(s+0x18);
}
void delete_script_situation_005d48e0(Memory& m,Ports& p,Word s){if(s){script_destroy_005d4550(m,p,s);p.free_006fa390(s);}}
Word stack_pop_command_005d4380(Memory& m,Ports& p,Word vm,Word out){
 const Word allocation=p.allocate_006fa7e6(0x28);Word s=0;
 if(allocation){p.script_construct_005d1740(allocation);s=allocation;}
 m.write(out,s);
 const Word fr=frame(vm,m);
 if(m.read(fr+0x4c)==0){m.write(s+0x14,0);m.write(m.read(out)+0x10,0);}
 else{
  m.write(s+0x14,m.read(fr+0x40));
  const Word code=p.allocate_006fa7e6(m.read(frame(vm,m)+0x40));m.write(m.read(out)+0x10,code);
  const Word f=frame(vm,m);copy_bytes(m,p,m.read(m.read(out)+0x10),m.read(f+0x3c),m.read(f+0x40));
 }
 m.write(m.read(out)+0x24,m.read(vm+40*(m.read(vm+0x28)+2)));
 m.write(m.read(out)+0x20,m.read(frame(vm,m)+0x4c));
 m.write(m.read(out)+0xc,0);
 m.write(m.read(out)+8,m.read(vm+0x3c8));
 m.write(m.read(out)+4,m.read(vm+0x1ac));
 const Word stack_allocation=p.allocate_006fa7e6(0x18);Word stack=0;
 if(stack_allocation){for(Word o=0;o<0x14;o+=4)m.write(stack_allocation+o,0);stack=stack_allocation;}
 m.write(m.read(out),stack);
 m.write(m.read(m.read(out))+0x14,vm);
 copy_from_stack_005d28f0(m,p,m.read(m.read(out)),vm+0x1ac,m.read(vm+0x3cc),m.read(vm+0x3d0));
 p.string_assign_005e5c50(m.read(out)+0x18,frame(vm,m)+0x44);
 return 1;
}
I32 setup_script_situation_005d23e0(Memory& m,Ports& p,Word vm,Word s){
 Word owned=m.read(s+0x20);
 if(m.read(s+0x10)==0){
  const I32 r=p.read_script_file_005d2260(vm,s+0x18);
  if(r!=0)return r;
  owned=0;
 }else{
  const Word index=m.read(vm+0x28)+1;m.write(vm+0x28,index);
  if(si(index)>=8){m.write(vm+0x28,index-1);return -94;}
 }
 m.write(frame(vm,m)+0x4c,owned);
 p.string_assign_005e5c50(frame(vm,m)+0x44,s+0x18);
 m.write(frame(vm,m)+0x2c,m.read(s));
 m.write(frame(vm,m)+0x30,m.read(s+4));
 m.write(frame(vm,m)+0x34,m.read(s+8));
 m.write(frame(vm,m)+0x38,m.read(s+0xc));
 m.write(vm+40*(m.read(vm+0x28)+2),m.read(s+0x24));
 if(m.read(s+0x10)){m.write(frame(vm,m)+0x3c,m.read(s+0x10));m.write(frame(vm,m)+0x40,m.read(s+0x14));}
 m.write(s,0);m.write(s+0x10,0);
 return 0;
}
Word run_script_situation_005d4ad0(Memory& m,Ports& p,Word vm,Word s,Word object_self,Word flag){
 const Word stack=vm+0x1ac;
 p.stack_clear_005d2140(stack);
 m.write(stack,m.read(m.read(s)));
 m.write(vm+0x1b4,m.read(m.read(s)+8));
 m.write(vm+0x1c0,vm);
 copy_from_stack_005d28f0(m,p,stack,m.read(s),0,0);
 const Word saved=m.read(s);
 if(saved){p.stack_clear_005d2140(saved);p.free_006fa390(saved);m.write(s,0);}
 m.write(vm+0x1c4,0);m.write(vm+0x24,0);
 if(setup_script_situation_005d23e0(m,p,vm,s)!=0){script_destroy_005d4550(m,p,s);p.free_006fa390(s);return 0;}
 const Word index=m.read(vm+0x28);
 if(m.read(vm+40*(index+2))!=0){
  p.delete_script_005d44f0(vm,vm+0x2c+40*index);m.write(vm+0x28,m.read(vm+0x28)-1);
  script_destroy_005d4550(m,p,s);p.free_006fa390(s);return 0;
 }
 m.write(vm+0x16c+4*index,flag);
 m.write(vm+0x18c+4*m.read(vm+0x28),object_self);
 const Word implementer=m.read(vm+0x3d4);
 if(implementer){
  m.write(implementer+4,m.read(vm+0x16c+4*m.read(vm+0x28)));
  m.write(m.read(vm+0x3d4)+8,m.read(vm+0x18c+4*m.read(vm+0x28)));
 }
 const I32 result=p.run_script_file_005d4270(vm,m.read(s+8));
 p.delete_script_005d44f0(vm,vm+0x2c+40*m.read(vm+0x28));
 m.write(vm+0x28,m.read(vm+0x28)-1);
 script_destroy_005d4550(m,p,s);p.free_006fa390(s);
 return result==0;
}
}
