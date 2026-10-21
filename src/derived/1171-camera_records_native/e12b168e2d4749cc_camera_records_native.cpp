#include "4dec4b1bc7d4dc7e_camera_records.hpp"
#include <cstring>
namespace{using U=unsigned;void*ptr(U a){return reinterpret_cast<void*>(a);}U word(const void*a){return reinterpret_cast<U>(a);}struct Native final:recovered_camera_records::Ports{U read(U a)override{U v;std::memcpy(&v,ptr(a),4);return v;}void write(U a,U b)override{std::memcpy(ptr(a),&b,4);}U float_spill(U a)override{U r;__asm{fld dword ptr a}__asm{fstp dword ptr r}return r;}};}
extern "C" unsigned __fastcall recovered_camera_count_0062ac80(void*self,void*,unsigned count){Native p;return recovered_camera_records::count(word(self),count,p);}
extern "C" void __fastcall recovered_camera_insert_0062aca0(void*self,void*,unsigned index,unsigned id,void*position,void*quaternion,unsigned a,unsigned b,unsigned c,unsigned value){Native p;recovered_camera_records::insert(word(self),index,id,word(position),word(quaternion),a,b,c,value,p);}
