#include "3d96915e09-start_body.h"
#include "17f3e6b8af-ini_write.h"
#include "f730ec77e3-global_aliases.h"
#include "b593572262-m50_vtables.h"   // lane-m50-vtables W-4 scan: lane-tlk typed CSWTlkTable table address (005F899B)
#include <string.h>
namespace client_services_start {
Globals* globals=nullptr;
Word read(Global g){return kotor_menu_aliases::Cell32(globals->words[static_cast<unsigned>(g)]).load();}
void write(Global g,Word v){kotor_menu_aliases::Cell32(globals->words[static_cast<unsigned>(g)]).store(v);}
static Word ptr(const void* p){return reinterpret_cast<Word>(p);}
static Word get(Word p,unsigned offset){Word v;memcpy(&v,reinterpret_cast<void*>(p+offset),4);return v;}
static void put(Word p,unsigned offset,Word v){memcpy(reinterpret_cast<void*>(p+offset),&v,4);}
static Word call(Word site,Word target,Word self,std::initializer_list<Word> args={},Abi abi=Abi::Thiscall){
 Call c{};c.site=site;c.target=target;c.self=self;c.abi=abi;c.count=static_cast<Word>(args.size());unsigned n=0;for(Word a:args)c.args[n++]=a;
 return unresolved_client_services_provider(&c);
}
static Word alloc(Word site,Word size){return call(site,0x6fa7e6,0,{size},Abi::Cdecl);}
using String=client_services::String;
static void text(Word site,String& out,const char* input){call(site,0x5e5a90,ptr(&out),{ptr(input)});}
static void destroy(Word site,String& s){call(site,0x5e5c20,ptr(&s));}
// Addresses are the exact original call sites. Names are pinned PE literals.
static int ini_read(base_ini_reconstruction::PublicIni* ini,String& out,const char* key,const char* section,
 Word t1,Word t2,Word t3,Word invoke,Word d1,Word d2,Word d3){
 String k,s,f;text(t1,k,key);text(t2,s,section);text(t3,f,".\\swkotor.ini");
 const int result=static_cast<int>(call(invoke,0x5e67c0,ptr(ini),{ptr(&out),ptr(&f),ptr(&s),ptr(&k)}));
 destroy(d1,f);destroy(d2,s);destroy(d3,k);return result;
}
static void directory(Word t,Word invoke,Word d,const char* name){String s;text(t,s,name);call(invoke,0x408800,read(Global::Resource),{ptr(&s)});destroy(d,s);}
static void cache(Word t,Word invoke,Word d1,Word d2,const char* name){String s,out;text(t,s,name);call(invoke,0x4085f0,read(Global::Resource),{ptr(&out),ptr(&s),0x270e});destroy(d1,out);destroy(d2,s);}
static Word manager_client(){return get(read(Global::Manager),4);}
static Word options(Word site){return call(site,0x5ed700,manager_client());}
static int number(Word site,String& s){return static_cast<int>(call(site,0x5e5630,ptr(&s)));}
}
using namespace client_services_start;
client_services::Word client_services::Internal::start_services_005f8550(Word a,Word b,Word c,Word d,Word e){
 (void)b;(void)d;(void)e;
 base_ini_reconstruction::PublicIni ini;String value,dialog;
 call(0x5f8577,0x5e6750,ptr(&ini));call(0x5f8588,0x5b3190,ptr(&value));call(0x5f8596,0x5b3190,ptr(&dialog));
 word(0x1c0,alloc(0x5f85a6,1));
 Word p=alloc(0x5f85b3,0x44);word(0x24,p?call(0x5f85ca,0x4ae4c0,p):0);
 p=alloc(0x5f85dc,0x44);word(0x28,p?call(0x5f85f3,0x4ae4c0,p):0);
 p=alloc(0x5f8605,0x44);word(0x2c,p?call(0x5f861c,0x4ae4c0,p):0);
 p=alloc(0x5f862e,0x44);word(0x30,p?call(0x5f8645,0x4ae4c0,p):0);
 p=alloc(0x5f8657,0x18);word(0x14,p?call(0x5f8670,0x4d7d80,p,{1}):0);
 p=alloc(0x5f8682,0x60);write(Global::Resource,p?call(0x5f8699,0x409bf0,p):0);
 {
  String key,section,file;text(0x5f86b4,key,"AllowWindowedMode");text(0x5f86c7,section,"Graphics Options");text(0x5f86da,file,".\\swkotor.ini");
  int found=static_cast<int>(call(0x5f86fc,0x5e67c0,ptr(&ini),{ptr(&value),ptr(&file),ptr(&section),ptr(&key)}));
  destroy(0x5f870c,file);destroy(0x5f871a,section);destroy(0x5f8727,key);
  if(found)write(Global::AllowWindowed,number(0x5f8734,value)!=0?1u:0u);
 }
 write(Global::Aurora,call(0x5f8744,0x44ed70,0,{},Abi::Cdecl));
 call(0x5f8750,0x5ee990,ptr(this));
 p=alloc(0x5f875a,0x2007c);word(0x10,p?call(0x5f8771,0x5d54a0,p):0);
 if(!word(8)){p=alloc(0x5f878d,0x42c);word(8,p?call(0x5f87a4,0x54eb10,p):0);}
 p=alloc(0x5f87b6,0x50);word(0x14c,p?call(0x5f87cd,0x64b510,p):0);
 p=alloc(0x5f87e2,0x10);word(0xc,p?call(0x5f87f9,0x604b20,p):0);
 call(0x5f880c,0x604790,word(0xc),{ptr(this)});
 if(ini_read(&ini,value,"EnableReleaseLogging","Game Options",0x5f881a,0x5f882d,0x5f8840,0x5f8862,0x5f8872,0x5f8880,0x5f888d)){Word enabled=number(0x5f889a,value)!=0?1u:0u;put(get(read(Global::Base),8),0xc,enabled);}
 *globals->logging_byte_008328e1=0;
 {String s;text(0x5f88c1,s,"swc");call(0x5f88d9,0x5e93f0,get(read(Global::Base),8),{ptr(&s)});destroy(0x5f88e6,s);}
 directory(0x5f88f4,0x5f8909,0x5f8916,"TEMPCLIENT:");directory(0x5f8924,0x5f8939,0x5f8946,"OVERRIDE:");directory(0x5f8954,0x5f8969,0x5f8976,"ERRORTEX:");
 p=alloc(0x5f897d,0x64);
 // 005F897D operator new(0x64) (006FA7E6, non-zeroing); 005F8996 CTlkTable ctor 0041D8D0 stores +0,+4..+3Bh (rep stos),
 // +3Ch byte, +40h..+4Ch, +54h; 005F899B vptr 0074E700; 005F89A1 +60h=0. +58h has no initializing store in the original.
 // D2 (D86(c)/D88(1)): value-initialize CSWTlkTable+58h=0 here (deviation, not an original store). Its only original writer is
 // CSWTlkTable::Fetch 005EE3A0 (not on the menu path); CSWTlkTable::GetTokenValue keeps the original read at 00674C6F
 // (test [esi+58h]; jne 00674CAC), so the D83 trap 00674CAC is statically unreachable on the menu path.
 if(p){call(0x5f8996,0x41d8d0,p);put(p,0,kotor_vtable_word(kotor_vtable_address_cswtlktable_0074e700));/* 005F899B typed table, lane-m50-vtables */put(p,0x60,0);put(p,0x58,0);/* D2: CSWTlkTable+58h, see above */}
 write(Global::TlkClient,p);write(Global::Tlk,p);
 cache(0x5f89c5,0x5f89e4,0x5f89ed,0x5f89fa,"HD0:DATAXBOX\\items");cache(0x5f8a08,0x5f8a27,0x5f8a30,0x5f8a3d,"HD0:DATAXBOX\\scripts");cache(0x5f8a4b,0x5f8a6a,0x5f8a73,0x5f8a80,"HD0:DATAXBOX\\gui");
 {String s;text(0x5f8a8e,s,"HD0:CHITIN");call(0x5f8aa3,0x4087e0,read(Global::Resource),{ptr(&s)});destroy(0x5f8ab0,s);}
 directory(0x5f8abe,0x5f8ad3,0x5f8ae0,"RIMS:");
 p=alloc(0x5f8ae7,1);write(Global::Interface,p?call(0x5f8afe,0x70d9c0,p):0);
 directory(0x5f8b19,0x5f8b2e,0x5f8b3b,"SERVERVAULT:");directory(0x5f8b49,0x5f8b5e,0x5f8b6b,"PORTRAITS:");
 {String s;text(0x5f8b79,s,"OVERRIDE:textures");call(0x5f8b90,0x4087a0,read(Global::Resource),{ptr(&s),1});destroy(0x5f8b9d,s);}
 {String s;text(0x5f8bab,s,"HD0:patch");call(0x5f8bc2,0x4087a0,read(Global::Resource),{ptr(&s),1});destroy(0x5f8bcf,s);}
 directory(0x5f8bdd,0x5f8bf2,0x5f8bff,"HD0:MOVIES");directory(0x5f8c0d,0x5f8c22,0x5f8c2f,"HD0:STREAMWAVES");directory(0x5f8c3d,0x5f8c52,0x5f8c5f,"HD0:STREAMMUSIC");
 {unsigned char ref[16];call(0x5f8c6d,0x406d80,ptr(ref),{ptr("GLOBAL")});
  if(call(0x5f8c84,0x408bc0,read(Global::Resource),{ptr(ref),0xbba,0})){
   String s;text(0x5f8c96,s,"RIMS:GLOBAL");call(0x5f8cab,0x4087c0,read(Global::Resource),{ptr(&s)});destroy(0x5f8cb8,s);
  }
 }
 call(0x5f8cbf,0x5f1f70,ptr(this));
 if(a){call(0x5f8cd5,0x5e5c50,ptr(bytes+0x304),{c});call(0x5f8cdc,0x5f6480,ptr(this));}
 {String name,out;text(0x5f8cea,name,"HD0:DIALOG");Word returned=call(0x5f8d09,0x4085f0,read(Global::Resource),{ptr(&out),ptr(&name),0x7e2});call(0x5f8d18,0x5e5c50,ptr(&dialog),{returned});destroy(0x5f8d26,out);destroy(0x5f8d33,name);}
 call(0x5f8d43,0x41e5a0,read(Global::Tlk),{ptr(&dialog)});call(0x5f8d4a,0x5f4180,ptr(this));
 call(0x5f8d5c,0x5d5550,word(0x10),{manager_client()});Word seed=call(0x5f8d61,0x73d0f8,0,{},Abi::Stdcall);call(0x5f8d68,0x6fc43a,0,{seed},Abi::Cdecl);
 p=alloc(0x5f8d72,0x138);word(0x18,p?call(0x5f8d89,0x643f40,p):0);
 if(word(0x18)){
  int found=ini_read(&ini,value,"Texture Quality","Graphics Options",0x5f8daa,0x5f8dbd,0x5f8dd0,0x5f8df2,0x5f8e02,0x5f8e10,0x5f8e1d);int texture=number(0x5f8e26,value);



  call(0x5f8e3f,0x5f14a0,ptr(this),{found&&texture<4?static_cast<Word>(texture):0u,found&&texture<4?0u:1u});

  int fullscreen;
  found=ini_read(&ini,value,"FullScreen","Graphics Options",0x5f8e4d,0x5f8e60,0x5f8e73,0x5f8e95,0x5f8ea5,0x5f8eb3,0x5f8ec0);
  if(found)fullscreen=number(0x5f8ecd,value);
  else {
   fullscreen=0;call(0x5f8eef,0x5e5680,0,{ptr(&value),ptr("%d"),0},Abi::Cdecl);
   String key,section,file;text(0x5f8f00,key,"FullScreen");text(0x5f8f13,section,"Graphics Options");text(0x5f8f26,file,".\\swkotor.ini");call(0x5f8f48,0x5e67d0,ptr(&ini),{ptr(&value),ptr(&file),ptr(&section),ptr(&key)});destroy(0x5f8f56,file);destroy(0x5f8f64,section);destroy(0x5f8f71,key);
  }
  found=ini_read(&ini,value,"Disable Movies","Game Options",0x5f8f7f,0x5f8f92,0x5f8fa5,0x5f8fc7,0x5f8fd7,0x5f8fe5,0x5f8ff2);
  word(0x1f8,found?static_cast<Word>(number(0x5f8fff,value)):0u);
  const Word movies_enabled=word(0x1f8)==0?1u:0u;word(0x1fc,0);
  call(0x5f9036,0x61da60,word(4));call(0x5f903d,0x5f4710,ptr(this));call(0x5f905f,0x5f5ab0,ptr(this),{movies_enabled&&fullscreen?1u:0u,static_cast<Word>(fullscreen)});
  found=ini_read(&ini,value,"V-Sync","Graphics Options",0x5f906d,0x5f9080,0x5f9093,0x5f90b5,0x5f90c5,0x5f90d3,0x5f90e0);
  if(found&&options(0x5f90f1)){Word client=manager_client();int v=number(0x5f9107,value);Word o=call(0x5f910f,0x5ed700,client);call(0x5f9116,0x61d8e0,o,{static_cast<Word>(v)});}
  found=ini_read(&ini,value,"Anisotropy","Graphics Options",0x5f9124,0x5f9137,0x5f914a,0x5f916c,0x5f917c,0x5f918a,0x5f9197);
  if(found&&options(0x5f91a8)){Word client=manager_client();int v=number(0x5f91be,value);Word o=call(0x5f91c6,0x5ed700,client);call(0x5f91cd,0x61d8c0,o,{static_cast<Word>(v)});}
  found=ini_read(&ini,value,"Anti Aliasing","Graphics Options",0x5f91db,0x5f91ee,0x5f9201,0x5f9223,0x5f9233,0x5f9241,0x5f924e);
  if(found&&options(0x5f9263)){
   Word client=manager_client();int v=number(0x5f9279,value);Word o=call(0x5f9281,0x5ed700,client);call(0x5f9288,0x61d710,o,{static_cast<Word>(v)});
   if(number(0x5f9291,value)>0 && (!movies_enabled||!fullscreen)){
    Word flags=get(options(0x5f92b3),0);call(0x5f92d9,0x403800,0,{read(Global::Width),read(Global::Height),static_cast<Word>(*globals->depth_0078d1e0),(flags>>3)&1u,1},Abi::Cdecl);
   }
  }
  {String out;Word returned=call(0x5f92ee,0x41e8f0,read(Global::Tlk),{ptr(&out),0});call(0x5f92ff,0x5e5c50,ptr(bytes+0x13c),{returned});destroy(0x5f930c,out);}
  word(0x144,call(0x5f9317,0x41e0f0,read(Global::Tlk)));
  if(word(0x144)==5)call(0x5f932d,0x6fe34d,0,{2,ptr("Polish_Poland.1250")},Abi::Cdecl);
  if(static_cast<int>(word(0x144))>=1000){word(0xdc,1);p=alloc(0x5f9350,0x128);word(0xe0,p?call(0x5f9367,0x5ee400,p):0);}
  call(0x5f937c,0x5ee920,ptr(this));p=alloc(0x5f9383,0x10);word(0x1c4,p?call(0x5f939a,0x678c10,p):0);
  call(0x5f93af,0x5f0ad0,ptr(this));call(0x5f93b6,0x5f1c20,ptr(this));
  p=alloc(0x5f93c0,0xa8);word(0x274,p?call(0x5f93d7,0x40bad0,p):0);call(0x5f93fa,0x40be70,word(0x274),{read(Global::Width),read(Global::Height)});
  Word input=read(Global::Input);
  if(input){call(0x5f941f,0x5df5d0,input,{static_cast<Word>(static_cast<int>(read(Global::Width))/2),static_cast<Word>(static_cast<int>(read(Global::Height))/2)});



   if((call(0x5f942a,0x5df6e0,read(Global::Input))&0xffu)==1)

    call(0x5f9439,0x40b060,word(0x274));
  }
  call(0x5f9446,0x61f980,ptr(this),{1,0,0});call(0x5f9451,0x409f00,word(0x274));
  p=alloc(0x5f945b,0xc24);word(0x40,p?call(0x5f9472,0x62fed0,p):0);if(word(0x40))call(0x5f9488,0x5f2bf0,ptr(this));
 }
 destroy(0x5f9496,dialog);destroy(0x5f94a4,value);call(0x5f94b5,0x5e67e0,ptr(&ini));return 0;
}
