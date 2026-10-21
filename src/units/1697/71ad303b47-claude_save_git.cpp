// claude_save_git.cpp (lane worker save-git): the area GIT save behind CServerExoAppInternal::StoreCurrentModule
// 004B2E70, transcribed from the original swkotor.exe (read-only):
//   0050BA00 CSWSArea::SaveGIT(CERFFile*, CExoString*, CExoArrayList<ulong>* players) RET 0Ch kotor_fastcall_0050ba00
//   00507680 CSWSArea::SaveCreatures(CResGFF*, CResStruct*, CExoArrayList<ulong>*)       RET 0Ch kotor_fastcall_00507680
//   00507750 / 00507810 / 005078D0 / 00507990 / 00507A50 / 00507B10 / 00507BD0 / 00507CA0 / 00507D60
//            SaveItems / SaveDoors / SaveTriggers / SaveEncounters / SaveWaypoints / SaveSounds / SavePlaceables /
//            SaveStores / SaveAreaEffects (same shape)                                    RET 0Ch kotor_fastcall_<addr>
//   00506090 CSWSArea::SaveProperties(CResGFF*, CResStruct*)                              RET 8   kotor_fastcall_00506090
//   005061D0 CSWSArea::SaveMaps(CResGFF*, CResStruct*)                                    RET 8   kotor_fastcall_005061d0
//   005062A0 CSWSArea::SavePlaceableCameras(CResGFF*, CResStruct*)                        RET 8   kotor_fastcall_005062a0
//   00588AD0 CSWSDoor::SaveDoor(CResGFF*, CResStruct*) -> 1                               RET 8   kotor_fastcall_00588ad0
//   0058E660 CSWSTrigger::SaveTrigger(CResGFF*, CResStruct*) -> 1                         RET 8   kotor_fastcall_0058e660
//   00591350 CSWSEncounter::SaveEncounter(CResGFF*, CResStruct*) -> 1                     RET 8   kotor_fastcall_00591350
//   005C8230 CSWSWaypoint::SaveWaypoint(CResGFF*, CResStruct*) -> 1                       RET 8   kotor_fastcall_005c8230
//   005C86D0 CSWSSoundObject::Save(CResGFF*, CResStruct*)                                 RET 8   kotor_fastcall_005c86d0
//   00586A70 CSWSPlaceable::SavePlaceable(CResGFF*, CResStruct*) -> 1                     RET 8   kotor_fastcall_00586a70
//   005C6CD0 CSWSStore::SaveStore(CResGFF*, CResStruct*) -> 1                             RET 8   kotor_fastcall_005c6cd0
//   00594D80 CSWSAreaOfEffectObject::SaveEffect(CResGFF*, CResStruct*) -> 1               RET 8   kotor_fastcall_00594d80
//   005C96E0 CSWSAmbientSound::Save(CResGFF*, CResStruct*)                                RET 8   kotor_fastcall_005c96e0
// Imported from lane worker save-gff (declared, not implemented here):
//   005DDF30 CERFFile::WriteResource(char*, ushort, CRes*, int, void*) RET 14h   kotor_fastcall_005ddf30
//   00412CA0 CResGFF::WriteFieldQuaternion(CResStruct*, Quaternion*, char*) RET 0Ch kotor_fastcall_00412ca0
// Imported from lane worker save-module (coordinator ruling):
//   0062AD30 CGuiInGame::GetPlaceableCamera(int) RET 4                    kotor_fastcall_0062ad30
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); x87 at 53-bit precision -> double;
// live_operator_new / live_free only. GFF labels are the original .rdata strings (address in each comment); the
// writers copy the label text, so a literal with the same bytes is equivalent.
// Original quirks kept:
//   * SaveGIT default-constructs an unused CExoString at entry and destroys it at exit; its CExoString* argument is
//     never read; the root CResStruct comes from operator new(4) without a null check and is freed with free().
//   * SaveDoor compares the zero-extended PortraitId (movzx eax,ax) with -1, so the "Portrait" CResRef branch is
//     unreachable for doors (SaveTrigger / SavePlaceable compare ax with FFFFh and can take it).
//   * The encounter CreatureList / SpawnPointList / SpawnList / Geometry and the store ItemList list elements use
//     the loop index as the CResStruct type id; the encounter AreaList and the trigger Geometry use the constant 3
//     (0058E9F0 push 3 inside the loop; CSWSTrigger::LoadTriggerGeometry 0058D060 only accepts type-3 points).
//   * fld dword / fstp dword copies of floats quiet a signalling NaN (x87_reload below), plain dword moves do not.
#include "842d6b1618-w5_t3.hpp"

namespace lane_config_required { void* __fastcall GetPartyTable_004aee70(void*, void*); }     // CServerExoApp::GetPartyTable [0575]
#ifndef W1R_MENU_RESOURCES_REF
#define W1R_MENU_RESOURCES_REF
namespace menu_resources { struct Ref { unsigned char bytes[16]; Ref* __thiscall construct_00406d80(const char* text); }; } // CResRef(char*) [0030]
#endif

