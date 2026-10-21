struct NativeVmScriptTrace:recovered_vm_script_trace::Ports{
 NativeMemory&m;explicit NativeVmScriptTrace(NativeMemory&v):m(v){}
 Word read(Word a)override{return m.read(a);}Word ai(Word a)override{NativeModuleComplete p(m);return p.ai(a);}Word not_empty(Word a)override{return static_cast<Word>(client_movies::StringNotEqual_005e5390(static_cast<options_video::String*>(ptr(a)),nullptr,""));}
 void comma(Word a)override{frame_LogStringCtor_005e5a90(ptr(a),nullptr,",");}
 Word concat(Word a,Word b,Word c)override{return word(GuiConcat005e5d10(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b)),static_cast<const kotor_reconstruction::StartupString*>(ptr(c))));}
 void assign(Word a,Word b)override{(void)async_assign_string_005e5c50(ptr(a),nullptr,ptr(b));}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
};
extern "C" void __fastcall recovered_vm_script_trace_0052c100(void*,void*,void*script){NativeMemory m;NativeVmScriptTrace p(m);alignas(4)unsigned char scratch[24];recovered_vm_script_trace::command(p,word(script),word(scratch));}
