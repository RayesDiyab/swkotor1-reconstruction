extern "C" unsigned __fastcall recovered_party_list_00567f90(void*,void*,unsigned,unsigned,const void*,unsigned,unsigned);
// 005BF920 CSWSFaction::RemoveMember(unsigned id): __thiscall, ECX=this, one
// stack argument, RET 4.  The port takes (faction, id); bind it to the real
// reconstructed body ?RemoveMember@CSWSFaction@case54_r2@@QAEXI@Z without a
// second implementation.  __fastcall here is the ABI-faithful call shape:
// ECX=this, EDX unused, id on the stack, callee pops 4.
extern "C" void __fastcall recovered_faction_remove_005bf920(void*,void*,unsigned);
#pragma comment(linker, "/alternatename:@recovered_faction_remove_005bf920@12=?RemoveMember@CSWSFaction@case54_r2@@QAEXI@Z")
struct NativeFactionMember:FactionMemberPorts,FactionMemberHelperPorts{NativeMemory&m;explicit NativeFactionMember(NativeMemory&v):m(v){}
Word creature(Word a,Word b)override{return word(kotor_fastcall_004ae770(ptr(a),nullptr,b));}Word current(Word a)override{return creature_faction_00513fc0(m,a);}void remove(Word a,Word b)override{recovered_faction_remove_005bf920(ptr(a),nullptr,b);}void notification(Word a,Word b)override{unsigned argument;faction_notify_005be9c0(m,*this,a,b,word(&argument));}void append(Word a,Word b)override{unique_append_006449d0(m,*this,a,b);}void grow(Word a,Word b)override{menu_list_growth::allocate_005e03c0(static_cast<kotor_reconstruction::ArrayMutations*>(ptr(a)),nullptr,b);}Word message(Word a)override{return m.read(m.read(a+4)+0x10010);}Word player(Word a,Word b)override{return word(kotor_fastcall_004ae910(ptr(a),nullptr,b));}void send(Word a,Word b,Word c,Word d,Word e,Word f)override{(void)recovered_party_list_00567f90(ptr(a),nullptr,b,c,ptr(d),e,f);}
};
void native_faction_member(Word a,Word b,Word c){NativeMemory m;NativeFactionMember p(m);faction_member_005bfa70(m,p,a,b,c);}
void native_unique_append(Word a,Word b){NativeMemory m;NativeFactionMember p(m);unique_append_006449d0(m,p,a,b);}