namespace save_git {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c14 candidate map; owner object in brackets) ----------------
void* __fastcall recovered_object_array_004aed70(void* server, void*);                         // CServerExoApp::GetObjectArray [w2]
void* __fastcall frame_R4_005ed690(void* client, void*);                                       // CClientExoApp::GetInGameGui [0457]
void* __fastcall recovered_gff_construct_004105a0(void* gff, void*);                           // CResGFF::CResGFF [0886]
U __fastcall recovered_gff_00411260(void* gff, void*, void* root, void* type, void* version);  // CResGFF::CreateGFFFile
U __fastcall recovered_gff_00412450(void* gff, void*, void* list, void* parent, const char* label); // CResGFF::AddList
U __fastcall recovered_gff_004124e0(void* gff, void*, void* element, void* list, U type);      // CResGFF::AddListElement
U __fastcall recovered_gff_004125b0(void* gff, void*, void* child, void* parent, const char* label, U type); // AddStructToStruct
U __fastcall recovered_gff_00412620(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldBYTE
U __fastcall recovered_gff_004126c0(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldWORD
U __fastcall recovered_gff_00412710(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldSHORT
U __fastcall recovered_gff_00412760(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldDWORD
U __fastcall recovered_gff_004127b0(void* gff, void*, void* s, U value, const char* label);    // CResGFF::WriteFieldINT
U __fastcall recovered_gff_00412870(void* gff, void*, void* s, U value_bits, const char* label); // CResGFF::WriteFieldFLOAT
U __fastcall recovered_gff_004128c0(void* gff, void*, void* s, void* resref, const char* label); // CResGFF::WriteFieldCResRef
U __fastcall recovered_gff_00412970(void* gff, void*, void* s, void* string, const char* label); // CResGFF::WriteFieldCExoString
U __fastcall recovered_gff_00412a10(void* gff, void*, void* s, void* string, const char* label); // CResGFF::WriteFieldCExoLocString
U __fastcall recovered_gff_00412c10(void* gff, void*, void* s, const void* data, U size, const char* label); // WriteFieldVOID
U __fastcall recovered_gff_00412d30(void* gff, void*, void* s, void* vector, const char* label); // CResGFF::WriteFieldVector
void* __fastcall async_ref_construct_string_00406d60(void* ref, void*, void* string);          // CResRef(CExoString const&)
char* __fastcall async_string_cstr_005e5670(void* string, void*);                              // CExoString::CStr
void __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out_string);               // CResRef::CopyToString(CExoString&)
U __fastcall recovered_save_creature_00500610(void* creature, void*, void* gff, void* s);      // CSWSCreature::SaveCreature [W2 t6]
U __fastcall recovered_save_item_0055ccd0(void* item, void*, void* gff, void* s);              // CSWSItem::SaveItem [W2 t6]
U __fastcall recovered_save_object_state_004cec50(void* object, void*, void* gff, void* s);    // CSWSObject::SaveObjectState [W2 t6]
U __fastcall recovered_save_var_table_0059adb0(void* table, void*, void* gff, void* s);        // CSWSScriptVarTable::SaveVarTable
U __fastcall recovered_sw_var_table_save_0059b250(void* table, void*, void* gff, void* s);     // CSWVarTable save (+1F4h)
U __fastcall kotor_fastcall_00579090(void* map, void*, U* size, I* res_x, I* res_y);           // CSWSAreaMap::GetMapData
void* __fastcall w2x_inventory_item_005560b0(void* repository, void*, U index);                // CItemRepository::ItemListGetItem
U __fastcall recovered_placeable_light_get_00585130(void* placeable, void*);                   // CSWSPlaceable::GetLightIsOn
void* __fastcall recovered_aoe_position_005946e0(void* aoe, void*, void* out);                 // CSWSAreaOfEffectObject::GetPosition
// ---------------- imports owned by lane worker save-gff (SAVE-GFF-EXPORTS.txt) ----------------
U __fastcall kotor_fastcall_005ddf30(void* erf, void*, const char* name, U res_type, void* res, U is_gff, U unused); // CERFFile::WriteResource
U __fastcall kotor_fastcall_00412ca0(void* gff, void*, void* res_struct, const void* quaternion16, const char* label); // WriteFieldQuaternion
// ---------------- import owned by lane worker save-module (coordinator ruling) ----------------
void* __fastcall kotor_fastcall_0062ad30(void* gui, void*, I index);                           // CGuiInGame::GetPlaceableCamera RET 4

// ---------------- exports of this file ----------------
void __fastcall kotor_fastcall_0050ba00(void* self, void*, void* erf, void* name, void* players);
void __fastcall kotor_fastcall_00507680(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507750(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507810(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_005078d0(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507990(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507a50(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507b10(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507bd0(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507ca0(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00507d60(void* self, void*, void* gff, void* parent, void* ids);
void __fastcall kotor_fastcall_00506090(void* self, void*, void* gff, void* parent);
void __fastcall kotor_fastcall_005061d0(void* self, void*, void* gff, void* parent);
void __fastcall kotor_fastcall_005062a0(void* self, void*, void* gff, void* parent);
U __fastcall kotor_fastcall_00588ad0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_0058e660(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_00591350(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_005c8230(void* self, void*, void* gff, void* s);
void __fastcall kotor_fastcall_005c86d0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_00586a70(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_005c6cd0(void* self, void*, void* gff, void* s);
U __fastcall kotor_fastcall_00594d80(void* self, void*, void* gff, void* s);
void __fastcall kotor_fastcall_005c96e0(void* self, void*, void* gff, void* s);
}

namespace {
struct alignas(4) ExoString { unsigned char b[8]; };                                           // CExoString {char*, len}
struct alignas(4) ResRef { unsigned char b[16]; };                                             // CResRef
struct GffList { U s; char label[16]; };                                                       // CResList (20 bytes)
static_assert(sizeof(GffList) == 20, "CResList");
struct GffStruct { U id; };                                                                    // CResStruct
struct IdList { U* data; I size; I alloc; };                                                   // CExoArrayList<ulong>
static_assert(sizeof(IdList) == 12, "CExoArrayList");

inline unsigned char* P(void* p, U off) { return static_cast<unsigned char*>(p) + off; }
// fld dword [m] ; fstp dword [m'] at 53-bit precision: exact for every float except a signalling NaN, which the
// load quiets (invalid operation is masked) by setting the mantissa MSB.
inline U x87_reload(U b) {
    if ((b & 0x7f800000u) == 0x7f800000u && (b & 0x007fffffu) != 0 && (b & 0x00400000u) == 0) b |= 0x00400000u;
    return b;
}
inline void gff_byte(void* g, void* s, U v, const char* l) { (void)recovered_gff_00412620(g, nullptr, s, v, l); }
inline void gff_word(void* g, void* s, U v, const char* l) { (void)recovered_gff_004126c0(g, nullptr, s, v, l); }
inline void gff_short(void* g, void* s, U v, const char* l) { (void)recovered_gff_00412710(g, nullptr, s, v, l); }
inline void gff_dword(void* g, void* s, U v, const char* l) { (void)recovered_gff_00412760(g, nullptr, s, v, l); }
inline void gff_int(void* g, void* s, U v, const char* l) { (void)recovered_gff_004127b0(g, nullptr, s, v, l); }
inline void gff_float(void* g, void* s, U v, const char* l) { (void)recovered_gff_00412870(g, nullptr, s, v, l); }
inline void gff_resref(void* g, void* s, void* v, const char* l) { (void)recovered_gff_004128c0(g, nullptr, s, v, l); }
inline void gff_string(void* g, void* s, void* v, const char* l) { (void)recovered_gff_00412970(g, nullptr, s, v, l); }
inline void gff_locstring(void* g, void* s, void* v, const char* l) { (void)recovered_gff_00412a10(g, nullptr, s, v, l); }
inline void gff_add_list(void* g, void* list, void* parent, const char* l) { (void)recovered_gff_00412450(g, nullptr, list, parent, l); }
inline void gff_add_element(void* g, void* e, void* list, U type) { (void)recovered_gff_004124e0(g, nullptr, e, list, type); }
inline void* resref_from_string(ResRef* r, void* string) { return async_ref_construct_string_00406d60(r, nullptr, string); }
// CExoArrayList<ulong>::Add as the original inlines it: grow to 2*alloc (16 when empty) through Allocate 005E03C0.
inline void id_list_add(IdList* list, U id) {
    if (list->size == list->alloc)
        menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(list), nullptr,
                                            static_cast<unsigned>(list->alloc != 0 ? list->alloc + list->alloc : 0x10));
    list->data[list->size] = id;
    ++list->size;
}
inline bool lookup(void* objects, U id, void** out) {                                          // AL == [0074666C] (0)
    return static_cast<U8>(kotor_fastcall_004d8230(objects, nullptr, id, out)) == 0;
}

// Shared body of the ten CSWSArea::Save<kind> list writers 00507680..00507E10 (one RET 0Ch each):
//   AddList(label); for i < ids->size (re-read, signed): GetGameObject; As<kind> (vtable slot); optional skip flag;
//   AddListElement(type); WriteFieldDWORD(ObjectId = obj[id_off]); saver(gff, &element); SaveObjectState.
using Saver = void (*)(void* object, void* gff, void* element);
inline void save_kind(void* gff, void* parent, IdList* ids, const char* label, U slot, U skip_off, U type, U id_off,
                      Saver saver, U state_off, bool state) {
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                     // GetObjectArray
    GffList list;
    gff_add_list(gff, &list, parent, label);                                                   // AddList
    for (I i = 0; i < ids->size; ++i) {
        void* game_object = nullptr;
        if (!lookup(objects, ids->data[i], &game_object)) continue;                            // GetGameObject
        void* object = vcall<void*>(game_object, slot);                                        // As<kind>
        if (skip_off != 0 && at<U>(object, skip_off) != 0) continue;
        GffStruct element;
        gff_add_element(gff, &element, &list, type);                                           // AddListElement
        gff_dword(gff, &element, at<U>(object, id_off), "ObjectId");                           // [00744C24]
        saver(object, gff, &element);
        if (state) (void)recovered_save_object_state_004cec50(P(object, state_off), nullptr, gff, &element);
    }
}
void saver_creature(void* o, void* g, void* e) { (void)recovered_save_creature_00500610(o, nullptr, g, e); }
void saver_item(void* o, void* g, void* e) { (void)recovered_save_item_0055ccd0(o, nullptr, g, e); }
void saver_door(void* o, void* g, void* e) { (void)kotor_fastcall_00588ad0(o, nullptr, g, e); }
void saver_trigger(void* o, void* g, void* e) { (void)kotor_fastcall_0058e660(o, nullptr, g, e); }
void saver_encounter(void* o, void* g, void* e) { (void)kotor_fastcall_00591350(o, nullptr, g, e); }
void saver_waypoint(void* o, void* g, void* e) { (void)kotor_fastcall_005c8230(o, nullptr, g, e); }
void saver_sound(void* o, void* g, void* e) { kotor_fastcall_005c86d0(o, nullptr, g, e); }
void saver_placeable(void* o, void* g, void* e) { (void)kotor_fastcall_00586a70(o, nullptr, g, e); }
void saver_store(void* o, void* g, void* e) { (void)kotor_fastcall_005c6cd0(o, nullptr, g, e); }
void saver_aoe(void* o, void* g, void* e) { (void)kotor_fastcall_00594d80(o, nullptr, g, e); }

// The eleven local CExoArrayList<ulong> of SaveGIT; while unwinding their buffers are freed in the original
// destructor order (EH states 9..0).
struct GitLists {
    IdList creatures{}, items{}, doors{}, triggers{}, encounters{}, waypoints{}, sounds{}, placeables{}, stores{}, effects{};
    bool live = true;
    void release() {
        if (!live) return;
        live = false;
        live_free(effects.data); live_free(stores.data); live_free(placeables.data); live_free(sounds.data);
        live_free(waypoints.data); live_free(encounters.data); live_free(triggers.data); live_free(doors.data);
        live_free(items.data); live_free(creatures.data);
    }
    ~GitLists() { release(); }
};
struct ScopedString {                                                                          // one CExoString's EH state
    ExoString s; bool live = false;
    void* p() { return s.b; }
    void* empty() { kotor_menu20::ConstructString005b3190(reinterpret_cast<kotor_menu20::String*>(s.b), nullptr); live = true; return s.b; }
    void* text(const char* t) { (void)frame_LogStringCtor_005e5a90(s.b, nullptr, t); live = true; return s.b; }
    void end() { if (live) { live = false; frame_LogStringDtor_005e5c20(s.b, nullptr); } }
    ~ScopedString() { end(); }
};
}  // namespace

extern "C" {
// =========================================================================================================
// 0050BA00 CSWSArea::SaveGIT(CERFFile* erf, CExoString* name (unused), CExoArrayList<ulong>* players): RET 0Ch.
// Sorts the area's object ids (+190h data, +194h count) by kind, PC creatures (creature [+A74h]->+6Ch != 0) into
// *players, builds the "GIT " V2.0 GFF and writes it into the ERF as <area resref>.git (type 7E7h).
void __fastcall kotor_fastcall_0050ba00(void* self, void*, void* erf, void* name, void* players) {
    (void)name;
    ScopedString unused; unused.empty();                                                       // 0050BA21..BA28 (state 0)
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                     // 0050BA2D..BA45
    GitLists lists;                                                                            // 0050BA49..BACF (states 1..10)
    void* gff = nullptr;
    {
        void* memory = live_operator_new(0xa0);                                                // 0050BAD3..BAE5
        if (memory != nullptr) {
            try { gff = recovered_gff_construct_004105a0(memory, nullptr); }                   // 0050BAF8..BAFF (state 11)
            catch (...) { live_free(memory); throw; }
        }
    }
    void* root = live_operator_new(4);                                                         // 0050BB09..BB18 (no null check)
    {
        ScopedString version; version.text("V2.0");                                            // 0050BB1B..BB29 [00745338]
        ScopedString type; type.text("GIT ");                                                  // 0050BB2E..BB42 [00747B70]
        (void)recovered_gff_00411260(gff, nullptr, root, type.p(), version.p());               // 0050BB47..BB64 CreateGFFFile
        type.end();                                                                            // 0050BB69..BB78
        version.end();                                                                         // 0050BB7D..BB8C
    }
    for (I i = 0; i < at<I>(self, 0x194); ++i) {                                               // 0050BB91..BE39
        void* object = nullptr;
        const U id = at<U*>(self, 0x190)[i];
        if (!lookup(objects, id, &object)) continue;                                           // 0050BBA0..BBBE
        if (vcall<void*>(object, 0x30) != nullptr) {                                           // 0050BBC4..BBD3 AsSWSCreature
            void* creature = vcall<void*>(object, 0x30);                                       // 0050BBD9..BBDB
            if (at<U>(at<void*>(creature, 0xa74), 0x6c) != 0)                                  // 0050BBDE..BBE7
                id_list_add(static_cast<IdList*>(players), at<U*>(self, 0x190)[i]);            // 0050BBE9..BC35
            else
                id_list_add(&lists.creatures, at<U*>(self, 0x190)[i]);                         // 0050BC3A..BC7E
        } else if (vcall<void*>(object, 0x38) != nullptr) {                                    // 0050BC83..BC8A AsSWSItem
            id_list_add(&lists.items, at<U*>(self, 0x190)[i]);                                 // 0050BC8C..BCCE
        } else if (vcall<void*>(object, 0x18) != nullptr) {                                    // 0050BCD3..BCDE AsSWSDoor
            id_list_add(&lists.doors, at<U*>(self, 0x190)[i]);                                 // 0050BCE0..BD22
        } else if (vcall<void*>(object, 0x40) != nullptr) {                                    // 0050BD27..BD32 AsSWSTrigger
            id_list_add(&lists.triggers, at<U*>(self, 0x190)[i]);                              // 0050BD34..BD76
        } else if (vcall<void*>(object, 0x5c) != nullptr) {                                    // 0050BD7B..BD86 AsSWSEncounter
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&lists.encounters),
                                        nullptr, at<U*>(self, 0x190)[i]);                      // 0050BD88..BE2B
        } else if (vcall<void*>(object, 0x58) != nullptr) {                                    // 0050BD9A..BDA5 AsSWSWaypoint
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&lists.waypoints),
                                        nullptr, at<U*>(self, 0x190)[i]);
        } else if (vcall<void*>(object, 0x68) != nullptr) {                                    // 0050BDB9..BDC4 AsSWSSoundObject
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&lists.sounds),
                                        nullptr, at<U*>(self, 0x190)[i]);
        } else if (vcall<void*>(object, 0x48) != nullptr) {                                    // 0050BDD8..BDE3 AsSWSPlaceable
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&lists.placeables),
                                        nullptr, at<U*>(self, 0x190)[i]);
        } else if (vcall<void*>(object, 0x64) != nullptr) {                                    // 0050BDF4..BDFF AsSWSStore
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&lists.stores),
                                        nullptr, at<U*>(self, 0x190)[i]);
        } else if (vcall<void*>(object, 0x50) != nullptr) {                                    // 0050BE10..BE1B AsSWSAreaOfEffectObject
            gui_load_array_add_00671c00(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&lists.effects),
                                        nullptr, at<U*>(self, 0x190)[i]);
        }
    }
    (void)recovered_save_var_table_0059adb0(P(self, 0x1e4), nullptr, gff, root);               // 0050BE3F..BE4B
    (void)recovered_sw_var_table_save_0059b250(P(self, 0x1f4), nullptr, gff, root);            // 0050BE50..BE58
    gff_byte(gff, root, at<U8>(self, 0x20c), "CurrentWeather");                                // 0050BE5D..BE6E [00747B60]
    gff_byte(gff, root, at<U8>(self, 0x210), "WeatherStarted");                                // 0050BE73..BE84 [00747B50]
    gff_byte(gff, root, at<U8>(self, 0x2c4), "TransPending");                                  // 0050BE89..BE9A [007475EC]
    gff_byte(gff, root, at<U8>(self, 0x2c8), "TransPendNextID");                               // 0050BE9F..BEB0 [007475DC]
    gff_byte(gff, root, at<U8>(self, 0x2c9), "TransPendCurrID");                               // 0050BEB5..BEC6 [007475CC]
    kotor_fastcall_00507680(self, nullptr, gff, root, &lists.creatures);                       // 0050BECB..BED4
    kotor_fastcall_00507750(self, nullptr, gff, root, &lists.items);                           // 0050BED9..BEE2
    kotor_fastcall_00507810(self, nullptr, gff, root, &lists.doors);                           // 0050BEE7..BEF0
    kotor_fastcall_005078d0(self, nullptr, gff, root, &lists.triggers);                        // 0050BEF5..BEFE
    kotor_fastcall_00507990(self, nullptr, gff, root, &lists.encounters);                      // 0050BF03..BF0C
    kotor_fastcall_00507a50(self, nullptr, gff, root, &lists.waypoints);                       // 0050BF11..BF1D
    kotor_fastcall_00507b10(self, nullptr, gff, root, &lists.sounds);                          // 0050BF22..BF2E
    kotor_fastcall_00507bd0(self, nullptr, gff, root, &lists.placeables);                      // 0050BF33..BF3C
    kotor_fastcall_00507ca0(self, nullptr, gff, root, &lists.stores);                          // 0050BF41..BF4A
    kotor_fastcall_00507d60(self, nullptr, gff, root, &lists.effects);                         // 0050BF4F..BF58
    kotor_fastcall_00506090(self, nullptr, gff, root);                                         // 0050BF5D..BF61
    kotor_fastcall_005061d0(self, nullptr, gff, root);                                         // 0050BF66..BF6A
    kotor_fastcall_005062a0(self, nullptr, gff, root);                                         // 0050BF6F..BF73
    {
        ScopedString resname; resname.empty();                                                 // 0050BF78..BF7F (state 14)
        c2da_ref_to_string_00405f70(P(self, 0x10c), nullptr, resname.p());                     // 0050BF84..BF9A
        (void)kotor_fastcall_005ddf30(erf, nullptr, async_string_cstr_005e5670(resname.p(), nullptr), 0x7e7, gff, 1,
                                      0xffffffffu);                                          // 0050BF9F..BFBD
        if (gff != nullptr) reinterpret_cast<void(__thiscall*)(void*, U)>(at<void*>(at<void*>(gff, 0), 0))(gff, 1); // 0050BFC2..BFCC
        live_free(root);                                                                       // 0050BFCE..BFD4
        resname.end();                                                                         // 0050BFD7..BFE6
    }
    lists.release();                                                                           // 0050BFEB..C0A2 (states 9..0)
    unused.end();                                                                              // 0050C0AA..C0BC
}

// =========================================================================================================
// 00507680 CSWSArea::SaveCreatures: "Creature List", AsSWSCreature (slot 30h), skipped when [+A88h] != 0,
// element type 4, ObjectId [+4], SaveCreature only. GetPartyTable is called and its result discarded. RET 0Ch.
void __fastcall kotor_fastcall_00507680(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    void* objects = recovered_object_array_004aed70(server_app(), nullptr);                     // 00507683..769C
    (void)lane_config_required::GetPartyTable_004aee70(server_app(), nullptr);                 // 00507693..76A0 (unused)
    GffList list;
    gff_add_list(gff, &list, parent, "Creature List");                                         // 005076A5..76BA [007458DC]
    IdList* list_ids = static_cast<IdList*>(ids);
    for (I i = 0; i < list_ids->size; ++i) {                                                   // 005076BF..773D
        void* game_object = nullptr;
        if (!lookup(objects, list_ids->data[i], &game_object)) continue;                       // 005076D0..76EB
        void* creature = vcall<void*>(game_object, 0x30);                                      // 005076ED..76F6
        if (at<U>(creature, 0xa88) != 0) continue;                                             // 005076F8..7700
        GffStruct element;
        gff_add_element(gff, &element, &list, 4);                                              // 00507702..7710
        gff_dword(gff, &element, at<U>(creature, 4), "ObjectId");                              // 00507715..7725 [00744C24]
        saver_creature(creature, gff, &element);                                               // 0050772A..7732
    }
}
// 00507750 CSWSArea::SaveItems: "List", AsSWSItem (38h), type 0, ObjectId [+14h], SaveItem, SaveObjectState(+10h).
void __fastcall kotor_fastcall_00507750(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "List", 0x38, 0, 0, 0x14, saver_item, 0x10, true); // [007474A4]
}
// 00507810 CSWSArea::SaveDoors: "Door List", AsSWSDoor (18h), type 8, ObjectId [+4], SaveDoor, SaveObjectState.
void __fastcall kotor_fastcall_00507810(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "Door List", 0x18, 0, 8, 4, saver_door, 0, true); // [00747680]
}
// 005078D0 CSWSArea::SaveTriggers: "TriggerList", AsSWSTrigger (40h), type 1.
void __fastcall kotor_fastcall_005078d0(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "TriggerList", 0x40, 0, 1, 4, saver_trigger, 0, true); // [0074768C]
}
// 00507990 CSWSArea::SaveEncounters: "Encounter List", AsSWSEncounter (5Ch), type 7.
void __fastcall kotor_fastcall_00507990(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "Encounter List", 0x5c, 0, 7, 4, saver_encounter, 0, true); // [007474C8]
}
// 00507A50 CSWSArea::SaveWaypoints: "WaypointList", AsSWSWaypoint (58h), type 5.
void __fastcall kotor_fastcall_00507a50(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "WaypointList", 0x58, 0, 5, 4, saver_waypoint, 0, true); // [007474D8]
}
// 00507B10 CSWSArea::SaveSounds: "SoundList", AsSWSSoundObject (68h), type 6.
void __fastcall kotor_fastcall_00507b10(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "SoundList", 0x68, 0, 6, 4, saver_sound, 0, true); // [007474F8]
}
// 00507BD0 CSWSArea::SavePlaceables: "Placeable List", AsSWSPlaceable (48h), skipped when [+44Ch] != 0, type 9.
void __fastcall kotor_fastcall_00507bd0(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "Placeable List", 0x48, 0x44c, 9, 4, saver_placeable, 0, true); // [00747698]
}
// 00507CA0 CSWSArea::SaveStores: "StoreList", AsSWSStore (64h), type 0Bh.
void __fastcall kotor_fastcall_00507ca0(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "StoreList", 0x64, 0, 0xb, 4, saver_store, 0, true); // [00747510]
}
// 00507D60 CSWSArea::SaveAreaEffects: "AreaEffectList", AsSWSAreaOfEffectObject (50h), type 0Dh.
void __fastcall kotor_fastcall_00507d60(void* self, void*, void* gff, void* parent, void* ids) {
    (void)self;
    save_kind(gff, parent, static_cast<IdList*>(ids), "AreaEffectList", 0x50, 0, 0xd, 4, saver_aoe, 0, true); // [0074751C]
}

