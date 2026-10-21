// Returns0 for no intersection,1 for endpoint path,2 for central projected path.
static Word collision_numeric_route(Word scratch){void*s=ptr(scratch);float zero=0.0f,one=1.0f,negative=-1.0f;Word result;
 __asm{mov ecx,s
 fld dword ptr[ecx+30h]
 fcomp zero
 fnstsw ax
 test ah,44h
 jp collision_route_slope
 fld dword ptr[ecx+64h]
 fcomp dword ptr[ecx+30h]
 fnstsw ax
 test ah,41h
 jp collision_route_upper
 fld dword ptr[ecx+18h]
 fsub dword ptr[ecx+60h]
 fld dword ptr[ecx+30h]
 fsub dword ptr[ecx+64h]
 jmp collision_route_distance
 collision_route_upper:
 fld dword ptr[ecx+64h]
 fcomp dword ptr[ecx+34h]
 fnstsw ax
 test ah,1
 jnz collision_route_range
 fld dword ptr[ecx+1ch]
 fsub dword ptr[ecx+60h]
 fld dword ptr[ecx+34h]
 fsub dword ptr[ecx+64h]
 collision_route_distance:
 fld st(0)
 fmul st(0),st(1)
 fld st(2)
 fmul st(0),st(3)
 faddp st(1),st(0)
 fld dword ptr[ecx+10h]
 fmul dword ptr[ecx+10h]
 fcompp
 fnstsw ax
 fstp st(0)
 test ah,1
 fstp st(0)
 jnz collision_route_none
 jmp collision_route_endpoint
 collision_route_range:
 fld dword ptr[ecx+18h]
 fsub dword ptr[ecx+10h]
 fcomp dword ptr[ecx+60h]
 fnstsw ax
 test ah,41h
 jp collision_route_none
 fld dword ptr[ecx+10h]
 fadd dword ptr[ecx+1ch]
 fcomp dword ptr[ecx+60h]
 fnstsw ax
 test ah,1
 jnz collision_route_none
 collision_route_endpoint:
 mov result,1
 jmp collision_route_done
 collision_route_slope:
 fld dword ptr[ecx+60h]
 fmul dword ptr[ecx+14h]
 fadd dword ptr[ecx+48h]
 fstp dword ptr[ecx+40h]
 fld dword ptr[ecx+64h]
 fsub dword ptr[ecx+40h]
 fabs
 fmul dword ptr[ecx+38h]
 fcom dword ptr[ecx+10h]
 fnstsw ax
 test ah,5
 jp collision_route_pop_none
 fld one
 fld dword ptr[ecx+64h]
 fcomp dword ptr[ecx+40h]
 fnstsw ax
 test ah,5
 jp collision_route_sign_ready
 fstp st(0)
 fld negative
 collision_route_sign_ready:
 fmul st(0),st(1)
 fmul dword ptr[ecx+38h]
 fmul dword ptr[ecx+14h]
 fadd dword ptr[ecx+60h]
 fstp dword ptr[ecx+40h]
 fld dword ptr[ecx+10h]
 fmul dword ptr[ecx+10h]
 fld st(1)
 fmul st(0),st(2)
 fsubp st(1),st(0)
 fsqrt
 fstp st(1)
 fmul dword ptr[ecx+38h]
 fstp dword ptr[ecx+3ch]
 fld dword ptr[ecx+40h]
 fsub dword ptr[ecx+3ch]
 fcom dword ptr[ecx+18h]
 fnstsw ax
 test ah,1
 jz collision_route_check_max
 fld dword ptr[ecx+3ch]
 fadd dword ptr[ecx+40h]
 fcomp dword ptr[ecx+18h]
 fnstsw ax
 test ah,1
 jnz collision_route_pop_none
 collision_route_check_max:
 fcomp dword ptr[ecx+1ch]
 fnstsw ax
 test ah,41h
 jnp collision_route_central
 fld dword ptr[ecx+3ch]
 fadd dword ptr[ecx+40h]
 fcomp dword ptr[ecx+1ch]
 fnstsw ax
 test ah,41h
 jp collision_route_none
 collision_route_central:
 mov result,2
 jmp collision_route_done
 collision_route_pop_none:
 fstp st(0)
 collision_route_none:
 mov result,0
 collision_route_done:}
 return result;}
