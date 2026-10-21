// Original 00579090 — CSWSAreaMap::GetMapData
// Full entry-to-return reconstruction; new ownership in this lane.
//
// Original span 00579090..005790B9 (0x29):
//   eax=[map+4]; test eax,eax; ja success; xor eax,eax; ret 0xC
//   success: *out1=[map+4]; *out2=[map+8]; *out3=[map+C]; return [map]
//
// ABI: thiscall (ECX=map), three stack U32* outputs, RET 0xC.
// Export: @kotor_fastcall_00579090@20 (ECX=this, EDX unused, 3 args).
// No calls, allocation, x87, SEH or cleanup. Null map/outputs fault exactly as
// the original; the current 1566 selected 00579090 is the unrelated
// ?map_float_integer body and must be rebound by the integrator.

#include <cstdint>

namespace {

std::uint32_t load_u32(const void* base, std::uintptr_t offset) {
    return *reinterpret_cast<const std::uint32_t*>(
        reinterpret_cast<std::uintptr_t>(base) + offset);
}

}  // namespace

extern "C" void* __fastcall kotor_fastcall_00579090(
    void* self,
    void* /*unused_edx*/,
    std::uint32_t* out1,
    std::uint32_t* out2,
    std::uint32_t* out3) {
    const std::uint32_t count = load_u32(self, 0x04);
    if (count == 0) {
        return nullptr;
    }

    *out1 = count;
    *out2 = load_u32(self, 0x08);
    *out3 = load_u32(self, 0x0C);
    return reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(load_u32(self, 0x00)));
}
