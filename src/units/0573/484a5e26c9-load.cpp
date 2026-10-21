#include "dd2c538ad1-load_imports.h"
#include "689940810d-string_priority.h"
#include "7b8c81fcff-allocator_live.h"
#include "e91fbdadb7-baseinternal.h"
#include <cstring>
#include <type_traits>
using namespace c2da_current;
static_assert(std::is_same<base_resource_table::String,String>::value,"reuse actual canonical vector lifetime owner");
static_assert(sizeof(String)==8 && sizeof(Word)==4,"native array cookies and elements");
namespace {
bool positive(Word v) {return static_cast<std::int32_t>(v)>0;}
bool signed_less(Word a,Word b) {return static_cast<std::int32_t>(a)<static_cast<std::int32_t>(b);}
Word line_prefix(char* data,Word size) {
    Word n=0;while(n<size && data[n]!='\n' && data[n]!='\r')++n;return n;
}
bool equal_bytes(const char* a,const char* b,unsigned n) {
    for(unsigned i=0;i<n;++i)if(static_cast<unsigned char>(a[i])!=static_cast<unsigned char>(b[i]))return false;
    return true;
}
String* array_element(String* base,Word byte_offset) {
    return std::launder(reinterpret_cast<String*>(reinterpret_cast<unsigned char*>(base)+byte_offset));
}
String* make_strings(Word count) {
    void* raw=live_operator_new(count*8u+4u);
    if(!raw)return nullptr;
    std::memcpy(raw,&count,4);
    auto* result=reinterpret_cast<String*>(static_cast<unsigned char*>(raw)+4);
    base_resource_table::ConstructStringVectorNormal(result,8,static_cast<std::int32_t>(count),
        &base_resource_table::ConstructString,&base_resource_table::DestroyString);
    return result;
}
void store_row(Table* self,Word index,String* value) {
    auto* root=load_pointer<unsigned char>(self,0x38);std::memcpy(root+index*4u,&value,4);
}
String* load_row(Table* self,Word byte_offset) {
    String* row;auto* root=load_pointer<unsigned char>(self,0x38);std::memcpy(&row,root+byte_offset,4);return row;
}
void store_short(unsigned char* root,Word index,Word offset) {
    const auto value=static_cast<std::uint16_t>(offset);std::memcpy(root+index*2u,&value,2);
}
Word pool_label(char* pool,Word offset) {
    while(pool[offset]!='\t' && pool[offset]!=0)++offset;
    pool[offset]=0;return offset+1u;
}
int failed(Table* self) {
    store_word(self,0x2c,1);c2da_current_unload_004139e0(self,nullptr);return 0;
}
}

