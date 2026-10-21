// Authored packet source — reviewed, unbuilt; provider/EH gates remain.
//
// 005A4010 CSWSDialog::SendDialogEntry — thiscall, 4 stack args, ret 0x10.
//
// Two original top-level paths:
//   * reply_id != -1: resolve listener/speaker, SendDialogEntryNode, status 10,
//     optional field17 -> SendDialogReplies, return.
//   * reply_id == -1: entry bounds, speaker, per-player SendDialogEntryNode +
//     AddJournalEntry, player-id array, plot XP, sound/alias probe, camera
//     dialog fallback, delay, RunScript, current entry, prior-core 004CD500.
#include "a4d5785d2e-dialog_dependencies_contracts.hpp"

using namespace dialog_dep;

namespace {

struct LiveString {
    CExoString s;
    bool live = false;
    void arm() { live = true; }
    void destroy_now() {
        if (live) {
            live = false;
            str_destroy(&s);
        }
    }
    ~LiveString() { if (live) str_destroy(&s); }
};

struct CClientOptionsView {
    U8 pad[0x14];
    U8 bit_flags_2;  // +0x14
};

struct CGuiInGameView {
    U8 pad_c0[0xC0];
    U32 field48_0xc0;
    U8 pad_c4[0xBB0 - 0xC4];
    U32 field135_0xbb0;
    U32 field136_0xbb4;
};

inline U32* get_speaker_id(CSWSDialog* self, CSWSObject* object,
                           CExoString* out) {
    return static_cast<U32*>(kotor_fastcall_0059fc30(self, nullptr, object, out));
}

inline U32 get_player_language(CSWSDialog* self, U32 player_id, U32 /*link_flag*/,
                               U8 /*field21*/) {
    (void)self;
    return kotor_fastcall_004ae810(load_server(), nullptr, player_id);
}

}  // namespace