// =========================================================================================================
// 00506090 CSWSArea::SaveProperties: child struct "AreaProperties" (type 64h) built in the CResStruct* argument
// slot; ambient sound (+208h) saved into it, then the property fields. RET 8.
void __fastcall kotor_fastcall_00506090(void* self, void*, void* gff, void* parent) {
    GffStruct child;
    (void)recovered_gff_004125b0(gff, nullptr, &child, parent, "AreaProperties", 0x64);        // 00506090..60AB [00747660]
    kotor_fastcall_005c96e0(at<void*>(self, 0x208), nullptr, gff, &child);                     // 005060B0..60BC
    gff_byte(gff, &child, at<U8>(self, 0x2ac), "Unescapable");                                 // 005060C1..60D6 [00747654]
    gff_byte(gff, &child, at<U8>(self, 0x2b0), "RestrictMode");                                // 005060DB..60F0 [00747644]
    gff_dword(gff, &child, at<U>(self, 0x2b4), "StealthXPMax");                                // 005060F5..6108 [00747634]
    gff_dword(gff, &child, at<U>(self, 0x2b8), "StealthXPCurrent");                            // 0050610D..6120 [00747620]
    gff_dword(gff, &child, at<U>(self, 0x2bc), "StealthXPLoss");                               // 00506125..6138 [00747610]
    gff_byte(gff, &child, at<U8>(self, 0x2c0), "StealthXPEnabled");                            // 0050613D..6152 [007475FC]
    gff_byte(gff, &child, at<U8>(self, 0x2c4), "TransPending");                                // 00506157..616C [007475EC]
    gff_byte(gff, &child, at<U8>(self, 0x2c8), "TransPendNextID");                             // 00506171..6186 [007475DC]
    gff_byte(gff, &child, at<U8>(self, 0x2c9), "TransPendCurrID");                             // 0050618B..61A0 [007475CC]
    gff_dword(gff, &child, at<U>(self, 0x8c), "SunFogColor");                                  // 005061A5..61B8 [007475C0]
}
// 005061D0 CSWSArea::SaveMaps: when the module's area map (+218h) has entries ([map+4] unsigned > 0), writes the
// "AreaMap" struct (type 65h): ResX, ResY, DataSize = 4*count and the raw map data. RET 8.
void __fastcall kotor_fastcall_005061d0(void* self, void*, void* gff, void* parent) {
    (void)self;
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                          // 005061D0..61DB
    if (module == nullptr) return;                                                             // 005061E0..61E4
    void* map = at<void*>(module, 0x218);                                                      // 005061EA
    if (map == nullptr) return;                                                                // 005061F0..61F2
    if (at<U>(map, 4) == 0) return;                                                            // 005061F8..61FB (jbe)
    I res_y = 0;                                                                               // 00506208 [esp+10h]
    U count = 0;                                                                               // 00506204 [esp+14h]
    I res_x;                                                                                   // [esp+18h], written by the callee
    const U data = kotor_fastcall_00579090(map, nullptr, &count, &res_x, &res_y);              // 0050620C..621B
    GffStruct child;
    (void)recovered_gff_004125b0(gff, nullptr, &child, parent, "AreaMap", 0x65);               // 00506220..6239 [00747560]
    gff_int(gff, &child, static_cast<U>(res_x), "AreaMapResX");                                // 0050623E..624F [00747554]
    gff_int(gff, &child, static_cast<U>(res_y), "AreaMapResY");                                // 00506254..6265 [00747548]
    const U size = count << 2;                                                                 // 0050626A..6273
    gff_dword(gff, &child, size, "AreaMapDataSize");                                           // 0050626E..627E [00747538]
    (void)recovered_gff_00412c10(gff, nullptr, &child, reinterpret_cast<const void*>(static_cast<std::uintptr_t>(data)),
                                 size, "AreaMapData");                                         // 00506283..6291 [0074752C]
}
// 005062A0 CSWSArea::SavePlaceableCameras: "CameraList" of the in-game GUI's cameras (count +1B0h). RET 8.
void __fastcall kotor_fastcall_005062a0(void* self, void*, void* gff, void* parent) {
    (void)self;
    void* gui = frame_R4_005ed690(client_app(), nullptr);                                      // 005062A0..62AE
    GffList list;
    gff_add_list(gff, &list, parent, "CameraList");                                            // 005062B3..62CA [007475B4]
    for (I i = 0; i < at<I>(gui, 0x1b0); ++i) {                                                // 005062CF..63A0
        void* camera = kotor_fastcall_0062ad30(gui, nullptr, i);                               // 005062E0..62E3
        if (camera == nullptr) continue;                                                       // 005062E8..62EC
        GffStruct element;
        gff_add_element(gff, &element, &list, 0xe);                                            // 005062F2..6300
        gff_int(gff, &element, at<U>(camera, 0), "CameraID");                                  // 00506305..6314 [007475A8]
        (void)recovered_gff_00412d30(gff, nullptr, &element, P(camera, 4), "Position");        // 00506319..6329 [0074759C]
        (void)kotor_fastcall_00412ca0(gff, nullptr, &element, P(camera, 0x10), "Orientation"); // 0050632E..633E [00747590]
        gff_float(gff, &element, at<U>(camera, 0x20), "Pitch");                                // 00506343..6353 [00747588]
        gff_float(gff, &element, at<U>(camera, 0x24), "Height");                               // 00506358..6368 [00747580]
        gff_float(gff, &element, at<U>(camera, 0x28), "FieldOfView");                          // 0050636D..637D [00747574]
        gff_float(gff, &element, at<U>(camera, 0x2c), "MicRange");                             // 00506382..6392 [00747568]
    }
}
// =========================================================================================================
// 00588AD0 CSWSDoor::SaveDoor(CResGFF*, CResStruct*): RET 8, EAX 1.
U __fastcall kotor_fastcall_00588ad0(void* self, void*, void* gff, void* s) {
    gff_dword(gff, s, at<U8>(self, 0x2a0), "Appearance");                                      // 00588AE0..8AF0 [00746EFC]
    gff_byte(gff, s, at<U8>(self, 0x2a1), "GenericType");                                      // 00588AF5..8B06 [0074988C]
    gff_byte(gff, s, at<U8>(self, 0x2cc), "OpenState");                                        // 00588B0B..8B1C [00749880]
    gff_byte(gff, s, at<U8>(self, 0x2f0), "AutoRemoveKey");                                    // 00588B21..8B32 [007497D0]
    gff_float(gff, s, x87_reload(at<U>(self, 0x2b4)), "Bearing");                              // 00588B37..8B49 [00747A8C] fld/fstp
    gff_float(gff, s, at<U>(self, 0x90), "X");                                                 // 00588B4E..8B5D [007476E8]
    gff_float(gff, s, at<U>(self, 0x94), "Y");                                                 // 00588B62..8B71 [0074699C]
    gff_float(gff, s, at<U>(self, 0x98), "Z");                                                 // 00588B76..8B85 [00747A88]
    gff_dword(gff, s, at<U>(self, 0x2b8), "Faction");                                          // 00588B8A..8B99 [007497C8]
    gff_byte(gff, s, at<U8>(self, 0x2bc), "Fort");                                             // 00588B9E..8BAF [00749710]
    gff_byte(gff, s, at<U8>(self, 0x2be), "Will");                                             // 00588BB4..8BC5 [00749708]
    gff_byte(gff, s, at<U8>(self, 0x2bd), "Ref");                                              // 00588BCA..8BDB [00749704]
    gff_short(gff, s, at<U16>(self, 0xe0), "HP");                                              // 00588BE0..8BF2 [00749730]
    gff_short(gff, s, vcall<U>(self, 0x9c, U(0)), "CurrentHP");                                // 00588BF7..8C0C [00749724]
    gff_byte(gff, s, at<U8>(self, 0xf8), "Plot");                                              // 00588C11..8C22 [00748F64]
    gff_byte(gff, s, at<U8>(self, 0x200), "Min1HP");                                           // 00588C27..8C38 [007497C0]
    gff_string(gff, s, P(self, 0x2d0), "KeyName");                                             // 00588C3D..8C4C [0074979C]
    gff_byte(gff, s, at<U8>(self, 0x2d8), "KeyRequired");                                      // 00588C51..8C62 [007496D4]
    gff_byte(gff, s, at<U8>(self, 0x2bf), "OpenLockDC");                                       // 00588C67..8C78 [007497A4]
    gff_byte(gff, s, at<U8>(self, 0x2c0), "CloseLockDC");                                      // 00588C7D..8C8E [007496C8]
    gff_byte(gff, s, at<U8>(self, 0x2c1), "SecretDoorDC");                                     // 00588C93..8CA4 [00749870]
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 00588CA9..8CB5 [00747A28]
    ResRef conversation;
    gff_resref(gff, s, vcall<void*>(self, 0x80, static_cast<void*>(&conversation)), "Conversation"); // 00588CBA..8CD2 [007496B8]
    if (static_cast<U>(static_cast<U16>(vcall<U>(self, 0xd0))) == 0xffffffffu) {                // 00588CD7..8CEB (never: movzx)
        ResRef portrait;
        gff_resref(gff, s, vcall<void*>(self, 0xc8, static_cast<void*>(&portrait)), "Portrait"); // 00588CED..8D06 [00749370]
    } else {
        gff_word(gff, s, vcall<U>(self, 0xd0), "PortraitId");                                  // 00588D08..8D17 [0074937C]
    }
    gff_byte(gff, s, at<U8>(self, 0x2c2), "Hardness");                                         // 00588D1C..8D2D [00749718]
    ResRef script;
    resref_from_string(&script, P(self, 0x230)); gff_resref(gff, s, &script, "OnClosed");      // 00588D32..8D4F [00749684]
    resref_from_string(&script, P(self, 0x238)); gff_resref(gff, s, &script, "OnDamaged");     // 00588D54..8D71 [00749678]
    resref_from_string(&script, P(self, 0x240)); gff_resref(gff, s, &script, "OnDeath");       // 00588D76..8D93 [00744D0C]
    resref_from_string(&script, P(self, 0x248)); gff_resref(gff, s, &script, "OnDisarm");      // 00588D98..8DB5 [0074966C]
    resref_from_string(&script, P(self, 0x250)); gff_resref(gff, s, &script, "OnHeartbeat");   // 00588DBA..8DD7 [00747A4C]
    resref_from_string(&script, P(self, 0x258)); gff_resref(gff, s, &script, "OnLock");        // 00588DDC..8DF9 [00749654]
    resref_from_string(&script, P(self, 0x260)); gff_resref(gff, s, &script, "OnMeleeAttacked"); // 00588DFE..8E1B [00749644]
    resref_from_string(&script, P(self, 0x228)); gff_resref(gff, s, &script, "OnOpen");        // 00588E20..8E3D [0074963C]
    resref_from_string(&script, P(self, 0x268)); gff_resref(gff, s, &script, "OnSpellCastAt"); // 00588E42..8E5F [0074962C]
    resref_from_string(&script, P(self, 0x270)); gff_resref(gff, s, &script, "OnTrapTriggered"); // 00588E64..8E81 [007495EC]
    resref_from_string(&script, P(self, 0x278)); gff_resref(gff, s, &script, "OnUnlock");      // 00588E86..8EA3 [00749620]
    resref_from_string(&script, P(self, 0x280)); gff_resref(gff, s, &script, "OnUserDefined"); // 00588EA8..8EC5 [00747A3C]
    resref_from_string(&script, P(self, 0x288)); gff_resref(gff, s, &script, "OnClick");       // 00588ECA..8EE7 [00749868]
    resref_from_string(&script, P(self, 0x298)); gff_resref(gff, s, &script, "OnFailToOpen");  // 00588EEC..8F09 [00749858]
    resref_from_string(&script, P(self, 0x290)); gff_resref(gff, s, &script, "OnDialog");      // 00588F0E..8F2B [0074960C]
    gff_byte(gff, s, at<U8>(self, 0x2f8), "TrapType");                                         // 00588F30..8F41 [0074973C]
    gff_byte(gff, s, at<U8>(self, 0x2fc), "TrapDisarmable");                                   // 00588F46..8F57 [0074978C]
    gff_byte(gff, s, at<U8>(self, 0x300), "TrapDetectable");                                   // 00588F5C..8F6D [0074977C]
    gff_byte(gff, s, at<U8>(self, 0x2ec), "DisarmDC");                                         // 00588F72..8F83 [00749770]
    gff_byte(gff, s, at<U8>(self, 0x2ed), "TrapDetectDC");                                     // 00588F88..8F99 [00749760]
    gff_byte(gff, s, at<U8>(self, 0x2e8), "TrapFlag");                                         // 00588F9E..8FAF [00749754]
    gff_byte(gff, s, at<U8>(self, 0x304), "TrapOneShot");                                      // 00588FB4..8FC5 [00749748]
    gff_byte(gff, s, at<U8>(self, 0x2c4), "Locked");                                           // 00588FCA..8FDB [007496F0]
    gff_byte(gff, s, at<U8>(self, 0x2c8), "Lockable");                                         // 00588FE0..8FF1 [007496F8]
    gff_byte(gff, s, at<U8>(self, 0x384), "LinkedToFlags");                                    // 00588FF6..9007 [00747A94]
    gff_string(gff, s, P(self, 0x388), "LinkedTo");                                            // 0058900C..901B [00747AA4]
    menu_resources::Ref module;
    module.construct_00406d80(async_string_cstr_005e5670(P(self, 0x390), nullptr));            // 00589020..9030
    gff_resref(gff, s, &module, "LinkedToModule");                                             // 00589035..9042 [00747AC8]
    gff_word(gff, s, at<U16>(self, 0x3ac), "LoadScreenID");                                    // 00589047..9059 [00747880]
    gff_locstring(gff, s, vcall<void*>(self, 0x8c), "LocName");                                // 0058905E..9071 [007497E0]
    gff_locstring(gff, s, P(self, 0x3a4), "Description");                                      // 00589076..9085 [007488C0]
    gff_byte(gff, s, at<U8>(self, 0x3c0), "Static");                                           // 0058908A..909B [00749734]
    gff_locstring(gff, s, P(self, 0x3c8), "TransitionDestination");                            // 005890A0..90AF [00747AB0]
    return 1;                                                                                  // 005890B6
}

