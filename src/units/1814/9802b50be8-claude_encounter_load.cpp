// claude_encounter_load.cpp (lane "encounter", load path): CSWSEncounter GFF / template loading and the area loader
// CSWSArea::LoadEncounters, transcribed from the original swkotor.exe (read-only, capstone).
//
//   00590410 CSWSEncounter::LoadEncounterSpawnPoints(CResGFF*, CResList*)   RET 8    @kotor_fastcall_00590410@16
//   00590580 CSWSEncounter::LoadEncounterGeometry(CResGFF*, CResList*)      RET 8    @kotor_fastcall_00590580@16
//   00590820 CSWSEncounter::ReadEncounterScriptsFromGff(CResGFF*, CResStruct*) RET 8 @kotor_fastcall_00590820@16
//   00592430 CSWSEncounter::ReadEncounterFromGff(CResGFF*, CResStruct*, int default_geometry, Vector* position)
//                                                                            RET 10h  @kotor_fastcall_00592430@24
//   00593830 CSWSEncounter::LoadEncounter(CResGFF*, CResStruct*)            RET 8    @kotor_fastcall_00593830@16
//   00593A90 CSWSEncounter::LoadFromTemplate(CResRef*, int default_geometry, Vector* position) RET 0Ch
//                                                                                     @kotor_fastcall_00593a90@20
//   00505060 CSWSArea::LoadEncounters(CResGFF*, CResStruct*, int load_state, int from_template) RET 10h
//                                                                                     @kotor_fastcall_00505060@24
// Note: the "CResList*" of LoadEncounterSpawnPoints / LoadEncounterGeometry is the list filled by the caller's GetList
// (the header names it `void* s`).
// Quirks kept (all original):
//   * LoadEncounters: when LoadEncounterGeometry fails the encounter is deleted and EDI = 0, but a present
//     SpawnPointList is still loaded through LoadEncounterSpawnPoints with this == nullptr (00505285..AA).
//   * LoadEncounterGeometry frees +290 and leaves it dangling when the list is empty (0 vertices -> return 0); vertices
//     of failed list elements stay zero (Vector ctor), their index slots stay uninitialised.
//   * ReadEncounterFromGff: AreaList elements are always read from list index 3 (00592F84 PUSH 3), the +284 buffer is
//     sized from AreaListMaxSize (+280) not from the list count; +2B0 / +290 / +284 are freed but not cleared before the
//     new allocation; a failed SpawnList entry allocation still dereferences / appends the null entry.
//   * Bubble sort of the creature list (descending CR +10) in both ReadEncounterFromGff and LoadEncounter; the swapped
//     float fields go through fld/fstp dword; the CopyToString/CStr loops compute an unused string.
//   * LoadFromTemplate dereferences a failed CResGFF allocation and formats a "doesn't exist" string that is never used.
//   * ReadEncounterScriptsFromGff passes the 16-byte CResRef result to CExoString(char const*) (self_return 00641DB0).
#include "d8bbdade43-claude_encounter_common.hpp"

extern "C" {
// ---- existing providers (c40 map; object in brackets) ----
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, unsigned type, const char* ext, const void* ref); // CResGFF [0289]
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out_struct);                      // GetTopLevelStruct [claude_dialogue_start]
void* __fastcall gui_load_gff_string_00411ec0(void* gff, void*, void* out, void* s, const char* label, U* ok, void* fallback);
void* __fastcall recovered_gff_localized_00411fd0(void* gff, void*, void* out, void* s, const char* label, unsigned* ok, void* fallback);
void* __fastcall gui_load_gff_resref_00411e10(void* gff, void*, void* out, void* s, const char* label, unsigned* ok, void* fallback);
int __fastcall gui_load_gff_int_00411c90(void* gff, void*, void* s, const char* label, unsigned* ok, int fallback);
U __fastcall gui_load_gff_dword_00411c20(void* gff, void*, void* s, const char* label, U* ok, U fallback);
unsigned char __fastcall gui_load_gff_byte_00411a60(void* gff, void*, void* s, const char* label, U* ok, U fallback);
float __fastcall gui_load_gff_float_00411d00(void* gff, void*, void* s, const char* label, unsigned* ok, float fallback);
U __fastcall gff_list_004118c0(void* gff, void*, void* list, void* s, const char* label);
U __fastcall gff_list_element_00411990(void* gff, void*, void* out, void* list, U index);
unsigned __fastcall gff_list_count_00411940(void* gff, void*, void* list);
void __fastcall w2x_object_state_004d1cf0(void* object, void*, void* gff, void* s);             // LoadObjectState [0886]
void __fastcall kotor_fastcall_00553040(void* object, void*, void* tag);                       // CSWSObject::SetTag [claude_w1_route_bodies]
void __fastcall recovered_object_position_004cd1a0(void* object, void*, const Vector* p, U a, U b, U c); // SetPosition
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, const void* string);     // CResRef(CExoString const&) [0200]
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*, void*);                               // self_return 00641DB0 [0832]
void __stdcall vector_constructor_iterator_00401130(void* first, U size, I count, void* construct); // `vector ctor iterator' [0526]
void* __fastcall vector_default_construct_00409e70(void* v, void*);                             // Vector::Vector (zero) [0526]
}

