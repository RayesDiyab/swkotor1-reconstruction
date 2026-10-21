// lane-m47-core rev2 ADAPTED COPY of the M46/M47 stage unit resman-object-bodies (resman-core/resman_objects.cpp, base sha256
// faf99c920c62295c66312922b398f29911a64476a314fdfa4736b4766db4d682). Only change: 00409B80 returns the 00408C90 EAX (D12).
// Compiles with the adapted resman_objects.h next to it (quoted include resolves here first).
// lane-resman: CExoResMan/CRes object-management bodies (donor reuse).
// Every body below is copied from a reviewed Task10 donor; only the import
// names were re-bound to the symbols that M40 already defines. No address
// credit is new: see PACKET.json donor_reused_or_authored.
#include "974dcbd34c-resman_objects.h"
#include <cstring>

namespace {
using Word = menu_resources::Word;
using Ref = menu_resources::Ref;
using Manager = menu_resources::Manager;
using String = menu_resources::String;
inline Word word(const void* p, unsigned offset) { Word v; std::memcpy(&v, static_cast<const unsigned char*>(p) + offset, 4); return v; }
inline void word(void* p, unsigned offset, Word v) { std::memcpy(static_cast<unsigned char*>(p) + offset, &v, 4); }
inline void* pointer(const void* p, unsigned offset) { return reinterpret_cast<void*>(word(p, offset)); }
inline void pointer(void* p, unsigned offset, void* v) { word(p, offset, reinterpret_cast<Word>(v)); }
template<class T> T load(const void* p, unsigned offset) { T value; std::memcpy(&value, static_cast<const unsigned char*>(p) + offset, sizeof(value)); return value; }
template<class T> void store(void* p, unsigned offset, T value) { std::memcpy(static_cast<unsigned char*>(p) + offset, &value, sizeof(value)); }
static_assert(sizeof(void*) == 4 && sizeof(Word) == 4 && sizeof(Ref) == 16 && sizeof(String) == 8, "native x86 carriers");
}

// ---------------------------------------------------------------------------
// 004074D0 CExoResMan::GetResObject(CResRef const&, unsigned short) RET8.
// Donor: 2026-10-02\task-10\c2da-resource-object-r1\source\lookup.cpp (sha256 e9bbb36b...),
// approved by c2da-resource-object-review-r1. Evidence: 004074F5-0040750D
// GetKeyEntry(ref,type,&table,&entry); 0040751F-00407526 entry+10 ->+1C ++;
// 0040752D table+1C==4 early return; 00407537-00407542 refs!=1 return;
// 00407548-00407567 three CExoString lifetimes; 00407570-00407592/0040762A-00407662
// kind switch BIF/DIR(re-probe manager+1C, writes incoming arg slots)/ERF;
// 00407597-004075E1 extension + CopyToString + four discarded CStr calls;
// 00407612 return entry+10 (reloaded).
extern "C" __declspec(noinline) void* __fastcall c2da_lookup_004074d0(Manager* manager, void*, Ref* incoming, Word type) {
    Ref* const saved_incoming = incoming;
    Word table_word = 0, entry_word = 0;
    if (!unresolved_key_entry_00407230(manager, nullptr, saved_incoming, type, &table_word, &entry_word)) return nullptr;
    void* const entry = reinterpret_cast<void*>(entry_word);
    void* existing = pointer(entry, 0x10);
    if (existing) {
        word(existing, 0x1c, word(existing, 0x1c) + 1u);
    }
    void* const table = reinterpret_cast<void*>(table_word);
    if (word(table, 0x1c) != 4u) {
        existing = pointer(entry, 0x10);
        if (!existing || word(existing, 0x1c) == 1u) {
            String name;
            String archive;
            String extension;
            const Word kind = word(table, 0x1c);
            bool describe = true;
            if (kind == 1u) {
                c2da_assign_text_005e5140(&archive, nullptr, c2da_archive_label_0073d8dc());
                describe = false;
            } else if (kind == 2u) {
                // The original lower call writes the actual incoming argument slots.
                const Word status = unresolved_key_entry_from_table_004071a0(manager, nullptr,
                    reinterpret_cast<unsigned char*>(manager) + 0x1c, saved_incoming, type,
                    reinterpret_cast<Word*>(&incoming), &type, 0);
                if (status) describe = false;
                else c2da_assign_text_005e5140(&archive, nullptr, c2da_archive_label_0073d8e0());
            } else if (kind == 3u) {
                c2da_assign_text_005e5140(&archive, nullptr, c2da_archive_label_0073d8e4());
            }
            if (describe) {
                std::uint16_t extension_type;
                std::memcpy(&extension_type, static_cast<unsigned char*>(entry) + 0x1a, 2);
                String* const suffix = c2da_base_005e6660(key_debug_root_007a39e0(), nullptr, extension_type);
                async_assign_string_005e5c50(&extension, nullptr, suffix);
                c2da_ref_to_string_00405f70(saved_incoming, nullptr, &name);
                (void)WindowCStr005e5670(&name, nullptr);
                (void)WindowCStr005e5670(&extension, nullptr);
                (void)WindowCStr005e5670(&archive, nullptr);
                (void)WindowCStr005e5670(reinterpret_cast<String*>(static_cast<unsigned char*>(table) + 0x20), nullptr);
            }
        }
    }
    return pointer(entry, 0x10);
}

