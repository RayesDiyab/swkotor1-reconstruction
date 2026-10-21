// Numeric providers retain original x87 spills and status tests.
static void mesh_clip_length(Word scratch){void* s=ptr(scratch);__asm{mov ecx,s
 fld dword ptr[ecx+1ch]
 fsub dword ptr[ecx+10h]
 fld st(0)
 fld dword ptr[ecx+20h]
 fsub dword ptr[ecx+14h]
 fst dword ptr[ecx+2ch]
 fld dword ptr[ecx+24h]
 fsub dword ptr[ecx+18h]
 fst dword ptr[ecx+70h]
 fstp dword ptr[ecx+30h]
 fxch st(2)
 fstp dword ptr[ecx+34h]
 fld dword ptr[ecx+70h]
 fmul dword ptr[ecx+30h]
 fxch st(2)
 fmul dword ptr[ecx+2ch]
 faddp st(2),st(0)
 fld dword ptr[ecx+34h]
 fmul st(0),st(1)
 faddp st(2),st(0)
 fxch st(1)
 fstp dword ptr[ecx+70h]
 fstp st(0)}}
static bool mesh_clip_first_compare(Word scratch,Word point,bool even){void*s=ptr(scratch);void*p=ptr(point);unsigned result;
 __asm{mov ecx,s
 mov edx,p
 cmp even,0
 je clip_first_odd
 fld dword ptr[edx]
 mov eax,dword ptr[edx+0ch]
 fld dword ptr[edx+4]
 mov dword ptr[ecx+34h],eax
 fstp dword ptr[ecx+2ch]
 fld dword ptr[edx+10h]
 fstp dword ptr[ecx+38h]
 fsub dword ptr[ecx+10h]
 fld st(0)
 fld dword ptr[ecx+2ch]
 fsub dword ptr[ecx+14h]
 fld st(0)
 fld dword ptr[ecx+18h]
 fchs
 fst dword ptr[ecx+08h]
 mov eax,dword ptr[ecx+08h]
 fst dword ptr[ecx+0ch]
 fxch st(2)
 mov dword ptr[ecx+30h],eax
 fstp dword ptr[ecx+2ch]
 jmp clip_first_common
 clip_first_odd:
 fld dword ptr[edx]
 fld dword ptr[edx+4]
 fstp dword ptr[ecx+38h]
 fsub dword ptr[ecx+10h]
 fld st(0)
 fld dword ptr[ecx+38h]
 fsub dword ptr[ecx+14h]
 fld st(0)
 fld dword ptr[ecx+18h]
 fchs
 fst dword ptr[ecx+08h]
 mov eax,dword ptr[ecx+08h]
 fxch st(2)
 fstp dword ptr[ecx+2ch]
 mov dword ptr[ecx+30h],eax
 clip_first_common:
 fld dword ptr[ecx+30h]
 fmul st(0),st(2)
 fld dword ptr[ecx+2ch]
 fmul st(0),st(2)
 faddp st(1),st(0)
 fxch st(4)
 fmul st(0),st(3)
 faddp st(4),st(0)
 fld dword ptr[ecx+70h]
 fxch st(4)
 fxch st(1)
 fxch st(4)
 fcompp
 fstp st(0)
 fnstsw ax
 fstp st(1)
 test ah,5
 fstp st(0)
 setp al
 movzx eax,al
 mov result,eax}
 return result!=0;
}
static bool mesh_clip_second_compare(Word scratch){void*s=ptr(scratch);unsigned result;__asm{mov ecx,s
 fld dword ptr[ecx+34h]
 fsub dword ptr[ecx+10h]
 fld st(0)
 fld dword ptr[ecx+38h]
 fsub dword ptr[ecx+14h]
 fld st(0)
 fld dword ptr[ecx+0ch]
 fst dword ptr[ecx+50h]
 mov eax,dword ptr[ecx+50h]
 fxch st(2)
 mov dword ptr[ecx+3ch],eax
 fstp dword ptr[ecx+38h]
 fld dword ptr[ecx+3ch]
 fmul st(0),st(2)
 fld dword ptr[ecx+38h]
 fmul st(0),st(2)
 faddp st(1),st(0)
 fxch st(4)
 fmul st(0),st(3)
 faddp st(4),st(0)
 fld dword ptr[ecx+70h]
 fxch st(4)
 fxch st(1)
 fxch st(4)
 fcompp
 fstp st(0)
 fnstsw ax
 fstp st(1)
 test ah,5
 fstp st(0)
 setp al
 movzx eax,al
 mov result,eax}
 return result!=0;}
