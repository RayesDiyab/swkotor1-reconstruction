#include "c76910f86e-winmain.h"
namespace kotor_renderer_gl_cells { extern std::uint32_t width_0078d1d4; }   // W-5 single owner (gl-renderer-gl-cells, D45; cold 800 = original .data 0078D1D4)
// lane-m48-fade REPLACEMENT COPY of lane-m48-b winmain/winmain.cpp (itself the replacement of the M47 stage unit winmain,
// isolated-inputs/r12-mode1-router-ini-r3/winmain/winmain.cpp). D86 (f): the WinMain view no longer owns the cells
// 007A3C50 / 007A3A40 / 007A3A4C; it references the single link cells defined by lane-m48-fade
// (src/winmain_console_cells.cpp). Changes vs lane-m48-b (everything else byte-identical):
//   00404687  cmp [007A3C50],ebx       g.headlight_7a3c50  -> winmain_headlight_007a3c50
//   004047C2  cmp [007A3A40],ebx       g.debug_7a3a40      -> winmain_console_open_007a3a40
//   00404854  cmp [007A3A40],ebx       g.debug_7a3a40      -> winmain_console_open_007a3a40
//   0040482B  mov byte [007A3A4C],bl   g.text_7a3a4c[0]=0  -> winmain_console_text_007a3a4c[0]=0
// The routed AurPostString/strlen calls keep passing the original address word 0x007a3a4c (unchanged route contract;
// the route table does not admit it, and both sites are gated by 007A3A40, written only behind EnableCheats - D86 b).
// winmain.h is unchanged: Globals::headlight_7a3c50/debug_7a3a40/text_7a3a4c are left unused (no second storage is
// read or written), same precedent as lane-m48-b's callback_78e39c.
extern "C" {
// Declarations identical to lane-m48-fade include/winmain_console_cells.h (owner: src/winmain_console_cells.cpp).
extern volatile std::uint32_t winmain_headlight_007a3c50;     // 007A3C50
extern volatile std::uint32_t winmain_console_open_007a3a40;  // 007A3A40
extern char winmain_console_text_007a3a4c[0x200];             // 007A3A4C
}
// lane-m48-b REPLACEMENT COPY of the M47 stage unit winmain (isolated-inputs/r12-mode1-router-ini-r3/winmain/winmain.cpp).
// Only change: the original store at 00404418 (C7 05 9C E3 78 00 10 15 40 00: mov dword ptr [0078E39C],00401510h, between
// the 00401700 call at 00404407 and the 005E5A90 ctor at 00404422) now writes the message-pump function 00401510 into the
// single typed cell 0078E39C owned by lane-m48-a (source/winmain_callback_cell.cpp: constant-initialised to 004015A0,
// read by call [0078E39C] at 004504E5 / 00457C15). The former integer field Globals::callback_78e39c was a second,
// unread storage for the same original word; it is no longer written (winmain.h unchanged, field left unused).
namespace kotor_winmain {
using Callback0078e39c = void (__cdecl*)();
extern Callback0078e39c volatile callback_0078e39c;              // lane-m48-a owner (?callback_0078e39c@kotor_winmain@@3R6AXXZA)
extern "C" void __cdecl winmain_message_pump_00401510();        // lane-m48-a body of 00401510 (_winmain_message_pump_00401510)
}
// DEVIATION FROM ORIGINAL: see LIST_OF_CHANGES_FROM_ORIGINAL.md#frame-rate-cap
// The original WinMain frame limiter (004048FD..00404A05: spin on CExoTimers::GetHighResolutionTimer 005E9260 until
// (now - [007A3C68]) * 0.001 >= 1000.0 / [007A3C64]) is dormant because its only writer (CSWSModule::LoadModuleFinish
// 004C594B, guarded by the never-written [00832904]) never runs, so [007A3C64] stays 0.0 and the game is uncapped.
// Uncapped frame rates trigger the original's "cannot move with WASD after combat" bug. This reconstruction arms the
// engine's own limiter once before the main loop with the swkotor.ini value
//   [Reconstruction Options] Frame Rate Cap=<fps>   (absent -> 60; 0 or negative -> original behavior, limiter stays off)
// The limiter body itself (frame_iteration below) is unchanged and faithful.
extern "C" __declspec(dllimport) unsigned int __stdcall GetPrivateProfileIntA(const char*,const char*,int,const char*);

