// 0886-gates (Claude): private copy; sysadmin send / option lookup bound (claude_0886_bodies_server).
extern "C" {
unsigned __fastcall kotor_fastcall_00675690(void*,void*,void*);          // 00675690 CClientMsgSysAdmin::SendSysAdminToServerMessage RET 4
unsigned __fastcall kotor_fastcall_0054e9d0(void*,void*,void*,void*);    // 0054E9D0 CServerInfo::FindOptionIndex RET 8
}
extern "C" {unsigned __fastcall recovered_client_token_006754d0(void*,void*,const char*,unsigned,void*,void*,void*);void* __fastcall recovered_client_serverinfo_005ed710(void*,void*);}
struct NativeClientSystem:ClientSystemPorts{NativeMemory&m;NativeServerInfo text;explicit NativeClientSystem(NativeMemory&v):m(v),text(v){}
const char* literal(Word a){switch(a){case 0x752bf8:return ":: Server mode: Idle.\n";case 0x752bd8:return ":: Server mode: Module Loaded.\n";case 0x752bb4:return ":: Server mode: Module Running.\n";case 0x752b9c:return ":: Server module list: ";case 0x752b8c:return "GetBannedList";case 0x752b64:return ":: Server character vault list: %s.\n";case 0x752b44:return ":: Server player list ::\n%s\n";case 0x752b24:return ":: Module savegame list: %s.\n";case 0x752b14:return ":: %s.%s %s\n";case 0x73d5cc:return "\n";default:return static_cast<const char*>(ptr(a));}}
void empty(Word a)override{text.empty(a);}void destroy(Word a)override{text.destroy(a);}Word counted(Word a,Word b,Word c)override{return word(resman_string_counted_005e5b70(ptr(a),nullptr,static_cast<const char*>(ptr(b)),c));}void assign(Word a,Word b)override{text.assign(a,b);}
Word parse(Word a,Word b,Word c,Word d,Word e,Word f)override{return recovered_client_token_006754d0(ptr(a),nullptr,static_cast<const char*>(ptr(b)),c,ptr(d),ptr(e),ptr(f));}
Word equal(Word a,Word b)override{return static_cast<kotor_reconstruction::StartupString*>(ptr(a))->BorrowedEquals005e5290(literal(b));}
Word info(Word a)override{return word(recovered_client_serverinfo_005ed710(ptr(a),nullptr));}void module_name(Word a,Word b)override{NativeModuleHelpers p(m);module_identifier_0054eaa0(m,p,a,b);}Word cstr(Word a)override{return text.cstr(a);}
void log0(Word a)override{std::printf("%s",literal(a));}void log1(Word a,Word b)override{std::printf(literal(a),literal(b));}void log3(Word a,Word b,Word c,Word d)override{std::printf(literal(a),literal(b),literal(c),literal(d));}
void format2(Word a,Word fmt,Word b,Word c)override{if(fmt!=0x7493b4)required(fmt);kotor_reconstruction::StartupFormat005e5680(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),"%s.%s",literal(b),literal(c));}
void send(Word a,Word b)override{(void)kotor_fastcall_00675690(ptr(a),nullptr,ptr(b));}Word option(Word a,Word b,Word c)override{return kotor_fastcall_0054e9d0(ptr(a),nullptr,ptr(b),ptr(c));}Word integer(Word a)override{return static_cast<Word>(std::atol(literal(a)));}
Word character(Word a,Word b)override{using S=kotor_reconstruction::StartupString;return directory_list::At005e5600(static_cast<S*>(ptr(a)),nullptr,static_cast<int>(b));}
Word left(Word a,Word b,Word c)override{return word(base_ini_reconstruction::left_005e5f70(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,static_cast<kotor_reconstruction::StartupString*>(ptr(b)),static_cast<int>(c)));}Word length(Word a)override{return async_string_length_005e5790(ptr(a),nullptr);}
};