extern "C" int __fastcall kotor_fastcall_005a4010(
    void* self_bits, void*, void* object_bits, U32 reply_id, U32 entry_id,
    U32* status) {
    auto* self = static_cast<CSWSDialog*>(self_bits);
    auto* object = static_cast<CSWSObject*>(object_bits);
    CSWSDialogEntry* const current = &self->entries[self->field19_0x70];
    int result = 0;

    if (reply_id != 0xFFFFFFFFu) {
        U32 speaker_id = 0x7F000000u;
        LiveString listener;
        str_copy(&listener.s, &current->node.camera.listener);
        listener.arm();
        if (str_ne(&listener.s, "")) {
            if (str_nc_eq_chars(&listener.s, "OWNER") != 0) {
                speaker_id = object->id;
            } else if (str_nc_eq_chars(&listener.s, "PLAYER") != 0) {
                speaker_id = kotor_fastcall_004aea40(load_server(), nullptr);
            } else {
                U32* const speaker = get_speaker_id(self, object, &listener.s);
                if (speaker != nullptr) {
                    speaker_id = speaker[1];
                }
            }
        }

        CSWSObject* speaker_obj = object;
        if (str_ne(&current->node.base.speaker, "") &&
            exostring_compare_no_case_005e6450(
                &object->tag, nullptr, &current->node.base.speaker) == 0) {
            speaker_obj = static_cast<CSWSObject*>(
                kotor_fastcall_0059fc30(self, nullptr, object,
                                        &current->node.base.speaker));
            if (speaker_obj == nullptr) {
                listener.destroy_now();
                return 0;
            }
        }

        result = kotor_fastcall_005a13d0(
            self, nullptr, reply_id, object->id, speaker_obj->id, speaker_id,
            self->player_object_id, self->field21_0x78, object, current, status);
        if (*status == 10) {
            listener.destroy_now();
            return 1;
        }
        if (result != 0 && self->field17_0x68 != 0) {
            result = kotor_fastcall_005a3820(self, nullptr, object, reply_id);
        }
        listener.destroy_now();
        return result;
    }

    if (entry_id >= self->entries_count) {
        return 0;
    }
    CSWSDialogEntry* const entry = &self->entries[entry_id];

    CSWSObject* speaker_obj = object;
    if (str_ne(&entry->node.base.speaker, "") &&
        exostring_compare_no_case_005e6450(
            &object->tag, nullptr, &entry->node.base.speaker) == 0) {
        speaker_obj = static_cast<CSWSObject*>(
            kotor_fastcall_0059fc30(self, nullptr, object,
                                    &entry->node.base.speaker));
        if (speaker_obj == nullptr) {
            return 0;
        }
    }

    U32 speaker_id = 0x7F000000u;
    LiveString listener;
    str_copy(&listener.s, &entry->node.camera.listener);
    listener.arm();
    if (str_ne(&listener.s, "")) {
        if (str_nc_eq_chars(&listener.s, "OWNER") != 0) {
            speaker_id = object->id;
        } else if (str_nc_eq_chars(&listener.s, "PLAYER") != 0) {
            speaker_id = kotor_fastcall_004aea40(load_server(), nullptr);
        } else {
            U32* const speaker = get_speaker_id(self, object, &listener.s);
            if (speaker != nullptr) {
                speaker_id = speaker[1];
            }
        }
    }

    self->field17_0x68 = 0;
    CExoLinkedListInternal* const list = self->players;
    U32* player_ids = nullptr;

    CExoLinkedListNode* cursor = list->head;
    CSWSDialogPlayer* player =
        (list->head != nullptr)
            ? static_cast<CSWSDialogPlayer*>(list->head->data)
            : nullptr;
    while (player != nullptr) {
        (void)kotor_fastcall_005a13d0(
            self, nullptr, player->field0, object->id, speaker_obj->id,
            speaker_id, self->player_object_id, self->field21_0x78, object,
            entry, status);
        if (*status == 10) {
            listener.destroy_now();
            return 1;
        }
        (void)kotor_fastcall_0059ef00(self, nullptr, &entry->node.base.quest,
                                      entry->node.base.quest_entry,
                                      player->field0);
        player = static_cast<CSWSDialogPlayer*>(
            unresolved_list_next_005e9b90(list, nullptr, &cursor));
    }

    if (list->head != nullptr) {
        player_ids = static_cast<U32*>(live_operator_new(list->count * 4));
        cursor = list->head;
        player = static_cast<CSWSDialogPlayer*>(list->head->data);
        U32* out = player_ids;
        while (player != nullptr) {
            *out++ = player->field0;
            player = static_cast<CSWSDialogPlayer*>(
                unresolved_list_next_005e9b90(list, nullptr, &cursor));
        }
    }

    (void)kotor_fastcall_0059f3c0(self, nullptr, entry->node.base.plot_index,
                                  entry->node.base.plot_xp_percentage);

    U32 sound_loaded = 0;
    LiveString sound_name;
    LiveString resolved_name;

    const U32 server_id =
        kotor_fastcall_005eda50(load_client(), nullptr, speaker_obj->id);
    void* const gui0 = frame_R4_005ed690(load_client(), nullptr);
    sound_loaded = static_cast<U32>(
        kotor_fastcall_0062e1d0(gui0, nullptr, server_id, &entry->node.base.sound));

    const int module_loaded =
        kotor_fastcall_005ee0d0(load_client(), nullptr, 8);
    if (module_loaded == 1) {
        if (kotor_cell_disable_sound_00832894 == 0) {
            sound_loaded = (entry->node.base.sound_exists & 1) != 0;
        }
    } else if (kotor_cell_disable_sound_00832894 == 0) {
        sound_loaded = 1;
        str_default(&sound_name.s);
        sound_name.arm();
        c2da_ref_to_string_00405f70(&entry->node.base.sound, nullptr,
                                    &sound_name.s);
        const char c0 = str_at(&sound_name.s, 0);
        const U32 length = str_length(&sound_name.s);
        if ((c0 == 'n' || c0 == 'N') && length == 0x10 &&
            str_at(&sound_name.s, 1) != '_') {
            char part1[8] = {};
            char part2[8] = {};
            for (int i = 0; i < 5; ++i) {
                part1[i] = str_at(&sound_name.s, 1 + i);
            }
            part1[5] = '\0';
            for (int i = 0; i < 6; ++i) {
                part2[i] = str_at(&sound_name.s, 6 + i);
            }
            part2[6] = '\0';
            kotor_reconstruction::StartupFormat005e5680(
                sstring(&sound_name.s), "HD0:STREAMWAVES\\%s\\%s\\%s", part1,
                part2, str_text(&sound_name.s));
        } else {
            kotor_reconstruction::StartupFormat005e5680(
                sstring(&sound_name.s), "HD0:STREAMWAVES\\%s",
                str_text(&sound_name.s));
        }

        str_default(&resolved_name.s);
        resolved_name.arm();
        // Exact selected 1568 donor: ECX=alias holder, stack out,name,type.
        // Original type is 8 for MP3 and 4 for WAV (5A45C1/5A4617).
        {
            LiveString alias_temp;
            CExoString* const r = reinterpret_cast<CExoString*>(
                path_resolution::resolve_005e68a0(
                    static_cast<alias_path::AliasListView*>(load_alias_list()),
                    nullptr, sstring(&alias_temp.s),
                    sstring(&sound_name.s), 8 /* MP3 */));
            alias_temp.arm();
            str_assign(&resolved_name.s, r);
            alias_temp.destroy_now();
        }
        FILE* file = std::fopen(str_text(&resolved_name.s), "r");
        if (file == nullptr) {
            LiveString alias_temp;
            CExoString* const r = reinterpret_cast<CExoString*>(
                path_resolution::resolve_005e68a0(
                    static_cast<alias_path::AliasListView*>(load_alias_list()),
                    nullptr, sstring(&alias_temp.s),
                    sstring(&sound_name.s), 4 /* WAV */));
            alias_temp.arm();
            str_assign(&resolved_name.s, r);
            alias_temp.destroy_now();
            file = std::fopen(str_text(&resolved_name.s), "r");
            if (file != nullptr) {
                std::fclose(file);
            } else {
                sound_loaded = 0;
            }
        } else {
            std::fclose(file);
        }
    }

    resolved_name.destroy_now();
    sound_name.destroy_now();

    if (sound_loaded == 0) {
        auto* const options = static_cast<CClientOptionsView*>(
            kotor_fastcall_005ed700(load_client(), nullptr));
        if ((options->bit_flags_2 & 4) == 0 &&
            entry->node.base.field6_0x2c == 0) {
            void* const gui = frame_R4_005ed690(load_client(), nullptr);
            if (kotor_fastcall_0062abd0(gui, nullptr) != 0) {
                auto* const gui_view = static_cast<CGuiInGameView*>(gui);
                gui_view->field135_0xbb0 = 1;
                if (gui_view->field136_0xbb4 != 0) {
                    kotor_fastcall_0062b130(gui, nullptr);
                }
            }
        }
    }

    if (player_ids != nullptr) {
        live_free(player_ids);
    }

    if (list->head != nullptr) {
        self->field31_0x94 = speaker_obj->id;
    }

    if (sound_loaded == 0 && (entry->node.base.wait_flags & 0x10) == 0) {
        if (entry->node.base.field5_0x28 != -1) {
            const U32 link_flag = entry->link_entry_count != 0;
            (void)kotor_fastcall_0059ff50(
                self, nullptr, object, &entry->node.base.text, 0, link_flag, 0);
        } else {
            void* const gui = frame_R4_005ed690(load_client(), nullptr);
            if (*reinterpret_cast<U32*>(static_cast<U8*>(gui) + 0xC0) != 0) {
                kotor_fastcall_004cb1a0(object, nullptr, 0);
            } else {
                const U32 link_flag = entry->link_entry_count != 0;
                const U8 field21 = self->field21_0x78;
                const U32 language = get_player_language(
                    self, self->player_object_id, link_flag, field21);
                const float estimate = kotor_fastcall_005ea270(
                    &entry->node.base.text, nullptr, language,
                    static_cast<U32>(field21));
                const U32 delay = static_cast<U32>(static_cast<int>(estimate));
                (void)kotor_fastcall_0059ff50(
                    self, nullptr, object, &entry->node.base.text, delay,
                    link_flag, 0);
            }
        }
    } else {
        const U32 link_flag = entry->link_entry_count != 0;
        (void)kotor_fastcall_0059ff50(
            self, nullptr, object, &entry->node.base.text, entry->node.base.delay,
            link_flag, 0);
    }

    object->field23_0x5c = entry->node.base.wait_flags;
    (void)kotor_fastcall_0059ed70(self, nullptr, object, &entry->node.base.script);
    self->field19_0x70 = entry_id;
    result = kotor_fastcall_004cd500(object, nullptr);
    listener.destroy_now();
    return result;
}