// Normal serial nonthrowing source reconstruction. Parser/resource/literal
// providers are genuine imports; no parser/IO or publication is supplied here.
extern "C" __declspec(noinline) int __fastcall
c2da_current_load_004143b0(Table* self,void*) {
    c2da_current_unload_004139e0(self,nullptr);
    if(!c2da_current_resource_demand_00409b10(self->helper.resource,nullptr))return 0;
    char* data=static_cast<char*>(c2da_current_resource_data_00404dd0(self->helper.resource,nullptr));
    Word size=c2da_current_resource_size_00710e50(self->helper.resource,nullptr);
    Word mode;std::memcpy(&mode,static_cast<const unsigned char*>(self->helper.resource)+0x38,4);
    store_word(self,0x3c,mode);
    (void)c2da_current_skip_lines_00413940(self,nullptr,&data,&size);
    char* saved_data=data;
    const Word header_size=line_prefix(data,size);
    Word token_remaining=header_size,token_size;
    char token[256];
    (void)c2da_current_token_004137e0(self,nullptr,&saved_data,&token_remaining,token,&token_size);
    (void)c2da_current_strupr_007136f3(token);
    if(equal_bytes(token,c2da_load_literal_0073e30c(),9)) {
        (void)c2da_current_token_004137e0(self,nullptr,&saved_data,&token_remaining,token,&token_size);
        (void)GuiAssign005e5140(default_string(self),nullptr,token);
        data=saved_data;size-=header_size;
    } else if(equal_bytes(token,c2da_load_literal_0073e304(),8)) {
        (void)c2da_current_token_004137e0(self,nullptr,&saved_data,&token_remaining,token,&token_size);
        const char* text=token;
        if(token[0]==':') {
            const char* end=token+1;while(*end)++end;
            if(static_cast<Word>(end-(token+1))>=1u)text=token+1;
            else {
                (void)c2da_current_token_004137e0(self,nullptr,&saved_data,&token_remaining,token,&token_size);
                text=token;
            }
        }
        (void)GuiAssign005e5140(default_string(self),nullptr,text);
        // Original DEFAULT-without-colon path subtracts the full header size
        // without replacing the main data pointer with saved_data.
        size-=header_size;
    }
    (void)c2da_current_skip_lines_00413940(self,nullptr,&data,&size);
    saved_data=data;store_word(self,0x28,0);
    token_remaining=line_prefix(data,size);
    int got=c2da_current_token_004137e0(self,nullptr,&saved_data,&token_remaining,token,&token_size);
    while(got) {
        store_word(self,0x28,load_word(self,0x28)+1u);
        got=c2da_current_token_004137e0(self,nullptr,&saved_data,&token_remaining,token,&token_size);
    }
    const Word columns=load_word(self,0x28);
    if(!positive(columns))return failed(self);
    if(load_word(self,0x3c)) {
        char* pool=data;store_word(self,0x48,reinterpret_cast<Word>(pool));
        void* allocated=live_operator_new(columns*2u);store_word(self,0x50,reinterpret_cast<Word>(allocated));
        Word offset=0,index=0;
        while(signed_less(index,load_word(self,0x28))) {
            store_short(load_pointer<unsigned char>(self,0x50),index,offset);
            offset=pool_label(pool,offset);++index;
        }
        Word rows;std::memcpy(&rows,pool+offset+1u,4);++offset;store_word(self,0x24,rows);offset+=4u;
        allocated=live_operator_new(rows*2u);store_word(self,0x4c,reinterpret_cast<Word>(allocated));
        index=0;
        while(signed_less(index,load_word(self,0x24))) {
            store_short(load_pointer<unsigned char>(self,0x4c),index,offset);
            offset=pool_label(pool,offset);++index;
        }
        char* cells=pool+offset;
        const Word product=load_word(self,0x28)*load_word(self,0x24);
        store_word(self,0x44,reinterpret_cast<Word>(cells));
        store_word(self,0x40,reinterpret_cast<Word>(cells)+product*2u+2u);
        store_word(self,0x2c,1);return 1; // packed data retains the demand
    }
    String* labels=make_strings(columns);store_word(self,0x34,reinterpret_cast<Word>(labels));
    if(!labels)return failed(self);
    got=c2da_current_token_004137e0(self,nullptr,&data,&size,token,&token_size);
    Word label_offset=0;
    while(got) {
        char* lower=c2da_current_strlwr_007135df(token);
        (void)GuiAssign005e5140(array_element(load_pointer<String>(self,0x34),label_offset),nullptr,lower);
        label_offset+=8u;
        got=c2da_current_token_004137e0(self,nullptr,&data,&size,token,&token_size);
    }
    (void)c2da_current_skip_lines_00413940(self,nullptr,&data,&size);
    const Word saved_remaining=size;saved_data=data;store_word(self,0x24,0);
    while(size) {
        const Word consumed=c2da_current_line_end_004137b0(self,nullptr,data,size);
        data+=consumed;size-=consumed;
        (void)c2da_current_skip_lines_00413940(self,nullptr,&data,&size);
        store_word(self,0x24,load_word(self,0x24)+1u);
    }
    const Word rows=load_word(self,0x24);data=saved_data;size=saved_remaining;
    if(!rows) {
        store_word(self,0x2c,1);c2da_release_demand_00409b80(self->helper.resource,nullptr);return 1;
    }
    labels=make_strings(rows);store_word(self,0x30,reinterpret_cast<Word>(labels));
    if(!labels)return failed(self);
    void* row_storage=live_operator_new(load_word(self,0x24)*4u);
    store_word(self,0x38,reinterpret_cast<Word>(row_storage));
    // Original tests row-label field+30, rather than the new allocation+38.
    if(!load_pointer<String>(self,0x30))return failed(self);
    Word row=0;
    while(signed_less(row,load_word(self,0x24))) {
        String* values=make_strings(load_word(self,0x28));store_row(self,row,values);
        if(!load_row(self,row*4u))return failed(self);
        ++row;
    }
    if(saved_remaining) {
        Word row_offset=0,label_byte_offset=0;
        do {
            const Word line_size=c2da_current_line_end_004137b0(self,nullptr,data,size);
            char* row_data=data;Word row_remaining=size;
            (void)c2da_current_token_004137e0(self,nullptr,&row_data,&row_remaining,token,&token_size);
            char* lower=c2da_current_strlwr_007135df(token);
            (void)GuiAssign005e5140(array_element(load_pointer<String>(self,0x30),label_byte_offset),nullptr,lower);
            Word column=0;
            while(signed_less(column,load_word(self,0x28))) {
                got=c2da_current_token_004137e0(self,nullptr,&row_data,&row_remaining,token,&token_size);
                if(got) {
                    auto* captured_rows=load_pointer<unsigned char>(self,0x38);
                    const bool missing=equal_bytes(token,c2da_load_literal_0073e2fc(),5);
                    String* values;std::memcpy(&values,captured_rows+row_offset,4);
                    (void)GuiAssign005e5140(array_element(values,column*8u),nullptr,missing?c2da_empty_0073d71c():token);
                } else {
                    String* values=load_row(self,row_offset);
                    (void)array_element(values,column*8u)->Assign005e5c50(default_string(self));
                }
                ++column;
            }
            data+=line_size;size-=line_size;
            (void)c2da_current_skip_lines_00413940(self,nullptr,&data,&size);
            row_offset+=4u;label_byte_offset+=8u;
        } while(size);
    }
    c2da_release_demand_00409b80(self->helper.resource,nullptr);
    store_word(self,0x2c,1);return 1;
}
