#include "42316df135e33e44_object_vector_compare.hpp"
#include <cstring>
namespace kotor::server_mainloop {
bool object_float_equal_x87(Word left,Word right){float a,b;unsigned short status;std::memcpy(&a,&left,4);std::memcpy(&b,&right,4);
#if defined(_MSC_VER) && defined(_M_IX86)
 __asm {fld a
        fcomp b
        fnstsw status}
#elif defined(__i386__) || defined(__x86_64__)
 __asm__ volatile("flds %1; fcomps %2; fnstsw %0":"=m"(status):"m"(a),"m"(b):"st");
#else
 #error Original x87 comparison requires x86.
#endif
 // TEST AH,44 / JP rejects ordered-unequal and unordered alike.
 return ((status>>8)&0x44)==0x40;
}
}
