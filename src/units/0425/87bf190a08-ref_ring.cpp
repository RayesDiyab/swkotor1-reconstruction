#include "d6795b968b-ref_ring.hpp"
#include "b472198264-shared_boundary.hpp"

namespace ref_00405fe0_private {
std::uint32_t next_index_bits(std::uint32_t prior) noexcept {
    std::uint32_t next=(prior+1u)&0x80000003u;
    if (next&0x80000000u) next=((next-1u)|0xfffffffcu)+1u;
    return next;
}
char* get(menu_resources::Ref* reference,Shared shared) noexcept {
    const std::uint32_t next=next_index_bits(shared.index);
    shared.index=next; // Original store precedes every Ref load.
    // Acceptance domain: shared.index starts at original zero and is only
    // updated by this routine, so next is 0..3. Negative-index memory accesses,
    // data races and access-violation/SEH behavior are outside this C++ proposal.
    unsigned char* destination=shared.slots[next];
    const unsigned char* source=reference->bytes;
    for (unsigned offset=0;offset<16;offset+=4) {
        // Capture a complete original DWORD before its stores. Do not replace
        // with one memcpy or interleave each source byte with its destination.
        const std::uint32_t word=std::uint32_t(source[offset]) |
            (std::uint32_t(source[offset+1])<<8) |
            (std::uint32_t(source[offset+2])<<16) |
            (std::uint32_t(source[offset+3])<<24);
        destination[offset]=static_cast<unsigned char>(word);
        destination[offset+1]=static_cast<unsigned char>(word>>8);
        destination[offset+2]=static_cast<unsigned char>(word>>16);
        destination[offset+3]=static_cast<unsigned char>(word>>24);
    }
    destination[16]=0;
    return reinterpret_cast<char*>(destination);
}
}
extern "C" char* __fastcall c2da_ref_00405fe0(void* receiver,void*) noexcept {
    return ref_00405fe0_private::get(
        static_cast<menu_resources::Ref*>(receiver),ref_00405fe0_private::bindings);
}
