#pragma once
#include <cstdint>
namespace sound_pause29_required {
// Proposal for Main6's one genuine writable word at 00832890; no cell or
// initializer is supplied here. The PE loader initially zero-fills its section.
extern volatile std::uint32_t* const word_00832890;
// Task29 sound-lane lower contracts, imported without substitute bodies. The
// caller ignores these four EAX words; their canonical declaration adoption
// and transitive lower closures remain explicit integration prerequisites.
std::uint32_t __fastcall stream_pause_005dc3c0(void*,void*);
std::uint32_t __fastcall stream_resume_005dcdb0(void*,void*);
std::uint32_t __fastcall actor_pause_005db150(void*,void*);
std::uint32_t __fastcall actor_resume_005db1c0(void*,void*);
// Shared three-byte 0063E7F0 leaf: XOR EAX,EAX; RET. No arguments or receiver
// are consumed. The existing startup transport describes stdcall/RET0. This
// declaration proposes one compatible zero-argument direct carrier, not a
// new body or a recovered original source-level prototype.
std::uint32_t __stdcall zero_0063e7f0();
}

