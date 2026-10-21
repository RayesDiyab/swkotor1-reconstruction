struct NativeAreaUpdate final:AreaUpdatePorts{
 NativeMemory&m;explicit NativeAreaUpdate(NativeMemory&v):m(v){}
 Word objects(Word a)override{return m.read(m.read(a+4)+0x1005c);}
 Word lookup(Word a,Word b,Word c)override{return kotor_fastcall_004d8230(ptr(a),nullptr,b,static_cast<void**>(ptr(c)));}
 Word compare_status(Word left,Word right)override{unsigned short status;__asm{fld dword ptr left
 fcomp dword ptr right
 fnstsw status}
 return status>>8;}
};
Word native_area_update(Word a,Word b){NativeMemory m;NativeAreaUpdate p(m);unsigned scratch[9];return area_update_00506fe0(m,p,a,b,word(scratch));}
extern "C" unsigned __fastcall recovered_area_update_00506fe0(void*self,void*,void*object){return native_area_update(word(self),word(object));}
