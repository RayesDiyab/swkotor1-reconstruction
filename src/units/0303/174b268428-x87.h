#pragma once
#include "a45128e337-contracts.h"
namespace gui_wrap_draw {
// Local arithmetic carrier only. No original/shared object, address or provider
// ownership. TBYTE spills preserve x87 extended values across authored helpers.
struct Extended {U8 bytes[10];};
inline Extended F(float value){Extended result;__asm {
fld value
fstp tbyte ptr result
}return result;}
inline Extended I(I32 value){Extended result;__asm {
fild value
fstp tbyte ptr result
}return result;}
inline Extended Add(Extended a,Extended b){Extended result;__asm {
fld tbyte ptr a
fld tbyte ptr b
faddp st(1),st
fstp tbyte ptr result
}return result;}
inline Extended Sub(Extended a,Extended b){Extended result;__asm {
fld tbyte ptr b
fld tbyte ptr a
fsub st,st(1)
fstp tbyte ptr result
fstp st(0)
}return result;}
inline Extended Mul(Extended a,Extended b){Extended result;__asm {
fld tbyte ptr a
fld tbyte ptr b
fmulp st(1),st
fstp tbyte ptr result
}return result;}
inline Extended Div(Extended a,Extended b){Extended result;__asm {
fld tbyte ptr b
fld tbyte ptr a
fdiv st,st(1)
fstp tbyte ptr result
fstp st(0)
}return result;}
inline float Rounded(Extended a){float result;__asm {
fld tbyte ptr a
fstp result
}return result;}
inline U16 Status(Extended a,Extended b){U16 result;__asm {
fld tbyte ptr b
fld tbyte ptr a
fcom st(1)
fnstsw ax
mov result,ax
fstp st(0)
fstp st(0)
}return result;}
inline bool Less(Extended a,Extended b){return (Status(a,b)&0x0500u)==0x0100u;}
inline bool Greater(Extended a,Extended b){return (Status(a,b)&0x4100u)==0;}
inline I32 Convert(Extended a){auto target=bindings.ftol_006fae8c;I32 result;__asm {
fld tbyte ptr a
call target
mov result,eax
}return result;}
inline Extended ViewportAlpha(){auto target=bindings.viewport_alpha_00458ef0;Extended result;__asm {
call target
fstp tbyte ptr result
}return result;}
inline void* FontInfo(void* texture){using Fn=void*(__thiscall*)(void*);return reinterpret_cast<Fn>(PointerAt(PointerAt(texture,0),0x38))(texture);}
inline U32 NextCapacity(U32 v){return v?v+v:8u;}
inline void Append(void* array,U32 value){U32 count=WordAt(array,4),capacity=WordAt(array,8);if(count==capacity)bindings.array_allocate_005e03c0(static_cast<menu_list_growth::List*>(array),nullptr,NextCapacity(capacity));count=WordAt(array,4);StoreWord(PointerAt(array,0),count*4,value);StoreWord(array,4,count+1u);}
inline U32 Distance(const U8* a,const U8* b){return static_cast<U32>(reinterpret_cast<U32>(a)-reinterpret_cast<U32>(b));}
inline U32 Length(const U8* p){U32 n=0;while(*p++){++n;}return n;}
inline Extended Width(void* self,void* info,U8 c,bool first,bool quarter,bool negative=false){
 unsigned offset=static_cast<unsigned>(c)*12;Extended value=Sub(F(FloatAt(PointerAt(info,0x24),offset)),F(FloatAt(PointerAt(info,0x18),offset)));
 if(first)value=Mul(Mul(value,F(FloatAt(self,0x40))),F(FloatAt(info,0x0c)));
 else value=Mul(Add(Mul(value,F(FloatAt(info,0x0c))),F(FloatAt(info,0x10))),F(FloatAt(self,0x40)));
 value=Mul(value,F(negative?*bindings.negative_hundred_0074186c:*bindings.hundred_00741838));return quarter?Add(value,F(*bindings.quarter_0073edf0)):value;
}
}
