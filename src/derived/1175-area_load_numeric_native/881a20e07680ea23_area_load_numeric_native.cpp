#include <cmath>
#include <cstdint>
#include "0b41d8ac71e50cea_ftol2.h"
// Finite area-width product adapter: reuse the linked CRT ceil, then the retained
// original ST0 -> EDX:EAX conversion. This is not a new generic 006FC880 body.
extern "C" unsigned __cdecl recovered_area_load_numeric_0050e216(unsigned product,unsigned scale){
 double input;__asm{fild dword ptr product}__asm{fmul dword ptr scale}__asm{fstp qword ptr input}
 double rounded=::ceil(input);unsigned result;__asm{fld qword ptr rounded}__asm{call unresolved_ftol2_006fae8c}__asm{mov result,eax}return result;
}
