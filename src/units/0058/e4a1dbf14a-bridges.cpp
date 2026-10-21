// Native window integration adapters.
#include "e818ecbcdd-bridges.h"
#include <cstring>
namespace {
using Word=std::uint32_t;
[[noreturn]] void invalid_binding(const char*,Word=0,Word=0){ExitProcess(86);}
Word word(const void* p){return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p));}
const kotor_menu_window::Publication& publication(){
    const auto* p=kotor_menu_window_publication;
    if(!p)invalid_binding("bridge.publication.null");
    if(!p->winmain)invalid_binding("bridge.publication.winmain_null");
    if(!p->shell)invalid_binding("bridge.publication.shell_null");
    if(!p->enter_reinit)invalid_binding("bridge.publication.enter_reinit_null");
    if(!p->leave_reinit)invalid_binding("bridge.publication.leave_reinit_null");
    if(!p->enter_message)invalid_binding("bridge.publication.enter_message_null");
    if(!p->leave_message)invalid_binding("bridge.publication.leave_message_null");
    return *p;
}
template<class Context> struct BoundScope {
    Context*& slot;Context* prior;Context* current;void (__cdecl* leave)(Context*) noexcept;
    BoundScope(Context*& s,Context* c,void (__cdecl* done)(Context*) noexcept):slot(s),prior(s),current(c),leave(done){slot=c;}
    ~BoundScope(){slot=prior;leave(current);}
    BoundScope(const BoundScope&)=delete;BoundScope& operator=(const BoundScope&)=delete;
};
void reinit(Word width,Word height,Word depth,Word fullscreen,Word check_movies){
    const auto& p=publication();auto* c=p.enter_reinit();
    if(!c)invalid_binding("bridge.reinit.context_null");
    if(!c->globals)invalid_binding("bridge.reinit.globals_null");
    if(!c->constants)invalid_binding("bridge.reinit.constants_null");
    if(!c->frame)invalid_binding("bridge.reinit.frame_null");
    if(!c->dependencies.invoke)invalid_binding("bridge.reinit.invoke_null");
    if(!c->dependencies.read_word)invalid_binding("bridge.reinit.read_word_null");
    BoundScope<kotor_reinit::Context> scope(kotor_reinit::bound_context,c,p.leave_reinit);
    kotor_reinit::reinit_00403800(width,height,depth,fullscreen,check_movies);
}
template<class Call> void signature(const Call& c,Word count){if(c.self||c.count!=count)invalid_binding("bridge.signature.self_or_count",c.site,c.target);}
}
extern "C" void __cdecl kotor_menu_bind_winmain_context(){
    const auto& p=publication();
    if(!p.winmain->globals)invalid_binding("bridge.bind_winmain.winmain_globals_null");
    if(!p.winmain->frame)invalid_binding("bridge.bind_winmain.winmain_frame_null");
    if(!p.winmain->dependencies.invoke)invalid_binding("bridge.bind_winmain.winmain_invoke_null");
    if(!p.winmain->dependencies.read_word)invalid_binding("bridge.bind_winmain.winmain_read_word_null");
    if(!p.shell->globals)invalid_binding("bridge.bind_winmain.shell_globals_null");
    if(!p.shell->constants)invalid_binding("bridge.bind_winmain.shell_constants_null");
    if(!p.shell->frame)invalid_binding("bridge.bind_winmain.shell_frame_null");
    if(!p.shell->dependencies.invoke)invalid_binding("bridge.bind_winmain.shell_invoke_null");
    // Binding must happen before the first registration/creation callback.
    kotor_winmain::bound_context=p.winmain;kotor_window_shell::bound_context=p.shell;
}
extern "C" client_services_start::Word __cdecl kotor_menu_service_00403800(const client_services_start::Call* c){
    using Abi=client_services_start::Abi;
    if(!c)invalid_binding("bridge.service_00403800.call_null");
    if(c->site!=0x005f92d9||c->target!=0x00403800||c->abi!=Abi::Cdecl)invalid_binding("bridge.service_00403800.key",c->site,c->target);
    signature(*c,5);reinit(c->args[0],c->args[1],c->args[2],c->args[3],c->args[4]);
    return 0; // Transport word only: the source caller ignores this void result.
}
extern "C" LRESULT CALLBACK kotor_menu_source_wndproc(HWND hwnd,UINT msg,WPARAM wp,LPARAM lp){
    const auto& p=publication();auto* c=p.enter_message(word(hwnd),msg,static_cast<Word>(wp),static_cast<Word>(lp));
    if(!c)invalid_binding("bridge.wndproc.context_null",msg);
    if(!c->globals)invalid_binding("bridge.wndproc.globals_null",msg);
    if(!c->frame)invalid_binding("bridge.wndproc.frame_null",msg);
    if(!c->dependencies.invoke)invalid_binding("bridge.wndproc.invoke_null",msg);
    if(!c->dependencies.read_word)invalid_binding("bridge.wndproc.read_word_null",msg);
    if(!c->dependencies.write_word)invalid_binding("bridge.wndproc.write_word_null",msg);
    BoundScope<kotor_message::Context> scope(kotor_message::bound_context,c,p.leave_message);
    const Word result=kotor_message::message_00402800(word(hwnd),msg,static_cast<Word>(wp),static_cast<Word>(lp));
    return static_cast<LRESULT>(result); // Preserve all EAX bits, including sign.
}
namespace kotor_menu_window {
bool __cdecl winmain_window_route(const kotor_winmain::Call& c,kotor_winmain::Tick* result){
    if(c.target!=0x00403f20&&c.target!=0x00403800)return false;
    if(!result)invalid_binding("bridge.winmain_route.result_null",c.site,c.target);
    if(c.target==0x00403f20){if(c.site!=0x004042df)invalid_binding("bridge.winmain_route.site_00403f20",c.site,c.target);signature(c,2);*result=kotor_window_shell::init_game_app_00403f20(c.args[0],static_cast<std::int32_t>(c.args[1]));}
    else{if(c.site!=0x00404663)invalid_binding("bridge.winmain_route.site_00403800",c.site,c.target);signature(c,5);reinit(c.args[0],c.args[1],c.args[2],c.args[3],c.args[4]);*result=0;}
    return true;
}
bool __cdecl shell_import(const kotor_window_shell::Call& c,kotor_window_shell::Result* result){
    Word count=0;
    switch(c.target){case 0x73d424:count=2;break;case 0x73d01c:case 0x73d41c:count=1;break;case 0x73d410:count=12;break;case 0x73d398:case 0x73d3a4:count=2;break;default:return false;}
    if(!result)invalid_binding("bridge.shell_import.result_null",c.site,c.target);signature(c,count);
    switch(c.target){
    case 0x73d424:*result=word(LoadIconA(reinterpret_cast<HINSTANCE>(c.args[0]),reinterpret_cast<LPCSTR>(c.args[1])));break;
    case 0x73d01c:*result=word(GetStockObject(static_cast<int>(c.args[0])));break;
    case 0x73d41c:{
        const auto* input=reinterpret_cast<const kotor_window_shell::Class*>(c.args[0]);
        if(!input)invalid_binding("bridge.shell_import.class_null",c.site);
        if(input->procedure!=0x00402800)invalid_binding("bridge.shell_import.wndproc_not_00402800",c.site,input->procedure);
        WNDCLASSA wc;std::memcpy(&wc,input,sizeof(wc));wc.lpfnWndProc=&kotor_menu_source_wndproc;
        *result=RegisterClassA(&wc);break;}
    case 0x73d410:*result=word(::CreateWindowExA(c.args[0],reinterpret_cast<LPCSTR>(c.args[1]),reinterpret_cast<LPCSTR>(c.args[2]),c.args[3],
        static_cast<int>(c.args[4]),static_cast<int>(c.args[5]),static_cast<int>(c.args[6]),static_cast<int>(c.args[7]),
        reinterpret_cast<HWND>(c.args[8]),reinterpret_cast<HMENU>(c.args[9]),reinterpret_cast<HINSTANCE>(c.args[10]),reinterpret_cast<void*>(c.args[11])));break;
    case 0x73d398:*result=word(MonitorFromWindow(reinterpret_cast<HWND>(c.args[0]),c.args[1]));break;
    case 0x73d3a4:*result=static_cast<Word>(GetMonitorInfoA(reinterpret_cast<HMONITOR>(c.args[0]),reinterpret_cast<MONITORINFO*>(c.args[1])));break;
    }
    return true;
}
bool __cdecl message_import(const kotor_message::Call& c,kotor_message::Result* result){
    if(c.kind!=kotor_message::Kind::import_slot)return false;
    Word count=0;
    switch(c.target){case 0x73d3b0:case 0x73d434:case 0x73d428:count=4;break;case 0x73d400:case 0x73d42c:count=2;break;case 0x73d430:count=1;break;case 0x73d0f8:count=0;break;default:return false;}
    if(!result)invalid_binding("bridge.message_import.result_null",c.site,c.target);signature(c,count);
    switch(c.target){
    case 0x73d3b0:*result=static_cast<Word>(DefWindowProcA(reinterpret_cast<HWND>(c.args[0]),c.args[1],c.args[2],static_cast<LPARAM>(c.args[3])));break;
    case 0x73d434:*result=static_cast<Word>(PostMessageA(reinterpret_cast<HWND>(c.args[0]),c.args[1],c.args[2],static_cast<LPARAM>(c.args[3])));break;
    case 0x73d428:*result=static_cast<Word>(SendMessageA(reinterpret_cast<HWND>(c.args[0]),c.args[1],c.args[2],static_cast<LPARAM>(c.args[3])));break;
    case 0x73d400:*result=static_cast<Word>(::ShowWindow(reinterpret_cast<HWND>(c.args[0]), static_cast<int>(c.args[1])));break;
    case 0x73d0f8:*result=GetTickCount();break;
    case 0x73d430:*result=word(::SetCapture(reinterpret_cast<HWND>(c.args[0])));break;
    case 0x73d42c:*result=static_cast<Word>(ScreenToClient(reinterpret_cast<HWND>(c.args[0]),reinterpret_cast<POINT*>(c.args[1])));break;
    }
    return true;
}
}
static_assert(sizeof(void*)==4&&sizeof(LRESULT)==4&&sizeof(MSG)==28&&sizeof(WNDCLASSA)==40&&sizeof(MONITORINFO)==40,"native x86 protocol");
static_assert(sizeof(kotor_message::Frame)==48&&offsetof(kotor_message::Frame,message)==20,"per-callback source frame");
