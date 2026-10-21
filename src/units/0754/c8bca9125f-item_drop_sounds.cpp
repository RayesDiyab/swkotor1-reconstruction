// lane-m52-route REPLACEMENT COPY of M51 unit startup46-item-drop-sounds (startup46/item_drop_sounds.cpp
// sha 6e651a4c...). Only change: the router adapter at the end checks its own shape inline (M51 run exit 86
// router.contract_violation w0=005F93AF w1=005F0AD0: shape.cpp has no row for this site; 005F0AD0 is one of the
// three providers.json router_only_missing_services, the other two check inline). Body unchanged.
// lane-m46-startup: D21 C++ port of CClientExoAppInternal::LoadItemDropSoundList 005F0AD0..005F0C55
// (MASM donor 2026-10-02\task-8\service-005f0ad0-source-r3\source\service_core.asm e879f9c7..; same
// algorithm, re-expressed as C++ so the pinned cl emits the cleanup that the original SEH/C++ EH frame
// (handler 00725796 -> FuncInfo 00776144) performs). Router transport: services-router service_router.cpp
// case 0x005f0ad0 (call site 005F93AF, StartServices internal: unconditional, before the first menu frame).
//
// Original EH states (funclets 00725770..00725791, ebp frame of 005F0AD0):
//   state 0 (005F0B18..005F0B46): ctor 00413CC0 throws -> 006FA390 free(raw 0x54 block)
//   state 1 (005F0B75..005F0C36): ~String(list scratch)                  [frame -28h]
//   state 2 (005F0B8D..005F0BB3): 00401130 throws -> free(raw array) then state 1
//   state 3 (005F0BDB..005F0BEB): ~String(column name)                     then state 1
// C++ RAII below reproduces exactly these cleanups (String destructors, explicit catch-free of the two raw
// blocks). Quirks kept: a Load2DArray failure leaks the table (005F0B5D jumps straight to the exit); an
// array allocation failure stores 0 to +1C8 and the row loop still runs, addressing 0+row*16 (005F0BA4,
// 005F0C0C..005F0C19).
#include "188d5db40f-bridges.h"            // service_adapter_closure::checked, Call/Word transport
#include "74272cb0e4-table_contract.h"     // c2da_current::Table/TableSlot0, c2da_default_ref_00405ed0
#include "44a9f3b25b-getstring.h"          // c2da_current_get_string_00413270
#include "86c8ae5113-load.h"               // c2da_current_load_004143b0
#include "9e92e53d13-resources.h"          // menu_resources::Ref::construct_00406d80, unresolved_ref_assign_00406290
#include "8fffcbcb29-native_strings.h"     // WindowCStr005e5670
#include "3d785914bb-allocator_live.h"     // live_operator_new (006FA7E6), live_free (006FA390)
#include <cstdint>
#include <cstring>

extern "C" {
// Compiler `vector constructor iterator` 00401130 (stdcall, RET 10h); single provider world-los area_los.cpp
// (identical declaration of world_los.h:158).
void __stdcall vector_constructor_iterator_00401130(void* base, std::uint32_t element_size, std::uint32_t count,
                                                    void* (__fastcall* construct)(void*, void*));
// Router entry (services-router service_router.cpp:59 declaration).
service_adapter_closure::Word __cdecl kotor_menu_service_005F0AD0(const service_adapter_closure::Call*);
}

namespace kotor_item_drop_sounds {
using c2da_current::Table;
using c2da_current::String;
using Ref = menu_resources::Ref;
using Word = std::uint32_t;

// Original immutable literals pushed at 005F0B2D (0074E7FC) and 005F0BC0 (0074E7EC).
const char inventorysnds_0074e7fc[] = "inventorysnds";
const char inventorysound_0074e7ec[] = "InventorySound";

inline Word word_at(const void* object, Word offset) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(object) + offset, 4); return v; }
inline void put_word(void* object, Word offset, Word v) { std::memcpy(static_cast<unsigned char*>(object) + offset, &v, 4); }

// Typed element-constructor for 00401130: the original passes 00405ED0 (CResRef::CResRef(), thiscall, EAX=this)
// directly; the iterator's slot type is void*(fastcall)(void*,void*), so this adapter forwards the same element
// pointer to the single 00405ED0 owner (lane-m42-menu-joins) and returns its EAX. No function-pointer cast.
// [M45 compose] 00405ED0 body removed: earliest-staged owner c2da-default-ref (M42, D9) keeps the body (definition removed; declaration kept)
void* __fastcall ref_default_element_00405ed0(void* element, void*);

