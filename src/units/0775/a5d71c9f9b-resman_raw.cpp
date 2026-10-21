// lane-m47-core: raw (header) reads of the resource manager, used by the Aurora ReadTextureHeader members
// (0070ECE0 / 00710430 / 00710810 / 0070F590):
//   00409B20 CRes::ReadRaw(size, out)              -> 00408E30 on [007A39E8]
//   00408E30 CExoResMan::ReadRaw(res, size, out)   key>>30 jump table 00408E74 = {00408E61, 00408E58, 00408E4F, 00408E46}
//   00408450 ServiceFromResFileRaw (list +10h, BIF index key>>20), 00408390 ServiceFromImageRaw (list +1Ch),
//   004082D0 ServiceFromEncapsulatedRaw (list +18h), 00408080 ServiceFromDirectoryRaw (list +14h, CExoFile).
// Same list walk / archive receiver / slot conventions as the M46 loaders 00407E00 / 00407BD0 / 004078F0
// (resource-successor r32 + file-directory-epoch): archive slots +04 open, +14 close, +24 size(key&3FFFh),
// +34 read(key, out, size, 0). Slots are called through D1 fastcall views read from the table (call_slot0 style).
// Evidence: lane dis/00409b20.asm, 00408e30.asm, 00408450.asm, 00408390.asm, 004082d0.asm, 00408080.asm.
#include "d06e8b44c9-manager_demand.h"
#include "0da4ee585a-directory_data.h"
#include "b8781553f5-directory_ref_boundary.hpp"
#include "f0cd59da21-string_priority.h"
#include "39c670fc66-allocator_live.h"
#include <cstdint>
#include <cstring>
#include <new>

namespace {
using Word = std::uint32_t;
using String = kotor_reconstruction::StartupString;
template <class T> T read(const void* p, Word off) { T v; std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof(v)); return v; }
template <class T> void write(void* p, Word off, T v) { std::memcpy(static_cast<unsigned char*>(p) + off, &v, sizeof(v)); }
using Effect = void (__fastcall*)(void*, void*);
using Sized = Word (__fastcall*)(void*, void*, Word);
using Transfer = Word (__fastcall*)(void*, void*, Word, void*, Word, Word);
template <class Fn> Fn slot(void* object, Word offset) {
    Fn fn; std::memcpy(&fn, static_cast<const unsigned char*>(read<const void*>(object, 0)) + offset, sizeof(fn)); return fn;
}
void effect(void* archive, Word offset) { slot<Effect>(archive, offset)(archive, nullptr); }
}

extern "C" {
Word __fastcall unresolved_list_at_pos_005e9b60(void*, void*, void*);
Word __fastcall c2da_manager_004077b0(menu_resources::Manager*, void*, void*);
void __fastcall c2da_manager_00407020(menu_resources::Manager*, void*, void*);
Word __fastcall c2da_file_005e6a10(void*, void*);
Word __fastcall c2da_file_005e6960(void*, void*, void*, Word, Word);
void __fastcall c2da_005e69e0(void*, void*);
}
void* __fastcall c2da_file_construct_005e68d0(void*, void*, String*, Word, String*);   // m39-directory-ports (C++ linkage)

namespace {
// List walk of 004082E3..0040832F (and the identical walks of the other three readers): GetAtPos(head), then
// GetNext until an entry whose +28h matches (key>>14)&3Fh in the low 28 bits; null when the list ends.
void* find_entry(menu_resources::Manager* manager, void* resource, Word list_offset) {
    void* list = read<void*>(manager, list_offset);
    void* position = read<void*>(list, 0);
    if (!position) return nullptr;
    void* entry = reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_at_pos_005e9b60(list, nullptr, position)));
    while (position) {
        if (entry) {
            const Word key = (read<Word>(resource, 8) >> 14) & 0x3fu;
            if (((key ^ read<Word>(entry, 0x28)) & 0x0fffffffu) == 0) return entry;
        }
        entry = reinterpret_cast<void*>(static_cast<std::uintptr_t>(unresolved_list_next_005e9b90(
            read<void*>(manager, list_offset), nullptr, &position)));
    }
    return nullptr;
}
}

