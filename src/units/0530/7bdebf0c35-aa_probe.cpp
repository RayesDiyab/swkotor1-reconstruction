#include "f2a58ba33f-aa_probe.h"
#include <cstring>
namespace kotor_renderer_init {
static void Store16(PixelFormat& format,std::size_t offset,U16 value) {
    std::memcpy(format.bytes+offset,&value,sizeof(value));
}
static void Store32(PixelFormat& format,std::size_t offset,U32 value) {
    std::memcpy(format.bytes+offset,&value,sizeof(value));
}
void __cdecl InitializeAntiAliasing_0042e040() {
    WindowClass window_class{};
    window_class.style=0x0b;
    window_class.procedure=*aa_probe.def_window_proc_iat_0073d3b0;
    window_class.instance=*aa_probe.instance_007a39c0;
    window_class.background=GetStockObject_0073d01c(4);
    window_class.class_name=aa_probe.class_name_0078e310;
    RegisterClassA_0073d41c(&window_class);
    void* window=CreateWindowExA_0073d410(0,aa_probe.class_name_0078e310,
        aa_probe.window_title_0073f478,0x06000000,0,0,(-2147483647-1),
        (-2147483647-1),nullptr,nullptr,nullptr,nullptr);
    PixelFormat format{};
    Store16(format,0,40);Store16(format,2,1);Store32(format,4,0x25);
    format.bytes[9]=32;format.bytes[23]=24;format.bytes[24]=8;
    void* dc=GetDC_0073d408(window);
    I32 pixel_format=ChoosePixelFormat_0073d018(dc,&format);
    if (SetPixelFormat_0073d014(dc,pixel_format,nullptr)==0) {
        ReleaseDC_0073d404(window,dc);
        return;
    }
    DescribePixelFormat_0073d008(dc,pixel_format,40,&format);
    void* context=wglCreateContext_0073d2b8(dc);
    wglMakeCurrent_0073d2b0(dc,context);
    const LoadProcedure load=*aa_probe.load_proc_iat_0073d310;
    *aa_probe.extension_proc_007bb7ec=reinterpret_cast<ExtensionString>(
        load(aa_probe.extension_proc_name_0073f45c));
    *aa_probe.choose_proc_007bb730=reinterpret_cast<ChooseArb>(
        load(aa_probe.choose_proc_name_0073f444));
    const char* gl_extensions=glGetString_0073d290(0x1f03);
    const bool ati=CheckExtension_0045f980(aa_probe.ati_extension_name_0073f42c,
        gl_extensions)!=0;
    const ExtensionString extensions=*aa_probe.extension_proc_007bb7ec;
    if (extensions!=nullptr) {
        const char* wgl_extensions=extensions(dc);
        *aa_probe.extension_string_007a688c=wgl_extensions;
        if (wgl_extensions!=nullptr && CheckExtension_0045f980(
            aa_probe.multisample_name_0073f418,wgl_extensions)!=0) {
            bool enable=!ati;
            if (ati) {
                VersionInfo version{};version.size=148;
                enable=GetVersionExA_0073d060(&version)!=0 && version.major>4u;
            }
            if (enable) *aa_probe.capabilities_007bb85c |= *aa_probe.multisample_mask_0078e4ec;
        }
    }
    wglDeleteContext_0073d30c(context);
    ReleaseDC_0073d404(window,dc);
    DestroyWindow_0073d3fc(window);
}
}
