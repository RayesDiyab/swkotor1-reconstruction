// Authored graceful-shutdown bodies below the WinMain exit rows (swkotor.exe 9c10e045...1435;
// lane-routes/evidence/full.asm.txt). Each body cites its range. Original thiscall callees
// are transported in the D1 fastcall(this, unused EDX, ...) form.
#include "643747a7ff-winmain_routes.h"
#include "e1a5556035-window_shell.h"
#include "397879e70b-startup_string.h"
#include "2b590d51e1-string_priority.h"   // GuiConcat005e5d10
#include "e94c8df4c3-native_strings.h"    // WindowCopy005e5b00 / WindowCStr005e5670
#include "16ff5a2b95-ini_cleanup.h"       // remove_head_005e9c80
#include "270b6a8a6e-manager_cell.h"      // kotor_resource_demand::resource_manager_007a39e8
#include "9b8e09d300-manager_lifetime.h"  // menu_resources::DestroyOwnedManager00408670
#include "05a3c5e4fb-allocator_live.h"
#include "b593572262-m50_vtables.h"      // lane-m50-vtables W-4: typed CClientExoApp / CServerExoApp / CSWCModule tables
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>
#include <new>

extern "C" {   // owners: lane-prepare prepare_image_data.cpp / lane-sound (exact declarations, game_prepare_producers.h)
extern void* kotor_cell_interface_007a39f0;
extern void* kotor_cell_tlk_007a3a08;
extern void* kotor_cell_sound_007a39ec;
}

namespace {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
Word word_at(const void* object, Word offset) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(object) + offset, 4); return v; }
void put_word(void* object, Word offset, Word v) { std::memcpy(static_cast<unsigned char*>(object) + offset, &v, 4); }
void* pointer_at(const void* object, Word offset) { return reinterpret_cast<void*>(word_at(object, offset)); }
String* string_at(void* object, Word offset) { return reinterpret_cast<String*>(static_cast<unsigned char*>(object) + offset); }
// GATED (D41): non-Aurora vtable slot 0 read (input/menu/module/panels) pending published typed tables; via a published typed vtable struct once available.
using DeletingDestructor = void* (__fastcall*)(void*, void*, Word);
void delete_through_vtable(void* object) { reinterpret_cast<DeletingDestructor>(word_at(pointer_at(object, 0), 0))(object, nullptr, 1); }
using Destroy = void (__fastcall*)(void*, void*);
// `if (p=[o+off]) { dtor(p); free(p); [o+off]=0; }` - the repeated original pattern.
void destroy_member(void* object, Word offset, Destroy dtor) {
    if (void* p = pointer_at(object, offset)) { dtor(p, nullptr); live_free(p); put_word(object, offset, 0); }
}
void free_member(void* object, Word offset) {
    if (void* p = pointer_at(object, offset)) { live_free(p); put_word(object, offset, 0); }
}
const char space_0073d5d8[] = " ";
const char tempclient_0074ebb0[] = "TEMPCLIENT:";
const char portraits_0074eb58[] = "PORTRAITS:";
const char servervault_00745514[] = "SERVERVAULT:";
const char chitin_0074eb64[] = "HD0:CHITIN";
const char override_0074553c[] = "OVERRIDE:";
const char movies_0074eb2c[] = "HD0:MOVIES";
const char music_0074eb0c[] = "HD0:STREAMMUSIC";
const char waves_0074eb1c[] = "HD0:STREAMWAVES";
const char global_0074eb04[] = "GLOBAL";
const char rims_global_0074eaf8[] = "RIMS:GLOBAL";
const char rims_0073d924[] = "RIMS:";
}

// 004015A0: `ret` (void_return). 005ED310: mov ecx,[ecx+4]; jmp 004015A0.
extern "C" void __fastcall kotor_fastcall_004015a0(void*, void*) {}
extern "C" void __fastcall kotor_fastcall_005ed310(void* object, void*) { kotor_fastcall_004015a0(pointer_at(object, 4), nullptr); }

