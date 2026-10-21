// Lane vmA, sub-batch F2: NWScript engine-command handlers (CSWVirtualMachineCommands::ExecuteCommand*) for sound
// objects, music queries, journal, PrintFloat, strings, locations and object distances, reconstructed from the
// original swkotor.exe (read-only, capstone linear sweep).  Addresses in comments are the original instructions.
// Every handler: original int __thiscall (int routine, int argc), RET 8 -> extern "C" int __fastcall
// kotor_fastcall_<addr>(self, edx, routine, argc) (@16).  Conventions: claude_vm_vmA_common.hpp.
// .rdata facts used as literals: [00741870] float 1.0E-4 (38D1B717h); [00747E00] double pi/180 (3F91DF46A2529D39h);
// [0073D71C] "" ; [00748748] "NW_JOURNAL_PICTURE%s"; [0074869C]/[007486AC]/[007486E0]/[007486FC] PrintFloat formats.
#include "520143efd3-claude_vm_vma_common.hpp"

namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // CExoString() [menu20_bindings]
namespace kotor_reconstruction {
struct StartupString;
void __cdecl StartupFormat005e5680(StartupString*, const char*, ...);                       // CExoString::Format [0125]
}
namespace alias_add {
kotor_reconstruction::StartupString* __fastcall Upper005e6360(kotor_reconstruction::StartupString* self, void*,
    kotor_reconstruction::StartupString* out);                                               // CExoString::UpperCase [0163]
}

extern "C" {
// ---- existing providers (c31 map; object in brackets) ----
void* __fastcall kotor_fastcall_004ae800(void* server, void*, U id);                         // GetSoundObjectByGameObjectID [claude_vm_actions_game]
void* __fastcall kotor_fastcall_004ae780(void* server, void*, U id);                         // GetAreaByGameObjectID [0530]
void* __fastcall recovered_placeable_lookup_004ae7a0(void* server, void*, U id);             // GetPlaceableByGameObjectID [0886]
void __fastcall recovered_placeable_light_set_00585080(void* placeable, void*, U value);     // CSWSPlaceable::SetIllumination RET 4 [0886]
void* __fastcall recovered_client_get_sound_object_005ed620(void* client, void*, U id);      // CClientExoApp::GetSoundObjectByGameObjectID [1204-01]
void __fastcall kotor_fastcall_005d5a30(void* source, void*, float variance);                // CExoSoundSource::SetFixedVariance RET 4 [w4-creature_ai]
U __cdecl aurora_vector_equal_004aa980(const float* left, const float* right);               // Vector::operator== [0446]
void* __fastcall recovered_server_sws_message_004aedb0(void* server, void*);                 // CServerExoApp::GetSWSMessage [creature_destructor_r2]
void* __fastcall recovered_server_player_list_004aed90(void* server, void*);                 // CServerExoApp::GetPlayerList [w1_native_b7]
void* __fastcall kotor_fastcall_00561080(void* player, void*);                               // CSWSPlayer::GetGameObject [0595]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* pos);              // CExoLinkedListInternal::GetAtPos [0168]
U __fastcall unresolved_list_next_005e9b90(void* list, void*, void** pos);                   // CExoLinkedListInternal::GetNext [0168]
void __fastcall kotor_fastcall_004d68a0(void* message, void*, U size, U a, U b);             // CSWMessage::CreateWriteMessage RET 0Ch [w2_message_native]
void __fastcall kotor_fastcall_00578570(void* message, void*, U id);                         // CSWSMessage::WriteOBJECTIDServer RET 4 [w2_message_native]
void __fastcall kotor_fastcall_004d6920(void* message, void*, U value_bits, U scale, U bits);// CSWMessage::WriteFLOAT RET 0Ch [w2_message_native]
U __fastcall kotor_fastcall_004d6440(void* message, void*, void** data, U* size);            // CSWMessage::GetWriteMessage RET 8 [w2_message_native]
U __fastcall kotor_fastcall_00567e60(void* message, void*, U player, U major, U minor, void* data, U size); // SendServerToPlayerMessage RET 14h [w2_message_native]
void __fastcall kotor_fastcall_0059a6f0(void* table, void*, void* name, U value, U flag);    // CSWSPlotVarTable SetInt-style journal hook RET 0Ch [claude_0886_bodies_vars]
void* __fastcall combat_reuse_faction_00513fc0(void* creature, void*);                       // CSWSCreature::GetFaction [w1_native_b7]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                            // CServerExoApp::GetModule [0530]
void* __fastcall kotor_fastcall_004aede0(void* server, void*);                               // CServerExoApp::GetWorldTimer [0457]
void __fastcall frame_GetWorldTime_004ade40(void* timer, void*, U* day, U* time);            // CWorldTimer::GetWorldTime RET 8 [0457]
void __fastcall kotor_fastcall_004c7ed0(void* module, void*, U index, void* s1, void* s2, U t1, U t2); // CSWSModule::AddWorldJournalEntry RET 14h [claude_p2s_b_journal]
U __fastcall kotor_fastcall_0056a1a0(void* msg, void*, void* player, U index, void* s1, void* s2, U t1, U t2); // SendServerToPlayerJournalAddWorld RET 18h [claude_p2s_b_journal]
void* __fastcall kotor_fastcall_004ca7a0(void* location, void*);                             // CScriptLocation::CScriptLocation [claude_vm_actions_queries]
void __fastcall w1x_object_set_orientation_004cd170(void* object, void*, void* orientation); // CSWSObject::SetOrientation RET 4 [0886]
// ---- reconstructed in this file (section at the end) ----
void __fastcall kotor_fastcall_005c8da0(void* sound, void*, float variance);
void __fastcall kotor_fastcall_0066ad80(void* sound, void*, float variance);
void __fastcall kotor_fastcall_005c8e50(void* sound, void*, Vector position);
U __fastcall kotor_fastcall_0056d5d0(void* message, void*, void* player, U id, Vector position);
}

