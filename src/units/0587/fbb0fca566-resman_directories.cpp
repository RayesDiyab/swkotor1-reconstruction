// lane-resman rev2: CExoResMan directory/file maintenance family consumed by the
// MainMenu ctor (lane-gui-load rev2 mainmenu.h:46-63; M43 gui-load/mainmenu.cpp
// 0067C907..0067CA0B via CleanDirectory 00409460). Authored from static disassembly
// of the pinned GOG swkotor.exe; never executed. Export names exactly as proposed by
// lane-gui-load. Thiscall originals use the D1 __fastcall(receiver, unused EDX, ...)
// transport; the Manager receiver is used only as the recursion receiver (it is
// otherwise ignored by every body, as in the original).
#include "c30aaa70eb-resources.h"        // menu_resources::{Manager,String}
#include "c534281817-startup_string.h"
#include <cstdint>
#include <cstring>
#include <sys/stat.h>
#include <io.h>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
using Manager = menu_resources::Manager;

// ---- M43-defined lowers (exact existing symbols)
extern "C" {
void* __cdecl key_debug_root_007a39e0();                                       // [007A39E0] CExoBase*
Manager* __cdecl c2da_manager_007a39e8();                                      // [007A39E8] CExoResMan*
extern const char c2da_directory_literal_0073d8f4[];                           // 0073D8F4 "\\"
void __stdcall c2da_destroy_array_006fa852(void*, Word, Word, void (__fastcall*)(String*, void*)); // 006FA852 (normal path)
void __cdecl live_free(void*);                                                 // 006FA390
}
namespace f200_ports {                                                          // M43 lowers-directory-list / lowers-resref-from-file
struct Enumeration { String* entries; Word count, capacity; };                  // f200.h:9 (same layout; +4C/+50/+54 locals)
void __fastcall Enumerate005e6640(void*, void*, void*, String*, Word, Word, Word);  // CExoBase::GetDirectoryList
Word __fastcall Type00406650(Manager*, void*, String*);                        // CExoResMan::GetResTypeFromFile
}
// M43 engine-file-epoch path.h:15 (m40-publication-proposal.obj): CExoAliasList::ResolveFileName 005E68A0 transport.
namespace path_resolution { String* __fastcall ResolveActual(void*, void*, String*, String*, Word); }
namespace fb80_ports { std::int32_t __fastcall Find005e5f20(String*, void*, Word, std::int32_t); } // CExoString::Find(char, start)
namespace startup_rand { String* __fastcall SubString005e6270(String*, void*, String*, int, int); }   // CExoString::SubString(out, start, count)
String* __fastcall GuiConcat005e5d10(String*, void*, String*, const String*); // CExoString::operator+ (hidden out)
extern "C" char* __fastcall WindowCStr005e5670(String*, void*);               // CExoString::CStr

// ---- lane exports (lane-gui-load rev2 mainmenu.h names)
extern "C" {
Word __fastcall gui_load_resman_free_disk_space_004067b0(Manager*, void*, String*, std::uint64_t*) noexcept(false);
Word __fastcall gui_load_resman_create_directory_004068c0(Manager*, void*, String*) noexcept(false);
Word __fastcall gui_load_resman_remove_file_00406b20(Manager*, void*, String*, Word) noexcept(false);
Word __fastcall gui_load_resman_wipe_directory_00408e90(Manager*, void*, String*, Word, Word, Word, Word) noexcept(false);
Word __fastcall gui_load_resman_nuke_directory_00409480(Manager*, void*, String*, Word, Word) noexcept(false);
}

namespace {
// Hidden-return CExoString temporary, destroyed in reverse construction order (EH states).
struct Temporary {
    alignas(String) unsigned char storage[sizeof(String)];
    String* live = nullptr;
    String* out() { return reinterpret_cast<String*>(storage); }
    void adopt(String* s) { live = s; }
    ~Temporary() { if (live) live->~StartupString(); }
};
// CExoAliasList::ResolveFileName on [CExoBase+0Ch] into `path` (operator= then temp dtor).
void resolve(String* path, String* name, Word type) {
    void* aliases;
    std::memcpy(&aliases, static_cast<unsigned char*>(key_debug_root_007a39e0()) + 0x0c, sizeof(aliases));
    Temporary resolved;
    resolved.adopt(path_resolution::ResolveActual(aliases, nullptr, resolved.out(), name, type));
    path->Assign005e5c50(resolved.live);
}
void __fastcall DestroyElement(String* value, void*) { value->~StartupString(); }   // 005E5C20 per element
// CExoArrayList<CExoString> owner: eh_vector_destructor_iterator(data, 8, data[-4], 005E5C20); free(data-4).
struct EnumerationOwner {
    f200_ports::Enumeration value{};                                            // +4C/+50/+54 = 0 (00408ED8..EE0)
    ~EnumerationOwner() {
        String* elements = value.entries;
        if (elements) {
            unsigned char* allocation = reinterpret_cast<unsigned char*>(elements) - 4;
            Word cookie; std::memcpy(&cookie, allocation, 4);
            c2da_destroy_array_006fa852(elements, 8, cookie, &DestroyElement);
            live_free(allocation);
        }
    }
};
}

