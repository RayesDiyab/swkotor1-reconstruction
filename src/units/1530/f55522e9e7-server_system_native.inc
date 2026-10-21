extern "C" void* __fastcall kotor_fastcall_004ae920(void*,void*,unsigned,unsigned);
struct NativeServerSystem:ServerSystemPorts{NativeMemory&m;NativeServerInfo text;explicit NativeServerSystem(NativeMemory&v):m(v),text(v){}
void empty(Word a)override{text.empty(a);}void destroy(Word a)override{text.destroy(a);}Word length(Word a)override{return static_cast<Word>(std::strlen(static_cast<const char*>(ptr(a))));}Word compare(Word a,Word b,Word c)override{return static_cast<Word>(std::strncmp(static_cast<const char*>(ptr(a)),static_cast<const char*>(ptr(b)),c));}
Word network(Word a)override{return text.network(a);}Word connection(Word a,Word b)override{return text.connection(a,b);}Word counted(Word a,Word b,Word c)override{return word(resman_string_counted_005e5b70(ptr(a),nullptr,static_cast<const char*>(ptr(b)),c));}void assign(Word a,Word b)override{text.assign(a,b);}
void status_text(Word a,Word b,Word c)override{NativeServerJoin p(m);(void)status_packet_00579890(m,p,a,b,c);}void status(Word a,Word b)override{NativeAdminStatus p(m);unsigned char scratch[0x20];admin_status_00579920(m,p,a,b,word(scratch));}void modules(Word,Word)override{required(0x00579b50);}void players(Word a,Word b)override{NativeServerJoin p(m);unsigned char scratch[0x20];joined_00579a80(m,p,a,b,word(scratch));}
Word player(Word a,Word b,Word c)override{return word(kotor_fastcall_004ae920(ptr(a),nullptr,b,c));}
Word player_object(Word a)override{using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(m.read(m.read(a)+4))(ptr(a)));}
void load_module(Word a,Word b,Word c)override{NativeLoadRequest p(m);alignas(8) unsigned char scratch[0xe8];(void)load_module_004b95b0(m,p,m.read(a+4),b,c,word(scratch));}
void run_module(Word a)override{NativeRun p(m);unsigned char scratch[0x10];(void)server_run_004b6270(m,p,m.read(a+4),word(scratch));}Word save_tail(Word,Word,Word,Word,Word)override{required(0x005286b1);}Word remaining_tail(Word,Word,Word,Word,Word)override{required(0x00528835);}
};
