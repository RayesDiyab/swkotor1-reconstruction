// Lane B — three no-EH owned bodies and five B-owned new bodies.
//
// 005E6A20, 005DD170, 005DE2C0, 005DD240, 005E6970, 005EB9F0, 004010A0,
// 006C8750. These bodies have no original C++ EH record, no catch and no RAII
// cleanup; the leaf TU uses the package default (/EHsc). No trap, stub, partial
// body, raw payload or numeric-address alias is present.
#include "326d18cd84-cerf_quicksave_types.h"

using kotor_menu_window_cells::manager_007a39fc;

// 005E6A20 — CExoFile::Flush. ECX=&CExoFile wrapper; 0 args; RET 0.
// Returns 1 when fflush(FILE*)==0, 0 otherwise or when FILE* is null. The
// original dereferences the wrapper's internal pointer without a guard.
int __fastcall cerf_c_exofile_flush_005e6a20(void* self) {
    void* const internal = ldptr(self, 0);
    void* const stream = ldptr(internal, 0);
    if (stream == nullptr) return 0;
    return fflush(stream) == 0 ? 1 : 0;
}

// 005DD170 — CERFRes::CopyToFile. ECX=record; 3 args; RET 0xC. Always returns 1.
// arg3 is not read by the original (ABI slot only).
int __fastcall cerf_cerf_res_copy_to_file_005dd170(void* self, void*,
                                                   void* in_file, void* out_file,
                                                   void*) {
    const int offset = static_cast<int>(ld32(self, 0));
    int remaining = static_cast<int>(ld32(self, 4));
    (void)exofile_seek_005e6aa0(in_file, nullptr, offset, 0);

    void* const client = manager_007a39fc;
    if (ldptr(client, 0) == nullptr) {
        cerf_alloc_large_temp_buffer_004010a0(client);
    }
    void* const buffer = ldptr(client, 0);

    while (remaining > 0) {
        const int chunk = remaining > 0x40000 ? 0x40000 : remaining;
        (void)exofile_read_005e6960(in_file, nullptr, buffer, chunk, 1);        // 005DD1B0..005DD1B6
        (void)recovered_file_write_005e69a0(out_file, nullptr, buffer, chunk, 1);  // 005DD1BF..005DD1C3
        remaining -= chunk;
    }
    return 1;
}

// 005DE2C0 — CERFFile::Reset. ECX=this; 0 args; RET 0. Always returns 1.
// The header clear is first; +0xA4 payload word +0x14 is set to 0xFFFF.
int __fastcall cerf_cerf_file_reset_005de2c0(void* self) {
    std::memset(self, 0, 0xA0);

    // List +0xA0 and +0xA8: free payload+8, zero payload+0/+4, free the node.
    for (int which = 0; which < 2; ++which) {
        void* const list = ldptr(self, which == 0 ? 0xA0 : 0xA8);
        if (list == nullptr) continue;
        while (ld32(list, 0) != 0) {
            void* const payload = cerf_remove_head(list, nullptr);
            if (payload == nullptr) break;
            void* const extra = ldptr(payload, 8);
            if (extra != nullptr) live_free(extra);
            stptr(payload, 8, nullptr);
            stptr(payload, 0, nullptr);
            stptr(payload, 4, nullptr);
            live_free(payload);
        }
    }

    // List +0xA4: +0x10=0, word +0x14=0xFFFF, +0/+4/+8/+0xC=0, then free.
    void* const resources = ldptr(self, 0xA4);
    if (resources != nullptr) {
        while (ld32(resources, 0) != 0) {
            void* const payload = cerf_remove_head(resources, nullptr);
            if (payload == nullptr) break;
            st32(payload, 0x10, 0);
            st16(payload, 0x14, 0xFFFF);
            st32(payload, 0x00, 0);
            st32(payload, 0x04, 0);
            st32(payload, 0x08, 0);
            st32(payload, 0x0C, 0);
            live_free(payload);
        }
    }
    return 1;
}

