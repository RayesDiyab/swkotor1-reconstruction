#include "27b4f16387-framebuffer.h"
namespace kotor_renderer_init { namespace {
void* handle_word(FBWord bits){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(bits));}
void restore_live(FBCurrent current,const FrameBufferBindings& b){
 // Original paired live restore reads context first, then DC.
 void* context=handle_word(*b.main_context_007a6854);
 void* dc=handle_word(*b.main_dc_007a47e4);
 current(dc,context);
}
FBWord ramp(int index,int threshold,unsigned shift){
 if(index<threshold)return 0;int delta=index-threshold;FBWord converted_word;
 const float* factor=threshold==234?framebuffer.ramp22_0073f410:framebuffer.ramp40_0073f408;const float* scale=framebuffer.ramp_scale_0073f40c;
 __asm {fild delta} __asm {mov eax,factor} __asm {fmul dword ptr [eax]} __asm {mov eax,scale} __asm {fmul dword ptr [eax]} __asm {call unresolved_ftol2_006fae8c} __asm {mov converted_word,eax}
 return converted_word<<shift;
}
FBWord bloom(int index,bool first_order,unsigned shift){
 FBWord converted_word;const auto&b=framebuffer;const float* inv=b.inv255_0073ede0;const double* threshold=b.threshold_0073f3d0;
 const float* intensity=b.intensity_0078daec;const float* quad=b.quadratic_0073f404;const float* half=b.half_0073e9ac;const float* scale=b.scale255_0073ede4;
 __asm {
  fild index
  mov eax,inv
  fmul dword ptr [eax]
  mov eax,threshold
  fcom qword ptr [eax]
  fnstsw ax
  test ah,5
  jp root_case
  cmp first_order,0
  je square_first
  mov eax,intensity
  fld dword ptr [eax]
  fmul st,st(1)
  fmulp st(1),st
  jmp quadratic_case
 square_first:
  fld st(0)
  fmulp st(1),st
  mov eax,intensity
  fmul dword ptr [eax]
 quadratic_case:
  mov eax,quad
  fmul dword ptr [eax]
  jmp scale_case
 root_case:
  mov eax,half
  fsub dword ptr [eax]
  fmul dword ptr [eax]
  fsqrt
  fadd dword ptr [eax]
 scale_case:
  mov eax,scale
  fmul dword ptr [eax]
  call unresolved_ftol2_006fae8c
  mov converted_word,eax
 }
 return converted_word<<shift;
}
void params(FBParam fn,FBWord target,bool one=false){fn(target,0x2802,0x2900);if(!one)fn(target,0x2803,0x2900);fn(target,0x2801,0x2601);fn(target,0x2800,0x2601);}
FBWord next_power(FBWord value){
 value|=value>>1;value|=value>>2;value|=value>>4;value|=value>>8;value|=value>>16;value>>=1;
 value-=((value>>1)&0x55555555u);value=(value&0x33333333u)+((value>>2)&0x33333333u);value=(value+(value>>4))&0x0f0f0f0fu;value+=value>>8;value+=value>>16;
 return 1u<<(((value&0x3fu)+1u)&31u);
}
void program(const char* source){
 const auto&b=framebuffer;(*b.glEnable_0073d390)(0x8620);(*b.genPrograms_007bb528)(1,b.program_007a68d4);(*b.bindProgram_007bb788)(0x8620,*b.program_007a68d4);
 FBWord length=0;while(source[length])++length;(*b.programString_007bb580)(0x8620,0x8875,static_cast<int>(length),source);
 (*b.bindProgram_007bb788)(0x8620,0);(*b.glDisable_0073d22c)(0x8620);ReportARBError_0047a2c0(source);
}
void resources(bool separate,FBBind bind,FBParam param,FBCurrent current){
 const auto&b=framebuffer;void* saved_dc=nullptr;void* saved_context=nullptr;if(!separate){saved_dc=handle_word(*b.main_dc_007a47e4);saved_context=handle_word(*b.main_context_007a6854);}
 for(unsigned i=0;i<6;++i)b.textures_007a47c4[i]=0;for(unsigned i=0;i<6;++i)b.contexts_007a4804[i]=nullptr;
 for(unsigned i=0;i<6;++i)b.dc_007a47ec[i]=nullptr;for(unsigned i=0;i<6;++i)b.pbuffers_007a6824[i]=nullptr;*b.active_007a6860=1;
 const FBWord sizes[6]={64,128,256,512,256,256};FBCreate create=nullptr;FBGen gen=nullptr;
 for(unsigned i=0;i<6;++i){
  void* pb=CreatePBuffer_00426560(sizes[i],sizes[i]);b.pbuffers_007a6824[i]=pb;void* dc=(*b.getPbufferDC_007bb6b8)(pb);
  if(separate){if(i==0)create=*b.wglCreate_0073d2b8;b.dc_007a47ec[i]=dc;void* context=create(dc);b.contexts_007a4804[i]=context;
   (*b.wglShare_0073d2b4)(handle_word(*b.main_context_007a6854),context);current(b.dc_007a47ec[i],b.contexts_007a4804[i]);(*b.glGen_0073d24c)(1,b.textures_007a47c4+i);
  }else{void* context=handle_word(*b.main_context_007a6854);b.dc_007a47ec[i]=dc;b.contexts_007a4804[i]=context;current(dc,context);if(i==0)gen=*b.glGen_0073d24c;gen(1,b.textures_007a47c4+i);}
  bind(0xde1,b.textures_007a47c4[i]);param(0xde1,0x2802,0x2900);param(0xde1,0x2803,0x2900);param(0xde1,0x2800,0x2601);param(0xde1,0x2801,0x2601);restore_live(current,b);
 }
 *b.bloom_dc_007a68c8=nullptr;*b.bloom_context_007a68cc=nullptr;*b.bloom_pbuffer_007a68c4=nullptr;*b.bloom_texture_007a68c0=0;*b.bloom_active_0078d990=1;
 void* pb=CreatePBuffer_00426560(128,128);*b.bloom_pbuffer_007a68c4=pb;void* dc=(*b.getPbufferDC_007bb6b8)(pb);
 if(separate){*b.bloom_dc_007a68c8=dc;void* context=create(dc);*b.bloom_context_007a68cc=context;(*b.wglShare_0073d2b4)(handle_word(*b.main_context_007a6854),context);current(*b.bloom_dc_007a68c8,*b.bloom_context_007a68cc);(*b.glGen_0073d24c)(1,b.bloom_texture_007a68c0);}
 else{void* context=handle_word(*b.main_context_007a6854);*b.bloom_dc_007a68c8=dc;*b.bloom_context_007a68cc=context;current(dc,context);gen(1,b.bloom_texture_007a68c0);}
 bind(0xde1,*b.bloom_texture_007a68c0);param(0xde1,0x2802,0x2900);param(0xde1,0x2803,0x2900);param(0xde1,0x2800,0x2601);param(0xde1,0x2801,0x2601);
 if(separate)restore_live(current,b);else current(saved_dc,saved_context);
}
}
void __cdecl InitializeFrameBufferModifications_00427c90(){
 const auto&b=framebuffer;FBWord data[6][256];if(!*b.gate_0078d98c||!*b.enabled_0078daf4)return;
 FBWord nv=FrameBufferAvailable_0045f820();FBBind bind=*b.glBind_0073d240;FBParam param=*b.glParam_0073d23c;FBCurrent current=*b.wglCurrent_0073d2b0;
 if(nv){
  if(!*b.root_texture_007a687c){(*b.glGen_0073d24c)(1,b.root_texture_007a687c);bind(0x84f5,*b.root_texture_007a687c);(*b.glCopy_0073d2d8)(0x84f5,0,0x8058,0,0,*b.width_0078d1d4,*b.height_0078d1d8,0);bind(0x84f5,0);}
  for(int i=0;i<256;++i)data[1][i]=ramp(i,234,0);for(int i=0;i<256;++i)data[0][i]=ramp(i,216,8);for(int i=0;i<256;++i)data[2][i]=ramp(i,216,16);
  FBTex2 tex=nullptr;for(unsigned i=0;i<3;++i){(*b.glGen_0073d24c)(1,b.ramps[i]);bind(0xde1,*b.ramps[i]);if(i==0)tex=*b.glTex2_0073d280;tex(0xde1,0,i==0?0x8058:0x1908,i==1?256:1,i==1?1:256,0,0x1908,0x1401,data[i==0?1:i==1?0:2]);params(param,0xde1);}
  resources(true,bind,param,current);program(b.source_0078de48);
  for(int i=0;i<256;++i)data[3][i]=bloom(i,true,0);for(int i=0;i<256;++i)data[4][i]=bloom(i,false,8);for(int i=0;i<256;++i)data[5][i]=bloom(i,false,16);
  for(unsigned i=0;i<3;++i){(*b.glGen_0073d24c)(1,b.bloom_ramps[i]);bind(0xde1,*b.bloom_ramps[i]);if(i==0)tex=*b.glTex2_0073d280;tex(0xde1,0,0x8058,i==1?256:1,i==1?1:256,0,0x1908,0x1401,data[3+i]);params(param,0xde1);}
 }
 if(!ATIFrameBufferAvailable_0045f860())return;SetupATIFrameBuffer_00427490();*b.power_width_007a6890=next_power(*b.width_0078d1d4);*b.power_height_007a6894=next_power(*b.height_0078d1d8);
 if(!*b.root_texture_007a687c){(*b.glGen_0073d24c)(1,b.root_texture_007a687c);bind(0xde1,*b.root_texture_007a687c);(*b.glCopy_0073d2d8)(0xde1,0,0x8058,0,0,*b.power_width_007a6890,*b.power_height_007a6894,0);bind(0xde1,0);}
 for(int i=0;i<256;++i)data[2][i]=ramp(i,234,0);for(int i=0;i<256;++i)data[0][i]=ramp(i,216,8);for(int i=0;i<256;++i)data[1][i]=ramp(i,216,16);
 FBTex1 tex=nullptr;for(unsigned i=0;i<3;++i){(*b.glGen_0073d24c)(1,b.ramps[i]);bind(0xde0,*b.ramps[i]);if(i==0)tex=*b.glTex1_0073d2f0;tex(0xde0,0,i==0?0x8058:0x1908,256,0,0x1908,0x1401,data[i==0?2:i==1?0:1]);params(param,0xde0);}
 resources(false,bind,param,current);program(b.source_0078db80);
 for(int i=0;i<256;++i)data[5][i]=bloom(i,false,0);for(int i=0;i<256;++i)data[4][i]=bloom(i,false,8);for(int i=0;i<256;++i)data[3][i]=bloom(i,false,16);
 FBGen gen=*b.glGen_0073d24c;for(unsigned i=0;i<3;++i){gen(1,b.bloom_ramps[i]);bind(0xde0,*b.bloom_ramps[i]);if(i==0)tex=*b.glTex1_0073d2f0;tex(0xde0,0,0x8058,256,0,0x1908,0x1401,data[5-i]);params(param,0xde0,true);}
}
}