// ---------------------------------------------------------------------------
// 00407680 CExoResMan::SetResObject(CResRef const&, unsigned short, CRes*) RET12.
// Donor: 2026-10-02\task-10\c2da-register-00407680-r1\source\register.cpp (sha256 b5685adf...),
// approved by c2da-resource-object-review-r1. Evidence: 00407686-00407690 null
// resource; 00407692-004076A6 GetKeyEntry(ref,type,&type_slot,&entry);
// 004076AF-004076B8 occupied; 004076BA-004076D0 entry+10=res, res+14=entry,
// [entry+10]+1C=1, [entry+10]+8=entry+14.
extern "C" __declspec(noinline) void __fastcall c2da_register_00407680(
    Manager* manager, void*, Ref* reference, Word type, void* resource) {
    Word entry_word = 0;
    if (!resource) return;
    Word* const output1 = &type;
    if (!unresolved_key_entry_00407230(manager, nullptr, reference, type, output1, &entry_word)) return;
    void* const entry = reinterpret_cast<void*>(entry_word);
    if (word(entry, 0x10)) return;
    pointer(entry, 0x10, resource);
    pointer(resource, 0x14, entry);
    word(pointer(entry, 0x10), 0x1c, 1);
    void* const linked = pointer(entry, 0x10);
    const Word metadata = word(entry, 0x14);
    word(linked, 8, metadata);
}

// ---------------------------------------------------------------------------
// 00408620 CRes::Request() RET0, EAX 0/1.
// Donor: 2026-10-02\task-10\resource-request-00408620-r3\source\request.cpp (sha256 c417b45d...)
// + export_bridge.cpp (a18dc325...), approved by resource-request-review-r3.
// Evidence: 00408625 manager read before exits; 0040862D flag bit 4; 00408637-0040864C
// u16 +6 nonzero increment; 0040864E-00408661 zero, AddTail(manager+24), increment.
extern "C" __declspec(noinline) int __fastcall resource_request_00408620(void* resource, void*) {
    Manager* manager = c2da_manager_007a39e8();
    if (!resource || (static_cast<const unsigned char*>(resource)[0x0c] & 4u)) return 0;
    const std::uint16_t existing = load<std::uint16_t>(resource, 6);
    if (existing != 0) {
        store(resource, 6, static_cast<std::uint16_t>(existing + 1u));
        return 1;
    }
    store(resource, 6, std::uint16_t{0});
    resource_append_005e9dc0(reinterpret_cast<kotor_list::Internal*>(pointer(manager, 0x24)), nullptr, resource);
    store(resource, 6, static_cast<std::uint16_t>(load<std::uint16_t>(resource, 6) + 1u));
    return 1;
}
// Type-correct support adapter for void consumers; no extra address credit.
extern "C" __declspec(noinline) void __fastcall c2da_request_00408620(void* resource, void* unused) {
    (void)resource_request_00408620(resource, unused);
}
// Support join (no address credit): request.h's extern "C" AddTail transport
// forwards to the single M40 owner ini_section::AddTail005e9dc0 (RET4, ECX list).
extern "C" __declspec(noinline) void __fastcall resource_append_005e9dc0(kotor_list::Internal* list, void*, void* payload) {
    (void)ini_section::AddTail005e9dc0(list, nullptr, payload);
}