namespace {
using namespace w5;
using namespace enc;
const float k_half_size_00749d80 = 7.5f;                 // .rdata 00749D80 (default square half-extent)
inline void ref_empty(void* ref) {                       // CResRef::CResRef() + operator=("") (0058FF80 shape)
    ResRef discard; kotor_fastcall_00405ed0(ref, nullptr); unresolved_ref_assign_00406290(ref, nullptr, &discard, "");
}
inline void ref_copy(void* dst, const void* src) { ResRef discard; ref_assign_copy_00406120(dst, nullptr, &discard, src); }
inline void ref_from_text(void* ref, const char* text) {
    static_cast<menu_resources::Ref*>(ref)->construct_00406d80(text);
}
inline EncounterListEntry* creatures(void* self) { return at<EncounterListEntry*>(self, 0x2b0); }
inline float x87_float(float v) { return static_cast<float>(static_cast<double>(v)); }   // fld dword / fstp dword
// delete[] of a CEncounterListEntry array (count cookie at -4; dtor = void_return 004015A0)
inline void delete_entries(EncounterListEntry* array) {
    unsigned char* block = reinterpret_cast<unsigned char*>(array) - 4;
    c2da_destroy_array_006fa852(array, 0x24, *reinterpret_cast<I*>(block),
                                reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0));
    live_free(block);
}
// The in-place bubble sort of the creature list by CR (+10) descending, shared shape of 00592B3F..CA9 / 005938C0..9EF.
// temp entry: resref, +10, +14, +20 copied; +18 / +1C zero.
void sort_creatures(void* self) {
    EncounterListEntry temp;                                                                     // [esp+4C] / [esp+48]
    ref_empty(temp.resref);
    temp.f10 = 0; temp.cr = 0.0f; temp.f20 = 0; temp.f18 = 0; temp.f1c = 0;
    for (I i = 0; i < at<I>(self, 0x2b4); ++i) {                                                 // outer (signed, re-read)
        for (I k = 0; k < at<I>(self, 0x2b4) - 1; ++k) {                                         // inner (signed, re-read)
            EncounterListEntry* e = creatures(self) + k;
            if (!(reinterpret_cast<float&>(e[0].f10) < reinterpret_cast<float&>(e[1].f10))) continue; // fcomp; test ah,5; jp
            temp.f10 = e[0].f10;
            ref_copy(temp.resref, e[0].resref);
            temp.cr = e[0].cr; temp.f20 = e[0].f20;
            e[0].f10 = e[1].f10;
            ref_copy(e[0].resref, e[1].resref);
            e[0].cr = e[1].cr; e[0].f20 = e[1].f20; e[0].f1c = 0; e[0].f18 = 0;
            reinterpret_cast<float&>(e[1].f10) = x87_float(reinterpret_cast<float&>(temp.f10));
            ref_copy(e[1].resref, temp.resref);
            e[1].cr = x87_float(temp.cr);
            e[1].f20 = temp.f20; e[1].f1c = 0; e[1].f18 = 0;
        }
    }
}
// CExoString s; for each creature: CopyToString(&s); CStr() (result unused); then +2D0 = last entry's +14.
void creature_names_and_minimum(void* self) {
    ExoString name; kotor_movie::string_element_construct_005b3190(&name, nullptr);
    for (I i = 0; i < at<I>(self, 0x2b4); ++i) {
        c2da_ref_to_string_00405f70(creatures(self)[i].resref, nullptr, &name);
        (void)frame_LogCStr_005e5670(&name, nullptr);
    }
    const I count = at<I>(self, 0x2b4);
    if (count > 0) at<float>(self, 0x2d0) = creatures(self)[count - 1].cr;
    frame_LogStringDtor_005e5c20(&name, nullptr);
}
}  // namespace

extern "C" {

// 00590410 CSWSEncounter::LoadEncounterSpawnPoints(CResGFF*, CResList*): RET 8. EAX = the spawn point count (+2BC).
U __fastcall kotor_fastcall_00590410(void* self, void*, void* gff, void* list) {
    const I count = static_cast<I>(gff_list_count_00411940(gff, nullptr, list));               // 00590434..37
    at<I>(self, 0x2bc) = count;                                                                  // 0059043E
    if (count <= 0) return static_cast<U>(count);                                                // 00590444 (signed)
    if (at<void*>(self, 0x2b8) != nullptr) live_free(at<void*>(self, 0x2b8));                    // 0059044A..5A (not cleared)
    const I n = at<I>(self, 0x2bc);
    void* points = live_operator_new(static_cast<U>(n) << 4);                                    // 00590466..6B
    if (points != nullptr)
        vector_constructor_iterator_00401130(points, 0x10, n, reinterpret_cast<void*>(&kotor_fastcall_0058ffc0)); // 00590485..8E
    at<void*>(self, 0x2b8) = points;                                                             // 005904A9
    for (I i = 0; i < at<I>(self, 0x2bc); ++i) {                                                 // 005904B7..64
        U element[4] = {};                                                                       // argument slot reuse in the original
        unsigned ok = 0;
        if (gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i)) == 0) continue; // 005904BB..CB
        unsigned char* p = at<unsigned char*>(self, 0x2b8) + i * 0x10;
        at<float>(p, 0x0) = gui_load_gff_float_00411d00(gff, nullptr, element, "X", &ok, 0.0f);           // 005904D1..F1 ([007476E8])
        at<float>(at<unsigned char*>(self, 0x2b8) + i * 0x10, 0x4) =
            gui_load_gff_float_00411d00(gff, nullptr, element, "Y", &ok, 0.0f);                  // 005904EF..12 ([0074699C])
        at<float>(at<unsigned char*>(self, 0x2b8) + i * 0x10, 0x8) =
            gui_load_gff_float_00411d00(gff, nullptr, element, "Z", &ok, 0.0f);                  // 00590510..34 ([00747A88])
        at<float>(at<unsigned char*>(self, 0x2b8) + i * 0x10, 0xc) =
            gui_load_gff_float_00411d00(gff, nullptr, element, "Orientation", &ok, 0.0f);        // 00590532..54 ([00747590])
    }
    return at<U>(self, 0x2bc);                                                                   // 00590558 (EAX = re-read count)
}