// 0058E660 CSWSTrigger::SaveTrigger(CResGFF*, CResStruct*): RET 8, EAX 1. The Geometry points are written relative
// to the trigger position: each component fld/fsub dword/fstp dword (double difference rounded to float).
U __fastcall kotor_fastcall_0058e660(void* self, void*, void* gff, void* s) {
    ResRef script;
    resref_from_string(&script, P(self, 0x244)); gff_resref(gff, s, &script, "ScriptHeartbeat");  // 0058E669..E68E [00746C94]
    resref_from_string(&script, P(self, 0x24c)); gff_resref(gff, s, &script, "ScriptOnEnter");    // 0058E693..E6B0 [00749A28]
    resref_from_string(&script, P(self, 0x254)); gff_resref(gff, s, &script, "ScriptOnExit");     // 0058E6B5..E6D2 [00749A18]
    resref_from_string(&script, P(self, 0x25c)); gff_resref(gff, s, &script, "ScriptUserDefine"); // 0058E6D7..E6F4 [00746BE8]
    resref_from_string(&script, P(self, 0x264)); gff_resref(gff, s, &script, "OnTrapTriggered");  // 0058E6F9..E716 [007495EC]
    resref_from_string(&script, P(self, 0x26c)); gff_resref(gff, s, &script, "OnDisarm");         // 0058E71B..E738 [0074966C]
    resref_from_string(&script, P(self, 0x274)); gff_resref(gff, s, &script, "OnClick");          // 0058E73D..E75A [00749868]
    gff_byte(gff, s, at<U8>(self, 0x2d4), "TrapType");                                         // 0058E75F..E770 [0074973C]
    gff_byte(gff, s, at<U8>(self, 0x2d0), "TrapOneShot");                                      // 0058E775..E786 [00749748]
    gff_dword(gff, s, at<U>(self, 0x2e4), "CreatorId");                                        // 0058E78B..E79A [007473F0]
    gff_string(gff, s, P(self, 0x230), "LinkedTo");                                            // 0058E79F..E7AE [00747AA4]
    gff_byte(gff, s, at<U8>(self, 0x240), "LinkedToFlags");                                    // 0058E7B3..E7C4 [00747A94]
    menu_resources::Ref module;
    module.construct_00406d80(async_string_cstr_005e5670(P(self, 0x238), nullptr));            // 0058E7C9..E7D9
    gff_resref(gff, s, &module, "LinkedToModule");                                             // 0058E7DE..E7EB [00747AC8]
    gff_byte(gff, s, at<U8>(self, 0x2cc), "AutoRemoveKey");                                    // 0058E7F0..E801 [007497D0]
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 0058E806..E812 [00747A28]
    gff_locstring(gff, s, P(self, 0x228), "LocalizedName");                                    // 0058E817..E826 [00748FEC]
    gff_dword(gff, s, at<U>(self, 0x2b8), "Faction");                                          // 0058E82B..E83A [007497C8]
    gff_byte(gff, s, at<U8>(self, 0x2fc), "Cursor");                                           // 0058E83F..E850 [00749A10]
    gff_string(gff, s, P(self, 0x27c), "KeyName");                                             // 0058E855..E864 [0074979C]
    gff_byte(gff, s, at<U8>(self, 0x2c4), "TrapDisarmable");                                   // 0058E869..E87A [0074978C]
    gff_byte(gff, s, at<U8>(self, 0x2c0), "TrapDetectable");                                   // 0058E87F..E890 [0074977C]
    if (static_cast<U16>(vcall<U>(self, 0xd0)) == 0xffffu) {                                   // 0058E895..E8A3
        ResRef portrait;
        gff_resref(gff, s, vcall<void*>(self, 0xc8, static_cast<void*>(&portrait)), "Portrait"); // 0058E8A5..E8BD [00749370]
    } else {
        gff_word(gff, s, vcall<U>(self, 0xd0), "PortraitId");                                  // 0058E8C4..E8D7 [0074937C]
    }
    U type = 0;                                                                                // 0058E8DC..E8F9
    if (at<U>(self, 0x2b4) != 0) type = 1;
    else if (at<U>(self, 0x2bc) != 0) type = 2;
    gff_int(gff, s, type, "Type");                                                             // 0058E8FE..E907 [00745E38]
    gff_float(gff, s, x87_reload(at<U>(self, 0x2e0)), "HighlightHeight");                      // 0058E90C..E91E [00749A00] fld/fstp
    U pos[3];                                                                                  // three dword moves of +90h
    std::memcpy(pos, P(self, 0x90), 12);                                                       // 0058E923..E940
    gff_float(gff, s, pos[0], "XPosition");                                                    // 0058E933..E944 [00745D80]
    gff_float(gff, s, pos[1], "YPosition");                                                    // 0058E949..E956 [00745D74]
    gff_float(gff, s, pos[2], "ZPosition");                                                    // 0058E95B..E968 [00745D68]
    U orient[3];
    std::memcpy(orient, P(self, 0x9c), 12);                                                    // 0058E96D..E988
    gff_float(gff, s, orient[0], "XOrientation");                                              // 0058E97B..E98C [00745D38]
    gff_float(gff, s, orient[1], "YOrientation");                                              // 0058E991..E99E [00745D48]
    gff_float(gff, s, orient[2], "ZOrientation");                                              // 0058E9A3..E9B0 [00745D58]
    float origin[3];
    std::memcpy(origin, P(self, 0x90), 12);                                                    // 0058E9B5..E9D3 (local copy)
    GffList list;
    gff_add_list(gff, &list, s, "Geometry");                                                   // 0058E9BE..E9D7 [007474BC]
    for (I i = 0; i < at<I>(self, 0x284); ++i) {                                               // 0058E9DC..EA86
        GffStruct element;
        gff_add_element(gff, &element, &list, 3);                                              // 0058E9F0..E9FE push 3 (loadstall: was the index)
        const float* point = reinterpret_cast<const float*>(at<unsigned char*>(self, 0x288) + static_cast<U>(i) * 12u);
        const float dx = static_cast<float>(static_cast<double>(point[0]) - static_cast<double>(origin[0])); // 0058EA0D..EA1F
        const float dy = static_cast<float>(static_cast<double>(point[1]) - static_cast<double>(origin[1])); // 0058EA27..EA32
        const float dz = static_cast<float>(static_cast<double>(point[2]) - static_cast<double>(origin[2])); // 0058EA36..EA3D
        gff_float(gff, &element, bits(dx), "PointX");                                          // 0058EA16..EA41 [007499E4]
        gff_float(gff, &element, bits(dy), "PointY");                                          // 0058EA46..EA57 [007499DC]
        gff_float(gff, &element, bits(dz), "PointZ");                                          // 0058EA5C..EA6D [007499D4]
    }
    gff_word(gff, s, at<U16>(self, 0x308), "LoadScreenID");                                    // 0058EA8C..EA9E [00747880]
    gff_locstring(gff, s, P(self, 0x30c), "TransitionDestination");                            // 0058EAA3..EAB2 [00747AB0]
    gff_byte(gff, s, at<U8>(self, 0x314), "SetByPlayerParty");                                 // 0058EAB7..EAC8 [007499EC]
    return 1;                                                                                  // 0058EAD0
}

