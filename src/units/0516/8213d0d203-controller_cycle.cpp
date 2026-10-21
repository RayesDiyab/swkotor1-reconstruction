// controller_cycle.cpp - CycleTIDTextureController (vtable 00741DD8, 0x44 bytes). Evidence build/ctl/0045FD70.txt.
#include "49aa053254-controller_math.hpp"
#include "59377d41d2-texture_cells.hpp"
using namespace controller_math;
namespace {
struct CycleTIDTextureController {
 texture_controller::TextureController base;   // +00..+37
 float fps;                                    // +38 "fps" (004634F0); ctor 1.0f (0046348B)
 I volatile index;                             // +3C current cell (004609C6)
 float elapsed;                                // +40 accumulated deltaT (0046098A)
};
static_assert(sizeof(CycleTIDTextureController)==0x44,"CycleTID layout");
CycleTIDTextureController* self_of(void* p){return static_cast<CycleTIDTextureController*>(p);}
}
extern "C" {
// 00463470 ctor(owner) RET4.
void* __fastcall cycle_controller_ctor_00463470(void* p,void*,font::Texture* owner){
 texture_controller_ctor_00462a70(p,nullptr,owner);                             // 00463478
 CycleTIDTextureController* const c=self_of(p);
 c->index=0;std::memset(&c->elapsed,0,4);                                       // 0046347F/00463482
 c->base.vtable=&texture_controller::cycle_tid_vtable_00741dd8;                 // 00463485
 std::memcpy(&c->fps,"\x00\x00\x80\x3f",4);                                     // 0046348B 3F800000h
 return c;
}
// 004634C0 dtor: table then tail jump to base dtor 00460CF0.
void __fastcall cycle_controller_dtor_004634c0(void* p,void*){
 self_of(p)->base.vtable=&texture_controller::cycle_tid_vtable_00741dd8;        // 004634C0
 texture_controller_dtor_00460cf0(p,nullptr);                                   // 004634C6
}
// 004634A0 scalar deleting destructor.
void* __fastcall cycle_controller_delete_004634a0(void* p,void*,texture::U flags){
 cycle_controller_dtor_004634c0(p,nullptr);
 if(flags&1u)live_free(p);
 return p;
}
// 00460970 Control: once per frame, advance the TID cell when elapsed exceeds 1/fps.
void __fastcall cycle_controller_control_00460970(void* p,void*){
 CycleTIDTextureController* const c=self_of(p);
 const U frame=texture_cells::texture_current_frame_007a46f4;                   // 00460971
 if(static_cast<U>(c->base.f08)==frame)return;                                  // 00460979
 c->base.f08=static_cast<I>(frame);                                             // 0046097E
 const double sum=static_cast<double>(texture_cells::delta_t_0078e574)+static_cast<double>(c->elapsed); // 00460981..00460987
 c->elapsed=static_cast<float>(sum);                                            // 0046098A fst
 const double period=1.0/static_cast<double>(c->fps);                           // 0046098D..00460993 [0073D764]=1.0f
 if(!(sum>period))return;                                                       // fcompp; test ah,41h
 font::Texture* const owner=c->base.owner;                                      // 004609A1
 std::memset(&c->elapsed,0,4);                                                  // 004609A8
 I cells=texture_vtable::head(owner)->common.s5c(owner,nullptr);                // 004609B7 GetNumY
 cells*=texture_vtable::head(owner)->common.s58(owner,nullptr);                 // 004609C0 GetNumX
 const I next=(c->index+1)%cells;                                               // 004609C6..004609CB idiv (#DE when cells==0, held)
 c->index=next;                                                                 // 004609D0
 font::Texture* const reloaded=c->base.owner;                                   // 004609CD
 texture_vtable::head(reloaded)->common.sa8(reloaded,nullptr,static_cast<U>(next)); // 004609D6 IncrementTIDIndex
}
// 004634D0 ParseField: "fps" then base 00462B00.
void __fastcall cycle_controller_parse_field_004634d0(void* p,void*,const char* text){
 char* const line=const_cast<char*>(text);
 if(kotor_stricmp_0070acaf("fps",firstword_00463530(line))==0)                  // 004634D9..004634EE
  parse_float_004635c0(line+3,&self_of(p)->fps);                                // 004634F0..004634F8
 texture_controller_parse_field_00462b00(p,nullptr,text);                       // 00463503
}
}
namespace texture_controller {
const TextureControllerVTable cycle_tid_vtable_00741dd8={
 &cycle_controller_delete_004634a0,&cycle_controller_control_00460970,&texture_controller_void_return_004015a0,
 &cycle_controller_parse_field_004634d0,&texture_controller_is_linear_0041f250,&texture_controller_distort_00460d70,
 &texture_controller_set_image_0045fd70};
}
