// 0062B0F0 — CGuiInGame::ChangeCharacter (owned export).
// Original 11 bytes: MOV ECX,[ECX+90h]; JMP 00687FD0.
// The [self+90h] value is dead at the tail target but the read is preserved. The exact
// nested 00687FD0 thunk is bound; there is no GetSWParty fold and no 00687FD0 credit here.
#include "78e79b69b7-party_selection_decls.h"

namespace kotor_input_required {

void __fastcall change_character_0062b0f0(void* gui, void* /*unused_edx*/) {
    void* const loaded = party_selection::at<void*>(gui, 0x90);  // 0062B0F0
    (void)kotor_thunk_00687fd0(loaded, nullptr);                 // 0062B0F6 jmp 00687FD0
}

}  // namespace kotor_input_required
