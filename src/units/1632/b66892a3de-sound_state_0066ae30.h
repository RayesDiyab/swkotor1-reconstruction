#pragma once
// S1 sound-state leaves: 005D6190, 005D6280 and 0066AE30.
//
// Original: GOG KOTOR 1 Windows x86 swkotor.exe (image base 0x00400000).
// Private evidence pins (never packaged as raw bytes):
//   lane-residual-r2/evidence/orig-005D6190-disasm.txt  48833141...
//   lane-residual-r2/evidence/orig-005D6280-disasm.txt  C70915D3...
//   lane-residual-r2/evidence/oracle-0066AE30-disasm.txt 7387D682...
//
// ABI convention used by this delivery: every original __thiscall entry is an
// extern "C" __fastcall(Self*, void* unused_edx, ...) veneer with the same
// callee stack cleanup.  The two getters have no stack arguments (@8, RET0);
// 0066AE30 has one stack argument (@12, RET4).  EDX is ignored by all three.
//
// 005D6190 is CExoSoundSource's internal looping-state getter:
//   [source+4] -> CExoSoundSourceInternal*, then [internal+0x24], or 0 when the
//   internal pointer is null.  A null source is dereferenced, as in the
//   original; there is deliberately no null-self guard.
//
// 005D6280 is CExoStreamingSoundSource's internal looping-state getter:
//   [source] -> CExoStreamingSoundSourceInternal*, then [internal+4], or 0 when
//   the internal pointer is null.  A null source is dereferenced.
//
// 0066AE30 is CSWCSoundObject's sound-id/activation setter.  Its actual caller
//   at 0066B4F7 (CSWCSoundObject::LoadFromMessage) ignores EAX, so the export is
//   void and does not fabricate a success return.  It resets +0x14C, stores the
//   new id at +0x104, then consults the original +0x168/+0xFC/+0x100 predicates
//   and sets +0x128 to 1 only when the selected getter returns nonzero.
#include <cstdint>

extern "C" {
std::uint32_t __fastcall kotor_m15_sound_source_get_looping_005d6190(void* source, void* unused_edx);
std::uint32_t __fastcall kotor_m15_stream_source_get_looping_005d6280(void* source, void* unused_edx);
void __fastcall recovered_client_sound_object_set_sound_id_0066ae30(void* self, void* unused_edx,
                                                                    std::uint32_t sound_id);
}
