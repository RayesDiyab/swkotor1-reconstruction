#include "67a3cbeb44-options.h"
namespace options_video {
void __thiscall Client::InitializeClientOptions_005f1c20() {
    auto& b=options_bindings;Ini ini;String value;
    b.construct_ini_005e6750(&ini,nullptr);
    b.default_string_005b3190(&value,nullptr);
    String key,category,file;
    b.text_string_005e5a90(&key,nullptr,b.tooltip_key_0074e880);
    b.text_string_005e5a90(&category,nullptr,b.game_category_0073d658);
    b.text_string_005e5a90(&file,nullptr,b.ini_file_0073d648);
    const I32 tooltip=b.read_005e67c0(&ini,nullptr,&value,&file,&category,&key);
    b.destroy_string_005e5c20(&file,nullptr);b.destroy_string_005e5c20(&category,nullptr);b.destroy_string_005e5c20(&key,nullptr);
    if(tooltip==0 || b.as_int_005e5630(&value,nullptr)<=0) {
        if(tooltip!=0)b.format_005e5680(&value,b.one_00741e1c);
        b.text_string_005e5a90(&file,nullptr,b.tooltip_key_0074e880);
        b.text_string_005e5a90(&category,nullptr,b.game_category_0073d658);
        b.text_string_005e5a90(&key,nullptr,b.ini_file_0073d648);
        (void)b.write_005e67d0(&ini,nullptr,&value,&key,&category,&file);
        b.destroy_string_005e5c20(&key,nullptr);b.destroy_string_005e5c20(&category,nullptr);b.destroy_string_005e5c20(&file,nullptr);
    }
    b.text_string_005e5a90(&file,nullptr,b.hardware_key_0074e86c);
    b.text_string_005e5a90(&category,nullptr,b.graphics_category_0073d6ac);
    b.text_string_005e5a90(&key,nullptr,b.ini_file_0073d648);
    const I32 hardware=b.read_005e67c0(&ini,nullptr,&value,&key,&category,&file);
    b.destroy_string_005e5c20(&key,nullptr);b.destroy_string_005e5c20(&category,nullptr);b.destroy_string_005e5c20(&file,nullptr);
    if(hardware!=0) {
        void* const option=PointerAt(this,4);
        const I32 enabled=b.as_int_005e5630(&value,nullptr)!=0?1:0;
        b.hardware_mouse_0061d670(option,nullptr,enabled);
    } else {
        b.hardware_mouse_0061d670(PointerAt(this,4),nullptr,1);
        b.format_005e5680(&value,b.integer_format_0073d720,1);
        b.text_string_005e5a90(&file,nullptr,b.hardware_key_0074e86c);
        b.text_string_005e5a90(&category,nullptr,b.graphics_category_0073d6ac);
        b.text_string_005e5a90(&key,nullptr,b.ini_file_0073d648);
        (void)b.write_005e67d0(&ini,nullptr,&value,&key,&category,&file);
        b.destroy_string_005e5c20(&key,nullptr);b.destroy_string_005e5c20(&category,nullptr);b.destroy_string_005e5c20(&file,nullptr);
    }
    b.destroy_string_005e5c20(&value,nullptr);
    b.destroy_ini_005e67e0(&ini,nullptr);
}
}
