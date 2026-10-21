#include "e9734a9a27-dependencies.h"
namespace kotor_menu_10_19 {
String* Tlk::simple_0041e8f0(String* output,Word strref){
    StringResource result;
    result.text.default_005b3190();result.sound.default_00405ed0();
    write8(this,0x3c,0);
    (void)fetch_0041e1a0(strref,&result,0);



    bool display=read32(this,0x54)!=0;

    if(display){const char* text=result.text.text_005e5670();dependencies::format_005e5680(&result.text,"[%d]%s",strref,text);}
    (void)output->copy_005e5b00(&result.text);
    result.text.destroy_005e5c20();



    return output;

}
}
