#pragma once
// lane-menu-tables, D1 epoch: authored original bodies exposed in the ABI-identical
// __fastcall(receiver, unused EDX, stack words) form (DECISIONS.md D1). Receivers are
// opaque original objects addressed by observed offsets only.
#include <cstdint>
#include "4dec35f274-providers.h"   // menu_providers_40_49::Options (0xA0 CClientOptions view)
#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Pinned MSVC x86 only
#endif
namespace menu_tables_m41 {
using U8=unsigned char;using U32=std::uint32_t;using I32=std::int32_t;
using Constructor=void*(__fastcall*)(void*,void*);
using Destructor=void(__fastcall*)(void*,void*);
// menu_providers_40_49 slots
void* __fastcall ClientListConstruct_004b25d0(void* list,void* unused_edx);                              // RET
void __fastcall SetDefaultKeyboardCamera_0061d9e0(menu_providers_40_49::Options* options,void* unused_edx); // RET
U32 __fastcall InputSetMouseCursor_005df5c0(void* input,void* unused_edx,U32 cursor,U32 unused,U32 adjust); // RET 0Ch, EAX
U32 __fastcall InputInternalSetMouseCursor_005e0000(void* internal,void* unused_edx,U32 cursor,U32 unused,U32 adjust); // RET 0Ch
void __fastcall GuiSetSoftwareMouseCursor_0040a270(void* gui,void* unused_edx,U32 cursor,U32 visible,U32 adjust); // RET 0Ch
// menu_normal_40_49 first-frame slots
void* __fastcall StringListConstruct_0062b510(void* list,void* unused_edx);
void* __fastcall LiveContentConstruct_0059ea90(void* element,void* unused_edx);
void __fastcall LiveContentDestroy_0059ea30(void* element,void* unused_edx);
void* __fastcall DialogMessageConstruct_006285b0(void* element,void* unused_edx);
void __fastcall DialogMessageDestroy_00628600(void* element,void* unused_edx);
void __stdcall VectorConstruct_006fb55f(void* destination,U32 size,U32 count,Constructor ctor,Destructor dtor); // RET 14h
void __stdcall ArrayUnwind_006fa7f4(void* destination,U32 size,U32 count,Destructor dtor);                   // RET 10h
void __fastcall SetCameraMode_0061b6f0(void* options,void* unused_edx,U32 mode);                             // RET 4 (byte arg)
// options_bindings slot
void __fastcall SetEnableHardwareMouseCursor_0061d670(void* options,void* unused_edx,std::int32_t enabled);  // RET 4
void __fastcall ClientAppSetMouseMode_005ee150(void* app,void* unused_edx,U32 mode);                        // RET 4 (byte arg)
// texture_bindings slot
void __cdecl SetDynamicMemoryRatio_0041e9b0(float ratio);
// lane-defined original cells (Main may relocate to its cell owner)
namespace cells {
extern float keyboard_camera_007a2320,keyboard_camera_007a2324,keyboard_camera_007a2328; // .data 0xBF800000
extern U32 dynamic_memory_ratio_0078d3f0; // .data 0x3F000000, raw bits
extern const U8 module_type_00746632;     // .rdata 0x03
extern const U32 invalid_object_00746644; // .rdata 0x7F000000
extern const U32 module_word_0074410c;    // .rdata 0x0000053C
}
}
// Required providers with no body in M41 or this lane (declared once, D1 form).
namespace menu_tables_required {
int __cdecl SetAnisotropy_0044f2c0(int);                                   // AurSetAnisotropy
void __cdecl SetVSyncState_0044ee90(unsigned char);                         // AurSetVSyncState
void __fastcall UpdateAllFonts_0040b420(void* gui,void* unused_edx);        // CSWGuiManager::UpdateAllFonts (task-28 donor is a member; needs D1 port)
void __fastcall AnimateExternal_0043e150(void* object,void* unused_edx,float weight); // CAurObject::AnimateExternal RET 4
void __fastcall ClientInternalSetMouseMode_005f49b0(void* internal,void* unused_edx,std::uint32_t mode); // RET 4
}
