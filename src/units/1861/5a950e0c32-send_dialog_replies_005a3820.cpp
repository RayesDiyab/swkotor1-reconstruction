// Authored packet source — reviewed, unbuilt; provider/EH gates remain.
//
// 005A3820 CSWSDialog::SendDialogReplies — thiscall, 2 stack args, ret 8.
//
// Two-counter structural translation of the original:
//   * temp array of link_entry_count+1 CSWSDialogReply (header count at -4)
//   * ascending active region at arr[1..]
//   * descending "appended" region at arr[n-1..], then compacted to
//     arr[1+active..] by 0x5A3BFE..0x5A3CBE
//   * each selected entry's field3 is the link-entry ordinal, not the source
//     reply's field3
//   * array destructor count is the header n, not the selected count
#include "a4d5785d2e-dialog_dependencies_contracts.hpp"

using namespace dialog_dep;

namespace {

struct ReplyArray {
    CSWSDialogReply* arr = nullptr;
    U32 count = 0;
    void* header = nullptr;
    bool live = false;

    void destroy() {
        if (live) {
            c2da_destroy_array_006fa852(
                arr, 0xB4, static_cast<I32>(count),
                reinterpret_cast<void*>(&dialog_node_dtor_0059f5e0));
            live = false;
        }
        if (header != nullptr) {
            live_free(header);
            header = nullptr;
        }
    }
};

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

struct RawBlockGuard {
    void* p;
    bool live;
    explicit RawBlockGuard(void* p_) : p(p_), live(true) {}
    void release() { live = false; }
    ~RawBlockGuard() { if (live) live_free(p); }
};

void copy_base(CSWSDialogReply* dst, const CSWSDialogReply* src) {
    (void)kotor_fastcall_0059f140(dst, nullptr, src);
}

void copy_node(CSWSDialogReply* dst, const CSWSDialogReply* src) {
    (void)kotor_fastcall_005a0360(dst, nullptr, src);
}

void copy_camera(CSWSDialogReply* dst, const CSWSDialogReply* src) {
    str_assign(&dst->node.camera.listener,
               const_cast<CExoString*>(&src->node.camera.listener));
    dst->node.camera.camera_angle = src->node.camera.camera_angle;
    dst->node.camera.camera_id = src->node.camera.camera_id;
    dst->node.camera.camera_height_offset = src->node.camera.camera_height_offset;
    dst->node.camera.tar_height_offset = src->node.camera.tar_height_offset;
    dst->node.camera.camera_animation = src->node.camera.camera_animation;
    dst->node.camera.camera_fov = src->node.camera.camera_fov;
    dst->node.camera.camera_video_effect = src->node.camera.camera_video_effect;
}

void copy_base_and_camera(CSWSDialogReply* dst, const CSWSDialogReply* src) {
    copy_base(dst, src);
    copy_camera(dst, src);
}

void copy_fields(CSWSDialogReply* dst, const CSWSDialogReply* src, U32 link_index) {
    dst->field1_0xa8 = src->field1_0xa8;
    dst->field2_0xac = src->field2_0xac;
    dst->field3_0xb0 = static_cast<I32>(link_index);
}

}  // namespace

