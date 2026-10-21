// lane-rules: CSWRules vtable 00748BB4, scalar deleting destructor 00553020 and
// ~CSWRules 00551440 (swkotor.exe GOG K1 sha256 9c10e045..., base 0x400000).
// Shutdown path only (not reached before the first menu frame); required so that the
// vtable stored by CSWRules::CSWRules at 00552C78 holds a real typed slot.
#include "45d2335566-rules_common.h"
#include "f7bace6431-rules_vtable.h"
#include "55aae85039-two_dim_arrays.h"
#include "bc7bc38e49-spell_array.h"
#include "1039f35635-base_item_array.h"
#include "b71ed2c913-class_info.h"
#include "22ceec0ec1-race_skill_feat.h"
#include <initializer_list>

namespace kotor_rules {
namespace {
// D24 typed adapters: 006FA852 (M41 c2da_destroy_array_006fa852) takes its element
// destructor as void(__fastcall*)(StartupString*, void*); the original passes the
// element destructors 004015A0 / 005BCBB0 / 005C78D0 / 0059D7C0 (pushed as immediates
// at 005514C6 / 0055150C / 0055153C / 00551569). Each adapter runs exactly the owning
// body on the same element address (object-pointer conversion only).
void __fastcall feat_element_destroy_004015a0(String* element, void*) { kotor_fastcall_004015a0(element, nullptr); }
void __fastcall class_element_destroy_005bcbb0(String* element, void*) { kotor_fastcall_005bcbb0(element, nullptr); }
void __fastcall race_element_destroy_005c78d0(String* element, void*) { kotor_fastcall_005c78d0(element, nullptr); }
void __fastcall skill_element_destroy_0059d7c0(String* element, void*) { kotor_fastcall_0059d7c0(element, nullptr); }

// "delete[] p" for an array allocated with a count cookie at p-4:
// 006FA852(p, stride, [p-4], dtor) then 006FA390(p-4).
void destroy_counted(void* self, unsigned off, Word stride, c2da_current::StringDestroy destroy) {
    unsigned char* const p = ptr_at<unsigned char>(self, off);
    const Word count = rd32(p - 4, 0);
    c2da_destroy_array_006fa852(p, stride, count, destroy);
    live_free(p - 4);
}
}  // namespace

// 00551440 CSWRules::~CSWRules (00551440..005515D4), RET.
void __fastcall kotor_fastcall_00551440(void* self, void*) {
    put_ptr(self, 0x00, &kotor_vtable_00748bb4);                         // 0055144F mov [esi],748BB4h
    if (rd32(self, 0xB8) != 0) {                                          // 00551444..00551455
        kotor_fastcall_005c5070(ptr_at<void>(self, 0xB8), nullptr);       // 00551457 UnLoad2DArrays
        if (void* const arrays = ptr_at<void>(self, 0xB8)) {              // 0055145C..00551464
            kotor_fastcall_005bfcc0(arrays, nullptr);                     // 00551468 ~CTwoDimArrays
            live_free(arrays);                                            // 0055146D 006FA390
        }
        wr32(self, 0xB8, 0);                                              // 00551476
    }
    if (void* const items = ptr_at<void>(self, 0x34)) {                   // 0055147C..00551481
        kotor_fastcall_005b4730(items, nullptr);                          // 00551485 ~CSWBaseItemArray
        live_free(items);                                                 // 0055148B
        wr32(self, 0x34, 0);                                              // 00551493
    }
    if (void* const spells = ptr_at<void>(self, 0x8C)) {                  // 00551496..0055149E
        kotor_fastcall_0059b9e0(spells, nullptr);                         // 005514A2 ~CSWSpellArray
        live_free(spells);                                                // 005514A8
        wr32(self, 0x8C, 0);                                              // 005514B0
    }
    if (rd32(self, 0x90) != 0) {                                          // 005514B6..005514BE
        destroy_counted(self, 0x90, 0x48, &feat_element_destroy_004015a0); // 005514C0..005514D5 (stride 48h, dtor 004015A0)
        wr32(self, 0x90, 0);                                              // 005514DD
    }
    if (void* const p = ptr_at<void>(self, 0xA0)) {                       // 005514E3..005514EB
        live_free(p);                                                     // 005514EE
        wr32(self, 0xA0, 0);                                              // 005514F6
    }
    if (rd32(self, 0xAC) != 0) {                                          // 005514FC..00551504
        destroy_counted(self, 0xAC, 0x1A0, &class_element_destroy_005bcbb0); // 00551506..0055151E (stride 1A0h, dtor 005BCBB0)
        wr32(self, 0xAC, 0);                                              // 00551526
    }
    if (rd32(self, 0xB0) != 0) {                                          // 0055152C..00551534
        destroy_counted(self, 0xB0, 0x34, &race_element_destroy_005c78d0); // 00551536..0055154B (stride 34h, dtor 005C78D0)
        wr32(self, 0xB0, 0);                                              // 00551553
    }
    if (rd32(self, 0xB4) != 0) {                                          // 00551559..00551561
        destroy_counted(self, 0xB4, 0x20, &skill_element_destroy_0059d7c0); // 00551563..00551578 (stride 20h, dtor 0059D7C0)
        wr32(self, 0xB4, 0);                                              // 00551580
    }
    for (unsigned off : {0x94u, 0x98u, 0x9Cu}) {                          // 00551586..005515CB
        if (void* const p = ptr_at<void>(self, off)) {
            live_free(p);
            wr32(self, off, 0);
        }
    }
}

// 00553020 CSWRules::`scalar deleting destructor' (00553020..0055303B), RET 4, EAX=this.
void* __fastcall kotor_fastcall_00553020(void* self, void*, std::uint32_t flags) {
    kotor_fastcall_00551440(self, nullptr);                               // 00553023
    if ((flags & 1u) != 0) live_free(self);                               // 00553028..00553030 006FA390
    return self;                                                          // 00553038
}
}  // namespace kotor_rules

// 00748BB4 CSWRules vtable (.rdata): { 00553020 }.
extern "C" const kotor_rules::CSWRulesMethods kotor_vtable_00748bb4 = {&kotor_rules::kotor_fastcall_00553020};
