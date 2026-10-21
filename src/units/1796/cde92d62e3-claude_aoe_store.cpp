// claude_aoe_store.cpp (lane "aoe", coordinator addendum): CSWSStore construction / loading path for the VM CreateObject
// handler and the area store loader, transcribed from the original swkotor.exe (read-only).
//
//   005C6AB0 CSWSStore::CSWSStore(id)                     RET 4    @kotor_fastcall_005c6ab0@12
//   005C6BF0 CSWSStore::~CSWSStore                        RET 0    @kotor_fastcall_005c6bf0@8
//   005C7160 CSWSStore scalar deleting destructor         RET 4    @kotor_fastcall_005c7160@12
//   005C6E80 CSWSStore::AddToArea(area, x, y, z, flag)    RET 14h  @kotor_fastcall_005c6e80@28
//   005C6EE0 CSWSStore::EventHandler(type, caller, event, a, b) RET 14h @kotor_fastcall_005c6ee0@28
//   005C7180 CSWSStore::LoadStore(CResGFF*, CResStruct*, int from_template) RET 0Ch @kotor_fastcall_005c7180@20
//   005C7760 CSWSStore::LoadFromTemplate(CResRef*, int)   RET 8    @kotor_fastcall_005c7760@16
//   005057A0 CSWSArea::LoadStores(CResGFF*, CResStruct*, int load_state, int from_template) RET 10h
//                                                                  @kotor_fastcall_005057a0@24
//   0074BDB8 CSWSStore vtable (56 slots)                           _kotor_vtable_0074bdb8
// Quirks kept: LoadStore's final possessor loop tests the LAST item pointer seen in the item loop (the CResGFF* argument
// slot when ItemList is absent/empty; reset to 0 at the start of every item and after a failed item load) instead of
// each item; the BuySellFlag default carries the
// upper three bytes of the struct-pointer argument slot; LoadFromTemplate dereferences a failed CResGFF allocation and
// formats a "doesn't exist" string that is never used; the destructor calls GetArea and discards the result.
#include "842d6b1618-w5_t3.hpp"

namespace kotor_movie { void* __fastcall string_element_construct_005b3190(void* string, void*); }   // CExoString() [0603]
namespace frame_control { extern unsigned message_manager_007a3a00; }                        // g_pVirtualMachine [0456]
namespace menu_resources { struct Ref { Ref* construct_00406d80(const char* text); }; }       // CResRef(char const*) [0030]
namespace kotor_q05 { struct StructHandle; struct CResGFF { unsigned GetElementType(StructHandle* s); }; }       // [0290]

