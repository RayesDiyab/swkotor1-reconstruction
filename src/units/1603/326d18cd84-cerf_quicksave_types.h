#pragma once
// Lane B private declarations and exact object layouts.
//
// No original address is dereferenced and no numeric-address alias is defined.
// Every callee below is a selected v5 provider reached through its exact symbol;
// the local C-linkage names for the C++-mangled selected providers are bound by
// one /alternatename directive each in cerf_quicksave_bindings.cpp.

#include <cstddef>
#include <cstdint>
#include <cstring>
#include "6091d1cbd5-cerf_quicksave_exports.h"

// Selected shared storage (symbols, not addresses).
namespace kotor_menu_window_cells {
extern void* volatile manager_007a39fc;  // ?manager_007a39fc@kotor_menu_window_cells@@3RAXA
}
namespace frame_control {
extern std::uint32_t resource_manager_007a39e8;  // ?resource_manager_007a39e8@frame_control@@3IA
}

// 8-byte CExoString/StartupString view. The bodies below never access the fields
// directly except in CERFString (id/length/text), which is a different 0xC record.
namespace kotor_reconstruction {
struct StartupString {
    char* data;
    std::uint32_t capacity;
};
}  // namespace kotor_reconstruction
static_assert(sizeof(kotor_reconstruction::StartupString) == 8,
              "accepted CExoString layout");

namespace kotor_list {
struct Internal;  // +0 head, +4 tail, +8 count (selected list bodies)
}
namespace base_ini_reconstruction {
struct Node;
}
namespace tlk_file {
struct File;
}

// ---------------------------------------------------------------------------
// Selected providers with exact C-linkage decorations (no alias needed).
// ---------------------------------------------------------------------------
extern "C" {

// 005E5670 / 005E5140 / 005E5A90 / 005E5C20 / 005E5C50 / 005E5680 / 005E5D10 /
// 005E5790 are reached through the local alias declarations below.
char* __fastcall WindowCStr005e5670(void* self, void* unused);              // @8
void* __fastcall GuiAssign005e5140(void* self, void* unused,
                                   const char* text);                        // @12

int __fastcall exofile_size_005e6950(void* self, void* unused);              // @8
int __fastcall exofile_seek_005e6aa0(void* self, void* unused, int offset,
                                     int origin);                             // @16
// savecrash: stack-argument order is (buffer, size, count) -- the original callers push count, size, buffer
// (e.g. 005DCF7B push 1 / push 74h / push esi), so [esp] is the buffer; 005EBA63..005EBA79 forwards them to
// fread(buffer, size, count, FILE*). The previous declaration named the slots (flags, size, buffer), i.e. reversed,
// so every CERF read/write passed buffer=count and count=buffer to the selected providers.
int __fastcall exofile_read_005e6960(void* self, void* unused, void* buffer,
                                     int size, int count);                    // @20
int __fastcall internal_read_005eba40(void* self, void* unused, void* buffer,
                                      int size, int count);                   // @20
int __fastcall recovered_file_write_005e69a0(void* self, void* unused,
                                             void* buffer, int size,
                                             int count);                       // @20
int __fastcall file_opened_005e6a10(void* self, void* unused);               // @8
void __fastcall file_destroy_005e69e0(void* self, void* unused);             // @8

int __fastcall kotor_fastcall_005edff0(void* self, void* unused, int index); // @12
int __fastcall gui_load_resman_create_directory_004068c0(
    void* unused, void* unused2, void* name);                                // @12
int __fastcall gui_load_resman_clean_directory_00409460(
    void* unused, void* unused2, void* name, void* arg2, void* arg3);        // @20

void __fastcall kotor_m15_client_set_load_bar_progress_005edd40(
    void* internal, void* unused, int progress, int flags);                  // @16
void __cdecl kotor_cdecl_00401c10(float time, unsigned a, unsigned b);       // _kotor_cdecl_00401c10

unsigned __cdecl fread(void* buffer, unsigned size, unsigned count, void* file);
int __cdecl fflush(void* file);

extern void* __cdecl live_operator_new(unsigned size);   // _live_operator_new
extern void __cdecl live_free(void* memory);             // _live_free

}  // extern "C"