// 00591350 CSWSEncounter::SaveEncounter(CResGFF*, CResStruct*): RET 8, EAX 1. (The original keeps the list element
// CResStruct in the CResStruct* argument slot and the loop byte offset in the CResGFF* slot.)
U __fastcall kotor_fastcall_00591350(void* self, void*, void* gff, void* s) {
    Vector point{0.0f, 0.0f, 0.0f};                                                            // 00591370..9380 Vector()
    gff_byte(gff, s, at<U8>(self, 0x238), "Active");                                           // 00591361..938A [00749D58]
    gff_byte(gff, s, at<U8>(self, 0x23c), "Reset");                                            // 0059138F..93A0 [00744FE8]
    gff_int(gff, s, at<U>(self, 0x240), "ResetTime");                                          // 005913A5..93B4 [00749D4C]
    gff_int(gff, s, at<U>(self, 0x2c4), "Respawns");                                           // 005913B9..93C8 [00749D40]
    gff_int(gff, s, at<U>(self, 0x244), "SpawnOption");                                        // 005913CD..93DC [00749D34]
    gff_int(gff, s, at<U>(self, 0x254), "MaxCreatures");                                       // 005913E1..93F0 [00749D24]
    gff_int(gff, s, at<U>(self, 0x250), "RecCreatures");                                       // 005913F5..9404 [00749D14]
    gff_byte(gff, s, at<U8>(self, 0x318), "PlayerOnly");                                       // 00591409..941A [00749D08]
    gff_dword(gff, s, at<U>(self, 0x22c), "Faction");                                          // 0059141F..942E [007497C8]
    gff_int(gff, s, at<U>(self, 0x24c), "DifficultyIndex");                                    // 00591433..9442 [00749CF8]
    gff_int(gff, s, at<U>(self, 0x248), "Difficulty");                                         // 00591447..9456 [00744CAC]
    gff_float(gff, s, at<U>(self, 0x90), "XPosition");                                         // 0059145B..946A [00745D80]
    gff_float(gff, s, at<U>(self, 0x94), "YPosition");                                         // 0059146F..947E [00745D74]
    gff_float(gff, s, at<U>(self, 0x98), "ZPosition");                                         // 00591483..9492 [00745D68]
    gff_locstring(gff, s, P(self, 0x230), "LocalizedName");                                    // 00591497..94A6 [00748FEC]
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 005914AB..94B7 [00747A28]
    ResRef script;
    resref_from_string(&script, P(self, 0x2e8)); gff_resref(gff, s, &script, "OnEntered");     // 005914BC..94D9 [00749BB4]
    resref_from_string(&script, P(self, 0x2f0)); gff_resref(gff, s, &script, "OnExit");        // 005914DE..94FB [00747A2C]
    resref_from_string(&script, P(self, 0x2f8)); gff_resref(gff, s, &script, "OnHeartbeat");   // 00591500..951D [00747A4C]
    resref_from_string(&script, P(self, 0x300)); gff_resref(gff, s, &script, "OnExhausted");   // 00591522..953F [00749BA8]
    resref_from_string(&script, P(self, 0x308)); gff_resref(gff, s, &script, "OnUserDefined"); // 00591544..9561 [00747A3C]
    GffList list;
    GffStruct element;
    if (at<I>(self, 0x294) > 0) {                                                              // 00591566..956E
        gff_add_list(gff, &list, s, "Geometry");                                               // 00591574..9581 [007474BC]
        for (I i = 0; i < at<I>(self, 0x294); ++i) {                                           // 00591586..9636
            std::memcpy(&point, at<unsigned char*>(self, 0x290) + static_cast<U>(i) * 12u, 12); // 005915A0..95BA
            (void)vector_subtract_assign_004a9770(&point, nullptr, reinterpret_cast<const Vector*>(P(self, 0x90))); // 005915BE..95C9
            gff_add_element(gff, &element, &list, static_cast<U>(i));                          // 005915CE..95DB (type = index)
            gff_float(gff, &element, bits(point.x), "X");                                      // 005915E0..95F1 [007476E8]
            gff_float(gff, &element, bits(point.y), "Y");                                      // 005915F6..9607 [0074699C]
            gff_float(gff, &element, bits(point.z), "Z");                                      // 0059160C..961D [00747A88]
        }
    }
    if (at<I>(self, 0x2b4) > 0) {                                                              // 0059163C..9646
        gff_add_list(gff, &list, s, "CreatureList");                                           // 0059164C..9659 [00749CE8]
        for (I i = 0; i < at<I>(self, 0x2b4); ++i) {                                           // 0059165E..96EE
            gff_add_element(gff, &element, &list, static_cast<U>(i));                          // 00591670..9681 (type = index)
            unsigned char* entry = at<unsigned char*>(self, 0x2b0) + static_cast<U>(i) * 0x24u; // stride 24h
            gff_resref(gff, &element, entry, "ResRef");                                        // 00591686..969B [00747508]
            gff_float(gff, &element, at<U>(at<unsigned char*>(self, 0x2b0) + static_cast<U>(i) * 0x24u, 0x10), "CR"); // 005916A0..96B7 [00749CE4]
            gff_byte(gff, &element, at<U8>(at<unsigned char*>(self, 0x2b0) + static_cast<U>(i) * 0x24u, 0x20), "SingleSpawn"); // 005916BC..96D5 [00749CD8]
        }
    }
    if (at<I>(self, 0x2bc) > 0) {                                                              // 005916F0..96FA
        gff_add_list(gff, &list, s, "SpawnPointList");                                         // 00591700..970D [007474AC]
        for (I i = 0; i < at<I>(self, 0x2bc); ++i) {                                           // 00591712..97BB
            gff_add_element(gff, &element, &list, static_cast<U>(i));                          // 00591722..9733 (type = index)
            const U off = static_cast<U>(i) * 0x10u;
            gff_float(gff, &element, at<U>(at<unsigned char*>(self, 0x2b8) + off, 0), "X");    // 00591738..974E [007476E8]
            gff_float(gff, &element, at<U>(at<unsigned char*>(self, 0x2b8) + off, 4), "Y");    // 00591753..976A [0074699C]
            gff_float(gff, &element, at<U>(at<unsigned char*>(self, 0x2b8) + off, 8), "Z");    // 0059176F..9786 [00747A88]
            gff_float(gff, &element, at<U>(at<unsigned char*>(self, 0x2b8) + off, 0xc), "Orientation"); // 0059178B..97A2 [00747590]
        }
    }
    gff_int(gff, s, at<U>(self, 0x264), "NumberSpawned");                                      // 005917C1..97D0 [00749CC8]
    gff_dword(gff, s, at<U>(self, 0x268), "HeartbeatDay");                                     // 005917D5..97E4 [00749CB8]
    gff_dword(gff, s, at<U>(self, 0x26c), "HeartbeatTime");                                    // 005917E9..97F8 [00749CA8]
    gff_dword(gff, s, at<U>(self, 0x270), "LastSpawnDay");                                     // 005917FD..980C [00749C98]
    gff_dword(gff, s, at<U>(self, 0x274), "LastSpawnTime");                                    // 00591811..9820 [00749C88]
    gff_byte(gff, s, at<U8>(self, 0x278), "Started");                                          // 00591825..9836 [00749C80]
    gff_byte(gff, s, at<U8>(self, 0x27c), "Exhausted");                                        // 0059183B..984C [00749C74]
    gff_int(gff, s, at<U>(self, 0x2c8), "CurrentSpawns");                                      // 00591851..9860 [00749C64]
    gff_float(gff, s, at<U>(self, 0x2dc), "SpawnPoolActive");                                  // 00591865..9874 [00749C54]
    gff_dword(gff, s, at<U>(self, 0x2e0), "LastEntered");                                      // 00591879..9888 [00749C48]
    gff_dword(gff, s, at<U>(self, 0x2e4), "LastLeft");                                         // 0059188D..989C [00749C3C]
    gff_int(gff, s, at<U>(self, 0x314), "CustomScriptId");                                     // 005918A1..98B0 [00749C2C]
    gff_int(gff, s, at<U>(self, 0x280), "AreaListMaxSize");                                    // 005918B5..98C4 [00749C1C]
    gff_float(gff, s, at<U>(self, 0x2d4), "AreaPoints");                                       // 005918C9..98D8 [00749C10]
    if (at<I>(self, 0x288) > 0) {                                                              // 005918DD..98E5
        gff_int(gff, s, at<U>(self, 0x288), "AreaListSize");                                   // 005918E7..98F0 [00749C00]
        gff_add_list(gff, &list, s, "AreaList");                                               // 005918F5..9902 [00749BF4]
        for (I i = 0; i < at<I>(self, 0x288); ++i) {                                           // 00591907..994A
            gff_add_element(gff, &element, &list, 3);                                          // 00591913..9921
            gff_dword(gff, &element, at<U*>(self, 0x284)[i], "AreaObject");                    // 00591926..993C [00749BE8]
        }
    }
    if (at<I>(self, 0x25c) > 0) {                                                              // 0059194C..9954
        gff_add_list(gff, &list, s, "SpawnList");                                              // 00591956..9963 [00749BDC]
        for (I i = 0; i < at<I>(self, 0x25c); ++i) {                                           // 00591968..99C8
            gff_add_element(gff, &element, &list, static_cast<U>(i));                          // 00591974..9981 (type = index)
            gff_resref(gff, &element, at<void**>(self, 0x258)[i], "SpawnResRef");              // 00591986..999C [00749BD0]
            gff_float(gff, &element, at<U>(at<void**>(self, 0x258)[i], 0x10), "SpawnCR");      // 005919A1..99BA [00749BC8]
        }
    }
    return 1;                                                                                  // 005919CD
}

