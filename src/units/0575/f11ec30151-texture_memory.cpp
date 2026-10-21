// lane-startup-misc: Aurora texture cache reset / texture-memory budget (swkotor.exe GOG K1, base 0x400000).
//   00421D50 AurTextureResetAll, 00421D90 AurTextureSetMem (lane-menu-tables texture_bindings slots),
//   and their lowers 0046BC80 CExoArrayList<CAurTextureBasic*>::operator=, 00420670 CAurTextureBasic::Reset,
//   00421AC0 AurTextureMemCheck, 00420DB0 AurTextureCalculateNewTexMem, 00420710 CAurTextureBasic::GetMemoryUsage
//   (name declared missing by lane-texture-epoch texture_cells.cpp:95), 0045E270 ImageGetS3TCSize.
// Types: lane-texture-epoch epoch E1 (texture::Basic, texture_cells::*, texture_scheduler::*). Floating point:
// the original x87 sequences run at the CRT default precision control (53-bit); each is written here as the
// equivalent IEEE double computation followed by the original float store (cited per line).
#include "29e3a61b41-texture_cells.hpp"   // texture_cells lists/accounting, texture::Basic, texture_scheduler (lane-texture-epoch)
#include "d65319cf1e-texture_vtable.hpp"  // slot +1C type (texture_is_bump_map_0041eef0)
#include "b31d883255-growth.h"            // menu_list_growth::allocate_005e03c0 (M41 viewport-list-growth.obj)
#include "7b8c81fcff-allocator_live.h"    // 006FA7E6 live_operator_new / 006FA390 live_free
#include "9ea344dff0-d1_bodies.h"         // menu_tables_m41::cells::dynamic_memory_ratio_0078d3f0 (lane-menu-tables owner)
#include <cmath>
#include <cstdint>
#include <cstring>
#include <type_traits>

// ---- providers / cells owned elsewhere (identical declarations) ----
namespace kotor_renderer_init {
void __stdcall glDeleteTextures_0073d248(std::int32_t,const std::uint32_t*);      // lane-renderer-bodies GL adapter owner
const char* __cdecl StringSearch_006fbd00(const char*,const char*);              // lane-renderer-bodies: 006FBD00 = CRT strstr
}
namespace kotor_renderer_gl_cells { extern std::uint16_t color_bits_007b9220; }   // lane-renderer-bodies (U16)
// lane-texture-epoch texture_cells.cpp:27 defines maxTexMemory in an unnamed namespace; this unit is the original
// writer (00421DA6) and needs it with external linkage under this exact name (PACKET open gate G-TEX-1).
namespace texture_cells { extern texture::I volatile max_tex_memory_0078d3f4; }

using texture::Basic; using texture::I; using texture::U; using texture::List;

// ---- cells owned by this lane (no other owner in M41 or the lane packets read) ----
namespace kotor_startup_misc_cells {
// Two 5-DWORD memory records written by 00421AC0 through its third argument (00421E02 / 00421E2C) and read by
// 00421E12..00421E85. [0] total, [1] lightmap ("_lm"/"_a00"), [2] compressed, [3] cube, [4] bump (00420710).
I dynamic_usage_007a469c[5]={};    // 007A469C..007A46AC (BSS)
I static_usage_007a46c0[5]={};     // 007A46C0..007A46D0 (BSS)
U volatile dynamic_total_007a46dc=0; // 007A46DC (BSS), written 00421E1D
U volatile static_total_007a46e0=0;  // 007A46E0 (BSS), written 00421E96
float volatile over_budget_time_007a474c=0.0f; // 007A474C (BSS): seconds over the soft budget (00421B00..00421B33)
U volatile dynamic_ratio_0078d3f8=0x3f4ccccdu;  // .data 0x3F4CCCCD (0.8f), pushed raw at 00421DF8/00421E01
float volatile over_budget_limit_0078d3fc=50.0f; // .data 0x42480000, fcomp at 00421B0C
}
namespace C=kotor_startup_misc_cells;