using namespace vma;

namespace {
// 004015A0 is a bare `ret` (CScriptLocation trivial dtor) -> file-local no-op, as in the endar-actions files.
inline void location_dtor_004015a0(void*) {}
// [0083282C]: .bss dword read only by PrintString / PrintFloat; the original image contains no store to it, so it
// keeps its zero initial value.  File-local never-written zero cell (identical to the strings lane's cell).
U g_script_log_suppressed_0083282c = 0;
const double k_deg_to_rad_00747e00 = 0.017453292519943295;      // .rdata 00747E00 (3F91DF46A2529D39h)
constexpr float k_min_length_sq_00741870 = 1.0e-4f;             // .rdata 00741870 (38D1B717h)
inline void exo_ctor(ExoString* s) { (void)kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s), nullptr); }
inline kotor_reconstruction::StartupString* ss(ExoString* s) { return reinterpret_cast<kotor_reconstruction::StartupString*>(s); }
inline float fsub(float a, float b) { return static_cast<float>(static_cast<double>(a) - static_cast<double>(b)); }
inline void* client() { return client_app(); }
}  // namespace

extern "C" {

// ================================================================== 00543C30 SoundObjectSetFixedVariance(object, float) [routine 124]
int __fastcall kotor_fastcall_00543c30(void*, void*, int, unsigned) {
    U id = 0; float variance = 0.0f;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00543C30..45
    if (!pop_float(&variance)) return k_pop_failed;                                    // 00543C47..63
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                  // 00543C66..79
    if (sound) kotor_fastcall_005c8da0(sound, nullptr, variance);                      // 00543C7D..84
    return 0;                                                                          // 00543C89
}

// ================================================================== 00543D10 SoundObjectSetPosition(object, vector) [routine 416]
int __fastcall kotor_fastcall_00543d10(void*, void*, int, unsigned) {
    U id = 0; Vector position = {0.0f, 0.0f, 0.0f};                                    // 00543D1E..2E
    if (!pop_object(&id)) return k_pop_failed;                                         // 00543D36..3D
    if (!recovered_vm_outer_pop_vector_005d1040(vm(), nullptr, &position)) return k_pop_failed; // 00543D3F..5B
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                  // 00543D5E..73
    if (sound) kotor_fastcall_005c8e50(sound, nullptr, position);                      // 00543D75..90 (by value)
    return 0;                                                                          // 00543D95
}

// ================================================================== 00543B80 SoundObjectGetVolume(object) [routine 691]
// Quirk (original): when the id is not a sound object, the popped object id itself is pushed as the volume.
int __fastcall kotor_fastcall_00543b80(void*, void*, int, unsigned) {
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00543B80..9B
    void* sound = kotor_fastcall_004ae800(server_app(), nullptr, id);                  // 00543B9E..AC
    const U volume = sound ? at<U>(sound, 0x234) : id;                                 // 00543BB1..BD
    return push_int(static_cast<I>(volume));                                           // 00543BC1..DC
}

// ================================================================== 00536A10 MusicBackgroundGetBattleTrack(area) [routine 569]
int __fastcall kotor_fastcall_00536a10(void*, void*, int, unsigned) {
    U id = 0; I track = 0;                                                             // 00536A1D xor esi,esi
    if (!pop_object(&id)) return k_pop_failed;                                         // 00536A18..2F
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, id);                   // 00536A32..40
    if (area) track = at<I>(at<void*>(area, 0x208), 0x18);                             // 00536A45..4F
    return push_int(track);                                                            // 00536A52..6E
}

