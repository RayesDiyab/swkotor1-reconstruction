// Lane B — eight original-EH bodies, compiled /EHs.
//
// 0062FBE0, 005DD9C0, 005DCE50, 005DD3C0, 005DD710, 005DE3A0, 005DD2B0,
// 005DD350. Each body keeps the accepted PLAN's exact ABI/RET, selected-provider
// calls, fail paths and destructor lifetimes. No trap, stub, partial body, raw
// payload or numeric-address alias is present.

#include <new>
#include "167ebc6909-cerf_quicksave_types.h"

using kotor_menu_window_cells::manager_007a39fc;
using frame_control::resource_manager_007a39e8;

namespace {

union CerfCallbackSlot {
    void* raw;
    int(__cdecl* fn)(int, int, int, int);
};

inline void* client_manager() {
    return manager_007a39fc;
}
inline void* client_internal() {
    return ldptr(client_manager(), 4);
}

}  // namespace

// 005DD350 — CExoLinkedList<CERFString>::CExoLinkedList. ECX=this; 0 args; RET 0;
// EAX=this (original 005DD38C/005DD3A6).
void* __fastcall cerf_list_construct_005dd350(void* self) {
    void* raw = live_operator_new(0xC);
    void* internal = nullptr;
    if (raw != nullptr) {
        try {
            internal = cerf_construct_list_internal(raw);
        } catch (...) {
            live_free(raw);
            throw;
        }
    }
    stptr(self, 0, internal);
    return self;
}

// 005DD9C0 — CERFFile::CERFFile. ECX=this; 0 args; RET 0; EAX=this
// (original 005DDA3D).
void* __fastcall cerf_construct_cerf_file_005dd9c0(void* self) {
    (void)new (self) CERFFile();
    return self;
}

// 005DE3A0 — CERFFile::~CERFFile. ECX=this; 0 args; RET 0.
void __fastcall cerf_destroy_cerf_file_005de3a0(void* self) {
    static_cast<CERFFile*>(self)->~CERFFile();
}

// 005DCE50 — CERFFile::Read. ECX=this; one path arg; RET 4. Always returns 1.
int __fastcall cerf_read_cerf_file_005dce50(void* self, void*, void* path) {
    CExoFileLocal file;
    {
        CExoStringLocal open_mode("rb");
        file.construct(path, 0x809, open_mode.get());
    }
    if (file.opened() == 0) return 1;

    file.seek(0, 0);
    st32(self, 0xC4, static_cast<std::uint32_t>(file.size()));

    file.read(1, 8, static_cast<unsigned char*>(self) + 0x00);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x08);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x0C);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x10);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x14);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x18);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x1C);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x20);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x24);
    file.read(1, 4, static_cast<unsigned char*>(self) + 0x28);
    file.read(1, 0x74, static_cast<unsigned char*>(self) + 0x2C);
    return 1;
}

