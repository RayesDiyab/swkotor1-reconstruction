#include "92e8e37443-gff_menu_lists.h"
#include <cstring>

namespace {
using namespace task17_gff;
template<class T> T* address(void* base, Word offset) {
    return reinterpret_cast<T*>(reinterpret_cast<Word>(base) + offset);
}
Word read_word(void* base, Word offset) {
    return *address<volatile Word>(base, offset);
}
}

Word __fastcall gff_label_00411630(Gff* self, void*, Struct* handle, const char* label) {
    if (!label || !self->labels) return 0xffffffffu;
    Word copied[4] = {};
    // Original 006FAB80 is the Lane single-match cdecl Visual Studio strncpy.
    // Same /MT CRT support is already the Main6 provider for this dependency.
    std::strncpy(reinterpret_cast<char*>(copied), label, 16);
    kotor_q05::StructRecord* record = nullptr;
    if (handle && self->header && self->structs && handle->index < self->header->struct_count)
        record = address<kotor_q05::StructRecord>(self->structs, handle->index * 12u);
    const Word count = record ? record->field_count : 0xffffffffu;
    if (!count) return 0xffffffffu;
    for (Word index = 0; index < count; ++index) {
        auto* field = self->GetField(record, index); // Existing 00410930 donor.
        if (!field || field->label >= self->header->label_count) return 0xffffffffu;
        void* candidate = address<void>(self->labels, field->label << 4);



        if (copied[0] == read_word(candidate, 0) && copied[1] == read_word(candidate, 4) &&
            copied[2] == read_word(candidate, 8) && copied[3] == read_word(candidate, 12)) return index;

    }
    return 0xffffffffu;
}

Word __fastcall gff_list_004118c0(Gff* self, void*, List* output, Struct* handle, const char* label) {
    if (!output || !handle || !label) return 0;



    const Word index = gff_label_00411630(self, nullptr, handle, label);
    auto* field = self->GetField(handle, index);
    if (!field || field->type != 15u) return 0;
    Word remaining;
    const Word data = self->GetDataLayoutList(field, &remaining);



    if (!data || remaining < 4u) return 0;

    // No list count/data dereference here. Original stores handle first, then
    // copies/pads exactly 16 bytes; it adds no seventeenth-byte terminator.
    output->record.index = handle->index;
    std::strncpy(output->label, label, 16);
    return 1;
}

Word __fastcall gff_list_count_00411940(Gff* self, void*, List* list) {
    if (!list) return 0;
    const Word index = gff_label_00411630(self, nullptr, &list->record, list->label);
    auto* field = self->GetField(&list->record, index); // Existing 00410990 donor.
    if (!field || field->type != 15u) return 0;
    Word remaining;
    const Word data = self->GetDataLayoutList(field, &remaining); // Existing 00410A60.
    if (!data || remaining < 4u) return 0;
    const Word count = read_word(reinterpret_cast<void*>(data), 0);



    return count;

}

Word __fastcall gff_list_element_00411990(Gff* self, void*, Struct* output, List* list, Word index) {
    if (!output || !list) return 0;
    const Word field_index = gff_label_00411630(self, nullptr, &list->record, list->label);
    auto* field = self->GetField(&list->record, field_index);
    if (!field || field->type != 15u) return 0;
    Word remaining;
    const Word data_bits = self->GetDataLayoutList(field, &remaining);
    if (!data_bits || remaining < index * 4u + 8u) return 0;
    void* data = reinterpret_cast<void*>(data_bits);
    const Word count = read_word(data, 0);



    // The original MOV of entry occurs after CMP but before its JAE. Preserve
    // this observable read, including an out-of-count index with a valid span.
    const Word entry = read_word(data, index * 4u + 4u);
    if (index >= count || entry >= self->header->struct_count) return 0;
    output->index = entry;



    return 1;

}
