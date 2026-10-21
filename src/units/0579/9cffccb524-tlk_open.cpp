// lane-tlk: CTlkTable::OpenFile 0041E5A0, hash 00410560, token row allocation, and the
// StartServices router entry for call site 005F8D43.
// Evidence: dumpbin /disasm swkotor.exe 0x41E5A0-0x41E8E6, 0x410560-0x410597, 0x59EA20, 0x41D910.
#include "6db30bdc1c-tlk_core.h"
#include "c7e191db10-native_strings.h"     // WindowCStr005e5670 (005E5670)
#include "7b8c81fcff-allocator_live.h"     // live_operator_new (006FA7E6), live_free (006FA390)
#include "8ae0e4efda-bridges.h"            // client_services_start::Call, kotor_menu_service_contract_violation

namespace tlk_core {
void* Token::operator new[](std::size_t bytes) noexcept {      // 0041E67B call 006FA7E6 (null tested at 0041E687)
    return live_operator_new(static_cast<std::uint32_t>(bytes));
}
void Token::operator delete[](void* cookie) noexcept {         // 0041E5F2 call 006FA390 on [eax-4]
    live_free(cookie);
}

Word __cdecl Hash00410560(const char* text,Word length) {
    Word hash=0;                                               // 00410565 xor eax,eax
    for(Word i=0;i<length;++i){                                // 00410567..0041056B, 0041058D..00410592 (unsigned)
        const unsigned char c=static_cast<unsigned char>(text[i]); // 00410574
        if(!c)break;                                           // 00410577
        hash=kotor_gff_crc_table_007a4178[(c^hash)&0xffu]^(hash>>8); // 0041057B..0041058E
    }
    return hash;
}
}

namespace {
using namespace tlk_core;
const char tokens_resource_0073ed30[]="StringTokens";          // 0041E60D push 73ED30h
// Token row address from the +40h word (re-read for every access, as the original does
// at 0041E6D0/0041E6F5/0041E711/0041E73C/...). Integer arithmetic keeps the original
// null-array fault domain (allocation failure at 0041E6AD) free of C++ pointer UB.
Token* row_at(Tlk* self,Word row){return reinterpret_cast<Token*>(get<Word>(self,0x40)+row*0x24u);}
// The C2DA local [esp+48h] (0x54 bytes = c2da_current::Table84); ~C2DA 00413D80 runs on every
// exit (EH state 1). Constructor/Load/destructor are the task-10 donor bodies staged by
// lane-startup-misc (identical addresses 00413CC0 / 004143B0 / 00413D80).
struct TableLocal {
    alignas(c2da_current::Table) unsigned char storage[sizeof(c2da_current::Table)];
    c2da_current::Table* table;
    TableLocal(){                                              // 0041E603..0041E622
        Ref name;                                              // outgoing by-value CResRef slot (sub esp,10h)
        (void)name.construct_00406d80(tokens_resource_0073ed30); // 0041E607..0041E619
        table=c2da_current_construct_00413cc0(storage,nullptr,name,0); // 0041E603 push 0; 0041E622 (EAX = this)
    }
    ~TableLocal(){c2da_current_destroy_00413d80(table,nullptr);} // 0041E647 / 0041E8AD
    TableLocal(const TableLocal&)=delete;TableLocal& operator=(const TableLocal&)=delete;
};
static_assert(sizeof(c2da_current::Table)==0x54,"C2DA local 54h");
}


