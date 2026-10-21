// lane-m48-b fork CLIENT, unit client-server-dtor: CServerExoAppInternal::~CServerExoAppInternal 004B7230 (routes-shutdown
// consumer, server teardown on the graceful shutdown route only) and its member destructors.
// Evidence: dumpbin /disasm swkotor.exe 004B7230..004B7463 (full contiguous body; Ghidra ranges split), 00401060..0040107B,
// 004AE5F0..004AE635, 00563D40..00563D8E, 00528DE0..00528E46, 00555DE0..00555DF7, 005C58F0..005C591F.
// EH states 8..0 of 004B7230 only re-run the remaining member destructors; no callee throws (D30-FINAL).
#include "e6446a7354-r4_lowers.h"

using namespace r4_lowers;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
__declspec(noinline) void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);
}
namespace kotor_movie { void __fastcall string_element_destroy_005e5c20(void*, void*); }
extern "C" {
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
void __fastcall kotor_fastcall_004015a0(void*, void*);           // RET-only element destructor (routes-shutdown)
void __fastcall kotor_fastcall_005c5500(void*, void*);           // SJournalEntry::~ (client-dtors)
void* __fastcall kotor_fastcall_00401060(void* app_manager, void*, U32 which);
void __fastcall kotor_fastcall_004ae5f0(void* tables, void*);
void __fastcall kotor_fastcall_00555de0(void* repository, void*);
void __fastcall kotor_fastcall_005c58f0(void* journal, void*);
void __fastcall kotor_fastcall_00563d40(void* party_table, void*);
void __fastcall kotor_fastcall_00528de0(void* globals, void*);
void __fastcall kotor_fastcall_004b7230(void* internal, void*);
}
// 00831FDC: .bss word (beyond the .data raw size), cleared by 004B7397; no other owner in M47 -> single storage here.
namespace kotor_server_cells { extern volatile U32 word_00831fdc; volatile U32 word_00831fdc = 0; }

namespace {
void destroy_list_object(void* list) {   // pattern: destroy_list([list]) + free([list]) then free(list) (004B729F..004B72BB)
    if (void* internal = pw(list, 0)) {
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(internal), nullptr);
        live_free(internal);
    }
    live_free(list);
}
// 004B7250..004B72BB / 004B72BE..004B731B: drain a CExoLinkedList<object*> (scalar-deleting destructor, slot 0 arg 1).
void drain_owned_list(void* self, U32 off) {
    void* list = pw(self, off);
    if (!list) return;
    while (w(pw(list, 0), 0) != 0) {
        void* object = base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(pw(pw(self, off), 0)), nullptr);
        if (object) vcall1(object, 0, 1);
        list = pw(self, off);
    }
    if (void* l = pw(self, off)) destroy_list_object(l);
}
}

