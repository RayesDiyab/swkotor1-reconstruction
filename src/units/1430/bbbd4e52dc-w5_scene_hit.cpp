// W5 t3: SceneHitCheck 0060F1E0 (client line-of-sight probe: W4's CSWCCreature::CanSee 00617AD0 and others), with
// CSWSArea::GetRoom(int) 004BB700 and CSWRoomSurfaceMeshHitInfo::ClearOutput 00580750. Transcribed from the original.
#include "842d6b1618-w5_t3.hpp"

using namespace w5;

extern "C" {
void* __fastcall surface_hit_info_construct_00581320(void* self, void*, U mask);   // CSWRoomSurfaceMeshHitInfo ctor (0526)
U __fastcall w5_room_check_aabb_all_00579610(void* room, void*, Vector from, Vector to, SurfaceHit* hit);   // t3 area queries
extern unsigned scene3d_gate_0078e5a8;                                            // [0078E5A8] (0537 core46 cells)

// 004BB700 CSWSArea::GetRoom(int index): room 4Ch-record pointer, null outside [0, +22C). RET 4.
void* __fastcall w5_area_get_room_by_index_004bb700(void* area, void*, I index) {
    if (index < 0 || index >= at<I>(area, 0x22c)) return nullptr;                  // 004BB700..0E
    return at<unsigned char*>(area, 0x230) + index * 0x4c;                          // 004BB710..1B
}
// 00580750 CSWRoomSurfaceMeshHitInfo::ClearOutput: +8 hit and +1C cleared, +10 point = 0. RET 0.
void __fastcall w5_surface_hit_clear_output_00580750(SurfaceHit* hit, void*) {
    hit->hit_8 = 0; hit->bit_1c = 0;                                                // 00580755..58
    hit->point_10.x = 0.0f; hit->point_10.y = 0.0f; hit->point_10.z = 0.0f;         // 0058075E..71 (DWORD zero stores)
}

// 0060F1E0 SceneHitCheck(scene, Vector from, Vector to, int flags, CHitInfo* hit): cdecl. The end point is first
// shortened by every server room's all-material walkmesh hit (in room order), then the scene's own hit check
// (vtbl+38, flags 0, with the scene gate [0078E5A8] cleared around it) runs on from -> end. EAX = the scene result
// when nonzero, else 1 when a walkmesh hit shortened the line, else 0. `flags` is not read.
// Export name is the one W4's can_see.cpp imports.
I __cdecl kotor_cdecl_0060f1e0(void* scene, Vector from, Vector to, I flags, void* hit) {
    (void)flags;
    SurfaceHit room_hit;                                                            // [esp+18]
    void* server = at<void*>(manager(), 8);                                         // 0060F1E3..EB
    surface_hit_info_construct_00581320(&room_hit, nullptr, 0xffffffffu);           // 0060F1EF..F5
    I result = 0;                                                                   // [esp+14]
    Vector end = to;                                                                // EBX/EDI/ESI
    void* module = server ? server_get_module_004ae6b0(server, nullptr) : nullptr;  // 0060F1FA..13
    void* area = module ? module_get_area_004c30f0(module, nullptr) : nullptr;      // 0060F219..24
    if (area != nullptr) {
        I index = 0;                                                                // [esp+10]
        void* room = w5_area_get_room_by_index_004bb700(area, nullptr, 0);          // 0060F22A..3D
        while (room != nullptr) {                                                   // 0060F250..C2
            w5_surface_hit_clear_output_00580750(&room_hit, nullptr);               // 0060F250..54
            if (w5_room_check_aabb_all_00579610(w5_area_get_room_by_index_004bb700(area, nullptr, index), nullptr,
                                                from, end, &room_hit) != 0) {       // 0060F259..99
                end = room_hit.point_10;                                            // 0060F29B..A3
                result = 1;                                                         // 0060F2A7
            }
            ++index;                                                                // 0060F2AF..B7
            room = w5_area_get_room_by_index_004bb700(area, nullptr, index);        // 0060F2B4..C2
        }
    }
    const unsigned gate = scene3d_gate_0078e5a8;                                    // 0060F2D6
    scene3d_gate_0078e5a8 = 0;                                                      // 0060F306
    using HitCheck = I(__fastcall*)(void*, void*, Vector, Vector, U, void*);
    const I r = reinterpret_cast<HitCheck>(at<void*>(*static_cast<void**>(scene), 0x38))(scene, nullptr, from, end, 0, hit); // 0060F2DC..15
    scene3d_gate_0078e5a8 = gate;                                                   // 0060F31A
    return r != 0 ? r : result;                                                     // 0060F318..22
}
}  // extern "C"
