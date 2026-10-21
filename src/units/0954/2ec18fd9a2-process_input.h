#pragma once
// CClientExoAppInternal::ProcessInput 006227E0..006244B0 (lane-input, fork P2).
// Externals already declared by client_input_event.h (fork H) are reused as-is;
// only callees new to this unit are declared here (D1: an original __thiscall is
// ABI-identical to __fastcall(self, unused EDX, stack args...); floats are passed
// on the stack and returned in ST0 in both conventions).
// Return types: the consumed EAX/AL/AX/ST0 is typed; "void" means the result is
// never consumed at any call site in this unit (provider type not claimed).
#include <cstdint>
#include "deeac869ec-hover_path.h"
#include "68bf0f0956-client_input_event.h"
#include "0047cfa72a-startup_string.h"

namespace kotor_input_required {
using U32 = std::uint32_t;
// CClientExoApp (ECX = *(007A39FC)+4)
int __fastcall get_capture_event_play_back_005ed480(void* app, void*);          // RET
int __fastcall get_capture_events_005ed470(void* app, void*);                   // RET
void* __fastcall get_capture_event_file_005ed460(void* app, void*);             // RET
void* __fastcall get_world_timer_005ed4f0(void* app, void*);                    // RET
void* __fastcall get_module_005ed530(void* app, void*);                         // RET
// CClientExoAppInternal (ECX = this)
void __fastcall play_back_input_events_006224e0(void* self, void*);             // RET
void* __fastcall get_game_object_005f10f0(void* self, void*, U32 id);           // RET 4
void __fastcall empty_hitcheck_ignore_list_005f6410(void* self, void*);         // RET
void __fastcall add_object_to_hitcheck_ignore_list_005f6340(void* self, void*, U32 id);  // RET 4
void __fastcall set_hitcheck_ignore_list_005f94d0(void* self, void*, void* hit_info);    // RET 4
void __fastcall handle_mouse_over_006222f0(void* self, void*, U32 id, void* position, U32 flag);  // RET 0Ch
// CWorldTimer
int __fastcall get_snapshot_time_difference_004adc80(void* timer, void*);       // RET (EAX consumed as signed by FILD 00622890)
unsigned __int64 __fastcall get_snapshot_time_004adc70(void* timer, void*);     // RET (EDX:EAX)
// CExoInput (ECX = *(007A39E4)); wrapper JMPs CExoInputInternal::PollInput 005E23C0
float __fastcall poll_input_005df4d0(void* input, void*, U32 event, U32 input_class);  // RET 8, ST0
// CExoArrayList<CExoInputEvent> (ECX = &list)
void __fastcall event_list_delete_0061fb50(kotor_input_hover::InputEventList* list, void*, int index);  // RET 4
void __fastcall event_list_add_005e0a30(kotor_input_hover::InputEventList* list, void*, kotor_input_hover::InputEvent e);  // RET 10h
void __fastcall event_list_insert_00620480(kotor_input_hover::InputEventList* list, void*, kotor_input_hover::InputEvent e, int index);  // RET 14h
// CHitInfo / CExoArrayList
void* __fastcall hit_info_ctor_004578f0(void* hit, void*, U32, U32, U32);       // RET 0Ch, EAX=this
void __fastcall array_remove_00445080(void* list, void*, void* const* element); // RET 4
// C2DA
int __fastcall c2da_get_int_entry_00413510(void* table, void*, U32 row, U32 column, int* out);  // RET 0Ch
// CSWCModule / CSWCArea / CSWCPlaceable / CSWSCreature / CSWCItem / CSWCDoor / CSWCObject
void* __fastcall get_mini_game_00641980(void* module, void*);                   // RET
void __fastcall accl_tilt_camera_0063fdb0(void* module, void*, float tilt);     // RET 4
int __fastcall get_is_object_corpse_00604bc0(void* area, void*, U32 id);        // RET 4
void* __fastcall get_server_placeable_00682e00(void* placeable, void*);         // RET
int __fastcall get_dead_temp_004ef890(void* server_creature, void*);            // RET
void* __fastcall get_equipped_by_00646630(void* item, void*);                   // RET
int __fastcall get_is_open_0061f790(void* door, void*);                         // RET
unsigned short __fastcall get_current_animation_0063c060(void* object, void*);  // RET, AX consumed (0062323F)
void __fastcall abort_fire_forget_animation_0063cad0(void* object, void*, U32);  // RET 4
// CSWCCreature
unsigned char __fastcall can_see_00617ad0(void* creature, void*, void* object); // RET 4, AL consumed (00622FEC)
int __fastcall is_drive_animation_blocked_0060f0f0(void* creature, void*);      // RET
void __fastcall set_lock_orientation_to_object_00610950(void* creature, void*, U32 id, U32);  // RET 8
void __fastcall set_engaged_state_0060f140(void* creature, void*, U32);         // RET 4
void __fastcall set_way_points_0060c790(void* creature, void*, U32, U32);       // RET 8
// CSWParty / CSWPartyMemberData / camera
void* __fastcall get_character_006346c0(void* party, void*, U32 index);         // RET 4
int __fastcall get_index_006349c0(void* party, void*, U32 id);                  // RET 4
void __fastcall set_combat_message_006345e0(void* member, void*, U32 message);  // RET 4
void __fastcall clear_combat_camera_0063a360(void* camera, void*);              // RET
// CSWMiniGame
void __fastcall update_axis_00670fb0(void* mini_game, void*, void* axis, float delta);       // RET 8
void __fastcall update_mouse_00671020(void* mini_game, void*, float x, float y, float delta); // RET 0Ch
void __fastcall control_006710e0(void* mini_game, void*, float delta);          // RET 4
// CExoFile (same declaration as input_system.cpp)
void __fastcall file_write_string_005e6a70(void* file, void*, kotor_reconstruction::StartupString* text);  // RET 4
}

