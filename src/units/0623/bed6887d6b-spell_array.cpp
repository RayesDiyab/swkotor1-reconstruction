// lane-rules (fork A): CSWSpellArray::Load 0059BA20, CSWSpell::CSWSpell 0059B310,
// CSWSpell::~CSWSpell 0059B5B0, CSWSpell::SetPrerequisites 0059B700,
// CSWSpell::SetMasterSpells 0059B7D0, CSWSpellArray::~CSWSpellArray 0059B9E0
// (swkotor.exe GOG K1 sha256 9c10e045..., base 0x400000). D1 form, C++ linkage.
// Disassembly: lane-rules/orig/59ba20_spell_nb.txt, orig/59b310_spell_subtree.txt.
// Receivers are minimum addressed views of the original allocations (only cited
// offsets touched). Original .rdata strings come from the D13 generated unit
// (kotor_rules_str_*); "" (0073D71C) is the M41 accessor c2da_empty_0073d71c().
#include "bc7bc38e49-spell_array.h"
#include <initializer_list>

namespace kotor_rules {
namespace {
using kotor_reconstruction::StartupString;

// Hidden-return CExoString temporary (left/right/lower/concat output slot): the
// callee constructs into the slot; the EH state that covers it destroys it with
// 005E5C20 on scope exit or unwind.
struct Fresh {
    alignas(4) unsigned char storage[sizeof(StartupString)];
    StartupString* live = nullptr;
    StartupString* slot() noexcept { return reinterpret_cast<StartupString*>(storage); }
    Fresh() = default;
    Fresh(const Fresh&) = delete;
    Fresh& operator=(const Fresh&) = delete;
    ~Fresh() { if (live) live->~StartupString(); }
};

inline std::int32_t not_equal(StartupString* s, const char* text) {
    // 005E5390 CExoString::operator!=(char const*) (lane-config owner; object view)
    return client_movies::StringNotEqual_005e5390(reinterpret_cast<options_video::String*>(s), nullptr, text);
}

// [edi+4] + row*198h, re-read at every use (e.g. 0059BB52, 0059BBA2, 0059C0DA ...).
inline unsigned char* spell_at(void* array, Word offset) noexcept {
    return ptr_at<unsigned char>(array, 4) + offset;
}

// 0059BBF3..0059BDE8 (and 0059BE3E..0059C03C): prepend "_" unless the first char is
// "_" or ":"; append "_" unless the last char is "_" or ":".
void affix_separators(StartupString* s) {
    bool flag;
    {
        Fresh first;                                                            // [esp+208h] state 6
        first.live = base_ini_reconstruction::left_005e5f70(s, nullptr, first.slot(), 1); // 0059BBF3..0059BC01
        if (not_equal(first.live, kotor_rules_str_0073d704) != 0) {            // 0059BC06..0059BC1C "_"
            Fresh second;                                                       // [esp+1F8h] state 7
            second.live = base_ini_reconstruction::left_005e5f70(s, nullptr, second.slot(), 1); // 0059BC1E..0059BC2C
            const std::int32_t ne = not_equal(second.live, kotor_rules_str_0073e2a0); // 0059BC31..0059BC40 ":"
            flag = true;                                                        // 0059BC63 (set before test)
            if (ne == 0) flag = false;                                          // 0059BC68 jne / 0059BC6A
        } else {                                                                // 0059BC1C je 0059BC6A
            flag = false;                                                       // 0059BC6A
        }
    }                                                                           // 0059BC6F..0059BC7E ~first
    if (flag) {                                                                 // 0059BC83..0059BC89
        StartupString underscore(kotor_rules_str_0073d704);                    // 0059BC8B..0059BC97 [esp+288h] state 8
        Fresh joined;                                                           // [esp+290h] state 9
        joined.live = GuiConcat005e5d10(&underscore, nullptr, joined.slot(), s); // 0059BC9C..0059BCB3 "_" + s
        s->Assign005e5c50(joined.live);                                        // 0059BCB8..0059BCC5
    }                                                                           // 0059BCCA..0059BCED ~joined, ~underscore
    {
        Fresh first;                                                            // [esp+258h] state 0Ah
        first.live = base_ini_reconstruction::right_005e6170(s, nullptr, first.slot(), 1); // 0059BCF2..0059BD00
        if (not_equal(first.live, kotor_rules_str_0073d704) != 0) {            // 0059BD05..0059BD1B
            Fresh second;                                                       // [esp+218h] state 0Bh
            second.live = base_ini_reconstruction::right_005e6170(s, nullptr, second.slot(), 1); // 0059BD1D..0059BD2B
            const std::int32_t ne = not_equal(second.live, kotor_rules_str_0073e2a0); // 0059BD30..0059BD3F
            flag = true;                                                        // 0059BD62
            if (ne == 0) flag = false;                                          // 0059BD67/0059BD69
        } else {
            flag = false;                                                       // 0059BD69
        }
    }                                                                           // 0059BD6E..0059BD7D
    if (flag) {                                                                 // 0059BD82..0059BD88
        StartupString underscore(kotor_rules_str_0073d704);                    // 0059BD8A..0059BD93 [esp+58h] state 0Ch
        Fresh joined;                                                           // [esp+228h] state 0Dh
        joined.live = GuiConcat005e5d10(s, nullptr, joined.slot(), &underscore); // 0059BD98..0059BDB1 s + "_"
        s->Assign005e5c50(joined.live);                                        // 0059BDB6..0059BDC3
    }                                                                           // 0059BDC8..0059BDE8
}

// GetCExoStringEntry -> CResRef(CExoString) 00406D60 -> CResRef::operator= 00406120
// (hidden by-value return slot, ignored). The element pointer is re-read after
// 00406D60 (e.g. 0059C230 after 0059C22B).
void assign_ref(void* array, Word row_offset, unsigned field, StartupString* text) {
    Ref converted;                                                              // [esp+368h]-style slot
    async_ref_construct_string_00406d60(&converted, nullptr, text);            // 00406D60
    Ref returned;                                                               // [esp+46Ch]-style hidden return
    c2da_copy_00406120(reinterpret_cast<Ref*>(spell_at(array, row_offset) + field), nullptr, &returned, &converted); // 00406120
}

// GetCExoStringEntry + 005E6080 LowerCase + 005E5C50 assign back (e.g. 0059C883..0059C8B5).
void lower_in_place(StartupString* s) {
    Fresh lowered;                                                              // [esp+240h] etc.
    lowered.live = directory_list::LowerCase005e6080(s, nullptr, lowered.slot());
    s->Assign005e5c50(lowered.live);
}

// 0059B700 / 0059B7D0 shared algorithm (identical code, differing only in the
// assigned member and the five-int table offset).
void parse_five(unsigned char* self, unsigned string_member, unsigned table, StartupString* text) {
    reinterpret_cast<StartupString*>(self + string_member)->Assign005e5c50(text); // 0059B70B..0059B712
    const std::int32_t length = text->BorrowedLength005e5790();                 // 0059B717..0059B71E
    std::int32_t i = 0;                                                         // 0059B720
    if (length > 0) {                                                           // 0059B722..0059B728
        do {
            if (directory_list::At005e5600(text, nullptr, i) != '_' &&          // 0059B730..0059B73A
                directory_list::At005e5600(text, nullptr, i) != ':')            // 0059B73C..0059B746
                break;
            ++i;                                                                // 0059B748
        } while (i < length);                                                   // 0059B749..0059B74B
    }
    unsigned char* out = self + table;                                          // 0059B74D
    for (int remaining = 5; remaining != 0; --remaining, out += 4) {           // 0059B753 [esp+18h]=5 .. 0059B7BB
        Word acc = 0;                                                           // 0059B764
        wr32(out, 0, 0xFFFFFFFFu);                                        // 0059B768 dword -1
        if (i >= length) continue;                                              // 0059B766..0059B76F
        if (directory_list::At005e5600(text, nullptr, i) != '_') {              // 0059B771..0059B77B
            // Original quirk: the digit loop stops only at '_' or ':'. 005E5600 returns 0
            // past the end, so a field without a terminating separator never stops; the
            // Load caller (affix_separators) appends "_" first.
            do {
                if (directory_list::At005e5600(text, nullptr, i) == ':') break; // 0059B780..0059B78A
                const unsigned char c = static_cast<unsigned char>(directory_list::At005e5600(text, nullptr, i)); // 0059B78C..0059B794 movzx
                acc = c + acc * 10u - 0x30u;                                    // 0059B797..0059B79A lea (DWORD wrap)
                ++i;                                                            // 0059B79E
            } while (directory_list::At005e5600(text, nullptr, i) != '_');      // 0059B79F..0059B7A9
        }
        ++i;                                                                    // 0059B7AB
        wr32(out, 0, acc);                                                      // 0059B7AC
    }
}

std::int32_t matched_index(StartupString* s, const char* const* names, const std::int32_t* values, int n, std::int32_t otherwise) {
    for (int k = 0; k < n; ++k)
        if (s->BorrowedEquals005e5290(names[k]) != 0) return values[k];
    return otherwise;
}
} // namespace

// ---------------------------------------------------------------- 0059B310
void* __fastcall kotor_fastcall_0059b310(void* self, void*) {
    unsigned char* const b = bytes(self);
    // EH frame 007211D5: state k destroys the CExoString members constructed so far.
    static const unsigned string_members[6] = {0x00, 0x20, 0x28, 0x34, 0x150, 0x158};
    int constructed = 0;
    try {
        ::new (b + 0x00) StartupString(); constructed = 1;                     // 0059B332 005B3190, state 0 (0059B33C)
        c2da_default_ref_00405ed0(reinterpret_cast<Ref*>(b + 0x10), nullptr);  // 0059B339..0059B340
        ::new (b + 0x20) StartupString(); constructed = 2;                     // 0059B345..0059B348, state 1
        ::new (b + 0x28) StartupString(); constructed = 3;                     // 0059B34D..0059B355, state 2
        ::new (b + 0x34) StartupString(); constructed = 4;                     // 0059B35A..0059B364, state 3
        for (unsigned off : {0x46u, 0x56u, 0x66u, 0x76u, 0x86u, 0x96u, 0xACu, 0xBCu, 0xCCu, 0xDCu, 0xF8u, 0x10Du})
            c2da_default_ref_00405ed0(reinterpret_cast<Ref*>(b + off), nullptr); // 0059B369..0059B3E3
        ::new (b + 0x150) StartupString(); constructed = 5;                    // 0059B3E8..0059B3EE, state 4
        ::new (b + 0x158) StartupString(); constructed = 6;                    // 0059B3F3..0059B3FE, state 5
        Ref discarded;                                                          // [esp+14h] hidden return of 00406290
        wr32(b, 0x40, 0xBB8u);                                                  // 0059B415
        unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(b + 0x76), nullptr, &discarded, kotor_rules_str_0074a190); // 0059B403..0059B41C
        unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(b + 0x86), nullptr, &discarded, kotor_rules_str_0074a184); // 0059B421..0059B431
        unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(b + 0x96), nullptr, &discarded, c2da_empty_0073d71c());     // 0059B436..0059B446
        unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(b + 0xDC), nullptr, &discarded, c2da_empty_0073d71c());     // 0059B44B..0059B45B
        wr16(b, 0x30, 0);                                                       // 0059B460
        GuiAssign005e5140(reinterpret_cast<StartupString*>(b + 0x34), nullptr, kotor_rules_str_0073df6c); // 0059B464..0059B46B
        wr32(b, 0xF4, 0);                                                       // 0059B480
        unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(b + 0xF8), nullptr, &discarded, kotor_rules_str_0074a178); // 0059B470..0059B486
        wr32(b, 0x108, 1);                                                      // 0059B49B
        unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(b + 0x10D), nullptr, &discarded, kotor_rules_str_0074a170); // 0059B48B..0059B4A5
        for (unsigned off : {0xACu, 0xBCu, 0xCCu, 0x46u, 0x56u, 0x66u})
            unresolved_ref_assign_00406290(reinterpret_cast<Ref*>(b + off), nullptr, &discarded, c2da_empty_0073d71c()); // 0059B4AA..0059B519
    } catch (...) {
        while (constructed > 0)                                                 // funclet 007211D5 (reverse order)
            reinterpret_cast<StartupString*>(b + string_members[--constructed])->~StartupString();
        throw;
    }
    wr32(b, 0x138, 0xFFFFFFFFu); wr32(b, 0x13C, 0xFFFFFFFFu);                  // 0059B522..0059B52B
    wr8(b, 0x10C, 0); wr32(b, 0xA8, 0); wr8(b, 0x11D, 0); wr8(b, 0x11E, 0);    // 0059B532..0059B544
    wr32(b, 0x120, 0); wr32(b, 0x12C, 0); wr8(b, 0x130, 0); wr32(b, 0x134, 0); // 0059B54A..0059B55C
    wr8(b, 0x140, 0); wr32(b, 0x144, 0); wr32(b, 0x148, 0); wr32(b, 0x188, 0); // 0059B562..0059B574
    wr32(b, 0x18C, 0); wr32(b, 0x190, 0); wr8(b, 0x194, 0); wr32(b, 0xEC, 0);  // 0059B57A..0059B58C
    wr16(b, 0xF0, 0);                                                           // 0059B592
    return self;                                                                // 0059B599 mov eax,esi
}

