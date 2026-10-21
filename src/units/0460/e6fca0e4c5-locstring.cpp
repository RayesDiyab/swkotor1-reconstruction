// lane-resman: CExoLocString / CExoLocStringInternal (needed by every
// CExoEncapsulatedFile: ctor 0040EFD1, dtor 0040F071, UnloadHeader 0040DBB4,
// LoadHeader 0040E53C). Authored from static disassembly of the pinned GOG PE.
//
// Observed layouts (raw offsets; nothing else claimed):
//   CExoLocString         (8):  +00 CExoLocStringInternal*  +04 DWORD strref (ctor -1)
//   CExoLocStringInternal (12): +00 kotor_list::Internal* (CExoLinkedList wrapper)
//                               +04 element count  +08 DWORD flag (set to 1 after a TLK fill)
//   element               (12): +00 DWORD id (= lang*2 + (lang ? gender : 0))  +04 CExoString
//   list node data word at node+8 (as in M40 manager_demand.cpp / original 005EC728).
#include "9f8b86cc9d-startup_string.h"
#include "e33c2d2042-constructors.h"   // kotor_list::Internal (M40 canonical-list-constructors, 005E9AA0)
#include "89f7135107-tlk_core.h"       // lane-tlk: tlk_core::StringResource (28-byte STR_RES), cell 007A3A08, Fetch 0041E550 (D44)
#include <cstdint>
#include <cstring>
#include <new>

using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;

namespace base_ini_reconstruction { struct Node; }
namespace base_ini_reconstruction {
__declspec(noinline) void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);   // M40 ini-list-cleanup
__declspec(noinline) void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);   // M40 ini-list-cleanup
}
namespace ini_section {
__declspec(noinline) base_ini_reconstruction::Node* __fastcall AddTail005e9dc0(kotor_list::Internal*, void*, void*); // M40 ini-add-tail
}
extern "C" {
void* __cdecl c2da_directory_new_006fa7e6(std::uint32_t) noexcept(false);  // 006FA7E6
void __cdecl live_free(void*);                                            // 006FA390
Word __fastcall unresolved_list_next_005e9b90(void*, void*, void*);        // 005E9B90 GetNext(&pos) -> data
void __fastcall c2da_005e9c10(void*, void*, void*);                       // 005E9C10 Remove(pos)
const char* __cdecl c2da_empty_0073d71c();                                // 0073D71C ""
int __fastcall async_string_equal_005e5200(String*, void*, String*);       // 005E5200 operator==(CExoString const&)
// TLK cell 007A3A08 (extern "C" void*) and tlk_fetch_0041e550 are declared by lane-tlk tlk_core.h.
// 00405ED0 CResRef::CResRef() (lane-tpc/lane-menu name; zeroes 16 bytes).
// 00405ED0 declared by task-10 table_contract.h (via lane-tlk tlk_core.h); duplicate declaration removed by M43 composer (C2733).

// Lane exports.
void* __fastcall exo_loclist_construct_005ec690(void* self, void*) noexcept(false);
void* __fastcall exo_locinternal_construct_005ecaf0(void* self, void*) noexcept(false);
void __fastcall exo_locinternal_clear_005ecb10(void* self, void*);
void __fastcall exo_locinternal_destroy_005ecb50(void* self, void*);
Word __fastcall exo_locinternal_get_005ec9c0(void* self, void*, Word id, String* out);
void __fastcall exo_locinternal_remove_005eca80(void* self, void*, Word id);
Word __fastcall exo_locinternal_add_005ec700(void* self, void*, Word id, String* text);
void* __fastcall exo_locstring_construct_005e9e10(void* self, void*) noexcept(false);
void __fastcall exo_locstring_destroy_005ea000(void* self, void*);
void __fastcall exo_locstring_clear_005e9ea0(void* self, void*);
Word __fastcall exo_locstring_get_005ea130(void* self, void*, Word language, String* out, Word gender);
Word __fastcall exo_locstring_add_005ea340(void* self, void*, Word language, String* text, Word gender);
}

namespace {
template<class T> T get(const void* p, Word off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
template<class T> void put(void* p, Word off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v)); }
kotor_list::Internal* list_of(void* internal) { return get<kotor_list::Internal*>(internal, 0); }
String* text_of(void* element) { return reinterpret_cast<String*>(static_cast<unsigned char*>(element) + 4); }
void* first(kotor_list::Internal* list, void** position) {
    *position = get<void*>(list, 0);                               // [list] head node
    return *position ? get<void*>(*position, 8) : nullptr;        // node+8 data
}
void destroy_element(void* element) {                             // 0059EA20 (ICF: ~CExoString at +4) + free
    text_of(element)->~StartupString();
    live_free(element);
}
// id = lang*2 + (lang ? gender : 0) (005EA35E-005EA385 / 005EA154-005EA16A; gender is a byte).
Word element_id(Word language, Word gender, Word* effective_gender) {
    const Word g = language ? (gender & 0xffu) : 0u;
    *effective_gender = g;
    return language * 2u + g;
}
}