namespace kotor_winmain {
static const int frame_rate_cap_default=60;
static float deviation_frame_rate_cap(float original_cap) {
    const int fps=static_cast<int>(GetPrivateProfileIntA("Reconstruction Options","Frame Rate Cap",frame_rate_cap_default,".\\swkotor.ini"));
    return fps>0?static_cast<float>(fps):original_cap;
}
}
namespace kotor_winmain {
Context* bound_context=nullptr;
static Word pointer(const void* value){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(value));}
static Word call(Word site,Word target,Word self,std::initializer_list<Word> args={}) {
    Call c{};c.site=site;c.target=target;c.self=self;c.count=static_cast<Word>(args.size());
    Word n=0;for(Word value:args)c.args[n++]=value;
    return static_cast<Word>(bound_context->dependencies.invoke(bound_context->dependencies.context,c));
}
static Word read(Word address){return bound_context->dependencies.read_word(bound_context->dependencies.context,address);}
static Word client(){return read(bound_context->globals->manager_7a39fc+4);}
static Word timers(){return read(bound_context->globals->base_7a39e0+4);}
static Tick clock(Word site){Call c{};c.site=site;c.target=0x005e9260;c.self=timers();return bound_context->dependencies.invoke(bound_context->dependencies.context,c);}
static void ctor(Word site,String& s,Word text){call(site,0x005e5a90,pointer(&s),{text});}
static void dtor(Word site,String& s){call(site,0x005e5c20,pointer(&s));}
static const Word scale_bits=0x3a83126f;
float delta_milliseconds(Tick delta) {
    Tick magnitude=delta&0x7fffffffffffffffULL,sign=delta&0x8000000000000000ULL;float result;
    __asm {
        fild qword ptr magnitude
        fild qword ptr sign
        fchs
        faddp st(1),st(0)
        fmul dword ptr scale_bits
        fstp dword ptr result
    }
    return result;
}
bool delta_less_than(Tick delta,float target) {
    Tick magnitude=delta&0x7fffffffffffffffULL,sign=delta&0x8000000000000000ULL;Word result;
    __asm {
        fild qword ptr magnitude
        fild qword ptr sign
        fchs
        faddp st(1),st(0)
        fmul dword ptr scale_bits
        fcomp dword ptr target
        fnstsw ax
        test ah,5
        setnp al
        movzx eax,al
        mov result,eax
    }
    return result!=0;
}
static bool positive(float value) {
    Word result;static const float zero=0.0f;
    __asm {
        fld dword ptr value
        fcomp dword ptr zero
        fnstsw ax
        test ah,41h
        setz al
        movzx eax,al
        mov result,eax
    }
    return result!=0;
}
static float frame_target(float cap) {
    static const float thousand=1000.0f;float result;
    __asm {fld dword ptr thousand
        fdiv dword ptr cap
        fstp dword ptr result
    }
    return result;
}
static Word frame_word(float bias) {
    static const float ceiling=262144.0f;
    Call c{};c.site=0x0040457c;c.target=0x006fae8c;c.has_x87_operand=true;
    unsigned char* operand=c.x87_operand;
    __asm {mov eax,operand
        fld dword ptr ceiling
        fsub dword ptr bias
        fstp tbyte ptr [eax]
    }
    return static_cast<Word>(bound_context->dependencies.invoke(bound_context->dependencies.context,c));
}
// Existing WinMain iteration factored once; false is the original GetMessage-zero exit.
static bool frame_iteration(Globals& g, Frame& f) {
        g.frame_word_7a3c5c=frame_word(g.bias_7b9274);
        if(call(0x00404592,0x0073d3f4,0,{pointer(&f.message),0,0,0,0})) {
            if(!call(0x004045a7,0x0073d3b4,0,{pointer(&f.message),0,0,0}))return false;
            call(0x004045ba,0x0073d3a0,0,{pointer(&f.message)});call(0x004045c5,0x0073d3b8,0,{pointer(&f.message)});
        } else {
            if(g.priority_changed_7a3ca4) {
                g.priority_changed_7a3ca4=0;
                if(g.priority_mode_7a3ca0==2) {Word process=static_cast<Word>(call(0x004045f2,0x0073d0bc,0));call(0x004045f5,0x0073d110,0,{process,0x100});}
                Word priority=g.priority_mode_7a3ca0==1?0x80:0x20;
                Word process=static_cast<Word>(call(0x00404609,0x0073d0bc,0));call(0x0040460c,0x0073d110,0,{process,priority});
            }
            if(g.reinit_7a3a2c) {
                Word width,height,depth;
                if(read(read(client()+4)+0x4bc)) {width=640;height=480;depth=32;}
                else {width=kotor_renderer_gl_cells::width_0078d1d4;height=g.height_78d1d8;depth=g.depth_78d1e0;} // 0040465A [0078D1D4]: W-5 single owner
                call(0x00404663,0x00403800,0,{width,height,depth,g.reinit_7a3a2c==2?1u:0u,1});
            }
            if(!call(0x00404673,0x005edb40,client()))call(0x00404681,0x0073d21c,0,{0x4500});
            call(winmain_headlight_007a3c50?0x0040468f:0x00404696,winmain_headlight_007a3c50?0x0044da60:0x0044da80,0);
            g.start_7a39c8=clock(0x004046a4);
            Word result=static_cast<Word>(call(0x004046bd,0x005ed7d0,client()));g.quit_7a39b8|=result;
            g.end_7a39b0=clock(0x004046d8);g.client_ms_7a3c7c=delta_milliseconds(g.end_7a39b0-g.start_7a39c8);
            g.start_7a39c8=clock(0x00404736);
            Word server=read(g.manager_7a39fc+8);
            if(server) {
                result=static_cast<Word>(call(0x00404757,0x004ae860,server));g.quit_7a39b8|=result;
                // Original one-iteration loop also performs this state read.
                (void)read(read(g.manager_7a39fc+0x14));
            }
            g.end_7a39b0=clock(0x00404785);g.server_ms_7a3c80=delta_milliseconds(g.end_7a39b0-g.start_7a39c8);
            if(winmain_console_open_007a3a40) {call(0x004047ee,0x0044d490,0,{0x0073d640,0,0,0x40a00000});call(0x00404800,0x0044d490,0,{0x007a3a4c,2,0,0x40a00000});}
            if(g.deferred_7a3a48) {
                Word text=static_cast<Word>(call(0x00404815,0x0044c1f0,0,{0x007a3a4c}));
                call(0x00404823,0x0044d490,0,{text,0,1,0x40a00000});winmain_console_text_007a3a4c[0]=0;g.position_7a3a44=0;g.deferred_7a3a48=0;
            }
            if(!call(0x00404846,0x005edb40,client()))call(0x0040484f,0x0044d6a0,0);
            if(winmain_console_open_007a3a40) {call(0x00404870,0x0073d224,0,{0x3f333333,0x3f333333,0x3e99999a,0x3f800000});call(0x00404886,0x0044ce50,0,{0x0073d704,g.position_7a3a44+2,0});}
            if(!call(0x00404896,0x005edb40,client())&&!g.reinit_7a3a2c) {
                g.swapping_7a3c9c=1;Word dc=static_cast<Word>(call(0x004048b8,0x0073d408,0,{g.hwnd_7a39d8}));
                call(0x004048c1,0x0073d00c,0,{dc});call(0x004048cf,0x0073d404,0,{g.hwnd_7a39d8,dc});g.swapping_7a3c9c=0;
            }
            call(0x004048e3,0x005ee1f0,client());
            if(g.sleep_enabled_7a3c58)call(0x004048f7,0x0073d1e4,0,{g.sleep_ms_78d1e8});
            if(positive(g.cap_7a3c64)) {
                float target=frame_target(g.cap_7a3c64);g.cap_end_7a3c70=clock(0x0040492d);
                if(delta_less_than(g.cap_end_7a3c70-g.cap_start_7a3c68,target)) {
                    do {g.cap_end_7a3c70=clock(0x00404998);}while(delta_less_than(g.cap_end_7a3c70-g.cap_start_7a3c68,target));
                }
                g.cap_start_7a3c68=clock(0x004049fb);
            }
        }
    return true;
}
std::int32_t __stdcall winmain_004041f0(Word instance,Word previous,char* command,std::int32_t show) {
    (void)previous;(void)command;Globals& g=*bound_context->globals;Frame& f=*bound_context->frame;
    Security security{12,0,0};
    Word mutex=static_cast<Word>(call(0x00404222,0x0073d050,0,{pointer(&security),0,0x0073d75c}));
    if(!mutex)return -1;
    Word wait=static_cast<Word>(call(0x00404234,0x0073d05c,0,{mutex,0}));



    if(wait!=0) {

        call(0x0040423f,0x0073d200,0,{mutex});return -1;
    }
    call(0x00404252,0x0073d598,0,{0});
    call(0x00404264,0x0073d3f4,0,{pointer(&f.message),0,0,0,0});
    call(0x0040426f,0x0044c980,0,{0x0073d750});
    Word allocation=static_cast<Word>(call(0x00404276,0x006fa7e6,0,{24}));
    g.base_7a39e0=allocation?static_cast<Word>(call(0x00404284,0x005e6500,allocation)):0;
    ctor(0x0040429b,f.s14,0x0073d744);
    call(0x004042ab,0x005e6680,g.base_7a39e0,{pointer(&f.s14)});
    dtor(0x004042b4,f.s14);
    allocation=static_cast<Word>(call(0x004042bb,0x006fa7e6,0,{32}));
    g.manager_7a39fc=allocation?static_cast<Word>(call(0x004042c9,0x00401160,allocation)):0;
    if(!call(0x004042df,0x00403f20,0,{instance,static_cast<Word>(show)}))return 0;
    call(0x004042ef,0x004015b0,0);call(0x004042f4,0x00401610,0);
    call(0x004042fd,0x005e6750,pointer(&f.ini));
    call(0x00404306,0x005b3190,pointer(&f.value));
    ctor(0x00404314,f.s30,0x0073d734);ctor(0x00404322,f.s28,0x0073d724);ctor(0x00404330,f.s14,0x0073d648);
    Word found=static_cast<Word>(call(0x0040434d,0x005e67c0,pointer(&f.ini),{pointer(&f.value),pointer(&f.s14),pointer(&f.s28),pointer(&f.s30)}));
    dtor(0x00404358,f.s14);dtor(0x00404361,f.s28);dtor(0x0040436a,f.s30);
    if(found)g.disable_sound_832894=call(0x00404377,0x005e5630,pointer(&f.value))?1:0;
    else {
        call(0x00404390,0x005e5680,0,{pointer(&f.value),0x0073d720,0});
        ctor(0x004043a1,f.s14,0x0073d734);ctor(0x004043af,f.s28,0x0073d724);ctor(0x004043bd,f.s30,0x0073d648);
        call(0x004043da,0x005e67d0,pointer(&f.ini),{pointer(&f.value),pointer(&f.s30),pointer(&f.s28),pointer(&f.s14)});
        dtor(0x004043e3,f.s30);dtor(0x004043ec,f.s28);dtor(0x004043f5,f.s14);
    }
    call(0x00404407,0x00401700,0,{g.argc_833bfc,g.argv_833c00});
    callback_0078e39c=&winmain_message_pump_00401510;            // 00404418 mov dword ptr [0078E39C],00401510h
    ctor(0x00404422,f.s14,0x0073d71c);ctor(0x00404430,f.s28,0x0073d71c);ctor(0x0040443e,f.s30,0x0073d71c);
    Word service=static_cast<Word>(call(0x0040445d,0x005ed860,client(),{0,pointer(&f.s30),pointer(&f.s28),pointer(&f.s14),0}));
    dtor(0x00404468,f.s30);dtor(0x00404471,f.s28);dtor(0x0040447a,f.s14);
    if(service) {
        call(0x00404484,0x0073d200,0,{mutex});dtor(0x0040448e,f.value);call(0x00404497,0x005e67e0,pointer(&f.ini));return 0;
    }
    call(0x004044b0,0x005ed7b0,client());g.quit_7a39b8=0;
    call(0x004044c7,0x005eda10,client(),{1,0});
    call(0x004044d1,0x0044c980,0,{0x0073d710});
    call(0x004044da,0x0044ef00,0,{0,0,0,0});
    call(0x004044ed,0x005edfd0,client(),{0,10});call(0x004044ff,0x005edfd0,client(),{1,20});
    call(0x00404511,0x005edfd0,client(),{2,23});call(0x00404522,0x005edfd0,client(),{3,23});call(0x00404534,0x005edfd0,client(),{4,23});
    call(0x00404542,0x005ed300,client());call(0x00404551,0x005ed420,client(),{1});
    // DEVIATION FROM ORIGINAL: see LIST_OF_CHANGES_FROM_ORIGINAL.md#frame-rate-cap (arms the dormant 007A3C64 limiter)
    g.cap_7a3c64=deviation_frame_rate_cap(g.cap_7a3c64);
    while(!g.quit_7a39b8) {
        if(!frame_iteration(g,f))break;
    }
    call(0x00404a21,0x00401280,g.manager_7a39fc);call(0x00404a2f,0x005ed810,client());call(0x00404a3c,0x005ed880,client());
    if(g.mouse_trails_7a3ca8>=1)call(0x00404a56,0x0073d3f8,0,{0x5d,static_cast<Word>(g.mouse_trails_7a3ca8),0,0});
    if(g.screen_saver_7a3cac>=1)call(0x00404a67,0x0073d3f8,0,{0x11,static_cast<Word>(g.screen_saver_7a3cac),0,0});
    Word object=g.manager_7a39fc;
    if(object) {call(0x00404a75,0x004012c0,object);call(0x00404a7b,0x006fa390,0,{object});}
    object=g.base_7a39e0;g.manager_7a39fc=0;
    if(object) {call(0x00404a95,0x005e66a0,object);call(0x00404a9b,0x006fa390,0,{object});}
    g.base_7a39e0=0;call(0x00404aa9,0x0073d59c,0);call(0x00404ab0,0x0073d200,0,{mutex});
    Word result=f.message.wparam;dtor(0x00404abe,f.value);call(0x00404ac7,0x005e67e0,pointer(&f.ini));
    return static_cast<std::int32_t>(result);
}


}
