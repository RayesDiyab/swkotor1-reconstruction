#pragma once
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace menu_action {
using Callback = void (__fastcall*)(void* self, void* unused_edx,
                                    std::uint32_t action_id, void* creature);
// The existing action owner copies four DWORDs at action+C..18. 00619950
// constructs this record with 00617DD0 followed by three zero DWORDs;
// 00618A30 copies its four incoming DWORDs without invoking the callback.
struct CallbackRecord {
    Callback callback;
    std::uint32_t word1;
    std::uint32_t word2;
    std::uint32_t word3;
};
static_assert(sizeof(CallbackRecord) == 16, "original four-DWORD callback record");
static_assert(offsetof(CallbackRecord, callback) == 0, "callback word");
static_assert(offsetof(CallbackRecord, word1) == 4, "opaque word1");
static_assert(offsetof(CallbackRecord, word2) == 8, "opaque word2");
static_assert(offsetof(CallbackRecord, word3) == 12, "opaque word3");
static_assert(std::is_trivially_copyable<CallbackRecord>::value, "by-value ABI record");
}

extern "C" void __fastcall kotor_fastcall_00618a30(
    void* self, void* unused_edx, std::uint32_t feat, void* output_list,
    menu_action::CallbackRecord callback, void* creature);
