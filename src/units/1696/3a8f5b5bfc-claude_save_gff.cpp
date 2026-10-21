// claude_save_gff.cpp (lane worker save-gff): CResGFF writer internals, the CERFFile resource writer, resource-manager
// key-table helpers, CExoString / CExoLocString compare helpers and the game-heap _calloc used by
// CServerExoAppInternal::StoreCurrentModule 004B2E70 and its module / GIT savers. Transcribed from the original
// swkotor.exe (read-only). Exports (thiscall -> extern "C" __fastcall(self, unused_edx, stack args...)):
//   00410AA0 CResGFF::InitializeForWriting()                       RET 0    kotor_fastcall_00410aa0  [wraps 0867 body]
//   00410D60 CResGFF::AddStruct(ulong type)                        RET 4    kotor_fastcall_00410d60  [wraps 0870 body]
//   00410E20 CResGFF::AddLabel(char const*)                        RET 4    kotor_fastcall_00410e20  [wraps 0868 body]
//   00411020 CResGFF::AddDataLayoutField(offset, old, new)         RET 0Ch  kotor_fastcall_00411020  [wraps 0868 section 1]
//   004110F0 CResGFF::AddDataLayoutList(offset, old, new)          RET 0Ch  kotor_fastcall_004110f0  [wraps 0868 section 2]
//   00411730 CResGFF::AddField(CResStruct*, char const*, ulong)    RET 0Ch  kotor_fastcall_00411730  [wraps 0868 body]
//   004113D0 CResGFF::WriteGFFData(CExoFile*, ulong&)              RET 8    kotor_fastcall_004113d0  [wraps 0871 body]
//   00411540 CResGFF::GetTotalSize()                               RET 0    kotor_fastcall_00411540  [binds 0290 q05 body]
//   00412CA0 CResGFF::WriteFieldQuaternion(CResStruct*, Quaternion const&, char const*) RET 0Ch kotor_fastcall_00412ca0
//   00412DB0 CResGFF::Pack(uchar percent, ulong threshold)         RET 8    kotor_fastcall_00412db0  [wraps 0871 body]
//   005DDF30 CERFFile::WriteResource(char const*, ushort, CRes*, int is_gff, int) RET 14h kotor_fastcall_005ddf30
//   005DD1E0 CERFRes::Write(CExoFile&, ulong entry_offset)         RET 8    kotor_fastcall_005dd1e0
//   00408C60 CExoResMan::GetResOfType(ushort, CRes*)               RET 8    kotor_fastcall_00408c60
//   0040ED20 CExoKeyTable::GetKeysOfType(ushort)                   RET 4    kotor_fastcall_0040ed20
//   005E9550 CExoStringList::CExoStringList(a1, a2, capacity)      RET 0Ch  kotor_fastcall_005e9550
//   00675DD0 GetProperty0x4 ([ecx+4])                              RET 0    kotor_fastcall_00675dd0
//   004088E0 CExoResMan::UpdateDirectoryKeyTable(CExoString const&) RET 4   kotor_fastcall_004088e0
//   00407FE0 CExoResMan::UpdateKeyTable(CExoString const&, int)    RET 8    kotor_fastcall_00407fe0
//   005E5950 CExoString::StripNonAlphaNumeric(int, int, int)       RET 0Ch  kotor_fastcall_005e5950
//   005EA020 CExoLocString::Compare (jmp 005E9F10)                 RET 4    kotor_fastcall_005ea020
//   005E9F10 CExoLocString::Compare(CExoLocString const&)          RET 4    kotor_fastcall_005e9f10
//   005EC7F0 CExoLocStringInternal::Compare(CExoLocString const&)  RET 4    kotor_fastcall_005ec7f0
//   005E9EF0 CExoLocString::GetStringCount()                       RET 0    kotor_fastcall_005e9ef0
//   005E5310 CExoString::operator!=(CExoString const&)             RET 4    kotor_fastcall_005e5310
//   006FB823 _calloc (cdecl, game heap)                                     kotor_cdecl_006fb823
// Reuse gate: the CResGFF writer bodies already exist as the Memory/Ports model in kotor::server_mainloop
// (objects 0867-gff_write_storage, 0868-gff_write_fields, 0870/0873 gff_write_core, 0871-gff_write_output, all in the
// c14 map). They are re-derived against the original here (same field offsets, growth/capacity order, strncpy label
// semantics, rep movsd/movsb copies, waste counters) and bound through the same native Memory/Ports adapter pattern
// as 0873 gff_writer_native and w2_save_gff_native: raw memory, game heap (live_operator_new / live_free),
// gff_resource_release_004108c0, CExoFile::Write 005E69A0. No trapping port: every port of the adapters is bound
// to its real provider.
// Conventions (W5): live_operator_new / live_free only; no original address is dereferenced.
#include "78171d00fd-gff_write_core.hpp"
#include "02d6b1a3de-gff_write_fields.hpp"
#include "9bd7dc65af-gff_write_output.hpp"
#include <cstdint>
#include <cstring>

namespace kotor_reconstruction { struct StartupString; }
namespace kotor_sound_internal {   // [0675-sound-providers / 0671-sound-internal] CExoStringList members
void __fastcall StringListAdd005e98b0(void* list, void*, kotor_reconstruction::StartupString* string);   // 005E98B0 Add RET 4
void __fastcall StringListDestroy005e9860(void* list, void*);                                           // 005E9860 dtor RET 0
}
namespace kotor_menu20 { struct String; String* __fastcall ConstructString005b3190(String*, void*); }  // 005B3190 CExoString() [menu20_bindings]
namespace add_key_lower { void __fastcall Rebuild00410260(void* table, void*); }                    // 00410260 CExoKeyTable::RebuildTable [0280]
namespace kotor_q05 { struct CResGFF { __declspec(noinline) std::uint32_t GetTotalSize(); }; }      // 00411540 body [0290-gff-views]

