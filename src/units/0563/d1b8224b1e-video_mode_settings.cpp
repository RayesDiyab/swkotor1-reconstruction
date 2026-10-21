#include "b01b98864e-video_mode_settings.h"
#include <cstring>
// Owner declaration repeated exactly (lane-prepare game_prepare_producers.h:26, F8).
extern "C" std::uint32_t kotor_cell_allow_windowed_007a3a30;   // .data 0
namespace options_video {
namespace {
// Receiver fields beyond the task28 minimum are addressed bytewise with the
// original offsets (Client is the full 0x4D4 CClientExoAppInternal under P2).
void StoreWordAt(void* receiver,std::size_t offset,U32 value){std::memcpy(static_cast<U8*>(receiver)+offset,&value,4);}
U32 WordAt(const void* receiver,std::size_t offset){U32 value;std::memcpy(&value,static_cast<const U8*>(receiver)+offset,4);return value;}
void* PointerWordAt(const void* receiver,std::size_t offset){void* value;std::memcpy(&value,static_cast<const U8*>(receiver)+offset,4);return value;}
void ResetMode(DEVMODEA& mode){std::memset(&mode,0,sizeof mode);mode.dmSize=0x9c;} // rep stos 0x27 dwords + word dmSize
// x86 abs(a-b) exactly as SUB/CDQ/XOR/SUB: 32-bit wrap, INT_MIN stays INT_MIN.
I32 AbsDelta(U32 a,U32 b){const U32 x=a-b;const U32 s=(x&0x80000000u)?0xffffffffu:0u;return static_cast<I32>((x^s)-s);}
// D1 vtable-slot views (C-2): words read from the live vtable by memcpy, called
// with ECX=object, EDX unused (ABI-identical to the original thiscall slots).
using AuroraQuery=U32 (__fastcall*)(void*,void*);
using DeletingDestructor=void* (__fastcall*)(void*,void*,U32);
static_assert(sizeof(DEVMODEA)==0x9c,"DEVMODEA is 156 bytes");
}

// 005F0C60: ECX unused; two stack words; RET 8.
I32 __fastcall IsValidResolution_005f0c60(Client* self,void* unused_edx,I32 width,I32 height) {
    (void)self;(void)unused_edx;
    if(width==0x320)return height==0x258?1:0;                          // 005F0C64/005F0C6B
    if(width==0x400)return height==0x300?1:0;                          // 005F0C7D/005F0C84
    if(width==0x500)return (height==0x3c0||height==0x400)?1:0;         // 005F0C96..005F0CAD
    if(width==0x640)return height==0x4b0?1:0;                          // 005F0CB7/005F0CBE
    return 0;                                                          // 005F0CD0
}

// 005F0CE0 CClientExoAppInternal::ReadVideoModeSettings, RET 10h.
I32 __fastcall ReadVideoModeSettings_005f0ce0(Client* self,void* unused_edx,I32* width,I32* height,U8* depth,U32* refresh) {
    (void)unused_edx;
    auto& o=options_bindings;auto& v=video_mode_bindings;
    String value;Ini ini;String key,category,file;
    o.default_string_005b3190(&value,nullptr);                          // 005F0D09
    o.construct_ini_005e6750(&ini,nullptr);                             // 005F0D1A
    o.text_string_005e5a90(&key,nullptr,v.width_key_00749ff8);          // 005F0D2E
    o.text_string_005e5a90(&category,nullptr,o.graphics_category_0073d6ac); // 005F0D41
    o.text_string_005e5a90(&file,nullptr,o.ini_file_0073d648);          // 005F0D54
    const I32 width_found=o.read_005e67c0(&ini,nullptr,&value,&file,&category,&key); // 005F0D76
    o.destroy_string_005e5c20(&file,nullptr);                           // 005F0D88
    o.destroy_string_005e5c20(&category,nullptr);                       // 005F0D96
    o.destroy_string_005e5c20(&key,nullptr);                            // 005F0DA3
    *width=o.as_int_005e5630(&value,nullptr);                           // 005F0DAC, store 005F0DBD (unconditional)
    o.text_string_005e5a90(&key,nullptr,v.height_key_00747580);         // 005F0DBF
    o.text_string_005e5a90(&category,nullptr,o.graphics_category_0073d6ac); // 005F0DD2
    o.text_string_005e5a90(&file,nullptr,o.ini_file_0073d648);          // 005F0DE5
    (void)o.read_005e67c0(&ini,nullptr,&value,&file,&category,&key);    // 005F0E07 result ignored
    o.destroy_string_005e5c20(&file,nullptr);                           // 005F0E15
    o.destroy_string_005e5c20(&category,nullptr);                       // 005F0E23
    o.destroy_string_005e5c20(&key,nullptr);                            // 005F0E30
    *height=o.as_int_005e5630(&value,nullptr);                          // 005F0E39, store 005F0E41
    *depth=0x20;                                                        // 005F0E4F
    o.text_string_005e5a90(&key,nullptr,v.high_frequency_key_0074e818); // 005F0E52
    o.text_string_005e5a90(&category,nullptr,o.graphics_category_0073d6ac); // 005F0E65
    o.text_string_005e5a90(&file,nullptr,o.ini_file_0073d648);          // 005F0E78
    const I32 high_found=o.read_005e67c0(&ini,nullptr,&value,&file,&category,&key); // 005F0E9A
    o.destroy_string_005e5c20(&file,nullptr);                           // 005F0EAA
    o.destroy_string_005e5c20(&category,nullptr);                       // 005F0EB8
    o.destroy_string_005e5c20(&key,nullptr);                            // 005F0EC5
    if(high_found!=0)StoreWordAt(self,0x4c0,o.as_int_005e5630(&value,nullptr)!=0?1u:0u); // 005F0ED2..005F0EE1
    else StoreWordAt(self,0x4c0,0);                                     // 005F0EED
    o.text_string_005e5a90(&key,nullptr,v.refresh_key_0074e80c);        // 005F0F00
    o.text_string_005e5a90(&category,nullptr,o.graphics_category_0073d6ac); // 005F0F13
    o.text_string_005e5a90(&file,nullptr,o.ini_file_0073d648);          // 005F0F26
    const I32 refresh_found=o.read_005e67c0(&ini,nullptr,&value,&file,&category,&key); // 005F0F48
    o.destroy_string_005e5c20(&file,nullptr);                           // 005F0F58
    o.destroy_string_005e5c20(&category,nullptr);                       // 005F0F66
    o.destroy_string_005e5c20(&key,nullptr);                            // 005F0F73
    if(refresh_found!=0) {
        const U32 rate=static_cast<U32>(o.as_int_005e5630(&value,nullptr)); // 005F0F80
        *v.refresh_0078d1dc=rate;                                       // 005F0F88 global first
        *refresh=rate;                                                  // 005F0F8D
    } else {
        *refresh=*v.refresh_0078d1dc;                                   // 005F0F94/005F0F9A
    }
    if(IsValidResolution_005f0c60(self,nullptr,*width,*height)==0) {   // 005F0FA9 (ECX=receiver)
        *width=0x320;*height=0x258;                                     // 005F0FB2/005F0FB8
    }
    if(*depth!=0x20)*depth=0x20;                                        // 005F0FC1..005F0FC6
    if(*refresh<0x3c)*refresh=0x3c;                                     // 005F0FC9 unsigned JAE
    if(WordAt(self,0x4c0)==0 && *refresh>0x55)*refresh=0x3c;           // 005F0FD4..005F0FE3 unsigned JBE
    o.destroy_ini_005e67e0(&ini,nullptr);                               // 005F0FF2
    o.destroy_string_005e5c20(&value,nullptr);                          // 005F1003
    return width_found;                                                 // 005F100C
}

// 0061B710 CClientOptions::SetFullScreenEnabled: ECX=options, RET 4.
// NOTE: lane-menu-tables also authored this address (member form); single
// owner to be chosen by the integrator (see PACKET conflicts).
void __fastcall SetFullScreenEnabled_0061b710(void* options,void* unused_edx,I32 enabled) {
    (void)unused_edx;
    const U32 requested=static_cast<U32>(enabled);
    if(requested==((WordAt(options,0)>>3)&1u))return;                   // 0061B710..0061B71E full-dword compare
    kotor_menu_window_cells::reinit_007a3a2c=(requested!=0?1u:0u)+1u;  // 0061B720..0061B72B
    U32 flags=WordAt(options,0);                                        // 0061B731 reload
    flags^=((requested<<3)^flags)&8u;                                   // 0061B727/0061B733..0061B738
    StoreWordAt(options,0,flags);                                       // 0061B73A
}

// 0070D220 SetDisplayDevMode: cdecl; enumerated index or 0xFFFFFFFF.
U32 __cdecl SetDisplayDevMode_0070d220(U32 width,U32 height,U8 depth,U32 refresh) {
    DEVMODEA mode;U32 index=0;
    ResetMode(mode);                                                    // 0070D235..0070D24B
    if(EnumDisplaySettingsA(nullptr,index,&mode)==0)return 0xffffffffu; // 0070D252..0070D256
    for(;;) {
        if(mode.dmPelsWidth==width && mode.dmPelsHeight==height && mode.dmBitsPerPel==static_cast<U32>(depth)) { // 0070D263..0070D27D
            const U32 frequency=mode.dmDisplayFrequency;                // 0070D27F
            if(frequency==refresh || frequency==0 || frequency==1)return index; // 0070D286..0070D292
        }
        ResetMode(mode);                                                // 0070D294..0070D2A9
        ++index;                                                        // 0070D2A6
        if(EnumDisplaySettingsA(nullptr,index,&mode)==0)return 0xffffffffu; // 0070D2B0..0070D2B6
    }
}

// 0070D0A0 CAuroraInterface::GetNearestVideoMode: cdecl; best index or 0xFFFFFFFF.
U32 __cdecl GetNearestVideoMode_0070d0a0(U32 width,U32 height,U8 depth,U32 refresh) {
    DEVMODEA best,mode;
    std::memset(&best,0,sizeof best);                                   // 0070D0AF..0070D0BD (no dmSize)
    ResetMode(mode);                                                    // 0070D0BF..0070D0D3
    U32 chosen=0xffffffffu,index=0;                                     // 0070D0DA esi, 0070D0CF ebx
    if(EnumDisplaySettingsA(nullptr,index,&mode)==0)return chosen;      // 0070D0DD..0070D0E5
    do {
        bool take=false,eligible=true;
        if(mode.dmPelsWidth<0x280 || mode.dmPelsHeight<0x1e0 || mode.dmBitsPerPel<0x10)eligible=false; // 0070D0F0..0070D116 unsigned
        const U32 frequency=mode.dmDisplayFrequency;                    // 0070D11C
        if(eligible && frequency<0x3c && frequency!=0 && frequency!=1)eligible=false;      // 0070D123..0070D12F
        if(eligible) {
            const U32 d=depth;
            const I32 cur_bits=AbsDelta(mode.dmBitsPerPel,d),best_bits=AbsDelta(best.dmBitsPerPel,d); // 0070D135..0070D14E
            if(cur_bits<best_bits)take=true;                            // 0070D152 JL
            else if(cur_bits==best_bits) {                              // 0070D154 JNE skip
                const I32 cw=AbsDelta(mode.dmPelsWidth,width),bw=AbsDelta(best.dmPelsWidth,width); // 0070D15A..0070D176
                if(cw<bw)take=true;                                     // 0070D17A
                else if(cw==bw) {                                       // 0070D17C
                    const I32 ch=AbsDelta(mode.dmPelsHeight,height),bh=AbsDelta(best.dmPelsHeight,height); // 0070D17E..0070D19D
                    if(ch<bh)take=true;                                 // 0070D1A1
                    else if(ch==bh) {                                   // 0070D1A3
                        const I32 bf=AbsDelta(best.dmDisplayFrequency,refresh),cf=AbsDelta(frequency,refresh); // 0070D1A5..0070D1C4
                        if(cf<bf)take=true;                             // 0070D1C6 JGE skip
                    }
                }
            }
        }
        if(take){std::memcpy(&best,&mode,sizeof best);chosen=index;}   // 0070D1CA..0070D1DC
        ResetMode(mode);                                                // 0070D1DE..0070D1F3
        ++index;                                                        // 0070D1F0
    } while(EnumDisplaySettingsA(nullptr,index,&mode)!=0);              // 0070D1FA..0070D202
    return chosen;                                                      // 0070D209
}

// 005F1830 CClientExoAppInternal::SetVideoMode(mode,style): RET 8, returns 0/1.
I32 __fastcall SetVideoMode_005f1830(Client* self,void* unused_edx,U32 mode,U8 style) {
    (void)unused_edx;auto& v=video_mode_bindings;
    if(mode==WordAt(self,0x1a0))return 1;                               // 005F1857/005F185F -> 005F1A79
    DEVMODEA display;ResetMode(display);                                // 005F1865..005F187A
    if(EnumDisplaySettingsA(nullptr,mode,&display)==0)return 0;         // 005F1881..005F189F (EAX=API result 0)
    *v.depth_0078d1e0=static_cast<U8>(display.dmBitsPerPel);           // 005F18A2/005F18B7 low byte
    *v.width_0078d1d4=display.dmPelsWidth;                              // 005F18C1
    *v.height_0078d1d8=display.dmPelsHeight;                            // 005F18C7
    U32 fullscreen;
    if(style==1)fullscreen=0;                                           // 005F18BF..005F18CF
    else if(style==2)fullscreen=1;                                      // 005F18D9..005F18DD
    else {                                                              // 005F18E7..005F18F2 CAurora vtbl+4
        void* const aurora=kotor_menu_window_cells::aurora_007a39f8;
        AuroraQuery query;std::memcpy(&query,static_cast<U8*>(PointerWordAt(aurora,0))+4,4);
        fullscreen=query(aurora,nullptr);
    }
    const U32 saved_width=*v.width_0078d1d4,saved_height=*v.height_0078d1d8; // 005F18FD/005F1903 before the test
    if(kotor_menu_window_cells::reinit_007a3a2c==1) {                   // 005F18F6/005F1909
        void* const aurora=kotor_menu_window_cells::aurora_007a39f8;
        AuroraQuery query;std::memcpy(&query,static_cast<U8*>(PointerWordAt(aurora,0))+0x18,4);
        (void)query(aurora,nullptr);                                            // 005F1913 vtbl+18, EAX ignored
    }
    // D2 (005F1916 [esp+10h] RECT): no initializing store in the original;
    // GetWindowRect 005F192E fills it on success and its failure result is
    // ignored, so a later read would be UB in C++: value-initialized to 0.
    RECT desktop{};
    *v.width_0078d1d4=saved_width;*v.height_0078d1d8=saved_height;      // 005F191B/005F1921 restore
    GetWindowRect(GetDesktopWindow(),&desktop);                         // 005F1927/005F192E, result ignored
    const U32 w=*v.width_0078d1d4,h=*v.height_0078d1d8;                 // 005F193B/005F1941
    const U32 bits=*v.depth_0078d1e0;                                   // zero-extended byte pushes
    if(kotor_cell_allow_windowed_007a3a30==0) {                         // 005F1934..005F1949
        v.reinit_aurora_00403800(w,h,bits,1,1);                         // 005F19A5..005F19B2
    } else if(fullscreen!=0) {                                          // 005F1950..005F1956
        v.reinit_aurora_00403800(w,h,bits,fullscreen,1);                // 005F19A1
    } else if(desktop.bottom<=0x1df || desktop.right<=0x27f) {          // 005F195C..005F196D signed JLE
        v.reinit_aurora_00403800(*v.width_0078d1d4,*v.height_0078d1d8,bits,1,1); // 005F196F..005F1988 reloads w/h
    } else if(desktop.bottom>static_cast<I32>(h) && desktop.right>static_cast<I32>(w)) { // 005F198A..005F1990 signed
        v.reinit_aurora_00403800(w,h,bits,fullscreen,1);                // 005F19A1 (fullscreen==0): windowed
    } else {
        v.reinit_aurora_00403800(w,h,bits,1,1);                         // 005F1992..005F19B2
    }
    StoreWordAt(self,0x1a0,mode);                                       // 005F19B7/005F19BA
    void* const gui=PointerWordAt(self,0x274);                          // 005F19C0
    if(gui!=nullptr) {
        v.gui_set_size_0040be70(gui,nullptr,static_cast<I32>(*v.width_0078d1d4),static_cast<I32>(*v.height_0078d1d8)); // 005F19D1..005F19DE
        void* const menu=PointerWordAt(self,0x280);                     // 005F19E3
        if(menu!=nullptr && v.gui_panel_exists_0040bd70(PointerWordAt(self,0x274),nullptr,menu)!=0) // 005F19ED..005F19FB
            v.main_menu_load_layout_0067ace0(PointerWordAt(self,0x280),nullptr,PointerWordAt(self,0x274)); // 005F19FD..005F1A0A
        void* const load=PointerWordAt(self,0x278);                     // 005F1A0F
        if(load!=nullptr) {
            DeletingDestructor destroy;std::memcpy(&destroy,PointerWordAt(load,0),4);
            (void)destroy(load,nullptr,1);                                            // 005F1A19..005F1A1D vtbl+0, flag 1
            void* const memory=v.allocate_006fa7e6(0x6b8);              // 005F1A24
            void* const screen=memory?v.load_screen_0067a710(memory,nullptr,PointerWordAt(self,0x274)):nullptr; // 005F1A3F..005F1A4F
            StoreWordAt(self,0x278,static_cast<U32>(reinterpret_cast<std::uintptr_t>(screen))); // 005F1A5C
        }
        void* const in_game=PointerWordAt(self,0x40);                   // 005F1A62
        if(in_game!=nullptr)v.in_game_reset_0062f5f0(in_game,nullptr);  // 005F1A6B
    }
    return 1;                                                           // 005F1A79
}

// F1: the three Views words are attached at the original publication point by
// the game_prepare owner (lane-prepare); definition storage in lane_config_tables.cpp.
}
