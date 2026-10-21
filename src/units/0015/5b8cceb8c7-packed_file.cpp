// lane-packed-file: CExoPackedFile family - all slot bodies, constructors, destructors and
// the four method tables, in original slot order. BIF/ERF bodies: donor lane-resman
// src/packed_files.cpp (19:42), re-reviewed instruction by instruction against
// evidence/bif_small.txt, evidence/erf.txt, evidence/rim_ctor.txt (own dumps of the pinned
// GOG swkotor.exe); RIM, abstract table and consumer names authored here. Never executed.
#include "92d14e6be8-packed_file.h"
#include "a481b9bd58-size_seek.h"      // M40 file-directory-epoch: file_internal::exofile_seek_005e6aa0
#include <cstring>
#include <new>

using kotor_packed_file::Word;
using kotor_packed_file::String;
using kotor_packed_file::Methods;
using kotor_packed_file::ImageMethods;

// ---------------------------------------------------------------------------
// Imports: existing M40 providers (exact symbols; see PACKET undefined_externals).
extern "C" {
void* __cdecl live_operator_new(std::uint32_t);                            // 006FA7E6 operator new
void __cdecl live_free(void*);                                              // 006FA390 free
extern const char c2da_directory_literal_0073d5b0[];                        // 0073D5B0 "rb"
Word __fastcall c2da_file_005e6a10(void*, void*);                           // 005E6A10 CExoFile::FileOpened
Word __fastcall c2da_file_005e6950(void*, void*);                           // 005E6950 CExoFile::GetSize
Word __fastcall c2da_file_005e6960(void*, void*, void*, Word, Word);        // 005E6960 CExoFile::Read(buf,size,count) -> bytes
void __fastcall c2da_file_005e6980(void*, void*, void*, Word, Word);        // 005E6980 CExoFile::ReadAsync(buf,size,count)
void __fastcall c2da_005e69e0(void*, void*);                                // 005E69E0 CExoFile::~CExoFile
char* __fastcall WindowCStr005e5670(String*, void*);                        // 005E5670 CExoString::CStr
Word __stdcall cres_base_constant_word_0063e7a0();                          // 0063E7A0 mov eax,1; ret (leaf.obj)
void __cdecl unresolved_fatal_protected_006FB344(int);                      // 006FB344 _amsg_exit (rim providers.obj)
// CExoLocString (owner: lane-resman src/locstring.cpp, exact same declarations; not M40).
void* __fastcall exo_locstring_construct_005e9e10(void* self, void* unused_edx) noexcept(false); // 0040EFD1
void __fastcall exo_locstring_clear_005e9ea0(void* self, void* unused_edx);                      // 0040DBB4
void __fastcall exo_locstring_destroy_005ea000(void* self, void* unused_edx);                    // 0040F071
Word __fastcall exo_locstring_add_005ea340(void* self, void* unused_edx, Word language, String* text, Word gender); // 0040E53C
// 005E5B70 CExoString::CExoString(char const*, int) RET8: single provider lane-resman (DECISIONS D18).
String* __fastcall resman_string_counted_005e5b70(String* self, void* unused_edx, const char* text, int length);
}
// 005E68D0 CExoFile::CExoFile(CExoString const&, unsigned short, CExoString const&) on raw
// 4-byte storage; C++ linkage keeps the /EHsc throwing transport (m39-directory-ports.obj).
void* __fastcall c2da_file_construct_005e68d0(void*, void*, String*, Word, String*);

namespace {
template<class T> T get(const void* p, Word off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
template<class T> void put(void* p, Word off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v)); }
const Methods* methods(const void* self) { return get<const Methods*>(self, 0); }
void set_methods(void* self, const void* table) { put(self, 0, table); }
String* name_of(void* self) { return std::launder(reinterpret_cast<String*>(static_cast<unsigned char*>(self) + 4)); }
void* at(void* self, Word off) { return static_cast<unsigned char*>(self) + off; }
const unsigned char* bytes(const void* self, Word off) { return get<const unsigned char*>(self, off); }

Word seek(void* file, Word offset) {  // CExoFile::Seek(offset, 0) at every site (push 0; push offset)
    return file_internal::exofile_seek_005e6aa0(static_cast<file_internal::File*>(file), nullptr, offset, 0u);
}
// Release a CExoFile: ~CExoFile then free (e.g. 0040D5DA-0040D5E2).
void destroy_file(void* file) { c2da_005e69e0(file, nullptr); live_free(file); }

// `member = new CExoFile(name, type, CExoString("rb"))` (0040D730-0040D795 and every sibling):
// new(4); only when nonnull build the temp CExoString("rb") (005E5A90) and construct; store the
// result (or 0) to the member, then destroy the temp. Unwind: temp state destroys the temp,
// then the raw-allocation state frees it (funclets 007148C1/007148D2 family).
void open_into(void* self, Word member, Word type) {
    void* allocation = live_operator_new(4);
    if (!allocation) { put<void*>(self, member, nullptr); return; }
    try {
        String mode(c2da_directory_literal_0073d5b0);
        void* file = c2da_file_construct_005e68d0(allocation, nullptr, name_of(self), type, &mode);
        put(self, member, file);
    } catch (...) { live_free(allocation); throw; }
}

// Encapsulated archive mode -> CExoFile resource type (0040DCAC 7DB, 0040DD1A 809,
// 0040DD8C 270D, 0040DDFE 80D, 0040DE69 80E; same in 0040DF10 and 0040E23B-0040E3E5).
bool encapsulated_type(unsigned char mode, Word* type) {
    switch (mode) {
    case 0: *type = 0x7dbu; return true;
    case 1: *type = 0x809u; return true;
    case 2: *type = 0x270du; return true;
    case 3: *type = 0x80du; return true;
    case 4: *type = 0x80eu; return true;
    default: return false;   // 0040DE38 jne 0040DEA4: nothing constructed, member already 0
    }
}

// Encapsulated sync/async open body (0040DC30 / 0040DF10, diff = +14/+24 vs +18/+28 only).
Word encapsulated_open(void* self, Word flag, Word member) {
    if (get<Word>(self, flag)) return 1;                       // 0040DC58-0040DC5E
    const unsigned char mode = get<unsigned char>(self, 0x40);  // 0040DC75
    put<void*>(self, member, nullptr);                         // 0040DC7A
    Word type = 0;
    if (encapsulated_type(mode, &type)) open_into(self, member, type);
    void* file = get<void*>(self, member);                     // 0040DEA4
    if (file) {
        if (c2da_file_005e6a10(file, nullptr)) {               // 0040DEAD-0040DEB4
            put<Word>(self, flag, 1);                          // 0040DEF1
            return 1;
        }
        void* live = get<void*>(self, member);                 // 0040DEB6
        if (live) destroy_file(live);                          // 0040DEBD-0040DEC5
        put<void*>(self, member, nullptr);                     // 0040DECD
    }
    put<Word>(self, flag, 0);                                  // 0040DED0
    return 0;
}

