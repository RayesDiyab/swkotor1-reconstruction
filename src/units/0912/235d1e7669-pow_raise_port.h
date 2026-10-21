#pragma once
// Assembly-only relocation declaration; never issue an ordinary zero-argument
// C++ call. Original0070773F consumes six caller-owned32-bit stack words:
// FP record, mutable control-word storage, flags, operation, argument storage,
// result storage. The caller cleans24bytes and ignores EAX. The FP record has
// writes through+0x60 and requires at least0x64 accessible bytes. This spelling
// intentionally supplies no host return or pointed-to type reconstruction.
extern "C" void __cdecl task8_math_port_0070773f(void);
