// Authored GetGameVersionInfo (InitGameApp first call) and the graceful-shutdown WinMain
// routes 00404A21..00404A95 (swkotor.exe 9c10e045...1435; lane-routes/evidence/full.asm.txt).
#include "81dc466724-winmain_routes.h"
#include "cecd025c06-window_shell.h"
#include "27f068960c-startup_string.h"
#include "f752071125-warning_runtime.h"   // StartupAssign005e5c50
#include "255876c9ce-string_priority.h"   // GuiAssign005e5140
#include "b85de36c60-base_ini.h"          // PublicIni::destroy_005e67e0
#include "a7f3b86d9a-ini_cleanup.h"       // remove_head_005e9c80
#include "8af36a49a4-allocator_live.h"
#include "fbfe8fbaca-d1_bodies.h"         // lane-menu-tables D1: LiveContentDestroy_0059ea30
#include "4162be38be-contracts.h"  // options_video::Client::SetTexturePack_005f14a0
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>

extern "C" { extern void* kotor_cell_rules_007a3a28; }   // owner: lane-prepare prepare_image_data.cpp (exact declaration)

namespace {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
Word word_at(const void* object, Word offset) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(object) + offset, 4); return v; }
void put_word(void* object, Word offset, Word v) { std::memcpy(static_cast<unsigned char*>(object) + offset, &v, 4); }
void* pointer_at(const void* object, Word offset) { return reinterpret_cast<void*>(word_at(object, offset)); }
using DeletingDestructor = void* (__fastcall*)(void*, void*, Word);
// `mov eax,[ecx]; push 1; call [eax]`: scalar deleting destructor through the object's own table.
void delete_through_vtable(void* object) { reinterpret_cast<DeletingDestructor>(word_at(pointer_at(object, 0), 0))(object, nullptr, 1); }
const char empty_0073d71c[] = "";
const char key_0074dc30[] = "\\StringFileInfo\\040904B0\\PrivateBuild";
const char key_0074dc08[] = "\\StringFileInfo\\040904B0\\ProductVersion";
const char key_0074dbe0[] = "\\StringFileInfo\\040904B0\\InternalName";
const char key_0074dbb8[] = "\\StringFileInfo\\040904B0\\LegalCopyright";
// 005E6B9B..005E6BDD (one of four): VerQueryValueA ok -> String tmp(text); target=tmp; ~tmp;
// else target=""(005E5140).
void version_field(void* block, const char* key, String* target) {
    char* text = nullptr; UINT length = 0;
    if (VerQueryValueA(block, key, reinterpret_cast<void**>(&text), &length)) {
        String value(text);
        kotor_reconstruction::StartupAssign005e5c50(target, nullptr, &value);
    } else {
        GuiAssign005e5140(target, nullptr, empty_0073d71c);
    }
}
}

// 005E6B10..005E6D1F GetGameVersionInfo(): GetModuleFileNameA([007A39C0],buf,100h);
// size=GetFileVersionInfoSizeA(buf,&handle); size==0 -> return; block=malloc(size) (no null
// check, as original); GetFileVersionInfoA(buf,handle,size,block); four fields into
// 007A3CC4/007A3CD4/007A3CCC/007A3CDC; free(block) when non-null.
extern "C" void __cdecl kotor_cdecl_005e6b10() {
    char path[0x100];
    DWORD handle = 0;                                                            // 005E6B3D
    GetModuleFileNameA(static_cast<HMODULE>(kotor_menu_window_cells::instance_007a39c0), path, 0x100);  // 005E6B45
    const DWORD size = GetFileVersionInfoSizeA(path, &handle);                   // 005E6B55
    if (!size) return;                                                           // 005E6B5E
    void* block = live_operator_new(size);                                       // 005E6B66
    GetFileVersionInfoA(path, handle, size, block);                              // 005E6B7C
    auto& g = *kotor_window_shell::bound_context->globals;
    version_field(block, key_0074dc30, reinterpret_cast<String*>(&g.graphics_version_7a3cc4));   // 7A3CC4
    version_field(block, key_0074dc08, winmain_routes::cells.product_version_007a3cd4);         // 7A3CD4
    version_field(block, key_0074dbe0, reinterpret_cast<String*>(&g.title_7a3ccc));              // 7A3CCC
    version_field(block, key_0074dbb8, reinterpret_cast<String*>(&g.caption_prefix_7a3cdc));     // 7A3CDC
    if (block) live_free(block);                                                 // 005E6CFC..005E6D01
}

