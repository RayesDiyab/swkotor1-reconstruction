bool trigger_positive(Word value){unsigned short status;Word zero=0;__asm{fld dword ptr value
 fcomp dword ptr zero
 fnstsw status}
 return (status&0x4100u)==0;
}
void trigger_normalize(Word address){void* v=ptr(address);unsigned short status;Word one=0x3f800000;__asm{mov eax,v
 fld dword ptr[eax+8]
 fmul dword ptr[eax+8]
 fld dword ptr[eax+4]
 fmul dword ptr[eax+4]
 faddp st(1),st(0)
 fld dword ptr[eax]
 fmul dword ptr[eax]
 faddp st(1),st(0)
 fcomp dword ptr one
 fnstsw status}
 Word bits=status&0x4400u;if(bits==0||bits==0x4400u)vector_normalize_004ab130(static_cast<kotor_scene3d::Vector*>(v),nullptr);
}
void trigger_subtract(Word out,Word vertex,Word position){void*o=ptr(out);void*v=ptr(vertex);void*p=ptr(position);__asm{mov eax,v
 mov ecx,p
 mov edx,o
 fld dword ptr[eax+8]
 fsub dword ptr[ecx+8]
 fld dword ptr[eax+4]
 fsub dword ptr[ecx+4]
 fld dword ptr[eax]
 fsub dword ptr[ecx]
 fstp dword ptr[edx]
 fstp dword ptr[edx+4]
 fstp dword ptr[edx+8]}
}
void trigger_translate_store(Word target,Word position,Word vector,Word scratch){void*t=ptr(target);void*p=ptr(position);void*v=ptr(vector);void*s=ptr(scratch);__asm{mov ecx,p
 mov eax,v
 mov edx,t
 fld dword ptr[ecx+8]
 fadd dword ptr[eax+8]
 fld dword ptr[ecx+4]
 fadd dword ptr[eax+4]
 fld dword ptr[ecx]
 fadd dword ptr[eax]
 mov ecx,s
 fstp dword ptr[ecx]
 mov eax,dword ptr[ecx]
 mov dword ptr[edx],eax
 fstp dword ptr[ecx+4]
 mov eax,dword ptr[ecx+4]
 mov dword ptr[edx+4],eax
 fstp dword ptr[ecx+8]
 mov eax,dword ptr[ecx+8]
 mov dword ptr[edx+8],eax}
}
