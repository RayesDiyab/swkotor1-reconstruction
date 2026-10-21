// closeB (Claude): private shadow copy of st/source/server_event_native.inc (shutdown port bound).
// Native event adapter; remaining disconnected-path providers are explicit gates.
extern "C" {
void __fastcall recovered_debug_timestamp_005e9410(void*,void*,void*);
void* __fastcall recovered_connection_player_00562c70(void*,void*,unsigned);
void* __fastcall recovered_connection_player_005802f0(void*,void*,unsigned);
void* __fastcall c2da_005e9d70(void*,void*,void*);
void __fastcall c2da_005e9c10(void*,void*,void*);
}
struct NativeServerEvent:ServerEventPorts {NativeMemory&m;explicit NativeServerEvent(NativeMemory&v):m(v){}
void empty(Word a)override{m.write(a,0);m.write(a+4,0);}
void timestamp(Word a,Word b)override{recovered_debug_timestamp_005e9410(ptr(a),nullptr,ptr(b));}
Word connection(Word a,Word)override{return a+0x20014;}
void copy(Word a,Word b)override{WindowCopy005e5b00(ptr(a),nullptr,ptr(b));}
void cstr(Word a)override{(void)WindowCStr005e5670(ptr(a),nullptr);}
void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}
Word allocate(Word a)override{return word(::operator new(a));}
Word player(Word address,Word a,Word b)override{if(address==0x5802f0)return word(recovered_connection_player_005802f0(ptr(a),nullptr,b));if(address==0x562c70)return word(recovered_connection_player_00562c70(ptr(a),nullptr,b));required(address);}
void add_head(Word a,Word b)override{(void)c2da_005e9d70(ptr(a),nullptr,ptr(b));}
Word at(Word a,Word b)override{return unresolved_list_at_pos_005e9b60(ptr(a),nullptr,ptr(b));}
void next(Word a,Word b)override{(void)unresolved_list_next_005e9b90(ptr(a),nullptr,static_cast<void**>(ptr(b)));}
void remove(Word a,Word b)override{c2da_005e9c10(ptr(a),nullptr,ptr(b));}
void deleting_destroy(Word a,Word flag)override{using F=void(__thiscall*)(void*,unsigned);reinterpret_cast<F>(m.read(m.read(a)))(ptr(a),flag);}
void joined(Word a,Word recipient)override{NativeServerJoin p(m);alignas(8) unsigned char scratch[0x20];joined_00579a80(m,p,a,recipient,word(scratch));}
void left(Word a,Word recipient,Word player)override{NativeModuleMessage p(m);unsigned scratch[2];message_player_left_005682b0(m,p,a,recipient,player,word(scratch));}
Word application_client(Word a,Word mode)override{return mode==0?m.read(a+12):mode==1?m.read(a+16):0;}
void client_left(Word a,Word id)override{if(id==0xffffffff||id>=96)return;Word entry=m.read(a+id*4);if(!entry)return;Word allocation=m.read(entry);if(allocation)::operator delete(ptr(allocation));::operator delete(ptr(entry));m.write(a+id*4,0);}
void shutdown(Word a,Word b,Word c)override{kotor_fastcall_004b69c0(ptr(a),nullptr,b,c);/* closeB: 004B7BDF/004B7CB2 CServerExoAppInternal::Shutdown */}
void remove_id(Word a,Word id)override{Word count=m.read(a+4),index=0;if(static_cast<int>(count)>0){Word cursor=m.read(a);do{if(m.read(cursor)==id)break;++index;cursor+=4;}while(static_cast<int>(index)<static_cast<int>(m.read(a+4)));}Word last=count-1;m.write(a+4,last);while(static_cast<int>(index)<static_cast<int>(last)){Word base=m.read(a);m.write(base+index*4,m.read(base+index*4+4));last=m.read(a+4);++index;}}
};