// 005DD3C0 — CERFFile::ReadHeaderVariance. ECX=this; one path arg; RET 4.
// Three bounded loops; each node is pre-initialized before its fixed reads.
int __fastcall cerf_read_header_variance_005dd3c0(void* self, void*, void* path) {
    CExoFileLocal file;
    {
        CExoStringLocal open_mode("rb");
        file.construct(path, 0x809, open_mode.get());
    }
    if (file.opened() == 0) return 1;
    file.seek(0xA0, 0);

    const std::uint32_t string_count = ld32(self, 0x08);
    const std::uint32_t resource_count = ld32(self, 0x10);
    void* const strings = ldptr(self, 0xA0);
    void* const resources = ldptr(self, 0xA4);
    void* const locstrings = ldptr(self, 0xA8);

    for (std::uint32_t i = 0; i < string_count; ++i) {
        void* const node = live_operator_new(0xC);
        if (node != nullptr) {
            st32(node, 0, 0);
            st32(node, 4, 0);
            st32(node, 8, 0);
        }
        (void)cerf_c_exofile_read_string_record_005dd2b0(node, nullptr, &file);
        (void)cerf_list_add_tail(strings, nullptr, node);
    }

    for (std::uint32_t i = 0; i < resource_count; ++i) {
        void* const node = live_operator_new(0x18);
        if (node != nullptr) {
            std::memset(node, 0, 0x10);
            st32(node, 0x10, 0);
            st16(node, 0x14, 0xFFFF);
            st16(node, 0x16, 0);
        }
        (void)file.read(1, 0x10, node);
        (void)file.read(1, 4, static_cast<unsigned char*>(node) + 0x10);
        (void)file.read(1, 2, static_cast<unsigned char*>(node) + 0x14);
        (void)file.read(1, 2, static_cast<unsigned char*>(node) + 0x16);
        (void)cerf_list_add_tail(resources, nullptr, node);
    }

    for (std::uint32_t i = 0; i < resource_count; ++i) {
        void* const node = live_operator_new(0xC);
        if (node != nullptr) {
            st32(node, 0, 0);
            st32(node, 4, 0);
            st32(node, 8, 0);
        }
        (void)file.read(1, 4, node);
        (void)file.read(1, 4, static_cast<unsigned char*>(node) + 4);
        (void)cerf_list_add_tail(locstrings, nullptr, node);
    }
    return 1;
}

// 005DD710 — CERFFile::ExportFilesFromERF. ECX=this; six stack args; RET 0x18.
// The original does not assign EAX on open failure either: the dtor overwrites
// the je's zero. The export is void; callers ignore EAX.
void __fastcall cerf_export_files_from_erf_005dd710(
    void* self, void*, void* path, void* out_name, int flags, void* callback,
    int ctx1, int ctx2) {
    CExoFileLocal input;
    {
        CExoStringLocal open_mode("rb");
        input.construct(path, 0x809, open_mode.get());
    }
    if (input.opened() == 0) return;

    CExoStringLocal destination;  // [esp+28h] 005DD781, assigned per resource
    const std::uint32_t count = ld32(self, 0x10);
    void* pos_resources = ldptr(ldptr(self, 0xA4), 0);  // first node, not list
    void* pos_locstrings = ldptr(ldptr(self, 0xA8), 0);  // first node, not list
    CerfCallbackSlot cb;
    cb.raw = callback;

    for (std::uint32_t index = 0; index < count; ++index) {
        void* const resource = cerf_list_at_pos(pos_resources);
        void* const locstring = cerf_list_at_pos(pos_locstrings);

        {
            // 005DD7D6..005DD821: name = CExoString(resref chars); destination = out_name + name (result
            // temporary [esp+48h]); the temporary, then the name, are destroyed.
            CExoStringLocal name(reinterpret_cast<const char*>(resource));
            CExoStringTemp sum;
            destination.assign(sum.concat(out_name, name.get()));
        }

        void* const raw = live_operator_new(4);
        {
            CExoFileRaw output;
            output.attach(raw);
            if (raw != nullptr) {
                CExoStringLocal write_mode("wb");
                output.construct(destination.get(),
                                 static_cast<int>(ld16(resource, 0x14)),
                                 write_mode.get());
            }
            (void)cerf_cerf_res_copy_to_file_005dd170(
                locstring, nullptr, input.get(), output.get(),
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(flags)));
            (void)output.flush();

            if (cb.raw != nullptr) {
                (void)cb.fn(static_cast<int>(index), static_cast<int>(count),
                            ctx1, ctx2);
            }
        }

        (void)cerf_list_next(&pos_resources);
        (void)cerf_list_next(&pos_locstrings);
    }

    if (cb.raw != nullptr) {
        (void)cb.fn(static_cast<int>(ld32(self, 0x10)),
                    static_cast<int>(ld32(self, 0x10)), ctx1, ctx2);
    }
}

