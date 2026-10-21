struct NativeRoomHeight final:RoomHeightPorts{
 Word plane(Word normal,Word dBits,Word point)override{float* n=static_cast<float*>(ptr(normal));float* p=static_cast<float*>(ptr(point));float d,result,zero=0.0f;std::memcpy(&d,&dBits,4);
 __asm{mov ecx,n
 fld dword ptr[ecx+8]
 fcomp zero
 fnstsw ax
 test ah,44h
 jp height_calculate
 fldz
 jmp height_store
 height_calculate:
 mov edx,p
 fld dword ptr[ecx+4]
 fmul dword ptr[edx+4]
 fld dword ptr[ecx]
 fmul dword ptr[edx]
 faddp st(1),st(0)
 fadd d
 fdiv dword ptr[ecx+8]
 fchs
 height_store:
 fstp result}
 Word bits;std::memcpy(&bits,&result,4);return bits;}
};