// ---------------------------------------------------------------- 0059B5B0
void __fastcall kotor_fastcall_0059b5b0(void* self, void*) {
    unsigned char* const b = bytes(self);
    if (void* p = ptr_at<void>(b, 0x12C)) live_free(p);                        // 0059B5CD..0059B5E5 (field not cleared)
    for (unsigned off : {0x158u, 0x150u, 0x34u, 0x28u, 0x20u, 0x00u})          // 0059B5E8..0059B639 states 4..-1
        reinterpret_cast<StartupString*>(b + off)->~StartupString();           // 005E5C20
}

void __fastcall kotor_fastcall_0059b5b0_array_destroy(StartupString* self, void*) {
    kotor_fastcall_0059b5b0(self, nullptr);                                     // D24 typed view (same body)
}

// ---------------------------------------------------------------- 0059B700 / 0059B7D0
void __fastcall kotor_fastcall_0059b700(void* self, void*, StartupString* text) {
    parse_five(bytes(self), 0x150, 0x160, text);                                // 0059B70C (+150h) / 0059B74D (+160h)
}
void __fastcall kotor_fastcall_0059b7d0(void* self, void*, StartupString* text) {
    parse_five(bytes(self), 0x158, 0x174, text);                                // 0059B7DC (+158h) / 0059B81D (+174h)
}

