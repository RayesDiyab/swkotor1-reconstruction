// lane-world-los: CServerExoApp / CServerExoAppInternal / CSWSModule accessors used by the LOS closure.
// Evidence: lane-world-los\asm\004AE6B0.asm .. 004D8230.asm (dumpbin of the GOG swkotor.exe).
#include "dc46124e28-world_los.h"

namespace {
using namespace kotor_world_los;
// CServerExoApp: +4 -> CServerExoAppInternal* (004AE6B0 / 004AE750 / 004AE780 `mov ecx,[ecx+4]`).
inline void* internal_of(void* server) { return view<void*>(server, 4); }
// CServerExoAppInternal: +1005C CGameObjectArray* (004B1507 / 004B1705 / 004B17F2).
inline void* object_array_of(void* internal) { return view<void*>(internal, 0x1005C); }
}

extern "C" {

// 004AE6B0: mov ecx,[ecx+4]; jmp 004B14F0 (tail call, RET 0).
void* __fastcall server_get_module_004ae6b0(void* server, void*) {
    return server_internal_get_module_004b14f0(internal_of(server), nullptr);
}

// 004AE750: mov ecx,[ecx+4]; jmp 004B1700 (tail call, callee RET 4). Consumer-required spelling (lane-r4-lowers).
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U object_id) {
    return server_internal_get_game_object_004b1700(internal_of(server), nullptr, object_id);
}

// 004AE780: mov ecx,[ecx+4]; jmp 004B17E0 (tail call, callee RET 4).
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U object_id) {
    return server_internal_get_area_by_object_id_004b17e0(internal_of(server), nullptr, object_id);
}

// 004B14F0 CServerExoAppInternal::GetModule (RET 0). Cache: id +1B918, module +1B91C; module id at +10060.
void* __fastcall server_internal_get_module_004b14f0(void* internal, void*) {
    const U module_id = view<U>(internal, 0x10060);                       // 004B14F4
    if (view<U>(internal, 0x1B918) != module_id)                          // 004B14FA..004B1500
    {
        void* object = nullptr;                                            // 004B150E local = 0 (before call)
        // 004B1502..004B1516: status in AL is not inspected by this function.
        (void)kotor_fastcall_004d8230(object_array_of(internal), nullptr, module_id, &object);
        if (object != nullptr) {                                           // 004B151B..004B1521
            view<U>(internal, 0x1B918) = view<U>(internal, 0x10060);     // 004B1523..004B1529 (id re-read)
            GameObject* const game_object = static_cast<GameObject*>(object);
            void* const module = game_object->vtbl->as_module_20(game_object, nullptr); // 004B152F..004B1531 [vt+20h]
            view<void*>(internal, 0x1B91C) = module;                      // 004B1534
            return module;                                                 // 004B153A..004B153C (EAX = slot result)
        }
        view<U>(internal, 0x1B918) = k_invalid_object_7f000000;           // 004B153D
        view<void*>(internal, 0x1B91C) = nullptr;                         // 004B1547
    }
    return view<void*>(internal, 0x1B91C);                                // 004B1551
}

// 004B1700 CServerExoAppInternal::GetGameObject (RET 4): object when status == [0074666C] else 0.
void* __fastcall server_internal_get_game_object_004b1700(void* internal, void*, U object_id) {
    void* object = nullptr;                                                // 004B1711 local = 0
    const unsigned char status =
        kotor_fastcall_004d8230(object_array_of(internal), nullptr, object_id, &object); // 004B1701..004B1719
    // 004B171E..004B1730: sub al,dl; neg al; sbb eax,eax; not eax; and eax,ecx
    return status == k_object_found_0074666c ? object : nullptr;
}

// 004B17E0 CServerExoAppInternal::GetAreaByGameObjectID (RET 4). Cache: id +1B920, area +1B924.
void* __fastcall server_internal_get_area_by_object_id_004b17e0(void* internal, void*, U object_id) {
    if (view<U>(internal, 0x1B920) != object_id) {                        // 004B17E3..004B17F0
        view<void*>(internal, 0x1B924) = nullptr;                         // 004B17FE
        view<U>(internal, 0x1B920) = k_invalid_object_7f000000;           // 004B1808
        void* object = nullptr;                                            // 004B1812
        const unsigned char status =
            kotor_fastcall_004d8230(object_array_of(internal), nullptr, object_id, &object); // 004B181A
        if (status == k_object_found_0074666c && object != nullptr &&      // 004B181F..004B182D
            object_type(static_cast<GameObject*>(object)) == k_type_area_00746633) { // 004B182F..004B1838 ([00746633]=04)
            view<U>(internal, 0x1B920) = object_id;                       // 004B183A
            GameObject* const game_object = static_cast<GameObject*>(object);
            view<void*>(internal, 0x1B924) = game_object->vtbl->as_area_28(game_object, nullptr); // 004B1840..004B1845
        }
    }
    return view<void*>(internal, 0x1B924);                                // 004B184B
}

// 004C30F0 CSWSModule::GetArea (RET 0). Area id +40; cache id +21C, area +220.
void* __fastcall module_get_area_004c30f0(void* module, void*) {
    const U area_id = view<U>(module, 0x40);                              // 004C30F3
    if (view<U>(module, 0x21C) != area_id) {                              // 004C30F6..004C30FC
        void* const server = view<void*>(kotor_menu_window_cells::manager_007a39fc, 8); // 004C30FE..004C3104 [[7A39FC]+8]
        view<void*>(module, 0x220) = kotor_fastcall_004ae780(server, nullptr, area_id); // 004C3107..004C310D
    }
    void* const area = view<void*>(module, 0x220);                        // 004C3113
    if (area != nullptr) {                                                 // 004C3119..004C311B
        view<U>(module, 0x21C) = view<U>(module, 0x40);                  // 004C311D..004C3120 (id re-read)
        return area;
    }
    view<void*>(module, 0x220) = nullptr;                                 // 004C3128
    void* const result = view<void*>(module, 0x220);                      // 004C3132 (reloaded: 0)
    view<U>(module, 0x21C) = k_invalid_object_7f000000;                   // 004C3138
    return result;
}

} // extern "C"
