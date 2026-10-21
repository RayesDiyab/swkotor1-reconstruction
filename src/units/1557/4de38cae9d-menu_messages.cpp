#include "50a5e32a47-menu8_imports.h"
#include <cstring>

namespace {
using U=std::uint32_t;
U word(const void* p,U offset) {
    U value;std::memcpy(&value,static_cast<const unsigned char*>(p)+offset,4);return value;
}
// 006776D5..00677702: preserve the extended comparison after the single store.
__declspec(noinline) bool magnitude_extended(const void* position,U* rounded) {
    const U threshold=0x3727c5acu;
    std::uint16_t status;
    __asm {
        mov ecx,position
        fld dword ptr [ecx+8]
        fld dword ptr [ecx+4]
        fld dword ptr [ecx]
        fld st(0)
        fmul st(0),st(1)
        fld st(2)
        fmul st(0),st(3)
        faddp st(1),st(0)
        fld st(3)
        fmul st(0),st(4)
        faddp st(1),st(0)
        mov edx,rounded
        fst dword ptr [edx]
        fstp st(3)
        fstp st(0)
        fstp st(0)
        fcomp dword ptr threshold
        fnstsw ax
        mov status,ax
    }
    return (status&0x0100u)==0;
}
// 0067776C..0067777B: compare the separately reloaded rounded single.
__declspec(noinline) bool magnitude_single(U rounded) {
    const U threshold=0x3727c5acu;
    std::uint16_t status;
    __asm {
        fld dword ptr rounded
        fcomp dword ptr threshold
        fnstsw ax
        mov status,ax
    }
    return (status&0x0100u)==0;
}
__declspec(noinline) U raised_z(const void* position) {
    const U amount=0x3e99999au;
    U bits;
    __asm {
        mov ecx,position
        fld dword ptr [ecx+8]
        fadd dword ptr amount
        fstp dword ptr bits
    }
    return bits;
}
}

extern "C" U __fastcall kotor_fastcall_006776a0(void* self,void*,U action,U byte_a,
        U byte_b,U byte_c,U flag,const void* position,U object_id) {
    U size=4;
    kotor_fastcall_004d68a0(self,nullptr,4,0xffffffffu,1);
    U flags=object_id!=0x7f000000u?1u:0u;
    U rounded;
    if(magnitude_extended(position,&rounded))flags+=2;
    if(flag!=0)flags+=4;
    kotor_fastcall_004d6340(self,nullptr,action,32);
    kotor_fastcall_004d62c0(self,nullptr,byte_a,8);
    kotor_fastcall_004d62c0(self,nullptr,byte_b,8);
    kotor_fastcall_004d62c0(self,nullptr,byte_c,8);
    kotor_fastcall_004d62c0(self,nullptr,flags,8);
    if(object_id!=0x7f000000u)kotor_fastcall_004d68e0(self,nullptr,object_id);
    if(magnitude_single(rounded)) {
        kotor_fastcall_004d6920(self,nullptr,word(position,0),0x3f800000u,32);
        kotor_fastcall_004d6920(self,nullptr,word(position,4),0x3f800000u,32);
        kotor_fastcall_004d6920(self,nullptr,raised_z(position),0x3f800000u,32);
    }
    void* bytes;
    if(kotor_fastcall_004d6440(self,nullptr,&bytes,&size)==0)return 0;
    return recovered_client_send_00677410(self,nullptr,6,0x12,bytes,size);
}

extern "C" U __fastcall kotor_fastcall_00677bd0(void* self,void*,U action,U byte_a,
        U byte_b,U object_id,const void* position) {
    U size=0x15;
    kotor_fastcall_004d68a0(self,nullptr,0x15,0xffffffffu,1);
    kotor_fastcall_004d68e0(self,nullptr,action);
    kotor_fastcall_004d62c0(self,nullptr,byte_a,8);
    if(static_cast<std::uint8_t>(byte_b)!=0xff) {
        kotor_fastcall_004d68e0(self,nullptr,1);
        kotor_fastcall_004d62c0(self,nullptr,byte_b,8);
    } else kotor_fastcall_004d68e0(self,nullptr,0);
    if(object_id!=0x7f000000u) {
        kotor_fastcall_004d68e0(self,nullptr,1);
        kotor_fastcall_004d68e0(self,nullptr,object_id);
    } else kotor_fastcall_004d68e0(self,nullptr,0);
    const U zero[3]={0,0,0};
    if(aurora_vector_equal_004aa980(position,zero)==0) {
        kotor_fastcall_004d68e0(self,nullptr,1);
        kotor_fastcall_004d6920(self,nullptr,word(position,0),0x3f800000u,32);
        kotor_fastcall_004d6920(self,nullptr,word(position,4),0x3f800000u,32);
        kotor_fastcall_004d6920(self,nullptr,word(position,8),0x3f800000u,32);
    } else kotor_fastcall_004d68e0(self,nullptr,0);
    void* bytes;
    if(kotor_fastcall_004d6440(self,nullptr,&bytes,&size)==0)return 0;
    return recovered_client_send_00677410(self,nullptr,6,9,bytes,size);
}

extern "C" U __fastcall kotor_fastcall_00677de0(void* self,void*,U first,U second) {
    U size=8;
    kotor_fastcall_004d68a0(self,nullptr,8,0xffffffffu,1);
    kotor_fastcall_004d68e0(self,nullptr,first);
    kotor_fastcall_004d68e0(self,nullptr,second);
    void* bytes;
    if(kotor_fastcall_004d6440(self,nullptr,&bytes,&size)==0)return 0;
    return recovered_client_send_00677410(self,nullptr,6,0xc,bytes,size);
}