extern "C" {
using w5::U; using w5::I; using w5::U8; using w5::U16; using w5::Vector;
// ---- lane "aoe" exports (claude_aoe_base.cpp / claude_aoe.cpp)
void* __fastcall kotor_fastcall_004cfcb0(void* self, void*, U type, U id, U flag);
U __fastcall kotor_fastcall_004b0850(void* ai, void*, void* object, I level);
U __fastcall kotor_fastcall_004cc390(void*, void*, void*, U);
void __fastcall kotor_fastcall_004ccf60(void*, void*, U);
void* __fastcall kotor_fastcall_004d0170(void*, void*, void*);
U8 __fastcall kotor_fastcall_004cb190(void*, void*);
void* __fastcall kotor_fastcall_004d0190(void*, void*);
U __fastcall kotor_fastcall_004cb810(void*, void*);
U16 __fastcall kotor_fastcall_004d01a0(void*, void*, U);
U __fastcall kotor_fastcall_004caec0(void*, void*, U);
int __fastcall kotor_fastcall_004caee0(void*, void*, U);
int __fastcall kotor_fastcall_004caf70(void*, void*, U);
U __fastcall kotor_fastcall_004d01b0(void*, void*);
void* __fastcall kotor_fastcall_004d01c0(void*, void*, void*);
// ---- generic slot bodies
void __fastcall slot_void_return_4_0060e760_a1(void*, void*, U);
void __fastcall kotor_m15e_slot008_void8_005b5e90(void*, void*, U, U);
U __fastcall kotor_m15e_slot010_zero_0063e7f0(void*, void*);
void* __fastcall kotor_m15e_slot00c_self_00641db0(void*, void*);
U __fastcall kotor_m15e_slot0d4_true_0063e7a0(void*, void*);
void __fastcall kotor_m15e_slot090_void0_004015a0(void*, void*);
void __fastcall kotor_fastcall_004ccf80(void* object, void*, I damage);
int __fastcall kotor_fastcall_004cf160(void*, void*, void*, int, U, U, U);
int __fastcall kotor_fastcall_004d09e0(void*, void*, void*, int, U, U, U);
int __fastcall kotor_fastcall_004d0e40(void*, void*, void*, int, U, U, U, U);
void __fastcall recovered_object_portrait_ref_004d01f0(void*, void*, void*);
U __fastcall recovered_creature_portrait_004d0210(void*, void*);
void __fastcall recovered_object_portrait_004cad20(void*, void*, U);
U __stdcall c2da_resource_constant_word_00406490();
// ---- existing providers in the c32 candidate
void* __fastcall recovered_server_ai_master_004aed80(void* server, void*);                     // GetServerAIMaster
U __fastcall recovered_ai_remove_004af3d0(void* ai, void*, void* object);                      // RemoveObject [1070]
void* __fastcall server_get_module_004ae6b0(void* server, void*);                              // GetModule [0530]
U __fastcall recovered_module_remove_tag_004c5ef0(void* module, void*, void* tag, U id);       // [1011]
U __fastcall recovered_module_register_tag_004c7de0(void* module, void*, void* tag, U id);     // [1011]
void* __fastcall exo_locstring_construct_005e9e10(void* self, void*);                          // CExoLocString() [0265]
void __fastcall exo_locstring_destroy_005ea000(void* string, void*);                           // ~CExoLocString [0265]
void* __fastcall recovered_localized_assign_005e9e70(void* l, void*, const void* src);         // CExoLocString::operator= [0886]
void* __fastcall recovered_inventory_construct_0055d290(void* self, void*, unsigned owner, unsigned flag); // CItemRepository [w2]
void __fastcall kotor_fastcall_00555de0(void* self, void*);                                    // ~CItemRepository [0718]
void __fastcall recovered_object_destroy_004d0220(void* self, void*);                          // ~CSWSObject [0963]
void* __fastcall recovered_object_area_004cb120(void* object, void*);                          // GetArea [1060]
void __fastcall kotor_fastcall_004cb160(void* object, void*, void* area);                     // SetArea
void __fastcall recovered_object_position_004cd1a0(void* object, void*, const Vector* p, U a, U b, U c); // SetPosition
unsigned __fastcall recovered_area_insert_0050dfd0(void* area, void*, unsigned id, unsigned flag); // AddObjectToArea [1065]
void __fastcall frame_R4_005d0fc0(void* vm, void*, void* script, U id, U flag);                // RunScript
unsigned __fastcall recovered_vm_run_script_situation_005d0fd0(void* vm, void*, void* situation, U id, U flag); // [1194s]
void __fastcall recovered_event_destroy_004d7590(void* event, void*);                          // ~CScriptEvent
void __fastcall kotor_fastcall_00553040(void* object, void*, void* tag);                      // SetTag
void* __fastcall kotor_fastcall_004ae750(void* server, void*, U id);                          // GetGameObject [0530]
void* __fastcall recovered_item_construct_005530a0(void* self, void*, U id);                   // CSWSItem::CSWSItem [0963]
U __fastcall recovered_item_template_005608b0(void* item, void*, void* resref);                // CSWSItem::LoadFromTemplate [0995]
U __fastcall recovered_load_item_00560970(void* item, void*, void* gff, void* s);              // CSWSItem::LoadItem [w2]
U __fastcall recovered_item_cost_00554000(void* item, void*);                                  // CSWSItem::GetCost [w2_g07]
void __fastcall w1x_pointer_list_insert_00616940(void* list, void*, U value, U index);         // CExoArrayList<void*>::Insert
unsigned __fastcall recovered_store_add_item_005c70c0(void* store, void*, void* cell);         // CSWSStore::AddItemToInventory [w2]
void __fastcall recovered_item_owner_00553210(void* item, void*, U owner, U a, U b, U c);      // CSWSItem::SetPossessor [1013]
void* __fastcall frame_LogStringCtor_005e5a90(void* self, void*, const char* text);            // CExoString(char const*)
void __fastcall frame_LogStringDtor_005e5c20(void* self, void*);                               // ~CExoString
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                 // CExoString::operator=
const char* __fastcall frame_LogCStr_005e5670(void* s, void*);                                 // CExoString::CStr
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                    // CExoString::Format
void __fastcall c2da_ref_to_string_00405f70(void* ref, void*, void* out_string);               // CResRef::CopyToString
void* __fastcall gui_load_gff_construct_00410630(void* gff, void*, unsigned type, const char* ext, const void* ref); // CResGFF
U __fastcall kotor_fastcall_00411240(void* gff, void*, void* out_struct);                     // GetTopLevelStruct
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
void __fastcall w2x_object_state_004d1cf0(void* object, void*, void* gff, void* s);            // LoadObjectState [0886]
void __stdcall crt_eh_vector_constructor_006fb55f(void* first, U size, I count, void* construct, void* destroy);
void __stdcall c2da_destroy_array_006fa852(void* first, U size, I count, void* destroy);
}