// 00590580 CSWSEncounter::LoadEncounterGeometry(CResGFF*, CResList*): RET 8, EAX 1 (0 when there are no vertices).
U __fastcall kotor_fastcall_00590580(void* self, void*, void* gff, void* list) {
    const U count = gff_list_count_00411940(gff, nullptr, list);                                 // 005905A5..A8
    at<U>(self, 0x294) = count;                                                                  // 005905AD
    at<U>(self, 0x29c) = count;                                                                  // 005905B3
    if (at<void*>(self, 0x290) != nullptr) live_free(at<void*>(self, 0x290));                    // 005905B9..C9 (dangling)
    const U n = at<U>(self, 0x294);
    if (n == 0) {                                                                                // 005905D2..D4
        ExoString message; kotor_movie::string_element_construct_005b3190(&message, nullptr);    // 005905D6..DA
        frame_LogFormat_005e5680(&message,
            "Problem loading encounter with tag '%s'.  It has geometry, but no vertices.  Skipping.",
            frame_LogCStr_005e5670(enc::bytes(self) + 0x18, nullptr));                           // 005905DF..FB ([00749B50])
        frame_LogStringDtor_005e5c20(&message, nullptr);                                         // 005905FE..0A
        return 0;
    }
    void* vertices = live_operator_new(n * 12);                                                  // 00590625..32
    if (vertices != nullptr)
        vector_constructor_iterator_00401130(vertices, 0xc, static_cast<I>(n),
                                             reinterpret_cast<void*>(&vector_default_construct_00409e70)); // 00590647..50
    at<void*>(self, 0x290) = vertices;                                                           // 00590669
    if (at<void*>(self, 0x298) != nullptr) live_free(at<void*>(self, 0x298));                    // 00590659..77
    at<void*>(self, 0x298) = live_operator_new(at<U>(self, 0x29c) << 2);                         // 0059067A..89
    U first = 1;                                                                                 // [esp+14]
    for (I i = 0; i < at<I>(self, 0x294); ++i) {                                                 // 005906B0..F1 (signed)
        U element[4] = {};                                                                       // [esp+30]
        unsigned ok = 0;                                                                         // [esp+10]
        if (gff_list_element_00411990(gff, nullptr, element, list, static_cast<U>(i)) == 0) continue; // 005906B4..C4
        Vector* v = at<Vector*>(self, 0x290) + i;
        v->x = static_cast<float>(static_cast<double>(gui_load_gff_float_00411d00(gff, nullptr, element, "X", &ok, 0.0f)) +
                                  static_cast<double>(at<float>(self, 0x90)));                   // 005906CA..F4 (fadd dword)
        (at<Vector*>(self, 0x290) + i)->y =
            static_cast<float>(static_cast<double>(gui_load_gff_float_00411d00(gff, nullptr, element, "Y", &ok, 0.0f)) +
                               static_cast<double>(at<float>(self, 0x94)));                      // 005906EE..1B
        (at<Vector*>(self, 0x290) + i)->z =
            static_cast<float>(static_cast<double>(gui_load_gff_float_00411d00(gff, nullptr, element, "Z", &ok, 0.0f)) +
                               static_cast<double>(at<float>(self, 0x98)));                      // 00590715..3D
        at<I*>(self, 0x298)[i] = i;                                                              // 00590741..47
        v = at<Vector*>(self, 0x290) + i;
        if (first != 0) {                                                                        // 0059074A..56
            at<float>(self, 0x2a0) = v->x; at<float>(self, 0x2a4) = v->x;                        // 00590758..66
            at<float>(self, 0x2a8) = v->y; at<float>(self, 0x2ac) = v->y;                        // 0059076C..DF
            first = 0;                                                                           // 00590778
            continue;
        }
        if (v->x < at<float>(self, 0x2a0)) at<float>(self, 0x2a0) = v->x;                        // 00590782..96 (test ah,5; jp)
        if (v->x > at<float>(self, 0x2a4)) at<float>(self, 0x2a4) = v->x;                        // 0059079C..AD (test ah,41h; jne)
        if (v->y < at<float>(self, 0x2a8)) at<float>(self, 0x2a8) = v->y;                        // 005907B3..C6
        if (v->y > at<float>(self, 0x2ac)) at<float>(self, 0x2ac) = v->y;                        // 005907CC..DF
    }
    return 1;
}

