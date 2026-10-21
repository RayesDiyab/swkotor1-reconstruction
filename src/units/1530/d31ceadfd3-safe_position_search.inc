// Original 004BE860 continuation 004BE93D; no separate function credit.
// Bounded x87 primitives preserve original memory operands, stores and live comparisons.
#include "3c66185f25-w5_types.hpp"
struct W1Vector { unsigned x, y, z; };
extern "C" unsigned __fastcall recovered_area_position_walkable_00506400(void*,void*,W1Vector);
extern "C" unsigned __fastcall recovered_area_no_nonwalk_polys_in_doors_0050ca10(void*,void*,void*,void*,float,float,float,unsigned,unsigned*,void*,unsigned,unsigned);
extern "C" double __fastcall w5_room_compute_height_00579430(void*,void*,w5::Vector,w5::I);
extern "C" w5::I __fastcall w5_area_test_direct_line_004bcb70(void*,void*,const w5::Vector*,const w5::Vector*,const float*,float,w5::U,void*);
namespace safe_position_search {
inline Word add(Word left,Word right){Word result;__asm {fld left
 fadd right
 fstp result}return result;}
inline Word subtract(Word left,Word right){Word result;__asm {fld left
 fsub right
 fstp result}return result;}
inline Word status(Word left,Word right){Word result;__asm {fld left
 fcomp right
 fnstsw ax
 movzx eax,ax
 mov result,eax}return result;}
inline bool at_most(Word left,Word right){const Word fp_status=status(left,right);return ((fp_status>>8&0x41)==1)||((fp_status>>8&0x41)==0x40);}
inline bool below(Word left,Word right){const Word fp_status=status(left,right);return (fp_status&0x0500)==0x0100;}
inline bool advance(Word* cursor,Word step,Word limit,bool strict){Word fp_status;__asm {mov ecx,cursor
 fld dword ptr[ecx]
 fadd step
 fst dword ptr[ecx]
 fcomp limit
 fnstsw ax
 movzx eax,ax
 mov fp_status,eax}
 return strict ? (fp_status&0x0500)==0x0100 : (((fp_status>>8&0x41)==1)||((fp_status>>8&0x41)==0x40));}
inline bool opposite(Word* cursor,Word band_lower,Word band_upper){Word fp_status;__asm {mov ecx,cursor
 fld band_upper
 fsub band_lower
 fadd dword ptr[ecx]
 fst dword ptr[ecx]
 fcomp band_upper
 fnstsw ax
 movzx eax,ax
 mov fp_status,eax}
 return ((fp_status>>8&0x41)==1)||((fp_status>>8&0x41)==0x40);}
inline Word height_store(double value){Word result;__asm {fld value
 fstp result}return result;}
inline void initial_box(Word* current,Word height,Word* minus,Word* plus,Word* top){
 const float epsilon=0.01f;
 __asm {mov ecx,current
 mov edx,plus
 mov eax,minus
 fld height
 fld dword ptr[ecx]
 fadd epsilon
 fstp dword ptr[edx]
 fld dword ptr[ecx+4]
 fadd epsilon
 fstp dword ptr[edx+4]
 fld dword ptr[ecx+8]
 fst dword ptr[edx+8]
 fld dword ptr[ecx]
 fsub epsilon
 fstp dword ptr[eax]
 fld dword ptr[ecx+4]
 fsub epsilon
 fstp dword ptr[eax+4]
 fstp dword ptr[eax+8]
 fadd dword ptr[ecx+8]
 mov eax,top
 fstp dword ptr[eax]}
}
inline void horizontal_band(Word current_z,Word input_z,Word height,Word* band_lower,Word* band_upper){
 const float margin=0.1f;
 __asm {mov ecx,band_lower
 mov edx,band_upper
 fld current_z
 fcom input_z
 fnstsw ax
 test ah,5
 jp horizontal_input_low
 fstp dword ptr[ecx]
 fld input_z
 jmp horizontal_band_finish
 horizontal_input_low:
 mov eax,input_z
 mov dword ptr[ecx],eax
 horizontal_band_finish:
 fld dword ptr[ecx]
 fsub margin
 fstp dword ptr[ecx]
 fadd height
 fstp dword ptr[edx]}
}
inline void vertical_band(Word current_z,Word input_z,Word* band_lower,Word* band_upper){
 __asm {mov ecx,band_lower
 mov edx,band_upper
 fld current_z
 fcom input_z
 fnstsw ax
 test ah,5
 jp vertical_input_low
 fstp dword ptr[ecx]
 mov eax,input_z
 mov dword ptr[edx],eax
 jmp vertical_band_finish
 vertical_input_low:
 fstp dword ptr[edx]
 mov eax,input_z
 mov dword ptr[ecx],eax
 vertical_band_finish:}
}
inline w5::Vector vector_at(Word address){w5::Vector result;std::memcpy(&result,ptr(address),12);return result;}
inline Word doors(Word area,Word from,Word to,Word band_lower,Word band_upper,Word radius,Word result){
 // Raw-bit locals feed real float parameters without an ST0-return conversion helper.
 float lo,hi,r;std::memcpy(&lo,&band_lower,4);std::memcpy(&hi,&band_upper,4);std::memcpy(&r,&radius,4);
 return recovered_area_no_nonwalk_polys_in_doors_0050ca10(ptr(area),nullptr,ptr(from),ptr(to),lo,hi,r,0x7f000000u,static_cast<unsigned*>(ptr(result)),nullptr,1,0);
}
inline int direct_line(Word area,Word from,Word to,Word radius_bits,Word height_bits){
 float radius,height;
 std::memcpy(&radius,&radius_bits,4);std::memcpy(&height,&height_bits,4);
 return w5_area_test_direct_line_004bcb70(ptr(area),nullptr,reinterpret_cast<const w5::Vector*>(ptr(from)),reinterpret_cast<const w5::Vector*>(ptr(to)),&radius,height,0,nullptr);
}
}