// CExoResFile sync/async open body (0040D6F0 / 0040D800): BIF type 0x270E.
Word res_file_open(void* self, Word flag, Word member, bool describe) {
    if (get<Word>(self, flag)) return 1;                       // 0040D712-0040D71A
    open_into(self, member, 0x270eu);                          // 0040D730-0040D795
    void* file = get<void*>(self, member);                     // 0040D79A
    if (!file) return 0;                                       // 0040D79F -> 0040D7C8 (flag untouched)
    if (!c2da_file_005e6a10(file, nullptr)) {                  // 0040D7A1-0040D7A8
        void* live = get<void*>(self, member);                 // 0040D7AA
        if (live) destroy_file(live);
        put<void*>(self, member, nullptr);                     // 0040D7C1 (flag untouched)
        return 0;
    }
    if (describe) (void)WindowCStr005e5670(name_of(self), nullptr); // 0040D7DC-0040D7DF (sync only; result dropped)
    put<Word>(self, flag, 1);                                  // 0040D7EE / 0040D8F6
    return 1;
}

// Abstract slots of 0073E128 all hold 006FB8DE. Typed D1 adapters (no address credit) keep
// each slot's transport without casts and enter the one 006FB8DE body.
Word __fastcall pure_effect(void*, void*) { packed_purecall_006fb8de(); return 0; }
Word __fastcall pure_sized(void*, void*, Word) { packed_purecall_006fb8de(); return 0; }
Word __fastcall pure_transfer(void*, void*, Word, void*, Word, Word) { packed_purecall_006fb8de(); return 0; }
void __fastcall pure_async_transfer(void*, void*, Word, void*, Word, Word) { packed_purecall_006fb8de(); }
Word __fastcall pure_load_header(void*, void*, Word) { packed_purecall_006fb8de(); return 0; }

// RIM slots +08/+18 hold 004015A0 (a bare RET, ICF-shared; M41 owns a cdecl-typed provider of
// that address for an unrelated slot type). The original leaves EAX as the caller left it, which
// has no faithful source value: return 0 (documented deviation, D2 analogue). No consumer reads it.
Word __fastcall image_void_slot(void*, void*) { return 0; }
// RIM slots +10/+30/+40 hold 0063E7A0: typed adapter over the existing leaf provider.
Word __fastcall image_true_slot(void*, void*) { return cres_base_constant_word_0063e7a0(); }
}

// ---------------------------------------------------------------------------
// 006FB8DE _purecall: push 19h; call 006FB344 (_amsg_exit); pop ecx; ret.
extern "C" __declspec(noinline) void __cdecl packed_purecall_006fb8de(void) {
    unresolved_fatal_protected_006FB344(0x19);
}
// 0063E7C0: xor eax,eax; ret 4 (RIM +3C LoadHeader).
extern "C" __declspec(noinline) Word __fastcall packed_return_false_0063e7c0(void*, void*, Word) {
    return 0;
}

// ---------------------------------------------------------------------------
// Trivial accessors.
// 0040D570 CExoPackedFile::GetFile: mov eax,[ecx+14h]; ret.
extern "C" __declspec(noinline) void* __fastcall packed_file_get_file_0040d570(void* self, void*) {
    return get<void*>(self, 0x14);
}
// 00644760 (ICF-shared accessor): mov eax,[ecx+18h]; ret. Slot +20 of all four tables.
extern "C" __declspec(noinline) void* __fastcall packed_file_get_async_file_00644760(void* self, void*) {
    return get<void*>(self, 0x18);
}

// ---------------------------------------------------------------------------
// Reference counting (CExoResFile bodies shared by BIF/ERF; 0040D580 also RIM +04).
// 0040D580 AddRefCount: if !((signed)[+1C] > 0) { [+1C]=0; vslot2C } ; [+1C]++.
// EAX: the loaded count (0040D583) or the OpenFile result (0040D593).
extern "C" __declspec(noinline) Word __fastcall res_file_add_ref_0040d580(void* self, void*) noexcept(false) {
    Word result = get<Word>(self, 0x1c);                      // 0040D583
    if (!(static_cast<std::int32_t>(result) > 0)) {           // 0040D586-0040D588 jg
        const Methods* table = methods(self);                 // 0040D58A
        put<Word>(self, 0x1c, 0);                             // 0040D58C
        result = table->open_file(self, nullptr);             // 0040D593
    }
    put<Word>(self, 0x1c, get<Word>(self, 0x1c) + 1u);        // 0040D596
    return result;
}
// 0040D5A0 AddAsyncRefCount: same on [+20], vslot30.
extern "C" __declspec(noinline) Word __fastcall res_file_add_async_ref_0040d5a0(void* self, void*) noexcept(false) {
    Word result = get<Word>(self, 0x20);                      // 0040D5A3
    if (!(static_cast<std::int32_t>(result) > 0)) {           // 0040D5A6-0040D5A8
        const Methods* table = methods(self);                 // 0040D5AA
        put<Word>(self, 0x20, 0);                             // 0040D5AC
        result = table->open_async_file(self, nullptr);       // 0040D5B3
    }
    put<Word>(self, 0x20, get<Word>(self, 0x20) + 1u);        // 0040D5B6
    return result;
}
// 0040D650 DeleteRefCount: (signed)[+1C] > 1 ? --[+1C] (EAX=new) : ([+1C]=0, tail vslot0C).
extern "C" __declspec(noinline) Word __fastcall res_file_delete_ref_0040d650(void* self, void*) {
    const Word count = get<Word>(self, 0x1c);                 // 0040D650
    if (static_cast<std::int32_t>(count) > 1) {               // 0040D653-0040D656 jg
        put<Word>(self, 0x1c, count - 1u);                    // 0040D664-0040D665
        return count - 1u;
    }
    const Methods* table = methods(self);                     // 0040D658
    put<Word>(self, 0x1c, 0);                                 // 0040D65A
    return table->close_file(self, nullptr);                  // 0040D661 jmp [eax+0Ch]
}
// 0040D670 DeleteAsyncRefCount: same on [+20], tail vslot10.
extern "C" __declspec(noinline) Word __fastcall res_file_delete_async_ref_0040d670(void* self, void*) {
    const Word count = get<Word>(self, 0x20);
    if (static_cast<std::int32_t>(count) > 1) {               // 0040D673-0040D676
        put<Word>(self, 0x20, count - 1u);                    // 0040D684-0040D685
        return count - 1u;
    }
    const Methods* table = methods(self);                     // 0040D678
    put<Word>(self, 0x20, 0);                                 // 0040D67A
    return table->close_async_file(self, nullptr);            // 0040D681 jmp [eax+10h]
}
// 0040DB50 CloseAsyncFile (BIF/ERF): if [+28] { f=[+18]; if f {~CExoFile; free; [+18]=0} [+28]=0 } return 1.
extern "C" __declspec(noinline) Word __fastcall res_file_close_async_0040db50(void* self, void*) {
    if (get<Word>(self, 0x28)) {                              // 0040DB53-0040DB58
        void* file = get<void*>(self, 0x18);                  // 0040DB5B
        if (file) { destroy_file(file); put<void*>(self, 0x18, nullptr); } // 0040DB62-0040DB72
        put<Word>(self, 0x28, 0);                             // 0040DB79
    }
    return 1;
}

