// claude_w1_chains_erf.cpp (lane worker w1-chains): the CERFFile writer set and CExoFile/CExoResMan leaves used by
// CServerExoAppInternal::DoPCAutosave 004B8300 (claude_w1_chains_autosave.cpp) and by the module save chain of
// StoreCurrentModule 004B2E70, transcribed from the original swkotor.exe (read-only):
//   005DCFC0 CERFFile::Create(CExoString const& path)                     RET 4   kotor_fastcall_005dcfc0
//   005DCE30 CERFFile::SetVersion(char const* version)                    RET 4   kotor_fastcall_005dce30
//   005DD080 CERFFile::WriteHeader()                                      RET 0   kotor_fastcall_005dd080
//   005DD5A0 CERFFile::WriteStringTable()                                 RET 0   kotor_fastcall_005dd5a0
//   005DDA50 CERFFile::SetNumEntries(ulong count)                         RET 4   kotor_fastcall_005dda50
//   005DDBC0 CERFFile::WriteResource(CExoString const& path, int)         RET 8   kotor_fastcall_005ddbc0
//   005DD120 CERFKey::Write(CExoFile& file)                               RET 4   kotor_fastcall_005dd120
//   005DE1A0 CERFFile::ImportFiles(CExoString const& dir, int, callback, a4, a5) RET 14h kotor_fastcall_005de1a0
//   005DD0E0 CERFFile::Finish()                                           RET 0   kotor_fastcall_005dd0e0
//   005E6940 CExoFile::GetOffset()  (jmp 005EB970 CExoFileInternal::GetOffset) RET 0 kotor_fastcall_005e6940/_005eb970
//   005E6AF0 CExoFile::SeekEnd()                                          RET 0   kotor_fastcall_005e6af0
//   00406990 CExoResMan::FileCopy(src, dst, src_type, dst_type)          RET 10h kotor_fastcall_00406990
// Conventions (W5): thiscall -> extern "C" __fastcall(self, unused_edx, ...); live_operator_new / live_free only.
// CExoFileInternal holds the host CRT FILE* at +0 (file_internal::FileOps binds std::ftell/std::fseek), so the
// original's _ftell/_fseek calls are the host std::ftell/std::fseek on that stream; time()/localtime() are the
// host 32-bit _time32/_localtime32.
#include "842d6b1618-w5_t3.hpp"
#include <cstdio>
#include <ctime>

namespace kotor_reconstruction { struct StartupString; }
namespace tlk_file { struct File; }
namespace options_video { struct String; }
namespace file_internal {   // 005E68D0 CExoFile::CExoFile(CExoString const& name, ushort type, CExoString const& mode) [0230]
tlk_file::File* __fastcall exofile_construct_005e68d0(tlk_file::File*, void*, kotor_reconstruction::StartupString*, unsigned,
                                                     kotor_reconstruction::StartupString*);
}
namespace base_ini_reconstruction {   // 005E5F70 Left / 005E6170 Right (this, CExoString* result, int count) [0126]
kotor_reconstruction::StartupString* __fastcall left_005e5f70(kotor_reconstruction::StartupString*, void*,
                                                               kotor_reconstruction::StartupString*, int);
kotor_reconstruction::StartupString* __fastcall right_005e6170(kotor_reconstruction::StartupString*, void*,
                                                                kotor_reconstruction::StartupString*, int);
}
namespace client_movies { int __fastcall StringFind_005e5eb0(options_video::String*, void*, options_video::String*, int); } // CExoString::Find [0374]
namespace f200_ports {   // 005E6640 CExoBase::GetDirectoryList(CExoArrayList<CExoString>*, CExoString const&, ushort, int, int) [0288]
void __fastcall Enumerate005e6640(void*, void*, void*, kotor_reconstruction::StartupString*, unsigned, unsigned, unsigned);
}
namespace kotor_menu_10_19 { namespace dependencies { extern void* base_007a39e0; } }       // [007A39E0] CExoBase* [0068]
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); } // CExoString() [menu20_bindings]