// ================================================================== 0053C1B0 MusicBackgroundGetNightTrack(area) [routine 559]
int __fastcall kotor_fastcall_0053c1b0(void*, void*, int, unsigned) {
    U id = 0; I track = 0;                                                             // 0053C1BD xor esi,esi
    if (!pop_object(&id)) return k_pop_failed;                                         // 0053C1B8..CF
    void* area = kotor_fastcall_004ae780(server_app(), nullptr, id);                   // 0053C1D2..E0
    if (area) track = at<I>(at<void*>(area, 0x208), 0x10);                             // 0053C1E5..EF
    return push_int(track);                                                            // 0053C1F2..0E
}

// ================================================================== 0054B1E0 SetJournalQuestEntryPicture(plot, object, picture, allParty, allPlayers) [routine 678]
// Quirk (original): with bAllPlayers set and no player list, returns 1.
int __fastcall kotor_fastcall_0054b1e0(void*, void*, int, unsigned) {
    ExoString plot, name;
    exo_ctor(&plot);                                                                   // 0054B202..06 [esp+30]
    exo_ctor(&name);                                                                   // 0054B20D..15 [esp+28]
    int status = 0;
    try {
        U id = 0; I picture = 0, all_party = 0, all_players = 0;
        if (!pop_string(&plot) || !pop_object(&id) || !pop_int(&picture) ||            // 0054B21A..45 [30],[1C],[10]
            !pop_int(&all_party) || !pop_int(&all_players)) {                          // 0054B247..87 [20],[18]
            exo_destroy(&name); exo_destroy(&plot); return k_pop_failed;               // 0054B28D..AC
        }
        kotor_reconstruction::StartupFormat005e5680(ss(&name), "NW_JOURNAL_PICTURE%s", plot.text, plot.length); // 0054B2B1..C1 (CExoString by value)
        if (all_players == 0) {                                                        // 0054B2C6..CF
            void* creature = kotor_fastcall_004ae770(server_app(), nullptr, id);       // 0054B2D5..E3
            if (creature) {                                                            // 0054B2E8..F0
                unsigned char* journal = static_cast<unsigned char*>(creature) + 0x100;
                if (journal) kotor_fastcall_0059a6f0(journal, nullptr, &name, static_cast<U>(picture), 0); // 0054B2F6..0B
                if (all_party != 0) {                                                  // 0054B310..14
                    void* faction = combat_reuse_faction_00513fc0(creature, nullptr);  // 0054B31A..1C
                    if (faction) {                                                     // 0054B321..23
                        U* members = at<U*>(faction, 0);                               // 0054B329
                        const I count = at<I>(faction, 4);                             // 0054B32D
                        if (members && count > 0) {                                    // 0054B32B..3A
                            for (I i = 0; i < count; ++i) {                            // 0054B336..79
                                void* other = kotor_fastcall_004ae770(server_app(), nullptr, members[i]); // 0054B340..4D
                                if (other && other != creature) {                      // 0054B352..5A
                                    unsigned char* j = static_cast<unsigned char*>(other) + 0x100;
                                    if (j) kotor_fastcall_0059a6f0(j, nullptr, &name, static_cast<U>(picture), 0); // 0054B35C..71
                                }
                            }
                        }
                    }
                }
            }
        } else {
            void* players = recovered_server_player_list_004aed90(server_app(), nullptr); // 0054B380..8D
            if (!players) { exo_destroy(&name); exo_destroy(&plot); return 1; }       // 0054B38F..B6
            void* position = at<void*>(at<void*>(players, 0), 0);                      // 0054B3BB..C1
            while (position) {                                                         // 0054B3C5
                void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, position); // 0054B3C7..CA
                if (player && kotor_fastcall_00561080(player, nullptr)) {              // 0054B3D1..DE
                    void* creature = as_creature(kotor_fastcall_00561080(player, nullptr)); // 0054B3E0..EB
                    if (creature) {                                                    // 0054B3EE..F0
                        unsigned char* j = static_cast<unsigned char*>(creature) + 0x100;
                        if (j) kotor_fastcall_0059a6f0(j, nullptr, &name, static_cast<U>(picture), 0); // 0054B3F2..07
                    }
                }
                (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &position); // 0054B40C..13
            }                                                                          // 0054B418..1E
        }
    } catch (...) { exo_destroy(&name); exo_destroy(&plot); throw; }
    exo_destroy(&name);                                                                // 0054B420..28
    exo_destroy(&plot);                                                                // 0054B42D..39
    return status;                                                                     // 0054B43E
}