// ---------------------------------------------------------------------------
// CExoResFile (BIF, table 0073E1B0).
// 0040D5C0 CloseFile: if [+24] { CStr(name) dropped; f=[+14]; if f {destroy; [+14]=0} [+24]=0 } return 1.
extern "C" __declspec(noinline) Word __fastcall res_file_close_0040d5c0(void* self, void*) {
    if (get<Word>(self, 0x24)) {                              // 0040D5C3-0040D5C8
        (void)WindowCStr005e5670(name_of(self), nullptr);      // 0040D5CB-0040D5CE
        void* file = get<void*>(self, 0x14);                  // 0040D5D3
        if (file) { destroy_file(file); put<void*>(self, 0x14, nullptr); } // 0040D5DA-0040D5EA
        put<Word>(self, 0x24, 0);                             // 0040D5F1
    }
    return 1;
}
// 0040D600 UnloadHeader: if [+2C] { free [+34] (zero); free [+30] (zero); [+2C]=0; vslot14 } return 1.
extern "C" __declspec(noinline) Word __fastcall res_file_unload_header_0040d600(void* self, void*) {
    if (get<Word>(self, 0x2c)) {                              // 0040D603-0040D608
        void* entries = get<void*>(self, 0x34);
        if (entries) { live_free(entries); put<void*>(self, 0x34, nullptr); } // 0040D60A-0040D61A
        void* header = get<void*>(self, 0x30);
        if (header) { live_free(header); put<void*>(self, 0x30, nullptr); }   // 0040D621-0040D631
        const Methods* table = methods(self);                 // 0040D638
        put<Word>(self, 0x2c, 0);                             // 0040D63C
        (void)table->delete_ref(self, nullptr);               // 0040D643
    }
    return 1;
}
// 0040D690 GetResourceSize(key): entries[(key&3FFF)*16 + 8]; no bounds check.
extern "C" __declspec(noinline) Word __fastcall res_file_size_0040d690(void* self, void*, Word key) {
    return get<Word>(bytes(self, 0x34), ((key & 0x3fffu) << 4) + 8u);
}
// 0040D6B0 Initialize: CStr(name) dropped; zero +14,+18,u16 +0C,+10,+1C,+20,+24,+28,+30,+34; return 1.
extern "C" __declspec(noinline) Word __fastcall res_file_initialize_0040d6b0(void* self, void*) {
    (void)WindowCStr005e5670(name_of(self), nullptr);          // 0040D6B3-0040D6B6
    put<Word>(self, 0x14, 0); put<Word>(self, 0x18, 0); put<std::uint16_t>(self, 0x0c, 0);
    put<Word>(self, 0x10, 0); put<Word>(self, 0x1c, 0); put<Word>(self, 0x20, 0);
    put<Word>(self, 0x24, 0); put<Word>(self, 0x28, 0); put<Word>(self, 0x30, 0); put<Word>(self, 0x34, 0); // 0040D6BD-0040D6D9
    return 1;
}
extern "C" __declspec(noinline) Word __fastcall res_file_open_0040d6f0(void* self, void*) noexcept(false) {
    return res_file_open(self, 0x24, 0x14, true);
}
extern "C" __declspec(noinline) Word __fastcall res_file_open_async_0040d800(void* self, void*) noexcept(false) {
    return res_file_open(self, 0x28, 0x18, false);
}
// 0040D910 LoadHeader(unused) RET4.
extern "C" __declspec(noinline) Word __fastcall res_file_load_header_0040d910(void* self, void*, Word) noexcept(false) {
    if (get<Word>(self, 0x2c)) return 1;                       // 0040D913-0040D91B -> 0040DA11
    (void)methods(self)->add_ref(self, nullptr);              // 0040D921-0040D923 vslot04
    if (!get<void*>(self, 0x14)) {                             // 0040D926
        put<Word>(self, 0x1c, get<Word>(self, 0x1c) - 1u);    // 0040D92B dec (no close: quirk)
        return 0;
    }
    (void)WindowCStr005e5670(name_of(self), nullptr);          // 0040D935-0040D938
    put<Word>(self, 0x10, c2da_file_005e6950(get<void*>(self, 0x14), nullptr)); // 0040D93D-0040D947
    void* header = live_operator_new(0x14);                    // 0040D94A (no null test: quirk)
    put<void*>(self, 0x30, header);                            // 0040D95A
    (void)c2da_file_005e6960(get<void*>(self, 0x14), nullptr, header, 0x14u, 1u); // 0040D94F-0040D95D
    const unsigned char* h = bytes(self, 0x30);                // 0040D962
    if (get<Word>(h, 0) != 0x46464942u ||                      // 0040D965 "BIFF"
        get<Word>(h, 4) != 0x20203156u) {                      // 0040D987 "V1  "
        live_free(get<void*>(self, 0x30));                     // 0040D96D / 0040D990
        const Methods* table = methods(self);                  // 0040D973 / 0040D996
        put<void*>(self, 0x30, nullptr);                       // 0040D97A / 0040D99D
        (void)table->delete_ref(self, nullptr);                // 0040D97D / 0040D9A0
        return 0;
    }
    put<void*>(self, 0x34, nullptr);                           // 0040D9AA
    const Word count = get<Word>(h, 8);                        // 0040D9AD
    if (count) put<void*>(self, 0x34, live_operator_new(count << 4)); // 0040D9B4-0040D9C0
    if (!get<void*>(self, 0x34)) {                             // 0040D9C3
        live_free(get<void*>(self, 0x30));                     // 0040D9C8-0040D9CC
        const Methods* table = methods(self);                  // 0040D9D1
        put<void*>(self, 0x30, nullptr);                       // 0040D9D8
        (void)table->delete_ref(self, nullptr);                // 0040D9DB
        return 0;
    }
    (void)seek(get<void*>(self, 0x14), get<Word>(bytes(self, 0x30), 0x10));          // 0040D9E5-0040D9F0
    (void)c2da_file_005e6960(get<void*>(self, 0x14), nullptr, get<void*>(self, 0x34), 0x10u,
                             get<Word>(bytes(self, 0x30), 8));                        // 0040D9F5-0040DA05
    put<Word>(self, 0x2c, 1);                                  // 0040DA0A (sync ref stays held)
    return 1;
}
// 0040DA20 ReadResource(key, buffer, size, unused) RET10h: read min(stored,size) bytes, count 1.
extern "C" __declspec(noinline) Word __fastcall res_file_read_0040da20(void* self, void*, Word key, void* buffer, Word size, Word) {
    if (!get<Word>(self, 0x24) || !buffer) return 0;          // 0040DA24-0040DA31
    const Word index = key & 0x3fffu;                          // 0040DA3D
    if (index >= get<Word>(bytes(self, 0x30), 8)) return 0;    // 0040DA37-0040DA44 (unsigned jae)
    const Word row = index << 4;                               // 0040DA4D
    (void)seek(get<void*>(self, 0x14), get<Word>(bytes(self, 0x34), row + 4u));      // 0040DA46-0040DA59
    const Word stored = get<Word>(bytes(self, 0x34), row + 8u);                       // 0040DA5E-0040DA61
    return c2da_file_005e6960(get<void*>(self, 0x14), nullptr, buffer, stored > size ? size : stored, 1u); // 0040DA65-0040DA81
}
// 0040DAA0 ReadResourceAsync: same on +28/+18 with CExoFile::ReadAsync; EAX unclaimed.
extern "C" __declspec(noinline) void __fastcall res_file_read_async_0040daa0(void* self, void*, Word key, void* buffer, Word size, Word) {
    if (!get<Word>(self, 0x28) || !buffer) return;            // 0040DAA3-0040DAB2
    const Word index = key & 0x3fffu;
    if (index >= get<Word>(bytes(self, 0x30), 8)) return;     // 0040DAB4-0040DAC5
    const Word row = index << 4;
    (void)seek(get<void*>(self, 0x18), get<Word>(bytes(self, 0x34), row + 4u));      // 0040DAC7-0040DAD9
    const Word stored = get<Word>(bytes(self, 0x34), row + 8u);                       // 0040DADE-0040DAE1
    c2da_file_005e6980(get<void*>(self, 0x18), nullptr, buffer, stored > size ? size : stored, 1u); // 0040DAE5-0040DB01
}