namespace {
using namespace w5;
constexpr U k_invalid_00746644 = 0x7f000000u;
constexpr U k_event_timed_007441b8 = 1;
constexpr U k_event_script_007441dc = 10;
constexpr U k_event_destroy_007441e0 = 11;
constexpr U8 k_store_type_0074663d = 0x0e;               // .rdata 0074663D byte 0Eh
inline unsigned char* bytes(void* p) { return static_cast<unsigned char*>(p); }
inline void* vm() { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(frame_control::message_manager_007a3a00)); }
inline void* ai() { return recovered_server_ai_master_004aed80(server_app(), nullptr); }
struct ExoString { void* data; U length; };
struct LocString { unsigned char raw[0x10]; };          // CExoLocString storage (ctor/dtor/assign via providers)
struct ResRef { unsigned char raw[0x10]; };
}

extern "C" {
extern const void* const kotor_vtable_0074bdb8[56];

// 005C6AB0 CSWSStore::CSWSStore(ulong id): RET 4.
void* __fastcall kotor_fastcall_005c6ab0(void* self, void*, U id) {
    kotor_fastcall_004cfcb0(self, nullptr, k_store_type_0074663d, id, 0);                        // 005C6AC6..E1
    at<const void*>(self, 0) = kotor_vtable_0074bdb8;                                            // 005C6AFF
    crt_eh_vector_constructor_006fb55f(bytes(self) + 0x228, 8, 1,
        reinterpret_cast<void*>(&kotor_movie::string_element_construct_005b3190),
        reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20));                                 // 005C6AE6..B05
    exo_locstring_construct_005e9e10(bytes(self) + 0x234, nullptr);                              // 005C6B0A..15
    at<U16>(self, 0x254) = 0;                                                                    // 005C6B21
    void* memory = live_operator_new(0x18);                                                      // 005C6B1A..28
    at<void*>(self, 0x230) = memory != nullptr
        ? recovered_inventory_construct_0055d290(memory, nullptr, at<U>(self, 4), 0) : nullptr;  // 005C6B30..4D
    at<U>(self, 0x24c) = 0; at<U>(self, 0x250) = 0; at<U8>(self, 0x256) = 3;                     // 005C6B53..5F
    (void)kotor_fastcall_004b0850(ai(), nullptr, self, 0);                                       // 005C6B66..7D
    return self;
}

// 005C6BF0 CSWSStore::~CSWSStore: RET 0.
void __fastcall kotor_fastcall_005c6bf0(void* self, void*) {
    at<const void*>(self, 0) = kotor_vtable_0074bdb8;                                            // 005C6C0E
    void* repository = at<void*>(self, 0x230);                                                   // 005C6C14
    if (repository != nullptr) { kotor_fastcall_00555de0(repository, nullptr); live_free(repository); } // 005C6C1A..33
    at<U>(self, 0x240) = k_invalid_00746644;                                                     // 005C6C38
    (void)recovered_object_area_004cb120(self, nullptr);                                         // 005C6C36..42 (result unused)
    (void)recovered_ai_remove_004af3d0(ai(), nullptr, self);                                     // 005C6C47..57
    void* module = server_get_module_004ae6b0(server_app(), nullptr);                            // 005C6C5C..65
    if (module != nullptr) (void)recovered_module_remove_tag_004c5ef0(module, nullptr, bytes(self) + 0x18, at<U>(self, 4)); // 005C6C6A..78
    exo_locstring_destroy_005ea000(bytes(self) + 0x234, nullptr);                                // 005C6C7D..88
    c2da_destroy_array_006fa852(bytes(self) + 0x228, 8, 1, reinterpret_cast<void*>(&frame_LogStringDtor_005e5c20)); // 005C6C8D..A2
    recovered_object_destroy_004d0220(self, nullptr);                                            // 005C6CA7..B1
}