// 004067B0 CExoResMan::GetFreeDiskSpace(CExoString const& alias_path, unsigned __int64* free) RET8.
extern "C" __declspec(noinline) Word __fastcall gui_load_resman_free_disk_space_004067b0(
    Manager*, void*, String* name, std::uint64_t* free_bytes) noexcept(false) {
    String path;                                                                 // 004067CC
    resolve(&path, name, 0xffffu);                                               // 004067D1-0040680E
    if (path.BorrowedLength005e5790() == 0) return 0;                            // 00406813-00406841
    // 00406845-0040685E: GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetDiskFreeSpaceExA")
    // is only null-tested; the call itself goes through the static import (0073D1D8).
    if (!GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetDiskFreeSpaceExA")) return 0; // 00406897
    // D2 (00406860-00406879): the three outputs are uninitialized stack locals in the
    // original; when the API fails they are copied unwritten. Value-initialized here.
    ULARGE_INTEGER available{}, total{}, total_free{};
    (void)GetDiskFreeSpaceExA(WindowCStr005e5670(&path, nullptr), &available, &total, &total_free); // result ignored
    *free_bytes = available.QuadPart;                                            // 0040687F-00406892 (low,high)
    return 1;                                                                    // 0040688B
}

// 004068C0 CExoResMan::CreateDirectory(CExoString const&) RET4: CreateDirectoryA(path, NULL).
extern "C" __declspec(noinline) Word __fastcall gui_load_resman_create_directory_004068c0(
    Manager*, void*, String* name) noexcept(false) {
    String path;                                                                 // 004068DC
    resolve(&path, name, 0xffffu);                                               // 004068E1-0040691E
    if (path.BorrowedLength005e5790() == 0) return 0;                            // 00406923-00406951
    return static_cast<Word>(CreateDirectoryA(WindowCStr005e5670(&path, nullptr), nullptr)); // 00406955-0040695D
}

// 00406B20 CExoResMan::RemoveFile(CExoString const&, unsigned short type) RET8.
extern "C" __declspec(noinline) Word __fastcall gui_load_resman_remove_file_00406b20(
    Manager*, void*, String* name, Word type) noexcept(false) {
    String path;                                                                 // 00406B3C
    resolve(&path, name, type);                                                  // 00406B41-00406B7D (full arg dword)
    if (path.BorrowedLength005e5790() == 0) return 0;                            // 00406B82-00406B8D
    struct _stat32 info;                                                         // [esp+10h], st_mode at +6 (00406BCF)
    if (_stat32(WindowCStr005e5670(&path, nullptr), &info) == -1) return 0;      // 00406B8F-00406BA9
    if (info.st_mode & _S_IFDIR) return 0;                                       // 00406BCF-00406BDA (test ah,40h)
    (void)_chmod(WindowCStr005e5670(&path, nullptr), _S_IWRITE);                 // 00406BFC-00406C08 (80h)
    return _unlink(WindowCStr005e5670(&path, nullptr)) != -1 ? 1u : 0u;          // 00406C10-00406C27
}

// 00408E90 CExoResMan::WipeDirectory(CExoString const& dir, int files, int remove_self,
//          int clean_subdirs, int recurse) RET14h.
extern "C" __declspec(noinline) Word __fastcall gui_load_resman_wipe_directory_00408e90(
    Manager* self, void*, String* name, Word files, Word remove_self, Word clean_subdirs, Word recurse) noexcept(false) {
    String path;                                                                 // 00408EB3 [esp+1Ch]
    Word result = 1;                                                             // 00408ECA [esp+14h]
    if (files) {                                                                 // 00408EC4-00408ED2
        EnumerationOwner list;
        String entry;                                                            // 00408EED [esp+24h]
        f200_ports::Enumerate005e6640(key_debug_root_007a39e0(), nullptr, &list.value, name, 0xffffu, 0, 0); // 00408EF2-00408F0A
        for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(list.value.count); ++i) { // 00408F0F-00408F19 / 00409012-0040901D
            entry.Assign005e5c50(list.value.entries + i);                        // 00408F20-00408F2C
            const Word type = f200_ports::Type00406650(c2da_manager_007a39e8(), nullptr, &entry); // 00408F31-00408F45
            if (static_cast<std::uint16_t>(type) == 0xffffu) continue;           // 00408F41-00408F49
            const std::int32_t dot = fb80_ports::Find005e5f20(&entry, nullptr, '.', 0); // 00408F4F-00408F5C
            if (dot < 0) return 0;                                               // 00408F60 -> 00409197 (list freed, 004093EB)
            bool failed;
            {
                String slash(c2da_directory_literal_0073d8f4);                   // 00408F66-00408F6F
                Temporary stem, joined, full;
                stem.adopt(startup_rand::SubString005e6270(&entry, nullptr, stem.out(), 0, dot)); // 00408F74-00408F85
                joined.adopt(GuiConcat005e5d10(name, nullptr, joined.out(), &slash));            // 00408F8C-00408F9D
                full.adopt(GuiConcat005e5d10(joined.live, nullptr, full.out(), stem.live));      // 00408FA2-00408FAF
                failed = gui_load_resman_remove_file_00406b20(self, nullptr, full.live, type) == 0; // 00408FB4-00408FD0
            }                                                                    // 00408FD7-00409001 (full, joined, stem, slash)
            if (failed) return 0;                                                // 00409006 -> 004091C7
        }
    }                                                                            // 00409023-0040905B entry, list
    if (recurse) {                                                               // 0040905D-00409060
        EnumerationOwner list;
        String entry;                                                            // 0040907B
        f200_ports::Enumerate005e6640(key_debug_root_007a39e0(), nullptr, &list.value, name, 0xffffu, 1, 0); // 00409080-00409099
        for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(list.value.count); ++i) {
            entry.Assign005e5c50(list.value.entries + i);                        // 004090B0-004090C0
            bool failed;
            {
                String slash(c2da_directory_literal_0073d8f4);                   // 004090C5
                Temporary joined, full;
                joined.adopt(GuiConcat005e5d10(name, nullptr, joined.out(), &slash));           // 004090D3-004090E4
                full.adopt(GuiConcat005e5d10(joined.live, nullptr, full.out(), &entry));        // 004090E9-004090FA
                failed = gui_load_resman_wipe_directory_00408e90(self, nullptr, full.live, files, 1, 1, 1) == 0; // 004090FF-00409120
            }
            if (failed) return 0;                                                // 0040914A -> 004091D1
        }
    } else if (clean_subdirs) {                                                  // 00409201-00409204
        EnumerationOwner list;
        String entry;                                                            // 0040921F
        f200_ports::Enumerate005e6640(key_debug_root_007a39e0(), nullptr, &list.value, name, 0xffffu, 1, 0); // 00409224-0040923D
        for (Word i = 0; static_cast<std::int32_t>(i) < static_cast<std::int32_t>(list.value.count); ++i) {
            entry.Assign005e5c50(list.value.entries + i);                        // 00409250-00409260
            bool failed;
            {
                String slash(c2da_directory_literal_0073d8f4);                   // 00409265
                Temporary joined, full;
                joined.adopt(GuiConcat005e5d10(name, nullptr, joined.out(), &slash));           // 00409273-00409284
                full.adopt(GuiConcat005e5d10(joined.live, nullptr, full.out(), &entry));        // 00409289-0040929A
                failed = gui_load_resman_wipe_directory_00408e90(self, nullptr, full.live, files, 0, 1, 0) == 0; // 0040929F-004092BE
            }
            if (failed) return 0;                                                // 004092E8 -> 004093B3
        }
    }                                                                            // 00409305-0040933A
    if (remove_self) {                                                           // 0040933D-00409340
        resolve(&path, name, 0xffffu);                                           // 00409346-00409376
        if (path.BorrowedLength005e5790() == 0) return 0;                        // 0040937B-004093B0
        result = static_cast<Word>(RemoveDirectoryA(WindowCStr005e5670(&path, nullptr))); // 0040941A-00409426
    }
    return result;                                                               // 0040942A-00409450
}

// 00409480 CExoResMan::NukeDirectory(CExoString const&, int files, int recurse_flag) RET0Ch:
// WipeDirectory(dir, files, 1, 0, recurse_flag).
extern "C" __declspec(noinline) Word __fastcall gui_load_resman_nuke_directory_00409480(
    Manager* self, void*, String* name, Word files, Word recurse) noexcept(false) {
    return gui_load_resman_wipe_directory_00408e90(self, nullptr, name, files, 1, 0, recurse); // 00409480-00409493
}
