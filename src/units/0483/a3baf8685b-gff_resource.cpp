// Lane gff: CResGFF construction / callbacks / destruction (G2).
// Evidence: original swkotor.exe (GOG K1) ranges cited per statement.
#include "cd081f1aea-gff_resource.h"
#include "d2c5c47a35-allocator_live.h"   // live_free: original 006FA390 (cdecl JMP 006FB7B2)
#include <cstring>

namespace {
using namespace gff_resource;
template<class T> T load(const void* object, unsigned offset) {
    T value;
    std::memcpy(&value, static_cast<const unsigned char*>(object) + offset, sizeof(value));
    return value;
}
template<class T> void store(void* object, unsigned offset, T value) {
    std::memcpy(static_cast<unsigned char*>(object) + offset, &value, sizeof(value));
}

// Shared shape of 0041075E..004107B2 (OnResourceServiced) and 004115A4..004115F8
// (~CResGFF): free +70,+64,+5C,+54,+4C,+44,+40 in that order through 006FA390.
// Each next pointer is loaded after the previous free returns and before the
// previous cell is cleared; +40 is cleared last, after its own free.
void free_owned_sections(void* self) {
    static const unsigned order[] = { kListIndices, kFieldIndices, kFieldData, kLabels,
                                      kFields, kStructs, kHeader };
    live_free(load<void*>(self, order[0]));
    for (unsigned i = 1; i < sizeof(order) / sizeof(order[0]); ++i) {
        void* next = load<void*>(self, order[i]);
        store<Word>(self, order[i - 1], 0);
        live_free(next);
    }
    store<Word>(self, kHeader, 0);
}

// Zero the 15 dwords +40..+78 (00410673..0041069D; 004108F5..0041091F).
void clear_section_words(void* self) {
    for (unsigned offset = kHeader; offset <= kZeroEnd; offset += 4) store<Word>(self, offset, 0);
}
}

extern "C" {

const char gff_version_text_0073e2c8[8] = "V3.2";            // 0073E2C8 56 33 2E 32 00 00 00 00
const char* volatile gff_version_0078d3cc = gff_version_text_0073e2c8;   // 0078D3CC -> 0073E2C8

const gff_resource::Methods0073e2d0 gff_resource_method_table_0073e2d0{
    &gff_resource_deleting_destroy_00413010,     // 0073E2D0 = 00413010
    &c2da_resource_constant_word_00406490,       // 0073E2D4 = 00406490 (M40 constants.obj)
    &c2da_resource_constant_word_0063e7f0,       // 0073E2D8 = 0063E7F0 (M40 constants.obj)
    &gff_resource_freed_00410880,                // 0073E2DC = 00410880
    &gff_resource_serviced_00410740              // 0073E2E0 = 00410740
};

Gff* __fastcall gui_load_gff_construct_00410630(Gff* self, void*, Word type, const char* file_type,
                                                const Ref* name) {
    // 00410630..00410645: SEH frame, state -1 -> 0 at 004106D5 after the CRes base exists.
    // Unwind (handler 714D68 -> 00409D40) is not modelled; see PACKET review_needed.
    c2da_resource_base_ctor_00406460(self, nullptr);                 // 0041064E
    store<Word>(self, kCapacity38, 0xFD4u);                          // 0041065E
    store<Word>(self, kCapacity3C, 0xFD4u);                          // 00410661
    store<const void*>(self, 0, &gff_resource_method_table_0073e2d0); // 0041066D
    clear_section_words(self);                                       // 00410673..0041069D
    store<Word>(self, kConst28, 0x6Eu);                              // 004106A0
    store<Word>(self, kConst2C, 0x286u);                             // 004106A7
    store<Word>(self, kConst30, 0x62u);                              // 004106AE
    store<Word>(self, kConst34, 0x72Cu);                             // 004106B5
    store<unsigned char>(self, kByte8C, 0);                          // 004106BC
    store<Word>(self, kServiced, 0);                                 // 004106C2
    store<Word>(self, kBorrowed, 0);                                 // 004106C8
    store<Word>(self, kHeld, 0);                                     // 004106CE
    store<Word>(self, kFileType, 0);                                 // 004106D9
    // 004106DB: strncpy(this+8D, file_type, 4) (006FAB80 = CRT strncpy; same /MT
    // provider precedent as task-17 00411630). No terminator beyond 4 bytes.
    std::strncpy(reinterpret_cast<char*>(self) + kFileType, file_type, 4);
    // 004106E8..004106F4: [007A39E8]->SetResObject(name, type, this); type is the
    // caller's full stack dword (0040A6CE pushes 0x7FF).
    c2da_register_00407680(c2da_manager_007a39e8(), nullptr, const_cast<Ref*>(name), type, self);
    (void)c2da_current_resource_demand_00409b10(self, nullptr);      // 004106FB, EAX ignored
    if (load<void*>(self, kData) == nullptr) {                       // 00410700..00410703
        (void)c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, self); // 00410705..0041070C
    } else {
        store<Word>(self, kHeld, 1);                                 // 00410713
    }
    return self;                                                     // 00410721
}