// 005C7160 scalar deleting destructor: RET 4.
void* __fastcall kotor_fastcall_005c7160(void* self, void*, U flags) {
    kotor_fastcall_005c6bf0(self, nullptr);                                                      // 005C7161..63
    if (flags & 1) live_free(self);                                                              // 005C7168..75
    return self;
}

// 005C6E80 CSWSStore::AddToArea(CSWSArea*, x, y, z, int flag): RET 14h.
void __fastcall kotor_fastcall_005c6e80(void* self, void*, void* area, float x, float y, float z, U flag) {
    if (area == nullptr) return;                                                                 // 005C6E87..8C
    kotor_fastcall_004cb160(self, nullptr, area);                                                // 005C6E8E..8F
    const Vector pos = {x, y, z};                                                                // 005C6E94..B5
    recovered_object_position_004cd1a0(self, nullptr, &pos, 0, 1, 0);                            // 005C6EA0..B9
    (void)recovered_area_insert_0050dfd0(recovered_object_area_004cb120(self, nullptr), nullptr, at<U>(self, 4), flag); // 005C6EBE..D0
}

// 005C6EE0 CSWSStore::EventHandler(type, caller, event, a, b): RET 14h.
void __fastcall kotor_fastcall_005c6ee0(void* self, void*, U type, U caller, void* event, U, U) {
    if (type == k_event_script_007441dc) {                                                       // 005C6EE0..EA
        if (at<U16>(event, 0) == 0x16) {                                                         // 005C6EED..F5
            at<U>(self, 0x23c) = caller;                                                         // 005C6EFA..00
            frame_R4_005d0fc0(vm(), nullptr, bytes(self) + 0x228, at<U>(self, 4), 1);            // 005C6EF7..14 OnOpenStore
        }
        recovered_event_destroy_004d7590(event, nullptr);                                        // 005C6F19..1B
        live_free(event);                                                                        // 005C6F20..26
        return;
    }
    if (type == k_event_timed_007441b8) {                                                        // 005C6F2D..33
        if (event != nullptr)
            (void)recovered_vm_run_script_situation_005d0fd0(vm(), nullptr, event, at<U>(self, 4), 1); // 005C6F35..4A
        return;
    }
    if (type == k_event_destroy_007441e0 && self != nullptr) vcall<void*>(self, 0x00, 1u);       // 005C6F52..62
}

