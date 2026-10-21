#include "e76b1c7603-getstring.h"
#include <cstring>
#include <type_traits>

namespace {
using c2da_current::String;
using c2da_current::Table;
using c2da_current::Word;

Word read_word_at(Word address) {
    Word value;std::memcpy(&value,reinterpret_cast<const void*>(address),4);
    return value;
}
std::uint16_t read_half_at(Word address) {
    std::uint16_t value;std::memcpy(&value,reinterpret_cast<const void*>(address),2);
    return value;
}
String* normal_cell(Table* table,Word row,Word column) {
    // This helper is deliberately called again after equality; the original
    // rereads both the live row array and the live row pointer at0041330A.
    const Word rows=c2da_current::load_word(table,0x38);
    const Word cells=read_word_at(rows+row*4u);
    return reinterpret_cast<String*>(cells+column*8u);
}
}

extern "C" __declspec(noinline) std::int32_t __fastcall
c2da_current_get_string_00413270(
    Table* table,void*,std::int32_t row,String* column,String* output) {
    // Column lookup is first, even when row is negative. Its providers may
    // mutate the table before the following original field reads.
    const std::int32_t index=c2da_current_column_index_00413100(table,nullptr,column);
    if(row<0 || row>=static_cast<std::int32_t>(c2da_current::load_word(table,0x24)) || index==-1) {
        (void)output->Assign005e5c50(c2da_current::default_string(table));
        return 0;
    }

    const Word row_bits=static_cast<Word>(row);
    const Word index_bits=static_cast<Word>(index);
    if(c2da_current::load_word(table,0x3c)!=0u) {
        // IMUL/ADD/scale2 are DWORD arithmetic, with zero-extended WORD offset.
        const Word at=c2da_current::load_word(table,0x28)*row_bits+index_bits;
        const Word indices=c2da_current::load_word(table,0x44);
        const Word offset=read_half_at(indices+at*2u);
        const Word text=c2da_current::load_word(table,0x40)+offset;
        String* const assigned=GuiAssign005e5140(output,nullptr,reinterpret_cast<const char*>(text));
        // Original left the pushed index0 for the second call and uses the
        // assignment-returned pointer as its receiver. Only AL is consumed.
        return c2da_current_string_index_005e5600(assigned,nullptr,0)!=0?1:0;
    }

    const char* const empty=c2da_empty_0073d71c();
    if(normal_cell(table,row_bits,index_bits)->BorrowedEquals005e5290(empty)!=0) {
        (void)GuiAssign005e5140(output,nullptr,empty);
        return 0;
    }
    (void)output->Assign005e5c50(normal_cell(table,row_bits,index_bits));
    // The original queries the requested output receiver, not assignment EAX.
    return output->BorrowedLength005e5790()>0?1:0;
}

static_assert(std::is_same<decltype(&c2da_current_get_string_00413270),
    std::int32_t(__fastcall*)(Table*,void*,std::int32_t,String*,String*)>::value,
    "canonical Table/String physical GetString carrier, no function cast");