// Game CRT heap state (0004-live-policy): same layout as allocator_live.h (HANDLE = void*, NewHandler = int(*)(U)).
struct LiveAllocatorGlobals {
    void* volatile* heap;                                            // [00834338] _crtheap
    std::uint32_t volatile *mode, *threshold, *new_mode;             // [0083433C] __active_heap, [00834328] __sbh_threshold, [00833ECC] _newmode
    int(__cdecl* volatile* handler)(std::uint32_t);
    std::uint32_t volatile *header_base, *header_count;
};
struct LiveAllocatorDependencies {
    void*(__stdcall* heap_alloc)(void*, unsigned long, unsigned long);   // HeapAlloc (IAT 0073D14C)
    int(__stdcall* heap_free)(void*, unsigned long, void*);
    void(__cdecl* lock)(int);                                            // 00701A74 _lock
    void(__cdecl* unlock)(int);                                          // 007019E0 _unlock
    void*(__cdecl* sbh_alloc)(std::uint32_t);                            // 00703310 __sbh_alloc_block
    void(__cdecl* sbh_free)(void*, void*);
};
extern LiveAllocatorGlobals live_globals;
extern LiveAllocatorDependencies live_dependencies;

namespace save_gff {
using U = std::uint32_t;
using U16 = std::uint16_t;
using I = std::int32_t;
using kotor::server_mainloop::Word;

extern "C" {
// ---------------- imports: existing providers (c14 candidate map; owner object in brackets) ----------------
void* __cdecl live_operator_new(std::uint32_t);                                              // 006FA7E6 [0004-live-policy]
void __cdecl live_free(void*);                                                               // 006FA390 [0004-live-policy]
int __cdecl live_callnewh(std::uint32_t);                                                    // 0070360C _callnewh [0004-live-policy]
void __fastcall gff_resource_release_004108c0(void* gff, void*);                             // CResGFF::ReleaseResource [0289]
void* __fastcall recovered_gff_00410f50(void* gff, void*, U offset, U old_size, U new_size); // CResGFF::AddDataField [w2_save_gff_native]
U __fastcall recovered_file_write_005e69a0(void* file, void*, const void* data, U size, U count); // CExoFile::Write [server_character_ingress]
U __fastcall exofile_seek_005e6aa0(void* file, void*, U offset, U origin);                   // CExoFile::Seek [0232]
void __fastcall frame_LogStringCtor_005e5a90(void* string, void*, const char* text);         // CExoString(char const*) [0459]
void __fastcall frame_LogStringDtor_005e5c20(void* string, void*);                           // ~CExoString [0459]
U __fastcall async_string_length_005e5790(void* string, void*);                              // CExoString::GetLength [0188]
const char* __fastcall async_string_cstr_005e5670(void* string, void*);                      // CExoString::CStr [0188]
void* __fastcall async_assign_string_005e5c50(void* string, void*, void* other);             // CExoString::operator= [0199]
U __fastcall exostring_compare_no_case_005e6450(void* string, void*, const void* other);     // CExoString::CompareNoCase [0286]
void __fastcall frame_LogFileCtor_005e68d0(void* file, void*, void* name, U type, void* mode); // CExoFile::CExoFile [0459]
void __fastcall frame_LogFileDtor_005e69e0(void* file, void*);                               // CExoFile::~CExoFile [0459]
U __fastcall frame_LogFileOpened_005e6a10(void* file, void*);                                // CExoFile::FileOpened [0459]
void* __fastcall unresolved_list_at_pos_005e9b60(void* list, void*, void* position);         // CExoLinkedList::GetAtPos [0168]
void* __fastcall unresolved_list_next_005e9b90(void* list, void*, void** position);          // CExoLinkedList::GetNext [0168]
void* __fastcall c2da_manager_004076e0(void* resman, void*, void* res);                      // CExoResMan::GetTable(CRes*) [0192]
void __fastcall gui_load_resref_copy_to_string_00405fb0(const void* resref, void*, char* out); // CResRef::CopyToString [0292]
U __fastcall packed_file_get_async_file_00644760(void* self, void*);                         // 00644760 GetProperty0x18 = CRes size [0268]
void* __fastcall gui_load_slot_prop10_004064a0(void* self, void*);                           // 004064A0 GetProperty0x10 = CRes data [0296]
U __fastcall kotor_fastcall_005ea050(void* loc, void*, U language, void* out, U gender);     // CExoLocString::GetString [claude_dialogue_server]
// srcw5 siblings (claude_w1_chains_erf.cpp, the CERF writer set):
U __fastcall kotor_fastcall_005e6940(void* file, void*);                                     // CExoFile::GetOffset
U __fastcall kotor_fastcall_005e6af0(void* file, void*);                                     // CExoFile::SeekEnd
U __fastcall kotor_fastcall_005dd120(void* key, void*, void* file);                          // CERFKey::Write

// ---------------- exports of this file ----------------
void __fastcall kotor_fastcall_00410aa0(void* self, void*);
U __fastcall kotor_fastcall_00410d60(void* self, void*, U type);
U __fastcall kotor_fastcall_00410e20(void* self, void*, const char* label);
void* __fastcall kotor_fastcall_00411020(void* self, void*, U offset, U old_size, U new_size);
void* __fastcall kotor_fastcall_004110f0(void* self, void*, U offset, U old_size, U new_size);
void* __fastcall kotor_fastcall_00411730(void* self, void*, void* root, const char* label, U type);
U __fastcall kotor_fastcall_004113d0(void* self, void*, void* file, U* size);
U __fastcall kotor_fastcall_00411540(void* self, void*);
U __fastcall kotor_fastcall_00412ca0(void* self, void*, void* root, const void* quaternion, const char* label);
void __fastcall kotor_fastcall_00412db0(void* self, void*, U percent, U threshold);
U __fastcall kotor_fastcall_005ddf30(void* self, void*, const char* name, U type, void* res, U is_gff, U unused);
U __fastcall kotor_fastcall_005dd1e0(void* self, void*, void* file, U entry_offset);
void* __fastcall kotor_fastcall_00408c60(void* self, void*, U type, void* res);
void* __fastcall kotor_fastcall_0040ed20(void* self, void*, U type);
void* __fastcall kotor_fastcall_005e9550(void* self, void*, U a1, U a2, U capacity);
U __fastcall kotor_fastcall_00675dd0(void* self, void*);
U __fastcall kotor_fastcall_004088e0(void* self, void*, void* name);
U __fastcall kotor_fastcall_00407fe0(void* self, void*, void* name, U table_type);
U __fastcall kotor_fastcall_005e5950(void* self, void*, U lowercase, U allow_at_dot, U allow_extended);
U __fastcall kotor_fastcall_005ea020(void* self, void*, void* other);
U __fastcall kotor_fastcall_005e9f10(void* self, void*, void* other);
U __fastcall kotor_fastcall_005ec7f0(void* self, void*, void* other);
U __fastcall kotor_fastcall_005e9ef0(void* self, void*);
U __fastcall kotor_fastcall_005e5310(void* self, void*, void* other);
void* __cdecl kotor_cdecl_006fb823(U count, U size);
}

namespace {
template <class T> inline T& at(void* p, U off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off); }
template <class T> inline T at(const void* p, U off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(T)); return v; }
inline void* ptr(Word v) { return reinterpret_cast<void*>(static_cast<std::uintptr_t>(v)); }
inline Word word(const void* p) { return static_cast<Word>(reinterpret_cast<std::uintptr_t>(p)); }