// 005C7180 CSWSStore::LoadStore(CResGFF*, CResStruct*, int from_template): RET 0Ch, EAX 1.
U __fastcall kotor_fastcall_005c7180(void* self, void*, void* gff, void* s, U from_template) {
    unsigned ok = 0;                                                                             // [esp+10]
    {
        ExoString fallback; frame_LogStringCtor_005e5a90(&fallback, nullptr, "");                // 005C719E..A7 [esp+20]
        ExoString tag;                                                                           // [esp+18]
        void* t = gui_load_gff_string_00411ec0(gff, nullptr, &tag, s, "Tag", &ok, &fallback);   // 005C71AC..DC ([00747A28])
        kotor_fastcall_00553040(self, nullptr, t);                                               // 005C71E1..EC
        frame_LogStringDtor_005e5c20(&tag, nullptr);                                             // 005C71F1..FA
        frame_LogStringDtor_005e5c20(&fallback, nullptr);                                        // 005C71FF..0B
    }
    {
        LocString fallback; void* f = exo_locstring_construct_005e9e10(&fallback, nullptr);      // 005C7210..14 [esp+18]
        LocString name;                                                                          // [esp+20]
        void* n = recovered_gff_localized_00411fd0(gff, nullptr, &name, s, "LocName", &ok, f);   // 005C7219..38 ([007497E0])
        recovered_localized_assign_005e9e70(bytes(self) + 0x234, nullptr, n);                    // 005C723D..4C
        exo_locstring_destroy_005ea000(&name, nullptr);                                          // 005C7251..59
        exo_locstring_destroy_005ea000(&fallback, nullptr);                                      // 005C725E..6A
    }
    at<I>(self, 0x244) = gui_load_gff_int_00411c90(gff, nullptr, s, "MarkDown", &ok, 0);         // 005C726F..85
    at<I>(self, 0x248) = gui_load_gff_int_00411c90(gff, nullptr, s, "MarkUp", &ok, 0);           // 005C7284..A1
    ExoString script; kotor_movie::string_element_construct_005b3190(&script, nullptr);          // 005C729D..A7 [esp+28]
    {
        ResRef fallback; static_cast<menu_resources::Ref*>(static_cast<void*>(&fallback))->construct_00406d80(""); // 005C72B4..BD [esp+30]
        ResRef value;                                                                            // [esp+50]
        void* r = gui_load_gff_resref_00411e10(gff, nullptr, &value, s, "OnOpenStore", &ok, &fallback); // 005C72C2..DE ([0074BEB0])
        c2da_ref_to_string_00405f70(r, nullptr, &script);                                        // 005C72E3..E5
        async_assign_string_005e5c50(bytes(self) + 0x228, nullptr, &script);                     // 005C72EA..F5
    }
    // 005C72FA..07: the default byte replaces only the low byte of the struct-pointer argument slot
    const U flag_default = (static_cast<U>(reinterpret_cast<std::uintptr_t>(s)) & 0xffffff00u) | at<U8>(self, 0x256);
    at<U8>(self, 0x256) = gui_load_gff_byte_00411a60(gff, nullptr, s, "BuySellFlag", &ok, flag_default); // 005C730E..21
    struct { void** data; I size; I capacity; } items = {nullptr, 0, 0};                         // 005C7327..2F [esp+30..38]
    void* last = gff;                                                                            // [esp+84] argument slot reuse
    unsigned char list[0x14] = {};                                                               // CResList: dword + char[16] label (GetList 004118C0 strncpy 16 at +4); [esp+60]
    if (gff_list_004118c0(gff, nullptr, list, s, "ItemList") != 0 &&                             // 005C7333..4F ([00747210])
        gff_list_count_00411940(gff, nullptr, list) != 0) {                                      // 005C7355..69
        U i = 0;                                                                                 // [esp+18]
        do {
            last = nullptr;                                                                      // 005C737F [esp+84] = ebx (0) every item (review A-4)
            U element[4] = {};                                                                   // [esp+14]
            (void)gff_list_element_00411990(gff, nullptr, element, list, i);                     // 005C7370..86 (result unchecked)
            const U id = gui_load_gff_dword_00411c20(gff, nullptr, element, "ObjectId", &ok, k_invalid_00746644); // 005C738B..A1
            if (kotor_fastcall_004ae750(server_app(), nullptr, id) == nullptr) {                 // 005C73A6..B8
                void* memory = live_operator_new(0x29c);                                         // 005C73BE..C3
                void* item = memory != nullptr ? recovered_item_construct_005530a0(memory, nullptr, id) : nullptr; // 005C73CB..E7
                last = item;                                                                     // 005C73F0
                if (item != nullptr) {                                                           // 005C73E9..F7
                    bool loaded;
                    if (from_template != 0) {                                                    // 005C73FD..04
                        ResRef fallback; static_cast<menu_resources::Ref*>(static_cast<void*>(&fallback))->construct_00406d80(""); // 005C7406..0F [esp+40]
                        ResRef res;                                                              // [esp+50]
                        (void)gui_load_gff_resref_00411e10(gff, nullptr, &res, element, "InventoryRes", &ok, &fallback); // 005C7414..2F
                        loaded = ok != 0 && recovered_item_template_005608b0(item, nullptr, &res) != 0; // 005C7434..48
                    } else {
                        loaded = recovered_load_item_00560970(item, nullptr, gff, element) != 0; // 005C745E..6D
                    }
                    if (!loaded) {
                        vcall<void*>(item, 0x00, 1u);                                            // 005C744A..50 / 005C746F..75
                        last = nullptr;                                                          // 005C7452 / 005C7477
                    } else {
                        const U8 infinite = gui_load_gff_byte_00411a60(gff, nullptr, element, "Infinite", &ok, 0); // 005C7483..95
                        at<U>(item, 0x288) ^= ((static_cast<U>(infinite) * 4) ^ at<U>(item, 0x288)) & 4; // 005C749A..B1
                        const U cost = recovered_item_cost_00554000(item, nullptr);              // 005C74B7..BE [esp+20]
                        I position = -1;                                                         // [esp+88] argument slot reuse
                        for (I k = 0; k < items.size && position == -1; ++k) {                   // 005C74C2..FF
                            if (recovered_item_cost_00554000(items.data[k], nullptr) < cost) position = k; // 005C74DF..F1 (unsigned)
                        }
                        if (position != -1) {                                                    // 005C7501..09
                            w1x_pointer_list_insert_00616940(&items, nullptr,
                                static_cast<U>(reinterpret_cast<std::uintptr_t>(item)), static_cast<U>(position)); // 005C750B..18
                        } else {
                            if (items.size == items.capacity) {                                  // 005C751F..27
                                menu_list_growth::allocate_005e03c0(reinterpret_cast<kotor_reconstruction::ArrayMutations*>(&items),
                                    nullptr, static_cast<unsigned>(items.capacity != 0 ? items.capacity * 2 : 0x10)); // 005C7529..3B
                            }
                            items.data[items.size] = item; ++items.size;                         // 005C7540..4C
                        }
                    }
                }
            }
            ++i;                                                                                 // 005C7550..5C
        } while (i < gff_list_count_00411940(gff, nullptr, list));                               // 005C7560..67
    }
    void** const data = items.data;                                                              // 005C756D (ebx)
    for (I k = items.size - 1; k >= 0; --k) {                                                    // 005C7571..AF
        if (recovered_store_add_item_005c70c0(self, nullptr, &data[k]) != 0 && last != nullptr)  // 005C7580..95
            recovered_item_owner_00553210(data[k], nullptr, at<U>(self, 4), 1, 1, k_invalid_00746644); // 005C7597..A6
    }
    (void)recovered_module_register_tag_004c7de0(server_get_module_004ae6b0(server_app(), nullptr), nullptr,
                                                  bytes(self) + 0x18, at<U>(self, 4));           // 005C75B1..C9
    live_free(data);                                                                             // 005C75CE..D7
    frame_LogStringDtor_005e5c20(&script, nullptr);                                              // 005C75DF..EB
    return 1;
}