// ================================================================== 00548640 AddJournalWorldEntry(index, entry, title) [routine 669]
int __fastcall kotor_fastcall_00548640(void*, void*, int, unsigned) {
    ExoString entry, title;
    exo_ctor(&entry);                                                                  // 00548662..66 [esp+28]
    exo_ctor(&title);                                                                  // 0054866D..75 [esp+20]
    try {
        void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);  // 0054867A..87 (ebp)
        void* module = server_get_module_004ae6b0(server_app(), nullptr);              // 0054868C..97 (esi)
        I index = 0;
        if (!pop_int(&index) || !pop_string(&entry) || !pop_string(&title)) {          // 0054869C..D8 [18],[28],[20]
            exo_destroy(&title); exo_destroy(&entry); return k_pop_failed;             // 005486DA..FD
        }
        U day = 0, time = 0;
        frame_GetWorldTime_004ade40(kotor_fastcall_004aede0(server_app(), nullptr), nullptr, &day, &time); // 00548702..1C [14],[10]
        if (module)                                                                    // 00548721..23
            kotor_fastcall_004c7ed0(module, nullptr, static_cast<U>(index), &entry, &title, day, time); // 00548725..40
        void* players = recovered_server_player_list_004aed90(server_app(), nullptr);  // 00548745..53
        if (players) {                                                                 // 00548755..57
            void* position = at<void*>(at<void*>(players, 0), 0);                      // 00548759..5F
            while (position) {                                                         // 00548763
                void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, position); // 00548765..68
                if (player && kotor_fastcall_00561080(player, nullptr) && message)     // 0054876F..80
                    (void)kotor_fastcall_0056a1a0(message, nullptr, player, static_cast<U>(index), &entry, &title, day, time); // 00548782..9E
                (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &position); // 005487A3..AA
            }                                                                          // 005487AF..B5
        }
    } catch (...) { exo_destroy(&title); exo_destroy(&entry); throw; }
    exo_destroy(&title);                                                               // 005487B7..BF
    exo_destroy(&entry);                                                               // 005487C4..D0
    return 0;                                                                          // 005487D5
}