extern "C" {
// 00408450 ServiceFromResFileRaw(res, size, out) RET 0Ch.
__declspec(noinline) Word __fastcall resman_read_raw_resfile_00408450(menu_resources::Manager* manager, void*, void* resource,
                                                                      Word size, char* out) noexcept(false) {
    if (!resource) return 0;                                                     // 00408452..60
    const Word bif = (read<Word>(resource, 8) >> 20) & 0x3ffu;                   // 0040846A..70 (captured first)
    void* const entry = find_entry(manager, resource, 0x10);                     // 00408463..BE
    if (!entry) return 0;                                                        // 004084C0..C6
    auto archive = [&]() { return read<void*>(read<void*>(entry, 0x30), bif * 4u); };
    effect(archive(), 0x04);                                                     // 004084C9..D1
    const Word available = slot<Sized>(archive(), 0x24)(archive(), nullptr, read<Word>(resource, 8) & 0x3fffu); // 004084D4..E5
    Word count = size;                                                           // 004084E8
    if (static_cast<std::int32_t>(count) > static_cast<std::int32_t>(available)) count = available; // 004084EC..F0 (JLE signed)
    write(resource, 0x18, count);                                                // 004084F8
    const Word got = slot<Transfer>(archive(), 0x34)(archive(), nullptr, read<Word>(resource, 8), out, count, 0u); // 004084F2..509
    if (got != count) return 0;                                                  // 0040850C..0E (no close)
    effect(archive(), 0x14);                                                     // 00408510..18
    return 1;
}
// 00408390 ServiceFromImageRaw(res, size, out) RET 0Ch.
__declspec(noinline) Word __fastcall resman_read_raw_image_00408390(menu_resources::Manager* manager, void*, void* resource,
                                                                    Word size, char* out) noexcept(false) {
    if (!resource) return 0;                                                     // 00408393..A2
    void* const entry = find_entry(manager, resource, 0x1c);                     // 004083A5..F1
    if (!entry) return 0;                                                        // 004083F3..F9
    void* const archive = read<void*>(read<void*>(entry, 0x30), 0);              // 004083FC..FF
    (void)slot<Sized>(archive, 0x24)(archive, nullptr, read<Word>(resource, 8) & 0x3fffu); // 00408401..0D (EAX discarded)
    write(resource, 0x18, size);                                                 // 00408410..17
    if (!c2da_manager_004077b0(manager, nullptr, resource)) return 0;            // 00408414..21
    void* const reader = read<void*>(read<void*>(entry, 0x30), 0);               // 00408423..26
    const Word got = slot<Transfer>(reader, 0x34)(reader, nullptr, read<Word>(resource, 8), out, size, 0u); // 00408428..36
    return got == size ? 1u : 0u;                                                // 00408439..42 (sub/neg/sbb/inc)
}
// 004082D0 ServiceFromEncapsulatedRaw(res, size, out) RET 0Ch.
__declspec(noinline) Word __fastcall resman_read_raw_encapsulated_004082d0(menu_resources::Manager* manager, void*, void* resource,
                                                                           Word size, char* out) noexcept(false) {
    if (!resource) return 0;                                                     // 004082D1..E0
    void* const entry = find_entry(manager, resource, 0x18);                     // 004082E3..2F
    if (!entry) return 0;                                                        // 00408331..36
    auto archive = [&]() { return read<void*>(read<void*>(entry, 0x30), 0); };
    effect(archive(), 0x04);                                                     // 00408339..41
    write(resource, 0x18, size);                                                 // 00408344..4E
    const Word got = slot<Transfer>(archive(), 0x34)(archive(), nullptr, read<Word>(resource, 8), out, size, 0u); // 00408348..5E
    if (got != size) return 0;                                                   // 00408361..64 (no close)
    c2da_manager_00407020(manager, nullptr, resource);                           // 00408366..69 CExoResMan::Free
    effect(archive(), 0x14);                                                     // 0040836E..75
    return 1;
}
}

namespace {
struct OptionalString {   // [esp+14h] bit 0 guards the destruction of the "rb" temporary (0040822B..0040823E)
    alignas(String) unsigned char storage[sizeof(String)];
    bool constructed = false;
    String* get() { return reinterpret_cast<String*>(storage); }
    ~OptionalString() { if (constructed) get()->~StartupString(); }
};
struct Temporary {        // completed operator+ results (states 2..4)
    alignas(String) unsigned char storage[sizeof(String)];
    bool constructed = false;
    String* get() { return reinterpret_cast<String*>(storage); }
    ~Temporary() { if (constructed) get()->~StartupString(); }
};
struct RawAllocation {    // state 5: free the 4-byte CExoFile allocation if its constructor throws
    void* raw;
    ~RawAllocation() { if (raw) live_free(raw); }
};
}

