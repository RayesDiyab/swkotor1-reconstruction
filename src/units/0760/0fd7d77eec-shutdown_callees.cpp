// lane-m46-startup: graceful-shutdown callees still MISSING below routes-lane shutdown_bodies.cpp
// (declarations: routes-lane/routes/kotor_original_calls.h:127-155). Evidence: lane-m46-startup/dis/<addr>.asm.
// Style follows shutdown_bodies.cpp: raw offsets, original allocator family (006FA390 = live_free), canonical
// StartupString destructor for 005E5C20, reverse-order element destruction for the compiler `eh vector destructor
// iterator` 006FA852 (elements destroyed last-to-first, then the cookie block freed).
#include "6bdce58525-winmain_routes.h"
#include "4b90ecc798-startup_string.h"
#include "096ca033a8-ini_cleanup.h"       // base_ini_reconstruction::destroy_list_005e9ab0 (List = kotor_list::Internal)
#include "d08b97bcca-allocator_live.h"
#include <cstdint>
#include <cstring>
#include <new>

namespace kotor_menu20 { struct NetLayer; extern NetLayer* volatile network_head_0083288c; }   // menu20-bindings owner
extern "C" void __fastcall file_destroy_005e69e0(void*, void*);   // resource-successor-file-destructor (CExoFile::~CExoFile)

namespace m46_shutdown {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
Word word_at(const void* o, Word off) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(o) + off, 4); return v; }
void put_word(void* o, Word off, Word v) { std::memcpy(static_cast<unsigned char*>(o) + off, &v, 4); }
void* pointer_at(const void* o, Word off) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(word_at(o, off))); }
String* string_at(void* o, Word off) { return std::launder(reinterpret_cast<String*>(static_cast<unsigned char*>(o) + off)); }
void destroy_string(void* p) { std::launder(static_cast<String*>(p))->~String(); }   // 005E5C20 (also 0059EA30 = jmp 005E5C20)
// `if (p=[o+off]) free(p); [o+off]=0` (clear only where the original clears).
void free_member(void* o, Word off, bool clear) { if (void* p = pointer_at(o, off)) live_free(p); if (clear) put_word(o, off, 0); }
// 006FA852(array, size, [array-4], dtor) + free(array-4): elements last-to-first, element dtor = String at `inner`.
void destroy_cookie_array(void* array, Word element_size, Word inner) {
    unsigned char* const base = static_cast<unsigned char*>(array);
    const Word count = word_at(base - 4, 0);
    for (Word n = count; n != 0; --n) destroy_string(base + (n - 1) * element_size + inner);
    live_free(base - 4);
}
// Embedded fixed array (no cookie): 006FA852(base, size, count, dtor).
void destroy_embedded_array(void* base, Word element_size, Word count) {
    for (Word n = count; n != 0; --n) destroy_string(static_cast<unsigned char*>(base) + (n - 1) * element_size);
}
// `if (l=[o+off]) { 005E9AB0(l); free(l) }` (CExoLinkedList internal teardown).
void destroy_list_member(void* o, Word off) {
    if (void* list = pointer_at(o, off)) {
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<base_ini_reconstruction::List*>(list), nullptr);
        live_free(list);
    }
}
// Scalar deleting destructor through the object's live table slot 0 (thiscall, flag on stack, RET 4). D21 naked
// veneer: reload [ecx] and tail-jump [table+0]; no C++ function-pointer cast. (D41 typed-table publication for the
// game-object classes is not available in M45; recorded review item.)
__declspec(naked) void __fastcall vcall_slot0_delete(void*, void*, Word) { __asm { mov eax, [ecx] } __asm { jmp dword ptr [eax] } }
}  // namespace m46_shutdown

using namespace m46_shutdown;