// 00590820 CSWSEncounter::ReadEncounterScriptsFromGff(CResGFF*, CResStruct*): RET 8. The five script resrefs default to
// the current script names. EAX is not set deliberately by the original (left from ~CExoString); no caller reads it.
U __fastcall kotor_fastcall_00590820(void* self, void*, void* gff, void* s) {
    static const char* const labels[5] = {"OnEntered", "OnExit", "OnHeartbeat", "OnExhausted", "OnUserDefined"};
    // [00749BB4] [00747A2C] [00747A4C] [00749BA8] [00747A3C]; strings +2E8 / +2F0 / +2F8 / +300 / +308
    for (U k = 0; k < 5; ++k) {
        void* script = enc::bytes(self) + 0x2e8 + 8 * k;
        ResRef fallback; async_ref_construct_string_00406d60(&fallback, nullptr, script);         // 00590849..4E
        ResRef value; unsigned ok = 0;
        void* r = gui_load_gff_resref_00411e10(gff, nullptr, &value, s, labels[k], &ok, &fallback); // 00590859..70
        const char* text = static_cast<const char*>(kotor_m15e_slot00c_self_00641db0(r, nullptr)); // 00590875..77
        ExoString temp; frame_LogStringCtor_005e5a90(&temp, nullptr, text);                      // 0059087C..81
        async_assign_string_005e5c50(script, nullptr, &temp);                                    // 00590886..99
        frame_LogStringDtor_005e5c20(&temp, nullptr);                                            // 0059089E..AA
    }
    return 0;
}

