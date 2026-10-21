// lane-m48-b fork CLIENT, unit client-dtors: shutdown-path destructors referenced by routes-shutdown
// (shutdown_bodies.cpp: 00602C10, 00604B40, 006303B0). Reached only on the graceful shutdown routes (D37), never on
// the first frame. Evidence: dumpbin /disasm swkotor.exe (tools/dis/<addr>.txt), addresses cited per statement.
// EH: the original SEH states only re-run the remaining member destructors; no callee here throws (D30-FINAL), so the
// unwind tables are cited, not reproduced. Array deletes keep the original vector-deleting form: element destructor
// iterator 006FA852 over the count word at [p-4], then free(p-4) (006FA390 -> live_free).
#include "e6446a7354-r4_lowers.h"

using namespace r4_lowers;

namespace kotor_list { struct Internal; }
namespace base_ini_reconstruction {
__declspec(noinline) void __fastcall destroy_list_005e9ab0(kotor_list::Internal*, void*);
void* __fastcall remove_head_005e9c80(kotor_list::Internal*, void*);
}
namespace menu_tables_m41 {
void __fastcall LiveContentDestroy_0059ea30(void*, void*);     // 0059EA30 element destructor
void __fastcall DialogMessageDestroy_00628600(void*, void*);   // 00628600 element destructor
}
namespace kotor_movie { void __fastcall string_element_destroy_005e5c20(void*, void*); }   // 005E5C20 as element destructor
namespace sound_stream_play29_required { void __fastcall close_existing_005dc380(void*, void*); } // 005DC380
extern "C" {
void __stdcall c2da_destroy_array_006fa852(void* first, std::uint32_t size, std::int32_t count, void (__fastcall* destroy)(void*, void*));
std::uint32_t __fastcall unresolved_list_next_005e9b90(void*, void*, void**);
void __fastcall c2da_005e9c10(void*, void*, void*);
void __fastcall exo_locstring_destroy_005ea000(void*, void*);
void __fastcall frame_R4_0062c310(void*, void*);
void* __fastcall kotor_fastcall_005f10f0(void* internal, void*, std::uint32_t id);   // client-leaves-a
// 005D6200 CExoStreamingSoundSource::~ (lane-sound body; table29 spelling)
}
namespace table29_required { void __fastcall gui_sound_destroy_005d6200(void*, void*); }

extern "C" {
void __fastcall kotor_fastcall_005c5500(void* entry, void*);
void __fastcall kotor_fastcall_005fcb50(void* list, void*);
void __fastcall kotor_fastcall_00602c10(void* journal, void*);
void __fastcall kotor_fastcall_0062a970(void* in_game, void*);
void __fastcall kotor_fastcall_005d5b80(void* source, void*);
void __fastcall kotor_fastcall_006303b0(void* in_game, void*);
void __fastcall kotor_fastcall_00604a40(void* list, void*);
void __fastcall kotor_fastcall_00604b40(void* master, void*);
}

namespace {
// Vector-deleting form used by every array member below (e.g. 00602C3C..00602C57).
void delete_array(void* first, std::uint32_t size, void (__fastcall* destroy)(void*, void*)) {
    unsigned char* block = bytes(first) - 4;
    c2da_destroy_array_006fa852(first, size, static_cast<std::int32_t>(w(block, 0)), destroy);
    live_free(block);
}
kotor_list::Internal* list_at(void* p, U32 off) { return static_cast<kotor_list::Internal*>(pw(p, off)); }
}

