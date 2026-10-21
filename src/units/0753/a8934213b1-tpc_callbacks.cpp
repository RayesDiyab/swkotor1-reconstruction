// lane-m46-startup: Aurora processed-texture callback entries (first-frame TPC path) and the
// three tagged release entries of the CAuroraInterface family.
// Evidence: swkotor.exe (GOG K1) disassembly saved in lane-m46-startup/dis/<addr>.asm.
// Reuse: 0070D510 / 0070EA30 / 0070F330 / 0070F380 algorithms adopted from the task-4
// processed-raster-r1 candidate (source/get_raster.cpp baa4a517.., recovered.cpp, attrib.cpp),
// re-expressed against the M45 stage headers (tpc.hpp / read_callback.hpp / texture_cells.hpp)
// and the canonical 007A39E8 manager cell (manager_cell.h) instead of the donor's private cell.
#include "3c95defd93-tpc.hpp"
#include "c64cb3858e-read_callback.hpp"
#include "0926cb2a6d-manager_cell.h"

using font_callback_read::ProcessedTexture;
using font_callback_read::String;
using tpc_lower::Word;
using tpc_lower::Signed;
using tpc_lower::load;
using tpc_lower::call_slot0;

extern "C" {
// Consumer declaration (service-tail-8 / texture_cells.hpp): nine cdecl stack slots, pointer EAX.
char* __cdecl processed_raster_0070d510(const char*, Signed*, Signed*, char*, Signed*, Signed*, Signed*, float*, char*) noexcept(false);
// 0070F330 CAuroraProcessedTexture::GetProcessedTextureAttrib: thiscall + 8 stack slots, RET 20h (D1 form).
Signed __fastcall processed_attrib_0070f330(ProcessedTexture*, void*, Signed*, Signed*, char*, Signed*, Signed*, Signed*, float*, char*) noexcept(false);
// Release entries (consumer declarations: tpc_release.cpp lines 15/18/19).
void __cdecl borrowed_release_model_0070e0e0(void*);
void __cdecl borrowed_release_visibility_0070eb00(void*);
void __cdecl borrowed_release_model_x_0070eb60(void*);
}

// 0070EA30..0070EA34: `jmp 0070E4F0` (cdecl one pointer, caller cleanup, pointer EAX). D21 naked tail.
namespace font_callback_read {
extern "C" __declspec(naked) ProcessedTexture* __cdecl processed_0070ea30(String*) noexcept(false) {
    __asm { jmp aurora_processed_target_0070e4f0 }
}

// 0070F380..0070F3A0 GetProcessedTextureDataPtr: r=[this+8]; if r && 004064B0(r)>=1 (JL signed)
// -> tail JMP 00712F90 with ECX=[this+8] RELOADED at 0070F394; else EAX=0.
extern "C" __declspec(noinline) char* __fastcall processed_pointer_0070f380(ProcessedTexture* self, void*) {
    void* const initial = load<void*>(self, 8);                                      // 0070F383
    if (!initial) return nullptr;                                                     // 0070F386..0070F388
    if (aurora_get_demands_004064b0(initial, nullptr) < 1) return nullptr;            // 0070F38A..0070F392
    return aurora_processed_pointer_00712f90(load<void*>(self, 8), nullptr);          // 0070F394..0070F398
}
}  // namespace font_callback_read
using font_callback_read::aurora_get_demands_004064b0;
using font_callback_read::aurora_processed_pointer_00712f90;
using font_callback_read::processed_attrib_lower_00712ef0;

// 0070F330..0070F37B GetProcessedTextureAttrib: same guard; then 00712EF0([this+8] reloaded at
// 0070F36C, the eight caller slots in order); RET 20h; EAX = lower result or 0.
extern "C" __declspec(noinline) Signed __fastcall processed_attrib_0070f330(ProcessedTexture* self, void*, Signed* width, Signed* height,
        char* encoding, Signed* compressed_bytes, Signed* image_bytes, Signed* txi_bytes, float* alpha, char* mip_count) noexcept(false) {
    void* const initial = load<void*>(self, 8);                                      // 0070F333
    if (!initial) return 0;                                                           // 0070F336..0070F338
    if (aurora_get_demands_004064b0(initial, nullptr) < 1) return 0;                  // 0070F33A..0070F342
    return processed_attrib_lower_00712ef0(load<void*>(self, 8), nullptr, width, height, encoding,   // 0070F344..0070F36F
                                           compressed_bytes, image_bytes, txi_bytes, alpha, mip_count);
}

