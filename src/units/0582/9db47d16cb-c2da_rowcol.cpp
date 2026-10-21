// lane-tlk: C2DA::GetCExoStringEntry(int row, int column, CExoString*) 00413190 and
// C2DA::GetINTEntry(int row, int column, int*) 00413510 -- the (row, column-index) getters
// CTlkTable::OpenFile needs (0041E6DF, 0041E6F0, 0041E70C, 0041E733). No existing body:
// lane-startup-misc stages the column-name variants 00413270 / 00413660 only. Same
// c2da_current::Table84 epoch and cell model as those bodies (rows [+38h], column count
// [+28h], row count [+24h], binary flag [+3Ch], blob [+40h], WORD offsets [+44h], default
// CExoString [+1Ch]).
// Evidence: dumpbin /disasm swkotor.exe 0x413190-0x413267 and 0x413510-0x41365C (dis/).
#include "6db30bdc1c-tlk_core.h"
#include "689940810d-string_priority.h"    // GuiAssign005e5140 (005E5140)
#include "c7e191db10-native_strings.h"     // WindowCStr005e5670 (005E5670)
#include "9281d0a58c-warning_runtime.h"    // warning_empty_0073d71c (0073D71C "")
#include "66a5366142-menu_targets.h"       // kotor_menu20::String::as_int_005e5630 (005E5630, M41 service-menu20-4.obj)
#include <new>

extern "C" int __cdecl kotor_sscanf_006fae08(const char*,const char*,...); // lane-font txi_readers.cpp (006FAE08)
namespace directory_list {                                                   // lane-resource-lowers (005E5600)
char __fastcall At005e5600(kotor_reconstruction::StartupString*,void*,int);
}

namespace {
using c2da_current::Table;
using String=kotor_reconstruction::StartupString;
using Word=std::uint32_t;
const char int_format_0073e2ec[]="%i";                           // 00413564 push 73E2ECh
const char hex_format_0073e2e8[]="%x";                           // 0041360B push 73E2E8h
std::int32_t signed_word(const Table* t,unsigned offset){const Word w=c2da_current::load_word(t,offset);std::int32_t v;std::memcpy(&v,&w,4);return v;}
unsigned char* bytes_at(Word address){return reinterpret_cast<unsigned char*>(static_cast<std::uintptr_t>(address));}
// [[esi+38h]+row*4]+column*8 -- the live row array and row pointer are re-read at every use.
String* cell(const Table* t,Word row,Word column){
    Word cells;std::memcpy(&cells,bytes_at(c2da_current::load_word(t,0x38)+row*4u),4);
    return std::launder(reinterpret_cast<String*>(bytes_at(cells+column*8u)));
}
// 005E5630 CExoString::AsINT on the same 8-byte CExoString object (lane-startup-misc convention).
std::int32_t as_int(String* s){return std::launder(static_cast<kotor_menu20::String*>(static_cast<void*>(s)))->as_int_005e5630();}
bool in_range(const Table* t,std::int32_t row,std::int32_t column){
    return row>=0 && row<signed_word(t,0x24) && column>=0 && column<signed_word(t,0x28); // jl/jge signed
}
}

extern "C" __declspec(noinline) std::int32_t __fastcall
c2da_current_get_string_entry_00413190(Table* self,void*,std::int32_t row,std::int32_t column,String* out){
    if(!in_range(self,row,column)){                                // 00413197..004131BB
        (void)out->Assign005e5c50(c2da_current::default_string(self)); // 00413255..0041325D (+1Ch)
        return 0;
    }
    const Word row_bits=static_cast<Word>(row),column_bits=static_cast<Word>(column);
    if(c2da_current::load_word(self,0x3c)!=0){                     // 004131C1..004131C6
        const Word at=c2da_current::load_word(self,0x28)*row_bits+column_bits;           // 004131CB..004131CE
        std::uint16_t offset;std::memcpy(&offset,bytes_at(c2da_current::load_word(self,0x44)+at*2u),2); // 004131D0..004131D3
        String* assigned=GuiAssign005e5140(out,nullptr,
            reinterpret_cast<const char*>(bytes_at(c2da_current::load_word(self,0x40)+offset))); // 004131C8, 004131DB..004131E0
        return directory_list::At005e5600(assigned,nullptr,0)!=0?1:0; // 004131E5..004131F3 (receiver = EAX)
    }
    if(cell(self,row_bits,column_bits)->BorrowedEquals005e5290(base_ini_reconstruction::warning_empty_0073d71c)!=0){ // 004131F8..00413213
        (void)GuiAssign005e5140(out,nullptr,base_ini_reconstruction::warning_empty_0073d71c); // 00413215..0041321E
        return 0;
    }
    (void)out->Assign005e5c50(cell(self,row_bits,column_bits));   // 0041322B..0041323A
    return out->BorrowedLength005e5790()>0?1:0;                   // 0041323F..0041324A (requested receiver; setg)
}

extern "C" __declspec(noinline) std::int32_t __fastcall
c2da_current_get_int_entry_00413510(Table* self,void*,std::int32_t row,std::int32_t column,std::int32_t* out){
    if(!in_range(self,row,column)){                                // 00413517..0041353B
        *out=as_int(c2da_current::default_string(self));           // 00413649..00413657
        return 0;
    }
    const Word row_bits=static_cast<Word>(row),column_bits=static_cast<Word>(column);
    if(c2da_current::load_word(self,0x3c)!=0){                     // 00413541..00413546
        const Word at=c2da_current::load_word(self,0x28)*row_bits+column_bits;           // 00413548..0041354B
        std::uint16_t offset;std::memcpy(&offset,bytes_at(c2da_current::load_word(self,0x44)+at*2u),2); // 0041354D..00413550
        *out=0;                                                    // 00413558
        (void)kotor_sscanf_006fae08(reinterpret_cast<const char*>(bytes_at(c2da_current::load_word(self,0x40)+offset)),
                                    int_format_0073e2ec,out);      // 0041355E..0041356A (result ignored)
        return *bytes_at(c2da_current::load_word(self,0x40)+offset)!=0?1:0; // 0041356F..0041357C ([+40h] reloaded)
    }
    if(cell(self,row_bits,column_bits)->BorrowedEquals005e5290(base_ini_reconstruction::warning_empty_0073d71c)!=0){ // 00413587..004135A2
        *out=0;                                                    // 004135AA
        return 0;
    }
    if(cell(self,row_bits,column_bits)->BorrowedLength005e5790()>2){ // 004135B6..004135C6 jle signed
        if(WindowCStr005e5670(cell(self,row_bits,column_bits),nullptr)[0]=='0' &&          // 004135C8..004135D8
           (WindowCStr005e5670(cell(self,row_bits,column_bits),nullptr)[1]=='x' ||         // 004135DA..004135EB
            WindowCStr005e5670(cell(self,row_bits,column_bits),nullptr)[1]=='X')){         // 004135ED..004135FE
            (void)kotor_sscanf_006fae08(WindowCStr005e5670(cell(self,row_bits,column_bits),nullptr),
                                        hex_format_0073e2e8,out);  // 00413600..00413618 (no pre-store)
            return 1;                                              // 00413622
        }
    }
    *out=as_int(cell(self,row_bits,column_bits));                  // 0041362B..0041363E
    return 1;                                                      // 00413640
}