// 005EC690 CExoLinkedList<ExoLocString_st>::CExoLinkedList: new(0xC); Internal::Construct
// (005E9AA0) on success; [this] = result or 0. RET0, EAX=this.
extern "C" __declspec(noinline) void* __fastcall exo_loclist_construct_005ec690(void* self, void*) noexcept(false) {
    void* allocation = c2da_directory_new_006fa7e6(0x0c);           // 005EC6AB
    if (allocation) {
        kotor_list::Internal* list = (new (allocation) kotor_list::Internal)->Construct(); // 005EC6C5
        put(self, 0, list);                                         // 005EC6CA
    } else {
        put<void*>(self, 0, nullptr);                               // 005EC6E4
    }
    return self;
}
// 005ECAF0 CExoLocStringInternal::CExoLocStringInternal.
extern "C" __declspec(noinline) void* __fastcall exo_locinternal_construct_005ecaf0(void* self, void*) noexcept(false) {
    (void)exo_loclist_construct_005ec690(self, nullptr);           // 005ECAF3
    put<Word>(self, 4, 0); put<Word>(self, 8, 0);                   // 005ECAFA-005ECAFD
    return self;
}
// 005ECB10 ClearLocString: while list head: RemoveHead, destroy element; [+4]=0.
extern "C" __declspec(noinline) void __fastcall exo_locinternal_clear_005ecb10(void* self, void*) {
    while (get<void*>(list_of(self), 0)) {                         // 005ECB13-005ECB18 / 005ECB3D-005ECB42
        void* element = base_ini_reconstruction::remove_head_005e9c80(list_of(self), nullptr); // 005ECB22
        if (element) destroy_element(element);                     // 005ECB2D-005ECB35
    }
    put<Word>(self, 4, 0);                                          // 005ECB45
}
// 005ECB50 ~CExoLocStringInternal: Clear; list wrapper dtor (005E9AB0 + free).
extern "C" __declspec(noinline) void __fastcall exo_locinternal_destroy_005ecb50(void* self, void*) {
    exo_locinternal_clear_005ecb10(self, nullptr);                  // 005ECB75
    kotor_list::Internal* list = list_of(self);                     // 005ECB7A
    if (list) {
        base_ini_reconstruction::destroy_list_005e9ab0(list, nullptr); // 005ECB8A
        live_free(list);                                            // 005ECB90
    }
}
// 005EC9C0 GetString(id, CExoString& out) RET8.
extern "C" __declspec(noinline) Word __fastcall exo_locinternal_get_005ec9c0(void* self, void*, Word id, String* out) {
    void* position;
    void* element = first(list_of(self), &position);               // 005EC9C4-005EC9D3
    while (element) {
        if (get<Word>(element, 0) == id) {                          // 005EC9E0
            out->Assign005e5c50(text_of(element));                  // 005EC9FC-005ECA04
            return 1;
        }
        element = reinterpret_cast<void*>(unresolved_list_next_005e9b90(list_of(self), nullptr, &position)); // 005EC9EB
    }
    return 0;
}
// 005ECA80 RemoveString(id) RET4; EAX unclaimed.
extern "C" __declspec(noinline) void __fastcall exo_locinternal_remove_005eca80(void* self, void*, Word id) {
    void* position;
    void* element = first(list_of(self), &position);
    while (element) {
        if (get<Word>(element, 0) == id) {                          // 005ECAA1
            c2da_005e9c10(list_of(self), nullptr, position);        // 005ECABE-005ECAC3
            destroy_element(element);                               // 005ECAC8-005ECAD0
            put<Word>(self, 4, get<Word>(self, 4) - 1u);            // 005ECAD5-005ECADC
            return;
        }
        element = reinterpret_cast<void*>(unresolved_list_next_005e9b90(list_of(self), nullptr, &position));
    }
}
// 005EC700 AddString(id, CExoString const&) RET8: overwrite existing id, else append.
extern "C" __declspec(noinline) Word __fastcall exo_locinternal_add_005ec700(void* self, void*, Word id, String* text) {
    void* position;
    void* element = first(list_of(self), &position);
    while (element) {
        if (get<Word>(element, 0) == id) {                          // 005EC733
            text_of(element)->Assign005e5c50(text);                 // 005EC76E-005EC776
            return 1;
        }
        element = reinterpret_cast<void*>(unresolved_list_next_005e9b90(list_of(self), nullptr, &position));
    }
    void* fresh = c2da_directory_new_006fa7e6(0x0c);                // 005EC748
    if (!fresh) return 0;                                           // 005EC795-005EC7B4
    ::new (text_of(fresh)) String();                                // 005EC764 (005B3190)
    put<Word>(fresh, 0, id);                                        // 005EC7BF
    text_of(fresh)->Assign005e5c50(text);                           // 005EC7C1
    (void)ini_section::AddTail005e9dc0(list_of(self), nullptr, fresh); // 005EC7C9
    put<Word>(self, 4, get<Word>(self, 4) + 1u);                    // 005EC7CE-005EC7D6
    return 1;
}

