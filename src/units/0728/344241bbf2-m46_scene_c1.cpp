// lane-m46-3dcore fork C1: Scene vtable 00741708 slot bodies (small/medium) for swkotor.exe GOG K1.
// Evidence: dumpbin /disasm of the original (local d/ADDR.txt); every behavior cites instruction addresses.
// Conventions: D1 (__thiscall -> __fastcall(self, edx_unused, ...)), signatures exactly as the canonical
// stage declarations (3d/render/vtables_gen.h raw-word contract: one U per stack DWORD). Virtual calls go
// through the published typed tables (D41/D42) with the vtable pointer reloaded at every call.
// "EAX:" comments record what the original leaves in EAX; where the original leaves EAX undefined the body
// returns 0 under a D2 label (no faithful value exists; consumers in the stage ignore these results).
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "6c81d8874d-scene3d.h"
#include "74db077be7-m46_scene_externs_c1.h"

namespace {
using namespace kotor_scene3d;
inline const SceneVTable* scene_vt(void* scene) { return *static_cast<const SceneVTable* const volatile*>(scene); }
inline const aurora_model::GobVTable* gob_vt(void* gob) { return *static_cast<const aurora_model::GobVTable* const volatile*>(gob); }
inline U word(const void* p) { return static_cast<U>(reinterpret_cast<std::uintptr_t>(p)); }
template<class T> inline T* ptr(U w) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(w)); }
inline PointerList* room_list(void* scene) { return &field<PointerList>(scene, 0xC8); }    // data +C8, count +CC (00452B15/00452B30)
inline void* room_at(void* scene, I i) { return ptr<void>(field<U*>(scene, 0xC8)[i]); }
inline PointerList* gob_order(void* scene) { return &field<PointerList>(scene, 0x54); }   // lea esi,[ecx+54h] (00453E06)

// .rdata strings read by the original (values from the pinned image).
constexpr char kLyt_007415a0[] = ".lyt";
constexpr char kRoomLine_00741594[] = "%s %f%f%f";
constexpr char kRoomCount_00741588[] = "roomcount";
constexpr char kTrackCount_0074157c[] = "trackcount";
constexpr char kDot_00741480[] = ".";
constexpr char kVisPath_007415e8[] = "%s/%s.VIS";
constexpr char kWrite_0073d868[] = "w";
constexpr char kVisRoom_007415e0[] = "%s %d\n";
constexpr char kVisEntry_007415d8[] = "  %s\n";
constexpr char kVis_00741604[] = ".vis";
constexpr char kNameCount_0073edd0[] = "%s%d";
constexpr char kName_0073d5d0[] = "%s";
constexpr U kTwelve = 0x41400000u;   // 12.0f immediate (0044FB9B)
constexpr U kEight = 0x41000000u;    // 8.0f immediate (0044FBF8)
constexpr U kOneF = 0x3F800000u;     // 1.0f immediate (0044F9A2)
}

