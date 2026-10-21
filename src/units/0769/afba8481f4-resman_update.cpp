// lane-m47-core: CExoResMan::Update 00408D40 (first-frame: lane-firstframe PATH.md 7c, MainLoop ResourceUpdate,
// reached from frame-r4 entry_router case 0x00408d40).
// Evidence: dumpbin /disasm swkotor.exe 00408D40..00408E18 + jump table 00408E1C = {00408DE4, 00408DF0,
// 00408DD8, 00408DCC} (lane dis/00408d40.asm; tools/rd.py 00408e1c:16).
// ABI: thiscall, one stack word (RET 4) -> D1 form `void __fastcall(void* self, void* edx, unsigned)` exactly as
// declared by the consumer (frame-gui/source/entry_providers.h). The stack word is read only on the
// unreachable `ja 00408DFC` edge (key>>30 is always <= 3), so it is unused.
// Callees (all M46 providers): RemoveHead 005E9C80 (ini-list-cleanup), ReadAsyncComplete 005E6990,
// ServiceCurrentAsyncRes 00408530, loaders 00407E00/00407D50/00407BD0/004078F0 (task-10 manager_demand.h).
#include "d06e8b44c9-manager_demand.h"
#include "37219f0b20-ini_cleanup.h"
#include <cstdint>
#include <cstring>

namespace {
using Word = std::uint32_t;
template <class T> T read(const void* object, unsigned offset) {
    T value; std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, sizeof(value)); return value;
}
template <class T> void write(void* object, unsigned offset, T value) {
    std::memcpy(static_cast<unsigned char*>(object) + offset, &value, sizeof(value));
}
}

extern "C" __declspec(noinline) void __fastcall frame_R4_00408d40(void* self, void*, unsigned) {
    menu_resources::Manager* const manager = static_cast<menu_resources::Manager*>(self);
    if (read<Word>(self, 0x30) != 0) return;                                     // 00408D45..00408D4C
    void* const list = read<void*>(self, 0x24);                                  // 00408D52 ECX
    if (read<void*>(list, 0) == nullptr) return;                                 // 00408D55..00408D57 (head node)
    if (read<void*>(self, 0x28) == nullptr) {                                    // 00408D5D..00408D60
        void* const head = read<void*>(list, 0);                                 // 00408D62
        write<void*>(self, 0x28, head ? read<void*>(head, 8) : nullptr);         // 00408D64..00408D6F
    }
    void* const resource = read<void*>(self, 0x28);                              // 00408D72
    const Word key = read<Word>(resource, 8);                                    // 00408D76
    const unsigned char flags = read<unsigned char>(resource, 0x0c);             // 00408D90 (DL tested)
    if (key == 0xffffffffu || (flags & 4u) != 0) {                               // 00408D79 / 00408D93
        // 00408D7E: RemoveHead(ECX = the [this+24h] list loaded at 00408D52), EAX ignored.
        (void)base_ini_reconstruction::remove_head_005e9c80(static_cast<kotor_list::Internal*>(list), nullptr);
        write<Word>(self, 0x28, 0);                                              // 00408D84
        write<Word>(self, 0x2c, 0);                                              // 00408D87
        return;                                                                  // 00408D8D RET 4
    }
    if ((flags & 0x10u) != 0) {                                                  // 00408D98
        void* const completion = read<void*>(self, 0x2c);                        // 00408D9D
        if (completion == nullptr) return;                                       // 00408DA2
        if (c2da_005e6990(completion, nullptr) == 0) return;                     // 00408DA4..00408DAB
        (void)c2da_manager_00408530(manager, nullptr);                           // 00408DAD..00408DAF, EAX ignored
        return;                                                                  // 00408DB8 RET 4
    }
    Word loaded = 0;
    switch (key >> 30) {                                                         // 00408DBB..00408DC5 (table 00408E1C)
    case 0: loaded = c2da_manager_00407e00(manager, nullptr, resource, 1u); break; // 00408DE4..00408DE9
    case 1: loaded = c2da_manager_00407d50(manager, nullptr, resource, 1u); break; // 00408DF0..00408DF5
    case 2: loaded = c2da_manager_00407bd0(manager, nullptr, resource, 1u); break; // 00408DD8..00408DDD
    case 3: loaded = c2da_manager_004078f0(manager, nullptr, resource, 1u); break; // 00408DCC..00408DD1
    }
    if (loaded == 0) return;                                                     // 00408E00..00408E02
    void* const current = read<void*>(self, 0x28);                               // 00408E04 (reloaded)
    const Word state = read<Word>(current, 0x0c);                                // 00408E07
    if ((state & 4u) != 0) return;                                               // 00408E0A..00408E0C
    write<Word>(current, 0x0c, state | 0x10u);                                   // 00408E0E..00408E11
}
