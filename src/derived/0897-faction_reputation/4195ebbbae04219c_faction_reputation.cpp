#include "010a271934ca2b7b_faction_reputation.hpp"
#include <cstdint>
namespace kotor::server_mainloop {
static std::int32_t signed_word(Word x){return static_cast<std::int32_t>(x);}
static void resize(Memory&m,ReputationPorts&p,Word array,Word capacity){Word old=m.read(array);m.write(array+8,capacity);Word fresh=p.allocate(capacity*4);m.write(array,fresh);for(Word i=0;signed_word(i)<signed_word(m.read(array+4));++i)m.write(m.read(array)+4*i,m.read(old+4*i));if(old)p.free(old);}
static void set_reputation(Memory&m,Word self,Word row,Word column,Word value){Word n=m.read(self+0x1c);if(signed_word(column)<1||signed_word(column)>signed_word(n)||signed_word(row)<0||signed_word(row)>signed_word(n))return;Word bounded=signed_word(value)>100?100:signed_word(value)<0?0:value;Word array=m.read(self+0x20),index=n*row+column-1;if(signed_word(index)<signed_word(m.read(array+4)))m.write(m.read(array)+4*index,bounded);}
Word reputation_defaults_0052b9e0(Memory&m,ReputationPorts&p,Word self,Word scratch){auto s=[scratch](Word x){return scratch+x;};Word n=m.read(self+0x1c),length=(n+1)*n,old=m.read(self+0x20);m.write(s(0x10),1);if(old){p.free(m.read(old));m.write(old,0);p.free(old);}Word array=p.allocate(12);if(array){m.write(array+4,0);m.write(array+8,0);m.write(array,0);}m.write(self+0x20,array);
 if(signed_word(length)>0){Word remaining=length;do{array=m.read(self+0x20);Word cap=m.read(array+8);if(m.read(array+4)==cap)resize(m,p,array,cap?cap*2:16);Word count=m.read(array+4);m.write(m.read(array)+count*4,100);m.write(array+4,m.read(array+4)+1);--remaining;}while(remaining);}
 array=m.read(self+0x20);Word count=m.read(array+4);if(!count){Word data=m.read(array);if(data){p.free(data);m.write(array,0);m.write(array+8,0);}}else resize(m,p,array,count);m.write(array+4,count);
 Word table=m.read(m.read(m.read(0x7a3a28)+0xb8)+0xe0);m.write(s(0x14),m.read(table+0x24));m.write(s(0x18),m.read(table+0x28));Word row=0;
 while(signed_word(row)<=signed_word(m.read(s(0x14)))){Word column=0;if(m.read(s(0x10)))while(signed_word(column)<signed_word(m.read(s(0x18)))){Word ok=p.table_int_00413510(table,row,column,s(0x1c));m.write(s(0x10),ok);Word value=m.read(s(0x1c));set_reputation(m,self,row,column,value);++column;if(!m.read(s(0x10)))break;}++row;if(!m.read(s(0x10)))break;}
 return m.read(s(0x10));
}
void reputation_load_0052bbe0(Memory&m,ReputationPorts&p,Word self,Word resource,Word list,Word scratch){(void)reputation_defaults_0052b9e0(m,p,self,scratch+0x40);m.write(scratch,1);Word count=p.count_00411940(resource,list);for(Word index=0;m.read(scratch)&&signed_word(index)<=signed_word(count);++index){p.element_00411990(resource,scratch+4,list,index);Word row=p.dword_00411c20(resource,scratch+4,0x74865c,scratch,0);Word column=p.dword_00411c20(resource,scratch+4,0x748650,scratch,0);Word value=p.dword_00411c20(resource,scratch+4,0x748644,scratch,0);set_reputation(m,self,row,column,value);}}
}