// ---------------------------------------------------------------------------
// CExoEncapsulatedFile (table 0073E1F8).
// 0040DB10 CloseFile: like 0040D5C0 without the CStr call.
extern "C" __declspec(noinline) Word __fastcall encapsulated_close_0040db10(void* self, void*) {
    if (get<Word>(self, 0x24)) {                              // 0040DB13-0040DB18
        void* file = get<void*>(self, 0x14);
        if (file) { destroy_file(file); put<void*>(self, 0x14, nullptr); } // 0040DB1B-0040DB32
        put<Word>(self, 0x24, 0);                             // 0040DB39
    }
    return 1;
}
// 0040DB90 UnloadHeader: if [+2C] { free [+3C] (zero); ClearLocString(+30); free [+38] (zero); [+2C]=0 } return 1.
extern "C" __declspec(noinline) Word __fastcall encapsulated_unload_header_0040db90(void* self, void*) {
    if (get<Word>(self, 0x2c)) {                              // 0040DB93-0040DB98
        void* rows = get<void*>(self, 0x3c);
        if (rows) { live_free(rows); put<void*>(self, 0x3c, nullptr); }     // 0040DB9A-0040DBAA
        exo_locstring_clear_005e9ea0(at(self, 0x30), nullptr);              // 0040DBB1-0040DBB4
        void* header = get<void*>(self, 0x38);
        if (header) { live_free(header); put<void*>(self, 0x38, nullptr); } // 0040DBB9-0040DBC9
        put<Word>(self, 0x2c, 0);                             // 0040DBD0
    }
    return 1;
}
// 0040DBE0 GetResourceSize(key): rows[(key&3FFF)].size; no bounds check.
extern "C" __declspec(noinline) Word __fastcall encapsulated_size_0040dbe0(void* self, void*, Word key) {
    return get<Word>(bytes(self, 0x3c), (key & 0x3fffu) * 8u + 4u);
}
// 0040DC00 Initialize: zero +14,+18,u16 +0C,+10,+1C,+20,+24,+28,+38,+3C, u8 +40; return 1.
extern "C" __declspec(noinline) Word __fastcall encapsulated_initialize_0040dc00(void* self, void*) {
    put<Word>(self, 0x14, 0); put<Word>(self, 0x18, 0); put<std::uint16_t>(self, 0x0c, 0);
    put<Word>(self, 0x10, 0); put<Word>(self, 0x1c, 0); put<Word>(self, 0x20, 0);
    put<Word>(self, 0x24, 0); put<Word>(self, 0x28, 0); put<Word>(self, 0x38, 0); put<Word>(self, 0x3c, 0);
    put<unsigned char>(self, 0x40, 0);                        // 0040DC02-0040DC21
    return 1;
}
extern "C" __declspec(noinline) Word __fastcall encapsulated_open_0040dc30(void* self, void*) noexcept(false) {
    return encapsulated_open(self, 0x24, 0x14);
}
extern "C" __declspec(noinline) Word __fastcall encapsulated_open_async_0040df10(void* self, void*) noexcept(false) {
    return encapsulated_open(self, 0x28, 0x18);
}

