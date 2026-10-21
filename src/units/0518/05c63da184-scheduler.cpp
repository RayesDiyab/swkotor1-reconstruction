#include "d21d0ebf81-scheduler.h"
#include "6fdf26ef30-texture_controller.hpp"
#include <cstring>

namespace texture_scheduler {
namespace {
I signed_word(U n) { return n&0x80000000u ? -1-static_cast<I>(~n) : static_cast<I>(n); }
U word(const void* p) { U n; std::memcpy(&n,p,4); return n; }
void put(void* p,U n) { std::memcpy(p,&n,4); }
void* pointer(U n) { return reinterpret_cast<void*>(n); }
U address(const void* p) { return reinterpret_cast<U>(p); }
void* indexed(U base,U index) { return pointer(base+index*4u); }
U grid(Basic* p) { const I rows=p->c4;const I columns=p->c2;return static_cast<U>(rows)*static_cast<U>(columns); }

// E1: controller+8, slot+30 (AL) and slot+B0 use the shared texture_call_* carriers (texture_x86.asm).
// Preserve the original ST0 -> binary32 store without an invented double return.
U alpha_mean_bits(const void* image,I width,I height,U format) {
 U result;
 const auto target=&scheduler_alpha_mean_0045e790;
 __asm {
  push format
  push height
  push width
  push image
  call target
  fstp dword ptr result
  add esp,10h
 }
 return result;
}
void round_unused_budget(const I volatile* budget,const U volatile* scale) { // E1 edit S9: raw scale cell
 float rounded_float;I rounded_int;
 __asm {
  mov ecx,budget
  fild dword ptr [ecx]
  mov ecx,scale
  fmul dword ptr [ecx]
  fstp dword ptr rounded_float
  fld dword ptr rounded_float
  frndint
  fistp dword ptr rounded_int
 }
 // Original rounded integer is not subsequently consumed; retain the FP work.
}
bool delta_greater(const float volatile* delta,const float volatile* peak) {
 unsigned char greater;
 __asm {
  mov ecx,delta
  fld dword ptr [ecx]
  mov ecx,peak
  fcomp dword ptr [ecx]
  fnstsw ax
  test ah,41h
  setz greater
 }
 return greater!=0;
}
// REP stores retain the original zero-count/no-access case and exact byte pattern.
void fill_grey(void* memory,U count) { // E1 compile fix: 'size' is a MASM operator inside __asm (C2400)
 __asm {
  mov edi,memory
  mov ecx,count
  mov edx,ecx
  shr ecx,2
  mov eax,7f7f7f7fh
  rep stosd
  mov ecx,edx
  and ecx,3
  rep stosb
 }
}
void copy_float_word(float volatile* destination,const float volatile* source) {
 __asm {
  mov ecx,source
  mov eax,dword ptr [ecx]
  mov ecx,destination
  mov dword ptr [ecx],eax
 }
}
}

U __stdcall texture_make_current_00421ee0() {
 // 00421EE0..00421F2F. Keep unused x87 rounding and counter order.
 if(*cells.skip_007a46d5==0) {
  round_unused_budget(cells.budget_0078d3f4,cells.scale_0078d3f0);
  const bool pending=cells.pending_007a4770->size!=0;
  *cells.count_007a4724=0;
  *cells.count_007a4730=0;
  *cells.count_007a4728=0;
  if(pending) {
   build_and_store_all_004217f0();
   U now=*cells.current_007a46b0;
   if(signed_word(*cells.peak_007a4754)<signed_word(now))*cells.peak_007a4754=now;
   now=*cells.count_007a472c;
   if(signed_word(*cells.peak_007a4758)<signed_word(now))*cells.peak_007a4758=now;
   now=*cells.count_007a4730;
   if(signed_word(*cells.peak_007a475c)<signed_word(now))*cells.peak_007a475c=now;
   now=*cells.count_007a4728;
   if(signed_word(*cells.peak_007a4760)<signed_word(now))*cells.peak_007a4760=now;
   *cells.count_007a472c=0;
  }
 }
 // 00421F84: compare first; load frame; only then reload/store delta.
 const bool greater=delta_greater(cells.delta_0078e574,cells.peak_delta_007a4750);
 U frame=*cells.frame_007a46f4;
 if(greater)copy_float_word(cells.peak_delta_007a4750,cells.delta_0078e574);
 frame+=1u;*cells.frame_007a46f4=frame;return frame;
}

void __stdcall build_and_store_all_004217f0() {
 // 004217F0..0042181A. Snapshot local count; publish after each callback.
 U remaining=static_cast<U>(cells.pending_007a4770->size);
 if(remaining==0)return;
 do {
  const U data=cells.pending_007a4770->data;
  auto* self=static_cast<Basic*>(pointer(word(indexed(data,remaining-1u))));
  remaining-=1u;
  build_and_store_texture_004216a0(self,nullptr);
  cells.pending_007a4770->size=signed_word(remaining);
 } while(remaining!=0);
 // 0042181C..00421856. Sort arguments, callback, then live count reloads.
 const I count=cells.ordered_007a47a4->size;
 const U data=cells.ordered_007a47a4->data;
 scheduler_sort_words_0041e9c0(static_cast<U*>(pointer(data)),count);
 const U lookup=static_cast<U>(cells.lookup_007a4798->size);
 const I ordered_count=cells.ordered_007a47a4->size;
 *cells.lookup_count_007a4748=lookup;
 cells.ids_007a478c->size=0;
 *cells.maximum_id_007a46bc=0;
 if(ordered_count<=0)return;
 U outer=0;
 do {
  const U ordered_data=cells.ordered_007a47a4->data;
  auto* self=static_cast<Basic*>(pointer(word(indexed(ordered_data,outer))));
  if(self->flags[9]==0 && signed_word(grid(self))>0) { // +E1, C4*C2
   U inner=0;
   do {
    const U capacity=static_cast<U>(cells.ids_007a478c->capacity);
    const bool grow=static_cast<U>(cells.ids_007a478c->size)==capacity;
    const U id_data=self->f40;
    const U saved_id=word(indexed(id_data,inner)); // Before possible allocator callbacks.
    if(grow) {
     const U next=capacity?capacity+capacity:8u;
     const U old=cells.ids_007a478c->data;
     cells.ids_007a478c->capacity=signed_word(next); // Publish capacity before allocation.
     void* fresh=live_operator_new(next*4u);
     const I copy_count=cells.ids_007a478c->size;
     cells.ids_007a478c->data=address(fresh);
     if(copy_count>0) {
      U i=0;
      do {
       const U value=word(indexed(old,i));
       const U destination=cells.ids_007a478c->data;
       put(indexed(destination,i),value);
       const I current_count=cells.ids_007a478c->size;
       i+=1u;
       if(signed_word(i)>=current_count)break;
      } while(true);
     }
     if(old)live_free(pointer(old));
    }
    const U count_now=static_cast<U>(cells.ids_007a478c->size);
    const U destination=cells.ids_007a478c->data;
    put(indexed(destination,count_now),saved_id);
    const U increment=static_cast<U>(cells.ids_007a478c->size)+1u;
    const U maximum=*cells.maximum_id_007a46bc;
    cells.ids_007a478c->size=signed_word(increment);
    const U reloaded_data=self->f40;
    const U reloaded_id=word(indexed(reloaded_data,inner));
    if(reloaded_id>maximum)*cells.maximum_id_007a46bc=reloaded_id; // Unsigned JBE.
    const U total=grid(self);
    inner+=1u;
    if(signed_word(inner)>=signed_word(total))break;
   } while(true);
  }
  const I current_ordered=cells.ordered_007a47a4->size;
  outer+=1u;
  if(signed_word(outer)>=current_ordered)break;
 } while(true);
}

void __fastcall build_and_store_texture_004216a0(Basic* self,void*) {
 if(self->flags[10]!=0) { // +E2 special path; original REPE CMPS stops on mismatch.
  for(U i=0;i<12u;++i)
   if(static_cast<unsigned char>(self->name[i])!=static_cast<unsigned char>(cells.norm_cube_map_0073ee34[i]))return;
  texture::basic_as_normalize_vector_cube_map_00420c20(self,nullptr);return; // E1 edit S11 (004216CB jmp 00420C20)
 }
 if(self->flags[8]==0)construct_image_0041fa30(self,nullptr); // +E0 is attempt state.
 if(self->f44!=0) {
  (void)texture::basic_gl_image_00420970(self,nullptr,0);
  if(texture::alpha_bits(*self)==texture::alpha_sentinel_bits) {
   const I mip_count=texture::field_load(self->f5c);
   if(mip_count>0)texture::set_alpha_bits(*self,texture::alpha_one_bits);
   else {
    const U format=static_cast<U>(texture::field_load(self->f70));
    const I height=texture::field_load(self->f6c);
    const I width=texture::field_load(self->f68);
    const U image=self->f44;
    texture::set_alpha_bits(*self,alpha_mean_bits(pointer(image),width,height,format));
   }
  }
 }
 if(texture::field_load(self->f5c)==0) {
  const U controller=self->f34;
  if(controller){ // E1 edit S10: typed TextureController +08 Init (00421733..35)
   texture_controller::TextureController* const object=texture_controller::from_word(controller);
   texture_controller::table(object)->s08(object,nullptr);
  }
 }
 if(texture::texture_call_processed_30_al(self)==0)texture::texture_call_free_b0(self); // Reload table after query callback.
}

void __fastcall construct_image_0041fa30(Basic* self,void*) {
 // Preserve source load order before each genuine call.
 const U gamma=texture::float_bits(self->f24); // E1: +24 float, pushed raw (0041FA34)
 const U original_image=self->f44;
 const U format=static_cast<U>(texture::field_load(self->f70));
 const I height=texture::field_load(self->f6c);
 const I width=texture::field_load(self->f68);
 scheduler_gamma_words_0045dd90(width,height,format,pointer(original_image),gamma);
 U* components=self->components;
 if(components) {
  scheduler_load_plt_0041f9b0(self,nullptr);
  const U saved_image=self->f44;
  texture::field_store(self->f5c,0);
  if(saved_image) {
   const I count=self->component_count;
   components=self->components;
   const I palette_parameter=*cells.palette_parameter_007a4708;
   const unsigned char* palette=*cells.palette_007a4704;
   const I current_height=texture::field_load(self->f6c);
   const I current_width=texture::field_load(self->f68);
   void* decoded=scheduler_decode_0045de10(pointer(saved_image),current_width,current_height,palette,palette_parameter,components,count);
   self->f44=address(decoded);
   if(self->flags[11]!=0) {
    const I release_height=texture::field_load(self->f6c);
    const I release_width=texture::field_load(self->f68);
    (*cells.release_components_007a46fc)(pointer(saved_image),release_width,release_height);
    self->flags[11]=0;
    texture::field_store(self->f70,4);
    texture::set_alpha_bits(*self,texture::alpha_one_bits);
   } else {
    live_free(pointer(saved_image));
    texture::field_store(self->f70,4);
    texture::set_alpha_bits(*self,texture::alpha_one_bits);
   }
  } else texture::set_alpha_bits(*self,texture::alpha_one_bits);
 } else (void)acquisition::load_image_a_0041f6a0(self,nullptr); // full char* EAX ignored
 // Original carries this one image observation in ECX through dimension stores.
 const U image=self->f44;
 if(image==0) {
  if(texture::field_load(self->d4)!=0)texture::field_store(self->f70,1);
  const I old_width=texture::field_load(self->f68);
  texture::field_store(self->f68,old_width<2?2:old_width);
  const I old_height=texture::field_load(self->f6c);
  texture::field_store(self->f6c,old_height<2?2:old_height);
  const U controller=self->f34;
  if(controller) {
   const I new_height=self->ca;
   const I new_width=self->c8;
   texture::field_store(self->f6c,new_height);
   const U allocation=static_cast<U>(new_height)*static_cast<U>(new_width)*4u;
   texture::field_store(self->f68,new_width);
   texture::field_store(self->f70,4);
   void* memory=live_operator_new(allocation);
   U fill=static_cast<U>(texture::field_load(self->f70));
   fill*=static_cast<U>(texture::field_load(self->f6c));
   fill*=static_cast<U>(texture::field_load(self->f68));
   self->f44=address(memory);
   fill_grey(memory,fill); // No allocation-failure branch added.
  } else self->flags[9]=1;
 }
 if(self->flags[2]!=0 && *cells.fill_alpha_007a46d4!=0) {
  const U current_format=static_cast<U>(texture::field_load(self->f70));
  const I current_height=texture::field_load(self->f6c);
  const I current_width=texture::field_load(self->f68);
  const U current_image=self->f44;
  scheduler_fill_alpha_0045f0d0(pointer(current_image),current_width,current_height,current_format,cells.fill_vector_007a4764);
 }
 self->flags[8]=1; // Original +E0 write even when +44 remains null.
}
}