// 005FC6F0..005FCA21 CClientExoAppInternal::~CClientExoAppInternal (EH states 15h..0 over the members below).
extern "C" void __fastcall kotor_fastcall_005fc6f0(void* self, void*) {
    if (void* p = pointer_at(self, 0x270)) { kotor_fastcall_004015a0(p, nullptr); live_free(p); put_word(self, 0x270, 0); }   // 005FC70F..005FC733
    if (void* wrapper = pointer_at(self, 0x1d0)) {                                                    // 005FC739..005FC741
        if (void* list = pointer_at(wrapper, 0)) {                                                    // 005FC744..005FC748
            base_ini_reconstruction::destroy_list_005e9ab0(static_cast<base_ini_reconstruction::List*>(list), nullptr);   // 005FC74C
            live_free(list);                                                                          // 005FC752
        }
        live_free(wrapper);                                                                           // 005FC75B (no clear)
    }
    free_member(self, 0x0, true);                                                                     // 005FC764..005FC773
    if (void* stream = pointer_at(self, 0x3c4)) {                                                     // 005FC775..005FC77D
        kotor_fastcall_005d6200(stream, nullptr);                                                     // 005FC781 CExoStreamingSoundSource::~ (lane-sound)
        live_free(stream); put_word(self, 0x3c4, 0);                                                  // 005FC787..005FC78F
    }
    if (void* a = pointer_at(self, 0x4c8)) destroy_cookie_array(a, 0x38, 0);                          // 005FC795..005FC7BE (dtor 0059EA30)
    put_word(self, 0x4c8, 0);                                                                         // 005FC7D1
    destroy_embedded_array(static_cast<unsigned char*>(self) + 0x418, 0x10, 7);                       // 005FC7C1..005FC7DC (dtor 0059EA30)
    if (void* a = pointer_at(self, 0x358)) destroy_cookie_array(a, 0x0c, 4);                          // 005FC7E1..005FC80A (dtor 0059EA20 = ~String(+4))
    put_word(self, 0x358, 0);                                                                         // 005FC80D
    free_member(self, 0x34c, true);                                                                   // 005FC813..005FC824 (unconditional free(NULL) ok)
    free_member(self, 0x340, true);                                                                   // 005FC82A..005FC844
    string_at(self, 0x330)->~String();                                                                // 005FC83E..005FC84F
    free_member(self, 0x314, true);                                                                   // 005FC854..005FC86E
    string_at(self, 0x304)->~String();                                                                // 005FC868..005FC879
    string_at(self, 0x2fc)->~String();                                                                // 005FC87E..005FC889
    free_member(self, 0x2f0, true);                                                                   // 005FC88E..005FC89F
    if (void* a = pointer_at(self, 0x2e4)) destroy_cookie_array(a, 8, 0);                             // 005FC8A5..005FC8D1 (dtor 005E5C20)
    put_word(self, 0x2e4, 0);                                                                         // 005FC8D4
    if (void* a = pointer_at(self, 0x2d8)) destroy_cookie_array(a, 8, 0);                             // 005FC8DA..005FC903
    put_word(self, 0x2d8, 0);                                                                         // 005FC906
    free_member(self, 0x2a8, true);                                                                   // 005FC90C..005FC926
    string_at(self, 0x294)->~String();                                                                // 005FC920..005FC931
    string_at(self, 0x1e4)->~String();                                                                // 005FC936..005FC941
    if (void* a = pointer_at(self, 0x1d8)) destroy_cookie_array(a, 8, 0);                             // 005FC946..005FC96F
    put_word(self, 0x1d8, 0);                                                                         // 005FC972
    destroy_list_member(self, 0x190);                                                                 // 005FC978..005FC994 (no clear)
    destroy_list_member(self, 0x168);                                                                 // 005FC997..005FC9B3
    string_at(self, 0x13c)->~String();                                                                // 005FC9B6..005FC9C1
    destroy_list_member(self, 0x98);                                                                  // 005FC9C6..005FC9E2
    string_at(self, 0x74)->~String();                                                                 // 005FC9E5..005FC9EC
    destroy_list_member(self, 0x1c);                                                                  // 005FC9F1..005FCA0D
}

// 004B1360..004B13D6 CServerInfo::~CServerInfo: ~String +FC, +20, +18, +10, +8 (EH states 3..0).
extern "C" void __fastcall kotor_fastcall_004b1360(void* self, void*) {
    string_at(self, 0xfc)->~String(); string_at(self, 0x20)->~String(); string_at(self, 0x18)->~String();
    string_at(self, 0x10)->~String(); string_at(self, 0x8)->~String();
}

// 005D5600..005D564C CNetLayer::ShutDown (EAX 0/1; consumer declares void).
//  if [+20010]==0 return 0; if [+2003C]==1 [+2003C]=0; n=[+2000C]; if n [n+2000C]=0;
//  if [0083288C]==this [0083288C]=[+2000C] (reloaded); [+20010]=0; return 1.
extern "C" void __fastcall kotor_fastcall_005d5600(void* self, void*) {
    if (word_at(self, 0x20010) == 0) return;                                                          // 005D5600..005D560E
    if (word_at(self, 0x2003c) == 1) put_word(self, 0x2003c, 0);                                      // 005D560F..005D5618
    if (void* next = pointer_at(self, 0x2000c)) put_word(next, 0x2000c, 0);                           // 005D561E..005D5628
    if (static_cast<void*>(kotor_menu20::network_head_0083288c) == self)                              // 005D562E..005D5634
        kotor_menu20::network_head_0083288c = static_cast<kotor_menu20::NetLayer*>(pointer_at(self, 0x2000c));   // 005D5636..005D563C
    put_word(self, 0x20010, 0);                                                                       // 005D5641
}

// 005D5720..005D57C0 CNetLayer::~CNetLayer: if [+20010]==1 ShutDown; ~String +20064,+20058,+20050,+20048,+20040,+20018.
extern "C" void __fastcall kotor_fastcall_005d5720(void* self, void*) {
    if (word_at(self, 0x20010) == 1) kotor_fastcall_005d5600(self, nullptr);                         // 005D573D..005D574E
    string_at(self, 0x20064)->~String(); string_at(self, 0x20058)->~String(); string_at(self, 0x20050)->~String();
    string_at(self, 0x20048)->~String(); string_at(self, 0x20040)->~String(); string_at(self, 0x20018)->~String();
}