namespace tlk_core {
Word __fastcall OpenFile0041e5a0(Tlk* self,void*,String* name) {
    if(!AddFile0041d920(self,nullptr,name,0))return 0;        // 0041E5C3..0041E5D0, 0041E660
    if(Token* previous=get<Token*>(self,0x40))delete[] previous; // 0041E5D6..0041E5F7 (+40h not cleared here)
    String temporary;                                          // [esp+28h] 0041E5FA (EH state 0)
    TableLocal table;                                          // [esp+48h] (EH state 1)
    if(!c2da_current_load_004143b0(table.table,nullptr))return 0; // 0041E627..0041E63A; 0041E647, 0041E65B
    const Word count=c2da_current::load_word(table.table,0x24);              // 0041E667 [esp+6Ch]
    put<Word>(self,0x44,count);                                // 0041E66E
    Token* tokens=new Token[count];                            // 0041E66B..0041E6AD (cookie=count, ctor 0041D910)
    put<Token*>(self,0x40,tokens);                             // 0041E6BA
    // [esp+14h] is reused across rows without reset and has no initializing store before
    // its first read at 0041E6F8 if 00413510 does not write it; value-initialized to 0 as a
    // documented D2-style deviation (no faithful value exists for an unwritten stack word).
    std::int32_t value=0;
    for(Word row=0;row<get<Word>(self,0x44);++row){            // 0041E6AF..0041E6C1, 0041E88A..0041E89B
        (void)c2da_current_get_string_entry_00413190(table.table,nullptr,static_cast<std::int32_t>(row),0,&row_at(self,row)->text); // 0041E6D0..0041E6DF
        (void)c2da_current_get_int_entry_00413510(table.table,nullptr,static_cast<std::int32_t>(row),1,&value); // 0041E6E4..0041E6F0
        row_at(self,row)->column1=static_cast<Word>(value);   // 0041E6F5..0041E703
        (void)c2da_current_get_int_entry_00413510(table.table,nullptr,static_cast<std::int32_t>(row),2,&value); // 0041E6FC..0041E70C
        row_at(self,row)->column2=static_cast<Word>(value);   // 0041E711..0041E71B
        for(Word column=0;column<4;++column){                  // 0041E71F..0041E751
            (void)c2da_current_get_int_entry_00413510(table.table,nullptr,static_cast<std::int32_t>(row),static_cast<std::int32_t>(column+3),&value); // 0041E725..0041E733
            row_at(self,row)->columns3[column]=static_cast<Word>(value); // 0041E738..0041E743
        }
        const std::int32_t length=row_at(self,row)->text.BorrowedLength005e5790(); // 0041E753..0041E75A
        const char* text=WindowCStr005e5670(&row_at(self,row)->text,nullptr);    // 0041E75F..0041E767
        row_at(self,row)->hash=Hash00410560(text,static_cast<Word>(length));     // 0041E76C..0041E77A
        // Insertion step toward the front, unsigned hash order (0041E77D..0041E884).
        Word remaining=row;Word position=row;
        if(row){                                               // 0041E778 test edi,edi; jbe
            do{
                if(row_at(self,position)->hash>=row_at(self,position-1)->hash)break; // 0041E790..0041E7A2 jae
                const Word saved_hash=row_at(self,position-1)->hash;              // 0041E79D/0041E7B0
                temporary.Assign005e5c50(&row_at(self,position-1)->text);        // 0041E7A8..0041E7B4
                const Word saved_column1=row_at(self,position-1)->column1;       // 0041E7B9..0041E7BC
                Word saved_columns3[4];
                for(Word i=0;i<4;++i)saved_columns3[i]=row_at(self,position-1)->columns3[i]; // 0041E7C0..0041E7D2
                const Word saved_column2=row_at(self,position-1)->column2;       // 0041E7D4..0041E7E1
                Token* current=row_at(self,position);Token* previous=row_at(self,position-1); // 0041E7D7..0041E7EA
                previous->hash=current->hash;                  // 0041E7E5..0041E7F1
                previous->text.Assign005e5c50(&current->text); // 0041E7E7..0041E7F3
                previous->column1=current->column1;            // 0041E7F8..0041E7FD
                for(Word i=0;i<4;++i)previous->columns3[i]=current->columns3[i]; // 0041E800..0041E819
                previous->column2=current->column2;            // 0041E81B..0041E822
                current=row_at(self,position);                 // 0041E825..0041E828
                current->hash=saved_hash;                      // 0041E82A..0041E832
                current->text.Assign005e5c50(&temporary);      // 0041E82E..0041E838
                current->column1=saved_column1;                // 0041E83D..0041E841
                for(Word i=0;i<4;++i)current->columns3[i]=saved_columns3[i]; // 0041E844..0041E85D
                current->column2=saved_column2;                // 0041E85F..0041E86F
                --remaining;--position;                        // 0041E872..0041E880
            }while(remaining);                                 // 0041E884 ja
        }
    }
    return 1;                                                  // 0041E8A1..0041E8C6 (~C2DA, ~CExoString)
}
}

// Router entry for StartServices 005F8D43: ECX=[007A3A08] (005F8D3F), one stack word
// &dialog (CExoString), thiscall. Shape is checked here (no shape.cpp row exists).
extern "C" client_services_start::Word __cdecl kotor_menu_service_0041E5A0(const client_services_start::Call* call) {
    if(!call||call->site!=0x005f8d43u||call->target!=0x0041e5a0u||
       call->abi!=client_services_start::Abi::Thiscall||call->count!=1u||!call->self)
        kotor_menu_service_contract_violation(call);
    return tlk_core::OpenFile0041e5a0(reinterpret_cast<tlk_core::Tlk*>(call->self),nullptr,
                                      reinterpret_cast<tlk_core::String*>(call->args[0]));
}