namespace {
// Raw storage for the counted CExoString temporary built at 0040E509 and destroyed at 0040E526
// (EH state 10h only while it is live).
struct CountedTemporary {
    alignas(String) unsigned char storage[sizeof(String)];
    bool constructed = false;
    String* address() { return std::launder(reinterpret_cast<String*>(storage)); }
    ~CountedTemporary() { if (constructed) address()->~StartupString(); }
};
// 0040E57F-0040E5A2: free header (+38), clear it, then destroy/free the sync CExoFile, clear +14.
Word failed_header(void* self) {
    live_free(get<void*>(self, 0x38));                         // 0040E57F-0040E580
    put<void*>(self, 0x38, nullptr);                           // 0040E588
    void* file = get<void*>(self, 0x14);                       // 0040E58B
    if (file) destroy_file(file);                              // 0040E592-0040E59A
    put<void*>(self, 0x14, nullptr);                           // 0040E5A2
    return 0;
}
// 0040E58B-0040E5A2
Word closed_file(void* self) {
    void* file = get<void*>(self, 0x14);
    if (file) destroy_file(file);
    put<void*>(self, 0x14, nullptr);
    return 0;
}
}

// 0040E1F0 LoadHeader(u8 mode) RET4. Opens its own sync CExoFile (flag +24 untouched), reads the
// 0xA0 header, localized strings into +30, the resource list into +3C, then destroys the file.
extern "C" __declspec(noinline) Word __fastcall encapsulated_load_header_0040e1f0(void* self, void*, Word mode_word) noexcept(false) {
    String local;                                              // 0040E215-0040E21F (destroyed on every exit)
    if (get<Word>(self, 0x2c)) return 1;                       // 0040E224-0040E22B -> 0040E60F
    const unsigned char mode = static_cast<unsigned char>(mode_word); // 0040E231 byte [ebp+8]
    put<void*>(self, 0x14, nullptr);                           // 0040E236
    Word type = 0;
    if (encapsulated_type(mode, &type)) open_into(self, 0x14, type); // 0040E23B-0040E407
    void* file = get<void*>(self, 0x14);                       // 0040E409
    if (!file) return 0;                                       // 0040E40E -> 0040E5A5
    if (!c2da_file_005e6a10(file, nullptr)) return closed_file(self); // 0040E414-0040E41B
    put<unsigned char>(self, 0x40, mode);                      // 0040E421-0040E427
    put<Word>(self, 0x10, c2da_file_005e6950(get<void*>(self, 0x14), nullptr)); // 0040E424-0040E434
    void* header = live_operator_new(0xa0);                    // 0040E42F-0040E437
    put<void*>(self, 0x38, header);                            // 0040E441
    if (!header) return closed_file(self);                     // 0040E43F-0040E444 -> 0040E58B
    (void)c2da_file_005e6960(get<void*>(self, 0x14), nullptr, header, 0xa0u, 1u); // 0040E44A-0040E455
    const unsigned char* h = bytes(self, 0x38);                // 0040E45D
    const Word signature = get<Word>(h, 0);                    // 0040E460
    const Word expected = mode == 2 ? 0x20465245u /* "ERF " */ : mode == 3 ? 0x204b4148u /* "HAK " */ : 0x20444f4du /* "MOD " */;
    if (signature != expected || get<Word>(h, 4) != 0x302e3156u /* "V1.0" */) return failed_header(self); // 0040E45A-0040E496
    (void)seek(get<void*>(self, 0x14), get<Word>(h, 0x14));   // 0040E49C-0040E4A4
    const Word strings_size = get<Word>(bytes(self, 0x38), 0x0c); // 0040E4A9-0040E4AC
    if (strings_size) {                                        // 0040E4AF-0040E4B1
        unsigned char* strings = static_cast<unsigned char*>(live_operator_new(strings_size)); // 0040E4B7-0040E4B8
        if (!strings) return failed_header(self);              // 0040E4C5-0040E4CB (eax reloaded at 0040E4BF)
        (void)c2da_file_005e6960(get<void*>(self, 0x14), nullptr, strings, 1u,
                                 get<Word>(bytes(self, 0x38), 0x0c)); // 0040E4D1-0040E4DB (size 1, count n)
        const unsigned char* cursor = strings;                 // 0040E4E8
        if (get<Word>(bytes(self, 0x38), 8) != 0) {            // 0040E4E0-0040E4F2 (unsigned jbe)
            Word index = 0;                                    // 0040E4EA
            do {
                const Word language = get<Word>(cursor, 0);   // 0040E4F4
                const Word length = get<Word>(cursor, 4);     // 0040E4F6
                cursor += 8;                                   // 0040E4F9/0040E4FD
                {
                    CountedTemporary temporary;
                    String* built = resman_string_counted_005e5b70(temporary.address(), nullptr,
                        reinterpret_cast<const char*>(cursor), static_cast<int>(length)); // 0040E4FC-0040E509
                    temporary.constructed = true;              // 0040E513 state 10h
                    local.Assign005e5c50(built);               // 0040E50E-0040E518
                }                                              // 0040E51D-0040E526 temp destroyed
                cursor += length;                              // 0040E53A
                (void)exo_locstring_add_005ea340(at(self, 0x30), nullptr, language, &local, 0u); // 0040E52B-0040E53C
                ++index;                                       // 0040E541-0040E54B
            } while (index < get<Word>(bytes(self, 0x38), 8)); // 0040E545-0040E552 (reloaded each pass)
        }
        live_free(strings);                                    // 0040E554-0040E559
    }
    void* rows = live_operator_new(get<Word>(bytes(self, 0x38), 0x10) << 3); // 0040E563-0040E56D
    put<void*>(self, 0x3c, rows);                              // 0040E577
    if (!rows) return failed_header(self);                     // 0040E575-0040E57C
    (void)seek(get<void*>(self, 0x14), get<Word>(bytes(self, 0x38), 0x1c)); // 0040E5CC-0040E5D7
    (void)c2da_file_005e6960(get<void*>(self, 0x14), nullptr, get<void*>(self, 0x3c), 8u,
                             get<Word>(bytes(self, 0x38), 0x10)); // 0040E5DC-0040E5EC
    put<Word>(self, 0x2c, 1);                                  // 0040E5F1
    void* done = get<void*>(self, 0x14);                       // 0040E5F8
    if (done) destroy_file(done);                              // 0040E5FB-0040E607
    // Observed quirk: +14 is NOT cleared on this success path (0040E60C-0040E60F).
    return 1;
}
// 0040E640 ReadResource(key, buffer, size, unused) RET10h.
extern "C" __declspec(noinline) Word __fastcall encapsulated_read_0040e640(void* self, void*, Word key, void* buffer, Word size, Word) {
    if (!get<Word>(self, 0x24) || !buffer) return 0;          // 0040E644-0040E652
    const Word index = key & 0x3fffu;                          // 0040E65E
    if (index >= get<Word>(bytes(self, 0x38), 0x10)) return 0; // 0040E658-0040E666
    (void)seek(get<void*>(self, 0x14), get<Word>(bytes(self, 0x3c), index * 8u));     // 0040E668-0040E674
    const Word stored = get<Word>(bytes(self, 0x3c), index * 8u + 4u);                // 0040E679-0040E67C
    return c2da_file_005e6960(get<void*>(self, 0x14), nullptr, buffer, stored > size ? size : stored, 1u); // 0040E680-0040E69C
}
// 0040E6B0 ReadResourceAsync: same on +28/+18 with CExoFile::ReadAsync; EAX unclaimed.
extern "C" __declspec(noinline) void __fastcall encapsulated_read_async_0040e6b0(void* self, void*, Word key, void* buffer, Word size, Word) {
    if (!get<Word>(self, 0x28) || !buffer) return;            // 0040E6B3-0040E6C2
    const Word index = key & 0x3fffu;
    if (index >= get<Word>(bytes(self, 0x38), 0x10)) return;  // 0040E6C4-0040E6D6
    (void)seek(get<void*>(self, 0x18), get<Word>(bytes(self, 0x3c), index * 8u));     // 0040E6D8-0040E6E4
    const Word stored = get<Word>(bytes(self, 0x3c), index * 8u + 4u);                // 0040E6E9-0040E6EC
    c2da_file_005e6980(get<void*>(self, 0x18), nullptr, buffer, stored > size ? size : stored, 1u); // 0040E6F0-0040E70C
}