namespace {
inline I sar(I value,I count){return value>>(count&31);}   // x86 SAR uses the low five bits of CL
inline unsigned char* address(U word){return reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(word));}
inline Basic* element(const List& list,I index){Basic* p;std::memcpy(&p,address(list.data+static_cast<U>(index)*4u),4);return p;}
inline void set_element(List& list,I index,Basic* value){std::memcpy(address(list.data+static_cast<U>(index)*4u),&value,4);}
inline float bits_float(U bits){float f;std::memcpy(&f,&bits,4);return f;}
// x87 "fcom/fcomp; fnstsw ax; test ah,41h; jne": taken when ST < m, ST == m or unordered.
inline bool x87_not_greater(double st,double m){return !(st>m);}
using BumpSlot=decltype(&texture_is_bump_map_0041eef0);
inline std::uint8_t is_bump_map_al(Basic* self){                 // call [vtable+1Ch]; test al,al
    const unsigned char* table;std::memcpy(&table,self,4);BumpSlot f;std::memcpy(&f,table+0x1c,4);
    return static_cast<std::uint8_t>(f(self,nullptr));
}
}

extern "C" {
// 0045E270 ImageGetS3TCSize(width, height, components), cdecl: DXT block bytes over the mip chain.
__declspec(noinline) I __cdecl image_get_s3tc_size_0045e270(I width,I height,I components){
    const I block=components==4?16:8;                           // 0045E284..0045E28A sete / lea [ebx*8+8]
    I total=0;                                                  // 0045E282
    for(;;){
        if(width==0){if(height==0)return total;width=1;}        // 0045E291..0045E299
        if(height==0)height=1;                                  // 0045E29E..0045E2A2
        const I across=(width+3)/4,down=(height+3)/4;           // 0045E2A7..0045E2BE cdq/and 3/add/sar 2 (toward zero)
        total=static_cast<I>(static_cast<U>(total)+static_cast<U>(across)*static_cast<U>(down)*static_cast<U>(block)); // 0045E2C1..0045E2C7
        width>>=1;height>>=1;                                   // 0045E2C9/0045E2CB sar 1
    }
}
// 00420710 CAurTextureBasic::GetMemoryUsage(int usage[5]), RET 4.
__declspec(noinline) void __fastcall texture_memory_usage_00420710(Basic* self,void*,U* usage){
    if(self->flags[12]!=0){                                     // 00420714..0042071D +E4 (size stale)
        if(self->flags[8]==0){                                  // 004207C7..004207CF +E0
            usage[0]=0;usage[1]=0;usage[2]=0;usage[3]=0;usage[4]=0; // 004207D7..004207E4
            return;
        }
        if(reinterpret_cast<std::uintptr_t>(self)==reinterpret_cast<std::uintptr_t>(texture_cells::null_texture_007a477c.target)){ // 004207EB
            usage[0]=0;usage[1]=0;usage[2]=0;usage[3]=0;usage[4]=0; // 004207F9..00420806
            return;
        }
        const I shift=static_cast<I>(self->bc)-static_cast<I>(self->be);              // 0042080D..0042081E
        if(texture::field_load(self->f5c)>0){                                          // 0042081B..00420822
            self->e8=image_get_s3tc_size_0045e270(sar(texture::field_load(self->f68),shift),
                sar(texture::field_load(self->f6c),shift),texture::field_load(self->f70)); // 00420824..0042083C
        }else{
            I w=sar(texture::field_load(self->f68),shift);if(w<2)w=2;                  // 00420844..00420853 (edx=2 unless >=2)
            I h=sar(texture::field_load(self->f6c),shift);if(h<2)h=2;                  // 00420855..00420864
            I components=texture::field_load(self->f70);                               // 0042086E
            if(kotor_renderer_gl_cells::color_bits_007b9220==16&&components>2)components=2; // 00420866..00420878 (16-bit: skip 3->4)
            else if(components==3)components=4;                                        // 0042087F..00420884
            I size=static_cast<I>(static_cast<U>(components)*static_cast<U>(h)*static_cast<U>(w)); // 00420889..00420892
            self->e8=size;                                                             // 00420897
            if(self->flags[4]!=0)                                                      // 0042088C/00420895 +DC (mipmapped)
                self->e8=static_cast<I>(static_cast<U>(size)*4u)/3;                    // 0042089F..004208B4 signed /3 toward zero
        }
        self->flags[12]=0;                                                             // 004208BE
    }
    usage[0]=0;usage[1]=0;usage[2]=0;usage[3]=0;usage[4]=0;                            // 0042072B..00420736 / 004208C9..004208D4
    usage[0]=static_cast<U>(self->e8);                                                 // 00420739/0042074B, 004208D7/004208E9
    const char* derived=self->derived;                                                 // lea ebx,[esi+98h]
    if(kotor_renderer_init::StringSearch_006fbd00(derived,"_lm")!=nullptr||            // 00420745..00420757 0073EE14 "_lm"
       kotor_renderer_init::StringSearch_006fbd00(derived,"_a00")!=nullptr){           // 00420759..00420769 0073EE0C "_a00"
        usage[1]=static_cast<U>(self->e8);return;                                      // 004207B8..004207BE
    }
    if(self->flags[3]!=0){usage[3]=static_cast<U>(self->e8);return;}                   // 0042076B..0042077B +DB cube
    if(is_bump_map_al(self)!=0){usage[4]=static_cast<U>(self->e8);return;}             // 00420784..00420795
    if(texture::field_load(self->f5c)>0)usage[2]=static_cast<U>(self->e8);             // 0042079E..004207AF
}
// 00420DB0 AurTextureCalculateNewTexMem(list, int usage[5]), cdecl.
__declspec(noinline) void __cdecl texture_calculate_new_mem_00420db0(List* list,U* total){
    total[4]=0;total[2]=0;total[3]=0;total[1]=0;total[0]=0;     // 00420DC0..00420DCC
    for(I i=0;i<list->size;++i){                                // 00420DCE / 00420E3A..00420E40 live size
        Basic* texture=element(*list,i);                         // 00420DD4..00420DD6
        if(texture->flags[8]==0)texture_scheduler::construct_image_0041fa30(texture,nullptr); // 00420DD9..00420DE5 +E0
        if(texture->d2!=0)continue;                              // 00420DEA..00420DF2
        U usage[5];
        texture_memory_usage_00420710(texture,nullptr,usage);    // 00420DF4..00420DFB
        total[2]+=usage[2];total[4]+=usage[4];total[3]+=usage[3];total[1]+=usage[1];total[0]+=usage[0]; // 00420E19..00420E38
    }
}
// 00421AC0 AurTextureMemCheck(list, int budget, int usage[5], float ratio), cdecl.
__declspec(noinline) void __cdecl texture_memory_check_00421ac0(List* list,I budget,U* usage,float ratio){
    texture_calculate_new_mem_00420db0(list,usage);                                    // 00421ACF..00421AD1
    I used=static_cast<I>(usage[0]);                                                   // 00421ADA..00421ADC (argument slot reused)
    const float soft=static_cast<float>(static_cast<double>(budget)*static_cast<double>(ratio)); // 00421AD6..00421AE7 fild/fmul/fstp dword
    if(!x87_not_greater(static_cast<double>(used),static_cast<double>(soft))){         // 00421AEB..00421AF8 used > soft
        const double elapsed=static_cast<double>(texture_scheduler::cells.delta_0078e574[0])
                            +static_cast<double>(C::over_budget_time_007a474c);        // 00421AFA..00421B00
        C::over_budget_time_007a474c=static_cast<float>(elapsed);                      // 00421B06 fst dword
        if(!x87_not_greater(elapsed,static_cast<double>(C::over_budget_limit_0078d3fc))) goto evict; // 00421B0C..00421B19
    }else{
        C::over_budget_time_007a474c=0.0f;                                             // 00421B1B
    }
    if(used<=budget)return;                                                            // 00421B25..00421B29 / 00421D41 fstp st(0)
evict:
    C::over_budget_time_007a474c=0.0f;                                                 // 00421B33
    if(x87_not_greater(static_cast<double>(used),static_cast<double>(soft)))return;    // 00421B2F..00421B42
    do{
        Basic* victim=nullptr;I best_size=0,best_levels=0;                             // 00421B57..00421B5F
        for(I i=0;i<list->size;++i){                                                   // 00421B54..00421B67 / 00421BE1..00421BF3
            Basic* texture=element(*list,i);                                           // 00421B70..00421B7A
            const std::int16_t level=texture->bc;                                      // 00421B7D
            const I shift=static_cast<I>(level)-static_cast<I>(texture->be);           // 00421B84..00421B90
            const std::int16_t levels=texture->b8;                                     // 00421B92
            if(level>=levels)continue;                                                 // 00421B99..00421B9C (16-bit signed)
            if(sar(texture::field_load(texture->f68),shift)<=2)continue;               // 00421B9E..00421BA6
            if(sar(texture::field_load(texture->f6c),shift)<=2)continue;               // 00421BA8..00421BB0
            const I remaining=static_cast<I>(levels)-static_cast<I>(level);            // 00421BB6..00421BBC
            U current[5];
            texture_memory_usage_00420710(texture,nullptr,current);                    // 00421BB2..00421BBE
            const I size=static_cast<I>(current[0]);                                   // 00421BC3
            if(size>best_size||(size==best_size&&remaining>best_levels)){              // 00421BCB..00421BD5
                best_levels=remaining;best_size=size;victim=texture;                   // 00421BD7..00421BDF
            }
        }
        if(!victim)return;                                                             // 00421BF9..00421BFB
        U before[5];
        texture_memory_usage_00420710(victim,nullptr,before);                          // 00421C01..00421C08
        used=static_cast<I>(static_cast<U>(used)-before[0]);                           // 00421C1F/00421C26
        usage[4]-=before[4];usage[2]-=before[2];usage[3]-=before[3];usage[1]-=before[1];usage[0]-=before[0]; // 00421C36..00421C4F
        I next=static_cast<I>(victim->bc)+1;                                           // 00421C51/00421C5F
        if(!(next<static_cast<I>(victim->b8)))next=static_cast<I>(victim->b8);        // 00421C58..00421C64
        const bool changed=static_cast<std::int16_t>(next)!=victim->be;                // 00421C66 cmp ax,[ebx+BEh]
        victim->bc=static_cast<std::int16_t>(next);                                    // 00421C6D
        if(changed){
            victim->flags[12]=1;                                                       // 00421C76 +E4
            List& refresh=texture_cells::aur_refresh_textures_007a4770;
            I count=refresh.size;                                                      // 00421C7D
            I found=0;
            for(I k=0;k<count;++k)if(element(refresh,k)==victim)++found;               // 00421C89..00421C9A
            if(count<=0||found==0){                                                    // 00421C8F jle / 00421C9C..00421C9E
                if(count==refresh.capacity){                                           // 00421CA0..00421CA7
                    const I capacity=refresh.capacity;
                    menu_list_growth::allocate_005e03c0(&refresh,nullptr,static_cast<U>(capacity!=0?capacity*2:8)); // 00421CA9..00421CBC
                    count=refresh.size;                                                // 00421CC1 reload
                }
                set_element(refresh,count,victim);                                     // 00421CC7..00421CCD
                refresh.size=refresh.size+1;                                           // 00421CD0 inc dword
            }
        }
        U after[5];
        texture_memory_usage_00420710(victim,nullptr,after);                           // 00421CD6..00421CDD
        used=static_cast<I>(static_cast<U>(used)+after[0]);                            // 00421CF7..00421CF9
        usage[4]+=after[4];usage[2]+=after[2];usage[3]+=after[3];usage[1]+=after[1];usage[0]+=after[0]; // 00421D04..00421D31
    }while(!x87_not_greater(static_cast<double>(used),static_cast<double>(soft)));      // 00421CFD/00421D0A/00421D25/00421D33
}
// 00420670 CAurTextureBasic::Reset, RET.
__declspec(noinline) void __fastcall texture_reset_00420670(Basic* self,void*){
    texture::texture_call_free_b0(self);                                               // 00420674..00420676 [vtable+B0]
    const U image=self->f48;                                                           // 0042067C
    const std::int16_t base=self->ba;                                                  // 0042067F
    texture::field_store(self->f5c,0);texture::field_store(self->f60,0);texture::field_store(self->f64,0); // 0042068A..00420690
    self->ce=0;self->be=0;self->bc=base;                                               // 00420693..004206A1
    self->flags[12]=1;self->flags[8]=0;self->flags[13]=0;self->flags[14]=0;            // 004206A8..004206BB +E4/+E0/+E5/+E6
    if(image!=0){live_free(address(image));self->f48=0;}                               // 004206C1..004206CC
    const U ids=self->f40;                                                             // 004206CF
    if(ids!=0){
        const I count=static_cast<I>(self->c4)*static_cast<I>(self->c2);               // 004206D6..004206E5 movsx/imul
        kotor_renderer_init::glDeleteTextures_0073d248(count,reinterpret_cast<const std::uint32_t*>(address(ids))); // 004206E9 [0073D248]
        live_free(address(self->f40));                                                 // 004206EF..004206F3 reload
        self->f40=0;self->f58=0;                                                       // 004206FB/004206FE
    }
}
// 0046BC80 CExoArrayList<CAurTextureBasic*>::operator=(const CExoArrayList&), RET 4, EAX=this.
__declspec(noinline) List* __fastcall texture_list_assign_0046bc80(List* self,void*,const List* source){
    self->size=0;                                                                      // 0046BC8B
    for(I i=0;i<source->size;++i){                                                     // 0046BC8E / 0046BCFF..0046BD05 live
        Basic* value=element(*source,i);                                               // 0046BC94..0046BC9D
        if(self->size==self->capacity){                                                // 0046BC9A..0046BCA4
            const I capacity=self->capacity!=0?self->capacity*2:8;                     // 0046BCA6..0046BCAE
            const U old=self->data;                                                    // 0046BCB3
            self->capacity=capacity;                                                   // 0046BCBD
            self->data=static_cast<U>(reinterpret_cast<std::uintptr_t>(live_operator_new(static_cast<U>(capacity)*4u))); // 0046BCC0..0046BCC8
            for(I k=0;k<self->size;++k){U word;std::memcpy(&word,address(old+static_cast<U>(k)*4u),4);std::memcpy(address(self->data+static_cast<U>(k)*4u),&word,4);} // 0046BCCD..0046BCE1
            if(old!=0)live_free(address(old));                                        // 0046BCE3..0046BCE8
        }
        set_element(*self,self->size,value);                                           // 0046BCF0..0046BCF9
        self->size=self->size+1;                                                       // 0046BCFC inc dword
    }
    return self;                                                                       // 0046BD08
}
}

