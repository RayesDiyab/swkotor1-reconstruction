// Original inline ReadStatsFromGff region 005B0D49..005B1253.
// The exported transport is a new seam, not an original standalone entry.
#include <cstdint>
#include <cstring>
#include "ab77de67ab-level.hpp"
namespace kotor_reconstruction { struct StartupString; void __cdecl StartupFormat005e5680(StartupString*,const char*,...); struct ArrayMutations; }
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*,void*); }
namespace menu_list_growth { void __fastcall allocate_005e03c0(kotor_reconstruction::ArrayMutations*,void*,unsigned); }
extern "C" {
void* __cdecl live_operator_new(unsigned); void __cdecl live_free(void*);
void __stdcall crt_eh_vector_constructor_006fb55f(void*,unsigned,std::int32_t,void*,void*);
unsigned __fastcall gff_list_004118c0(void*,void*,void*,void*,const char*);
unsigned __fastcall gff_list_count_00411940(void*,void*,void*);
unsigned __fastcall gff_list_element_00411990(void*,void*,void*,void*,unsigned);
unsigned char __fastcall gui_load_gff_byte_00411a60(void*,void*,void*,const char*,unsigned*,unsigned char);
unsigned __fastcall recovered_gff_word_00411b40(void*,void*,void*,const char*,unsigned*,unsigned);
unsigned char __fastcall recovered_stats_level_005a5fd0(void*,void*,unsigned);
void* __fastcall recovered_history_level_ctor_005cb1b0(void*,void*);
unsigned __fastcall recovered_skill_rank_change_005cb0f0(void*,void*,unsigned);
char* __fastcall async_string_cstr_005e5670(void*,void*);
void __fastcall frame_LogStringDtor_005e5c20(void*,void*);
void __fastcall kotor_m15_array_u16_allocate_004d2fa0(void*,void*,unsigned);
extern void* kotor_cell_rules_007a3a28;
}
namespace {
using U=std::uint32_t;
static_assert(sizeof(void*)==4,"original x86 pointer width");
void* ptr(U a){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(a));}
U word(const void* p){return static_cast<U>(reinterpret_cast<std::uintptr_t>(p));}
U read(U a){U v;std::memcpy(&v,ptr(a),4);return v;}
void write(U a,U v){std::memcpy(ptr(a),&v,4);}
U byte(U a){return *static_cast<unsigned char*>(ptr(a));}
U half(U a){std::uint16_t v;std::memcpy(&v,ptr(a),2);return v;}
void putbyte(U a,U v){*static_cast<unsigned char*>(ptr(a))=static_cast<unsigned char>(v);}
void puthalf(U a,U v){const auto h=static_cast<std::uint16_t>(v);std::memcpy(ptr(a),&h,2);}
struct Mem final:kotor::character_level_stats::LevelMemory {
 U read(U a)const override{return ::read(a);}void write(U a,U v)override{::write(a,v);}
 std::uint8_t read_byte(U a)const override{return static_cast<std::uint8_t>(byte(a));}
 std::uint16_t read_short(U a)const override{return static_cast<std::uint16_t>(half(a));}
 void write_byte(U a,std::uint8_t v)override{putbyte(a,v);}void write_short(U a,std::uint16_t v)override{puthalf(a,v);}
};
struct Deps final:kotor::character_level_stats::Dependencies {
 U rules_007a3a28()override{return word(kotor_cell_rules_007a3a28);}
 U allocate_006fa7e6(U n)override{return word(live_operator_new(n));} // 006FA7E6 operator new -> game heap
 void vector_construct_006fb55f(U a,U b,U c,U d,U e)override{crt_eh_vector_constructor_006fb55f(ptr(a),b,static_cast<std::int32_t>(c),ptr(d),ptr(e));} // 006FB55F stdcall RET 14h
 void grow_u16_004d2fa0(U a,U n)override{kotor_m15_array_u16_allocate_004d2fa0(ptr(a),nullptr,n);}
};
void add_dword(U list,U value){U cap=read(list+8);if(read(list+4)==cap)menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(ptr(list)),nullptr,cap?cap+cap:16);U index=read(list+4);write(read(list)+index*4,value);write(list+4,read(list+4)+1);}
struct Key {
 U a;explicit Key(U p):a(p){(void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(ptr(a)),nullptr);}
 ~Key(){frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
};
}
extern "C" void __fastcall recovered_stats_level_history_005b0d49(void* object,void*,void* file,void* parent,void* frame){
 const U self=word(object),s=word(frame);
 (void)gff_list_004118c0(file,nullptr,ptr(s+0x110),parent,"LvlStatList");
 putbyte(s+0x1f,0);
 if(!recovered_stats_level_005a5fd0(object,nullptr,0))return;
 do {
  U index=byte(s+0x1f),entry;
  if(index){
   void* allocation=live_operator_new(0x30);entry=word(allocation);write(s+0x14,entry);
   if(allocation){try{entry=word(recovered_history_level_ctor_005cb1b0(allocation,nullptr));}catch(...){live_free(allocation);throw;}}
   write(s+0x14,entry);add_dword(self+0x28,entry);
  }else{
   entry=0;if(read(self+0x6c)&&static_cast<std::int32_t>(index)<static_cast<std::int32_t>(read(self+0x2c)))entry=read(read(self+0x28)+index*4);
  }
  if(gff_list_element_00411990(file,nullptr,ptr(s+0x18),ptr(s+0x110),byte(s+0x1f))){
   // 005B0E27: original reads byte [entry+2Ah] without a null check (a null entry faults there); no trap substituted.
   const U offsets[]={0x2a,0x2b,0x2d,0x2c};const char* labels[]={"LvlStatAbility","LvlStatHitDie","LvlStatForce","LvlStatClass"};
   for(unsigned i=0;i<4;++i)putbyte(entry+offsets[i],gui_load_gff_byte_00411a60(file,nullptr,ptr(s+0x18),labels[i],static_cast<unsigned*>(ptr(s+0x10)),static_cast<unsigned char>(byte(entry+offsets[i]))));
   {
    Key key(s+0x28);
    const char* bases[]={"KnownList","KnownRemoveList"};const U fields[]={0,0x0c},elements[]={0x5c,0x54};
    for(unsigned kind=0;kind<2;++kind){
     kotor_reconstruction::StartupFormat005e5680(reinterpret_cast<kotor_reconstruction::StartupString*>(ptr(s+0x28)),"%s%d",bases[kind],0);
     if(gff_list_004118c0(file,nullptr,ptr(s+0x74),ptr(s+0x18),async_string_cstr_005e5670(ptr(s+0x28),nullptr))){
      write(s+0x14,0);
      if(gff_list_count_00411940(file,nullptr,ptr(s+0x74))){
       write(s+0x20,0);
       do{
        if(gff_list_element_00411990(file,nullptr,ptr(s+elements[kind]),ptr(s+0x74),read(s+0x20))){
         U spell=recovered_gff_word_00411b40(file,nullptr,ptr(s+elements[kind]),"Spell",static_cast<unsigned*>(ptr(s+0x10)),0xffff)&0xffff;
         write(s+0x24,spell);
         // Original MOVZX AX then CMP EAX,-1 cannot reject the 0xffff fallback.
         add_dword(entry+fields[kind],read(s+0x24));
        }
        write(s+0x14,read(s+0x14)+1);write(s+0x20,read(s+0x14)&0xffff);
       }while(read(s+0x20)<gff_list_count_00411940(file,nullptr,ptr(s+0x74)));
      }
     }
    }
   }
   puthalf(entry+0x28,recovered_gff_word_00411b40(file,nullptr,ptr(s+0x18),"SkillPoints",static_cast<unsigned*>(ptr(s+0x10)),half(self+0x164)));
   if(gff_list_004118c0(file,nullptr,ptr(s+0xd8),ptr(s+0x18),"SkillList")){
    U ranks=read(self+0x168);/* 005B1116: unchecked store through [self+168h], as original */write(ranks,0);write(ranks+4,0);write(s+0x20,0);write(s+0x24,0);
    do{
     if(gff_list_element_00411990(file,nullptr,ptr(s+0x38),ptr(s+0xd8),read(s+0x24))){
      U skill=read(s+0x20),fallback=recovered_skill_rank_change_005cb0f0(ptr(entry),nullptr,skill);
      U rank=gui_load_gff_byte_00411a60(file,nullptr,ptr(s+0x38),"Rank",static_cast<unsigned*>(ptr(s+0x10)),static_cast<unsigned char>(fallback));
      Mem m;Deps d;kotor::character_level_stats::set_skill_005cb120(m,d,entry,read(s+0x20),rank);
     }
     write(s+0x20,read(s+0x20)+1);write(s+0x24,read(s+0x24)+1);
    }while(half(s+0x20)<8);
   }
   if(gff_list_004118c0(file,nullptr,ptr(s+0x88),ptr(s+0x18),"FeatList")){
    U count=gff_list_count_00411940(file,nullptr,ptr(s+0x88))&0xffff;
    if(count){write(s+0x24,0);write(s+0x14,count);do{
     if(gff_list_element_00411990(file,nullptr,ptr(s+0x3c),ptr(s+0x88),read(s+0x24))){
      U feat=recovered_gff_word_00411b40(file,nullptr,ptr(s+0x3c),"Feat",static_cast<unsigned*>(ptr(s+0x10)),0);
      if(read(s+0x10)){Mem m;Deps d;kotor::character_level_stats::add_feat_005cb150(m,d,entry,feat);}
     }
     write(s+0x24,read(s+0x24)+1);write(s+0x14,read(s+0x14)-1);
    }while(read(s+0x14));}
   }
  }
  putbyte(s+0x1f,byte(s+0x1f)+1);
 }while(byte(s+0x1f)<recovered_stats_level_005a5fd0(object,nullptr,0));
}
