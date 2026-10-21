struct NativePlaceableRotation final:recovered_placeable_rotation::Ports{NativeDoorRotation base;unsigned read(unsigned a)override{return base.read(a);}void write(unsigned a,unsigned b)override{base.write(a,b);}void derive(unsigned q,unsigned out)override{const unsigned one=0x3f800000;
 __asm{mov eax,q
 mov edx,out
 fld dword ptr[eax+8]
 fmul dword ptr[eax+4]
 fld dword ptr[eax+12]
 mov dword ptr[edx+8],0
 fmul dword ptr[eax]
 fsubp st(1),st(0)
 fadd st(0),st(0)
 fstp dword ptr[edx]
 fld dword ptr[eax+12]
 fld dword ptr[eax+4]
 fld st(1)
 fmul st(0),st(2)
 fld st(1)
 fmul st(0),st(2)
 faddp st(1),st(0)
 fadd st(0),st(0)
 fsubr dword ptr one
 fstp dword ptr[edx+4]
 fstp st(0)
 fstp st(0)}
 }unsigned normalize(unsigned a,unsigned b)override{return base.normalize(a,b);}void orientation(unsigned a,unsigned b)override{base.orientation(a,b);}};
extern "C" void __fastcall recovered_placeable_rotation_005853c0(void*self,void*,void*quaternion){NativePlaceableRotation p;unsigned scratch[6];recovered_placeable_rotation::rotate(word(self),word(quaternion),word(scratch),p);}