namespace menu_tables_required {
// 00421D50 AurTextureResetAll, RET.
void __stdcall TextureResetAll_00421d50(){
    (void)texture_list_assign_0046bc80(&texture_cells::aur_refresh_textures_007a4770,nullptr,&texture_cells::ordered_texture_list_007a4798); // 00421D51..00421D5B
    for(I i=0;i<texture_cells::aur_refresh_textures_007a4770.size;++i)                 // 00421D60..00421D85 live size
        texture_reset_00420670(element(texture_cells::aur_refresh_textures_007a4770,i),nullptr); // 00421D70..00421D78 live data
}
// 00421D90 AurTextureSetMem(int maxTexMemory), cdecl; EAX = (total <= maxTexMemory) via setle on zeroed EAX.
unsigned char __cdecl TextureSetMem_00421d90(int memory){
    const float ratio=bits_float(menu_tables_m41::cells::dynamic_memory_ratio_0078d3f0);              // 00421DA0 fmul dword [0078D3F0]
    const float scaled=static_cast<float>(static_cast<double>(memory)*static_cast<double>(ratio));    // 00421D93/00421DA0/00421DAF fild/fmul/fstp dword
    texture_cells::max_tex_memory_0078d3f4=memory;                                                     // 00421DA6
    const double rounded=std::nearbyint(static_cast<double>(scaled));                                  // 00421DB3..00421DBB fld/frndint (RC nearest)
    // fistp dword into the argument slot: out-of-range or NaN stores the integer indefinite 80000000h.
    const I dynamic=(rounded>=-2147483648.0&&rounded<2147483648.0)?static_cast<I>(rounded):static_cast<I>(0x80000000u); // 00421DBD
    (void)texture_list_assign_0046bc80(&texture_cells::aur_refresh_textures_007a4770,nullptr,&texture_cells::ordered_texture_list_007a4798); // 00421DBF..00421DC9
    for(I i=0;i<texture_cells::aur_refresh_textures_007a4770.size;++i)                                // 00421DCE..00421DF6
        texture_reset_00420670(element(texture_cells::aur_refresh_textures_007a4770,i),nullptr);       // 00421DE0..00421DE9
    texture_memory_check_00421ac0(&texture_cells::aur_dynamic_texture_list_007a4780,dynamic,
        reinterpret_cast<U*>(C::dynamic_usage_007a469c),bits_float(C::dynamic_ratio_0078d3f8));        // 00421DF8..00421E0D
    C::dynamic_total_007a46dc=static_cast<U>(C::dynamic_usage_007a469c[0]);                            // 00421E12..00421E1D
    const I remaining=static_cast<I>(static_cast<U>(texture_cells::max_tex_memory_0078d3f4)-static_cast<U>(dynamic)); // 00421E18/00421E23/00421E31
    texture_memory_check_00421ac0(&texture_cells::aur_static_texture_list_007a47a4,remaining,
        reinterpret_cast<U*>(C::static_usage_007a46c0),1.0f);                                          // 00421E27..00421E39 push 3F800000h
    const I* d=C::dynamic_usage_007a469c;const I* s=C::static_usage_007a46c0;
    const U dynamic0=C::dynamic_total_007a46dc;                                                        // 00421E8D
    texture_cells::accounting_007a46f0=static_cast<U>(s[4])+static_cast<U>(d[4]);                      // 00421E3E..00421E57
    texture_cells::accounting_007a46e8=static_cast<U>(s[2])+static_cast<U>(d[2]);                      // 00421E51..00421E6B
    texture_cells::accounting_007a46ec=static_cast<U>(s[3])+static_cast<U>(d[3]);                      // 00421E65..00421E7F
    C::static_total_007a46e0=static_cast<U>(s[0]);                                                     // 00421E4A/00421E96
    texture_cells::accounting_007a46e4=static_cast<U>(s[1])+static_cast<U>(d[1]);                      // 00421E79..00421EA7
    texture_cells::accounting_007a46d8=static_cast<U>(s[0])+dynamic0;                                  // 00421E9B/00421EAD
    (void)texture_list_assign_0046bc80(&texture_cells::aur_refresh_textures_007a4770,nullptr,&texture_cells::ordered_texture_list_007a4798); // 00421E9D..00421EB2
    texture_scheduler::build_and_store_all_004217f0();                                                 // 00421EB7
    return static_cast<I>(texture_cells::accounting_007a46d8)<=texture_cells::max_tex_memory_0078d3f4?1:0; // 00421EBC..00421ECC setle
}
}
static_assert(std::is_same<decltype(&menu_tables_required::TextureResetAll_00421d50),void(__stdcall*)()>::value,"reset slot");
static_assert(std::is_same<decltype(&menu_tables_required::TextureSetMem_00421d90),unsigned char(__cdecl*)(int)>::value,"memory slot");