namespace w1c_erf {
using namespace w5;

extern "C" {
// ---------------- imports: existing providers (c14 candidate map; owner object in brackets) ----------------
U __fastcall file_opened_005e6a10(void* file, void*);                                         // CExoFile::FileOpened [0224]
U __fastcall exofile_size_005e6950(void* file, void*);                                        // CExoFile::GetSize [0232]
U __fastcall exofile_read_005e6960(void* file, void*, void* out, U size, U count);             // CExoFile::Read [0231]
U __fastcall recovered_file_write_005e69a0(void* file, void*, const void* data, U size, U count); // CExoFile::Write
U __fastcall exofile_seek_005e6aa0(void* file, void*, U offset, U origin);                    // CExoFile::Seek [0232]
void __fastcall cerf_c_exofile_flush_005e6a20(void* file);                                    // CExoFile::Flush [cerf_quicksave_leaves]
void __fastcall file_destroy_005e69e0(void* file, void*);                                     // CExoFile::~CExoFile [0224]
U __fastcall async_string_length_005e5790(void* string, void*);                               // CExoString::GetLength [0188]
const char* __fastcall WindowCStr005e5670(void*, void*);                                      // CExoString::CStr [0078]
void __cdecl frame_LogFormat_005e5680(void* self, const char* format, ...);                  // CExoString::Format [0125]
void* __fastcall async_assign_string_005e5c50(void* self, void*, void* other);                // CExoString::operator= [0199]
U __fastcall c2da_base_005e6670(void* base, void*, void* extension);                          // CExoBase::GetResTypeFromExtension [0286]

// ---------------- exports of this file ----------------
U __fastcall kotor_fastcall_005dcfc0(void* self, void*, void* path);
void __fastcall kotor_fastcall_005dce30(void* self, void*, const char* version);
U __fastcall kotor_fastcall_005dd080(void* self, void*);
U __fastcall kotor_fastcall_005dd5a0(void* self, void*);
U __fastcall kotor_fastcall_005dda50(void* self, void*, U count);
U __fastcall kotor_fastcall_005ddbc0(void* self, void*, void* path, U unused);
U __fastcall kotor_fastcall_005dd120(void* key, void*, void* file);
void __fastcall kotor_fastcall_005de1a0(void* self, void*, void* directory, U type, void* callback, U a4, U a5);
U __fastcall kotor_fastcall_005dd0e0(void* self, void*);
U __fastcall kotor_fastcall_005e6940(void* file, void*);
U __fastcall kotor_fastcall_005eb970(void* internal, void*);
U __fastcall kotor_fastcall_005e6af0(void* file, void*);
U __fastcall kotor_fastcall_00406990(void* resman, void*, void* source, void* target, U source_type, U target_type);
}

namespace {
const char k_wb_0073d8a4[] = "wb";                     // .rdata 0073D8A4
const char k_rb_0073d5b0[] = "rb";                     // .rdata 0073D5B0
const char k_colon_0073e2a0[] = ":";                   // .rdata 0073E2A0
const char k_dot_00741480[] = ".";                     // .rdata 00741480
const char k_join_00740ccc[] = "%s%s";                 // .rdata 00740CCC

using Str = kotor_reconstruction::StartupString;
inline Str* str(void* p) { return static_cast<Str*>(p); }
inline void string_default(void* s) { kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(s), nullptr); }
inline void string_text(void* s, const char* t) { (void)frame_LogStringCtor_005e5a90(s, nullptr, t); }
inline void string_destroy(void* s) { frame_LogStringDtor_005e5c20(s, nullptr); }
inline int string_find(void* s, void* what) {
    return client_movies::StringFind_005e5eb0(static_cast<options_video::String*>(s), nullptr, static_cast<options_video::String*>(what), 0);
}
inline void* file_of(void* erf) { return at<void*>(erf, 0xac); }
inline U write(void* file, const void* data, U size) { return recovered_file_write_005e69a0(file, nullptr, data, size, 1); }
inline U seek(void* file, U offset) { return exofile_seek_005e6aa0(file, nullptr, offset, 0); }
// CExoFile::CExoFile(name, type, mode) with the mode literal in a temporary CExoString (destroyed after the call).
void* file_construct(void* memory, void* name, U type, const char* mode) {
    alignas(4) unsigned char mode_string[8];
    string_text(mode_string, mode);
    struct Guard { void* s; ~Guard() { string_destroy(s); } } guard{mode_string};
    return file_internal::exofile_construct_005e68d0(static_cast<tlk_file::File*>(memory), nullptr, str(name), type, str(mode_string));
}
}  // namespace

