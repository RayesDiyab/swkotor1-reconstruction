// 00422390..00422AE9 CAurTextureBasic::ParseField (lane-font packet 5).
// Evidence: lane-font/disasm/00422390.asm.txt; keyword strings 0073EED0..0073F08C.
#include "7cea577bba-font_txi.hpp"
// lane-m48-b fork GUI REPLACEMENT COPY of the M47 stage unit font-txi-parse (font-txi/txi_parse.cpp). D76 fix: the four
// procedural-controller tables were read through font_txi::procedure_bindings, which no unit defines (M47 unresolved
// ?procedure_bindings@font_txi@@3ABUProcedureBindings@1@B). The genuine tables are the texture-epoch owners
// (controller_life_random_ring.cpp / controller_perlin_arturo.cpp); their addresses are now stored directly
// (0042246D / 004224BA / 00422604 / 00422657 mov [esi],vtable). Object-pointer storage only (no function-pointer casts).
#include <cstring>
namespace texture_controller {
struct TextureControllerVTable; struct PerlinVTable;
extern const TextureControllerVTable life_vtable_0073ed54;           // texture-epoch-controller_life_random_ring
extern const PerlinVTable perlin_vtable_0073ed70;                    // texture-epoch-controller_perlin_arturo
extern const TextureControllerVTable random_vtable_0073ed90;         // texture-epoch-controller_life_random_ring
extern const TextureControllerVTable ring_distort_vtable_0073edac;   // texture-epoch-controller_life_random_ring
}
namespace font_txi {
namespace {
// Each test re-runs firstword (static 007FBAF8 token) then _stricmp(keyword,token): 004223B6/004223C1 pattern.
bool Key(const char* keyword,char* line){return kotor_stricmp_0070acaf(keyword,firstword_00463530(line))==0;}
// repe cmpsb over strlen(keyword)+1 bytes == exact case-sensitive equality (00422413.., 0042241E).
bool Same(const char* a,const char* b){for(;;){char x=*a++,y=*b++;if(x!=y)return false;if(!x)return true;}}
void* NewBase(U size,Basic* owner,const void* vtable){
 void* memory=live_operator_new(size);             // 006FA7E6
 if(!memory)return nullptr;                         // 00422479/004224C6: null -> store 0
 procedure_base_ctor_00462a70(memory,nullptr,owner); // EAX ignored; ESI (memory) kept
 std::memcpy(memory,&vtable,sizeof vtable);          // mov [esi],vtable (table address word)
 return memory;
}
void* NewDerived(U size,Basic* owner,void*(__fastcall* ctor)(void*,void*,Basic*)){
 void* memory=live_operator_new(size);
 return memory?ctor(memory,nullptr,owner):nullptr;  // constructor EAX stored
}
}
}
using namespace font_txi;
U __fastcall texture_parse_field_00422390(Basic* self,void*,const char* text){
 char* line=const_cast<char*>(text);   // readers take char*; the original never writes the line
 char procedure[64];                    // [esp+0C] after sub esp,40h (004223A5)
 procedure[0]=0;                        // 004223B1
 if(Key("proceduretype",line))parse_cstr_00463590(line+13,procedure);          // 004223B6..004223D6
 U length=0;while(procedure[length])++length;                                   // 004223DE..004223EC
 if(length!=0){                                                                  // 004223F2
  ProcedureObject* old=reinterpret_cast<ProcedureObject*>(self->f34);
  if(old){old->vtable->s00(old,nullptr,1);self->f34=0;} // 004223F8..00422405
  if(Same(procedure,"water"))self->f34=reinterpret_cast<U>(NewDerived(0x6c,self,&procedure_water_ctor_004630b0)); // 0042240E
  if(Same(procedure,"life"))self->f34=reinterpret_cast<U>(NewBase(0x3c,self,&texture_controller::life_vtable_0073ed54));       // 00422453
  if(Same(procedure,"perlin"))self->f34=reinterpret_cast<U>(NewBase(0x44,self,&texture_controller::perlin_vtable_0073ed70));   // 004224A0
  if(Same(procedure,"arturo"))self->f34=reinterpret_cast<U>(NewDerived(0x54,self,&procedure_arturo_ctor_00463320));         // 004224ED
  if(Same(procedure,"wave"))self->f34=reinterpret_cast<U>(NewDerived(0x40,self,&procedure_wave_ctor_00463260));             // 00422532
  if(Same(procedure,"cycle"))self->f34=reinterpret_cast<U>(NewDerived(0x44,self,&procedure_cycle_ctor_00463470));           // 00422577
  if(Same(procedure,"random")){                                                                                              // 004225BC
   void* p=NewBase(0x40,self,&texture_controller::random_vtable_0073ed90);
   if(p){static_cast<volatile U*>(p)[14]=0;static_cast<volatile U*>(p)[15]=0;} // +38,+3C (004225FC)
   self->f34=reinterpret_cast<U>(p);
  }
  if(Same(procedure,"ringtexdistort")){                                                                                      // 00422611
   void* p=NewBase(0x40,self,&texture_controller::ring_distort_vtable_0073edac);
   if(p){static_cast<volatile U*>(p)[14]=0;static_cast<volatile U*>(p)[15]=0;} // 00422651
   self->f34=reinterpret_cast<U>(p);
  }
 }
 if(Key("filerange",line))parse_short_00463650(line+9,(&self->c6));                 // 00422668
 if(Key("defaultwidth",line))parse_short_00463650(line+12,(&self->c8));             // 00422693
 if(Key("defaultheight",line))parse_short_00463650(line+13,(&self->ca));            // 004226BE
 if(Key("downsamplemax",line))parse_short_00463650(line+13,(&self->b8));            // 004226E9
 if(Key("downsamplemin",line))parse_short_00463650(line+13,(&self->ba));            // 00422714
 if(Key("mipmap",line))parse_bool_00463680(line+6,AsBool(self->flags[4]));                // 0042273F  +DC
 if(Key("filter",line))parse_bool_00463680(line+6,AsBool(self->flags[5]));                // 0042276A  +DD
 if(Key("maptexelstopixels",line))parse_bool_00463680(line+17,AsBool(self->flags[7]));    // 00422795  +DF
 if(Key("gamma",line))parse_float_004635c0(line+5,AsFloat(self->f24));                    // 004227C0  +24
 if(Key("isbumpmap",line))parse_int_004635e0(line+9,AsInt(self->d4));                     // 004227E8  +D4
 if(Key("clamp",line))parse_short_00463650(line+5,(&self->c0));                     // 00422813  +C0
 if(Key("alphamean",line))parse_float_004635c0(line+9,AsFloat(self->f20));                // 0042283E  +20
 if(Key("isdiffusebumpmap",line))parse_bool_00463680(line+16,AsBool(self->flags[0]));     // 00422866  +D8
 if(Key("isspecularbumpmap",line))parse_bool_00463680(line+17,AsBool(self->flags[1]));    // 00422891  +D9
 if(Key("bumpmapscaling",line))parse_float_004635c0(line+14,AsFloat(self->f18));          // 004228BC  +18
 if(Key("specularcolor",line))parse_vector_00463570(line+13,AsFloat(self->f4c));          // 004228E4  +4C..+54
 if(Key("numx",line))parse_short_00463650(line+4,(&self->c2));                      // 0042290C  +C2
 if(Key("numy",line))parse_short_00463650(line+4,(&self->c4));                      // 00422937  +C4
 if(Key("cube",line))parse_bool_00463680(line+4,AsBool(self->flags[3]));                  // 00422962  +DB cube
 if(Key("bumpintensity",line))parse_float_004635c0(line+13,AsFloat(self->f1c));           // 0042298D  +1C
 if(Key("temporary",line))parse_bool_00463680(line+9,AsBool(self->d2));                   // 004229B5  +D2
 if(Key("useglobalalpha",line))parse_bool_00463680(line+14,AsBool(self->flags[6]));       // 004229E0  +DE
 if(Key("isenvironmentmapped",line))parse_bool_00463680(line+19,AsBool(self->flags[2]));  // 00422A0B  +DA
 if(Key("envmapalpha",line))parse_float_004635c0(line+11,AsFloat(self->f28));             // 00422A36  +28
 if(Key("diffusebumpintensity",line))parse_float_004635c0(line+20,AsFloat(self->f2c));    // 00422A5E  +2C
 if(Key("specularbumpintensity",line))parse_float_004635c0(line+21,AsFloat(self->f30));   // 00422A86  +30
 // 00422AAE: FontInfo parse with ECX=[+38] unconditionally (null receiver not guarded).
 U eax=font_info_parse_00422210(self->info,nullptr,text);
 ProcedureObject* procedure_object=reinterpret_cast<ProcedureObject*>(self->f34);           // 00422AB7
 if(procedure_object)eax=procedure_object->vtable->s0c(procedure_object,nullptr,text);     // 00422AC1 call [edx+0Ch]
 std::int16_t minimum=self->ba;                                                           // 00422AC4 mov ax,[+BA]
 if(minimum>0)self->bc=minimum;                                                           // 00422ACE jle / 00422AD0
 return (eax&0xffff0000u)|static_cast<std::uint16_t>(minimum);                            // EAX high half from last call
}