// ---------------------------------------------------------------------------
// CExoResourceImageFile (RIM, table 0073E240). Constructed inline by 0040F990 (f990 donor).
// 0040E720 CloseFile: if [+24] { if [+30] { free; [+30]=0 } [+24]=0 } return 1.
extern "C" __declspec(noinline) Word __fastcall image_file_close_0040e720(void* self, void*) {
    if (get<Word>(self, 0x24)) {                              // 0040E723-0040E728
        void* image = get<void*>(self, 0x30);                 // 0040E72A
        if (image) { live_free(image); put<void*>(self, 0x30, nullptr); } // 0040E72F-0040E73A
        put<Word>(self, 0x24, 0);                             // 0040E741
    }
    return 1;
}
// 0040E750 DeleteRefCount: (signed)[+1C] > 1 ? --[+1C] (EAX=new) : ([+1C]=0, EAX=old). No close.
extern "C" __declspec(noinline) Word __fastcall image_file_delete_ref_0040e750(void* self, void*) {
    const Word count = get<Word>(self, 0x1c);                 // 0040E750
    if (static_cast<std::int32_t>(count) > 1) {               // 0040E753-0040E756 jg
        put<Word>(self, 0x1c, count - 1u);                    // 0040E760-0040E761
        return count - 1u;
    }
    put<Word>(self, 0x1c, 0);                                 // 0040E758
    return count;                                             // 0040E75F (EAX still the loaded count)
}
// 0040E770 Initialize: zero +14, u16 +0C, +10, +1C, +24, +30; return 1.
extern "C" __declspec(noinline) Word __fastcall image_file_initialize_0040e770(void* self, void*) {
    put<Word>(self, 0x14, 0); put<std::uint16_t>(self, 0x0c, 0); put<Word>(self, 0x10, 0);
    put<Word>(self, 0x1c, 0); put<Word>(self, 0x24, 0); put<Word>(self, 0x30, 0); // 0040E772-0040E782
    return 1;
}
// 0040E790 OpenFile: load the whole file into +30 (type 0BC1h, retry 0BBAh), then drop the file.
extern "C" __declspec(noinline) Word __fastcall image_file_open_0040e790(void* self, void*) noexcept(false) {
    if (get<Word>(self, 0x24)) return 1;                       // 0040E7B2-0040E7CC
    open_into(self, 0x14, 0x0bc1u);                            // 0040E7CE-0040E832
    if (!get<void*>(self, 0x14)) return 0;                     // 0040E837-0040E851
    if (!c2da_file_005e6a10(get<void*>(self, 0x14), nullptr)) { // 0040E852-0040E859
        void* first = get<void*>(self, 0x14);                  // 0040E85F
        if (first) destroy_file(first);                        // 0040E866-0040E873
        put<void*>(self, 0x14, nullptr);                       // 0040E878
        open_into(self, 0x14, 0x0bbau);                        // 0040E876-0040E8E0
        if (!get<void*>(self, 0x14)) return 0;                 // 0040E8E5-0040E8EA
        // Observed quirk: a second CExoFile that fails FileOpened is neither destroyed nor
        // cleared; +14 keeps it (0040E8F0-0040E8F7 -> 0040E83E).
        if (!c2da_file_005e6a10(get<void*>(self, 0x14), nullptr)) return 0;
    }
    const Word size = c2da_file_005e6950(get<void*>(self, 0x14), nullptr); // 0040E8FD-0040E900
    put<Word>(self, 0x10, size);                               // 0040E908
    void* image = live_operator_new(size);                     // 0040E907-0040E90B
    put<void*>(self, 0x30, image);                             // 0040E917
    Word result = 0;                                           // 0040E933 (failure paths)
    if (image) {                                               // 0040E915-0040E91A
        const Word read = c2da_file_005e6960(get<void*>(self, 0x14), nullptr, image, size, 1u); // 0040E937-0040E93E
        // Read mismatch keeps the image allocation and +24==0 (0040E943-0040E948 -> 0040E91F).
        if (read == size) {
            put<Word>(self, 0x24, 1);                          // 0040E94C (before the file is dropped)
            result = 1;                                        // 0040E965
        }
    }
    void* file = get<void*>(self, 0x14);                       // 0040E91C / 0040E945
    if (file) destroy_file(file);                              // 0040E923-0040E92B / 0040E955-0040E95D
    put<void*>(self, 0x14, nullptr);                           // 0040E96E
    return result;
}
// 0040E980 ReadResourceAsync: mov eax,[ecx]; jmp [eax+34h] (synchronous ReadResource, same args).
extern "C" __declspec(noinline) void __fastcall image_file_read_async_0040e980(void* self, void*, Word key, void* buffer, Word size, Word unused) {
    (void)methods(self)->read_resource(self, nullptr, key, buffer, size, unused);
}
namespace {
// 32-byte RIM key record copied by value (rep movsd, ecx=8) at 0040F13C-0040F145 / 0040F17C-0040F185.
struct ImageRecord { unsigned char bytes[0x20]; };
ImageRecord image_record(void* self, Word key) {
    const unsigned char* image = bytes(self, 0x30);            // 0040F120
    ImageRecord record;
    std::memcpy(&record, image + get<Word>(image, 0x10) + ((key & 0x3fffu) << 5), sizeof(record)); // no bounds check
    return record;
}
}
// 0040F120 GetResourceSize(key) RET4: record.size (+1Ch).
extern "C" __declspec(noinline) Word __fastcall image_file_size_0040f120(void* self, void*, Word key) {
    const ImageRecord record = image_record(self, key);
    return get<Word>(record.bytes, 0x1c);                      // 0040F147
}
// 0040F160 GetResource(key) RET4: image + record.offset (+18h).
extern "C" __declspec(noinline) void* __fastcall image_file_get_resource_0040f160(void* self, void*, Word key) {
    const ImageRecord record = image_record(self, key);
    return static_cast<unsigned char*>(get<void*>(self, 0x30)) + get<Word>(record.bytes, 0x18); // 0040F187-0040F18C
}
// 0040F1A0 ReadResource(key, buffer, size, unused) RET10h. Observed quirk kept: the key table is
// indexed with stride 4 (lea eax,[ecx+eax*4] 0040F1C6) and the DWORD found there is
// dereferenced as a record pointer (0040F1CE-0040F1DA), unlike the 32-byte stride of
// 0040F120/0040F160. Copies min(record.size, size) bytes from image+record.offset; EAX = count.
extern "C" __declspec(noinline) Word __fastcall image_file_read_0040f1a0(void* self, void*, Word key, void* buffer, Word size, Word) {
    if (!get<Word>(self, 0x24) || !buffer) return 0;          // 0040F1A0-0040F1AE
    const Word index = key & 0x3fffu;                          // 0040F1BA
    const unsigned char* image = bytes(self, 0x30);            // 0040F1B4
    if (index >= get<Word>(image, 0x0c)) return 0;             // 0040F1B7-0040F1C1 (unsigned jae)
    const unsigned char* record = get<const unsigned char*>(image, get<Word>(image, 0x10) + index * 4u); // 0040F1C3-0040F1CE
    Word count = get<Word>(record, 0x1c);                      // 0040F1D1
    if (!(count < size)) count = size;                         // 0040F1D4-0040F1D8 (jb keeps record size)
    std::memcpy(buffer, image + get<Word>(record, 0x18), count); // 0040F1DA-0040F1ED
    return count;
}
// 0040F0A0 ~CExoResourceImageFile: [0]=73E240; CloseFile inline; [0]=73E128; ~CExoString(+4).
extern "C" __declspec(noinline) void __fastcall image_file_destroy_0040f0a0(void* self, void*) {
    set_methods(self, &image_file_methods_0073e240);           // 0040F0BD
    if (get<Word>(self, 0x24)) {                               // 0040F0C3-0040F0D0
        void* image = get<void*>(self, 0x30);
        if (image) { live_free(image); put<void*>(self, 0x30, nullptr); } // 0040F0D2-0040F0E2
        put<Word>(self, 0x24, 0);                              // 0040F0E9
    }
    set_methods(self, &packed_file_methods_0073e128);          // 0040F0FB
    name_of(self)->~StartupString();                           // 0040F101
}
// 00410500 scalar deleting ~CExoResourceImageFile (vslot 00; 0040F990 discard calls it with 1).
extern "C" __declspec(noinline) void* __fastcall image_file_deleting_destroy_00410500(void* self, void*, Word flags) {
    image_file_destroy_0040f0a0(self, nullptr);                // 00410503
    if (flags & 1u) live_free(self);                           // 00410508-00410510 (byte test)
    return self;                                               // 00410518
}

