#include "174b268428-x87.h"
namespace gui_wrap_draw {
namespace {
void NewlineAppend(void* array){
 U32 capacity=WordAt(array,8);
 if(WordAt(array,4)==capacity){
  void* old=PointerAt(array,0);capacity=NextCapacity(capacity);StoreWord(array,8,capacity);
  void* fresh=bindings.allocate_006fa7e6(capacity*4u);StorePointer(array,0,fresh);
  for(I32 i=0;i<Signed(WordAt(array,4));++i)StoreWord(PointerAt(array,0),static_cast<U32>(i)*4,WordAt(old,static_cast<U32>(i)*4));
  if(old)bindings.free_006fa390(old);
 }
 U32 count=WordAt(array,4);StoreWord(PointerAt(array,0),count*4,0);StoreWord(array,4,count+1u);
}
}
void __fastcall wrap_0045a2f0(void* self,void*,U32 limit_argument){
 if(!WordAt(self,4)&&!WordAt(self,0x0c))return;
 void* info=FontInfo(PointerAt(self,0x18));
 StoreWord(self,0x2c,0);StoreWord(self,0x38,0);
 U8* cursor=static_cast<U8*>(PointerAt(self,0x14));U8* line=cursor;
 void* captured_right=PointerAt(info,0x24);
 I32 first_full=Convert(Width(self,info,*cursor,true,false));
 I32 width=ShortSigned(static_cast<U32>(first_full)),break_width=first_full;
 U32 limit_bits=limit_argument;
 if(limit_argument==0xffffffffu)limit_bits=(limit_argument&0xffff0000u)|(WordAt(self,0x0c)&0xffffu);
 Extended height=Sub(Add(F(FloatAt(captured_right,0x534)),F(FloatAt(info,0x10))),F(FloatAt(PointerAt(info,0x18),0x534)));
 I32 height_full=Convert(Mul(Mul(Mul(height,F(FloatAt(self,0x40))),F(FloatAt(info,0x0c))),F(*bindings.hundred_00741838)));
 I32 limit=ShortSigned(limit_bits);
 U32 length=Length(line);
 if(length==1 && limit<ShortSigned(static_cast<U32>(height_full)))return;
 if(length<=2 && limit<ShortSigned(static_cast<U32>(height_full))*2)return;
 if(!*cursor)return;
 void* widths=static_cast<U8*>(self)+0x28;void* lengths=static_cast<U8*>(self)+0x34;
 do {
  U8* last_break=nullptr;
  while(*cursor=='\n'){++cursor;line=cursor;NewlineAppend(widths);NewlineAppend(lengths);}
  if(!*cursor)width=limit;
  else if(width<limit){
   do {
    if(*cursor=='-' && bindings.alpha_006fc99d(cursor[-1])!=0 && bindings.alpha_006fc99d(cursor[1])!=0){last_break=cursor;break_width=width;}
    ++cursor;
    if(!*cursor)goto whole_line;
    if(*cursor=='\n')break;
    width=Signed(static_cast<U32>(width)+static_cast<U32>(Convert(Width(self,info,*cursor,false,true))));
    if(*cursor==' '){last_break=cursor;break_width=width;}
   }while(width<limit);
  }
  if(!*cursor || *cursor=='\n'){
whole_line:
   Append(widths,static_cast<U32>(width));Append(lengths,Distance(cursor,line));if(*cursor=='\n')++cursor;
  }else if(!last_break){
   break_width=Signed(static_cast<U32>(width)-static_cast<U32>(Convert(Width(self,info,*cursor,false,false))));
   --cursor;
   if(cursor==PointerAt(self,0x14)){StoreWord(self,0x2c,0);StoreWord(self,0x38,0);return;}
   I32 previous=Convert(Width(self,info,*cursor,false,false));I32 hyphen=Convert(Width(self,info,'-',false,false));
   break_width=Signed(static_cast<U32>(break_width)+static_cast<U32>(hyphen)-static_cast<U32>(previous));
   Append(lengths,Distance(line,cursor));Append(widths,static_cast<U32>(ShortSigned(static_cast<U32>(break_width))));
  }else{
   if(*last_break==' '){Append(lengths,Distance(last_break,line));break_width=Signed(static_cast<U32>(break_width)+static_cast<U32>(Convert(Width(self,info,' ',false,false,true))));}
   else Append(lengths,Distance(last_break,line)+1u);
   Append(widths,static_cast<U32>(ShortSigned(static_cast<U32>(break_width))));cursor=last_break+1;
  }
  line=cursor;width=Convert(Width(self,info,*cursor,true,false));
 }while(*cursor);
}
}