// 005C5500 SJournalEntry::~SJournalEntry: CExoString +18h (005E5C20), CExoLocString +08h and +00h (005EA000).
extern "C" void __fastcall kotor_fastcall_005c5500(void* entry, void*) {
    kotor_movie::string_element_destroy_005e5c20(bytes(entry) + 0x18, nullptr); // 005C551D..005C5528
    exo_locstring_destroy_005ea000(bytes(entry) + 8, nullptr);          // 005C552D..005C5535
    exo_locstring_destroy_005ea000(entry, nullptr);                     // 005C553A..005C5544
}
// 005FCB50 CSWCSortedJournalArrayList::~: free([+0Ch]); [+0Ch]=0; delete[] 3Ch-byte SJournalEntry array at [+0]; [+0]=0.
extern "C" void __fastcall kotor_fastcall_005fcb50(void* list, void*) {
    live_free(pw(list, 0x0c));                                          // 005FCB6D..005FCB79
    sw(list, 0x0c, 0);                                                  // 005FCB7E
    if (void* first = pw(list, 0)) delete_array(first, 0x3c, &kotor_fastcall_005c5500); // 005FCB85..005FCBB1
    sw(list, 0, 0);                                                     // 005FCBB9
}
// 00602C10 CSWCJournal::~CSWCJournal: delete[] 0Ch-byte array at [+44h] (element dtor 0059EA30), [+44h]=0,
// then the sorted lists at +20h and +00h.
extern "C" void __fastcall kotor_fastcall_00602c10(void* journal, void*) {
    if (void* first = pw(journal, 0x44)) delete_array(first, 0x0c, &menu_tables_m41::LiveContentDestroy_0059ea30); // 00602C2D..00602C57
    sw(journal, 0x44, 0);                                               // 00602C5E
    kotor_fastcall_005fcb50(bytes(journal) + 0x20, nullptr);            // 00602C5B / 00602C6A
    kotor_fastcall_005fcb50(journal, nullptr);                          // 00602C6F..00602C79
}

// 0062A970 CGuiInGame::ShutDownGlobal: scalar-deleting destructor (vtable slot 0, argument 1) of the four global panels
// at +98h/+9Ch/+A0h/+A4h, each cell cleared after; [+38h] = 0.
extern "C" void __fastcall kotor_fastcall_0062a970(void* in_game, void*) {
    for (U32 off = 0x98; off <= 0xa4; off += 4) {                       // 0062A973..0062A9C8
        if (void* panel = pw(in_game, off)) { vcall1(panel, 0, 1); sw(in_game, off, 0); }
    }
    sw(in_game, 0x38, 0);                                               // 0062A9CE
}
// 005D5B80 CExoStreamingSoundSource::Stop: internal [this] -> CExoStreamingSoundSourceInternal::Stop 005DC380.
extern "C" void __fastcall kotor_fastcall_005d5b80(void* source, void*) {
    if (void* internal = pw(source, 0)) sound_stream_play29_required::close_existing_005dc380(internal, nullptr); // 005D5B80..005D5B86
}

// 006303B0 CGuiInGame::~CGuiInGame.
extern "C" void __fastcall kotor_fastcall_006303b0(void* in_game, void*) {
    frame_R4_0062c310(in_game, nullptr);                                // 006303D6 (CGuiInGame::ShutDownGameGui; core47 body)
    kotor_fastcall_0062a970(in_game, nullptr);                          // 006303DB..006303DD
    if (void* first = pw(in_game, 0xfc))                                // 006303E2..006303EA
        delete_array(first, 0x10, &menu_tables_m41::DialogMessageDestroy_00628600); // 006303EC..00630406
    if (void* first = pw(in_game, 0xf8))                                // 00630409..00630411
        delete_array(first, 0x10, &menu_tables_m41::LiveContentDestroy_0059ea30); // 00630413..0063042D
    while (w(pw(in_game, 0xb44), 0) != 0)                               // 00630430..00630439 / 0063045C
        live_free(base_ini_reconstruction::remove_head_005e9c80(list_at(in_game, 0xb44), nullptr)); // 00630440..00630459
    if (void* sound = pw(in_game, 0xbbc)) {                             // 00630460..00630468
        table29_required::gui_sound_destroy_005d6200(sound, nullptr);   // 0063046A..0063046C
        live_free(sound);                                               // 00630471..00630477
        sw(in_game, 0xbbc, 0);                                          // 0063047A
    }
    if (pw(in_game, 0xb40)) {                                           // 00630484..0063048C
        kotor_fastcall_005d5b80(pw(in_game, 0xb40), nullptr);           // 0063048E
        if (void* sound = pw(in_game, 0xb40)) {                         // 00630493..0063049B
            table29_required::gui_sound_destroy_005d6200(sound, nullptr); // 0063049D..0063049F
            live_free(sound);                                           // 006304A4
        }
        sw(in_game, 0xb40, 0);                                          // 006304AD
    }
    c2da_destroy_array_006fa852(bytes(in_game) + 0xb58, 8, 8, &kotor_movie::string_element_destroy_005e5c20); // 006304B7..006304CC
    static const U32 k_lists[3] = {0xb4cu, 0xb48u, 0xb44u};
    for (U32 off : k_lists) {                                           // 006304D1..0063052B
        if (void* list = pw(in_game, off)) {
            base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(list), nullptr);
            live_free(list);
        }
    }
    kotor_movie::string_element_destroy_005e5c20(bytes(in_game) + 0xb18, nullptr); // 0063052E..0063053C
}

