struct NativeDoorFinishMemory:NativeMemory{Word read(Word a)override{if(a==0x746890)return 16383;return NativeMemory::read(a);}Word read16(Word a)override{if(a==0x746684||a==0x7473c4)return 2;return NativeMemory::read16(a);}};
struct NativeDoorFinish final:DoorFinishPorts{NativeMemory&m;explicit NativeDoorFinish(NativeMemory&v):m(v){}
 Word allocate(Word a)override{return word(::operator new(a));}Word construct(Word a,Word b)override{return native_effect_construct(a,b);}
 void integer(Word a,Word b,Word c)override{effect_integer_005036a0(m,a,b,c);}void apply(Word a,Word b,Word c,Word d)override{native_effect_apply(a,b,c,d);}
 void state_00589600(Word a,Word b,Word c)override{recovered_door_state_00589600(ptr(a),nullptr,b,c);}
 void empty(Word a)override{m.write(a,0);m.write(a+4,0);}void literal(Word a,Word b)override{if(b!=0x7498a0)required(b);frame_LogStringCtor_005e5a90(ptr(a),nullptr,"StaticAnim");}
 Word table_string(Word a,Word b,Word c,Word d)override{return static_cast<Word>(c2da_current_get_string_00413270(ptr(a),nullptr,static_cast<int>(b),ptr(c),ptr(d)));}
 void destroy(Word a)override{frame_LogStringDtor_005e5c20(ptr(a),nullptr);}void assign_literal(Word a,Word b)override{if(b!=0x73d71c)required(b);GuiAssign005e5140(ptr(a),nullptr,"");}
 Word compare(Word a,Word b)override{if(b!=0x749898)required(b);return ini_comparison::CompareNC005e58c0(static_cast<kotor_reconstruction::StartupString*>(ptr(a)),nullptr,"busted");}
 void animation(Word a,Word b)override{using F=void(__fastcall*)(void*,void*,unsigned);reinterpret_cast<F>(m.read(m.read(a)+0x7c))(ptr(a),nullptr,b);}
};
void native_door_finish(Word self){NativeDoorFinishMemory m;NativeDoorFinish p(m);alignas(4) unsigned char scratch[0x18];door_finish_00589d40(m,p,self,word(scratch));}
