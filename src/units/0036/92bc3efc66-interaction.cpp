// Original Endar door/use interaction closure. Original image is read-only.
// Uses the selected W2 message and W1 action providers; no synthetic actors/input.
#include "2c75bb10c4-w5_t3.hpp"
#include "a3b6729268-w5_server_env.hpp"
#include <cstdint>
#include <cstring>
using namespace w5;
namespace kotor_input_cells_required { extern volatile std::int32_t word_008338f8; }
namespace kotor_input_required { void* __fastcall get_in_game_gui_005ed690(void*, void*); }
namespace w4 { extern const float map_note_color_007a23f0[3]; }
extern "C" {
extern const unsigned char borrowed_exit_color_007a23b4[12];
extern const float kotor_m15d_color_007a23d8[3], kotor_m15d_color_007a23e4[3];
extern const unsigned char kotor_m1_color_007a23fc[12];
void* __fastcall kotor_fastcall_005ed6f0(void*, void*);
void* __fastcall kotor_fastcall_005eda40(void*, void*);
U __fastcall kotor_fastcall_005ede70(void*, void*);
U __fastcall kotor_fastcall_005edef0(void*, void*);
void __fastcall frame_SetPaused_0062def0(void*, void*, U, U);
void __fastcall kotor_fastcall_00610a10(void*, void*, U);
void __fastcall kotor_fastcall_0063d470(void*, void*);
void __fastcall w5_object_clear_all_actions_004ccd80(void*, void*, U);
void __fastcall recovered_combat_round_remove_all_actions_004d3770(void*, void*);
void* __fastcall kotor_fastcall_005ed580(void*, void*, U);
void* __fastcall kotor_fastcall_00635460(void*, void*);
void __fastcall kotor_fastcall_0040a140(void*, void*, U);
void* __fastcall frame_GetWorldTimer_005ed4f0(void*, void*);
void __fastcall frame_GetWorldTime_004ade40(void*, void*, U*, U*);
U __fastcall recovered_world_time_difference_004ae460(void*, void*, U, U, U, U, U*, U*);
void __fastcall kotor_fastcall_004d68a0(void*, void*, U, U, U);
void __fastcall kotor_fastcall_004d68e0(void*, void*, U);
void __fastcall kotor_fastcall_004d6300(void*, void*, U, U);
U __fastcall kotor_fastcall_004d6440(void*, void*, void**, U*);
U __fastcall recovered_client_send_00677410(void*, void*, U, U, void*, U);
void* __fastcall kotor_fastcall_005edeb0(void*, void*, void*, U);
void __fastcall gui_load_text_params_set_color_00414e10(void*, void*, const void*);
void __fastcall gui_load_text_params_set_text_00415e00(void*, void*, void*) noexcept(false);
void __fastcall c2da_current_string_destroy_005e5c20(void*, void*);
void __fastcall kotor_fastcall_006887d0(void*, void*);
void __fastcall kotor_fastcall_0068ad60(void*, void*, I, U);
void __fastcall kotor_fastcall_00689610(void*, void*, I, U);
void __fastcall kotor_fastcall_00687700(void*, void*, U);
}
namespace {
void* app() { return client_app(); }
void* offset(void* p, U o) { return static_cast<unsigned char*>(p) + o; }
bool combat(void* pc) { return (at<unsigned char>(pc, 0x440) & 1) != 0; }
// The six jump-table entries are direct case numbers, not an index-byte table.
void disabled_action(void* panel, U flags, U text_field, U time_field) {
    const U reason = (flags >> 1) & 15;
    if (reason != 0) {
        static const U messages[6] = {0x96d5, 0x96d6, 0x96d7, 0xa5b6, 0xa602, 0xbb40};
        if (reason <= 6) at<U>(panel, text_field) = messages[reason - 1];
        at<U>(panel, time_field) = 0x40a00000;
    }
}
// Real MSVC member-function pointer: code at+C and this-adjustment at+10.
void invoke_action(void* action, void* target, void* pc) {
    using Callback = void (__thiscall*)(void*, U, void*);
    auto fn = reinterpret_cast<Callback>(static_cast<std::uintptr_t>(at<U>(action, 0x0c)));
    void* receiver = offset(target, at<U>(action, 0x10));
    fn(receiver, at<U>(action, 8), pc);
}
}
extern "C" {
// 00677D10 [00677D10,00677D6D): thiscall(id), RET4; major6/minor0B.
U __fastcall kotor_fastcall_00677d10(void* message, void*, U id) {
    kotor_fastcall_004d68a0(message, nullptr, 4, 0xffffffffu, 1);
    kotor_fastcall_004d68e0(message, nullptr, id); // original WriteBOOL writes all four bytes without normalization
    void* bytes; U size;
    if (kotor_fastcall_004d6440(message, nullptr, &bytes, &size) == 0) return 0;
    return recovered_client_send_00677410(message, nullptr, 6, 0x0b, bytes, size);
}
// 00677D70 [00677D70,00677DDB): thiscall(id,state), RET8; major6/minor3.
U __fastcall kotor_fastcall_00677d70(void* message, void*, U id, U state) {
    kotor_fastcall_004d68a0(message, nullptr, 6, 0xffffffffu, 1);
    kotor_fastcall_004d68e0(message, nullptr, id);
    kotor_fastcall_004d6300(message, nullptr, state, 16);
    void* bytes; U size;
    if (kotor_fastcall_004d6440(message, nullptr, &bytes, &size) == 0) return 0;
    return recovered_client_send_00677410(message, nullptr, 6, 3, bytes, size);
}
// 0062F9A0 [0062F9A0,0062FA29): CanPerformAction, RET0.
U __fastcall kotor_fastcall_0062f9a0(void* gui, void*) {
    U day, time, days, elapsed = 0;
    frame_GetWorldTime_004ade40(frame_GetWorldTimer_005ed4f0(app(), nullptr), nullptr, &day, &time);
    recovered_world_time_difference_004ae460(frame_GetWorldTimer_005ed4f0(app(), nullptr), nullptr,
        day, time, at<U>(gui, 0xbcc), at<U>(gui, 0xbd0), &days, &elapsed);
    if (days > 0) return 1;
    return elapsed >= at<U>(gui, 0xbd4) ? 1u : 0u;
}
// 005F4100 [005F4100,005F411E): test ah,41h / JP skips ordered-greater and unordered.
void __fastcall kotor_fastcall_005f4100(void* ci, void*) {
    const float zero = 0.0f; // original literal0073D700; local memory operand, no original-address access
    unsigned short status;
    __asm {
        mov ecx, ci
        fld dword ptr [ecx + 390h]
        fcomp zero
        fnstsw status
    }
    const U flags = static_cast<U>(status) & 0x4100u;
    // TEST AH,41h / JP: one set bit means ordered less or equal; zero/two mean skip.
    if (flags == 0x0100u || flags == 0x4000u) at<U>(ci, 0x38c) = 0x40a00000;
}
// 005EDF30 [005EDF30,005EDF38): original tail wrapper.
void __fastcall kotor_fastcall_005edf30(void* client, void*) {
    kotor_fastcall_005f4100(at<void*>(client, 4), nullptr);
}
// 00682660 [00682660,0068269C): action id and actor are unused, but RET8 is mandatory.
void __fastcall kotor_fastcall_00682660(void* placeable, void*, U, void*) {
    void* message = kotor_fastcall_005ed6f0(app(), nullptr);
    if (kotor_fastcall_0062f9a0(kotor_input_required::get_in_game_gui_005ed690(app(), nullptr), nullptr) != 0)
        (void)kotor_fastcall_00677d10(message, nullptr, at<U>(placeable, 4));
}
// 00683D90 [00683D90,00683DF9): real state-dependent open/close request; RET8.
void __fastcall kotor_fastcall_00683d90(void* door, void*, U, void* actor) {
    if (actor != nullptr) kotor_fastcall_0063d470(actor, nullptr);
    void* message = kotor_fastcall_005ed6f0(app(), nullptr);
    const std::uint16_t state = at<std::uint16_t>(door, 0x48);
    if (state == 0x2726) {
        (void)kotor_fastcall_00677d70(message, nullptr, at<U>(door, 4), 0x2725);
        kotor_fastcall_005edf30(app(), nullptr);
        return;
    }
    if (state == 0x2742 || state == 0x2743)
        (void)kotor_fastcall_00677d70(message, nullptr, at<U>(door, 4), 0x2726);
}
// 004AEFA0 [004AEFA0,004AEFBF): no fabricated creature; original null return retained.
void __fastcall kotor_fastcall_004aefa0(void* server, void*, U id) {
    void* creature = kotor_fastcall_004b1790(at<void*>(server, 4), nullptr, id);
    if (creature != nullptr) recovered_combat_round_remove_all_actions_004d3770(at<void*>(creature, 0x9c8), nullptr);
}
// 0063D490 [0063D490,0063D4AE): delegates to the real server and existing W1 round owner.
void __fastcall kotor_fastcall_0063d490(void* object, void*) {
    void* server = at<void*>(manager(), 8);
    kotor_fastcall_004aefa0(server, nullptr, kotor_fastcall_004aea30(server, nullptr, at<U>(object, 4)));
}
// 00688490 [00688490,006884A9): target focus control metadata, RET4.
void __fastcall kotor_fastcall_00688490(void* panel, void*, void* control) {
    if (at<U>(control, 0x4c) != 0)
        at<unsigned char>(panel, 0x1ba7) = static_cast<unsigned char>(at<unsigned char>(control, 0x58) - 4);
}
// 006856C0 [006856C0,006856DF): personal-action focus, RET4.
void __fastcall kotor_fastcall_006856c0(void* panel, void*, void* control) {
    if (at<U>(control, 0x4c) != 0 && at<I>(control, 0x58) != at<I>(panel, 0x1bc4))
        at<I>(panel, 0x1bc4) = at<I>(control, 0x58);
}
// 006887D0 [006887D0,00688819): original shift-modifier clear path, RET0.
void __fastcall kotor_fastcall_006887d0(void* panel, void*) {
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(app(), nullptr), nullptr);
    if (pc != nullptr) {
        kotor_fastcall_00610a10(pc, nullptr, 0);
        kotor_fastcall_0063d470(pc, nullptr);
        kotor_fastcall_0040a140(at<void*>(panel, 0x18), nullptr, 0);
        at<U>(panel, 0x5cb0) = 0xbf800000;
        at<U>(panel, 0x6c) = 0xbf800000;
    }
}
// 0068B970 [0068B970,0068B9C8): original GUI event dispatch, RET4.
void __fastcall kotor_fastcall_0068b970(void* panel, void*, void* control) {
    if (at<U>(control, 0x4c) == 0) return;
    const I index = at<I>(control, 0x58);
    if (index >= 0 && index < 4) { kotor_fastcall_0068ad60(panel, nullptr, index, 0); return; }
    const I target_index = index - 4;
    if (target_index < 0 || target_index >= 3) return;
    if (kotor_input_cells_required::word_008338f8 != 0) kotor_fastcall_006887d0(panel, nullptr);
    kotor_fastcall_00689610(offset(panel, 0xbc), nullptr, target_index, 1);
}
// 00689610 [00689610,00689881), six-entry jump table00689884..0068989B; RET8.
void __fastcall kotor_fastcall_00689610(void* menu, void*, I slot, U) {
    void* row = offset(menu, static_cast<U>(slot) * 12);
    if (at<I>(row, 4) == 0) return;
    void* client = app();
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client, nullptr), nullptr);
    const I category = static_cast<I>(at<std::int8_t>(menu, 0x1aea));
    const U selected = at<U>(menu, static_cast<U>(slot + (category + 3) * 3) * 4);
    void* action = nullptr;
    if (selected != 0xffffffffu) {
        for (I i = 0; i < at<I>(row, 4); ++i) {
            void* entry = offset(at<void*>(row, 0), static_cast<U>(i) * 0x38);
            if (at<U>(entry, 8) == selected) { action = entry; break; }
        }
    }
    if (action == nullptr) action = at<void*>(row, 0);
    if (pc == nullptr || action == nullptr) return;
    void* object = kotor_fastcall_005ed580(app(), nullptr, at<U>(action, 0x1c));
    if (object == nullptr) return;
    void* target = vcall<void*>(object, 0x0c);
    if (target == nullptr) return;
    const U flags = at<U>(action, 0x30);
    if ((flags & 1) == 0 || at<U>(action, 0x0c) == 0) {
        disabled_action(menu, flags, 0x1ae4, 0x1adc);
        kotor_fastcall_0040a140(kotor_fastcall_005eda40(app(), nullptr), nullptr, 2);
        return;
    }
    U walking = 0;
    if (!combat(pc)) {
        if (kotor_fastcall_005ede70(client, nullptr) == 0 && at<std::int8_t>(menu, 0x1aea) == 3) walking = 1;
        kotor_fastcall_0063d490(pc, nullptr);
        const std::int8_t type = at<std::int8_t>(menu, 0x1aea);
        if ((type == 0 && at<U>(action, 8) == 0x3f5) || type == 3) {
            at<U>(at<void*>(menu, 0x15a8), 0x7724) = 0xbf800000;
            kotor_fastcall_00687700(at<void*>(menu, 0x15a8), nullptr, 0xbc50);
        }
    } else if (kotor_fastcall_005edef0(client, nullptr) != 0) {
        frame_SetPaused_0062def0(kotor_input_required::get_in_game_gui_005ed690(client, nullptr), nullptr, 1, 10);
    }
    kotor_fastcall_0040a140(kotor_fastcall_005eda40(app(), nullptr), nullptr, 6);
    invoke_action(action, target, pc);
    void* button = offset(menu, static_cast<U>(slot) * 0x71c + 0x54);
    at<unsigned char>(button, 0x714) = 2;
    at<U>(button, 0x710) = 0x3dcccccd;
    if (!combat(pc)) {
        if (walking != 0) kotor_fastcall_00610a10(pc, nullptr, 1);
        if (kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr) != nullptr)
            w5_object_clear_all_actions_004ccd80(kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr), nullptr, 0);
    }
}
// 0068AD60 [0068AD60,0068AF54), six-entry jump table0068AF54..0068AF6B; RET8.
void __fastcall kotor_fastcall_0068ad60(void* panel, void*, I slot, U) {
    if (slot < 0 || slot >= 6) return;
    if (kotor_input_cells_required::word_008338f8 != 0) kotor_fastcall_006887d0(panel, nullptr);
    void* row = offset(panel, static_cast<U>(slot) * 12 + 0x78);
    at<I>(panel, 0x1bc4) = slot;
    if (at<I>(row, 0) == 0) return;
    void* client = app();
    void* pc = kotor_fastcall_00635460(kotor_fastcall_005ed8b0(client, nullptr), nullptr);
    const U selected = at<U>(panel, static_cast<U>(slot) * 4 + 0x1bac);
    void* action = nullptr;
    if (selected != 0xffffffffu) {
        for (I i = 0; i < at<I>(row, 0); ++i) {
            void* entry = offset(at<void*>(panel, static_cast<U>(slot) * 12 + 0x74), static_cast<U>(i) * 0x38);
            if (at<U>(entry, 8) == selected) { action = entry; break; }
        }
    }
    if (action == nullptr) action = at<void*>(panel, static_cast<U>(slot) * 12 + 0x74);
    if (pc == nullptr || action == nullptr) return;
    void* object = kotor_fastcall_005ed580(app(), nullptr, at<U>(action, 0x1c));
    if (object == nullptr) return;
    void* target = vcall<void*>(object, 0x0c);
    if (target == nullptr) return;
    const U flags = at<U>(action, 0x30);
    if ((flags & 1) == 0 || at<U>(action, 0x0c) == 0) {
        disabled_action(panel, flags, 0x70, 0x6c);
        kotor_fastcall_0040a140(at<void*>(panel, 0x18), nullptr, 2);
        return;
    }
    if (!combat(pc)) {
        kotor_fastcall_0063d490(pc, nullptr);
        kotor_fastcall_0040a140(at<void*>(panel, 0x18), nullptr, 6);
    } else if (kotor_fastcall_005edef0(client, nullptr) != 0) {
        frame_SetPaused_0062def0(kotor_input_required::get_in_game_gui_005ed690(client, nullptr), nullptr, 1, 10);
    }
    invoke_action(action, target, pc);
    void* button = offset(panel, static_cast<U>(slot) * 0x71c + 0x772c);
    at<unsigned char>(button, 0x714) = 2;
    at<U>(button, 0x710) = 0x3dcccccd;
    if (!combat(pc) && kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr) != nullptr)
        w5_object_clear_all_actions_004ccd80(kotor_m15a_creature_get_server_creature_0060fb20(pc, nullptr), nullptr, 0);
}
// 00687700 [00687700,00687851): reuse the five existing color storage owners and preserve original EH.
void __fastcall kotor_fastcall_00687700(void* panel, void*, U message) {
    if (at<U>(panel, 0x7724) != 0xbf800000) return;
    const void* color = borrowed_exit_color_007a23b4;
    bool timed = true;
    switch (message) {
    case 0xa5ed:
        if (kotor_fastcall_005edef0(app(), nullptr) != 0) return;
        timed = false; break;
    case 0xa5ec:
        if (kotor_fastcall_005edef0(app(), nullptr) != 0) return;
        color = kotor_m15d_color_007a23e4; timed = false; break;
    case 0xa5ee: color = kotor_m15d_color_007a23e4; break;
    case 0xbaf3:
        if (kotor_fastcall_005edef0(app(), nullptr) != 0) return;
        color = kotor_m1_color_007a23fc; break;
    case 0xbb2b: color = w4::map_note_color_007a23f0; timed = false; break;
    case 0xbc50: color = kotor_m15d_color_007a23d8; timed = false; break;
    default: break;
    }
    void* params = offset(panel, 0x7444);
    gui_load_text_params_set_color_00414e10(params, nullptr, color);
    if (timed) { at<U>(panel, 0x7720) = 0x40200000; at<U>(panel, 0x7724) = 0x40200000; }
    at<U>(panel, 0x7728) = message;
    alignas(4) unsigned char text[8];
    kotor_fastcall_005edeb0(app(), nullptr, text, message);
    try { gui_load_text_params_set_text_00415e00(params, nullptr, text); at<U>(panel, 0x746c) = 0x3f800000; }
    catch (...) { c2da_current_string_destroy_005e5c20(text, nullptr); throw; }
    c2da_current_string_destroy_005e5c20(text, nullptr);
}
}
