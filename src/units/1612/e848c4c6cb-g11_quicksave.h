#pragma once
// G11-quicksave lane: private CExoString/CExoFile RAII declarations and the
// exact local C-linkage binding names that G11-core maps by /alternatename.
//
// Nothing here defines a selected provider. The one shared header is
// G11-core/include/g11_crt_state.h (frozen); it is included exactly once. The
// include is source-relative on purpose: from source/ the path is
// source -> G11-quicksave -> lanes -> G11-core, i.e. ../../G11-core. No extra
// /I root is required for this header (see RECIPE.json include_roots).
//
// Evidence:
//   original 006323A0..0063271A (890 B), 23-state EH
//   direct callees 005B3190/005E5A90/005E5C20/005E5C50/005E5790/005E5F70/
//   005E68A0/005E5670/005E5680/005E5D10/005E5140/00409480/005EDA90/005E68D0/
//   005E6A40/005E69E0/005E6A20 and B's 0062FBE0
//   D-owned CRT helper 006FE5C6 (this lane, g11_error.cpp)
// No numeric live original address is dereferenced or aliased here.

#include <cstddef>
#include <cstdint>
#include "cd725180f8-g11_crt_state.h"

extern "C" {

// ---------------------------------------------------------------------------
// CExoString selected-provider views (exact frozen local signatures,
// BINDING-NAMES.md). Declarations only; G11-core owns every mapping.
// ---------------------------------------------------------------------------
void* __fastcall g11_exostring_default(void* self, void* unused);              // @8
void* __fastcall g11_exostring_text(void* self, void* unused, const char* text);  // @12
void  __fastcall g11_exostring_destroy(void* self, void* unused);              // @8
void* __fastcall g11_exostring_assign(void* self, void* unused, void* rhs);    // @12
int   __fastcall g11_exostring_length(void* self, void* unused);               // @8
char* __fastcall g11_exostring_cstr(void* self, void* unused);                 // @8
void* __fastcall g11_exostring_assign_text(void* self, void* unused,
                                           const char* text);                  // @12
void  __cdecl   g11_exostring_format(void* out, const char* fmt, ...);         // _g11_exostring_format
void* __fastcall g11_exostring_concat(void* a, void* unused, void* b, void* c);  // @16
void* __fastcall g11_exostring_left(void* src, void* unused, void* out,
                                    int count);                                // @16
void* __fastcall g11_exostring_resolve(void* aliases, void* unused, void* out,
                                       void* name, unsigned flags);            // @20

// ---------------------------------------------------------------------------
// CExoFile and selected game-provider views (exact frozen local signatures).
// ---------------------------------------------------------------------------
void* __fastcall g11_exofile_construct(void* self, void* unused, void* path,
                                       unsigned flags, void* name);            // @20
void  __fastcall g11_exofile_destroy(void* self, void* unused);                // @8
int   __fastcall g11_exofile_write(void* file, void* unused, const char* text);  // @12
int   __fastcall g11_exofile_flush(void* file);                                // @4
void* __fastcall g11_hide_load_screen(void* internal, void* unused, int a,
                                      int b);                                  // @16
void* __fastcall g11_nuke_directory(void* resman, void* unused, void* name,
                                    void* a, void* b);                         // @20
int   __stdcall  g11_copy_quick_save(int save_id, void* suffix);               // @8

// Selected data cells. Storage is owned by the selected image; these are
// declarations only and are read exactly where the original reads them:
//   g11_exo_base          [cell] + 0x0C   -> ResolveFileName alias-list this
//   g11_resource_manager  [cell]          -> NukeDirectory this
//   g11_app_manager       [cell] + 0x04   -> HideLoadScreen internal
extern void* g11_exo_base;
extern void* g11_resource_manager;
extern void* g11_app_manager;

// Reconstructed original 006323A0 export: __fastcall, 4 parameters (16 bytes),
// ret 8; ECX (self) and EDX are not read, save_id/suffix are the stack args.
// Exact decorated name: @load_screen_unpack_quick_save_game_006323a0@16
int __fastcall load_screen_unpack_quick_save_game_006323a0(
    void* self, void* unused, int save_id, void* suffix);

}  // extern "C"

namespace g11_quicksave {

// Original CExoString is an 8-byte record: text pointer + capacity/length dword.
// ExoStringStorage is exactly that provider-facing record. The ExoString wrapper
// adds only a private destroyed_ flag, which is never passed to a provider.
// Normal /GS cookies are retained; cookie presence is not an acceptance
// criterion.
struct ExoStringStorage {
    void* data;
    unsigned int capacity;
};
static_assert(sizeof(ExoStringStorage) == 8, "CExoString must stay 8 bytes");
static_assert(alignof(ExoStringStorage) == 4, "CExoString alignment is 4");

// RAII CExoString. Every construction/destruction goes through the exact
// selected provider. By-value construction/concat/left/resolve write into this
// object's storage through the provider's hidden sret pointer, so no extra
// default construction occurs.
//
// Destroy() exists because the original explicitly ends some named temporaries
// before a later provider call. The quicksave body calls it from plain
// statements, so no provider/cleanup order depends on argument evaluation
// order. Destroy() marks the object dead before its provider call: a throwing
// destroy is not retried during unwinding, and the scope-exit destructor is a
// no-op after an explicit Destroy().
class ExoString {
public:
    ExoString() : destroyed_(false) {
        g11_exostring_default(&storage_, nullptr);
    }
    ExoString(const char* text) : destroyed_(false) {
        g11_exostring_text(&storage_, nullptr, text);
    }
    // The selected destroy provider is not positively proven nonthrowing; keep
    // the original propagation/terminate behavior instead of the implicit
    // noexcept(true) that /EHs would otherwise retain.
    ~ExoString() noexcept(false) { Destroy(); }

