// stale-stubs lane, group w6 (Claude): faithful bodies for originals that had no rebuilt provider and were reached
// only through W6 import trap defaults (1194u vm_commands_native_t3 /alternatename fallbacks).
// Transcribed from the original swkotor.exe (read-only); addresses in comments are original instructions.
// ABI: original __thiscall members exported as extern "C" __fastcall(self, unused_edx, stack args...):
//   00578FA0 CSWSAreaMap::SetEntireMapExplored(void)                              RET 0
#include <cstdint>

namespace {
using U = std::uint32_t;
}

extern "C" {

// ===================================================================================== 00578FA0
// CSWSAreaMap::SetEntireMapExplored: fills the explored bitmap (this+0 data, this+4 dword count) with 0xFF.
// Quirk kept: the byte count is (count << 2) in 32 bits (00578FAA shl ecx,2), so a count >= 2^30 wraps exactly as
// the original rep stosd / rep stosb pair does.
void __fastcall kotor_fastcall_00578fa0(void* self, void*) {
    U* data = *static_cast<U**>(self);                                               // 00578FA1 mov edi,[ecx]
    if (data == nullptr) return;                                                     // 00578FA3..00578FA5
    const U bytes = static_cast<U>(static_cast<U*>(self)[1] << 2);                   // 00578FA7..00578FAD
    const U dwords = bytes >> 2;                                                     // 00578FAF shr ecx,2
    for (U i = 0; i < dwords; ++i) data[i] = 0xffffffffu;                            // 00578FB2..00578FB5 rep stosd
    unsigned char* tail = reinterpret_cast<unsigned char*>(data + dwords);
    for (U i = 0; i < (bytes & 3u); ++i) tail[i] = 0xffu;                            // 00578FB7..00578FBC rep stosb
}                                                                                    // 00578FBE pop edi; ret

}  // extern "C"
