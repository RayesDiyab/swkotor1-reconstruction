// W5 t2 (G18, server side, first part): the door / use-object request producers and CSWSDoor::OpenDoor.
//   client 00677D70 SendPlayerToServerInput_ChangeDoorState (6/3: OBJECTID, WORD state)
//   -> 005254C0 minor 3 (in source/w5_input_message.cpp)
//   -> AddOpenDoorAction 004CFAC0 (state 2725h) or AddCloseDoorAction 004CFB20
//   -> W1 runner 0057F4A0 -> AIActionOpenDoor 0057D490 (W5 t3; trap body today)
//   -> AI event 7 to the door -> CSWSDoor::EventHandler 0058B850 -> OpenDoor 00589C70 (here)
//   -> SetOpenState 00589600 (existing) -> the door 'U' record of W2's UpdateObject 00574A10 -> client 0064D420.
#include "d8371efa32-w5_t2.hpp"
#include "71303376b8-w5_action_queue.hpp"
#include "ab3fc15780-w5_drive.hpp"

using namespace w5;

namespace frame_control { extern unsigned message_manager_007a3a00; }                    // [007A3A00] CVirtualMachine*

extern "C" {
void __fastcall recovered_door_state_00589600(void* door, void*, U state, U notify);       // CSWSDoor::SetOpenState (1121)
U __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U object_id, U flag);       // CVirtualMachine::RunScript (1121)
U __fastcall w5_t2_module_find_object_by_tag_ordinal_004c5fe0(void* module, void*, void* tag, U ordinal); // trap import

// 004CFAC0 CSWSObject::AddOpenDoorAction(door id, unused): AddAction(14h, FFFFh, {3: id}, {1: arg2})
U __fastcall w5_object_add_open_door_action_004cfac0(void* object, void*, U door_id, U arg2) {
    if (at<U>(object, 0xe8) == 0) return 0;                                                // 004CFAC0..CC
    const U pairs[26] = {3, static_cast<U>(reinterpret_cast<std::uintptr_t>(&door_id)),
                         1, static_cast<U>(reinterpret_cast<std::uintptr_t>(&arg2))};
    w5_add_action_body(object, 0x14, 0xffff, pairs);                                        // 004CFACF..10
    return 1;
}
// 004CFB20 CSWSObject::AddCloseDoorAction(door id, unused): AddAction(15h, FFFFh, {3: id}, {1: arg2})
U __fastcall w5_object_add_close_door_action_004cfb20(void* object, void*, U door_id, U arg2) {
    if (at<U>(object, 0xe8) == 0) return 0;
    const U pairs[26] = {3, static_cast<U>(reinterpret_cast<std::uintptr_t>(&door_id)),
                         1, static_cast<U>(reinterpret_cast<std::uintptr_t>(&arg2))};
    w5_add_action_body(object, 0x15, 0xffff, pairs);
    return 1;
}
// 0057C810 CSWSObject::AddUseObjectAction(object id): AddAction(28h, FFFFh, {3: id})
U __fastcall w5_object_add_use_object_action_0057c810(void* object, void*, U target_id) {
    if (at<U>(object, 0xe8) == 0) return 0;
    const U pairs[26] = {3, static_cast<U>(reinterpret_cast<std::uintptr_t>(&target_id))};
    w5_add_action_body(object, 0x28, 0xffff, pairs);
    return 1;
}

// 00589580 CSWSDoor::GetLinkedObject: linked kind (+384) 1 or 2 -> door +398, else by tag (+388) via the module
void* __fastcall w5_door_get_linked_object_00589580(void* door, void*) {
    const U8 kind = at<U8>(door, 0x384);
    if (kind != 1 && kind != 2) return nullptr;                                             // 00589583..94
    void* linked = w5_server_get_door_by_id_004ae7c0(server_app(), nullptr, at<U>(door, 0x398)); // 00589596..A5
    if (linked) return linked;
    const U id = w5_t2_module_find_object_by_tag_ordinal_004c5fe0(server_get_module_004ae6b0(server_app(), nullptr), nullptr,
        static_cast<unsigned char*>(door) + 0x388, 0);                                    // 005895B0..C7
    at<U>(door, 0x398) = id;                                                                // 005895CC
    void* object = game_object(id);                                                         // 005895D2..DC
    return object ? vcall<void*>(object, 0x10) : nullptr;                                   // 005895E1..EB (tail call)
}

// 00589C70 CSWSDoor::OpenDoor(opener id): side 1 if the opener is in front (dot(opener - door, facing) > 0), else 2
void __fastcall w5_door_open_door_00589c70(void* door, void*, U opener) {
    U side = 2;                                                                             // EBX
    if (void* o = game_object(opener)) {                                                    // 00589C7E..8E
        unsigned char* p = static_cast<unsigned char*>(vcall<void*>(o, 0x10)) + 0x90;       // 00589C90..A3
        const double dx = static_cast<double>(at<float>(p, 0)) - at<float>(door, 0x90);
        const double dy = static_cast<double>(at<float>(p, 4)) - at<float>(door, 0x94);
        const double dz = static_cast<double>(at<float>(p, 8)) - at<float>(door, 0x98);
        const double d = (dz * at<float>(door, 0xa4) + dy * at<float>(door, 0xa0)) + dx * at<float>(door, 0x9c); // 00589CA8..D2
        if (d > 0.0) side = 1;                                                              // 00589CD4..E1
    }
    at<U>(door, 0x31c) = opener;                                                            // 00589CEB
    recovered_door_state_00589600(door, nullptr, side, 1);                                  // 00589CE6..F1
    if (void* linked = w5_door_get_linked_object_00589580(door, nullptr)) {                 // 00589CF6..01
        if (vcall<void*>(linked, 0x18))                                                     // 00589D03..0C (AsDoor)
            recovered_door_state_00589600(vcall<void*>(linked, 0x18), nullptr, side, 1);    // 00589D0E..1A
    }
    frame_R4_005d0fc0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)), nullptr,
                      static_cast<unsigned char*>(door) + 0x228, at<U>(door, 4), 1);       // 00589D1F..32 (OnOpen)
}
}  // extern "C"