const char k_wb_0073d8a4[] = "wb";                                                           // .rdata 0073D8A4
// 005E5950 character sets (.rdata, copied with rep movsd/movsw/movsb into the 80h-byte stack buffer).
const char k_alnum_0074db3c[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";   // 3Fh bytes
const char k_lower_0074db7c[] = "abcdefghijklmnopqrstuvwxyz0123456789'"
    "\xdf\xe9\xc9\xe8\xc8\xc7\xe7\xf9\xd9\xf1\xd1\xe0\xc0\xc4\xe4\xdc\xfc\xd6\xf6_-";          // 3Bh bytes
const char k_at_dot_0074db38[] = "@.";                                                       // 3 bytes
const char k_extended_0074db1c[] = ".',"
    "\xdf\xe9\xc9\xe8\xc8\xc7\xe7\xf9\xd9\xf1\xd1\xe0\xc0\xc4\xe4\xdc\xfc\xd6\xf6_- ";          // 1Ah bytes
static_assert(sizeof(k_alnum_0074db3c) == 0x3f && sizeof(k_lower_0074db7c) == 0x3b, "005E5950 copy lengths");
static_assert(sizeof(k_at_dot_0074db38) == 3 && sizeof(k_extended_0074db1c) == 0x1a, "005E5950 append lengths");

// ---- native Memory / Ports adapters for the kotor::server_mainloop writer bodies ----
struct M final : kotor::server_mainloop::Memory {
    Word read(Word a) override { Word v; std::memcpy(&v, ptr(a), 4); return v; }
    Word read8(Word a) override { return *static_cast<unsigned char*>(ptr(a)); }
    Word read16(Word a) override { unsigned short v; std::memcpy(&v, ptr(a), 2); return v; }
    void write(Word a, Word v) override { std::memcpy(ptr(a), &v, 4); }
    void write16(Word a, Word v) override { const auto s = static_cast<unsigned short>(v); std::memcpy(ptr(a), &s, 2); }
};
// GffStoragePorts: 006FA7E6 operator new, 006FA390 free, 004108C0 ReleaseResource, byte stores of rep movsb.
struct S final : kotor::server_mainloop::GffStoragePorts {
    Word allocate(Word a) override { return word(live_operator_new(a)); }
    void free(Word a) override { live_free(ptr(a)); }
    void release_004108c0(Word a) override { gff_resource_release_004108c0(ptr(a), nullptr); }
    void byte(Word a, Word b) override { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(b); }
};
// GffOutputPorts: WriteGFFData only reaches write (005E69A0 CExoFile::Write, ECX = the CExoFile*). The file
// ports are bound to the real CExoFile / CExoString providers as in 0873 gff_writer_native (WriteGFFFile shape).
struct O final : kotor::server_mainloop::GffOutputPorts {
    Word allocate(Word a) override { return word(live_operator_new(a)); }
    void free(Word a) override { live_free(ptr(a)); }
    void release_004108c0(Word a) override { gff_resource_release_004108c0(ptr(a), nullptr); }
    void byte(Word a, Word b) override { *static_cast<unsigned char*>(ptr(a)) = static_cast<unsigned char>(b); }
    Word write(Word file, Word data, Word size, Word count) override {
        return recovered_file_write_005e69a0(ptr(file), nullptr, ptr(data), size, count);
    }
    void string_literal(Word a, Word) override { frame_LogStringCtor_005e5a90(ptr(a), nullptr, k_wb_0073d8a4); } // only 0073D8A4 "wb" is passed
    void string_destroy(Word a) override { frame_LogStringDtor_005e5c20(ptr(a), nullptr); }
    void file_construct(Word a, Word b, Word c, Word d) override { frame_LogFileCtor_005e68d0(ptr(a), nullptr, ptr(b), c, ptr(d)); }
    Word file_open(Word a) override { return frame_LogFileOpened_005e6a10(ptr(a), nullptr); }
    void file_destroy(Word a) override { frame_LogFileDtor_005e69e0(ptr(a), nullptr); }
};

// CExoString temporaries owned by a stack slot (EH unwind = ~CExoString).
struct StringGuard { void* s; ~StringGuard() { frame_LogStringDtor_005e5c20(s, nullptr); } };
}  // namespace

