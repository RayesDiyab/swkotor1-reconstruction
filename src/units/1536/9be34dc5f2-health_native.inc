struct Health:recovered_creature_health::Ports{M m;U read(U a)override{return m.read(a);}U byte(U a)override{return m.read8(a);}U half(U a)override{return m.read16(a);}U party()override{return read(m.server()+4)+0x1b770;}U slot(U a,U offset,U arg,bool stack)override{U f=read(read(a)+offset);if(stack){using F=unsigned short(__thiscall*)(void*,unsigned);return reinterpret_cast<F>(f)(ptr(a),arg);}using F=void*(__thiscall*)(void*);return word(reinterpret_cast<F>(f)(ptr(a)));}U ratio(U a,U b)override{int hp=static_cast<short>(a),max=static_cast<short>(b);float rounded;unsigned status;__asm{
 fild hp
 fstp rounded
 fild max
 fdivr rounded
 fcomp recovered_health_threshold_007a1b38
 fnstsw ax
 mov status,eax
}U bits=(status>>8)&5;return bits==1||bits==4;}};
unsigned __fastcall current_hp(void*s,void*,unsigned flag){Health p;return recovered_creature_health::current(word(s),flag,p);}
unsigned short __fastcall maximum_hp(void*s,void*,unsigned flag){Health p;return static_cast<unsigned short>(recovered_creature_health::maximum(word(s),flag,p));}
