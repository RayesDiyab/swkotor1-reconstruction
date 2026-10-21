#include "ba24338184-collision_methods.h"
namespace collision_transport {
// The portable interface carries raw DWORDs. Original callers PUSH the radius
// without an FP load, including signaling NaNs. Keep that precise ABI seam.
inline Word contact(Word area,Word info,Word position,Word id,Word from,Word radius_bits,Word unused,Word mode,Word output){Word result;__asm{
 push output
 push mode
 push unused
 push radius_bits
 push from
 push id
 push position
 push info
 mov ecx,area
 xor edx,edx
 call w3_area_collision_contact_004bba40
 mov result,eax
}return result;}
inline std::int32_t notify_result(Word area,Word center,Word radius_bits,Word segment_start,Word segment_end,Word info){std::int32_t result;__asm{
 push info
 push segment_end
 push segment_start
 push radius_bits
 push center
 mov ecx,area
 xor edx,edx
 call w3_area_collision_notify_004bc0e0
 mov result,eax
}return result;}
inline void notify(Word area,Word center,Word radius_bits,Word segment_start,Word segment_end,Word info){(void)notify_result(area,center,radius_bits,segment_start,segment_end,info);}
}