// ================================================================== 005414D0 PrintFloat(float, width, decimals, center) [routine 2]
// Quirk (original): only the *format string* is built (width/decimals substituted into "%%%Nd.%02df"); the float is
// never formatted or printed, and the result is only CStr'd while [0083282C] is zero (it always is).
int __fastcall kotor_fastcall_005414d0(void*, void*, int, unsigned argc) {
    float value = 0.0f;
    if (!pop_float(&value)) return k_pop_failed;                                       // 005414EE..0F [esp+C]
    (void)value;
    const I n = static_cast<I>(argc);                                                  // 00541513 esi
    I width = 0, decimals = 0, center = 0;
    if (n >= 2) {                                                                      // 0054151A..1D (jl)
        if (!pop_int(&width)) return k_pop_failed;                                     // 0054151F..31
        if (width >= 0x12) width = 0x12;                                               // 00541537..3A -> 00541546
        else if (width < 0) width = 0;                                                 // 0054153C..40
    } else {
        width = 0x12;                                                                  // 00541546
    }
    if (n >= 3) {                                                                      // 0054154E..51
        if (!pop_int(&decimals)) return k_pop_failed;                                  // 00541553..65
        if (decimals >= 0xa) decimals = 9;                                             // 0054156B..6E -> 0054157A
        else if (decimals < 0) decimals = 0;                                           // 00541570..74
    } else {
        decimals = 9;                                                                  // 0054157A
    }
    if (n >= 4) {                                                                      // 00541582..85
        if (!pop_int(&center)) return k_pop_failed;                                    // 00541587..B0
    } else {
        center = 0;                                                                    // 005415B3
    }
    ExoString text;
    exo_ctor(&text);                                                                   // 005415B7..BB
    try {
        const char* format = center == 0
            ? (width >= 0xa ? "%%%2d.%02df\n" : "%%%1d.%02df\n")                       // 005415DA..EB (7486AC / 74869C)
            : (width >= 0xa ? "PRINTFLOAT2: %%%2d.%02df\n" : "PRINTFLOAT2: %%%1d.%02df\n"); // 005415ED..F9 (7486FC / 7486E0)
        kotor_reconstruction::StartupFormat005e5680(ss(&text), format, width, decimals); // 005415C0..FF
        if (g_script_log_suppressed_0083282c == 0) (void)WindowCStr005e5670(&text, nullptr); // 00541604..14
    } catch (...) { exo_destroy(&text); throw; }
    exo_destroy(&text);                                                                // 00541619..25
    return 0;                                                                          // 0054162F
}

// ================================================================== 0053DB80 GetStringUpperCase(string) [routine 60]
int __fastcall kotor_fastcall_0053db80(void*, void*, int, unsigned) {
    ExoString in, out;
    exo_ctor(&in);                                                                     // 0053DB98..9C [esp+8]
    exo_ctor(&out);                                                                    // 0053DBA1..AD [esp]
    int status = 0;
    try {
        if (!pop_string(&in)) { exo_destroy(&out); exo_destroy(&in); return k_pop_failed; } // 0053DBB2..FC
        ExoString temp;
        alias_add::Upper005e6360(ss(&in), nullptr, ss(&temp));                         // 0053DBFF..08 [esp+10]
        try {
            (void)async_assign_string_005e5c50(&out, nullptr, &temp);                  // 0053DC0D..17
        } catch (...) { exo_destroy(&temp); throw; }
        exo_destroy(&temp);                                                            // 0053DC1C..25
        if (!recovered_vm_outer_push_string_005d1090(vm(), nullptr, &out)) status = k_push_failed; // 0053DC2A..45 / 0053DC5D
    } catch (...) { exo_destroy(&out); exo_destroy(&in); throw; }
    exo_destroy(&out);                                                                 // 0053DC47 / 0053DC73
    exo_destroy(&in);                                                                  // 0053DC4C..58 / 0053DC78..84
    return status;
}