// 005F1020..005F1082 CExoImeUI::~CExoImeUI: ~String +120, +118, +110.
extern "C" void __fastcall kotor_fastcall_005f1020(void* self, void*) {
    string_at(self, 0x120)->~String(); string_at(self, 0x118)->~String(); string_at(self, 0x110)->~String();
}

// 004D7E10..004D7E86 CGameObjectArray::~CGameObjectArray.
//  buckets = [+14] ? 2000h : 1000h (neg/sbb/and/add); for i<buckets: while (e=[[this]+i*4]) { o=[e+4];
//  if o: o->vtable[0](1); if [[this]+i*4]==e { [[this]+i*4]=[e+8]; free(e) } }; free([this]); [this]=0.
extern "C" void __fastcall kotor_fastcall_004d7e10(void* self, void*) {
    const std::int32_t buckets = word_at(self, 0x14) != 0 ? 0x2000 : 0x1000;                          // 004D7E14..004D7E22
    for (std::int32_t i = 0; i < buckets; ++i) {                                                      // 004D7E28..004D7E6F
        if (word_at(pointer_at(self, 0), static_cast<Word>(i) * 4) == 0) continue;                    // 004D7E30..004D7E36
        do {
            void* const entry = pointer_at(pointer_at(self, 0), static_cast<Word>(i) * 4);            // 004D7E38..004D7E3A
            if (void* object = pointer_at(entry, 4)) vcall_slot0_delete(object, nullptr, 1);          // 004D7E3D..004D7E48
            unsigned char* const slot = static_cast<unsigned char*>(pointer_at(self, 0)) + static_cast<Word>(i) * 4;   // 004D7E4A..004D7E51
            if (pointer_at(slot, 0) == entry) {                                                       // 004D7E4F..004D7E54
                put_word(slot, 0, word_at(entry, 8));                                                 // 004D7E56..004D7E5A
                live_free(entry);                                                                     // 004D7E59..004D7E5C
            }
        } while (word_at(pointer_at(self, 0), static_cast<Word>(i) * 4) != 0);                        // 004D7E64..004D7E6A
    }
    live_free(pointer_at(self, 0));                                                                   // 004D7E72..004D7E75
    put_word(self, 0, 0);                                                                             // 004D7E7F
}

// 004C4030..004C407B CWorldJournalEntry::~CWorldJournalEntry: ~String(+8), ~String(+0). (Element dtor of 00676DE0.)
extern "C" void __fastcall world_journal_entry_destroy_004c4030(void* self, void*) {
    string_at(self, 8)->~String(); string_at(self, 0)->~String();
}
// 00676DE0..00676E5A CSWCWorldJournal::~CSWCWorldJournal: free([+C]); [+C]=0; a=[+0]: if a { 006FA852(a, 24h,
// [a-4], 004C4030) (last-to-first); free(a-4) }; [+0]=0.
extern "C" void __fastcall kotor_fastcall_00676de0(void* self, void*) {
    free_member(self, 0x0c, true);                                                                    // 00676DFD..00676E0E
    if (unsigned char* a = static_cast<unsigned char*>(pointer_at(self, 0))) {                        // 00676E15..00676E24
        const Word count = word_at(a - 4, 0);
        for (Word n = count; n != 0; --n) world_journal_entry_destroy_004c4030(a + (n - 1) * 0x24, nullptr);   // 00676E2D..00676E36
        live_free(a - 4);                                                                             // 00676E3B..00676E3C
    }
    put_word(self, 0, 0);                                                                             // 00676E49
}

// 00678880..006788C9 CSoundSetList::~CSoundSetList: free+clear +4, +8, +C (each only when set).
extern "C" void __fastcall kotor_fastcall_00678880(void* self, void*) {
    for (Word off : {0x4u, 0x8u, 0xcu}) if (void* p = pointer_at(self, off)) { live_free(p); put_word(self, off, 0); }
}

// 005EC5C0..005EC672 CExoDebugInternal::CloseLogFiles: if [+0] { for off in 4,C,10,14,18,8: f=[+off]:
// if f { 005E69E0(f); free(f); [+off]=0 } ; [+0]=0 }.
extern "C" void __fastcall kotor_fastcall_005ec5c0(void* self, void*) {
    if (word_at(self, 0) == 0) return;                                                                // 005EC5C4..005EC5CA
    for (Word off : {0x4u, 0xcu, 0x10u, 0x14u, 0x18u, 0x8u}) {                                        // 005EC5D1..005EC66A
        if (void* file = pointer_at(self, off)) { file_destroy_005e69e0(file, nullptr); live_free(file); put_word(self, off, 0); }
    }
    put_word(self, 0, 0);                                                                             // 005EC66D
}
