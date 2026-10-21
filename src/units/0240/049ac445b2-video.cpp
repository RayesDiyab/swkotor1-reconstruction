#include "6b902ac777-video.h"
namespace options_video {
void __thiscall Client::ReadAndSetVideoMode_005f5ab0(I32 forMovie,I32 fullscreen) {
    auto& b=video_bindings;
    I32 width=0,height=0;U8 depth=0;U32 refresh=0;
    String temporary;Ini ini;
    b.default_string_005b3190(&temporary,nullptr);
    b.construct_ini_005e6750(&ini,nullptr);
    const I32 settings=b.read_settings_005f0ce0(this,nullptr,&width,&height,&depth,&refresh);
    b.fullscreen_0061b710(PointerAt(this,4),nullptr,fullscreen);
    if(forMovie!=0) {
        width=640;height=480;depth=32;
        *b.movie_flag_007a3c54=1;
    }
    U32 mode=b.exact_mode_0070d220(static_cast<U32>(width),static_cast<U32>(height),depth,refresh);
    if((settings==0 && forMovie==0) || mode==0xffffffffu) {
        const U32 nearest=b.nearest_mode_0070d0a0(800,600,32,60);
        DisplayMode display{};
        const U16 size=156;std::memcpy(display.bytes+36,&size,2);
        const I32 enumerated=b.enum_modes_0073d3e4(nullptr,nearest,&display);
        mode=(enumerated!=0 && IntegerAt(&display,104)==32)?nearest:0xffffffffu;
    }
    (void)b.set_mode_005f1830(this,nullptr,mode,fullscreen!=0?2:1);
    if(forMovie!=0) {
        (void)b.read_settings_005f0ce0(this,nullptr,&width,&height,&depth,&refresh);
        *b.saved_width_0078d1d4=static_cast<U32>(width);
        *b.saved_height_0078d1d8=static_cast<U32>(height);
        *b.saved_depth_0078d1e0=depth;
    }
    b.destroy_ini_005e67e0(&ini,nullptr);
    b.destroy_string_005e5c20(&temporary,nullptr);
}
}