// ================================================================== 0053F3B0 Location(vector, float facing) [routine 215]
// Quirks (original): the new CScriptLocation leaks on a pop failure or a push failure; a failed allocation is not
// checked before the stores.  Orientation = normalize(cos(f*pi/180), sin(f*pi/180), 0) computed on the x87.
int __fastcall kotor_fastcall_0053f3b0(void*, void*, int, unsigned) {
    Vector position = {0.0f, 0.0f, 0.0f};                                              // 0053F3CB..DB [esp+C]
    void* memory = live_operator_new(0x18);                                            // 0053F3C9..E8
    void* location = memory ? kotor_fastcall_004ca7a0(memory, nullptr) : nullptr;     // 0053F3EF..00
    float facing = 0.0f;
    if (!recovered_vm_outer_pop_vector_005d1040(vm(), nullptr, &position)) return k_pop_failed; // 0053F402..1C
    if (!pop_float(&facing)) return k_pop_failed;                                      // 0053F422..34
    copy_vector(location, &position);                                                  // 0053F43A..4D
    Vector direction;
    float cos_x, sin_y;
    const float zero = 0.0f;
    __asm {
        fld dword ptr [facing]                                                         // 0053F450
        fmul qword ptr [k_deg_to_rad_00747e00]                                         // 0053F454
        fld st(0)                                                                      // 0053F463
        fcos                                                                           // 0053F466
        fstp dword ptr [cos_x]                                                           // 0053F470
        fsin                                                                           // 0053F474
        fstp dword ptr [sin_y]                                                           // 0053F476
    }
    direction.x = cos_x; direction.y = sin_y; direction.z = zero;                            // 0053F468 z = 0
    Vector normal;
    copy_vector(static_cast<unsigned char*>(location) + 0xc, normalize_004ab180(&normal, &direction)); // 0053F47A..93
    if (!recovered_vm_outer_push_engine_structure_005d10b0(vm(), nullptr, 2, location)) return k_push_failed; // 0053F492..BB
    location_dtor_004015a0(location);                                                  // 0053F4BE..C0
    live_free(location);                                                               // 0053F4C5..CB
    return 0;                                                                          // 0053F4CE
}

// ================================================================== 00537590 GetDistanceBetweenLocations(a, b) [routine 298]
int __fastcall kotor_fastcall_00537590(void*, void*, int, unsigned) {
    void* a = nullptr; void* b = nullptr;
    if (!pop_struct(2, &a)) return k_pop_failed;                                       // 00537590..B1
    if (!pop_struct(2, &b)) {                                                          // 005375B5..C9
        if (a) { location_dtor_004015a0(a); live_free(a); }                            // 005375CB..E0
        return k_pop_failed;                                                           // 005375E3
    }
    Vector d;
    d.z = fsub(at<float>(a, 8), at<float>(b, 8));                                      // 005375F3..FA
    d.y = fsub(at<float>(a, 4), at<float>(b, 4));                                      // 00537601..05
    d.x = fsub(at<float>(a, 0), at<float>(b, 0));                                      // 00537608..0A
    const float distance = static_cast<float>(kotor_cdecl_004ab100(&d));               // 0053760C..23
    const U pushed = recovered_vm_outer_push_float_005d1030(vm(), nullptr, distance);  // 00537627..34
    if (a) { location_dtor_004015a0(a); live_free(a); }                                // 0053763C..50 / 0053767C..90
    if (b) { location_dtor_004015a0(b); live_free(b); }                                // 00537658..6D / 00537698..AD
    return pushed ? 0 : k_push_failed;                                                 // 00537670 / 005376B0
}

// ================================================================== 00543360 SetPlaceableIllumination(object, int) [routine 544]
int __fastcall kotor_fastcall_00543360(void*, void*, int, unsigned) {
    U id = 0; I value = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00543360..75
    if (!pop_int(&value)) return k_pop_failed;                                         // 00543377..93
    void* placeable = recovered_placeable_lookup_004ae7a0(server_app(), nullptr, id);  // 00543396..A4
    if (placeable) recovered_placeable_light_set_00585080(placeable, nullptr, static_cast<U>(value)); // 005433A9..B4
    return 0;                                                                          // 005433B9
}