// 005C7760 CSWSStore::LoadFromTemplate(CResRef*, int from_template): RET 8, EAX 0 when the UTM does not exist.
U __fastcall kotor_fastcall_005c7760(void* self, void*, void* resref, U from_template) {
    void* memory = live_operator_new(0xa0);                                                      // 005C777B..82
    void* gff = memory != nullptr ? gui_load_gff_construct_00410630(memory, nullptr, 0x803, "UTM ", resref) : nullptr; // 005C778A..AE ([0074BEF4])
    if (at<U>(gff, 0x94) == 0) {                                                                 // 005C77B0..C2 (null gff dereferenced: original)
        vcall<void*>(gff, 0x00, 1u);                                                             // 005C77C8..CC
        ExoString message; kotor_movie::string_element_construct_005b3190(&message, nullptr);    // 005C77CE..D2 [esp+18]
        ExoString name; kotor_movie::string_element_construct_005b3190(&name, nullptr);          // 005C77D7..E3 [esp+10]
        c2da_ref_to_string_00405f70(resref, nullptr, &name);                                     // 005C77E8..F4
        frame_LogFormat_005e5680(&message, "Store template %s doesn't exist.\n", frame_LogCStr_005e5670(&name, nullptr)); // 005C77F9..12
        frame_LogStringDtor_005e5c20(&name, nullptr);                                            // 005C7815..1E
        frame_LogStringDtor_005e5c20(&message, nullptr);                                         // 005C7823..2F
        return 0;
    }
    U top[4] = {};                                                                               // [esp+C]
    (void)kotor_fastcall_00411240(gff, nullptr, top);                                            // 005C784A..4F
    (void)kotor_fastcall_005c7180(self, nullptr, gff, top, from_template);                       // 005C7854..61
    vcall<void*>(gff, 0x00, 1u);                                                                 // 005C7866..6C
    return 1;
}

