// 00633C50 — CGuiInGame::DoQuickLoad (1,156 bytes, 0x00633C50..0x006340D4).
//
// Original-byte + CFG reference: EVIDENCE/function-00633C50.disasm.txt and
// EVIDENCE/cfg-00633C50.txt (prior immutable packet).  This unit implements only the
// owned 00633C50 address.  It binds accepted shared providers and declares the
// bind-only contracts; it authors no shared type, global, vtable or CExoString body.
//
// Exact accepted 0769 import:
//   ?do_quick_load_00633c50@kotor_input_required@@YIXPAX0@Z
//
// ABI: __fastcall(ECX=CGuiInGame*, EDX unused, 0 stack args), RET 0, void.
// Quirks preserved: operator_new null is not guarded before LoadData; self+0x38,
// self+0x98 and the entry vtable are not null-guarded; 00832CA8 is read, never written.
#include "04de266238-ui_save_state_types.h"

// The vector element destructor used by the accepted EH vector helper is the exact
// accepted 0012 dtor.  A member destructor's address cannot be taken in standard C++,
// so bind the fastcall-shaped callback symbol to the exact accepted member body at
// link time.  This is a symbol binding, not a new body, and not an address alias.
#pragma comment(linker, "/alternatename:@startup_string_element_destroy_005e5c20@8=?destroy_005e5c20@String@kotor_menu_10_19@@QAEXXZ")

using kotor_menu_10_19::String;
using kotor_reconstruction::RawString;
using kotor_reconstruction::StartupString;

namespace kotor_input_required {
namespace {

struct DirectoryList {
    void* base;
    std::uint32_t count;
    std::uint32_t capacity;
};
static_assert(sizeof(DirectoryList) == 12, "GetDirectoryList vector triple");

// EH-safe cleanup guard.  Constructed before strA so the destructor order is
// strA (and any inner CExoString) first, then the GetDirectoryList vector, on
// both the normal return and an unwind.  This preserves the original unwind
// destruction of the vector instead of making cleanup normal-return-only.
struct DirectoryListGuard {
    DirectoryList* list;
    ~DirectoryListGuard() {
        if (list->base != nullptr) {
            std::int32_t const count = ui_load_at<std::int32_t>(
                static_cast<unsigned char*>(list->base) - 4, 0);
            c2da_destroy_array_006fa852(
                list->base, 8u, count, &startup_string_element_destroy_005e5c20);
            _live_free(static_cast<unsigned char*>(list->base) - 4);
        }
    }
};

using EntryDeletingDestructor = void* (__fastcall*)(void*, void*, unsigned int);
union EntryDeletingDestructorSlot {
    void* raw;
    EntryDeletingDestructor fn;
};

using MessageBoxSetMessage = void (__fastcall*)(void*, void*, RawString);
union MessageBoxSetMessageSlot {
    void* raw;
    MessageBoxSetMessage fn;
};

inline void* vector_element(void* base, std::int32_t index) {
    return static_cast<unsigned char*>(base) + static_cast<std::size_t>(index) * 8u;
}

inline void call_entry_deleting_destructor(void* entry) {
    void* const vptr = ui_load_at<void*>(entry, 0);
    void** const methods = reinterpret_cast<void**>(vptr);
    EntryDeletingDestructorSlot slot;
    slot.raw = methods[0];
    slot.fn(entry, nullptr, 1u);
}

}  // namespace

void __fastcall do_quick_load_00633c50(void* self, void*) {
    // Original locals: strA spans the whole body; strC is re-created at each site.
    // Scopes preserve the original destruction order (strC/strD before strA, then the
    // GetDirectoryList vector via the EH-safe guard).
    DirectoryList vec = {nullptr, 0u, 0u};
    DirectoryListGuard vec_guard{&vec};
    void* entry = nullptr;
    bool load_path = false;

    {
        StartupString strA;

        // A1: CExoString("SAVES:"), GetDirectoryList(&vec,&strC,0xFFFF,1,1).
        {
            StartupString strC("SAVES:");
            void* const base = kotor_menu_10_19::dependencies::base_007a39e0;
            f200_ports::Enumerate005e6640(base, nullptr, &vec, &strC, 0xFFFFu, 1u, 1u);
        }

        // A2/A3: CreateDirectory2("TEMP:"); if it returned 0, CleanDirectory("TEMP:",0,0).
        bool clean_temp = false;
        {
            StartupString strC("TEMP:");
            void* const resman = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(frame_control::resource_manager_007a39e8));
            clean_temp =
                gui_load_resman_create_directory_004068c0(resman, nullptr, &strC) == 0;
        }
        if (clean_temp) {
            StartupString strC("TEMP:");
            void* const resman = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(frame_control::resource_manager_007a39e8));
            gui_load_resman_clean_directory_00409460(resman, nullptr, &strC, 0u, 0u);
        }

