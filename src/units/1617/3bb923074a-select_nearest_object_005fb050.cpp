// 005FB050 — CClientExoAppInternal::SelectNearestObject (owned export).
// Full 914-byte body, all 53 branches. Original quirks preserved: inverted GetGameObject
// success flag (AL==0 found), arg2 low-byte filter reused from the stack slot, 12-byte list
// entry shifts, no added null guards, and arg1 direction 0=forward / 1=backward.
#include "78e79b69b7-party_selection_decls.h"

namespace kotor_input_required {

void __fastcall select_nearest_object_005fb050(void* self, void* /*unused_edx*/,
                                                U32 arg1, U32 arg2) {
    using namespace party_selection;

    // Original reloads [self+0x40] for the second call at 005FB065.
    if (kotor_m15_ingame_tutorial_reason_will_show_0062f420(
            at<void*>(self, 0x40), nullptr, 8) != 0 &&
        kotor_m15_ingame_set_tutorial_reason_0062b0d0(
            at<void*>(self, 0x40), nullptr, 8) != 0) {
        put<U32>(self, 0x3ac, 0x7f000000u);            // 005FB073
        put<U32>(self, 0x3b0, 0x7f000000u);            // 005FB07E
        put<U32>(self, 0x3b4, 0);                      // 005FB084
        put<U8>(self, 0x3b8, static_cast<U8>(8));      // 005FB08E
    }

    U32 count = at<U32>(self, 0x2ac);                  // 005FB095
    if (count == 0) return;                            // 005FB0A6 (EAX 0, callers ignore)

    U8 mode = static_cast<U8>(arg2);                   // low byte of arg2
    void* base = nullptr;
    int idx = 0;

    if (mode != 0) {                                   // 005FB0B1
        idx = static_cast<int>(count) - 1;             // 005FB0B7
        int stride = idx * 12;
        bool scan_found = false;

        while (idx >= 0) {                             // 005FB0BE
            // Original re-reads [self+0x2A8] for every scan step (005FB0D1).
            void* const list = at<void*>(self, 0x2a8);
            void* out = nullptr;
            const U32 id = at<U32>(list, static_cast<U32>(stride));  // 005FB0D1..D3
            const unsigned char status = kotor_fastcall_004d8230(
                at<void*>(self, 0x14), nullptr, id, &out);          // 005FB0E7
            bool remove = false;
            if (status != object_projectile_literal_0074666c[0] || out == nullptr) {  // 005FB0EC/0FE
                remove = true;
            } else {
                base = vcall<void*>(out, 0xC);         // 005FB106
                if (base == nullptr) {                 // 005FB10F
                    remove = true;
                } else {
                    // Original re-reads [self+0x2A8] at 005FB115 for the cache write.
                    unsigned char* const entry = static_cast<unsigned char*>(
                        at<void*>(self, 0x2a8)) + stride;       // 005FB115
                    if (entry[4] == 0xFF) {            // 005FB11E
                        void* const pc = kotor_fastcall_00635460(
                            at<void*>(self, 0x270), nullptr);       // 005FB127
                        entry[4] = kotor_fastcall_00617ad0(pc, nullptr, base);  // 005FB12E
                    }
                    // Original re-reads [self+0x2A8] again at 005FB13A after CanSee.
                    const unsigned char cached = at<U8>(
                        at<void*>(self, 0x2a8),
                        static_cast<U32>(stride) + 4u);     // 005FB13A
                    if (cached == 0) {                 // 005FB142
                        remove = true;
                    } else if (vcall<void*>(base, 0x2C) == nullptr) {  // 005FB14C
                        // 005FB151 -> 005FB163
                    } else if (vcall<U32>(base, 0x138) != 0) {         // 005FB159
                        scan_found = true;             // 005FB161 -> 005FB17C
                        break;
                    }
                }
            }
            if (remove) {
                kotor_fastcall_0048dde0(
                    static_cast<unsigned char*>(self) + 0x2a8, nullptr,
                    static_cast<U32>(idx));            // 005FB1B9 / 005FB1CF
            }
            --idx;                                     // 005FB167
            stride -= 12;
        }
        if (!scan_found) mode = 0;                     // 005FB177 (arg2 slot low byte)
    }

    {   // 005FB17C selection-phase index selection
        const U32 selected = at<U32>(self, 0x2b4);
        if (selected == 0x7f000000u) {                 // 005FB187
            if (arg1 == 0) {
                idx = 0;                               // 005FB1F7
            } else {
                count = at<U32>(self, 0x2ac);          // 005FB1EE
                idx = static_cast<int>(count) - 1;
            }
        } else {
            const int found = kotor_fastcall_005f4fb0(  // 005FB195
                static_cast<unsigned char*>(self) + 0x2a8, nullptr, selected,
                &kotor_cdecl_0061fa90);
            count = at<U32>(self, 0x2ac);              // 005FB1A0 after IndexOf
            if (found < 0 || found >= static_cast<int>(count)) {  // 005FB19E/A8
                if (arg1 == 0) {
                    idx = 0;                           // 005FB1F7
                } else {
                    count = at<U32>(self, 0x2ac);      // 005FB1EE
                    idx = static_cast<int>(count) - 1;
                }
            } else if (arg1 == 0) {                    // 005FB1B0
                idx = (found >= static_cast<int>(count) - 1) ? 0 : found + 1;
            } else {                                   // 005FB1B4
                idx = (found == 0) ? static_cast<int>(count) - 1 : found - 1;
            }
        }
    }

    count = at<U32>(self, 0x2ac);                      // 005FB1F9
    int budget = static_cast<int>(count);
    bool accepted = false;

    while (count != 0 && budget != 0) {                // 005FB210..21E
        // Original re-reads [self+0x2A8] at 005FB224 and clears the base/out slots.
        void* const list = at<void*>(self, 0x2a8);
        base = nullptr;
        void* out = nullptr;
        const U32 id = at<U32>(list, static_cast<U32>(idx * 12));  // 005FB224
        const unsigned char status = kotor_fastcall_004d8230(
            at<void*>(self, 0x14), nullptr, id, &out);              // 005FB24C
        bool remove = false;
        bool match = false;

        if (status != object_projectile_literal_0074666c[0] || out == nullptr) {  // 005FB251..274
            remove = true;
        } else {
            base = vcall<void*>(out, 0xC);             // 005FB26B
            if (base == nullptr) {                     // 005FB274
                remove = true;
            } else if (mode == 0) {                    // 005FB280
                match = true;
            } else if (mode == 1) {                    // 005FB284/294
                match = (vcall<U32>(base, 0x138) == 0);
            } else if (mode == 2) {                    // 005FB2C9..E8
                match = (vcall<void*>(base, 0x2C) != nullptr) &&
                        (vcall<U32>(base, 0x138) != 0);
            }

            if (match) {                               // 005FB2EA
                // Original re-reads [self+0x2A8] at 005FB2EA for the cache write.
                unsigned char* const entry = static_cast<unsigned char*>(
                    at<void*>(self, 0x2a8)) + idx * 12;         // 005FB2EA
                if (entry[4] == 0xFF) {                // 005FB2F7
                    void* const pc = kotor_fastcall_00635460(
                        at<void*>(self, 0x270), nullptr);          // 005FB304
                    entry[4] = kotor_fastcall_00617ad0(pc, nullptr, base);  // 005FB30B
                }
                // Original re-reads [self+0x2A8] again at 005FB313 after CanSee.
                const unsigned char cached = at<U8>(
                    at<void*>(self, 0x2a8),
                    static_cast<U32>(idx * 12) + 4u);  // 005FB313
                if (cached == 0) {                     // 005FB31F
                    remove = true;
                } else {
                    accepted = true;                   // 005FB321
                }
            }
        }

        if (remove) {                                  // 005FB32E
            // Original re-reads [self+0x2AC] at 005FB32E before decrementing.
            const U32 new_count = at<U32>(self, 0x2ac) - 1u;
            put<U32>(self, 0x2ac, new_count);
            if (idx < static_cast<int>(new_count)) {
                // Original re-reads [self+0x2A8] at 005FB343.
                unsigned char* dst =
                    static_cast<unsigned char*>(at<void*>(self, 0x2a8)) + idx * 12;
                for (int i = idx; i < static_cast<int>(new_count); ++i) {
                    std::memcpy(dst, dst + 12, 12);    // 005FB343..5E
                    dst += 12;
                }
            }
            count = new_count;
        }

        if (arg1 != 0) {                               // 005FB296 -> 005FB378
            --idx;
            // Original re-reads [self+0x2AC] at 005FB37B or 005FB389.
            count = at<U32>(self, 0x2ac);
            if (idx < 0) {
                idx = static_cast<int>(count) - 1;     // 005FB389
            } else if (idx >= static_cast<int>(count)) {
                idx = static_cast<int>(count) - 1;     // 005FB383 fall-through
            }
        } else {
            ++idx;                                     // 005FB2A3
            // Original re-reads [self+0x2AC] at 005FB2A3.
            count = at<U32>(self, 0x2ac);
            if (idx >= static_cast<int>(count)) idx = 0;
        }
        --budget;                                      // 005FB2AF
        if (accepted) break;                           // 005FB2BE/2C4
    }

    if (accepted) {                                    // 005FB397 -> 005FB39B
        const U32 id = at<U32>(base, 4);
        put<U32>(self, 0x2b4, id);
        kotor_fastcall_005f9c60(self, nullptr, base, 1, 1, 1);  // 005FB3CF
    } else if (at<U32>(self, 0x2ac) == 0) {            // 005FB3B1
        put<U32>(self, 0x2b4, 0x7f000000u);
        kotor_fastcall_005f9c60(self, nullptr, nullptr, 1, 0, 1);  // 005FB3CF
    }
}

}  // namespace kotor_input_required