// 005F0AD0 body. `self` = CClientExoAppInternal* (ECX at 005F0AEF).
void load_item_drop_sound_list_005f0ad0(void* self) {
    if (word_at(self, 0x1c8) != 0) return;                                           // 005F0AF1..005F0B00
    Table* table = nullptr;
    if (void* raw = live_operator_new(0x54)) {                                       // 005F0B06..005F0B20
        try {                                                                         // EH state 0
            Ref name;
            name.construct_00406d80(inventorysnds_0074e7fc);                         // 005F0B22..005F0B32 (outgoing Ref slot)
            table = c2da_current_construct_00413cc0(raw, nullptr, name, 0);          // 005F0B37..005F0B3E (flag 0 pushed 005F0B22)
        } catch (...) { live_free(raw); throw; }                                      // funclet 00725770
    }
    if (!table) return;                                                               // 005F0B44..005F0B4E
    if (!c2da_current_load_004143b0(table, nullptr)) return;                         // 005F0B54..005F0B5D (table not freed)
    String scratch;                                                                   // 005F0B63..005F0B67 005B3190; EH state 1
    const Word rows = word_at(table, 0x24);                                           // 005F0B6C (read once for the array)
    void* array = live_operator_new(rows << 4);                                      // 005F0B6F..005F0B7D
    if (array) {                                                                      // 005F0B8B..005F0B92
        try {                                                                         // EH state 2
            vector_constructor_iterator_00401130(array, 0x10, rows, &ref_default_element_00405ed0);   // 005F0B94..005F0B9D
        } catch (...) { live_free(array); throw; }                                    // funclet 00725783
    }
    put_word(self, 0x1c8, static_cast<Word>(reinterpret_cast<std::uintptr_t>(array)));   // 005F0BA6
    std::uint8_t counter = 0;                                                         // 005F0BAF xor bl,bl
    if (static_cast<std::int32_t>(word_at(table, 0x24)) > 0) {                        // 005F0BAC..005F0BB8 (jle, reloaded)
        Word row = 0;                                                                 // 005F0BBA
        do {
            std::int32_t found;
            {
                String column(inventorysound_0074e7ec);                               // 005F0BC0..005F0BC9 005E5A90; EH state 3
                found = c2da_current_get_string_00413270(table, nullptr, static_cast<std::int32_t>(row), &column, &scratch);  // 005F0BCE..005F0BE0
            }                                                                         // 005F0BE5..005F0BF0 005E5C20
            if (found) {                                                              // 005F0BF5..005F0BF7
                const char* text = WindowCStr005e5670(&scratch, nullptr);            // 005F0BF9..005F0BFD
                Ref result;                                                           // 005F0C07 hidden out slot [esp+2Ch]
                const Word base = word_at(self, 0x1c8);                               // 005F0C02..005F0C0C (reloaded)
                Ref* target = reinterpret_cast<Ref*>(static_cast<std::uintptr_t>(base + (row << 4)));  // 005F0C12..005F0C17
                (void)unresolved_ref_assign_00406290(target, nullptr, &result, text); // 005F0C19
            }
            ++counter;                                                                // 005F0C21 inc bl
            row = counter;                                                            // 005F0C23 movzx edi,bl
        } while (static_cast<std::int32_t>(row) < static_cast<std::int32_t>(word_at(table, 0x24)));   // 005F0C1E..005F0C28 (jl)
    }
    // 005F0C2A..005F0C30: [table] slot 0 (scalar deleting destructor, flag 1). Read through the published typed
    // slot struct c2da_current::TableSlot0 (object-pointer view of the vtable word, D24/D41; no function-pointer cast).
    const c2da_current::TableSlot0* methods;
    std::memcpy(&methods, table, sizeof(methods));
    (void)methods->deleting(table, nullptr, 1);
}                                                                                     // 005F0C32..005F0C3E ~scratch
}  // namespace kotor_item_drop_sounds

// Router adapter: services-router routes site 005F93AF -> target 005F0AD0 with self = CClientExoAppInternal.
// The original is `void` (EAX dead at the call site): return 0 like the other void adapters (accepted.cpp).
// Shape is checked here (no shape.cpp row exists; same form as tlk_open.cpp 0041E5A0 / tlk_download.cpp 005F4180):
// original 005F93A3 mov ecx,esi (esi = CClientExoAppInternal this) ; 005F93AF call 005F0AD0 with no pushes ->
// thiscall, zero stack words; callee 005F0C55 plain `ret` (no stack cleanup); 005F93B4 overwrites ECX, EAX unused.
extern "C" service_adapter_closure::Word __cdecl kotor_menu_service_005F0AD0(const service_adapter_closure::Call* c) {
    if (!c || c->site != 0x005f93afu || c->target != 0x005f0ad0u ||
        c->abi != service_adapter_closure::Abi::Thiscall || c->count != 0u || !c->self)
        kotor_menu_service_contract_violation(c);
    kotor_item_drop_sounds::load_item_drop_sound_list_005f0ad0(reinterpret_cast<void*>(static_cast<std::uintptr_t>(c->self)));
    return 0;
}