// 00592430 CSWSEncounter::ReadEncounterFromGff(CResGFF*, CResStruct*, int default_geometry, Vector* position): RET 10h.
// EAX 1, or 0 when a Geometry list is present but LoadEncounterGeometry fails.
U __fastcall kotor_fastcall_00592430(void* self, void*, void* gff, void* s, U default_geometry, U position_arg) {
    unsigned ok = 0;                                                                             // [esp+10]
    at<U>(self, 0x238) = gui_load_gff_byte_00411a60(gff, nullptr, s, "Active", &ok, at<U8>(self, 0x238)); // 0059245E..89
    at<U>(self, 0x23c) = gui_load_gff_byte_00411a60(gff, nullptr, s, "Reset", &ok, at<U8>(self, 0x23c)); // 0059247D..B5
    at<I>(self, 0x240) = gui_load_gff_int_00411c90(gff, nullptr, s, "ResetTime", &ok, at<I>(self, 0x240)); // 005924A1..C7
    at<I>(self, 0x2c4) = gui_load_gff_int_00411c90(gff, nullptr, s, "Respawns", &ok, at<I>(self, 0x2c4)); // 005924C0..F3
    at<I>(self, 0x244) = gui_load_gff_int_00411c90(gff, nullptr, s, "SpawnOption", &ok, at<I>(self, 0x244)); // 005924DF..FE
    at<I>(self, 0x254) = gui_load_gff_int_00411c90(gff, nullptr, s, "MaxCreatures", &ok, at<I>(self, 0x254)); // 00592504..24
    at<I>(self, 0x250) = gui_load_gff_int_00411c90(gff, nullptr, s, "RecCreatures", &ok, at<I>(self, 0x250)); // 0059251D..44
    at<U>(self, 0x318) = gui_load_gff_byte_00411a60(gff, nullptr, s, "PlayerOnly", &ok, at<U8>(self, 0x318)); // 0059253C..74
    at<U>(self, 0x22c) = gui_load_gff_dword_00411c20(gff, nullptr, s, "Faction", &ok, at<U>(self, 0x22c)); // 0059255D..81
    at<I>(self, 0x24c) = gui_load_gff_int_00411c90(gff, nullptr, s, "DifficultyIndex", &ok, -1); // 0059257F..99
    void* table = at<void*>(two_da(), 0xa8);                                                     // 0059259F..AB encdifficulty
    if (table != nullptr) {                                                                      // 005925B1..B3
        ExoString column; frame_LogStringCtor_005e5a90(&column, nullptr, "VALUE");               // 005925B5..BE ([00749BC0])
        (void)c2da_current_get_int_00413660(table, nullptr, at<U>(self, 0x24c), &column, &at<I>(self, 0x248)); // 005925C3..E3
        frame_LogStringDtor_005e5c20(&column, nullptr);                                          // 005925E8..F7
    } else {
        at<I>(self, 0x248) = gui_load_gff_int_00411c90(gff, nullptr, s, "Difficulty", &ok, at<I>(self, 0x248)); // 005925FE..17
    }
    Vector position = {0.0f, 0.0f, 0.0f};                                                        // [esp+34] 00592631..41
    position.x = gui_load_gff_float_00411d00(gff, nullptr, s, "XPosition", &ok, at<float>(self, 0x90)); // 0059261D..4E
    position.y = gui_load_gff_float_00411d00(gff, nullptr, s, "YPosition", &ok, at<float>(self, 0x94)); // 00592652..6B
    position.z = gui_load_gff_float_00411d00(gff, nullptr, s, "ZPosition", &ok, at<float>(self, 0x98)); // 0059266F..88
    recovered_object_position_004cd1a0(self, nullptr, &position, 1, 1, 0);                       // 0059268C..99
    const Vector* override_position = reinterpret_cast<const Vector*>(static_cast<std::uintptr_t>(position_arg));
    if (override_position != nullptr) recovered_object_position_004cd1a0(self, nullptr, override_position, 1, 1, 0); // 0059269E..B2
    {
        unsigned char name[8];                                                                   // [esp+1C] CExoLocString
        void* l = recovered_gff_localized_00411fd0(gff, nullptr, name, s, "LocalizedName", &ok, enc::bytes(self) + 0x230); // 005926B7..D0
        recovered_localized_assign_005e9e70(enc::bytes(self) + 0x230, nullptr, l);               // 005926D5..E3
        exo_locstring_destroy_005ea000(name, nullptr);                                           // 005926E8..F7
    }
    {
        ExoString tag;                                                                           // [esp+1C]
        void* t = gui_load_gff_string_00411ec0(gff, nullptr, &tag, s, "Tag", &ok, enc::bytes(self) + 0x18); // 005926FC..12
        kotor_fastcall_00553040(self, nullptr, t);                                               // 00592717..25
        frame_LogStringDtor_005e5c20(&tag, nullptr);                                             // 0059272A..39
    }
    (void)recovered_module_register_tag_004c7de0(server_get_module_004ae6b0(server(), nullptr), nullptr,
                                                  enc::bytes(self) + 0x18, at<U>(self, 4));      // 0059273E..53
    unsigned char geometry_list[0x14] = {};                                                      // CResList [esp+DC]
    if (gff_list_004118c0(gff, nullptr, geometry_list, s, "Geometry") != 0) {                    // 00592758..6F ([007474BC])
        const U loaded = kotor_fastcall_00590580(self, nullptr, gff, geometry_list);             // 00592771..7C
        if (loaded == 0) return loaded;                                                          // 00592781..89 -> 005931AE (EAX 0)
    } else if (default_geometry != 0) {                                                          // 0059278E..97
        at<U>(self, 0x294) = 4; at<U>(self, 0x29c) = 4;                                          // 0059279D..A8
        if (at<void*>(self, 0x290) != nullptr) live_free(at<void*>(self, 0x290));                // 005927AE..BE (not cleared)
        const U nv = at<U>(self, 0x294);
        void* vertices = live_operator_new(nv * 12);                                             // 005927C1..D3
        if (vertices != nullptr)
            vector_constructor_iterator_00401130(vertices, 0xc, static_cast<I>(nv),
                                                 reinterpret_cast<void*>(&vector_default_construct_00409e70)); // 005927E9..F2
        at<void*>(self, 0x290) = vertices;                                                       // 005927FF
        if (at<void*>(self, 0x298) != nullptr) live_free(at<void*>(self, 0x298));                // 00592805..20
        at<void*>(self, 0x298) = live_operator_new(at<U>(self, 0x29c) << 2);                     // 00592823..38
        const float x = at<float>(self, 0x90), y = at<float>(self, 0x94), z = at<float>(self, 0x98); // 00592832..62
        const double d = k_half_size_00749d80;
        const float x0 = static_cast<float>(static_cast<double>(x) - d);                         // 0059284E..66 fsub/fst
        const float y0 = static_cast<float>(static_cast<double>(y) - d);                         // 0059286F..79
        const float x1 = static_cast<float>(static_cast<double>(x) + d);                         // 0059287F..89
        const float y1 = static_cast<float>(static_cast<double>(y) + d);                         // 0059288F..9D
        at<float>(self, 0x2a0) = x0; at<float>(self, 0x2a8) = y0;
        at<float>(self, 0x2a4) = x1; at<float>(self, 0x2ac) = y1;
        Vector* v = at<Vector*>(self, 0x290);
        I* index = at<I*>(self, 0x298);
        v[0].x = x0; v[0].y = y0; v[0].z = z; index[0] = 0;                                      // 005928A3..C5
        v[1].x = x1; v[1].y = y0; v[1].z = z; index[1] = 1;                                      // 005928CB..F2
        v[2].x = x1; v[2].y = y1; v[2].z = z; index[2] = 2;                                      // 005928F9..22
        v[3].x = x0; v[3].y = y1; v[3].z = z; index[3] = 3;                                      // 00592929..52
    }
    unsigned char creature_list[0x14] = {};                                                      // CResList [esp+94]
    if (gff_list_004118c0(gff, nullptr, creature_list, s, "CreatureList") != 0) {                // 00592959..70 ([00749CE8])
        at<U>(self, 0x2b4) = gff_list_count_00411940(gff, nullptr, creature_list);               // 00592976..85
        if (creatures(self) != nullptr) delete_entries(creatures(self));                         // 0059298B..AF (not cleared)
        const I n = at<I>(self, 0x2b4);
        unsigned char* block = static_cast<unsigned char*>(live_operator_new(static_cast<U>(n) * 0x24 + 4)); // 005929B2..C3
        EncounterListEntry* array = nullptr;
        if (block != nullptr) {                                                                  // 005929CF..DC
            *reinterpret_cast<I*>(block) = n;                                                    // 005929E9
            array = reinterpret_cast<EncounterListEntry*>(block + 4);
            crt_eh_vector_constructor_006fb55f(array, 0x24, n, reinterpret_cast<void*>(&kotor_fastcall_0058ff80),
                reinterpret_cast<void*>(&kotor_m15e_slot090_void0_004015a0));                    // 005929DE..F5
        }
        at<EncounterListEntry*>(self, 0x2b0) = array;                                            // 00592A02
        for (I i = 0; i < at<I>(self, 0x2b4); ++i) {                                             // 00592A30..1D (signed)
            U element[4] = {};                                                                   // [esp+2C]
            if (gff_list_element_00411990(gff, nullptr, element, creature_list, static_cast<U>(i)) == 0) continue; // 00592A30..4B
            ResRef fallback; ref_from_text(&fallback, "");                                       // 00592A51..5A
            ResRef value;
            void* r = gui_load_gff_resref_00411e10(gff, nullptr, &value, element, "ResRef", &ok, &fallback); // 00592A5F..7D
            ref_copy(creatures(self)[i].resref, r);                                              // 00592A82..90
            reinterpret_cast<float&>(creatures(self)[i].f10) =
                gui_load_gff_float_00411d00(gff, nullptr, element, "CR", &ok, 0.0f);             // 00592A95..B3
            EncounterListEntry* e = creatures(self) + i;                                         // [esp+18]
            e->cr = static_cast<float>(kotor_fastcall_005919e0(self, nullptr, reinterpret_cast<float&>(e->f10))); // 00592AB7..D5
            creatures(self)[i].f20 = gui_load_gff_byte_00411a60(gff, nullptr, element, "SingleSpawn", &ok, 0); // 00592AD3..F7
            creatures(self)[i].f18 = 0;                                                          // 00592AFB..01
        }
    }
    sort_creatures(self);                                                                        // 00592B23..CA9
    creature_names_and_minimum(self);                                                            // 00592CAF..D2C
    unsigned char spawn_list[0x14] = {};                                                         // CResList [esp+F0]
    if (gff_list_004118c0(gff, nullptr, spawn_list, s, "SpawnPointList") != 0)                   // 00592D32..49 ([007474AC])
        (void)kotor_fastcall_00590410(self, nullptr, gff, spawn_list);                           // 00592D4B..56
    at<I>(self, 0x264) = gui_load_gff_int_00411c90(gff, nullptr, s, "NumberSpawned", &ok, at<I>(self, 0x264)); // 00592D5B..88
    at<U>(self, 0x268) = gui_load_gff_dword_00411c20(gff, nullptr, s, "HeartbeatDay", &ok, at<U>(self, 0x268)); // 00592D74..93
    at<U>(self, 0x26c) = gui_load_gff_dword_00411c20(gff, nullptr, s, "HeartbeatTime", &ok, at<U>(self, 0x26c)); // 00592D99..B9
    at<U>(self, 0x270) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastSpawnDay", &ok, at<U>(self, 0x270)); // 00592DB2..E5
    at<U>(self, 0x274) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastSpawnTime", &ok, at<U>(self, 0x274)); // 00592DD1..F0
    at<U>(self, 0x2e0) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastEntered", &ok, at<U>(self, 0x2e0)); // 00592DF6..16
    at<U>(self, 0x2e4) = gui_load_gff_dword_00411c20(gff, nullptr, s, "LastLeft", &ok, at<U>(self, 0x2e4)); // 00592E0F..3A
    at<U>(self, 0x278) = gui_load_gff_byte_00411a60(gff, nullptr, s, "Started", &ok, at<U8>(self, 0x278)); // 00592E2E..5E
    at<U>(self, 0x27c) = gui_load_gff_byte_00411a60(gff, nullptr, s, "Exhausted", &ok, at<U8>(self, 0x27c)); // 00592E4F..8A
    at<I>(self, 0x2c8) = gui_load_gff_int_00411c90(gff, nullptr, s, "CurrentSpawns", &ok, at<I>(self, 0x2c8)); // 00592E73..95
    at<I>(self, 0x314) = gui_load_gff_int_00411c90(gff, nullptr, s, "CustomScriptId", &ok, at<I>(self, 0x314)); // 00592E9B..BB
    at<I>(self, 0x280) = gui_load_gff_int_00411c90(gff, nullptr, s, "AreaListMaxSize", &ok, at<I>(self, 0x280)); // 00592EB4..E7
    at<float>(self, 0x2dc) = gui_load_gff_float_00411d00(gff, nullptr, s, "SpawnPoolActive", &ok, at<float>(self, 0x2dc)); // 00592ED3..F2
    at<float>(self, 0x2d4) = gui_load_gff_float_00411d00(gff, nullptr, s, "AreaPoints", &ok, at<float>(self, 0x2d4)); // 00592EF8..11
    unsigned char area_list[0x14] = {};                                                          // CResList [esp+80]
    if (gff_list_004118c0(gff, nullptr, area_list, s, "AreaList") != 0) {                        // 00592F17..2E ([00749BF4])
        const I count = static_cast<I>(gff_list_count_00411940(gff, nullptr, area_list));        // 00592F34..3E
        at<I>(self, 0x288) = count;                                                              // 00592F45
        if (count > 0) {                                                                         // 00592F4B (signed)
            if (at<void*>(self, 0x284) != nullptr) live_free(at<void*>(self, 0x284));            // 00592F4D..5D (not cleared)
            at<void*>(self, 0x284) = live_operator_new(at<U>(self, 0x280) << 2);                 // 00592F60..6F (AreaListMaxSize)
            for (I i = 0; i < at<I>(self, 0x288); ++i) {                                         // 00592F84..C8
                U element[4] = {};                                                               // [esp+40]
                if (gff_list_element_00411990(gff, nullptr, element, area_list, 3) == 0) continue; // 00592F84..9C (index 3: original)
                at<U*>(self, 0x284)[i] = gui_load_gff_dword_00411c20(gff, nullptr, element, "AreaObject", &ok, 0); // 00592F9E..BC
            }
        }
    }
    unsigned char spawned_list[0x14] = {};                                                       // CResList [esp+A8]
    if (gff_list_004118c0(gff, nullptr, spawned_list, s, "SpawnList") != 0) {                    // 00592FCA..E1 ([00749BDC])
        if (at<I>(self, 0x25c) > 0) kotor_fastcall_005921a0(self, nullptr);                      // 00592FE7..F3 ClearSpawnList
        const I count = static_cast<I>(gff_list_count_00411940(gff, nullptr, spawned_list));     // 00592FF8..07 [esp+18]
        for (I i = 0; i < count; ++i) {                                                          // 00593020..8F
            U element[4] = {};                                                                   // [esp+30]
            if (gff_list_element_00411990(gff, nullptr, element, spawned_list, static_cast<U>(i)) == 0) continue; // 00593020..37
            EncounterListEntry* entry = static_cast<EncounterListEntry*>(live_operator_new(0x24)); // 0059303D..44
            if (entry != nullptr) {                                                              // 0059304F..59 (inlined 0058FF80)
                ref_empty(entry->resref);
                entry->f10 = 0; entry->cr = 0.0f; entry->f20 = 0; entry->f18 = 0; entry->f1c = 0;
            }
            ResRef fallback; ref_from_text(&fallback, "");                                       // 00593086..9A [esp+BC]
            ResRef value;                                                                        // [esp+104]
            void* r = gui_load_gff_resref_00411e10(gff, nullptr, &value, element, "SpawnResRef", &ok, &fallback); // 0059309F..C0
            ref_copy(entry->resref, r);                                                          // 005930C5..D0 (null entry: original)
            reinterpret_cast<float&>(entry->f10) =
                gui_load_gff_float_00411d00(gff, nullptr, element, "SpawnCR", &ok, 0.0f);        // 005930D5..EC
            // inline CExoArrayList<CEncounterListEntry*>::Add on +258 / +25C / +260
            if (at<I>(self, 0x25c) == at<I>(self, 0x260)) {                                      // 005930EF..FB
                const I capacity = at<I>(self, 0x260) != 0 ? at<I>(self, 0x260) * 2 : 0x10;      // 005930FD..05
                void** old = at<void**>(self, 0x258);                                            // 0059310A
                at<I>(self, 0x260) = capacity;                                                   // 00593118
                at<void**>(self, 0x258) = static_cast<void**>(live_operator_new(static_cast<U>(capacity) * 4)); // 00593110..29
                for (I k = 0; k < at<I>(self, 0x25c); ++k) at<void**>(self, 0x258)[k] = old[k];  // 00593132..55
                if (old != nullptr) live_free(old);                                              // 00593157..61
            }
            at<void**>(self, 0x258)[at<I>(self, 0x25c)] = entry;                                 // 00593164..74
            ++at<I>(self, 0x25c);                                                                // 00593177..82
        }
    }
    return 1;                                                                                    // 00593195..A9
}

