// Lane W4 G14: 00641540 CSWCModule::SetCombatCamera and 00641DC0 CSWCModule::RestoreCamera (callers: party fork
// UpdateCombatMode 005F3AAE..005F3AC1). Transcribed from the original disassembly (dis via tools/odis.py).
// module+10Ch..+128h holds the camera settings saved by CSWCameraOnAStick::SaveCameraSettings 0063BA80
// (+10C valid flag, +110 saved type, +114 camera, +118 SafePointer target, +11C style, +120 body, +124 head, +128 -1).
#include "167f903b9c-camera_stick.hpp"

namespace menu_tables_m41 {
void __fastcall SetCameraMode_0061b6f0(void* options, void*, unsigned mode);   // CClientOptions::SetCameraMode (0366)
}

using namespace w4;

extern "C" {
void* __fastcall kotor_fastcall_005ed8b0(void*, void*); // existing GetSWParty, RET0
void* __fastcall frame_GetCharacter_006346c0(void*,void*,int);
U32 __fastcall kotor_fastcall_00610490(void*,void*);
void* __fastcall kotor_fastcall_0063a5d0(void*,void*,void*,void*,U32);
void* __fastcall kotor_fastcall_0063baf0(void*,void*,void*);
void* __fastcall kotor_fastcall_0063ad60(void*,void*,void*);
void __fastcall kotor_fastcall_006d22c0(void*,void*);
void* __fastcall kotor_fastcall_006d12b0(void*,void*,void*);
}
namespace {
// Keep the D8 FSTP DWORD and the0060F010 retained ST0 FADD in the original order.
void store_camera_height_sum(void* destination,U32 destination_offset,void* creature) {
    volatile U32 saved_offset = destination_offset; // survives both hidden assembly calls
    U32 stored_height, summed_height;
    __asm {
        mov ecx, creature
        mov eax, [ecx]
        call dword ptr [eax+0D8h]
        fstp dword ptr stored_height
        mov ecx, creature
        call kotor_fastcall_0060f010
        fadd dword ptr stored_height
        fstp dword ptr summed_height
    }
    sw(destination,saved_offset,summed_height);
}

}


// Full original00641540..00641602; mode change follows real combat constructor success/null return.
extern "C" U32 __fastcall kotor_fastcall_00641540(void* module,void*) {
    if (lb(kotor_fastcall_005ed700(client_app(),nullptr),0x6D)==3) {
        void* stick=vcall<void*,I32>(lp(module,0x40),0x80,0x106A);
        if (stick!=nullptr) kotor_fastcall_0063ba80(stick,nullptr,static_cast<U8*>(module)+0x10C);
    }
    void* allocation=live_operator_new(0x94);
    void* made=nullptr;
    try {
        if (allocation!=nullptr)
            made=kotor_fastcall_006d12b0(allocation,nullptr,vcall<void*>(lp(module,0x40),4));
    } catch (...) { live_free(allocation); throw; }
    menu_tables_m41::SetCameraMode_0061b6f0(kotor_fastcall_005ed700(client_app(),nullptr),nullptr,6);
    vcall<void,void*>(lp(module,0x40),0x7C,made);
    return 1;
}

