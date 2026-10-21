#include "1d7398170f-vm_execute_live.hpp"
#include <cstring>
// Status values are the original's literal EAX constants at 005D412F..005D4194.
namespace kotor::vm_execute_live {
namespace {
enum:I32{kOk=0,kNoCodeLeft=-107,kInstructionLimit=-93,kPushFailed=-99,kNoImplementer=-108,kBadEqualityTag=-109,
 kStackUnderflow=-100,kDivideByZero=-105,kBadType=-97,kBadOpcode=-101};
I32 si(Word x){I32 y;std::memcpy(&y,&x,4);return y;}
Word bits(I32 x){Word y;std::memcpy(&y,&x,4);return y;}
I32 asr(Word x,unsigned n){return si(x)<0?si(~(~x>>n)):si(x>>n);}
I32 asr2(Word x){return asr(x,2);}
I32 s16(Word x){return si((x&0x8000)?(x|0xffff0000):(x&0xffff));}
I32 s16_asr(Word x,unsigned n){return asr(bits(s16(x)),n);}   // 16-bit SAR then MOVSX
Word be32(Runtime& r,Word a){Word x=r.memory.read8(a);for(Word i=1;i<4;++i)x=(x<<8)|r.memory.read8(a+i);return x;}
Word be16(Runtime& r,Word a){return (Word(r.memory.read8(a))<<8)|r.memory.read8(a+1);}
Word get(Runtime& r,Word off){return r.memory.read(r.internal+off);}
void set(Runtime& r,Word off,Word x){r.memory.write(r.internal+off,x);}
void trim(Runtime&r,I32 d){r.ports.trim(r.internal+0x1ac,d);}
Word push_int(Runtime&r,Word x){return r.ports.push(r.internal,x,3);}
Word push_float(Runtime&r,Word x){return r.ports.push(r.internal,x,4);}
bool pop_int(Runtime&r,Word&x){return r.ports.pop(r.internal,&x,3)!=0;}
bool pop_float(Runtime&r,Word&x){return r.ports.pop(r.internal,&x,4)!=0;}
I32 depth(Runtime&r){return si(get(r,0x1ac));}
I32 tag_at(Runtime&r,I32 i){const Word t=r.memory.read8(get(r,0x1b8)+bits(i));return (t&0x80)?si(t|0xffffff00):si(t);}
Word word_at(Runtime&r,I32 i){return r.memory.read(get(r,0x1bc)+4*bits(i));}
void zero_vector(Runtime&r,Word v){r.memory.write(v,0);r.memory.write(v+4,0);r.memory.write(v+8,0);}
Word push_vector_at(Runtime&r,Word v){return push_vector_005d18b0(r,r.memory.read(v),r.memory.read(v+4),r.memory.read(v+8));}
}
Word push_return_005d17f0(Runtime& r,Word position){
 const Word index=get(r,0x1c4);set(r,0x1c8+4*index,position);
 const Word next=get(r,0x1c4)+1;set(r,0x1c4,next);return si(next)<128;
}
Word pop_return_005d17b0(Runtime& r,Word& position){
 const Word next=get(r,0x1c4)-1;set(r,0x1c4,next);
 if(si(next)<0){set(r,0x1c4,0);return 0;}
 position=get(r,0x1c8+4*next);return 1;
}
// 005D18B0: three AddToTopOfStack(4)+store sequences, identical to three 005D1880 calls.
Word push_vector_005d18b0(Runtime& r,Word x,Word y,Word z){push_float(r,x);push_float(r,y);push_float(r,z);return 1;}
// 005D2560: z leaves through FLD/FSTP (SNaN quieting preserved via float_reload_x87);
// y and x use StackPopFloat. Partial pops are kept; only full success returns 1.
Word pop_vector_005d2560(Runtime& r,Word out){
 const I32 d=depth(r);
 if(d<=0||r.memory.read8(get(r,0x1b8)+bits(d-1))!=4)return 0;
 const Word z=r.ports.float_reload_x87(word_at(r,d-1));
 trim(r,d-1);
 r.memory.write(out+8,z);
 Word v=0;
 if(!pop_float(r,v))return 0;
 r.memory.write(out+4,v);
 if(!pop_float(r,v))return 0;
 r.memory.write(out,v);return 1;
}
// 005D19C0: implementer copy (vtbl+20) BEFORE AddToTopOfStack(type+0x10); null copy still appended.
Word push_engine_structure_005d19c0(Runtime& r,Word type,Word value){
 const Word implementer=get(r,0x3d4);
 if(!implementer)return 0;
 const Word copied=r.ports.implementer_copy_slot8(implementer,type,value);
 r.ports.add_to_top_005d15a0(r.internal+0x1ac,type+0x10);
 r.memory.write(get(r,0x1bc)+4*(get(r,0x1ac)-1),copied);
 return 1;
}
// 005D2630: depth>0, top tag (signed) == type+0x10, implementer present; copy via vtbl+20, then SetStackPointer(top).
Word pop_engine_structure_005d2630(Runtime& r,Word type,Word& out){
 const I32 d=depth(r);
 if(d<=0)return 0;
 if(tag_at(r,d-1)!=si(type+0x10))return 0;
 const Word value=word_at(r,d-1);
 const Word implementer=get(r,0x3d4);
 if(!implementer)return 0;
 out=r.ports.implementer_copy_slot8(implementer,type,value);
 trim(r,d-1);
 return 1;
}
// 005D16C0: no upper bound in the original. An index past the tag/word
// allocations (capacity +1B4) would be an out-of-bounds access there.
void modify_stack_integer_005d16c0(Runtime& r,I32 index,Word delta){
 if(index<0||!get(r,0x1c0))return;
 if(bits(index)>=get(r,0x1b4))throw CoverageStop("modifyStackInteger index beyond original stack allocation",0x005d16d2);
 if(r.memory.read8(get(r,0x1b8)+bits(index))!=3)return;
 const Word a=get(r,0x1bc)+4*bits(index);
 r.memory.write(a,r.memory.read(a)+delta);
}
I32 execute_code_005d2bd0(Runtime& r,Word& pc,Word code,Word size){
 while(pc!=0xffffffff){
  if(si(pc+1)>=si(size))return kNoCodeLeft;
  const Word instructions=get(r,0x24)+1;set(r,0x24,instructions);
  if(si(instructions)>=0x20000)return kInstructionLimit;
  const Word op=r.memory.read8(code+pc),type=r.memory.read8(code+pc+1);
  if(op<1||op>0x2c)return kBadOpcode;
  Word a=0,b=0;
  switch(op){
  case 0x01:case 0x26:{ // CPDOWNSP / CPDOWNBP
   const Word offset=be32(r,code+pc+2);
   const I32 base=op==0x01?depth(r):si(get(r,0x1b0));
   const I32 start=si(bits(base)-bits(asr2(0-offset)));
   const I32 count=s16_asr(be16(r,code+pc+6),2);
   if(start<0)return kStackUnderflow;
   for(I32 i=0;i<count;++i)r.ports.copy(r.internal,si(bits(depth(r))-bits(count)+bits(i)),si(bits(start)+bits(i)));
   pc+=8;break;
  }
  case 0x02: // RSADD
   if(type==3){if(!push_int(r,0))return kPushFailed;}
   else if(type==4){if(!push_float(r,0))return kPushFailed;}
   else if(type==5){r.ports.string_chars_005e5a90(r.scratch.string_a,0x0073d71c);const Word ok=r.ports.push_string(r.internal,r.scratch.string_a);r.ports.string_destroy_005e5c20(r.scratch.string_a);if(!ok)return kPushFailed;}
   else if(type==6){if(!r.ports.push(r.internal,0x7f000000,6))return kPushFailed;}
   else if(type>=0x10&&type<=0x19){if(!push_engine_structure_005d19c0(r,type-0x10,0))return kPushFailed;}
   else return kBadType;
   pc+=2;break;
  case 0x03:case 0x27:{ // CPTOPSP / CPTOPBP
   const Word offset=be32(r,code+pc+2);
   const I32 base=op==0x03?depth(r):si(get(r,0x1b0));
   const I32 start=si(bits(base)-bits(asr2(0-offset)));
   const I32 count=s16_asr(be16(r,code+pc+6),2);
   if(start<0)return kStackUnderflow;
   const Word delta=bits(depth(r))-bits(start);
   for(I32 i=0;i<count;++i){const I32 source=si(bits(start)+bits(i));r.ports.copy(r.internal,source,si(bits(source)+delta));}
   pc+=8;break;
  }
  case 0x04: // CONST
   if(type==3){if(!push_int(r,be32(r,code+pc+2)))return kPushFailed;pc+=6;}
   else if(type==6){
    const Word value=be32(r,code+pc+2);Word object=0x7f000000;
    if(value==0){const Word frame=get(r,0x28);if(get(r,0x16c+4*frame)==1)object=get(r,0x18c+4*frame);}
    if(!r.ports.push(r.internal,object,6))return kPushFailed;pc+=6;
   }
   else if(type==4){if(!push_float(r,be32(r,code+pc+2)))return kPushFailed;pc+=6;}
   else if(type==5){
    r.ports.string_default_005b3190(r.scratch.string_a);
    const I32 length=s16(be16(r,code+pc+2));
    if(length<0){r.ports.string_destroy_005e5c20(r.scratch.string_a);throw CoverageStop("CONST string with negative length (original writes before its allocation)",0x005d3062);}
    const Word buffer=r.ports.allocate_006fa7e6(bits(length)+1);
    I32 copied=0;for(;copied<length;++copied)r.ports.byte(buffer+bits(copied),r.memory.read8(code+pc+bits(copied)+4));
    r.ports.byte(buffer+bits(length),0);
    r.ports.string_format_005e5680(r.scratch.string_a,0x0073d5d0,buffer);
    r.ports.free_006fa390(buffer);
    const Word ok=r.ports.push_string(r.internal,r.scratch.string_a);
    if(!ok){r.ports.string_destroy_005e5c20(r.scratch.string_a);return kPushFailed;}
    pc+=bits(copied)+4;r.ports.string_destroy_005e5c20(r.scratch.string_a);
   }
   else return kBadType;
   break;
  case 0x05:{ // ACTION: implementer vtbl+08 (RunCommand -> 0052C0D0 table dispatch)
   const I32 routine=s16(be16(r,code+pc+2));const Word argc=r.memory.read8(code+pc+4);
   const Word implementer=get(r,0x3d4);if(!implementer)return kNoImplementer;
   const I32 status=r.ports.action_slot2(implementer,routine,argc);
   if(status<0)return status;
   pc+=5;break;
  }
  case 0x06:case 0x07:case 0x08:case 0x09:case 0x0a:{ // LOGAND LOGOR INCOR EXCOR BOOLAND (II)
   if(type!=0x20)return kBadType;
   if(!pop_int(r,a)||!pop_int(r,b))return kStackUnderflow; // a=top, b=lower
   Word v=0;
   if(op==0x06)v=(b!=0&&a!=0);else if(op==0x07)v=(b!=0||a!=0);else if(op==0x08)v=b|a;else if(op==0x09)v=b^a;else v=b&a;
   push_int(r,v);pc+=2;break;
  }
  case 0x0b:case 0x0c:{ // EQUAL / NEQUAL
   if(!((type>=0x20&&type<=0x24)||(type>=0x30&&type<=0x39)))return kBadType;
   const Word size=type==0x24?be16(r,code+pc+2):4;
   I32 d=depth(r);
   if(d<asr(bits(s16(size)),1))return kStackUnderflow;
   const I32 count=s16_asr(size,2);
   Word flag=1;Word lower_off=bits(d-count)*4;
   for(I32 i=0;i<count;++i){
    const I32 tag=tag_at(r,d-1);--d;lower_off-=4;
    const Word top=word_at(r,d);const Word lower=r.memory.read(get(r,0x1bc)+lower_off);
    if(tag==3||tag==4||tag==6){if(top!=lower)flag=0;}
    else if(tag==5){if(!r.ports.compare_no_case_005e6450(top,lower))flag=0;}
    else if(tag>=0x10&&tag<=0x19){
     const Word implementer=get(r,0x3d4);if(!implementer)return kNoImplementer;
     if(!r.ports.implementer_equal_slot7(implementer,bits(tag-0x10),top,lower))flag=0;
    } else return kBadEqualityTag;
   }
   trim(r,si(get(r,0x1ac)-bits(count)*2));
   push_int(r,op==0x0b?flag:Word(flag==0));
   pc+=type==0x24?4:2;break;
  }
  case 0x0d:case 0x0e:case 0x0f:case 0x10: // GEQ GT LT LEQ
   if(type==0x20){
    if(!pop_int(r,a)||!pop_int(r,b))return kStackUnderflow;
    const I32 x=si(b),y=si(a);
    push_int(r,op==0x0d?x>=y:op==0x0e?x>y:op==0x0f?x<y:x<=y);
   } else if(type==0x21){
    if(!pop_float(r,a)||!pop_float(r,b))return kStackUnderflow;
    push_int(r,r.ports.float_compare_x87(op,b,a));
   } else return kBadType;
   pc+=2;break;
  case 0x11:case 0x12:case 0x13: // SHLEFT SHRIGHT USHRIGHT (II); CL masks to 5 bits
   if(type!=0x20)return kBadType;
   if(!pop_int(r,a)||!pop_int(r,b))return kStackUnderflow;
   if(op==0x11)push_int(r,b<<(a&31));
   else if(op==0x12)push_int(r,si(b)<0?0-bits(asr(0-b,a&31)):bits(asr(b,a&31)));
   else push_int(r,bits(asr(b,a&31))); // original USHRIGHT uses SAR
   pc+=2;break;
  case 0x14:case 0x15:case 0x16:case 0x17: // ADD SUB MUL DIV
   if(type==0x20){
    if(!pop_int(r,a)||!pop_int(r,b))return kStackUnderflow;
    if(op==0x14)push_int(r,b+a);else if(op==0x15)push_int(r,b-a);else if(op==0x16)push_int(r,b*a);
    else{
     if(a==0)return kDivideByZero;
     if(b==0x80000000&&a==0xffffffff)throw CoverageStop("DIVII INT_MIN/-1 (original IDIV raises #DE)",0x005d377f);
     push_int(r,bits(si(b)/si(a)));
    }
   } else if(type==0x21||type==0x25||type==0x26){
    if(type==0x26){if(!pop_int(r,a))return kStackUnderflow;a=r.ports.int_to_float_x87(a);}
    else if(!pop_float(r,a))return kStackUnderflow;
    if(type==0x25){if(!pop_int(r,b))return kStackUnderflow;b=r.ports.int_to_float_x87(b);}
    else if(!pop_float(r,b))return kStackUnderflow;
    if(op==0x17&&r.ports.divisor_is_zero_x87(a))return kDivideByZero;
    push_float(r,r.ports.float_binary_x87(op,b,a));
   } else if(type==0x23){
    r.ports.string_default_005b3190(r.scratch.string_a);
    if(!r.ports.pop_string(r.internal,r.scratch.string_a)){r.ports.string_destroy_005e5c20(r.scratch.string_a);return kStackUnderflow;}
    r.ports.string_default_005b3190(r.scratch.string_b);
    if(!r.ports.pop_string(r.internal,r.scratch.string_b)){r.ports.string_destroy_005e5c20(r.scratch.string_b);r.ports.string_destroy_005e5c20(r.scratch.string_a);return kStackUnderflow;}
    if(op==0x14){
     const Word joined=r.ports.string_concat_005e5d10(r.scratch.string_b,r.scratch.string_result,r.scratch.string_a);
     r.ports.push_string(r.internal,joined);
     r.ports.string_destroy_005e5c20(r.scratch.string_result);
    }
    pc+=2;r.ports.string_destroy_005e5c20(r.scratch.string_b);r.ports.string_destroy_005e5c20(r.scratch.string_a);
    break;
   } else if(type==0x3a){
    const Word va=r.scratch.vector_a,vb=r.scratch.vector_b;
    zero_vector(r,va);if(!pop_vector_005d2560(r,va))return kStackUnderflow;
    zero_vector(r,vb);if(!pop_vector_005d2560(r,vb))return kStackUnderflow;
    if(op==0x14||op==0x15){
     const Word x=r.ports.float_binary_x87(op,r.memory.read(vb),r.memory.read(va));
     const Word y=r.ports.float_binary_x87(op,r.memory.read(vb+4),r.memory.read(va+4));
     const Word z=r.ports.float_binary_x87(op,r.memory.read(vb+8),r.memory.read(va+8));
     push_vector_005d18b0(r,x,y,z);
    } // MUL/DIV VV: both vectors consumed, nothing pushed (original falls to 005D4118)
   } else if(type==0x3b||type==0x3c){
    const Word v=r.scratch.vector_a,out=r.scratch.vector_result;zero_vector(r,v);Word f=0;
    if(type==0x3b){if(!pop_float(r,f))return kStackUnderflow;if(!pop_vector_005d2560(r,v))return kStackUnderflow;}
    else{if(!pop_vector_005d2560(r,v))return kStackUnderflow;if(!pop_float(r,f))return kStackUnderflow;}
    if(op==0x16){r.ports.vector_component_mult_00423bf0(out,v,f);push_vector_at(r,out);}
    else if(op==0x17){
     if(r.ports.divisor_is_zero_x87(f))return kDivideByZero;
     r.ports.vector_divide_00437a90(out,v,f);push_vector_at(r,out);
    } // ADD/SUB VF/FV: operands consumed, nothing pushed
   } else return kBadType;
   pc+=2;break;
  case 0x18: // MOD II
   if(type!=0x20)return kBadType;
   if(!pop_int(r,a)||!pop_int(r,b))return kStackUnderflow;
   if(a==0)return kDivideByZero;
   if(b==0x80000000&&a==0xffffffff)throw CoverageStop("MODII INT_MIN%-1 (original IDIV raises #DE)",0x005d3c13);
   push_int(r,bits(si(b)%si(a)));pc+=2;break;
  case 0x19: // NEG
   if(type==3){if(!pop_int(r,a))return kStackUnderflow;push_int(r,0-a);}
   else if(type==4){if(!pop_float(r,a))return kStackUnderflow;push_float(r,r.ports.float_negate_x87(a));}
   else return kBadType;
   pc+=2;break;
  case 0x1a: // COMP
   if(type!=3)return kBadType;
   if(!pop_int(r,a))return kStackUnderflow;push_int(r,~a);pc+=2;break;
  case 0x1b: // MOVSP
   a=be32(r,code+pc+2);trim(r,si(bits(depth(r))-bits(asr2(0-a))));pc+=6;break;
  case 0x1c: // STORE_STATEALL: resume = pc + type byte
   set(r,0x3c8,pc+type);set(r,0x3d0,0);set(r,0x3cc,0);pc+=2;break;
  case 0x1d: // JMP
   pc+=be32(r,code+pc+2);break;
  case 0x1e:{ // JSR
   const Word displacement=be32(r,code+pc+2);pc+=6;
   if(!push_return_005d17f0(r,pc))return kPushFailed;
   pc+=displacement-6;break;
  }
  case 0x1f: // JZ
   if(!pop_int(r,a))return kStackUnderflow;
   if(a)pc+=6;else pc+=be32(r,code+pc+2);break;
  case 0x20: // RETN
   if(!pop_return_005d17b0(r,pc))return kStackUnderflow;break;
  case 0x21:{ // DESTRUCT
   const I32 total=s16_asr(be16(r,code+pc+2),2);
   const I32 keep_offset=s16_asr(be16(r,code+pc+4),2);
   const I32 keep_size=s16_asr(be16(r,code+pc+6),2);
   I32 d=depth(r);
   if(keep_offset+keep_size<total){d=si(bits(d)+bits(keep_offset-total+keep_size));trim(r,d);}
   const I32 end=si(bits(d)-bits(keep_offset));
   for(I32 i=si(bits(d)-bits(keep_offset)-bits(keep_size));i<end;++i)r.ports.copy(r.internal,si(bits(keep_offset)+bits(i)),i);
   if(keep_offset>0)trim(r,si(get(r,0x1ac)-bits(keep_offset)));
   pc+=8;break;
  }
  case 0x22: // NOT
   if(type!=3)return kBadType;
   if(!pop_int(r,a))return kStackUnderflow;push_int(r,a==0);pc+=2;break;
  case 0x23:case 0x24:case 0x28:case 0x29:{ // DECISP INCISP DECIBP INCIBP
   if(type==3){
    const Word offset=be32(r,code+pc+2);
    const Word base=(op==0x23||op==0x24)?get(r,0x1ac):get(r,0x1b0);
    const I32 index=si(base-bits(asr2(0-offset)));
    if(index<0)return kStackUnderflow;
    modify_stack_integer_005d16c0(r,index,(op==0x24||op==0x29)?1u:0xffffffffu);
   } // other types: no effect, still advance 6 (005D402D -> 005D40D4)
   pc+=6;break;
  }
  case 0x25: // JNZ
   if(!pop_int(r,a))return kStackUnderflow;
   if(!a)pc+=6;else pc+=be32(r,code+pc+2);break;
  case 0x2a:{ // SAVEBP
   const Word old_base=get(r,0x1b0);a=get(r,0x1ac);push_int(r,old_base);set(r,0x1b0,a);pc+=2;break;
  }
  case 0x2b: // RESTOREBP
   if(!pop_int(r,a))return kStackUnderflow;set(r,0x1b0,a);pc+=2;break;
  case 0x2c: // STORE_STATE
   set(r,0x3c8,pc+type);set(r,0x3cc,be32(r,code+pc+6));set(r,0x3d0,be32(r,code+pc+2));pc+=10;break;
  default:return kBadOpcode;
  }
 }
 return kOk;
}
I32 run_script_file_005d4270(Runtime& r,Word initial){
 const Word old_calls=get(r,0x1c4);const I32 old_depth=depth(r);
 set(r,0x1c,0);set(r,0x20,0);
 set(r,0x1c8+4*old_calls,0xffffffff);
 const Word next=get(r,0x1c4)+1;
 const Word frame=r.internal+40*get(r,0x28);
 set(r,0x1c4,next);
 const Word size=r.memory.read(frame+0x40),code=r.memory.read(frame+0x3c);
 const I32 result=execute_code_005d2bd0(r,initial,code,size);
 if(result<0||old_calls!=get(r,0x1c4)){
  const Word name=r.internal+40*get(r,0x28)+0x44;
  const Word chars=r.ports.string_cstr_005e5670(name);
  r.ports.string_chars_005e5a90(r.scratch.string_a,chars);
  r.ports.error_slot4(get(r,0x3d4),r.scratch.string_a,0-bits(result));
  if(depth(r)>old_depth)trim(r,old_depth);else set(r,0x1ac,bits(old_depth));
  set(r,0x1c4,old_calls);r.ports.string_destroy_005e5c20(r.scratch.string_a);
 }
 return result;
}
I32 execute_command_0052c0d0(Memory& m,DispatchPorts& p,Word self,I32 routine,Word count){
 if(routine<772){const Word function=m.read(m.read(self+0xc)+4*bits(routine));if(function)return p.invoke(function,self,routine,count);}
 return -2002;
}
}
#include "3e4c4bbc2c-vm_action_names.inc"
