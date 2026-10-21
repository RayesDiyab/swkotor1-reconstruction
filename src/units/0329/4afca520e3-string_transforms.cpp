#include "9c79b7f222-string_transforms.h"
#include "7deb550b61-allocator_live.h"
#include <cstring>
#include <new>

namespace base_ini_reconstruction {

String* __fastcall left_005e5f70(String* self, void*, String* output, int count) {
    String temporary;
    const volatile unsigned char* measured =
        reinterpret_cast<const volatile unsigned char*>(self->data);
    if (measured) {
        const volatile unsigned char* cursor = measured;
        while (*cursor) ++cursor;
        const std::uint32_t length = static_cast<std::uint32_t>(cursor - measured);
        if (count >= 0 && length != 0) {
            std::uint32_t selected = static_cast<std::uint32_t>(count);
            if (selected > length) selected = length;
            temporary.capacity = selected + 1u;
            char* allocation = static_cast<char*>(live_operator_new(selected + 1u));
            temporary.data = allocation;
            // Original reloads the source after the temporary allocation.
            std::strncpy(allocation, self->data, selected);
            allocation[selected] = '\0';
        }
    }
    // Parent-owned support-only copy-constructor shell calls unchanged
    // WindowCopy005e5b00 on this actual object, without early field stores.
    // The genuine temporary destructor frees its private saved allocation
    // after output copy construction and before this entry returns.
    return ::new (static_cast<void*>(output)) String(temporary);
}

String* __fastcall right_005e6170(String* self, void*, String* output, int count) {
    String temporary;
    const volatile unsigned char* measured =
        reinterpret_cast<const volatile unsigned char*>(self->data);
    if (measured) {
        const volatile unsigned char* cursor = measured;
        while (*cursor) ++cursor;
        const std::uint32_t length = static_cast<std::uint32_t>(cursor - measured);
        if (count >= 0 && length != 0) {
            std::uint32_t selected = static_cast<std::uint32_t>(count);
            if (selected > length) selected = length;
            temporary.capacity = selected + 1u;
            char* allocation = static_cast<char*>(live_operator_new(selected + 1u));
            temporary.data = allocation;
            // Offset uses the first measured length and the clamped count;
            // the source pointer itself is reloaded after allocation.
            const char* source = self->data;
            std::strncpy(allocation, source + (length - selected), selected);
            allocation[selected] = '\0';
        }
    }
    return ::new (static_cast<void*>(output)) String(temporary);
}

} // namespace base_ini_reconstruction
