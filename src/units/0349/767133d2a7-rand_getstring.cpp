#include "bce7a91e4c-rand_getstring.h"
#include "dcaf3f57f3-allocator_live.h"
#include "20414ac044-string_priority.h"
#include <new>

namespace startup_rand {
namespace {
// Support-only forward transfers, with all four bytes read before each dword
// group's stores. This preserves the REP MOVSD/MOVSB transfer order in the
// valid-memory domain; it adds no separately reconstructed address.
void CopyForward(unsigned char* destination, const unsigned char* source,
                 std::uint32_t count) {
    volatile unsigned char* out = destination;
    const volatile unsigned char* in = source;
    for (std::uint32_t words = count >> 2; words != 0; --words) {
        const unsigned char a = in[0], b = in[1], c = in[2], d = in[3];
        out[0] = a; out[1] = b; out[2] = c; out[3] = d;
        in += 4; out += 4;
    }
    for (std::uint32_t bytes = count & 3u; bytes != 0; --bytes) *out++ = *in++;
}
}

kotor_reconstruction::StartupString* __fastcall GetString005ecea0(
    kotor_reconstruction::CExoRandInternal* self, void*,
    kotor_reconstruction::StartupString* output, std::uint16_t length) {
    const std::int32_t count = static_cast<std::int32_t>(length);
    // The original IDIV precedes allocation and default String construction.
    // Zero length is outside the admitted normal, nonthrowing domain.
    const std::int32_t quotient = 128 / count;
    const std::int32_t remainder = 128 % count;
    unsigned char* const allocation = static_cast<unsigned char*>(
        live_operator_new(static_cast<std::uint32_t>(count + 1)));
    kotor_reconstruction::StartupString temporary;
    volatile kotor_reconstruction::CExoRandInternal* const live = self;
    CopyForward(allocation, live->random_pool, static_cast<std::uint32_t>(count));

    if (quotient > 1) {
        std::uint32_t offset = static_cast<std::uint32_t>(count);
        for (std::int32_t blocks = quotient - 1; blocks != 0; --blocks) {
            for (std::int32_t i = 0; i != count; ++i) {
                const volatile unsigned char* const pool = live->random_pool;
                volatile unsigned char* const generated = allocation;
                const unsigned char value = pool[offset + static_cast<std::uint32_t>(i)];
                const unsigned char previous = generated[i];
                generated[i] = static_cast<unsigned char>(previous ^ value);
            }
            offset += static_cast<std::uint32_t>(count);
        }
    }
    if (remainder != 0) {
        const std::uint32_t offset = static_cast<std::uint32_t>(quotient * count);
        for (std::int32_t i = 0; i != remainder; ++i) {
            const volatile unsigned char* const pool = live->random_pool;
            volatile unsigned char* const generated = allocation;
            const unsigned char value = pool[offset + static_cast<std::uint32_t>(i)];
            const unsigned char previous = generated[i];
            generated[i] = static_cast<unsigned char>(previous ^ value);
        }
    }
    for (std::int32_t i = 0; i != count; ++i) {
        volatile unsigned char* const generated = allocation;
        if (generated[i] == 0) {
            // Reuse the sole Task17 state algorithm. The constructor calls
            // this before its later ReSeed; no substitute initial state.
            const std::uint32_t random = self->Rand();
            generated[i] = static_cast<unsigned char>(random % 250u + 1u);
        }
    }
    static_cast<volatile unsigned char*>(allocation)[count] = 0;

    const std::uint32_t position = live->pool_position;
    unsigned char* const first_pool = live->random_pool;
    std::uint32_t adjusted_count = static_cast<std::uint32_t>(count);
    if (position + adjusted_count >= 128u) {
        CopyForward(first_pool + position, allocation, 128u - position);
        // The original rereads both fields after the first transfer.
        const std::uint32_t current_position = live->pool_position;
        unsigned char* const current_pool = live->random_pool;
        CopyForward(current_pool, allocation + (128u - current_position),
                    current_position + adjusted_count - 128u);
        adjusted_count -= 128u;
    } else {
        CopyForward(first_pool + position, allocation, adjusted_count);
    }
    // The original performs an ADD on the live16-bit field after the copies.
    live->pool_position = static_cast<std::uint16_t>(
        live->pool_position + adjusted_count);
    GuiAssign005e5140(&temporary, nullptr, reinterpret_cast<const char*>(allocation));
    live_free(allocation);
    // Existing copy shell invokes the sole 005E5B00 algorithm without early
    // output stores. The actual temporary destructor runs before RET8.
    return ::new (static_cast<void*>(output)) kotor_reconstruction::StartupString(temporary);
}
}
