#include "bd9f6ce7e8-m15e_animbase.h"
#include <cstring>

namespace {
void* model(void* self) {
    void* value;
    std::memcpy(&value, static_cast<unsigned char*>(self) + 0x68, sizeof(value));
    return value;
}
const KotorM15eAnimBaseVtbl* methods(void* value) {
    const KotorM15eAnimBaseVtbl* table;
    std::memcpy(&table, value, sizeof(table));
    return table;
}
}

// 006825F0..00682614: reload the model after UnloadModel before scalar deletion.
extern "C" void __fastcall recovered_client_object_unload_model_006825f0(void* self, void*) {
    void* value = model(self);
    if (value == nullptr) return;
    (void)methods(value)->s078(value, nullptr);
    value = model(self);                       // 006825FF, potentially changed by the virtual call
    if (value != nullptr)
        (void)methods(value)->s000(value, nullptr, 1);
    value = nullptr;
    std::memcpy(static_cast<unsigned char*>(self) + 0x68, &value, sizeof(value));
}