// 00401060 CAppManager::GetObjectTableManager(which), RET 4: 0 -> [+0Ch], 1 -> [+10h], else 0.
extern "C" void* __fastcall kotor_fastcall_00401060(void* app_manager, void*, U32 which) {
    if (which == 0) return pw(app_manager, 0x0c);                       // 00401060..0040106B
    if (which == 1) return pw(app_manager, 0x10);                       // 0040106E..00401076
    return nullptr;                                                     // 00401079
}
// 004AE5F0 CObjectTableManager::~: for i < [+180h]: entry [this+i*4] -> free(entry[0]), free(entry), slot = 0.
extern "C" void __fastcall kotor_fastcall_004ae5f0(void* tables, void*) {
    for (U32 i = 0; i < w(tables, 0x180); ++i) {                        // 004AE5F3..004AE630 (unsigned, count reread)
        if (void* entry = pw(tables, i * 4)) {                          // 004AE601..004AE606
            if (void* data = pw(entry, 0)) live_free(data);             // 004AE608..004AE614
            live_free(entry);                                           // 004AE617..004AE61D
            sw(tables, i * 4, 0);                                       // 004AE620
        }
    }
}
// 00555DE0 CItemRepository::~: free([+0Ch]); [+0Ch] = 0.
extern "C" void __fastcall kotor_fastcall_00555de0(void* repository, void*) {
    live_free(pw(repository, 0x0c));                                    // 00555DE3..00555DEC
    sw(repository, 0x0c, 0);                                            // 00555DEF
}
// 005C58F0 CSWSJournal::~: delete[] 3Ch-byte SJournalEntry array at [+0]; [+0] = 0.
extern "C" void __fastcall kotor_fastcall_005c58f0(void* journal, void*) {
    if (void* first = pw(journal, 0)) {                                 // 005C58F3..005C58F7
        unsigned char* block = bytes(first) - 4;                        // 005C58FD
        c2da_destroy_array_006fa852(first, 0x3c, static_cast<std::int32_t>(w(block, 0)), &kotor_fastcall_005c5500); // 005C58F9..005C5909
        live_free(block);                                               // 005C590E..005C590F
    }
    sw(journal, 0, 0);                                                  // 005C5918
}
// 00563D40 CSWPartyTable::~: item repository [+118h] and journal [+11Ch].
extern "C" void __fastcall kotor_fastcall_00563d40(void* party_table, void*) {
    if (void* repository = pw(party_table, 0x118)) {                    // 00563D44..00563D4C
        kotor_fastcall_00555de0(repository, nullptr); live_free(repository); sw(party_table, 0x118, 0); // 00563D4E..00563D5E
    }
    if (void* journal = pw(party_table, 0x11c)) {                       // 00563D68..00563D70
        kotor_fastcall_005c58f0(journal, nullptr); live_free(journal); sw(party_table, 0x11c, 0); // 00563D72..00563D82
    }
}
// 00528DE0 CSWGlobalVariableTable::~: 5 CExoStrings at +B230h, 100 18h-byte entries at +A8D0h (RET-only element dtor).
extern "C" void __fastcall kotor_fastcall_00528de0(void* globals, void*) {
    c2da_destroy_array_006fa852(bytes(globals) + 0xb230, 8, 5, &kotor_movie::string_element_destroy_005e5c20); // 00528DFD..00528E15
    c2da_destroy_array_006fa852(bytes(globals) + 0xa8d0, 0x18, 0x64, &kotor_fastcall_004015a0); // 00528E1A..00528E32
}

// 004B7230 CServerExoAppInternal::~CServerExoAppInternal.
extern "C" void __fastcall kotor_fastcall_004b7230(void* internal, void*) {
    drain_owned_list(internal, 0x10064);                                // 004B7250..004B72BB
    drain_owned_list(internal, 0x10068);                                // 004B72BE..004B731B
    if (void* data = pw(internal, 0x10018)) live_free(data);            // 004B731E..004B732E
    if (void* list = pw(internal, 0x10070)) destroy_list_object(list);  // 004B7331..004B7357
    sw(internal, 0x10070, 0);                                           // 004B7362
    if (void* list = pw(internal, 0x10074)) destroy_list_object(list);  // 004B735A..004B7386
    sw(internal, 0x10074, 0);                                           // 004B7389
    kotor_server_cells::word_00831fdc = 0;                              // 004B7397
    kotor_fastcall_004ae5f0(kotor_fastcall_00401060(menu_continuation::app_manager_007a39fc, nullptr, 1), nullptr); // 004B738F..004B73A4
    kotor_movie::string_element_destroy_005e5c20(bytes(internal) + 0x1b93c, nullptr); // 004B73A9..004B73B4
    kotor_movie::string_element_destroy_005e5c20(bytes(internal) + 0x1b908, nullptr); // 004B73B9..004B73C4
    kotor_fastcall_00563d40(bytes(internal) + 0x1b770, nullptr);        // 004B73C9..004B73D4
    kotor_fastcall_00528de0(bytes(internal) + 0x100fc, nullptr);        // 004B73D9..004B73E4
    live_free(pw(internal, 0x100ec));                                   // 004B73E9..004B73F5
    sw(internal, 0x100ec, 0);                                           // 004B7403
    kotor_movie::string_element_destroy_005e5c20(bytes(internal) + 0x10094, nullptr); // 004B73FD..004B740E
    kotor_movie::string_element_destroy_005e5c20(bytes(internal) + 0x1008c, nullptr); // 004B7413..004B741E
    kotor_movie::string_element_destroy_005e5c20(bytes(internal) + 0x10084, nullptr); // 004B7423..004B742D
    live_free(pw(internal, 0x1001c));                                   // 004B7432..004B7441
    sw(internal, 0x1001c, 0);                                           // 004B744E
}