// 005E9E10 CExoLocString::CExoLocString: new(0xC) Internal (freed if its ctor throws,
// EH state 0); [+0]=internal or 0; [+4]=0xFFFFFFFF. C++ exceptions may propagate.
extern "C" __declspec(noinline) void* __fastcall exo_locstring_construct_005e9e10(void* self, void*) noexcept(false) {
    void* allocation = c2da_directory_new_006fa7e6(0x0c);           // 005E9E2B
    void* internal = nullptr;
    if (allocation) {
        try { internal = exo_locinternal_construct_005ecaf0(allocation, nullptr); } // 005E9E45
        catch (...) { live_free(allocation); throw; }                // EH state 0 (00724EFB)
    }
    put(self, 0, internal);                                          // 005E9E52
    put<Word>(self, 4, 0xffffffffu);                                 // 005E9E54
    return self;
}
// 005EA000 ~CExoLocString.
extern "C" __declspec(noinline) void __fastcall exo_locstring_destroy_005ea000(void* self, void*) {
    void* internal = get<void*>(self, 0);
    if (internal) {
        exo_locinternal_destroy_005ecb50(internal, nullptr);         // 005EA009
        live_free(internal);                                         // 005EA00F
    }
}
// 005E9EA0 CExoLocString::ClearLocString: tail jump to Internal::Clear (strref untouched).
extern "C" __declspec(noinline) void __fastcall exo_locstring_clear_005e9ea0(void* self, void*) {
    exo_locinternal_clear_005ecb10(get<void*>(self, 0), nullptr);
}
// 005EA130 CExoLocString::GetString(int lang, CExoString& out, u8 gender) RET0C, EAX 0/1.
extern "C" __declspec(noinline) Word __fastcall exo_locstring_get_005ea130(void* self, void*, Word language, String* out, Word gender) {
    Word effective_gender = 0;
    const Word id = element_id(language, gender, &effective_gender);
    if (exo_locinternal_get_005ec9c0(get<void*>(self, 0), nullptr, id, out)) return 1; // 005EA16D-005EA177
    // 005EA149 sub esp,1Ch: 28-byte STR_RES local (lane-tlk tlk_core::StringResource; D44).
    // +0 CExoString (005B3190 at 005EA196, destroyed 005EA218 incl. EH state 0),
    // +8 CResRef (00405ED0 at 005EA1A7), +18h written only by FetchInternal (0041E344/0041E350).
    tlk_core::StringResource token;
    String* text = &token.text;
    (void)c2da_default_ref_00405ed0(&token.sound, nullptr);          // 005EA1A7
    bool result = false;
    void* tlk = kotor_cell_tlk_007a3a08;                              // 005EA1AC mov ecx,[007A3A08]
    if (tlk && tlk_fetch_0041e550(static_cast<tlk_core::Tlk*>(tlk), nullptr, get<Word>(self, 4), &token, effective_gender)) { // 005EA1BE-005EA1D3
        out->Assign005e5c50(text);                                   // 005EA1D5-005EA1DC
        if (!out->BorrowedEquals005e5290(c2da_empty_0073d71c())) {   // 005EA1E1-005EA1EF (005E5390 == !005E5290, all null cases)
            (void)exo_locinternal_add_005ec700(get<void*>(self, 0), nullptr, id, out); // 005EA1F1-005EA1F5
            put<Word>(get<void*>(self, 0), 8, 1);                    // 005EA1FA-005EA1FC
        }
        result = true;                                               // 005EA203
    }
    return result ? 1u : 0u;
}
// 005EA340 CExoLocString::AddString(int lang, CExoString const& text, u8 gender) RET0C.
extern "C" __declspec(noinline) Word __fastcall exo_locstring_add_005ea340(void* self, void*, Word language, String* text, Word gender) {
    Word effective_gender = 0;
    const Word id = element_id(language, gender, &effective_gender);
    exo_locinternal_remove_005eca80(get<void*>(self, 0), nullptr, id); // 005EA387-005EA38A
    String current;                                                   // 005EA38F
    (void)exo_locstring_get_005ea130(self, nullptr, language, &current, effective_gender); // 005EA398-005EA3AD
    if (async_string_equal_005e5200(&current, nullptr, text)) return 1; // 005EA3B2-005EA3C2 (local == text)
    return exo_locinternal_add_005ec700(get<void*>(self, 0), nullptr, id, text); // 005EA3EE-005EA3F2 (id kept in EBX)
}
