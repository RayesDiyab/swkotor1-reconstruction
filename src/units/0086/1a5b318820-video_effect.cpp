// Existing W4 authored Enable/DisableVideoEffect bodies, with original String EH scopes restored.
// Donors lane-w4/build/t2-camera-arms/dialog_camera_arms.cpp and lane-w4/src/dialog/dialog_ingame.cpp.
#include "bbf0d3a26f-freelook.h"
#include "037606fe26-startup_string.h"
namespace options_video { struct Table; struct String; }
namespace menu_tables_required {
int __fastcall C2daIntEntry_00413660(options_video::Table*,void*,int,options_video::String*,int*);
int __fastcall C2daFloatEntry_00413430(options_video::Table*,void*,int,options_video::String*,float*);
}
extern "C" {
extern void* kotor_cell_rules_007a3a28;
extern volatile unsigned char fb_saturation_007a6881,fb_scan_noise_007a6882;
extern volatile float fb_saturation_param_0078db24,fb_saturation_param_0078db28,fb_saturation_param_0078db2c,fb_saturation_param_007a6884;
}
namespace {
unsigned load(void* p,unsigned o){return *reinterpret_cast<volatile unsigned*>(static_cast<unsigned char*>(p)+o);}
void store(void* p,unsigned o,unsigned v){*reinterpret_cast<volatile unsigned*>(static_cast<unsigned char*>(p)+o)=v;}
void* pointer(void* p,unsigned o){return reinterpret_cast<void*>(load(p,o));}
}
extern "C" void __fastcall kotor_fastcall_005f7710(void* internal,void*,int effect){
    int scan_noise=0,saturation_on=0;
    float red=0.f,green=0.f,blue=0.f,saturation=0.f;
    auto* table=static_cast<options_video::Table*>(pointer(pointer(kotor_cell_rules_007a3a28,0xb8),0x110));
    store(internal,0x3a8,0xfffffffe);
    fb_saturation_007a6881=0;fb_scan_noise_007a6882=0;
    if(table==nullptr || effect<0) return;
    int ok;
    {
        kotor_reconstruction::StartupString column("EnableScanNoise");
        ok=menu_tables_required::C2daIntEntry_00413660(table,nullptr,effect,reinterpret_cast<options_video::String*>(&column),&scan_noise);
    }
    if(ok!=0 && scan_noise!=0){store(internal,0x3a8,static_cast<unsigned>(effect));fb_scan_noise_007a6882=1;}
    {
        kotor_reconstruction::StartupString column("EnableSaturation");
        ok=menu_tables_required::C2daIntEntry_00413660(table,nullptr,effect,reinterpret_cast<options_video::String*>(&column),&saturation_on);
    }
    if(ok==0 || saturation_on==0) return;
    store(internal,0x3a8,static_cast<unsigned>(effect));
    const char* names[4]={"ModulationRed","ModulationGreen","ModulationBlue","Saturation"};
    float* outs[4]={&red,&green,&blue,&saturation};
    for(int i=0;i<4;++i){
        kotor_reconstruction::StartupString column(names[i]);
        menu_tables_required::C2daFloatEntry_00413430(table,nullptr,effect,reinterpret_cast<options_video::String*>(&column),outs[i]);
    }
    fb_saturation_007a6881=1;
    fb_saturation_param_0078db24=red;fb_saturation_param_0078db28=green;
    fb_saturation_param_0078db2c=blue;fb_saturation_param_007a6884=saturation;
}
extern "C" void __fastcall kotor_fastcall_005edf10(void* client,void*,int effect){kotor_fastcall_005f7710(pointer(client,4),nullptr,effect);}
extern "C" void __fastcall kotor_fastcall_005f40e0(void* internal,void*){
    store(internal,0x3a8,0xfffffffe);fb_saturation_007a6881=0;fb_scan_noise_007a6882=0;
}
extern "C" void __fastcall kotor_fastcall_005edf20(void* client,void*){kotor_fastcall_005f40e0(pointer(client,4),nullptr);}