// 00593830 CSWSEncounter::LoadEncounter(CResGFF*, CResStruct*): RET 8, EAX 1 (0 when ReadEncounterFromGff fails).
U __fastcall kotor_fastcall_00593830(void* self, void*, void* gff, void* s) {
    const U read = kotor_fastcall_00592430(self, nullptr, gff, s, 0, 0);                         // 00593857..5F
    if (read == 0) return read;                                                                  // 00593864..66
    (void)kotor_fastcall_00590820(self, nullptr, gff, s);                                        // 0059386C..70
    sort_creatures(self);                                                                        // 00593875..9EF
    creature_names_and_minimum(self);                                                            // 005939F5..A62
    return 1;
}

// 00593A90 CSWSEncounter::LoadFromTemplate(CResRef*, int default_geometry, Vector* position): RET 0Ch.
// EAX 0 when the UTE does not exist, else 1 (the ReadEncounterFromGff result is not checked).
U __fastcall kotor_fastcall_00593a90(void* self, void*, void* resref, U default_geometry, U position) {
    void* memory = live_operator_new(0xa0);                                                      // 00593AAB..B2
    void* gff = memory != nullptr ? gui_load_gff_construct_00410630(memory, nullptr, 0x7f8, "UTE ", resref) : nullptr; // 00593ACC..DE ([00749EA0])
    if (at<U>(gff, 0x94) == 0) {                                                                 // 00593AE0..F2 (null gff dereferenced: original)
        vcall<void*>(gff, 0x00, 1u);                                                             // 00593AF8..FC
        ExoString message; kotor_movie::string_element_construct_005b3190(&message, nullptr);    // 00593AFE..02 [esp+18]
        ExoString name; kotor_movie::string_element_construct_005b3190(&name, nullptr);          // 00593B07..13 [esp+10]
        c2da_ref_to_string_00405f70(resref, nullptr, &name);                                     // 00593B18..24
        frame_LogFormat_005e5680(&message, "Encounter template %s doesn't exist.\n", frame_LogCStr_005e5670(&name, nullptr)); // 00593B29..42 ([00749E78])
        frame_LogStringDtor_005e5c20(&name, nullptr);                                            // 00593B45..4E
        frame_LogStringDtor_005e5c20(&message, nullptr);                                         // 00593B53..5F
        return 0;
    }
    U top[4] = {};                                                                               // [esp+C]
    (void)kotor_fastcall_00411240(gff, nullptr, top);                                            // 00593B7A..7F
    if (default_geometry != 0)                                                                   // 00593B84..8A
        (void)kotor_fastcall_00592430(self, nullptr, gff, top, default_geometry, position);      // 00593B8C..A5
    else
        (void)kotor_fastcall_00592430(self, nullptr, gff, top, 0, 0);                            // 00593B99..A5
    (void)kotor_fastcall_00590820(self, nullptr, gff, top);                                      // 00593BAA..B2
    vcall<void*>(gff, 0x00, 1u);                                                                 // 00593BB7..BD
    return 1;
}