// 005C8230 CSWSWaypoint::SaveWaypoint(CResGFF*, CResStruct*): RET 8, EAX 1.
U __fastcall kotor_fastcall_005c8230(void* self, void*, void* gff, void* s) {
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 005C8241..824D [00747A28]
    gff_locstring(gff, s, P(self, 0x238), "LocalizedName");                                    // 005C8252..8261 [00748FEC]
    U pos[3];
    std::memcpy(pos, P(self, 0x90), 12);                                                       // 005C8266..827A
    gff_float(gff, s, pos[0], "XPosition");                                                    // 005C8274..8281 [00745D80]
    gff_float(gff, s, pos[1], "YPosition");                                                    // 005C8286..828F [00745D74]
    gff_float(gff, s, pos[2], "ZPosition");                                                    // 005C8294..82A1 [00745D68]
    U orient[3];
    std::memcpy(orient, P(self, 0x9c), 12);                                                    // 005C82A6..82BF
    gff_float(gff, s, orient[0], "XOrientation");                                              // 005C82B6..82C3 [00745D38]
    gff_float(gff, s, orient[1], "YOrientation");                                              // 005C82C8..82D1 [00745D48]
    gff_float(gff, s, orient[2], "ZOrientation");                                              // 005C82D6..82E3 [00745D58]
    gff_byte(gff, s, at<U8>(self, 0x228), "HasMapNote");                                       // 005C82E8..82F9 [0074C078]
    if (at<U>(self, 0x228) != 0) {                                                             // 005C82FE..8306 (full dword)
        gff_byte(gff, s, at<U8>(self, 0x22c), "MapNoteEnabled");                               // 005C8308..8319 [0074C068]
        gff_locstring(gff, s, P(self, 0x230), "MapNote");                                      // 005C831E..832D [0074C060]
    }
    return 1;                                                                                  // 005C8335
}

