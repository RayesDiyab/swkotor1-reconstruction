// 005F7960 — CClientExoAppInternal::ChangeCharacterToNextLivingPartyMember (owned export).
// Full 440-byte body, all 22 branches, four RET 0xC exits. The original computes 0/1 in EAX;
// the accepted import is void, so each exit materializes the original immediate with a
// reviewed MSVC x86 mov eax,imm immediately before the compiler epilogue. No wrapper/alias.
#include "78e79b69b7-party_selection_decls.h"

// A compound statement, not do/while(0), so /W4 does not see a constant condition.
// Every use is a statement body or a standalone statement.
#define PS_RET_EAX(v) \
    { __asm { mov eax, v } return; }

namespace kotor_input_required {

void __fastcall change_character_to_next_living_party_member_005f7960(
    void* self, void* /*unused_edx*/, U32 arg1, U32 arg2, U32 arg3) {
    using namespace party_selection;

    const U8 n = static_cast<U8>(arg3);                 // 005F7964 low byte
    if (n == 0) PS_RET_EAX(1);                          // 005F797B

    void* const server = server_app();                  // [007A39FC]+8, as at 005F797E
    if (server == nullptr) PS_RET_EAX(0);               // 005F7992

    const U32 pid = kotor_fastcall_004aea40(server, nullptr);          // 005F7996
    // Original reloads [007A39FC]+8 at 005F799B before the second call.
    void* const initial = kotor_fastcall_004ae910(server_app(), nullptr, pid);  // 005F79A5
    if (initial == nullptr) PS_RET_EAX(0);              // 005F79BA -> 005F7B15

    // Original re-reads [self+0x270] at 005F79C0, 005F7A35 and 005F7AAE.
    void* const pc = kotor_fastcall_00635460(
        at<void*>(self, 0x270), nullptr);               // 005F79C6
    const U32 initial_id = at<U32>(pc, 4);              // 005F79CB

    int initial_alive = 0;
    void* initial_creature = nullptr;
    if (arg1 != 0) {                                    // 005F79D8
        // Original reloads [007A39FC]+8 at 005F79DD.
        initial_creature = kotor_fastcall_004ae770(     // 005F79E7
            server_app(), nullptr, at<U32>(initial, 0x38));
        if (initial_creature != nullptr &&
            vcall<U32>(initial_creature, 0x94) == 0 &&  // 005F79FA
            recovered_creature_incapacitated_004ef890(initial_creature, nullptr) == 0) {  // 005F7A06
            initial_alive = 1;                          // 005F7A0F
        }
    }

    int budget = 3 * static_cast<int>(n);               // 005F7A23
    U8 counter = 0;                                     // 005F7A1E arg1-slot byte
    void* current = nullptr;

    while (budget > 0) {                                // 005F7A26 / 005F7A28
        kotor_fastcall_005f6b60(self, nullptr, static_cast<U32>(-2), 0);  // 005F7A30
        current = kotor_fastcall_00635460(
            at<void*>(self, 0x270), nullptr);                            // 005F7A3B
        if (current == nullptr) {                       // 005F7A44
            if (counter != n) continue;
            break;
        }

        void* const sc1 = kotor_m15a_creature_get_server_creature_0060fb20(
            current, nullptr);                                            // 005F7A48
        const U32 dead = vcall<U32>(sc1, 0x94);                           // 005F7A51
        void* const sc2 = kotor_m15a_creature_get_server_creature_0060fb20(
            current, nullptr);                                            // 005F7A5B
        const U32 dead_temp = recovered_creature_incapacitated_004ef890(sc2, nullptr);      // 005F7A62

        if (dead != 0 || dead_temp != 0) {              // 005F7A69..76
            --budget;                                   // 005F7A6F
            if (counter != n) continue;                 // 005F7A94
            break;
        }

        if (initial_alive != 0 &&                       // 005F7A7E
            at<U32>(initial_creature, 4) != at<U32>(initial, 0x38)) {  // 005F7A8E
            if (counter != n) continue;
            break;
        }

        ++counter;                                      // 005F7A90
        if (counter != n) continue;                     // 005F7A9C
        break;                                          // 005F7A9E
    }

    if (counter != n) {                                 // 005F7AA8
        if (budget == 0) PS_RET_EAX(0);                 // 005F7AE3 -> 005F7B15
        const U32 id = at<U32>(initial, 0x38);          // 005F7AE9
        // Original reloads [007A39FC]+4 at 005F7AEC.
        void* const p = kotor_fastcall_005ed8b0(client_app(), nullptr);  // 005F7AF6
        const int index = recovered_party_index_006349c0(p, nullptr, id);            // 005F7AFD
        kotor_fastcall_005f6b60(self, nullptr, static_cast<U32>(index), 0);  // 005F7B07
        PS_RET_EAX(0);                                  // 005F7B15
    }

    if (budget <= 0) PS_RET_EAX(0);                     // 005F7AAC -> 005F7AE3
    void* const final_pc = kotor_fastcall_00635460(
        at<void*>(self, 0x270), nullptr);                    // 005F7AB4
    if (at<U32>(final_pc, 4) == initial_id) PS_RET_EAX(0);           // 005F7AC0
    if (arg2 != 0) kotor_nested_005f4d60(self, nullptr, current);  // 005F7ACD
    PS_RET_EAX(1);                                      // 005F7ADE
}

}  // namespace kotor_input_required
