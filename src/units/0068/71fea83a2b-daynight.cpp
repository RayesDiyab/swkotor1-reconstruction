// Complete 00606490..00606C50 (1984 bytes), CSWCArea::UpdateDayNightCycle.
// State/callback control is C++; bounded numeric regions retain the original live x87 stack.
#include "b80d065e6d-daynight.h"
using U = unsigned;
static_assert(sizeof(U)==4 && sizeof(void*)==4, "original x86 layout");
namespace kotor_menu_window_cells { extern void* volatile manager_007a39fc; }
extern "C" {
extern float beam_alpha_0078e6a0;
extern float kotor_cell_0078e408;
void* __fastcall kotor_fastcall_005ed530(void*,void*);
void* __fastcall client_get_player_creature_005ed540(void*,void*);
void* __fastcall object_vfx_00614aa0(void*,void*,U);
int __cdecl format_006fadb0(char*,const char*,...);
unsigned char __cdecl recovered_aur_control_part_0044b900(const char*);
void __cdecl recovered_set_clear_color_00401690(const char*);
void __fastcall recovered_client_area_update_sound_objects_time_of_day_00605b10(void*,void*);
void __fastcall recovered_client_ambient_sound_update_time_of_day_0068d690(void*,void*);
}
namespace {
const float kpercent=.01f, kbeam=.05f, khalf=.5f, kone=1.f;
const char ambient_format[]="sun gidy_sun_amb color %f %f %f";
const char diffuse_format[]="sun gidy_sun_diff color %f %f %f";
const char fog_format[]="%f %f %f";
U read(void* p,U off) { return *reinterpret_cast<volatile U*>(static_cast<unsigned char*>(p)+off); }
void write(void* p,U off,U value) { *reinterpret_cast<volatile U*>(static_cast<unsigned char*>(p)+off)=value; }
void* ptr(void* p,U off) { return reinterpret_cast<void*>(read(p,off)); }
void* client() { return ptr(kotor_menu_window_cells::manager_007a39fc,4); }
U brightness(void* area) { return *reinterpret_cast<volatile unsigned char*>(static_cast<unsigned char*>(area)+0xb4); }
// Scratch offsets name the original DWORD stores, not an invented object layout.
void copy3(U* s,U to,void* area,U from) {
    s[to/4]=read(area,from); s[to/4+1]=read(area,from+4); s[to/4+2]=read(area,from+8);
}
unsigned short compare32(U left,U right) {
    unsigned short status;
    __asm {
        fld dword ptr left
        fcomp dword ptr right
        fnstsw ax
        mov status, ax
    }
    return status;
}
void full_brightness(U value) {
    __asm {
        fild dword ptr value
        fmul dword ptr kpercent
        fstp dword ptr kotor_cell_0078e408
    }
}
void dawn_interpolation(void* area,U* s) {
    __asm {
        mov esi, area
        mov edi, s
        fld dword ptr [edi + 0x18]
        fdiv dword ptr [edi + 0x1c]
        fld dword ptr kbeam
        fmul st(1)
        fstp dword ptr beam_alpha_0078e6a0
        fld dword ptr [esi + 0x14c]
        fsub dword ptr [esi + 0x128]
        fld dword ptr [esi + 0x150]
        fsub dword ptr [esi + 0x12c]
        fld dword ptr [esi + 0x154]
        fsub dword ptr [esi + 0x130]
        fstp dword ptr [edi + 0x60]
        fld dword ptr [esi + 0x158]
        fsub dword ptr [esi + 0x134]
        fstp dword ptr [edi + 0x38]
        fld dword ptr [esi + 0x15c]
        fsub dword ptr [esi + 0x138]
        fstp dword ptr [edi + 0x3c]
        fld dword ptr [esi + 0x160]
        fsub dword ptr [esi + 0x13c]
        fstp dword ptr [edi + 0x40]
        fld dword ptr [esi + 0x164]
        fsub dword ptr [esi + 0x140]
        fstp dword ptr [edi + 0xc]
        fld dword ptr [esi + 0x168]
        fsub dword ptr [esi + 0x144]
        fstp dword ptr [edi + 0x10]
        fld dword ptr [esi + 0x16c]
        fsub dword ptr [esi + 0x148]
        fstp dword ptr [edi + 0x14]
        fxch st(1)
        fmul st(2)
        fstp dword ptr [edi + 0x64]
        fmul st(1)
        fld dword ptr [edi + 0x60]
        fmul st(2)
        fld dword ptr [edi + 0x64]
        fadd dword ptr [esi + 0x128]
        fstp dword ptr [edi + 0x58]
        mov edx, dword ptr [edi + 0x58]
        fxch st(1)
        mov dword ptr [edi + 0x2c], edx
        fadd dword ptr [esi + 0x12c]
        fstp dword ptr [edi + 0x5c]
        mov eax, dword ptr [edi + 0x5c]
        fadd dword ptr [esi + 0x130]
        mov dword ptr [edi + 0x30], eax
        fstp dword ptr [edi + 0x60]
        fld dword ptr [edi + 0x38]
        mov ecx, dword ptr [edi + 0x60]
        fmul st(1)
        mov dword ptr [edi + 0x34], ecx
        fld dword ptr [edi + 0x3c]
        fmul st(2)
        fld dword ptr [edi + 0x40]
        fmul st(3)
        fstp dword ptr [edi + 0x6c]
        fxch st(1)
        fadd dword ptr [esi + 0x134]
        fstp dword ptr [edi + 0x38]
        mov edx, dword ptr [edi + 0x38]
        mov dword ptr [edi + 0x4c], edx
        fadd dword ptr [esi + 0x138]
        fstp dword ptr [edi + 0x3c]
        fld dword ptr [edi + 0x6c]
        fadd dword ptr [esi + 0x13c]
        fstp dword ptr [edi + 0x40]
        fld dword ptr [edi + 0xc]
        mov eax, dword ptr [edi + 0x3c]
        fmul st(1)
        mov ecx, dword ptr [edi + 0x40]
        fld dword ptr [edi + 0x10]
        mov dword ptr [edi + 0x50], eax
        fmul st(2)
        mov dword ptr [edi + 0x54], ecx
        fld dword ptr [edi + 0x14]
        fmul st(3)
        fstp dword ptr [edi + 0x6c]
        fxch st(1)
        fadd dword ptr [esi + 0x140]
        fstp dword ptr [edi + 0xc]
        mov edx, dword ptr [edi + 0xc]
        mov dword ptr [edi + 0x20], edx
        fadd dword ptr [esi + 0x144]
        fstp dword ptr [edi + 0x10]
        fld dword ptr [edi + 0x6c]
        mov eax, dword ptr [edi + 0x10]
        fadd dword ptr [esi + 0x148]
        mov dword ptr [edi + 0x24], eax
        fstp dword ptr [edi + 0x14]
        fld dword ptr [esi + 0x90]
        mov ecx, dword ptr [edi + 0x14]
        fsub dword ptr [esi + 0x74]
        mov dword ptr [edi + 0x28], ecx
        fmul st(1)
        fadd dword ptr [esi + 0x74]
        fstp dword ptr [edi + 0x48]
        fld dword ptr [esi + 0x94]
        fsub dword ptr [esi + 0x78]
        fmul st(1)
        fadd dword ptr [esi + 0x78]
        fstp dword ptr [edi+0x44]
        fstp st(0)
    }
}
void dusk_interpolation(void* area,U* s) {
    __asm {
        mov esi, area
        mov edi, s
        fld dword ptr [edi + 0x18]
        fdiv dword ptr [edi + 0x1c]
        fld dword ptr kbeam
        fmul st(1)
        fsubr dword ptr kbeam
        fstp dword ptr beam_alpha_0078e6a0
        fld dword ptr [esi + 0x128]
        fsub dword ptr [esi + 0x14c]
        fld dword ptr [esi + 0x12c]
        fsub dword ptr [esi + 0x150]
        fld dword ptr [esi + 0x130]
        fsub dword ptr [esi + 0x154]
        fstp dword ptr [edi + 0x6c]
        fld dword ptr [esi + 0x134]
        fsub dword ptr [esi + 0x158]
        fstp dword ptr [edi + 0x38]
        fld dword ptr [esi + 0x138]
        fsub dword ptr [esi + 0x15c]
        fstp dword ptr [edi + 0x3c]
        fld dword ptr [esi + 0x13c]
        fsub dword ptr [esi + 0x160]
        fstp dword ptr [edi + 0x40]
        fld dword ptr [esi + 0x140]
        fsub dword ptr [esi + 0x164]
        fstp dword ptr [edi + 0x58]
        fld dword ptr [esi + 0x144]
        fsub dword ptr [esi + 0x168]
        fstp dword ptr [edi + 0x5c]
        fld dword ptr [esi + 0x148]
        fsub dword ptr [esi + 0x16c]
        fstp dword ptr [edi + 0x60]
        fxch st(1)
        fmul st(2)
        fstp dword ptr [edi + 0xc]
        fmul st(1)
        fld dword ptr [edi + 0x6c]
        fmul st(2)
        fld dword ptr [edi + 0xc]
        fadd dword ptr [esi + 0x14c]
        fstp dword ptr [edi + 0xc]
        mov eax, dword ptr [edi + 0xc]
        fxch st(1)
        mov dword ptr [edi + 0x2c], eax
        fadd dword ptr [esi + 0x150]
        fstp dword ptr [edi + 0x10]
        mov ecx, dword ptr [edi + 0x10]
        fadd dword ptr [esi + 0x154]
        mov dword ptr [edi + 0x30], ecx
        fstp dword ptr [edi + 0x14]
        fld dword ptr [edi + 0x38]
        mov edx, dword ptr [edi + 0x14]
        fmul st(1)
        mov dword ptr [edi + 0x34], edx
        fld dword ptr [edi + 0x3c]
        fmul st(2)
        fld dword ptr [edi + 0x40]
        fmul st(3)
        fstp dword ptr [edi + 0x6c]
        fxch st(1)
        fadd dword ptr [esi + 0x158]
        fstp dword ptr [edi + 0xc]
        mov eax, dword ptr [edi + 0xc]
        fadd dword ptr [esi + 0x15c]
        fstp dword ptr [edi + 0x10]
        fld dword ptr [edi + 0x6c]
        fadd dword ptr [esi + 0x160]
        fstp dword ptr [edi + 0x14]
        fld dword ptr [edi + 0x58]
        mov ecx, dword ptr [edi + 0x10]
        fmul st(1)
        mov edx, dword ptr [edi + 0x14]
        fld dword ptr [edi + 0x5c]
        mov dword ptr [edi + 0x4c], eax
        fmul st(2)
        mov dword ptr [edi + 0x50], ecx
        fld dword ptr [edi + 0x60]
        mov dword ptr [edi + 0x54], edx
        fmul st(3)
        fstp dword ptr [edi + 0x6c]
        fxch st(1)
        fadd dword ptr [esi + 0x164]
        fstp dword ptr [edi + 0xc]
        mov eax, dword ptr [edi + 0xc]
        mov dword ptr [edi + 0x20], eax
        fadd dword ptr [esi + 0x168]
        fstp dword ptr [edi + 0x10]
        fld dword ptr [edi + 0x6c]
        mov ecx, dword ptr [edi + 0x10]
        fadd dword ptr [esi + 0x16c]
        mov dword ptr [edi + 0x24], ecx
        fstp dword ptr [edi + 0x14]
        fld dword ptr [esi + 0x74]
        mov edx, dword ptr [edi + 0x14]
        fsub dword ptr [esi + 0x90]
        mov dword ptr [edi + 0x28], edx
        fmul st(1)
        fadd dword ptr [esi + 0x90]
        fstp dword ptr [edi + 0x48]
        fld dword ptr [esi + 0x78]
        fsub dword ptr [esi + 0x94]
        fmul st(1)
        fadd dword ptr [esi + 0x94]
        fstp dword ptr [edi + 0x44]
        fstp st(0)
    }
}
void half_duration(U* s) {
    __asm {
        mov eax, s
        fld dword ptr [eax+0x1c]
        fmul dword ptr khalf
        fstp dword ptr [eax+8]
    }
}
void fade_down(U* s,U value) {
    __asm {
        mov eax, s
        fild dword ptr value
        fmul dword ptr kpercent
        fld dword ptr [eax+0x18]
        fdiv dword ptr [eax+8]
        fld st(1)
        fchs
        fmulp st(1), st(0)
        fadd st(0), st(1)
        fstp dword ptr kotor_cell_0078e408
        fstp st(0)
    }
}
void fade_up(U* s,U value) {
    __asm {
        mov eax, s
        fld dword ptr [eax+0x18]
        fdiv dword ptr [eax+8]
        fsub dword ptr kone
        fild dword ptr value
        fmul dword ptr kpercent
        fmulp st(1), st(0)
        fstp dword ptr kotor_cell_0078e408
    }
}
// FLD32/FSTP64 promotes the original raw floats even when MXCSR.DAZ is set.
void format3(char* buffer,const char* format,U* xyz) {
    __asm {
        mov esi, xyz
        fld dword ptr [esi+8]
        sub esp, 24
        fstp qword ptr [esp+16]
        fld dword ptr [esi+4]
        fstp qword ptr [esp+8]
        fld dword ptr [esi]
        fstp qword ptr [esp]
        push format
        push buffer
        call format_006fadb0
        add esp, 32
    }
}
void refresh_light(void* area) {
    if(read(area,0x188)!=0) {
        void* player=client_get_player_creature_005ed540(client(),nullptr);
        if(player!=nullptr) object_vfx_00614aa0(player,nullptr,1);
    }
}
template<class F> F slot(void* self,U offset) {
    return reinterpret_cast<F>(*reinterpret_cast<void**>(static_cast<unsigned char*>(ptr(self,0))+offset));
}
}
extern "C" void __fastcall kotor_fastcall_00606490(void* area,void*,float) {
    U s[0x70/4];
    s[0x2c/4]=s[0x30/4]=s[0x34/4]=0;
    s[0x4c/4]=s[0x50/4]=s[0x54/4]=0;
    s[0x20/4]=s[0x24/4]=s[0x28/4]=0;
    if(read(area,0x1b4)==0 && read(area,0x1b8)==0) return;
    void* module=kotor_fastcall_005ed530(client(),nullptr);
    if(module==nullptr) return;
    s[0x1c/4]=read(module,0xbc); s[0x18/4]=read(module,0xb8);
    if(read(area,0x1bc)==0) {
        s[8/4]=read(area,0x1b4);
        write(area,0x1bc,1);
    }
    if(read(area,0x1b4)!=0) {
        if((compare32(s[0x18/4],s[0x1c/4])&0x0100)==0) {
            *reinterpret_cast<volatile U*>(&beam_alpha_0078e6a0)=0x3d4ccccd;
            full_brightness(brightness(area));
            copy3(s,0x2c,area,0x14c); copy3(s,0x4c,area,0x158); copy3(s,0x20,area,0x164);
            s[0x44/4]=read(area,0x94); s[0x48/4]=read(area,0x90);
            write(area,0x1bc,0); write(area,0x1b4,0);
            refresh_light(area); write(area,0xa4,0);
        } else dawn_interpolation(area,s);
    } else if(read(area,0x1b8)!=0) {
        if((compare32(s[0x18/4],s[0x1c/4])&0x0100)==0) {
            *reinterpret_cast<volatile U*>(&beam_alpha_0078e6a0)=0;
            full_brightness(brightness(area));
            copy3(s,0x2c,area,0x128); copy3(s,0x4c,area,0x134); copy3(s,0x20,area,0x140);
            s[0x48/4]=read(area,0x60); s[0x44/4]=read(area,0x64);
            write(area,0x1bc,0); write(area,0x1b8,0);
            refresh_light(area); write(area,0xa4,1);
        } else dusk_interpolation(area,s);
    }
    half_duration(s);
    if((compare32(s[0x18/4],s[8/4])&0x0100)==0 && read(area,0x1c0)!=0) {
        if(read(area,0x1b8)!=0) write(area,0xa4,1);
        else if(read(area,0x1b4)!=0) write(area,0xa4,0);
        refresh_light(area); write(area,0x1c0,0);
    }
    // TEST AH,5 / JP skips both unordered and >= cases; only ordered-less fades.
    const U status=compare32(s[0x18/4],s[0x1c/4]);
    if((status&0x0500)==0x0100) {
        if(read(area,0x1c0)!=0) fade_down(s,brightness(area));
        else fade_up(s,brightness(area));
    }
    char buffer[128];
    format3(buffer,ambient_format,s+0x2c/4); recovered_aur_control_part_0044b900(buffer);
    format3(buffer,diffuse_format,s+0x4c/4); recovered_aur_control_part_0044b900(buffer);
    write(area,0x60,s[0x48/4]); write(area,0x64,s[0x44/4]);
    void* scene=ptr(area,0x184);
    // Original pushes raw fog DWORDs: the selected Scene callbacks also store raw DWORDs.
    slot<U(__fastcall*)(void*,void*,U,U)>(scene,0x44)(scene,nullptr,s[0x48/4],s[0x44/4]);
    scene=ptr(area,0x184);
    slot<U(__fastcall*)(void*,void*,U)>(scene,8)(scene,nullptr,reinterpret_cast<U>(s+0x20/4));
    format3(buffer,fog_format,s+0x20/4); recovered_set_clear_color_00401690(buffer);
    recovered_client_area_update_sound_objects_time_of_day_00605b10(area,nullptr);
    recovered_client_ambient_sound_update_time_of_day_0068d690(ptr(area,0x1dc),nullptr);
}