// ---------------------------------------------------------------------------
// Local declarations for selected C++-mangled providers. Each one has exactly one
// /alternatename in cerf_quicksave_bindings.cpp; none is a new body.
// ---------------------------------------------------------------------------
extern "C" {

void* __fastcall cerf_startup_string_default(void* self, void* unused);  // @8
void* __fastcall cerf_startup_string_from_text(void* self, void* unused,
                                               const char* text);        // @12
void __fastcall cerf_startup_string_destroy(void* self, void* unused);   // @8
void* __fastcall cerf_startup_string_assign(void* self, void* unused,
                                            void* rhs);                  // @12
int __fastcall cerf_startup_string_borrowed_length(void* self, void* unused);  // @8
void __cdecl cerf_startup_format(void* out, const char* fmt, ...);       // _cerf_startup_format
void* __fastcall cerf_gui_concat(void* out, void* unused, void* a,
                                 void* b);                               // @16

void* __cdecl cerf_construct_list_internal(void* pointer);               // _cerf_construct_list_internal
void __fastcall cerf_destroy_list(void* list, void* unused);             // @8
void* __fastcall cerf_remove_head(void* list, void* unused);             // @8
void __fastcall cerf_list_unwind(void* member, void* unused);            // @8
void* __fastcall cerf_list_add_tail(void* list, void* unused, void* data);  // @12
void* __stdcall cerf_list_at_pos(void* node);                            // _cerf_list_at_pos@4
void* __stdcall cerf_list_next(void* position);                          // _cerf_list_next@4

void* __fastcall cerf_exofile_construct(void* self, void* unused, void* a,
                                        void* b, void* c);               // @20

}  // extern "C"

// ---------------------------------------------------------------------------
// Small exact-layout helpers.
// ---------------------------------------------------------------------------
inline std::uint32_t ld32(const void* base, std::size_t offset) {
    std::uint32_t value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset,
                sizeof(value));
    return value;
}
inline void st32(void* base, std::size_t offset, std::uint32_t value) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value,
                sizeof(value));
}
inline std::uint16_t ld16(const void* base, std::size_t offset) {
    std::uint16_t value;
    std::memcpy(&value, static_cast<const unsigned char*>(base) + offset,
                sizeof(value));
    return value;
}
inline void st16(void* base, std::size_t offset, std::uint16_t value) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &value,
                sizeof(value));
}
inline void* ldptr(const void* base, std::size_t offset) {
    return reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(ld32(base, offset)));
}
inline void stptr(void* base, std::size_t offset, const void* value) {
    st32(base, offset,
         static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(value)));
}

// RAII wrapper around a local 8-byte CExoString. Construction/destruction go
// through the exact selected providers. If the ctor throws, C++ does not run the
// dtor for the unconstructed object (the original has no dtor for that object).
class CExoStringLocal {
public:
    CExoStringLocal() { cerf_startup_string_default(storage_, nullptr); }
    explicit CExoStringLocal(const char* text) {
        cerf_startup_string_from_text(storage_, nullptr, text);
    }
    ~CExoStringLocal() { cerf_startup_string_destroy(storage_, nullptr); }
    CExoStringLocal(const CExoStringLocal&) = delete;
    CExoStringLocal& operator=(const CExoStringLocal&) = delete;

    void* get() { return storage_; }
    const void* get() const { return storage_; }
    void assign(void* rhs) { cerf_startup_string_assign(storage_, nullptr, rhs); }
    void assign_text(const char* text) {
        GuiAssign005e5140(storage_, nullptr, text);
    }
    const char* c_str() const {
        return WindowCStr005e5670(const_cast<unsigned char*>(storage_), nullptr);
    }
    int borrowed_length() const {
        return cerf_startup_string_borrowed_length(
            const_cast<unsigned char*>(storage_), nullptr);
    }

private:
    alignas(4) unsigned char storage_[8];
};
static_assert(sizeof(CExoStringLocal) == 8, "CExoString storage");