// 005DD240 — CERFString::SetText. ECX=this; one CExoString*; RET 4.
// EAX is the copied length (the original leaves it in EAX); callers ignore it.
int __fastcall cerf_cerf_string_set_text_005dd240(void* self, void*,
                                                  void* src) {
    char* const old_text = static_cast<char*>(ldptr(self, 8));
    if (old_text != nullptr) live_free(old_text);
    stptr(self, 8, nullptr);

    const int length = cerf_startup_string_borrowed_length(src, nullptr);
    st32(self, 4, static_cast<std::uint32_t>(length));

    char* const buffer =
        static_cast<char*>(live_operator_new(
            static_cast<unsigned>(static_cast<std::size_t>(length) + 1)));
    stptr(self, 8, buffer);

    const char* const text = WindowCStr005e5670(src, nullptr);
    std::memcpy(buffer, text, static_cast<std::size_t>(length));
    buffer[length] = '\0';
    return length;
}

// 005E6970 — CExoFile::ReadString thunk. ECX=CExoFile*; two args; RET 8.
int __fastcall cerf_c_exofile_read_string_005e6970(void* file, void*,
                                                   void* out, int length) {
    return cerf_c_exofile_internal_read_string_005eb9f0(
        ldptr(file, 0), nullptr, out, length);
}

// 005EB9F0 — CExoFileInternal::ReadString. ECX=FILE holder; two args; RET 8.
// Null FILE* returns 0 without touching the output string.
int __fastcall cerf_c_exofile_internal_read_string_005eb9f0(void* file, void*,
                                                            void* out,
                                                            int length) {
    void* const stream = ldptr(file, 0);
    if (stream == nullptr) return 0;

    char* const buffer =
        static_cast<char*>(live_operator_new(
            static_cast<unsigned>(static_cast<std::size_t>(length) + 1)));
    const unsigned got = fread(buffer, 1, static_cast<unsigned>(length), stream);
    buffer[got] = '\0';
    GuiAssign005e5140(out, nullptr, buffer);  // original 005EBA23 uses 005E5140
    live_free(buffer);
    return static_cast<int>(got);
}

// 004010A0 — CAppManager::AllocLargeTempBuffer. ECX=client; 0 args; RET 0.
void __fastcall cerf_alloc_large_temp_buffer_004010a0(void* self) {
    if (ldptr(self, 0) == nullptr) {
        stptr(self, 0, live_operator_new(0x40000));
    }
}

// 006C8750 — LoadScreenUpdate. Four stack args; RET 0.
// progress = low32(__ftol2_64((a/b)*d)) + c; ftol FIRST, then the integer additive term.
// transition lane (review F4): after the `push 1` at 006C8754 the multiplier `fimul [esp+14h]` (006C875A) is the
// FOURTH argument and the additive `add eax,[esp+10h]` (006C8763) is the THIRD; the previous body had them swapped.
// The leaf TU is built /arch:IA32. The 64-bit cast is volatile so /O2 cannot narrow
// it back to the 32-bit __ftol2_sse/__ftoi2 helper; bindings.cpp maps __ftol2 to
// _genuine_x87_ftol_006fae8c. This is a source-local conversion correction, not a
// link-global alias; root GB12 checks the emitted call.
int __cdecl cerf_load_screen_update_006c8750(int a, int b, int c, int d) {
    const volatile long long truncated64 =
        static_cast<long long>((static_cast<double>(a) / static_cast<double>(b)) *
                               static_cast<double>(d));   // 006C8750 fild a / fidiv b / fimul d
    const int truncated = static_cast<int>(truncated64);
    const int progress = truncated + c;   // 006C8763 add eax, c

    // Original 006C8768 dereferences [0x7A39FC]+4 unconditionally; no null guard.
    void* const internal = ldptr(manager_007a39fc, 4);
    kotor_m15_client_set_load_bar_progress_005edd40(internal, nullptr, progress,
                                                    1);

    union {
        std::uint32_t bits;
        float value;
    } update = {0x3D088889u};
    kotor_cdecl_00401c10(update.value, 0, 0);
    return 0;
}