// 00401440..0040150A SetWindowTitle: tmp(" "); r1=[7A3CDC]+tmp; r2=r1+[7A3CCC]; ~r1; ~tmp;
// c=copy([7A3CCC]); SetWindowTextA([007A39D4], c.CStr()); ~c; ~r2. (r2 is discarded: the
// window text is the title copy, exactly as 004014B4..004014D7.)
extern "C" void __cdecl kotor_cdecl_00401440() {
    auto& g = *kotor_window_shell::bound_context->globals;
    String* const prefix = reinterpret_cast<String*>(&g.caption_prefix_7a3cdc);
    String* const title = reinterpret_cast<String*>(&g.title_7a3ccc);
    alignas(String) unsigned char r2_storage[sizeof(String)];
    String* r2 = reinterpret_cast<String*>(r2_storage);
    {
        String space(space_0073d5d8);                                              // 00401461
        alignas(String) unsigned char r1_storage[sizeof(String)];
        String* r1 = reinterpret_cast<String*>(r1_storage);
        String* first = GuiConcat005e5d10(prefix, nullptr, r1, &space);             // 0040147D (constructs r1)
        GuiConcat005e5d10(first, nullptr, r2, title);                               // 00401493 (constructs r2)
        r1->~String();                                                              // 004014A1
    }                                                                               // 004014AF ~space
    alignas(String) unsigned char copy_storage[sizeof(String)];
    String* copy = WindowCopy005e5b00(reinterpret_cast<String*>(copy_storage), nullptr, title);   // 004014BD
    SetWindowTextA(kotor_menu_window_cells::top_007a39d4, WindowCStr005e5670(copy, nullptr));    // 004014CB..004014D7
    copy->~String();                                                                // 004014E6
    r2->~String();                                                                  // 004014F7
}

// 004AE640: jmp 004AE5F0 CObjectTableManager::~: for i<[+180] (reloaded): e=[this+4i];
// if e { if [e] free([e]); free(e); [this+4i]=0 }.
extern "C" void __fastcall kotor_fastcall_004ae640(void* table, void*) {
    for (Word i = 0; i < word_at(table, 0x180); ++i) {
        if (void* entry = pointer_at(table, i * 4)) {
            if (void* data = pointer_at(entry, 0)) live_free(data);
            live_free(entry);
            put_word(table, i * 4, 0);
        }
    }
}
// 00401000..00401059 CSWReentrantServerStats::~: ~String(+28), ~String(+20), ~String(+18).
extern "C" void __fastcall kotor_fastcall_00401000(void* stats, void*) {
    string_at(stats, 0x28)->~String(); string_at(stats, 0x20)->~String(); string_at(stats, 0x18)->~String();
}
// 005EE360..005EE37F CClientExoApp::~: [this]=0074E6D8 (005EE366; typed table kotor_vtable_cclientexoapp_0074e6d8,
// lane-m50-vtables); i=[+4]; if i { 005FC6F0(i); free(i) } (no clear).
extern "C" void __fastcall kotor_fastcall_005ee360(void* client, void*) {
    put_word(client, 0, kotor_vtable_word(&kotor_vtable_cclientexoapp_0074e6d8));
    if (void* internal = pointer_at(client, 4)) { kotor_fastcall_005fc6f0(internal, nullptr); live_free(internal); }
}
// 004AEC70..004AEC8F CServerExoApp::~: [this]=00744184 (004AEC76; typed table kotor_vtable_cserverexoapp_00744184,
// lane-m50-vtables); i=[+4]; if i { 004B7230(i); free(i) }.
extern "C" void __fastcall kotor_fastcall_004aec70(void* server, void*) {
    put_word(server, 0, kotor_vtable_word(&kotor_vtable_cserverexoapp_00744184));
    if (void* internal = pointer_at(server, 4)) { kotor_fastcall_004b7230(internal, nullptr); live_free(internal); }
}
// 004AE900: mov ecx,[ecx+4]; jmp 004B7C60.
extern "C" void __fastcall kotor_fastcall_004ae900(void* server, void*) { kotor_fastcall_004b7c60(pointer_at(server, 4), nullptr); }