// ================================================================== 00547760 FaceObjectAwayFromObject(facer, from) [routine 553]
int __fastcall kotor_fastcall_00547760(void*, void*, int, unsigned) {
    U facer_id = 0, from_id = 0;
    if (!pop_object(&facer_id)) return k_pop_failed;                                   // 0054776B..7B
    if (!pop_object(&from_id)) return k_pop_failed;                                    // 0054777D..9B
    void* facer = nullptr; void* from = nullptr;                                       // 00547770..72
    void* o = game_object(facer_id);                                                   // 0054779E..AC
    if (o) facer = as_object(o);                                                       // 005477B1..BC
    o = game_object(from_id);                                                          // 005477BE..CB
    if (o) from = as_object(o);                                                        // 005477D0..DB
    if (facer && from && at<U>(facer, 0x8c) == at<U>(from, 0x8c)) {                   // 005477DD..F9 same area
        Vector v = {0.0f, 0.0f, 0.0f};                                                 // 00547812..22
        Vector t;
        copy_vector(&v, kotor_cdecl_00423bc0(&t, reinterpret_cast<Vector*>(static_cast<unsigned char*>(facer) + 0x90),
                                             reinterpret_cast<Vector*>(static_cast<unsigned char*>(from) + 0x90))); // 005477FF..3F
        // 00547843..62: z*z + x*x + y*y in x87 (53-bit); each float product is exact in double
        const double sum = (static_cast<double>(v.z) * v.z + static_cast<double>(v.x) * v.x) + static_cast<double>(v.y) * v.y;
        if (sum > static_cast<double>(k_min_length_sq_00741870)) {                     // 00547862..6D (C0|C3 or NaN skip)
            Vector n;
            copy_vector(&v, normalize_004ab180(&n, &v));                               // 0054786F..98
            w1x_object_set_orientation_004cd170(facer, nullptr, &v);                   // 00547895..9C
        }
    }
    return 0;                                                                          // 005478A2
}

// ================================================================== 00537820 GetDistanceToObject(object) [routine 41]
// Quirk (original): the caller object is not null-checked (status-only lookup).
int __fastcall kotor_fastcall_00537820(void* self, void*, int, unsigned) {
    float result = -1.0f;                                                              // 00537831 0BF800000h
    U id = 0;
    if (!pop_object(&id)) return k_pop_failed;                                         // 00537824..4B
    if (at<U>(self, 4) == 1) {                                                         // 0053784E..52
        void* caller = nullptr;
        if (static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, at<U>(self, 8), &caller)) == 0) { // 00537858..7C
            Vector from;
            copy_vector(&from, static_cast<unsigned char*>(caller) + 0x90);            // 00537882..9C
            void* caller_area = recovered_object_area_004cb120(caller, nullptr);       // 005378A0..A5
            void* target = nullptr;
            if (static_cast<unsigned char>(kotor_fastcall_004d8230(object_array(), nullptr, id, &target)) == 0 && // 005378A7..CB
                target && type_of(target) > k_type_area_00746633) {                   // 005378CD..DE (jbe)
                Vector to;
                copy_vector(&to, static_cast<unsigned char*>(target) + 0x90);          // 005378E0..F6
                if (recovered_object_area_004cb120(target, nullptr) == caller_area) {  // 005378FA..01
                    Vector d;
                    d.x = fsub(to.x, from.x);                                          // 00537903..10
                    d.y = fsub(to.y, from.y);                                          // 00537914..1C
                    d.z = fsub(to.z, from.z);                                          // 00537920..28
                    result = static_cast<float>(kotor_cdecl_004ab100(&d));             // 0053792C..31
                }
            }
        }
    }
    return push_float(result);                                                         // 00537938..5A
}

// =====================================================================================================
// Reconstructed callees without a callable provider in the c31 map (exact original ABI).
// =====================================================================================================

// 005C8DA0 CSWSSoundObject::SetFixedVariance(float): thiscall, RET 4.  Skips only when equal (fcomp + test ah,44h
// + jnp: an unordered compare proceeds); stores +248, then forwards to the client sound object (tail jump).
void __fastcall kotor_fastcall_005c8da0(void* sound, void*, float variance) {
    if (variance == at<float>(sound, 0x248)) return;                                   // 005C8DA0..AF
    at<float>(sound, 0x248) = variance;                                                // 005C8DB1..B5 (dword move)
    const U client_id = kotor_fastcall_005eda50(client(), nullptr, at<U>(sound, 4));   // 005C8DBB..C8
    void* client_sound = recovered_client_get_sound_object_005ed620(client(), nullptr, client_id); // 005C8DCD..D7
    if (client_sound) kotor_fastcall_0066ad80(client_sound, nullptr, variance);        // 005C8DDC..E2 (jmp)
}

