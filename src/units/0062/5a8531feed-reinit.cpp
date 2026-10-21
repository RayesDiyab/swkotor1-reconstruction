#include "287311afba-reinit.h"
#include <initializer_list>
namespace kotor_reinit {
Context* bound_context=nullptr;
static Word address(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
static Word call(Word site,Word target,Word self,std::initializer_list<Word> args,Kind kind=Kind::direct,Word slot=0) {
    Call request{};request.site=site;request.target=target;request.self=self;request.count=static_cast<Word>(args.size());request.kind=kind;request.slot=slot;
    Word n=0;for(Word value:args)request.args[n++]=value;
    return static_cast<Word>(bound_context->dependencies.invoke(bound_context->dependencies.context,request));
}
static Word read(Word p){return bound_context->dependencies.read_word(bound_context->dependencies.context,p);}
static Word client(){return read(bound_context->globals->manager_7a39fc+4);}
static Word virt(Word site,Word self,Word slot,std::initializer_list<Word> args) {return call(site,read(read(self)+slot),self,args,Kind::virtual_slot,slot);}
static Word api(Word site,Word slot,std::initializer_list<Word> args){return call(site,slot,0,args,Kind::import_slot);}
static void tick(Word site){api(site,0x0073d0f8,{});}
void __cdecl reinit_00403800(Word width,Word height,Word depth,Word fullscreen,Word check_movies) {
    auto& g=*bound_context->globals;auto& f=*bound_context->frame;auto& k=*bound_context->constants;
    f.mouse_mode=(f.mouse_mode&0xffffff00u)|0xffu;
    if(!g.aurora_7a39f8)return;
    call(0x40383f,0x5ee170,client(),{0x40});
    f.backdrop=0;
    if(virt(0x403850,g.aurora_7a39f8,4,{})==1 && fullscreen==1) {
        f.backdrop=1;
        f.event=api(0x403869,0x73d1ec,{0,0,0,0});
        f.thread=api(0x403885,0x73d1f0,{0,0x8000,k.thread_procedure,f.event,0,address(&f.thread_id)});
        api(0x403892,0x73d1f4,{f.thread,15});tick(0x403898);
        Word window=api(0x4038a4,0x73d3a8,{k.thread_class,0});
        if(window){api(0x4038b2,0x73d3ac,{window,0});api(0x4038ba,0x73d400,{window,0});}
    }



    f.nested_or_exit=g.guard_7a3a3c==1?1:0;

    if(!f.nested_or_exit)g.guard_7a3a3c=1;
    for(Word& component:f.clear_color)component=0;
    api(0x403902,0x73d218,{0xc22,address(f.clear_color)});
    Word old_render=g.render_7a39d8;
    virt(0x403923,g.aurora_7a39f8,0x20,{0,f.backdrop==0?1u:0u});
    g.render_7a39d8=0;
    call(0x403930,0x5e6750,address(&f.ini),{});
    call(0x40393d,0x5b3190,address(&f.value),{});
    call(0x403950,0x5e5a90,address(&f.string_48),{k.disable_vbo});
    call(0x403963,0x5e5a90,address(&f.string_40),{k.section});
    call(0x403977,0x5e5a90,address(&f.string_30),{k.ini_file});
    Word found=call(0x403999,0x5e67c0,address(&f.ini),{address(&f.value),address(&f.string_30),address(&f.string_40),address(&f.string_48)});
    call(0x4039a8,0x5e5c20,address(&f.string_30),{});
    call(0x4039b6,0x5e5c20,address(&f.string_40),{});
    call(0x4039c4,0x5e5c20,address(&f.string_48),{});
    if(found && call(0x4039d1,0x5e5630,address(&f.value),{})==1)call(0x4039db,0x44ef60,0,{});
    tick(0x4039e0);
    if(g.render_7a39d8) {
        call(0x4039fc,0x5eda10,client(),{0,0});
        if(g.input_7a39e4)virt(0x403a0f,g.input_7a39e4,0,{1});
        g.input_7a39e4=0;
    }
    call(0x403a23,0x4036f0,0,{g.instance_7a39c0,1});
    f.mode=g.mode_78d1dc;
    if(fullscreen)virt(0x403a57,g.aurora_7a39f8,0xc,{address(&width),address(&height),address(&depth),address(&f.mode),1});



    virt(0x403a86,g.aurora_7a39f8,8,{g.top_7a39d4,g.render_7a39d8,width,height,depth,f.mode,fullscreen,depth&0xff,depth&0xff});
    call(0x403a92,0x5e5a90,address(&f.string_30),{k.disable_write_only});
    call(0x403aa5,0x5e5a90,address(&f.string_40),{k.section});
    call(0x403ab9,0x5e5a90,address(&f.string_48),{k.ini_file});
    found=call(0x403adb,0x5e67c0,address(&f.ini),{address(&f.value),address(&f.string_48),address(&f.string_40),address(&f.string_30)});
    call(0x403aea,0x5e5c20,address(&f.string_48),{});
    call(0x403af8,0x5e5c20,address(&f.string_40),{});
    call(0x403b06,0x5e5c20,address(&f.string_30),{});
    if(found){Word option=call(0x403b15,0x5e5630,address(&f.value),{});if(option==1)call(0x403b1f,0x44ef70,0,{});else call(0x403b26,0x44ef80,0,{});}
    api(0x403b32,0x73d430,{g.render_7a39d8});
    if(old_render)api(0x403b3d,0x73d3fc,{old_render});
    tick(0x403b49);call(0x403b4b,0x401bb0,0,{});
    if(g.reinit_7a3a2c)g.reinit_7a3a2c=0;
    api(0x403b72,0x73d220,{f.clear_color[0],f.clear_color[1],f.clear_color[2],f.clear_color[3]});
    if(g.input_7a39e4) {
        Word mode=call(0x403b82,0x5df6e0,g.input_7a39e4,{});
        f.mouse_mode=(f.mouse_mode&0xffffff00u)|(mode&0xff);
        if(g.input_7a39e4)virt(0x403b99,g.input_7a39e4,0,{1});
    }
    f.allocation=call(0x403b9d,0x6fa7e6,0,{8});
    Word input=f.allocation?call(0x403bb4,0x5df3f0,f.allocation,{}):0;
    g.input_7a39e4=input;
    if((f.mouse_mode&0xff)!=0xff) {



        call(0x403bd5,0x5df5a0,input,{f.mouse_mode});

    }
    call(0x403bda,0x401d90,0,{});
    if(call(0x403be8,0x5ed7c0,client(),{}))call(0x403bf9,0x5ed870,client(),{});
    call(0x403bfe,0x401e00,0,{});tick(0x403c03);
    Word manager=g.manager_7a39fc;
    if(manager){Word current=read(manager+4);if(current && !f.nested_or_exit && check_movies==1)call(0x403c21,0x5ed980,current,{});}
    if(!f.nested_or_exit)g.guard_7a3a3c=0;
    if(f.backdrop==1) {
        Word window=api(0x403c41,0x73d3a8,{k.thread_class,0});
        if(window){api(0x403c50,0x73d3ac,{window,1});api(0x403c59,0x73d400,{window,4});}
        api(0x403c64,0x73d1f8,{f.event});tick(0x403c6a);
        do{api(0x403c8a,0x73d1fc,{f.thread,address(&f.nested_or_exit)});}while(f.nested_or_exit==0x103);
        tick(0x403c98);api(0x403ca5,0x73d200,{f.thread});api(0x403ca8,0x73d200,{f.event});tick(0x403caa);
    }
    call(0x403cb7,0x5ee180,client(),{0x40});
    call(0x403cc5,0x5e5c20,address(&f.value),{});
    call(0x403cd6,0x5e67e0,address(&f.ini),{});
}
}