extern "C" {
// =========================================================================================================
// 005EB970 CExoFileInternal::GetOffset: stream ? ftell(stream) : 0. RET 0.
U __fastcall kotor_fastcall_005eb970(void* internal, void*) {
    std::FILE* stream = at<std::FILE*>(internal, 0);                                           // 005EB970
    if (stream == nullptr) return 0;                                                           // 005EB972..B974, B980
    return static_cast<U>(std::ftell(stream));                                                 // 005EB976..B97F
}

// 005E6940 CExoFile::GetOffset: ECX = [ECX]; jmp 005EB970. RET 0.
U __fastcall kotor_fastcall_005e6940(void* file, void*) {
    return kotor_fastcall_005eb970(at<void*>(file, 0), nullptr);                               // 005E6940..6942
}

// 005E6AF0 CExoFile::SeekEnd: stream ? (fseek(stream, 0, SEEK_END) == 0) : 0. RET 0.
U __fastcall kotor_fastcall_005e6af0(void* file, void*) {
    std::FILE* stream = at<std::FILE*>(at<void*>(file, 0), 0);                                 // 005E6AF0..6AF2
    if (stream == nullptr) return 0;                                                           // 005E6AF4..6AF6, 6B0B
    return std::fseek(stream, 0, SEEK_END) == 0 ? 1u : 0u;                                     // 005E6AF8..6B0A
}

// =========================================================================================================
// 005DCE30 CERFFile::SetVersion(char const*): the first two dwords (8 characters). RET 4.
void __fastcall kotor_fastcall_005dce30(void* self, void*, const char* version) {
    std::memcpy(self, version, 8);                                                             // 005DCE30..CE3B
}

// 005DCFC0 CERFFile::Create(CExoString const& path): new CExoFile(path, 809h, "wb") at +ACh. RET 4, EAX = opened.
// A failed allocation stores 0 and FileOpened then runs on the null file (original).
U __fastcall kotor_fastcall_005dcfc0(void* self, void*, void* path) {
    void* memory = live_operator_new(4);                                                       // 005DCFDD..CFEA
    void* file = nullptr;
    if (memory != nullptr) {                                                                   // 005DCFF3..CFF9
        try {
            file = file_construct(memory, path, 0x809, k_wb_0073d8a4);                         // 005DCFFB..D048
        } catch (...) {
            live_free(memory);                                                                 // EH state 0
            throw;
        }
    }
    at<void*>(self, 0xac) = file;                                                              // 005DD034
    return file_opened_005e6a10(file_of(self), nullptr) != 0 ? 1u : 0u;                       // 005DD04D..D062
}

// 005DD080 CERFFile::WriteHeader: stamps build year/day from localtime(time(0)) and writes the A0h header. RET 0.
U __fastcall kotor_fastcall_005dd080(void* self, void*) {
    if (file_of(self) == nullptr) return 0;                                                    // 005DD084..D08C, D0D1
    __time32_t now = _time32(nullptr);                                                         // 005DD08E..D095
    const std::tm* local = _localtime32(&now);                                                 // 005DD099..D0A3
    if (local != nullptr) {                                                                    // 005DD0A6..D0A8
        at<I>(self, 0x20) = local->tm_year;                                                    // 005DD0AA..D0AD (+14h)
        at<I>(self, 0x24) = local->tm_yday;                                                    // 005DD0B0..D0B3 (+1Ch)
    }
    (void)write(file_of(self), self, 0xa0);                                                    // 005DD0B6..D0C4
    return 1;                                                                                  // 005DD0C9
}

// 005DD5A0 CERFFile::WriteStringTable: writes each localized string (id, length, text) and back-patches the header
// (+08h count, +0Ch size, +14h/+18h offsets). The size is the LAST entry's length + 8 (original). RET 0.
U __fastcall kotor_fastcall_005dd5a0(void* self, void*) {
    void* file = file_of(self);
    if (file == nullptr) return 0;                                                             // 005DD5A6..D5AE, D707
    void* list = at<void*>(self, 0xa0);                                                        // 005DD5B4
    U count = at<U>(list, 8);                                                                  // 005DD5BA..D5BD [esp+14h]
    U size = 0;                                                                                // 005DD5C1 [esp+8]
    U start = kotor_fastcall_005e6940(file, nullptr);                                          // 005DD5C9..D5CE [esp+0Ch]
    void* pos = at<void*>(at<void*>(self, 0xa0), 0);                                           // 005DD5D2..D5DC [esp+4]
    while (pos != nullptr) {                                                                   // 005DD5E0, D649..D64F
        void* node = unresolved_list_at_pos_005e9b60(at<void*>(self, 0xa0), nullptr, pos);    // 005DD5F0..D5F7
        void* out = file_of(self);                                                             // 005DD5FC
        size = at<U>(node, 4) + 8;                                                             // 005DD604..D611
        (void)write(out, node, 4);                                                             // 005DD60A..D618
        (void)write(out, static_cast<unsigned char*>(node) + 4, 4);                            // 005DD61D..D624
        (void)write(out, at<void*>(node, 8), at<U>(node, 4));                                  // 005DD629..D634
        (void)unresolved_list_next_005e9b90(at<void*>(self, 0xa0), nullptr, &pos);             // 005DD639..D644
    }
    U end = kotor_fastcall_005e6940(file_of(self), nullptr);                                   // 005DD654..D669 [esp+10h]
    (void)seek(file_of(self), 8);                                                              // 005DD65F..D66D
    (void)write(file_of(self), &count, 4);                                                     // 005DD672..D681
    (void)write(file_of(self), &size, 4);                                                      // 005DD686..D695
    const U here = kotor_fastcall_005e6940(file_of(self), nullptr);                            // 005DD69A..D6A0
    (void)seek(file_of(self), here + 4);                                                       // 005DD6A5..D6B1
    at<U>(self, 0xb0) = start;                                                                 // 005DD6B6..D6C2
    (void)write(file_of(self), &start, 4);                                                     // 005DD6B8..D6CF
    at<U>(self, 0xb4) = end;                                                                   // 005DD6D4..D6E7
    (void)write(file_of(self), &end, 4);                                                       // 005DD6D8..D6ED
    (void)kotor_fastcall_005e6af0(file_of(self), nullptr);                                     // 005DD6F2..D6F8
    return 1;                                                                                  // 005DD6FD
}

// 005DDA50 CERFFile::SetNumEntries(ulong count): header +10h = count; reserves count empty keys (18h bytes each,
// type FFFFh) at +B4h, records the resource list offset at +B8h/header +1Ch, then count BAADF00D pairs. RET 4.
U __fastcall kotor_fastcall_005dda50(void* self, void*, U count) {
    struct Key { U resref[4]; U id; U16 type; U16 unused; } key = {{0, 0, 0, 0}, 0, 0xffff, 0}; // [esp+4..1Bh] 005DDA68..DA89
    static_assert(sizeof(Key) == 0x18, "CERFKey record is 18h bytes");
    if (file_of(self) == nullptr) return 0;                                                    // 005DDA90..DA9C, DBA9
    at<U>(self, 0xbc) = count;                                                                 // 005DDAAA
    at<U>(self, 0xc0) = 0;                                                                     // 005DDAB0
    (void)seek(file_of(self), 0x10);                                                           // 005DDAA7..DAB6
    (void)write(file_of(self), &count, 4);                                                     // 005DDABB..DACA
    (void)seek(file_of(self), at<U>(self, 0xb4));                                              // 005DDACF..DADD
    for (U i = 0; i < count; ++i) (void)write(file_of(self), &key, 0x18);                      // 005DDAE2..DB0B
    at<U>(self, 0xb8) = kotor_fastcall_005e6940(file_of(self), nullptr);                       // 005DDB0D..DB27
    (void)seek(file_of(self), 0x1c);                                                           // 005DDB1E..DB29
    (void)write(file_of(self), static_cast<unsigned char*>(self) + 0xb8, 4);                   // 005DDB2E..DB39
    (void)kotor_fastcall_005e6af0(file_of(self), nullptr);                                     // 005DDB3E..DB44
    const U placeholder = 0xbaadf00du;                                                         // 005DDB4D [esp+0Ch]
    for (U i = 0; i < count; ++i) {                                                            // 005DDB49..DB8F
        (void)write(file_of(self), &placeholder, 4);                                           // 005DDB60..DB6F
        (void)write(file_of(self), &placeholder, 4);                                           // 005DDB74..DB83
    }
    return 1;                                                                                  // 005DDB92
}

// 005DD120 CERFKey::Write(CExoFile& file): resref (10h), id (4), type (2), unused (2). RET 4, EAX = 1.
U __fastcall kotor_fastcall_005dd120(void* key, void*, void* file) {
    unsigned char* k = static_cast<unsigned char*>(key);
    (void)write(file, k, 0x10);                                                                // 005DD126..D12F
    (void)write(file, k + 0x10, 4);                                                            // 005DD134..D13E
    (void)write(file, k + 0x14, 2);                                                            // 005DD143..D14D
    (void)write(file, k + 0x16, 2);                                                            // 005DD152..D15C
    return 1;                                                                                  // 005DD162
}

// 005DDBC0 CERFFile::WriteResource(CExoString const& path, int): appends the file's bytes, fills its resource
// entry (offset, size) and key (name without directory/extension, type from the extension, id). RET 8.
// The read buffer (size + 1 bytes) is never freed and the resref copy is GetLength bytes (original).
U __fastcall kotor_fastcall_005ddbc0(void* self, void*, void* path, U) {
    if (at<U>(self, 0xc0) >= at<U>(self, 0xbc) || file_of(self) == nullptr) return 0;          // 005DDBE8..DC01, DF13
    struct Key { unsigned char resref[16]; U id; U16 type; U16 unused; } key = {{}, 0, 0xffff, 0}; // [esp+48h] 005DDC07..DC22
    void* memory = live_operator_new(4);                                                       // 005DDC39..DC45
    void* in = nullptr;                                                                        // [esp+18h]
    if (memory != nullptr) {                                                                   // 005DDC4E..DC58
        try {
            in = file_construct(memory, path, 0xffff, k_rb_0073d5b0);                          // 005DDC5A..DCA8
        } catch (...) {
            live_free(memory);                                                                 // EH state 2
            throw;
        }
    }
    alignas(4) unsigned char name[8], extension[8], tmp[8];                                    // [esp+24h], [esp+34h], [esp+2Ch]
    string_text(tmp, k_colon_0073e2a0);                                                        // 005DDCAD..DCB6
    I colon;
    {
        struct G { void* s; ~G() { string_destroy(s); } } g{tmp};
        colon = string_find(path, tmp);                                                        // 005DDCBB..DCC9
    }                                                                                          // 005DDCCE..DCD9
    (void)base_ini_reconstruction::right_005e6170(str(path), nullptr, str(name),
        static_cast<int>(async_string_length_005e5790(path, nullptr)) - colon - 1);            // 005DDCDE..DCF0
    struct GName { void* s; ~GName() { string_destroy(s); } } g_name{name};
    string_text(tmp, k_dot_00741480);                                                          // 005DDCF5..DD03
    I dot;
    {
        struct G { void* s; ~G() { string_destroy(s); } } g{tmp};
        dot = string_find(name, tmp);                                                          // 005DDD08..DD18
    }                                                                                          // 005DDD1D..DD28
    (void)base_ini_reconstruction::right_005e6170(str(name), nullptr, str(extension),
        static_cast<int>(async_string_length_005e5790(name, nullptr)) - dot - 1);              // 005DDD2D..DD43
    struct GExt { void* s; ~GExt() { string_destroy(s); } } g_ext{extension};
    (void)base_ini_reconstruction::left_005e5f70(str(name), nullptr, str(tmp), dot);           // 005DDD48..DD57
    {
        struct G { void* s; ~G() { string_destroy(s); } } g{tmp};
        (void)async_assign_string_005e5c50(name, nullptr, tmp);                                // 005DDD5C..DD66
    }                                                                                          // 005DDD6B..DD74
    const U type = static_cast<U16>(c2da_base_005e6670(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, extension)); // 005DDD79..DDA2
    std::memset(key.resref, 0, 16);                                                            // 005DDD89..DD9A
    const U length = async_string_length_005e5790(name, nullptr);                              // 005DDD9E..DDAF
    std::memcpy(key.resref, WindowCStr005e5670(name, nullptr), length);                        // 005DDDAB..DDD6 (rep movs, unbounded)
    key.type = static_cast<U16>(type);                                                         // 005DDDD8..DDE1
    key.id = at<U>(self, 0xc0);                                                                // 005DDDC4, DDE8
    U size = exofile_size_005e6950(in, nullptr);                                               // 005DDDE6..DDF1 [esp+10h]
    void* buffer = live_operator_new(size + 1);                                                // 005DDDF5..DDFC (never freed)
    U offset = kotor_fastcall_005e6940(file_of(self), nullptr);                                // 005DDDFC..DE16 [esp+20h]
    (void)exofile_read_005e6960(in, nullptr, buffer, size, 1);                                 // 005DDE0C..DE1A
    (void)write(file_of(self), buffer, size);                                                  // 005DDE1F..DE2D
    (void)seek(file_of(self), at<U>(self, 0xb8) + at<U>(self, 0xc0) * 8);                      // 005DDE32..DE4A
    (void)write(file_of(self), &offset, 4);                                                    // 005DDE4F..DE5E
    (void)write(file_of(self), &size, 4);                                                      // 005DDE63..DE72
    (void)kotor_fastcall_005e6af0(file_of(self), nullptr);                                     // 005DDE77..DE7D
    if (in != nullptr) {                                                                       // 005DDE82..DE84
        file_destroy_005e69e0(in, nullptr);                                                    // 005DDE86..DE88
        live_free(in);                                                                         // 005DDE8D..DE93
    }
    (void)seek(file_of(self), at<U>(self, 0xb4) + at<U>(self, 0xc0) * 0x18);                   // 005DDE96..DEB1
    (void)kotor_fastcall_005dd120(&key, nullptr, file_of(self));                               // 005DDEB6..DEC1
    (void)kotor_fastcall_005e6af0(file_of(self), nullptr);                                     // 005DDEC6..DECC
    at<U>(self, 0xc0) = at<U>(self, 0xc0) + 1;                                                 // 005DDED1..DEDC
    return 1;                                                                                  // 005DDEE2..DEFA (~extension, ~name)
}

// 005DE1A0 CERFFile::ImportFiles(CExoString const& dir, int type, int (*callback)(int, int, int, int), a4, a5):
// every file of the directory list is written as a resource; the callback gets (index, count, a4, a5). RET 14h.
void __fastcall kotor_fastcall_005de1a0(void* self, void*, void* directory, U type, void* callback, U a4, U a5) {
    struct List { U* data; I count; I allocated; } list = {nullptr, 0, 0};                     // [esp+14h] 005DE1C5..DE1CD
    alignas(4) unsigned char path[8];                                                          // [esp+0Ch]
    string_default(path);                                                                      // 005DE1D1..DE1D9
    auto cleanup = [&]() {
        string_destroy(path);                                                                  // 005DE26D..DE276
        if (list.data != nullptr) {                                                            // 005DE27B..DE289
            const I n = static_cast<I>(list.data[-1]);
            for (I k = n - 1; k >= 0; --k) string_destroy(list.data + 2 * k);                  // 005DE28B..DE29A eh_vector_destructor_iterator
            live_free(list.data - 1);                                                          // 005DE29F..DE2A5
        }
    };
    try {
        f200_ports::Enumerate005e6640(kotor_menu_10_19::dependencies::base_007a39e0, nullptr, &list, str(directory),
                                      0xffff, 0, 0);                                           // 005DE1DE..DE1F9
        (void)kotor_fastcall_005dda50(self, nullptr, static_cast<U>(list.count));              // 005DE1FE..DE205
        using Callback = int(__cdecl*)(I, I, U, U);
        for (I i = 0; i < list.count; ++i) {                                                   // 005DE20A..DE269
            frame_LogFormat_005e5680(path, k_join_00740ccc, WindowCStr005e5670(directory, nullptr),
                                     list.data[2 * i], list.data[2 * i + 1]);                  // 005DE213..DE233 (CExoString by value)
            (void)kotor_fastcall_005ddbc0(self, nullptr, path, type);                          // 005DE238..DE246
            if (callback != nullptr)                                                           // 005DE24B..DE24D
                (void)reinterpret_cast<Callback>(callback)(i, list.count, a4, a5);             // 005DE24F..DE25F
        }
    } catch (...) {
        cleanup();
        throw;
    }
    cleanup();
}

// 005DD0E0 CERFFile::Finish: Flush, delete the CExoFile, +ACh = 0. RET 0, EAX = 1. (Flush runs on a null file.)
U __fastcall kotor_fastcall_005dd0e0(void* self, void*) {
    cerf_c_exofile_flush_005e6a20(file_of(self));                                              // 005DD0E3..D0EA
    void* file = file_of(self);                                                                // 005DD0EF
    if (file != nullptr) {                                                                     // 005DD0F5..D0F7
        file_destroy_005e69e0(file, nullptr);                                                  // 005DD0F9..D0FB
        live_free(file);                                                                       // 005DD100..D106
    }
    at<void*>(self, 0xac) = nullptr;                                                           // 005DD10A
    return 1;                                                                                  // 005DD114
}

// 00406990 CExoResMan::FileCopy(CExoString const& source, CExoString const& target, ushort source_type,
// ushort target_type): FFFFh target type means the source type. RET 10h, EAX = copied.
U __fastcall kotor_fastcall_00406990(void*, void*, void* source, void* target, U source_type, U target_type) {
    if (async_string_equal_005e5200(source, nullptr, target) != 0) return 0;                  // 004069B2..69D0
    if (static_cast<U16>(target_type) == 0xffff) target_type = source_type;                   // 004069D4..69E4
    alignas(4) unsigned char in[4], out[4];                                                    // [esp+34h], [esp+30h]
    (void)file_construct(in, source, source_type, k_rb_0073d5b0);                              // 004069E6..6A16
    struct GIn { void* f; ~GIn() { file_destroy_005e69e0(f, nullptr); } } g_in{in};
    (void)file_construct(out, target, target_type, k_wb_0073d8a4);                            // 00406A1B..6A47
    struct GOut { void* f; ~GOut() { file_destroy_005e69e0(f, nullptr); } } g_out{out};
    if (file_opened_005e6a10(in, nullptr) == 0 || file_opened_005e6a10(out, nullptr) == 0)     // 00406A4C..6A68
        return 0;                                                                              // 00406AE1..6AFF (~out, ~in)
    void* buffer = live_operator_new(exofile_size_005e6950(in, nullptr));                      // 00406A6A..6A82
    (void)exofile_read_005e6960(in, nullptr, buffer, exofile_size_005e6950(in, nullptr), 1);   // 00406A7C..6A8F
    (void)recovered_file_write_005e69a0(out, nullptr, buffer, exofile_size_005e6950(in, nullptr), 1); // 00406A94..6AA5
    cerf_c_exofile_flush_005e6a20(out);                                                        // 00406AAA..6AAE
    live_free(buffer);                                                                         // 00406AB3..6AB9
    return 1;                                                                                  // 00406ABC..6ADA (~out, ~in)
}
}  // extern "C"
}  // namespace w1c_erf