// 0070D510..0070D6AE CAuroraInterface::GetProcessedRaster (cdecl, 9 args, SEH frame 0073A878 for the String).
//  0070D52F..0070D534 Ref(filename) via 00406D80 (trivial 16-byte Ref, no destructor).
//  0070D53E..0070D5D4 four CExoResMan::Exists(&ref,type,&code) calls on [007A39E8], reloaded before each,
//     in order 0BBF,07F1,080B,0003; each code zeroed first (0070D550..0070D55C) and zeroed again when
//     Exists returns 0 (the 4th via EDX=0 at 0070D5CC).
//  0070D5D4..0070D61F precedence: TPC wins at code 2, else any other at 2 -> 0; same for 4, then 3;
//     else TPC must be 1.
//  0070D621..0070D64A String(filename) 005E5A90; 0070EA30(&string); ~String 005E5C20 (EH state 0).
//  0070D64F..0070D666 null -> 0.
//  0070D667..0070D698 0070F330(obj, 8 slots) result ignored; return 0070F380(obj).
extern "C" __declspec(noinline) char* __cdecl processed_raster_0070d510(const char* filename, Signed* width, Signed* height,
        char* encoding, Signed* compressed_bytes, Signed* image_bytes, Signed* txi_bytes, float* alpha, char* mip_count) noexcept(false) {
    menu_resources::Ref name;
    name.construct_00406d80(filename);                                                // 0070D534
    Word tpc = 0, compressed = 0, sixteen = 0, raster = 0;                            // 0070D550..0070D55C
    if (!kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&name, 0x0bbf, &tpc)) tpc = 0;               // 0070D560..0070D569
    if (!kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&name, 0x07f1, &compressed)) compressed = 0; // 0070D582..0070D58B
    if (!kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&name, 0x080b, &sixteen)) sixteen = 0;       // 0070D5A4..0070D5AD
    if (!kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(&name, 0x0003, &raster)) raster = 0;         // 0070D5C3..0070D5CC
    if (tpc != 2) {                                                                   // 0070D5D8
        if (compressed == 2 || sixteen == 2 || raster == 2) return nullptr;           // 0070D5DD..0070D5F2
        if (tpc != 4) {                                                               // 0070D5F4
            if (compressed == 4 || sixteen == 4 || raster == 4) return nullptr;       // 0070D5F9..0070D606
            if (tpc != 3) {                                                           // 0070D608
                if (compressed == 3 || sixteen == 3 || raster == 3) return nullptr;   // 0070D60D..0070D61A
                if (tpc != 1) return nullptr;                                         // 0070D61C..0070D61F
            }
        }
    }
    ProcessedTexture* object;
    {
        String resource_name(filename);                                               // 0070D626 005E5A90
        object = font_callback_read::processed_0070ea30(&resource_name);              // 0070D634
    }                                                                                 // 0070D64A 005E5C20
    if (!object) return nullptr;                                                      // 0070D64F..0070D651
    (void)processed_attrib_0070f330(object, nullptr, width, height, encoding,         // 0070D667..0070D691 (EAX ignored)
                                    compressed_bytes, image_bytes, txi_bytes, alpha, mip_count);
    return font_callback_read::processed_pointer_0070f380(object, nullptr);           // 0070D696..0070D698
}

namespace {
// Shared shape of 0070E0E0 / 0070EB00 / 0070EB60 (byte-identical except the tag word):
//  +05 [esi+8] passed to 004064B0 with NO null test; <1 -> return.
//  +12 info=0070FAF0(this); null -> return; [info] word != tag -> return; [info+2] != this -> return.
//  +29 00710100(this); +30 reload [esi+8], 004064B0 >= 1 -> return; else +3D call [vtable+0](this,1).
void release_tagged_helper(void* self, std::uint16_t tag) {
    if (aurora_get_demands_004064b0(load<void*>(self, 8), nullptr) < 1) return;
    const void* info = aurora_helper_info_0070faf0(self, nullptr);
    if (!info) return;
    if (load<std::uint16_t>(info, 0) != tag) return;
    if (load<void*>(info, 2) != self) return;
    aurora_unload_00710100(self, nullptr);
    if (aurora_get_demands_004064b0(load<void*>(self, 8), nullptr) >= 1) return;
    call_slot0(self, 1u);
}
}
// 0070E0E0..0070E126 ReleaseModelInternal (tag 07D2h at 0070E0FD).
extern "C" __declspec(noinline) void __cdecl borrowed_release_model_0070e0e0(void* self) { release_tagged_helper(self, 0x07d2); }
// 0070EB00..0070EB46 ReleaseVisibilityInternal (tag 0BB9h at 0070EB1D).
extern "C" __declspec(noinline) void __cdecl borrowed_release_visibility_0070eb00(void* self) { release_tagged_helper(self, 0x0bb9); }
// 0070EB60..0070EBA6 ReleaseModelXInternal (tag 0BC0h at 0070EB7D).
extern "C" __declspec(noinline) void __cdecl borrowed_release_model_x_0070eb60(void* self) { release_tagged_helper(self, 0x0bc0); }