extern "C" {
// 0078E3A0 geometry-fade mode: .data initialised word 00000001 (pinned image); writers 0044FD28 only.
volatile U scene_geom_fade_mode_0078e3a0 = 1u;

// 0044F3A0 Scene::EnableAnimations (vtbl+60): mov byte [ecx+DAh],1; ret.
// EAX: not written by the original (D2: 0 returned; undefined in the original).
U __fastcall scene_enable_animations_0044f3a0(void* scene, void*) {
    field<U8>(scene, 0xDA) = 1;                                        // 0044F3A0
    return 0;                                                          // D2 0044F3A7
}

// 0044F780 Scene::GetWindVector(Vector* out, Vector point, raw last) (vtbl+D0), RET 14h.
// Forwards to WindManager::GetGlobalWind on the wind manager at Scene+84 with the vector copied by value.
U __fastcall scene_get_wind_vector_0044f780(void* scene, void*, U out, U x, U y, U z, U last) {
    void* wind = field<void*>(scene, 0x84);                            // 0044F784
    Vector point{bits_to_float(x), bits_to_float(y), bits_to_float(z)}; // 0044F794..0044F7A7 (raw DWORD copies)
    wind_manager_get_global_wind_004a6050(wind, nullptr, ptr<Vector>(out), point, last);   // 0044F7AA
    return out;                                                        // 0044F7AF mov eax,esi
}

// 0044F7E0 Scene::EnableVisibilityGraph (vtbl+88): AL = ([+D4] != 0); word [+D8] = 0.
U8 __fastcall scene_enable_visibility_graph_0044f7e0(void* scene, void*) {
    const U current = field<U>(scene, 0xD4);                           // 0044F7E0
    field<U16>(scene, 0xD8) = 0;                                       // 0044F7EA
    return current != 0 ? 1 : 0;                                       // 0044F7E6/0044F7F3 (EAX zero-extended)
}

// 0044F810 Scene::GetIgnoreVisibilityGraph (vtbl+84): movsx eax, word [+D8].
U __fastcall scene_get_ignore_visibility_graph_0044f810(void* scene, void*) {
    return static_cast<U>(static_cast<I>(static_cast<std::int16_t>(field<U16>(scene, 0xD8))));   // 0044F810
}

// 0044F8B0 Scene::GetNumRooms (vtbl+5C): mov eax,[ecx+CCh].
U __fastcall scene_get_num_rooms_0044f8b0(void* scene, void*) { return field<U>(scene, 0xCC); }

// 0044F8C0 Scene::SetCurrentRoom (vtbl+80): [+D4] = arg; EAX = arg; RET 4.
U __fastcall scene_set_current_room_0044f8c0(void* scene, void*, U room) {
    field<U>(scene, 0xD4) = room;                                      // 0044F8C4
    return room;                                                       // 0044F8C0 (EAX = arg)
}

// 0044F8D0 Scene::SpawnRooms(const char* layout) (vtbl+6C), RET 4: parses <layout>.lyt.
U __fastcall scene_spawn_rooms_0044f8d0(void* scene, void*, U layout_name) {
    const char* const name = ptr<const char>(layout_name);
    void* const descriptor = aurres_get_0044c740(name, kLyt_007415a0, nullptr, 1u);   // 0044F8DE..0044F8EA (memory_only=1, size=NULL)
    if (descriptor == nullptr) return 0;                               // 0044F8F4..0044F902 (EAX = ebp = 0)
    // Frame locals (sub esp,0B0h): L10 room count, L14 track count, L18..L20 position, L24..L30 quaternion,
    // L34 SetPosition out vector, L40 name buffer (0x70 bytes).
    volatile std::int32_t room_count = 0;                              // 0044F905 [esp+10h]
    volatile std::int32_t track_count = 0;                             // 0044F90D [esp+14h]
    float position[3] = {0.0f, 0.0f, 0.0f};                            // 0044F915..0044F925 [esp+18h..20h]
    // D2 (0044F955/0044F9E2): the original never initialises the L40 name buffer; sscanf %s fills it on every
    // matched line, so the zero fill only differs when sscanf stores nothing.
    char room_name[0x70] = {};
    char* line = resource_next_0044bfa0();                             // 0044F92D
    while (line != nullptr) {                                          // 0044F934 / 0044FA80
        while (*line == ' ' || *line == '\t') ++line;                  // 0044F940..0044F94B
        if (room_count > 0) {                                          // 0044F94D..0044F953
            kotor_sscanf_006fae08(line, kRoomLine_00741594, room_name, &position[0], &position[1], &position[2]);   // 0044F955..0044F96F
            const Quaternion orientation{bits_to_float(kOneF), 0.0f, 0.0f, 0.0f};   // 0044F98A..0044F9A2 L24=1.0f, L28..L30=0
            const Vector at{position[0], position[1], position[2]};   // &L18 passed by pointer (0044F97E)
            scene_vt(scene)->s070_scene_spawn_room(scene, nullptr, room_name, &at, &orientation);   // 0044F974/0044F9AA [edx+70h]
            room_count = room_count - 1;                               // 0044F9AD dec [esp+10h]
        }
        if (kotor_stricmp_0070acaf(kRoomCount_00741588, firstword_00463530(line)) == 0)   // 0044F9B1..0044F9C7
            parse_int_004635e0(line + 9, &room_count);                // 0044F9C9..0044F9D2
        if (track_count > 0) {                                         // 0044F9DA..0044F9E0
            kotor_sscanf_006fae08(line, kRoomLine_00741594, room_name, &position[0], &position[1], &position[2]);   // 0044F9E2..0044F9FC
            aurora_model::Gob* gob = aurora_new_aur_object_00449cc0(room_name, room_name, nullptr, nullptr);   // 0044FA01..0044FA0F
            if (gob != nullptr) {                                      // 0044FA19
                gob_vt(gob)->attach_to_scene(gob, nullptr, static_cast<aurora_model::Scene*>(scene));   // 0044FA1D..0044FA22 [eax+4Ch]
                Vector out;                                            // L34 (0044FA40)
                const Vector at{position[0], position[1], position[2]};   // 0044FA25..0044FA3D by-value copy of L18..L20
                gob_vt(gob)->s05c_gob_set_position(gob, nullptr, &out, at);   // 0044FA29/0044FA47 [edx+5Ch]
            }
            track_count = track_count - 1;                             // 0044FA4A dec [esp+14h]
        }
        if (kotor_stricmp_0070acaf(kTrackCount_0074157c, firstword_00463530(line)) == 0)   // 0044FA4E..0044FA64
            parse_int_004635e0(line + 0x0A, &track_count);            // 0044FA66..0044FA6F
        line = resource_next_0044bfa0();                               // 0044FA77
    }
    aurres_release_0044c8f0(descriptor, 0);                            // 0044FA86..0044FA89
    scene_vt(scene)->s094_scene_load_visibility(scene, nullptr, layout_name);   // 0044FA8E..0044FA9D [edx+94h] (original arg)
    return field<U>(scene, 0xCC);                                      // 0044FAA3
}

// 0044FB40 Scene::SetTileLights(const Vector* tile, const Vector* light0, const Vector* light1) (vtbl+28), RET 0Ch.
U __fastcall scene_set_tile_lights_0044fb40(void* scene, void*, U tile_point, U light0_position, U light1_position) {
    void* const* const tile = static_cast<void* const*>(scene_bsp_get_tile_data_0046a680(scene, ptr<const Vector>(tile_point)));   // 0044FB4A..0044FB51
    U eax = 0;
    void* light = tile[0];                                             // 0044FB53
    if (light != nullptr) {                                            // 0044FB58..0044FB5F
        const Vector* position = ptr<const Vector>(light0_position);
        eax = aurora_vector_equal_004aa980(&field<Vector>(light, 0x4C), position);   // 0044FB65..0044FB6A
        if (eax == 0) {                                                // 0044FB74
            field<Vector>(tile[0], 0x4C) = *position;                  // 0044FB76..0044FB88 (three DWORD copies)
            update_static_lighting_0046ca30(scene, tile[0]);           // 0044FB8B..0044FB8F
            field<U>(tile[0], 0x5C) = 1;                               // 0044FB96
            field<U>(tile[0], 0x60) = kTwelve;                         // 0044FB9B 12.0f
            field<U>(tile[0], 0x6C) = 1;                               // 0044FBA4
            field<U>(tile[0], 0x74) = 5;                               // 0044FBAC
            eax = word(tile[0]);                                       // 0044FBA2 (last EAX load on this path)
        }
    }
    light = tile[1];                                                   // 0044FBB3
    eax = word(light);
    if (light != nullptr) {                                            // 0044FBB6..0044FBB8
        const Vector* position = ptr<const Vector>(light1_position);
        eax = aurora_vector_equal_004aa980(&field<Vector>(light, 0x4C), position);   // 0044FBBE..0044FBC3
        if (eax == 0) {                                                // 0044FBCD
            field<Vector>(tile[1], 0x4C) = *position;                  // 0044FBCF..0044FBE2
            update_static_lighting_0046ca30(scene, tile[1]);           // 0044FBE5..0044FBEA
            field<U>(tile[1], 0x5C) = 1;                               // 0044FBF2
            field<U>(tile[1], 0x60) = kEight;                          // 0044FBF8 8.0f
            field<U>(tile[1], 0x6C) = 1;                               // 0044FC02
            field<U>(tile[1], 0x74) = 5;                               // 0044FC0B
            eax = word(tile[1]);                                       // 0044FBF5 (last EAX load on this path)
        }
    }
    return eax;                                                        // 0044FC12..0044FC16
}

// 0044FC80 Scene::ReplaceTileTexture(a, b, c) (vtbl+30), RET 0Ch: cdecl AurTextureReplaceTileReferences(this, a, b, c).
U __fastcall scene_replace_tile_texture_0044fc80(void* scene, void*, U a, U b, U c) {
    return aur_texture_replace_tile_references_004237e0(scene, a, b, c);   // 0044FC80..0044FC95 (EAX passed through)
}

// 0044FCC0 CAurScene::ComputeStaticLighting (vtbl+3C): if (this) tail-jump to vtbl+D4.
// EAX: the original leaves whatever the void slot leaves (D2: 0 returned).
U __fastcall caur_scene_compute_static_lighting_0044fcc0(void* scene, void*) {
    if (scene != nullptr)                                              // 0044FCC0..0044FCC2
        scene_vt(scene)->s0d4_scene_void_return(scene, nullptr);       // 0044FCC4..0044FCC6 jmp [eax+D4h]
    return 0;                                                          // D2 0044FCCC
}

// 0044FD10 Scene::SetGeomFadeProperties(mode) (vtbl+4C), RET 4: accepts 1/2/4 into [0078E3A0].
// EAX: the argument with AL replaced by the result (0044FD10 loads it into EAX; 0044FD23 xor al / 0044FD2D mov al,1).
U __fastcall scene_set_geom_fade_properties_0044fd10(void*, void*, U mode) {
    if (mode == 1 || mode == 2 || mode == 4) {                         // 0044FD14..0044FD21
        scene_geom_fade_mode_0078e3a0 = mode;                          // 0044FD28
        return (mode & 0xFFFFFF00u) | 1u;                              // 0044FD2D
    }
    return mode & 0xFFFFFF00u;                                         // 0044FD23
}

// 0044FD50 Scene::SetBlindLights(value) (vtbl+68), RET 4: [008305E0] = value; EAX = value.
U __fastcall scene_set_blind_lights_0044fd50(void*, void*, U value) {
    scene_black_fog_008305e0 = value;                                  // 0044FD54
    return value;                                                      // 0044FD50
}

// 00452B10 Scene::FindRoom(const char* name) (vtbl+74), RET 4: first room whose name (+78) matches (_stricmp).
U __fastcall scene_find_room_00452b10(void* scene, void*, U name) {
    for (I i = 0; i < field<I>(scene, 0xCC); ++i) {                    // 00452B15..00452B1F / 00452B4A..00452B53 (count reloaded)
        void* room = room_at(scene, i);                                // 00452B30..00452B36
        if (kotor_stricmp_0070acaf(ptr<const char>(name), &field<char>(room, 0x78)) == 0)   // 00452B39..00452B48
            return word(room_at(scene, i));                            // 00452B5D..00452B63
    }
    return 0;                                                          // 00452B57
}

// 00452B70 Scene::SaveVisibility(const char* directory, const char* name) (vtbl+90), RET 8: writes <dir>/<name>.VIS.
// 006FAD9D fopen / 006FAF01 fprintf / 006FACF0 fclose: modern CRT with the same text-mode "w" stream and the same
// %s/%d conversions (equivalence record in parts/C1.json review_needed; D35 runtime rule).
U __fastcall scene_save_visibility_00452b70(void* scene, void*, U directory, U name) {
    const char* dir = ptr<const char>(directory);
    if (dir == nullptr || *dir == '\0') dir = kDot_00741480;           // 00452B7A..00452B87
    char path[0x100];                                                  // sub esp,100h (00452B74)
    format_006fadb0(path, kVisPath_007415e8, dir, ptr<const char>(name));   // 00452B8C..00452B9F
    std::FILE* file = std::fopen(path, kWrite_0073d868);               // 00452BA4..00452BAE
    if (file == nullptr) return 0;                                     // 00452BB8..00452BC4 (EAX = 0)
    for (I i = 0; i < field<I>(scene, 0xCC); ++i) {                    // 00452BC7..00452BD2 / 00452C39..00452C42
        void* room = room_at(scene, i);                                // 00452BE0
        std::fprintf(file, kVisRoom_007415e0, &field<char>(room, 0x78), field<I>(room, 0x60));   // 00452BE3..00452BF1
        for (I j = 0; j < field<I>(room_at(scene, i), 0x60); ++j) {    // 00452BF6..00452C09 / 00452C25..00452C37 (room reloaded)
            void* visible = ptr<void>(field<U*>(room_at(scene, i), 0x5C)[j]);   // 00452C10..00452C13
            std::fprintf(file, kVisEntry_007415d8, &field<char>(visible, 0x78));   // 00452C16..00452C20
        }
    }
    std::fclose(file);                                                 // 00452C45..00452C46
    return 1;                                                          // 00452C50
}

// 00452C60 Scene::GetRoom(index) (vtbl+78), RET 4: bounds-checked room pointer, else 0.
U __fastcall scene_get_room_00452c60(void* scene, void*, U index) {
    const I i = static_cast<I>(index);
    if (i < 0 || i >= field<I>(scene, 0xCC)) return 0;                 // 00452C64..00452C6E / 00452C7C
    return word(room_at(scene, i));                                    // 00452C70..00452C76
}

// 004568D0 Scene::LoadVisibility(const char* name) (vtbl+94), RET 4: parses <name>.vis.
U __fastcall scene_load_visibility_004568d0(void* scene, void*, U name) {
    for (I i = 0; i < field<I>(scene, 0xCC); ++i)                      // 004568DA..004568E7 / 004568FC..00456905
        field<U>(room_at(scene, i), 0x60) = 0;                         // 004568F0..004568F9
    void* const descriptor = aurres_get_0044c740(ptr<const char>(name), kVis_00741604, nullptr, 1u);   // 00456907..00456917
    if (descriptor == nullptr) return 0;                               // 00456921..00456934
    // Frame locals (sub esp,88h): L0C visible count, L14 room name (0x40 bytes), L54 visible room name (0x34 bytes).
    char* line = resource_next_0044bfa0();                             // 00456937
    while (line != nullptr) {                                          // 0045693C / 004569CA
        volatile std::int32_t count = 0;                               // 00456959 [esp+0Ch]
        char room_name[0x40];
        room_name[0] = '\0';                                           // 00456955 byte [esp+14h] = 0
        kotor_sscanf_006fae08(line, kNameCount_0073edd0, room_name, &count);   // 00456945..0045695D
        void* const room = ptr<void>(scene_vt(scene)->s074_scene_find_room(scene, nullptr, word(room_name)));   // 00456962..0045696E [eax+74h]
        if (room != nullptr && count != 0) {                           // 00456973..0045697D
            for (std::int32_t k = 0; k < count; ++k) {                 // 0045697F..00456983 / 004569BC..004569C3 (count reloaded)
                char visible_name[0x34];
                visible_name[0] = '\0';                                // 00456985 byte [esp+54h] = 0
                kotor_sscanf_006fae08(resource_next_0044bfa0(), kName_0073d5d0, visible_name);   // 00456989..00456999
                void* const visible = ptr<void>(scene_vt(scene)->s074_scene_find_room(scene, nullptr, word(visible_name)));   // 0045699E..004569AA
                if (visible != nullptr)                                // 004569AD..004569AF
                    scene_set_visibility_00454940(scene, nullptr, room, visible, 1u);   // 004569B1..004569B7
            }
        }
        line = resource_next_0044bfa0();                               // 004569C5
    }
    aurres_release_0044c8f0(descriptor, 0);                            // 004569D7..004569D9
    return 1;                                                          // 004569E3
}

// 00453E00 Scene::RenderGobFirst(gob) (vtbl+98), RET 4: remove then insert at index 0 in the list at Scene+54.
U __fastcall scene_render_gob_first_00453e00(void* scene, void*, U gob) {
    texture_list_remove_00445080(gob_order(scene), nullptr, gob);      // 00453E06..00453E0C
    return list_insert_00445020(gob_order(scene), nullptr, gob, 0);    // 00453E11..00453E16 (EAX from Insert)
}

// 00453E20 Scene::RenderGobFirst (second body, vtbl+9C), RET 4: remove then append (inline Add with 005E03C0 growth).
U __fastcall scene_render_gob_first_00453e20(void* scene, void*, U gob) {
    PointerList* list = gob_order(scene);
    texture_list_remove_00445080(list, nullptr, gob);                  // 00453E26..00453E2C
    const U capacity = field<U>(list, 8);
    if (field<U>(list, 4) == capacity)                                 // 00453E31..00453E37
        menu_list_growth::allocate_005e03c0(list, nullptr, capacity != 0 ? capacity + capacity : 8u);   // 00453E39..00453E49
    field<U*>(list, 0)[field<U>(list, 4)] = gob;                       // 00453E4E..00453E53
    const U count = field<U>(list, 4) + 1;                             // 00453E56..00453E59
    field<U>(list, 4) = count;                                         // 00453E5B
    return count;                                                      // EAX = new count (00453E59)
}

// 00453E70 Scene::RenderGobAfter(gob, anchor) (vtbl+A0), RET 8: index of anchor looked up BEFORE the removal,
// -1 when absent; gob removed and inserted at index+1.
U __fastcall scene_render_gob_after_00453e70(void* scene, void*, U gob, U anchor) {
    PointerList* list = gob_order(scene);
    I index = -1;                                                      // 00453E91
    const I count = field<I>(list, 4);
    const U* data = field<U*>(list, 0);
    for (I i = 0; i < count; ++i)                                      // 00453E75..00453E8F
        if (data[i] == anchor) { index = i; break; }                   // 00453E85..00453E87 / 00453EB0
    texture_list_remove_00445080(list, nullptr, gob);                  // 00453E94..00453E9B
    return list_insert_00445020(list, nullptr, gob, index + 1);        // 00453EA0..00453EA5
}

// 00453EC0 Scene::RenderGobBefore(gob, anchor) (vtbl+A4), RET 8: as RenderGobAfter but inserts at the anchor's
// pre-removal index; when the anchor is absent the original passes -1 to Insert (quirk preserved, 00453EE1/00453EF0).
U __fastcall scene_render_gob_before_00453ec0(void* scene, void*, U gob, U anchor) {
    PointerList* list = gob_order(scene);
    I index = -1;                                                      // 00453EE1
    const I count = field<I>(list, 4);
    const U* data = field<U*>(list, 0);
    for (I i = 0; i < count; ++i)                                      // 00453EC5..00453EDF
        if (data[i] == anchor) { index = i; break; }                   // 00453ED5..00453ED7 / 00453EFF
    texture_list_remove_00445080(list, nullptr, gob);                  // 00453EE4..00453EEB
    return list_insert_00445020(list, nullptr, gob, index);            // 00453EF0..00453EF4
}

// 00458B40 Scene::SetFogColor(const Vector*) (vtbl+08), RET 4: copies 3 DWORDs to Scene+74; EAX = third DWORD.
U __fastcall scene_set_fog_color_00458b40(void* scene, void*, U color) {
    const U* source = ptr<const U>(color);
    field<U>(scene, 0x74) = source[0];                                 // 00458B44..00458B49
    field<U>(scene, 0x78) = source[1];                                 // 00458B4B..00458B4E
    const U last = source[2];                                          // 00458B51
    field<U>(scene, 0x7C) = last;                                      // 00458B54
    return last;
}

// 00458B60 Scene::GetFogRange(U* start, U* end) (vtbl+40), RET 8: immediate out writes; EAX = [+68].
U __fastcall scene_get_fog_range_00458b60(void* scene, void*, U start_out, U end_out) {
    const U start = field<U>(scene, 0x64);                             // 00458B60
    *ptr<U>(start_out) = start;                                        // 00458B67
    const U end = field<U>(scene, 0x68);                               // 00458B69
    *ptr<U>(end_out) = end;                                            // 00458B70
    return end;
}

// 00458B80 Scene::SetFogRange(start, end) (vtbl+44), RET 8: raw DWORD stores; EAX = start.
U __fastcall scene_set_fog_range_00458b80(void* scene, void*, U start, U end) {
    field<U>(scene, 0x64) = start;                                     // 00458B88
    field<U>(scene, 0x68) = end;                                       // 00458B8B
    return start;                                                      // 00458B80
}

// 00458BC0 Scene::IsGeomFadeEnabled (vtbl+50): AL = ([0078E3A0] == 1).
U8 __fastcall scene_is_geom_fade_enabled_00458bc0(void*, void*) {
    return scene_geom_fade_mode_0078e3a0 == 1u ? 1 : 0;                // 00458BC0..00458BC7
}

// 00458ED0 Scene scalar deleting destructor (vtbl+00), RET 4: ~Scene 00458BE0, then operator delete 006FA390 when
// bit 0 of the flags is set; EAX = this.
U __fastcall scene_destruct_scene_00458ed0(void* scene, void*, U flags) {
    scene_destruct_00458be0(scene, nullptr);                           // 00458ED1..00458ED3
    if ((flags & 1u) != 0) live_free(scene);                           // 00458ED8..00458EE0 (M41 allocator family, 006FA390)
    return word(scene);                                                // 00458EE8
}
} // extern "C"