extern "C" {
// 00408080 ServiceFromDirectoryRaw(res, size, out) RET 0Ch (SEH 007143BA).
__declspec(noinline) Word __fastcall resman_read_raw_directory_00408080(menu_resources::Manager* manager, void*, void* resource,
                                                                        Word size, char* out) noexcept(false) {
    String path;                                                                 // 004080A5..AD [esp+28h] (state 0)
    String name;                                                                 // 004080B2..BA [esp+20h] (state 1)
    if (!resource || !read<void*>(resource, 0x14)) return 0;                     // 004080BF..CE -> 00408120
    void* const entry = find_entry(manager, resource, 0x14);                     // 004080D0..11E
    if (!entry) return 0;
    c2da_directory_ref_string_00405f70(read<menu_resources::Ref*>(resource, 0x14), nullptr, &name); // 00408155..5D
    {
        String slash(c2da_directory_literal_0073d8f4);                           // 00408162..6B "\\" (state 2)
        Temporary directory_slash;
        String* const first = GuiConcat005e5d10(reinterpret_cast<String*>(static_cast<unsigned char*>(entry) + 0x20),
                                                nullptr, directory_slash.get(), &slash);   // 00408170..82
        directory_slash.constructed = true;                                      // state 3
        Temporary directory_file;
        String* const second = GuiConcat005e5d10(first, nullptr, directory_file.get(), &name);  // 00408187..99
        directory_file.constructed = true;                                       // state 4
        path.Assign005e5c50(second);                                             // 0040819E..A8
    }                                                                            // 004081AD..D1 (three destructors)
    void* const raw = live_operator_new(4);                                      // 004081D6..DD
    void* file = nullptr;
    {
        RawAllocation guard{raw};                                                // state 5
        OptionalString mode;
        if (raw) {
            new (mode.storage) String(c2da_directory_literal_0073d5b0);          // 004081EF..F8 "rb" (state 6)
            mode.constructed = true;                                             // 00408214 [esp+14h] = 1
            const Word type = read<std::uint16_t>(read<void*>(resource, 0x14), 0x1a); // 004081FD..202
            file = c2da_file_construct_005e68d0(raw, nullptr, &path, type, mode.get()); // 00408206..220
        }
        guard.raw = nullptr;                                                     // state 1 (00408230)
    }                                                                            // 0040822B..3E ~"rb" when built
    if (!file) return 0;                                                         // 00408243..45
    if (!c2da_file_005e6a10(file, nullptr)) {                                    // 0040824B..56
        c2da_005e69e0(file, nullptr);                                            // 00408258
        live_free(file);                                                         // 0040825D
        return 0;
    }
    write(resource, 0x18, size);                                                 // 0040826B..71
    (void)c2da_file_005e6960(file, nullptr, out, size, 1u);                      // 0040826E..78 (EAX ignored)
    c2da_005e69e0(file, nullptr);                                                // 0040827D..7F
    live_free(file);                                                             // 00408284..85
    return 1;                                                                    // 0040828D..C2
}

// 00408E30 CExoResMan::ReadRaw(res, size, out) RET 0Ch: tail JMP by (key >> 30); default returns `out`.
__declspec(noinline) Word __fastcall resman_read_raw_00408e30(menu_resources::Manager* manager, void*, void* resource,
                                                              Word size, char* out) noexcept(false) {
    switch (read<Word>(resource, 8) >> 30) {                                     // 00408E30..3F
    case 0: return resman_read_raw_resfile_00408450(manager, nullptr, resource, size, out);   // 00408E61 JMP
    case 1: return resman_read_raw_image_00408390(manager, nullptr, resource, size, out);     // 00408E58 JMP
    case 2: return resman_read_raw_encapsulated_004082d0(manager, nullptr, resource, size, out); // 00408E4F JMP
    case 3: return resman_read_raw_directory_00408080(manager, nullptr, resource, size, out); // 00408E46 JMP
    }
    return static_cast<Word>(reinterpret_cast<std::uintptr_t>(out));            // 00408E6A (unreachable: key>>30 <= 3)
}

// 00409B20 CRes::ReadRaw(size, out) RET 8: [007A39E8]->ReadRaw(this, size, out).
__declspec(noinline) Word __fastcall resman_res_read_raw_00409b20(void* resource, void*, std::int32_t bytes, char* out) noexcept(false) {
    return resman_read_raw_00408e30(c2da_manager_007a39e8(), nullptr, resource, static_cast<Word>(bytes), out); // 00409B20..36
}
}