// 0066AD80 CSWCSoundObject::SetFixedVariance(float): thiscall, RET 4.  Without a +168 override the sound source
// (+FC) receives the variance; +124 = variance in both cases.
void __fastcall kotor_fastcall_0066ad80(void* sound, void*, float variance) {
    if (at<U>(sound, 0x168) == 0)                                                      // 0066AD83..8B
        kotor_fastcall_005d5a30(at<void*>(sound, 0xfc), nullptr, variance);            // 0066AD8D..98
    at<float>(sound, 0x124) = variance;                                                // 0066AD9D..A1 / 0066ADAB..AF
}

// 005C8E50 CSWSSoundObject::SetPosition(Vector by value): thiscall, RET 0Ch.  When the position changes, moves the
// object (SetPosition(p, 1, 1, 0)) and sends the new position to every player whose game object is a creature.
void __fastcall kotor_fastcall_005c8e50(void* sound, void*, Vector position) {
    if (aurora_vector_equal_004aa980(&position.x, &at<float>(sound, 0x90))) return;   // 005C8E57..6D
    recovered_object_position_004cd1a0(sound, nullptr, &position, 1, 1, 0);           // 005C8E73..7F
    void* message = recovered_server_sws_message_004aedb0(server_app(), nullptr);      // 005C8E84..8C (ebx)
    void* players = recovered_server_player_list_004aed90(server_app(), nullptr);      // 005C8E91..9C (edi)
    if (!message || !players) return;                                                  // 005C8EA1..A9
    void* cursor = at<void*>(at<void*>(players, 0), 0);                                // 005C8EAB..B1
    if (!cursor) return;                                                               // 005C8EB5
    do {
        void* player = unresolved_list_at_pos_005e9b60(at<void*>(players, 0), nullptr, cursor); // 005C8EB7..BA
        if (player && kotor_fastcall_00561080(player, nullptr) &&                      // 005C8EC1..CE
            as_creature(kotor_fastcall_00561080(player, nullptr)))                     // 005C8ED0..E0
            (void)kotor_fastcall_0056d5d0(message, nullptr, player, at<U>(sound, 4), position); // 005C8EE2..02
        (void)unresolved_list_next_005e9b90(at<void*>(players, 0), nullptr, &cursor);  // 005C8F07..0E
    } while (cursor);                                                                  // 005C8F13..19
}

// 0056D5D0 CSWSMessage::SendServerToPlayerSoundObject_ChangePosition(CSWSPlayer*, id, Vector): thiscall, RET 14h.
// Message 17h minor 5: object id + three 32-bit floats (scale 1.0).
U __fastcall kotor_fastcall_0056d5d0(void* message, void*, void* player, U id, Vector position) {
    kotor_fastcall_004d68a0(message, nullptr, 0x10, 0xffffffffu, 1);                   // 0056D5D4..DC
    kotor_fastcall_00578570(message, nullptr, id);                                     // 0056D5E1..E8
    kotor_fastcall_004d6920(message, nullptr, w5::bits(position.x), 0x3f800000u, 0x20); // 0056D5ED..FB
    kotor_fastcall_004d6920(message, nullptr, w5::bits(position.y), 0x3f800000u, 0x20); // 0056D600..0E
    kotor_fastcall_004d6920(message, nullptr, w5::bits(position.z), 0x3f800000u, 0x20); // 0056D613..21
    void* data = nullptr; U size = 0;
    if (kotor_fastcall_004d6440(message, nullptr, &data, &size) == 0) return 0;        // 0056D626..3F
    return kotor_fastcall_00567e60(message, nullptr, at<U>(player, 4), 0x17, 5, data, size); // 0056D642..63
}

}  // extern "C"