// 005C86D0 CSWSSoundObject::Save(CResGFF*, CResStruct*): RET 8 (void). The "Sounds" list is always added.
void __fastcall kotor_fastcall_005c86d0(void* self, void*, void* gff, void* s) {
    U pos[3];
    std::memcpy(pos, P(self, 0x90), 12);                                                       // 005C86E1..86FF
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 005C86EF..8703 [00747A28]
    gff_byte(gff, s, at<U8>(self, 0x228), "Active");                                           // 005C8708..8719
    gff_byte(gff, s, at<U8>(self, 0x22c), "Positional");                                       // 005C871E..872F
    gff_byte(gff, s, at<U8>(self, 0x230), "Looping");                                          // 005C8734..8745
    gff_byte(gff, s, at<U8>(self, 0x234), "Volume");                                           // 005C874A..875B
    gff_byte(gff, s, at<U8>(self, 0x238), "VolumeVrtn");                                       // 005C8760..8771
    gff_byte(gff, s, at<U8>(self, 0x23c), "Times");                                            // 005C8776..8787
    gff_float(gff, s, x87_reload(at<U>(self, 0x240)), "PitchVariation");                       // 005C878C..879E fld/fstp
    gff_dword(gff, s, at<U>(self, 0x244), "Hours");                                            // 005C87A3..87B2
    gff_dword(gff, s, at<U8>(self, 0x24c), "GeneratedType");                                   // 005C87B7..87C7
    gff_dword(gff, s, at<U>(self, 0x268), "Interval");                                         // 005C87CC..87DB [00744CB8]
    gff_dword(gff, s, at<U>(self, 0x26c), "IntervalVrtn");                                     // 005C87E0..87EF
    gff_float(gff, s, x87_reload(at<U>(self, 0x270)), "MinDistance");                          // 005C87F4..8806 [0074C200] fld/fstp
    gff_float(gff, s, x87_reload(at<U>(self, 0x274)), "MaxDistance");                          // 005C880B..881D [0074C1F4] fld/fstp
    gff_byte(gff, s, at<U8>(self, 0x278), "Continuous");                                       // 005C8822..8833 [0074C1E8]
    gff_byte(gff, s, at<U8>(self, 0x27c), "Random");                                           // 005C8838..8849 [0074C1E0]
    gff_float(gff, s, x87_reload(at<U>(self, 0x248)), "FixedVariance");                        // 005C884E..8860 [0074C1D0] fld/fstp
    gff_byte(gff, s, at<U8>(self, 0x250), "RandomPosition");                                   // 005C8865..8876
    gff_float(gff, s, x87_reload(at<U>(self, 0x254)), "RandomRangeX");                         // 005C887B..888D fld/fstp
    gff_float(gff, s, x87_reload(at<U>(self, 0x258)), "RandomRangeY");                         // 005C8892..88A4 fld/fstp
    gff_float(gff, s, pos[0], "XPosition");                                                    // 005C88A9..88B2 [00745D80]
    gff_float(gff, s, pos[1], "YPosition");                                                    // 005C88B7..88C4 [00745D74]
    gff_float(gff, s, pos[2], "ZPosition");                                                    // 005C88C9..88D6 [00745D68]
    GffList list;
    gff_add_list(gff, &list, s, "Sounds");                                                     // 005C88DB..88E8 [0074C198]
    for (I i = 0; i < at<I>(self, 0x260); ++i) {                                               // 005C88ED..8939
        GffStruct element;
        gff_add_element(gff, &element, &list, 0);                                              // 005C8900..890E
        gff_resref(gff, &element, at<unsigned char*>(self, 0x25c) + static_cast<U>(i) * 0x10u, "Sound"); // 005C8913..8928 [007442C8]
    }
}

// 00586A70 CSWSPlaceable::SavePlaceable(CResGFF*, CResStruct*): RET 8, EAX 1. Recomputes the bearing from the
// orientation (Yaw(+9Ch) * [007415F8] = 0.0174532924f), stores it in +25Ch and writes it.
U __fastcall kotor_fastcall_00586a70(void* self, void*, void* gff, void* s) {
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 00586A80..6A8C [00747A28]
    gff_locstring(gff, s, vcall<void*>(self, 0x8c), "LocName");                                // 00586A91..6AA4 [007497E0]
    gff_byte(gff, s, at<U8>(self, 0x270), "AutoRemoveKey");                                    // ..00586ABA [007497D0]
    gff_dword(gff, s, at<U>(self, 0x23c), "Faction");                                          // ..00586ACE [007497C8]
    gff_byte(gff, s, at<U8>(self, 0xf8), "Plot");                                              // ..00586AE4 [00748F64]
    gff_byte(gff, s, at<U8>(self, 0x200), "Min1HP");                                           // ..00586AFA [007497C0]
    gff_byte(gff, s, at<U8>(self, 0x274), "OpenLockDC");                                       // ..00586B10 [007497A4]
    gff_string(gff, s, P(self, 0x264), "KeyName");                                             // ..00586B24 [0074979C]
    gff_byte(gff, s, at<U8>(self, 0x280), "TrapDisarmable");                                   // ..00586B3A [0074978C]
    gff_byte(gff, s, at<U8>(self, 0x284), "TrapDetectable");                                   // ..00586B50 [0074977C]
    gff_byte(gff, s, at<U8>(self, 0x27c), "DisarmDC");                                         // ..00586B66 [00749770]
    gff_byte(gff, s, at<U8>(self, 0x276), "TrapDetectDC");                                     // ..00586B7C [00749760]
    gff_byte(gff, s, at<U8>(self, 0x278), "TrapFlag");                                         // ..00586B92 [00749754]
    gff_byte(gff, s, at<U8>(self, 0x288), "TrapOneShot");                                      // ..00586BA8 [00749748]
    gff_byte(gff, s, at<U8>(self, 0x290), "TrapType");                                         // ..00586BBE [0074973C]
    gff_byte(gff, s, at<U8>(self, 0x328), "Useable");                                          // ..00586BD4
    gff_byte(gff, s, at<U8>(self, 0x398), "Static");                                           // ..00586BEA [00749734]
    gff_byte(gff, s, at<U8>(self, 0x254), "GroundPile");                                       // ..00586C00
    gff_dword(gff, s, at<U8>(self, 0x230), "Appearance");                                      // ..00586C15 [00746EFC]
    gff_short(gff, s, at<U16>(self, 0xe0), "HP");                                              // ..00586C2C [00749730]
    gff_short(gff, s, vcall<U>(self, 0x9c, U(1)), "CurrentHP");                                // 00586C31..6C46 [00749724]
    gff_byte(gff, s, at<U8>(self, 0x258), "Hardness");                                         // ..00586C5C [00749718]
    gff_byte(gff, s, at<U8>(self, 0x314), "Fort");                                             // ..00586C72 [00749710]
    gff_byte(gff, s, at<U8>(self, 0x315), "Will");                                             // ..00586C88 [00749708]
    gff_byte(gff, s, at<U8>(self, 0x316), "Ref");                                              // ..00586C9E [00749704]
    gff_byte(gff, s, at<U8>(self, 0x330), "Lockable");                                         // ..00586CB4 [007496F8]
    gff_byte(gff, s, at<U8>(self, 0x260), "Locked");                                           // ..00586CCA [007496F0]
    gff_byte(gff, s, at<U8>(self, 0x324), "HasInventory");                                     // ..00586CE0
    gff_byte(gff, s, at<U8>(self, 0x26c), "KeyRequired");                                      // ..00586CF6 [007496D4]
    gff_byte(gff, s, at<U8>(self, 0x275), "CloseLockDC");                                      // ..00586D0C [007496C8]
    gff_byte(gff, s, at<U8>(self, 0x338), "Open");                                             // ..00586D22
    gff_byte(gff, s, at<U8>(self, 0x218), "PartyInteract");                                    // ..00586D38
    if (static_cast<U16>(vcall<U>(self, 0xd0)) == 0xffffu) {                                   // 00586D3D..6D4F
        ResRef portrait;
        gff_resref(gff, s, vcall<void*>(self, 0xc8, static_cast<void*>(&portrait)), "Portrait"); // 00586D51..6D65 [00749370]
    } else {
        gff_word(gff, s, vcall<U>(self, 0xd0), "PortraitId");                                  // 00586D6C..6D7B [0074937C]
    }
    ResRef conversation;
    gff_resref(gff, s, vcall<void*>(self, 0x80, static_cast<void*>(&conversation)), "Conversation"); // 00586D80..6D98 [007496B8]
    gff_byte(gff, s, at<U8>(self, 0x394), "BodyBag");                                          // 00586D9D..6DAE
    gff_byte(gff, s, at<U8>(self, 0x334), "DieWhenEmpty");                                     // ..00586DC4
    gff_byte(gff, s, recovered_placeable_light_get_00585130(self, nullptr), "LightState");     // 00586DC9..6DD9 [00749690]
    gff_locstring(gff, s, P(self, 0x234), "Description");                                      // 00586DDE..6DED [007488C0]
    ResRef script;
    resref_from_string(&script, P(self, 0x294)); gff_resref(gff, s, &script, "OnClosed");      // ..00586E0F [00749684]
    resref_from_string(&script, P(self, 0x29c)); gff_resref(gff, s, &script, "OnDamaged");     // ..00586E31 [00749678]
    resref_from_string(&script, P(self, 0x2a4)); gff_resref(gff, s, &script, "OnDeath");       // ..00586E53 [00744D0C]
    resref_from_string(&script, P(self, 0x2ac)); gff_resref(gff, s, &script, "OnDisarm");      // ..00586E75 [0074966C]
    resref_from_string(&script, P(self, 0x2b4)); gff_resref(gff, s, &script, "OnHeartbeat");   // ..00586E97 [00747A4C]
    resref_from_string(&script, P(self, 0x2bc)); gff_resref(gff, s, &script, "OnInvDisturbed"); // ..00586EB9
    resref_from_string(&script, P(self, 0x2c4)); gff_resref(gff, s, &script, "OnLock");        // ..00586EDB [00749654]
    resref_from_string(&script, P(self, 0x2cc)); gff_resref(gff, s, &script, "OnMeleeAttacked"); // ..00586EFD [00749644]
    resref_from_string(&script, P(self, 0x2d4)); gff_resref(gff, s, &script, "OnOpen");        // ..00586F1F [0074963C]
    resref_from_string(&script, P(self, 0x2dc)); gff_resref(gff, s, &script, "OnSpellCastAt"); // ..00586F41 [0074962C]
    resref_from_string(&script, P(self, 0x2ec)); gff_resref(gff, s, &script, "OnUnlock");      // ..00586F63 [00749620]
    resref_from_string(&script, P(self, 0x2f4)); gff_resref(gff, s, &script, "OnUsed");        // ..00586F85
    resref_from_string(&script, P(self, 0x2fc)); gff_resref(gff, s, &script, "OnUserDefined"); // ..00586FA7 [00747A3C]
    resref_from_string(&script, P(self, 0x304)); gff_resref(gff, s, &script, "OnDialog");      // ..00586FC9 [0074960C]
    resref_from_string(&script, P(self, 0x30c)); gff_resref(gff, s, &script, "OnEndDialogue"); // ..00586FEB
    resref_from_string(&script, P(self, 0x2e4)); gff_resref(gff, s, &script, "OnTrapTriggered"); // ..0058700D [007495EC]
    gff_int(gff, s, at<U>(self, 0xd4), "Animation");                                           // 00587012..7021 [00746060]
    if (at<U>(at<void*>(self, 0x36c), 0x10) != 0) {                                            // 00587026..7031 (jbe)
        GffList list;
        gff_add_list(gff, &list, s, "ItemList");                                               // 00587033..7041 [00747210]
        for (U i = 0; i < at<U>(at<void*>(self, 0x36c), 0x10); ++i) {                          // 00587046..70A8 (unsigned)
            GffStruct element;
            gff_add_element(gff, &element, &list, 0);                                          // 00587055..7063
            void* item = w2x_inventory_item_005560b0(at<void*>(self, 0x36c), nullptr, i);      // 00587068..706F
            gff_dword(gff, &element, at<U>(item, 0x14), "ObjectId");                           // 00587074..7088 [00744C24]
            (void)recovered_save_item_0055ccd0(item, nullptr, gff, &element);                  // 0058708D..7097
        }
    }
    const double yaw = kotor_m15a_yaw_004aa0f0(reinterpret_cast<const Vector*>(P(self, 0x9c))); // 005870AB..70B2 (ST0)
    const float bearing = static_cast<float>(yaw * static_cast<double>(flt(0x3c8efa35u)));    // 005870B7 fmul [007415F8]; fst
    at<float>(self, 0x25c) = bearing;                                                          // 005870CF fstp [esi+25Ch]
    gff_float(gff, s, bits(bearing), "Bearing");                                               // 005870C0..70D7 [00747A8C]
    gff_float(gff, s, at<U>(self, 0x90), "X");                                                 // 005870DC..70EB [007476E8]
    gff_float(gff, s, at<U>(self, 0x94), "Y");                                                 // 005870F0..70FF [0074699C]
    gff_float(gff, s, at<U>(self, 0x98), "Z");                                                 // 00587104..7113 [00747A88]
    gff_byte(gff, s, at<U8>(self, 0x440), "IsBodyBag");                                        // 00587118..7129 [007495D4]
    gff_byte(gff, s, at<U8>(self, 0x444), "IsBodyBagVisible");                                 // 0058712E..713F [00749800]
    gff_byte(gff, s, at<U8>(self, 0x44c), "IsCorpse");                                         // 00587144..7155 [007495C8]
    return 1;                                                                                  // 0058715C
}

