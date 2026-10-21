bool door_bounds_less(Word a,Word b){unsigned short status;
 __asm{fld dword ptr a
 fcomp dword ptr b
 fnstsw status}
 return ((status>>8)&5u)==1u||((status>>8)&5u)==4u;
}
bool door_bounds_greater(Word a,Word b){unsigned short status;
 __asm{fld dword ptr a
 fcomp dword ptr b
 fnstsw status}
 return (status&0x4100u)==0;
}
