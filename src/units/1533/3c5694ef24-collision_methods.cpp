#include "ba24338184-collision_methods.h"
#include "17849a3f29-collision_math.h"
#include "9ddb018d05-world_los.h"
#include <cstring>
namespace menu_continuation {struct AppManager;extern AppManager* app_manager_007a39fc;}
extern "C" {
extern unsigned recovered_debug_cell_00832838;
void* __fastcall kotor_fastcall_004ae770(void*,void*,unsigned);
float* __cdecl kotor_cdecl_00423bc0(float*,const float*,const float*);
}
namespace collision {
template<class T>T read(const void* object,U offset){T value;std::memcpy(&value,static_cast<const unsigned char*>(object)+offset,sizeof value);return value;}
inline void* field(void* object,U offset){return static_cast<unsigned char*>(object)+offset;}
inline const V* position(void* object){return static_cast<const V*>(field(object,0x90));}
inline void copy(V* destination,const V* source){for(U n=0;n!=12;n+=4){const U bits=read<U>(source,n);std::memcpy(static_cast<unsigned char*>(static_cast<void*>(destination))+n,&bits,4);}}
inline void* creature(U id){void* server=read<void*>(menu_continuation::app_manager_007a39fc,8);return kotor_fastcall_004ae770(server,nullptr,id);}
// Original PUSH preserves the incoming radius bits until the actual callee FLD.
inline I intersect(const V* center,const float* radius,const V* segment_start,const V* segment_end,V* first,V* second){I result;__asm{
 push 1
 push second
 push first
 push segment_end
 push segment_start
 mov eax,radius
 push dword ptr[eax]
 push center
 call intersect_circle_2d_004aab50
 add esp,1ch
 mov result,eax
}return result;}
}
extern "C" collision::U __fastcall w3_area_collision_contact_004bba40(void*,void*,void* info,const collision::V* point,collision::U id,const collision::V* from,float radius,collision::U,collision::I mode,collision::U* output_id){
 using namespace collision;
 if(recovered_debug_cell_00832838){
  void* first=creature(id);
  void* second=creature(read<U>(info,0x2c));
  V first_position,second_position,delta,unit;
  debug_difference(&delta,&first_position,&second_position,position(first),position(second));
  (void)normalize_004ab180(&unit,&delta);
  difference(&delta,&second_position,&first_position);
  const V* normalized=normalize_004ab180(&unit,&delta);
  copy(&delta,normalized);
  (void)creature(id);
 }
 if(!outside_or_unordered(point,from,&radius))return 0;
 if(output_id)*output_id=id;
 if(mode!=1||!info)return 0;
 void* other=creature(read<U>(info,0x30));
 void* current=creature(id);
 if(!other||!current)return 0;
 V delta;
 (void)kotor_cdecl_00423bc0(&delta.x,&position(other)->x,&position(current)->x);
 return overlapping_xy(&delta,current,other)?1u:0u;
}
extern "C" collision::I __fastcall w3_area_collision_notify_004bc0e0(void*,void*,const collision::V* center,float radius,const collision::V* start,const collision::V* end,void* info){
 using namespace collision;
 if(!info)return 1;
 V first,second;std::memset(&first,0,sizeof first);std::memset(&second,0,sizeof second);
 const I count=intersect(center,&radius,start,end,&first,&second);
 V* chosen;V* opposite;
 if(count==2&&second_is_nearer(&first,&second,start)){chosen=&second;opposite=&first;}
 else if(count==1||count==2){chosen=&first;opposite=&second;}
 else return count;
 V delta,normal,plus;
 difference(&delta,chosen,center);
 (void)normalize_004ab180(&normal,&delta);
 perpendicular(&plus,opposite,chosen,&normal);
 walk_hit_info_initialize_004bb570(info,nullptr,1,chosen,&normal,opposite,&plus,1,&normal);
 return count;
}