// 00604A40 CClientAIList::~CClientAIList (this = CClientAIMaster+4): {+0 count, +4 CExoLinkedList internal*,
// +8 position node*}. Detaches every listed object's AI node (+1Ch) and destroys the list.
extern "C" void __fastcall kotor_fastcall_00604a40(void* list, void*) {
    if (w(pw(list, 4), 0) != 0) {                                       // 00604A5D..00604A6B
        do {
            void* head = pw(pw(list, 4), 0);                            // 00604A70..00604A73
            const U32 id = head ? w(head, 8) : 0;                       // 00604A75..00604A7D
            void* internal = pw(pw(menu_continuation::app_manager_007a39fc, 4), 4); // 00604A80..00604A89
            void* object = kotor_fastcall_005f10f0(internal, nullptr, id); // 00604A8C..00604A8F
            void* ai = at(vcall0(object, 0x0c));                        // 00604A94..00604A98 (no null check in the original)
            void* node = pw(ai, 0x1c);                                  // 00604A9D
            if (node) {                                                 // 00604AA0..00604AA2
                if (pw(list, 8) == node) {                              // 00604AA4..00604AAC
                    void** position = reinterpret_cast<void**>(bytes(list) + 8);
                    (void)unresolved_list_next_005e9b90(pw(list, 4), nullptr, position); // 00604AAE..00604AB2
                    if (w(list, 8) == 0) {                              // 00604AB7..00604ABA
                        void* first = pw(pw(list, 4), 0);               // 00604ABC..00604ABF
                        sw(list, 8, static_cast<U32>(reinterpret_cast<std::uintptr_t>(first))); // 00604AC3
                        if (first == node) sw(list, 8, 0);              // 00604AC1 / 00604AC5..00604AC7
                    }
                }
                sw(list, 0, w(list, 0) - 1);                            // 00604ACD
                c2da_005e9c10(pw(list, 4), nullptr, node);              // 00604ACF..00604AD3
                sw(ai, 0x1c, 0);                                        // 00604AD8
            }
        } while (w(pw(list, 4), 0) != 0);                               // 00604ADF..00604AE5
    }
    if (void* internal = pw(list, 4)) {                                 // 00604AEA..00604AF7
        base_ini_reconstruction::destroy_list_005e9ab0(static_cast<kotor_list::Internal*>(internal), nullptr); // 00604AF9..00604AFB
        live_free(internal);                                            // 00604B00..00604B06
    }
}
// 00604B40 CClientAIMaster::~CClientAIMaster: add ecx,4; jmp 00604A40.
extern "C" void __fastcall kotor_fastcall_00604b40(void* master, void*) {
    kotor_fastcall_00604a40(bytes(master) + 4, nullptr);                // 00604B40..00604B43
}
