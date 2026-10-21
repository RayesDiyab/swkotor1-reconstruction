#include "34dc360361-client_options_load.h"
#include <cstring>
namespace client_options {
namespace {
U32 Word(const void* p,std::size_t o){U32 v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
void PutWord(void* p,std::size_t o,U32 v){std::memcpy(static_cast<U8*>(p)+o,&v,4);}
void PutByte(void* p,std::size_t o,U8 v){static_cast<U8*>(p)[o]=v;}
void PutFloatBits(void* p,std::size_t o,U32 bits){std::memcpy(static_cast<U8*>(p)+o,&bits,4);}
void* Pointer(const void* p,std::size_t o){void* v;std::memcpy(&v,static_cast<const U8*>(p)+o,4);return v;}
// bit n of the word at o := bit 0 of value (original SHL/XOR/AND/XOR idiom)
void PutBit(void* p,std::size_t o,unsigned n,I32 value){const U32 f=Word(p,o);PutWord(p,o,f^(((static_cast<U32>(value)<<n)^f)&(1u<<n)));}
// Pinned PE literals (addresses in comments).
const char kIni[]=".\\swkotor.ini";            // 0073D648
const char kGame[]="Game Options";             // 0073D658
const char kAutopause[]="Autopause Options";   // 0074F9EC
const char kGraphics[]="Graphics Options";     // 0073D6AC
const char kSound[]="Sound Options";           // 0073D724
const char kMovies[]="Movies Shown";           // 0074F900
const char kMovieFormat[]="Movie%2d";          // 0074F910
// Key/section/file construction order and file,section,key destruction
// order are the same in every block (e.g. 0061DC2C..0061DCA4).
I32 Read(Ini& ini,String& value,const char* key,const char* section) {
    auto& o=options_video::options_bindings;String k,s,f;
    o.text_string_005e5a90(&k,nullptr,key);o.text_string_005e5a90(&s,nullptr,section);o.text_string_005e5a90(&f,nullptr,kIni);
    const I32 found=o.read_005e67c0(&ini,nullptr,&value,&f,&s,&k);
    o.destroy_string_005e5c20(&f,nullptr);o.destroy_string_005e5c20(&s,nullptr);o.destroy_string_005e5c20(&k,nullptr);
    return found;
}
I32 AsInt(String& value){return options_video::options_bindings.as_int_005e5630(&value,nullptr);}
// 005E5650 returns its float in ST0 (D1 float-return slot, C-3). Both original
// stores (FST global / FSTP member) round that same value to float32.
float AsFloat(String& value){return client_options_bindings.as_float_005e5650(&value,nullptr);}
// 0061F338..0061F356 pattern: global updated only when it is not equal to the
// -1.0 sentinel (FCOMP/FNSTSW/TEST AH,44h/JNP; unordered also stores).
void Camera(void* self,std::size_t member,float* global,Ini& ini,String& value,const char* key) {
    if(Read(ini,value,key,kGame)==0)return;
    const float read=AsFloat(value);
    const float current=*global;
    if(!(current==*client_options_bindings.sentinel_00740cbc))*global=read;
    std::memcpy(static_cast<U8*>(self)+member,&read,4);
}
}

// 0061DB60 CClientOptions::CClientOptions: ECX=this, RET, EAX=this.
void* __fastcall CClientOptions_0061db60(void* self,void* unused_edx) {
    (void)unused_edx;auto& b=client_options_bindings;
    // D2 (0061DB63 [this+0], 0061DBC1 [this+14h]): the original read-modify-writes
    // these words of a fresh 006FA7E6 block (operator new 0xA0 at 005FC681, no
    // initializing store), preserving heap bits outside the masks. In C++ that
    // read is indeterminate; the faithful alternative is for the allocating caller
    // (005FC67C..005FC693 owner) to zero words 0 and 0x14 before this call.
    PutWord(self,0,(Word(self,0)&0xfffffff6u)|0x54u);                   // 0061DB63..0061DB6D
    PutByte(self,0x6d,3);PutByte(self,0x6e,1);PutByte(self,0x73,0x10);  // 0061DB79..0061DB80
    PutByte(self,0x6b,0);PutByte(self,0x72,2);PutByte(self,0x6c,2);     // 0061DB84..0061DB8B
    PutFloatBits(self,0x78,0x40400000u);PutFloatBits(self,0x7c,0x3f800000u); // 0061DB8E/0061DB95
    PutByte(self,0x6f,3);PutByte(self,0x70,2);PutByte(self,0x71,1);     // 0061DB98..0061DB9F
    PutByte(self,0x80,2);PutFloatBits(self,0x84,0x40a00000u);           // 0061DBA2/0061DBA8
    PutFloatBits(self,0x0c,0x3f800000u);                                // 0061DBB2
    void* const gui=*b.gui_manager_007a39f4;                            // 0061DBB5
    if(gui!=nullptr)PutFloatBits(gui,0x48,0x3f800000u);                 // 0061DBBE
    PutWord(self,0x14,Word(self,0x14)|0x400u);                          // 0061DBC1..0061DBCB
    PutWord(self,0x10,1);                                               // 0061DBCE
    b.set_defaults_0061da60(self,nullptr);                              // 0061DBD1
    return self;                                                        // 0061DBD6
}

// 0061DBE0 CClientOptions::LoadOptions: ECX=this, RET, EAX=1.
I32 __fastcall LoadOptions_0061dbe0(void* self,void* unused_edx) {
    (void)unused_edx;auto& o=options_video::options_bindings;auto& b=client_options_bindings;
    b.set_defaults_0061da60(self,nullptr);                              // 0061DC03
    Ini ini;String value;
    o.construct_ini_005e6750(&ini,nullptr);                             // 0061DC0C
    o.default_string_005b3190(&value,nullptr);                          // 0061DC1D
    if(Read(ini,value,"Difficulty Level",kGame))PutByte(self,4,static_cast<U8>(AsInt(value))); // 0061DC7C/0061DCB9
    if(Read(ini,value,"Auto Level Up NPCs",kGame)) {                    // 0061DD0D
        const I32 v=AsInt(value);const U32 f=Word(self,8);
        const U32 n=f^((f^static_cast<U32>(v))&1u);PutWord(self,8,n);  // 0061DD4A..0061DD5A
        if((n&1u)!=0) {                                                 // 0061DD56..0061DD5D
            void* const server=Pointer(kotor_menu_window_cells::manager_007a39fc,8); // 0061DD5F/0061DD64
            if(server!=nullptr)b.auto_level_up_npcs_00563b90(b.get_party_table_004aee70(server,nullptr),nullptr); // 0061DD6B..0061DD72
        }
    }
    if(Read(ini,value,"Mouse Look",kGame))PutBit(self,8,1,AsInt(value));            // 0061DDC8..0061DE14
    if(Read(ini,value,"AutoSave",kGame)) {                              // 0061DE68
        const U32 f=Word(self,8);
        const U32 a=(((static_cast<U32>(AsInt(value))<<2)^f)&4u)^f;   // 0061DEA5..0061DEB2
        PutWord(self,8,((((a>>2)&1u)<<2)^a)&4u^a);                      // 0061DEB4..0061DEC5 (identity re-merge)
    }
    if(Read(ini,value,"Reverse Minigame YAxis",kGame))PutBit(self,8,3,AsInt(value)); // 0061DF19..0061DF65
    if(Read(ini,value,"Combat Movement",kGame))PutBit(self,8,4,AsInt(value));        // 0061DFB9..0061E005
    if(Read(ini,value,"Mouse Sensitivity",kGame))PutByte(self,0x50,static_cast<U8>(AsInt(value))); // 0061E059/0061E096
    if(Read(ini,value,"Enable Mouse Teleporting To Buttons",kGame))PutWord(self,0x10,static_cast<U32>(AsInt(value))); // 0061E0EA/0061E127
    if(Read(ini,value,"Hide Unequippable",kGame))PutBit(self,0x14,0,AsInt(value));   // 0061E17B..0061E1C4
    if(Read(ini,value,"Tutorial Popups",kGame))PutBit(self,0x14,1,AsInt(value));     // 0061E218..0061E264
    if(Read(ini,value,"Subtitles",kGame))PutBit(self,0x14,2,AsInt(value));           // 0061E2B8..0061E304
    if(Read(ini,value,"Mini Map",kGame))PutBit(self,0x14,3,AsInt(value));            // 0061E358..0061E3A8
    if(Read(ini,value,"Floating Numbers",kGame))PutBit(self,0x14,4,AsInt(value));    // 0061E3FC..0061E448
    if(Read(ini,value,"Status Summary",kGame))PutBit(self,0x14,5,AsInt(value));      // 0061E49C..0061E4E8
    if(Read(ini,value,"Use Small Fonts",kGame)) {                       // 0061E53C
        const I32 v=AsInt(value);
        PutBit(self,0x14,8,v);                                          // 0061E579..0061E592
        PutByte(self,0x80,static_cast<U8>((v==0?1:0)+1));               // 0061E58B..0061E595
        void* const gui=*b.gui_manager_007a39f4;                        // 0061E59B
        if(gui!=nullptr)b.update_all_fonts_0040b420(gui,nullptr);       // 0061E5A5
    }
    if(Read(ini,value,"Hide InGame GUI",kGame))PutBit(self,0x14,9,AsInt(value));             // 0061E5FB..0061E649
    if(Read(ini,value,"End Of Combat Round",kAutopause))PutBit(self,0x14,11,AsInt(value));   // 0061E69D..0061E6EB
    if(Read(ini,value,"Enemy Sighted",kAutopause))PutBit(self,0x14,12,AsInt(value));         // 0061E73F..0061E78D
    if(Read(ini,value,"Mine Sighted",kAutopause))PutBit(self,0x14,13,AsInt(value));          // 0061E7E1..0061E82F
    if(Read(ini,value,"Party Killed",kAutopause))PutBit(self,0x14,14,AsInt(value));          // 0061E883..0061E8D1
    if(Read(ini,value,"Action Menu",kAutopause))PutBit(self,0x14,15,AsInt(value));           // 0061E925..0061E973
    if(Read(ini,value,"New Target Selected",kAutopause))PutBit(self,0x14,16,AsInt(value));   // 0061E9C7..0061EA15
    if(Read(ini,value,"Brightness",kGraphics))PutByte(self,0x18,static_cast<U8>(AsInt(value))); // 0061EA69/0061EAA6
    if(Read(ini,value,"Shadows",kGraphics)) {                           // 0061EAFA
        const I32 v=AsInt(value);PutWord(self,0x24,static_cast<U32>(v));// 0061EB39
        if(v!=0)b.enable_shadows_0044ee20();else b.disable_shadows_0044f080(); // 0061EB3C..0061EB45
    }
    if(Read(ini,value,"AllowSoftShadows",kGraphics) && AsInt(value)==1)b.unlock_soft_shadows_0044ef90(); // 0061EB9B..0061EBDC
    if(Read(ini,value,"Soft Shadows",kGraphics)) {                      // 0061EC32
        const I32 v=AsInt(value);PutWord(self,0x28,static_cast<U32>(v));// 0061EC71
        if(v!=0)b.enable_soft_shadows_0044ee50();else b.disable_soft_shadows_0044ee60(); // 0061EC74..0061EC7D
    }
    if(Read(ini,value,"Grass",kGraphics)) {                             // 0061ECD3
        const I32 v=AsInt(value);PutWord(self,0x2c,static_cast<U32>(v));// 0061ED12
        if(v!=0)b.enable_grass_0044ee30();else b.disable_grass_0044ee40(); // 0061ED15..0061ED1E
    }
    if(Read(ini,value,"Texture Quality",kGraphics)) {                   // 0061ED74
        const U8 v=static_cast<U8>(AsInt(value));PutByte(self,0x3c,v);  // 0061EDB1
        void* const app=Pointer(kotor_menu_window_cells::manager_007a39fc,4); // 0061EDB4/0061EDBA
        b.set_texture_pack_005ed8f0(app,nullptr,v);                     // 0061EDBD..0061EDC6 (low byte; upper bytes stale in original)
    }
    if(Read(ini,value,"Frame Buffer",kGraphics))b.set_frame_buffer_0061d750(self,nullptr,AsInt(value)); // 0061EE1C..0061EE5C
    if(Read(ini,value,"V-Sync",kGraphics)) {                            // 0061EEB2
        const I32 v=AsInt(value);PutWord(self,0x40,static_cast<U32>(v));// 0061EEF1
        b.set_vsync_state_0044ee90(static_cast<unsigned char>(v!=0?1:0));                           // 0061EEF4..0061EF00 cdecl
    }
    if(Read(ini,value,"Music Volume",kSound))PutByte(self,0x45,static_cast<U8>(AsInt(value)));        // 0061EF54/0061EF91
    if(Read(ini,value,"Voiceover Volume",kSound))PutByte(self,0x46,static_cast<U8>(AsInt(value)));    // 0061EFE5/0061F022
    if(Read(ini,value,"Sound Effects Volume",kSound))PutByte(self,0x47,static_cast<U8>(AsInt(value)));// 0061F076/0061F0B3
    if(Read(ini,value,"Movie Volume",kSound))PutByte(self,0x48,static_cast<U8>(AsInt(value)));        // 0061F107/0061F144
    b.apply_sound_options_0061d1c0(self,nullptr);                       // 0061F147/0061F149
    String name;o.default_string_005b3190(&name,nullptr);              // 0061F14E/0061F152
    for(I32 index=0;index<0xb;++index) {                                // 0061F15C..0061F209
        o.format_005e5680(&name,kMovieFormat,index);                    // 0061F160..0061F170
        (void)Read(ini,value,b.c_str_005e5670(&name,nullptr),kMovies);  // 0061F173..0061F1F3 result ignored
        PutByte(self,0x60+static_cast<std::size_t>(index),static_cast<U8>(AsInt(value))); // 0061F1FC/0061F201 unconditional
    }
    PutWord(self,0x90,0);                                               // 0061F218
    if(Read(ini,value,"EnableCheats",kGame))PutWord(self,0x90,static_cast<U32>(AsInt(value))); // 0061F26A/0061F2A8
    Camera(self,0x94,b.camera_dps_007a2320,ini,value,"Keyboard Camera DPS");            // 0061F2FF..0061F356
    Camera(self,0x98,b.camera_accel_007a2324,ini,value,"Keyboard Camera Acceleration"); // 0061F3AD..0061F404
    Camera(self,0x9c,b.camera_decel_007a2328,ini,value,"Keyboard Camera Deceleration"); // 0061F45B..0061F4B2
    if(Read(ini,value,"Enable Tooltips",kGame))PutBit(self,0x14,10,AsInt(value));       // 0061F509..0061F558
    if(Read(ini,value,"Reverse Mouse Buttons",kGame))PutWord(self,0x54,static_cast<U32>(AsInt(value))); // 0061F5AC/0061F5EA
    if(Read(ini,value,"EnableScreenShot",kGame))PutByte(self,0x58,static_cast<U8>(AsInt(value)));     // 0061F63E/0061F67C
    if(Read(ini,value,"GUIsInScreenShot",kGame))PutBit(self,0x5c,0,AsInt(value));       // 0061F6D0..0061F71A
    o.destroy_string_005e5c20(&name,nullptr);                           // 0061F721
    o.destroy_string_005e5c20(&value,nullptr);                          // 0061F72E
    o.destroy_ini_005e67e0(&ini,nullptr);                               // 0061F744
    return 1;                                                           // 0061F74F (EBX=1)
}

// 0061D1C0 CClientOptions::ApplySoundOptions: ECX=options, RET.
void __fastcall ApplySoundOptions_0061d1c0(void* self,void* unused_edx) {
    (void)unused_edx;auto& b=client_options_bindings;
    if(*b.sound_007a39ec==nullptr)return;                               // 0061D1C4..0061D1CC
    const float k=*b.volume_scale_0073dfec;                             // [0073DFEC]
    // FILD (exact int) * FMUL m32 then FSTP m32: one rounding of the exact product.
    b.sound_music_volume_005d5c50(*b.sound_007a39ec,nullptr,static_cast<float>(static_cast<I32>(static_cast<U8*>(self)[0x45]))*k,0); // 0061D1CE..0061D1E6
    b.sound_dialog_volume_005d5d50(*b.sound_007a39ec,nullptr,static_cast<float>(static_cast<I32>(static_cast<U8*>(self)[0x46]))*k); // 0061D1EB..0061D207 (cell reloaded)
    b.sound_effect_volume_005d5cd0(*b.sound_007a39ec,nullptr,static_cast<float>(static_cast<I32>(static_cast<U8*>(self)[0x47]))*k); // 0061D20C..0061D228
}

// 005ED8F0 CClientExoApp::SetTexturePack(byte): mov ecx,[ecx+4]; push 1; push index; call 005F14A0; RET 4.
void __fastcall ClientExoAppSetTexturePack_005ed8f0(void* app,void* unused_edx,U8 index) {
    (void)unused_edx;
    (void)static_cast<options_video::Client*>(Pointer(app,4))->SetTexturePack_005f14a0(index,1); // 005ED8F0..005ED8FA
}
}