// 005E8BB0..005E8BE8 CExoBaseInternal::~: free([+4]) if set (not cleared); a=[+8]: if a,
// eh vector destructor iterator(a, 8, [a-4], 005E5C20) then free(a-4).
extern "C" void __fastcall kotor_fastcall_005e8bb0(void* internal, void*) {
    if (void* p = pointer_at(internal, 4)) live_free(p);
    if (unsigned char* array = static_cast<unsigned char*>(pointer_at(internal, 8))) {
        const Word count = word_at(array - 4, 0);
        for (Word n = count; n != 0; --n) reinterpret_cast<String*>(array + (n - 1) * 8)->~String();   // 006FA852: reverse order
        live_free(array - 4);
    }
}
// 005EC0E0..005EC15F CExoAliasListInternal::~: node=[+8]; while node { next=[node+10];
// ~String(node+8); ~String(node); free(node); node=next } ; ~String(this+0).
extern "C" void __fastcall kotor_fastcall_005ec0e0(void* internal, void*) {
    for (void* node = pointer_at(internal, 8); node;) {
        void* const next = pointer_at(node, 0x10);
        string_at(node, 8)->~String(); string_at(node, 0)->~String(); live_free(node);
        node = next;
    }
    string_at(internal, 0)->~String();
}
// 005E69B0 CExoAliasList::~: i=[this]; if i { 005EC0E0(i); free(i); [this]=0 }.
extern "C" void __fastcall kotor_fastcall_005e69b0(void* list, void*) { destroy_member(list, 0, &kotor_fastcall_005ec0e0); }
// 005E9270 CExoTimers::~: i=[this]; if i { 004015A0(i); free(i) } (no clear).
extern "C" void __fastcall kotor_fastcall_005e9270(void* timers, void*) {
    if (void* internal = pointer_at(timers, 0)) { kotor_fastcall_004015a0(internal, nullptr); live_free(internal); }
}
// 005ECC80 CExoRandInternal::~: if [+64] { free; [+64]=0 }.   005EA4A0 CExoRand::~: [this].
extern "C" void __fastcall kotor_fastcall_005ecc80(void* internal, void*) { free_member(internal, 0x64); }
extern "C" void __fastcall kotor_fastcall_005ea4a0(void* rand, void*) { destroy_member(rand, 0, &kotor_fastcall_005ecc80); }
// 005E9430..005E9493 CExoDebug::~: i=[+8]; if i { 005EC680(i) (jmp CloseLogFiles 005EC5C0);
// free(i) } (no clear); ~String(this+0).
extern "C" void __fastcall kotor_fastcall_005e9430(void* debug, void*) {
    if (void* internal = pointer_at(debug, 8)) { kotor_fastcall_005ec5c0(internal, nullptr); live_free(internal); }
    string_at(debug, 0)->~String();
}
// 005F6410..005F643E EmptyHitcheckIgnoreList: while *[+190]: e=RemoveHead([+190]); if e free(e).
extern "C" void __fastcall kotor_fastcall_005f6410(void* internal, void*) {
    while (word_at(pointer_at(internal, 0x190), 0) != 0) {
        if (void* e = base_ini_reconstruction::remove_head_005e9c80(static_cast<base_ini_reconstruction::List*>(pointer_at(internal, 0x190)), nullptr)) live_free(e);
    }
}
// 00690910..0069093C UnloadCachedVFX: while *[00833BC8]: e=RemoveHead; if e e->slot0(1).
extern "C" void __fastcall kotor_fastcall_00690910(void*, void*) {
    while (word_at(*winmain_routes::cells.vfx_list_00833bc8, 0) != 0) {
        if (void* e = base_ini_reconstruction::remove_head_005e9c80(static_cast<base_ini_reconstruction::List*>(*winmain_routes::cells.vfx_list_00833bc8), nullptr)) delete_through_vtable(e);
    }
}