// 005C6CD0 CSWSStore::SaveStore(CResGFF*, CResStruct*): RET 8, EAX 1. Infinite = (item[+288h] >> 2) & FFFFFF01h.
U __fastcall kotor_fastcall_005c6cd0(void* self, void*, void* gff, void* s) {
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 005C6CE1..6CED [00747A28]
    gff_locstring(gff, s, P(self, 0x234), "LocName");                                          // ..005C6D01 [007497E0]
    gff_int(gff, s, at<U>(self, 0x244), "MarkDown");                                           // ..005C6D15
    gff_int(gff, s, at<U>(self, 0x248), "MarkUp");                                             // ..005C6D29
    ResRef script;
    resref_from_string(&script, P(self, 0x228)); gff_resref(gff, s, &script, "OnOpenStore");   // ..005C6D4B
    gff_byte(gff, s, at<U8>(self, 0x256), "BuySellFlag");                                      // ..005C6D61
    GffList list;
    gff_add_list(gff, &list, s, "ItemList");                                                   // ..005C6D73 [00747210]
    for (U i = 0; i < at<U>(at<void*>(self, 0x230), 0x10); ++i) {                              // 005C6D78..6DEF (unsigned)
        void* item = w2x_inventory_item_005560b0(at<void*>(self, 0x230), nullptr, i);          // 005C6D87..6D88
        GffStruct element;
        gff_add_element(gff, &element, &list, i);                                              // 005C6D8D..6D9C (type = index)
        gff_dword(gff, &element, at<U>(item, 0x14), "ObjectId");                               // 005C6DA1..6DB1 [00744C24]
        gff_byte(gff, &element, (at<U>(item, 0x288) >> 2) & 0xffffff01u, "Infinite");          // 005C6DB6..6DD1 [0074BE98]
        (void)recovered_save_item_0055ccd0(item, nullptr, gff, &element);                      // 005C6DD6..6DDE
    }
    gff_float(gff, s, at<U>(self, 0x90), "XPosition");                                         // 005C6DF5..6E04 [00745D80]
    gff_float(gff, s, at<U>(self, 0x94), "YPosition");                                         // 005C6E09..6E18 [00745D74]
    gff_float(gff, s, at<U>(self, 0x98), "ZPosition");                                         // ..005C6E2C [00745D68]
    gff_float(gff, s, at<U>(self, 0x9c), "XOrientation");                                      // ..005C6E40 [00745D38]
    gff_float(gff, s, at<U>(self, 0xa0), "YOrientation");                                      // ..005C6E54 [00745D48]
    gff_float(gff, s, at<U>(self, 0xa4), "ZOrientation");                                      // ..005C6E68 [00745D58]
    return 1;
}

// 00594D80 CSWSAreaOfEffectObject::SaveEffect(CResGFF*, CResStruct*): RET 8, EAX 1. Shape 0 writes Radius (+238h,
// fld/fstp), shape 1 writes Length (+240h) and Width (+23Ch), other shapes neither.
U __fastcall kotor_fastcall_00594d80(void* self, void*, void* gff, void* s) {
    gff_string(gff, s, P(self, 0x18), "Tag");                                                  // 00594D91..4D9D [00747A28]
    gff_int(gff, s, at<U>(self, 0x22c), "AreaEffectId");                                       // 00594DA2..4DB1 [0074A040]
    gff_dword(gff, s, vcall<U>(self, 0xb8), "SpellId");                                        // 00594DB6..4DC9 [00746D30]
    gff_byte(gff, s, at<U8>(self, 0x230), "Shape");                                            // 00594DCE..4DDF [0074A038]
    gff_byte(gff, s, at<U8>(self, 0x1d5), "MetaMagicType");                                    // 00594DE4..4DF5 [0074A028]
    gff_int(gff, s, at<U>(self, 0x258), "SpellSaveDC");                                        // ..00594E09
    gff_int(gff, s, at<U>(self, 0x25c), "SpellLevel");                                         // ..00594E1D
    const U8 shape = at<U8>(self, 0x230);                                                      // 00594E22..4E28
    if (shape == 0) {
        gff_float(gff, s, x87_reload(at<U>(self, 0x238)), "Radius");                           // 00594E2C..4E64 [0074A008] fld/fstp
    } else if (shape == 1) {                                                                   // 00594E3D..4E3F
        gff_float(gff, s, at<U>(self, 0x240), "Length");                                       // 00594E41..4E50 [0074A000]
        gff_float(gff, s, at<U>(self, 0x23c), "Width");                                        // 00594E55..4E64 [00749FF8]
    }
    gff_dword(gff, s, at<U>(self, 0x248), "CreatorId");                                        // 00594E69..4E78 [007473F0]
    gff_dword(gff, s, at<U>(self, 0x24c), "LinkedToObject");                                   // 00594E7D..4E8C [00749FE8]
    gff_dword(gff, s, at<U>(self, 0x250), "LastEntered");                                      // ..00594EA0 [00749C48]
    gff_dword(gff, s, at<U>(self, 0x254), "LastLeft");                                         // ..00594EB4 [00749C3C]
    gff_dword(gff, s, at<U>(self, 0x288), "Duration");                                         // ..00594EC8 [00747420]
    gff_byte(gff, s, at<U8>(self, 0x28c), "DurationType");                                     // ..00594EDE
    gff_dword(gff, s, at<U>(self, 0x280), "LastHrtbtDay");                                     // ..00594EF2
    gff_dword(gff, s, at<U>(self, 0x284), "LastHrtbtTime");                                    // ..00594F06
    gff_string(gff, s, P(self, 0x260), "OnHeartbeat");                                         // ..00594F1A
    gff_string(gff, s, P(self, 0x268), "OnUserDefined");                                       // ..00594F2E
    gff_string(gff, s, P(self, 0x270), "OnObjEnter");                                          // ..00594F42
    gff_string(gff, s, P(self, 0x278), "OnObjExit");                                           // ..00594F56
    Vector where;
    const U* position = static_cast<const U*>(recovered_aoe_position_005946e0(self, nullptr, &where)); // 00594F5B..4F62
    const U px = position[0], py = position[1], pz = position[2];                              // 00594F67..4F6F
    gff_float(gff, s, px, "PositionX");                                                        // 00594F73..4F7C [0074423C]
    gff_float(gff, s, py, "PositionY");                                                        // 00594F81..4F8A [00744230]
    gff_float(gff, s, pz, "PositionZ");                                                        // 00594F8F..4F9C [00744224]
    U orient[3];
    std::memcpy(orient, P(self, 0x9c), 12);                                                    // 00594FA1..4FAE
    gff_float(gff, s, orient[0], "OrientationX");                                              // 00594FB1..4FBA [00745DF0]
    gff_float(gff, s, orient[1], "OrientationY");                                              // 00594FBF..4FC8 [00745DE0]
    gff_float(gff, s, orient[2], "OrientationZ");                                              // 00594FCD..4FD6 [00745DD0]
    return 1;                                                                                  // 00594FDE
}

// 005C96E0 CSWSAmbientSound::Save(CResGFF*, CResStruct*): RET 8 (void).
void __fastcall kotor_fastcall_005c96e0(void* self, void*, void* gff, void* s) {
    gff_int(gff, s, at<U>(self, 0x8), "MusicDelay");                                           // 005C96ED..96F9 [0074C2F8]
    gff_int(gff, s, at<U>(self, 0xc), "MusicDay");                                             // 005C96FE..970A [0074C2EC]
    gff_int(gff, s, at<U>(self, 0x10), "MusicNight");                                          // ..005C971B
    gff_int(gff, s, at<U>(self, 0x18), "MusicBattle");                                         // ..005C972C
    gff_int(gff, s, at<U>(self, 0x20), "AmbientSndDay");                                       // ..005C973D
    gff_int(gff, s, at<U>(self, 0x24), "AmbientSndNight");                                     // ..005C974E
    gff_int(gff, s, at<U8>(self, 0x28), "AmbientSndDayVol");                                   // ..005C9760
    gff_int(gff, s, at<U8>(self, 0x29), "AmbientSndNitVol");                                   // ..005C9772
}
}  // extern "C"
}  // namespace save_git
