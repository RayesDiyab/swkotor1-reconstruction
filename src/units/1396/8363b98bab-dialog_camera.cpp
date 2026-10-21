// Lane W4 G19 (client side): 0062ED30 CGuiInGame::GetDialogCameraPosition(Vector* out, int flag) (thiscall, RET 8).
// Transcribed from dis/0062ED30.asm. x87: products/sums are carried in double (PC53) exactly where the original keeps
// them on the FPU stack; the z product is stored to a float temporary ([esp+30h]) before its add, x/y are not.
#include "27e3c46f56-w4_frame.hpp"

extern "C" {
void* __fastcall client_get_player_creature_005ed540(void* app, void*);           // CClientExoApp::GetPlayerCreature
W4Vector* __fastcall quaternion_zdir_0043e250(const void* quaternion, void*, W4Vector* out); // Quaternion::zdir
W4Vector* __cdecl normalize_004ab180(W4Vector* out, const W4Vector* in);          // normalize (cdecl)
}
using namespace w4;

extern "C" W4Vector* __fastcall kotor_fastcall_0062ed30(void* in_game, void*, W4Vector* out, int flag) {
    W4Vector result;                                                                       // edi/ebx/ebp
    W4Vector camera_position = {0.0f, 0.0f, 0.0f};                                         // [esp+10h..18h] (0062ED48..58)
    void* player = client_get_player_creature_005ed540(client_app(), nullptr);            // 0062ED33..0062ED41
    if (player != nullptr) std::memcpy(&result, static_cast<U8*>(player) + 0x1DC, 12);    // 0062ED62..0062ED6C
    else result = camera_position;                                                        // 0062ED71..0062ED79
    if ((lw(in_game, 0xB20) != 0 || lw(in_game, 0xB24) != 0) &&                           // 0062ED7D..0062ED8F
        kotor_fastcall_005ed530(client_app(), nullptr) != nullptr) {                      // 0062ED95..0062EDA5
        void* camera = lp(kotor_fastcall_005ed530(client_app(), nullptr), 0x40);          // 0062EDAB..0062EDB9
        (void)client_get_player_creature_005ed540(client_app(), nullptr);                 // 0062EDBC..0062EDC4 (unused)
        if (camera != nullptr) {                                                          // 0062EDC9..0062EDCB
            vcall<W4Vector*, W4Vector*>(camera, 0x20, &camera_position);                  // 0062EDD1..0062EDDA GetPosition
            if (flag == 0) {                                                              // 0062EDDD..0062EDE3
                result = camera_position;                                                 // 0062EE7F..0062EE8F
            } else {
                U32 orientation[4];                                                       // [esp+34h]
                vcall<void*, void*>(camera, 0x24, orientation);                           // 0062EDE9..0062EDF2 GetOrientation
                W4Vector z, back, unit;                                                   // [esp+28h], [esp+1Ch]
                const W4Vector* zd = quaternion_zdir_0043e250(orientation, nullptr, &z);  // 0062EDF5..0062EDFE
                back.z = static_cast<float>(-static_cast<double>(zd->z));                 // 0062EE03..0062EE06
                back.y = static_cast<float>(-static_cast<double>(zd->y));                 // 0062EE0C..0062EE10
                back.x = static_cast<float>(-static_cast<double>(zd->x));                 // 0062EE12..0062EE18
                const W4Vector* n = normalize_004ab180(&unit, &back);                     // 0062EE1B..0062EE27
                const double x5 = static_cast<double>(n->x) * 5.0f;                       // 0062EE2C..0062EE31
                const double y5 = static_cast<double>(n->y) * 5.0f;                       // 0062EE37..0062EE3A



                const double z5 = static_cast<float>(static_cast<double>(n->z) * 5.0f);   // 0062EE40..0062EE49 fstp dword

                result.x = static_cast<float>(static_cast<double>(camera_position.x) + x5); // 0062EE4D..0062EE53
                result.y = static_cast<float>(static_cast<double>(camera_position.y) + y5); // 0062EE5B..0062EE61
                result.z = static_cast<float>(static_cast<double>(camera_position.z) + z5); // 0062EE6D..0062EE75
            }
        }
    }
    *out = result;                                                                        // 0062EE91..0062EE9E
    return out;                                                                           // 0062EEA6 ret 8
}

// 0062DBD0 CGuiInGame::IsInDialog(OBJECT_ID id) (thiscall, RET 4): id is the dialog owner (+170h) or speaker (+174h).
extern "C" int __fastcall kotor_fastcall_0062dbd0(void* in_game, void*, U32 id) {
    if (lw(in_game, 0x170) == id) return 1;                                                // 0062DBD0..0062DBDA
    if (lw(in_game, 0x174) == id) return 1;                                                // 0062DBDC..0062DBE2
    return 0;                                                                              // 0062DBE4
}
