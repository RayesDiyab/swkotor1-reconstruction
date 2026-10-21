#include "7514f48b34-window_shell.h"
namespace kotor_window_shell {
Context* bound_context=nullptr;
static Word pointer(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
static Word call(Word site,Word target,Word self,std::initializer_list<Word> args={}) {
    Call request{};request.site=site;request.target=target;request.self=self;
    request.count=static_cast<Word>(args.size());Word n=0;for(Word value:args)request.args[n++]=value;
    return static_cast<Word>(bound_context->dependencies.invoke(bound_context->dependencies.context,request));
}
static void construct(Word site,String& value,Word text){call(site,0x005e5a90,pointer(&value),{text});}
static void destroy(Word site,String& value){call(site,0x005e5c20,pointer(&value));}
static void class_fields(Class& value,Word instance,Word name) {
    value.style=0x0b;value.procedure=0x00402800;value.class_extra=0;value.window_extra=0;
    value.instance=instance;value.cursor=0;value.menu=0;value.class_name=name;
}
void __cdecl init_render_window_004036f0(Word instance,std::int32_t show) {
    (void)show;Globals& g=*bound_context->globals;Frame& f=*bound_context->frame;
    // Three DWORDs plus one WORD are copied in the original: exactly14bytes.
    for(Word n=0;n<14;++n)f.render_class_name[n]=bound_context->constants->render_class[n];
    call(0x0040373b,0x005e5b00,pointer(&f.render_title),{pointer(&g.title_7a3ccc)});
    Class& wc=f.render_class;class_fields(wc,instance,pointer(f.render_class_name));
    wc.icon=call(0x00403769,0x0073d424,0,{instance,0x65});
    wc.brush=call(0x00403779,0x0073d01c,0,{4});
    call(0x00403794,0x0073d41c,0,{pointer(&wc)});
    Word title=call(0x004037bb,0x005e5670,pointer(&f.render_title));
    Word window=call(0x004037cb,0x0073d410,0,{0x40000,pointer(f.render_class_name),title,0x02ca0000,
        0x80000000,0x80000000,0x80000000,0x80000000,0,0,instance,0});



    if(window)g.render_window_7a39d8=window;

    destroy(0x004037e8,f.render_title);
}
Word __cdecl init_game_app_00403f20(Word instance,std::int32_t show) {
    Globals& g=*bound_context->globals;Frame& f=*bound_context->frame;
    call(0x00403f44,0x005e6b10,0);
    Word version=call(0x00403f49,0x0044eec0,0);
    construct(0x00403f53,f.version,version);
    construct(0x00403f6a,f.graphics,0x0073d6f0);
    Word graphics=call(0x00403f86,0x005e5d10,pointer(&g.graphics_version_7a3cc4),{pointer(&f.concat_graphics),pointer(&f.graphics)});
    Word combined=call(0x00403f9f,0x005e5d10,graphics,{pointer(&f.concat_version),pointer(&f.version)});
    call(0x00403fb2,0x005e5c50,pointer(&g.graphics_version_7a3cc4),{combined});
    destroy(0x00403fc3,f.concat_version);destroy(0x00403fd4,f.concat_graphics);
    destroy(0x00403fe4,f.graphics);destroy(0x00403ff8,f.version);
    construct(0x00404006,f.version,0x0073d5d8);
    Word prefix=call(0x00404025,0x005e5d10,pointer(&g.caption_prefix_7a3cdc),{pointer(&f.concat_graphics),pointer(&f.version)});
    call(0x0040403e,0x005e5d10,prefix,{pointer(&f.unused_caption),pointer(&g.title_7a3ccc)});
    destroy(0x0040404f,f.concat_graphics);destroy(0x00404060,f.version);
    call(0x0040406e,0x005e5b00,pointer(&f.title),{pointer(&g.title_7a3ccc)});
    Class& wc=f.top_class;class_fields(wc,instance,pointer(bound_context->constants->top_class));
    wc.icon=call(0x0040409d,0x0073d424,0,{instance,0x65});
    wc.brush=call(0x004040ad,0x0073d01c,0,{4});
    call(0x004040c8,0x0073d41c,0,{pointer(&wc)});
    Word title=call(0x004040ef,0x005e5670,pointer(&f.title));
    Word window=call(0x004040ff,0x0073d410,0,{0x80,pointer(bound_context->constants->top_class),title,0x02ca0000,
        0x80000000,0x80000000,0x80000000,0x80000000,0,0,instance,0});
    bool accepted=window!=0;
    if(accepted) {
        Word monitor=call(0x00404111,0x0073d398,0,{window,0});
        if(monitor) {
            f.monitor={};f.monitor.size=40;
            accepted=call(0x00404138,0x0073d3a4,0,{monitor,pointer(&f.monitor)})!=0;



            if(accepted)accepted=f.monitor.flags==1;

        }
    }
    if(!accepted) {
        destroy(0x00404158,f.title);destroy(0x0040416c,f.unused_caption);return 0;
    }
    // In this slice the owned render creator is source-composed directly.
    // Its undefined/incidental return is ignored in the original caller too.
    init_render_window_004036f0(instance,show);
    g.top_window_7a39d4=window;g.instance_7a39c0=instance;
    destroy(0x004041b1,f.title);destroy(0x004041c5,f.unused_caption);
    return window;
}
void __stdcall setup_opengl_00401bb0() {
    Globals& g=*bound_context->globals;
    call(0x00401bb9,0x0073d220,0,{0,0,0,0});
    call(0x00401bcb,0x0073d390,0,{g.enable_78d454});




    call(0x00401bd4,0x0073d390,0,{g.enable_78d474});
    call(0x00401bdd,0x0073d390,0,{g.enable_78d45c});

    call(0x00401be5,0x0073d390,0,{g.enable_78d458});
    call(0x00401bee,0x0073d390,0,{g.enable_78d448});
    Word destination=bound_context->constants->blend_destination_73f274;
    Word source=bound_context->constants->blend_source_73f270;
    call(0x00401bfd,0x0073d38c,0,{source,destination});
    call(0x00401c0a,0x0073d390,0,{g.enable_78d44c});
}
}