// Concatenate a + b + c through the selected 005E5D10 provider and assign the
// result to `out`. The original passes two stack strings and ECX=first string;
// the local alias uses ECX=first, two stack args, RET 8.
inline void assign_concat(CExoStringLocal& out, const void* a, const void* b,
                          const void* c) {
    out.assign(cerf_gui_concat(const_cast<void*>(a), nullptr,
                               const_cast<void*>(b), const_cast<void*>(c)));
}

// RAII around a local 4-byte CExoFile wrapper (its first dword is the internal
// pointer). The selected ctor/dtor own the internal allocation.
class CExoFileLocal {
public:
    CExoFileLocal() : internal_(nullptr), constructed_(false) {}
    ~CExoFileLocal() {
        if (constructed_) file_destroy_005e69e0(&internal_, nullptr);
    }
    CExoFileLocal(const CExoFileLocal&) = delete;
    CExoFileLocal& operator=(const CExoFileLocal&) = delete;

    void construct(void* path, int flags, void* name) {
        try {
            cerf_exofile_construct(&internal_, nullptr, path,
                                   reinterpret_cast<void*>(
                                       static_cast<std::uintptr_t>(flags)),
                                   name);
            constructed_ = true;
        } catch (...) {
            // Original 005DCE50/005DD710 EH state 1 destroys the stack CExoFile
            // through 005E69E0 even when its ctor throws. internal_ is initialized
            // null by this object's constructor; the selected dtor is a no-op for
            // a null internal, so the state-1 call is preserved.
            file_destroy_005e69e0(&internal_, nullptr);
            throw;
        }
    }
    void* get() { return &internal_; }
    int opened() { return file_opened_005e6a10(&internal_, nullptr); }
    void seek(int offset, int origin) {
        exofile_seek_005e6aa0(&internal_, nullptr, offset, origin);
    }
    int size() { return exofile_size_005e6950(&internal_, nullptr); }
    // Arguments in the original push order (count, size, buffer); forwarded as (buffer, size, count).
    int read(int count, int size, void* buffer) {
        return exofile_read_005e6960(&internal_, nullptr, buffer, size, count);
    }
    int write(int count, int size, void* buffer) {
        return recovered_file_write_005e69a0(&internal_, nullptr, buffer, size,
                                             count);
    }

private:
    void* internal_;
    bool constructed_;
};

// RAII around a caller-allocated 4-byte CExoFile wrapper. The selected ctor is
// called at the raw allocation address. Original 005DD710 EH state 6 frees the
// raw 4-byte allocation only on a ctor throw; the selected ctor's own EH owns the
// internal allocation. The stack CExoFileLocal, not this heap wrapper, is the
// state-1/2 dtor case.
class CExoFileRaw {
public:
    CExoFileRaw() : storage_(nullptr), constructed_(false) {}
    ~CExoFileRaw() {
        if (storage_ != nullptr) {
            if (constructed_) file_destroy_005e69e0(storage_, nullptr);
            live_free(storage_);
        }
    }
    CExoFileRaw(const CExoFileRaw&) = delete;
    CExoFileRaw& operator=(const CExoFileRaw&) = delete;

    void attach(void* storage) { storage_ = storage; }
    void construct(void* path, int flags, void* name) {
        try {
            cerf_exofile_construct(storage_, nullptr, path,
                                   reinterpret_cast<void*>(
                                       static_cast<std::uintptr_t>(flags)),
                                   name);
            constructed_ = true;
        } catch (...) {
            // Original 005DD710 EH state 6 frees the raw allocation only; the
            // selected ctor EH has already released its internal. Do not call
            // 005E69E0 here: it can see a stale internal and double-free.
            throw;
        }
    }
    void* get() { return storage_; }
    int opened() { return file_opened_005e6a10(storage_, nullptr); }
    void seek(int offset, int origin) {
        exofile_seek_005e6aa0(storage_, nullptr, offset, origin);
    }
    int size() { return exofile_size_005e6950(storage_, nullptr); }
    // Arguments in the original push order (count, size, buffer); forwarded as (buffer, size, count).
    int read(int count, int size, void* buffer) {
        return exofile_read_005e6960(storage_, nullptr, buffer, size, count);
    }
    int write(int count, int size, void* buffer) {
        return recovered_file_write_005e69a0(storage_, nullptr, buffer, size,
                                             count);
    }
    int flush() { return cerf_c_exofile_flush_005e6a20(storage_); }

private:
    void* storage_;
    bool constructed_;
};

