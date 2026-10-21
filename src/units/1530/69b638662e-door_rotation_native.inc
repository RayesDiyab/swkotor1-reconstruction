struct NativeDoorRotation final:recovered_door_rotation::Ports{NativeMemory m;unsigned read(unsigned a)override{return m.read(a);}void write(unsigned a,unsigned b)override{m.write(a,b);}bool derive(unsigned a,unsigned b,unsigned c,unsigned d,unsigned out)override{unsigned x,y;const unsigned one=0x3f800000;const unsigned threshold=0x38d1b717;unsigned short status;
 __asm{fld dword ptr c
 fmul dword ptr b
 fld dword ptr d
 fmul dword ptr a
 fsubp st(1),st(0)
 fadd st(0),st(0)
 fstp dword ptr x
 fld dword ptr d
 fmul dword ptr d
 fld dword ptr b
 fmul dword ptr b
 faddp st(1),st(0)
 fadd st(0),st(0)
 fsubr dword ptr one
 fstp dword ptr y
 fld dword ptr y
 fmul dword ptr y
 fld dword ptr x
 fmul dword ptr x
 faddp st(1),st(0)
 fcomp dword ptr threshold
 fnstsw status}
 m.write(out,x);m.write(out+4,y);m.write(out+8,0);return (status&0x4100)==0;}
 unsigned normalize(unsigned a,unsigned b)override{return reinterpret_cast<unsigned>(normalize_004ab180(static_cast<kotor_scene3d::Vector*>(ptr(a)),static_cast<const kotor_scene3d::Vector*>(ptr(b))));}
 void orientation(unsigned a,unsigned b)override{object_orientation_004cd170(m,a,b);}
};
extern "C" void __fastcall recovered_door_rotation_005896e0(void*self,void*,unsigned a,unsigned b,unsigned c,unsigned d){NativeDoorRotation p;alignas(4)unsigned scratch[6];recovered_door_rotation::rotate(word(self),a,b,c,d,word(scratch),p);}
