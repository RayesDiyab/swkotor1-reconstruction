#include "e53f4d496b-exorand.h"

namespace kotor_reconstruction {
std::uint32_t exo_seed_mask = 0xffffffffu;

static std::uint32_t Shl(std::uint32_t value, std::uint32_t count) { return value << (count & 31u); }
static std::uint32_t Shr(std::uint32_t value, std::uint32_t count) { return value >> (count & 31u); }

static __declspec(noinline) std::uint32_t __cdecl NextState(CExoRandInternal* object) {
    std::uint32_t output = 0;
    for (unsigned i = 0; i != 4; ++i) {
        const std::uint32_t old = object->state_vars[i];
        const std::uint32_t next = Shr(Shl(old, object->left_shifts[i]) ^ old, object->right_shifts[i]) ^
                                   Shl(old & object->masks[i], object->additional_left_shifts[i]);
        object->state_vars[i] = next;
        output ^= next;
    }
    return output;
}

// Rand's original body restores its incoming ECX and returns this in EDX, in
// addition to the declared thiscall contract. This adapter preserves that quirk;
// the unsigned state arithmetic itself is compiled C++ above.
__declspec(naked) std::uint32_t CExoRandInternal::Rand() {
    __asm {
        push ecx
        push ecx
        call NextState
        add esp, 4
        pop ecx
        mov edx, ecx
        or eax, eax
        ret
    }
}

void CExoRandInternal::ReSeed(std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d) {
    const std::uint32_t seeds[4] = {a, b, c, d};
    const std::uint32_t global = exo_seed_mask; // one read, as in the original
    for (unsigned i = 0; i != 4; ++i) {
        right_shifts[i] = total_shifts[i] - additional_left_shifts[i];
        masks[i] = Shl(global, 32u - total_shifts[i]);
    }
    seeded = 1;
    for (unsigned i = 0; i != 4; ++i)
        if (seeds[i] > Shl(1u, right_shifts[i])) state_vars[i] = seeds[i];
    Rand();
}

// Forward dword transfers followed by forward byte transfers. All four source
// bytes are captured before a dword store, preserving the original overlap quirk.
static void CopyForward(unsigned char* dest, unsigned char* src, std::uint32_t count) {
    for (std::uint32_t words = count >> 2; words != 0; --words) {
        const unsigned char a = src[0], b = src[1], c = src[2], d = src[3];
        dest[0] = a; dest[1] = b; dest[2] = c; dest[3] = d;
        src += 4; dest += 4;
    }
    for (std::uint32_t bytes = count & 3u; bytes != 0; --bytes) *dest++ = *src++;
}

void CExoRandInternal::Add(unsigned char* source, std::int32_t count) {
    const std::uint32_t position = pool_position;
    std::uint32_t amount = static_cast<std::uint32_t>(count);
    const std::uint32_t sum = position + amount;
    // Signed JGE after 32-bit wrapping LEA. No invented validation or modulo loop.
    if ((sum ^ 0x80000000u) >= 0x80000080u) {
        const std::uint32_t first = 128u - position;
        CopyForward(random_pool + position, source, first);
        const std::uint32_t current_position = pool_position; // original re-reads
        CopyForward(random_pool, source + (128u - current_position), current_position + amount - 128u);
        amount -= 128u;
    } else {
        CopyForward(random_pool + position, source, amount);
    }
    pool_position = static_cast<std::uint16_t>(pool_position + amount);
}

std::uint32_t CExoRand::Rand() { return internal->Rand(); }
}