        // B: scan the list; the first entry with number==0 enters the load path
        // without being deleted.  operator_new NULL is deliberately not guarded.
        std::int32_t const count = static_cast<std::int32_t>(vec.count);
        if (count > 0) {
            for (std::int32_t i = 0; i < count; ++i) {
                void* const memory = _live_operator_new(kotor_m1_saveload_offsets::object_size);
                entry = (memory != nullptr)
                            ? kotor_m1_saveload_entry_construct_006cb940(memory, nullptr)
                            : nullptr;
                kotor_m1_saveload_entry_load_data_006c8e50(
                    entry, nullptr,
                    reinterpret_cast<StartupString*>(vector_element(vec.base, i)));
                if (ui_load_at<std::uint32_t>(entry, kotor_m1_saveload_offsets::number) == 0u) {
                    load_path = true;
                    break;
                }
                call_entry_deleting_destructor(entry);  // entry->vtable[0](entry,1)
            }
        }

        if (!load_path) {
            // C: no zero-number entry.  RemoveResourceDirectory("TEMP:"), then the
            // normal cleanup path (strA then vector) follows this scope.
            StartupString strD("TEMP:");
            void* const resman = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(frame_control::resource_manager_007a39e8));
            kotor_fastcall_004088d0(resman, nullptr, &strD);
        } else {
            // D: load path.
            void* const app = kotor_menu_window_cells::manager_007a39fc;
            void* const internal = ui_load_at<void*>(app, 4);

            menu_tables_required::SoundSetMode_005d5e80(
                reinterpret_cast<void*>(
                    static_cast<std::uintptr_t>(frame_control::sound_manager_007a39ec)),
                nullptr, 0u);
            load_screen_busy_wait_mute_sound_005ee130(internal, nullptr);

            bool confirmed = false;
            if ((ui_load_at<std::uint8_t>(entry, kotor_m1_saveload_offsets::entry_flags) & 0x20u) != 0u) {
                if (kotor_cell_00832ca8 == 0u) {
                    confirmed = true;

                    // Confirmation page.  self+0x98 is not null-guarded.
                    void* const gui = get_in_game_gui_005ed690(internal, nullptr);
                    void* const box = ui_load_at<void*>(gui, 0x98);
                    kotor_fastcall_00627130(box, nullptr, 0u);
                    gui_load_messagebox_set_callback_00624a40(box, nullptr, 0u, 0u, 0u);

                    {
                        StartupString strC;  // default ctor at 00633E8B
                        void* const tlk = kotor_cell_tlk_007a3a08;

                        // GetSimpleString(&tmpOut,0) constructs tmpOut; the original
                        // does not default-construct it and destroys it after Assign.
                        alignas(8) unsigned char tmp_storage[8];
                        String* const tmp =
                            reinterpret_cast<kotor_menu_10_19::Tlk*>(tlk)
                                ->simple_0041e8f0(
                                    reinterpret_cast<String*>(tmp_storage), 0u);
                        strC.Assign005e5c50(reinterpret_cast<StartupString*>(tmp));
                        // The original destroys tmpOut with the real 005E5C20
                        // destructor (no delete flag; call at 00633EBE), not the
                        // scalar deleting destructor 005B0F20.  Reuse the already
                        // reviewed 005E5C20 element-destroy binding so this TU does
                        // not emit a duplicate ??_GStartupString COMDAT or ??3.
                        startup_string_element_destroy_005e5c20(
                            reinterpret_cast<StartupString*>(tmp_storage), nullptr);

                        kotor_fastcall_0041db50(
                            tlk, nullptr, 0u,
                            reinterpret_cast<StartupString*>(
                                static_cast<unsigned char*>(entry) + kotor_m1_saveload_offsets::live_missing));
                        tlk_core::ParseStr0041dd30(
                            reinterpret_cast<kotor_menu_10_19::Tlk*>(tlk), nullptr, &strC);

                        // box->vtable[+0x70](box, CExoString by value).  The accepted
                        // copy ctor makes the deep copy; the callee destroys the arg.
                        {
                            void* const vptr = ui_load_at<void*>(box, 0);
                            void** const methods = reinterpret_cast<void**>(vptr);
                            MessageBoxSetMessageSlot slot;
                            slot.raw = methods[0x70 / 4];
                            alignas(8) unsigned char copied[8];
                            WindowCopy005e5b00(
                                reinterpret_cast<StartupString*>(copied), nullptr, &strC);
                            slot.fn(box, nullptr, *reinterpret_cast<RawString*>(copied));
                        }

                        void* const manager = ui_load_at<void*>(self, 0x38);
                        kotor_fastcall_0040bc70(manager, nullptr, box, 1u, 1u);
                    }
                }
            }

            if (!confirmed) {
                // E: load-screen path.  ResetCurrentSessionStartTime is called with
                // whatever ECX the preceding call leaves; no receiver is loaded.
                kotor_m15_party_reset_session_start_time_00563cf0();

                void* const gui = get_in_game_gui_005ed690(internal, nullptr);
                std::uint32_t const d8 = ui_load_at<std::uint32_t>(gui, 0xD8);
                ui_store_at<std::uint32_t>(gui, 0xD8, d8 + 1u);

                kotor_m15_client_set_load_screen_by_module_005edcf0(
                    internal, nullptr,
                    reinterpret_cast<StartupString*>(
                        static_cast<unsigned char*>(entry) +
                        kotor_m1_saveload_offsets::last_module));  // &last_module[0]

                {
                    StartupString strC("");
                    load_screen_set_load_music_005edd30(internal, nullptr, &strC);
                }

                kotor_m15_client_show_load_screen_005eda80(
                    internal, nullptr, 0u, 1u, 0u);

                unsigned int const gameplay_hint =
                    ui_load_at<std::uint8_t>(entry, kotor_m1_saveload_offsets::gameplay_hint);
                load_screen_set_gameplay_hint_005ee060(
                    internal, nullptr, gameplay_hint);

                unsigned int const story_hint =
                    ui_load_at<std::uint8_t>(entry, kotor_m1_saveload_offsets::story_hint);
                load_screen_set_story_hint_005ee080(internal, nullptr, story_hint);

                {
                    StartupString strC("");
                    unsigned int const hint =
                        load_screen_get_next_load_screen_hint_strref_005ee090(
                            internal, nullptr, &strC, 2u);
                    kotor_m15_client_set_load_screen_hint_005edf00(
                        internal, nullptr, hint);
                }

                load_screen_set_load_screen_hint_update_timer_005ee040(
                    internal, nullptr, 0u);

                int const ok = load_screen_unpack_quick_save_game_006323a0(
                    self, nullptr,
                    static_cast<int>(ui_load_at<std::uint32_t>(entry, kotor_m1_saveload_offsets::number)),
                    static_cast<unsigned char*>(entry) + kotor_m1_saveload_offsets::dir_suffix);

                if (ok != 0) {
                    // Success: pop every modal panel, hide the SW in-game GUI, tell
                    // the server this is a save-game load, send the request and
                    // delete the entry.  The original reloads [self+0x38] on every
                    // iteration (0x63406B/0x63406F), so do not cache the manager.
                    for (;;) {
                        void* const manager_now = ui_load_at<void*>(self, 0x38);
                        if (ui_load_at<void*>(manager_now, 0x98) == nullptr) {
                            break;
                        }
                        kotor_fastcall_0040be00(manager_now, nullptr);
                    }

                    void* const in_game = get_in_game_gui_005ed690(internal, nullptr);
                    frame_R4_0062cba0(in_game, nullptr, 1u);

                    void* const server = ui_load_at<void*>(app, 8);
                    load_screen_set_load_from_save_game_004af060(
                        server, nullptr, 1u);

                    load_screen_send_load_game_request_005ed680(
                        internal, nullptr,
                        static_cast<int>(ui_load_at<std::uint32_t>(entry, kotor_m1_saveload_offsets::number)),
                        static_cast<unsigned char*>(entry) + kotor_m1_saveload_offsets::dir_suffix,
                        static_cast<unsigned char*>(entry) +
                        kotor_m1_saveload_offsets::last_module);  // &last_module[0]

                    call_entry_deleting_destructor(entry);  // vtable[0](entry,1)
                }
            }
        }
    }
}

}  // namespace kotor_input_required
