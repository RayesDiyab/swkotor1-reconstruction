// lane-config: 0061B780 CClientOptions::SaveOptions (ECX=options, RET, EAX=1).
// Writes 43 + 11 entries to .\swkotor.ini in the original order; every block is
// Format(value) -> key/section/file text -> WriteIniEntry(value,file,section,key)
// -> destroy file, section, key (e.g. 0061B7C8..0061B859).
#include "2e9f118037-movie_list.h"
#include "c534281817-startup_string.h"
#include "c7e191db10-native_strings.h"
#include <cstring>
#include <new>
namespace client_movies {
namespace {
const char kIni[]=".\\swkotor.ini";            // 0073D648
const char kGame[]="Game Options";             // 0073D658
const char kAutopause[]="Autopause Options";   // 0074F9EC
const char kGraphics[]="Graphics Options";     // 0073D6AC
const char kSound[]="Sound Options";           // 0073D724
const char kMovies[]="Movies Shown";           // 0074F900
const char kMovieFormat[]="Movie%2d";          // 0074F910
const char kInt[]="%d";                        // 0073D720
const char kFloat[]="%f";                      // 0073E2E4
U32 Word(const void* p,std::size_t o){U32 v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
U8 Byte(const void* p,std::size_t o){return static_cast<const U8*>(p)[o];}
const char* WindowCStrView(String* s){return WindowCStr005e5670(std::launder(static_cast<kotor_reconstruction::StartupString*>(static_cast<void*>(s))),nullptr);} // 0061CE7E
float Float(const void* p,std::size_t o){float v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
void Write(options_video::Ini& ini,String& value,const char* key,const char* section) {
    auto& o=options_video::options_bindings;String k,s,f;
    o.text_string_005e5a90(&k,nullptr,key);o.text_string_005e5a90(&s,nullptr,section);o.text_string_005e5a90(&f,nullptr,kIni);
    (void)o.write_005e67d0(&ini,nullptr,&value,&f,&s,&k);
    o.destroy_string_005e5c20(&f,nullptr);o.destroy_string_005e5c20(&s,nullptr);o.destroy_string_005e5c20(&k,nullptr);
}
void Int(options_video::Ini& ini,String& value,U32 v,const char* key,const char* section) {
    options_video::options_bindings.format_005e5680(&value,kInt,v);Write(ini,value,key,section);
}
// FLD m32; FSTP m64 (exact widening) as the variadic double for "%f".
void Real(options_video::Ini& ini,String& value,float v,const char* key) {
    options_video::options_bindings.format_005e5680(&value,kFloat,static_cast<double>(v));Write(ini,value,key,kGame);
}
}
I32 __fastcall SaveOptions_0061b780(void* self,void* unused_edx) {
    (void)unused_edx;auto& o=options_video::options_bindings;
    options_video::Ini ini;String value;
    o.construct_ini_005e6750(&ini,nullptr);                                     // 0061B7A7
    o.default_string_005b3190(&value,nullptr);                                  // 0061B7B6
    const void* p=self;
    Int(ini,value,(Word(p,8)>>4)&1u,"Combat Movement",kGame);                  // 0061B7BB..0061B859
    Int(ini,value,(Word(p,8)>>3)&1u,"Reverse Minigame YAxis",kGame);           // 0061B85E
    Int(ini,value,Word(p,0x54),"Reverse Mouse Buttons",kGame);                 // 0061B8F8
    Int(ini,value,Byte(p,0x58),"EnableScreenShot",kGame);                      // 0061B98D
    Int(ini,value,Word(p,0x5c)&1u,"GUIsInScreenShot",kGame);                   // 0061BA23
    Int(ini,value,(Word(p,8)>>2)&1u,"AutoSave",kGame);                         // 0061BABA
    Int(ini,value,(Word(p,8)>>1)&1u,"Mouse Look",kGame);                       // 0061BB54
    Int(ini,value,Word(p,8)&1u,"Auto Level Up NPCs",kGame);                    // 0061BBED
    Int(ini,value,Byte(p,4),"Difficulty Level",kGame);                         // 0061BC84
    Int(ini,value,Byte(p,0x50),"Mouse Sensitivity",kGame);                     // 0061BD1A
    Int(ini,value,Word(p,0x10),"Enable Mouse Teleporting To Buttons",kGame);   // 0061BDB0
    Int(ini,value,(Word(p,0x14)>>5)&1u,"Status Summary",kGame);                // 0061BE45
    Int(ini,value,(Word(p,0x14)>>4)&1u,"Floating Numbers",kGame);
    Int(ini,value,(Word(p,0x14)>>3)&1u,"Mini Map",kGame);
    Int(ini,value,(Word(p,0x14)>>2)&1u,"Subtitles",kGame);
    Int(ini,value,(Word(p,0x14)>>1)&1u,"Tutorial Popups",kGame);
    Int(ini,value,Word(p,0x14)&1u,"Hide Unequippable",kGame);
    Int(ini,value,(Word(p,0x14)>>8)&1u,"Use Small Fonts",kGame);
    Int(ini,value,(Word(p,0x14)>>9)&1u,"Hide InGame GUI",kGame);
    Int(ini,value,Byte(p,0x16)&1u,"New Target Selected",kAutopause);          // byte [esi+16h] & 1
    Int(ini,value,(Word(p,0x14)>>15)&1u,"Action Menu",kAutopause);
    Int(ini,value,(Word(p,0x14)>>14)&1u,"Party Killed",kAutopause);
    Int(ini,value,(Word(p,0x14)>>12)&1u,"Enemy Sighted",kAutopause);
    Int(ini,value,(Word(p,0x14)>>13)&1u,"Mine Sighted",kAutopause);
    Int(ini,value,(Word(p,0x14)>>11)&1u,"End Of Combat Round",kAutopause);
    Int(ini,value,Byte(p,0x18),"Brightness",kGraphics);
    Int(ini,value,Word(p,0x24),"Shadows",kGraphics);
    Int(ini,value,Word(p,0x28),"Soft Shadows",kGraphics);
    Int(ini,value,Word(p,0x2c),"Grass",kGraphics);
    Int(ini,value,Byte(p,0x3c),"Texture Quality",kGraphics);
    Int(ini,value,Byte(p,0x44),"Anti Aliasing",kGraphics);
    Int(ini,value,Word(p,0x30),"Frame Buffer",kGraphics);
    Int(ini,value,Word(p,0x34),"Anisotropy",kGraphics);
    Int(ini,value,Word(p,0x40),"V-Sync",kGraphics);
    Int(ini,value,Byte(p,0x48),"Movie Volume",kSound);
    Int(ini,value,Byte(p,0x47),"Sound Effects Volume",kSound);
    Int(ini,value,Byte(p,0x46),"Voiceover Volume",kSound);
    Int(ini,value,Byte(p,0x45),"Music Volume",kSound);
    String name;o.default_string_005b3190(&name,nullptr);                     // 0061CE44..0061CE48
    for(I32 i=0;i<0xb;++i) {                                                   // 0061CE52..0061CF03 (EDI from 0061B7AC)
        o.format_005e5680(&name,kMovieFormat,i);                               // 0061CE52..0061CE5D
        o.format_005e5680(&value,kInt,static_cast<U32>(Byte(p,0x60+static_cast<std::size_t>(i)))); // 0061CE62..0061CE72
        Write(ini,value,WindowCStrView(&name),kMovies);                        // 0061CE7A..0061CEFA
    }
    Real(ini,value,Float(p,0x94),"Keyboard Camera DPS");                       // 0061CF09
    Real(ini,value,Float(p,0x98),"Keyboard Camera Acceleration");
    Real(ini,value,Float(p,0x9c),"Keyboard Camera Deceleration");
    Int(ini,value,(Word(p,0x14)>>10)&1u,"Enable Tooltips",kGame);
    o.destroy_string_005e5c20(&name,nullptr);                                  // 0061D17E..0061D186
    o.destroy_string_005e5c20(&value,nullptr);                                 // 0061D18B..0061D194
    o.destroy_ini_005e67e0(&ini,nullptr);                                      // 0061D199..0061D1A5
    return 1;                                                                  // 0061D1B0 (EBX=1)
}
}
