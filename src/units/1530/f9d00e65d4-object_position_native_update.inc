struct NativeObjectPosition:ObjectPositionPorts{NativeMemory&m;explicit NativeObjectPosition(NativeMemory&v):m(v){}
Word equal(Word a,Word b)override{for(Word i=0;i<12;i+=4){Word left=m.read(a+i);Word right=m.read(b+i);if(!object_float_equal_x87(left,right))return 0;}return 1;}
Word object_array(Word a)override{return m.read(m.read(a+4)+0x1005c);}
void lookup(Word a,Word b,Word c)override{(void)kotor_fastcall_004d8230(ptr(a),nullptr,b,static_cast<void**>(ptr(c)));}
Word virtual_call(Word a,Word slot)override{using F=void*(__fastcall*)(void*,void*);return word(reinterpret_cast<F>(m.read(m.read(a)+slot))(ptr(a),nullptr));}
void update_area(Word a,Word b)override{(void)native_area_update(a,b);}
};
void native_object_position(Word a,Word b,Word c,Word d,Word e){NativeMemory m;NativeObjectPosition p(m);unsigned out;object_position_004cd1a0(m,p,a,b,c,d,e,word(&out));}
