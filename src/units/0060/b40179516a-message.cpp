#include "2f53f87007-message.h"
#include <initializer_list>
namespace kotor_message {
Context* bound_context=nullptr;
static Word address(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
static Word read(Word p){return bound_context->dependencies.read_word(bound_context->dependencies.context,p);}
static void write(Word p,Word value){bound_context->dependencies.write_word(bound_context->dependencies.context,p,value);}
static Word call(Word site,Word target,Word self,std::initializer_list<Word> args={},Kind kind=Kind::direct,Word slot=0){Call c{};c.site=site;c.target=target;c.self=self;c.count=static_cast<Word>(args.size());c.kind=kind;c.slot=slot;Word n=0;for(Word value:args)c.args[n++]=value;return static_cast<Word>(bound_context->dependencies.invoke(bound_context->dependencies.context,c));}
static Word client(){return read(bound_context->globals->manager_7a39fc+4);}
static Word virt(Word site,Word self,Word slot){return call(site,read(read(self)+slot),self,{},Kind::virtual_slot,slot);}
static Word api(Word site,Word slot,std::initializer_list<Word> args={}){return call(site,slot,0,args,Kind::import_slot);}
static Word default_message(Word hwnd,Word msg,Word wp,Word lp,Word site=0x4028e9){return api(site,0x73d3b0,{hwnd,msg,wp,lp});}
static Word packed(Word height,Word position){return ((height-(position>>16)-1)&0xffff)<<16|(position&0xffff);}
static void buffer(Word site,Word wp,Word lp){auto& f=*bound_context->frame;f.message[2]=wp;f.message[3]=lp;call(site,0x5df590,bound_context->globals->input_7a39e4,{address(f.message)});}
Word __stdcall message_00402800(Word hwnd,Word msg,Word wp,Word lp){auto& g=*bound_context->globals;auto& f=*bound_context->frame;
    switch(msg){
    case 5:if(hwnd==g.render_7a39d8){if(wp==2){g.reinit_7a3a2c=2;return 0;}return default_message(hwnd,msg,wp,lp,0x40287e);}break;
    case 7:if(g.focus_78d1d0==1)call(0x4028dc,0x5eda10,client(),{1,0});break;
    case 8:if(!call(0x402895,0x5edb40,client())){call(0x4028a9,0x5eda10,client(),{0,0});g.focus_78d1d0=1;}else g.focus_78d1d0=0;break;
    case 0x10:call(0x402b51,0x5eda00,client());return 0;
    case 0x1f:if(g.flag_8338f0)g.flag_8338f0=0;break;
    case 0x20:if(wp==g.render_7a39d8 && g.input_7a39e4 && (lp&0xffff)==1){call(0x402b19,0x5df5b0,g.input_7a39e4,{0});return 1;}break;
    case 0x46:case 0x47:{Word movie=call(msg==0x46?0x402900:0x402b6a,0x5edb40,client());if(movie==1){Word block=call(msg==0x46?0x40290c:0x402b7a,0x6fa7e6,0,{28});write(block,g.movie_window_7a3cf8);write(block+4,0);
        // The two cases have different observed copy orders.
        if(msg==0x46){write(block+8,read(lp+8));write(block+12,read(lp+12));write(block+16,read(lp+16));write(block+20,read(lp+20));}
        else {write(block+16,read(lp+16));write(block+20,read(lp+20));write(block+8,read(lp+8));write(block+12,read(lp+12));}
        write(block+24,0);api(msg==0x46?0x402949:0x402bb7,0x73d434,{g.movie_window_7a3cf8,msg==0x46?0x407u:0x408u,wp,block});}break;}
    case 0x1c:
        if(!g.input_7a39e4)break;
        if(wp==0){g.movie_fullscreen_7a3a10=0;g.movie_inactive_7a3a18=0;
            Word manager=g.manager_7a39fc;
            if(manager&&read(manager+4)){
                if(g.aurora_7a39f8){if(call(0x402988,0x5edb40,client())==1){if(virt(0x40299a,g.aurora_7a39f8,4))g.movie_fullscreen_7a3a10=1;g.movie_inactive_7a3a18=1;}}
            }
            if(!g.movie_fullscreen_7a3a10&&g.aurora_7a39f8&&virt(0x4029ca,g.aurora_7a39f8,4)==1){api(0x4029db,0x73d400,{g.render_7a39d8,6});virt(0x4029e9,g.aurora_7a39f8,0x18);}
            if(hwnd==g.render_7a39d8){call(0x4029f8,0x401d90,0);g.last_tick_7a3c4c=api(0x4029fd,0x73d0f8);}
            if(!g.movie_inactive_7a3a18){call(0x402a15,0x4015f0,0);call(0x402a1a,0x401650,0);}
        }else{
            if(!g.movie_fullscreen_7a3a10&&g.aurora_7a39f8){if(virt(0x402a3f,g.aurora_7a39f8,4)){
                if(!g.reinit_7a3a2c){if(hwnd==g.render_7a39d8)virt(0x402a63,g.aurora_7a39f8,0x1c);g.reinit_7a3a2c=2;}}
                else if(!g.movie_inactive_7a3a18&&!call(0x402a83,0x5edb40,client())){Word elapsed=api(0x402a8c,0x73d0f8)-g.last_tick_7a3c4c;if(elapsed>100)call(0x402aa2,0x5ed980,client());}}
            if(hwnd==g.render_7a39d8){Word elapsed=api(0x402ab3,0x73d0f8)-g.last_tick_7a3c4c;if(elapsed<1000||!virt(0x402aca,g.aurora_7a39f8,4))call(0x402ad1,0x401e00,0);}
            if(!g.movie_inactive_7a3a18){call(0x402ae4,0x4015b0,0);call(0x402ae9,0x401610,0);}
        }break;
    case 0x104:
        if(wp==0x12){call(0x402c04,0x5eda10,client(),{0,0});return 0;}
        if(wp==0x79)return 0;
        if(wp==0xd && (lp&0x20000000))return 0;
        if(wp==0x73){if(lp&0x20000000)call(0x402c29,0x5eda10,client(),{1,0});api(0x402c37,0x73d428,{hwnd,0x10,wp,lp});}break;
    case 0x105:
        if(wp==0x79)return 0;
        if(wp==0x12){call(0x402c7f,0x5eda10,client(),{1,0});return 0;}
        if(wp==0xd){if(lp&0x20000000)call(0x402c9f,0x401e90,0,{0,0});}
        else if(lp&0x20000000)call(0x4028dc,0x5eda10,client(),{1,0});break;
    case 0x106:if(wp==0xd&&(lp&0x20000000))return 0;break;
    case 0x100:
        if(wp==0x1b&&g.manager_7a39fc&&read(g.manager_7a39fc+4)&&call(0x402ce1,0x5edb40,client())==1)call(0x402cf4,0x5ee160,client());
        if(!call(0x402d01,0x5ed480,client())&&call(0x402d15,0x4024f0,0,{hwnd,wp,lp})==1)return 0;break;
    case 0x102:if(!call(0x402d3a,0x5ed480,client())&&call(0x402d48,0x402240,0,{wp})==1)return 0;break;
    case 0x10d:case 0x10e:{bool start=msg==0x10d;if(call(start?0x402d6c:0x402da2,0x5ed4d0,client())){Word ime=call(start?0x402d82:0x402db7,0x5ed4e0,client());Word state=read(ime);write(ime,start?(state|1):(state&0xfffffffe));return 0;}break;}
    case 0x10f:case 0x282:{bool composition=msg==0x10f;if(call(composition?0x402dd7:0x4032c3,0x5ed4d0,client())){Word ime=call(composition?0x402dfc:0x4032e8,0x5ed4e0,client());call(composition?0x402e03:0x4032ef,composition?0x5dfb00:0x5dfc40,ime,{hwnd,wp,lp});return 0;}break;}
    case 0x51:case 0x281:case 0x283:case 0x284:case 0x290:if(call(0x403350,0x5ed4d0,client()))return 0;break;
    case 0x112:



        if((wp&0xfffffff0)==0xf000)return 0;

        if((wp&0xffff)==0xf140||(wp&0xffff)==0xf170)return 0;break;
    case 0x409:call(0x403334,0x401e90,0,{1,wp!=0?1u:0u});return 0;
    case 0x40a:call(0x403316,0x403de0,0);return 0;
    case 0x200:case 0x208:case 0x201:case 0x202:case 0x203:case 0x204:case 0x205:case 0x206:case 0x207:case 0x20a:{
        Word active_site=0,buffer_site=0;bool false_zero=false;
        switch(msg){case 0x200:case 0x208:active_site=0x402e52;buffer_site=0x402e8e;break;case 0x201:active_site=0x402ea6;buffer_site=0x4030e5;false_zero=true;break;case 0x202:active_site=0x402f81;buffer_site=0x4032b0;false_zero=true;break;case 0x203:active_site=0x402efe;buffer_site=0x402f69;false_zero=true;break;case 0x204:active_site=0x403078;buffer_site=0x4030e5;break;case 0x205:active_site=0x403108;buffer_site=0x403173;break;case 0x206:active_site=0x402fe9;buffer_site=0x403055;false_zero=true;break;case 0x207:active_site=0x4031ba;buffer_site=0x4031fa;break;default:active_site=0x403252;buffer_site=0x4032b0;break;}
        if(!g.input_7a39e4||!call(active_site,0x5df510,g.input_7a39e4)){if(false_zero)return 0;break;}
        if(msg!=0x200&&msg!=0x208){f.message[1]=msg;
            if(msg!=0x207&&msg!=0x20a){Word first=0,second=0,swapped=msg;
                switch(msg){case 0x201:first=0x402ec0;second=0x402ed6;swapped=0x204;break;case 0x202:first=0x402f9b;second=0x402fad;swapped=0x205;break;case 0x203:first=0x402f17;second=0x402f29;swapped=0x206;break;case 0x204:first=0x403092;second=0x4030a4;swapped=0x201;break;case 0x205:first=0x403121;second=0x403133;swapped=0x202;break;default:first=0x403003;second=0x403014;swapped=0x203;break;}
                if(call(first,0x5ed700,client())){Word options=call(second,0x5ed700,client());if(read(options+0x54))f.message[1]=swapped;}
            }
        }else f.message[1]=msg;
        Word transformed;
        if(msg==0x20a){f.point_x=lp&0xffff;

            f.message[2]=wp; // Original00403279 precedes POINT.y/ScreenToClient.

            f.point_y=lp>>16;api(0x403281,0x73d42c,{hwnd,address(&f.point_x)});transformed=((g.height_78d1d8-f.point_y-1)&0xffff)<<16|(f.point_x&0xffff);}
        else transformed=packed(g.height_78d1d8,lp);



        buffer(buffer_site,wp,transformed);
if(msg==0x201||msg==0x204)api(0x4030ef,0x73d430,{hwnd});else if(msg==0x206)api(0x40305f,0x73d430,{hwnd});else if(msg==0x207)api(0x403204,0x73d430,{hwnd});
        break;}
    default:break;
    }



    return default_message(hwnd,msg,wp,lp);

}
}