extern "C" int __fastcall kotor_fastcall_005a3820(void* self_bits, void*,
                                                   void* object_bits, U32 reply_id) {
    auto* self = static_cast<CSWSDialog*>(self_bits);
    auto* object = static_cast<CSWSObject*>(object_bits);

    if (self->field19_0x70 >= self->entries_count) {
        return 0;
    }
    CSWSDialogEntry* const entry = &self->entries[self->field19_0x70];
    const U32 count = entry->link_entry_count + 1;
    void* const header = live_operator_new(count * 0xB4 + 4);
    *static_cast<U32*>(header) = count;
    auto* const arr = reinterpret_cast<CSWSDialogReply*>(
        static_cast<U8*>(header) + 4);

    RawBlockGuard header_guard(header);
    ReplyArray array;
    array.arr = arr;
    array.count = count;
    array.header = header;
    crt_eh_vector_constructor_006fb55f(
        arr, 0xB4, static_cast<I32>(count),
        reinterpret_cast<void*>(&dialog_reply_ctor_0059f5b0),
        reinterpret_cast<void*>(&dialog_node_dtor_0059f5e0));
    header_guard.release();
    array.live = true;

    arr[0].node.base.text.internal =
        reinterpret_cast<void*>(static_cast<intptr_t>(-1));
    arr[0].field3_0xb0 = -1;

    CExoLinkedListInternal* const list = self->players;
    CExoLinkedListNode* cursor = list->head;
    CSWSDialogPlayer* player =
        (list->head != nullptr)
            ? static_cast<CSWSDialogPlayer*>(list->head->data)
            : nullptr;
    U32 saved_player_id = 0;
    bool first = true;
    int result = 1;

    auto finish = [&](int value) -> int {
        array.destroy();
        if (object->dialog != nullptr) {
            if (reply_id == 0xFFFFFFFFu) {
                self->field17_0x68 = 1;
                self->field20_0x74 = 0xFFFFFFFFu;
            } else if (self->field20_0x74 != 0xFFFFFFFFu) {
                CSWSDialogLinkEntry* const link =
                    &entry->link_entries[self->field20_0x74];
                CSWSDialogReply* const reply = &self->replies[link->index];

                LiveString listener;
                str_copy(&listener.s, &reply->node.camera.listener);
                listener.arm();
                U32 object_id = self->player_object_id;
                if (str_ne(&listener.s, "")) {
                    if (str_nc_eq_chars(&listener.s, "OWNER") != 0) {
                        object_id = object->id;
                    } else if (str_nc_eq_chars(&listener.s, "PLAYER") != 0) {
                        object_id = kotor_fastcall_004aea40(load_server(), nullptr);
                    } else {
                        U32* const speaker = static_cast<U32*>(
                            kotor_fastcall_0059fc30(self, nullptr, object, &listener.s));
                        if (speaker != nullptr) {
                            object_id = speaker[1];
                        }
                    }
                }

                LiveString text;
                (void)kotor_fastcall_0059f320(self, nullptr, &text.s,
                                              &reply->node.base.text,
                                              self->field21_0x78);
                text.arm();
                const U32 field18 = self->field18_0x6c;
                const U8 field21 = self->field21_0x78;
                const U32 server_id =
                    kotor_fastcall_005eda50(load_client(), nullptr, object_id);
                void* const gui = frame_R4_005ed690(load_client(), nullptr);
                (void)kotor_fastcall_0062e100(
                    gui, nullptr, reply_id, self->field20_0x74, self->field19_0x70,
                    &text.s, server_id, field21, static_cast<I32>(field18));
                text.destroy_now();
                listener.destroy_now();
            }
        }
        return value;
    };

    auto build_selection = [&](bool first_player, U32& active, U32& appended) {
        active = 0;
        appended = 0;
        for (U32 i = 0; i < entry->link_entry_count; ++i) {
            CSWSDialogLinkEntry* const link = &entry->link_entries[i];
            CSWSDialogReply* const reply = &self->replies[link->index];
            const U32 check = static_cast<U32>(
                kotor_fastcall_0059ec90(self, nullptr, object, &link->active));
            if (check == 0) {
                if (link->display_inactive != 0) {
                    CSWSDialogReply* const out = &arr[count - 1 - appended];
                    copy_base_and_camera(out, reply);
                    copy_fields(out, reply, i);
                    ++appended;
                }
            } else if (self->field29_0x8c == 0 || first_player) {
                CSWSDialogReply* const out = &arr[1 + active];
                copy_base_and_camera(out, reply);
                copy_fields(out, reply, i);
                ++active;
            } else {
                CSWSDialogReply* const out = &arr[count - 1 - appended];
                copy_node(out, reply);
                copy_fields(out, reply, i);
                ++appended;
            }
        }
        if (appended != 0) {
            for (U32 j = 0; j < appended; ++j) {
                CSWSDialogReply* const src = &arr[count - appended + j];
                CSWSDialogReply* const dst = &arr[1 + active + j];
                copy_base_and_camera(dst, src);
                dst->field1_0xa8 = src->field1_0xa8;
                dst->field2_0xac = src->field2_0xac;
                dst->field3_0xb0 = src->field3_0xb0;
            }
        }
    };

    auto send_to_player = [&](CSWSDialogPlayer* current, bool first_player,
                              U32& active, U32& appended) -> int {
        build_selection(first_player, active, appended);
        const U32 total = active + appended;
        self->player_object_id = saved_player_id;  // 0x5A3CD7, before the call
        if (total == 0) {
            self->field18_0x6c = 1;
            if (self->field29_0x8c == 0 || first_player) {
                if (reply_id == 0xFFFFFFFFu) {
                    self->field20_0x74 = 0xFFFFFFFFu;
                }
                return kotor_fastcall_005a1c00(
                    self, nullptr, current->field0, current->id, current->byte8,
                    arr, 1, 0, self->field19_0x70, 1);
            }
            if (reply_id == 0xFFFFFFFFu) {
                self->field20_0x74 = 0xFFFFFFFFu;
            }
            return kotor_fastcall_005a1c00(
                self, nullptr, current->field0, current->id, current->byte8,
                arr, 0, appended, self->field19_0x70, 1);
        }
        if (active == 1 &&
            self->replies[entry->link_entries[arr[1].field3_0xb0].index]
                    .field1_0xa8 == 0) {
            self->field18_0x6c = 1;
        }
        if (reply_id == 0xFFFFFFFFu) {
            self->field20_0x74 = 0xFFFFFFFFu;
        }
        return kotor_fastcall_005a1c00(
            self, nullptr, current->field0, current->id, current->byte8,
            arr + 1, active, appended, self->field19_0x70,
            static_cast<I32>(self->field18_0x6c));
    };

    for (;;) {
        if (player == nullptr) {
            if (reply_id != 0xFFFFFFFFu) {
                return finish(result);
            }
            U32 active = 0;
            U32 appended = 0;
            build_selection(first, active, appended);
            if (active < 1) {
                self->field18_0x6c = 1;
                result = kotor_fastcall_004cb480(
                    object, nullptr, 0xFFFFFFFFu,
                    static_cast<U32>(arr[0].field3_0xb0), 0,
                    self->field19_0x70, 0);
            } else {
                if (active == 1 &&
                    self->replies[entry->link_entries[arr[1].field3_0xb0].index]
                            .field1_0xa8 == 0) {
                    self->field18_0x6c = 1;
                }
                result = kotor_fastcall_004cb480(
                    object, nullptr, 0xFFFFFFFFu,
                    static_cast<U32>(arr[1].field3_0xb0), 0,
                    self->field19_0x70, 0);
            }
            return finish(result);
        }

        if (reply_id == 0xFFFFFFFFu || player->field0 == reply_id) {
            saved_player_id = self->player_object_id;
            self->player_object_id = player->id;
            U32 active = 0;
            U32 appended = 0;
            result = send_to_player(player, first, active, appended);
            first = false;
            player = static_cast<CSWSDialogPlayer*>(
                unresolved_list_next_005e9b90(list, nullptr, &cursor));
            if (result == 0 || cursor == nullptr) {
                return finish(result);
            }
            continue;
        }

        first = false;
        player = static_cast<CSWSDialogPlayer*>(
            unresolved_list_next_005e9b90(list, nullptr, &cursor));
        if (cursor == nullptr) {
            return finish(result);
        }
    }
}