// Original00641DC0..006422DB: full RestoreCamera; existing SetCombatCamera is a separately open dependency.
extern "C" U32 __fastcall kotor_fastcall_00641dc0(void* module,void*) {
    if (kotor_fastcall_005ede70(client_app(),nullptr)!=0) {
        (void)kotor_fastcall_00641540(module,nullptr);
        void* combat=vcall<void*,I32>(lp(module,0x40),0x80,0x1070);
        if (combat!=nullptr) kotor_fastcall_006d22c0(combat,nullptr);
        return 1;
    }
    if (lw(module,0x10C)==0) {
        menu_tables_m41::SetCameraMode_0061b6f0(kotor_fastcall_005ed700(client_app(),nullptr),nullptr,3);
        return 0;
    }

    W4Vector position={0,0,0};
    W4CamQuaternion orientation={1,0,0,0};
    if (vcall<void*,I32>(lp(module,0x40),0x80,-1)!=nullptr) {
        void* old=vcall<void*,I32>(lp(module,0x40),0x80,-1);
        void* owner=lp(old,0x10);
        W4Vector scratch_position;
        position=*vcall<W4Vector*,W4Vector*>(owner,0x64,&scratch_position);
        old=vcall<void*,I32>(lp(module,0x40),0x80,-1);
        owner=lp(old,0x10);
        W4CamQuaternion scratch_orientation;
        orientation=*vcall<W4CamQuaternion*,W4CamQuaternion*>(owner,0x68,&scratch_orientation);
    }
    U32 reset_orientation=0,reset_position=0;
    if (vcall<void*,I32>(lp(module,0x40),0x80,-1)!=nullptr) {
        if (vcall<U32>(vcall<void*,I32>(lp(module,0x40),0x80,-1),0x10)==0x106D)
            reset_orientation=1;
    }
    if (vcall<void*,I32>(lp(module,0x40),0x80,-1)!=nullptr) {
        if (vcall<U32>(vcall<void*,I32>(lp(module,0x40),0x80,-1),0x10)==0x106E)
            reset_orientation=1;
    }
    if (vcall<void*,I32>(lp(module,0x40),0x80,-1)!=nullptr) {
        if (vcall<U32>(vcall<void*,I32>(lp(module,0x40),0x80,-1),0x10)==0x1070)
            reset_position=1;
    }
    const U32 current_mode=lw(module,0xC);
    if ((current_mode!=3 && current_mode!=5 && current_mode!=7) || lw(module,0x10C)==0)
        return 0; // 00641FCE/00641FD6 -> original return0 at00641E51

    void* const saved=static_cast<U8*>(module)+0x10C;
    void* const target=static_cast<U8*>(module)+0x118;
    if (lp(module,0x118)==nullptr) {
        void* creature=frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(),nullptr),nullptr,0);
        if (creature!=nullptr) {
            const U32 saved_type=lw(module,0x110);
            if (saved_type==1) {
                void* allocation=live_operator_new(0xC4);
                void* made=nullptr;
                try {
                    if (allocation!=nullptr) {
                        void* camera=lp(module,0x40);
                        const U32 offset=kotor_fastcall_00610490(creature,nullptr);
                        void* part=vcall<void*>(creature,0xD0);
                        void* owner=vcall<void*>(camera,4);
                        made=kotor_fastcall_0063a5d0(allocation,nullptr,owner,part,offset);
                    }
                } catch (...) { live_free(allocation); throw; }
                vcall<void,void*>(lp(module,0x40),0x7C,made);
            } else if (saved_type==0) {
                void* allocation=live_operator_new(0x138);
                void* made=nullptr;
                try {
                    if (allocation!=nullptr) {
                        void* camera=lp(module,0x40);
                        void* part=vcall<void*>(creature,0xD0);
                        made=kotor_fastcall_00639fc0(allocation,nullptr,vcall<void*>(camera,4),part);
                    }
                } catch (...) { live_free(allocation); throw; }
                const U32 body=vcall<U32>(creature,0xD4);
                sw(made,0x68,body);
                store_camera_height_sum(made,0x6C,creature); // original D8 DWORD store then0060F010 ST0 add
                if (reset_position!=0) kotor_fastcall_00637c20(made,nullptr,position,orientation);
                sw(made,0x60,reset_orientation);
                vcall<void,void*>(lp(module,0x40),0x7C,made);
            }
        }
    } else {
        const U32 saved_type=lw(module,0x110);
        if (saved_type==1) {
            void* allocation=live_operator_new(0xC4);
            void* made=nullptr;
            try {
                if (allocation!=nullptr) made=kotor_fastcall_0063baf0(allocation,nullptr,saved);
            } catch (...) { live_free(allocation); throw; }
            vcall<void,void*>(lp(module,0x40),0x7C,made);
            sw(module,0xC,5);
        } else if (saved_type==0) {
            void* creature=frame_GetCharacter_006346c0(kotor_fastcall_005ed8b0(client_app(),nullptr),nullptr,0);
            if (creature!=nullptr) {
                safe_pointer_assign_00447fa0(target,nullptr,vcall<void*>(creature,0xD0));
                sw(module,0x120,vcall<U32>(creature,0xD4));
                store_camera_height_sum(module,0x124,creature);
            }
            void* allocation=live_operator_new(0x138);
            void* made=nullptr;
            try {
                if (allocation!=nullptr) made=kotor_fastcall_0063ad60(allocation,nullptr,saved);
            } catch (...) { live_free(allocation); throw; }
            if (reset_position!=0) kotor_fastcall_00637c20(made,nullptr,position,orientation);
            sw(made,0x60,reset_orientation);
            vcall<void,void*>(lp(module,0x40),0x7C,made);
            sw(module,0xC,3);
        }
    }
    void* saved_behavior=lp(module,0x12C);
    if (saved_behavior!=nullptr) {
        vcall<void,U32>(saved_behavior,0,1);
        sw(module,0x12C,0);
    }
    menu_tables_m41::SetCameraMode_0061b6f0(kotor_fastcall_005ed700(client_app(),nullptr),nullptr,lb(module,0xC));
    return 1;
}
