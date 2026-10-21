// lane-tlk: CTlkFile::CTlkFile 0041D810, CTlkFile::ReadHeader 0041D890, CTlkTable::AddFile 0041D920.
// Evidence: dumpbin /disasm swkotor.exe 0x41D800-0x41DB50.
#include "6db30bdc1c-tlk_core.h"
#include "3988cffdb6-internal.h"           // file_internal::exofile_construct_005e68d0 / seek_beginning / read
#include "fcb874b170-directory_data.h"     // c2da_directory_literal_0073d5b0 ("rb")
#include "689940810d-string_priority.h"    // GuiConcat005e5d10 (005E5D10)
#include "9281d0a58c-warning_runtime.h"    // warning_empty_0073d71c
#include "7b8c81fcff-allocator_live.h"     // live_operator_new (006FA7E6), live_free (006FA390)
#include <new>

namespace {
using namespace tlk_core;
const char female_suffix_0073ece0[]="F";                       // 0041DA4C push 73ECE0h
TlkFile* male_file(Tlk* self,int slot){return get<TlkFile*>(self,4u+8u*static_cast<unsigned>(slot));}  // [edi+esi*8+4]
TlkFile* female_file(Tlk* self,int slot){return get<TlkFile*>(self,8u+8u*static_cast<unsigned>(slot));}// [edi+esi*8+8]
void set_male(Tlk* self,int slot,TlkFile* f){put<TlkFile*>(self,4u+8u*static_cast<unsigned>(slot),f);}
void set_female(Tlk* self,int slot,TlkFile* f){put<TlkFile*>(self,8u+8u*static_cast<unsigned>(slot),f);}
// CTlkFile* -> its CExoFile base (first member, standard layout: pointer-interconvertible,
// null stays null so the original null IsOpen fault domain is preserved without UB here).
tlk_file::File* exofile(TlkFile* file){return reinterpret_cast<tlk_file::File*>(file);}
static_assert(std::is_standard_layout<TlkFile>::value && offsetof(TlkFile,file)==0,"CExoFile base at +0");
// delete CTlkFile: ~CExoFile 005E69E0 then 006FA390 (0041D98D..0041D995 and every later copy).
void delete_file(TlkFile* file){
    if(file){tlk_file::file_destroy_005e69e0(exofile(file),nullptr);live_free(file);}
}
// new CTlkFile(name): 006FA7E6(18h); EH state frees the block if the ctor throws
// (0041D9C3 state 0 / 0041DA42 state 1); null allocation yields null (0041D9DB / 0041DA91).
}

namespace tlk_core {
TlkFile* __fastcall ConstructFile0041d810(TlkFile* self,void*,String* name) {
    {
        String mode(c2da_directory_literal_0073d5b0);          // 0041D82B..0041D838 CExoString("rb") (EH state 0)
        (void)file_internal::exofile_construct_005e68d0(&self->file,nullptr,name,0x7e2u,&mode); // 0041D83D..0041D856
    }                                                          // 0041D864 ~CExoString
    for(Word& word:self->header)word=0;                        // 0041D869..0041D87D [esi+4..+14h]=0
    return self;                                               // 0041D880 ; ret 4
}

Word __fastcall ReadHeader0041d890(TlkFile* self,void*) {
    (void)file_internal::exofile_seek_beginning_005e6ad0(&self->file,nullptr); // 0041D894
    if(file_internal::exofile_read_005e6960(&self->file,nullptr,self->header,1,0x14)!=0x14u)return 0; // 0041D899..0041D8B1
    return self->header[0]==0x204b4c54u?1u:0u;                 // 0041D8B2..0041D8BD 'TLK '
}

Word __fastcall AddFile0041d920(Tlk* self,void*,String* name,int slot) {
    if(slot<0||slot>7)return 0;                                // 0041D945 jl / 0041D950 cmp 7; jg
    if(name->BorrowedEquals005e5290(base_ini_reconstruction::warning_empty_0073d71c))return 0; // 0041D959..0041D969
    if(female_file(self,slot)==male_file(self,slot))set_female(self,slot,nullptr); // 0041D96F..0041D983
    delete_file(female_file(self,slot));                       // 0041D986..0041D99A
    delete_file(male_file(self,slot));                         // 0041D99D..0041D9B0 (slot words left as-is)
    {
        void* memory=live_operator_new(0x18);                  // 0041D9B3..0041D9B5
        TlkFile* male=nullptr;
        if(memory){
            try{male=ConstructFile0041d810(::new(memory) TlkFile,nullptr,name);} // 0041D9CD..0041D9D4 (TlkFile is trivial: no init)
            catch(...){live_free(memory);throw;}               // EH state 0 (new-expression cleanup)
        }
        set_male(self,slot,male);                              // 0041D9E7
    }
    // 0041D9DD..0041D9E9: IsOpen is called on the stored value even when it is null (original fault domain).
    if(!tlk_file::file_opened_005e6a10(exofile(male_file(self,slot)),nullptr))return 0; // 0041D9E9..0041D9F0 (male kept)
    if(!ReadHeader0041d890(male_file(self,slot),nullptr)){     // 0041D9F2..0041D9FB
        delete_file(male_file(self,slot));                     // 0041D9FD..0041DA10
        set_male(self,slot,nullptr);                           // 0041DA13
        return 0;
    }
    {
        void* memory=live_operator_new(0x18);                  // 0041DA30..0041DA32
        if(memory){
            try{
                String suffix(female_suffix_0073ece0);         // 0041DA4C..0041DA55 (flag bit0)
                alignas(String) unsigned char sum_storage[sizeof(String)];
                String* sum=GuiConcat005e5d10(name,nullptr,reinterpret_cast<String*>(sum_storage),&suffix); // 0041DA5A..0041DA75 (flag bit1)
                struct Sum{String* s;~Sum(){s->~StartupString();}} sum_lifetime{sum}; // 0041DAA3..0041DAAE (bit1 first)
                set_female(self,slot,ConstructFile0041d810(::new(memory) TlkFile,nullptr,sum)); // 0041DA7F..0041DA96
            }                                                  // sum then suffix destroyed (0041DAAE, 0041DAC4)
            catch(...){live_free(memory);throw;}               // EH state 1 (new-expression cleanup)
        }else{
            set_female(self,slot,nullptr);                     // 0041DA91 xor eax,eax; 0041DA96
        }
    }
    if(!tlk_file::file_opened_005e6a10(exofile(female_file(self,slot)),nullptr)){ // 0041DAC9..0041DAD3
        delete_file(female_file(self,slot));                   // 0041DAD5..0041DAE9
        set_female(self,slot,male_file(self,slot));            // 0041DAEC..0041DAEE female := male
        return 1;                                              // 0041DAF1
    }
    if(ReadHeader0041d890(female_file(self,slot),nullptr))return 1; // 0041DAFB..0041DB05
    delete_file(male_file(self,slot));                         // 0041DB07..0041DB1A
    set_male(self,slot,nullptr);                               // 0041DB22
    delete_file(female_file(self,slot));                       // 0041DB1D..0041DB37
    set_female(self,slot,nullptr);                             // 0041DB3A
    return 0;                                                  // 0041DA19
}
}
