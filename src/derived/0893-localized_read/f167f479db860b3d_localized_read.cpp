#include "f304e68f25dfdba2_localized_read.hpp"
#include <cstdint>
namespace kotor::server_mainloop {
Word localized_internal_copy_005ecbb0(Memory&m,LocalizedPorts&p,Word self,Word source,Word scratch){
 p.clear_005ecb10(self);Word list=m.read(source);m.write(scratch,m.read(list));Word node=m.read(list);Word element=node?m.read(node+8):0;
 while(element){Word allocated=p.allocate(12),fresh=0;if(allocated){p.empty_005b3190(allocated+4);fresh=allocated;}if(!fresh)return 0;
 m.write(fresh,m.read(element));p.string_assign_005e5c50(fresh+4,element+4);Word count=m.read(self+4)+1;Word dest=m.read(self);m.write(self+4,count);p.append_005e9dc0(dest,fresh);element=p.next_005e9b90(m.read(source),scratch);}
 m.write(self+8,m.read(source+8));return 1;
}
Word localized_assign_005e9e70(Memory&m,LocalizedPorts&p,Word self,Word source,Word scratch){if(self!=source){Word src=m.read(source),dst=m.read(self);(void)localized_internal_copy_005ecbb0(m,p,dst,src,scratch);m.write(self+4,m.read(source+4));}return self;}
Word localized_copy_construct_005e9f80(Memory&m,LocalizedPorts&p,Word self,Word source,Word scratch){Word allocated=p.allocate(12),internal=0;if(allocated)internal=p.internal_construct_005ecaf0(allocated);m.write(self,internal);if(self!=source){(void)localized_internal_copy_005ecbb0(m,p,internal,m.read(source),scratch);m.write(self+4,m.read(source+4));}return self;}
Word gff_localized_00411fd0(Memory&m,LocalizedPorts&p,Word self,Word out,Word record,Word label,Word success,Word fallback,Word scratch){
 // scratch+0: data size / live remaining count; +4: temporary localized string;
 // +12: temporary ExoString; +20: copy traversal position.
 Word temp=scratch+4; p.loc_construct_005e9e10(temp);Word selected=fallback;bool ok=false;
 if(record&&label){Word field=p.field_00410990(self,record,p.label_00411630(self,record,label));if(field&&m.read(field)==12){Word data=p.data_00410a20(self,field,scratch);if(data&&m.read(scratch)>=4){Word left=m.read(data);if(m.read(scratch)>=left+4){
 m.write(temp+4,m.read(data+4));Word cursor=data+8;Word count=m.read(cursor);cursor+=4;left-=8;
 if(count){m.write(scratch,count);do{Word id=m.read(cursor),length=m.read(cursor+4);cursor+=8;left-=8;Word str=p.string_bytes_005e5b70(scratch+12,cursor,length);
 // Original signed divide/remainder by two, including negative string IDs.
 std::int32_t signed_id=static_cast<std::int32_t>(id);Word language=static_cast<Word>(signed_id/2),gender=static_cast<Word>(signed_id%2);
 p.loc_add_005ea340(temp,language,str,gender);p.string_destroy_005e5c20(scratch+12);Word remaining=m.read(scratch);cursor+=length;left-=length;--remaining;m.write(scratch,remaining);if(!remaining)break;}while(true);}
 if(left==0){ok=true;selected=temp;}
 }}}}
 m.write(success,ok?1:0);(void)localized_copy_construct_005e9f80(m,p,out,selected,scratch+20);p.loc_destroy_005ea000(temp);return out;
}
}