Word __fastcall gff_resource_serviced_00410740(void* self, void*) {
    if (load<void*>(self, kData) == nullptr) return 0;               // 00410743..0041074B -> 0041080E
    if (load<void*>(self, kHeader) != nullptr && load<Word>(self, kBorrowed) == 0) // 00410751..0041075C
        free_owned_sections(self);                                   // 0041075E..004107B2
    // 004107B5..004107DF: tag = bytes +8D,+8E,+8F,+90 little-endian (one dword).
    const Word tag = load<Word>(self, kFileType);
    unsigned char* header = load<unsigned char*>(self, kData);       // 004107BC (reloaded)
    store<unsigned char*>(self, kHeader, header);                    // 004107C7
    if (load<Word>(header, 0) != tag) {                              // 004107E1..004107E3
        store<Word>(self, kHeader, 0);                               // 0041080B
        return 0;
    }
    // 004107E5..00410805: version dword assembled from the four bytes at [0078D3CC].
    const char* version = gff_version_0078d3cc;
    if (load<Word>(header, 4) != load<Word>(version, 0)) {
        store<Word>(self, kHeader, 0);                               // 0041080B
        return 0;
    }
    // 00410813..0041085E: each section base = header + offset when its count/size
    // word is nonzero; otherwise the cell is left unchanged. Uses the cached header.
    if (load<Word>(header, 0x0C)) store<unsigned char*>(self, kStructs, header + load<Word>(header, 0x08));
    if (load<Word>(header, 0x14)) store<unsigned char*>(self, kFields, header + load<Word>(header, 0x10));
    if (load<Word>(header, 0x1C)) store<unsigned char*>(self, kLabels, header + load<Word>(header, 0x18));
    if (load<Word>(header, 0x24)) store<unsigned char*>(self, kFieldData, header + load<Word>(header, 0x20));
    if (load<Word>(header, 0x2C)) store<unsigned char*>(self, kFieldIndices, header + load<Word>(header, 0x28));
    if (load<Word>(header, 0x34)) store<unsigned char*>(self, kListIndices, header + load<Word>(header, 0x30));
    store<Word>(self, kBorrowed, 1);                                 // 00410867
    store<Word>(self, kServiced, 1);                                 // 0041086D
    return 1;                                                        // 00410861
}

Word __fastcall gff_resource_freed_00410880(void* self, void*) {
    const Word borrowed = load<Word>(self, kBorrowed);               // 00410880
    store<Word>(self, kServiced, 0);                                 // 0041088A (before the test)
    if (borrowed != 0) {                                             // 00410888/00410890
        store<Word>(self, kListIndices, 0);                          // 00410892
        store<Word>(self, kFieldIndices, 0);                         // 00410895
        store<Word>(self, kFieldData, 0);                            // 00410898
        store<Word>(self, kLabels, 0);                               // 0041089B
        store<Word>(self, kFields, 0);                               // 0041089E
        store<Word>(self, kStructs, 0);                              // 004108A1
        store<Word>(self, kHeader, 0);                               // 004108A4
        store<Word>(self, kBorrowed, 0);                             // 004108A7
    }
    return 1;                                                        // 004108AD
}

void __fastcall gff_resource_release_004108c0(void* self, void*) {
    if (load<Word>(self, kBorrowed) != 0) {                          // 004108C3..004108CE
        if (load<Word>(self, kHeld) != 0) {                          // 004108D0..004108D6
            c2da_release_demand_00409b80(self, nullptr);             // 004108D8 CRes::Release
            (void)c2da_release_00409cf0(c2da_manager_007a39e8(), nullptr, self); // 004108DD..E4
            store<Word>(self, kHeld, 0);                             // 004108E9
        }
        store<Word>(self, kBorrowed, 0);                             // 004108EF
    }
    clear_section_words(self);                                       // 004108F5..0041091F
}

void __fastcall gff_resource_destroy_00411570(void* self, void*) {
    // 00411570..0041158A SEH frame (state 0 until 00411606); unwind not modelled.
    store<const void*>(self, 0, &gff_resource_method_table_0073e2d0); // 0041158E
    if (load<Word>(self, kBorrowed) == 0) {                          // 00411594..004115A2
        free_owned_sections(self);                                   // 004115A4..004115F8
    } else {
        gff_resource_release_004108c0(self, nullptr);                // 004115FD..004115FF
    }
    c2da_resource_base_destroy_00409d40(self, nullptr);              // 0041160E
}

void* __fastcall gff_resource_deleting_destroy_00413010(void* self, void*, Word flags) {
    gff_resource_destroy_00411570(self, nullptr);                    // 00413013
    if ((flags & 1u) != 0u) live_free(self);                         // 00413018..00413020
    return self;                                                     // 00413028, RET 4
}

}
