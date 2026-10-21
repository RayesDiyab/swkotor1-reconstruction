// Complete original mode builders; sole table/literal/lifecycle owner is object_projectile foundation.
#include "120e70883b-object_projectile_internal.h"
#include "8c39a2f5e4-object_projectile_providers.h"
#include "ead2dcbdd7-m15e.h"
#include "f58c269829-scene3d.h"
#include <cstring>
using U=std::uint32_t;
using Vec=kotor_scene3d::Vector;
extern "C" Vec* __cdecl normalize_004ab180(Vec*,const Vec*);
namespace menu_tables_m41 { namespace cells { extern const unsigned int invalid_object_00746644; }}
extern "C" {
extern const unsigned char object_projectile_literal_007455dc[8];
extern const unsigned char object_projectile_literal_00747504[8];
extern const unsigned char object_projectile_literal_0074666c[8];
}
static_assert(sizeof(U)==4 && sizeof(void*)==4 && sizeof(Vec)==12,"native x86 contracts");
namespace {
U ld(const void* p,U o){return *reinterpret_cast<const volatile U*>(static_cast<const unsigned char*>(p)+o);}
void st(void* p,U o,U v){*reinterpret_cast<volatile U*>(static_cast<unsigned char*>(p)+o)=v;}
void byte(void* p,U o,unsigned char v){*(static_cast<volatile unsigned char*>(p)+o)=v;}
void* ptr(const void* p,U o){return reinterpret_cast<void*>(ld(p,o));}
void read3(const void* p,U o,U* v){v[0]=ld(p,o);v[1]=ld(p,o+4);v[2]=ld(p,o+8);}
void write3(void* p,U o,const U* v){st(p,o,v[0]);st(p,o+4,v[1]);st(p,o+8,v[2]);}
void* app(){return ptr(kotor_menu_window_cells::manager_007a39fc,4);}
void append(void* self,void* p){resource_append_005e9dc0(static_cast<kotor_list::Internal*>(ptr(self,0x194)),nullptr,p);}
void* packet(){
    void* p=live_operator_new(0x34);
    if(p!=nullptr){st(p,0,0);st(p,4,0);st(p,8,0);st(p,12,0);st(p,16,0);st(p,20,0);}
    // Original allocation-null branches still proceed to the caller's unconditional descriptor write.
    return p;
}
void target_vector(void* self,U* v){
    const U id=ld(self,0x124);
    if(id==menu_tables_m41::cells::invalid_object_00746644){read3(self,0x114,v);return;}
    if(kotor_fastcall_005ed580(app(),nullptr,id)!=nullptr){v[0]=0;v[1]=0;v[2]=0;return;}
    read3(self,0x114,v);
}
void raised_packet_destination(void* p,const U* destination,const void* constant,U second){
    // Keep ST0 live across the original destination X/Y stores; no temporary FP spill.
    __asm {
        mov eax,p
        mov ecx,constant
        fld dword ptr [eax+8]
        fadd dword ptr [ecx]
        mov edx,destination
        mov ecx,[edx]
        mov [eax+0ch],ecx
        mov ecx,[edx+4]
        mov [eax+10h],ecx
        fstp dword ptr [eax+8]
        cmp second,0
        je destination_z
        mov byte ptr [eax+2dh],11
    destination_z:
        mov ecx,[edx+8]
        mov [eax+14h],ecx
    }
}
void floating_parameter_copy(void* self,void* p){
    __asm {
        mov ecx,self
        mov eax,p
        fld dword ptr [ecx+1c0h]
        fstp dword ptr [eax+28h]
    }
}
void ordinary_mode(void* self,unsigned char mode,bool zero_end){
    U v[3];target_vector(self,v);
    void* p=packet();write3(p,0,v);
    st(p,12,zero_end?0:v[0]);st(p,16,zero_end?0:v[1]);
    byte(p,0x2d,mode);st(p,20,zero_end?0:v[2]);byte(p,0x2c,0);
    st(p,0x20,ld(self,0x190));st(p,0x24,0);st(p,0x18,ld(self,0x124));st(p,0x1c,0);st(p,0x28,0xbf800000);
    append(self,p);
}
void fixed_target_packet(void* self,void* p,const U* v,bool floating_parameter){
    byte(p,0x2d,1);st(p,0x18,ld(self,0x124));write3(p,0,v);write3(p,12,v);byte(p,0x2c,0);
    st(p,0x20,ld(self,0x190));st(p,0x24,0);st(p,0x1c,1);
    if(floating_parameter)floating_parameter_copy(self,p);else st(p,0x28,0);
    append(self,p);
}
void extra_position_packet(void* self){
    void* p=packet();byte(p,0x2d,1);st(p,0x18,0x7f000000);
    U v[3];read3(self,0x1b0,v);write3(p,0,v);read3(self,0x1b0,v);write3(p,12,v);
    byte(p,0x2c,0);st(p,0x20,ld(self,0x1bc));st(p,0x24,0);st(p,0x1c,1);st(p,0x28,0);append(self,p);
}
void side_difference(void* self,U* difference){
    __asm {
        mov ecx,self
        mov edx,difference
        fld dword ptr [ecx+114h]
        fsub dword ptr [ecx+24h]
        fstp dword ptr [edx]
        fld dword ptr [ecx+118h]
        fsub dword ptr [ecx+28h]
        fstp dword ptr [edx+4]
    }
}
U negate32(U value){U out;__asm { fld dword ptr value } __asm { fchs } __asm { fstp dword ptr out } return out;}
bool normalize_side(const U* side,const void* epsilon){
    U result;
    __asm {
        mov ecx,side
        fld dword ptr [ecx+4]
        fmul dword ptr [ecx+4]
        fld dword ptr [ecx]
        fmul dword ptr [ecx]
        faddp st(1),st
        mov ecx,epsilon
        fcomp dword ptr [ecx]
        fnstsw ax
        test ah,41h
        jp yes
        mov result,0
        jmp done
    yes:
        mov result,1
    done:
    }
    return result!=0;
}
void double_vector(const Vec* v,U* output){
    __asm {
        mov ecx,v
        mov edx,output
        fld dword ptr [ecx]
        fadd st,st
        fstp dword ptr [edx]
        fld dword ptr [ecx+4]
        fadd st,st
        fstp dword ptr [edx+4]
        fld dword ptr [ecx+8]
        fadd st,st
        fstp dword ptr [edx+8]
    }
}
void add_vector(const U* a,const void* b,U* output){
    __asm {
        mov ecx,a
        mov edx,b
        mov eax,output
        fld dword ptr [ecx]
        fadd dword ptr [edx]
        fstp dword ptr [eax]
        fld dword ptr [ecx+4]
        fadd dword ptr [edx+4]
        fstp dword ptr [eax+4]
        fld dword ptr [ecx+8]
        fadd dword ptr [edx+8]
        fstp dword ptr [eax+8]
    }
}
void set_position(void* self,const U* position){
    const auto* table=*static_cast<const KotorM15ObjectMethods_007516c8* const*>(self);
    table->s08c_set_position(self,nullptr,reinterpret_cast<const float*>(position));
}
}