extern "C" {
// =========================================================================================================
// CResGFF writer internals (bodies: kotor::server_mainloop, re-derived against the original below)
// =========================================================================================================

// 00410AA0 CResGFF::InitializeForWriting: new 38h header (copied from the current one when present, else zero),
// six section buffers sized from the capacities (+48h/+50h/+58h/+60h/+68h/+74h; defaults 6Eh/286h/62h/72Ch/FD4h/FD4h,
// otherwise count + growth), old contents copied by count*stride; a serviced resource is released (+98h) instead of
// the seven frees. RET 0. [0867 gff_owned_storage_00410aa0: 00410AA0..00410D5F]
void __fastcall kotor_fastcall_00410aa0(void* self, void*) {
    M m; S p;
    kotor::server_mainloop::gff_owned_storage_00410aa0(m, p, word(self));
}

// 00410D60 CResGFF::AddStruct(ulong type): appends {type, FFFFFFFFh, 0} to the struct array (+44h, count hdr+0Ch,
// capacity +48h, growth +28h doubled); FFFFFFFFh without header/array. RET 4. [0870 gff_add_struct_00410d60]
U __fastcall kotor_fastcall_00410d60(void* self, void*, U type) {
    M m; S p;
    return kotor::server_mainloop::gff_add_struct_00410d60(m, p, word(self), type);
}

// 00410E20 CResGFF::AddLabel(char const*): strncpy(label, 16) into a zeroed 16-byte key, reverse search of the label
// array (+54h, count hdr+1Ch) for an equal key, else append (capacity +58h, growth +30h doubled). Returns the index,
// FFFFFFFFh without header/array. RET 4. [0868 gff_add_label_00410e20]
U __fastcall kotor_fastcall_00410e20(void* self, void*, const char* label) {
    M m; S p;
    return kotor::server_mainloop::gff_add_label_00410e20(m, p, word(self), word(label));
}

// 00411020 CResGFF::AddDataLayoutField(ulong offset, ulong old_size, ulong new_size): field-index buffer (+64h, used
// hdr+2Ch, capacity +68h, growth +38h doubled, waste +6Ch). In place when offset+old_size is the end; otherwise the
// old bytes are moved to the end and counted as waste. Returns +64h + base, 0 without header/buffer. RET 0Ch.
// [0868 gff_grow_buffer section 1: 00411020..004110ED]
void* __fastcall kotor_fastcall_00411020(void* self, void*, U offset, U old_size, U new_size) {
    M m; S p;
    return ptr(kotor::server_mainloop::gff_grow_buffer(m, p, word(self), offset, old_size, new_size, 1));
}

// 004110F0 CResGFF::AddDataLayoutList(offset, old_size, new_size): the same shape on the list-index buffer (+70h,
// used hdr+34h, capacity +74h, growth +3Ch doubled, waste +78h). RET 0Ch. [0868 gff_grow_buffer section 2]
void* __fastcall kotor_fastcall_004110f0(void* self, void*, U offset, U old_size, U new_size) {
    M m; S p;
    return ptr(kotor::server_mainloop::gff_grow_buffer(m, p, word(self), offset, old_size, new_size, 2));
}

// 00411730 CResGFF::AddField(CResStruct* root, char const* label, ulong type): appends a field record (+4Ch, count
// hdr+14h, capacity +50h, growth +2Ch doubled) to struct [root]; the struct's first field is stored inline, the second
// moves both into AddDataLayoutField(-1, 0, 8), later ones grow (+4, n*4, n*4+4). Record = {type, AddLabel(label),
// FFFFFFFFh}; returns its address, 0 on any failure. RET 0Ch. [0868 gff_add_field_00411730]
void* __fastcall kotor_fastcall_00411730(void* self, void*, void* root, const char* label, U type) {
    M m; S p;
    return ptr(kotor::server_mainloop::gff_add_field_00411730(m, p, word(self), word(root), word(label), type));
}

// 004113D0 CResGFF::WriteGFFData(CExoFile* file, ulong& size): lays out the six section offsets in the header
// (hdr+08h.. = 38h, then += count*stride for 12/12/16/1/1/1), writes the 38h header and each section with
// CExoFile::Write(data, size, 1); 1 only when every write returned its size. RET 8. [0871 gff_write_stream_004113d0]
U __fastcall kotor_fastcall_004113d0(void* self, void*, void* file, U* size) {
    M m; O p;
    return kotor::server_mainloop::gff_write_stream_004113d0(m, p, word(self), word(file), word(size));
}

// 00411540 CResGFF::GetTotalSize: header ? (structs+fields)*12 + labels*16 + list + indices + data + 38h : 0. RET 0.
// [0290 kotor_q05::CResGFF::GetTotalSize, same layout: header pointer at +40h, counts hdr+0Ch/14h/1Ch/24h/2Ch/34h]
U __fastcall kotor_fastcall_00411540(void* self, void*) {
    return reinterpret_cast<kotor_q05::CResGFF*>(self)->GetTotalSize();                       // 00411540..00411569
}

// 00412DB0 CResGFF::Pack(uchar percent, ulong threshold): when field-index waste (+6Ch) or list-index waste (+78h)
// exceeds threshold and (waste*100)/GetTotalSize() > percent, compacts each buffer (invalid ranges become
// FFFFFFFFh / 0) and clears its waste counter. RET 8. [0871 gff_pack_00412db0; the inline total is GetTotalSize]
void __fastcall kotor_fastcall_00412db0(void* self, void*, U percent, U threshold) {
    M m; S p;
    kotor::server_mainloop::gff_pack_00412db0(m, p, word(self), percent & 0xffu, threshold);  // movzx byte [esp+1Ch]
}

// 00412CA0 CResGFF::WriteFieldQuaternion(CResStruct* root, Quaternion const& q, char const* label): field type 10h
// with a 16-byte data block. RET 0Ch, EAX = 1 / 0.
U __fastcall kotor_fastcall_00412ca0(void* self, void*, void* root, const void* quaternion, const char* label) {
    if (root == nullptr || label == nullptr) return 0;                                         // 00412CA4..00412CB6, 00412D24
    if (at<U>(self, 0x98) != 0) kotor_fastcall_00410aa0(self, nullptr);                       // 00412CB8..00412CC2
    void* field = kotor_fastcall_00411730(self, nullptr, root, label, 0x10);                   // 00412CC7..00412CD6
    if (field == nullptr) return 0;
    void* data = recovered_gff_00410f50(self, nullptr, 0xffffffffu, 0, 0x10);                  // 00412CD8..00412CE7
    if (data == nullptr) return 0;
    const U q0 = at<U>(quaternion, 0), q1 = at<U>(quaternion, 4);                              // 00412CEA..00412CF1
    const U q2 = at<U>(quaternion, 8), q3 = at<U>(quaternion, 0xc);                            // 00412CF4..00412CFA
    at<U>(field, 8) = static_cast<U>(static_cast<unsigned char*>(data) - at<unsigned char*>(self, 0x5c)); // 00412CFE..00412D03
    at<U>(data, 0) = q0;                                                                       // 00412D06
    at<U>(data, 4) = q1;                                                                       // 00412D0D
    at<U>(data, 8) = q2;                                                                       // 00412D11
    at<U>(data, 0xc) = q3;                                                                     // 00412D15
    return 1;                                                                                  // 00412D18
}

// =========================================================================================================
// CERF writer: resource from memory (CRes data or a packed CResGFF)
// =========================================================================================================

// 005DD1E0 CERFRes::Write(CExoFile& file, ulong entry_offset): {+0 offset, +4 size, +8 data}; offset = GetOffset,
// data written, then (offset, size) at entry_offset and SeekEnd. RET 8, EAX = 1.
U __fastcall kotor_fastcall_005dd1e0(void* self, void*, void* file, U entry_offset) {
    at<U>(self, 0) = kotor_fastcall_005e6940(file, nullptr);                                   // 005DD1E9..005DD1F3
    (void)recovered_file_write_005e69a0(file, nullptr, at<void*>(self, 8), at<U>(self, 4), 1); // 005DD1F0..005DD201
    (void)exofile_seek_005e6aa0(file, nullptr, entry_offset, 0);                               // 005DD206..005DD20F
    (void)recovered_file_write_005e69a0(file, nullptr, self, 4, 1);                            // 005DD214..005DD21B
    (void)recovered_file_write_005e69a0(file, nullptr, static_cast<unsigned char*>(self) + 4, 4, 1); // 005DD220..005DD227
    (void)kotor_fastcall_005e6af0(file, nullptr);                                              // 005DD22C..005DD22E
    return 1;                                                                                  // 005DD235
}

// 005DDF30 CERFFile::WriteResource(char const* name, ushort type, CRes* res, int is_gff, int): appends one resource
// at the end of the file (+ACh), stores (offset, size) in its resource entry (+B8h + id*8) and its key (resref =
// GetLength(name) bytes of name, unbounded; id = +C0h; type) at +B4h + id*18h, then ++id. A GFF is packed
// (Pack(0, 0)) and written with WriteGFFData; GetTotalSize is called and its result discarded (original). Any other
// CRes is copied (GetSize bytes of GetData) into a CERFRes and written by CERFRes::Write; its buffer is freed at the
// end. Fifth argument unused. RET 14h, EAX = 1, or 0 when the entry count is reached or no file is open.
U __fastcall kotor_fastcall_005ddf30(void* self, void*, const char* name, U type, void* res, U is_gff, U) {
    struct ErfRes { U offset; U size; void* data; } erf_res = {0, 0, nullptr};                 // [esp+18h] 005DDF74..005DDF7C
    struct ErfKey { unsigned char resref[16]; U id; U16 type; U16 unused; } key = {{}, 0, 0xffff, 0}; // [esp+24h] 005DDF48..005DDF70
    static_assert(sizeof(ErfKey) == 0x18, "CERFKey record is 18h bytes");
    // EH state 1: CERFRes::~CERFRes 005DD990 (frees +8 when set); state 0: CERFKey::~CERFKey 005DD970 (zeroes the key).
    struct ResGuard {
        ErfRes& r; bool armed = true;
        ~ResGuard() { if (armed && r.data != nullptr) { live_free(r.data); r.data = nullptr; r.offset = 0; r.size = 0; } }
    } res_guard{erf_res};
    if (at<U>(self, 0xc0) >= at<U>(self, 0xbc) || at<void*>(self, 0xac) == nullptr) {          // 005DDF80..005DDF9D
        res_guard.armed = false;
        return 0;                                                                              // 005DE184..005DE196
    }
    alignas(4) unsigned char name_string[8];                                                   // [esp+10h]
    frame_LogStringCtor_005e5a90(name_string, nullptr, name);                                  // 005DDFA3..005DDFAE
    {
        StringGuard name_guard{name_string};                                                   // EH state 2: ~CExoString 005E5C20
        std::memset(key.resref, 0, 16);                                                        // 005DDFB3..005DDFCA
        U length = async_string_length_005e5790(name_string, nullptr);                         // 005DDFC1..005DDFD7 [esp+4Ch]
        std::memcpy(key.resref, async_string_cstr_005e5670(name_string, nullptr), length);     // 005DDFD3..005DDFFE (rep movs, unbounded)
        key.id = at<U>(self, 0xc0);                                                            // 005DDFE8, 005DE005
        key.type = static_cast<U16>(type);                                                     // 005DE000..005DE00D
        void* file = at<void*>(self, 0xac);
        if (is_gff != 0) {                                                                     // 005DE009..005DE012
            U offset = kotor_fastcall_005e6940(at<void*>(self, 0xac), nullptr);                // 005DE014..005DE027 [arg2 slot]
            kotor_fastcall_00412db0(res, nullptr, 0, 0);                                       // 005DE01F..005DE02B
            (void)kotor_fastcall_00411540(res, nullptr);                                       // 005DE030..005DE032 (result unused)
            (void)kotor_fastcall_004113d0(res, nullptr, at<void*>(self, 0xac), &length);       // 005DE037..005DE045 [arg1 slot]
            (void)exofile_seek_005e6aa0(at<void*>(self, 0xac), nullptr,
                                        at<U>(self, 0xb8) + at<U>(self, 0xc0) * 8, 0);         // 005DE04A..005DE061
            (void)recovered_file_write_005e69a0(at<void*>(self, 0xac), nullptr, &offset, 4, 1); // 005DE066..005DE075
            (void)recovered_file_write_005e69a0(at<void*>(self, 0xac), nullptr, &length, 4, 1); // 005DE07A..005DE089
        } else {
            erf_res.size = packed_file_get_async_file_00644760(res, nullptr);                  // 005DE090..005DE09D
            void* buffer = live_operator_new(packed_file_get_async_file_00644760(res, nullptr)); // 005DE09B..005DE0B1
            const U size = packed_file_get_async_file_00644760(res, nullptr);                  // 005DE0AF..005DE0BA [arg2 slot]
            std::memcpy(buffer, gui_load_slot_prop10_004064a0(res, nullptr), size);            // 005DE0B8..005DE0D7
            erf_res.data = buffer;                                                             // 005DE0F4
            (void)kotor_fastcall_005dd1e0(&erf_res, nullptr, file,
                                          at<U>(self, 0xb8) + at<U>(self, 0xc0) * 8);          // 005DE0D9..005DE0F8
        }
        (void)exofile_seek_005e6aa0(at<void*>(self, 0xac), nullptr,
                                    at<U>(self, 0xb4) + at<U>(self, 0xc0) * 0x18, 0);          // 005DE0FF..005DE119
        (void)kotor_fastcall_005dd120(&key, nullptr, at<void*>(self, 0xac));                   // 005DE11E..005DE129
        (void)kotor_fastcall_005e6af0(at<void*>(self, 0xac), nullptr);                         // 005DE12E..005DE134
        at<U>(self, 0xc0) = at<U>(self, 0xc0) + 1;                                             // 005DE139..005DE144
    }                                                                                          // 005DE14A..005DE14F ~CExoString
    res_guard.armed = false;
    if (erf_res.data != nullptr) live_free(erf_res.data);                                      // 005DE154..005DE169 (inline ~CERFRes)
    return 1;                                                                                  // 005DE16D
}

// =========================================================================================================
// CExoResMan / CExoKeyTable / CExoStringList
// =========================================================================================================

// 006FB823 _calloc on the game CRT heap: n*size (0 -> 1); with the small-block heap active (mode 3) the request is
// rounded to 16 and a request whose UNROUNDED size is <= the threshold goes to __sbh_alloc_block(n*size) under
// _lock(4), zero-filled with memset(n*size); otherwise HeapAlloc(_crtheap, HEAP_ZERO_MEMORY, request). Requests
// above FFFFFFE0h skip allocation. A null result retries while _newmode is set and _callnewh(request) succeeds.
void* __cdecl kotor_cdecl_006fb823(U count, U size) {
    const U total = count * size;                                                              // 006FB82F..006FB836 [ebp-1Ch]
    U request = total != 0 ? total : 1;                                                        // 006FB839..006FB83D
    for (;;) {
        void* block = nullptr;                                                                 // 006FB83E..006FB840
        if (request <= 0xffffffe0u) {                                                          // 006FB843..006FB846
            if (*live_globals.mode == 3) {                                                     // 006FB848..006FB84F
                request = (request + 0xf) & 0xfffffff0u;                                       // 006FB851..006FB857
                if (total <= *live_globals.threshold) {                                        // 006FB85A..006FB863
                    live_dependencies.lock(4);                                                 // 006FB865..006FB86C
                    block = live_dependencies.sbh_alloc(total);                                // 006FB870..006FB877
                    live_dependencies.unlock(4);                                               // 006FB87E (006FB8CD)
                    if (block != nullptr) {                                                    // 006FB883..006FB888
                        std::memset(block, 0, total);                                          // 006FB88A..006FB895
                        return block;                                                          // 006FB898..006FB89A
                    }
                }
            }
            block = live_dependencies.heap_alloc(*live_globals.heap, 8, request);              // 006FB89C..006FB8AB
        }
        if (block != nullptr || *live_globals.new_mode == 0) return block;                     // 006FB8AD..006FB8B7
        if (live_callnewh(request) == 0) return nullptr;                                       // 006FB8B9..006FB8C8
    }
}

// 005E9550 CExoStringList::CExoStringList(a1, a2, capacity): +0 = calloc(capacity, 4), +4 count = 0, +8 = a2,
// +0Ch = a1, +10h = capacity. RET 0Ch, EAX = this.
void* __fastcall kotor_fastcall_005e9550(void* self, void*, U a1, U a2, U capacity) {
    at<void*>(self, 0) = kotor_cdecl_006fb823(capacity, 4);                                    // 005E9552..005E9564
    at<U>(self, 0x10) = capacity;                                                              // 005E956D
    at<U>(self, 8) = a2;                                                                       // 005E9570
    at<U>(self, 4) = 0;                                                                        // 005E9574
    at<U>(self, 0xc) = a1;                                                                     // 005E957B
    return self;                                                                               // 005E957E
}

// 00675DD0 GetProperty0x4: EAX = [ECX+4] (here CExoStringList::GetCount). RET 0.
U __fastcall kotor_fastcall_00675dd0(void* self, void*) {
    return at<U>(self, 4);                                                                     // 00675DD0
}

// 0040ED20 CExoKeyTable::GetKeysOfType(ushort type): a new CExoStringList(0, 1, 5) with a new CExoString of every
// key (1Ch-byte entries at +10h, +0Ch of them) whose type word (+1Ah) matches; 0 for type FFFFh, an empty table or
// no match (the list is then destroyed). A failed list allocation leaves Add running on null (original). RET 4.
void* __fastcall kotor_fastcall_0040ed20(void* self, void*, U type) {
    if (static_cast<U16>(type) == 0xffff) return nullptr;                                      // 0040ED38..0040ED55
    if (at<U>(self, 0xc) == 0) return nullptr;                                                 // 0040ED58..0040ED74
    void* memory = live_operator_new(0x14);                                                    // 0040ED79..0040ED83
    void* list = nullptr;
    if (memory != nullptr) {                                                                   // 0040ED87..0040ED8D
        try {                                                                                  // EH state 0
            list = kotor_fastcall_005e9550(memory, nullptr, 0, 1, 5);                          // 0040ED8F..0040ED9B
        } catch (...) {
            live_free(memory);
            throw;
        }
    }
    for (U i = 0; i < at<U>(self, 0xc); ++i) {                                                 // 0040EDA1..0040EE1F
        unsigned char* entry = at<unsigned char*>(self, 0x10) + i * 0x1c;                      // 0040EDC0..0040EDCD
        if (at<U16>(entry, 0x1a) != static_cast<U16>(type)) continue;                          // 0040EDC3..0040EDD0
        char text[0x14];                                                                       // [esp+18h]
        gui_load_resref_copy_to_string_00405fb0(entry, nullptr, text);                         // 0040EDD2..0040EDD7
        void* string_memory = live_operator_new(8);                                            // 0040EDDC..0040EDE6
        void* string = nullptr;
        if (string_memory != nullptr) {                                                        // 0040EDEA..0040EDF4
            try {                                                                              // EH state 1
                frame_LogStringCtor_005e5a90(string_memory, nullptr, text);                    // 0040EDF6..0040EDFD
            } catch (...) {
                live_free(string_memory);
                throw;
            }
            string = string_memory;
        }
        kotor_sound_internal::StringListAdd005e98b0(list, nullptr,
            static_cast<kotor_reconstruction::StartupString*>(string));                        // 0040EE06..0040EE11
    }
    if (static_cast<I>(kotor_fastcall_00675dd0(list, nullptr)) > 0) return list;               // 0040EE25..0040EE2E (jg)
    if (list != nullptr) {                                                                     // 0040EE30..0040EE32
        kotor_sound_internal::StringListDestroy005e9860(list, nullptr);                        // 0040EE34..0040EE36
        live_free(list);                                                                       // 0040EE3B..0040EE41
    }
    return nullptr;                                                                            // 0040EE44
}

// 00408C60 CExoResMan::GetResOfType(ushort type, CRes* res): the key table of res (GetTable); when present and its
// +4 is 0, its GetKeysOfType(type), else 0. RET 8.
void* __fastcall kotor_fastcall_00408c60(void* self, void*, U type, void* res) {
    void* table = c2da_manager_004076e0(self, nullptr, res);                                   // 00408C60..00408C6A
    if (table == nullptr || at<U>(table, 4) != 0) return nullptr;                              // 00408C6C..00408C77, 00408C83
    return kotor_fastcall_0040ed20(table, nullptr, type);                                      // 00408C79..00408C7E
}

// 00407FE0 CExoResMan::UpdateKeyTable(CExoString const& name, int table_type): table_type 1..4 selects the key-table
// list at +10h/+14h/+18h/+1Ch (jump table 0040806C); the first table whose name (+20h) equals `name` ignoring case is
// rebuilt (RebuildTable) and 1 returned; 0 otherwise. RET 8.
U __fastcall kotor_fastcall_00407fe0(void* self, void*, void* name, U table_type) {
    unsigned char* slot;
    switch (table_type) {                                                                      // 00407FE0..00407FED
    case 1: slot = static_cast<unsigned char*>(self) + 0x10; break;                            // 00407FF4
    case 2: slot = static_cast<unsigned char*>(self) + 0x14; break;                            // 00407FF9
    case 3: slot = static_cast<unsigned char*>(self) + 0x18; break;                            // 00407FFE
    case 4: slot = static_cast<unsigned char*>(self) + 0x1c; break;                            // 00408003
    default: return 0;                                                                         // 0040804F
    }
    void* position = at<void*>(at<void*>(slot, 0), 0);                                         // 00408006..0040800C [esp+14h]
    if (position == nullptr) return 0;                                                         // 00408010
    void* table = unresolved_list_at_pos_005e9b60(at<void*>(slot, 0), nullptr, position);      // 00408012..00408018
    if (position == nullptr) return 0;                                                         // 0040801A..00408020
    do {
        if (table != nullptr &&
            exostring_compare_no_case_005e6450(name, nullptr, static_cast<unsigned char*>(table) + 0x20) != 0) { // 00408026..00408037
            add_key_lower::Rebuild00410260(table, nullptr);                                    // 00408057..00408059
            return 1;                                                                          // 00408060
        }
        table = unresolved_list_next_005e9b90(at<void*>(slot, 0), nullptr, &position);         // 00408039..00408045
    } while (position != nullptr);                                                             // 00408047..0040804D
    return 0;                                                                                  // 0040804F..00408054
}

// 004088E0 CExoResMan::UpdateDirectoryKeyTable(CExoString const& name): UpdateKeyTable(name, 2). RET 4.
U __fastcall kotor_fastcall_004088e0(void* self, void*, void* name) {
    return kotor_fastcall_00407fe0(self, nullptr, name, 2);                                    // 004088E0..004088EC
}

// =========================================================================================================
// CExoString / CExoLocString helpers
// =========================================================================================================

// 005E5950 CExoString::StripNonAlphaNumeric(int lowercase, int allow_at_dot, int allow_extended): builds the allowed
// set in an 80h-byte stack buffer (lowercase: 0074DB7C, else 0074DB3C; + "@." from 0074DB38; + 0074DB1C), then for each
// character (optionally lower-cased in place for 'A'..'Z', signed compare) not found by strchr in the set, shifts the
// rest (including the terminator) left. The CExoString length (+4) is NOT updated (original). Returns 1 when
// something was removed; 0 for a null or empty buffer. RET 0Ch.
U __fastcall kotor_fastcall_005e5950(void* self, void*, U lowercase, U allow_at_dot, U allow_extended) {
    U removed = 0;                                                                             // 005E595D [esp+10h]
    char* text = at<char*>(self, 0);                                                           // 005E5959
    if (text == nullptr) return 0;                                                             // 005E595B..005E596E
    U length = static_cast<U>(std::strlen(text));                                              // 005E5971..005E597E
    if (length == 0) return 0;                                                                 // 005E5980..005E598C
    char allowed[0x80];                                                                        // [esp+14h]
    if (lowercase != 0) std::memcpy(allowed, k_lower_0074db7c, sizeof(k_lower_0074db7c));      // 005E598F..005E59AA, 005E59C3..005E59C7
    else std::memcpy(allowed, k_alnum_0074db3c, sizeof(k_alnum_0074db3c));                     // 005E59AC..005E59B6, 005E59C3..005E59C7
    if (allow_at_dot != 0)                                                                     // 005E59BA..005E59C8
        std::memcpy(allowed + std::strlen(allowed), k_at_dot_0074db38, sizeof(k_at_dot_0074db38)); // 005E59CA..005E59E7
    if (allow_extended != 0)                                                                   // 005E59EA..005E59F3
        std::memcpy(allowed + std::strlen(allowed), k_extended_0074db1c, sizeof(k_extended_0074db1c)); // 005E59F5..005E5A14
    for (U i = 0; i < length; ++i) {                                                           // 005E5A16..005E5A1A, 005E5A79..005E5A7C
        if (lowercase != 0) {                                                                  // 005E5A20..005E5A29
            char* c = at<char*>(self, 0) + i;                                                  // 005E5A2B..005E5A32
            const signed char v = static_cast<signed char>(*c);
            if (v >= 0x41 && v <= 0x5a) *c = static_cast<char>(v + 0x20);                      // 005E5A30..005E5A3D (jl/jg)
        }
        const int value = static_cast<signed char>(at<char*>(self, 0)[i]);                     // 005E5A3F..005E5A41 (movsx)
        if (std::strchr(static_cast<const char*>(allowed), value) != nullptr) continue;                                // 005E5A45..005E5A55 (_strchr 006FB5E0)
        for (U k = i; k < length; ++k) {                                                       // 005E5A57..005E5A6D
            char* buffer = at<char*>(self, 0);
            buffer[k] = buffer[k + 1];
        }
        --i;                                                                                   // 005E5A6F
        --length;                                                                              // 005E5A70
        removed = 1;                                                                           // 005E5A71
    }
    return removed;                                                                            // 005E5A7E
}

// 005E5310 CExoString::operator!=(CExoString const& other): byte compare of the two buffers (strcmp != 0); a null
// buffer equals an empty one. RET 4.
U __fastcall kotor_fastcall_005e5310(void* self, void*, void* other) {
    const unsigned char* a = at<const unsigned char*>(self, 0);                                // 005E5310
    if (a != nullptr) {                                                                        // 005E5312..005E5319
        const unsigned char* b = at<const unsigned char*>(other, 0);                           // 005E531B
        if (b != nullptr) {                                                                    // 005E531D..005E531F
            for (;;) {                                                                         // 005E5322..005E5344
                if (*a != *b) return 1;                                                        // 005E5328..005E532A, 005E5356..005E5366
                if (*a == 0) return 0;                                                         // 005E532C..005E532E, 005E5346..005E5353
                if (a[1] != b[1]) return 1;                                                    // 005E5330..005E533A
                a += 2;
                b += 2;
                if (a[-1] == 0) return 0;                                                      // 005E533C..005E5344
            }
        }
        if (*a != 0) return 1;                                                                 // 005E5369..005E536C, 005E5379
    }
    const unsigned char* b = at<const unsigned char*>(other, 0);                               // 005E536E
    if (b == nullptr || *b == 0) return 0;                                                     // 005E5370..005E5377, 005E5382
    return 1;                                                                                  // 005E5379
}

// 005E9EF0 CExoLocString::GetStringCount: [[this]+4]. RET 0.
U __fastcall kotor_fastcall_005e9ef0(void* self, void*) {
    return at<U>(at<void*>(self, 0), 4);                                                       // 005E9EF0..005E9EF2
}

// 005EC7F0 CExoLocStringInternal::Compare(CExoLocString const& other): equal string counts and, for every
// (language, text) node of this list (+0), other.GetString(language, gender 0) == text. Two heap CExoStrings are
// used as scratch (a failed allocation leaves a null string, original). RET 4, EAX = 1 / 0.
U __fastcall kotor_fastcall_005ec7f0(void* self, void*, void* other) {
    const U count = at<U>(self, 4);                                                            // 005EC810
    if (count != kotor_fastcall_005e9ef0(other, nullptr)) return 0;                            // 005EC80B..005EC82E
    auto new_string = []() -> void* {
        void* memory = live_operator_new(8);                                                   // 005EC833..005EC83D / 005EC85B..005EC869
        if (memory == nullptr) return nullptr;
        try {                                                                                  // EH states 0 / 1
            return kotor_menu20::ConstructString005b3190(static_cast<kotor_menu20::String*>(memory), nullptr); // 005EC84B / 005EC879
        } catch (...) {
            live_free(memory);
            throw;
        }
    };
    void* mine = new_string();                                                                 // EBX
    void* theirs = new_string();                                                               // EDI
    auto destroy = [](void* s) {
        if (s == nullptr) return;
        frame_LogStringDtor_005e5c20(s, nullptr);
        live_free(s);
    };
    U result = 1;
    void* list = at<void*>(self, 0);                                                           // 005EC882
    void* head = at<void*>(list, 0);                                                           // 005EC885
    void* node = head != nullptr ? at<void*>(head, 8) : nullptr;                               // 005EC887..005EC88F
    void* position = head;                                                                     // 005EC894..005EC896 [esp+10h]
    while (node != nullptr) {                                                                  // 005EC892..005EC89A, 005EC8D3..005EC8D5
        const U language = at<U>(node, 0);                                                     // 005EC8A0
        (void)async_assign_string_005e5c50(mine, nullptr, static_cast<unsigned char*>(node) + 4); // 005EC8A2..005EC8A8
        (void)kotor_fastcall_005ea050(other, nullptr, language, theirs, 0);                    // 005EC8AD..005EC8B5
        if (kotor_fastcall_005e5310(mine, nullptr, theirs) != 0) { result = 0; break; }        // 005EC8BA..005EC8C4
        node = unresolved_list_next_005e9b90(at<void*>(self, 0), nullptr, &position);          // 005EC8C6..005EC8CE
    }
    destroy(mine);                                                                             // 005EC8D7..005EC8E8 / 005EC919..005EC92A
    destroy(theirs);                                                                           // 005EC8EB..005EC8FC / 005EC92D..005EC93E
    return result;                                                                             // 005EC8FF / 005EC941
}

// 005E9F10 CExoLocString::Compare(CExoLocString const& other): +0 internal, +4 strref. When exactly one internal has a
// nonzero +8, the other's string count must be 0; with both +8 nonzero only the strrefs are compared; with both zero,
// the internal compare must succeed and the strrefs match. RET 4, EAX = 1 equal / 0.
U __fastcall kotor_fastcall_005e9f10(void* self, void*, void* other) {
    void* mine = at<void*>(self, 0);                                                           // 005E9F18
    if (at<U>(mine, 8) != 0) {                                                                 // 005E9F1A..005E9F1F
        void* theirs = at<void*>(other, 0);                                                    // 005E9F21
        if (at<U>(theirs, 8) == 0 && at<U>(theirs, 4) != 0) return 0;                          // 005E9F23..005E9F37, 005E9F47
    } else {
        void* theirs = at<void*>(other, 0);                                                    // 005E9F39
        if (at<U>(theirs, 8) == 0) {                                                           // 005E9F3B..005E9F40
            if (kotor_fastcall_005ec7f0(mine, nullptr, other) == 0) return 0;                  // 005E9F59..005E9F61
            return at<U>(self, 4) == at<U>(other, 4) ? 1u : 0u;                                // 005E9F63..005E9F72
        }
        if (at<U>(mine, 4) != 0) return 0;                                                     // 005E9F42..005E9F47
    }
    return at<U>(self, 4) == at<U>(other, 4) ? 1u : 0u;                                        // 005E9F49..005E9F56
}

// 005EA020 CExoLocString::Compare: jmp 005E9F10. RET 4.
U __fastcall kotor_fastcall_005ea020(void* self, void*, void* other) {
    return kotor_fastcall_005e9f10(self, nullptr, other);                                      // 005EA020
}
}  // extern "C"
}  // namespace save_gff
