#include "508fcc519a-gff_views.h"

namespace kotor_q05 {
// Integer address arithmetic deliberately models LEA/add modulo 2^32, including
// unchecked returned addresses. No additional format/field-index validation.
template<class T> inline T* Address(void* base, U32 offset) {
    return reinterpret_cast<T*>(reinterpret_cast<U32>(base) + offset);
}
FieldRecord* CResGFF::GetField(StructRecord* record, U32 index) {
    if (!record || !fields || index >= record->field_count) return nullptr;
    U32 field_index = record->data_or_offset;
    if (record->field_count == 1) {
        if (field_index >= header->field_count) return nullptr;
    } else {
        const U32 offset = field_index + index * 4u;
        if (!field_indices || offset + 4u > header->indices_size) return nullptr;
        field_index = *Address<U32>(field_indices, offset);
    }



    return Address<FieldRecord>(fields, field_index * 12u);

}
FieldRecord* CResGFF::GetField(StructHandle* handle, U32 index) {
    if (!handle || !header || !structs || handle->index >= header->struct_count) return nullptr;
    StructRecord* record = Address<StructRecord>(structs, handle->index * 12u);
    if (!record || !fields || index >= record->field_count) return nullptr;
    U32 field_index = record->data_or_offset;
    if (record->field_count == 1) {
        if (field_index >= header->field_count) return nullptr;
    } else {
        if (!field_indices || field_index + index * 4u + 4u > header->indices_size) return nullptr;
        field_index = *Address<U32>(field_indices, field_index + index * 4u);
    }



    return Address<FieldRecord>(fields, field_index * 12u);

}
U32 CResGFF::GetDataField(FieldRecord* field, U32* size) {
    if (!field || !field_data || field->data_or_offset >= header->data_size) return 0;
    *size = header->data_size - field->data_or_offset;



    return reinterpret_cast<U32>(field_data) + field->data_or_offset;

}
U32 CResGFF::GetDataLayoutList(FieldRecord* field, U32* size) {
    if (!field || !list_indices || field->data_or_offset >= header->list_size) return 0;
    *size = header->list_size - field->data_or_offset;



    return reinterpret_cast<U32>(list_indices) + field->data_or_offset;

}
U32 CResGFF::GetElementType(StructHandle* handle) {
    if (!handle || !header || !structs || handle->index >= header->struct_count) return 0xffffffffu;
    StructRecord* record = Address<StructRecord>(structs, handle->index * 12u);



    return record ? record->type : 0xffffffffu;

}
U32 CResGFF::GetFieldCount(StructHandle* handle) {
    if (!handle || !header || !structs || handle->index >= header->struct_count) return 0xffffffffu;
    StructRecord* record = Address<StructRecord>(structs, handle->index * 12u);



    return record ? record->field_count : 0xffffffffu;

}
U32 CResGFF::GetTopLevelStruct(StructHandle* handle) {
    if (!handle) return 0;



    handle->index = 0;

    return 1;
}
U32 CResGFF::GetTotalSize() {
    if (!header) return 0;
    U32 total = (header->struct_count + header->field_count) * 12u;
    total += header->label_count * 16u + header->list_size + header->indices_size + header->data_size;



    return total + 0x38u;

}
}
