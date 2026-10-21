// Native ABI transport for the selected1076 portable algorithm; no copied body.
#include "3ae3c3deb8-area_region_glue.h"
#include "f6a1f0b69c-area_region_wrapper.hpp"
#include <cstring>
namespace kotor_reconstruction {struct ArrayMutations;}
extern "C" std::uint32_t __fastcall area_no_nonwalk_polys_detailed_00506650(
    void*,void*,kotor_scene3d::Vector*,kotor_scene3d::Vector*,float,float,float,
    std::uint32_t,std::uint32_t*,std::int32_t,kotor_reconstruction::ArrayMutations*,
    std::int32_t,void*,std::int32_t);
namespace {
using U=std::uint32_t;
void* pointer(U value){return reinterpret_cast<void*>(static_cast<std::uintptr_t>(value));}
U word(const void* value){return static_cast<U>(reinterpret_cast<std::uintptr_t>(value));}
U float_word(const float& value){U result;std::memcpy(&result,&value,4);return result;}
struct Memory final:kotor::server_mainloop::Memory {
    U read(U address)override{U value;std::memcpy(&value,pointer(address),4);return value;}
    void write(U address,U value)override{std::memcpy(pointer(address),&value,4);}
    U read16(U address)override{std::uint16_t value;std::memcpy(&value,pointer(address),2);return value;}
    void write16(U address,U value)override{const auto half=static_cast<std::uint16_t>(value);std::memcpy(pointer(address),&half,2);}
    U read8(U address)override{std::uint8_t value;std::memcpy(&value,pointer(address),1);return value;}
};
struct Ports final:kotor::server_mainloop::AreaRegionPorts {
    U detailed(U self,U a,U b,U c,U d,U e,U f,U output,U flag8,U list,U flag10,U hit,U doors)override{
        float lower,upper,margin;
        std::memcpy(&lower,&c,4);std::memcpy(&upper,&d,4);std::memcpy(&margin,&e,4);
        return area_no_nonwalk_polys_detailed_00506650(pointer(self),nullptr,
            static_cast<kotor_scene3d::Vector*>(pointer(a)),static_cast<kotor_scene3d::Vector*>(pointer(b)),
            lower,upper,margin,f,static_cast<U*>(pointer(output)),
            static_cast<std::int32_t>(flag8),static_cast<kotor_reconstruction::ArrayMutations*>(pointer(list)),
            static_cast<std::int32_t>(flag10),pointer(hit),static_cast<std::int32_t>(doors));
    }
};
}
extern "C" U __fastcall recovered_area_region_005086d0(void* self,void*,
    kotor_scene3d::Vector* from,kotor_scene3d::Vector* to,float lower,float upper,float margin,
    U object_id,void* hit,std::int32_t doors){
    Memory memory;Ports ports;
    U scratch; // The actual portable provider writes the original eighth-word bits first.
    return kotor::server_mainloop::area_region_005086d0(memory,ports,word(self),word(from),word(to),
        float_word(lower),float_word(upper),float_word(margin),object_id,word(hit),static_cast<U>(doors),word(&scratch));
}
