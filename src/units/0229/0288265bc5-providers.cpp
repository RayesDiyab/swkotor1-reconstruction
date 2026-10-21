#include "3d9d7a4993-providers.h"
#include "1acaf0e36e-texture_cells.hpp" // E1: installer-owned Aurora raster cells
namespace menu_tables_required { unsigned char __cdecl TextureSetMem_00421d90(int); } // lane-startup-misc (dependency)
#include <cstring>
// lane-m47-core rev2 ADAPTED COPY of the M46 stage unit service-tail-8 (base sha256 6740192a4d1c590b5badc47a00649ebbb4d2add8cdf1feb144e9e59883c6aceb).
// Changes (first-frame StartServices path; the unbound `bindings.dispatch` / `bindings.texture_memory_007a399c`
// were never populated in the M46 stage, so every use below was a null call/read):
//  1. G-PTD-2 (D70): Seed = srand 006FC43A calls the single PTD owner compat_getptd (model46 m46_ptd_join.cpp, the same
//     owner crt_rand_006fc447 uses) instead of dispatching 0x006FEEDF.
//  2. Message::Construct = CSWCMessage::CSWCMessage 0064B510 (lane-m47-core sw_message.cpp: genuine base ctor 004D60F0,
//     real tables 007465A4 / 00751C6C) instead of an invoke(0x004d60f0) plus an integer table word.
//  3. Interface::Construct reads the original .data cell 007A399C directly (owner here: texture_memory_007a399c,
//     PE-initialised 01000000h, sole reference 0070DA4F per a full-image scan; never written).
// rev2: the never-filled `bindings` table is no longer defined at all (its last reader, service-tail-22's address
// dispatcher, is replaced by rev2/adapted/service-tail-22/remaining.cpp), so no null table remains (D76).
extern "C" void* __cdecl compat_getptd();                              // model46-m46-ptd-join (D52/D70 PTD owner)
// lane-m51-hang rev3 (M50 run hang fix): the rev2 Construct forwarded to @kotor_fastcall_0064b510@8, which the M47
// integrator join (/alternatename:@kotor_fastcall_0064b510@8=?Construct@Message@services_tail5@@QAEPAU12@XZ) resolves
// back to Construct itself (sw_message.cpp body was removed at M45 compose in favour of this owner, D78). /O2 emitted
// `xor edx,edx; jmp <self>` = infinite loop at image 0040A1A0 (M50 run hang EIP +2). Construct now carries the genuine
// 0064B510 body itself and calls the base ctor 004D60F0 + table 00751C6C owned by core47-sw-message; the join keeps
// its direction (fastcall spelling -> this body) and is no longer circular.
extern "C" {
struct SwMessageMethods { void* (__fastcall* deleting_destroy)(void*, void*, std::uint32_t); }; // = core47 sw_message.cpp
void* __fastcall kotor_fastcall_004d60f0(void* self, void*);       // core47-sw-message: CSWMessage::CSWMessage 004D60F0
extern const SwMessageMethods swc_message_methods_00751c6c;         // core47-sw-message: table 00751C6C = {00654CA0}
}
namespace services_tail5 {
// 007A399C: dword 01000000h in the PE .data image (tools/rd.py 007a399c); only reader 0070DA4F.
extern const U32 texture_memory_007a399c;
const U32 texture_memory_007a399c = 0x01000000u;
U32 load(U32 address,U32 offset){U32 value;std::memcpy(&value,reinterpret_cast<const void*>(address+offset),4);return value;}
void store(U32 address,U32 offset,U32 value){std::memcpy(reinterpret_cast<void*>(address+offset),&value,4);}
Message* __thiscall Message::Construct(){
 // 0064B510 CSWCMessage::CSWCMessage (thiscall, no stack args, RET):
 //   0064B510 push esi / 0064B511 mov esi,ecx
 //   0064B513 call 004D60F0          (CSWMessage base ctor; its EAX is not used)
 (void)kotor_fastcall_004d60f0(this,nullptr);
 //   0064B518 mov dword ptr [esi],751C6Ch   (CSWCMessage table overwrites the base table 007465A4)
 const U32 table=static_cast<U32>(reinterpret_cast<std::uintptr_t>(&swc_message_methods_00751c6c));
 std::memcpy(words,&table,4);
 //   0064B51E mov eax,esi / pop esi / ret   (EAX = this)
 return this;
}
Interface* __thiscall Interface::Construct(){
 // E1 (lane-texture-epoch) genuine 0070D9C0 body: typed stores of the real target functions into the real cells
 // (texture_cells.hpp / texture_x86.asm), in original order 0070D9C7..0070DA3F.
 aurres_get_callback_007b8fc4=&aurora_get_resource_0070c920;          // 0070D9C7 (cell owner lane-aurres)
 aurres_release_callback_007b8fc8=&aurora_release_resource_0070cbd0;  // 0070D9D1 (cell owner lane-aurres)
 aurora_cell_0078d404=&aurora_get_raster_0070c790;                     // 0070D9DB
 aurora_cell_0078d408=&aurora_read_raster_header_0070cc60;             // 0070D9E5
 aurora_cell_007a46f8=&aurora_release_raster_0070c870;                 // 0070D9EF
 aurora_cell_0078d41c=&aurora_get_layered_raster_0070c6e0;             // 0070D9F9
 aurora_cell_007a46fc=&aurora_release_layered_raster_0070c850;         // 0070DA03
 aurora_cell_0078d40c=&aurora_get_compressed_raster_0070d380;          // 0070DA0D
 aurora_cell_0078d410=&aurora_read_compressed_raster_header_0070cdd0;  // 0070DA17
 aurora_cell_0078d414=&aurora_get_16bit_raster_0070d6b0;               // 0070DA21
 aurora_cell_0078d418=&aurora_get_16bit_raster_header_0070ce80;        // 0070DA2B
 aurora_cell_0078d420=&processed_raster_0070d510;                      // 0070DA35
 aurora_cell_0078d424=&aurora_read_processed_raster_header_0070cd10;   // 0070DA3F
 (void)c2da_resource_constant_word_0063e7f0();                         // 0070DA49 direct call, RET0, EAX ignored
 // 0070DA4E..0070DA59: push [007A399C]; call 00421D90 (00421D90 reads only arg1; the two zeros pushed at
 // 0070D9C1/C3 are never read). E-D3: direct typed call to lane-startup-misc AurTextureSetMem.
 (void)menu_tables_required::TextureSetMem_00421d90(static_cast<int>(texture_memory_007a399c));
 return this;
}
U32 __cdecl Seed(U32 value){
 // 006FC43A: call 006FEEDF (_getptd); mov ecx,[esp+4]; mov [eax+14h],ecx; ret (EAX = PTD).
 const U32 ptd=static_cast<U32>(reinterpret_cast<std::uintptr_t>(compat_getptd()));



 store(ptd,0x14,value);

 return ptd;
}
}