// ---------------------------------------------------------------- 0059B9E0
void __fastcall kotor_fastcall_0059b9e0(void* self, void*) {
    unsigned char* const array = ptr_at<unsigned char>(self, 4);               // 0059B9E3
    if (array == nullptr) return;                                               // 0059B9E6..0059B9E8
    c2da_destroy_array_006fa852(array, 0x198, rd32(array - 4, 0),               // 0059B9EA..0059B9FD (count at [arr-4])
                                &kotor_fastcall_0059b5b0_array_destroy);
    live_free(array - 4);                                                       // 0059BA02..0059BA03
    wr32(self, 4, 0);                                                           // 0059BA0B
    wr32(self, 0, 0);                                                           // 0059BA12
}

// ---------------------------------------------------------------- 0059BA20
void __fastcall kotor_fastcall_0059ba20(void* self, void*) {
    Table* const table = new_c2da(kotor_rules_str_0074a4d0);                    // 0059BA45..0059BA7F (state 0 frees 54h block)
    c2da_current_load_004143b0(table, nullptr);                                 // 0059BA81..0059BA8E (also when the allocation failed)
    const std::int32_t rows = row_count(table);                                 // 0059BA93 [ebx+24h]
    wr32(self, 0, static_cast<Word>(rows));                                     // 0059BAA2
    {
        void* const block = live_operator_new(static_cast<Word>(rows) * 0x198u + 4u); // 0059BA96..0059BAA4 (DWORD wrap)
        unsigned char* elements = nullptr;                                      // 0059BADE xor eax,eax
        if (block != nullptr) {                                                 // 0059BAB0..0059BABD
            wr32(block, 0, static_cast<Word>(rows));                            // 0059BAD3 count cookie
            try {                                                               // state 1 (0059BAB2): free block on unwind
                crt_eh_vector_constructor_006fb55f(bytes(block) + 4, 0x198, rows, // 0059BABF..0059BAD5
                                                   &kotor_fastcall_0059b310, &kotor_fastcall_0059b5b0);
            } catch (...) { live_free(block); throw; }
            elements = bytes(block) + 4;                                        // 0059BADA
        }
        put_ptr(self, 4, elements);                                             // 0059BAEF
    }
    StartupString text;                                                         // 0059BAE0..0059BAF2 [esp+1Ch], state 2
    // [esp+14h] scratch int: every C2DA getter writes it on both paths before it is read
    // (00413660 default branch 00413796..004137A4), so the initializer is never observed.
    std::int32_t value = 0;
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(rd32(self, 0)); ++row) { // 0059BAF7..0059BB08 / 0059D77B..0059D780 (count re-read)
        const Word at = static_cast<Word>(row) * 0x198u;                        // 0059BB59..0059BB5B esi
        int_entry(table, row, kotor_rules_str_007489d0, &value);                // 0059BB10..0059BB4D
        wr32(spell_at(self, at), 0x124, static_cast<Word>(value));              // 0059BB52..0059BB63
        int_entry(table, row, kotor_rules_str_007489c8, &value);                // 0059BB69..0059BB9D
        wr32(spell_at(self, at), 0x128, static_cast<Word>(value));              // 0059BBA2..0059BBAB

        string_entry(table, row, kotor_rules_str_0074a4c0, &text);              // 0059BBB1..0059BBEE
        affix_separators(&text);                                                // 0059BBF3..0059BDE8
        kotor_fastcall_0059b700(spell_at(self, at), nullptr, &text);            // 0059BDED..0059BDF7
        string_entry(table, row, kotor_rules_str_0074a4b4, &text);              // 0059BDFC..0059BE39
        affix_separators(&text);                                                // 0059BE3E..0059C03C
        kotor_fastcall_0059b7d0(spell_at(self, at), nullptr, &text);            // 0059C041..0059C04B

        string_entry(table, row, kotor_rules_str_00749490, &text);              // 0059C050..0059C08D
        reinterpret_cast<StartupString*>(spell_at(self, at))->Assign005e5c50(&text); // 0059C092..0059C09C (+0)
        int_entry(table, row, kotor_rules_str_00745eec, &value);                // 0059C0A1..0059C0D5
        wr32(spell_at(self, at), 0x08, static_cast<Word>(value));               // 0059C0DA..0059C0EF
        int_entry(table, row, kotor_rules_str_0074a4a8, &value);                // 0059C0E3..0059C123
        wr8(spell_at(self, at), 0x14E, static_cast<unsigned char>(value));      // 0059C128..0059C13D
        string_entry(table, row, kotor_rules_str_0074a49c, &text);              // 0059C131..0059C174
        {
            const char first = directory_list::At005e5600(&text, nullptr, 0);   // 0059C179..0059C17F
            wr8(spell_at(self, at), 0x14F, static_cast<unsigned char>(first));  // 0059C184..0059C189
        }
        int_entry(table, row, kotor_rules_str_0074a490, &value);                // 0059C18F..0059C1CC
        wr32(spell_at(self, at), 0x0C, static_cast<Word>(value));               // 0059C1D1..0059C1DA
        string_entry(table, row, kotor_rules_str_00746b04, &text);              // 0059C1DD..0059C21A
        assign_ref(self, at, 0x10, &text);                                      // 0059C21F..0059C248
        string_entry(table, row, kotor_rules_str_0074a488, &text);              // 0059C24D..0059C28A
        {
            Fresh first;                                                        // [esp+230h] state 1Eh
            first.live = base_ini_reconstruction::left_005e5f70(&text, nullptr, first.slot(), 1); // 0059C28F..0059C29D
            reinterpret_cast<StartupString*>(spell_at(self, at) + 0x20)->Assign005e5c50(first.live); // 0059C2A2..0059C2B3
        }                                                                       // 0059C2B8..0059C2C7
        int_entry(table, row, kotor_rules_str_0074a47c, &value);                // 0059C2CC..0059C309
        wr16(spell_at(self, at), 0x30, static_cast<std::uint16_t>(value));      // 0059C30E..0059C318
        string_entry(table, row, kotor_rules_str_0074a46c, &text);              // 0059C31C..0059C359
        reinterpret_cast<StartupString*>(spell_at(self, at) + 0x34)->Assign005e5c50(&text); // 0059C35E..0059C36B
        wr8(spell_at(self, at), 0x3C, 0xFF);                                    // 0059C370..0059C375
        wr8(spell_at(self, at), 0x3D, 0xFF);                                    // 0059C379..0059C37E
        wr8(spell_at(self, at), 0x3E, 0xFF);                                    // 0059C382..0059C387
        wr8(spell_at(self, at), 0x3F, 0xFF);                                    // 0059C38B..0059C39C
        {
            const char* const columns[4] = {kotor_rules_str_0074a460, kotor_rules_str_0074a454,
                                            kotor_rules_str_0074a448, kotor_rules_str_0074a440};
            for (unsigned k = 0; k < 4; ++k)                                    // 0059C390..0059C4F9
                if (int_entry(table, row, columns[k], &value) != 0)
                    wr8(spell_at(self, at), 0x3C + k, static_cast<unsigned char>(value));
        }
        int_entry(table, row, kotor_rules_str_0074a434, &value);                // 0059C4FC..0059C539
        wr32(spell_at(self, at), 0x40, static_cast<Word>(value));               // 0059C53E..0059C547
        {
            const char* const columns[6] = {kotor_rules_str_0074a424, kotor_rules_str_0074a414, kotor_rules_str_0074a404,
                                            kotor_rules_str_0074a3f4, kotor_rules_str_0074a3e4, kotor_rules_str_0074a3d4};
            static const unsigned fields[6] = {0x76, 0x86, 0x96, 0x46, 0x56, 0x66};
            for (unsigned k = 0; k < 6; ++k) {                                  // 0059C54A..0059C7EB
                string_entry(table, row, columns[k], &text);
                assign_ref(self, at, fields[k], &text);
            }
        }
        int_entry(table, row, kotor_rules_str_0074a3c8, &value);                // 0059C7F0..0059C82D
        wr32(spell_at(self, at), 0xA8, static_cast<Word>(value));               // 0059C832..0059C83B
        string_entry(table, row, kotor_rules_str_0074a3bc, &text);              // 0059C841..0059C87E
        lower_in_place(&text);                                                  // 0059C883..0059C8B5
        {
            static const char* const names[9] = {kotor_rules_str_0074a3b4, kotor_rules_str_0074a3ac, kotor_rules_str_0074a3a8,
                                                 kotor_rules_str_0074a3a0, kotor_rules_str_0074a398, kotor_rules_str_0074a390,
                                                 kotor_rules_str_0074a388, kotor_rules_str_0074a380, kotor_rules_str_0074a378};
            static const std::int32_t values[9] = {2, 1, 3, 4, 6, 6, 7, 8, 9}; // 0059C8BA..0059C99D (last: neg/sbb/and 9)
            value = matched_index(&text, names, values, 9, 0);
            wr16(spell_at(self, at), 0xA6, static_cast<std::uint16_t>(value));  // 0059C9A0..0059C9A7
            wr16(spell_at(self, at), 0x44, static_cast<std::uint16_t>(value));  // 0059C9AF..0059C9C7
        }
        {
            const char* const columns[4] = {kotor_rules_str_0074a368, kotor_rules_str_0074a358,
                                            kotor_rules_str_0074a34c, kotor_rules_str_0074a33c};
            static const unsigned fields[4] = {0xAC, 0xBC, 0xDC, 0xCC};
            for (unsigned k = 0; k < 4; ++k) {                                  // 0059C9BB..0059CB74
                string_entry(table, row, columns[k], &text);
                assign_ref(self, at, fields[k], &text);
            }
        }
        int_entry(table, row, kotor_rules_str_0074a330, &value);                // 0059CB79..0059CBB6
        wr32(spell_at(self, at), 0xEC, static_cast<Word>(value));               // 0059CBBB..0059CBD0
        string_entry(table, row, kotor_rules_str_0074a324, &text);              // 0059CBC4..0059CC07
        lower_in_place(&text);                                                  // 0059CC0C..0059CC3E
        if (text.BorrowedEquals005e5290(kotor_rules_str_0074a31c) != 0)         // 0059CC43..0059CC53
            value = 0;                                                          // 0059CC55 (overwritten by the next getter)

        // Five-slot local table [esp+2Ch..3Ch] and its count [esp+28h] (0059CC5D..0059CC7F).
        Word cost[5] = {0, 0, 0, 0, 0};
        std::int32_t count = 0;
        if (int_entry(table, row, kotor_rules_str_0074a30c, &value) != 0) {     // 0059CC5F..0059CCC3
            cost[0] = (static_cast<Word>(value) * 5u + 0x5DCu) << 1;           // 0059CCC5..0059CCDE
            int_entry(table, row, kotor_rules_str_0074a2fc, &value);            // 0059CCD2..0059CD13 (result ignored)
            cost[0] += static_cast<Word>(value);                                // 0059CD18..0059CD22
            count = 1;                                                          // 0059CD26 (store, not increment)
        }
        if (int_entry(table, row, kotor_rules_str_0074a2ec, &value) != 0) {     // 0059CD2E..0059CD7A
            Word& slot = cost[count];                                           // 0059CD80..0059CD84
            slot = (static_cast<Word>(value) * 5u + 0x1F4u) << 1;              // 0059CD88..0059CD95
            int_entry(table, row, kotor_rules_str_0074a2fc, &value);            // 0059CD97..0059CDD4
            slot += static_cast<Word>(value);                                   // 0059CDD9..0059CDE5
            ++count;                                                            // 0059CDE7
        }
        if (int_entry(table, row, kotor_rules_str_0074a2dc, &value) != 0) {     // 0059CDEB..0059CE37
            Word& slot = cost[count];                                           // 0059CE39..0059CE41
            slot = (static_cast<Word>(value) * 5u + 0x3E8u) << 1;              // 0059CE45..0059CE52
            int_entry(table, row, kotor_rules_str_0074a2fc, &value);            // 0059CE54..0059CE91
            slot += static_cast<Word>(value);                                   // 0059CE96..0059CEA2
            ++count;                                                            // 0059CEA4
        }
        unsigned char stored = static_cast<unsigned char>(count);              // 0059CEA8..0059CEAC [esp+18h]
        {
            unsigned char* const spell = spell_at(self, at);                    // 0059CEB0..0059CEBE
            if (void* old = ptr_at<void>(spell, 0x12C)) {                       // 0059CEB3..0059CEC2
                live_free(old);                                                 // 0059CEC4..0059CEC5
                wr32(spell, 0x12C, 0);                                          // 0059CED1
            }
            if (stored > 0) {                                                   // 0059CEDD..0059CEE3 (jbe)
                void* const list = live_operator_new(static_cast<Word>(stored) * 4u); // 0059CEE5..0059CEEC
                put_ptr(spell, 0x12C, list);                                    // 0059CEF8
                if (list == nullptr) stored = 0;                                // 0059CEFE..0059CF04
            }
            wr8(spell, 0x130, stored);                                          // 0059CF09..0059CF0D
        }
        for (std::int32_t k = 0; k < count; ++k) {                              // 0059CF13..0059CF51
            unsigned char* const spell = spell_at(self, at);                    // 0059CF25..0059CF2C
            if (static_cast<unsigned char>(k) < rd8(spell, 0x130))              // 0059CF2E..0059CF34 (byte compare)
                wr32(ptr_at<unsigned char>(spell, 0x12C), static_cast<unsigned char>(k) * 4u, cost[k]); // 0059CF36..0059CF3F
        }
        if (int_entry(table, row, kotor_rules_str_0074a2d0, &value) != 0)       // 0059CF53..0059CFA2
            wr32(spell_at(self, at), 0x13C, static_cast<Word>(value));          // 0059CFA4..0059CFAA
        else
            wr32(spell_at(self, at), 0x13C, 0xFFFFFFFFu);                       // 0059CFB2..0059CFB4
        int_entry(table, row, kotor_rules_str_0074a2c8, &value);                // 0059CFBE..0059CFFB
        wr32(spell_at(self, at), 0xF4, static_cast<Word>(value));               // 0059D000..0059D009
        if (value == 0) {                                                       // 0059D00F..0059D015
            GuiAssign005e5140(&text, nullptr, kotor_rules_str_0074a178);        // 0059D01B..0059D024
            assign_ref(self, at, 0xF8, &text);                                  // 0059D029..0059D055
            wr32(spell_at(self, at), 0x108, 1);                                 // 0059D05A..0059D06B
            Ref named;                                                          // [esp+318h]
            named.construct_00406d80(kotor_rules_str_0074a170);                 // 0059D05F..0059D075
            Ref returned;
            c2da_copy_00406120(reinterpret_cast<Ref*>(spell_at(self, at) + 0x10D), nullptr, &returned, &named); // 0059D07A..0059D095
        } else {
            string_entry(table, row, kotor_rules_str_0074a2bc, &text);          // 0059D09F..0059D0DC
            assign_ref(self, at, 0xF8, &text);                                  // 0059D0E1..0059D10D
            string_entry(table, row, kotor_rules_str_0074a2b0, &text);          // 0059D112..0059D14F
            lower_in_place(&text);                                              // 0059D154..0059D186
            value = 0;                                                          // 0059D194
            {
                // 0059D18B..0059D2CF; 0074A25C appears twice (second test unreachable).
                static const char* const names[11] = {kotor_rules_str_0074a2a8, kotor_rules_str_0074a29c, kotor_rules_str_0074a28c,
                                                      kotor_rules_str_0074a284, kotor_rules_str_0074a274, kotor_rules_str_0074a26c,
                                                      kotor_rules_str_0074a264, kotor_rules_str_0074a25c, kotor_rules_str_007429ec,
                                                      kotor_rules_str_0074a25c, kotor_rules_str_0074a254};
                static const std::int32_t values[11] = {1, 2, 3, 9, 5, 6, 7, 0x0B, 8, 0x0B, 0x32};
                value = matched_index(&text, names, values, 11, value);
            }
            wr32(spell_at(self, at), 0x108, static_cast<Word>(value));          // 0059D2D7..0059D2E0
            string_entry(table, row, kotor_rules_str_0074a244, &text);          // 0059D2E6..0059D323
            lower_in_place(&text);                                              // 0059D328..0059D35A
            {
                static const char* const names[3] = {kotor_rules_str_0074a23c, kotor_rules_str_0074a234, kotor_rules_str_0074a388};
                static const std::int32_t values[3] = {1, 2, 3};               // 0059D35F..0059D3A3
                value = matched_index(&text, names, values, 3, 0);
            }
            wr8(spell_at(self, at), 0x10C, static_cast<unsigned char>(value));  // 0059D3A6..0059D3AF
            string_entry(table, row, kotor_rules_str_0074a228, &text);          // 0059D3B5..0059D3F2
            assign_ref(self, at, 0x10D, &text);                                 // 0059D3F7..0059D423
            string_entry(table, row, kotor_rules_str_0074a218, &text);          // 0059D428..0059D465
            lower_in_place(&text);                                              // 0059D46A..0059D49C
            value = 0;                                                          // 0059D4AA
            {
                static const char* const names[2] = {kotor_rules_str_0074a210, kotor_rules_str_0074a208};
                static const std::int32_t values[2] = {1, 2};                  // 0059D4A1..0059D4D7
                value = matched_index(&text, names, values, 2, value);
            }
            wr8(spell_at(self, at), 0x11D, static_cast<unsigned char>(value));  // 0059D4DF..0059D4E8
        }
        int_entry(table, row, kotor_rules_str_0074a1fc, &value);                // 0059D4EE..0059D52B
        wr8(spell_at(self, at), 0x140, static_cast<unsigned char>(value));      // 0059D530..0059D545
        int_entry(table, row, kotor_rules_str_0074a1f0, &value);                // 0059D539..0059D57C
        wr32(spell_at(self, at), 0x144, static_cast<Word>(value));              // 0059D581..0059D596
        int_entry(table, row, kotor_rules_str_0074a1e0, &value);                // 0059D58A..0059D5CD
        wr32(spell_at(self, at), 0x148, value != 0 ? 1u : 0u);                  // 0059D5D2..0059D5E7 setne
        int_entry(table, row, kotor_rules_str_0074a1d8, &value);                // 0059D5E2..0059D61C
        wr16(spell_at(self, at), 0x14C, static_cast<std::uint16_t>(value));     // 0059D621..0059D62B
        int_entry(table, row, kotor_rules_str_0074a1cc, &value);                // 0059D632..0059D66F
        wr32(spell_at(self, at), 0x188, static_cast<Word>(value));              // 0059D674..0059D67D
        int_entry(table, row, kotor_rules_str_0074a1bc, &value);                // 0059D683..0059D6C0
        wr32(spell_at(self, at), 0x190, static_cast<Word>(value));              // 0059D6C5..0059D6CE
        int_entry(table, row, kotor_rules_str_0074a1ac, &value);                // 0059D6D4..0059D708
        wr32(spell_at(self, at), 0x18C, static_cast<Word>(value));              // 0059D70D..0059D716
        if (int_entry(table, row, kotor_rules_str_0074a19c, &value) != 0)       // 0059D71C..0059D762
            wr8(spell_at(self, at), 0x194, static_cast<unsigned char>(value));  // 0059D764..0059D76A
        else
            wr8(spell_at(self, at), 0x194, 0);                                  // 0059D772..0059D774
    }
    delete_c2da(table);                                                         // 0059D786..0059D78C [vtbl+0](1), no null test
}                                                                               // 0059D78E..0059D79D ~text (005E5C20)
} // namespace kotor_rules