// 00505060 CSWSArea::LoadEncounters(CResGFF*, CResStruct*, int load_state, int from_template): RET 10h, EAX 1.
U __fastcall kotor_fastcall_00505060(void* self, void*, void* gff, void* parent, U load_state, U from_template) {
    unsigned char list[0x14] = {};                                                               // CResList [esp+4C]
    if (gff_list_004118c0(gff, nullptr, list, parent, "Encounter List") == 0) return 1;          // 00505078..9A ([007474C8])
    if (gff_list_count_00411940(gff, nullptr, list) == 0) return 1;                              // 005050A0..B4 (jbe)
    U i = 0;                                                                                     // [esp+C]
    do {
        U element[4] = {};                                                                       // parent argument slot in the original
        unsigned ok = 0;                                                                         // gff argument slot in the original
        if (gff_list_element_00411990(gff, nullptr, element, list, i) != 0 &&                    // 005050C2..D9
            static_cast<kotor_q05::CResGFF*>(gff)->GetElementType(
                reinterpret_cast<kotor_q05::StructHandle*>(element)) == 7) {                     // 005050DF..F1
            const U id = gui_load_gff_dword_00411c20(gff, nullptr, element, "ObjectId", &ok, k_invalid_00746644); // 005050F7..13 ([00744C24])
            void* memory = live_operator_new(0x31c);                                             // 00505118..1F
            void* encounter = memory != nullptr ? kotor_fastcall_00593c70(memory, nullptr, id) : nullptr; // 0050512E..46
            if (encounter != nullptr) {                                                          // 00505148..52
                bool loaded;
                if (from_template != 0) {                                                        // 00505158..61
                    ResRef fallback; ref_from_text(&fallback, "");                               // 00505167..70 [esp+24]
                    ResRef res;                                                                  // [esp+34]
                    (void)gui_load_gff_resref_00411e10(gff, nullptr, &res, element, "TemplateResRef", &ok, &fallback); // 00505175..96 ([00747494])
                    loaded = ok != 0 && kotor_fastcall_00593a90(encounter, nullptr, &res, 0, 0) != 0; // 0050519B..B9
                    if (loaded) {
                        const float x = gui_load_gff_float_00411d00(gff, nullptr, element, "XPosition", &ok, 0.0f); // 005051BF..DD
                        const float y = gui_load_gff_float_00411d00(gff, nullptr, element, "YPosition", &ok, 0.0f); // 005051E4..02
                        const float z = gui_load_gff_float_00411d00(gff, nullptr, element, "ZPosition", &ok, 0.0f); // 00505206..24
                        const Vector position = {x, y, z};                                       // [esp+18]
                        recovered_object_position_004cd1a0(encounter, nullptr, &position, 1, 1, 0); // 00505228..48
                        unsigned char sub[0x14] = {};                                            // CResList [esp+44]
                        if (gff_list_004118c0(gff, nullptr, sub, element, "Geometry") != 0 &&    // 0050524D..68 ([007474BC])
                            kotor_fastcall_00590580(encounter, nullptr, gff, sub) == 0) {        // 0050526A..79
                            vcall<void*>(encounter, 0x00, 1u);                                   // 0050527B..81
                            encounter = nullptr;                                                 // 00505283
                        }
                        if (gff_list_004118c0(gff, nullptr, sub, element, "SpawnPointList") != 0) // 00505285..A0 ([007474AC])
                            (void)kotor_fastcall_00590410(encounter, nullptr, gff, sub);         // 005052A2..AA (this may be null: original)
                    } else {
                        vcall<void*>(encounter, 0x00, 1u);                                       // 00505326..2E
                        encounter = nullptr;
                    }
                } else {
                    loaded = kotor_fastcall_00593830(encounter, nullptr, gff, element) != 0;     // 00505330..42
                    if (!loaded) { vcall<void*>(encounter, 0x00, 1u); encounter = nullptr; }     // 00505348..50
                }
                if (encounter != nullptr) {                                                      // 005052AF..B1
                    if (load_state != 0) w2x_object_state_004d1cf0(encounter, nullptr, gff, element); // 005052B3..C2
                    kotor_fastcall_00593520(encounter, nullptr, self, at<float>(encounter, 0x90), at<float>(encounter, 0x94),
                                            at<float>(encounter, 0x98), load_state == 0 ? 1u : 0u); // 005052C7..EB
                }
            }
        }
        ++i;                                                                                     // 005052F0..FC
    } while (i < gff_list_count_00411940(gff, nullptr, list));                                   // 005052F4..07 (jb)
    return 1;
}

}  // extern "C"