// ---------------------------------------------------------------------------
// CERFFile: exact 0xD0 layout. Member construction order matches the original
// 005DD9C0 (three list wrappers, then the CExoString; header zeroing follows in
// the ctor body). Destruction order matches 005DE3A0 (Reset, string, lists).
// ---------------------------------------------------------------------------
namespace cerf_detail {

struct ListMember {
    void* internal;
    ListMember() : internal(nullptr) { cerf_list_construct_005dd350(this); }
    ~ListMember() { cerf_list_unwind(this, nullptr); }
    ListMember(const ListMember&) = delete;
    ListMember& operator=(const ListMember&) = delete;
};

struct StringMember {
    alignas(4) unsigned char storage[8];
    StringMember() { cerf_startup_string_default(storage, nullptr); }
    ~StringMember() { cerf_startup_string_destroy(storage, nullptr); }
    StringMember(const StringMember&) = delete;
    StringMember& operator=(const StringMember&) = delete;
    void* get() { return storage; }
};
static_assert(sizeof(StringMember) == 8, "StringMember must stay 8 bytes");

}  // namespace cerf_detail

struct CERFFile {
    unsigned char header[0xA0];                 // +0x00..+0x9F, zeroed in body
    cerf_detail::ListMember list_strings;       // +0xA0
    cerf_detail::ListMember list_resources;     // +0xA4
    cerf_detail::ListMember list_locstrings;    // +0xA8
    unsigned char fields[0x1C];                 // +0xAC..+0xC7
    cerf_detail::StringMember name;             // +0xC8

    CERFFile() {
        std::memset(header, 0, sizeof(header));
        st32(fields, 0x00, 0);  // +0xAC
        st32(fields, 0x10, 0);  // +0xBC
        st32(fields, 0x14, 0);  // +0xC0
    }
    ~CERFFile() { (void)cerf_cerf_file_reset_005de2c0(this); }
    // The compiler's deleting-dtor branch is not executed by 005DE3A0 (flag 0),
    // but if it is emitted/called it must use the original game heap, never CRT.
    static void operator delete(void* p) noexcept { live_free(p); }
    static void operator delete(void* p, std::size_t) noexcept { live_free(p); }
    // Placement-new cleanup is a no-op: the holder owns and frees the raw 0xD0 block once.
    static void operator delete(void*, void*) noexcept {}
    CERFFile(const CERFFile&) = delete;
    CERFFile& operator=(const CERFFile&) = delete;
};
static_assert(sizeof(CERFFile) == 0xD0, "CERFFile 0xD0 layout");

// RAII holder for the 0xD0 game-heap allocation. If the ctor throws, only the raw
// allocation is freed; if it succeeds, the real dtor runs and the 0xD0 block is
// then released with the game deallocator (original 0062FE39 dtor / 0062FE3F free).
class CERFFileHolder {
public:
    CERFFileHolder() : storage_(nullptr), constructed_(false) {}
    ~CERFFileHolder() {
        if (storage_ != nullptr) {
            if (constructed_) cerf_destroy_cerf_file_005de3a0(storage_);
            live_free(storage_);
        }
    }
    CERFFileHolder(const CERFFileHolder&) = delete;
    CERFFileHolder& operator=(const CERFFileHolder&) = delete;

    // Returns the raw pointer (null on allocation failure), preserving the real
    // 0062FBE0 guard: ctor skipped and ESI=0 when the allocation is null.
    void* allocate_and_construct() {
        storage_ = live_operator_new(0xD0);
        if (storage_ != nullptr) {
            cerf_construct_cerf_file_005dd9c0(storage_);
            constructed_ = true;
        }
        return storage_;
    }
    void* get() { return storage_; }

private:
    void* storage_;
    bool constructed_;
};