// 005DD2B0 — CERFString::Read. ECX=this; one CExoFile*; RET 4. Always returns 1.
int __fastcall cerf_c_exofile_read_string_record_005dd2b0(void* self, void*,
                                                          void* file) {
    CExoStringLocal text;
    (void)exofile_read_005e6960(file, nullptr, self, 4, 1);           // 005DD2DA..005DD2E9
    (void)exofile_read_005e6960(file, nullptr,
                                static_cast<unsigned char*>(self) + 4, 4, 1);  // 005DD2EE..005DD2F8
    const int length = static_cast<int>(ld32(self, 4));
    (void)cerf_c_exofile_read_string_005e6970(file, nullptr, text.get(), length);
    (void)cerf_cerf_string_set_text_005dd240(self, nullptr, text.get());
    return 1;
}

// 0062FBE0 — CGuiInGame::CopyQuickSaveGameToFutureGame.
// ECX not read; 2 stack args; RET 8. Allocation failure skips the ctor and keeps
// file=null (the later Read then faults exactly as the original).
int __stdcall cerf_copy_quick_save_game_to_future_game_0062fbe0(
    int save_id, void* name) {
    // Locals in the original frame order: [18h] path, [20h] game, [10h] erf path, [40h] (never used).
    CExoStringLocal path;
    CExoStringLocal name_text;
    CExoStringLocal erf_path;
    CExoStringLocal unused_40;
    if (save_id != -1) {
        cerf_startup_format(name_text.get(), "%06d - %s", save_id,
                            WindowCStr005e5670(name, nullptr));    // 0062FC47..0062FC5D
    } else {
        name_text.assign(name);                                     // 0062FC62..0062FC67
    }

    path.assign_text("SAVES:");                                     // 0062FC6C..0062FC75
    {
        CExoStringTemp sum;                                         // [esp+28h]
        path.assign(sum.concat(path.get(), name_text.get()));       // 0062FC7A..0062FCA4 path = path + game
    }

    void* const resman = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(resource_manager_007a39e8));
    {
        CExoStringLocal future_dir("FUTUREGAME:");                  // 0062FCA9..0062FCD4
        (void)gui_load_resman_create_directory_004068c0(resman, nullptr,
                                                        future_dir.get());
    }
    {
        CExoStringLocal future_dir("FUTUREGAME:");                  // 0062FCD9..0062FD08
        (void)gui_load_resman_clean_directory_00409460(resman, nullptr,
                                                       future_dir.get(), nullptr,
                                                       nullptr);
    }
    {
        CExoStringLocal savegame("SAVEGAME");                       // [esp+28h] 0062FD0D
        CExoStringLocal backslash("\\");                           // [esp+8]   0062FD1B
        CExoStringTemp first;                                       // [esp+38h]
        void* const left = first.concat(path.get(), backslash.get());  // 0062FD2E..0062FD41
        CExoStringTemp second;                                      // [esp+30h]
        erf_path.assign(second.concat(left, savegame.get()));       // 0062FD46..0062FD66
    }                                                               // 0062FD6B..0062FD9D: second, first, "\\", SAVEGAME
    (void)unused_40;

    CERFFileHolder file_holder;
    void* const cerf = file_holder.allocate_and_construct();
    (void)cerf_read_cerf_file_005dce50(cerf, nullptr, erf_path.get());           // 0062FDC9..0062FDD4
    (void)cerf_read_header_variance_005dd3c0(cerf, nullptr, erf_path.get());     // 0062FDD9..0062FDE0

    CExoStringLocal future_dir("FUTUREGAME:");                                  // [esp+28h] 0062FDE5

    // Original 0062FE08 does movzx edx,al: only the low byte of the accessor
    // result becomes ctx2 for the progress callback.
    const int step =
        kotor_fastcall_005edff0(client_internal(), nullptr, 0) & 0xFF;
    union UpdateSlot {
        void* raw;
        int(__cdecl* fn)(int, int, int, int);
    } callback;
    callback.fn = &cerf_load_screen_update_006c8750;
    cerf_export_files_from_erf_005dd710(
        cerf, nullptr, erf_path.get(), future_dir.get(), -1, callback.raw, 0,
        step);                                                                  // 0062FE0B..0062FE21
    return 1;
}
