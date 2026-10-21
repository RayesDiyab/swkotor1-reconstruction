#pragma once
#include <cstdint>
#include <cstddef>
namespace kotor_menu_10_19 {
using Word=std::uint32_t; using Wide=unsigned __int64;
inline Word read32(const void* p,std::size_t off){return *reinterpret_cast<const volatile Word*>(static_cast<const unsigned char*>(p)+off);}
inline void write32(void* p,std::size_t off,Word v){*reinterpret_cast<volatile Word*>(static_cast<unsigned char*>(p)+off)=v;}
inline void write16(void* p,std::size_t off,std::uint16_t v){*reinterpret_cast<volatile std::uint16_t*>(static_cast<unsigned char*>(p)+off)=v;}
inline unsigned char read8(const void* p,std::size_t off){return static_cast<const volatile unsigned char*>(p)[off];}
inline void write8(void* p,std::size_t off,unsigned char v){static_cast<volatile unsigned char*>(p)[off]=v;}
struct WorldTimer { alignas(4) unsigned char bytes[0x44]; WorldTimer* __thiscall construct_004ae4c0(); };
struct ObjectArray { alignas(4) unsigned char bytes[0x18]; ObjectArray* __thiscall construct_004d7d80(int); };
struct String { alignas(4) unsigned char bytes[8]; String* __thiscall default_005b3190(); const char* __thiscall text_005e5670(); String* __thiscall copy_005e5b00(const String*); void __thiscall destroy_005e5c20(); String* __thiscall assign_text_005e5140(const char*); String* __thiscall concatenate_005e5d10(String*,const String*); String* __thiscall assign_005e5c50(const String*); };
struct ResRef { alignas(4) unsigned char bytes[16]; ResRef* __thiscall default_00405ed0(); ResRef* __thiscall assign_00406290(ResRef*,const char*); };
struct StringResource { String text; ResRef sound; Word duration_bits; };
struct Tlk { alignas(4) unsigned char bytes[0x64]; Tlk* __thiscall construct_0041d8d0(); Word __thiscall language_0041e0f0(); String* __thiscall simple_0041e8f0(String*,Word); Word __thiscall open_0041e5a0(String*); int __thiscall fetch_0041e1a0(Word,StringResource*,int); };
struct Gui { alignas(4) unsigned char bytes[0xa8]; Gui* __thiscall construct_0040bad0(); void __thiscall size_0040be70(int,int); void __thiscall update_fonts_0040b420(); void __thiscall redraw_0040acc0(); };
struct ServerInfo { alignas(4) unsigned char bytes[0x42c]; ServerInfo* __thiscall construct_0054eb10(); };
struct TimerDependency { Wide __thiscall high_resolution_005e9260(); };
struct Divmod { Word quotient_low,quotient_high,remainder_low,remainder_high; };
namespace dependencies {
extern void* base_007a39e0;
void* __cdecl allocate_006fa7e6(Word);
Wide __stdcall unsigned_divide_006fb030(Wide,Wide);
void __cdecl divmod_adapter(Wide,Wide,Divmod*);
}
static_assert(sizeof(Word)==4 && sizeof(String)==8 && sizeof(ResRef)==16 && alignof(StringResource)==4 && offsetof(StringResource,text)==0 && offsetof(StringResource,sound)==8 && offsetof(StringResource,duration_bits)==24,"Observed TLK output fields");
static_assert(sizeof(void*)==4 && sizeof(WorldTimer)==0x44 && sizeof(ObjectArray)==0x18 && sizeof(Tlk)==0x64 && sizeof(Gui)==0xa8 && sizeof(ServerInfo)==0x42c && sizeof(StringResource)==28,"Exact x86 views");
}
// This entry has a custom register result: EDX:EAX quotient, EBX:ECX remainder;
// RET16. EBX is intentionally a result register, rather than preserved.
extern "C" void __stdcall unresolved_aulldvrm_006fd0c0(kotor_menu_10_19::Wide,kotor_menu_10_19::Wide);
