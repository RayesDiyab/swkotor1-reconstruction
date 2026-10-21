// W6 t1: exact x87 instruction sequences used by ExecuteCode 005D2BD0 (x86 only).
// Shared verbatim by the native port (0886 adapter) and the host tests.
// Each body is the original's FLD/F<op>/FSTP or FCOMP/FNSTSW sequence; the
// result therefore inherits the ambient x87 control word exactly as the original does.
namespace w6_x87 {
using U32=unsigned;
inline U32 binary(U32 op,U32 lhs,U32 rhs){ // 005D3B48.. / 005D38E8..: FLD lhs; Fop rhs; FSTP
 U32 out=0;
 switch(op){
 case 0x14:__asm{fld dword ptr lhs}__asm{fadd dword ptr rhs}__asm{fstp dword ptr out}break;
 case 0x15:__asm{fld dword ptr lhs}__asm{fsub dword ptr rhs}__asm{fstp dword ptr out}break;
 case 0x16:__asm{fld dword ptr lhs}__asm{fmul dword ptr rhs}__asm{fstp dword ptr out}break;
 default:__asm{fld dword ptr lhs}__asm{fdiv dword ptr rhs}__asm{fstp dword ptr out}break;
 }
 return out;
}
inline unsigned status_compare(U32 lhs,U32 rhs){unsigned short sw=0;__asm{fld dword ptr lhs}__asm{fcomp dword ptr rhs}__asm{fnstsw ax}__asm{mov sw,ax}return sw>>8;}
inline unsigned popcount_bits(unsigned v){unsigned n=0;for(;v;v&=v-1)++n;return n;}
inline U32 compare(U32 op,U32 lhs,U32 rhs){ // 005D356A..005D35CE
 const unsigned ah=status_compare(lhs,rhs);
 switch(op){
 case 0x0d:return (ah&1)==0;                 // test ah,1 ; je push1
 case 0x10:return popcount_bits(ah&0x41)==1; // test ah,41 ; jnp push1
 case 0x0e:return (ah&0x41)==0;              // test ah,41 ; je push1
 default:return popcount_bits(ah&5)==1;      // test ah,5 ; jp push0
 }
}
inline bool compare_equal(U32 lhs,U32 rhs){return popcount_bits(status_compare(lhs,rhs)&0x44)==1;} // FCOMP; test ah,44h; jp -> unequal
inline bool is_zero(U32 v){ // FLD m32; FCOMP m64 [0073EDF8]=0.0; test ah,44 ; jnp -> zero
 const double zero=0.0;unsigned short sw=0;
 __asm{fld dword ptr v}__asm{fcomp qword ptr zero}__asm{fnstsw ax}__asm{mov sw,ax}
 return popcount_bits((sw>>8)&0x44)==1;
}
// __ftol2: FISTP qword with truncation; EAX = low 32 bits. NaN, inf and |x| >= 2^63 give the 64-bit integer
// indefinite 0x8000000000000000, whose low 32 bits are 0.
inline U32 ftol2_low32(double x){
 if(!(x>-9223372036854775808.0&&x<9223372036854775808.0))return 0;
 const long long t=static_cast<long long>(x);   // C++ conversion truncates toward zero, as __ftol2 does
 return static_cast<U32>(static_cast<unsigned long long>(t));
}
inline U32 negate(U32 v){U32 out=0;__asm{fld dword ptr v}__asm{fchs}__asm{fstp dword ptr out}return out;}
inline U32 from_int(U32 v){U32 out=0;__asm{fild dword ptr v}__asm{fstp dword ptr out}return out;}
inline U32 reload(U32 v){U32 out=0;__asm{fld dword ptr v}__asm{fstp dword ptr out}return out;}
// 00423BF0 vectorComponentMult (out = v * s), original sequence.
inline void component_mult(U32* out,const U32* v,U32 s){
 U32 x=v[0],y=v[1],z=v[2],ox=0,oy=0,oz=0;
 __asm{fld dword ptr s}__asm{fmul dword ptr z}__asm{fld dword ptr s}__asm{fmul dword ptr y}__asm{fld dword ptr s}__asm{fmul dword ptr x}
 __asm{fstp dword ptr ox}__asm{fstp dword ptr oy}__asm{fstp dword ptr oz}
 out[0]=ox;out[1]=oy;out[2]=oz;
}
// 00437A90 operator/(Vector,float): zero -> copy; else r=1/s kept in ST for z,
// the FST-rounded r for y and x (original register reuse).
inline void divide(U32* out,const U32* v,U32 s){
 const float zero=0.0f,one=1.0f;unsigned short sw=0;
 __asm{fld dword ptr s}__asm{fcomp dword ptr zero}__asm{fnstsw ax}__asm{mov sw,ax}
 if(popcount_bits((sw>>8)&0x44)==1){out[0]=v[0];out[1]=v[1];out[2]=v[2];return;}
 U32 x=v[0],y=v[1],z=v[2],ox=0,oy=0,oz=0,rs=s;
 __asm{fld dword ptr one}__asm{fdiv dword ptr rs}__asm{fst dword ptr rs}__asm{fmul dword ptr z}
 __asm{fld dword ptr rs}__asm{fmul dword ptr y}__asm{fld dword ptr rs}__asm{fmul dword ptr x}
 __asm{fstp dword ptr ox}__asm{fstp dword ptr oy}__asm{fstp dword ptr oz}
 out[0]=ox;out[1]=oy;out[2]=oz;
}
}