    void Destroy() {
        if (!destroyed_) {
            destroyed_ = true;
            g11_exostring_destroy(&storage_, nullptr);
        }
    }

    ExoString(const ExoString&) = delete;
    ExoString& operator=(const ExoString& rhs) {
        g11_exostring_assign(
            &storage_, nullptr,
            const_cast<ExoStringStorage*>(&rhs.storage_));
        return *this;
    }

    void* raw() { return &storage_; }
    const void* raw() const { return &storage_; }

    // 005E5C50 Assign from a caller-owned CExoString (the ret8 suffix argument).
    void AssignRaw(void* rhs) { g11_exostring_assign(&storage_, nullptr, rhs); }

    // 005E5140 GuiAssign from a C string.
    ExoString& AssignText(const char* text) {
        g11_exostring_assign_text(&storage_, nullptr, text);
        return *this;
    }

    int Length() const {
        return g11_exostring_length(
            const_cast<ExoStringStorage*>(&storage_), nullptr);
    }
    char* CStr() const {
        return g11_exostring_cstr(
            const_cast<ExoStringStorage*>(&storage_), nullptr);
    }

private:
    struct ConcatTag {
        const ExoString* lhs;
        const ExoString* rhs;
    };
    struct LeftTag {
        const ExoString* src;
        int count;
    };
    struct ResolveTag {
        void* aliases;
        const ExoString* name;
        unsigned flags;
    };

    // 005E5D10: ECX=lhs, hidden sret=this, first stack arg=sret, second=rhs.
    explicit ExoString(ConcatTag tag) : destroyed_(false) {
        g11_exostring_concat(
            const_cast<ExoStringStorage*>(&tag.lhs->storage_), nullptr,
            &storage_,
            const_cast<ExoStringStorage*>(&tag.rhs->storage_));
    }
    // 005E5F70: ECX=src, hidden sret=this (first stack arg), count.
    explicit ExoString(LeftTag tag) : destroyed_(false) {
        g11_exostring_left(
            const_cast<ExoStringStorage*>(&tag.src->storage_), nullptr,
            &storage_, tag.count);
    }
    // 005E68A0: ECX=alias list, hidden sret=this, name, flags=0xFFFF.
    explicit ExoString(ResolveTag tag) : destroyed_(false) {
        g11_exostring_resolve(
            tag.aliases, nullptr, &storage_,
            const_cast<ExoStringStorage*>(&tag.name->storage_), tag.flags);
    }

    friend ExoString operator+(const ExoString& lhs, const ExoString& rhs);
    friend ExoString left(const ExoString& src, int count);
    friend ExoString resolve(void* aliases, const ExoString& name,
                             unsigned flags);

    ExoStringStorage storage_;
    bool destroyed_;
};

// Returns the 005E5D10 result by value (guaranteed-elided prvalue), exactly the
// original's full-expression temporary.
inline ExoString operator+(const ExoString& lhs, const ExoString& rhs) {
    return ExoString(ExoString::ConcatTag{&lhs, &rhs});
}
inline ExoString left(const ExoString& src, int count) {
    return ExoString(ExoString::LeftTag{&src, count});
}
inline ExoString resolve(void* aliases, const ExoString& name, unsigned flags) {
    return ExoString(ExoString::ResolveTag{aliases, &name, flags});
}

// RAII CExoFile wrapper: exactly one 4-byte internal pointer, constructed and
// destroyed through the selected 005E68D0 / 005E69E0 providers. It takes the
// already-constructed path/name storage pointers, so the provider call itself
// has only plain arguments. It is declared after the five named CExoString
// temporaries it consumes, so the normal scope-exit order is file dtor then the
// (already explicitly destroyed) temporaries, and an exception during their
// destruction destroys the file first.
class ExoFile {
public:
    ExoFile(void* path, unsigned flags, void* name) {
        g11_exofile_construct(&internal_, nullptr, path, flags, name);
    }
    // Same nonthrowing-contract rule as ~ExoString above.
    ~ExoFile() noexcept(false) { g11_exofile_destroy(&internal_, nullptr); }

    ExoFile(const ExoFile&) = delete;
    ExoFile& operator=(const ExoFile&) = delete;

    void* raw() { return &internal_; }
    int Write(const char* text) {
        return g11_exofile_write(&internal_, nullptr, text);
    }
    int Flush() { return g11_exofile_flush(&internal_); }

private:
    void* internal_;
};
static_assert(sizeof(ExoFile) == 4, "CExoFile wrapper is one pointer");

}  // namespace g11_quicksave
