// Bounded full originals00610490 GetFreeLookVideoEffect /006383C0 UpdateCameraStyle.
#include "e82981358b-camera_stick.hpp"

using namespace w4;
namespace menu_tables_required {
int __fastcall C2daIntEntry_00413660(options_video::Table*,void*,int,options_video::String*,int*);
}

extern "C" U32 __fastcall kotor_fastcall_00610490(void* creature,void*) {
    if (lp(creature,0x21C)==nullptr) return 0xFFFFFFFFu;
    alignas(4) unsigned char name[8];
    frame_LogStringCtor_005e5a90(name,nullptr,"FreeLookEffect");
    const U32 row=lh(lp(creature,0x21C),0x18); // original reload after real String constructor
    void* table=lp(lp(kotor_cell_rules_007a3a28,0xB8),0xC);
    int result=-1; // original006104B1/B6 initialized output DWORD
    int status;
    try {
        status=menu_tables_required::C2daIntEntry_00413660(static_cast<options_video::Table*>(table),nullptr,static_cast<int>(row),reinterpret_cast<options_video::String*>(name),&result);
    } catch (...) { frame_LogStringDtor_005e5c20(name,nullptr); throw; }
    frame_LogStringDtor_005e5c20(name,nullptr);
    return status!=0 ? static_cast<U32>(result) : 0xFFFFFFFFu;
}

extern "C" void __fastcall kotor_fastcall_006383c0(void* free_look,void*) {
    void* table=lp(lp(kotor_cell_rules_007a3a28,0xB8),0x74);
    if (table==nullptr) {
        sw(free_look,0x28,0x42700000u);
        sw(free_look,0x2C,0x42700000u); // original continues to the real table calls
    }
    void* module=kotor_fastcall_005ed530(client_app(),nullptr);
    const int row=li(lp(module,0x48),0xB8);
    alignas(4) unsigned char name[8];
    alignas(4) unsigned char last_name[8]; // original fourth String has a distinct stack address
    frame_LogStringCtor_005e5a90(name,nullptr,"FL_TiltSpeed");
    try {
        menu_tables_required::C2daFloatEntry_00413430(static_cast<options_video::Table*>(table),nullptr,row,reinterpret_cast<options_video::String*>(name),reinterpret_cast<float*>(static_cast<U8*>(free_look)+0x2C));
    } catch (...) { frame_LogStringDtor_005e5c20(name,nullptr); throw; }
    frame_LogStringDtor_005e5c20(name,nullptr);
    frame_LogStringCtor_005e5a90(name,nullptr,"FL_RotateSpeed");
    try {
        menu_tables_required::C2daFloatEntry_00413430(static_cast<options_video::Table*>(table),nullptr,row,reinterpret_cast<options_video::String*>(name),reinterpret_cast<float*>(static_cast<U8*>(free_look)+0x28));
    } catch (...) { frame_LogStringDtor_005e5c20(name,nullptr); throw; }
    frame_LogStringDtor_005e5c20(name,nullptr);
    frame_LogStringCtor_005e5a90(name,nullptr,"FL_LOOKUP");
    try {
        menu_tables_required::C2daFloatEntry_00413430(static_cast<options_video::Table*>(table),nullptr,row,reinterpret_cast<options_video::String*>(name),reinterpret_cast<float*>(static_cast<U8*>(free_look)+0x30));
    } catch (...) { frame_LogStringDtor_005e5c20(name,nullptr); throw; }
    frame_LogStringDtor_005e5c20(name,nullptr);
    frame_LogStringCtor_005e5a90(last_name,nullptr,"FL_LOOKDOWN");
    try {
        menu_tables_required::C2daFloatEntry_00413430(static_cast<options_video::Table*>(table),nullptr,row,reinterpret_cast<options_video::String*>(last_name),reinterpret_cast<float*>(static_cast<U8*>(free_look)+0x34));
    } catch (...) { frame_LogStringDtor_005e5c20(last_name,nullptr); throw; }
    frame_LogStringDtor_005e5c20(last_name,nullptr);
    U32 selected;
    __asm {
        mov ecx, free_look
        fld dword ptr [ecx+30h]
        fcomp dword ptr [ecx+34h]
        fnstsw ax
        test ah, 41h
        jnz choose_lookup_down
        mov eax, [ecx+30h]
        jmp store_selection
    choose_lookup_down:
        mov eax, [ecx+34h]
    store_selection:
        mov selected, eax
    }
    sw(free_look,0xBC,selected);
}