// ---------------------------------------------------------------------------
// Construction / destruction.
// 0040D560 ~CExoPackedFile: [0]=0073E128; ~CExoString(+4) (tail jmp 005E5C20).
extern "C" __declspec(noinline) void __fastcall packed_file_destroy_0040d560(void* self, void*) {
    set_methods(self, &packed_file_methods_0073e128);
    name_of(self)->~StartupString();
}
// 0040EE60 scalar deleting ~CExoPackedFile.
extern "C" __declspec(noinline) void* __fastcall packed_file_deleting_destroy_0040ee60(void* self, void*, Word flags) {
    packed_file_destroy_0040d560(self, nullptr);               // 0040EE63
    if (flags & 1u) live_free(self);                           // 0040EE68-0040EE70
    return self;
}
// 0040EE80 CExoResFile::CExoResFile (56 bytes). EH state 0 after the name string (funcinfo
// 00714B28 -> 00714B20 runs 0040D560); nothing after that point can throw.
void* __fastcall fb80_ports::Archive0040ee80(void* self, void*) {
    set_methods(self, &packed_file_methods_0073e128);          // 0040EEA4
    ::new (at(self, 4)) String();                              // 0040EEAA (005B3190)
    put<Word>(self, 0x24, 0); put<Word>(self, 0x2c, 0);        // 0040EEB1-0040EEB4
    set_methods(self, &res_file_methods_0073e1b0);             // 0040EEBD
    put<Word>(self, 0x1c, 0);                                  // 0040EEC3
    (void)WindowCStr005e5670(name_of(self), nullptr);          // 0040EEC6 (inlined Initialize)
    put<Word>(self, 0x14, 0); put<Word>(self, 0x18, 0); put<std::uint16_t>(self, 0x0c, 0);
    put<Word>(self, 0x10, 0); put<Word>(self, 0x1c, 0); put<Word>(self, 0x20, 0);
    put<Word>(self, 0x24, 0); put<Word>(self, 0x28, 0); put<Word>(self, 0x30, 0); put<Word>(self, 0x34, 0); // 0040EECF-0040EEEB
    return self;
}
// 0040EF00 ~CExoResFile: [0]=73E1B0; CloseFile inline (with CStr); [0]=73E128; ~CExoString(+4).
extern "C" __declspec(noinline) void __fastcall res_file_destroy_0040ef00(void* self, void*) {
    set_methods(self, &res_file_methods_0073e1b0);             // 0040EF1D
    if (get<Word>(self, 0x24)) {                               // 0040EF23-0040EF30
        (void)WindowCStr005e5670(name_of(self), nullptr);      // 0040EF33-0040EF36
        void* file = get<void*>(self, 0x14);
        if (file) { destroy_file(file); put<void*>(self, 0x14, nullptr); } // 0040EF3B-0040EF52
        put<Word>(self, 0x24, 0);                              // 0040EF59
    }
    set_methods(self, &packed_file_methods_0073e128);          // 0040EF6C
    name_of(self)->~StartupString();                           // 0040EF72
}
// 004104C0 scalar deleting ~CExoResFile (vslot 00).
extern "C" __declspec(noinline) void* __fastcall res_file_deleting_destroy_004104c0(void* self, void*, Word flags) {
    res_file_destroy_0040ef00(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}
// 0040EF90 CExoEncapsulatedFile::CExoEncapsulatedFile (68 bytes). EH state 0 after the name
// string: an exception from the CExoLocString ctor runs ~CExoPackedFile (00714B20 -> 0040D560).
void* __fastcall f3c0_ports::Archive0040ef90(void* self, void*) {
    set_methods(self, &packed_file_methods_0073e128);          // 0040EFB1
    ::new (at(self, 4)) String();                              // 0040EFB7 (005B3190)
    put<Word>(self, 0x24, 0); put<Word>(self, 0x2c, 0);        // 0040EFBE-0040EFC1
    set_methods(self, &encapsulated_file_methods_0073e1f8);    // 0040EFCB
    try {
        (void)exo_locstring_construct_005e9e10(at(self, 0x30), nullptr); // 0040EFD1
    } catch (...) {
        packed_file_destroy_0040d560(self, nullptr);
        throw;
    }
    put<Word>(self, 0x14, 0); put<Word>(self, 0x18, 0); put<std::uint16_t>(self, 0x0c, 0);
    put<Word>(self, 0x10, 0); put<Word>(self, 0x1c, 0); put<Word>(self, 0x20, 0);
    put<Word>(self, 0x24, 0); put<Word>(self, 0x28, 0); put<Word>(self, 0x38, 0); put<Word>(self, 0x3c, 0);
    put<unsigned char>(self, 0x40, 0);                         // 0040EFDA-0040EFF9
    return self;
}
// 0040F010 ~CExoEncapsulatedFile. Unwind states (1: ~CExoLocString, 0: ~CExoPackedFile,
// funcinfo 00714B53) only matter if ~CExoFile/free throw; their M40 transports are extern "C"
// (nothrow under /EHsc), so no handler is reproduced (open gate: EH parity).
extern "C" __declspec(noinline) void __fastcall encapsulated_destroy_0040f010(void* self, void*) {
    set_methods(self, &encapsulated_file_methods_0073e1f8);    // 0040F02D
    if (get<Word>(self, 0x24)) {                               // 0040F033-0040F040
        void* file = get<void*>(self, 0x14);
        if (file) { destroy_file(file); put<void*>(self, 0x14, nullptr); } // 0040F043-0040F05A
        put<Word>(self, 0x24, 0);                              // 0040F061
    }
    exo_locstring_destroy_005ea000(at(self, 0x30), nullptr);   // 0040F069-0040F071
    set_methods(self, &packed_file_methods_0073e128);          // 0040F081
    name_of(self)->~StartupString();                           // 0040F087
}
// 004104E0 scalar deleting ~CExoEncapsulatedFile (vslot 00).
extern "C" __declspec(noinline) void* __fastcall encapsulated_deleting_destroy_004104e0(void* self, void*, Word flags) {
    encapsulated_destroy_0040f010(self, nullptr);
    if (flags & 1u) live_free(self);
    return self;
}

// ---------------------------------------------------------------------------
// Method tables (exact slot order from the pinned PE .rdata; evidence/vtables.txt).
extern "C" const Methods packed_file_methods_0073e128{
    &packed_file_deleting_destroy_0040ee60, &pure_effect, &pure_effect, &pure_effect,
    &pure_effect, &pure_effect, &pure_effect, &packed_file_get_file_0040d570,
    &packed_file_get_async_file_00644760, &pure_sized, &pure_effect, &pure_effect,
    &pure_effect, &pure_transfer, &pure_async_transfer, &pure_load_header, &pure_effect};
extern "C" const Methods res_file_methods_0073e1b0{
    &res_file_deleting_destroy_004104c0, &res_file_add_ref_0040d580,
    &res_file_add_async_ref_0040d5a0, &res_file_close_0040d5c0,
    &res_file_close_async_0040db50, &res_file_delete_ref_0040d650,
    &res_file_delete_async_ref_0040d670, &packed_file_get_file_0040d570,
    &packed_file_get_async_file_00644760, &res_file_size_0040d690,
    &res_file_initialize_0040d6b0, &res_file_open_0040d6f0,
    &res_file_open_async_0040d800, &res_file_read_0040da20,
    &res_file_read_async_0040daa0, &res_file_load_header_0040d910,
    &res_file_unload_header_0040d600};
extern "C" const Methods encapsulated_file_methods_0073e1f8{
    &encapsulated_deleting_destroy_004104e0, &res_file_add_ref_0040d580,
    &res_file_add_async_ref_0040d5a0, &encapsulated_close_0040db10,
    &res_file_close_async_0040db50, &res_file_delete_ref_0040d650,
    &res_file_delete_async_ref_0040d670, &packed_file_get_file_0040d570,
    &packed_file_get_async_file_00644760, &encapsulated_size_0040dbe0,
    &encapsulated_initialize_0040dc00, &encapsulated_open_0040dc30,
    &encapsulated_open_async_0040df10, &encapsulated_read_0040e640,
    &encapsulated_read_async_0040e6b0, &encapsulated_load_header_0040e1f0,
    &encapsulated_unload_header_0040db90};
extern "C" const ImageMethods image_file_methods_0073e240{
    {&image_file_deleting_destroy_00410500, &res_file_add_ref_0040d580,
     &image_void_slot /* 004015A0 */, &image_file_close_0040e720,
     &image_true_slot /* 0063E7A0 */, &image_file_delete_ref_0040e750,
     &image_void_slot /* 004015A0 */, &packed_file_get_file_0040d570,
     &packed_file_get_async_file_00644760, &image_file_size_0040f120,
     &image_file_initialize_0040e770, &image_file_open_0040e790,
     &image_true_slot /* 0063E7A0 */, &image_file_read_0040f1a0,
     &image_file_read_async_0040e980, &packed_return_false_0063e7c0,
     &image_true_slot /* 0063E7A0 */},
    &image_file_get_resource_0040f160};

// Consumer table-address producers (f990_ports): immutable addresses, no copies.
const void* __cdecl f990_ports::InitialTable0073e128() noexcept { return &packed_file_methods_0073e128; }
const void* __cdecl f990_ports::FinalTable0073e240() noexcept { return &image_file_methods_0073e240; }