// 005057A0 CSWSArea::LoadStores(CResGFF*, CResStruct*, int load_state, int from_template): RET 10h, EAX 1.
U __fastcall kotor_fastcall_005057a0(void* self, void*, void* gff, void* parent, U load_state, U from_template) {
    unsigned char list[0x14] = {};                                                               // CResList: dword + char[16] label (GetList 004118C0 strncpy 16 at +4); [esp+58]
    if (gff_list_004118c0(gff, nullptr, list, parent, "StoreList") == 0) return 1;               // 005057B8..DA ([00747510])
    if (gff_list_count_00411940(gff, nullptr, list) == 0) return 1;                              // 005057E0..F4
    U i = 0;                                                                                     // [esp+10]
    do {
        U element[4] = {};                                                                       // argument slot reuse in the original
        unsigned ok = 0;                                                                         // argument slot reuse in the original
        if (gff_list_element_00411990(gff, nullptr, element, list, i) != 0 &&                    // 00505802..19
            static_cast<kotor_q05::CResGFF*>(gff)->GetElementType(
                reinterpret_cast<kotor_q05::StructHandle*>(element)) == 0xb) {                   // 0050581F..31
            const U id = gui_load_gff_dword_00411c20(gff, nullptr, element, "ObjectId", &ok, k_invalid_00746644); // 00505837..53
            void* memory = live_operator_new(0x258);                                             // 00505858..5F
            void* store = memory != nullptr ? kotor_fastcall_005c6ab0(memory, nullptr, id) : nullptr; // 00505867..86
            if (store != nullptr) {                                                              // 00505888..92
                U loaded;
                if (from_template != 0) {                                                        // 00505898..A1
                    ResRef fallback; static_cast<menu_resources::Ref*>(static_cast<void*>(&fallback))->construct_00406d80(""); // 005058A3..AC
                    ResRef res;                                                                  // [esp+4C]
                    (void)gui_load_gff_resref_00411e10(gff, nullptr, &res, element, "ResRef", &ok, &fallback); // 005058B1..D2 ([00747508])
                    loaded = ok != 0 ? kotor_fastcall_005c7760(store, nullptr, &res, 1) : 0;     // 005058D7..F0
                } else {
                    loaded = kotor_fastcall_005c7180(store, nullptr, gff, element, 0);           // 005058F2..FF
                }
                if (loaded == 0) {                                                               // 00505904..06
                    vcall<void*>(store, 0x00, 1u);                                               // 00505908..0E
                } else {
                    const float oz = gui_load_gff_float_00411d00(gff, nullptr, element, "ZOrientation", &ok, 0.0f); // 00505915..33
                    const float oy = gui_load_gff_float_00411d00(gff, nullptr, element, "YOrientation", &ok, 0.0f); // 00505937..55
                    const float ox = gui_load_gff_float_00411d00(gff, nullptr, element, "XOrientation", &ok, 0.0f); // 0050595C..7A
                    Vector v = {ox, oy, oz};                                                     // [esp+18]
                    const double len2 = (static_cast<double>(ox) * ox + static_cast<double>(oy) * oy)
                                        + static_cast<double>(oz) * oz;                          // 0050597E..B1
                    Vector orientation;
                    if (len2 > static_cast<double>(0.00001f)) {                                  // 005059B3..BE ([00747504])
                        Vector n;
                        orientation = *normalize_004ab180(&n, &v);                               // 005059C0..E2
                    } else {
                        orientation.x = 0.0f; orientation.y = 1.0f; orientation.z = 0.0f;        // 005059E4..0C
                    }
                    w1x_object_set_orientation_004cd170(store, nullptr, &orientation);           // 00505A10..1B
                    if (load_state != 0) w2x_object_state_004d1cf0(store, nullptr, gff, element); // 00505A20..2F
                    const U flag = load_state == 0 ? 1u : 0u;                                    // 00505A34..38
                    const float pz = gui_load_gff_float_00411d00(gff, nullptr, element, "ZPosition", &ok, 0.0f); // 00505A3B..55
                    const float py = gui_load_gff_float_00411d00(gff, nullptr, element, "YPosition", &ok, 0.0f); // 00505A5E..77
                    const float px = gui_load_gff_float_00411d00(gff, nullptr, element, "XPosition", &ok, 0.0f); // 00505A80..99
                    kotor_fastcall_005c6e80(store, nullptr, self, px, py, pz, flag);             // 00505A9E..A9
                }
            }
        }
        ++i;                                                                                     // 00505AAE..BA
    } while (i < gff_list_count_00411940(gff, nullptr, list));                                   // 00505ABE..C5
    return 1;
}