// ---------------------------------------------------------------------------
// 00409B00 / 00409B10 / 00409B80 / 00409CF0 / 00406060
// Donor: 2026-10-02\task-10\c2da-current-resource-lower-r1\source\resource_lower.cpp
// (sha256 f305d237...), conditionally approved by c2da-current-resource-lower-review-r1.
// Evidence: 00409B01-00409B07 / 00409B11-00409B17 / 00409B81-00409B87 one
// [007A39E8] read then Manager Cancel/Demand/Release; 00409CF0-00409D39 ReleaseResObject.
extern "C" __declspec(noinline) void __fastcall c2da_cancel_00409b00(void* resource, void*) {
    c2da_manager_cancel_004088f0(c2da_manager_007a39e8(), nullptr, resource);
}
extern "C" __declspec(noinline) Word __fastcall c2da_current_resource_demand_00409b10(void* resource, void*) {
    return c2da_manager_demand_004089f0(c2da_manager_007a39e8(), nullptr, resource);
}
// rev2 (D12, m47-compose SND47-2): CRes::Release returns CExoResMan::Release's EAX unchanged (00409B87 call 00408C90;
// 00409B8C ret). The value is the pre-decrement count / -1 / 0 of the single 00408C90 body owner; lane-sound reads it
// (005DAE9E..005DAEAA through the /alternatename join of res_release_00409b80).
extern "C" __declspec(noinline) Word __fastcall c2da_release_demand_00409b80(void* resource, void*) {
    return c2da_manager_release_demand_body_00408c90(c2da_manager_007a39e8(), nullptr, resource);
}
extern "C" __declspec(noinline) int __fastcall c2da_release_00409cf0(Manager*, void*, void* resource) {
    if (!resource) return 0;
    Word count = load<Word>(resource, 0x1c);
    if (count > 1u) {
        --count; store(resource, 0x1c, count);
        int result; std::memcpy(&result, &count, 4); return result;
    }
    const std::uint16_t demands = load<std::uint16_t>(resource, 4);
    store(resource, 0x1c, Word{0});
    if (demands > 0u) return 0;
    void* key = load<void*>(resource, 0x14);
    if (key) store(key, 0x10, Word{0});
    c2da_resource_serviced_00409b40(resource, nullptr, 1u);
    Word flags = load<Word>(resource, 0x0c);
    if (flags & 8u) store(resource, 0x0c, flags | 0x200u);
    return 0;
}
// 00406060 CResRef::operator!=(char const*) — consumed at 0070F846 by every
// CResHelper::SetResRef; inverts the single M40 004060D0 algorithm.
extern "C" __declspec(noinline) int __fastcall c2da_not_equal_text_00406060(Ref* self, void*, const char* text) {
    return c2da_equal_text_004060d0(self, nullptr, text) == 0;
}

// ---------------------------------------------------------------------------
// Literal data: original .rdata 0073D8DC "BIF", 0073D8E0 "DIR", 0073D8E4 "ERF"
// (read from the pinned PE; strings only, not game assets).
namespace { const char label_bif_0073d8dc[] = "BIF"; const char label_dir_0073d8e0[] = "DIR"; const char label_erf_0073d8e4[] = "ERF"; }
extern "C" __declspec(noinline) const char* __cdecl c2da_archive_label_0073d8dc() { return label_bif_0073d8dc; }
extern "C" __declspec(noinline) const char* __cdecl c2da_archive_label_0073d8e0() { return label_dir_0073d8e0; }
extern "C" __declspec(noinline) const char* __cdecl c2da_archive_label_0073d8e4() { return label_erf_0073d8e4; }