// 00401280..004012BD CAppManager::DestroyServer: if [+8]: 005ED310([+8]); 004AE900([+8]);
// s=[+8]; if s {004AEC70(s); free(s)}; [+8]=0. Then tail JMP SetWindowTitle 00401440.
extern "C" void __fastcall kotor_fastcall_00401280(void* manager, void*) {
    if (pointer_at(manager, 8)) {
        kotor_fastcall_005ed310(pointer_at(manager, 8), nullptr);
        kotor_fastcall_004ae900(pointer_at(manager, 8), nullptr);
        if (void* server = pointer_at(manager, 8)) { kotor_fastcall_004aec70(server, nullptr); live_free(server); }
        put_word(manager, 8, 0);
    }
    kotor_cdecl_00401440();
}
// 004012C0..00401376 CAppManager::~CAppManager.
extern "C" void __fastcall kotor_fastcall_004012c0(void* manager, void*) {
    if (void* client = pointer_at(manager, 4)) { kotor_fastcall_005ee360(client, nullptr); live_free(client); put_word(manager, 4, 0); }
    if (pointer_at(manager, 8)) {                                                // 004012E1..0040130C (inlined DestroyServer core)
        kotor_fastcall_005ed310(pointer_at(manager, 8), nullptr);
        kotor_fastcall_004ae900(pointer_at(manager, 8), nullptr);
        if (void* server = pointer_at(manager, 8)) { kotor_fastcall_004aec70(server, nullptr); live_free(server); }
        put_word(manager, 8, 0);
    }
    kotor_cdecl_00401440();                                                      // 0040130F
    for (Word offset : {0x0cu, 0x10u}) {                                         // 00401314..00401345
        if (void* table = pointer_at(manager, offset)) { kotor_fastcall_004ae640(table, nullptr); live_free(table); put_word(manager, offset, 0); }
    }
    if (void* stats = pointer_at(manager, 0x14)) { kotor_fastcall_00401000(stats, nullptr); live_free(stats); put_word(manager, 0x14, 0); }
    if (void* buffer = pointer_at(manager, 0)) { live_free(buffer); put_word(manager, 0, 0); }   // 00401362..00401371
}
// 005E66A0..005E6744 CExoBase::~CExoBase: +14 internal, +0 ini, +0C aliases, +04 timers,
// +08 debug, +10 random; each: dtor, free, clear.
extern "C" void __fastcall kotor_fastcall_005e66a0(void* base, void*) {
    if (void* p = pointer_at(base, 0x14)) { kotor_fastcall_005e8bb0(p, nullptr); live_free(p); put_word(base, 0x14, 0); }
    if (void* p = pointer_at(base, 0x00)) { static_cast<base_ini_reconstruction::PublicIni*>(p)->destroy_005e67e0(); live_free(p); put_word(base, 0x00, 0); }
    if (void* p = pointer_at(base, 0x0c)) { kotor_fastcall_005e69b0(p, nullptr); live_free(p); put_word(base, 0x0c, 0); }
    if (void* p = pointer_at(base, 0x04)) { kotor_fastcall_005e9270(p, nullptr); live_free(p); put_word(base, 0x04, 0); }
    if (void* p = pointer_at(base, 0x08)) { kotor_fastcall_005e9430(p, nullptr); live_free(p); put_word(base, 0x08, 0); }
    if (void* p = pointer_at(base, 0x10)) { kotor_fastcall_005ea4a0(p, nullptr); live_free(p); put_word(base, 0x10, 0); }
}
// 005ED810: mov ecx,[ecx+4]; jmp 005F8470.   005ED880: mov ecx,[ecx+4]; jmp 00604130.
extern "C" void __fastcall kotor_fastcall_005ed810(void* outer, void*) { kotor_fastcall_005f8470(pointer_at(outer, 4), nullptr); }
extern "C" void __fastcall kotor_fastcall_005ed880(void* outer, void*) { kotor_fastcall_00604130(pointer_at(outer, 4), nullptr); }
// 005F8470..005F8541 CClientExoAppInternal::Uninitialize.
extern "C" void __fastcall kotor_fastcall_005f8470(void* internal, void*) {
    kotor_fastcall_00690910(internal, nullptr);                                  // 005F8474 (ECX=this preserved)
    put_word(internal, 0x248, 0);                                                // 005F8481
    while (word_at(pointer_at(internal, 0x168), 0) != 0) {                       // 005F8487/005F84B9
        void* element = base_ini_reconstruction::remove_head_005e9c80(static_cast<base_ini_reconstruction::List*>(pointer_at(internal, 0x168)), nullptr);
        if (element) { menu_tables_m41::LiveContentDestroy_0059ea30(element, nullptr); live_free(element); }
    }
    kotor_fastcall_005f6410(internal, nullptr);                                  // 005F84BE
    if (void* p = *winmain_routes::cells.object_008338fc) { delete_through_vtable(p); *winmain_routes::cells.object_008338fc = nullptr; }
    if (void* p = *winmain_routes::cells.object_008338ec) { delete_through_vtable(p); *winmain_routes::cells.object_008338ec = nullptr; }
    if (void* p = kotor_cell_rules_007a3a28) { delete_through_vtable(p); kotor_cell_rules_007a3a28 = nullptr; }
    if (void* p = pointer_at(internal, 0x278)) { delete_through_vtable(p); put_word(internal, 0x278, 0); }
    if (void* p = pointer_at(internal, 0x27c)) { delete_through_vtable(p); put_word(internal, 0x27c, 0); }
    static_cast<options_video::Client*>(internal)->SetTexturePack_005f14a0(0xff, 1);                         // 005F8531..005F853A SetTexturePack(0FFh,1)
}
