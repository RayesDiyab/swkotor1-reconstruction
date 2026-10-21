// lane-m54-fonttex replacement of unit gui-draw (0045A850 CAurGUIStringInternal::Draw); base M53 draw.cpp 8f6a14e1.
// F1 visible-row test polarity (0045AB2E..0045AB3C). F2 vertical advances use the height-based scale [B+30]
// (0045AAA8, 0045AB26, 0045AB63, 0045B1A3). F3 hyphen quad: y_high=1-y, y_low recomputed, vertex 3=(x_next,y_low).
#include "c9999ef6eb-x87.h"
namespace gui_wrap_draw {
namespace {
I32 Viewport(unsigned axis){I16 result;I32 index=*bindings.viewport_index_007b9460;std::memcpy(&result,bindings.viewport_dimensions_007b946c+index*10+axis,2);return result;}
Extended Scale(void* self,void* info,unsigned offset){return Mul(Mul(F(FloatAt(info,offset)),F(FloatAt(self,0x40))),F(*bindings.hundred_00741838));}
float Line(void* self,void* info){return Rounded(Scale(self,info,4));}
I32 Channel(Extended value){float stored=Rounded(Mul(value,F(*bindings.channel_0073ede4)));I32 result;__asm {
fld stored
frndint
fistp result
}return result;}
void SkipRow(U8*& cursor,U32 length){I32 signed_length=Signed(length);cursor+=signed_length<0?Signed(0u-length):signed_length;if(*cursor=='\n'||*cursor==' ')++cursor;}
float GlyphCoordinate(void* info,U8 c,bool right,unsigned axis){return FloatAt(PointerAt(info,right?0x24:0x18),static_cast<unsigned>(c)*12+axis*4);}
void TextureCorner(decltype(&::glTexCoord2f) target,void* info,U8* cursor,bool right_x,bool right_y){
 float y=Rounded(Sub(F(GlyphCoordinate(info,*cursor,right_y,1)),F(*bindings.epsilon_00741870)));
 float x=Rounded(Sub(F(GlyphCoordinate(info,*cursor,right_x,0)),F(*bindings.epsilon_00741870)));
 target(x,y);
}
}
void __fastcall draw_0045a850(void* self,void*,U32 alpha_bits){
 U8* initial=static_cast<U8*>(PointerAt(self,0x14));
 if(!Length(initial))return;
 if(*bindings.busy_007bb4d0)return;
 (*bindings.disable_0073d22c)(*bindings.disable_0078d45c);
 *bindings.reset_007a689c=0;
 if(!WordAt(self,4)&&!WordAt(self,0x0c))return; // Original leaves the disable in effect on this path.
 void* info=FontInfo(PointerAt(self,0x18));
 Extended numerator=Scale(self,info,0x0c);
 float x_scale=Rounded(Div(numerator,I(Viewport(0))));
 float viewport_height=Rounded(I(Viewport(2)));
 float y_scale=Rounded(Div(numerator,F(viewport_height)));
 float y=Rounded(Div(I(Signed(WordAt(self,8))),F(viewport_height)));
 U8* cursor=static_cast<U8*>(PointerAt(self,0x14));U32 row=0;
 U8 vertical=static_cast<U8*>(self)[0x46]&0x38u;
 if(vertical==0x10u){
  float line_height=Line(self,info);
  Extended gap=Div(Sub(I(Signed(WordAt(self,0x10))),Mul(I(Signed(WordAt(self,0x38))),F(line_height))),Add(F(viewport_height),F(viewport_height)));
  if(Less(gap,F(*bindings.zero_0073d700))){
   Extended step=Div(F(line_height),F(viewport_height));U8* lengths=static_cast<U8*>(PointerAt(self,0x34));
   do {gap=Add(gap,step);SkipRow(cursor,WordAt(lengths,row*4));++row;}while(Less(gap,F(*bindings.zero_0073d700)));
  }
  y=Rounded(Add(gap,F(y)));
 }else if(vertical==0x20u){
  float line_height=Line(self,info);Extended total=Mul(I(Signed(WordAt(self,0x38))),F(line_height));Extended height=I(Signed(WordAt(self,0x10)));
  if(Greater(total,height)){U8* lengths=static_cast<U8*>(PointerAt(self,0x34));do {total=Sub(total,F(line_height));SkipRow(cursor,WordAt(lengths,row*4));++row;}while(Greater(total,height));}
  y=Rounded(Add(Div(Sub(height,total),F(viewport_height)),F(y)));
 }
 using TextureUse=void(__thiscall*)(void*);void* texture=PointerAt(self,0x18);
 reinterpret_cast<TextureUse>(PointerAt(PointerAt(texture,0),0xb4))(texture);
 (*bindings.begin_0073d2d0)(*bindings.quads_0073f34c);
 if(Signed(row)<Signed(WordAt(self,0x38))){
  auto vertex=*bindings.vertex_0073d324;auto texcoord=*bindings.texcoord_0073d2cc;
  do {
   U32* lengths=static_cast<U32*>(PointerAt(self,0x34));
   if(lengths[row]==0){
    float spacer=Rounded(Div(Scale(self,info,0x14),I(Viewport(2))));
    do {
     Extended delta=Sub(Add(F(spacer),F(GlyphCoordinate(info,*cursor,false,1))),F(GlyphCoordinate(info,*cursor,true,1)));
     y=Rounded(Add(Mul(delta,F(y_scale)),F(y)));++cursor;++row; // F2 0045AAA8 fmul [esp+40h] = height-based scale
     if(row==WordAt(self,0x38))goto finished;
    }while(lengths[row]==0);
   }
   if(row==WordAt(self,0x38))break;
   Extended spacer=Div(Scale(self,info,0x14),I(Viewport(2)));
   Extended baseline=Add(Mul(Sub(Add(spacer,F(GlyphCoordinate(info,*cursor,false,1))),F(GlyphCoordinate(info,*cursor,true,1))),F(y_scale)),F(y)); // F2 0045AB15..0045AB2A
   if(Less(baseline,F(*bindings.zero_0073d700))){ // F1 0045AB2E..0045AB3C: <0 -> skip row (fallthrough 0045AB3E); >=0 -> draw (jp 0045AB74)
    cursor+=Signed(WordAt(PointerAt(self,0x34),row*4)); // Original adds negative lengths without abs on this skip path.
    if(*cursor=='\n'||*cursor==' ')++cursor;
    y=Rounded(Add(Mul(Sub(Add(spacer,F(GlyphCoordinate(info,*cursor,false,1))),F(GlyphCoordinate(info,*cursor,true,1))),F(y_scale)),F(y))); // F2 0045AB58..0045AB6B
    goto next_row;
   }
   {
    U32 encoded_length=WordAt(PointerAt(self,0x34),row*4);bool hyphen=Signed(encoded_length)<0;U32 remaining=hyphen?0u-encoded_length:encoded_length;
    unsigned glyph_offset=static_cast<unsigned>(*cursor)*12;
    float x=Rounded(Div(I(Signed(WordAt(self,4))),I(Viewport(0))));
    if(!WordAt(self,0x2c)){
     U8* all=static_cast<U8*>(PointerAt(self,0x14));U32 measured=0;
     if(*all){ // Original measures one cached glyph once and subtracts the same result for every source character.
      Extended value=Sub(F(FloatAt(PointerAt(info,0x24),glyph_offset)),F(FloatAt(PointerAt(info,0x18),glyph_offset)));
      I32 step=Convert(Mul(Mul(Add(Mul(value,F(FloatAt(info,0xc))),F(FloatAt(info,0x10))),F(FloatAt(self,0x40))),F(*bindings.negative_hundred_0074186c)));
      do {measured-=static_cast<U32>(step);}while(*++all);
     }
     I32 spacing=Convert(Mul(Mul(F(FloatAt(info,0x10)),F(FloatAt(self,0x40))),F(*bindings.hundred_00741838)));
     bindings.array_add_00444fe0(reinterpret_cast<menu_list_growth::List*>(static_cast<U8*>(self)+0x28),nullptr,ShortSigned(measured-static_cast<U32>(spacing)));
    }
    U8 horizontal=static_cast<U8*>(self)[0x46]&7u;
    if(horizontal==2u || horizontal==4u){U32 margin=WordAt(self,0xc)-WordAt(PointerAt(self,0x28),row*4);Extended divisor=I(Viewport(0));if(horizontal==2u)divisor=Add(divisor,divisor);x=Rounded(Add(Div(I(Signed(margin)),divisor),F(x)));}
    I32 bottom=Signed(WordAt(self,8)+WordAt(self,0x10));
    if(Less(Div(I(bottom),I(Viewport(2))),F(y))){row=WordAt(self,0x38);goto next_row;}
    U32 r=static_cast<U32>(Channel(F(FloatAt(self,0x1c))));U32 g=static_cast<U32>(Channel(F(FloatAt(self,0x20))));U32 b=static_cast<U32>(Channel(F(FloatAt(self,0x24))));
    float alpha;std::memcpy(&alpha,&alpha_bits,4);U32 a=static_cast<U32>(Channel(Mul(ViewportAlpha(),F(alpha))));
    U32 packed=((((a<<8)|b)<<8)|g)<<8|r;U32 white=(a<<24)|0x00ffffffu;
    float x_next=x,y_low=y,y_high=y;
    if(Signed(remaining)>0){
     y_high=Rounded(Sub(F(*bindings.one_0073d764),F(y)));
     do {
      Extended width=Sub(F(GlyphCoordinate(info,*cursor,true,0)),F(GlyphCoordinate(info,*cursor,false,0)));x_next=Rounded(Add(Mul(width,F(x_scale)),F(x)));
      y_low=Rounded(Sub(F(*bindings.one_0073d764),Add(Div(Mul(Mul(F(FloatAt(self,0x40)),F(FloatAt(info,4))),F(*bindings.hundred_00741838)),I(Viewport(2))),F(y))));
      U32 color=*cursor<0x20?white:packed;(*bindings.color_0073d2e8)(reinterpret_cast<const GLubyte*>(&color));
      TextureCorner(texcoord,info,cursor,false,false);vertex(x,y_high,0.0f,1.0f);
      TextureCorner(texcoord,info,cursor,false,true);vertex(x,y_low,0.0f,1.0f);
      TextureCorner(texcoord,info,cursor,true,true);vertex(x_next,y_low,0.0f,1.0f);
      TextureCorner(texcoord,info,cursor,true,false);vertex(x_next,y_high,0.0f,1.0f);
      Extended spacing=Div(Mul(Mul(F(FloatAt(self,0x40)),F(FloatAt(info,0x10))),F(*bindings.hundred_00741838)),I(Viewport(0)));
      width=Sub(Add(spacing,F(GlyphCoordinate(info,*cursor,true,0))),F(GlyphCoordinate(info,*cursor,false,0)));
      ++cursor;--remaining;x=Rounded(Add(Mul(width,F(x_scale)),F(x)));
     }while(remaining);
    }
    if(*cursor=='\n'||*cursor==' ')++cursor;
    if(hyphen){
     x_next=Rounded(Add(Mul(Sub(F(GlyphCoordinate(info,'-',true,0)),F(GlyphCoordinate(info,'-',false,0))),F(x_scale)),F(x))); // 0045AFE1..0045B009 [B+2C]
     y_high=Rounded(Sub(F(*bindings.one_0073d764),F(y)));                                    // F3 0045B00D..0045B017 [B+28]
     y_low=Rounded(Sub(F(*bindings.one_0073d764),Add(Div(Mul(Mul(F(FloatAt(self,0x40)),F(FloatAt(info,4))),F(*bindings.hundred_00741838)),I(Viewport(2))),F(y)))); // F3 0045B01B..0045B04A [B+24]
     (*bindings.color_0073d2e8)(reinterpret_cast<const GLubyte*>(&packed));U8 hyphen_glyph='-'; // 0045B03C..0045B04E
     TextureCorner(texcoord,info,&hyphen_glyph,false,false);vertex(x,y_high,0.0f,1.0f);      // 0045B081/0045B094
     TextureCorner(texcoord,info,&hyphen_glyph,false,true);vertex(x,y_low,0.0f,1.0f);        // 0045B0C6/0045B0D9
     TextureCorner(texcoord,info,&hyphen_glyph,true,true);vertex(x_next,y_low,0.0f,1.0f);    // F3 0045B108/0045B11B
     TextureCorner(texcoord,info,&hyphen_glyph,true,false);vertex(x_next,y_high,0.0f,1.0f);
    }
    Extended next=Add(Mul(Sub(Add(Div(Mul(Mul(F(FloatAt(self,0x40)),F(FloatAt(info,0x14))),F(*bindings.hundred_00741838)),I(Viewport(2))),F(GlyphCoordinate(info,*cursor,false,1))),F(GlyphCoordinate(info,*cursor,true,1))),F(y_scale)),F(y)); // F2 0045B162..0045B1A7
    y=Rounded(next);if(Greater(next,F(*bindings.one_0073d764)))break;
   }
next_row:
   ++row;
  }while(Signed(row)<Signed(WordAt(self,0x38)));
 }
finished:
 (*bindings.end_0073d2c4)();(*bindings.enable_0073d390)(*bindings.disable_0078d45c);
}
}
