// Original 004D2B30 — CSWSCombatRound::GetCurrentAction
// Reconstruction of the complete original entry-to-return body.
//
// Original span: 004D2B30..004D2B36, 7 bytes.
//   mov al, byte ptr [ecx+0x9D0]
//   ret
//
// ABI: thiscall with no stack arguments (ECX = CSWSCombatRound*, EDX unused),
// ret 0, unsigned byte result in AL. The original leaves EAX[31:8] untouched;
// the only caller (0068A061) tests AL only. No calls, no allocation, no x87,
// no SEH, no cleanup, no null check: a null ECX faults at the load exactly as
// the original does.
//
// Export name under the project's fastcall-symbol convention:
//   @kotor_fastcall_004d2b30@8   (two pointer-sized parameters = 8 bytes)
//
// This TU defines only original 004D2B30.

extern "C" unsigned char __fastcall kotor_fastcall_004d2b30(void* self, void* /*edx*/)
{
    // Byte field at +0x9D0; neighbor 004D2B40 is the matching byte setter.
    return *reinterpret_cast<unsigned char*>(static_cast<unsigned char*>(self) + 0x9D0);
}