// 00604130..0060478A CClientExoAppInternal::StopServices.
extern "C" void __fastcall kotor_fastcall_00604130(void* self, void*) {
    kotor_fastcall_005f7190(self, nullptr);                                                    // 00604154 ShutDownMoviePlayer
    if (frame_ContainsPanel_0040bd70(static_cast<gui_ctor::Gui*>(pointer_at(self, 0x274)), nullptr, pointer_at(self, 0x280))) {   // 00604166
        if (void* menu = pointer_at(self, 0x280)) delete_through_vtable(menu);                  // 0060417F
    }
    put_word(self, 0x280, 0);                                                                  // 00604189
    if (void* p = pointer_at(self, 0x284)) { delete_through_vtable(p); put_word(self, 0x284, 0); }
    destroy_member(self, 0x1c4, &kotor_fastcall_00678880);                                     // CSoundSetList
    free_member(self, 0x1c8);
    if (void* p = pointer_at(self, 0x18)) {                                                    // 006041D6..006041DB module (CSWCModule)
        kotor_game_object::module_methods_of(p)->scalar_deleting_destructor(p, nullptr, 1);       // 006041DD..006041E1 typed slot 0 = 00644150 (lane-m50-vtables, D41)
        put_word(self, 0x18, 0);                                                               // 006041E3
    }
    destroy_member(self, 0x14, &kotor_fastcall_004d7e10);                                      // CGameObjectArray
    destroy_member(self, 0x3c, &kotor_fastcall_00676de0);                                      // CSWCWorldJournal
    destroy_member(self, 0xe0, &kotor_fastcall_005f1020);                                      // CExoImeUI
    if (pointer_at(self, 0x40)) {                                                              // 0060423A
        frame_R4_0062c310(pointer_at(self, 0x40), nullptr);                                    // CGuiInGame::ShutDown
        if (void* p = pointer_at(self, 0x40)) { kotor_fastcall_006303b0(p, nullptr); live_free(p); }
        put_word(self, 0x40, 0);
    }
    if (void* gui = pointer_at(self, 0x274)) {                                                 // 00604260
        kotor_fastcall_0040aaa0(gui, nullptr); live_free(gui);
        put_word(self, 0x274, 0); *winmain_routes::cells.gui_007a39f4 = nullptr;              // 00604277/00604280
    }
    destroy_member(self, 0x0c, &kotor_fastcall_00604b40);                                      // CClientAIMaster
    while (word_at(pointer_at(self, 0x1c), 0) != 0) {                                          // 006042A0..006042F8
        if (void* e = base_ini_reconstruction::remove_head_005e9c80(static_cast<base_ini_reconstruction::List*>(pointer_at(self, 0x1c)), nullptr)) {
            string_at(e, 0x30)->~String();
            kotor_fastcall_004015a0(static_cast<unsigned char*>(e) + 0x14, nullptr);
            string_at(e, 0x0c)->~String();
            live_free(e);
        }
    }
    destroy_member(self, 0x08, &kotor_fastcall_004b1360);                                      // CServerInfo
    if (pointer_at(self, 0x10)) {                                                              // 00604314
        kotor_fastcall_005d5600(pointer_at(self, 0x10), nullptr);                              // CNetLayer::ShutDown
        if (void* p = pointer_at(self, 0x10)) { kotor_fastcall_005d5720(p, nullptr); live_free(p); }
        put_word(self, 0x10, 0);
    }
    if (void* input = kotor_menu_window_cells::input_007a39e4) { delete_through_vtable(input); kotor_menu_window_cells::input_007a39e4 = nullptr; }
    destroy_member(self, 0x238, &kotor_fastcall_005d6200);
    destroy_member(self, 0x23c, &kotor_fastcall_005d6200);
    if (void* sound = kotor_cell_sound_007a39ec) { kotor_fastcall_005d62f0(sound, nullptr); live_free(sound); kotor_cell_sound_007a39ec = nullptr; }
    if (void* p = pointer_at(self, 0x14c)) { delete_through_vtable(p); put_word(self, 0x14c, 0); }
    if (void* tlk = kotor_cell_tlk_007a3a08) { kotor_fastcall_0041e100(tlk, nullptr); live_free(tlk); kotor_cell_tlk_007a3a08 = nullptr; }
    destroy_member(self, 0x1c4, &kotor_fastcall_00678880);                                     // 006043E6 (second check, as original)
    if (void* iface = kotor_cell_interface_007a39f0) { kotor_fastcall_0070d340(iface, nullptr); live_free(iface); kotor_cell_interface_007a39f0 = nullptr; }
    if (kotor_menu_window_cells::aurora_007a39f8) { kotor_cdecl_0044ed80(); kotor_menu_window_cells::aurora_007a39f8 = nullptr; }   // 0060442E
    if (menu_resources::Manager* resources = kotor_resource_demand::resource_manager_007a39e8) {   // 00604439
        (void)resources;
        auto remove_directory = [](const char* name) {
            String s(name); kotor_fastcall_004088d0(kotor_resource_demand::resource_manager_007a39e8, nullptr, &s);
        };
        remove_directory(portraits_0074eb58);                                                  // 00604445
        remove_directory(servervault_00745514);                                                // 00604478
        { String s(chitin_0074eb64); kotor_fastcall_004088c0(kotor_resource_demand::resource_manager_007a39e8, nullptr, &s); }   // 006044AB..006044CC
        remove_directory(override_0074553c);                                                   // 006044DE
        remove_directory(movies_0074eb2c);                                                     // 00604511
        remove_directory(music_0074eb0c);                                                      // 00604544
        remove_directory(waves_0074eb1c);                                                      // 00604577
        remove_directory(tempclient_0074ebb0);                                                 // 006045AA
        alignas(4) unsigned char ref_storage[16];                                              // [esp+10h] raw stack, constructed by 00406D80
        auto* ref = reinterpret_cast<menu_resources::Ref*>(ref_storage);
        ref->construct_00406d80(global_0074eb04);                                              // 006045E6
        if (kotor_resource_demand::resource_manager_007a39e8->exists_00408bc0(ref, 0xbba, nullptr)) {   // 006045FC
            String s(rims_global_0074eaf8); kotor_fastcall_00408830(kotor_resource_demand::resource_manager_007a39e8, nullptr, &s);   // 00604626
        }
        remove_directory(rims_0073d924);
        if (menu_resources::Manager* m = kotor_resource_demand::resource_manager_007a39e8) {   // 0060466B
            menu_resources::DestroyOwnedManager00408670(m, nullptr); live_free(m);
            kotor_resource_demand::resource_manager_007a39e8 = nullptr;
        }
    }
    for (Word offset : {0x24u, 0x28u, 0x2cu, 0x30u, 0x04u})                                    // 0060468B..0060470A (world timers etc.)
        destroy_member(self, offset, &kotor_fastcall_004015a0);
    free_member(self, 0x1c0);
    if (void* p = pointer_at(self, 0x2a0)) live_free(p);                                       // 00604726 (not cleared)
    destroy_member(self, 0x38, &kotor_fastcall_00602c10);                                      // CSWCJournal
    free_member(self, 0x1b4);
    put_word(self, 0x1b8, 0); put_word(self, 0x1bc, 0);                                        // 00604770/00604776
}