extern "C" void __fastcall object_projectile_006d4520(void* self,void*){
    auto* list=static_cast<kotor_list::Internal*>(ptr(self,0x194));
    while(ld(list,0)!=0){void* p=base_ini_reconstruction::remove_head_005e9c80(list,nullptr);if(p!=nullptr)live_free(p);list=static_cast<kotor_list::Internal*>(ptr(self,0x194));}
}
extern "C" void __fastcall object_projectile_006d4550(void* self,void*){ordinary_mode(self,1,false);}
extern "C" void __fastcall object_projectile_006d4640(void* self,void*){ordinary_mode(self,2,false);}
extern "C" void __fastcall object_projectile_006d4930(void* self,void*){ordinary_mode(self,4,false);}
extern "C" void __fastcall object_projectile_006d4a20(void* self,void*){ordinary_mode(self,9,false);}
extern "C" void __fastcall object_projectile_006d4b10(void* self,void*){ordinary_mode(self,5,false);}
extern "C" void __fastcall object_projectile_006d5500(void* self,void*){ordinary_mode(self,12,false);}
extern "C" void __fastcall object_projectile_006d4fe0(void* self,void*){ordinary_mode(self,7,true);}

extern "C" void __fastcall object_projectile_006d4730(void* self,void*){
    U target[3];target_vector(self,target);
    void* p=packet();byte(p,0x2d,1);
    U start[3];read3(self,0x24,start);write3(p,0,start);
    raised_packet_destination(p,target,object_projectile_literal_007455dc,0);
    byte(p,0x2c,0);st(p,0x20,ld(self,0x190)>>1);st(p,0x24,0);
    st(p,0x18,menu_tables_m41::cells::invalid_object_00746644);st(p,0x1c,0);st(p,0x28,0xbf800000);append(self,p);
    p=packet();write3(p,0,target);
    raised_packet_destination(p,target,object_projectile_literal_007455dc,1);byte(p,0x2c,0);
    st(p,0x20,0);st(p,0x24,0);st(p,0x18,ld(self,0x124));st(p,0x1c,0);st(p,0x28,0xbf800000);append(self,p);
    p=packet();write3(p,0,target);write3(p,12,target);byte(p,0x2d,1);byte(p,0x2c,0);
    const U duration=ld(self,0x190);st(p,0x20,duration-(duration>>1));st(p,0x24,0);st(p,0x18,ld(self,0x124));st(p,0x1c,0);st(p,0x28,0xbf800000);append(self,p);
}
extern "C" void __fastcall object_projectile_006d50f0(void* self,void*){
    U v[3];target_vector(self,v);void* p=packet();fixed_target_packet(self,p,v,false);extra_position_packet(self);
}
extern "C" void __fastcall object_projectile_006d5250(void* self,void*){
    U v[3];target_vector(self,v);void* p=packet();fixed_target_packet(self,p,v,false);
    p=packet();byte(p,0x2d,1);st(p,0x18,ld(self,0x1c4));write3(p,0,v);write3(p,12,v);byte(p,0x2c,0);
    st(p,0x20,ld(self,0x190));st(p,0x24,0);st(p,0x1c,1);st(p,0x28,0);append(self,p);
}
extern "C" void __fastcall object_projectile_006d53a0(void* self,void*){
    U v[3];target_vector(self,v);void* p=packet();fixed_target_packet(self,p,v,true);extra_position_packet(self);
}
extern "C" void __fastcall object_projectile_006d55f0(void* self,void*){
    U target[3];target_vector(self,target);
    void* p=packet();write3(p,0,target);byte(p,0x2d,2);
    U end[3];read3(self,0x114,end);write3(p,12,end);byte(p,0x2c,0);
    st(p,0x20,ld(self,0x190));const U duration=ld(self,0x190);st(p,0x24,((duration*3u)<<1)/10u);
    st(p,0x18,ld(self,0x124));st(p,0x1c,0);st(p,0x28,0xbf800000);append(self,p);
    p=packet();write3(p,0,target);byte(p,0x2d,4);read3(self,0x114,end);write3(p,12,end);byte(p,0x2c,3);
    const U later_duration=ld(self,0x190);st(p,0x20,((later_duration*3u)<<1)/10u);st(p,0x24,0);
    st(p,0x18,ld(self,0x124));st(p,0x1c,9);st(p,0x28,0xbf800000);append(self,p);
}
extern "C" void __fastcall object_projectile_006d4c00(void* self,void*){
    U difference[2];side_difference(self,difference);
    const unsigned char sign=static_cast<unsigned char>(crt_rand_006fc447()%2);byte(self,0x1a0,sign);
    U side[3];side[2]=0;
    if(sign!=0){side[0]=negate32(difference[1]);side[1]=difference[0];}
    else{side[0]=difference[1];side[1]=negate32(difference[0]);}
    if(!normalize_side(side,object_projectile_literal_00747504)){side[0]=0;side[1]=0x3f800000;side[2]=0;}
    Vec input,normalized;std::memcpy(&input,side,12);
    Vec* const returned=normalize_004ab180(&normalized,&input);
    double_vector(returned,side);
    const U invalid=menu_tables_m41::cells::invalid_object_00746644;const U target=ld(self,0x124);
    if(target==invalid){U absolute[3];add_vector(side,static_cast<unsigned char*>(self)+0x114,absolute);for(U i=0;i<3;++i)side[i]=absolute[i];side[2]=ld(self,0x2c);}
    const U remaining=ld(self,0x190)-2500u;
    if(remaining!=0){
        void* p=packet();byte(p,0x2d,10);write3(p,0,side);
        U end[3];if(ld(self,0x124)==menu_tables_m41::cells::invalid_object_00746644)read3(self,0x114,end);else{end[0]=0;end[1]=0;end[2]=0;}
        write3(p,12,end);byte(p,0x2c,0);st(p,0x20,ld(self,0x190)-2500u);st(p,0x24,0);
        st(p,0x18,ld(self,0x124));st(p,0x1c,0);st(p,0x28,0x40000000);append(self,p);
    }else if(target==invalid){set_position(self,side);}
    else{
        void* const array=kotor_fastcall_005ed570(app(),nullptr);void* object;
        const unsigned char type=kotor_fastcall_004d8230(array,nullptr,ld(self,0x124),&object);
        if(type==static_cast<unsigned char>(object_projectile_literal_0074666c[0])){
            U absolute[3];add_vector(side,static_cast<unsigned char*>(object)+0x24,absolute);
            absolute[2]=ld(self,0x2c); // original post-PUSH [ESP+2C] overwrites pre-PUSH vectorZ.
            set_position(self,absolute);
        }
    }
    void* p=packet();byte(p,0x2d,6);
    U v[3];if(ld(self,0x124)==menu_tables_m41::cells::invalid_object_00746644){
        read3(self,0x114,v);write3(p,0,v);read3(self,0x114,v);write3(p,12,v);
    }else{v[0]=0;v[1]=0;v[2]=0;write3(p,0,v);write3(p,12,v);}
    byte(p,0x2c,0);st(p,0x20,2500);st(p,0x24,0);st(p,0x18,ld(self,0x124));st(p,0x1c,0);st(p,0x28,0xbf800000);append(self,p);
}