// 0074BDB8 CSWSStore vtable (56 slots, original order): the CSWSObject 00745F18 table with +00 deleting dtor, +64 self_return, +70 void_return (AIUpdate),
// +78 EventHandler.
const void* const kotor_vtable_0074bdb8[56] = {
    reinterpret_cast<const void*>(&kotor_fastcall_005c7160),                   // +00 deleting dtor
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +04
    reinterpret_cast<const void*>(&kotor_m15e_slot008_void8_005b5e90),         // +08
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +0C
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),          // +10 AsSWSObject
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +14
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +18
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +1C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +20
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +24
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +28
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +2C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +30 AsSWSCreature
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +34
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +38
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +3C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +40
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +44
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +48
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +4C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +50
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +54
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +58
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +5C
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +60
    reinterpret_cast<const void*>(&kotor_m15e_slot00c_self_00641db0),          // +64 AsSWSStore
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +68
    reinterpret_cast<const void*>(&kotor_m15e_slot010_zero_0063e7f0),          // +6C
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),         // +70 AIUpdate (void_return)
    reinterpret_cast<const void*>(&kotor_fastcall_004cc390),                   // +74 ClearAction
    reinterpret_cast<const void*>(&kotor_fastcall_005c6ee0),                   // +78 EventHandler
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf60),                   // +7C SetAnimation
    reinterpret_cast<const void*>(&kotor_fastcall_004d0170),                   // +80 GetDialogResref
    reinterpret_cast<const void*>(&kotor_m15e_slot0d4_true_0063e7a0),          // +84 return_true
    reinterpret_cast<const void*>(&kotor_fastcall_004cb190),                   // +88 GetGender
    reinterpret_cast<const void*>(&kotor_fastcall_004d0190),                   // +8C GetLastName
    reinterpret_cast<const void*>(&kotor_fastcall_004d0190),                   // +90 GetLastName
    reinterpret_cast<const void*>(&kotor_fastcall_004cb810),                   // +94 GetDead
    reinterpret_cast<const void*>(&kotor_fastcall_004d01a0),                   // +98 GetMaxHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004caec0),                   // +9C GetCurrentHitPoints
    reinterpret_cast<const void*>(&kotor_fastcall_004ccf80),                   // +A0 DoDamage
    reinterpret_cast<const void*>(&kotor_fastcall_004d09e0),                   // +A4 DoDamageReduction
    reinterpret_cast<const void*>(&kotor_fastcall_004d0e40),                   // +A8 DoDamageResistance
    reinterpret_cast<const void*>(&kotor_fastcall_004cf160),                   // +AC DoDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caee0),                   // +B0 GetDamageImmunity
    reinterpret_cast<const void*>(&kotor_fastcall_004caf70),                   // +B4 GetDamageImmunityByFlags
    reinterpret_cast<const void*>(&c2da_resource_constant_word_00406490),      // +B8 GetEffectSpellId (-1)
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +BC SetEffectSpellId (void)
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +C0
    reinterpret_cast<const void*>(&kotor_fastcall_004d01b0),                   // +C4 GetLockOrientationToObject
    reinterpret_cast<const void*>(&kotor_fastcall_004d01c0),                   // +C8 GetPortrait
    reinterpret_cast<const void*>(&recovered_object_portrait_ref_004d01f0),    // +CC SetPortrait
    reinterpret_cast<const void*>(&recovered_creature_portrait_004d0210),      // +D0 GetPortraitId
    reinterpret_cast<const void*>(&recovered_object_portrait_004cad20),        // +D4 SetPortraitId
    reinterpret_cast<const void*>(&kotor_m15e_slot090_void0_004015a0),         // +D8
    reinterpret_cast<const void*>(&slot_void_return_4_0060e760_a1),            // +DC
};
}
