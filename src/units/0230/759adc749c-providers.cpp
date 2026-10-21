#include "8ba4a4693e-providers.h"
using namespace menu_providers_40_49;
AI* __thiscall AI::construct_00604b20(){
    providers.list_construct_004b25d0(&list08,nullptr);
    word0c=0;word04=0;return this;
}
void __thiscall AI::set_app_00604790(U32 app){app_bits=app;}
void __thiscall Options::antialias_0061d710(U8 value){
    antialias44=value;



    (void)providers.aa_0044f2f0(value);

}
void __thiscall Options::anisotropy_0061d8c0(U32 value){anisotropy34=value;(void)providers.aniso_0044f2c0(static_cast<int>(value));}
void __thiscall Options::vsync_0061d8e0(U32 value){



    vsync40=value;

    providers.vsync_0044ee90(value!=0);
}
void __thiscall Options::defaults_0061da60(){
    flags08=(flags08&0xfffffff4u)|0x14u;
    flags14=(flags14&0xfffffcbeu)|0x4beu;
    enabled04=1;byte80=2;
    void* gui=*providers.gui_007a39f4;
    if(gui)providers.gui_0040b420(gui,nullptr);
    flags14=(flags14&0xffff77ffu)|0x17000u;
    byte45=byte46=byte47=0x55;
    const U32 f5c=flags5c|1u;
    mode18=0x39;float1c=0x3e6b851f;float20=0x40000000;
    word24=word2c=word30=anisotropy34=word38=word28=1;
    byte3d=1;byte3c=0;vsync40=0;antialias44=0;byte48=100;
    word54=0;byte50=0x2c;byte58=0;flags5c=f5c;
    providers.options_0061d9e0(this,nullptr);



    std::memset(array60,0,11);

    word90=word88=word8c=0;
}
void __thiscall Internal::mouse_0061f980(U32 first,U32 second,U32 third){
    void* input=*providers.input_007a39e4;



    if(input)first=providers.input_cursor_005df5c0(input,nullptr,first,second,third);

    void* gui=pointer(0x274);
    if(gui)providers.gui_cursor_0040a270(gui,nullptr,first,second,third);
}
