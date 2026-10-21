extern "C" unsigned __fastcall recovered_area_index_00506f20(void*,void*,unsigned*,unsigned);
#include "9574bd9c18-area_collision_scan_native_reaction.inc"
struct NativeAreaCreatureCollision final:AreaCreatureCollisionPorts{
 bool equal(Word a,Word b)override{return aurora_vector_equal_004aa980(ptr(a),ptr(b))!=0;}
 Word radius(Word bits)override{float in,out,delta=0.01f;std::memcpy(&in,&bits,4);__asm{fld in
 fadd delta
 fstp out}Word r;std::memcpy(&r,&out,4);return r;}
 Word creature(Word a,Word b)override{return word(kotor_fastcall_004ae770(ptr(a),nullptr,b));}
 void geometry(Word a,Word b,Word scratch)override{void*from=ptr(a);void*to=ptr(b);void*s=ptr(scratch);float zero=0.0f,one=1.0f;
 __asm{mov edi,from
 mov esi,to
 mov edx,s
 fld dword ptr[esi+4]
 fsub dword ptr[edi+4]
 fld dword ptr[esi]
 fsub dword ptr[edi]
 fstp dword ptr[edx+2ch]
 fld dword ptr[esi]
 fcomp dword ptr[edi]
 fnstsw ax
 test ah,41h
 jnz collision_max_x_from
 mov eax,dword ptr[esi]
 jmp collision_max_x_store
 collision_max_x_from:
 mov eax,dword ptr[edi]
 collision_max_x_store:
 mov dword ptr[edx+1ch],eax
 fld dword ptr[esi]
 fcomp dword ptr[edi]
 fnstsw ax
 test ah,5
 jp collision_min_x_from
 mov eax,dword ptr[esi]
 jmp collision_min_x_store
 collision_min_x_from:
 mov eax,dword ptr[edi]
 collision_min_x_store:
 mov dword ptr[edx+18h],eax
 fld dword ptr[esi+4]
 fcomp dword ptr[edi+4]
 fnstsw ax
 test ah,41h
 jnz collision_max_y_from
 mov eax,dword ptr[esi+4]
 jmp collision_max_y_store
 collision_max_y_from:
 mov eax,dword ptr[edi+4]
 collision_max_y_store:
 mov dword ptr[edx+34h],eax
 fld dword ptr[esi+4]
 fcomp dword ptr[edi+4]
 fnstsw ax
 test ah,5
 jp collision_min_y_from
 mov eax,dword ptr[esi+4]
 jmp collision_min_y_store
 collision_min_y_from:
 mov eax,dword ptr[edi+4]
 collision_min_y_store:
 mov dword ptr[edx+30h],eax
 fld dword ptr[edx+2ch]
 fcomp zero
 fnstsw ax
 test ah,44h
 jp collision_slope
 fstp st(0)
 mov dword ptr[edx+14h],0
 mov dword ptr[edx+48h],0
 jmp collision_geometry_done
 collision_slope:
 fdiv dword ptr[edx+2ch]
 fst dword ptr[edx+14h]
 fmul dword ptr[edi]
 fsubr dword ptr[edi+4]
 fstp dword ptr[edx+48h]
 fld dword ptr[edx+14h]
 fmul dword ptr[edx+14h]
 fadd one
 fdivr one
 fsqrt
 fstp dword ptr[edx+38h]
 collision_geometry_done:}
 }
 Word expand(Word bits,bool up)override{float in,out,delta=6.0f;std::memcpy(&in,&bits,4);if(up){__asm{fld in
 fadd delta
 fstp out}}else{__asm{fld in
 fsub delta
 fstp out}}Word r;std::memcpy(&r,&out,4);return r;}
 Word index(Word a,Word b,Word c)override{return recovered_area_index_00506f20(ptr(a),nullptr,static_cast<unsigned*>(ptr(b)),c);}
 Word nonempty_004bc790(Word a,Word b,Word,Word,Word output,Word scratch)override{NativeMemory m;NativeAreaCollisionScan p(m);return area_collision_scan_004bc790(m,p,a,b,output,scratch);}
};