// Existing M41 providers (declarations identical to their defining objects).
namespace kotor_menu_10_19 { namespace dependencies {
unsigned __int64 __stdcall unsigned_divide_006fb030(unsigned __int64, unsigned __int64);
} }
namespace kotor_reconstruction { void __cdecl StartupFormat005e5680(StartupString*, const char*, ...); }
namespace kotor_menu20 {
using Word=std::uint32_t;
struct Input {Word unknown;void* volatile internal;__declspec(noinline) void __thiscall set_mouse_005df5d0(int x,int y);};
}

// Original cells with no M41 owner (declared only; owner to be assigned).
namespace kotor_input_cells_required {
extern void* volatile rules_007a3a28;                    // *(007A3A28)+B8 -> +2C C2DA (00622EC2)
extern volatile std::uint32_t column_007a225c;           // C2DA column index (00622ECD; writer 005C193A)
}

namespace kotor_process_input {
using U32 = std::uint32_t;
// D1 vtable views for the objects ProcessInput dispatches on. Slots not
// consumed here are opaque words; offsets static_asserted.
using Vector3 = kotor_input_hover::Vector3;                // {x,y,z} floats (hover_path.h)
struct GameObjectVtable {                                  // CSWCObject-like game object (hit target / party object)
    const void* s00_08[3];
    void* (__fastcall* object_0c)(void*, void*);           // +0x0C (00622FBF, 006232A0)
    const void* s10;
    void* (__fastcall* door_14)(void*, void*);             // +0x14 (006231A9)
    const void* s18_28[5];
    void* (__fastcall* creature_2c)(void*, void*);         // +0x2C (00623255)
    const void* s30;
    void* (__fastcall* item_34)(void*, void*);             // +0x34 (00623331)
    const void* s38;
    void* (__fastcall* object_3c)(void*, void*);           // +0x3C (00623219)
    const void* s40_48[3];
    void* (__fastcall* placeable_4c)(void*, void*);        // +0x4C (00622E85)
    const void* s50_134[58];
    void (__fastcall* slot_138)(void*, void*);             // +0x138 (006230FD, 0062344E; result unused)
    void (__fastcall* screen_position_13c)(void*, void*, Vector3*);  // +0x13C (00623011)
    const void* s140;
    float (__fastcall* pick_radius_144)(void*, void*);     // +0x144 (00623505; ST0 consumed)
};
struct GameObject { const GameObjectVtable* vtable; unsigned char opaque04[0x20]; Vector3 position; };  // +0x04 id, +0x24 position
struct HitObjectVtable {                                   // render object stored at CHitInfo+0x38
    const void* s00_6c[28];
    void* (__fastcall* user_data_70)(void*, void*);        // +0x70 (00622E3A)
    const void* s74;
    int (__fastcall* kind_78)(void*, void*);               // +0x78 (00622E4E)
};
struct HitObject { const HitObjectVtable* vtable; };
struct ServerCreatureVtable { const void* s00_90[37]; int (__fastcall* slot_94)(void*, void*); };  // +0x94 (006232DE)
struct ServerCreature { const ServerCreatureVtable* vtable; };
struct SceneVtable {                                       // module+0x40
    const void* s00_1c[8];
    void (__fastcall* camera_position_20)(void*, void*, Vector3*);           // +0x20 (00623056)
    const void* s24_2c[3];
    void (__fastcall* hit_test_30)(void*, void*, int x, int y, Vector3* out, void* hit);  // +0x30 (00622E26)
    const void* s34_7c[19];
    void* (__fastcall* camera_80)(void*, void*, U32 id);   // +0x80 (00623BF2, 00623DE5)
};
struct Scene { const SceneVtable* vtable; };
struct CameraVtable { const void* s00_0c[4]; int (__fastcall* type_10)(void*, void*); };  // +0x10 (00623DF5)
struct Camera { const CameraVtable* vtable; };
struct DriveVtable {                                       // CClientExoAppInternal+0x2A0 controller
    void (__fastcall* set_x_00)(void*, void*, float);      // +0x00 (00623D12)
    const void* s04;
    void (__fastcall* set_y_08)(void*, void*, float);      // +0x08 (00623F20)
    const void* s0c;
    void (__fastcall* set_flag_10)(void*, void*, U32);     // +0x10 (00623AA0)
    const void* s14_18[2];
    float (__fastcall* speed_1c)(void*, void*);            // +0x1C (00623BA4; ST0 consumed)
    const void* s20_24[2];
    void (__fastcall* update_28)(void*, void*, float);     // +0x28 (00623F3E)
};
struct Drive { const DriveVtable* vtable; };
struct UserDataVtable { const void* s00_08[3]; void* (__fastcall* object_0c)(void*, void*); };  // +0x0C (00622E74)
struct UserData { const UserDataVtable* vtable; };
static_assert(offsetof(GameObjectVtable, object_0c) == 0x0C && offsetof(GameObjectVtable, door_14) == 0x14 &&
              offsetof(GameObjectVtable, creature_2c) == 0x2C && offsetof(GameObjectVtable, item_34) == 0x34 &&
              offsetof(GameObjectVtable, object_3c) == 0x3C && offsetof(GameObjectVtable, placeable_4c) == 0x4C &&
              offsetof(GameObjectVtable, slot_138) == 0x138 && offsetof(GameObjectVtable, screen_position_13c) == 0x13C &&
              offsetof(GameObjectVtable, pick_radius_144) == 0x144 && offsetof(GameObject, position) == 0x24, "game object");
static_assert(offsetof(HitObjectVtable, user_data_70) == 0x70 && offsetof(HitObjectVtable, kind_78) == 0x78, "hit object");
static_assert(offsetof(ServerCreatureVtable, slot_94) == 0x94 && offsetof(CameraVtable, type_10) == 0x10, "creature/camera");
static_assert(offsetof(SceneVtable, camera_position_20) == 0x20 && offsetof(SceneVtable, hit_test_30) == 0x30 &&
              offsetof(SceneVtable, camera_80) == 0x80, "scene");
static_assert(offsetof(DriveVtable, set_y_08) == 0x08 && offsetof(DriveVtable, set_flag_10) == 0x10 &&
              offsetof(DriveVtable, speed_1c) == 0x1C && offsetof(DriveVtable, update_28) == 0x28, "drive");
static_assert(offsetof(UserDataVtable, object_0c) == 0x0C && sizeof(Vector3) == 12, "user data");
// CHitInfo local (0x64 bytes at [esp+C0h]; ctor 004578F0): +0x08 freed data, +0x38 hit object.
struct HitInfo { unsigned char opaque00[8]; void* data08; unsigned char opaque0c[0x2C]; HitObject* object38; unsigned char opaque3c[0x28]; };
static_assert(sizeof(HitInfo) == 0x64 && offsetof(HitInfo, object38) == 0x38, "hit info");
// Client options (CClientExoAppInternal+0x04 / CClientExoApp::GetClientOptions): +0x08 flags, +0x6D byte.
struct OptionsView { unsigned char opaque00[8]; unsigned char flags08; unsigned char opaque09[0x64]; unsigned char mode6d; };
static_assert(offsetof(OptionsView, mode6d) == 0x6D, "options");
}
