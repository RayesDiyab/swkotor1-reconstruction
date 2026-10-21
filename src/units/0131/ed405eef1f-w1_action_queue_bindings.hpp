#pragma once
// 004F6F30 action-queue join contracts (private delivery, admission L1).
// Original identities are the addresses; each selected provider keeps its own full body.
// Stack/register ABI is the selected provider's, not a cast of the original identity.
extern "C" {
// 004F6F30 -> selected w2_ifo_native.obj body 0x005D7E30, @16, RET8.
// Original is thiscall(creature, mode, value); the fastcall form names the unused EDX
// explicitly, so it has four total dwords and cleans exactly 8 stack bytes.
void __fastcall recovered_set_stealth_mode_0050ee50(
    void* creature, void* unused_edx, unsigned mode, unsigned value);

// 00553960 -> selected menu_active_property.obj facade 0x005A6C50, @12, RET4.
// ECX=item, EDX unused, stack index; pointer result (or null for signed out-of-range).
void* __fastcall kotor_m15_menu_active_property_00553960(
    void* item, void* unused_edx, unsigned index);

// 0057A2B0 -> selected 1194w-vm_commands_native_t5-w6t5.obj export 0x004FD790, @16, RET8.
// ECX=unused object, EDX unused, stack type then flag; AX result (low 16 bits used).
unsigned short __fastcall recovered_object_aq_action_id_by_id_0057a2b0(
    void* object, void* unused_edx, unsigned type, int flag);

// New export wrapping the portable 004F6F30 body for ABI/readback tests.
void __fastcall recovered_creature_update_action_queue_004f6f30(void* self, void* unused_edx);
}
